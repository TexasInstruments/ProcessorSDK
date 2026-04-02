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
 * \file  enet_trace.c
 *
 * \brief This file contains the implementation of the Enet trace feature.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdarg.h>
#include <ti/drv/enet/include/core/enet_trace.h>
#include <ti/drv/enet/include/core/enet_utils.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Line terminator to be used by all ENETTRACE macros */
#define ENET_TRACE_LINE_TERM                  "\n"

/* Helper macro to set the module id in the error code */
#define ENET_TRACE_ERRCODE_SET_MOD(x)         ((x) << ENET_TRACE_ERRCODE_MOD_OFFSET)

/* Helper macro to set the line number in the error code */
#define ENET_TRACE_ERRCODE_SET_LINE(x)        ((x) << ENET_TRACE_ERRCODE_LINE_OFFSET)

/* Helper macro to set the status value in the error code */
#define ENET_TRACE_ERRCODE_SET_STATUS(x)      ((x) << ENET_TRACE_ERRCODE_STATUS_OFFSET)

/* Helper macro to generate an error code from module id, line number
 * and status value */
#define ENET_TRACE_ERRCODE(mod, line, err)    (ENET_TRACE_ERRCODE_SET_MOD(mod) |   \
                                               ENET_TRACE_ERRCODE_SET_LINE(line) | \
                                               ENET_TRACE_ERRCODE_SET_STATUS(err))
/* Microseconds in a second */
#define ENET_TRACE_SEC_TO_USEC                (1000000ULL)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if (ENET_CFG_TRACE_LEVEL > ENET_CFG_TRACE_LEVEL_NONE)
EnetTrace_TraceLevel gEnetTrace_runtimeLevel = ENET_TRACE_INFO;
#else
EnetTrace_TraceLevel gEnetTrace_runtimeLevel = ENET_TRACE_NONE;
#endif

EnetTrace_TraceTsFunc gEnetTrace_tsFunc = NULL;

EnetTrace_ExtTraceFunc gEnetTrace_extTraceFunc = NULL;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#if (ENET_CFG_TRACE_LEVEL > ENET_CFG_TRACE_LEVEL_NONE)
void EnetTrace_trace(EnetTrace_TraceLevel globalLevel,
                     EnetTrace_TraceLevel level,
                     uint32_t mod,
                     const char *file,
                     uint32_t line,
                     const char *func,
                     uint32_t status,
                     const char *fmt,
                     ...)
{
#if (ENET_CFG_TRACE_FORMAT >= ENET_CFG_TRACE_FORMAT_FUNC_TS)
    uint64_t tsUsecs = 0ULL;
#endif
    va_list ap;
    uint32_t errCode;

    /* Check if specified level is enabled */
    if (globalLevel >= level)
    {
        /* Generate unique error code and call extended trace callback */
        if ((status != ENET_SOK) &&
            (gEnetTrace_extTraceFunc != NULL))
        {
            errCode = ENET_TRACE_ERRCODE(mod, line, (uint8_t)(-1 * status));
            gEnetTrace_extTraceFunc(errCode);
        }

#if (ENET_CFG_TRACE_FORMAT >= ENET_CFG_TRACE_FORMAT_FUNC_TS)
        if (gEnetTrace_tsFunc != NULL)
        {
            tsUsecs = gEnetTrace_tsFunc();
        }

        EnetUtils_printf("%6u.%06u: ",
                         (uint32_t)(tsUsecs / ENET_TRACE_SEC_TO_USEC),
                         (uint32_t)(tsUsecs % ENET_TRACE_SEC_TO_USEC));
#endif

        va_start(ap, fmt);
        EnetUtils_vprintf(fmt, ap);
        va_end(ap);

        if (status != ENET_SOK)
        {
            EnetUtils_printf(": %d" ENET_TRACE_LINE_TERM, status);
        }
        else
        {
            EnetUtils_printf(ENET_TRACE_LINE_TERM);
        }
    }
}
#endif

EnetTrace_TraceLevel EnetTrace_getLevel(void)
{
    return gEnetTrace_runtimeLevel;
}

EnetTrace_TraceLevel EnetTrace_setLevel(EnetTrace_TraceLevel level)
{
    EnetTrace_TraceLevel prevLevel = gEnetTrace_runtimeLevel;

    gEnetTrace_runtimeLevel = level;

    return prevLevel;
}

void EnetTrace_setTsFunc(EnetTrace_TraceTsFunc func)
{
    gEnetTrace_tsFunc = func;
}

void EnetTrace_setExtTraceFunc(EnetTrace_ExtTraceFunc func)
{
    gEnetTrace_extTraceFunc = func;
}
