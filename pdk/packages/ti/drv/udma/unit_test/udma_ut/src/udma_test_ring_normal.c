/*
 *  Copyright (c) Texas Instruments Incorporated 2023
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
 *  \file udma_test_ring_normal.c
 *
 *  \brief UDMA negative test cases for udma_ring_normal.c test case file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <udma_test.h>
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

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Test Case Description: Verifies the function Udma_ringGetMemPtrNormal when
 * 1)ringHandle is NULL.
 * 2)ringHandle = UDMA_DEINIT_DONE.
 */
int32_t udmaTestRingGetMemPtrNormal(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal      = UDMA_SOK;
    void    *pRingMem    = NULL_PTR;
    struct Udma_RingObj ringObj;
    Udma_RingHandle ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ring get mem ptr normal negative Testcase ::\r\n", taskObj->taskId);

    /* ringHandle is NULL */
    ringHandle = (Udma_RingHandle)NULL_PTR;
    pRingMem = Udma_ringGetMemPtrNormal(ringHandle);
    if(NULL_PTR == pRingMem)
    {
       retVal = UDMA_SOK;
    }
    else
    {
       GT_0trace(taskObj->traceMask, GT_ERR,
           " |TEST INFO|:: Test:: Udma ring get mem ptr normal negative Testcase Failed!!\n");
    }

    /* ringInitDone = UDMA_DEINIT_DONE */
    ringHandle                 = &ringObj;
    ringHandle->ringInitDone   = UDMA_DEINIT_DONE;
    pRingMem = Udma_ringGetMemPtrNormal(ringHandle);
    if(NULL_PTR == pRingMem)
    {
       retVal = UDMA_SOK;
    }
    else
    {
      GT_0trace(taskObj->traceMask, GT_ERR,
          " |TEST INFO|:: Test:: Udma ring get mem ptr normal negative Testcase Failed!!\n");
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringGetModeNormal when
 * 1)ringHandle is NULL.
 * 2)ringHandle = UDMA_DEINIT_DONE.
 */
int32_t udmaTestRingGetModeNormal(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal      = UDMA_SOK;
    uint32_t ringMode    = CSL_RINGACC_RING_MODE_INVALID;
    struct Udma_RingObj   ringObj;
    Udma_RingHandle  ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
               " |TEST INFO|:: Task:%d: Udma ring get mode normal negative Testcase ::\r\n", taskObj->taskId);

    /* ringHandle is NULL */
    ringHandle = (Udma_RingHandle)NULL_PTR;
    ringMode = Udma_ringGetModeNormal(ringHandle);
    if(CSL_RINGACC_RING_MODE_INVALID == ringMode)
    {
       retVal = UDMA_SOK;
    }
    else
    {
       GT_0trace(taskObj->traceMask, GT_ERR,
           " |TEST INFO|:: Test:: Udma ring get mode normal negative Testcase Failed!!\n");
    }

    /* ringInitDone = UDMA_DEINIT_DONE */
    ringHandle               = &ringObj;
    ringHandle->ringInitDone = UDMA_DEINIT_DONE;
    ringMode = Udma_ringGetModeNormal(ringHandle);
    if(CSL_RINGACC_RING_MODE_INVALID == ringMode)
    {
       retVal = UDMA_SOK;
    }
    else
    {
       GT_0trace(taskObj->traceMask, GT_ERR,
           " |TEST INFO|:: Test:: Udma ring get mode normal negative Testcase Failed!!\n");
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringGetElementCntNormal when
 * 1)ringHandle is NULL.
 * 2)ringHandle = UDMA_DEINIT_DONE.
 */
int32_t udmaTestRingGetElementCntNormal(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal            = UDMA_SOK;
    uint32_t ringElementCnt    = 0U;
    struct Udma_RingObj ringObj;
    Udma_RingHandle ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: Udma ring get element count normal negative Testcase ::\r\n", taskObj->taskId);

    /* ringHandle is NULL */
    ringHandle = (Udma_RingHandle)NULL_PTR;
    ringElementCnt = Udma_ringGetElementCntNormal(ringHandle);
    if(0U == ringElementCnt)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
            " |TEST INFO|:: Test:: Udma ring get element count normal negative Testcase Failed!!\n");
    }

    /* ringInitDone = UDMA_DEINIT_DONE */
    ringHandle                 = &ringObj;
    ringHandle->ringInitDone   = UDMA_DEINIT_DONE;
    ringElementCnt = Udma_ringGetElementCntNormal(ringHandle);
    if(0U == ringElementCnt)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
            " |TEST INFO|:: Test:: Udma ring get element count normal negative Testcase Failed!!\n");
    }

    return retVal;
}
/*
 * Test Case Description: Verifies the function Udma_ringGetRingOccNormal when
 * 1)ringHandle is NULL.
 * 2)ringHandle = UDMA_DEINIT_DONE.
 */
