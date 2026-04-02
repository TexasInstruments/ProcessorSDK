/*
 *
 * Copyright (C) 2010-2013 Texas Instruments Incorporated - http://www.ti.com/ 
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
 * for every test in the SA basic example.
 * 
 */

#include "unittest.h"

#include <ti/drv/pa/pa_osal.h>
#include <ti/csl/cslr_device.h>
#include <ti/csl/csl_psc.h>
#include <ti/csl/csl_pscAux.h>

/* CSL CHIP, SEM Functional layer includes */
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_semAux.h>

/* Firmware images */
#include <ti/drv/pa/fw/pafw.h>

/* QMSS device specific configuration */
extern Qmss_GlobalConfigParams  qmssGblCfgParams;

/* CPPI device specific configuration */
extern Cppi_GlobalConfigParams  cppiGblCfgParams;

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

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
uint32_t      coreKey [MAX_NUM_CORES];
uint32_t      qmssMallocCounter   = 0;
uint32_t      qmssFreeCounter     = 0;
uint32_t      cppiMallocCounter   = 0;
uint32_t      cppiFreeCounter     = 0;

/**
 * @brief  This macro is used to alert the application that the PA is
 *         going to access table memory. The application must ensure
 *         cache coherency and semaphores for multi-core applications
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
 *        protection of different memory tables. The multicore semaphore
 *        should be allocated only for the first call of a nested group 
 *        of calls. 
 */

