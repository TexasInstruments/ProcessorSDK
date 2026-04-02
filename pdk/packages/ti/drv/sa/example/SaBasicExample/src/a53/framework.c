/*
 *
 * Copyright (C) 2010-2018 Texas Instruments Incorporated - http://www.ti.com/
 *
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
 *
*/

/* Generate and verify the system test framework
 *
 * The test framework consists of the pa and sa driver instance, a cppi/cdma/qm configuration,
 * memory for packet transmission and reception, and semaphores that are used
 * for every test in the SA unit test.
 *
 */

#include "unittest.h"
#include <ti/drv/udma/src/udmap/csl_udmap.h>
#include <ti/board/board.h>
#include <ti/osal/osal.h>

/*
 * UDMA driver objects
 */
struct Udma_DrvObj      gUdmaDrvObj;
struct Udma_ChObj       gUdmaTxChObj;
struct Udma_ChObj       gUdmaRxChObj[2];
struct Udma_FlowObj     gUdmaFlowObj;
struct Udma_RingObj     gUdmaRingObj[2];
struct Udma_EventObj    gUdmaEvtObj;
volatile int                     gRxPktCntInRing = 0;

void framework_rxIsrFxn(Udma_EventHandle  eventHandle,
                       uint32_t          eventType,
                       void             *appData)
{
    gRxPktCntInRing++;
    return;
}

int frameworkUdmaInitDrv(void)
{
    int32_t         retVal = UDMA_SOK;
    Udma_InitPrms   initPrms;
    uint32_t        instId;
    instId = UDMA_INST_ID_MAIN_0;
    UdmaInitPrms_init(instId, &initPrms);
    retVal = Udma_init(&gUdmaDrvObj, &initPrms);
    if(UDMA_SOK == retVal)
    {
        tFramework.gDrvHandle = &gUdmaDrvObj;
    }
    return (retVal);
}

int frameworkUdmaSetupTxChannel(void)
{
    uint32_t            chType;
    Udma_ChPrms         chPrms;
    Udma_ChTxPrms       txPrms;
    Udma_RingHandle     ringHandle;

    int32_t         retVal = UDMA_SOK;
    /* Create the Tx Channel */
    /* TX channel parameters */
    chType                      = UDMA_CH_TYPE_TX;
    UdmaChPrms_init(&chPrms, chType);
    chPrms.peerChNum            = TF_SA2UL_PEER_TXCHAN;
    chPrms.fqRingPrms.ringMem   = &memTxRing[0];
    chPrms.cqRingPrms.ringMem   = &memTxCompRing[0];
    chPrms.fqRingPrms.elemCnt   = TF_RING_TRCNT;
    chPrms.cqRingPrms.elemCnt   = TF_RING_TRCNT;

    chPrms.fqRingPrms.mode      = CSL_RINGACC_RING_MODE_RING;
    chPrms.cqRingPrms.mode      = CSL_RINGACC_RING_MODE_RING;

    /* Open TX channel for transmit */
    tFramework.gTxChHandle = &gUdmaTxChObj;
    retVal = Udma_chOpen(tFramework.gDrvHandle, tFramework.gTxChHandle, chType, &chPrms);

    if(UDMA_SOK == retVal)
    {
        UdmaChTxPrms_init(&txPrms, chType);
        txPrms.dmaPriority = UDMA_DEFAULT_UTC_CH_DMA_PRIORITY;
        txPrms.fetchWordSize = TF_SIZE_DESC >> 2;
        retVal = Udma_chConfigTx(tFramework.gTxChHandle, &txPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(tFramework.gTxChHandle);
        }
    }

    if (retVal == UDMA_SOK)
    {
        /* Update the Tx Ring numbers */
        ringHandle                = Udma_chGetFqRingHandle(tFramework.gTxChHandle);
        tFramework.gTxRingHandle  = ringHandle;
        tFramework.txRingNum      = Udma_ringGetNum(ringHandle);

        ringHandle                = Udma_chGetCqRingHandle(tFramework.gTxChHandle);
        tFramework.gTxComplRingHandle = ringHandle;
        tFramework.txComplRingNum = Udma_ringGetNum(ringHandle);
    }
    return (retVal);

}

