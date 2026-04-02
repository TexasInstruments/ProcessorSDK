/*
 *  Copyright (c) Texas Instruments Incorporated 2024
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
 *  \file udma_test_ring_lcdma_neg.c
 *
 *  \brief File containing test cases for UDMA Ring Lcdma related APIs.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <udma_test.h>

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

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Test Case Description: Verifies the function Udma_ringGetModeLcdma when
 * 1)Test scenario 1: Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestRingGetModeLcdmaNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal   = UDMA_SOK;
    uint32_t            ringMode = CSL_LCDMA_RINGACC_RING_MODE_INVALID;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringGetModeLcdma negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle is NULL */
    ringHandle = (Udma_RingHandle) NULL_PTR;
    ringMode   = Udma_ringGetModeLcdma(ringHandle);
    if(CSL_LCDMA_RINGACC_RING_MODE_INVALID == ringMode)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|::FAIL:: UDMA:: ringGetModeLcdma :: Neg::"
                  " Check when ringHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(retVal == UDMA_SOK)
    {
        /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_DEINIT_DONE;
        ringMode                 = Udma_ringGetModeLcdma(ringHandle);
        if(CSL_LCDMA_RINGACC_RING_MODE_INVALID == ringMode)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: ringGetModeLcdma :: Neg::"
                      " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringGetElementCntLcdma when
 * 1)Test scenario 1: Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestRingGetElementCntLcdmaNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal         = UDMA_SOK;
    uint32_t            ringElementCnt = 0U;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringGetElementCntLcdma negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle is NULL */
    ringHandle     = (Udma_RingHandle) NULL_PTR;
    ringElementCnt = Udma_ringGetElementCntLcdma(ringHandle);
    if(0U == ringElementCnt)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|::FAIL:: UDMA:: ringGetElementCntLcdma :: Neg::"
                  " Check when ringHandle is NULL !!\n");
        retVal = UDMA_EFAIL;
    }

    if(retVal == UDMA_SOK)
    {
        /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_DEINIT_DONE;
        ringElementCnt           = Udma_ringGetElementCntLcdma(ringHandle);
        if(0U == ringElementCnt)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: ringGetElementCntLcdma :: Neg::"
                      " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringGetForwardRingOccLcdma when
 * 1)Test scenario 1: Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE .
 */
