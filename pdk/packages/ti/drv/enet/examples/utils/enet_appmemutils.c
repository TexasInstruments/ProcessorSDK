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

/**
 *  \file enet_appmemutils.c
 *
 *  \brief Enet DMA memory allocation utility functions.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* This is needed for memset/memcpy */
#include <string.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/include/core/enet_utils.h>

#include <ti/drv/enet/include/core/enet_dma.h>

#include "include/enet_appmemutils.h"
#include "include/enet_appmemutils_cfg.h"
#include "include/enet_apputils.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ENET_MEM_NUM_DESCS ((ENET_MEM_NUM_TX_PKTS * ENET_CFG_TX_CHANNELS_NUM) + \
                                    (ENET_MEM_NUM_RX_PKTS * ENET_CFG_RX_FLOWS_NUM))

/* TDCQ is allocated in driver, we allocate only FQ and CQ */
#define ENET_MEM_NUM_RINGS_TYPES (2U)

#define ENET_MEM_NUM_RINGS (ENET_MEM_NUM_RINGS_TYPES * \
                                    (ENET_CFG_TX_CHANNELS_NUM + ENET_CFG_RX_FLOWS_NUM))

#define ENET_MEM_RING_MAX_SIZE \
    (ENET_UDMA_RING_MEM_SIZE * ENET_MEM_RING_MAX_ELEM_CNT)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief
 */
typedef struct EnetMem_DmaDescMem_s
{
    /*! The node element so this packet can be added to a queue
     * Note- Keep EnetQ_Node as first member always as driver uses generic Q functions
     *       and deferences to this member */
    EnetQ_Node node;

    /*! DMA descriptor element */
    EnetUdma_DmaDesc dmaDesc
    __attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT)));

    /*! DMA descriptor state, refer to CpswUtils_DescStateMemMgr */
    uint32_t dmaDescState;
}EnetMem_DmaDescMem;

/**
 *  \brief
 */
typedef struct EnetMem_RingMem_s
{
    /*! The node element so this packet can be added to a queue
     * Note- Keep EnetQ_Node as first member always as driver uses generic Q functions
     *       and deferences to this member */
    EnetQ_Node node;

    /*! Ring memory element */
    uint8_t ringEle[ENET_UTILS_ALIGN(ENET_MEM_RING_MAX_SIZE, ENETDMA_CACHELINE_ALIGNMENT)]
    __attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT)));
}EnetMem_RingMem;

/**
 *  \brief
 */
typedef struct EnetMem_EthPktMem_s
{
    /*! The node element so this packet can be added to a queue
     * Note- Keep EnetQ_Node as first member always as driver uses generic Q functions
     *       and deferences to this member */
    EnetQ_Node node;

    /*! Eth packet info structure - shared with driver */
    EnetDma_Pkt dmaPkt;

    /*! Original packet size - we can't use this info from dmaPkt as app can change it */
    uint32_t orgBufSize;

    /*! Original packet size - we can't use this info from dmaPkt as app can change it */
    uintptr_t orgBufAddr;

}EnetMem_AppPktInfoMem;

typedef EnetQ EnetMem_DmaDescMemQ;

typedef EnetQ EnetMem_RingMemQ;

typedef EnetQ EnetMem_EthPktMemQ;

/**
 *  \brief
 */
typedef struct EnetMem_MemAllocObj_s
{
    /**< DMA packet Q */
    bool memUtilsInitFlag;

    /**< DMA packet Q */
    EnetMem_DmaDescMemQ dmaDescFreeQ;

    /**< Ring memory Q */
    EnetMem_RingMemQ ringMemQ;

    /**< Pool1 ethernet packet memory Q */
    EnetMem_EthPktMemQ ethPktMem_LargePoolQ;

    /**< Pool3 ethernet packet memory Q */
    EnetMem_EthPktMemQ ethPktMem_MediumPoolQ;

    /**< Pool3 ethernet packet memory Q */
    EnetMem_EthPktMemQ ethPktMem_SmallPoolQ;
} EnetMem_MemAllocObj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Enet UDMA DESC memories */
static EnetMem_DmaDescMem gDmaDescMemArray[ENET_MEM_NUM_DESCS]
__attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT),
                section(".bss:ENET_DMA_DESC_MEMPOOL")));

