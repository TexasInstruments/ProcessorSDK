/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2024
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
 *  \file udma_test_event.c
 *
 *  \brief UDMA event related test case file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <udma_test.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*
 * Ring parameters
 */
/** \brief Number of ring entries - we can prime this much MCRC operations */
#define UDMA_APP_RING_ENTRIES      (1U)
/** \brief Size (in bytes) of each ring entry (Size of pointer - 64-bit) */
#define UDMA_APP_RING_ENTRY_SIZE   (sizeof(uint64_t))
/** \brief Total ring memory */
#define UDMA_APP_RING_MEM_SIZE     (UDMA_APP_RING_ENTRIES * \
                                         UDMA_APP_RING_ENTRY_SIZE)
                                
/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t udmaTestEventOutOfRangeFlowLoop(UdmaTestTaskObj *taskObj,
                                               uint32_t loopCnt);
static int32_t udmaTestEventDisableEnableSanityLoop(UdmaTestTaskObj *taskObj,
                                                    uint32_t loopCnt);
static int32_t udmaTestEventDisableEnableFxns(UdmaTestTaskObj *taskObj,
                                              Udma_EventHandle eventHandle);
static void udmaTestEventCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData);
static void udmaTestEventDmaCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Global test pass/fail flag */
static volatile int32_t gUdmaTestEventResult = UDMA_SOK;
static SemaphoreP_Handle gUdmaAppDoneSem = NULL;
static volatile int32_t gUdmaAppResult = UDMA_SOK;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t udmaTestEventOutOfRangeFlow(UdmaTestTaskObj *taskObj)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    loopCnt = 0U;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Out Of Range Flow Event Testcase ::\r\n", taskObj->taskId);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Loop count           : %d ::\r\n", taskObj->taskId, taskObj->loopCnt);

    gUdmaTestEventResult = UDMA_SOK;
    while(loopCnt < taskObj->loopCnt)
    {
        /* Perform flow event test */
        retVal = udmaTestEventOutOfRangeFlowLoop(taskObj, loopCnt);
        if(UDMA_SOK != retVal)
        {
            break;
        }

        loopCnt++;
    }

    retVal += gUdmaTestEventResult;

    return (retVal);
}

int32_t udmaTestEventDisableEnableSanity(UdmaTestTaskObj *taskObj)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    loopCnt = 0U;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Event Disable and Enable Sanity check Testcase ::\r\n", taskObj->taskId);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Loop count           : %d ::\r\n", taskObj->taskId, taskObj->loopCnt);

    gUdmaTestEventResult = UDMA_SOK;
    while(loopCnt < taskObj->loopCnt)
    {
        retVal = udmaTestEventDisableEnableSanityLoop(taskObj, loopCnt);
        if(UDMA_SOK != retVal)
        {
            break;
        }

        loopCnt++;
    }

    retVal += gUdmaTestEventResult;

    return (retVal);
}