int32_t UdmaTestRingGetForwardRingOccLcdmaNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal     = UDMA_SOK;
    uint32_t            ringOccCnt = 0U;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringGetForwardRingOccLcdma negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle is NULL */
    ringHandle = (Udma_RingHandle) NULL_PTR;
    ringOccCnt = Udma_ringGetForwardRingOccLcdma(ringHandle);
    if(0U == ringOccCnt)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|::FAIL:: UDMA:: ringGetForwardRingOccLcdma :: Neg::"
                  " Check when ringHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(retVal == UDMA_SOK)
    {
        /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_DEINIT_DONE;
        ringOccCnt               = Udma_ringGetForwardRingOccLcdma(ringHandle);
        if(0U == ringOccCnt)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: ringGetForwardRingOccLcdma :: Neg::"
                      " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringGetReverseRingOccLcdma when
 * 1)Test scenario 1: Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestRingGetReverseRingOccLcdmaNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal     = UDMA_SOK;
    uint32_t            ringOccCnt = 0U;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma  negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle is NULL */
    ringHandle = (Udma_RingHandle) NULL_PTR;
    ringOccCnt = Udma_ringGetReverseRingOccLcdma(ringHandle);
    if(0U == ringOccCnt)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|::FAIL:: UDMA:: ringGetReverseRingOccLcdma :: Neg::"
                  " Check when ringHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(retVal == UDMA_SOK)
    {
        /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_DEINIT_DONE;
        ringOccCnt               = Udma_ringGetReverseRingOccLcdma(ringHandle);
        if(0U == ringOccCnt)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: ringGetReverseRingOccLcdma :: Neg::"
                      " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringGetWrIdxLcdma when
 * 1)Test scenario 1: Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestRingGetWrIdxLcdmaNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal    = UDMA_SOK;
    uint32_t            ringWrIdx = 0U;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringGetWrIdxLcdma negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle is NULL */
    ringHandle = (Udma_RingHandle) NULL_PTR;
    ringWrIdx  = Udma_ringGetWrIdxLcdma(ringHandle);
    if(0U == ringWrIdx)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|::FAIL:: UDMA:: ringGetWrIdxLcdma :: Neg::"
                  " Check when ringHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(retVal == UDMA_SOK)
    {
        /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_DEINIT_DONE;
        ringWrIdx                = Udma_ringGetWrIdxLcdma(ringHandle);
        if(0U == ringWrIdx)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: ringGetWrIdxLcdma :: Neg::"
                      " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringGetRdIdxLcdma when
 * 1)Test scenario 1: Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestRingGetRdIdxLcdmaNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal    = UDMA_SOK;
    uint32_t            ringRdIdx = 0U;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringGetRdIdxLcdma negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle is NULL */
    ringHandle = (Udma_RingHandle) NULL_PTR;
    ringRdIdx  = Udma_ringGetRdIdxLcdma(ringHandle);
    if(0U == ringRdIdx)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|::FAIL:: UDMA:: ringGetRdIdxLcdma :: Neg::"
                  " Check when ringHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(retVal == UDMA_SOK)
    {
        /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_DEINIT_DONE;
        ringRdIdx                = Udma_ringGetRdIdxLcdma(ringHandle);
        if(0U == ringRdIdx)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: ringGetRdIdxLcdma :: Neg::"
                      " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringGetMemPtrLcdma when
 * 1)Test scenario 1: Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestRingGetMemPtrLcdmaNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal   = UDMA_SOK;
    void                *pRingMem = NULL_PTR;
    struct Udma_RingObj  ringObj;
    Udma_RingHandle      ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringGetMemPtrLcdma negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle is NULL */
    ringHandle = (Udma_RingHandle) NULL_PTR;
    pRingMem   = Udma_ringGetMemPtrLcdma(ringHandle);
    if(NULL_PTR == pRingMem)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|::FAIL:: UDMA:: ringGetMemPtrLcdma :: Neg::"
                  " Check when ringHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
    ringHandle               = &ringObj;
    ringHandle->ringInitDone = UDMA_DEINIT_DONE;
    pRingMem                 = Udma_ringGetMemPtrLcdma(ringHandle);
    if(NULL_PTR == pRingMem)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|::FAIL:: UDMA:: ringGetMemPtrLcdma :: Neg::"
                  " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
        retVal = UDMA_EFAIL;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringSetCfgLcdma when
 * 1)Test scenario 1: Check when ringPrms is NULL.
 */
int32_t UdmaTestRingSetCfgLcdmaNeg(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal  = UDMA_SOK;
    uint32_t          elemCnt = 50U;
    uint32_t          heapId  = UTILS_MEM_HEAP_ID_MSMC;
    void             *ringMem = NULL;
    uint32_t          ringMemSize;
    Udma_ChHandle     chHandle;
    struct Udma_ChObj chObj;
    Udma_ChPrms       chPrms;
    uint32_t          chType;
    uint32_t          ringMode;
    Udma_ChTxPrms     txChPrms;
    Udma_DrvHandle    drvHandle;

    ringMode    = TISCI_MSG_VALUE_RM_RING_MODE_RING;
    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem     = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }
    chHandle                      = &chObj;
    chType                        = UDMA_CH_TYPE_TX;
    UdmaChPrms_init(&chPrms, chType);
    drvHandle                     = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];
    chPrms.peerChNum              = UDMA_PSIL_CH_MAIN_CSI_TX;
    chPrms.fqRingPrms.ringMem     = ringMem;
    chPrms.fqRingPrms.ringMemSize = ringMemSize;
    chPrms.fqRingPrms.elemCnt     = elemCnt;
    chPrms.fqRingPrms.mode        = ringMode;
    retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        UdmaChTxPrms_init(&txChPrms, chType);
        retVal = Udma_chConfigTx(chHandle, &txChPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(chHandle);
        }
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: UdmaringSetCfgLcdma negative Testcase ::\r\n",
                      taskObj->taskId);

            /* Test scenario 1: Check when ringPrms is NULL */
            Udma_ringSetCfgLcdma(drvHandle, chHandle->fqRing, NULL_PTR);
        }
        Udma_chDisable(chHandle, 0);
    }
    Udma_chClose(chHandle);

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringDequeueRawLcdma
 * 1)Test scenario 1: Check to get error UDMA_ETIMEOUT.
 */
