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
 *  \file enet_udma_priv.c
 *
 *  \brief This file contains UDMA private function definations.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x302

#include <string.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/enet/include/core/enet_types.h>
#include <ti/drv/enet/include/core/enet_queue.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/include/core/enet_osal.h>
#include <ti/drv/enet/priv/core/enet_trace_priv.h>
#include <ti/drv/enet/include/mod/cpsw_ale.h>
#include <ti/drv/enet/include/core/enet_rm.h>
#include <ti/drv/enet/include/core/enet_dma.h>

#include <ti/drv/enet/priv/per/enet_hostport_udma.h>

#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>

#include "enet_udma_priv.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

typedef EnetQ EnetUdma_RxFlowObjMemQ;

typedef EnetQ EnetUdma_TxChObjMemQ;

typedef EnetQ EnetUdma_RingMonObjMemQ;

typedef EnetQ EnetUdma_tdCqRingObjMemQ;

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief
 */
typedef struct EnetUdma_TxChObjMem_s
{
    /*! The node element so this packet can be added to a queue
     * Note- Keep EnetQ_Node as first member always as driver uses generic Q functions
     *       and deferences to this member */
    EnetQ_Node node;

    /*! Tx channel object mem element */
    EnetUdma_TxChObj txChObj;
} EnetUdma_TxChObjMem;

/**
 *  \brief
 */
typedef struct EnetUdma_RxFlowObjMem_s
{
    /*! The node element so this packet can be added to a queue
     * Note- Keep EnetQ_Node as first member always as driver uses generic Q functions
     *       and deferences to this member */
    EnetQ_Node node;

    /*! Rx flow object mem element */
    EnetUdma_RxFlowObj rxFlowObj;
} EnetUdma_RxFlowObjMem;

/**
 *  \brief
 */
typedef struct EnetUdma_RingMonObjMem_s
{
    /*! The node element so this packet can be added to a queue
     * Note- Keep EnetQ_Node as first member always as driver uses generic Q functions
     *       and deferences to this member */
    EnetQ_Node node;

    /*! Ring monitor object mem element */
    struct Udma_RingMonObj ringMonObj;
} EnetUdma_RingMonObjMem;

/**
 *  \brief
 */
typedef struct EnetUdma_TdCqRingObjMem_s
{
    /*! The node element so this packet can be added to a queue
     * Note- Keep EnetQ_Node as first member always as driver uses generic Q functions
     *       and deferences to this member */
    EnetQ_Node node;

    /*! Teardown Cq Ring object mem element. Allocating cahce aligned start address and size,
     *  to avoid cache operations on other memory regions while cache invalidating after
     *  popping from the ring. Only allocating single element i.e. ENET_UDMA_TDCQ_RING_ELE_CNT of 1,
     *  as it stores the return status */
    uint64_t tdCqRingMemObj [ENET_UTILS_ALIGN(ENET_UDMA_TDCQ_RING_ELE_CNT, ENETDMA_CACHELINE_ALIGNMENT)]
    __attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT)));
} EnetUdma_TdCqRingObjMem;

/**
 *  \brief
 */
typedef struct EnetUdma_MemMgrObj_s
{
    /*! Mem manager init flag. Used to track no. of clients using mem manager */
    uint32_t clientCnt;

    /*! RX flow driver object Q */
    EnetUdma_RxFlowObjMemQ rxFlowObjMemQ;

    /*! TX channel driver object Q */
    EnetUdma_TxChObjMemQ txChObjMemQ;

    /*! TX channel driver object Q */
    EnetUdma_RingMonObjMemQ ringMonObjMemQ;

    /*! Teardown CQ memory Q */
    EnetUdma_tdCqRingObjMemQ tdCqRingObjMemQ;
} EnetUdma_MemMgrObj;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* RX flow object memories */
static EnetUdma_RxFlowObjMem gEnetUdmaRxFlowObjMem[ENET_CFG_RX_FLOWS_NUM]
__attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT), section(".bss:ENET_DMA_OBJ_MEM")));

/* Tx channel object memories */
static EnetUdma_TxChObjMem gEnetUdmaTxChObjMem[ENET_CFG_TX_CHANNELS_NUM]
__attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT), section(".bss:ENET_DMA_OBJ_MEM")));

/* Ring monitor object memories */
static EnetUdma_RingMonObjMem gEnetUdmaRinMonObjMem[ENET_CFG_RING_MON_NUM]
__attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT), section(".bss:ENET_DMA_OBJ_MEM")));

static uint8_t gRsvdFlowRingMemFQ[ENET_UDMA_RSVDFLOW_RX_PKTS_NUM][ENET_UDMA_RING_MEM_SIZE]
__attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT), section(".bss:ENET_DMA_OBJ_MEM")));

static uint8_t gRsvdFlowRingMemCQ[ENET_UDMA_RSVDFLOW_RX_PKTS_NUM][ENET_UDMA_RING_MEM_SIZE]
__attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT), section(".bss:ENET_DMA_OBJ_MEM")));

/* Teardown Cq ring object memories */
static EnetUdma_TdCqRingObjMem gEnetUdmaTdCqRingObjMem[ENET_CFG_TX_CHANNELS_NUM +
                                                       ENET_UDMA_RX_CH_NUM]
__attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT), section(".bss:ENET_DMA_OBJ_MEM")));

/* Cpsw mem utils driver object */
static EnetUdma_MemMgrObj gEnetUdmaMemMgrObj = {.clientCnt = 0U};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static int32_t EnetUdma_processRetrievedDesc(EnetPer_Handle hPer,
                                             EnetUdma_DmaDesc *pDmaDesc,
                                             EnetDma_PktQ *pFromHwQueue,
                                             EnetUdma_DmaDescQ *pDmaDescQ,
                                             EnetUdma_Dir transferDir)
{
    int32_t retVal = UDMA_SOK;
    EnetUdma_CpswHpdDesc *pHpdDesc;
    EnetDma_Pkt *dmaPkt;
    EnetUdma_CppiTxStatus *cppiTxStatus;
    uint32_t srcTag;

    if (NULL != pDmaDesc)
    {
        pHpdDesc = &pDmaDesc->hpdDesc;

        /* Check that the desc address from cqRing is valid */
        if (NULL != pHpdDesc)
        {
            dmaPkt = pDmaDesc->dmaPkt;
            if (ENET_UDMA_DIR_RX == transferDir)
            {
                uint32_t i = 0U;
                uint32_t lastFilledSegmentIndex;

                lastFilledSegmentIndex = 0U;
                while (i < dmaPkt->sgList.numScatterSegments)
                {
                    if (i == ENET_UDMA_CPSW_HOSTPKTDESC_INDEX)
                    {
                        dmaPkt->sgList.list[i].segmentFilledLen = CSL_udmapCppi5GetBufferLen(&pHpdDesc->hostDesc);
                    }
                    else
                    {
                        dmaPkt->sgList.list[i].segmentFilledLen = CSL_udmapCppi5GetBufferLen(&(pDmaDesc->hostBufDesc[(i - ENET_UDMA_CPSW_HOSTBUFDESC_INDEX)].desc));
                    }
                    if (dmaPkt->sgList.list[i].segmentFilledLen != 0U)
                    {
                        lastFilledSegmentIndex = i;
                    }
                    i++;
                }
                if (Enet_isIcssFamily(hPer->enetType))
                {
                    Enet_assert((lastFilledSegmentIndex < ENET_ARRAYSIZE(dmaPkt->sgList.list)) && (dmaPkt->sgList.list[lastFilledSegmentIndex].segmentFilledLen >= 4));
                    dmaPkt->sgList.list[lastFilledSegmentIndex].segmentFilledLen -= 4U;
                }
            }

            if (ENET_UDMA_DIR_RX == transferDir)
            {
#if ENET_CFG_IS_ON(DEV_ERROR)
                /* Protocol-specific data is should be in the descriptor (psLocation set to
                 * TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_PS_END_PD */
                Enet_assert(0U == CSL_udmapCppi5GetPsDataLoc(&pHpdDesc->hostDesc));
                /* EPI(Extended packet info) block should be present in the descriptor (einfoPresent set to
                 * TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_EINFO_PRESENT */
                Enet_assert(BTRUE == CSL_udmapCppi5IsEpiDataPresent(&pHpdDesc->hostDesc));
#endif
                cppiTxStatus           = (EnetUdma_CppiTxStatus *)pHpdDesc->psInfo;
                dmaPkt->chkSumInfo     = cppiTxStatus->chkSumInfo;

                dmaPkt->tsInfo.rxPktTs = ((((uint64_t)cppiTxStatus->tsHigh) << 32U) |
                                                ((uint64_t)cppiTxStatus->tsLow));

                /* Convert timestamp to nanosecs if peripheral provides a conversion function */
                if (hPer->convertTs != NULL)
                {
                    dmaPkt->tsInfo.rxPktTs = hPer->convertTs(hPer, dmaPkt->tsInfo.rxPktTs);
                }

                srcTag = CSL_udmapCppi5GetSrcTag(&pHpdDesc->hostDesc) &
                                  ENET_UDMA_HPD_SRC_TAG_LOW_MASK;
                dmaPkt->rxPortNum = CPSW_ALE_ALEPORT_TO_MACPORT(srcTag);
                dmaPkt->errCode = CSL_udmapCppi5GetErrorFlags(&pHpdDesc->hostDesc);
            }
            else
            {
                dmaPkt->errCode = 0U;
            }

#if ENET_CFG_IS_ON(DEV_ERROR)
            Enet_assert(dmaPkt->sgList.list[0].segmentAllocLen == pHpdDesc->hostDesc.orgBufLen);
            Enet_assert((uint64_t)dmaPkt->sgList.list[0].bufPtr == CSL_udmapCppi5GetBufferAddr(&pHpdDesc->hostDesc));
            Enet_assert((uint64_t)dmaPkt->sgList.list[0].bufPtr == pHpdDesc->hostDesc.orgBufPtr);
#endif

            EnetDma_checkPktState(&dmaPkt->pktState,
                                    ENET_PKTSTATE_MODULE_DRIVER,
                                    (uint32_t)ENET_PKTSTATE_DMA_WITH_HW,
                                    (uint32_t)ENET_PKTSTATE_DMA_NOT_WITH_HW);

            EnetQueue_enq(pFromHwQueue, &dmaPkt->node);

            /* Enq free Tx packet in txFreeDmaDescQ and full Rx into rxReadyDmaDescQ */
            EnetUdma_dmaDescEnque(pDmaDescQ, pDmaDesc);
        }
        else
        {
            retVal = UDMA_EFAIL;
        }
    }
    else
    {
        retVal = UDMA_EFAIL;
    }

    return retVal;
}

