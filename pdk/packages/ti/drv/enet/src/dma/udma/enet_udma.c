/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 * \file  enet_udma.c
 *
 * \brief This file contains the implementation of the Enet data path with UDMA.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x301

#include <string.h>
#include <ti/csl/arch/csl_arch.h>

#include <ti/drv/enet/enet_cfg.h>
#include <ti/drv/enet/include/core/enet_base.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/include/core/enet_soc.h>
#include <ti/drv/enet/include/core/enet_per.h>
#include <ti/drv/enet/include/core/enet_queue.h>
#include <ti/drv/enet/include/core/enet_osal.h>
#include <ti/drv/enet/include/core/enet_rm.h>
#include <ti/drv/enet/include/core/enet_dma.h>
#include <ti/drv/enet/priv/core/enet_base_priv.h>
#include <ti/drv/enet/priv/core/enet_trace_priv.h>
#include <ti/drv/enet/include/common/enet_utils_dflt.h>

#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>

#include "enet_udma_priv.h"

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

static int32_t EnetUdma_checkTxChParams(const EnetUdma_OpenTxChPrms *pTxChPrms);

static int32_t EnetUdma_checkRxFlowParams(const EnetUdma_OpenRxFlowPrms *pRxFlowPrms);

static void EnetUdma_drainIsrCq(EnetQ *dstQ,
                                EnetQ *isrCq);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Data Path Common Function Definitions             */
/* ========================================================================== */

void EnetUdma_initRxFlowParams(EnetUdma_OpenRxFlowPrms *pRxFlowPrms)
{
    Udma_FlowPrms flowPrms;

    memset(&flowPrms, 0, sizeof(flowPrms));
    UdmaFlowPrms_init(&flowPrms, UDMA_CH_TYPE_RX);

    pRxFlowPrms->flowPrms.psInfoPresent = TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_PSINFO_PRESENT;
    pRxFlowPrms->flowPrms.einfoPresent  = TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_EINFO_PRESENT;
    pRxFlowPrms->flowPrms.sopOffset    = flowPrms.sopOffset;
    pRxFlowPrms->flowPrms.defaultRxCQ  = flowPrms.defaultRxCQ;
    pRxFlowPrms->flowPrms.srcTagHi     = flowPrms.srcTagHi;
    pRxFlowPrms->flowPrms.srcTagLo     = flowPrms.srcTagLo;
    pRxFlowPrms->flowPrms.srcTagHiSel  = flowPrms.srcTagHiSel;
    pRxFlowPrms->flowPrms.srcTagLoSel  = TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_SRC_SELECT_SRC_TAG;
    pRxFlowPrms->flowPrms.destTagHi    = flowPrms.destTagHi;
    pRxFlowPrms->flowPrms.destTagLo    = flowPrms.destTagLo;
    pRxFlowPrms->flowPrms.destTagHiSel = flowPrms.destTagHiSel;
    pRxFlowPrms->flowPrms.destTagLoSel = flowPrms.destTagLoSel;
    pRxFlowPrms->flowPrms.sizeThreshEn =  TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_SIZE_THRESH_MAX;

    pRxFlowPrms->udmaChPrms.fqRingPrms.orderId    = UDMA_DEFAULT_RING_ORDER_ID;
    /* Use message mode for Rx FQ so at channel/flow teardown time UDMA IP can push
     * inflow descriptors back into the ring.*/
    pRxFlowPrms->udmaChPrms.fqRingPrms.mode       = (uint8_t)CSL_RINGACC_RING_MODE_MESSAGE;
    pRxFlowPrms->udmaChPrms.fqRingPrms.useRingMon = BFALSE;

    pRxFlowPrms->udmaChPrms.cqRingPrms.orderId    = UDMA_DEFAULT_RING_ORDER_ID;
    pRxFlowPrms->udmaChPrms.cqRingPrms.mode       = (uint8_t)CSL_RINGACC_RING_MODE_RING;
    pRxFlowPrms->udmaChPrms.cqRingPrms.useRingMon = BFALSE;

    /* We initialize UDMA driver handle to NULL */
    pRxFlowPrms->hUdmaDrv        = NULL;

    pRxFlowPrms->useGlobalEvt = BFALSE;
    pRxFlowPrms->notifyCb = NULL;
    pRxFlowPrms->cbArg   = NULL;

    pRxFlowPrms->useProxy = BFALSE;

    /* auto-reclaim initialization */
    pRxFlowPrms->autoReclaimPrms.enableFlag   = BFALSE;
    pRxFlowPrms->autoReclaimPrms.hDmaDescPool = NULL;
    pRxFlowPrms->autoReclaimPrms.hReclaimRing = NULL;

    return;
}

