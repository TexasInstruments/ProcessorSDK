/**
 *  \file   emac_utils.c
 *
 *  \brief  EMAC Driver utility file.
 *
 *   This file contains helper functions internal to EMAC driver.
 */

/*
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
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
#include <ti/csl/cslr_icss.h>
#include <ti/drv/emac/emac_drv.h>
#include <ti/drv/emac/src/v5/emac_drv_v5.h>
#include <ti/drv/emac/emac_ioctl.h>
#include <ti/drv/emac/src/v5/emac_utils.h>
#include <ti/drv/emac/src/v5/emac_hwq.h>

#include <ti/drv/emac/firmware/icss_dualmac/bin_pg2/fw_mem_map.h>

extern EMAC_MCB_V5_T      emac_mcb;
extern EMAC_PORT_INFO_T emac_port_info[];

#define EMAC_ALE_THREAD_DEF_REG     ((uint32_t)0x0003E134U)

#define EMAC_ICSSG_TX_STATS_OFFSET  ((uint32_t)40U)

#define EMAC__RX_PKT_CHAN ((uint32_t)(1U))
#define EMAC_RX_MGMT_CHAN ((uint32_t)(2U))
#define EMAC_RX_MGMT2_CHAN ((uint32_t)(3U))

#define EMAC_RX_RING_MGMT_PSI_RESPONSE ((uint32_t)(1U))
#define EMAC_RX_RING_TX_TS_RESPONSE ((uint32_t)(2U))
#define EMAC_MIN_PKT_SIZE ((uint32_t)(60U))

/* utility to return lower 16 bits of 32 bit crc on array of 8 bytes */
uint16_t emac_util_crc64( uint8_t input[8])
{
    int32_t i;
    uint16_t res = 0x0000;
    int32_t j;
    int32_t k=0;

    uint8_t d[64];
    uint8_t emac_next_c[64];

    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            d[k] =  (input[i] & (1<<j)) ?  1 : 0;
            k+=1;
        }
    }

    emac_next_c[ 0] = d[63] ^ d[57] ^ d[54] ^ d[53] ^ d[51] ^ d[47] ^ d[39] ^ d[38] ^ d[37] ^
                      d[35] ^ d[34] ^ d[33] ^ d[32] ^ d[31] ^ d[29] ^ d[26] ^ d[19] ^ d[18] ^
                      d[16] ^ d[15] ^ d[13] ^ d[10] ^ d[ 9] ^ d[ 8] ^ d[ 5] ^ d[ 3] ^ d[ 2] ^
                      d[ 0];
    emac_next_c[ 1] = d[63] ^ d[62] ^ d[57] ^ d[56] ^ d[54] ^ d[52] ^ d[51] ^ d[50] ^ d[47] ^
                      d[46] ^ d[39] ^ d[36] ^ d[35] ^ d[30] ^ d[29] ^ d[28] ^ d[26] ^ d[25] ^
                      d[19] ^ d[17] ^ d[16] ^ d[14] ^ d[13] ^ d[12] ^ d[10] ^ d[ 7] ^ d[ 5] ^
                      d[ 4] ^ d[ 3] ^ d[ 1] ^ d[ 0];
    emac_next_c[ 2] = d[63] ^ d[62] ^ d[61] ^ d[57] ^ d[56] ^ d[55] ^ d[54] ^ d[50] ^ d[49] ^
                      d[47] ^ d[46] ^ d[45] ^ d[39] ^ d[37] ^ d[33] ^ d[32] ^ d[31] ^ d[28] ^
                      d[27] ^ d[26] ^ d[25] ^ d[24] ^ d[19] ^ d[12] ^ d[11] ^ d[10] ^ d[ 8] ^
                      d[ 6] ^ d[ 5] ^ d[ 4];
    emac_next_c[ 3] = d[62] ^ d[61] ^ d[60] ^ d[56] ^ d[55] ^ d[54] ^ d[53] ^ d[49] ^ d[48] ^
                      d[46] ^ d[45] ^ d[44] ^ d[38] ^ d[36] ^ d[32] ^ d[31] ^ d[30] ^ d[27] ^
                      d[26] ^ d[25] ^ d[24] ^ d[23] ^ d[18] ^ d[11] ^ d[10] ^ d[ 9] ^ d[ 7] ^
                      d[ 5] ^ d[ 4] ^ d[ 3];
    emac_next_c[ 4] = d[63] ^ d[61] ^ d[60] ^ d[59] ^ d[57] ^ d[55] ^ d[52] ^ d[51] ^ d[48] ^
                      d[45] ^ d[44] ^ d[43] ^ d[39] ^ d[38] ^ d[34] ^ d[33] ^ d[32] ^ d[30] ^
                      d[25] ^ d[24] ^ d[23] ^ d[22] ^ d[19] ^ d[18] ^ d[17] ^ d[16] ^ d[15] ^
                      d[13] ^ d[ 6] ^ d[ 5] ^ d[ 4] ^ d[ 0];
    emac_next_c[ 5] = d[62] ^ d[60] ^ d[59] ^ d[58] ^ d[57] ^ d[56] ^ d[53] ^ d[50] ^ d[44] ^
                      d[43] ^ d[42] ^ d[39] ^ d[35] ^ d[34] ^ d[26] ^ d[24] ^ d[23] ^ d[22] ^
                      d[21] ^ d[19] ^ d[17] ^ d[14] ^ d[13] ^ d[12] ^ d[10] ^ d[ 9] ^ d[ 8] ^
                      d[ 4] ^ d[ 2] ^ d[ 0];
    emac_next_c[ 6] = d[62] ^ d[61] ^ d[59] ^ d[58] ^ d[57] ^ d[56] ^ d[55] ^ d[52] ^ d[49] ^
                      d[43] ^ d[42] ^ d[41] ^ d[38] ^ d[34] ^ d[33] ^ d[25] ^ d[23] ^ d[22] ^
                      d[21] ^ d[20] ^ d[18] ^ d[16] ^ d[13] ^ d[12] ^ d[11] ^ d[ 9] ^ d[ 8] ^
                      d[ 7] ^ d[ 3] ^ d[ 1];
    emac_next_c[ 7] = d[63] ^ d[61] ^ d[60] ^ d[58] ^ d[56] ^ d[55] ^ d[53] ^ d[48] ^ d[47] ^
                      d[42] ^ d[41] ^ d[40] ^ d[39] ^ d[38] ^ d[35] ^ d[34] ^ d[31] ^ d[29] ^
                      d[26] ^ d[24] ^ d[22] ^ d[21] ^ d[20] ^ d[18] ^ d[17] ^ d[16] ^ d[13] ^
                      d[12] ^ d[11] ^ d[ 9] ^ d[ 7] ^ d[ 6] ^ d[ 5] ^ d[ 3];
    emac_next_c[ 8] = d[63] ^ d[62] ^ d[60] ^ d[59] ^ d[55] ^ d[53] ^ d[52] ^ d[51] ^ d[46] ^
                      d[41] ^ d[40] ^ d[35] ^ d[32] ^ d[31] ^ d[30] ^ d[29] ^ d[28] ^ d[26] ^
                      d[25] ^ d[23] ^ d[21] ^ d[20] ^ d[18] ^ d[17] ^ d[13] ^ d[12] ^ d[11] ^
                      d[ 9] ^ d[ 6] ^ d[ 4] ^ d[ 3] ^ d[ 0];
    emac_next_c[ 9] = d[62] ^ d[61] ^ d[59] ^ d[58] ^ d[54] ^ d[52] ^ d[51] ^ d[50] ^ d[45] ^
                      d[40] ^ d[39] ^ d[34] ^ d[31] ^ d[30] ^ d[29] ^ d[28] ^ d[27] ^ d[25] ^
                      d[24] ^ d[22] ^ d[20] ^ d[19] ^ d[17] ^ d[16] ^ d[12] ^ d[11] ^ d[10] ^
                      d[ 8] ^ d[ 5] ^ d[ 3] ^ d[ 2];
    emac_next_c[10] = d[63] ^ d[61] ^ d[60] ^ d[58] ^ d[54] ^ d[50] ^ d[49] ^ d[47] ^ d[44] ^
                      d[37] ^ d[35] ^ d[34] ^ d[32] ^ d[31] ^ d[30] ^ d[28] ^ d[27] ^ d[24] ^
                      d[23] ^ d[21] ^ d[13] ^ d[11] ^ d[ 8] ^ d[ 7] ^ d[ 5] ^ d[ 4] ^ d[ 3] ^
                      d[ 1] ^ d[ 0];
    emac_next_c[11] = d[63] ^ d[62] ^ d[60] ^ d[59] ^ d[54] ^ d[51] ^ d[49] ^ d[48] ^ d[47] ^
                      d[46] ^ d[43] ^ d[39] ^ d[38] ^ d[37] ^ d[36] ^ d[35] ^ d[32] ^ d[30] ^
                      d[27] ^ d[23] ^ d[22] ^ d[20] ^ d[19] ^ d[18] ^ d[16] ^ d[15] ^ d[13] ^
                      d[12] ^ d[ 9] ^ d[ 8] ^ d[ 7] ^ d[ 6] ^ d[ 5] ^ d[ 4];
    emac_next_c[12] = d[63] ^ d[62] ^ d[61] ^ d[59] ^ d[58] ^ d[57] ^ d[54] ^ d[51] ^ d[50] ^
                      d[48] ^ d[46] ^ d[45] ^ d[42] ^ d[39] ^ d[36] ^ d[33] ^ d[32] ^ d[22] ^
                      d[21] ^ d[17] ^ d[16] ^ d[14] ^ d[13] ^ d[12] ^ d[11] ^ d[10] ^ d[ 9] ^
                      d[ 7] ^ d[ 6] ^ d[ 4] ^ d[ 2] ^ d[ 0];
    emac_next_c[13] = d[61] ^ d[60] ^ d[58] ^ d[57] ^ d[56] ^ d[53] ^ d[50] ^ d[49] ^ d[47] ^
                      d[45] ^ d[44] ^ d[41] ^ d[38] ^ d[35] ^ d[32] ^ d[31] ^ d[21] ^ d[20] ^
                      d[16] ^ d[15] ^ d[13] ^ d[12] ^ d[11] ^ d[10] ^ d[ 9] ^ d[ 8] ^ d[ 6] ^
                      d[ 5] ^ d[ 3] ^ d[ 1];
    emac_next_c[14] = d[61] ^ d[60] ^ d[59] ^ d[57] ^ d[56] ^ d[55] ^ d[52] ^ d[49] ^ d[48] ^
                      d[46] ^ d[44] ^ d[43] ^ d[40] ^ d[37] ^ d[34] ^ d[31] ^ d[30] ^ d[20] ^
                      d[19] ^ d[15] ^ d[14] ^ d[12] ^ d[11] ^ d[10] ^ d[ 9] ^ d[ 8] ^ d[ 7] ^
                      d[ 5] ^ d[ 4] ^ d[ 2] ^ d[ 0];
    emac_next_c[15] = d[60] ^ d[59] ^ d[58] ^ d[56] ^ d[55] ^ d[54] ^ d[51] ^ d[48] ^ d[47] ^
                      d[45] ^ d[43] ^ d[42] ^ d[39] ^ d[36] ^ d[33] ^ d[30] ^ d[29] ^ d[19] ^
                      d[18] ^ d[14] ^ d[13] ^ d[11] ^ d[10] ^ d[ 9] ^ d[ 8] ^ d[ 7] ^ d[ 6] ^
                      d[ 4] ^ d[ 3] ^ d[ 1];

  /* assign crc_result = emac_next_c[15:0]; */
  /* so we walk the lower 16 bytes and pack into 16 bits */
    for(i=0;i<16;i++)
    {
        res |= (emac_next_c[i] ? (1<<i): 0) ;
    }
    return res;
}



/*************************************************************************************
 * \brief emac_util_fdb_helper: return broad-side slot to use for fdb entry
 *        also return the 8 bit fid to populate as part of the entry
 *
 * \param vlan_table pointer to base of vlan table.
 * \param vlan_id
 * \param mac 6 byte mac address
 * \param p_fid pointer to uint8_t where we ruturn the fid
 *
   \retval return broad-side slot to use for fdb entry
 * NOTE: only supports the 2048 entry x 4 entries/broadside slot fdb configuration
 **************************************************************************************/
uint16_t emac_util_fdb_helper(uintptr_t vlan_table, uint16_t vlan_id,  uint8_t mac[], uint8_t * p_fid)
{
    uint16_t* pTable = (uint16_t*)vlan_table;
    uint8_t input_64[8];  //64 bit input
    *p_fid = (pTable[vlan_id]&0xff00)>>8;

    /* juggle input be->le */
    input_64[0] = mac[0];
    input_64[1] = mac[1];
    input_64[2] = mac[2];
    input_64[3] = mac[3];
    input_64[4] = mac[4];
    input_64[5] = mac[5];

    input_64[6] = *p_fid;
    input_64[7] = 0x00;
    return (0x1ff&emac_util_crc64(input_64));
}

/* commands to program ICSSG R30 registers */
typedef struct EMAC_R30_CMD_S {
    volatile uint32_t cmd[4];
} EMAC_R30_CMD_T;

