/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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

/*!
 * \file  multi_channel_dataflow.c
 *
 * \brief This file contains the implementation of the APIs for data flow for multi-channel example.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "multi_channel_common.h"
#include "multi_channel_dataflow.h"
#include "ti_enet_config.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */




void EnetApp_rxIsrFxn(void *appData)
{
    EnetApp_PerCtxt *perCtxt = (EnetApp_PerCtxt *)appData;

    SemaphoreP_post(&perCtxt->rxSemObj);
}

void EnetApp_rxIsrFxnPtp(void *appData)
{
    EnetApp_PerCtxt *perCtxt = (EnetApp_PerCtxt *)appData;

    SemaphoreP_post(&perCtxt->rxPtpSemObj);
}

static void EnetApp_addMCastEntry(Enet_Handle hEnet,
                                  uint32_t coreId,
                                  const uint8_t *testMCastAddr,
                                  uint32_t portMask)
{
    Enet_IoctlPrms prms;
    int32_t status;
    CpswAle_SetMcastEntryInArgs setMcastInArgs;
    uint32_t setMcastOutArgs;

    memset(&setMcastInArgs, 0, sizeof(setMcastInArgs));
    memcpy(&setMcastInArgs.addr.addr[0U], testMCastAddr,
           sizeof(setMcastInArgs.addr.addr));
    setMcastInArgs.addr.vlanId  = 0;
    setMcastInArgs.info.super = false;
    setMcastInArgs.info.numIgnBits = 0;
    setMcastInArgs.info.fwdState = CPSW_ALE_FWDSTLVL_FWD;
    setMcastInArgs.info.portMask = portMask;
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setMcastInArgs, &setMcastOutArgs);
    ENET_IOCTL(hEnet, coreId, CPSW_ALE_IOCTL_ADD_MCAST, &prms, status);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestBcastMcastLimit_AddAleEntry() failed CPSW_ALE_IOCTL_ADD_MCAST: %d\n",
                           status);
    }
}

int32_t EnetApp_openDma(EnetApp_PerCtxt *perCtxt)
{
    EnetApp_GetDmaHandleInArgs     txRegularInArgs;
    EnetApp_GetTxDmaHandleOutArgs  txRegularChInfo; 
    EnetApp_GetDmaHandleInArgs     txPtpInArgs;
    EnetApp_GetTxDmaHandleOutArgs  txPtpChInfo; 
    EnetApp_GetDmaHandleInArgs     rxRegularInArgs;
    EnetApp_GetRxDmaHandleOutArgs  rxRegularChInfo; 
    EnetApp_GetDmaHandleInArgs     rxPtpInArgs;
    EnetApp_GetRxDmaHandleOutArgs  rxPtpChInfo; 
    int32_t status = ENET_SOK;

    txRegularInArgs.cbArg    = NULL;
    txRegularInArgs.notifyCb = NULL;
    EnetApp_getTxDmaHandle(ENET_DMA_TX_CH0,
                           &txRegularInArgs,
                           &txRegularChInfo);

    perCtxt->txChNum = txRegularChInfo.txChNum;
    perCtxt->hTxCh   = txRegularChInfo.hTxCh;

	/* Allocate TX packets and keep them locally enqueued */
    EnetApp_initTxFreePktQ(&gEnetApp.txFreePktInfoQ, ENET_DMA_TX_CH0);

    if (perCtxt->hTxCh == NULL)
    {
        EnetAppUtils_print("EnetApp_openDma() failed to open TX channel\r\n");
        status = ENET_EFAIL;
        EnetAppUtils_assert(perCtxt->hTxCh != NULL);
    }
	else
	{
		status = EnetDma_enableTxEvent(perCtxt->hTxCh);
	}
	
	if (status == ENET_SOK)
    {
        /* Open the TX channel for PTP Traffic */
        txPtpInArgs.cbArg    = NULL;
        txPtpInArgs.notifyCb = NULL;
	    
        EnetApp_getTxDmaHandle(ENET_DMA_TX_CH_PTP,
                               &txPtpInArgs,
                               &txPtpChInfo);
        
        perCtxt->txPtpChNum = txPtpChInfo.txChNum;
        perCtxt->hTxPtpCh   = txPtpChInfo.hTxCh;
        
        
	    /* Allocate TX packets and keep them locally enqueued */
        EnetApp_initTxFreePktQ(&gEnetApp.txPtpFreePktInfoQ, ENET_DMA_TX_CH_PTP);

        if (perCtxt->hTxPtpCh == NULL)
        {
            EnetAppUtils_print("EnetApp_openDma() failed to open TX channel\r\n");
            status = ENET_EFAIL;
            EnetAppUtils_assert(perCtxt->hTxPtpCh != NULL);
        }
	    else
	    {
	    	status = EnetDma_enableTxEvent(perCtxt->hTxPtpCh);
	    }
    }
    

    /*Opening RX  flow for frames*/
    if (status == ENET_SOK)
    {
        rxRegularInArgs.notifyCb = EnetApp_rxIsrFxn;
        rxRegularInArgs.cbArg = perCtxt;

        EnetApp_getRxDmaHandle(ENET_DMA_RX_CH0,
                              &rxRegularInArgs,
                              &rxRegularChInfo);

 
        perCtxt->rxChNum = rxRegularChInfo.rxChNum;
        perCtxt->hRxCh   = rxRegularChInfo.hRxCh;
        EnetAppUtils_assert(rxRegularChInfo.macAddressValid == true);
        EnetUtils_copyMacAddr(perCtxt->macAddr, rxRegularChInfo.macAddr);

        if (perCtxt->hRxCh == NULL)
        {
            EnetAppUtils_print("EnetApp_openRxCh() failed to open RX flow\r\n");
            status = ENET_EFAIL;
            EnetAppUtils_assert(perCtxt->hRxPtpCh != NULL);
        }
		else
		{
            /* Submit all ready RX buffers to DMA.*/
            EnetApp_initRxReadyPktQ(perCtxt->hRxCh, ENET_DMA_RX_CH0);
		}
    }
	

    /*Opening RX PTP flow for frames*/
    if (status == ENET_SOK)
    {
        rxPtpInArgs.notifyCb = EnetApp_rxIsrFxnPtp;
        rxPtpInArgs.cbArg = perCtxt;
		
        EnetApp_getRxDmaHandle(ENET_DMA_RX_CH_PTP,
                              &rxPtpInArgs,
                              &rxPtpChInfo);

        perCtxt->rxPtpChNum = rxPtpChInfo.rxChNum;
        perCtxt->hRxPtpCh   = rxPtpChInfo.hRxCh;

        EnetAppUtils_assert(rxPtpChInfo.macAddressValid == false);

        if (perCtxt->hRxPtpCh == NULL)
        {
            EnetAppUtils_print("EnetApp_openRxCh() failed to open RX flow\r\n");
            status = ENET_EFAIL;
            EnetAppUtils_assert(perCtxt->hRxPtpCh != NULL);
        }
		else
		{
            /* Submit all ready RX buffers to DMA.*/
            EnetApp_initRxReadyPktQ(perCtxt->hRxPtpCh, ENET_DMA_RX_CH_PTP);
		}
    }

     return status;
}