int32_t UdmaTestRingDequeueRawLcdmaNeg(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal  = UDMA_SOK;
    uint32_t          elemCnt = 5U;
    uint32_t          heapId  = UTILS_MEM_HEAP_ID_MSMC;
    void             *ringMem = NULL;
    uint32_t          ringMemSize,qCnt;
    Udma_ChHandle     chHandle;
    struct Udma_ChObj chObj;
    Udma_ChPrms       chPrms;
    uint32_t          chType;
    uint32_t          ringMode;
    Udma_ChTxPrms     txChPrms;
    uint64_t          ringData;
    int32_t           status;
    Udma_DrvHandle    drvHandle;

    ringMode    = TISCI_MSG_VALUE_RM_RING_MODE_RING;
    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem     = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }
    chHandle                      = &chObj;
    chType                        = UDMA_CH_TYPE_TX;
    UdmaChPrms_init(&chPrms, chType);
    drvHandle                     = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];
    chPrms.peerChNum              = UDMA_PSIL_CH_MAIN_CSI_TX;
    chPrms.fqRingPrms.ringMem     = ringMem;
    chPrms.fqRingPrms.ringMemSize = ringMemSize;
    chPrms.fqRingPrms.elemCnt     = elemCnt;
    chPrms.fqRingPrms.mode        = ringMode;
    retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        UdmaChTxPrms_init(&txChPrms, chType);
        retVal = Udma_chConfigTx(chHandle, &txChPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(chHandle);
        }
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: Udma_ringDequeueRawLcdma negative Testcase ::\r\n",
                      taskObj->taskId);

            /* Test scenario 1: Check to get error UDMA_ETIMEOUT */
            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                status   = Udma_ringDequeueRawLcdma(drvHandle, chHandle->fqRing, &ringData);
                if(UDMA_SOK != status)
                {
                    retVal = UDMA_SOK;
                }
                else
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|::FAIL:: UDMA:: Udma_ringDequeueRawLcdma :: Neg::"
                              " Check to get error UDMA_ETIMEOUT!!\n");
                    retVal = UDMA_EFAIL;
                }
            }
        }
        Udma_chDisable(chHandle, 0);
    }
    Udma_chClose(chHandle);

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringFlushRawLcdma
 * 1)Test scenario 1: Check to get return as UDMA_ETIMEOUT from ring flush API.
 */