EnetDma_RxChHandle EnetUdma_openRxFlow(EnetDma_Handle hDma,
                                       const EnetUdma_OpenRxFlowPrms *pRxFlowPrms)
{
    int32_t retVal;
    Udma_RingHandle dropFqRing = NULL, localHandle;
    Udma_FlowPrms flowPrms;
    Udma_RingPrms ringPrms;
    EnetUdma_RxFlowObj *pRxFlow;
    EnetUdma_udmaInfo udmaInfo;
    EnetUdma_ringAllocInfo ringAllocInfo;
    uint32_t eventType;
    const EnetUdma_UdmaRingPrms *pFqRingPrms = &pRxFlowPrms->udmaChPrms.fqRingPrms;
    const EnetUdma_AutoReclaimPrms *reclaimPrms = &pRxFlowPrms->autoReclaimPrms;
    bool allocFlowObj = BFALSE, allocFqRing = BFALSE, allocDropFqRing = BFALSE;
    bool allocCqRing  = BFALSE, flowAttachFlag = BFALSE;
    bool allocProxy   = BFALSE, allocRingMon = BFALSE;
    uintptr_t intrKey;
    uint32_t flowStart;

    intrKey = EnetOsal_disableAllIntr();

    /* Set to NULL so if error condition we return NULL */
    pRxFlow = NULL;

    /* Error check */
    retVal = EnetUdma_checkRxFlowParams(pRxFlowPrms);

    if (UDMA_SOK == retVal)
    {
        pRxFlow = EnetUdma_memMgrAllocRxFlowObj();

        if (pRxFlow == NULL)
        {
            retVal = UDMA_EALLOC;
            ENETTRACE_ERR(retVal, "RX flow object mem alloc failed");
        }
        else
        {
            allocFlowObj = BTRUE;
            memset(pRxFlow, 0, sizeof(*pRxFlow));
            /* Save Flow config */
            pRxFlow->rxFlowPrms = *pRxFlowPrms;
            /* Initialize local variables to be used in reminder of this function  */

            pRxFlow->hUdmaDrv  = pRxFlowPrms->hUdmaDrv;
            dropFqRing         = &pRxFlow->rxFlowMemObj.dropFqRingObj;
            pRxFlow->hUdmaFlow = &pRxFlow->rxFlowMemObj.flowUdmaObj;
            pRxFlow->fqRing    = &pRxFlow->rxFlowMemObj.fqRingObj;
            pRxFlow->hUdmaEvt  = &pRxFlow->rxFlowMemObj.udmaEvtObj;
            pRxFlow->useGlobalEvt = pRxFlowPrms->useGlobalEvt;

            if (!reclaimPrms->enableFlag)
            {
                pRxFlow->cqRing       = &pRxFlow->rxFlowMemObj.cqRingObj;
                pRxFlow->hDmaDescPool = &pRxFlow->rxFlowMemObj.rxReadyDmaDescQ;
            }
            else
            {
#if ENET_CFG_IS_ON(DEV_ERROR)
                Enet_devAssert(NULL != reclaimPrms->hDmaDescPool);
                Enet_devAssert(NULL != reclaimPrms->hReclaimRing);
                /* Ring mode should be message as both RX flow (pushes to CQ) and
                 * TX channel (pops from FQ) uses this ring */
                Enet_devAssert(CSL_RINGACC_RING_MODE_MESSAGE == Udma_ringGetMode(reclaimPrms->hReclaimRing));
                /* Filtering of protocol specific and extended info should be enabled as this
                 * information might corrupt TX channel (as the TX PSI and EINFO gets mapped to
                 * RX flow PSI and EINFO which has entirely different meaning)*/
                Enet_devAssert(pRxFlowPrms->flowPrms.psInfoPresent ==
                            TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_PSINFO_PRESENT);
                Enet_devAssert(pRxFlowPrms->flowPrms.einfoPresent ==
                            TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_EINFO_PRESENT);
                /* Notify callback should be NULL when auto recycle is enabled as the
                 * CQ event should be disabled */
                Enet_devAssert(pRxFlowPrms->notifyCb == NULL);
#endif
                pRxFlow->cqRing       = reclaimPrms->hReclaimRing;
                pRxFlow->hDmaDescPool = reclaimPrms->hDmaDescPool;
            }
        }
    }

    if (UDMA_SOK == retVal)
    {
        UdmaRingPrms_init(&ringPrms);
        ringPrms.elemCnt = pRxFlowPrms->numRxPkts;
        ringPrms.orderId = pRxFlowPrms->udmaChPrms.fqRingPrms.orderId;
        ringPrms.mode    = pRxFlowPrms->udmaChPrms.fqRingPrms.mode;

        ringAllocInfo.allocRingMem    = BTRUE;
        ringAllocInfo.cbArg          = pRxFlowPrms->cbArg;
        ringAllocInfo.ringNum         = UDMA_RING_ANY;

        retVal = EnetUdma_allocRing(pRxFlow->hUdmaDrv,
                                   pRxFlow->fqRing,
                                   &ringPrms,
                                   &ringAllocInfo);
        ENETTRACE_ERR_IF((retVal != UDMA_SOK), retVal, "RX FQ ring alloc failed");
        if (UDMA_SOK == retVal)
        {
            allocFqRing = BTRUE;
        }
    }

    if ((UDMA_SOK == retVal) && (pFqRingPrms->useRingMon))
    {
        pRxFlow->hUdmaRingMon = EnetUdma_allocRingMon(pRxFlow->hUdmaDrv,
                                                     pRxFlow->fqRing,
                                                     pRxFlowPrms->numRxPkts,
                                                     &pFqRingPrms->ringMonCfg);
        if (NULL == pRxFlow->hUdmaRingMon)
        {
            retVal = UDMA_EALLOC;
            ENETTRACE_ERR(retVal, "RX FQ ring mon config failed");
        }
        else
        {
            allocRingMon = BTRUE;
        }
    }

    if ((UDMA_SOK == retVal) &&
        (TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_SIZE_THRESH_MAX == pRxFlowPrms->flowPrms.sizeThreshEn))
    {
        /* PDK-3723 We use drop ring for routing packets larger than flow max. allowed size
         * to avoid invalid data getting spilled into multiple descriptors. */
        UdmaRingPrms_init(&ringPrms);
        ringPrms.elemCnt = 1U;
        ringPrms.mode    = (uint8_t)CSL_RINGACC_RING_MODE_RING;

        ringAllocInfo.allocRingMem    = BTRUE;
        ringAllocInfo.cbArg          = pRxFlowPrms->cbArg;
        ringAllocInfo.ringNum         = UDMA_RING_ANY;

        retVal = EnetUdma_allocRing(pRxFlow->hUdmaDrv,
                                   dropFqRing,
                                   &ringPrms,
                                   &ringAllocInfo);
        ENETTRACE_ERR_IF((retVal != UDMA_SOK), retVal, "Drop FQ ring alloc failed");
        if (UDMA_SOK == retVal)
        {
            allocDropFqRing = BTRUE;
        }
    }

    if (UDMA_SOK == retVal)
    {
        if (pRxFlowPrms->useProxy)
        {
            retVal = EnetUdma_allocProxy(pRxFlow->hUdmaDrv, &pRxFlow->rxFlowMemObj.proxyObj, pRxFlow->fqRing);
            if (UDMA_SOK == retVal)
            {
                allocProxy          = BTRUE;
                pRxFlow->hUdmaProxy = &pRxFlow->rxFlowMemObj.proxyObj;
            }
        }
        else
        {
            pRxFlow->hUdmaProxy = NULL;
        }
    }

    if ((!reclaimPrms->enableFlag) && (UDMA_SOK == retVal))
    {
        UdmaRingPrms_init(&ringPrms);
        ringPrms.elemCnt = pRxFlowPrms->numRxPkts;
        ringPrms.orderId = pRxFlowPrms->udmaChPrms.cqRingPrms.orderId;
        ringPrms.mode    = pRxFlowPrms->udmaChPrms.cqRingPrms.mode;

        ringAllocInfo.allocRingMem    = BTRUE;
        ringAllocInfo.cbArg          = pRxFlowPrms->cbArg;
        ringAllocInfo.ringNum         = UDMA_RING_ANY;

        retVal = EnetUdma_allocRing(pRxFlow->hUdmaDrv,
                                   pRxFlow->cqRing,
                                   &ringPrms,
                                   &ringAllocInfo);
        ENETTRACE_ERR_IF((retVal != UDMA_SOK), retVal, "RX CQ ring alloc failed");
        if (UDMA_SOK == retVal)
        {
            allocCqRing = BTRUE;
        }
    }

    if (UDMA_SOK == retVal)
    {
        memset(&flowPrms, 0, sizeof(flowPrms));
        UdmaFlowPrms_init(&flowPrms, UDMA_CH_TYPE_RX);

        /* UdmaFlowPrms_init sets errorHandling to retry(1) by default,
         * in case of descriptor/buffer starvation UDMA retries unless it
         * gets a descriptor. In case of multi flow, this results in bottom
         * of FIFO drop, to avoid this errorHandling must be set to drop(0).
         */
        flowPrms.errorHandling = TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_ERR_DROP;

        /* CPPI TX Status Data Word [0..3] are mapped to Host Packet Descriptor Protocol Specific
         * Words if RFLOW[a]_RFA.rx_psinfo_present = 1 and RFLOW[a]_RFA.rx_ps_location = 0 (end) */
        flowPrms.psInfoPresent = pRxFlowPrms->flowPrms.psInfoPresent;
        /* Extended packet info block present CPPI TX info words are mapped */
        flowPrms.einfoPresent  = pRxFlowPrms->flowPrms.einfoPresent;
        flowPrms.psLocation    = TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_PS_END_PD;

        /* Set Src tag low selection to get port number of the received packet (or)
         * flow Id (or) config tag from CPPI. */
        flowPrms.srcTagLoSel   = pRxFlowPrms->flowPrms.srcTagLoSel;

        flowPrms.defaultRxCQ = pRxFlow->cqRing->ringNum;

        if (TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_SIZE_THRESH_MAX == pRxFlowPrms->flowPrms.sizeThreshEn)
        {
            /* Enable Rx Packet Size Threshold so we can route packets larger than
             * pRxFlowPrms->rxFlowMtu to drop ring */
            flowPrms.sizeThreshEn = pRxFlowPrms->flowPrms.sizeThreshEn;
            localHandle = dropFqRing;
        }
        else
        {
            localHandle = pRxFlow->fqRing;
        }

        /* Align the RX MTU size to avoid truncation. Refer to ENET_UDMA_RXMTU_ALIGN description for
         * more details */
        flowPrms.sizeThresh0 = ENET_UTILS_ALIGN(pRxFlowPrms->rxFlowMtu, ENET_UDMA_RXMTU_ALIGN);
        flowPrms.sizeThresh1 = ENET_UTILS_ALIGN(ENET_UDMA_JUMBO_PACKET_SIZE, ENET_UDMA_RXMTU_ALIGN);
        flowPrms.sizeThresh2 = ENET_UTILS_ALIGN(ENET_UDMA_JUMBO_PACKET_SIZE, ENET_UDMA_RXMTU_ALIGN);

        flowPrms.fdq0Sz0Qnum = pRxFlow->fqRing->ringNum;
        flowPrms.fdq0Sz1Qnum = localHandle->ringNum;
        flowPrms.fdq0Sz2Qnum = localHandle->ringNum;
        flowPrms.fdq0Sz3Qnum = localHandle->ringNum;
        flowPrms.fdq1Qnum    = localHandle->ringNum;
        flowPrms.fdq2Qnum    = localHandle->ringNum;
        flowPrms.fdq3Qnum    = localHandle->ringNum;

        flowStart = pRxFlowPrms->startIdx + pRxFlowPrms->flowIdx;

        /* Attach and configure the flows */
        retVal = Udma_flowAttach(pRxFlow->hUdmaDrv,
                                 pRxFlow->hUdmaFlow,
                                 flowStart,
                                 1U /* flowCnt */);

        ENETTRACE_ERR_IF((retVal != UDMA_SOK), retVal, "RX flow attach failed");
        if (UDMA_SOK == retVal)
        {
            flowAttachFlag = BTRUE;
            retVal         = Udma_flowConfig(pRxFlow->hUdmaFlow,
                                             0U, /* Flow Index */
                                             &flowPrms);
        }
    }

    if (UDMA_SOK == retVal)
    {
        /* Initialize the completion queue.
         * Must be done before enabling CQ events */
        EnetQueue_initQ(&pRxFlow->cqIsrQ);
    }

    if (UDMA_SOK == retVal)
    {
        if (!reclaimPrms->enableFlag)
        {
            EnetUdma_initRxFreeDescQ(pRxFlow);
        }

        ENETTRACE_VERBOSE_IF((pRxFlowPrms->notifyCb == NULL), "Event callback function is null");
        if (pRxFlowPrms->notifyCb != NULL)
        {
            if (pFqRingPrms->useRingMon && allocRingMon)
            {
                /* If ring mon is enabled on FQ, we register event for FQ low
                 * threshold and not CQ */
                eventType   = UDMA_EVENT_TYPE_RING_MON;
                localHandle = pRxFlow->fqRing;
            }
            else
            {
                eventType   = UDMA_EVENT_TYPE_RING;
                localHandle = pRxFlow->cqRing;
            }

            EnetUdma_getRxFlowUdmaInfo(pRxFlow, &udmaInfo);
            retVal = EnetUdma_registerEvent(&udmaInfo,
                                           localHandle,
                                           EnetUdma_rxCqIsr,
                                           pRxFlow,
                                           eventType);
            ENETTRACE_ERR_IF((retVal != UDMA_SOK), retVal, "RX flow event registration failed");
            if (ENET_SOK == retVal)
            {
                pRxFlow->evtInitFlag = BTRUE;
            }
        }
    }

    if (UDMA_SOK != retVal)
    {
        if ((pRxFlow != NULL) && pRxFlow->evtInitFlag)
        {
            retVal = EnetUdma_unregisterEvent(pRxFlow->hUdmaEvt);
            Enet_assert(UDMA_SOK == retVal);
        }

        /* Error. Free-up resource if allocated */
        if (allocFqRing)
        {
            retVal = EnetUdma_freeRing(pRxFlow->fqRing,
                                      pRxFlow->rxFlowPrms.numRxPkts,
                                      &pRxFlow->rxFlowPrms.cbArg);
            Enet_assert(UDMA_SOK == retVal);
        }

        if (allocDropFqRing)
        {
            retVal = EnetUdma_freeRing(&pRxFlow->rxFlowMemObj.dropFqRingObj,
                                      1U,
                                      &pRxFlow->rxFlowPrms.cbArg);
            Enet_assert(UDMA_SOK == retVal);
        }

        if (allocCqRing)
        {
            /* We free cqRing without retrieving packets here as packets are
             * not submitted by application yet so there wouldn't be any packets
             * in the cqRing */
            retVal = EnetUdma_freeRing(pRxFlow->cqRing,
                                      pRxFlow->rxFlowPrms.numRxPkts,
                                      &pRxFlow->rxFlowPrms.cbArg);
            Enet_assert(UDMA_SOK == retVal);
        }

        if (allocRingMon)
        {
            retVal = EnetUdma_freeRingMon(pRxFlow->hUdmaRingMon);
            Enet_assert(UDMA_SOK == retVal);
        }

        if (flowAttachFlag)
        {
            retVal = Udma_flowDetach(pRxFlow->hUdmaFlow);
            Enet_assert(UDMA_SOK == retVal);

            /* Free up ready DMA descriptors */
            if (!reclaimPrms->enableFlag)
            {
                EnetUdma_deInitRxFreeDescQ(pRxFlow);
            }
        }

        if (allocProxy)
        {
            retVal = EnetUdma_freeProxy(pRxFlow->hUdmaProxy);
            Enet_assert(retVal == UDMA_SOK);
            pRxFlow->hUdmaProxy = NULL;
        }

        /* Free the rxFlowObj last */
        if (allocFlowObj)
        {
            EnetUdma_memMgrFreeRxFlowObj(pRxFlow);
        }

        /* As flow open is failed return NULL */
        pRxFlow = NULL;
    }
    else
    {
        pRxFlow->initFlag = BTRUE;
        pRxFlow->hDma = hDma;
    }

    EnetOsal_restoreAllIntr(intrKey);

    return pRxFlow;
}

