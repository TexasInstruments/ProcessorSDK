/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2022
 *  All rights reserved.
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
 *  \file dss_dctrlApi.c
 *
 *  \brief File containing the DSS Controller driver implementation.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/dss/dss.h>
#include <ti/drv/dss/src/drv/common/dss_evtMgr.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlDrv.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlIntr.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlPriv.h>
#include <ti/drv/dss/src/csl/dsi/csl_dsi.h>
#include <ti/drv/dss/src/csl/dphy/csl_dphy.h>
#include <dss_soc_priv.h>
#include <ti/drv/sciclient/sciclient.h>
#include <string.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* TODO: Only RGB24 input and output supported as of now,
   multipled by 3 for RGB24 */
#define DSITX_VID_DATA_TYPE                 (DSITX_VID_DATA_TYPE_RGB_24)
#define DSITX_VID_PIX_MODE                  (DSITX_VID_PIXEL_MODE_RGB_24)
#define BPP                                 (3U)

#define PLL_LOCK_REPEAT_COUNT               (10000U)
#define WAIT_FOR_LANES_ACTIVE_REPEAT_COUNT  (10000U)

#define DSI_INSTANCE_ID_0               ((uint32_t) 0x0U)
#if defined (SOC_J721S2) || defined (SOC_J784S4)
#define DSI_INSTANCE_ID_1               ((uint32_t) 0x1U)
#endif

#if defined (SOC_J721S2) || defined (SOC_J784S4)
#define DSI_INSTANCE_ID_MAX             ((uint32_t)DSI_INSTANCE_ID_1 + 1U)
#else
#define DSI_INSTANCE_ID_MAX             ((uint32_t)DSI_INSTANCE_ID_0 + 1U)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    DSITX_Config                cfgDsiTx;
    DSITX_SysReq                sysReqDsiTx;
    DSITX_PhyConfig             phyCfg;
    DSITX_DphyConfig            dphyCfg;
    DSITX_DsiLinkConfig         dsiLinkCfg;
    DSITX_DphyPwrRstConfig      dphyRstCfg;
    DSITX_DataPathConfig        datapathCfg;
    DSITX_PrivateData           privDsiTx;

    DSITX_VideoSize             videoSizeCfg;
    DSITX_VideoModeSettings     videoModeCfg;

    uint32_t                    dphyTxIpDiv;
    uint32_t                    dphyTxOpDiv;
    uint32_t                    dphyTxFbDiv;
    uint32_t                    dphyTxRate;
    uint32_t                    dphyCoreBase;

    CSL_wiz16b8m4cdtRegs_DPHYTX     *dphyRegs;
    CSL_wiz16b8m4cdtRegs_WIZ_CONFIG *dphyWizRegs;
    DSITX_Regs                 *topRegs;

} Dss_DctrlDSIDrvObj;

typedef struct
{
    uint32_t numInst;
    /**< Number of valid instance. */
    Dss_DctrlDSIDrvObj instObj[DSI_INSTANCE_ID_MAX];
    /**< DSI instance objects pointer. */
} Dss_DctrlDSICommonObj;

/**
 *  struct Dsitx_DphyRangeData
 *
 *  \brief This structure holds information about DSI Tx Range. Typically used
 *         for DPHY programming.
 *
 */
typedef struct
{
    uint32_t rangeMin;
    /**< Lower boundary of the range */
    uint32_t rangeMax;
    /**< Higher boundary of the range */
    uint32_t progVal;
    /**< Value to be programmed for given range */
} Dsitx_DphyRangeData;

/**
 *  struct Dss_DctrlvpDsiSupportedBlanks
 *
 *  \brief This structure holds information about VP and DSI2DP blanking parameters.
 *         Driver needs to have an additional set of blanking parameters for DSI2DP Bridge
 *         as bridge does not support the standard VESA parameters.
 *
 */
typedef struct
{
    Fvid2_ModeInfo vpBlankingParams;
    /**< Supported VP blanking params */
    Fvid2_ModeInfo dsi2DpBlankingParams;
    /**< DSI2DP bridge blanking params corresponding to VP blanking params */
} Dss_DctrlDsi2DpBridgeBlankingParams;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


/**
 *  Below ifdef __cplusplus is added so that C++ build passes without
 *  typecasting. This is because the prototype is build as C type
 *  whereas this file is build as CPP file. Hence we get C++ build error.
 *  Also if typecasting is used, then we get MisraC error Rule 11.1.
 */
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static Dss_DctrlDSICommonObj gDssDctrlDsiCommonObj;

/* This contains information of the PLL input divider value for DPHY
   rangeMin and rangeMax is in KHz */

static Dsitx_DphyRangeData gDsiTxIpDivInfo[] =
{
    {
        .rangeMin = 9600U,
        .rangeMax = 19200U,
        .progVal  = 1U,
    },
    {
        .rangeMin = 19200U,
        .rangeMax = 38400U,
        .progVal  = 2U,
    },
    {
        .rangeMin = 38400U,
        .rangeMax = 76800U,
        .progVal  = 4U,
    },
    {
        .rangeMin = 76800U,
        .rangeMax = 150000U,
        .progVal  = 8U,
    },
};

/* This contains information of the PLL output divider value for DPHY
   rangeMin and rangeMax is in Mbps */
