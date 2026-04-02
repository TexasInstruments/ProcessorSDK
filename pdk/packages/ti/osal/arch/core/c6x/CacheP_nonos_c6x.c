/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
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
 *  ======== CacheP_nonos_c6x.c ========
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/CacheP.h>
#include <ti/osal/src/nonos/Nonos_config.h>

#define MAR                     ((volatile uint32_t *) (SOC_DSP_ICFG_BASE + DSP_MAR(0U)))

/* Max word count per cache operations */
#define MAXWC                   (0xFF00U)

/* Following parameter CACHEP_ATOMIC_BLOCK_SIZE is used to break up
 * large blocks into multiple small blocks which are done atomically.
 * Each block of the specified size waits for the cache operation to
 * finish before starting the next block.
 * Setting this size to 0, means the cache operations are not done atomically.
 */
/* #define CACHEP_ATOMIC_BLOCK_SIZE    (CACHE_L2_LINESIZE) */
#define CACHEP_ATOMIC_BLOCK_SIZE    (0U)

/* Defines the prototype of the CSL Cache Ops Function */
typedef void (* CacheP_Function_t)( const void* blockPtr, uint32_t byteCnt, CACHE_Wait wait);

static void CacheP_block(const void * addr, uint32_t size, CacheP_Function_t fxnPtr);

void CacheP_wb(const void * addr, uint32_t size)
{
    /*
     * Writes back the range of memory within the specified starting address
     * and byte count.  The range of addresses operated on gets quantized to
     * whole cache lines in each cache.  There is no effect on L1P cache.
     * All cache lines within the range are left valid in L1D cache and the data
     * within the range in L1D cache will be written back to L2 or external.
     * All cache lines within the range are left valid in L2 cache and the data
     * within the range in L2 cache will be written back to external
     */
    CacheP_block(addr, size, (CacheP_Function_t)CACHE_wbL2);
}

void CacheP_wbInv(const void * addr, uint32_t size)
{
    /*
     * Writes back and invalidates the range of memory within the specified
     * starting address and byte count. The range of addresses operated on gets
     * quantized to whole cache lines in each cache. All cache lines within range
     * are invalidated in L1P cache. All cache lines within the range are
     * written back to L2 or external and then invalidated in L1D cache
     * All cache lines within the range are written back to external and then
     * invalidated in L2 cache.
     */
    CacheP_block(addr, size, (CacheP_Function_t)CACHE_wbInvL2);
}

void CacheP_Inv(const void * addr, uint32_t size)
{
    /*
     * Invalidate the range of memory within the specified starting address and
     * byte count. The range of addresses operated on gets quantized to whole
     * cache lines in each cache. All cache lines in range are invalidated in L1P
     * cache. All cache lines in range are invalidated in L1D cache.
     * All cache lines in range are invalidated in L2 cache
     */
    CacheP_block(addr, size, (CacheP_Function_t)CACHE_invL2);
}

void CacheP_setMar(void *baseAddr, uint32_t size, uint32_t value)
{
    uint32_t maxAddr;
    uint32_t firstMar, lastMar;
    uint32_t marNum;
    volatile uint32_t *marBase = (volatile uint32_t *)MAR;

    /* calculate the maximum address */
    maxAddr = ((uint32_t) baseAddr) + (size - 1U);

    /* range of MAR's that need to be modified */
    firstMar = ((uint32_t) baseAddr) >> 24U;
    lastMar  = ((uint32_t) maxAddr)  >> 24U;

    /* write back invalidate all cached entries */
    CACHE_wbInvAllL2(CACHE_WAIT);

    /* loop through the number of MAR registers affecting the address range */
    for(marNum = firstMar; marNum <= lastMar; marNum++)
    {
        /* set the MAR registers to the specified value */
        marBase[marNum] = value;
    }
}

uint32_t CacheP_getMar(uintptr_t baseAddr)
{
    uint32_t marNum;
    volatile uint32_t *marBase = (volatile uint32_t *)MAR;

    /* MAR register number can be obtained by right shifting the
     * base address by 24 bits (upper 8 bits correspond to the MAR number) */
    marNum = ((uint32_t) baseAddr) >> 24U;

    /* return the value of the MAR register */
    return (marBase[marNum]);

}

static void CacheP_block(const void * addr, uint32_t size, CacheP_Function_t fxnPtr)
{
    uintptr_t alignedAddr     = (uintptr_t)addr & ~((uintptr_t)0x3U);
    uint32_t  alignedSize     = size + (uint32_t)((uintptr_t)addr - (uintptr_t)alignedAddr);
    uintptr_t block_addr      = alignedAddr;
    uint32_t  size_remaining  = alignedSize;
    uint32_t  bytes_count;
    uint32_t  incCnt, currSize;

    /* determine the increment count */
    if(0U != CACHEP_ATOMIC_BLOCK_SIZE)
    {
        incCnt = CACHEP_ATOMIC_BLOCK_SIZE;
    }
    else
    {
        /* convert words to bytes */
        incCnt = MAXWC * sizeof(uint32_t);
    }

    for (currSize = 0U; currSize < size_remaining; currSize += incCnt)
    {
        bytes_count = (size_remaining > incCnt )? incCnt: size_remaining;

        fxnPtr((void *)block_addr, bytes_count, CACHE_WAIT);

        block_addr     += incCnt;
    }

}
