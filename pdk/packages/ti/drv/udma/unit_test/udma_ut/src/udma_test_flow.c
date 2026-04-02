/*
 *  Copyright (c) Texas Instruments Incorporated 2019-2022
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
 *  \file udma_test_flow.c
 *
 *  \brief UDMA flow related test case file.
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

static int32_t udmaTestFlowAttachTestLoop(UdmaTestTaskObj *taskObj);
static int32_t udmaTestFlowAttachMappedTestLoop(UdmaTestTaskObj *taskObj);
static int32_t udmaTestFlowAllocTestLoop(UdmaTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Global test pass/fail flag */
static volatile int32_t gUdmaTestFlowResult = UDMA_SOK;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t udmaTestFlowAttachTc(UdmaTestTaskObj *taskObj)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    loopCnt = 0U;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Flow Attach/Detach Testcase ::\r\n", taskObj->taskId);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Loop count           : %d ::\r\n", taskObj->taskId, taskObj->loopCnt);

    gUdmaTestFlowResult = UDMA_SOK;
    while(loopCnt < taskObj->loopCnt)
    {
        /* Perform flow attach test */
        retVal = udmaTestFlowAttachTestLoop(taskObj);
        if(UDMA_SOK != retVal)
        {
            break;
        }

        loopCnt++;
    }

    retVal += gUdmaTestFlowResult;

    return (retVal);
}

int32_t udmaTestFlowAttachMappedTc(UdmaTestTaskObj *taskObj)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    loopCnt = 0U;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Mapped Flow Attach/Detach Testcase ::\r\n", taskObj->taskId);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Loop count           : %d ::\r\n", taskObj->taskId, taskObj->loopCnt);

    gUdmaTestFlowResult = UDMA_SOK;
    while(loopCnt < taskObj->loopCnt)
    {
        /* Perform mapped flow attach test */
        retVal = udmaTestFlowAttachMappedTestLoop(taskObj);
        if(UDMA_SOK != retVal)
        {
            break;
        }

        loopCnt++;
    }

    retVal += gUdmaTestFlowResult;

    return (retVal);
}

int32_t udmaTestFlowAllocTc(UdmaTestTaskObj *taskObj)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    loopCnt = 0U;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Flow Alloc/Free Testcase ::\r\n", taskObj->taskId);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Loop count           : %d ::\r\n", taskObj->taskId, taskObj->loopCnt);

    gUdmaTestFlowResult = UDMA_SOK;
    while(loopCnt < taskObj->loopCnt)
    {
        /* Perform flow alloc test */
        retVal = udmaTestFlowAllocTestLoop(taskObj);
        if(UDMA_SOK != retVal)
        {
            break;
        }

        loopCnt++;
    }

    retVal += gUdmaTestFlowResult;

    return (retVal);
}

