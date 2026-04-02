/*
 *  Copyright (c) Texas Instruments Incorporated 2019
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
 * \file mibspi_trace_config.h
 *
 * \brief This file contains the trace's build-time configuration
 */

#ifndef MIBSPI_TRACE_CONFIG_H_
#define MIBSPI_TRACE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdlib.h>
#include <stdint.h>

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/** \brief All traces disabled at build-time */
#define MIBSPI_TRACE_CFG_LEVEL_NONE             (0U)

/** \brief Build-time error level */
#define MIBSPI_TRACE_CFG_LEVEL_ERROR            (1U)

/** \brief Build-time warning level */
#define MIBSPI_TRACE_CFG_LEVEL_WARN             (2U)

/** \brief Build-time information level */
#define MIBSPI_TRACE_CFG_LEVEL_INFO             (3U)

/** \brief Build-time debug level */
#define MIBSPI_TRACE_CFG_LEVEL_DEBUG            (4U)

/** \brief Build-time verbose level */
#define MIBSPI_TRACE_CFG_LEVEL_VERBOSE          (5U)

/* Default trace level if none is set */
#ifndef MIBSPI_TRACE_CFG_TRACE_LEVEL
#define MIBSPI_TRACE_CFG_TRACE_LEVEL            (MIBSPI_TRACE_CFG_LEVEL_INFO)
#endif

/*! \brief Trace prefix: "<func>: string" */
#define MIBSPI_TRACE_CFG_FORMAT_FUNC            (0U)

/*! \brief Trace prefix: "<func>: <line>: string" */
#define MIBSPI_TRACE_CFG_FORMAT_FILE            (1U)

/*! \brief Trace prefix: "<file>: <line>: <func>: <line>: string" */
#define MIBSPI_TRACE_CFG_FORMAT_FULL            (2U)

/* Default trace format if none is specified */
#ifndef MIBSPI_TRACE_CFG_TRACE_FORMAT
#define MIBSPI_TRACE_CFG_TRACE_FORMAT           (MIBSPI_TRACE_CFG_FORMAT_FUNC)
#endif

/*! \brief MIBSPI print buffer length */
#define MIBSPI_CFG_PRINT_BUF_LEN                      (200U)

/*! \brief Maximum length of a single Trace_print string */
#define MIBSPI_CFG_TRACE_BUF_LEN                      (200U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

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

#endif /* MIBSPI_TRACE_CONFIG_H_ */
