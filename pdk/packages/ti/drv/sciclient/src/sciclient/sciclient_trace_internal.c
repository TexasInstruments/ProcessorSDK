/*
 *  Copyright (C) 2025 Texas Instruments Incorporated
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <lib/trace.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/sciclient/src/sciclient/sciclient_trace_internal.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief Function to store character in buffer.
 *
 * \param ch Character to output into memory buffer.
 */
static void Sciclient_traceBuffer(char ch);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* gDMTraceBufIndex macro is last index where trace has already been updated */
uint32_t gDMTraceBufIndex = 0U;

/* gDMTraceBufCount macros is used to track the total number of times trace has been updated */
uint32_t gDMTraceBufCount = 0U;

/* tracelog_dm is used to track the tracelogbuffer defined by user */
static char* tracelog_dm = NULL;

/* gDMTracelogBufsize macro is used to track the size of the tracelogbuffer defined by user */
static uint32_t gDMTracelogBufsize = 0U;

/*
 * gSciclientTraceErrorFlag macro is a static error flag created
 * to ensure the invalid buffer error is logged only once.
 */
static int32_t gSciclientTraceErrorFlag = 0;

/*
 * Memory buffer starts storing logs in tracelog_dm only when the variable gStoreLog is set to 1.
 * This is used to prevent DM init trace logs and can be set to one if DM init trace logs are required.
 */
uint32_t gStoreLog = 0;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Sciclient_traceBufferString(const char *str,...)
{
    if(gStoreLog != 0)
    {
        int32_t buffer_index = 0;
        char buffer[TRACE_PRINT_MAX_LENGTH];
        va_list args;

        va_start(args, str);
        vsprintf(buffer, str, args);
        va_end(args);

        if((0 != gDMTracelogBufsize) && (NULL != tracelog_dm))
        {
            for (buffer_index = 0; buffer_index < TRACE_PRINT_MAX_LENGTH; buffer_index++)
            {
                if (buffer[buffer_index] != '\0')
                {
                    Sciclient_traceBuffer(buffer[buffer_index]);
                }

                if (buffer[buffer_index] == '\0')
                {
                    break;
                }
            }
        }
        else
        {
            if(0 == gSciclientTraceErrorFlag)
            {
                UART_printf("Please use Sciclient_setTracelogBuffer() with correct Params");
                gSciclientTraceErrorFlag = 1;
            }
        }
    }

    return;
}

static void Sciclient_traceBuffer(char ch)
{
    tracelog_dm[gDMTraceBufIndex++] = ch;

    if (gDMTraceBufIndex == gDMTracelogBufsize)
    {
        gDMTraceBufIndex = 0;
        gDMTraceBufCount++;
    }

    return;
}

void Sciclient_setTracelogBuffer(char* buffer, uint32_t buffersize)
{
    tracelog_dm = buffer;
    gDMTracelogBufsize = buffersize;
}