/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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

/*!
 * \file     enet_test_entry.c
 *
 * \brief    Enet LLD UT test entry.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include "enet_test_entry.h"
#include "enet_test_base.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#define APP_TSK_STACK_MAIN                    (10U * 1024U)
/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* Task function */
// static void taskFxn(void *a0, void *a1);

static EnetTest_ClkHandle EnetTest_createClock(EnetTestTaskObj *taskObj);

static void EnetTest_delete(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
static uint8_t gAppTskStackTick[APP_TSK_STACK_MAIN]
__attribute__ ((aligned(32)));
volatile uint32_t exitFlag = UFALSE;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void CpswUt_testCreatePhyEvent(EnetTestTaskObj *taskObj)
{
    EventP_Params evtPrms;

    EventP_Params_init(&evtPrms);
    taskObj->stateObj.phyEvent = EventP_create(&evtPrms);
}

static void CpswUt_testDeletePhyEvent(EnetTestTaskObj *taskObj)
{
    EventP_delete(&taskObj->stateObj.phyEvent);
    taskObj->stateObj.phyEvent = NULL;
}

static void CpswUt_testCreatePortEvent(EnetTestTaskObj *taskObj)
{
    EventP_Params evtPrms;

    EventP_Params_init(&evtPrms);
    taskObj->stateObj.portEvent = EventP_create(&evtPrms);
}

static void CpswUt_testDeletePortEvent(EnetTestTaskObj *taskObj)
{
    EventP_delete(&taskObj->stateObj.portEvent);
    taskObj->stateObj.portEvent = NULL;
}

static void  CpswUt_initStateObj(EnetTestStateObj *stateObj)
{
    uint32_t i;

    memset(stateObj->rxFlowObj, 0, sizeof(stateObj->rxFlowObj));
    memset(stateObj->txChObj, 0, sizeof(stateObj->txChObj));
    stateObj->phyEvent          = NULL;
    stateObj->hEnet             = NULL;
    stateObj->task_periodicTick = NULL;
    stateObj->hTimer            = NULL;
    stateObj->timerSem          = NULL;
    stateObj->coreId            = EnetSoc_getCoreId();
    stateObj->coreKey           = ENET_RM_INVALIDCORE;
    stateObj->noPhyPortMask     = 0U;

    for (i = 0U; i < ENET_ARRAYSIZE(stateObj->rxFlowObj); i++)
    {
        EnetQueue_initQ(&stateObj->rxFlowObj[i].rxFreeQ);
        EnetQueue_initQ(&stateObj->rxFlowObj[i].rxReadyQ);
    }

    for (i = 0U; i < ENET_ARRAYSIZE(stateObj->txChObj); i++)
    {
        EnetQueue_initQ(&stateObj->txChObj[i].txFreePktInfoQ);
    }
}

int32_t CpswUt_testEntry(EnetTestTaskObj *taskObj)
{
    EnetTest_ClkHandle clkhandle;
    int retVal = ENET_EFAIL;

    CpswUt_initStateObj(&taskObj->stateObj);
    CpswUt_testCreatePhyEvent(taskObj);
    CpswUt_testCreatePortEvent(taskObj);

    clkhandle = EnetTest_createClock(taskObj);
    if (clkhandle != NULL)
    {
        retVal = EnetTestCommon_Init(taskObj);
        if (ENET_SOK == retVal)
        {
            int32_t deInitRetVal;

            /* Start timer */
            EnetTest_startClock(clkhandle);
            retVal = taskObj->taskCfg->runTest(taskObj);
            /* Stop timer */
            EnetTest_stopClock(clkhandle);
            if (NULL != taskObj->taskCfg->deInitTest)
            {
                deInitRetVal = taskObj->taskCfg->deInitTest(taskObj);
            }
            else
            {
                deInitRetVal = EnetTestCommon_DeInit(taskObj);
            }

            if (ENET_SOK == retVal)
            {
                /* If test run was successful use the deinit retval as return status for test
                 * If test run failed , use the run fail status as return status for test
                 */
                retVal = deInitRetVal;
            }
        }
    }

    CpswUt_testDeletePhyEvent(taskObj);
    CpswUt_testDeletePortEvent(taskObj);
    EnetTest_delete(taskObj);

    return(retVal);
}

static void EnetTest_timerCallback(void *arg)
{
    SemaphoreP_Handle timerSem = (SemaphoreP_Handle)arg;

    /* Tick! */
    SemaphoreP_post(timerSem);
}

static void EnetTest_periodicTick(void *a0,
                                   void *a1)
{
    SemaphoreP_Handle timerSem = (SemaphoreP_Handle)a0;

    while (!exitFlag)
    {
        SemaphoreP_pend(timerSem, SemaphoreP_WAIT_FOREVER);
        /* Cpsw_periodicTick should be called from non-ISR context */

        EnetTestTaskObj *taskObj  = (EnetTestTaskObj *)a1;
        Enet_periodicTick(EnetTestCommon_getCpswHandle(taskObj));
    }
}

static EnetTest_ClkHandle EnetTest_createClock(EnetTestTaskObj *taskObj)
{
    TaskP_Params params;
    SemaphoreP_Params semParams;
    ClockP_Params clkParams;
    uint32_t period = 100U; /* msecs */

    SemaphoreP_Params_init(&semParams);
    semParams.mode             = SemaphoreP_Mode_COUNTING;
    taskObj->stateObj.timerSem = SemaphoreP_create(0, &semParams);

    /* Initialize the taskperiodicTick params. Set the task priority higher than
     * the
     * default priority (1) */
    TaskP_Params_init(&params);
    params.priority       = 7U;
    params.stack          = gAppTskStackTick;
    params.stacksize      = sizeof(gAppTskStackTick);
    params.arg0           = (void *)taskObj->stateObj.timerSem;
    params.arg1           = (void *)taskObj;
    params.name           = (const char *)"UT Periodic Tick";

    taskObj->stateObj.task_periodicTick =
        TaskP_create(&EnetTest_periodicTick, &params);
    if (taskObj->stateObj.task_periodicTick == NULL)
    {
        OS_stop();
    }

    ClockP_Params_init(&clkParams);
    clkParams.startMode = ClockP_StartMode_USER;
    clkParams.period    = period;
    clkParams.runMode   = ClockP_RunMode_CONTINUOUS;
    clkParams.arg       = (void *)taskObj->stateObj.timerSem;

    /* Creating timer and setting timer callback function*/
    taskObj->stateObj.hTimer = ClockP_create((void *)&EnetTest_timerCallback,
                                            &clkParams);
    if (taskObj->stateObj.hTimer == NULL)
    {
       EnetAppUtils_print("EnetTest_createClock() failed to create clock: %d\n");
    }

    return (EnetTest_ClkHandle)taskObj->stateObj.hTimer;
}

void EnetTest_startClock(EnetTest_ClkHandle handle)
{
    ClockP_Handle clkHandle = (ClockP_Handle)handle;

    /* Start timer */
    ClockP_start(clkHandle);
}

void EnetTest_stopClock(EnetTest_ClkHandle handle)
{
    ClockP_Handle clkHandle = (ClockP_Handle)handle;

    /* Stop and delete the tick timer */
    ClockP_stop(clkHandle);
}

void EnetTest_wait(uint32_t waitTime)
{
    TaskP_sleep(waitTime);
}

static void EnetTest_delete(EnetTestTaskObj *taskObj)
{
    if (taskObj->stateObj.hTimer != NULL)
    {
        ClockP_delete(taskObj->stateObj.hTimer);
    }

    if (taskObj->stateObj.timerSem != NULL)
    {
        SemaphoreP_delete(taskObj->stateObj.timerSem);
    }

    if (taskObj->stateObj.task_periodicTick != NULL)
    {
        TaskP_delete(&taskObj->stateObj.task_periodicTick);
    }
}