static Dsitx_DphyRangeData gDsiTxOpDivInfo[] =
{
    {
        .rangeMin = 1250U,
        .rangeMax = 2500U,
        .progVal  = 1U,
    },
    {
        .rangeMin = 630U,
        .rangeMax = 1240U,
        .progVal  = 2U,
    },
    {
        .rangeMin = 320U,
        .rangeMax = 620U,
        .progVal  = 4U,
    },
    {
        .rangeMin = 160U,
        .rangeMax = 310U,
        .progVal  = 8U,
    },
    {
        .rangeMin = 80U,
        .rangeMax = 150U,
        .progVal  = 16U,
    },
};

/* This contains information of the PLL output divider value for DPHY
   rangeMin and rangeMax is in Mbps */
static Dsitx_DphyRangeData gDsiTxLaneSpeedBandInfo[] =
{
    {
        .rangeMin = 80U,
        .rangeMax = 100U,
        .progVal  = 0x0U,
    },
    {
        .rangeMin = 100U,
        .rangeMax = 120U,
        .progVal  = 0x1U,
    },
    {
        .rangeMin = 120U,
        .rangeMax = 160U,
        .progVal  = 0x2U,
    },
    {
        .rangeMin = 160U,
        .rangeMax = 200U,
        .progVal  = 0x3U,
    },
    {
        .rangeMin = 200U,
        .rangeMax = 240U,
        .progVal  = 0x4U,
    },
    {
        .rangeMin = 240U,
        .rangeMax = 320U,
        .progVal  = 0x5U,
    },
    {
        .rangeMin = 320U,
        .rangeMax = 390U,
        .progVal  = 0x6U,
    },
    {
        .rangeMin = 390U,
        .rangeMax = 450U,
        .progVal  = 0x7U,
    },
    {
        .rangeMin = 450U,
        .rangeMax = 510U,
        .progVal  = 0x8U,
    },
    {
        .rangeMin = 510U,
        .rangeMax = 560U,
        .progVal  = 0x9U,
    },
    {
        .rangeMin = 560U,
        .rangeMax = 640U,
        .progVal  = 0xAU,
    },
    {
        .rangeMin = 640U,
        .rangeMax = 690U,
        .progVal  = 0xBU,
    },
    {
        .rangeMin = 690U,
        .rangeMax = 770U,
        .progVal  = 0xCU,
    },
    {
        .rangeMin = 770U,
        .rangeMax = 870U,
        .progVal  = 0xDU,
    },
    {
        .rangeMin = 870U,
        .rangeMax = 950U,
        .progVal  = 0xE,
    },
    {
        .rangeMin = 950U,
        .rangeMax = 1000U,
        .progVal  = 0xFU,
    },
    {
        .rangeMin = 1000U,
        .rangeMax = 1200U,
        .progVal  = 0x10U,
    },
    {
        .rangeMin = 1200U,
        .rangeMax = 1400U,
        .progVal  = 0x11U,
    },
    {
        .rangeMin = 1400U,
        .rangeMax = 1600U,
        .progVal  = 0x12U,
    },
    {
        .rangeMin = 1600U,
        .rangeMax = 1800U,
        .progVal  = 0x13U,
    },
    {
        .rangeMin = 1800U,
        .rangeMax = 2000U,
        .progVal  = 0x14U,
    },
    {
        .rangeMin = 2000U,
        .rangeMax = 2200U,
        .progVal  = 0x15U,
    },
    {
        .rangeMin = 2200U,
        .rangeMax = 2500U,
        .progVal  = 0x16U,
    },
};

/* This array contains information about what DSI blanking should be programmed for what supported VP Blanking */
static Dss_DctrlDsi2DpBridgeBlankingParams supportedBlanks[] =
{
    {
        .vpBlankingParams = {
            .standard = FVID2_STD_CUSTOM,
            .scanFormat = FVID2_SF_PROGRESSIVE,
            .fps = 60U,
            .width = 1280U,
            .height = 720U,
            .pixelClock = 64000000U,
            .hFrontPorch = 48U,
            .hBackPorch = 80U,
            .hSyncLen = 32U,
            .vFrontPorch = 4U,
            .vBackPorch = 12U,
            .vSyncLen = 5U
        },
        .dsi2DpBlankingParams = {
            .standard = FVID2_STD_CUSTOM,
            .scanFormat = FVID2_SF_PROGRESSIVE,
            .fps = 60U,
            .width = 1280U,
            .height = 720U,
            .pixelClock = 64000000U,
            .hFrontPorch = 48U,
            .hBackPorch = 80U,
            .hSyncLen = 32U,
            .vFrontPorch = 4U,
            .vBackPorch = 12U,
            .vSyncLen = 5U
        }
    },
    {
        .vpBlankingParams = {
            .standard = FVID2_STD_CUSTOM,
            .scanFormat = FVID2_SF_PROGRESSIVE,
            .fps = 60U,
            .width = 1920U,
            .height = 1080U,
            .pixelClock = 158400000U,
            .hFrontPorch = 60U,
            .hBackPorch = 70U,
            .hSyncLen = 62U,
            .vFrontPorch = 55U,
            .vBackPorch = 60U,
            .vSyncLen = 55U
        },
        .dsi2DpBlankingParams = {
            .standard = FVID2_STD_CUSTOM,
            .scanFormat = FVID2_SF_PROGRESSIVE,
            .fps = 60U,
            .width = 1920U,
            .height = 1080U,
            .pixelClock = 158400000U,
            .hFrontPorch = 60U,
            .hBackPorch = 70U,
            .hSyncLen = 62U,
            .vFrontPorch = 55U,
            .vBackPorch = 60U,
            .vSyncLen = 55U
        }
    },

};

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

static void dssDctrlSetDSIInCtrlMod(uint32_t instId);
static uint32_t dssDctrlInitPhyConfig(Dss_DctrlDSIDrvObj *drvObj,
    DSITX_PrivateData* pD, DSITX_Config* cfg);