/* RX & TX RingAcc memories */
static EnetMem_RingMem gRingMemArray[ENET_MEM_NUM_RINGS]
__attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT),
                section(".bss:ENET_DMA_RING_MEMPOOL")));

/* Eth packet info memory Q - large pool */
static EnetMem_AppPktInfoMem gAppPktInfoMem_LargePool[ENET_MEM_LARGE_POOL_PACKET_NUM]
__attribute__ ((section(".bss:ENET_DMA_PKT_INFO_MEMPOOL")));

/* Eth packet large pool memories */
static uint8_t gEthPktMem_LargePool[ENET_MEM_LARGE_POOL_PACKET_NUM][ENET_MEM_LARGE_POOL_PKT_SIZE]
__attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT),
                section(".bss:ENET_DMA_PKT_MEMPOOL")));

/* Eth packet info memory Q - medium pool */
static EnetMem_AppPktInfoMem gAppPktInfoMem_MediumPool[ENET_MEM_MEDIUM_POOL_PACKET_NUM]
__attribute__ ((section(".bss:ENET_DMA_PKT_INFO_MEMPOOL")));

/* Eth packet medium pool memories */
static uint8_t gEthPktMem_MediumPool[ENET_MEM_MEDIUM_POOL_PACKET_NUM][ENET_MEM_MEDIUM_POOL_PKT_SIZE]
__attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT),
                section(".bss:ENET_DMA_PKT_MEMPOOL")));

/* Eth packet info memory Q - small pool */
static EnetMem_AppPktInfoMem gAppPktInfoMem_SmallPool[ENET_MEM_SMALL_POOL_PACKET_NUM]
__attribute__ ((section(".bss:ENET_DMA_PKT_INFO_MEMPOOL")));

/* Eth packet medium pool memories */
static uint8_t gEthPktMem_SmallPool[ENET_MEM_SMALL_POOL_PACKET_NUM][ENET_MEM_SMALL_POOL_PKT_SIZE]
__attribute__ ((aligned(ENETDMA_CACHELINE_ALIGNMENT),
                section(".bss:ENET_DMA_PKT_MEMPOOL")));
/* Cpsw mem utils driver object */
static EnetMem_MemAllocObj gEnetMemObj = {.memUtilsInitFlag = BFALSE};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*! Ethernet packet allocation function  */
EnetDma_Pkt *EnetMem_allocEthPkt(void *appPriv,
                                 uint32_t alignSize,
                                 uint32_t numScatterSegments,
                                 uint32_t scatterSegmentSize[])
{
    EnetDma_Pkt *pPktInfo                     = NULL;
    EnetMem_AppPktInfoMem *pAppPktInfoMem = NULL;
    EnetMem_EthPktMemQ *selectedQ         = NULL;
    uint32_t i;
    uintptr_t segmentBufAddr;
    uint32_t pktSize;

    pktSize = 0U;
    for (i = 0U; i < numScatterSegments; i++)
    {
        pktSize += scatterSegmentSize[i];
    }
    if (gEnetMemObj.memUtilsInitFlag == BTRUE)
    {
        if (ENET_MEM_SMALL_POOL_PKT_SIZE >= pktSize)
        {
            /* Allocate packet from  smallest pool*/
            selectedQ = &gEnetMemObj.ethPktMem_SmallPoolQ;
        }
        else if (ENET_MEM_MEDIUM_POOL_PKT_SIZE >= pktSize)
        {
            selectedQ = &gEnetMemObj.ethPktMem_MediumPoolQ;
        }
        else if (ENET_MEM_LARGE_POOL_PKT_SIZE >= pktSize)
        {
            selectedQ = &gEnetMemObj.ethPktMem_LargePoolQ;
        }
        else
        {
            selectedQ = NULL;
        }

        if (NULL != selectedQ)
        {
            pAppPktInfoMem = (EnetMem_AppPktInfoMem *)EnetQueue_deq(selectedQ);
        }

        if (NULL != pAppPktInfoMem)
        {
            pPktInfo = &pAppPktInfoMem->dmaPkt;

            EnetAppUtils_assert(numScatterSegments <= ENET_ARRAYSIZE(pPktInfo->sgList.list));
            segmentBufAddr = pAppPktInfoMem->orgBufAddr;
            for(i = 0U; i < numScatterSegments; i++)
            {
                pPktInfo->sgList.list[i].bufPtr = (uint8_t *)(ENET_UTILS_ALIGN(segmentBufAddr, alignSize));
                pPktInfo->sgList.list[i].segmentAllocLen = scatterSegmentSize[i];
                segmentBufAddr = (uintptr_t)(pPktInfo->sgList.list[i].bufPtr) + scatterSegmentSize[i];
                EnetAppUtils_assert(segmentBufAddr <= (pAppPktInfoMem->orgBufAddr + pAppPktInfoMem->orgBufSize));
            }
            pPktInfo->sgList.numScatterSegments = numScatterSegments;
            pPktInfo->appPriv = (void *)appPriv;
            EnetDma_checkPktState(&pPktInfo->pktState,
                                      ENET_PKTSTATE_MODULE_MEMMGR,
                                      ENET_PKTSTATE_MEMMGR_FREE,
                                      ENET_PKTSTATE_MEMMGR_ALLOC);
            ENET_UTILS_SET_PKT_DRIVER_STATE(&pPktInfo->pktState,
                                                ENET_PKTSTATE_DMA_NOT_WITH_HW);
        }
        else
        {
            pPktInfo = NULL;
        }
    }

    return pPktInfo;
}

