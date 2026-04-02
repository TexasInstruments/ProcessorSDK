/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
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
 *  \file udma_ch.c
 *
 *  \brief File containing the UDMA driver channel related APIs.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/udma/src/udma_priv.h>

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

static void  Udma_chAssignRegOverlay(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle);
static void  Udma_chInitRegs(Udma_ChHandle chHandle);
static void  Udma_chPauseTxLocal(Udma_DrvHandle drvHandle, uint32_t txChNum, uint32_t chType);
static void  Udma_chUnpauseTxLocal(Udma_DrvHandle drvHandle, uint32_t txChNum, uint32_t chType);
static void  Udma_chPauseRxLocal(Udma_DrvHandle drvHandle, uint32_t rxChNum);
static void  Udma_chUnpauseRxLocal(Udma_DrvHandle drvHandle, uint32_t rxChNum);
static int32_t Udma_chCheckParams(Udma_DrvHandle drvHandle,
                                  uint32_t chType,
                                  const Udma_ChPrms *chPrms);
static void Udma_chSetPeerReg(Udma_DrvHandle drvHandle,
                              const Udma_ChPdmaPrms *pdmaPrms,
                              volatile uint32_t *PEER8,
                              volatile uint32_t *PEER1,
                              volatile uint32_t *PEER0);
static int32_t Udma_chAllocResource(Udma_ChHandle chHandle);
static int32_t Udma_chFreeResource(Udma_ChHandle chHandle);
static int32_t Udma_chPair(Udma_ChHandle chHandle);
static int32_t Udma_chUnpair(Udma_ChHandle chHandle);
static int32_t Udma_chEnableLocal(Udma_ChHandle chHandle);
static int32_t Udma_chDisableBlkCpyChan(Udma_ChHandle chHandle, uint32_t timeout);
static int32_t Udma_chDisableTxChan(Udma_ChHandle chHandle, uint32_t timeout);
static int32_t Udma_chDisableRxChan(Udma_ChHandle chHandle, uint32_t timeout);
#if (UDMA_NUM_UTC_INSTANCE > 0)
static int32_t Udma_chDisableExtChan(Udma_ChHandle chHandle, uint32_t timeout);
#endif
#if (UDMA_NUM_UTC_INSTANCE > 0)
static int32_t Udma_psilcfgSetRtEnable(Udma_DrvHandle drvHandle,
                                       uint32_t threadId,
                                       uint32_t bEnable);
static int32_t Udma_psilcfgSetEnable(Udma_DrvHandle drvHandle,
                                     uint32_t threadId,
                                     uint32_t bEnable);
static int32_t Udma_psilcfgWrite(Udma_DrvHandle drvHandle,
                                 uint32_t threadId,
                                 uint32_t regId,
                                 uint32_t data);
static int32_t Udma_psilcfgRead(Udma_DrvHandle drvHandle,
                                uint32_t threadId,
                                uint32_t regId,
                                uint32_t *pData);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Udma_chOpen(Udma_DrvHandle drvHandle,
                    Udma_ChHandle chHandle,
                    uint32_t chType,
                    const Udma_ChPrms *chPrms)
{
    int32_t     retVal = UDMA_SOK, tempRetVal;
    uint32_t    allocDone = UFALSE;

    /* Error check */
    if((NULL_PTR == drvHandle) || (NULL_PTR == chHandle) || (NULL_PTR == chPrms))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        if(UDMA_INIT_DONE != drvHandle->drvInitDone)
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chCheckParams(drvHandle, chType, chPrms);
    }

    if(UDMA_SOK == retVal)
    {
        /* Copy and init parameters */
        (void) memset(chHandle, 0, sizeof(struct Udma_ChObj));
        (void) memcpy(&chHandle->chPrms, chPrms, sizeof(Udma_ChPrms));
        chHandle->chType            = chType;
        chHandle->drvHandle         = drvHandle;
#if (UDMA_NUM_UTC_INSTANCE > 0)
        chHandle->utcInfo           = (const Udma_UtcInstInfo *) NULL_PTR;
#endif
        chHandle->txChNum           = UDMA_DMA_CH_INVALID;
        chHandle->rxChNum           = UDMA_DMA_CH_INVALID;
        chHandle->extChNum          = UDMA_DMA_CH_INVALID;
        chHandle->pdmaChNum         = UDMA_DMA_CH_INVALID;
        chHandle->peerThreadId      = UDMA_THREAD_ID_INVALID;
        chHandle->fqRing            = (Udma_RingHandle) NULL_PTR;
        chHandle->cqRing            = (Udma_RingHandle) NULL_PTR;
        chHandle->tdCqRing          = (Udma_RingHandle) NULL_PTR;
        UdmaChTxPrms_init(&chHandle->txPrms, chType);
        UdmaChRxPrms_init(&chHandle->rxPrms, chType);
        UdmaChUtcPrms_init(&chHandle->utcPrms);
        Udma_chInitRegs(chHandle);
#if (UDMA_NUM_UTC_INSTANCE > 0)
        chHandle->pDruNrtRegs       = (volatile CSL_DRU_CHNRTRegs_CHNRT *) NULL_PTR;
        chHandle->pDruRtRegs        = (volatile CSL_DRU_CHRTRegs_CHRT *) NULL_PTR;
#endif
        chHandle->chOesAllocDone    = UFALSE;
        chHandle->trigger           = CSL_UDMAP_TR_FLAGS_TRIGGER_NONE;

        if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
        {
#if (UDMA_NUM_UTC_INSTANCE > 0)
            /* Get UTC instance object pointer */
            chHandle->utcInfo = Udma_chGetUtcInst(drvHandle, chPrms->utcId);
            if(NULL_PTR == chHandle->utcInfo)
            {
                retVal = UDMA_EINVALID_PARAMS;
                Udma_printf(drvHandle, "[Error] Invalid UTC ID!!\n");
            }
#else
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle, "[Error] UTC Not supported!!!\n");
#endif
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Alloc UDMA channel/rings */
        retVal = Udma_chAllocResource(chHandle);
        if(UDMA_SOK == retVal)
        {
            allocDone = UTRUE;
        }
        else
        {
            Udma_printf(drvHandle, "[Error] Channel resource allocation failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Pair channels */
        retVal = Udma_chPair(chHandle);
        if(UDMA_SOK != retVal)
        {
            Udma_printf(drvHandle, "[Error] UDMA channel paring failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        chHandle->chInitDone        = UDMA_INIT_DONE;
    }
    else
    {
        /* Error. Free-up resource if allocated */
        if(UTRUE == allocDone)
        {
            tempRetVal = Udma_chFreeResource(chHandle);
            if(UDMA_SOK != tempRetVal)
            {
                Udma_printf(drvHandle, "[Error] Free resource failed!!!\n");
            }
        }
    }

    return (retVal);
}

int32_t Udma_chClose(Udma_ChHandle chHandle)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR == chHandle) || (UDMA_INIT_DONE != chHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }
    if(UDMA_SOK == retVal)
    {
        if(UTRUE == chHandle->chOesAllocDone)
        {
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle, "[Error] Channel OES not de-allocated!!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Unpair channels */
        retVal = Udma_chUnpair(chHandle);
        if(UDMA_SOK != retVal)
        {
            Udma_printf(drvHandle, "[Error] UDMA channel unparing failed!!\n");
        }

        /* Free-up UDMA channel/rings */
        retVal += Udma_chFreeResource(chHandle);
        if(UDMA_SOK != retVal)
        {
            Udma_printf(drvHandle, "[Error] Free resource failed!!!\n");
        }

        (void) memset(chHandle, 0, sizeof(*chHandle));
        chHandle->chInitDone = UDMA_DEINIT_DONE;
    }

    return (retVal);
}

int32_t Udma_chConfigTx(Udma_ChHandle chHandle, const Udma_ChTxPrms *txPrms)
{
    int32_t             retVal = UDMA_SOK;
    Udma_DrvHandle      drvHandle;
    struct tisci_msg_rm_udmap_tx_ch_cfg_req     rmUdmaTxReq;
    struct tisci_msg_rm_udmap_tx_ch_cfg_resp    rmUdmaTxResp;

    /* Error check */
    if((NULL_PTR        == chHandle) ||
       (UDMA_INIT_DONE  != chHandle->chInitDone) ||
       (UDMA_CH_FLAG_TX != (chHandle->chType & UDMA_CH_FLAG_TX)))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->txChNum));

        /* Copy params */
        rmUdmaTxReq.valid_params        = TISCI_MSG_VALUE_RM_UDMAP_CH_PAUSE_ON_ERR_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_ATYPE_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_CHAN_TYPE_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_FETCH_SIZE_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_CQ_QNUM_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_PRIORITY_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_QOS_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_ORDER_ID_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_SCHED_PRIORITY_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_TX_FILT_EINFO_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_TX_FILT_PSWORDS_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_TX_SUPR_TDPKT_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_TX_FDEPTH_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_BURST_SIZE_VALID |
                                          TISCI_MSG_VALUE_RM_UDMAP_CH_TX_CREDIT_COUNT_VALID;
        rmUdmaTxReq.nav_id              = drvHandle->devIdUdma;
        rmUdmaTxReq.index               = (uint16_t)chHandle->txChNum;
        rmUdmaTxReq.tx_pause_on_err     = txPrms->pauseOnError;
        rmUdmaTxReq.tx_filt_einfo       = txPrms->filterEinfo;
        rmUdmaTxReq.tx_filt_pswords     = txPrms->filterPsWords;
        rmUdmaTxReq.tx_atype            = txPrms->addrType;
        rmUdmaTxReq.tx_chan_type        = txPrms->chanType;
        rmUdmaTxReq.tx_fetch_size       = txPrms->fetchWordSize;
        rmUdmaTxReq.tx_priority         = txPrms->busPriority;
        rmUdmaTxReq.tx_qos              = txPrms->busQos;
        rmUdmaTxReq.tx_orderid          = txPrms->busOrderId;
        rmUdmaTxReq.fdepth              = txPrms->fifoDepth;
        rmUdmaTxReq.tx_burst_size       = txPrms->burstSize;
        rmUdmaTxReq.tx_sched_priority   = txPrms->dmaPriority;
        rmUdmaTxReq.tx_credit_count     = txPrms->txCredit;
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            if(UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY))
            {
                rmUdmaTxReq.valid_params    |= TISCI_MSG_VALUE_RM_UDMAP_EXTENDED_CH_TYPE_VALID;
                rmUdmaTxReq.extended_ch_type = UDMA_DMSC_EXTENDED_CH_TYPE_BCDMA_BLK_CPY;
            }
            else if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
            {
                rmUdmaTxReq.valid_params    |= TISCI_MSG_VALUE_RM_UDMAP_EXTENDED_CH_TYPE_VALID;
                rmUdmaTxReq.extended_ch_type = UDMA_DMSC_EXTENDED_CH_TYPE_BCDMA_SPLIT_TR_TX;
            }
            else
            {
                /* Do Nothing */
            }
        }
#endif
        if(NULL_PTR != chHandle->tdCqRing)
        {
            Udma_assert(drvHandle,
                (UDMA_RING_INVALID != chHandle->tdCqRing->ringNum));
            /* used for pass by value and teardown */
            rmUdmaTxReq.txcq_qnum       = chHandle->tdCqRing->ringNum;
            rmUdmaTxReq.tx_supr_tdpkt   = txPrms->supressTdCqPkt;
        }
        else
        {
            /* TD CQ not used */
            rmUdmaTxReq.txcq_qnum       = UDMA_RING_INVALID;
            rmUdmaTxReq.tx_supr_tdpkt   = TISCI_MSG_VALUE_RM_UDMAP_TX_CH_SUPPRESS_TD_ENABLED;
        }

        /* Config UDMAP TX channel */
        retVal = Sciclient_rmUdmapTxChCfg(
                     &rmUdmaTxReq, &rmUdmaTxResp, UDMA_SCICLIENT_TIMEOUT);
        if(CSL_PASS != retVal)
        {
            Udma_printf(drvHandle, "[Error] UDMA TX config failed!!!\n");
        }

        /* Copy the config */
        (void) memcpy(&chHandle->txPrms, txPrms, sizeof(chHandle->txPrms));
    }

    return (retVal);
}

int32_t Udma_chConfigRx(Udma_ChHandle chHandle, const Udma_ChRxPrms *rxPrms)
{
    int32_t             retVal = UDMA_SOK;
    Udma_DrvHandle      drvHandle;
    struct tisci_msg_rm_udmap_rx_ch_cfg_req     rmUdmaRxReq;
    struct tisci_msg_rm_udmap_rx_ch_cfg_resp    rmUdmaRxResp;
    Udma_FlowPrms       flowPrms;
    uint16_t            cqRing, fqRing;

    /* Error check */
    if((NULL_PTR == chHandle) ||
       (UDMA_INIT_DONE != chHandle->chInitDone) ||
       (UDMA_CH_FLAG_RX != (chHandle->chType & UDMA_CH_FLAG_RX)))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        if ((UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType) &&
            (UDMA_CH_FLAG_BLK_COPY      == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY)))
        {
            /* For BCDMA Block Copy, no need to configure Rx Channel.*/
        }
        else
        {
            /* Note: Block copy uses same RX channel as TX */
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->rxChNum));

            /* Copy params */
            rmUdmaRxReq.valid_params        = TISCI_MSG_VALUE_RM_UDMAP_CH_PAUSE_ON_ERR_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_ATYPE_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_CHAN_TYPE_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_FETCH_SIZE_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_CQ_QNUM_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_PRIORITY_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_QOS_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_ORDER_ID_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_SCHED_PRIORITY_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_RX_FLOWID_START_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_RX_FLOWID_CNT_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_RX_IGNORE_SHORT_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_RX_IGNORE_LONG_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_BURST_SIZE_VALID;
            rmUdmaRxReq.nav_id              = drvHandle->devIdUdma;
            rmUdmaRxReq.index               = (uint16_t)chHandle->rxChNum;
            rmUdmaRxReq.rx_pause_on_err     = rxPrms->pauseOnError;
            rmUdmaRxReq.rx_atype            = rxPrms->addrType;
            rmUdmaRxReq.rx_chan_type        = rxPrms->chanType;
            rmUdmaRxReq.rx_fetch_size       = rxPrms->fetchWordSize;
            rmUdmaRxReq.rx_priority         = rxPrms->busPriority;
            rmUdmaRxReq.rx_qos              = rxPrms->busQos;
            rmUdmaRxReq.rx_orderid          = rxPrms->busOrderId;
            rmUdmaRxReq.rx_sched_priority   = rxPrms->dmaPriority;
            rmUdmaRxReq.flowid_start        = rxPrms->flowIdFwRangeStart;
            rmUdmaRxReq.flowid_cnt          = rxPrms->flowIdFwRangeCnt;
            rmUdmaRxReq.rx_ignore_short     = rxPrms->ignoreShortPkts;
            rmUdmaRxReq.rx_ignore_long      = rxPrms->ignoreLongPkts;
            rmUdmaRxReq.rx_burst_size       = rxPrms->burstSize;
            if(NULL_PTR != chHandle->tdCqRing)
            {
                Udma_assert(drvHandle,
                    (UDMA_RING_INVALID != chHandle->tdCqRing->ringNum));
                /* used for pass by value and teardown */
                rmUdmaRxReq.rxcq_qnum          = chHandle->tdCqRing->ringNum;
            }
            else
            {
                /* TD CQ not used */
                rmUdmaRxReq.rxcq_qnum          = UDMA_RING_INVALID;
            }

            /* Config UDMAP RX channel */
            retVal = Sciclient_rmUdmapRxChCfg(
                         &rmUdmaRxReq, &rmUdmaRxResp, UDMA_SCICLIENT_TIMEOUT);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] UDMA RX config failed!!!\n");
            }

            /* Configure default flow for PDMA and other PSIL channels */
            if(((UDMA_CH_FLAG_PDMA == (chHandle->chType & UDMA_CH_FLAG_PDMA)) ||
                (UDMA_CH_FLAG_PSIL == (chHandle->chType & UDMA_CH_FLAG_PSIL))) &&
               (UTRUE == rxPrms->configDefaultFlow) && 
               (UDMA_INST_TYPE_LCDMA_BCDMA != drvHandle->instType))
            {
                UdmaFlowPrms_init(&flowPrms, chHandle->chType);
                flowPrms.psInfoPresent = rxPrms->flowPsInfoPresent;
                flowPrms.einfoPresent  = rxPrms->flowEInfoPresent;
                flowPrms.errorHandling = rxPrms->flowErrorHandling;
                flowPrms.sopOffset     = rxPrms->flowSopOffset;

                if(NULL_PTR == chHandle->cqRing)
                {
                    /* Ring not allocated */
                    cqRing = UDMA_RING_INVALID;
                }
                else
                {
                    cqRing = chHandle->cqRing->ringNum;
                    Udma_assert(drvHandle, (UDMA_RING_INVALID != cqRing));
                }
                if(NULL_PTR == chHandle->fqRing)
                {
                    /* Ring not allocated */
                    fqRing = UDMA_RING_INVALID;
                }
                else
                {
                    fqRing = chHandle->fqRing->ringNum;
                    Udma_assert(drvHandle, (UDMA_RING_INVALID != fqRing));
                }

                flowPrms.defaultRxCQ    = cqRing;
                /* Use the same free queue as default flow is not used in
                 * selecting different queues based on threshold */
                flowPrms.fdq0Sz0Qnum    = fqRing;
                flowPrms.fdq0Sz1Qnum    = fqRing;
                flowPrms.fdq0Sz2Qnum    = fqRing;
                flowPrms.fdq0Sz3Qnum    = fqRing;
                flowPrms.fdq1Qnum       = fqRing;
                flowPrms.fdq2Qnum       = fqRing;
                flowPrms.fdq3Qnum       = fqRing;

                /* Config default flow. Caller can overwite again if required */
                retVal = Udma_flowConfig(chHandle->defaultFlow, 0U, &flowPrms);
                if(UDMA_SOK != retVal)
                {
                    Udma_printf(drvHandle,
                        "[Error] UDMAP default RX flow config failed!!!\n");
                }
            }

            if(UDMA_SOK == retVal)
            {
                /* Copy the config */
                (void) memcpy(&chHandle->rxPrms, rxPrms, sizeof(chHandle->rxPrms));
            }
        }
    }
    return (retVal);
}