static int32_t udmaTestEventOutOfRangeFlowLoop(UdmaTestTaskObj *taskObj,
                                               uint32_t loopCnt)
{
    int32_t                     retVal = UDMA_SOK;
    uint32_t                    instId;
    Udma_DrvHandle              drvHandle;
    Udma_EventPrms              eventPrms;
    struct Udma_EventObj        eventObj;
    Udma_EventHandle            eventHandle = NULL;
    Udma_EventRxFlowIdFwStatus  status;
    char                       *instanceIdStr[] = {"MAIN", "MCU", "BCDMA", "PKTDMA"};

    for(instId = UDMA_INST_ID_UDMAP_START; instId <= UDMA_INST_ID_UDMAP_MAX; instId++)
    {
        if(0U == loopCnt)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " Testing for NAVSS Inst: %s ...\r\n", instanceIdStr[instId]);
        }

        drvHandle = &taskObj->testObj->drvObj[instId];

        /* Register flow error event */
        eventHandle = &eventObj;
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventType         = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
        eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
        eventPrms.eventCb           = &udmaTestEventCb;
        retVal = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA Flow error event register failed!!\n");
            break;
        }

        /* Note: Since this event can't be reproduced in a standalone UDMA
         * level testcase, this test doesn't wait for the event to happen.
         * This just checks register and unregister check
         */

        /* Just check status API sanity */
        retVal = Udma_eventGetRxFlowIdFwStatus(eventHandle, &status);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " RX flow ID FW status get failed!!\n");
        }
        else
        {
            if(0U == loopCnt)
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                    " Rx Flow ID Status : %d\r\n", status.isException);
                GT_1trace(taskObj->traceMask, GT_INFO1,
                    " Rx Flow ID        : %d\r\n", status.flowId);
                GT_1trace(taskObj->traceMask, GT_INFO1,
                    " Rx Channel        : %d\r\n", status.chNum);
            }
        }

        retVal += Udma_eventUnRegister(eventHandle);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Event unregister failed!!\n");
            break;
        }

        if(0U == loopCnt)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " Testing for NAVSS Inst: %s passed!!\r\n", instanceIdStr[instId]);
        }
    }

    return (retVal);
}

static int32_t udmaTestEventDisableEnableSanityLoop(UdmaTestTaskObj *taskObj,
                                                    uint32_t loopCnt)
{
    int32_t                     retVal = UDMA_SOK;
    uint32_t                    instId;
    Udma_DrvHandle              drvHandle;
    Udma_EventPrms              eventPrms;
    struct Udma_EventObj        eventObj;
    Udma_EventHandle            eventHandle = NULL;
    char                       *instanceIdStr[] = {"MAIN", "MCU", "BCDMA", "PKTDMA"};

    for(instId = UDMA_INST_ID_UDMAP_START; instId <= UDMA_INST_ID_UDMAP_MAX; instId++)
    {
        if(0U == loopCnt)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " Testing for NAVSS Inst: %s ...\r\n", instanceIdStr[instId]);
        }

        drvHandle = &taskObj->testObj->drvObj[instId];

        eventHandle = &eventObj;

        /* Exclusive Event Enable/Disable Test */
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventType         = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
        eventPrms.eventMode         = UDMA_EVENT_MODE_EXCLUSIVE;
        eventPrms.masterEventHandle = NULL;
        eventPrms.eventCb           = &udmaTestEventCb;
        retVal = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA exclusive event register failed!!\n");
        }
        
        retVal += udmaTestEventDisableEnableFxns(taskObj, eventHandle);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Exclusive Event Disable/Enable passed!!\n");
        }

        retVal += Udma_eventUnRegister(eventHandle);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Exclusive Event unregister failed!!\n");
        }

        /* Shared Event Enable/Disable Test */
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventType         = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
        eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
        eventPrms.eventCb           = &udmaTestEventCb;
        retVal += Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA shared event register failed!!\n");
        }

        retVal += udmaTestEventDisableEnableFxns(taskObj, eventHandle);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Shared Event Disable/Enable passed!!\n");
        }

        retVal += Udma_eventUnRegister(eventHandle);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Shared Event unregister failed!!\n");
        }
        

        if(0U == loopCnt)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " Testing for NAVSS Inst: %s passed!!\r\n", instanceIdStr[instId]);
        }
    }

    return (retVal);
}

static int32_t udmaTestEventDisableEnableFxns(UdmaTestTaskObj *taskObj,
                                              Udma_EventHandle eventHandle)
{
    int32_t                     retVal = UDMA_SOK;

    /* Note: This sanity just checks event disable and enable API's.
     * This test doesn't wait for the event to happen.
     */

    /* Just check API sanity */
    retVal = Udma_eventDisable(eventHandle);
    if(UDMA_SOK != retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
            " UDMA event disable failed!!\n");
    }
    retVal += Udma_eventEnable(eventHandle);
    if(UDMA_SOK != retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
            " UDMA event enable failed!!\n");
    }

    return (retVal);
}