/*! Ethernet packet free function  */
void EnetMem_freeEthPkt(EnetDma_Pkt *pPktInfo)
{
    EnetMem_AppPktInfoMem *pAppPktInfoMem;

    if (gEnetMemObj.memUtilsInitFlag == BTRUE)
    {
        EnetDma_checkPktState(&pPktInfo->pktState,
                                ENET_PKTSTATE_MODULE_MEMMGR,
                                ENET_PKTSTATE_MEMMGR_ALLOC,
                                ENET_PKTSTATE_MEMMGR_FREE);
        pPktInfo->appPriv = NULL;
        pAppPktInfoMem    = container_of(pPktInfo, EnetMem_AppPktInfoMem, dmaPkt);

        ENET_UTILS_COMPILETIME_ASSERT(offsetof(EnetMem_AppPktInfoMem, node) == 0);
        if (ENET_MEM_SMALL_POOL_PKT_SIZE == pAppPktInfoMem->orgBufSize)
        {
            EnetQueue_enq(&gEnetMemObj.ethPktMem_SmallPoolQ,
                          &pAppPktInfoMem->node);
        }
        else if (ENET_MEM_MEDIUM_POOL_PKT_SIZE == pAppPktInfoMem->orgBufSize)
        {
            EnetQueue_enq(&gEnetMemObj.ethPktMem_MediumPoolQ,
                          &pAppPktInfoMem->node);
        }
        else if (ENET_MEM_LARGE_POOL_PKT_SIZE == pAppPktInfoMem->orgBufSize)
        {
            EnetQueue_enq(&gEnetMemObj.ethPktMem_LargePoolQ,
                          &pAppPktInfoMem->node);
        }
        else
        {
            /* packet is not allocated by me */
            EnetAppUtils_assert(BFALSE);
        }
    }

    return;
}

/*! Ring memory allocation function  */
uint8_t *EnetMem_allocRingMem(void *appPriv,
                                         uint32_t numRingEle,
                                         uint32_t alignSize)
{
    uint8_t *ringMemPtr = NULL;
    EnetMem_RingMem *pRingMemEle;

    if (gEnetMemObj.memUtilsInitFlag == BTRUE)
    {
        EnetAppUtils_assert(numRingEle <= ENET_MEM_RING_MAX_ELEM_CNT);
        pRingMemEle = (EnetMem_RingMem *)EnetQueue_deq(&gEnetMemObj.ringMemQ);
        if (pRingMemEle != NULL)
        {
            ringMemPtr = &pRingMemEle->ringEle[0U];

            EnetAppUtils_assert(ENET_UTILS_IS_ALIGNED(ringMemPtr, alignSize));
        }
        else
        {
            ringMemPtr = NULL;
        }
    }

    return ringMemPtr;
}

/*! Ring memory free function  */
void EnetMem_freeRingMem(const void *appPriv,
                                    void *ringMemPtr,
                                    uint32_t numRingEle)
{
    EnetMem_RingMem *pRingMemEle;

    if (gEnetMemObj.memUtilsInitFlag == BTRUE)
    {
        pRingMemEle = container_of((const uint8_t *)ringMemPtr, EnetMem_RingMem, ringEle[0U]);
        /* TODO - just to get it compiling - Need to fix this after discussion with Misa/Badri */
        EnetQueue_enq(&gEnetMemObj.ringMemQ, &pRingMemEle->node);
    }

    return;
}

