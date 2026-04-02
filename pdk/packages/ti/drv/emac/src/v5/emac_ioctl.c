/**
 *  \file   emac_ioctl.c
 *
 *  \brief  emac driver IOCTL implementaion for ICSSG IP
 *
*  This file contains helper functions to implement emac_ioctl API for ICSSG subsystem
 */

/*
 * Copyright (C) 2019-2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>

#include <ti/csl/soc.h>

#include "ti/csl/cslr_icss.h"

#include <ti/csl/csl_cpswitch.h>
#include <ti/drv/emac/emac_drv.h>
#include <ti/drv/emac/src/emac_osal.h>
#include <ti/drv/emac/soc/emac_soc_v5.h>
#include "ti/drv/emac/src/v5/emac_drv_v5.h"
#include "ti/drv/emac/src/v5/emac_utils.h"
#include <ti/drv/emac/src/v5/emac_hwq.h>
#include <ti/drv/emac/emac_ioctl.h>
#include <ti/drv/emac/firmware/icss_dualmac/bin_pg2/fw_mem_map.h>

extern EMAC_MCB_V5_T      emac_mcb;
extern EMAC_PORT_INFO_T emac_port_info[];

extern int32_t
emac_udma_ring_dequeue
(
    Udma_RingHandle ringHandle,
    EMAC_CPPI_DESC_T     **pHostDescriptor
);

extern uint16_t emac_util_fdb_helper(uintptr_t vlan_table, uint16_t vlan_id,  uint8_t mac[], uint8_t * p_fid);

#if defined (EMAC_INCOHERENT)
EMAC_IOCTL_CMD_T cmd1Icssg __attribute__ ((aligned (EMAC_CACHELINE_ALIGNMENT)));
EMAC_IOCTL_CMD_T cmd2Icssg __attribute__ ((aligned (EMAC_CACHELINE_ALIGNMENT)));
#else
EMAC_IOCTL_CMD_T cmd1Icssg;
EMAC_IOCTL_CMD_T cmd2Icssg;
#endif

uintptr_t emac_get_vlan_tbl_addr(uint32_t portNum)
{
    return (emac_mcb.port_cb[portNum].icssSharedRamBaseAddr + EMAC_ICSSG_SWITCH_DEFAULT_VLAN_TABLE_OFFSET);
}

void emac_get_vlan_id(uint32_t portNum, EMAC_IOCTL_FDB_ENTRY *pEntry)
{
    uintptr_t regAddr;

    if (pEntry->vlanId == EMAC_VLAN_UNTAGGED)
    {
        /* if EMAC_VLAN_UNTAGGED, get default vlanId */
        regAddr = emac_mcb.port_cb[portNum].icssgCfgRegBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FDB_DF_VLAN + 2*portNum;
        pEntry->vlanId = CSL_REG16_RD (regAddr);
    }
}

void emac_update_cmd(uint32_t portNum, uint32_t ioctlCmd, EMAC_IOCTL_CMD_T *pCmd,EMAC_IOCTL_PARAMS *pParams, EMAC_IOCTL_FDB_ENTRY *pEntry, uint32_t cmdType, uint16_t broadSideSlot, uint8_t fid)
{
    memset(pCmd, 0, sizeof(EMAC_IOCTL_CMD_T));

    pCmd->commandHeader = EMAC_FW_MGMT_CMD_HEADER;
    pCmd->commandType = cmdType;
    pCmd->seqNumber = pParams->seqNumber;
    pCmd->commandParam |= pParams->subCommand;
    uint32_t portLoc = 0;
    if((portNum  & 1) == 1)
    {
        portLoc = 1;
        pCmd->commandParam |= (1 << 4);
    }
    if (ioctlCmd == EMAC_IOCTL_FDB_ENTRY_CTRL)
    {
        switch (pParams->subCommand)
        {
            case EMAC_IOCTL_FDB_ENTRY_ADD:
            case EMAC_IOCTL_FDB_ENTRY_DELETE:
                if(pEntry)
                {
                    memcpy((void*)(&pCmd->spare[0]), (void*)(&pEntry->mac[0]), 4);
                    memcpy((void*)(&pCmd->spare[1]), (void*)(&pEntry->mac[4]), 2);
                    pCmd->spare[2] = broadSideSlot;
                    pCmd->spare[1] = pCmd->spare[1] | (fid << 16);
                    pCmd->spare[1] = pCmd->spare[1] | (pEntry->fdbEntry[portLoc]<< 24);
                }
                break;
            case EMAC_IOCTL_FDB_ENTRY_DELETE_ALL:
                pCmd->spare[0] = 0x0;
                break;
            case EMAC_IOCTL_FDB_ENTRY_DELETE_ALL_AGEABLE:
                pCmd->spare[0] = 0x000000008;
                break;
            default:
                break;
        }
    }
}

EMAC_DRV_ERR_E  emac_ioctl_send_mgmt_msg(uint32_t portNum, EMAC_IOCTL_CMD_T* pMgmtCmd, EMAC_CPPI_DESC_T* p_tx_cppi_desc);

/*
 *  ======== emac_ioctl_icss_add_mac ========
 */
void emac_ioctl_icss_add_mac (uint32_t portNum, uint8_t*  macAddr)
{
    int32_t macLo;
    int32_t macHi;
    int16_t temp;
    uintptr_t baseAddr;

    baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE;
    macLo =  *((int32_t *) &macAddr[0]);
    temp =  *((int16_t *) &macAddr[4]);
    macHi = (int32_t) temp;
    if ((portNum % 2U) == 0)
    {
        /* add mac */
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_MAC_PRU0_0, macLo);
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_MAC_PRU0_1, macHi);
        /* enable filter */
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS4_AND_EN_PRU0, 0x0U);
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS4_OR_EN_PRU0, 0x22000000U);
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG1_PRU0, 0x300U);
    }
    else
    {
        /* add mac */
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_MAC_PRU1_0, macLo);
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_MAC_PRU1_1, macHi);
        /* enable filter */
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS4_AND_EN_PRU1, 0x0U);
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS4_OR_EN_PRU1, 0x22000000U);
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG1_PRU1, 0x300U);
    }
}
/*
 *  ======== emac_ioctl_icss_promiscous_ctrl ========
 */
EMAC_DRV_ERR_E emac_ioctl_icss_promiscous_ctrl(uint32_t portNum, void*  ctrl)
{
    uintptr_t baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE;
    if(1U == *((uint32_t*)ctrl))
    {
        if ((portNum % 2U) == 0U)
        {
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_START_LEN_PRU0, 0x60000U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_CFG_PRU0, 0x5555U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA0_PRU0, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA1_PRU0, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA_MASK0_PRU0, 0xFFFFFFFEU);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA_MASK1_PRU0, 0xFFFFU);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS4_AND_EN_PRU0, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS4_OR_EN_PRU0, 0x60010000U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG1_PRU0, 0x300U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG2_PRU0, 0x0U);
        }
        else
        {
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_START_LEN_PRU1, 0x60000U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_CFG_PRU1, 0x5555U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA0_PRU1, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA1_PRU1, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA_MASK0_PRU1, 0xFFFFFFFEU);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA_MASK1_PRU1, 0xFFFFU);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS4_AND_EN_PRU1, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS4_OR_EN_PRU1, 0x60010000U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG1_PRU1, 0x300U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG2_PRU1, 0x0U);
        }
    }
    else
    {
         if ((portNum % 2U) == 0U)
        {
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_START_LEN_PRU0, 0x60000U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_CFG_PRU0, 0x5555U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA0_PRU0, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA1_PRU0, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA_MASK0_PRU0, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA_MASK1_PRU0, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS4_AND_EN_PRU0, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS4_OR_EN_PRU0, 0x22000000U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG1_PRU0, 0x300U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG2_PRU0, 0x0U);
        }
        else
        {
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_START_LEN_PRU1, 0x60000U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_CFG_PRU1, 0x5555U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA0_PRU1, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA1_PRU1, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA_MASK0_PRU1, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA_MASK1_PRU1, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS4_AND_EN_PRU1, 0x0U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS4_OR_EN_PRU1, 0x22000000U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG1_PRU1, 0x300U);
            CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG2_PRU1, 0x0U);
        }
    }
    return EMAC_DRV_RESULT_OK;
}

/* classifer  utilities*/
/*
 *  ======== emac_make_class_addr ========
 */
physptr_t emac_make_class_addr(physptr_t inst, physptr_t base, int32_t classi)
{
    physptr_t addr = inst+ base + classi*8;
    return addr;
}

/*
 *  ======== emac_make_class_cfg_addr ========
 */
physptr_t emac_make_class_cfg_addr(physptr_t inst, physptr_t base)
{
    physptr_t addr = inst + base + EMAC_CLASS_CFG1_OFF;
    return addr;
}

