/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2022
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
 *  \file udma_rm.c
 *
 *  \brief File containing the UDMA driver Resource Manager (RM)
 *  abstraction functions.
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

static int32_t Udma_rmSetSharedResRmInitPrms(const Udma_RmSharedResPrms *rmSharedResPrms,
                                             uint32_t  instId,
                                             uint32_t  startInstId,
                                             uint32_t  rangeStart,
                                             uint32_t  rangeTotalNum,
                                             uint32_t *start,
                                             uint32_t *num);

static int32_t Udma_rmGetSciclientDefaultBoardCfgRmRange(const Udma_RmDefBoardCfgPrms *rmDefBoardCfgPrms,
                                                         Udma_RmDefBoardCfgResp *rmDefBoardCfgResp,
                                                         uint32_t *splitResFlag);

static int32_t Udma_rmCheckResLeak(Udma_DrvHandle drvHandle,
                                   const uint32_t *allocFlag,
                                   uint32_t numRes,
                                   uint32_t arrSize);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Udma_rmInit(Udma_DrvHandle drvHandle)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
#if (UDMA_NUM_UTC_INSTANCE > 0)
    uint32_t            utcId;
#endif
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    uint32_t            mappedGrp;
#endif

    /* Mark all resources as free */
    for(i = 0U; i < rmInitPrms->numBlkCopyCh; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_CH_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->blkCopyChFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numBlkCopyHcCh; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_HC_CH_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->blkCopyHcChFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numBlkCopyUhcCh; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_UHC_CH_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->blkCopyUhcChFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numTxCh; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_TX_CH_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->txChFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numRxCh; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_RX_CH_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->rxChFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numTxHcCh; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_TX_HC_CH_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->txHcChFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numRxHcCh; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_RX_HC_CH_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->rxHcChFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numTxUhcCh; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_TX_UHC_CH_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->txUhcChFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numRxUhcCh; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_RX_UHC_CH_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->rxUhcChFlag[offset] |= bitMask;
    }
    #if (UDMA_NUM_UTC_INSTANCE > 0)
    for(utcId = 0U; utcId < UDMA_NUM_UTC_INSTANCE; utcId++)
    {
        for(i = 0U; i < rmInitPrms->numUtcCh[utcId]; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_UTC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->utcChFlag[utcId][offset] |= bitMask;
        }
    }
    #endif
    #if (UDMA_NUM_MAPPED_TX_GROUP > 0)
    for(mappedGrp = 0U; mappedGrp < UDMA_NUM_MAPPED_TX_GROUP; mappedGrp++)
    {
        for(i = 0U; i < rmInitPrms->numMappedTxCh[mappedGrp]; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_TX_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->mappedTxChFlag[mappedGrp][offset] |= bitMask;
        }
    }
    #endif
    #if (UDMA_NUM_MAPPED_RX_GROUP > 0)
    for(mappedGrp = 0U; mappedGrp < UDMA_NUM_MAPPED_RX_GROUP; mappedGrp++)
    {
        for(i = 0U; i < rmInitPrms->numMappedRxCh[mappedGrp]; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_RX_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->mappedRxChFlag[mappedGrp][offset] |= bitMask;
        }
    }
    #endif
    #if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    for(mappedGrp = 0U; mappedGrp < (UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP); mappedGrp++)
    {
        for(i = 0U; i < rmInitPrms->numMappedRing[mappedGrp]; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_RING_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->mappedRingFlag[mappedGrp][offset] |= bitMask;
        }
    }
    #endif
    for(i = 0U; i < rmInitPrms->numFreeRing; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_FREE_RING_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->freeRingFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numFreeFlow; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_FREE_FLOW_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->freeFlowFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numGlobalEvent; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_GLOBAL_EVENT_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->globalEventFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numVintr; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_VINTR_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->vintrFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numIrIntr; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_IR_INTR_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->irIntrFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numProxy; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_PROXY_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->proxyFlag[offset] |= bitMask;
    }
    for(i = 0U; i < rmInitPrms->numRingMon; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_RING_MON_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        drvHandle->ringMonFlag[offset] |= bitMask;
    }

    return (retVal);
}

int32_t Udma_rmDeinit(Udma_DrvHandle drvHandle)
{
    int32_t             retVal = UDMA_SOK;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
#if (UDMA_NUM_UTC_INSTANCE > 0)
    uint32_t            utcId;
#endif
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    uint32_t            mappedGrp;
#endif

    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->blkCopyChFlag[0U],
                  rmInitPrms->numBlkCopyCh,
                  UDMA_RM_BLK_COPY_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->blkCopyHcChFlag[0U],
                  rmInitPrms->numBlkCopyHcCh,
                  UDMA_RM_BLK_COPY_HC_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->blkCopyUhcChFlag[0U],
                  rmInitPrms->numBlkCopyUhcCh,
                  UDMA_RM_BLK_COPY_UHC_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->txChFlag[0U],
                  rmInitPrms->numTxCh,
                  UDMA_RM_TX_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->txHcChFlag[0U],
                  rmInitPrms->numTxHcCh,
                  UDMA_RM_TX_HC_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->txUhcChFlag[0U],
                  rmInitPrms->numTxUhcCh,
                  UDMA_RM_TX_UHC_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->rxChFlag[0U],
                  rmInitPrms->numRxCh,
                  UDMA_RM_RX_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->rxHcChFlag[0U],
                  rmInitPrms->numRxHcCh,
                  UDMA_RM_RX_HC_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->rxUhcChFlag[0U],
                  rmInitPrms->numRxUhcCh,
                  UDMA_RM_RX_UHC_CH_ARR_SIZE);
#if (UDMA_NUM_UTC_INSTANCE > 0)
    for(utcId = 0U; utcId < UDMA_NUM_UTC_INSTANCE; utcId++)
    {
        retVal += Udma_rmCheckResLeak(
                      drvHandle,
                      &drvHandle->utcChFlag[utcId][0U],
                      rmInitPrms->numUtcCh[utcId],
                      UDMA_RM_UTC_CH_ARR_SIZE);
    }
#endif
#if (UDMA_NUM_MAPPED_TX_GROUP > 0)
    for(mappedGrp = 0U; mappedGrp < UDMA_NUM_MAPPED_TX_GROUP; mappedGrp++)
    {
        retVal += Udma_rmCheckResLeak(
                      drvHandle,
                      &drvHandle->mappedTxChFlag[mappedGrp][0U],
                      rmInitPrms->numMappedTxCh[mappedGrp],
                      UDMA_RM_MAPPED_TX_CH_ARR_SIZE);
    }
#endif
#if (UDMA_NUM_MAPPED_RX_GROUP > 0)
    for(mappedGrp = 0U; mappedGrp < UDMA_NUM_MAPPED_RX_GROUP; mappedGrp++)
    {
        retVal += Udma_rmCheckResLeak(
                      drvHandle,
                      &drvHandle->mappedRxChFlag[mappedGrp][0U],
                      rmInitPrms->numMappedRxCh[mappedGrp],
                      UDMA_RM_MAPPED_RX_CH_ARR_SIZE);
    }
#endif
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    for(mappedGrp = 0U; mappedGrp < (UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP); mappedGrp++)
    {
        retVal += Udma_rmCheckResLeak(
                      drvHandle,
                      &drvHandle->mappedRingFlag[mappedGrp][0U],
                      rmInitPrms->numMappedRing[mappedGrp],
                      UDMA_RM_MAPPED_RING_ARR_SIZE);
    }
#endif
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->freeRingFlag[0U],
                  rmInitPrms->numFreeRing,
                  UDMA_RM_FREE_RING_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->freeFlowFlag[0U],
                  rmInitPrms->numFreeFlow,
                  UDMA_RM_FREE_FLOW_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->globalEventFlag[0U],
                  rmInitPrms->numGlobalEvent,
                  UDMA_RM_GLOBAL_EVENT_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->vintrFlag[0U],
                  rmInitPrms->numVintr,
                  UDMA_RM_VINTR_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->irIntrFlag[0U],
                  rmInitPrms->numIrIntr,
                  UDMA_RM_IR_INTR_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->proxyFlag[0U],
                  rmInitPrms->numProxy,
                  UDMA_RM_PROXY_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->ringMonFlag[0U],
                  rmInitPrms->numRingMon,
                  UDMA_RM_RING_MON_ARR_SIZE);

    return (retVal);
}