void Osal_paBeginMemAccess (Ptr addr, uint32_t size)
{
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
 *        protection of different memory tables. The multicore semaphore
 *        should be freed when all previous memory access has completed,
 *        in other words, when the nested call level reaches 0.
 */
void Osal_paEndMemAccess   (Ptr addr, uint32_t size)
{
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
    /* Get the hardware semaphore. 
     *
     * Acquire Multi core QMSS synchronization lock 
     */
    while ((CSL_semAcquireDirect (QMSS_HW_SEM)) == 0);

    /* Disable all interrupts and OS scheduler. 
     *
     * Acquire Multi threaded / process synchronization lock.
     */
    coreKey [CSL_chipReadReg(CSL_CHIP_DNUM)] = Hwi_disable();

    return NULL;
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
    /* Enable all interrupts and enables the OS scheduler back on.
     *
     * Release multi-threaded / multi-process lock on this core.
     */
    Hwi_restore(coreKey [CSL_chipReadReg(CSL_CHIP_DNUM)]);

    /* Release the hardware semaphore 
     *
     * Release multi-core lock.
     */ 
    CSL_semReleaseSemaphore (QMSS_HW_SEM);
  
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
    /* Get the hardware semaphore. 
     *
     * Acquire Multi core CPPI synchronization lock 
     */
    while ((CSL_semAcquireDirect (CPPI_HW_SEM)) == 0);

    /* Disable all interrupts and OS scheduler. 
     *
     * Acquire Multi threaded / process synchronization lock.
     */
    coreKey [CSL_chipReadReg(CSL_CHIP_DNUM)] = Hwi_disable();

    return NULL;
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
    /* Enable all interrupts and enables the OS scheduler back on.
     *
     * Release multi-threaded / multi-process lock on this core.
     */
    Hwi_restore(coreKey [CSL_chipReadReg(CSL_CHIP_DNUM)]);

    /* Release the hardware semaphore 
     *
     * Release multi-core lock.
     */ 
    CSL_semReleaseSemaphore (CPPI_HW_SEM);
    
    return;
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
Ptr Osal_cppiMalloc (uint32_t num_bytes)
{
	Error_Block	errorBlock;
    Ptr dataPtr;

    /* Increment the allocation counter. */
    cppiMallocCounter++;	

	/* Allocate memory. */
    dataPtr = Memory_alloc(NULL, num_bytes, 0, &errorBlock);
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
    /* Disable all interrupts and OS scheduler. 
     *
     * Acquire Multi threaded / process synchronization lock.
     */
    //coreKey [CSL_chipReadReg (CSL_CHIP_DNUM)] = Hwi_disable();

    return NULL;
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
    /* Enable all interrupts and enables the OS scheduler back on.
     *
     * Release multi-threaded / multi-process lock on this core.
     */
    //Hwi_restore(key);

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
void Osal_cppiBeginMemAccess (void *blockPtr, uint32_t size)
{
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
    
    return;
}


Int downloadPaFirmware (void)
{
  
    int ret = pa_OK, i;
	uint32_t  version;

	if (no_bootMode == FALSE)
		return 0;

	Pa_resetControl (tFramework.passHandle, pa_STATE_RESET);
    
#ifndef NSS_GEN2  
  
    /* PDPSs 0-2 use image c1_x */
    Pa_downloadImage (tFramework.passHandle, 0, (Ptr)c1_0, c1_0Size);
    Pa_downloadImage (tFramework.passHandle, 1, (Ptr)c1_1, c1_1Size);
    Pa_downloadImage (tFramework.passHandle, 2, (Ptr)c1_2, c1_2Size);

    /* PDSP 3 uses image c2 */
    Pa_downloadImage (tFramework.passHandle, 3, (Ptr)c2, c2Size);

    /* PDSPs 4-5 use image m */
    for (i = 4; i < 6; i++)
        Pa_downloadImage (tFramework.passHandle, i, (Ptr)m, mSize);

#else

	Pa_downloadImage (tFramework.passHandle, 0, (Ptr)in0_pdsp0, in0_pdsp0Size);
	Pa_downloadImage (tFramework.passHandle, 1, (Ptr)in0_pdsp1, in0_pdsp1Size);
	Pa_downloadImage (tFramework.passHandle, 2, (Ptr)in1_pdsp0, in1_pdsp0Size);
	Pa_downloadImage (tFramework.passHandle, 3, (Ptr)in1_pdsp1, in1_pdsp1Size);
	Pa_downloadImage (tFramework.passHandle, 4, (Ptr)in2_pdsp0, in2_pdsp0Size);
	Pa_downloadImage (tFramework.passHandle, 5, (Ptr)in3_pdsp0, in3_pdsp0Size);
	Pa_downloadImage (tFramework.passHandle, 6, (Ptr)in4_pdsp0, in4_pdsp0Size);
	Pa_downloadImage (tFramework.passHandle, 7, (Ptr)in4_pdsp1, in4_pdsp1Size);
	Pa_downloadImage (tFramework.passHandle, 8, (Ptr)post_pdsp0, post_pdsp0Size);
	Pa_downloadImage (tFramework.passHandle, 9, (Ptr)post_pdsp1, post_pdsp1Size);
	Pa_downloadImage (tFramework.passHandle, 10, (Ptr)eg0_pdsp0, eg0_pdsp0Size);
	Pa_downloadImage (tFramework.passHandle, 11, (Ptr)eg0_pdsp1, eg0_pdsp1Size);
	Pa_downloadImage (tFramework.passHandle, 12, (Ptr)eg0_pdsp2, eg0_pdsp2Size);
	Pa_downloadImage (tFramework.passHandle, 13, (Ptr)eg1_pdsp0, eg1_pdsp0Size);
	Pa_downloadImage (tFramework.passHandle, 14, (Ptr)eg2_pdsp0, eg2_pdsp0Size);

#endif
	ret = Pa_resetControl (tFramework.passHandle, pa_STATE_ENABLE);

	if (ret != pa_STATE_ENABLE)
	{
	  System_printf ("downloadPaFirmware: Pa_resetControl return with error code %d\n", ret);
	  System_flush();
	  return (-1);
	}

	for ( i = 0; i < TF_NUM_PASS_PDSPS; i++)
	{
	  Pa_getPDSPVersion(tFramework.passHandle, i, &version);
	  System_printf ("PDSP %d version = 0x%08x\n", i, version);
	  System_flush();
	}

	return (0);

}

/* The PA LLD instance is created, the PA firmware is
 * downloaded and started */
Int initPa (void)
{
  paSizeInfo_t  paSize;
  paConfig_t    paCfg;
  int           ret;
  int           sizes[pa_N_BUFS];
  int           aligns[pa_N_BUFS];
  void*         bases[pa_N_BUFS];

  /* The maximum number of handles that can exists are 32 for L2, and 64 for L3. */
  memset(&paSize, 0, sizeof(paSize));
  memset(sizes, 0, sizeof(sizes));
  memset(aligns, 0, sizeof(aligns));
  memset(bases, 0, sizeof(bases));
  memset(&paCfg, 0, sizeof(paCfg));
  paSize.nMaxL2 = TF_MAX_NUM_L2_HANDLES;
  paSize.nMaxL3 = TF_MAX_NUM_L3_HANDLES;
  paSize.nUsrStats = 0;

  ret = Pa_getBufferReq(&paSize, sizes, aligns);

  if (ret != pa_OK)  {
    System_printf ("initPa: Pa_getBufferReq() return with error code %d\n", ret);
    return (-1);
  }

  /* The first buffer is used as the instance buffer */
  if ((uint32_t)memPaInst & (aligns[0] - 1))  {
    System_printf ("initPa: Pa_getBufferReq requires %d alignment for instance buffer, but address is 0x%08x\n", aligns[0], (uint32_t)memPaInst);
    return (-1);
  }

  if (sizeof(memPaInst) < sizes[0])  {
    System_printf ("initPa: Pa_getBufferReq requires size %d for instance buffer, have only %d\n", sizes[0], sizeof(memPaInst));
    return (-1);
  }

  bases[0] = (void *)memPaInst;


  /* The second buffer is the L2 table */
  if ((uint32_t)memL2Ram & (aligns[1] - 1))  {
    System_printf ("initPa: Pa_getBufferReq requires %d alignment for buffer 1, but address is 0x%08x\n", aligns[1], (uint32_t)memL2Ram);
    return (-1);
  }

  if (sizeof(memL2Ram) <  sizes[1])  {
    System_printf ("initPa: Pa_getBufferReq requires %d bytes for buffer 1, have only %d\n", sizes[1], sizeof(memL2Ram));
    return (-1);
  }

  bases[1] = (void *)memL2Ram;

  /* The third buffer is the L3 table */
  if ((uint32_t)memL3Ram & (aligns[2] - 1))  {
    System_printf ("initPa: Pa_getBufferReq requires %d alignment for buffer 1, but address is 0x%08x\n", aligns[2], (uint32_t)memL3Ram);
    return (-1);
  }

  if (sizeof(memL3Ram) <  sizes[2])  {
    System_printf ("initPa: Pa_getBufferReq requires %d bytes for buffer 1, have only %d\n", sizes[2], sizeof(memL3Ram));
    return (-1);
  }

  bases[2] = (void *)memL3Ram;
  bases[3] = NULL;

  paCfg.initTable = TRUE;
#ifndef SIMULATOR_SUPPORT
  paCfg.initDefaultRoute = TRUE;
#ifdef __LINUX_USER_SPACE
    paCfg.rmServiceHandle = rmClientServiceHandle;
#else
    paCfg.rmServiceHandle = rmServiceHandle;
#endif /* __LINUX_USER_SPACE */
#endif  
  paCfg.baseAddr = CSL_NETCP_CFG_REGS;
  paCfg.sizeCfg = &paSize;
  
  ret = Pa_create (&paCfg, bases, &tFramework.passHandle);
  if (ret != pa_OK)  {
    System_printf ("initPa: Pa_create returned with error code %d\n", ret);
    return (-1);
  }

  /* Download the firmware */
  if (downloadPaFirmware ())
    return (-1);
 
   return (0);
   
}

Int setupQmMem (void)
{
  Qmss_InitCfg     qmssInitConfig;
  Qmss_MemRegInfo  memInfo;
  Cppi_DescCfg     descCfg;
  int32_t            result;
  int              n;

  memset (&qmssInitConfig, 0, sizeof (Qmss_InitCfg));
  memset (memDescRam,      0, sizeof (memDescRam));

  qmssInitConfig.linkingRAM0Base = 0;   /* Use internal RAM */
  qmssInitConfig.linkingRAM0Size = 0;
  qmssInitConfig.linkingRAM1Base = 0;
  qmssInitConfig.maxDescNum      = TF_NUM_DESC;
#ifndef SIMULATOR_SUPPORT
#ifdef __LINUX_USER_SPACE
    qmssGblCfgParams.qmRmServiceHandle = rmClientServiceHandle;
#else
    if (rmServiceHandle)
		qmssGblCfgParams.qmRmServiceHandle = rmServiceHandle;
#endif
#endif
  if (no_bootMode == FALSE)
	  qmssInitConfig.qmssHwStatus = QMSS_HW_INIT_COMPLETE;

  result = Qmss_init (&qmssInitConfig, &qmssGblCfgParams);
  if (result != QMSS_SOK)  {
    System_printf ("setupQmMem: qmss_Init failed with error code %d\n", result);
    return (-1);
  }

  result = Qmss_start();
  if (result != QMSS_SOK)  {
    System_printf ("setupQmMem: Qmss_start failed with error code %d\n", result);
    return (-1);
  }
  
  /* Setup a single memory region for descriptors */
  memset (memDescRam, 0, sizeof(memDescRam));
  memInfo.descBase       = (uint32_t *)utilgAddr((uint32_t)memDescRam);
  memInfo.descSize       = TF_SIZE_DESC;
  memInfo.descNum        = TF_NUM_DESC;
  memInfo.manageDescFlag = Qmss_ManageDesc_MANAGE_DESCRIPTOR;
  memInfo.memRegion      = Qmss_MemRegion_MEMORY_REGION0;
  memInfo.startIndex     = 0;

  result = Qmss_insertMemoryRegion (&memInfo);
  if (result < QMSS_SOK)  {
    System_printf ("setupQmMem: Qmss_insertMemoryRegion returned error code %d\n", result);
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

  descCfg.cfg.host.returnPolicy     = Cppi_ReturnPolicy_RETURN_ENTIRE_PACKET;
  descCfg.cfg.host.psLocation       = Cppi_PSLoc_PS_IN_DESC;

  tFramework.QfreeDesc = Cppi_initDescriptor (&descCfg, (uint32_t *)&n);

  if (n != descCfg.descNum)  {
    System_printf ("setupQmMem: expected %d descriptors to be initialized, only %d are initialized\n", descCfg.descNum, n);
    return (-1);
  }

  return (0);

}

int closeQmMem(void)
{
	Qmss_Result qmss_result;

    if ((qmss_result = Qmss_removeMemoryRegion (Qmss_MemRegion_MEMORY_REGION0, 0)) != QMSS_SOK)
    {
         System_printf ("Error : Remove memory region error code : %d\n", qmss_result);
         return qmss_result;
    }

    if ((qmss_result = Qmss_exit ()))
    {
        System_printf ("Error : exit error code : %d\n", qmss_result);
        return qmss_result;
    }

    return qmss_result;
}

Int setupCpdma (void)
{
  Cppi_CpDmaInitCfg cpdmaCfg;
  Cppi_RxChInitCfg  rxChCfg;
  Cppi_TxChInitCfg  txChCfg;

  int32_t result;
  int   i;
  uint8_t isAlloc;
#ifndef SIMULATOR_SUPPORT
  Cppi_StartCfg     cppiStartCfg;
#endif
  uint32_t cppi_pa_tx_ch_disable_list[TF_PA_NUM_TX_CPDMA_CHANNELS];
  uint32_t cppi_pa_rx_ch_disable_list[TF_PA_NUM_RX_CPDMA_CHANNELS];
  uint32_t disable_list_flag = 0;

  /* Clear the list by default */
  memset (cppi_pa_tx_ch_disable_list, 0, sizeof (cppi_pa_tx_ch_disable_list));
  memset (cppi_pa_rx_ch_disable_list, 0, sizeof (cppi_pa_rx_ch_disable_list));

  result = Cppi_init (&cppiGblCfgParams);
  if (result != CPPI_SOK)  {
    System_printf ("setupCpdma: cpp_Init returned error %d\n", result);
    return (-1);
  }
#ifndef SIMULATOR_SUPPORT
#ifdef __LINUX_USER_SPACE
	if (rmClientServiceHandle) {
	   /* Register RM with CPPI */
	   cppiStartCfg.rmServiceHandle = rmClientServiceHandle;
	   Cppi_startCfg(&cppiStartCfg);
	}
#else
	 if (rmServiceHandle)
	 {
        cppiStartCfg.rmServiceHandle = rmServiceHandle;
        Cppi_startCfg(&cppiStartCfg);
	 }
#endif /* __LINUX_USER_SPACE */
#endif /* SIMULATOR_SUPPORT */

  memset(&cpdmaCfg, 0, sizeof(Cppi_CpDmaInitCfg));
  cpdmaCfg.dmaNum           = Cppi_CpDma_PASS_CPDMA;

  tFramework.tfPaCppiHandle = Cppi_open (&cpdmaCfg);
  if (tFramework.tfPaCppiHandle == NULL)  {
    System_printf ("setupCpdma: cppi_Open returned NULL cppi handle\n");
    return (-1);
  }

  /* Open all 24 rx channels */
  rxChCfg.rxEnable = Cppi_ChState_CHANNEL_DISABLE;

  for (i = 0; i < TF_PA_NUM_RX_CPDMA_CHANNELS; i++)  {
    rxChCfg.channelNum        = i;
    tFramework.tfPaRxChHnd[i] = Cppi_rxChannelOpen (tFramework.tfPaCppiHandle, &rxChCfg, &isAlloc);

    if (tFramework.tfPaRxChHnd[i] == NULL)  {
        if (no_bootMode == TRUE)
        {
        	System_printf ("Error opening Rx channel: %d \n", rxChCfg.channelNum);
            return -1;
        }
        else
        {
        	cppi_pa_rx_ch_disable_list[i] = 1;
        	disable_list_flag = 1;
        	continue;
        }
    }
    
    Cppi_channelEnable (tFramework.tfPaRxChHnd[i]);
  }

  /* Print if there are any CPPI Rx channels that are not enabled by above code, presuming linux enabled it */
  if (disable_list_flag)
  {
  	System_printf ("Unable to open below cppi Rx channels...presuming linux has already enabled it \n");
		for (i = 0; i<TF_PA_NUM_RX_CPDMA_CHANNELS; i++)
		{
			 if (cppi_pa_rx_ch_disable_list[i])
				 System_printf ("%d ", i);
		}
		System_printf (" \n ");
		disable_list_flag = 0;
  }

  /* Open all 9 tx channels.  */
  txChCfg.priority     = 2;
  txChCfg.txEnable     = Cppi_ChState_CHANNEL_DISABLE;
  txChCfg.filterEPIB   = FALSE;
  txChCfg.filterPS     = FALSE;
  txChCfg.aifMonoMode  = FALSE;

  for (i = 0; i < TF_PA_NUM_TX_CPDMA_CHANNELS; i++)  {
    txChCfg.channelNum 	 	  = i;
    tFramework.tfPaTxChHnd[i] = Cppi_txChannelOpen (tFramework.tfPaCppiHandle, &txChCfg, &isAlloc);

    if (tFramework.tfPaTxChHnd[i] == NULL)  {
        if (no_bootMode == TRUE)
        {
    	   System_printf ("Error opening Tx channel %d\n", txChCfg.channelNum);
           return -1;
        }
        else
        {
        	cppi_pa_tx_ch_disable_list[i] = 1;
        	disable_list_flag = 1;
        	continue;
        }
    }
    
    Cppi_channelEnable (tFramework.tfPaTxChHnd[i]);
  }

  /* Print if there are any CPPI Tx channels that are not enabled by above code, presuming linux enabled it */
  if (disable_list_flag)
  {
  	System_printf ("Unable to open below cppi tx channels...presuming linux has already enabled it \n");
		for (i = 0; i< TF_PA_NUM_TX_CPDMA_CHANNELS; i++)
		{
			 if (cppi_pa_tx_ch_disable_list[i])
				 System_printf ("%d ", i);
		}
		System_printf (" \n ");
  }

  /* Clear CPPI Loobpack bit in PASS CDMA Global Emulation Control Register */
  Cppi_setCpdmaLoopback(tFramework.tfPaCppiHandle, 0);   

  return (0);

}

int closeCpdma(void)
{
	int i;
    Cppi_Result cppi_result;

	for (i = 0; i < TF_PA_NUM_TX_CPDMA_CHANNELS; i++)
	{
		 if (tFramework.tfPaTxChHnd[i] != NULL) {
    	     if ((cppi_result = Cppi_channelClose (tFramework.tfPaTxChHnd[i])) != CPPI_SOK) {
    	 	    return (cppi_result);
    	     }
		 }
	}

	for (i = 0; i < TF_PA_NUM_RX_CPDMA_CHANNELS; i++)
	{
		 if (tFramework.tfPaRxChHnd[i] != NULL) {
    	     if ((cppi_result = Cppi_channelClose (tFramework.tfPaRxChHnd[i])) != CPPI_SOK) {
    	 	    return (cppi_result);
    	     }
		 }
	}

	/* Close CPPI CPDMA instance */
	if ((cppi_result = Cppi_close (tFramework.tfPaCppiHandle)) != CPPI_SOK)
	{
		System_printf ("Error: Closing CPPI CPDMA error code : %d\n", cppi_result);
	 	return (cppi_result);
	}

	/* Deinitialize CPPI LLD */
	if ((cppi_result = Cppi_exit ()) != CPPI_SOK)
	{
		System_printf ("Error : Exiting CPPI error code : %d\n", cppi_result);
	 	return (cppi_result);
	}

 	return (cppi_result);
}

/* Setup all the queues used in the example */
Int setupQueues (void)
{
  int    i;
  uint8_t  isAlloc;
  
  Qmss_Queue q;
  Cppi_HostDesc *hd;

  
  /* The 10 PA transmit queues (corresponding to the 10 tx cdma channels */
  for (i = 0; i < TF_PA_NUM_TX_CPDMA_CHANNELS; i++)  {

    tFramework.QPaTx[i] = Qmss_queueOpen (Qmss_QueueType_PASS_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAlloc);

    if (tFramework.QPaTx[i] < 0)  {
      System_printf ("setupQueues: Qmss_queueOpen failed for PA transmit queue number %d\n", 640+i);
      return (-1);
    }
    
    Qmss_setQueueThreshold (tFramework.QPaTx[i], 1, 1);
    

  }
  
  /* The queues with attached buffers */
  tFramework.QLinkedBuf1 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LINKED_BUF_Q1, &isAlloc);
  
  if (tFramework.QLinkedBuf1 < 0)  {
  	System_printf ("setupQueues: Qmss_queueOpen failed for queue %d\n", TF_LINKED_BUF_Q1);
  	return (-1);
  }
  
  tFramework.QLinkedBuf2 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LINKED_BUF_Q2, &isAlloc);
  
  if (tFramework.QLinkedBuf2 < 0)  {
  	System_printf ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_LINKED_BUF_Q2);
  	return (-1);
  }
  
  tFramework.QLinkedBuf3 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LINKED_BUF_Q3, &isAlloc);
  
  if (tFramework.QLinkedBuf3 < 0)  {
  	System_printf ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_LINKED_BUF_Q3);
  	return (-1);
  }
  
  tFramework.QLinkedBuf4 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LINKED_BUF_Q4, &isAlloc);
  
  if (tFramework.QLinkedBuf4 < 0)  {
  	System_printf ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_LINKED_BUF_Q3);
  	return (-1);
  }
  
  
  /* Attach buffers to the queues and push them onto the queue */
  q.qMgr = 0;
  q.qNum = tFramework.QLinkedBuf1;
  
  for (i = 0; i < TF_LINKED_BUF_Q1_NBUFS; i++)   {
  	
    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QfreeDesc)) & ~15);
    if (hd == NULL)  {
      System_printf ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ1[i])), sizeof(memQ1[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ1[i])), sizeof(memQ1[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Qmss_queuePushDesc (tFramework.QLinkedBuf1, (Ptr)hd);

  }
  
  q.qNum = tFramework.QLinkedBuf2;
  
  for (i = 0; i < TF_LINKED_BUF_Q2_NBUFS; i++)   {
  	
    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QfreeDesc)) & ~15);
    if (hd == NULL)  {
      System_printf ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ2[i])), sizeof(memQ2[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ2[i])), sizeof(memQ2[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Qmss_queuePushDesc (tFramework.QLinkedBuf2, (Ptr)hd);

  }
  
  q.qNum = tFramework.QLinkedBuf3;
  
  for (i = 0; i < TF_LINKED_BUF_Q3_NBUFS; i++)   {
  	
    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QfreeDesc)) & ~15);
    if (hd == NULL)  {
      System_printf ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ3[i])), sizeof(memQ3[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ3[i])), sizeof(memQ3[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Qmss_queuePushDesc (tFramework.QLinkedBuf3, (Ptr)hd);

  }
  
  q.qNum = tFramework.QLinkedBuf4;
  
  for (i = 0; i < TF_LINKED_BUF_Q4_NBUFS; i++)   {
  	
    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tFramework.QfreeDesc)) & ~15);
    if (hd == NULL)  {
      System_printf ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
      return (-1);
    }

    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ4[i])), sizeof(memQ4[i]));
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(memQ4[i])), sizeof(memQ4[i]));
    hd->nextBDPtr = NULL;
    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
    Qmss_queuePushDesc (tFramework.QLinkedBuf4, (Ptr)hd);

  }
  
    
  /* General purpose queues */
  for (i = 0; i < TF_NUM_GEN_QUEUES; i++)  {
  	
  	tFramework.QGen[i] = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_FIRST_GEN_QUEUE + i, &isAlloc);

 	if (tFramework.QGen[i] < 0)  {
  	  System_printf ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_FIRST_GEN_QUEUE + i);
  	  return (-1);
  	}
  }


  return (0);
  
}