int32_t EnetUdma_retrievePkts(EnetPer_Handle hPer,
                              Udma_RingHandle hUdmaRing,
                              EnetDma_PktQ *pFromHwQueue,
                              EnetUdma_DmaDescQ *pDmaDescQ,
                              bool disableCacheOpsFlag,
                              EnetUdma_Dir transferDir)
{
    int32_t retVal = UDMA_SOK;
    EnetUdma_DmaDesc *pDmaDesc;
    bool isExposedRing;
    uint32_t ringOcc = 0U;
    uint64_t *ringMemPtr = NULL, *currRingMemPtr = NULL;
    uint32_t ringMemEleCnt = 0U, ringRdIdx = 0U;
    uint32_t i = 0U;
    int32_t retrieveCnt = 0;

    EnetQueue_initQ(pFromHwQueue);
    isExposedRing = (Udma_ringGetMode(hUdmaRing) == CSL_RINGACC_RING_MODE_RING);

    if (isExposedRing == BTRUE)
    {
        /* Disable interrupts before retrieving descriptors using Prime APIs to prevent race condition.
         * As ring occupancy is updated by the driver only at the end of this function, CQ interrupts
         * occurring in b/w might result in context switch and update the occupancy value more than once
         * for same set of descriptors. */
        uintptr_t key = EnetOsal_disableAllIntr();

        ringOcc = Udma_ringGetReverseRingOcc(hUdmaRing);
        while (ringOcc != 0U)
        {
            ringMemPtr = (uint64_t *)Udma_ringGetMemPtr(hUdmaRing);
            ringMemEleCnt = Udma_ringGetElementCnt(hUdmaRing);
            ringRdIdx = Udma_ringGetRdIdx(hUdmaRing);
            currRingMemPtr = ringMemPtr + ringRdIdx;

            /* Invalidate the Ring memory before reading from the ring */
            if ((ringRdIdx + ringOcc) > ringMemEleCnt)
            {
                EnetOsal_cacheInv(currRingMemPtr,
                                  ((ringMemEleCnt - ringRdIdx) * ENET_UDMA_RING_MEM_SIZE));
                EnetOsal_cacheInv(ringMemPtr,
                                  ((ringOcc - ringMemEleCnt + ringRdIdx) * ENET_UDMA_RING_MEM_SIZE));
            }
            else
            {
                EnetOsal_cacheInv(currRingMemPtr,
                                  (ringOcc * ENET_UDMA_RING_MEM_SIZE));
            }

            for (i = 0U; i < ringOcc; i++)
            {
                retVal = EnetUdma_ringDequeue(hUdmaRing,
                                             &pDmaDesc,
                                             disableCacheOpsFlag,
                                             transferDir);

                if ((UDMA_SOK == retVal) && (NULL != pDmaDesc))
                {
                    retVal = EnetUdma_processRetrievedDesc(hPer,
                                                           pDmaDesc,
                                                           pFromHwQueue,
                                                           pDmaDescQ,
                                                           transferDir);
                    if (UDMA_SOK != retVal)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }

            if ((retVal == UDMA_SOK) && (ringOcc > 0U))
            {
                /* Make the count negative as the occupancy needs to be reduced after popping from ring */
                retrieveCnt = -1 * (int32_t)ringOcc;
                /* Set Ring door bell register with count of number of descriptors de-queued */
                Udma_ringSetDoorBell(hUdmaRing, retrieveCnt);
            }

            ringOcc = Udma_ringGetReverseRingOcc(hUdmaRing);
        }

        /* Re-enable interrupts */
        EnetOsal_restoreAllIntr(key);
    }
    /* If ring is in message mode */
    else
    {
        /* Dequeue descs from cqRing */
        retVal = EnetUdma_ringDequeue(hUdmaRing,
                                     &pDmaDesc,
                                     disableCacheOpsFlag,
                                     transferDir);

        while ((retVal == UDMA_SOK) && (NULL != pDmaDesc))
        {
            retVal = EnetUdma_processRetrievedDesc(hPer,
                                                   pDmaDesc,
                                                   pFromHwQueue,
                                                   pDmaDescQ,
                                                   transferDir);
            if (UDMA_SOK != retVal)
            {
                break;
            }

            retVal = EnetUdma_ringDequeue(hUdmaRing,
                                         &pDmaDesc,
                                         disableCacheOpsFlag,
                                         transferDir);
        }
    }

    /* It's ok if there are no more descs in cqRing */
    if (UDMA_ETIMEOUT == retVal)
    {
        retVal = UDMA_SOK;
    }

    return retVal;
}

int32_t EnetUdma_submitPkts(EnetPer_Handle hPer,
                            Udma_RingHandle hUdmaRing,
                            EnetDma_PktQ *pToHwQueue,
                            EnetUdma_DmaDescQ *pDmaDescQ,
                            bool disableCacheOpsFlag,
                            EnetUdma_Dir transferDir,
                            Udma_ProxyHandle hUdmaProxy)
{
    int32_t retVal = UDMA_SOK;
    EnetUdma_DmaDesc *pDmaDesc;
    EnetDma_Pkt *dmaPkt;
    uint32_t dstTag;
    EnetUdma_CppiRxControl *cppiRxCntr;
    uint32_t *tsInfo;
    int32_t submitCnt = 0;
    bool isExposedRing;
    uint64_t *ringMemPtr = NULL, *currRingMemPtr = NULL;
    uint32_t ringWrIdx = 0U, ringMemEleCnt = 0U;
    uint32_t i;
    uint32_t totalPacketFilledLen = 0U;
    CSL_UdmapCppi5HMPD *pDescEntry;

    ENET_UTILS_COMPILETIME_ASSERT(ENET_UDMA_CPSW_HOSTPKTDESC_INDEX < ENET_UDMA_CPSW_HOSTBUFDESC_INDEX);
    isExposedRing = (Udma_ringGetMode(hUdmaRing) == CSL_RINGACC_RING_MODE_RING);
    if (isExposedRing == BTRUE)
    {
        ringMemPtr = (uint64_t *)Udma_ringGetMemPtr(hUdmaRing);
        ringWrIdx = Udma_ringGetWrIdx(hUdmaRing);
        ringMemEleCnt = Udma_ringGetElementCnt(hUdmaRing);
    }

    /* Enqueue packets until fqRing is full */
    dmaPkt = (EnetDma_Pkt *)EnetQueue_deq(pToHwQueue);
    while (NULL != dmaPkt)
    {
        /* Enqueue desc to fqRing */
        pDmaDesc = EnetUdma_dmaDescDeque(pDmaDescQ);
        totalPacketFilledLen = 0U;

        if (NULL != pDmaDesc)
        {
            EnetUdma_CpswHpdDesc *pHpdDesc = (EnetUdma_CpswHpdDesc *)pDmaDesc;
            CSL_UdmapCppi5HMPD *pHostDesc = &pHpdDesc->hostDesc;
            CSL_UdmapCppi5HMPD *pLastDescEntry;

            pDmaDesc->dmaPkt = dmaPkt;
            EnetDma_checkPktState(&pDmaDesc->dmaPkt->pktState,
                                    ENET_PKTSTATE_MODULE_DRIVER,
                                    (uint32_t)ENET_PKTSTATE_DMA_NOT_WITH_HW,
                                    (uint32_t)ENET_PKTSTATE_DMA_WITH_HW);

            pLastDescEntry = NULL;
            pDescEntry = &pHpdDesc->hostDesc;
            Enet_assert(dmaPkt->sgList.numScatterSegments >= 1U);
            for (i = 0U; i < dmaPkt->sgList.numScatterSegments; i++)
            {

                if (i > ENET_UDMA_CPSW_HOSTPKTDESC_INDEX)
                {
                    pDescEntry = &pDmaDesc->hostBufDesc[i - ENET_UDMA_CPSW_HOSTBUFDESC_INDEX].desc;
                }
                if (pLastDescEntry != NULL)
                {
                    /* Link the HostPktDesc to the first HostBufDesc */
                    CSL_udmapCppi5LinkDesc(pLastDescEntry, (uintptr_t) pDescEntry);
                }

                CSL_udmapCppi5SetBufferAddr(pDescEntry, (uint64_t)dmaPkt->sgList.list[i].bufPtr);

                CSL_udmapCppi5SetOrgBufferAddr(pDescEntry, (uint64_t)dmaPkt->sgList.list[i].bufPtr);

                CSL_udmapCppi5SetOrgBufferLen(pDescEntry, dmaPkt->sgList.list[i].segmentAllocLen);

                CSL_udmapCppi5SetBufferLen(pDescEntry, dmaPkt->sgList.list[i].segmentFilledLen);

                totalPacketFilledLen += dmaPkt->sgList.list[i].segmentFilledLen;
                pLastDescEntry = pDescEntry;
            }
            Enet_assert(pLastDescEntry != NULL);
            /* Link the last segment's Desc to NULL */
            CSL_udmapCppi5LinkDesc(pLastDescEntry, 0);

            CSL_udmapCppi5HostSetPktLen(pHostDesc, totalPacketFilledLen);

            if (ENET_UDMA_DIR_TX == transferDir)
            {
#if ENET_CFG_IS_ON(DEV_ERROR)
                /* For TX all defaults are set in EnetUdma_buffDescInit function. We just confirm those
                 * are not changed here */
                Enet_assert(CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST == CSL_udmapCppi5GetDescType(pHostDesc));
                Enet_assert(0U == CSL_udmapCppi5GetPsDataLoc(pHostDesc));
                Enet_assert(BTRUE == CSL_udmapCppi5IsEpiDataPresent(pHostDesc));
                Enet_assert(ENET_UDMA_PROTOCOL_SPECIFIC_INFO_BLOCK_SIZE ==
                                CSL_udmapCppi5GetPsDataLen(pHostDesc));
#endif
                cppiRxCntr = (EnetUdma_CppiRxControl *)pHpdDesc->psInfo;
                cppiRxCntr->chkSumInfo = dmaPkt->chkSumInfo;

                tsInfo = (uint32_t *)&cppiRxCntr->tsInfo;

                if (dmaPkt->tsInfo.enableHostTxTs == BTRUE)
                {
                    ENETUDMA_CPPIPSI_SET_TSEN(*tsInfo, 1U);

                    /* Below fields are only valid for CPSW. In ICSSG, whole tsInfo word is
                     * used for enabling timestamp.
                     * This is don't care/reserved word for ICSSG so we set without any check */
                    ENETUDMA_CPPIPSI_SET_DOMAIN(*tsInfo,
                                               dmaPkt->tsInfo.txPktDomain);
                    ENETUDMA_CPPIPSI_SET_MSGTYPE(*tsInfo,
                                                dmaPkt->tsInfo.txPktMsgType);
                    ENETUDMA_CPPIPSI_SET_SEQID(*tsInfo,
                                              dmaPkt->tsInfo.txPktSeqId);

                    /* Set host Tx timestamp flag back to false. */
                    dmaPkt->tsInfo.enableHostTxTs = BFALSE;
                }
                else
                {
                    ENETUDMA_CPPIPSI_SET_TSEN(*tsInfo, 0U);
                }

                if (dmaPkt->txPortNum != ENET_MAC_PORT_INV)
                {
                    dstTag = CPSW_ALE_MACPORT_TO_ALEPORT(dmaPkt->txPortNum);
                    /* Set txPortNum back to invalid to reset directed packet configuration */
                    dmaPkt->txPortNum = ENET_MAC_PORT_INV;
                }
                else
                {
                    dstTag = 0U;
                }

                if (dmaPkt->txPktTc != ENET_TRAFFIC_CLASS_INV)
                {
                    dstTag |= dmaPkt->txPktTc << 8U;
                }

                CSL_udmapCppi5SetDstTag(pHostDesc, dstTag);
            }

            /* Check that the buffer pointer and length are correct */
            retVal = EnetUdma_dmaDescCheck(pDmaDesc, transferDir);

            if (UDMA_SOK == retVal)
            {
                retVal = EnetUdma_ringEnqueue(hUdmaRing,
                                             pDmaDesc,
                                             disableCacheOpsFlag,
                                             transferDir,
                                             hUdmaProxy);
            }

            /* Dequeue from toHwQ only if the packet was actually queued to the fqRing */
            if (UDMA_SOK == retVal)
            {
                dmaPkt = (EnetDma_Pkt *)EnetQueue_deq(pToHwQueue);
                submitCnt++;
            }
            else
            {
                // TODO - based on error we should add back dmadesc and pktInfo to orig Queues
                EnetDma_checkPktState(&pDmaDesc->dmaPkt->pktState,
                                        ENET_PKTSTATE_MODULE_DRIVER,
                                        (uint32_t)ENET_PKTSTATE_DMA_WITH_HW,
                                        (uint32_t)ENET_PKTSTATE_DMA_NOT_WITH_HW);
                break;
            }
        }
        else
        {
            /* Return dequeued packet buffer as couldn't get free dma desc to attach packet to */
            EnetQueue_enqHead(pToHwQueue, &dmaPkt->node);
            retVal = UDMA_EALLOC;
            break;
        }
    }

    /* Wb the Ring memory cache before commiting to the ring in case of exposed ring mode */
    if ((UDMA_SOK == retVal) && (isExposedRing == BTRUE))
    {
        if ((ringWrIdx + submitCnt) > ringMemEleCnt)
        {
            currRingMemPtr = ringMemPtr + ringWrIdx;
            EnetOsal_cacheWb(currRingMemPtr,
                             (ringMemEleCnt - ringWrIdx) * ENET_UDMA_RING_MEM_SIZE);
            EnetOsal_cacheWb(ringMemPtr,
                             (submitCnt - ringMemEleCnt + ringWrIdx) * ENET_UDMA_RING_MEM_SIZE);
        }
        else
        {
            EnetOsal_cacheWb(currRingMemPtr,
                             submitCnt * ENET_UDMA_RING_MEM_SIZE);
        }

        /* Set Ring door bell register with count of number of descriptors queued */
        Udma_ringSetDoorBell(hUdmaRing, submitCnt);
    }

    /* If fqRing ran out of space is not an error, packets will be re-submitted from toHwQ */
    if (UDMA_ETIMEOUT == retVal)
    {
        retVal = UDMA_SOK;
    }

    return retVal;
}

int32_t EnetUdma_flushRxFlowRing(EnetDma_RxChHandle hRxFlow,
                                Udma_RingHandle hUdmaRing,
                                EnetDma_PktQ *pPktInfoQ)
{
    int32_t retVal = UDMA_SOK;
    uint64_t pDesc = 0;
    EnetUdma_DmaDesc *pDmaDesc;

    EnetQueue_initQ(pPktInfoQ);

    /* Dequeue the packets still sitting in the cqRing */
    retVal = Udma_ringDequeueRaw(hUdmaRing, &pDesc);
    while ((UDMA_SOK == retVal) && (0ULL != pDesc))
    {
        pDmaDesc = (EnetUdma_DmaDesc *)EnetUtils_physToVirt(pDesc, NULL);
        EnetDma_checkPktState(&pDmaDesc->dmaPkt->pktState,
                                ENET_PKTSTATE_MODULE_DRIVER,
                                (uint32_t)ENET_PKTSTATE_DMA_WITH_HW,
                                (uint32_t)ENET_PKTSTATE_DMA_NOT_WITH_HW);
        EnetQueue_enq(pPktInfoQ, &pDmaDesc->dmaPkt->node);

        EnetMem_freeDmaDesc(hRxFlow->rxFlowPrms.cbArg,
                            pDmaDesc);

        retVal = Udma_ringDequeueRaw(hUdmaRing, &pDesc);
    }

    /* It's ok if there are no more descs in cqRing */
    if (UDMA_ETIMEOUT == retVal)
    {
        retVal = UDMA_SOK;
    }

    return retVal;
}

int32_t EnetUdma_flushTxChRing(EnetDma_TxChHandle hTxCh,
                              Udma_RingHandle hUdmaRing,
                              EnetDma_PktQ *pFqPktInfoQ)
{
    int32_t retVal = UDMA_SOK;
    uint64_t pDesc = 0;
    EnetUdma_DmaDesc *pDmaDesc;

    EnetQueue_initQ(pFqPktInfoQ);

    /* Dequeue the packets still sitting in the ring */
    retVal = Udma_ringDequeueRaw(hUdmaRing, &pDesc);
    while ((UDMA_SOK == retVal) && (0ULL != pDesc))
    {
        pDmaDesc = (EnetUdma_DmaDesc *)EnetUtils_physToVirt(pDesc, NULL);
        EnetDma_checkPktState(&pDmaDesc->dmaPkt->pktState,
                                ENET_PKTSTATE_MODULE_DRIVER,
                                (uint32_t)ENET_PKTSTATE_DMA_WITH_HW,
                                (uint32_t)ENET_PKTSTATE_DMA_NOT_WITH_HW);
        EnetQueue_enq(pFqPktInfoQ, &pDmaDesc->dmaPkt->node);

        EnetMem_freeDmaDesc(hTxCh->txChPrms.cbArg,
                                       pDmaDesc);

        retVal = Udma_ringDequeueRaw(hUdmaRing, &pDesc);
    }

    /* It's ok if there are no more descs in ring */
    if (UDMA_ETIMEOUT == retVal)
    {
        retVal = UDMA_SOK;
    }

    return retVal;
}

int32_t EnetUdma_freeRing(Udma_RingHandle hUdmaRing,
                        uint32_t numPkts,
                        const void *cbArg)
{
    uint8_t *ringMemPtr;
    int32_t retVal = UDMA_SOK;

    ringMemPtr = (uint8_t *)Udma_ringGetMemPtr(hUdmaRing);
    retVal     = Udma_ringFree(hUdmaRing);

    if (NULL != cbArg)
    {
        EnetMem_freeRingMem(cbArg, ringMemPtr, numPkts);
    }

    return retVal;
}

int32_t EnetUdma_allocRing(Udma_DrvHandle hUdmaDrv,
                          Udma_RingHandle hUdmaRing,
                          Udma_RingPrms *pRingPrms,
                          EnetUdma_ringAllocInfo *pRingAllocInfo)
{
    int32_t retVal = UDMA_SOK;
    bool freeMem   = BFALSE;

    if ((pRingAllocInfo != NULL ) && (pRingAllocInfo->allocRingMem))
    {
        Enet_assert(NULL != pRingAllocInfo);

        pRingPrms->ringMem = EnetMem_allocRingMem(pRingAllocInfo->cbArg,
                                             pRingPrms->elemCnt,
                                             ENETDMA_CACHELINE_ALIGNMENT);
        if (pRingPrms->ringMem == NULL)
        {
            retVal = UDMA_EALLOC;
            ENETTRACE_ERR(retVal, "RX ring memory allocation failed");
        }
        else if (!ENET_UTILS_IS_ALIGNED(pRingPrms->ringMem, ENETDMA_CACHELINE_ALIGNMENT))
        {
            retVal  = UDMA_EALLOC;
            ENETTRACE_ERR(retVal, "TX fq ring memory not aligned");
            freeMem = BTRUE;
        }
        else
        {
        }
    }
    else
    {
        /* ring memory should not be NULL if allocRingMem flag is false */
        Enet_assert (NULL != pRingPrms->ringMem);
    }


    if ((UDMA_SOK == retVal) && (pRingAllocInfo != NULL))
    {
        /* Clear and invalidate ring memory */
        memset(pRingPrms->ringMem, 0, (pRingPrms->elemCnt * ENET_UDMA_RING_MEM_SIZE));
        if (BFALSE == Enet_isCacheCoherent())
        {
            EnetOsal_cacheWbInv(pRingPrms->ringMem, (pRingPrms->elemCnt * ENET_UDMA_RING_MEM_SIZE));
        }

        /* need to allocate rxfree/rxcompletion ring pair */
        retVal = Udma_ringAlloc(hUdmaDrv,
                                hUdmaRing,
                                pRingAllocInfo->ringNum,
                                pRingPrms);
        if (UDMA_SOK != retVal)
        {
            freeMem = BTRUE;
        }
    }

    if ((pRingAllocInfo != NULL) && (pRingAllocInfo->allocRingMem) && (freeMem))
    {
        EnetMem_freeRingMem(pRingAllocInfo->cbArg, pRingPrms->ringMem, pRingPrms->elemCnt);
        pRingPrms->ringMem = NULL;
    }

    return retVal;
}

void EnetUdma_getTxChUdmaInfo(EnetUdma_TxChObj *pTxCh,
                              EnetUdma_udmaInfo *pUdmaInfo)
{
    pUdmaInfo->hUdmaDrv     = pTxCh->hUdmaDrv;
    pUdmaInfo->hUdmaCh      = pTxCh->hUdmaCh;
    pUdmaInfo->hUdmaEvt     = pTxCh->hUdmaEvt;
    pUdmaInfo->hUdmaRingMon = pTxCh->hUdmaRingMon;
    pUdmaInfo->useGlobalEvt = pTxCh->useGlobalEvt;
}

void EnetUdma_getRxFlowUdmaInfo(EnetUdma_RxFlowObj *pRxFlow,
                                EnetUdma_udmaInfo *pUdmaInfo)
{
    pUdmaInfo->hUdmaDrv     = pRxFlow->hUdmaDrv;
    pUdmaInfo->hUdmaEvt     = pRxFlow->hUdmaEvt;
    pUdmaInfo->hUdmaRingMon = pRxFlow->hUdmaRingMon;
    pUdmaInfo->useGlobalEvt = pRxFlow->useGlobalEvt;
}

static int32_t EnetUdma_ringProxyEnqueue(Udma_RingHandle hUdmaRing,
                                        Udma_ProxyHandle hUdmaProxy,
                                        uint64_t phyDescMem)
{
    int32_t retVal = UDMA_SOK;
    uint32_t ringHwOcc;
    uintptr_t key;

#if ENET_CFG_IS_ON(DEV_ERROR)
    if ((NULL == hUdmaProxy) ||
        (NULL == hUdmaRing))
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR_IF((NULL == hUdmaProxy), retVal, "hUdmaProxy is NULL");
        ENETTRACE_ERR_IF((NULL == hUdmaRing), retVal, "hUdmaRing is NULL");
    }
    else
#endif
    {
        key = EnetOsal_disableAllIntr();

        /* Get ring occupancy. We should write to FIFO (through proxy) only
         * when there is room available. Otherwise ring will overflow */
        ringHwOcc = CSL_ringaccGetRingHwOcc(&hUdmaRing->drvHandle->raRegs,
                                            hUdmaRing->ringNum);
        if (ringHwOcc >= hUdmaRing->cfg.elCnt)
        {
            /* Ring full */
            retVal = UDMA_EFAIL;
        }
        else
        {
            Udma_proxyQueue(hUdmaProxy, phyDescMem);
            retVal = UDMA_SOK;
        }

        EnetOsal_restoreAllIntr(key);
    }

    return(retVal);
}

