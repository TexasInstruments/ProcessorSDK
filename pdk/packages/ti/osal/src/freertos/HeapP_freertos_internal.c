/*
 * FreeRTOS Kernel V10.4.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */
/*
 *  Copyright (C) 2018-2025 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * A sample implementation of heap based on heap_4.c from FreeRTOS 
 * 
 * This implementation extends to support multiple heap instances using arbitrary 
 * memory blocks given by users.
 *
 * Mainly it convert the global variables into a "instance" struct
 * and allows users to created multiple heap instances with user specified
 * heap buffer address and size
 * 
 * See heap_4.c and the
 * memory management pages of https://www.FreeRTOS.org for more information.
 */
#include <stdlib.h>
#include <ti/osal/src/freertos/HeapP_freertos_internal.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/HeapP.h>

/* Assumes 8bit bytes! */
#define heapBITS_PER_BYTE         ( ( size_t ) 8 )

/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void prvInsertBlockIntoFreeList( StaticHeap_t *heap, HeapBlockLink_t * pxBlockToInsert );

/*
 * Called automatically to setup the required heap structures the first time
 * pvHeapMalloc() is called.
 */
static void prvHeapInit( StaticHeap_t *heap );


void vHeapCreateStatic( StaticHeap_t *heap, void *pvHeap, size_t xTotalHeapSize, uint32_t align )
{

    heap->pxEnd = NULL;
    heap->pvHeap = pvHeap;
    heap->xTotalHeapSize = xTotalHeapSize;
    heap->align = align;

    /* The size of the structure placed at the beginning of each allocated memory
     * block must by correctly byte aligned. */
    heap->xHeapStructSize = ( sizeof( HeapBlockLink_t ) + ( (size_t)align - (size_t)1U ) ) & ~( (size_t)align - (size_t)1U );

    prvHeapInit(heap);
}

void vHeapDelete(StaticHeap_t *heap)
{
    /* nothing to do */
}

void * pvHeapMalloc( StaticHeap_t *heap, size_t xWantedSize )
{
    HeapBlockLink_t * pxBlock, * pxPreviousBlock, * pxNewBlockLink;
    void * pvReturn = NULL;
    size_t xMallocSize = xWantedSize;
    uint32_t align_mask = heap->align - 1U;
    size_t xHeapStructSize = heap->xHeapStructSize;
    {
        /* Check the requested block size is not so large that the top bit is
         * set.  The top bit of the block size member of the HeapBlockLink_t structure
         * is used to determine who owns the block - the application or the
         * kernel, so it must be free. */
        if( 0UL ==  ( xMallocSize & heap->xBlockAllocatedBit ) )
        {
            /* The wanted size is increased so it can contain a HeapBlockLink_t
             * structure in addition to the requested amount of bytes. */
            if( xMallocSize > ( size_t )0 )
            {
                xMallocSize += xHeapStructSize;

                /* Ensure that blocks are always aligned to the required number
                 * of bytes. */
                if( ( size_t )0x00 != ( xMallocSize & align_mask ) )
                {
                    /* Byte alignment required. */
                    xMallocSize += ( heap->align - ( xMallocSize & align_mask ) );
                    DebugP_assert( ( size_t )0 == ( xMallocSize & align_mask ) );
                }
            }

            if( ( ( size_t )0 < xMallocSize ) && ( xMallocSize <= heap->xFreeBytesRemaining ) )
            {
                /* Traverse the list from the start	(lowest address) block until
                 * one	of adequate size is found. */
                pxPreviousBlock = &heap->xStart;
                pxBlock = heap->xStart.pxNextFreeBlock;

                while( ( pxBlock->xBlockSize < xMallocSize ) && ( NULL != pxBlock->pxNextFreeBlock ) )
                {
                    pxPreviousBlock = pxBlock;
                    pxBlock = pxBlock->pxNextFreeBlock;
                }

                /* If the end marker was reached then a block of adequate size
                 * was	not found. */
                if( pxBlock != heap->pxEnd )
                {
                    /* Block sizes must not get too small. */
                    size_t heap_minimum_block_size = ( ( size_t ) ( xHeapStructSize << 1 ) );

                    /* Return the memory space pointed to - jumping over the
                     * HeapBlockLink_t structure at its start. */
                    pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->pxNextFreeBlock ) + xHeapStructSize );

                    /* This block is being returned for use so must be taken out
                     * of the list of free blocks. */
                    pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

                    /* If the block is larger than required it can be split into
                     * two. */
                    if( ( pxBlock->xBlockSize - xMallocSize ) > heap_minimum_block_size )
                    {
                        /* This block is to be split into two.  Create a new
                         * block following the number of bytes requested. The void
                         * cast is used to prevent byte alignment warnings from the
                         * compiler. */
                        pxNewBlockLink = ( HeapBlockLink_t * ) ( ( ( uint8_t * ) pxBlock ) + xMallocSize );
                        DebugP_assert( 0U == ( ( ( size_t ) pxNewBlockLink ) & (align_mask ) ) );

                        /* Calculate the sizes of two blocks split from the
                         * single block. */
                        pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xMallocSize;
                        pxBlock->xBlockSize = xMallocSize;

                        /* Insert the new block into the list of free blocks. */
                        prvInsertBlockIntoFreeList( heap, pxNewBlockLink );
                    }

                    heap->xFreeBytesRemaining -= pxBlock->xBlockSize;

                    if( heap->xFreeBytesRemaining < heap->xMinimumEverFreeBytesRemaining )
                    {
                        heap->xMinimumEverFreeBytesRemaining = heap->xFreeBytesRemaining;
                    }

                    /* The block is being returned - it is allocated and owned
                     * by the application and has no "next" block. */
                    pxBlock->xBlockSize |= heap->xBlockAllocatedBit;
                    pxBlock->pxNextFreeBlock = NULL;
                    heap->xNumberOfSuccessfulAllocations++;
                }
            }
        }
    }

    DebugP_assert( 0U == ( ( ( size_t ) pvReturn ) & ( size_t ) align_mask ) );
    return pvReturn;
}