int closeQueues(void)
{
	int i;

    /* The 10 PA transmit queues (corresponding to the 10 tx cdma channels */
 	for (i = 0; i < TF_PA_NUM_TX_CPDMA_CHANNELS; i++)  {
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

	Qmss_queueEmpty(tFramework.QfreeDesc);
	Qmss_queueClose(tFramework.QfreeDesc);

	/* General purpose queues */
	for (i = 0; i < TF_NUM_GEN_QUEUES; i++)  {
		Qmss_queueEmpty(tFramework.QGen[i]);
		Qmss_queueClose(tFramework.QGen[i]);
	}

	return 0;
}


/* Configure flows */
int setupFlows (void)
{
  Cppi_RxFlowCfg  rxFlowCfg;
  uint8_t         isAlloc;

  /* Configure Rx flow */
  rxFlowCfg.flowIdNum      = CPPI_PARAM_NOT_SPECIFIED;
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
  rxFlowCfg.rx_fdq0_sz0_qnum = tFramework.QLinkedBuf1;
  rxFlowCfg.rx_fdq0_sz0_qmgr = 0;

  rxFlowCfg.rx_fdq3_qnum = tFramework.QLinkedBuf2;
  rxFlowCfg.rx_fdq3_qmgr = 0;
  rxFlowCfg.rx_fdq2_qnum = tFramework.QLinkedBuf2;
  rxFlowCfg.rx_fdq2_qmgr = 0;

  rxFlowCfg.rx_size_thresh1 = TF_LINKED_BUF_Q2_BUF_SIZE;
  rxFlowCfg.rx_size_thresh0 = TF_LINKED_BUF_Q1_BUF_SIZE;
    
  rxFlowCfg.rx_fdq0_sz1_qnum = tFramework.QLinkedBuf2;
  rxFlowCfg.rx_fdq0_sz1_qmgr = 0;
  rxFlowCfg.rx_size_thresh2  = TF_LINKED_BUF_Q3_BUF_SIZE;

  rxFlowCfg.rx_fdq0_sz3_qnum = tFramework.QLinkedBuf3;
  rxFlowCfg.rx_fdq0_sz3_qmgr = 0;
  rxFlowCfg.rx_fdq0_sz2_qnum = tFramework.QLinkedBuf3;
  rxFlowCfg.rx_fdq0_sz2_qmgr = 0;

  tFramework.tfPaFlowHnd = Cppi_configureRxFlow (tFramework.tfPaCppiHandle, &rxFlowCfg, &isAlloc);
  if (tFramework.tfPaFlowHnd == NULL)  {
    System_printf ("setupFlows: cppi_ConfigureRxFlow returned NULL\n");
    return (-1);
  }
  tFramework.tfFlowNum = (uint8_t) Cppi_getFlowId(tFramework.tfPaFlowHnd);
  return (0);

}

int closeFlows(void)
{
    Cppi_Result cppi_result;
    if ((cppi_result = Cppi_closeRxFlow (tFramework.tfPaFlowHnd)) != CPPI_SOK) {
       	return (-1);
    }
    
#ifdef NETSS_INTERNAL_PKTDMA

    if ((cppi_result = Cppi_closeRxFlow (tFramework.tfPaLocFlowHnd0)) != CPPI_SOK) {
       	return (-1);
    }


#endif
    
    return cppi_result;
}


/* The QM/CPDMA are setup */
Int initQm (void)
{
  if (setupQmMem())  {
  	System_printf ("initQm: setupQmMem failed\n");
    return (-1);
  }

  if (setupCpdma ())  {
  	System_printf ("initQm: setupCpdma failed\n");
    return (-1);
  }
  
  if (setupQueues ())  {
  	System_printf ("initQm: setupQueues failed\n");
    return (-1);
  }

  if (setupFlows ())  {
  	System_printf ("initQm: setupFlows failed\n");
    return (-1);
  }

  return (0);
  
}

/* clean up for QM/CPPI */
static int exitQm(void)
{
  if (closeFlows ())  {
  	System_printf ("exitQm: closeFlows failed\n");
    return (-1);
  }

  if (closeQueues ())  {
  	System_printf ("exitQm: closeQueues failed\n");
    return (-1);
  }

  if (closeCpdma ())  {
  	System_printf ("exitQm: closeCpdma failed\n");
    return (-1);
  }

  if (closeQmMem())  {
	  	System_printf ("exitQm: closeQmMem failed\n");
	    return (-1);
  }

  return 0;
}
	
/* Two semaphores are used to gate access to the PA handle tables */
Int initSems (void)
{
	Semaphore_Params params;
	Error_Block      eb;
	
	Semaphore_Params_init (&params);
	params.mode = Semaphore_Mode_BINARY;
	
	tFramework.tfSaSem 		  = Semaphore_create (1, &params, &eb);
	
	return (0);
}

/* Two semaphores are used to gate access to the PA handle tables */
static int deleteSems (void)
{
	Semaphore_delete (&tFramework.tfSaSem);

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

    /* Set PASS Power domain to ON */        
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_NETCP);

    /* Enable the clocks for PASS modules */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_PA,      PSC_MODSTATE_ENABLE);
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_CPGMAC,  PSC_MODSTATE_ENABLE);
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_SA,      PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_NETCP);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_NETCP));
}