EnetDma_RxChHandle EnetUdma_openRxRsvdFlow(EnetUdma_RsvdRxFlowPrms *pRxFlowPrms)
{
    int32_t retVal = UDMA_SOK;
    Udma_FlowPrms flowPrms;
    Udma_RingPrms ringPrms;
    EnetUdma_RxFlowObj *pRxFlow;
    EnetUdma_ringAllocInfo ringAllocInfo;
    bool allocFlowObj = BFALSE, allocFqRing = BFALSE;
    bool allocCqRing  = BFALSE, flowAttachFlag = BFALSE;
    uintptr_t intrKey;
    uint32_t flowStart;

    intrKey = EnetOsal_disableAllIntr();

    /* Set to NULL so if error condition we return NULL */
    pRxFlow = NULL;

    /* Error check */
    retVal = UDMA_EBADARGS;
    if (NULL != pRxFlowPrms)
    {
        if (NULL != pRxFlowPrms->hUdmaDrv)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            ENETTRACE_ERR(retVal, "pRxFlowPrms has null UDMA handle");
        }
    }
    else
    {
        ENETTRACE_ERR(retVal, "pRxFlowPrms NULL");
    }

    if (UDMA_SOK == retVal)
    {
        pRxFlow = EnetUdma_memMgrAllocRxFlowObj();

        if (pRxFlow == NULL)
        {
            retVal = UDMA_EALLOC;
            ENETTRACE_ERR(retVal, "Rx flow Memory alloc object failed");
        }
        else
        {
            allocFlowObj = BTRUE;
            memset(pRxFlow, 0U, sizeof(*pRxFlow));
            /* Save Flow config */
            pRxFlow->rxFlowPrms.startIdx = pRxFlowPrms->startIdx;
            pRxFlow->rxFlowPrms.flowIdx  = pRxFlowPrms->flowIdx;
            pRxFlow->rxFlowPrms.hUdmaDrv = pRxFlowPrms->hUdmaDrv;

            /* Initialize local variables to be used in reminder of this function  */
            pRxFlow->hUdmaDrv  = pRxFlowPrms->hUdmaDrv;
            pRxFlow->hUdmaFlow = &pRxFlow->rxFlowMemObj.flowUdmaObj;
            pRxFlow->fqRing    = &pRxFlow->rxFlowMemObj.fqRingObj;
            pRxFlow->cqRing    = &pRxFlow->rxFlowMemObj.cqRingObj;

        }
    }

    if (UDMA_SOK == retVal)
    {
        UdmaRingPrms_init(&ringPrms);
        ringPrms.elemCnt = ENET_ARRAYSIZE(gRsvdFlowRingMemFQ);
        ringPrms.mode    = CSL_RINGACC_RING_MODE_MESSAGE;
        ringPrms.ringMem = &gRsvdFlowRingMemFQ[0U][0U];

        ringAllocInfo.allocRingMem = BFALSE;
        ringAllocInfo.ringNum      = UDMA_RING_ANY;

        retVal = EnetUdma_allocRing(pRxFlow->hUdmaDrv,
                                   pRxFlow->fqRing,
                                   &ringPrms,
                                   &ringAllocInfo);
        ENETTRACE_ERR_IF((retVal != UDMA_SOK), retVal, "Fq ring allocation failed");
        if (UDMA_SOK == retVal)
        {
            allocFqRing = BTRUE;
        }
    }

    if (UDMA_SOK == retVal)
    {
        UdmaRingPrms_init(&ringPrms);
        ringPrms.elemCnt = ENET_ARRAYSIZE(gRsvdFlowRingMemCQ);
        ringPrms.mode    = CSL_RINGACC_RING_MODE_MESSAGE;
        ringPrms.ringMem = &gRsvdFlowRingMemCQ[0U][0U];

        ringAllocInfo.allocRingMem = BFALSE;
        ringAllocInfo.ringNum      = UDMA_RING_ANY;

        retVal = EnetUdma_allocRing(pRxFlow->hUdmaDrv,
                                   pRxFlow->cqRing,
                                   &ringPrms,
                                   &ringAllocInfo);
        ENETTRACE_ERR_IF((retVal != UDMA_SOK), retVal, "Cq ring allocation failed");
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
        flowPrms.psInfoPresent = TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_PSINFO_PRESENT;

        flowPrms.defaultRxCQ = pRxFlow->cqRing->ringNum;

        flowPrms.fdq0Sz0Qnum = pRxFlow->fqRing->ringNum;
        flowPrms.fdq0Sz1Qnum = pRxFlow->fqRing->ringNum;
        flowPrms.fdq0Sz2Qnum = pRxFlow->fqRing->ringNum;
        flowPrms.fdq0Sz3Qnum = pRxFlow->fqRing->ringNum;
        flowPrms.fdq1Qnum    = pRxFlow->fqRing->ringNum;
        flowPrms.fdq2Qnum    = pRxFlow->fqRing->ringNum;
        flowPrms.fdq3Qnum    = pRxFlow->fqRing->ringNum;

        flowStart = pRxFlowPrms->startIdx + pRxFlowPrms->flowIdx;

        /* Attach and configure the flows */
        retVal = Udma_flowAttach(pRxFlow->hUdmaDrv,
                                 pRxFlow->hUdmaFlow,
                                 flowStart, /* flowStart */
                                 1U /* flowCnt */);

        ENETTRACE_ERR_IF((retVal != UDMA_SOK), retVal, "Flow attach failed");
        if (UDMA_SOK == retVal)
        {
            flowAttachFlag = BTRUE;
            retVal         = Udma_flowConfig(pRxFlow->hUdmaFlow,
                                             0U, /* Flow Index */
                                             &flowPrms);
        }
    }

    /* Save flow config */
    if (UDMA_SOK == retVal)
    {
        pRxFlow->initFlag    = BTRUE;
        pRxFlow->evtInitFlag = BFALSE;
    }
    else
    {
        /* Error. Free-up resources if allocated */

        if (allocFqRing)
        {
            retVal = EnetUdma_freeRing(pRxFlow->fqRing, 0U, NULL);
            Enet_assert(UDMA_SOK == retVal);
        }

        if (allocCqRing)
        {
            retVal = EnetUdma_freeRing(pRxFlow->cqRing, 0U, NULL);
            Enet_assert(UDMA_SOK == retVal);
        }

        if (flowAttachFlag)
        {
            retVal = Udma_flowDetach(pRxFlow->hUdmaFlow);
            Enet_assert(UDMA_SOK == retVal);
        }

        /* Free the rxFlowObj last */
        if (allocFlowObj)
        {
            EnetUdma_memMgrFreeRxFlowObj(pRxFlow);
        }

        /* As flow open is failed return NULL */
        pRxFlow = NULL;
    }

    EnetOsal_restoreAllIntr(intrKey);

    return pRxFlow;
}