int32_t Udma_chConfigUtc(Udma_ChHandle chHandle, const Udma_ChUtcPrms *utcPrms)
{
#if (UDMA_NUM_UTC_INSTANCE > 0)
    int32_t                 retVal = UDMA_SOK;
    uint32_t                utcChNum;
    Udma_DrvHandle          drvHandle;
    struct tisci_msg_rm_udmap_tx_ch_cfg_req     rmUdmaTxReq;
    struct tisci_msg_rm_udmap_tx_ch_cfg_resp    rmUdmaTxResp;
    const Udma_UtcInstInfo *utcInfo;
    CSL_DruChConfig         druChCfg;

    /* Error check */
    if((NULL_PTR         == chHandle)             ||
       (UDMA_INIT_DONE   != chHandle->chInitDone) ||
       (UDMA_CH_FLAG_UTC != (chHandle->chType & UDMA_CH_FLAG_UTC)))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        utcInfo = chHandle->utcInfo;
        Udma_assert(drvHandle, (NULL_PTR != utcInfo));
        Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->extChNum));
        Udma_assert(drvHandle, chHandle->extChNum >= utcInfo->startCh);
        utcChNum = chHandle->extChNum - utcInfo->startCh;

        /* Direct TR mode doesn't need UDMAP channel programming */
        if(CSL_DRU_OWNER_UDMAC_TR == utcPrms->druOwner)
        {
            /* Copy params */
            rmUdmaTxReq.valid_params        = TISCI_MSG_VALUE_RM_UDMAP_CH_PAUSE_ON_ERR_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_ATYPE_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_CHAN_TYPE_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_FETCH_SIZE_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_CQ_QNUM_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_PRIORITY_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_QOS_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_ORDER_ID_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_SCHED_PRIORITY_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_TX_FILT_EINFO_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_TX_FILT_PSWORDS_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_TX_SUPR_TDPKT_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_TX_FDEPTH_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_BURST_SIZE_VALID |
                                              TISCI_MSG_VALUE_RM_UDMAP_CH_TX_CREDIT_COUNT_VALID;
            rmUdmaTxReq.nav_id              = drvHandle->devIdUdma;
            rmUdmaTxReq.index               = (uint16_t)(chHandle->extChNum + drvHandle->extChOffset);
            rmUdmaTxReq.tx_pause_on_err     = utcPrms->pauseOnError;
            rmUdmaTxReq.tx_filt_einfo       = TISCI_MSG_VALUE_RM_UDMAP_TX_CH_FILT_EINFO_DISABLED;
            rmUdmaTxReq.tx_filt_pswords     = TISCI_MSG_VALUE_RM_UDMAP_TX_CH_FILT_PSWORDS_DISABLED;
            rmUdmaTxReq.tx_atype            = utcPrms->addrType;
            rmUdmaTxReq.tx_chan_type        = utcPrms->chanType;
            rmUdmaTxReq.tx_fetch_size       = utcPrms->fetchWordSize;
            rmUdmaTxReq.tx_priority         = utcPrms->busPriority;
            rmUdmaTxReq.tx_qos              = utcPrms->busQos;
            rmUdmaTxReq.tx_orderid          = utcPrms->busOrderId;
            rmUdmaTxReq.fdepth              = 0U;   /* Not used for external ch */
            rmUdmaTxReq.tx_burst_size       = utcPrms->burstSize;
            rmUdmaTxReq.tx_sched_priority   = utcPrms->dmaPriority;
            rmUdmaTxReq.tx_credit_count     = utcInfo->txCredit;
            if(NULL_PTR != chHandle->tdCqRing)
            {
                Udma_assert(drvHandle,
                    (UDMA_RING_INVALID != chHandle->tdCqRing->ringNum));
                /* used for pass by value and teardown */
                rmUdmaTxReq.txcq_qnum       = chHandle->tdCqRing->ringNum;
                rmUdmaTxReq.tx_supr_tdpkt   = utcPrms->supressTdCqPkt;
            }
            else
            {
                /* TD CQ not used */
                rmUdmaTxReq.txcq_qnum       = UDMA_RING_INVALID;
                rmUdmaTxReq.tx_supr_tdpkt   = TISCI_MSG_VALUE_RM_UDMAP_TX_CH_SUPPRESS_TD_ENABLED;
            }

            /* Config UDMA UTC channel */
            retVal = Sciclient_rmUdmapTxChCfg(
                         &rmUdmaTxReq, &rmUdmaTxResp, UDMA_SCICLIENT_TIMEOUT);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] UDMA UTC config failed!!!\n");
            }
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Configure DRU */
        if(UDMA_UTC_TYPE_DRU == utcInfo->utcType)
        {
            Udma_assert(drvHandle, (NULL_PTR != utcInfo->druRegs));

            /* Disable the channel before any configuration */
            retVal = CSL_druChDisable(utcInfo->druRegs, utcChNum);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] DRU channel disable failed!!\n");
            }

            if(UDMA_SOK == retVal)
            {
                druChCfg.type       = 0U;   /* Not used */
                druChCfg.owner      = utcPrms->druOwner;
                druChCfg.pauseOnErr = utcPrms->pauseOnError;
#if defined (CSL_DRU_CHNRT_CFG_ATYPE_MASK)
                druChCfg.atype      = utcPrms->addrType;
#endif
                druChCfg.evtNum     = UDMA_EVENT_INVALID;
                druChCfg.queueId    = (uint64_t)utcPrms->druQueueId;
                retVal = CSL_druChConfig(utcInfo->druRegs, utcChNum, &druChCfg);
                if(CSL_PASS != retVal)
                {
                    Udma_printf(drvHandle,
                        "[Error] DRU channel config failed!!\n");
                }
            }
        }

        /* DRU in VHWA doesn't need enable/disable. Just config is sufficient */
        if(UDMA_UTC_TYPE_DRU_VHWA == utcInfo->utcType)
        {
            Udma_assert(drvHandle, (NULL_PTR != utcInfo->druRegs));

            druChCfg.type       = 0U;   /* Not used */
            druChCfg.owner      = CSL_DRU_OWNER_UDMAC_TR;   /* Always through UDMAC */
            druChCfg.pauseOnErr = utcPrms->pauseOnError;
            druChCfg.evtNum     = UDMA_EVENT_INVALID;
            druChCfg.queueId    = (uint64_t)utcPrms->druQueueId;
            retVal = CSL_druChConfig(utcInfo->druRegs, utcChNum, &druChCfg);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle,
                    "[Error] VHWA DRU channel config failed!!\n");
            }
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Copy the config */
        (void) memcpy(&chHandle->utcPrms, utcPrms, sizeof(chHandle->utcPrms));
    }
#else
    int32_t     retVal = UDMA_EFAIL;
#endif

    return (retVal);
}

int32_t Udma_chConfigPdma(Udma_ChHandle chHandle,
                          const Udma_ChPdmaPrms *pdmaPrms)
{
    int32_t         retVal = UDMA_SOK;
    volatile uint32_t  *PEER8 = NULL, *PEER0 = NULL, *PEER1 = NULL;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR == chHandle) ||
       (NULL_PTR == pdmaPrms) ||
       (UDMA_INIT_DONE    != chHandle->chInitDone) ||
       (UDMA_CH_FLAG_PDMA != (chHandle->chType & UDMA_CH_FLAG_PDMA)))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {

#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
            {
                Udma_assert(drvHandle, (NULL_PTR != chHandle->pTxRtRegs));

                PEER8 = &chHandle->pTxRtRegs->PEER8;
                PEER1 = &chHandle->pTxRtRegs->PEER1;
                PEER0 = &chHandle->pTxRtRegs->PEER0;
            }
            else
            {
                Udma_assert(drvHandle, (NULL_PTR != chHandle->pRxRtRegs));

                PEER8 = &chHandle->pRxRtRegs->PEER8;
                PEER1 = &chHandle->pRxRtRegs->PEER1;
                PEER0 = &chHandle->pRxRtRegs->PEER0;
            }
            Udma_chSetPeerReg(drvHandle, pdmaPrms, PEER8, PEER1, PEER0);
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
            {
                Udma_assert(drvHandle, (NULL_PTR != chHandle->pBcdmaTxRtRegs));

                PEER8 = &chHandle->pBcdmaTxRtRegs->PEER8;
                PEER1 = &chHandle->pBcdmaTxRtRegs->PEER1;
                PEER0 = &chHandle->pBcdmaTxRtRegs->PEER0;
            }
            else
            {
                Udma_assert(drvHandle, (NULL_PTR != chHandle->pBcdmaRxRtRegs));

                PEER8 = &chHandle->pBcdmaRxRtRegs->PEER8;
                PEER1 = &chHandle->pBcdmaRxRtRegs->PEER1;
                PEER0 = &chHandle->pBcdmaRxRtRegs->PEER0;
            }
            Udma_chSetPeerReg(drvHandle, pdmaPrms, PEER8, PEER1, PEER0);

        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
            {
                Udma_assert(drvHandle, (NULL_PTR != chHandle->pPktdmaTxRtRegs));

                PEER8 = &chHandle->pPktdmaTxRtRegs->PEER8;
                PEER1 = &chHandle->pPktdmaTxRtRegs->PEER1;
                PEER0 = &chHandle->pPktdmaTxRtRegs->PEER0;
            }
            else
            {
                Udma_assert(drvHandle, (NULL_PTR != chHandle->pPktdmaRxRtRegs));

                PEER8 = &chHandle->pPktdmaRxRtRegs->PEER8;
                PEER1 = &chHandle->pPktdmaRxRtRegs->PEER1;
                PEER0 = &chHandle->pPktdmaRxRtRegs->PEER0;
            }
            Udma_chSetPeerReg(drvHandle, pdmaPrms, PEER8, PEER1, PEER0);
        }
#endif
    }

    return (retVal);
}

int32_t Udma_chEnable(Udma_ChHandle chHandle)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR == chHandle) || (UDMA_INIT_DONE != chHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Enable channel based on channel type */
        retVal = Udma_chEnableLocal(chHandle);
        if(UDMA_SOK != retVal)
        {
            Udma_printf(drvHandle, "[Error] UDMA channel enable failed!!\n");
        }
    }

    return (retVal);
}

int32_t Udma_chDisablePolling(Udma_ChHandle chHandle, uint32_t timeout)
{
    int32_t             retVal = UDMA_SOK;
    Udma_DrvHandle      drvHandle;
    uint64_t            startTime = TimerP_getTimeInUsecs();
    uint64_t            timeoutUs = (uint64_t)timeout * 1000UL;
    bool                end_loop = BFALSE;
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    CSL_UdmapRT     udmapRtStatus;
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    CSL_BcdmaRT         bcdmaRtStatus;
#endif
#if (UDMA_NUM_UTC_INSTANCE > 0)
    const Udma_UtcInstInfo *utcInfo;
    uint32_t                utcChNum;
#endif

    /* Error check */
    if((NULL_PTR == chHandle) || (chHandle->chInitDone != UDMA_INIT_DONE))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (drvHandle->drvInitDone != UDMA_INIT_DONE))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chDisable(chHandle, 0U);

        while(UDMA_SOK != retVal)
        {
            if(0U == timeout)
            {
                end_loop = BTRUE;
            }
            else if ((TimerP_getTimeInUsecs() - startTime) >= timeoutUs)
            {
                end_loop = BTRUE;
                retVal = UDMA_ETIMEOUT;
            }
            else
            {
                /* loop until channel teardown complete */
                end_loop = BFALSE;
            }
#if (UDMA_NUM_UTC_INSTANCE > 0)
            if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
            {
                utcInfo = chHandle->utcInfo;
                utcChNum = chHandle->extChNum - utcInfo->startCh;
                if(CSL_druChIsTeardownComplete(utcInfo->druRegs, utcChNum) == TRUE)
                {
                    retVal = UDMA_SOK;
                    end_loop = BTRUE;
                }
            }
            else
#endif
            if((chHandle->chType & UDMA_CH_FLAG_RX) == UDMA_CH_FLAG_RX)
            {
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
                if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
                {
                    (void) CSL_bcdmaGetRxRT(&drvHandle->bcdmaRegs, chHandle->rxChNum, &bcdmaRtStatus);
                    if(FALSE == bcdmaRtStatus.enable)
                    {
                        /* Teardown complete */
                        retVal = UDMA_SOK;
                        end_loop = BTRUE;
                    }
                }
                else
#endif
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
                if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
                {
                    (void) CSL_udmapGetRxRT(&drvHandle->udmapRegs, chHandle->rxChNum, &udmapRtStatus);
                    if(UFALSE == udmapRtStatus.enable)
                    {
                        /* Teardown complete */
                        retVal = UDMA_SOK;
                        end_loop = BTRUE;
                    }
                }
                else
#endif
                {
                    /* Invalid Instance */
                    retVal = UDMA_EFAIL;
                }
            }
            else
            {
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
                if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
                {
                    (void) CSL_bcdmaGetTxRT(&drvHandle->bcdmaRegs, chHandle->txChNum, &bcdmaRtStatus);
                    if(FALSE == bcdmaRtStatus.enable)
                    {
                        /* Teardown complete */
                        retVal = UDMA_SOK;
                        end_loop = BTRUE;
                    }
                }
#endif
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
                if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
                {
                    (void) CSL_udmapGetTxRT(&drvHandle->udmapRegs, chHandle->txChNum, &udmapRtStatus);
                    if(UFALSE == udmapRtStatus.enable)
                    {
                        /* Teardown complete */
                        retVal = UDMA_SOK;
                        end_loop = BTRUE;
                    }
                }
#endif
            }

            if(BTRUE == end_loop)
            {
                break;
            }
        }
    }

    return retVal;
}

int32_t Udma_chDisable(Udma_ChHandle chHandle, uint32_t timeout)
{
    int32_t                 retVal = UDMA_SOK;
    Udma_DrvHandle          drvHandle;

    /* Error check */
    if((NULL_PTR == chHandle) || (UDMA_INIT_DONE != chHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Call disable sequence for respective modes */
        if(UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY))
        {
            retVal = Udma_chDisableBlkCpyChan(chHandle, timeout);
        }
#if (UDMA_NUM_UTC_INSTANCE > 0)
        else if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
        {
            retVal = Udma_chDisableExtChan(chHandle, timeout);
        }
#endif
        else
        {
            if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
            {
                retVal = Udma_chDisableTxChan(chHandle, timeout);
            }
            else
            {
                retVal = Udma_chDisableRxChan(chHandle, timeout);
            }
        }
    }

    return (retVal);
}

int32_t Udma_chPause(Udma_ChHandle chHandle)
{
    int32_t                 retVal = UDMA_SOK;
    Udma_DrvHandle          drvHandle;
#if (UDMA_NUM_UTC_INSTANCE > 0)
    uint32_t                utcChNum;
    const Udma_UtcInstInfo *utcInfo;
#endif

    /* Error check */
    if((NULL_PTR == chHandle) || (UDMA_INIT_DONE != chHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
        {
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->txChNum));
            Udma_chPauseTxLocal(drvHandle, chHandle->txChNum, chHandle->chType);
        }

        if(UDMA_CH_FLAG_RX == (chHandle->chType & UDMA_CH_FLAG_RX))
        {
            /* Note: Block copy uses same RX channel. So do for both TX/RX */
            if ((UDMA_INST_TYPE_LCDMA_BCDMA == chHandle->drvHandle->instType) &&
                (UDMA_CH_FLAG_BLK_COPY      == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY)))
            {
                /* In case of BCDMA Block Copy, No need to do for RX */
            }
            else
            {
                Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->rxChNum));
                Udma_chPauseRxLocal(drvHandle, chHandle->rxChNum);
            }
        }

#if (UDMA_NUM_UTC_INSTANCE > 0)
        if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
        {
            utcInfo = chHandle->utcInfo;
            Udma_assert(drvHandle, (NULL_PTR != utcInfo));

            /* Same TX channel CSL API is used for UTC as well - but need to
             * add the EXT channel offset */
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->extChNum));

            /* Direct TR mode doesn't need UDMAP channel programming */
            if(CSL_DRU_OWNER_UDMAC_TR == chHandle->utcPrms.druOwner)
            {
                Udma_chPauseTxLocal(drvHandle, chHandle->extChNum + drvHandle->extChOffset, chHandle->chType);
            }

            /* Pause DRU incase of direct TR mode */
            if((UDMA_UTC_TYPE_DRU       == utcInfo->utcType) &&
               (CSL_DRU_OWNER_DIRECT_TR == chHandle->utcPrms.druOwner))
            {
                Udma_assert(drvHandle, (NULL_PTR != utcInfo->druRegs));
                Udma_assert(drvHandle, chHandle->extChNum >= utcInfo->startCh);
                utcChNum = chHandle->extChNum - utcInfo->startCh;

                retVal = CSL_druChPause(utcInfo->druRegs, utcChNum);
                if(CSL_PASS != retVal)
                {
                    Udma_printf(drvHandle,
                        "[Error] DRU channel pause failed!!\n");
                }
            }
        }
#endif
    }

    return (retVal);
}

int32_t Udma_chResume(Udma_ChHandle chHandle)
{
    int32_t                 retVal = UDMA_SOK;
    Udma_DrvHandle          drvHandle;
#if (UDMA_NUM_UTC_INSTANCE > 0)
    uint32_t                utcChNum;
    const Udma_UtcInstInfo *utcInfo;
#endif

    /* Error check */
    if((NULL_PTR == chHandle) || (UDMA_INIT_DONE != chHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
        {
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->txChNum));
            Udma_chUnpauseTxLocal(drvHandle, chHandle->txChNum, chHandle->chType);
        }
        if(UDMA_CH_FLAG_RX == (chHandle->chType & UDMA_CH_FLAG_RX))
        {
            /* Note: Block copy uses same RX channel. So do for both TX/RX */
            if ((UDMA_INST_TYPE_LCDMA_BCDMA == chHandle->drvHandle->instType) &&
                (UDMA_CH_FLAG_BLK_COPY      == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY)))
            {
                /* In case of BCDMA Block Copy, No need to do for RX */
            }
            else
            {
                Udma_assert(drvHandle, chHandle->rxChNum != UDMA_DMA_CH_INVALID);
                Udma_chUnpauseRxLocal(drvHandle, chHandle->rxChNum);
            }
        }

#if (UDMA_NUM_UTC_INSTANCE > 0)
        if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
        {
            utcInfo = chHandle->utcInfo;
            Udma_assert(drvHandle, (NULL_PTR != utcInfo));

            /* Same TX channel CSL API is used for UTC as well - but need to
             * add the EXT channel offset */
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->extChNum));

            /* Direct TR mode doesn't need UDMAP channel programming */
            if(CSL_DRU_OWNER_UDMAC_TR == chHandle->utcPrms.druOwner)
            {
                Udma_chUnpauseTxLocal(drvHandle, chHandle->extChNum + drvHandle->extChOffset, chHandle->chType);
            }

            /* Resume DRU incase of direct TR mode */
            if((UDMA_UTC_TYPE_DRU       == utcInfo->utcType) &&
               (CSL_DRU_OWNER_DIRECT_TR == chHandle->utcPrms.druOwner))
            {
                Udma_assert(drvHandle, (NULL_PTR != utcInfo->druRegs));
                Udma_assert(drvHandle, chHandle->extChNum >= utcInfo->startCh);
                utcChNum = chHandle->extChNum - utcInfo->startCh;

                retVal = CSL_druChResume(utcInfo->druRegs, utcChNum);
                if(CSL_PASS != retVal)
                {
                    Udma_printf(drvHandle,
                        "[Error] DRU channel resume failed!!\n");
                }
            }
        }
#endif
    }

    return (retVal);
}

uint32_t Udma_chGetNum(Udma_ChHandle chHandle)
{
    int32_t         retVal = UDMA_SOK;
    uint32_t        chNum = UDMA_DMA_CH_INVALID;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR == chHandle) || (UDMA_INIT_DONE != chHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
        {
#if (UDMA_NUM_UTC_INSTANCE > 0)
            Udma_assert(drvHandle, (NULL_PTR != chHandle->utcInfo));
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->extChNum));
            Udma_assert(drvHandle, chHandle->extChNum >= chHandle->utcInfo->startCh);
            /* Provide the channel offset within a UTC */
            chNum = chHandle->extChNum - chHandle->utcInfo->startCh;
#endif
        }
        else
        {
            if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
            {
                Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->txChNum));
                chNum = chHandle->txChNum;
            }
            else
            {
                Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->rxChNum));
                chNum = chHandle->rxChNum;
            }
        }
    }

    return (chNum);
}

Udma_RingHandle Udma_chGetFqRingHandle(Udma_ChHandle chHandle)
{
    int32_t         retVal = UDMA_SOK;
    Udma_RingHandle fqRing = (Udma_RingHandle) NULL_PTR;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR == chHandle) || (UDMA_INIT_DONE != chHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        fqRing = chHandle->fqRing;
    }

    return (fqRing);
}

