/**
 *   Copyright (c) Texas Instruments Incorporated 2018
 *   All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  \file vhwa_examples_crc.c
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <ti/csl/soc.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <examples/include/vhwa_examples_common.h>
#include <examples/include/vhwa_common_crc.h>
#include <vhwa_vpac_abstraction_layer.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/*
 * Ring parameters
 */
/** \brief Number of ring entries - we can prime this much CRC operations */
#define UDMA_TEST_APP_RING_ENTRIES      (1U)
/** \brief Size (in bytes) of each ring entry (Size of pointer - 64-bit) */
#define UDMA_TEST_APP_RING_ENTRY_SIZE   (sizeof(uint64_t))
/** \brief Total ring memory */
#define UDMA_TEST_APP_RING_MEM_SIZE     (UDMA_TEST_APP_RING_ENTRIES * \
                                         UDMA_TEST_APP_RING_ENTRY_SIZE)
/**
 *  \brief UDMA TR packet descriptor memory.
 *  This contains the CSL_UdmapCppi5TRPD + Padding to sizeof(CSL_UdmapTR15) +
 *  one Type_15 TR (CSL_UdmapTR15) + one TR response of 4 bytes.
 *  Since CSL_UdmapCppi5TRPD is less than CSL_UdmapTR15, size is just two times
 *  CSL_UdmapTR15 for alignment.
 */
#define UDMA_TEST_APP_TRPD_SIZE         ((sizeof(CSL_UdmapTR15) * 2U) + 4U)

/* CRC channel parameters */
#define APP_MAX_CRC_CHANNEL             (4U)
#define APP_CRC_CH_CCITENR_MASK(ch)     (0x1 << (8 * (ch - 1)))
#define APP_CRC_WATCHDOG_PRELOAD_VAL    ((uint32_t) 0U)
#define APP_CRC_BLOCK_PRELOAD_VAL       ((uint32_t) 0U)

/* CRC size parameters */
#define APP_CRC_PATTERN_SIZE            ((uint32_t) 8U)
#define APP_CRC_SECT_CNT                ((uint32_t) 1U)

#define APP_CRC_BASE                    (CSL_NAVSS0_MCRC_BASE)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t App_udmaCrc(AppCrc_hdlPrms *chHandle,
                            uint64_t sAddr,
                            AppCrc_inPrms *srcPrms,
                            uint64_t *crcSignVal);

static void App_udmaTrpdInit(Udma_ChHandle chHandle,
                             uint8_t *pTrpdMem,
                             const void *destBuf,
                             uint64_t sAddr,
                             uint32_t width,
                             uint32_t pitch,
                             uint32_t height);

static void App_udmaCrcEventCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData);

static int32_t App_crcInit(uint32_t crcChannel);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/*
 * UDMA driver objects
 */
static struct Udma_ChObj       gUdmaCrcChObj[APP_MAX_CRC_CHANNEL];
static struct Udma_EventObj    gUdmaCrcCqEventObj[APP_MAX_CRC_CHANNEL];
static struct Udma_EventObj    gUdmaCrcTdCqEventObj[APP_MAX_CRC_CHANNEL];

/*
 * UDMA Memories
 */
static uint8_t gTxRingMem[APP_MAX_CRC_CHANNEL][UDMA_TEST_APP_RING_MEM_SIZE]
                    __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

static uint8_t gTxCompRingMem[APP_MAX_CRC_CHANNEL][UDMA_TEST_APP_RING_MEM_SIZE]
                    __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

static uint8_t gTxTdCompRingMem[APP_MAX_CRC_CHANNEL][UDMA_TEST_APP_RING_MEM_SIZE]
                    __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

static uint8_t gUdmaTprdMem[APP_MAX_CRC_CHANNEL][UDMA_TEST_APP_TRPD_SIZE]
                    __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

static VhwaAl_SemaphoreP_Struct gCrcDoneSem[APP_MAX_CRC_CHANNEL] = {NULL};
static VhwaAl_SemaphoreP_Struct gCrcChLock[APP_MAX_CRC_CHANNEL] = {NULL};

