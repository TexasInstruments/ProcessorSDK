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
 *  \ingroup  DRV_NF_MODULE
 *  \defgroup DRV_NF_MODULE_CFG NF Configuration
 *            This is NF Configuration file
 *
 *  @{
 */
/**
 *  \file nf_cfg.h
 *
 *  \brief  Defines the structures / control operations that could be used to
 *          configure / control NF of VPAC
 */

#ifndef NF_CFG_H_
#define NF_CFG_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_vpac_abstraction_layer.h>


#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \brief NF Lut Size in bytes for in Bi-lateral filter mode
 */
#define NF_BL_LUT_SIZE              (5U*256U)
/**
 *  \brief NF Lut Size in Words for in Bi-lateral filter mode
 */
#define NF_BL_LUT_WORDS             (NF_BL_LUT_SIZE/4U)
/**
 *  \brief NF Lut Size in bytes for in Generic filter mode
 */
#define NF_GEN_LUT_SIZE             (24U)
/**
 *  \brief NF Lut Size in Words for in Generic filter mode
 */
#define NF_GEN_LUT_WORDS            (NF_GEN_LUT_SIZE/2U)

/**
 *  \anchor Nf_ErrorEvents
 *  \name   NF Error Events
 *  \brief  Macro's to define NF Error events, for which NF
 *          callback could be registered.
 *
 *          Caution: These macro values are directly used by driver
 *          for enabling events.
 *  @{
 */
/**< \brief Error on NF VBUSM Read interface */
#define VHWA_NF_RD_ERR                         (0x400U)
/**< \brief Error on NF SL2 VBSUM Write interface */
#define VHWA_NF_WR_ERR                         (0x200U)
/**< \brief Macro to combine all NF error events */
#define VHWA_NF_ALL_ERR_EVENTS (VHWA_NF_RD_ERR | VHWA_NF_WR_ERR)
/**< \brief Watchdog Timeout Error for NF */
#define VHWA_NF_WDTIMER_ERR                   (0x4000000U)
/** @} */

/**
 *  \anchor Nf_FilterMode
 *  \name   Filter Modes
 *  \brief  Macro's to define different filter mode.
 *
 *  @{
 */
/**< \brief Use Bilateral filtering */
#define    NF_FILTER_MODE_BILATERAL             (0x0u)
/**< \brief Use Generic 2D filtering */
#define    NF_FILTER_MODE_GENERIC_2D_FILTER     (0x1u)
/** @} */
/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 * \brief Prototype for the Error Event for NF.
 *        The callback for the NF error events can be registered
 *        using #Nf_ErrEventParams. One of the parameter in this
 *        is call back. Driver calls this callback when error occurs.
 *
 * \param handle                FVID2 driver handle, for which error
 *                              has occurred.
 * \param errEvents             Error Events occured,
 *                              Refer to \ref Nf_ErrorEvents for valid values.
 *
 * \return None.
 */
typedef void (*Nf_ErrEventCbFxn)(Fvid2_Handle handle, uint32_t errEvents,
    void *appData);

/**
 * \brief Prototype for the Watchdog Timeout Error Event for NF.
 *        The callback for the NF Watchdog Timeout error events can be 
 *        registered using #Nf_WdTimerErrEventParams. One of the parameter in this
 *        is call back. Driver calls this callback when error occurs.
 *
 * \param handle                FVID2 driver handle, for which error
 *                              has occurred.
 * \param errEvents             Error Events occured,
 *                              Refer to \ref Nf_ErrorEvents for valid values.
 *
 * \return None.
 */
typedef void (*Nf_WdTimerErrEventCbFxn)(Fvid2_Handle handle, uint32_t errEvents,
    void *appData);

/**
 * \brief Prototype for the config Register Memory Compare Event for NF.
 *        The callback for the NF register memory compare events can be registered
 *        using #Nf_ConfigRegMemParams. One of the parameters in this
 *        is the callback. Driver calls this callback when the event occurs.
 *
 * \param handle                FVID2 driver handle, for which the event
 *                              has occurred.
 * \param configRegPrms         Pointer to configuration register parameters.
 *
 * \return int32_t              Status of the callback execution.
 */
typedef int32_t (*Nf_ConfigRegCompareCbFxn)(Fvid2_Handle handle, void *configRegPrms);

/**
 *  struct Nf_Config
 *  \brief Connfiguration parameters of NF.
 */
typedef struct
{
    uint32_t               filterMode;
    /**< Filter mode, 0= Bi-lateral filtering; 1= Generic 2D Filtering
     */
    uint32_t               tableMode;
    /**< Defines what controls the selection of the sub table.
     * 0 = Bi-lateral filtering Adaptive mode is Disabled,
     * 1 = Bi-lateral filtering Adaptive mode is Enabled
     * and the top left corner 4x4 avg is used to determine sub table
     */
    uint32_t               skipMode;
    /**< 0 - Disabled,
     *   1- It will start the skip on the 1st or even 5x5 pixel.
     *      Interleave Mode : then skip on the 1st set/odd set of U and V 5x5
     *   2- It will start the skip on the 2nd or odd 5x5 pixel.
     *      Interleave Mode : then skip on the 2nd set/odd set of U and V 5x5
     */
    uint32_t               interleaveMode;
    /**< 0 = Disabled, 1 = Enabled, When enabled it build the U and V 5x5 matrix
     * by demultiplex the data into 2 separate 5x5 for processing.
     * Ignored by the FVID2 driver,
     * FVID2 driver internally sets it based Luma or chroma format
     * This paramter is overwritten by the driver based on the input ddataFormat
     */
    int32_t                outputShift;
    /**< Signed 4 bit  value (24 is added before using it inside IP)
     */
    uint32_t               outputOffset;
    /**< unsigned offset value to added to output after shifting
     * and before clipping
     */
    uint32_t               numSubTables;
    /**< 0 : 1 table
     *   1 : 2 sub tables
     *   2 : 4 sub tables
     *   3 : 8 sub tables
     */
    uint32_t               subTableIdx;
    /**< (0 -7 ) Subtable to be statically.
     *   Only valid when adaptive_mode is disabled
     */
    int32_t                centralPixelWeight;
    /**< 8 bit unsigned in Bi-lateral filtering,
     *   9 bit signed in Generic 2D Filtering
     */
} Nf_Config;



/**
 *  struct Nf_WgtTableConfig
 *  \brief Weight table parameters of NF.
 */
typedef struct
{
    uint32_t              filterMode;
    /**< Filter mode, 0= Bi-lateral filtering; 1= Generic 2D Filtering
     */
    uint32_t               blFilterLut[NF_BL_LUT_SIZE];
    /**< Lookup table for Bi-lateral filtering 5*256 (1280 entries)
     */
    int32_t                genFilterCoeffs[NF_GEN_LUT_SIZE];
    /**< Lookup table for Generic 2D filtering (5x5-1) 24 entries
     */
} Nf_WgtTableConfig;

/**
 *  struct Nf_ErrEventParams
 *  \brief Structure for error event parameters
 *         Used to register callback for the given set of events.
 */
typedef struct
{
    uint32_t                    errEvents;
    /**< bitmask of error events,
     *   Multiple error events can be registered with the same callback,
     *   Driver returns bitmask of events, which has occured,
     *   Refer to \ref Nf_ErrorEvents for valid values. */
    Nf_ErrEventCbFxn           cbFxn;
    /**< Callback function to be call on error events */
    void                       *appData;
    /**< private data */
} Nf_ErrEventParams;
/**
 *  struct Nf_WdTimerErrEventParams
 *  \brief WD Timer parameters
 *         Used to configure WD Timer parameters
 *         Must be called after SET_PARAMS ioctl
 *         Used with IOCTL_VHWA_M2M_NF_CONFIG_WDTIMER ioctl
 */
typedef struct
{
    uint32_t                    wdTimeoutCycles;
    /**< Used to select timeout cycles
     *   Refer to #VHWA_WdTimeOutCycles for valid values */
    uint32_t                    wdTimerErrEvents;
    /**< Watchdog timer error event value,
     *   VHWA_NF_WDTIMER_ERR registered with this callback
     *   Refer to \ref Nf_ErrorEvents for valid value. */
    Nf_WdTimerErrEventCbFxn     cbFxn;
    /**< Callback function to be call on error events */
    void                       *appData;
    /**< private data */
} Nf_WdTimerErrEventParams;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief This function should be used to initialize variable of type
 *         #Nf_Config.
 *
 *  \param nfCfg   A pointer of type Nf_Config
 *  \return         None
 */
static inline void Nf_ConfigInit(Nf_Config *nfCfg);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static inline void Nf_ConfigInit(Nf_Config *nfCfg)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale:
    The negative test framework and test apps cannot reach this portion.
    nfCfg is a member of the struct pointed by nf_generic_obj. It is ensured that nf_generic_obj is not null. Therefore, no test configuration will result in nfCfg to be NULL.
    Effect on this Unit:
    If nfCfg is NULL, the initialization is skipped and the configuration structure remains uninitialized, which may lead to undefined behavior.
    <justification end> */  
    if (NULL != nfCfg)
    /* LDRA_JUSTIFY_END */
    {
        nfCfg->filterMode = NF_FILTER_MODE_BILATERAL;
        nfCfg->tableMode = 0u;
        nfCfg->skipMode = 0u;
        nfCfg->interleaveMode = 0u;
        nfCfg->outputShift = 0;
        nfCfg->outputOffset = 0u;
        nfCfg->numSubTables = 0u;
        nfCfg->subTableIdx = 0u;
        nfCfg->centralPixelWeight = 255;
    }
}



#ifdef __cplusplus
}
#endif

#endif /** NF_CFG_H_ */

/** @} */
