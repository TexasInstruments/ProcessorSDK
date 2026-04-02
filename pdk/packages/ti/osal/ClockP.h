/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
 *  \defgroup DRV_OSAL_ClockP ClockP
 *            ClockP interface
 *
 *  @{
 */
/** ============================================================================
 *  @file       ClockP.h
 *
 *  @brief      Clock module for the RTOS Porting Interface. This implements
 *              the software timer support. For hardware timer support, please
 *              check TimerP.h
 *  
 *  The ClockP module can be used to create a software (OS) timer and configure 
 *  it to invoke a User callback when timer expires.
 *
 *  The timer can be configured as a one-shot or a continuous mode timer.
 * 
 *  ============================================================================
 */

#ifndef ti_osal_ClockP__include
#define ti_osal_ClockP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * \anchor ClockP_Status
 * \name Status codes for ClockP APIs
 *
 * @{
 */
/*!
 *  @brief    Status codes for ClockP APIs
 */
typedef int32_t ClockP_Status;
/*! API completed successfully */
#define ClockP_OK                                   ((int32_t) 0)
/*! API failed */
#define ClockP_FAILURE                              ((int32_t) (-1))
/* @} */

/**
 * \anchor ClockP_RunMode
 * \name Timer runtime modes for ClockP APIs
 *
 * @{
 */
/*!
 *  @brief  This enumerator defines the Timer runtime modes
 */
typedef uint8_t ClockP_RunMode;
      /** Timer runs for a single period values and stops */
#define ClockP_RunMode_ONESHOT                      ((uint8_t) 0U)
      /** Timer is periodic and runs continuously */
#define ClockP_RunMode_CONTINUOUS                   ((uint8_t) 1U)
/* @} */

/**
 * \anchor ClockP_StartMode
 * \name Timer start modes for ClockP APIs
 *
 * @{
 */
/*!
 *  @brief  This enumerator defines the Timer start modes
 */
typedef uint8_t ClockP_StartMode;
      /** Timer will be started by the user */
 #define ClockP_StartMode_USER                      ((uint8_t) 0U)
      /** Timer starts automatically after create or scheduler start */
#define ClockP_StartMode_AUTO                       ((uint8_t) 1U)
/* @} */

/*!
 *  @brief    Opaque client reference to an instance of a ClockP
 *
 *  A ClockP_Handle returned from the ::ClockP_create represents that
 *  instance and  is used in the other instance based functions
 */
typedef  void *ClockP_Handle;

/*!
 *  @brief    Basic ClockP Parameters
 *
 *  Structure that contains the parameters are passed into ::ClockP_create
 *  when creating a ClockP instance.
 */
typedef struct ClockP_Params_s
{
    void     *pErrBlk;   /*!< Pointer to the error block for clock create */
    uint8_t  startMode;  /*!< Timer Start Mode \ref ClockP_StartMode */
    uint32_t period;     /*!< The clock period in units of clock ticks */
    uint8_t  runMode;    /*!< Timer Run Mode \ref ClockP_RunMode */
    void     *arg;       /*!< User argument that is available inside the callback */
} ClockP_Params;

/*!
 *  @brief  Initialize params structure to default values.
 *
 *  @param params  Pointer to the instance configuration parameters.
 */
extern void ClockP_Params_init(ClockP_Params *params);

/**
 * \brief Callback that is called when the clock expires
 *
 * \param args [in] user specific argument pointer that was passed via \ref ClockP_Params
 */
typedef void (*ClockP_FxnCallback)(void *args);

/*!
 *  @brief  Function to create a clock.
 *
 *  @param  clockfxn  Function pointer of the clock function \ref ClockP_FxnCallback.
 *
 *  @param  params  Pointer to the instance configuration parameters.
 *
 *  @return A ClockP_Handle on success or a NULL on an error
 */
extern ClockP_Handle ClockP_create(ClockP_FxnCallback clockfxn,
                                   const ClockP_Params *params);

/*!
 *  @brief  Function to delete a clock.
 *
 *  @param  handle  A ClockP_Handle returned from ClockP_create
 *
 *  @return Status of the functions
 *    - ClockP_OK: Deleted the clock instance
 *    - ClockP_FAILURE: Failed to delete the clock instance
 */
extern ClockP_Status ClockP_delete(ClockP_Handle handle);

/*!
 *  @brief  Function to start a clock.
 *
 *  @param  handle  A ClockP_Handle returned from ClockP_create
 *
 *  @return Status of the functions
 *    - ClockP_OK: Started the clock instance
 *    - ClockP_FAILURE: Failed to start the clock instance
 */
extern ClockP_Status ClockP_start(ClockP_Handle handle);

/*!
 *  @brief  Function to stop a clock.
 *
 *  @param  handle  A ClockP_Handle returned from ClockP_create
 *
 *  @return Status of the functions
 *    - ClockP_OK: Stopped the clock instance
 *    - ClockP_FAILURE: Failed to stop the clock instance
 */
extern ClockP_Status ClockP_stop(ClockP_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* ti_osal_ClockP__include */
/* @} */
