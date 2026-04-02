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
 *  \file udma_test_event_neg.c
 *
 *  \brief File containing UDMA negative test cases for UDMA event APIs.
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

static void udmaTestEventCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData);
static void udmaTestEventDmaCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static volatile int32_t gUdmaTestEventResult = UDMA_SOK;
static volatile int32_t gUdmaTestAppResult = UDMA_SOK;
static SemaphoreP_Handle gUdmaTestAppDoneSem = NULL;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Test Case Description: Verifies the function Udma_eventRegister for
 * 1)Test scenario 1: Check when driver handle, event Handle and eventPrms are
 *                    NULL.
 * 2)Test scenario 2: Check when driver handle and event Handle are NULL.
 * 3)Test scenario 3: Check when driver handle and eventPrms are NULL.
 * 4)Test scenario 4: Check when driver handle is NULL.
 * 5)Test scenario 5: Check when event handle and eventPrms are NULL.
 * 6)Test scenario 6: Check when event handle is NULL.
 * 7)Test scenario 7: Check when event Prms is NULL.
 * 8)Test scenario 8: Check when drvInitDone is UDMA_DEINIT_DONE.
 * 9)Test scenario 9: Check to print error message "[Error] Event resource
 *                    allocation failed.
 */
int32_t UdmaTestEventRegisterNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal = UDMA_SOK;
    Udma_EventHandle     eventHandle;
    Udma_EventPrms       eventPrms;
    struct Udma_EventObj eventObj;
    Udma_DrvHandle       drvHandle;
    uint32_t             instId;
    struct Udma_DrvObj   backUpDrvObj;
    struct Udma_ChObj    chObj;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA EventRegister negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when driver handle, event Handle and eventPrms
     *                  are NULL */
    UdmaEventPrms_init(&eventPrms);
    drvHandle   = (Udma_DrvHandle) NULL_PTR;
    eventHandle = (Udma_EventHandle) NULL_PTR;
    retVal      = Udma_eventRegister(drvHandle, eventHandle, NULL );
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                  " EventRegister::Neg:: Check when driver handle, event Handle"
                  " and eventPrms are NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when driver handle and event handle are NULL */
        drvHandle   = (Udma_DrvHandle) NULL_PTR;
        eventHandle = (Udma_EventHandle) NULL_PTR;
        retVal      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventRegister:: Neg:: Check when driver handle and"
                      " event handle are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when driver handle and eventPrms are NULL */
        drvHandle   = (Udma_DrvHandle) NULL_PTR;
        eventHandle = &eventObj;
        retVal      = Udma_eventRegister(drvHandle, eventHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventRegister:: Neg:: Check when driver handle and"
                      " eventPrms are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when driver handle is NULL */
        drvHandle   = (Udma_DrvHandle) NULL_PTR;
        eventHandle = &eventObj;
        retVal      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventRegister:: Neg:: Check when driver handle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when event handle and eventPrms are NULL */
        instId      = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle   = &taskObj->testObj->drvObj[instId];
        eventHandle = (Udma_EventHandle) NULL_PTR;
        retVal      = Udma_eventRegister(drvHandle, eventHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventRegister:: Neg:: Check when event handle and"
                      " eventPrms are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check when event handle is NULL */
        eventHandle = (Udma_EventHandle) NULL_PTR ;
        retVal      = Udma_eventRegister(drvHandle, eventHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventRegister:: Neg:: Check when event handle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 7: Check when event Prms is NULL */
        eventHandle = &eventObj;
        retVal      = Udma_eventRegister(drvHandle, eventHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventRegister:: Neg:: Check when event handle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 8: Check when drvInitDone is UDMA_DEINIT_DONE */
        backUpDrvObj           = taskObj->testObj->drvObj[instId];
        drvHandle              = &taskObj->testObj->drvObj[instId];
        drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                 = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventRegister:: Neg:: Check when drvInitDone is "
                      " UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[instId] = backUpDrvObj;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 9: Check to print error message "[Error] Event resource
         *                   allocation failed" */
        backUpDrvObj                                  = taskObj->testObj->drvObj[instId];
        drvHandle                                     = &taskObj->testObj->drvObj[instId];
        eventPrms.chHandle                            = &chObj;
        instId                                        = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle                                     = &taskObj->testObj->drvObj[instId];
        drvHandle->initPrms.rmInitPrms.numGlobalEvent = 0U;
        retVal                                        = Udma_eventRegister(drvHandle,
                                                                           eventHandle,
                                                                           &eventPrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: EventRegister::"
                      " Neg:: Check to print error message [Error] Event resource"
                      " allocation failed !!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[instId] = backUpDrvObj;
    }

    return (retVal);
}

/*
 * Test Case Description: Verifies the function Udma_eventUnRegister for
 * 1)Test scenario 1: Check when eventHandle is NULL.
 * 2)Test scenario 2: Check when drvInitDone is UDMA_INIT_DONE.
 * 3)Test scenario 3: Check when eventInitDone is UDMA_DEINIT_DONE
 * 4)Test scenario 4: Check to free-up master event when shared events are still not yet
 *                    unregistered
 * 5)Test scenario 5: Check Udma_eventUnRegister when vintrBitNum is euqal to UDMA_EVENT_INVALID
 */
int32_t UdmaTestEventUnRegisterNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal = UDMA_SOK;
    Udma_EventHandle     eventHandle;
    struct Udma_EventObj eventObj;
    uint32_t             instId;
    struct Udma_DrvObj   backUpDrvObj;
    uint32_t             backUpvintrBitNum;
    struct Udma_ChObj    chObj;
    Udma_ChHandle        chHandle;
    Udma_EventPrms       eventPrms;
    Udma_ChPrms          chPrms;
    uint32_t             chType;
    uint32_t             instID;
    Udma_DrvHandle       drvHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA EventUnRegister negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when eventHandle is NULL */
    eventHandle = (Udma_EventHandle) NULL_PTR;
    retVal      = Udma_eventUnRegister(eventHandle);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                  " EventUnRegister:: Neg:: Check when eventHandle is NULL !!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when drvInitDone is UDMA_INIT_DONE */
        instId                              = UDMA_TEST_INST_ID_MAIN_0;
        eventHandle                         = &eventObj;
        backUpDrvObj                        = taskObj->testObj->drvObj[instId];
        eventHandle->drvHandle              = &taskObj->testObj->drvObj[instId];
        eventHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                              = Udma_eventUnRegister(eventHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventUnRegister:: Neg:: Check when drvInitDone is "
                      " UDMA_INIT_DONE !!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[instId] = backUpDrvObj;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when eventInitDone is UDMA_DEINIT_DONE */
        instId                              = UDMA_TEST_INST_ID_MAIN_0;
        eventHandle                         = &eventObj;
        backUpDrvObj                        = taskObj->testObj->drvObj[instId];
        eventHandle->drvHandle              = &taskObj->testObj->drvObj[instId];
        eventHandle->drvHandle->drvInitDone = UDMA_INIT_DONE;
        eventHandle -> eventInitDone        = UDMA_DEINIT_DONE; 
        retVal                              = Udma_eventUnRegister(eventHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventUnRegister:: Neg:: Check when eventInitDone is "
                      " UDMA_DEINIT_DONE !!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[instId] = backUpDrvObj;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check to free-up master event when shared events are still not yet
         *                  unregistered */
        eventHandle->eventInitDone               = UDMA_INIT_DONE;
        eventHandle->eventPrms.chHandle          = &chObj;
        eventHandle->globalEvent                 = UDMA_EVENT_INVALID;
        eventHandle->irIntrNum                   = UDMA_INTR_INVALID;
        eventHandle->vintrNum                    = UDMA_EVENT_INVALID;
        eventHandle->vintrBitNum                 = UDMA_EVENT_INVALID;
        eventHandle->coreIntrNum                 = UDMA_INTR_INVALID;
        eventHandle->eventPrms.masterEventHandle = (Udma_EventHandle) NULL_PTR;
        retVal                                   = Udma_eventUnRegister(eventHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventUnRegister:: Neg:: Check to free-up master event when"
                      " shared events are still not yet \n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check Udma_eventUnRegister when vintrBitNum is euqal to UDMA_EVENT_INVALID */
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
                retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms); 
                backUpvintrBitNum           = eventHandle->vintrBitNum;
                eventHandle->vintrBitNum    = UDMA_EVENT_INVALID;
                if(UDMA_SOK == retVal)
                {
                    retVal = Udma_eventUnRegister(eventHandle);
                    if(UDMA_SOK != retVal)
                    {
                        retVal = UDMA_SOK;
                    }
                    else
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                                  " EventUnRegister:: Neg:: Check when vintrBitNum is euqal to"
                                  " UDMA_EVENT_INVALID!!\n");
                        retVal = UDMA_EFAIL;
                    }
                }
                eventHandle->vintrBitNum = backUpvintrBitNum;
                Udma_eventUnRegister(eventHandle);
            }
        }
        Udma_chClose(chHandle);
    }

    return (retVal);
}