/*
 *  ======== emac_classifier_disable ========
 */
void emac_classifier_disable(uint32_t portNum)
{
    uint32_t classi;
    uintptr_t baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE;
    physptr_t addr;
    uint32_t temp;

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);
    for(classi=0;classi<EMAC_NUM_CLASSIFIERS;classi++)
    {
        addr = emac_make_class_addr((physptr_t) baseAddr,((portNum % 2U) ==0)?CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_AND_EN_PRU0: CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_AND_EN_PRU1,classi);
        CSL_REG32_WR(addr+ EMAC_CLASS_AND_EN_OFF, 0x0);
        CSL_REG32_WR(addr+ EMAC_CLASS_OR_EN_OFF, 0x0);

        addr = emac_make_class_cfg_addr((physptr_t)baseAddr, ((portNum % 2U) ==0)? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_AND_EN_PRU0 : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_AND_EN_PRU1);
        temp = CSL_REG32_RD(addr+EMAC_CLASS_CFG2_OFF);
        temp &= ~(1U<< (classi+16U));
        temp &= ~(1U<< (classi));
        CSL_REG32_WR(addr+ EMAC_CLASS_CFG2_OFF, temp);
        temp = CSL_REG32_RD(addr);
        /* clear the bits */
        temp &= ~(0x3U << (classi*2U));
        temp |=    (0U << (classi *2U));
        CSL_REG32_WR(addr,temp);

        temp = CSL_REG32_RD(addr+EMAC_CLASS_GATE_OFF);
        /*clear the classi & allow masks */
        temp &= ~(0x60U );
        /*set rate & phase masks */
          temp |= (0x50U);
        CSL_REG32_WR(addr+EMAC_CLASS_GATE_OFF+classi*4, temp);
    }
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT",portNum);
}

/*
 *  ======== emac_switch_vlan_init ========
 */
void emac_switch_vlan_init(uint32_t portNum,EMAC_OPEN_CONFIG_INFO_T *p_config)
{
    EMAC_HwAttrs_V5 *hwAttrs = (EMAC_HwAttrs_V5*)p_config->hwAttrs;
    uint32_t vlanDefaultTblOffset;
    uint32_t regVal;
    uintptr_t regAddr;
    int32_t vlanEntry;

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);
    vlanDefaultTblOffset = EMAC_ICSSG_SWITCH_DEFAULT_VLAN_TABLE_OFFSET;

    /* Program Port VLAN in MMR */
    regAddr = hwAttrs->portCfg[portNum].icssgCfgRegBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FDB_DF_VLAN;
    regVal = 0;
    CSL_REG32_WR (regAddr, regVal);

    /*Program VLAN TABLE address in MMR*/
    regAddr = hwAttrs->portCfg[portNum].icssgCfgRegBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FDB_GEN_CFG1;
    CSL_REG32_FINS(regAddr,
               ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FDB_GEN_CFG1_SMEM_VLAN_OFFSET,
               CSL_ICSS_G_RAM_SLV_RAM_REGS_BASE + vlanDefaultTblOffset);

    /* Enable VLAN FDB in MMR */
    regAddr = hwAttrs->portCfg[portNum].icssgCfgRegBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FDB_GEN_CFG2;
    regVal =CSL_REG32_RD (regAddr);

    regVal |= (0x1 << CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FDB_GEN_CFG2_FDB_PRU0_EN_SHIFT)|
              (0x1 << CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FDB_GEN_CFG2_FDB_PRU1_EN_SHIFT)|
              (0x1 << CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FDB_GEN_CFG2_FDB_HOST_EN_SHIFT)|
              (0x1 << CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FDB_GEN_CFG2_FDB_VLAN_EN_SHIFT);
    CSL_REG32_WR (regAddr,regVal);

    /* Initialize VLAN-FID table to 0, do two entries at a time (4 byte) */
    for(vlanEntry=0; vlanEntry<2048; vlanEntry++)
        CSL_REG32_WR((hwAttrs->portCfg[portNum].icssSharedRamBaseAddr+ vlanDefaultTblOffset) + vlanEntry*4, 0);

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT",portNum);
}


/*
 *  ======== emac_ioctl_vlan_ctrl_get_entry ========
 */
EMAC_DRV_ERR_E emac_ioctl_vlan_ctrl_get_entry(uint32_t portNum, void* ctrl)
{
    uint16_t readEntry;
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
    EMAC_IOCTL_VLAN_FID_ENTRY *pVlanTblEntry = (EMAC_IOCTL_VLAN_FID_ENTRY*)ctrl;

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);
    if (pVlanTblEntry->vlanId> (EMAC_VLAN_TBL_MAX_ENTRIES-1))
    {
        UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d, invalid vlan id parameter: %d",portNum, pVlanTblEntry->vlanId);
        retVal = EMAC_DRV_RESULT_IOCTL_ERR_INVALID_VLAN_ID;
    }
    else
    {
        readEntry = CSL_REG16_RD((emac_mcb.port_cb[portNum].icssSharedRamBaseAddr+ EMAC_ICSSG_SWITCH_DEFAULT_VLAN_TABLE_OFFSET) + pVlanTblEntry->vlanId*2);
        pVlanTblEntry->vlanFidPrams.hostMember  = readEntry& 0x1;
        pVlanTblEntry->vlanFidPrams.p1Member    = (readEntry & 0x2) >> 0x1;
        pVlanTblEntry->vlanFidPrams.p2Member    = (readEntry & 0x4) >> 0x2;
        pVlanTblEntry->vlanFidPrams.hostTagged  = (readEntry & 0x8) >> 0x3;
        pVlanTblEntry->vlanFidPrams.p1Tagged    = (readEntry & 0x10) >> 0x4;
        pVlanTblEntry->vlanFidPrams.p2Tagged    = (readEntry & 0x20) >> 0x5;
        pVlanTblEntry->vlanFidPrams.streamVid   = (readEntry & 0x40) >> 0x6;
        pVlanTblEntry->vlanFidPrams.floodToHost = (readEntry & 0x80) >> 0x7;
        pVlanTblEntry->vlanFidPrams.fid         = (readEntry & 0xff00) >> 0x8;
   }
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;
}

/*
 *  ======== emac_ioctl_vlan_ctrl_set_entry ========
 */
EMAC_DRV_ERR_E emac_ioctl_vlan_ctrl_set_entry(uint32_t portNum, void* ctrl)
{
    uint16_t  vlanEntry;
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);
    EMAC_IOCTL_VLAN_FID_ENTRY *pVlanTblEntry = (EMAC_IOCTL_VLAN_FID_ENTRY*)ctrl;

    if (pVlanTblEntry->vlanId> (EMAC_VLAN_TBL_MAX_ENTRIES-1))
    {
        UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d, invalid vlan id parameter: %d",portNum, pVlanTblEntry->vlanId);
        retVal = EMAC_DRV_RESULT_IOCTL_ERR_INVALID_VLAN_ID;
    }
    else
    {
        vlanEntry = (pVlanTblEntry->vlanFidPrams.hostMember) | (pVlanTblEntry->vlanFidPrams.p1Member << 1) |
                 (pVlanTblEntry->vlanFidPrams.p2Member << 2)  | (pVlanTblEntry->vlanFidPrams.hostTagged << 3)|
                 (pVlanTblEntry->vlanFidPrams.p1Tagged << 4)  | (pVlanTblEntry->vlanFidPrams.p2Tagged << 5) |
                (pVlanTblEntry->vlanFidPrams.streamVid << 6) | (pVlanTblEntry->vlanFidPrams.floodToHost << 7);
        vlanEntry  |=  (pVlanTblEntry->vlanFidPrams.fid<< 8);
        CSL_REG16_WR(( emac_mcb.port_cb[portNum].icssSharedRamBaseAddr+ EMAC_ICSSG_SWITCH_DEFAULT_VLAN_TABLE_OFFSET) + pVlanTblEntry->vlanId*2, vlanEntry);
    }
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;
}

/*
 *  ======== emac_ioctl_vlan_ctrl_set_default_tbl ========
 */