void vHeapFree( StaticHeap_t *heap, void * pv )
{
    uint8_t * puc = ( uint8_t * ) pv;
    HeapBlockLink_t * pxLink;
    size_t xHeapStructSize = heap->xHeapStructSize;

    if( NULL != pv )
    {
        /* The memory being freed will have an HeapBlockLink_t structure immediately
         * before it. */
        puc -= xHeapStructSize;

        /* This casting is to keep the compiler from issuing warnings. */
        pxLink = ( HeapBlockLink_t * ) puc;

        /* Check the block is actually allocated. */
        DebugP_assert( ( size_t )0 != ( pxLink->xBlockSize & heap->xBlockAllocatedBit ) );
        DebugP_assert( NULL == pxLink->pxNextFreeBlock );

        if( ( size_t )0 != ( pxLink->xBlockSize & heap->xBlockAllocatedBit ) )
        {
            if( NULL == pxLink->pxNextFreeBlock )
            {
                /* The block is being returned to the heap - it is no longer
                 * allocated. */
                pxLink->xBlockSize &= ~heap->xBlockAllocatedBit;

                {
                    /* Add this block to the list of free blocks. */
                    heap->xFreeBytesRemaining += pxLink->xBlockSize;
                    prvInsertBlockIntoFreeList( heap, ( ( HeapBlockLink_t * ) pxLink ) );
                    heap->xNumberOfSuccessfulFrees++;
                }
            }
        }
    }
}

size_t xHeapGetFreeHeapSize( StaticHeap_t *heap )
{
    return heap->xFreeBytesRemaining;
}

size_t xHeapGetMinimumEverFreeHeapSize( StaticHeap_t *heap )
{
    return heap->xMinimumEverFreeBytesRemaining;
}

static void prvHeapInit( StaticHeap_t *heap )
{
    HeapBlockLink_t * pxFirstFreeBlock;
    uint8_t * pucAlignedHeap;
    size_t uxAddress;
    size_t xTotalHeapSize = heap->xTotalHeapSize;
    uint32_t align_mask = heap->align - 1U;
    size_t xHeapStructSize = heap->xHeapStructSize;

    /* Ensure the heap starts on a correctly aligned boundary. */
    uxAddress = ( size_t ) heap->pvHeap;

    if( ( size_t )0 != ( uxAddress & align_mask ) )
    {
        uxAddress += ( heap->align - ( size_t )1 );
        uxAddress &= ~( ( size_t ) align_mask );
        xTotalHeapSize -= uxAddress - ( size_t ) heap->pvHeap;
    }

    pucAlignedHeap = ( uint8_t * ) uxAddress;

    /* xStart is used to hold a pointer to the first item in the list of free
     * blocks.  The void cast is used to prevent compiler warnings. */
    heap->xStart.pxNextFreeBlock = ( HeapBlockLink_t * ) pucAlignedHeap;
    heap->xStart.xBlockSize = ( size_t ) 0;

    /* pxEnd is used to mark the end of the list of free blocks and is inserted
     * at the end of the heap space. */
    uxAddress = ( ( size_t ) pucAlignedHeap ) + xTotalHeapSize;
    uxAddress -= xHeapStructSize;
    uxAddress &= ~( ( size_t ) align_mask );
    heap->pxEnd = ( HeapBlockLink_t * ) uxAddress;
    heap->pxEnd->xBlockSize = 0;
    heap->pxEnd->pxNextFreeBlock = NULL;

    /* To start with there is a single free block that is sized to take up the
     * entire heap space, minus the space taken by pxEnd. */
    pxFirstFreeBlock = ( HeapBlockLink_t * ) pucAlignedHeap;
    pxFirstFreeBlock->xBlockSize = uxAddress - ( size_t ) pxFirstFreeBlock;
    pxFirstFreeBlock->pxNextFreeBlock = heap->pxEnd;

    /* Only one block exists - and it covers the entire usable heap space. */
    heap->xMinimumEverFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
    heap->xFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;

    /* Work out the position of the top bit in a size_t variable. */
    heap->xBlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * heapBITS_PER_BYTE ) - 1U );
}

