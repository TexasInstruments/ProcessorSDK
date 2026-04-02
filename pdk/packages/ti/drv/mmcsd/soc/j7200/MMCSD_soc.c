/**
 *  \file   MMCSD_soc.c
 *
 *  \brief  J7200 SoC specific MMCSD hardware attributes.
 *
 *   This file contains the hardware attributes of MMCSD peripheral like
 *   base address, interrupt number etc.
 */

/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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
#include <stdint.h>
#include <ti/drv/mmcsd/MMCSD.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl_mmcsd.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/src/ip/mmc/V4/priv/low_level_hw.h>

#include <ti/drv/mmcsd/soc/MMCSD_soc.h>
#include <ti/drv/sciclient/sciclient.h>
/* MMCSD configuration structure */
MMCSD_v2_HwAttrs MMCSDInitCfg[MMCSD_CNT] =
{
    {
        1,
#if defined (__aarch64__)
        CSL_MMCSD0_CTL_CFG_BASE, /* Controller Registers */
        CSL_MMCSD0_SS_CFG_BASE , /* SS Registers */
        CSL_MMCSD0_SS_CFG_BASE + 0x100 ,/* PHY  Registers */
        CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_MMCSD0_EMMCSS_INTR_0, /* Corresponds to GIC500_SPI_INT_IN_168 */
        0,  /* Event ID is not used */
#else
        CSL_MMCSD0_CTL_CFG_BASE,
        CSL_MMCSD0_SS_CFG_BASE , /* SS Registers */
        CSL_MMCSD0_SS_CFG_BASE + 0x100,/* PHY  Registers */
        CSLR_R5FSS0_CORE1_INTR_MMCSD0_EMMCSS_INTR_0, /* Corresponds to MAIN_R5_CORE1
                                                        This will update dynamically either through
                                                        dedicated interrupt line or query from BoardCfg*/
        0,
#endif
        200000000U,
        25000000U,
        MMCSD_CARD_EMMC,
        MMCSD_BUS_WIDTH_8BIT,
        (MMCSD_BUS_VOLTAGE_1_8V | MMCSD_BUS_VOLTAGE_3_3V),
        1,
        0,
        NULL, /* ioDelay Fn */
        NULL,
        NULL, // MMCSD_inputClockControl,
        1,  /* Enable DMA by default */
        0, /* edmaChRx */
        0, /* edmaChTx */
        0U,
        0U,
        0U,
        0U,
        0U,
        NULL,
#if defined (__aarch64__)
        0, /* Crossbar default for IPU1 */
        0, /* Crossbar mux in */
        0,  /* Cross bar instance number for IRQ66 used in eventNum */
#else
        0, /* Crossbar default for MPU */
        0,/* Crossbar mux in */
        0, /* Cross bar instance number for IRQ83 used in eventNum */
#endif
        MMCSD_SUPPORT_MMC_ALL, /* Supported modes */
        MMCSD_configSocIntrPath,
        MMCSD_HW_PHY,
        MMCSD_AUTO_HW_TUNING,
        0, /*Device Drive Strength*/
        0 /*Phy Drive Strength*/
    },
    {
        2,
#if defined (__aarch64__)
        CSL_MMCSD1_CTL_CFG_BASE,
        CSL_MMCSD1_SS_CFG_BASE , /* SS Registers */
        CSL_MMCSD1_SS_CFG_BASE + 0x100,
        CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_MMCSD1_EMMCSDSS_INTR_0, /* Corresponds to MPU_IRQ_86 (32 + MPU_IRQ_86) */
        0,
#else
        CSL_MMCSD1_CTL_CFG_BASE,
        CSL_MMCSD1_SS_CFG_BASE , /* SS Registers */
        CSL_MMCSD1_SS_CFG_BASE +  0x100,
        0, /* Interrupt number will be updated dynamically */
        0,
#endif
        200000000U,
        25000000U,
        MMCSD_CARD_SD,
        (MMCSD_BUS_WIDTH_1BIT | MMCSD_BUS_WIDTH_4BIT),
        (MMCSD_BUS_VOLTAGE_1_8V | MMCSD_BUS_VOLTAGE_3_3V),
        1,
        0,
        NULL, /* ioDelay Fn */
        NULL,
        NULL, // MMCSD_inputClockControl,
        1,  /* Enable DMA by default */
        0, /* edmaChRx */
        0, /* edmaChTx */
        0U,
        0U,
        0U,
        0U,
        0U,
        NULL,
#if defined (__aarch64__)
        0, /* Crossbar default for IPU1 */
        0, /* Crossbar mux in */
        0,  /* Cross bar instance number for IRQ67 used in eventNum */
#else
        0, /* Crossbar default for MPU */
        0,/* Crossbar mux in */
        0, /* Cross bar instance number for IRQ84 used in eventNum */
#endif
        (MMCSD_SUPPORT_SD_DS | MMCSD_SUPPORT_SD_HS | MMCSD_SUPPORT_SD_SDR50 | MMCSD_SUPPORT_SD_DDR50), /* Supported modes - SDR104 is not supported */
        MMCSD_configSocIntrPath,
        MMCSD_SW_PHY,
        MMCSD_AUTO_HW_TUNING,
        0, /*Device Drive Strength*/
        0 /*Phy Drive Strength*/
    }
};