int32_t UdmaTestRingFlushRawLcdmaNeg(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal  = UDMA_SOK;
    uint32_t          elemCnt = 50U;
    uint32_t          heapId  = UTILS_MEM_HEAP_ID_MSMC;
    void             *ringMem = NULL;
    uint32_t          ringMemSize,qCnt;
    Udma_ChHandle     chHandle;
    struct Udma_ChObj chObj;
    Udma_ChPrms       chPrms;
    uint32_t          chType;
    uint32_t          ringMode;
    Udma_ChTxPrms     txChPrms;
    uint64_t          ringData;
    int32_t           status;
    Udma_DrvHandle    drvHandle;

    ringMode    = TISCI_MSG_VALUE_RM_RING_MODE_RING;
    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem     = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }
    chHandle                      = &chObj;
    chType                        = UDMA_CH_TYPE_TX;
    UdmaChPrms_init(&chPrms, chType);
    drvHandle                     = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];
    chPrms.peerChNum              = UDMA_PSIL_CH_MAIN_CSI_TX;
    chPrms.fqRingPrms.ringMem     = ringMem;
    chPrms.fqRingPrms.ringMemSize = ringMemSize;
    chPrms.fqRingPrms.elemCnt     = elemCnt;
    chPrms.fqRingPrms.mode        = ringMode;
    retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        UdmaChTxPrms_init(&txChPrms, chType);
        retVal = Udma_chConfigTx(chHandle, &txChPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(chHandle);
        }
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: Udma ringGetMemPtrLcdma negative Testcase ::\r\n",
                      taskObj->taskId);

            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                /* Queue the descriptors to the ring */
                ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                status   = Udma_ringQueueRawLcdma(drvHandle, chHandle->fqRing, ringData);
                if(UDMA_SOK == status)
                {
                    retVal = UDMA_SOK;
                }
                else
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|::FAIL:: UDMA:: Udma_ringQueueRawLcdma :: Neg::"
                              " Fail to queue the descriptors!!\n");
                     retVal = UDMA_EFAIL;
                }
            }

            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                /* Dequeue the descriptors from the ring */
                ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                status   = Udma_ringDequeueRawLcdma(drvHandle, chHandle->fqRing, &ringData);
                if(UDMA_SOK == status)
                {
                    retVal = UDMA_SOK;
                }
                else
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|::FAIL:: UDMA:: Udma_ringDequeueRawLcdma :: Neg::"
                              " Fail to dequeue the descriptors!!\n");
                    retVal = UDMA_EFAIL;
                }
            }

            /* Test scenario 1: Check to get return as UDMA_ETIMEOUT from ring flush API */
            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                status   = Udma_ringFlushRawLcdma(drvHandle, chHandle->fqRing, &ringData);
                if(UDMA_SOK != status)
                {
                    retVal = UDMA_SOK;
                }
                else
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|::FAIL:: UDMA:: Udma_ringFlushRawLcdma :: Neg::"
                              " Fail to Flush raw descriptors!!\n");
                    retVal = UDMA_EFAIL;
                }
            }
        }
        Udma_chDisable(chHandle, 0);
    }
    Udma_chClose(chHandle);

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringPrimeReadLcdma
 * 1)Test scenario 1: Check to get physical descriptor mermory pointer value as 0.
 */
int32_t UdmaTestRingPrimeReadLcdmaNeg(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal  = UDMA_SOK;
    uint32_t          elemCnt = 5U;
    uint32_t          heapId  = UTILS_MEM_HEAP_ID_MSMC;
    void             *ringMem = NULL;
    uint32_t          ringMemSize,qCnt;
    Udma_ChHandle     chHandle;
    struct Udma_ChObj chObj;
    Udma_ChPrms       chPrms;
    uint32_t          chType;
    uint32_t          ringMode;
    Udma_ChTxPrms     txChPrms;
    uint64_t          ringData;
    Udma_DrvHandle    drvHandle;

    ringMode    = TISCI_MSG_VALUE_RM_RING_MODE_RING;
    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem     = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }
    chHandle                      = &chObj;
    chType                        = UDMA_CH_TYPE_TX;
    UdmaChPrms_init(&chPrms, chType);
    drvHandle                     = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];
    chPrms.peerChNum              = UDMA_PSIL_CH_MAIN_CSI_TX;
    chPrms.fqRingPrms.ringMem     = ringMem;
    chPrms.fqRingPrms.ringMemSize = ringMemSize;
    chPrms.fqRingPrms.elemCnt     = elemCnt;
    chPrms.fqRingPrms.mode        = ringMode;
    retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        UdmaChTxPrms_init(&txChPrms, chType);
        retVal = Udma_chConfigTx(chHandle, &txChPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(chHandle);
        }
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: Udma_ringPrimeReadLcdma negative Testcase ::\r\n",
                      taskObj->taskId);

            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                /* Test scenario 1: Check to get physical descriptor mermory pointer value as 0 */
                ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                Udma_ringPrimeReadLcdma(chHandle->fqRing, &ringData);
            }
        }
        Udma_chDisable(chHandle, 0);
    }
    Udma_chClose(chHandle);

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringFlushRawLcdma
 * 1)Test scenario 1: Check to get Ring reset failed error when devIdRing is invalid
 */