static EMAC_R30_CMD_T emac_R30_bitmask[EMAC_PORT_MAX_COMMANDS] =
{
    {{0xffff0004, 0xffff0100, 0xffff0104 ,EMAC_NONE}},      /* EMAC_PORT_DISABLE */
    {{0xfffb0040, 0xfeff0200, 0xfefb0208 ,EMAC_NONE}},      /* EMAC_PORT_BLOCK */
    {{0xffbb0000, 0xfcff0000, 0xdcf30000 ,EMAC_NONE}},      /* EMAC_PORT_FORWARD */
    {{0xffbb0000, 0xfcff0000, 0xfcf32000 ,EMAC_NONE}},      /* EMAC_PORT_FORWARD_WO_LEARNING */
    {{0xffff0001, EMAC_NONE,  0xffff0001 ,EMAC_NONE}},      /* ACCEPT ALL */
    {{0xfffe0002, EMAC_NONE,  0xfffe0002 ,EMAC_NONE}},      /* ACCEPT TAGGED */
    {{0xfffc0000, EMAC_NONE,  0xfffc0000 ,EMAC_NONE}},      /* ACCEPT UNTAGGED and PRIO */
    {{EMAC_NONE,  0xffff0020, EMAC_NONE  ,EMAC_NONE}},      /*TAS Trigger List change */
    {{EMAC_NONE,  0xdfff1000, EMAC_NONE  ,EMAC_NONE}},      /*TAS set state ENABLE*/
    {{EMAC_NONE,  0xefff2000, EMAC_NONE  ,EMAC_NONE}},      /*TAS set state RESET*/
    {{EMAC_NONE,  0xcfff0000, EMAC_NONE  ,EMAC_NONE}},      /*TAS set state DISABLE*/
    {{EMAC_NONE,  EMAC_NONE,  0xffff0400 ,EMAC_NONE}},      /*UC flooding ENABLE*/
    {{EMAC_NONE,  EMAC_NONE,  0xfbff0000 ,EMAC_NONE}},      /*UC flooding DISABLE*/
    {{EMAC_NONE,  EMAC_NONE,  0xffff0800 ,EMAC_NONE}},      /*MC flooding ENABLE*/
    {{EMAC_NONE,  EMAC_NONE,  0xf7ff0000 ,EMAC_NONE}},      /*MC flooding DISABLE*/
    {{EMAC_NONE,  0xffff4000, EMAC_NONE  ,EMAC_NONE}},      /*Premption on Tx ENABLE*/
    {{EMAC_NONE,  0xbfff0000, EMAC_NONE  ,EMAC_NONE}}        /*Premption on Tx DISABLE*/
};

#ifdef SOC_AM65XX
static EMAC_R30_CMD_T *emac_R30_dmem[6] = {
    (EMAC_R30_CMD_T *)( CSL_PRU_ICSSG0_DRAM0_SLV_RAM_BASE + MGR_R30_CMD_OFFSET),
    (EMAC_R30_CMD_T *)( CSL_PRU_ICSSG0_DRAM1_SLV_RAM_BASE + MGR_R30_CMD_OFFSET),
    (EMAC_R30_CMD_T *)( CSL_PRU_ICSSG1_DRAM0_SLV_RAM_BASE + MGR_R30_CMD_OFFSET),
    (EMAC_R30_CMD_T *)( CSL_PRU_ICSSG1_DRAM1_SLV_RAM_BASE + MGR_R30_CMD_OFFSET),
    (EMAC_R30_CMD_T *)( CSL_PRU_ICSSG2_DRAM0_SLV_RAM_BASE + MGR_R30_CMD_OFFSET),
    (EMAC_R30_CMD_T *)( CSL_PRU_ICSSG2_DRAM1_SLV_RAM_BASE + MGR_R30_CMD_OFFSET)
};
#else
static EMAC_R30_CMD_T *emac_R30_dmem[4] = {
    (EMAC_R30_CMD_T *)( CSL_PRU_ICSSG0_DRAM0_SLV_RAM_BASE + MGR_R30_CMD_OFFSET),
    (EMAC_R30_CMD_T *)( CSL_PRU_ICSSG0_DRAM1_SLV_RAM_BASE + MGR_R30_CMD_OFFSET),
    (EMAC_R30_CMD_T *)( CSL_PRU_ICSSG1_DRAM0_SLV_RAM_BASE + MGR_R30_CMD_OFFSET),
    (EMAC_R30_CMD_T *)( CSL_PRU_ICSSG1_DRAM1_SLV_RAM_BASE + MGR_R30_CMD_OFFSET)
};
#endif

void emac_R30_cmd_init(uint32_t portNum)
{
    int j;
    for ( j = 0; j < 4; j++)
        emac_R30_dmem[portNum]->cmd[j] = EMAC_NONE; 
}

/*
 * return 1 of all cores are finished the command
 * otherwise return 0
 */
int32_t emac_R30_is_done(uint32_t portNum)
{
    int j, ret = 1;
    for ( j = 0; j < 4; j++) {
        if (emac_R30_dmem[portNum]->cmd[j] == EMAC_NONE)
            continue;

        ret = 0;
        break;
    }
    return ret;
}

/*
 * emac_send_R30_cmd
 */
EMAC_DRV_ERR_E emac_send_R30_cmd(EMAC_IOctlR30Cmd cmd, uint32_t portNum, uint8_t sequenceNumber)
{
    int j;
    emac_mcb.ioctl_cb.ioctlInProgress = true;
    emac_mcb.ioctl_cb.sequenceNumber = sequenceNumber;
    emac_mcb.ioctl_cb.internalIoctl = 0;
    emac_mcb.port_cb[portNum].ioctlType = EMAC_ICSSG_IOCTL_TYPE_R30_OVER_DMEM;
    for (j = 0; j < 4; j++)
    {
        emac_R30_dmem[portNum]->cmd[j] = emac_R30_bitmask[cmd].cmd[j];
    }

    return EMAC_DRV_RESULT_IOCTL_IN_PROGRESS;
}

#if defined (EMAC_INCOHERENT)
inline void emac_cache_alignment_wb_inv(void *ptr, uint32_t len)
{
    uintptr_t p = (uintptr_t)ptr;
    uint32_t delta;

    delta = p & 0x3f;
    p &= ~0x3f;

    len += delta;

    delta = len & 0x3f;
    if (delta) {
      delta = 64 - delta;
    }  /* else len already aligned */
  

    len += delta;
    ptr = (void *)p;
    EMAC_osalCacheWbInv(ptr, len);
}

inline void emac_cache_alignment_inv(void *ptr, uint32_t len)
{
    uintptr_t p = (uintptr_t)ptr;
    uint32_t delta;

    delta = p & 0x3f;
    p &= ~0x3f;

    len += delta;

    delta = len & 0x3f;
    if (delta) {
      delta = 64 - delta;
    }  /* else len already aligned */
  
    len += delta;
    ptr = (void *)p;
    EMAC_osalCacheInv(ptr, len);
}
#endif

/*
 *  ======== emac_hw_mem_write ========
 */
void emac_hw_mem_write(uintptr_t addr, const void *ptr, uint32_t element_count)
{
    uint32_t  i;
    volatile uint32_t *dst = (volatile uint32_t *)addr;
    uint32_t *src = (uint32_t *)ptr;
    for (i = 0; i < element_count; i++)
    {
        *dst++ = *src++;
    }
}

/*
 *  ======== EMAC_rxIsrFxn ========
 */
void EMAC_rxIsrFxn(Udma_EventHandle  eventHandle,
                                 uint32_t          eventType,
                                 void             *appData)
{
    uint32_t portNum;
    uint32_t ringNum = 0;

    uintptr_t intArg = (uintptr_t)appData;
    portNum = (intArg  & 0xFFFF0000U  )>> 16U;
    ringNum = intArg & 0x0000FFFFU;
    EMAC_osalPostLock(EMAC_GLOBAL_RX_SEM_HANDLE(portNum, ringNum));
}


/*
 *  ======== EMAC_rxMgmtIsrFxn ========
 */
void EMAC_rxMgmtIsrFxn(Udma_EventHandle  eventHandle,
                                 uint32_t          eventType,
                                 void             *appData)
{
    uint32_t portNum;
    uint32_t ringNum = 0;

    uintptr_t intArg = (uintptr_t)appData;
    portNum = (intArg  & 0xFFFF0000U  )>> 16U;
    ringNum = intArg & 0x0000FFFFU;
    EMAC_osalPostLock(EMAC_GLOBAL_RX_MGMT_SEM_HANDLE(portNum, ringNum));
}

/*
 *  ======== emac_pkt_too_big ========
 */
static inline bool emac_pkt_too_big(uint32_t portNum, EMAC_CPPI_DESC_T *pCppiDesc, uint32_t *pPktSize)
{
    bool tooBig = FALSE;
    *pPktSize = CSL_FEXT (pCppiDesc->hostDesc.descInfo, UDMAP_CPPI5_PD_DESCINFO_PKTLEN);

    /* Subtract 4 byte CRC from packet size*/
    *pPktSize -= 4;
    if ((*pPktSize) > (EMAC_PKT_SIZE(portNum) + EMAC_ETHHDR_SIZE))
    {
        tooBig = TRUE;
    }
    return tooBig;
}

/*
 *  ======== emac_cppi5InitHostDescQueue ========
 */
void emac_cppi5InitHostDescQueue( uint32_t portNum, uint32_t retqIdx, uint32_t descCnt,
                     uint32_t descSize, void* descPool, uint32_t ringNum,
                     Udma_RingHandle ringHandle, void (*pfPush)(uint32_t, uint32_t, physptr_t,
                     uint32_t, Udma_RingHandle))
{
    CSL_UdmapCppi5HMPD *pHpd;
    uint32_t        i;
    uint32_t descType;

    if( descSize == 0U)
    {
        descSize = sizeof(CSL_UdmapCppi5HMPD);   /* Use default size of descriptor if descSize==0 */
    }
    pHpd   = (CSL_UdmapCppi5HMPD *)descPool;
    if (descCnt != 0)
    {
        memset (pHpd, 0, descCnt * descSize);

        for(i=0; i<descCnt; i++)
        {
            descType = (uint32_t)CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST;
            CSL_udmapCppi5SetDescType(pHpd, descType);
            CSL_udmapCppi5SetReturnPolicy(
                                pHpd,
                                descType,
                                CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                retqIdx);
            CSL_udmapCppi5SetOrgBufferAddr(pHpd, (uint64_t) pHpd->bufPtr);

            pfPush(portNum, 0, Emac_osalVirtToPhys(pHpd), ringNum, ringHandle );
            pHpd = (CSL_UdmapCppi5HMPD *)(((uint8_t *)pHpd) + descSize);
        }
    }
} /* my_cppi5InitHostDescQueue */

static volatile uint32_t emac_rx_free_push_errors = 0;

/*
 *  ======== emac_rx_free_push ========
 */
void
emac_rx_free_push(uint32_t portNum, uint32_t size, physptr_t phys, uint32_t ringNum, Udma_RingHandle rxRingHandle)
{
    EMAC_CPPI_DESC_T *pCppiDesc = (EMAC_CPPI_DESC_T *)Emac_osalPhysToVirt (phys);
    EMAC_PKT_DESC_T  *p_rx_pkt_desc;


    /* Allocate the PKT */
    if ((p_rx_pkt_desc = EMAC_ALLOC_PKT(portNum, EMAC_PKT_SIZE(portNum))) != NULL)
    {
        /* Populate the Rx free descriptor with the buffer we just allocated. */
        pCppiDesc->appPtr             = p_rx_pkt_desc;
        pCppiDesc->hostDesc.bufPtr    = (uint64_t)p_rx_pkt_desc->pDataBuffer;
        pCppiDesc->hostDesc.orgBufPtr = (uint64_t)p_rx_pkt_desc->pDataBuffer;
        pCppiDesc->hostDesc.orgBufLen = p_rx_pkt_desc->BufferLen;
        pCppiDesc->hostDesc.bufInfo1  = p_rx_pkt_desc->BufferLen;
        /* Push descriptor to Rx free queue */

        emac_udma_ring_enqueue (rxRingHandle,
                                pCppiDesc,
                                pCppiDesc->hostDesc.orgBufLen);
    }
    else
    {
        UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d: RX pkt allocation error during free ring setup",portNum);
        emac_rx_free_push_errors |= (1u << portNum);
    }
}

/*
 *  ======== emac_tx_ready_push ========
 */
void emac_tx_ready_push(uint32_t portNum, uint32_t size, physptr_t phys, uint32_t ringNum, Udma_RingHandle rxRingHandle)
{
    EMAC_CPPI_DESC_T *pCppiDesc = (EMAC_CPPI_DESC_T *)Emac_osalPhysToVirt (phys);

    pCppiDesc->nextPtr = emac_mcb.port_cb[portNum].txReadyDescs[ringNum];
    emac_mcb.port_cb[portNum].txReadyDescs[ringNum] = pCppiDesc;
} /* emac_tx_ready_push */

/*
 *  ======== emac_get_hw_cppi_tx_desc ========
 */
void emac_free_hw_cppi_tx_desc(uint32_t p1, uint32_t ch1, EMAC_CPPI_DESC_T* pCppiDesc)
{
    uintptr_t key;
    key = EMAC_osalHardwareIntDisable();
    pCppiDesc->nextPtr = emac_mcb.port_cb[p1].txReadyDescs[ch1];
    emac_mcb.port_cb[p1].txReadyDescs[ch1] = pCppiDesc;
    EMAC_osalHardwareIntRestore(key);
}

/*
 *  ======== emac_get_hw_cppi_tx_desc ========
 */
void emac_get_hw_cppi_tx_desc(uint32_t p1, uint32_t ch1, EMAC_CPPI_DESC_T** pCppiDesc)
{
    EMAC_CPPI_DESC_T *temp1;
    uintptr_t key;
    key = EMAC_osalHardwareIntDisable();
    *pCppiDesc = emac_mcb.port_cb[p1].txReadyDescs[ch1];
    if(*pCppiDesc)
    {
        temp1 = *pCppiDesc;
        emac_mcb.port_cb[p1].txReadyDescs[ch1] = temp1->nextPtr;
    }
    EMAC_osalHardwareIntRestore(key);
}

/*
 *  ======== emac_get_hw_cppi_tx_descs ========
 */

bool emac_get_hw_cppi_tx_descs(uint32_t p1, uint32_t ch1, uint32_t p2, uint32_t ch2, EMAC_CPPI_DESC_T** pCppiDesc1, EMAC_CPPI_DESC_T** pCppiDesc2)
{
    bool retVal = FALSE;
    uintptr_t key;
    EMAC_CPPI_DESC_T *temp1, *temp2;

    key = EMAC_osalHardwareIntDisable();
    *pCppiDesc1 = emac_mcb.port_cb[p1].txReadyDescs[ch1];
    *pCppiDesc2 = emac_mcb.port_cb[p2].txReadyDescs[ch2];
    if(*pCppiDesc1 && *pCppiDesc2)
    {
        temp1 = *pCppiDesc1;
        temp2 = *pCppiDesc2;
        emac_mcb.port_cb[p1].txReadyDescs[ch1] = temp1->nextPtr;
        emac_mcb.port_cb[p2].txReadyDescs[ch2] = temp2->nextPtr;
        retVal = TRUE;
    }
    EMAC_osalHardwareIntRestore(key);

    return retVal;
}