int32_t EnetUdma_closeRxRsvdFlow(EnetDma_RxChHandle hRxFlow)
{
    int32_t retVal = UDMA_SOK;
    uintptr_t intrKey;

#if ENET_CFG_IS_ON(DEV_ERROR)
    if (NULL == hRxFlow)
    {
        retVal = UDMA_EBADARGS;
        ENETTRACE_ERR(retVal, "Input params can't be NULL");
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
            Enet_assert(hRxFlow->evtInitFlag == BFALSE);

            retVal = EnetUdma_freeRing(hRxFlow->fqRing, 0U, NULL);
            Enet_assert(UDMA_SOK == retVal);

            retVal = EnetUdma_freeRing(hRxFlow->cqRing, 0U, NULL);
            Enet_assert(UDMA_SOK == retVal);

            retVal = Udma_flowDetach(hRxFlow->hUdmaFlow);
            Enet_assert(UDMA_SOK == retVal);
            hRxFlow->initFlag = BFALSE;

            /* Free Rx Flow driver object memory */
            EnetUdma_memMgrFreeRxFlowObj(hRxFlow);
        }

        EnetOsal_restoreAllIntr(intrKey);
    }

    return retVal;
}

int32_t EnetUdma_ringEnqueue(Udma_RingHandle hUdmaRing,
                            EnetUdma_DmaDesc *pDmaDesc,
                            bool disableCacheOpsFlag,
                            EnetUdma_Dir transferDir,
                            Udma_ProxyHandle hUdmaProxy)
{
    int32_t retVal = UDMA_SOK;
    bool isExposedRing;
    void *virtBufPtr;
    uint64_t physDescPtr;
    CSL_UdmapCppi5HMPD *pDescEntry;
    uint32_t i;

    if ((pDmaDesc != NULL) && (hUdmaRing != NULL))
    {
        EnetDma_Pkt *dmaPkt = pDmaDesc->dmaPkt;
        uint32_t numScatterSegments = dmaPkt->sgList.numScatterSegments;

        EnetUdma_CpswHpdDesc *pHpdDesc = (EnetUdma_CpswHpdDesc *)pDmaDesc;

        isExposedRing = (Udma_ringGetMode(hUdmaRing) == CSL_RINGACC_RING_MODE_RING);

        pDescEntry = &pHpdDesc->hostDesc;
        physDescPtr = (uint64_t)EnetUtils_virtToPhys((void *)pDescEntry, NULL);

        for (i = 0U; i < numScatterSegments; i++)
        {
            if (i > ENET_UDMA_CPSW_HOSTPKTDESC_INDEX)
            {
                pDescEntry =  &pDmaDesc->hostBufDesc[i - ENET_UDMA_CPSW_HOSTBUFDESC_INDEX].desc;
            }

            virtBufPtr = (void *)(uintptr_t)pDescEntry->bufPtr;

            pDescEntry->bufPtr =
                EnetUtils_virtToPhys((void *)(uintptr_t)pDescEntry->bufPtr, NULL);

            pDescEntry->orgBufPtr =
                EnetUtils_virtToPhys((void *)(uintptr_t)pDescEntry->orgBufPtr, NULL);

            /* Wb Invalidate data cache */
            if ((BFALSE == disableCacheOpsFlag) &&
                (BFALSE  == Enet_isCacheCoherent()))
            {
                if (ENET_UDMA_DIR_RX == transferDir)
                {
                    EnetOsal_cacheInv(virtBufPtr, CSL_udmapCppi5GetOrgBufferLen(pDescEntry));
                }
                else
                {
                    if (dmaPkt->sgList.list[i].disableCacheOps != BTRUE)
                    {
                        EnetOsal_cacheWbInv(virtBufPtr, CSL_udmapCppi5GetBufferLen(pDescEntry));
                    }
                }
            }

            if (pDescEntry->nextDescPtr != 0ULL)
            {
                pDescEntry->nextDescPtr = EnetUtils_virtToPhys((void *)(uintptr_t)pDescEntry->nextDescPtr, NULL);
            }
        }
        if (BFALSE == Enet_isCacheCoherent())
        {
            EnetOsal_cacheWbInv((void *)pDmaDesc, sizeof(*pDmaDesc));
        }
        if (isExposedRing == BTRUE)
        {
           /* Udma_ringPrime doesn't check for invalid parameters and also doesn't return
            * any errors */
           Udma_ringPrime(hUdmaRing, physDescPtr);
        }
        else
        {
            if (hUdmaProxy == NULL)
            {
                retVal = Udma_ringQueueRaw(hUdmaRing, physDescPtr);
            }
            else
            {
                retVal = EnetUdma_ringProxyEnqueue(hUdmaRing, hUdmaProxy, physDescPtr);
            }
        }
    }
    else
    {
        retVal = UDMA_EFAIL;
    }

    return retVal;
}

