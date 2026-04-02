/*
 * Copyright (c) 2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== LoadP_safertos.c ========
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/osal/TaskP.h>
#include <ti/osal/LoadP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/soc/osal_soc.h>

#include "SafeRTOS_priv.h"
#include "SafeRTOS_config.h"


#if defined (configINCLUDE_RUNTIMESTATS) && ( configINCLUDE_RUNTIMESTATS == 1 )

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct LoadP_taskLoadObj_s
{
    bool                used;
    TaskP_Handle        pTsk;
    portUInt32Type            threadTime;
    portUInt32Type            lastUpdate_threadTime;
} LoadP_taskLoadObj;

typedef struct LoadP_safertos_s
{   
    LoadP_taskLoadObj taskLoadObj[OSAL_SAFERTOS_CONFIGNUM_TASK];
    portUInt32Type          idlTskTime;
    portUInt32Type          lastUpdate_idlTskTime;
} LoadP_safertos;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void LoadP_addTask(TaskP_Handle handle, uint32_t tskId);
void LoadP_removeTask(uint32_t tskId);
extern portTaskHandleType TaskP_getSafeRTOSHandle(TaskP_Handle handle);
extern uint32_t TaskP_getTaskId(TaskP_Handle handle);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static LoadP_safertos   gLoadP_safertos;
static bool             gLoadP_initDone = BFALSE;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void LoadP_reset(void)
{
    uint32_t            i;
    LoadP_taskLoadObj   *pHndl;

    vTaskSuspendScheduler();

    gLoadP_safertos.idlTskTime = 0U;

    for (i = 0U; i < OSAL_FREERTOS_CONFIGNUM_TASK; i++)
    {
        pHndl = &gLoadP_safertos.taskLoadObj[i];

        if (BTRUE == pHndl->used )
        {
            pHndl->threadTime = 0U;
        }
    }
    (void)xTaskResumeScheduler();

    return;
}

/* ========================================================================================================================== */

LoadP_Status LoadP_getTaskLoad(TaskP_Handle taskHandle, LoadP_Stats *stats)
{
    LoadP_taskLoadObj   *pHndl;
    LoadP_Status        ret_val = LoadP_FAILURE;
    uint32_t            tskId;

    if((NULL_PTR != stats) && (NULL_PTR != taskHandle))
    {
        vTaskSuspendScheduler();

        LoadP_update();

        tskId = TaskP_getTaskId(taskHandle);

        pHndl = &gLoadP_safertos.taskLoadObj[tskId];

        if ((BTRUE == pHndl->used) && (pHndl->pTsk == taskHandle))
        {
            stats->percentLoad = (uint32_t)pHndl->threadTime;
            ret_val = LoadP_OK;
        }

        (void)xTaskResumeScheduler();
    }

    return ret_val;
}

/* ========================================================================================================================== */

uint32_t LoadP_getCPULoad(void)
{
    uint32_t cpuLoad;

    LoadP_update();

    vTaskSuspendScheduler();

    /* SafeRTOS RTS returns percentages as fixed point values of 2 decimal. Ex: 1 corresponds to 0.01% */
    cpuLoad = (uint32_t)(10000U - gLoadP_safertos.idlTskTime);

    (void)xTaskResumeScheduler();

    return cpuLoad;
}

/* ========================================================================================================================== */

void LoadP_update(void)
{
    uint32_t            i;
    LoadP_taskLoadObj   *pHndl;
    xPERCENTAGES xIdleTaskPercentages = { { 0U, 0U }, { 0U, 0U } };
    xPERCENTAGES xTaskPercentages = { { 0U, 0U }, { 0U, 0U } };
    portBaseType xStatus;

    vTaskSuspendScheduler();

    if(!gLoadP_initDone)
    {
        (void)memset( (void *)&gLoadP_safertos,0,sizeof(gLoadP_safertos));
        gLoadP_initDone = BTRUE;
    }

    /* SafeRTOS RTS returns percentages as fixed point values of 2 decimal. Ex: 1 corresponds to 0.01% */
    /* Idle Task Update */
    xStatus = xCalculateCPUUsage( xGetIdleTaskHandle(), &xIdleTaskPercentages );
    if(pdPASS == xStatus)
    {
    gLoadP_safertos.idlTskTime = xIdleTaskPercentages.xOverall.ulCurrent;
    gLoadP_safertos.lastUpdate_idlTskTime = xIdleTaskPercentages.xPeriod.ulCurrent;
    }

    /* All tasks Update */
    for (i = 0U; i < OSAL_FREERTOS_CONFIGNUM_TASK; i++)
    {
        pHndl = &gLoadP_safertos.taskLoadObj[i];

        if ((BTRUE == pHndl->used) && (NULL_PTR != pHndl->pTsk))
        {
            xStatus = xCalculateCPUUsage( TaskP_getSafeRTOSHandle(pHndl->pTsk), &xTaskPercentages );
            if(pdPASS == xStatus)
            {
            pHndl->threadTime = xTaskPercentages.xOverall.ulCurrent;
            pHndl->lastUpdate_threadTime = xTaskPercentages.xPeriod.ulCurrent;
            }
        }
    }

    (void)xTaskResumeScheduler();

    return;
}

/* ========================================================================================================================== */

void LoadP_addTask(TaskP_Handle handle, uint32_t tskId)
{
    LoadP_taskLoadObj   *pHndl = &gLoadP_safertos.taskLoadObj[tskId];

    vTaskSuspendScheduler();
    if(!gLoadP_initDone)
    {
        (void)memset( (void *)&gLoadP_safertos,0,sizeof(gLoadP_safertos));
        gLoadP_initDone = BTRUE;
    }

    pHndl->used                  = BTRUE;
    pHndl->pTsk                  = handle;
    pHndl->threadTime            = 0U;
    pHndl->lastUpdate_threadTime = 0U;

    (void)xTaskResumeScheduler();

}

/* ========================================================================================================================== */

void LoadP_removeTask(uint32_t tskId)
{
    LoadP_taskLoadObj   *pHndl = &gLoadP_safertos.taskLoadObj[tskId];

    pHndl->used = BFALSE; 
}

/* ========================================================================================================================== */

#endif