int32_t EnetUdma_closeRxFlow(EnetDma_RxChHandle hRxFlow,
                             EnetDma_PktQ *pFqPktInfoQ,
                             EnetDma_PktQ *pCqPktInfoQ)
{
    int32_t retVal = UDMA_SOK;
    uintptr_t intrKey;

#if ENET_CFG_IS_ON(DEV_ERROR)
    if ((NULL == hRxFlow) ||
        (NULL == pFqPktInfoQ) ||
        (NULL == pCqPktInfoQ))
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR_IF((NULL == hRxFlow), retVal, "hRxFlow is NULL");
        ENETTRACE_ERR_IF((NULL == pFqPktInfoQ), retVal, "pFqPktInfoQ is NULL");
        ENETTRACE_ERR_IF((NULL == pFqPktInfoQ), retVal, "pFqPktInfoQ is NULL");
        Enet_assert(BFALSE);
    }
    else
#endif
    {
        intrKey = EnetOsal_disableAllIntr();

        /* Error check */
        if (hRxFlow == NULL)
        {
            retVal = UDMA_EBADARGS;
        }

        if (UDMA_SOK == retVal)
        {
            if (hRxFlow->evtInitFlag)
            {
                /* Disable event */
                retVal = EnetDma_disableRxEvent(hRxFlow);
                Enet_assert(UDMA_SOK == retVal);
            }

            /* Flush packets in Rx FQ - retrieve any packets that haven't been
             * processed yet */
            retVal += EnetUdma_flushRxFlowRing(hRxFlow, hRxFlow->fqRing, pFqPktInfoQ);
            if (UDMA_SOK == retVal)
            {
                retVal = EnetUdma_freeRing(hRxFlow->fqRing,
                                          hRxFlow->rxFlowPrms.numRxPkts,
                                          &hRxFlow->rxFlowPrms.cbArg);
                Enet_assert(UDMA_SOK == retVal);

                if (TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_SIZE_THRESH_MAX ==  hRxFlow->rxFlowPrms.flowPrms.sizeThreshEn)
                {
                    retVal = EnetUdma_freeRing(&hRxFlow->rxFlowMemObj.dropFqRingObj,
                                              1U,
                                              &hRxFlow->rxFlowPrms.cbArg);
                    Enet_assert(UDMA_SOK == retVal);
                }
            }
        }

        if (UDMA_SOK == retVal)
        {
            /* Retrieve packets in Rx CQ so app can free them */
            retVal += EnetUdma_flushRxFlowRing(hRxFlow, hRxFlow->cqRing, pCqPktInfoQ);
            if ((UDMA_SOK == retVal) && (hRxFlow->evtInitFlag))
            {
                /* Return if any packets retrieved in the ISR */
                EnetQueue_append(pCqPktInfoQ, &hRxFlow->cqIsrQ);

                /* Unregister event - make sure you unregister the event after flushing
                 * the ring. This is because unregister event resets the ring and all the
                 * descriptors in the ring gets discarded causing resource leakage */
                retVal              += EnetUdma_unregisterEvent(hRxFlow->hUdmaEvt);
                hRxFlow->evtInitFlag = BFALSE;
            }

            if (UDMA_SOK == retVal)
            {
                retVal = EnetUdma_freeRing(hRxFlow->cqRing,
                                          hRxFlow->rxFlowPrms.numRxPkts,
                                          &hRxFlow->rxFlowPrms.cbArg);
                Enet_assert(UDMA_SOK == retVal);
            }
        }

        if (UDMA_SOK == retVal)
        {
            /* Free if any DMA descriptors in the flows ready Q which application has
             * not consumed */
            if (!hRxFlow->rxFlowPrms.autoReclaimPrms.enableFlag)
            {
                EnetUdma_deInitRxFreeDescQ(hRxFlow);
            }

            if (hRxFlow->rxFlowPrms.useProxy)
            {
                retVal += EnetUdma_freeProxy(hRxFlow->hUdmaProxy);
                Enet_assert(retVal == UDMA_SOK);
            }

            if (NULL != hRxFlow->hUdmaRingMon)
            {
                retVal = EnetUdma_freeRingMon(hRxFlow->hUdmaRingMon);
                Enet_assert(UDMA_SOK == retVal);
            }

            // TODO should detach happen at beginning?
            /* Detach UDMA flow so data reception starts */
            retVal           += Udma_flowDetach(hRxFlow->hUdmaFlow);
            hRxFlow->initFlag = BFALSE;

            /* Free Tx channel driver object memory */
            EnetUdma_memMgrFreeRxFlowObj(hRxFlow);
        }

        EnetOsal_restoreAllIntr(intrKey);
    }

    return retVal;
}

void EnetUdma_initTxChParams(EnetUdma_OpenTxChPrms *pTxChPrms)
{
    Udma_ChTxPrms txPrms;

    UdmaChTxPrms_init(&txPrms, UDMA_CH_TYPE_TX);
    pTxChPrms->udmaTxChPrms.filterPsWords = TISCI_MSG_VALUE_RM_UDMAP_TX_CH_FILT_PSWORDS_DISABLED;
    /* By default - don't filter extended packet info block where CPPI RX info words are mapped.
       App can enable filtering in case auto-reclaim use-case */
    pTxChPrms->udmaTxChPrms.filterEinfo   = TISCI_MSG_VALUE_RM_UDMAP_TX_CH_FILT_EINFO_DISABLED;
    pTxChPrms->udmaTxChPrms.addrType    = txPrms.addrType;
    pTxChPrms->udmaTxChPrms.chanType    = txPrms.chanType;
    pTxChPrms->udmaTxChPrms.busPriority = txPrms.busPriority;
    pTxChPrms->udmaTxChPrms.busQos      = txPrms.busQos;
    pTxChPrms->udmaTxChPrms.busOrderId  = txPrms.busOrderId;
    pTxChPrms->udmaTxChPrms.dmaPriority = txPrms.dmaPriority;
    pTxChPrms->udmaTxChPrms.txCredit    = txPrms.txCredit;
    pTxChPrms->udmaTxChPrms.fifoDepth   = txPrms.fifoDepth;

    pTxChPrms->chNum = UDMA_DMA_CH_INVALID;

    pTxChPrms->udmaChPrms.fqRingPrms.orderId    = UDMA_DEFAULT_RING_ORDER_ID;
    /* Use message mode for Tx FQ so at channel/flow teardown time UDMA IP can push
     * inflow descriptors back into the ring.*/
    pTxChPrms->udmaChPrms.fqRingPrms.mode       = CSL_RINGACC_RING_MODE_MESSAGE;
    pTxChPrms->udmaChPrms.fqRingPrms.useRingMon = BFALSE;

    pTxChPrms->udmaChPrms.cqRingPrms.orderId    = UDMA_DEFAULT_RING_ORDER_ID;
    pTxChPrms->udmaChPrms.cqRingPrms.mode       = CSL_RINGACC_RING_MODE_RING;
    pTxChPrms->udmaChPrms.cqRingPrms.useRingMon = BFALSE;

    /* We initialize UDMA driver handle to NULL */
    pTxChPrms->hUdmaDrv        = NULL;

    pTxChPrms->useGlobalEvt = BFALSE;
    pTxChPrms->notifyCb = NULL;
    pTxChPrms->cbArg   = NULL;

    pTxChPrms->useProxy = BFALSE;

    /* auto-reclaim initialization */
    pTxChPrms->autoReclaimPrms.enableFlag   = BFALSE;
    pTxChPrms->autoReclaimPrms.hDmaDescPool = NULL;
    pTxChPrms->autoReclaimPrms.hReclaimRing = NULL;

    return;
}

