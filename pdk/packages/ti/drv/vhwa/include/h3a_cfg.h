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
 *  \defgroup DRV_H3A_MODULE_CFG H3A Configuration
 *            This is VISS H3A Configuration file
 *
 *  @{
 */

/**
 *  \file h3a_cfg.h
 *
 *  \brief Interface file for H3A module,
 *         Defines the structures / control operations that could be used to
 *         configure / control H3A module in VISS
 *         H3A is independent module, which takes input from RAWFE.
 *         This is why a separate interface file is used for
 *         controlling H3A.
 *
 */

#ifndef H3A_CFG_H_
#define H3A_CFG_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_vpac_abstraction_layer.h>

/* Included to get the ioctl start */
#include <include/vhwa_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \anchor H3a_Ioctl
 *  \name   H3A IOCTL macros
 *  \brief  Control commands for H3A module in VISS
 *          Mainly used to set H3A AF/AEWB configuration
 *
 *  @{
 */
/**
 * \brief Ioctl for setting H3A parameters.
 *        Only one of AF or AEWB module can be configured at a time.
 *        Only one of AF and AEWB can be enabled in the H3A
 *        module at a time for a frame.
 *        So this ioctl enables/supports only one of these modules. Refer to
 *        structure #H3a_Config
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_H3A_SET_CONFIG                    (VHWA_IOCTL_H3A_IOCTL_BASE)

/**
 * \brief Ioctl for Getting H3a output size for AF/AEWB module.
 *        Depending on the H3A AF/AEWB module enabled, it
 *        returns the output frame size. Refer to structure #H3a_Config
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_H3A_GET_AEWB_SIZE                 (IOCTL_H3A_SET_CONFIG + 1U)
/** @} */


/**
 *  \brief Maximum IIR coefficients
 */
#define H3A_AF_IIR_COEFF_MAX                    (11U)

/**
 *  \brief Maximum FIR coefficients
 */
#define H3A_AF_FIR_COEFF_MAX                    (5U)


/**
 *  \anchor H3a_Module
 *  \name   H3A Module, AF or AEWB
 *  \brief  Defines the sub-modules with in H3A. Used to identify sub-module
 *          of interest.
 *
 *  @{
 */
/**< AutoFocus module */
#define H3A_MODULE_MIN                          (0x0U)
/**< AutoFocus module */
#define H3A_MODULE_AF                           (0x1U)
/**< Auto Exposure and Auto White Balance Module */
#define H3A_MODULE_AEWB                         (0x2U)
/**< Max value, used internally for error checking */
#define H3A_MODULE_MAX                          (0x3U)
/** @} */

/**
 *  \anchor H3a_AfRgbPos
 *  \name   RGB Bayer format for AF
 *  \brief  Defines the sub-modules with in H3A. Used to identify sub-module
 *          of interest.
 *
 *          Caution: This macro values are directly used in
 *          configuring register
 *
 *  @{
 */
/**< The input Bayer format is GRGB */
#define H3A_AF_RGBPOS_GR_GB                     (0x0U)
/**< The input Bayer format is RGGB */
#define H3A_AF_RGBPOS_RG_GB                     (0x1U)
/**< The input Bayer format is GRBG */
#define H3A_AF_RGBPOS_GR_BG                     (0x2U)
/**< The input Bayer format is RGBG */
#define H3A_AF_RGBPOS_RG_BG                     (0x3U)
/**< The input Bayer format is GGRB */
#define H3A_AF_RGBPOS_GG_RB                     (0x4U)
/**< The input Bayer format is RBGG */
#define H3A_AF_RGBPOS_RB_GG                     (0x5U)
/**<  Max Value, used for error checking */
#define H3A_AF_RGBPOS_MAX                       (0x6U)
/** @} */

/**
 *  \anchor H3a_AfFvMode
 *  \name   AF Focus Value Accumulation Mode
 *  \brief  Defines accumulation mode for the focus value
 *          Refer to specs for more details
 *
 *          Caution: This macro values are directly used in
 *          configuring register
 *
 *  @{
 */
/**< FV Accumulation mode is sum mode */
#define H3A_AF_FV_MODE_SUM                      (0x0U)
/**< FV Accumulation mode is Peak mode */
#define H3A_AF_FV_MODE_PEAK                     (0x1U)
/**< Max Value, used for error checking */
#define H3A_AF_FV_MODE_MAX                      (0x2U)
/** @} */

/**
 *  \anchor H3a_AfVfMode
 *  \name   Auto Focus Vertical Focus Mode
 *  \brief  Defines vertical focus mode
 *          Refer to specs for more details
 *
 *          Caution: This macro values are directly used in
 *          configuring register
 *
 *  @{
 */
/**< Vertical Focus mode is 4 color horizontal FV Only */
#define H3A_AF_VF_HORZ_ONLY                     (0x0U)
/**< Vertical Focus mode is 1 color horizontal and 1 color vertical */
#define H3A_AF_VF_VERT_HORZ                     (0x1U)
/** @} */

/**
 *  \anchor H3a_AewbOutputMode
 *  \name   AEWB Output Mode
 *  \brief  Defines output mode for AEWB.
 *          Refer to specs for more details
 *
 *          Caution: This macro values are directly used in
 *          configuring register
 *
 *  @{
 */
/**< Output format is sum of square */
#define H3A_AEWB_OUTPUT_MODE_SUM_SQR            (0x0U)
/**< Output format is Min and Max Value */
#define H3A_AEWB_OUTPUT_MODE_MIN_MAX            (0x1U)
/**< Output format is sum only */
#define H3A_AEWB_OUTPUT_MODE_SUM_ONLY           (0x2U)
/** @} */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief IIR Filter Configuration structure
 */
typedef struct
{
    int32_t                     coeff[H3A_AF_IIR_COEFF_MAX];
    /**< IIR filter coefficients */
    uint32_t                    threshold;
    /**< Threshold of the filter */
} H3a_AfIirFiltConfig;

/**
 *  \brief FIR Filter Configuration structure
 */
typedef struct
{
    int32_t                     coeff[H3A_AF_FIR_COEFF_MAX];
    /**< FIR filter coefficients */
    uint32_t                    threshold;
    /**< Threshold of the filter */
} H3a_AfFirFilrConfig;

/**
 *  \brief Paxel/Window configuration structure
 *         Used to define paxel for AEWB and window for AF
 */
typedef struct
{
    Fvid2_PosConfig             pos;
    /**< Paxel/Window start Position
     *   For AF,
     *      Start horizontal position must be iirFiltStartPos + 2 and must be even
     *      Start vertical position must be greater than 8 if vertical mode is enabled,
     *      range is 0-4095
     *   For AEWB,
     *      Start horizontal position must be in 0-4095
     *      Start vertical position must be 0-4095 */
    uint32_t                    width;
    /**< Width of the paxel,
     *   minimum width required is 8 pixels */
    uint32_t                    height;
    /**< Height of the paxel
     *   The height could be from 2 to 256 */
    uint32_t                    horzCount;
    /**< Horizontal paxel count,
     *   Valid value is from 2 to 36 */
    uint32_t                    vertCount;
    /**< Vertical Paxel count,
     *   Maximum value supported is 128 */
    uint32_t                    horzIncr;
    /**< Horizontal increment,
     *   The Range is from 2 to 32 */
    uint32_t                    vertIncr;
    /**< Vertical increment,
     *   The range is from 2 to 32 */
} H3a_PaxelConfig;

/**
 * \brief Structure for ISP H3A AF engine parameters.
 */
typedef struct
{
    uint32_t                    enableALowCompr;
    /**< Flag to enable A Low Compression */

    uint32_t                    enableMedFilt;
    /**< Flag to enable/disable Medial Filter,
     *      to reduce Temperature Induced Noise */
    uint32_t                    midFiltThreshold;
    /**< Median Filter Threshold */

    uint32_t                    rgbPos;
    /**< RGB layout in bayer format,
     *   Refer to \ref H3a_AfRgbPos for valid values */

    H3a_PaxelConfig             paxelCfg;
    /**< Paxel configuration */

    uint32_t                    fvMode;
    /**< Defines type of accumulation for FV to be done,
     *   Refer \ref H3a_AfFvMode for valid values */

    uint32_t                    vfMode;
    /**< Vertical Focus Mode,
     *   Refer \ref H3a_AfVfMode for valid values */

    H3a_AfIirFiltConfig         iirCfg1;
    /**< IIR parameters */
    H3a_AfIirFiltConfig         iirCfg2;
    /**< IIR parameters */
    H3a_AfFirFilrConfig         firCfg1;
    /**< FiR Filter configuration */
    H3a_AfFirFilrConfig         firCfg2;
    /**< FiR Filter configuration */

    uint32_t                    iirFiltStartPos;
    /**< IIR filter start position */
} H3a_AfConfig;

/**
 * \brief Structure for ISP H3A AEWB engine parameters.
 */
typedef struct
{
    uint32_t                    enableALowComp;
    /**< Flag to enable A Low Compression */

    uint32_t                    enableMedFilt;
    /**< Flag to enable/disable Medial Filter, to reduce
     *   Temperature Induced Noise */
    uint32_t                    midFiltThreshold;
    /**< Median Filter Threshold */

    H3a_PaxelConfig             winCfg;
    /**< Paxel configuration */

    UInt32                      blackLineVertStart;
    /**< Vertical Window Start Position for single black line of windows.
     *   Sets the first line for the single black line of windows */
    UInt32                      blackLineHeight;
    /**< Window Height for the single black line of windows.
     *   This specifies the window height in an even number of pixels */

    uint32_t                    outMode;
    /**< Output Mode
     *   Refer to \ref H3a_AewbOutputMode for valid values */

    uint32_t                    sumShift;
    /**< AE/AWB engine shift value for the accumulation of pixel values
     *   This bit field sets the right shift value which is applied on
     *   the result of the pixel accumulation before it is stored in
     *   the packet. The accumulation takes place on 26 bits
     *   which is enough for 10-bit data and a maximum widow
     *   size of 512 x 512 which results into the accumulation of
     *   256 x 256 pixels of the same color. The shift value must
     *   be set such that the result fits on 16 bits. */

    uint32_t                    satLimit;
    /**< Saturation Limit, This is the value that all sub sampled pixels in the
     *   AE/AWB engine are compared to. If the data is greater or
     *   equal to this data then the block is considered saturated. */
} H3a_AewbConfig;

/**
 * \brief Structure for H3A configuration
 *        VPAC can output only one of AF or AEWB output. This structure
 *        is used for selecting H3A output module AF/AEWB and also used for
 *        for setting module configuration.
 *        When AF is enabled as an output module, only AF will be configured.
 *        Also when AEWB module is enabled/selected as an output module, only
 *        AEWB mdule will be configured.
 */
typedef struct
{
    uint32_t                    module;
    /**< H3A module to be configured,
     *   Refer \ref H3a_Module for valid values */
    Fvid2_PosConfig             pos;
    /**< Position of the actual frame start for H3A */
    H3a_AewbConfig              aewbCfg;
    /**< AEWB configuration, configured only when H3A_MODULE_AEWB is
     *   selected/enabled in #module */
    H3a_AfConfig                afCfg;
    /**< AF configuration, configured only when H3A_MODULE_AF is
     *   selected/enabled in #module */
    uint32_t                    outputSize;
    /**< Output Parameter returned by the driver,
     *   Internally driver calculates the output buffer required
     *   for the given AF/AEWB configuration and returns in this variable.
     *   Application can use to allocate H3A output buffer. */
} H3a_Config;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* H3A_CFG_H_ */

/* @} */