uint32_t Udma_rmAllocBlkCopyCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from Blk Copy channel pool */
        for(i = 0U; i < rmInitPrms->numBlkCopyCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_BLK_COPY_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->blkCopyChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->blkCopyChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startBlkCopyCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific Block Copy channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startBlkCopyCh) &&
           (preferredChNum < (rmInitPrms->startBlkCopyCh + rmInitPrms->numBlkCopyCh)))
        {
            i = preferredChNum - rmInitPrms->startBlkCopyCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_BLK_COPY_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->blkCopyChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->blkCopyChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeBlkCopyCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= (rmInitPrms->startBlkCopyCh));
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startBlkCopyCh + rmInitPrms->numBlkCopyCh));
    i = chNum - rmInitPrms->startBlkCopyCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_BLK_COPY_CH_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (0U == (drvHandle->blkCopyChFlag[offset] & bitMask)));
    drvHandle->blkCopyChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocBlkCopyHcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from Blk Copy high capacity channel pool */
        for(i = 0U; i < rmInitPrms->numBlkCopyHcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_BLK_COPY_HC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->blkCopyHcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->blkCopyHcChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startBlkCopyHcCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific Block Copy channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startBlkCopyHcCh) &&
           (preferredChNum < (rmInitPrms->startBlkCopyHcCh + rmInitPrms->numBlkCopyHcCh)))
        {
            i = preferredChNum - rmInitPrms->startBlkCopyHcCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_BLK_COPY_HC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->blkCopyHcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->blkCopyHcChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeBlkCopyHcCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= (rmInitPrms->startBlkCopyHcCh));
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startBlkCopyHcCh + rmInitPrms->numBlkCopyHcCh));
    i = chNum - rmInitPrms->startBlkCopyHcCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_BLK_COPY_HC_CH_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (0U == (drvHandle->blkCopyHcChFlag[offset] & bitMask)));
    drvHandle->blkCopyHcChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocBlkCopyUhcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from Blk Copy ultra high capacity channel pool */
        for(i = 0U; i < rmInitPrms->numBlkCopyUhcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_BLK_COPY_UHC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->blkCopyUhcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->blkCopyUhcChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startBlkCopyUhcCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific Block Copy channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startBlkCopyUhcCh) &&
           (preferredChNum < (rmInitPrms->startBlkCopyUhcCh + rmInitPrms->numBlkCopyUhcCh)))
        {
            i = preferredChNum - rmInitPrms->startBlkCopyUhcCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_BLK_COPY_UHC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->blkCopyUhcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->blkCopyUhcChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeBlkCopyUhcCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= (rmInitPrms->startBlkCopyUhcCh));
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startBlkCopyUhcCh + rmInitPrms->numBlkCopyUhcCh));
    i = chNum - rmInitPrms->startBlkCopyUhcCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_BLK_COPY_UHC_CH_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (0U == (drvHandle->blkCopyUhcChFlag[offset] & bitMask)));
    drvHandle->blkCopyUhcChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocTxCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from TX channel pool */
        for(i = 0U; i < rmInitPrms->numTxCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_TX_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->txChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->txChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startTxCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific TX channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startTxCh) &&
           (preferredChNum < (rmInitPrms->startTxCh + rmInitPrms->numTxCh)))
        {
            i = preferredChNum - rmInitPrms->startTxCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_TX_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->txChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->txChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeTxCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= (rmInitPrms->startTxCh));
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startTxCh + rmInitPrms->numTxCh));
    i = chNum - rmInitPrms->startTxCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_TX_CH_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (0U == (drvHandle->txChFlag[offset] & bitMask)));
    drvHandle->txChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocRxCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from RX channel pool */
        for(i = 0U; i < rmInitPrms->numRxCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_RX_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->rxChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->rxChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startRxCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific RX channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startRxCh) &&
           (preferredChNum < (rmInitPrms->startRxCh + rmInitPrms->numRxCh)))
        {
            i = preferredChNum - rmInitPrms->startRxCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_RX_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->rxChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->rxChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeRxCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= rmInitPrms->startRxCh);
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startRxCh + rmInitPrms->numRxCh));
    i = chNum - rmInitPrms->startRxCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_RX_CH_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (0U == (drvHandle->rxChFlag[offset] & bitMask)));
    drvHandle->rxChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocTxHcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from TX HC channel pool */
        for(i = 0U; i < rmInitPrms->numTxHcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_TX_HC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->txHcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->txHcChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startTxHcCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific TX HC channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startTxHcCh) &&
           (preferredChNum < (rmInitPrms->startTxHcCh + rmInitPrms->numTxHcCh)))
        {
            i = preferredChNum - rmInitPrms->startTxHcCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_TX_HC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->txHcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->txHcChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeTxHcCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= (rmInitPrms->startTxHcCh));
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startTxHcCh + rmInitPrms->numTxHcCh));
    i = chNum - rmInitPrms->startTxHcCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_TX_HC_CH_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (0U == (drvHandle->txHcChFlag[offset] & bitMask)));
    drvHandle->txHcChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocRxHcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from RX HC channel pool */
        for(i = 0U; i < rmInitPrms->numRxHcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_RX_HC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->rxHcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->rxHcChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startRxHcCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific RX HC channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startRxHcCh) &&
           (preferredChNum < (rmInitPrms->startRxHcCh + rmInitPrms->numRxHcCh)))
        {
            i = preferredChNum - rmInitPrms->startRxHcCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_RX_HC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->rxHcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->rxHcChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeRxHcCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= rmInitPrms->startRxHcCh);
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startRxHcCh + rmInitPrms->numRxHcCh));
    i = chNum - rmInitPrms->startRxHcCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_RX_HC_CH_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (0U == (drvHandle->rxHcChFlag[offset] & bitMask)));
    drvHandle->rxHcChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocTxUhcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from TX UHC channel pool */
        for(i = 0U; i < rmInitPrms->numTxUhcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_TX_UHC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->txUhcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->txUhcChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startTxUhcCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific TX UHC channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startTxUhcCh) &&
           (preferredChNum < (rmInitPrms->startTxUhcCh + rmInitPrms->numTxUhcCh)))
        {
            i = preferredChNum - rmInitPrms->startTxUhcCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_TX_UHC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->txUhcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->txUhcChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeTxUhcCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= (rmInitPrms->startTxUhcCh));
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startTxUhcCh + rmInitPrms->numTxUhcCh));
    i = chNum - rmInitPrms->startTxUhcCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_TX_UHC_CH_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (0U == (drvHandle->txUhcChFlag[offset] & bitMask)));
    drvHandle->txUhcChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocRxUhcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from RX UHC channel pool */
        for(i = 0U; i < rmInitPrms->numRxUhcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_RX_UHC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->rxUhcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->rxUhcChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startRxUhcCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific RX UHC channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startRxUhcCh) &&
           (preferredChNum < (rmInitPrms->startRxUhcCh + rmInitPrms->numRxUhcCh)))
        {
            i = preferredChNum - rmInitPrms->startRxUhcCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_RX_UHC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->rxUhcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->rxUhcChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeRxUhcCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= rmInitPrms->startRxUhcCh);
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startRxUhcCh + rmInitPrms->numRxUhcCh));
    i = chNum - rmInitPrms->startRxUhcCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_RX_UHC_CH_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (0U == (drvHandle->rxUhcChFlag[offset] & bitMask)));
    drvHandle->rxUhcChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

#if (UDMA_NUM_UTC_INSTANCE > 0)
uint32_t Udma_rmAllocExtCh(uint32_t preferredChNum,
                           Udma_DrvHandle drvHandle,
                           const Udma_UtcInstInfo *utcInfo)
{
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            utcId;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, (NULL_PTR != utcInfo));
    utcId = utcInfo->utcId;
    Udma_assert(drvHandle, (utcId < UDMA_NUM_UTC_INSTANCE));
    Udma_assert(drvHandle,
        rmInitPrms->startUtcCh[utcId] >= utcInfo->startCh);
    Udma_assert(drvHandle,
        (rmInitPrms->startUtcCh[utcId] < (utcInfo->startCh + utcInfo->numCh)));
    Udma_assert(drvHandle,
        (rmInitPrms->startUtcCh[utcId] + rmInitPrms->numUtcCh[utcId]) <=
            (utcInfo->startCh + utcInfo->numCh));

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from specific external channel pool */
        for(i = 0U; i < rmInitPrms->numUtcCh[utcId]; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_UTC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->utcChFlag[utcId][offset] & bitMask) == bitMask)
            {
                drvHandle->utcChFlag[utcId][offset] &= ~bitMask;
                chNum = i + rmInitPrms->startUtcCh[utcId];  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        if(preferredChNum < rmInitPrms->numUtcCh[utcId])
        {
            i = preferredChNum;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_UTC_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->utcChFlag[utcId][offset] & bitMask) == bitMask)
            {
                drvHandle->utcChFlag[utcId][offset] &= ~bitMask;
                chNum = preferredChNum + rmInitPrms->startUtcCh[utcId];
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeExtCh(uint32_t chNum,
                      Udma_DrvHandle drvHandle,
                      const Udma_UtcInstInfo *utcInfo)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            utcId;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, (NULL_PTR != utcInfo));
    utcId = utcInfo->utcId;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);
    Udma_assert(drvHandle, chNum >= rmInitPrms->startUtcCh[utcId]);
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startUtcCh[utcId] + rmInitPrms->numUtcCh[utcId]));
    i = chNum - rmInitPrms->startUtcCh[utcId];
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_UTC_CH_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (0U == (drvHandle->utcChFlag[utcId][offset] & bitMask)));
    drvHandle->utcChFlag[utcId][offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}
#endif

#if (UDMA_NUM_MAPPED_TX_GROUP > 0)
uint32_t Udma_rmAllocMappedTxCh(uint32_t preferredChNum,
                                Udma_DrvHandle drvHandle,
                                const uint32_t mappedChGrp)
{
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, (mappedChGrp < UDMA_NUM_MAPPED_TX_GROUP));

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from specific mapped TX channel pool */
        for(i = 0U; i < rmInitPrms->numMappedTxCh[mappedChGrp]; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_MAPPED_TX_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->mappedTxChFlag[mappedChGrp][offset] & bitMask) == bitMask)
            {
                drvHandle->mappedTxChFlag[mappedChGrp][offset] &= ~bitMask;
                chNum = i + rmInitPrms->startMappedTxCh[mappedChGrp];  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific mapped TX channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startMappedTxCh[mappedChGrp]) &&
           (preferredChNum < (rmInitPrms->startMappedTxCh[mappedChGrp] + rmInitPrms->numMappedTxCh[mappedChGrp])))
        {
            i = preferredChNum;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_MAPPED_TX_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->mappedTxChFlag[mappedChGrp][offset] & bitMask) == bitMask)
            {
                drvHandle->mappedTxChFlag[mappedChGrp][offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeMappedTxCh(uint32_t chNum,
                           Udma_DrvHandle drvHandle,
                           const uint32_t mappedChGrp)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= rmInitPrms->startMappedTxCh[mappedChGrp]);
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startMappedTxCh[mappedChGrp] + rmInitPrms->numMappedTxCh[mappedChGrp]));
    i = chNum - rmInitPrms->startMappedTxCh[mappedChGrp];
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_MAPPED_TX_CH_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (0U == (drvHandle->mappedTxChFlag[mappedChGrp][offset] & bitMask)));
    drvHandle->mappedTxChFlag[mappedChGrp][offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}