int32_t UdmaTestRingFlushRawLcdmaCslNeg(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal  = UDMA_SOK;
    uint32_t          elemCnt = 5U;
    uint32_t          heapId  = UTILS_MEM_HEAP_ID_MSMC;
    void             *ringMem = NULL;
    uint32_t          ringMemSize,qCnt;
    Udma_ChHandle     chHandle;
    struct Udma_ChObj chObj;
    Udma_ChPrms       chPrms;
    uint32_t          chType;
    uint32_t          ringMode;
    Udma_ChTxPrms     txChPrms;
    uint64_t          ringData;
    int32_t           status;
    Udma_DrvHandle    drvHandle;
    uint32_t          backupDevIdRing;

    ringMode    = TISCI_MSG_VALUE_RM_RING_MODE_RING;
    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem     = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }
    chHandle                      = &chObj;
    chType                        = UDMA_CH_TYPE_TX;
    UdmaChPrms_init(&chPrms, chType);
    drvHandle                     = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];
    chPrms.peerChNum              = UDMA_PSIL_CH_MAIN_CSI_TX;
    chPrms.fqRingPrms.ringMem     = ringMem;
    chPrms.fqRingPrms.ringMemSize = ringMemSize;
    chPrms.fqRingPrms.elemCnt     = elemCnt;
    chPrms.fqRingPrms.mode        = ringMode;
    retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        UdmaChTxPrms_init(&txChPrms, chType);
        retVal = Udma_chConfigTx(chHandle, &txChPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(chHandle);
        }
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: Udma ringGetMemPtrLcdma negative Testcase ::\r\n",
                      taskObj->taskId);

            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                /* Queue the descriptors to the ring */
                ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                status   = Udma_ringQueueRawLcdma(drvHandle, chHandle->fqRing, ringData);
                if(UDMA_SOK == status)
                {
                    retVal = UDMA_SOK;
                }
                else
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|::FAIL:: UDMA:: Udma_ringQueueRawLcdma :: Neg::"
                              " Fail to queue the descriptors!!\n");
                     retVal = UDMA_EFAIL;
                }
            }

            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                /* Dequeue the descriptors from the ring */
                ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                status   = Udma_ringDequeueRawLcdma(drvHandle, chHandle->fqRing, &ringData);
                if(UDMA_SOK == status)
                {
                    retVal = UDMA_SOK;
                }
                else
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|::FAIL:: UDMA:: Udma_ringDequeueRawLcdma :: Neg::"
                              " Fail to dequeue the descriptors!!\n");
                    retVal = UDMA_EFAIL;
                }
            }

            /* Test scenario 1: Check to get Ring reset failed error when devIdRing is invalid */
            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                ringData             = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                backupDevIdRing      = drvHandle->devIdRing;
                drvHandle->devIdRing = UDMA_RING_ANY;
                status               = Udma_ringFlushRawLcdma(drvHandle, chHandle->fqRing, &ringData);
                if(UDMA_SOK != status)
                {
                    retVal = UDMA_SOK;
                }
                else
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|::FAIL:: UDMA:: Udma_ringFlushRawLcdma :: Neg::"
                              " [Error] Ring reset failed when devIdRing is invalid!!!\n");
                    retVal = UDMA_EFAIL;
                }
                drvHandle->devIdRing = backupDevIdRing;
            }
        }
        Udma_chDisable(chHandle, 0);
    }
    Udma_chClose(chHandle);

    return retVal;
}