EnetDma_TxChHandle EnetUdma_openTxCh(EnetDma_Handle hDma,
                                     const EnetUdma_OpenTxChPrms *pTxChPrms)
{
    int32_t retVal         = UDMA_SOK;
    EnetUdma_TxChObj *pTxCh = NULL;
    Udma_ChPrms chPrms;
    Udma_ChTxPrms txPrms;
    Udma_RingPrms ringPrms;
    EnetUdma_udmaInfo udmaInfo;
    EnetUdma_ringAllocInfo ringAllocInfo;
    uint32_t chType;
    bool allocChObj    = BFALSE, chOpenFlag = BFALSE, chEnFlag = BFALSE;
    bool allocFqRing   = BFALSE, allocCqRing  = BFALSE;
    bool allocProxy    = BFALSE;
    const EnetUdma_AutoReclaimPrms *reclaimPrms = &pTxChPrms->autoReclaimPrms;
    void *pTdCqRingMem;
    uintptr_t intrKey;

    intrKey = EnetOsal_disableAllIntr();

    /* Set to NULL so if error condition we return NULL */
    pTxCh = NULL;

    /* Error check */
    retVal = EnetUdma_checkTxChParams(pTxChPrms);

    if (UDMA_SOK == retVal)
    {
        /* allocate Tx channel object */
        pTxCh = EnetUdma_memMgrAllocTxChObj();
        if (pTxCh == NULL)
        {
            retVal = UDMA_EALLOC;
            ENETTRACE_ERR(retVal, "TX channel object mem alloc failed");
        }
        else
        {
            allocChObj = BTRUE;
            memset(pTxCh, 0, sizeof(*pTxCh));
            /* Save channel config */
            pTxCh->txChPrms = *pTxChPrms;
            /* Initialize local variables to be used in reminder of this function  */
            chType          = UDMA_CH_TYPE_TX;
            pTxCh->hUdmaDrv = pTxChPrms->hUdmaDrv;
            pTxCh->hUdmaCh  = &pTxCh->txChMemObj.udmaChObj;
            pTxCh->fqRing   = &pTxCh->txChMemObj.fqRingObj;
            pTxCh->hUdmaEvt = &pTxCh->txChMemObj.udmaEvtObj;
            pTxCh->useGlobalEvt = pTxChPrms->useGlobalEvt;

            if (!reclaimPrms->enableFlag)
            {
                pTxCh->cqRing       = &pTxCh->txChMemObj.cqRingObj;
                pTxCh->hDmaDescPool = &pTxCh->txChMemObj.txFreeDmaDescQ;
            }
            else
            {
#if ENET_CFG_IS_ON(DEV_ERROR)
                Enet_assert(NULL != reclaimPrms->hDmaDescPool);
                Enet_assert(NULL != reclaimPrms->hReclaimRing);
                /* Ring mode should be message as both TX channel (pushes to CQ) and RX flow
                 * (pops from FQ) uses this ring */
                Enet_assert(CSL_RINGACC_RING_MODE_MESSAGE == Udma_ringGetMode(reclaimPrms->hReclaimRing));
                /* Filtering of protocol specific and extended info should be enabled as this
                 * information might corrupt RX flow (as the RX flow PSI and EINFO gets mapped to
                 * Tx channel PSI and EINFO which has entirely different meaning) */
                Enet_assert(pTxChPrms->udmaTxChPrms.filterEinfo ==
                            TISCI_MSG_VALUE_RM_UDMAP_TX_CH_FILT_EINFO_DISABLED);
                Enet_assert(pTxChPrms->udmaTxChPrms.filterPsWords ==
                            TISCI_MSG_VALUE_RM_UDMAP_TX_CH_FILT_PSWORDS_ENABLED);
                /* Notify callback should be NULL when auto recycle is enabled as the
                 * CQ event should be disabled */
                Enet_assert(pTxChPrms->notifyCb == NULL);
#endif
                pTxCh->cqRing       = reclaimPrms->hReclaimRing;
                pTxCh->hDmaDescPool = reclaimPrms->hDmaDescPool;
            }
        }
    }

    if (UDMA_SOK == retVal)
    {
        /* Initialize channel params (PSI-L thread and RingAcc memories) */
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum          = pTxChPrms->chNum;
    }

    if (UDMA_SOK == retVal)
    {
        pTdCqRingMem = EnetUdma_memMgrAllocTdCqRingMemObj();
        if (pTdCqRingMem != NULL)
        {
            chPrms.tdCqRingPrms.ringMem = pTdCqRingMem;
            chPrms.tdCqRingPrms.ringMemSize = sizeof(uint64_t);
            chPrms.tdCqRingPrms.elemCnt = ENET_UDMA_TDCQ_RING_ELE_CNT;
            chPrms.tdCqRingPrms.mode    = (uint8_t)CSL_RINGACC_RING_MODE_RING;
            chPrms.chNum                = UDMA_DMA_CH_ANY;

            /* Open the UDMA channel */
            retVal = Udma_chOpen(pTxCh->hUdmaDrv, pTxCh->hUdmaCh, chType, &chPrms);
            ENETTRACE_ERR_IF((UDMA_SOK != retVal), retVal, "TX channel open failed");
            if (UDMA_SOK == retVal)
            {
                chOpenFlag = BTRUE;
            }
        }
        else
        {
            retVal = UDMA_EALLOC;
            ENETTRACE_ERR_IF((pTdCqRingMem == NULL), retVal, "TX TDCQ ring mem alloc failed");
        }
    }

    if (UDMA_SOK == retVal)
    {
        UdmaRingPrms_init(&ringPrms);
        ringPrms.elemCnt = pTxChPrms->numTxPkts;
        ringPrms.orderId = pTxChPrms->udmaChPrms.fqRingPrms.orderId;
        ringPrms.mode    = pTxChPrms->udmaChPrms.fqRingPrms.mode;

        ringAllocInfo.allocRingMem    = BTRUE;
        ringAllocInfo.cbArg          = pTxChPrms->cbArg;
        ringAllocInfo.ringNum         = Udma_chGetNum(pTxCh->hUdmaCh);

        retVal = EnetUdma_allocRing(pTxCh->hUdmaDrv,
                                   pTxCh->fqRing,
                                   &ringPrms,
                                   &ringAllocInfo);
        ENETTRACE_ERR_IF((UDMA_SOK != retVal), retVal, "TX FQ ring alloc failed");
        if (UDMA_SOK == retVal)
        {
            allocFqRing = BTRUE;
        }
    }

    if ((!reclaimPrms->enableFlag) && (UDMA_SOK == retVal))
    {
        UdmaRingPrms_init(&ringPrms);
        ringPrms.elemCnt = pTxChPrms->numTxPkts;
        ringPrms.orderId = pTxChPrms->udmaChPrms.cqRingPrms.orderId;
        ringPrms.mode    = pTxChPrms->udmaChPrms.cqRingPrms.mode;

        ringAllocInfo.allocRingMem    = BTRUE;
        ringAllocInfo.cbArg          = pTxChPrms->cbArg;
        ringAllocInfo.ringNum         = UDMA_RING_ANY;

        retVal = EnetUdma_allocRing(pTxCh->hUdmaDrv,
                                   pTxCh->cqRing,
                                   &ringPrms,
                                   &ringAllocInfo);
        ENETTRACE_ERR_IF((UDMA_SOK != retVal), retVal, "TX CQ ring alloc failed");
        if (UDMA_SOK == retVal)
        {
            allocCqRing = BTRUE;
        }
    }

    if (UDMA_SOK == retVal)
    {
        /* Configure TX channel */
        UdmaChTxPrms_init(&txPrms, chType);

        /* TODO - Should this be enabled? */
        txPrms.pauseOnError    = TISCI_MSG_VALUE_RM_UDMAP_CH_PAUSE_ON_ERROR_DISABLED;

        /* CPPI RX (host TX) Control Data Words [0..2] are mapped to Host Packet Descriptor
         * Protocol Specific Words if (TCHAN[a]_TCFG.tx_filt_pswords = 0) and
         * (Host PD Word 1.Protocol Specific Region Location.bit[28] = 0h) and
         * (Host PD Word 1.Protocol Specific Valid Word Count.bits[22-27] =4h)
         */
        txPrms.filterPsWords = pTxChPrms->udmaTxChPrms.filterPsWords;
        txPrms.filterEinfo   = pTxChPrms->udmaTxChPrms.filterEinfo;
        txPrms.chanType      = pTxChPrms->udmaTxChPrms.chanType;
        txPrms.busPriority   = pTxChPrms->udmaTxChPrms.busPriority;
        txPrms.busQos        = pTxChPrms->udmaTxChPrms.busQos;
        txPrms.busOrderId    = pTxChPrms->udmaTxChPrms.busOrderId;
        txPrms.dmaPriority   = pTxChPrms->udmaTxChPrms.dmaPriority;
        txPrms.txCredit      = pTxChPrms->udmaTxChPrms.txCredit;
        txPrms.fifoDepth     = pTxChPrms->udmaTxChPrms.fifoDepth;

        retVal = Udma_chConfigTx(pTxCh->hUdmaCh, &txPrms);
        ENETTRACE_ERR_IF((UDMA_SOK != retVal), retVal, "TX channel config failed");
        if (UDMA_SOK == retVal)
        {
            chOpenFlag = BTRUE;
        }
    }

    if (UDMA_SOK == retVal)
    {
        /* Initialize the completion queue.
         * Must be done before enabling CQ events */
        EnetQueue_initQ(&pTxCh->cqIsrQ);
    }

    if (UDMA_SOK == retVal)
    {
        if (NULL != pTxCh->fqRing)
        {
            if (pTxChPrms->useProxy)
            {
                retVal = EnetUdma_allocProxy(pTxCh->hUdmaDrv, &pTxCh->txChMemObj.proxyObj, pTxCh->fqRing);
                if (UDMA_SOK == retVal)
                {
                    allocProxy        = BTRUE;
                    pTxCh->hUdmaProxy = &pTxCh->txChMemObj.proxyObj;
                }
            }
            else
            {
                pTxCh->hUdmaProxy = NULL;
            }
        }
    }

    if (UDMA_SOK == retVal)
    {
        if (NULL != pTxCh->cqRing)
        {
            if (reclaimPrms->enableFlag)
            {
                /* In auto-reclaim case we "re" configure the app passed desc Q
                 * to change return Q index (retQIdx) configured in descriptor */
                EnetUdma_reconfigureDescQ(pTxCh);
            }
            else
            {
                /* Initialize the descriptor Q.*/
                EnetUdma_initTxFreeDescQ(pTxCh);
            }

            /* Enable the UDMA channel */
            retVal = Udma_chEnable(pTxCh->hUdmaCh);
        }

        ENETTRACE_VERBOSE_IF((pTxChPrms->notifyCb == NULL), "Event callback function is null");
        if ((UDMA_SOK == retVal) && (pTxChPrms->notifyCb != NULL))
        {
            chEnFlag = BTRUE;
            EnetUdma_getTxChUdmaInfo(pTxCh, &udmaInfo);
            retVal = EnetUdma_registerEvent(&udmaInfo,
                                           pTxCh->cqRing,
                                           &EnetUdma_txCqIsr,
                                           pTxCh,
                                           UDMA_EVENT_TYPE_RING);
            ENETTRACE_ERR_IF((retVal != UDMA_SOK), retVal, "TX channel event registration failed");
            if (ENET_SOK == retVal)
            {
                pTxCh->evtInitFlag = BTRUE;
            }
        }
    }

    /* Get teardown cqRing handle */
    if (UDMA_SOK == retVal)
    {
        pTxCh->tdCqRing = Udma_chGetTdCqRingHandle(pTxCh->hUdmaCh);
        if(NULL == pTxCh->tdCqRing)
        {
            retVal = UDMA_EALLOC;
        }
    }

    if (UDMA_SOK != retVal)
    {
        if (allocChObj)
        {
            /* Error. Free-up resource if allocated - order for freeing is important due
             * to dependencies */
            if (pTxCh->evtInitFlag)
            {
                retVal = EnetUdma_unregisterEvent(pTxCh->hUdmaEvt);
                Enet_assert(UDMA_SOK == retVal);
            }

            if (allocFqRing)
            {
                retVal = EnetUdma_freeRing(pTxCh->fqRing,
                                           pTxChPrms->numTxPkts,
                                           &pTxChPrms->cbArg);
                Enet_assert(UDMA_SOK == retVal);
            }

            if (allocCqRing)
            {
                /* We free cqRing without retrieving packets here as packets are
                 * not submitted by application yet so there wouldn't be any packets
                 * in the cqRing */
                retVal = EnetUdma_freeRing(pTxCh->cqRing,
                                           pTxChPrms->numTxPkts,
                                           &pTxChPrms->cbArg);
                Enet_assert(UDMA_SOK == retVal);
            }

            /* Free up ready DMA descriptors */
            if (!reclaimPrms->enableFlag)
            {
                /* Free DMA descriptors in txFreeDmaDescQ */
                EnetUdma_deInitTxFreeDescQ(pTxCh);
            }

            if (allocProxy)
            {
                retVal = EnetUdma_freeProxy(pTxCh->hUdmaProxy);
                Enet_assert(retVal == UDMA_SOK);
                pTxCh->hUdmaProxy = NULL;
            }

            if (chEnFlag)
            {
                retVal = Udma_chDisable(pTxCh->hUdmaCh, ENET_UDMA_TEARDOWN_TIME_MS);
                Enet_assert(retVal == UDMA_SOK);
            }

            if (chOpenFlag)
            {
                retVal = Udma_chClose(pTxCh->hUdmaCh);
                Enet_assert(retVal == UDMA_SOK);
            }

            EnetUdma_memMgrFreeTxChObj(pTxCh);
        }
    }
    else
    {
        pTxCh->hDma = hDma;
        pTxCh->initFlag = BTRUE;
    }


    EnetOsal_restoreAllIntr(intrKey);

    return pTxCh;
}