#endif

#if (UDMA_NUM_MAPPED_RX_GROUP > 0)
uint32_t Udma_rmAllocMappedRxCh(uint32_t preferredChNum,
                                Udma_DrvHandle drvHandle,
                                const uint32_t mappedChGrp)
{
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, (mappedChGrp < UDMA_NUM_MAPPED_RX_GROUP));

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from specific mapped RX channel pool */
        for(i = 0U; i < rmInitPrms->numMappedRxCh[mappedChGrp]; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_MAPPED_RX_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->mappedRxChFlag[mappedChGrp][offset] & bitMask) == bitMask)
            {
                drvHandle->mappedRxChFlag[mappedChGrp][offset] &= ~bitMask;
                chNum = i + rmInitPrms->startMappedRxCh[mappedChGrp];  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific mapped RX channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startMappedRxCh[mappedChGrp]) &&
           (preferredChNum < (rmInitPrms->startMappedRxCh[mappedChGrp] + rmInitPrms->numMappedRxCh[mappedChGrp])))
        {
            i = preferredChNum;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_MAPPED_RX_CH_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->mappedRxChFlag[mappedChGrp][offset] & bitMask) == bitMask)
            {
                drvHandle->mappedRxChFlag[mappedChGrp][offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeMappedRxCh(uint32_t chNum,
                           Udma_DrvHandle drvHandle,
                           const uint32_t mappedChGrp)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= rmInitPrms->startMappedRxCh[mappedChGrp]);
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startMappedRxCh[mappedChGrp] + rmInitPrms->numMappedRxCh[mappedChGrp]));
    i = chNum - rmInitPrms->startMappedRxCh[mappedChGrp];
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_MAPPED_RX_CH_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (0U == (drvHandle->mappedRxChFlag[mappedChGrp][offset] & bitMask)));
    drvHandle->mappedRxChFlag[mappedChGrp][offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}
#endif

#if((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
uint32_t Udma_rmAllocMappedRing(Udma_DrvHandle drvHandle,
                                const uint32_t mappedRingGrp,
                                const uint32_t mappedChNum)
{
    uint32_t    ringNum = UDMA_RING_INVALID;
    uint32_t    i,offset, bitPos, bitMask;
    uint32_t    loopStart, loopMax;
    int32_t     retVal = UDMA_SOK;

    Udma_RmInitPrms             *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
    Udma_MappedChRingAttributes  chAttr;

    Udma_assert(drvHandle, (mappedRingGrp < (UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP)));

    retVal = Udma_getMappedChRingAttributes(drvHandle, mappedRingGrp, mappedChNum, &chAttr);

    if((UDMA_SOK == retVal) && (0U != chAttr.numFreeRing))
    {
        /* Derive the intersecting pool (loopStart and loopMax) based on the rings reserved for the core (rmcfg)
        * and the permissible range for the given channel(free rings reserved for specific channels).
        *
        * Core_ring_Start (rmInitPrms->startMappedRing) & Core_ring_End (rmInitPrms->startMappedRing +rmInitPrms->numMappedRing)
        * refers to the range of reserved rings for the core.
        * Channel_ring_Start (chAttr->startFreeRing) & Channel_ring_End (chAttr->startFreeRing + chAttr->numFreeRing)
        * refers to permissible range of free rings for the particular channel.
        *
        * CASE 'A' refers to those that affects the loopStart
        * CASE 'B' refers to those that affects the loopMax
        */

        /* Default Loop Values*/
        loopStart = 0U;
        loopMax   = rmInitPrms->numMappedRing[mappedRingGrp];

        /* CASE_A_1 : Channel_ring_Start > Core_ring_Start */
        if(chAttr.startFreeRing > rmInitPrms->startMappedRing[mappedRingGrp])
        {
            /* Update loopStart to start from Channel_ring_Start,
            * so as to skip the starting rings which are reserved for the core,
            * but can't be used for the current channel */
            loopStart = chAttr.startFreeRing - rmInitPrms->startMappedRing[mappedRingGrp];
        }
        /* For all other CASE 'A's, loopStart should be 0 itself. */

        /* CASE_B_1 : Channel_ring_End < Core_ring_End */
        if((chAttr.startFreeRing + chAttr.numFreeRing) < (rmInitPrms->startMappedRing[mappedRingGrp] + rmInitPrms->numMappedRing[mappedRingGrp]))
        {
            /* Update loopMax to stop at Channel_ring_End,
            * so as to skip the last rings which are reserved for the core,
            * but can't be used for the current channel */
            Udma_assert(drvHandle, (chAttr.startFreeRing + chAttr.numFreeRing) >= rmInitPrms->startMappedRing[mappedRingGrp]);
            loopMax = (chAttr.startFreeRing + chAttr.numFreeRing) - rmInitPrms->startMappedRing[mappedRingGrp];
        }
        /* For all other CASE 'B's, loopMax should be rmInitPrms->numMappedRing[mappedRingGrp] itself. */

        Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
        drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

        /* Search and allocate from derived intersecting pool */
        for(i = loopStart; i < loopMax; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_MAPPED_RING_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->mappedRingFlag[mappedRingGrp][offset] & bitMask) == bitMask)
            {
                drvHandle->mappedRingFlag[mappedRingGrp][offset] &= ~bitMask;
                ringNum = i + rmInitPrms->startMappedRing[mappedRingGrp];  /* Add start offset */
                break;
            }
        }

        Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
        drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);
    }

    return (ringNum);
}

void Udma_rmFreeMappedRing(uint32_t ringNum,
                           Udma_DrvHandle drvHandle,
                           const uint32_t mappedRingGrp,
                           const uint32_t mappedChNum)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
    Udma_MappedChRingAttributes  chAttr;
    int32_t     retVal = UDMA_SOK;

    Udma_assert(drvHandle, (mappedRingGrp < (UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP)));

    retVal = Udma_getMappedChRingAttributes(drvHandle, mappedRingGrp, mappedChNum, &chAttr);
    Udma_assert(drvHandle, UDMA_SOK == retVal);

    /* Free up only the free mapped ring - ignore default mapped ring */
    if(ringNum != chAttr.defaultRing)
    {
        Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
        drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

        Udma_assert(drvHandle, (ringNum >= rmInitPrms->startMappedRing[mappedRingGrp]));
        Udma_assert(drvHandle,
            ringNum < (rmInitPrms->startMappedRing[mappedRingGrp] + rmInitPrms->numMappedRing[mappedRingGrp]));
        i = ringNum - rmInitPrms->startMappedRing[mappedRingGrp];
        offset = i >> 5U;
        Udma_assert(drvHandle, (offset < UDMA_RM_MAPPED_RING_ARR_SIZE));
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        Udma_assert(drvHandle,
            (0U == (drvHandle->mappedRingFlag[mappedRingGrp][offset] & bitMask)));
        drvHandle->mappedRingFlag[mappedRingGrp][offset] |= bitMask;

        Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
        drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);
    }
    return;
}
#endif

uint16_t Udma_rmAllocFreeRing(Udma_DrvHandle drvHandle)
{
    uint16_t            ringNum = UDMA_RING_INVALID;

    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        uint16_t            i, offset, temp;
        uint32_t            bitPos, bitMask;
        Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

        Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
        drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

        for(i = 0U; i < rmInitPrms->numFreeRing; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_FREE_RING_ARR_SIZE));
            temp = i - (offset << 5U);
            bitPos = (uint32_t) temp;
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->freeRingFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->freeRingFlag[offset] &= ~bitMask;
                ringNum = (uint16_t)rmInitPrms->startFreeRing;  /* Add start offset */
                ringNum += i;
                ringNum += (uint16_t)(drvHandle->udmapRegs.txChanCnt +
                                    drvHandle->udmapRegs.txExtUtcChanCnt +
                                    drvHandle->udmapRegs.rxChanCnt);
                break;
            }
        }

        Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
        drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);
#endif
    }
    return (ringNum);
}

void Udma_rmFreeFreeRing(uint16_t ringNum, Udma_DrvHandle drvHandle)
{
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        uint32_t            i, offset, bitPos, bitMask;
        Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
        uint32_t            freeRingOffset = rmInitPrms->startFreeRing +
                                    drvHandle->udmapRegs.txChanCnt +
                                    drvHandle->udmapRegs.txExtUtcChanCnt +
                                    drvHandle->udmapRegs.rxChanCnt;

        /* Free up only the free ring - ignore FQ rings */
        if(ringNum >= freeRingOffset)
        {
            Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
            drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

            i = ringNum - freeRingOffset;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_FREE_RING_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            Udma_assert(drvHandle, (0U == (drvHandle->freeRingFlag[offset] & bitMask)));
            drvHandle->freeRingFlag[offset] |= bitMask;

            Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
            drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);
        }
#endif
    }

    return;
}

