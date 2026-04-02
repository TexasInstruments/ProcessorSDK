/*
 *  Copyright (C) 2020-2024 Texas Instruments Incorporated
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
 *
 */

/**
 *  \file sciserver_tirtos.c
 *
 *  \brief Example Library functions for an application including Sciserver
 *         This file is only for RTOS based applications.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <lib/strncpy.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/RegisterIntr.h>
#include <ti/drv/sciclient/sciserver.h>
#include <ti/drv/sciclient/sciserver_tirtos.h>
#include <sciserver_hwiData.h>
#include "sciserver_secproxyTransfer.h"
#include <ti/drv/sciclient/src/sciclient/sciclient_trace_internal.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Macro to determine the size of an array */
#define SCISERVER_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

HwiP_Handle gSciserverHwiHandles[SCISERVER_HWI_NUM];
SemaphoreP_Params gSciserverUserSemParams[SCISERVER_SEMAPHORE_MAX_CNT];
SemaphoreP_Handle gSciserverUserSemHandles[SCISERVER_SEMAPHORE_MAX_CNT];
SemaphoreP_Params gSciserverSyncParam;
SemaphoreP_Handle gSciserverSyncHandle;
TaskP_Handle gSciserverUserTaskHandles[SCISERVER_TASK_MAX_CNT];
TaskP_Params gSciserverUserTaskParams[SCISERVER_TASK_MAX_CNT];
uint8_t hwiMasked = 1U;
#if defined (ENABLE_DM_TRACE)
extern uint32_t gStoreLog;
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Function Declarations                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Internal Function Declarations                      */
/* ========================================================================== */

static int32_t Sciserver_tirtosInitHwis(void);
static int32_t Sciserver_tirtosInitSemaphores(void);
static int32_t Sciserver_tirtosInitUserTasks(Sciserver_TirtosCfgPrms_t *pPrms);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Sciserver_tirtosInit(Sciserver_TirtosCfgPrms_t *pAppPrms)
{
    Sciserver_CfgPrms_t prms;
    int32_t ret = CSL_PASS;

    if (pAppPrms == NULL)
    {
        ret = CSL_EBADARGS;
    }

    /* Initialize the Init Parameters for the Sciserver */
    if (ret == CSL_PASS)
    {
        ret = Sciserver_initPrms_Init(&prms);
    }
    /* Initialize the Sciserver */
    if (ret == CSL_PASS)
    {
        ret = Sciserver_init(&prms);
    }

    /* Create a semaphore for the Sciserver Task */
    if (ret == CSL_PASS)
    {
        ret = Sciserver_tirtosInitSemaphores();
    }

    /* user task initialization */
    if (ret == CSL_PASS) {
#if defined (ENABLE_DM_TRACE)
        /* Stores logs in trace_dm buffer from this point */
        gStoreLog = 1;
#endif
        ret = Sciserver_tirtosInitUserTasks(pAppPrms);
    }

    /* hwi initialization */
    if (ret == CSL_PASS) {
        ret = Sciserver_tirtosInitHwis();
    }

    /* Set the process State */
    if (ret == CSL_PASS)
    {
        Sciserver_setCtrlState(SCISERVER_PROCESS_STATE_RUN);
    }
    return ret;
}

void Sciserver_tirtosUserMsgHwiFxn(uintptr_t arg)
{
    Sciserver_hwiData *uhd = (Sciserver_hwiData *) arg;
    int32_t ret = CSL_PASS;
    bool soft_error = false;

    Osal_DisableInterrupt(0, uhd->irq_num);

    ret = Sciserver_interruptHandler(uhd, &soft_error);

    if ((ret != CSL_PASS) && (soft_error == true))
    {
        Osal_EnableInterrupt(0, uhd->irq_num);
    }
    else
    {
        (void) SemaphoreP_post(gSciserverUserSemHandles[uhd->semaphore_id]);
    }

    Osal_ClearInterrupt(0, uhd->irq_num);

    if ((ret != CSL_PASS) && (soft_error != true)) {
        /* At this point secure proxy is broken so halt */
        OS_stop();
    }
}

void Sciserver_tirtosUnRegisterIntr(void)
{
    uint32_t i = 0U;

    for (i = 0U; i < SCISERVER_ARRAY_SIZE(sciserver_hwi_list); i++) {
        if (gSciserverHwiHandles[i] != NULL)
        {
            Osal_DeleteInterrupt(gSciserverHwiHandles[i],
                                 sciserver_hwi_list[i].irq_num);
            gSciserverHwiHandles[i] = NULL_PTR;
        }
    }
}

void Sciserver_tirtosDeinit(void)
{
    uint32_t i = 0U;

    for (i = 0U; i < (uint32_t)SCISERVER_SEMAPHORE_MAX_CNT; i++) {
        SemaphoreP_delete(gSciserverUserSemHandles[i]);
    }
    Sciserver_deinit();
    Sciserver_tirtosUnRegisterIntr();
}