int32_t udmaTestRingGetRingOccNormal(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal        = UDMA_SOK;
    uint32_t ringOccCnt    = 0U;
    struct Udma_RingObj ringObj;
    Udma_RingHandle ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: Udma get ring occupancy normal negative Testcase ::\r\n", taskObj->taskId);

    /* ringHandle is NULL */
    ringHandle = (Udma_RingHandle)NULL_PTR;
    ringOccCnt = Udma_ringGetRingOccNormal(ringHandle);
    if(0U == ringOccCnt)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
            " |TEST INFO|:: Test:: Udma get ring occupancy normal negative Testcase Failed!!\n");
    }

    /* ringInitDone = UDMA_DEINIT_DONE */
    ringHandle                 = &ringObj;
    ringHandle->ringInitDone   = UDMA_DEINIT_DONE;
    ringOccCnt = Udma_ringGetRingOccNormal(ringHandle);
    if(0U == ringOccCnt)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
            " |TEST INFO|:: Test:: Udma get ring occupancy normal negative Testcase Failed!!\n");
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringGetWrIdxNormal when
 * 1)ringHandle is NULL.
 * 2)ringHandle = UDMA_DEINIT_DONE.
 */
int32_t udmaTestRingGetWrIdxNormal(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal       = UDMA_SOK;
    uint32_t ringWrIdx    = 0U;
    struct Udma_RingObj ringObj;
    Udma_RingHandle ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
                 " |TEST INFO|:: Task:%d: Udma ring get write index normal negative Testcase ::\r\n", taskObj->taskId);

    /* ringHandle is NULL */
    ringHandle = (Udma_RingHandle)NULL_PTR;
    ringWrIdx = Udma_ringGetWrIdxNormal(ringHandle);
    if(0U == ringWrIdx)
    {
          retVal = UDMA_SOK;
    }
    else
    {
          GT_0trace(taskObj->traceMask, GT_ERR,
              " |TEST INFO|:: Test:: Udma ring get write index normal negative Testcase Failed!!\n");
    }

    /* ringInitDone = UDMA_DEINIT_DONE */
    ringHandle                 = &ringObj;
    ringHandle->ringInitDone   = UDMA_DEINIT_DONE;
    ringWrIdx = Udma_ringGetWrIdxNormal(ringHandle);
    if(0U == ringWrIdx)
    {
        retVal = UDMA_SOK;
    }
    else
    {
       GT_0trace(taskObj->traceMask, GT_ERR,
           " |TEST INFO|:: Test:: Udma ring get write index normal negative Testcase Failed!!\n");
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringGetRdIdxNormal when
 * 1)ringHandle is NULL.
 * 2)ringHandle = UDMA_DEINIT_DONE.
 */
int32_t udmaTestRingGetRdIdxNormal(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal       = UDMA_SOK;
    uint32_t ringRdIdx    = 0U;
    struct Udma_RingObj ringObj;
    Udma_RingHandle ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
			  " |TEST INFO|:: Task:%d: Udma ring get read index normal negative Testcase ::\r\n", taskObj->taskId);

    /* ringHandle is NULL */
    ringHandle = (Udma_RingHandle)NULL_PTR;
    ringRdIdx = Udma_ringGetRdIdxNormal(ringHandle);
    if(0U == ringRdIdx)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
            " |TEST INFO|:: Test:: Udma ring get read index normal negative negative Testcase Failed!!\n");
    }

    /* ringInitDone = UDMA_DEINIT_DONE */
    ringHandle                 = &ringObj;
    ringHandle->ringInitDone   = UDMA_DEINIT_DONE;
    ringRdIdx = Udma_ringGetRdIdxNormal(ringHandle);
    if(0U == ringRdIdx)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
            " |TEST INFO|:: Test:: Udma ring get read index normal negative Testcase Failed!!\n");
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringPrimeReadLcdma
 * 1)Test scenario 1: Check to get physical descriptor memory pointer value as 0.
 */
int32_t UdmaTestRingPrimeReadNormalNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal     = UDMA_SOK;
    uint32_t            heapId     = UTILS_MEM_HEAP_ID_MSMC;
    uint32_t            elemCnt    = 50U;
    void               *ringMem    = NULL;
    uint64_t            phyDescMem = 0UL;
    uint32_t            ringMode,ringMemSize;
    Udma_DrvHandle      drvHandle;
    Udma_RingPrms       ringPrms;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;


    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem     = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }

    if(UDMA_SOK == retVal)
    {
        ringHandle = &ringObj;
        ringMode   = TISCI_MSG_VALUE_RM_RING_MODE_RING;
        drvHandle  = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];

        UdmaRingPrms_init(&ringPrms);
        ringPrms.ringMem     = ringMem;
        ringPrms.ringMemSize = ringMemSize;
        ringPrms.mode        = ringMode;
        ringPrms.elemCnt     = elemCnt;

        /* Allocate a free ring */
        retVal = Udma_ringAlloc(drvHandle, ringHandle, UDMA_RING_ANY, &ringPrms);
        if(UDMA_SOK == retVal)
        {
            Udma_ringPrimeReadNormal(ringHandle, &phyDescMem);
        }
        Udma_ringFree(ringHandle);
    }

    return retVal;
}