EMAC_DRV_ERR_E emac_ioctl_vlan_ctrl_set_default_tbl(uint32_t portNum, void* ctrl)
{
    EMAC_IOCTL_VLAN_FID_PARAMS *pVlanTblEntryParams = (EMAC_IOCTL_VLAN_FID_PARAMS*)ctrl;
    uint32_t count;
    uint16_t vlanEntry;

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);
    vlanEntry = (pVlanTblEntryParams->hostMember) | (pVlanTblEntryParams->p1Member << 1) |
                    (pVlanTblEntryParams->p2Member << 2)  | (pVlanTblEntryParams->hostTagged << 3)|
                    (pVlanTblEntryParams->p1Tagged << 4)  | (pVlanTblEntryParams->p2Tagged << 5) |
                    (pVlanTblEntryParams->streamVid << 6) | (pVlanTblEntryParams->floodToHost << 7);

    vlanEntry  |=  (pVlanTblEntryParams->fid << 8);

    /* Initialize VLAN-FID table */
    for(count=0U; count < (EMAC_VLAN_TBL_MAX_ENTRIES-1U); count++)
    {
        CSL_REG16_WR(( emac_mcb.port_cb[portNum].icssSharedRamBaseAddr+ EMAC_ICSSG_SWITCH_DEFAULT_VLAN_TABLE_OFFSET) + count*2U, vlanEntry);
    }
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT",portNum);
    return EMAC_DRV_RESULT_OK;
}

/*
*  ======== emac_ioctl_vlan_ctrl_set_default_vlan_id_host_port ========
*/
EMAC_DRV_ERR_E emac_ioctl_vlan_ctrl_set_default_vlan_id_host_port(uint32_t portNum, void* ctrl)
{
    uintptr_t regAddr;
    uint32_t regVal=0;
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
    uint8_t tempByte1 = 0;
    uint8_t tempByte2 = 0;
    EMAC_IOCTL_VLAN_DEFAULT_ENTRY *vlanDefaultEntry= (EMAC_IOCTL_VLAN_DEFAULT_ENTRY*)ctrl;

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);
    if (vlanDefaultEntry->vlanId >= (EMAC_VLAN_TBL_MAX_ENTRIES-1) || ((vlanDefaultEntry->vlanId) == 0))
    {
        UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d, invalid vlan id parameter: %d",portNum, vlanDefaultEntry->vlanId);
        retVal = EMAC_DRV_RESULT_IOCTL_ERR_INVALID_VLAN_ID;
    }
    else
    {
        /* Write Host default VLAN to memory. This is used directly to update MMR for inserting VLAN tag */
        /*Populate first byte which contains 3 bits of PCP + 1 bit of DEI and 4 bits of VLAN*/
        tempByte1 = (uint8_t)(vlanDefaultEntry->pcp << 5) | (uint8_t)((uint16_t)vlanDefaultEntry->vlanId >> 8);
        /*Populate second byte which contains 8 bits of VLAN*/
        tempByte2 = (uint8_t)((uint16_t)vlanDefaultEntry->vlanId & 0xFF);  /*Lower 8 bits*/

        /* Write Host default VLAN to memory. This is used directly to update MMR for inserting VLAN tag . DEI is 0*/
        /*OR the values*/
        regVal = (uint32_t)(0x0081) | ((uint32_t)tempByte1 << 16) | ((uint32_t)tempByte2 << 24);

        regAddr = emac_mcb.port_cb[portNum].icssSharedRamBaseAddr + EMAC_ICSSG_SWITCH_PORT0_DEFAULT_VLAN_OFFSET;
        CSL_REG32_WR (regAddr, regVal);
    }
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;
}

/*
*  ======== emac_ioctl_vlan_ctrl_set_default_vlan_id ========
*/
EMAC_DRV_ERR_E emac_ioctl_vlan_ctrl_set_default_vlan_id(uint32_t portNum, void* ctrl)
{
    uintptr_t regAddr;
    uint32_t regVal=0;
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
    uint8_t tempByte1 = 0;
    uint8_t tempByte2 = 0;
    EMAC_IOCTL_VLAN_DEFAULT_ENTRY *vlanDefaultEntry= (EMAC_IOCTL_VLAN_DEFAULT_ENTRY*)ctrl;

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);
    if (vlanDefaultEntry->vlanId >= (EMAC_VLAN_TBL_MAX_ENTRIES-1) || ((vlanDefaultEntry->vlanId) == 0))
    {
        UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d, invalid vlan id parameter: %d",portNum, vlanDefaultEntry->vlanId);
        retVal = EMAC_DRV_RESULT_IOCTL_ERR_INVALID_VLAN_ID;
    }
    else
    {
        /* Program Port VLAN in HW MMR, SW1 and SW2 for both ICSSG instances*/
        regAddr = emac_mcb.port_cb[portNum].icssgCfgRegBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FDB_DF_VLAN + 2*portNum;
        CSL_REG16_WR (regAddr, vlanDefaultEntry->vlanId);

        /*Populate first byte which contains 3 bits of PCP + 1 bit of DEI and 4 bits of VLAN*/
        tempByte1 = (uint8_t)(vlanDefaultEntry->pcp << 5) | (uint8_t)((uint16_t)vlanDefaultEntry->vlanId >> 8);
        /*Populate second byte which contains 8 bits of VLAN*/
        tempByte2 = (uint8_t)((uint16_t)vlanDefaultEntry->vlanId & 0xFF);  /*Lower 8 bits*/

        /* Write Host default VLAN to memory. This is used directly to update MMR for inserting VLAN tag . DEI is 0*/
        /*OR the values*/
        regVal = (uint32_t)(0x0081) | ((uint32_t)tempByte1 << 16) | ((uint32_t)tempByte2 << 24);

        /* Update for host port */
        if ((portNum & 1) == 0)
        {
            regAddr = emac_mcb.port_cb[portNum].icssSharedRamBaseAddr + EMAC_ICSSG_SWITCH_PORT1_DEFAULT_VLAN_OFFSET;
        }
        else
        {
            regAddr = emac_mcb.port_cb[portNum].icssSharedRamBaseAddr + EMAC_ICSSG_SWITCH_PORT2_DEFAULT_VLAN_OFFSET;
        }
        CSL_REG32_WR (regAddr, regVal);

        /* Update Deafult Queue number for untagged packet*/
        if ((portNum & 1) == 0)
        {
            tempByte1 = CSL_REG8_RD(emac_mcb.port_cb[portNum].icssDram0BaseAddr + PORT_Q_PRIORITY_MAPPING_OFFSET + vlanDefaultEntry->pcp);
            regAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + QUEUE_NUM_UNTAGGED;
        }
        else
        {
            tempByte1 = CSL_REG8_RD(emac_mcb.port_cb[portNum].icssDram1BaseAddr + PORT_Q_PRIORITY_MAPPING_OFFSET + vlanDefaultEntry->pcp);
            regAddr = emac_mcb.port_cb[portNum].icssDram1BaseAddr + QUEUE_NUM_UNTAGGED;
        }
        CSL_REG8_WR (regAddr, tempByte1);
    }
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;
}

EMAC_DRV_ERR_E emac_icssg_filter3_config (uint32_t portNum, uint32_t pruNum, uint32_t filterNum, EMAC_FILTER3_CONFIG* ftConfig)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
    uintptr_t baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE;

    uintptr_t icssg_filter_base, offsetToFt3Pattern = (CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_P0_PRU0-CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_START_PRU0);

    if (pruNum == 0)
        icssg_filter_base = baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_START_PRU0;
    else if (pruNum == 1)
        icssg_filter_base = baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_START_PRU1;
    else
        retVal = EMAC_DRV_RESULT_IOCTL_ERR;

    if (retVal == EMAC_DRV_RESULT_OK)
    {
        CSL_REG32_WR((icssg_filter_base+filterNum*32), ftConfig->ft3Start);
        CSL_REG32_WR((icssg_filter_base+filterNum*32+4), ftConfig->ft3StartAuto);
        CSL_REG32_WR((icssg_filter_base+filterNum*32+8), ftConfig->ft3StartOffset);
        CSL_REG32_WR((icssg_filter_base+filterNum*32+12), ftConfig->ft3JmpOffset);
        CSL_REG32_WR((icssg_filter_base+filterNum*32+16), ftConfig->ft3Len);
        CSL_REG32_WR((icssg_filter_base+filterNum*32+20), ftConfig->ft3Config);
        CSL_REG32_WR((icssg_filter_base+filterNum*32+24), ftConfig->ft3Type);
        CSL_REG32_WR((icssg_filter_base+filterNum*32+28), ftConfig->ft3TypeMask);
        CSL_REG32_WR((icssg_filter_base+filterNum*16+offsetToFt3Pattern), ftConfig->ft3PatternLow);
        CSL_REG32_WR((icssg_filter_base+filterNum*16+offsetToFt3Pattern+4), ftConfig->ft3PatternHigh);
        CSL_REG32_WR((icssg_filter_base+filterNum*16+offsetToFt3Pattern+8), ftConfig->ft3PatternMaskLow);
        CSL_REG32_WR((icssg_filter_base+filterNum*16+offsetToFt3Pattern+12), ftConfig->ft3PatternMaskHigh);
    }
    return retVal;
}

/*
 *  ======== emac_ioctl_port_prio_mapping_ctrl ========
 */
