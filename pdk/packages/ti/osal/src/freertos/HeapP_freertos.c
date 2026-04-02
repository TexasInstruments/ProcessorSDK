/*
 * Copyright (c) 2018-2025, Texas Instruments Incorporated
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
 *  ======== HeapP_freertos.c ========
 */
#include <ti/osal/HeapP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/soc/osal_soc.h>
#include <ti/osal/src/freertos/HeapP_freertos_internal.h>

#if defined (FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif
#if defined (SAFERTOS)
#include "SafeRTOS.h"
#include "task.h"
/*Replace with safertos API's */
#define vTaskSuspendAll vTaskSuspendScheduler
#define xTaskResumeAll xTaskResumeScheduler
#endif

extern uint32_t  gOsalHeapAllocCnt, gOsalHeapPeak;

/*!
 *  @brief    Heap structure
 */
typedef struct HeapP_freertos_s {
    bool used;
    StaticHeap_t heapHndl;
} HeapP_freertos;

/* global pool of statically allocated heap pools */
static HeapP_freertos gOsalHeapPFreeRtosPool[OSAL_FREERTOS_CONFIGNUM_HEAP];

/*
 *  ======== HeapP_Params_init ========
 */
void HeapP_Params_init(HeapP_Params *params)
{
    if (NULL_PTR != params )
    {
        params->size    =  0U;
        params->buf     =  NULL_PTR;
        params->align   =  256U;
    }
    return;
}

/*
 *  ======== HeapP_create ========
 */
HeapP_Handle HeapP_create(const HeapP_Params *params)
{
    HeapP_Handle     ret_handle; 
    HeapP_freertos   *handle = (HeapP_freertos *) NULL_PTR;
    HeapP_freertos   *heapPool;
    uint32_t         i;
    uintptr_t        key;
    uint32_t         maxHeap;

    DebugP_assert(NULL_PTR != params);
    DebugP_assert(NULL_PTR != params->buf);
    DebugP_assert(0U       != params->size);
    DebugP_assert(0U       == (params->align & (params->align - 1U)));

    /* Pick up the internal static memory block */
    heapPool       = (HeapP_freertos *) &gOsalHeapPFreeRtosPool[0];
    maxHeap        = OSAL_FREERTOS_CONFIGNUM_HEAP;
    
    if(0U == gOsalHeapAllocCnt) 
    {
        (void)memset( (void *)gOsalHeapPFreeRtosPool,0,sizeof(gOsalHeapPFreeRtosPool));
    }

    key = HwiP_disable();

     for (i = 0U; i < maxHeap; i++)
     {
         if (BFALSE == heapPool[i].used)
         {
             heapPool[i].used = BTRUE;
             /* Update statistics */
             gOsalHeapAllocCnt++;
             if (gOsalHeapAllocCnt > gOsalHeapPeak)
             {
                 gOsalHeapPeak = gOsalHeapAllocCnt;
             }
             break;
         }
     }
    HwiP_restore(key);

    if (i < maxHeap)
    {
        /* Grab the memory */
        handle = (HeapP_freertos *) &heapPool[i];
    }

    if (NULL_PTR == handle) {
        ret_handle = NULL_PTR;
    }
    else
    {
        vHeapCreateStatic(&handle->heapHndl, params->buf, params->size, params->align);

        ret_handle = ((HeapP_Handle)handle);
    }

    return ret_handle;
}

/*
 *  ======== HeapP_delete ========
 */
HeapP_Status HeapP_delete(HeapP_Handle handle)
{
    uintptr_t   key;
    HeapP_Status ret_val = HeapP_OK;
    HeapP_freertos *heap = (HeapP_freertos *)handle;

    if((NULL_PTR != heap) && (BTRUE == heap->used))
    {
        vTaskSuspendAll();
        vHeapDelete(&heap->heapHndl);
        (void)xTaskResumeAll();

        key = HwiP_disable();
        heap->used = BFALSE;
        /* Found the osal heap object to delete */
        if (0U < gOsalHeapAllocCnt)
        {
            gOsalHeapAllocCnt--;
        }
        HwiP_restore(key);
    }
    else
    {
       ret_val = HeapP_FAILURE;
    }  

    return ret_val;
}

/*
 *  ======== HeapP_alloc ========
 */
void *HeapP_alloc(HeapP_Handle handle, size_t allocSize)
{
    void *ptr = NULL_PTR;
    HeapP_freertos *heap = (HeapP_freertos *)handle;

    if((NULL_PTR != heap) && (BTRUE == heap->used))
    {
        vTaskSuspendAll();
        ptr = pvHeapMalloc(&heap->heapHndl, allocSize);
        (void)xTaskResumeAll();
    }

    return ptr;
}

/*
 *  ======== HeapP_free ========
 */
HeapP_Status HeapP_free(HeapP_Handle handle, void *ptr, uint32_t size)
{
    HeapP_Status ret_val = HeapP_OK;
    HeapP_freertos *heap = (HeapP_freertos *)handle;

    if((NULL_PTR != heap) && (BTRUE == heap->used) && (NULL_PTR != ptr))
    {
        vTaskSuspendAll();
        vHeapFree(&heap->heapHndl, ptr);
        (void)xTaskResumeAll();
    }
    else
    {
       ret_val = HeapP_FAILURE;
    }  

    return ret_val;
}

/*
 *  ======== HeapP_getHeapStats ========
 */
HeapP_Status HeapP_getHeapStats(HeapP_Handle handle, HeapP_MemStats *stats)
{
    HeapP_Status ret_val = HeapP_OK;
    HeapP_freertos *heap = (HeapP_freertos *)handle;
    HeapMemStats_t pHeapStats;

    if((NULL_PTR != heap) && (BTRUE == heap->used))
    {
        vTaskSuspendAll();
        vHeapGetHeapStats(&heap->heapHndl, &pHeapStats);
        (void)xTaskResumeAll();

        stats->totalSize        = (uint32_t)pHeapStats.totalHeapSizeInBytes;
        stats->totalFreeSize    = (uint32_t)pHeapStats.availableHeapSpaceInBytes;
        stats->largestFreeSize  = (uint32_t)pHeapStats.sizeOfLargestFreeBlockInBytes;
    }
    else
    {
       ret_val = HeapP_FAILURE;
    }

    return ret_val;
}

