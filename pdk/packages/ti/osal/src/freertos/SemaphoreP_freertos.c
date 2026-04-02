/*
 * Copyright (c) 2015-2024, Texas Instruments Incorporated
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
 *  ======== SemaphoreP_freertos.c ========
 */
#include <ti/osal/SemaphoreP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/soc/osal_soc.h>
#include <ti/osal/src/nonos/Nonos_config.h>

#include <FreeRTOS.h>
#include <semphr.h>

extern uint32_t  gOsalSemAllocCnt, gOsalSemPeak;

/*!
 *  @brief    Semaphore structure
 */
typedef struct SemaphoreP_freertos_s {
    bool               used;
    StaticSemaphore_t  semObj;
    SemaphoreHandle_t  semHndl;
} SemaphoreP_freertos;

/* global pool of statically allocated semaphore pools */
static SemaphoreP_freertos gOsalSemPfreertosPool[OSAL_FREERTOS_CONFIGNUM_SEMAPHORE];

static int32_t SemaphoreP_constructBinary(SemaphoreP_freertos *handle, uint32_t initCount);
static int32_t SemaphoreP_constructCounting(SemaphoreP_freertos *handle, uint32_t initCount, uint32_t maxCount);

/*
 *  ======== SemaphoreP_create ========
 */
SemaphoreP_Handle SemaphoreP_create(uint32_t count,
                                    const SemaphoreP_Params *params)
{
    SemaphoreP_Handle ret_handle;
    SemaphoreP_freertos *handle = (SemaphoreP_freertos *) NULL_PTR;
    uint32_t          i;
    uintptr_t         key;
    uintptr_t         temp;
    SemaphoreP_freertos *semPool;
    uint32_t          maxSemaphores;
    int32_t           retVal;

    /* Check if user has specified any memory block to be used, which gets
     * the precedence over the internal static memory block
     */
    if ((uintptr_t)0U != gOsal_HwAttrs.extSemaphorePBlock.base)
    {
        /* pick up the external memory block configured */
        semPool        = (SemaphoreP_freertos *) gOsal_HwAttrs.extSemaphorePBlock.base;
        temp           = (uintptr_t) gOsal_HwAttrs.extSemaphorePBlock.size;
        maxSemaphores  = (uint32_t)(temp/(sizeof(SemaphoreP_freertos)));
    }
    else
    {
        /* Pick up the internal static memory block */
        semPool        = (SemaphoreP_freertos *) &gOsalSemPfreertosPool[0];
        maxSemaphores  = OSAL_FREERTOS_CONFIGNUM_SEMAPHORE;
        
        if(0U == gOsalSemAllocCnt) 
        {
			(void)memset( (void *)gOsalSemPfreertosPool,0,sizeof(gOsalSemPfreertosPool));
		}
    }

    key = HwiP_disable();

     for (i = 0; i < maxSemaphores; i++)
     {
         if (BFALSE == semPool[i].used)
         {
             semPool[i].used = BTRUE;
             /* Update statistics */
             gOsalSemAllocCnt++;
             if (gOsalSemAllocCnt > gOsalSemPeak)
             {
                 gOsalSemPeak = gOsalSemAllocCnt;
             }
             break;
         }
     }
     HwiP_restore(key);

    if (i < maxSemaphores)
    {
        /* Grab the memory */
        handle = (SemaphoreP_freertos *) &semPool[i];
    }

    if (NULL_PTR == handle) {
        ret_handle = NULL_PTR;
    }
    else
    {
        if (NULL_PTR == params)
        {
            SemaphoreP_Params semParam;
            SemaphoreP_Params_init(&semParam);
            retVal = SemaphoreP_constructCounting(handle, count, semParam.maxCount);
        }
        else
        {
            if (SemaphoreP_Mode_BINARY == params->mode)
            {
                retVal = SemaphoreP_constructBinary(handle, count);
            }
            else
            {
                retVal = SemaphoreP_constructCounting(handle, count, params->maxCount);
            }
        }

        /* If there was an error reset the sem object and return NULL. */
        if (retVal != SemaphoreP_OK)
        {
            key = HwiP_disable();
            handle->used = BFALSE;
            /* Found the osal semaphore object to delete */
            if (0U < gOsalSemAllocCnt)
            {
                gOsalSemAllocCnt--;
            }
            HwiP_restore(key);
            ret_handle = NULL_PTR;
        }
        else
        {
            ret_handle = ((SemaphoreP_Handle)handle);
        }
    }
    return ret_handle;
}