int32_t EnetUdma_closeTxCh(EnetDma_TxChHandle hTxCh,
                           EnetDma_PktQ *pFqPktInfoQ,
                           EnetDma_PktQ *pCqPktInfoQ)
{
    CSL_UdmapTdResponse tdResp;
    void *tdCqRingMemPtr = NULL;
    uintptr_t intrKey;
    int32_t retVal = ENET_SOK;

#if ENET_CFG_IS_ON(DEV_ERROR)
    if ((NULL == hTxCh) ||
        (NULL == pFqPktInfoQ) ||
        (NULL == pCqPktInfoQ))
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR_IF((NULL == hTxCh), retVal, "hTxCh is NULL");
        ENETTRACE_ERR_IF((NULL == pFqPktInfoQ), retVal, "pFqPktInfoQ is NULL");
        ENETTRACE_ERR_IF((NULL == pFqPktInfoQ), retVal, "pFqPktInfoQ is NULL");
        Enet_assert(BFALSE);
    }
    else
#endif
    {
        intrKey = EnetOsal_disableAllIntr();

        /* Error check */
        if (hTxCh == NULL)
        {
            retVal = UDMA_EBADARGS;
            ENETTRACE_ERR(retVal, "Invalid TX channel handle");
        }

        if (UDMA_SOK == retVal)
        {
            if (!hTxCh->initFlag)
            {
                retVal = UDMA_EFAIL;
                ENETTRACE_ERR(retVal, "TX channel is not initialized");
            }
        }

        /* Decode response information. Return an error if tear down is incomplete
         * or if tear-down was not graceful and data was potentially lost */
        if (UDMA_SOK == retVal)
        {
            /* Disable the UDMA channel */
            retVal += Udma_chDisable(hTxCh->hUdmaCh, ENET_UDMA_TEARDOWN_TIME_MS);

            /* Dequeue the teardown response */
            if (UDMA_SOK == retVal)
            {
                /* Store the ring memory pointers so we can free ring memory when UDMA frees
                 * Ring via Udma_ringFree internally in Udma_chClose */
                tdCqRingMemPtr = Udma_ringGetMemPtr(hTxCh->tdCqRing);

                do
                {
                    retVal = Udma_chDequeueTdResponse(hTxCh->hUdmaCh, &tdResp);
                }
                while (UDMA_SOK != retVal);

                if (UFALSE == tdResp.tdIndicator)
                {
                    retVal += UDMA_EFAIL;
                    ENETTRACE_ERR(retVal, "TX channel teardown failed");
                }

                /* Free tdCq ring memory */
                EnetUdma_memMgrFreeTdCqRingMemObj(tdCqRingMemPtr);
            }
        }

        if (UDMA_SOK == retVal)
        {
            if (hTxCh->evtInitFlag)
            {
                /* Disable channel Event */
                retVal += EnetDma_disableTxEvent(hTxCh);
            }

            /* Flush packets in Rx FQ - retrieve any packets that haven't been
             * processed yet */
            retVal += EnetUdma_flushTxChRing(hTxCh, hTxCh->fqRing, pFqPktInfoQ);
            retVal += EnetUdma_freeRing(hTxCh->fqRing,
                                       hTxCh->txChPrms.numTxPkts,
                                       &hTxCh->txChPrms.cbArg);
            Enet_assert(UDMA_SOK == retVal);
        }

        if (UDMA_SOK == retVal)
        {
            /* Retrieve packets in Rx CQ */
            retVal += EnetUdma_flushTxChRing(hTxCh, hTxCh->cqRing, pCqPktInfoQ);

            if ((UDMA_SOK == retVal) && (hTxCh->evtInitFlag))
            {
                /* Return if any packets retrieved in the ISR */
                EnetQueue_append(pCqPktInfoQ, &hTxCh->cqIsrQ);

                /* Unregister event - make sure you unregister the event after flushing the ring.
                 * This is because unregister event resets the ring and all the descriptors
                 * in the ring gets discarded causing resource leakage */
                retVal            += EnetUdma_unregisterEvent(hTxCh->hUdmaEvt);
                hTxCh->evtInitFlag = BFALSE;
            }

            retVal += EnetUdma_freeRing(hTxCh->cqRing,
                                       hTxCh->txChPrms.numTxPkts,
                                       &hTxCh->txChPrms.cbArg);
            Enet_assert(UDMA_SOK == retVal);
        }

        if (UDMA_SOK == retVal)
        {
            /* Free up ready DMA descriptors */
            if (!hTxCh->txChPrms.autoReclaimPrms.enableFlag)
            {
                /* Free DMA descriptors in txFreeDmaDescQ */
                EnetUdma_deInitTxFreeDescQ(hTxCh);
            }

            /* Free proxies if allocated */
            if (hTxCh->txChPrms.useProxy)
            {
                retVal += EnetUdma_freeProxy(hTxCh->hUdmaProxy);
                Enet_assert(retVal == UDMA_SOK);
            }

            if (UDMA_SOK == retVal)
            {
                /* Close the UDMA channel */
                retVal         += Udma_chClose(hTxCh->hUdmaCh);
                hTxCh->initFlag = BFALSE;
            }

            /* Free Tx channel driver object memory */
            EnetUdma_memMgrFreeTxChObj(hTxCh);
        }

        EnetOsal_restoreAllIntr(intrKey);
    }

    return retVal;
}

int32_t EnetDma_enableRxEvent(EnetDma_RxChHandle hRxFlow)
{
    int32_t retVal = ENET_SOK;

    ENETTRACE_VAR(retVal);

    if ((NULL != hRxFlow) && (BTRUE == hRxFlow->evtInitFlag))
    {
        retVal = Udma_eventEnable(hRxFlow->hUdmaEvt);
    }
    else
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR(retVal, "RX event enable failed");
    }

    return retVal;
}

int32_t EnetDma_disableRxEvent(EnetDma_RxChHandle hRxFlow)
{
    int32_t retVal = ENET_SOK;

    ENETTRACE_VAR(retVal);

    if ((NULL != hRxFlow) && (BTRUE == hRxFlow->evtInitFlag))
    {
        retVal = Udma_eventDisable(hRxFlow->hUdmaEvt);
    }
    else
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR(retVal, "RX event disabled failed");
    }

    return retVal;
}

int32_t EnetDma_getRxChStats(EnetDma_RxChHandle hRxFlow,
                               EnetDma_RxChStats *rxFlowStats)
{
    int32_t retVal = ENET_ENOTSUPPORTED;

#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
    *rxFlowStats = hRxFlow->stats;
    retVal = ENET_SOK;
#endif

    return retVal;
}

int32_t EnetDma_getTxChStats(EnetDma_TxChHandle hTxCh,
                             EnetDma_TxChStats *txChStats)
{
    int32_t retVal = ENET_ENOTSUPPORTED;

#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
    *txChStats = hTxCh->stats;
    retVal = ENET_SOK;
#endif

    return retVal;
}

int32_t EnetDma_resetRxChStats(EnetDma_RxChHandle hRxFlow)
{
    int32_t retVal = ENET_ENOTSUPPORTED;

#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
    memset(&hRxFlow->stats, 0, sizeof(hRxFlow->stats));
    retVal = ENET_SOK;
#endif

    return retVal;
}

int32_t EnetDma_resetTxChStats(EnetDma_TxChHandle hTxCh)
{
    int32_t retVal = ENET_ENOTSUPPORTED;

#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
    memset(&hTxCh->stats, 0, sizeof(hTxCh->stats));
    retVal = ENET_SOK;
#endif

    return retVal;
}

int32_t EnetDma_enableTxEvent(EnetDma_TxChHandle hTxCh)
{
    int32_t retVal = ENET_SOK;

    ENETTRACE_VAR(retVal);

    if ((NULL != hTxCh) && (BTRUE == hTxCh->evtInitFlag))
    {
        retVal = Udma_eventEnable(hTxCh->hUdmaEvt);
    }
    else
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR(retVal, "TX event enable failed");
    }

    return retVal;
}

int32_t EnetDma_disableTxEvent(EnetDma_TxChHandle hTxCh)
{
    int32_t retVal = ENET_SOK;

    ENETTRACE_VAR(retVal);

    if ((NULL != hTxCh) && (BTRUE == hTxCh->evtInitFlag))
    {
        retVal = Udma_eventDisable(hTxCh->hUdmaEvt);
    }
    else
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR(retVal, "TX event disabled failed");
    }

    return retVal;
}

int32_t EnetDma_retrieveRxPktQ(EnetDma_RxChHandle hRxFlow,
                               EnetDma_PktQ *pRetrieveQ)
{
    EnetPer_Handle hPer = hRxFlow->hDma->hPer;
    int32_t retVal = UDMA_SOK;
    EnetQ tempQ;

#if ENET_CFG_IS_ON(DEV_ERROR)
    if ((NULL == hRxFlow) ||
        (NULL == pRetrieveQ))
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR_IF((NULL == hRxFlow), retVal, "hRxFlow is NULL");
        ENETTRACE_ERR_IF((NULL == pRetrieveQ), retVal, "pRetrieveQ is NULL");
        Enet_assert(BFALSE);
    }
    else
