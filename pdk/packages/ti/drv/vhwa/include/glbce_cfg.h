/*
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
 *  \ingroup  DRV_VISS_MODULE
 *  \defgroup DRV_GLBCE_MODULE_CFG GLBCE Configuration
 *            This is VISS GLBCE Configuration file
 *
 *  @{
 */

/**
 *  \file glbce_cfg.h
 *
 *  \brief Interface file for GLBCE module,
 *         Defines the structures / control operations that could be used to
 *         configure / control GLBCE module in VISS
 *         GLBCE is independent module, between RAW FE and FCFA.
 *         This is why a separate interface file is used for
 *         controlling GLBCE.
 *
 */

#ifndef GLBCE_CFG_H_
#define GLBCE_CFG_H_

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
 *  \anchor Glbce_Ioctl
 *  \name   GLBCE IOCTL macros
 *  \brief  Control Commands for GLBCE module in VISS
 *
 *  @{
 */
/**
 * \brief Used for setting individual GLBCE sub-module's configuration.
 *        Single ioctl for configuring all sub-module's configuration,
 *        by selecting module id and setting appropriate pointer in
 *        #Glbce_Control.
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_GLBCE_SET_CONFIG                  (VHWA_IOCTL_GLBCE_IOCTL_BASE + \
    1U)
/**
 * \brief Used for getting individual GLBCE sub-module's configuration.
 *        Single ioctl for configuring all sub-module's configuration,
 *        by selecting module id and setting appropriate pointer in
 *        #Glbce_Control.
 *
 *        Currently this is supported only Statistics information.
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_GLBCE_GET_CONFIG                  (VHWA_IOCTL_GLBCE_IOCTL_BASE + \
    2U)

/**
 * \brief Used for getting the External Memory pointer allocated for GLBCE
 *        context save/restore stats memory.
 *
 *        Currently this is supported only for AM62A and J722S
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_GLBCE_SET_EXT_CNTXT_PTR                  (VHWA_IOCTL_GLBCE_IOCTL_BASE + \
    3U)
/** @} */

/**
 *  \brief Size of the asymmentry LUT
 */
#define GLBCE_ASYMMETRY_LUT_SIZE                (33U)

/**
 *  \brief Size of Perceptual LUT
 */
#define GLBCE_PERCEPT_LUT_SIZE                  (65U)

/**
 *  \brief Size of WDR LUT
 */
#define GLBCE_WDR_LUT_SIZE                      (257U)

/**
 *  \brief Value of the second pole, used in generating assymetry lut.
 */
#define GLBCE_ASYMMETRY_LUT_SECOND_POLE         (255U)


/**
 *  \anchor Glbce_DitherSize
 *  \name   GLBCE Dithering Size
 *  \brief  Defines number of bits to dither at the GLBCE output.
 *
 *          Caution: The values of this macro is directly in
 *          configuring register
 *
 *  @{
 */
/**< No Dithering */
#define GLBCE_NO_DITHER                     (0x0U)
/**< One least significant bit of the output is dithered */
#define GLBCE_DITHER_ONE_BIT                (0x1U)
/**< Two least significant bit of the output are dithered */
#define GLBCE_DITHER_TWO_BIT                (0x2U)
/**< Three least significant bit of the output are dithered */
#define GLBCE_DITHER_THREE_BIT              (0x3U)
/**< Four least significant bit of the output are dithered */
#define GLBCE_DITHER_FOUR_BIT               (0x4U)
/**< Max Dither, used for error checking */
#define GLBCE_DITHER_MAX                    (0x5U)
/** @} */

/**
 *  \anchor Glbce_Module
 *  \name   GLBCE Module
 *  \brief  Defines the sub-modules with in GLBCE.
 *          Used to identify sub-module of interest, in #Glbce_Control
 *          while calling #IOCTL_GLBCE_SET_CONFIG.
 *
 *  @{
 */
/**< Configure GLBCE common Settings */
#define GLBCE_MODULE_GLBCE                      (0x0U)
/**< Configure Forward Perceptual Settings */
#define GLBCE_MODULE_FWD_PERCEPT                (0x1U)
/**< Configure Forward Perceptual Settings */
#define GLBCE_MODULE_REV_PERCEPT                (0x2U)
/**< Command to get the GLBCE Stats Information */
#define GLBCE_MODULE_GET_STATS_INFO             (0x3U)
/**< Command to get/set WDR module configuration */
#define GLBCE_MODULE_WDR                        (0x4U)
/** @} */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief GLBCE configuration structure
 *         Used to configure common GLBCE configuration
 *         which is used for contrast brightness enhancement.
 */
typedef struct
{
    uint32_t                    irStrength;
    /**< This sets processing Strength. Minimum value is 0, maximum is 255.
     *   When set to 0x00, Video data will not be processed at all and will
     *   go to output unchanged. */

    uint32_t                    blackLevel;
    /**< Blank level of the input pixels,
     *   Value used here will be used as zero level for all GLBCE processing
     *   Data below Black level will not be processed and stay unchanged */
    uint32_t                    whiteLevel;
    /**< White level of the input pixels,
     *   Value used here will be used as white level for all GLBCE processing
     *   Data above white level will not be processed and stay unchanged*/

    uint32_t                    intensityVariance;
    /**< Variance Intensity - Sets the degree of sensitivity in the
     *   luminance domain. Maximum Variance is 0xF, and minimum
     *   Variance is 0x0 */
    uint32_t                    spaceVariance;
    /**< Variance Space - Sets the degree of spatial sensitivity of
     *   the algorithm. As this parameter is made smaller,
     *   the algorithm focuses on smaller regions within the image.
     *   Maximum Variance is 0xF, and minimum Variance is 0x0 */

    uint32_t                    brightAmplLimit;
    /**< The resultant tone curve cannot be lower than bright
     *   amplification limit line controlled by the this parameter.
     *   Maximum limit is 0xF, when the value is 0x0 there is no limit */
    uint32_t                    darkAmplLimit;
    /**< The resultant tone curve cannot be higher than dark
     *   amplification limit line controlled by the this parameter.
     *   Maximum limit is 0xF, when the value is 0x0 there is no limit */

    uint32_t                    dither;
    /**< Sets the number of LSB bits to dither,
     *   Refer \ref Glbce_DitherSize for valid values */

    uint32_t                    maxSlopeLimit;
    /**< Slope Max Limit is used to restrict the slope of the
     *   tone-curve generated by GLBCE */
    uint32_t                    minSlopeLimit;
    /**< Slope Min Limit is used to restrict the slope of the
     *   tone-curve generated by GLBCE */

    uint32_t                    asymLut[GLBCE_ASYMMETRY_LUT_SIZE];
    /**< The Asymmetry Function Lookup Table, The size of each entry is 16bits.
     *   The Asymmetry function is used to balance the GLBCE
     *   effect between the dark and bright regions of the image */
} Glbce_Config;


/**
 *  \brief Control structure for Perceptual Module.
 *         Used for both forward & reverse.
 *         Used to enable/disable module and to set Perceptual Lut
 */
typedef struct
{
    uint32_t                    enable;
    /**< Flag to enable/disable Forward or Reverse Percept */
    uint32_t                    table[GLBCE_PERCEPT_LUT_SIZE];
    /**< Perceptual Table */
} Glbce_PerceptConfig;

/**
 *  \brief Control structure for WDR Module.
 *         Used to enable/disable module and to set WDR Lut
 */
typedef struct
{
    uint32_t                    enable;
    /**< Flag to enable/disable FrontEnd Wdr */
    uint32_t                    table[GLBCE_WDR_LUT_SIZE];
    /**< WDR Table */
} Glbce_WdrConfig;

/**
 *  \brief GLBCE Stats memory information
 *         Used to get the address and size of the GLBCE statistics memory
 *         for the context save and restore purpose.
 */
typedef struct
{
    uint64_t                    addr;
    /**< Address of the stats memory */
    uint32_t                    size;
    /**< size of the stats memory */
} Glbce_StatsInfo;

/**
 *  \brief GLBCE control structure, passed as an argument to
 *         IOCTL_GLBCE_SET_CONFIG.
 */
typedef struct
{
    uint32_t                    module;
    /**< Id of the module to be configured,
     *   could be either AF or AEWB configuration
     *   Appropriate structure pointer need to be assigned when one of
     *   these modules is selected here
     *   Refer \ref Glbce_Module for valid values */
    Glbce_Config               *glbceCfg;
    /**< Glbce Configuration Structure */
    Glbce_PerceptConfig        *fwdPrcptCfg;
    /**< Forward Percept Config Configuration Structure */
    Glbce_PerceptConfig        *revPrcptCfg;
    /**< Forward Percept Config Configuration Structure */
    Glbce_WdrConfig            *wdrCfg;
    /**< WDR Configuration structure */
    Glbce_StatsInfo            *statsInfo;
    /**< Stats Informaion */
} Glbce_Control;

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

#endif /* GLBCE_CFG_H_ */

/* @} */