/*
 * Test Case Description: Verifies the function Udma_eventGetId for
 * 1)Test scenario 1: Check when eventHandle is NULL.
 * 2)Test scenario 2: Check when eventInitDone is UDMA_DEINIT_DONE.
 * 3)Test scenario 3: Check when drvHandle is NULL.
 */
int32_t UdmaTestEventGetIdNeg(UdmaTestTaskObj *taskObj)
{
    uint32_t             evtNum = UDMA_EVENT_INVALID;
    int32_t              retVal = UDMA_SOK;
    Udma_EventHandle     eventHandle;
    struct Udma_EventObj eventObj;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA EventGetId negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when eventHandle is NULL */
    eventHandle = (Udma_EventHandle) NULL_PTR;
    evtNum      = Udma_eventGetId(eventHandle);
    if(UDMA_EVENT_INVALID == evtNum)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                  " EventGetId:: Neg:: Check when eventHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when eventInitDone is UDMA_DEINIT_DONE */
        eventHandle                = &eventObj;
        eventHandle->eventInitDone = UDMA_DEINIT_DONE;
        retVal                     = Udma_eventGetId(eventHandle);
        if(UDMA_EVENT_INVALID == evtNum)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventGetId:: Neg:: Check when eventInitDone is "
                      " UDMA_DEINIT_DONE !!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when drvHandle is NULL */
        eventHandle->eventInitDone = UDMA_INIT_DONE;
        eventHandle->drvHandle     = NULL_PTR;
        evtNum                     = Udma_eventGetId(eventHandle);
        if(UDMA_EVENT_INVALID == evtNum)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventGetId:: Neg:: Check when drvHandle is NU!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_eventDisable for
 * 1)Test scenario 1: Check when eventHandle is NULL.
 * 2)Test scenario 2: Check when eventInitDone is UDMA_DEINIT_DONE.
 * 3)Test scenario 3: Check when drvHandle is NULL.
 */
int32_t UdmaTestEventDisableNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal = UDMA_SOK;
    Udma_EventHandle     eventHandle;
    struct Udma_EventObj eventObj;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA EventDisable negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when eventHandle is NULL */
    eventHandle = (Udma_EventHandle) NULL_PTR;
    retVal      = Udma_eventDisable(eventHandle);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                  " EventDisable:: Neg:: Check when eventHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when eventInitDone is UDMA_DEINIT_DONE */
        eventHandle                = &eventObj;
        eventHandle->eventInitDone = UDMA_DEINIT_DONE;
        retVal                     = Udma_eventDisable(eventHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventDisable:: Neg:: Check when eventInitDone is "
                      " UDMA_DEINIT_DONE !!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when drvHandle is NULL */
        eventHandle->eventInitDone = UDMA_INIT_DONE;
        eventHandle->drvHandle     = NULL_PTR;
        retVal                     = Udma_eventDisable(eventHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventDisable:: Neg:: Check when drvHandle is NU!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_eventEnable for
 * 1)Test scenario 1: Check when eventHandle is NULL.
 * 2)Test scenario 2: Check when eventInitDone is UDMA_DEINIT_DONE.
 * 3)Test scenario 3: Check when drvHandle is NULL.
 */
int32_t UdmaTestEvenEnableNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal = UDMA_SOK;
    Udma_EventHandle     eventHandle;
    struct Udma_EventObj eventObj;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA EventEnable negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when eventHandle is NULL */
    eventHandle = (Udma_EventHandle) NULL_PTR;
    retVal      = Udma_eventEnable(eventHandle);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                  " EventEnable:: Neg:: Check when eventHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when eventInitDone is UDMA_DEINIT_DONE */
        eventHandle                = &eventObj;
        eventHandle->eventInitDone = UDMA_DEINIT_DONE;
        retVal                     = Udma_eventEnable(eventHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventEnable:: Neg:: Check when eventInitDone is "
                      " UDMA_DEINIT_DONE !!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when drvHandle is NULL */
        eventHandle->eventInitDone = UDMA_INIT_DONE;
        eventHandle->drvHandle     = NULL_PTR;
        retVal                     = Udma_eventEnable(eventHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventEnable:: Neg:: Check when drvHandle is NU!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_eventGetGlobalHandle for
 * 1)Test scenario 1: Check when eventHandle is NULL.
 * 2)Test scenario 2: Check when drvInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestEventGetGlobalNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal      = UDMA_SOK;
    Udma_EventHandle     eventHandle = NULL_PTR;
    Udma_DrvHandle       drvHandle;
    uint32_t             instId;
    struct Udma_DrvObj   backUpDrvObj;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA EventGetGlobal negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when driver Handle is NULL */
    drvHandle   = (Udma_DrvHandle) NULL_PTR;
    eventHandle = Udma_eventGetGlobalHandle(drvHandle);
    if(NULL_PTR == eventHandle)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                  " EventGetGlobal:: Neg:: Check when driver Handle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when drvInitDone is UDMA_DEINIT_DONE */
        instId                 = UDMA_TEST_INST_ID_MAIN_0;
        backUpDrvObj           = taskObj->testObj->drvObj[instId];
        drvHandle              = &taskObj->testObj->drvObj[instId];
        drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        eventHandle            = Udma_eventGetGlobalHandle(drvHandle);
        if(NULL_PTR == eventHandle)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventGetGlobal:: Neg:: Check when drvInitDone is "
                      " UDMA_DEINIT_DONE !!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[instId] = backUpDrvObj;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_eventGetRxFlowIdFwStatus for
 * 1)Test scenario 1: Check eventHandle and Status are NULL.
 * 2)Test scenario 2: Check when eventHandle is NULL.
 * 3)Test scenario 3: Check when Status is NULL.
 * 4)Test scenario 4: Check when eventInitDone is UDMA_DEINIT_DONE.
 * 5)Test scenario 5: Check when drvHandle is NULL.
 * 6)Test scenario 6: Check when drvInitDone is UDMA_DEINIT_DONE
 */
int32_t UdmaTestEventGetRxFlowStatusNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                    retVal = UDMA_SOK;
    Udma_EventHandle           eventHandle;
    uint32_t                   instId;
    struct Udma_DrvObj         backUpDrvObj;
    Udma_EventRxFlowIdFwStatus status;
    struct Udma_EventObj       eventObj;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA EventGetRxFlowStatus negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check eventHandle and Status are NULL */
    eventHandle = (Udma_EventHandle) NULL_PTR;
    retVal      = Udma_eventGetRxFlowIdFwStatus(eventHandle, NULL);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                  " EventGetRxFlowStatus:: Neg:: Check eventHandle Status"
                  " are NULL !!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when eventHandle is NULL */
        retVal = Udma_eventGetRxFlowIdFwStatus(eventHandle, &status);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventGetRxFlowStatus:: Neg:: Check when eventHandle"
                      " is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when Status is NULL */
        eventHandle                = &eventObj;
        eventHandle->eventInitDone = UDMA_INIT_DONE;
        retVal                     = Udma_eventGetRxFlowIdFwStatus(eventHandle, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventGetRxFlowStatus:: Neg:: Check when Status is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when eventInitDone is UDMA_DEINIT_DONE */
        eventHandle->eventInitDone = UDMA_DEINIT_DONE;
        retVal                     = Udma_eventGetRxFlowIdFwStatus(eventHandle, &status);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventGetRxFlowStatus:: Neg:: Check when eventInitDone is "
                      " UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when drvHandle is NULL */
        eventHandle->eventInitDone = UDMA_INIT_DONE;
        eventHandle->drvHandle     = NULL_PTR;
        retVal                     = Udma_eventGetRxFlowIdFwStatus(eventHandle, &status);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventGetRxFlowStatus:: Neg:: Check when drvHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6:  Check when drvInitDone is UDMA_DEINIT_DONE */
        instId                              = UDMA_TEST_INST_ID_MAIN_0;
        backUpDrvObj                        = taskObj->testObj->drvObj[instId];
        eventHandle->drvHandle              = &taskObj->testObj->drvObj[instId];
        eventHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                              = Udma_eventGetRxFlowIdFwStatus(eventHandle,
                                                                            &status);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " EventGetRxFlowStatus:: Neg:: Check when drvInitDone is "
                      " UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[instId] = backUpDrvObj;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_eventRegister for
 * 1)Test scenario 1: Check when instType is UDMA_INST_ID_BCDMA_0.
 */
int32_t UdmaTestEventRegisterBcdmaTestNeg(UdmaTestTaskObj *taskObj)
{

    int32_t              retVal = UDMA_SOK;
    Udma_EventHandle     eventHandle;
    Udma_EventPrms       eventPrms;
    struct Udma_EventObj eventObj;
    Udma_DrvHandle       drvHandle;
    uint32_t             instId;
    struct Udma_ChObj    chObj;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA EventRegisterBcdmaTest negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when instType is UDMA_INST_ID_BCDMA_0 */
    UdmaEventPrms_init(&eventPrms);
    eventHandle         = &eventObj;
    instId              = UDMA_TEST_INST_ID_BCDMA_0;
    eventPrms.chHandle  = &chObj;
    drvHandle           = &taskObj->testObj->drvObj[instId];
    eventPrms.eventType = UDMA_EVENT_TYPE_TEARDOWN_PACKET;
    retVal              = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
    if(UDMA_SOK == retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                  " EventRegister:: Neg:: Check when instType is "
                  " UDMA_INST_ID_BCDMA_0 !!\n");
        retVal = UDMA_EFAIL;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_eventUnRegister for
 * 1)Test scenario 1: Check when instType is UDMA_INST_ID_BCDMA_0.
 */
