/*
 *  Copyright (c) Texas Instruments Incorporated 2025
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
 *  \file dss_test.h
 *
 *  \brief DSS unit test header file that defines test structures and various macros.
 *
 */

#ifndef DSS_TEST_H_
#define DSS_TEST_H_

#include <ti/drv/dss/dss.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define DSS_TEST_NONE                (0U)
#define DSS_TEST_FLIP                (1U)
#define DSS_TEST_CROP                (2U)
#define DSS_TEST_MULTISYNC           (3U)
#define DSS_TEST_COMM_REG_1          (4U)
#define DSS_TEST_COLORBAR            (5U)
#define DSS_TEST_FREEZE_FRAME        (6U)
#define DSS_TEST_DP_TO_HDMI          (7U)

#define DSS_TEST_BUFFER_NOT_REQURIED (0U)
#define DSS_TEST_BUFFER_REQURIED	 (1U)

#define DSS_TEST_DSI                 (0U)
#define DSS_TEST_EDP                 (1U)
#define DSS_TEST_HDMI                (2U)
#define DSS_TEST_INTEFACE_MAX        (3U)

#define DSS_TEST_FPS_FAILURE         (0U)
#define DSS_TEST_UNKNOWN_FAILURE     (1U)

#define DSS_TEST_PRINT_BUFFER_SIZE   (500U)
#define DSS_TEST_MAX_FRAMES_PER_PIPE (2U)
#define DSS_TEST_MAX_TESTS           (18U)
#define DSS_TEST_RUN_COUNT           (500U)
#define DSS_TEST_DDR_LOAD_ADDRESS    (0x82000000U)

#define DSS_TEST_1080P_PIXEL_CLK     (148500000ULL) 
#define DSS_TEST_720P_PIXEL_CLK      (74250000U)
#define DSS_TEST_DSI_1080P_PIXEL_CLK (158400000ULL)
#define DSS_TEST_DSI_720P_PIXEL_CLK  (64000000U)

#define DSS_LCD_OUT_RES_1080P        (0U)
#define DSS_LCD_OUT_RES_720P         (1U)
#define DSS_LCD_OUT_RES_CUSTOM       (2U)

#define DSS_NUM_SAFETY_REGIONS       (4U)


/* Set only one of the below two macros to 1U based on your choice for IO and disable the other one */
#define DSS_TEST_UART_INPUT          (1U)
#define DSS_TEST_CIO_ENABLE          (0U)

#if defined (SOC_J721E)
#define DSS_DCTRL_NODE_DSI_DPI1     (0U)
#define DSS_DCTRL_NODE_DSI_DPI3     (0U)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 * \brief Structure containing DCTRL configuration
 */
typedef struct
{
    uint32_t                      dctrlDrvId;
    uint32_t                      dctrlInstId;

    Fvid2_Handle                  dctrlDrvHandle;
    Dss_DctrlPathInfo             pathInfoObj;
    Dss_DctrlVpParams             vpParamsObj;
    Dss_DctrlVpParams             syncVpParamsObj;
    Dss_DctrlOverlayParams        overlayParamsObj;
    Dss_DctrlOverlayLayerParams   layerParamsObj;
    Dss_DctrlVpSafetyChkParams    vpSafetyObj[CSL_DSS_VP_SAFETY_REGION_MAX];
    Dss_DctrlVpErrorStats         errorStatsObj;
    Dss_DctrlAdvVpParams          advVpParamsObj;
    Dss_DctrlAdvVpParams          syncAdvVpParamsObj;
    Dss_DctrlLcdBlankTimingParams lcdBlankTiminfObj;
    Dss_DctrlSyncLostCbParams     syncLostCbParamsObj;
    Dss_DctrlLineNumCbParams      lineNumCbParamsObj;
    Dss_DctrlGlobalDssParams      globalDssParamsObj;
    Dss_DctrlDsiParams            dsiParamsObj;
} DssDctrlTest_Cfg;

/**
 * \brief Structure containing Dss disp driver information.
 */
typedef struct
{
    uint32_t                    dssDispDrvId;
    uint32_t                    dssDispDrvInstId;
    Fvid2_Frame                 frames[DSS_TEST_MAX_FRAMES_PER_PIPE];

    uint32_t                    pipeNodeId;
    uint32_t                    pipeType;
    Fvid2_Handle                dispDrvHandle;
    Dss_DispCreateParams        createParamsObj;
    Dss_DispCreateStatus        createStatusObj;
    Dss_DispParams              dispParamsObj;
    Dss_DispBufPrgmCbParams     bufCbParamObj;
    Dss_DispPipePrgmCbParams    pipePrgrmCbObj;
    Dss_DispPipeMflagParams     mflagParamsObj;
    Dss_DispRtParams            rtParamsObj;
    Dss_DispPipeSafetyChkParams pipeSafetyObj;
    Dss_DispUnderFlowCbParams   underflowCbObj;
    Fvid2_CbParams              cbParamsObj;
    SemaphoreP_Handle           syncSem;
} DssTest_PipelineParams;

