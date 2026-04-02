/*
 *  Copyright (c) Texas Instruments Incorporated 2020-2022
 *  All rights reserved.
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
 *  \file csitx_drvUdma.c
 *
 *  \brief File containing the CSITX driver UDMA initialization and
 *         configuration code.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/drv/csitx/src/csitx_drvPriv.h>
#include <ti/drv/csitx/src/csitx_drvUdma.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/*
 * UDMA Memories
 */
static uint8_t gCsitxRingMem[CSITX_NUM_CH][CSITX_DRV_RING_MEM_SIZE_ALIGN]
                            __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
#if defined (SOC_J721E)
static uint8_t gCsitxCompRingMem[CSITX_NUM_CH][CSITX_DRV_RING_MEM_SIZE_ALIGN]
                            __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gCsitxTdCompRingMem[CSITX_NUM_CH][CSITX_DRV_RING_MEM_SIZE_ALIGN]
                            __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
#endif
static uint8_t gCsitxUdmaTprdMem
        [CSITX_NUM_CH][CSITX_TX_QUEUE_DEPTH_PER_CH][CSITX_DRV_TRPD_SIZE_ALIGN]
                            __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
extern CsitxDrv_CommonObj gCsitxCommonObj;
#if (CSITX_DRV_ENABLE_DEBUG == 1U)
extern uint32_t gTxTrSubmit[CSITX_DRV_TR_LOG_CNT][3];
extern uint32_t gTxTrSubmitCnt;
extern uint32_t gTxTrOut[CSITX_DRV_TR_LOG_CNT][3];
extern uint32_t gTxTrOutCnt;
#endif
/* ========================================================================== */
/*                  Internal/Private Function Declarations                   */
/* ========================================================================== */
static uint16_t CsitxDrv_getIcnt0(const Csitx_ChCfg *chCfg);
static uint32_t CsitxDrv_dtIndex(const CsitxDrv_ChObj *chObj);
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int32_t CsitxDrv_setDMACfgParams(CsitxDrv_ChObj *chObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t chType;

    /* Assign UDMA Tx channel params from channel configurations */
    chObj->txChParams = &chObj->chCfg->txChParams;
    /* Init UDMA channel parameters */
    chType = UDMA_CH_TYPE_TX;
    UdmaChPrms_init(&chObj->chParams, chType);
    chObj->trpdMem = &gCsitxUdmaTprdMem[chObj->psilThreadId][0U][0U];
    chObj->txFqRingMem = &gCsitxRingMem[chObj->psilThreadId][0U];
#if defined (SOC_J721E)
    chObj->txCqRingMem = &gCsitxCompRingMem[chObj->psilThreadId][0U];
    chObj->txTdCqRingMem = &gCsitxTdCompRingMem[chObj->psilThreadId][0U];
    chObj->chParams.cqRingPrms.ringMem   = chObj->txCqRingMem;
    chObj->chParams.tdCqRingPrms.ringMem = chObj->txTdCqRingMem;
    /* Initialize ring order ID */
    chObj->chParams.cqRingPrms.orderId   = chObj->chCfg->ringOrderId;
    chObj->chParams.tdCqRingPrms.orderId = chObj->chCfg->ringOrderId;
    chObj->chParams.cqRingPrms.elemCnt   = CSITX_TX_QUEUE_DEPTH_PER_CH;
    chObj->chParams.tdCqRingPrms.elemCnt = CSITX_TX_QUEUE_DEPTH_PER_CH;
#endif
    if(CSITX_INSTANCE_ID_0 == chObj->instObj->drvInstId)
    {
        chObj->chParams.peerChNum            = (CSL_PSIL_CSI_TX0_CH0 +
                                           chObj->psilThreadId);
    }
#if defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)
    else if(CSITX_INSTANCE_ID_1 == chObj->instObj->drvInstId)
    {
        chObj->chParams.peerChNum            = (CSL_PSIL_CSI_TX1_CH0 +
                                           chObj->psilThreadId);
    }
#endif    
    else
    {
        /* Do nothing */
    }
    chObj->chParams.fqRingPrms.ringMem   = chObj->txFqRingMem;
    chObj->chParams.fqRingPrms.orderId   = chObj->chCfg->ringOrderId;
    chObj->chParams.fqRingPrms.elemCnt   = CSITX_TX_QUEUE_DEPTH_PER_CH;
    /* Config TX channel */
    /* Get the PSI-L Threads/DMA channel configuration parameters */
    chObj->dmaCfgParams.chNum = chObj->psilThreadId;
    chObj->dmaCfgParams.vc    = chObj->chCfg->vcNum;
#if defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)
    chObj->dmaCfgParams.dt    = chObj->chCfg->outCsiDataType;
#else
    chObj->dmaCfgParams.dt    = CsitxDrv_dtIndex(chObj);
    GT_assert(CsitxTrace,
            (chObj->dmaCfgParams.dt < chObj->instObj->cslObj.dtInUse));

#endif
#if defined (SOC_J721E)
    /* Enable YUV420 mode if needed */
    if ((FVID2_CSI2_DF_YUV420_8B  == chObj->chCfg->outCsiDataType) ||
        (FVID2_CSI2_DF_YUV420_10B == chObj->chCfg->outCsiDataType) ||
        (FVID2_CSI2_DF_YUV420_8B_LEGACY == chObj->chCfg->outCsiDataType))
    {
        chObj->dmaCfgParams.yuv420Enable    = UTRUE;
    }
#endif

    if (FVID2_CSI2_DF_YUV422_8B == chObj->chCfg->outCsiDataType)
    {
#if defined (SOC_J721E)
        chObj->dmaCfgParams.yuv422Enable    = UTRUE;
#endif
        /* Storage mode for YUV422 data format */
        if (FVID2_DF_YUV422I_UYVY == chObj->chCfg->inFmt.dataFormat)
        {
            chObj->dmaCfgParams.yuv422Mode      = CSL_CSITX_YUV422_MODE_YVYU;
        }
        else if (FVID2_DF_YUV422I_YUYV == chObj->chCfg->inFmt.dataFormat)
        {
            chObj->dmaCfgParams.yuv422Mode = CSL_CSITX_YUV422_MODE_VYUY;
        }
        else if (FVID2_DF_YUV422I_YVYU == chObj->chCfg->inFmt.dataFormat)
        {
            chObj->dmaCfgParams.yuv422Mode = CSL_CSITX_YUV422_MODE_UYVY;
        }
        else if (FVID2_DF_YUV422I_VYUY == chObj->chCfg->inFmt.dataFormat)
        {
            chObj->dmaCfgParams.yuv422Mode = CSL_CSITX_YUV422_MODE_YUYV;
        }
        else
        {
            /* If input format is not YUV422, this parameter values is not used */
            chObj->dmaCfgParams.yuv422Mode = CSL_CSITX_YUV422_MODE_UYVY;
        }
    }

    if (FVID2_CCSF_BITS12_PACKED == chObj->chCfg->inFmt.ccsFormat)
    {
        chObj->dmaCfgParams.pck12CfgEnable = UTRUE;
    }
    else if (FVID2_CCSF_BITS12_UNPACKED16 == chObj->chCfg->inFmt.ccsFormat)
    {
        chObj->dmaCfgParams.pck12CfgEnable = UFALSE;
    }
    else
    {
        /* If input format is not RAW12, this parameter values is not used */
        chObj->dmaCfgParams.pck12CfgEnable = UFALSE;
    }
    if (FVID2_CSI2_DF_RAW20 == chObj->chCfg->outCsiDataType)
    {
        chObj->dmaCfgParams.dataSizeShift =
                            CSL_CSITX_DMA_DATA_SIZE_SHIFT_32BITS;
    }
    else if ((FVID2_CSI2_DF_YUV420_10B == chObj->chCfg->outCsiDataType) ||
             (FVID2_CSI2_DF_YUV422_10B == chObj->chCfg->outCsiDataType) ||
             (FVID2_CSI2_DF_YUV422_8B  == chObj->chCfg->outCsiDataType) ||
             (FVID2_CSI2_DF_RAW10      == chObj->chCfg->outCsiDataType) ||
             (FVID2_CSI2_DF_RAW12      == chObj->chCfg->outCsiDataType) ||
             (FVID2_CSI2_DF_RAW14      == chObj->chCfg->outCsiDataType) ||
             (FVID2_CSI2_DF_RAW16      == chObj->chCfg->outCsiDataType) ||
             (FVID2_CSI2_DF_RGB888     == chObj->chCfg->outCsiDataType))
    {
        chObj->dmaCfgParams.dataSizeShift =
                            CSL_CSITX_DMA_DATA_SIZE_SHIFT_32BITS;
        if ((FVID2_CSI2_DF_RAW12      == chObj->chCfg->outCsiDataType) &&
            (FVID2_CCSF_BITS12_PACKED == chObj->chCfg->inFmt.ccsFormat))
        {
            chObj->dmaCfgParams.dataSizeShift =
                            CSL_CSITX_DMA_DATA_SIZE_SHIFT_8BITS;
        }
    }
    else if (FVID2_CSI2_DF_RAW8 == chObj->chCfg->outCsiDataType)
    {
        chObj->dmaCfgParams.dataSizeShift =
                            CSL_CSITX_DMA_DATA_SIZE_SHIFT_16BITS;
    }
    else
    {
        chObj->dmaCfgParams.dataSizeShift =
                            CSL_CSITX_DMA_DATA_SIZE_SHIFT_8BITS;
    }
    chObj->dmaCfgParams.l2lDelay     = chObj->chCfg->hBlank;
    retVal = CSL_csitxFrmToFrmDelayCfg((CSL_csi_tx_ifRegs *)chObj->instObj->shimBaseAddr,
                                                            chObj->chCfg->vcNum,
                                                            chObj->chCfg->vBlank);
#if defined(SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)
    chObj->dmaCfgParams.numBytesLine =  (chObj->chCfg->inFmt.width * CsitxDrv_getBpp(chObj->chCfg->outCsiDataType)) / 8U;
#else      
    chObj->dmaCfgParams.numBytesLine = CsitxDrv_getIcnt0(chObj->chCfg);
#endif    
    GT_assert(CsitxTrace,(0U != chObj->dmaCfgParams.numBytesLine));

    return retVal;
}