Udma_RingHandle Udma_chGetCqRingHandle(Udma_ChHandle chHandle)
{
    int32_t         retVal = UDMA_SOK;
    Udma_RingHandle cqRing = (Udma_RingHandle) NULL_PTR;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR == chHandle) || (UDMA_INIT_DONE != chHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        cqRing = chHandle->cqRing;
    }

    return (cqRing);
}

Udma_RingHandle Udma_chGetTdCqRingHandle(Udma_ChHandle chHandle)
{
    int32_t         retVal = UDMA_SOK;
    Udma_RingHandle tdCqRing = (Udma_RingHandle) NULL_PTR;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR == chHandle) || (UDMA_INIT_DONE != chHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        tdCqRing = chHandle->tdCqRing;
    }

    return (tdCqRing);
}

uint16_t Udma_chGetFqRingNum(Udma_ChHandle chHandle)
{
    uint16_t        ringNum = UDMA_RING_INVALID;
    Udma_RingHandle ringHandle;

    ringHandle = Udma_chGetFqRingHandle(chHandle);
    if(NULL_PTR != ringHandle)
    {
        ringNum = Udma_ringGetNum(ringHandle);
    }

    return (ringNum);
}

uint16_t Udma_chGetCqRingNum(Udma_ChHandle chHandle)
{
    uint16_t        ringNum = UDMA_RING_INVALID;
    Udma_RingHandle ringHandle;

    ringHandle = Udma_chGetCqRingHandle(chHandle);
    if(NULL_PTR != ringHandle)
    {
        ringNum = Udma_ringGetNum(ringHandle);
    }

    return (ringNum);
}

Udma_FlowHandle Udma_chGetDefaultFlowHandle(Udma_ChHandle chHandle)
{
    int32_t         retVal = UDMA_SOK;
    Udma_FlowHandle defaultFlow = (Udma_FlowHandle) NULL_PTR;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR == chHandle) || (UDMA_INIT_DONE != chHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        defaultFlow = chHandle->defaultFlow;
    }

    return (defaultFlow);
}

int32_t Udma_chDequeueTdResponse(Udma_ChHandle chHandle,
                                 CSL_UdmapTdResponse *tdResponse)
{
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    int32_t     retVal = UDMA_SOK, cslRetVal;
    uint64_t    response;

    if((NULL_PTR          != chHandle->tdCqRing) &&
       (UDMA_RING_INVALID != chHandle->tdCqRing->ringNum) &&
       (NULL_PTR          != tdResponse))
    {
        cslRetVal = CSL_ringaccPop64(
            &chHandle->drvHandle->raRegs,
            &chHandle->tdCqRing->cfg,
            &response,
            &Udma_ringaccMemOps);

        if(CSL_PASS != cslRetVal)
        {
            retVal = UDMA_ETIMEOUT;
        }
        else
        {
            CSL_udmapGetTdResponse(response, tdResponse);
        }
    }
    else
    {
        retVal = UDMA_EFAIL;
    }
#else
    int32_t         retVal = UDMA_EFAIL;
    Udma_printf(chHandle->drvHandle, "[Error] Teardown not supported!!!\n");
#endif

    return (retVal);
}

uint32_t Udma_chGetTriggerEvent(Udma_ChHandle chHandle,
                                uint32_t trigger)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;
    uint32_t        triggerEvent = UDMA_EVENT_INVALID;

    /* Error check */
    if((NULL_PTR == chHandle) ||
       (UDMA_INIT_DONE != chHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        if((CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL0 == trigger) ||
        (CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1 == trigger))
        {
            /* Global 0/1 triggers are interleaved - so multiply by 2 */
            if((UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY)) ||
            (UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX)))
            {
                /* For block copy return the TX channel trigger */
                triggerEvent = (chHandle->txChNum * 2U);
                if(CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1 == trigger)
                {
                    triggerEvent++; /* Global1 trigger is next to global0 */
                }
                /* Add the global offset */
                triggerEvent += drvHandle->trigGemOffset;
            }
            else if(UDMA_CH_FLAG_RX == (chHandle->chType & UDMA_CH_FLAG_RX))
            {
                /* RX trigger is after TX channel triggers
                * Note: There is no external channel triggers - hence not
                * using rxChOffset */
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
                if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
                {
                    triggerEvent  = (drvHandle->udmapRegs.txChanCnt * 2U);
                }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
                if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
                {
                    triggerEvent  = (drvHandle->bcdmaRegs.txChanCnt * 2U);
                }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
                if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
                {
                    triggerEvent  = (drvHandle->pktdmaRegs.txChanCnt * 2U);
                }
#endif
                triggerEvent += (chHandle->rxChNum * 2U);
                if(CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1 == trigger)
                {
                    triggerEvent++; /* Global1 trigger is next to global0 */
                }
                /* Add the global offset */
                triggerEvent += drvHandle->trigGemOffset;
            }
            else
            {
                /* Trigger not supported for external channel -
                * return no event - already set */
            }
        }
    }

    return (triggerEvent);
}

void *Udma_chGetSwTriggerRegister(Udma_ChHandle chHandle)
{
    int32_t                 retVal = UDMA_SOK;
    Udma_DrvHandle          drvHandle;
    void                   *pSwTriggerReg = NULL;
#if (UDMA_NUM_UTC_INSTANCE > 0)
    const Udma_UtcInstInfo *utcInfo;
#endif

    /* Error check */
    if((NULL_PTR == chHandle) ||
       (UDMA_INIT_DONE != chHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        if((UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY)) ||
           (UDMA_CH_FLAG_TX       == (chHandle->chType & UDMA_CH_FLAG_TX)))
        {
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
            if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
            {
                Udma_assert(drvHandle, chHandle->pTxRtRegs != NULL_PTR);
                pSwTriggerReg = (void *) &chHandle->pTxRtRegs->SWTRIG;
            }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
            if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
            {
                if(UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY))
                {
                    Udma_assert(drvHandle, (NULL_PTR != chHandle->pBcdmaBcRtRegs));
                    pSwTriggerReg = (void *) &chHandle->pBcdmaBcRtRegs->SWTRIG;
                }
                else if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
                {
                    Udma_assert(drvHandle, (NULL_PTR != chHandle->pBcdmaTxRtRegs));
                    pSwTriggerReg = (void *) &chHandle->pBcdmaTxRtRegs->SWTRIG;
                }
                else
                {
                    /* Do Nothing */
                }
            }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
            if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
            {
                Udma_printf(drvHandle,
                            "[Error] SW trigger not supported for PKTDMA TX channels!!!\n");
            }
#endif
        }
        else if(UDMA_CH_FLAG_RX == (chHandle->chType & UDMA_CH_FLAG_RX))
        {
            Udma_printf(drvHandle,
                        "[Error] SW trigger not supported for RX channels!!!\n");
        }
        else
        {
#if (UDMA_NUM_UTC_INSTANCE > 0)
            utcInfo = chHandle->utcInfo;
            Udma_assert(drvHandle, (NULL_PTR != utcInfo));
            if(UDMA_UTC_TYPE_DRU == utcInfo->utcType)
            {
                Udma_assert(drvHandle, (NULL_PTR != chHandle->pDruRtRegs));
                pSwTriggerReg = (void *) &chHandle->pDruRtRegs->CHRT_SWTRIG;
            }
            else
            {
                Udma_printf(drvHandle,
                            "[Error] SW trigger not supported for other UTCs !!!\n");
            }
#else
            Udma_printf(drvHandle, "[Error] UTC Not supported!!!\n");
#endif
        }
    }

    return (pSwTriggerReg);
}

int32_t Udma_chSetSwTrigger(Udma_ChHandle chHandle, uint32_t trigger)
{
    int32_t                 retVal = UDMA_SOK;
    Udma_DrvHandle          drvHandle;
    void                   *pSwTriggerReg = NULL;

    /* Error check */
    if((NULL_PTR       == chHandle) ||
       (UDMA_INIT_DONE != chHandle->chInitDone) ||
       (CSL_UDMAP_TR_FLAGS_TRIGGER_LOCAL_EVENT < trigger) ||
       (CSL_UDMAP_TR_FLAGS_TRIGGER_NONE       == trigger))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        pSwTriggerReg = Udma_chGetSwTriggerRegister(chHandle);
        if(NULL != pSwTriggerReg)
        {
            if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
            {
                /* UTC is 64-bit register */
                CSL_REG64_WR(pSwTriggerReg, ((uint64_t)1U << (trigger - 1U)));
            }
            else
            {
                CSL_REG32_WR(pSwTriggerReg, ((uint32_t)1U << (trigger - 1U)));
            }
        }
        else
        {
            retVal = UDMA_EFAIL;
        }
    }

    return (retVal);
}

int32_t Udma_chSetChaining(Udma_ChHandle triggerChHandle,
                           Udma_ChHandle chainedChHandle,
                           uint32_t trigger)
{
    int32_t                             retVal = UDMA_SOK;
    Udma_DrvHandle                      drvHandle;
    uint32_t                            triggerEvent;
    struct tisci_msg_rm_irq_set_req     rmIrqReq;
    struct tisci_msg_rm_irq_set_resp    rmIrqResp;

    /* Error check */
    if((NULL_PTR == triggerChHandle) || (UDMA_INIT_DONE != triggerChHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if((NULL_PTR == chainedChHandle) || (UDMA_INIT_DONE != chainedChHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = triggerChHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }
    if(UDMA_SOK == retVal)
    {
        if(UTRUE == triggerChHandle->chOesAllocDone)
        {
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle,
                "[Error] Trigger channel OES already allocated!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Get the global trigger event to set */
        triggerEvent =
            Udma_chGetTriggerEvent(chainedChHandle, trigger);
        if(UDMA_EVENT_INVALID == triggerEvent)
        {
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle, "[Error] UDMA invalid trigger mode!!\n");
       }
    }

    if(UDMA_SOK == retVal)
    {
        rmIrqReq.valid_params           = TISCI_MSG_VALUE_RM_GLOBAL_EVENT_VALID;
        rmIrqReq.src_id                 = drvHandle->srcIdTrIrq;
        rmIrqReq.global_event           = (uint16_t) triggerEvent;
        rmIrqReq.src_index              = 0U;
        rmIrqReq.dst_id                 = 0U;
        rmIrqReq.dst_host_irq           = 0U;
        rmIrqReq.ia_id                  = 0U;
        rmIrqReq.vint                   = 0U;
        rmIrqReq.vint_status_bit_index  = 0U;
        rmIrqReq.secondary_host         = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle, "UDMA chaining not supported for PKTDMA instance!!!\n");
        }
        else if(UDMA_CH_FLAG_BLK_COPY == (triggerChHandle->chType & UDMA_CH_FLAG_BLK_COPY))
        {
            Udma_assert(drvHandle,
                (UDMA_DMA_CH_INVALID != triggerChHandle->txChNum));
            rmIrqReq.src_index = (uint16_t)triggerChHandle->txChNum;
            rmIrqReq.src_index += drvHandle->blkCopyTrIrqOffset;
            retVal = Sciclient_rmIrqSet(
                         &rmIrqReq, &rmIrqResp, UDMA_SCICLIENT_TIMEOUT);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] RM Block Copy Channel chain config failed!!!\n");
            }
        }
        else if(UDMA_CH_FLAG_RX == (triggerChHandle->chType & UDMA_CH_FLAG_RX))
        {
            Udma_assert(drvHandle,
                (UDMA_DMA_CH_INVALID != triggerChHandle->rxChNum));
            rmIrqReq.src_index = (uint16_t)triggerChHandle->rxChNum;
            rmIrqReq.src_index += drvHandle->rxTrIrqOffset;
            retVal = Sciclient_rmIrqSet(
                         &rmIrqReq, &rmIrqResp, UDMA_SCICLIENT_TIMEOUT);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] RM RX Channel chain config failed!!!\n");
            }
        }
        else if(UDMA_CH_FLAG_TX == (triggerChHandle->chType & UDMA_CH_FLAG_TX))
        {
            Udma_assert(drvHandle,
                triggerChHandle->txChNum != UDMA_DMA_CH_INVALID);
            rmIrqReq.src_index = (uint16_t)triggerChHandle->txChNum;
            rmIrqReq.src_index += drvHandle->txTrIrqOffset;
            retVal = Sciclient_rmIrqSet(
                         &rmIrqReq, &rmIrqResp, UDMA_SCICLIENT_TIMEOUT);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] RM TX Channel chain config failed!!!\n");
            }
        }
        else
        {
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle,
                "[Error] UDMA chaining not supported for UTC channels!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Mark OES alloc flag */
        triggerChHandle->chOesAllocDone = UTRUE;
        triggerChHandle->trigger        = trigger;
    }

    return (retVal);
}

int32_t Udma_chBreakChaining(Udma_ChHandle triggerChHandle,
                             Udma_ChHandle chainedChHandle)
{
    int32_t                             retVal = UDMA_SOK;
    Udma_DrvHandle                      drvHandle;
    uint32_t                            triggerEvent;
    struct tisci_msg_rm_irq_release_req rmIrqReq;

    /* Error check */
    if((NULL_PTR == triggerChHandle) || (UDMA_INIT_DONE != triggerChHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if((NULL_PTR == chainedChHandle) || (UDMA_INIT_DONE != chainedChHandle->chInitDone))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = triggerChHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }
    if(UDMA_SOK == retVal)
    {
        if(UFALSE == triggerChHandle->chOesAllocDone)
        {
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle,
                "[Error] Trigger channel OES not allocated!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Get the global trigger event to set */
        triggerEvent =
            Udma_chGetTriggerEvent(chainedChHandle, triggerChHandle->trigger);
        if(UDMA_EVENT_INVALID == triggerEvent)
        {
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle, "[Error] UDMA invalid trigger mode!!\n");
       }
    }

    if(UDMA_SOK == retVal)
    {
        rmIrqReq.valid_params           = TISCI_MSG_VALUE_RM_GLOBAL_EVENT_VALID;
        rmIrqReq.src_id                 = drvHandle->srcIdTrIrq;
        rmIrqReq.src_index              = 0U;
        rmIrqReq.global_event           = (uint16_t)triggerEvent;
        rmIrqReq.dst_id                 = 0U;
        rmIrqReq.dst_host_irq           = 0U;
        rmIrqReq.ia_id                  = 0U;
        rmIrqReq.vint                   = 0U;
        rmIrqReq.vint_status_bit_index  = 0U;
        rmIrqReq.secondary_host         = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle, "UDMA chaining not supported for PKTDMA instance!!!\n");
        }
        else if(UDMA_CH_FLAG_BLK_COPY == (triggerChHandle->chType & UDMA_CH_FLAG_BLK_COPY))
        {
            Udma_assert(drvHandle,
                (UDMA_DMA_CH_INVALID != triggerChHandle->txChNum));
            rmIrqReq.src_index = (uint16_t)triggerChHandle->txChNum;
            rmIrqReq.src_index += drvHandle->blkCopyTrIrqOffset;
            retVal = Sciclient_rmIrqRelease(&rmIrqReq, UDMA_SCICLIENT_TIMEOUT);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] RM Block Copy Channel chain reset failed!!!\n");
            }
        }
        else if(UDMA_CH_FLAG_RX == (triggerChHandle->chType & UDMA_CH_FLAG_RX))
        {
            Udma_assert(drvHandle,
                (UDMA_DMA_CH_INVALID != triggerChHandle->rxChNum));
            rmIrqReq.src_index = (uint16_t)triggerChHandle->rxChNum;
            rmIrqReq.src_index += drvHandle->rxTrIrqOffset;
            retVal = Sciclient_rmIrqRelease(&rmIrqReq, UDMA_SCICLIENT_TIMEOUT);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] RM RX Channel chain reset failed!!!\n");
            }
        }
        else if(UDMA_CH_FLAG_TX == (triggerChHandle->chType & UDMA_CH_FLAG_TX))
        {
            Udma_assert(drvHandle,
                (UDMA_DMA_CH_INVALID != triggerChHandle->txChNum));
            rmIrqReq.src_index = (uint16_t)triggerChHandle->txChNum;
            rmIrqReq.src_index += drvHandle->txTrIrqOffset;
            retVal = Sciclient_rmIrqRelease(&rmIrqReq, UDMA_SCICLIENT_TIMEOUT);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] RM TX Channel chain reset failed!!!\n");
            }
        }
        else
        {
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle,
                "[Error] UDMA chaining not supported for UTC channels!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Mark OES alloc flag as free */
        triggerChHandle->chOesAllocDone = UFALSE;
    }

    return (retVal);
}

void UdmaChPrms_init(Udma_ChPrms *chPrms, uint32_t chType)
{
    if(NULL_PTR != chPrms)
    {
        chPrms->chNum       = UDMA_DMA_CH_ANY;
        chPrms->peerChNum   = UDMA_DMA_CH_INVALID;
        if((UDMA_CH_TYPE_TR_BLK_COPY == chType) ||
           (UDMA_CH_TYPE_UTC         == chType))
        {
            chPrms->peerChNum   = UDMA_DMA_CH_NA;
        }
        chPrms->utcId       = UDMA_UTC_ID_INVALID;
        chPrms->mappedChGrp = UDMA_MAPPED_GROUP_INVALID;
        chPrms->appData     = NULL_PTR;
        UdmaRingPrms_init(&chPrms->fqRingPrms);
        UdmaRingPrms_init(&chPrms->cqRingPrms);
        UdmaRingPrms_init(&chPrms->tdCqRingPrms);
        /* TD and TR response is always 8 byte irrespective of mode */
        chPrms->tdCqRingPrms.elemSize = UDMA_RING_ES_8BYTES;
    }

    return;
}

void UdmaChTxPrms_init(Udma_ChTxPrms *txPrms, uint32_t chType)
{
    if(NULL_PTR != txPrms)
    {
        txPrms->pauseOnError    = TISCI_MSG_VALUE_RM_UDMAP_CH_PAUSE_ON_ERROR_DISABLED;
        txPrms->filterEinfo     = TISCI_MSG_VALUE_RM_UDMAP_TX_CH_FILT_EINFO_DISABLED;
        txPrms->filterPsWords   = TISCI_MSG_VALUE_RM_UDMAP_TX_CH_FILT_PSWORDS_DISABLED;
        txPrms->addrType        = TISCI_MSG_VALUE_RM_UDMAP_CH_ATYPE_PHYS;
        txPrms->chanType        = TISCI_MSG_VALUE_RM_UDMAP_CH_TYPE_PACKET;
        if(UDMA_CH_FLAG_BLK_COPY == (chType & UDMA_CH_FLAG_BLK_COPY))
        {
            txPrms->chanType    = TISCI_MSG_VALUE_RM_UDMAP_CH_TYPE_3P_BLOCK_REF;
        }
        txPrms->fetchWordSize   = 16U;  /* sizeof(CSL_UdmapTR15) / sizeof(uint32_t) */
        txPrms->busPriority     = UDMA_DEFAULT_TX_CH_BUS_PRIORITY;
        txPrms->busQos          = UDMA_DEFAULT_TX_CH_BUS_QOS;
        txPrms->busOrderId      = UDMA_DEFAULT_TX_CH_BUS_ORDERID;
        txPrms->dmaPriority     = UDMA_DEFAULT_TX_CH_DMA_PRIORITY;
        txPrms->txCredit        = 0U;
        if(UDMA_CH_FLAG_UHC == (chType & UDMA_CH_FLAG_UHC))
        {
            txPrms->fifoDepth   = (uint16_t)UDMA_TX_UHC_CHANS_FDEPTH;
            txPrms->burstSize   = TISCI_MSG_VALUE_RM_UDMAP_CH_BURST_SIZE_256_BYTES;
        }
        else if(UDMA_CH_FLAG_HC == (chType & UDMA_CH_FLAG_HC))
        {
            txPrms->fifoDepth   = (uint16_t)UDMA_TX_HC_CHANS_FDEPTH;
            txPrms->burstSize   = TISCI_MSG_VALUE_RM_UDMAP_CH_BURST_SIZE_256_BYTES;
        }
        else
        {
            txPrms->fifoDepth   = (uint16_t)UDMA_TX_CHANS_FDEPTH;
            txPrms->burstSize   = TISCI_MSG_VALUE_RM_UDMAP_CH_BURST_SIZE_64_BYTES;
        }
        txPrms->supressTdCqPkt  = TISCI_MSG_VALUE_RM_UDMAP_TX_CH_SUPPRESS_TD_DISABLED;
    }

    return;
}