static int32_t SemaphoreP_constructBinary(SemaphoreP_freertos *handle, uint32_t initCount)
{
    int32_t status;

    if ((NULL == handle) || (initCount > 1U))
    {
        status = SemaphoreP_FAILURE;
    }
    else
    {
        handle->semHndl = xSemaphoreCreateBinaryStatic(&handle->semObj);
        if( NULL == handle->semHndl)
        {
            status = SemaphoreP_FAILURE;
        }
        else
        {
            vQueueAddToRegistry(handle->semHndl, "Binary Sem (OSAL)");
            if(1U == initCount)
            {
                /* post a semaphore to increment initial count to 1 */
                (void)xSemaphoreGive(handle->semHndl);
            }
            status = SemaphoreP_OK;
        }
    }

    return status;
}

static int32_t SemaphoreP_constructCounting(SemaphoreP_freertos *handle, uint32_t initCount, uint32_t maxCount)
{
    int32_t status;

    handle->semHndl = xSemaphoreCreateCountingStatic(
                                maxCount,
                                initCount,
                                &handle->semObj);
    if( NULL == handle->semHndl )
    {
        status = SemaphoreP_FAILURE;
    }
    else
    {
        vQueueAddToRegistry(handle->semHndl, "Counting Sem (OSAL)");
        status = SemaphoreP_OK;
    }

    return status;
}

/*
 *  ======== SemaphoreP_delete ========
 */
SemaphoreP_Status SemaphoreP_delete(SemaphoreP_Handle handle)
{
    uintptr_t   key;
    SemaphoreP_Status ret = SemaphoreP_OK;
    SemaphoreP_freertos *semaphore = (SemaphoreP_freertos *)handle;

    if((NULL_PTR != semaphore) && (BTRUE == semaphore->used))
    {
        vSemaphoreDelete(semaphore->semHndl);

        key = HwiP_disable();
        semaphore->used = BFALSE;
        /* Found the osal semaphore object to delete */
        if (0U < gOsalSemAllocCnt)
        {
            gOsalSemAllocCnt--;
        }
        HwiP_restore(key);
        ret = SemaphoreP_OK;
    }
    else
    {
       ret = SemaphoreP_FAILURE;
    }  
    return (ret);
}

/*
 *  ======== SemaphoreP_Params_init ========
 */
void SemaphoreP_Params_init(SemaphoreP_Params *params)
{
    if(NULL_PTR != params)
    {
        params->mode = SemaphoreP_Mode_COUNTING;
        params->name = (char *) NULL_PTR;
        params->maxCount = 0xFFU;
    }
}

/*
 *  ======== SemaphoreP_pend ========
 */
