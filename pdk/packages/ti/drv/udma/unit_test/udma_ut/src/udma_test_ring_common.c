/*
 *  Copyright (c) Texas Instruments Incorporated 2023-2024
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
 *  \file udma_test_ring_common.c
 *
 *  \brief UDMA negative test cases for Udma Ring Common API's.
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
 * Test Case Description: Verifies the function Udma_ringAttach when
 * 1)Test scenario 1: Check when ringHandle and driverHandle are NULL.
 * 2)Test scenario 2: Check when driverHandle is NULL.
 * 3)Test scenario 3: Check when ringHandle is NULL.
 * 4)Test scenario 4: Check when drvInitDone is UDMA_DEINIT_DONE.
 * 5)Test scenario 5: Check when ringNum greater than maxRings.
 */
int32_t UdmaTestRingAttachNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal  = UDMA_SOK;
    uint16_t            ringNum = UDMA_RING_ANY;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;
    Udma_DrvHandle      drvHandle;
    uint32_t            backUpDrvInitDone;
    uint32_t            instId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringAttach negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle and driverHandle are NULL */
    drvHandle  = (Udma_DrvHandle) NULL_PTR;
    ringHandle = (Udma_RingHandle) NULL_PTR;
    retVal     = Udma_ringAttach(drvHandle, ringHandle, ringNum);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringAttach :: Neg::"
                  " Check when ringHandle and driverHandle are NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when driverHandle is NULL */
        ringHandle = &ringObj;
        drvHandle  = (Udma_DrvHandle) NULL_PTR;
        retVal     = Udma_ringAttach(drvHandle, ringHandle, ringNum);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAttach :: Neg::"
                      " Check when driverHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when ringHandle is NULL */
        ringHandle = (Udma_RingHandle) NULL_PTR;
        instId     = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle  = &taskObj->testObj->drvObj[instId];
        retVal     = Udma_ringAttach(drvHandle, ringHandle, ringNum);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAttach :: Neg::"
                      " Check when ringHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when drvInitDone is UDMA_DEINIT_DONE */
        ringHandle             = &ringObj;
        drvHandle              = &taskObj->testObj->drvObj[instId];
        backUpDrvInitDone      = drvHandle->drvInitDone;
        drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                 = Udma_ringAttach(drvHandle, ringHandle, ringNum);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAttach :: Neg::"
                      " Check when drvInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        drvHandle->drvInitDone = backUpDrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when ringNum greater than maxRings */
        drvHandle->drvInitDone = UDMA_INIT_DONE;
        retVal                 = Udma_ringAttach(drvHandle, ringHandle, ringNum);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAttach :: Neg::"
                      " Check when ringNum greater than maxRings!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringDetach when
 * 1)Test scenario 1: Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE.
 * 3)Test scenario 3: Check when driverHandle is NULL.
 * 4)Test scenario 4: Check when drvInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestRingDetachNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;
    uint32_t            instId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringDetach negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle is NULL */
    ringHandle = (Udma_RingHandle) NULL_PTR;
    retVal     = Udma_ringDetach(ringHandle);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringDetach :: Neg::"
                  " Check when ringHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_DEINIT_DONE;
        retVal                   = Udma_ringDetach(ringHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringDetach :: Neg::"
                      " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when driverHandle is NULL */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_INIT_DONE;
        ringHandle->drvHandle    = (Udma_DrvHandle) NULL_PTR;
        retVal                   = Udma_ringDetach(ringHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringDetach :: Neg::"
                      " Check when driverHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when drvInitDone is UDMA_DEINIT_DONE */
        instId                             = UDMA_TEST_INST_ID_MAIN_0;
        ringHandle->drvHandle              = &taskObj->testObj->drvObj[instId];
        ringHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                             = Udma_ringDetach(ringHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringDetach :: Neg::"
                      " Check when drvInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        ringHandle->drvHandle->drvInitDone = UDMA_INIT_DONE;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringGetNum when
 * 1)Test scenario 1: NULL Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestRingGetNumNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
    uint16_t            ringNum;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringGetNum negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: NULL Check when ringHandle is NULL */
    ringHandle = (Udma_RingHandle) NULL_PTR;
    ringNum    = Udma_ringGetNum(ringHandle);
    if(UDMA_RING_INVALID == ringNum)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringGetNum :: Neg::"
                  " NULL Check when ringHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_DEINIT_DONE;
        ringNum                  = Udma_ringGetNum(ringHandle);
        if(UDMA_RING_INVALID == ringNum)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringGetNum :: Neg::"
                      " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringDequeueRaw when
 * 1)Test scenario 1: Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE.
 * 3)Test scenario 3: Check when ringNum is UDMA_RING_INVALID.
 * 4)Test scenario 4: Check when driverHandle is NULL.
 * 5)Test scenario 5: Check when drvInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestRingDequeueRawNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal     = UDMA_SOK;
    uint64_t            phyDescMem = 0UL;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;
    uint32_t            instId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringDequeue negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle is NULL */
    ringHandle = (Udma_RingHandle) NULL_PTR;
    retVal     = Udma_ringDequeueRaw(ringHandle, &phyDescMem);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringDequeue :: Neg::"
                  " Check when ringHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_DEINIT_DONE;
        ringHandle->ringNum      = UDMA_RING_ANY;
        retVal                   = Udma_ringDequeueRaw(ringHandle, &phyDescMem);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringDequeue :: Neg::"
                      " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when ringNum is UDMA_RING_INVALID */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_INIT_DONE;
        ringHandle->ringNum      = UDMA_RING_INVALID;
        retVal                   = Udma_ringDequeueRaw(ringHandle, &phyDescMem);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringDequeue :: Neg::"
                      " Check when ringNum is UDMA_RING_INVALID!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when driverHandle is NULL */
        ringHandle->ringNum   = UDMA_RING_ANY;
        ringHandle->drvHandle = (Udma_DrvHandle) NULL_PTR;
        retVal                = Udma_ringDequeueRaw(ringHandle, &phyDescMem);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringDequeue :: Neg::"
                      " Check when driverHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when drvInitDone is UDMA_DEINIT_DONE */
        instId                             = UDMA_TEST_INST_ID_MAIN_0;
        ringHandle->drvHandle              = &taskObj->testObj->drvObj[instId];
        ringHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                             = Udma_ringDequeueRaw(ringHandle, &phyDescMem);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringDequeue :: Neg::"
                      " Check when drvInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        ringHandle->drvHandle->drvInitDone = UDMA_INIT_DONE;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringQueueRaw when
 * 1)Test scenario 1: Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE.
 * 3)Test scenario 3: Check when ringNum is UDMA_RING_INVALID.
 * 4)Test scenario 4: Check when driverHandle is NULL.
 * 5)Test scenario 5: Check when drvInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestRingQueueRawNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal     = UDMA_SOK;
    uint64_t            phyDescMem = 0UL;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;
    uint32_t            instId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringQueueRaw negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle is NULL */
    ringHandle = (Udma_RingHandle) NULL_PTR;
    retVal     = Udma_ringQueueRaw(ringHandle, phyDescMem);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringQueueRaw :: Neg::"
                  " Check when ringHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_DEINIT_DONE;
        ringHandle->ringNum      = UDMA_RING_ANY;
        retVal                   = Udma_ringQueueRaw(ringHandle, phyDescMem);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringQueueRaw :: Neg::"
                      " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when ringNum is UDMA_RING_INVALID */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_INIT_DONE;
        ringHandle->ringNum      = UDMA_RING_INVALID;
        retVal                   = Udma_ringQueueRaw(ringHandle, phyDescMem);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringQueueRaw :: Neg::"
                      " Check when ringNum is UDMA_RING_INVALID!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when driverHandle is NULL */
        ringHandle->ringNum   = UDMA_RING_ANY;
        ringHandle->drvHandle = (Udma_DrvHandle) NULL_PTR;
        retVal                = Udma_ringQueueRaw(ringHandle, phyDescMem);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      "|TEST INFO|:: FAIL:: UDMA:: ringQueueRaw :: Neg::"
                      " Check when driverHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when drvInitDone is UDMA_DEINIT_DONE */
        instId                             = UDMA_TEST_INST_ID_MAIN_0;
        ringHandle->drvHandle              = &taskObj->testObj->drvObj[instId];
        ringHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                             = Udma_ringQueueRaw(ringHandle, phyDescMem);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringQueueRaw :: Neg::"
                      " Check when drvInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        ringHandle->drvHandle->drvInitDone = UDMA_INIT_DONE;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringFree when
 * 1)Test scenario 1: Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE.
 * 3)Test scenario 3: Check when driverHandle is NULL.
 * 4)Test scenario 4: Check when drvInitDone is UDMA_DEINIT_DONE.
 * 5)Test scenario 5: Check when mappedRingGrp is not UDMA_MAPPED_GROUP_INVALID.
 */