/*
 *  ======== emac_udma_ring_enqueue ========
 */
EMAC_DRV_ERR_E
emac_udma_ring_enqueue
(
    Udma_RingHandle ringHandle,
    EMAC_CPPI_DESC_T *pHostDescriptor,
    uint32_t            packetSize
)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
#if defined (EMAC_INCOHERENT)
    void *virtBufPtr;
#endif
    physptr_t  physDescPtr;
    if (ringHandle == NULL)
    {
        retVal = EMAC_DRV_RESULT_ERR_UDMA_RING_ENQUEUE;
    }
    if (retVal == EMAC_DRV_RESULT_OK)
    {
#if defined (EMAC_INCOHERENT)
        virtBufPtr = (void *)(uintptr_t)pHostDescriptor->hostDesc.bufPtr;
#endif

        pHostDescriptor->hostDesc.bufPtr = Emac_osalVirtToPhys ((void *)(uintptr_t)pHostDescriptor->hostDesc.bufPtr);
        pHostDescriptor->hostDesc.orgBufPtr = Emac_osalVirtToPhys ((void *)(uintptr_t)pHostDescriptor->hostDesc.orgBufPtr);
        physDescPtr = (uint64_t) Emac_osalVirtToPhys ((void *)&pHostDescriptor->hostDesc);

#if defined (EMAC_INCOHERENT)
            /* Wb Invdata cache */
        emac_cache_alignment_wb_inv(virtBufPtr, packetSize);
        emac_cache_alignment_wb_inv((void *)&pHostDescriptor->hostDesc, sizeof(*pHostDescriptor));
#endif

        if (Udma_ringQueueRaw(ringHandle,(uint64_t)physDescPtr) != 0)
        {
            retVal = EMAC_DRV_RESULT_ERR_UDMA_RING_ENQUEUE;
        }
    }
    return retVal;
} /* emac_udma_ring_enqueue */


/*
 *  ======== emac_udma_ring_dequeue ========
 */
int32_t
emac_udma_ring_dequeue
(
    Udma_RingHandle ringHandle,
    EMAC_CPPI_DESC_T **pHostDescriptor
)
{
    uint64_t pDesc = 0;
    EMAC_CPPI_DESC_T *pVirtHostDesc;

    if (ringHandle == NULL)
    {
        return -1;
    }
    Udma_ringDequeueRaw(ringHandle,&pDesc);
    if(pDesc == 0)
     {
        *pHostDescriptor = NULL;
    }
    else
    {
        *pHostDescriptor = pVirtHostDesc = (EMAC_CPPI_DESC_T *)Emac_osalPhysToVirt (pDesc);

#if defined (EMAC_INCOHERENT)
        emac_cache_alignment_inv((void *) pVirtHostDesc, sizeof(EMAC_CPPI_DESC_T));
#endif
        pVirtHostDesc->hostDesc.bufPtr = (uint64_t)Emac_osalPhysToVirt
        (pVirtHostDesc->hostDesc.bufPtr);
        pVirtHostDesc->hostDesc.orgBufPtr = (uint64_t)Emac_osalPhysToVirt
        (pVirtHostDesc->hostDesc.orgBufPtr);

#if defined (EMAC_INCOHERENT)
        uint32_t    pktsize  = CSL_FEXT (pVirtHostDesc->hostDesc.descInfo, UDMAP_CPPI5_PD_DESCINFO_PKTLEN);
        emac_cache_alignment_inv((void *)(uintptr_t)pVirtHostDesc->hostDesc.bufPtr, (int32_t)pktsize);
#endif
    }

    return 0;
} /* emac_ring_pop */

/*
 *  ======== emac_free_ring_desc========
 */
static void
emac_cleanup_comp_ring (uint32_t portNum, Udma_RingHandle ringHandle)
{
    EMAC_CPPI_DESC_T *pCppiDesc;

    do
    {
        /* Go through the Rx Completion queue */
        pCppiDesc = NULL;
        if ((emac_udma_ring_dequeue (ringHandle, &pCppiDesc)) == 0)
        {
            if (pCppiDesc != NULL)
            {
                emac_mcb.port_cb[portNum].free_pkt_cb(portNum, pCppiDesc->appPtr);
                pCppiDesc->nextPtr = NULL;
            }
        }
    } while (pCppiDesc != NULL);

}

/*
 *  ======== emac_cleanup_free_ring ========
 */
void emac_cleanup_free_ring(uint32_t portNum, uint64_t *pRingMem)
{
    uint32_t descNum;
    EMAC_CPPI_DESC_T *pCppiDesc;

    if (pRingMem)
    {
        /* free all the packet descriptors back to the application */
        for (descNum = 0; descNum < emac_mcb.port_cb[portNum].num_of_rx_pkt_desc;descNum++)
        {
            pCppiDesc = (EMAC_CPPI_DESC_T*)(uintptr_t)pRingMem[descNum];
            emac_mcb.port_cb[portNum].free_pkt_cb(portNum, pCppiDesc->appPtr);
        }
    }
}

/*
 *  ======== emac_setup_tx_subsystem ========
 */
EMAC_DRV_ERR_E
emac_setup_tx_subsystem (uint32_t                 portNum,
                        EMAC_OPEN_CONFIG_INFO_T *p_config,
                        EMAC_HwAttrs_V5         *pHwAttrs)
{
    int32_t retVal = EMAC_DRV_RESULT_OK;
    int32_t chanNum;
    Udma_ChHandle txChHandle;

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    emac_mcb.port_cb[portNum].numTxChans = pHwAttrs->portCfg[portNum].nTxChans;
    for (chanNum = 0; chanNum < pHwAttrs->portCfg[portNum].nTxChans; chanNum++)
    {
        txChHandle = (Udma_ChHandle)(pHwAttrs->portCfg[portNum].txChannel[chanNum].chHandle);
        emac_mcb.port_cb[portNum].txChHandle[chanNum] = txChHandle;
        retVal = emac_open_udma_channel_tx(portNum,
                                &(pHwAttrs->portCfg[portNum].txChannel[chanNum]));
        if (retVal == EMAC_DRV_RESULT_OK)
        {
            emac_mcb.port_cb[portNum].pollTable.txCompletion[chanNum].ringPollFxn = emac_poll_tx_complete;
            emac_mcb.port_cb[portNum].pollTable.txCompletion[chanNum].compRingHandle =txChHandle->cqRing;

            emac_mcb.port_cb[portNum].txReadyDescs[chanNum] = NULL;
            emac_cppi5InitHostDescQueue(portNum,
                                      txChHandle->cqRing->ringNum,
                                      p_config->num_of_tx_pkt_desc,
                                      EMAC_CPPI_DESC_SIZE,
                                      pHwAttrs->portCfg[portNum].txChannel[chanNum].hPdMem,
                                      chanNum,
                                      NULL,
                                      emac_tx_ready_push);
        }
        else
        {
            UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d: UDMA TX channel open error", portNum);
            break;
        }
    }
    /* All done with Tx configuration. Return success. */
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",
                                            portNum, retVal);

    return (EMAC_DRV_ERR_E)retVal;
} /* emac_setup_tx_subsystem */

/*
 *  ======== emac_close_tx_subsystem ========
 */
int32_t
emac_close_tx_subsystem (uint32_t portNum)
{
    EMAC_CPPI_DESC_T *pCppiDesc;
    uint32_t chanNum;
    Udma_ChHandle txChHandle;
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    for (chanNum = 0; chanNum < emac_mcb.port_cb[portNum].numTxChans;chanNum++)
    {
        txChHandle = emac_mcb.port_cb[portNum].txChHandle[chanNum];
        /* Lets free back resources to the application prior to resetting the rings */
        do
        {
            /* Go through the TX Completion queue */
            pCppiDesc = NULL;
            if ((emac_udma_ring_dequeue (Udma_chGetCqRingHandle(txChHandle), &pCppiDesc)) == 0)
            {
                if (pCppiDesc)
                {
                    emac_mcb.port_cb[portNum].free_pkt_cb(portNum, pCppiDesc->appPtr);
                    pCppiDesc->nextPtr =  emac_mcb.port_cb[portNum].txReadyDescs[chanNum];
                    emac_mcb.port_cb[portNum].txReadyDescs[chanNum] = pCppiDesc;
                }
            }
        } while (pCppiDesc != NULL);

        emac_mcb.port_cb[portNum].txReadyDescs[chanNum] = NULL;
        Udma_chClose(txChHandle);
    }
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT",portNum);
    return 0;
}


/*
 *  ======== emac_setup_additional_flows ========
 */
int32_t
emac_setup_additional_flows(uint32_t portNum, EMAC_PER_CHANNEL_CFG_RX*pChCfg, uint32_t channelType, EMAC_CHANNEL_RX_T* pRxChannel)
{
    Udma_RingHandle freeRingHandle, completionRingHandle;
    Udma_FlowPrms flowPrms;
    Udma_RingPrms ringPrms;
    uint32_t subChanNum;
    int32_t retVal = EMAC_DRV_RESULT_OK;
    if (pChCfg->nsubChan > 1)
    {
        /* First allocate flow */
        pRxChannel->flowHandle = (Udma_FlowHandle)pChCfg->flowHandle;
        retVal = Udma_flowAlloc(emac_mcb.port_cb[portNum].udmaHandle,pRxChannel->flowHandle, pChCfg->nsubChan -1);
        if (retVal == 0)
        {
            /* subChan 0 is the default flow, start with subChan 1 for additinal flows*/
            for (subChanNum = 1; subChanNum < pChCfg->nsubChan;subChanNum++)
            {
                 freeRingHandle = (Udma_RingHandle)(pChCfg->subChan[subChanNum].freeRingHandle[0]);
                 completionRingHandle = (Udma_RingHandle)(pChCfg->subChan[subChanNum].compRingHandle);
 
                if (channelType == EMAC__RX_PKT_CHAN)
                {
                    emac_mcb.port_cb[portNum].pollTable.rxPkt[subChanNum].freeRingHandle = freeRingHandle;
                    emac_mcb.port_cb[portNum].pollTable.rxPkt[subChanNum].compRingHandle= completionRingHandle;
                    emac_mcb.port_cb[portNum].pollTable.rxPkt[subChanNum].ringPollFxn =emac_poll_rx_pkts;
                    emac_mcb.port_cb[portNum].rxPktCh.subChan[subChanNum].freeRingMem[0] = pChCfg->subChan[subChanNum].freeRingMem[0];
                    emac_mcb.port_cb[portNum].rxPktCh.subChan[subChanNum].compRingMem = pChCfg->subChan[subChanNum].compRingMem;
                }
                else if (channelType == EMAC_RX_MGMT_CHAN)
                {
                    emac_mcb.port_cb[portNum].pollTable.rxMgmt[subChanNum].freeRingHandle = freeRingHandle;
                    emac_mcb.port_cb[portNum].pollTable.rxMgmt[subChanNum].compRingHandle= completionRingHandle;
                    emac_mcb.port_cb[portNum].rxMgmtCh.subChan[subChanNum].freeRingMem[0] = pChCfg->subChan[subChanNum].freeRingMem[0];
                    emac_mcb.port_cb[portNum].rxMgmtCh.subChan[subChanNum].compRingMem = pChCfg->subChan[subChanNum].compRingMem;
                }

                UdmaRingPrms_init(&ringPrms);
                ringPrms.elemCnt =  pChCfg->subChan[subChanNum].elementCountFree[0];
                ringPrms.elemSize = UDMA_RING_ES_8BYTES;
                ringPrms.ringMem = pChCfg->subChan[subChanNum].freeRingMem[0];
                /* need to allocate rxfree/rxcompletion ring pair */
                retVal = Udma_ringAlloc(emac_mcb.port_cb[portNum].udmaHandle,freeRingHandle,UDMA_RING_ANY, &ringPrms);
                if (retVal == 0)
                {
                    UdmaRingPrms_init(&ringPrms);
                    ringPrms.elemCnt =  pChCfg->subChan[subChanNum].elementCountCompletion;
                    ringPrms.elemSize = UDMA_RING_ES_8BYTES;
                    ringPrms.ringMem = pChCfg->subChan[subChanNum].compRingMem;
                    retVal = Udma_ringAlloc(emac_mcb.port_cb[portNum].udmaHandle,
                                            completionRingHandle,
                                            UDMA_RING_ANY,
                                            &ringPrms);
                    if (retVal == 0)
                    {
                        memset(&flowPrms, 0, sizeof(Udma_FlowPrms));
                        UdmaFlowPrms_init(&flowPrms, UDMA_CH_TYPE_RX);
                        flowPrms.psInfoPresent = 1U;
                        flowPrms.defaultRxCQ = completionRingHandle->ringNum;
                        flowPrms.fdq0Sz0Qnum    = freeRingHandle->ringNum;
                        flowPrms.fdq0Sz1Qnum    = freeRingHandle->ringNum;
                        flowPrms.fdq0Sz2Qnum    = freeRingHandle->ringNum;
                        flowPrms.fdq0Sz3Qnum    = freeRingHandle->ringNum;
                        flowPrms.fdq1Qnum       = freeRingHandle->ringNum;
                        flowPrms.fdq2Qnum       = freeRingHandle->ringNum;
                        flowPrms.fdq3Qnum       = freeRingHandle->ringNum;
                        retVal = Udma_flowConfig(pRxChannel->flowHandle,(subChanNum  -1), &flowPrms);
                        if (retVal == EMAC_DRV_RESULT_OK)
                        {
                            emac_cppi5InitHostDescQueue(portNum,
                                        0,
                                        emac_mcb.port_cb[portNum].num_of_rx_pkt_desc,
                                        EMAC_CPPI_DESC_SIZE,
                                        pChCfg->subChan[subChanNum].hPdMem[0],
                                        freeRingHandle->ringNum,
                                        freeRingHandle,
                                        emac_rx_free_push);
                        }
                        else
                        {
                            UTILS_trace(UTIL_TRACE_LEVEL_ERR,emac_mcb.drv_trace_cb, "port: %d: UDMA flow config error",portNum);
                            retVal = EMAC_DRV_RESULT_ERR_UDMA_FLOW_SETUP;
                            break;
                        }
                        /* setup the flow */
                    }
                    else
                    {
                        UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb,  "port: %d: UDMA ring alloc error",portNum);
                        retVal = EMAC_DRV_RESULT_ERR_UDMA_FLOW_SETUP;
                        break;
                    }
                    /* now setup the flow */
                }
                else
                {
                    UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb,  "port: %d: UDMA ring alloc error",portNum);
                    retVal = EMAC_DRV_RESULT_ERR_UDMA_FLOW_SETUP;
                    break;
                }
            }
        }
        else
        {
            UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb,  "port: %d: UDMA flow alloc error",portNum);
            retVal = EMAC_DRV_RESULT_ERR_UDMA_FLOW_SETUP;
        }
    }
    return retVal;
}



