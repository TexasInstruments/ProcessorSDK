/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
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
 *  ======== MemoryP_freertos.c ========
 */
#include <ti/osal/MemoryP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <FreeRTOS.h>
#include <portable.h>

typedef struct MemoryTrack_s
{
    uint32_t allocKey;
    void *origAllocAddress;
    uint32_t origAllocSize;
} MemoryTrack_t __attribute__ ((aligned(portBYTE_ALIGNMENT)));

/*
 *  ======== MemoryP_ctrlAlloc ========
 */
void* MemoryP_ctrlAlloc(uint32_t size, uint8_t alignment)
{
    /* Allocate memory from the default heap for the control memory:
     * One possible use case is to use seperate heaps for the data
     * and control operations. */

    void *allocAddress;
    void *origAllocAddress;
    MemoryTrack_t *memoryTrackAddress;
    uintptr_t offset;
    uint8_t   heapAlignment = alignment;
    uint32_t alignedSize;
    
    if (heapAlignment < (uint8_t)portBYTE_ALIGNMENT)
    {
        /* If alignment not requested by application set to minimum alignment. */
        heapAlignment = portBYTE_ALIGNMENT;
    }
    /* Assert that requested align is a power of 2 */
    DebugP_assert(0U == (heapAlignment & (heapAlignment - 1U)));

    /* Assert that requested block size is non-zero */
    DebugP_assert(0U != size);

    alignedSize = size + heapAlignment;
    origAllocAddress = pvPortMalloc((size_t)(alignedSize + sizeof(MemoryTrack_t)));
    offset = (uintptr_t)origAllocAddress & ((uintptr_t)heapAlignment - 1U);
    allocAddress = (void *)((uintptr_t)origAllocAddress - offset + (uintptr_t)heapAlignment);
    if (((uintptr_t)allocAddress - (uintptr_t)origAllocAddress) < sizeof(MemoryTrack_t))
    {
        /* We cant add the mem track address before the alloc Address.
         * Move the alloc address by alignment value.
         * This happens when the original alloc address is already aligned.
         */
        allocAddress = (void *)((uintptr_t)allocAddress + (uintptr_t)heapAlignment);
        if ((heapAlignment < sizeof(MemoryTrack_t)) &&
            (((uintptr_t)allocAddress - (uintptr_t)origAllocAddress) < sizeof(MemoryTrack_t)))
        {
            /* if alignment value is less than MemoryTrack_t,
             * the alloc address might need to be updated again.
             * to accomodate the MemoryTrack_t before the address passed to app.
             */
            allocAddress = (void *)((uintptr_t)allocAddress + (uintptr_t)heapAlignment);
        }
    }
    memoryTrackAddress = (MemoryTrack_t *)((uintptr_t)allocAddress - sizeof(MemoryTrack_t));
    DebugP_assert((void *)memoryTrackAddress >= origAllocAddress);
    memoryTrackAddress->allocKey = 0xDEADBEEFU;
    memoryTrackAddress->origAllocAddress = origAllocAddress;
    memoryTrackAddress->origAllocSize = size + (uint32_t)heapAlignment + (uint32_t)sizeof(MemoryTrack_t);
    DebugP_assert(  0U == ((uintptr_t)allocAddress & ((uintptr_t)heapAlignment - 1U)) );
    DebugP_assert(((uintptr_t)allocAddress + size) <= ((uintptr_t)memoryTrackAddress->origAllocAddress + memoryTrackAddress->origAllocSize));
    return allocAddress;
}

/*
 *  ======== MemoryP_ctrlFree ========
 */
void MemoryP_ctrlFree(void* ptr, uint32_t size)
{
    void *origAllocAddress;
    MemoryTrack_t *memoryTrackAddress;

    memoryTrackAddress = (MemoryTrack_t *)((uintptr_t)ptr - sizeof(MemoryTrack_t));
    DebugP_assert(0xDEADBEEFU == memoryTrackAddress->allocKey);

    origAllocAddress = memoryTrackAddress->origAllocAddress;
    vPortFree(origAllocAddress);
}

/*
 *  ======== MemoryP_dataAlloc ========
 */
void* MemoryP_dataAlloc(uint32_t size, uint8_t alignment)
{
    /* FREE RTOS uses the same memory for ctrl and data heaps. */
    return MemoryP_ctrlAlloc (size, alignment);
}

/*
 *  ======== MemoryP_dataFree ========
 */
void MemoryP_dataFree(void* ptr, uint32_t size)
{
    /* Free the memory to the appropriate heap: */
    MemoryP_ctrlFree(ptr, size);
}

/*
 *  ======== MemoryP_getStats ========
 */
void MemoryP_getStats(MemoryP_Stats *stats)
{
    HeapStats_t memStats;

    vPortGetHeapStats(&memStats);
    // stats->totalSize = (uint32_t)memStats.totalSize;
    stats->totalFreeSize = (uint32_t)memStats.xAvailableHeapSpaceInBytes;
}

