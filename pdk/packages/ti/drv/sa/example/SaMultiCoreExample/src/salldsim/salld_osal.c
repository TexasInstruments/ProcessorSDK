/**
 *   @file  salldosal.c
 *
 *   @brief   
 *      This is the OS abstraction layer and is used by the the SA LLD.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009-2013, Texas Instruments, Inc.
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

#ifdef USE_BIOS
/* CSL CHIP, SEM Functional layer includes */
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_semAux.h>
#endif

/**********************************************************************
 ****************************** Defines *******************************
 **********************************************************************/

#define SALLD_HW_SEM     1

#ifdef USE_BIOS
/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
static uint32_t salldMallocCounter = 0;
static uint32_t salldFreeCounter   = 0;
static uint32_t    rmMallocCounter         =   0;
static uint32_t    rmFreeCounter           =   0;
/**********************************************************************
 ******************************* Macros *******************************
 **********************************************************************/

/**********************************************************************
 *************************** OSAL Functions **************************
 **********************************************************************/
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
Ptr salld_sim_malloc (uint32_t num_bytes, int alignmnet)
{
	Error_Block	errorBlock;

    /* Increment the allocation counter. */
    salldMallocCounter++;	

	/* Allocate memory. */
    return (Memory_alloc (NULL, num_bytes, alignmnet, &errorBlock));
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
    Memory_free (NULL, ptr, size);
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
    #ifndef ARM11
    
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();

    /* Cleanup the prefetch buffer also. */
    CSL_XMC_invalidatePrefetchBuffer();

    SYS_CACHE_INV (addr, size, CACHE_FENCE_WAIT);
    
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");

    /* Reenable Interrupts. */
    Hwi_restore(key);

    #endif     
        
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
    
    #ifndef ARM11
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();

    SYS_CACHE_WB (addr, size, CACHE_FENCE_WAIT);
    
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");

    /* Reenable Interrupts. */
    Hwi_restore(key);
    #endif     
    
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
    #ifndef ARM11
    
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();

    /* Cleanup the prefetch buffer also. */
    CSL_XMC_invalidatePrefetchBuffer();

    SYS_CACHE_INV (addr, size, CACHE_FENCE_WAIT);

    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    
    /* Reenable Interrupts. */
    Hwi_restore(key);

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
    #ifndef ARM11
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();

    SYS_CACHE_WB (addr, size, CACHE_FENCE_WAIT);

    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    
    /* Reenable Interrupts. */
    Hwi_restore(key);
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
#ifdef USE_BIOS
    /* Get the hardware semaphore.
     *
     * Acquire Multi core SA synchronization lock
     */
     while ((CSL_semAcquireDirect (SA_HW_SEM)) == 0);
     *key = 0;
#else
    #ifndef ARM11
    *key = (uint32_t)thwCriticalBeginFast();
    #else
    *key = 0x6000;
    #endif
#endif    

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
#ifdef USE_BIOS
    /* Release the hardware semaphore */
    CSL_semReleaseSemaphore (SA_HW_SEM);
#else
  #ifndef ARM11 
  thwCriticalEndFast(key);
  #endif
#endif  

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
    return (uint16_t)CSL_chipReadReg(CSL_CHIP_DNUM);
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
    int endian;
    uint32_t number = 0x1;
    uint8_t *numPtr = (uint8_t*)&number;
    if ((numPtr[0] == 1))
    {
       endian =((int)sa_SYS_ENDIAN_MODE_LITTLE);
    }
    else
    {
       endian =((int)sa_SYS_ENDIAN_MODE_BIG);
    }
    return (endian);
}

/**
 *  @b Description
 *  @n
 *      The function is used to allocate a memory block of the specified size.
 *
 *  @param[in]  num_bytes
 *      Number of bytes to be allocated.
 *
 *  @retval
 *      Allocated block address
 */
void *Osal_rmMalloc (uint32_t num_bytes)
{
	Error_Block	errorBlock;
	void* memBlk;

    /* Increment the allocation counter. */
    rmMallocCounter++;

    memBlk = Memory_alloc(NULL, num_bytes, 0, &errorBlock);

    if (memBlk == 0)
    	while (1); /* trap for no memory allocations */

	/* Allocate memory. */
	return (memBlk);
}

/**
 *  @b Description
 *  @n
 *      The function is used to free a memory block of the specified size.
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
void Osal_rmFree (void *ptr, uint32_t size)
{
    /* Increment the free counter. */
    rmFreeCounter++;
	Memory_free(NULL, ptr, size);
}

/* FUNCTION PURPOSE: Critical section enter
 ***********************************************************************
 * DESCRIPTION: The function is used to enter a critical section.
 *              Function protects against
 *
 *              access from multiple cores
 *              and
 *              access from multiple threads on single core
 */
void *Osal_rmCsEnter(void)
{

    return NULL;
}

