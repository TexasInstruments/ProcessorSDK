/**
 *   @file  salldosal.c
 *
 *   @brief
 *      This is the OS abstraction layer and is used by the the SA LLD.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009-2020, Texas Instruments, Inc.
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
 *
*/
#include "../unittest.h"
#include <ti/drv/sa/sa_osal.h>

/* CSL CHIP, SEM Functional layer includes */
#if defined (BUILD_DSP)
#include <ti/csl/csl_chip.h>
#endif

#ifndef NSS_LITE2
#include <ti/csl/csl_semAux.h>
#endif

/**********************************************************************
 ****************************** Defines *******************************
 **********************************************************************/

#define SALLD_HW_SEM     1

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
uint32_t salldMallocCounter = 0;
uint32_t salldFreeCounter   = 0;

/**********************************************************************
 ******************************* Macros *******************************
 **********************************************************************/

/**********************************************************************
 *************************** OSAL Functions **************************
 **********************************************************************/

uint32_t size_malloced = 0;

#if defined(SAU_PRMOTE_DEMOTE_TEST)
#define OSAL_MALLOC_BLK_SIZE    (0x1000)
#else
#define OSAL_MALLOC_BLK_SIZE    (0x10000)
#endif

/* R5F tool chain fails to do malloc() and hence
 * providing a static block for memory requirements
 */
#if ((__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R')) || defined(SAU_PRMOTE_DEMOTE_TEST)
#define OSAL_MALLOC_STATIC
#else
#undef  OSAL_MALLOC_STATIC
#endif

#if defined(OSAL_MALLOC_STATIC)
uint8_t  osal_malloc_block[OSAL_MALLOC_BLK_SIZE] __attribute__ ((aligned (128))); /* 200 KB memory reserved */
#endif

static void CacheP_fenceCpu2Dma(uintptr_t addr, uint32_t size, Osal_CacheP_isCoherent isCoherent)
{
    /* CPU to DMA would be to do Wb call if it is not coherent */
    if (OSAL_CACHEP_NOT_COHERENT == isCoherent)
    {
        CacheP_wb( (const void *)addr, size);
    }
}

static void CacheP_fenceDma2Cpu(uintptr_t addr, uint32_t size, Osal_CacheP_isCoherent isCoherent)
{
    /* DMA to CPU would be to do Cache inv call if it is not coherent */
    if (OSAL_CACHEP_NOT_COHERENT == isCoherent)
    {
        CacheP_Inv( (const void *)addr, size);
    }
}

void *Osal_malloc (uint32_t size)
{
    void *result;
    char  msg_malloc[80] = "WARNING: malloc overrun heap!!!\n";
#if defined(OSAL_MALLOC_STATIC)
    uint32_t index = size_malloced;
#endif
    size_malloced += size;

    if (size_malloced >= OSAL_MALLOC_BLK_SIZE) {
      Sa_osalLog (msg_malloc);
      result = (void *) NULL;
    }
    else
    {
#if defined(OSAL_MALLOC_STATIC)
        result = &osal_malloc_block[index];
#else
        result = malloc(size);
#endif
    }
    //Osal_Log ("Osal_malloc(%d) @ 0x%08x (sum=%x)\n", size, result, size_malloced);
    return result;
}

void *Osal_Malloc
(
    uint32_t region,
    uint32_t size,
    uint32_t align,
    uint32_t fWordAccess
)
{
   size += align;
   void *result = Osal_malloc(size);
   if (result)
   {
       uintptr_t *aligned = (uintptr_t *)((uintptr_t)result & ~(align - 1));
       if ((uintptr_t)aligned != (uintptr_t)result)
       {
           aligned = (uintptr_t *)((uintptr_t)aligned + align);
           size_malloced += ((uintptr_t) aligned - (uintptr_t) result);
           result = (void *)aligned;
       }
   }
   return result;
}


/**
 *  @b Description
 *  @n
 *      The function is used to allocate a memory block of the specified size.
 *
 *      Note: If the LLD is used by applications on multiple core, the "salldHeap"
 *      should be in shared memory
 *
 *  @param[in]  num_bytes
 *      Number of bytes to be allocated.
 *
 *  @retval
 *      Allocated block address
 */
