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
 *  \defgroup DRV_FCP_MODULE_CFG FCP Configuration
 *            This is VISS FCP FCPA Configuration file
 *
 *  @{
 */

/**
 *  \file   fcp_cfg.h
 *
 *  \brief  Inteface file for Flex Color processing module
 */

#ifndef FCP_CFG_H_
#define FCP_CFG_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_vpac_abstraction_layer.h>

/* Included to get the ioctl start */
#include <include/vhwa_common.h>
#include <soc/vhwa_soc.h>


#ifdef __cplusplus
extern "C" {
#endif

/* TODO:
 * 1, Histogram: Add API for Histogram Bank selection and API for
 *               reading histogram
 * 2, Currently all output muxes are exposed to the user, they will be
 *    moved to HAL layer once driver layer is implemented
 * 3, There is only one MuxC1_4 mux which is used as one of the
 *    input for both MLuma12 and Luma8 output, but both the enums
 *    \ref Fcp_Luma12OutSelect and \ref Fcp_Luma8OutSelect
 *    have options to select one of this color as output. If these
 *    muxes output is selected for both Luma12 and Luma8 outputs, the
 *    value should be same.
 *
 */


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \anchor Fcp_Ioctl
 *  \name   FCP IOCTL macros
 *  \brief  Control commands for FCP module in VISS
 *          Mainly used to set configuration of FCP submodules
 *
 *  @{
 */
/**
 * \brief Used for setting individual FCP sub-module's configuration.
 *        Single ioctl for configuring all sub-module's configuration,
 *        by selecting module id and setting appropriate pointer in
 *        #Fcp_Control.
 *        This is also used for configuring CFA module.
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_FCP_SET_CONFIG                    (VHWA_IOCTL_FCP_IOCTL_BASE)

/**
 * \brief Used for reading histogram data.
 *        This ioctl reads the current histogram data from the
 *        registers and returns it to user.
 *        Uses #Fcp_HistData as an argument
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_FCP_GET_HISTOGRAM                 (VHWA_IOCTL_FCP_IOCTL_BASE + 1U)

#if defined VHWA_VPAC_IP_REV_VPAC3
/**
 * \brief Used for setting individual FCP sub-module's configuration.
 *        Single ioctl for configuring all sub-module's configuration,
 *        by selecting module id and setting appropriate pointer in
 *        #Fcp_Control.
 *        This is also used for configuring CFA module.
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_FCP2_SET_CONFIG                    (IOCTL_FCP_GET_HISTOGRAM + 1u)
#endif
/** @} */



/*
 *  \brief Maximum number of CCM coefficients in each row
 */
#define FCP_MAX_CCM_COEFF_IN_RAW                (4U)
/*
 *  \brief Maximum number of CCM coefficients row
 */
#define FCP_MAX_CCM_COEFF                       (3U)

/*
 *  \brief Maximum number of RGB2YUV coefficients per Row
 */
#define FCP_MAX_RGB2YUV_COEFF                   (3U)

/*
 *  \brief Maximum number of RGB2HSV Input color
 */
#define FCP_RGB2HSV_MAX_IN_COLOR                (3U)

/*
 *  \brief Maximum number of CFA Coefficients
 */
#define FCP_MAX_CFA_COEFF                       (1728U)

/*
 *  \brief Maximum number of color components supported in RAWFE
 *         RAW FE supports any 2x2 format
 *         This enum is used in array size for different gains/offsets
 */
#define FCP_MAX_COLOR_COMP                      (4U)

/*
 *  \brief Maximum number of CFA Phase
 */
#define FCP_CFA_MAX_PHASE                       (4U)

/*
 *  \brief Maximum number of CFA Sets
 */
#define FCP_CFA_MAX_SET                         (2U)

/*
 *  \brief Maximum number of CFA Set threshold
 */
#define FCP_CFA_MAX_SET_THR                     (7U)

/** \brief Companding module Lut size
 */
#define FCP_COMPANDING_LUT_SIZE                 (639U)

/** \brief Contrast stretch/gamma module Lut size
 */
#define FCP_GAMMA_LUT_SIZE                      (513U)

/*
 *  \brief Lookup table size for the Edge Enhancer.
 */
#define FCP_EE_LUT_SIZE                         (4096U)


/** \brief 12b to 8b companding module Lut Size for RGB and YUV/Saturation
 */
#define FCP_RGB_YUVSAT_LUT_SIZE                 (513U)

/** \brief Size of the histogram data
 */
#define FCP_HISTOGRAM_SIZE                      (256U)

/**
 *  \anchor Fcp_SaturationMode
 *  \name   Saturation Mode
 *  \brief  This mode is used for selecting numerator in the saturation
 *          calculation block.
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< Numerator is sum of RGB minus min of RGB */
#define FCP_SAT_MODE_SUM_RGB_MINUS_MIN_RGB      (0U)
/**< Numerator is max of RGB minus min of RGB */
#define FCP_SAT_MODE_MAX_RGB_MINUS_MIN_RGB      (1U)
/**< Max value, used for error checking  */
#define FCP_SAT_MODE_MAX                        (2U)
/** @} */


/**
 *  \anchor Fcp_SaturationDiv
 *  \name   Saturation Divison
 *  \brief  This is used for selecting denominator in saturation operation.
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< Denominator is 1 */
#define FCP_SAT_DIV_1                           (0U)
/**< Denominator is Max of RGB */
#define FCP_SAT_DIV_MAX_RGB                     (1U)
/**< Denominator is 4096 - grey scale value  */
#define FCP_SAT_DIV_4096_MINUS_GREY             (2U)
/**< Denominator is sum of RGB */
#define FCP_SAT_DIV_SUM_RGB                     (3U)
/**< Max value, used for error checking  */
#define FCP_SAT_DIV_MAX                         (4U)
/** @} */


/**
 *  \anchor Fcp_Rgb2HsvInput
 *  \name   RGB 2 HSV input select
 *  \brief  This enum is used for selecting input to the RGB 2 HSV block.
 *          RGB input for the RGB2HSV module can come either from the
 *          output of the Contrast block, ie after gamma correcttion
 *          or from the input of the contrast block ie before
 *          gamma correction.
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< Input come from output of Gamma correction */
#define FCP_RGB2HSV_INPUT_CONTRAST_OUTPUT       (0U)
/**< Input come from input of Gamma correction */
#define FCP_RGB2HSV_INPUT_CONTRAST_INPUT        (1U)
/**< Max Value, used for error checking */
#define FCP_RGB2HSV_INPUT_MAX                   (2U)
/** @} */


/**
 *  \anchor Fcp_Rgb2HsvH1InputSelect
 *  \name   RGB 2 HSV H1 input select
 *  \brief  H1 input is used in calculating grey scale in the RGB 2 HSV module.
 *          This enum is used for selecting h1 input.
 *
 *          GreyScale is calculated using weighted average of H1, H2 and H3.
 *          H1 Input is selected using this enum.
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< H1 input is Red color */
#define FCP_RGB2HSV_H1_INPUT_RED_COLOR          (0U)
/**< H1 input is Min of RGB */
#define FCP_RGB2HSV_H1_INPUT_MIN_RGB            (1U)
/**< This will ensure enum is not packed, will
 *      always be contained in int */
#define FCP_RGB2HSV_H1_INPUT_MAX                (2U)
/** @} */


/**
 *  \anchor Fcp_Rgb2HsvH2InputSelect
 *  \name   RGB 2 HSV H2 input select
 *  \brief  H1 input is used in calculating grey scale in the RGB 2 HSV module.
 *          This enum is used for selecting h2 input.
 *
 *          GreyScale is calculated using weighted average of H1, H2 and H3.
 *          H2 Input is selected using this enum.
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< H2 input is Blue color */
#define FCP_RGB2HSV_H2_INPUT_BLUE_COLOR         (0U)
/**< H2 input is Max of RGB */
#define FCP_RGB2HSV_H2_INPUT_MAX_RGB            (1U)
/**< Max Value used for error checking */
#define FCP_RGB2HSV_H2_INPUT_MAX                (2U)
/** @} */


/**
 *  \anchor Fcp_HistInputSelect
 *  \name   Histogram Input Selection
 *  \brief  Selects the input to the histogram module
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< Histogram input is color Red */
#define FCP_HIST_IN_SEL_COLOR_RED               (0U)
/**< Histogram input is color Green */
#define FCP_HIST_IN_SEL_COLOR_GREEN             (1U)
/**< Histogram input is color Blue */
#define FCP_HIST_IN_SEL_COLOR_BLUE              (2U)
/**< Histogram input is Clear Chanel */
#define FCP_HIST_IN_SEL_CLR                     (3U)
/**< Histogram input is (R+2G+B)/4 */
#define FCP_HIST_IN_SEL_COLOR_R2GB_DIV_4        (4U)
/**< Histogram input is (R+G+B)/3 */
#define FCP_HIST_IN_SEL_COLOR_RGB_DIV_3         (5U)
/**< Max Value, used for error checking */
#define FCP_HIST_IN_SEL_MAX                     (6U)
/** @} */


/**
 *  \anchor Fcp_CfaCoreSelect
 *  \name   CFA Core Selection
 *  \brief  Selects CFA Gradient and direction core
 *          There are two sets of gradient and direction core in CFA
 *          to support sensors having two kinds of data type ie RGB-IR.
 *          Each set can be used for one kind of data type.
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< CFA Core set 0 */
#define FCP_CFA_CORE_SEL_CORE0                  (0U)
/**< CFA Core set 1 */
#define FCP_CFA_CORE_SEL_CORE1                  (1U)
/** @} */


/**
 *  \anchor Fcp_CfaCoreBlendMode
 *  \name   CFA blending mode
 *  \brief  Gradient and intensity calculation uses bitmask to
 *          select the desired pixel position for calculating
 *          the horizontal and the vertical gradient. This enum is
 *          used for selecting bit mask.
 *          Refer to spec for more details
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< CFA Core mode with Input 0 */
#define FCP_CFA_CORE_BLEND_MODE_INPUT0          (0U)
/**< CFA Core mode with Input 0 and 1 */
#define FCP_CFA_CORE_BLEND_MODE_INPUT01         (1U)
/**< CFA Core mode with Input 0, 1 and 2 */
#define FCP_CFA_CORE_BLEND_MODE_INPUT012        (2U)
/**< CFA Core mode with adaptive Input 0, 1 and 2 */
#define FCP_CFA_CORE_BLEND_MODE_ADAPT_INPUT012  (3U)
/**< Max Value, used for error checking */
#define FCP_CFA_CORE_BLEND_MODE_MAX             (4U)
/** @} */

/**
 *  \anchor Fcp_Luma12OutSelect
 *  \name   Selects the output on Luma12 output path
 *  \brief  enum to select output pipe/channel on luma12 output
 *          Luma12 output pipe is mainly used for outputting 12b luma, but
 *          It can also be used to output one of the CFA output color.
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< Output is disabled */
#define FCP_LUMA12_OUT_DISABLE                  (0U)
/**< C1 output of CFA on Luma12 output */
#define FCP_LUMA12_OUT_C1                       (1U)
/**< C2 output of CFA on Luma12 output */
#define FCP_LUMA12_OUT_C2                       (2U)
/**< C3 output of CFA on Luma12 output */
#define FCP_LUMA12_OUT_C3                       (3U)
/**< C4 output of CFA on Luma12 output */
#define FCP_LUMA12_OUT_C4                       (4U)
/**< Luma output after RGB2YUV conversion */
#define FCP_LUMA12_OUT_RGB2YUV                  (5U)
/**< Greyscale value after RGB2HSV conversion on Luma12 output */
#define FCP_LUMA12_OUT_RGB2HSV                  (6U)
/**< C1 Output of CFA on Luma12 output */
#define FCP_LUMA12_OUT_CFA_C1                   (7U)
/**< Max Value, used for error checking */
#define FCP_LUMA12_OUT_MAX                      (8U)
/** @} */


/**
 *  \anchor Fcp_Chroma12OutSelect
 *  \name   Selects the output on Chroma12 output path
 *  \brief  Enum to select output pipe/channel on chroma12 output
 *          Chroma12 output pipe is mainly used for outputting 12b chroma, but
 *          It can also be used to output C1 output color from CFA.
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< Output is disabled */
#define FCP_CHROMA12_OUT_DISABLE                (0U)
/**< 12b Chroma output */
#define FCP_CHROMA12_OUT_CHROMA                 (1U)
/**< C1 output of CFA on Chroma12 output */
#define FCP_CHROMA12_OUT_C1                     (2U)
/**< Max Value, used for error checking */
#define FCP_CHROMA12_OUT_MAX                    (3U)
/** @} */


/**
 *  \anchor Fcp_Luma8OutSelect
 *  \name   Selects the output on Luma8 output path
 *  \brief  Enum to select output pipe/channel on Luma8 output
 *          This channel is used to output Luma in 8b, it could either
 *          after RGB2YUV conversion or from RGB2HSV conversion. It can
 *          also be used to output red color of the RGB or one of the CFA
 *          output. When CFA output is selected, only MSB 8bits will be output.
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< Output is disabled */
#define FCP_LUMA8_OUT_DISABLE                   (0U)
/**< C1 output of CFA on Luma8 output */
#define FCP_LUMA8_OUT_C1                        (1U)
/**< C2 output of CFA on Luma8 output */
#define FCP_LUMA8_OUT_C2                        (2U)
/**< C3 output of CFA on Luma8 output */
#define FCP_LUMA8_OUT_C3                        (3U)
/**< C4 output of CFA on Luma8 output */
#define FCP_LUMA8_OUT_C4                        (4U)
/**< Luma output after RGB2YUV conversion
 *   This output will be converted from 12b to 8b using either
 *   Lut or shift */
#define FCP_LUMA8_OUT_RGB2YUV_Y8                (5U)
/**< Grey scale output after RGB2HSV conversion
 *   This output will be converted from 12b to 8b using either
 *   Lut or shift */
#define FCP_LUMA8_OUT_RGB2HSV_Y8                (6U)
/**< Red color output after CCM
 *   This output will be converted from 12b to 8b using either
 *   Lut or shift */
#define FCP_LUMA8_OUT_RED8                      (7U)
/**< C2 output of CFA on Luma8 output */
#define FCP_LUMA8_OUT_CFA_C2                    (8U)
/**< Max Value, used for error checking */
#define FCP_LUMA8_OUT_MAX                       (9U)
/** @} */


/**
 *  \anchor Fcp_Chroma8OutSelect
 *  \name   Selects the output on Chroma8 output path
 *  \brief  Enum to select output pipe/channel on Chroma8 output
 *          This channel is used to output Chroma in 8b, It can
 *          also be used to output green color of the RGB or one of the CFA
 *          output. When CFA output is selected, only MSB 8bits will be output.
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< Output is disabled */
#define FCP_CHROMA8_OUT_DISABLE                 (0U)
/**< Chroma output after RGB2YUV conversion,
 *   This output will be converted from 12b to 8b using either
 *   Lut or shift */
#define FCP_CHROMA8_OUT_CHROMA                  (1U)
/**< Green color output after CCM
 *   This output will be converted from 12b to 8b using either
 *   Lut or shift */
#define FCP_CHROMA8_OUT_GREEN                   (2U)
/**< C3 output of CFA on Chroma8 output */
#define FCP_CHROMA8_OUT_CFA_C3                  (3U)
/**< This will ensure enum is not packed, will
 *      always be contained in int */
#define FCP_CHROMA8_OUT_MAX                     (4U)
/** @} */


/**
 *  \anchor Fcp_Sat8OutSelect
 *  \name   Selects the output on Saturation-8 output path
 *  \brief  Enum to select output pipe/channel on Saturatin8 output
 *          This channel is used to output either saturation from RGB2HSV
 *          module or Blue color from after CCM or C4 output of CFA.
 *          When CFA output is selected, only MSB 8bits will be output.
 *
 *          Caution: This macro value is used directly in configuring register.
 *  @{
 */
/**< Output is disabled */
#define FCP_SAT8_OUT_DISABLE                    (0U)
/**< Saturation output from RGB2HSV module
 *   This output will be converted from 12b to 8b using either
 *   Lut or shift */
#define FCP_SAT8_OUT_SATURATION                 (1U)
/**< Blue color output after CCM
 *   This output will be converted from 12b to 8b using either
 *   Lut or shift */
#define FCP_SAT8_OUT_BLUE                       (2U)
/**< C4 output of CFA on Chroma8 output */
#define FCP_SAT8_OUT_CFA_C4                     (3U)
/**< Max Value, used for error checking */
#define FCP_SAT8_OUT_MAX                        (4U)
/** @} */


/**
 *  \anchor Fcp_ChromaMode
 *  \name   Chroma Mode
 *  \brief  FCP Chroma output mode, ie 422 or 420.
 *          Used only for YUV420 8bit output ie second output
 *          TODO: Remove it and decide it based on dataFormat
 *  @{
 */
/**< Output from chroma conversion is 420, ie downsampling on both sides */
#define FCP_CHROMA_MODE_420                     (0U)
/**< Output from chroma conversion is 422,
 *   ie downsampling only on vertical side */
#define FCP_CHROMA_MODE_422                     (1U)
/**< Max Value, used for error checking */
#define FCP_CHROMA_MODE_MAX                     (2U)
/** @} */


/**
 *  \anchor Fcp_Module
 *  \name   FCP Modules
 *  \brief  Defines the sub-modules within FCP. Used to identify sub-module
 *          of interest and to set or get the configuration of that module.
 *  @{
 */
/**< Companding module in FCP, this is used for converting 16bit input
 *   to 12bit output.
 *   It uses pointer to #Vhwa_LutConfig as an argument.
 *   Only enable, inputBits and tableAddr fields of this structure
 *   are used to configure companding module.
 *   Output clip parameter is not used as output is fixed to 12bit */
#define FCP_MODULE_COMPANDING                   (0U)
/**< Flex CFA module,
 *   Used for configuring CFA coefficients and thresholds */
#define FCP_MODULE_CFA                          (1U)
/**< Color Space Conversion Module,
 *   Used for setting color conversion modules coefficients and offsets */
#define FCP_MODULE_CCM                          (2U)
/**< Gamma/Contrast stretch module */
#define FCP_MODULE_GAMMA                        (3U)
/**< RGB 2 HSV color conversion module,
 *   Used for setting color conversion modules coefficients and offsets */
#define FCP_MODULE_RGB2HSV                      (4U)
/**< RGB 2 YUV color conversion module,
 *   Used for setting color conversion modules coefficients and offsets */
#define FCP_MODULE_RGB2YUV                      (5U)
/**< Module to select the outputs */
#define FCP_MODULE_OUT_SELECT                   (6U)
/**< Histogram module */
#define FCP_MODULE_HISTOGRAM                    (7U)
/**< Used for configuring RGB LUT */
#define FCP_MODULE_RGB_LUT                      (8U)
/**< Used for configuring YUV and Saturation */
#define FCP_MODULE_YUV_SAT_LUT                  (9U)
/**< Used for configuring YUV and Saturation */
#define FCP_MODULE_EE                           (10U)
#if defined VHWA_VPAC_IP_REV_VPAC3 || defined (VHWA_VPAC_IP_REV_VPAC3L)
/**< Flex CFA Companding LUT for 24 to 12 bit pixel conversion */
#define FCP_MODULE_CFA_COMPANDING               (11U)
/**< Flex CFA Decompanding LUT for 16 to 24 bit pixel conversion */
#define FCP_MODULE_DECOMPANDING                 (12U)
#endif
/**< Max Module, used for error checking */
#define FCP_MODULE_MAX                          (13U)
/** @} */


/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/**
 *  \brief Color Conversion Module
 *         Used 4 outputs of the CFA and generates 3 outputs.
 *         Allows genration of the different color combination from
 *         four inputs colors
 */
typedef struct
{
    int32_t                     weights[FCP_MAX_CCM_COEFF]
        [FCP_MAX_CCM_COEFF_IN_RAW];
    /**<  Weights of the CCM matrix.
          Four weights in each row for four input color.
          Format is 12b signed each (S12Q8) representing a range
          from -8 to +7.996 with 8 bits of fraction. */
    int32_t                     offsets[FCP_MAX_CCM_COEFF];
    /**<  Offsets of the CCM Matrix
          Format is 13b signed notation (S13Q11) representing a
          range of -4096 to +4095.
          An offset of 2048 should be programmed to apply a +1 offset
          to the incoming pixel value. */
} Fcp_CcmConfig;

/**
 *  \brief Contrast stretch/gamma correction module configuration
 *         Can be used to provide a gain and offset to stretch the
 *         histogram and increase contrast in the image.
 *         Can replace the standard gamma table for standard color processing.
 *         Uses independent Lut for each color.
 */
typedef struct
{
    uint32_t                    enable;
    /**< Flag to enable/disble Contrast Stretch/Gamm Correction module */
    uint32_t                    outClip;
    /**< Sets the contrast bit clip value,
     *   Contrast module bydefault supports 12bit input and 12bit
     *   unsigned output,
     *   But by using LUT and output Clip value, different output bitwidth
     *   are possible. */
    uint32_t                   *tableC1;
    /**< Pointer to the table containing 513 entries of Lut for C1 */
    uint32_t                   *tableC2;
    /**< Pointer to the table containing 513 entries of Lut for C2 */
    uint32_t                   *tableC3;
    /**< Pointer to the table containing 513 entries of Lut for C3 */
} Fcp_GammaConfig;

/**
 *  \brief RGB 2 YUV conversion coefficients.
 *         Converts 12bit RGB to 12bit YUV.
 *         Output of this conversion is YUV444.
 */
typedef struct
{
    int32_t                     weights[FCP_MAX_RGB2YUV_COEFF]
        [FCP_MAX_RGB2YUV_COEFF];
    /**< Weights of the conversion matrix
         Signed 12 bits (S12Q8) with 8 bit of fraction precision,
         representing a range of -8 to +7.996. */
    int32_t                     offsets[FCP_MAX_RGB2YUV_COEFF];
    /**< offsts of the conversion matrix
         13bit signed value. */
} Fcp_Rgb2YuvConfig;

/**
 *  \brief RGB to HSV color conversion module
 *         This module is used to convert from RGB format to Saturation
 *         and Greyscale value. Does not support hue generation.
 *         Greyscale is computed by doing weighter average of three input,
 *         H1, H2 and H3. H1 and H2 inputs can be selected from
 *         different sources.
 *         Saturation block first applies dynamic white balance offset to
 *         correct the saturation plane and then calculates the saturation.
 */
typedef struct
{
    uint32_t                    inputSelect;
    /**< Flag to select input source for RGB2HSV
     *   RGB input for the RGB2HSV module can come either from the
     *   output of the Contrast block, ie after gamma correcttion
     *   or from the input of the contrast block ie before
     *   gamma correction.
     *   Used to configure MuxRGBHSV mux,
     *   Refer \ref Fcp_Rgb2HsvInput for valid values */

    /* Parameters used for calculating grey value*/
    uint32_t                    h1Input;
    /**< Flag to select H1 input, it can either original color Red or
     *   Min of RGB, Configures MuxRGBHSV_H1,
     *   Refer \ref Fcp_Rgb2HsvH1InputSelect for valid values */
    uint32_t                    h2Input;
    /**< Flag to select H2 input, it can either original color Blue or
     *   Max of RGB, Configures MuxRGBHSV_H2,
     *   Refer \ref Fcp_Rgb2HsvH2InputSelect for valid values */
    uint32_t                    weights[FCP_RGB2HSV_MAX_IN_COLOR];
    /**< Weights used in calculating grey scale value
     *   12b signed with a range of -8 to +7.9996 in S12Q8 format */
    uint32_t                    offset;
    /**< Offset used in calculating grey scale value
     *   Signed 13 bits in S13Q11 format with a range of -2 to +1.995 */
    uint32_t                    useWbDataForGreyCalc;
    /**< Flag to select either white balance correct data or
     *   uncorrected data for V/grey calculation.
     *   UTRUE: Uses white balance corrected data
     *   UFALSE: Uses uncorrected data
     *   Configures Mux MuxRGBHSV_Mux_V*/

    /* White Balance Parameters */
    uint32_t                    wbOffset[FCP_RGB2HSV_MAX_IN_COLOR];
    /**< Dynamic White Balance offset applied to correct the saturation plane
     *   Applied only if independent pixel values are below a
     *   threshold (RGBHSV_WB_LINLOGTHR_*)
     *   */
    uint32_t                    threshold[FCP_RGB2HSV_MAX_IN_COLOR];
    /**< Dynamic White Balance threshold,
     *   pixel value is compared againts this threshold and wbOffset
     *   is applied only if pixel value is below this threshold.
     *   in U12 format
     *   index 0 is for Red channel format
     *   index 1 is for green channel format
     *   index 2 is for blue channel format */
    uint32_t                    satMinThr;
    /**< Threshold for comparing Min(RGB) limit
     *   Refer to FCP specs for more details */

    /* Saturation Parameters */
    uint32_t                    satMode;
    /**< Saturation Mode, used for selecting the numerator in
     *   saturation calculation,
     *   Refer \ref Fcp_SaturationMode for valid values  */
    uint32_t                    satDiv;
    /**< Saturation Divisor,
     *   Used for selecting denominator for the divisor in
     *   saturatin calculation
     *   Refer \ref Fcp_SaturationDiv for valid values */
} Fcp_Rgb2HsvConfig;

/**
 *  \brief Histogram Module configuration
 *         Used to configure Histogram module
 *         Used to enable/disable histogram module
 */
typedef struct
{
    uint32_t                    enable;
    /**< Flag to enable/disable histogram module
     *   UTRUE: Enables histogram module, when enabled stores the
     *         output in two 256x2bit memory in ping-pong fashion
     *   UFALSE: Disables histogram module */
    uint32_t                    input;
    /**< Selects histogram input,
     *   Refer \ref Fcp_HistInputSelect for valid values */
    Fvid2_CropConfig            roi;
    /**< Configures ROI for histogram */
} Fcp_HistConfig;

/**
 *  \brief Used to read histogram data
 */
typedef struct
{
    uint32_t                    fcpIdx;
    uint32_t                    *hist;
    /**< Histogram read data */
} Fcp_HistData;

/**
 *  \brief There are 5 output channels from VISS, the outputs format
 *         for all of them can be selecting appropriate data formats.
 *         The data format can further be overridded using below
 *         variables.
 *         For example, Luma and chroma 12 bit output can be selected
 *         by YUV420 data format and 12bit CCSF, luma12 output channel
 *         can further be changed to get Greyscale value from RGB2HSV
 *         output using \ref Fcp_Luma12OutSelect in the below structure.
 */
typedef struct
{
    uint32_t                    luma12OutSel;
    /**< Select output on Luma12b output channel,
     *   Refer \ref Fcp_Luma12OutSelect for valid values */
    uint32_t                    chroma12OutSel;
    /**< Select output on Chroma12b output channel,
     *   Refer \ref Fcp_Chroma12OutSelect for valid values */
    uint32_t                    luma8OutSel;
    /**< Select output on Luma8b output channel,
     *   Refer \ref Fcp_Luma8OutSelect for valid values */
    uint32_t                    chroma8OutSel;
    /**< Select output on Chroma8b output channel,
     *   Refer \ref Fcp_Chroma8OutSelect for valid values */
    uint32_t                    sat8OutSel;
    /**< Select output on Saturation output channel,
     *   Refer \ref Fcp_Sat8OutSelect for valid values */
    uint32_t                    chromaMode;
    /**< Used to select 420 or 422 output,
     *   Refer \ref Fcp_ChromaMode for valid values. */
} Fcp_OutputSelect;

#if defined VHWA_VPAC_IP_REV_VPAC3 || defined (VHWA_VPAC_IP_REV_VPAC3L)
/**
 *  struct Fcp_comDecomLutConfig
 *  \brief FCP Companding/Decompanding Lut configuration Structure
 *
 */
typedef struct
{
    uint32_t                    enable;
    /**< Flag to enable Lut */
    uint32_t                   *tableAddr[FCP_MAX_COLOR_COMP];
    /**< Pointer to the Lut Address,
         must not be null if enable is set to UTRUE */
} Fcp_comDecomLutConfig;

/**
 *  \brief Structure used to configure the CFA CCM
 */
typedef struct
{
    int32_t                    inputCh0;
    /**< Weight for each output channel from input channel 0 */
    int32_t                    inputCh1;
    /**< Weight for each output channel from input channel 1 */
    int32_t                    inputCh2;
    /**< Weight for each output channel from input channel 2 */
    int32_t                    inputCh3;
    /**< Weight for each output channel from input channel 3 */
    int32_t                     offset;
    /**< offset for each output channel */
} Fcp_CfaCcmConfig;

/**
 *  \brief Structure used to configure the CFA FIR
 */
typedef struct
{
    uint32_t                    enable;
    /**< Enable FIR for channel */
    uint32_t                    scaler;
    /**< U14Q8 scaler for FIR  */
    uint32_t                    offset;
    /**< U16 offset for FIR filter */
} Fcp_CfaFirConfig;
#endif

/**
 *  \brief Flex CFA/Demosaicing Module Configuration
 *         Refer to specs for more details on this module
 */
typedef struct
{
    uint32_t                    bypass[FCP_MAX_COLOR_COMP];
    /**< Enable/Bypass specific Core
     *   UTRUE: Bypasses the code ie output = input
     *   UFALSE: Enables core for CFA */
    int32_t                     coeff[FCP_MAX_CFA_COEFF];
    /**< CFA Coefficients */
    uint32_t                    coreSel[FCP_MAX_COLOR_COMP];
    /**< There are two sets of gradiant and direction cores available in the CFA
     *   This enum array is used for selecting core for each input color.
     *   Refer \ref Fcp_CfaCoreSelect for valid values. */
    uint32_t                    coreBlendMode[FCP_MAX_COLOR_COMP];
    /**< core Mode,
     *   Refer \ref Fcp_CfaCoreBlendMode for valid values */
    uint32_t                    gradHzPh[FCP_CFA_MAX_SET]
        [FCP_CFA_MAX_PHASE];
    /**< Gradient horizontal phase for both the cores,
     *   Refer to specs for more details */
    uint32_t                    gradVtPh[FCP_CFA_MAX_SET]
        [FCP_CFA_MAX_PHASE];
    /**< Gradient Vertical phase for both the cores,
     *   Refer to specs for more details */
    uint32_t                    intsBitField[FCP_CFA_MAX_SET]
        [FCP_CFA_MAX_PHASE];
    /**< Intensity bit fields for both the cores,
     *   Refer to specs for more details */
    uint32_t                    intsShiftPh[FCP_CFA_MAX_SET]
        [FCP_CFA_MAX_PHASE];
    /**< Intensity phase shift,
     *   Refer to specs for more details */
    uint32_t                    thr[FCP_CFA_MAX_SET]
        [FCP_CFA_MAX_SET_THR];
    /**< Thresholds for both the cores,
     *   Refer to specs for more details */
#if defined VHWA_VPAC_IP_REV_VPAC3 || defined (VHWA_VPAC_IP_REV_VPAC3L)
    uint32_t                    enable16BitMode;
    /**< Enables enhanced 16 bit CFA mode, enabled when LUT is disabled */
    uint32_t                    linearBitWidth;
    /**< the DLUT output bit width the CCM clipping bit width and the CLUT
         input bit width. Valid Value is between 12 to 24 */
    uint32_t                    ccmEnable;
    /**< Enable CCM */
    Fcp_CfaCcmConfig            ccmConfig[FCP_MAX_COLOR_COMP];
    /**< CCM configuration for each channel */
    Fcp_CfaFirConfig            firConfig[FCP_MAX_COLOR_COMP];
    /**< FIR configuration for each channel */
#endif
} Fcp_CfaConfig;

/**
 *  \brief Structure for configuring RGB Companding LUT
 *         This Lut is used for companding 12b RGB to 8b RGB
 *         If this lut is disabled, a fixed shift is used for
 *         converting 12b to 8b RGB.
 *         There is only one enable bit for all three luts.
 *
 *         Note: There are only 3 Luts physically present and the LUTs are
 *                muxed between Y/R, UV/G, Sat/B, so depending on the output
 *                selected, configure the companding Luts.
 */
typedef struct
{
    uint32_t                    enable;
    /**< Used to enable Lut based companding for RGB 12b to 8b conversion
         All three Lut pointer must not be null when set to UTRUE
         when set to UFALSE, shift is used to convert from 12b to 8b */
    uint32_t                   *redLutAddr;
    /**< Pointer to the Lut with 513 entries
         Used for converting red color from 12b to 8b */
    uint32_t                   *greenLutAddr;
    /**< Pointer to the Lut with 513 entries
         Used for converting green color from 12b to 8b */
    uint32_t                   *blueLutAddr;
    /**< Pointer to the Lut with 513 entries
         Used for converting blue color from 12b to 8b */
} Fcp_RgbLutConfig;

/**
 *  \brief Structure for configuring YUV and Saturation Companding LUT
 *         This Lut is used for companding 12b RGB to 8b RGB
 *         If this lut is disabled, a fixed shift is used for
 *         converting 12b to 8b Luma/Chroma/Saturation
 *         Each individual Lut can be enabled/disabled.
 *
 *         Note: There are only 3 Luts physically present and the LUTs are
 *                muxed between Y/R, UV/G, Sat/B, so depending on the output
 *                selected, configure the companding Luts.
 */
typedef struct
{
    uint32_t                    lumaInputBits;
    /**< Size of the Luma input in Bits
         Used as the input to 12to8 module and for for shift
         Must be less than or equal to 12 */
    uint32_t                    enableLumaLut;
    /**< Used to enable Lut based companding for Luma 12b to 8b conversion
         When set to UTRUE, #lumaLutAddr must not be Null
         When Set to UFALSE, shift, calculated based on #lumaInputBits, is used
         to convert from 12b to 8b */
    uint32_t                   *lumaLutAddr;
    /**< Pointer to the Lut with 513 entries
         Used for converting lut color from 12b to 8b */
    uint32_t                    enableChromaLut;
    /**< Used to enable Lut based companding for chroma 12b to 8b conversion
         When set to UTRUE, #chromaLutAddr must not be Null
         When Set to UFALSE, shift to convert from 12b to 8b */
    uint32_t                   *chromaLutAddr;
    /**< Pointer to the Lut with 513 entries
         Used for converting chroma color from 12b to 8b */
    uint32_t                    enableSaturLut;
    /**< Used to enable Lut based companding for Saturation 12b to 8b conversion
         When set to UTRUE, #saturLutAddr must not be Null
         When Set to UFALSE, shift to convert from 12b to 8b */
    uint32_t                   *saturLutAddr;
    /**< Pointer to the Lut with 513 entries
         Used for converting saturation color from 12b to 8b */
} Fcp_YuvSatLutConfig;

/*
 *  \brief Edge Enhancer configuration structure
 *         Defines the configuration required for the Edge Enhancer
 *
 */
typedef struct
{
    uint32_t                    enable;
    /**< Flag to Enable/Disable EE,
     *   UTRUE: Enables EE
     *   UFALSE: Disables EE */
    uint32_t                    alignY12withChroma;
    uint32_t                    alignY8withChroma;
    uint32_t                    eeForY12OrY8;
    /**< Selects Y12 or Y8 for EE input
        0, Y12
        1, Y8 */
    uint32_t                    bypassY12;
    uint32_t                    bypassC12;
    uint32_t                    bypassY8;
    uint32_t                    bypassC8;

    uint32_t                    leftShift;
    uint32_t                    rightShift;

    uint32_t                    yeeShift;
    int32_t                     coeff[9];
    uint32_t                    yeeEThr;
    uint32_t                    yeeMergeSel;
    uint32_t                    haloReductionOn;
    uint32_t                    yesGGain;
    uint32_t                    yesEGain;
    uint32_t                    yesEThr1;
    uint32_t                    yesEThr2;
    uint32_t                    yesGOfset;

    int16_t                    *lut;
} Fcp_EeConfig;

/**
 *  \brief FCP control structure
 */
typedef struct
{
    uint32_t                    module;
    /**< Select the module to be configured,
     *   Refer \ref Fcp_Module for valid values. */
    Vhwa_LutConfig             *inComp;
    /**< Input Companding configuration
         Used when module is set to #FCP_MODULE_COMPANDING
         Must not be null when module is set to
            #FCP_MODULE_COMPANDING */
    Fcp_CfaConfig              *cfa;
    /**< Pointer to CFA Configuration
         Used when module is set to #FCP_MODULE_CFA
         Must not be null when module is set to
            #FCP_MODULE_CFA */

    Fcp_CcmConfig              *ccm;
    /**< Pointer to CCM/Color Conversion Configuration
         Used when module is set to #FCP_MODULE_CCM
         Must not be null when module is set to
            #FCP_MODULE_CCM */
    Fcp_GammaConfig            *gamma;
    /**< Pointer to gamma/contrast stretch configuration
         Used when module is set to #FCP_MODULE_GAMMA
         Must not be null when module is set to
            #FCP_MODULE_GAMMA */
    Fcp_Rgb2HsvConfig          *rgb2Hsv;
    /**< Pointer to Rgb2Hsv config,
         Used when module is set to #FCP_MODULE_RGB2HSV
         Must not be null when module is set to
            #FCP_MODULE_RGB2HSV */
    Fcp_Rgb2YuvConfig          *rgb2Yuv;
    /**< Pointer to Rgb2Yuv config,
         Used when module is set to #FCP_MODULE_RGB2YUV
         Must not be null when module is set to
            #FCP_MODULE_RGB2HSV */
    Fcp_OutputSelect           *outSelect;
    /**< Pointer to output select structure
         Used for selecting different output formats on five outputs
         Must not be null when module is set to
         #FCP_MODULE_OUT_SELECT */
    Fcp_HistConfig             *hist;
    /**< Pointer to histogram structure
         Must not be null when module is set to
         #FCP_MODULE_HISTOGRAM */
    Fcp_RgbLutConfig           *rgbLut;
    /**< Pointer to Lut configuration,
         Must not be null when module is set to
         #FCP_MODULE_RGB_LUT */
    Fcp_YuvSatLutConfig        *yuvSatLut;
    /**< Pointer to Lut configuration,
         Must not be null when module is set to
         #FCP_MODULE_YUV_SAT_LUT */
    Fcp_EeConfig               *eeCfg;
    /**< Pointer to Edge Enhancer configuration,
         Must not be null when module is set to
         #FCP_MODULE_EE */
#if defined VHWA_VPAC_IP_REV_VPAC3 || defined (VHWA_VPAC_IP_REV_VPAC3L)
    Fcp_comDecomLutConfig       *cLutComp;
    /**< Input Companding configuration
         Used when module is set to #FCP_MODULE_CFA_COMPANDING
         Must not be null when module is set to
            #FCP_MODULE_CFA_COMPANDING */
    Fcp_comDecomLutConfig       *dLutComp;
    /**< Input Companding configuration
         Used when module is set to #FCP_MODULE_DECOMPANDING
         Must not be null when module is set to
            #FCP_MODULE_DECOMPANDING */
#endif
} Fcp_Control;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef FCP_CFG_H_ */
 /** @} */