/* FUNCTION PURPOSE: Critical section exit
 ***********************************************************************
 * DESCRIPTION: The function is used to exit a critical section
 *              protected using Osal_cppiCsEnter() API.
 */
void Osal_rmCsExit(void *CsHandle)
{

}

/* FUNCTION PURPOSE: Multi-threaded critical section enter
***********************************************************************
* DESCRIPTION: The function is used to enter a multi-threaded critical
*              section. Function protects against 
 *      
 *              access from multiple threads on single core
*/  
void *Osal_rmMtCsEnter(void *mtSemObj)
{

    return NULL;
}

/* FUNCTION PURPOSE: Multi-threaded critical section exit
***********************************************************************
* DESCRIPTION: The function is used to exit a multi-threaded critical
*              section protected using Osal_rmMtCsEnter() API.
*/  
void Osal_rmMtCsExit(void *mtSemObj, void *CsHandle)
{

}

/* FUNCTION PURPOSE: Critical section exit
 ***********************************************************************
 * DESCRIPTION: The function is used to indicate that a block of memory is
 *              about to be accessed. If the memory block is cached then this
 *              indicates that the application would need to ensure that the
 *              cache is updated with the data from the actual memory.
 */
void Osal_rmBeginMemAccess(void *ptr, uint32_t size)
{
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();

    /* Cleanup the prefetch buffer also. */
    CSL_XMC_invalidatePrefetchBuffer();

#ifdef L2_CACHE
    /* Invalidate L2 cache. This should invalidate L1D as well.
     * Wait until operation is complete. */
    CACHE_invL2 (ptr, size, CACHE_FENCE_WAIT);
#else
    /* Invalidate L1D cache and wait until operation is complete.
     * Use this approach if L2 cache is not enabled */
    CACHE_invL1d (ptr, size, CACHE_FENCE_WAIT);
#endif

    /* Reenable Interrupts. */
    Hwi_restore(key);

    return;
}

/* FUNCTION PURPOSE: Critical section exit
 ***********************************************************************
 * DESCRIPTION: The function is used to indicate that the block of memory has
 *              finished being accessed. If the memory block is cached then the
 *              application would need to ensure that the contents of the cache
 *              are updated immediately to the actual memory.
 */
void Osal_rmEndMemAccess(void *ptr, uint32_t size)
{
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();

#ifdef L2_CACHE
    /* Writeback L2 cache. This should Writeback L1D as well.
     * Wait until operation is complete. */
    CACHE_wbL2 (ptr, size, CACHE_FENCE_WAIT);

#else
    /* Writeback L1D cache and wait until operation is complete.
     * Use this approach if L2 cache is not enabled */
    CACHE_wbL1d (ptr, size, CACHE_FENCE_WAIT);
#endif

    /* Reenable Interrupts. */
    Hwi_restore(key);

    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to create a task blocking object
 *      capable of blocking the task a RM instance is running
 *      within
 *
 *  @retval
 *      Allocated task blocking object
 */
void *Osal_rmTaskBlockCreate(void)
{
    Semaphore_Params semParams;

    Semaphore_Params_init(&semParams);
    return((void *)Semaphore_create(0, &semParams, NULL));
}

/**
 *  @b Description
 *  @n
 *      The function is used to block a task whose context a
 *      RM instance is running within.
 *
 *  @param[in]  handle
 *      Task blocking object handle.
 *
 *  @retval
 *      Not Applicable
 */
void Osal_rmTaskBlock(void *handle)
{
    Semaphore_pend((Semaphore_Handle)handle, BIOS_WAIT_FOREVER);
}

/**
 *  @b Description
 *  @n
 *      The function is used to unblock a task whose context a
 *      RM instance is running within.
 *
 *  @param[in]  handle
 *      Task blocking object handle.
 *
 *  @retval
 *      Not Applicable
 */
void Osal_rmTaskUnblock(void *handle)
{
    Semaphore_post((Semaphore_Handle)handle);
}

/**
 *  @b Description
 *  @n
 *      The function is used to delete a task blocking object
 *      provided to a RM instance
 *
 *  @param[in]  handle
 *      Task blocking object handle.
 *
 *  @retval
 *      Not Applicable
 */
void Osal_rmTaskBlockDelete(void *handle)
{
    Semaphore_delete((Semaphore_Handle *)&handle);
}

/**
 *  @b Description
 *  @n
 *      The function is the RM OSAL Logging API which logs
 *      the messages on the console.
 *
 *  @param[in]  fmt
 *      Formatted String.
 *
 *  @retval
 *      Not Applicable
 */
void Osal_rmLog (char *fmt, ... )
{
    VaList ap;

    va_start(ap, fmt);
    System_vprintf(fmt, ap);
    va_end(ap);
}
#endif /* USE_BIOS */