static uint32_t dssDctrlSetDphyConfiguration(Dss_DctrlDSIDrvObj *dsiObj,
    DSITX_PrivateData *pD);
static int32_t dssDctrlSetClockConfig(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlWaitForLock(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlSetDphyPowerAndReset(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlInitDsiLinkConfig(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlSetVideoConfig(Dss_DctrlDSIDrvObj *dsiObj,
    const Fvid2_ModeInfo *mInfo, uint32_t connectedTo);
static int32_t dssDctrlGetVideoModeAndSizeConfig(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlSetVideoModeAndSizeConfig(Dss_DctrlDSIDrvObj *dsiObj);
static void dssDctrlUpdateVideoModeConfig(Dss_DctrlDSIDrvObj *dsiObj,
    const Fvid2_ModeInfo *mInfo);
static int32_t dssDctrlUpdateVideoSizeConfig(Dss_DctrlDSIDrvObj *dsiObj,
    const Fvid2_ModeInfo *mInfo, uint32_t connectedTo);
static int32_t dssDctrlSetVideoModeAndSizeConfig(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlEnableDsiLinkAndPath(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlEnableDsiLink(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlEnableDsiDatapath(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlWaitForLaneReady(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssdctrlCalcDsiParams(Dss_DctrlDSIDrvObj *dsiObj, const Dss_DctrlDsiParams *dsiPrms);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Dss_dctrlDrvInitDSI(void)
{
    Dss_DctrlDSICommonObj *dsiCommonObj;
    uint32_t instCnt;

    dsiCommonObj = &gDssDctrlDsiCommonObj;

    for (instCnt = 0U; instCnt < DSI_INSTANCE_ID_MAX; instCnt++)
    {
        Fvid2Utils_memset(&dsiCommonObj->instObj[instCnt].cfgDsiTx, 0, sizeof(DSITX_Config));
        Fvid2Utils_memset(&dsiCommonObj->instObj[instCnt].sysReqDsiTx, 0, sizeof(DSITX_SysReq));
        Fvid2Utils_memset(&dsiCommonObj->instObj[instCnt].privDsiTx, 0, sizeof(DSITX_PrivateData));
        if (instCnt == DSI_INSTANCE_ID_0)
        {
            dsiCommonObj->instObj[instCnt].cfgDsiTx.regBase = (DSITX_Regs*) CSL_DSS_DSI0_DSI_TOP_VBUSP_CFG_DSI_0_DSI_BASE;
            dsiCommonObj->instObj[instCnt].privDsiTx.regBase = (DSITX_Regs*) CSL_DSS_DSI0_DSI_TOP_VBUSP_CFG_DSI_0_DSI_BASE;
            dsiCommonObj->instObj[instCnt].topRegs = (DSITX_Regs*) CSL_DSS_DSI0_DSI_TOP_VBUSP_CFG_DSI_0_DSI_BASE;
            dsiCommonObj->instObj[instCnt].dphyRegs = (CSL_wiz16b8m4cdtRegs_DPHYTX *)CSL_DPHY_TX0_BASE;
            dsiCommonObj->instObj[instCnt].dphyWizRegs = (CSL_wiz16b8m4cdtRegs_WIZ_CONFIG *)
                (CSL_DPHY_TX0_BASE + CSL_WIZ16B8M4CDT_WIZ_CONFIG_MOD_VER);
            dsiCommonObj->instObj[instCnt].dphyCoreBase = CSL_DPHY_TX0_BASE;
        }
#if defined (SOC_J721S2) || defined (SOC_J784S4)
        else if (instCnt == DSI_INSTANCE_ID_1)
        {
            dsiCommonObj->instObj[instCnt].cfgDsiTx.regBase = (DSITX_Regs*) CSL_DSS_DSI1_DSI_TOP_VBUSP_CFG_DSI_0_DSI_BASE;
            dsiCommonObj->instObj[instCnt].privDsiTx.regBase = (DSITX_Regs*) CSL_DSS_DSI1_DSI_TOP_VBUSP_CFG_DSI_0_DSI_BASE;
            dsiCommonObj->instObj[instCnt].topRegs = (DSITX_Regs*) CSL_DSS_DSI1_DSI_TOP_VBUSP_CFG_DSI_0_DSI_BASE;
            dsiCommonObj->instObj[instCnt].dphyRegs = (CSL_wiz16b8m4cdtRegs_DPHYTX *)CSL_DPHY_TX1_BASE;
            dsiCommonObj->instObj[instCnt].dphyWizRegs = (CSL_wiz16b8m4cdtRegs_WIZ_CONFIG *)
                (CSL_DPHY_TX1_BASE + CSL_WIZ16B8M4CDT_WIZ_CONFIG_MOD_VER);
            dsiCommonObj->instObj[instCnt].dphyCoreBase = CSL_DPHY_TX1_BASE;
        }
#endif

        dsiCommonObj->instObj[instCnt].dphyTxIpDiv = 0x2U;
        dsiCommonObj->instObj[instCnt].dphyTxOpDiv = 0x2U;
        dsiCommonObj->instObj[instCnt].dphyTxFbDiv = 0x173U;
        dsiCommonObj->instObj[instCnt].dphyTxRate = 0x1CEU;
        dsiCommonObj->instObj[instCnt].cfgDsiTx.numOfLanes = 0x2U;
        dsiCommonObj->instObj[instCnt].privDsiTx.numOfLanes = 0x2U;
    }

}

int32_t Dss_dctrlDrvSetDSIParams(Dss_DctrlDrvInfo *drvInfo,
    const Dss_DctrlDsiParams *dsiPrms)
{
    int32_t status;
    Dss_DctrlDSICommonObj *dsiCommonObj;

    if (dsiPrms->instId < DSI_INSTANCE_ID_MAX)
    {
        drvInfo->dsiParams.instId = dsiPrms->instId;
        dsiCommonObj = &gDssDctrlDsiCommonObj;

        dsiCommonObj->instObj[dsiPrms->instId].cfgDsiTx.numOfLanes = dsiPrms->numOfLanes;
        dsiCommonObj->instObj[dsiPrms->instId].privDsiTx.numOfLanes = dsiPrms->numOfLanes;

        status = dssdctrlCalcDsiParams(&dsiCommonObj->instObj[dsiPrms->instId], dsiPrms);
    }
    else
    {
        GT_0trace(DssTrace,
                  GT_ERR,
                  "Invalid DSI Instance\r\n");
        status = FVID2_EINVALID_PARAMS;
    }
    if(FVID2_SOK == status)
    {
        /* Checks to see if the configuration (num of lanes) is valid */
        status = DSITX_Probe(&dsiCommonObj->instObj[dsiPrms->instId].cfgDsiTx, &dsiCommonObj->instObj[dsiPrms->instId].sysReqDsiTx);
    }

    if (CDN_EOK == (uint32_t)status)
    {
        /* Calculate lane parameters based on the input speed */
        status = FVID2_SOK;
    }
    else
    {
        GT_0trace(DssTrace,
                  GT_ERR,
                  "Invalid DSI Params \r\n");
        status = FVID2_EINVALID_PARAMS;
    }

    return (status);
}

static int32_t dssDctrlValidateSupportedVpModes(const Fvid2_ModeInfo *mInfo, Fvid2_ModeInfo **dsiMode)
{
    uint32_t supLen = sizeof(supportedBlanks)/sizeof(supportedBlanks[0]);
    uint32_t supInd = 0U;
    int32_t retVal = CSL_EFAIL;
    for (supInd = 0U; supInd < supLen; supInd++)
    {
        if (0 == memcmp(mInfo, &(supportedBlanks[supInd].vpBlankingParams), sizeof(Fvid2_ModeInfo)))
        {
            *dsiMode = &(supportedBlanks[supInd].dsi2DpBlankingParams);
            retVal = CSL_PASS;
            break;
        }
    }

    return retVal;
}


int32_t Dss_dctrlDrvEnableVideoDSI(Dss_DctrlDrvInfo *drvInfo, const Fvid2_ModeInfo *mInfo,
                                uint32_t hsyncPolarity, uint32_t vsyncPolarity, uint32_t connectedTo)
{
    int32_t status;
    Dss_DctrlDSICommonObj *dsiCommonObj;
    /* filled in Dss_dctrlDrvSetDSIParams */
    uint32_t instId = drvInfo->dsiParams.instId;

    dsiCommonObj = &gDssDctrlDsiCommonObj;

    dssDctrlSetDSIInCtrlMod(instId);

    /* Disables/clears interrupts, sets default status */
    status = DSITX_Init(&dsiCommonObj->instObj[instId].privDsiTx, &dsiCommonObj->instObj[instId].cfgDsiTx);

    if (CDN_EOK == status)
    {
        /* Initialize PHY configuration, number of lanes, continuous clock etc */
        status = dssDctrlInitPhyConfig(&dsiCommonObj->instObj[instId],
            &dsiCommonObj->instObj[instId].privDsiTx, &dsiCommonObj->instObj[instId].cfgDsiTx);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlSetDphyConfiguration(&dsiCommonObj->instObj[instId], &dsiCommonObj->instObj[instId].privDsiTx);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlSetClockConfig(&dsiCommonObj->instObj[instId]);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlWaitForLock(&dsiCommonObj->instObj[instId]);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlSetDphyPowerAndReset(&dsiCommonObj->instObj[instId]);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlInitDsiLinkConfig(&dsiCommonObj->instObj[instId]);
    }

    if (CDN_EOK == status)
    {
        DSITX_Start(&dsiCommonObj->instObj[instId].privDsiTx);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlSetVideoConfig(&dsiCommonObj->instObj[instId], mInfo, connectedTo);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlEnableDsiLinkAndPath(&dsiCommonObj->instObj[instId]);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlWaitForLaneReady(&dsiCommonObj->instObj[instId]);
    }

    if (CDN_EOK == status)
    {
        status = FVID2_SOK;
    }
    else
    {
        status = FVID2_EINVALID_PARAMS;
    }
    return (status);
}


/* ========================================================================== */
/*                  Internal/Private Function Definitions                     */
/* ========================================================================== */

static int32_t dssdctrlCalcDsiParams(Dss_DctrlDSIDrvObj *dsiObj, const Dss_DctrlDsiParams *dsiPrms)
{
    int32_t retVal = FVID2_SOK;
    uint32_t min, max;
    uint32_t idx = 0U;
    uint64_t tempResult = 0, refClkKHz = 0;

    /* Get speed band for given lane speed */
    for (idx = 0U ;
         idx < (sizeof(gDsiTxLaneSpeedBandInfo) / sizeof(Dsitx_DphyRangeData));
         idx++)
    {
        min = gDsiTxLaneSpeedBandInfo[idx].rangeMin * 1000;
        max = gDsiTxLaneSpeedBandInfo[idx].rangeMax * 1000;
        if ((dsiPrms->laneSpeedInKbps >= min) &&
            (dsiPrms->laneSpeedInKbps <= max))
        {
            break;
        }

    }
    if (idx < (sizeof(gDsiTxLaneSpeedBandInfo) / sizeof(Dsitx_DphyRangeData)))
    {
        dsiObj->dphyTxRate = (gDsiTxLaneSpeedBandInfo[idx].progVal) |
            (gDsiTxLaneSpeedBandInfo[idx].progVal << 5);
    }
    else
    {
        GT_0trace(DssTrace, GT_ERR, "Invalid dphy parameters \r\n");
        retVal = FVID2_EFAIL;
    }

    if (FVID2_SOK == retVal)
    {
        if (dsiPrms->instId == DSI_INSTANCE_ID_0)
        {
            Sciclient_pmGetModuleClkFreq(TISCI_DEV_DPHY_TX0,
                                         TISCI_DEV_DPHY_TX0_DPHY_REF_CLK,
                                         &refClkKHz,
                                         SCICLIENT_SERVICE_WAIT_FOREVER);
        }
#if defined (SOC_J721S2) || defined (SOC_J784S4)
        else if (dsiPrms->instId == DSI_INSTANCE_ID_1)
        {
            Sciclient_pmGetModuleClkFreq(TISCI_DEV_DPHY_TX1,
                                         TISCI_DEV_DPHY_TX1_DPHY_REF_CLK,
                                         &refClkKHz,
                                         SCICLIENT_SERVICE_WAIT_FOREVER);
        }
#endif
        refClkKHz = refClkKHz/1000;
        /* Calculate DPHY ipdiv - PLL input divider */
        if (FVID2_SOK == retVal)
        {
            for (idx = 0U ;
                 idx < (sizeof(gDsiTxIpDivInfo) / sizeof(Dsitx_DphyRangeData));
                 idx++)
            {
                if ((refClkKHz >= gDsiTxIpDivInfo[idx].rangeMin) &&
                    (refClkKHz < gDsiTxIpDivInfo[idx].rangeMax))
                {
                    break;
                }
            }
            if (idx < (sizeof(gDsiTxIpDivInfo) / sizeof(Dsitx_DphyRangeData)))
            {
                dsiObj->dphyTxIpDiv = gDsiTxIpDivInfo[idx].progVal;
            }
            else
            {
                retVal = FVID2_EFAIL;
            }
        }

        /* Calculate DPHY opdiv - PLL output divider */
        if (FVID2_SOK == retVal)
        {
            for (idx = 0U ;
                 idx < (sizeof(gDsiTxOpDivInfo) / sizeof(Dsitx_DphyRangeData));
                 idx++)
            {
                min = gDsiTxOpDivInfo[idx].rangeMin * 1000;
                max = gDsiTxOpDivInfo[idx].rangeMax * 1000;
                if ((dsiPrms->laneSpeedInKbps >= min) &&
                    (dsiPrms->laneSpeedInKbps <= max))
                {
                    break;
                }
            }
            if (idx < (sizeof(gDsiTxOpDivInfo) / sizeof(Dsitx_DphyRangeData)))
            {
                dsiObj->dphyTxOpDiv = gDsiTxOpDivInfo[idx].progVal;
            }
            else
            {
                retVal = FVID2_EFAIL;
            }
        }

        /* Calculate DPHY fbdiv - PLL feedback divider */
        if (FVID2_SOK == retVal)
        {
            tempResult = (((uint64_t)dsiPrms->laneSpeedInKbps) *
                          ((uint64_t)2U) *
                          ((uint64_t)dsiObj->dphyTxIpDiv) *
                          ((uint64_t)dsiObj->dphyTxOpDiv));
            if ((tempResult % refClkKHz) != 0U)
            {
                GT_0trace(DssTrace,
                          GT_ERR,
                          "Invalid lane speed provided, FbDiv value should turn out integral\r\n");
                retVal = FVID2_EINVALID_PARAMS;
            }
            tempResult /= (uint64_t)refClkKHz;

            dsiObj->dphyTxFbDiv = (uint32_t)tempResult;
        }
    }

    return retVal;
}

static void dssDctrlSetDSIInCtrlMod(uint32_t instId)
{
    /*
     * XXX push this things to some CSL kind of functions
     */
    if (instId == DSI_INSTANCE_ID_0)
    {
        /* Enable DPI0, no register overlays for Wrapper */
        CSL_REG32_WR(CSL_DSS_DSI0_DSI_WRAP_MMR_VBUSP_CFG_DSI_WRAP_BASE +
            CSL_DSI_WRAP_DPI_CONTROL, 1);

        /* Assumes the MMRs are unlocked for the 1st partition */

        /* Select DSITX as the source for DPHYTX */
        CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE +
            CSL_MAIN_CTRL_MMR_CFG0_DPHY_TX0_CTRL, 0x0);
    }
#if defined (SOC_J721S2) || defined (SOC_J784S4)
    else if (instId == DSI_INSTANCE_ID_1)
    {
        /* Enable DPI0, no register overlays for Wrapper */
        CSL_REG32_WR(CSL_DSS_DSI1_DSI_WRAP_MMR_VBUSP_CFG_DSI_WRAP_BASE +
            CSL_DSI_WRAP_DPI_CONTROL, 1);

        /* Assumes the MMRs are unlocked for the 1st partition */

        /* Select DSITX as the source for DPHYTX */
        CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE +
            CSL_MAIN_CTRL_MMR_CFG0_DPHY_TX1_CTRL, 0x0);
    }
#endif
}

static uint32_t dssDctrlInitPhyConfig(Dss_DctrlDSIDrvObj *drvObj,
    DSITX_PrivateData* pD, DSITX_Config* cfg)
{
    uint32_t status = CDN_EOK;

    status = DSITX_GetPhyConfig(pD, &drvObj->phyCfg);
    if (CDN_EOK == status)
    {
        drvObj->phyCfg.lane2Enabled =  (((cfg->numOfLanes) >  (1U)) ? BTRUE : BFALSE);
        drvObj->phyCfg.lane3Enabled =  (((cfg->numOfLanes) >  (2U)) ? BTRUE : BFALSE);
        drvObj->phyCfg.lane4Enabled =  (((cfg->numOfLanes) >  (3U)) ? BTRUE : BFALSE);
        drvObj->phyCfg.laneClkContinous = BTRUE;
        drvObj->phyCfg.waitBurstTime = 15; /* TODO: How to calculate waitBurstTime */
        status = DSITX_SetPhyConfig(pD, &drvObj->phyCfg);
    }
    return (status);
}

static uint32_t dssDctrlSetDphyConfiguration(Dss_DctrlDSIDrvObj *dsiObj,
    DSITX_PrivateData *pD)
{
    int32_t status;

    status = DSITX_GetDphyConfig(pD, &dsiObj->dphyCfg);
    if (CDN_EOK == status)
    {
        /* TODO: How to calculate these parameters */
        dsiObj->dphyCfg.clkDivisionRatio = 0xBU;
        dsiObj->dphyCfg.hstxTimeout = 0xAFFFU;
        dsiObj->dphyCfg.lprxTimeout = 0x3FFFFU;
        dsiObj->dphyCfg.clkLaneUlpTimeout = 0x105U;
        dsiObj->dphyCfg.dataLaneUlpTimeout = 0x1D5U;

        status = DSITX_SetDphyConfig(pD, &dsiObj->dphyCfg);
    }

    return status;
}

static int32_t dssDctrlSetClockConfig(Dss_DctrlDSIDrvObj *dsiObj)
{
    /* Set DPHY Wrapper PSM clock */
    /*
    ** The signal must be driven with a value such that the internal psm frequency of the divided psm clock is 1 MHz.PSM clock is 20MHz, so dividing by 20 which is ** 14 in hex
    */
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_WIZ_CONFIG_PSM_FREQ), 7, 0, 0x14);

    /* PWM control divider */
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_CMN0_CMN_DIG_TBIT10),
        9, 0, 0x00E /*0xE4*/);
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_CMN0_CMN_DIG_TBIT10),
        19, 10, 0x1FF /*0xC*/);

    /* pllcnt start value */
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_CMN0_CMN_DIG_TBIT7),
        15, 0, 0xC8);

    /* pllcnt lock threshold value */
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_CMN0_CMN_DIG_TBIT7),
        31, 16, 0x4);

    /* Enable CMN startup state machine */
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_CMN0_CMN_DIG_TBIT2),
        0, 0, 0x1);

    /* TX mode enable */
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_CMN0_CMN_DIG_TBIT2),
        9, 9, 0x1);

    /* Lane Reset */
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_WIZ_CONFIG_RST_CTRL),
        31, 31, 0x1);

    /* PLL Programming */
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_WIZ_CONFIG_PLL_CTRL),
        4, 0, dsiObj->dphyTxIpDiv);
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_WIZ_CONFIG_PLL_CTRL),
        13, 8, dsiObj->dphyTxOpDiv);
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_WIZ_CONFIG_PLL_CTRL),
        25, 16, dsiObj->dphyTxFbDiv);

    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_WIZ_CONFIG_PLL_CTRL),
        28, 28, 0x0);

    /* PHY Band ctrl - Data Rate */
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_PCS_TX_DIG_TBIT0),
        4, 0, dsiObj->dphyTxRate);
    CSL_FINSR(*(volatile uint32_t *)(
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_PCS_TX_DIG_TBIT0),
        9, 5, dsiObj->dphyTxRate);

    return (CDN_EOK);
}