int32_t UdmaTestRingFreeNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;
    uint32_t            instId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringFree Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle is NULL */
    ringHandle = (Udma_RingHandle) NULL_PTR;
    retVal     = Udma_ringFree(ringHandle);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringFree :: Neg::"
                  " Check when ringHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_DEINIT_DONE;
        retVal                   = Udma_ringFree(ringHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringFree :: Neg::"
                      " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when driverHandle is NULL */
        ringHandle->ringInitDone = UDMA_INIT_DONE;
        ringHandle->drvHandle    = (Udma_DrvHandle) NULL_PTR;
        retVal                   = Udma_ringFree(ringHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringFree :: Neg::"
                      " Check when driverHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when drvInitDone is UDMA_DEINIT_DONE */
        instId                             = UDMA_TEST_INST_ID_MAIN_0;
        ringHandle->drvHandle              = &taskObj->testObj->drvObj[instId];
        ringHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                             = Udma_ringFree(ringHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringFree :: Neg::"
                      " Check when drvInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        ringHandle->drvHandle->drvInitDone = UDMA_INIT_DONE;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when mappedRingGrp is not UDMA_MAPPED_GROUP_INVALID */
        instId                    = UDMA_TEST_INST_ID_MAIN_0;
        ringHandle->drvHandle     = &taskObj->testObj->drvObj[instId];
        ringHandle->mappedRingGrp = UDMA_UTC_ID_INVALID;
        retVal                    = Udma_ringFree(ringHandle);
        if(UDMA_SOK == retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringFree :: Neg::"
                      " Check when mappedRingGrp is not UDMA_MAPPED_GROUP_INVALID!!\n");
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringAlloc when
 * 1)Test scenario 1: Check when ringHandle,driverHandle and ringPrms are NULL.
 * 2)Test scenario 2: Check when ringHandle and driverHandle are NULL.
 * 3)Test scenario 3: Check when ringHandle and ringPrms are NULL.
 * 4)Test scenario 4: Check when ringHandle is NULL.
 * 5)Test scenario 5: Check when ringPrms and driverHandle are NULL.
 * 6)Test scenario 6: Check when driverHandle is NULL.
 * 7)Test scenario 7: Check when ringPrms is NULL.
 * 8)Test scenario 8: Check when drvInitDone is UDMA_DEINIT_DONE.
 * 9)Test scenario 9: Check to get ringNum as UDMA_RING_INVALID from Udma_rmAllocFreeRing.
 * 10)Test scenario 10: Check when ringNum as UDMA_RING_INVALID, 
 *                  and resource not available to allocate
 * 11)Test scenario 11: To get error print message [Error] Out of range
 *                      ring index.
 * 12)Test scenario 12: To get error print message [Error] Ring config failed!.
 * 13)Test scenario 13: Check when mappedRingGrp is not UDMA_MAPPED_GROUP_INVALID.
 */