/* MMCSD objects */
MMCSD_v2_Object MMCSDObjects[MMCSD_CNT];

Bool gIrqRangeQueryFirstTime = UTRUE;

#if defined(BUILD_MCU)
/* This SOC function sets the interrupt path by checking all the available 
   interrupt lines for that core */
static MMCSD_Error MMCSD_configInterruptRouter(uint32_t instNum,  uint32_t eventId, 
                        uint16_t irId, uint16_t dstCoreId, uint16_t rangeStart, 
                            uint16_t rangeNum, uint16_t* coreIntrIdx);

/* This function gets the available interrupt range for that particular core */
static MMCSD_Error MMCSD_getIrqRange(uint16_t irId, uint16_t* rangeStart, uint16_t* rangeNum);
#endif

/* MMC configuration structure */
const MMCSD_Config_list MMCSD_config = {
    {
        &MMCSD_v2_FxnTable,
        &MMCSDObjects[0],
        &MMCSDInitCfg[0]
    },

    {
        &MMCSD_v2_FxnTable,
        &MMCSDObjects[1],
        &MMCSDInitCfg[1]
    },

    {NULL, NULL, NULL}
};

/* This function converts the local L2 address to a global address and
*  will be used for DMA transactions which need a global addresses.
*/
uint32_t MMCSD_soc_l2_global_addr (uint32_t addr)
{
  return addr;
}

/* This function will set interrupt path by checking all the available interrupt lines till we get interrupt line which is free */
MMCSD_Error MMCSD_configInterruptRouter(uint32_t instNum,  uint32_t eventId, 
                uint16_t irId, uint16_t dstCoreId, uint16_t rangeStart, 
                    uint16_t rangeNum, uint16_t* coreIntrIdx)
{
        int32_t retVal = CSL_EFAIL;
        struct tisci_msg_rm_irq_set_req     rmIrqReq;
        struct tisci_msg_rm_irq_set_resp    rmIrqResp;

        memset (&rmIrqReq,0,sizeof(rmIrqReq));       
        rmIrqReq.secondary_host         = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;
        rmIrqReq.src_id = (instNum == 1) ? TISCI_DEV_MMCSD0 : TISCI_DEV_MMCSD1;
        rmIrqReq.src_index = eventId;  /* This is the event coming out of the peripheral */

        /* Set the destination interrupt */ 
        rmIrqReq.valid_params |= TISCI_MSG_VALUE_RM_DST_ID_VALID;
        rmIrqReq.valid_params |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;

        /* Set the destination based on the core */
        rmIrqReq.dst_id       = dstCoreId;

        /* Try to find a free interrupt line */
        for(uint16_t irIntrIdx = rangeStart; irIntrIdx < rangeStart+rangeNum; irIntrIdx++)
        {
            retVal = Sciclient_rmIrqTranslateIrOutput(irId, irIntrIdx, dstCoreId, coreIntrIdx);
            if(CSL_PASS == retVal)
            {
                rmIrqReq.dst_host_irq = *coreIntrIdx;
                retVal = Sciclient_rmIrqSet(
                            (const struct tisci_msg_rm_irq_set_req *)&rmIrqReq, 
                            &rmIrqResp, 
                            SCICLIENT_SERVICE_WAIT_FOREVER);
                if(CSL_PASS == retVal)
                {
                    break;
                }
            }
        }
        return retVal;
}

