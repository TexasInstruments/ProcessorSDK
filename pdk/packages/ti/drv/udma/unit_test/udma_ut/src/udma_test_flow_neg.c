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
 *  \file udma_test_flow_neg.c
 *
 *  \brief File containing negative test cases for UDMA flow APIs.
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
 * Test Case Description: Verifies the function Udma_flowGetNum when
 * 1)Test scenario 1: Check when flowHandle is NULL.
 * 2)Test scenario 2: Check when flowInitDone is not UDMA_INIT_DONE.
 */
int32_t UdmaTestFlowGetNumNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal  = UDMA_SOK;
    uint32_t            flowNum = UDMA_FLOW_INVALID;
    struct Udma_FlowObj flowObj;
    Udma_FlowHandle     flowHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Flow get Num negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when flowHandle is NULL */
    flowHandle = (Udma_FlowHandle) NULL_PTR;
    flowNum    = Udma_flowGetNum(flowHandle);
    if(UDMA_FLOW_INVALID == flowNum)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: flowGetNum:: Neg:: "
                  " Check when flowHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when flowInitDone is not UDMA_INIT_DONE */
        flowHandle               = &flowObj;
        flowHandle->flowInitDone = UDMA_DEINIT_DONE;
        flowNum                  = Udma_flowGetNum(flowHandle);
        if(UDMA_FLOW_INVALID == flowNum)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: flowGetNum:: Neg:: "
                      " Check when flowInitDone is not UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_flowGetCount when
 * 1)Test scenario 1: Check when flowHandle is NULL.
 * 2)Test scenario 2: Check when flowInitDone is not UDMA_INIT_DONE.
 */
int32_t UdmaTestFlowGetCountNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal  = UDMA_SOK;
    uint32_t            flowCnt = UDMA_FLOW_INVALID;
    struct Udma_FlowObj flowObj;
    Udma_FlowHandle     flowHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Flow get count negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when flowHandle is NULL */
    flowHandle = (Udma_FlowHandle) NULL_PTR;
    flowCnt    = Udma_flowGetCount(flowHandle);
    if(UDMA_FLOW_INVALID == flowCnt)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: flowGetCount:: Neg:: "
                  " Check when flowHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when flowInitDone is not UDMA_INIT_DONE */
        flowHandle               = &flowObj;
        flowHandle->flowInitDone = UDMA_DEINIT_DONE;
        flowCnt                  = Udma_flowGetCount(flowHandle);
        if(UDMA_FLOW_INVALID == flowCnt)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: flowGetCount:: Neg:: "
                      " Check when flowInitDone is not UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_flowFree when
 * 1)Test scenario 1: Check when flowHandle is NULL.
 * 2)Test scenario 2: Check when flowInitDone is not UDMA_INIT_DONE.
 * 3)Test scenario 3: Check when driver handle is NULL.
 * 4)Test scenario 4: Check when drvInitDone is not UDMA_INIT_DONE.
 * 5)Test scenario 5: Check when instType is not UDMA_INST_TYPE_NORMAL.
 * 6)Test scenario 6: Check when mappedFlowGrp is not UDMA_MAPPED_GROUP_INVALID.
 */
int32_t UdmaTestFlowFreeNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal  = UDMA_SOK;
    Udma_FlowHandle     flowHandle;
    struct Udma_FlowObj flowObj;
    uint32_t            backUpDrvInitDone;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Flow free negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when flowHandle is NULL */
    flowHandle = (Udma_FlowHandle) NULL_PTR;
    retVal     = Udma_flowFree(flowHandle);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: flowFree:: Neg:: "
                  " Check when flowHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when flowInitDone is not UDMA_INIT_DONE */
        flowHandle               = &flowObj;
        flowHandle->flowInitDone = UDMA_DEINIT_DONE;
        retVal                   = Udma_flowFree(flowHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: flowFree:: Neg:: "
                      " Check when flowInitDone is not UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when driver handle is NULL */
        flowHandle               = &flowObj;
        flowHandle->flowInitDone = UDMA_INIT_DONE;
        flowHandle->drvHandle    = NULL_PTR;
        retVal                   = Udma_flowFree(flowHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: flowFree:: Neg:: "
                      " Check when driver handle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when drvInitDone is not UDMA_INIT_DONE */
        flowHandle->drvHandle              = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        backUpDrvInitDone                  = flowHandle->drvHandle->drvInitDone;
        flowHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                             = Udma_flowFree(flowHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: flowFree:: Neg:: "
                      " Check when drvInitDone is not UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        flowHandle->drvHandle->drvInitDone = backUpDrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when instType is not UDMA_INST_TYPE_NORMAL */
        flowHandle->mappedFlowGrp = UDMA_MAPPED_GROUP_INVALID;
        flowHandle->drvHandle     = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];
        retVal                    = Udma_flowFree(flowHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: flowFree:: Neg:: "
                      " Check when mappedFlowGrp is not UDMA_INST_TYPE_NORMAL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check when mappedFlowGrp is not UDMA_MAPPED_GROUP_INVALID */
        flowHandle                = &flowObj;
        flowHandle->flowInitDone  = UDMA_INIT_DONE;
        flowHandle->drvHandle     = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        flowHandle->mappedFlowGrp = UDMA_UTC_ID_INVALID;
        retVal                    = Udma_flowFree(flowHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: flowFree :: Neg:: "
                      " Check when mappedFlowGrp is not "
                      " UDMA_MAPPED_GROUP_INVALID!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function UdmaFlowPrms_init when
 * 1)Test scenario 1: Check when flowPrms is NULL.
 */
int32_t UdmaTestFlowPrmsInitNeg(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal = UDMA_SOK;
    uint32_t chType = UDMA_CH_TYPE_RX;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Flow prms init negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when flowPrms is NULL */
    Udma_FlowPrms *flowPrms = (Udma_FlowPrms*) NULL_PTR;
    retVal                  = UdmaFlowPrms_init(flowPrms, chType);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA::FlowPrms_init:: Neg:: "
                  " Check when flowPrms is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_flowDetach when
 * 1)Test scenario 1: Check when flowHandle is NULL.
 * 2)Test scenario 2: Check when flowInitDone is not UDMA_INIT_DONE.
 */
int32_t UdmaTestFlowDetachNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
    Udma_FlowHandle     flowHandle;
    struct Udma_FlowObj flowObj;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Flow detach negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when flowHandle is NULL */
    flowHandle = (Udma_FlowHandle) NULL_PTR;
    retVal     = Udma_flowDetach(flowHandle);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Flow detach:: Neg:: "
                  " Check when flowHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when flowInitDone is not UDMA_INIT_DONE */
        flowHandle               = &flowObj;
        flowHandle->flowInitDone = UDMA_DEINIT_DONE;
        retVal                   = Udma_flowDetach(flowHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow detach:: Neg:: "
                      " Check when flowInitDone is not UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_flowAttach when
 * 1)Test scenario 1: Check when drvHandle and flowHandle are NULL, flowCnt
 *                    is zero.
 * 2)Test scenario 2: Check when drvHandle and flowHandle are NULL.
 * 3)Test scenario 3: Check when driver handle is NULL and flowCnt is zero.
 * 4)Test scenario 4: Check when driver handle is NULL.
 * 5)Test scenario 5: Check when flowHandle is NULL and flowCnt is zero.
 * 6)Test scenario 6: Check when flowHandle is NULL.
 * 7)Test scenario 7: Check when flowCnt is zero.
 * 8)Test scenario 8: Check when drvInitDone is not UDMA_INIT_DONE.
 */
int32_t UdmaTestFlowAttachNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal    = UDMA_SOK;
    uint32_t            flowStart = 0U;
    uint32_t            flowCnt;
    Udma_DrvHandle      drvHandle;
    struct Udma_FlowObj flowObj;
    Udma_FlowHandle     flowHandle;
    uint32_t            backUpDrvInitDone;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Flow attach negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when drvHandle and flowHandle are NULL, flowCnt
     *                  is zero
     */
    drvHandle  = (Udma_DrvHandle) NULL_PTR;
    flowHandle = (Udma_FlowHandle) NULL_PTR;
    flowCnt    = 0U;
    retVal     = Udma_flowAttach(drvHandle, flowHandle, flowStart, flowCnt);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Flow Attach:: Neg:: "
                  " Check when drvHandle and flowHandle are NULL,flowCnt "
                  " is zero!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when drvHandle and flowHandle are NULL */
        drvHandle  = (Udma_DrvHandle) NULL_PTR;
        flowHandle = (Udma_FlowHandle) NULL_PTR;
        flowCnt    = 1U;
        retVal     = Udma_flowAttach(drvHandle, flowHandle, flowStart, flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Attach:: Neg:: "
                      " Check when drvHandle and flowHandle are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when driver handle is NULL and flowCnt is zero */
        drvHandle  = (Udma_DrvHandle) NULL_PTR;
        flowHandle = &flowObj;
        flowCnt    = 0U;
        retVal     = Udma_flowAttach(drvHandle, flowHandle, flowStart, flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Attach:: Neg:: "
                      " Check when driver handle is NULL and flowCnt is zero!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when driver handle is NULL */
        drvHandle  = (Udma_DrvHandle) NULL_PTR;
        flowHandle = &flowObj;
        flowCnt    = 1U;
        retVal     = Udma_flowAttach(drvHandle, flowHandle, flowStart, flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Attach:: Neg:: "
                      " Check when driver handle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when flowHandle is NULL and flowCnt is zero */
        drvHandle  = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        flowHandle = (Udma_FlowHandle) NULL_PTR;
        flowCnt    = 0U;
        retVal     = Udma_flowAttach(drvHandle, flowHandle, flowStart, flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Attach:: Neg:: "
                      " Check when flowHandle is NULL and flowCnt is zero!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check when flowHandle is NULL */
        drvHandle  = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        flowHandle = (Udma_FlowHandle) NULL_PTR;
        flowCnt    = 1U;
        retVal     = Udma_flowAttach(drvHandle, flowHandle, flowStart, flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Attach:: Neg:: "
                      " Check when flowHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 7: Check when flowCnt is zero */
        drvHandle  = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        flowHandle = &flowObj;
        flowCnt    = 0U;
        retVal     = Udma_flowAttach(drvHandle, flowHandle, flowStart, flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Attach:: Neg:: "
                      " Check when flowCnt is zero!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 8: Check when drvInitDone is not UDMA_INIT_DONE */
        flowHandle             = &flowObj;
        flowCnt                = 1U;
        backUpDrvInitDone      = drvHandle->drvInitDone;
        drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                 = Udma_flowAttach(drvHandle, flowHandle, flowStart,
                                                 flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Attach:: Neg:: Test:: "
                      " Check when drvInitDone is not UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        drvHandle->drvInitDone = backUpDrvInitDone;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_flowAlloc when
 * 1)Test scenario 1: Check when drvHandle and flowHandle are NULL,
 *                    flowCnt is zero.
 * 2)Test scenario 2: Check when driver Handle and flowHandle are NULL.
 * 3)Test scenario 3: Check when driver handle is NULL and flowCnt is zero.
 * 4)Test scenario 4: Check when driver handle is NULL.
 * 5)Test scenario 5: Check when flowHandle is NULL and flowCnt is zero.
 * 6)Test scenario 6: Check when flowHandle is NULL.
 * 7)Test scenario 7: Check when flowCnt is zero.
 * 8)Test scenario 8: Check when drvInitDone is not UDMA_INIT_DONE.
 */
int32_t UdmaTestFlowAllocNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            flowCnt;
    Udma_DrvHandle      drvHandle;
    struct Udma_FlowObj flowObj;
    Udma_FlowHandle     flowHandle;
    uint32_t            backUpDrvInitDone;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Flow alloc negative Testcase ::\r\n",
              taskObj->taskId);

    /*
     * Test scenario 1: Check when driver Handle and flowHandle are NULL,
     *                  flowCnt is zero
     */
    drvHandle  = (Udma_DrvHandle) NULL_PTR;
    flowHandle = (Udma_FlowHandle) NULL_PTR;
    flowCnt    = 0U;
    retVal     = Udma_flowAlloc(drvHandle, flowHandle, flowCnt);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Flow alloc:: Neg:: "
                  " Check when driver Handle and flowHandle are NULL,flowCnt is "
                  " zero!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when driver Handle and flowHandle are NULL */
        drvHandle  = (Udma_DrvHandle) NULL_PTR;
        flowHandle = (Udma_FlowHandle) NULL_PTR;
        flowCnt    = 1U;
        retVal     = Udma_flowAlloc(drvHandle, flowHandle, flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow alloc:: Neg:: "
                      " Check when driver Handle and flowHandle are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when driver handle is NULL and flowCnt is zero */
        drvHandle  = (Udma_DrvHandle) NULL_PTR;
        flowHandle = &flowObj;
        flowCnt    = 0U;
        retVal     = Udma_flowAlloc(drvHandle, flowHandle, flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow alloc:: Neg:: "
                      " Check when driver handle is NULL and flowCnt is zero!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when driver handle is NULL */
        drvHandle = (Udma_DrvHandle) NULL_PTR;
        flowCnt   = 1U;
        retVal    = Udma_flowAlloc(drvHandle, flowHandle, flowCnt);
        if(UDMA_SOK != retVal)
        {
           retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow alloc:: Neg:: "
                      " Check when driver handle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when flowHandle is NULL and flowCnt is zero */
        drvHandle  = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        flowHandle = (Udma_FlowHandle) NULL_PTR;
        flowCnt    = 0U;
        retVal     = Udma_flowAlloc(drvHandle, flowHandle, flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow alloc:: Neg:: "
                      " Check when flowHandle is NULL and flowCnt is zero!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check when flowHandle is NULL */
        flowHandle = (Udma_FlowHandle) NULL_PTR;
        flowCnt    = 1U;
        retVal     = Udma_flowAlloc(drvHandle, flowHandle, flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow alloc:: Neg:: "
                      " Check when flowHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 7: Check when flowCnt is zero */
        drvHandle  = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        flowHandle = &flowObj;
        flowCnt    = 0U;
        retVal     = Udma_flowAlloc(drvHandle, flowHandle, flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow alloc:: Neg:: "
                      " Check when flowCnt is zero!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 8: Check when drvInitDone is not UDMA_INIT_DONE */
        flowHandle             = &flowObj;
        flowCnt                = 1U;
        backUpDrvInitDone      = drvHandle->drvInitDone;
        drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                 = Udma_flowAlloc(drvHandle, flowHandle, flowCnt);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow alloc:: Neg:: "
                      " Check when drvInitDone is not UDMA_INIT_DONE \n");
            retVal = UDMA_EFAIL;
        }
        drvHandle->drvInitDone = backUpDrvInitDone;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function UdmaTest_flowConfig when
 * 1)Test scenario 1: Check when flowHandle is NULL and flow params.
 * 2)Test scenario 2: Check when flowHandle is NULL.
 * 3)Test scenario 3: Check when flowInitDone is it not UDMA_INIT_DONE and
 *                    flowPrms is NULL.
 * 4)Test scenario 4: Check when flowInitDone is it not UDMA_INIT_DONE.
 * 5)Test scenario 5: Check when flowPrms is NULL.
 * 6)Test scenario 6: Check when driver handle is NULL.
 * 7)Test scenario 7: Check when drvInitDone is not UDMA_INIT_DONE.
 * 8)Test scenario 8: To get error print message as [Error] Invalid flow index.
 * 9)Test scenario 9: To get sciclient error message [Error] UDMA RX flow config failed!!
 *                    and [Error] UDMA RX flow threshold config failed!!.
 */
int32_t UdmaTestFlowConfigNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal  = UDMA_SOK;
    uint32_t            flowIdx = 1U;
    Udma_FlowHandle     flowHandle;
    Udma_FlowPrms       flowPrms;
    struct Udma_FlowObj flowObj;
    struct Udma_DrvObj  backUpDrvObj;
    uint32_t            backUpDrvInitDone;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Flow Config negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when flowHandle is NULL and flow params */
    flowHandle = (Udma_FlowHandle) NULL_PTR;
    retVal     = Udma_flowConfig(flowHandle, flowIdx, NULL);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Flow Config:: Neg:: "
                  " Check when flowHandle is NULL and flow params!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when flowHandle is NULL */
        flowHandle = (Udma_FlowHandle) NULL_PTR;
        retVal     = Udma_flowConfig(flowHandle, flowIdx, &flowPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Config:: Neg:: "
                      " Check when flowHandle is NULL!! \n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /*
         * Test scenario 3: Check when flowInitDone is it not UDMA_INIT_DONE and
         *                  flowPrms is NULL
         */
        flowHandle               = &flowObj;
        flowHandle->flowInitDone = UDMA_DEINIT_DONE;
        retVal                   = Udma_flowConfig(flowHandle, flowIdx, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Config:: Neg:: "
                      " Check when flowInitDone is it not UDMA_INIT_DONE "
                      " and flowPrms is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when flowInitDone is it not UDMA_INIT_DONE */
        flowHandle               = &flowObj;
        flowHandle->flowInitDone = UDMA_DEINIT_DONE;
        retVal                   = Udma_flowConfig(flowHandle, flowIdx, &flowPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Config:: Neg:: "
                      " Check when flowInitDone is it not UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when flowPrms is NULL */
        flowHandle               = &flowObj;
        flowHandle->flowInitDone = UDMA_INIT_DONE;
        retVal                   = Udma_flowConfig(flowHandle, flowIdx, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Config:: Neg:: "
                      " Check when flowPrms is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check when driver handle is NULL */
        UdmaFlowPrms_init(&flowPrms, UDMA_CH_TYPE_RX);
        flowHandle->drvHandle = NULL_PTR;
        retVal                = Udma_flowConfig(flowHandle, flowIdx, &flowPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Config:: Neg:: "
                      " Check when driver handle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 7: Check when drvInitDone is not UDMA_INIT_DONE */
        flowHandle->drvHandle              = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        backUpDrvInitDone                  = flowHandle->drvHandle->drvInitDone;
        flowHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                             = Udma_flowConfig(flowHandle, flowIdx,
                                                             &flowPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Config:: Neg:: "
                      " Check when drvInitDone is not UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        flowHandle->drvHandle->drvInitDone = backUpDrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 8: To get error print message as [Error] Invalid flow index */
        flowIdx = flowHandle->flowCnt + 1U;
        retVal  = Udma_flowConfig(flowHandle, flowIdx, &flowPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Config:: Neg:: "
                      " To get error print message as [Error] Invalid flow index!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 9: To get sciclient error message [Error] UDMA RX flow config failed!!
         *                  and [Error] UDMA RX flow threshold config failed!!
         */
        backUpDrvObj                     = taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        flowHandle->drvHandle            = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        flowHandle->flowCnt              = flowIdx + 1U;
        flowHandle->drvHandle->devIdUdma = UDMA_DEFAULT_FLOW_ID;
        retVal                           = Udma_flowConfig(flowHandle, flowIdx, &flowPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Flow Config:: Neg:: "
                      " To get sciclient error message [Error] UDMA RX flow config failed!! "
                      " and [Error] UDMA RX flow threshold config failed!!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0] = backUpDrvObj;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_flowAlloc when
 * 1)Test scenario 1: Check To get freeFlowCnt as 0
 */
int32_t UdmaTestFlowAllocfreeFlowCntNeg(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            flowCnt;
    Udma_DrvHandle      drvHandle;
    struct Udma_FlowObj flowObj;
    Udma_FlowHandle     flowHandle;
    struct Udma_DrvObj  backUpDrvObj;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Flow alloc Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check To get freeFlowCnt as 0 */
    flowHandle                 = &flowObj;
    backUpDrvObj               = taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
    drvHandle                  = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
    /* Check for mcu2_1 core which has 8 flows allocated to it */
    flowCnt                    = 8U;
    drvHandle->freeFlowFlag[0] = 0U;
    retVal                     = Udma_flowAlloc(drvHandle, flowHandle, flowCnt);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Flow alloc:: Neg:: "
                  " Check to get freeFlowCnt as 0!!\n");
        retVal = UDMA_EFAIL;
    }
    taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0] = backUpDrvObj;

    return retVal;
}