/* Initialize the test framework */
Int setupTestFramework (void)
{
	GateHwi_Params prms;
	
	/* Create the HW disable gate. It is used by QM call backs */
	GateHwi_Params_init(&prms);
	tFramework.gateHwi = GateHwi_create(&prms, NULL);
#ifndef SIMULATOR_SUPPORT
    if (setupRm ())
    {
      System_printf ("Function setupRm failed\n");
      System_flush();
      return (-1);
    }
#endif
	/* Setup the semaphores used for access to the PA tables. 
	 * This has to be done before the PA is initialized */
	if (initSems())  {
		System_printf ("setupTestFramework: initQm returned error, exiting\n");
		return (-1);
	}
    
    /* Power up PA sub-systems */
	if (no_bootMode == TRUE)
		passPowerUp();
	
	/* Create the PA driver instance */
	if (initPa())  {
		System_printf ("setupTestFramework: initPa returned error, exiting\n");
		return (-1);
	}
	
	/* Setup the QM with associated buffers and descriptors */
	if (initQm())  {
		System_printf ("setupTestFramework: initQm returned error, exiting\n");
		return (-1);
	}
    
    /* Initialize the SA unit test support and create the SA driver instance */
    salld_sim_initialize();

	return (0);

}

int exitTestFramework(void)
{

    salld_sim_close_sa();

	/* Delete the semaphores created */
	if (deleteSems())  {
		System_printf ("exitTestFramework: deleteSems returned error, exiting\n");
		return (-1);
	}

	/* Clean up CPPI/QM entries */
	if (exitQm()) {
		System_printf ("exitTestFramework: exitQm returned error, exiting\n");
		return (-1);
	}

	return 0;
}

