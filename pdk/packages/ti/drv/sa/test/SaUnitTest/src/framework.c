/*
 *
 * Copyright (C) 2010-2020 Texas Instruments Incorporated - http://www.ti.com/
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

/* Generate and verify the system test framework
 *
 * The test framework consists of the pa and sa driver instance, a cppi/cdma/qm configuration,
 * memory for packet transmission and reception, and semaphores that are used
 * for every test in the SA unit test.
 *
 */
#include "ti/osal/osal.h"
#include "ti/osal/TaskP.h"

#include "unittest.h"
#include "testconn.h"
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
#include <ti/drv/pa/pa_osal.h>
#endif

#include <ti/csl/cslr_device.h>

#ifdef NSS_LITE2
#if defined (SOC_AM64X)
#include <ti/csl/csl_pktdma.h>
#else
#include <ti/drv/udma/src/udmap/csl_udmap.h>
#endif
#include <ti/board/board.h>
#include <ti/osal/osal.h>
#include <ti/csl/arch/csl_arch.h>
#else
#include <ti/csl/csl_psc.h>
#include <ti/csl/csl_pscAux.h>

/* CSL CHIP, SEM Functional layer includes */
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_semAux.h>

/* QMSS device specific configuration */
extern Qmss_GlobalConfigParams  qmssGblCfgParams;
extern Qmss_GlobalConfigParams  qmssNetssGblCfgParams;

/* CPPI device specific configuration */
extern Cppi_GlobalConfigParams  cppiGblCfgParams;
#endif

#ifndef ARM11
#ifdef _TMS320C6X
cregister volatile unsigned int CSR ;
cregister volatile unsigned int IER ;
#endif
#endif

#ifndef USE_RTOS
#ifndef ARM11

#define GIE     (1U<<0)

#ifdef _TMS320C6X
/*****************************************************************************
 * DESCRIPTION: This variable is specific bit-stack implementation. It is
 *            manipulated by thwCriticalBegin() and thwCriticalEnd() functions
 *            in order to properly recover status of INTM bit after
 *            completion of critical section. Up to 16 bits can be shifted in
 *            and out, meaning that 15-level nesting of critical sections
 *            is allowed (highly unlikely).
 *****************************************************************************/
volatile uint16_t thwCriticalStatusSaved = 0;

/******************************************************************************
 * FUNCTION PURPOSE: Start of critical section (up to 15 sections can be nested)
 *
 ******************************************************************************
 * DESCRIPTION:  Save current interrupt enable/disable status, and disable
 *               all interrupts. Bit ST1_INTM is pushed in global variable.
 *               IER0 and IER1 are saved and restored
 *****************************************************************************/
void thwCriticalBegin (void)
{

  uint32_t ierStore;

  /* Disable interrupts by clearing the IER0 */
  ierStore = IER;
  IER = 0;
  /* Disable interrupts by clearing the IER1 */

  if(!(CSR & (GIE))) /* Is global interrupt enabled ? */
  {
    CSR &= ~(GIE);
//    asm("	bit(st1, #st1_intm) = #1"); /* Always disable interrupt on exit */
    asm ("  NOP");
    asm ("  NOP");
    asm ("  NOP");
    thwCriticalStatusSaved <<= 1;  /* Bit stack prepared */
    thwCriticalStatusSaved |= 1; /* It is disabled ! */
  }
  else {                           /* else ... means it is enabled */
    CSR &= ~(GIE);
  //  asm("	bit(st1, #st1_intm) = #1"); /* Always disable interrupt on exit */
    asm ("  NOP");
    asm ("  NOP");
    asm ("  NOP");
    thwCriticalStatusSaved <<= 1;  /* Bit stack prepared */
  }
  IER = ierStore;
}
/******************************************************************************
 * FUNCTION PURPOSE: End of critical section (up to 15 sections can be nested)
 *
 ******************************************************************************
 * DESCRIPTION: Restore interrupt enable/disable status encountered on entry to
 *              critical section. Bit ST1_INTM is poped from the global variable.
 *****************************************************************************/
void thwCriticalEnd (void)
{
  if(thwCriticalStatusSaved & 0x0001) /* Was the last global interrupt
                                       * disabled ? */
    thwCriticalStatusSaved >>= 1;     /* Drop the last bit saved !
                                       * It was already disabled, prior to
                                       * critical section entry */
  else {
    thwCriticalStatusSaved >>= 1;     /* Drop the last bit saved ! */
    asm ("   NOP");
    asm ("   NOP");
    asm ("   NOP");
//    asm("	bit(st1, #st1_intm) = #0"); /* It was enabled prior to critical
//                                         * section entry */
    CSR |= (GIE);
  }
} /* End of thwCriticalEnd() */
#endif /* #ifdef _TMS320C6X */
#endif /* ARM11 */
#else

/**********************************************************************
 ****************************** Defines *******************************
 **********************************************************************/
#define     MAX_NUM_CORES       8

/* Hardware Semaphore to synchronize access from
 * multiple applications (PA applications and non-PASS applications)
 * across different cores to the QMSS library.
 */
#define     QMSS_HW_SEM         3

/* Hardware Semaphore to synchronize access from
 * multiple applications (PASS applications and non-PASS applications)
 * across different cores to the CPPI library.
 */
#define     CPPI_HW_SEM         4

/* Hardware Semaphore to synchronize access from
 * multiple applications (PASS applications and non-PASS applications)
 * across different cores to the PA library.
 */
#define     PA_HW_SEM           5

/**
 *  @b Description
 *  @n
 *     General Memory Barrier guarantees that all LOAD and STORE operations that were issued before the
 *     barrier occur before the LOAD and STORE operations issued after the barrier
 *
 */
#if defined(__ARM_ARCH_7A__)
void memBarrier(void) {
   __sync_synchronize();
}
#endif

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
uint32_t      paMemProtNestedLevel= 0;
uint32_t      coreKey [MAX_NUM_CORES];

#endif /* USE_RTOS */

uint32_t      qmssMallocCounter   = 0;
uint32_t      qmssFreeCounter     = 0;
uint32_t      cppiMallocCounter   = 0;
uint32_t      cppiFreeCounter     = 0;

/*
 * Netss Local PKTDMA related convert functions
 */

uintptr_t Netss_qmssVirtToPhy (void *ptr)
{
    uintptr_t addr = (uintptr_t)ptr;

#if !defined(USE_RTOS) && !defined(NSS_LITE2)
    {
        if ((addr & 0xFF000000) == CSL_NETCP_CFG_REGS)
        {
            addr = (addr & 0x00FFFFFF) | 0xFF000000;
        }
    }
#endif

    return ((uintptr_t)addr);
}

uintptr_t Netss_qmssPhyToVirt (void *ptr)
{
    uintptr_t addr = (uintptr_t)ptr;

#if !defined(USE_RTOS) && !defined(NSS_LITE2)
    {
        if ((addr & 0xFF000000) == 0xFF000000)
        {
            addr = (addr & 0x00FFFFFF) | CSL_NETCP_CFG_REGS;
        }
    }
#endif

    return ((uintptr_t)addr);
}

#ifndef NSS_LITE2
/**
 * @brief  This macro is used to alert the application that the PA is
 *         going to access table memory. The application must ensure
 *         cache coherency for multi-core applications
 *
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
        void Osal_paBeginMemAccess (void* addr, uint32_t sizeWords)
    @endverbatim
 *
 *  <b> Parameters </b>
 *  @n  The address of the table to be accessed
 *  @n  The number of bytes in the table
 *
 *  @note PA will make nested calls to this function for memory access
 *        protection of different memory tables.
 */

void Osal_paBeginMemAccess (Ptr addr, uint32_t size)
{
#ifdef USE_RTOS
#ifdef _TMS320C6X
    uint32_t    key;

    /* Disable Interrupts */
    key = HwiP_disable();

    /* Cleanup the prefetch buffer also. */
    CSL_XMC_invalidatePrefetchBuffer();

    SYS_CACHE_INV (addr, size, CACHE_FENCE_WAIT);

    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");

    /* Reenable Interrupts. */
    HwiP_restore(key);
#endif
#else
    if ((addr != (Ptr)memL2Ram) && (addr != (Ptr)memL3Ram))
    {
		SALog ("Osal_paBeginMemAccess: Invalid address supplied!\n");
    }

    #ifndef ARM11
    thwCriticalBegin();
    #endif

#endif

}

/**
 * @brief  This macro is used to alert the application that the PA
 *         has completed access to table memory. This call will always
 *         be made following a call to Osal_paBeginMemAccess and have
 *         the same parameters
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
        void Osal_paEndMemAccess (void* addr, uint32_t sizeWords)
    @endverbatim
 *
 *  <b> Parameters </b>
 *  @n The address of the table to be accessed
 *  @n The number of bytes in the table
 *
 *  @note PA will make nested calls to this function for memory access
 *        protection of different memory tables.
 */