void EnetApp_closeDma(EnetApp_PerCtxt *perCtxt)
{
    EnetDma_PktQ fqPktInfoQ;
    EnetDma_PktQ cqPktInfoQ;

    /* Close RX channel */
    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    EnetApp_closeRxDma(ENET_DMA_RX_CH0,
                       perCtxt->hEnet,
                       perCtxt->coreKey,
                       gEnetApp.coreId,
                       &fqPktInfoQ,
                       &cqPktInfoQ);

    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);

    /* Close RX PTP channel */
    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

	EnetApp_closeRxDma(ENET_DMA_RX_CH_PTP,
                       perCtxt->hEnet,
                       perCtxt->coreKey,
                       gEnetApp.coreId,
                       &fqPktInfoQ,
                       &cqPktInfoQ);


    /* Close TX channel */
    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    /* Retrieve any pending TX packets from driver */
    EnetApp_retrieveFreeTxPkts(perCtxt->hTxCh, &gEnetApp.txFreePktInfoQ);

    EnetApp_closeTxDma(ENET_DMA_TX_CH0,
                       perCtxt->hEnet,
                       perCtxt->coreKey,
                       gEnetApp.coreId,
                       &fqPktInfoQ,
                       &cqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&gEnetApp.txFreePktInfoQ);
	
	/* Close PTP TX channel */
    EnetQueue_initQ(&fqPktInfoQ);
    EnetQueue_initQ(&cqPktInfoQ);

    /* Retrieve any pending TX packets from driver */
    EnetApp_retrieveFreeTxPkts(perCtxt->hTxPtpCh, &gEnetApp.txPtpFreePktInfoQ);

    EnetApp_closeTxDma(ENET_DMA_TX_CH_PTP,
                       perCtxt->hEnet,
                       perCtxt->coreKey,
                       gEnetApp.coreId,
                       &fqPktInfoQ,
                       &cqPktInfoQ);

    EnetAppUtils_freePktInfoQ(&fqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&cqPktInfoQ);
    EnetAppUtils_freePktInfoQ(&gEnetApp.txPtpFreePktInfoQ);
}