EMAC_DRV_ERR_E emac_ioctl_port_prio_mapping_ctrl(uint32_t portNum, void*  ctrl)
{
    uint8_t index;
    uintptr_t icssgBaseAddr;
    uint8_t tempVal;
    EMAC_IOCTL_PORT_PRIO_MAP *pPrioMap;
    uint8_t untaggedQueueNum;

    uintptr_t baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE;
    uint32_t ft3Type = 0;
    uint32_t classSelect = 0;
    uint32_t orEnable[EMAC_IOCTL_PRIO_MAX] = {0,};
    uint32_t andEnable[EMAC_IOCTL_PRIO_MAX] = {0,};
    uint16_t orNvEnable[EMAC_IOCTL_PRIO_MAX] = {0,};
    uint16_t andNvEnable[EMAC_IOCTL_PRIO_MAX] = {0,};
    uint32_t gateConfig = 0x50;
    uint32_t tempReg = 0;
    EMAC_FILTER3_CONFIG ft3ConfigPcp = {0xc, 0, 0, 0, 0, 5, 0, 0xff1f0000, 0, 0, 0xffffffff, 0xffffffff};
    uint8_t pcp;
    uint8_t finalPrioQueueMap[EMAC_IOCTL_PRIO_MAX]={0};
    uint32_t portLoc = 0;
    if((portNum  & 1) == 1)
    {
        portLoc = 1;
    }

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    pPrioMap = (EMAC_IOCTL_PORT_PRIO_MAP*)ctrl;
    /* FIXME: Is icssgBaseAddr being calculated properly for PG2.0 FW */
    icssgBaseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc;
    for (index = 0; index < EMAC_IOCTL_PRIO_MAX;index++)
    {
        tempVal = (uint8_t)pPrioMap->portPrioMap[index];
        CSL_REG8_WR(icssgBaseAddr + PORT_Q_PRIORITY_MAPPING_OFFSET + index, tempVal);
    }

    /* Re configure classifier to map incoming pcp to Queue. Take into account Prio regen config also */
    /*
        One-to-one mapping from PCP -> Traffic Class.
        Managed using FT3[0:7] and Classifier[0:7].
    */
    /* set up filter type 3's to match pcp bits */
    for (pcp = 0U; pcp < EMAC_IOCTL_PRIO_MAX; pcp++)
    {
        /*Setup FT3[0:7] to detect PCP0 - PCP7 */
        ft3Type = (uint32_t)((((uint32_t)pcp) << 21U) | 0x00000081U);
        ft3ConfigPcp.ft3Type = ft3Type;
        emac_icssg_filter3_config(portNum, (portNum & 1), pcp, &ft3ConfigPcp);
    }

    /*Get the Queue mapping value from DRAM0 and calculate incoming PCP to Queue mapping*/
    for(pcp = 0U; pcp < EMAC_IOCTL_PRIO_MAX; pcp++)
    {
        /* Get regenerated value for PCP = p*/
        tempVal = CSL_REG8_RD(icssgBaseAddr + PORT_Q_PRIORITY_REGEN_OFFSET + pcp);
        /*Shift PCP value by 5 to get the value*/
        tempVal = tempVal >> 5;
         finalPrioQueueMap[pcp] = (uint8_t)pPrioMap->portPrioMap[tempVal];
    }

    /* Build up the or lists */
    for (pcp = 0U; pcp < EMAC_IOCTL_PRIO_MAX; pcp++)
    {
        classSelect = finalPrioQueueMap[pcp];
        orEnable[classSelect] |= (1U << pcp);
    }

    if ((portNum % 2U) == 1U)
    {
        baseAddr += (CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_OR_EN_PRU1
                    - CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_OR_EN_PRU0);
    }
    /* now program classifier c */
    for (pcp = 0U; pcp < EMAC_IOCTL_PRIO_MAX;pcp++ )
    {
        /* Configure OR Enable*/
        CSL_REG32_WR((baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_OR_EN_PRU0 + (8U*pcp)), orEnable[pcp]);
        /* Configure AND Enable */
        CSL_REG32_WR((baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_AND_EN_PRU0 + (8U*pcp)), andEnable[pcp]);
        tempReg = CSL_REG32_RD(baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG1_PRU0);
        tempReg &= ~(0x3U << (pcp * 2U));
        CSL_REG32_WR((baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG1_PRU0), tempReg);
        /* Configure NV Enable */
        /* Configure NV Enable bits (1 bit in upper16, 1bit in lower16 */
        tempReg = CSL_REG32_RD(baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG2_PRU0);
        if (orNvEnable[pcp])
            tempReg |= 1U << (pcp + 16U);
        else
            tempReg &= ~(1U << (pcp + 16U));
        if (andNvEnable[pcp])
            tempReg |= 1U << (pcp);
        else
            tempReg &= ~(1U << (pcp));
        CSL_REG32_WR((baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG2_PRU0), tempReg);
        /* Configure class gate */
        CSL_REG32_WR((baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_GATES0_PRU0 + (4U * pcp)), gateConfig);
    }

    /* Update Deafult Queue number for untagged packet*/
    /*Read the configured PCP value*/
    if ((portNum & 1) == 0)
    {
        baseAddr = emac_mcb.port_cb[portNum].icssSharedRamBaseAddr + EMAC_ICSSG_SWITCH_PORT1_DEFAULT_VLAN_OFFSET;
    }
    else
    {
        baseAddr = emac_mcb.port_cb[portNum].icssSharedRamBaseAddr + EMAC_ICSSG_SWITCH_PORT2_DEFAULT_VLAN_OFFSET;
    }
    tempVal = CSL_REG8_RD(baseAddr + 2); /*Read PCP value*/
    tempVal = tempVal >> 5; /*Shif to get the value in correct format*/
    untaggedQueueNum = pPrioMap->portPrioMap[tempVal];

    if ((portNum & 1) == 0)
    {
        baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + QUEUE_NUM_UNTAGGED;
    }
    else
    {
        baseAddr =  emac_mcb.port_cb[portNum].icssDram1BaseAddr + QUEUE_NUM_UNTAGGED;
    }
    CSL_REG8_WR (baseAddr, untaggedQueueNum);

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, EMAC_DRV_RESULT_OK);
    return EMAC_DRV_RESULT_OK;
}

/*
 *  ======== emac_switch_config_ft3_priority_tag ========
 */
void emac_switch_config_ft3_priority_tag(uint32_t portNum)
{
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);
    /* Program FT3[8] to detect priority tagged frames*/
    EMAC_FILTER3_CONFIG ft3ConfigPrioTagFrames =   {0xc, 0, 0, 0, 0, 5, 0x00000081, 0x00f00000, 0, 0, 0xffffffff, 0xffffffff};
    emac_icssg_filter3_config(portNum, 0, 8, &ft3ConfigPrioTagFrames);
    emac_icssg_filter3_config(portNum, 1, 8, &ft3ConfigPrioTagFrames);
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT",portNum);
}

EMAC_DRV_ERR_E emac_ioctl_port_state_ctrl(uint32_t portNum, void* p_params)
{
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_IOCTL_ERR;
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    switch (pParams->subCommand)
    {
        case EMAC_IOCTL_PORT_STATE_DISABLE:
            retVal = emac_send_R30_cmd(EMAC_PORT_DISABLE, portNum, pParams->seqNumber);
            break;
        case EMAC_IOCTL_PORT_STATE_BLOCKING:
            retVal = emac_send_R30_cmd(EMAC_PORT_BLOCK, portNum, pParams->seqNumber);
            break;
        case EMAC_IOCTL_PORT_STATE_FORWARD:
            retVal = emac_send_R30_cmd(EMAC_PORT_FORWARD, portNum, pParams->seqNumber);
            break;
        case EMAC_IOCTL_PORT_STATE_FORWARD_WO_LEARNING:
            retVal = emac_send_R30_cmd(EMAC_PORT_FORWARD_WO_LEARNING, portNum, pParams->seqNumber);
            break;
        case EMAC_IOCTL_PORT_STATE_TAS_TRIGGER:
            retVal = emac_send_R30_cmd(EMAC_PORT_TAS_TRIGGER, portNum, pParams->seqNumber);
            break;
        case EMAC_IOCTL_PORT_STATE_TAS_ENABLE:
            retVal = emac_send_R30_cmd(EMAC_PORT_TAS_ENABLE, portNum, pParams->seqNumber);
            break;
        case EMAC_IOCTL_PORT_STATE_TAS_RESET:
            retVal = emac_send_R30_cmd(EMAC_PORT_TAS_RESET, portNum, pParams->seqNumber);
            break;
        case EMAC_IOCTL_PORT_STATE_TAS_DISABLE:
            retVal = emac_send_R30_cmd(EMAC_PORT_TAS_DISABLE, portNum, pParams->seqNumber);
            break;
        default:
            break;
    }

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;
}

