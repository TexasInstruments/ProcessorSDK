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
 *  \file csl_vissTop.h
 *
 *  \brief CSL Header file implementing functionality
 *         Implements VISS_Top and VPAC_Top registers
 *
 */

#ifndef CSL_VISS_TOP_H_
#define CSL_VISS_TOP_H_

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

/* None */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  struct
 *  \brief VPAC MSC Hal configuration,
 *         This structure is used for configuring entire MSC module,
 */
typedef struct
{
    uint32_t        horzDelay;
    /**< Horizontal delay, used for generating HS
         Cycle delay between HS Input to Output */
    uint32_t        vertDelay;
    /**< Vertical delay, used for generating VS
         Cycle delay between VS Input to Output */
} CslGlbce_DelayConfig;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Function returns maximum line width supported in VISS
 *
 *  \param vissBaseAddr     VISS Base Address
 *  \return                 Returns maximum width
 */
uint32_t CSL_vissTopGetMaxWidth(const CSL_viss_topRegs *vissTopRegs);

/**
 *  \brief Function sets GLBCE delay configuration
 *
 *  \param vissBaseAddr     VISS Base Address
 *  \param dlyCfg           GLBCE Delay configuration
 */
void CSL_vissTopGlbceSetDelayConfig(CSL_viss_topRegs *vissTopRegs,
    const CslGlbce_DelayConfig *dlyCfg);

/**
 *  \brief Function to enable/disable GLBCE module
 *
 *  \param vissBaseAddr     VISS Base Address
 *  \param enable           UTRUE enables GLBCE Module
 *                          UFALSE bypasses GLBCE module, configuration
 *                          register of GLBCE will not be accessible
 *                          in this case.
 */
void CSL_vissTopGlbceEnable(CSL_viss_topRegs *vissTopRegs, uint32_t enable);

/**
 *  \brief Function to enable/disable NSF4 module
 *
 *  \param vissBaseAddr     VISS Base Address
 *  \param enable           UTRUE enables NSF4 Module
 *                          UFALSE bypasses NSF4 module, configuration
 *                          register of NSF4 will not be accessible
 *                          in this case.
 */
void CSL_vissTopNsf4Enable(CSL_viss_topRegs *vissTopRegs, uint32_t enable);

/**
 *  \brief Function to enable/disable GLBCE for Free running
 *         Used mainly for providing initial clocks for GLBCE.
 *
 *  \param vissBaseAddr     VISS Base Address
 *  \param enable           UTRUE: Enables GLBCE free running clock
 *                          UFALSE: Disable Free Running clock
 *
 */
void CSL_vissTopGlbceFreeRunning(CSL_viss_topRegs *vissTopRegs,
    uint32_t enable);

/**
 *  \brief Function to return status of the GLBCE Filter Done event
 *
 *  \param vissBaseAddr     VISS Base Address
 *
 *  \return UTRUE: If GLBCE filter Done event is set
 *          UFALSE: If GLBCE filter Done event is reset
 *
 */
uint32_t CSL_vissTopIsGlbceFiltDone(const CSL_viss_topRegs *vissTopRegs);

#ifdef __cplusplus
}
#endif

#endif  /* CSL_VISS_TOP_H_ */