void Osal_paEndMemAccess   (Ptr addr, uint32_t size)
{
#ifdef USE_RTOS
#ifdef _TMS320C6X
    uint32_t    key;

    /* Disable Interrupts */
    key = HwiP_disable();

    SYS_CACHE_WB (addr, size, CACHE_FENCE_WAIT);

    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");

    /* Reenable Interrupts. */
    HwiP_restore(key);
#endif
#else
    #ifndef ARM11
    thwCriticalEnd();
    #endif
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
void Osal_paMtCsEnter (uint32_t *key)
{

    /* Get the hardware semaphore.
     *
     * Acquire Multi core PA synchronization lock
     */
     while ((CSL_semAcquireDirect (PA_HW_SEM)) == 0);
     *key = 0;
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
void Osal_paMtCsExit (uint32_t key)
{
    /* Release the hardware semaphore */
    CSL_semReleaseSemaphore (PA_HW_SEM);
}

void* Netss_qmssConvertDescVirtToPhy(uint32_t QID, void *descAddr)
{
    uint32_t addr = (uint32_t) descAddr;

#ifndef USE_RTOS
    {
        if ((addr & 0xFF000000) == CSL_NETCP_CFG_REGS)
        {
            addr = (addr & 0x00FFFFFF) | 0xFF000000;
        }
    }
#else
#ifdef __ARM_ARCH_7A__
    memBarrier();
#endif
#endif

    return ((void *)addr);
}

void* Netss_qmssConvertDescPhyToVirt(uint32_t QID, void *descAddr)
{
    uint32_t addr = (uint32_t) descAddr;

#ifndef USE_RTOS
    {
        if ((addr & 0xFF000000) == 0xFF000000)
        {
            addr = (addr & 0x00FFFFFF) | CSL_NETCP_CFG_REGS;
        }
    }
#else
#ifdef __ARM_ARCH_7A__
    memBarrier();
#endif
#endif

    return ((void *)addr);
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
void* Osal_qmssMalloc (uint32_t num_bytes)
{
#ifdef USE_BIOS
	Error_Block	errorBlock;
#endif
    void* dataPtr;

    /* Increment the allocation counter. */
    qmssMallocCounter++;

	/* Allocate memory. */
#ifdef USE_BIOS
    dataPtr = Memory_alloc(NULL, num_bytes, 0, &errorBlock);
#else
    dataPtr = (void*)Osal_Malloc((uint32_t)0U, num_bytes, 0, (uint32_t) 0U);
#endif
	return (dataPtr);
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
Ptr Osal_qmssCsEnter (void)
{
#ifdef USE_RTOS
#ifdef _TMS320C6X
    uint32_t coreNum = CSL_chipReadReg(CSL_CHIP_DNUM);
#else
    uint32_t coreNum = 0;
#endif
    /* Get the hardware semaphore.
     *
     * Acquire Multi core QMSS synchronization lock
     */
    while ((CSL_semAcquireDirect (QMSS_HW_SEM)) == 0);

    /* Disable all interrupts and OS scheduler.
     *
     * Acquire Multi threaded / process synchronization lock.
     */
    coreKey [coreNum] = HwiP_disable();

    return NULL;
#else
    #ifndef ARM11
    thwCriticalBegin();
    #endif
    return NULL;
#endif
}

/**
 *  @b Description
 *  @n
 *      The function is used to exit a critical section
 *      protected using Osal_qmssCsEnter() API.
 *
 *  @param[in]  key
 *      Key used to unlock the critical section.
 *
 *  @retval
 *      Not Applicable
 */
void Osal_qmssCsExit (Ptr CsHandle)
{
#ifdef USE_RTOS
#ifdef _TMS320C6X
    uint32_t coreNum = CSL_chipReadReg(CSL_CHIP_DNUM);
#else
    uint32_t coreNum = 0;
#endif
    /* Enable all interrupts and enables the OS scheduler back on.
     *
     * Release multi-threaded / multi-process lock on this core.
     */
    HwiP_restore(coreKey [coreNum]);

    /* Release the hardware semaphore
     *
     * Release multi-core lock.
     */
    CSL_semReleaseSemaphore (QMSS_HW_SEM);
#else
    #ifndef ARM11
    thwCriticalEnd();
    #endif
#endif

}

/******************************************************************************
* Function to issue memory barrier
*
* NOTE: QMSS unit tests are not using CPPI descriptors
******************************************************************************/
void* Osal_qmssMemBarrier(uint32_t QID, void *descAddr)
{
#ifdef __ARM_ARCH_7A__
    /* Issue memory barrier */
    memBarrier();
#endif
    return descAddr;
}

/**
 * ============================================================================
 *  @n@b Osal_cppiCsEnter
 *
 *  @b  brief
 *  @n  This API ensures multi-core and multi-threaded
 *      synchronization to the caller.
 *
 *      This is a BLOCKING API.
 *
 *      This API ensures multi-core synchronization between
 *      multiple processes trying to access CPPI shared
 *      library at the same time.
 *
 *  @param[in]
 *  @n  None
 *
 *  @return
 *  @n  Handle used to lock critical section
 * =============================================================================
 */
Ptr Osal_cppiCsEnter (void)
{
#ifdef USE_RTOS
#ifdef _TMS320C6X
    uint32_t coreNum = CSL_chipReadReg(CSL_CHIP_DNUM);
#else
    uint32_t coreNum = 0;
#endif
    /* Get the hardware semaphore.
     *
     * Acquire Multi core CPPI synchronization lock
     */
    while ((CSL_semAcquireDirect (CPPI_HW_SEM)) == 0);

    /* Disable all interrupts and OS scheduler.
     *
     * Acquire Multi threaded / process synchronization lock.
     */
    coreKey [coreNum] = HwiP_disable();

    return NULL;
#else
    #ifndef ARM11
    thwCriticalBegin();
    #endif
    return NULL;
#endif
}

/**
 * ============================================================================
 *  @n@b Osal_cppiCsExit
 *
 *  @b  brief
 *  @n  This API needs to be called to exit a previously
 *      acquired critical section lock using @a Osal_cppiCsEnter ()
 *      API. It resets the multi-core and multi-threaded lock,
 *      enabling another process/core to grab CPPI access.
 *
 *  @param[in]  CsHandle
 *      Handle for unlocking critical section.
 *
 *  @return     None
 * =============================================================================
 */
void Osal_cppiCsExit (Ptr CsHandle)
{
#ifdef USE_RTOS
#ifdef _TMS320C6X
    uint32_t coreNum = CSL_chipReadReg(CSL_CHIP_DNUM);
#else
    uint32_t coreNum = 0;
#endif
    /* Enable all interrupts and enables the OS scheduler back on.
     *
     * Release multi-threaded / multi-process lock on this core.
     */
    HwiP_restore(coreKey [coreNum]);

    /* Release the hardware semaphore
     *
     * Release multi-core lock.
     */
    CSL_semReleaseSemaphore (CPPI_HW_SEM);

    return;
#else
    #ifndef ARM11
    thwCriticalEnd();
    #endif
#endif
}

/**
 *  @b Description
 *  @n
 *      The function is used to allocate a memory block of the specified size.
 *
 *      Note: If the LLD is used by applications on multiple core, the "cppiHeap"
 *      should be in shared memory
 *
 *  @param[in]  num_bytes
 *      Number of bytes to be allocated.
 *
 *  @retval
 *      Allocated block address
 */
void* Osal_cppiMalloc (uint32_t num_bytes)
{
#ifdef USE_BIOS
	Error_Block	errorBlock;
#endif
    void* dataPtr;

    /* Increment the allocation counter. */
    cppiMallocCounter++;

	/* Allocate memory. */
#ifdef USE_BIOS
    dataPtr = Memory_alloc(NULL, num_bytes, 0, &errorBlock);
#else
    dataPtr = (void*)Osal_Malloc((uint32_t)0U, num_bytes, 0, (uint32_t) 0U);
#endif
	return (dataPtr);
}

/**
 *  @b Description
 *  @n
 *      The function is used to free a memory block of the specified size allocated
 *      using Osal_cppiMalloc() API.
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
void Osal_cppiFree (void *ptr, uint32_t size)
{
    /* Increment the free counter. */
    cppiFreeCounter++;
	Memory_free (NULL, ptr, size);
}


/**
 * ============================================================================
 *  @n@b Osal_qmssMtCsEnter
 *
 *  @b  brief
 *  @n  This API ensures ONLY multi-threaded
 *      synchronization to the QMSS user.
 *
 *      This is a BLOCKING API.
 *
 *  @param[in] None
 *
 *  @return
 *       Handle used to lock critical section
 * =============================================================================
 */
Ptr Osal_qmssMtCsEnter (void)
{
#ifdef USE_RTOS
    /* Disable all interrupts and OS scheduler.
     *
     * Acquire Multi threaded / process synchronization lock.
     */
    //coreKey [CSL_chipReadReg(CSL_CHIP_DNUM)] = Hwi_disable();

    return NULL;
#else
    #ifndef ARM11
    thwCriticalBegin();
    #endif
    return NULL;
#endif

}

/**
 * ============================================================================
 *  @n@b Osal_qmssMtCsExit
 *
 *  @b  brief
 *  @n  This API needs to be called to exit a previously
 *      acquired critical section lock using @a Osal_cpswQmssMtCsEnter ()
 *      API. It resets the multi-threaded lock, enabling another process
 *      on the current core to grab it.
 *
 *  @param[in]  CsHandle
 *      Handle for unlocking critical section.
 *
 *  @return     None
 * =============================================================================
 */
void Osal_qmssMtCsExit (Ptr CsHandle)
{
#ifdef USE_RTOS
    /* Enable all interrupts and enables the OS scheduler back on.
     *
     * Release multi-threaded / multi-process lock on this core.
     */
    //Hwi_restore(key);

    return;
#else
    #ifndef ARM11
    thwCriticalEnd();
    #endif
#endif

}

/**
 *  @b Description
 *  @n
 *      The function is used to indicate that a block of memory is
 *      about to be accessed. If the memory block is cached then this
 *      indicates that the application would need to ensure that the
 *      cache is updated with the data from the actual memory.
 *
 *  @param[in]  blockPtr
 *       Address of the block which is to be invalidated
 *
 *  @param[in]  size
 *       Size of the block to be invalidated

 *  @retval
 *      Not Applicable
 */
void Osal_cppiBeginMemAccess (void *blockPtr, uint32_t size)
{
#ifdef _TMS320C6X
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();

    /* Cleanup the prefetch buffer also. */
    CSL_XMC_invalidatePrefetchBuffer();

    SYS_CACHE_INV (blockPtr, size, CACHE_FENCE_WAIT);

    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");

    /* Reenable Interrupts. */
    Hwi_restore(key);
#endif

    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to indicate that the block of memory has
 *      finished being accessed. If the memory block is cached then the
 *      application would need to ensure that the contents of the cache
 *      are updated immediately to the actual memory.
 *
 *  @param[in]  blockPtr
 *       Address of the block which is to be written back
 *
 *  @param[in]  size
 *       Size of the block to be written back

 *  @retval
 *      Not Applicable
 */
void Osal_cppiEndMemAccess (void *blockPtr, uint32_t size)
{
#ifdef _TMS320C6X
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();

    SYS_CACHE_WB (blockPtr, size, CACHE_FENCE_WAIT);

    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");

    /* Reenable Interrupts. */
    Hwi_restore(key);
#endif
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to indicate that a block of memory is
 *      about to be accessed. If the memory block is cached then this
 *      indicates that the application would need to ensure that the
 *      cache is updated with the data from the actual memory.
 *
 *  @param[in]  blockPtr
 *       Address of the block which is to be invalidated
 *
 *  @param[in]  size
 *       Size of the block to be invalidated

 *  @retval
 *      Not Applicable
 */
void Osal_qmssBeginMemAccess (void *blockPtr, uint32_t size)
{
#ifdef _TMS320C6X
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();

    /* Cleanup the prefetch buffer also. */
    CSL_XMC_invalidatePrefetchBuffer();

    SYS_CACHE_INV (blockPtr, size, CACHE_FENCE_WAIT);

    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");

    /* Reenable Interrupts. */
    Hwi_restore(key);
#endif

    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to indicate that the block of memory has
 *      finished being accessed. If the memory block is cached then the
 *      application would need to ensure that the contents of the cache
 *      are updated immediately to the actual memory.
 *
 *  @param[in]  blockPtr
 *       Address of the block which is to be written back
 *
 *  @param[in]  size
 *       Size of the block to be written back

 *  @retval
 *      Not Applicable
 */
void Osal_qmssEndMemAccess (void *blockPtr, uint32_t size)
{
#ifdef _TMS320C6X
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();

    SYS_CACHE_WB (blockPtr, size, CACHE_FENCE_WAIT);

    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");

    /* Reenable Interrupts. */
    Hwi_restore(key);
#endif

    return;
}

int downloadPaFirmware (void)
{
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    int ret = pa_OK, i;
	uint32_t  version;

	Pa_resetControl (tFramework.passHandle, pa_STATE_RESET);

	for ( i = 0; i < nssGblCfgParams.layout.numPaPdsps; i++)
    {

        Pa_downloadImage (tFramework.passHandle, i,
                          (Ptr)nssGblCfgParams.layout.paPdspImage[i],
                          nssGblCfgParams.layout.paPdspImageSize[i]);
    }

	ret = Pa_resetControl (tFramework.passHandle, pa_STATE_ENABLE);

	if (ret != pa_STATE_ENABLE)
	{
	  SALog ("downloadPaFirmware: Pa_resetControl return with error code %d\n", ret);
	  System_flush();
	  return (-1);
	}

	for ( i = 0; i < nssGblCfgParams.layout.numPaPdsps; i++)
	{
	  Pa_getPDSPVersion(tFramework.passHandle, i, &version);
	  SALog ("PDSP %d version = 0x%08x\n", i, version);
	  System_flush();
	}
#endif
	return (0);
}

/* The PA LLD instance is created, the PA firmware is
 * downloaded and started */
int initPa (void)
{
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  paSizeInfo_t  paSize;
  paConfig_t    paCfg;
  paRaConfig_t  raCfg;
  int           ret;
  int           sizes[pa_N_BUFS];
  int           aligns[pa_N_BUFS];
  void*         bases[pa_N_BUFS];

  /* The maximum number of handles that can exists are 32 for L2, and 64 for L3. */
  memset(&paSize, 0, sizeof(paSizeInfo_t));
  memset(&paCfg, 0, sizeof(paConfig_t));
  memset(&raCfg, 0, sizeof(paRaConfig_t));
  memset(sizes, 0, sizeof(sizes));
  memset(aligns, 0, sizeof(aligns));
  memset(bases, 0, pa_N_BUFS*sizeof(void*));
  paSize.nMaxL2 = TF_MAX_NUM_L2_HANDLES;
  paSize.nMaxL3 = TF_MAX_NUM_L3_HANDLES;
  paSize.nMaxVlnk = 0;
  paSize.nUsrStats = 0;
  paSize.nMaxAcl = 0;
  paSize.nMaxFc = 0;
  paSize.nUsrStats = 0;

  ret = Pa_getBufferReq(&paSize, sizes, aligns);

  if (ret != pa_OK)  {
    SALog ("initPa: Pa_getBufferReq() return with error code %d\n", ret);
    return (-1);
  }

  /* The first buffer is used as the instance buffer */
  if ((uint32_t)memPaInst & (aligns[pa_BUF_INST] - 1))  {
    SALog ("initPa: Pa_getBufferReq requires %d alignment for instance buffer, but address is 0x%08x\n", aligns[pa_BUF_INST], (uint32_t)memPaInst);
    return (-1);
  }

  if (sizeof(memPaInst) < sizes[pa_BUF_INST])  {
    SALog ("initPa: Pa_getBufferReq requires size %d for instance buffer, have only %d\n", sizes[pa_BUF_INST], sizeof(memPaInst));
    return (-1);
  }

  bases[pa_BUF_INST] = (void *)memPaInst;


  /* The second buffer is the L2 table */
  if ((uint32_t)memL2Ram & (aligns[pa_BUF_L2_TABLE] - 1))  {
    SALog ("initPa: Pa_getBufferReq requires %d alignment for L2 buffer, but address is 0x%08x\n", aligns[pa_BUF_L2_TABLE], (uint32_t)memL2Ram);
    return (-1);
  }

  if (sizeof(memL2Ram) <  sizes[pa_BUF_L2_TABLE])  {
    SALog ("initPa: Pa_getBufferReq requires %d bytes for buffer L2 buffer, have only %d\n", sizes[pa_BUF_L2_TABLE], sizeof(memL2Ram));
    return (-1);
  }

  bases[pa_BUF_L2_TABLE] = (void *)memL2Ram;

  /* The third buffer is the L3 table */
  if ((uint32_t)memL3Ram & (aligns[pa_BUF_L3_TABLE] - 1))  {
    SALog ("initPa: Pa_getBufferReq requires %d alignment for L3 buffer, but address is 0x%08x\n", aligns[pa_BUF_L3_TABLE], (uint32_t)memL3Ram);
    return (-1);
  }

  if (sizeof(memL3Ram) <  sizes[pa_BUF_L3_TABLE])  {
    SALog ("initPa: Pa_getBufferReq requires %d bytes for L3 buffer, have only %d\n", sizes[pa_BUF_L3_TABLE], sizeof(memL3Ram));
    return (-1);
  }

  bases[pa_BUF_L3_TABLE] = (void *)memL3Ram;

  if (nssGblCfgParams.layout.fNssGen2)
  {
    /* set default RA system configuration */
    raCfg.ipv4MinPktSize      = 28;   /* 20-byte IPv4 header plus 8-byte payload */
    raCfg.numCxts             = 0x400;
    raCfg.cxtDiscardThresh    = 0x400;
    raCfg.nodeDiscardThresh   = 0x1000;
    raCfg.cxtTimeout          = 60000;
    raCfg.clockRate           = 350;
    raCfg.heapRegionThresh    = 0x400;
    raCfg.heapBase[0]         = 0x90000000ULL;

    paCfg.raCfg = &raCfg;
  }

  paCfg.initTable = TRUE;
#ifndef SIMULATOR_SUPPORT
  paCfg.initDefaultRoute = TRUE;
#endif
  paCfg.baseAddr = CSL_NETCP_CFG_REGS;
  paCfg.sizeCfg = &paSize;


  ret = Pa_create (&paCfg, bases, &tFramework.passHandle);
  if (ret != pa_OK)  {
    SALog ("initPa: Pa_create returned with error code %d\n", ret);
    return (-1);
  }

  /* Download the firmware */
  if (downloadPaFirmware ())
    return (-1);
 
 #endif
 
   return (0);

}

int setupQmMem (void)
{
  Qmss_InitCfg     qmssInitConfig;
  Qmss_MemRegInfo  memInfo;
  Cppi_DescCfg     descCfg;
  int32_t          result;
  int              n;

  memset (&qmssInitConfig, 0, sizeof (Qmss_InitCfg));
  memset (memDescRam,      0, sizeof (memDescRam));

  qmssInitConfig.linkingRAM0Base = 0;  /* Use internal linking RAM */
  #if !defined(NSS_LITE) && !defined(NSS_LITE2)
  qmssInitConfig.linkingRAM0Size = 0;
  #else
  qmssInitConfig.linkingRAM0Size = TF_NUM_DESC;  /* 0 */
  #endif
  qmssInitConfig.linkingRAM1Base = 0;
  qmssInitConfig.maxDescNum      = TF_NUM_DESC;

  result = Qmss_init (&qmssInitConfig, &qmssGblCfgParams);
  if (result != QMSS_SOK)  {
    SALog ("setupQmMem: qmss_Init failed with error code %d\n", result);
    return (-1);
  }

  result = Qmss_start();
  if (result != QMSS_SOK)  {
    SALog ("setupQmMem: Qmss_start failed with error code %d\n", result);
    return (-1);
  }

  /* Setup a single memory region for descriptors */
  memset(&memInfo, 0, sizeof(memInfo));
  memset (memDescRam, 0, sizeof(memDescRam));
  memInfo.descBase       = (uint32_t *)utilgAddr((uint32_t)memDescRam);
  memInfo.descSize       = TF_SIZE_DESC;
  memInfo.descNum        = TF_NUM_DESC;
  memInfo.manageDescFlag = Qmss_ManageDesc_MANAGE_DESCRIPTOR;
  memInfo.memRegion      = Qmss_MemRegion_MEMORY_REGION0;
  memInfo.startIndex     = 0;

  result = Qmss_insertMemoryRegion (&memInfo);
  if (result < QMSS_SOK)  {
    SALog ("setupQmMem: Qmss_insertMemoryRegion returned error code %d\n", result);
    return (-1);
  }


  /* Initialize the descriptors. This function opens a general
   * purpose queue and intializes the memory from region 0, placing
   * the initialized descriptors onto that queue */
  memset(&descCfg, 0, sizeof(descCfg));
  descCfg.queueGroup        = 0;
  descCfg.memRegion         = Qmss_MemRegion_MEMORY_REGION0;
  descCfg.descNum           = TF_NUM_DESC;
  descCfg.destQueueNum      = TF_Q_FREE_DESC;
  descCfg.queueType         = Qmss_QueueType_GENERAL_PURPOSE_QUEUE;
  descCfg.initDesc          = Cppi_InitDesc_INIT_DESCRIPTOR;
  descCfg.descType          = Cppi_DescType_HOST;
  descCfg.returnQueue.qNum  = QMSS_PARAM_NOT_SPECIFIED;
  descCfg.returnQueue.qMgr  = 0;
  descCfg.epibPresent       = Cppi_EPIB_EPIB_PRESENT;

  /* descCfg.returnPushPolicy = Qmss_Location_TAIL; */
  descCfg.cfg.host.returnPolicy     = Cppi_ReturnPolicy_RETURN_ENTIRE_PACKET;
  descCfg.cfg.host.psLocation       = Cppi_PSLoc_PS_IN_DESC;

  tFramework.QfreeDesc = Cppi_initDescriptor (&descCfg, (uint32_t *)&n);

  if (n != descCfg.descNum)  {
    SALog ("setupQmMem: expected %d descriptors to be initialized, only %d are initialized\n", descCfg.descNum, n);
    return (-1);
  }

  return (0);

}

int setupPassQmMem (void)
{

#ifdef NETSS_INTERNAL_PKTDMA

  Qmss_InitCfg     qmssInitConfig;
  Qmss_StartCfg    qmssStartConfig;
  Qmss_MemRegInfo  memInfo;
  Cppi_DescCfg     descCfg;
  int32_t          result;
  int              n;

  memset (&qmssInitConfig, 0, sizeof (Qmss_InitCfg));
  memset (&qmssStartConfig, 0, sizeof (Qmss_StartCfg));

  //qmssInitConfig.linkingRAM0Base = utilgAddr((uint32_t)memLinkRam);  // It should be 0x0 for internal RAM
  qmssInitConfig.linkingRAM0Base = 0;
  qmssInitConfig.linkingRAM0Size = TF_NUM_DESC;
  qmssInitConfig.linkingRAM1Base = 0;
  qmssInitConfig.maxDescNum      = TF_NUM_DESC;

  // Supply virtual-2-physical conversion functions
  qmssNetssGblCfgParams.virt2Phy     = Netss_qmssVirtToPhy;
  qmssNetssGblCfgParams.phy2Virt     = Netss_qmssPhyToVirt;
  qmssNetssGblCfgParams.virt2PhyDesc = Netss_qmssConvertDescVirtToPhy;
  qmssNetssGblCfgParams.phy2VirtDesc = Netss_qmssConvertDescPhyToVirt;

  result = Qmss_initSubSys (&tFramework.tfPaQmssHandle, Qmss_SubSys_NETSS, &qmssInitConfig, &qmssNetssGblCfgParams);
  if (result != QMSS_SOK)  {
    SALog ("setupPassQmMem: Qmss_Init failed with error code %d\n", result);
    return (-1);
  }

  result = Qmss_startSubSysCfg(&tFramework.tfPaQmssHandle, Qmss_SubSys_NETSS, &qmssStartConfig);
  if (result != QMSS_SOK)  {
    SALog ("setupPassQmMem: Qmss_start failed with error code %d\n", result);
    return (-1);
  }

  /* Setup a single memory region for descriptors */
  memset(&memInfo, 0, sizeof(memInfo));
  memset (passDescRam, 0, TF_SIZE_DESC*TF_NUM_DESC);
  memInfo.descBase       = (uint32_t *)(passDescRam);
  memInfo.descSize       = TF_SIZE_DESC;
  memInfo.descNum        = TF_NUM_DESC;
  memInfo.manageDescFlag = Qmss_ManageDesc_MANAGE_DESCRIPTOR;
  memInfo.memRegion      = Qmss_MemRegion_MEMORY_REGION0;
  memInfo.startIndex     = 0;

  result = Qmss_insertMemoryRegionSubSys (tFramework.tfPaQmssHandle, &memInfo);
  if (result < QMSS_SOK)  {
    SALog ("setupQmMem: Qmss_insertMemoryRegion returned error code %d\n", result);
    return (-1);
  }


  /* Initialize the descriptors. This function opens a general
   * purpose queue and intializes the memory from region 0, placing
   * the initialized descriptors onto that queue */
  memset(&descCfg, 0, sizeof(descCfg));
  descCfg.queueGroup        = 0;
  descCfg.memRegion         = Qmss_MemRegion_MEMORY_REGION0;
  descCfg.descNum           = TF_NUM_DESC;
  descCfg.destQueueNum      = TF_Q_LOC_FREE_DESC;
  descCfg.queueType         = Qmss_QueueType_GENERAL_PURPOSE_QUEUE;
  descCfg.initDesc          = Cppi_InitDesc_INIT_DESCRIPTOR;
  descCfg.descType          = Cppi_DescType_HOST;
  descCfg.returnQueue.qNum  = QMSS_PARAM_NOT_SPECIFIED;
  descCfg.returnQueue.qMgr  = 0;
  descCfg.epibPresent       = Cppi_EPIB_EPIB_PRESENT;

  //descCfg.cfg.host.returnPolicy     = Cppi_ReturnPolicy_RETURN_ENTIRE_PACKET;
  descCfg.cfg.host.returnPolicy     = Cppi_ReturnPolicy_RETURN_BUFFER;
  descCfg.cfg.host.psLocation       = Cppi_PSLoc_PS_IN_DESC;

  tFramework.QLocfreeDesc = Cppi_initDescriptorSubSys (tFramework.tfPaQmssHandle, &descCfg, (uint32_t *)&n);

  if (n != descCfg.descNum)  {
    SALog ("setupPassQmMem: expected %d descriptors to be initialized, only %d are initialized\n", descCfg.descNum, n);
    return (-1);
  }
#endif

  return (0);

}

int closeQmMem(void)
{
	Qmss_Result qmss_result;

    if ((qmss_result = Qmss_removeMemoryRegion (Qmss_MemRegion_MEMORY_REGION0, 0)) != QMSS_SOK)
    {
         SALog ("Error : Remove memory region error code : %d\n", qmss_result);
         return qmss_result;
    }

    if ((qmss_result = Qmss_exit ()))
    {
        SALog ("Error : exit error code : %d\n", qmss_result);
        return qmss_result;
    }

#ifdef NETSS_INTERNAL_PKTDMA

    if ((qmss_result = Qmss_removeMemoryRegionSubSys (tFramework.tfPaQmssHandle, Qmss_MemRegion_MEMORY_REGION0, 0)) != QMSS_SOK)
    {
         SALog ("Error : Remove PASS QMSS memory region error code : %d\n", qmss_result);
         return qmss_result;
    }

    if ((qmss_result = Qmss_exitSubSys (tFramework.tfPaQmssHandle)))
    {
        SALog ("Error : PASS QMSS exit error code : %d\n", qmss_result);
        return qmss_result;
    }

#endif

    return qmss_result;
}

int setupCpdma (void)
{
  Cppi_CpDmaInitCfg cpdmaCfg;
  Cppi_RxChInitCfg  rxChCfg;
  Cppi_TxChInitCfg  txChCfg;

  int32_t result;
  int     i;
  uint8_t isAlloc;

  result = Cppi_init (&cppiGblCfgParams);
  if (result != CPPI_SOK)  {
    SALog ("setupCpdma: cpp_Init returned error %d\n", result);
    return (-1);
  }

  memset(&cpdmaCfg, 0, sizeof(Cppi_CpDmaInitCfg));
  cpdmaCfg.dmaNum           = Cppi_CpDma_NETCP_CPDMA;

  tFramework.tfPaCppiHandle = Cppi_open (&cpdmaCfg);
  if (tFramework.tfPaCppiHandle == NULL)  {
    SALog ("setupCpdma: cppi_Open returned NULL cppi handle\n");
    return (-1);
  }

#ifdef NETSS_INTERNAL_PKTDMA

  memset(&cpdmaCfg, 0, sizeof(Cppi_CpDmaInitCfg));
  cpdmaCfg.dmaNum           = Cppi_CpDma_NETCP_LOCAL_CPDMA;
  cpdmaCfg.qm0BaseAddress   = 0xff1b8000;                    // will CSL definition
  cpdmaCfg.qm1BaseAddress   = 0xff1b8400;                    // will CSL definition
  cpdmaCfg.qm2BaseAddress   = 0xff1b8000;                    // will CSL definition
  cpdmaCfg.qm3BaseAddress   = 0xff1b8400;                    // will CSL definition

  tFramework.tfPaLocCppiHandle = Cppi_open (&cpdmaCfg);
  if (tFramework.tfPaLocCppiHandle == NULL)  {
    SALog ("setupCpdma: cppi_Open returned NULL PA local cppi handle\n");
    return (-1);
  }

#endif

  /* Open all rx channels */
  rxChCfg.rxEnable = Cppi_ChState_CHANNEL_DISABLE;

  for (i = 0; i < nssGblCfgParams.layout.numRxCpdmaChans; i++)  {
    rxChCfg.channelNum        = i;
    tFramework.tfPaRxChHnd[i] = Cppi_rxChannelOpen (tFramework.tfPaCppiHandle, &rxChCfg, &isAlloc);

    if (tFramework.tfPaRxChHnd[i] == NULL)  {
      SALog ("setupCpdma: cppi_RxChannelOpen returned NULL handle for channel number %d\n", i);
      return (-1);
    }

    Cppi_channelEnable (tFramework.tfPaRxChHnd[i]);
  }

  /* Open all 10 tx channels.  */
  txChCfg.priority     = 2;
  txChCfg.txEnable     = Cppi_ChState_CHANNEL_DISABLE;
  txChCfg.filterEPIB   = FALSE;
  txChCfg.filterPS     = FALSE;
  txChCfg.aifMonoMode  = FALSE;

  for (i = 0; i < nssGblCfgParams.layout.numTxCpdmaChans; i++)  {
    txChCfg.channelNum 	 	  = i;
    tFramework.tfPaTxChHnd[i] = Cppi_txChannelOpen (tFramework.tfPaCppiHandle, &txChCfg, &isAlloc);

    if (tFramework.tfPaTxChHnd[i] == NULL)  {
      SALog ("setupCpdma: cppi_TxChannelOpen returned NULL handle for channel number %d\n", i);
      return (-1);
    }

    Cppi_channelEnable (tFramework.tfPaTxChHnd[i]);
  }

  /* Clear CPPI Loobpack bit in PASS CDMA Global Emulation Control Register */
  Cppi_setCpdmaLoopback(tFramework.tfPaCppiHandle, 0);

#ifdef NETSS_INTERNAL_PKTDMA

  /* Open all local rx channels */
  rxChCfg.rxEnable = Cppi_ChState_CHANNEL_DISABLE;

  for (i = 0; i < nssGblCfgParams.layout.numRxCpdmaChans; i++)  {
    rxChCfg.channelNum        = i;
    tFramework.tfPaLocRxChHnd[i] = Cppi_rxChannelOpen (tFramework.tfPaLocCppiHandle, &rxChCfg, &isAlloc);

    if (tFramework.tfPaLocRxChHnd[i] == NULL)  {
      SALog ("setupCpdma: cppi_RxChannelOpen returned NULL handle for local rx channel number %d\n", i);
      return (-1);
    }

    Cppi_channelEnable (tFramework.tfPaLocRxChHnd[i]);
  }

  /* Open all locL tx channels.  */
  txChCfg.priority     = 2;
  txChCfg.txEnable     = Cppi_ChState_CHANNEL_DISABLE;
  txChCfg.filterEPIB   = FALSE;
  txChCfg.filterPS     = FALSE;
  txChCfg.aifMonoMode  = FALSE;

  for (i = 0; i < nssGblCfgParams.layout.numTxCpdmaChans; i++)  {
    txChCfg.channelNum 	 	  = i;
    tFramework.tfPaLocTxChHnd[i] = Cppi_txChannelOpen (tFramework.tfPaLocCppiHandle, &txChCfg, &isAlloc);

    if (tFramework.tfPaLocTxChHnd[i] == NULL)  {
      SALog ("setupCpdma: cppi_TxChannelOpen returned NULL handle for local tx channel number %d\n", i);
      return (-1);
    }

    Cppi_channelEnable (tFramework.tfPaLocTxChHnd[i]);
  }

  /* Clear CPPI Loobpack bit in PASS CDMA Global Emulation Control Register */
  Cppi_setCpdmaLoopback(tFramework.tfPaLocCppiHandle, 0);

#endif

  return (0);

}

static int closeCpdma(void)
{
	int i;
    Cppi_Result cppi_result;

	for (i = 0; i < nssGblCfgParams.layout.numTxCpdmaChans; i++)
	{
    	 if ((cppi_result = Cppi_channelClose (tFramework.tfPaTxChHnd[i])) != CPPI_SOK) {
    	     SALog ("Cppi_channelClose for Tx err: %d \n", cppi_result);
    	 	return (cppi_result);
    	 }
	}

	for (i = 0; i < nssGblCfgParams.layout.numRxCpdmaChans; i++)
	{
    	 if ((cppi_result = Cppi_channelClose (tFramework.tfPaRxChHnd[i])) != CPPI_SOK) {
    	    SALog ("Cppi_channelClose for Rx err: %d \n", cppi_result);
    	 	return (cppi_result);
    	 }
	}

#ifdef NETSS_INTERNAL_PKTDMA

    /* Close the local cpDma setup */
    for (i = 0; i < nssGblCfgParams.layout.numRxCpdmaChans; i++)  {
	    if ((ret = Cppi_channelClose (tFramework.tfPaLocRxChHnd[i])) != CPPI_SOK) {
            SALog ("clearCpdma: Cppi_channelClose returned error code (%d) for PASS local rx channel %d\n", ret, i);
	 	    return (-1);
	    }
    }
    for (i = 0; i < nssGblCfgParams.layout.numTxCpdmaChans; i++)  {
	    if ((ret = Cppi_channelClose (tFramework.tfPaLocTxChHnd[i])) != CPPI_SOK) {
            SALog ("clearCpdma: Cppi_channelClose returned error code (%d) for PASS local tx channel %d\n", ret, i);
	 	    return (-1);
	    }
    }

#endif


	/* Close CPPI CPDMA instance */
	{
		if ((cppi_result = Cppi_close (tFramework.tfPaCppiHandle)) != CPPI_SOK)
		{
			SALog ("Error: Closing CPPI CPDMA error code : %d\n", cppi_result);
			return (cppi_result);
		}

#ifdef NETSS_INTERNAL_PKTDMA
		if ((cppi_result = Cppi_close (tFramework.tfPaLocCppiHandle)) != CPPI_SOK)
		{
			SALog ("Error: Closing CPPI CPDMA error code : %d\n", cppi_result);
			return (cppi_result);
		}
#endif

		/* Deinitialize CPPI LLD */
		if ((cppi_result = Cppi_exit ()) != CPPI_SOK)
		{
			SALog ("Error : Exiting CPPI error code : %d\n", cppi_result);
			return (cppi_result);
		}
	}

 	return (cppi_result);
}



/* Setup all the queues used in the example */
int setupQueues (void)
{
  int      i;
  uint8_t  isAlloc;

  Qmss_Queue q;
  Cppi_HostDesc *hd;

  /* The 10 PA transmit queues (corresponding to the 10 tx cdma channels */
  for (i = 0; i < nssGblCfgParams.layout.numTxQueues; i++)  {

    tFramework.QPaTx[i] = Qmss_queueOpen (Qmss_QueueType_PASS_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAlloc);

    if (tFramework.QPaTx[i] < 0)  {
      SALog ("setupQueues: Qmss_queueOpen failed for PA transmit queue number %d\n", nssGblCfgParams.layout.txQueueBase+i);
      return (-1);
    }

    Qmss_setQueueThreshold (tFramework.QPaTx[i], 1, 1);

  }

  /* The queues with attached buffers */
  tFramework.QLinkedBuf1 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LINKED_BUF_Q1, &isAlloc);

  if (tFramework.QLinkedBuf1 < 0)  {
  	SALog ("setupQueues: Qmss_queueOpen failed for queue %d\n", TF_LINKED_BUF_Q1);
  	return (-1);
  }

  tFramework.QLinkedBuf2 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LINKED_BUF_Q2, &isAlloc);

  if (tFramework.QLinkedBuf2 < 0)  {
  	SALog ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_LINKED_BUF_Q2);
  	return (-1);
  }

  tFramework.QLinkedBuf3 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LINKED_BUF_Q3, &isAlloc);

  if (tFramework.QLinkedBuf3 < 0)  {
  	SALog ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_LINKED_BUF_Q3);
  	return (-1);
  }

  tFramework.QLinkedBuf4 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LINKED_BUF_Q4, &isAlloc);

  if (tFramework.QLinkedBuf4 < 0)  {
  	SALog ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_LINKED_BUF_Q3);
  	return (-1);
  }

  tFramework.QHostLinkedBuf1 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_HOST_LINKED_BUF_Q1, &isAlloc);

  if (tFramework.QHostLinkedBuf1 < 0)  {
  	SALog ("setupQueues: Qmss_queueOpen failed for queue %d\n", TF_HOST_LINKED_BUF_Q1);
  	return (-1);
  }

  tFramework.QHostLinkedBuf2 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_HOST_LINKED_BUF_Q2, &isAlloc);

  if (tFramework.QHostLinkedBuf2 < 0)  {
  	SALog ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_HOST_LINKED_BUF_Q2);
  	return (-1);
  }

  tFramework.QHostLinkedBuf3 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_HOST_LINKED_BUF_Q3, &isAlloc);

  if (tFramework.QHostLinkedBuf3 < 0)  {
  	SALog ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_HOST_LINKED_BUF_Q3);
  	return (-1);
  }

  tFramework.QHostLinkedBuf4 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_HOST_LINKED_BUF_Q4, &isAlloc);

  if (tFramework.QHostLinkedBuf4 < 0)  {
  	SALog ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_HOST_LINKED_BUF_Q3);
  	return (-1);
  }



  /* Attach buffers to the queues and push them onto the queue */
  q.qMgr = 0;
  q.qNum = tFramework.QLinkedBuf1;

  for (i = 0; i < TF_LINKED_BUF_Q1_NBUFS; i++)   {

    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QfreeDesc)) & ~15);
    if (hd == NULL)  {
      SALog ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ1[i])), sizeof(memQ1[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ1[i])), sizeof(memQ1[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Cppi_setReturnPolicy (Cppi_DescType_HOST, (Cppi_Desc *)hd, Cppi_ReturnPolicy_RETURN_BUFFER);
    Qmss_queuePushDesc (tFramework.QLinkedBuf1, (Ptr)hd);

  }

  q.qNum = tFramework.QLinkedBuf2;

  for (i = 0; i < TF_LINKED_BUF_Q2_NBUFS; i++)   {

    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QfreeDesc)) & ~15);
    if (hd == NULL)  {
      SALog ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ2[i])), sizeof(memQ2[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ2[i])), sizeof(memQ2[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Cppi_setReturnPolicy (Cppi_DescType_HOST, (Cppi_Desc *)hd, Cppi_ReturnPolicy_RETURN_BUFFER);
    Qmss_queuePushDesc (tFramework.QLinkedBuf2, (Ptr)hd);

  }

  q.qNum = tFramework.QLinkedBuf3;

  for (i = 0; i < TF_LINKED_BUF_Q3_NBUFS; i++)   {

    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QfreeDesc)) & ~15);
    if (hd == NULL)  {
      SALog ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ3[i])), sizeof(memQ3[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ3[i])), sizeof(memQ3[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Cppi_setReturnPolicy (Cppi_DescType_HOST, (Cppi_Desc *)hd, Cppi_ReturnPolicy_RETURN_BUFFER);
    Qmss_queuePushDesc (tFramework.QLinkedBuf3, (Ptr)hd);

  }

  q.qNum = tFramework.QLinkedBuf4;

  for (i = 0; i < TF_LINKED_BUF_Q4_NBUFS; i++)   {

    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QfreeDesc)) & ~15);
    if (hd == NULL)  {
      SALog ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ4[i])), sizeof(memQ4[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ4[i])), sizeof(memQ4[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Cppi_setReturnPolicy (Cppi_DescType_HOST, (Cppi_Desc *)hd, Cppi_ReturnPolicy_RETURN_BUFFER);
    Qmss_queuePushDesc (tFramework.QLinkedBuf4, (Ptr)hd);

  }

  /* Attach buffers to the queues and push them onto the queue */
  q.qNum = tFramework.QHostLinkedBuf1;

  for (i = 0; i < TF_HOST_LINKED_BUF_Q1_NBUFS; i++)   {

    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QfreeDesc)) & ~15);
    if (hd == NULL)  {
      SALog ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memHostQ1[i])), sizeof(memHostQ1[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memHostQ1[i])), sizeof(memHostQ1[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Cppi_setReturnPolicy (Cppi_DescType_HOST, (Cppi_Desc *)hd, Cppi_ReturnPolicy_RETURN_BUFFER);
    Qmss_queuePushDesc (tFramework.QHostLinkedBuf1, (Ptr)hd);

  }

  q.qNum = tFramework.QHostLinkedBuf2;

  for (i = 0; i < TF_HOST_LINKED_BUF_Q2_NBUFS; i++)   {

    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QfreeDesc)) & ~15);
    if (hd == NULL)  {
      SALog ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memHostQ2[i])), sizeof(memHostQ2[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memHostQ2[i])), sizeof(memHostQ2[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Cppi_setReturnPolicy (Cppi_DescType_HOST, (Cppi_Desc *)hd, Cppi_ReturnPolicy_RETURN_BUFFER);
    Qmss_queuePushDesc (tFramework.QHostLinkedBuf2, (Ptr)hd);

  }

  q.qNum = tFramework.QHostLinkedBuf3;

  for (i = 0; i < TF_HOST_LINKED_BUF_Q3_NBUFS; i++)   {

    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QfreeDesc)) & ~15);
    if (hd == NULL)  {
      SALog ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memHostQ3[i])), sizeof(memHostQ3[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memHostQ3[i])), sizeof(memHostQ3[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Cppi_setReturnPolicy (Cppi_DescType_HOST, (Cppi_Desc *)hd, Cppi_ReturnPolicy_RETURN_BUFFER);
    Qmss_queuePushDesc (tFramework.QHostLinkedBuf3, (Ptr)hd);

  }

  q.qNum = tFramework.QHostLinkedBuf4;

  for (i = 0; i < TF_HOST_LINKED_BUF_Q4_NBUFS; i++)   {

    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QfreeDesc)) & ~15);
    if (hd == NULL)  {
      SALog ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memHostQ4[i])), sizeof(memHostQ4[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memHostQ4[i])), sizeof(memHostQ4[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Cppi_setReturnPolicy (Cppi_DescType_HOST, (Cppi_Desc *)hd, Cppi_ReturnPolicy_RETURN_BUFFER);
    Qmss_queuePushDesc (tFramework.QHostLinkedBuf4, (Ptr)hd);

  }

  /* General purpose queues */
  for (i = 0; i < TF_NUM_GEN_QUEUES; i++)  {

  	tFramework.QGen[i] = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_FIRST_GEN_QUEUE + i, &isAlloc);

 	if (tFramework.QGen[i] < 0)  {
  	  SALog ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_FIRST_GEN_QUEUE + i);
  	  return (-1);
  	}
  }

