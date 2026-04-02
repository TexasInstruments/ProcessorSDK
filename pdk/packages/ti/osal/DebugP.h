/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
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
/** @defgroup DEBUGP_OSAL      DebugP OSAL Porting Layer
 *
 * @brief      Debug support
 *
 *  The DebugP module allows application to do logging and assert checking.
 *
 *  DebugP_assert calls can be added into code. If the code
 *  is compiled with the compiler define DebugP_ASSERT_ENABLED set to a
 *  non-zero value, the call is passed onto the underlying assert checking.
 *  If DebugP_ASSERT_ENABLED is zero (or not defined), the calls are
 *  resolved to nothing.
 *
 *  This module sits on top of the assert checking of the underlying
 *  RTOS. Please refer to the underlying RTOS port implementation for
 *  more details.
 *
 *  Similarly, DebugP_logN calls can be added into code. If the code
 *  is compiled with the compiler define DebugP_LOG_ENABLED set to a
 *  non-zero value, the call is passed onto the underlying assert checking.
 *  If DebugP_LOG_ENABLED is zero (or not defined), the calls are
 *  resolved to nothing.

 *  This module sits on top of the logging of the underlying
 *  RTOS. Please refer to the underlying RTOS port implementation for
 *  more details.
 */

/**
 *  \ingroup DRV_OSAL_MODULE
 *  \defgroup DRV_OSAL_DebugP DebugP
 *            DebugP interface
 *
 *  @{
 */

#ifndef ti_drivers_ports_DebugP__include
#define ti_drivers_ports_DebugP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#define DebugP_ASSERT_ENABLED 1
#define DebugP_LOG_ENABLED    1

/*!
 *  @brief    Assert Enabled
 *
 *  The defintion is used to control the feature if assertions are to be enabled
 *  or not. This flag can be enabled for debug builds by changing the file *OR*
 *  through the makefile.
 */
#ifndef DebugP_ASSERT_ENABLED
#define DebugP_ASSERT_ENABLED 0
#endif

/*!
 *  @brief    Log Enabled
 *
 *  The defintion is used to plug the logging API to the OS provided logging mechansim
 *  Application developers can port the backend Logging API as per their requirements
 */
#ifndef DebugP_LOG_ENABLED
#define DebugP_LOG_ENABLED    0
#endif

#if DebugP_LOG_ENABLED

/*!
 *  @brief    DebugP Log Function signature to be used for printing
 */
typedef void (*DebugP_exptnLogFxn)(const char *format, ...);

/*!
 *  @brief    DebugP Log Function registration return types
 *
 *  DEBUGP_PRINTFXN_REGISTER_SUCCESS is returned on succesful registration of log API.
 *  DEBUGP_PRINTFXN_ALREADY_REGISTERD is returned, if an API is already registered.
 */
#define DEBUGP_LOGFXN_REGISTER_SUCCESS   (0)
#define DEBUGP_LOGFXN_ALREADY_REGISTERD  (-1)
#endif

/*!
 *  @brief  Assert checking function
 *
 *  If the expression is evaluated to true, the API does nothing.
 *  If it is evaluated to false, the underlying RTOS port implementation
 *  handles the assert via its mechanisms.
 *
 *  @param  expression Expression to evaluate
 *  @param  file       File name
 *  @param  line       Line number where evaluation occurs
 *
 *  @sa DebugP_ASSERT_ENABLED
 *
 */
#if DebugP_ASSERT_ENABLED
extern void Osal_DebugP_assert_fcn(bool expression, const char *file, int32_t line);
#define DebugP_assert(expression) (Osal_DebugP_assert_fcn(expression,         \
                                                         __FILE__, __LINE__))
#else
#define DebugP_assert(expression)
#endif