int32_t CsitxDrv_setChUdmaParams(CsitxDrv_ChObj *chObj)
{
    int32_t retVal = FVID2_SOK, tempRetVal = UDMA_SOK;
    uint32_t chType;
    Udma_EventHandle eventHandle;
    Udma_EventPrms eventParams;
    CsitxDrv_CommonObj *txObj;

    txObj = chObj->instObj->commonObjRef;
   if (chObj->chCfg->chType == CSITX_CH_TYPE_TX)
   {
       if (txObj->initParams.drvHandle == NULL)
       {
           retVal = FVID2_EBADARGS;
           GT_0trace(CsitxTrace,
                     GT_ERR,
                     "Invalid UDMA DRV Handle!!\r\n");
       }
       if (txObj->masterEvent.eventInitDone != UDMA_INIT_DONE)
       {

           /* Register the UDMA Master event handle */
           UdmaEventPrms_init(&eventParams);
           eventParams.eventType         = UDMA_EVENT_TYPE_MASTER;
           eventParams.appData           = txObj;
           tempRetVal = Udma_eventRegister(txObj->initParams.drvHandle,
                                           &txObj->masterEvent,
                                           &eventParams);

       }
       /* Program UDMA channels only for Tx channels */
       chType = UDMA_CH_TYPE_TX;
       /* Open UDMA channel */
       tempRetVal = Udma_chOpen(txObj->initParams.drvHandle,
                                &chObj->txChObj,
                                chType,
                                &chObj->chParams);

       if(tempRetVal != UDMA_SOK)
       {
           retVal = FVID2_EFAIL;
           GT_0trace(CsitxTrace,
                     GT_ERR,
                     "[ERROR]UDMA channel open FAILED!!!\r\n");
       }

       if (retVal == FVID2_SOK)
       {
           /* Configure UDMA Tx Channel */
           tempRetVal = Udma_chConfigTx(&chObj->txChObj,
                                        chObj->txChParams);

           if(tempRetVal != UDMA_SOK)
           {
               retVal = FVID2_EFAIL;
               GT_0trace(CsitxTrace,
                     GT_ERR,
                     "[ERROR]UDMA TX channel config FAILED!!!\r\n");
           }

       }

       if (retVal == FVID2_SOK)
       {
           /* Register callbacks here */
           /* Register ring completion callback */
           eventHandle = &chObj->cqEventObj;
           UdmaEventPrms_init(&eventParams);
           eventParams.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
           eventParams.eventMode         = UDMA_EVENT_MODE_SHARED;
           eventParams.chHandle          = &chObj->txChObj;
           eventParams.masterEventHandle = &txObj->masterEvent;
           eventParams.eventCb           = &CsitxDrv_udmaCQEventCb;
           eventParams.appData           = chObj;
           tempRetVal = Udma_eventRegister(txObj->initParams.drvHandle,
                                           eventHandle,
                                           &eventParams);

           if(tempRetVal != UDMA_SOK)
           {
               retVal = FVID2_EFAIL;
               GT_0trace(CsitxTrace,
                     GT_ERR,
                     "[ERROR]UDMA CQ event register FAILED!!!\r\n");
           }

        }

#if defined (SOC_J721E)
       if(UDMA_SOK == retVal)
       {
           /* Register teardown ring completion callback */
           eventHandle = &chObj->tdcEventObj;
           UdmaEventPrms_init(&eventParams);
           eventParams.eventType         = UDMA_EVENT_TYPE_TEARDOWN_PACKET;
           eventParams.eventMode         = UDMA_EVENT_MODE_SHARED;
           eventParams.chHandle          = &chObj->txChObj;
           eventParams.masterEventHandle = &txObj->masterEvent;
           eventParams.eventCb           = &CsitxDrv_udmaTDCEventCb;
           eventParams.appData           = chObj;
           tempRetVal = Udma_eventRegister(txObj->initParams.drvHandle,
                                           eventHandle,
                                           &eventParams);

           if(UDMA_SOK != tempRetVal)
           {
               retVal = FVID2_EFAIL;
               GT_0trace(CsitxTrace,
                   GT_ERR,
                   "[ERROR]UDMA Tear Down Q event register FAILED!!!\r\n");
           }

       }
#endif
   }

   if (FVID2_SOK == retVal)
   {
       /* Set channel status to created */
       chObj->status = CSITX_DRV_CH_STATE_CREATED;
   }


    return retVal;
}