static int32_t dssDctrlWaitForLock(Dss_DctrlDSIDrvObj *dsiObj)
{
    uint32_t    value;
    uint32_t    timeout;

    /* First, wait for PLL to get locked */
    timeout = 0U;
    do
    {
        value = *(volatile uint32_t *)(
            dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_WIZ_CONFIG_PLL_CTRL) &
            0x80000000U;

        if (PLL_LOCK_REPEAT_COUNT <= timeout)
        {
            break;
        }
        else
        {
            timeout ++;
        }
    } while(0x80000000U != value);

    /* Wait for CMN to be ready */
    timeout = 0U;
    do
    {
        value = *(volatile uint32_t *)(
            dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_WIZ_CONFIG_STATUS) &
            0x80000000U;
        if (PLL_LOCK_REPEAT_COUNT <= timeout)
        {
            break;
        }
        else
        {
            timeout ++;
        }
    } while(0x80000000U != value);

    return (CDN_EOK);
}

static int32_t dssDctrlSetDphyPowerAndReset(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status, cnt;

    /* DSI - D-PHY release c_rstb and d_rstb */
    Fvid2Utils_memset(&dsiObj->dphyRstCfg, 0, sizeof(DSITX_DphyPwrRstConfig));

    dsiObj->dphyRstCfg.dphyCRstb = 0x1U;

    for (cnt = 0; cnt < dsiObj->cfgDsiTx.numOfLanes; cnt ++)
    {
        dsiObj->dphyRstCfg.dphyDRstb |= (1 << cnt);
    }

    status = DSITX_SetDphyPwrAndRstCtrl(
        &dsiObj->privDsiTx, &dsiObj->dphyRstCfg);

    return (status);
}

