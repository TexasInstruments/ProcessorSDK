/**
 *   Copyright (c) Texas Instruments Incorporated 2018
 *   All rights reserved.
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
 *  \ingroup DRV_VHWA_MODULE
 *  \defgroup CSL_VHWA CSL-FL
 *            CSL Layer for VHWA module
 */

/**
 *  \ingroup CSL_VHWA
 *  \defgroup CSL_VHWA_TOP Top Level CSL FL
 *            This can be used across sub component CSL FLs of VHWA
 *
 *  @{
 */
/**
 *  \file csl_vpactop.h
 *
 *  \brief HAL Header file implementing functionality
 *         Implements VISS_Top and VPAC_Top registers
 *
 */

#ifndef CSL_VPAC_TOP_H_
#define CSL_VPAC_TOP_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <soc/vhwa_soc.h>
#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** None */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/**
 *  \anchor VPAC_ModuleEnable
 *  \name   VPAC module ID's
 *  \brief  MACRO's used for enabling individual accelerator in VPAC
 *          Used in clock gating accelerator when disabled.
 *
 *  @{
 */
#define VPAC_MODULE_VISS0           (0U)
#define VPAC_MODULE_LDC             (2U)
#define VPAC_MODULE_MSC             (4U)
#define VPAC_MODULE_NF              (5U)
 /** @} */

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Function to enable/disable HWA in VPAC
 *
 *  \param vpacTopRegs      Pointer to structure containing VPAC Base Address
 *  \param module           Id of the module to be enabled/disabled
 *  \param enable           flag UTRUE/UFALSE
 *  \return                 Returns 0 on success else returns error value
 */
void CSL_vpacEnableModule(CSL_vpac_cntlRegs *vpacTopRegs, uint32_t module,
    uint32_t enable);

#ifdef __cplusplus
}
#endif

#endif  /* CSL_VPAC_TOP_H_ */
/** @} */
