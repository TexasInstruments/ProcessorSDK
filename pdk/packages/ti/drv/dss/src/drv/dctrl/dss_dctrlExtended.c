/*
 *  Copyright (c) Texas Instruments Incorporated 2025
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
 *  \file dss_dctrlExtended.c
 *
 *  \brief File containing the DSS Controller APIs for extended peripherals
 *         like eDP, DSI.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/dss/dss.h>
#include <ti/drv/dss/src/csl/dp/csl_dp.h>
#include <ti/drv/dss/src/csl/dp_sd0801/csl_dp_sd0801.h>
#include <ti/drv/dss/src/drv/common/dss_evtMgr.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlDrv.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlIntr.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlPriv.h>
#include <dss_soc_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

typedef enum
{
    DP_VIDEO_STATE_IDLE      = 0U,
    DP_VIDEO_STATE_REQUESTED = 1U,
    DP_VIDEO_STATE_RUNNING   = 2U,
} Dss_DctrlDisplayPortVideoState;

#define DP_PRIVATE_DATA_SIZE          (131072U)
#define DP_PHY_PRIVATE_DATA_SIZE      (1024U)
#define DP_INTR                       (64U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t hfp;
    uint32_t hsw;
    uint32_t hbp;
    uint32_t vfp;
    uint32_t vsw;
    uint32_t vbp;
    uint32_t pclk;
    uint32_t scanMode;
    uint32_t hsyncPol;
    uint32_t vsyncPol;
} Dss_DctrlDisplayPortVideoMode;

typedef struct
{
    DP_Config dpCfg;
    DP_SD0801_Config dpPhyCfg;
    DP_FirmwareImage dpFWImage;
    DP_PrivateData *dpPrivData;
    DP_SD0801_PrivateData *dpPhyPrivData;
    DP_Callbacks dpCbInfo;
    DP_UcpuClock dpClkInfo;
    DP_SourceDeviceCapabilities srcCaps;
    HwiP_Handle intrHandle;
    uint32_t isConnected;
    uint32_t isMstEnabled;
    uint32_t hpdInProgress;
    uint32_t hpdPending;
    uint32_t numStreams;
    uint32_t streamIds[DP_MAX_NUMBER_OF_STREAMS];
    Dss_DctrlDisplayPortVideoState videoState;
    Dss_DctrlDisplayPortVideoMode videoMode[DP_MAX_NUMBER_OF_STREAMS];
    uint32_t isHpdSupported;
    Dss_DctrlDpHpdCbFxn hpdCbFxn;
    void *hpdCbData;
} Dss_DctrlDisplayPortDrvObj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

extern uint8_t gDctrlDpDram[DCTRL_DP_DMEM_SIZE];
extern uint8_t gDctrlDpIram[DCTRL_DP_IMEM_SIZE];

static uint8_t gDssDctrlDisplayPortDpPrivateData[DP_PRIVATE_DATA_SIZE];
static uint8_t gDssDctrlDisplayPortDpPhyPrivateData[DP_PHY_PRIVATE_DATA_SIZE];

static Dss_DctrlDisplayPortDrvObj gDssDctrlDisplayPortDrvObj;

Fvid2_ModeInfo gDpStdModeInfo[] = {
    {FVID2_STD_1080P_60,       1920, 1080, FVID2_SF_PROGRESSIVE, 148500, 60,
     88, 148, 44, 4, 36, 5},
     {FVID2_STD_720P_60,       1280,  720, FVID2_SF_PROGRESSIVE,  74250, 60,
        110, 220, 40, 5, 20, 5}
};

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

static int32_t Dss_dctrlDrvInitDPTX(uint32_t isHpdSupported, uint32_t multilinkPhyType, uint32_t isMstEnabled);
static void Dss_dctrlDrvDpIntr(uintptr_t arg);
static int32_t Dss_dctrlDrvDpStartVideo(Dss_DctrlDisplayPortDrvObj *pObj);
static int32_t Dss_dctrlDrvDpStopVideo(Dss_DctrlDisplayPortDrvObj *pObj);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Dss_dctrlDrvDisableVideoDP(void)
{
    int32_t retVal = FVID2_SOK;
    uint32_t cookie;
    Dss_DctrlDisplayPortDrvObj *pObj = &gDssDctrlDisplayPortDrvObj;

    cookie = HwiP_disable();

    /*
     * We cannot be IDLE when this function is called.
     * Valid states are REQUESTED || RUNNING
     *
     * If state == REQUESTED, HPD never happened, and we
     * never went to RUNNING. So just go back to IDLE
     *
     * If state == RUNNING, we stop video and go to IDLE
     *
     * There is nothing we can do if stopVideo fails, so
     * dont bother
     */
    if((DP_VIDEO_STATE_REQUESTED == pObj->videoState) ||
    (DP_VIDEO_STATE_RUNNING == pObj->videoState))
    {
        if(DP_VIDEO_STATE_RUNNING == pObj->videoState)
        {
            Dss_dctrlDrvDpStopVideo(pObj);
        }
        pObj->videoState = DP_VIDEO_STATE_IDLE;
    }

    HwiP_restore(cookie);

    return retVal;
}

