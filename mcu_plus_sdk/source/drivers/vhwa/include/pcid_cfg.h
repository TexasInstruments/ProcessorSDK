/*
 *   Copyright (c) Texas Instruments Incorporated 2019
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
 *  \ingroup  DRV_VISS_MODULE
 *  \defgroup DRV_VISS_MODULE_CFG PCID Configuration
 *            The file provides the configuration options p
 *
 *  @{
 */

/**
 *  \file pcid_cfg.h
 *
 *  \brief  Defines the structures / control operations that could be used to
 *              configure / control PCID module
 */

#ifndef CFG_PCID_H_
#define CFG_PCID_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_vpac_abstraction_layer.h>
#include <include/vhwa_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \brief PCID IR Remap LUT size
 */
#define PCID_IR_REMAP_LUT_SIZE  (320U)

/**
 *  \anchor Pcid_BayerOutSel
 *  \name   PCID Bayer output configuration
 *  \brief  Enum for sending IR subtracted bayer or IR before remaping, on the
 *              bayer channel
 *
 *  @{
 */
/**< \brief IR subtracted bayer output */
#define PCID_BAYEROUTSEL_IR_SUB_BAYER      (0U)
/**< \brief IR outpt before remap */
#define PCID_BAYEROUTSEL_IR_PRE_REMAP      (1U)
/** @} */

/**
 *  \anchor Pcid_colorAtIRPix
 *  \name   PCID Select color at IR location
 *  \brief  Enum for selecting either blue or red at IR position
 *
 *  @{
 */
/**< \brief B at IR and R at B */
#define PCID_COLOR_B_AT_IR_PIX_R_AT_B_PIX   (0U)
/**< \brief R at IR and B at R */
#define PCID_COLOR_R_AT_IR_PIX_B_AT_R_PIX   (1U)
/** @} */

/**
 *  \anchor Pcid_rbIRIntpMethod
 *  \name   R & B interpolation method
 *  \brief  Enum to define different interpolation options for R & B
 *
 *  @{
 */
/**< \brief Interpolated missing color based on hue */
#define PCID_COLOR_INTERPOLATE_HUE          (0x0U)
/**< \brief Interpolated missing color based on color difference */
#define PCID_COLOR_INTERPOLATE_COLOR_DIFF   (0x1U)
/** @} */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief PCID IR Remap LUT
 */
typedef struct
{
    uint32_t    lut[PCID_IR_REMAP_LUT_SIZE*2];
    /**< IR Remap LUT */
}Pcid_IRremapLut;


/**
 *  \brief PCID output Channel configurations
 */
typedef struct
{
    uint32_t                irOutEn;
    /**< Enable IR output channel, a non-zero value enables output */
    uint32_t                bayerOutEn;
    /**< Enable bayer output channel, a non-zero value enables output */
    uint32_t                bayerOutSel;
    /**< Select either bayer or IR to be sent out on bayer channel. Refer
        #Pcid_BayerOutSel for valid values. Valid only when bayerOutEn is
        non-zero */
    uint32_t                rbIntpAtIR;
    /**< Select either B or R at IR pixel, refer #Pcid_colorAtIRPix */
    uint32_t                rbIRIntpMethod;
    /**< R & B Interpolation method, refer #Pcid_rbIRIntpMethod */
    uint32_t                irSubtractEn;
    /**< Enable IR subtraction in bayer output, a non-zero value enables
        subtraction */

} Pcid_OpChCfg;


/**
 *  \brief R B & Ir interpolation threshold configurations
 */
typedef struct
{
    uint32_t    t1;
    /**< T1 threshold, values in bits 0-15 is used, others discarded */
    uint32_t    t2;
    /**< T2 threshold, values in bits 0-15 is used, others discarded */
    uint32_t    t3;
    /**< T3 threshold, values in bits 0-15 is used, others discarded */

} Pcid_RBIrIntThCfg;

/**
 *  \brief R B & Ir interpolation color difference interpolation configuration
 */
typedef struct
{
    uint32_t    gHFXferFactorIr;
    /**< Portion of green high frequency component mixed with interpolated Ir
            values in color difference IR interpolation method in U9Q8 format */
    uint32_t    gHFXferFactor;
    /**< Portion of green high frequency component mixed with interpolated R/B
            values in color difference R/B interpolation Method in U9Q8 format*/

} Pcid_RBIrIntClrDiffCfg;


/**
 *  \brief Ir subtraction configuration
 */
