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
 *  \file csl_glbce.h
 *
 *  \brief CSL Header file for GLBCE
 *      This file defines the CSL APIs for GLBCE module
 *
 */

#ifndef CSL_GLBCE_H_
#define CSL_GLBCE_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <soc/vhwa_soc.h>
#include "include/glbce_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Notes/Questions:
 * 1, Requires start up sequence after powering it on. Should be taken care
 *    in the upper layer, HAL does not take care
 * 2, There are 8 sets of 16bit x 256 entry table, mapped to MMR
 * 3, Minimum input size is 480x240
 * 4, Requires initialization state of 554 clock cycles, while in this stage,
 *    filter flag is on and there should not be any input frame
 * 5, LUTs must not be changed during active frame, must be changed in
 *    blanking interval. In most cases, LUT_F1 is changed from frame to frame,
 *    other LUTs are not expected to be updated dynamically
 */


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined (VHWA_VPAC_IP_REV_VPAC3L)
/** \brief Horizontal latency required for GLBCE
 */
#define CSL_GLBCE_HORZ_LATENCY                  (61u)

/** \brief Horizontal blanking required for GLBCE
 */
#define CSL_GLBCE_HORZ_BLANKING                  (5u)

#else
/** \brief Horizontal latency required for GLBCE
 */
#define CSL_GLBCE_HORZ_LATENCY                  (61u)
#endif
/** \brief Vertical Blanking required for Flex CC
 */
#define CSL_GLBCE_VERT_BLANKING                 (1u)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  struct CSL_GlbceConfig
 *  \brief VPAC GLBCE Hal configuration,
 *         This structure is used for configuring entire GLBCE module,
 *         It is used to configure input frame size, asymmentry configuration,
 *         forward/reverse perpetual configuration and wdr configuration.
 *
 *         Can be used by the upper layer to store the configuration
 *         in its local data structure
 */
typedef struct
{
    uint32_t                    width;
    /**< Input frame width */
    uint32_t                    height;
    /**< Input frame height */
    uint32_t                    isOneShotMode;
    /**< Flag to configure glbce in one shot mode
     *   UTRUE: One Shot mode enabled GLBCE turns off it self at the
     *         beginning of each frame
     *   UFALSE: One shot mode is disabled ie GLBCE module keep running
     *          after each frame until the enable bit is turned off */
} CSL_GlbceConfig;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Sets the entire GLBCE module configuration to the LDC registers.
 *
 *  \param glbceBaseAddr    GLBCE Base Address,
 *                          This parameter should not be 0.
 *  \param config           Pointer to CSL_GlbceConfig structure
 *                          containing the register configurations.
 *                          This parameter should not be NULL.
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_glbceSetConfig(CSL_glbceRegs *gRegs,
    const CSL_GlbceConfig  *cfg);

/**
 *  \brief Sets the Tone Mapping Settings.
 *
 *  \param glbceBaseAddr    GLBCE Base Address,
 *                          This parameter should not be 0.
 *  \param config           Pointer to Glbce_Config structure
 *                          containing the Glbce configurations.
 *                          This parameter should not be NULL.
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_glbceSetToneMapConfig(CSL_glbceRegs *gRegs,
    const Glbce_Config *cfg);

/**
 *  \brief Sets the WDR configuration.
 *         Used to enable/disable this module and also to set the WDR Lut
 *         When Disabled, it just resets the enable bit, but does not
 *         update LUT
 *
 *  \param glbceBaseAddr    GLBCE Base Address,
 *                          This parameter should not be 0.
 *  \param config           Pointer to Glbce_WdrConfig structure
 *                          containing the WDR configurations.
 *                          This parameter should not be NULL.
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_glbceSetWdrConfig(CSL_glbceRegs *gRegs,
    const Glbce_WdrConfig *cfg);

/**
 *  \brief Sets the Forward/Reverse Percept Configuration.
 *         Used to enable/disable this module and also to set the perceptual Lut
 *         When Disabled, it just resets the enable bit, but does not
 *         update LUT
 *
 *  \param glbceBaseAddr    GLBCE Base Address,
 *                          This parameter should not be 0.
 *  \param config           Pointer to Glbce_PerceptConfig structure
 *                          containing the perceptual configurations.
 *                          This parameter should not be NULL.
 *  \param perceptDir       Percept direction, one of
 *                          VPS_VPAC_GLBCE_MODULE_FWD_PERCEPT or
 *                          VPS_VPAC_GLBCE_MODULE_REV_PERCEPT
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_glbceSetPerceptConfig(CSL_glbceRegs *gRegs,
    const Glbce_PerceptConfig *cfg, uint32_t perceptDir);

/**
 *  \brief Starts Glbce module
 *
 *  \param glbceBaseAddr    GLBCE Base Address,
 *                          This parameter should not be 0.
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_glbceStart(CSL_glbceRegs *gRegs);

/**
 *  \brief Stops Glbce module
 *
 *  \param glbceBaseAddr    GLBCE Base Address,
 *                          This parameter should not be 0.
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_glbceStop(CSL_glbceRegs *gRegs);

/**
 *  \brief Get the GLBCE Statistics info
 *
 *  \param glbceBaseAddr    GLBCE Base Address,
 *                          This parameter should not be 0.
 *  \param stats            GLBCE statistics info into which info
 *                          will be returned.
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_glbceGetStatsInfo(const CSL_glbce_statmemRegs *glbceStatsMem,
    Glbce_StatsInfo *stats);

#ifdef __cplusplus
}
#endif

#endif  /* CSL_GLBCE_H_ */
