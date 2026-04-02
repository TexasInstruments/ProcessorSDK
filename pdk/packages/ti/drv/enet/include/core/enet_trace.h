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
 * \file  enet_trace.h
 *
 * \brief This file contains the type definitions and helper macros for the
 *        Enet Trace interface.
 */

/*!
 * \ingroup  DRV_ENET_MODULE
 * \defgroup ENET_TRACE_API Enet Trace API
 *
 * @{
 */

#ifndef ENET_TRACE_H_
#define ENET_TRACE_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/*!
 * \name Build-Time Trace Levels
 *
 * Enet LLD supports two types of trace levels: build-time and runtime.
 * Build time trace level is specified via \ref ENET_CFG_TRACE_LEVEL
 * and can be any of the levels in this group.  The runtime trace level is set
 * through Enet_setTraceLevel().
 *
 * The runtime trace level should be set equal to or less than the build-time
 * trace level.
 *
 * @{
 */

/*! \brief All traces disabled at build-time. */
#define ENET_CFG_TRACE_LEVEL_NONE             (0U)

/*! \brief Build-time error level. */
#define ENET_CFG_TRACE_LEVEL_ERROR            (1U)

/*! \brief Build-time warning level. */
#define ENET_CFG_TRACE_LEVEL_WARN             (2U)

/*! \brief Build-time information level. */
#define ENET_CFG_TRACE_LEVEL_INFO             (3U)

/*! \brief Build-time debug level. */
#define ENET_CFG_TRACE_LEVEL_DEBUG            (4U)

/*! \brief Build-time verbose level. */
#define ENET_CFG_TRACE_LEVEL_VERBOSE          (5U)

/*! @} */

/*!
 * \name Trace Formats
 *
 * The following trace formats are supported:
 * - Function prefix: `<func>: string`
 * - File and line number prefix: `<func>: <line>: string`
 * - File, line number and function prefix: `<file>: <line>: <func>: string`
 *
 * Any of these trace formats can also be prefixed with a timestamp.
 *
 * @{
 */

/*! \brief Trace prefix: "<func>: string" */
#define ENET_CFG_TRACE_FORMAT_FUNC            (0U)

/*! \brief Trace prefix: "<file>: <line>: string" */
#define ENET_CFG_TRACE_FORMAT_FILE            (1U)

/*! \brief Trace prefix: "<file>: <line>: <func>: string" */
#define ENET_CFG_TRACE_FORMAT_FULL            (2U)

/*! \brief Trace prefix: "<timestamp>: <func>: string" */
#define ENET_CFG_TRACE_FORMAT_FUNC_TS         (3U)

/*! \brief Trace prefix: "<timestamp>: <file>: <line>: string" */
#define ENET_CFG_TRACE_FORMAT_FILE_TS         (4U)

/*! \brief Trace prefix: "<timestamp>: <file>: <line>: <func>: string" */
#define ENET_CFG_TRACE_FORMAT_FULL_TS         (5U)

/*! @} */

/*!
 * \name Enet Trace Configuration Parameters
 *
 * The default values of the Enet Trace configuration parameters if none is
 * provided via enet_cfg.h.
 *
 * @{
 */

/*! \brief Default trace level if none is set. */
#ifndef ENET_CFG_TRACE_LEVEL
#define ENET_CFG_TRACE_LEVEL                  (ENET_CFG_TRACE_LEVEL_INFO)
#endif

#if (ENET_CFG_TRACE_LEVEL > ENET_CFG_TRACE_LEVEL_VERBOSE)
#error "Invalid Enet trace level"
#endif

/*! \brief Default trace format if none is specified. */
#ifndef ENET_CFG_TRACE_FORMAT
#define ENET_CFG_TRACE_FORMAT                 (ENET_CFG_TRACE_FORMAT_FUNC)
#endif
/*! @} */

/*!
 * \anchor EnetTrace_ErrorCode
 * \name Enet Trace Error Code
 *
 * Enet Trace is capable of generating unique error codes from module id
 * (Enet core, soc, per, mod, phy, etc), line number (where error code was
 * reported) and status (error value reported by the module).
 *
 * The error code is a 32-bit value which is generated as follows:
 * - Bits 31:28 - Module's major number (see `ENET_TRACE_MOD_ID`)
 * - Bits 27:20 - Module's minor number (see `ENET_TRACE_MOD_ID`)
 * - Bits 19:8  - Line number where error was reported by the driver
 * - Bits 7:0   - Status value (positive value)
 *
 * Major Number |     Module     | File location
 * -------------|----------------|------------------------
 *            0 | Enet core      | src/core/\*.c, src/common/\*.c
 *            1 | Peripherals    | src/per/\*.c
 *            2 | Modules        | src/mod/\*.c
 *            3 | DMA            | src/dma/\*.c
 *            4 | Enet SoC       | soc/\*.c
 *            5 | PHY            | src/phy/\*.c
 *
 * Each individual source file in the locations shown in previous table is
 * considered a _module_ and is tagged with a unique `ENET_TRACE_MOD_ID`,
 * denoting the module's major (4-bit) and minor (16-bit) numbers.
 *
 * @{
 */
/*! \brief Module id offset in the error code value */
#define ENET_TRACE_ERRCODE_MOD_OFFSET         (20U)

/*! \brief Module id (major number) offset in the error code value */
#define ENET_TRACE_ERRCODE_MOD_MAJ_OFFSET     (28U)

/*! \brief Module id (minor number) offset in the error code value */
#define ENET_TRACE_ERRCODE_MOD_MIN_OFFSET     (20U)

/*! \brief Line number offset in the error code value */
#define ENET_TRACE_ERRCODE_LINE_OFFSET        (8U)

/*! \brief Status offset in the error code value */
#define ENET_TRACE_ERRCODE_STATUS_OFFSET      (0U)

/*! \brief Module id mask in the error code value */
#define ENET_TRACE_ERRCODE_MOD_MASK           (0xFFF00000U)

/*! \brief Module id (major number) mask in the error code value */
#define ENET_TRACE_ERRCODE_MOD_MAJ_MASK       (0xF0000000U)

/*! \brief Module id (minor number) mask in the error code value */
#define ENET_TRACE_ERRCODE_MOD_MIN_MASK       (0x0FF00000U)

/*! \brief Line number mask in the error code value */
#define ENET_TRACE_ERRCODE_LINE_MASK          (0x000FFF00U)

/*! \brief Status mask in the error code value */
#define ENET_TRACE_ERRCODE_STATUS_MASK        (0x000000FFU)

/*! \brief Helper macro to extract the module id from the error code value */
#define ENET_TRACE_ERRCODE_MOD(x)             (((x) & ENET_TRACE_ERRCODE_MOD_MASK) \
                                               >> ENET_TRACE_ERRCODE_MOD_OFFSET)