int32_t EnetUdma_ringDequeue(Udma_RingHandle hUdmaRing,
                            EnetUdma_DmaDesc **pDmaDesc,
                            bool disableCacheOpsFlag,
                            EnetUdma_Dir transferDir)
{
    int32_t retVal = UDMA_SOK;
    bool isExposedRing;
    uint64_t pDesc = 0;
    EnetUdma_DmaDesc *pVirtDmaDesc;
    CSL_UdmapCppi5HMPD *pDescEntry;
    CSL_UdmapCppi5HMPD *pNextDescEntryVirt;

    if ((pDmaDesc != NULL) && (hUdmaRing != NULL))
    {
        isExposedRing = (Udma_ringGetMode(hUdmaRing) == CSL_RINGACC_RING_MODE_RING);
        if (isExposedRing == BTRUE)
        {
            /* Udma_ringPrimeRead doesn't check for invalid parameters and also doesn't return
             * any errors */
            Udma_ringPrimeRead(hUdmaRing, &pDesc);
            if (pDesc == 0U)
            {
                *pDmaDesc = NULL;
                retVal = UDMA_ETIMEOUT;
            }
        }
        else
        {
            retVal = Udma_ringDequeueRaw(hUdmaRing, &pDesc);
            if (UDMA_SOK != retVal)
            {
                *pDmaDesc = NULL;
            }
        }

        if (UDMA_SOK == retVal)
        {
            pVirtDmaDesc = (EnetUdma_DmaDesc *)EnetUtils_physToVirt(pDesc, NULL);
            *pDmaDesc = pVirtDmaDesc;
            pDescEntry = &pVirtDmaDesc->hpdDesc.hostDesc;
            if (BFALSE == Enet_isCacheCoherent())
            {
#if ENET_CFG_IS_ON(DEV_ERROR)
                Enet_assert(ENET_UTILS_IS_ALIGNED(sizeof(*pVirtDmaDesc), ENETDMA_CACHELINE_ALIGNMENT));
#endif
                EnetOsal_cacheInv((void *)pVirtDmaDesc, sizeof(*pVirtDmaDesc));
            }

            while(pDescEntry != NULL)
            {
                pDescEntry->bufPtr =
                    (uint64_t)EnetUtils_physToVirt(pDescEntry->bufPtr, NULL);
                pDescEntry->orgBufPtr =
                    (uint64_t)EnetUtils_physToVirt(pDescEntry->orgBufPtr, NULL);
                if (pDescEntry->nextDescPtr != 0ULL)
                {
                    pNextDescEntryVirt = (CSL_UdmapCppi5HMPD *)EnetUtils_physToVirt(pDescEntry->nextDescPtr, NULL);
                    pDescEntry->nextDescPtr = (uintptr_t)pNextDescEntryVirt;
                    pDescEntry = pNextDescEntryVirt;
                }
                else
                {
                    pDescEntry = (CSL_UdmapCppi5HMPD *)pDescEntry->nextDescPtr;
                }
            }
            /* Debug point -> The Cortex A cores can do speculative reads which
             * can mandate to do cache_invalidate before packets are received.
             * If Cortex-A doesn't support coherency, invalidate the data buffer
             * so the CPU is forced to read new data from HW.
             * We only need to so this on the Cortex v7A devices because of possible
             * speculative reads done by the CPU core while the buffers are owned by
             * the DMA hardware
             */
        }
    }
    else
    {
        retVal = UDMA_EFAIL;
    }
    return retVal;
}

void EnetUdma_reconfigureDescQ(EnetUdma_TxChObj *pTxCh)
{
    EnetUdma_DmaDescQ tempQ;
    EnetUdma_DmaDesc *dmaDesc;
    EnetUdma_CpswHpdDesc *pHpdDesc;

    Enet_assert(pTxCh != NULL);
    Enet_assert(pTxCh->hDmaDescPool != NULL);

    EnetUdma_DmaDescQ *descQPtr = pTxCh->hDmaDescPool;
    uint32_t retQIdx = pTxCh->cqRing->ringNum;

    EnetUdma_dmaDescQInit(&tempQ);

    while (EnetUdma_dmaDescQCount(descQPtr) > 0U)
    {
        dmaDesc = EnetUdma_dmaDescDeque(descQPtr);
        Enet_assert(dmaDesc != NULL);

        pHpdDesc = (EnetUdma_CpswHpdDesc *)dmaDesc;
        CSL_udmapCppi5SetReturnPolicy(&pHpdDesc->hostDesc,
                                      ENET_UDMA_UDMAP_DESC_TYPE,
                                      CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                      CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                      CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                      retQIdx);
        EnetUdma_dmaDescEnque(&tempQ, dmaDesc);
    }

    EnetUdma_dmaDescCopyQ(descQPtr, &tempQ);

    return;
}