/*
 *  ======== emac_ioctl_fdb_del_all ========
 */
EMAC_DRV_ERR_E emac_ioctl_fdb_del_all(uint32_t portNum, void* p_params)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_IOCTL_IN_PROGRESS;
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;

    /* make sure there is hw descriptor for boths ICSSG instances */
    emac_update_cmd(portNum, EMAC_IOCTL_FDB_ENTRY_CTRL, emac_mcb.ioctl_cb.pCmd1Icssg, pParams, NULL, EMAC_FW_MGMT_FDB_CMD_TYPE, 0, 0);
    retVal = emac_ioctl_send_mgmt_msg(portNum, emac_mcb.ioctl_cb.pCmd1Icssg, NULL);
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);

    return retVal;
}

/*
 *  ======== emac_ioctl_fdb_entry_ctrl ========
 */
EMAC_DRV_ERR_E emac_ioctl_fdb_entry_ctrl(uint32_t portNum, void* p_params)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_IOCTL_ERR_INVALID_VLAN_ID;
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    EMAC_IOCTL_FDB_ENTRY *entry = (EMAC_IOCTL_FDB_ENTRY*)pParams->ioctlVal;
    uintptr_t vlanDefaultTblAddr;
    uint8_t fid;
    uint16_t broadSideSlot;

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);
    if ((entry->vlanId < (int16_t)(EMAC_VLAN_TBL_MAX_ENTRIES)) || (entry->vlanId == EMAC_VLAN_UNTAGGED))
    {
        vlanDefaultTblAddr = emac_get_vlan_tbl_addr(portNum);
        emac_get_vlan_id(portNum, entry);
        broadSideSlot = emac_util_fdb_helper( vlanDefaultTblAddr, entry->vlanId, entry->mac, &fid);
        emac_update_cmd(portNum, EMAC_IOCTL_FDB_ENTRY_CTRL, emac_mcb.ioctl_cb.pCmd1Icssg, pParams, entry, EMAC_FW_MGMT_FDB_CMD_TYPE, broadSideSlot, fid);

        retVal = emac_ioctl_send_mgmt_msg(portNum, emac_mcb.ioctl_cb.pCmd1Icssg, NULL);
    }
    else
    {
        UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d, invalid vlan id parameter: %d",portNum, entry->vlanId);
    }
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;
}
/*Buffer for 8 FDB entries to be added by 'Add Multiple FDB entries IOCTL*/
/*
 *  ======== emac_add_fdb_multiple_ctrl ========
 */
typedef struct FDB_CMD_BUFFER_S {
    volatile uint8_t mac[6];
    volatile uint8_t fid;
    volatile uint8_t fid_c2;
    volatile uint16_t   slot;
    volatile uint16_t   flags;
} FDB_CMD_BUFFER_T;

#if defined (SOC_AM65XX)
static FDB_CMD_BUFFER_T *fdb_cmd_buffer[EMAC_MAX_PORTS-1] = {
    (FDB_CMD_BUFFER_T *)( CSL_PRU_ICSSG0_DRAM0_SLV_RAM_BASE + FDB_CMD_BUFFER),
    (FDB_CMD_BUFFER_T *)( CSL_PRU_ICSSG0_DRAM1_SLV_RAM_BASE + FDB_CMD_BUFFER),
    (FDB_CMD_BUFFER_T *)( CSL_PRU_ICSSG1_DRAM0_SLV_RAM_BASE + FDB_CMD_BUFFER),
    (FDB_CMD_BUFFER_T *)( CSL_PRU_ICSSG1_DRAM1_SLV_RAM_BASE + FDB_CMD_BUFFER),
    (FDB_CMD_BUFFER_T *)( CSL_PRU_ICSSG2_DRAM0_SLV_RAM_BASE + FDB_CMD_BUFFER),
    (FDB_CMD_BUFFER_T *)( CSL_PRU_ICSSG2_DRAM1_SLV_RAM_BASE + FDB_CMD_BUFFER)
};
#else
static FDB_CMD_BUFFER_T *fdb_cmd_buffer[EMAC_MAX_PORTS-1] = {
    (FDB_CMD_BUFFER_T *)( CSL_PRU_ICSSG0_DRAM0_SLV_RAM_BASE + FDB_CMD_BUFFER),
    (FDB_CMD_BUFFER_T *)( CSL_PRU_ICSSG0_DRAM1_SLV_RAM_BASE + FDB_CMD_BUFFER),
    (FDB_CMD_BUFFER_T *)( CSL_PRU_ICSSG1_DRAM0_SLV_RAM_BASE + FDB_CMD_BUFFER),
    (FDB_CMD_BUFFER_T *)( CSL_PRU_ICSSG1_DRAM1_SLV_RAM_BASE + FDB_CMD_BUFFER)
};
#endif

EMAC_DRV_ERR_E emac_add_fdb_multiple_ctrl(uint32_t portNum, void* p_params)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_IOCTL_ERR_INVALID_VLAN_ID;
    uint32_t j;
    uintptr_t vlanDefaultTblAddr;
    uint8_t fid;
    uint16_t slot;
    FDB_CMD_BUFFER_T *fdb_cmd_entry;
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    EMAC_IOCTL_FDB_ENTRY_MULTIPLE *fdbEntryMultiple = (EMAC_IOCTL_FDB_ENTRY_MULTIPLE*)pParams->ioctlVal;
    EMAC_IOCTL_CMD_T *pCmd = emac_mcb.ioctl_cb.pCmd1Icssg;

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    if (fdbEntryMultiple->numFdbEntries > EMAC_IOCTL_FDB_ENTRY_MULTIPLE_MAX)
    {
        retVal = EMAC_DRV_RESULT_IOCTL_ERR;
    }
    else
    {
        fdb_cmd_entry = fdb_cmd_buffer[portNum];

       for (j = 0; j < fdbEntryMultiple->numFdbEntries; j++)
        {
            EMAC_IOCTL_FDB_ENTRY *entry = fdbEntryMultiple[j].fdbEntry;
            if ((entry->vlanId < (int16_t)(EMAC_VLAN_TBL_MAX_ENTRIES)) || (entry->vlanId == EMAC_VLAN_UNTAGGED))
            {
                vlanDefaultTblAddr = emac_get_vlan_tbl_addr(portNum);
                emac_get_vlan_id(portNum, entry);
                slot = emac_util_fdb_helper( vlanDefaultTblAddr, entry->vlanId, entry->mac, &fid);
                memcpy((void *)&fdb_cmd_entry->mac[0], (void *)&entry->mac[0], 6);
                fdb_cmd_entry->fid      = fid;
                fdb_cmd_entry->fid_c2   = entry->fdbEntry[portNum & 1];
                fdb_cmd_entry->slot     = slot;
                fdb_cmd_entry->flags    = 1; // entry is valid
            }
            else
            {
                fdb_cmd_entry->flags    = 0; // entry is invalid
            }
        }

        pCmd->commandHeader = EMAC_FW_MGMT_CMD_HEADER;
        pCmd->commandType = 0x3;  // FDB COMMAND
        pCmd->seqNumber = pParams->seqNumber;
        pCmd->commandParam = ((portNum & 1) << 4) | EMAC_IOCTL_FDB_ENTRY_ADD_MULTIPLE;
        retVal = emac_ioctl_send_mgmt_msg(portNum, emac_mcb.ioctl_cb.pCmd1Icssg, NULL);
        UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    }
    return retVal;
}

/*
 *  ======== emac_ioctl_accept_frame_check_ctrl ========
 */
EMAC_DRV_ERR_E emac_ioctl_accept_frame_check_ctrl(uint32_t portNum, void* p_params)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_IOCTL_ERR;
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    switch (pParams->subCommand)
    {
        case EMAC_IOCTL_ACCEPTABLE_FRAME_CHECK_ONLY_VLAN_TAGGED:
            retVal = emac_send_R30_cmd(EMAC_PORT_ACCEPT_TAGGED, portNum, pParams->seqNumber);
            break;
        case EMAC_IOCTL_ACCEPTABLE_FRAME_CHECK_ONLY_UN_TAGGED_PRIO_TAGGED:
            retVal = emac_send_R30_cmd(EMAC_PORT_ACCEPT_UNTAGGED_N_PRIO, portNum, pParams->seqNumber);
            break;
        case EMAC_IOCTL_ACCEPTABLE_FRAME_CHECK_ALL_FRAMES:
            retVal = emac_send_R30_cmd(EMAC_PORT_ACCEPT_ALL, portNum, pParams->seqNumber);
            break;
        default:
            break;
    }

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;
}

/*
 *  ======== emac_ioctl_prio_regen_mapping_ctrl ========
 */