static int32_t udmaTestFlowAttachTestLoop(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            instId, flowIdx;
    uint32_t            elemCnt = 100U, ringMemSize;
    uint32_t            flowCnt = 3U, flowStart, flowCntTest;
    uint32_t            ringAllocated = UFALSE, flowAllocated = UFALSE;
    uint16_t            ringNum;
    uint32_t            heapId = UTILS_MEM_HEAP_ID_MSMC;
    Udma_DrvHandle      drvHandle;
    Udma_RingPrms       ringPrms;
    struct Udma_RingObj ringObj;
    Udma_RingHandle     ringHandle = &ringObj;
    struct Udma_FlowObj flowObj, attachFlowObj;
    Udma_FlowHandle     flowHandle = &flowObj, attachFlowHandle = &attachFlowObj;
    Udma_FlowPrms       flowPrms;
    void               *ringMem = NULL;

    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }

    if(UDMA_SOK == retVal)
    {
        instId = UDMA_TEST_DEFAULT_UDMA_INST;
        drvHandle = &taskObj->testObj->drvObj[instId];

        UdmaRingPrms_init(&ringPrms);
        ringPrms.ringMem = ringMem;
        ringPrms.ringMemSize = ringMemSize;
        ringPrms.mode = TISCI_MSG_VALUE_RM_RING_MODE_MESSAGE;
        ringPrms.elemCnt = elemCnt;

        /* Allocate a free ring */
        retVal = Udma_ringAlloc(drvHandle, ringHandle, UDMA_RING_ANY, &ringPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring alloc failed!!\n");
        }
        else
        {
            ringAllocated = UTRUE;
        }

        if(UDMA_SOK == retVal)
        {
            /* Allocate free flows */
            retVal = Udma_flowAlloc(drvHandle, flowHandle, flowCnt);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Flow alloc failed!!\n");
            }
            else
            {
                flowAllocated = UTRUE;
            }
        }

        if(UDMA_SOK == retVal)
        {
            flowStart = Udma_flowGetNum(flowHandle);
            if(UDMA_FLOW_INVALID == flowStart)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Invalid flow ID!!\n");
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            flowCntTest = Udma_flowGetCount(flowHandle);
            if(flowCntTest != flowCnt)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Flow count doesn't match!!\n");
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Attach and configure the flows */
            retVal = Udma_flowAttach(drvHandle, attachFlowHandle, flowStart, flowCnt);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Flow attach failed!!\n");
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Do flow config with the allocated ring - we can't do any
             * other functional test in standalone testing */
            ringNum = Udma_ringGetNum(ringHandle);
            UdmaFlowPrms_init(&flowPrms, UDMA_CH_TYPE_RX);
            flowPrms.defaultRxCQ = ringNum;
            flowPrms.fdq0Sz0Qnum = ringNum;
            flowPrms.fdq1Qnum    = ringNum;
            flowPrms.fdq2Qnum    = ringNum;
            flowPrms.fdq3Qnum    = ringNum;
            flowPrms.fdq0Sz1Qnum = ringNum;
            flowPrms.fdq0Sz2Qnum = ringNum;
            flowPrms.fdq0Sz3Qnum = ringNum;
            for(flowIdx = 0U; flowIdx < flowCnt; flowIdx++)
            {
                retVal = Udma_flowConfig(attachFlowHandle, flowIdx, &flowPrms);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Flow config failed!!\n");
                }
            }
        }

        if(UDMA_SOK == retVal)
        {
            retVal = Udma_flowDetach(attachFlowHandle);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Flow detach failed!!\n");
            }
        }

        /* Free allocated flows and rings */
        if(UTRUE == flowAllocated)
        {
            retVal += Udma_flowFree(flowHandle);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Flow free failed!!\n");
            }
        }

        if(UTRUE == ringAllocated)
        {
            retVal += Udma_ringFree(ringHandle);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
            }
        }
    }

    if(NULL != ringMem)
    {
        retVal += Utils_memFree(heapId, ringMem, ringMemSize);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
        }
    }

    return (retVal);
}

