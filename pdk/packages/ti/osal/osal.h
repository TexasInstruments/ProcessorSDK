/*
 * Copyright (c) 2015 - 2023, Texas Instruments Incorporated
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

/**
 *  \defgroup DRV_OSAL_MODULE OSAL
 *
 *  @{
 */
/* @} */

/**
 *  \ingroup DRV_OSAL_MODULE
 *  \defgroup DRV_OSAL_API OSAL_API
 *            OSAL interface
 *
 *  @{
 */
/** ============================================================================
 *  @file       osal.h
 *
 *  @brief      OS Abstraction Layer header
 *
 *  ============================================================================
 */

#ifndef ti_OSAL__include
#define ti_OSAL__include

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/csl/tistdtypes.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/HwiP.h>
#include <ti/osal/MuxIntcP.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/osal/CacheP.h>
#include <ti/osal/TimerP.h>
#include <ti/osal/RegisterIntr.h>
#include <ti/osal/QueueP.h>
#include <ti/osal/CycleprofilerP.h>
#include <ti/osal/MailboxP.h>
#include <ti/osal/MutexP.h>
#include <ti/osal/ClockP.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/HeapP.h>
#include <ti/osal/MemoryP.h>
#include <ti/osal/EventP.h>
#include <ti/osal/StartuphooksP.h>
#ifdef _TMS320C6X
#include <ti/osal/EventCombinerP.h>
#endif

/**
 *  \name Osal Error return Codes
 *  @brief  Return error codes for Osal generic APIs
 *  @{
 */
/*********************************************************************
 * @def osal_OK
 * OSAL operation successful
 *********************************************************************/
#define osal_OK                ((int32_t)(0))

/*********************************************************************
 * @def osal_FAILURE
 * OSAL operation failed
 *********************************************************************/
#define osal_FAILURE           (-(int32_t)(1))

/*********************************************************************
 * @def osal_UNSUPPORTED
 * OSAL operation is not supported
 *********************************************************************/
#define osal_UNSUPPORTED       (-(int32_t)(2))

/*********************************************************************
 * @def osal_NOMEM
 * OSAL operation is not successful due to internal osal static memory
 * is full. Application would need to create additional memory blocks
 * during startup by calling @ref Osal_setHwAttrs API to set the additional
 * memory blocks for semaphoreP and hwiP
 *********************************************************************/
#define osal_NOMEM             (-(int32_t)(3))
/* @} */

/*********************************************************************
 * @def osal_WAIT_FOREVER
 * OSAL timeout wait forever
 *********************************************************************/
#define osal_WAIT_FOREVER      (~((uint32_t)0U))

/*********************************************************************
 * @def osal_NO_WAIT
 * OSAL timeout no wait
 *********************************************************************/
#define osal_NO_WAIT           ((uint32_t)0U)

/*********************************************************************
 * @def OSAL_NONOS_SEMAPHOREP_SIZE_BYTES
 * Semaphorep handle size for non-os use case
 * Application can use this size to provide a secondary extended
 * memory block for semaphore creation when all the memory  (internal to osal)
 * are utilized
 *
 *********************************************************************/
#define  OSAL_NONOS_SEMAPHOREP_SIZE_BYTES             ((uint32_t) 48U)

/*********************************************************************
 * @def OSAL_NONOS_HWI_SIZE_BYTES
 * HwiP handle size for non-os use case
 * Application can use this size to provide a secondary extended
 * memory block for hwip creation when all the memory  (internal to osal)
 * are utilized
 *********************************************************************/
#define  OSAL_NONOS_HWIP_SIZE_BYTES                   ((uint32_t) 48U)

/*********************************************************************
 * @def OSAL_FREERTOS_HWIP_C7X_SIZE_BYTES
 * HwiP handle size for freertos-os c7x use case
 * Application can use this size to provide a secondary extended
 * memory block for hwip creation when all the memory  (internal to osal)
 * are utilized
 *********************************************************************/
#define  OSAL_FREERTOS_HWIP_C7X_SIZE_BYTES            ((uint32_t) 56U)

/*********************************************************************
 * @def OSAL_SAFERTOS_HWIP_C7X_SIZE_BYTES
 * HwiP handle size for safertos-os c7x use case
 * Application can use this size to provide a secondary extended
 * memory block for hwip creation when all the memory  (internal to osal)
 * are utilized
 *********************************************************************/