#ifdef NETSS_INTERNAL_PKTDMA

  /* The queues with attached buffers */
  tFramework.QLocLinkedBuf1 = Qmss_queueOpenSubSys (tFramework.tfPaQmssHandle, Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LOC_LINKED_BUF_Q1, &isAlloc);

  if (tFramework.QLinkedBuf1 < 0)  {
  	SALog ("setupQueues: Qmss_queueOpenSubSys failed for queue %d\n", TF_LOC_LINKED_BUF_Q1);
  	return (-1);
  }

  tFramework.QLocLinkedBuf2 = Qmss_queueOpenSubSys (tFramework.tfPaQmssHandle, Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LOC_LINKED_BUF_Q2, &isAlloc);

  if (tFramework.QLinkedBuf2 < 0)  {
  	SALog ("SetupQueues: Qmss_queueOpenSubSys failed for queue %d\n", TF_LOC_LINKED_BUF_Q2);
  	return (-1);
  }

  tFramework.QLocLinkedBuf3 = Qmss_queueOpenSubSys (tFramework.tfPaQmssHandle, Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LOC_LINKED_BUF_Q3, &isAlloc);

  if (tFramework.QLinkedBuf3 < 0)  {
  	SALog ("SetupQueues: Qmss_queueOpenSubSys failed for queue %d\n", TF_LOC_LINKED_BUF_Q3);
  	return (-1);
  }

  tFramework.QLocLinkedBuf4 = Qmss_queueOpenSubSys (tFramework.tfPaQmssHandle, Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LOC_LINKED_BUF_Q4, &isAlloc);

  if (tFramework.QLinkedBuf4 < 0)  {
  	SALog ("SetupQueues: Qmss_queueOpenSubSys failed for queue %d\n", TF_LOC_LINKED_BUF_Q4);
  	return (-1);
  }


  /* Attach buffers to the queues and push them onto the queue */
  q.qMgr = 0;

  q.qNum = TF_LOC_LINKED_BUF_Q1;
  for (i = 0; i < TF_LINKED_BUF_Q1_NBUFS; i++)   {

    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QLocfreeDesc)) & ~15);
    if (hd == NULL)  {
      SALog ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QLocfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)memLocQ1[i], sizeof(memLocQ1[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)memLocQ1[i], sizeof(memLocQ1[i]));
    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)memLocQ1[i], sizeof(memLocQ1[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Qmss_queuePushDesc (tFramework.QLocLinkedBuf1, (Ptr)hd);

  }

  q.qNum = TF_LOC_LINKED_BUF_Q2;
  for (i = 0; i < TF_LINKED_BUF_Q2_NBUFS; i++)   {

    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QLocfreeDesc)) & ~15);
    if (hd == NULL)  {
      SALog ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QLocfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)memLocQ2[i], sizeof(memLocQ2[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)memLocQ2[i], sizeof(memLocQ2[i]));
    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)memLocQ2[i], sizeof(memLocQ2[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Qmss_queuePushDesc (tFramework.QLocLinkedBuf2, (Ptr)hd);

  }

  q.qNum = TF_LOC_LINKED_BUF_Q3;
  for (i = 0; i < TF_LINKED_BUF_Q3_NBUFS; i++)   {

    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QLocfreeDesc)) & ~15);
    if (hd == NULL)  {
      SALog ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QLocfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)memLocQ3[i], sizeof(memLocQ3[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)memLocQ3[i], sizeof(memLocQ3[i]));
    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)memLocQ3[i], sizeof(memLocQ3[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Qmss_queuePushDesc (tFramework.QLocLinkedBuf3, (Ptr)hd);

  }

  q.qNum = TF_LOC_LINKED_BUF_Q4;
  for (i = 0; i < TF_LINKED_BUF_Q4_NBUFS; i++)   {

    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QLocfreeDesc)) & ~15);
    if (hd == NULL)  {
      SALog ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QLocfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)memLocQ4[i], sizeof(memLocQ4[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)memLocQ4[i], sizeof(memLocQ4[i]));
    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)memLocQ4[i], sizeof(memLocQ4[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Qmss_queuePushDesc (tFramework.QLocLinkedBuf4, (Ptr)hd);

  }


#endif



  return (0);

}

