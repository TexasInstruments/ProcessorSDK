/*
 *  Copyright (c) Texas Instruments Incorporated 2018
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
 *  \file main_tirtos.c
 *
 *  \brief Main file for TI-RTOS build
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdio.h>
#include <ti/board/board.h>

#include "ti/osal/osal.h"
#if defined(FREERTOS)
#include "ti/osal/LoadP.h"
#endif
#include "ti/osal/SemaphoreP.h"
#include "ti/osal/TaskP.h"
#include <st_mcan.h>
#include <utils_prf.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**< Demo application stack size */
#define CAPT_APP_TSK_STACK_MAIN         (10U * 1024U)

/**< Demo application task priority */
#define CAPT_APP_TSK_PRI_MAIN           (10U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
static void taskFxn(void* a0, void* a1);
int32_t main_task(void);
void enbaleIsrPrint(uint32_t status);
void st_mcanCallTxFunc(st_mcanTestcaseParams_t *testParams);
static void mcanTestTask(void *arg0, void *arg1);
int32_t mcanTestCreateTestTasks(st_mcanTestcaseParams_t *testParams, TaskP_Handle *taskHandle);
extern int32_t st_mcanTxApp_main(st_mcanTestcaseParams_t *testParams);
void App_ConsolePrintf(uint32_t type, uint32_t baseAddr, const char *pcString, ...);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/**< Align stack memory to integer boundary. */
static uint8_t gCaptAppTskStackMain[CAPT_APP_TSK_STACK_MAIN]
                    __attribute__ ((section (".bss:taskStackSection")))
                    __attribute__((aligned(32)));
extern volatile uint32_t isrPrintEnable;
extern uint32_t                uartBaseAddr;
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int main(void)
{
    TaskP_Handle task;
    TaskP_Params taskParams;

    OS_init();

#if defined(FREERTOS)
    /* Reset Load measurement */
    LoadP_reset();
#endif

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority      = CAPT_APP_TSK_PRI_MAIN;
    taskParams.stack         = gCaptAppTskStackMain;
    taskParams.stacksize     = sizeof (gCaptAppTskStackMain);

    task = TaskP_create(&taskFxn, &taskParams);
    if(NULL == task)
    {
        OS_stop();
    }
    OS_start();    /* does not return */

    return(0);
}

static void taskFxn(void* a0, void* a1)
{
    main_task();
    return;
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

void enbaleIsrPrint(uint32_t status)
{
    /* Cant enable prints in ISR for RTOS application. */
    isrPrintEnable = FALSE;
}

void st_mcanCallTxFunc(st_mcanTestcaseParams_t *testParams)
{
	TaskP_Handle        taskHandle = NULL;
#if defined(FREERTOS)
    LoadP_Status        status = LoadP_OK;
    LoadP_Stats         loadStatsTask;
#endif

    mcanTestCreateTestTasks(testParams, &taskHandle);

#if defined(FREERTOS)
    status = LoadP_getTaskLoad(taskHandle, &loadStatsTask);
    if(LoadP_OK == status)
    {
        if(loadStatsTask.percentLoad > 0U)
        {
            App_ConsolePrintf((uint32_t)PRINT_MSG_TYPE_NORMAL, uartBaseAddr, "LoadP_getTaskLoad :    %s - Load = %d%% \n", loadStatsTask.name, loadStatsTask.percentLoad);
        }
        else
        {
            /* Load less than 1%, Try to get fractional part */
            App_ConsolePrintf((uint32_t)PRINT_MSG_TYPE_NORMAL, uartBaseAddr, "\n LoadP_getTaskLoad :   %s - Load = 0.000%d%% \n", loadStatsTask.name, loadStatsTask.threadTime/(loadStatsTask.totalTime/(100*1000)));
        }
    }
    else
    {
        App_ConsolePrintf((uint32_t)PRINT_MSG_TYPE_NORMAL, uartBaseAddr, "Failed to get tasks load measurement \n");
    }
#endif

    if(NULL != taskHandle)
    {
        TaskP_delete(&taskHandle);
    }
}

/* Test application stack size */
#define APP_TSK_STACK_MAIN              (16U * 1024U)
static uint8_t  gMcanParserTskStack[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));;

int32_t mcanTestCreateTestTasks(st_mcanTestcaseParams_t *testParams, TaskP_Handle *taskHandle)
{
    int32_t             retVal = STW_SOK;
    TaskP_Params        taskPrms;
    SemaphoreP_Params   semPrms;
    SemaphoreP_Handle   taskCompleteSem;

    SemaphoreP_Params_init(&semPrms);
    semPrms.mode = SemaphoreP_Mode_COUNTING;
    taskCompleteSem = SemaphoreP_create(0U, &semPrms);
    if(NULL == taskCompleteSem)
    {
        App_ConsolePrintf((uint32_t)PRINT_MSG_TYPE_STATUS, uartBaseAddr, " Sem create failed\r\n");
        retVal = STW_EFAIL;
    }

    if(STW_SOK == retVal)
    {
        TaskP_Params_init(&taskPrms);
        taskPrms.arg0 = testParams;
        taskPrms.arg1 = taskCompleteSem;
        taskPrms.stack = &gMcanParserTskStack[0U];
        taskPrms.stacksize = APP_TSK_STACK_MAIN;
        *taskHandle = TaskP_create(&mcanTestTask, &taskPrms);
        if(NULL == *taskHandle)
        {
            retVal = STW_EFAIL;
        }
    }

    if(STW_SOK == retVal)
    {
        /* Wait for tasks to complete */
        SemaphoreP_pend(taskCompleteSem, SemaphoreP_WAIT_FOREVER);
    }

    if(NULL != taskCompleteSem)
    {
        SemaphoreP_delete(taskCompleteSem);
        taskCompleteSem = NULL;
    }

    return (retVal);
}

static void mcanTestTask(void *arg0, void *arg1)
{
    st_mcanTestcaseParams_t *testParams;
    SemaphoreP_Handle   taskCompleteSem;

    /* Run the test */
    testParams = (st_mcanTestcaseParams_t *) arg0;
    taskCompleteSem = (SemaphoreP_Handle) arg1;

    /* Run the test */
    st_mcanTxApp_main(testParams);

    /* Test complete. Signal it */
    SemaphoreP_post(taskCompleteSem);

    return;
}
