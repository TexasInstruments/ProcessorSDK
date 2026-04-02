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
 *  (INCLUDING PosLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  \file udma_test_ring_lcdma.c
 *
 *  \brief File containing test cases for UDMA Ring LCDMA related APIs.
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
 * Test Case Description: Verifies the ring lcdma functions with below test scenario's
 * 1)Test scenario 1: Check when ringMem is not NULL_PTR.
 * 2)Test scenario 2: Check ringMode with valid parameters.
 * 3)Test scenario 3: Check to get ring element count.
 * 4)Test scenario 4: Check to get wrIdx value of ring.
 * 5)Test scenario 5: Check to get the reverse ring occupancy count.
 * 6)Test scenario 6: Check to get RdIdx value of ring.
 */
int32_t UdmaTestRingLcdma(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal         = UDMA_SOK;
    uint32_t          ringElementCnt = 0U;
    uint32_t          ringWrIdx      = 0U;
    uint32_t          ringRdIdx      = 0U;
    uint32_t          elemCnt        = 50U;
    uint32_t          heapId         = UTILS_MEM_HEAP_ID_MSMC;
    void             *ringMem        = NULL;
    uint32_t          timeOut        = 100;
    uint32_t          ringMemSize;
    Udma_DrvHandle    drvHandle;
    Udma_ChHandle     chHandle;
    struct Udma_ChObj chObj;
    Udma_ChPrms       chPrms;
    uint32_t          chType,RevOcc;
    uint32_t          ringMode,qCnt;
    uint64_t          ringData;
    Udma_ChTxPrms     txChPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ring lcdma Testcase ::\r\n",
              taskObj->taskId);

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
            /* Test scenario 1 : Check when ringMem is not NULL_PTR */
            if(Udma_ringGetMemPtrLcdma(chHandle->fqRing) == ringMem)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|::FAIL:: UDMA:: Udma_ringGetMemPtrLcdma :: Pos::"
                          " Check when ringMem is not NULL_PTR!!\n");
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 2: Check ringMode with valid parameters */
            ringMode = Udma_ringGetModeLcdma(chHandle->fqRing);
            if(CSL_LCDMA_RINGACC_RING_MODE_INVALID != ringMode)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|::FAIL:: UDMA:: ringGetModeLcdma :: Pos::"
                          " Check ringMode with valid parameters!!\n");
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 3: Check to get ring element count */
            ringElementCnt = Udma_ringGetElementCntLcdma(chHandle->fqRing);
            if(0U != ringElementCnt)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|::FAIL:: UDMA:: ringGetElementCntLcdma :: Pos::"
                          " Check to get ring element count!!\n");
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 4: Check to get wrIdx value of ring */
            ringWrIdx = Udma_ringGetWrIdxLcdma(chHandle->fqRing);
            if(0U == ringWrIdx)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|::FAIL:: UDMA:: Udma_ringGetWrIdxLcdma :: Pos::"
                          " Check to get wrIdx value of ring!!\n");
                retVal = UDMA_EFAIL;
            }
        }

        /* Queue using ring prime API */
        for(qCnt = 0U; qCnt < elemCnt; qCnt++)
        {
            ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
            Udma_ringPrimeLcdma(chHandle->fqRing, ringData);
        }

        /* Do Cache flush and commit to ring */
        Udma_appUtilsCacheWb(ringMem, ringMemSize);
        Udma_ringSetDoorBellLcdma(chHandle->fqRing, elemCnt);

        /* Test scenario 5: Check to get the reverse ring occupancy count */
        if(UDMA_SOK == retVal)
        {
            while(timeOut > 0U)
            {
                RevOcc = Udma_ringGetReverseRingOccLcdma(chHandle->fqRing);
                if(0U != RevOcc)
                {
                    retVal = UDMA_SOK;
                    break;
                }
                else
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|::FAIL:: UDMA:: Udma_ringGetReverseRingOccLcdma :: pos::"
                              " Check to get reverse ring occupancy count!!\n");
                    retVal = UDMA_EFAIL;
                }
                timeOut--;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 6: Check to get RdIdx value of ring */
            ringRdIdx = Udma_ringGetRdIdxLcdma(chHandle->fqRing);
            if(0U == ringRdIdx)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|::FAIL:: UDMA:: Udma_ringGetRdIdxLcdma :: Pos::"
                          " Check to get RdIdx value of ring!!\n");
                retVal = UDMA_EFAIL;
            }
        }

        /* Dequeue using prime read API */
        for(qCnt = 0U; qCnt < elemCnt; qCnt++)
        {
            ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
            Udma_ringPrimeReadLcdma(chHandle->fqRing, &ringData);
        }
        Udma_chDisable(chHandle, 0);
    }
    Udma_chClose(chHandle);

    return retVal;
}