void EnetApp_initTxFreePktQ(EnetDma_PktQ *freePktInfoQ, uint32_t txCh)
{
    EnetDma_Pkt *pPktInfo;
    uint32_t i;
    const uint32_t txChNumPkts[ENET_SYSCFG_TX_CHANNELS_NUM] = 
                              {
                                  [ENET_DMA_TX_CH0] = ENET_DMA_TX_CH0_NUM_PKTS, 
                                  [ENET_DMA_TX_CH_PTP] =  ENET_DMA_TX_CH_PTP_NUM_PKTS 
                              };

    /* Initialize TX EthPkts and queue them to freePktInfoQ */
    for (i = 0U; i < txChNumPkts[txCh]; i++)
    {
        pPktInfo = EnetMem_allocEthPkt(&gEnetApp,
                                       ENET_MEM_LARGE_POOL_PKT_SIZE,
                                       ENETDMA_CACHELINE_ALIGNMENT);
        EnetAppUtils_assert(pPktInfo != NULL);
        ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState, ENET_PKTSTATE_APP_WITH_FREEQ);

        EnetQueue_enq(freePktInfoQ, &pPktInfo->node);
    }

    EnetAppUtils_print("initQs() freePktInfoQ initialized with %d pkts\r\n",
                       EnetQueue_getQCount(freePktInfoQ));
}

void EnetApp_initRxReadyPktQ(EnetDma_RxChHandle hRxCh, uint32_t rxCh)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_PktQ rxFreeQ;
    EnetDma_Pkt *pPktInfo;
    uint32_t i;
    int32_t status;
    const uint32_t rxChNumPkts[ENET_SYSCFG_RX_FLOWS_NUM] = 
                              {
                                  [ENET_DMA_RX_CH0]    = ENET_DMA_RX_CH0_NUM_PKTS, 
                                  [ENET_DMA_RX_CH_PTP] =  ENET_DMA_RX_CH_PTP_NUM_PKTS 
                              };


    EnetQueue_initQ(&rxFreeQ);

    for (i = 0U; i < rxChNumPkts[i]; i++)
    {
        pPktInfo = EnetMem_allocEthPkt(&gEnetApp,
                                       ENET_MEM_LARGE_POOL_PKT_SIZE,
                                       ENETDMA_CACHELINE_ALIGNMENT);
        EnetAppUtils_assert(pPktInfo != NULL);

        ENET_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState, ENET_PKTSTATE_APP_WITH_FREEQ);

        EnetQueue_enq(&rxFreeQ, &pPktInfo->node);
    }

    /* Retrieve any packets which are ready */
    EnetQueue_initQ(&rxReadyQ);
    status = EnetDma_retrieveRxPktQ(hRxCh, &rxReadyQ);
    EnetAppUtils_assert(status == ENET_SOK);

    /* There should not be any packet with DMA during init */
    EnetAppUtils_assert(EnetQueue_getQCount(&rxReadyQ) == 0U);

    EnetAppUtils_validatePacketState(&rxFreeQ,
                                     ENET_PKTSTATE_APP_WITH_FREEQ,
                                     ENET_PKTSTATE_APP_WITH_DRIVER);

    EnetDma_submitRxPktQ(hRxCh, &rxFreeQ);

    /* Assert here, as during init, the number of DMA descriptors should be equal to
     * the number of free Ethernet buffers available with app */
    EnetAppUtils_assert(EnetQueue_getQCount(&rxFreeQ) == 0U);
}

uint32_t EnetApp_retrieveFreeTxPkts(EnetDma_TxChHandle hTxCh, EnetDma_PktQ *txPktInfoQ)
{
    EnetDma_PktQ txFreeQ;
    EnetDma_Pkt *pktInfo;
    uint32_t txFreeQCnt = 0U;
    int32_t status;

    EnetQueue_initQ(&txFreeQ);

    /* Retrieve any packets that may be free now */
    status = EnetDma_retrieveTxPktQ(hTxCh, &txFreeQ);
    if (status == ENET_SOK)
    {
        txFreeQCnt = EnetQueue_getQCount(&txFreeQ);

        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFreeQ);
        while (NULL != pktInfo)
        {
            EnetDma_checkPktState(&pktInfo->pktState,
                                    ENET_PKTSTATE_MODULE_APP,
                                    ENET_PKTSTATE_APP_WITH_DRIVER,
                                    ENET_PKTSTATE_APP_WITH_FREEQ);

            EnetQueue_enq(txPktInfoQ, &pktInfo->node);
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFreeQ);
        }
    }
    else
    {
        EnetAppUtils_print("retrieveFreeTxPkts() failed to retrieve pkts: %d\r\n", status);
    }

    return txFreeQCnt;
}