/*
 *  ======== emac_setup_udma_channel_rx ========
 */
int32_t
emac_setup_udma_channel_rx(uint32_t portNum,EMAC_PER_CHANNEL_CFG_RX* pChCfg, uint32_t channelType, EMAC_CHANNEL_RX_T* pRxChannel)
{
    int32_t retVal = EMAC_DRV_RESULT_OK;
    Udma_ChPrms chPrms;
    Udma_ChRxPrms rxPrms;
    Udma_FlowHandle flowHandle;
    Udma_FlowPrms flowParams;
    Udma_ChHandle chHandle;
    uint32_t regVal;

    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);
    chHandle = (Udma_ChHandle)(pChCfg->chHandle);

    if (pChCfg->nsubChan > 0)
    {
        retVal = emac_setup_additional_flows(portNum, pChCfg, channelType, pRxChannel);
        if (retVal == EMAC_DRV_RESULT_OK)
        {
            /* RX channel parameters */
            UdmaChPrms_init(&chPrms, UDMA_CH_TYPE_RX);
            chPrms.peerChNum            = pChCfg->threadId;
            /* subChan[0] is default flow */
            chPrms.fqRingPrms.ringMem   = pChCfg->subChan[0].freeRingMem[0];
            chPrms.fqRingPrms.elemCnt   = pChCfg->subChan[0].elementCountFree[0];
            chPrms.fqRingPrms.mode =  CSL_RINGACC_RING_MODE_MESSAGE;
            chPrms.cqRingPrms.ringMem   = pChCfg->subChan[0].compRingMem;
            chPrms.cqRingPrms.elemCnt   = pChCfg->subChan[0].elementCountCompletion;

            /* Open RX channel for receive with default flow */
            retVal = Udma_chOpen(emac_mcb.port_cb[portNum].udmaHandle, chHandle, UDMA_CH_TYPE_RX, &chPrms);
            if(UDMA_SOK == retVal)
            {
                UdmaChRxPrms_init(&rxPrms, UDMA_CH_TYPE_RX);
                rxPrms.dmaPriority = UDMA_DEFAULT_RX_CH_DMA_PRIORITY; 
                
                /* only need to update channel config with additional flow if more than 1 sub channel/flow */
                if (pChCfg->nsubChan > 1)
                {
                    rxPrms.flowIdFwRangeStart = Udma_flowGetNum(pRxChannel->flowHandle);
                    rxPrms.flowIdFwRangeCnt = pChCfg->nsubChan -1;
                    rxPrms.configDefaultFlow = FALSE;
                }
                retVal = Udma_chConfigRx(chHandle, &rxPrms);
                if(UDMA_SOK == retVal)
                {
                    /* need to enable RX timestamping, psInfoPresent flag set to 1 */
                    flowHandle = Udma_chGetDefaultFlowHandle(chHandle);
                    if(NULL == flowHandle)
                    {
                        UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb,  "port: %d: UDMA default flow handle is NULL error",portNum);
                        retVal = EMAC_DRV_RESULT_ERR_UDMA_RX_CHAN_SETUP;
                    }
                    else
                    {
                        memset(&flowParams, 0, sizeof(Udma_FlowPrms));
                        UdmaFlowPrms_init(&flowParams, UDMA_CH_TYPE_RX);
                        flowParams.rxChHandle = chHandle;
                        flowParams.psInfoPresent = 1U;
                        flowParams.defaultRxCQ = chHandle->cqRing->ringNum;
                        flowParams.fdq0Sz0Qnum    = chHandle->fqRing->ringNum;
                        flowParams.fdq0Sz1Qnum    = chHandle->fqRing->ringNum;
                        flowParams.fdq0Sz2Qnum    = chHandle->fqRing->ringNum;
                        flowParams.fdq0Sz3Qnum    = chHandle->fqRing->ringNum;
                        flowParams.fdq1Qnum       = chHandle->fqRing->ringNum;
                        flowParams.fdq2Qnum       = chHandle->fqRing->ringNum;
                        flowParams.fdq3Qnum       = chHandle->fqRing->ringNum;

                        retVal = Udma_flowConfig(flowHandle, 0, &flowParams);
                        if (UDMA_SOK != retVal)
                        {
                            UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d: UDMA RX flow config error",portNum);
                            retVal = EMAC_DRV_RESULT_ERR_UDMA_RX_CHAN_SETUP;
                        }
                    }
                }
                else
                {
                    UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d: UDMA RX channel config error",portNum);
                    retVal = EMAC_DRV_RESULT_ERR_UDMA_RX_CHAN_SETUP;
                }
            }
            else
            {
                UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d: UDMA RX channel open error",portNum);
                retVal = EMAC_DRV_RESULT_ERR_UDMA_RX_CHAN_SETUP;
            }
        
            if(retVal == UDMA_SOK)
            {
                if (emac_mcb.port_cb[portNum].mode_of_operation == EMAC_MODE_INTERRUPT)
                {
                    retVal = emac_setup_udma_channel_interrupt(portNum, pChCfg,channelType , pRxChannel);
                }

                if ((retVal == EMAC_DRV_RESULT_OK) && (portNum == EMAC_CPSW_PORT1))
                {
                    regVal = 0U;
                    regVal = 1U << CSL_ALE_THREADMAPDEF_DEFTHREAD_EN_SHIFT;
                    regVal = regVal | (chHandle->defaultFlowObj.flowStart << CSL_ALE_THREADMAPDEF_DEFTHREADVAL_SHIFT);
                    /* Setup Thread Map Register for flow configuration*/
                    CSL_REG32_WR((CSL_MCU_CPSW0_NUSS_BASE + EMAC_ALE_THREAD_DEF_REG), regVal);
                }
                if (retVal == EMAC_DRV_RESULT_OK)
                {
                    emac_cppi5InitHostDescQueue(portNum,
                                            0,
                                            emac_mcb.port_cb[portNum].num_of_rx_pkt_desc,
                                            EMAC_CPPI_DESC_SIZE,
                                            pChCfg->subChan[0].hPdMem[0],
                                            chHandle->fqRing->ringNum,
                                            chHandle->fqRing,
                                            emac_rx_free_push);
                    if ((emac_rx_free_push_errors & (1u << portNum)) != 0)
                    {
                        retVal = EMAC_DRV_RESULT_NO_MEM_AVAIL;
                    }
                }
            }
            if (retVal == EMAC_DRV_RESULT_OK)
            {
                retVal = Udma_chEnable(chHandle);
                /* update the free and completion ring handle for default flow in mcb */
                if (channelType == EMAC__RX_PKT_CHAN)
                {
                    emac_mcb.port_cb[portNum].rxPktCh.subChan[0].freeRingMem[0] = pChCfg->subChan[0].freeRingMem[0];
                    emac_mcb.port_cb[portNum].rxPktCh.subChan[0].compRingMem = pChCfg->subChan[0].compRingMem;
                    emac_mcb.port_cb[portNum].pollTable.rxPkt[0].compRingHandle = Udma_chGetCqRingHandle(emac_mcb.port_cb[portNum].rxPktCh.rxChHandle);
                    emac_mcb.port_cb[portNum].pollTable.rxPkt[0].freeRingHandle = Udma_chGetFqRingHandle(emac_mcb.port_cb[portNum].rxPktCh.rxChHandle);
                    emac_mcb.port_cb[portNum].pollTable.rxPkt[0].ringPollFxn =emac_poll_rx_pkts;
                }
                else if (channelType == EMAC_RX_MGMT_CHAN)
                {
                    emac_mcb.port_cb[portNum].rxMgmtCh.subChan[0].freeRingMem[0] = pChCfg->subChan[0].freeRingMem[0];
                    emac_mcb.port_cb[portNum].rxMgmtCh.subChan[0].compRingMem = pChCfg->subChan[0].compRingMem;
                    emac_mcb.port_cb[portNum].pollTable.rxMgmt[0].compRingHandle = Udma_chGetCqRingHandle(emac_mcb.port_cb[portNum].rxMgmtCh.rxChHandle);
                    emac_mcb.port_cb[portNum].pollTable.rxMgmt[0].freeRingHandle = Udma_chGetFqRingHandle(emac_mcb.port_cb[portNum].rxMgmtCh.rxChHandle);
                    emac_mcb.port_cb[portNum].pollTable.rxMgmt[EMAC_RX_RING_TX_TS_RESPONSE].ringPollFxn = emac_poll_tx_ts_resp;
                    emac_mcb.port_cb[portNum].pollTable.rxMgmt[EMAC_RX_RING_MGMT_PSI_RESPONSE].ringPollFxn = emac_poll_mgmt_pkts;
                }
                else 
                {
                    if (emac_mcb.port_cb[portNum].pgVersion  == EMAC_AM65XX_PG1_0_VERSION)
                    {
                        emac_mcb.port_cb[portNum].rxMgmtCh2.subChan[0].freeRingMem[0] = pChCfg->subChan[0].freeRingMem[0];
                        emac_mcb.port_cb[portNum].rxMgmtCh2.subChan[0].compRingMem = pChCfg->subChan[0].compRingMem;
                        emac_mcb.port_cb[portNum].pollTable.rxMgmt[EMAC_RX_RING_TX_TS_RESPONSE].compRingHandle = Udma_chGetCqRingHandle(emac_mcb.port_cb[portNum].rxMgmtCh2.rxChHandle);
                        emac_mcb.port_cb[portNum].pollTable.rxMgmt[EMAC_RX_RING_TX_TS_RESPONSE].freeRingHandle = Udma_chGetFqRingHandle(emac_mcb.port_cb[portNum].rxMgmtCh2.rxChHandle);
                        emac_mcb.port_cb[portNum].pollTable.rxMgmt[EMAC_RX_RING_TX_TS_RESPONSE].ringPollFxn = emac_poll_tx_ts_resp;
                        emac_mcb.port_cb[portNum].pollTable.rxMgmt[EMAC_RX_RING_MGMT_PSI_RESPONSE].ringPollFxn = emac_poll_mgmt_pkts;
                    }
                }
            }
        }
    }
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d", portNum, retVal);
    return retVal;
}


/*
 *  ======== emac_setup_rx_subsystem ========
 */
EMAC_DRV_ERR_E
emac_setup_rx_subsystem (uint32_t portNum,
                                  EMAC_OPEN_CONFIG_INFO_T *p_config,
                                  EMAC_HwAttrs_V5         *pHwAttrs)
{
    int32_t retVal = EMAC_DRV_RESULT_OK;
    Udma_ChHandle rxChHandle;
    Udma_ChHandle rxMgmtChHandle;
    Udma_ChHandle rxMgmt2ChHandle;
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    rxChHandle = (Udma_ChHandle)(pHwAttrs->portCfg[portNum].rxChannel.chHandle);
    emac_mcb.port_cb[portNum].rxPktCh.rxChHandle= rxChHandle;
    emac_mcb.port_cb[portNum].rxPktCh.nsubChan = pHwAttrs->portCfg[portNum].rxChannel.nsubChan;
    if (portNum == EMAC_CPSW_PORT1)
    {
        /* Initialize CPSW base address */
        emac_mcb.port_cb[portNum].hCpswRegs = (CSL_Xge_cpswRegs *)
            (pHwAttrs->portCfg[portNum].cpswSSRegsBaseAddr + CPSW_NU_OFFSET);
        emac_mcb.port_cb[portNum].hCpswAleRegs = (CSL_AleRegs *)
            (pHwAttrs->portCfg[portNum].cpswSSRegsBaseAddr + CPSW_ALE_OFFSET);
    }
    /* Setup UDMA RX channel for data packets */
    retVal = emac_setup_udma_channel_rx(portNum,
                            &(pHwAttrs->portCfg[portNum].rxChannel),
                            EMAC__RX_PKT_CHAN,
                            &emac_mcb.port_cb[portNum].rxPktCh);

    if((retVal == EMAC_DRV_RESULT_OK) && (portNum != EMAC_CPSW_PORT1))
    {
        /* Setup UDMA RX channel for rx config responses over PSI for ICSSG only*/
        rxMgmtChHandle = (Udma_ChHandle)(pHwAttrs->portCfg[portNum].rxChannelCfgOverPSI.chHandle);
        emac_mcb.port_cb[portNum].rxMgmtCh.nsubChan = pHwAttrs->portCfg[portNum].rxChannelCfgOverPSI.nsubChan;
        emac_mcb.port_cb[portNum].rxMgmtCh.rxChHandle= rxMgmtChHandle;
        retVal = emac_setup_udma_channel_rx(portNum,
                                 &(pHwAttrs->portCfg[portNum].rxChannelCfgOverPSI),
                                 EMAC_RX_MGMT_CHAN,
                                 &emac_mcb.port_cb[portNum].rxMgmtCh);

        if (emac_mcb.port_cb[portNum].pgVersion  == EMAC_AM65XX_PG1_0_VERSION)
        {
            if((retVal == EMAC_DRV_RESULT_OK) && (pHwAttrs->portCfg[portNum].rxChannel2CfgOverPSI.chHandle))
            {
                /* Setup UDMA RX channel for rx config responses over PSI for ICSSG only*/
                rxMgmt2ChHandle = (Udma_ChHandle)(pHwAttrs->portCfg[portNum].rxChannel2CfgOverPSI.chHandle);
                emac_mcb.port_cb[portNum].rxMgmtCh2.nsubChan = pHwAttrs->portCfg[portNum].rxChannel2CfgOverPSI.nsubChan;
                emac_mcb.port_cb[portNum].rxMgmtCh2.rxChHandle= rxMgmt2ChHandle;
                retVal = emac_setup_udma_channel_rx(portNum,
                                     &(pHwAttrs->portCfg[portNum].rxChannel2CfgOverPSI),
                                     EMAC_RX_MGMT2_CHAN,
                                     &emac_mcb.port_cb[portNum].rxMgmtCh2);
            }
        }
    }
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",
                                            portNum, retVal);
    return (EMAC_DRV_ERR_E)retVal;
} /* emac_setup_rx_subsystem */