static int32_t udmaTestFlowAttachMappedTestLoop(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    uint32_t            instId, mappedFlowGrp;;
    uint32_t            flowCnt = 1U, mappepdFlowNum, flowCntTest;
    uint32_t            numMappedFlowGrp;
    uint32_t            mappedFlowAllocated = UFALSE;
    Udma_DrvHandle      drvHandle;
    struct Udma_FlowObj flowObj, attachFlowObj;
    Udma_FlowHandle     flowHandle = &flowObj, attachFlowHandle = &attachFlowObj;
    Udma_FlowPrms       flowPrms;
    Udma_FlowAllocMappedPrms    flowAllocMappedPrms;
    Udma_RmInitPrms             *rmInitPrms;
    char *mappedFlowGrpStr[] = { "CPSW RX", "SAUL RX", "ICSSG0 RX", "ICSSG1_RX"};

    if(UDMA_SOK == retVal)
    {
        instId = UDMA_TEST_INST_ID_FLOW;
        drvHandle = &taskObj->testObj->drvObj[instId];

        numMappedFlowGrp = UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP;

        for(mappedFlowGrp = UDMA_NUM_MAPPED_TX_GROUP; mappedFlowGrp < numMappedFlowGrp; mappedFlowGrp++)
        {

            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " Testing for PKTDMA Mapped Flow Group : %s ...\r\n",
                      mappedFlowGrpStr[mappedFlowGrp - UDMA_NUM_MAPPED_TX_GROUP]);

            rmInitPrms = &drvHandle->initPrms.rmInitPrms;
            
            if(0U == rmInitPrms->numMappedRing[mappedFlowGrp])
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " Skipping the Test for PKTDMA Mapped Flow Group : %s , since no rings are reserved!!\r\n",
                          mappedFlowGrpStr[mappedFlowGrp - UDMA_NUM_MAPPED_TX_GROUP]);
                continue;
            }
            
            /* Allocate mapped free flows */
            flowAllocMappedPrms.mappedFlowGrp = mappedFlowGrp;
            flowAllocMappedPrms.mappedChNum = udmaTestGetMappedRingChNum(drvHandle, mappedFlowGrp, rmInitPrms->startMappedRing[mappedFlowGrp]);
            retVal = Udma_flowAllocMapped(drvHandle, flowHandle, &flowAllocMappedPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Mapped Flow alloc failed!!\n");
            }
            else
            {
                mappedFlowAllocated = UTRUE;
            }
        
            if(UDMA_SOK == retVal)
            {
                mappepdFlowNum = Udma_flowGetNum(flowHandle);
                if(UDMA_FLOW_INVALID == mappepdFlowNum)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Invalid flow ID!!\n");
                    retVal = UDMA_EFAIL;
                }
            }

            if(UDMA_SOK == retVal)
            {
                flowCntTest = Udma_flowGetCount(flowHandle);
                if(flowCntTest != flowCnt)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Flow count doesn't match!!\n");
                }
            }

            if(UDMA_SOK == retVal)
            {
                /* Attach and configure the mapped flows */
                retVal = Udma_flowAttachMapped(drvHandle, attachFlowHandle, mappepdFlowNum, &flowAllocMappedPrms);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Mapped Flow attach failed!!\n");
                }
            }

            if(UDMA_SOK == retVal)
            {
                /* Do flow config with the allocated ring - we can't do any
                * other functional test in standalone testing */
                UdmaFlowPrms_init(&flowPrms, UDMA_CH_TYPE_RX);
                flowPrms.defaultRxCQ = mappepdFlowNum;
                flowPrms.fdq0Sz0Qnum = mappepdFlowNum;
                flowPrms.fdq1Qnum    = mappepdFlowNum;
                flowPrms.fdq2Qnum    = mappepdFlowNum;
                flowPrms.fdq3Qnum    = mappepdFlowNum;
                flowPrms.fdq0Sz1Qnum = mappepdFlowNum;
                flowPrms.fdq0Sz2Qnum = mappepdFlowNum;
                flowPrms.fdq0Sz3Qnum = mappepdFlowNum;
                
                retVal = Udma_flowConfig(attachFlowHandle, 0U, &flowPrms);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Flow config failed!!\n");
                }
            }

            if(UDMA_SOK == retVal)
            {
                retVal = Udma_flowDetach(attachFlowHandle);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Flow detach failed!!\n");
                }
            }

            /* Free allocated flows */
            if(UTRUE == mappedFlowAllocated)
            {
                retVal += Udma_flowFree(flowHandle);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Flow free failed!!\n");
                }
                else
                {
                    mappedFlowAllocated = UFALSE;
                }
            }

            if(UDMA_SOK == retVal)
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " Testing for PKTDMA Mapped Flow Group : %s passed!!\r\n",
                          mappedFlowGrpStr[mappedFlowGrp - UDMA_NUM_MAPPED_TX_GROUP]);
            }
            else
            {
                break;
            }
        }
    }
#endif
    return (retVal);
}

static int32_t udmaTestFlowAllocTestLoop(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            instId;
    uint32_t            flowCnt = 500U;     /* More than actual count */
    Udma_DrvHandle      drvHandle;
    struct Udma_FlowObj flowObj;
    Udma_FlowHandle     flowHandle = &flowObj;

    for(instId = 0U; instId < UDMA_TEST_INST_ID_MAX; instId++)
    {
        drvHandle = &taskObj->testObj->drvObj[instId];

        /* Allocate free flows - one more than available - this should fail */
        retVal = Udma_flowAlloc(drvHandle, flowHandle, flowCnt);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " Flow alloc passed when it should have failed!!\n");
            retVal = UDMA_EFAIL;
            break;
        }
        else
        {
            /* Test passed as it returned error */
            retVal = UDMA_SOK;
        }
    }

    return (retVal);
}