int32_t udmaTestRingAllocNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal  = UDMA_SOK;
    uint16_t            ringNum = UDMA_RING_ANY;
    uint32_t            elemCnt = 50U;
    void               *ringMem = NULL;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;
    Udma_RingPrms       ringPrms;
    Udma_RingPrms       ringPrmsinit;
    Udma_DrvHandle      drvHandle;
    uint32_t            backUpDrvInitDone;
    uint32_t            ringMemSize;
    uint32_t            instId;
    struct Udma_DrvObj  backUpDrvObj;
    int32_t             i = 0;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringAlloc Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle,driverHandle and ringPrms are NULL */
    drvHandle  = (Udma_DrvHandle) NULL_PTR;
    ringHandle = (Udma_RingHandle) NULL_PTR;
    retVal     = Udma_ringAlloc(drvHandle, ringHandle, ringNum, NULL);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringAlloc :: Neg:: "
                  " Check when ringHandle,driverHandle and ringPrms "
                  " are NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when ringHandle and driverHandle are NULL */
        ringHandle = (Udma_RingHandle) NULL_PTR;
        drvHandle  = (Udma_DrvHandle) NULL_PTR;
        retVal     = Udma_ringAlloc(drvHandle, ringHandle, ringNum, &ringPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAlloc :: Neg::"
                      " Check when ringHandle and driverHandle are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when ringHandle and ringPrms are NULL */
        ringHandle = (Udma_RingHandle) NULL_PTR;
        instId     = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle  = &taskObj->testObj->drvObj[instId];
        retVal     = Udma_ringAlloc(drvHandle, ringHandle, ringNum, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAlloc :: Neg::"
                      " Check when ringHandle and ringPrms are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when ringHandle is NULL */
        ringHandle = (Udma_RingHandle) NULL_PTR;
        drvHandle  = &taskObj->testObj->drvObj[instId];
        retVal     = Udma_ringAlloc(drvHandle, ringHandle, ringNum, &ringPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ring alloc :: Neg::"
                      " Check when ringHandle is NULL!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when ringPrms and driverHandle are NULL */
        ringHandle = &ringObj;
        drvHandle  = (Udma_DrvHandle) NULL_PTR;
        retVal     = Udma_ringAlloc(drvHandle, ringHandle, ringNum, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAlloc :: Neg::"
                      " Check when ringPrms and driverHandle are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check when driverHandle is NULL */
        ringHandle = &ringObj;
        drvHandle  = (Udma_DrvHandle) NULL_PTR;
        retVal     = Udma_ringAlloc(drvHandle, ringHandle, ringNum, &ringPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAlloc :: Neg::"
                      " Check when driverHandle is NULL!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 7: Check when ringPrms is NULL */
        ringHandle = &ringObj;
        drvHandle  = &taskObj->testObj->drvObj[instId];
        retVal     = Udma_ringAlloc(drvHandle, ringHandle, ringNum, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAlloc :: Neg::"
                      " Check when ringPrms is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 8: Check when drvInitDone is UDMA_DEINIT_DONE */
        UdmaRingPrms_init(&ringPrmsinit);
        backUpDrvInitDone      = drvHandle->drvInitDone;
        drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                 = Udma_ringAlloc(drvHandle, ringHandle, ringNum,
                                                &ringPrmsinit);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAlloc :: Neg::"
                      " Check when drvInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        drvHandle->drvInitDone = backUpDrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 9: Check to get ringNum as UDMA_RING_INVALID from Udma_rmAllocFreeRing */
        ringMemSize                = elemCnt * sizeof (uint64_t);
        ringMem                    = Utils_memAlloc(UTILS_MEM_HEAP_ID_MSMC, ringMemSize,
                                                    UDMA_CACHELINE_ALIGNMENT);
        UdmaRingPrms_init(&ringPrmsinit);
        ringPrmsinit.ringMem       = ringMem;
        ringPrmsinit.ringMemSize   = ringMemSize;
        ringPrmsinit.mode          = UDMA_TEST_RING_MODE_DEFAULT_START;
        ringPrmsinit.elemCnt       = elemCnt;
        ringNum                    = UDMA_RING_ANY;
        ringPrmsinit.mappedRingGrp = UDMA_MAPPED_GROUP_INVALID;
        drvHandle                  = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];
        retVal                     = Udma_ringAlloc(drvHandle, ringHandle, ringNum,
                                                    &ringPrmsinit);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAlloc:: Neg::"
                      " Check to get ringNum as UDMA_RING_INVALID from "
                      " Udma_rmAllocFreeRing !!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 10: Check when ringNum is UDMA_RING_INVALID, 
         *                  and resource not available to allocate */
        instId                     = UDMA_TEST_INST_ID_MCU_0;
        ringMemSize                = elemCnt * sizeof (uint64_t);
        ringMem                    = Utils_memAlloc(UTILS_MEM_HEAP_ID_MSMC, ringMemSize,
                                                    UDMA_CACHELINE_ALIGNMENT);
        UdmaRingPrms_init(&ringPrmsinit);
        ringPrmsinit.ringMem       = ringMem;
        ringPrmsinit.ringMemSize   = ringMemSize;
        ringPrmsinit.mode          = UDMA_TEST_RING_MODE_DEFAULT_START;
        ringPrmsinit.elemCnt       = elemCnt;
        ringNum                    = UDMA_RING_ANY;
        ringPrmsinit.mappedRingGrp = UDMA_MAPPED_GROUP_INVALID;
        drvHandle                  = &taskObj->testObj->drvObj[instId];
        backUpDrvObj               = taskObj->testObj->drvObj[instId];
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numFreeRing; i++)
        {
            drvHandle->freeRingFlag[i] = 0U; //make resource not available
        }
        
        retVal  = Udma_ringAlloc(drvHandle, ringHandle, ringNum,
                                                    &ringPrmsinit);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAlloc:: Neg::"
                      " Check when ringNum is UDMA_RING_INVALID!!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[instId] = backUpDrvObj;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 11: To get error print message [Error] Out of
         *                   range ring index
         */
        instId    = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle = &taskObj->testObj->drvObj[instId];
        ringNum   = UDMA_RING_INVALID;
        retVal    = Udma_ringAlloc(drvHandle, ringHandle, ringNum, &ringPrmsinit);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAlloc :: Neg::"
                      " To get error print message [Error] Out of range ring index!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 12: To get error print message [Error] Ring config failed! */
        instId              = UDMA_TEST_INST_ID_MAIN_0;
        backUpDrvObj        = taskObj->testObj->drvObj[instId];
        drvHandle           = &taskObj->testObj->drvObj[instId];
        ringNum             = UDMA_RING_ANY;
        ringHandle->ringNum = UDMA_RING_INVALID;
        ringPrmsinit.virtId = UDMA_RING_VIRTID_INVALID - 1U;
        retVal              = Udma_ringAlloc(drvHandle, ringHandle, ringNum, &ringPrmsinit);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringAlloc :: Neg::"
                      " To get error print message [Error] Ring config failed!!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[instId] = backUpDrvObj;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 13: Check when mappedRingGrp is not UDMA_MAPPED_GROUP_INVALID */
        instId                     = UDMA_TEST_INST_ID_MAIN_0;
        backUpDrvObj               = taskObj->testObj->drvObj[instId];
        drvHandle                  = &taskObj->testObj->drvObj[instId];
        ringNum                    = UDMA_RING_ANY;
        ringPrmsinit.mappedRingGrp = UDMA_UTC_ID_INVALID;
        retVal                     = Udma_ringAlloc(drvHandle, ringHandle, ringNum, &ringPrmsinit);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringFree :: Neg::"
                      " Check when mappedRingGrp is not UDMA_MAPPED_GROUP_INVALID!!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[instId] = backUpDrvObj;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringFlushRaw when
 * 1)Test scenario 1: Check when ringHandle is NULL.
 * 2)Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE.
 * 3)Test scenario 3: Check when ringNum is UDMA_RING_INVALID.
 * 4)Test scenario 4: Check when drvHandle is NULL.
 * 5)Test scenario 5: Check when drvInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestRingFlushRawNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal     = UDMA_SOK;
    uint64_t            phyDescMem = 0UL;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;
    uint32_t            instId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Udma ringflushraw negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringHandle is NULL */
    ringHandle = (Udma_RingHandle) NULL_PTR;
    retVal     = Udma_ringFlushRaw(ringHandle, &phyDescMem);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringflushraw :: Neg::"
                  " Check when ringHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when ringInitDone is UDMA_DEINIT_DONE */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_DEINIT_DONE;
        ringHandle->ringNum      = UDMA_RING_ANY;
        retVal                   = Udma_ringFlushRaw(ringHandle, &phyDescMem);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringflushraw :: Neg::"
                      " Check when ringInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when ringNum is UDMA_RING_INVALID */
        ringHandle               = &ringObj;
        ringHandle->ringInitDone = UDMA_INIT_DONE;
        ringHandle->ringNum      = UDMA_RING_INVALID;
        retVal                   = Udma_ringFlushRaw(ringHandle, &phyDescMem);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringflushraw :: Neg::"
                      " Check when ringNum is UDMA_RING_INVALI!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when drvHandle is NULL */
        ringHandle->ringNum   = UDMA_RING_ANY;
        ringHandle->drvHandle = (Udma_DrvHandle) NULL_PTR;
        retVal                = Udma_ringFlushRaw(ringHandle, &phyDescMem);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringflushraw :: Neg::"
                      " Check when drvHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when drvInitDone is UDMA_DEINIT_DONE */
        instId                             = UDMA_TEST_INST_ID_MAIN_0;
        ringHandle->drvHandle              = &taskObj->testObj->drvObj[instId];
        ringHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                             = Udma_ringFlushRaw(ringHandle, &phyDescMem);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringflushraw :: Neg::"
                      " Check when drvInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        ringHandle->drvHandle->drvInitDone = UDMA_INIT_DONE;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function UdmaRingPrms_init when
 * Test scenario 1: Check when ringPrms is NULL.
 */