int32_t Dss_dctrlDrvEnableVideoDP(const Fvid2_ModeInfo *mInfo[DP_MAX_NUMBER_OF_STREAMS],
                                  uint32_t hsyncPolarity[DP_MAX_NUMBER_OF_STREAMS],
                                  uint32_t vsyncPolarity[DP_MAX_NUMBER_OF_STREAMS],
                                  uint32_t streamIds[DP_MAX_NUMBER_OF_STREAMS],
                                  uint32_t numStreams)
{
    int32_t retVal = FVID2_SOK;
    uint32_t cookie;
    Dss_DctrlDisplayPortDrvObj *pObj = &gDssDctrlDisplayPortDrvObj;
    uint32_t numStdModes, modeCnt;

    pObj->numStreams = numStreams;

    if(UFALSE == pObj->isHpdSupported)
    {
        retVal = Dss_dctrlDrvDetectDp();
    }

    cookie = HwiP_disable();

    if((FVID2_SOK == retVal) &&
       (UFALSE    == pObj->isHpdSupported))
    {
        pObj->isConnected = UTRUE;
    }

    /*
     * Copy the video mode parameters to be
     * used by subsequent videoStart calls
     *
     * XXX bring in EDID support so that we can validate
     * params passed by app, and when all parameters not
     * provided, make a good guess whether to continue or not
     */

    GT_1trace(DssTrace, GT_INFO, "numStreams = %d\n\r", numStreams);

    for (uint32_t streamCnt = 0U; streamCnt < numStreams; streamCnt++)
    {
        pObj->streamIds[streamCnt] = streamIds[streamCnt];
        if(FVID2_STD_CUSTOM != mInfo[streamCnt]->standard)
        {
            numStdModes = sizeof (gDpStdModeInfo) / sizeof (Fvid2_ModeInfo);
            for (modeCnt = 0U; modeCnt < numStdModes; modeCnt++)
            {
                if (mInfo[streamCnt]->standard == gDpStdModeInfo[modeCnt].standard)
                {
                    pObj->videoMode[streamCnt].width    = gDpStdModeInfo[modeCnt].width;
                    pObj->videoMode[streamCnt].height   = gDpStdModeInfo[modeCnt].height;
                    pObj->videoMode[streamCnt].hfp      = gDpStdModeInfo[modeCnt].hFrontPorch;
                    pObj->videoMode[streamCnt].hsw      = gDpStdModeInfo[modeCnt].hSyncLen;
                    pObj->videoMode[streamCnt].hbp      = gDpStdModeInfo[modeCnt].hBackPorch;
                    pObj->videoMode[streamCnt].vfp      = gDpStdModeInfo[modeCnt].vFrontPorch;
                    pObj->videoMode[streamCnt].vsw      = gDpStdModeInfo[modeCnt].vSyncLen;
                    pObj->videoMode[streamCnt].vbp      = gDpStdModeInfo[modeCnt].vBackPorch;
                    pObj->videoMode[streamCnt].pclk     = gDpStdModeInfo[modeCnt].pixelClock;
                    pObj->videoMode[streamCnt].scanMode = gDpStdModeInfo[modeCnt].scanFormat;
                    break;
                }
            }
            if (modeCnt == numStdModes)
            {
                retVal = FVID2_EFAIL;
            }
        }
        else
        {
            pObj->videoMode[streamCnt].width    = mInfo[streamCnt]->width;
            pObj->videoMode[streamCnt].height   = mInfo[streamCnt]->height;
            pObj->videoMode[streamCnt].hfp      = mInfo[streamCnt]->hFrontPorch;
            pObj->videoMode[streamCnt].hsw      = mInfo[streamCnt]->hSyncLen;
            pObj->videoMode[streamCnt].hbp      = mInfo[streamCnt]->hBackPorch;
            pObj->videoMode[streamCnt].vfp      = mInfo[streamCnt]->vFrontPorch;
            pObj->videoMode[streamCnt].vsw      = mInfo[streamCnt]->vSyncLen;
            pObj->videoMode[streamCnt].vbp      = mInfo[streamCnt]->vBackPorch;
            pObj->videoMode[streamCnt].pclk     = mInfo[streamCnt]->pixelClock;
            pObj->videoMode[streamCnt].scanMode = mInfo[streamCnt]->scanFormat;
        }

        pObj->videoMode[streamCnt].hsyncPol = hsyncPolarity[streamCnt];
        pObj->videoMode[streamCnt].vsyncPol = vsyncPolarity[streamCnt];
    }
    /*
     * We are supposed to be IDLE when this function is called
     *
     * If HPD didnt happen yet, we set to REQUESTED, HDP will
     * start video later if this flag is set, and then set us
     * to RUNNING
     *
     * If HPD happend, and LT was successful, we start video
     * and set to RUNNING
     *
     * If startVideo fails, we stay at REQUESTED. reason:
     * startVideo can fail because the connected monitor
     * does not support the timings provided. So we hope
     * that when another monitor is connected, we are at
     * REQUESTED, and HPD takes us to RUNNING
     *
     * XXX check the timings, match with EDID and print a trace
     * accordingly
     */
    if((FVID2_SOK == retVal) &&
    (DP_VIDEO_STATE_IDLE == pObj->videoState))
    {
        pObj->videoState = DP_VIDEO_STATE_REQUESTED;
        if(UTRUE == pObj->isConnected)
        {
            retVal = Dss_dctrlDrvDpStartVideo(pObj);
            if(FVID2_SOK == retVal)
            {
                pObj->videoState = DP_VIDEO_STATE_RUNNING;
            }
        }
    }

    HwiP_restore(cookie);

    return retVal;
}