void EnetApp_createRxTask(EnetApp_PerCtxt *perCtxt)
{
    TaskP_Params taskParams;
    TaskP_Params taskParamsPTP;
    int32_t status;
    status = SemaphoreP_constructBinary(&perCtxt->rxSemObj, 0);
    DebugP_assert(SystemP_SUCCESS == status);

    status = SemaphoreP_constructCounting(&perCtxt->rxDoneSemObj, 0, COUNTING_SEM_COUNT);
    DebugP_assert(SystemP_SUCCESS == status);

    status = SemaphoreP_constructBinary(&perCtxt->rxPtpSemObj, 0);
    DebugP_assert(SystemP_SUCCESS == status);

    status = SemaphoreP_constructCounting(&perCtxt->rxPtpDoneSemObj, 0, COUNTING_SEM_COUNT);
    DebugP_assert(SystemP_SUCCESS == status);

    TaskP_Params_init(&taskParams);
    taskParams.priority       = 5U;
    taskParams.stack          = gEnetAppTaskStackRx;
    taskParams.stackSize      = sizeof(gEnetAppTaskStackRx);
    taskParams.args           = (void*)perCtxt;
    taskParams.name           = "Rx Task";
    taskParams.taskMain           = &EnetApp_rxTask;

    status = TaskP_construct(&perCtxt->rxTaskObj, &taskParams);
    DebugP_assert(SystemP_SUCCESS == status);

    TaskP_Params_init(&taskParamsPTP);
    taskParamsPTP.priority       = 5U;
    taskParamsPTP.stack          = gEnetAppTaskStackRXPtp;
    taskParamsPTP.stackSize      = sizeof(gEnetAppTaskStackRXPtp);
    taskParamsPTP.args           = (void*)perCtxt;
    taskParamsPTP.name           = "Rx Task PTP";
    taskParamsPTP.taskMain           = &EnetApp_rxTaskPTP;

    status = TaskP_construct(&perCtxt->rxPtpTaskObj, &taskParamsPTP);
    DebugP_assert(SystemP_SUCCESS == status);
}

void EnetApp_destroyRxTask(EnetApp_PerCtxt *perCtxt)
{
    SemaphoreP_destruct(&perCtxt->rxSemObj);
    SemaphoreP_destruct(&perCtxt->rxDoneSemObj);
    SemaphoreP_destruct(&perCtxt->rxPtpSemObj);
    SemaphoreP_destruct(&perCtxt->rxPtpDoneSemObj);
    TaskP_destruct(&perCtxt->rxTaskObj);
    TaskP_destruct(&perCtxt->rxPtpTaskObj);
}

