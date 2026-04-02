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
 *  ======== ClockP_safertos.c ========
 */

#include "SafeRTOS_priv.h"

#include <ti/osal/ClockP.h>
#include <timers.h>

typedef struct ClockP_safertos_s
{
    bool                        used;
    timerControlBlockType       timerObj;
    timerHandleType             timerHndl;
    ClockP_FxnCallback          callback;
    void                        *arg;
} ClockP_safertos;

/* global pool of statically allocated clock pools */
static ClockP_safertos gOsalClockPSafeRtosPool[OSAL_SAFERTOS_CONFIGNUM_CLOCK];

uint32_t gOsalClockAllocCnt = 0U, gOsalClockPeak = 0U;

static void ClockP_timerCallbackFunction(timerHandleType xTimer);

static void ClockP_timerCallbackFunction(timerHandleType xTimer)
{
    /*
     * TODO check what is the parameter the call back function is passing.
     * For now assuming it is passing the timer ID, which is index as
     * initialized at the time of creation.
     */
    portBaseType    xTimerID = ((timerControlBlockType *)xTimer)->xTimerID;
    ClockP_safertos *pTimer = &gOsalClockPSafeRtosPool[xTimerID];

    if((NULL_PTR != pTimer) && (pTimer->callback) && (BTRUE == pTimer->used))
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

    ClockP_safertos         *pTimer = (ClockP_safertos *)NULL_PTR;
    ClockP_safertos         *timerPool;
    portBaseType            uxAutoReload = pdFALSE;
    ClockP_Handle           ret_handle = NULL_PTR;
    uint32_t                i;
    uintptr_t               key;
    uint32_t                maxClocks;
    timerInitParametersType xTimerInitParameters;
    portBaseType            xCreateResult;

    /* Pick up the internal static memory block */
    timerPool = (ClockP_safertos *) &gOsalClockPSafeRtosPool[0];
    maxClocks  = OSAL_SAFERTOS_CONFIGNUM_CLOCK;

    if(0U == gOsalClockAllocCnt) 
    {
        (void)memset((void *)gOsalClockPSafeRtosPool,0,sizeof(gOsalClockPSafeRtosPool));
    }

    key = HwiP_disable();

    for(i = 0U; i < maxClocks; i++)
    {
        if(BFALSE == timerPool[i].used)
        {
            timerPool[i].used = BTRUE;
            /* Update statistics */
            gOsalClockAllocCnt++;
            if(gOsalClockAllocCnt > gOsalClockPeak)
            {
                gOsalClockPeak = gOsalClockAllocCnt;
            }
            break;
        }
    }
    HwiP_restore(key);

    if(i < maxClocks)
    {
        /* Grab the memory */
        pTimer = (ClockP_safertos *) &timerPool[i];
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

        /* Update the xTimerInitParameters structure */

        /* pcTimerName */
        xTimerInitParameters.pcTimerName        = (const char *)NULL_PTR;
        /* xTimerPeriodInTicks */
        xTimerInitParameters.xTimerPeriodInTicks= params->period;
        /* xIsPeriodic */
        xTimerInitParameters.xIsPeriodic        = uxAutoReload;
        /* xTimerID is the index */
        xTimerInitParameters.xTimerID           = (portBaseType)i;
        /* pxNewTimer */
        xTimerInitParameters.pxNewTimer         = &pTimer->timerObj;
        /* pxCallbackFunction */
        xTimerInitParameters.pxCallbackFunction = ClockP_timerCallbackFunction;
        /* xTaskToNotify */
        xTimerInitParameters.xTaskToNotify      = NULL;
        /* pxTimerInstance */
        xTimerInitParameters.pxTimerInstance    = NULL;
        /* pvObjec */
        xTimerInitParameters.pvObject           = NULL;

        xCreateResult = xTimerCreate(&xTimerInitParameters, &pTimer->timerHndl);

        if(pdPASS != xCreateResult)
        {
            /* If there was an error reset the clock object and return NULL. */
            key = HwiP_disable();
            pTimer->used      = BFALSE;
            /* Found the osal clock object to delete */
            if (0U < gOsalClockAllocCnt)
            {
                gOsalClockAllocCnt--;
            }
            HwiP_restore(key);
            ret_handle = NULL_PTR;
        }
        else
        {
            if(ClockP_StartMode_AUTO == params->startMode)
            {
                xCreateResult = xTimerStart(pTimer->timerHndl, safertosapiMAX_DELAY);
                if(pdPASS != xCreateResult)
                {
                    /* If there was an error reset the clock object and return NULL. */
                    key = HwiP_disable();
                    pTimer->used      = BFALSE;
                    /* Found the osal clock object to delete */
                    gOsalClockAllocCnt--;
                    HwiP_restore(key);
                    ret_handle = NULL_PTR;
                }
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
    ClockP_safertos *pTimer = (ClockP_safertos*)handle;
    uintptr_t       key;
    ClockP_Status   ret = ClockP_OK;
    portBaseType    xCreateResult;

    if((NULL_PTR != pTimer) && (BTRUE == pTimer->used))
    {
        xCreateResult = xTimerDelete(pTimer->timerHndl, safertosapiMAX_DELAY);

        if(pdPASS == xCreateResult)
        {
            key = HwiP_disable();
            pTimer->used      = BFALSE;
            pTimer->timerHndl = NULL;
            pTimer->callback  = NULL;
            pTimer->arg       = NULL;
            if(0U < gOsalClockAllocCnt)
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
    }
    else
    {
        ret = ClockP_FAILURE;
    }

    return ret;
}

ClockP_Status ClockP_start(ClockP_Handle handle)
{
    ClockP_Status   ret       = ClockP_OK;
    ClockP_safertos *pTimer = (ClockP_safertos*)handle;
    portBaseType    xCreateResult;

    if((NULL_PTR != pTimer) && (BTRUE == pTimer->used))
    {
        if(1 == Osal_isInISRContext())
        {
            /* timeout is ignored when in ISR mode */
            xCreateResult = xTimerStartFromISR(pTimer->timerHndl);
            safertosapiYIELD_FROM_ISR();
        }
        else
        {
            xCreateResult = xTimerStart(pTimer->timerHndl, safertosapiMAX_DELAY);
        }
        if(pdPASS == xCreateResult)
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
    ClockP_Status   ret       = ClockP_OK;
    ClockP_safertos *pTimer   = (ClockP_safertos*)handle;
    portBaseType    xCreateResult;

    if((NULL_PTR != pTimer) && (BTRUE == pTimer->used))
    {
        if(1 == Osal_isInISRContext())
        {
            /* timeout is ignored when in ISR mode */
            xCreateResult = xTimerStopFromISR(pTimer->timerHndl);
            safertosapiYIELD_FROM_ISR();
        }
        else
        {
            xCreateResult = xTimerStop(pTimer->timerHndl, safertosapiMAX_DELAY);
        }
        if(pdPASS == xCreateResult)
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