uint16_t Udma_rmAllocProxy(Udma_DrvHandle drvHandle)
{
    uint16_t            i, offset, proxyNum = UDMA_PROXY_INVALID, temp;
    uint32_t            bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    for(i = 0U; i < rmInitPrms->numProxy; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, (offset < UDMA_RM_PROXY_ARR_SIZE));
        temp = i - (offset << 5U);
        bitPos = (uint32_t) temp;
        bitMask = (uint32_t) 1U << bitPos;
        if((drvHandle->proxyFlag[offset] & bitMask) == bitMask)
        {
            drvHandle->proxyFlag[offset] &= ~bitMask;
            proxyNum = rmInitPrms->startProxy;  /* Add start offset */
            proxyNum += i;
            break;
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (proxyNum);
}

void Udma_rmFreeProxy(uint16_t proxyNum, Udma_DrvHandle drvHandle)
{
    uint16_t            i, offset, bitPos;
    uint32_t            bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    i = proxyNum - rmInitPrms->startProxy;
    offset = (i >> 5U);
    Udma_assert(drvHandle, (offset < UDMA_RM_PROXY_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle, (0U == (drvHandle->proxyFlag[offset] & bitMask)));
    drvHandle->proxyFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
} 

uint32_t Udma_rmAllocEvent(Udma_DrvHandle drvHandle)
{
    uint32_t            globalEvent = UDMA_EVENT_INVALID;
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    for(i = 0U; i < rmInitPrms->numGlobalEvent; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, (offset < UDMA_RM_GLOBAL_EVENT_ARR_SIZE));
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        if((drvHandle->globalEventFlag[offset] & bitMask) == bitMask)
        {
            drvHandle->globalEventFlag[offset] &= ~bitMask;
            globalEvent = i + rmInitPrms->startGlobalEvent;  /* Add start offset */
            break;
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (globalEvent);
}

void Udma_rmFreeEvent(uint32_t globalEvent, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle,
        globalEvent < (rmInitPrms->startGlobalEvent + rmInitPrms->numGlobalEvent));
    Udma_assert(drvHandle, globalEvent >= rmInitPrms->startGlobalEvent);

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    i = globalEvent - rmInitPrms->startGlobalEvent;
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_GLOBAL_EVENT_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle, (0U == (drvHandle->globalEventFlag[offset] & bitMask)));
    drvHandle->globalEventFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocVintr(Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            vintrNum = UDMA_EVENT_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    for(i = 0U; i < rmInitPrms->numVintr; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, (offset < UDMA_RM_VINTR_ARR_SIZE));
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        if((drvHandle->vintrFlag[offset] & bitMask) == bitMask)
        {
            drvHandle->vintrFlag[offset] &= ~bitMask;
            vintrNum = i + rmInitPrms->startVintr;  /* Add start offset */
            break;
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (vintrNum);
}

void Udma_rmFreeVintr(uint32_t vintrNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle,
        vintrNum < (rmInitPrms->startVintr + rmInitPrms->numVintr));
    Udma_assert(drvHandle, vintrNum >= rmInitPrms->startVintr);

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    i = vintrNum - rmInitPrms->startVintr;
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_VINTR_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle, (0U == (drvHandle->vintrFlag[offset] & bitMask)));
    drvHandle->vintrFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocVintrBit(Udma_EventHandle eventHandle)
{
    uint32_t                i;
    uint32_t                vintrBitNum = UDMA_EVENT_INVALID;
    uint64_t                bitMask;
    Udma_EventHandle        masterEventHandle;
    const Udma_EventPrms   *eventPrms;
    Udma_DrvHandle          drvHandle = eventHandle->drvHandle;

    masterEventHandle = eventHandle;
    eventPrms = &eventHandle->eventPrms;
    if(NULL_PTR != eventPrms->masterEventHandle)
    {
        /* Shared event. Get the master handle */
        masterEventHandle = eventPrms->masterEventHandle;
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    for(i = 0U; i < UDMA_MAX_EVENTS_PER_VINTR; i++)
    {
        bitMask = ((uint64_t) 1U << i);
        if(0U == (masterEventHandle->vintrBitAllocFlag & bitMask))
        {
            masterEventHandle->vintrBitAllocFlag |= bitMask;
            vintrBitNum = i;
            break;
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (vintrBitNum);
}

void Udma_rmFreeVintrBit(uint32_t vintrBitNum,
                         Udma_DrvHandle drvHandle,
                         Udma_EventHandle eventHandle)
{
    uint64_t                bitMask;
    Udma_EventHandle        masterEventHandle;
    const Udma_EventPrms   *eventPrms;

    masterEventHandle = eventHandle;
    eventPrms = &eventHandle->eventPrms;
    if(NULL_PTR != eventPrms->masterEventHandle)
    {
        /* Shared event. Get the master handle */
        masterEventHandle = eventPrms->masterEventHandle;
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, (vintrBitNum < UDMA_MAX_EVENTS_PER_VINTR));
    bitMask = ((uint64_t) 1U << vintrBitNum);
    Udma_assert(drvHandle,
        (masterEventHandle->vintrBitAllocFlag & bitMask) == bitMask);
    masterEventHandle->vintrBitAllocFlag &= ~bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocIrIntr(uint32_t preferredIrIntrNum,
                              Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            irIntrNum = UDMA_INTR_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_CORE_INTR_ANY == preferredIrIntrNum)
    {
        /* Search and allocate from pool */
        for(i = 0U; i < rmInitPrms->numIrIntr; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_IR_INTR_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->irIntrFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->irIntrFlag[offset] &= ~bitMask;
                irIntrNum = i + rmInitPrms->startIrIntr;    /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific IR interrupt number if free */
        /* Array bound check */
        if((preferredIrIntrNum >= rmInitPrms->startIrIntr) &&
           (preferredIrIntrNum < (rmInitPrms->startIrIntr + rmInitPrms->numIrIntr)))
        {
            i = preferredIrIntrNum - rmInitPrms->startIrIntr;
            offset = i >> 5U;
            Udma_assert(drvHandle, (offset < UDMA_RM_IR_INTR_ARR_SIZE));
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->irIntrFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->irIntrFlag[offset] &= ~bitMask;
                irIntrNum = preferredIrIntrNum;
            }
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (irIntrNum);
}

void Udma_rmFreeIrIntr(uint32_t irIntrNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle,
        irIntrNum < (rmInitPrms->startIrIntr + rmInitPrms->numIrIntr));
    Udma_assert(drvHandle, irIntrNum >= rmInitPrms->startIrIntr);

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    i = irIntrNum - rmInitPrms->startIrIntr;
    offset = i >> 5U;
    Udma_assert(drvHandle, (offset < UDMA_RM_IR_INTR_ARR_SIZE));
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle, (0U == (drvHandle->irIntrFlag[offset] & bitMask)));
    drvHandle->irIntrFlag[offset] |= bitMask;

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint16_t Udma_rmAllocRingMon(Udma_DrvHandle drvHandle)
{
    uint16_t            i, offset, ringMonNum = UDMA_RING_MON_INVALID, temp;
    uint32_t            bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    for(i = 0U; i < rmInitPrms->numRingMon; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, (offset < UDMA_RM_RING_MON_ARR_SIZE));
        temp = i - (offset << 5U);
        bitPos = (uint32_t) temp;
        bitMask = (uint32_t) 1U << bitPos;
        if((drvHandle->ringMonFlag[offset] & bitMask) == bitMask)
        {
            drvHandle->ringMonFlag[offset] &= ~bitMask;
            ringMonNum = (uint16_t)rmInitPrms->startRingMon;  /* Add start offset */
            ringMonNum += i;
            break;
        }
    }

    Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (ringMonNum);
}

void Udma_rmFreeRingMon(uint16_t ringMonNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
    uint32_t            ringMonOffset = rmInitPrms->startRingMon;

    if(ringMonNum >= ringMonOffset)
    {
        Udma_assert(drvHandle, ((Udma_OsalMutexLockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.lockMutex));
        drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

        i = ringMonNum - ringMonOffset;
        offset = i >> 5U;
        Udma_assert(drvHandle, (offset < UDMA_RM_RING_MON_ARR_SIZE));
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        Udma_assert(drvHandle, (0U == (drvHandle->ringMonFlag[offset] & bitMask)));
        drvHandle->ringMonFlag[offset] |= bitMask;

        Udma_assert(drvHandle, ((Udma_OsalMutexUnlockFxn) NULL_PTR != drvHandle->initPrms.osalPrms.unlockMutex));
        drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);
    }

    return;
}

uint32_t Udma_rmTranslateIrOutput(Udma_DrvHandle drvHandle, uint32_t irIntrNum)
{
    uint32_t    coreIntrNum = UDMA_INTR_INVALID;

/* If Interrupt Router is present */
#if (UDMA_SOC_CFG_INTR_ROUTER_PRESENT == 1)
#if defined (_TMS320C6X)
        /* For C66x Sciclient translates NAVSS IR Idx to corresponding C66 IR Idx,
         * Not the Core Interrupt Idx. */
        Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

        coreIntrNum = irIntrNum - rmInitPrms->startIrIntr;
        coreIntrNum += rmInitPrms->startC66xCoreIntr;
#else
        int32_t  retVal = UDMA_SOK;
        uint16_t dstId = 0U;

        retVal = Sciclient_rmIrqTranslateIrOutput(drvHandle->devIdIr,
                                                (uint16_t)irIntrNum,
                                                drvHandle->devIdCore,
                                                &dstId);
        if(UDMA_SOK == retVal)
        {
            coreIntrNum = dstId;
        }
#endif
#else
        /* In case of devices like AM64x, where there are no Interrupt Routers,
         * irIntrNum refers to coreIntrNum number itself. */
        coreIntrNum = irIntrNum;
#endif

    return (coreIntrNum);
}

uint32_t Udma_rmTranslateCoreIntrInput(Udma_DrvHandle drvHandle, uint32_t coreIntrNum)
{
    uint32_t    irIntrNum = UDMA_INTR_INVALID;


#if (UDMA_SOC_CFG_INTR_ROUTER_PRESENT == 1)
#if defined (_TMS320C6X)
        /* For C66x Sciclient expects C66 IR Idx to translate to NAVSS IR Idx.
         * Not the Core Interrupt Idx. */
        Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

        irIntrNum = coreIntrNum - rmInitPrms->startC66xCoreIntr;
        irIntrNum += rmInitPrms->startIrIntr;
#else
        int32_t  retVal = UDMA_SOK;
        uint16_t irId = 0U;

        retVal = Sciclient_rmIrqTranslateIrqInput(drvHandle->devIdCore,
                                                  (uint16_t)coreIntrNum,
                                                  drvHandle->devIdIr,
                                                  &irId);
        if(UDMA_SOK == retVal)
        {
            irIntrNum = irId;
        }
#endif
#else
        /* In case of devices like AM64x, where there are no Interrupt Routers,
         * coreIntrNum refers to irIntrNum number itself. */
        irIntrNum = coreIntrNum;
#endif

    return (irIntrNum);
}

int32_t UdmaRmInitPrms_init(uint32_t instId, Udma_RmInitPrms *rmInitPrms)
{
    const Udma_RmDefBoardCfgPrms                *rmDefBoardCfgPrms;
    int32_t                                      retVal = UDMA_SOK;
    Udma_RmDefBoardCfgResp                       rmDefBoardCfgResp[UDMA_RM_DEFAULT_BOARDCFG_NUM_RES];
    uint32_t                                     splitResFlag[UDMA_RM_DEFAULT_BOARDCFG_NUM_RES] = {0U};
    uint32_t numRes = 0U;
    uint32_t resIdx;
    bool blkCopySubType = BFALSE;

    /* Error check */
    if(NULL_PTR == rmInitPrms)
    {
        retVal = UDMA_EBADARGS;
    }

    if(UDMA_SOK == retVal)
        {
            rmDefBoardCfgPrms = Udma_rmGetDefBoardCfgPrms(instId);

            memset(rmDefBoardCfgResp, 0, sizeof(rmDefBoardCfgResp));
            memset(rmInitPrms, 0, sizeof(*rmInitPrms));

        #if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)

            uint32_t numtTxCh = 0U;
            uint32_t numRxCh  = 0U;
            uint32_t numExtCh = 0U;

            if(UDMA_INST_ID_MCU_0 == instId)
            {
                numRes         = UDMA_RM_DEFAULT_BOARDCFG_NUM_RES;
                blkCopySubType = BFALSE;
                /* Assign offset Params */
                numtTxCh    = CSL_NAVSS_MCU_UDMAP_NUM_TX_CHANS;
                numRxCh     = CSL_NAVSS_MCU_UDMAP_NUM_RX_CHANS;
                numExtCh    = CSL_NAVSS_MCU_UDMAP_NUM_EXT_CHANS;
            }
            else if(UDMA_INST_ID_MAIN_0 == instId)
            {
                numRes         = UDMA_RM_DEFAULT_BOARDCFG_NUM_RES;
                blkCopySubType = BFALSE;
                /* Assign offset Params */
                numtTxCh    = CSL_NAVSS_MAIN_UDMAP_NUM_TX_CHANS;
                numRxCh     = CSL_NAVSS_MAIN_UDMAP_NUM_RX_CHANS;
                numExtCh    = CSL_NAVSS_MAIN_UDMAP_NUM_EXT_CHANS;
            }
            else
        #endif
        #if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
            if(UDMA_INST_ID_BCDMA_0 == instId)
            {
                blkCopySubType = BTRUE;
                numRes = UDMA_RM_NUM_BCDMA_RES;
            }
            else
        #endif
        #if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
            if(UDMA_INST_ID_PKTDMA_0 == instId)
            {
                blkCopySubType = BTRUE;
                numRes = UDMA_RM_NUM_PKTDMA_RES;
            }
            else
        #endif
            {
                retVal = UDMA_EFAIL;
            }
          for(resIdx = 0U; resIdx < numRes; resIdx++)
          {
              /* Query all the resources range from Sciclient Default BoardCfg */
              retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[resIdx], &rmDefBoardCfgResp[resIdx], &splitResFlag[resIdx]);
          }

      #if (UDMA_SOC_CFG_BCDMA_PRESENT == 1) || (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
          if(blkCopySubType)
          {
              /* Ultra High Capacity Block Copy Channels */
              rmInitPrms->startBlkCopyUhcCh   = rmDefBoardCfgResp[UDMA_RM_RES_ID_BC_UHC].rangeStart;
              rmInitPrms->numBlkCopyUhcCh     = rmDefBoardCfgResp[UDMA_RM_RES_ID_BC_UHC].rangeNum;

              /* High Capacity Block Copy Channels */
              rmInitPrms->startBlkCopyHcCh    = rmDefBoardCfgResp[UDMA_RM_RES_ID_BC_HC].rangeStart;
              rmInitPrms->numBlkCopyHcCh      = rmDefBoardCfgResp[UDMA_RM_RES_ID_BC_HC].rangeNum;

              /* Normal Capacity Block Copy Channels */
              rmInitPrms->startBlkCopyCh      = rmDefBoardCfgResp[UDMA_RM_RES_ID_BC].rangeStart;
              rmInitPrms->numBlkCopyCh        = rmDefBoardCfgResp[UDMA_RM_RES_ID_BC].rangeNum;

              /* Ultra High Capacity TX Channels */
              rmInitPrms->startTxUhcCh        = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX_UHC].rangeStart;
              rmInitPrms->numTxUhcCh          = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX_UHC].rangeNum;

              /* High Capacity TX Channels */
              rmInitPrms->startTxHcCh         = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX_HC].rangeStart;
              rmInitPrms->numTxHcCh           = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX_HC].rangeNum;

              /* Normal Capacity TX Channels */
              rmInitPrms->startTxCh           = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX].rangeStart;
              rmInitPrms->numTxCh             = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX].rangeNum;

              /* Ultra High Capacity RX Channels */
              rmInitPrms->startRxUhcCh        = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX_UHC].rangeStart;
              rmInitPrms->numRxUhcCh          = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX_UHC].rangeNum;

               /* High Capacity RX Channels */
              rmInitPrms->startRxHcCh         = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX_HC].rangeStart;
              rmInitPrms->numRxHcCh           = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX_HC].rangeNum;

              /* Normal Capacity RX Channels */
              rmInitPrms->startRxCh           = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX].rangeStart;
              rmInitPrms->numRxCh             = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX].rangeNum;
          }
      #endif
      #if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
          if(!blkCopySubType)
          {
              /* Ultra High Capacity Block Copy and TX Channels */
              /* Primary for Block Copy Channel */
              rmInitPrms->startBlkCopyUhcCh   = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX_UHC].rangeStart;
              rmInitPrms->numBlkCopyUhcCh     = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX_UHC].rangeNum;
               /* Secondary for TX Channel */
              rmInitPrms->startTxUhcCh        = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX_UHC].rangeStartSec;
              rmInitPrms->numTxUhcCh          = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX_UHC].rangeNumSec;

              /* High Capacity Block Copy and TX Channels */
              /* Primary for Block Copy Channel */
              rmInitPrms->startBlkCopyHcCh    = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX_HC].rangeStart;
              rmInitPrms->numBlkCopyHcCh      = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX_HC].rangeNum;
               /* Secondary for TX Channel */
              rmInitPrms->startTxHcCh         = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX_HC].rangeStartSec;
              rmInitPrms->numTxHcCh           = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX_HC].rangeNumSec;

              /* Normal Capacity Block Copy and TX Channels */
              /* Primary for Block Copy Channel */
              rmInitPrms->startBlkCopyCh      = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX].rangeStart;
              rmInitPrms->numBlkCopyCh        = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX].rangeNum;
              /* Secondary for TX Channel */
              rmInitPrms->startTxCh           = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX].rangeStartSec;
              rmInitPrms->numTxCh             = rmDefBoardCfgResp[UDMA_RM_RES_ID_TX].rangeNumSec;

              /* Ultra High Capacity RX Channels */
              /* Secondary for RX Channel (Primary for Block Copy Channel) */
              rmInitPrms->startRxUhcCh        = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX_UHC].rangeStartSec;
              rmInitPrms->numRxUhcCh          = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX_UHC].rangeNumSec;

              /* High Capacity RX Channels */
              /* Secondary for RX Channel (Primary for Block Copy Channel) */
              rmInitPrms->startRxHcCh         = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX_HC].rangeStartSec;
              rmInitPrms->numRxHcCh           = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX_HC].rangeNumSec;

              /* Normal Capacity RX Channels */
              /* Secondary for RX Channel (Primary for Block Copy Channel) */
              rmInitPrms->startRxCh           = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX].rangeStartSec;
              rmInitPrms->numRxCh             = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX].rangeNumSec;
          }
      #endif

      #if (UDMA_NUM_UTC_INSTANCE > 0)
          /* UTC - Extended Channels (MSMC_DRU/VPAC_TC0/VPAC_TC1/DMPAC) */
          /* Primary for MSMC_DRU Channel */
          rmInitPrms->startUtcCh[UDMA_UTC_ID_MSMC_DRU0]       = rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeStart;
          rmInitPrms->numUtcCh[UDMA_UTC_ID_MSMC_DRU0]         = rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeNum;
          /* Sub offset. TODO: Remove offset if feasible */
          if(rmInitPrms->startUtcCh[UDMA_UTC_ID_MSMC_DRU0] >= numtTxCh)
          {
              rmInitPrms->startUtcCh[UDMA_UTC_ID_MSMC_DRU0]  -= numtTxCh;
          }
      #if (UDMA_NUM_UTC_INSTANCE > 1)
          /* Secondary for VPAC_TC0+VPAC_TC1+DMPAC Channels */
          /* Here the assumption taken to split the external channels for HWA is that,
           * all similar type of channels (VPAC_TC0/VPAC_TC1/DMPAC) will be reserved to same core */
          uint32_t vpac0Start = CSL_NAVSS_MAIN_UDMAP_NUM_TX_CHANS + UDMA_UTC_START_CH_VPAC_TC0;
          uint32_t vpac1Start = CSL_NAVSS_MAIN_UDMAP_NUM_TX_CHANS + UDMA_UTC_START_CH_VPAC_TC1;
      #if (UDMA_SOC_CFG_VPAC1_PRESENT == 1)
          uint32_t vpac10Start = CSL_NAVSS_MAIN_UDMAP_NUM_TX_CHANS + UDMA_UTC_START_CH_VPAC1_TC0;
          uint32_t vpac11Start = CSL_NAVSS_MAIN_UDMAP_NUM_TX_CHANS + UDMA_UTC_START_CH_VPAC1_TC1;
      #endif
          uint32_t dmpacStart = CSL_NAVSS_MAIN_UDMAP_NUM_TX_CHANS + UDMA_UTC_START_CH_DMPAC_TC0;

          if((rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeStartSec == vpac0Start) &&
             ((rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeStartSec + rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeNumSec) >= (vpac0Start + UDMA_UTC_NUM_CH_VPAC_TC0)))
          {
              rmInitPrms->startUtcCh[UDMA_UTC_ID_VPAC_TC0]    = UDMA_UTC_START_CH_VPAC_TC0;
              rmInitPrms->numUtcCh[UDMA_UTC_ID_VPAC_TC0]      = UDMA_UTC_NUM_CH_VPAC_TC0;
          }
          if((rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeStartSec <= vpac1Start) &&
             ((rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeStartSec + rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeNumSec) >= (vpac1Start + UDMA_UTC_NUM_CH_VPAC_TC1)))
          {
              rmInitPrms->startUtcCh[UDMA_UTC_ID_VPAC_TC1]    = UDMA_UTC_START_CH_VPAC_TC1;
              rmInitPrms->numUtcCh[UDMA_UTC_ID_VPAC_TC1]      = UDMA_UTC_NUM_CH_VPAC_TC1;
          }
      #if (UDMA_SOC_CFG_VPAC1_PRESENT == 1)
          if((rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeStartSec <= vpac10Start) &&
             ((rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeStartSec + rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeNumSec) >= (vpac10Start + UDMA_UTC_NUM_CH_VPAC1_TC0)))
          {
              rmInitPrms->startUtcCh[UDMA_UTC_ID_VPAC1_TC0]    = UDMA_UTC_START_CH_VPAC1_TC0;
              rmInitPrms->numUtcCh[UDMA_UTC_ID_VPAC1_TC0]      = UDMA_UTC_NUM_CH_VPAC1_TC0;
          }
          if((rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeStartSec <= vpac11Start) &&
             ((rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeStartSec + rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeNumSec) >= (vpac11Start + UDMA_UTC_NUM_CH_VPAC1_TC1)))
          {
              rmInitPrms->startUtcCh[UDMA_UTC_ID_VPAC1_TC1]    = UDMA_UTC_START_CH_VPAC1_TC1;
              rmInitPrms->numUtcCh[UDMA_UTC_ID_VPAC1_TC1]      = UDMA_UTC_NUM_CH_VPAC1_TC1;
          }
      #endif
          if((rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeStartSec <= dmpacStart) &&
             ((rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeStartSec + rmDefBoardCfgResp[UDMA_RM_RES_ID_UTC].rangeNumSec) == (dmpacStart + UDMA_UTC_NUM_CH_DMPAC_TC0)))
          {
              rmInitPrms->startUtcCh[UDMA_UTC_ID_DMPAC_TC0]   = UDMA_UTC_START_CH_DMPAC_TC0;
              rmInitPrms->numUtcCh[UDMA_UTC_ID_DMPAC_TC0]     = UDMA_UTC_NUM_CH_DMPAC_TC0;
          }
      #endif
      #if defined (BUILD_C7X) && (UDMA_LOCAL_C7X_DRU_PRESENT == 1)
          uint32_t utcId, rmId;
          for(rmId = UDMA_RM_START_C7X_DRU ; rmId <= UDMA_RM_MAX_C7X_DRU ; rmId++)
          {
              /**
               * RM ids and UTC ids for these DRUs are continuous
               * To obtain UTC id get offset from RM id for particular DRU
               * Add the offset to DRU Start.
               */
              utcId    = (rmId - UDMA_RM_START_C7X_DRU) + UDMA_LOCAL_UTC_START;

              const Udma_RmDefBoardCfgResp *resp = Udma_rmGetLocalBoardCfgResp(rmId);

              if(resp != NULL_PTR)
              {
                  retVal += Udma_rmSetSharedResRmInitPrms(Udma_rmGetSharedResPrms(rmId),
                                                          Udma_getCoreId(),
                                                          UDMA_CORE_ID_C7X_1,
                                                          resp->rangeStart,
                                                          resp->rangeNum,
                                                          &rmInitPrms->startUtcCh[utcId],
                                                          &rmInitPrms->numUtcCh[utcId]);
              }
              else
              {
                  retVal = UDMA_EFAIL;
              }
          }
      #endif
      #endif

      #if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
          /* Free Flows */
          if(0U == splitResFlag[UDMA_RM_RES_ID_RX_FLOW])
          {
              rmInitPrms->startFreeFlow                           = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX_FLOW].rangeStart;
              rmInitPrms->numFreeFlow                             = rmDefBoardCfgResp[UDMA_RM_RES_ID_RX_FLOW].rangeNum;
          }
          else
          {
              retVal += Udma_rmSetSharedResRmInitPrms(Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_RX_FLOW),
                                                      Udma_getCoreId(),
                                                      0U,
                                                      rmDefBoardCfgResp[UDMA_RM_RES_ID_RX_FLOW].rangeStart,
                                                      rmDefBoardCfgResp[UDMA_RM_RES_ID_RX_FLOW].rangeNum,
                                                      &rmInitPrms->startFreeFlow,
                                                      &rmInitPrms->numFreeFlow);
          }
          /* Sub offset. TODO: Remove offset if feasible */
          if(rmInitPrms->startFreeFlow >= numRxCh)
          {
              rmInitPrms->startFreeFlow                      -= numRxCh;
          }

          /* Free Rings */
          rmInitPrms->startFreeRing                           = rmDefBoardCfgResp[UDMA_RM_RES_ID_RING].rangeStart;
          rmInitPrms->numFreeRing                             = rmDefBoardCfgResp[UDMA_RM_RES_ID_RING].rangeNum;
          /* Sub offset. TODO: Remove offset if feasible */
          if(rmInitPrms->startFreeRing >= (numtTxCh + numRxCh + numExtCh))
          {
              rmInitPrms->startFreeRing                      -= (numtTxCh + numRxCh + numExtCh);
          }
      #endif

          /* Global Event */
          /* Shared resource - Split based on instance */
          retVal += Udma_rmSetSharedResRmInitPrms(Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_GLOBAL_EVENT),
                                                 instId,
                                                 UDMA_INST_ID_START,
                                                 rmDefBoardCfgResp[UDMA_RM_RES_ID_GLOBAL_EVENT].rangeStart,
                                                 rmDefBoardCfgResp[UDMA_RM_RES_ID_GLOBAL_EVENT].rangeNum,
                                                 &rmInitPrms->startGlobalEvent,
                                                 &rmInitPrms->numGlobalEvent);
          if(0U != rmInitPrms->numGlobalEvent)
          {
              uint32_t offset = Udma_getGlobalEventOffset();
              /* Substract Offset for Global Events */
              if(rmInitPrms->startGlobalEvent >= offset)
              {
                  rmInitPrms->startGlobalEvent               -=  offset;
              }
              else
              {
                  retVal = UDMA_EFAIL;
              }
          }

          /* Virtual Interrupts */
          /* Shared resource - Split based on instance */
          retVal += Udma_rmSetSharedResRmInitPrms(Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_VINTR),
                                                 instId,
                                                 UDMA_INST_ID_START,
                                                 rmDefBoardCfgResp[UDMA_RM_RES_ID_VINTR].rangeStart,
                                                 rmDefBoardCfgResp[UDMA_RM_RES_ID_VINTR].rangeNum,
                                                 &rmInitPrms->startVintr,
                                                 &rmInitPrms->numVintr);

          /* Interrupt Router Interrupts */
  #if defined (BUILD_C7X)
          /* Interrupts reserved for other drivers */
          uint32_t totalResrvIntr = 0U;
          /* Interrupts available for UDMA driver */
          uint32_t numIrIntrAvbl  = 0U;

          Udma_RmSharedResPrms *rmSharedResPrms = Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_IR_INTR);
          /*
           * startResrvCnt and endResrvCnt are number of interrupts reserved for other
           * drivers, can't be used by UDMA driver. \ref Udma_RmSharedResPrms
           */
          if(NULL_PTR != rmSharedResPrms)
          {
              totalResrvIntr  = (rmSharedResPrms->startResrvCnt + rmSharedResPrms->endResrvCnt);
              numIrIntrAvbl   = (rmDefBoardCfgResp[UDMA_RM_RES_ID_IR_INTR].rangeNum - totalResrvIntr);

              /* Make sure that number of interrupts never execeeds number of C7x events for UDMA driver */
              if(numIrIntrAvbl > UDMA_C7X_CORE_NUM_INTR)
              {
                  rmDefBoardCfgResp[UDMA_RM_RES_ID_IR_INTR].rangeNum = (uint16_t)(totalResrvIntr + UDMA_C7X_CORE_NUM_INTR);
              }
          }
          else
          {
              retVal = UDMA_EFAIL;
          }
  #endif
      #if (UDMA_SOC_CFG_INTR_ROUTER_PRESENT == 1)
          /* Shared resource - Split based on instance */
          retVal += Udma_rmSetSharedResRmInitPrms(Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_IR_INTR),
                                              instId,
                                              UDMA_INST_ID_START,
                                              rmDefBoardCfgResp[UDMA_RM_RES_ID_IR_INTR].rangeStart,
                                              rmDefBoardCfgResp[UDMA_RM_RES_ID_IR_INTR].rangeNum,
                                              &rmInitPrms->startIrIntr,
                                              &rmInitPrms->numIrIntr);
      #else
          /* One to one mapping exists from Virtual Interrupts.
              * So translate to corresponding range.
              * In case of devices like AM64x, where there are no Interrupt Routers,
              * this refers to core interrupt itslef. */
          retVal += Sciclient_rmIrqTranslateIaOutput(rmDefBoardCfgPrms[UDMA_RM_RES_ID_VINTR].sciclientReqType,
                                                      rmInitPrms->startVintr,
                                                      Udma_getCoreSciDevId(),
                                                      (uint16_t *) &rmInitPrms->startIrIntr);

          rmInitPrms->numIrIntr                          = rmInitPrms->numVintr;
      #endif

      #if defined (BUILD_C7X) || defined (_TMS320C6X)
      #if defined (_TMS320C6X)
          /* Start C6xx Core Interrupt */
          rmInitPrms->startC66xCoreIntr                       = UDMA_C66X_CORE_INTR_OFFSET;
      #endif
          if(UDMA_INST_ID_START != instId)
          {
              uint32_t curInstIrStart     = 0U;
              uint32_t startInstIrStart   = 0U;
              /* Returned num value not used in this case, Passing to avoid dereferencing of NULL pointer */
              uint32_t num        = 0U;

              /* Get the startInstIrStart i.e., start value of range of IR interrupts
                 allocated to first instance */
              retVal += Udma_rmSetSharedResRmInitPrms(Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_IR_INTR),
                                                     UDMA_INST_ID_START,
                                                     UDMA_INST_ID_START,
                                                     rmDefBoardCfgResp[UDMA_RM_RES_ID_IR_INTR].rangeStart,
                                                     rmDefBoardCfgResp[UDMA_RM_RES_ID_IR_INTR].rangeNum,
                                                     &startInstIrStart,
                                                     &num);

              /* Get the curInstIrStart i.e., start value of range of IR interrupts
                 allocated to current instance */
              retVal += Udma_rmSetSharedResRmInitPrms(Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_IR_INTR),
                                                     instId,
                                                     UDMA_INST_ID_START,
                                                     rmDefBoardCfgResp[UDMA_RM_RES_ID_IR_INTR].rangeStart,
                                                     rmDefBoardCfgResp[UDMA_RM_RES_ID_IR_INTR].rangeNum,
                                                     &curInstIrStart,
                                                     &num);
              /* Add the no. of IR Interrupts reserved for C7x/C66x before current instance
                 to make sure each instance has their own range C7x/C66x events */
          #if defined (BUILD_C7X)
              /* Start C7x Core Interrupt */
              rmInitPrms->startC7xCoreIntr                   =  (UDMA_C7X_CORE_INTR_OFFSET + (curInstIrStart - startInstIrStart));
          #endif
          #if defined (_TMS320C6X)
              rmInitPrms->startC66xCoreIntr                  +=  (curInstIrStart - startInstIrStart);
          #endif
          }
      #endif

      #if (UDMA_SOC_CFG_PROXY_PRESENT == 1)
          /* Proxy */
          if(0U != rmDefBoardCfgResp[UDMA_RM_RES_ID_PROXY].rangeNum)
          {
              rmInitPrms->proxyThreadNum                      = rmDefBoardCfgResp[UDMA_RM_RES_ID_PROXY].rangeStart;
              rmInitPrms->startProxy                          = rmDefBoardCfgResp[UDMA_RM_RES_ID_PROXY].rangeStart + 1U;
              rmInitPrms->numProxy                            = rmDefBoardCfgResp[UDMA_RM_RES_ID_PROXY].rangeNum - 1U;
          }
      #endif

      #if (UDMA_SOC_CFG_RING_MON_PRESENT == 1)
          /* Ring Monitors */
          rmInitPrms->startRingMon                            = rmDefBoardCfgResp[UDMA_RM_RES_ID_RING_MON].rangeStart;
          rmInitPrms->numRingMon                              = rmDefBoardCfgResp[UDMA_RM_RES_ID_RING_MON].rangeNum;
      #endif

      #if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
          if(UDMA_INST_ID_PKTDMA_0 == instId)
          {
          #if (UDMA_NUM_MAPPED_TX_GROUP > 0)

              /* Mapped TX Channels for CPSW */
              rmInitPrms->startMappedTxCh[UDMA_MAPPED_TX_GROUP_CPSW]     = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_CPSW].rangeStart;
              rmInitPrms->numMappedTxCh[UDMA_MAPPED_TX_GROUP_CPSW]       = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_CPSW].rangeNum;

              /* Mapped TX Channels for SAUL */
              if(0U != rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_SAUL_0].rangeNum)
              {
                  rmInitPrms->startMappedTxCh[UDMA_MAPPED_TX_GROUP_SAUL] = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_SAUL_0].rangeStart;
              }
              else
              {
                  rmInitPrms->startMappedTxCh[UDMA_MAPPED_TX_GROUP_SAUL] = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_SAUL_1].rangeStart;
              }
              rmInitPrms->numMappedTxCh[UDMA_MAPPED_TX_GROUP_SAUL]       = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_SAUL_0].rangeNum +
                                                                           rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_SAUL_1].rangeNum;

              /* Mapped TX Channels for ICSSG_0 */
              rmInitPrms->startMappedTxCh[UDMA_MAPPED_TX_GROUP_ICSSG_0]  = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_ICSSG_0].rangeStart;
              rmInitPrms->numMappedTxCh[UDMA_MAPPED_TX_GROUP_ICSSG_0]    = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_ICSSG_0].rangeNum;

              /* Mapped TX Channels for ICSSG_1 */
              rmInitPrms->startMappedTxCh[UDMA_MAPPED_TX_GROUP_ICSSG_1]  = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_ICSSG_1].rangeStart;
              rmInitPrms->numMappedTxCh[UDMA_MAPPED_TX_GROUP_ICSSG_1]    = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_ICSSG_1].rangeNum;
          #endif
          #if (UDMA_NUM_MAPPED_RX_GROUP > 0)

              /* Mapped RX Channels for CPSW */
              rmInitPrms->startMappedRxCh[UDMA_MAPPED_RX_GROUP_CPSW - UDMA_NUM_MAPPED_TX_GROUP]     = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_CPSW].rangeStart;
              rmInitPrms->numMappedRxCh[UDMA_MAPPED_RX_GROUP_CPSW - UDMA_NUM_MAPPED_TX_GROUP]       = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_CPSW].rangeNum;

              /* Mapped RX Channels for SAUL */
              if(0U != rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_SAUL_0].rangeNum)
              {
                  rmInitPrms->startMappedRxCh[UDMA_MAPPED_RX_GROUP_SAUL - UDMA_NUM_MAPPED_TX_GROUP] = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_SAUL_0].rangeStart;
              }
              else
              {
                  rmInitPrms->startMappedRxCh[UDMA_MAPPED_RX_GROUP_SAUL - UDMA_NUM_MAPPED_TX_GROUP] = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_SAUL_2].rangeStart;
              }
              rmInitPrms->numMappedRxCh[UDMA_MAPPED_RX_GROUP_SAUL - UDMA_NUM_MAPPED_TX_GROUP]       = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_SAUL_0].rangeNum +
                                                                                                      rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_SAUL_1].rangeNum +
                                                                                                      rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_SAUL_2].rangeNum +
                                                                                                      rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_SAUL_3].rangeNum;


              /* Mapped RX Channels for ICSSG_0 */
              rmInitPrms->startMappedRxCh[UDMA_MAPPED_RX_GROUP_ICSSG_0 - UDMA_NUM_MAPPED_TX_GROUP]  = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_ICSSG_0].rangeStart;
              rmInitPrms->numMappedRxCh[UDMA_MAPPED_RX_GROUP_ICSSG_0 - UDMA_NUM_MAPPED_TX_GROUP]    = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_ICSSG_0].rangeNum;

              /* Mapped RX Channels for ICSSG_1 */
              rmInitPrms->startMappedRxCh[UDMA_MAPPED_RX_GROUP_ICSSG_1 - UDMA_NUM_MAPPED_TX_GROUP]  = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_ICSSG_1].rangeStart;
              rmInitPrms->numMappedRxCh[UDMA_MAPPED_RX_GROUP_ICSSG_1 - UDMA_NUM_MAPPED_TX_GROUP]    = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_ICSSG_1].rangeNum;
          #endif
          #if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)

              /* Mapped TX Rings for CPSW */
              rmInitPrms->startMappedRing[UDMA_MAPPED_TX_GROUP_CPSW]     = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_CPSW].rangeStart;
              rmInitPrms->numMappedRing[UDMA_MAPPED_TX_GROUP_CPSW]       = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_CPSW].rangeNum;

              /* Mapped TX Rings for SAUL */
            /*
              Temp disabled until SYSFW-4170 is properly fixed and Sysconfig Tool is updated.
              (SAUL_RX_0_CHAN,SAUL_RX_1_CHAN is reserved is SYSFW, But the corresponding RX Rings are not reserved.)

              if(0U != rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_SAUL_0].rangeNum)
              {
                  rmInitPrms->startMappedRing[UDMA_MAPPED_TX_GROUP_SAUL] = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_SAUL_0].rangeStart;
              }
              else
              {
                  rmInitPrms->startMappedRing[UDMA_MAPPED_TX_GROUP_SAUL] = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_SAUL_1].rangeStart;
              }
              rmInitPrms->numMappedRing[UDMA_MAPPED_TX_GROUP_SAUL]       = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_SAUL_0].rangeNum +
                                                                           rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_SAUL_1].rangeNum;
            */
              rmInitPrms->startMappedRing[UDMA_MAPPED_TX_GROUP_SAUL]     = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_SAUL_1].rangeStart;
              rmInitPrms->numMappedRing[UDMA_MAPPED_TX_GROUP_SAUL]       = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_SAUL_1].rangeNum;

              /* Mapped TX Rings for ICSSG_0 */
              rmInitPrms->startMappedRing[UDMA_MAPPED_TX_GROUP_ICSSG_0]  = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_ICSSG_0].rangeStart;
              rmInitPrms->numMappedRing[UDMA_MAPPED_TX_GROUP_ICSSG_0]    = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_ICSSG_0].rangeNum;

              /* Mapped TX Rings for ICSSG_1 */
              rmInitPrms->startMappedRing[UDMA_MAPPED_TX_GROUP_ICSSG_1]  = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_ICSSG_1].rangeStart;
              rmInitPrms->numMappedRing[UDMA_MAPPED_TX_GROUP_ICSSG_1]    = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_TX_RING_ICSSG_1].rangeNum;

              /* Mapped RX Rings for CPSW */
              rmInitPrms->startMappedRing[UDMA_MAPPED_RX_GROUP_CPSW]     = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_CPSW].rangeStart;
              rmInitPrms->numMappedRing[UDMA_MAPPED_RX_GROUP_CPSW]       = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_CPSW].rangeNum;

              /* Mapped RX Rings for SAUL */
            /*
              Temp disabled until SYSFW-4170 is properly fixed and Sysconfig Tool is updated.
              (SAUL_RX_0_CHAN,SAUL_RX_1_CHAN is reserved is SYSFW, But the corresponding RX Rings are not reserved.)

              if(0U != rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_SAUL_0].rangeNum)
              {
                  rmInitPrms->startMappedRing[UDMA_MAPPED_RX_GROUP_SAUL] = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_SAUL_0].rangeStart;
              }
              else
              {
                  rmInitPrms->startMappedRing[UDMA_MAPPED_RX_GROUP_SAUL] = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_SAUL_2].rangeStart;
              }
              rmInitPrms->numMappedRing[UDMA_MAPPED_RX_GROUP_SAUL]       = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_SAUL_0].rangeNum +
                                                                           rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_SAUL_2].rangeNum;
            */
              rmInitPrms->startMappedRing[UDMA_MAPPED_RX_GROUP_SAUL]     = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_SAUL_2].rangeStart;
              rmInitPrms->numMappedRing[UDMA_MAPPED_RX_GROUP_SAUL]       = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_SAUL_2].rangeNum;

              /* Mapped RX Rings for ICSSG_0 */
              rmInitPrms->startMappedRing[UDMA_MAPPED_RX_GROUP_ICSSG_0]  = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_ICSSG_0].rangeStart;
              rmInitPrms->numMappedRing[UDMA_MAPPED_RX_GROUP_ICSSG_0]    = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_ICSSG_0].rangeNum;

              /* Mapped RX Rings for ICSSG_1 */
              rmInitPrms->startMappedRing[UDMA_MAPPED_RX_GROUP_ICSSG_1]  = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_ICSSG_1].rangeStart;
              rmInitPrms->numMappedRing[UDMA_MAPPED_RX_GROUP_ICSSG_1]    = rmDefBoardCfgResp[UDMA_RM_RES_ID_MAPPED_RX_RING_ICSSG_1].rangeNum;
          #endif
          }
      #endif
      }
    return (retVal);
}