static void prvInsertBlockIntoFreeList( StaticHeap_t *heap, HeapBlockLink_t * pxBlockToInsert )
{
    HeapBlockLink_t * pxIterator;
    uint8_t * puc;
    HeapBlockLink_t * pxInsertBlock;
    pxInsertBlock = pxBlockToInsert;

    /* Iterate through the list until a block is found that has a higher address
     * than the block being inserted. */
    for( pxIterator = &heap->xStart; pxIterator->pxNextFreeBlock < pxInsertBlock; pxIterator = pxIterator->pxNextFreeBlock )
    {
        /* Nothing to do here, just iterate to the right position. */
    }

    /* Do the block being inserted, and the block it is being inserted after
     * make a contiguous block of memory? */
    puc = ( uint8_t * ) pxIterator;

    if( ( puc + pxIterator->xBlockSize ) == ( uint8_t * ) pxInsertBlock )
    {
        pxIterator->xBlockSize += pxInsertBlock->xBlockSize;
        pxInsertBlock = pxIterator;
    }

    /* Do the block being inserted, and the block it is being inserted before
     * make a contiguous block of memory? */
    puc = ( uint8_t * ) pxInsertBlock;

    if( ( puc + pxInsertBlock->xBlockSize ) == ( uint8_t * ) pxIterator->pxNextFreeBlock )
    {
        if( pxIterator->pxNextFreeBlock != heap->pxEnd )
        {
            /* Form one big block from the two blocks. */
            pxInsertBlock->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
            pxInsertBlock->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
        }
        else
        {
            pxInsertBlock->pxNextFreeBlock = heap->pxEnd;
        }
    }
    else
    {
        pxInsertBlock->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
    }

    /* If the block being inserted plugged a gab, so was merged with the block
     * before and the block after, then it's pxNextFreeBlock pointer will have
     * already been set, and should not be set here as that would make it point
     * to itself. */
    if( pxIterator != pxInsertBlock )
    {
        pxIterator->pxNextFreeBlock = pxInsertBlock;
    }
}

void vHeapGetHeapStats( StaticHeap_t *heap, HeapMemStats_t * pxHeapStats )
{
    HeapBlockLink_t * pxBlock;
    size_t xBlocks = 0, xMaxSize = 0, xMinSize = 0xFFFFFFFFU;

    {
        pxBlock = heap->xStart.pxNextFreeBlock;

        /* pxBlock will be NULL if the heap has not been initialised.  The heap
         * is initialised automatically when the first allocation is made. */
        if( NULL != pxBlock )
        {
            do
            {
                /* Increment the number of blocks and record the largest block seen
                 * so far. */
                xBlocks++;

                if( pxBlock->xBlockSize > xMaxSize )
                {
                    xMaxSize = pxBlock->xBlockSize;
                }

                if( pxBlock->xBlockSize < xMinSize )
                {
                    xMinSize = pxBlock->xBlockSize;
                }

                /* Move to the next block in the chain until the last block is
                 * reached. */
                pxBlock = pxBlock->pxNextFreeBlock;
            } while( pxBlock != heap->pxEnd );
        }
    }

    pxHeapStats->sizeOfLargestFreeBlockInBytes = xMaxSize;
    pxHeapStats->sizeOfSmallestFreeBlockInBytes = xMinSize;
    pxHeapStats->numberOfFreeBlocks = xBlocks;

    {
        pxHeapStats->availableHeapSpaceInBytes = heap->xFreeBytesRemaining;
        pxHeapStats->numberOfSuccessfulAllocations = heap->xNumberOfSuccessfulAllocations;
        pxHeapStats->numberOfSuccessfulFrees = heap->xNumberOfSuccessfulFrees;
        pxHeapStats->minimumEverFreeBytesRemaining = heap->xMinimumEverFreeBytesRemaining;
        pxHeapStats->totalHeapSizeInBytes = heap->xTotalHeapSize;
    }
}

