/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   osal_extended_testapp_task.c
 *
 *  \brief  OSAL task Sub Module testcases file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined (FREERTOS)
#define OSAL_APP_MAX_TASK         (OSAL_FREERTOS_MAX_TASKP_PER_SOC)
#elif defined (SAFERTOS)
#define OSAL_APP_MAX_TASK         (OSAL_SAFERTOS_MAX_TASKP_PER_SOC)
#endif
#define OSAL_APP_TASK_STACK_SIZE  (2*1024U)
#define OSAL_APP_PRIORITY_HIGHEST (17)
#define OSAL_APP_PRIORITY_LOWEST  (-1)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
#if defined(BUILD_C7X)
uint8_t      gOsalAppTskStack[OSAL_APP_MAX_TASK][OSAL_APP_TASK_STACK_SIZE] __attribute__((aligned( 0x2000 )));
uint32_t     gstackSize = sizeof(gOsalAppTskStack);
#else
uint8_t      gOsalAppTskStack[OSAL_APP_MAX_TASK][OSAL_APP_TASK_STACK_SIZE] __attribute__((aligned(OSAL_APP_TASK_STACK_SIZE)));
uint32_t     gstackSize = OSAL_APP_TASK_STACK_SIZE;
#endif
TaskP_Handle gOsalAppTaskPSelfmacroTaskHandle;
uint32_t     gOsalAppMakeProgress = UFALSE;

/* ========================================================================== */
/*                            Function Declarations                           */
/* ========================================================================== */

#if defined(BUILD_MCU)
/*
 * Description  : Test the following:
 *                  1) Create tasks with priorities lower than lowest supported.
 *                  2) Create tasks with priorities higher than highest supported.
 *                  3) Create tasks more than supported number of tasks.
 */
static int32_t OsalApp_taskCreateMaxTaskTest(void);
#endif

/*
 * Description  : Test the following:
 *                  1) TaskP_sleepInMsecs()
 *                  2) TaskP_sleep()
 *                  3) TaskP_selfmacro()
 */
static int32_t OsalApp_taskGeneralTests(void);

/*
 * Description  : Test the TaskP_isTerminated API
 */
static int32_t OsalApp_taskIsTerminated(void);

/*
 * Description : Testing Negative checks for Task APIs
 */
static int32_t OsalApp_taskNegativeTests(void);

#if defined(SAFERTOS)
extern portTaskHandleType TaskP_getSafeRTOSHandle( TaskP_Handle handle );
#endif

/* ========================================================================== */
/*                     Internal Function Definitions                          */
/* ========================================================================== */

static void OsalApp_dummytaskFxn(void *arg)
{
    /* Do Nothing */
}

static void OsalApp_taskSelfHandleTestTask(void *arg)
{
    if(gOsalAppTaskPSelfmacroTaskHandle == TaskP_selfmacro())
    {
        gOsalAppMakeProgress = UTRUE;
    }

    TaskP_delete(&gOsalAppTaskPSelfmacroTaskHandle);
}

#if defined(BUILD_MCU)
/*
Description  : Creating OSAL_APP_MAX_TASK number of tasks tp test the when tasks count go beyond limit
*/
static int32_t OsalApp_taskCreateMaxTaskTest(void)
{
    TaskP_Params params;
    TaskP_Handle tskHandles[OSAL_APP_MAX_TASK - 1];
    uint32_t taskIter;
    int32_t result = osal_OK;

    TaskP_Params_init(&params);
    params.stacksize = gstackSize;

    /* Create one task lesser than max supported, as current task is already getting executed  */
    for(taskIter = 0U; taskIter < OSAL_APP_MAX_TASK - 1; taskIter++)
    {
        /* Set priority as lower the lowest and higher than highest.
         * Lowest is 0 and highest is 15 supported by OSAL.
         * params.priority is an int, so negative numbers are welcome.
         */
        params.priority = OSAL_APP_PRIORITY_LOWEST - 1;
        params.stack    = gOsalAppTskStack[taskIter];
        tskHandles[taskIter] = TaskP_create((TaskP_Fxn)OsalApp_dummytaskFxn,(void *)&params);
        if(NULL == tskHandles[taskIter])
        {
            result = osal_FAILURE;
            break;
        }
    }

    if(osal_OK == result)
    {
        if(NULL != TaskP_create((TaskP_Fxn)OsalApp_dummytaskFxn,(void *)&params))
        {
            result = osal_FAILURE;
        }
        if(NULL_PTR == TaskP_self())
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        for(taskIter = 0U; taskIter < OSAL_APP_MAX_TASK - 1; taskIter++)
        {
            if(TaskP_OK != TaskP_delete(&tskHandles[taskIter]))
            {
                result = osal_FAILURE;
                break;
            }
        }
    }

    if(osal_OK != result)
    {
        OSAL_log("Creating max tasks test has failed!!\n");
    }

    return result;
}
#endif

