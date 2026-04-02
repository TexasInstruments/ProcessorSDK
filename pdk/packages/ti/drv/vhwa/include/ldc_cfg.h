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
 *  \ingroup  DRV_LDC_MODULE
 *  \defgroup DRV_LSC_MODULE_CFG LDC Configuration
 *            This is LDC Configuration file
 *
 *  @{
 */

/**
 *  \file ldc_cfg.h
 *
 *  \brief  Defines the structures / control operations that could be used to
 *              configure / control LDC module
 */

#ifndef CFG_LDC_H_
#define CFG_LDC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_vpac_abstraction_layer.h>

#include <include/vhwa_common.h>
#include <soc/vhwa_soc.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TODO: added support numBlockPerRow */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/**
 *  \anchor LDC_Ioctls
 *  \name   Ioctls for the LDCmodule
 *  \brief  Input/Output control MACRO's for LDC module
 *
 *  @{
 */
/**
 * \brief IOCTL for setting Luma Lut.
 *        struct #Ldc_RemapLutCfg is as argument
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_LDC_SET_LUMA_TONEMAP_LUT_CFG      (VHWA_IOCTL_LDC_IOCTL_BASE +   \
    0x0U)

/**
 * \brief IOCTL for setting Chroma Lut.
 *        struct #Ldc_RemapLutCfg is as argument
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_LDC_SET_CHROMA_TONEMAP_LUT_CFG    (VHWA_IOCTL_LDC_IOCTL_BASE +   \
    0x1U)
/** @} */


/* Maximum number of outputs supported in LDC */
#define LDC_MAX_OUTPUT                          (2u)

/**
 *  \brief Alignment requirement for LDC Mesh Table
 */
#define LDC_MESH_BUF_ADDR_ALIGN                 (0x10u)

/**
 *  \brief Maximum number of horizontal regions supported by LDC
 */
#define LDC_MAX_HORZ_REGIONS                    (3U)
/**
 *  \brief Maximum number of horizontal regions supported by LDC
 */
#define LDC_MAX_VERT_REGIONS                    (3U)

/**
 *  \brief Maximum number of regions supported by LDC
 */
#define LDC_MAX_REGIONS                         (LDC_MAX_HORZ_REGIONS * \
    LDC_MAX_VERT_REGIONS)

#define LDC_REMAP_LUT_SIZE                      (513u)

/**
 *  \anchor Ldc_LumaIntrType
 *  \name   LDC Luma Interpolation type
 *  \brief  Enum to define Luma interpolation type. For all other components,
 *         i.e. chroma or bayer, bilinear interpolation is used.
 *
 *  @{
 */
/**< \brief Bicubic interpolation is used for Luma */
#define VHWA_LDC_LUMA_INTRP_BICUBIC             (0U)
/**< \brief Bilinear interpolation is used for Luma */
#define VHWA_LDC_LUMA_INTRP_BILINEAR            (1U)
/**< \brief Interpolation Max, used for error checking */
#define VHWA_LDC_LUMA_INTRP_MAX                 (2U)
/** @} */

#if defined VHWA_VPAC_IP_REV_VPAC3 || defined VHWA_VPAC_IP_REV_VPAC3L
/**
 *  \anchor Ldc_ContinuousMode
 *  \name   LDC Continuous Mode
 *  \brief  Enum to define LDC continuous mode operation.
 *          Available only on VPAC3 and VPAC3L.
 *
 *  @{
 */
/**< \brief One Shot mode - LDC enable is cleared at the end of frame */
#define VHWA_LDC_CNTU_MODE_ONE_SHOT             (0U)
/**< \brief Continuous mode - LDC will continue to be enabled once set */
#define VHWA_LDC_CNTU_MODE_CONTINUOUS           (1U)
/**< \brief Continuous mode Max, used for error checking */
#define VHWA_LDC_CNTU_MODE_MAX                  (2U)
/** @} */
#endif

/**
 *  \anchor Ldc_LutDownScFactor
 *  \name   LDC Downscaling factor for LUT
 *  \brief  Enum to define down scaling factor for LUT.
 *
 *  @{
 */
/**< \brief no downsampling */
#define VHWA_LDC_LUT_DS_FACTOR_1                (0U)
/**< \brief Down sampling by 2 */
#define VHWA_LDC_LUT_DS_FACTOR_2                (1U)
/**< \brief Down sampling by 4 */
#define VHWA_LDC_LUT_DS_FACTOR_4                (2U)
/**< \brief Down sampling by 8 */
#define VHWA_LDC_LUT_DS_FACTOR_8                (3U)
/**< \brief Down sampling by 16 */
#define VHWA_LDC_LUT_DS_FACTOR_16               (4U)
/**< \brief Down sampling by 32 */
#define VHWA_LDC_LUT_DS_FACTOR_32               (5U)
/**< \brief Down sampling by 64 */
#define VHWA_LDC_LUT_DS_FACTOR_64               (6U)
/**< \brief Down sampling by 128 */
#define VHWA_LDC_LUT_DS_FACTOR_128              (7U)
/**< \brief Downscaling factor MAX, used for error checking  */
#define VHWA_LDC_LUT_DS_FACTOR_MAX              (8U)
/** @} */

/**
 *  \anchor Ldc_BurstLength
 *  \name   Burst Length for LDC Read DMA
 *  \brief  Enum to define max burst length for the Read Dma of LDC.
 *
 *  @{
 */
/**< \brief Max Burst length of 16 */
#define VHWA_LDC_MAX_BURST_LENGTH_16            (0U)
/**< \brief Max Burst length of 8 */
#define VHWA_LDC_MAX_BURST_LENGTH_8             (1U)
/**< \brief Max Burst length of 4 */
#define VHWA_LDC_MAX_BURST_LENGTH_4             (2U)
/**< \brief Max Burst length of 2 */
#define VHWA_LDC_MAX_BURST_LENGTH_2             (3U)
/**< \brief LDC Burst Length max, used for error checking */
#define VHWA_LDC_MAX_BURST_LENGTH_MAX           (4U)
/** @} */

/**
 *  \anchor Ldc_ErrorEvents
 *  \name   LDC Error Events
 *  \brief  Enum to define LDC Error events, for which LDC
 *          callback could be registered.
 *
 *          Caution: These macro values are directly used by driver
 *          for enabling events.
 *  @{
 */
/**< \brief Back mapped pixel co-ordinate goes out of the
 *          pre-computed input pixel bounding box */
#define VHWA_LDC_PIX_IBLK_OUTOFBOUND_ERR        (0x1U)
/**< \brief Block mesh co-ordinate goes out of the pre-computed
 *          mesh bounding box */
#define VHWA_LDC_MESH_IBLK_OUTOFBOUND           (0x2U)
/**< \brief Input Pixel block memory overflow */
#define VHWA_LDC_PIX_IBLK_MEMOVF                (0x4U)
/**< \brief Mesh block memory overflow */
#define VHWA_LDC_MESH_IBLK_MEMOVF               (0x8U)
/**< \brief Back mapped input co-ordinate goes out of input frame range */
#define VHWA_LDC_IFR_OUTOFBOUND                 (0x10U)
/**< \brief Affine and perspective transform precision overflow error */
#define VHWA_LDC_INT_SZOVF                      (0x20U)
/**< \brief Error on SL2 VBSUM Write interface */
#define VHWA_LDC_SL2_WR_ERR                     (0x80U)
/**< \brief Error on Input VBUSM Read interface */
#define VHWA_LDC_VBUSM_RD_ERR                   (0x100U)
/**< \brief Bitmask for all LDC Error Events */
#define VHWA_LDC_ALL_ERR_EVENTS                                                \
  (VHWA_LDC_PIX_IBLK_OUTOFBOUND_ERR | VHWA_LDC_MESH_IBLK_OUTOFBOUND |          \
   VHWA_LDC_PIX_IBLK_MEMOVF | VHWA_LDC_MESH_IBLK_MEMOVF |                      \
   VHWA_LDC_IFR_OUTOFBOUND | VHWA_LDC_INT_SZOVF | VHWA_LDC_SL2_WR_ERR |        \
   VHWA_LDC_VBUSM_RD_ERR)
/**< \brief Watchdog Timeout Error for LDC */
#define VHWA_LDC_WDTIMER_ERR                   (0x400000U)
/** @} */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 * \brief Prototype for the Error Event for LDC.
 *        The callback for the LDC error events can be registered
 *        using #Ldc_ErrEventParams. One of the parameter in this
 *        is call back. Driver calls this callback when error occurs.
 *
 * \param handle                FVID2 driver handle, for which error
 *                              has occurred.
 * \param errEvents             Error Events occured,
 *                              Refer to \ref Ldc_ErrorEvents for valid values.
 *
 * \return None.
 */
typedef void (*Ldc_ErrEventCbFxn)(Fvid2_Handle handle, uint32_t errEvents,
    void *appData);

/**
 * \brief Prototype for the Watchdog Timeout Error Event for LDC.
 *        The callback for the LDC Watchdog Timeout error events can be 
 *        registered using #Ldc_WdTimerErrEventParams. One of the parameter in this
 *        is call back. Driver calls this callback when error occurs.
 *
 * \param handle                FVID2 driver handle, for which error
 *                              has occurred.
 * \param errEvents             Error Events occured,
 *                              Refer to \ref Ldc_ErrorEvents for valid values.
 *
 * \return None.
 */
typedef void (*Ldc_WdTimerErrEventCbFxn)(Fvid2_Handle handle, uint32_t errEvents,
    void *appData);

/**
 * \brief Prototype for the config Register Memory Compare Event for LDC.
 *        The callback for the LDC register memory compare events can be registered
 *        using #Ldc_ConfigRegMemParams. One of the parameters in this
 *        is the callback. Driver calls this callback when the event occurs.
 *
 * \param handle                FVID2 driver handle, for which the event
 *                              has occurred.
 * \param configRegPrms         Pointer to configuration register parameters.
 * \param appData               Application-specific data pointer.
 *
 * \return                      Return FVID2_SOK on success, else error code.
 */
typedef int32_t (*Ldc_ConfigRegCompareCbFxn)(Fvid2_Handle handle, void *configRegPrms);

/**
 *  struct Ldc_IndChPrms
 *  \brief Independent channel parameters.
 *  [ Only applicable for VPAC3. NA for VPAC(J721E) ]
 */
typedef struct
{
    uint32_t    enable;
    /**< Enable Independent channel Params */
    uint32_t    pitch;
    /**< 2nd Channel pitch */
    uint32_t    ccsf;
    /**< 2nd channel ccsf */
}Ldc_IndChPrms;

/**
 *  struct Ldc_LutCfg
 *  \brief All LUT Parameters.
 */
typedef struct
{
    uint64_t                    address;
    /**< Read address for mesh offset table,
         must be #LDC_MESH_BUF_ADDR_ALIGN byte aligned */
    uint32_t                    lineOffset;
    /**< LDC Mesh table line offset,
         See \ref Ldc_LutDownScFactor for valid values */
    uint32_t                    dsFactor;
    /**< Defines the downsampling factors used for the mesh offset tables,
         Look into \ref Ldc_LutDownScFactor for valid values */
    uint32_t                    width;
    /**< The width of the frame for which mesh table mapping is available.
         This is independent of the compute/output frame width specified in
         outputFrameWidth. */
    uint32_t                    height;
    /**< The height of the frame for which mesh table mapping is available.
         This is independent of the compute/output frame height specified in
         outputFrameHeight. */
} Ldc_LutCfg;

/**
 *  struct Ldc_PerspectiveTransformCfg
 *  \brief Perspective transformation parameters.
 */
typedef struct
{
    uint32_t                    enableWarp;
    /**< Enable perspective warp transformation,
     *   0: Disables this transformation
     *   1: Enables this transformation
     *   If Disabled, coeffG and coeffH must be set to 0.
     *   For the affine transformation,
     *      set the parameters coeffA, coeffB, coeffC, coeffD, coeffE, coeffF,
     *   To disable affine transformation, set the parameters as below
     *   coeffA = coeffE = 4096, coeffB = coeffC = coeffD = coeffD = 0 */
    int32_t                     coeffA;
    /**< Perspective Transformation Parameter A */
    int32_t                     coeffB;
    /**< Perspective Transformation Parameter B */
    int32_t                     coeffC;
    /**< Perspective Transformation Parameter C */
    int32_t                     coeffD;
    /**< Perspective Transformation Parameter D */
    int32_t                     coeffE;
    /**< Perspective Transformation Parameter E */
    int32_t                     coeffF;
    /**< Perspective Transformation Parameter F */
    int32_t                     coeffG;
    /**< Perspective Transformation Parameter G */
    int32_t                     coeffH;
    /**< Perspective Transformation Parameter H */
} Ldc_PerspectiveTransformCfg;

/**
 *  struct Ldc_RegionConfig
 *  \brief LDC Region configuration parameters.
 */
typedef struct
{
    uint32_t            enable[LDC_MAX_VERT_REGIONS][LDC_MAX_HORZ_REGIONS];
    /**< Flag to enable/Disable a region
         Supports max 3x3 regions,
         Each region can be enabled or bypassed usig this flag */
    uint32_t            blockWidth[LDC_MAX_VERT_REGIONS][LDC_MAX_HORZ_REGIONS];
    /**< Block Width for each region */
    uint32_t            blockHeight[LDC_MAX_VERT_REGIONS][LDC_MAX_HORZ_REGIONS];
    /**< Block Height for each region */
    uint32_t            pixelPad[LDC_MAX_VERT_REGIONS][LDC_MAX_HORZ_REGIONS];
    /**< Pixel Padding for each region */

    uint32_t            width[LDC_MAX_HORZ_REGIONS];
    /**< Width of the horizontal slices
         Supports slicing entire frame into 3 horizontal slices
         Within each slice, vertical regions have same width */
    uint32_t            height[LDC_MAX_VERT_REGIONS];
    /**< height of the horizontal slices
         Supports slicing entire frame into 3 vertical slices
         Within each slice, horizontal regions have same height */
} Ldc_RegionConfig;

/**
 *  struct Ldc_RdBwLimitConfig
 *  \brief Bandwidth limit configuration for Read DMA.
 *         This condiguration is directly set in the register, so will be
 *         used by all handles/opens.
 */
typedef struct
{
    uint32_t                    rdBwLimit;
    /**< Limits the mean bandwidth (computed over one block)
         that the LDC module can request for read from
         system memory.
         0 = Bandwidth limiter is bypassed
         1:4095 = maximum number of bytes per cycle multiplied by 2^8 */
    uint32_t                    rdTagCnt;
    /**< Limits the maximum number of outstanding LDC requests,
         The Max possible value is 0x1F */
    uint32_t                    rdMaxBurstLength;
    /**< Limits the maximum burst length that could be used by LDC.
         Each burst is of 16 bytes.
         For K3 devices, it is best to keep burst size of 8 which
         command size of 128 bytes.
         Look into \ref Ldc_BurstLength for valid values */
} Ldc_RdBwLimitConfig;

/**
 *  struct Ldc_Config
 *  \brief All configuration Parameters for LDC.
 */
typedef struct
{
    Fvid2_Format                inFmt;
    /**< Input Frame format, describing input frames storage format
         Following parametes are used from this
         width = width of the input frame
         height = height of the input frame
         pitch = line offset for the input data,
                 only pitch[0] is used for all dataformats
                 For YUV420, pitch is common for both luma and chroma
         dataFormat = Input DataFormat
         sfcc = Storage format */

    UInt32                      enableOutput[LDC_MAX_OUTPUT];
    /**< Flags to enable LDC Outputs
         Output[0] must be enabled
         Output[1] is optional
         1: Enables the output
         0: Disables the output */

    Fvid2_Format                outFmt[LDC_MAX_OUTPUT];
    /**< Output Frame format, describing output frames storage format

         Following parametes are used from this
         pitch = line offset for the input data, used based on the data format
         dataFormat = Input DataFormat
         sfcc = Storage format

         Frame size for all the outputs is same and is configured in
         #outputFrameWidth and #outputFrameHeight in common place */

    uint32_t                    enableBackMapping;
    /**< Flag to enable/disable LDC Back Mapping
     *   1: Enables LDC back mapping
     *   0: Disables LDC Back mapping */

    uint32_t                    lumaIntrType;
    /**< Luma Interpolation Type,
         Look into \ref Ldc_LumaIntrType for valid values */

#if defined VHWA_VPAC_IP_REV_VPAC3 || defined VHWA_VPAC_IP_REV_VPAC3L
    uint32_t                    continuousMode;
    /**< Continuous mode enable (VPAC3/VPAC3L only)
         0: One Shot mode (default) - LDC enable is cleared at the end of frame
         1: Continuous mode - LDC will continue to be enabled once set
         Look into \ref Ldc_ContinuousMode for valid values */
#endif

    Ldc_PerspectiveTransformCfg     perspTrnsformCfg;
    /**< Affine transformation parameters */

    Ldc_LutCfg                  lutCfg;
    /**< LDC Lut configuration */

    uint32_t                    outputFrameWidth;
    /**< Output Frame Width, could be different from input frame width */
    uint32_t                    outputFrameHeight;
    /**< Output Frame Height, could be different from input frame width */
    uint32_t                    pixelPad;
    /**< Pixel pad */
    uint32_t                    outputBlockWidth;
    /**< Output block width
     *   must be
     *      multiple of 8 in 422/422 mode
     *      must be greater than or equal to 8 */
    uint32_t                    outputBlockHeight;
    /**< Output block Height
     *   must be even and greater than 0 */
    uint32_t                    outputStartX;
    /**< Output starting X-coordinate (must be even and multiple of 8)
     */
    uint32_t                    outputStartY;
    /**< Output starting Y-coordinate (must be even)
     */

    uint32_t                    enableMultiRegions;
    /**< Enable Multi Region Processing */
    Ldc_RegionConfig            regCfg;
    /**< Region Configuration
         Used only when enableMultiRegions = UTRUE
         Used to configure different block size and pixel pad
         for each region
         Output Width of all enabled regions in a vetical slice
         must be same
         Similarly Output Height of all enabled region in a horizontal slice
         must be same
         The sum of width of all enabled regions in a vertical slice
         must be equal or less than #outputFrameWidth
         Similarly, The sum of height of all enabled regions in a
         horizontal slice must be equal or less than #outputFrameHeight
      */

    /* Below Parameters are only applicable for VPAC3. NA for VPAC(J721E) */
    Ldc_IndChPrms               indChPrms;
    /**< Independent cahnnel parameters */

    uint32_t                    indOutChCcsf[LDC_MAX_OUTPUT];
    /**< This parameter is valid only when the independent channel configuration
         is enabled and used to set Storage format for Chroma channel */
     Ldc_RdBwLimitConfig        rdBwLimitCfg;
    /**< Used to save a copy of the LDC bandwidth limiter config in the handle object for Reconfiguration and readback */
    /* Additional Parameter end */
} Ldc_Config;

/**
 *  struct Ldc_ErrEventParams
 *  \brief Structure for error event parameters
 *         Used to register callback for the given set of events.
 */
typedef struct
{
    uint32_t                    errEvents;
    /**< bitmask of error events,
     *   Multiple error events can be registered with the same callback,
     *   Driver returns bitmask of events, which has occured,
     *   Refer to \ref Ldc_ErrorEvents for valid values. */
    Ldc_ErrEventCbFxn           cbFxn;
    /**< Callback function to be call on error events */
    void                       *appData;
    /**< private data */
} Ldc_ErrEventParams;
/**
 *  struct Ldc_WdTimerErrEventParams
 *  \brief WD Timer parameters
 *         Used to configure WD Timer parameters
 *         Must be called after SET_PARAMS ioctl
 *         Used with IOCTL_VHWA_M2M_LDC_CONFIG_WDTIMER ioctl
 */
typedef struct
{
    uint32_t                    wdTimeoutCycles;
    /**< Used to select timeout cycles
     *   Refer to #VHWA_WdTimeOutCycles for valid values */
    uint32_t                    wdTimerErrEvents;
    /**< watchdog timer error event value,
     *   VHWA_LDC_WDTIMER_ERR can be registered with this callback
     *   Refer to \ref Ldc_ErrorEvents for valid values. */
    Ldc_WdTimerErrEventCbFxn    cbFxn;
    /**< Callback function to be call on error events */
    void                       *appData;
    /**< private data */
} Ldc_WdTimerErrEventParams;
/**
 *  struct Ldc_RemapLutCfg
 *  \brief Structure for Remap Lut Configuration
 */
typedef struct
{
    uint32_t                    enable;
    /**< Flag to enable/disable Lut */
    uint32_t                    inputBits;
    /**< Number of input bits, valid values 8 to 12 */
    uint32_t                    outputBits;
    /**< Number of output bits, valid values 8 to 12 */
    uint16_t                   *tableAddr;
    /**< Pointer to the Lut Address,
         must not be null if enable is set to UTRUE */
} Ldc_RemapLutCfg;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  Ldc_ConfigInit
 *  \brief This function should be used to initialize variable of type
 *          #Ldc_Config.
 *
 *  \param ldcCfg   A pointer of type Ldc_Config
 *  \return         None
 */
static inline void Ldc_ConfigInit(Ldc_Config *ldcCfg);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static inline void Ldc_ConfigInit(Ldc_Config *ldcCfg)
{
    if (NULL != ldcCfg)
    {
        ldcCfg->enableBackMapping           = (uint32_t) UTRUE;
        ldcCfg->lumaIntrType                = VHWA_LDC_LUMA_INTRP_BICUBIC;
#if defined VHWA_VPAC_IP_REV_VPAC3 || defined VHWA_VPAC_IP_REV_VPAC3L
        ldcCfg->continuousMode              = VHWA_LDC_CNTU_MODE_ONE_SHOT;
#endif
        ldcCfg->perspTrnsformCfg.enableWarp = (uint32_t) UFALSE;
        ldcCfg->perspTrnsformCfg.coeffA     = 4096;
        ldcCfg->perspTrnsformCfg.coeffB     = 0;
        ldcCfg->perspTrnsformCfg.coeffC     = 0;
        ldcCfg->perspTrnsformCfg.coeffD     = 0;
        ldcCfg->perspTrnsformCfg.coeffE     = 4096;
        ldcCfg->perspTrnsformCfg.coeffF     = 0;
        ldcCfg->perspTrnsformCfg.coeffG     = 0;
        ldcCfg->perspTrnsformCfg.coeffH     = 0;
        ldcCfg->lutCfg.address              = 0U;
        ldcCfg->lutCfg.lineOffset           = 0U;
        ldcCfg->lutCfg.dsFactor             = VHWA_LDC_LUT_DS_FACTOR_4;
        ldcCfg->lutCfg.width                = 0U;
        ldcCfg->lutCfg.height               = 0U;

        ldcCfg->pixelPad                    = 2U;
        ldcCfg->outputBlockWidth            = 32U;
        ldcCfg->outputBlockHeight           = 36U;
        ldcCfg->outputFrameWidth            = 0U;
        ldcCfg->outputFrameHeight           = 0U;
        ldcCfg->outputStartX                = 0U;
        ldcCfg->outputStartY                = 0U;
        ldcCfg->enableMultiRegions          = (uint32_t)UFALSE;
        ldcCfg->rdBwLimitCfg.rdBwLimit      = (uint32_t)UFALSE;
        ldcCfg->rdBwLimitCfg.rdTagCnt       = 31U;
        ldcCfg->rdBwLimitCfg.rdMaxBurstLength = 1U;
    }

    return;
}

#ifdef __cplusplus
}
#endif

#endif /* CFG_LDC_H_ */

/** @} */