void UdmaChRxPrms_init(Udma_ChRxPrms *rxPrms, uint32_t chType)
{
    if(NULL_PTR != rxPrms)
    {
        rxPrms->pauseOnError        = TISCI_MSG_VALUE_RM_UDMAP_CH_PAUSE_ON_ERROR_DISABLED;
        rxPrms->addrType            = TISCI_MSG_VALUE_RM_UDMAP_CH_ATYPE_PHYS;
        rxPrms->chanType            = TISCI_MSG_VALUE_RM_UDMAP_CH_TYPE_PACKET;
        if(UDMA_CH_FLAG_BLK_COPY == (chType & UDMA_CH_FLAG_BLK_COPY))
        {
            rxPrms->chanType        = TISCI_MSG_VALUE_RM_UDMAP_CH_TYPE_3P_BLOCK_REF;
        }
        rxPrms->fetchWordSize       = 16U;  /* sizeof(CSL_UdmapTR15) / sizeof(uint32_t) */
        rxPrms->busPriority         = UDMA_DEFAULT_RX_CH_BUS_PRIORITY;
        rxPrms->busQos              = UDMA_DEFAULT_RX_CH_BUS_QOS;
        rxPrms->busOrderId          = UDMA_DEFAULT_RX_CH_BUS_ORDERID;
        rxPrms->dmaPriority         = UDMA_DEFAULT_RX_CH_DMA_PRIORITY;
        rxPrms->flowIdFwRangeStart  = 0U;       /* Reset value - to use default flow */
        rxPrms->flowIdFwRangeCnt    = 0U;       /* Reset value - to use default flow */
        rxPrms->flowEInfoPresent    = TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_EINFO_NOT_PRESENT;       /* Default no EINFO */
        rxPrms->flowPsInfoPresent   = TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_PSINFO_NOT_PRESENT;      /* Default no PSINFO */
        rxPrms->flowErrorHandling   = TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_ERR_RETRY;       /* Default Re-try descriptor allocation operation on starvation error */
        rxPrms->flowSopOffset       = 0U;      /* Default SOP offset is 0 */
        rxPrms->ignoreShortPkts     = TISCI_MSG_VALUE_RM_UDMAP_RX_CH_PACKET_EXCEPTION;
        rxPrms->ignoreLongPkts      = TISCI_MSG_VALUE_RM_UDMAP_RX_CH_PACKET_EXCEPTION;
        rxPrms->configDefaultFlow   = UTRUE;
        if(UDMA_CH_FLAG_UHC == (chType & UDMA_CH_FLAG_UHC))
        {
            rxPrms->burstSize   = TISCI_MSG_VALUE_RM_UDMAP_CH_BURST_SIZE_256_BYTES;
        }
        else if(UDMA_CH_FLAG_HC == (chType & UDMA_CH_FLAG_HC))
        {
            rxPrms->burstSize   = TISCI_MSG_VALUE_RM_UDMAP_CH_BURST_SIZE_256_BYTES;
        }
        else
        {
            rxPrms->burstSize   = TISCI_MSG_VALUE_RM_UDMAP_CH_BURST_SIZE_64_BYTES;
        }
    }

    return;
}

void UdmaChUtcPrms_init(Udma_ChUtcPrms *utcPrms)
{
    if(NULL_PTR != utcPrms)
    {
        utcPrms->pauseOnError   = (uint8_t)0U;
        utcPrms->addrType       = TISCI_MSG_VALUE_RM_UDMAP_CH_ATYPE_PHYS;
        utcPrms->chanType       = TISCI_MSG_VALUE_RM_UDMAP_CH_TYPE_3P_DMA_REF;
        utcPrms->fetchWordSize  = 16U;  /* sizeof(CSL_UdmapTR15) / sizeof(uint32_t) */
        utcPrms->busPriority    = UDMA_DEFAULT_UTC_CH_BUS_PRIORITY;
        utcPrms->busQos         = UDMA_DEFAULT_UTC_CH_BUS_QOS;
        utcPrms->busOrderId     = UDMA_DEFAULT_UTC_CH_BUS_ORDERID;
        utcPrms->dmaPriority    = UDMA_DEFAULT_UTC_CH_DMA_PRIORITY;
        utcPrms->burstSize      = TISCI_MSG_VALUE_RM_UDMAP_CH_BURST_SIZE_128_BYTES;
        utcPrms->supressTdCqPkt = TISCI_MSG_VALUE_RM_UDMAP_TX_CH_SUPPRESS_TD_DISABLED;
#if (UDMA_NUM_UTC_INSTANCE > 0)
        utcPrms->druOwner       = CSL_DRU_OWNER_UDMAC_TR;
        utcPrms->druQueueId     = UDMA_DEFAULT_UTC_DRU_QUEUE_ID;
#endif
    }

    return;
}

void UdmaChPdmaPrms_init(Udma_ChPdmaPrms *pdmaPrms)
{
    if(NULL_PTR != pdmaPrms)
    {
        pdmaPrms->elemSize  = UDMA_PDMA_ES_8BITS;
        pdmaPrms->elemCnt   = 0U;
        pdmaPrms->fifoCnt   = 0U;
    }

    return;
}

#if (UDMA_NUM_UTC_INSTANCE > 0)
const Udma_UtcInstInfo *Udma_chGetUtcInst(Udma_DrvHandle drvHandle,
                                          uint32_t utcId)

{
    const Udma_UtcInstInfo *utcInfo = (const Udma_UtcInstInfo *) NULL_PTR;
    uint32_t                i;

    for(i = 0U; i < UDMA_NUM_UTC_INSTANCE; i++)
    {
        if(drvHandle->utcInfo[i].utcId == utcId)
        {
            utcInfo = &drvHandle->utcInfo[i];
            break;
        }
    }

    return (utcInfo);
}
#endif

int32_t Udma_chGetStats(Udma_ChHandle chHandle, Udma_ChStats *chStats)
{
    int32_t            retVal = UDMA_SOK;
    Udma_DrvHandle     drvHandle;
    uint32_t           chNum;
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    CSL_UdmapChanStats udmapChanStats;
    CSL_UdmapChanDir   udmapChDir;
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    CSL_BcdmaChanStats  bcdmaChanStats;
    CSL_BcdmaChanDir    bcdmaChDir;
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    CSL_PktdmaChanStats pktdmaChanStats;
    CSL_PktdmaChanDir   pktdmaChDir;
#endif

    /* Error check */
    if ((NULL_PTR == chHandle)                   ||
        (UDMA_INIT_DONE != chHandle->chInitDone) ||
        (NULL == chStats))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = chHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {

#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            if(UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY))
            {
                chNum       = chHandle->txChNum;
                udmapChDir = CSL_UDMAP_CHAN_DIR_TX;
            }
            else if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
            {
                chNum       = chHandle->extChNum + drvHandle->extChOffset;
                udmapChDir = CSL_UDMAP_CHAN_DIR_TX;
            }
            else
            {
                if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
                {
                    chNum       = chHandle->txChNum;
                    udmapChDir = CSL_UDMAP_CHAN_DIR_TX;
                }
                else
                {
                    chNum       = chHandle->rxChNum;
                    udmapChDir = CSL_UDMAP_CHAN_DIR_RX;
                }
            }
            memset(&udmapChanStats, 0, sizeof(CSL_UdmapChanStats));
            CSL_udmapGetChanStats(&drvHandle->udmapRegs, chNum, udmapChDir, &udmapChanStats);
            (void)memcpy(chStats, &udmapChanStats, sizeof(Udma_ChStats));
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            if(UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY))
            {
                chNum       = chHandle->txChNum;
                bcdmaChDir = CSL_BCDMA_CHAN_DIR_TX;
            }
            else if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
            {
                chNum       = chHandle->extChNum + drvHandle->extChOffset;
                bcdmaChDir = CSL_BCDMA_CHAN_DIR_TX;
            }
            else
            {
                if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
                {
                    /* Add offset to chNum, so that BCDMA can identify it as Tx channel*/
                    chNum       = chHandle->txChNum + drvHandle->txChOffset;
                    bcdmaChDir = CSL_BCDMA_CHAN_DIR_TX;
                }
                else
                {
                    /* Add offset to chNum, so that BCDMA can identify it as Rx channel*/
                    chNum       = chHandle->rxChNum + drvHandle->rxChOffset;
                    bcdmaChDir = CSL_BCDMA_CHAN_DIR_RX;
                }
            }
            memset(&bcdmaChanStats, 0, sizeof(CSL_BcdmaChanStats));
            CSL_bcdmaGetChanStats(&drvHandle->bcdmaRegs, chNum, bcdmaChDir, &bcdmaChanStats);
            (void)memcpy(chStats, &bcdmaChanStats, sizeof(Udma_ChStats));
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            if(UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY))
            {
                chNum       = chHandle->txChNum;
                pktdmaChDir = CSL_PKTDMA_CHAN_DIR_TX;
            }
            else if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
            {
                chNum       = chHandle->extChNum + drvHandle->extChOffset;
                pktdmaChDir = CSL_PKTDMA_CHAN_DIR_TX;
            }
            else
            {
                if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
                {
                    chNum       = chHandle->txChNum;
                    pktdmaChDir = CSL_PKTDMA_CHAN_DIR_TX;
                }
                else
                {
                    chNum       = chHandle->rxChNum;
                    pktdmaChDir = CSL_PKTDMA_CHAN_DIR_RX;
                }
            }
            memset(&pktdmaChanStats, 0, sizeof(CSL_PktdmaChanStats));
            CSL_pktdmaGetChanStats(&drvHandle->pktdmaRegs, chNum, pktdmaChDir, &pktdmaChanStats);
            (void)memcpy(chStats, &pktdmaChanStats, sizeof(Udma_ChStats));
        }
#endif
    }

    return (retVal);
}

static int32_t Udma_chCheckParams(Udma_DrvHandle drvHandle,
                                  uint32_t chType,
                                  const Udma_ChPrms *chPrms)
{
    int32_t     retVal = UDMA_SOK;

    if(UDMA_CH_FLAG_UTC == (chType & UDMA_CH_FLAG_UTC))
    {
        if(UDMA_UTC_ID_INVALID == chPrms->utcId)
        {
            retVal = UDMA_EINVALID_PARAMS;
            Udma_printf(drvHandle, "[Error] Invalid UTC ID!!!\n");
        }
    }
    if(UDMA_CH_FLAG_PDMA == (chType & UDMA_CH_FLAG_PDMA))
    {
        if((UDMA_DMA_CH_INVALID == chPrms->peerChNum) ||
           (UDMA_DMA_CH_NA == chPrms->peerChNum))
        {
            retVal = UDMA_EINVALID_PARAMS;
            Udma_printf(drvHandle, "[Error] Invalid Peer Channel Number!!!\n");
        }
    }
    if(UDMA_CH_FLAG_PSIL == (chType & UDMA_CH_FLAG_PSIL))
    {
        if((UDMA_DMA_CH_INVALID == chPrms->peerChNum) ||
           (UDMA_DMA_CH_NA == chPrms->peerChNum))
        {
            retVal = UDMA_EINVALID_PARAMS;
            Udma_printf(drvHandle, "[Error] Invalid Peer Channel Number!!!\n");
        }
    }
    if(UDMA_CH_FLAG_MAPPED == (chType & UDMA_CH_FLAG_MAPPED))
    {
        if(UDMA_MAPPED_GROUP_INVALID == chPrms->mappedChGrp)
        {
            retVal = UDMA_EINVALID_PARAMS;
            Udma_printf(drvHandle, "[Error] Invalid Mapped Channel Group!!!\n");
        }
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
        if(UDMA_CH_FLAG_TX == (chType & UDMA_CH_FLAG_TX))
        {
            if(UDMA_NUM_MAPPED_TX_GROUP <= chPrms->mappedChGrp)
            {
                retVal = UDMA_EINVALID_PARAMS;
                Udma_printf(drvHandle, "[Error] Incorrect TX Mapped Channel Group!!!\n");
            }
        }
        if(UDMA_CH_FLAG_RX == (chType & UDMA_CH_FLAG_RX))
        {
            /* Since RX mapped group index follows TX,
             * also check whether the group index is less than no.of tx groups
             * when channel type is RX */
            if((UDMA_NUM_MAPPED_TX_GROUP > chPrms->mappedChGrp) ||
               ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) <= chPrms->mappedChGrp))
            {
                retVal = UDMA_EINVALID_PARAMS;
                Udma_printf(drvHandle, "[Error] Incorrect RX Mapped Channel Group!!!\n");
            }
        }
#endif
    }

    return (retVal);
}

static int32_t Udma_chAllocResource(Udma_ChHandle chHandle)
{
    int32_t                 retVal = UDMA_SOK, tempRetVal;
    Udma_DrvHandle          drvHandle;
    uint16_t                ringNum = UDMA_RING_INVALID;
#if (UDMA_NUM_UTC_INSTANCE > 0)
    uint32_t                utcChNum;
    const Udma_UtcInstInfo *utcInfo;
#endif
#if((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    Udma_MappedChRingAttributes  chAttr;
#endif

    drvHandle = chHandle->drvHandle;

    /* Allocate UDMAP channel */
    if(UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY))
    {
        if(UDMA_CH_FLAG_HC == (chHandle->chType & UDMA_CH_FLAG_HC))
        {
            chHandle->txChNum =
                Udma_rmAllocBlkCopyHcCh(chHandle->chPrms.chNum, drvHandle);
        }
        else if(UDMA_CH_FLAG_UHC == (chHandle->chType & UDMA_CH_FLAG_UHC))
        {
            chHandle->txChNum =
                Udma_rmAllocBlkCopyUhcCh(chHandle->chPrms.chNum, drvHandle);
        }
        else
        {
            chHandle->txChNum =
                Udma_rmAllocBlkCopyCh(chHandle->chPrms.chNum, drvHandle);
        }
        if(UDMA_DMA_CH_INVALID == chHandle->txChNum)
        {
            retVal = UDMA_EALLOC;
            Udma_printf(drvHandle, "[Error] RM Alloc Blkcpy Ch failed!!!\n");
        }
        else
        {
            if (UDMA_INST_TYPE_LCDMA_BCDMA == chHandle->drvHandle->instType)
            {
                /* For BCDMA Block Copy, rxChNum is not used.*/
                chHandle->rxChNum     = UDMA_DMA_CH_INVALID;
                /* For BCDMA Block Copy, PeerThreadID is not applicable (no pairing required). */
                chHandle->peerThreadId = UDMA_THREAD_ID_INVALID;
            }
            else
            {
                /* RX channel same as TX channel for block copy */
                chHandle->rxChNum = chHandle->txChNum;
                /* Add thread offset and or RX relative channel as the thread
                 * already has bit info (CSL_PSILCFG_DEST_THREAD_OFFSET)
                 * for destination thread */
                chHandle->peerThreadId =
                    chHandle->rxChNum + drvHandle->udmapDestThreadOffset;
            }

        }
    }
#if (UDMA_NUM_UTC_INSTANCE > 0)
    else if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
    {
        utcInfo = chHandle->utcInfo;
        Udma_assert(drvHandle, (NULL_PTR != utcInfo));
        /* Allocate external channel */
        chHandle->extChNum = Udma_rmAllocExtCh(
                                 chHandle->chPrms.chNum,
                                 drvHandle,
                                 utcInfo);
        if(UDMA_DMA_CH_INVALID == chHandle->extChNum)
        {
            retVal = UDMA_EALLOC;
            Udma_printf(drvHandle, "[Error] RM Alloc Ext Ch failed!!!\n");
        }
        else
        {
            Udma_assert(drvHandle, chHandle->extChNum >= utcInfo->startCh);
            utcChNum = chHandle->extChNum - utcInfo->startCh;
            chHandle->peerThreadId = utcChNum + utcInfo->startThreadId;
            if(NULL_PTR != utcInfo->druRegs)
            {
                Udma_assert(drvHandle, (utcChNum < 512U));    /* Array check */
                chHandle->pDruNrtRegs  = &utcInfo->druRegs->CHNRT[utcChNum];
                chHandle->pDruRtRegs   = &utcInfo->druRegs->CHRT[utcChNum];
            }
        }
    }