SemaphoreP_Status SemaphoreP_pend(SemaphoreP_Handle handle, uint32_t timeout)
{
    uint32_t            isSemTaken;
    SemaphoreP_Status   ret_val;
    SemaphoreP_freertos *pSemaphore = (SemaphoreP_freertos *)handle;

    if(NULL_PTR == handle)
    {
        ret_val = SemaphoreP_FAILURE;
    }
    else
    {
        if( 1 == xPortInIsrContext() )
        {
            BaseType_t xHigherPriorityTaskWoken = 0;

            /* timeout is ignored when in ISR mode */
            isSemTaken = (uint32_t)xSemaphoreTakeFromISR(pSemaphore->semHndl,&xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR((uint32_t)xHigherPriorityTaskWoken);
        }
        else
        {
            if (timeout == SemaphoreP_WAIT_FOREVER)
            {
                isSemTaken = (uint32_t)xSemaphoreTake(pSemaphore->semHndl, portMAX_DELAY);
            }
            else
            {
                isSemTaken = (uint32_t)xSemaphoreTake(pSemaphore->semHndl, timeout);
            }
        }

        if(0U != isSemTaken)
        {
            ret_val = SemaphoreP_OK;
        }
        else
        {
            ret_val = SemaphoreP_TIMEOUT;
        }
    }

    return ret_val;
}

/*
 *  ======== SemaphoreP_post ========
 */
SemaphoreP_Status SemaphoreP_post(SemaphoreP_Handle handle)
{
    SemaphoreP_Status ret = SemaphoreP_OK;
    SemaphoreP_freertos *pSemaphore = (SemaphoreP_freertos *)handle;

    if(NULL_PTR != pSemaphore)
    {
        if( 1 == xPortInIsrContext() )
        {
            BaseType_t xHigherPriorityTaskWoken = 0;

            (void)xSemaphoreGiveFromISR(pSemaphore->semHndl, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR((uint32_t)xHigherPriorityTaskWoken);
        }
        else
        {
            (void)xSemaphoreGive(pSemaphore->semHndl);
        }
        ret = SemaphoreP_OK;
    }
    else
    {
        ret = SemaphoreP_FAILURE;
    }

    return ret;
}

/*
 *  ======== SemaphoreP_postFromClock ========
 */
SemaphoreP_Status SemaphoreP_postFromClock(SemaphoreP_Handle handle)
{
    return (SemaphoreP_post(handle));
}

/*
 *  ======== SemaphoreP_postFromISR ========
 */
SemaphoreP_Status SemaphoreP_postFromISR(SemaphoreP_Handle handle)
{
    return (SemaphoreP_post(handle));
}
/*
 *  ======== SemaphoreP_getCount ========
 */
int32_t SemaphoreP_getCount(SemaphoreP_Handle handle)
{
    int32_t uxCount = 0;
    if(NULL_PTR != handle)
    {
        SemaphoreP_freertos *pSemaphore = (SemaphoreP_freertos *)handle;
        uxCount = ((int32_t)uxSemaphoreGetCount(pSemaphore->semHndl));
    }

    return uxCount;
}

/* IMPORTANT:
   This structure is copied from packages/ti/kernel/freertos/FreeRTOS-LTS/FreeRTOS-Kernel/queue.c
   and is used for ROV based views in CCS

   Due to presence of union in xQUEUE, ROV does not fetch the data correctly.
   Below is identical structure except that there is no union and
   instead the element having maximum size in the union is put as a field in below structure.
   Rest of the fields are same as xQUEUE.

   This is not used anywhere in the run-time application and has effect only when
   ROV is used
 */
typedef struct SemaphoreData
{
    TaskHandle_t xMutexHolder;        /*< The handle of the task that holds the mutex. */
    UBaseType_t uxRecursiveCallCount; /*< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} SemaphoreData_t;

typedef struct /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pcHead;           /*< Points to the beginning of the queue storage area. */
    int8_t * pcWriteTo;        /*< Points to the free next place in the storage area. */

    SemaphoreData_t xSemaphore; /*< Data required exclusively when this structure is used as a semaphore. */

    List_t xTasksWaitingToSend;             /*< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t xTasksWaitingToReceive;          /*< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    volatile UBaseType_t uxMessagesWaiting; /*< The number of items currently in the queue. */
    UBaseType_t uxLength;                   /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t uxItemSize;                 /*< The size of each items that the queue will hold. */

    volatile int8_t cRxLock;                /*< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    volatile int8_t cTxLock;                /*< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */

    #if ( ( 1 == configSUPPORT_STATIC_ALLOCATION ) && ( 1 == configSUPPORT_DYNAMIC_ALLOCATION ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #endif

    #if ( 1 == configUSE_QUEUE_SETS )
        struct QueueDefinition * pxQueueSetContainer;
    #endif

    #if ( 1 == configUSE_TRACE_FACILITY )
        UBaseType_t uxQueueNumber;
        uint8_t ucQueueType;
    #endif
} xQUEUE_ROV;

/* This is defined so that the symbol is retained in the final executable */
xQUEUE_ROV gRovDummyQueue __attribute__((used));


/* Nothing past this point */
