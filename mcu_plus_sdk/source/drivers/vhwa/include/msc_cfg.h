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
 *  \ingroup  DRV_MSC_MODULE
 *  \defgroup DRV_MSC_MODULE_CFG MSC Configuration
 *            This is MSC Configuration file
 *
 *  @{
 */

/**
 *  \file msc_cfg.h
 *
 *  \brief  Defines the structures / control operations that could be used to
 *              configure / control MSC module
 */

#ifndef CFG_MSC_H_
#define CFG_MSC_H_

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
/** \brief Maximum number of scalar output supported in MSC */
#define MSC_MAX_OUTPUT                     (10U)

/** \brief Maximum number of filter tap supported in the scalar */
#define MSC_MAX_TAP                        (5U)

/** \brief Maximum number of single phase coefficients set */
#define MSC_MAX_SP_COEFF_SET               (2U)

/** \brief Maximum number of Multi phase coefficients set */
#define MSC_MAX_MP_COEFF_SET               (4U)

/**
 *  \anchor Msc_CoeffShift
 *  \name   MSC Coefficient shifts
 *  \brief  Macros for selecting coefficient shift amount
 *          The scalar uses 10bit signed filter coefficient. The
 *          precision of these coefficients can be changed by
 *          selecting number of bits for integer and fraction
 *          portion in the coefficients. This enum is used for configuring
 *          the precision of the 10-bit signed filter coefficients
 *          by selecting the amount of coefficient shift.
 *
 *  @{
 */
/** \brief Coefficient shift by 5bits, 5bit fraction */
#define MSC_COEFF_SHIFT_5       (5U)
/** \brief Coefficient shift by 6bits, 6bit fraction */
#define MSC_COEFF_SHIFT_6       (6U)
/** \brief Coefficient shift by 7bits, 7bit fraction */
#define MSC_COEFF_SHIFT_7       (7U)
/** \brief Coefficient shift by 8bits, 8bit fraction */
#define MSC_COEFF_SHIFT_8       (8U)
/** \brief Coefficient shift by 9bits, 9bit fraction */
#define MSC_COEFF_SHIFT_9       (9U)
/** @} */

/**
 *  \anchor Msc_MultiPhaseCoeffSel
 *  \name   MSC Multiphase Coefficient select
 *  \brief  Macros for selecting coefficient set for multiphase operation
 *          There are four sets for 5tap/32-phase coefficient set. This
 *          enum is used for selecting coefficient set for each scalar.
 *
 *          For 5tap/64-phase filter mode, only coefficients set 0 and
 *          2 are used.
 *
 *  @{
 */
/** \brief Coefficient Set 0 for 32phase coefficients */
#define MSC_MULTI_32PHASE_COEFF_SET_0       (0U)
/** \brief Coefficient Set 1 for 32phase coefficients */
#define MSC_MULTI_32PHASE_COEFF_SET_1       (1U)
/** \brief Coefficient Set 2 for 32phase coefficients */
#define MSC_MULTI_32PHASE_COEFF_SET_2       (2U)
/** \brief Coefficient Set 3 for 32phase coefficients */
#define MSC_MULTI_32PHASE_COEFF_SET_3       (3U)
/** \brief Coefficient Set 0 for 64phase coefficients */
#define MSC_MULTI_64PHASE_COEFF_SET_0       (0U)
/** \brief Coefficient Set 2 for 64phase coefficients */
#define MSC_MULTI_64PHASE_COEFF_SET_2       (2U)
/** @} */

/**
 *  \anchor Msc_TapSel
 *  \name   MSC Tap select
 *  \brief  Macros for selecting number of taps
 *          Scalar can be configured to use either 3tap,4tap or 5tap filter.
 *          This is used in the scalar input.
 *          Unused coefficients should be set to 0.
 *
 *  @{
 */

/** \brief Selects the three tap filter */
#define MSC_TAP_SEL_3TAPS               (0U)
/** \brief Selects the 4 tap filter */
#define MSC_TAP_SEL_4TAPS               (1U)
/** \brief Selects the 5 tap filter */
#define MSC_TAP_SEL_5TAPS               (2U)
 /** @} */

/**
 *  \anchor Msc_SinglePhaseCoeffSel
 *  \name   MSC Single Phase Coefficient select
 *  \brief  Enum for selecting coefficients set for single phase operation.
 *          There are two dedicated single phase coefficients.
 *          In addition to this, one of the entry from 5tap/32phase
 *          coefficient set 0 can be used for single phase coefficients.
 *
 *  @{
 */
/** \brief Use Dedicated Single phase coef-0 */
#define MSC_SINGLE_PHASE_SP_COEFF_0         (0U)
/** \brief Use Dedicated Single phase coef-1 */
#define MSC_SINGLE_PHASE_SP_COEFF_1         (1U)
/** \brief Use 0th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_0        (2U)
/** \brief Use 1th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_1        (3U)
/** \brief Use 2th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_2        (4U)
/** \brief Use 3th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_3        (5U)
/** \brief Use 4th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_4        (6U)
/** \brief Use 5th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_5        (7U)
/** \brief Use 6th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_6        (8U)
/** \brief Use 7th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_7        (9U)
/** \brief Use 8th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_8        (10U)
/** \brief Use 9th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_9        (11U)
/** \brief Use 10th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_10       (12U)
/** \brief Use 11th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_11       (13U)
/** \brief Use 12th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_12       (14U)
/** \brief Use 13th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_13       (15U)
/** \brief Use 14th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_14       (16U)
/** \brief Use 15th entry of multiphase coefficient */
#define MSC_SINGLE_PHASE_MP_COEFF0_15       (17U)
 /** @} */

/**
 *  \anchor Msc_PhaseMode
 *  \name   MSC Phase mode select
 *  \brief  Enum for selecting phase mode for scaling operation
 *          It could be either 64phase or 32phase.
 *          The same phase mode is used for horizontal and vertical direction.
 *
 *  @{
 */
#define MSC_PHASE_MODE_64PHASE      (0U)

#define MSC_PHASE_MODE_32PHASE      (1U)
/** @} */

/**
 *  \anchor Msc_FilterMode
 *  \name   MSC Filter mode select
 *  \brief  Enum for selecting either single phase or multi phase filter.
 *
 *  @{
 */
#define MSC_FILTER_MODE_SINGLE_PHASE        (0U)

#define MSC_FILTER_MODE_MULTI_PHASE         (1U)
/** @} */

/**
 *  \anchor Msc_ErrorEvents
 *  \name   MSC Error Events
 *  \brief  Enum to define MSC Error events, for which MSC
 *          callback could be registered.
 *
 *          Caution: These macro values are directly used by driver
 *          for enabling events.
 *  @{
 */
/**< \brief Error on Input VBUSM Read interface */
#define VHWA_MSC_VBUSM_RD_ERR                   (0x4U)
/**< \brief Error on SL2 VBSUM Write interface */
#define VHWA_MSC_SL2_WR_ERR                     (0x8U)
/**< \brief Watchdog Timeout Error for MSC0 */
#define VHWA_MSC0_WDTIMER_ERR                   (0x1000000U)
/**< \brief Watchdog Timeout Error for MSC1 */
#define VHWA_MSC1_WDTIMER_ERR                   (0x2000000U)

/**< \brief Unified macro for all MSC error events (excludes watchdog timer) */
#define VHWA_MSC_ALL_ERR_EVENTS (VHWA_MSC_VBUSM_RD_ERR | VHWA_MSC_SL2_WR_ERR)
/** @} */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 * \brief Prototype for the Error Event for MSC.
 *        The callback for the MSC error events can be registered
 *        using #Msc_ErrEventParams. One of the parameter in this
 *        is call back. Driver calls this callback when error occurs.
 *
 * \param handle                FVID2 driver handle, for which error
 *                              has occurred.
 * \param errEvents             Error Events occured,
 *                              Refer to \ref Msc_ErrorEvents for valid values.
 *
 * \return None.
 */
typedef void (*Msc_ErrEventCbFxn)(Fvid2_Handle handle, uint32_t errEvents,
    void *appData);

/**
 * \brief Prototype for the Watchdog Timeout Error Event for MSC.
 *        The callback for the MSC Watchdog Timeout error events can be 
 *        registered using #Msc_WdTimerErrEventParams. One of the parameter in this
 *        is call back. Driver calls this callback when error occurs.
 *
 * \param handle                FVID2 driver handle, for which error
 *                              has occurred.
 * \param errEvents             Error Events occured,
 *                              Refer to \ref Msc_ErrorEvents for valid values.
 *
 * \return None.
 */
typedef void (*Msc_WdTimerErrEventCbFxn)(Fvid2_Handle handle, uint32_t errEvents,
    void *appData);

/**
 * \brief Prototype for the config Register Memory Compare Event for MSC.
 *        Following DOF pattern for configuration register validation callback.
 *
 * \param handle                FVID2 driver handle
 * \param configRegPrms         Pointer to configuration register parameters
 *
 * \return FVID2_SOK on success, else error code.
 */
typedef int32_t (*Msc_ConfigRegCompareCbFxn)(Fvid2_Handle handle, void *configRegPrms);

/**
 *  struct Msc_ScConfig
 *  \brief Individual Scalar configuration
 */
typedef struct
{
    uint32_t                        enable;
    /**< Flag to enable/disable given scalar */

    Fvid2_CropConfig                inRoi;
    /**< Configuration parameters used to select partial image for scaling.
         Refer to #Fvid2_CropConfig
         cropStartX and cropStartY parameters are used for starting position in
         image for scaling and cropWidth & cropHeight provide the width & height
         of image from starting position to be scaled
         cropStartX + cropWidth <= Image Width
         cropStartY + cropHeight <= Image Height
         */

    uint32_t                        outWidth;
    /**< Output frame width */
    uint32_t                        outHeight;
    /**< Output frame height */
    uint32_t                        horzAccInit;
    /**< Horizontal accumulator init */
    uint32_t                        vertAccInit;
    /**< Vertical accumulator init */

    uint32_t                        filtMode;
    /**< Filter Mode, single phase or multi phase
         Refer \ref Msc_FilterMode */
    uint32_t                        phaseMode;
    /**< Phase mode, Refer \ref Msc_PhaseMode */
    uint32_t                        hsSpCoeffSel;
    /**< Select coefficient set for single phase mode for horizontal direction,
         used only if #filtMode is set to single phase mode,
         Refer \ref Msc_SinglePhaseCoeffSel */
    uint32_t                        vsSpCoeffSel;
    /**< Select coefficient set for single phase mode for vertical direction,
         used only if #filtMode is set to single phase mode
         Refer \ref Msc_SinglePhaseCoeffSel */
    uint32_t                        hsMpCoeffSel;
    /**< Select coefficient set for Multi phase mode for horizontal direction,
         used only if #filtMode is set to multi phase mode
         Refer \ref Msc_MultiPhaseCoeffSel */
    uint32_t                        vsMpCoeffSel;
    /**< Select coefficient set for Multi phase mode for vertical direction,
         used only if #filtMode is set to multi phase mode
         Refer \ref Msc_MultiPhaseCoeffSel */

    uint32_t                        coeffShift;
    /**< Coefficient Shift Size, Refer \ref Msc_CoeffShift */

    uint32_t                        isSignedData;
    /**< UTRUE: if type of input/output data is signed
         UFALSE: if input/output data is unsigned */
    uint32_t                        isEnableFiltSatMode;
    /**< UTRUE: output data is clipped to [-2048, 2047] followed
               by addition of 2048
         UFALSE: output data is clipped to [0, 4095] */
    uint32_t                isInterleaveFormat;
    /**< UTRUE: if the input is inteleaved, used for UV buffer
         UFALSE: if input is non-interleaved */
} Msc_ScConfig;

/**
 *  struct Msc_Config
 *  \brief Structure for complete MSC Configuration
 */
typedef struct
{
    uint32_t       tapSel;
    /**< Used to select the number of taps for scaling operation */
    Msc_ScConfig   scCfg[MSC_MAX_OUTPUT];
    /**< Individual Scalar configuration */
} Msc_Config;

/**
 *  struct Msc_ErrEventParams
 *  \brief Structure for error event parameters
 *         Used to register callback for the given set of events.
 */
typedef struct
{
    uint32_t                    errEvents;
    /**< bitmask of error events,
     *   Multiple error events can be registered with the same callback,
     *   Driver returns bitmask of events, which has occured,
     *   Refer to \ref Msc_ErrorEvents for valid values. */
    Msc_ErrEventCbFxn           cbFxn;
    /**< Callback function to be call on error events */
    void                       *appData;
    /**< private data */
} Msc_ErrEventParams;

/**
 *  struct Msc_WdTimerErrEventParams
 *  \brief WD Timer parameters
 *         Used to configure WD Timer parameters
 *         Must be called after SET_PARAMS ioctl
 *         Used with IOCTL_VHWA_M2M_MSC_CONFIG_WDTIMER ioctl
 */
typedef struct
{
    uint32_t                    wdTimeoutCycles;
    /**< Used to select timeout cycles
     *   Refer to #VHWA_WdTimeOutCycles for valid values */
    uint32_t                    wdTimerErrEvents;
    /**< bitmask of watchdog timer error events,
     *   VHWA_MSC0_WDTIMER_ERR and VHWA_MSC1_WDTIMER_ERR can be
     *   registered with the same callback,
     *   Driver returns bitmask of events, which has occured,
     *   Refer to \ref Msc_ErrorEvents for valid values. */
    Msc_WdTimerErrEventCbFxn    cbFxn;
    /**< Callback function to be call on error events */
    void                       *appData;
    /**< private data */
} Msc_WdTimerErrEventParams;

/**
 *  struct Msc_Coeff
 *  \brief Structure used for setting scalar coefficients.
 */
typedef struct
{
    int32_t        *spCoeffSet[MSC_MAX_SP_COEFF_SET];
    /**< Pointer to the array containing single phase coefficients for set 0,
         if set to null, coefficients will not be updated/changed,
         The size of this array must be #MSC_MAX_TAP */
    int32_t        *mpCoeffSet[MSC_MAX_MP_COEFF_SET];
    /**< Pointer to the array containing multi phase coefficients for set 0,
         if set to null, coefficients will not be updated/changed,
         This is used to set 32phase/5tap filter coefficients, so
         The size of this array must be #MSC_MAX_TAP * 32
         In case of 64phase/5tap, set 0/1 of 32 phase sets will be used as set
         0 of 64 phase and set 2/3 will similarly be used as set 2 of 64 phase
         */
} Msc_Coeff;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief This function should be used to initialize variable of type
 *          #Msc_Config.
 *
 *  \param mscCfg       A pointer of type #Msc_Config
 *
 *  \return             None
 */
static inline void Msc_configInit(Msc_Config *mscCfg);

/**
 *  \brief This function should be used to initialize variable of type
 *          #Msc_Coeff.
 *
 *  \param coeffCfg     A pointer of type #Msc_Coeff
 *
 *  \return             None
 */
static inline void Msc_coeffInit(Msc_Coeff *coeffCfg);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static inline void Msc_coeffInit(Msc_Coeff *coeffCfg)
{   /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> Rationale:

    The negative test framework and test applications are unable to reach this portion of the code. The coeffCfg is passed using control commands from the application; if these control commands are not called, this API will not be invoked.
    Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL != coeffCfg)
    /* LDRA_JUSTIFY_END */           

    {
        Fvid2Utils_memset(coeffCfg, 0, sizeof(Msc_Coeff));
    }
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>  Rationale:

    The negative test framework and test applications are unable to reach this portion of the code. The coeffCfg is passed using control commands from the application; if these control commands are not called, this API will not be invoked.
    Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    else

    {
        GT_0trace(VhwaMscTrace, GT_ERR,"Msc_coeffInit: Error - coeffCfg is null\n");
    }
        /* LDRA_JUSTIFY_END */           

}