static int32_t dssDctrlInitDsiLinkConfig(Dss_DctrlDSIDrvObj *dsiObj)
{
    uint32_t cnt;
    uint32_t status = CDN_EOK;

    status = DSITX_GetDsiLinkConfig(&dsiObj->privDsiTx, &dsiObj->dsiLinkCfg);
    if (CDN_EOK == status)
    {
        dsiObj->dsiLinkCfg.pllEnabled = BTRUE;
        dsiObj->dsiLinkCfg.clkLaneEnabled = BTRUE;

        for (cnt = 0U; cnt < (dsiObj->privDsiTx.numOfLanes); cnt ++)
        {
            dsiObj->dsiLinkCfg.datLaneEnabled[cnt] = BTRUE;
        }

        status = DSITX_SetDsiLinkConfig(&dsiObj->privDsiTx,
            &dsiObj->dsiLinkCfg);
    }

    return (status);
}

static int32_t dssDctrlSetVideoConfig(Dss_DctrlDSIDrvObj *dsiObj,
    const Fvid2_ModeInfo *mInfo, uint32_t connectedTo)
{
    int32_t status;

    status = dssDctrlGetVideoModeAndSizeConfig(dsiObj);

    if (CDN_EOK == status)
    {
        dssDctrlUpdateVideoModeConfig(dsiObj, mInfo);
        if (CSL_PASS != dssDctrlUpdateVideoSizeConfig(dsiObj, mInfo, connectedTo))
        {
            status = CDN_EINVAL;
        }
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlSetVideoModeAndSizeConfig(dsiObj);
    }

    return (status);
}