int32_t UdmaTestRingPrmsInitNeg(UdmaTestTaskObj *taskObj)
{
    int32_t        retVal = UDMA_SOK;
    Udma_RingPrms *ringPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA RingPrmsinit negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when ringPrms is NULL */
    ringPrms = (Udma_RingPrms*) NULL_PTR;
    retVal   = UdmaRingPrms_init(ringPrms);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: RingPrmsinit :: Neg::"
                  " Check when ringPrms is NUL!!\n");
        retVal = UDMA_EFAIL;
    }

    return retVal;

}

/*
 * Test Case Description: Verifies the function Udma_ringProxyQueueRaw when
 * 1) Test scenario 1: Check when instType is not UDMA_INST_TYPE_NORMAL.
 */
int32_t UdmaTestRingProxyQueueRawNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal     = UDMA_SOK;
    uint64_t            phyDescMem = 0UL;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;
    Udma_DrvHandle      drvHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ringProxyQueueRaw negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when instType is not UDMA_INST_TYPE_NORMAL */
    ringHandle = &ringObj;
    drvHandle  = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];
    retVal     = Udma_ringProxyQueueRaw(ringHandle, drvHandle, phyDescMem);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringProxyQueueRaw:: Neg::"
                  " Check when instType is not UDMA_INST_TYPE_NORMAL!!\n");
        retVal = UDMA_EFAIL;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringProxyDequeueRaw when
 * 1) Test scenario 1: Check when instType is not UDMA_INST_TYPE_NORMAL.
 */