int32_t CsitxDrv_clearUdmaParams(CsitxDrv_ChObj *chObj)
{
    int32_t retVal = FVID2_SOK, tempRetVal = UDMA_SOK;
    Udma_EventHandle eventHandle;

        if (CSITX_CH_TYPE_TX == chObj->chCfg->chType)
        {
            /* Unregister ring completion callback */
            eventHandle = &chObj->cqEventObj;
            tempRetVal += Udma_eventUnRegister(eventHandle);
#if defined (SOC_J721E)
            /* Unregister teardown ring completion callback */
            eventHandle = &chObj->tdcEventObj;
            tempRetVal += Udma_eventUnRegister(eventHandle);
#endif

            /* Close UDMA channel */
            tempRetVal += Udma_chClose(&chObj->txChObj);
        }

    if (UDMA_SOK != tempRetVal)
    {
        retVal = FVID2_EFAIL;
    }

    return retVal;
}

int32_t CsitxDrv_udmaTxTrpdInit(Udma_ChHandle txChHandle,
                                uint8_t *pTrpdMem,
                                const uint32_t *srcBuf,
                                const Csitx_ChCfg *chCfg,
                                uint32_t chIdx)
{
    int32_t retVal = FVID2_SOK;
    CSL_UdmapCppi5TRPD *pTrpd = (CSL_UdmapCppi5TRPD *) pTrpdMem;
    uint32_t descType = (uint32_t)CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_TR;
    uint32_t cqRingNum = Udma_chGetCqRingNum(txChHandle);
    CSL_UdmapTR1 *pTr = (CSL_UdmapTR1 *)(pTrpdMem + sizeof(CSL_UdmapTR15));
    uint32_t *pTrResp =
                (uint32_t *) (pTrpdMem + (sizeof(CSL_UdmapTR15) *
                                         (1U + 1U)));

    /* Setup descriptor */
    CSL_udmapCppi5SetDescType(pTrpd, descType);

    CSL_udmapCppi5TrSetReload(pTrpd, UFALSE, 0U);
    /* Since each TRPD will always have only one TR */
    CSL_udmapCppi5SetPktLen(pTrpd, descType, 1U);
    /* Flow ID and Packet ID */
    CSL_udmapCppi5SetIds(pTrpd, descType, 0U, (uint32_t)UDMA_DEFAULT_FLOW_ID);
    CSL_udmapCppi5SetSrcTag(pTrpd, 0x0044);             /* Not used */
    CSL_udmapCppi5SetDstTag(pTrpd, 0x0416);             /* Not used */
    CSL_udmapCppi5TrSetEntryStride(pTrpd,
        CSL_UDMAP_CPPI5_TRPD_PKTINFO_RECSIZE_VAL_64B);
    CSL_udmapCppi5SetReturnPolicy(pTrpd,
        descType,
        /* Don't care for TR */
        CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
        CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
        CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
        cqRingNum);

    /* Initialize TR packet: Start */
    /* Setup TR */
    /* Bit fields::
       Bit No.->Info
       3:0->Type: 2D transfer
       7:6->Event Size: Event is only generated with the TR is complete
       9:8->Trigger0: No Trigger
       10:11->Trigger0 Type:
                    The second inner most loop (ICNT1) will be decremented by 1.
       13:12->Trigger1: No Trigger
       15:14->Trigger1 Type:
                    The second inner most loop (ICNT1) will be decremented by 1.
       23:16->Command ID: The Command ID for the TR
       31:24->Conf Flags: Configuration Specific Flags
       */
    pTr->flags = 0U;
    pTr->flags   |= CSL_FMK(UDMAP_TR_FLAGS_TYPE,
                            CSL_UDMAP_TR_FLAGS_TYPE_2D_DATA_MOVE);
    pTr->flags   |= CSL_FMK(UDMAP_TR_FLAGS_STATIC, UFALSE);
    /* EOP and EOL should be enabled as PSIL/SHIM expects these packets to
       mark the dimensions of the frame for transmission. */
    /* EOL is set to 'CSL_UDMAP_TR_FLAGS_EOL_ICNT0', this is due to ICNT0 is
       equal to the line size. EOL packet has to be sent out for each line of
       the frame. */
    pTr->flags   |= CSL_FMK(UDMAP_TR_FLAGS_EOL, CSL_UDMAP_TR_FLAGS_EOL_ICNT0);
    pTr->flags   |= CSL_FMK(UDMAP_TR_FLAGS_EOP, 1U);
    pTr->flags   |= CSL_FMK(UDMAP_TR_FLAGS_EVENT_SIZE,
                            CSL_UDMAP_TR_FLAGS_EVENT_SIZE_COMPLETION);
    pTr->flags   |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0,
                            CSL_UDMAP_TR_FLAGS_TRIGGER_NONE);
    pTr->flags   |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0_TYPE,
                            CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ALL);
    pTr->flags   |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1,
                            CSL_UDMAP_TR_FLAGS_TRIGGER_NONE);
    pTr->flags   |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1_TYPE,
                            CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ALL);
    pTr->flags   |= CSL_FMK(UDMAP_TR_FLAGS_CMD_ID, chIdx);
    pTr->flags   |= CSL_FMK(UDMAP_TR_FLAGS_SA_INDIRECT, 0U);
    pTr->flags   |= CSL_FMK(UDMAP_TR_FLAGS_DA_INDIRECT, 0U);
    pTr->icnt0    = CsitxDrv_getIcnt0(chCfg);
    pTr->icnt1    = (uint16_t)(chCfg->inFmt.height);
    pTr->dim1     = (int32_t)(chCfg->inFmt.pitch[0U]);
    /* Destination address does not needed to be programmed,
       this will overwritten in Queue */
    pTr->addr     = (uint64_t) srcBuf;

    /* Clear TR response memory */
    *pTrResp = 0xFFFFFFFFU;
    /* Initialize TR packet: End */
    /* Writeback cache */
    CsitxDrv_cacheWb(pTrpdMem,
                     CSITX_DRV_TRPD_SIZE_ALIGN);
    /* check if line size is greater than pitch provided, pitch should be
       equal to or more than line size in bytes */
    if ((pTr->icnt0 > pTr->dim1) || ((uint16_t)0U == pTr->icnt0))
    {
        retVal = FVID2_EFAIL;
    }

    return retVal;
}