EMAC_DRV_ERR_E emac_ioctl_prio_regen_mapping_ctrl(uint32_t portNum, void*  ctrl)
{
    uint8_t index;
    uintptr_t icssgBaseAddr;
    uint32_t tempVal;
    EMAC_IOCTL_PRIO_REGEN_MAP *pPrioRegenMap = (EMAC_IOCTL_PRIO_REGEN_MAP*)ctrl;

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);
    /*
        One-to-one mapping from PCP -> Traffic Class.
        Managed using FT3[0:7] and Classifier[0:7].
    */
    uintptr_t baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE;
    uint32_t ft3Type = 0;
    uint32_t classSelect = 0;
    uint32_t orEnable[EMAC_IOCTL_PRIO_MAX] = {0,};
    uint32_t andEnable[EMAC_IOCTL_PRIO_MAX] = {0,};
    uint16_t orNvEnable[EMAC_IOCTL_PRIO_MAX] = {0,};
    uint16_t andNvEnable[EMAC_IOCTL_PRIO_MAX] = {0,};
    uint32_t gateConfig = 0x50;
    uint32_t tempReg = 0;
    EMAC_FILTER3_CONFIG ft3ConfigPcp = {0xc, 0, 0, 0, 0, 5, 0, 0xff1f0000, 0, 0, 0xffffffff, 0xffffffff};
    uint8_t pcp;
    uint8_t finalPrioQueueMap[EMAC_IOCTL_PRIO_MAX]={0};
    uint32_t portLoc = 0;
    if((portNum  & 1) == 1)
    {
        portLoc = 1;
    }

    /* set up filter type 3's to match pcp bits */
    for (pcp = 0U; pcp < EMAC_IOCTL_PRIO_MAX; pcp++)
    {
        /*Setup FT3[0:7] to detect PCP0 - PCP7 */
        ft3Type = (uint32_t)((((uint32_t)pcp) << 21U) | 0x00000081U);
        ft3ConfigPcp.ft3Type = ft3Type;
        emac_icssg_filter3_config(portNum, portLoc, pcp, &ft3ConfigPcp);
    }
    /*Get the Queue mapping value from DRAM0 and calculate incoming PCP to Queue mapping*/
    icssgBaseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr;
    for(pcp = 0U; pcp < EMAC_IOCTL_PRIO_MAX; pcp++)
    {
        finalPrioQueueMap[pcp] = CSL_REG8_RD(icssgBaseAddr + PORT_Q_PRIORITY_MAPPING_OFFSET + (pPrioRegenMap->prioRegenMap[pcp]));
    }
    /* Build up the or lists */
    for (pcp = 0U; pcp < EMAC_IOCTL_PRIO_MAX; pcp++)
    {
        classSelect = finalPrioQueueMap[pcp];
        orEnable[classSelect] |= (1U << pcp);
    }

    if ((portNum % 2U) == 1U)
    {
        baseAddr += (CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_OR_EN_PRU1
                    - CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_OR_EN_PRU0);
    }
    /* now program classifier c */
    for (pcp = 0U; pcp < EMAC_IOCTL_PRIO_MAX; pcp++ )
    {
        /* Configure OR Enable*/
        CSL_REG32_WR((baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_OR_EN_PRU0 + (8U * pcp)), orEnable[pcp]);
        /* Configure AND Enable */
        CSL_REG32_WR((baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_AND_EN_PRU0 + (8U * pcp)), andEnable[pcp]);
        tempReg = CSL_REG32_RD(baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG1_PRU0);
        tempReg &= ~(0x3U << (pcp * 2U));
        CSL_REG32_WR((baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG1_PRU0), tempReg);
        /* Configure NV Enable bits (1 bit in upper16, 1bit in lower16 */
        tempReg = CSL_REG32_RD(baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG2_PRU0);
        if (orNvEnable[pcp])
            tempReg |= 1U << (pcp + 16U);
        else
            tempReg &= ~(1U << (pcp + 16U));
        if (andNvEnable[pcp])
            tempReg |= 1U << (pcp);
        else
            tempReg &= ~(1U << (pcp));
        CSL_REG32_WR((baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG2_PRU0), tempReg);
        /* Configure class gate */
        CSL_REG32_WR((baseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_GATES0_PRU0 + (4U * pcp)), gateConfig);
    }

    icssgBaseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc;
    for (index = 0; index < EMAC_IOCTL_PRIO_MAX;index++)
    {
        tempVal = (uint32_t)pPrioRegenMap->prioRegenMap[index];
        /*Shift PCP value by 5 so HW can save a cycle*/
        tempVal = tempVal << 5;
        HWREGB(icssgBaseAddr + PORT_Q_PRIORITY_REGEN_OFFSET + index) = tempVal;
    }
    return EMAC_DRV_RESULT_OK;
}

/*
 *  ======== emac_ioctl_uc_flooding_ctrl ========
 */
EMAC_DRV_ERR_E emac_ioctl_uc_flooding_ctrl(uint32_t portNum, void* p_params)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_IOCTL_ERR;
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    switch (pParams->subCommand)
    {
        case EMAC_IOCTL_PORT_UC_FLOODING_ENABLE:
            retVal = emac_send_R30_cmd(EMAC_PORT_UC_FLOODING_ENABLE, portNum, pParams->seqNumber);
            break;
        case EMAC_IOCTL_PORT_UC_FLOODING_DISABLE:
            retVal = emac_send_R30_cmd(EMAC_PORT_UC_FLOODING_DISABLE, portNum, pParams->seqNumber);
            break;
         default:
            break;
    }

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;
}

/*
 *  ======== emac_ioctl_configure_interface_mac_ctrl ========
 */
EMAC_DRV_ERR_E emac_ioctl_mc_flooding_ctrl(uint32_t portNum, void* p_params)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_IOCTL_ERR;
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    switch (pParams->subCommand)
    {
        case EMAC_IOCTL_PORT_MC_FLOODING_ENABLE:
            retVal = emac_send_R30_cmd(EMAC_PORT_MC_FLOODING_ENABLE, portNum, pParams->seqNumber);
            break;
        case EMAC_IOCTL_PORT_MC_FLOODING_DISABLE:
            retVal = emac_send_R30_cmd(EMAC_PORT_MC_FLOODING_DISABLE, portNum, pParams->seqNumber);
            break;
         default:
            break;
    }

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;
}

/*
 *  ======== emac_ioctl_configure_interface_mac_ctrl ========
 */
EMAC_DRV_ERR_E emac_ioctl_configure_interface_mac_ctrl(uint32_t portNum, void* p_params)
{
    int32_t macLo;
    int32_t macHi;
    int16_t temp;
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    EMAC_MAC_ADDR_T *entry = (EMAC_MAC_ADDR_T*)pParams->ioctlVal;
    uintptr_t baseAddr;
    uint8_t* macAddr = &(entry->addr[0]);

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);


    baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE;

    macLo =  *((int32_t *) &macAddr[0]);
    temp =  *((int16_t *) &macAddr[4]);
    macHi = (int32_t) temp;

    if((portNum  & 1) == 0)
    {
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_MAC_PRU0_0, macLo);
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_MAC_PRU0_1, macHi);
    }
    else
    {
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_MAC_PRU1_0, macLo);
        CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_MAC_PRU1_1, macHi);
    }

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;

}

/*
 *  ======== emac_ioctl_configure_interface_mac_ctrl_host_port ========
 */
EMAC_DRV_ERR_E emac_ioctl_configure_interface_mac_ctrl_host_port(uint32_t portNum, void* p_params)
{
    int32_t macLo;
    int32_t macHi;
    int16_t temp;
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    EMAC_MAC_ADDR_T *entry = (EMAC_MAC_ADDR_T*)pParams->ioctlVal;
    uintptr_t baseAddr;
    uint8_t* macAddr = &(entry->addr[0]);

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE;

    macLo =  *((int32_t *) &macAddr[0]);
    temp =  *((int16_t *) &macAddr[4]);
    macHi = (int32_t) temp;

    CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_MAC_INTERFACE_0, macLo);
    CSL_REG32_WR(baseAddr+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_MAC_INTERFACE_1, macHi);

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;

}

/*
 *  ======== emac_ioctl_configure_cut_through_or_prempt_select_ctrl ========
 */