#endif
    {
#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
        uint32_t startTime, diffTime;
        uint32_t pktCnt;
        startTime = CpswOsal_timerRead();
#endif

        EnetQueue_initQ(pRetrieveQ);
        EnetUdma_drainIsrCq(pRetrieveQ, &hRxFlow->cqIsrQ);
        EnetQueue_initQ(&tempQ);

        /* EnetUdma_retrievePkts initializes the queue so cannot pass
         * pRetrieveQ as it contains packets drained from isrq
         */
        retVal = EnetUdma_retrievePkts(hPer,
                                       hRxFlow->cqRing,
                                       &tempQ,
                                       hRxFlow->hDmaDescPool,
                                       hRxFlow->rxFlowPrms.disableCacheOpsFlag,
                                       ENET_UDMA_DIR_RX);
        if (ENET_SOK == retVal)
        {
            EnetQueue_append(pRetrieveQ, &tempQ);
        }

#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
        pktCnt = EnetQueue_getQCount(pRetrieveQ);
        EnetUdmaStats_addCnt(&hRxFlow->stats.rxRetrievePktDeq, pktCnt);
        diffTime = CpswOsal_timerGetDiff(startTime);
        EnetUdmaStats_updateNotifyStats(&hRxFlow->stats.retrievePktStats, pktCnt, diffTime);
#endif
    }

    return retVal;
}

int32_t EnetDma_submitRxPktQ(EnetDma_RxChHandle hRxFlow,
                             EnetDma_PktQ *pSubmitQ)
{
    EnetPer_Handle hPer = hRxFlow->hDma->hPer;
    int32_t retVal = UDMA_SOK;

#if ENET_CFG_IS_ON(DEV_ERROR)
    if ((NULL == hRxFlow) ||
        (NULL == pSubmitQ))
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR_IF((NULL == hRxFlow), retVal, "hRxFlow is NULL");
        ENETTRACE_ERR_IF((NULL == pSubmitQ), retVal, "pSubmitQ is NULL");
        Enet_assert(BFALSE);
    }
    else
#endif
    {
#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
        uint32_t startTime, diffTime;
        uint32_t pktCnt, notifyCount;

        startTime = CpswOsal_timerRead();
        pktCnt    = EnetQueue_getQCount(pSubmitQ);
#endif

        /* Enqueue descs to fqRing regardless of caller's queue state */
        if (EnetQueue_getQCount(pSubmitQ) > 0U)
        {
            retVal = EnetUdma_submitPkts(hPer,
                                         hRxFlow->fqRing,
                                         pSubmitQ,
                                         hRxFlow->hDmaDescPool,
                                         hRxFlow->rxFlowPrms.disableCacheOpsFlag,
                                         ENET_UDMA_DIR_RX,
                                         hRxFlow->hUdmaProxy);
        }

        /* If fqRing ran out of space is not an error, packets will be re-submitted from application */
        if (UDMA_ETIMEOUT == retVal)
        {
            ENETTRACE_INFO("RX FQ underflow had occurred");
            retVal = UDMA_SOK;
        }

#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
        EnetUdmaStats_addCnt(&hRxFlow->stats.rxSubmitPktUnderFlowCnt,
                            EnetQueue_getQCount(pSubmitQ));
        pktCnt -= EnetQueue_getQCount(pSubmitQ);
        EnetUdmaStats_addCnt(&hRxFlow->stats.rxSubmitPktEnq, pktCnt);
        diffTime = CpswOsal_timerGetDiff(startTime);
        notifyCount = hRxFlow->stats.submitPktStats.dataNotifyCnt & (ENET_UDMA_STATS_HISTORY_CNT - 1U);
        hRxFlow->stats.submitPktStats.readyDmaDescQCnt[notifyCount] = EnetUdma_dmaDescQCount(hRxFlow->hDmaDescPool);
        EnetUdmaStats_updateNotifyStats(&hRxFlow->stats.submitPktStats, pktCnt, diffTime);
#endif
    }

    return retVal;
}

int32_t EnetDma_retrieveTxPktQ(EnetDma_TxChHandle hTxCh,
                               EnetDma_PktQ *pRetrieveQ)
{
    EnetPer_Handle hPer = hTxCh->hDma->hPer;
    int32_t retVal = UDMA_SOK;
    EnetQ tempQ;

#if ENET_CFG_IS_ON(DEV_ERROR)
    if ((NULL == hTxCh) ||
        (NULL == pRetrieveQ))
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR_IF((NULL == hTxCh), retVal, "hTxCh is NULL");
        ENETTRACE_ERR_IF((NULL == pRetrieveQ), retVal, "pRetrieveQ is NULL");
        Enet_assert(BFALSE);
    }
    else
#endif
    {
#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
        uint32_t startTime, diffTime;
        uint32_t pktCnt;
        startTime = CpswOsal_timerRead();
#endif

        EnetQueue_initQ(pRetrieveQ);
        EnetUdma_drainIsrCq(pRetrieveQ, &hTxCh->cqIsrQ);
        EnetQueue_initQ(&tempQ);

        /* EnetUdma_retrievePkts initializes the queue so cannot pass
         * pRetrieveQ as it contains packets drained from isrq
         */
        retVal = EnetUdma_retrievePkts(hPer,
                                       hTxCh->cqRing,
                                       &tempQ,
                                       hTxCh->hDmaDescPool,
                                       hTxCh->txChPrms.disableCacheOpsFlag,
                                       ENET_UDMA_DIR_TX);
        if (ENET_SOK == retVal)
        {
            EnetQueue_append(pRetrieveQ, &tempQ);
        }

#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
        pktCnt = EnetQueue_getQCount(pRetrieveQ);
        EnetUdmaStats_addCnt(&hTxCh->stats.txRetrievePktDeq, pktCnt);
        diffTime = CpswOsal_timerGetDiff(startTime);
        EnetUdmaStats_updateNotifyStats(&hTxCh->stats.retrievePktStats, pktCnt, diffTime);
#endif
    }
    return retVal;
}

int32_t EnetDma_submitTxPktQ(EnetDma_TxChHandle hTxCh,
                             EnetDma_PktQ *pSubmitQ)

{
    EnetPer_Handle hPer = hTxCh->hDma->hPer;
    int32_t retVal = UDMA_SOK;

#if ENET_CFG_IS_ON(DEV_ERROR)
    if ((NULL == hTxCh) ||
        (NULL == pSubmitQ))
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR_IF((NULL == hTxCh), retVal, "hTxCh is NULL");
        ENETTRACE_ERR_IF((NULL == pSubmitQ), retVal, "pSubmitQ is NULL");
        Enet_assert(BFALSE);
    }
    else
#endif
    {
#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
        uint32_t startTime, diffTime;
        uint32_t pktCnt, notifyCount;
        startTime = CpswOsal_timerRead();
        pktCnt    = EnetQueue_getQCount(pSubmitQ);
#endif

        /* Enqueue descs to fqRing regardless of caller's queue state */
        if (EnetQueue_getQCount(pSubmitQ) > 0U)
        {
            retVal = EnetUdma_submitPkts(hPer,
                                         hTxCh->fqRing,
                                         pSubmitQ,
                                         hTxCh->hDmaDescPool,
                                         hTxCh->txChPrms.disableCacheOpsFlag,
                                         ENET_UDMA_DIR_TX,
                                         hTxCh->hUdmaProxy);
        }

        /* If fqRing ran out of space it is not an error, packets will be re-submitted by application*/
        if (UDMA_ETIMEOUT == retVal)
        {
            ENETTRACE_INFO("TX Channel FQ underflow had occurred");
            retVal = UDMA_SOK;
        }

#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
        EnetUdmaStats_addCnt(&hTxCh->stats.txSubmitPktOverFlowCnt, EnetQueue_getQCount(pSubmitQ));
        pktCnt -= EnetQueue_getQCount(pSubmitQ);
        EnetUdmaStats_addCnt(&hTxCh->stats.txSubmitPktEnq, pktCnt);
        diffTime                                                  = CpswOsal_timerGetDiff(startTime);
        notifyCount                                               = hTxCh->stats.submitPktStats.dataNotifyCnt & (ENET_UDMA_STATS_HISTORY_CNT - 1U);
        hTxCh->stats.submitPktStats.readyDmaDescQCnt[notifyCount] = hTxCh->hDmaDescPool->count;

        EnetUdmaStats_updateNotifyStats(&hTxCh->stats.submitPktStats, pktCnt, diffTime);
#endif
    }

    return retVal;
}

void EnetDma_initPktInfo(EnetDma_Pkt *pktInfo)
{
    memset(&pktInfo->node, 0, sizeof(pktInfo->node));
    pktInfo->sgList.numScatterSegments = 0U;
    for(uint32_t i = 0U; i < ENET_UDMA_CPSW_MAX_SG_LIST; i++)
    {
        pktInfo->sgList.list[i].bufPtr         = NULL;
        pktInfo->sgList.list[i].segmentFilledLen = 0U;
        pktInfo->sgList.list[i].segmentAllocLen = 0U;
        pktInfo->sgList.list[i].disableCacheOps = BFALSE;
    }
    pktInfo->appPriv               = NULL;
    pktInfo->chkSumInfo            = 0U;
    pktInfo->tsInfo.enableHostTxTs = BFALSE;
    pktInfo->txPortNum             = ENET_MAC_PORT_INV;
    pktInfo->txPktTc               = ENET_TRAFFIC_CLASS_INV;
    ENET_UTILS_SET_PKT_DRIVER_STATE(&pktInfo->pktState,
                                    (uint32_t)ENET_PKTSTATE_DMA_NOT_WITH_HW);
}

/* ========================================================================== */
/*                          DMA peripheral specific Function Definitions      */
/* ========================================================================== */

void EnetUdma_initCfg(EnetDma_Cfg *pDmaConfig)
{
    pDmaConfig->hUdmaDrv                 = NULL;
    pDmaConfig->rxChInitPrms.dmaPriority = UDMA_DEFAULT_RX_CH_DMA_PRIORITY;
}

