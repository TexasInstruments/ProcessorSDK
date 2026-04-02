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
 *  \defgroup BOARD_CONFIG Board Configuration
 *
 *  Board Configuration is used along with the PMLIB Clock rate APIs.
 *  (\ref CLK_RATE)
 *  Board configuration takes into account the ganging of voltage rails
 *  which can happen outside the device boundaries. Board configuration
 *  is also used to find all the dependent CPUs in the voltage rails
 *  which are ganged and if the new voltage is found to satisfy the OPP
 *  requirement of any ganged CPU the voltage is changed along with the
 *  DPLL configuration.
 *
 *  It also captures the clock frequencies of clock sources input to
 *  device (like an external oscillator frequency).
 *
 * @{
 */

/**
 *  \file      pmlib_boardconfig.h
 *
 *  \brief     This file contains structure declarations for aggregating
 *             board specific power management related information
 *             such as - root clocks, ganging of voltage rails.
 */

#ifndef PMLIB_BOARDCONFIG_COMMON_H_
#define PMLIB_BOARDCONFIG_COMMON_H_

/* ========================================================================== */
/*                                Include Files                               */
/* ========================================================================== */

#if defined (SOC_TDA2XX) || defined (SOC_TDA2PX) || defined (SOC_AM572x) || defined (SOC_AM574x) || defined (SOC_DRA75x) || defined (SOC_TDA2EX) || defined (SOC_DRA72x) || defined (SOC_AM571x) || defined (SOC_TDA3XX) || defined (SOC_DRA78x)
#include <ti/drv/pm/include/prcm/pmlib_boardconfig.h>
#endif

#endif /* PMLIB_BOARDCONFIG_COMMON_H_ */

/* @} */


