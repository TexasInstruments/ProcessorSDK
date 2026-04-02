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
 *
 */

/**
 *  \ingroup  DRV_SDE_MODULE
 *  \defgroup DRV_SDE_MODULE_CFG SDE Configuration
 *            This is SDE Configuration file
 *
 *  @{
 */
/**
 *  \file sde_cfg.h
 *
 *  \brief  Defines the structures / control operations that could be used to
 *          configure / control SDE of DMPAC
 */

#ifndef DMPAC_CFG_SDE_H_
#define DMPAC_CFG_SDE_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <string.h>
#include <vhwa_dmpac_abstraction_layer.h>


#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define DMPAC_SDE_NUM_SCORE_MAP          (8U)

#define DMPAC_SDE_MAX_HISTOGRAM_SIZE     (128u)

/**
 *  \anchor SDE_SupportedResolution
 *  \name   SDE Supported Resolution
 *  \brief  Maximum and minimum supported image resolution by SDE
 *
 *  @{
 */
#define SDE_MIN_IMAGE_WIDTH               (128U)
#define SDE_MIN_IMAGE_HEIGHT              (64U)
#define SDE_MAX_IMAGE_WIDTH               (2048U)
#define SDE_MAX_IMAGE_HEIGHT              (1024U)
/** @} */

/**
 *  \anchor SDE_DispSearchRange
 *  \name   SDE SR
 *  \brief  Disparity Search Range
 *
 *  @{
 */
#define SDE_SR_64                       (0U)
#define SDE_SR_128                      (1U)
#define SDE_SR_192                      (2U)
 /** @} */

/**
 *  \anchor Sde_ErrorEvents
 *  \name   SDE Error Events
 *  \brief  Macro's to define SDE Error events, for which SDE
 *          callback could be registered.
 *
 *          Caution: These macro values are directly used by driver
 *          for enabling events.
 *  @{
 */
/**< \brief Error on SDE VBUSM Read interface */
#define VHWA_SDE_RD_ERR                         (0x40000U)
/**< \brief Error on SDE SL2 VBSUM Write interface */
#define VHWA_SDE_WR_ERR                         (0x80000U)
/**< \brief Error on FOCO0 SL2 VBSUM Write interface */
#define VHWA_SDE_FOCO0_SL2_WR_ERR               (0x2000000U)
/**< \brief Error on FOCO0 VBUSM Read interface */
#define VHWA_SDE_FOCO0_VBUSM_RD_ERR             (0x1000000U)
/**< \brief Bitmask for all SDE Error Events */
#define VHWA_SDE_ALL_ERR_EVENTS                                                \
  (VHWA_SDE_RD_ERR | VHWA_SDE_WR_ERR | VHWA_SDE_FOCO0_SL2_WR_ERR |             \
   VHWA_SDE_FOCO0_VBUSM_RD_ERR)
/**< \brief Watchdog timer Error on SDE */
#define VHWA_SDE_WDTIMER_ERR                    (0x02U)
/**< \brief Watchdog timer Error on FOCO1 */
#define VHWA_SDE_FOCO1_WDTIMER_ERR              (0x08U)
/**< \brief Bitmask for all SDE Watchdog Timer Error Events */
#define VHWA_SDE_ALL_WDTIMER_ERR_EVENTS                                        \
  (VHWA_SDE_WDTIMER_ERR | VHWA_SDE_FOCO1_WDTIMER_ERR)
/** @} */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/**
 * \brief Prototype for the Error Event for SDE.
 *        The callback for the SDE error events can be registered
 *        using #Sde_ErrEventParams. One of the parameter in this
 *        is call back. Driver calls this callback when error occurs.
 *
 * \param handle                FVID2 driver handle, for which error
 *                              has occurred.
 * \param errEvents             Error Events occured,
 *                              Refer to \ref Sde_ErrorEvents for valid values.
 *
 * \return None.
 */
typedef void (*Sde_ErrEventCbFxn)(Fvid2_Handle handle, uint32_t errEvents,
    void *appData);

/**
 * \brief Prototype for the Watchdog Timeout Error Event for SDE.
 *        The callback for the SDE Watchdog Timeout error events can be
 *        registered using #Sde_WdTimerErrEventParams. One of the parameter in this
 *        is call back. Driver calls this callback when error occurs.
 *
 * \param handle                FVID2 driver handle, for which error
 *                              has occurred.
 * \param errEvents             Error Events occured,
 *                              Refer to \ref Sde_ErrorEvents for valid values.
 *
 * \return None.
 */
typedef void (*Sde_WdTimerErrEventCbFxn)(Fvid2_Handle handle, uint32_t errEvents,
    void *appData);

    /**
 * \brief Prototype for the config Register Memory Compare Event for SDE.
 *        The callback for the SDE register memory compare events can be registered
 *        using #Sde_ConfigRegMemParams. One of the parameters in this
 *        is the callback. Driver calls this callback when the event occurs.
 *
 * \param handle                FVID2 driver handle, for which the event
 *                              has occurred.
 * \param appData               Application-specific data pointer.
 *
 * \return None.
 */