EnetDma_Handle EnetUdma_open(Enet_Type enetType,
                             uint32_t instId,
                             const EnetDma_Cfg *pEnetUdmaCfg)
{
    EnetUdma_DrvObj *pEnetUdmaObj = NULL;
    int32_t retVal;

    ENET_UTILS_COMPILETIME_ASSERT(ENET_UDMA_RING_MEM_SIZE == sizeof(uint64_t));

    /* Error check */
    retVal = UDMA_EBADARGS;
    if (NULL != pEnetUdmaCfg)
    {
        if (NULL != pEnetUdmaCfg->hUdmaDrv)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            ENETTRACE_ERR(retVal, "pEnetUdmaCfg has null UDMA handle");
        }
    }
    else
    {
        ENETTRACE_ERR(retVal, "pEnetUdmaCfg NULL");
    }

    if (UDMA_SOK == retVal)
    {
        pEnetUdmaObj = EnetSoc_getDmaHandle(enetType, instId);
        Enet_assert(pEnetUdmaObj != NULL,
                    "No DMA object for peripheral (eneType=%u instId=%u)",
                    enetType, instId);

        pEnetUdmaObj->initFlag = BTRUE;
        pEnetUdmaObj->hUdmaDrv = pEnetUdmaCfg->hUdmaDrv;
        pEnetUdmaObj->numRxCh = 1U;
        Enet_assert(pEnetUdmaObj->numRxCh <= ENET_UDMA_NUM_RXCHAN_MAX);

        /* Initialize memory manager module managing driver object memories */
        EnetUdma_memMgrInit();
    }

    return pEnetUdmaObj;
}

int32_t EnetUdma_close(EnetDma_Handle hEnetUdma)
{
    int32_t retVal = UDMA_SOK;

    // TODO check all Rx & Tx channels & are closed
    /* Error check */
    if (hEnetUdma == NULL)
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR(retVal, "Enet UDMA handle is NULL");
    }

    if (UDMA_SOK == retVal)
    {
        if (hEnetUdma->initFlag == BFALSE)
        {
            retVal = UDMA_EFAIL;
            ENETTRACE_ERR(retVal, "DMA was not initialized, cannot be closed");
        }
    }

    if (UDMA_SOK == retVal)
    {
        /* De-initialize driver memory manager */
        EnetUdma_memMgrDeInit();
        hEnetUdma->initFlag = BFALSE;
    }

    return retVal;
}

int32_t EnetUdma_openRxCh(EnetDma_Handle hEnetUdma,
                          const EnetUdma_RxChInitPrms *pRxChInitPrms,
                          uint32_t totalRxFlowCount,
                          uint32_t chIdx)
{
    int32_t retVal;
    EnetPer_Handle hPer = NULL;
    Udma_DrvHandle hUdmaDrv;
    EnetUdma_RxChObj *pRxCh;
    Udma_ChHandle hUdmaCh;
    Udma_FlowHandle hFlow;
    Udma_ChPrms chPrms;
    Udma_ChRxPrms rxPrms;
    bool allocFlow = BFALSE, chOpenFlag = BFALSE;
    uint32_t chType, rxFlowCount;
    uintptr_t intrKey;

    void *pTdCqRingMem;

    /* Error check */
    retVal = UDMA_EBADARGS;
    if (hEnetUdma != NULL)
    {
        if (hEnetUdma->initFlag)
        {
            hPer = hEnetUdma->hPer;
            retVal = UDMA_SOK;
        }
        else
        {
            ENETTRACE_ERR(retVal, "Enet UDMA driver not initialized");
        }
    }
    else
    {
        ENETTRACE_ERR(retVal, "Enet UDMA handle is NULL");
    }

    if (UDMA_SOK == retVal)
    {
        rxFlowCount = EnetSoc_getRxFlowCount(hPer->enetType, hPer->instId);
        if (totalRxFlowCount > rxFlowCount)
        {
            /* flows num greater than CPSW instance max. supported */
            retVal = UDMA_EBADARGS;
            ENETTRACE_ERR(retVal,
                          "Invalid Rx channel flow count (req=%d, SoC available=%d)",
                          totalRxFlowCount, rxFlowCount);
        }
    }

    intrKey = EnetOsal_disableAllIntr();

    if (UDMA_SOK == retVal)
    {
        hUdmaDrv = hEnetUdma->hUdmaDrv;
        pRxCh    = &hEnetUdma->rxChObj[chIdx];
        hUdmaCh  = &pRxCh->udmaChObj;
        chType   = UDMA_CH_TYPE_RX;

        /* Initialize channel params (PSI-L thread and RingAcc memories) */
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = EnetSoc_getRxChPeerId(hPer->enetType, hPer->instId, chIdx);
        chPrms.chNum     = UDMA_DMA_CH_ANY;

        UdmaRingPrms_init(&chPrms.tdCqRingPrms);
        pTdCqRingMem = EnetUdma_memMgrAllocTdCqRingMemObj();
        if (pTdCqRingMem != NULL)
        {
            chPrms.tdCqRingPrms.ringMem = pTdCqRingMem;
            chPrms.tdCqRingPrms.ringMemSize = sizeof(uint64_t);
            chPrms.tdCqRingPrms.elemCnt = ENET_UDMA_TDCQ_RING_ELE_CNT;
            chPrms.tdCqRingPrms.mode    = CSL_RINGACC_RING_MODE_RING;

            /* Open the UDMA channel */
            retVal = Udma_chOpen(hUdmaDrv, hUdmaCh, chType, &chPrms);
            ENETTRACE_ERR_IF((retVal != UDMA_SOK), retVal, "RX channel open failed");
            if (UDMA_SOK == retVal)
            {
                chOpenFlag = BTRUE;
            }
        }
        else
        {
            retVal = UDMA_EALLOC;
            ENETTRACE_ERR(retVal, "RX TDCQ ring mem alloc failed");
        }
    }

    if (UDMA_SOK == retVal)
    {
        /* Configure RX parameters */
        hFlow  = &pRxCh->flowUdmaObj;

        retVal = Udma_flowAlloc(hUdmaDrv, hFlow, totalRxFlowCount);
        ENETTRACE_ERR_IF((retVal != UDMA_SOK), retVal, "RX flow alloc failed");
        if (UDMA_SOK == retVal)
        {
            allocFlow = BTRUE;

            if (totalRxFlowCount != Udma_flowGetCount(hFlow))
            {
                retVal = UDMA_EALLOC;
                ENETTRACE_ERR(retVal,
                              "Allocated flow count (%u) doesn't match num requested (%u)",
                              Udma_flowGetCount(hFlow), totalRxFlowCount);
            }
        }
    }

    if (UDMA_SOK == retVal)
    {
        UdmaChRxPrms_init(&rxPrms, chType);

        rxPrms.dmaPriority        = pRxChInitPrms->dmaPriority;
        rxPrms.flowIdFwRangeStart = Udma_flowGetNum(hFlow);
        rxPrms.flowIdFwRangeCnt   = Udma_flowGetCount(hFlow);

        rxPrms.configDefaultFlow = UFALSE;

        retVal = Udma_chConfigRx(hUdmaCh, &rxPrms);
        if (UDMA_SOK == retVal)
        {
            /* Enable the UDMA channel */
            retVal = Udma_chEnable(&pRxCh->udmaChObj);
        }
        else
        {
            ENETTRACE_ERR(retVal, "RX channel enable failed, closing channel");
            retVal = Udma_chClose(hUdmaCh);
            chOpenFlag = BFALSE;
        }
    }

    /* Get teardown cqRing handle */
    if (UDMA_SOK == retVal)
    {
        pRxCh->tdCqRing = Udma_chGetTdCqRingHandle(hUdmaCh);
        if (NULL == pRxCh->tdCqRing)
        {
            retVal = UDMA_EALLOC;
            ENETTRACE_ERR(retVal, "RX TDCQ ring handle is invalid");
        }
    }

    if (UDMA_SOK == retVal)
    {
        /* Save channel config */
        hEnetUdma->rxChObj[chIdx].rxChInitPrms = *pRxChInitPrms;
        pRxCh->initFlag = BTRUE;
    }
    else
    {
        /* Error. Free-up resource if allocated */
        if (chOpenFlag)
        {
            retVal = Udma_chClose(hUdmaCh);
            ENETTRACE_ERR_IF((UDMA_SOK != retVal), retVal, "RX channel close failed");
        }

        if (allocFlow)
        {
            retVal = Udma_flowFree(&pRxCh->flowUdmaObj);
            ENETTRACE_ERR_IF((UDMA_SOK != retVal), retVal, "RX flow free failed");
        }

        /* return error as channel open has failed */
        retVal = UDMA_EFAIL;
    }

    EnetOsal_restoreAllIntr(intrKey);

    return retVal;
}

int32_t EnetUdma_closeRxCh(EnetDma_Handle hEnetUdma,
                           uint32_t chIdx)
{
    int32_t retVal = UDMA_SOK;
    EnetUdma_RxChObj *pRxCh;
    CSL_UdmapTdResponse tdResp;
    uintptr_t intrKey;
    void *tdCqRingMemPtr = NULL;

    // TODO how to check all flows are closed?
    // TODO check all Rx flows & Tx channels & are closed

    /* Error check */
    if (hEnetUdma == NULL)
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR(retVal, "DMA handle is NULL");
    }

    if ((UDMA_SOK == retVal) && (!hEnetUdma->initFlag))
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR(retVal, "DMA not initialized");
    }

    if (UDMA_SOK == retVal)
    {
        pRxCh = &hEnetUdma->rxChObj[chIdx];
        if (!pRxCh->initFlag)
        {
            retVal = ENET_EFAIL;
            ENETTRACE_ERR(retVal, "RX channel not initialized");
        }
    }

    intrKey = EnetOsal_disableAllIntr();

    if (UDMA_SOK == retVal)
    {
        /* Disable the UDMA channel */
        retVal += Udma_chDisable(&pRxCh->udmaChObj, ENET_UDMA_TEARDOWN_TIME_MS);

        /* Dequeue the teardown response */
        if (UDMA_SOK == retVal)
        {
            tdCqRingMemPtr = Udma_ringGetMemPtr(pRxCh->tdCqRing);

            do
            {
                retVal = Udma_chDequeueTdResponse(&pRxCh->udmaChObj, &tdResp);
            }
            while (UDMA_SOK != retVal);

            if (BFALSE == tdResp.tdIndicator)
            {
                retVal += UDMA_EFAIL;
                ENETTRACE_ERR(retVal, "RX channel teardown failed");
            }

            /* Free tdCq ring memory */
            EnetUdma_memMgrFreeTdCqRingMemObj(tdCqRingMemPtr);
        }
    }

    // TODO flush FQ ring

    if (UDMA_SOK == retVal)
    {
        /* free all flow ids allocated for this channel */
        retVal += Udma_flowFree(&pRxCh->flowUdmaObj);
        ENETTRACE_ERR_IF((UDMA_SOK != retVal), retVal, "RX flow free failed");
    }

    if (UDMA_SOK == retVal)
    {
        /* Close the UDMA channel */
        retVal += Udma_chClose(&pRxCh->udmaChObj);
        ENETTRACE_ERR_IF((UDMA_SOK != retVal), retVal, "RX channel close failed");
    }

    EnetOsal_restoreAllIntr(intrKey);

    return retVal;
}