/*
 *  ======== emac_close_rx_subsystem ========
 */

int32_t
emac_close_rx_subsystem (uint32_t portNum)
{
    uint32_t subChanNum ;
    Udma_RingHandle ringHandle = NULL;

    uint32_t numSubChan;

    subChanNum = 0;
    if (emac_mcb.port_cb[portNum].mode_of_operation == EMAC_MODE_INTERRUPT)
    {
        EMAC_osalDeleteBlockingLock(EMAC_GLOBAL_RX_SEM_HANDLE(portNum, subChanNum));
        Udma_eventUnRegister((Udma_EventHandle)(emac_mcb.port_cb[portNum].rxPktCh.subChan[subChanNum].eventHandle));
        if (portNum != EMAC_CPSW_PORT1)
        {
            EMAC_osalDeleteBlockingLock(EMAC_GLOBAL_RX_MGMT_SEM_HANDLE(portNum, subChanNum));
            Udma_eventUnRegister((Udma_EventHandle)(emac_mcb.port_cb[portNum].rxMgmtCh.subChan[subChanNum].eventHandle));
        }
    }

    /* free up resources for rx packet channel and sub-channels */
    if (emac_mcb.port_cb[portNum].rxPktCh.rxChHandle)
    {
        Udma_chClose(emac_mcb.port_cb[portNum].rxPktCh.rxChHandle);
        emac_cleanup_comp_ring(portNum, emac_mcb.port_cb[portNum].rxPktCh.rxChHandle->cqRing);
        emac_cleanup_free_ring(portNum, (uint64_t *)(emac_mcb.port_cb[portNum].rxPktCh.subChan[0].freeRingMem[0]));
        numSubChan = emac_mcb.port_cb[portNum].rxPktCh.nsubChan;
        if (numSubChan > 1)
        {
            Udma_flowFree(emac_mcb.port_cb[portNum].rxPktCh.flowHandle);
            for(subChanNum = 1; subChanNum < numSubChan;subChanNum++)
            {
                ringHandle = emac_mcb.port_cb[portNum].pollTable.rxPkt[subChanNum].compRingHandle;
                emac_cleanup_comp_ring(portNum, ringHandle);
                Udma_ringFree(ringHandle);

                ringHandle = emac_mcb.port_cb[portNum].pollTable.rxPkt[subChanNum].freeRingHandle;
                emac_cleanup_free_ring(portNum, (uint64_t *)(emac_mcb.port_cb[portNum].rxPktCh.subChan[subChanNum].freeRingMem[0]));
                Udma_ringFree(ringHandle);
            }
        }
    }

    /* free up resources for rx mgmt channel2, this has no sub-channels*/
    if ((emac_mcb.port_cb[portNum].pgVersion == EMAC_AM65XX_PG1_0_VERSION) && (emac_mcb.port_cb[portNum].rxMgmtCh2.rxChHandle))
    {
        Udma_chClose(emac_mcb.port_cb[portNum].rxMgmtCh2.rxChHandle);
        emac_cleanup_free_ring(portNum, (uint64_t *)(emac_mcb.port_cb[portNum].rxMgmtCh2.subChan[0].freeRingMem[0]));
    }

    /* free up resources for rx mgmt channel and sub-channels */
    if (emac_mcb.port_cb[portNum].rxMgmtCh.rxChHandle)
    {
        Udma_chClose(emac_mcb.port_cb[portNum].rxMgmtCh.rxChHandle);
        emac_cleanup_comp_ring(portNum, emac_mcb.port_cb[portNum].rxMgmtCh.rxChHandle->cqRing);
        emac_cleanup_free_ring(portNum, (uint64_t *)(emac_mcb.port_cb[portNum].rxMgmtCh.subChan[0].freeRingMem[0]));
        numSubChan = emac_mcb.port_cb[portNum].rxMgmtCh.nsubChan;
        if (numSubChan > 1)
        {
            Udma_flowFree(emac_mcb.port_cb[portNum].rxMgmtCh.flowHandle);
            for(subChanNum = 1;subChanNum < numSubChan;subChanNum++)
            {
                ringHandle = emac_mcb.port_cb[portNum].pollTable.rxMgmt[subChanNum].compRingHandle;
                emac_cleanup_comp_ring(portNum, ringHandle);
                Udma_ringFree(ringHandle);
    
                ringHandle = emac_mcb.port_cb[portNum].pollTable.rxMgmt[subChanNum].freeRingHandle;
                emac_cleanup_free_ring(portNum, (uint64_t *)(emac_mcb.port_cb[portNum].rxMgmtCh.subChan[subChanNum].freeRingMem[0]));
                Udma_ringFree(ringHandle);
            }
        }
    }

    return 0;
}

/*
 *  ======== emac_close_v5_common ========
 */
EMAC_DRV_ERR_E emac_close_v5_common( uint32_t portNum)
{
    uint32_t ringNum = 0;
    emac_mcb.port_cb[portNum].emacState = EMAC_PORT_STATE_CLOSED;
    if (emac_mcb.port_cb[portNum].mode_of_operation == EMAC_MODE_INTERRUPT)
    {
        if (EMAC_GLOBAL_RX_SEM_HANDLE(portNum, ringNum) != NULL)
        {
            EMAC_osalPostLock(EMAC_GLOBAL_RX_SEM_HANDLE(portNum, ringNum));
        }
        if (EMAC_GLOBAL_RX_MGMT_SEM_HANDLE(portNum, ringNum) != NULL)
        {
            EMAC_osalPostLock(EMAC_GLOBAL_RX_MGMT_SEM_HANDLE(portNum, ringNum));
        }
    }
    emac_close_tx_subsystem(portNum);
    emac_close_rx_subsystem(portNum);
    return EMAC_DRV_RESULT_OK;
}

/*
 *  ======== emac_poll_tx_ts_resp ========
 */
void emac_poll_tx_ts_resp(uint32_t portNum, uint32_t ringNum)
{

    if (emac_mcb.port_cb[portNum].pgVersion == EMAC_AM65XX_PG1_0_VERSION)
    {
        Udma_RingHandle compRingHandle =emac_mcb.port_cb[portNum].pollTable.rxMgmt[ringNum].compRingHandle;
        Udma_RingHandle freeRingHandle =emac_mcb.port_cb[portNum].pollTable.rxMgmt[ringNum].freeRingHandle;
        EMAC_CPPI_DESC_T *pCppiDesc = NULL;
        EMAC_TX_TS_RESPONSE *tx_ts;
        uint64_t tx_timestamp;
        bool is_valid;
        do
        {
            if ((emac_udma_ring_dequeue(compRingHandle, &pCppiDesc)) == 0)
            {
               if(pCppiDesc != NULL)
               {
                   tx_ts = (EMAC_TX_TS_RESPONSE*)(uintptr_t)pCppiDesc->hostDesc.bufPtr;
                   tx_timestamp = tx_ts->hi_txts;
                   tx_timestamp = (tx_timestamp << 32) | tx_ts->lo_txts ;
                   is_valid = (bool) tx_ts->ts_valid;
                   if(emac_mcb.port_cb[portNum].tx_ts_cb != NULL)
                   {
                       emac_mcb.port_cb[portNum].tx_ts_cb(portNum, tx_ts->ts_id, tx_timestamp, is_valid);
                   }
                   emac_udma_ring_enqueue(freeRingHandle,pCppiDesc, pCppiDesc->hostDesc.orgBufLen);
               }
            }
        } while (pCppiDesc != 0);
    }
    else
    {
        uint32_t *pMgmtPkt;
        uint64_t tx_timestamp;
        EMAC_PORT_INFO_T * pPortInfo = &emac_port_info[portNum];
        int32_t portLoc = pPortInfo->pollPortNum;
        int32_t icssgInst = pPortInfo->icssgInst;

        int32_t hwQLevel = emac_hwq_level(icssgInst, ((portLoc & 1) == 0) ? EMAC_ICSSG_TXTS_RX_HWQA_PORT0 : EMAC_ICSSG_TXTS_RX_HWQA_PORT1);

        while (hwQLevel != 0)
        {
            pMgmtPkt =(uint32_t*)(emac_hwq_pop(icssgInst, ((portLoc & 1) == 0) ? EMAC_ICSSG_TXTS_RX_HWQA_PORT0 : EMAC_ICSSG_TXTS_RX_HWQA_PORT1));
            if (pMgmtPkt != NULL)
            {
                tx_timestamp = pMgmtPkt[4];
                tx_timestamp = tx_timestamp << 32U | pMgmtPkt[3];
                if(emac_mcb.port_cb[portLoc].tx_ts_cb != NULL)
                {
                    emac_mcb.port_cb[portLoc].tx_ts_cb(portLoc, pMgmtPkt[2], tx_timestamp, 1);
                }
                emac_hwq_push(icssgInst, ((pMgmtPkt[0] & (1 << 23)) == 0)  ? EMAC_ICSSG_TXTS_FREE_HWQA_PORT0 : EMAC_ICSSG_TXTS_FREE_HWQA_PORT1, pMgmtPkt);
                hwQLevel = emac_hwq_level(icssgInst, ((portLoc & 1) == 0) ? EMAC_ICSSG_TXTS_RX_HWQA_PORT0 : EMAC_ICSSG_TXTS_RX_HWQA_PORT1);
           }
       }
    }
}

/*
 *  ======== emac_poll_tx_ring ========
 */
EMAC_DRV_ERR_E emac_poll_tx_ring(uint32_t portNum, Udma_RingHandle compRingHandle, uint32_t ringNum)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_SEND_ERR;
    EMAC_CPPI_DESC_T *pCppiDesc = NULL;
    EMAC_PKT_DESC_T  *pPktDesc;
    uintptr_t key;
    uint32_t temp=0;
    do
    {
        if ((emac_udma_ring_dequeue(compRingHandle, &pCppiDesc)) ==0)
        {
            retVal = EMAC_DRV_RESULT_OK;
            if (pCppiDesc != NULL)
            {
                pPktDesc = pCppiDesc->appPtr;
                if ((pPktDesc != NULL) && (!(pCppiDesc->hostDesc.pktInfo2 & EMAC_FW_MGMT_PKT)))
                {
                   /* EMAC FREE PKT needs to be invoked on Port/Slice used to transmit the packet
                      as packet will be scheduled by ICSSG firmware irrespective of the port in which
                      application is polling for TX completion */ 
                   
                    temp = pCppiDesc->hostDesc.srcDstTag & (uint32_t)0x3 ;

                    if(temp == 1) 
                    {
                         temp = (portNum & 0x6); /*Switch Port0 */
                    }
                    else if (temp == 2)
                    {
                         temp = (portNum & 0x6) + 1; /*Switch Port1 */
                    }
              
                    emac_mcb.port_cb[portNum].free_pkt_cb(temp, pCppiDesc->appPtr);
                }
                key = EMAC_osalHardwareIntDisable();
                pCppiDesc->nextPtr = emac_mcb.port_cb[portNum].txReadyDescs[ringNum];
                emac_mcb.port_cb[portNum].txReadyDescs[ringNum] = pCppiDesc;
                EMAC_osalHardwareIntRestore(key);
            }
        }
    } while (pCppiDesc != NULL);
    return retVal;
}

/*
 *  ======== emac_poll_tx_complete ========
 */
void emac_poll_tx_complete(uint32_t portNum, uint32_t ringNum)
{
    Udma_RingHandle compRingHandle = emac_mcb.port_cb[portNum].pollTable.txCompletion[ringNum].compRingHandle;
    emac_poll_tx_ring(portNum, compRingHandle, ringNum);
}

/*
 *  ======== emac_poll_rx_pkts ========
 */
void emac_poll_rx_pkts(uint32_t portNum, uint32_t ringNum)
{
    EMAC_CPPI_DESC_T *pCppiDesc =NULL;
    EMAC_PKT_DESC_T  *pPktDesc;
    uint32_t          pktSize;
    bool              dropPacket;
    Udma_RingHandle compRingHandle =emac_mcb.port_cb[portNum].pollTable.rxPkt[ringNum].compRingHandle;
    Udma_RingHandle freeRingHandle =emac_mcb.port_cb[portNum].pollTable.rxPkt[ringNum].freeRingHandle;

    do
    {
        if ((emac_udma_ring_dequeue(compRingHandle, &pCppiDesc)) == 0)
        {
            if(pCppiDesc != NULL)
            {
                dropPacket = FALSE;
                if (emac_pkt_too_big(portNum, pCppiDesc, &pktSize))
                {
                    dropPacket = true;
                }
                else
                {
                    /* Update the software desciptor being passed to application */
                    pPktDesc            = pCppiDesc->appPtr;
                    pPktDesc->ValidLen = pktSize;
                    pPktDesc->PktLength = pktSize;
                    pPktDesc->RxTimeStamp =  ((uint64_t)pCppiDesc->psinfo[1] << 32U) | pCppiDesc->psinfo[0] ;
                    pPktDesc->PktChannel = ringNum;
                    /* Allocate the PKT to replenish the RX_FREE_QUEUE queue */
                    if ((pPktDesc = EMAC_ALLOC_PKT(portNum, EMAC_PKT_SIZE(portNum))) != NULL)
                    {
                        /* Call the callback */
                        emac_mcb.port_cb[portNum].rx_pkt_cb(portNum, pCppiDesc->appPtr);
                        /* Populate the Rx free descriptor with the buffer we just allocated. */
                        pCppiDesc->appPtr             = pPktDesc;
                        pCppiDesc->hostDesc.bufPtr    = (uint64_t)pPktDesc->pDataBuffer;
                        pCppiDesc->hostDesc.orgBufPtr = (uint64_t)pPktDesc->pDataBuffer;
                        pCppiDesc->hostDesc.orgBufLen = pPktDesc->BufferLen;
                        pCppiDesc->hostDesc.bufInfo1  = pPktDesc->BufferLen;
                        emac_udma_ring_enqueue(freeRingHandle,pCppiDesc, pCppiDesc->hostDesc.orgBufLen);
                    }
                    else
                    {
                        /* Drop this packet since we couldn't get new buffer */
                        UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d: RX pkt allocation error during replinish of RX free queue, packet dropped",
                                                portNum);
                        dropPacket = true;
                    }
                }
                if (dropPacket == true) 
                {
                    /* It was either too big, or no replacement buffer so avoid leaking descriptors */
                    CSL_FINS (pCppiDesc->hostDesc.descInfo, UDMAP_CPPI5_PD_DESCINFO_PKTLEN,
                                          pCppiDesc->hostDesc.orgBufLen);
                    emac_udma_ring_enqueue(freeRingHandle, pCppiDesc, pCppiDesc->hostDesc.orgBufLen);
                }
            }
        }
    } while (pCppiDesc != 0);
}