int32_t UdmaTestEventUnRegisterBcdmaTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal = UDMA_SOK;
    Udma_EventHandle     eventHandle;
    struct Udma_EventObj eventObj;
    uint32_t             instId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA EventUnRegister negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when instType is UDMA_INST_ID_BCDMA_0 */
    instId                           = UDMA_TEST_INST_ID_BCDMA_0;
    eventHandle                      = &eventObj;
    eventHandle->drvHandle           = &taskObj->testObj->drvObj[instId];
    eventHandle->eventPrms.eventType = UDMA_EVENT_TYPE_TEARDOWN_PACKET;
    retVal                           = Udma_eventUnRegister(eventHandle);
    if(UDMA_SOK == retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                  " EventUnRegister:: Neg:: Check when instType is "
                  " UDMA_INST_ID_BCDMA_0 !!\n");
        retVal = UDMA_EFAIL;
    }

    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_eventRegister
 * Test scenario 1: Check Udma_eventCheckParams when eventMode is 
 *                  UDMA_EVENT_MODE_EXCLUSIVE and masterEventHandle is not NULL 
 * Test scenario 2: Check Udma_eventCheckParams when eventCb is NULL 
 * Test scenario 3: Check Udma_eventCheckParams when eventCb for masterEventHandle is not NULL
 * Test scenario 4: Check Udma_eventCheckParams when eventType is UDMA_EVENT_TYPE_RING 
 *                  and ringHandle is NULL
 * Test scenario 5: Check Udma_eventCheckParams when eventType is UDMA_EVENT_TYPE_RING_MON 
 *                  and monHandle is NULL
 * Test scenario 6: Check Udma_eventCheckParams when eventType is UDMA_EVENT_TYPE_MASTER 
 *                  and eventMode is not UDMA_EVENT_MODE_SHARED
 * Test scenario 7: Check when eventType is UDMA_EVENT_TYPE_MASTER and numVintr is 0U
 * Test scenario 8: Check Udma_eventAllocResource when vintrBitAllocFlag bits are not available
 * Test scenario 9: Check Udma_eventAllocResource when preferredCoreIntrNum is not valid 
 * Test scenario 10: Check Udma_eventAllocResource when eventType is UDMA_EVENT_TYPE_TR 
 *                   and chOesAllocDone is UTRUE
 * Test scenario 11: Check Udma_eventCheckUnRegister to free-up master event when 
 *                   shared events are still not yet unregistered 
 * Test scenario 12: Check Udma_eventAllocResource when masterEventHandle is NULL 
 *                   and irIntr is not available
 * Test scenario 13: Check Udma_eventAllocResource when eventMode is neither 
 *                   shared nor exclusive
 */