/* Check that all the queues are setup correctly */
Int verifyTestFramework (void)
{ 
	Int i, j;
	Int count;
	Int returnVal = 0;
	Cppi_HostDesc *hd;
	uint8_t *bufp;
	uint32_t bufLen;
	
	int32_t linkedQ[4];
	int32_t nbufs[]  = { TF_LINKED_BUF_Q1_NBUFS,    TF_LINKED_BUF_Q2_NBUFS,    TF_LINKED_BUF_Q3_NBUFS,    TF_LINKED_BUF_Q4_NBUFS };
	int32_t bSize[]  = { TF_LINKED_BUF_Q1_BUF_SIZE, TF_LINKED_BUF_Q2_BUF_SIZE, TF_LINKED_BUF_Q3_BUF_SIZE, TF_LINKED_BUF_Q4_BUF_SIZE};
	
	linkedQ[0] = tFramework.QLinkedBuf1;
	linkedQ[1] = tFramework.QLinkedBuf2;    
	linkedQ[2] = tFramework.QLinkedBuf3; 
	linkedQ[3] = tFramework.QLinkedBuf4; 
	
	/* Verify that all of the general purpose queues are empty */
	for (i = 0; i < TF_NUM_GEN_QUEUES; i++)  {
		if ((count = Qmss_getQueueEntryCount (tFramework.QGen[i])) != 0)  {
			System_printf ("verifyTestFramework: Expected 0 entry count for queue %d, found %d entries\n", tFramework.QGen[i], count);
			returnVal = -1;
		}
	}
	
	/* Verify that the number of descriptors in the free descriptor queue is correct */
	count = Qmss_getQueueEntryCount (tFramework.QfreeDesc);
	if (count != (TF_NUM_DESC - TF_LINKED_BUF_Q1_NBUFS - TF_LINKED_BUF_Q2_NBUFS - TF_LINKED_BUF_Q3_NBUFS  - TF_LINKED_BUF_Q4_NBUFS))  {
		System_printf ("verifyTestFramework: Expected %d entry count in the free descriptor queue (%d), found %d\n",
						TF_NUM_DESC - TF_LINKED_BUF_Q1_NBUFS - TF_LINKED_BUF_Q2_NBUFS - TF_LINKED_BUF_Q3_NBUFS - TF_LINKED_BUF_Q4_NBUFS,
						tFramework.QfreeDesc, count);
		returnVal = -1;
	}
	
	/* Verify the number and sizing of descriptors with linked buffers in the three queues */
	for (j = 0; j < 4; j++)  {
		
		count = Qmss_getQueueEntryCount (linkedQ[j]);
		if (count != nbufs[j])  {
		System_printf ("verifyTestFramework: Expected %d entry count in linked buffer queue 1 (%d), found %d\n",
						nbufs[j], linkedQ[j], count);
		returnVal = -1;
		}
	
		for (i = 0; i < count; i++)  {
			hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (linkedQ[j])) & ~15);
			Cppi_getOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, &bufp, &bufLen);
			Qmss_queuePush (linkedQ[j], (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
				
			if (bufLen != bSize[j])  {
				System_printf ("verifyTestFramework: Linked buffer queue %d (%d) expected orignal length of %d, found %d\n",
								j, linkedQ[j], bSize[j], bufLen);
				returnVal = -1;
				break;
			}
		}
	}
	
	
	return (returnVal);  
}