static int32_t Udma_rmGetSciclientDefaultBoardCfgRmRange(const Udma_RmDefBoardCfgPrms *rmDefBoardCfgPrms,
                                                         Udma_RmDefBoardCfgResp *rmDefBoardCfgResp,
                                                         uint32_t *splitResFlag)
{
    int32_t                                     retVal = UDMA_SOK;
    struct tisci_msg_rm_get_resource_range_req  req;
    struct tisci_msg_rm_get_resource_range_resp res;
    memset(&res, 0, sizeof(res));
    memset(&req, 0, sizeof(req));

    req.type           = rmDefBoardCfgPrms->sciclientReqType;
    req.subtype        = rmDefBoardCfgPrms->sciclientReqSubtype;
    req.secondary_host = rmDefBoardCfgPrms->sciclientSecHost;

    /* Skip for invalid type/subtype.
     * This is for the cases in which IP supports some type of resorces and
     * a particular SOC dosen't have any.
     * (Here, no TISCI define for those resource will be defined for the SOC)
     */
    if((UDMA_RM_SCI_REQ_TYPE_INVALID != req.type) && (UDMA_RM_SCI_REQ_SUBTYPE_INVALID != req.subtype))
    {
        /* Get resource number range */
        retVal =  Sciclient_rmGetResourceRange(
                    &req,
                    &res,
                    UDMA_SCICLIENT_TIMEOUT);
        if((CSL_PASS != retVal) ||
        ((0U == res.range_num) && (0U == res.range_num_sec)))
        {
            /* If range_num and range_num_sec = 0 (no entry for the core),
            * There is no reservation for the current core.
            * In this case, Try with HOST_ID_ALL*/
            req.secondary_host = TISCI_HOST_ID_ALL;

            retVal = Sciclient_rmGetResourceRange(
                        &req,
                        &res,
                        UDMA_SCICLIENT_TIMEOUT);
            if((CSL_PASS == retVal) && (0U != res.range_num))
            {
                /* If range_num != 0,
                * ie, When using TISCI_HOST_ID_ALL entry,
                * Set the split resource flag, if passed.
                * (no need to check for range_num_sec,
                *  since there will only be single entry for TISCI_HOST_ID_ALL) */
                if(NULL_PTR != splitResFlag)
                {
                    *splitResFlag = 1U;
                }
            }
        }
    }

    rmDefBoardCfgResp->resId         = rmDefBoardCfgPrms->resId;
    rmDefBoardCfgResp->rangeStart    = res.range_start;
    rmDefBoardCfgResp->rangeNum      = res.range_num;
    rmDefBoardCfgResp->rangeStartSec = res.range_start_sec;
    rmDefBoardCfgResp->rangeNumSec   = res.range_num_sec;

    return (retVal);
}