static void udmaTestEventCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData)
{
    SemaphoreP_Handle   transferDoneSem = (SemaphoreP_Handle) appData;

    if(transferDoneSem != NULL)
    {
        if(UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW == eventType)
        {
            SemaphoreP_post(transferDoneSem);
        }
        else
        {
            gUdmaTestEventResult = UDMA_EFAIL;
        }
    }
    else
    {
        gUdmaTestEventResult = UDMA_EFAIL;
    }

    return;
}

/* 
 * Test Case Description: Verifies the function Udma_rmTranslateCoreIntrInput
 * Test scenario 1: Check when valid args are passed
 */
int32_t UdmaRmTranslateCoreIntrInputTest(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal = UDMA_SOK;
    uint32_t             instID;
    Udma_ChHandle        chHandle;
    struct Udma_ChObj    chObj;
    Udma_ChPrms          chPrms;
    uint32_t             chType;
    Udma_DrvHandle       drvHandle;
    struct Udma_EventObj eventObj;
    Udma_EventHandle     eventHandle;
    Udma_EventPrms       eventPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Rm Translate Core Intr Input Testcase ::\r\n"
              , taskObj->taskId);

    /* Test scenario 1: Check when valid args are passed */
    chHandle         = &chObj;
    chType           = UDMA_CH_TYPE_TX;
    instID           = UDMA_TEST_DEFAULT_UDMA_INST;
    drvHandle        = &taskObj->testObj->drvObj[instID];
    UdmaChPrms_init(&chPrms, chType);
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    Udma_ChTxPrms txChPrms;
    UdmaChTxPrms_init(&txChPrms, chType);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigTx(chHandle, &txChPrms);
        if(UDMA_SOK == retVal)
        {
            eventHandle = &eventObj;
            UdmaEventPrms_init(&eventPrms);
            eventPrms.eventType         = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
            eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
            eventPrms.masterEventHandle = NULL_PTR;
            eventPrms.eventCb           = &udmaTestEventCb;
            retVal = Udma_eventRegister(drvHandle, eventHandle, &eventPrms); 
            if(UDMA_SOK == retVal)
            {
                retVal = Udma_rmTranslateCoreIntrInput(drvHandle, eventHandle->coreIntrNum);
                if(UDMA_INTR_INVALID == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: Udma_rmTranslateCoreIntrInput::" 
                              " Pos:: Check when valid args are passed!!\n");
                    retVal = UDMA_EFAIL;
                }
                else 
                {
                    retVal = UDMA_SOK;
                }
                Udma_eventUnRegister(eventHandle);
            }
        }
        Udma_chClose(chHandle);
    }

    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_eventRegister
 * Test scenario 1: Check Udma_eventConfig when chType is UDMA_CH_TYPE_TX 
 *                  and eventType is UDMA_EVENT_TYPE_DMA_COMPLETION
 * Test scenario 2: Check Udma_eventConfig when chType is UDMA_CH_TYPE_TX 
 *                  and eventType is UDMA_EVENT_TYPE_TR
 * Test scenario 3: Check Udma_eventConfig when chType is UDMA_CH_TYPE_RX and 
 *                  eventType is UDMA_EVENT_TYPE_TR
 * Test scenario 4: Check Udma_eventAllocResource when eventMode is 
 *                  UDMA_EVENT_MODE_EXCLUSIVE and masterEventHandle is NULL
 * Test scenario 5: Check Udma_eventAllocResource when osalRegisterDisable is UTRUE, 
 *                  masterEventHandle and eventCb are NULL
 * Test scenario 6: Check Udma_eventCheckParams when osalRegisterDisable is UTRUE
 * Test scenario 7: Check Udma_eventCheckParams when eventCb is NULL 
 * Test scenario 8: Check Udma_eventConfig when osalRegisterDisable is UTRUE and 
 *                  coreIntrNum is not UDMA_INTR_INVALID
 */
