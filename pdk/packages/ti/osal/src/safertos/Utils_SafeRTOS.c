/*
 * Copyright ( c ) 2015-2023, Texas Instruments Incorporated
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
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT ( INCLUDING NEGLIGENCE OR
 * OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== Utils_safertos.c ========
 */

#include <ti/osal/src/nonos/Nonos_config.h>
#include <ti/osal/TaskP.h>

#include "SafeRTOS_priv.h"

/* External Clock should be defined under osal_soc.h
 * if SOC is not supporting it, set to -1
 */
#ifndef EXTERNAL_CLOCK_KHZ_DEFAULT
#define EXTERNAL_CLOCK_KHZ_DEFAULT (0xFFFFFFFFU)
#endif

#ifndef OSAL_DELAY_TIMER_ADDR_DEFAULT
#define OSAL_DELAY_TIMER_ADDR_DEFAULT ( ( uintptr_t )( 0U ) )
#endif

#ifndef OSAL_TARGET_PROC_MASK_DEFAULT
#define OSAL_TARGET_PROC_MASK_DEFAULT ( 0xFFFFU )
#endif

/* Global Osal static memory status variables */
extern uint32_t __IRQ_STACK_START;
extern uint32_t __IRQ_STACK_END;
uint32_t  gOsalSemAllocCnt   = 0U, gOsalSemPeak = 0U;
uint32_t  gOsalTimerAllocCnt = 0U, gOsalTimerPeak = 0U;
uint32_t  gOsalHwiAllocCnt   = 0U, gOsalHwiPeak = 0U;
uint32_t  gOsalMutexAllocCnt = 0U, gOsalMutexPeak = 0U;
uint32_t  gOsalHeapAllocCnt  = 0U, gOsalHeapPeak = 0U;
#ifndef OSAL_CPU_FREQ_KHZ_DEFAULT
#define OSAL_CPU_FREQ_KHZ_DEFAULT ( 400000U )
#endif

/* Global Osal ISR hooks */
Osal_ISRHooks gOsalISRHooks          = {NULL};

volatile bool Osal_DebugP_Assert_Val = BTRUE;

/* Global Osal_HwAttr structure */
Osal_HwAttrs  gOsal_HwAttrs = {
   OSAL_CPU_FREQ_KHZ_DEFAULT,
   EXTERNAL_CLOCK_KHZ_DEFAULT,
#if defined( gnu_targets_arm_A15F )
   OSAL_TARGET_PROC_MASK_DEFAULT,
#endif
#ifdef _TMS320C6X
   /* ECM_intNum[]: Event combiner interrupts */
  { OSAL_ECM_GROUP0_INT, /* Interrupt[4-15] to use for Event Combiner Group 0 */
    OSAL_ECM_GROUP1_INT, /* Interrupt[4-15] to use for Event Combiner Group 1 */
    OSAL_ECM_GROUP2_INT, /* Interrupt[4-15] to use for Event Combiner Group 2 */
    OSAL_ECM_GROUP3_INT  /* Interrupt[4-15] to use for Event Combiner Group 3 */
  },
#endif
  OSAL_HWACCESS_UNRESTRICTED, /* Unrestricted access to hardware resources */
  /* Below timer base configuration is applicable for only AM335x/AM437x SoCs
   * It is not applicable and should not be set for other SoCs
   * Osal setHwAttrs API would return failure ( osal_FAILURE ) if attempted to
   * be set for SoCs other than AM335x and AM437x.
   */
  ( uintptr_t ) OSAL_DELAY_TIMER_ADDR_DEFAULT, /* Timer Base address for osal delay implementation for AM3/AM4 parts */
  /* Default external Semaphore Memory Block */
  {
     ( uintptr_t ) 0U,
     0U
  },
  /* Default external HwiP Memory Block */
  {
     ( uintptr_t ) 0U,
     0U
  }
};

#if defined (BUILD_MCU)
/* This implementation reads the the stack pointer on ARM cores and
 * checks if the current context is an IRQ context.
 * function is defined in SafeRTOS_utils_r5f.asm
 */
