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
 *  ======== MailboxP_freertos.c ========
 */

#include <ti/osal/MailboxP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/soc/osal_soc.h>

#include <FreeRTOS.h>
#include "queue.h"

uint32_t  gOsalMailboxAllocCnt = 0U, gOsalMailboxPeak = 0U;

/*!
 *  @brief    MailboxP_freertos structure
 */
typedef struct MailboxP_freertos_s
{
    bool                used;
    StaticQueue_t       mailboxObj;
    QueueHandle_t       mailboxHndl;
} MailboxP_freertos;

/* global pool of statically allocated mailbox pools */
static MailboxP_freertos gOsalMailboxPFreeRtosPool[OSAL_FREERTOS_CONFIGNUM_MAILBOX];

void MailboxP_Params_init(MailboxP_Params *params)
{
    if (NULL_PTR != params)
    {
        params->pErrBlk =  NULL_PTR;
        params->name    =  NULL_PTR;
        params->size    =  0U;
        params->count   =  0U;
        params->bufsize =  0U;
        params->buf     =  NULL_PTR;
    }
    return;
}

MailboxP_Handle MailboxP_create(const MailboxP_Params *params)
{
    MailboxP_Handle     ret_handle; 
    MailboxP_freertos   *handle = (MailboxP_freertos *) NULL_PTR;
    MailboxP_freertos   *mailboxPool;
    uint32_t            i;
    uintptr_t           key;
    uint32_t            maxMailbox;

    DebugP_assert(NULL_PTR != params);
    DebugP_assert(NULL_PTR != params->buf);
    DebugP_assert((params->bufsize >= (params->size * params->count)));

    /* Pick up the internal static memory block */
    mailboxPool       = (MailboxP_freertos *) &gOsalMailboxPFreeRtosPool[0];
    maxMailbox        = OSAL_FREERTOS_CONFIGNUM_MAILBOX;
    
    if(0U == gOsalMailboxAllocCnt) 
    {
        (void)memset( (void *)gOsalMailboxPFreeRtosPool,0,sizeof(gOsalMailboxPFreeRtosPool));
    }

    key = HwiP_disable();

     for (i = 0U; i < maxMailbox; i++)
     {
         if (BFALSE == mailboxPool[i].used)
         {
             mailboxPool[i].used = BTRUE;
             /* Update statistics */
             gOsalMailboxAllocCnt++;
             if (gOsalMailboxAllocCnt > gOsalMailboxPeak)
             {
                 gOsalMailboxPeak = gOsalMailboxAllocCnt;
             }
             break;
         }
     }
    HwiP_restore(key);

    if (i < maxMailbox)
    {
        /* Grab the memory */
        handle = (MailboxP_freertos *) &mailboxPool[i];
    }

    if (NULL_PTR == handle) {
        ret_handle = NULL_PTR;
    }
    else
    {
        handle->mailboxHndl = xQueueCreateStatic((UBaseType_t)params->count, 
                                        (UBaseType_t)params->size, 
                                        (uint8_t*)params->buf,
                                        &handle->mailboxObj);
        if(NULL == handle->mailboxHndl)
        {
            /* If there was an error reset the mailbox object and return NULL. */
            key = HwiP_disable();
            handle->used = BFALSE;
            /* Found the osal mailbox object to delete */
            if (0U < gOsalMailboxAllocCnt)
            {
                gOsalMailboxAllocCnt--;
            }
            HwiP_restore(key);
            ret_handle = NULL_PTR;
        }
        else
        {
            ret_handle = ((MailboxP_Handle)handle);
        }
    }

    return ret_handle;
}

MailboxP_Status MailboxP_delete(MailboxP_Handle handle)
{
    uintptr_t   key;
    MailboxP_Status ret_val = MailboxP_OK;
    MailboxP_freertos *mailbox = (MailboxP_freertos *)handle;

    if((NULL_PTR != mailbox) && (BTRUE == mailbox->used))
    {
        vQueueDelete(mailbox->mailboxHndl);

        key = HwiP_disable();
        mailbox->used = BFALSE;
        /* Found the osal mailbox object to delete */
        if (0U < gOsalMailboxAllocCnt)
        {
            gOsalMailboxAllocCnt--;
        }
        HwiP_restore(key);
        ret_val = MailboxP_OK;
    }
    else
    {
       ret_val = MailboxP_FAILURE;
    }  

    return ret_val;
}

MailboxP_Status MailboxP_post(MailboxP_Handle handle,
                              void * msg,
                              uint32_t timeout)
{
    DebugP_assert(NULL_PTR != handle);
    
    BaseType_t qStatus;
    MailboxP_Status ret_val = MailboxP_OK;
    MailboxP_freertos *mailbox = (MailboxP_freertos *)handle;

    if( 1 == xPortInIsrContext() )
    {
        BaseType_t xHigherPriorityTaskWoken = 0;

        /* timeout is ignored when in ISR mode */
        qStatus = xQueueSendToBackFromISR(mailbox->mailboxHndl, msg, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR((uint32_t)xHigherPriorityTaskWoken);
    }
    else
    {
        if (MailboxP_WAIT_FOREVER == timeout)
        {
            qStatus = xQueueSendToBack(mailbox->mailboxHndl, msg, portMAX_DELAY);
        }
        else
        {
            qStatus = xQueueSendToBack(mailbox->mailboxHndl, msg, timeout);
        }
    }

    if (pdPASS == qStatus)
    {
        ret_val = MailboxP_OK;
    }
    else
    {
        ret_val = MailboxP_TIMEOUT;
    }

    return ret_val;
}

MailboxP_Status MailboxP_pend(MailboxP_Handle handle,
                              void * msg,
                              uint32_t timeout)
{
    DebugP_assert(NULL_PTR != handle);
    
    BaseType_t qStatus;
    MailboxP_Status ret_val = MailboxP_OK;
    MailboxP_freertos *mailbox = (MailboxP_freertos *)handle;

    if( 1 == xPortInIsrContext() )
    {
        BaseType_t xHigherPriorityTaskWoken = 0;

        /* timeout is ignored when in ISR mode */
        qStatus = xQueueReceiveFromISR(mailbox->mailboxHndl, msg, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR((uint32_t)xHigherPriorityTaskWoken);
    }
    else
    {
        if (MailboxP_WAIT_FOREVER == timeout)
        {
            qStatus = xQueueReceive(mailbox->mailboxHndl, msg, portMAX_DELAY);
        }
        else
        {
            qStatus = xQueueReceive(mailbox->mailboxHndl, msg, timeout);
        }
    }

    if (pdPASS == qStatus )
    {
        ret_val = MailboxP_OK;
    }
    else
    {
        ret_val = MailboxP_TIMEOUT;
    }

    return ret_val;
}

int32_t MailboxP_getNumPendingMsgs(MailboxP_Handle handle)
{
    DebugP_assert(NULL_PTR != handle);

    UBaseType_t numMsg;
    MailboxP_freertos *mailbox = (MailboxP_freertos *)handle;

    if( 1 == xPortInIsrContext() )
    {
        numMsg = uxQueueMessagesWaitingFromISR(mailbox->mailboxHndl);
    }
    else
    {
        numMsg = uxQueueMessagesWaiting(mailbox->mailboxHndl);
    }

    return ((int32_t)numMsg);
}

/* Nothing past this point */
