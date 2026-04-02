/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2021-2022
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
 *  \file csl_cac.h
 *
 *  \brief CSL Header file for CAC
 *      This file defines the CAC APIs for VPAC CAC module
 *
 */

#ifndef CSL_VPAC_CAC_H_
#define CSL_VPAC_CAC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <soc/vhwa_soc.h>
#include <include/cac_cfg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Horizontal latency required for CAC
 */
#define CSL_CAC_HORZ_LATENCY                  (24u)

/** \brief Horizontal Blanking required for Flex CC
 */
#define CSL_CAC_HORZ_BLANKING                 (20u)

/** \brief Horizontal Blanking required for Flex CC
 */
#define CSL_CAC_VERT_BLANKING                 (6u)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    uint32_t    width;
    uint32_t    height;
} CslCac_FrameConfig;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Sets the entire CAC configuration to the CAC registers.
 *
 *  \param cacRegs          CAC Base Address
 *  \param cacLutRegs       CAC LUT Base Address
 *  \param cfg              Pointer to Cac_Config structure
 *                          containing the register configurations.
 *                          This parameter should be non-NULL.
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_cacSetConfig(CSL_cacRegs *cacRegs, CSL_cac_lutRegs *cacLutRegs,
                            const Cac_Config *cfg);

/**
 *  \brief Sets frame size in the CAC
 *
 *  \param cacRegs          CAC Base Address
 *  \param config           Pointer to CslCac_FrameConfig structure
 *                          containing frame size.
 *                          This parameter should be non-NULL.
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_cacSetFrameConfig(CSL_cacRegs *cacRegs,
    const CslCac_FrameConfig *cfg);

/* ========================================================================== */
/*                           Function Definition                              */
/* ========================================================================== */

#ifdef __cplusplus
}
#endif

#endif  /* CSL_VPAC_CAC_H_ */