typedef struct
{
    uint32_t        hfp;
    uint32_t        vfp;
    uint32_t        hbp;
    uint32_t        vbp;
    uint32_t        hsync;
    uint32_t        vsync;
    uint64_t        pixelClk;
}DssTest_blankingParams;

typedef struct 
{
    uint32_t         outPort;
    uint32_t         lcdOutHeight;
    uint32_t         lcdOutWidth;
    uint64_t         pixelClk;
    uint32_t         overlayId;
    uint32_t         overlayNodeId;
    uint32_t         vpId;
    uint32_t         vpNodeId;
    uint32_t         syncVPId;
    uint32_t         syncVPNodeId;
    uint32_t         outNodeId;
    uint32_t         syncOutNodeId;
} DssTest_DctrlParamsInfo;

typedef struct
{
    uint32_t         bpp;
    uint32_t         inWidth;
    uint32_t         inHeight;
    /* pitch is calculated from inWidth and bpp basically */
    uint32_t         pitch[FVID2_MAX_PLANES];
    uint32_t         inScanFmt;
    uint32_t         outWidth;
    uint32_t         outHeight;
    uint32_t         scEnable;
    uint32_t         globalAlpha;
    uint32_t         preMultiplyAlpha;
    uint32_t         posx;
    uint32_t         posy;
    /* To segregate FVID2_DF_YUV420SP_UV ccsFormat i.e how many bits, and packed or non packed */
    uint32_t         ccsFormat;
} DssTest_PipeConfig;


/**
 * \brief Structure containing configuration of an individual test.
 */
typedef struct
{
    uint32_t         testId;
    char             *testDesc;
    uint32_t         feature;
    bool             bufferRequired;
    uint32_t         testResult;  // redundant across test list structure and here - Make sense for individual test only remove from below
    uint32_t         failReason;
    /* Dctrl specific params */
    uint32_t         lcdOutRes; // output resolution
    DssTest_blankingParams *blankingParams; /* required in case of custome blanking params */
    uint32_t         vpId;
    /* Pipe specific params */
    uint32_t         numPipes;
    uint32_t         pipeId[CSL_DSS_VID_PIPE_ID_MAX];
    uint32_t         inDataFmt[CSL_DSS_VID_PIPE_ID_MAX];
    DssTest_PipeConfig*  pipeConfig[CSL_DSS_VID_PIPE_ID_MAX];
} DssTest_TestObj;

typedef struct
{
    uint32_t safetyInterupt;
    /**< Flag to check interupt received */
    uint32_t vpId;
    /**< Video Port Id */
    uint32_t pipeId;
    /**< Pipe Id */
    uint32_t frameCount;
    /**< Frame Count */
    uint32_t safetyCheckMode;
    /**< Mode of operation of the safety check module */
    Dss_DctrlVpSafetyCbData cbData;
    /**< Safety Error callback data to be returned to application */
} Dss_safetyTestCbData;

/**
 * \brief Structure representing Test Suite containing all tests
 */
typedef struct
{
    const bool      enableTest;
    const char      *tcName;
    const uint32_t  iterationCnt;
    const uint32_t  numTests;
    DssTest_TestObj testObj[DSS_TEST_MAX_TESTS];
    int32_t         testResult; /* Use For the entire test list */
} DssTest_Params;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void DssTest_main(void);
void DssTest_displayTest(DssTest_TestObj *test);
void DssTest_print(const char *format, ...);
void DispApp_ErrorRegRead(void);
int32_t DispApp_cfgAdditionalDsiPeripherals(uint32_t outResolution);
void DssTest_wait(uint32_t waitMs);
int32_t DssTest_configureClock(DssTest_TestObj *testObject);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Common across all configs, populated based on some config params*/
DssDctrlTest_Cfg gDctrlCfg;
DssTest_DctrlParamsInfo gDctrlParamsInfo;
DssTest_DctrlParamsInfo *pDctrlParamsInfo;
uint32_t gIntfSelected;
Dss_InitParams gInitParams;
DssTest_PipelineParams gPipelineParams[CSL_DSS_VID_PIPE_ID_MAX];

extern Dss_DispPipeSafetyChkParams gDssPipelineSafetyParams[CSL_DSS_VID_PIPE_ID_MAX];
extern Dss_DctrlVpSafetyChkParams gDssVpSafetyParams[DSS_NUM_SAFETY_REGIONS];

extern DssTest_blankingParams gDsiBlankingParams720p;
extern DssTest_blankingParams gDsiBlankingParams1080p;
#ifdef __cplusplus
}
#endif

#endif /* DSS_TEST_H_ */