void CsitxDrv_udmaCQEventCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData)
{
    int32_t retVal = UDMA_SOK;
    CsitxDrv_ChObj *chObj;
    CsitxDrv_QueObj *qObj;
    CsitxDrv_ChBufManObj *bmObj;
    CsitxDrv_InstObj *instObj;
    uint64_t pDesc = 0;
    uint32_t *pTrResp;
    uint32_t pTrRespVal, pTrRespStsTypeVal;
    uint32_t curQCnt, appCb = UFALSE, cookie, enterULP = UFALSE, chIdx;
    uint32_t chNum = 0U;
    uint64_t timeStamp;
    CsitxDrv_ChObj *submitChObj;

    chObj = (CsitxDrv_ChObj *) appData;
    instObj = chObj->instObj;
    GT_assert(CsitxTrace, (NULL != chObj));

    /* Take out completed TRPD from UDMA CQ Ring */
    if(UDMA_EVENT_TYPE_DMA_COMPLETION == eventType)
    {
        /* Disable HW interrupts here */
        cookie = HwiP_disable();
        bmObj = &chObj->bufManObj;
        /* Response received in completion queue, empty up full CQ Ring */
        while (UDMA_SOK == retVal)
        {
            retVal = Udma_ringDequeueRaw(
                         Udma_chGetCqRingHandle(&chObj->txChObj), &pDesc);
            if(UDMA_SOK == retVal)
            {
#if (CSITX_DRV_ENABLE_DEBUG == 1U)
                {
                    gTxTrOut[gTxTrOutCnt][0U] = (uint32_t)pDesc;
                    gTxTrOut[gTxTrOutCnt][1U] = chObj->chId;
                    if (NULL != gCsitxCommonObj.getTimeStamp)
                    {
                        gTxTrOut[gTxTrOutCnt][2U] =
                                        (uint32_t)gCsitxCommonObj.getTimeStamp(NULL);;
                    }
                    else
                    {
                        gTxTrOut[gTxTrOutCnt][2U] = 0U;
                    }
                    gTxTrOutCnt = (gTxTrOutCnt + 1U) % CSITX_DRV_TR_LOG_CNT;
                    CsitxDrv_cacheWb(&gTxTrOut[0U],
                                     sizeof(gTxTrOut));
                }
#endif
                if (NULL != gCsitxCommonObj.getTimeStamp)
                {
                    timeStamp = gCsitxCommonObj.getTimeStamp(NULL);
                }
                else
                {
                    timeStamp = 0U;
                }
                /* De-queue from curQ */
                qObj = (CsitxDrv_QueObj *) Fvid2Utils_dequeue(bmObj->curQ);
                GT_assert(CsitxTrace, (NULL != qObj));
                GT_assert(CsitxTrace,
                          (chObj->chId == qObj->chObj->chId));
                /* Check returned descriptor pointer */
                GT_assert(CsitxTrace, (pDesc == ((uint64_t) qObj->trpd)));
#if (CSITX_DRV_ENABLE_DEBUG == 1U)
                gCsitxCommonObj.frmChId[gCsitxCommonObj.frmCnt] =
                                                qObj->chObj->chId;
                gCsitxCommonObj.frmInstId[gCsitxCommonObj.frmCnt] =
                                        qObj->chObj->instObj->drvInstId;
                if (NULL != gCsitxCommonObj.getTimeStamp)
                {
                    gCsitxCommonObj.frmTS[gCsitxCommonObj.frmCnt] =
                                            (uint32_t)(timeStamp / 1000U);
                }
#endif
                GT_assert(CsitxTrace, (NULL != qObj->frm));
                qObj->frm->timeStamp64 = timeStamp;
                /* Invalidate cache */
                CsitxDrv_cacheInv(qObj->trpd,
                                  CSITX_DRV_TRPD_SIZE_ALIGN);
                /* de-queued TRPD was for frame submitted
                   by application */
                /* Check for truncated/elongated frame */
                pTrResp = (uint32_t *) ((uint8_t *)(qObj->trpd) +
                                       (sizeof(CSL_UdmapTR15) *
                                       (1U + 1U)));
#if (CSITX_DRV_ENABLE_DEBUG == 1U)
                gCsitxCommonObj.trResp[gCsitxCommonObj.frmCnt] =
                                            CSL_REG32_RD(pTrResp);
                gCsitxCommonObj.frmType[gCsitxCommonObj.frmCnt] = 1U;
#endif
                pTrRespVal = CSL_REG32_RD(pTrResp);
                pTrRespStsTypeVal = CSL_REG32_FEXT(pTrResp, UDMAP_TR_RESPONSE_STATUS_TYPE);
                /* check TR response */
                if (CSL_UDMAP_TR_RESPONSE_STATUS_COMPLETE != pTrRespStsTypeVal)
                {

                    if ((CSITX_DRV_CH_STATE_STOPPING == chObj->status) ||
                        (CSITX_DRV_CH_STATE_STOPPED  == chObj->status) ||
                        (CSL_UDMAP_TR_RESPONSE_STATUS_ABORTED_ERR == pTrRespStsTypeVal))
                    {
                        /* Aborted frame due to forced teardown at
                           Udma_chDisable() */
                        qObj->frm->status = FVID2_FRAME_STATUS_ABORTED;
                    }
                    else if (CSL_UDMAP_TR_RESPONSE_STATUS_SUBMISSION_ERR == pTrRespStsTypeVal)
                    { 
                        /* Submission error */
                        qObj->frm->status = FVID2_FRAME_STATUS_SUBMISSION_ERROR; 
                    }
                    else if (CSL_UDMAP_TR_RESPONSE_STATUS_TRANSFER_EXCEP_ERR == pTrRespStsTypeVal)
                    {
                        /* Transfer exception*/
                        if(CSL_UDMAP_TR_RESPONSE_STATUS_TRANSFER_EXCEP_SHORT_PACKET == 
                            ((pTrRespVal >> CSL_UDMAP_TR_RESPONSE_STATUS_INFO_SHIFT) &
                              CSL_UDMAP_TR_RESPONSE_STATUS_INFO_MASK))
                        {
                            /* Received frame is truncated */
                            qObj->frm->status = FVID2_FRAME_STATUS_TRUNCATED;
                        }
                        else if (CSL_UDMAP_TR_RESPONSE_STATUS_TRANSFER_EXCEP_LONG_PACKET == 
                                  ((pTrRespVal >> CSL_UDMAP_TR_RESPONSE_STATUS_INFO_SHIFT) &
                                    CSL_UDMAP_TR_RESPONSE_STATUS_INFO_MASK))
                        {
                            /* Received frame is elongated */
                            qObj->frm->status = FVID2_FRAME_STATUS_ELONGATED;
                        }
                        else
                        {
                            /* Do nothing */
                        }
                    }

                    else
                    {
                        /* Other UDMA TR errors */
                        qObj->frm->status = FVID2_FRAME_STATUS_ERROR;
                    }
#if (CSITX_DRV_ENABLE_DEBUG == 1U)
                    gCsitxCommonObj.frmStatus[gCsitxCommonObj.frmCnt] =
                                                qObj->frm->status;
#endif
                }
                else
                {
                    /* Received frame has no error */
                    qObj->frm->status = FVID2_FRAME_STATUS_COMPLETED;
#if (CSITX_DRV_ENABLE_DEBUG == 1U)
                    gCsitxCommonObj.frmStatus[gCsitxCommonObj.frmCnt] =
                                                    qObj->frm->status;
#endif
                }
                appCb = UTRUE;
                /* Return the frame to done queue */
                Fvid2Utils_queue(
                    instObj->bufManObj.doneQ,
                    &qObj->qElem,
                    qObj);
                if (CSITX_ENABLE_SERIAL_TRANS_ENABLE == 
                      gCsitxCommonObj.platData.serialMultiChanEnable)
                {
                    instObj->bufManObj.curFrmNum--;
                }
#if (CSITX_DRV_ENABLE_DEBUG == 1U)
                uint32_t oldFrmCnt;

                oldFrmCnt = gCsitxCommonObj.frmCnt;
                gCsitxCommonObj.frmCnt = (gCsitxCommonObj.frmCnt + 1U) %
                                          CSITX_DRV_LOG_CNT;
                if (oldFrmCnt > gCsitxCommonObj.frmCnt)
                {
                    gCsitxCommonObj.frmCntOverflow = UTRUE;
                }
#endif
            }
        }
        if (CSITX_ENABLE_SERIAL_TRANS_ENABLE == 
                gCsitxCommonObj.platData.serialMultiChanEnable)
        {
            /* Queue the next frame from waitQ to UDMA, this is needed to have only
               one frame active at a time. */
            /* Check if no frames are current being queue to UDMA channels.
               If not, submit a frame to UDMA channel */
            if (0U == instObj->bufManObj.curFrmNum)
            {
                /* Take a object from waiting Q and put in respective channel's
                   curQ (submit the same to UDMA) */
                qObj =
                 (CsitxDrv_QueObj *) Fvid2Utils_dequeue(instObj->bufManObj.waitQ);
                if (NULL == qObj)
                {
                    /* No Error since, no frames could be queued */
                }
                else
                {
                    /* Submit it to respective channel's curQ */
                    submitChObj = qObj->chObj;
                    retVal = Udma_ringQueueRaw(
                        Udma_chGetFqRingHandle(&submitChObj->txChObj),
                        (uint64_t)qObj->trpd);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(
                            CsitxTrace, GT_ERR,
                            "Queue: Submitting of TR failed\r\n");
                        /* Queue back the Object which was
                           de-queued earlier from waitQ */
                        Fvid2Utils_queue(instObj->bufManObj.waitQ, &qObj->qElem, qObj);
                        retVal = FVID2_EFAIL;
                    }
                    else
                    {
                        /* Add the queue object in driver's current queue */
                        Fvid2Utils_queue(submitChObj->bufManObj.curQ,
                                         &qObj->qElem,
                                         qObj);
                        instObj->bufManObj.curFrmNum++;
                    }
                }
            }
        }
        /** Enable HW interrupts here */
        HwiP_restore(cookie);

        if (CSITX_DRV_CH_STATE_RUNNING == chObj->status)
        {
            /* Application call back */
            if (UTRUE == appCb)
            {
                if(NULL != chObj->virtContext->fdmCbParams.fdmCbFxn)
                {
                    chObj->virtContext->fdmCbParams.fdmCbFxn(chObj->virtContext->fdmCbParams.fdmData);
                }
            }
        }

        /* Disable HW interrupts here */
        cookie = HwiP_disable();
        /* Go into ULP state only if there are no frames queued */
        if ((FVID2_SOK == retVal) &&
            (CSITX_CLK_MODE_NON_CONTINUOUS == instObj->dphyCfg.clkMode))
        {
            /* check waitQ first */
            if (0U == Fvid2Utils_getNumQElem(instObj->bufManObj.waitQ))
            {
                enterULP = UTRUE;
            }
            if ((0U != instObj->numColorbarCh) ||
                (0U != instObj->numLpbkCh))
            {
                enterULP = UTRUE;
            }
            /* check curQ for each channel */
            if (UFALSE == enterULP)
            {
                /* Set 'enterULP' to '1' and unset it if atleast a frame is
                   queued for any channel */
                enterULP = UTRUE;

                chNum = (instObj->numTxCh +
                         instObj->numColorbarCh +
                         instObj->numLpbkCh);
                for (chIdx = 0U ; chIdx < chNum ; chIdx++)
                {
                    if ((UTRUE == instObj->chObj[chIdx].psilThreadstatus) &&
                        (Fvid2Utils_getNumQElem(instObj->chObj[chIdx].bufManObj.curQ)
                                                                    > 0U))
                    {
                        enterULP = UFALSE;
                        break;
                    }
                }
            }

            if (UTRUE == enterULP)
            {
                retVal = CsitxDrv_dphytxSetLaneState(instObj,
                                                CSITX_DPHY_MODE_ULP);
            }
        }
        if (CSITX_DRV_CH_STATE_RUNNING == chObj->status)
        {
            if (UTRUE == instObj->createParams.instCfg.enableFrameRepeat)
            {
                /* Repeat Frame only if enabled */
                /* Check if one more frame is queued by application,
                   if not then re-submit last TR */
                curQCnt = Fvid2Utils_getNumQElem(bmObj->curQ);
                if (0U == curQCnt)
                {
                    /* Repeat last frame */
                }
            }
        }
        /** Enable HW interrupts here */
        HwiP_restore(cookie);
    }
    else
    {
        /* Spurious UDMA interrupt */
    }
}