int frameworkUdmaSetupRxChannel(void)
{
    uint32_t            chType;
    Udma_ChPrms         chPrms;
    Udma_ChRxPrms       rxPrms;
    Udma_RingHandle     ringHandle;
    Udma_FlowHandle     flowHandle;
    Udma_FlowPrms       flowPrms;
    Udma_EventPrms      eventPrms;
    int32_t             retVal = UDMA_SOK;
    uint32_t            intArg = 0;

    /* Create the Rx Channel */
    /* TX channel parameters */
    chType                      = UDMA_CH_TYPE_RX;

    /* Note that the ring memory is not provided for the second channel thread for SA as
     * We create the channel and ring for thr1 and use that flow for SA2UL applications
     */

     /* create a flow allocation here for Rx channels */
     tFramework.gRxFlowHandle = flowHandle = &gUdmaFlowObj;
     retVal  = Udma_flowAlloc(tFramework.gDrvHandle, flowHandle,1);

     if (retVal != UDMA_SOK)
     {
         return (retVal);
     }
     /* Update the created default flow with above configurations for sa2ul */
     tFramework.tfFlowNum      = Udma_flowGetNum(flowHandle);

    /* Create the Rx Channel */
    /* RX channel parameters */
    UdmaChPrms_init(&chPrms, chType);
    chPrms.peerChNum            = TF_SA2UL_PEER_RXCHAN1;
    chPrms.fqRingPrms.ringMem   = &memRxFreeRing[0];
    chPrms.cqRingPrms.ringMem   = &memRxRing[0];
    chPrms.fqRingPrms.elemCnt   = TF_RING_TRCNT;
    chPrms.cqRingPrms.elemCnt   = TF_RING_TRCNT;
    chPrms.fqRingPrms.mode      = CSL_RINGACC_RING_MODE_RING;
    chPrms.cqRingPrms.mode      = CSL_RINGACC_RING_MODE_RING;

    /* Open RX channel for receive from SA */
    tFramework.gRxChHandle[1] = &gUdmaRxChObj[1];
    retVal = Udma_chOpen(tFramework.gDrvHandle, tFramework.gRxChHandle[1], chType, &chPrms);

    if(UDMA_SOK == retVal)
    {
        UdmaChRxPrms_init(&rxPrms, chType);
        rxPrms.dmaPriority = UDMA_DEFAULT_UTC_CH_DMA_PRIORITY;
        rxPrms.fetchWordSize = TF_SIZE_DESC >> 2;
        rxPrms.flowIdFwRangeStart = tFramework.tfFlowNum;
        rxPrms.flowIdFwRangeCnt   = 1;
        rxPrms.configDefaultFlow  = FALSE;
        retVal = Udma_chConfigRx(tFramework.gRxChHandle[1], &rxPrms);
    }

    /* Create the channel for the second thread with same flow as other thread */
    UdmaChPrms_init(&chPrms, chType);
    chPrms.peerChNum            = TF_SA2UL_PEER_RXCHAN0;
    tFramework.gRxChHandle[0]   = &gUdmaRxChObj[0];
    retVal = Udma_chOpen(tFramework.gDrvHandle, tFramework.gRxChHandle[0], chType, &chPrms);

    if(UDMA_SOK == retVal)
    {
        UdmaChRxPrms_init(&rxPrms, chType);
        rxPrms.dmaPriority = UDMA_DEFAULT_UTC_CH_DMA_PRIORITY;
        rxPrms.fetchWordSize = TF_SIZE_DESC >> 2;
        rxPrms.flowIdFwRangeStart = tFramework.tfFlowNum;
        rxPrms.flowIdFwRangeCnt   = 1;
        rxPrms.configDefaultFlow  = FALSE;
        retVal = Udma_chConfigRx(tFramework.gRxChHandle[0], &rxPrms);
    }

    if (UDMA_SOK == retVal)
    {
        /* Update the Rx Ring numbers */
        ringHandle                    = Udma_chGetFqRingHandle(tFramework.gRxChHandle[1]);
        tFramework.gRxFreeRingHandle  = ringHandle;
        tFramework.rxFreeRingNum      = Udma_ringGetNum(ringHandle);
    
        ringHandle                    = Udma_chGetCqRingHandle(tFramework.gRxChHandle[1]);
        tFramework.gRxRingHandle      = ringHandle;
        tFramework.rxComplRingNum     = Udma_ringGetNum(ringHandle);

        /* Update the flow configuration to be used for both SA2UL Rx channels */
        /* Update the Rx Flow to be used for SA2UL */
        UdmaFlowPrms_init(&flowPrms, UDMA_CH_TYPE_RX);
        flowPrms.einfoPresent     = TRUE;
        flowPrms.psInfoPresent    = TRUE;
        flowPrms.errorHandling    = TRUE;
        flowPrms.descType         = CSL_UDMAP_DESC_TYPE_HOST;
        flowPrms.psLocation       = CSL_UDMAP_PS_LOC_DESC;
        flowPrms.defaultRxCQ      = tFramework.rxComplRingNum;

        flowPrms.srcTagLo         = 0;
        flowPrms.srcTagLoSel      = 4;
        flowPrms.srcTagHi         = 0;
        flowPrms.srcTagHiSel      = 2;
        flowPrms.destTagLo        = 0;
        flowPrms.destTagLoSel     = 4;
        flowPrms.destTagHi        = 0;
        flowPrms.destTagHiSel     = 5;
    
        /* Use the same free queue as default flow is not used in
         * selecting different queues based on threshold */
        flowPrms.fdq0Sz0Qnum    = tFramework.rxFreeRingNum;
        flowPrms.fdq0Sz1Qnum    = tFramework.rxFreeRingNum;
        flowPrms.fdq0Sz2Qnum    = tFramework.rxFreeRingNum;
        flowPrms.fdq0Sz3Qnum    = tFramework.rxFreeRingNum;
        flowPrms.fdq1Qnum       = tFramework.rxFreeRingNum;
        flowPrms.fdq2Qnum       = tFramework.rxFreeRingNum;
        flowPrms.fdq3Qnum       = tFramework.rxFreeRingNum;

        /* Update the created default flow with above configurations for sa2ul */
        Udma_flowConfig(flowHandle, 0U, &flowPrms);

        /* Register Ring complete Isr */
        tFramework.gRxEvtHandle = &gUdmaEvtObj;
    
        /* Initialize event parameters */
        intArg                      = tFramework.rxComplRingNum;
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
        eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        eventPrms.chHandle          = tFramework.gRxChHandle[1];
        eventPrms.masterEventHandle = NULL;
        eventPrms.eventCb           = &framework_rxIsrFxn;
        eventPrms.appData           = (void *)intArg;
        retVal = Udma_eventRegister(tFramework.gDrvHandle, tFramework.gRxEvtHandle, &eventPrms);
    }

    /* Enable the channel after everything is setup */
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chEnable(tFramework.gRxChHandle[1]);
        if (UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(tFramework.gRxChHandle[0]);
        }
    }
    return (retVal);

}