static int32_t Udma_rmSetSharedResRmInitPrms(const Udma_RmSharedResPrms *rmSharedResPrms,
                                             uint32_t  instId,
                                             uint32_t  startInstId,
                                             uint32_t  rangeStart,
                                             uint32_t  rangeTotalNum,
                                             uint32_t *start,
                                             uint32_t *num)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    i;
    uint32_t    sumInstShare = 0U;
    uint32_t    instFinalShare[UDMA_RM_SHARED_RES_MAX_INST]={0};
    uint32_t    splitCnt = 0U;
    uint32_t    splitShare;
    uint32_t    splitMod;
    uint32_t    udmaResvCnt;
    uint32_t    numInst;
    uint32_t    minReq;
    uint32_t    startResrvCnt;
    uint32_t    endResrvCnt;
    uint32_t    numUnresvRes;

    /* Error Check */
    if((NULL_PTR == rmSharedResPrms) ||
    (UDMA_CORE_ID_INVALID == instId) ||
    (UDMA_RM_SHARED_RES_MAX_INST <= (instId - startInstId)) ||
    (startInstId > instId))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        numInst = rmSharedResPrms->numInst;
        minReq = rmSharedResPrms->minReq;
        startResrvCnt = rmSharedResPrms->startResrvCnt;
        endResrvCnt = rmSharedResPrms->endResrvCnt;
        numUnresvRes = (rangeTotalNum > (startResrvCnt + endResrvCnt)) ?
                                    (rangeTotalNum - (startResrvCnt + endResrvCnt)) : 0U;

        /* Check for minimum requirement with total usable*/
        if((numInst > UDMA_RM_SHARED_RES_MAX_INST) ||
           (numUnresvRes < (numInst * minReq)))
        {
            retVal = UDMA_EBADARGS;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Populate share for each instance; Get sum of shares for validation */
        for(i = 0U;i < numInst; i++)
        {
            if(UDMA_RM_SHARED_RES_CNT_MIN == rmSharedResPrms->instShare[i])
            {
                sumInstShare += minReq;
                instFinalShare[i] = minReq;
            }
            else if(UDMA_RM_SHARED_RES_CNT_REST == rmSharedResPrms->instShare[i])
            {
                sumInstShare += minReq;
                splitCnt++;
            }
            else
            {
                sumInstShare += rmSharedResPrms->instShare[i];
                instFinalShare[i] = rmSharedResPrms->instShare[i];
            }
        }
        /* Check for sum of share with total usable */
        if(numUnresvRes < sumInstShare)
        {
            retVal = UDMA_EINVALID_PARAMS;
        }
    }

    if(UDMA_SOK == retVal)
    {
        if(splitCnt > 0U)
        {
            /* Populate for #UDMA_RM_SHARED_RES_CNT_REST with rest of the available resources */
            udmaResvCnt = sumInstShare - (splitCnt * minReq);
            splitShare = (numUnresvRes - udmaResvCnt) / splitCnt;
            splitMod = (numUnresvRes - udmaResvCnt) % splitCnt;
            for(i = 0U; i < numInst; i++)
            {
                if(UDMA_RM_SHARED_RES_CNT_REST == rmSharedResPrms->instShare[i])
                {
                    instFinalShare[i] = splitShare + splitMod;
                    splitMod = 0U;
                }
            }
        }

        *num = instFinalShare[instId - startInstId];
        /* Calculate the start for requested instance */
        *start = rangeStart + startResrvCnt;
        for(i = 0U; i < (instId - startInstId); i++)
        {
            *start += instFinalShare[i];
        }
    }

    return (retVal);
}

static int32_t Udma_rmCheckResLeak(Udma_DrvHandle drvHandle,
                                   const uint32_t *allocFlag,
                                   uint32_t numRes,
                                   uint32_t arrSize)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    i, offset, bitMask;

    offset = 0U;
    i = numRes;
    while(i > 0U)
    {
        if((uint32_t)32U <= i)          /* 32 resource per array element */
        {
            bitMask = (uint32_t) 0xFFFFFFFFU;
            i -= 32U;
        }
        else
        {
            bitMask = ((uint32_t)1U << i) - ((uint32_t)1U);
            i = 0U;
        }

        Udma_assert(drvHandle, offset < arrSize);
        if((allocFlag[offset] & bitMask) != bitMask)
        {
            retVal = UDMA_EFAIL;        /* Some resources not freed */
            break;
        }
        offset++;
    }

    return (retVal);
}