#define  OSAL_SAFERTOS_HWIP_C7X_SIZE_BYTES            ((uint32_t) 56U)

/*********************************************************************
 * @def OSAL_FREERTOS_SEMAPHOREP_SIZE_BYTES
 * Semaphorep handle size for non-os use case
 * Application can use this size to provide a secondary extended
 * memory block for semaphore creation when all the memory  (internal to osal)
 * are utilized
 *
 *********************************************************************/
#if defined (BUILD_C7X)
#define  OSAL_FREERTOS_SEMAPHOREP_SIZE_BYTES          ((uint32_t) 176U)
#else
#define  OSAL_FREERTOS_SEMAPHOREP_SIZE_BYTES          ((uint32_t) 96U)
#endif

/*********************************************************************
 * @def OSAL_FREERTOS_TASKP_SIZE_BYTES
 * TaskP handle size for non-os use case
 * Application can use this size to provide a secondary extended
 * memory block for semaphore creation when all the memory  (internal to osal)
 * are utilized
 *
 *********************************************************************/
#define  OSAL_FREERTOS_TASKP_SIZE_BYTES               ((uint32_t) 128U)

/*********************************************************************
 * @def OSAL_SAFERTOS_SEMAPHOREP_SIZE_BYTES
 * Semaphorep handle size for non-os use case
 * Application can use this size to provide a secondary extended
 * memory block for semaphore creation when all the memory  (internal to osal)
 * are utilized
 *
 *********************************************************************/
#if defined (BUILD_C7X)
#define  OSAL_SAFERTOS_SEMAPHOREP_SIZE_BYTES          ((uint32_t) 304U)
#else
#define  OSAL_SAFERTOS_SEMAPHOREP_SIZE_BYTES          ((uint32_t) 248U)
#endif

/*********************************************************************
 * @def OSAL_SAFERTOS_TASKP_SIZE_BYTES
 * TaskP handle size for non-os use case
 * Application can use this size to provide a secondary extended
 * memory block for semaphore creation when all the memory  (internal to osal)
 * are utilized
 *
 *********************************************************************/
#if defined (BUILD_C7X)
#define  OSAL_SAFERTOS_TASKP_SIZE_BYTES               ((uint32_t) 320U)
#else
#define  OSAL_SAFERTOS_TASKP_SIZE_BYTES               ((uint32_t) 248U)
#endif

/**
 *  \brief This structure holds static memory status parameters of OSAL
 *         library
 */
typedef struct Osal_StaticMemoryStatus
{
    uint32_t peakSemObjs;       /*!< Maxmium number of Semaphore objects used */
    uint32_t numMaxSemObjs;     /*!< Number of Semaphore objects allocated */
    uint32_t numFreeSemObjs;    /*!< Number of Semaphore objects available */

    uint32_t peakTimerObjs;     /*!< Maxmium number of Timer objects used */
    uint32_t numMaxTimerObjs;   /*!< Number of Timer objects allocated */
    uint32_t numFreeTimerObjs;  /*!< Number of Timer objects available */

    uint32_t peakHwiObjs;       /*!< Maxmium number of Hwi objects used */
    uint32_t numMaxHwiObjs;     /*!< Number of Hwi objects allocated */
    uint32_t numFreeHwiObjs;    /*!< Number of Hwi objects available */
} Osal_StaticMemStatus;

/**
 *  \brief This enum defines the multiple thread types used under OSAL
 *         library
 */
typedef enum Osal_ThreadType_e {
    Osal_ThreadType_Hwi,   /*!< Current thread is a Hwi  */
    Osal_ThreadType_Swi,   /*!< Current thread is a Swi  */
    Osal_ThreadType_Task,  /*!< Current thread is a Task */
    Osal_ThreadType_Main,  /*!< Current thread is Main   */
    Osal_ThreadType_Abort, /*!< Current thread is an Abort */
} Osal_ThreadType;

/* Internal function for assert */
extern void Osal_DebugP_assert(int32_t expression, const char *file, int32_t line);
/*!
 *  @brief  Assert checking function
 *
 *  If the expression is evaluated to false, the API does nothing.
 *  If it is evaluated to true, the underlying RTOS port implementation
 *  handles the assert via its mechanisms.
 *
 *  @param  expression Expression to evaluate
 */