/*
 * Test Case Description: Verifies Udma_ringSetDoorBellLcdma
 * 1)Test scenario 1: Check to get negative count by setting count as -1 * elemCnt.
 * 2)Test scenario 2: Check when the more ring elements are queued than the maximum count.
 * 3)Test scenario 3: Check when the more ring elements are dequeued than the maximum count.
 */
int32_t UdmaTestRingLcdmaDoorBell(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal  = UDMA_SOK;
    uint32_t          elemCnt = 50U;
    uint32_t          heapId  = UTILS_MEM_HEAP_ID_MSMC;
    void             *ringMem = NULL;
    uint32_t          ringMemSize;
    Udma_DrvHandle    drvHandle;
    Udma_ChHandle     chHandle;
    struct Udma_ChObj chObj;
    Udma_ChPrms       chPrms;
    uint32_t          chType;
    uint32_t          ringMode,qCnt;
    uint64_t          ringData;
    Udma_ChTxPrms     txChPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UdmaRingLcdmaDoorBell Testcase ::\r\n",
              taskObj->taskId);

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
            /* Test scenario 1: Check to get negative dbcount by setting value as -1 * elemCnt */
            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                Udma_ringPrimeLcdma(chHandle->fqRing, ringData);
            }
            Udma_ringSetDoorBellLcdma(chHandle->fqRing, (-1 * (int32_t)elemCnt));

            /* Test scenario 2: Check when the more ring elements are queued than the maximum count */
            elemCnt = 128U;
            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                Udma_ringPrimeLcdma(chHandle->fqRing, ringData);
            }
            Udma_ringSetDoorBellLcdma(chHandle->fqRing, elemCnt);

            /* Test scenario 3: Check when the more ring elements are dequeued than the maximum count */
            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                Udma_ringPrimeLcdma(chHandle->fqRing, ringData);
            }
            Udma_ringSetDoorBellLcdma(chHandle->fqRing, (-1 * (int32_t)elemCnt));
        }
        Udma_chDisable(chHandle, 0);
     }
     Udma_chClose(chHandle);

     return retVal;
}

/*
 * Test Case Description: Verifies Udma_ringGetForwardRingOccLcdma when
 * 1)Test scenario 1: Check to get forward ring occupancy count.
 */
int32_t UdmaTestRingGetForwardRingOccLcdma(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal  = UDMA_SOK;
    uint32_t          elemCnt = 50U;
    uint32_t          heapId  = UTILS_MEM_HEAP_ID_MSMC;
    void             *ringMem = NULL;
    uint32_t          ringMemSize;
    Udma_DrvHandle    drvHandle;
    Udma_ChHandle     chHandle;
    struct Udma_ChObj chObj;
    Udma_ChPrms       chPrms;
    uint32_t          chType;
    uint32_t          ringMode,qCnt;
    uint64_t          ringData;
    Udma_ChTxPrms     txChPrms;
    uint32_t          occ;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UdmaTestRingGetForwardRingOccLcdma ::\r\n",
              taskObj->taskId);

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
            /* Test scenario 1: Check to get forward ring occupancy count */
            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                Udma_ringPrimeLcdma(chHandle->fqRing, ringData);
            }
            Udma_ringSetDoorBellLcdma(chHandle->fqRing, elemCnt);
            occ = Udma_ringGetForwardRingOccLcdma(chHandle->fqRing);
            if(0U != occ)
            {
                 retVal = UDMA_SOK;
            }
            else
            {
                 GT_0trace(taskObj->traceMask, GT_ERR,
                           " |TEST INFO|::FAIL:: UDMA:: Udma_ringGetForwardRingOccLcdma :: pos::"
                           " Check to get forward ring occupancy count!!\n");
                 retVal = UDMA_EFAIL;
            }
        }
        Udma_chDisable(chHandle, 0);
    }
    Udma_chClose(chHandle);

    return retVal;
}