/*
 *  ======== emac_poll_mgmt_pkts ========
 */
void emac_poll_mgmt_pkts(uint32_t portNum, uint32_t ringNum)
{
    EMAC_IOCTL_CMD_RESP_T cmdResponse;
    EMAC_IOCTL_CMD_T *pIoctlData;
    EMAC_PORT_INFO_T * pPortInfo = &emac_port_info[portNum];
    int32_t icssgInst = pPortInfo->icssgInst;
    int32_t portLoc = pPortInfo->pollPortNum;
    uint32_t *pMgmtPkt = NULL;

    if (emac_mcb.port_cb[portLoc].ioctlType == EMAC_ICSSG_IOCTL_TYPE_R30_OVER_DMEM)
    {
        if (emac_R30_is_done(portLoc) == 1)
        {
            emac_mcb.port_cb[portLoc].ioctlType = 0;
            emac_mcb.ioctl_cb.ioctlInProgress = false;
            if (emac_mcb.port_cb[portLoc].rx_mgmt_response_cb != NULL)
            {
                cmdResponse.seqNumber = emac_mcb.ioctl_cb.sequenceNumber;
                cmdResponse.status = 1;
                cmdResponse.respParamsLength = 0;
                emac_mcb.port_cb[portLoc].rx_mgmt_response_cb(portLoc, &cmdResponse);
            }
        }
    }
    else
    {
        int32_t hwQLevel = emac_hwq_level(icssgInst, ((portLoc & 1) == 0) ? EMAC_ICSSG_MGMT_RX_HWQA_PORT0 : EMAC_ICSSG_MGMT_RX_HWQA_PORT1);
        while (hwQLevel != 0)
        {
            pMgmtPkt = (uint32_t*)(emac_hwq_pop(icssgInst, ((portLoc & 1) == 0) ? EMAC_ICSSG_MGMT_RX_HWQA_PORT0: EMAC_ICSSG_MGMT_RX_HWQA_PORT1));
            if (pMgmtPkt != NULL)
            {
                pIoctlData = (EMAC_IOCTL_CMD_T*)&pMgmtPkt[1];
                cmdResponse.status = pIoctlData->commandParam;
                cmdResponse.seqNumber = pIoctlData->seqNumber;
                cmdResponse.respParamsLength = 0;
                /* for command response with status 0x3, its cmd response for ADD FDB entry.
                   if adding entry results in removing aged-out FDB entry, that is returned to caller
                   in response Params*/
                if (cmdResponse.status == 0x3)
                {
                    cmdResponse.respParamsLength = 2;
                    memcpy(&cmdResponse.respParams, pIoctlData->spare, 2);
                }
                if ((emac_mcb.port_cb[portLoc].rx_mgmt_response_cb != NULL) && (emac_mcb.ioctl_cb.internalIoctl == false))
                {
                    emac_mcb.port_cb[portLoc].rx_mgmt_response_cb(portLoc, &cmdResponse);
                }
                emac_mcb.ioctl_cb.internalIoctl = false;

                emac_hwq_push(icssgInst, ((portLoc & 1) == 0) ? EMAC_ICSSG_MGMT_FREE_HWQA_PORT0 : EMAC_ICSSG_MGMT_FREE_HWQA_PORT1, pMgmtPkt);
                hwQLevel = emac_hwq_level(icssgInst, ((portLoc & 1) == 0) ? EMAC_ICSSG_MGMT_RX_HWQA_PORT0 : EMAC_ICSSG_MGMT_RX_HWQA_PORT1);
                emac_mcb.ioctl_cb.ioctlInProgress = false;
            }
        }
    }
}

/*
 *  ======== EMAC_poll_ctrl_v5_dual_mac ========
 */
EMAC_DRV_ERR_E emac_poll_ctrl_v5_common(uint32_t portNum, uint32_t rxPktRings, uint32_t rxMgmtRings, uint32_t txRings)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
    int32_t i;
    uint32_t pollMask;
    /* Poll the rx completion queue for receive packet */
    if ((rxPktRings > 0) && (emac_mcb.port_cb[portNum].rxPktCh.nsubChan > 0))
    {
        pollMask = 1 << (emac_mcb.port_cb[portNum].rxPktCh.nsubChan - 1);
        for (i = emac_mcb.port_cb[portNum].rxPktCh.nsubChan-1; i >= 0;i--)
        {
            if ((rxPktRings & pollMask) && (emac_mcb.port_cb[portNum].pollTable.rxPkt[i].ringPollFxn))
            {
                emac_mcb.port_cb[portNum].pollTable.rxPkt[i].ringPollFxn(portNum, i);
            }
            pollMask = pollMask >> 1;
        }
    }

    /* Poll the rx management completion queue  management response or TX TS response */
    if ((rxMgmtRings > 0) && (emac_mcb.port_cb[portNum].rxMgmtCh.nsubChan > 0))
    {
        pollMask = 1 << (emac_mcb.port_cb[portNum].rxMgmtCh.nsubChan - 1);
        for (i = emac_mcb.port_cb[portNum].rxMgmtCh.nsubChan-1; i >= 0;i--)
        {
            if ((rxMgmtRings & pollMask) && (emac_mcb.port_cb[portNum].pollTable.rxMgmt[i].ringPollFxn))
            {
                emac_mcb.port_cb[portNum].pollTable.rxMgmt[i].ringPollFxn(portNum, i);
            }
            pollMask = pollMask >> 1;
        }
    }

    /* Poll the TX completion queue for tx completion events*/
    if ((txRings > 0) && (emac_mcb.port_cb[portNum].numTxChans > 0))
    {
        pollMask = 1 << (emac_mcb.port_cb[portNum].numTxChans - 1);
        for (i = emac_mcb.port_cb[portNum].numTxChans - 1; i >= 0;i--)
        {
            if ((txRings & pollMask) && (emac_mcb.port_cb[portNum].pollTable.txCompletion[i].ringPollFxn))
            {
                emac_mcb.port_cb[portNum].pollTable.txCompletion[i].ringPollFxn(portNum, i);
            }
            pollMask = pollMask >> 1;
        }
    }
    return retVal;
}

/*
 *  ======== emac_poll_pkt_v5_common ========
 */
EMAC_DRV_ERR_E emac_poll_pkt_v5_common(uint32_t portNum)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
    int32_t ringNum = 0;

    /* Poll the rx completion queue for receive packet */
    if (emac_mcb.port_cb[portNum].mode_of_operation == EMAC_MODE_POLL)
    {
        retVal = emac_poll_ctrl_v5_common(portNum, EMAC_POLL_RX_PKT_RING1, 0, 0);
    }
    /* wait for ISR to hit for receive packet */
    else
    {
        while(retVal == EMAC_DRV_RESULT_OK )
        {
            EMAC_osalPendLock(EMAC_GLOBAL_RX_SEM_HANDLE(portNum,ringNum), SemaphoreP_WAIT_FOREVER);
            if( emac_mcb.port_cb[portNum].emacState  == EMAC_PORT_STATE_CLOSED)
            {
                retVal =   EMAC_DRV_RESULT_GENERAL_ERR;
                break;
            }
            /* poll rx pkt on default flow */
            retVal = emac_poll_ctrl_v5_common(portNum, EMAC_POLL_RX_PKT_RING1, 0, 0);
        }
    }
   return retVal;
} /* EMAC_poll_pkt_v5_dual_mac */


/*
 *  ======== emac_open_v5_dual_mac ========
 */
EMAC_DRV_ERR_E  emac_open_v5_common(uint32_t portNum, EMAC_OPEN_CONFIG_INFO_T *p_config)
{
    EMAC_HwAttrs_V5       *pHwAttrs;
    uint32_t regId;
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OPEN_PORT_ERR;
    uint32_t size, rem;
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    pHwAttrs = (EMAC_HwAttrs_V5*)p_config->hwAttrs;
    /* Initialize EMAC MCB when opening a port */

    memset (&emac_mcb.port_cb[portNum], 0, sizeof(EMAC_PORT_CB_V5_T));
    emac_mcb.drv_trace_cb = p_config->drv_trace_cb;

    /* Input parameter validation */
    if ((p_config->p_chan_mac_addr == NULL)             ||
        (p_config->rx_pkt_cb == NULL)                   ||
        (p_config->alloc_pkt_cb == NULL)                ||
        (p_config->free_pkt_cb == NULL))
    {
        UTILS_trace(UTIL_TRACE_LEVEL_ERR,emac_mcb.drv_trace_cb,"port: %d: invalid params", portNum);
        retVal = EMAC_DRV_RESULT_OPEN_PORT_ERR;
    }
    else
    {
        if (pHwAttrs->portCfg[portNum].phyAddr > EMAC_MAX_NUM_PHY_ADDR)
        {
            UTILS_trace(UTIL_TRACE_LEVEL_ERR,emac_mcb.drv_trace_cb,"port: %d: invalid phy address: %d",
            portNum, pHwAttrs->portCfg[portNum].phyAddr);
            retVal =  EMAC_DRV_RESULT_ERR_INVALID_PHY_ADDR;
        }
        else
        {
            emac_mcb.port_cb[portNum].udmaHandle = (Udma_DrvHandle)p_config->udmaHandle;
            emac_mcb.port_cb[portNum].mode_of_operation = p_config->mode_of_operation;
            emac_mcb.port_cb[portNum].pgVersion = CSL_REG32_RD(CSL_WKUP_CTRL_MMR0_CFG0_BASE + CSL_WKUP_CTRL_MMR_CFG0_JTAGID);
            size = p_config->max_pkt_size;
            rem = size % 4;
            if (rem != 0)
            {
                size += 4-rem;
            }
            EMAC_PKT_SIZE(portNum) = size;
            memcpy(&emac_mcb.port_cb[portNum].macaddr_cfg.addr, p_config->p_chan_mac_addr->p_mac_addr, sizeof(EMAC_MAC_ADDR_T));
            EMAC_NUM_RX_PKTS(portNum)              = p_config->num_of_rx_pkt_desc;
            EMAC_NUM_TX_PKTS(portNum)              = p_config->num_of_tx_pkt_desc;
            EMAC_NUM_TXRX_PKTS(portNum)            = p_config->num_of_tx_pkt_desc + p_config->num_of_rx_pkt_desc;
            EMAC_CFG_LOOPBACK(portNum)             = p_config->loop_back;

            /* Intialize the call back function pointers in EMAC MCB */
            emac_mcb.port_cb[portNum].rx_pkt_cb      =     p_config->rx_pkt_cb;
            emac_mcb.port_cb[portNum].alloc_pkt_cb   =     p_config->alloc_pkt_cb;
            emac_mcb.port_cb[portNum].free_pkt_cb    =     p_config->free_pkt_cb;
            emac_mcb.port_cb[portNum].rx_mgmt_response_cb = p_config->rx_mgmt_response_cb;
            emac_mcb.port_cb[portNum].tx_ts_cb= p_config->tx_ts_cb;

            /* Setup Tx */
            if ((retVal = emac_setup_tx_subsystem (portNum, p_config, pHwAttrs)) != EMAC_DRV_RESULT_OK)
            {
                return retVal;
            }

            /* Setup Rx */
            if ((retVal = emac_setup_rx_subsystem (portNum, p_config, pHwAttrs)) != EMAC_DRV_RESULT_OK)
            {
                return retVal;
            }

            /* need to figure out the instNum, for icss-g, there are 2 ports using the same MDIO IP block, 
                port 0,2,4,6 are part of  USERGROUP0 and USER PHY0 and will use  index 0,
                port 1,3,5 are part of  USERGROUP1 and USER PHY1 and will use  index 1
            */
            if ((portNum % 2U) == 0)
            {
                regId = 0U;
            }
            else
            {
                regId = 1U;
            }
            emac_mcb.port_cb[portNum].MdioDev.mdioBaseAddr = (uint32_t)(pHwAttrs->portCfg[portNum].mdioRegsBaseAddr);
            emac_mcb.port_cb[portNum].mdio_flag = p_config->mdio_flag;
            EMAC_mdioOpen(portNum, regId, pHwAttrs->portCfg[portNum].phyAddr, &emac_mcb.port_cb[portNum].MdioDev);
            emac_mcb.port_cb[portNum].emacState =EMAC_PORT_STATE_OPEN;
             UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",
                                                    portNum, retVal);
        }
    }
 return retVal;
}

/*
 *  ======== emac_open_udma_channel_tx ========
 */
