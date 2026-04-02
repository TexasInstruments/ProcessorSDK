/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 */

/**
 * \file       mibspi_trace_priv.h
 *
 * \brief      This file has the internal function definitions of the
 *             trace module.
 */

#ifndef MIBSPI_TRACE_PRIV_H_
#define MIBSPI_TRACE_PRIV_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include "mibspi_trace.h"
#include "mibspi_trace_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/**
 * \brief Log a trace message if log level is enabled
 *
 * Log trace messages for log levels that are enabled at runtime.
 *
 * \param  globalLevel  [IN] Trace module global level
 * \param  level        [IN] Trace level intended to be logged
 * \param  infoString   [IN] Print string
 */
void MibspiTrace_trace(MibspiTrace_TraceLevel globalLevel,
                       MibspiTrace_TraceLevel level,
                       const char *infoString,
                       ...);

/* Trace prefix type */
#if (MIBSPI_TRACE_CFG_TRACE_LEVEL > MIBSPI_TRACE_CFG_LEVEL_NONE)
#if (MIBSPI_TRACE_CFG_TRACE_FORMAT == MIBSPI_TRACE_CFG_FORMAT_FUNC)
/* Trace prefix: "<func>: infoString" */
#define MIBSPITRACE_trace(globalLevel, level, infoString, ...) \
    MibspiTrace_trace((globalLevel), (level),                  \
                    "%s: " infoString,                       \
                    __func__, ## __VA_ARGS__)

#elif (MIBSPI_TRACE_CFG_TRACE_FORMAT == MIBSPI_TRACE_CFG_FORMAT_FILE)
/* Trace prefix: "<file>: <line>: infoString" */
#define MIBSPITRACE_trace(globalLevel, level, infoString, ...) \
    MibspiTrace_trace((globalLevel), (level),                  \
                    "%s: %d: " infoString,                   \
                    __FILE__, __LINE__, ## __VA_ARGS__)
#else
/* Trace prefix: "<file>: <line>: <func>: infoString" */
#define MIBSPITRACE_trace(globalLevel, level, infoString, ...) \
    MibspiTrace_trace((globalLevel), (level),                  \
                    "%s: %d: %s: " infoString,               \
                    __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#endif
#else /* MIBSPI_TRACE_CFG_LEVEL_NONE */
#define MIBSPITRACE_trace(globalLevel, level, infoString, ...)
#endif

/**
 * \brief Helper macro to add trace message with #MIBSPI_TRACE_ERROR level
 */
#if (MIBSPI_TRACE_CFG_TRACE_LEVEL >= MIBSPI_TRACE_CFG_LEVEL_ERROR)
#define MIBSPITRACE_ERROR(infoString, ...) MIBSPITRACE_trace(MibspiTrace_runtimeLevel, \
                                                         MIBSPI_TRACE_ERROR,       \
                                                         infoString,             \
                                                         ## __VA_ARGS__)
/*TODO: Replace the below #ifdef with a method applicable for both C & C++ */
#ifdef __cplusplus
#define MIBSPITRACE_ERROR_IF(cond, ...) ((cond) ? MIBSPITRACE_ERROR(__VA_ARGS__) : void())
#else
#define MIBSPITRACE_ERROR_IF(cond, ...) ((cond) ? MIBSPITRACE_ERROR(__VA_ARGS__) : 0U)
#endif
#else
#define MIBSPITRACE_ERROR(infoString, ...)
#define MIBSPITRACE_ERROR_IF(cond, ...)
#endif

/**
 * \brief Helper macro to add trace message with #MIBSPI_TRACE_WARN level
 */
#if (MIBSPI_TRACE_CFG_TRACE_LEVEL >= MIBSPI_TRACE_CFG_LEVEL_WARN)
#define MIBSPITRACE_WARN(infoString, ...) MIBSPITRACE_trace(MibspiTrace_runtimeLevel, \
                                                        MIBSPI_TRACE_WARN,        \
                                                        infoString,             \
                                                        ## __VA_ARGS__)
#ifdef __cplusplus
#define MIBSPITRACE_WARN_IF(cond, ...) ((cond) ? MIBSPITRACE_WARN(__VA_ARGS__) : void())
#else
#define MIBSPITRACE_WARN_IF(cond, ...) ((cond) ? MIBSPITRACE_WARN(__VA_ARGS__) : 0U)
#endif
#else
#define MIBSPITRACE_WARN(infoString, ...)
#define MIBSPITRACE_WARN_IF(cond, ...)
#endif

/**
 * \brief Helper macro to add trace message with #MIBSPI_TRACE_INFO level
 *
 * Traces with this level should give only important informational
 * messages to the user, which typically they don't occur very often
 * (i.e. "MIBSPI NIMU is ready", "PHY n link is up").
 * This trace level may be enabled by default.
 */
#if (MIBSPI_TRACE_CFG_TRACE_LEVEL >= MIBSPI_TRACE_CFG_LEVEL_INFO)
#define MIBSPITRACE_INFO(infoString, ...) MIBSPITRACE_trace(MibspiTrace_runtimeLevel, \
                                                        MIBSPI_TRACE_INFO,        \
                                                        infoString,             \
                                                        ## __VA_ARGS__)
#ifdef __cplusplus
#define MIBSPITRACE_INFO_IF(cond, ...) ((cond) ? MIBSPITRACE_INFO(__VA_ARGS__) : void())
#else
#define MIBSPITRACE_INFO_IF(cond, ...) ((cond) ? MIBSPITRACE_INFO(__VA_ARGS__) : 0U)
#endif
#else
#define MIBSPITRACE_INFO(infoString, ...)
#define MIBSPITRACE_INFO_IF(cond, ...)
#endif

/**
 * \brief Helper macro to add trace message with #MIBSPI_TRACE_DEBUG level
 *
 * Traces with this level will provide the user further information
 * about operations taking place (i.e. "MDIO module is open", "PHY n has
 * started auto-negotiation, etc).
 * This trace level is likely not enabled by default, so most of driver's
 * traces will follow in this category.
 */
#if (MIBSPI_TRACE_CFG_TRACE_LEVEL >= MIBSPI_TRACE_CFG_LEVEL_DEBUG)
#define MIBSPITRACE_DEBUG(infoString, ...) MIBSPITRACE_trace(MibspiTrace_runtimeLevel, \
                                                         MIBSPI_TRACE_DEBUG,       \
                                                         infoString,             \
                                                         ## __VA_ARGS__)
#ifdef __cplusplus
#define MIBSPITRACE_DEBUG_IF(cond, ...) ((cond) ? MIBSPITRACE_DEBUG(__VA_ARGS__) : void())
#else
#define MIBSPITRACE_DEBUG_IF(cond, ...) ((cond) ? MIBSPITRACE_DEBUG(__VA_ARGS__) : 0U)
#endif
#else
#define MIBSPITRACE_DEBUG(infoString, ...)
#define MIBSPITRACE_DEBUG_IF(cond, ...)
#endif

/**
 * \brief Helper macro to add trace message with #MIBSPI_TRACE_VERBOSE level
 *
 * Traces with this level will provide even more information and
 * much more often than the DEBUG level (i.e. "PHY n: NWAY_WAIT state",
 * "DMA transfer is complete")
 * Enabling this trace level is likely going to flood with messages, so
 * developers must ensure that their debug messages that occur often
 * enough are set with VERBOSE level.
 */
#if (MIBSPI_TRACE_CFG_TRACE_LEVEL >= MIBSPI_TRACE_CFG_LEVEL_VERBOSE)
#define MIBSPITRACE_VERBOSE(infoString, ...) MIBSPITRACE_trace(MibspiTrace_runtimeLevel, \
                                                           MIBSPI_TRACE_VERBOSE,     \
                                                           infoString,             \
                                                           ## __VA_ARGS__)
#ifdef __cplusplus
#define MIBSPITRACE_VERBOSE_IF(cond, ...) ((cond) ? MIBSPITRACE_VERBOSE(__VA_ARGS__) : void())
#else
#define MIBSPITRACE_VERBOSE_IF(cond, ...) ((cond) ? MIBSPITRACE_VERBOSE(__VA_ARGS__) : 0U)
#endif
#else
#define MIBSPITRACE_VERBOSE(infoString, ...)
#define MIBSPITRACE_VERBOSE_IF(cond, ...)
#endif

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Global Variables Declarations                      */
/* ========================================================================== */

/**
 * \brief Enabled runtime trace level
 */
extern MibspiTrace_TraceLevel MibspiTrace_runtimeLevel;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief Set runtime trace level
 *
 * Set the driver's runtime travel level
 *
 * \param level    [IN] Trace level to be enabled
 * \return Previous trace level
 */
MibspiTrace_TraceLevel MibspiTrace_setLevel(MibspiTrace_TraceLevel level);

/**
 * \brief Get runtime trace level
 *
 * Get the driver's runtime travel level
 *
 * \return Current trace level
 */
MibspiTrace_TraceLevel MibspiTrace_getLevel(void);

/* ========================================================================== */
/*                        Deprecated Function Declarations                    */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* MIBSPI_TRACE_PRIV_H_ */