#if defined (SOC_J721E)
void CsitxDrv_udmaTDCEventCb(Udma_EventHandle eventHandle,
                             uint32_t eventType,
                             void *appData)
{
    int32_t retVal = UDMA_SOK;
    CsitxDrv_ChObj *chObj;
    CSL_UdmapTdResponse tdResp;

    chObj = (CsitxDrv_ChObj *) appData;
    GT_assert(CsitxTrace, (NULL != chObj));
    /* Take out completed TRPD from UDMA TDC Ring */
    if(UDMA_EVENT_TYPE_TEARDOWN_PACKET == eventType)
    {
        /* Response received in Teardown completion queue */
        retVal = Udma_chDequeueTdResponse(&chObj->txChObj, &tdResp);
        if(UDMA_SOK != retVal)
        {
            /* [Error] No TD response after callback!! */
            GT_0trace(
                        CsitxTrace, GT_ERR,
                        "UDMA Tear Down Failed!!!\r\n");
        }
    }
    else
    {
        /* Spurious UDMA interrupt */
    }
}
#endif

void CsitxDrv_cacheWb(const void *addr, uint32_t size)
{
    uint32_t    isCacheCoherent = Udma_isCacheCoherent();

    if (UTRUE != isCacheCoherent)
    {
        CacheP_wb(addr, size);
    }

    return;
}