/* in real code use TI's version of OSAL_Assert (which is also fatal for OSAL_Assert(1)*/
#define OSAL_Assert(expression) (Osal_DebugP_assert((int32_t)((expression)?1:0),\
                                                  __FILE__, __LINE__))

/*!
 *  @brief  Function to get the current thread type.
 *
 *
 *  @return current thread type
 */
extern Osal_ThreadType Osal_getThreadType(void);

/*!
 *  @brief  Function to delay/sleep the specified number of ticks.
 *
 *  @param  nTicks number of ticks
 *
 *  #@note  for bare metal application it is number of 1ms ticks
 *
 *          Prerequisites: Board_init() to be called before 
 *          invoking this API
 *
 *  @return osal_OK for success, osal_FAILURE on failure
 */
extern int32_t Osal_delay(uint32_t nTicks);

/*!
 *  @brief  Osal hw access type 
 *
 */
typedef enum {
  OSAL_HWACCESS_UNRESTRICTED, /*!< No restriction on hardware access or configuration */
  OSAL_HWACCESS_RESTRICTED    /*!< Restrictive access to hardware such as GIC */
} Osal_HwAccessType;

/*!
 *  @brief  Osal memory address range
 *
 */
typedef struct Osal_memRange_s {
  uintptr_t    base;          /*!< Start address value */
  uint32_t     size;          /*!< size in bytes */
} Osal_memRange;

/*!
 *  @brief  Osal ISR Hook pointer
 *
 */
typedef void  (*IsrHookPtr)(void *ptr);

/*!
 *  @brief  Osal ISR Hooks
 *
 */
typedef struct
{
    IsrHookPtr preISRHook;
    void *preISRHookArgs;
    IsrHookPtr postISRHook;
    void *postISRHookArgs;
}Osal_ISRHooks;


/*!
 *  @brief  Osal hw attributes structure
 *
 */
typedef struct Osal_HwAttrs_s
{
    /*! CPU frequency in KHz */
    uint32_t  cpuFreqKHz;
    /*! External Clock value in KHz */
    uint32_t  extClkKHz;
#if defined(gnu_targets_arm_A15F)
    /*! Set Target processor list */
    uint32_t a15TargetProcMask;
#endif
#ifdef _TMS320C6X
    /*! Interrupt numbers for Event combiner groups (0-3) */
    uint32_t ECM_intNum[4];
#endif
    /*! Hardware access type */
    Osal_HwAccessType hwAccessType;
   /*!
    * Delay timer base address:
    * Delay timer base configuration is applicable for only AM335x/AM437x SoCs
    * It is not applicable and should not be set for other SoCs
    * Osal setHwAttrs API would return failure (osal_UNSUPPORTED) if attempted to
    * be set for SoCs other than AM335x and AM437x.
    */
   uintptr_t     osalDelayTimerBaseAddr;
   /*! Semaphore external memory control block */
   Osal_memRange  extSemaphorePBlock;
   /*! Hwi external memory control block */
   Osal_memRange  extHwiPBlock;
} Osal_HwAttrs;

/**
 *  \anchor Osal_HwAttrs_ValidBits
 *  \name Osal HwAttrs ValidBit definitions
 *  @{
 */
/*!
 * bit map to set external clock in Osal_HwAttr default value
 */
#define  OSAL_HWATTR_SET_EXT_CLK                      (0x00000001U)

/*!
 * bit map to set Event combiner interrupt numbers in the Osal_HwAttr
 */
#define  OSAL_HWATTR_SET_ECM_INT                      (0x00000002U)

/*!
 * bit map to set the hardware access type
 */
#define OSAL_HWATTR_SET_HWACCESS_TYPE                 (0x00000004U)

/*!
 * bit map to set the osal_delay Timer base address
 */
#define OSAL_HWATTR_SET_OSALDELAY_TIMER_BASE          (0x00000008U)

/*!
 * bit map to set the extended SemaphoreP memory block for additional SemaphoreP needs
 */
#define OSAL_HWATTR_SET_SEMP_EXT_BASE                 (0x00000010U)

/*!
 * bit map to set the extended HwiP memory block for additional HwiP needs
 */
#define OSAL_HWATTR_SET_HWIP_EXT_BASE                 (0x00000020U)

/*!
 * bit map to set the CPU frequency
 */
#define OSAL_HWATTR_SET_CPU_FREQ                      (0x00000040U)

