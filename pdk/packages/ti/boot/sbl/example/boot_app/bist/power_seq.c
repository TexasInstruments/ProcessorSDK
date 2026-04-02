/*
 *  Copyright (c) Texas Instruments Incorporated 2024
 *  All rights reserved.
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
 *  \file power_seq.c
 *
 *  \brief Custom core power sequence functions needed for proper powering
 *         beyond just control of SYSFW devices.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <sdl_types.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/sciclient/sciclient.h>

#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)|| defined (SOC_J784S4) || defined (SOC_J742S2)
#include "armv8_power_utils.h"
#include "power_seq.h"
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
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
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* Custom core power restore sequence contains any core-custom power steps
 * needed prior to powering up, outside of device/power domain controls */
int32_t BootApp_customPrepareForPowerUpSequence(uint8_t processorId)
{
    int32_t status = SDL_EFAIL;

    switch(processorId)
    {
#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)
        case (SCICLIENT_PROC_ID_A72SS0_CORE0):
            status = BootApp_armv8PowerPrepareForPowerUpSequence(processorId);
            break;
#elif defined (SOC_J784S4) || defined (SOC_J742S2)
        case (SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_A72SS0_CORE0_0):
        case (SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_A72SS1_CORE0_0):
            status = BootApp_armv8PowerPrepareForPowerUpSequence(processorId);
            break;
#endif
        default:
            UART_printf("BootApp_customPrepareForPowerUpSequence - Invalid ProcId %d\n",
                            processorId);
            break;
    }

    return (status);
}

/* Custom core power down sequence contains any core-custom powerdown steps
 * needed to be properly powered down, outside of device/power domain controls */
int32_t BootApp_customPowerDownSequence(uint8_t processorId)
{
    int32_t status = SDL_EFAIL;

    switch(processorId)
    {
#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)
        case (SCICLIENT_PROC_ID_A72SS0_CORE0):
            status = BootApp_armv8PowerDownSequence(processorId);
            break;
#elif defined (SOC_J784S4) || defined (SOC_J742S2)
        case (SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_A72SS0_CORE0_0):
        case (SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7AHP0_A72SS1_CORE0_0):
            status = BootApp_armv8PowerDownSequence(processorId);
            break;
#endif
        default:
            UART_printf("BootApp_customPowerDownSequence - Invalid ProcId %d\n",
                            processorId);
            break;
    }

    return (status);
}