#endif
    else
    {
        /* Allocate UDMAP for PDMA channels */
        if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
        {
            if(UDMA_CH_FLAG_HC == (chHandle->chType & UDMA_CH_FLAG_HC))
            {
                chHandle->txChNum =
                    Udma_rmAllocTxHcCh(chHandle->chPrms.chNum, drvHandle);
            }
#if (UDMA_NUM_MAPPED_TX_GROUP > 0)
            else if(UDMA_CH_FLAG_MAPPED == (chHandle->chType & UDMA_CH_FLAG_MAPPED))
            {
                chHandle->txChNum =
                    Udma_rmAllocMappedTxCh(chHandle->chPrms.chNum, drvHandle, chHandle->chPrms.mappedChGrp);
            }
#endif
            else if(UDMA_CH_FLAG_UHC == (chHandle->chType & UDMA_CH_FLAG_UHC))
            {
                chHandle->txChNum =
                    Udma_rmAllocTxUhcCh(chHandle->chPrms.chNum, drvHandle);
            }
            else
            {
                chHandle->txChNum =
                    Udma_rmAllocTxCh(chHandle->chPrms.chNum, drvHandle);
            }
            if(UDMA_DMA_CH_INVALID == chHandle->txChNum)
            {
                retVal = UDMA_EALLOC;
                Udma_printf(drvHandle, "[Error] RM Alloc TX Ch failed!!!\n");
            }
        }
        else
        {
            if(UDMA_CH_FLAG_HC == (chHandle->chType & UDMA_CH_FLAG_HC))
            {
                chHandle->rxChNum =
                    Udma_rmAllocRxHcCh(chHandle->chPrms.chNum, drvHandle);
            }
#if (UDMA_NUM_MAPPED_RX_GROUP > 0)
            else if(UDMA_CH_FLAG_MAPPED == (chHandle->chType & UDMA_CH_FLAG_MAPPED))
            {
                /* For RX, Subtract the #UDMA_NUM_MAPPED_TX_GROUP from mappedChGrp, because the group id for TX and RX are continous */
                Udma_assert(drvHandle, (UDMA_NUM_MAPPED_TX_GROUP <= chHandle->chPrms.mappedChGrp));
                chHandle->rxChNum =
                    Udma_rmAllocMappedRxCh(chHandle->chPrms.chNum, drvHandle, chHandle->chPrms.mappedChGrp - UDMA_NUM_MAPPED_TX_GROUP);
            }
#endif
            else if(UDMA_CH_FLAG_UHC == (chHandle->chType & UDMA_CH_FLAG_UHC))
            {
                chHandle->rxChNum =
                    Udma_rmAllocRxUhcCh(chHandle->chPrms.chNum, drvHandle);
            }
            else
            {
                chHandle->rxChNum =
                    Udma_rmAllocRxCh(chHandle->chPrms.chNum, drvHandle);
            }
            if(UDMA_DMA_CH_INVALID == chHandle->rxChNum)
            {
                retVal = UDMA_EALLOC;
                Udma_printf(drvHandle, "[Error] RM Alloc RX Ch failed!!!\n");
            }
            else
            {
                /* Assign default flow */
                chHandle->defaultFlow               = &chHandle->defaultFlowObj;
                chHandle->defaultFlow->drvHandle    = drvHandle;
                chHandle->defaultFlow->flowStart    = chHandle->rxChNum;
                chHandle->defaultFlow->flowCnt      = 1U;
                chHandle->defaultFlow->flowInitDone = UDMA_INIT_DONE;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Allocate peer channel for PDMA channels */
            if(UDMA_CH_FLAG_PDMA == (chHandle->chType & UDMA_CH_FLAG_PDMA))
            {
                /* PDMA peer channel assignment */
                chHandle->pdmaChNum = chHandle->chPrms.peerChNum;
                /* Thread ID already added while getting PDMA channel num */
                chHandle->peerThreadId = chHandle->pdmaChNum;
            }

            if(UDMA_CH_FLAG_PSIL == (chHandle->chType & UDMA_CH_FLAG_PSIL))
            {
                chHandle->peerThreadId = chHandle->chPrms.peerChNum;
            }
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Free queue ring number is same as UDMAP channel number */
        if(NULL_PTR != chHandle->chPrms.fqRingPrms.ringMem)
        {
            /* Allocate only when memory is provided */
            if(UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY))
            {
                /* Same as TX channel incase of block copy */
                ringNum = (uint16_t)chHandle->txChNum;
            }
            else if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
            {
                ringNum = (uint16_t)(chHandle->extChNum + drvHandle->extChOffset);
            }
            else
            {
                if(UDMA_CH_FLAG_MAPPED == (chHandle->chType & UDMA_CH_FLAG_MAPPED))
                {
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
                    chHandle->chPrms.fqRingPrms.mappedRingGrp  = chHandle->chPrms.mappedChGrp;
                    if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
                    {
                        chHandle->chPrms.fqRingPrms.mappedChNum    = chHandle->txChNum;
                        /* Get default ringNum for the mapped TX channel */
                        retVal = Udma_getMappedChRingAttributes(drvHandle, chHandle->chPrms.mappedChGrp, chHandle->txChNum, &chAttr);
                        Udma_assert(drvHandle, (UDMA_SOK == retVal));
                        ringNum = chAttr.defaultRing;
                    }
                    else
                    {
                        chHandle->chPrms.fqRingPrms.mappedChNum    = chHandle->rxChNum;
                        /* Get default ringNum for the mapped RX channel */
                        retVal = Udma_getMappedChRingAttributes(drvHandle, chHandle->chPrms.mappedChGrp, chHandle->rxChNum, &chAttr);
                        Udma_assert(drvHandle, (UDMA_SOK == retVal));
                        ringNum = chAttr.defaultRing;
                    }
#endif
                }
                else if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
                {
                    /* For UDMAP, txChOffset is 0 */
                    ringNum = (uint16_t)(chHandle->txChNum + drvHandle->txChOffset);
                }
                else
                {
                    ringNum = (uint16_t)(chHandle->rxChNum + drvHandle->rxChOffset);
                }
            }

            chHandle->fqRing = &chHandle->fqRingObj;
            retVal = Udma_ringAlloc(
                         drvHandle,
                         chHandle->fqRing,
                         ringNum,
                         &chHandle->chPrms.fqRingPrms);
            if(UDMA_SOK != retVal)
            {
                chHandle->fqRing = (Udma_RingHandle) NULL_PTR;
                Udma_printf(drvHandle, "[Error] FQ ring alloc failed!!!\n");
            }
            else if((UDMA_CH_FLAG_MAPPED == (chHandle->chType & UDMA_CH_FLAG_MAPPED)) &&
                    (UDMA_CH_FLAG_RX     == (chHandle->chType & UDMA_CH_FLAG_RX)))
            {
                /* Assign the default flow start id as the allocated default ring num(without offset) for mapped RX channels.
                 * This is because the default flow start idx is not equal to rxChNum,
                 * since there may be 1 to many mapping between RX Channels and dedicated flows */
                Udma_assert(drvHandle, chHandle->fqRing->ringNum >= drvHandle->rxChOffset);
                chHandle->defaultFlow->flowStart    = chHandle->fqRing->ringNum - drvHandle->rxChOffset;
            }
            else
            {
                /* Do Nothing*/
            }
        }
    }

    if(UDMA_SOK == retVal)
    {
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
            /* Allocate completion ring only when memory is provided */
            if(NULL_PTR != chHandle->chPrms.cqRingPrms.ringMem)
            {
                chHandle->cqRing = &chHandle->cqRingObj;
                retVal = Udma_ringAlloc(
                            drvHandle,
                            chHandle->cqRing,
                            UDMA_RING_ANY,
                            &chHandle->chPrms.cqRingPrms);
                if(UDMA_SOK != retVal)
                {
                    chHandle->cqRing = (Udma_RingHandle) NULL_PTR;
                    Udma_printf(drvHandle, "[Error] CQ ring alloc failed!!!\n");
                }
            }
#endif
        }
        else
        {
            /* In devices like AM64x, there is no seperate completion queue. */
            chHandle->cqRing = &chHandle->fqRingObj;
        }
    }

    if(UDMA_SOK == retVal)
    {
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
            /* Allocate teardown completion ring only when memory is provided */
            if(NULL_PTR != chHandle->chPrms.tdCqRingPrms.ringMem)
            {
                chHandle->tdCqRing = &chHandle->tdCqRingObj;
                retVal = Udma_ringAlloc(
                            drvHandle,
                            chHandle->tdCqRing,
                            UDMA_RING_ANY,
                            &chHandle->chPrms.tdCqRingPrms);
                if(UDMA_SOK != retVal)
                {
                    chHandle->tdCqRing = (Udma_RingHandle) NULL_PTR;
                    Udma_printf(drvHandle, "[Error] TD CQ ring alloc failed!!!\n");
                }
            }
#endif
        }
        else
        {
            /* In devices like AM64x, teardown is not supported.*/
            chHandle->tdCqRing = (Udma_RingHandle) NULL_PTR;
        }
    }

    if(UDMA_SOK != retVal)
    {
        tempRetVal = Udma_chFreeResource(chHandle);
        if(UDMA_SOK != tempRetVal)
        {
            Udma_printf(drvHandle, "[Error] Free resource failed!!!\n");
        }
    }
    else
    {
        /* Assign overlay pointers */
        Udma_chAssignRegOverlay(drvHandle, chHandle);
    }

    return (retVal);
}

static int32_t Udma_chFreeResource(Udma_ChHandle chHandle)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;

    drvHandle = chHandle->drvHandle;
    if(UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY))
    {
        if(UDMA_DMA_CH_INVALID != chHandle->txChNum)
        {
            /* TX channel free */
            if(UDMA_CH_FLAG_HC == (chHandle->chType & UDMA_CH_FLAG_HC))
            {
                Udma_rmFreeBlkCopyHcCh(chHandle->txChNum, drvHandle);
            }
            else if(UDMA_CH_FLAG_UHC == (chHandle->chType & UDMA_CH_FLAG_UHC))
            {
                Udma_rmFreeBlkCopyUhcCh(chHandle->txChNum, drvHandle);
            }
            else
            {
                Udma_rmFreeBlkCopyCh(chHandle->txChNum, drvHandle);
            }
            chHandle->txChNum = UDMA_DMA_CH_INVALID;
            chHandle->rxChNum = UDMA_DMA_CH_INVALID;
        }
    }
    else
    {
        if(UDMA_DMA_CH_INVALID != chHandle->txChNum)
        {
            /* TX channel free */
            if(UDMA_CH_FLAG_HC == (chHandle->chType & UDMA_CH_FLAG_HC))
            {
                Udma_rmFreeTxHcCh(chHandle->txChNum, drvHandle);
            }
#if (UDMA_NUM_MAPPED_TX_GROUP > 0)
            else if(UDMA_CH_FLAG_MAPPED == (chHandle->chType & UDMA_CH_FLAG_MAPPED))
            {
                Udma_rmFreeMappedTxCh(chHandle->txChNum, drvHandle, chHandle->chPrms.mappedChGrp);
            }
#endif
            else if(UDMA_CH_FLAG_UHC == (chHandle->chType & UDMA_CH_FLAG_UHC))
            {
                Udma_rmFreeTxUhcCh(chHandle->txChNum, drvHandle);
            }
            else
            {
                Udma_rmFreeTxCh(chHandle->txChNum, drvHandle);
            }
            chHandle->txChNum = UDMA_DMA_CH_INVALID;
        }
        if(UDMA_DMA_CH_INVALID != chHandle->rxChNum)
        {
            /* RX channel free */
            if(UDMA_CH_FLAG_HC == (chHandle->chType & UDMA_CH_FLAG_HC))
            {
                Udma_rmFreeRxHcCh(chHandle->rxChNum, drvHandle);
            }
#if (UDMA_NUM_MAPPED_RX_GROUP > 0)
            else if(UDMA_CH_FLAG_MAPPED == (chHandle->chType & UDMA_CH_FLAG_MAPPED))
            {
                Udma_assert(drvHandle, (UDMA_NUM_MAPPED_TX_GROUP <= chHandle->chPrms.mappedChGrp));
                Udma_rmFreeMappedRxCh(chHandle->rxChNum, drvHandle, chHandle->chPrms.mappedChGrp - UDMA_NUM_MAPPED_TX_GROUP);
            }
#endif
            else if(UDMA_CH_FLAG_UHC == (chHandle->chType & UDMA_CH_FLAG_UHC))
            {
                Udma_rmFreeRxUhcCh(chHandle->rxChNum, drvHandle);
            }
            else
            {
                Udma_rmFreeRxCh(chHandle->rxChNum, drvHandle);
            }
            chHandle->rxChNum = UDMA_DMA_CH_INVALID;
        }

        chHandle->defaultFlowObj.drvHandle    = (Udma_DrvHandle) NULL_PTR;
        chHandle->defaultFlowObj.flowStart    = UDMA_FLOW_INVALID;
        chHandle->defaultFlowObj.flowCnt      = 0U;
        chHandle->defaultFlowObj.flowInitDone = UDMA_DEINIT_DONE;
        chHandle->defaultFlow                 = (Udma_FlowHandle) NULL_PTR;
    }
#if (UDMA_NUM_UTC_INSTANCE > 0)
    if(UDMA_DMA_CH_INVALID != chHandle->extChNum)
    {
        /* External channel free */
        Udma_rmFreeExtCh(chHandle->extChNum, drvHandle, chHandle->utcInfo);
        chHandle->extChNum = UDMA_DMA_CH_INVALID;
    }
#endif
    chHandle->pdmaChNum = UDMA_DMA_CH_INVALID;
    chHandle->peerThreadId = UDMA_THREAD_ID_INVALID;

    if(NULL_PTR != chHandle->fqRing)
    {
        retVal += Udma_ringFree(chHandle->fqRing);
        if(UDMA_SOK != retVal)
        {
            Udma_printf(drvHandle, "[Error] RM Free FQ ring failed!!!\n");
        }
        chHandle->fqRing = (Udma_RingHandle) NULL_PTR;
    }
    
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
        if(NULL_PTR != chHandle->cqRing)
        {
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
            retVal += Udma_ringFree(chHandle->cqRing);
            if(UDMA_SOK != retVal)
            {
                Udma_printf(drvHandle, "[Error] RM Free CQ ring failed!!!\n");
            }
#endif
            chHandle->cqRing = (Udma_RingHandle) NULL_PTR;
        }

        if(NULL_PTR != chHandle->tdCqRing)
        {
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
            retVal += Udma_ringFree(chHandle->tdCqRing);
            if(UDMA_SOK != retVal)
            {
                Udma_printf(drvHandle, "[Error] RM Free TDCQ ring failed!!!\n");
            }
#endif
            chHandle->tdCqRing = (Udma_RingHandle) NULL_PTR;
        }
    }


    return (retVal);
}

static int32_t Udma_chPair(Udma_ChHandle chHandle)
{
    int32_t                 retVal = UDMA_SOK;
    Udma_DrvHandle          drvHandle;
    struct tisci_msg_rm_psil_pair_req rmPairReq;

    drvHandle = chHandle->drvHandle;

    if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
    {
        /* For UTC, pairing not required. Enable done as part of enable API */
    }
    else if ((UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType) &&
             (UDMA_CH_FLAG_BLK_COPY      == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY)))
    {
        /* For BCDMA Block Copy, pairing not required.*/
    }
    else
    {
        rmPairReq.nav_id = drvHandle->devIdPsil;
        /* Do TX check first so that TX becomes source thread for block copy */
        if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
        {
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->txChNum));

            /* For TX, UDMAP channel is source */
            rmPairReq.src_thread = chHandle->txChNum + drvHandle->udmapSrcThreadOffset;
            rmPairReq.dst_thread = chHandle->peerThreadId;
        }
        else    /* RX channel */
        {
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->rxChNum));

            /* For RX, UDMAP channel is destination */
            rmPairReq.src_thread = chHandle->peerThreadId;
            rmPairReq.dst_thread = chHandle->rxChNum + drvHandle->udmapDestThreadOffset;
        }

        /* Pair source thread with destination thread */
        retVal = Sciclient_rmPsilPair(&rmPairReq, UDMA_SCICLIENT_TIMEOUT);
        if(CSL_PASS != retVal)
        {
            Udma_printf(drvHandle, "[Error] RM PSI Pairing failed!!!\n");
        }
    }

    return (retVal);
}

static int32_t Udma_chUnpair(Udma_ChHandle chHandle)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;
    struct tisci_msg_rm_psil_unpair_req rmUnpairReq;

    drvHandle = chHandle->drvHandle;

    if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
    {
#if (UDMA_NUM_UTC_INSTANCE > 0)
        uint32_t        dstThreadId;
        if(CSL_DRU_OWNER_UDMAC_TR == chHandle->utcPrms.druOwner)
        {
            /* For UTC, destination thread disable should be done */
            dstThreadId = chHandle->peerThreadId;
            retVal = Udma_psilcfgSetRtEnable(drvHandle, dstThreadId, UFALSE);
            if(UDMA_SOK != retVal)
            {
                Udma_printf(drvHandle,
                    "[Error] PSI UTC destination thread RT disable failed!!!\n");
            }
            retVal += Udma_psilcfgSetEnable(drvHandle, dstThreadId, UFALSE);
            if(UDMA_SOK != retVal)
            {
                Udma_printf(drvHandle,
                    "[Error] PSI UTC destination thread disable failed!!!\n");
            }
        }
#else
        retVal = UDMA_EFAIL;
        Udma_printf(drvHandle, "[Error] UTC Not supported!!!\n");
#endif
    }
    else if ((UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType) &&
             (UDMA_CH_FLAG_BLK_COPY      == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY)))
    {
        /* For BCDMA Block Copy, un-pairing / thread disbale not required.*/
    }
    else
    {
        rmUnpairReq.nav_id = drvHandle->devIdPsil;
        /* Do TX check first so that TX becomes source thread for block copy */
        if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
        {
            Udma_assert(drvHandle, UDMA_DMA_CH_INVALID != chHandle->txChNum);

            /* For TX, UDMAP channel is source */
            rmUnpairReq.src_thread = chHandle->txChNum + drvHandle->udmapSrcThreadOffset;
            rmUnpairReq.dst_thread = chHandle->peerThreadId;
        }
        else    /* RX channel */
        {
            Udma_assert(drvHandle, UDMA_DMA_CH_INVALID != chHandle->rxChNum);

            /* For RX, UDMAP channel is destination */
            rmUnpairReq.src_thread = chHandle->peerThreadId;
            rmUnpairReq.dst_thread = chHandle->rxChNum + drvHandle->udmapDestThreadOffset;
        }

        /* Unpair source thread with destination thread */
        retVal = Sciclient_rmPsilUnpair(&rmUnpairReq, UDMA_SCICLIENT_TIMEOUT);
        if(CSL_PASS != retVal)
        {
            Udma_printf(drvHandle, "[Error] RM PSI Un-Pairing failed!!!\n");
        }
    }

    return (retVal);
}