void EnetApp_rxTask(void *args)
{
    EnetApp_PerCtxt *perCtxt = (EnetApp_PerCtxt *)args;
    EnetDma_PktQ rxReadyQ;
    EnetDma_PktQ rxFreeQ;
    EnetDma_PktQ txSubmitQ;
    EnetDma_Pkt *rxPktInfo;
    EnetDma_Pkt *txPktInfo;
    EthFrame *rxFrame;
    EthFrame *txFrame;
    uint32_t totalRxCnt = 0U;
    int32_t status = ENET_SOK;

    while ((ENET_SOK == status) && (gEnetApp.run))
    {
        /* Wait for packet reception */
        SemaphoreP_pend(&perCtxt->rxSemObj, SystemP_WAIT_FOREVER);

        /* All peripherals have single hardware RX channel, so we only need to retrieve
         * packets from a single flow.*/
        EnetQueue_initQ(&rxReadyQ);
        EnetQueue_initQ(&rxFreeQ);
        EnetQueue_initQ(&txSubmitQ);

        /* Get the packets received so far */
        status = EnetDma_retrieveRxPktQ(perCtxt->hRxCh, &rxReadyQ);
        if (status != ENET_SOK)
        {
            /* Should we bail out here? */
            EnetAppUtils_print("Failed to retrieve RX pkt queue: %d\r\n", status);
            continue;
        }
#if DEBUG
        EnetAppUtils_print("%s: Received %u packets\r\n", perCtxt->name, EnetQueue_getQCount(&rxReadyQ));
#endif
        totalRxCnt += EnetQueue_getQCount(&rxReadyQ);

        /* Consume the received packets and send them back */
        rxPktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxReadyQ);
        while (rxPktInfo != NULL)
        {
            rxFrame = (EthFrame *)rxPktInfo->bufPtr;
            EnetDma_checkPktState(&rxPktInfo->pktState,
                                    ENET_PKTSTATE_MODULE_APP,
                                    ENET_PKTSTATE_APP_WITH_DRIVER,
                                    ENET_PKTSTATE_APP_WITH_READYQ);

            /* Retrieve TX packets from driver and recycle them */
            EnetApp_retrieveFreeTxPkts(perCtxt->hTxCh, &gEnetApp.txFreePktInfoQ);

            /* Dequeue one free TX Eth packet */
            txPktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetApp.txFreePktInfoQ);
            if (txPktInfo != NULL)
            {
                /* Fill the TX Eth frame with test content */
                txFrame = (EthFrame *)txPktInfo->bufPtr;
                memcpy(txFrame->hdr.dstMac, rxFrame->hdr.srcMac, ENET_MAC_ADDR_LEN);
                memcpy(txFrame->hdr.srcMac, &perCtxt->macAddr[0U], ENET_MAC_ADDR_LEN);
                txFrame->hdr.etherType = rxFrame->hdr.etherType;

                memcpy(&txFrame->payload[0U],
                        &rxFrame->payload[0U],
                        rxPktInfo->bufPtrFilledLen - sizeof(EthFrameHeader));

                txPktInfo->bufPtrFilledLen = rxPktInfo->bufPtrFilledLen;
				txPktInfo->txTotalPktLen = rxPktInfo->bufPtrFilledLen;
                txPktInfo->sgList.numScatterSegments = 0;
                txPktInfo->appPriv = &gEnetApp;
                txPktInfo->tsInfo.enableHostTxTs = false;

                EnetDma_checkPktState(&txPktInfo->pktState,
                                        ENET_PKTSTATE_MODULE_APP,
                                        ENET_PKTSTATE_APP_WITH_FREEQ,
                                        ENET_PKTSTATE_APP_WITH_DRIVER);

                /* Enqueue the packet for later transmission */
                EnetQueue_enq(&txSubmitQ, &txPktInfo->node);
            }
            else
            {
                EnetAppUtils_print("%s: Drop due to TX pkt not available\r\n", perCtxt->name);
            }

            EnetDma_checkPktState(&rxPktInfo->pktState,
                                    ENET_PKTSTATE_MODULE_APP,
                                    ENET_PKTSTATE_APP_WITH_READYQ,
                                    ENET_PKTSTATE_APP_WITH_FREEQ);

            /* Release the received packet */
            EnetQueue_enq(&rxFreeQ, &rxPktInfo->node);
            rxPktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxReadyQ);
        }

        /* Transmit all enqueued packets */
        status = EnetDma_submitTxPktQ(perCtxt->hTxCh, &txSubmitQ);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("%s: Failed to submit TX pkt queue: %d\r\n", perCtxt->name, status);
        }

        EnetAppUtils_validatePacketState(&rxFreeQ,
                                            ENET_PKTSTATE_APP_WITH_FREEQ,
                                            ENET_PKTSTATE_APP_WITH_DRIVER);

        /* Submit now processed buffers */
        status = EnetDma_submitRxPktQ(perCtxt->hRxCh, &rxFreeQ);
        EnetAppUtils_assert(status == ENET_SOK);
        EnetAppUtils_assert(0U==EnetQueue_getQCount(&rxFreeQ));
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("%s: Failed to submit RX pkt queue: %d\r\n", perCtxt->name, status);
        }
    }

    EnetAppUtils_print("%s: Received %u packets\r\n", perCtxt->name, totalRxCnt);

    SemaphoreP_post(&perCtxt->rxDoneSemObj);
    TaskP_exit();
}

void EnetApp_getCurrentTime(EnetApp_PerCtxt *perCtxt,
                             uint32_t *nanoseconds,
                             uint64_t *seconds)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    uint64_t tsVal = 0U;

    if (perCtxt != NULL)
    {
        /* Software Time stamp Push event */
        ENET_IOCTL_SET_OUT_ARGS(&prms, &tsVal);
        ENET_IOCTL(perCtxt->hEnet,
                   gEnetApp.coreId,
                   ENET_TIMESYNC_IOCTL_GET_CURRENT_TIMESTAMP,
                   &prms,
                   status);
        EnetAppUtils_assert(status == ENET_SOK);

        *nanoseconds = (uint32_t)(tsVal % (uint64_t)TIME_SEC_TO_NS);
        *seconds = tsVal / (uint64_t)TIME_SEC_TO_NS;
    }
    else
    {
        status = ENET_ENOTFOUND;
        *nanoseconds = 0U;
        *seconds = 0U;
    }
}