int closeQueues(void)
{
	int i;

    /* The PA transmit queues (corresponding to the tx cdma channels */
 	for (i = 0; i < nssGblCfgParams.layout.numTxQueues; i++)  {
 		 Qmss_queueEmpty (tFramework.QPaTx[i]);
 	 	 Qmss_queueClose (tFramework.QPaTx[i]);
 	}

	Qmss_queueEmpty(tFramework.QLinkedBuf1);
	Qmss_queueClose(tFramework.QLinkedBuf1);

	Qmss_queueEmpty(tFramework.QLinkedBuf2);
	Qmss_queueClose(tFramework.QLinkedBuf2);

	Qmss_queueEmpty(tFramework.QLinkedBuf3);
	Qmss_queueClose(tFramework.QLinkedBuf3);

	Qmss_queueEmpty(tFramework.QLinkedBuf4);
	Qmss_queueClose(tFramework.QLinkedBuf4);

	Qmss_queueEmpty(tFramework.QHostLinkedBuf1);
	Qmss_queueClose(tFramework.QHostLinkedBuf1);

	Qmss_queueEmpty(tFramework.QHostLinkedBuf2);
	Qmss_queueClose(tFramework.QHostLinkedBuf2);

	Qmss_queueEmpty(tFramework.QHostLinkedBuf3);
	Qmss_queueClose(tFramework.QHostLinkedBuf3);

	Qmss_queueEmpty(tFramework.QHostLinkedBuf4);
	Qmss_queueClose(tFramework.QHostLinkedBuf4);

	Qmss_queueEmpty(tFramework.QfreeDesc);
	Qmss_queueClose(tFramework.QfreeDesc);

	/* General purpose queues */
	for (i = 0; i < TF_NUM_GEN_QUEUES; i++)  {
		Qmss_queueEmpty(tFramework.QGen[i]);
		Qmss_queueClose(tFramework.QGen[i]);
	}

#ifdef NETSS_INTERNAL_PKTDMA

	Qmss_queueEmpty(tFramework.QLocLinkedBuf1);
	Qmss_queueClose(tFramework.QLocLinkedBuf1);

	Qmss_queueEmpty(tFramework.QLocLinkedBuf2);
	Qmss_queueClose(tFramework.QLocLinkedBuf2);

	Qmss_queueEmpty(tFramework.QLocLinkedBuf3);
	Qmss_queueClose(tFramework.QLocLinkedBuf3);

	Qmss_queueEmpty(tFramework.QLocLinkedBuf4);
	Qmss_queueClose(tFramework.QLocLinkedBuf4);

	Qmss_queueEmpty(tFramework.QLocfreeDesc);
	Qmss_queueClose(tFramework.QLocfreeDesc);

#endif
	return 0;
}




