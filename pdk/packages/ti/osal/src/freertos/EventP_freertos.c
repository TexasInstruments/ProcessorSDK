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
 *  ======== EventP_freertos.c ========
 */

#include <ti/osal/EventP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/soc/osal_soc.h>

#include <FreeRTOS.h>
#include <event_groups.h>

uint32_t  gOsalEventAllocCnt = 0U, gOsalEventPeak = 0U;

/*!
 *  @brief    EventP_freertos structure
 */
typedef struct EventP_freertos_s
{
    bool               used;
    StaticEventGroup_t eventObj;
    EventGroupHandle_t eventHndl;
} EventP_freertos;

/* global pool of statically allocated event pools */
static EventP_freertos gOsalEventPFreeRtosPool[OSAL_FREERTOS_CONFIGNUM_EVENT];

/*
 *  ======== EventP_Params_init ========
 */
void EventP_Params_init(EventP_Params *params)
{

}

/*
 *  ======== EventP_create ========
 */
EventP_Handle EventP_create(EventP_Params *params)
{
    EventP_Handle       ret_handle; 
    EventP_freertos     *handle = (EventP_freertos *) NULL_PTR;
    EventP_freertos     *eventPool;
    uint32_t            i;
    uintptr_t           key;
    uint32_t            maxEvent;

    DebugP_assert(NULL_PTR != params);

    /* Pick up the internal static memory block */
    eventPool       = (EventP_freertos *) &gOsalEventPFreeRtosPool[0];
    maxEvent        = OSAL_FREERTOS_CONFIGNUM_EVENT;
    
    if(0U == gOsalEventAllocCnt) 
    {
        (void)memset( (void *)gOsalEventPFreeRtosPool,0,sizeof(gOsalEventPFreeRtosPool));
    }

    key = HwiP_disable();

     for (i = 0U; i < maxEvent; i++)
     {
         if (BFALSE == eventPool[i].used)
         {
             eventPool[i].used = BTRUE;
             /* Update statistics */
             gOsalEventAllocCnt++;
             if (gOsalEventAllocCnt > gOsalEventPeak)
             {
                 gOsalEventPeak = gOsalEventAllocCnt;
             }
             break;
         }
     }
    HwiP_restore(key);

    if (i < maxEvent)
    {
        /* Grab the memory */
        handle = (EventP_freertos *) &eventPool[i];
    }

    if (NULL_PTR == handle) {
        ret_handle = NULL_PTR;
    }
    else
    {
        handle->eventHndl = xEventGroupCreateStatic(&(handle->eventObj));
        if(NULL == handle->eventHndl)
        {
            /* If there was an error reset the event object and return NULL. */
            key = HwiP_disable();
            handle->used = BFALSE;
            /* Found the osal event object to delete */
            if (0U < gOsalEventAllocCnt)
            {
                gOsalEventAllocCnt--;
            }
            HwiP_restore(key);
            ret_handle = NULL_PTR;
        }
        else
        {
            ret_handle = ((EventP_Handle)handle);
        }
    }

    return ret_handle;
}

/*
 *  ======== EventP_delete ========
 */
EventP_Status EventP_delete(EventP_Handle *handle)
{
    uintptr_t       key;
    EventP_Status   ret_val = EventP_OK;
    EventP_freertos *event = (EventP_freertos *)*handle;

    if((NULL_PTR != event) && (BTRUE == event->used))
    {
        vEventGroupDelete(event->eventHndl);

        key = HwiP_disable();
        event->used = BFALSE;
        /* Found the osal event object to delete */
        if (0U < gOsalEventAllocCnt)
        {
            gOsalEventAllocCnt--;
        }
        HwiP_restore(key);
        ret_val = EventP_OK;
    }
    else
    {
       ret_val = EventP_FAILURE;
    }  

    return ret_val;
}

/*
 *  ======== EventP_wait ========
 */
uint32_t EventP_wait(EventP_Handle handle, uint32_t eventMask,
                    uint8_t waitMode, uint32_t timeout)
{
    DebugP_assert(EventP_ID_23   >= eventMask);
    DebugP_assert(EventP_ID_NONE != eventMask);

    EventP_freertos *event = (EventP_freertos *)handle;
    uint32_t        eventBits = 0U;

    if((NULL_PTR != event) && 
       (BTRUE == event->used) && 
       (EventP_WaitMode_ALL >= waitMode))
    {
        eventBits = (uint32_t)xEventGroupWaitBits(event->eventHndl,
                                                  (EventBits_t)eventMask,
                                                  (BaseType_t)1U,
                                                  (BaseType_t)waitMode,
                                                  (TickType_t)timeout);
    }

    return eventBits;    
}

/*
 *  ======== EventP_post ========
 */
EventP_Status EventP_post(EventP_Handle handle, uint32_t eventMask)
{
    DebugP_assert(EventP_ID_23 >= eventMask);

    EventP_Status   ret_val = EventP_OK;
    EventP_freertos *event = (EventP_freertos *)handle;
    
    if((NULL_PTR != event) && (BTRUE == event->used))
    {
        if( 1 == xPortInIsrContext() )
        {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            BaseType_t xResult;

            xResult  = xEventGroupSetBitsFromISR(event->eventHndl,
                                                 (EventBits_t)eventMask,
                                                 &xHigherPriorityTaskWoken);

            if(pdFAIL != xResult)
            {
                portYIELD_FROM_ISR((uint32_t)xHigherPriorityTaskWoken);
                ret_val = EventP_OK;
            }
            else
            {
                ret_val = EventP_FAILURE;
            }
        }
        else
        {
            (void)xEventGroupSetBits(event->eventHndl, (EventBits_t)eventMask);
        }
    }
    else
    {
        ret_val = EventP_FAILURE;
    }

    return ret_val;
}

/*
 *  ======== EventP_getPostedEvents ========
 */
uint32_t EventP_getPostedEvents(EventP_Handle handle)
{
    EventP_freertos *event = (EventP_freertos *)handle;
    uint32_t        eventBits = 0U;
    
    if((NULL_PTR != event) && (BTRUE == event->used))
    {
        if( 1 == xPortInIsrContext() )
        {
            eventBits = (uint32_t)xEventGroupGetBitsFromISR(event->eventHndl);
        }
        else
        {
            eventBits = (uint32_t)xEventGroupGetBits(event->eventHndl);
        }
    }

    return eventBits;
}