int32_t UdmaEventRegisterTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    uint32_t               instID;
    Udma_DrvHandle         drvHandle;
    struct Udma_EventObj   eventObj;
    Udma_EventHandle       eventHandle;
    Udma_EventPrms         eventPrms;
    struct Udma_DrvObj     backUpDrvObj;
    struct Udma_ChObj      chObj;
    Udma_ChHandle          chHandle;
    Udma_ChPrms            chPrms;
    uint32_t               chType;
    UdmaTestChObj          *testChObj;
    uint32_t               heapIdSrc = UTILS_MEM_HEAP_ID_MSMC;
    uint32_t               elemCnt = 50U;
    Udma_RingMonHandle     monHandle;
    struct Udma_RingMonObj monObj;
    int32_t                resArrIdx;
    int32_t                resArrSize;

    /* Test scenario 1: Check Udma_eventCheckParams when eventMode is 
     *                  UDMA_EVENT_MODE_EXCLUSIVE and masterEventHandle is not NULL */
    eventHandle                 = &eventObj;
    instID                      = UDMA_TEST_DEFAULT_UDMA_INST;
    drvHandle                   = &taskObj->testObj->drvObj[instID];
    UdmaEventPrms_init(&eventPrms);
    eventPrms.eventType         = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
    eventPrms.eventMode         = UDMA_EVENT_MODE_EXCLUSIVE;
    eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
    eventPrms.eventCb           = &udmaTestEventCb;
    retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_eventCheckParams::Neg:: Check when" 
                  " eventMode is UDMA_EVENT_MODE_EXCLUSIVE and masterEventHandle is not NULL!!\n");
        retVal = UDMA_EFAIL;
        Udma_eventUnRegister(eventHandle);
    }
    else 
    {
        retVal = UDMA_SOK;
    }

    /* Test scenario 2: Check Udma_eventCheckParams when eventCb is NULL */
    if(UDMA_SOK == retVal)
    {
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventCb                              = (Udma_EventCallback) NULL_PTR;
        eventPrms.eventMode                            = UDMA_EVENT_MODE_SHARED;
        eventPrms.masterEventHandle                    = Udma_eventGetGlobalHandle(drvHandle);
        eventPrms.masterEventHandle->eventPrms.eventCb = &udmaTestEventCb;
        retVal                                         = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventCheckParams::" 
                      " Neg:: Check when eventCb is NULL!!\n");
            retVal = UDMA_EFAIL;
            Udma_eventUnRegister(eventHandle);
        }
        else 
        {
            retVal = UDMA_SOK;
        }
    }   

    /* Test scenario 3: Check Udma_eventCheckParams when eventCb for masterEventHandle is not NULL */
    if(UDMA_SOK == retVal)
    {
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventCb                              = &udmaTestEventCb;
        eventPrms.eventMode                            = UDMA_EVENT_MODE_SHARED;
        eventPrms.masterEventHandle                    = Udma_eventGetGlobalHandle(drvHandle);
        eventPrms.masterEventHandle->eventPrms.eventCb = &udmaTestEventCb;
        retVal                                         = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventCheckParams::" 
                      " Neg:: Check when eventCb for masterEventHandle is not NULL!!\n");
            retVal = UDMA_EFAIL;
            Udma_eventUnRegister(eventHandle);
        }
        else 
        {
            retVal = UDMA_SOK;
        }
    }  

    /* Test scenario 4: Check Udma_eventCheckParams when eventType is UDMA_EVENT_TYPE_RING 
     *                  and ringHandle is NULL */
    if(UDMA_SOK == retVal)
    {
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventCb           = &udmaTestEventCb;
        eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        eventPrms.eventType         = UDMA_EVENT_TYPE_RING;
        eventPrms.masterEventHandle = NULL;
        retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventCheckParams::Neg:: Check when" 
                      " eventType is UDMA_EVENT_TYPE_RING and ringHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else 
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 5: Check Udma_eventCheckParams when eventType is UDMA_EVENT_TYPE_RING_MON 
     *                  and monHandle is NULL */
    if(UDMA_SOK == retVal)
    {
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventCb           = &udmaTestEventCb;
        eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        eventPrms.eventType         = UDMA_EVENT_TYPE_RING_MON;
        eventPrms.masterEventHandle = NULL;
        retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventCheckParams::Neg:: Check when" 
                      " eventType is UDMA_EVENT_TYPE_RING_MON and monHandle is NULL!!\n");
            retVal = UDMA_EFAIL;
            Udma_eventUnRegister(eventHandle);
        }
        else 
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 6: Check Udma_eventCheckParams when eventType is UDMA_EVENT_TYPE_MASTER 
     *                  and eventMode is not UDMA_EVENT_MODE_SHARED */
    if(UDMA_SOK == retVal)
    {
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventCb           = &udmaTestEventCb;
        eventPrms.eventMode         = UDMA_EVENT_MODE_EXCLUSIVE;
        eventPrms.eventType         = UDMA_EVENT_TYPE_MASTER;
        eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
        retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventCheckParams::Neg:: Check when eventType" 
                      " is UDMA_EVENT_TYPE_MASTER and eventMode is not UDMA_EVENT_MODE_SHARED!!\n");
            retVal = UDMA_EFAIL;
            Udma_eventUnRegister(eventHandle);
        }
        else 
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 7: Check when eventType is UDMA_EVENT_TYPE_MASTER and numVintr is 0U */
    if(UDMA_SOK == retVal)
    {
        instID                                               = UDMA_TEST_INST_ID_MAIN_0;
        eventHandle->drvHandle                               = &taskObj->testObj->drvObj[instID];
        backUpDrvObj                                         = taskObj->testObj->drvObj[instID];
        eventHandle->drvHandle->initPrms.rmInitPrms.numVintr = 0U;
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventCb           = &udmaTestEventCb;
        eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        eventPrms.eventType         = UDMA_EVENT_TYPE_MASTER;
        eventPrms.masterEventHandle = NULL;
        retVal                      = Udma_eventRegister(eventHandle->drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventAllocResource::Neg:: Check" 
                      " when eventType is UDMA_EVENT_TYPE_MASTER and numVintr is 0U!!\n");
            retVal = UDMA_EFAIL;
            Udma_eventUnRegister(eventHandle);
        }
        else 
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 8: Check Udma_eventAllocResource when vintrBitAllocFlag bits are not available */
    if(UDMA_SOK == retVal)
    {
        chHandle                      = &chObj;
        chType                        = UDMA_CH_TYPE_TX;
        instID                        = UDMA_TEST_DEFAULT_UDMA_INST;
        drvHandle                     = &taskObj->testObj->drvObj[instID];
        backUpDrvObj                  = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum              = UDMA_PSIL_CH_MCU_CPSW0_TX;
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        Udma_ChTxPrms txChPrms;
        UdmaChTxPrms_init(&txChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigTx(chHandle, &txChPrms);
            if(UDMA_SOK == retVal)
            {
                UdmaEventPrms_init(&eventPrms);
                eventPrms.chHandle                             = chHandle;
                eventPrms.eventCb                              = &udmaTestEventCb;
                eventPrms.eventMode                            = UDMA_EVENT_MODE_SHARED;
                eventPrms.eventType                            = UDMA_EVENT_TYPE_TR;
                eventPrms.masterEventHandle                    = Udma_eventGetGlobalHandle(drvHandle);
                eventPrms.masterEventHandle->vintrBitAllocFlag = (uint64_t) 0xFFFFFFFFFFFFFFFFUL;
                retVal                                         = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: Udma_eventAllocResource::Neg:: " 
                              " Check when vintrBitAllocFlag bits are not available!!\n");
                    retVal = UDMA_EFAIL;
                    Udma_eventUnRegister(eventHandle);
                }
                else 
                {
                    retVal = UDMA_SOK;
                }
            }
            Udma_chClose(chHandle);
        }
    }
    taskObj->testObj->drvObj[instID] = backUpDrvObj;

    /* Test scenario 9: Check Udma_eventAllocResource when preferredCoreIntrNum is not valid */
    if(UDMA_SOK == retVal)
    {
        chHandle                      = &chObj;
        chType                        = UDMA_CH_TYPE_TX;
        instID                        = UDMA_TEST_DEFAULT_UDMA_INST;
        drvHandle                     = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum              = UDMA_PSIL_CH_MCU_CPSW0_TX;
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        Udma_ChTxPrms txChPrms;
        UdmaChTxPrms_init(&txChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigTx(chHandle, &txChPrms);
            if(UDMA_SOK == retVal)
            {
                UdmaEventPrms_init(&eventPrms);
                eventPrms.chHandle             = &chObj;
                eventPrms.eventCb              = &udmaTestEventCb;
                eventPrms.eventMode            = UDMA_EVENT_MODE_SHARED;
                eventPrms.eventType            = UDMA_EVENT_TYPE_TR;
                eventPrms.masterEventHandle    = NULL;
                eventPrms.preferredCoreIntrNum = UDMA_CORE_INTR_ANY +1U;
                retVal                         = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: Udma_eventAllocResource::Neg::" 
                              " Check when preferredCoreIntrNum is not valid!!\n");
                    retVal = UDMA_EFAIL;
                    Udma_eventUnRegister(eventHandle);
                }
                else 
                {
                    retVal = UDMA_SOK;
                }
            }
            Udma_chClose(chHandle);
        }
    }

    /* Test scenario 10: Check Udma_eventAllocResource when eventType is UDMA_EVENT_TYPE_TR 
     *                   and chOesAllocDone is UTRUE */
    if(UDMA_SOK == retVal)
    {
        chHandle                      = &chObj;
        chType                        = UDMA_CH_TYPE_TX;
        instID                        = UDMA_TEST_DEFAULT_UDMA_INST;
        drvHandle                     = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum              = UDMA_PSIL_CH_MCU_CPSW0_TX;
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        Udma_ChTxPrms txChPrms;
        UdmaChTxPrms_init(&txChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigTx(chHandle, &txChPrms);
            if(UDMA_SOK == retVal)
            {
                UdmaEventPrms_init(&eventPrms);
                eventPrms.chHandle                 = &chObj;
                eventPrms.eventCb                  = &udmaTestEventCb;
                eventPrms.eventMode                = UDMA_EVENT_MODE_SHARED;
                eventPrms.eventType                = UDMA_EVENT_TYPE_TR;
                eventPrms.masterEventHandle        = NULL;
                eventPrms.chHandle->chOesAllocDone = UTRUE;
                retVal                             = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: Udma_eventAllocResource::Neg:: Check when" 
                              " eventType is UDMA_EVENT_TYPE_TR and chOesAllocDone is UTRUE!!\n");
                    retVal = UDMA_EFAIL;
                    Udma_eventUnRegister(eventHandle);
                }
                else 
                {
                    retVal = UDMA_SOK;
                }
            }
            Udma_chClose(chHandle);
        }
    }

    /* Test scenario 11: Check Udma_eventCheckUnRegister to free-up master event when 
     *                   shared events are still not yet unregistered */
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
            eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
            eventPrms.monHandle         = monHandle;
            eventPrms.chHandle          = chHandle;
            eventPrms.appData           = testChObj;
            retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
            if(UDMA_SOK == retVal)
            {
                retVal = Udma_eventUnRegister(eventPrms.masterEventHandle);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                                " |TEST INFO|:: FAIL:: UDMA:: Udma_eventCheckUnRegister::" 
                                " Neg:: Check to free-up master event when shared"
                                " events are still not yet unregistered!!\n");
                    retVal = UDMA_EFAIL;
                }
                else 
                {
                    retVal = UDMA_SOK;
                    Udma_eventUnRegister(eventHandle);
                }
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

    /* Test scenario 12: Check Udma_eventAllocResource when masterEventHandle is NULL 
     *                   and irIntr is not available. */
    if(UDMA_SOK == retVal)
    {
        instID                      = UDMA_TEST_INST_ID_MAIN_0;
        eventHandle->drvHandle      = &taskObj->testObj->drvObj[instID];
        backUpDrvObj                = taskObj->testObj->drvObj[instID];
        UdmaEventPrms_init(&eventPrms);
        eventPrms.chHandle          = &chObj;
        eventPrms.eventCb           = &udmaTestEventCb;
        eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        eventPrms.eventType         = UDMA_EVENT_TYPE_MASTER;
        eventPrms.masterEventHandle = NULL;
         /* Udma driver will utilize 1-bit per resource of 32-bit element size array to maintain availability status.
              since each element is 32-bit, no.of elements required is:
                         "numRes/32" if numRes is multiple of 32 else "numRes/32 + 1".               
        */
        resArrSize = eventHandle->drvHandle->initPrms.rmInitPrms.numIrIntr >> 5; 
        if(eventHandle->drvHandle->initPrms.rmInitPrms.numIrIntr - (resArrSize << 5) != 0)
        {
            resArrSize += 1U;
        }
        for(resArrIdx = 0U; resArrIdx < resArrSize; resArrIdx++)
        {
            eventHandle->drvHandle->irIntrFlag[resArrIdx] = 0U;
        }
        retVal = Udma_eventRegister(eventHandle->drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventAllocResource::Neg:: " 
                      " Check when masterEventHandle is NULL and irIntr is not available!!\n");
            retVal = UDMA_EFAIL;
            Udma_eventUnRegister(eventHandle);
        }
        else 
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 13: Check Udma_eventAllocResource when eventMode is neither 
     *                   shared nor exclusive */
    if(UDMA_SOK == retVal)
    {
        instID                      = UDMA_TEST_INST_ID_MAIN_0;
        eventHandle->drvHandle      = &taskObj->testObj->drvObj[instID];
        UdmaEventPrms_init(&eventPrms);
        eventPrms.chHandle          = &chObj;
        eventPrms.eventCb           = &udmaTestEventCb;
        eventPrms.eventMode         = 0xFFFFFFFFU;
        eventPrms.eventType         = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
        eventPrms.masterEventHandle = NULL;
        retVal                      = Udma_eventRegister(eventHandle->drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventAllocResource::Neg:: " 
                      " Check when eventMode is neither shared nor exclusive!!\n");
            retVal = UDMA_EFAIL;
            Udma_eventUnRegister(eventHandle);
        }
        else 
        {
            retVal = UDMA_SOK;
        }
    }

    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_eventRegister
 * Test scenario 1: Check Udma_eventCheckParams when eventType is UDMA_EVENT_TYPE_DMA_COMPLETION 
 *                  and eventCb for masterEventHandle is NULL
 */
int32_t UdmaEventTypeDmaCompletionTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    uint32_t               instID;
    Udma_DrvHandle         drvHandle;
    struct Udma_EventObj   eventObj;
    Udma_EventHandle       eventHandle;
    Udma_EventPrms         eventPrms;
    struct Udma_DrvObj     backUpDrvObj;

    /* Test scenario 1: Check Udma_eventCheckParams when eventType is UDMA_EVENT_TYPE_DMA_COMPLETION 
     *                  and eventCb for masterEventHandle is NULL */

    instID       = UDMA_TEST_DEFAULT_UDMA_INST;
    drvHandle    = &taskObj->testObj->drvObj[instID];
    backUpDrvObj = taskObj->testObj->drvObj[instID];
    UdmaEventPrms_init(&eventPrms);
    eventHandle                                      = &eventObj;
    eventPrms.eventCb                                = &udmaTestEventDmaCb;
    eventPrms.eventMode                              = UDMA_EVENT_MODE_SHARED;
    eventPrms.masterEventHandle                      = Udma_eventGetGlobalHandle(drvHandle);
    eventPrms.masterEventHandle->eventPrms.eventCb   = NULL_PTR;
    eventPrms.masterEventHandle->eventPrms.eventType = UDMA_EVENT_TYPE_DMA_COMPLETION;
    retVal                                           = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_eventCheckParams::Neg:: Check when eventType is" 
                  " UDMA_EVENT_TYPE_DMA_COMPLETION and eventCb for masterEventHandle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }
    else 
    {
        retVal = UDMA_SOK;
    }
    taskObj->testObj->drvObj[instID] = backUpDrvObj;

    return retVal;
}