int32_t UdmaTestRingProxyDequeueRawNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal     = UDMA_SOK;
    uint64_t            phyDescMem = 0UL;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle;
    Udma_DrvHandle      drvHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ringProxyDequeueRaw negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when instType is not UDMA_INST_TYPE_NORMAL */
    ringHandle = &ringObj;
    drvHandle  = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];
    retVal     = Udma_ringProxyDequeueRaw(ringHandle, drvHandle, &phyDescMem);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringProxyDequeueRaw:: Neg::"
                  " Check when instType is not UDMA_INST_TYPE_NORMAL!!\n");
        retVal = UDMA_EFAIL;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringCheckParams when
 * 1) Test scenario 1: Check to get [Error] Ring orderId out of range (%u)!!
 */
int32_t UdmaTestRingCheckParamsNeg(UdmaTestTaskObj *taskObj)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;
    Udma_RingPrms   ringPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ringCheckParams negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check to get [Error] Ring orderId out of range (%u)!! */

    drvHandle = &taskObj->testObj->drvObj[taskObj->chObj[0]->instId];
    if(taskObj->chObj[0]->instId != UDMA_TEST_DEFAULT_UDMA_INST)
    {
        ringPrms.mode    = TISCI_MSG_VALUE_RM_RING_MODE_MESSAGE;
    }
    ringPrms.orderId = UDMA_RING_ORDERID_MAX + 1U;
    retVal           = Udma_ringCheckParams(drvHandle, &ringPrms);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringCheckParams:: Neg::"
                  " Check to get [Error] Ring orderId out of range (%u)!!\n");
        retVal = UDMA_EFAIL;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringProxyQueueRaw
 * 1)Test scenario 1: Check to get error from CSL_proxyCfgThread when proxy target is invalid.
 * 2)Test scenario 2: Check to get error from CSL_ringaccGetRingHwOcc when ring is full.
 */
