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
 */

/**
 *  \file csl_ee.h
 *
 *  \brief CSL Header file for EE
 *      This file defines the EE APIs for VPAC EE module
 *
 */

#ifndef CSL_VPAC_EE_H_
#define CSL_VPAC_EE_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <soc/vhwa_soc.h>
#include "include/fcp_cfg.h"


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

typedef struct
{
    uint32_t    width;
    uint32_t    height;
} CslEe_FrameConfig;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Sets the entire Edge enhancer configuration to the YEE registers.
 *
 *  \param eeRegs           YEE Base Address
 *  \param config           Pointer to Fcp_EeConfig structure
 *                          containing the register configurations.
 *                          This parameter should be non-NULL.
 *  \param arg              Not used, should be NULL
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_eeSetConfig(CSL_flexeeRegs *eeRegs, const Fcp_EeConfig *cfg);

/**
 *  \brief Sets frame size in the Edge Enhancer
 *
 *  \param eeRegs           YEE Base Address
 *  \param config           Pointer to CslEe_FrameConfig structure
 *                          containing frame size.
 *                          This parameter should be non-NULL.
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_eeSetFrameConfig(CSL_flexeeRegs *eeRegs,
    const CslEe_FrameConfig *cfg);

/**
 *  \brief Starts the edge enhancer with the given mux value
 *
 *  \param eeRegs           YEE Base Address
 *  \param eeMux            EE Mux value, to be set in CFG_1 register
 */
void CSL_eeStart(CSL_flexeeRegs *eeRegs, uint32_t eeMux);

/**
 *  \brief Stops the edge enhancer with the given mux value
 *
 *  \param eeRegs           YEE Base Address
 *  \param eeMux            EE Mux value, to be set in CFG_1 register
 */
void CSL_eeStop(CSL_flexeeRegs *eeRegs, uint32_t eeMux);

/**
 *  \brief Returns the current Mux value, CFG_1 register value
 *
 *  \param eeRegs           YEE Base Address
 */
uint32_t CSL_eeGetMuxValue(const CSL_flexeeRegs *eeRegs);

/* ========================================================================== */
/*                           Function Definition                              */
/* ========================================================================== */

#ifdef __cplusplus
}
#endif

#endif  /* CSL_VPAC_EE_H_ */