void EnetUdma_initTxFreeDescQ(EnetUdma_TxChObj *pTxCh)
{
    uint32_t descType;
    uint32_t alignSize = ENETDMA_CACHELINE_ALIGNMENT;
    uint32_t i;
    int32_t retVal = ENET_SOK;

    ENETTRACE_VAR(retVal);

    Enet_assert(pTxCh != NULL);
    Enet_assert(pTxCh->hDmaDescPool != NULL);

    EnetUdma_dmaDescQInit(pTxCh->hDmaDescPool);

    for (i = 0U; i < pTxCh->txChPrms.numTxPkts; i++)
    {
        EnetUdma_DmaDesc *dmaDesc =
            EnetMem_allocDmaDesc(pTxCh->txChPrms.cbArg, alignSize);
        if (dmaDesc == NULL)
        {
            retVal = ENET_EALLOC;
            ENETTRACE_ERR(retVal, "Tx DMA descriptor memory allocation failed");
            Enet_assert(BFALSE);
        }
        else if (!ENET_UTILS_IS_ALIGNED(dmaDesc, alignSize))
        {
            retVal = ENET_EALLOC;
            ENETTRACE_ERR(retVal, "dmaDesc memory not aligned");
            Enet_assert(BFALSE);
        }
        else
        {
            EnetUdma_CpswHpdDesc *pHpdDesc = (EnetUdma_CpswHpdDesc *)dmaDesc;
            CSL_UdmapCppi5HMPD *hostDesc  = &pHpdDesc->hostDesc;

            descType = ENET_UDMA_UDMAP_DESC_TYPE;

            EnetUdma_dmaDescInit(dmaDesc);

            CSL_udmapCppi5SetDescType(hostDesc, descType);

            CSL_udmapCppi5SetReturnPolicy(hostDesc,
                                          descType,
                                          CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                          CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                          CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                          pTxCh->cqRing->ringNum);
            EnetUdma_dmaDescEnque(pTxCh->hDmaDescPool, dmaDesc);
        }
    }
}

void EnetUdma_deInitTxFreeDescQ(EnetUdma_TxChObj *pTxCh)
{
    EnetUdma_DmaDesc *pDmaDesc;

    pDmaDesc = EnetUdma_dmaDescDeque(pTxCh->hDmaDescPool);
    while (NULL != pDmaDesc)
    {
        EnetMem_freeDmaDesc(pTxCh->txChPrms.cbArg, pDmaDesc);
        pDmaDesc = EnetUdma_dmaDescDeque(pTxCh->hDmaDescPool);
    }
}

void EnetUdma_initRxFreeDescQ(EnetUdma_RxFlowObj *pRxFlow)
{
    uint32_t descType;
    uint32_t alignSize = ENETDMA_CACHELINE_ALIGNMENT;
    uint32_t i;
    int32_t retVal = ENET_SOK;

    ENETTRACE_VAR(retVal);

    EnetUdma_dmaDescQInit(pRxFlow->hDmaDescPool);

    for (i = 0U; i < pRxFlow->rxFlowPrms.numRxPkts; i++)
    {
        EnetUdma_DmaDesc *dmaDesc =
            EnetMem_allocDmaDesc(pRxFlow->rxFlowPrms.cbArg, alignSize);
        if (dmaDesc == NULL)
        {
            retVal = ENET_EALLOC;
            ENETTRACE_ERR(retVal, "Tx DMA descriptor memory allocation failed");
            Enet_assert(dmaDesc != NULL);
        }
        else if (!ENET_UTILS_IS_ALIGNED(dmaDesc, alignSize))
        {
            retVal = ENET_EALLOC;
            ENETTRACE_ERR(retVal, "dmaDesc memory not aligned");
            Enet_assert(BFALSE);
        }
        else
        {
            EnetUdma_CpswHpdDesc *pHpdDesc = (EnetUdma_CpswHpdDesc *)dmaDesc;
            CSL_UdmapCppi5HMPD *hostDesc  = &pHpdDesc->hostDesc;

            EnetUdma_dmaDescInit(dmaDesc);

            descType = ENET_UDMA_UDMAP_DESC_TYPE;

            CSL_udmapCppi5SetDescType(hostDesc, descType);

            CSL_udmapCppi5SetReturnPolicy(hostDesc,
                                          descType,
                                          CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                          CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                          CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                          pRxFlow->cqRing->ringNum);
            EnetUdma_dmaDescEnque(pRxFlow->hDmaDescPool, dmaDesc);
        }
    }
}

void EnetUdma_deInitRxFreeDescQ(EnetUdma_RxFlowObj *pRxFlow)
{
    EnetUdma_DmaDesc *pDmaDesc;

    pDmaDesc = EnetUdma_dmaDescDeque(pRxFlow->hDmaDescPool);

    while (NULL != pDmaDesc)
    {
        EnetMem_freeDmaDesc(pRxFlow->rxFlowPrms.cbArg, pDmaDesc);
        pDmaDesc = EnetUdma_dmaDescDeque(pRxFlow->hDmaDescPool);
    }
}

/*!
 * \defgroup EnetUdma_HELPERS_BUFF_DESC_QUEUE EnetUdma Buffer Descriptor Queue Helpers
 * @{
 */

/*! Zero init the buffer descriptor queue */
void EnetUdma_buffDescInit(EnetUdma_CpswHpdDesc *hpdDesc)
{
    CSL_UdmapCppi5HMPD *hostDesc = &hpdDesc->hostDesc;

    uint32_t descType = (uint32_t)CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST;

    CSL_udmapCppi5SetDescType(hostDesc, descType);
    CSL_udmapCppi5SetEpiDataPresent(hostDesc, BTRUE);
    CSL_udmapCppi5SetPsDataLoc(hostDesc, TISCI_MSG_VALUE_RM_UDMAP_RX_FLOW_PS_END_PD);
    CSL_udmapCppi5SetPsDataLen(hostDesc, ENET_UDMA_PROTOCOL_SPECIFIC_INFO_BLOCK_SIZE);
    CSL_udmapCppi5HostSetPktLen(hostDesc, 0U);
    CSL_udmapCppi5SetPsFlags(hostDesc, 0U);
    CSL_udmapCppi5SetIds(hostDesc, descType, 0U, 0U);
    CSL_udmapCppi5SetSrcTag(hostDesc, 0x0000U);
    CSL_udmapCppi5SetDstTag(hostDesc, 0x0000U);
    CSL_udmapCppi5LinkDesc(hostDesc, 0U);
    CSL_udmapCppi5SetBufferAddr(hostDesc, (uint64_t)NULL);
    CSL_udmapCppi5SetBufferLen(hostDesc, 0U);
    CSL_udmapCppi5SetOrgBufferAddr(hostDesc, (uint64_t)NULL);
    CSL_udmapCppi5SetOrgBufferLen(hostDesc, 0U);
}

/*  Packet operations  */

/*!
 * \defgroup EnetUdma_HELPERS_PACKET EnetUdma Packet Helpers
 * @{
 */
/*! DMA descriptor Q init */
void EnetUdma_dmaDescQInit(EnetUdma_DmaDescQ *pDmaDescQ)
{
    pDmaDescQ->head = NULL;
    pDmaDescQ->tail = NULL;
    pDmaDescQ->count = 0U;
}

uint32_t EnetUdma_dmaDescQCount(EnetUdma_DmaDescQ *pDmaDescQ)
{
    return pDmaDescQ->count;
}

/*! Zero init the packet */
void EnetUdma_dmaDescInit(EnetUdma_DmaDesc *pDmaDesc)
{
    memset(pDmaDesc, 0, sizeof(*pDmaDesc));
    pDmaDesc->pNextDesc   = NULL;
    pDmaDesc->dmaPkt = NULL;

    EnetUdma_buffDescInit(&pDmaDesc->hpdDesc);
}

void EnetUdma_dmaDescEnque(EnetUdma_DmaDescQ *pDmaDescQ,
                          EnetUdma_DmaDesc *pDmaDesc)
{
    if (pDmaDesc != NULL)
    {
        uintptr_t key = EnetOsal_disableAllIntr();

        pDmaDesc->pNextDesc = NULL;

        if (NULL == pDmaDescQ->head)
        {
            /* Queue is empty, make head point to new packet */
            pDmaDescQ->head = pDmaDesc;
        }
        else
        {
            /* Queue is not empty, make previous tail point to new packet */
            pDmaDescQ->tail->pNextDesc = pDmaDesc;
        }

        /* Make tail of queue point to new */
        pDmaDescQ->count++;
        pDmaDescQ->tail = pDmaDesc;

        EnetOsal_restoreAllIntr(key);
    }
    else
    {
        Enet_assert(BFALSE);
    }
}

EnetUdma_DmaDesc *EnetUdma_dmaDescDeque(EnetUdma_DmaDescQ *pDmaDescQ)
{
    EnetUdma_DmaDesc *pDmaDesc = NULL;

    if (NULL != pDmaDescQ)
    {
        uintptr_t key = EnetOsal_disableAllIntr();
        pDmaDesc = pDmaDescQ->head;

        if (NULL != pDmaDesc)
        {
            pDmaDescQ->head = pDmaDesc->pNextDesc;
            if (NULL == pDmaDescQ->head)
            {
                pDmaDescQ->tail = NULL;
            }

            pDmaDescQ->count--;
            pDmaDesc->pNextDesc = NULL;
        }

        EnetOsal_restoreAllIntr(key);
    }
    else
    {
        pDmaDesc = NULL;
    }

    return(pDmaDesc);
}