typedef int32_t (*Sde_ConfigRegCompareCbFxn)(Fvid2_Handle handle, void *configRegPrms);

    /**
 *  struct Sde_Config
 *  \brief Connfiguration parameters of DMPAC SDE.
 */
typedef struct
{
  uint32_t enableSDE;
  /**<Set to 0x1 to enable
   * SDE engine.
   */
  uint32_t medianFilter;
  /**<Median filter enable bit. Set to 0x1 to enable median
   * filter processing.
   */
  uint32_t width;
  /**< Image Width to be processed.
   */
  uint32_t height;
  /**< Image height to be processed.
  */
  uint32_t minDisparity;
  /**<Configure minimum disparity (minDisp) to
   * be searched in pixels. 0: minDisp=0, 1:minDisp= -3.
  */
  uint32_t searchRange;
  /**<Configure disparity search range
   * Suported Vlaues : \ref SDE_DispSearchRange
   */
  uint32_t lrThreshold;
  /**< Left-right consistence check threshold in pixels
    *
    *  If lrConsistenceThreshold >= maxDisp-minDisp
    *  will disable Left-right consistence check
   */
  uint32_t enableTextureFilter;
  /**< Enable texture based filtering
   */
  uint32_t textureFilterThreshold;
  /**< Scaled texture threshold. Any pixel whose texture
   * metric is lower than txtthld is considered to be low texture.
   * It is specified as normalized texture threshold times 1024.
   * For instance, if txtthld = 204, the normalized texture threshold
   * is 204/1024 = 0.1992.
   */
  uint32_t penaltyP1;
  /**< SDE aggregation penalty P1. Optimization penalty
   * constant for small disparity change.
   */
  uint32_t penaltyP2;
  /**< SDE aggregation penalty P2. Optimization penalty
   * constant for large disparity change. p2 <= 192.
   * Any value greater than 192 will be treated as 192
   */
  uint32_t confScoreMap[DMPAC_SDE_NUM_SCORE_MAP];
  /**< SDE aggregation penalty P2. Optimization penalty
   * constant for large disparity change. p2 <= 192.
   * Any value greater than 192 will be treated as 192
   */
} Sde_Config;

/**
 *  struct Sde_ErrEventParams
 *  \brief Structure for error event parameters
 *         Used to register callback for the given set of events.
 */
typedef struct
{
    uint32_t                    errEvents;
    /**< bitmask of error events,
     *   Multiple error events can be registered with the same callback,
     *   Driver returns bitmask of events, which has occured,
     *   Refer to \ref Sde_ErrorEvents for valid values. */
    Sde_ErrEventCbFxn           cbFxn;
    /**< Callback function to be call on error events */
    void                       *appData;
    /**< private data */
} Sde_ErrEventParams;

/**
 *  struct Sde_WdTimerErrEventParams
 *  \brief WD Timer parameters
 *         Used to configure WD Timer parameters
 *         Must be called after SET_PARAMS ioctl
 *         Used with IOCTL_VHWA_M2M_SDE_CONFIG_WDTIMER ioctl
 */
typedef struct
{
    uint32_t                    wdTimeoutCycles;
    /**< Used to select timeout cycles for SDE
     *   Refer to #VHWA_WdTimeOutCycles for valid values */
    uint32_t                    focoWdTimeoutCycles;
    /**< Used to select timeout cycles for FOCO
     *   Refer to #VHWA_WdTimeOutCycles for valid values */
    uint32_t                    wdTimerErrEvents;
    /**< bitmask of watchdog timer error events,
     *   VHWA_SDE_WDTIMER_ERR and VHWA_SDE_FOCO1_WDTIMER_ERR
     *   watchdog timer error events
     *   can be registered with the same callback,
     *   Driver returns bitmask of events, which has occured,
     *   Refer to \ref Sde_ErrorEvents for valid values. */
    Sde_WdTimerErrEventCbFxn    cbFxn;
    /**< Callback function to be call on error events */
    void                       *appData;
    /**< private data */
} Sde_WdTimerErrEventParams;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 *  \brief This function should be used to initialize SDE configuration
 *
 *  \param sdeCfg   A pointer of type #Sde_Config
 *  \return         None
 */
static inline void Sde_ConfigInit(Sde_Config *sdeCfg);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static inline void Sde_ConfigInit(Sde_Config *sdeCfg)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */


    if (NULL != sdeCfg)
    /* LDRA_JUSTIFY_END */
    {
        (void)memset(sdeCfg, 0, sizeof(Sde_Config));
    }

    return;
}

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */



#ifdef __cplusplus
}
#endif

#endif /* DMPAC_CFG_SDE_H_ */

/** @} */
