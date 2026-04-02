/*
 * Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "sbl_lockstep_multicore.h"
#include "sbl_print.h"
#include <ti/osal/osal.h>

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int SBLApp_isLockStepModeEnabled( void );

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int SBLApp_main(void)
{
#if defined(BUILD_MCU1_0)
    volatile int *bootFlagAddr = (volatile int *)POKE_MEM_ADDR_MCU2_0;
    volatile int numCoresBooted = 0;
#else
    volatile int bootDelay = BOOT_DELAY;
    volatile int *pokeMemAddr = (volatile int *)POKE_MEM_ADDR;
    int lockStepEnableFlag = 0;
#endif

#if !defined(BUILD_MCU1_0)
    if (*pokeMemAddr != UPDATE_VALUE)
    {
        while (bootDelay--)
        {
            /* Do Nothing */
        }
        lockStepEnableFlag = SBLApp_isLockStepModeEnabled();
        if (lockStepEnableFlag == 1)
        {
            sbl_puts(CORE_NAME);
            sbl_puts(" running in lockstep mode \n\r");
            *pokeMemAddr = UPDATE_VALUE;
            /* Write back to OCMC address so that mcu1_0 can see the updated value in OCMC */
            CacheP_wb((const void *)pokeMemAddr, sizeof (int));
        }
    }
#endif

#if defined(BUILD_MCU1_0)
    while (numCoresBooted == 0)
    {
        /* Invalidate the cache so that mcu1_0 can read the values written by other cores */
        for (bootFlagAddr = (volatile int *)POKE_MEM_ADDR_MCU2_0;
            bootFlagAddr <= (volatile int *)POKE_MEM_ADDR_MCU4_0;
            bootFlagAddr += POKE_MEM_ADDR_GAP)
        {
            CacheP_wbInv((const int *)bootFlagAddr, sizeof(int));
        }
        for (bootFlagAddr = (volatile int *)POKE_MEM_ADDR_MCU2_0; bootFlagAddr <= (volatile int *)POKE_MEM_ADDR_MCU4_0; bootFlagAddr += POKE_MEM_ADDR_GAP)
        {
            if (*bootFlagAddr == UPDATE_VALUE)
            {
                numCoresBooted++;
            }
        }

        if(numCoresBooted == SBL_LOCKSTEP_TEST_NUM_BOOT_CORES)
        {
            sbl_puts("mcu1_0 reports : \n\r");
            sbl_puts("All tests have passed\n");
            break;
        }
        else
        {
            numCoresBooted = 0;
        }
    }
    /* Clean up pokemem flags for the next run */
    for (bootFlagAddr = (volatile int *)POKE_MEM_ADDR_MCU2_0;
         bootFlagAddr <= (volatile int *)POKE_MEM_ADDR_MCU4_0;
         bootFlagAddr += POKE_MEM_ADDR_GAP)
    {
        *bootFlagAddr = DEFAULT_VALUE;
    }
#endif

    return DEFAULT_VALUE;
}

