/*
 *  Copyright (C) 2018-2021 Texas Instruments Incorporated
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

#include <kernel/dpl/TaskP.h>
#include <kernel/dpl/ClockP.h>
#include <SafeRTOS_API.h>
#include <taskAPI.h>
#include <SafeRTOSConfigs.h>

#define TaskP_LOAD_CPU_LOAD_SCALE   (10000U)
#define TaskP_REGISTRY_MAX_ENTRIES  (16U)
#define TaskP_STACK_SIZE_MIN        (128U)

typedef struct TaskP_Struct_ {
    xTCB taskObj;
    portTaskHandleType taskHndl;
    uint32_t     lastRunTime;
    uint64_t     accRunTime;
} TaskP_Struct;

typedef struct {
    TaskP_Struct *taskRegistry[TaskP_REGISTRY_MAX_ENTRIES];
    uint32_t lastTotalTime;
    uint64_t accTotalTime;
    uint32_t idleTskLastRunTime;
    uint64_t idleTskAccRunTime;
} TaskP_Ctrl;

TaskP_Ctrl gTaskP_ctrl;

/* Declare the stack for the Idle task. */
portInt8Type acIdleTaskStack[ configIDLE_TASK_STACK_SIZE ] __attribute__( ( aligned( configIDLE_TASK_STACK_SIZE ) ) ) = { 0 };

/* Declare the stack for the timer task. */
portInt8Type acTimerTaskStack[ configTIMER_TASK_STACK_SIZE ] __attribute__( ( aligned( configTIMER_TASK_STACK_SIZE ) ) ) = { 0 };

/* The buffer for the timer command queue. */
portInt8Type acTimerCommandQueueBuffer[ configTIMER_CMD_QUEUE_BUFFER_SIZE ] __attribute__( ( aligned( safertosapiWORD_ALIGNMENT ) ) ) = { 0 };

/* The structure passed to xTaskInitializeScheduler() to configure the kernel
 * with the application defined constants and call back functions. 
 * This is defined as weak so that application can override this definition.
 */
__attribute__((weak)) const xPORT_INIT_PARAMETERS xPortInit =
{
    configSYSTICK_CLOCK_HZ,                 /* ulTimerClockHz */
    configTICK_RATE_HZ,                     /* ulTickRateHz */
#if defined (BUILD_C7X)
    0,            /* uxYieldInterruptNumber */
#endif

    /* Hook Functions */
    NULL,                                   /* pvSvcHookFunction */

    /* System Stack parameters */
    configSTACK_CHECK_MARGIN,               /* uxAdditionalStackCheckMarginBytes */

    /* Idle Task parameters */
    acIdleTaskStack,                        /* pcIdleTaskStackBuffer */
    configIDLE_TASK_STACK_SIZE,             /* uxIdleTaskStackSizeBytes */

#if defined (BUILD_MCU)
    pdTRUE,                                 /* xIdleTaskUsingFPU */
    /* xIdleTaskMPUParameters */
    {
        mpuUNPRIVILEGED_TASK,               /* The idle hook will be executed in unprivileged mode. */
        {
            { NULL, 0U, 0U, 0U },
            { NULL, 0U, 0U, 0U }
        }
    },
#endif

    NULL,                                   /* pvIdleTaskTLSObject */

    /* Timer feature initialisation parameters */
    configTIMER_TASK_PRIORITY,              /* uxTimerTaskPriority */
    configTIMER_TASK_STACK_SIZE,            /* uxTimerTaskStackSize */
    acTimerTaskStack,                       /* pcTimerTaskStackBuffer */
    configTIMER_QUEUE_LENGTH,               /* uxTimerCommandQueueLength */
    configTIMER_CMD_QUEUE_BUFFER_SIZE,      /* uxTimerCommandQueueBufferSize */
    acTimerCommandQueueBuffer,              /* pcTimerCommandQueueBuffer */
#if defined (BUILD_MCU)
    pdFALSE                                 /* xEnableCache */
#endif
};

static void TaskP_addToRegistry(TaskP_Struct *task)
{
    uint32_t i;
    portBaseType isStarted;

    isStarted = xTaskIsSchedulerStarted();
    if(isStarted == pdTRUE)
    {
        vTaskSuspendScheduler();
    }

    for(i=0; i<TaskP_REGISTRY_MAX_ENTRIES; i++)
    {
        if(gTaskP_ctrl.taskRegistry[i]==NULL)
        {
            gTaskP_ctrl.taskRegistry[i] = task;
            break;
        }
    }

    if(isStarted == pdTRUE)
    {
        xTaskResumeScheduler();
    }
}