/*
Description  : Creating a task having priority more than zero to fulfill condition TaskP_PRIORITY_HIGHEST < taskPriority
               calling TaskP_sleepInMsecs to configure sleep time in msec.
               Calling TaskP_self to get handle of current task
               TaskP_selfmacro calls TaskP_self
               finally calling taskp sleep
*/
static int32_t OsalApp_taskGeneralTests(void)
{
    TaskP_Params params;
    uint32_t start, end, testTimeout = 100000U;
    int32_t result = osal_OK;

    TaskP_Params_init(&params);
    params.priority = 1;
    memset(gOsalAppTskStack, 0, sizeof(gOsalAppTskStack));
    params.stack    = gOsalAppTskStack[0];
    params.stacksize = gstackSize;

    /* Test tasks sleeping for corect amount of time. */
    start = TimerP_getTimeInUsecs();
    TaskP_sleepInMsecs(100);
    TaskP_sleep(100);
    end = TimerP_getTimeInUsecs();

    /* LDRA instrumentation adds a lot of code into the OSAL library.
     * This increases execution time of these APIs and hence, the measured *time gets hampered.
     * Hence, increase the tolerance of time measurement. */
    if((end - start) > (10*(200000+5000)))
    {
        result = osal_FAILURE;
    }

    gOsalAppTaskPSelfmacroTaskHandle = TaskP_create((TaskP_Fxn)OsalApp_taskSelfHandleTestTask, &params);

    TaskP_setPrio(gOsalAppTaskPSelfmacroTaskHandle, 3);
    while((0U == gOsalAppMakeProgress) && (testTimeout--))
    {
      /* Do Nothing */
    }

    if((UTRUE != gOsalAppMakeProgress) || (NULL == gOsalAppTaskPSelfmacroTaskHandle))
    {
        result = osal_FAILURE;
    }
#if defined(FREERTOS)
    if(0U == TaskP_getTaskStackHighWatermark(gOsalAppTaskPSelfmacroTaskHandle))
#elif defined(SAFERTOS)
    /* SafeRTOS doesn't support WaterMark functionality, always returns zero */
    if(0U != TaskP_getTaskStackHighWatermark(gOsalAppTaskPSelfmacroTaskHandle))
#endif
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("Generel TaskP test has failed!!\n");
    }

    return result;
}

static int32_t OsalApp_taskIsTerminated(void)
{
    TaskP_Params params;
    TaskP_Handle handle;
    int32_t result = osal_OK;

    TaskP_Params_init(&params);
    params.priority = 1;
    memset(gOsalAppTskStack, 0, sizeof(gOsalAppTskStack));
    params.stack    = gOsalAppTskStack[0];
    params.stacksize = gstackSize;

    handle = TaskP_create((TaskP_Fxn)OsalApp_dummytaskFxn, &params);
    if((NULL == handle) || (UTRUE == TaskP_isTerminated(handle)))
    {
        result = osal_FAILURE;
    }

    TaskP_setPrio(handle, 3);

    if(UFALSE == TaskP_isTerminated(handle))
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("TaskP Termination test check has failed!!\n");
    }

    return result;
}

static int32_t OsalApp_taskNegativeTests(void)
{
    TaskP_Params    params;
    TaskP_Handle    handle;
    TaskP_Handle    junkHandle;
    int32_t         result = osal_OK;

#if defined(FREERTOS)
    TaskP_Params_init(NULL_PTR);
    uint32_t        key = 0U;

    key = TaskP_disable();
    
    TaskP_restore(key);
#endif
    
    TaskP_Params_init(&params);
    params.priority = OSAL_APP_PRIORITY_HIGHEST;
    memset(gOsalAppTskStack, 0, sizeof(gOsalAppTskStack));
    params.stack    = gOsalAppTskStack[0];
    params.stacksize = gstackSize;

    handle = TaskP_create((TaskP_Fxn)OsalApp_dummytaskFxn, &params);
    if(NULL_PTR == handle)
    {
        result = osal_FAILURE;
    }

#if defined(SAFERTOS)
    if(NULL_PTR == TaskP_getSafeRTOSHandle(handle))
    {
        result = osal_FAILURE;
    }
#endif

    memset(&junkHandle, 0U, sizeof(TaskP_Handle));

    if(TaskP_OK == TaskP_delete(&junkHandle))
    {
        result = osal_FAILURE;
    }

    if((TaskP_OK != TaskP_delete(&handle)) || (TaskP_OK == TaskP_delete(NULL_PTR)))
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("\n OsalApp_taskNegativeTests failed \n");
    }

    return result;
}

/* ========================================================================== */
/*                            Function Definitions                            */
/* ========================================================================== */

int32_t OsalApp_taskTests(void)
{
    int32_t result = osal_OK;

#if defined(BUILD_MCU)
    result += OsalApp_taskCreateMaxTaskTest();
#endif
    result += OsalApp_taskGeneralTests();
    result += OsalApp_taskIsTerminated();
    result += OsalApp_taskNegativeTests();

    if(osal_OK == result)
    {
        OSAL_log("\n All TaskP tests have passed!!\n");
    }
    else
    {
        OSAL_log("\n Some or all TaskP tests have failed!!\n");
    }

    return result;
}

