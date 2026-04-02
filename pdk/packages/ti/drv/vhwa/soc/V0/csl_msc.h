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
 *  \ingroup CSL_VHWA
 *  \defgroup CSL_MSC MSC CSL FL
 *
 *  @{
 */
/**
 *  \file csl_msc.h
 *
 *  \brief  MSC interface file
 */

#ifndef CSL_VPAC_MSC_H_
#define CSL_VPAC_MSC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <soc/vhwa_soc.h>
#include <include/msc_cfg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define MSC_MAX_PHASE           (32U)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */


typedef struct
{
    uint32_t hsFirInc;
    /**< Horizontal scaling factor */
    uint32_t vsFirInc;
    /**< Vertical scaling factor */
} CSL_MscScFactorInfo;

/**
 *  \brief VPAC MSC Hal configuration
 *         This structure is used for configuring entire MSC module,
 */
typedef struct
{
    uint32_t                inWidth;
    /**< Input Frame Width */
    uint32_t                inHeight;
    /**< Input Frame height */
    Msc_Config              mscCfg;
    /**< MSC Configuration for each scalar */

    CSL_MscScFactorInfo     scFact[MSC_MAX_OUTPUT];
} CSL_MscConfig;


/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Function to verify scalar configuration
 *
 *  \param cfg          Pointer to #CSL_MscConfig structure
 *                      containing the register configurations.
 *                      This parameter should be non-NULL.
 *
 *  \return             Returns 0 on success else returns error value
 */
int32_t CSL_mscCheckConfig(const CSL_MscConfig *cfg);

/**
 *  \brief Sets the entire MSC configuration to the MSC registers.
 *
 *  \param mscRegs      Pointer to structure containing MSC Base Address
 *  \param cfg          Pointer to #CSL_MscConfig structure
 *                      containing the register configurations.
 *                      This parameter should be non-NULL.
 *
 *  \return             Returns 0 on success else returns error value
 */
int32_t CSL_mscSetConfig(CSL_mscRegs *mscRegs, CSL_MscConfig *cfg);

/**
 *  \brief Sets the Frame size for input and output both in MSC registers.
 *         This is typically used in the YUV420 processing, where
 *         chroma height is half of luma height. In this case,
 *         Only frame size changes between two iterations of MSC operation.
 *
 *  \param mscRegs      Pointer to structure containing MSC Base Address
 *  \param cfg          Pointer to #CSL_MscConfig structure
 *                      containing the register configurations.
 *                      This parameter should be non-NULL.
 *
 *  \return             Returns 0 on success else returns error value
 */
int32_t CSL_mscSetFrameSize(CSL_mscRegs *mscRegs, CSL_MscConfig *cfg);

/**
 *  \brief Update the selected MSC configuration parameters for Luma and Chroma
 *
 *  \param mscRegs      Pointer to structure containing MSC Base Address
 *  \param cfg          Pointer to #CSL_MscConfig structure
 *                      containing the register configurations.
 *                      This parameter should be non-NULL.
 *  \param vsSchFactor  Vertical scaling factor, If 0 value is passed vertical
 *                      scaling factor will be caculated and stored in this
 *                      variable, otherwise provided value will be used.
 *
 *
 *  \return             Returns 0 on success else returns error value
 */
int32_t CSL_mscUpdateUVPrms(CSL_MscConfig *cfg);

/**
 *  \brief API for setting MSC Coefficients.
 *
 *  \param mscRegs      Pointer to structure containing MSC Base Address
 *  \param coeff        Pointer to the structure #Msc_Coeff
 *                      containing coefficients
 *
 *  \return             Returns 0 on success else returns error value
 */
int32_t CSL_mscSetCoeff(CSL_mscRegs *mscRegs, const Msc_Coeff *coeff);

/**
 *  \brief This function Enables MSC module.
 *
 *  \param mscRegs      Pointer to structure containing MSC Base Address
 *
 *  \return             None
 */
void CSL_mscEnable(CSL_mscRegs *mscRegs);

/**
 *  \brief This function Disables MSC module.
 *
 *  \param mscRegs      Pointer to structure containing MSC Base Address
 *
 *  \return             None
 */
void CSL_mscDisable(CSL_mscRegs *mscRegs);

/**
 *  \brief This function should be used to initialize variable of type
 *          #CSL_MscConfig.
 *
 *  \param cfg      Pointer to #CSL_MscConfig
 *
 *  \return         None
 */
static inline void CSL_mscInit(CSL_MscConfig *cfg);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
static inline void CSL_mscConfigInit(CSL_MscConfig *cfg)
{
    uint32_t cnt;

    cfg->inWidth = 0U;
    cfg->inHeight = 0U;
    cfg->mscCfg.tapSel = MSC_TAP_SEL_5TAPS;

    for (cnt = 0; cnt < MSC_MAX_OUTPUT; cnt++)
    {
        cfg->mscCfg.scCfg[cnt].enable = UFALSE;
        cfg->mscCfg.scCfg[cnt].outWidth = 0U;
        cfg->mscCfg.scCfg[cnt].outHeight = 0U;
        cfg->mscCfg.scCfg[cnt].horzAccInit = 0U;
        cfg->mscCfg.scCfg[cnt].vertAccInit = 0U;
        cfg->mscCfg.scCfg[cnt].filtMode = MSC_FILTER_MODE_SINGLE_PHASE;
        cfg->mscCfg.scCfg[cnt].phaseMode = MSC_PHASE_MODE_64PHASE;
        cfg->mscCfg.scCfg[cnt].hsSpCoeffSel = MSC_SINGLE_PHASE_SP_COEFF_0;
        cfg->mscCfg.scCfg[cnt].vsSpCoeffSel = MSC_SINGLE_PHASE_SP_COEFF_0;
        cfg->mscCfg.scCfg[cnt].hsMpCoeffSel = MSC_MULTI_64PHASE_COEFF_SET_0;
        cfg->mscCfg.scCfg[cnt].vsMpCoeffSel = MSC_MULTI_64PHASE_COEFF_SET_0;
        cfg->mscCfg.scCfg[cnt].coeffShift = MSC_COEFF_SHIFT_8;
        cfg->mscCfg.scCfg[cnt].isSignedData = UFALSE;
        cfg->mscCfg.scCfg[cnt].isEnableFiltSatMode = UFALSE;
        cfg->mscCfg.scCfg[cnt].inRoi.cropStartX = 0U;
        cfg->mscCfg.scCfg[cnt].inRoi.cropStartY = 0U;
        cfg->mscCfg.scCfg[cnt].inRoi.cropWidth = 0U;
        cfg->mscCfg.scCfg[cnt].inRoi.cropHeight = 0U;
        cfg->mscCfg.scCfg[cnt].isInterleaveFormat = 0U;
    }
}

#ifdef __cplusplus
}
#endif

#endif  /* CSL_VPAC_MSC_H_ */
/** @} */