/*! DMA packet allocation function  */
EnetUdma_DmaDesc *EnetMem_allocDmaDesc(void *appPriv,
                                                 uint32_t alignSize)
{
    EnetUdma_DmaDesc *dmaDescPtr = NULL;
    EnetMem_DmaDescMem *pDmaDescMem;

    if (gEnetMemObj.memUtilsInitFlag == BTRUE)
    {
        pDmaDescMem = (EnetMem_DmaDescMem *)
                      EnetQueue_deq(&gEnetMemObj.dmaDescFreeQ);
        if (NULL != pDmaDescMem)
        {
            dmaDescPtr = &pDmaDescMem->dmaDesc;
            if (!ENET_UTILS_IS_ALIGNED(dmaDescPtr, alignSize))
            {
                EnetQueue_enq(&gEnetMemObj.dmaDescFreeQ,
                              &pDmaDescMem->node);
                dmaDescPtr = NULL;
            }
            else
            {
                EnetDma_checkDescState(&pDmaDescMem->dmaDescState,
                                         ENET_DESCSTATE_MEMMGR_FREE,
                                         ENET_DESCSTATE_MEMMGR_ALLOC);
            }
        }
    }

    return dmaDescPtr;
}

/*! DMA packet free function  */
void EnetMem_freeDmaDesc(void *appPriv,
                                    EnetUdma_DmaDesc *dmaDescPtr)
{
    EnetMem_DmaDescMem *pDmaDescMem;

    if (gEnetMemObj.memUtilsInitFlag == BTRUE)
    {
        pDmaDescMem = container_of(dmaDescPtr, EnetMem_DmaDescMem, dmaDesc);
        EnetDma_checkDescState(&pDmaDescMem->dmaDescState,
                                 ENET_DESCSTATE_MEMMGR_ALLOC,
                                 ENET_DESCSTATE_MEMMGR_FREE);
        EnetQueue_enq(&gEnetMemObj.dmaDescFreeQ, &pDmaDescMem->node);
    }

    return;
}

