/*
 * Copyright (c) 2017-present, Texas Instruments Incorporated
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
 *  \ingroup DRV_OSAL_SOC_CONFIG
 *  \defgroup DRV_OSAL_SOC_CONFIG_J721E J721E specific SOC config
 *
 *  @{
 */ 

/** ============================================================================
 *  @file       j721e/osal_soc.h
 *
 *  @brief      SOC specific includes for Osal
 *
 *
 *  ============================================================================
 */
#ifndef ti_osal_soc_j7_include
#define ti_osal_soc_j7_include

#ifdef __cplusplus
extern "C" {
#endif
#include <ti/osal/osal.h>
#include <ti/csl/soc.h>
#if defined(TimerP_numTimerDevices)
#undef  TimerP_numTimerDevices
#endif

#if defined(TIMERP_ANY_MASK)
#undef  TIMERP_ANY_MASK
#endif

#define OSAL_MPU1_0                         (0U)    /**< ARM A72 - VM0 */
#define OSAL_MCU1_0                         (1U)    /**< ARM MCU  R5F - core0 */
#define OSAL_MCU1_1                         (2U)    /**< ARM MCU  R5F - core1 */
#define OSAL_MCU2_0                         (3U)    /**< ARM Main R5F - core0 */
#define OSAL_MCU2_1                         (4U)    /**< ARM Main R5F - core1 */
#define OSAL_MCU3_0                         (5U)    /**< ARM Main R5F - core2 */
#define OSAL_MCU3_1                         (6U)    /**< ARM Main R5F - core3 */
#define OSAL_C66_1                          (7U)    /**< DSP C66 - core1 */
#define OSAL_C66_2                          (8U)   /**< DSP C66 - core2 */
#define OSAL_C7X_1                          (9U)   /**< DSP C7x - core1 */
#define OSAL_INVALID_CORE_ID                (10U)

/* Number of timers restricted for use */
//#define MAX_RESTRICTED_TIMERS               (9U)

#define  EXTERNAL_CLOCK_KHZ_DEFAULT         (24000U)
#define  PMU_CLOCK_KHZ_DEFAULT              (1000000U)

#if defined(BUILD_MCU)
  #define  OSAL_CPU_FREQ_KHZ_DEFAULT        (1000000U)
#elif defined(_TMS320C6X)
  #define  OSAL_CPU_FREQ_KHZ_DEFAULT        (1350000U)
#elif defined(BUILD_C7X)
  #define  OSAL_CPU_FREQ_KHZ_DEFAULT        (1000000U)
#else
  #define  OSAL_CPU_FREQ_KHZ_DEFAULT        (1000000U)
#endif


#if defined(TIMERP_TIMER_FREQ_LO)
#undef   TIMERP_TIMER_FREQ_LO
#endif

#if defined(TIMERP_TIMER_FREQ_HI)
#undef   TIMERP_TIMER_FREQ_HI
#endif

#define  TIMERP_TIMER_FREQ_LO   (19200000U)
#define  TIMERP_TIMER_FREQ_HI   (0U)

#define TimerP_numTimerDevices          ((uint32_t) 20U )
#if defined (BUILD_MCU)
  #define TIMERP_ANY_MASK_MCU_DOMAIN                ((uint32_t) 0x00FF)
  #define TIMERP_AVAILABLE_MASK_MCU_DOMAIN          ((uint32_t) 0x00FF)
  #define TIMERP_ANY_MASK                           ((uint32_t) 0x00FF)
  #define TIMERP_AVAILABLE_MASK                     ((uint32_t) 0x00FF)
#elif defined (BUILD_C7X)
  #define TIMERP_ANY_MASK               ((uint32_t) 0x000F)
  #define TIMERP_AVAILABLE_MASK         ((uint32_t) 0x000F)
#else
  #define TIMERP_ANY_MASK               ((uint32_t) 0x0FFF)
  #define TIMERP_AVAILABLE_MASK         ((uint32_t) 0x0FFF)
#endif

/* using the default timer base addresses */
#if defined(__aarch64__)
#define OSAL_DELAY_TIMER_ADDR_DEFAULT   (CSL_TIMER1_CFG_BASE)
#else
#define OSAL_DELAY_TIMER_ADDR_DEFAULT   (CSL_MCU_TIMER1_CFG_BASE)
#endif

#define TIMER_INITIAL_COUNT             (0xFFF00000U)
#define TIMER_RLD_COUNT                 (0xFFF00000U)

/* GTC count clock for A72 */
#define  OSAL_SOC_MPU_GTC_CLK               (200000U)

/* Max number of various modules for NonOs */
#define OSAL_NONOS_MAX_SEMAPHOREP_PER_SOC   ((uint32_t) 80U)
#define OSAL_NONOS_MAX_HWIP_PER_SOC         ((uint32_t) 40U)
#define OSAL_NONOS_MAX_TIMERP_PER_SOC       (TimerP_numTimerDevices)

/* Max number of various modules for FreeRTOS */
#define OSAL_FREERTOS_MAX_SEMAPHOREP_PER_SOC ((uint32_t) 360U)
#define OSAL_FREERTOS_MAX_HWIP_PER_SOC       ((uint32_t) 40U)
#define OSAL_FREERTOS_MAX_TIMERP_PER_SOC     (TimerP_numTimerDevices)
#define OSAL_FREERTOS_MAX_TASKP_PER_SOC      ((uint32_t) 160U)
#define OSAL_FREERTOS_MAX_CLOCKP_PER_SOC     ((uint32_t) 20U)
#define OSAL_FREERTOS_MAX_MUTEXP_PER_SOC     ((uint32_t) 32U)
#define OSAL_FREERTOS_MAX_MAILBOXP_PER_SOC   ((uint32_t) 20U)
#define OSAL_FREERTOS_MAX_QUEUEP_PER_SOC     ((uint32_t) 40U)
#define OSAL_FREERTOS_MAX_HEAPP_PER_SOC      ((uint32_t) 20U)
#define OSAL_FREERTOS_MAX_EVENTP_PER_SOC     ((uint32_t) 20U)

/* Max number of various modules for SafeRTOS */
#define OSAL_SAFERTOS_MAX_HWIP_PER_SOC       ((uint32_t) 40U)
#define OSAL_SAFERTOS_MAX_SEMAPHOREP_PER_SOC ((uint32_t) 360U)
#define OSAL_SAFERTOS_MAX_TASKP_PER_SOC      ((uint32_t) 160U)
#define OSAL_SAFERTOS_MAX_TIMERP_PER_SOC     (TimerP_numTimerDevices)
#define OSAL_SAFERTOS_MAX_CLOCKP_PER_SOC     ((uint32_t) 20U)
#define OSAL_SAFERTOS_MAX_MUTEXP_PER_SOC     ((uint32_t) 20U)
#define OSAL_SAFERTOS_MAX_MAILBOXP_PER_SOC   ((uint32_t) 20U)
#define OSAL_SAFERTOS_MAX_EVENTP_PER_SOC     ((uint32_t) 20U)

#define OSAL_ARCH_TIMER_INST_FOR_TS         (TimerP_ANY)
/**< Default timer instance for timer */

/* Default DM Timer allocation for r5f cores in j721e   */
#define OSAL_SAFERTOS_OS_TIMER_ID_MCU1_0        ( 1U )   /*  MCU DM Timer 1  */
#define OSAL_SAFERTOS_OS_TIMER_ID_MCU1_1        ( 2U )   /*  MCU DM Timer 2  */  
#define OSAL_SAFERTOS_OS_TIMER_ID_MCU2_0        ( 0U )   /*  DM Timer 12  */
#define OSAL_SAFERTOS_OS_TIMER_ID_MCU2_1        ( 1U )   /*  DM Timer 13  */   
#define OSAL_SAFERTOS_OS_TIMER_ID_MCU3_0        ( 2U )   /*  DM Timer 14  */
#define OSAL_SAFERTOS_OS_TIMER_ID_MCU3_1        ( 3U )   /*  DM Timer 15  */
#define OSAL_FREERTOS_OS_TIMER_ID_MCU1_0        ( 1U )   /*  MCU DM Timer 1  */
#define OSAL_FREERTOS_OS_TIMER_ID_MCU1_1        ( 2U )   /*  MCU DM Timer 2  */
#define OSAL_FREERTOS_OS_TIMER_ID_MCU2_0        ( 0U )   /*  DM Timer 12  */
#define OSAL_FREERTOS_OS_TIMER_ID_MCU2_1        ( 1U )   /*  DM Timer 13  */
#define OSAL_FREERTOS_OS_TIMER_ID_MCU3_0        ( 2U )   /*  DM Timer 14  */
#define OSAL_FREERTOS_OS_TIMER_ID_MCU3_1        ( 3U )   /*  DM Timer 15  */

/* Default DM Timer allocation for c66x cores in j721e   */
#define OSAL_SAFERTOS_OS_TIMER_ID_C66X_1        ( 0U )   /*  DM Timer 0  */
#define OSAL_SAFERTOS_OS_TIMER_ID_C66X_2        ( 1U )   /*  DM Timer 1  */
#define OSAL_FREERTOS_OS_TIMER_ID_C66X_1        ( 0U )   /*  DM Timer 0  */
#define OSAL_FREERTOS_OS_TIMER_ID_C66X_2        ( 1U )   /*  DM Timer 1  */

#define OSAL_SAFERTOS_OS_TIMER_INT_NUM_C66X_1   ( 14U ) 
#define OSAL_SAFERTOS_OS_TIMER_INT_NUM_C66X_2   ( 15U )
#define OSAL_SAFERTOS_OS_TIMER_EVENT_ID_C66X_1  ( 21U )
#define OSAL_SAFERTOS_OS_TIMER_EVENT_ID_C66X_2  ( 20U )

/* Default DM Timer allocation for c7x cores in j721e   */
#define OSAL_SAFERTOS_OS_TIMER_ID_C7X_1         ( 2U )   /*  DM Timer 2  */
#define OSAL_FREERTOS_OS_TIMER_ID_C7X_1         ( 2U )   /*  DM Timer 2  */

#define OSAL_SAFERTOS_OS_TIMER_INT_NUM_C7X_1    ( 16U )

#define OSAL_SAFERTOS_OS_YEILD_INT_NUM_C7X      ( 13U )

/* external references */
extern Osal_HwAttrs  gOsal_HwAttrs;

#if defined (BUILD_C7X)
/* The C7x CLEC should be programmed to allow config/re config either in secure
 * OR non secure mode. This function configures all inputs to given level
 *
 * Instance is hard-coded for J721e only
 *
 */
void OsalCfgClecAccessCtrl (bool onlyInSecure);
#endif


#ifdef __cplusplus
}
#endif

/* @} */

#endif
/* nothing past this point */