//=============================================================================
//  RingPush
//  Copy packet descriptor ptr to the specified ring's next free entry
//  (pass-by-reference) and commit it.
//=============================================================================
void RingPush( Udma_RingHandle ringHandle, uint32_t pktSize, physptr_t ptr )
{
#if defined (TEST_CORE_CACHE_COHERENT)
            /* No cache operations are needed */
#else
    const void *virtBufPtr;
#endif
    physptr_t  physDescPtr;
    int32_t    retVal;
    FW_CPPI_DESC_T *pDesc = (FW_CPPI_DESC_T *) ptr;
    if ( (pDesc == NULL) || (ringHandle == NULL) )
    {
        return;
    }

    pDesc->hostDesc.bufPtr = (uint64_t) Osal_VirtToPhys ((void *)(uintptr_t)pDesc->hostDesc.bufPtr);
    pDesc->hostDesc.orgBufPtr = (uint64_t) Osal_VirtToPhys ((void *)(uintptr_t)pDesc->hostDesc.orgBufPtr);
    physDescPtr = (uint64_t) Osal_VirtToPhys ((void *)&pDesc->hostDesc);

#if defined (TEST_CORE_CACHE_COHERENT)
            /* No cache operations are needed */
#else
    /* Wb Invdata cache */
    CacheP_wbInv((const void *)&pDesc->hostDesc, TF_SIZE_DESC);
    virtBufPtr = (const void *)(uintptr_t)pDesc->hostDesc.bufPtr;
    CacheP_wbInv(virtBufPtr, pktSize);
#endif
    retVal  = Udma_ringQueueRaw(ringHandle,(uint64_t)physDescPtr);

    if (retVal != UDMA_SOK)
    {
        while(1);
    }

    return;
}