int32_t
emac_open_udma_channel_tx(uint32_t portNum,EMAC_PER_CHANNEL_CFG_TX*pChCfg)
{
    int32_t retVal = EMAC_DRV_RESULT_OK;
    Udma_ChPrms chPrms;
    Udma_ChTxPrms txPrms;

    /* TX channel parameters */
    UdmaChPrms_init(&chPrms, UDMA_CH_TYPE_TX);
    chPrms.peerChNum            = pChCfg->threadId;
    chPrms.fqRingPrms.ringMem   = pChCfg->freeRingMem;
    chPrms.fqRingPrms.elemCnt   = pChCfg->elementCount;

    chPrms.cqRingPrms.ringMem   = pChCfg->compRingMem;
    chPrms.cqRingPrms.elemCnt   = pChCfg->elementCount;

    /* Open TX channel for transmit */
    retVal = Udma_chOpen(emac_mcb.port_cb[portNum].udmaHandle, (Udma_ChHandle)(pChCfg->chHandle), UDMA_CH_TYPE_TX, &chPrms);

    if(UDMA_SOK == retVal)
    {
        UdmaChTxPrms_init(&txPrms, UDMA_CH_TYPE_TX);
        txPrms.filterPsWords = TISCI_MSG_VALUE_RM_UDMAP_TX_CH_FILT_PSWORDS_ENABLED;
        txPrms.dmaPriority = UDMA_DEFAULT_UTC_CH_DMA_PRIORITY;
        retVal = Udma_chConfigTx((Udma_ChHandle)(pChCfg->chHandle), &txPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable((Udma_ChHandle)(pChCfg->chHandle));
            if (UDMA_SOK == retVal)
            {
                retVal =  EMAC_DRV_RESULT_OK;
            }
        }
        else
        {
            UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d: UDMA TX channel config error",portNum);
            retVal = EMAC_DRV_RESULT_ERR_UDMA_TX_CHAN_SETUP;
        }
    }
    else
    {
        UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d: UDMA TX channel open error",portNum);
        retVal = EMAC_DRV_RESULT_ERR_UDMA_TX_CHAN_SETUP;
    }
    return retVal;
}




/*
 *  ======== emac_setup_udma_channel_interrupt ========
 */
int32_t
emac_setup_udma_channel_interrupt(uint32_t portNum, EMAC_PER_CHANNEL_CFG_RX*pChCfg, uint32_t eventType, EMAC_CHANNEL_RX_T* pRxChannel)
{
    uintptr_t intArg = 0;
    uintptr_t temp = 0;
    int32_t retVal = EMAC_DRV_RESULT_OK;
    Udma_EventHandle eventHandle;
    Udma_EventPrms eventPrms;
    SemaphoreP_Params semParams;
    uint32_t ringNum= 0;
    intArg =  ringNum;
    temp = portNum << 16;
    intArg = intArg | temp;
    /* Register ring completion callback */
    eventHandle = (Udma_EventHandle)(pChCfg->subChan[ringNum].eventHandle);

    UdmaEventPrms_init(&eventPrms);
    eventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
    eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
    eventPrms.chHandle          = (Udma_ChHandle)(pChCfg->chHandle);
    eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(emac_mcb.port_cb[portNum].udmaHandle);

    pRxChannel->subChan[0].eventHandle = eventHandle;

    if (eventType == EMAC__RX_PKT_CHAN)
    {
        eventPrms.eventCb = EMAC_rxIsrFxn;
    }
    else
    {
        eventPrms.eventCb = EMAC_rxMgmtIsrFxn;
    }

    eventPrms.appData           = (void *)intArg;
    retVal = Udma_eventRegister(emac_mcb.port_cb[portNum].udmaHandle, eventHandle, &eventPrms);
    if (retVal == UDMA_SOK)
    {
        EMAC_osalSemParamsInit(&semParams);
        semParams.mode = SemaphoreP_Mode_BINARY;
        semParams.name= (char*)("rxSemaphore");
        if (eventType == EMAC__RX_PKT_CHAN)
        {
            semParams.name= (char*)("rxSemaphore");
            EMAC_GLOBAL_RX_SEM_HANDLE(portNum, ringNum) =  EMAC_osalCreateBlockingLock(0,&semParams);
            if((EMAC_GLOBAL_RX_SEM_HANDLE(portNum, ringNum)) ==  NULL)
            {
                retVal =  EMAC_DRV_RESULT_OPEN_PORT_ERR;
            }
        }
        else
        {
            semParams.name= (char*)("rxMgmtSemaphore");
            EMAC_GLOBAL_RX_MGMT_SEM_HANDLE(portNum, ringNum) =  EMAC_osalCreateBlockingLock(0,&semParams);
            if((EMAC_GLOBAL_RX_MGMT_SEM_HANDLE(portNum, ringNum)) ==  NULL)
            {
                retVal =  EMAC_DRV_RESULT_OPEN_PORT_ERR;
            }
        }

    }
    return retVal;
}

/*
 *  ======== EMAC_send_v5_common ========
 */
EMAC_DRV_ERR_E EMAC_send_v5_common(uint32_t portNum, EMAC_PKT_DESC_T* pDesc, EMAC_CPPI_DESC_T* pCppiDesc)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
    Udma_ChHandle txChHandle;
    uint32_t chNum =pDesc->PktChannel;
    uintptr_t key;

    txChHandle = emac_mcb.port_cb[portNum].txChHandle[chNum];
    retVal = emac_poll_tx_ring(portNum,Udma_chGetCqRingHandle(txChHandle), chNum);
    if (retVal == EMAC_DRV_RESULT_OK)
    {
        if (pDesc->PktLength < EMAC_MIN_PKT_SIZE)
        {
            pDesc->PktLength = EMAC_MIN_PKT_SIZE;
        }

        uint64_t bufPtr = (uint64_t)pDesc->pDataBuffer + pDesc->DataOffset;
        pCppiDesc->hostDesc.bufPtr    = bufPtr;
        pCppiDesc->hostDesc.orgBufPtr = bufPtr;
        pCppiDesc->hostDesc.orgBufLen = pDesc->BufferLen;
        pCppiDesc->hostDesc.bufInfo1  = pDesc->PktLength;

        /* Clear packet type flag bits in case its set, default of 0 indicates TX packet type*/
        pCppiDesc->hostDesc.pktInfo2 &= 0x07FFFFFF;
        CSL_FINS (pCppiDesc->hostDesc.descInfo, UDMAP_CPPI5_PD_DESCINFO_PKTLEN, pDesc->PktLength);
        pCppiDesc->appPtr = pDesc;

        /*enqueue packet for transmission */
        if ((retVal = emac_udma_ring_enqueue (Udma_chGetFqRingHandle(txChHandle),pCppiDesc, pDesc->PktLength)) != EMAC_DRV_RESULT_OK)
        {
            key = EMAC_osalHardwareIntDisable();
            pCppiDesc->nextPtr = emac_mcb.port_cb[portNum].txReadyDescs[chNum];
            emac_mcb.port_cb[portNum].txReadyDescs[chNum] = pCppiDesc;
                EMAC_osalHardwareIntRestore(key);
            UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d, Udma ring enqueue error emac_send",portNum);
        }
    }
    return retVal;
}

/*
 *  ======== emac_config_icssg_dual_mac_fw_pg1 ========
 */
static void emac_config_icssg_dual_mac_fw_pg1(uint32_t portNum, EMAC_HwAttrs_V5 *hwAttrs)
{
    uint32_t bufferPoolNum;
    EMAC_PER_PORT_ICSSG_FW_CFG *pEmacFwCfg;
    EMAC_PRU_CFG_T pruCfg;
    Udma_FlowHandle flowHandle;
    Udma_ChHandle chHandle;
    EMAC_ICSSG_DUALMAC_FW_CFG *pDmFwCfg;

    memset(&pruCfg, 0, sizeof(EMAC_PRU_CFG_T));
    hwAttrs->portCfg[portNum].getFwCfg(portNum,&pEmacFwCfg);
    pDmFwCfg = (EMAC_ICSSG_DUALMAC_FW_CFG*)pEmacFwCfg->pFwPortCfg;
    pruCfg.addr_lo = pEmacFwCfg->fwAppCfg.txPortQueueLowAddr;
    pruCfg.addr_hi = pEmacFwCfg->fwAppCfg.txPortQueueHighAddr;

    /*  flow for rx packet. if only 1 subChan use default flow */
    if (emac_mcb.port_cb[portNum].rxPktCh.nsubChan > 1)
    {
        pruCfg.def_flow = Udma_flowGetNum(emac_mcb.port_cb[portNum].rxPktCh.flowHandle);
    }
    else
    {
        chHandle = emac_mcb.port_cb[portNum].rxPktCh.rxChHandle;
        flowHandle = Udma_chGetDefaultFlowHandle(chHandle);
        pruCfg.def_flow= Udma_flowGetNum( flowHandle);
    }
    /* Flow for rx config mgmt. 1 subChan use default flow */
    if (emac_mcb.port_cb[portNum].rxMgmtCh.nsubChan > 1)
    {
        pruCfg.mgr_flow = Udma_flowGetNum(emac_mcb.port_cb[portNum].rxMgmtCh.flowHandle);
    }
    else
    {
        chHandle = emac_mcb.port_cb[portNum].rxMgmtCh.rxChHandle;
        flowHandle = Udma_chGetDefaultFlowHandle(chHandle);
        pruCfg.mgr_flow= Udma_flowGetNum( flowHandle);
    }

    for (bufferPoolNum = 8U; bufferPoolNum < EMAC_NUM_TRANSMIT_FW_QUEUES*2U;bufferPoolNum++)
    {
        pruCfg.tx_bs[bufferPoolNum] = pDmFwCfg->txHostQueueSize[bufferPoolNum-8];
    }

    emac_hw_mem_write(hwAttrs->portCfg[portNum].icssSharedRamBaseAddr, &(pruCfg),(sizeof(EMAC_PRU_CFG_T)/4));

}

/*
 *  ======== emac_config_icssg_dual_mac_fw_pg2 ========
 */
static void emac_config_icssg_dual_mac_fw_pg2(uint32_t portNum, EMAC_HwAttrs_V5 *hwAttrs)
{
    EMAC_ICSSG_FW_CFG_PG2 icssgCfg;
    EMAC_PER_PORT_ICSSG_FW_CFG *pEmacFwCfg;
    uint32_t flowStart;
    Udma_FlowHandle flowHandle;
    uint32_t i;
    hwAttrs->portCfg[portNum].getFwCfg(portNum,&pEmacFwCfg);

    if (pEmacFwCfg->pIcssgFwCfgFxn != NULL)
    {
        icssgCfg.icssgBaseAddr = hwAttrs->portCfg[portNum].icssDram0BaseAddr;
        /* Flow for rx config mgmt. 1 subChan use default flow */
        if (emac_mcb.port_cb[portNum].rxMgmtCh.nsubChan > 1)
        {
            flowStart = Udma_flowGetNum(emac_mcb.port_cb[portNum].rxMgmtCh.flowHandle);
        }
        else
        {
            flowHandle = Udma_chGetDefaultFlowHandle(emac_mcb.port_cb[portNum].rxMgmtCh.rxChHandle);
            flowStart= Udma_flowGetNum( flowHandle);
        }
        icssgCfg.rxMgmtFlowStart = flowStart;

        /*  flow for rx packet. if only 1 subChan use default flow */
        if (emac_mcb.port_cb[portNum].rxPktCh.nsubChan > 1)
        {
            flowStart = Udma_flowGetNum(emac_mcb.port_cb[portNum].rxPktCh.flowHandle);
        }
        else
        {
            flowHandle = Udma_chGetDefaultFlowHandle(emac_mcb.port_cb[portNum].rxPktCh.rxChHandle);
            flowStart= Udma_flowGetNum( flowHandle);
        }
        icssgCfg.rxPktFlowStart = flowStart;

        icssgCfg.bufferPoolLowAddr = pEmacFwCfg->fwAppCfg.bufferPoolLowAddr;
        icssgCfg.bufferPoolHighAddr = pEmacFwCfg->fwAppCfg.bufferPoolHighAddr;
        icssgCfg.numBufferPool = pEmacFwCfg->fwAppCfg.numBufferPool;
        for (i = 0; i < pEmacFwCfg->fwAppCfg.numBufferPool; i++)
        {
            icssgCfg.bufferPoolSize[i] = pEmacFwCfg->fwAppCfg.bufferPoolSize[i];
        }
        pEmacFwCfg->pIcssgFwCfgFxn(portNum, &icssgCfg);
        emac_R30_cmd_init(portNum);
    }
}

/*
 *  ======== emac_config_icssg_fw ========
 */
void emac_config_icssg_fw(uint32_t portNum, EMAC_HwAttrs_V5 *hwAttrs)
{
    if (emac_mcb.port_cb[portNum].pgVersion == EMAC_AM65XX_PG1_0_VERSION)
    {
        emac_config_icssg_dual_mac_fw_pg1(portNum, hwAttrs);
    }
    else
    {
        emac_config_icssg_dual_mac_fw_pg2(portNum, hwAttrs);
    }
}

uint32_t txIpgPortMap [EMAC_MAX_ICSSG_INSTANCES*EMAC_MAC_PORTS_PER_ICSS][EMAC_MAC_PORTS_PER_ICSS] =
{
    {EMAC_ICSSG0_PORT0,EMAC_ICSSG0_PORT1},
    {EMAC_ICSSG0_PORT1,EMAC_ICSSG0_PORT0},
    {EMAC_ICSSG1_PORT0,EMAC_ICSSG1_PORT1},
    {EMAC_ICSSG1_PORT1,EMAC_ICSSG1_PORT0},
#ifdef SOC_AM65XX
    {EMAC_ICSSG2_PORT0,EMAC_ICSSG2_PORT1},
    {EMAC_ICSSG2_PORT1,EMAC_ICSSG2_PORT0}
#endif
};

/*
 *  ======== emac_get_icssg_cfg_base_addr ========
 */
uintptr_t emac_get_icssg_cfg_base_addr(uint32_t portNum)
{
    return (emac_mcb.port_cb[portNum].icssDram0BaseAddr);
}

/*
 *  ======== emac_get_icssg_rgmii_cfg_base_addr ========
 */
uintptr_t emac_get_icssg_rgmii_cfg_base_addr(uint32_t portNum)
{
    uintptr_t baseAddr = emac_get_icssg_cfg_base_addr(portNum);
    baseAddr += CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
               CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG;
    return baseAddr;
}

/*
 *  ======== emac_get_icssg_tx_ipg_cfg_base_addr ========
 */
uintptr_t emac_get_icssg_tx_ipg_cfg_base_addr(uint32_t portNum)
{
    uintptr_t baseAddr = emac_get_icssg_cfg_base_addr(portNum);
    if ((portNum & 1) == 0)
    {
        baseAddr += CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_TX_IPG0;
    }
    else
    {
        baseAddr += CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_TX_IPG1;
    }
    return baseAddr;
}

