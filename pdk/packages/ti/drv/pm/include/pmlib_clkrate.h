/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2014
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
 * for the clocks for the CPUs and peripherals and right voltage for
 * the voltage rails.
 *
 * The initialization of the system is carried in 3 stages:
 *  - Setting the AVS class-0 voltage and ABB voltage at boot from the SBL
 *    (\ref PM_HAL_VM)
 *  - Configuring the DPLL, multiplexer and dividers at boot time from the SBL
 *    (\ref PM_HAL_CM)
 *  - Configuring peripheral, Video Pll and CPU frequency at run time with
 *    necessaryvoltage changes. This is taken care by the
 *    PMLIB Clock Rate Configuration APIs.
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

#ifndef PMLIB_CLKRATE_COMMON_H_
#define PMLIB_CLKRATE_COMMON_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#if defined (SOC_TDA2XX) || defined (SOC_TDA2PX) || defined (SOC_AM572x) || defined (SOC_AM574x) || defined (SOC_DRA75x) || defined (SOC_TDA2EX) || defined (SOC_DRA72x) || defined (SOC_AM571x) || defined (SOC_TDA3XX) || defined (SOC_DRA78x)
#include <ti/drv/pm/include/prcm/pmlib_clkrate.h>
#endif

#if defined (SOC_AM65XX) || (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#include <ti/drv/pm/include/dmsc/pmlib_clkrate.h>
#endif

#endif /* PMLIB_CLKRATE_H_ */

/* @} */