int32_t UdmaTestRingProxyQueueRawNormalNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal  = UDMA_SOK;
    uint32_t             elemCnt = 1U;
    uint32_t             heapId  = UTILS_MEM_HEAP_ID_MSMC;
    void                *ringMem = NULL;
    uint32_t             ringMode,qCnt;
    uint32_t             ringMemSize;
    Udma_DrvHandle       drvHandle;
    Udma_RingPrms        ringPrms;
    struct Udma_RingObj  ringObj;
    struct Udma_ProxyObj queueProxyObj;
    Udma_RingHandle      ringHandle;
    Udma_ProxyHandle     queueProxyHandle;
    Udma_ProxyCfg        proxyCfg;
    uint64_t             ringData;
    uint32_t             backupproxyTargetNumRing;

    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
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
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring alloc failed!!\n");
        }
        else
        {
            /* Allocate a proxy for queue operation */
            queueProxyHandle = &queueProxyObj;
            retVal           = Udma_proxyAlloc(drvHandle, queueProxyHandle, UDMA_PROXY_ANY);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Proxy alloc failed!!\n");
            }
            else
            {
                /* Config proxy for queue operation */
                proxyCfg.proxyMode = CSL_PROXY_QUEUE_ACCESS_MODE_TAIL;
                proxyCfg.elemSize  = UDMA_RING_ES_8BYTES;
                proxyCfg.ringNum   = Udma_ringGetNum(ringHandle);
                retVal             = Udma_proxyConfig(queueProxyHandle, &proxyCfg);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Proxy config failed!!\n");
                }
                else
                {

                    for(qCnt = 0U; qCnt < elemCnt; qCnt++)
                    {
                        /* Test scenario 1: Check to get error from CSL_proxyCfgThread when proxy
                         * target is invalid
                         */
                        backupproxyTargetNumRing      = drvHandle->proxyTargetNumRing;
                        drvHandle->proxyTargetNumRing = UDMA_RING_INVALID;
                        ringData                      = ((uint64_t) qCnt
                                                         | (uint64_t) 0xDEADBEEF00000000UL);
                        retVal                        = Udma_ringProxyQueueRaw(ringHandle,
                                                                               drvHandle, ringData);
                        if(UDMA_SOK != retVal)
                        {
                            retVal = UDMA_SOK;
                        }
                        else
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: Test:: Check to get error from "
                                      " CSL_proxyCfgThread Failed when proxy target is "
                                      " invalid!!\n");
                            retVal = UDMA_EFAIL;
                        }
                        drvHandle->proxyTargetNumRing = backupproxyTargetNumRing;
                    }

                    if(UDMA_SOK == retVal)
                    {
                        /* Test scenario 2: Check to get error from CSL_ringaccGetRingHwOcc when
                         *                  ring is full
                         */
                        for(qCnt = 0U; qCnt < elemCnt; qCnt++)
                        {
                            ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                            retVal   = Udma_ringProxyQueueRaw(ringHandle, drvHandle, ringData);
                        }
                        ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                        retVal   = Udma_ringProxyQueueRaw(ringHandle, drvHandle, ringData);
                        if(UDMA_SOK != retVal)
                        {
                            retVal = UDMA_SOK;
                        }
                        else
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: Test:: Check to get error from "
                                      " CSL_ringaccGetRingHwOcc when ring is full!!\n");
                            retVal = UDMA_EFAIL;
                        }
                    }
                }
            }
            Udma_proxyFree( queueProxyHandle);
        }
        Udma_ringFree(ringHandle);
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringProxyDequeueRaw
 * 1) Test scenario 1: Check to get error from CSL_proxyCfgThread when proxyTargetNumRing
 *                     is invalid
 */