/*! \brief Helper macro to extract the module id (major number) from the error code value */
#define ENET_TRACE_ERRCODE_MOD_MAJ(x)         (((x) & ENET_TRACE_ERRCODE_MOD_MAJ_MASK) \
                                               >> ENET_TRACE_ERRCODE_MOD_MAJ_OFFSET)

/*! \brief Helper macro to extract the module id (minor number) from the error code value */
#define ENET_TRACE_ERRCODE_MOD_MIN(x)         (((x) & ENET_TRACE_ERRCODE_MOD_MIN_MASK) \
                                               >> ENET_TRACE_ERRCODE_MOD_MIN_OFFSET)

/*! \brief Helper macro to extract the line number from the error code value */
#define ENET_TRACE_ERRCODE_LINE(x)            (((x) & ENET_TRACE_ERRCODE_LINE_MASK) \
                                               >> ENET_TRACE_ERRCODE_LINE_OFFSET)

/*! \brief Helper macro to extract the status value from the error code value */
#define ENET_TRACE_ERRCODE_STATUS(x)          (((x) & ENET_TRACE_ERRCODE_STATUS_MASK) \
                                               >> ENET_TRACE_ERRCODE_STATUS_OFFSET)
/*! @} */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/*!
 * \brief Enumerates the types of trace level.
 */
typedef enum
{
    /*! All traces are disabled at runtime */
    ENET_TRACE_NONE    = 0U,

    /*! Error trace level */
    ENET_TRACE_ERROR   = 1U,

    /*! Warning trace level */
    ENET_TRACE_WARN    = 2U,

    /*! Info trace level: enables only important informational messages for the
     * user (i.e. PHY link is up or down, NIMU layer is ready, etc).
     *
     * The amount of info logs is not invasive in nature so this trace level
     * may be enabled by applications at init time. */
    ENET_TRACE_INFO    = 3U,

    /*! Debug trace level: enables further information messages about operations
     * taking place in the driver (i.e. a module is being opened, PHY
     * auto-negotiation is started, etc).
     *
     * The debug level should be enabled by the user on a need basis (i.e.
     * for debugging or tracing execution flow, etc) as the number of messages
     * will increase considerably with respect to #ENET_TRACE_INFO level.
     *
     * This trace level can be enabled at runtime only in 'debug' builds. */
    ENET_TRACE_DEBUG   = 4U,

    /*! Verbose trace level: enables even further messages about operations
     * taking place in the driver (i.e. PHY state transitions, DMA transfer
     * completion, etc) that are periodic in nature or simply happen very often
     * during normal execution.
     *
     * The amount of messages will increase drastically when the verbose level
     * is enabled, so it's recommended to set it only if really needed.
     *
     * This trace level can be enabled at runtime only in 'debug' builds. */
    ENET_TRACE_VERBOSE = 5U,
} EnetTrace_TraceLevel;

/*!
 * \brief Callback function used to get trace timestamps.
 *
 * Callback function called when \ref ENET_CFG_TRACE_FORMAT is set to
 * \ref ENET_CFG_TRACE_FORMAT_FUNC_TS, \ref ENET_CFG_TRACE_FORMAT_FILE_TS
 * or ENET_CFG_TRACE_FORMAT_FULL_TS.
 *
 * The timestamp value must be in microseconds.
 */
typedef uint64_t (*EnetTrace_TraceTsFunc)(void);

/*!
 * \brief Callback function called to report errors.
 *
 * Callback function called by EnetTrace when Enet driver has reported an
 * error.  A unique 32-bit error code is passed to the callback, which encodes
 * information about the module where error occurred, line of code and the
 * status value.
 *
 * Application can use the helper macros \ref ENET_TRACE_ERRCODE_MOD,
 * \ref ENET_TRACE_ERRCODE_MOD_MAJ, \ref ENET_TRACE_ERRCODE_MOD_MIN,
 * \ref ENET_TRACE_ERRCODE_LINE and \ref ENET_TRACE_ERRCODE_STATUS to decode
 * the error code.
 */
typedef void (*EnetTrace_ExtTraceFunc)(uint32_t errCode);

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

#endif /* ENET_TRACE_H_ */

/*! @} */