EMAC_DRV_ERR_E emac_ioctl_configure_cut_through_or_prempt_select_ctrl(uint32_t portNum, void* p_params)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;

    uint8_t queue_mask = 0;
    uint8_t queue_num;
    uint8_t temp_byte;
    uintptr_t expressPremptiveQueueAddr;
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    EMAC_IOCTL_PREMPT_OR_CUT_THROUGH_MAP *entry = (EMAC_IOCTL_PREMPT_OR_CUT_THROUGH_MAP*)pParams->ioctlVal;
    uint32_t portLoc = 0;
    if((portNum  & 1) == 1)
    {
        portLoc = 1;
    }

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    expressPremptiveQueueAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc + EXPRESS_PRE_EMPTIVE_Q_MAP;
    for (queue_num = 0U; queue_num < EMAC_IOCTL_PRIO_MAX; queue_num++)
    {
        temp_byte = (entry->pcpPreemptMap[queue_num] << 4U) | (entry->pcpCutThroughMap[queue_num] << 7U);   /*as per bit order in descriptor flags. Helps save PRU cycles*/
        CSL_REG8_WR(expressPremptiveQueueAddr+ queue_num, temp_byte);
        queue_mask = queue_mask | ((entry->pcpPreemptMap[queue_num]) << queue_num);
    }
    expressPremptiveQueueAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portNum + EXPRESS_PRE_EMPTIVE_Q_MASK;
    CSL_REG8_WR(expressPremptiveQueueAddr, ~queue_mask);
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;

}

/*
 *  ======== emac_ioctl_frame_premption_ctrl ========
 */
EMAC_DRV_ERR_E emac_ioctl_frame_premption_ctrl(uint32_t portNum, void* p_params)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_IOCTL_ERR;
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    EMAC_IOCTL_PREEMPTION_ENTRY *entry = (EMAC_IOCTL_PREEMPTION_ENTRY*)pParams->ioctlVal;
    uintptr_t premptionTxEnabledStatusAddr, premptionTxActiveStatusAddr, premptionVerifyStateStatusAddr, premptionVerifyStateValueAddr, premptionMinFragSizeAddr, premptionMinFragAddAddr;
    uint32_t portLoc = 0;
    if((portNum  & 1) == 1)
    {
        portLoc = 1;
    }

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    switch (pParams->subCommand)
    {
        case EMAC_IOCTL_PREEMPT_TX_ENABLE:
            retVal = emac_send_R30_cmd(EMAC_PORT_PREMPT_TX_ENABLE, portLoc, pParams->seqNumber);
            if (retVal == EMAC_DRV_RESULT_IOCTL_IN_PROGRESS)
            {
                premptionTxEnabledStatusAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc + PRE_EMPTION_ENABLE_TX;
                CSL_REG8_WR(premptionTxEnabledStatusAddr, 0x01);
            }
            break;
        case EMAC_IOCTL_PREEMPT_TX_DISABLE:
            retVal = emac_send_R30_cmd(EMAC_PORT_PREMPT_TX_DISABLE, portLoc, pParams->seqNumber);
            if (retVal == EMAC_DRV_RESULT_IOCTL_IN_PROGRESS)
            {
                premptionTxEnabledStatusAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc + PRE_EMPTION_ENABLE_TX;
                CSL_REG8_WR(premptionTxEnabledStatusAddr, 0x00);
            }
            break;
        case EMAC_IOCTL_PREEMPT_GET_TX_ENABLE_STATUS:
            premptionTxEnabledStatusAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc + PRE_EMPTION_ENABLE_TX;
            entry->premt_tx_enabled_status = CSL_REG8_RD(premptionTxEnabledStatusAddr);
            retVal = EMAC_DRV_RESULT_OK;
            break;
        case EMAC_IOCTL_PREEMPT_GET_TX_ACTIVE_STATUS:
            premptionTxActiveStatusAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc + PRE_EMPTION_ACTIVE_TX;
            entry->premt_tx_active_status = CSL_REG8_RD(premptionTxActiveStatusAddr);
            retVal = EMAC_DRV_RESULT_OK;
            break;
        case EMAC_IOCTL_PREEMPT_VERIFY_ENABLE:
            premptionVerifyStateStatusAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc  + PRE_EMPTION_ENABLE_VERIFY;
            CSL_REG8_WR(premptionVerifyStateStatusAddr, 0x01);
            retVal = EMAC_DRV_RESULT_OK;
            break;
        case EMAC_IOCTL_PREEMPT_VERIFY_DISABLE:
            premptionVerifyStateStatusAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc + PRE_EMPTION_ENABLE_VERIFY;
            CSL_REG8_WR(premptionVerifyStateStatusAddr, 0x00);
            retVal = EMAC_DRV_RESULT_OK;
            break;
        case EMAC_IOCTL_PREEMPT_GET_VERIFY_STATE:
            premptionVerifyStateValueAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc + PRE_EMPTION_VERIFY_STATUS;
            entry->premt_verify_state = (Preempt_Verify_States)(CSL_REG8_RD(premptionVerifyStateValueAddr));
            retVal = EMAC_DRV_RESULT_OK;
            break;
        case EMAC_IOCTL_PREEMPT_GET_MIN_FRAG_SIZE_LOCAL:
            premptionMinFragSizeAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc + PRE_EMPTION_ADD_FRAG_SIZE_LOCAL;
            entry->premt_min_fragment_size = (CSL_REG16_RD(premptionMinFragSizeAddr)/64)-1;
            retVal = EMAC_DRV_RESULT_OK;
            break;
        case  EMAC_IOCTL_PREEMPT_SET_MIN_FRAG_SIZE_REMOTE:
            premptionMinFragAddAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc + PRE_EMPTION_ADD_FRAG_SIZE_REMOTE;
            CSL_REG16_WR(premptionMinFragAddAddr, ((entry->add_min_fragment_size)+1)*64);
            retVal = EMAC_DRV_RESULT_OK;
            break;
        default:
            break;
    }
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, retVal);
    return retVal;
}

/*
 *  ======== emac_ioctl_configure_special_frame_prio_ctrl ========
 */
EMAC_DRV_ERR_E emac_ioctl_configure_special_frame_prio_ctrl(uint32_t portNum, void* p_params)
{
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    uintptr_t splPacketDefaultPrioAddr;
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    EMAC_IOCTL_SPECIAL_FRAME_DEFAULT_PRIO *entry = (EMAC_IOCTL_SPECIAL_FRAME_DEFAULT_PRIO*)pParams->ioctlVal;
    uint32_t portLoc = 0;
    if((portNum  & 1) == 1)
    {
        portLoc = 1;
    }
    splPacketDefaultPrioAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + 0x2000*portLoc + SPL_PKT_DEFAULT_PRIORITY;
    CSL_REG8_WR(splPacketDefaultPrioAddr, entry->specialFramePrio);

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",portNum, EMAC_DRV_RESULT_OK);
    return EMAC_DRV_RESULT_OK;
}

/*
 *  ======== emac_ioctl_speed_duplexity_cfg ========
 */
EMAC_DRV_ERR_E emac_ioctl_speed_duplexity_cfg(uint32_t portNum, void* p_params)
{
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    EMAC_DRV_ERR_E retVal;
    EMAC_IOCTL_CMD_T *pCmd = emac_mcb.ioctl_cb.pCmd1Icssg;

    memset(pCmd, 0, sizeof(EMAC_IOCTL_CMD_T));
    pCmd->commandHeader = EMAC_FW_MGMT_CMD_HEADER;
    pCmd->commandType = EMAC_FW_MGMT_SPEED_DUPLEXITY_CMD_TYPE;
    pCmd->seqNumber = pParams->seqNumber;
    pCmd->commandParam = (uint8_t)pParams->subCommand;

    retVal = emac_ioctl_send_mgmt_msg(portNum, pCmd, NULL);

    return retVal;
}

/*
 *  ======== emac_ioctl_send_mgmt_msg ========
 */
EMAC_DRV_ERR_E  emac_ioctl_send_mgmt_msg(uint32_t portNum, EMAC_IOCTL_CMD_T* pMgmtCmd, EMAC_CPPI_DESC_T* p_tx_cppi_desc)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_IOCTL_IN_PROGRESS;
    EMAC_MGMT_PKT_T  *pMgmtPkt;
    EMAC_PORT_INFO_T * pPortInfo = &emac_port_info[portNum];
    int32_t icssgInst = pPortInfo->icssgInst;

    pMgmtPkt = (EMAC_MGMT_PKT_T*)(emac_hwq_pop(icssgInst,((portNum & 1) == 0) ? EMAC_ICSSG_MGMT_FREE_HWQA_PORT0 : EMAC_ICSSG_MGMT_FREE_HWQA_PORT1));
    if (pMgmtPkt == NULL)
    {
        UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb,
                    "port: %d, NO free buffers availalble for sending MGMT message", portNum);
        retVal = EMAC_DRV_RESULT_ERR_NO_FREE_DESC;
    }
    else
    {
        emac_mcb.ioctl_cb.ioctlInProgress = true;
        emac_hw_mem_write((uintptr_t)&(pMgmtPkt->payload), pMgmtCmd, 4);
        emac_hwq_push(icssgInst, ((portNum & 1) == 0) ? EMAC_ICSSG_MGMT_TX_HWQA_PORT0 : EMAC_ICSSG_MGMT_TX_HWQA_PORT1, pMgmtPkt);
    }

    return retVal;
} /* emac_ioctl_send_mgmt_msg */