/*!
 *  @brief  DebugP Exception state log function.
 *          It takes a pair of values to be printed during exception dump.
 *
 *
 *  @param  format "printf" format string
 *  @param  arg1 first parameter to format string
 *  @param  arg2 second parameter to format string
 *
 *  @sa DebugP_LOG_ENABLED
 *
 */
#if DebugP_LOG_ENABLED
extern void DebugP_exceptionLog(const char * format, uint32_t arg1, uint32_t arg2);
#else
#define DebugP_exceptionLog(const char * format, uint32_t arg1, uint32_t arg2)
#endif

/*!
 *  @brief  Debug log function with 0 parameters
 *
 *  The underlying RTOS port implementation handles the
 *  logging via its mechanisms.
 *
 *  @param  format "printf" format string
 *
 *  @sa DebugP_LOG_ENABLED
 *
 */
#if DebugP_LOG_ENABLED
extern void DebugP_log0(const char *format);
#else
#define DebugP_log0(format)
#endif

/*!
 *  @brief  Debug log function registration
 *
 *  Application needs to call this API to register its print API.
 *
 *  @param  fxn Function pointer for print function to be registerd.
 *
 *  @sa DebugP_LOG_ENABLED
 *
 */
#if DebugP_LOG_ENABLED
int32_t DebugP_registerExcptnLogFxn(DebugP_exptnLogFxn fxn);
#else
#define DebugP_registerExcptnLogFxn(DebugP_exptnLogFxn fxn)
#endif

/*!
 *  @brief  Debug log function deregistration
 *
 *  Application needs to call this API to de-register its print API.
 *
 *
 *  @sa DebugP_LOG_ENABLED
 *
 */
#if DebugP_LOG_ENABLED
void DebugP_deRegisterExcptnLogFxn(void);
#else
#define DebugP_deRegisterExcptnLogFxn(void)
#endif

/*!
 *  @brief  Debug log function with 1 parameters
 *
 *  The underlying RTOS port implementation handles the
 *  logging via its mechanisms.
 *
 *  @param  format "printf" format string
 *  @param  p1 first parameter to format string
 *
 */
#if DebugP_LOG_ENABLED
extern void DebugP_log1(const char *format, uintptr_t p1);
#else
#define DebugP_log1(format, p1)
#endif

/*!
 *  @brief  Debug log function with 2 parameters
 *
 *  The underlying RTOS port implementation handles the
 *  logging via its mechanisms.
 *
 *  @param  format "printf" format string
 *  @param  p1 first parameter to format string
 *  @param  p2 second parameter to format string
 *
 */

#if DebugP_LOG_ENABLED
extern void DebugP_log2(const char *format, uintptr_t p1, uintptr_t p2);
#else
#define DebugP_log2(format, p1, p2)
#endif

/*!
 *  @brief  Debug log function with 3 parameters
 *
 *  The underlying RTOS port implementation handles the
 *  logging via its mechanisms.
 *
 *  @param  format "printf" format string
 *  @param  p1 first parameter to format string
 *  @param  p2 second parameter to format string
 *  @param  p3 third parameter to format string
 *
 */
#if DebugP_LOG_ENABLED
extern void DebugP_log3(const char *format, uintptr_t p1, uintptr_t p2, uintptr_t p3);
#else
#define DebugP_log3(format, p1, p2, p3)
#endif

/*!
 *  @brief  Debug log function with 4 parameters
 *
 *  The underlying RTOS port implementation handles the
 *  logging via its mechanisms.
 *
 *  @param  format "printf" format string
 *  @param  p1 first parameter to format string
 *  @param  p2 second parameter to format string
 *  @param  p3 third parameter to format string
 *  @param  p4 fourth parameter to format string
 *
 */
#if DebugP_LOG_ENABLED
extern void DebugP_log4(const char *format, uintptr_t p1, uintptr_t p2, uintptr_t p3, uintptr_t p4);
#else
#define DebugP_log4(format, p1, p2, p3, p4)
#endif

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_ports_DebugP__include */
/* @} */
