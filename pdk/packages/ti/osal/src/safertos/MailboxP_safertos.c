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
 *  ======== MailboxP_safertos.c ========
 */

#include "SafeRTOS_priv.h"

#include <ti/osal/MailboxP.h>
#include <queue.h>


uint32_t  gOsalMailboxAllocCnt = 0U, gOsalMailboxPeak = 0U;

/*!
 *  @brief    MailboxP_safertos structure
 */
typedef struct MailboxP_safertos_s
{
    bool                used;
    xQueueHandle        mailboxHndl;
    /* Copy of buffer pointer passed by application */
    void                *buf;
    uint32_t            bufsize;
} MailboxP_safertos;

/* global pool of statically allocated mailbox pools */
static MailboxP_safertos gOsalMailboxPSafeRtosPool[OSAL_SAFERTOS_CONFIGNUM_MAILBOX];

void MailboxP_Params_init(MailboxP_Params *params)
{
    if(NULL_PTR != params)
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
    MailboxP_safertos   *handle = (MailboxP_safertos *) NULL_PTR;
    MailboxP_safertos   *mailboxPool;
    uint32_t            i;
    uintptr_t           key;
    uint32_t            maxMailbox;
    portBaseType        xCreateResult;

    DebugP_assert(NULL_PTR != params);
    DebugP_assert(NULL_PTR != params->buf);
    DebugP_assert((params->bufsize >= (params->size * params->count)));

    /* Pick up the internal static memory block */
    mailboxPool       = (MailboxP_safertos *) &gOsalMailboxPSafeRtosPool[0];
    maxMailbox        = OSAL_SAFERTOS_CONFIGNUM_MAILBOX;
    
    if(0U == gOsalMailboxAllocCnt) 
    {
        (void)memset( (void *)gOsalMailboxPSafeRtosPool,0,sizeof(gOsalMailboxPSafeRtosPool));
    }

    key = HwiP_disable();

     for(i = 0U; i < maxMailbox; i++)
     {
         if(BFALSE == mailboxPool[i].used)
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

    if(i < maxMailbox)
    {
        /* Grab the memory */
        handle = (MailboxP_safertos *) &mailboxPool[i];
    }

    if(NULL_PTR == handle) {
        ret_handle = NULL_PTR;
    }
    else
    {
        /* Note : Here buffer size should have addition portQUEUE_OVERHEAD_BYTES */

        xCreateResult = xQueueCreate((portInt8Type*)params->buf,
                                (portUnsignedBaseType)params->bufsize,
                                (portUnsignedBaseType)params->count,
                                (portUnsignedBaseType)params->size,
                                &handle->mailboxHndl);
        if(pdPASS != xCreateResult)
        {
            /* If there was an error reset the mailbox object and return NULL. */
            key = HwiP_disable();
            handle->used = BFALSE;
            /* Found the osal mailbox object to delete */
            if(0U < gOsalMailboxAllocCnt)
            {
                gOsalMailboxAllocCnt--;
            }
            HwiP_restore(key);
            ret_handle = NULL_PTR;
        }
        else
        {
            /* Make a local Copy of buffer address and size passed by application */
            handle->buf = params->buf;
            handle->bufsize = params->bufsize;
            ret_handle = ((MailboxP_Handle)handle);
        }
    }

    return ret_handle;
}

MailboxP_Status MailboxP_delete(MailboxP_Handle handle)
{
    DebugP_assert(NULL_PTR != handle);

    MailboxP_Status ret_val = MailboxP_FAILURE;

    /*
     * NOTE : there is no delete queue API in safertos.
     */

    return ret_val;
}

MailboxP_Status MailboxP_post(MailboxP_Handle handle,
                              void * msg,
                              uint32_t timeout)
{
    DebugP_assert(NULL_PTR != handle);
    
    portBaseType xCreateResult;
    MailboxP_Status ret_val = MailboxP_OK;
    MailboxP_safertos *mailbox = (MailboxP_safertos *)handle;

    if(1 == Osal_isInISRContext())
    {
        /* timeout is ignored when in ISR mode */
        xCreateResult = xQueueSendFromISR(mailbox->mailboxHndl, msg);
        safertosapiYIELD_FROM_ISR();
    }
    else
    {
        if(MailboxP_WAIT_FOREVER == timeout)
        {
            xCreateResult = xQueueSend(mailbox->mailboxHndl, msg, safertosapiMAX_DELAY);
        }
        else
        {
            xCreateResult = xQueueSend(mailbox->mailboxHndl, msg, timeout);
        }
    }

    if(pdPASS == xCreateResult)
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
    
    portBaseType xCreateResult;
    MailboxP_Status ret_val = MailboxP_OK;
    MailboxP_safertos *mailbox = (MailboxP_safertos *)handle;

    if(1 == Osal_isInISRContext())
    {
        /* timeout is ignored when in ISR mode */
        xCreateResult = xQueueReceiveFromISR(mailbox->mailboxHndl, msg);
        safertosapiYIELD_FROM_ISR();
    }
    else
    {
        if(MailboxP_WAIT_FOREVER == timeout)
        {
            xCreateResult = xQueueReceive(mailbox->mailboxHndl, msg, safertosapiMAX_DELAY);
        }
        else
        {
            xCreateResult = xQueueReceive(mailbox->mailboxHndl, msg, timeout);
        }
    }

    if(pdPASS == xCreateResult)
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

    portBaseType xCreateResult;
    portUnsignedBaseType numMsg;
    MailboxP_safertos *mailbox = (MailboxP_safertos *)handle;

    if(1 == Osal_isInISRContext())
    {
        /* There is no ISR API for waiting in safertos */
        xCreateResult = errNULL_PARAMETER_SUPPLIED;
    }
    else
    {
        xCreateResult = xQueueMessagesWaiting(mailbox->mailboxHndl, &numMsg);
    }

    if(pdPASS != xCreateResult)
    {
        /* There is an error, return numMsg as zero */
        numMsg = 0;
    }

    return ((int32_t)numMsg);
}

/* Nothing past this point */