static void TaskP_removeFromRegistry(TaskP_Struct *task)
{
    uint32_t i;
    portBaseType isStarted;

    isStarted = xTaskIsSchedulerStarted();
    if(isStarted == pdTRUE)
    {
        vTaskSuspendScheduler();
    }

    for(i=0; i<TaskP_REGISTRY_MAX_ENTRIES; i++)
    {
        if(gTaskP_ctrl.taskRegistry[i]==task)
        {
            gTaskP_ctrl.taskRegistry[i] = NULL;
            break;
        }
    }

    if(isStarted == pdTRUE)
    {
        xTaskResumeScheduler();
    }
}

/* Not implemented in SafeRTOS */
#if 0
static uint32_t TaskP_calcCounterDiff(uint32_t cur, uint32_t last)
{
    uint32_t delta;

    if(cur >= last)
    {
        delta = cur - last;
    }
    else
    {
        delta = ( 0xFFFFFFFF - last ) + cur;
    }
    return delta;
}
#endif

void TaskP_Params_init(TaskP_Params *params)
{
    params->name = "Task (DPL)";
    params->stackSize = 0;
    params->stack = NULL;
    params->priority = (TaskP_PRIORITY_HIGHEST - TaskP_PRIORITY_LOWEST) / 2;
    params->args = NULL;
    params->taskMain = NULL;
}

int32_t TaskP_construct(TaskP_Object *obj, TaskP_Params *params)
{
    int32_t status = SystemP_SUCCESS;
    TaskP_Struct *taskObj = (TaskP_Struct *)obj;
    portBaseType xCreateResult;

    DebugP_assert(sizeof(TaskP_Struct) <= sizeof(TaskP_Object));
    DebugP_assert(params != NULL);
    DebugP_assert(taskObj != NULL);
    DebugP_assert(params->stackSize >= TaskP_STACK_SIZE_MIN);
    DebugP_assert( (params->stackSize & (sizeof(portStackType) - 1)) == 0);
    DebugP_assert(params->stack != NULL );
    DebugP_assert( ((uintptr_t)params->stack & (sizeof(portStackType) - 1)) == 0);
    DebugP_assert(params->taskMain != NULL );

    /* if prority is out of range, adjust to bring it in range */
    if(params->priority > TaskP_PRIORITY_HIGHEST)
    {
        params->priority = TaskP_PRIORITY_HIGHEST;
    }
    if(params->priority <= TaskP_PRIORITY_LOWEST)
    {
        params->priority = TaskP_PRIORITY_LOWEST;
    }

    taskObj->lastRunTime = 0;
    taskObj->accRunTime = 0;

    TaskP_addToRegistry(taskObj);

    xTaskParameters xTaskPParams = 
    {
        params->taskMain,
        params->name,
        &taskObj->taskObj,
        (portInt8Type *) params->stack,
        params->stackSize,
        params->args,
        params->priority,
        NULL,
#if defined (BUILD_MAIN_R5) || defined (BUILD_MCU_R5) || defined (BUILD_WKUP_R5)
        pdFALSE,                            /* Task does not use the FPU by default. */
        {                                   /* MPU task parameters. */
            params->taskPrivilege,          /* Check task is privileged. */
            {
                { NULL, 0U, 0U, 0U },       /* No additional region definitions are required. */
                { NULL, 0U, 0U, 0U },
            }
        }
#endif
    };

    xTaskPParams.pvTaskCode         = params->taskMain;
    xTaskPParams.pcTaskName         = params->name;
    xTaskPParams.pxTCB              = &taskObj->taskObj;
    xTaskPParams.pcStackBuffer      = (portInt8Type *) params->stack;
    xTaskPParams.uxStackDepthBytes  = params->stackSize;
    xTaskPParams.pvParameters       = params->args;
    xTaskPParams.uxPriority         = params->priority;
    xTaskPParams.pvObject           = NULL;  /* Thread Local Storage not used. */

    xCreateResult = xTaskCreate(&xTaskPParams, &taskObj->taskHndl);
    if((xCreateResult != pdPASS) || (taskObj->taskHndl == NULL))
    {
        status = SystemP_FAILURE;
    }
    return status;
}

void TaskP_destruct(TaskP_Object *obj)
{
    TaskP_Struct *taskObj = (TaskP_Struct *)obj;

    if(taskObj && taskObj->taskHndl)
    {
        xTaskDelete(taskObj->taskHndl);
        taskObj->taskHndl = NULL;
    }
    if(taskObj)
    {
        TaskP_removeFromRegistry(taskObj);
    }
}

void* TaskP_getHndl(TaskP_Object *obj)
{
    TaskP_Struct *taskObj = (TaskP_Struct *)obj;

    return (void*)taskObj->taskHndl;
}

void TaskP_yield()
{
    safertosapiYIELD();
}

void TaskP_exit()
{
    xTaskDelete(NULL);
}