/* FIXME: FOR now hard-code nubmer of fdb bucket entries and size*/
#define EMAC_ICSSG_NUM_FDB_BUCKET_ENTRIES 4
#define EMAC_ICSSG_SIZE_OF_FDB 2048
/*
 *  ======== emac_ioctl_configure_fdb_ageing_interval ========
 */
EMAC_DRV_ERR_E emac_ioctl_configure_fdb_ageing_interval(uint32_t portNum, void* p_params)
{
    uintptr_t fdbAgeingIntervalAddr;
    uint32_t numFDBBuckets;
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    EMAC_IOCTL_FDB_AGEING_INTERVAL *entry = (EMAC_IOCTL_FDB_AGEING_INTERVAL*)pParams->ioctlVal;
    /*Calculate number of buckets*/
    numFDBBuckets = EMAC_ICSSG_SIZE_OF_FDB / EMAC_ICSSG_NUM_FDB_BUCKET_ENTRIES;

    fdbAgeingIntervalAddr = emac_mcb.port_cb[portNum].icssSharedRamBaseAddr + FDB_AGEING_TIMEOUT_OFFSET;
    /*The actual value written to memory is ageing timeout divided by number of buckets
    because in firmware we iterate per bucket not for entire FDB. See NRT design doc for more details*/
    CSL_REG64_WR(fdbAgeingIntervalAddr, entry->fdbAgeingInterval/(uint64_t)numFDBBuckets);
    return EMAC_DRV_RESULT_OK;

}

/*
 *  ======== emac_ioctl_ingress_rate_limiter_ctrl ========
 */
EMAC_DRV_ERR_E emac_ioctl_ingress_rate_limiter_ctrl(uint32_t portNum, void* p_params)
{
    EMAC_IOCTL_PARAMS *pParams = (EMAC_IOCTL_PARAMS*) p_params;
    EMAC_IOCTL_INGRESS_RATE_LIMIT_T *entry = (EMAC_IOCTL_INGRESS_RATE_LIMIT_T*)pParams->ioctlVal;
    uintptr_t baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE;
    uintptr_t regOffset;
    uint32_t tempRegVal, tempRegMask = ~(0xFF), tempRegShift, i;
    regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_RATE_CFG0_PRU0 \
                                      : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_RATE_CFG0_PRU1 ;
    if (entry->rateIndex < 8)
        regOffset += (entry->rateIndex << 2);
    //Program rate in Mbps
    CSL_REG32_WR (baseAddr+regOffset, (entry->rateLimit * 32768)/250);
    regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_RATE_SRC_SEL0_PRU0 \
                                      : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_RATE_SRC_SEL0_PRU1 ;
    if (entry->rateIndex < 8)
    {
        if (entry->rateIndex < 4)
        {
            tempRegShift = entry->rateIndex << 3 ;
            tempRegMask = ~(0xFF << tempRegShift);
        }
        else if ((entry->rateIndex >= 4) && (entry->rateIndex <= 7))
        {
            tempRegShift = (entry->rateIndex - 4) << 3 ;
            regOffset += 4; //CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_RATE_SRC_SEL1_PRU0/1
            tempRegMask = ~(0xFF << tempRegShift);
        }
        tempRegVal = CSL_REG32_RD (baseAddr+regOffset);
        tempRegVal &= tempRegMask;
        tempRegVal |= (entry->rateSrcSel << tempRegShift);
        CSL_REG32_WR (baseAddr+regOffset, tempRegVal);
    }

    if (entry->classIndex < 16)
    {
        regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_GATES0_PRU0 \
                                          : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_GATES0_PRU1 ;
        regOffset += (entry->classIndex << 2);
        CSL_REG32_WR (baseAddr+regOffset, (0x10 | entry->rateIndex));

        regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_AND_EN_PRU0 \
                                          : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_AND_EN_PRU1 ;
        regOffset += (entry->classIndex << 3);
        CSL_REG32_WR (baseAddr+regOffset, entry->classDataAndTerm);

        regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_OR_EN_PRU0 \
                                          : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS0_OR_EN_PRU1 ;
        regOffset += (entry->classIndex << 3);
        CSL_REG32_WR (baseAddr+regOffset, entry->classDataOrTerm);

        tempRegShift = entry->classIndex << 1 ;
        tempRegMask = ~(0x3 << tempRegShift);

        regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG1_PRU0 \
                                          : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG1_PRU1 ;

        tempRegVal = CSL_REG32_RD (baseAddr+regOffset);
        tempRegVal &= tempRegMask;

        tempRegVal |= (entry->classSel << tempRegShift);
        CSL_REG32_WR (baseAddr+regOffset, tempRegVal);

        regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG2_PRU0 \
                                          : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_CLASS_CFG2_PRU1 ;

        tempRegVal = CSL_REG32_RD (baseAddr+regOffset);
        if (entry->notMask & 1) //AND nv enable
            tempRegVal |= (1 << entry->classIndex);
        if (entry->notMask & 2) //OR nv enable
            tempRegVal |= (1 << (entry->classIndex+16));
        CSL_REG32_WR (baseAddr+regOffset, tempRegVal);
    }

    for (i = 0; i < 2 ; i++)
    {
        if (((entry->filter[i].type == 0) && (entry->filter[i].index >= 8)) ||
            ((entry->filter[i].type == 1) && (entry->filter[i].index >= 16)))
            break;
        if ((entry->filter[i].type == 0) && (entry->filter[i].index < 8))
        {
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_START_LEN_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_START_LEN_PRU1 ;
            tempRegVal = CSL_REG32_RD (baseAddr+regOffset);
            if (tempRegVal != (entry->filter[i].ft1Start | (entry->filter[i].ft1Len << 16)))
            {
                return EMAC_DRV_RESULT_IOCTL_ERR;
            }
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_CFG_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_CFG_PRU1 ;
            tempRegVal = CSL_REG16_RD (baseAddr+regOffset);
            if (tempRegVal != entry->filter[i].ft1Cfg)
            {
                return EMAC_DRV_RESULT_IOCTL_ERR;
            }
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA0_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA0_PRU1 ;
            regOffset += (entry->filter[i].index << 4);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft1.destAddrLow);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA1_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA1_PRU1 ;
            regOffset += (entry->filter[i].index << 4);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft1.destAddrHigh);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA_MASK0_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA_MASK0_PRU1 ;
            regOffset += (entry->filter[i].index << 4);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft1.destAddrMaskLow);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA_MASK1_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_0_DA_MASK1_PRU1 ;
            regOffset += (entry->filter[i].index << 4);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft1.destAddrMaskHigh);

        } else if ((entry->filter[i].type == 1) && (entry->filter[i].index < 16))
        {
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_START_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_START_PRU1 ;
            regOffset += (entry->filter[i].index << 5);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft3.start);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_START_AUTO_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_START_AUTO_PRU1 ;
            regOffset += (entry->filter[i].index << 5);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft3.startAuto);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_START_LEN_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_START_LEN_PRU1 ;
            regOffset += (entry->filter[i].index << 5);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft3.startLen);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_JMP_OFFSET_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_JMP_OFFSET_PRU1 ;
            regOffset += (entry->filter[i].index << 5);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft3.jmpOffset);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_LEN_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_LEN_PRU1 ;
            regOffset += (entry->filter[i].index << 5);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft3.len);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_CFG_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_CFG_PRU1 ;
            regOffset += (entry->filter[i].index << 5);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft3.config);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_T_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_T_PRU1 ;
            regOffset += (entry->filter[i].index << 5);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft3.type);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_T_MASK_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_T_MASK_PRU1 ;
            regOffset += (entry->filter[i].index << 5);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft3.typeMask);

            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_P0_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_P0_PRU1 ;
            regOffset += (entry->filter[i].index << 5);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft3.patternLow);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_P1_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_P1_PRU1 ;
            regOffset += (entry->filter[i].index << 5);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft3.patternHigh);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_P_MASK0_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_P_MASK0_PRU1 ;
            regOffset += (entry->filter[i].index << 5);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft3.patternMaskLow);
            regOffset = ((portNum  & 1) == 0) ? CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_P_MASK1_PRU0 \
                                              : CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT3_0_P_MASK1_PRU1 ;
            regOffset += (entry->filter[i].index << 5);
            CSL_REG32_WR (baseAddr+regOffset, entry->filter[i].u.ft3.patternMaskHigh);
        }
    }

    return EMAC_DRV_RESULT_OK;

}
/* Nothing past this point */