void Sciserver_tirtosUserMsgTask(void *arg0, void* arg1)
{
    Sciserver_printf("Entering Sciserver_tirtosUserMsgTask function\n");
    uintptr_t key;
    uint32_t i = 0U;
    int32_t ret;
    volatile uint32_t loopForever = 1U;
    Sciserver_taskData *utd = (Sciserver_taskData *) arg0;

    /* To suppress unused variable warning */
    (void)arg1;

    /* Set the pending State first */
    utd->state->state = SCISERVER_TASK_PENDING;

    /* Enter critical section */
    key = HwiP_disable();

    /*
     * Here we check if all Hwi are still masked. We do this in order to control
     * when we enable the interrupts for the secure proxy messages in the case
     * there are pending messages queued prior to the task starting up. This
     * only should be done once, so we protect access with the global state.
     */
    if (hwiMasked == 1U)
    {
        hwiMasked = 0;
        for (i = 0U; i < SCISERVER_ARRAY_SIZE(sciserver_hwi_list); i++) {
            Osal_EnableInterrupt(0,sciserver_hwi_list[i].irq_num);
            Sciserver_printf("Sciserver_tirtosUserMsgTask: Enabled interrupt - %d\n", sciserver_hwi_list[i].irq_num);
        }
    }

    /* Leave critical section */
    HwiP_restore(key);

    while(loopForever != 0U)
    {
        SemaphoreP_pend(gSciserverUserSemHandles[utd->semaphore_id],
                        SemaphoreP_WAIT_FOREVER);

        SemaphoreP_pend(gSciserverSyncHandle, SCICLIENT_SERVICE_WAIT_FOREVER);

        ret = Sciserver_processtask (utd);

        SemaphoreP_post(gSciserverSyncHandle);

        if (ret != CSL_PASS)
        {
            Sciserver_printf("ERROR:: Sciserver_tirtosUserMsgTask: Error processing task\n");
            /* Failed to process message and failed to send nak response */
            OS_stop();
            Sciserver_printf("ERROR:: Sciserver_tirtosUserMsgTask: Stopping OS due to error\n");
        }
        else
        {
            Sciserver_printf("Sciserver_tirtosUserMsgTask: processing task Successful\n");
            /*
             * This is a bit of a hack... using the task ID to pick the offset
             * for the gloabl interrupt data array. This is functional but can
             * be cleaned up.
             */
            Osal_EnableInterrupt(0U, sciserver_hwi_list[(2 * utd->task_id) +
                    ((int32_t) utd->state->current_buffer_idx)].irq_num);
        }
    }
    Sciserver_printf("Exiting Sciserver_tirtosUserMsgTask function\n");
}

/* ========================================================================== */
/*                 Internal Function Definitions                              */
/* ========================================================================== */

static int32_t Sciserver_tirtosInitSemaphores(void)
{
    int32_t ret = CSL_PASS;
    uint32_t i = 0U;

    for (i = 0U; i < (uint32_t)SCISERVER_SEMAPHORE_MAX_CNT; i++) {
        SemaphoreP_Params_init(&gSciserverUserSemParams[i]);
        gSciserverUserSemHandles[i] = SemaphoreP_create(0U, &gSciserverUserSemParams[i]);

        if (gSciserverUserSemHandles[i] == NULL) {
            ret = CSL_EFAIL;
            break;
        }
    }

    if (ret == CSL_PASS) {
        SemaphoreP_Params_init(&gSciserverSyncParam);
        gSciserverSyncHandle = SemaphoreP_create(1U, &gSciserverSyncParam);

        if (gSciserverSyncHandle == NULL) {
            ret = CSL_EFAIL;
        }
    }

    return ret;
}

static int32_t Sciserver_tirtosInitHwis(void)
{
    uint32_t i = 0U;
    int32_t ret = CSL_PASS;

    for (i = 0U; i < SCISERVER_ARRAY_SIZE(sciserver_hwi_list); i++) {
        OsalRegisterIntrParams_t    intrPrms;
        Osal_RegisterInterrupt_initParams(&intrPrms);
        intrPrms.corepacConfig.arg  = (uintptr_t) &sciserver_hwi_list[i];
        intrPrms.corepacConfig.isrRoutine = &Sciserver_tirtosUserMsgHwiFxn;
        intrPrms.corepacConfig.enableIntr = FALSE;
        intrPrms.corepacConfig.corepacEventNum  = 0;
        intrPrms.corepacConfig.intVecNum =
            sciserver_hwi_list[i].irq_num;
        /* Register interrupts */
        ret = Osal_RegisterInterrupt(&intrPrms,
                                     &gSciserverHwiHandles[i]);
        if(OSAL_INT_SUCCESS != ret) {
            gSciserverHwiHandles[i] = NULL_PTR;
            break;
        }
    }

    return ret;
}

static int32_t Sciserver_tirtosInitUserTasks(Sciserver_TirtosCfgPrms_t *pPrms)
{
    uint32_t i = 0U;
    int32_t ret = CSL_PASS;

    for (i = 0U; i < SCISERVER_ARRAY_SIZE(gSciserverTaskList); i++)
    {
        TaskP_Params_init (&gSciserverUserTaskParams[i]);
        gSciserverUserTaskParams[i].priority = (int8_t)pPrms->taskPriority[i];
        gSciserverUserTaskParams[i].stack = gSciserverTaskList[i].stack;
        gSciserverUserTaskParams[i].stacksize = SCISERVER_TASK_STACK_SIZE;
        gSciserverUserTaskParams[i].arg0 = (void *) &gSciserverTaskList[i];
        gSciserverUserTaskHandles[i] =
            TaskP_create(&Sciserver_tirtosUserMsgTask, &gSciserverUserTaskParams[i]);
        if(NULL == gSciserverUserTaskHandles[i])
        {
            ret = CSL_EFAIL;
            break;
        }
    }

    return ret;
}