static volatile uint32_t crcEnableStatus[APP_MAX_CRC_CHANNEL] = {(uint32_t)NULL};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t App_getCrc(AppCrc_hdlPrms *chHandle, uint64_t sAddr,
                AppCrc_inPrms *srcPrms, uint64_t *crcSignVal)
{
    int32_t     retVal = UDMA_SOK;

    /* Perform UDMA CRC */
    retVal = App_udmaCrc(chHandle, sAddr, srcPrms, crcSignVal);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] App_udmaCrc\n");
    }

    return (retVal);
}

static int32_t App_udmaCrc(AppCrc_hdlPrms *chHandle, uint64_t sAddr,
                            AppCrc_inPrms *srcPrms,
                            uint64_t *crcSignVal)
{
    int32_t               retVal = UDMA_SOK;
    uint32_t             *pTrResp, trRespStatus;
    uint64_t              pDesc = 0;
    uint8_t              *tprdMem = &gUdmaTprdMem[chHandle->crcChannel-1][0U];
    crcSignatureRegAddr_t psaSignRegAddr;
    crcSignature_t        sectSignVal;
    uint32_t              patternCnt;
    uint32_t              iterCnt = 1;
    uint32_t              height;
    uint32_t              crcIntrStatus;


    VhwaAl_SemaphoreP_pend(gCrcChLock[chHandle->crcChannel-1], SYSTEMP_WAIT_FOREVER);

    sectSignVal.regL = 0U;
    sectSignVal.regH = 0U;
    height = srcPrms->height;
    patternCnt = (srcPrms->width * height) / APP_CRC_PATTERN_SIZE;

    while(patternCnt > 0xFFFFFU)
    {
        height = height/2;
        patternCnt = (srcPrms->width * height) / APP_CRC_PATTERN_SIZE;
        iterCnt = iterCnt*2;
    }

    while(iterCnt > 0)
    {
        /* Get CRC PSA signature register address */
        CRCGetPSASigRegAddr(APP_CRC_BASE, chHandle->crcChannel, &psaSignRegAddr);
        CRCChannelReset(APP_CRC_BASE, chHandle->crcChannel);

        CRCSetPSASeedSig(APP_CRC_BASE, chHandle->crcChannel, &sectSignVal);

        CRCConfigure(APP_CRC_BASE, chHandle->crcChannel, patternCnt, APP_CRC_SECT_CNT, CRC_OPERATION_MODE_SEMICPU);

        /* Update TR packet descriptor */
        App_udmaTrpdInit(chHandle->udmaCrcChHandle, tprdMem,
                        (void *)psaSignRegAddr.regL,
                        sAddr, srcPrms->width, srcPrms->pitch, height);

        /* Submit TRPD to channel */
        retVal = Udma_ringQueueRaw(Udma_chGetFqRingHandle(chHandle->udmaCrcChHandle),
                                   (uint64_t) tprdMem);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] Channel queue failed!!\n");
        }

        if(UDMA_SOK == retVal)
        {
            /* Wait for return descriptor in completion ring - this marks the
             * transfer completion */
            VhwaAl_SemaphoreP_pend(gCrcDoneSem[chHandle->crcChannel-1], SYSTEMP_WAIT_FOREVER);

            /* Response received in completion queue */
            retVal = Udma_ringDequeueRaw(
                            Udma_chGetCqRingHandle(chHandle->udmaCrcChHandle),
                            &pDesc);
            if(UDMA_SOK != retVal)
            {
                App_print("[Error] No descriptor after callback!!\n");
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /*
             * Sanity check
             */
            /* Check returned descriptor pointer */
            if(pDesc != ((uint64_t) tprdMem))
            {
                App_print("[Error] TR descriptor pointer returned doesn't "
                       "match the submitted address!!\n");
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Invalidate cache */
            VhwaAl_Cache_Inv(&gUdmaTprdMem[chHandle->crcChannel][0U],
                        UDMA_TEST_APP_TRPD_SIZE, CacheP_TYPE_L1D);

            /* check TR response status */
            pTrResp = (uint32_t *) (tprdMem + (sizeof(CSL_UdmapTR15) * 2U));
            trRespStatus = CSL_FEXT(*pTrResp, UDMAP_TR_RESPONSE_STATUS_TYPE);
            if(trRespStatus != CSL_UDMAP_TR_RESPONSE_STATUS_COMPLETE)
            {
                App_print("[Error] TR Response not completed!!\n");
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            CRCGetIntrStatus(APP_CRC_BASE, chHandle->crcChannel, &crcIntrStatus);
            while ((crcIntrStatus & APP_CRC_CH_CCITENR_MASK(chHandle->crcChannel))
                        != 0x1U)
            {
                /* Wait here till CRC compression complete is set. */
                CRCGetIntrStatus(APP_CRC_BASE, chHandle->crcChannel, &crcIntrStatus);
            }

            CRCGetPSASectorSig(APP_CRC_BASE, chHandle->crcChannel, &sectSignVal);

            CRCClearIntr(APP_CRC_BASE, chHandle->crcChannel, CRC_CHANNEL_IRQSTATUS_RAW_MAIN_ALL);
        }

        sAddr = sAddr + (srcPrms->pitch * height);
        iterCnt--;
    }

    *crcSignVal = ((sectSignVal.regL & 0xFFFFFFFFU) |
                  (((uint64_t)sectSignVal.regH << 32) & 0xFFFFFFFF00000000U));

    VhwaAl_SemaphoreP_post(gCrcChLock[chHandle->crcChannel-1]);

    return (retVal);
}

static void App_udmaCrcEventCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData)
{
    int32_t         retVal;
    CSL_UdmapTdResponse tdResp;

    if(UDMA_EVENT_TYPE_DMA_COMPLETION == eventType)
    {
        VhwaAl_SemaphoreP_post(gCrcDoneSem[*((uint32_t *)appData) - 1]);
    }
    if(UDMA_EVENT_TYPE_TEARDOWN_PACKET == eventType)
    {
        /* Response received in Teardown completion queue */
        retVal = Udma_chDequeueTdResponse(&gUdmaCrcChObj[*((uint32_t *)appData) - 1], &tdResp);
        if(UDMA_SOK != retVal)
        {
            /* [Error] No TD response after callback!! */
        }
    }

    return;
}

