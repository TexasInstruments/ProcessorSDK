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
 *  ======== QueueP_freertos.c ========
 */

#include <ti/osal/QueueP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/soc/osal_soc.h>

uint32_t  gOsalQueueAllocCnt = 0U, gOsalQueuePeak = 0U;

/*!
 *  @brief  FreeRTOS Queue element
 *
 *  Structure that defines a single queue element and/or a list of queue elements.
 *  A field of this type is placed at the head of client structs.
 *
 */
typedef struct Queue_Elem_s{
    /*! Pointer to the next queue element */
    struct Queue_Elem_s *next;

    /*! Pointer to the previous queue element */
    struct Queue_Elem_s *prev;
} Queue_Elem;


/*!
 *  @brief    QueueP_freertos structure
 */
typedef struct QueueP_freertos_s
{
    bool                used;
    Queue_Elem          queueHndl;
} QueueP_freertos;

/* global pool of statically allocated queue pools */
static QueueP_freertos gOsalQueuePFreeRtosPool[OSAL_FREERTOS_CONFIGNUM_QUEUE];

/*
 *  ======== QueueP_Params_init ========
 */
void QueueP_Params_init(QueueP_Params *params)
{
    if (NULL_PTR != params)
    {
        params->pErrBlk =  NULL_PTR;
    }
    return;
}

/*
 *  ======== QueueP_create ========
 */
QueueP_Handle QueueP_create(const QueueP_Params *params)
{
    QueueP_Handle       ret_handle; 
    QueueP_freertos     *handle = (QueueP_freertos *) NULL_PTR;
    QueueP_freertos     *queuePool;
    Queue_Elem          *q;
    uint32_t            i;
    uintptr_t           key;
    uint32_t            maxQueue;

    /* Pick up the internal static memory block */
    queuePool       = (QueueP_freertos *) &gOsalQueuePFreeRtosPool[0];
    maxQueue        = OSAL_FREERTOS_CONFIGNUM_QUEUE;
    
    if(0U == gOsalQueueAllocCnt) 
    {
        (void)memset( (void *)gOsalQueuePFreeRtosPool,0,sizeof(gOsalQueuePFreeRtosPool));
    }

    key = HwiP_disable();

     for (i = 0U; i < maxQueue; i++)
     {
         if (BFALSE == queuePool[i].used)
         {
             queuePool[i].used = BTRUE;
             /* Update statistics */
             gOsalQueueAllocCnt++;
             if (gOsalQueueAllocCnt > gOsalQueuePeak)
             {
                 gOsalQueuePeak = gOsalQueueAllocCnt;
             }
             break;
         }
     }
    HwiP_restore(key);

    if (i < maxQueue)
    {
        /* Grab the memory */
        handle = (QueueP_freertos *) &queuePool[i];
    }

    if (NULL_PTR == handle) {
        ret_handle = NULL_PTR;
    }
    else
    {
        q = &handle->queueHndl;

        q->next = q;
        q->prev = q;

        ret_handle = ((QueueP_Handle)handle);
    }

    return ret_handle;
}

/*
 *  ======== QueueP_delete ========
 */
QueueP_Status QueueP_delete(QueueP_Handle handle)
{
    uintptr_t       key;
    QueueP_Status   ret_val = QueueP_OK;
    QueueP_freertos *queue = (QueueP_freertos *)handle;
    Queue_Elem      *q;

    if((NULL_PTR != queue) && (BTRUE == queue->used))
    {
        q = &queue->queueHndl;

        q->next = (Queue_Elem *)NULL_PTR;
        q->prev = (Queue_Elem *)NULL_PTR;

        key = HwiP_disable();
        queue->used = BFALSE;
        /* Found the osal queue object to delete */
        if (0U < gOsalQueueAllocCnt)
        {
            gOsalQueueAllocCnt--;
        }
        HwiP_restore(key);
        ret_val = QueueP_OK;
    }
    else
    {
       ret_val = QueueP_FAILURE;
    }  

    return ret_val;
}

/*
 *  ======== QueueP_get ========
 */
void * QueueP_get(QueueP_Handle handle)
{
    DebugP_assert(NULL_PTR != handle);

    uintptr_t       key;
    QueueP_freertos *queue = (QueueP_freertos *)handle;
    Queue_Elem      *pElem = NULL;
    Queue_Elem      *q;
    
    key = HwiP_disable();

    if (BTRUE == queue->used)
    { 
        q = &queue->queueHndl;

        if (BTRUE != QueueP_isEmpty(handle))
        {
            pElem = q->next;

            q->next = pElem->next;
            pElem->next->prev = q;
        }
        else
        {
            pElem = NULL;
        }
    }
    
    HwiP_restore(key);

    return (pElem);
}

/*
 *  ======== QueueP_put ========
 */
QueueP_Status QueueP_put(QueueP_Handle handle, void *elem)
{
    uintptr_t       key;
    QueueP_Status   ret_val = QueueP_OK;
    QueueP_freertos *queue = (QueueP_freertos *)handle;
    Queue_Elem      *pElem = (Queue_Elem *)elem;
    Queue_Elem      *q;

    key = HwiP_disable();

    if((NULL_PTR != queue) && (BTRUE == queue->used) && (NULL_PTR != elem))
    {
        q = &queue->queueHndl;

        pElem->next = q;
        pElem->prev = q->prev;
        q->prev->next = pElem;
        q->prev = pElem;
    }
    else
    {
       ret_val = QueueP_FAILURE;
    }  

    HwiP_restore(key);

    return ret_val;
}

/*
 *  ======== QueueP_isEmpty ========
 */
QueueP_State QueueP_isEmpty(QueueP_Handle handle)
{
    DebugP_assert(NULL_PTR != handle);

    QueueP_State    ret_val;
    QueueP_freertos *queue = (QueueP_freertos *)handle;

    if(queue->queueHndl.next == &(queue->queueHndl))
    {
        ret_val = QueueP_EMPTY;
    } 
    else
    {
        ret_val = QueueP_NOTEMPTY;  
    }
    
    return (ret_val);
}

/*
 *  ======== QueueP_getQPtr ========
 */
void * QueueP_getQPtr(QueueP_Handle handle)
{
    DebugP_assert(NULL_PTR != handle);

    QueueP_freertos *queue = (QueueP_freertos *)handle;

    return (&queue->queueHndl);
}

/* Nothing past this point */