//=============================================================================
//  RingPop
//  Return the next packet descriptor ptr (if available) from the specified
//  ring and acknowledge it.
//=============================================================================
int32_t RingPop( Udma_RingHandle ringHandle, FW_CPPI_DESC_T **pAppDesc )
{
    uint64_t pDesc = 0;
    int32_t  retVal = 0;
    FW_CPPI_DESC_T *pVirtHostDesc;
#if defined (TEST_CORE_CACHE_COHERENT)
    /* No cache operations are needed */
#else
    uint32_t pktsize;
#endif
    if ((pAppDesc == (FW_CPPI_DESC_T **)NULL) || (ringHandle == (Udma_RingHandle)NULL))
    {
        return -1; /* NULL not allowed */
    }


    Udma_ringDequeueRaw(ringHandle, &pDesc);

    if(pDesc == 0)
    {
        *pAppDesc = (FW_CPPI_DESC_T *)NULL;
        retVal = -1;
    }
    else
    {
        *pAppDesc = pVirtHostDesc = (FW_CPPI_DESC_T *)Osal_PhysToVirt(pDesc);
#if defined (TEST_CORE_CACHE_COHERENT)
                /* No cache operations are needed */
#else
        CacheP_Inv((const void *) &pVirtHostDesc->hostDesc, TF_SIZE_DESC);
#endif
        pVirtHostDesc->hostDesc.bufPtr = (uint64_t)Osal_PhysToVirt(pVirtHostDesc->hostDesc.bufPtr);
        pVirtHostDesc->hostDesc.orgBufPtr = (uint64_t)Osal_PhysToVirt(pVirtHostDesc->hostDesc.orgBufPtr);
#if defined (TEST_CORE_CACHE_COHERENT)
        /* No cache operations are needed */
#else
        pktsize  = CSL_FEXT (pVirtHostDesc->hostDesc.descInfo, UDMAP_CPPI5_PD_DESCINFO_PKTLEN);
        CacheP_Inv((const void *)(uintptr_t)pVirtHostDesc->hostDesc.bufPtr, (int32_t)pktsize);
#endif
     }

     return (retVal);
}

/** ============================================================================
 *   @n@b fwTx_ready_push
 *
 *   @b Description
 *   @n This functions puts TX descriptors into txReadyDescs
 *
 *   @param[in]
 *   @n None
 *
 *   @return    none
 * =============================================================================
 */
static void
fwTx_ready_push
(
   uint32_t size,
   physptr_t phys
)
{
    FW_CPPI_DESC_T *pCppiDesc = (FW_CPPI_DESC_T *)Osal_PhysToVirt (phys);

    pCppiDesc->nextPtr      = tFramework.txReadyDescs;
    tFramework.txReadyDescs = pCppiDesc;
} /* fwTx_ready_push */

void setup_cppi5InitHostDescQueueTx(uint32_t retqIdx, uint32_t descCnt, void (*pfPush)(uint32_t, physptr_t)  )
{
    uint8_t         *pBuffer;
    CSL_UdmapCppi5HMPD   *pDesc;
    FW_CPPI_DESC_T *pCppiDesc;
    uint32_t        i;

    for(i=0; i<descCnt; i++)
    {
        pBuffer                = &memBufRamTx[i][0];
        pCppiDesc              = (FW_CPPI_DESC_T *) &memDescRamTx[i];
        pDesc                  = (CSL_UdmapCppi5HMPD *) &pCppiDesc->hostDesc;

        /* setup the descriptor */
        memset(pCppiDesc, 0, sizeof(FW_CPPI_DESC_T));
        CSL_udmapCppi5SetDescType(pDesc, CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST);
        CSL_udmapCppi5SetReturnPolicy( pDesc,
                                       CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                       retqIdx);
        pDesc->bufPtr          = (uint64_t) (uintptr_t) Osal_VirtToPhys(pBuffer);
        pDesc->bufInfo1        = TF_DESC_BUFSIZE;
        pDesc->orgBufLen       = TF_DESC_BUFSIZE;
        pDesc->orgBufPtr       = pDesc->bufPtr;

#if defined (TEST_CORE_CACHE_COHERENT)
                /* No cache operations are needed */
#else
        /* make sure the descriptor is written back to memory for coherancy */
        CacheP_wbInv(pCppiDesc, sizeof(FW_CPPI_DESC_T));
#endif
        pfPush( 0, (physptr_t)Osal_VirtToPhys(pCppiDesc));
    }
}

/** ============================================================================
 *   @n@b emac_rx_free_push
 *
 *   @b Description
 *   @n This function attaches buffers to rx free descriptors
 *
 * =============================================================================
 */
static void
fwRx_free_push
(
   uint32_t size,
   physptr_t phys
)
{
    CSL_UdmapCppi5HMPD *pDesc = (CSL_UdmapCppi5HMPD *)Osal_PhysToVirt (phys);

    /* Push descriptor to Rx free descriptor queue */
    RingPush (tFramework.gRxFreeRingHandle, pDesc->bufInfo1,(physptr_t) pDesc);

} /* fwRx_free_push */


