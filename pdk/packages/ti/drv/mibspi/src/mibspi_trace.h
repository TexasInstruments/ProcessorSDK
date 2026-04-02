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

/*!
 * \file mibspi_trace.h
 *
 * \brief This file contains the main structure definitions and function
 *        prototypes of the tracing functionality.
 */

/*!
 * \addtogroup MIBSPI_TRACE_API
 * @{
 */

#ifndef MIBSPI_TRACE_H_
#define MIBSPI_TRACE_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */



#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/*!
 *  \brief Enumerates the types of trace level
 */
typedef enum
{
    /*! All traces are disabled at runtime */
    MIBSPI_TRACE_NONE    = 0U,

    /*! Error trace level */
    MIBSPI_TRACE_ERROR   = 1U,

    /*! Warning trace level */
    MIBSPI_TRACE_WARN    = 2U,

    /*! Info trace level: enables only important informational messages for the
     * user 
     *
     * The amount of info logs is not invasive in nature so this trace level
     * may be enabled by applications at init time. */
    MIBSPI_TRACE_INFO    = 3U,

    /*! Debug trac level: enables further information messages about operations
     * taking place in the driver (i.e. a submodule is being opened, etc).
     *
     * The debug level should be enabled by the user on a need basis (i.e.
     * for debugging or tracing execution flow, etc) as the number of messages
     * will increase considerably with respect to #MIBSPI_TRACE_INFO level.
     *
     * This trace level can be enabled at runtime only in 'debug' builds. */
    MIBSPI_TRACE_DEBUG   = 4U,

    /*! Verbose trace level: enables even further messages about operations
     * taking place in the driver (i.e. DMA transfer
     * completion, etc) that are periodic in nature or simply happen very often
     * during normal execution.
     *
     * The amount of messages will increase drastically when the verbose level
     * is enabled, so it's recommended to set it only if really needed.
     *
     * This trace level can be enabled at runtime only in 'debug' builds. */
    MIBSPI_TRACE_VERBOSE = 5U,
} MibspiTrace_TraceLevel;

/* ========================================================================== */
/*                         Global Variables Declarations                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Deprecated Function Declarations                    */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* MIBSPI_TRACE_H_ */

/* @} */