/* Configure flows */
int setupFlows (void)
{
  Cppi_RxFlowCfg  rxFlowCfg;
  uint8_t         isAlloc;

  /* Configure Rx flow */
  rxFlowCfg.flowIdNum      = tFramework.tfFlowNum = 0;
  rxFlowCfg.rx_dest_qnum   = TF_FIRST_GEN_QUEUE + TF_NUM_GEN_QUEUES -1;   /* Override in PA */
  rxFlowCfg.rx_dest_qmgr   = 0;
  rxFlowCfg.rx_sop_offset  = 0;
  rxFlowCfg.rx_ps_location = Cppi_PSLoc_PS_IN_DESC;
  rxFlowCfg.rx_desc_type   = Cppi_DescType_HOST;
  rxFlowCfg.rx_error_handling = 1;
  rxFlowCfg.rx_psinfo_present = 1;
  rxFlowCfg.rx_einfo_present  = 1;

  rxFlowCfg.rx_dest_tag_lo = 0;
  rxFlowCfg.rx_dest_tag_hi = 0;
  rxFlowCfg.rx_src_tag_lo  = 0;
  rxFlowCfg.rx_src_tag_hi  = 0;

  rxFlowCfg.rx_size_thresh0_en = 1;
  rxFlowCfg.rx_size_thresh1_en = 1;
  rxFlowCfg.rx_size_thresh2_en = 1;

  rxFlowCfg.rx_dest_tag_lo_sel = 0;
  rxFlowCfg.rx_dest_tag_hi_sel = 0;
  rxFlowCfg.rx_src_tag_lo_sel  = 0;
  rxFlowCfg.rx_src_tag_hi_sel  = 0;

  rxFlowCfg.rx_fdq1_qnum = tFramework.QLinkedBuf2;
  rxFlowCfg.rx_fdq1_qmgr = 0;
  rxFlowCfg.rx_fdq2_qnum = tFramework.QLinkedBuf2;
  rxFlowCfg.rx_fdq2_qmgr = 0;
  rxFlowCfg.rx_fdq3_qnum = tFramework.QLinkedBuf2;
  rxFlowCfg.rx_fdq3_qmgr = 0;

  rxFlowCfg.rx_size_thresh0 = TF_LINKED_BUF_Q1_BUF_SIZE;
  rxFlowCfg.rx_size_thresh1 = TF_LINKED_BUF_Q2_BUF_SIZE;
  rxFlowCfg.rx_size_thresh2  = TF_LINKED_BUF_Q3_BUF_SIZE;

  rxFlowCfg.rx_fdq0_sz0_qnum = tFramework.QLinkedBuf4;
  rxFlowCfg.rx_fdq0_sz0_qmgr = 0;
  rxFlowCfg.rx_fdq0_sz1_qnum = tFramework.QLinkedBuf4;
  rxFlowCfg.rx_fdq0_sz1_qmgr = 0;
  rxFlowCfg.rx_fdq0_sz2_qnum = tFramework.QLinkedBuf4;
  rxFlowCfg.rx_fdq0_sz2_qmgr = 0;
  rxFlowCfg.rx_fdq0_sz3_qnum = tFramework.QLinkedBuf4;
  rxFlowCfg.rx_fdq0_sz3_qmgr = 0;

  tFramework.tfPaFlowHnd = Cppi_configureRxFlow (tFramework.tfPaCppiHandle, &rxFlowCfg, &isAlloc);
  if (tFramework.tfPaFlowHnd == NULL)  {
    SALog ("setupFlows: cppi_ConfigureRxFlow returned NULL\n");
    return (-1);
  }

  #ifdef NETSS_INTERNAL_PKTDMA

  /* Configure Local Rx flow */
  rxFlowCfg.flowIdNum      = tFramework.tfLocFlowNum = 0;
  rxFlowCfg.rx_dest_qnum   = 0;   /* Override in PA */
  rxFlowCfg.rx_dest_qmgr   = 0;
  rxFlowCfg.rx_sop_offset  = 0;
  rxFlowCfg.rx_ps_location = Cppi_PSLoc_PS_IN_DESC;
  rxFlowCfg.rx_desc_type   = Cppi_DescType_HOST;
  rxFlowCfg.rx_error_handling = 1;
  rxFlowCfg.rx_psinfo_present = 1;
  rxFlowCfg.rx_einfo_present  = 1;

  rxFlowCfg.rx_dest_tag_lo = 0;
  rxFlowCfg.rx_dest_tag_hi = 0;
  rxFlowCfg.rx_src_tag_lo  = 0;
  rxFlowCfg.rx_src_tag_hi  = 0;

  rxFlowCfg.rx_size_thresh0_en = 1;
  rxFlowCfg.rx_size_thresh1_en = 1;
  rxFlowCfg.rx_size_thresh2_en = 1;

  rxFlowCfg.rx_fdq1_qnum = TF_LOC_LINKED_BUF_Q2;
  rxFlowCfg.rx_fdq1_qmgr = 0;
  rxFlowCfg.rx_fdq3_qnum = TF_LOC_LINKED_BUF_Q2;
  rxFlowCfg.rx_fdq3_qmgr = 0;
  rxFlowCfg.rx_fdq2_qnum = TF_LOC_LINKED_BUF_Q2;
  rxFlowCfg.rx_fdq2_qmgr = 0;

  rxFlowCfg.rx_size_thresh2  = TF_LINKED_BUF_Q3_BUF_SIZE;
  rxFlowCfg.rx_size_thresh1  = TF_LINKED_BUF_Q2_BUF_SIZE;
  rxFlowCfg.rx_size_thresh0  = TF_LINKED_BUF_Q1_BUF_SIZE;

  rxFlowCfg.rx_fdq0_sz0_qnum = TF_LOC_LINKED_BUF_Q1;
  rxFlowCfg.rx_fdq0_sz0_qmgr = 0;
  rxFlowCfg.rx_fdq0_sz1_qnum = TF_LOC_LINKED_BUF_Q2;
  rxFlowCfg.rx_fdq0_sz1_qmgr = 0;
  rxFlowCfg.rx_fdq0_sz2_qnum = TF_LOC_LINKED_BUF_Q3;
  rxFlowCfg.rx_fdq0_sz2_qmgr = 0;
  rxFlowCfg.rx_fdq0_sz3_qnum = TF_LOC_LINKED_BUF_Q4;
  rxFlowCfg.rx_fdq0_sz3_qmgr = 0;

  tFramework.tfPaLocFlowHnd0 = Cppi_configureRxFlow (tFramework.tfPaLocCppiHandle, &rxFlowCfg, &isAlloc);
  if (tFramework.tfPaLocFlowHnd0 == NULL)  {
    SALog ("setupFlows: cppi_ConfigureRxFlow returned NULL on local flow 0\n");
    return (-1);
  }

  #endif



  return (0);

}

int closeFlows(void)
{
    Cppi_Result cppi_result;

    if ((cppi_result = Cppi_closeRxFlow (tFramework.tfPaFlowHnd)) != CPPI_SOK) {
  	    SALog ("closeFlows: Cppi_closeRxFlow failed with error code = %d\n", cppi_result);

       	return (-1);
    }

#ifdef NETSS_INTERNAL_PKTDMA
    if ((cppi_result = Cppi_closeRxFlow (tFramework.tfPaLocFlowHnd)) != CPPI_SOK) {
  	    SALog ("closeFlows: Cppi_closeRxFlow failed with error code = %d\n", cppi_result);

       	return (-1);
    }

#endif

    return 0;
}
#else

/*
 * UDMA driver objects
 */
struct Udma_DrvObj      gUdmaDrvObj;
struct Udma_ChObj       gUdmaTxChObj;
struct Udma_ChObj       gUdmaRxChObj[2];
struct Udma_FlowObj     gUdmaFlowObj;
struct Udma_RingObj     gUdmaRingObj[2];
struct Udma_EventObj    gUdmaEvtObj;
#if defined (AM64X_USE_DEFAULT_FLOW)
struct Udma_EventObj    gUdmaEvtObj0;
#endif

volatile int                     gRxPktCntInRing = 0;

void framework_rxIsrFxn(Udma_EventHandle  eventHandle,
                       uint32_t          eventType,
                       void             *appData)
{
    gRxPktCntInRing++;
    return;
}
#if defined (AM64X_USE_DEFAULT_FLOW)
volatile int                     gRxPktCntInRing0 = 0;

void framework_rxIsrFxn0(Udma_EventHandle  eventHandle,
                       uint32_t          eventType,
                       void             *appData)
{
    gRxPktCntInRing0++;
    return;
}
#endif

int frameworkUdmaInitDrv(void)
{
    int32_t         retVal = UDMA_SOK;
    Udma_InitPrms   initPrms;
    uint32_t        instId;

#if defined(DMA_TYPE_LCDMA)
    instId = UDMA_INST_ID_PKTDMA_0;
#else

#if defined(BUILD_MPU)
    instId = UDMA_INST_ID_MAIN_0;
#else
    instId = UDMA_INST_ID_MCU_0;
#endif /* BUILD_MPU */
#endif /* DMA_TYPE_LCDMA */

    UdmaInitPrms_init(instId, &initPrms);

#if defined (DMA_TYPE_LCDMA) && defined(BUILD_MPU)
    /*PKTDMA Tx Ch 25 (SAUL Tx Ch1) */
    initPrms.rmInitPrms.startMappedTxCh[UDMA_MAPPED_TX_GROUP_SAUL]  =  25U;
    initPrms.rmInitPrms.numMappedTxCh[UDMA_MAPPED_TX_GROUP_SAUL]    = 1U;

    /* PKTDMA Rx Ch 19,20 (SAUL Rx Ch2,3) */
    initPrms.rmInitPrms.startMappedRxCh[UDMA_MAPPED_RX_GROUP_SAUL- UDMA_NUM_MAPPED_TX_GROUP]    = 19U;
    initPrms.rmInitPrms.numMappedRxCh[UDMA_MAPPED_RX_GROUP_SAUL- UDMA_NUM_MAPPED_TX_GROUP]    = 2U;

    /* PKTDMA Tx Rings 88 to 95 (which are tied to SAUL Tx Ch1) */
    initPrms.rmInitPrms.startMappedRing[UDMA_MAPPED_TX_GROUP_SAUL]    = 88U;
    initPrms.rmInitPrms.startMappedRing[UDMA_MAPPED_RX_GROUP_SAUL]    = 8U;

    /* PKTDMA Rx Rings 40 to 47[with offset 112](which are tied to SAUL Rx Ch2,3) */
    initPrms.rmInitPrms.numMappedRing[UDMA_MAPPED_TX_GROUP_SAUL]    = 152U;
    initPrms.rmInitPrms.numMappedRing[UDMA_MAPPED_RX_GROUP_SAUL]    = 8U;

#endif

    retVal = Udma_init(&gUdmaDrvObj, &initPrms);
    if(UDMA_SOK == retVal)
    {
        tFramework.gDrvHandle = &gUdmaDrvObj;
    }
    else
    {
        SALog("error in frameworkUdmaInitDrv()  \n");
        System_flush();
    }
    return (retVal);
}