Udma_ChHandle App_crcCreate(Udma_DrvHandle drvHandle, uint32_t *crcChannel)
{
    int32_t             retVal;
    uint32_t            chType;
    uint32_t            chIdx;
    Udma_ChPrms         chPrms;
    Udma_ChTxPrms       txPrms;
    Udma_ChRxPrms       rxPrms;
    Udma_ChHandle       chHandle;
    Udma_EventHandle    eventHandle;
    Udma_EventPrms      eventPrms;
    SemaphoreP_Params   semPrms;

    chIdx = *crcChannel - 1;

    chHandle = &gUdmaCrcChObj[chIdx];

    if(0u == crcEnableStatus[chIdx])
    {
        retVal = App_crcInit(*crcChannel);

        SemaphoreP_Params_init(&semPrms);
        gCrcDoneSem[chIdx] = SemaphoreP_create(0, &semPrms);
        if(NULL == gCrcDoneSem[chIdx])
        {
            App_print("[Error] Sem create failed!!\n");
            retVal = UDMA_EFAIL;
        }

        /* Init channel parameters */
        chType = UDMA_CH_TYPE_TR_BLK_COPY;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.ringMem   = &gTxRingMem[chIdx][0U];
        chPrms.cqRingPrms.ringMem   = &gTxCompRingMem[chIdx][0U];
        chPrms.tdCqRingPrms.ringMem = &gTxTdCompRingMem[chIdx][0U];
        chPrms.fqRingPrms.elemCnt   = UDMA_TEST_APP_RING_ENTRIES;
        chPrms.cqRingPrms.elemCnt   = UDMA_TEST_APP_RING_ENTRIES;
        chPrms.tdCqRingPrms.elemCnt = UDMA_TEST_APP_RING_ENTRIES;

        /* Open channel for block copy */
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA channel open failed!!\n");
        }

        if(UDMA_SOK == retVal)
        {
            /* Config TX channel */
            UdmaChTxPrms_init(&txPrms, chType);
            retVal = Udma_chConfigTx(chHandle, &txPrms);
            if(UDMA_SOK != retVal)
            {
                App_print("[Error] UDMA TX channel config failed!!\n");
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Config RX channel - which is implicitly paired to TX channel in
             * block copy mode */
            UdmaChRxPrms_init(&rxPrms, chType);
            retVal = Udma_chConfigRx(chHandle, &rxPrms);
            if(UDMA_SOK != retVal)
            {
                App_print("[Error] UDMA RX channel config failed!!\n");
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Register ring completion callback */
            eventHandle = &gUdmaCrcCqEventObj[chIdx];
            UdmaEventPrms_init(&eventPrms);
            eventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
            eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
            eventPrms.chHandle          = chHandle;
            eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
            eventPrms.eventCb           = &App_udmaCrcEventCb;
            eventPrms.appData           = crcChannel;
            retVal = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
            if(UDMA_SOK != retVal)
            {
                App_print("[Error] UDMA CQ event register failed!!\n");
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Register teardown ring completion callback */
            eventHandle = &gUdmaCrcTdCqEventObj[chIdx];
            UdmaEventPrms_init(&eventPrms);
            eventPrms.eventType         = UDMA_EVENT_TYPE_TEARDOWN_PACKET;
            eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
            eventPrms.chHandle          = chHandle;
            eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
            eventPrms.eventCb           = &App_udmaCrcEventCb;
            eventPrms.appData           = crcChannel;
            retVal = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
            if(UDMA_SOK != retVal)
            {
                App_print("[Error] UDMA Teardown CQ event register failed!!\n");
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Channel enable */
            retVal = Udma_chEnable(chHandle);
            if(UDMA_SOK != retVal)
            {
                App_print("[Error] UDMA channel enable failed!!\n");
            }
            else
            {
                crcEnableStatus[chIdx] = 1U;
            }
        }
        if(UDMA_SOK != retVal)
        {
            chHandle = NULL;
        }
    }

    return (chHandle);
}

int32_t App_crcDelete(Udma_DrvHandle drvHandle, AppCrc_hdlPrms *chHandle)
{
    int32_t             retVal = UDMA_SOK;
    Udma_EventHandle    eventHandle;
    uint32_t            chIdx;

    chIdx = chHandle->crcChannel - 1;

    if(1u == crcEnableStatus[chIdx])
    {
        retVal = Udma_chDisable(chHandle->udmaCrcChHandle,
                                UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA channel disable failed!!\n");
        }

        /* Unregister master event at the end */
        eventHandle = &gUdmaCrcTdCqEventObj[chIdx];
        retVal += Udma_eventUnRegister(eventHandle);
        eventHandle = &gUdmaCrcCqEventObj[chIdx];
        retVal += Udma_eventUnRegister(eventHandle);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA event unregister failed!!\n");
        }

        retVal += Udma_chClose(chHandle->udmaCrcChHandle);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA channel close failed!!\n");
        }

        if(gCrcDoneSem[chIdx] != NULL)
        {
            SemaphoreP_delete(gCrcDoneSem[chIdx]);
            gCrcDoneSem[chIdx] = NULL;
        }

        if(gCrcChLock[chIdx] != NULL)
        {
            SemaphoreP_delete(gCrcChLock[chIdx]);
            gCrcChLock[chIdx] = NULL;
        }

        crcEnableStatus[chIdx] = 0u;
    }

    return (retVal);
}

static void App_udmaTrpdInit(Udma_ChHandle chHandle,
                             uint8_t *pTrpdMem,
                             const void *destBuf,
                             uint64_t sAddr,
                             uint32_t width,
                             uint32_t pitch,
                             uint32_t height)
{
    CSL_UdmapCppi5TRPD *pTrpd = (CSL_UdmapCppi5TRPD *) pTrpdMem;
    CSL_UdmapTR15 *pTr = (CSL_UdmapTR15 *)(pTrpdMem + sizeof(CSL_UdmapTR15));
    uint32_t *pTrResp = (uint32_t *) (pTrpdMem + (sizeof(CSL_UdmapTR15) * 2U));
    uint32_t cqRingNum = Udma_chGetCqRingNum(chHandle);
    uint32_t descType = CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_TR;
    uint32_t dicnt1,dicnt2;

    /* Setup descriptor */
    CSL_udmapCppi5SetDescType(pTrpd, descType);
    CSL_udmapCppi5TrSetReload(pTrpd, 0U, 0U);
    CSL_udmapCppi5SetPktLen(pTrpd, descType, 1U);       /* Only one TR in TRPD */
    CSL_udmapCppi5SetIds(pTrpd, descType, 0U, 0x3FFFU); /* Flow ID and Packet ID */
    CSL_udmapCppi5SetSrcTag(pTrpd, 0x0000);     /* Not used */
    CSL_udmapCppi5SetDstTag(pTrpd, 0x0000);     /* Not used */
    CSL_udmapCppi5TrSetEntryStride(
        pTrpd, CSL_UDMAP_CPPI5_TRPD_PKTINFO_RECSIZE_VAL_64B);
    CSL_udmapCppi5SetReturnPolicy(
        pTrpd,
        descType,
        CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,   /* Don't care for TR */
        CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
        CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
        cqRingNum);

    dicnt2 = 1;
    dicnt1 = width * height / APP_CRC_PATTERN_SIZE;
    while ((dicnt1 / dicnt2) > 0xFFFFU)
    {
        dicnt2 = dicnt2 * 2;
    }

    /* Setup TR */
    pTr->flags    = CSL_FMK(UDMAP_TR_FLAGS_TYPE, 15)                                            |
                    CSL_FMK(UDMAP_TR_FLAGS_STATIC, 0U)                                          |
                    CSL_FMK(UDMAP_TR_FLAGS_EOL, 0U)                                             |   /* NA */
                    CSL_FMK(UDMAP_TR_FLAGS_EVENT_SIZE, CSL_UDMAP_TR_FLAGS_EVENT_SIZE_COMPLETION)|
                    CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0, CSL_UDMAP_TR_FLAGS_TRIGGER_NONE)           |
                    CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0_TYPE, CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ALL)  |
                    CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1, CSL_UDMAP_TR_FLAGS_TRIGGER_NONE)           |
                    CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1_TYPE, CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ALL)  |
                    CSL_FMK(UDMAP_TR_FLAGS_CMD_ID, 0x25U)                                       |
                    CSL_FMK(UDMAP_TR_FLAGS_SA_INDIRECT, 0U)                                     |
                    CSL_FMK(UDMAP_TR_FLAGS_DA_INDIRECT, 0U)                                     |
                    CSL_FMK(UDMAP_TR_FLAGS_EOP, 1U);
    pTr->icnt0    = width;
    pTr->icnt1    = height;
    pTr->icnt2    = 1U;
    pTr->icnt3    = 1U;
    pTr->dim1     = pitch;
    pTr->dim2     = (pTr->dim1 * pTr->icnt1);
    pTr->dim3     = (pTr->dim2 * pTr->icnt2);
    pTr->addr     = sAddr;
    pTr->fmtflags = 0x00000000U;        /* Linear addressing, 1 byte per elem.
                                           Replace with CSL-FL API */
    pTr->dicnt0   = APP_CRC_PATTERN_SIZE;
    pTr->dicnt1   = dicnt1/dicnt2;
    pTr->dicnt2   = dicnt2;
    pTr->dicnt3   = 1U;
    pTr->ddim1    = 0U;
    pTr->ddim2    = 0U;
    pTr->ddim3    = 0U;
    pTr->daddr    = (uint64_t) destBuf;

    /* Clear TR response memory */
    *pTrResp = 0xFFFFFFFFU;

    /* Writeback cache */
    VhwaAl_Cache_Wb(pTrpdMem, UDMA_TEST_APP_TRPD_SIZE, CacheP_TYPE_L1D);

    return;
}

static int32_t App_crcInit(uint32_t crcChannel)
{
    int32_t retVal = UDMA_SOK;
    SemaphoreP_Params   semPrms;

    SemaphoreP_Params_init(&semPrms);
    semPrms.mode = SemaphoreP_Mode_BINARY;
    gCrcChLock[crcChannel-1] = SemaphoreP_create(1U, &semPrms);
    if(NULL == gCrcChLock[crcChannel-1])
    {
        App_print("[Error] Sem create failed!!\n");
        retVal = UDMA_EFAIL;
    }

    /* Configure CRC channel */
    CRCInitialize(
        APP_CRC_BASE,
        crcChannel,
        APP_CRC_WATCHDOG_PRELOAD_VAL,
        APP_CRC_BLOCK_PRELOAD_VAL);

    return retVal;
}