int32_t Dss_dctrlDrvInitDp(uint32_t isHpdSupported, uint32_t multilinkPhyType, uint32_t isMstEnabled)
{
    int32_t retVal = FVID2_SOK;

    /*
     * XXX push this things to some CSL kind of functions
     */

#if defined(SOC_J721S2)
/* J721S2 does not have the HPD pin connected, so no need to do anything here. */
#elif defined (SOC_J721E)
    /* HPD Pin Mux */
    CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PADCONFIG0 + 0x1C4, 0x00040005);
#elif defined (SOC_J784S4) || defined (SOC_J742S2)
    CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PADCONFIG51 , 0x0004000C); // MUX-Mode-->12
    /* This HPD pin is mapped to J784S4_EVM on SPI0_CS0--> PADCONFIG_51  */
#endif

    

#if defined (SOC_J721S2)
    CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_EDP0_CTRL, 0x10000000);
    CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_SERDES0_LN0_CTRL, 0x2);
    CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_SERDES0_LN1_CTRL, 0x1);
    CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_SERDES0_LN2_CTRL, 0x0);
    CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_SERDES0_LN3_CTRL, 0x0);
   
    /* WIZ PHY */
    CSL_REG32_WR(ADDR_AFE + 0x408, 0x30000000U);
    CSL_REG32_WR(ADDR_AFE + 0x40C, 0x39000000U);
    CSL_REG32_WR(ADDR_AFE + 0x480, 0x70000000U);
    CSL_REG32_WR(ADDR_AFE + 0x4C0, 0x70000000U);
    CSL_REG32_WR(ADDR_AFE + 0x500, 0x70000000U);
    CSL_REG32_WR(ADDR_AFE + 0x540, 0x70000000U);
    CSL_REG32_WR(ADDR_AFE + 0x484, 0x10001U   );
    CSL_REG32_WR(ADDR_AFE + 0x4C4, 0x10001U   );
    CSL_REG32_WR(ADDR_AFE + 0x504, 0x10001U   );
    CSL_REG32_WR(ADDR_AFE + 0x544, 0x10001U   );
    CSL_REG32_WR(ADDR_AFE + 0x5FC, 0x00000U   );
#else
    CSL_REG32_WR(ADDR_AFE + 0x408, 0x30000000U);
#if defined (SOC_J721E)
    CSL_REG32_WR(ADDR_AFE + 0x40C, 0x39000000U);
#elif defined(SOC_J784S4) || defined (SOC_J742S2)
    CSL_REG32_WR(ADDR_AFE + 0x40C, 0x2A800000U); // SerDes changed to Ct3. This offset is the serdes wiz_config_serdes_rst register. The MMR definition has changed.
#endif
    CSL_REG32_WR(ADDR_AFE + 0x480, 0x70000000U);
    CSL_REG32_WR(ADDR_AFE + 0x4C0, 0x80000000U);
    CSL_REG32_WR(ADDR_AFE + 0x500, 0x80000000U);
    CSL_REG32_WR(ADDR_AFE + 0x540, 0x80000000U);
    CSL_REG32_WR(ADDR_AFE + 0x484, 0x10001U   );
    CSL_REG32_WR(ADDR_AFE + 0x4C4, 0x10001U   );
    CSL_REG32_WR(ADDR_AFE + 0x504, 0x10001U   );
    CSL_REG32_WR(ADDR_AFE + 0x544, 0x10001U   );
#endif

#if defined(SOC_J721S2) || defined (SOC_J721E)
    /* Select EDP PHY CLK source */
    CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_EDP_PHY0_CLKSEL, 0x0);
#elif defined(SOC_J784S4) || defined (SOC_J742S2)
    /* This is by default set to 0 for J784S4, still setting explicitly. */
    CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_SERDES4_CLKSEL, 0x0);
#endif
    /* Set DPTX_SRC_CFG, 0:vif0_en, 1:vif_1_en, 2:vif_2_en, 3: vif_3_en, 4: vif_0_sel, 
     * 5: vif_1_sel, 6: vif_2_sel, 7: vif_3_sel
     * For vif_i_sel we are setting the bit to 1 to select dpi2,3,4,5 viz vp1,2,3,4 
     * respectively, dpi0,1 are tied off as per clocking architecture.*/
    CSL_REG32_WR(CSL_DSS_EDP0_INTG_CFG_VP_BASE + CSL_DPTX_DPTX_SRC_CFG, 0xFF);

    retVal = Dss_dctrlDrvInitDPTX(isHpdSupported, multilinkPhyType, isMstEnabled);

    return retVal;
}