void EnetUdma_dmaDescCopyQ(EnetUdma_DmaDescQ *pDstQueue,
                          const EnetUdma_DmaDescQ *pSrcQueue)
{
    uintptr_t key = EnetOsal_disableAllIntr();

    pDstQueue->head = pSrcQueue->head;
    pDstQueue->tail = pSrcQueue->tail;
    pDstQueue->count = pSrcQueue->count;

    EnetOsal_restoreAllIntr(key);
}

int32_t EnetUdma_registerEvent(EnetUdma_udmaInfo *pUdmaInfo,
                              Udma_RingHandle hUdmaRing,
                              Udma_EventCallback eventCb,
                              void *cbArg,
                              uint32_t eventType)
{
    Udma_EventPrms evtPrms;
    int32_t retVal = UDMA_SOK;

    /* Register for the UDMA completion event. The UDMA driver sets up
     * the Int Aggr and Int Router according to our preferred interrupt
     * number.  The interrupt registration itself doesn't happen here */
    UdmaEventPrms_init(&evtPrms);

    switch (eventType)
    {
        case UDMA_EVENT_TYPE_DMA_COMPLETION:
            evtPrms.chHandle = pUdmaInfo->hUdmaCh;
            break;

        case UDMA_EVENT_TYPE_RING:
            Enet_assert(NULL != hUdmaRing);
            evtPrms.ringHandle = hUdmaRing;
            break;

        case UDMA_EVENT_TYPE_RING_MON:
            Enet_assert(NULL != hUdmaRing);
            Enet_assert(NULL != pUdmaInfo->hUdmaRingMon);
            evtPrms.ringHandle = hUdmaRing;
            evtPrms.monHandle  = pUdmaInfo->hUdmaRingMon;
            break;

        default:
            retVal = ENET_EALLOC;
            ENETTRACE_ERR(retVal, "Event registration failed allocation failed");
            Enet_assert(BFALSE);
    }

    evtPrms.eventType         = eventType;
    evtPrms.eventMode         = UDMA_EVENT_MODE_SHARED;

    /* Global event can be used when number of events is too large
     * (i.e. large number of RX flows and/or TX channels) */
    if (pUdmaInfo->useGlobalEvt)
    {
        evtPrms.masterEventHandle = Udma_eventGetGlobalHandle(pUdmaInfo->hUdmaDrv);
    }
    else
    {
        evtPrms.masterEventHandle = NULL;
    }

    evtPrms.eventCb           = eventCb;
    evtPrms.appData           = cbArg;

    retVal = Udma_eventRegister(pUdmaInfo->hUdmaDrv, pUdmaInfo->hUdmaEvt, &evtPrms);
    ENETTRACE_ERR_IF((UDMA_SOK != retVal), retVal, "UDMA event registration failed");

    return retVal;
}

int32_t EnetUdma_unregisterEvent(Udma_EventHandle hUdmaEvt)
{
    int32_t retVal = UDMA_SOK;

    retVal = Udma_eventUnRegister(hUdmaEvt);

    return retVal;
}

int32_t EnetUdma_dmaDescCheck(EnetUdma_DmaDesc *dmaDesc,
                             EnetUdma_Dir transferDir)
{
    int32_t retVal = UDMA_SOK;

#if ENET_CFG_IS_ON(DEV_ERROR)
    EnetUdma_CpswHpdDesc *pHpdDesc = (EnetUdma_CpswHpdDesc *)dmaDesc;
    CSL_UdmapCppi5HMPD *hostDesc  = &pHpdDesc->hostDesc;

    /* Is the desc memory aligned? */
    if (0ULL != ((uint64_t)hostDesc % ENET_UDMA_HPD_SIZE))
    {
        retVal = UDMA_EBADARGS;
    }

    /* Is the desc of Host Packet type? */
    if (retVal == UDMA_SOK)
    {
        if ((uint32_t)CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST !=
            CSL_udmapCppi5GetDescType(hostDesc))
        {
            retVal = UDMA_EBADARGS;
        }
    }

    /* Is the buffer address not NULL? */
    if (retVal == UDMA_SOK)
    {
        if (0ULL == CSL_udmapCppi5GetBufferAddr(hostDesc))
        {
            retVal = UDMA_EBADARGS;
        }
    }

    /* Is the buffer length > 0? */
    if (retVal == UDMA_SOK)
    {
        if (0U == CSL_udmapCppi5GetBufferLen(hostDesc))
        {
            if (ENET_UDMA_DIR_TX == transferDir)
            {
                retVal = UDMA_EBADARGS;
            }
        }
    }
#endif
    return retVal;
}

/*! Rx flow object allocation function  */
EnetUdma_RxFlowObj *EnetUdma_memMgrAllocRxFlowObj(void)
{
    EnetUdma_RxFlowObj *pRxFlow = NULL;
    EnetUdma_RxFlowObjMem *pRxFlowMemObj;

    pRxFlowMemObj = (EnetUdma_RxFlowObjMem *)EnetQueue_deq(&gEnetUdmaMemMgrObj.rxFlowObjMemQ);
#if ENET_CFG_IS_ON(DEV_ERROR)
    Enet_assert(pRxFlowMemObj != NULL);
#endif
    if (pRxFlowMemObj != NULL)
    {
        pRxFlow = &pRxFlowMemObj->rxFlowObj;
    }

    return pRxFlow;
}

/*! Rx flow object free function  */
void EnetUdma_memMgrFreeRxFlowObj(EnetUdma_RxFlowObj *pRxFlow)
{
    EnetUdma_RxFlowObjMem *pRxFlowMemObj;

#if ENET_CFG_IS_ON(DEV_ERROR)
    Enet_assert(NULL != pRxFlow);
#endif

    pRxFlowMemObj = container_of(pRxFlow, EnetUdma_RxFlowObjMem, rxFlowObj);
    EnetQueue_enq(&gEnetUdmaMemMgrObj.rxFlowObjMemQ, &pRxFlowMemObj->node);
    pRxFlowMemObj = NULL;

    return;
}

/*! TX channel object allocation function  */
EnetUdma_TxChObj *EnetUdma_memMgrAllocTxChObj(void)
{
    EnetUdma_TxChObj *pTxChObj = NULL;
    EnetUdma_TxChObjMem *pTxChMemObj;

    pTxChMemObj = (EnetUdma_TxChObjMem *)EnetQueue_deq(&gEnetUdmaMemMgrObj.txChObjMemQ);
#if ENET_CFG_IS_ON(DEV_ERROR)
    Enet_assert(pTxChMemObj != NULL);
#endif
    if (pTxChMemObj != NULL)
    {
        pTxChObj = &pTxChMemObj->txChObj;
    }

    return pTxChObj;
}

/*! TX channel object free function  */
void EnetUdma_memMgrFreeTxChObj(EnetUdma_TxChObj *pTxChObj)
{
    EnetUdma_TxChObjMem *pTxChMemObj;

#if ENET_CFG_IS_ON(DEV_ERROR)
    Enet_assert(NULL != pTxChObj);
#endif

    pTxChMemObj = container_of(pTxChObj, EnetUdma_TxChObjMem, txChObj);
    EnetQueue_enq(&gEnetUdmaMemMgrObj.txChObjMemQ, &pTxChMemObj->node);
    pTxChObj = NULL;

    return;
}

/*! Ring Monitor object allocation function  */
Udma_RingMonHandle EnetUdma_memMgrAllocRingMonObj(void)
{
    struct Udma_RingMonObj *pRingMonObj = NULL;
    EnetUdma_RingMonObjMem *pRingMemObj;

    pRingMemObj = (EnetUdma_RingMonObjMem *)
                  EnetQueue_deq(&gEnetUdmaMemMgrObj.ringMonObjMemQ);
#if ENET_CFG_IS_ON(DEV_ERROR)
    Enet_assert(pRingMemObj != NULL);
#endif
    if (pRingMemObj != NULL)
    {
        pRingMonObj = &pRingMemObj->ringMonObj;
    }

    return pRingMonObj;
}

/*! Ring Monitor object free function  */
void EnetUdma_memMgrFreeRingMonObj(Udma_RingMonHandle hUdmaRingMon)
{
    EnetUdma_RingMonObjMem *pRingMemObj;

#if ENET_CFG_IS_ON(DEV_ERROR)
    Enet_assert(NULL != hUdmaRingMon);
#endif

    pRingMemObj = container_of(hUdmaRingMon, EnetUdma_RingMonObjMem, ringMonObj);
    EnetQueue_enq(&gEnetUdmaMemMgrObj.ringMonObjMemQ, &pRingMemObj->node);
    hUdmaRingMon = NULL;

    return;
}

/*! TD Cq Ring memory object allocation function  */
void *EnetUdma_memMgrAllocTdCqRingMemObj(void)
{
    void *pTdCqRingMem = NULL;
    EnetUdma_TdCqRingObjMem *pTdCqRingMemObj;

    pTdCqRingMemObj = (EnetUdma_TdCqRingObjMem *)
                      EnetQueue_deq(&gEnetUdmaMemMgrObj.tdCqRingObjMemQ);
#if ENET_CFG_IS_ON(DEV_ERROR)
    Enet_assert(NULL != pTdCqRingMemObj);
#endif
    if (pTdCqRingMemObj != NULL)
    {
        pTdCqRingMem = (void *)&pTdCqRingMemObj->tdCqRingMemObj[0U];
        Enet_assert(ENET_UTILS_IS_ALIGNED(pTdCqRingMem, ENETDMA_CACHELINE_ALIGNMENT));
        memset(pTdCqRingMem, 0U, sizeof(uint64_t));
    }

    return pTdCqRingMem;
}