static int32_t dssDctrlGetVideoModeAndSizeConfig(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status;

    status = DSITX_GetVideoSize(&dsiObj->privDsiTx, &dsiObj->videoSizeCfg);

    if (CDN_EOK == status)
    {
        status = DSITX_GetVideoMode(&dsiObj->privDsiTx, &dsiObj->videoModeCfg);
    }

    return (status);
}

static int32_t dssDctrlSetVideoModeAndSizeConfig(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status;

    status = DSITX_SetVideoSize(&dsiObj->privDsiTx, &dsiObj->videoSizeCfg);

    if (CDN_EOK == status)
    {
        status = DSITX_SetVideoMode(&dsiObj->privDsiTx, &dsiObj->videoModeCfg);
    }

    return (status);
}

static void dssDctrlUpdateVideoModeConfig(Dss_DctrlDSIDrvObj *dsiObj,
    const Fvid2_ModeInfo *mInfo)
{
    uint32_t horzTotal;

    /* TODO: Only RGB24 input and output supported as of now */
    dsiObj->videoModeCfg.header = DSITX_VID_DATA_TYPE;
    dsiObj->videoModeCfg.vidPixelMode = DSITX_VID_PIX_MODE;
    dsiObj->videoModeCfg.syncPulseActive = BTRUE;
    dsiObj->videoModeCfg.syncPulseHorizontal = BTRUE;
    dsiObj->videoModeCfg.blkLineMode = DSITX_VID_BLK_MODE_BLANKING_PACKET;
    dsiObj->videoModeCfg.blkEolMode = DSITX_VID_BLK_MODE_BLANKING_PACKET;
    /* TODO: How to calculate this params */
    dsiObj->videoModeCfg.regWakeupTime = 0x1A8U; // any non-zero value, value from other tests
    dsiObj->videoModeCfg.ignoreMissVsync = BTRUE;

    horzTotal = mInfo->width + mInfo->hFrontPorch + mInfo->hBackPorch + mInfo->hSyncLen;

    dsiObj->videoModeCfg.regLineDuration =
        ((horzTotal * BPP) / dsiObj->cfgDsiTx.numOfLanes) -
        ((mInfo->hSyncLen * BPP) / dsiObj->cfgDsiTx.numOfLanes) -
        (4U / dsiObj->cfgDsiTx.numOfLanes);
}