void EnetApp_setPortTsEventPrms(CpswMacPort_TsEventCfg *tsPortEventCfg)
{
    tsPortEventCfg->commonPortIpCfg.ttlNonzeroEn = true;
    tsPortEventCfg->commonPortIpCfg.tsIp107En = false;
    tsPortEventCfg->commonPortIpCfg.tsIp129En = false;
    tsPortEventCfg->commonPortIpCfg.tsIp130En = false;
    tsPortEventCfg->commonPortIpCfg.tsIp131En = false;
    tsPortEventCfg->commonPortIpCfg.tsIp132En = false;
    tsPortEventCfg->commonPortIpCfg.tsPort319En = true;
    tsPortEventCfg->commonPortIpCfg.tsPort320En = true;
    tsPortEventCfg->commonPortIpCfg.unicastEn = false;
    tsPortEventCfg->domainOffset = 4U;
    tsPortEventCfg->ltype2En = false;
    tsPortEventCfg->rxAnnexDEn = true;
    tsPortEventCfg->rxAnnexEEn = true;
    tsPortEventCfg->rxAnnexFEn = true;
    tsPortEventCfg->txAnnexDEn = true;
    tsPortEventCfg->txAnnexEEn = true;
    tsPortEventCfg->txAnnexFEn = true;
    tsPortEventCfg->txHostTsEn = true;
    tsPortEventCfg->mcastType = 0U;
    tsPortEventCfg->messageType = 0xFFFFU;
    tsPortEventCfg->seqIdOffset = 30U;
    /* VLAN untagged */
    tsPortEventCfg->rxVlanType = ENET_MACPORT_VLAN_TYPE_SINGLE_TAG;
    tsPortEventCfg->txVlanType = ENET_MACPORT_VLAN_TYPE_SINGLE_TAG;
    tsPortEventCfg->vlanLType1 = 0x8100U;
    tsPortEventCfg->vlanLType2 = 0U;

}

int32_t EnetApp_getRxTimestamp(EnetApp_PerCtxt *perCtxt,
                                EnetTimeSync_MsgType rxFrameType,
                                uint8_t rxPort,
                                uint16_t seqId,
                                uint32_t *nanoseconds,
                                uint64_t *seconds)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    EnetTimeSync_GetEthTimestampInArgs inArgs;
    uint64_t tsVal;

    if (perCtxt != NULL)
    {
        inArgs.msgType = rxFrameType;
        inArgs.seqId   = seqId;
        inArgs.portNum = rxPort;
        inArgs.domain  = 0U;

        if (status == ENET_SOK)
        {
            ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &tsVal);
            ENET_IOCTL(perCtxt->hEnet,
                       gEnetApp.coreId,
                       ENET_TIMESYNC_IOCTL_GET_ETH_RX_TIMESTAMP,
                       &prms,
                       status);
            if (status == ENET_ENOTFOUND)
            {
                tsVal = 0U;
            }
            *nanoseconds = (uint32_t)(tsVal % (uint64_t)TIME_SEC_TO_NS);
            *seconds = tsVal / (uint64_t)TIME_SEC_TO_NS;
        }
    }
    else
    {
        status = ENET_ENOTFOUND;
    }

    return status;
}

int32_t EnetApp_getTxTimestamp(EnetApp_PerCtxt *perCtxt,
                                EnetTimeSync_MsgType txFrameType,
                                uint8_t txPort,
                                uint16_t seqId,
                                uint32_t *nanoseconds,
                                uint64_t *seconds)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    EnetTimeSync_GetEthTimestampInArgs inArgs;
    uint64_t tsVal;

    if (perCtxt != NULL)
    {
        inArgs.msgType = txFrameType;
        inArgs.seqId = seqId;
        inArgs.portNum = txPort;
        inArgs.domain  = 0U;

        if (status == ENET_SOK)
        {
            ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &tsVal);
            ENET_IOCTL(perCtxt->hEnet,
                       gEnetApp.coreId,
                       ENET_TIMESYNC_IOCTL_GET_ETH_TX_TIMESTAMP,
                       &prms,
                       status);
            if (status == ENET_ENOTFOUND)
            {
                tsVal = 0U;
            }
            *nanoseconds = (uint32_t)(tsVal % (uint64_t)TIME_SEC_TO_NS);
            *seconds = tsVal / (uint64_t)TIME_SEC_TO_NS;
        }
    }
    else
    {
        status = ENET_ENOTFOUND;
    }

    return status;
}

int32_t EnetApp_addPTPMcastAddr(EnetApp_PerCtxt *perCtxt)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    CpswAle_SetMcastEntryInArgs setMcastInArgs;
    uint32_t setMcastoutArgs;


    memset(&setMcastInArgs, 0, sizeof(setMcastInArgs));
    memcpy(&setMcastInArgs.addr.addr[0U],
                      &peerDlyMsgMAC[0U],
                      ENET_MAC_ADDR_LEN);
    setMcastInArgs.addr.vlanId  = 0;
    setMcastInArgs.info.super = false;
    setMcastInArgs.info.numIgnBits = 0;
    setMcastInArgs.info.fwdState = CPSW_ALE_FWDSTLVL_FWD;
    setMcastInArgs.info.portMask = 3U;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setMcastInArgs, &setMcastoutArgs);
    ENET_IOCTL(perCtxt->hEnet,
               gEnetApp.coreId,
               CPSW_ALE_IOCTL_ADD_MCAST,
               &prms,
               status);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("EnetApp_addPTPMcastAddr() failed CPSW_ALE_IOCTL_ADD_MCAST: %d\n",
                           status);
    }
	
    return status;
}