/*! TD Cq Ring memory object free function  */
void EnetUdma_memMgrFreeTdCqRingMemObj(void *pTdCqRingMem)
{
    EnetUdma_TdCqRingObjMem *pTdCqRingMemObj;

#if ENET_CFG_IS_ON(DEV_ERROR)
    Enet_assert(NULL != pTdCqRingMem);
#endif

    pTdCqRingMemObj = container_of((const uint64_t (*)[ENET_UTILS_ALIGN(ENET_UDMA_TDCQ_RING_ELE_CNT,
                                                                        ENETDMA_CACHELINE_ALIGNMENT)]) pTdCqRingMem,
                                   EnetUdma_TdCqRingObjMem,
                                   tdCqRingMemObj);
    EnetQueue_enq(&gEnetUdmaMemMgrObj.tdCqRingObjMemQ, &pTdCqRingMemObj->node);
    pTdCqRingMem = NULL;
}

void EnetUdma_memMgrInit(void)
{
    uint32_t i;

    if (gEnetUdmaMemMgrObj.clientCnt == 0U)
    {
        memset(&gEnetUdmaMemMgrObj, 0U, sizeof(gEnetUdmaMemMgrObj));
        memset(&gEnetUdmaRxFlowObjMem, 0U, sizeof(gEnetUdmaRxFlowObjMem));
        memset(&gEnetUdmaTxChObjMem, 0U, sizeof(gEnetUdmaTxChObjMem));
        memset(&gEnetUdmaRinMonObjMem, 0U, sizeof(gEnetUdmaRinMonObjMem));
        memset(&gEnetUdmaTdCqRingObjMem, 0U, sizeof(gEnetUdmaTdCqRingObjMem));

        /*********************** Rx flow object memory  Q ************************/
        EnetQueue_initQ(&gEnetUdmaMemMgrObj.rxFlowObjMemQ);
        for (i = 0U; i < ENET_CFG_RX_FLOWS_NUM; i++)
        {
            EnetQueue_enq(&gEnetUdmaMemMgrObj.rxFlowObjMemQ, &gEnetUdmaRxFlowObjMem[i].node);
        }

        /*********************** Tx channel object memory  Q ************************/
        EnetQueue_initQ(&gEnetUdmaMemMgrObj.txChObjMemQ);
        for (i = 0U; i < ENET_CFG_TX_CHANNELS_NUM; i++)
        {
            EnetQueue_enq(&gEnetUdmaMemMgrObj.txChObjMemQ, &gEnetUdmaTxChObjMem[i].node);
        }

        /*********************** Ring monitor object memory  Q ************************/
        EnetQueue_initQ(&gEnetUdmaMemMgrObj.ringMonObjMemQ);
        for (i = 0U; i < ENET_CFG_RING_MON_NUM; i++)
        {
            EnetQueue_enq(&gEnetUdmaMemMgrObj.ringMonObjMemQ, &gEnetUdmaRinMonObjMem[i].node);
        }

        /*********************** Teardown CQ object memory  Q ************************/
        EnetQueue_initQ(&gEnetUdmaMemMgrObj.tdCqRingObjMemQ);
        for (i = 0U; i < (ENET_CFG_TX_CHANNELS_NUM + ENET_UDMA_RX_CH_NUM); i++)
        {
            EnetQueue_enq(&gEnetUdmaMemMgrObj.tdCqRingObjMemQ, &gEnetUdmaTdCqRingObjMem[i].node);
        }
    }

    gEnetUdmaMemMgrObj.clientCnt++;
}

void EnetUdma_memMgrDeInit(void)
{
    gEnetUdmaMemMgrObj.clientCnt--;

    if (gEnetUdmaMemMgrObj.clientCnt == 0U)
    {
        memset(&gEnetUdmaMemMgrObj, 0U, sizeof(gEnetUdmaMemMgrObj));
    }
}

void EnetUdma_txCqIsr(Udma_EventHandle hUdmaEvt,
                     uint32_t eventType,
                     void *appData)
{
    EnetUdma_TxChObj *pTxCh = (EnetUdma_TxChObj *)appData;

    if (pTxCh->txChPrms.notifyCb != NULL)
    {
        pTxCh->txChPrms.notifyCb(pTxCh->txChPrms.cbArg);
    }
}

void EnetUdma_rxCqIsr(Udma_EventHandle hUdmaEvt,
                     uint32_t eventType,
                     void *appData)
{
    EnetUdma_RxFlowObj *pRxFlow = (EnetUdma_RxFlowObj *)appData;

    if (pRxFlow->rxFlowPrms.notifyCb != NULL)
    {
        pRxFlow->rxFlowPrms.notifyCb(pRxFlow->rxFlowPrms.cbArg);
    }
}

int32_t EnetUdma_allocProxy(Udma_DrvHandle hUdmaDrv,
                           Udma_ProxyHandle hUdmaProxy,
                           Udma_RingHandle hUdmaRing)
{
    Udma_ProxyCfg proxyCfg;
    int32_t retVal;

    /* Allocate a proxy for queue operation */
    retVal = Udma_proxyAlloc(hUdmaDrv, hUdmaProxy, UDMA_PROXY_ANY);
    ENETTRACE_ERR_IF((UDMA_SOK != retVal), retVal, "Proxy alloc failed");

    if (UDMA_SOK == retVal)
    {
        /* Config proxy for queue operation */
        proxyCfg.proxyMode = CSL_PROXY_QUEUE_ACCESS_MODE_TAIL;
        proxyCfg.elemSize  = UDMA_RING_ES_8BYTES;
        proxyCfg.ringNum   = Udma_ringGetNum(hUdmaRing);

        retVal = Udma_proxyConfig(hUdmaProxy, &proxyCfg);

        if (UDMA_SOK != retVal)
        {
            ENETTRACE_ERR(retVal, "Proxy config failed");
            Enet_assert(UDMA_SOK == Udma_proxyFree(hUdmaProxy));
        }
    }

    return retVal;
}

int32_t EnetUdma_freeProxy(Udma_ProxyHandle hUdmaProxy)
{
    int32_t retVal = UDMA_EBADARGS;

    if (NULL != hUdmaProxy)
    {
        retVal = Udma_proxyFree(hUdmaProxy);
    }

    return retVal;
}

Udma_RingMonHandle EnetUdma_allocRingMon(Udma_DrvHandle hUdmaDrv,
                                        Udma_RingHandle hUdmaRing,
                                        uint32_t elemCnt,
                                        const EnetUdma_RingMonCfg *pRingMonCfg)
{
    Udma_RingMonPrms monPrms;
    Udma_RingMonHandle hRingMon;
    int32_t retVal = UDMA_SOK;

    /* Allocate ring monitor object memory*/
    hRingMon = EnetUdma_memMgrAllocRingMonObj();
    if (NULL == hRingMon)
    {
        retVal = UDMA_EALLOC;
        ENETTRACE_ERR(retVal, "Ring monitor object memory allocation failed");
    }

    if (UDMA_SOK == retVal)
    {
        /* Allocate and configure a ring monitor */
        retVal = Udma_ringMonAlloc(hUdmaDrv, hRingMon, UDMA_RING_MON_ANY);
        if (UDMA_SOK != retVal)
        {
            ENETTRACE_ERR(retVal, "Ring monitor alloc failed");
            EnetUdma_memMgrFreeRingMonObj(hRingMon);
        }
    }

    if (UDMA_SOK == retVal)
    {
        UdmaRingMonPrms_init(&monPrms);
        monPrms.source  = TISCI_MSG_VALUE_RM_MON_SRC_ELEM_CNT;
        monPrms.mode    = pRingMonCfg->mode;
        monPrms.ringNum = Udma_ringGetNum(hUdmaRing);
        monPrms.data0   = pRingMonCfg->data0;
        monPrms.data1   = pRingMonCfg->data1;

        retVal = Udma_ringMonConfig(hRingMon, &monPrms);
        if (UDMA_SOK != retVal)
        {
            ENETTRACE_ERR(retVal, "Ring monitor config failed");
            /*If free also fails, we can't do much but assert */
            Enet_assert(UDMA_SOK == Udma_ringMonFree(hRingMon));
            hRingMon = NULL;
        }
    }

    return hRingMon;
}

int32_t EnetUdma_freeRingMon(Udma_RingMonHandle hRingMon)
{
    int32_t retVal = UDMA_EBADARGS;

    if (NULL != hRingMon)
    {
        retVal = Udma_ringMonFree(hRingMon);
        EnetUdma_memMgrFreeRingMonObj(hRingMon);
    }

    return retVal;
}

void EnetUdmaStats_updateNotifyStats(EnetDma_CbStats *pktStats,
                                    uint32_t pktCnt,
                                    uint32_t timeDiff)
{
#if defined(ENETUDMA_INSTRUMENTATION_ENABLED)
    uint32_t notifyCount;
    uint32_t timePerPkt = timeDiff / pktCnt;

    if (pktCnt == 0U)
    {
        pktStats->zeroNotifyCnt++;
    }

    notifyCount = pktStats->dataNotifyCnt & (ENET_UDMA_STATS_HISTORY_CNT - 1U);
    pktStats->dataNotifyCnt++;

    pktStats->totalPktCnt   += pktCnt;
    pktStats->totalCycleCnt += timeDiff;

    pktStats->cycleCntPerNotify[notifyCount] = timeDiff;
    pktStats->pktsPerNotify[notifyCount]     = pktCnt;
    pktStats->cycleCntPerPkt[notifyCount]    = timePerPkt;

    if (notifyCount != 0U)
    {
        /* As app submits all packets for Rx during init, we ignore that to get run-time stats */
        if (timeDiff > pktStats->cycleCntPerNotifyMax)
        {
            pktStats->cycleCntPerNotifyMax = timeDiff;
        }

        if (pktCnt > pktStats->pktsPerNotifyMax)
        {
            pktStats->pktsPerNotifyMax = pktCnt;
        }

        if (timePerPkt > pktStats->cycleCntPerPktMax)
        {
            pktStats->cycleCntPerPktMax = timePerPkt;
        }
    }
#endif
}

/*!
 * @}
 */