void setup_cppi5InitHostDescQueueRx (uint32_t retqIdx, uint32_t descCnt, uint32_t buffSize, void (*pfPush)(uint32_t, physptr_t)  )
{
    uint8_t         *pBuffer;
    CSL_UdmapCppi5HMPD   *pDesc;
    uint32_t        i;

    for(i=0; i<descCnt; i++)
    {
        pBuffer                = &memBufRamRx[i][0];
        pDesc                  = (CSL_UdmapCppi5HMPD *)&memDescRamRx[i][0];

        /* setup the descriptor */
        memset(pDesc, 0, sizeof(CSL_UdmapCppi5HMPD));
        CSL_udmapCppi5SetDescType(pDesc, CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST);
        CSL_udmapCppi5SetReturnPolicy( pDesc, CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                       retqIdx);
        pDesc->bufPtr          = (uint64_t) (uintptr_t) Osal_VirtToPhys(pBuffer);
        pDesc->bufInfo1        = buffSize;
        pDesc->orgBufLen       = buffSize;
        pDesc->orgBufPtr       = pDesc->bufPtr;

        pfPush(0, (physptr_t)Osal_VirtToPhys(pDesc));
    }
}

int initNavss(void)
{
    int32_t  retVal = UDMA_SOK;

    /* Initialize UDMA */
    retVal = frameworkUdmaInitDrv();

    if (retVal != UDMA_SOK)
    {
        System_printf("error in creating the udma drv handle \n");
        System_flush();
        return (-1);
    }

    /* Create a Tx Channel */
    retVal = frameworkUdmaSetupTxChannel();
    if (retVal != UDMA_SOK)
    {
        System_printf("error in creating the udma tx channel \n");
        System_flush();
        return (-1);
    }

    /* Create the descriptor pool for the Tx Ring */
    setup_cppi5InitHostDescQueueTx(tFramework.txComplRingNum, TF_NUM_DESC, fwTx_ready_push);

    /* Create the Rx channel for SA */
    retVal = frameworkUdmaSetupRxChannel();
    if (retVal != UDMA_SOK)
    {
        System_printf("error in creating the udma rx channels \n");
        System_flush();
        return (-1);
    }

    /* Create the descriptor pool for the Rx ring */
    if (retVal == UDMA_SOK)
    {
        setup_cppi5InitHostDescQueueRx (tFramework.rxFreeRingNum,
                                        TF_NUM_DESC,
                                        TF_DESC_BUFSIZE, fwRx_free_push  );
    }

    return(retVal);
}

/* Two semaphores are used to gate access to the PA handle tables */
int initSems (void)
{
    SemaphoreP_Params params;

    SemaphoreP_Params_init (&params);
    params.mode = SemaphoreP_Mode_BINARY;
    tFramework.tfSaSem 		  = SemaphoreP_create (1, &params);
    return (0);
}

/* Two semaphores are used to gate access to the PA handle tables */
static int deleteSems (void)
{
    SemaphoreP_delete (tFramework.tfSaSem);
    return (0);
}

/* Initialize the test framework */
int setupTestFramework (void)
{
    uint32_t boardInitStatus;

    /* until board lib is fixed */
    Board_initCfg cfg = (BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_MODULE_CLOCK) ;

    boardInitStatus = Board_init(cfg);
    if (boardInitStatus !=BOARD_SOK)
    {
        System_printf("Board_init failure\n");
        return(0);
    }
    System_printf("Board_init success\n");

    /* Setup the semaphores used for access to the PA tables.
     * This has to be done before the PA is initialized */
    if (initSems())  {
        System_printf ("setupTestFramework: initSems returned error, exiting\n");
        return (-1);
    }

	/* Setup the QM with associated buffers and descriptors */
	if (initNavss())  {
		System_printf ("setupTestFramework: initNavss returned error, exiting\n");
		return (-1);
	}

    /* Initialize the SA unit test support and create the SA driver instance */
    salld_sim_initialize();

    /* Initialize the test connection module */
#if !defined(NSS_LITE2)
    sauConnInit();
#endif

	return (0);

}

int exitTestFramework(void)
{

    salld_sim_close_sa();

	/* Delete the semaphores created */
	if (deleteSems())  {
		System_printf ("exitTestFramework: deleteSems returned error, exiting\n");
		return (-1);
	}

	return 0;
}


/* Check that all the queues are setup correctly */
int verifyTestFramework (void)
{
  return(0);
}


#ifndef USE_BIOS
void Task_exit(void)
{
    while (TRUE)
    {

    }
}
#endif