#if defined (SOC_AM263X)
int32_t EnetApp_setCpswAleClassifier(EnetApp_PerCtxt *perCtxt,
                                     uint32_t dfltThreadId)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    CpswAle_SetPolicerEntryOutArgs setPolicerEntryOutArgs;
    CpswAle_SetPolicerEntryInArgs setPolicerEntryInArgs;
	CpswAle_DfltThreadCfg dfltThreadCfg;

    setPolicerEntryInArgs.policerMatch.policerMatchEnMask = CPSW_ALE_POLICER_MATCH_ETHERTYPE;
    setPolicerEntryInArgs.policerMatch.etherType = PTP_ETHERTYPE;
    setPolicerEntryInArgs.threadIdEn = true;
    setPolicerEntryInArgs.threadId = perCtxt->rxPtpChNum;
    setPolicerEntryInArgs.peakRateInBitsPerSec = 0U;
    setPolicerEntryInArgs.commitRateInBitsPerSec = 0U;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerEntryInArgs, &setPolicerEntryOutArgs);
    ENET_IOCTL(perCtxt->hEnet,
               gEnetApp.coreId,
               CPSW_ALE_IOCTL_SET_POLICER,
               &prms,
               status);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("EnetApp_setCpswAleClassifier() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                        status);
    }
	
	if(status == ENET_SOK)
    {
        dfltThreadCfg.dfltThreadEn = true;
        dfltThreadCfg.threadId     = dfltThreadId;
        ENET_IOCTL_SET_IN_ARGS(&prms, &dfltThreadCfg);
        
        ENET_IOCTL(perCtxt->hEnet,
  	     	       gEnetApp.coreId,
  	     	       CPSW_ALE_IOCTL_SET_DEFAULT_THREADCFG,
  	     	       &prms,
                   status);
        if (status != ENET_SOK)
        {
  	      EnetAppUtils_print("EnetApp_setCpswAleClassifier() failed CPSW_ALE_IOCTL_SET_DEFAULT_THREADCFG: %d\n",
  	     				 status);
        }
	}
	
    return status;
}
#endif