static int32_t dssDctrlUpdateVideoSizeConfig(Dss_DctrlDSIDrvObj *dsiObj,
    const Fvid2_ModeInfo *mInfo, uint32_t connectedTo)
{
    uint32_t horzTotal;
    int32_t retVal = CSL_PASS;

    if (DSS_DSI_CONNECTION_MAX > connectedTo)
    {
        if (DSS_DSI_CONNECTION_DSI2DP_BRIDGE == connectedTo)
        {
            Fvid2_ModeInfo *dsiMode;
            retVal = dssDctrlValidateSupportedVpModes(mInfo,  &dsiMode);
            mInfo = dsiMode;
        }
    }
    else
    {
        retVal = CSL_EFAIL;
    }
    if(CSL_PASS == retVal)
    {
        horzTotal = mInfo->width + mInfo->hFrontPorch + mInfo->hBackPorch + mInfo->hSyncLen;

        dsiObj->videoSizeCfg.vact = mInfo->height;
        if (DSS_DSI_CONNECTION_DSI2DP_BRIDGE != connectedTo)
        {
            dsiObj->videoSizeCfg.vfp = 1;
        }
        else
        {
            dsiObj->videoSizeCfg.vfp = mInfo->vFrontPorch;
        }
        dsiObj->videoSizeCfg.vbp = mInfo->vBackPorch;
        dsiObj->videoSizeCfg.vsa = mInfo->vSyncLen;
        dsiObj->videoSizeCfg.hsa = (mInfo->hSyncLen * BPP) - 14U;
        dsiObj->videoSizeCfg.hbp = (mInfo->hBackPorch * BPP) - 12U;
        dsiObj->videoSizeCfg.rgb = mInfo->width * BPP;
        dsiObj->videoSizeCfg.hfp = (mInfo->hFrontPorch * BPP) - 6U;
        dsiObj->videoSizeCfg.blkLinePulsePacket = (horzTotal * BPP) - 20U - dsiObj->videoSizeCfg.hsa;
    }
    return retVal;
}