/*
 *  ======== emac_icssg_update_rgmii_cfg_100hd ========
 */
void emac_icssg_update_rgmii_cfg_100hd(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr)
{
    uint32_t regVal;

    regVal = CSL_REG32_RD(icssgRgmiiCfgBaseAddr);
    /* Need to clear bit 17 for slice 0 or bit 21 for slice 1 of ICSSG_RGMII_CFG to configure 100 mpbs */
    /* Need to clear bit 18 for slice 0 or bit 22 for slice 1 of ICSSG_RGMII_CFG to configure half duplex */
    if ((portNum & 1) == 0)
    {
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_GIG_IN_MASK;
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_FULLDUPLEX_IN_MASK;
    }
    else
    {
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_GIG_IN_MASK;
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_FULLDUPLEX_IN_MASK;
    }
    CSL_REG32_WR (icssgRgmiiCfgBaseAddr, regVal);
}
/*
 *  ======== emac_icssg_update_rgmii_cfg_100fd ========
 */
void emac_icssg_update_rgmii_cfg_100fd(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr)
{
    uint32_t regVal;
    regVal = CSL_REG32_RD(icssgRgmiiCfgBaseAddr);

    /* Need to clear bit 17 for slice 0 or bit 21 for slice 1 of ICSSG_RGMII_CFG to configure 100 mpbs */
    /* Need to set bit 18 for slice 0 or bit 22 for slice 1 of ICSSG_RGMII_CFG to configure full duplex */
    if ((portNum & 1) == 0)
    {
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_GIG_IN_MASK;
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_FULLDUPLEX_IN_MASK;
    }
    else
    {
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_GIG_IN_MASK;
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_FULLDUPLEX_IN_MASK;

    }
    CSL_REG32_WR(icssgRgmiiCfgBaseAddr, regVal);
}
/*
 *  ======== emac_icssg_update_rgmii_cfg_100MB ========
 */
void emac_icssg_update_rgmii_cfg_100MB(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr, uint32_t link_status)
{
    if (link_status == EMAC_MDIO_LINKSTATUS_HD100)
    {
        emac_icssg_update_rgmii_cfg_100hd(portNum, icssgRgmiiCfgBaseAddr);
    }
    else
    {
        emac_icssg_update_rgmii_cfg_100fd(portNum, icssgRgmiiCfgBaseAddr);
    }
}
/*
 *  ======== emac_icssg_update_rgmii_cfg_1G ========
 */
void emac_icssg_update_rgmii_cfg_1G(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr)
{
    uint32_t regVal;

    regVal = CSL_REG32_RD(icssgRgmiiCfgBaseAddr);
    if ((portNum & 1) == 0)
    {
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_GIG_IN_MASK;
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_FULLDUPLEX_IN_MASK;
    }
    else
    {
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_GIG_IN_MASK;
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_FULLDUPLEX_IN_MASK;
    }
    CSL_REG32_WR (icssgRgmiiCfgBaseAddr, regVal);
}

/*
 *  ======== emac_icssg_update_rgmii_cfg_10hd ========
 */
void emac_icssg_update_rgmii_cfg_10hd(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr)
{
    uint32_t regVal;

    regVal = CSL_REG32_RD(icssgRgmiiCfgBaseAddr);
    if ((portNum & 1) == 0)
    {
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_GIG_IN_MASK;
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_INBAND_MASK;
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_FULLDUPLEX_IN_MASK;
    }
    else
    {
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_GIG_IN_MASK;
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_INBAND_MASK;
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_FULLDUPLEX_IN_MASK;
    }

    CSL_REG32_WR (icssgRgmiiCfgBaseAddr, regVal);
}

/*
 *  ======== emac_icssg_update_rgmii_cfg_10fd ========
 */
void emac_icssg_update_rgmii_cfg_10fd(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr)
{
    uint32_t regVal;

    regVal = CSL_REG32_RD(icssgRgmiiCfgBaseAddr);
    if ((portNum & 1) == 0)
    {
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_GIG_IN_MASK;
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_INBAND_MASK;
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_FULLDUPLEX_IN_MASK;
    }
    else
    {
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_GIG_IN_MASK;
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_INBAND_MASK;
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_FULLDUPLEX_IN_MASK;
    }

    CSL_REG32_WR (icssgRgmiiCfgBaseAddr, regVal);
}

/*
 *  ======== emac_icssg_update_rgmii_cfg_10MB ========
 */
void emac_icssg_update_rgmii_cfg_10MB(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr, uint32_t link_status)
{
    if (link_status == EMAC_MDIO_LINKSTATUS_HD10)
    {
        emac_icssg_update_rgmii_cfg_10hd(portNum, icssgRgmiiCfgBaseAddr);
    }
    else
    {
        emac_icssg_update_rgmii_cfg_10fd(portNum, icssgRgmiiCfgBaseAddr);
    }
}

/*
 *  ======== emac_icssg_update_link_speed_10MB ========
 */
void emac_icssg_update_link_speed_10MB(uint32_t portNum, uint32_t link_status)
{
    uintptr_t icssgRgmiiCfgAddr, baseAddr;

    icssgRgmiiCfgAddr = emac_get_icssg_rgmii_cfg_base_addr(portNum);
    emac_icssg_update_rgmii_cfg_10MB(portNum, icssgRgmiiCfgAddr, link_status);
     /* issue speed_duplexity ioctl to fw */
    if (link_status == EMAC_MDIO_LINKSTATUS_HD10)
    {
        emac_configure_link_speed_duplexity(portNum, EMAC_IOCTL_SPEED_DUPLEXITY_10HD);
    }
    else
    {
        emac_configure_link_speed_duplexity(portNum, EMAC_IOCTL_SPEED_DUPLEXITY_10FD);
    }
    
    /*Notify the port link speed to firmware */
    if ((portNum & 1) == 0)
    {
        baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + PORT_LINK_SPEED_OFFSET;
    }
    else
    {
        baseAddr =  emac_mcb.port_cb[portNum].icssDram1BaseAddr + PORT_LINK_SPEED_OFFSET;
    }
    CSL_REG8_WR (baseAddr, FW_LINK_SPEED_10M);
}
/*
 *  ======== emac_icssg_update_link_speed_100MB ========
 */
void emac_icssg_update_link_speed_100MB(uint32_t portNum, uint32_t link_status)
{
    uintptr_t icssgRgmiiCfgAddr, icssgTxIpgCfgAddr, baseAddr;
    uint32_t regVal;
    

    icssgRgmiiCfgAddr = emac_get_icssg_rgmii_cfg_base_addr(portNum);
    emac_icssg_update_rgmii_cfg_100MB(portNum, icssgRgmiiCfgAddr, link_status);
    if (link_status == EMAC_MDIO_LINKSTATUS_HD100)
    {
        emac_configure_link_speed_duplexity(portNum, EMAC_IOCTL_SPEED_DUPLEXITY_100HD);
    }
    else
    {
        emac_configure_link_speed_duplexity(portNum, EMAC_IOCTL_SPEED_DUPLEXITY_100FD);
    }

    /*Configuring 960ns TX IPG in ICSSG HW MMR*/
    if (emac_mcb.port_cb[portNum].pgVersion == EMAC_AM65XX_PG1_0_VERSION)
    {
        regVal = EMAC_ICSSG_CONFIG_TX_IPG_960_NS_PG1;
        icssgTxIpgCfgAddr  = emac_get_icssg_tx_ipg_cfg_base_addr(portNum);
    }
    else
    {
    
        regVal = EMAC_ICSSG_CONFIG_TX_IPG_960_NS;
        icssgTxIpgCfgAddr  = emac_get_icssg_tx_ipg_cfg_base_addr(txIpgPortMap[portNum][0]);
        CSL_REG32_WR (icssgTxIpgCfgAddr, regVal);

        /* HW Errata: work-around to do RMW to TXIPG0 so that TXIPG1 will be latched by HW */
        if ((portNum & 1) == 1)
        {
            icssgTxIpgCfgAddr  = emac_get_icssg_tx_ipg_cfg_base_addr(txIpgPortMap[portNum][1]);
            regVal = CSL_REG32_RD(icssgTxIpgCfgAddr);
            CSL_REG32_WR (icssgTxIpgCfgAddr, regVal);
        }
    }
    
    /*Notify the port link speed to firmware */
    if ((portNum & 1) == 0)
    {
        baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + PORT_LINK_SPEED_OFFSET;
    }
    else
    {
        baseAddr =  emac_mcb.port_cb[portNum].icssDram1BaseAddr + PORT_LINK_SPEED_OFFSET;
    }
    CSL_REG8_WR (baseAddr, FW_LINK_SPEED_100M);
}

/*
 *  ======== emac_icssg_update_link_speed_1G ========
 */
void emac_icssg_update_link_speed_1G(uint32_t portNum)
{
    uintptr_t icssgRgmiiCfgAddr, icssgTxIpgCfgAddr, baseAddr;
    uint32_t regVal;

    icssgRgmiiCfgAddr = emac_get_icssg_rgmii_cfg_base_addr(portNum);
    emac_icssg_update_rgmii_cfg_1G(portNum, icssgRgmiiCfgAddr);
    /* issue speed_duplexity ioctl to fw */
    emac_configure_link_speed_duplexity(portNum, EMAC_IOCTL_SPEED_DUPLEXITY_GIGABIT);
    /*Configuring 104ns TX IPG ICSSG HW MMR */
    if (emac_mcb.port_cb[portNum].pgVersion == EMAC_AM65XX_PG1_0_VERSION)
    {
        regVal = EMAC_ICSSG_CONFIG_TX_IPG_104_NS_PG1;
        icssgTxIpgCfgAddr  = emac_get_icssg_tx_ipg_cfg_base_addr(portNum);
        CSL_REG32_WR (icssgTxIpgCfgAddr, regVal);
    }
    else
    {
        regVal = EMAC_ICSSG_CONFIG_TX_IPG_104_NS;
        icssgTxIpgCfgAddr  = emac_get_icssg_tx_ipg_cfg_base_addr(txIpgPortMap[portNum][0]);
        CSL_REG32_WR (icssgTxIpgCfgAddr, regVal);

        /* HW Errata: work-around to do RMW to TXIPG0 so that TXIPG1 will be latched by HW */
        if ((portNum & 1) == 1)
        {
            icssgTxIpgCfgAddr  = emac_get_icssg_tx_ipg_cfg_base_addr(txIpgPortMap[portNum][1]);
            regVal = CSL_REG32_RD(icssgTxIpgCfgAddr);
            CSL_REG32_WR (icssgTxIpgCfgAddr, regVal);
        }
    }
    
    /*Notify the port link speed to firmware */
    if ((portNum & 1) == 0)
    {
        baseAddr = emac_mcb.port_cb[portNum].icssDram0BaseAddr + PORT_LINK_SPEED_OFFSET;
    }
    else
    {
        baseAddr =  emac_mcb.port_cb[portNum].icssDram1BaseAddr + PORT_LINK_SPEED_OFFSET;
    }
    CSL_REG8_WR (baseAddr, FW_LINK_SPEED_1G);
}

/*
 *  ======== emac_configure_link_speed_duplexity ========
 */
void emac_configure_link_speed_duplexity(uint32_t portNum, uint32_t val)
{
    
    if (emac_mcb.port_cb[portNum].pgVersion == EMAC_AM65XX_PG1_0_VERSION)
    {
        EMAC_IOCTL_PARAMS params = {0};
        EMAC_DRV_ERR_E status;
        if (portNum < (EMAC_MAX_PORTS -1))
        {
            params.subCommand = val;
            params.seqNumber = emac_mcb.ioctl_cb.sequenceNumber++;

            if (emac_mcb.ioctl_cb.ioctlInProgress == false)
            {
                emac_mcb.ioctl_cb.internalIoctl = TRUE;
                status = emac_ioctl_speed_duplexity_cfg(portNum, &params);
                if (status != EMAC_DRV_RESULT_IOCTL_IN_PROGRESS)
                {
                    emac_mcb.ioctl_cb.internalIoctl = FALSE;
                    UTILS_trace(UTIL_TRACE_LEVEL_ERR, emac_mcb.drv_trace_cb, "port: %d: error in sending speed duplexity IOCTL",portNum);
                }
            }
        }
    }
}

/*
 *  ======== emac_icssg_link_down ========
 */
void emac_icssg_link_down(uint32_t portNum)
{
    uint32_t regVal;
    uintptr_t icssgRgmiiCfgAddr;

    icssgRgmiiCfgAddr = emac_get_icssg_rgmii_cfg_base_addr(portNum);
    regVal = CSL_REG32_RD(icssgRgmiiCfgAddr);
    if ((portNum & 1) == 0)
    {
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_GIG_IN_MASK;
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_FULLDUPLEX_IN_MASK;
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII0_INBAND_MASK;
    }
    else
    {
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_GIG_IN_MASK;
        regVal |= CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_FULLDUPLEX_IN_MASK;
        regVal &= ~CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RGMII_CFG_RGMII1_INBAND_MASK;
    }
    CSL_REG32_WR (icssgRgmiiCfgAddr, regVal);
    emac_configure_link_speed_duplexity(portNum, EMAC_IOCTL_SPEED_DUPLEXITY_DISABLE);
}

void emac_icssg_update_link_params(uint32_t portNum, EMAC_LINK_INFO_T *p_info)
{
    switch(p_info->link_status)
    {
        case EMAC_MDIO_LINKSTATUS_FD1000:
            emac_icssg_update_link_speed_1G(portNum);
            break;
        case EMAC_MDIO_LINKSTATUS_FD100:
        case EMAC_MDIO_LINKSTATUS_HD100:
            emac_icssg_update_link_speed_100MB(portNum, p_info->link_status);
            break;
        case EMAC_MDIO_LINKSTATUS_FD10:
        case EMAC_MDIO_LINKSTATUS_HD10:
            emac_icssg_update_link_speed_10MB(portNum, p_info->link_status);
            break;
        default:
            emac_icssg_link_down(portNum);
            break;
    }
}