void CsitxDrv_cacheInv(const void * addr, uint32_t size)
{
    uint32_t    isCacheCoherent = Udma_isCacheCoherent();

    if (UTRUE != isCacheCoherent)
    {
        CacheP_Inv(addr, size);
    }

    return;
}

uint8_t* CsitxDrv_getTrpdMemAddr(uint32_t chIdx, uint32_t qCnt)
{
    return (&gCsitxUdmaTprdMem[chIdx][qCnt][0U]);
}
/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */
static uint16_t CsitxDrv_getIcnt0(const Csitx_ChCfg *chCfg)
{
    uint16_t icnt0     = 0U;
    uint32_t bpp       = 0U;
    uint32_t tempicnt0 = 0U;

    bpp = CsitxDrv_getStorageBpp(chCfg->outCsiDataType);
    GT_assert(CsitxTrace, (0U != bpp));
    /* check for RAW12 packed format */
    if ((FVID2_CSI2_DF_RAW12      == chCfg->outCsiDataType) &&
        (FVID2_CCSF_BITS12_PACKED == chCfg->inFmt.ccsFormat))
    {
        bpp = CSITX_BITS_PER_PIXEL_12_BITS;
    }

    /* icnt0 expressed in bits */
    tempicnt0 = (chCfg->inFmt.width * bpp);
    if ((FVID2_CSI2_DF_RAW12      == chCfg->outCsiDataType) &&
        (FVID2_CCSF_BITS12_PACKED == chCfg->inFmt.ccsFormat))
    {
        /* icnt0 currently in bits, rounding off to next word boundary */
        icnt0 = (tempicnt0 + 31U) / 32U;
        /* icnt0 in bytes */
        icnt0 = icnt0 * 4U;
    }
    else
    {
        /* icnt0 in bytes */
        icnt0 = tempicnt0 / 8U;
    }

    return icnt0;
}

static uint32_t CsitxDrv_dtIndex(const CsitxDrv_ChObj *chObj)
{
    uint32_t dtIndex;

    for (dtIndex = 0U ; dtIndex < (chObj->instObj->cslObj.dtInUse) ; dtIndex++)
    {
        if (chObj->chCfg->outCsiDataType ==
                                chObj->instObj->cslObj.dtCfg[dtIndex].dataType)
        {
            break;
        }
    }

    return dtIndex;
}