int32_t UdmaTestRingProxyDequeueRawNormalNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal  = UDMA_SOK;
    uint32_t             elemCnt = 1U;
    uint32_t             heapId  = UTILS_MEM_HEAP_ID_MSMC;
    void                *ringMem = NULL;
    uint32_t             ringMode,qCnt;
    Udma_DrvHandle       drvHandle;
    Udma_RingPrms        ringPrms;
    struct Udma_RingObj  ringObj;
    Udma_RingHandle      ringHandle;
    struct Udma_ProxyObj dequeueProxyObj;
    Udma_ProxyHandle     dequeueProxyHandle;
    Udma_ProxyCfg        proxyCfg;
    uint64_t             ringData;
    uint32_t             backupproxyTargetNumRing,ringMemSize;

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
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring alloc failed!!\n");
        }
        else
        {
            /* Allocate a proxy for queue operation */
            dequeueProxyHandle = &dequeueProxyObj;
            retVal             = Udma_proxyAlloc(drvHandle, dequeueProxyHandle,
                                                 UDMA_PROXY_ANY);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Proxy alloc failed!!\n");
            }
            else
            {
                /* Config proxy for queue operation */
                proxyCfg.proxyMode = CSL_PROXY_QUEUE_ACCESS_MODE_TAIL;
                proxyCfg.elemSize  = UDMA_RING_ES_8BYTES;
                proxyCfg.ringNum   = Udma_ringGetNum(ringHandle);
                retVal             = Udma_proxyConfig(dequeueProxyHandle, &proxyCfg);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Proxy config failed!!\n");
                }
                else
                {
                    /* Test scenario 1: Check to get error from CSL_proxyCfgThread
                     *                  when proxyTargetNumRing is invalid.
                     */
                    for(qCnt = 0U; qCnt < elemCnt; qCnt++)
                    {
                        ringData = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
                        retVal   = Udma_ringProxyQueueRaw(ringHandle, drvHandle, ringData);
                    }
                    for(qCnt = 0U; qCnt < elemCnt; qCnt++)
                    {
                        backupproxyTargetNumRing      = drvHandle->proxyTargetNumRing;
                        drvHandle->proxyTargetNumRing = UDMA_RING_INVALID;
                        ringData                      = ((uint64_t) qCnt |
                                                         (uint64_t) 0xDEADBEEF00000000UL);
                        retVal                        = Udma_ringProxyDequeueRaw(ringHandle,
                                                                                 drvHandle,
                                                                                 &ringData);
                        if(UDMA_SOK != retVal)
                        {
                            retVal = UDMA_SOK;
                        }
                        else
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: Udma_ringProxyDequeueRaw:: Neg:: "
                                      " CSL_proxyCfgThread when proxyTargetNumRing is invalid "
                                      " Failed!!\n");
                            retVal = UDMA_EFAIL;
                        }
                        drvHandle->proxyTargetNumRing = backupproxyTargetNumRing;
                    }
                }
            }
            Udma_proxyFree(dequeueProxyHandle);
        }
        Udma_ringFree(ringHandle);
    }

    return retVal;
}