void EnetApp_rxTaskPTP(void *args)
{
    EnetApp_PerCtxt *perCtxt = (EnetApp_PerCtxt *)args;
    EnetDma_PktQ rxReadyQ;
    EnetDma_PktQ rxFreeQ;
    EnetDma_PktQ txSubmitQ;
    EnetDma_Pkt *rxPktInfo;
    EnetDma_Pkt *txPktInfo;
    EthFrame *rxFrame;
    EthFrame *txFrame;
    uint32_t totalRxCnt = 0U;
    uint8_t pktType = 0;
    uint16_t seqId = 0U;
    uint32_t nanoSeconds = 0U;
    uint64_t seconds = 0U;
    uint64_t tsVal   = 0U;
    int32_t status = ENET_SOK;


    while ((ENET_SOK == status) && (gEnetApp.run))
    {
        /* Wait for packet reception */
        SemaphoreP_pend(&perCtxt->rxPtpSemObj, SystemP_WAIT_FOREVER);

        /* All peripherals have single hardware RX channel, so we only need to retrieve
         * packets from a single flow.*/
        EnetQueue_initQ(&rxReadyQ);
        EnetQueue_initQ(&rxFreeQ);
        EnetQueue_initQ(&txSubmitQ);

        /* Get the packets received so far */
        status = EnetDma_retrieveRxPktQ(perCtxt->hRxPtpCh, &rxReadyQ);
        if (status != ENET_SOK)
        {
            /* Should we bail out here? */
            EnetAppUtils_print("Failed to retrieve RX pkt queue: %d\r\n", status);
            continue;
        }
#if DEBUG
        EnetAppUtils_print("%s: Received %u packets\r\n", perCtxt->name, EnetQueue_getQCount(&rxReadyQ));
#endif
        totalRxCnt += EnetQueue_getQCount(&rxReadyQ);

        /* Consume the received packets and send them back */
        rxPktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxReadyQ);

        while (rxPktInfo != NULL)
        {
            rxFrame = (EthFrame *)rxPktInfo->bufPtr;
            EnetDma_checkPktState(&rxPktInfo->pktState,
                                    ENET_PKTSTATE_MODULE_APP,
                                    ENET_PKTSTATE_APP_WITH_DRIVER,
                                    ENET_PKTSTATE_APP_WITH_READYQ);

            if(status == ENET_SOK && gEnetApp.enableTs)
            {
                pktType = (uint8_t)(*((uint8_t *)rxFrame + PTP_MSG_ID_OFFSET));
                seqId = (uint16_t)(*(uint16_t *)((uint8_t *)rxFrame + PTP_SEQ_ID_OFFSET));
                seqId = Enet_htons(seqId);
                status = EnetApp_getRxTimestamp(perCtxt, (EnetTimeSync_MsgType)pktType, perCtxt->macPort, seqId, &nanoSeconds, &seconds);
                tsVal = (uint64_t)(((uint64_t)seconds * (uint64_t)TIME_SEC_TO_NS) + nanoSeconds);
                EnetAppUtils_print("RX PTP time is : %llu\r\n", tsVal);
            }

            /* Retrieve TX packets from driver and recycle them */
            EnetApp_retrieveFreeTxPkts(perCtxt->hTxPtpCh, &gEnetApp.txPtpFreePktInfoQ);

            /* Dequeue one free TX Eth packet */
            txPktInfo = (EnetDma_Pkt *)EnetQueue_deq(&gEnetApp.txPtpFreePktInfoQ);
            if (txPktInfo != NULL)
            {
                /* Fill the TX Eth frame with test content */
                txFrame = (EthFrame *)txPktInfo->bufPtr;
                memcpy(txFrame->hdr.dstMac, rxFrame->hdr.srcMac, ENET_MAC_ADDR_LEN);
                memcpy(txFrame->hdr.srcMac, &perCtxt->macAddr[0U], ENET_MAC_ADDR_LEN);
                txFrame->hdr.etherType = rxFrame->hdr.etherType;

                memcpy(&txFrame->payload[0U],
                        &rxFrame->payload[0U],
                        rxPktInfo->bufPtrFilledLen - sizeof(EthFrameHeader));

                txPktInfo->bufPtrFilledLen = rxPktInfo->bufPtrFilledLen;
                txPktInfo->txTotalPktLen = rxPktInfo->bufPtrFilledLen;
                txPktInfo->sgList.numScatterSegments = 0;
                txPktInfo->appPriv = &gEnetApp;
                txPktInfo->tsInfo.enableHostTxTs = false;

                EnetDma_checkPktState(&txPktInfo->pktState,
                                        ENET_PKTSTATE_MODULE_APP,
                                        ENET_PKTSTATE_APP_WITH_FREEQ,
                                        ENET_PKTSTATE_APP_WITH_DRIVER);

                /* Enqueue the packet for later transmission */
                EnetQueue_enq(&txSubmitQ, &txPktInfo->node);
            }
            else
            {
                EnetAppUtils_print("%s: Drop due to TX pkt not available\r\n", perCtxt->name);
            }

            EnetDma_checkPktState(&rxPktInfo->pktState,
                                    ENET_PKTSTATE_MODULE_APP,
                                    ENET_PKTSTATE_APP_WITH_READYQ,
                                    ENET_PKTSTATE_APP_WITH_FREEQ);

            /* Release the received packet */
            EnetQueue_enq(&rxFreeQ, &rxPktInfo->node);
            rxPktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxReadyQ);
        }

        /* Transmit all enqueued packets */
        status = EnetDma_submitTxPktQ(perCtxt->hTxPtpCh, &txSubmitQ);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("%s: Failed to submit TX pkt queue: %d\r\n", perCtxt->name, status);
        }

        EnetAppUtils_validatePacketState(&rxFreeQ,
                                            ENET_PKTSTATE_APP_WITH_FREEQ,
                                            ENET_PKTSTATE_APP_WITH_DRIVER);


        if(status == ENET_SOK && gEnetApp.enableTs)
        {
            status = EnetApp_getTxTimestamp(perCtxt, (EnetTimeSync_MsgType)pktType, perCtxt->macPort, seqId, &nanoSeconds, &seconds);
            tsVal = (uint64_t)(((uint64_t)seconds * (uint64_t)TIME_SEC_TO_NS) + nanoSeconds);
            EnetAppUtils_print("TX PTP time is : %llu\r\n", tsVal);
        }

        /* Submit now processed buffers */
        status = EnetDma_submitRxPktQ(perCtxt->hRxPtpCh, &rxFreeQ); 
        EnetAppUtils_assert(status == ENET_SOK);
        EnetAppUtils_assert(0U==EnetQueue_getQCount(&rxFreeQ));

        if (status != ENET_SOK)
        {
            EnetAppUtils_print("%s: Failed to submit RX pkt queue: %d\r\n", perCtxt->name, status);
        }

    }

    EnetAppUtils_print("%s: Received %u packets\r\n", perCtxt->name, totalRxCnt);

    SemaphoreP_post(&perCtxt->rxPtpDoneSemObj);
    TaskP_exit();
}