extern uint32_t Osal_getSP(void);
#endif

extern portBaseType xPortInIsrContext( void );

/*
 *  ======== Osal_DebugP_assert ========
 */
void Osal_DebugP_assert( int32_t expression, const char *file, int32_t line )
{
    ( void )file;
    ( void )line;

    if( 0 != expression ) {
        while ( BTRUE == Osal_DebugP_Assert_Val ) {}
    }
}

Osal_ThreadType Osal_getThreadType( void )
{
    Osal_ThreadType osalThreadType;
    if( 1 == Osal_isInISRContext() )
    {
        osalThreadType = Osal_ThreadType_Hwi;
    }
#if defined (BUILD_MCU)
    else if(BTRUE == Osal_isInAbortContext())
    {
        osalThreadType = Osal_ThreadType_Abort;
    }
#endif
    else if(pdFALSE == xTaskIsSchedulerStarted())
    {
        osalThreadType = Osal_ThreadType_Main;
    }
    else
    {
        osalThreadType = Osal_ThreadType_Task;
    }
    return ( osalThreadType );
}

/* Osal delay */
int32_t Osal_delay( uint32_t nTicks )
{
  Osal_ThreadType type;
  int32_t   ret;

  type = Osal_getThreadType(  );
  if( Osal_ThreadType_Task == type ) {
    TaskP_sleep( nTicks );
    ret = osal_OK;
  }
  else {
    ret = osal_FAILURE;
  }
  return( ret );
}

/*
 * set Osal_HwAttrs structure
 */
int32_t Osal_setHwAttrs( uint32_t ctrlBitMap, const Osal_HwAttrs *hwAttrs )
{
   int32_t  ret = osal_FAILURE;
   if( NULL_PTR != hwAttrs ) {
     if( 0U != ( ctrlBitMap & OSAL_HWATTR_SET_EXT_CLK ) ) {
       gOsal_HwAttrs.extClkKHz= hwAttrs->extClkKHz;
       ret = osal_OK;
     }
#ifdef _TMS320C6X
     /* Set the Event Combiner Interrupts */
     if( 0U != ( ctrlBitMap & OSAL_HWATTR_SET_ECM_INT ) ) {
       ( void )memcpy( gOsal_HwAttrs.ECM_intNum,hwAttrs->ECM_intNum,4U*sizeof( gOsal_HwAttrs.ECM_intNum[0] ) );
       ret = osal_OK;
     }
#endif
     /* Set the Hw Access type */
     if( 0U != ( ctrlBitMap & OSAL_HWATTR_SET_HWACCESS_TYPE ) ) {
       gOsal_HwAttrs.hwAccessType = hwAttrs->hwAccessType;
       ret = osal_OK;
     }

     /* Set the Hw Access type */
     if( 0U != ( ctrlBitMap & OSAL_HWATTR_SET_OSALDELAY_TIMER_BASE ) ) {
       ret = osal_UNSUPPORTED;
     }

     /* Set the extended memmory block for semaphore operations */
     if( 0U != ( ctrlBitMap & OSAL_HWATTR_SET_SEMP_EXT_BASE ) )
     {
         gOsal_HwAttrs.extSemaphorePBlock = hwAttrs->extSemaphorePBlock;
         /* Zero out the given memory block */
         ( void )memset( ( void* )gOsal_HwAttrs.extSemaphorePBlock.base, 0, gOsal_HwAttrs.extSemaphorePBlock.size );
         ret = osal_OK;
     }

     /* Set the extended memmory block for semaphore operations */
     if( 0U != ( ctrlBitMap & OSAL_HWATTR_SET_HWIP_EXT_BASE ) )
     {
         gOsal_HwAttrs.extHwiPBlock = hwAttrs->extHwiPBlock;
         /* Zero out the given memory block */
         ( void )memset( ( void* )gOsal_HwAttrs.extHwiPBlock.base, 0, gOsal_HwAttrs.extHwiPBlock.size );
         ret = osal_OK;
     }
     /* Set the CPU frequency */
     if( 0U != ( ctrlBitMap & OSAL_HWATTR_SET_CPU_FREQ ) )
     {
         gOsal_HwAttrs.cpuFreqKHz = hwAttrs->cpuFreqKHz;
         ret = osal_OK;
     }
   }
   return( ret );
}