static inline void Msc_ConfigInit(Msc_Config *mscCfg)
{
    uint32_t cnt;
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> Rationale:
    The negative test framework and test apps cannot reach this portion.

    mscCfg is passed using hObj  which is passed from Imaging kernel layer
    Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL != mscCfg)
    /* LDRA_JUSTIFY_END */           

    {
        mscCfg->tapSel = MSC_TAP_SEL_5TAPS;
        for (cnt = 0; cnt < MSC_MAX_OUTPUT; cnt++)
        {
            mscCfg->scCfg[cnt].enable = UFALSE;
            mscCfg->scCfg[cnt].outWidth = 0U;
            mscCfg->scCfg[cnt].outHeight = 0U;
            mscCfg->scCfg[cnt].horzAccInit = 0U;
            mscCfg->scCfg[cnt].vertAccInit = 0U;
            mscCfg->scCfg[cnt].filtMode = MSC_FILTER_MODE_SINGLE_PHASE;
            mscCfg->scCfg[cnt].phaseMode = MSC_PHASE_MODE_64PHASE;
            mscCfg->scCfg[cnt].hsSpCoeffSel = MSC_SINGLE_PHASE_SP_COEFF_0;
            mscCfg->scCfg[cnt].vsSpCoeffSel = MSC_SINGLE_PHASE_SP_COEFF_0;
            mscCfg->scCfg[cnt].hsMpCoeffSel = MSC_MULTI_64PHASE_COEFF_SET_0;
            mscCfg->scCfg[cnt].vsMpCoeffSel = MSC_MULTI_64PHASE_COEFF_SET_0;
            mscCfg->scCfg[cnt].coeffShift = MSC_COEFF_SHIFT_8;
            mscCfg->scCfg[cnt].isSignedData = UFALSE;
            mscCfg->scCfg[cnt].isEnableFiltSatMode = UFALSE;
            mscCfg->scCfg[cnt].inRoi.cropStartX = 0U;
            mscCfg->scCfg[cnt].inRoi.cropStartY = 0U;
            mscCfg->scCfg[cnt].inRoi.cropWidth = 0U;
            mscCfg->scCfg[cnt].inRoi.cropHeight = 0U;
        }
    }
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This failure case is out of scope for the imaging test framework.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    else
    {
        GT_0trace(VhwaMscTrace, GT_ERR, "Msc_ConfigInit: Error - mscCfg is null\n");
    }
/* LDRA_JUSTIFY_END */ 
}

#ifdef __cplusplus
}
#endif

#endif /** CFG_MSC_H_ */
 /** @} */
