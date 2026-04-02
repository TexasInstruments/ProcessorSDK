/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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

 /**
  *  \file    pm_checkers_warm_reset.c
  *
  *  \brief   This file contains warm reset safety checkers app code.
  *
  */

/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/

#include <sciclient.h>
#include <safety_checkers_common.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

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

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t SafetyCheckersApp_pmWarmResetRun(void *arg0)
{
    int32_t status = SAFETY_CHECKERS_SOK;

    SAFETY_CHECKERS_log("PM Safety Checkers App for Warm Reset Started\r\n");

    uint32_t mmrResetRegister = (*((volatile uint32_t *)(CSL_WKUP_CTRL_MMR0_CFG0_BASE + SAFETY_CHECKERS_PM_WKUP_CTRL_MMR_CFG0_RESET_SRC_STAT)));

    if((mmrResetRegister & SAFETY_CHECKERS_PM_WARM_RESET_MASK) == SAFETY_CHECKERS_PM_WARM_RESET_MASK)
    {
        /* Software MCU warm reset is occurred */
        SAFETY_CHECKERS_log("MCU reset occured due to warm reset\r\n");
    }
    else
    {
        SAFETY_CHECKERS_log("Doing MCU warm reset reset as it is encountered from POR\r\n");
        status = Sciclient_pmDeviceReset(0xFFFFFFFFU);
    }

    if(SAFETY_CHECKERS_SOK != status)
    {
        SAFETY_CHECKERS_log("\nWarm Reset example failed!!!\r\n");
    }
    else
    {
        SAFETY_CHECKERS_log("\nWarm Reset example Passed\r\n");
        SAFETY_CHECKERS_log("\nAll tests have passed\r\n");
    }

#if defined LDRA_DYN_COVERAGE_EXIT
    SAFETY_CHECKERS_log("\n LDRA ENTRY... \r\n");
    upload_execution_history();
    SAFETY_CHECKERS_log("\n LDRA EXIT... \r\n");
#endif

    return status;
}
