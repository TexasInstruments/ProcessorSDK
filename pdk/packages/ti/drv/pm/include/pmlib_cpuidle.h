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
 *   \ingroup PM_LIB PM LIB API
 *   \defgroup CPU_IDLE CPU Idle Configuration
 *      Dynamic power management of the CPUs involves setting the
 *      CPU power state to a lower power state when the CPU has
 *      nothing to do and then wake up from the low power state
 *      when the CPU has to resume its operations.
 *
 *      The CPU context is maintained every time the CPU goes to
 *      low power mode. Interrupts which are configured during the
 *      normal CPU operation can be configured as wakeup events which
 *      bring the CPU out of its low power state.
 *
 *      Some recommended power states for the different CPUs is as below:
 *      \n    - MPU : Subsystem Retention
 *      \n    - IPU : Subsystem Auto Clock Gate
 *      \n    - DSP : Subsystem Auto Clock Gate
 *      \n    - EVE : Subsystem Auto Clock Gate
 *
 *
 * @{
 */

/**
 *  \file  pmlib_cpuidle.h
 *
 *  \brief  This file declares the interface for CPU Idle functionality. This is
 *          is a low latency power saving mode which allows fast wakeup
 *          depending on the power mode set.
 */

#ifndef PMLIB_CPUIDLE_COMMON_H_
#define PMLIB_CPUIDLE_COMMON_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#if defined (SOC_TDA2XX) || defined (SOC_TDA2PX) || defined (SOC_AM572x) || defined (SOC_AM574x) || defined (SOC_DRA75x) || defined (SOC_TDA2EX) || defined (SOC_DRA72x) || defined (SOC_AM571x) || defined (SOC_TDA3XX) || defined (SOC_DRA78x)
#include <ti/drv/pm/include/prcm/pmlib_cpuidle.h>
#endif

#if defined (SOC_AM65XX) || defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#include <ti/drv/pm/include/dmsc/pmlib_cpuidle.h>
#endif

#endif

/* @} */


