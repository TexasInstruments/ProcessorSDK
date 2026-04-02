/*
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
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

/* SA Osal functions */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
void Osal_saBeginScAccess (void* addr, uint32_t size)
{
  /* stub function */
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
  /* stub function */
}

void* Osal_saGetSCPhyAddr(void* vaddr)
{
    /* Stub Function. TBD: Would need to handle when for multi proc access 
     * To be handled using infrastructure available from Kernel
     */
  return (NULL);
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
    /*
     * Need to extract SoC Endian mode from sytem call or read from DEVSTAT or BOOT_REG0 as defined at cslr_bootcfg.h.
     */
    return(0);
}

void Osal_saBeginMemAccess (void* addr, uint32_t size)
{
    /* Stub Function. TBD: Would need to handle when for multi proc access 
     * To be handled using infrastructure available from Kernel
     */
}

void Osal_saEndMemAccess (void* addr, uint32_t size)
{
    /* Stub Function. TBD: Would need to handle when for multi proc access 
     * To be handled using infrastructure available from Kernel
     */
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
uint8_t buf[10][1024];
int     cnt = 0;
void* salld_sim_malloc (uint32_t num_bytes)
{
	/* Allocate memory. */
	if (cnt >= 10)
		return (NULL);

	if (num_bytes <= 1024)
		return (buf[cnt++]);
	else
		return (NULL);
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
void salld_sim_free (void* ptr, uint32_t size)
{

}

/**
 *  @b Description
 *  @n  
 *      The function is used to enter a critical section.
 *      Function protects against 
 *      
 *      access from multiple cores 
 *      and 
 *      access from multiple threads on single core
 *
 *  @param[in]  key
 *      Key used to lock the critical section.
 *
 *  @retval
 *      Not Applicable
 */
void Osal_saCsEnter (uint32_t *key)
{
    /* Stub Function. TBD */
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
void Osal_saCsExit (uint32_t key)
{
    /* Stub Function. TBD */
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
    /* Stub Function. TBD: Would need to handle when for multi proc access 
     * To be handled using infrastructure available from Kernel
     */
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
    /* Stub Function. TBD: Would need to handle when for multi proc access 
     * To be handled using infrastructure available from Kernel
     */
}

/* Nothing past this point */
