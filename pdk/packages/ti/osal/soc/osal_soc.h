/*
 * Copyright (c) 2015-2022, Texas Instruments Incorporated
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
 *  \ingroup DRV_OSAL_MODULE
 *  \defgroup DRV_OSAL_SOC_CONFIG SOC config
 *
 *  @{
 */ 

/** ============================================================================
 *  @file       soc/osal_soc.h
 *
 *  @brief      SOC specific includes for Osal
 *
 *
 *  ============================================================================
 */
#ifndef ti_osal_soc_top_include
#define ti_osal_soc_top_include

#ifdef __cplusplus
   extern "C" {
#endif

#if defined (SOC_J721E)
#include <ti/osal/soc/j721e/osal_soc.h>
#elif defined (SOC_J7200)
#include <ti/osal/soc/j7200/osal_soc.h>
#elif defined (SOC_J721S2)
#include <ti/osal/soc/j721s2/osal_soc.h>
#elif defined (SOC_J784S4) || defined(SOC_J742S2)
#include <ti/osal/soc/j784s4/osal_soc.h>
#else
/* No known Soc is defined, have below defaults */
/* Max number of semaphores for NonOs */
#define OSAL_NONOS_MAX_SEMAPHOREP_PER_SOC   ((uint32_t) 80U)
#define OSAL_NONOS_MAX_HWIP_PER_SOC         ((uint32_t) 40U)
#define OSAL_NONOS_MAX_TIMERP_PER_SOC       ((uint32_t) 20U)

/* Max number of various modules for FreeRTOS */
#define OSAL_FREERTOS_MAX_SEMAPHOREP_PER_SOC ((uint32_t) 80U)
#define OSAL_FREERTOS_MAX_HWIP_PER_SOC       ((uint32_t) 40U)
#define OSAL_FREERTOS_MAX_TIMERP_PER_SOC     ((uint32_t) 20U)
#define OSAL_FREERTOS_MAX_TASKP_PER_SOC      ((uint32_t) 20U)
#define OSAL_FREERTOS_MAX_CLOCKP_PER_SOC     ((uint32_t) 20U)
#define OSAL_FREERTOS_MAX_MUTEXP_PER_SOC     ((uint32_t) 20U)
#define OSAL_FREERTOS_MAX_MAILBOXP_PER_SOC   ((uint32_t) 20U)
#define OSAL_FREERTOS_MAX_QUEUEP_PER_SOC     ((uint32_t) 20U)
#define OSAL_FREERTOS_MAX_HEAPP_PER_SOC      ((uint32_t) 20U)
#define OSAL_FREERTOS_MAX_EVENTP_PER_SOC     ((uint32_t) 20U)

/* Max number of various modules for SafeRTOS */
#define OSAL_SAFERTOS_MAX_SEMAPHOREP_PER_SOC ((uint32_t) 80U)
#define OSAL_SAFERTOS_MAX_HWIP_PER_SOC       ((uint32_t) 40U)
#define OSAL_SAFERTOS_MAX_TIMERP_PER_SOC     ((uint32_t) 20U)
#define OSAL_SAFERTOS_MAX_TASKP_PER_SOC      ((uint32_t) 20U)
#define OSAL_SAFERTOS_MAX_CLOCKP_PER_SOC     ((uint32_t) 20U)
#define OSAL_SAFERTOS_MAX_MUTEXP_PER_SOC     ((uint32_t) 20U)
#define OSAL_SAFERTOS_MAX_MAILBOXP_PER_SOC   ((uint32_t) 20U)
#define OSAL_SAFERTOS_MAX_EVENTP_PER_SOC     ((uint32_t) 20U)
#endif

/*********************************************************************
 * @def OSAL_FREERTOS_CONFIGNUM_SEMAPHORE
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for semaphores
 *********************************************************************/

/* Set the number of SemaphoreP_Handles for FREERTOS */
#ifndef OSAL_FREERTOS_CONFIGNUM_SEMAPHORE
#define OSAL_FREERTOS_CONFIGNUM_SEMAPHORE (OSAL_FREERTOS_MAX_SEMAPHOREP_PER_SOC)
#endif /* OSAL_FREERTOS_CONFIGNUM_SEMAPHORE */

/*********************************************************************
 * @def OSAL_FREERTOS_CONFIGNUM_TASK
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for tasks
 *********************************************************************/

/* Set the number of SemaphoreP_Handles for FREERTOS */
#ifndef OSAL_FREERTOS_CONFIGNUM_TASK
#define OSAL_FREERTOS_CONFIGNUM_TASK (OSAL_FREERTOS_MAX_TASKP_PER_SOC)
#endif /* OSAL_FREERTOS_CONFIGNUM_TASK */

/*********************************************************************
 * @def OSAL_FREERTOS_CONFIGNUM_CLOCK
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for clock
 *********************************************************************/

/* Set the number of ClockP_Handles for FREERTOS */
#ifndef OSAL_FREERTOS_CONFIGNUM_CLOCK
#define OSAL_FREERTOS_CONFIGNUM_CLOCK (OSAL_FREERTOS_MAX_CLOCKP_PER_SOC)
#endif /* OSAL_FREERTOS_CONFIGNUM_CLOCK */

/*********************************************************************
 * @def OSAL_FREERTOS_CONFIGNUM_MUTEX
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for mutex
 *********************************************************************/

/* Set the number of MutexP_Handles for FREERTOS */
#ifndef OSAL_FREERTOS_CONFIGNUM_MUTEX
#define OSAL_FREERTOS_CONFIGNUM_MUTEX (OSAL_FREERTOS_MAX_MUTEXP_PER_SOC)
#endif /* OSAL_FREERTOS_CONFIGNUM_MUTEX */

/*********************************************************************
 * @def OSAL_FREERTOS_CONFIGNUM_MAILBOX
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for mailbox
 *********************************************************************/

/* Set the number of MailboxP_Handles for FREERTOS */
#ifndef OSAL_FREERTOS_CONFIGNUM_MAILBOX
#define OSAL_FREERTOS_CONFIGNUM_MAILBOX (OSAL_FREERTOS_MAX_MAILBOXP_PER_SOC)
#endif /* OSAL_FREERTOS_CONFIGNUM_MAILBOX */

/*********************************************************************
 * @def OSAL_FREERTOS_CONFIGNUM_QUEUE
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for queue
 *********************************************************************/

/* Set the number of QueueP_Handles for FREERTOS */
#ifndef OSAL_FREERTOS_CONFIGNUM_QUEUE
#define OSAL_FREERTOS_CONFIGNUM_QUEUE (OSAL_FREERTOS_MAX_QUEUEP_PER_SOC)
#endif /* OSAL_FREERTOS_CONFIGNUM_QUEUE */

/*********************************************************************
 * @def OSAL_FREERTOS_CONFIGNUM_HEAP
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for heap
 *********************************************************************/

/* Set the number of HeapP_Handles for FREERTOS */
#ifndef OSAL_FREERTOS_CONFIGNUM_HEAP
#define OSAL_FREERTOS_CONFIGNUM_HEAP (OSAL_FREERTOS_MAX_HEAPP_PER_SOC)
#endif /* OSAL_FREERTOS_CONFIGNUM_HEAP */

/*********************************************************************
 * @def OSAL_FREERTOS_CONFIGNUM_EVENT
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for event
 *********************************************************************/

/* Set the number of EVENTP_Handles for FREERTOS */
#ifndef OSAL_FREERTOS_CONFIGNUM_EVENT
#define OSAL_FREERTOS_CONFIGNUM_EVENT (OSAL_FREERTOS_MAX_EVENTP_PER_SOC)
#endif /* OSAL_FREERTOS_CONFIGNUM_EVENT */

/*********************************************************************
 * @def OSAL_FREERTOS_CONFIGNUM_HWI
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for HwiP
 *********************************************************************
 */
/* Set the number of HwiP_Handles */
#ifndef OSAL_FREERTOS_CONFIGNUM_HWI
#define OSAL_FREERTOS_CONFIGNUM_HWI (OSAL_FREERTOS_MAX_HWIP_PER_SOC)
#endif

/*********************************************************************
 * @def OSAL_FREERTOS_CONFIGNUM_TIMER
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for timer
 *********************************************************************
 */
/* Set the number of TimerP_Handles for all boards */
#ifndef OSAL_FREERTOS_CONFIGNUM_TIMER
#define OSAL_FREERTOS_CONFIGNUM_TIMER (OSAL_FREERTOS_MAX_TIMERP_PER_SOC)
#endif

/*********************************************************************
 * @def OSAL_SAFERTOS_CONFIGNUM_HWI
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for HwiP
 *********************************************************************
 */
/* Set the number of HwiP_Handles */
#ifndef OSAL_SAFERTOS_CONFIGNUM_HWI
#define OSAL_SAFERTOS_CONFIGNUM_HWI (OSAL_SAFERTOS_MAX_HWIP_PER_SOC)
#endif

/*********************************************************************
 * @def OSAL_SAFERTOS_CONFIGNUM_TIMER
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for timer
 *********************************************************************
 */
/* Set the number of TimerP_Handles for all boards */
#ifndef OSAL_SAFERTOS_CONFIGNUM_TIMER
#define OSAL_SAFERTOS_CONFIGNUM_TIMER (OSAL_SAFERTOS_MAX_TIMERP_PER_SOC)
#endif

/*********************************************************************
 * @def OSAL_SAFERTOS_CONFIGNUM_CLOCK
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for clock
 *********************************************************************/

/* Set the number of ClockP_Handles for SAFERTOS */
#ifndef OSAL_SAFERTOS_CONFIGNUM_CLOCK
#define OSAL_SAFERTOS_CONFIGNUM_CLOCK (OSAL_SAFERTOS_MAX_CLOCKP_PER_SOC)
#endif /* OSAL_SAFERTOS_CONFIGNUM_CLOCK */

/*********************************************************************
 * @def OSAL_SAFERTOS_CONFIGNUM_EVENT
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for event
 *********************************************************************/

/* Set the number of EVENTP_Handles for SAFERTOS */
#ifndef OSAL_SAFERTOS_CONFIGNUM_EVENT
#define OSAL_SAFERTOS_CONFIGNUM_EVENT (OSAL_SAFERTOS_MAX_EVENTP_PER_SOC)
#endif /* OSAL_SAFERTOS_CONFIGNUM_EVENT */

/*********************************************************************
 * @def OSAL_SAFERTOS_CONFIGNUM_MAILBOX
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for mailbox
 *********************************************************************/

/* Set the number of MailboxP_Handles for SAFERTOS */
#ifndef OSAL_SAFERTOS_CONFIGNUM_MAILBOX
#define OSAL_SAFERTOS_CONFIGNUM_MAILBOX (OSAL_SAFERTOS_MAX_MAILBOXP_PER_SOC)
#endif /* OSAL_SAFERTOS_CONFIGNUM_MAILBOX */

/*********************************************************************
 * @def OSAL_SAFERTOS_CONFIGNUM_MUTEX
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for mutex
 *********************************************************************/

/* Set the number of MutexP_Handles for SAFERTOS */
#ifndef OSAL_SAFERTOS_CONFIGNUM_MUTEX
#define OSAL_SAFERTOS_CONFIGNUM_MUTEX (OSAL_SAFERTOS_MAX_MUTEXP_PER_SOC)
#endif /* OSAL_SAFERTOS_CONFIGNUM_MUTEX */

/*********************************************************************
 * @def OSAL_SAFERTOS_CONFIGNUM_SEMAPHORE
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for semaphores
 *********************************************************************/

/* Set the number of SemaphoreP_Handles for SAFERTOS */
#ifndef OSAL_SAFERTOS_CONFIGNUM_SEMAPHORE
#define OSAL_SAFERTOS_CONFIGNUM_SEMAPHORE (OSAL_SAFERTOS_MAX_SEMAPHOREP_PER_SOC)
#endif /* OSAL_SAFERTOS_CONFIGNUM_SEMAPHORE */

/*********************************************************************
 * @def OSAL_SAFERTOS_CONFIGNUM_TASK
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for task
 *********************************************************************/

/* Set the number of SemaphoreP_Handles for SAFERTOS */
#ifndef OSAL_SAFERTOS_CONFIGNUM_TASK
#define OSAL_SAFERTOS_CONFIGNUM_TASK (OSAL_SAFERTOS_MAX_TASKP_PER_SOC)
#endif /* OSAL_SAFERTOS_CONFIGNUM_TASK */

/*********************************************************************
 * @def OSAL_NONOS_CONFIGNUM_HWI
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for HwiP
 *********************************************************************
 */
/* Set the number of HwiP_Handles */
#ifndef OSAL_NONOS_CONFIGNUM_HWI
#define OSAL_NONOS_CONFIGNUM_HWI (OSAL_NONOS_MAX_HWIP_PER_SOC)
#endif

/*********************************************************************
 * @def OSAL_NONOS_CONFIGNUM_TIMER
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for timer
 *********************************************************************
 */
/* Set the number of TimerP_Handles for all boards */
#ifndef OSAL_NONOS_CONFIGNUM_TIMER
#define OSAL_NONOS_CONFIGNUM_TIMER (OSAL_NONOS_MAX_TIMERP_PER_SOC)
#endif

/*********************************************************************
 * @def OSAL_NONOS_CONFIGNUM_SEMAPHORE
 * If the need is more than the defaults, application would need to
 * suppliment the additional memory for OSAL using @ref Osal_setHwAttrs
 * API calls by setting the extended memory block for semaphores
 *********************************************************************/

/* Set the number of SemaphoreP_Handles */
#ifndef OSAL_NONOS_CONFIGNUM_SEMAPHORE
#define OSAL_NONOS_CONFIGNUM_SEMAPHORE (OSAL_NONOS_MAX_SEMAPHOREP_PER_SOC)
#endif

typedef struct {
	uint32_t coreId;
	uint32_t timerId;
} Osal_timerReserved;


#ifdef __cplusplus
}
#endif

#endif
/* @} */

/* Nothing past this point */
