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
 *  \file dss_display_test.h
 *
 *  \brief DSS display Test Header file.
 */

#ifndef DSS_MST_TEST_H_
#define DSS_MST_TEST_H_
#endif 

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/dss/dss.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define DSS_MST_RUN_COUNT               ((uint32_t)2000U)
#define DSS_MST_MAX_FRAMES_PER_HANDLE   (2U)
#define DSS_MST_LOG                     UART_printf
#define DSS_MST_MAX_PRINT_BUFFER        (500)

/* Can be updated as per use case */
#define DSS_MST_NUM_STREAMS             (2U)
#define DSS_MST_NUM_STREAMS_MAX         (4U)
/* Can be updated as per pipe config used */
#define DSS_MST_NUM_PIPES_USED          (3U)

#define DSS_MST_1080P_PIXEL_CLK         (148500000ULL) 
#define DSS_MST_720P_PIXEL_CLK          (74250000U)

#define CLOCK1_RATE                    (DSS_MST_1080P_PIXEL_CLK)
#define CLOCK2_RATE                    (DSS_MST_720P_PIXEL_CLK)

#define CLOCK_ID_1 TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK
#define CLOCK_ID_2 TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief Display application test parameters.
 *  The test case execution happens based on values of this structure
 */
typedef struct
{
    uint32_t numTestPipes;
    /**< Number of pipes in test params */
    uint32_t bpp;
    /**< Number of bytes per pixel */
    uint32_t instId[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Driver instance id */
    uint32_t pipeId[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Pipe id */
    uint32_t pipeNodeId[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Pipe Node id */
    uint32_t pipeType[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Video pipe type */
    uint32_t inDataFmt[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Data format */
    uint32_t inWidth[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Input buffer resolution width in pixels */
    uint32_t inHeight[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Input buffer resolution height in lines */
    uint32_t pitch[CSL_DSS_VID_PIPE_ID_MAX][FVID2_MAX_PLANES];
    /**< Pitch of input buffer */
    uint32_t inScanFmt[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Scan format */
    uint32_t outWidth[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Output buffer resolution width in pixels */
    uint32_t outHeight[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Output buffer resolution height in lines */
    uint32_t scEnable[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Scaler enable */
    uint32_t globalAlpha[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Global Alpha value */
    uint32_t preMultiplyAlpha[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Pre-multiply Alpha value */
    uint32_t posx[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Input buffer position x. */
    uint32_t posy[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Input buffer position y. */
    uint32_t invalidPipeId[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Pipe id */
} DssMst_PipelineParams;

/**
 *  \brief Driver instance information.
 */
typedef struct
{
    uint32_t instId;
    /**< Instance ID */
    Dss_DispCreateParams createParams;
    /**< Create time parameters */
    Dss_DispCreateStatus createStatus;
    /**< Create status returned by driver during Fvid2_create() */
    Dss_DispParams dispParams;
    /**< DSS display parameters */
    Dss_DispPipeMflagParams mflagParams;
    /**< DSS mflag parameters */
    Fvid2_Handle drvHandle;
    /**< FVID2 display driver handle */
    Fvid2_CbParams cbParams;
    /**< Callback parameters */
    Fvid2_Frame frames[DSS_MST_MAX_FRAMES_PER_HANDLE];
    /**< FVID2 Frames that will be used for display */
    SemaphoreP_Handle syncSem;
    /**< Semaphore for ISR */
} DssMst_dispInstObj;

/**
 *  \brief Test application data structure.
 */
typedef struct
{
    DssMst_dispInstObj instObj[CSL_DSS_VID_PIPE_ID_MAX];
    /**< Display driver instance objects */
    Fvid2_Handle dctrlHandle;
    /**< DCTRL handle */
    Dss_InitParams initParams;
    /**< DSS Initialization Parameters */
    Dss_DctrlPathInfo dctrlPathInfo;
    /**< DSS Path Information */
    Dss_DctrlVpParams vpParams;
    /**< VP Params */
    Dss_DctrlVpParams syncVpParams;
    /**< VP Params for synchronised VP */
    Dss_DctrlOverlayParams overlayParams;
    /**< Overlay Params */
    Dss_DctrlOverlayLayerParams layerParams;
    /**< Layer Params */
    Dss_DctrlVpErrorStats errorStats;
    /**< Error Stats */
    Dss_DctrlAdvVpParams advVpParams;
    /**< Advance VP Params */
    Dss_DctrlAdvVpParams syncAdvVpParams;
    /**< Advance VP Params for Synchronised VP */
    Dss_DctrlGlobalDssParams globalDssParams;
    /**< Global DSS Params */
} DssMst_testConfig;

typedef struct
{
    uint32_t vpId;
    uint32_t overlayId;
    /* Using default standards from Fvid2 - 1080p, 720p*/
    uint32_t standard;
} DssMst_dctrlInfo;

/* ========================================================================== */
/*                  Internal/Private Function Declarations                   */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void DssMst_print(const char *format, ...);

/* ========================================================================== */
/*                              Global Variables                              */
/* ========================================================================== */

static const DssMst_PipelineParams gDssMstPipelineParams[DSS_MST_NUM_STREAMS_MAX]=
{
    {
        /* Number of Pipes */
        2U,
        /* bpp */
        4U,
        /* Instance Id */
        {
            DSS_DISP_INST_VID1,
            DSS_DISP_INST_VIDL1
        },
        /* Pipe Id */
        {
            CSL_DSS_VID_PIPE_ID_VID1,
            CSL_DSS_VID_PIPE_ID_VIDL1
        },
        /* Pipe Node Id */
        {
            DSS_DCTRL_NODE_VID1,
            DSS_DCTRL_NODE_VIDL1
        },
        /* Pipe Type */
        {
            CSL_DSS_VID_PIPE_TYPE_VID,
            CSL_DSS_VID_PIPE_TYPE_VIDL
        },
        /* Data format */
        {
            FVID2_DF_BGRA32_8888,
            FVID2_DF_BGRA32_8888
        },
        /* Input buffer width */
        {
            480U,
            480U
        },
        /* Input buffer height */
        {
            360U,
            360U
        },
        /* Pitch */
        {
            {
                480U*4U, 0U, 0U, 0U, 0U, 0U
            },
            {
                480U*4U, 0U, 0U, 0U, 0U, 0U
            }
        },
        /* Scan format */
        {
            FVID2_SF_PROGRESSIVE,
            FVID2_SF_PROGRESSIVE
        },
        /* Output buffer width */
        {
            720U,
            480U
        },
        /* Output buffer height */
        {
            540U,
            360U
        },
        /* Scaler enable */
        {
            UTRUE,
            UFALSE
        },
        /* Global Alpha */
        {
            0xFFU,
            0xFFU
        },
        /* Pre-multiply alpha */
        {
            UFALSE,
            UFALSE
        },
        /* X Position */
        {
            0U,
            1440U
        },
        /* Y position */
        {
            0U,
            720U
        },
        /* Invalid Pipe Id */
        {
            CSL_DSS_VID_PIPE_ID_VID2,
            CSL_DSS_VID_PIPE_ID_VIDL2
        }
    },
    {
        /* Number of Pipes */
        1U,
        /* bpp */
        4U,
        /* Instance Id */
        {
            DSS_DISP_INST_VID2,
        },
        /* Pipe Id */
        {
            CSL_DSS_VID_PIPE_ID_VID2,
        },
        /* Pipe Node Id */
        {
            DSS_DCTRL_NODE_VID2,
        },
        /* Pipe Type */
        {
            CSL_DSS_VID_PIPE_TYPE_VID,
        },
        /* Data format */
        {
            FVID2_DF_BGRA32_8888,
        },
        /* Input buffer width */
        {
            480U,
        },
        /* Input buffer height */
        {
            360U,
        },
        /* Pitch */
        {
            {
                480U*4U, 0U, 0U, 0U, 0U, 0U
            },
        },
        /* Scan format */
        {
            FVID2_SF_PROGRESSIVE,
        },
        /* Output buffer width */
        {
            480U,
        },
        /* Output buffer height */
        {
            360U,
        },
        /* Scaler enable */
        {
            UFALSE,
        },
        /* Global Alpha */
        {
            0xFFU,
        },
        /* Pre-multiply alpha */
        {
            UFALSE,
        },
        /* X Position */
        {
            0U,
        },
        /* Y position */
        {
            0U,
        },
        /* Invalid Pipe Id */
        {
            CSL_DSS_VID_PIPE_ID_VID1,
            CSL_DSS_VID_PIPE_ID_VIDL1,
            CSL_DSS_VID_PIPE_ID_VIDL2
        }
    },
    {

    },
    {
    
    },
};

Dss_DctrlPathInfo gDssMstDctrlPathInfo = 
{
    .numEdges = 7U,
    .edgeInfo = 
    {
        {
            .startNode = DSS_DCTRL_NODE_VID1,
            .endNode = DSS_DCTRL_NODE_OVERLAY1,    
        },
        {
            .startNode = DSS_DCTRL_NODE_VIDL1,
            .endNode = DSS_DCTRL_NODE_OVERLAY1,
        },
        {
            .startNode = DSS_DCTRL_NODE_OVERLAY1,
            .endNode = DSS_DCTRL_NODE_VP1,
        },
        {
            .startNode = DSS_DCTRL_NODE_VP1,
            .endNode = DSS_DCTRL_NODE_EDP_MST,
        },
        {
            .startNode = DSS_DCTRL_NODE_VID2,
            .endNode = DSS_DCTRL_NODE_OVERLAY2,
        },
        {
            .startNode = DSS_DCTRL_NODE_OVERLAY2,
            .endNode = DSS_DCTRL_NODE_VP2,
        },
        {
            .startNode = DSS_DCTRL_NODE_VP2,
            .endNode = DSS_DCTRL_NODE_EDP_MST,
        }
    }
};

DssMst_dctrlInfo gDssMstDctrlInfo[DSS_MST_NUM_STREAMS_MAX] = 
{
    {
        CSL_DSS_VP_ID_1,
        CSL_DSS_OVERLAY_ID_1,
        FVID2_STD_1080P_60
    },
    {
        CSL_DSS_VP_ID_2,
        CSL_DSS_OVERLAY_ID_2,
        FVID2_STD_720P_60
    },
    {

    },
    {

    }
};

#ifdef __cplusplus
}
#endif /* DSS_MST_TEST_H_ */