static int32_t Udma_chEnableLocal(Udma_ChHandle chHandle)
{
    int32_t                 retVal = UDMA_SOK;
    uint32_t                regVal;
    Udma_DrvHandle          drvHandle;
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    CSL_UdmapRT             udmapRtEnable = {0U};
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    CSL_BcdmaRT             bcdmaRtEnable = {0U};
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    CSL_PktdmaRT            pktdmaRtEnable = {0U};
#endif
#if (UDMA_NUM_UTC_INSTANCE > 0)
    uint32_t                utcChNum;
    uint32_t                srcThreadId;
    const Udma_UtcInstInfo *utcInfo;
#endif

    drvHandle = chHandle->drvHandle;



#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
       /* Set only enable and clear all other flags which might be set from
        * previous run */
        udmapRtEnable.enable         = UTRUE;
        udmapRtEnable.teardown       = UFALSE;
        udmapRtEnable.forcedTeardown = UFALSE;
        udmapRtEnable.pause          = UFALSE;
        udmapRtEnable.error          = UFALSE;

        if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
        {
            Udma_assert(drvHandle, (NULL_PTR != chHandle->pTxRtRegs));
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->txChNum));

            regVal = CSL_REG32_RD(&chHandle->pTxRtRegs->PEER8);
            CSL_FINS(regVal, PSILCFG_REG_RT_ENABLE_ENABLE, (uint32_t) 1U);
            CSL_REG32_WR(&chHandle->pTxRtRegs->PEER8, regVal);

            (void) CSL_udmapSetTxRT(&drvHandle->udmapRegs, chHandle->txChNum, &udmapRtEnable);
        }

        if(UDMA_CH_FLAG_RX == (chHandle->chType & UDMA_CH_FLAG_RX))
        {
            Udma_assert(drvHandle, (NULL_PTR != chHandle->pRxRtRegs));
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->rxChNum));

            /*
            * Note: UDMAP shoule be enabled first (receiver) before enabling
            *       PEER/PDMA through PSIL (source)
            *       This ensures UDMAP RX is ready to receive data
            */
            /* Note: Block copy uses same RX channel. So do for both TX/RX */
            (void) CSL_udmapSetRxRT(
                &drvHandle->udmapRegs, chHandle->rxChNum, &udmapRtEnable);

            regVal = CSL_REG32_RD(&chHandle->pRxRtRegs->PEER8);
            CSL_FINS(regVal, PSILCFG_REG_RT_ENABLE_ENABLE, (uint32_t) 1U);
            CSL_REG32_WR(&chHandle->pRxRtRegs->PEER8, regVal);
        }
    }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
    {
       /* Set only enable and clear all other flags which might be set from
        * previous run */
        bcdmaRtEnable.enable         = UTRUE;
        bcdmaRtEnable.teardown       = UFALSE;
        bcdmaRtEnable.forcedTeardown = UFALSE;
        bcdmaRtEnable.pause          = UFALSE;
        bcdmaRtEnable.error          = UFALSE;

        if(UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY))
        {
            Udma_assert(drvHandle, (NULL_PTR != chHandle->pBcdmaBcRtRegs));
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->txChNum));

            /* PEER8 Reg param missing in CSL_bcdma_bcrtRegs_chan, So what to do here?*/
            //regVal = CSL_REG32_RD(&chHandle->pBcdmaBcRtRegs->PEER8);
            //CSL_FINS(regVal, PSILCFG_REG_RT_ENABLE_ENABLE, (uint32_t) 1U);
            //CSL_REG32_WR(&chHandle->pBcdmaBcRtRegs->PEER8, regVal);

            (void) CSL_bcdmaSetTxRT(&drvHandle->bcdmaRegs, chHandle->txChNum , &bcdmaRtEnable);
        }

        else if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
        {
            Udma_assert(drvHandle, (NULL_PTR != chHandle->pBcdmaTxRtRegs));
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->txChNum));

            regVal = CSL_REG32_RD(&chHandle->pBcdmaTxRtRegs->PEER8);
            CSL_FINS(regVal, PSILCFG_REG_RT_ENABLE_ENABLE, (uint32_t) 1U);
            CSL_REG32_WR(&chHandle->pBcdmaTxRtRegs->PEER8, regVal);

            /* Add offset to ChNum so that BCDMA can identify it as Tx Channel */
            (void) CSL_bcdmaSetTxRT(&drvHandle->bcdmaRegs, chHandle->txChNum + drvHandle->txChOffset, &bcdmaRtEnable);
        }

        else if (UDMA_CH_FLAG_RX == (chHandle->chType & UDMA_CH_FLAG_RX))
        {
            Udma_assert(drvHandle, (NULL_PTR != chHandle->pBcdmaRxRtRegs));
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->rxChNum));

            /*
            * Note: UDMAP shoule be enabled first (receiver) before enabling
            *       PEER/PDMA through PSIL (source)
            *       This ensures UDMAP RX is ready to receive data
            */
            /* Add offset to ChNum so that BCDMA can identify it as Rx Channel */
            (void) CSL_bcdmaSetRxRT(
                &drvHandle->bcdmaRegs, chHandle->rxChNum + drvHandle->rxChOffset, &bcdmaRtEnable);

            regVal = CSL_REG32_RD(&chHandle->pBcdmaRxRtRegs->PEER8);
            CSL_FINS(regVal, PSILCFG_REG_RT_ENABLE_ENABLE, (uint32_t) 1U);
            CSL_REG32_WR(&chHandle->pBcdmaRxRtRegs->PEER8, regVal);
        }
        else
        {
              /* Do Nothing */
        }
    }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
    {
       /* Set only enable and clear all other flags which might be set from
        * previous run */
        pktdmaRtEnable.enable         = UTRUE;
        pktdmaRtEnable.teardown       = UFALSE;
        pktdmaRtEnable.forcedTeardown = UFALSE;
        pktdmaRtEnable.pause          = UFALSE;
        pktdmaRtEnable.error          = UFALSE;

        if(UDMA_CH_FLAG_TX == (chHandle->chType & UDMA_CH_FLAG_TX))
        {
            Udma_assert(drvHandle, (NULL_PTR != chHandle->pPktdmaTxRtRegs));
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->txChNum));

            regVal = CSL_REG32_RD(&chHandle->pPktdmaTxRtRegs->PEER8);
            CSL_FINS(regVal, PSILCFG_REG_RT_ENABLE_ENABLE, (uint32_t) 1U);
            CSL_REG32_WR(&chHandle->pPktdmaTxRtRegs->PEER8, regVal);

            (void) CSL_pktdmaSetTxRT(&drvHandle->pktdmaRegs, chHandle->txChNum, &pktdmaRtEnable);
        }

        if(UDMA_CH_FLAG_RX == (chHandle->chType & UDMA_CH_FLAG_RX))
        {
            Udma_assert(drvHandle, (NULL_PTR != chHandle->pPktdmaRxRtRegs));
            Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->rxChNum));

            /*
            * Note: UDMAP shoule be enabled first (receiver) before enabling
            *       PEER/PDMA through PSIL (source)
            *       This ensures UDMAP RX is ready to receive data
            */
            /* Note: Block copy uses same RX channel. So do for both TX/RX */
            (void) CSL_pktdmaSetRxRT(
                &drvHandle->pktdmaRegs, chHandle->rxChNum, &pktdmaRtEnable);

            regVal = CSL_REG32_RD(&chHandle->pPktdmaRxRtRegs->PEER8);
            CSL_FINS(regVal, PSILCFG_REG_RT_ENABLE_ENABLE, (uint32_t) 1U);
            CSL_REG32_WR(&chHandle->pPktdmaRxRtRegs->PEER8, regVal);
        }

    }
#endif



#if (UDMA_NUM_UTC_INSTANCE > 0)
    if(UDMA_CH_FLAG_UTC == (chHandle->chType & UDMA_CH_FLAG_UTC))
    {
        utcInfo = chHandle->utcInfo;
        Udma_assert(drvHandle, (NULL_PTR != utcInfo));
        if(CSL_DRU_OWNER_DIRECT_TR == chHandle->utcPrms.druOwner)
        {
            /* Enable DRU incase of direct TR mode */
            Udma_assert(drvHandle, (NULL_PTR != utcInfo->druRegs));
            Udma_assert(drvHandle, (chHandle->extChNum >= utcInfo->startCh));
            utcChNum = chHandle->extChNum - utcInfo->startCh;

            retVal = CSL_druChEnable(utcInfo->druRegs, utcChNum);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle,
                    "[Error] DRU channel enable failed!!\n");
            }
        }
        else
        {
            /* For UTC, destination thread enable should be done -
             * pairing not required. Use src thread instead of dest thread
             * (already has bit info CSL_PSILCFG_DEST_THREAD_OFFSET,
             * so just reset the bit) */
            srcThreadId = chHandle->peerThreadId &
                                ~((uint32_t) CSL_PSILCFG_DEST_THREAD_OFFSET);
            retVal = Udma_psilcfgSetEnable(drvHandle, srcThreadId, UTRUE);
            if(UDMA_SOK != retVal)
            {
                Udma_printf(drvHandle,
                    "[Error] PSI UTC destination thread enable failed!!!\n");
            }
            else
            {
                retVal = Udma_psilcfgSetRtEnable(drvHandle, srcThreadId, UTRUE);
                if(UDMA_SOK != retVal)
                {
                    Udma_printf(drvHandle,
                        "[Error] PSI UTC destination thread RT enable failed!!!\n");
                }
            }

            /* Same TX channel CSL API is used for UTC as well - but need to
             * add the EXT channel offset */
            Udma_assert(drvHandle, UDMA_DMA_CH_INVALID != chHandle->extChNum);

#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
            if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
            {
                (void) CSL_udmapSetTxRT(
                    &drvHandle->udmapRegs,
                    chHandle->extChNum + drvHandle->extChOffset,
                    &udmapRtEnable);
            }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
            if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
            {
                (void) CSL_bcdmaSetTxRT(
                    &drvHandle->bcdmaRegs,
                    chHandle->extChNum + drvHandle->extChOffset,
                    &bcdmaRtEnable);
            }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
            if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
            {
                (void) CSL_pktdmaSetTxRT(
                    &drvHandle->pktdmaRegs,
                    chHandle->extChNum + drvHandle->extChOffset,
                    &pktdmaRtEnable);
            }
#endif
        }
    }
#endif

    return (retVal);
}

static int32_t Udma_chDisableBlkCpyChan(Udma_ChHandle chHandle, uint32_t timeout)
{
    int32_t         retVal = UDMA_SOK;
    Bool            exitLoop = UFALSE;
    uint32_t        currTimeout = 0U;
    Udma_DrvHandle  drvHandle;
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    CSL_UdmapRT     udmapRtStatus;
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    CSL_BcdmaRT     bcdmaRtStatus;
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    CSL_PktdmaRT    pktdmaRtStatus;
#endif

    drvHandle = chHandle->drvHandle;
    Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->txChNum));

    /* Initiate graceful teardown first - Source is udma thread for TX */
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
        retVal = CSL_udmapTeardownTxChan(
                    &drvHandle->udmapRegs, chHandle->txChNum, BFALSE, BFALSE);
    }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
    {
        retVal = CSL_bcdmaTeardownTxChan(
                 &drvHandle->bcdmaRegs, chHandle->txChNum, BFALSE, BFALSE);
    }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
    {
        retVal = CSL_pktdmaTeardownTxChan(
                 &drvHandle->pktdmaRegs, chHandle->txChNum, BFALSE, BFALSE);
    }
#endif

    if(CSL_PASS != retVal)
    {
        Udma_printf(drvHandle, "[Error] UDMA Blkcpy teardown failed!!\n");
    }

    /* Check if graceful teardown is complete */
    while(UDMA_SOK == retVal)
    {
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            (void) CSL_udmapGetTxRT(&drvHandle->udmapRegs, chHandle->txChNum, &udmapRtStatus);
            if(UFALSE == udmapRtStatus.enable)
            {
                /* Teardown complete */
                exitLoop = UTRUE;
            }
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            (void) CSL_bcdmaGetTxRT(&drvHandle->bcdmaRegs, chHandle->txChNum, &bcdmaRtStatus);
            if(UFALSE == bcdmaRtStatus.enable)
            {
                /* Teardown complete */
                exitLoop = UTRUE;
            }
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            (void) CSL_pktdmaGetTxRT(&drvHandle->pktdmaRegs, chHandle->txChNum, &pktdmaRtStatus);
            if(UFALSE == pktdmaRtStatus.enable)
            {
                /* Teardown complete */
                exitLoop = UTRUE;
            }
        }
#endif
        if (UTRUE == exitLoop)
        {
           break;
        }

        if(currTimeout >= timeout)
        {
            retVal = UDMA_ETIMEOUT;
        }
        else
        {
            (void) Osal_delay(1U);
            currTimeout++;
        }
    }

    if(UDMA_SOK != retVal)
    {
        /* Graceful teardown failed - initiate force teardown */
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            retVal = CSL_udmapTeardownTxChan(
                        &drvHandle->udmapRegs, chHandle->txChNum, BTRUE, BFALSE);
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            retVal = CSL_bcdmaTeardownTxChan(
                        &drvHandle->bcdmaRegs, chHandle->txChNum, BTRUE, BFALSE);
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            retVal = CSL_pktdmaTeardownTxChan(
                        &drvHandle->pktdmaRegs, chHandle->txChNum, BTRUE, BFALSE);
        }
#endif

        if(CSL_PASS != retVal)
        {
            Udma_printf(drvHandle, "[Error] UDMA Blkcpy force disable failed!!\n");
        }

        /* Wait for disable to complete */
        currTimeout = 0U;
        exitLoop = UFALSE;
        while(UDMA_SOK == retVal)
        {
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
            if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
            {
                (void) CSL_udmapGetTxRT(&drvHandle->udmapRegs, chHandle->txChNum, &udmapRtStatus);
                if(UFALSE == udmapRtStatus.enable)
                {
                    /* Teardown complete */
                    exitLoop = UTRUE;
                }
            }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
            if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
            {
                (void) CSL_bcdmaGetTxRT(&drvHandle->bcdmaRegs, chHandle->txChNum, &bcdmaRtStatus);
                if(UFALSE == bcdmaRtStatus.enable)
                {
                    /* Teardown complete */
                    exitLoop = UTRUE;
                }
            }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
            if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
            {
                (void) CSL_pktdmaGetTxRT(&drvHandle->pktdmaRegs, chHandle->txChNum, &pktdmaRtStatus);
                if(UFALSE == pktdmaRtStatus.enable)
                {
                    /* Teardown complete */
                    exitLoop = UTRUE;
                }
            }
#endif
            if (UTRUE == exitLoop)
            {
               break;
            }

            if(currTimeout >= timeout)
            {
                retVal = UDMA_ETIMEOUT;
                Udma_printf(drvHandle, "[Error] Blockcpy ch teardown timed out!!!\n");
            }
            else
            {
                (void) Osal_delay(1U);
                currTimeout++;
            }
        }
    }

    if(UDMA_SOK == retVal)
    {
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            /* Clear teardown and enable bits in UDMAP */
            udmapRtStatus.enable   = UFALSE;
            udmapRtStatus.teardown = UFALSE;
            udmapRtStatus.forcedTeardown = UFALSE;
            (void) CSL_udmapSetTxRT(&drvHandle->udmapRegs, chHandle->txChNum, &udmapRtStatus);
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            /* Clear teardown and enable bits in UDMAP */
            bcdmaRtStatus.enable   = UFALSE;
            bcdmaRtStatus.teardown = UFALSE;
            bcdmaRtStatus.forcedTeardown = UFALSE;
            (void) CSL_bcdmaSetTxRT(&drvHandle->bcdmaRegs, chHandle->txChNum, &bcdmaRtStatus);
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            /* Clear teardown and enable bits in UDMAP */
            pktdmaRtStatus.enable   = UFALSE;
            pktdmaRtStatus.teardown = UFALSE;
            pktdmaRtStatus.forcedTeardown = UFALSE;
            (void) CSL_pktdmaSetTxRT(&drvHandle->pktdmaRegs, chHandle->txChNum, &pktdmaRtStatus);
        }
#endif

    }

    return (retVal);
}

static int32_t Udma_chDisableTxChan(Udma_ChHandle chHandle, uint32_t timeout)
{
    int32_t         retVal = UDMA_SOK;
    Bool            exitLoop = UFALSE;
    uint32_t        peerRtEnable = 0U, currTimeout = 0U;
    Udma_DrvHandle  drvHandle;
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    CSL_UdmapRT     udmapRtStatus;
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    CSL_BcdmaRT     bcdmaRtStatus;
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    CSL_PktdmaRT    pktdmaRtStatus;
#endif
    uint32_t        rtEnableRegOffset;
    drvHandle = chHandle->drvHandle;
    Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->txChNum));
    rtEnableRegOffset = CSL_PSILCFG_REG_RT_ENABLE - CSL_PSILCFG_REG_STATIC_TR;

    /* Initiate graceful teardown first - Source is udma thread for TX */
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
        retVal = CSL_udmapTeardownTxChan(
                    &drvHandle->udmapRegs, chHandle->txChNum, BFALSE, BFALSE);
    }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
    {
        /*Add offset to chNum, so that BCDMA can identify it as Tx Channel*/
        retVal = CSL_bcdmaTeardownTxChan(
                 &drvHandle->bcdmaRegs, chHandle->txChNum + drvHandle->txChOffset, BFALSE, BFALSE);
    }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
    {
        retVal = CSL_pktdmaTeardownTxChan(
                 &drvHandle->pktdmaRegs, chHandle->txChNum, BFALSE, BFALSE);
    }
#endif

    if(CSL_PASS != retVal)
    {
        Udma_printf(drvHandle, "[Error] UDMA TX teardown failed!!\n");
    }

    /* Check if graceful teardown is complete */
    while(UDMA_SOK == retVal)
    {
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            (void) CSL_udmapGetTxRT(&drvHandle->udmapRegs, chHandle->txChNum, &udmapRtStatus);
            if(UFALSE == udmapRtStatus.enable)
            {
                /* Teardown complete */
                exitLoop = UTRUE;
            }
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            /*Add offset to chNum, so that BCDMA can identify it as Tx Channel*/
            (void) CSL_bcdmaGetTxRT(&drvHandle->bcdmaRegs, chHandle->txChNum + drvHandle->txChOffset, &bcdmaRtStatus);
            if(UFALSE == bcdmaRtStatus.enable)
            {
                /* Teardown complete */
                exitLoop = UTRUE;
            }
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            (void) CSL_pktdmaGetTxRT(&drvHandle->pktdmaRegs, chHandle->txChNum, &pktdmaRtStatus);
            if(UFALSE == pktdmaRtStatus.enable)
            {
                /* Teardown complete */
              exitLoop = UTRUE;
            }
        }
#endif
        if (UTRUE == exitLoop)
        {
          break;
        }

        if(currTimeout >= timeout)
        {
            retVal = UDMA_ETIMEOUT;
        }
        else
        {
            (void) Osal_delay(1U);
            currTimeout++;
        }
    }

    if(UDMA_SOK != retVal)
    {
        /* Graceful teardown failed - initiate force teardown */
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            retVal = CSL_udmapTeardownTxChan(
                        &drvHandle->udmapRegs, chHandle->txChNum, BTRUE, BFALSE);
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            /*Add offset to chNum, so that BCDMA can identify it as Tx Channel*/
            retVal = CSL_bcdmaTeardownTxChan(
                        &drvHandle->bcdmaRegs, chHandle->txChNum + drvHandle->txChOffset, BTRUE, BFALSE);
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            retVal = CSL_pktdmaTeardownTxChan(
                        &drvHandle->pktdmaRegs, chHandle->txChNum, BTRUE, BFALSE);
        }
#endif
        if(CSL_PASS != retVal)
        {
            Udma_printf(drvHandle, "[Error] UDMA TX force disable failed!!\n");
        }

#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            /* Set flush in peer */
            (void) CSL_udmapGetChanPeerReg(
                &drvHandle->udmapRegs,
                chHandle->txChNum,
                CSL_UDMAP_CHAN_DIR_TX,
                rtEnableRegOffset,
                &peerRtEnable);
            CSL_FINS(peerRtEnable, PSILCFG_REG_RT_ENABLE_FLUSH, (uint32_t) 1U);
            (void) CSL_udmapSetChanPeerReg(
                &drvHandle->udmapRegs,
                chHandle->txChNum,
                CSL_UDMAP_CHAN_DIR_TX,
                rtEnableRegOffset,
                &peerRtEnable);
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            /*Add offset to chNum, so that BCDMA can identify it as Tx Channel*/
            /* Set flush in peer */
            (void) CSL_bcdmaGetChanPeerReg(
                &drvHandle->bcdmaRegs,
                chHandle->txChNum + drvHandle->txChOffset,
                CSL_BCDMA_CHAN_DIR_TX,
                rtEnableRegOffset,
                &peerRtEnable);
            CSL_FINS(peerRtEnable, PSILCFG_REG_RT_ENABLE_FLUSH, (uint32_t) 1U);
            (void) CSL_bcdmaSetChanPeerReg(
                &drvHandle->bcdmaRegs,
                chHandle->txChNum + drvHandle->txChOffset,
                CSL_BCDMA_CHAN_DIR_TX,
                rtEnableRegOffset,
                &peerRtEnable);
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            /* Set flush in peer */
            (void) CSL_pktdmaGetChanPeerReg(
                &drvHandle->pktdmaRegs,
                chHandle->txChNum,
                CSL_BCDMA_CHAN_DIR_TX,
                rtEnableRegOffset,
                &peerRtEnable);
            CSL_FINS(peerRtEnable, PSILCFG_REG_RT_ENABLE_FLUSH, (uint32_t) 1U);
            (void) CSL_pktdmaSetChanPeerReg(
                &drvHandle->pktdmaRegs,
                chHandle->txChNum,
                CSL_PKTDMA_CHAN_DIR_TX,
                rtEnableRegOffset,
                &peerRtEnable);
        }