MMCSD_Error MMCSD_getIrqRange(uint16_t irId, uint16_t* rangeStart, uint16_t* rangeNum)
{
    int32_t retVal;
    MMCSD_Error ret = MMCSD_ERR;
    struct tisci_msg_rm_get_resource_range_resp res = {0};
    struct tisci_msg_rm_get_resource_range_req  req = {0};

    req.type           = irId;
    req.subtype        = (uint8_t)TISCI_RESASG_SUBTYPE_IR_OUTPUT;
    req.secondary_host = (uint8_t)TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

    res.range_num = 0;
    res.range_start = 0;
    /* Get interrupt number range */
    retVal =  Sciclient_rmGetResourceRange(&req, &res, SCICLIENT_SERVICE_WAIT_FOREVER);

    if (CSL_PASS != retVal || res.range_num == 0)
    {
        /* Try with HOST_ID_ALL */
        req.secondary_host = TISCI_HOST_ID_ALL;

        retVal = Sciclient_rmGetResourceRange(&req, &res, SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    if(retVal == CSL_PASS)
    {
        *rangeStart = res.range_start;
        *rangeNum = res.range_num;
        ret = MMCSD_OK;
    }

    return ret;
}

/* This function will configure the interrupt path to the destination CPU
  using SYSFW firmware via sciclient if setIntrPath is set to BTRUE. */
MMCSD_Error MMCSD_configSocIntrPath(const void *hwAttrs_ptr, bool setIntrPath)
{
   MMCSD_Error ret=MMCSD_OK;
#if defined(BUILD_MCU)
    CSL_ArmR5CPUInfo r5CpuInfo = {0};
    int32_t retVal= CSL_PASS;
    MMCSD_v2_HwAttrs const *hwAttrs = (MMCSD_v2_HwAttrs const *)(hwAttrs_ptr);
    bool isIrInvolved = BFALSE;
    static uint16_t        rangeStart;
    static uint16_t        rangeNum;
    static uint16_t        irId;
    static uint16_t        dstCoreId;
    static uint16_t        coreIntrIdx;

    CSL_armR5GetCpuID(&r5CpuInfo);

    /* IR invloved only in mcu1_0 and mcu1_1*/
    if (r5CpuInfo.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        isIrInvolved = BTRUE;
        irId = TISCI_DEV_MAIN2MCU_LVL_INTRTR0;
        dstCoreId = (r5CpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)?
                                TISCI_DEV_MCU_R5FSS0_CORE0:
                                    TISCI_DEV_MCU_R5FSS0_CORE1;
    }

    if(setIntrPath)
    {
        if(isIrInvolved)
        {
            if (gIrqRangeQueryFirstTime)
            {
                retVal = MMCSD_getIrqRange(irId, &rangeStart, &rangeNum); 
                gIrqRangeQueryFirstTime = UFALSE;
            }
            if(retVal == CSL_PASS)
            {
                retVal = MMCSD_configInterruptRouter(hwAttrs->instNum, hwAttrs->eventId, irId, dstCoreId, rangeStart, rangeNum, &coreIntrIdx);
                if(retVal == CSL_PASS)
                {
                    MMCSDInitCfg[hwAttrs->instNum-1].intNum = coreIntrIdx;
                }
            }
        }
        else
        {
            if (r5CpuInfo.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_1) /* Main R5 SS0*/
            {
                /* Configure the Main SS MMCSD instances for mcu2_0 and mcu2_1. R5FSS'n'_CORE'n' */
                /*  ********* MMCSD-0 and MMCSD-1 . intNum = R5FSS'n'_CORE'n' **************** */
                if(r5CpuInfo.cpuID == 0U) 
                {
                        MMCSDInitCfg[hwAttrs->instNum-1].intNum = (hwAttrs->instNum == 1)?
                                            CSLR_R5FSS0_CORE0_INTR_MMCSD0_EMMCSS_INTR_0:
                                                CSLR_R5FSS0_CORE0_INTR_MMCSD1_EMMCSDSS_INTR_0;
                }
                else
                {
                        MMCSDInitCfg[hwAttrs->instNum-1].intNum = (hwAttrs->instNum == 1)?
                                            CSLR_R5FSS0_CORE1_INTR_MMCSD0_EMMCSS_INTR_0:
                                                CSLR_R5FSS0_CORE1_INTR_MMCSD1_EMMCSDSS_INTR_0;
                }
            }
            else
            {
                retVal=CSL_EFAIL;
            }
        }
    }
    else
    {
        if(isIrInvolved)
        {
            struct tisci_msg_rm_irq_release_req rmIrqRelease;
            memset (&rmIrqRelease,0,sizeof(rmIrqRelease));

            rmIrqRelease.secondary_host         = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;
            /* Check if instance MMCSD0 or MMCSD1 */
            rmIrqRelease.src_id = (hwAttrs->instNum == 1)?TISCI_DEV_MMCSD0:TISCI_DEV_MMCSD1;
            rmIrqRelease.src_index = hwAttrs->eventId;  /* This is the event coming out of 
                                                        the peripheral */

            /* Set the destination interrupt */ 
            rmIrqRelease.valid_params |= TISCI_MSG_VALUE_RM_DST_ID_VALID;
            rmIrqRelease.valid_params |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;

            /* Set the destination based on the core */
            /* Since IR is involved only on MCU R5 cores. dst_id can be mcu1_0 or mcu1_1 */
            rmIrqRelease.dst_id       = dstCoreId;
            rmIrqRelease.dst_host_irq = hwAttrs->intNum;
            retVal = Sciclient_rmIrqRelease(
                    (const struct tisci_msg_rm_irq_release_req *)&rmIrqRelease,
                        SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        else
        {
            /* If IR is not involved we don't need to release the interrupt */
            ret = MMCSD_OK;
        }
    }

    if(CSL_PASS != retVal)
    {
        ret = MMCSD_ERR_SET_SOC_INTERRUPT_PATH;
    }
#endif
    return(ret);
}

/* Initializes PHY */
MMCSD_Error MMCSD_socPhyInit(MMCSD_v2_HwAttrs const *hwAttrs)
{
    MMCSD_Error ret = MMCSD_OK;

    if(hwAttrs->phyType == MMCSD_HW_PHY) {
       HSMMCSDPhyInit(hwAttrs->phyBaseAddr);
    } else if(hwAttrs->phyType == MMCSD_SW_PHY) {
       HSMMCSDSoftPhyInit(hwAttrs->phyBaseAddr);        
    }
    return (ret);
}

/* Initializes PHY */
MMCSD_Error MMCSD_socPhyDisableDLL(MMCSD_v2_HwAttrs const *hwAttrs)
{
    MMCSD_Error ret = MMCSD_OK;

    if(hwAttrs->phyType == MMCSD_HW_PHY) {
       HSMMCSDPhyDisableDLL (hwAttrs->phyBaseAddr);
    }
    return (ret);
}


/* Configures PHY */
MMCSD_Error MMCSD_socPhyConfigure(MMCSD_v2_HwAttrs const *hwAttrs,uint32_t phyMode, uint32_t phy_clk_freq, uint32_t phyDriverType)
{
    MMCSD_Error ret = MMCSD_OK;

    if(hwAttrs->phyType == MMCSD_HW_PHY) {
      ret = HSMMCSDPhyConfigure(hwAttrs->phyBaseAddr, phyMode, phy_clk_freq, phyDriverType);
    } else if (hwAttrs->phyType == MMCSD_SW_PHY) {
      ret = HSMMCSDSoftPhyConfigure(hwAttrs->phyBaseAddr, phyMode, phy_clk_freq, phyDriverType);     
    }
    return (ret);
}


#define writel(x,y) (*((uint32_t *)(y))=(x))
#define readl(x) (*((uint32_t *)(x)))

#define ioremap(x,y) (x)
MMCSD_Error mmcsd_tuning_procedure_SD_manual(MMCSD_Handle handle)
{
    static uint32_t secondTime = 0U;
    uint32_t itap, exitN = 0U;
    uint32_t cur_val;
    uint32_t phy_4, phy_5;
    uint32_t max_window = 0U, start_window = 0U, max_len = 0U, length = 0U, prev_match = 0U;

    for (itap = 0U; ((itap < 32U) && (exitN == 0U)); itap++)
    {
        if ( itap == (32U - 1U) )
        {
            itap = max_window + max_len / 2U;       // Choose the center ratio of the largest passing window
            phy_4 = readl(ioremap(0x4fB810cU, 0x4U));
            exitN = 1;
        }

        // Enable Rx DLY Flops
        phy_5 = readl(ioremap(0x4fB8110U, 0x4U));
        phy_5 &= ~(0x3U << 16U);  // Set to 0x0 for DLL mode
        writel(phy_5, ioremap(0x4fB8110U, 0x4U));

        /* Clear ITAP CHGWIN, DLYENA, DLYSEL */
        phy_4 = readl(ioremap(0x4fB810cU, 0x4U));
        phy_4 &= 0xfffffce0;
        writel(phy_4, ioremap(0x4fB810cU, 0x4U));

        phy_4 = readl(ioremap(0x4fB810cU, 0x4U));

        /* Enable CHGWIN */
        phy_4 |= 0x200U;
        writel(phy_4, ioremap(0x4fB810cU, 0x4U));

        /* Update DLYSEL */
        phy_4 |= (itap | 1U << 8U);
        writel(phy_4, ioremap(0x4fB810cU, 0x4U));

        /* Clear CHGWIN */
        phy_4 &= 0xfffffdffU;
        writel(phy_4, ioremap(0x4fB810cU, 0x4U));

        cur_val = !mmcsd_send_tuning(handle);

        if(secondTime < 2U)
        {
#if defined(DEBUG_MANUAL_TUNING_LOG)
           MMCSD_log("itap value:%d %s\n", itap, cur_val ? "PASS" : "FAIL");
#endif
        }
        if(cur_val) {
            if(prev_match) {
                length++;}
            else {
                start_window = itap;
                length = 1U;
            }
        }

        if (length > max_len) {
            max_window = start_window;
            max_len = length;

        }

        prev_match = cur_val;

    }

    secondTime++;
    if (secondTime == 0U)
        secondTime = 2U;

    return MMCSD_OK;
}

MMCSD_Error mmcsd_tuning_procedure_EMMC_manual(MMCSD_Handle handle)
{
    static uint32_t secondTime = 0U;
    uint32_t itap, exitN = 0U;
    uint32_t cur_val;
    uint32_t phy_4, phy_5;
    uint32_t max_window = 0U, start_window = 0U, max_len = 0U, length = 0U, prev_match = 0U;

    for (itap = 0U; ((itap < 32U) && (exitN == 0U)); itap++)
    {
        if ( itap == (32U - 1U) )
        {
            itap = max_window + max_len / 2U;       // Choose the center ratio of the largest passing window
            phy_4 = readl(ioremap(0x4f8810cU, 0x4U));
            exitN = 1U;
        }

        // Enable Rx DLY Flops
        phy_5 = readl(ioremap(0x4f88110U, 0x4U));
        phy_5 &= ~(0x3U << 16U);  // Set to 0x0 for DLL mode
        writel(phy_5, ioremap(0x4f88110U, 0x4U));

        /* Clear ITAP CHGWIN, DLYENA, DLYSEL */
        phy_4 = readl(ioremap(0x4f8810cU, 0x4U));
        phy_4 &= 0xfffffce0U;
        writel(phy_4, ioremap(0x4f8810cU, 0x4U));

        phy_4 = readl(ioremap(0x4f8810cU, 0x4U));

        /* Enable CHGWIN */
        phy_4 |= 0x200U;
        writel(phy_4, ioremap(0x4f8810cU, 0x4U));
        // Osal_delay(100);

        /* Update DLYSEL */
        phy_4 |= (itap | 1U << 8U);
        writel(phy_4, ioremap(0x4f8810cU, 0x4U));
        // Osal_delay(100);

        /* Clear CHGWIN */
        phy_4 &= 0xfffffdffU;
        writel(phy_4, ioremap(0x4f8810cU, 0x4U));

        cur_val = !mmcsd_send_tuning_eMMC(handle);

        if(secondTime < 2U) {
#if defined(DEBUG_MANUAL_TUNING_LOG)
            MMCSD_log("itap value:%d %s\n", itap, cur_val ? "PASS" : "FAIL");
#endif
        }
        if(cur_val) {
            if(prev_match) {
                length++;}
            else {
                start_window = itap;
                length = 1U;
            }
        }

        if (length > max_len) {
            max_window = start_window;
            max_len = length;

        }

        prev_match = cur_val;

    }

    secondTime++;
    if (secondTime == 0U)
        secondTime = 2U;

    return MMCSD_OK;
}