int frameworkUdmaSetupTxChannel(void)
{
    uint32_t            chType;
    Udma_ChPrms         chPrms;
    Udma_ChTxPrms       txPrms;
    Udma_RingHandle     ringHandle;

    int32_t         retVal = UDMA_SOK;
    /* Create the Tx Channel */
    /* TX channel parameters */
#if defined (SOC_AM64X)    
    chType                      = UDMA_CH_TYPE_TX_MAPPED;
#else
    chType                      = UDMA_CH_TYPE_TX;
#endif
    UdmaChPrms_init(&chPrms, chType);

#if defined (SOC_AM64X)
    chPrms.mappedChGrp          = UDMA_MAPPED_TX_GROUP_SAUL;
    chPrms.peerChNum            = TF_SA2UL_PEER_TXCHAN;
    chPrms.fqRingPrms.ringMem   = &memTxRing[0];
    chPrms.fqRingPrms.elemCnt   = TF_RING_TRCNT;
    /* this is the dual ring mode */
    chPrms.fqRingPrms.mode      = TISCI_MSG_VALUE_RM_RING_MODE_RING;
#if defined (ACP_COHERENCY)
    chPrms.fqRingPrms.asel      = CSL_LCDMA_RINGACC_ASEL_ENDPOINT_ACP_WR_ALLOC;
#else
    chPrms.fqRingPrms.asel      = CSL_LCDMA_RINGACC_ASEL_ENDPOINT_PHYSADDR;
#endif

#else
    chPrms.peerChNum            = TF_SA2UL_PEER_TXCHAN;
    chPrms.fqRingPrms.ringMem   = &memTxRing[0];
    chPrms.cqRingPrms.ringMem   = &memTxCompRing[0];
    chPrms.fqRingPrms.elemCnt   = TF_RING_TRCNT;
    chPrms.cqRingPrms.elemCnt   = TF_RING_TRCNT;

    chPrms.fqRingPrms.mode      = TISCI_MSG_VALUE_RM_RING_MODE_RING;
    chPrms.cqRingPrms.mode      = TISCI_MSG_VALUE_RM_RING_MODE_RING;
#endif
    /* Open TX channel for transmit */
    tFramework.gTxChHandle = &gUdmaTxChObj;
    retVal = Udma_chOpen(tFramework.gDrvHandle, tFramework.gTxChHandle, chType, &chPrms);

    if(UDMA_SOK == retVal)
    {
        UdmaChTxPrms_init(&txPrms, chType);
        txPrms.dmaPriority = UDMA_DEFAULT_UTC_CH_DMA_PRIORITY;
        txPrms.fetchWordSize = TF_SIZE_DESC >> 2;
        retVal = Udma_chConfigTx(tFramework.gTxChHandle, &txPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(tFramework.gTxChHandle);
        }
    }
    else
    {
        SALog("error in Tx Udma_chOpen()  \n");
        System_flush();
    }

    if (retVal == UDMA_SOK)
    {
        /* Update the Tx Ring numbers */
        ringHandle                = Udma_chGetFqRingHandle(tFramework.gTxChHandle);
        tFramework.gTxRingHandle  = ringHandle;
        tFramework.txRingNum      = Udma_ringGetNum(ringHandle);

        ringHandle                = Udma_chGetCqRingHandle(tFramework.gTxChHandle);
        tFramework.gTxComplRingHandle = ringHandle;
        tFramework.txComplRingNum = Udma_ringGetNum(ringHandle);
    }
    return (retVal);

}

int frameworkUdmaSetupRxChannel(void)
{
    uint32_t            chType;
    Udma_ChPrms         chPrms;
    Udma_ChRxPrms       rxPrms;
    Udma_RingHandle     ringHandle;
#if defined(SOC_AM64X)
    /* Flow allocation happens inside UDMA LLD */
#else
    Udma_FlowHandle     flowHandle;
    Udma_FlowPrms       flowPrms;
#endif
    Udma_EventPrms      eventPrms;
    uint32_t            intArg = 0;
    int32_t             retVal = UDMA_SOK;

    /* Create the Rx Channel */
    /* TX channel parameters */
#if defined (SOC_AM64X)
    chType                      = UDMA_CH_TYPE_RX_MAPPED;
#else
    chType                      = UDMA_CH_TYPE_RX;

    /* Note that the ring memory is not provided for the second channel thread for SA as
     * We create the channel and ring for thr1 and use that flow for SA2UL applications
     */

     /* create a flow allocation here for Rx channels */
     tFramework.gRxFlowHandle = flowHandle = &gUdmaFlowObj;
     retVal  = Udma_flowAlloc(tFramework.gDrvHandle, flowHandle,1);

     if (retVal != UDMA_SOK)
     {
         return (retVal);
     }
     /* Update the created default flow with above configurations for sa2ul */
     tFramework.tfFlowNum      = Udma_flowGetNum(flowHandle);
#endif

    /* Create the Rx Channel */
    /* RX channel parameters */
    UdmaChPrms_init(&chPrms, chType);
#if defined(SOC_AM64X)
    chPrms.peerChNum            = TF_SA2UL_PEER_RXCHAN1;
    chPrms.mappedChGrp          = UDMA_MAPPED_RX_GROUP_SAUL;    
    chPrms.fqRingPrms.ringMem   = &memRxFreeRing[0];
    chPrms.fqRingPrms.elemCnt   = TF_RING_TRCNT;
    chPrms.fqRingPrms.mode      = TISCI_MSG_VALUE_RM_RING_MODE_RING;
#if defined (ACP_COHERENCY)
    chPrms.fqRingPrms.asel      = CSL_LCDMA_RINGACC_ASEL_ENDPOINT_ACP_WR_ALLOC;
#else
    chPrms.fqRingPrms.asel      = CSL_LCDMA_RINGACC_ASEL_ENDPOINT_PHYSADDR;
#endif
#else
    chPrms.peerChNum            = TF_SA2UL_PEER_RXCHAN1;
    chPrms.fqRingPrms.ringMem   = &memRxFreeRing[0];
    chPrms.cqRingPrms.ringMem   = &memRxRing[0];
    chPrms.fqRingPrms.elemCnt   = TF_RING_TRCNT;
    chPrms.cqRingPrms.elemCnt   = TF_RING_TRCNT;

    chPrms.fqRingPrms.mode      = TISCI_MSG_VALUE_RM_RING_MODE_MESSAGE;
    chPrms.cqRingPrms.mode      = TISCI_MSG_VALUE_RM_RING_MODE_MESSAGE;
#endif
    /* Open RX channel for receive from SA */
    tFramework.gRxChHandle[1] = &gUdmaRxChObj[1];
    retVal = Udma_chOpen(tFramework.gDrvHandle, tFramework.gRxChHandle[1], chType, &chPrms);

    if(UDMA_SOK == retVal)
    {
        UdmaChRxPrms_init(&rxPrms, chType);
        rxPrms.dmaPriority = UDMA_DEFAULT_UTC_CH_DMA_PRIORITY;
        rxPrms.fetchWordSize = TF_SIZE_DESC >> 2;
#if defined(SOC_AM64X)
        rxPrms.flowEInfoPresent   = 1;
        rxPrms.flowPsInfoPresent  = 1;
#else
        rxPrms.flowIdFwRangeStart = tFramework.tfFlowNum;
        rxPrms.flowIdFwRangeCnt   = 1;
        rxPrms.configDefaultFlow  = FALSE;
#endif
        retVal = Udma_chConfigRx(tFramework.gRxChHandle[1], &rxPrms);
    }
    else
    {
        SALog("error in Rx-1 Udma_chOpen()  \n");
        System_flush();
    }
#if defined(SOC_AM64X)
    /* Update the Rx Ring numbers */
    ringHandle                    = Udma_chGetFqRingHandle(tFramework.gRxChHandle[1]);
    tFramework.gRxFreeRingHandle  = ringHandle;
    tFramework.rxFreeRingNum      = Udma_ringGetNum(ringHandle);
    tFramework.gRxFlowHandle      = Udma_chGetDefaultFlowHandle(tFramework.gRxChHandle[1]);
    tFramework.tfFlowNum          = Udma_flowGetNum(tFramework.gRxFlowHandle);
#if defined (AM64X_USE_DEFAULT_FLOW)
    tFramework.tfFlowNum          = 0x3FFF;
#endif
    /* Update the Rx Ring numbers */
    ringHandle                    = Udma_chGetCqRingHandle(tFramework.gRxChHandle[1]);
    tFramework.gRxRingHandle      = ringHandle;
    tFramework.rxComplRingNum     = Udma_ringGetNum(ringHandle);
#endif

    /* Create the channel for the second thread with same flow as other thread */
    UdmaChPrms_init(&chPrms, chType);
    chPrms.peerChNum            = TF_SA2UL_PEER_RXCHAN0;
#if defined(SOC_AM64X)
    chPrms.mappedChGrp          = UDMA_MAPPED_RX_GROUP_SAUL;
    chPrms.fqRingPrms.elemCnt   = TF_RING_TRCNT;
    chPrms.fqRingPrms.mode      = TISCI_MSG_VALUE_RM_RING_MODE_RING;
#if defined (ACP_COHERENCY)
    chPrms.fqRingPrms.asel      = CSL_LCDMA_RINGACC_ASEL_ENDPOINT_ACP_WR_ALLOC;
#else
    chPrms.fqRingPrms.asel      = CSL_LCDMA_RINGACC_ASEL_ENDPOINT_PHYSADDR;
#endif
#if defined(AM64X_USE_DEFAULT_FLOW)
    chPrms.fqRingPrms.ringMem   = &memRxRing[0];
#endif
#endif
    tFramework.gRxChHandle[0]   = &gUdmaRxChObj[0];
    retVal = Udma_chOpen(tFramework.gDrvHandle, tFramework.gRxChHandle[0], chType, &chPrms);

    if(UDMA_SOK == retVal)
    {
        UdmaChRxPrms_init(&rxPrms, chType);
        rxPrms.dmaPriority = UDMA_DEFAULT_UTC_CH_DMA_PRIORITY;
        rxPrms.fetchWordSize = TF_SIZE_DESC >> 2;
#if defined(SOC_AM64X)
#if defined(AM64X_USE_DEFAULT_FLOW)
        rxPrms.flowEInfoPresent   = 1;
        rxPrms.flowPsInfoPresent  = 1;
#endif
#else
        rxPrms.flowIdFwRangeStart = tFramework.tfFlowNum;
        rxPrms.flowIdFwRangeCnt   = 1;
#endif
        rxPrms.configDefaultFlow  = FALSE;

        retVal = Udma_chConfigRx(tFramework.gRxChHandle[0], &rxPrms);
    }
    else
    {
        SALog("error in Rx-0 Udma_chOpen()  \n");
        System_flush();
    }
#if defined(SOC_AM64X)
    /* Nothing to do since flow is already setup earlier */
    /* Dual ring mode, the same is completion ring */
    tFramework.rxComplRingNum = tFramework.rxFreeRingNum;
    /* Register Ring complete Isr */
    tFramework.gRxEvtHandle = &gUdmaEvtObj;

    /* Initialize event parameters */
    intArg                      = tFramework.rxFreeRingNum;
    UdmaEventPrms_init(&eventPrms);
    eventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
    eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
    eventPrms.chHandle          = tFramework.gRxChHandle[1];
    eventPrms.masterEventHandle = NULL;
    eventPrms.eventCb           = &framework_rxIsrFxn;
    eventPrms.appData           = (void *)(uintptr_t)intArg;
    retVal = Udma_eventRegister(tFramework.gDrvHandle, tFramework.gRxEvtHandle, &eventPrms);
    if (retVal != UDMA_SOK)
    {
        SALog("error in Udma_eventRegister()  \n");
        System_flush();
    }

#if defined (AM64X_USE_DEFAULT_FLOW)
    /* Nothing to do since flow is already setup earlier */
    /* Dual ring mode, the same is completion ring */
    /* Update the Rx Ring numbers */
    ringHandle                    = Udma_chGetCqRingHandle(tFramework.gRxChHandle[0]);
    tFramework.gRxRingHandle          = ringHandle;
    tFramework.rxComplRingNum     = Udma_ringGetNum(ringHandle);;

    /* Register Ring complete Isr */
    tFramework.gRxEvtHandle = &gUdmaEvtObj0;

    /* Initialize event parameters */
    intArg                      = tFramework.rxComplRingNum;
    UdmaEventPrms_init(&eventPrms);
    eventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
    eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
    eventPrms.chHandle          = tFramework.gRxChHandle[0];
    eventPrms.masterEventHandle = NULL;
    eventPrms.eventCb           = &framework_rxIsrFxn0;
    eventPrms.appData           = (void *)(uintptr_t)intArg;
    retVal = Udma_eventRegister(tFramework.gDrvHandle, tFramework.gRxEvtHandle, &eventPrms);
    if (retVal != UDMA_SOK)
    {
        SALog("error in Udma_eventRegister()  \n");
        System_flush();
    }
#endif
#else
    if (UDMA_SOK == retVal)
    {
        /* Update the Rx Ring numbers */
        ringHandle                    = Udma_chGetFqRingHandle(tFramework.gRxChHandle[1]);
        tFramework.gRxFreeRingHandle  = ringHandle;
        tFramework.rxFreeRingNum      = Udma_ringGetNum(ringHandle);
    
        ringHandle                    = Udma_chGetCqRingHandle(tFramework.gRxChHandle[1]);
        tFramework.gRxRingHandle      = ringHandle;
        tFramework.rxComplRingNum     = Udma_ringGetNum(ringHandle);

        /* Update the flow configuration to be used for both SA2UL Rx channels */
        /* Update the Rx Flow to be used for SA2UL */
        UdmaFlowPrms_init(&flowPrms, UDMA_CH_TYPE_RX);
        flowPrms.einfoPresent     = TRUE;
        flowPrms.psInfoPresent    = TRUE;
        flowPrms.errorHandling    = TRUE;
        flowPrms.descType         = CSL_UDMAP_DESC_TYPE_HOST;
        flowPrms.psLocation       = CSL_UDMAP_PS_LOC_DESC;
        flowPrms.defaultRxCQ      = tFramework.rxComplRingNum;
        flowPrms.srcTagLo         = 0;
        flowPrms.srcTagLoSel      = 4;
        flowPrms.srcTagHi         = 0;
        flowPrms.srcTagHiSel      = 2;
        flowPrms.destTagLo        = 0;
        flowPrms.destTagLoSel     = 4;
        flowPrms.destTagHi        = 0;
        flowPrms.destTagHiSel     = 5;

        /* Use the same free queue as default flow is not used in
         * selecting different queues based on threshold */
        flowPrms.fdq0Sz0Qnum    = tFramework.rxFreeRingNum;
        flowPrms.fdq0Sz1Qnum    = tFramework.rxFreeRingNum;
        flowPrms.fdq0Sz2Qnum    = tFramework.rxFreeRingNum;
        flowPrms.fdq0Sz3Qnum    = tFramework.rxFreeRingNum;
        flowPrms.fdq1Qnum       = tFramework.rxFreeRingNum;
        flowPrms.fdq2Qnum       = tFramework.rxFreeRingNum;
        flowPrms.fdq3Qnum       = tFramework.rxFreeRingNum;

        /* Update the created default flow with above configurations for sa2ul */
        retVal = Udma_flowConfig(flowHandle, 0U, &flowPrms);
        if (retVal != UDMA_SOK)
        {
            SALog("error in Flow Config Udma_flowConfig()  \n");
            System_flush();
        }

        /* Register Ring complete Isr */
        tFramework.gRxEvtHandle = &gUdmaEvtObj;

        /* Initialize event parameters */
        intArg                      = tFramework.rxComplRingNum;
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
        eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        eventPrms.chHandle          = tFramework.gRxChHandle[1];
        eventPrms.masterEventHandle = NULL;
        eventPrms.eventCb           = &framework_rxIsrFxn;
        eventPrms.appData           = (void *)(uintptr_t)intArg;
        retVal = Udma_eventRegister(tFramework.gDrvHandle, tFramework.gRxEvtHandle, &eventPrms);
        if (retVal != UDMA_SOK)
        {
            SALog("error in Udma_eventRegister()  \n");
            System_flush();
        }

    }
#endif
    /* Enable the channel after everything is setup */
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chEnable(tFramework.gRxChHandle[1]);
        if (UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(tFramework.gRxChHandle[0]);
        }

        if (retVal != UDMA_SOK)
        {
            SALog("error in Udma_chEnable()  \n");
            System_flush();
        }

    }
    return (retVal);

}

