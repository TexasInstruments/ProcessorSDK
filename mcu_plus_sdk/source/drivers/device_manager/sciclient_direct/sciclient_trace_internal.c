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
#include <drivers/device_manager/sciclient_direct/sciclient_trace_internal.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define TRACE_PRINT_MAX_LENGTH                           (255U)

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief Function to store character in buffer.
 *
 * \param ch Character to output into memory buffer.
 */
static void Dm_traceBuffer(char ch);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* gDMTraceBufIndex macro is last index in which tracelog_dm is updated */
uint32_t gDMTraceBufIndex = 0U;

/* gDMTraceBufCount macros is used to track the total number of times tracelog_dm is updated */
uint32_t gDMTraceBufCount = 0U;

/*
 * Memory buffer starts storing logs in tracelog_dm only when the variable gStoreLog is set to 1.
 * This is used to prevent DM init trace logs and can be set to one if DM init trace logs are required.
 */
uint32_t gStoreLog = 0;

char tracelog_dm[DM_TRACE_LOG_BUF_SIZE] = { '\0' };

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Dm_traceBufferString(const char *str,...)
{
    if(gStoreLog != 0)
    {
        int32_t buffer_index = 0;
        char buffer[TRACE_PRINT_MAX_LENGTH];
        va_list args;

        va_start(args, str);
        vsprintf(buffer, str, args);
        va_end(args);

        for (buffer_index = 0; buffer_index < TRACE_PRINT_MAX_LENGTH; buffer_index++)
        {
            if (buffer[buffer_index] != '\0')
            {
                Dm_traceBuffer(buffer[buffer_index]);
            }

            if (buffer[buffer_index] == '\0')
            {
                break;
            }
        }
    }

    return;
}

static void Dm_traceBuffer(char ch)
{
    tracelog_dm[gDMTraceBufIndex++]=ch;

    if (gDMTraceBufIndex == DM_TRACE_LOG_BUF_SIZE)
    {
        gDMTraceBufIndex = 0;
        gDMTraceBufCount++;
    }

    return;
}