/*
 * set Osal_HwAttrs structure
 */
int32_t Osal_getHwAttrs(  Osal_HwAttrs *hwAttrs )
{
   int32_t  ret = osal_FAILURE;
   if( NULL_PTR != hwAttrs ) {
     ( void )memcpy( hwAttrs, &gOsal_HwAttrs, sizeof( Osal_HwAttrs ) );
     ret = osal_OK;
   }
   return( ret );
}

int32_t Osal_getStaticMemStatus( Osal_StaticMemStatus *pMemStat )
{
    int32_t   retVal = osal_OK;
    uintptr_t cookie;

    if( NULL_PTR != pMemStat )
    {
        cookie = HwiP_disable(  );

        pMemStat->peakSemObjs    = gOsalSemPeak;
        pMemStat->numMaxSemObjs  = OSAL_SAFERTOS_CONFIGNUM_SEMAPHORE;
        pMemStat->numFreeSemObjs =
            pMemStat->numMaxSemObjs - gOsalSemAllocCnt;

        pMemStat->peakTimerObjs    = gOsalTimerPeak;
        pMemStat->numMaxTimerObjs  = OSAL_SAFERTOS_CONFIGNUM_TIMER;
        pMemStat->numFreeTimerObjs =
            pMemStat->numMaxTimerObjs - gOsalTimerAllocCnt;

        pMemStat->peakHwiObjs    = gOsalHwiPeak;
        pMemStat->numMaxHwiObjs  = OSAL_SAFERTOS_CONFIGNUM_HWI;
        pMemStat->numFreeHwiObjs =
            pMemStat->numMaxHwiObjs - gOsalHwiAllocCnt;

        HwiP_restore( cookie );
    }
    else
    {
        retVal = osal_FAILURE;
    }

    return ( retVal );
}

int32_t Osal_isInISRContext(void)
{
    int32_t retVal = 1;

    /* xPortInIsrContext accesses kernel data, and hence, causes abort when
     * a non privileged task tries to call the same. This implementation reads the
     * the stack pointer on ARM cores and checks if the current contxt is an IRQ context.
     */
#if defined (BUILD_MCU)

    uint32_t volatile sp = 0, start, end;
    sp = Osal_getSP();
    start = (uint32_t)&__IRQ_STACK_START;
    end = (uint32_t)&__IRQ_STACK_END;
    
    bool spStartBfrISRend  = (end >= sp);
    bool spStartAftISRstrt = (start <= sp);
    
    if(spStartAftISRstrt && spStartBfrISRend)
    {
        retVal = 1;
    }
    else
    {
        retVal = 0;
    }
#else
    retVal = (int32_t)xPortInIsrContext();
#endif
    return retVal;
}

int32_t Osal_isInPrivilegeMode(void)
{
  int32_t retVal = 1;

  /* SafeRTOS package does not have an API to check for privilege level for C7X. */
#if defined (BUILD_MCU) || defined (BUILD_C66X)
  retVal = (int32_t)xPortIsPrivilegedMode();
#endif
  return retVal;
}

void Osal_initISRHooks(Osal_ISRHooks *hooks)
{
    hooks->preISRHook = NULL;
    hooks->postISRHook = NULL;
    hooks->preISRHookArgs = NULL;
    hooks->postISRHookArgs = NULL;
}

void Osal_registerISRHooks(const Osal_ISRHooks *hooks)
{
    /* Copying ISR hooks */
    gOsalISRHooks.preISRHook = hooks->preISRHook;
    gOsalISRHooks.postISRHook = hooks->postISRHook;
    gOsalISRHooks.preISRHookArgs = hooks->preISRHookArgs;
    gOsalISRHooks.postISRHookArgs = hooks->postISRHookArgs;
}


/* Nothing past this point */