Udma_RingHandle EnetUdma_getTxChFqHandle(EnetDma_TxChHandle hTxCh)
{
    Enet_assert(NULL != hTxCh);
    Enet_assert(NULL != hTxCh->fqRing);

    return hTxCh->fqRing;
}

EnetUdma_DmaDescQHandle EnetUdma_getTxChDescPoolHandle(EnetDma_TxChHandle hTxCh)
{
    Enet_assert(NULL != hTxCh);
    return hTxCh->hDmaDescPool;
}

Udma_RingHandle EnetUdma_getRxFlowFqHandle(EnetDma_RxChHandle hRxFlow)
{
    Enet_assert(NULL != hRxFlow);
    Enet_assert(NULL != hRxFlow->fqRing);

    return hRxFlow->fqRing;
}

EnetUdma_DmaDescQHandle EnetUdma_getRxFlowDescPoolHandle(EnetDma_RxChHandle hRxFlow)
{
    Enet_assert(NULL != hRxFlow);
    return hRxFlow->hDmaDescPool;
}

uint32_t EnetUdma_getRxChFlowStartIdx(EnetDma_Handle hEnetUdma,
                                      uint32_t chIdx)
{
    EnetUdma_RxChObj *pRxCh = &hEnetUdma->rxChObj[chIdx];
    uint32_t flowStartIdx;

    flowStartIdx = Udma_flowGetNum(&pRxCh->flowUdmaObj);

    return flowStartIdx;
}

uint32_t EnetUdma_getRxFlowCnt(EnetDma_Handle hEnetUdma,
                               uint32_t chIdx)
{
    EnetUdma_RxChObj *pRxCh = &hEnetUdma->rxChObj[chIdx];
    uint32_t flowCnt;

    flowCnt = Udma_flowGetCount(&pRxCh->flowUdmaObj);

    return flowCnt;
}

int32_t EnetUdma_checkRxFlowSanity(EnetDma_RxChHandle hRxFlow,
                                  uint32_t margin)
{
    int32_t retVal       = ENET_SOK;
    uint32_t allocPkts   = hRxFlow->rxFlowPrms.numRxPkts;
    uint32_t fqRingHwOcc = CSL_ringaccGetRingHwOcc(&hRxFlow->fqRing->drvHandle->raRegs,
                                                   hRxFlow->fqRing->ringNum);
    uint32_t cqRingHwOcc = CSL_ringaccGetRingHwOcc(&hRxFlow->cqRing->drvHandle->raRegs,
                                                   hRxFlow->cqRing->ringNum);
    uint32_t withHwPkts = fqRingHwOcc + cqRingHwOcc;

    /* App allocated should match packets with DMA module and HW.
     * Also we consider margin packets in fly while reading the HW status */
    if ((allocPkts - margin) < (EnetUdma_dmaDescQCount(hRxFlow->hDmaDescPool) + withHwPkts))
    {
        retVal = ENET_SOK;
    }
    else
    {
        ENETTRACE_WARN("hRxFlow->rxFlowPrms.numTxPkts   = %d", hRxFlow->rxFlowPrms.numRxPkts);
        ENETTRACE_WARN("hRxFlow->hDmaDescPool->count    = %d", hRxFlow->hDmaDescPool->count);
        ENETTRACE_WARN("hRxFlow->fqRing->pRtRegs->HWOCC = %d", fqRingHwOcc);
        ENETTRACE_WARN("hRxFlow->cqRing->pRtRegs->HWOCC = %d", cqRingHwOcc);
        retVal = ENET_EFAIL;
    }

    ENETTRACE_ERR_IF((ENET_SOK != retVal), retVal, "RX channel sanity check failed");

    return retVal;
}

int32_t EnetUdma_checkTxChSanity(EnetDma_TxChHandle hTxCh,
                                uint32_t margin)
{
    int32_t retVal       = ENET_SOK;
    uint32_t allocPkts   = hTxCh->txChPrms.numTxPkts;
    uint32_t fqRingHwOcc = CSL_ringaccGetRingHwOcc(&hTxCh->fqRing->drvHandle->raRegs,
                                                   hTxCh->fqRing->ringNum);
    uint32_t cqRingHwOcc = CSL_ringaccGetRingHwOcc(&hTxCh->cqRing->drvHandle->raRegs,
                                                   hTxCh->cqRing->ringNum);
    uint32_t withHwPkts = fqRingHwOcc + cqRingHwOcc;

    /* App allocated should match packets with DMA module and HW.
     * Also we consider margin packets in fly while reading the HW status */
    if ((allocPkts - margin) < (hTxCh->hDmaDescPool->count + withHwPkts))
    {
        retVal = ENET_SOK;
    }
    else
    {
        ENETTRACE_WARN("hTxCh->txChPrms.numTxPkts     = %d", hTxCh->txChPrms.numTxPkts);
        ENETTRACE_WARN("hTxCh->hDmaDescPool->count    = %d", hTxCh->hDmaDescPool->count);
        ENETTRACE_WARN("hTxCh->fqRing->pRtRegs->HWOCC = %d", fqRingHwOcc);
        ENETTRACE_WARN("hTxCh->cqRing->pRtRegs->HWOCC = %d", cqRingHwOcc);
        retVal = ENET_EFAIL;
    }

    ENETTRACE_ERR_IF((ENET_SOK != retVal), retVal, "TX channel sanity check failed");

    return retVal;
}

static int32_t EnetUdma_checkRxFlowParams(const EnetUdma_OpenRxFlowPrms *pRxFlowPrms)
{
    int32_t retVal = UDMA_SOK;

    if (NULL == pRxFlowPrms)
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR(retVal, "Flow params is NULL");
    }
    else
    {
        if (NULL == pRxFlowPrms->hUdmaDrv)
        {
            retVal = UDMA_EBADARGS;
            ENETTRACE_ERR(retVal, "UDMA not opened");
        }
    }

    if (UDMA_SOK == retVal)
    {
        if (ENET_RM_RXFLOWIDX_INVALID == pRxFlowPrms->flowIdx)
        {
            retVal = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(retVal, "RX flow id %u is invalid", pRxFlowPrms->flowIdx);
        }
    }

    if ((UDMA_SOK == retVal) && (pRxFlowPrms->udmaChPrms.cqRingPrms.useRingMon))
    {
        retVal = ENET_ENOTSUPPORTED;
        ENETTRACE_ERR(retVal, "Driver supports Ring monitor only for RX FQ");
    }

    return retVal;
}

static int32_t EnetUdma_checkTxChParams(const EnetUdma_OpenTxChPrms *pTxChPrms)
{
    int32_t retVal = UDMA_SOK;

    ENETTRACE_VAR(retVal);

    if (NULL == pTxChPrms)
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR(retVal, "TX channel params can't be NULL");
    }
    else
    {
        if (NULL == pTxChPrms->hUdmaDrv)
        {
            /* DMA should be opened before opening channel/flow */
            retVal = UDMA_EBADARGS;
            ENETTRACE_ERR(retVal, "UDMA not opened");
        }
    }

    if (UDMA_SOK == retVal)
    {
        if (ENET_RM_TXCHNUM_INVALID == pTxChPrms->chNum)
        {
            retVal = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(retVal, "TX channel number %u is invalid", pTxChPrms->chNum);
        }
    }

    if ((UDMA_SOK == retVal) &&
        ((pTxChPrms->udmaChPrms.fqRingPrms.useRingMon) || (pTxChPrms->udmaChPrms.cqRingPrms.useRingMon)))
    {
        retVal = ENET_ENOTSUPPORTED;
        ENETTRACE_ERR(retVal, "Driver supports ring monitor only for RX FQ");
    }

    return retVal;
}

/*
 * Drain packets in ISR due to ring level fix
 * Udma ring completion (CQ) interrupt is a self clearing level interrupt.
 * The CQ ISR must drain the ring in Udma callback. This requires a fix in Udma driver
 * to not clear ring interrupt explicitly and change in CpswDma to drain buffers in ISR callback
 * Without this change, Ring interrupt will randomly stop occurring
 */
static void EnetUdma_drainIsrCq(EnetQ *dstQ,
                                EnetQ *isrCq)
{
    uintptr_t key = EnetOsal_disableAllIntr();

    EnetQueue_append(dstQ, isrCq);
    EnetQueue_initQ(isrCq);

    EnetOsal_restoreAllIntr(key);
}

void EnetUdma_initDataPathParams(EnetDma_initCfg *pDmaConfig)
{
    pDmaConfig->hUdmaDrv = NULL;
}

EnetDma_Handle EnetUdma_initDataPath(Enet_Type enetType,
                                     uint32_t instId,
                                     const EnetDma_initCfg *pDmaInitCfg)
{
    EnetDma_Cfg cfg;
    Enet_Handle hEnet = NULL;
    EnetDma_Handle hDmaHandle = NULL;

    EnetUdma_initCfg(&cfg);

    if (pDmaInitCfg != NULL)
    {
        cfg.hUdmaDrv = pDmaInitCfg->hUdmaDrv;

        hDmaHandle = EnetUdma_open(enetType, instId, &cfg);
        if (hDmaHandle != NULL)
        {
            hEnet = EnetSoc_getEnetHandle(enetType, instId);
            if (hEnet != NULL)
            {
                hDmaHandle->hPer = hEnet->enetPer;
            }
            else
            {
                Enet_devAssert(hEnet != NULL);
            }
        }
    }

    return hDmaHandle;
}

int32_t EnetUdma_deInitDataPath(EnetDma_Handle hEnetUdma)
{
    int32_t status;

    status = EnetUdma_close(hEnetUdma);
    return status;
}

/* End of file */