int32_t EnetMem_init(void)
{
    uint32_t i;
    int32_t retVal = ENET_SOK;
    EnetDma_Pkt *dmaPkt;
    EnetMem_AppPktInfoMem *pAppPktInfoMem;
    uint32_t alignSize = ENETDMA_CACHELINE_ALIGNMENT;

    if (gEnetMemObj.memUtilsInitFlag == BFALSE)
    {
        memset(&gEnetMemObj, 0U, sizeof(EnetMem_MemAllocObj));
        memset(&gRingMemArray, 0U, sizeof(gRingMemArray));

        /*********************** DMA packet Q ************************/
        if (ENET_SOK == retVal)
        {
            EnetQueue_initQ(&gEnetMemObj.dmaDescFreeQ);
            /* Initialize the DMA packet and enqueue into free packet Q */
            for (i = 0U; i < ENET_MEM_NUM_DESCS; i++)
            {
                gDmaDescMemArray[i].dmaDescState = 0;
                if (!ENET_UTILS_IS_ALIGNED(&gDmaDescMemArray[i].dmaDesc, alignSize))
                {
                    retVal = ENET_EFAIL;
                    break;
                }

                EnetQueue_enq(&gEnetMemObj.dmaDescFreeQ, &gDmaDescMemArray[i].node);
                ENET_UTILS_SET_DESC_MEMMGR_STATE(&gDmaDescMemArray[i].dmaDescState, ENET_DESCSTATE_MEMMGR_FREE);
            }
        }

        /*********************** Ring Mem Q ************************/
        if (ENET_SOK == retVal)
        {
            EnetQueue_initQ(&gEnetMemObj.ringMemQ);
            /* Enqueue ring memory element into free packet Q */
            for (i = 0U; i < ENET_MEM_NUM_RINGS; i++)
            {
                if (!ENET_UTILS_IS_ALIGNED(&gRingMemArray[i].ringEle, alignSize))
                {
                    retVal = ENET_EFAIL;
                    break;
                }

                EnetQueue_enq(&gEnetMemObj.ringMemQ, &gRingMemArray[i].node);
            }
        }

        /****************** memory Q for Large pool ethernet packets *****************/
        if (ENET_SOK == retVal)
        {
            EnetQueue_initQ(&gEnetMemObj.ethPktMem_LargePoolQ);
            /* Enqueue ring memory element into free packet Q */
            for (i = 0U; i < ENET_MEM_LARGE_POOL_PACKET_NUM; i++)
            {
                pAppPktInfoMem = &gAppPktInfoMem_LargePool[i];
                /* Keep record of allocated size - we use this in free */
                pAppPktInfoMem->orgBufSize = ENET_MEM_LARGE_POOL_PKT_SIZE;
                pAppPktInfoMem->orgBufAddr = (uintptr_t) (&gEthPktMem_LargePool[i][0]);
                dmaPkt                = &pAppPktInfoMem->dmaPkt;
                EnetDma_initPktInfo(dmaPkt);
                ENET_UTILS_SET_PKT_MEMMGR_STATE(&dmaPkt->pktState,
                                                  ENET_PKTSTATE_MEMMGR_FREE);
                EnetQueue_enq(&gEnetMemObj.ethPktMem_LargePoolQ, &pAppPktInfoMem->node);
            }
        }

        /****************** memory Q for Medium pool ethernet packets *****************/
        if (ENET_SOK == retVal)
        {
            EnetQueue_initQ(&gEnetMemObj.ethPktMem_MediumPoolQ);
            /* Enqueue ring memory element into free packet Q */
            for (i = 0U; i < ENET_MEM_MEDIUM_POOL_PACKET_NUM; i++)
            {
                pAppPktInfoMem = &gAppPktInfoMem_MediumPool[i];
                /* Keep record of allocated size - we use this in free */
                pAppPktInfoMem->orgBufSize = ENET_MEM_MEDIUM_POOL_PKT_SIZE;
                pAppPktInfoMem->orgBufAddr = (uintptr_t) (&gEthPktMem_MediumPool[i][0U]);
                dmaPkt                = &pAppPktInfoMem->dmaPkt;
                EnetDma_initPktInfo(dmaPkt);
                ENET_UTILS_SET_PKT_MEMMGR_STATE(&dmaPkt->pktState, ENET_PKTSTATE_MEMMGR_FREE);
                EnetQueue_enq(&gEnetMemObj.ethPktMem_MediumPoolQ, &pAppPktInfoMem->node);
            }
        }

        /****************** memory Q for Small pool ethernet packets *****************/
        if (ENET_SOK == retVal)
        {
            EnetQueue_initQ(&gEnetMemObj.ethPktMem_SmallPoolQ);
            /* Enqueue ring memory element into free packet Q */
            for (i = 0U; i < ENET_MEM_SMALL_POOL_PACKET_NUM; i++)
            {
                pAppPktInfoMem = &gAppPktInfoMem_SmallPool[i];
                /* Keep record of allocated size - we use this in free */
                pAppPktInfoMem->orgBufSize = ENET_MEM_SMALL_POOL_PKT_SIZE;
                pAppPktInfoMem->orgBufAddr = (uintptr_t) (&gEthPktMem_SmallPool[i][0U]);
                dmaPkt                = &pAppPktInfoMem->dmaPkt;
                EnetDma_initPktInfo(dmaPkt);
                ENET_UTILS_SET_PKT_MEMMGR_STATE(&dmaPkt->pktState, ENET_PKTSTATE_MEMMGR_FREE);
                EnetQueue_enq(&gEnetMemObj.ethPktMem_SmallPoolQ, &pAppPktInfoMem->node);
            }
        }

        if (ENET_SOK == retVal)
        {
            gEnetMemObj.memUtilsInitFlag = BTRUE;
        }
    }

    EnetAppUtils_assert(retVal == ENET_SOK);
    return retVal;
}

