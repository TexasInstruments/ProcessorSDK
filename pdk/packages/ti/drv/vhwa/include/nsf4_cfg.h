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
 *  \defgroup DRV_NSF4_MODULE_CFG NSF4 Configuration
 *            This is VISS NSF4 Configuration file
 *
 *  @{
 */

/**
 *  \file nsf4_cfg.h
 *
 *  \brief Interface file for NSF4 module,
 *         Defines the structures / control operations that could be used to
 *         configure / control NSF4 module in VISS
 *         NSF4 is independent module, between RAW FE and FCFA.
 *         This is why a separate interface file is used for
 *         controlling NSF4.
 *
 */

#ifndef NSF4_CFG_H_
#define NSF4_CFG_H_

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

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \anchor Nsf4v_Ioctl
 *  \name   Nsf4v IOCTL macros
 *  \brief  Control commands for NSF4v module in VISS
 *          Mainly used to set NSF4 configuration
 *
 *  @{
 */
/**
 * \brief Ioctl for getting NSF4 parameters. Refer to structure #Nsf4v_Config
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_NSF4_GET_CONFIG                   (VHWA_IOCTL_NSF4_IOCTL_BASE)

/**
 * \brief Ioctl for setting NSF4 parameters. Refer to structure #Nsf4v_Config
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_NSF4_SET_CONFIG                   (IOCTL_NSF4_GET_CONFIG + 1U)
#if defined (VHWA_VPAC_IP_REV_VPAC3) || defined (VHWA_VPAC_IP_REV_VPAC3L)
/**
 * \brief Ioctl for getting NSF4 histogram. Refer to structure #Nsf4_Histogram
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_NSF4_GET_HISTOGRAM                 (IOCTL_NSF4_SET_CONFIG + 1U)

/**
 * \brief Ioctl for setting NSF4 raw histogram ptr. Refer to structure #Nsf4_Histogram
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_SET_RAW_HIST_APP_PTR                 (IOCTL_NSF4_GET_HISTOGRAM + 1U)
#endif
 /** @} */


/** \brief Maximum number of PWL Sets for LSCC */
#define NSF4_LSCC_MAX_SET                       (2U)

/** \brief Maximum number of segments in PWL curve for LSCC */
#define NSF4_LSCC_MAX_SEGMENT                   (16U)

/** \brief Maximum TN Scale */
#define NSF4_MAX_SCALE                          (3U)

/** \brief Maximum number of segments in PWL curve for TN */
#define NSF4_TN_MAX_SEGMENT                     (12U)
#if defined (VHWA_VPAC_IP_REV_VPAC3) || defined (VHWA_VPAC_IP_REV_VPAC3L)
/** \brief Maximum number of segments in PWL curve for DWB */
#define NSF4_DWB_MAX_SEGMENT                     (8U)

/** \brief Maximum number of lines for DWB */
#define NSF4_DWB_MAX_LINE                        (2U)

/** \brief Maximum number of weights per line for DWB */
#define NSF4_DWB_MAX_LINE_WEIGHT                 (6U)

/** \brief Maximum number of ROIs for Histogram */
#define NSF4_HIST_MAX_ROI                        (8U)

/** \brief Histogram Lut size
 */
#define NSF4_HISTOGRAM_LUT_SIZE                  (609U)

/** \brief Histogram Max size
 */
#define NSF4_HISTOGRAM_SIZE                      (128U)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  struct VpacNsf4_PwlConfig
 *  \brief PWL configuration, used in defining PWL curve in LSCC
 */
typedef struct
{
    int32_t                     posX;
    /**< Position X for PWL config */
    int32_t                     posY;
    /**< Position X for PWL config */
    int32_t                     slope;
    /**< Slope S for PWL config */
} Nsf4_PwlConfig;

/**
 *  struct Nsf4_LsccConfig
 *  \brief LSCC configuration
 */
typedef struct
{
    uint32_t                    enable;
    /**< enable Lens Shading Correction Compensation */
    uint32_t                    setSel;
    /**< Selects one of the two sets of PWL curve for each
         color for LSCC,
         0: use set 0
         1: use set 1
         Fours bits are used, one for each color components,
         for selecting PWL curve.
         bit0 for color0
         bit1 for color1
         bit2 for color2
         bit3 for color3 */

    uint32_t                    gMaxCfg;
    /**< (U4.5) LSCC maximum gain.  Any calculated value is clipped
         to this max value
         Refer to Spec for more information */
    uint32_t                    tCfg;
    /**< (U4) LSCC radius dynamic range select.
         T is the right shift amount prior to MSB clip
         Refer to Spec for more information */
    uint32_t                    kvCfg;
    /**< (U2.6) LSCC horizontal or Y Gain for elliptical lens
         Refer to Spec for more information */
    uint32_t                    khCfg;
    /**< (U2.6) LSCC vertical or X Gain for elliptical lens
          Refer to Spec for more information */

    uint32_t                    lensCenterX;
    /**< (S14) Horizontal (X) position of lens center
         Refer to Spec for more information */
    uint32_t                    lensCenterY;
    /**< (S14) Horizontal (Y) position of lens center
         Refer to Spec for more information */

    Nsf4_PwlConfig              pwlCurve[NSF4_LSCC_MAX_SET]
        [NSF4_LSCC_MAX_SEGMENT];
    /**< PWL Curve parameter for both the sets */
} Nsf4_LsccConfig;

#if defined (VHWA_VPAC_IP_REV_VPAC3) || defined (VHWA_VPAC_IP_REV_VPAC3L)
/**
 *  struct Nsf4_Histogram
 *  \brief Structure for storing Histogram
 */
typedef struct
{
    uint32_t                    hist[NSF4_HISTOGRAM_SIZE];
    /**< Histogram Value */
} Nsf4_Histogram;

/**
 *  struct Nsf4_HistLutConfig
 *  \brief Structure for configuring Histogram LUT
 */
typedef struct
{
    uint32_t                    enable;
    /**< Used to enable Lut based companding for RGB 12b to 8b conversion
         All three Lut pointer must not be null when set to UTRUE
         when set to UFALSE, shift is used to convert from 12b to 8b */
    uint16_t                   *tableAddr;
    /**< Pointer to the Lut with 609 entries */
} Nsf4_HistLutConfig;

/**
 *  struct Nsf4_HistRoi
 *  \brief Histogram start and end pixel
 */
typedef struct
{
    uint32_t                    enable;
    /**< Enable the ROI for this region */
    Fvid2_PosConfig             start;
    /**< Valid line start pix position for histogram */
    Fvid2_PosConfig             end;
    /**< Valid line end pix position for histogram */
} Nsf4_HistRoi;

/**
 *  struct Nsf4_HistConfig
 *  \brief Histogram control parameters
 */
typedef struct
{
    uint32_t                     enable;
    /**< Raw domain Histogram Enable
         Refer to Spec for more information */
    uint32_t                     inBitWidth;
    /**< BitWidth of the input image
         Refer to Spec for more information */
    uint32_t                     phaseSelect;
    /**< Histogram Phase select enable
         Refer to Spec for more information */
    Nsf4_HistRoi                 roi[NSF4_HIST_MAX_ROI];
    /**< Histogram ROI */
    Nsf4_HistLutConfig           histLut;
    /**< Histogram LUT config */
} Nsf4_HistConfig;

/**
 *  struct Nsf4_DwbConfig
 *  \brief DWB control parameters
 */
typedef struct
{
    uint32_t                     enable;
    /**< Enable DWB */
    Nsf4_PwlConfig              dwbCurve[FVID2_BAYER_COLOR_COMP_MAX]
                                        [NSF4_DWB_MAX_SEGMENT];
    /**< DWB 8 segment piecewise linear curve for each color component */

    uint32_t                    dwbLineWeights[NSF4_DWB_MAX_LINE]
                                              [NSF4_DWB_MAX_LINE_WEIGHT];
    /**< DWB line weights to calculate intensity */
} Nsf4_DwbConfig;
#endif
/**
 *  struct Nsf4v_Config
 *  \brief NSF4 configuration
 */
typedef struct
{
    Nsf4_LsccConfig             lsccCfg;
    /**< Lens Shading Correction compansation configuration */

    uint32_t                    mode;
    /**< Noise filter mode configuration,
         bit4 is used selecting tnMode
         bit0 to 3 are used for selecing uMode, one bit for each
         color component */
    uint32_t                    gains[FVID2_BAYER_COLOR_COMP_MAX];
    /**< White balance gains for each color */
    uint32_t                    tKnee;
    /**< (U0.6) U Suppress curve knee.  X (LL2) value which separates
         constant suppression of 1.0 from linear suppression */
    uint32_t                    tnScale[NSF4_MAX_SCALE];
    /**< Tn scaling factor */

    Nsf4_PwlConfig              tnCurve[FVID2_BAYER_COLOR_COMP_MAX]
        [NSF4_TN_MAX_SEGMENT];
    /**< T_n 12 segment piecewise linear curve for each color component */
#if defined (VHWA_VPAC_IP_REV_VPAC3) || defined (VHWA_VPAC_IP_REV_VPAC3L)
    Nsf4_DwbConfig              dwbCfg;
    /**< Has Control parameters related to DWB */
    Nsf4_HistConfig             histCfg;
	/**< Has control parameters related to Raw Domain Histogram */
#endif
} Nsf4v_Config;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief This function should be used to initialize variable of type
 *          #Nsf4v_Config.
 *
 *  \param nsf4Config  A pointer of type Nsf4v_Config
 *
 *  \return         None
 */
static inline void Nsf4vConfig_init(Nsf4v_Config *nsf4Config);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static inline void Nsf4vConfig_init(Nsf4v_Config *nsf4Config)
{
    if (NULL != nsf4Config)
    {
        Fvid2Utils_memset(nsf4Config, 0U, sizeof(Nsf4v_Config));
    }
}

#ifdef __cplusplus
}
#endif

#endif /* NSF4_CFG_H_ */

/** @} */