/*
 * Test Case Description: Verifies
 * 1)Test scenario 1: Queue the descriptors to a ring.
 * 2)Test scenario 2: Dequeue the descriptors from a ring.
 */
int32_t UdmaTestQueueDequeueLcdma(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal  = UDMA_SOK;
    uint32_t          elemCnt = 30U;
    uint32_t          heapId  = UTILS_MEM_HEAP_ID_MSMC;
    void             *ringMem = NULL;
    uint32_t          ringMemSize;
    Udma_DrvHandle    drvHandle;
    Udma_ChHandle     chHandle;
    struct Udma_ChObj chObj;
    Udma_ChPrms       chPrms;
    uint32_t          chType;
    uint32_t          ringMode,qCnt;
    uint64_t          ringData;
    Udma_ChTxPrms     txChPrms;
    int32_t           status;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UdmaTestQueueDequeueLcdma ::\r\n",
              taskObj->taskId);

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
            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                /* Test scenario 1: Queue the descriptors to a ring */
                ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                status = Udma_ringQueueRawLcdma(drvHandle, chHandle->fqRing, ringData);
                if(UDMA_SOK == status)
                {
                    retVal = UDMA_SOK;
                }
                else
                {
                     GT_0trace(taskObj->traceMask, GT_ERR,
                               " |TEST INFO|::FAIL:: UDMA:: Udma_ringQueueRawLcdma :: Pos::"
                               " Queue the descriptors to a ring !!\n");
                     retVal = UDMA_EFAIL;
                }
            }

            if(UDMA_SOK == retVal)
            {
                for(qCnt = 0U; qCnt < elemCnt; qCnt++)
                {
                    /* Test scenario 2: Dequeue the descriptors from a ring */
                    ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                    status = Udma_ringDequeueRawLcdma(drvHandle, chHandle->fqRing, &ringData);
                    if(UDMA_SOK == status)
                    {
                        retVal = UDMA_SOK;
                    }
                    else
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|::FAIL:: UDMA:: Udma_ringDequeueRawLcdma :: Pos::"
                                  " Dequeue the descriptorsfrom a ring !!\n");
                        retVal = UDMA_EFAIL;
                    }
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
 * 1)Test scenario 1: Dequeue the descriptors from a ring
 */
int32_t UdmaTestRingFlushRawLcdma(UdmaTestTaskObj *taskObj)
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
                      " |TEST INFO|:: Task:%d: Udma Udma_ringFlushRawLcdma positive Testcase ::\r\n",
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
                              " |TEST INFO|::FAIL:: UDMA:: Udma_ringQueueRawLcdma :: pos::"
                              " Fail to queue the descriptors!!\n");
                     retVal = UDMA_EFAIL;
                }
            }

            /* Test scenario 1: Dequeue the descriptors from the ring */
            for(qCnt = 0U; qCnt < elemCnt; qCnt++)
            {
                ringData  = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                status    = Udma_ringFlushRawLcdma(drvHandle, chHandle->fqRing, &ringData);
                if(UDMA_SOK == status)
                {
                    retVal = UDMA_SOK;
                }
                else
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|::FAIL:: UDMA:: Udma_ringFlushRawLcdma :: pos::"
                              " Dequeue the descriptors from the ring!!\n");
                    retVal = UDMA_EFAIL;
                }
            }
        }
        Udma_chDisable(chHandle, 0);
    }
    Udma_chClose(chHandle);

    return retVal;
}

