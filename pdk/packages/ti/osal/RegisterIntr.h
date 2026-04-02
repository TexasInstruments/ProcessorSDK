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
 *  \ingroup DRV_OSAL_MODULE
 *  \defgroup DRV_OSAL_RegisterIntr RegisterIntr
 *            RegisterIntr interface
 *
 *  @{
 */
/** ============================================================================
 *  @file       RegisterIntr.h
 *
 *  @brief      Interrupt register routines
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_RegisterIntr__include
#define ti_osal_RegisterIntr__include

#include <ti/osal/EventCombinerP.h>
#include <ti/osal/HwiP.h>
#include <ti/osal/MuxIntcP.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
typedef void (*Osal_IsrRoutine)(uintptr_t arg);
/*!
 *  @brief  Interrupt Configuration parameters for the corepac (c6x/a15/m5/a8/a9)
 */
typedef struct {
    char    *name;              /*!< Name of the instance for debugging purposes, could be set to NULL */
    uint32_t corepacEventNum;   /*!< Event number going in to the DSP corepac */
    uint32_t intVecNum;         /*!< Interrupt vector number for ARM corepac */
    Osal_IsrRoutine isrRoutine; /*!< The ISR routine to hook the corepacEventNum to */
    uintptr_t arg;              /*!< Argument to the ISR routine */
    uint32_t priority;          /*!< Device specific priority for ARM corepac where a lower priority value
                                     indicates a higher priority, the priority range is corepac specific as
                                     listed below:
                                     - A8: 0-127
                                     - A9/A15/A53: 0-255
                                     - ARP32: 1(NMI), 2-13(INT4-15)
                                     - R5: 0-15
                                     - M4: None
                                     Note: For R5 Core, baremetal case, there is no device specific
                                           priority set - all interrupts are handled at the same priority
                                           So, this is not applicable for bare metal R5 Core interrupt config.
                                 */
    uint32_t triggerSensitivity; /*!< Set an interrupt's trigger sensitivity for ARM corepac as
                                      @ref OSAL_armGicTrigType_t. The applicable trigger types are
                                      corepac specific and listed below:
                                      - A53: Level and Edge Trigger
                                      - A15: Low/High Level and Rising/Falling Edge Trigger
                                      - A8/A9: Low/High Level and Rising/Falling/Both Edge Trigger
                                      - R5: Level Trigger and Pulse Trigger
                                  */
    uint32_t enableIntr;          /*!< When set to TRUE, interrupt is enabled after registration
                                       (@ref Osal_RegisterInterrupt) otherwise interrupt is disabled
                                   */
#ifdef QNX_OS
    uint32_t intAutoEnable;     /*!< Automatically enable interrupt after calling ISR routine */
#endif
} OsalRegisterIntParams_corepac_t;

/*!
 * @brief For C6x, if intVec=16, then event combiner is to be used
 */
#define OSAL_REGINT_INTVEC_EVENT_COMBINER (16U) 

/*!
 *  @brief  Interrupt Configuration parameters for soc mux prior to reaching the core
 */
typedef struct {
   MuxIntcP_inParams *muxInParams;      /*!< Basic MuxIntcP Input Parameters */
   MuxIntcP_outParams *muxOutParams;    /*!< Basic MuxIntcP Output Parameters */
} OsalRegisterIntParams_socmux_t;

/*!
 *  @brief  Interrupt Configuration parameters
 *          This is the data structure used to configure the interrupts in the system.
 *   coreConfig: This part configures the interrupt for corepac such as c66x, arm, m4
 *   socMuxConfig: This part configures the interrupt path for any SOC level mux such as CIC/GIC or cross bar,
 *                 leading up to the corepac
 */
typedef struct {
    OsalRegisterIntParams_corepac_t corepacConfig;  /*!< configures the interrupt for corepac such as c66x, arm, m4 */
    OsalRegisterIntParams_socmux_t  socMuxConfig;   /*!< configures the interrupt path for any SOC level mux such as
                                                         CIC/GIC or cross bar, leading up to the corepac
                                                     */
} OsalRegisterIntrParams_t;

/**
 *  \anchor OsalInterruptRetCode_e
 *  \name Osal Interrupt return Codes
 *  @{
 */
/*!
 *  @brief  Return error codes for  Osal Interrupt functions
 */
typedef int32_t OsalInterruptRetCode_e;
/*! Success return code */
#define  OSAL_INT_SUCCESS                       ((int32_t) 0)
/*! Invalid input parameters */
#define  OSAL_INT_ERR_INVALID_PARAMS            ((int32_t) -1)
/*! Error while registering the Hwi object */
#define  OSAL_INT_ERR_HWICREATE                 ((int32_t) -2)
/*! Error while registering the event combiner to the defaults */
#define  OSAL_INT_ERR_EVENTCOMBINER_REG         ((int32_t) -3)
/*! Failed to delete a Interrupt handle. */
#define  OSAL_INT_ERR_DELETE                    ((int32_t) -4)
/*! Unsupported function */
#define  OSAL_INT_UNSUPPORTED                   ((int32_t) -5)
/* @} */

/*!
 *  @brief  Function to initialize the interrupt registration configuration data structure
 *
 */
void Osal_RegisterInterrupt_initParams(OsalRegisterIntrParams_t *interruptRegParams);
/*!
 *  @brief  Function to register direct interrupt with parameters provided
 *
 */
OsalInterruptRetCode_e Osal_RegisterInterruptDirect(OsalRegisterIntrParams_t *interruptRegParams,
                                                    HwiP_DirectFxn isrFxn, HwiP_Handle *hwiPHandlePtr);
/*!
 *  @brief  Function to register interrupt with parameters provided
 *
 */
OsalInterruptRetCode_e Osal_RegisterInterrupt(OsalRegisterIntrParams_t *interruptRegParams,HwiP_Handle *hwiPHandlePtr);
/*!
 *  @brief  Function to delete interrupt corresponding to an event number
 *
 */
OsalInterruptRetCode_e Osal_DeleteInterrupt(HwiP_Handle hwiPhandle,uint32_t corepacEventNum);
/*!
 *  @brief  Function to enable the interrupt corresponding to an event number
 *
 */
void Osal_EnableInterrupt(uint32_t corepacEvent,uint32_t interruptNum);
/*!
 *  @brief  Function to disable the  interrupt corresponding to an event number
 *
 */
void Osal_DisableInterrupt(uint32_t corepacEvent,uint32_t interruptNum);
/*!
 *  @brief  Function to clear the interrupt corresponding to an event number
 *
 */
void Osal_ClearInterrupt(uint32_t corepacEvent,uint32_t interruptNum);
#ifdef __cplusplus
}
#endif

#endif /* ti_osal_RegisterIntr__include */
/* @} */
