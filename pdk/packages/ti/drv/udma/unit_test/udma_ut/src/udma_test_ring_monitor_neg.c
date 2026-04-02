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
 *  \file udma_test_ring_monitor_neg.c
 *
 *  \brief UDMA negative test cases for Udma Ring Monitor API's.
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
 * Test Case Description: Verifies the function UdmaRingMonPrms_init when
 * Test scenario 1: Check when monPrms is NULL.
 */
int32_t UdmaTestRingMonPrmsInitNeg(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal = UDMA_SOK;
    Udma_RingMonPrms *monPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ringMonprmsinit negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when monPrms is NULL */
    monPrms = (Udma_RingMonPrms*) NULL_PTR;
    retVal  = UdmaRingMonPrms_init(monPrms);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringMonprmsinit :: Neg::"
                  " Check when monPrms is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    return retVal;

}

/*
 * Test Case Description: Verifies the function Udma_ringMonAlloc when
 * 1)Test scenario 1: Check when monHandle and driver handle are NULL.
 * 2)Test scenario 2: Check when monHandle is NULL.
 * 3)Test scenario 3: Check when driverHandle is NULL.
 * 4)Test scenario 4: Check when drvInitDone is UDMA_DEINIT_DONE.
 * 5)Test scenario 5: Check when instType is not UDMA_INST_TYPE_NORMAL.
 * 6)Test scenario 6: To get error message [Error] Out of range
 *                    ring monitor index.
 * 7)Test scenario 7: Check to get error from Udma_rmAllocRingMon when ringnum is
 *                    UDMA_RING_MON_ANY.
 */
int32_t UdmaTestRingMonAllocNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    Udma_DrvHandle         drvHandle;
    struct Udma_RingMonObj ringMonObj;
    Udma_RingMonHandle     monHandle;
    uint32_t               ringMonNum;
    struct Udma_DrvObj     backUpDrvObj;
    uint32_t               instId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA RingMonAlloc negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when monHandle and driver handle are NULL */
    monHandle = (Udma_RingMonHandle) NULL_PTR;
    drvHandle = (Udma_DrvHandle) NULL_PTR;
    retVal    = Udma_ringMonAlloc(drvHandle, monHandle, UDMA_RING_MON_ANY);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: RingMonAlloc:: Neg::"
                  " Check when monHandle and driver handle are NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when monHandle is NULL */
        monHandle = (Udma_RingMonHandle) NULL_PTR;
        instId    = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle = &taskObj->testObj->drvObj[instId];
        retVal    = Udma_ringMonAlloc(drvHandle, monHandle, UDMA_RING_MON_ANY);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: RingMonAlloc:: Neg::"
                      " Check when monHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when driverHandle is NULL */
        monHandle = &ringMonObj;
        drvHandle = (Udma_DrvHandle) NULL_PTR;
        retVal    = Udma_ringMonAlloc(drvHandle, monHandle, UDMA_RING_MON_ANY);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: RingMonAlloc:: Neg::"
                      " Check when driverHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when drvInitDone is UDMA_DEINIT_DONE */
        instId                 = UDMA_TEST_INST_ID_MAIN_0;
        backUpDrvObj           = taskObj->testObj->drvObj[instId];
        drvHandle              = &taskObj->testObj->drvObj[instId];
        drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                 = Udma_ringMonAlloc(drvHandle, monHandle,
                                                   UDMA_RING_MON_ANY);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: RingMonAlloc:: Neg::"
                      " Check when drvInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[instId] = backUpDrvObj;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when instType is not UDMA_INST_TYPE_NORMAL */
        instId    = UDMA_TEST_INST_ID_BCDMA_0;
        drvHandle = &taskObj->testObj->drvObj[instId];
        retVal    = Udma_ringMonAlloc(drvHandle, monHandle, UDMA_RING_MON_ANY);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: RingMonAlloc:: Neg::"
                      " Check when instType is not UDMA_INST_TYPE_NORMAL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: To get error message [Error] Out of range
         *                  ring monitor index
         */
        instId     = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle  = &taskObj->testObj->drvObj[instId];
        ringMonNum = UDMA_RING_MON_INVALID;
        retVal     = Udma_ringMonAlloc(drvHandle, monHandle, ringMonNum);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: RingMonAlloc:: Neg::"
                      " To get [Error] Out of range ring monitor index!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 7: Check to get error from Udma_rmAllocRingMon when ringnum is
         *                  UDMA_RING_MON_ANY
         */
        instId                                    = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle                                 = &taskObj->testObj->drvObj[instId];
        ringMonNum                                = UDMA_RING_MON_ANY;
        drvHandle->initPrms.rmInitPrms.numRingMon = 0U;
        retVal                                    = Udma_ringMonAlloc(drvHandle,
                                                                      monHandle,
                                                                      ringMonNum);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: RingMonAlloc:: Neg::"
                      " Check to get error from Udma_rmAllocRingMon when ringnum is"
                      " UDMA_RING_MON_ANY!!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[instId] = backUpDrvObj;
    }

    return retVal;
}

