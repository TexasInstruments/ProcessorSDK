/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   osal_extended_testapp_cycleProfiler.c
 *
 *  \brief  OSAL CycleProfiler Sub Module testcase file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Function Declarations                           */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                      Internal Function Definitions                         */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_cycleProfilerTest(void)
{
    uint32_t time, timeChigh, timeClow, timeAhigh, timeAlow;
    uint64_t timeC, timeA, timeDiff, limitLow, limitHigh;
    TimeStamp_Struct timestamp64;
    int32_t result = osal_OK;

    OSAL_log("\n Cycle Profile nonos Test:\n\t  wait for 5 seconds\n");
    CSL_armR5PmuResetCycleCnt();
    CycleprofilerP_init();

    osalArch_TimestampGet64(&timestamp64);
    timeAhigh = timestamp64.hi;
    timeAlow = timestamp64.lo;
    time = CycleprofilerP_getTimeStamp();
    DebugP_assert((time - timeAlow) < 2000U);
    timeA = ((uint64_t)timeAhigh << 32U) | (timeAlow);

    /* Delay of 2 seconds */
    Osal_delay(2000);

    CycleprofilerP_refreshCounter();

    /* Delay of 3 seconds */
    Osal_delay(3000);

    osalArch_TimestampGet64(&timestamp64);
    timeChigh = timestamp64.hi;
    timeClow = timestamp64.lo;
    time = CycleprofilerP_getTimeStamp();
    DebugP_assert((time - timeClow) < 2000U);
    timeC = ((uint64_t)timeChigh << 32U) | (timeClow);

    timeDiff = timeC - timeA;
    /* PMU counter uses R5 clock. R5 operates with 1GHz.
     * There are 1000_000_000 ticks in a second. */
    limitLow = ((uint64_t)5*(1000U*1000U*1000U));
     /*  5.1 seconds */
    limitHigh = ((uint64_t)6*(1000U*1000U*1000U));

    if(limitLow <= timeDiff)
    {
      if(timeDiff < limitHigh)
      {
          result = osal_OK;
          OSAL_log("\n CycleprofilerP nonos Test Passed!!\n");
      }
    }
    else
    {
        result = osal_FAILURE;
        OSAL_log("\n CycleprofilerP nonos Test Failed!!\n");
    }
    
    return result;
}

