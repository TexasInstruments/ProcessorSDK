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
 *  \defgroup DRV_RAWFE_MODULE_CFG RAW FE Configuration
 *            This is VISS RAW FE Configuration file
 *
 *  @{
 */

/**
 *  \file rawfe_cfg.h
 *
 *  \brief Interface file for RAW FE module,
 *         Defines the structures / control operations that could be used to
 *         configure / control RAW FE module in VISS
 *
 *         The processing pipeline in the RAW FE module is
 *         PWL -> DC Subtraction -> White Balance -> Decompanding LUT ->
 *         Merge -> Companding -> DPC -> LSC -> Whitebalance/gain Offset.
 *
 */

#ifndef RAWFE_CFG_H_
#define RAWFE_CFG_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_vpac_abstraction_layer.h>

/* Included for getting ioctl base for RAW FE */
#include <include/vhwa_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \anchor RFE_Ioctls
 *  \name   Ioctls for the RAW FE module
 *  \brief  Input/Output control MACRO's for RAW FE module
 *
 *  @{
 */
/**
 * \brief Used for setting individual RAWFE sub-module's configuration.
 *        Single ioctl for configuring all sub-module's configuration,
 *        by selecting module id and setting appropriate pointer in
 *        #Rfe_Control
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_RFE_SET_CONFIG                    (VHWA_IOCTL_RFE_IOCTL_BASE +   \
    0x0U)
#if defined (VHWA_VPAC_IP_REV_VPAC3L)
#define IOCTL_PCID_SET_CONFIG                   (IOCTL_RFE_SET_CONFIG + 1U)
#endif
/** @} */

/*
 *  \brief Size of the DPC Loopup table for OTF mode.
 */
#define RFE_DPC_OTF_LUT_SIZE                    (8U)

/*
 *  \brief Size of the Lookup table when Lut based defect
 *         pixel correction is used.
 */
#define RFE_DPC_LUT_SIZE                        (256U)

/*
 *  \brief Size of the Companding/Decompanding Lookup table
 */
#define RFE_COMP_DECOMP_LUT_SIZE                (639U)

/*
 *  \brief Size of the H3A Lut, used for companding
 */
#define RFE_H3A_COMP_LUT_SIZE                   (639U)

/*
 *  \brief Maximum number of entries spported in the LSC Lut
 *         LSC Lookup table size depends on the frame size and
 *         downscaling factor, this macro defins the maximum size
 *         for this lut.
 */
#if defined (VHWA_VPAC_IP_REV_VPAC3L)
#define RFE_LSC_TBL_SIZE                        (5248U)
#else
#define RFE_LSC_TBL_SIZE                        (4864U)
#endif

/*
 *  \brief Size of the Gamma LUT in FCC
 */
#define RFE_GAMMA_LUT_SIZE                      (513U)

#if defined (VHWA_VPAC_IP_REV_VPAC3L)
/*  Size of the LSC LUT MAP for the 1st and 2nd 8 channels
 *  in 4x4 pattern
 */
 #define RFE_LSC_LUT_MAP_SIZE                   (16U)
 #endif

/*
 *  \brief Maximum number of color components supported in RAWFE
 *         RAW FE supports any 2x2 format
 *         This enum is used in array size for different gains/offsets
 */
#define RFE_MAX_COLOR_COMP                      (4U)


/**
 *  \anchor Rfe_LscGainFmt
 *  \name   LSE Gain Format
 *  \brief  Lens Shading Correction Gain Format
 *          The 8bit gain applied in the LSC module could be
 *          in one of the below format. This enum is used for
 *          selecting this format.
 *  @{
 */
/**< Gain Format U8Q8, gain ranges from 0 to 0.996 */
#define RFE_LSC_GAIN_FMT_U8Q8                   (0U)
/**< Gain Format U8Q8, gain ranges from 1 to 1.996 */
#define RFE_LSC_GAIN_FMT_U8Q8_1                 (1U)
/**< Gain Format U8Q8, gain ranges from 0 to 1.992 */
#define RFE_LSC_GAIN_FMT_U8Q7                   (2U)
/**< Gain Format U8Q8, gain ranges from 1 to 2.992 */
#define RFE_LSC_GAIN_FMT_U8Q7_1                 (3U)
/**< Gain Format U8Q8, gain ranges from 0 to 2.984 */
#define RFE_LSC_GAIN_FMT_U8Q6                   (4U)
/**< Gain Format U8Q8, gain ranges from 1 to 3.984 */
#define RFE_LSC_GAIN_FMT_U8Q6_1                 (5U)
/**< Gain Format U8Q8, gain ranges from 0 to 6.968 */
#define RFE_LSC_GAIN_FMT_U8Q5                   (6U)
/**< Gain Format U8Q8, gain ranges from 1 to 7.968 */
#define RFE_LSC_GAIN_FMT_U8Q5_1                 (7U)
/**< Gain Format Max, used internally for error checking */
#define RFE_LSC_GAIN_FMT_MAX                    (0x8U)
/** @} */

/**
 *  \anchor Rfe_LscDsFactor
 *  \name   LSC LUT Down Scaling Factor
 *  \brief  Down Scaling factor for LSC LUT
 *          Used to select the downscaling factor for the LSC Lut.
 *
 *          Caution: The value of the macro is directly used in configuring
 *          LSC register.
 *  @{
 */
/**< LSC Lut down scaling factor is 8 */
#define RFE_LSC_DS_FACTOR_8                     (3U)
/**< LSC Lut down scaling factor is 16 */
#define RFE_LSC_DS_FACTOR_16                    (4U)
/**< LSC Lut down scaling factor is 32 */
#define RFE_LSC_DS_FACTOR_32                    (5U)
/**< LSC Lut down scaling factor is 64 */
#define RFE_LSC_DS_FACTOR_64                    (6U)
/**< LSC Lut down scaling factor is 128 */
#define RFE_LSC_DS_FACTOR_128                   (7U)
/**< Last value, used for error checking */
#define RFE_LSC_DS_FACTOR_MAX                   (8U)
/** @} */

/**
 *  \anchor Rfe_LscChnMode
 *  \name   LSC color channel mode
 *  \brief  LSC
 *
 *  @{
 */
/**< LSC legacy 4 channel mode*/
#define RFE_LSC_CHNMODE_4_LEGACY             (0U)
/**< LSC new 4 channel mode
 * LUT is specified for grid corner for all colors
 */
#define RFE_LSC_CHNMODE_4_NEW                (1U)
/**< LSC 8 channel mode*/
#define RFE_LSC_CHNMODE_8                    (2U)
/**< Invalid data  */
#define RFE_LSC_CHNMODE_INVALID              (3U)
/** @} */

/**
 *  \anchor Rfe_CFA_CFG_MODE
 *  \name   RawFE CFA Mode
 *  \brief  LSC
 *
 *  @{
 */
/**< Legacy 2x2 mode*/
#define RFE_CFA_CFG_MODE_2_LEGACY            (0U)
/**< Enhanced 2x2 mode */
#define RFE_CFA_CFG_MODE_2_ENHANCED          (1U)
/**< 4x4 RGBIR mode*/
#define RFE_CFA_CFG_MODE_4                   (2U)
/**< Invalid data  */
#define RFE_CFA_CFG_INVALID                  (3U)
/** @} */

/**
 *  \anchor Rfe_H3aInputSelect
 *  \name   H3A Input Select
 *  \brief  Used for selecting input for the H3A module
 *          The input to the H3A module can come from either Long,
 *          or short or vshort or LSC output. This is used to select
 *          the input for the H3a module.
 *  @{
 */
/**< Use Long input frame for H3A */
#define RFE_H3A_IN_SEL_LONG_FRAME               (0U)
/**< Use Short input frame for H3A */
#define RFE_H3A_IN_SEL_SHOFT_FRAME              (1U)
/**< Use Very Short input frame for H3A */
#define RFE_H3A_IN_SEL_VSHOFT_FRAME             (2U)
/**< Use LSC Output as input frame for H3A */
#define RFE_H3A_IN_SEL_LSC_OUT_FRAME            (3U)
#if defined (VHWA_VPAC_IP_REV_VPAC3L)
/**< Use PCID Output as input frame for H3A */
#define RFE_H3A_IN_SEL_PCID_OUT_FRAME           (4U)
/**< Last value, used error checking */
#define RFE_H3A_IN_SEL_MAX                      (5U)
#else
/**< Last value, used error checking */
#define RFE_H3A_IN_SEL_MAX                      (4U)
#endif
/** @} */


/**
 *  \anchor Rfe_Module
 *  \name   RAW FE Submodule
 *  \brief  Defines the sub-modules within RAWFE.
 *          RFE supports only one IOCTL, IOCTL_RFE_SET_CONFIG.
 *          Using this ioctl, the configuration for all modules can be set
 *          by selecting appropriate module in #Rfe_Control and setting
 *          the correct pointer.
 *  @{
 */
/**< Lut based Defect Correction Module */
#define RFE_MODULE_WB1_DC_SUB_MASK              (0x0U)
/**< PWL Module used for the long exposure input,
 *   Used only for three exposure merge ,
 *   Not used for two exposure merge or single exposure or linear mode */
#define RFE_MODULE_PWL1                         (0x1U)
/**< PWL Module used for
 *      - short exposure input for three exposure merge
 *      - long exposure input for two exposure merge
 *       Not used for single exposure or linear mode input */
#define RFE_MODULE_PWL2                         (0x2U)
/**< PWL Module used for
 *      - very short exposure input for three exposure merge
 *      - short exposure input for two exposure merge
 *      - Also used for single exposure or linear input frame */
#define RFE_MODULE_PWL3                         (0x3U)
/**< Lut based Decompanding module for Long input channel */
#define RFE_MODULE_DECOMP_LUT1                  (0x4U)
/**< Lut based Decompanding module for short and long input channel */
#define RFE_MODULE_DECOMP_LUT2                  (0x5U)
/**< Lut based Decompanding module for short and very short input channel */
#define RFE_MODULE_DECOMP_LUT3                  (0x6U)
/**< WDR Merge Module for MA1 block,
 *   This block is used only for three exposure merge,
 *   It must be bypassed for two exposure merge mode.
 *   The position of the exposure input channel is fixed, for three
 *   exposure merge, this block expects long and short
 *   channels as inputs. */
#define RFE_MODULE_WDR_MERGE_MA1                (0x7U)
/**< WDR Merge Module for MA2 block,
 *   This module is used for both two and three exposure
 *   merge scenarios.
 *   In two exposure merge, it expects long and short channels
 *   as input channels.
 *   In three exposure merge, it expects very short and output of MA1
 *   as input channnels. */
#define RFE_MODULE_WDR_MERGE_MA2                (0x8U)
/**< Lut based companding module
 *   Used to convert 20bit merge input to 16bit
 *   Used after both WDR merge block */
#define RFE_MODULE_COMP_LUT                     (0x9U)
/**< Lut based Defect Correction Module */
#define RFE_MODULE_DPC_LUT                      (0xAU)
/**< On-the-fly Defect Correction Module */
#define RFE_MODULE_DPC_OTF                      (0xBU)
/**< Lens Shading Correction Module */
#define RFE_MODULE_LSC                          (0xCU)
/**< Gain and Offset/WhiteBalance Module */
#define RFE_MODULE_GAIN_OFST                    (0xDU)
/**< H3A Config
 *   Used to select input for the H3A, from Long, Short, VeryShort,
 *      LSC output
 *   Instead of LUT, shift can also be used for bit depth conversion.
 *   Lut based companding for H3A
 *   Input Bitdepth for this LUT cannot exceed 16 and the LUT
 *   entry size is only 10 bits, since the H3A logic works on 10 bit data */
#define RFE_MODULE_H3A                          (0xEU)
/**< Used for configure H3A Lut for convert 16bit input to
 *   10bit output */
#define RFE_MODULE_H3A_LUT                      (0xFU)
/** @} */


/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/**
 *  \brief White balance configuration structure
 *         There are two white balance modules in the RAW FE.
 *         This structure is used for configuring WB module, just before PWL.
 *         It is also used to configure DC offset and mask..
 */
typedef struct
{
    uint32_t                    mask;
    /**< 16bit mask bit pattern,
         used to mask out any control information present in the data
         this field is used and will be configured even if pwl module
         is disabled,
         it gets applied first in the input pixels. */
    uint32_t                    shift;
    /**< 3 bit configuration to perform a right shift (>>) from 0 to 7 bits,
     *   used to mask/remove out any control information present in the data
     *   this field is used and will be configured even if pwl module
     *   is disabled,
     *   it gets applied after application of mask. */
    int32_t                     offset[RFE_MAX_COLOR_COMP];
    /**< DC Subtraction in S24 format
     *   Used for removing black level from input pixel
     *   this field is used and will be configured even if pwl module
     *   is disabled.
     *   It gets applied after PWL */
    uint32_t                    gain[RFE_MAX_COLOR_COMP];
    /**< White balance gains for each color component
     *   Used after applying DC subtraction
     *   H3A receives DC subtracted (but not WB corrected) data */

} Rfe_Wb1Config;

/**
 *  \brief PWL configuration Structure
 *         Used for configuring All three PWL modules one by one.
 *         Can be used even for configuring mask/shift and dc subtraction
 */
typedef struct
{
    uint32_t                    mask;
    /**< 16bit mask bit pattern,
         used to mask out any control information present in the data
         this field is used and will be configured even if pwl module
         is disabled,
         it gets applied first in the input pixels. */
    uint32_t                    shift;
    /**< 3 bit configuration to perform a right shift (>>) from 0 to 7 bits,
     *   used to mask/remove out any control information present in the data
     *   this field is used and will be configured even if pwl module
     *   is disabled,
     *   it gets applied after application of mask. */
    int32_t                     offset[RFE_MAX_COLOR_COMP];
    /**< DC Subtraction in S24 format
     *   Used for removing black level from input pixel
     *   this field is used and will be configured even if pwl module
     *   is disabled.
     *   It gets applied after PWL */
    uint32_t                    gain[RFE_MAX_COLOR_COMP];
    /**< White balance gains for each color component
     *   Used after applying DC subtraction
     *   H3A receives DC subtracted (but not WB corrected) data */


    uint32_t                    enable;
    /**< Flag to enable/disable PWL,
     *   Used to uncompress sensor compressed pixel data using PWL
     *   The PWL block supports an input size from 8 - 16 bits and
     *   can support an output up to 24 bits. */
    uint32_t                    xthr1;
    /**< Threshold value, 16bit is used */
    uint32_t                    xthr2;
    /**< Threshold value, 16bit is used */
    uint32_t                    xthr3;
    /**< Threshold value, 16bit is used */
    uint32_t                    ythr1;
    /**< Threshold value, 24bit is used */
    uint32_t                    ythr2;
    /**< Threshold value, 24bit is used */
    uint32_t                    ythr3;
    /**< Threshold value, 24bit is used */
    uint32_t                    slope1;
    /**< Slope value, 16bit is used */
    uint32_t                    slope2;
    /**< Slope value, 16bit is used */
    uint32_t                    slope3;
    /**< Slope value, 16bit is used */
    uint32_t                    slope4;
    /**< Slope value, 16bit is used */
    uint32_t                    slopeShift;
    /**< Slope value for shift, 7 bits are used */
    uint32_t                    outClip;
    /**< Output Clip Value */
} Rfe_PwlConfig;

/**
 *  \brief WDR Merge configuration Structure
 *         Used for configuring both the WDR Merge blocks in VPAC individually
 */
typedef struct
{
    uint32_t                    enable;
    /**< Flag to enable or bypass WDR Merge block
     *   UTRUE: enables WDR merge block, in this case it expects two
     *         exposure inputs.
     *   UFALSE: bypassed WDR Merge block, short exposure input will
     *          pass to the output. */
    uint32_t                    useShortExpForWgtCalc;
    /**< Flag to make use of Short Exposure for the Weight calculation
     *   UTRUE: Use Short exposure for weight calculcation
     *   UFALSE: Use long exposure for weight calculation
     *   Refer to specs for more details */
    uint32_t                    dst;
    /**< Down shift value, after WDR merge. Range 0x0 to 0x1F */
    uint32_t                    sbit;
    /**< Shift up value for short exposure frame. Range 0x0 to 0xF */
    uint32_t                    lbit;
    /**< Shift up value for long exposure frame. Range 0x0 to 0xF */
    uint32_t                    gshort;
    /**< WDR Merge parameter. Gain parameter to be applied for short frame.
     *      Range is 0 - 65535. */
    uint32_t                    glong;
    /**< WDR Merge parameter. Gain parameter to be applied for long frame.
     *      Range is 0 - 65535. */
    uint32_t                    lbk[RFE_MAX_COLOR_COMP];
    /**< Black level for long exposure.
     *   The black level at index 0 is applied to even pixel on even line.
     *   The black level at index 1 is applied to odd pixel on even line.
     *   The black level at index 2 is applied to even pixel on odd line.
     *   The black level at index 3 is applied to odd pixel on odd line.
     *   only 12 bits are used. */
    uint32_t                    sbk[RFE_MAX_COLOR_COMP];
    /**< Black level for short exposure.
     *   The black level at index 0 is applied to even pixel on even line.
     *   The black level at index 1 is applied to odd pixel on even line.
     *   The black level at index 2 is applied to even pixel on odd line.
     *   The black level at index 3 is applied to odd pixel on odd line.
     *   only 12 bits are used. */
    uint32_t                    lwb[RFE_MAX_COLOR_COMP];
    /**< White balance gain for Long exposure input pixels
     *   The gain at index 0 is applied to even pixel on even line.
     *   The gain at index 1 is applied to odd pixel on even line.
     *   The gain at index 2 is applied to even pixel on odd line.
     *   The gain at index 3 is applied to odd pixel on odd line.
     *   only 13 bits are used */
    uint32_t                    swb[RFE_MAX_COLOR_COMP];
    /**< White balance gain for Short exposure input pixels
     *   The gain at index 0 is applied to even pixel on even line.
     *   The gain at index 1 is applied to odd pixel on even line.
     *   The gain at index 2 is applied to even pixel on odd line.
     *   The gain at index 3 is applied to odd pixel on odd line.
     *   only 13 bits are used */
    uint32_t                    wdrThr;
    /**< WDR Threshold value. only 16 bits are used */
    uint32_t                    afe;
    /**< WDR Merge parameter. Exponential part of value in weight calculation
     *   6 bits unsigned value */
    uint32_t                    afm;
    /**< WDR Merge parameter. Mantissa part of value in weight calculation
     *   16 bits signed value */
    uint32_t                    bf;
    /**< WDR Merge parameter. Q0.15 BF * 2 ^ -16 * 2 ^ -5
     *   16 bits are used */
    uint32_t                    mas;
    /**< Adaptive filter, slope config 16 bits are used. */
    uint32_t                    mad;
    /**< Adaptive filter, threshold config 16 bits are used. */
    uint32_t                    mergeShift;
    /**< Post merger divide factor*/
    uint32_t                    mergeClip;
    /**< Post merger clip threshold*/
} Rfe_WdrConfig;

/**
 *  \brief DPC OTF configuration Structure
 *         When both DPC LUT and DPC OTF are enabled, this DPC OTF
 *         gets applied after DPC LUT.
 *         Automatically detecting the defects based on thresholds and slopes
 *         Preferred method when exact location of the
 *         defective pixels is not known.
 */
typedef struct
{
    uint32_t                    enable;
    /**< Flag to enable/disable DPC OTF module
     *   UTRUE: Enable this module
     *   UFALSE: Disables this module
     *   When module disabled, it does not update threshold/slope Lut */
    uint32_t                    threshold[RFE_DPC_OTF_LUT_SIZE];
    /**< Threshold value */
    uint32_t                    slope[RFE_DPC_OTF_LUT_SIZE];
    /**< slope value */
    #if defined (VHWA_VPAC_IP_REV_VPAC3L)
    uint32_t                    cfa_mode;
    /**< CFA pattern mode
     *   Refer \ref RFE_CFA_CFG_MODE for valid values */
    uint32_t                    cfa_phase;
    /**< CFA color phase specifies first red pixel position
     *   in the first 4x2 pixel array in a frame. Valid values 0-7 */
    uint32_t                    thrx[RFE_DPC_OTF_LUT_SIZE];
    /**< Threshold x value */
    uint32_t                    lut2_threshold[RFE_DPC_OTF_LUT_SIZE];
    /**< LUT2 threshold value */
    uint32_t                    lut2_slope[RFE_DPC_OTF_LUT_SIZE];
    /**< LUT2 slope value */
    uint32_t                    lut2_thrx[RFE_DPC_OTF_LUT_SIZE];
    /**< LUT2 threshold x value */
    uint32_t                    lut_map;
    /**< OTF DPC threshold LUT selection mapping for 4 channels
     *   Valid value range 0-15
     *   0 : red, 1 : green, 2 : blue, 3 : IR,
     *   When LUT_MAP[i] = 1, LUT2 threshold will be used
     *   for color i.
     *   When LUT_MAP[0] = 0, LUT1 threshold will be used
     *   */
    #endif
} Rfe_DpcOtfConfig;

/**
 *  \brief DPC Lut configuration Structure
 *         When both DPC LUT and DPC OTF are enabled, this DPC LUT
 *         gets applied first.
 *         At max, 256 defective pixels can be corrected using DPC LUT.
 *         Used when exact location of the
 *         (typically manufacturing )defective pixels are known.
 */
typedef struct
{
    uint32_t                    enable;
    /**< Flag to enable/disable DPC Lut module
     *   UTRUE: Enable this module
     *   UFALSE: Disables this module
     *   When module disabled, it does not update Lut */
    uint32_t                    isReplaceWhite;
    /**< When correction method is selected as 0
     *  (replace with white or black color) in LUT,
     *  This flag is used to select black or white color
     *  UTRUE: Replace defect pixel with white color
     *  UFALSE: Replace detect pixel with black color */
    uint32_t                    table[RFE_DPC_LUT_SIZE];
    /**< Table containing defect pixel x and y position and
     *   correction method */
    uint32_t                    maxDefectPixels;
    /**< Maximum number of detective pixels, ie size of array #table
     *   must be less than #RFE_DPC_LUT_SIZE */
} Rfe_DpcLutConfig;

/**
 *  \brief LSC configuration Structure
 *         Lens shading corretion is applied after DPC.
 *         It uses loopup table containig gains for each color component,
 *         applies it to pixel.
 *         Lookup table is for the entire image.
 *
 */
typedef struct
{
    uint32_t                    enable;
    /**< Flag to enable/disable LSC module
     *   UTRUE: Enable this module
     *   UFALSE: Disables this module
     *   When module disabled, it does not update Lut */
    uint32_t                    gainFmt;
    /**< Format of the Gain the LSC Lut
     *   Refer \ref Rfe_LscGainFmt for valid values */
    uint32_t                    horzDsFactor;
    /**< Horizontal Downscaling factor for LSC Lut
     *   Refer \ref Rfe_LscDsFactor for valid values */
    uint32_t                    vertDsFactor;
    /**< Vertical Downscaling factor for LSC Lut
     *   Refer \ref Rfe_LscDsFactor for valid values */
    uint32_t                   *tableAddr;
    /**< LSC Table Address,
     *   The size of the LSC table can be at max 4758 entries,
     *   each entry contains gains for all 4 colors.
     *   4758 locations with 4 Bytes/location, ie each entry
     *   contains gains forall four colors.
     *   TODO: Changed data type to uint64_t if required */
    uint32_t                    numTblEntry;
    /**< the number of valid entries in #tableAddr
     *   The number of entries in the table must be equal to
     *  ((inputWidth / horzDcFactor) + 1) x ((inputHeight / vertDcFactor) + 1).
     *  The size of each entry is 4bytes */
    #if defined (VHWA_VPAC_IP_REV_VPAC3L)
    uint32_t                    chn_mode;
    /**< LSC color channel mode
     *   Refer \ref Rfe_LscChnMode for valid values */
    uint32_t                    lut_map[RFE_LSC_LUT_MAP_SIZE];
    /**< LSC LUT MAP define which LUT to use for the 8 channels in 4x4 mode
     */
    #endif
} Rfe_LscConfig;

/**
 *  \brief White Balance configuration Structure
 *         Used for white balance correction
 *         Used for digital gain to be applied if the image
 *         is too dark even after analog gain and exposure
 *         time have been set to the maximum.
 *         Allows to set gain independently for each channel,
 *         the gain value can range from 0 to 31.996 in steps of 1/256.
 */
typedef struct
{
    uint32_t                    gain[RFE_MAX_COLOR_COMP];
    /**< White balance gain for each pixel in 2x2 format,
     *   The gain value at index 0 is applied to even pixel on even line.
     *   The gain value at index 1 is applied to odd pixel on even line.
     *   The gain value at index 2 is applied to even pixel on odd line.
     *   The gain value at index 3 is applied to odd pixel on odd line.
     *   in U13Q9 format */
    uint32_t                    offset[RFE_MAX_COLOR_COMP];
    /**< White balance offset for each pixel in 2x2 format,
     *   in S16 format
     *   The offset value at index 0 is applied to even pixel on even line.
     *   The offset value at index 1 is applied to odd pixel on even line.
     *   The offset value at index 2 is applied to even pixel on odd line.
     *   The offset value at index 3 is applied to odd pixel on odd line.
     *   */
} Rfe_GainOfstConfig;

/**
 *  \brief H3A Input Configuration structure
 *         Used to configure input modules for the H3A
 *         Used to select input to the H3A module and also
 *         Lut configuration for H3A input.
 */
typedef struct
{
    uint32_t                    inSel;
    /** Select input source for H3A,
     *  Refer \ref Rfe_H3aInputSelect for valid values */
    uint32_t                    shift;
    /**< U8, number of right shift, from 0 to 14
     *   Used to convert input bit depth to 10bit */
} Rfe_H3aInConfig;

/**
 *  \brief RAWFE control structure, passed as an argument to
 *         IOCTL_RFE_SET_CONFIG.
 */
typedef struct
{
    uint32_t                    module;
    /**< Id of the module to be configured,
     *   could be either AF or AEWB configuration
     *   Appropriate structure pointer need to be assigned when one of
     *   these modules is selected here
     *   Refer \ref Rfe_Module for valid values */
    Rfe_PwlConfig              *pwl3Cfg;
    /**< Pointer to PWL Configuration Configuration for short and very short
     *   exposure input.
     *   Should not be null when module is RFE_MODULE_PWL_VSHORT_SHORT.
     *   Used as input channel for
     *      linear mode
     *      short exposure input for two exposure mode
     *      very short exposure input for three exposure mode */
    Rfe_PwlConfig              *pwl2Cfg;
    /**< Pointer to PWL Configuration Configuration for short exposure input
     *   Should not be null when module is RFE_MODULE_PWL_SHORT_LONG.
     *   Used as input channel for
     *      long exposure for two exposure merge mode
     *      short exposure input for three exposure mode */
    Rfe_PwlConfig              *pwl1Cfg;
    /**< Pointer to PWL Configuration Configuration for long exposure input
     *   Should not be null when module is RFE_MODULE_PWL_LONG.
     *   Used as input channel for
     *      long exposure input for three exposure mode,
     *      not used in other modes. */
    Vhwa_LutConfig             *decomp3Cfg;
    /**< Pointer to Decompanding Lut Configuration for short and very short
     *   exposure input.
     *   Should not be null when module is
     *      RFE_MODULE_DECOMP_LUT_VSHORT_SHORT. */
    Vhwa_LutConfig             *decomp2Cfg;
    /**< Pointer to Decompanding Lut Configuration for Long and Short
     *   exposure input.
     *   Should not be null when module is
     *      RFE_MODULE_DECOMP_LUT_SHORT_LONG. */
    Vhwa_LutConfig             *decomp1Cfg;
    /**< Pointer to Decompanding Lut Configuration for Long
     *   exposure input.
     *   Should not be null when module is
     *   RFE_MODULE_DECOMP_LUT_LONG. */
    Rfe_WdrConfig              *wdrMergeMa1;
    /**< Pointer to WDR Merge Configuration
     *   Should not be null when module is RFE_MODULE_WDR_MERGE_MA1,
     *   Used for configuring Merge block 1
     *   Merge block-1 is used only for three exposure merge,
     *   otherwise it needs to be bypassed by setting enable flag to false */
    Rfe_WdrConfig              *wdrMergeMa2;
    /**< Pointer to WDR Merge Configuration
     *   Should not be null when module is RFE_MODULE_WDR_MERGE_MA2
     *   Used for configuring Merge block 2
     *   Merge block-2 is used for all, one, two and three
     *   exposure frame, merge and also for linear input mode. */
    Vhwa_LutConfig             *compCfg;
    /**< Pointer to Companding Lut Configuration
     *   Should not be null when module is RFE_MODULE_COMP_LUT. */
    Rfe_DpcOtfConfig           *dpcOtfCfg;
    /**< Pointer to DPC OTF Configuration
     *   Should not be null when module is RFE_MODULE_DPC_OTF */
    Rfe_DpcLutConfig           *dpcLutCfg;
    /**< Pointer to DPC Lut Configuration
     *   Should not be null when module is RFE_MODULE_DPC_LUT */
    Rfe_LscConfig              *lscConfig;
    /**< Pointer to LSC Configuration
     *   Should not be null when module is RFE_MODULE_LSC */
    Rfe_GainOfstConfig         *wbConfig;
    /**< Pointer to Gain and Offset/White Balance Configuration
     *   Should not be null when module is RFE_MODULE_GAIN_OFST */
    Rfe_H3aInConfig            *h3aInCfg;
    /**< Pointer to H3A configuration in RAWFE
     *   Should not be null when module is set to RFE_MODULE_H3A */
    Vhwa_LutConfig             *h3aLutCfg;
    /**< Pointer to H3A Lut Configuration in RAWFE
     *   Used to setup LUT for H3A input bit depth conversion.
     *   Should not be null when module is set to RFE_MODULE_H3A_LUT */
} Rfe_Control;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static inline void RfeControl_init(Rfe_Control *ctrl);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

static inline void RfeControl_init(Rfe_Control *ctrl)
{
    if (NULL != ctrl)
    {
        Fvid2Utils_memset(ctrl, 0x0, sizeof(Rfe_Control));
    }
}

#ifdef __cplusplus
}
#endif

#endif /* RAWFE_CFG_H_ */

/* @} */