static int32_t dssDctrlEnableDsiLinkAndPath(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status;

    status = dssDctrlEnableDsiLink(dsiObj);
    if (CDN_EOK == status)
    {
        status = dssDctrlEnableDsiDatapath(dsiObj);
    }

    return (status);
}


static int32_t dssDctrlEnableDsiLink(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status;

    status = DSITX_GetDsiLinkConfig(&dsiObj->privDsiTx, &dsiObj->dsiLinkCfg);
    if (CDN_EOK == status)
    {
        dsiObj->dsiLinkCfg.if1Enabled = BFALSE;
        dsiObj->dsiLinkCfg.if2Enabled = BTRUE;
        dsiObj->dsiLinkCfg.if3Enabled = BFALSE;
        status = DSITX_SetDsiLinkConfig(&dsiObj->privDsiTx, &dsiObj->dsiLinkCfg);
    }

    return (status);
}

static int32_t dssDctrlEnableDsiDatapath(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status;

    status = DSITX_GetDataPathConfig(&dsiObj->privDsiTx, &dsiObj->datapathCfg);
    if (CDN_EOK == status)
    {
        dsiObj->datapathCfg.linkEnabled = BTRUE;
        dsiObj->datapathCfg.videoIfSelect = DSITX_VID_IF_SELECT_IF2;
        dsiObj->datapathCfg.videoStreamGenEnabled = BTRUE;
        status = DSITX_SetDataPathConfig(&dsiObj->privDsiTx, &dsiObj->datapathCfg);
    }

    if (CDN_EOK == status)
    {
        status = DSITX_WaitForPllLock(&dsiObj->privDsiTx, PLL_LOCK_REPEAT_COUNT);
    }

    if (CDN_EOK == status)
    {
        status = DSITX_CheckLanesState(&dsiObj->privDsiTx,
            WAIT_FOR_LANES_ACTIVE_REPEAT_COUNT);
    }

    return (status);
}

static int32_t dssDctrlWaitForLaneReady(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status = CDN_EOK;
    uint32_t timeout, laneCnt;
    uint32_t numOfLanes = dsiObj->cfgDsiTx.numOfLanes;
    uint32_t laneStatus, readData;
    bool done = BFALSE;
    uint32_t reg[] = {
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_CL_CNTRL_L,
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_DL_CTRL_L0,
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_DL_CTRL_L1,
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_ISO_LDD_PHY_ISO_DL_CTRL_L2,
        dsiObj->dphyCoreBase + CSL_WIZ16B8M4CDT_DPHYTX_ISO_LDD_PHY_ISO_DL_CTRL_L3
    };
    uint32_t mask[] = {
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_CL_CNTRL_L_ISO_LANE_READY_CL_L_MASK,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_DL_CTRL_L0_ISO_LANE_READY_DL_L_0_MASK,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_DL_CTRL_L1_ISO_LANE_READY_DL_L_1_MASK,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_LDD_PHY_ISO_DL_CTRL_L2_ISO_LANE_READY_DL_L_2_MASK,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_LDD_PHY_ISO_DL_CTRL_L3_ISO_LANE_READY_DL_L_3_MASK
    };
    uint32_t shift[] = {
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_CL_CNTRL_L_ISO_LANE_READY_CL_L_SHIFT,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_DL_CTRL_L0_ISO_LANE_READY_DL_L_0_SHIFT,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_DL_CTRL_L1_ISO_LANE_READY_DL_L_1_SHIFT,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_LDD_PHY_ISO_DL_CTRL_L2_ISO_LANE_READY_DL_L_2_SHIFT,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_LDD_PHY_ISO_DL_CTRL_L3_ISO_LANE_READY_DL_L_3_SHIFT
    };

    timeout = 0U;
    do
    {
        laneStatus = (CPS_REG_READ(reg[0]) & mask[0]) >> shift[0];
        for (laneCnt = 1U; laneCnt <= numOfLanes; laneCnt ++)
        {
            laneStatus = laneStatus &
                ((CPS_REG_READ(reg[laneCnt]) & mask[laneCnt]) >>
                shift[laneCnt]);
        }

        if (1U == laneStatus)
        {
            done = BTRUE;
        }

        if (WAIT_FOR_LANES_ACTIVE_REPEAT_COUNT == timeout)
        {
            status = CDN_ETIMEDOUT;
            done = BTRUE;
        }
        else
        {
            timeout ++;
        }
    } while (BFALSE == done);

    if (CDN_EOK == status)
    {
        done = BFALSE;
        timeout = 0U;
        /* DSI HS Ready Check */
        do
        {
            readData = CSL_REG32_RD (
                &dsiObj->cfgDsiTx.regBase->mctl_lane_sts);
            if (0x40000U == (readData & 0x40000U))
            {
                done = BTRUE;
            }

            if (WAIT_FOR_LANES_ACTIVE_REPEAT_COUNT == timeout)
            {
                status = CDN_ETIMEDOUT;
                done = BTRUE;
            }
            else
            {
                timeout ++;
            }
        } while (BFALSE == done);
    }

    return (status);
}

