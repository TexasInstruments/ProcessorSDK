/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 *  \file dss_m2m_test_cfg.h
 *
 *  \brief DSS Display M2M Test Configuration Header file.
 */

#ifndef DSS_DISPLAY_TEST_CFG_H_
#define DSS_DISPLAY_TEST_CFG_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdio.h>
#include "ti/osal/osal.h"
#include "ti/osal/TaskP.h"

#include <ti/board/board.h>
#include <ti/drv/dss/examples/utils/app_utils.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/csl/soc.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/dss/examples/utils/app_utils_prf.h>

#include <ti/drv/dss/dss.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/**< Application name */
#define APP_NAME                        "DSS_M2M_APP"

/**< Driver instance to use */
#define APP_DSS_M2M_INST                ((uint32_t)DSS_M2M_WB_PIPELINE_ID_0)

/**< Number of channels i.e number of driver instances to be opened */
#define APP_DSS_M2M_CH_NUM                           ((uint32_t)1U)

/**< Number of display pipelines to use */
#define APP_DSS_M2M_DSS_PIPE_NUM                     ((uint32_t)1U)

/**< ID of display pipeline to use */
#define APP_DSS_M2M_DSS_PIPE_ID                      (CSL_DSS_VID_PIPE_ID_VID1)

/**< ID of display overlay to use */
#define APP_DSS_M2M_DSS_OVERLAY_ID                   (CSL_DSS_OVERLAY_ID_1)

/**< Number of frames to convert */
#define APP_DSS_M2M_FRAMES_WB                      ((uint32_t)1000U)

/**< Buffer load address for runtime loading buffers */
#define APP_DSS_M2M_DDR_LOAD_ADDRESS               (0x82000000U)

/**< Input Frame Attribute: Format */
#define APP_DSS_M2M_IN_FRAME_FORMAT                     (FVID2_DF_YUV422I_YUYV)
/**< Input Frame Attribute: Width in pixels */
#define APP_DSS_M2M_IN_FRAME_WIDTH                      ((uint32_t)1920U)
/**< Input Frame Attribute: Height in pixels */
#define APP_DSS_M2M_IN_FRAME_HEIGHT                     ((uint32_t)1080U)
/**< Input Frame Attribute: Bytes per pixel */
#define APP_DSS_M2M_IN_FRAME_BPP                        ((uint32_t)2U)
/**< Input Frame Attribute: Pitch in bytes */
#define APP_DSS_M2M_IN_FRAME_PITCH                      ((uint32_t)\
                            (APP_DSS_M2M_IN_FRAME_WIDTH * APP_DSS_M2M_IN_FRAME_BPP))

/**< Output Frame Attribute: Format */
#define APP_DSS_M2M_OUT_FRAME_FORMAT                     (FVID2_DF_YUV420SP_UV)
/**< Output Frame Attribute: Width in pixels */
#define APP_DSS_M2M_OUT_FRAME_WIDTH                      ((uint32_t)1920U)
/**< Output Frame Attribute: Height in pixels */
#define APP_DSS_M2M_OUT_FRAME_HEIGHT                     ((uint32_t)1080U)
/**< Output Frame Attribute: Bytes per pixel */
#define APP_DSS_M2M_OUT_FRAME_BPP                        (1U)
/**< Output Frame Attribute: Pitch in bytes */
#define APP_DSS_M2M_OUT_FRAME_PITCH                      ((uint32_t)\
                            (APP_DSS_M2M_OUT_FRAME_WIDTH * APP_DSS_M2M_OUT_FRAME_BPP))

/* Print buffer character limit for prints- UART or CCS Console */
#define APP_PRINT_BUFFER_SIZE                   ((uint32_t)4000)

/** \brief Log enable for DSS M2M Sample application */
#define DssM2MAppTrace                       ((uint32_t) GT_INFO   |\
                                              (uint32_t) GT_TraceState_Enable)
/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
typedef struct M2MApp_AppObj_t M2MApp_AppObj;

/**
 *  \brief Display application test parameters.
 *  The test case execution happens based on values of this structure
 */
typedef struct
{
    uint32_t instIdx;
    /**< Instance index in application object */
    Fvid2_Handle drvHandle;
    /**< FVID2 display driver handle */
    uint32_t numPipe;
    /**< Number of Display pipe used */
    Dss_WbCreateParams createParams;
    /**< WB pipe create parameters */
    Dss_WbCreateStatus createStatus;
    /**< WB pipe create status */
    Fvid2_CbParams cbParams;
    /**< Callback parameters */
    Dss_WbStatus wbStatus;
    /**< WB pipe status */
    SemaphoreP_Handle syncSem;
    /**< Semaphore for ISR */
    Dss_WbPipeCfgParams wbCfg;
    /**< WB pipe configuration */
    CSL_DssWbPipeDmaCfg wbDmaCfg;
    /**< WB pipe DMA configuration */
    Dss_WbPipeMflagParams wbMflagCfg;
    /**< WB pipe MFlag configuration */
    CSL_DssCscCoeff wbCscCfg;
    /**< WB pipe CSC configuration */
    Dss_PipeCfgParams pipeCfg[DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE];
    /**< Display pipe configuration */
    Dss_PipeMflagParams mFlagCfg[DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE];
    /**< Display pipe MFlag configuration */
    Dss_PipeCscParams cscCfg[DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE];
    /**< Display pipe CSC configuration */
    Dss_DctrlOverlayParams ovrCfg;
    /**< Display Overlay configuration */
    Dss_DctrlOverlayLayerParams layerCfg;
    /**< Display Layer configuration */
    Dss_DctrlGlobalDssParams globalParams;
    /**< Display Global configuration */
   M2MApp_AppObj *appObj;
   /**< Reference to Application Object  */
   Fvid2_Frame inFrames[DSSM2M_NUM_PIPELINE_TO_USE_IN_M2M_MODE];
   /**< FVID2 Frames that will be used for capture. */
   Fvid2_Frame outFrames[1U];
   /**< FVID2 Frames that will be used for capture. */
} M2MApp_InstParams;

/**
 *  \brief Display application object.
 *  The test case execution happens based on values of this structure
 */
struct M2MApp_AppObj_t
{
    uint32_t numInst;
    /**< Number of driver instances to open */
    M2MApp_InstParams instParams[APP_DSS_M2M_CH_NUM];
    /**< Instance object */
    Dss_InitParams initParams;
    /**< DSS Initialization Parameters */
    uint32_t numFramesToConvert;
    /**< Number of frames to convert for a given configuration */
    volatile uint32_t numFramesWb;
   /**< Number of frames wrote-back */
};
/* ========================================================================== */
/*                  Internal/Private Function Declarations                   */
/* ========================================================================== */



/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void App_consolePrintf(const char *pcString, ...);

/* ========================================================================== */
/*                              Global Variables                              */
/* ========================================================================== */


/* ========================================================================== */
/*      Internal Function Declarations (Needed for other static inlines)      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSS_DISPLAY_TEST_CFG_H_ */

/* @} */