/*
 *  Test Case Description: Verifies the function Udma_ringMonFree when
 * 1)Test scenario 1: Check when for monHandle is NULL.
 * 2)Test scenario 2: Check when ringMonInitDone is UDMA_DEINIT_DONE and
 *                    ringMonNum is UDMA_RING_MON_INVALID.
 * 3)Test scenario 3: Check when ringMonInitDone is UDMA_DEINIT_DONE.
 * 4)Test scenario 4: Check when ringMonNum is UDMA_RING_MON_INVALID.
 * 5)Test scenario 5: Check when driverHandle is NULL.
 * 6)Test scenario 6: Check when drvInitDone is UDMA_DEINIT_DONE.
 * 7)Test scenario 7: Check when instType is not UDMA_INST_TYPE_NORMAL.
 */
int32_t UdmaTestRingMonFreeNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    struct Udma_RingMonObj ringMonObj;
    Udma_RingMonHandle     monHandle;
    uint32_t               backUpDrvInitDone;
    uint32_t               instId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ringMonFree negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when for monHandle is NULL */
    monHandle = (Udma_RingMonHandle) NULL_PTR;
    retVal    = Udma_ringMonFree(monHandle);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringMonFree:: Neg::"
                  " Check when for monHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when ringMonInitDone is UDMA_DEINIT_DONE and
         *                  ringMonNum is UDMA_RING_MON_INVALID
         */
        monHandle                  = &ringMonObj;
        monHandle->ringMonInitDone = UDMA_DEINIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_INVALID;
        retVal                     = Udma_ringMonFree(monHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonFree:: Neg::"
                      " Check when ringMonInitDone is UDMA_DEINIT_DONE and"
                      " ringMonNum is UDMA_RING_MON_INVALID!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when ringMonInitDone is UDMA_DEINIT_DONE */
        monHandle->ringMonInitDone = UDMA_DEINIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_ANY;
        retVal                     = Udma_ringMonFree(monHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonFree:: Neg::"
                      " Check when ringMonInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when ringMonNum is UDMA_RING_MON_INVALID */
        monHandle->ringMonInitDone = UDMA_INIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_INVALID;
        retVal                     = Udma_ringMonFree(monHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonFree:: Neg::"
                      " Check when ringMonNum is UDMA_RING_MON_INVALID!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when driverHandle is NULL */
        monHandle->ringMonNum = UDMA_RING_MON_ANY;
        monHandle->drvHandle  = NULL_PTR;
        retVal                = Udma_ringMonFree(monHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonFree:: Neg::"
                      " Check when driverHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check when drvInitDone is UDMA_DEINIT_DONE */
        instId                            = UDMA_TEST_INST_ID_MAIN_0;
        monHandle->drvHandle              = &taskObj->testObj->drvObj[instId];
        backUpDrvInitDone                 = monHandle->drvHandle->drvInitDone;
        monHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                            = Udma_ringMonFree(monHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonFree:: Neg::"
                      " Check when drvInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        monHandle->drvHandle->drvInitDone = backUpDrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 7: Check when instType is not UDMA_INST_TYPE_NORMAL */
        instId               = UDMA_TEST_INST_ID_BCDMA_0;
        monHandle->drvHandle = &taskObj->testObj->drvObj[instId];
        retVal               = Udma_ringMonFree(monHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonFree:: Neg::"
                      " Check when instType is not UDMA_INST_TYPE_NORMAL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringMonConfig when
 * 1)Test scenario 1: Check when monHandle and monPrms are NULL.
 * 2)Test scenario 2: Check when monHandle is NULL.
 * 3)Test scenario 3: Check when ringMonInitDone is UDMA_DEINIT_DONE,
 *                    ringMonNum is UDMA_RING_MON_INVALID and monPrms
 *                    is NULL.
 * 4)Test scenario 4: Check when ringMonInitDone is UDMA_INIT_DONE
 *                    and monPrms is NULL.
 * 5)Test scenario 5: Check when monPrms is NULL and ringMonNum is
 *                    UDMA_RING_MON_INVALID.
 * 6)Test scenario 6: Check when monPrms is NULL.
 * 7)Test scenario 7: Check when ringMonInitDone is UDMA_DEINIT_DONE
 *                    and ringMonNum is UDMA_RING_MON_INVALID.
 * 8)Test scenario 8: Check when ringMonInitDone is UDMA_DEINIT_DONE.
 * 9)Test scenario 9: Check when ringMonNum is UDMA_RING_MON_INVALID.
 * 10)Test scenario 10: Check when driver Handle is NULL.
 * 11)Test scenario 11: Check when drvInitDone is UDMA_DEINIT_DONE.
 * 12)Test scenario 12: Check when instType is not UDMA_INST_TYPE_NORMAL.
 * 13)Test scenario 13: To get print error message [Error] Ring monitor
 *                      config failed
 */
int32_t UdmaTestRingMonConfigNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    struct Udma_RingMonObj ringMonObj;
    Udma_RingMonHandle     monHandle;
    Udma_RingMonPrms       monPrms;
    uint32_t               backUpDrvInitDone;
    uint32_t               instId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ringMonConfig negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when monHandle and monPrms are NULL */
    monHandle = (Udma_RingMonHandle) NULL_PTR;
    retVal    = Udma_ringMonConfig(monHandle, NULL);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                  " Check when monHandle and monPrms are NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when monHandle is NULL */
        retVal = Udma_ringMonConfig(monHandle, &monPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                      " Check when monHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when ringMonInitDone is UDMA_DEINIT_DONE,
         *                  ringMonNum is UDMA_RING_MON_INVALID and monPrms
         *                  is NULL
         */
        monHandle                  = &ringMonObj;
        monHandle->ringMonInitDone = UDMA_DEINIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_INVALID;
        retVal                     = Udma_ringMonConfig(monHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                      " Check when ringMonInitDone is UDMA_DEINIT_DONE,"
                      " ringMonNum is UDMA_RING_MON_INVALID and monPrms is"
                      " NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when ringMonInitDone is UDMA_INIT_DONE
         *                  and monPrms is NULL
         */
        monHandle->ringMonInitDone = UDMA_DEINIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_ANY;
        retVal                     = Udma_ringMonConfig(monHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                      " Check when ringMonInitDone is UDMA_INIT_DONE and"
                      " monPrms is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when monPrms is NULL and ringMonNum is
         *                  UDMA_RING_MON_INVALID
         */
        monHandle->ringMonInitDone = UDMA_INIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_INVALID;
        retVal                     = Udma_ringMonConfig(monHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                      " Check when monPrms is NULL and ringMonNum is"
                      " UDMA_RING_MON_INVALID!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check when monPrms is NULL */
        monHandle->ringMonInitDone = UDMA_INIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_ANY;
        retVal                     = Udma_ringMonConfig(monHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                      " Check when monPrms is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 7: Check when ringMonInitDone is UDMA_DEINIT_DONE and
         *                  ringMonNum is UDMA_RING_MON_INVALID
         */
        monHandle->ringMonInitDone = UDMA_DEINIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_INVALID;
        retVal                     = Udma_ringMonConfig(monHandle, &monPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                      " Check when ringMonInitDone is UDMA_DEINIT_DONE and"
                      " ringMonNum is UDMA_RING_MON_INVALID!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 8: Check when ringMonInitDone is UDMA_DEINIT_DONE */
        monHandle->ringMonInitDone = UDMA_DEINIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_ANY;
        retVal                     = Udma_ringMonConfig(monHandle, &monPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                      " Check when ringMonInitDone is UDMA_DEINIT_DONE\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 9: Check when ringMonNum is UDMA_RING_MON_INVALID */
        monHandle->ringMonInitDone = UDMA_INIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_INVALID;
        retVal                     = Udma_ringMonConfig(monHandle, &monPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                      " Check when ringMonNum is UDMA_RING_MON_INVALID\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 10: Check when driver Handle is NULL */
        monHandle->ringMonInitDone = UDMA_INIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_ANY;
        monHandle->drvHandle       = NULL_PTR;
        retVal                     = Udma_ringMonConfig(monHandle, &monPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                      " Check when driver Handle is NULL\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 11: Check when drvInitDone is UDMA_DEINIT_DONE */
        monHandle->ringMonInitDone        = UDMA_INIT_DONE;
        monHandle->ringMonNum             = UDMA_RING_MON_ANY;
        instId                            = UDMA_TEST_INST_ID_MAIN_0;
        monHandle->drvHandle              = &taskObj->testObj->drvObj[instId];
        backUpDrvInitDone                 = monHandle->drvHandle->drvInitDone;
        monHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                            = Udma_ringMonConfig(monHandle, &monPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                      " Check when drvInitDone is UDMA_DEINIT_DONE\n");
            retVal = UDMA_EFAIL;
        }
        monHandle->drvHandle->drvInitDone = backUpDrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 12: Check when instType is not UDMA_INST_TYPE_NORMAL */
        instId               = UDMA_TEST_INST_ID_BCDMA_0;
        monHandle->drvHandle = &taskObj->testObj->drvObj[instId];
        retVal               = Udma_ringMonConfig(monHandle, &monPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                      " Check when instType is not UDMA_INST_TYPE_NORMAL\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 13: To get print error message [Error] Ring monitor
         *                   config failed
         */
        instId                = UDMA_TEST_INST_ID_MAIN_0;
        monHandle->drvHandle  = &taskObj->testObj->drvObj[instId];
        monHandle->ringMonNum = UDMA_RING_INVALID - 1U;
        retVal                = Udma_ringMonConfig(monHandle, &monPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonConfig:: Neg::"
                      " to get print error message [Error] Ring monitor"
                      " config failed\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringMonGetData when
 * 1)Test scenario 1: Check when monHandle and monData are NULL.
 * 2)Test scenario 2: Check when monHandle is NULL.
 * 3)Test scenario 3: Check when ringMonInitDone is UDMA_DEINIT_DONE,
 *                    ringMonNum is UDMA_RING_MON_INVALID and monData
 *                    is NULL.
 * 4)Test scenario 4: Check when ringMonInitDone is UDMA_INIT_DONE and
 *                    monData is NULL.
 * 5)Test scenario 5: Check when monData is NULL and ringMonNum is
 *                    UDMA_RING_MON_INVALID.
 * 6)Test scenario 6: Check when monData is NULL.
 * 7)Test scenario 7: Check when ringMonInitDone is UDMA_DEINIT_DONE
 *                    and ringMonNum is UDMA_RING_MON_INVALID.
 * 8)Test scenario 8: Check when ringMonInitDone is UDMA_DEINIT_DONE.
 * 9)Test scenario 9: Check when ringMonNum is UDMA_RING_MON_INVALID.
 * 10)Test scenario 10: Check when driver Handle is NULL.
 * 11)Test scenario 11: Check when drvInitDone is UDMA_DEINIT_DONE
 * 12)Test scenario 12: Check when instType is not UDMA_INST_TYPE_NORMAL.
 */
int32_t UdmaTestRingMonGetDataNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    struct Udma_RingMonObj ringMonObj;
    Udma_RingMonHandle     monHandle;
    uint32_t               backUpDrvInitDone;
    Udma_RingMonData       monData;
    uint32_t               instId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ring Mon Get Data negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when monHandle and monData are NULL */
    monHandle = (Udma_RingMonHandle) NULL_PTR;
    retVal    = Udma_ringMonGetData(monHandle, NULL);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringMonGetData:: Neg::"
                  " Check when monHandle and monData are NULL\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when monHandle is NULL */
        monHandle = (Udma_RingMonHandle) NULL_PTR;
        retVal    = Udma_ringMonGetData(monHandle, &monData);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonGetData:: Neg::"
                      " Check when monHandle is NULL\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when ringMonInitDone is UDMA_DEINIT_DONE,
         *                  ringMonNum is UDMA_RING_MON_INVALID and monData
         *                  is NULL
         */
        monHandle                  = &ringMonObj;
        monHandle->ringMonInitDone = UDMA_DEINIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_INVALID;
        retVal                     = Udma_ringMonGetData(monHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonGetData:: Neg::"
                      " Check when ringMonInitDone is UDMA_DEINIT_DONE,"
                      " ringMonNum is UDMA_RING_MON_INVALID and monData is"
                      " NULL\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when ringMonInitDone is UDMA_DEINIT_DONE and
         *                  monData is NULL
         */
        monHandle->ringMonNum = UDMA_RING_MON_ANY;
        retVal                = Udma_ringMonGetData(monHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonGetData:: Neg::"
                      " Check when ringMonInitDone is UDMA_DEINIT_DONE and"
                      " monData is NULL\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when monData is NULL and ringMonNum is
         *                  UDMA_RING_MON_INVALID
         */
        monHandle->ringMonInitDone = UDMA_INIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_INVALID;
        retVal                     = Udma_ringMonGetData(monHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonGetData:: Neg::"
                      " Check when monData is NULL and ringMonNum is "
                      " UDMA_RING_MON_INVALID\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check when monData is NULL */
        monHandle->ringMonNum = UDMA_RING_MON_ANY;
        retVal                = Udma_ringMonGetData(monHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonGetData:: Neg::"
                      " Check when monData is NULL\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 7: Check when ringMonInitDone is UDMA_DEINIT_DONE and
         *                  ringMonNum is UDMA_RING_MON_INVALID
         */
        monHandle->ringMonInitDone = UDMA_DEINIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_INVALID;
        retVal                     = Udma_ringMonGetData(monHandle, &monData);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonGetData:: Neg::"
                      " Check when ringMonInitDone is UDMA_DEINIT_DONE and"
                      " ringMonNum is UDMA_RING_MON_INVALID\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 8: Check when ringMonInitDone is UDMA_DEINIT_DONE */
        monHandle->ringMonNum = UDMA_RING_MON_ANY;
        retVal                = Udma_ringMonGetData(monHandle, &monData);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonGetData:: Neg::"
                      " Check when ringMonInitDone is UDMA_DEINIT_DONE\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 9: Check when ringMonNum is UDMA_RING_MON_INVALID */
        monHandle->ringMonInitDone = UDMA_INIT_DONE;
        monHandle->ringMonNum      = UDMA_RING_MON_INVALID;
        retVal                     = Udma_ringMonGetData(monHandle, &monData);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonGetData:: Neg::"
                      " Check when ringMonNum is UDMA_RING_MON_INVALID\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 10: Check when driver Handle is NULL */
        monHandle->ringMonNum = UDMA_RING_MON_ANY;
        monHandle->drvHandle  = NULL_PTR;
        retVal                = Udma_ringMonGetData(monHandle, &monData);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonGetData:: Neg::"
                      " Check when driver Handle is NULL\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 11: Check when drvInitDone is UDMA_DEINIT_DONE */
        instId                            = UDMA_TEST_INST_ID_MAIN_0;
        monHandle->drvHandle              = &taskObj->testObj->drvObj[instId];
        backUpDrvInitDone                 = monHandle->drvHandle->drvInitDone;
        monHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                            = Udma_ringMonGetData(monHandle, &monData);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonGetData:: Neg::"
                      " Check when drvInitDone is UDMA_DEINIT_DONE\n");
            retVal = UDMA_EFAIL;
        }
        monHandle->drvHandle->drvInitDone = backUpDrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 12: Check when instType is not UDMA_INST_TYPE_NORMAL */
        instId               = UDMA_TEST_INST_ID_BCDMA_0;
        monHandle->drvHandle = &taskObj->testObj->drvObj[instId];
        retVal               = Udma_ringMonGetData(monHandle, &monData);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonGetData:: Neg::"
                      " Check when instType is not UDMA_INST_TYPE_NORMAL\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_ringMonGetNum when
 * 1) Test scenario 1: Check when monHandle is NULL.
 * 2) Test scenario 2: Check when ringMonInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestRingMonGetNumNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    uint32_t               ringMonNum;
    struct Udma_RingMonObj ringMonObj;
    Udma_RingMonHandle     monHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ringMonGetNum negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when monHandle is NULL */
    monHandle  = (Udma_RingMonHandle) NULL_PTR;
    ringMonNum = Udma_ringMonGetNum(monHandle);
    if(UDMA_RING_MON_INVALID == ringMonNum)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: ringMonGetNum:: Neg::"
                  " Check when monHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when ringMonInitDone is UDMA_DEINIT_DONE */
        monHandle                  = &ringMonObj;
        monHandle->ringMonInitDone = UDMA_DEINIT_DONE;
        ringMonNum                 = Udma_ringMonGetNum(monHandle);
        if(UDMA_RING_MON_INVALID == ringMonNum)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ringMonGetNum:: Neg::"
                      " Check when ringMonInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