/* This is required for runtime load calculation, which SafeRTOS does not support. */
void TaskP_loadGet(TaskP_Object *obj, TaskP_Load *taskLoad)
{
#if 0
    TaskP_Struct *taskObj = (TaskP_Struct *)obj;
    /* Not implemented in SafeRTOS */
    //TaskStatus_t taskStatus;

    vTaskSuspendScheduler();

    /* Not implemented in SafeRTOS */
    //vTaskGetInfo(taskObj->taskHndl, &taskStatus, pdFALSE, eReady);

    taskLoad->runTime = taskObj->accRunTime;
    taskLoad->totalTime  = gTaskP_ctrl.accTotalTime;
    taskLoad->cpuLoad = TaskP_calcCpuLoad(taskObj->accRunTime, gTaskP_ctrl.accTotalTime);
    /* Not implemented in SafeRTOS */
    //taskLoad->name = taskStatus.pcTaskName;
    taskLoad->name = "Unknown";

    xTaskResumeScheduler();
#endif
}

/* This is required for runtime load calculation, which SafeRTOS does not support. */
uint32_t TaskP_loadGetTotalCpuLoad()
{
    uint32_t cpuLoad = 0;
#if 0
    /* This is need to get upto date IDLE task statistics, since the next update window could be some time away */
    TaskP_loadUpdateAll();

    vTaskSuspendScheduler();

    cpuLoad = TaskP_LOAD_CPU_LOAD_SCALE - TaskP_calcCpuLoad(gTaskP_ctrl.idleTskAccRunTime, gTaskP_ctrl.accTotalTime);

    xTaskResumeScheduler();
#endif

    return cpuLoad;
}

/* This is required for runtime load calculation, which SafeRTOS does not support. */
void TaskP_loadResetAll()
{
#if 0
    TaskP_Struct *taskObj;
    uint32_t i;

    vTaskSuspendScheduler();

    for(i=0; i<TaskP_REGISTRY_MAX_ENTRIES; i++)
    {
        if(gTaskP_ctrl.taskRegistry[i]!=NULL)
        {
            taskObj = gTaskP_ctrl.taskRegistry[i];

            taskObj->accRunTime = 0;
        }
    }

    gTaskP_ctrl.idleTskAccRunTime = 0;
    gTaskP_ctrl.accTotalTime = 0;

    xTaskResumeScheduler();
#endif
}

/* This is required for runtime load calculation, which SafeRTOS does not support. */
void TaskP_loadUpdateAll()
{
    /* Not implemented in SafeRTOS */
#if 0
    TaskP_Struct *taskObj;
    TaskStatus_t taskStatus;
    uint32_t i, delta, curTotalTime;
    TaskHandle_t idleTskHndl;

    vTaskSuspendScheduler();

    for(i=0; i<TaskP_REGISTRY_MAX_ENTRIES; i++)
    {
        if(gTaskP_ctrl.taskRegistry[i]!=NULL)
        {
            taskObj = gTaskP_ctrl.taskRegistry[i];

            vTaskGetInfo(taskObj->taskHndl, &taskStatus, pdFALSE, eReady);

            delta = TaskP_calcCounterDiff(taskStatus.ulRunTimeCounter, taskObj->lastRunTime);

            taskObj->accRunTime += delta;
            taskObj->lastRunTime = taskStatus.ulRunTimeCounter;
        }
    }

    idleTskHndl = xTaskGetIdleTaskHandle();
    if(idleTskHndl != NULL)
    {
        vTaskGetInfo(idleTskHndl, &taskStatus, pdFALSE, eReady);

        delta = TaskP_calcCounterDiff(taskStatus.ulRunTimeCounter, gTaskP_ctrl.idleTskLastRunTime);

        gTaskP_ctrl.idleTskAccRunTime += delta;
        gTaskP_ctrl.idleTskLastRunTime = taskStatus.ulRunTimeCounter;
    }

    curTotalTime = portGET_RUN_TIME_COUNTER_VALUE();

    delta = TaskP_calcCounterDiff(curTotalTime, gTaskP_ctrl.lastTotalTime);

    gTaskP_ctrl.accTotalTime += delta;
    gTaskP_ctrl.lastTotalTime = curTotalTime;

    xTaskResumeScheduler();
#endif
}

/* This is required for runtime load calculation, which SafeRTOS does not support. */
void vApplicationLoadHook()
{
#if 0    
    static uint64_t lastUpdateTime = 0;
    uint64_t curUpdateTime = ClockP_getTimeUsec();

    if( (curUpdateTime > lastUpdateTime) && (curUpdateTime - lastUpdateTime) > TaskP_LOAD_UPDATE_WINDOW_MSEC*1000u )
    {
        TaskP_loadUpdateAll();
        lastUpdateTime = curUpdateTime;
    }
#endif
}

void vApplicationErrorHook( portTaskHandleType xHandleOfTaskWithError,
                            portBaseType xErrorCode )
{
    /* The parameters are not used, these lines prevent compiler warnings. */
    ( void ) xHandleOfTaskWithError;
    ( void ) xErrorCode;

    /* Will only get here if an internal kernel error occurs. */
    DebugP_assert(pdFALSE);
}