/* Macro to align x to y */
#define align(x,y)   ((x + y) & (~y))

uintptr_t fw_mem_alloc_ptr = (uintptr_t) 0x80030000UL;
uintptr_t fw_mem_end       = (uintptr_t) 0x80040000UL;
uintptr_t salld_sim_malloc (uint32_t size, int alignment)
{
#ifdef USE_BIOS
  uintptr_t ret_addr;
  Error_Block	errorBlock;

  /* Increment the allocation counter. */
  salldMallocCounter++;

  /* Allocate memory. */
  ret_addr = (uintptr_t)Memory_alloc (NULL, size, alignment, &errorBlock);
  if (ret_addr == (uintptr_t) NULL)
  {
    salld_sim_halt();
  }
  return (ret_addr);

#else
    uintptr_t  ptr;
    ptr = (uintptr_t)Osal_Malloc((uint32_t)0U, size, (uint32_t)alignment, (uint32_t) 0U);
    if (ptr == (uintptr_t) NULL)
    {
       SALog ("\n\n ------- Osal Malloc error ---------\n");
       System_flush();
       salld_sim_halt();
    }
    return(ptr);
#endif
}

/**
 *  @b Description
 *  @n
 *      The function is used to free a memory block of the specified size allocated
 *      using Osal_saMalloc() API.
 *
 *  @param[in]  ptr
 *      Pointer to the memory block to be cleaned up.
 *
 *  @param[in]  size
 *      Size of the memory block to be cleaned up.
 *
 *  @retval
 *      Not Applicable
 */
void salld_sim_free (Ptr ptr, uint32_t size)
{
    /* Increment the free counter. */
    salldFreeCounter++;
#ifdef USE_BIOS
    Memory_free (NULL, ptr, size);
#else
#if defined(OSAL_MALLOC_STATIC)
#else
    free(ptr);
#endif
#endif
}

/**
 * @brief   The macro is used by the SA LLD to indicate that a block
 * of memory is about to be accessed. If the memory block is cached then
 * this indicates that the application would need to ensure that the cache
 * is updated with the data from the actual memory.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
        void Osal_saBeginMemAccess (void* addr, uint32_t sizeWords)
    @endverbatim
 *
 *  <b> Parameters </b>
 *  @n  Address of memory block.
 *  @n  Size of memory block.
 */
void Osal_saBeginMemAccess (void* addr, uint32_t size)
{
    uint32_t    key;
    /* Disable Interrupts */
    key = HwiP_disable();
#if defined (TEST_CORE_CACHE_COHERENT)
    CacheP_fenceDma2Cpu((uintptr_t) addr, size, OSAL_CACHEP_COHERENT);
#else
    CacheP_fenceDma2Cpu((uintptr_t) addr, size, OSAL_CACHEP_NOT_COHERENT);
#endif
    /* Reenable Interrupts. */
    HwiP_restore(key);
}
/**
 * @brief   The macro is used by the SA LLD to indicate that the block of
 * memory has finished being updated. If the memory block is cached then the
 * application would need to ensure that the contents of the cache are updated
 * immediately to the actual memory.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
        void Osal_saEndMemAccess (void* addr, uint32_t sizeWords)
    @endverbatim
 *
 *  <b> Parameters </b>
 *  @n  Address of memory block.
 *  @n  Size of memory block.
 */


void Osal_saEndMemAccess   (void* addr, uint32_t size)
{
    uint32_t    key;
    /* Disable Interrupts */
    key = HwiP_disable();
#if defined (TEST_CORE_CACHE_COHERENT)
    CacheP_fenceCpu2Dma((uintptr_t) addr, size, OSAL_CACHEP_COHERENT);
#else
    CacheP_fenceCpu2Dma((uintptr_t) addr, size, OSAL_CACHEP_NOT_COHERENT);
#endif
    /* Reenable Interrupts. */
    HwiP_restore(key);
}