/*!
 * bit map to set the target processor list to direct interrupts to specific core
 */
#define OSAL_HWATTR_SET_TARG_PROC_LIST                (0x00000080U)
/* @} */


/*!
 *  @brief  Function to set the Hw Attributes
 *
 *  @param  ctrlBitMap control bit map as defined by the OSAL_HWATTR_SET control bits
 *
 *  @param  hwAttrs    pointer to Osal_HwAttrs structure
 *
 *  @return osal_OK for success, osal_FAILURE on failure and
 *   osal_UNSUPPORTED for unsupported configurations
 *   osal_UNSUPPORTED is a non fatal error and applications
 *   can continue processing if they get this error code.
 *   Osal would be configured to utilize default timer for
 *   Osal_delay() API. The default timer address for AM437x and
 *   AM335x SoCs are found under ti/osal/soc/'<'soc_part'>'/osal_soc.h
 *   For other SoCs the time base address is not applicable and
 *   the API returns osal_UNSUPPORTED as the return value.
 */
extern int32_t Osal_setHwAttrs(uint32_t ctrlBitMap, const Osal_HwAttrs *hwAttrs);

/*!
 *  @brief  Function to get the Hw Attributes
 *
 *  @param  hwAttrs  pointer to Osal_HwAttrs structure
 *
 *  @return osal_OK for success, osal_FAILURE on failure
 */
extern int32_t Osal_getHwAttrs(Osal_HwAttrs *hwAttrs);

/*!
 *  @brief  Function to get the static memory usage of OSAL library
 *
 *  @param  pMemStat  pointer to Osal_StaticMemStatus structure
 *
 *  @return osal_OK for success, osal_FAILURE on failure
 */
extern int32_t Osal_getStaticMemStatus(Osal_StaticMemStatus *pMemStat);

/*!
 *  @brief  Function to get the core ID of the running Core.
 *
 *  @return core ID of the running core
 */
extern uint32_t Osal_getCoreId(void);

/*!
 *  @brief  Function to check if the calling task is in privileged mode.
 *
 *  @return Returns pdTrue
 */
int32_t Osal_isInPrivilegeMode(void);

/*!
 *  @brief  Function to initialise ISR hooks
 *
 *  @return Returns none
 */
void Osal_initISRHooks(Osal_ISRHooks *hooks);

/*!
 *  @brief  Function to register ISR hooks
 *
 *  @return Returns none
 */
void Osal_registerISRHooks(const Osal_ISRHooks *hooks);

/*! 
 * @brief external references for Osal Hw Attribute structure 
 */
extern Osal_HwAttrs  gOsal_HwAttrs;

/*!
 * This macro generates compilier error if postulate is false, so 
 * allows 0 overhead compile time size check.  This "works" when
 * the expression contains sizeof() which otherwise doesn't work
 * with preprocessor
 *
 *   @b Example
 *   @verbatim
        #define  OSAL_TEST_STRUCT_SIZE_BYTES ((uint32_t) 48U)
        typedef struct Test_Struct_Ext_s {
            Bool        used;
            Test_Struct test;
        } Test_Struct_Ext;

        void OsalTest_compileTime_SizeChk(void)
        {
        #if defined(__GNUC__) && !defined(__ti__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wunused-variable"
        #else
        #pragma diag_suppress 179
        #endif
            OSAL_COMPILE_TIME_SIZE_CHECK (sizeof(Test_Struct_Ext),OSAL_TEST_STRUCT_SIZE_BYTES);
        #if defined(__GNUC__) && !defined(__ti__)
        #pragma GCC diagnostic pop
        #endif
        }

    @endverbatim
 */
#define OSAL_COMPILE_TIME_SIZE_CHECK(x,y)                              \
   do {                                                                 \
       struct {                                                 \
         uint8_t NegativeSizeIfPostulateFalse[(y) + 1U - (x)];         \
       } PostulateCheck;                                              \
   }                                                                   \
   while (BFALSE)

/*!
 *  @brief  Function for initializing the OS / scheduler
 *
 */
extern void OS_init(void);

/*!
 *  @brief  Function to strat OS / scheduler
 *
 */
extern void OS_start(void);

/*!
 *  @brief  Function to stop OS / scheduler
 *
 */
extern void OS_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_OSAL__include */
/* @} */