int32_t UdmaEventRegisterTest(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    uint32_t               instID;
    Udma_DrvHandle         drvHandle;
    struct Udma_EventObj   eventObj;
    Udma_EventHandle       eventHandle;
    Udma_EventPrms         eventPrms;
    Udma_RingMonHandle     monHandle;
    struct Udma_RingMonObj monObj;
    Udma_ChHandle          chHandle;
    struct Udma_ChObj      chObj;
    Udma_ChPrms            chPrms;
    uint32_t               chType;
    UdmaTestChObj          *testChObj;
    uint32_t               heapIdSrc = UTILS_MEM_HEAP_ID_MSMC;
    uint32_t               elemCnt = 50U;
    uint32_t               timeout=0U;

    /* Test scenario 1: Check when chType is UDMA_CH_TYPE_TX and 
     *                  eventType is UDMA_EVENT_TYPE_DMA_COMPLETION */
    chHandle                      = &chObj;
    chType                        = UDMA_CH_TYPE_TX;
    instID                        = UDMA_TEST_DEFAULT_UDMA_INST;
    drvHandle                     = &taskObj->testObj->drvObj[instID];
    UdmaChPrms_init(&chPrms, chType);
    chPrms.peerChNum              = UDMA_PSIL_CH_MCU_CPSW0_TX;
    chPrms.cqRingPrms.elemCnt     = elemCnt;
    chPrms.cqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
    chPrms.cqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.cqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    Udma_ChTxPrms txChPrms;
    UdmaChTxPrms_init(&txChPrms, chType);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigTx(chHandle, &txChPrms);
        if(UDMA_SOK == retVal)
        {
            testChObj                   = taskObj->chObj[0];
            eventHandle                 = &eventObj;
            instID                      = UDMA_TEST_INST_ID_MAIN_0;
            monHandle                   = &monObj;
            UdmaEventPrms_init(&eventPrms);
            eventPrms.eventCb           = &udmaTestEventDmaCb;
            eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
            eventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
            eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);;
            eventPrms.monHandle         = monHandle;
            eventPrms.chHandle          = chHandle;
            eventPrms.appData           = testChObj;
            retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: Udma_eventConfig::Pos:: Check when chType" 
                          " is UDMA_CH_TYPE_TX and eventType is UDMA_EVENT_TYPE_DMA_COMPLETION!!\n");
                retVal = UDMA_EFAIL;
            }
            else 
            {
                retVal = UDMA_SOK;
                Udma_eventUnRegister(eventHandle);
            }
        }
        Udma_chClose(chHandle);
    }
    if(NULL != chPrms.cqRingPrms.ringMem)
    {
        retVal = Utils_memFree(heapIdSrc, chPrms.cqRingPrms.ringMem, chPrms.cqRingPrms.ringMemSize);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
        }
    }

    /* Test scenario 2: Check Udma_eventConfig when chType is UDMA_CH_TYPE_TX 
     *                  and eventType is UDMA_EVENT_TYPE_TR */
    if(UDMA_SOK == retVal)
    {
        chHandle         = &chObj;
        chType           = UDMA_CH_TYPE_TX;
        instID           = UDMA_TEST_DEFAULT_UDMA_INST;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        Udma_ChTxPrms txChPrms;
        UdmaChTxPrms_init(&txChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigTx(chHandle, &txChPrms);
            if(UDMA_SOK == retVal)
            {
                eventHandle                 = &eventObj;
                monHandle                   = &monObj;
                UdmaEventPrms_init(&eventPrms);
                eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
                eventPrms.eventType         = UDMA_EVENT_TYPE_TR;
                eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);;
                eventPrms.monHandle         = monHandle;
                eventPrms.chHandle          = chHandle;
                eventPrms.eventCb           = &udmaTestEventCb; 
                retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: Udma_eventConfig::Pos:: Check when" 
                              " chType is UDMA_CH_TYPE_TX and eventType is UDMA_EVENT_TYPE_TR!!\n");
                    retVal = UDMA_EFAIL;
                }
                else 
                {
                    retVal = UDMA_SOK;
                    Udma_eventUnRegister(eventHandle);
                }
            }
            Udma_chClose(chHandle);
        }
    }

    /* Test scenario 3: Check Udma_eventConfig when chType is UDMA_CH_TYPE_RX and 
     *                  eventType is UDMA_EVENT_TYPE_TR */
    if(UDMA_SOK == retVal)
    {
        chHandle         = &chObj;
        chType           = UDMA_CH_TYPE_RX;
        instID           = UDMA_TEST_DEFAULT_UDMA_INST;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        Udma_ChRxPrms rxChPrms;
        UdmaChRxPrms_init(&rxChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigRx(chHandle, &rxChPrms);
            if(UDMA_SOK == retVal)
            {
                eventHandle                 = &eventObj;
                monHandle                   = &monObj;
                UdmaEventPrms_init(&eventPrms);
                eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
                eventPrms.eventType         = UDMA_EVENT_TYPE_TR;
                eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);;
                eventPrms.monHandle         = monHandle;
                eventPrms.chHandle          = chHandle;
                eventPrms.eventCb           = &udmaTestEventCb; 
                retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: Udma_eventConfig::Pos:: Check when " 
                              " chType is UDMA_CH_TYPE_RX and eventType is UDMA_EVENT_TYPE_TR!!\n");
                    retVal = UDMA_EFAIL;
                }
                else 
                {
                    retVal = UDMA_SOK;
                    Udma_eventUnRegister(eventHandle);
                }
            }
            Udma_chClose(chHandle);
        }
    }

    /* Test scenario 4: Check Udma_eventAllocResource when eventMode is 
     *                  UDMA_EVENT_MODE_EXCLUSIVE and masterEventHandle is NULL */
    if(UDMA_SOK == retVal)
    {
        instID                      = UDMA_TEST_INST_ID_MAIN_0;
        eventHandle->drvHandle      = &taskObj->testObj->drvObj[instID];
        UdmaEventPrms_init(&eventPrms);
        eventPrms.chHandle          = &chObj;
        eventPrms.eventCb           = &udmaTestEventCb;
        eventPrms.eventMode         = UDMA_EVENT_MODE_EXCLUSIVE;
        eventPrms.eventType         = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
        eventPrms.masterEventHandle = NULL;
        retVal                      = Udma_eventRegister(eventHandle->drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventAllocResource::Pos:: Check when" 
                      " eventMode is UDMA_EVENT_MODE_EXCLUSIVE and masterEventHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else 
        {
            retVal = UDMA_SOK;
            Udma_eventUnRegister(eventHandle);
        }
    }

    /* Test scenario 5: Check Udma_eventAllocResource when osalRegisterDisable is UTRUE, 
     *                  masterEventHandle and eventCb are NULL */
    if(UDMA_SOK == retVal)
    {
        instID                        = UDMA_TEST_INST_ID_MAIN_0;
        eventHandle->drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaEventPrms_init(&eventPrms);
        eventPrms.chHandle            = &chObj;
        eventPrms.eventCb             = NULL;
        eventPrms.eventMode           = UDMA_EVENT_MODE_SHARED;
        eventPrms.eventType           = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
        eventPrms.masterEventHandle   = NULL;
        eventPrms.osalRegisterDisable = UTRUE;
        retVal                        = Udma_eventRegister(eventHandle->drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventAllocResource::Pos:: Check when" 
                      " osalRegisterDisable is UTRUE, masterEventHandle and eventCb are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else 
        {
            retVal = UDMA_SOK;
            Udma_eventUnRegister(eventHandle);
        }   
    }

    /* Test scenario 6: Check Udma_eventCheckParams when osalRegisterDisable is UTRUE */
    if(UDMA_SOK == retVal)
    {
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventCb             = &udmaTestEventCb;
        eventPrms.eventMode           = UDMA_EVENT_MODE_SHARED;
        eventPrms.masterEventHandle   = Udma_eventGetGlobalHandle(drvHandle);
        eventPrms.osalRegisterDisable = UTRUE;
        eventPrms.eventType           = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
        retVal                        = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventCheckParams::" 
                      " Pos:: Check when osalRegisterDisable is UTRUE!!\n");
            retVal = UDMA_EFAIL;
        }
        else 
        {
            retVal = UDMA_SOK;
            Udma_eventUnRegister(eventHandle);
        }
    }

    /* Test scenario 7: Check Udma_eventCheckParams when eventCb is NULL */
    if(UDMA_SOK == retVal)
    {
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventCb                              = NULL;
        eventPrms.eventMode                            = UDMA_EVENT_MODE_SHARED;
        eventPrms.masterEventHandle                    = Udma_eventGetGlobalHandle(drvHandle);
        eventPrms.masterEventHandle->eventPrms.eventCb = NULL;
        eventPrms.eventType                            = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
        retVal                                         = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventCheckParams::" 
                      " Pos:: Check when eventCb is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else 
        {
            retVal = UDMA_SOK;
            Udma_eventUnRegister(eventHandle);
        }
    }

    /* Test scenario 8: Check Udma_eventConfig when osalRegisterDisable is UTRUE and 
     *                  coreIntrNum is not UDMA_INTR_INVALID */
    if(UDMA_SOK == retVal)
    {
        chHandle         = &chObj;
        chType           = UDMA_CH_TYPE_TR_BLK_COPY;
        instID           = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.elemCnt     = elemCnt;
        chPrms.fqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
        chPrms.fqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.fqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        chPrms.cqRingPrms.elemCnt     = elemCnt;
        chPrms.cqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
        chPrms.cqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.cqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        Udma_ChTxPrms txChPrms;
        UdmaChTxPrms_init(&txChPrms, chType);
        Udma_ChRxPrms rxChPrms;
        UdmaChRxPrms_init(&rxChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigTx(chHandle, &txChPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chConfigTx failed!!\n");
            }
            retVal = Udma_chConfigRx(chHandle, &rxChPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chConfigRx failed!!\n");
            }
            retVal = Udma_chEnable(chHandle);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chEnable failed!!\n");
            }
            if(UDMA_SOK == retVal)
            {
                UdmaEventPrms_init(&eventPrms);
                testChObj                     = taskObj->chObj[0];
                eventPrms.appData             = testChObj;
                eventPrms.chHandle            = chHandle;
                eventPrms.eventCb             = &udmaTestEventDmaCb;
                eventPrms.eventMode           = UDMA_EVENT_MODE_SHARED;
                eventPrms.eventType           = UDMA_EVENT_TYPE_DMA_COMPLETION;
                eventPrms.masterEventHandle   = NULL;
                eventPrms.coreIntrNum         = UDMA_CORE_INTR_ANY;
                eventPrms.osalRegisterDisable = UTRUE;
                retVal                        = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: Udma_eventConfig::Pos::Check when" 
                              " osalRegisterDisable is UTRUE and coreIntrNum is not UDMA_INTR_INVALID!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = UDMA_SOK;
                    Udma_eventUnRegister(eventHandle);
                }
                Udma_chDisable(chHandle, timeout);
            }
            Udma_chClose(chHandle);
            if(NULL != chPrms.cqRingPrms.ringMem)
            {
                retVal  = Utils_memFree(heapIdSrc, chPrms.fqRingPrms.ringMem, chPrms.fqRingPrms.ringMemSize);
                retVal += Utils_memFree(heapIdSrc, chPrms.cqRingPrms.ringMem, chPrms.cqRingPrms.ringMemSize);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
                }
            }
        }
    }

    return retVal;
}

static void udmaTestEventDmaCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData)
{
    if(UDMA_EVENT_TYPE_DMA_COMPLETION == eventType)
    {
        SemaphoreP_post(gUdmaAppDoneSem);
    }
    else
    {
        gUdmaAppResult = UDMA_EFAIL;
    }

    return;
}