/* Pause/Resume Tx DMA Channel for sa2ul */
int salld_test_controlTxDma(uint32_t ctrl)
{
    int ret;
#if defined (__aarch64__)
    __asm    ("    isb                         ;");
     CSL_a53v8DsbSy();
#endif

#if defined (BUILD_MCU)
    CSL_armR5Dsb();
#endif

    if (ctrl == SA2UL_UDMAP_TX_PAUSE)
    {
        ret = Udma_chPause(tFramework.gTxChHandle);
    }
    else
    {
        ret = Udma_chResume(tFramework.gTxChHandle);
    }

#if defined (__aarch64__)
        __asm    ("    isb                         ;");
        CSL_a53v8DsbSy();
#endif

#if defined (BUILD_MCU)
        CSL_armR5Dsb();
#endif

    return (ret);
}

//=============================================================================
//  RingPush
//  Copy packet descriptor ptr to the specified ring's next free entry
//  (pass-by-reference) and commit it.
//=============================================================================
void RingPush( Udma_RingHandle ringHandle, uint32_t pktSize, physptr_t ptr )
{
#if defined (TEST_CORE_CACHE_COHERENT) || defined (ACP_COHERENCY)
            /* No cache operations are needed */
#else
    const void *virtBufPtr;
#endif
    physptr_t  physDescPtr;
    int32_t    retVal;
    FW_CPPI_DESC_T *pDesc = (FW_CPPI_DESC_T *) ptr;
    if ( (pDesc == NULL) || (ringHandle == NULL) )
    {
        return;
    }

    pDesc->hostDesc.bufPtr = (uint64_t) Osal_VirtToPhys ((void *)(uintptr_t)pDesc->hostDesc.bufPtr);
    pDesc->hostDesc.orgBufPtr = (uint64_t) Osal_VirtToPhys ((void *)(uintptr_t)pDesc->hostDesc.orgBufPtr);
    physDescPtr = (uint64_t) Osal_VirtToPhys ((void *)&pDesc->hostDesc);

#if defined (ACP_COHERENCY)
    physDescPtr = CSL_pktdmaMakeAselAddr((uint64_t) physDescPtr, \
                                CSL_LCDMA_RINGACC_ASEL_ENDPOINT_ACP_WR_ALLOC);
    pDesc->hostDesc.bufPtr = CSL_pktdmaMakeAselAddr((uint64_t) pDesc->hostDesc.bufPtr, \
                            CSL_LCDMA_RINGACC_ASEL_ENDPOINT_ACP_WR_ALLOC);

#endif

#if defined (TEST_CORE_CACHE_COHERENT) || defined (ACP_COHERENCY)
            /* No cache operations are needed */
#else
    /* Wb Invdata cache */
    CacheP_wbInv((const void *)&pDesc->hostDesc, TF_SIZE_DESC);
    virtBufPtr = (const void *)(uintptr_t)pDesc->hostDesc.bufPtr;
    CacheP_wbInv(virtBufPtr, pktSize);
#endif
    retVal  = Udma_ringQueueRaw(ringHandle,(uint64_t)physDescPtr);

    if (retVal != UDMA_SOK)
    {
        while(1);
    }

    return;
}

//=============================================================================
//  RingPop
//  Return the next packet descriptor ptr (if available) from the specified
//  ring and acknowledge it.
//=============================================================================
int32_t RingPop( Udma_RingHandle ringHandle, FW_CPPI_DESC_T **pAppDesc )
{
    uint64_t pDesc = 0;
    int32_t  retVal = 0;
    FW_CPPI_DESC_T *pVirtHostDesc;
#if defined (TEST_CORE_CACHE_COHERENT) || defined (ACP_COHERENCY)
    /* No cache operations are needed */
#else
    uint32_t pktsize;
#endif
    if ((pAppDesc == (FW_CPPI_DESC_T **)NULL) || (ringHandle == (Udma_RingHandle)NULL))
    {
        return -1; /* NULL not allowed */
    }


    Udma_ringDequeueRaw(ringHandle, &pDesc);

#if defined (ACP_COHERENCY)
    pDesc = CSL_pktdmaClrAselInAddr((uint64_t) pDesc);
#endif

    if(pDesc == 0)
    {
        *pAppDesc = (FW_CPPI_DESC_T *)NULL;
        retVal = -1;
    }
    else
    {
        *pAppDesc = pVirtHostDesc = (FW_CPPI_DESC_T *)Osal_PhysToVirt(pDesc);
#if defined (TEST_CORE_CACHE_COHERENT) || defined (ACP_COHERENCY)
                /* No cache operations are needed */
#else
        CacheP_Inv((const void *) &pVirtHostDesc->hostDesc, TF_SIZE_DESC);
#endif
        pVirtHostDesc->hostDesc.bufPtr = (uint64_t)Osal_PhysToVirt(pVirtHostDesc->hostDesc.bufPtr);
        pVirtHostDesc->hostDesc.orgBufPtr = (uint64_t)Osal_PhysToVirt(pVirtHostDesc->hostDesc.orgBufPtr);

#if defined (ACP_COHERENCY)
        /*clear any ASEL bits */
        pVirtHostDesc->hostDesc.bufPtr = CSL_pktdmaClrAselInAddr((uint64_t)pVirtHostDesc->hostDesc.bufPtr);
#endif

#if defined (TEST_CORE_CACHE_COHERENT) || defined (ACP_COHERENCY)
        /* No cache operations are needed */
#else
        pktsize  = CSL_FEXT (pVirtHostDesc->hostDesc.descInfo, UDMAP_CPPI5_PD_DESCINFO_PKTLEN);
        CacheP_Inv((const void *)(uintptr_t)pVirtHostDesc->hostDesc.bufPtr, (int32_t)pktsize);
#endif
     }

     return (retVal);
}

/** ============================================================================
 *   @n@b fwTx_ready_push
 *
 *   @b Description
 *   @n This functions puts TX descriptors into txReadyDescs
 *
 *   @param[in]
 *   @n None
 *
 *   @return    none
 * =============================================================================
 */
static void
fwTx_ready_push
(
   uint32_t size,
   physptr_t phys
)
{
    FW_CPPI_DESC_T *pCppiDesc = (FW_CPPI_DESC_T *)Osal_PhysToVirt (phys);

    pCppiDesc->nextPtr      = (uint64_t) (uintptr_t) tFramework.txReadyDescs;
    tFramework.txReadyDescs =  pCppiDesc;
} /* fwTx_ready_push */

void setup_cppi5InitHostDescQueueTx(uint32_t retqIdx, uint32_t descCnt, void (*pfPush)(uint32_t, physptr_t)  )
{
    uint8_t         *pBuffer;
    CSL_UdmapCppi5HMPD   *pDesc;
    FW_CPPI_DESC_T *pCppiDesc;
    uint32_t        i;

    for(i=0; i<descCnt; i++)
    {
        pBuffer                = &memBufRamTx[i][0];
        pCppiDesc              = (FW_CPPI_DESC_T *) &memDescRamTx[i];
        pDesc                  = (CSL_UdmapCppi5HMPD *) &pCppiDesc->hostDesc;

        /* setup the descriptor */
        memset(pCppiDesc, 0, sizeof(FW_CPPI_DESC_T));
        CSL_udmapCppi5SetDescType(pDesc, CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST);
#if defined (DMA_TYPE_LCDMA)
#else
        CSL_udmapCppi5SetReturnPolicy( pDesc,
                                       CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                       retqIdx);
#endif
        pDesc->bufPtr          = (uint64_t) (uintptr_t) Osal_VirtToPhys(pBuffer);
        pDesc->bufInfo1        = TF_DESC_BUFSIZE;
#if defined (DMA_TYPE_LCDMA)
        /* On AM64X (LCDMA), there is no original buffer fields */
#else
        pDesc->orgBufLen       = TF_DESC_BUFSIZE;
        pDesc->orgBufPtr       = pDesc->bufPtr;
#endif

#if defined (TEST_CORE_CACHE_COHERENT)
                /* No cache operations are needed */
#else
        /* make sure the descriptor is written back to memory for coherancy */
        CacheP_wbInv(pCppiDesc, sizeof(FW_CPPI_DESC_T));
#endif
        pfPush( 0, (physptr_t)Osal_VirtToPhys(pCppiDesc));
    }
}

/** ============================================================================
 *   @n@b emac_rx_free_push
 *
 *   @b Description
 *   @n This function attaches buffers to rx free descriptors
 *
 * =============================================================================
 */
static void
fwRx_free_push
(
   uint32_t size,
   physptr_t phys
)
{
    uint32_t        pktLen;
    CSL_UdmapCppi5HMPD *pDesc = (CSL_UdmapCppi5HMPD *)Osal_PhysToVirt (phys);

    pktLen = CSL_udmapCppi5GetPktLen((void *) pDesc);
    /* Push descriptor to Rx free descriptor queue */
    RingPush (tFramework.gRxFreeRingHandle, pktLen,(physptr_t) pDesc);

} /* fwRx_free_push */

#if defined (AM64X_USE_DEFAULT_FLOW)
static void
fwRx_free_push0
(
   uint32_t size,
   physptr_t phys
)
{
    uint32_t        pktLen;
    CSL_UdmapCppi5HMPD *pDesc = (CSL_UdmapCppi5HMPD *)Osal_PhysToVirt (phys);

    pktLen = CSL_udmapCppi5GetPktLen((void *) pDesc);

    /* Push descriptor to Rx free descriptor queue */
    RingPush (tFramework.gRxRingHandle, pktLen,(physptr_t) pDesc);

} /* fwRx_free_push0 */

#endif