static void udmaTestEventDmaCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData)
{
    if(UDMA_EVENT_TYPE_DMA_COMPLETION == eventType)
    {
        SemaphoreP_post(gUdmaTestAppDoneSem);
    }
    else
    {
        gUdmaTestAppResult = UDMA_EFAIL;
    }

    return;
}

/* 
 * Test Case Description: Verifies the function Udma_eventGetRxFlowIdFwStatus and Udma_eventRegister
 * Test scenario 1: Check Udma_eventGetRxFlowIdFwStatus when instType is not UDMA_INST_TYPE_NORMAL
 * Test scenario 2: Check Udma_eventConfig when eventType is UDMA_EVENT_TYPE_RING_MON 
 *                  and instType is not UDMA_INST_TYPE_NORMAL
 * Test scenario 3: Check Udma_eventConfig when eventType is UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW
 *                  and instType is UDMA_INST_TYPE_LCDMA_BCDMA 
 * Test scenario 4: Check Udma_eventCheckUnRegister to unregister the event when descriptors 
 *                  are present in the ring 
 * Test scenario 5: Check Udma_eventCheckUnRegister when eventInitDone is not UDMA_INIT_DONE 
 */
int32_t UdmaEventBcdmaInstTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                    retVal = UDMA_SOK;
    struct Udma_EventObj       eventObj;
    Udma_EventHandle           eventHandle;
    Udma_EventRxFlowIdFwStatus status;
    Udma_RingMonHandle         monHandle;
    struct Udma_RingMonObj     monObj;
    Udma_DrvHandle             drvHandle;
    Udma_EventPrms             eventPrms;
    uint32_t                   instID;
    uint32_t                   heapIdSrc = UTILS_MEM_HEAP_ID_MSMC;
    uint32_t                   elemCnt = 50U;
    UdmaTestChObj             *testChObj;
    uint32_t                   timeout=0U;
    uint32_t                   trpdSize = (sizeof(CSL_UdmapTR15) * 2U) + 4U;
    uint8_t                   *trpdMem;
    struct Udma_ChObj          chObj;
    Udma_ChHandle              chHandle;
    Udma_ChPrms                chPrms;
    uint32_t                   chType;
    uint64_t                   pDesc;

    /* Test scenario 1: Check when instType is not UDMA_INST_TYPE_NORMAL */
    eventHandle                = &eventObj;
    instID                     = UDMA_TEST_INST_ID_BCDMA_0;
    eventHandle->drvHandle     = &taskObj->testObj->drvObj[instID];
    eventHandle->eventInitDone = UDMA_INIT_DONE;
    retVal                     = Udma_eventGetRxFlowIdFwStatus(eventHandle, &status);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                    " |TEST INFO|:: FAIL:: UDMA:: Udma_eventGetRxFlowIdFwStatus::" 
                    " Neg:: Check when instType is not UDMA_INST_TYPE_NORMAL!!\n");
        retVal = UDMA_EFAIL;
    }
    else 
    {
        retVal = UDMA_SOK;
    }

    /* Test scenario 2: Check Udma_eventConfig when eventType is UDMA_EVENT_TYPE_RING_MON 
     *                  and instType is not UDMA_INST_TYPE_NORMAL */
    eventHandle                 = &eventObj;
    drvHandle                   = &taskObj->testObj->drvObj[instID];
    monHandle                   = &monObj;
    UdmaEventPrms_init(&eventPrms);
    eventPrms.eventCb           = &udmaTestEventCb;
    eventPrms.eventMode         = UDMA_EVENT_MODE_EXCLUSIVE;
    eventPrms.eventType         = UDMA_EVENT_TYPE_RING_MON;
    eventPrms.masterEventHandle = NULL;
    eventPrms.monHandle         = monHandle;
    retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_eventConfig::Neg:: Check when eventType" 
                  " is UDMA_EVENT_TYPE_RING_MON and instType is not UDMA_INST_TYPE_NORMAL!!\n");
        retVal = UDMA_EFAIL;
    }
    else 
    {
        retVal                     = UDMA_SOK;
        eventHandle->eventInitDone = UDMA_INIT_DONE;
        retVal = Udma_eventUnRegister(eventHandle);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventReset::Neg:: Check when eventType" 
                      " is UDMA_EVENT_TYPE_RING_MON and instType is not UDMA_INST_TYPE_NORMAL!!\n");
            retVal = UDMA_EFAIL;
        }
        else 
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 3: Check Udma_eventConfig when eventType is UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW
     *                  and instType is UDMA_INST_TYPE_LCDMA_BCDMA */
    if(UDMA_SOK == retVal)
    {
        drvHandle                   = &taskObj->testObj->drvObj[instID];
        monHandle                   = &monObj;
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventCb           = &udmaTestEventCb;
        eventPrms.eventMode         = UDMA_EVENT_MODE_EXCLUSIVE;
        eventPrms.eventType         = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
        eventPrms.masterEventHandle = NULL;
        eventPrms.monHandle         = monHandle;
        retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_eventConfig::Neg:: Check when" 
                      " eventType is UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW"
                      " and instType is UDMA_INST_TYPE_LCDMA_BCDMA!!\n");
            retVal = UDMA_EFAIL;
        }
        else 
        {
            retVal                     = UDMA_SOK;
            eventHandle->eventInitDone = UDMA_INIT_DONE;
            retVal = Udma_eventUnRegister(eventHandle);
            if(UDMA_SOK == retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: Udma_eventReset::Neg:: Check when eventType" 
                          " is UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW and instType is not UDMA_INST_TYPE_NORMAL!!\n");
                retVal = UDMA_EFAIL;
            }
            else 
            {
                retVal = UDMA_SOK;
            }
        }
    }

    /* Test scenario 4: Check Udma_eventCheckUnRegister to unregister the event when descriptors are present in the ring */
    if(UDMA_SOK == retVal)
    {
        chHandle                      = &chObj;
        chType                        = UDMA_CH_TYPE_TX;
        instID                        = UDMA_TEST_INST_ID_BCDMA_0;
        drvHandle                     = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.elemCnt     = elemCnt;
        chPrms.fqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
        chPrms.fqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.fqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        chPrms.cqRingPrms.elemCnt     = elemCnt;
        chPrms.cqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
        chPrms.cqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.cqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        chPrms.peerChNum              = UDMA_PSIL_CH_MCU_CPSW0_TX;
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        Udma_ChTxPrms txChPrms;
        UdmaChTxPrms_init(&txChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigTx(chHandle, &txChPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chConfigTx failed!!\n");
            }
            retVal = Udma_chEnable(chHandle);
            if(UDMA_SOK == retVal)
            {
                UdmaEventPrms_init(&eventPrms);
                testChObj                   = taskObj->chObj[0];
                eventPrms.chHandle          = chHandle;
                eventPrms.eventCb           = &udmaTestEventDmaCb;
                eventPrms.eventMode         = UDMA_EVENT_MODE_EXCLUSIVE;
                eventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
                eventPrms.masterEventHandle = NULL;
                eventPrms.appData           = testChObj;
                retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
                if(UDMA_SOK == retVal)
                {
                    trpdMem = Utils_memAlloc(UTILS_MEM_HEAP_ID_DDR, trpdSize, UDMA_CACHELINE_ALIGNMENT);
                    memset(trpdMem, 0, trpdSize);

                    /* Submit TRPD to channel */
                    retVal = Udma_ringQueueRaw(
                                Udma_chGetFqRingHandle(chHandle), (uint64_t) Udma_appVirtToPhyFxn(trpdMem, 0U, NULL));
                }   
                if(UDMA_SOK == retVal)
                {
                    retVal = Udma_eventUnRegister(eventHandle);
                    if(UDMA_SOK == retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                " |TEST INFO|:: FAIL:: UDMA:: Udma_eventCheckUnRegister::Neg::" 
                                " Check to unregister the event when descriptors are present in the ring!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                        pDesc  = Udma_appVirtToPhyFxn(trpdMem, 0U, NULL);
                        retVal = Udma_ringDequeueRaw(Udma_chGetCqRingHandle(chHandle), &pDesc);
                        if(UDMA_SOK == retVal)
                        {
                            Udma_eventUnRegister(eventHandle);
                        }
                    }
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

    /* Test scenario 5: Check Udma_eventCheckUnRegister when eventInitDone is not UDMA_INIT_DONE */
    if(UDMA_SOK == retVal)
    {
        eventHandle->drvHandle = &taskObj->testObj->drvObj[instID];
        retVal                 = Udma_eventUnRegister(eventHandle);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                        " |TEST INFO|:: FAIL:: UDMA:: Udma_eventCheckUnRegister::Neg::" 
                        " Check when eventInitDone is not UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        else 
        {
            retVal = UDMA_SOK;
        }
    }

    return retVal;
}

static void * UdmaOsalRegIntDummy(Udma_OsalIsrFxn isrFxn,
                                   uint32_t coreIntrNum,
                                   uint32_t intrPriority,
                                   void *arg)
{
    Udma_EventHandle    eventHandle = (Udma_EventHandle) arg;
    Udma_eventUnRegister(eventHandle);
    return NULL_PTR;
}

/* 
 * Test Case Description: Verifies the function Udma_eventRegister
 * Test scenario 1: Check Udma_eventConfig when osal interrupt registration fails
 */
int32_t UdmaEventConfigTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    uint32_t               instID;
    Udma_DrvHandle         drvHandle;
    struct Udma_EventObj   eventObj;
    Udma_EventHandle       eventHandle;
    Udma_EventPrms         eventPrms;
    struct Udma_DrvObj     backUpDrvObj;
    struct Udma_ChObj      chObj;
    Udma_ChHandle          chHandle;
    Udma_ChPrms            chPrms;
    uint32_t               chType;
    uint32_t               heapIdSrc = UTILS_MEM_HEAP_ID_MSMC;
    uint32_t               elemCnt = 50U;
    UdmaTestChObj         *testChObj;
    uint32_t               timeout=0U;

    /* Test scenario 1: Check Udma_eventConfig when osal interrupt registration fails */
    chHandle                      = &chObj;
    chType                        = UDMA_CH_TYPE_TR_BLK_COPY;
    instID                        = UDMA_TEST_INST_ID_MAIN_0;
    backUpDrvObj                  = taskObj->testObj->drvObj[instID];
    drvHandle                     = &taskObj->testObj->drvObj[instID];
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
        retVal  = Udma_chConfigTx(chHandle, &txChPrms);
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
            drvHandle->initPrms.osalPrms.registerIntr = &UdmaOsalRegIntDummy;
            UdmaEventPrms_init(&eventPrms);
            eventHandle                 = &eventObj;
            testChObj                   = taskObj->chObj[0];
            eventPrms.appData           = testChObj;
            eventPrms.chHandle          = chHandle;
            eventPrms.eventCb           = &udmaTestEventDmaCb;
            eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
            eventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
            eventPrms.masterEventHandle = NULL;
            eventPrms.coreIntrNum       = UDMA_CORE_INTR_ANY;
            retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
            if(UDMA_SOK == retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                            " |TEST INFO|:: FAIL:: UDMA:: Udma_eventConfig::Neg::" 
                            " Check when osal interrupt registration fails!!\n");
                retVal = UDMA_EFAIL;
                Udma_eventUnRegister(eventHandle);
            }
            else
            {
                retVal = UDMA_SOK;
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
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    return retVal;
}

static void udmaTestEventCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData)
{
    SemaphoreP_Handle transferDoneSem = (SemaphoreP_Handle) appData;

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

