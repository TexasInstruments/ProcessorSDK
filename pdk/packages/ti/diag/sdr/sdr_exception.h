/*
 * SDR EXCEPTION
 *
 * Software Diagnostics Reference module for Exception handling
 *
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
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

/**
 * @file  sdr_exception.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDR EXCEPTION interface.
 *
 *  The SDR exception data structures include:
 *      1. Structure of call back functions for various exception events
 *
 *  The SDR exception APIs include:
 *      1. API to execute SDR Exception module initialization
 *      2. APIs to handle various exceptions
 */

#ifndef INCLUDE_SDR_EXCEPTION_H_
#define INCLUDE_SDR_EXCEPTION_H_
#include "sdr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDR_EXCEPTION_API SDR EXCEPTION API
 *
 * ============================================================================
 */
/**
@defgroup SDR_EXCEPTION_DATASTRUCT  SDR EXCEPTION Data Structures
@ingroup SDR_EXCEPTION_API
*/
/**
@defgroup SDR_EXCEPTION_FUNCTION  SDR EXCEPTION Functions
@ingroup SDR_EXCEPTION_API
*/
/**
@defgroup SDR_EXCEPTION_MACROS SDR EXCEPTION Macro defines
@ingroup SDR_EXCEPTION_API
*/

/**
 *  \addtogroup SDR_EXCEPTION_MACROS
 *  @{
 */


/** \brief Format of Call back function for ECC error events */
typedef void (*SDR_EXCEPTION_ECCCallback_t) (uint32_t intSource,
                                             uint32_t errorAddr,
                                             uint32_t ramId,
                                             uint64_t bitErrorOffset,
                                             uint32_t bitErrorGroup);

/** \brief Format of Call back function for exception */
typedef void (*SDR_EXCEPTION_Callback_t) (void);

/* @} */

/**
 *  \addtogroup SDR_EXCEPTION_DATASTRUCT
 *  @{
 */
/** \brief Structure of call back functions for various exception events */
typedef struct SDR_EXCEPTION_CallbackFunctions_s
{
    SDR_EXCEPTION_Callback_t udefExptnCallback;
    /**< Undefined Instruction exception callback function */
    SDR_EXCEPTION_Callback_t swiExptnCallback;
    /**< Software Interrupt exception callback function  */
    SDR_EXCEPTION_Callback_t pabtExptnCallback;
    /**< Prefetch Abort exception callback function */
    SDR_EXCEPTION_Callback_t dabtExptnCallback;
    /**< Data Abort exception callback function */
    SDR_EXCEPTION_Callback_t irqExptnCallback;
    /**< Interrupt Request exception callback function.
     *   This will be used only if VIM is not enabled.
     */
    SDR_EXCEPTION_Callback_t fiqExptnCallback;
    /**< Fast Interrupt Request callback function.
     *   This will be used only if VIM is not enabled.
     */
}SDR_EXCEPTION_CallbackFunctions_t;


/* @} */

/**
 *  \addtogroup SDR_EXCEPTION_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief   Initialise Exception module
 *
 * \param  callbackFunctions: Pointer to callback Functions structure
 *
 * \return  None
 */
void SDR_EXCEPTION_init(const SDR_EXCEPTION_CallbackFunctions_t *callbackFunctions);

/** ============================================================================
 *
 * \brief   Function to register ECC exception handler
 *
 * \param  ECCCallBackFunctionPtr: callback function to register
 *
 * \return  None
 */
void SDR_EXCEPTION_registerECCHandler(SDR_EXCEPTION_ECCCallback_t ECCCallBackFunctionPtr);

/** ============================================================================
 *
 * \brief  Undefined Instruction Exception Handler
 *
 * \param  param: Parameter pointer
 *
 * \return  None
 */
void SDR_EXCEPTION_undefInstructionExptnHandler(void *param);

/** ============================================================================
 *
 * \brief  SW Interrupt Exception Handler
 *
 * \param  param: Parameter pointer
 *
 * \return  None
 */
void SDR_EXCEPTION_swIntrExptnHandler(void *param);

/** ============================================================================
 *
 * \brief  Prefetch Abort Exception Handler
 *
 * \param  param: Parameter pointer
 *
 * \return  None
 */
void SDR_EXCEPTION_prefetchAbortExptnHandler(void *param);

/** ============================================================================
 *
 * \brief  Data Abort Exception Handler
 *
 * \param  param: Parameter pointer
 *
 * \return  None
 */
void SDR_EXCEPTION_dataAbortExptnHandler(void *param);

/** ============================================================================
 *
 * \brief  IRQ Exception Handler
 *
 * \param  param: Parameter pointer
 *
 * \return  None
 */
void SDR_EXCEPTION_irqExptnHandler(void *param);

/** ============================================================================
 *
 * \brief  FIQ Exception Handler
 *
 * \param  param: Parameter pointer
 *
 * \return  None
 */
void SDR_EXCEPTION_fiqExptnHandler(void *param);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDR_EXCEPTION_H_ */