int32_t Dss_dctrlDrvRegisterHpdCb(const Dss_DctrlDpHpdCbParams *cbParams)
{
    int32_t retVal = FVID2_SOK;
    uint32_t cookie;
    Dss_DctrlDisplayPortDrvObj *pObj = &gDssDctrlDisplayPortDrvObj;

    if(NULL == cbParams)
    {
        retVal = FVID2_EFAIL;
    }

    cookie = HwiP_disable();

    if(FVID2_SOK == retVal)
    {
        pObj->hpdCbFxn = cbParams->hpdCbFxn;
        pObj->hpdCbData = cbParams->appData;
    }

    if((NULL != pObj->hpdCbFxn) &&
      (UTRUE == pObj->hpdPending))
    {
        pObj->hpdPending = UFALSE;
        pObj->hpdInProgress = UTRUE;
        pObj->hpdCbFxn(UTRUE, pObj->hpdCbData);
    }

    HwiP_restore(cookie);

    return retVal;
}

int32_t Dss_dctrlDrvProcessHpdDp(uint32_t hpdState)
{
    int32_t retVal = FVID2_SOK;
    uint32_t cookie, dpApiRet;
    uint32_t isConnected = UFALSE;
    Dss_DctrlDisplayPortDrvObj *pObj = &gDssDctrlDisplayPortDrvObj;
    DP_TrainingStatus trainingResult;

    if(UTRUE == hpdState)
    {
        dpApiRet = DP_LinkTraining(pObj->dpPrivData, &trainingResult);
        if ((CDN_EOK  == dpApiRet) &&
            (DP_LT_OK == trainingResult))
        {
            isConnected = UTRUE;
        }
    }

    /*
     * It is a little bit of unknown ATM whether all these
     * MBOX transactions to enable DP video takes sufficiently
     * insignificant time so that they can be safely called
     * inside a HWI disable/restore block.
     *
     * Keep them as is for the time being. We will see if
     * anyone comes back reporting issues!
     */
    cookie = HwiP_disable();
    pObj->isConnected = isConnected;

    /*
     * If we just connected, check if REQUESTED or RUNNING
     *
     * If RUNNING, then start video
     * If startVideo fails, go back to REQUESTED, so that we
     * do not do anything unexpected when stopVideo is called
     *
     * If REQUESTED, then start video, and if it passes,
     * set to RUNNING
     *
     * If we just disconnected, and we were previously RUNNING,
     * we set to REQUESTED and stop video
     */
    if(UTRUE == pObj->isConnected)
    {
        if((DP_VIDEO_STATE_REQUESTED == pObj->videoState) ||
        (DP_VIDEO_STATE_RUNNING == pObj->videoState))
        {
            retVal = Dss_dctrlDrvDpStartVideo(pObj);
            if(FVID2_SOK == retVal)
            {
                if(DP_VIDEO_STATE_REQUESTED == pObj->videoState)
                {
                    pObj->videoState = DP_VIDEO_STATE_RUNNING;
                }
            }
            else
            {
                if(DP_VIDEO_STATE_RUNNING == pObj->videoState)
                {
                    pObj->videoState = DP_VIDEO_STATE_REQUESTED;
                }
            }
        }
    }
    else
    {
        if(DP_VIDEO_STATE_RUNNING == pObj->videoState)
        {
            Dss_dctrlDrvDpStopVideo(pObj);
            pObj->videoState = DP_VIDEO_STATE_REQUESTED;
        }
    }

    pObj->hpdInProgress = UFALSE;
    HwiP_restore(cookie);

    return retVal;
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

int32_t Dss_dctrlDrvDetectDp(void)
{
    int32_t retVal = FVID2_SOK;
    uint32_t dpApiRet;
    bool hpdState;
    DP_TrainingStatus trainingResult;
    Dss_DctrlDisplayPortDrvObj *pObj = &gDssDctrlDisplayPortDrvObj;

    if(FVID2_SOK == retVal)
    {
        dpApiRet = DP_GetHpdStatus(pObj->dpPrivData, &hpdState);

#if defined (SOC_J721S2)
        /* HPD pin is not connected on j721s2_evm, so setting hpdDtate as true to avoid error. */
        hpdState = BTRUE;    
#endif        
        if((CDN_EOK != dpApiRet) ||
        (BTRUE != hpdState))
        {
            retVal = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == retVal)
    {
        dpApiRet = DP_LinkTraining(pObj->dpPrivData, &trainingResult);
        if ((CDN_EOK != dpApiRet) ||
        (DP_LT_OK != trainingResult))
        {
            retVal = FVID2_EFAIL;
        }
    }

    return retVal;
}

static void Dss_dctrlDrvDpIntr(uintptr_t arg)
{
    Dss_DctrlDisplayPortDrvObj *pObj = (Dss_DctrlDisplayPortDrvObj *)arg;
    uint32_t evt;
    uint32_t retVal;
    uint8_t hpdEvents = 0;

    retVal = DP_GetEvent(pObj->dpPrivData, &evt);

    if((CDN_EOK == retVal) &&
    (UFALSE != (evt & DP_TX_HPD_EVENT)))
    {
        retVal = DP_ReadHpdEvent(pObj->dpPrivData, &hpdEvents);
    }

    if((CDN_EOK == retVal) &&
       (UFALSE  == pObj->hpdInProgress))
    {

        if((0 != (hpdEvents & (uint8_t)DP_HPD_STATE)) &&
        (0 != (hpdEvents & (uint8_t)DP_HPD_TO_HIGH)))
        {
            if(NULL != pObj->hpdCbFxn)
            {
                pObj->hpdInProgress = UTRUE;
                pObj->hpdCbFxn(UTRUE, pObj->hpdCbData);
            }
            else
            {
                pObj->hpdPending = UTRUE;
            }
        }
        else if((0 == (hpdEvents & (uint8_t)DP_HPD_STATE)) &&
        (0 != (hpdEvents & (uint8_t)DP_HPD_TO_LOW)))
        {
            if(NULL != pObj->hpdCbFxn)
            {
                pObj->hpdInProgress = UTRUE;
                pObj->hpdCbFxn(UFALSE, pObj->hpdCbData);
            }
            else
            {
                pObj->hpdPending = UFALSE;
            }
        }
        if ((0U != (hpdEvents & DP_HPD_STATE)) && (0U != (hpdEvents & DP_HPD_PULSE)))
        {
            if (NULL != pObj->hpdCbFxn)
            {
                pObj->hpdInProgress = UTRUE;
                pObj->hpdCbFxn(UTRUE, pObj->hpdCbData);
            }
            else
            {
                pObj->hpdPending = UTRUE;
            }
        }
    }
}

static int32_t Dss_dctrlDrvDpStopVideo(Dss_DctrlDisplayPortDrvObj *pObj)
{
    /* XXX framer off */
    uint32_t dpApiRet = CDN_EOK;
    int32_t retVal = FVID2_SOK;
    if (pObj->isMstEnabled == UTRUE)
    {
        dpApiRet = DP_MstDisable(pObj->dpPrivData);
        if (CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_MstDisable\r\n");
            retVal = FVID2_EFAIL;
        }
    }
    else /* Single Stream Mode */
    {
        dpApiRet = DP_SetVideoSst(pObj->dpPrivData, BFALSE);
        if(CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_SetVideoSst\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    return retVal;
}

static int32_t Dss_dctrlDrvDpStartVideo(Dss_DctrlDisplayPortDrvObj *pObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t dpApiRet = CDN_EOK;
    DP_AudioVideoClkCfg clkCfg;
    uint32_t numStreams = pObj->numStreams;
    uint32_t isMstEnabled = pObj->isMstEnabled;
    uint32_t streamCnt;
    DP_VideoParameters videoParams[DP_MAX_NUMBER_OF_STREAMS];
    clkCfg.videoClockEnable   = BTRUE;
    clkCfg.audioClockEnable   = BTRUE;
    clkCfg.pktDataClockEnable = BTRUE;

    for (streamCnt = 0; streamCnt < numStreams; streamCnt++)
    {
        videoParams[streamCnt].bitsPerSubpixel    = 8;
        videoParams[streamCnt].pxEncFormat        = DP_PXENC_PXL_RGB;
        videoParams[streamCnt].stereoVidAttr      = DP_STEREO_VIDEO_LEFT;
        videoParams[streamCnt].btType             = DP_BT_601;
        videoParams[streamCnt].forceMiscIgnoreBit = BFALSE;
        videoParams[streamCnt].alignment          = DP_ALIGN_MSB;
        videoParams[streamCnt].dscEnable          = BFALSE;

        /* fill the ones the driver does not care about */
        videoParams[streamCnt].vicParams.vic   = 0U;
        videoParams[streamCnt].vicParams.hFreq = 0.0;
        videoParams[streamCnt].vicParams.vFreq = 0.0;
        videoParams[streamCnt].vicParams.vicR  = 0;
        videoParams[streamCnt].vicParams.vicPR = 0;

        /* and, now the actually important ones */
        videoParams[streamCnt].vicParams.hActive       = pObj->videoMode[streamCnt].width;
        videoParams[streamCnt].vicParams.hSync         = pObj->videoMode[streamCnt].hsw;
        videoParams[streamCnt].vicParams.hFrontPorch   = pObj->videoMode[streamCnt].hfp;
        videoParams[streamCnt].vicParams.hBackPorch    = pObj->videoMode[streamCnt].hbp;
        videoParams[streamCnt].vicParams.hBlank        = pObj->videoMode[streamCnt].hbp + pObj->videoMode[streamCnt].hfp + pObj->videoMode[streamCnt].hsw;
        videoParams[streamCnt].vicParams.hTotal        = videoParams[streamCnt].vicParams.hBlank + videoParams[streamCnt].vicParams.hActive;
        videoParams[streamCnt].vicParams.vActive       = pObj->videoMode[streamCnt].height;
        videoParams[streamCnt].vicParams.vSync         = pObj->videoMode[streamCnt].vsw;
        videoParams[streamCnt].vicParams.vFrontPorch   = pObj->videoMode[streamCnt].vfp;
        videoParams[streamCnt].vicParams.vBackPorch    = pObj->videoMode[streamCnt].vbp;
        videoParams[streamCnt].vicParams.vBlank        = pObj->videoMode[streamCnt].vbp + pObj->videoMode[streamCnt].vfp + pObj->videoMode[streamCnt].vsw;
        videoParams[streamCnt].vicParams.vTotal        = videoParams[streamCnt].vicParams.vBlank + videoParams[streamCnt].vicParams.vActive;
        videoParams[streamCnt].vicParams.pxlFreq       = pObj->videoMode[streamCnt].pclk / (float64_t)1000;
        videoParams[streamCnt].vicParams.scanMode      = ((FVID2_SF_PROGRESSIVE == pObj->videoMode[streamCnt].scanMode) ? DP_SM_PROGRESSIVE : DP_SM_INTERLACED);
        videoParams[streamCnt].vicParams.hSyncPolarity = ((FVID2_POL_HIGH == pObj->videoMode[streamCnt].hsyncPol) ? DP_SP_ACTIVE_HIGH : DP_SP_ACTIVE_LOW);
        videoParams[streamCnt].vicParams.vSyncPolarity = ((FVID2_POL_HIGH == pObj->videoMode[streamCnt].vsyncPol) ? DP_SP_ACTIVE_HIGH : DP_SP_ACTIVE_LOW);

        if(CDN_EOK == dpApiRet)
        {
            dpApiRet = DP_SetVic(pObj->dpPrivData, pObj->streamIds[streamCnt], &videoParams[streamCnt]);
            if(CDN_EOK != dpApiRet)
            {
                GT_0trace(DssTrace, GT_ERR, "error : DP_SetVic\r\n");
                retVal = FVID2_EFAIL;
            }
        }
    }

    if(CDN_EOK == dpApiRet)
    {
        dpApiRet = DP_SetFramerEnable(pObj->dpPrivData, BTRUE);
        if(CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_SetFramerEnable\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    for (streamCnt = 0; streamCnt < numStreams; streamCnt++)
    {
        if(CDN_EOK == dpApiRet)
        {
            GT_1trace(DssTrace, GT_INFO, "Clock config for stream %d\n", pObj->streamIds[streamCnt]);
            dpApiRet = DP_SetAudioVideoClkCfg(pObj->dpPrivData, pObj->streamIds[streamCnt], &clkCfg);
            if(CDN_EOK != dpApiRet)
            {
                GT_0trace(DssTrace, GT_ERR, "error : DP_SetAudioVideoClkCfg\r\n");
                retVal = FVID2_EFAIL;
                break;
            }
        }    
    }

    if (UTRUE == isMstEnabled)
    {
        uint8_t sinkCount;
        DP_SinkDevice *sinkList[DP_MAX_NUMBER_OF_STREAMS];
        const DP_SinkDevice **sinkListPtr = (const DP_SinkDevice **)sinkList;
        DP_SinkDevice *sinkDevice;
        
        for (streamCnt = 0; streamCnt < numStreams; streamCnt++)
        {
            if(CDN_EOK == dpApiRet)
            {
                GT_1trace(DssTrace, GT_INFO, "Enable stream %d\n", pObj->streamIds[streamCnt]);
                dpApiRet = DP_MstStreamEnable(pObj->dpPrivData, pObj->streamIds[streamCnt]);
                if (CDN_EOK != dpApiRet)
                {
                    GT_0trace(DssTrace, GT_ERR, "error : DP_MstStreamEnable \r\n");
                    retVal = FVID2_EFAIL;
                    break;
                } 
            }
        }
        
        if (CDN_EOK == dpApiRet)
        {
            dpApiRet = DP_MstScanTopology(pObj->dpPrivData);
            if (CDN_EOK != dpApiRet)
            {
                GT_0trace(DssTrace, GT_ERR, "error : DP_MstScanTopology \r\n");
                retVal = FVID2_EFAIL;
            }
        }

        if (CDN_EOK == dpApiRet)
        {
            dpApiRet = DP_MstGetSinkCount(pObj->dpPrivData, &sinkCount);
            GT_1trace(DssTrace, GT_INFO, "sinkCount = %d\n", sinkCount);
            if (CDN_EOK != dpApiRet || sinkCount != numStreams)
            {
                GT_0trace(DssTrace, GT_ERR, "error : DP_MstGetSinkCount \r\n");
                retVal = FVID2_EFAIL;
            }
        }
        
        if (CDN_EOK == dpApiRet)
        {
            dpApiRet = DP_MstGetSinkList(pObj->dpPrivData, sinkListPtr);
            if (CDN_EOK != dpApiRet)
            {
                GT_0trace(DssTrace, GT_ERR, "error : DP_MstGetSinkList \r\n");
                retVal = FVID2_EFAIL;
            }
        }

        /* Allocate VCPI for each stream */
        
        for (streamCnt = 0; streamCnt < numStreams; streamCnt++)
        {
            sinkDevice = sinkList[streamCnt];
            
            if(CDN_EOK == dpApiRet)
            {
                GT_1trace(DssTrace, GT_INFO, "Allocate VCPI for stream %d\n", pObj->streamIds[streamCnt]);
                dpApiRet = DP_MstAllocateVcpi(pObj->dpPrivData, pObj->streamIds[streamCnt], sinkDevice);
                if (CDN_EOK != dpApiRet)
                {
                    GT_0trace(DssTrace, GT_ERR, "error : DP_MstAllocateVcpi \r\n");
                    retVal = FVID2_EFAIL;
                    break;
                }
            }
        }    
    }
    else
    {
        /* Single stream mode */
        if(CDN_EOK == dpApiRet)
        {
            dpApiRet = DP_SetVideoSst(pObj->dpPrivData, BTRUE);
            if(CDN_EOK != dpApiRet)
            {
                GT_0trace(DssTrace, GT_ERR, "error : DP_SetVideoSst\r\n");
                retVal = FVID2_EFAIL;
            }
        }
    }

    return retVal;
}

static int32_t Dss_dctrlDrvInitDPTX(uint32_t isHpdSupported, uint32_t multilinkPhyType, uint32_t isMstEnabled)
{
    Dss_DctrlDisplayPortDrvObj *pObj;
    uint32_t memReqDp, memReqDpPhy;
    HwiP_Params hwiParams;
    uint32_t dpApiRet;
    int32_t retVal = FVID2_SOK;

    pObj = &gDssDctrlDisplayPortDrvObj;

    pObj->dpCfg.regBase      = (struct MHDP_ApbRegs_s *)CSL_DSS_EDP0_V2A_CORE_VP_REGS_APB_BASE;
    pObj->dpCfg.regBaseSapb  = (struct MHDP_ApbRegs_s *)CSL_DSS_EDP0_V2A_S_CORE_VP_REGS_SAPB_BASE;
#if defined (SOC_J784S4) || defined (SOC_J742S2)
    pObj->dpPhyCfg.regBase   = (uint32_t*)CSL_WIZ16B8M4CT3_4_WIZ16B8M4CT3_BASE;
#else
    pObj->dpPhyCfg.regBase   = (uint32_t*)CSL_SERDES_10G0_BASE;
#endif
    pObj->dpPhyCfg.regBaseDp = (struct DP_Regs_s *)CSL_DSS_EDP0_V2A_CORE_VP_REGS_APB_BASE;

    pObj->dpFWImage.iMem     = gDctrlDpIram;
    pObj->dpFWImage.iMemSize = DCTRL_DP_IMEM_SIZE;
    pObj->dpFWImage.dMem     = gDctrlDpDram;
    pObj->dpFWImage.dMemSize = DCTRL_DP_DMEM_SIZE;

    pObj->dpCbInfo.event     = NULL;
    pObj->dpClkInfo.mhz      = 125;
#if defined (SOC_J721S2)
    pObj->srcCaps.maxLinkRate = DP_LINK_RATE_8_10;
    pObj->srcCaps.laneCount   = 2;
#else
    pObj->srcCaps.maxLinkRate = DP_LINK_RATE_8_10;
    pObj->srcCaps.laneCount   = 4;
#endif
    pObj->srcCaps.ssc               = BFALSE;
    pObj->srcCaps.scramblerDisable  = BFALSE;
    pObj->srcCaps.tps3              = BTRUE;
    pObj->srcCaps.tps4              = BTRUE;
    pObj->srcCaps.fastLinkTraining  = BFALSE;
    pObj->srcCaps.maxVoltageSwing   = 3;
    pObj->srcCaps.maxPreemphasis    = 2;
    pObj->srcCaps.forceVoltageSwing = BFALSE;
    pObj->srcCaps.forcePreemphasis  = BFALSE;
    pObj->srcCaps.laneMapping       = DP_LANE_MAPPING_SINGLE_REGULAR;
    pObj->srcCaps.controllersPerPhy = DP_SINGLE_CONTROLLER;

    pObj->isConnected               = UFALSE;
    pObj->videoState                = DP_VIDEO_STATE_IDLE;

    pObj->isHpdSupported            = isHpdSupported;

    pObj->hpdPending                = UFALSE;
    pObj->hpdInProgress             = UFALSE;
    /* By default set the number of streams to 1 i.e SST (Single Stream Transport)*/
    pObj->numStreams                = 1U;
    pObj->isMstEnabled              = isMstEnabled;

    if(FVID2_SOK == retVal)
    {
        dpApiRet = DP_Probe(&pObj->dpCfg, &memReqDp);
        if((CDN_EOK != dpApiRet) ||
        (DP_PRIVATE_DATA_SIZE < memReqDp))
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_Probe\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == retVal)
    {
        dpApiRet = DP_SD0801_Probe(&pObj->dpPhyCfg, &memReqDpPhy);
        if ((CDN_EOK != dpApiRet) ||
        (DP_PHY_PRIVATE_DATA_SIZE < memReqDpPhy))
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_SD0801_Probe\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == retVal)
    {
        pObj->dpPrivData = (DP_PrivateData *)(&gDssDctrlDisplayPortDpPrivateData[0]);
        pObj->dpPhyPrivData = (DP_SD0801_PrivateData *)(&gDssDctrlDisplayPortDpPhyPrivateData[0]);

        dpApiRet = DP_Init(pObj->dpPrivData, &pObj->dpCfg, &pObj->dpCbInfo);
        if (CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_Init\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == retVal)
    {
        dpApiRet = DP_SD0801_Init(pObj->dpPhyPrivData, &pObj->dpPhyCfg);
        if (CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_SD0801_Init\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == retVal)
    {
        dpApiRet = DP_SetPhyPd(pObj->dpPrivData, pObj->dpPhyPrivData);
        if (CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_SetPhyPd\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == retVal)
    {
        dpApiRet = DP_SetClock(pObj->dpPrivData, &pObj->dpClkInfo);
        if (CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_SetClock\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == retVal)
    {
        dpApiRet = DP_LoadFirmware(pObj->dpPrivData, &pObj->dpFWImage);
        if (CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_LoadFirmware\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == retVal)
    {
       dpApiRet = DP_StartUcpu(pObj->dpPrivData);
       if(CDN_EOK != dpApiRet)
       {
            GT_0trace(DssTrace, GT_ERR, "error : DP_StartUcpu\r\n");
            retVal = FVID2_EFAIL;
       }
    }

    if(FVID2_SOK == retVal)
    {
        dpApiRet = DP_WaitAlive(pObj->dpPrivData);
        if (CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_WaitAlive\r\n");
            retVal = FVID2_EFAIL;
        }
    }


    if(FVID2_SOK == retVal)
    {
        uint8_t mainCtrlResp;

        dpApiRet = DP_MainControl(pObj->dpPrivData, (uint8_t)BTRUE, &mainCtrlResp);
        if (CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_MainControl\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == retVal)
    {
        dpApiRet = DP_ConfigurePhyAuxCtrl(pObj->dpPrivData);
        if (CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_ConfigurePhyAuxCtrl\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == retVal && UTRUE == isMstEnabled)
    {
        retVal = DP_MstEnable(pObj->dpPrivData);
        if (CDN_EOK != retVal)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_MstEnable failed\n");
            retVal = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == retVal)
    {
#if defined (SOC_J721S2)
        if(multilinkPhyType == DSS_DP_MULTILINK_PHY_NONE)
        {
            dpApiRet = DP_ConfigurePhyStartUp(pObj->dpPrivData,
                    0x2,
                    pObj->srcCaps.laneCount,
                    pObj->srcCaps.maxLinkRate);
        }
        else
        {
            DP_MlPhyInstance dpPhyInst, otherPhyInst;

            dpPhyInst.mLane = 2;
            otherPhyInst.mLane = 0;
            dpPhyInst.phyType = DP_SD0801_PHY_TYPE_DP;
            if(multilinkPhyType == DSS_DP_MULTILINK_PHY_USB)
            {
                otherPhyInst.phyType = DP_SD0801_PHY_TYPE_USB;
            }
            else
            {
                otherPhyInst.phyType = DP_SD0801_PHY_TYPE_PCIE;
            }
            
            dpPhyInst.numLanes = 2;
            otherPhyInst.numLanes = 2;

            dpApiRet = DP_ConfigureMlPhyStartUp(pObj->dpPrivData,
                    &dpPhyInst,
                    pObj->srcCaps.maxLinkRate,
                    &otherPhyInst);
        }
#else          
        dpApiRet = DP_ConfigurePhyStartUp(pObj->dpPrivData,
                0x0,
                pObj->srcCaps.laneCount,
                pObj->srcCaps.maxLinkRate);
#endif                      
        if(CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_ConfigurePhyStartUp\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == retVal)
    {
        dpApiRet = DP_SetSourceCapabilities(pObj->dpPrivData, &pObj->srcCaps);
        if (CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_SetSourceCapabilities\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    if((FVID2_SOK == retVal) &&
       (UTRUE     == pObj->isHpdSupported))
    {
        dpApiRet = DP_Start(pObj->dpPrivData);
        if(CDN_EOK != dpApiRet)
        {
            GT_0trace(DssTrace, GT_ERR, "error : DP_Start\r\n");
            retVal = FVID2_EFAIL;
        }
    }

    if((FVID2_SOK == retVal) &&
       (UTRUE     == pObj->isHpdSupported))
    {
        HwiP_clearInterrupt(DP_INTR);

        HwiP_Params_init(&hwiParams);
        hwiParams.arg = (uintptr_t) pObj;

        pObj->intrHandle = HwiP_create(DP_INTR,
                &Dss_dctrlDrvDpIntr,
                &hwiParams);

        if(NULL == pObj->intrHandle)
        {
            GT_0trace(DssTrace,
                      GT_ERR,
                      "Cannot Register IRQ");
            retVal = FVID2_EALLOC;
        }
    }

    return retVal;
}
