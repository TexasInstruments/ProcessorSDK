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

 /**
 *  \file   mcspiAppUtils.c
 *
 *  \brief This file contains the utility APIs for measuting timestamps.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdio.h>
#include <ti/csl/csl.h>
#include <ti/csl/soc.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/sciclient/sciclient.h>
#include "mcspiAppUtils.h"
#include "mcspiAppUtils_soc.h"

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static volatile uint64_t gGetTicksDelay = 0; /* No.of ticks taken to do a GTC Reg Read operation */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t McSPIAppUtilsSetGTCClk(uint32_t moduleId,
                                   uint32_t clkId,
                                   uint64_t clkRateHz)
{
    int32_t status = CSL_PASS;
    uint64_t currClkFreqHz;

    status = Sciclient_pmGetModuleClkFreq(moduleId,
                                          clkId,
                                          &currClkFreqHz,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if ((status == CSL_PASS) &&
        (currClkFreqHz != clkRateHz))
    {
        status = McSPIAppUtilsClkRateSet(moduleId, clkId, clkRateHz);
    }

    /* Enable GTC */
    CSL_REG32_WR(CSL_GTC0_GTC_CFG1_BASE + 0x0U, 0x1);

    /* Measure and store the time spent to do a getTime operation */
    gGetTicksDelay = McSPIGetGTCTimerTicks();
    gGetTicksDelay = McSPIGetGTCTimerTicks() - gGetTicksDelay;
    McSPIAppUtilsPrint("Time taken to read GTC Timer ticks = %d ns (%d ticks) \n",
                   (uint32_t)((gGetTicksDelay*1000000000U)/(uint64_t)MCSPI_GTC_CLK_FREQ),
                   (uint32_t)gGetTicksDelay);

    return (status);
}

int32_t McSPIAppUtilsClkRateSet(uint32_t modId,
                             uint32_t clkId,
                             uint64_t clkRate)
{
    uint32_t i                          = 0U;
    int32_t status                      = CSL_PASS;
    int32_t finalStatus                 = CSL_PASS;
    uint64_t respClkRate                = 0;
    uint32_t numParents                 = 0U;
    uint32_t moduleClockParentChanged   = 0U;
    uint32_t clockStatus                = 0U;
    uint32_t origParent                 = 0U;
    uint32_t foundParent                = 0U;

    /* Check if the clock is enabled or not */
    status = Sciclient_pmModuleGetClkStatus(modId,
                                            clkId,
                                            &clockStatus,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    if (status == CSL_PASS)
    {
        /* Get the number of parents for the clock */
        status = Sciclient_pmGetModuleClkNumParent(modId,
                                                   clkId,
                                                   &numParents,
                                                   SCICLIENT_SERVICE_WAIT_FOREVER);
        if ((status == CSL_PASS) && (numParents > 1U))
        {
            status = Sciclient_pmGetModuleClkParent(modId,
                                                    clkId,
                                                    &origParent,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }
    }
    if (status == CSL_PASS)
    {
        /* Disabling the clock */
        status = Sciclient_pmModuleClkRequest(modId,
                                              clkId,
                                              TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ,
                                              0U,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    if (status == CSL_PASS)
    {
        foundParent = 0U;
        /* Try to loop and change parents of the clock */
        for(i=0U;i<numParents;i++)
        {
            if (numParents > 1U)
            {
                /* Setting the new parent */
                status = Sciclient_pmSetModuleClkParent(modId,
                                                        clkId,
                                                        clkId+i+1U,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
                /* Check if the clock can be set to desirable freq. */
                if (status == CSL_PASS)
                {
                    moduleClockParentChanged = 1U;
                }
            }
            if (status == CSL_PASS)
            {
                status = Sciclient_pmQueryModuleClkFreq(modId,
                                                        clkId,
                                                        clkRate,
                                                        &respClkRate,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            }
            if ((status == CSL_PASS) && (respClkRate == clkRate))
            {
                foundParent = 1U;
                break;
            }
        }
    }
    if ((status == CSL_PASS) && (numParents == 0U))
    {
        status = Sciclient_pmQueryModuleClkFreq(modId,
                                                clkId,
                                                clkRate,
                                                &respClkRate,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
        if ((status == CSL_PASS) && (respClkRate == clkRate))
        {
            foundParent = 1U;
        }
    }
    if (foundParent == 1U)
    {
        /* Set the clock at the desirable frequency*/
        status = Sciclient_pmSetModuleClkFreq(modId,
                                              clkId,
                                              clkRate,
                                              TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    else
    {
        status = CSL_EFAIL;
    }
    if ((status == CSL_PASS) &&
        (clockStatus == (uint32_t) TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ))
    {
        /* Restore the clock again to original state */
        status = Sciclient_pmModuleClkRequest(modId,
                                              clkId,
                                              clockStatus,
                                              0U,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    finalStatus = status;
    if ((status != CSL_PASS) && (moduleClockParentChanged == 1U))
    {
        /* Setting the original parent if failure */
        (void) Sciclient_pmSetModuleClkParent(modId,
                                              clkId,
                                              origParent,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    return (finalStatus);
}

uint64_t McSPIAppUtilsGetElapsedTime(uint64_t startTicks, uint64_t stopTicks)
{
    volatile uint64_t    totalTicks; /* measured total no. of GTC Timer ticks*/
    volatile uint64_t    elapsedTime; /* elapsed time in nsec */

    totalTicks = stopTicks - startTicks  - gGetTicksDelay;
    elapsedTime = (totalTicks*1000000000U)/(uint64_t)MCSPI_GTC_CLK_FREQ;
    return elapsedTime;

}

/********************************* End Of File ******************************/