typedef struct
{
    uint32_t        irSubtractFiltEn;
    /**< When irSubtractEn is enabled, smoothing filter can be enabled,
        a non-zero value enables smoothening filter */
    uint32_t        irRemapLutEn;
    /**< Enable IR remap LUT, a non-zero value enables LUT based remap*/
    Pcid_IRremapLut *pIRRemapLut;
    /**< IR remap LUT - bit depth conversion */

    uint32_t    cutOffTh;
    /**< Threshold for Ir subtraction in U16 format. When R/G/B pixel values at
            a locations in the input are above this threshold no IR subtraction
            is performed (i.e. Ir subtraction factor == 0) */

    uint32_t    transitionRange;
    /**< Range of gray levels just below cutOffTh where Ir subtraction factor
            linearly changes from 1 at cutOffTh - transitionRange and 0 above
            cutOffTh. Is in U8 format */

    uint32_t    transitionRangeInv;
    /**< Reciprocal of TransitionRange in U9Q8 format (i.e. 8-bit of decimal
            with 1-bit of integer). SW driver needs to generate this from
            TransitionRange to avoid HW divider */

    uint32_t    irSubDistScaleLut[5U];
    /**< City Block Distance based IR Subtraction Scale factor LUT, Scales are
            used in Pre smoothing filter factor calculation
            irSubDistScaleLut[0U] = D0Scale, irSubDistScaleLut[1U] = D1Scale
            irSubDistScaleLut[4U] = D4Scale
            MAX value is 256
            Maps to IRSUBDISTSCALELUT1 & IRSUBDISTSCALELUT0 */

    uint32_t    irSubFactScale[4U];
    /**< IR Subtraction Factor after smoothing Factor Filter, Ir Subtraction
        factor scale for [x][x] co-ordinates in Bayer 2x2 pattern.
        Represented in U9Q8 format and Max value supported is 511
        irSubFactScale[0] = [0][0] co-ordinates in Bayer 2x2 pattern
        irSubFactScale[1] = [0][1] co-ordinates in Bayer 2x2 pattern
        irSubFactScale[2] = [1][0] co-ordinates in Bayer 2x2 pattern
        irSubFactScale[3] = [1][1] co-ordinates in Bayer 2x2 pattern
        Maps to IRSUBSCALE0 & IRSUBSCALE1 */

} Pcid_IrSubCfg;


/**
 *  \brief PCID configuration
 */
typedef struct
{
    uint32_t        cfaFormat;
    /**< CFA Format specifier, specify position of RED in first 4x2 in 4x4
            matrix of the RGBIR pixel pattern
            0 = RGGIr, 1 = GRIrG,  2 = BGGIr, 3 = GBIrG,
            4 = GIrRG, 5 = IrGGR , 6 = GIrBG, 7 = IrGGB */
    Pcid_OpChCfg    opChCfg;
    /**< PCID op configurations */

    Pcid_RBIrIntThCfg       thRBIrCfg;
    /**< R B & Ir interpolation threshold config */
    Pcid_RBIrIntClrDiffCfg  clrDiffRBIrCfg;
    /**< Color diff interpolation configuration params,
        When rbIRIntpMethod is PCID_COLOR_INTERPOLATE_COLOR_DIFF */
    Pcid_IrSubCfg           irSubCfg;
    /**< IR Subtraction configuration */

} Pcid_Cfg;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief This function should be used to initialize variable of type
 *          #Pcid_Cfg.
 *
 *  \param PCID Config  A pointer of type #Pcid_Cfg
 *
 *  \return             None
 */
static inline void Pcid_configInit(Pcid_Cfg *pCfg);



/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static inline void Pcid_configInit(Pcid_Cfg *pCfg)
{
    if (NULL != pCfg)
    {
        pCfg->cfaFormat = 1U;

        pCfg->opChCfg.irOutEn           = TRUE;
        pCfg->opChCfg.bayerOutEn        = TRUE;
        pCfg->opChCfg.bayerOutSel       = PCID_BAYEROUTSEL_IR_SUB_BAYER;
        pCfg->opChCfg.rbIntpAtIR        = PCID_COLOR_B_AT_IR_PIX_R_AT_B_PIX;
        pCfg->opChCfg.rbIRIntpMethod    = PCID_COLOR_INTERPOLATE_HUE;
        pCfg->opChCfg.irSubtractEn      = TRUE;

        pCfg->thRBIrCfg.t1  = 8192U;
        pCfg->thRBIrCfg.t2  = 16320U;
        pCfg->thRBIrCfg.t3  = 32768U;

        pCfg->clrDiffRBIrCfg.gHFXferFactorIr    = 128U;
        pCfg->clrDiffRBIrCfg.gHFXferFactor      = 128U;

        pCfg->irSubCfg.irSubtractFiltEn = TRUE;
        pCfg->irSubCfg.irRemapLutEn     = FALSE;
        pCfg->irSubCfg.pIRRemapLut      = NULL;

        pCfg->irSubCfg.cutOffTh         = 64745U;
        pCfg->irSubCfg.transitionRange  = 5U;
        pCfg->irSubCfg.transitionRangeInv       = 51U;
        pCfg->irSubCfg.irSubDistScaleLut[0U]    = 256U;
        pCfg->irSubCfg.irSubDistScaleLut[1U]    = 256U;
        pCfg->irSubCfg.irSubDistScaleLut[2U]    = 256U;
        pCfg->irSubCfg.irSubDistScaleLut[3U]    = 256U;
        pCfg->irSubCfg.irSubDistScaleLut[4U]    = 256U;
        pCfg->irSubCfg.irSubFactScale[0U]       = 256U;
        pCfg->irSubCfg.irSubFactScale[1U]       = 256U;
        pCfg->irSubCfg.irSubFactScale[2U]       = 256U;
        pCfg->irSubCfg.irSubFactScale[3U]       = 256U;
    }
}

#ifdef __cplusplus
}
#endif

#endif /** CFG_PCID_H_ */
 /** @} */