void setup_cppi5InitHostDescQueueRx (uint32_t retqIdx, uint32_t start, uint32_t descCnt, uint32_t buffSize, void (*pfPush)(uint32_t, physptr_t)  )
{
    uint8_t         *pBuffer;
    CSL_UdmapCppi5HMPD   *pDesc;
    uint32_t        i;

    for(i=start; i<descCnt; i++)
    {
        pBuffer                = &memBufRamRx[i][0];
        pDesc                  = (CSL_UdmapCppi5HMPD *)&memDescRamRx[i][0];

        /* setup the descriptor */
        memset(pDesc, 0, sizeof(CSL_UdmapCppi5HMPD));
        CSL_udmapCppi5SetDescType(pDesc, CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST);
#if defined (DMA_TYPE_LCDMA)
#else
        CSL_udmapCppi5SetReturnPolicy( pDesc, CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                       retqIdx);
#endif
        pDesc->bufPtr          = (uint64_t) (uintptr_t) Osal_VirtToPhys(pBuffer);
        pDesc->bufInfo1        = buffSize;
#if defined (DMA_TYPE_LCDMA)
        /* On AM64X (LCDMA), there is no original buffer fields */
#else
        pDesc->orgBufLen       = buffSize;
        pDesc->orgBufPtr       = pDesc->bufPtr;
#endif
        pfPush(0, (physptr_t)Osal_VirtToPhys(pDesc));
    }
}

int initNavss(void)
{
    int32_t  retVal = UDMA_SOK;

    /* Initialize UDMA */
    retVal = frameworkUdmaInitDrv();

    if (retVal != UDMA_SOK)
    {
        SALog("error in creating the udma drv handle \n");
        System_flush();
        return (-1);
    }

    /* Create a Tx Channel */
    retVal = frameworkUdmaSetupTxChannel();
    if (retVal != UDMA_SOK)
    {
        SALog("error in creating the udma tx channel \n");
        System_flush();
        return (-1);
    }

    /* Create the descriptor pool for the Tx Ring */
    setup_cppi5InitHostDescQueueTx(tFramework.txComplRingNum, TF_NUM_DESC, fwTx_ready_push);

    /* Create the Rx channel for SA */
    retVal = frameworkUdmaSetupRxChannel();
    if (retVal != UDMA_SOK)
    {
        SALog("error in creating the udma rx channels \n");
        System_flush();
        return (-1);
    }

    /* Create the descriptor pool for the Rx ring */
    if (retVal == UDMA_SOK)
    {
#if defined(AM64X_USE_DEFAULT_FLOW)
    setup_cppi5InitHostDescQueueRx (tFramework.rxFreeRingNum, 0,
                                    TF_NUM_DESC/2,
                                    TF_DESC_BUFSIZE, fwRx_free_push  );
    setup_cppi5InitHostDescQueueRx (tFramework.rxComplRingNum, TF_NUM_DESC/2,
                                    TF_NUM_DESC,
                                    TF_DESC_BUFSIZE, fwRx_free_push0  );
#else
    setup_cppi5InitHostDescQueueRx (tFramework.rxFreeRingNum, 0,
                                    TF_NUM_DESC,
                                    TF_DESC_BUFSIZE, fwRx_free_push  );
#endif
    }

    return(retVal);
}
#endif

#if defined (NSS_LITE2)
int setupNavss(void)
{
    /*
     * Only perform Navss init if we have not decided to bypass the test. In
     * case a bypass was decided, do not fail on Navss init so that other tests
     * can still run.
     */
    if (testCommonGetTestStatus(saDataModeTest) == SA_TEST_NOT_RUN)
    {
        if (initNavss())  {
            SALog ("initNavss: setupNavss failed\n");
            return (-1);
        }
    }
    return(0);
}
#endif

/* The QM/CPDMA are setup */
int initQm (void)
{
#ifdef NSS_LITE2
   return (setupNavss());

#else
  if (setupQmMem())  {
  	SALog ("initQm: setupQmMem failed\n");
    return (-1);
  }

  if (setupPassQmMem())  {
  	SALog ("initQm: setupPassQmMem failed\n");
    return (-1);
  }

  if (setupCpdma ())  {
  	SALog ("initQm: setupCpdma failed\n");
    return (-1);
  }

  if (setupQueues ())  {
  	SALog ("initQm: setupQueues failed\n");
    return (-1);
  }

  if (setupFlows ())  {
  	SALog ("initQm: setupFlows failed\n");
    return (-1);
  }
  return (0);
#endif

}

/* clean up for QM/CPPI */
static int exitQm(void)
{
#ifdef NSS_LITE2

#else
  if (closeFlows ())  {
  	SALog ("exitQm: closeFlows failed\n");
    return (-1);
  }

  if (closeQueues ())  {
  	SALog ("exitQm: closeQueues failed\n");
    return (-1);
  }

  if (closeCpdma ())  {
  	SALog ("exitQm: closeCpdma failed\n");
    return (-1);
  }

  if (closeQmMem())  {
	  	SALog ("exitQm: closeQmMem failed\n");
	    return (-1);
  }
#endif

  return 0;


}


/* Two semaphores are used to gate access to the PA handle tables */
int initSems (void)
{
    SemaphoreP_Params params;

    SemaphoreP_Params_init (&params);
    params.mode = SemaphoreP_Mode_BINARY;
    tFramework.tfSaSem 		  = SemaphoreP_create (1, &params);
    return (0);
}

/* Two semaphores are used to gate access to the PA handle tables */
static int deleteSems (void)
{
    SemaphoreP_delete (tFramework.tfSaSem);
    return (0);
}

/***************************************************************************************
 * FUNCTION PURPOSE: Power up PA subsystem
 ***************************************************************************************
 * DESCRIPTION: this function powers up the PA subsystem domains
 ***************************************************************************************/
void passPowerUp (void)
{
    /* PASS power domain is turned OFF by default. It needs to be turned on before doing any
     * PASS device register access. This not required for the simulator. */

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    /* Set PASS Power domain to ON */
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_NETCP);

    /* Enable the clocks for PASS modules */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_PA, PSC_MODSTATE_ENABLE);
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_CPGMAC,  PSC_MODSTATE_ENABLE);
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_SA,  PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_NETCP);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_NETCP));

#if defined(DEVICE_K2L) || defined(SOC_K2L)
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_OSR);

    /* Enable the clocks for OSR modules */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_OSR, PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_OSR);
    
    /* Wait until the state transition process is completed. */
    utilCycleDelay (1000);
#endif    
    
    
#else

#if !defined(NSS_LITE2)
    /* Set NSS Power domain to ON */        
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_NSS);

    /* Enable the clocks for NSS modules */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_NSS, PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_NSS);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_NSS));
    
    /* Set SA Power domain to ON */        
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_SA);

    /* Enable the clocks for SA modules */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_SA, PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_SA);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_SA));
#endif

#endif    
    
}

/* Initialize the test framework */
int setupTestFramework (void)
{
    uint8_t limitAccess = false;

    /* Setup the semaphores used for access to the PA tables.
     * This has to be done before the PA is initialized */
    if (initSems())  {
        SALog ("setupTestFramework: initQm returned error, exiting\n");
        return (-1);
    }

    /* Power up PA sub-systems */
    passPowerUp();

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
	/* Create the PA driver instance */
	if (initPa())  {
		SALog ("setupTestFramework: initPa returned error, exiting\n");
		return (-1);
	}
#endif

#if defined(NSS_LITE2)
    /*
     * Check SA resource availability if running on HS device and configure
     * which tests and test parameters are enabled.
     */
    limitAccess = configSaRes();
#endif

	/* Setup the QM with associated buffers and descriptors */
	if (initQm())  {
		SALog ("setupTestFramework: initQm returned error, exiting\n");
		return (-1);
	}

    /* Initialize the SA unit test support and create the SA driver instance */
    salld_sim_initialize(limitAccess);

    /* Initialize the test connection module */
#if !defined(NSS_LITE2)
    sauConnInit();
#endif

	return (0);

}

int exitTestFramework(void)
{

    salld_sim_close_sa();

	/* Delete the semaphores created */
	if (deleteSems())  {
		SALog ("exitTestFramework: deleteSems returned error, exiting\n");
		return (-1);
	}

	/* Clean up CPPI/QM entries */
	if (exitQm()) {
		SALog ("exitTestFramework: exitQm returned error, exiting\n");
		return (-1);
	}

	return 0;
}


/* Check that all the queues are setup correctly */
#ifdef NSS_LITE2
int verifyTestFramework (void)
{
  return(0);
}
#else
int verifyTestFramework (void)
{
	int i, j;
	int count;
	int returnVal = 0;
	Cppi_HostDesc *hd;
	uint8_t *bufp;
	uint32_t bufLen;

	int32_t linkedQ[8];
    #ifdef NETSS_INTERNAL_PKTDMA
	int32_t linkedLocQ[4];
    #endif

	int32_t nbufs[]  = { TF_LINKED_BUF_Q1_NBUFS,    TF_LINKED_BUF_Q2_NBUFS,    TF_LINKED_BUF_Q3_NBUFS,    TF_LINKED_BUF_Q4_NBUFS,
                       TF_HOST_LINKED_BUF_Q1_NBUFS,    TF_HOST_LINKED_BUF_Q2_NBUFS,    TF_HOST_LINKED_BUF_Q3_NBUFS,    TF_HOST_LINKED_BUF_Q4_NBUFS};
	int32_t bSize[]  = { TF_LINKED_BUF_Q1_BUF_SIZE, TF_LINKED_BUF_Q2_BUF_SIZE, TF_LINKED_BUF_Q3_BUF_SIZE, TF_LINKED_BUF_Q4_BUF_SIZE,
                       TF_HOST_LINKED_BUF_Q1_BUF_SIZE, TF_HOST_LINKED_BUF_Q2_BUF_SIZE, TF_HOST_LINKED_BUF_Q3_BUF_SIZE, TF_HOST_LINKED_BUF_Q4_BUF_SIZE };

	linkedQ[0] = tFramework.QLinkedBuf1;
	linkedQ[1] = tFramework.QLinkedBuf2;
	linkedQ[2] = tFramework.QLinkedBuf3;
	linkedQ[3] = tFramework.QLinkedBuf4;
	linkedQ[4] = tFramework.QHostLinkedBuf1;
	linkedQ[5] = tFramework.QHostLinkedBuf2;
	linkedQ[6] = tFramework.QHostLinkedBuf3;
	linkedQ[7] = tFramework.QHostLinkedBuf4;

    #ifdef NETSS_INTERNAL_PKTDMA
	linkedLocQ[0] = tFramework.QLocLinkedBuf1;
	linkedLocQ[1] = tFramework.QLocLinkedBuf2;
	linkedLocQ[2] = tFramework.QLocLinkedBuf3;
	linkedLocQ[3] = tFramework.QLocLinkedBuf4;
    #endif

	/* Verify that all of the general purpose queues are empty */
	for (i = 0; i < TF_NUM_GEN_QUEUES; i++)  {
		if ((count = Qmss_getQueueEntryCount (tFramework.QGen[i])) != 0)  {
			SALog ("verifyTestFramework: Expected 0 entry count for queue %d, found %d entries\n", tFramework.QGen[i], count);
			returnVal = -1;
		}
	}

	/* Verify that the number of descriptors in the free descriptor queue is correct */
	count = Qmss_getQueueEntryCount (tFramework.QfreeDesc);
	if (count != (TF_NUM_DESC - TF_NUM_RES_DESC))  {
		SALog ("verifyTestFramework: Expected %d entry count in the free descriptor queue (%d), found %d\n",
						TF_NUM_DESC - TF_NUM_RES_DESC,
						tFramework.QfreeDesc, count);
		returnVal = -1;
	}

	/* Verify the number and sizing of descriptors with linked buffers in the three queues */
	for (j = 0; j < 8; j++)  {

		count = Qmss_getQueueEntryCount (linkedQ[j]);
		if (count != nbufs[j])  {
		SALog ("verifyTestFramework: Expected %d entry count in linked buffer queue 1 (%d), found %d\n",
						nbufs[j], linkedQ[j], count);
		returnVal = -1;
		}

		for (i = 0; i < count; i++)  {
			hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (linkedQ[j])) & ~15);
			Cppi_getOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, &bufp, &bufLen);
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
			Qmss_queuePush (linkedQ[j], (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#else
      Qmss_queuePushDesc (linkedQ[j], (Ptr)hd);
#endif
			if (bufLen != bSize[j])  {
				SALog ("verifyTestFramework: Linked buffer queue %d (%d) expected orignal length of %d, found %d\n",
								j, linkedQ[j], bSize[j], bufLen);
				returnVal = -1;
				break;
			}
		}
	}

  #ifdef NETSS_INTERNAL_PKTDMA


	/* Verify that the number of descriptors in the free descriptor queue is correct */
	count = Qmss_getQueueEntryCount (tFramework.QLocfreeDesc);
	if (count != (TF_NUM_DESC - TF_LINKED_BUF_Q1_NBUFS - TF_LINKED_BUF_Q2_NBUFS - TF_LINKED_BUF_Q3_NBUFS - TF_LINKED_BUF_Q4_NBUFS))  {
		SALog ("verifyTestFramework: Expected %d entry count in the free descriptor queue (%d), found %d\n",
						TF_NUM_DESC - TF_LINKED_BUF_Q1_NBUFS - TF_LINKED_BUF_Q2_NBUFS - TF_LINKED_BUF_Q3_NBUFS - TF_LINKED_BUF_Q4_NBUFS,
						tFramework.QLocfreeDesc, count);
		returnVal = -1;
	}



	/* Verify the number and sizing of descriptors with linked buffers in the three queues */
	for (j = 0; j < 4; j++)  {

		count = Qmss_getQueueEntryCount (linkedLocQ[j]);
		if (count != nbufs[j])  {
		SALog ("verifyTestFramework: Expected %d entry count in Loc linked buffer queue %d (%d), found %d\n",
						nbufs[j], j, linkedQ[j], count);
		returnVal = -1;
		}

		for (i = 0; i < count; i++)  {
			hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (linkedLocQ[j])) & ~15);
			Cppi_getOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, &bufp, &bufLen);
            //Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
            Qmss_queuePushDesc(linkedLocQ[j], (Ptr)hd);

			if (bufLen != bSize[j])  {
				SALog ("verifyTestFramework: Linked buffer queue %d (%d) expected orignal length of %d, found %d\n",
								j, linkedQ[j], bSize[j], bufLen);
				returnVal = -1;
				break;
			}
		}
	}

    #endif


	return (returnVal);
}
#endif


#ifndef USE_RTOS
void Task_exit(void)
{
    while (TRUE)
    {

    }
}
#endif