/**
 * @brief   The macro is used by the SA LLD to indicate that the security
 * context byuffer is about to be accessed. If the security context buffer
 * is cacheable then this indicates that the application would need to ensure
 * that the cache is updated with the data from the actual memory since the
 * security context will be updated by SASS Cache engine.
 * If the security context buffers are non-cacheable then these macros can
 * be defined to be NOP.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
        void Osal_saBeginScAccess (void* addr, uint32_t sizeWords)
    @endverbatim
 *
 *  <b> Parameters </b>
 *  @n  Address of memory block.
 *  @n  Size of memory block.
 */

void Osal_saBeginScAccess (void* addr, uint32_t size)
{
#if defined(TEST_CORE_CACHE_COHERENT)
        /* No cache operations are needed */
#else
    uint32_t    key;
    /* Disable Interrupts */
    key = HwiP_disable();
    CacheP_Inv(addr, size);
    /* Reenable Interrupts. */
    HwiP_restore(key);
#endif
}



/**
 * @brief   The macro is used by the SA LLD to indicate that the security
 * context buffer has finished being updated. If the memory block is cacheable
 * then the application would need to ensure that the contents of the cache are
 * updated immediately to the actual memory.
 * If the security context buffers are non-cacheable then these macros can
 * be defined to be NOP.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
        void Osal_saEndScAccess (void* addr, uint32_t sizeWords)
    @endverbatim
 *
 *  <b> Parameters </b>
 *  @n  Address of memory block.
 *  @n  Size of memory block.
 */

void Osal_saEndScAccess   (void* addr, uint32_t size)
{
#if defined (TEST_CORE_CACHE_COHERENT)
        /* No cache operations are needed */
#else
    uint32_t    key;
    /* Disable Interrupts */
    key = HwiP_disable();
    CacheP_wbInv(addr, size);
    /* Reenable Interrupts. */
    HwiP_restore(key);
#endif
}

/**
 *  @b Description
 *  @n
 *      The function is used to enter a critical section.
 *      Function protects against
 *
 *      access from multiple threads on single core
 *      and
 *      access from multiple cores
 *
 *  @param[in]  key
 *      Key used to lock the critical section.
 *
 *  @retval
 *      Not Applicable
 */
void Osal_saMtCsEnter (uint32_t *key)
{
    SemaphoreP_pend (tFramework.tfSaSem, 10000);
}

/**
 *  @b Description
 *  @n
 *      The function is used to exit a critical section
 *      protected using Osal_salldCsEnter() API.
 *
 *  @param[in]  key
 *      Key used to unlock the critical section.
 *
 *  @retval
 *      Not Applicable
 */
void Osal_saMtCsExit (uint32_t key)
{
  SemaphoreP_post (tFramework.tfSaSem);
}

/**
 *  @b Description
 *  @n
 *      The function is the SA LLD OSAL Logging API which logs
 *      the messages on the console.
 *
 *  @param[in]  fmt
 *      Formatted String.
 *
 *  @retval
 *      Not Applicable
 */
void Osal_saLog ( String fmt, ... )
{

}

void* Osal_saGetSCPhyAddr(void* vaddr)
{
    return vaddr;
}

uint16_t Osal_saGetProcId (void )
{
#ifdef _TMS320C6X
    uint32_t coreNum = CSL_chipReadReg(CSL_CHIP_DNUM);
#else
    uint32_t coreNum = 0;
#endif
    return (uint16_t)coreNum;
}


/**
 * @brief   The macro is used by the SA LLD to the Endian mode of the system (SoC).
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
 *      int Osal_saGetSysEndianMode(void)
 *   @endverbatim
 *
 *  <b> Return Value Endian mode of the system (SoC) </b>
 *  <b> Parameters </b>
 *  @n  Endian mode of the system (SoC).
 */
int   Osal_saGetSysEndianMode(void)
{
   uint32_t val[1];
   uint8_t  *pVal = (uint8_t *) val;
   int endian;
   val[0] = (uint32_t) 0x12345678;
   if (*pVal == 0x12)
   {
        endian = (int)sa_SYS_ENDIAN_MODE_BIG;
   }
   else
   {
        endian = (int)sa_SYS_ENDIAN_MODE_LITTLE;
   }
   return (endian);
}