#endif
        /* Wait for disable to complete */
        currTimeout = 0U;
        exitLoop    = UFALSE;
        while(UDMA_SOK == retVal)
        {

#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
            if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
            {
                (void) CSL_udmapGetTxRT(&drvHandle->udmapRegs, chHandle->txChNum, &udmapRtStatus);
                (void) CSL_udmapGetChanPeerReg(
                    &drvHandle->udmapRegs,
                    chHandle->txChNum,
                    CSL_UDMAP_CHAN_DIR_TX,
                    rtEnableRegOffset, &peerRtEnable);
                if((UFALSE == udmapRtStatus.enable) &&
                (0U == CSL_FEXT(peerRtEnable, PSILCFG_REG_RT_ENABLE_ENABLE)))
                {
                    /* Teardown complete */
                    exitLoop = UTRUE;
                }
            }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
            if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
            {
                /*Add offset to chNum, so that BCDMA can identify it as Tx Channel*/
                (void) CSL_bcdmaGetTxRT(&drvHandle->bcdmaRegs, chHandle->txChNum + drvHandle->txChOffset, &bcdmaRtStatus);
                (void) CSL_bcdmaGetChanPeerReg(
                    &drvHandle->bcdmaRegs,
                    chHandle->txChNum + drvHandle->txChOffset,
                    CSL_BCDMA_CHAN_DIR_TX,
                    rtEnableRegOffset, &peerRtEnable);
                if((UFALSE == bcdmaRtStatus.enable) &&
                (0U == CSL_FEXT(peerRtEnable, PSILCFG_REG_RT_ENABLE_ENABLE)))
                {
                    /* Teardown complete */
                    exitLoop = UTRUE;
                }
            }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
            if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
            {
                (void) CSL_pktdmaGetTxRT(&drvHandle->pktdmaRegs, chHandle->txChNum, &pktdmaRtStatus);
                (void) CSL_pktdmaGetChanPeerReg(
                    &drvHandle->pktdmaRegs,
                    chHandle->txChNum,
                    CSL_PKTDMA_CHAN_DIR_TX,
                    rtEnableRegOffset, &peerRtEnable);
                if((UFALSE == pktdmaRtStatus.enable) &&
                (0U == CSL_FEXT(peerRtEnable, PSILCFG_REG_RT_ENABLE_ENABLE)))
                {
                    /* Teardown complete */
                    exitLoop = UTRUE;
                }
            }
#endif
            if(UTRUE == exitLoop)
            {
              break;
            }

            if(currTimeout >= timeout)
            {
                retVal = UDMA_ETIMEOUT;
                Udma_printf(drvHandle, "[Error] TX ch teardown timed out!!!\n");
            }
            else
            {
                (void) Osal_delay(1U);
                currTimeout++;
            }
        }
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            /* Clear flush in peer */
            CSL_FINS(peerRtEnable, PSILCFG_REG_RT_ENABLE_FLUSH, (uint32_t) 0U);
            (void) CSL_udmapSetChanPeerReg(
                &drvHandle->udmapRegs,
                chHandle->txChNum,
                CSL_UDMAP_CHAN_DIR_TX,
                rtEnableRegOffset,
                &peerRtEnable);
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            /* Clear flush in peer */
            CSL_FINS(peerRtEnable, PSILCFG_REG_RT_ENABLE_FLUSH, (uint32_t) 0U);
            (void) CSL_bcdmaSetChanPeerReg(
                &drvHandle->bcdmaRegs,
                chHandle->txChNum + drvHandle->txChOffset,
                CSL_BCDMA_CHAN_DIR_TX,
                rtEnableRegOffset,
                &peerRtEnable);
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            /* Clear flush in peer */
            CSL_FINS(peerRtEnable, PSILCFG_REG_RT_ENABLE_FLUSH, (uint32_t) 0U);
            (void) CSL_pktdmaSetChanPeerReg(
                &drvHandle->pktdmaRegs,
                chHandle->txChNum,
                CSL_PKTDMA_CHAN_DIR_TX,
                rtEnableRegOffset,
                &peerRtEnable);
        }
#endif
    }

    if(UDMA_SOK == retVal)
    {
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            /* Clear teardown and enable bits in both UDMAP and peer */
            udmapRtStatus.enable   = UFALSE;
            udmapRtStatus.teardown = UFALSE;
            udmapRtStatus.forcedTeardown = UFALSE;
            CSL_FINS(peerRtEnable, PSILCFG_REG_RT_ENABLE_TDOWN, (uint32_t) 0U);
            (void) CSL_udmapSetTxRT(&drvHandle->udmapRegs, chHandle->txChNum, &udmapRtStatus);
            (void) CSL_udmapSetChanPeerReg(
                &drvHandle->udmapRegs,
                chHandle->txChNum,
                CSL_UDMAP_CHAN_DIR_TX,
                rtEnableRegOffset,
                &peerRtEnable);
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            /* Clear teardown and enable bits in both BCDMA and peer */
            bcdmaRtStatus.enable   = UFALSE;
            bcdmaRtStatus.teardown = UFALSE;
            bcdmaRtStatus.forcedTeardown = UFALSE;
            CSL_FINS(peerRtEnable, PSILCFG_REG_RT_ENABLE_TDOWN, (uint32_t) 0U);
            /*Add offset to chNum, so that BCDMA can identify it as Tx Channel*/
            (void) CSL_bcdmaSetTxRT(&drvHandle->bcdmaRegs, chHandle->txChNum + drvHandle->txChOffset, &bcdmaRtStatus);
            (void) CSL_bcdmaSetChanPeerReg(
                &drvHandle->bcdmaRegs,
                chHandle->txChNum + drvHandle->txChOffset,
                CSL_BCDMA_CHAN_DIR_TX,
                rtEnableRegOffset,
                &peerRtEnable);
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            /* Clear teardown and enable bits in both PKTDMA and peer */
            pktdmaRtStatus.enable   = UFALSE;
            pktdmaRtStatus.teardown = UFALSE;
            pktdmaRtStatus.forcedTeardown = UFALSE;
            CSL_FINS(peerRtEnable, PSILCFG_REG_RT_ENABLE_TDOWN, (uint32_t) 0U);
            (void) CSL_pktdmaSetTxRT(&drvHandle->pktdmaRegs, chHandle->txChNum, &pktdmaRtStatus);
            (void) CSL_pktdmaSetChanPeerReg(
                &drvHandle->pktdmaRegs,
                chHandle->txChNum,
                CSL_PKTDMA_CHAN_DIR_TX,
                rtEnableRegOffset,
                &peerRtEnable);
        }
#endif
    }

    return (retVal);
}

static int32_t Udma_chDisableRxChan(Udma_ChHandle chHandle, uint32_t timeout)
{
    int32_t         retVal      = UDMA_SOK;
    Bool            exitLoop    = UFALSE;
    uint32_t        currTimeout = 0U, regVal;
    Udma_DrvHandle  drvHandle;
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    CSL_UdmapRT     udmapRtStatus;
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    CSL_BcdmaRT     bcdmaRtStatus;
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    CSL_PktdmaRT    pktdmaRtStatus;
#endif
    uint32_t        peerRtEnable = 0U, peerRtEnableBit = 0U;
    uint32_t        rtEnableRegOffset;

    drvHandle = chHandle->drvHandle;
    Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->rxChNum));
    Udma_assert(drvHandle, (UDMA_THREAD_ID_INVALID != chHandle->peerThreadId));
    rtEnableRegOffset = CSL_PSILCFG_REG_RT_ENABLE - CSL_PSILCFG_REG_STATIC_TR;

    /* Initiate graceful teardown first - Source is peer thread for RX */
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
        regVal = CSL_REG32_RD(&chHandle->pRxRtRegs->PEER8);
        CSL_FINS(regVal, PSILCFG_REG_RT_ENABLE_TDOWN, (uint32_t) 1U);
        CSL_REG32_WR(&chHandle->pRxRtRegs->PEER8, regVal);
    }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
    {
        regVal = CSL_REG32_RD(&chHandle->pBcdmaRxRtRegs->PEER8);
        CSL_FINS(regVal, PSILCFG_REG_RT_ENABLE_TDOWN, (uint32_t) 1U);
        CSL_REG32_WR(&chHandle->pBcdmaRxRtRegs->PEER8, regVal);
    }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
    {
        regVal = CSL_REG32_RD(&chHandle->pPktdmaRxRtRegs->PEER8);
        CSL_FINS(regVal, PSILCFG_REG_RT_ENABLE_TDOWN, (uint32_t) 1U);
        CSL_REG32_WR(&chHandle->pPktdmaRxRtRegs->PEER8, regVal);
    }
#endif

    /* Check if graceful teardown is complete */
    while(UDMA_SOK == retVal)
    {
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            (void) CSL_udmapGetRxRT(&drvHandle->udmapRegs, chHandle->rxChNum, &udmapRtStatus);
            if(UFALSE == udmapRtStatus.enable)
            {
                /* Teardown complete */
              exitLoop = UTRUE;
            }
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            /*Add offset to chNum, so that BCDMA can identify it as Rx Channel*/
            (void) CSL_bcdmaGetRxRT(&drvHandle->bcdmaRegs, chHandle->rxChNum + drvHandle->rxChOffset, &bcdmaRtStatus);
            if(UFALSE == bcdmaRtStatus.enable)
            {
                /* Teardown complete */
                exitLoop = UTRUE;
            }
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            (void) CSL_pktdmaGetRxRT(&drvHandle->pktdmaRegs, chHandle->rxChNum, &pktdmaRtStatus);
            if(UFALSE == pktdmaRtStatus.enable)
            {
                /* Teardown complete */
                exitLoop = UTRUE;
            }
        }
#endif
        if(UTRUE == exitLoop)
        {
          break;
        }

        if(currTimeout >= timeout)
        {
            retVal = UDMA_ETIMEOUT;
        }
        else
        {
            (void) Osal_delay(1U);
            currTimeout++;
        }
    }

    if(UDMA_SOK != retVal)
    {
        /* Graceful teardown failed - initiate force teardown */
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            retVal = CSL_udmapTeardownRxChan(
                        &drvHandle->udmapRegs, chHandle->rxChNum, BTRUE, BFALSE);
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            /*Add offset to chNum, so that BCDMA can identify it as Rx Channel*/
            retVal = CSL_bcdmaTeardownRxChan(
                        &drvHandle->bcdmaRegs, chHandle->rxChNum + drvHandle->rxChOffset, BTRUE, BFALSE);
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            retVal = CSL_pktdmaTeardownRxChan(
                        &drvHandle->pktdmaRegs, chHandle->rxChNum, BTRUE, BFALSE);
        }
#endif
        if(CSL_PASS != retVal)
        {
            Udma_printf(drvHandle, "[Error] UDMA RX force disable failed!!\n");
        }

        /* Wait for disable to complete - both locally and for peer thread */
        currTimeout = 0U;
        exitLoop    = UFALSE;
        while(UDMA_SOK == retVal)
        {
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
            if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
            {
                (void) CSL_udmapGetRxRT(&drvHandle->udmapRegs, chHandle->rxChNum, &udmapRtStatus);
                (void) CSL_udmapGetChanPeerReg(
                    &drvHandle->udmapRegs,
                    chHandle->rxChNum,
                    CSL_UDMAP_CHAN_DIR_RX,
                    rtEnableRegOffset, &peerRtEnable);
                peerRtEnableBit = CSL_FEXT(peerRtEnable, PSILCFG_REG_RT_ENABLE_ENABLE);
               if((UFALSE == udmapRtStatus.enable) && (UFALSE == peerRtEnableBit))
                {
                    /* Teardown complete */
                    exitLoop = UTRUE;
                }
            }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
            if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
            {
                /*Add offset to chNum, so that BCDMA can identify it as Rx Channel*/
                (void) CSL_bcdmaGetRxRT(&drvHandle->bcdmaRegs, chHandle->rxChNum + drvHandle->rxChOffset, &bcdmaRtStatus);
                (void) CSL_bcdmaGetChanPeerReg(
                    &drvHandle->bcdmaRegs,
                    chHandle->rxChNum + drvHandle->rxChOffset,
                    CSL_BCDMA_CHAN_DIR_RX,
                    rtEnableRegOffset, &peerRtEnable);
                peerRtEnableBit = CSL_FEXT(peerRtEnable, PSILCFG_REG_RT_ENABLE_ENABLE);
                if((UFALSE == bcdmaRtStatus.enable) && (UFALSE == peerRtEnableBit))
                {
                    /* Teardown complete */
                    exitLoop = UTRUE;
                }
            }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
            if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
            {
                (void) CSL_pktdmaGetRxRT(&drvHandle->pktdmaRegs, chHandle->rxChNum, &pktdmaRtStatus);
                (void) CSL_pktdmaGetChanPeerReg(
                    &drvHandle->pktdmaRegs,
                    chHandle->rxChNum,
                    CSL_PKTDMA_CHAN_DIR_RX,
                    rtEnableRegOffset, &peerRtEnable);
                peerRtEnableBit = CSL_FEXT(peerRtEnable, PSILCFG_REG_RT_ENABLE_ENABLE);
                if((UFALSE == pktdmaRtStatus.enable) && (UFALSE == peerRtEnableBit))
                {
                    /* Teardown complete */
                    exitLoop = UTRUE;
                }
            }
#endif
            if(UTRUE == exitLoop)
            {
              break;
            }

            if(currTimeout >= timeout)
            {
                retVal = UDMA_ETIMEOUT;
                Udma_printf(drvHandle, "[Error] RX ch teardown timed out!!!\n");
            }
            else
            {
                (void) Osal_delay(1U);
                currTimeout++;
            }
        }
    }

    if(UDMA_SOK == retVal)
    {

#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            /* Clear teardown bits in both the UDMAP and peer */
            udmapRtStatus.teardown = UFALSE;   /* Note that other bits are cleared from previous call */
            CSL_FINS(peerRtEnable, PSILCFG_REG_RT_ENABLE_TDOWN, UFALSE);
            (void) CSL_udmapSetRxRT(
                &drvHandle->udmapRegs, chHandle->rxChNum, &udmapRtStatus);
            (void) CSL_udmapSetChanPeerReg(
                &drvHandle->udmapRegs,
                chHandle->rxChNum,
                CSL_UDMAP_CHAN_DIR_RX,
                rtEnableRegOffset,
                &peerRtEnable);
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            /* Clear teardown bits in both the BCDMA and peer */
            bcdmaRtStatus.teardown = UFALSE;   /* Note that other bits are cleared from previous call */
            CSL_FINS(peerRtEnable, PSILCFG_REG_RT_ENABLE_TDOWN, UFALSE);
            /*Add offset to chNum, so that BCDMA can identify it as Rx Channel*/
            (void) CSL_bcdmaSetRxRT(
                &drvHandle->bcdmaRegs, chHandle->rxChNum + drvHandle->rxChOffset, &bcdmaRtStatus);
            (void) CSL_bcdmaSetChanPeerReg(
                &drvHandle->bcdmaRegs,
                chHandle->rxChNum + drvHandle->rxChOffset,
                CSL_BCDMA_CHAN_DIR_RX,
                rtEnableRegOffset,
                &peerRtEnable);
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            /* Clear teardown bits in both the PKTDMA and peer */
            pktdmaRtStatus.teardown = UFALSE;   /* Note that other bits are cleared from previous call */
            CSL_FINS(peerRtEnable, PSILCFG_REG_RT_ENABLE_TDOWN, UFALSE);
            (void) CSL_pktdmaSetRxRT(
                &drvHandle->pktdmaRegs, chHandle->rxChNum, &pktdmaRtStatus);
            (void) CSL_pktdmaSetChanPeerReg(
                &drvHandle->pktdmaRegs,
                chHandle->rxChNum,
                CSL_PKTDMA_CHAN_DIR_RX,
                rtEnableRegOffset,
                &peerRtEnable);
        }
#endif
    }

    return (retVal);
}

#if (UDMA_NUM_UTC_INSTANCE > 0)
static int32_t Udma_chDisableExtChan(Udma_ChHandle chHandle, uint32_t timeout)
{
    int32_t                 retVal = UDMA_SOK;
    Udma_DrvHandle          drvHandle;
    uint32_t                utcChNum, srcThreadId;
    uint32_t                status;
    uint32_t                currTimeout = 0U;
    const Udma_UtcInstInfo *utcInfo;

    drvHandle = chHandle->drvHandle;

    utcInfo = chHandle->utcInfo;
    Udma_assert(drvHandle, (NULL_PTR != utcInfo));
    Udma_assert(drvHandle, (UDMA_DMA_CH_INVALID != chHandle->extChNum));
    if(UDMA_UTC_TYPE_DRU == utcInfo->utcType)
    {
        Udma_assert(drvHandle, (NULL_PTR != utcInfo->druRegs));
        Udma_assert(drvHandle, (chHandle->extChNum >= utcInfo->startCh));
        utcChNum = chHandle->extChNum - utcInfo->startCh;
    }

    if(UDMA_UTC_TYPE_DRU == utcInfo->utcType)
    {
        if(CSL_DRU_OWNER_DIRECT_TR == chHandle->utcPrms.druOwner)
        {
            retVal = CSL_druChTeardown(utcInfo->druRegs, utcChNum);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] DRU channel teardown failed!!\n");
            }

            /* Wait for teardown to complete */
            while(UDMA_SOK == retVal)
            {
                status = CSL_druChIsTeardownComplete(utcInfo->druRegs, utcChNum);
                if(UTRUE == status)
                {
                    /* Teardown complete */
                    break;
                }

                if(currTimeout >= timeout)
                {
                    retVal = UDMA_ETIMEOUT;
                    Udma_printf(drvHandle, "[Error] DRU ch teardown timed out!!!\n");
                }
                else
                {
                    (void) Osal_delay(1U);
                    currTimeout++;
                }
            }
        }
        else
        {
            /*
             * Initiate teardown - DRU is special - should be done as per below only
             */
            /* Use src thread instead of dest thread
             * (already has bit info CSL_PSILCFG_DEST_THREAD_OFFSET,
             * so just reset the bit) */
            Udma_assert(drvHandle, (UDMA_THREAD_ID_INVALID != chHandle->peerThreadId));
            srcThreadId = chHandle->peerThreadId &
                                ~((uint32_t) CSL_PSILCFG_DEST_THREAD_OFFSET);
            retVal = Udma_psilcfgWrite(
                         drvHandle,
                         srcThreadId,
                         CSL_PSILCFG_REG_ENABLE,
                         CSL_PSILCFG_REG_ENABLE_TEARDOWN_MASK);
            if(UDMA_SOK != retVal)
            {
                Udma_printf(drvHandle, "[Error] PSIL write teardown failed!!\n");
            }

            /* Wait for teardown to complete */
            while(UDMA_SOK == retVal)
            {
                status = CSL_druChIsTeardownComplete(utcInfo->druRegs, utcChNum);
                if(UTRUE == status)
                {
                    /* Teardown complete */
                    break;
                }

                if(currTimeout >= timeout)
                {
                    retVal = UDMA_ETIMEOUT;
                    Udma_printf(drvHandle, "[Error] DRU ch teardown timed out!!!\n");
                }
                else
                {
                    (void) Osal_delay(1U);
                    currTimeout++;
                }
            }

            /* Disable External channel - this will clear the Enable bit */
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
            if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
            {
                retVal += CSL_udmapDisableTxChan(
                            &drvHandle->udmapRegs,
                            chHandle->extChNum + drvHandle->extChOffset);
            }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
            if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
            {
                retVal += CSL_bcdmaDisableTxChan(
                            &drvHandle->bcdmaRegs,
                            chHandle->extChNum + drvHandle->extChOffset);
            }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
            if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
            {
                retVal += CSL_pktdmaDisableTxChan(
                            &drvHandle->pktdmaRegs,
                            chHandle->extChNum + drvHandle->extChOffset);

            }
#endif
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] UDMA UTC disable failed!!\n");
            }
        }
    }
    else
    {
        /*
         * Initiate teardown - External channel is special - should be done
         * as per below only
         */
        /* Use src thread instead of dest thread
         * (already has bit info CSL_PSILCFG_DEST_THREAD_OFFSET,
         * so just reset the bit) */
        Udma_assert(drvHandle, (UDMA_THREAD_ID_INVALID != chHandle->peerThreadId));
        srcThreadId = chHandle->peerThreadId &
                            ~((uint32_t) CSL_PSILCFG_DEST_THREAD_OFFSET);
        retVal = Udma_psilcfgWrite(
                     drvHandle,
                     srcThreadId,
                     CSL_PSILCFG_REG_ENABLE,
                     CSL_PSILCFG_REG_ENABLE_TEARDOWN_MASK);
        if(UDMA_SOK != retVal)
        {
            Udma_printf(drvHandle, "[Error] PSIL UTC teardown failed!!\n");
        }

        /* Disable External channel - this will clear the Enable bit */
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
        {
            retVal += CSL_udmapDisableTxChan(
                        &drvHandle->udmapRegs,
                        chHandle->extChNum + drvHandle->extChOffset);
        }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
        {
            retVal += CSL_bcdmaDisableTxChan(
                        &drvHandle->bcdmaRegs,
                        chHandle->extChNum + drvHandle->extChOffset);
        }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
        if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
        {
            retVal += CSL_pktdmaDisableTxChan(
                        &drvHandle->pktdmaRegs,
                        chHandle->extChNum + drvHandle->extChOffset);

        }
#endif
        if(CSL_PASS != retVal)
        {
            Udma_printf(drvHandle, "[Error] UDMA UTC disable failed!!\n");
        }
    }

    return (retVal);
}
#endif

