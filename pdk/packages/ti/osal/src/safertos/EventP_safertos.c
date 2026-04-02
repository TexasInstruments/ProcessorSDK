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
 *  ======== EventP_safertos.c ========
 */

#include "SafeRTOS_priv.h"

#include <ti/osal/EventP.h>
#include <eventgroups.h>


uint32_t  gOsalEventAllocCnt = 0U, gOsalEventPeak = 0U;

/*!
 *  @brief    EventP_safertos structure
 */
typedef struct EventP_safertos_s
{
    bool                    used;
    eventGroupType          eventObj;
    eventGroupHandleType    eventHndl;
} EventP_safertos;

/* global pool of statically allocated event pools */
static EventP_safertos gOsalEventPSafeRtosPool[OSAL_SAFERTOS_CONFIGNUM_EVENT];

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
    EventP_Handle       ret_handle = NULL_PTR;
    EventP_safertos     *handle = (EventP_safertos *) NULL_PTR;
    EventP_safertos     *eventPool;
    uint32_t            i;
    uintptr_t           key;
    uint32_t            maxEvent;
    portBaseType        xCreateResult;

    DebugP_assert(NULL_PTR != params);

    /* Pick up the internal static memory block */
    eventPool       = (EventP_safertos *) &gOsalEventPSafeRtosPool[0];
    maxEvent        = OSAL_SAFERTOS_CONFIGNUM_EVENT;
    
    if(0U == gOsalEventAllocCnt)
    {
        (void)memset( (void *)gOsalEventPSafeRtosPool,0,sizeof(gOsalEventPSafeRtosPool));
    }

    key = HwiP_disable();

     for(i = 0; i < maxEvent; i++)
     {
         if(BFALSE == eventPool[i].used)
         {
             eventPool[i].used = BTRUE;
             /* Update statistics */
             gOsalEventAllocCnt++;
             if(gOsalEventAllocCnt > gOsalEventPeak)
             {
                 gOsalEventPeak = gOsalEventAllocCnt;
             }
             break;
         }
     }
    HwiP_restore(key);

    if(i < maxEvent)
    {
        /* Grab the memory */
        handle = (EventP_safertos *) &eventPool[i];
    }

    if(NULL_PTR == handle) {
        ret_handle = NULL_PTR;
    }
    else
    {
        xCreateResult = xEventGroupCreate(&(handle->eventObj), &handle->eventHndl);
        if(pdPASS == xCreateResult)
        {
            ret_handle = ((EventP_Handle)handle);
        }
        else
        {
            /* If there was an error reset the event object and return NULL. */
            key = HwiP_disable();
            handle->used = BFALSE;
            /* Found the osal event object to delete */
            if(0U < gOsalEventAllocCnt)
            {
                gOsalEventAllocCnt--;
            }
            HwiP_restore(key);
            ret_handle = NULL_PTR;
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
    EventP_safertos *event = (EventP_safertos *)*handle;
    portBaseType    xCreateResult;

    if((NULL_PTR != event) && (BTRUE == event->used))
    {
        xCreateResult = xEventGroupDelete(event->eventHndl);

        if(pdPASS == xCreateResult)
        {
            key = HwiP_disable();
            event->used = BFALSE;
            /* Found the osal event object to delete */
            if(0U < gOsalEventAllocCnt)
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
    DebugP_assert(EventP_ID_23 >= eventMask);
    DebugP_assert(EventP_ID_NONE != eventMask);

    EventP_safertos *event = (EventP_safertos *)handle;

    eventBitsType pxEventBitsSet = 0;
    uint32_t        eventBits = 0U;
    portBaseType    xCreateResult;

    if((NULL_PTR != event) && (BTRUE == event->used) && (EventP_WaitMode_ALL >= waitMode))
    {
        xCreateResult = xEventGroupWaitBits(event->eventHndl,
                                                  (eventBitsType)eventMask,
                                                  (portBaseType)1U,
                                                  (portBaseType)waitMode,
                                                  &pxEventBitsSet,
                                                  (portTickType)timeout);
        if(pdPASS == xCreateResult)
        {
            /* all the event mask bits heve been set, so updating eventBits with eventMask*/
            eventBits = (uint32_t)pxEventBitsSet;
        }
        else
        {
            /* There was en error so updating eventBits with zero */
            eventBits = 0U;
        }
    }

    return eventBits;    
}

/*
 *  ======== EventP_post ========
 */
EventP_Status EventP_post(EventP_Handle handle, uint32_t eventMask)
{
    DebugP_assert(eventMask <= EventP_ID_23);

    EventP_Status   ret_val = EventP_OK;
    EventP_safertos *event = (EventP_safertos *)handle;
    portBaseType    xCreateResult;
    
    if((NULL_PTR != event) && (BTRUE == event->used))
    {
        if(1 == Osal_isInISRContext())
        {
            xCreateResult  = xEventGroupSetBitsFromISR(event->eventHndl,
                                                 (eventBitsType)eventMask);

            if(pdPASS == xCreateResult)
            {
                safertosapiYIELD_FROM_ISR();
                ret_val = EventP_OK;
            }
            else
            {
                ret_val = EventP_FAILURE;
            }
        }
        else
        {
            xCreateResult  = xEventGroupSetBits(event->eventHndl, (eventBitsType)eventMask);
            if(pdPASS == xCreateResult)
            {
                ret_val = EventP_OK;
            }
            else
            {
                ret_val = EventP_FAILURE;
            }
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
    EventP_safertos *event = (EventP_safertos *)handle;
    uint32_t        eventBitsGet = 0U;
    portBaseType    xCreateResult;
    
    if((NULL_PTR != event) && (BTRUE == event->used))
    {
        if(1 == Osal_isInISRContext())
        {
            xCreateResult = xEventGroupGetBitsFromISR(event->eventHndl, (eventBitsType *)&eventBitsGet);
            if(pdPASS != xCreateResult)
            {
                /* error has occurred, updating eventBitsGet to zero */
                /* TODO this can be updated to 0xFF to report error */
                eventBitsGet = 0U;
            }
        }
        else
        {
            xCreateResult = xEventGroupGetBits(event->eventHndl, (eventBitsType *)&eventBitsGet);
            if(pdPASS != xCreateResult)
            {
                /* error has occurred, updating eventBitsGet to zero */
                /* TODO this can be updated to 0xFF to report error */
                eventBitsGet = 0U;
            }
        }
    }

    return eventBitsGet;
}

