/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
 *  ======== ClockP_freertos.c ========
 */

#include <ti/osal/ClockP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/soc/osal_soc.h>

#include <FreeRTOS.h>
#include <timers.h>

typedef struct ClockP_freertos_s
{
    bool                used;
    StaticTimer_t       timerObj;
    TimerHandle_t       timerHndl;
    ClockP_FxnCallback  callback;
    void                *arg;
} ClockP_freertos;

/* global pool of statically allocated clock pools */
static ClockP_freertos gOsalClockPFreeRtosPool[OSAL_FREERTOS_CONFIGNUM_CLOCK];

static void ClockP_timerCallbackFunction(TimerHandle_t xTimer);

uint32_t gOsalClockAllocCnt = 0U, gOsalClockPeak = 0U;

static void ClockP_timerCallbackFunction(TimerHandle_t xTimer)
{
    ClockP_freertos *pTimer = (ClockP_freertos *)pvTimerGetTimerID(xTimer);

    if( (NULL_PTR != pTimer) && (pTimer->callback) )
    {
        pTimer->callback(pTimer->arg);
    }
}

void ClockP_Params_init(ClockP_Params *params)
{
    if(NULL_PTR != params)
    {
        params->pErrBlk     = NULL_PTR;
        params->startMode   = ClockP_StartMode_USER;
        params->period      = 0U;
        params->runMode     = ClockP_RunMode_ONESHOT;
        params->arg         = NULL_PTR;
    }

    return;
}

/*
 *  ======== ClockP_create ========
 */
ClockP_Handle ClockP_create(ClockP_FxnCallback clockfxn,
                            const ClockP_Params *params)
{

    ClockP_freertos *pTimer = (ClockP_freertos *)NULL_PTR;
    ClockP_freertos *timerPool;
    BaseType_t       uxAutoReload = pdFALSE;
    ClockP_Handle    ret_handle = NULL_PTR;
    uint32_t         i;
    uintptr_t        key;
    uint32_t         maxClocks;

    /* Pick up the internal static memory block */
    timerPool = (ClockP_freertos *) &gOsalClockPFreeRtosPool[0];
    maxClocks  = OSAL_FREERTOS_CONFIGNUM_CLOCK;

    if(0U == gOsalClockAllocCnt)
    {
        (void)memset((void *)gOsalClockPFreeRtosPool,0,sizeof(gOsalClockPFreeRtosPool));
    }

    key = HwiP_disable();

    for (i = 0U; i < maxClocks; i++)
    {
        if (BFALSE == timerPool[i].used)
        {
            timerPool[i].used = BTRUE;
            /* Update statistics */
            gOsalClockAllocCnt++;
            if (gOsalClockAllocCnt > gOsalClockPeak)
            {
                gOsalClockPeak = gOsalClockAllocCnt;
            }
            break;
        }
    }
    HwiP_restore(key);

    if (i < maxClocks)
    {
        /* Grab the memory */
        pTimer = (ClockP_freertos *) &timerPool[i];
    }

    if((NULL_PTR == pTimer) || (NULL_PTR == params))
    {
        ret_handle = NULL_PTR;
    }
    else
    {
        if(ClockP_RunMode_CONTINUOUS == params->runMode)
        {
            uxAutoReload = pdTRUE;
        }

        pTimer->callback = (ClockP_FxnCallback)clockfxn;
        pTimer->arg = params->arg;

        pTimer->timerHndl = xTimerCreateStatic(
                                (const char *)NULL_PTR,
                                params->period,
                                uxAutoReload,
                                pTimer,
                                ClockP_timerCallbackFunction,
                                &pTimer->timerObj
                                );
        if(NULL_PTR == pTimer->timerHndl)
        {
            /* If there was an error reset the clock object and return NULL. */
            key = HwiP_disable();
            pTimer->used      = BFALSE;
            /* Found the osal clock object to delete */
            gOsalClockAllocCnt--;
            HwiP_restore(key);
            ret_handle = NULL_PTR;
        }
        else
        {
            if(ClockP_StartMode_AUTO == params->startMode)
            {
                (void)xTimerStart(pTimer->timerHndl, (portMAX_DELAY));
            }
            ret_handle = (ClockP_Handle)pTimer;
        }
    }
    return ret_handle;
}

/*
 *  ======== ClockP_delete ========
 */
ClockP_Status ClockP_delete(ClockP_Handle handle)
{
    ClockP_freertos *pTimer = (ClockP_freertos*)handle;
    uintptr_t       key;
    ClockP_Status   ret = ClockP_OK;

    if ((NULL_PTR != pTimer) && (BTRUE == pTimer->used))
    {
        (void)xTimerDelete(pTimer->timerHndl, (portMAX_DELAY));

        key = HwiP_disable();
        pTimer->used      = BFALSE;
        pTimer->timerHndl = NULL;
        pTimer->callback  = NULL;
        pTimer->arg       = NULL;
        if (0U < gOsalClockAllocCnt)
        {
            gOsalClockAllocCnt--;
        }
        HwiP_restore(key);
        ret = ClockP_OK;
    }
    else
    {
        ret = ClockP_FAILURE;
    }

    return ret;
}

ClockP_Status ClockP_start(ClockP_Handle handle)
{
    ClockP_Status ret       = ClockP_OK;
    ClockP_freertos *pTimer = (ClockP_freertos*)handle;
    BaseType_t  status;

    if ((NULL_PTR != pTimer) && (BTRUE == pTimer->used))
    {
        if( 1 == xPortInIsrContext())
        {
            BaseType_t xHigherPriorityTaskWoken = 0;

            /* timeout is ignored when in ISR mode */
            status = xTimerStartFromISR(pTimer->timerHndl, (&xHigherPriorityTaskWoken));
            portYIELD_FROM_ISR((uint32_t)xHigherPriorityTaskWoken);
        }
        else
        {
            status = xTimerStart(pTimer->timerHndl, (portMAX_DELAY));
        }
        if(pdPASS == status)
        {
            ret = ClockP_OK;
        }
        else
        {
            ret = ClockP_FAILURE;
        }

    }
    else
    {
        ret = ClockP_FAILURE;
    }

    return ret;
}

ClockP_Status ClockP_stop(ClockP_Handle handle)
{
    ClockP_Status ret       = ClockP_OK;
    ClockP_freertos *pTimer = (ClockP_freertos*)handle;
    BaseType_t  status;

    if ((NULL_PTR != pTimer) && (BTRUE == pTimer->used))
    {
        if( 1 == xPortInIsrContext() )
        {
            BaseType_t xHigherPriorityTaskWoken = 0;

            /* timeout is ignored when in ISR mode */
            status = xTimerStopFromISR(pTimer->timerHndl, (&xHigherPriorityTaskWoken));
            portYIELD_FROM_ISR((uint32_t)xHigherPriorityTaskWoken);
        }
        else
        {
            status = xTimerStop(pTimer->timerHndl, (portMAX_DELAY));
        }
        if(pdPASS == status)
        {
            ret = ClockP_OK;
        }
        else
        {
            ret = ClockP_FAILURE;
        }
    }
    else
    {
        ret = ClockP_FAILURE;
    }

    return ret;
}

/* Nothing past this point */
