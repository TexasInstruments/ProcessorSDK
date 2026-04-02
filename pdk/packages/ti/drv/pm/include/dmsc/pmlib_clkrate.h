/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2018
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
 *  \ingroup PM_LIB PM LIB API
 *  \defgroup CLK_RATE Clock Rate Configuration
 *
 * Initialization of the device involves setting the right frequency
 * for the clocks for the CPUs and peripherals.
 *
 * The Clock Rate Configuration provides 2 APIs to set and get the frequency
 * of any clock of any given module.
 * - PMLIBClkRateGet()
 * - PMLIBClkRateSet()
 * @{
 */

/**
 *  \file  pmlib_clkrate.h
 *
 *  \brief  PMLIB Clock Rate Manager API interface file.
 */

#ifndef PMLIB_CLKRATE_H_
#define PMLIB_CLKRATE_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/drv/sciclient/sciclient.h>

#ifdef __cplusplus
extern "C" {
#endif


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   Get the current clock rate of the given module.
 *          Requirement : DOX_REQ_TAG(PDK-2174)
 *
 *  This function interacts with dmsc and gives the clkrate for the clock  
 *  of a given module.
 *
 * \param modId            Module ID
 *                         Refer Enum #Sciclient_PmDeviceIds for values.
 * \param  clkId           Clock Id present in the module
 *                         Refer macro #Sciclient_PmModuleClockIds for values.
 * \param  clkRate         Clock rate in Hz returned for the clockID
 *
 * \retval errorStatus     Status of API call. Can be any of the following,
 *         - #PM_SUCCESS      Indicates the operation is success
 *         - #PM_TIMEOUT      Indicates that dmsc hasn't responded in the 
 *                            given time
 *         - #PM_FAIL         Can Indicate the following:
 *                          - ModId is not valid.
 *                          - ClockId provided is not valid/enabled
 *                          - Refer Enum #pmErrCode_t for detailed values.
 */
int32_t PMLIBClkRateGet(uint32_t modId, uint32_t clkId, uint64_t *clkRate);

/**
 * \brief   Set the clock rate of the given module.
 *          Requirement : DOX_REQ_TAG(PDK-2173)
 *
 *  This is typically desired when the default frequency of the hardware
 *  block's clock is not appropriate for the usecase desired.
 *  This sets the desired frequency for a clock within an allowable range.
 *  This message will fail on an enabled clock unless
 *  MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE is set for the clock. Additionally,
 *  if other clocks have their frequency modified due to this message,
 *  they also must have the MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE or be disabled.
 *
 * \param  modId           Module ID
 *                         Refer Enum #Sciclient_PmDeviceIds for values.
 * \param  clkId           Clock Id present in the module
 *                         Refer macro #Sciclient_PmModuleClockIds for values.
 * \param  clkRate         New clock rate in Hz to be provided for the clockID
 *
 * \retval errorStatus     Status of API call. Can be any of the following,
 *         - #PM_SUCCESS      Indicates the operation is success
 *         - #PM_FAIL         Can Indicate the following:
 *                          - ModId is not valid.
 *                          - ClockRate provided is not supported
 *                          - Structure provided is not properly initialized
 *                          - Refer Enum #pmErrCode_t for detailed values.
 */
int32_t PMLIBClkRateSet(uint32_t modId, uint32_t clkId, uint64_t clkRate);
#ifdef __cplusplus
}
#endif

#endif

/* @} */