#if (UDMA_NUM_UTC_INSTANCE > 0)
static int32_t Udma_psilcfgSetRtEnable(Udma_DrvHandle drvHandle,
                                       uint32_t threadId,
                                       uint32_t bEnable)
{
    uint32_t    regVal;
    int32_t     retVal;

    retVal = Udma_psilcfgRead(
                 drvHandle, threadId, CSL_PSILCFG_REG_RT_ENABLE, &regVal);
    if(UDMA_SOK == retVal)
    {
        uint32_t fieldVal;

        if(UTRUE == bEnable)
        {
            fieldVal = 1U;
        }
        else
        {
            fieldVal = 0U;
        }
        CSL_FINS(regVal, PSILCFG_REG_RT_ENABLE_ENABLE, fieldVal);
        /* Enable/disable realtime thread */
        retVal = Udma_psilcfgWrite(
                     drvHandle, threadId, CSL_PSILCFG_REG_RT_ENABLE, regVal);
    }

    return (retVal);
}

static int32_t Udma_psilcfgSetEnable(Udma_DrvHandle drvHandle,
                                     uint32_t threadId,
                                     uint32_t bEnable)
{
    uint32_t    regVal;
    int32_t     retVal;

    retVal = Udma_psilcfgRead(
                 drvHandle, threadId, CSL_PSILCFG_REG_ENABLE, &regVal);
    if(UDMA_SOK == retVal)
    {
        uint32_t fieldVal;

        if(UTRUE == bEnable)
        {
            fieldVal = 1U;
        }
        else
        {
            fieldVal = 0U;
        }
        CSL_FINS( regVal, PSILCFG_REG_ENABLE_ENABLE, fieldVal);
        /* Enable/disable thread */
        retVal = Udma_psilcfgWrite(
                     drvHandle, threadId, CSL_PSILCFG_REG_ENABLE, regVal);
    }

    return (retVal);
}

static int32_t Udma_psilcfgWrite(Udma_DrvHandle drvHandle,
                                 uint32_t threadId,
                                 uint32_t regId,
                                 uint32_t data)
{
    int32_t                             retVal;
    struct tisci_msg_rm_psil_write_req  rmPsilWrReq;

    rmPsilWrReq.valid_params = 0xFFFFU;         /* Not used */
    rmPsilWrReq.nav_id       = drvHandle->devIdPsil;
    rmPsilWrReq.thread       = (uint16_t)threadId;
    rmPsilWrReq.taddr        = (uint16_t)regId;
    rmPsilWrReq.data         = data;
    retVal = Sciclient_rmPsilWrite(&rmPsilWrReq, UDMA_SCICLIENT_TIMEOUT);
    if(CSL_PASS != retVal)
    {
        Udma_printf(drvHandle, "[Error] RM PSIL write failed!!!\n");
    }

    return (retVal);
}

static int32_t Udma_psilcfgRead(Udma_DrvHandle drvHandle,
                                uint32_t threadId,
                                uint32_t regId,
                                uint32_t *pData)
{
    int32_t                             retVal;
    struct tisci_msg_rm_psil_read_req   rmPsilRdReq;
    struct tisci_msg_rm_psil_read_resp  rmPsilRdResp;

    rmPsilRdReq.valid_params = 0xFFFFU;         /* Not used */
    rmPsilRdReq.nav_id       = drvHandle->devIdPsil;
    rmPsilRdReq.thread       = (uint16_t)threadId;
    rmPsilRdReq.taddr        = (uint16_t)regId;
    retVal = Sciclient_rmPsilRead(&rmPsilRdReq, &rmPsilRdResp, UDMA_SCICLIENT_TIMEOUT);
    if(CSL_PASS != retVal)
    {
        Udma_printf(drvHandle, "[Error] RM PSIL read failed!!!\n");
    }
    else
    {
        *pData = rmPsilRdResp.data;
    }

    return (retVal);
}
#endif

static void Udma_chAssignRegOverlay(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle)
{
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
        if(UDMA_DMA_CH_INVALID != chHandle->txChNum)
        {
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->udmapRegs.pTxChanCfgRegs));
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->udmapRegs.pTxChanRtRegs));
            Udma_assert(drvHandle,
                chHandle->txChNum <
                    (sizeof(CSL_udmap_txcrtRegs) /
                        sizeof(CSL_udmap_txcrtRegs_chan)));
            chHandle->pTxCfgRegs =
                &drvHandle->udmapRegs.pTxChanCfgRegs->CHAN[chHandle->txChNum];
            chHandle->pTxRtRegs  =
                &drvHandle->udmapRegs.pTxChanRtRegs->CHAN[chHandle->txChNum];
        }
        if(UDMA_DMA_CH_INVALID != chHandle->rxChNum)
        {
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->udmapRegs.pRxChanCfgRegs));
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->udmapRegs.pRxChanRtRegs));
            Udma_assert(drvHandle,
                chHandle->rxChNum <
                    (sizeof(CSL_udmap_rxcrtRegs) /
                        sizeof(CSL_udmap_rxcrtRegs_chan)));
            chHandle->pRxCfgRegs =
                &drvHandle->udmapRegs.pRxChanCfgRegs->CHAN[chHandle->rxChNum];
            chHandle->pRxRtRegs  =
                &drvHandle->udmapRegs.pRxChanRtRegs->CHAN[chHandle->rxChNum];
        }
        if(UDMA_DMA_CH_INVALID != chHandle->extChNum)
        {
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->udmapRegs.pTxChanCfgRegs));
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->udmapRegs.pTxChanRtRegs));
            Udma_assert(drvHandle,
                (chHandle->extChNum + drvHandle->extChOffset) <
                    (sizeof(CSL_udmap_txcrtRegs) /
                        sizeof(CSL_udmap_txcrtRegs_chan)));
            chHandle->pExtCfgRegs =
                &drvHandle->udmapRegs.pTxChanCfgRegs->CHAN[
                                chHandle->extChNum + drvHandle->extChOffset];
            chHandle->pExtRtRegs  =
                &drvHandle->udmapRegs.pTxChanRtRegs->CHAN[
                                chHandle->extChNum + drvHandle->extChOffset];
        }
    }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
    {
        if(UDMA_DMA_CH_INVALID != chHandle->txChNum)
        {
            if(UDMA_CH_FLAG_BLK_COPY == (chHandle->chType & UDMA_CH_FLAG_BLK_COPY))
            {
                Udma_assert(drvHandle, (NULL_PTR != drvHandle->bcdmaRegs.pBcChanCfgRegs));
                Udma_assert(drvHandle, (NULL_PTR != drvHandle->bcdmaRegs.pBcChanRtRegs));
                Udma_assert(drvHandle,
                    chHandle->txChNum <
                        (sizeof(CSL_bcdma_bcrtRegs) /
                            sizeof(CSL_bcdma_bcrtRegs_chan)));
                chHandle->pBcdmaBcCfgRegs =
                    &drvHandle->bcdmaRegs.pBcChanCfgRegs->CHAN[chHandle->txChNum];
                chHandle->pBcdmaBcRtRegs  =
                    &drvHandle->bcdmaRegs.pBcChanRtRegs->CHAN[chHandle->txChNum];
            }
            else
            {
                Udma_assert(drvHandle, (NULL_PTR != drvHandle->bcdmaRegs.pTxChanCfgRegs));
                Udma_assert(drvHandle, (NULL_PTR != drvHandle->bcdmaRegs.pTxChanRtRegs));
                Udma_assert(drvHandle,
                    chHandle->txChNum <
                        (sizeof(CSL_bcdma_txcrtRegs) /
                            sizeof(CSL_bcdma_txcrtRegs_chan)));
                chHandle->pBcdmaTxCfgRegs =
                    &drvHandle->bcdmaRegs.pTxChanCfgRegs->CHAN[chHandle->txChNum];
                chHandle->pBcdmaTxRtRegs  =
                    &drvHandle->bcdmaRegs.pTxChanRtRegs->CHAN[chHandle->txChNum];
            }
        }
        if(UDMA_DMA_CH_INVALID != chHandle->rxChNum)
        {
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->bcdmaRegs.pRxChanCfgRegs));
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->bcdmaRegs.pRxChanRtRegs));
            Udma_assert(drvHandle,
                chHandle->rxChNum <
                    (sizeof(CSL_bcdma_rxcrtRegs) /
                        sizeof(CSL_bcdma_rxcrtRegs_chan)));
            chHandle->pBcdmaRxCfgRegs =
                &drvHandle->bcdmaRegs.pRxChanCfgRegs->CHAN[chHandle->rxChNum];
            chHandle->pBcdmaRxRtRegs  =
                &drvHandle->bcdmaRegs.pRxChanRtRegs->CHAN[chHandle->rxChNum];
        }
    }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
    {
        if(UDMA_DMA_CH_INVALID != chHandle->txChNum)
        {
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->pktdmaRegs.pTxChanCfgRegs));
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->pktdmaRegs.pTxChanRtRegs));
            Udma_assert(drvHandle,
                chHandle->txChNum <
                    (sizeof(CSL_pktdma_txcrtRegs) /
                        sizeof(CSL_pktdma_txcrtRegs_chan)));
            chHandle->pPktdmaTxCfgRegs =
                &drvHandle->pktdmaRegs.pTxChanCfgRegs->CHAN[chHandle->txChNum];
            chHandle->pPktdmaTxRtRegs  =
                &drvHandle->pktdmaRegs.pTxChanRtRegs->CHAN[chHandle->txChNum];
        }
        if(UDMA_DMA_CH_INVALID != chHandle->rxChNum)
        {
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->pktdmaRegs.pRxChanCfgRegs));
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->pktdmaRegs.pRxChanRtRegs));
            Udma_assert(drvHandle,
                chHandle->rxChNum <
                    (sizeof(CSL_pktdma_rxcrtRegs) /
                        sizeof(CSL_pktdma_rxcrtRegs_chan)));
            chHandle->pPktdmaRxCfgRegs =
                &drvHandle->pktdmaRegs.pRxChanCfgRegs->CHAN[chHandle->rxChNum];
            chHandle->pPktdmaRxRtRegs  =
                &drvHandle->pktdmaRegs.pRxChanRtRegs->CHAN[chHandle->rxChNum];
        }
        if(UDMA_DMA_CH_INVALID != chHandle->extChNum)
        {
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->pktdmaRegs.pTxChanCfgRegs));
            Udma_assert(drvHandle, (NULL_PTR != drvHandle->pktdmaRegs.pTxChanRtRegs));
            Udma_assert(drvHandle,
                (chHandle->extChNum + drvHandle->extChOffset) <
                    (sizeof(CSL_pktdma_txcrtRegs) /
                        sizeof(CSL_pktdma_txcrtRegs_chan)));
            chHandle->pPktdmaExtCfgRegs =
                &drvHandle->pktdmaRegs.pTxChanCfgRegs->CHAN[
                                chHandle->extChNum + drvHandle->extChOffset];
            chHandle->pPktdmaExtRtRegs  =
                &drvHandle->pktdmaRegs.pTxChanRtRegs->CHAN[
                                chHandle->extChNum + drvHandle->extChOffset];
        }
    }
#endif
}

static void Udma_chInitRegs(Udma_ChHandle chHandle)
{
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    chHandle->pTxCfgRegs        = (volatile CSL_udmap_txccfgRegs_chan *) NULL_PTR;
    chHandle->pTxRtRegs         = (volatile CSL_udmap_txcrtRegs_chan *) NULL_PTR;
    chHandle->pRxCfgRegs        = (volatile CSL_udmap_rxccfgRegs_chan *) NULL_PTR;
    chHandle->pRxRtRegs         = (volatile CSL_udmap_rxcrtRegs_chan *) NULL_PTR;
    chHandle->pExtCfgRegs       = (volatile CSL_udmap_txccfgRegs_chan *) NULL_PTR;
    chHandle->pExtRtRegs        = (volatile CSL_udmap_txcrtRegs_chan *) NULL_PTR;
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    chHandle->pBcdmaBcCfgRegs    = (volatile CSL_bcdma_bccfgRegs_chan *) NULL_PTR;
    chHandle->pBcdmaBcRtRegs     = (volatile CSL_bcdma_bcrtRegs_chan *) NULL_PTR;
    chHandle->pBcdmaTxCfgRegs   = (volatile CSL_bcdma_txccfgRegs_chan *) NULL_PTR;
    chHandle->pBcdmaTxRtRegs    = (volatile CSL_bcdma_txcrtRegs_chan *) NULL_PTR;
    chHandle->pBcdmaRxCfgRegs   = (volatile CSL_bcdma_rxccfgRegs_chan *) NULL_PTR;
    chHandle->pBcdmaRxRtRegs    = (volatile CSL_bcdma_rxcrtRegs_chan *) NULL_PTR;
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    chHandle->pPktdmaTxCfgRegs  = (volatile CSL_pktdma_txccfgRegs_chan *) NULL_PTR;
    chHandle->pPktdmaTxRtRegs   = (volatile CSL_pktdma_txcrtRegs_chan *) NULL_PTR;
    chHandle->pPktdmaRxCfgRegs  = (volatile CSL_pktdma_rxccfgRegs_chan *) NULL_PTR;
    chHandle->pPktdmaRxRtRegs   = (volatile CSL_pktdma_rxcrtRegs_chan *) NULL_PTR;
    chHandle->pPktdmaExtCfgRegs = (volatile CSL_pktdma_txccfgRegs_chan *) NULL_PTR;
    chHandle->pPktdmaExtRtRegs  = (volatile CSL_pktdma_txcrtRegs_chan *) NULL_PTR;
#endif
}


static void Udma_chPauseTxLocal(Udma_DrvHandle drvHandle, uint32_t txChNum,uint32_t chType)
{
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
        (void) CSL_udmapPauseTxChan(&drvHandle->udmapRegs, txChNum);
    }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
    {
        if(UDMA_CH_FLAG_BLK_COPY != (chType & UDMA_CH_FLAG_BLK_COPY))
        {
            /*Add offset to chNum, so that BCDMA can identify it as Tx Channel*/
            (void) CSL_bcdmaPauseTxChan(&drvHandle->bcdmaRegs, (txChNum + drvHandle->txChOffset));
        }
        else
        {
            (void) CSL_bcdmaPauseTxChan(&drvHandle->bcdmaRegs, txChNum);
        }
    }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
    {
        (void) CSL_pktdmaPauseTxChan(&drvHandle->pktdmaRegs, txChNum);
    }
#endif
}

static void Udma_chUnpauseTxLocal(Udma_DrvHandle drvHandle, uint32_t txChNum, uint32_t chType)
{
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
        (void) CSL_udmapUnpauseTxChan(&drvHandle->udmapRegs, txChNum);
    }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
    {
        if(UDMA_CH_FLAG_BLK_COPY != (chType & UDMA_CH_FLAG_BLK_COPY))
        {
            /*Add offset to chNum, so that BCDMA can identify it as Tx Channel*/
            (void) CSL_bcdmaUnpauseTxChan(&drvHandle->bcdmaRegs, (txChNum + drvHandle->txChOffset));
        }
        else
        {
            (void) CSL_bcdmaUnpauseTxChan(&drvHandle->bcdmaRegs, txChNum);
        }
    }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
    {
        (void) CSL_pktdmaUnpauseTxChan(&drvHandle->pktdmaRegs, txChNum);
    }
#endif
}

static void Udma_chPauseRxLocal(Udma_DrvHandle drvHandle, uint32_t rxChNum)
{
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
        (void) CSL_udmapPauseRxChan(&drvHandle->udmapRegs, rxChNum);
    }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
    {
        /*Add offset to chNum, so that BCDMA can identify it as Rx Channel*/
        (void) CSL_bcdmaPauseRxChan(&drvHandle->bcdmaRegs + drvHandle->rxChOffset, rxChNum);
    }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
    {
        (void) CSL_pktdmaPauseRxChan(&drvHandle->pktdmaRegs, rxChNum);
    }
#endif
}

static void Udma_chUnpauseRxLocal(Udma_DrvHandle drvHandle, uint32_t rxChNum)
{
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
        (void) CSL_udmapUnpauseRxChan(&drvHandle->udmapRegs, rxChNum);
    }
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_BCDMA == drvHandle->instType)
    {
        /*Add offset to chNum, so that BCDMA can identify it as Rx Channel*/
        (void) CSL_bcdmaUnpauseRxChan(&drvHandle->bcdmaRegs + drvHandle->rxChOffset, rxChNum);
    }
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    if(UDMA_INST_TYPE_LCDMA_PKTDMA == drvHandle->instType)
    {
        (void) CSL_pktdmaUnpauseRxChan(&drvHandle->pktdmaRegs, rxChNum);
    }
#endif
}

static void Udma_chSetPeerReg(Udma_DrvHandle drvHandle,
                              const Udma_ChPdmaPrms *pdmaPrms,
                              volatile uint32_t *PEER8,
                              volatile uint32_t *PEER1,
                              volatile uint32_t *PEER0)
{
    uint32_t        regVal;

    Udma_assert(drvHandle, (NULL_PTR != PEER8));
    regVal = CSL_REG32_RD(PEER8);
    CSL_FINS(regVal, PSILCFG_REG_RT_ENABLE_ENABLE, (uint32_t) 0U);
    CSL_REG32_WR(PEER8, regVal);

    Udma_assert(drvHandle, (NULL_PTR != PEER0));
    regVal = (uint32_t)CSL_FMK(PSILCFG_REG_STATIC_TR_X, pdmaPrms->elemSize) |
              (uint32_t)CSL_FMK(PSILCFG_REG_STATIC_TR_Y, pdmaPrms->elemCnt);
    CSL_REG32_WR(PEER0, regVal);

    Udma_assert(drvHandle, (NULL_PTR != PEER1));
    regVal = (uint32_t)CSL_FMK(PSILCFG_REG_STATIC_TR_Z, pdmaPrms->fifoCnt);
    CSL_REG32_WR(PEER1, regVal);
}