void EnetMem_deInit(void)
{
    uint32_t i;
    int32_t status = ENET_SOK;
    EnetMem_AppPktInfoMem *pAppPktInfoMem;

    if (gEnetMemObj.memUtilsInitFlag)
    {
        if (EnetQueue_getQCount(&gEnetMemObj.ringMemQ) != ENET_MEM_NUM_RINGS)
        {
            EnetAppUtils_print("RingMemQ: Before: %d, after: %d\n",
                               ENET_MEM_NUM_RINGS,
                               EnetQueue_getQCount(&gEnetMemObj.ringMemQ));
            status = ENET_EFAIL;
        }

        if (EnetQueue_getQCount(&gEnetMemObj.dmaDescFreeQ) != ENET_MEM_NUM_DESCS)
        {
            EnetAppUtils_print("DmaDesQ: Before: %d, after: %d\n",
                               ENET_MEM_NUM_DESCS,
                               EnetQueue_getQCount(&gEnetMemObj.dmaDescFreeQ));
            status = ENET_EFAIL;
        }

        if (EnetQueue_getQCount(&gEnetMemObj.ethPktMem_LargePoolQ) !=
                                 ENET_MEM_LARGE_POOL_PACKET_NUM)
        {
            EnetAppUtils_print("PktMemQ Large: Before: %d, after: %d\n",
                               ENET_MEM_LARGE_POOL_PACKET_NUM,
                               EnetQueue_getQCount(&gEnetMemObj.ethPktMem_LargePoolQ));
            status = ENET_EFAIL;
        }

        if (EnetQueue_getQCount(&gEnetMemObj.ethPktMem_MediumPoolQ) !=
                                 ENET_MEM_MEDIUM_POOL_PACKET_NUM)
        {
            EnetAppUtils_print("PktMemQ Medium: Before: %d, after: %d\n",
                               ENET_MEM_MEDIUM_POOL_PACKET_NUM,
                               EnetQueue_getQCount(&gEnetMemObj.ethPktMem_MediumPoolQ));
            status = ENET_EFAIL;
        }

        if (EnetQueue_getQCount(&gEnetMemObj.ethPktMem_SmallPoolQ) !=
                                 ENET_MEM_SMALL_POOL_PACKET_NUM)
        {
            EnetAppUtils_print("PktMemQ Small: Before: %d, after: %d\n",
                               ENET_MEM_SMALL_POOL_PACKET_NUM,
                               EnetQueue_getQCount(&gEnetMemObj.ethPktMem_SmallPoolQ));
            status = ENET_EFAIL;
        }

        /* Assert if any of the Q has memory leak */
        EnetAppUtils_assert(ENET_SOK == status);

        memset(&gEnetMemObj, 0U, sizeof(EnetMem_MemAllocObj));

        for (i = 0U; i < ENET_MEM_NUM_DESCS; i++)
        {
            EnetDma_checkDescState(&gDmaDescMemArray[i].dmaDescState,
                                     ENET_DESCSTATE_MEMMGR_FREE,
                                     ENET_DESCSTATE_MEMMGR_FREE);
        }

        for (i = 0U; i < ENET_MEM_SMALL_POOL_PACKET_NUM; i++)
        {
            pAppPktInfoMem = &gAppPktInfoMem_SmallPool[i];
            EnetDma_Pkt *dmaPkt = &pAppPktInfoMem->dmaPkt;

            EnetDma_checkPktState(&dmaPkt->pktState,
                                    ENET_PKTSTATE_MODULE_MEMMGR,
                                    ENET_PKTSTATE_MEMMGR_FREE,
                                    ENET_PKTSTATE_MEMMGR_FREE);
        }

        for (i = 0U; i < ENET_MEM_MEDIUM_POOL_PACKET_NUM; i++)
        {
            pAppPktInfoMem = &gAppPktInfoMem_MediumPool[i];
            EnetDma_Pkt *dmaPkt = &pAppPktInfoMem->dmaPkt;

            EnetDma_checkPktState(&dmaPkt->pktState,
                                    ENET_PKTSTATE_MODULE_MEMMGR,
                                    ENET_PKTSTATE_MEMMGR_FREE,
                                    ENET_PKTSTATE_MEMMGR_FREE);
        }

        for (i = 0U; i < ENET_MEM_LARGE_POOL_PACKET_NUM; i++)
        {
            pAppPktInfoMem = &gAppPktInfoMem_LargePool[i];
            EnetDma_Pkt *dmaPkt = &pAppPktInfoMem->dmaPkt;

            EnetDma_checkPktState(&dmaPkt->pktState,
                                    ENET_PKTSTATE_MODULE_MEMMGR,
                                    ENET_PKTSTATE_MEMMGR_FREE,
                                    ENET_PKTSTATE_MEMMGR_FREE);
        }
    }
}

/* ========================================================================== */
/*                          Static Function Definitions                       */
/* ========================================================================== */

/* end of file */
