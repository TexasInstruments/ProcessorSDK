/*
 *  Copyright (c) Texas Instruments Incorporated 2021 - 2022
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
 *  \file dss_m2m_test.c
 *
 *  \brief DSS sample application that does conversion frames for two instances
 *         of M2M Driver.
 *         This sample application demonstrates following:
 *              - YUV422 -> YUV420 conversion
 *              - Single channel conversion (through single M2M DRV instance)
 *                  - controlled through 'APP_DSS_M2M_CH_NUM'
 *              - Runtime loading of buffers (in this case YUV422)
 *          Refer to 'dss_m2m_test_cfg' file for more details about
 *          reconfigurability of this application.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <dss_m2m_test_cfg.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
int32_t Dss_displayM2MTest(void);
static int32_t App_init(M2MApp_AppObj *appObj);
static int32_t App_deInit(M2MApp_AppObj *appObj);
static int32_t App_create(M2MApp_AppObj *appObj);
static int32_t App_delete(M2MApp_AppObj *appObj);
static int32_t App_runTest(M2MApp_AppObj *appObj);
static int32_t App_initParams(M2MApp_AppObj *appObj);
static int32_t App_allocAndQueueFrames(M2MApp_InstParams *appObj);
static int32_t App_pipeCbFxn(Fvid2_Handle handle, void *appData);
extern void App_wait(uint32_t wait_in_ms);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

M2MApp_AppObj gM2MAppObj;
uint32_t gTestStopTime, gTestStartTime;

/* This buffer will be used to store RGB or Y */
static uint8_t gOutFrms[(APP_DSS_M2M_CH_NUM * APP_DSS_M2M_DSS_PIPE_NUM)][(APP_DSS_M2M_OUT_FRAME_PITCH * APP_DSS_M2M_OUT_FRAME_HEIGHT)] __attribute__(( aligned(128), section(".data_buffer")));
/* This buffer will be used to store UV */
static uint8_t gOutUVComp[(APP_DSS_M2M_CH_NUM * APP_DSS_M2M_DSS_PIPE_NUM)][(APP_DSS_M2M_OUT_FRAME_PITCH * APP_DSS_M2M_OUT_FRAME_HEIGHT)] __attribute__(( aligned(128), section(".data_buffer")));
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int32_t Dss_displayM2MTest(void)
{
    int32_t retVal = FVID2_SOK;
    M2MApp_AppObj *appObj;

    appObj = &gM2MAppObj;
    /* Initialize application object to '0' and the set only required parameters */
    Fvid2Utils_memset(appObj, 0U, sizeof(M2MApp_AppObj));

    retVal = App_initParams(appObj);
    if (FVID2_SOK != retVal)
    {
        App_consolePrintf("\nERROR: App_initParams() Failed!!!\n");
    }

    if (FVID2_SOK == retVal)
    {
        retVal += App_init(appObj);
        if (FVID2_SOK != retVal)
        {
            App_consolePrintf("\nERROR: App_init() Failed!!!\n");
        }
    }

    if (FVID2_SOK == retVal)
    {
        retVal += App_create(appObj);
        if (FVID2_SOK != retVal)
        {
            App_consolePrintf("\nERROR: App_create() Failed!!!\n");
        }
    }

    if (FVID2_SOK == retVal)
    {
        retVal += App_runTest(appObj);
        if (FVID2_SOK != retVal)
        {
            App_consolePrintf("\nERROR: App_runTest() Failed!!!\n");
        }
    }

    if (FVID2_SOK == retVal)
    {
        retVal += App_delete(appObj);
        if (FVID2_SOK != retVal)
        {
            App_consolePrintf("\nERROR: App_delete() Failed!!!\n");
        }
    }

    if (FVID2_SOK == retVal)
    {
        retVal += App_deInit(appObj);
        if (FVID2_SOK != retVal)
        {
            App_consolePrintf("\nERROR: App_deInit() Failed!!!\n");
        }
    }


    return retVal;
}
static int32_t App_initParams(M2MApp_AppObj *appObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t loopCnt, pipeIdx, planeIdx, layerIdx;
    M2MApp_InstParams *instObj;
    SemaphoreP_Params semParams;
    Dss_DispParams *dispParams;
    CSL_DssWbPipeCfg *wbPipeCfg;
    Dss_DctrlOverlayParams *ovrParams;
    Dss_DctrlOverlayLayerParams *layerParams;

    appObj->numInst            = APP_DSS_M2M_CH_NUM;
    appObj->numFramesToConvert = APP_DSS_M2M_FRAMES_WB;
    appObj->numFramesWb        = 0U;
    for (loopCnt = 0U ; loopCnt < appObj->numInst ; loopCnt++)
    {
        instObj                     = &appObj->instParams[loopCnt];
        instObj->instIdx            = loopCnt;
        instObj->appObj             = appObj;
        Dss_m2mCreateParamsInit(&instObj->createParams);
        Fvid2CbParams_init(&instObj->cbParams);
        instObj->cbParams.cbFxn         = &App_pipeCbFxn;
        instObj->cbParams.appData       = instObj;
        instObj->numPipe                = APP_DSS_M2M_DSS_PIPE_NUM;
        instObj->createParams.numPipe   = APP_DSS_M2M_DSS_PIPE_NUM;
        instObj->createParams.overlayId = APP_DSS_M2M_DSS_OVERLAY_ID;
        /* Initialize Display pipeline parameters */
        for (pipeIdx = 0U ; pipeIdx < APP_DSS_M2M_DSS_PIPE_NUM ; pipeIdx++)
        {
            instObj->createParams.pipeId[pipeIdx] = APP_DSS_M2M_DSS_PIPE_ID;
            instObj->pipeCfg[pipeIdx].pipeId      = APP_DSS_M2M_DSS_PIPE_ID;
            instObj->mFlagCfg[pipeIdx].pipeId     = APP_DSS_M2M_DSS_PIPE_ID;
            instObj->cscCfg[pipeIdx].pipeId       = APP_DSS_M2M_DSS_PIPE_ID;
            dispParams = &instObj->pipeCfg[pipeIdx].cfgParams;
            Dss_dispParamsInit(dispParams);
            dispParams->pipeCfg.pipeType = CSL_DSS_VID_PIPE_TYPE_VID;
            dispParams->pipeCfg.inFmt.width = APP_DSS_M2M_IN_FRAME_WIDTH;
            dispParams->pipeCfg.inFmt.height = APP_DSS_M2M_IN_FRAME_HEIGHT;
            for(planeIdx = 0U ; planeIdx < FVID2_MAX_PLANES; planeIdx++)
            {
                dispParams->pipeCfg.inFmt.pitch[planeIdx] = APP_DSS_M2M_IN_FRAME_PITCH;
            }
            dispParams->pipeCfg.inFmt.dataFormat  = APP_DSS_M2M_IN_FRAME_FORMAT;
            dispParams->pipeCfg.outWidth          = APP_DSS_M2M_IN_FRAME_WIDTH;
            dispParams->pipeCfg.outHeight         = APP_DSS_M2M_IN_FRAME_HEIGHT;
            dispParams->pipeCfg.scEnable          = UFALSE;
            dispParams->alphaCfg.globalAlpha      = 0xFFU;
            dispParams->alphaCfg.preMultiplyAlpha = UFALSE;
            dispParams->layerPos.startX           = 0U;
            dispParams->layerPos.startY           = 0U;

            Dss_dispPipeMflagParamsInit(&instObj->mFlagCfg[pipeIdx].mFlagCfg);

            CSL_dssCscCoeffInit(&instObj->cscCfg[pipeIdx].csc);
        }
        /* Initialize WB pipeline parameters */
        Dss_m2mPipeCfgParamsInit(&instObj->wbCfg);
        wbPipeCfg = &instObj->wbCfg.pipeCfg;
        /* Dont' need to set dataFormat as this is fixed to 'FVID2_DF_ARGB48_12121212'
           and is set in 'Dss_m2mPipeCfgParamsInit()' */
        wbPipeCfg->inFmt.width       = APP_DSS_M2M_IN_FRAME_WIDTH;
        wbPipeCfg->inFmt.height      = APP_DSS_M2M_IN_FRAME_HEIGHT;
        wbPipeCfg->outFmt.width      = APP_DSS_M2M_OUT_FRAME_WIDTH;
        wbPipeCfg->outFmt.height     = APP_DSS_M2M_OUT_FRAME_HEIGHT;
        wbPipeCfg->outFmt.dataFormat = APP_DSS_M2M_OUT_FRAME_FORMAT;
        if ( (wbPipeCfg->inFmt.width != wbPipeCfg->outFmt.width) || (wbPipeCfg->inFmt.height != wbPipeCfg->outFmt.height))
        {
            wbPipeCfg->scEnable = UTRUE;
        }
        else
        {
            wbPipeCfg->scEnable = UFALSE;
        }
        wbPipeCfg->outFmt.ccsFormat  = FVID2_CCSF_BITS8_PACKED;
        wbPipeCfg->inPos.startX      = 0U;
        wbPipeCfg->inPos.startY      = 0U;
        for(planeIdx = 0U ; planeIdx < FVID2_MAX_PLANES; planeIdx++)
        {
            wbPipeCfg->outFmt.pitch[planeIdx] = APP_DSS_M2M_OUT_FRAME_PITCH;
        }
        CSL_dssWbPipeDmaCfgInit(&instObj->wbDmaCfg);
        Dss_m2mMFlagParamsInit(&instObj->wbMflagCfg);
        CSL_dssCscCoeffInit(&instObj->wbCscCfg);
        Dss_m2mStatusInit(&instObj->wbStatus);

        /* Initialize Display overlay parameters */
        ovrParams = &instObj->ovrCfg;
        Dss_dctrlOverlayParamsInit(ovrParams);
        ovrParams->overlayId = APP_DSS_M2M_DSS_OVERLAY_ID;
        ovrParams->colorbarEnable = UFALSE;
        ovrParams->overlayCfg.colorKeyEnable = UFALSE;
        ovrParams->overlayCfg.colorKeySel = CSL_DSS_OVERLAY_TRANS_COLOR_DEST;
        ovrParams->overlayCfg.backGroundColor = 0xC8C800U;

        layerParams = &instObj->layerCfg;
        Dss_dctrlOverlayLayerParamsInit(layerParams);
        layerParams->overlayId = APP_DSS_M2M_DSS_OVERLAY_ID;
        /* Set all layer to invalid first and then update only used ones */
        for(layerIdx = 0U ; layerIdx < CSL_DSS_VID_PIPE_ID_MAX ; layerIdx++)
        {
            layerParams->pipeLayerNum[layerIdx] = CSL_DSS_OVERLAY_LAYER_INVALID;
        }
        layerParams->pipeLayerNum[instObj->createParams.pipeId[0U]] =
                                                    CSL_DSS_OVERLAY_LAYER_NUM_0;
        if(APP_DSS_M2M_DSS_PIPE_NUM > 1U)
        {
            for(layerIdx = 1U ; layerIdx < APP_DSS_M2M_DSS_PIPE_NUM ; layerIdx++)
            {
                layerParams->pipeLayerNum[instObj->createParams.pipeId[0U]] = layerIdx;
            }
        }

        Dss_dctrlGlobalDssParamsInit(&instObj->globalParams);

        /* Create sync semaphore */
        /* Allocate instance semaphore */
        SemaphoreP_Params_init(&semParams);
        semParams.mode   = SemaphoreP_Mode_BINARY;
        instObj->syncSem = SemaphoreP_create(0U, &semParams);
        if (NULL == instObj->syncSem)
        {
            retVal = FVID2_EFAIL;
            App_consolePrintf("\nERROR: Semaphore create failed!!!\n");
        }
    }

    return retVal;
}

void App_consolePrintf(const char *pcString, ...)
{
    char printBuffer[APP_PRINT_BUFFER_SIZE];
    va_list arguments;

    /* Start the var args processing. */
    va_start(arguments, pcString);
    vsnprintf (printBuffer, sizeof(printBuffer), pcString, arguments);
#if !defined (QT_BUILD)
    UART_printf(printBuffer);
#else
    printf(printBuffer);
#endif
    /* End the var args processing. */
    va_end(arguments);
}

static int32_t App_init(M2MApp_AppObj *appObj)
{
    int32_t retVal = FVID2_SOK;
    Fvid2_InitPrms  initPrms;

    Fvid2InitPrms_init(&initPrms);
    initPrms.printFxn = &App_consolePrintf;
    retVal = Fvid2_init(&initPrms);
    if(FVID2_SOK != retVal)
    {
        App_consolePrintf("\nERROR: FVID2 Init Failed!!!\r\n");
    }

    Dss_initParamsInit(&appObj->initParams);
    appObj->initParams.socParams.irqParams.dssCommonRegionId                    = CSL_DSS_COMM_REG_ID_1;
    appObj->initParams.socParams.irqParams.numValidIrq                          = DSS_EVT_MGR_INST_ID_MAX;
    appObj->initParams.socParams.irqParams.irqNum[DSS_EVT_MGR_INST_ID_FUNC]     = 53U;
    appObj->initParams.socParams.irqParams.irqNum[DSS_EVT_MGR_INST_ID_SAFETY]   = 55U;
    appObj->initParams.socParams.irqParams.irqNum[DSS_EVT_MGR_INST_ID_SECURITY] = 57U;
    appObj->initParams.socParams.dpInitParams.isHpdSupported                    = UFALSE;
    Dss_init(&appObj->initParams);

    if (FVID2_SOK == retVal)
    {
        App_consolePrintf("App_init() - DONE !!!\r\n");
    }

    return retVal;
}

static int32_t App_deInit(M2MApp_AppObj *appObj)
{
    int32_t retVal = FVID2_SOK;

    retVal += Dss_deInit();
    retVal += Fvid2_deInit(NULL);
    if(FVID2_SOK != retVal)
    {
         App_consolePrintf("\nERROR: App_deInit failed!!!\r\n");
    }
    else
    {
         App_consolePrintf("App_deInit() - DONE !!!\r\n");
    }

    return retVal;
}

static int32_t App_create(M2MApp_AppObj *appObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t instIdx, pipeIdx;
    M2MApp_InstParams *instObj;

    /* Create DSS M2M Driver instance */
    for (instIdx = 0U ; instIdx < appObj->numInst ; instIdx++)
    {
        instObj = &appObj->instParams[instIdx];
        instObj->drvHandle = Fvid2_create(DSS_M2M_DRV_ID,
                                             APP_DSS_M2M_INST,
                                             &instObj->createParams,
                                             &instObj->createStatus,
                                             &instObj->cbParams);
        if((NULL == instObj->drvHandle) ||
           (FVID2_SOK != instObj->createStatus.retVal))
        {
            retVal = instObj->createStatus.retVal;
            App_consolePrintf("\nERROR: Display M2M DRV Create Failed!!!\r\n");
        }
        /* Call Driver IOCTLs */
        if (FVID2_SOK == retVal)
        {
            for (pipeIdx = 0U ;
                 pipeIdx < instObj->createParams.numPipe ;
                 pipeIdx++)
            {
                retVal += Fvid2_control(
                            instObj->drvHandle,
                            IOCTL_DSS_M2M_SET_PIPE_PARAMS,
                            &instObj->pipeCfg[pipeIdx],
                            NULL);
                retVal += Fvid2_control(
                            instObj->drvHandle,
                            IOCTL_DSS_M2M_SET_PIPE_MFLAG_PARAMS,
                            &instObj->mFlagCfg[pipeIdx],
                            NULL);
                if (FVID2_SOK != retVal)
                {
                    App_consolePrintf("\nERROR: Display Pipe IOCTL Failed!!!\r\n");
                    break;
                }
            }
            if (FVID2_SOK == retVal)
            {
                retVal = Fvid2_control(
                            instObj->drvHandle,
                            IOCTL_DSS_DCTRL_SET_OVERLAY_PARAMS,
                            &instObj->ovrCfg,
                            NULL);
                if (FVID2_SOK != retVal)
                {
                    App_consolePrintf("\nERROR: IOCTL_DSS_DCTRL_SET_OVERLAY_PARAMS Failed!!!\r\n");
                }
            }
            if (FVID2_SOK == retVal)
            {
                retVal = Fvid2_control(
                            instObj->drvHandle,
                            IOCTL_DSS_DCTRL_SET_LAYER_PARAMS,
                            &instObj->layerCfg,
                            NULL);
                if (FVID2_SOK != retVal)
                {
                    App_consolePrintf("\nERROR: IOCTL_DSS_DCTRL_SET_LAYER_PARAMS Failed!!!\r\n");
                }
            }
            if (FVID2_SOK == retVal)
            {
                retVal = Fvid2_control(
                            instObj->drvHandle,
                            IOCTL_DSS_DCTRL_SET_GLOBAL_DSS_PARAMS,
                            &instObj->globalParams,
                            NULL);
                if (FVID2_SOK != retVal)
                {
                    App_consolePrintf("\nERROR: IOCTL_DSS_DCTRL_SET_GLOBAL_DSS_PARAMS Failed!!!\r\n");
                }
            }
            if (FVID2_SOK == retVal)
            {
                retVal = Fvid2_control(
                            instObj->drvHandle,
                            IOCTL_DSS_M2M_SET_WB_PIPE_PARAMS,
                            &instObj->wbCfg,
                            NULL);
                if (FVID2_SOK != retVal)
                {
                    App_consolePrintf("\nERROR: IOCTL_DSS_M2M_SET_WB_PIPE_PARAMS Failed!!!\r\n");
                }
            }
            if (FVID2_SOK == retVal)
            {
                retVal = Fvid2_control(
                            instObj->drvHandle,
                            IOCTL_DSS_M2M_SET_WB_PIPE_MFLAG_PARAMS,
                            &instObj->wbMflagCfg,
                            NULL);
                if (FVID2_SOK != retVal)
                {
                    App_consolePrintf("\nERROR: IOCTL_DSS_M2M_SET_WB_PIPE_MFLAG_PARAMS Failed!!!\r\n");
                }
            }
            if (FVID2_SOK == retVal)
            {
                retVal = Fvid2_control(
                            instObj->drvHandle,
                            IOCTL_DSS_M2M_SET_WB_PIPE_DMA_CFG,
                            &instObj->wbDmaCfg,
                            NULL);
                if (FVID2_SOK != retVal)
                {
                    App_consolePrintf("\nERROR: IOCTL_DSS_M2M_SET_WB_PIPE_DMA_CFG Failed!!!\r\n");
                }
            }
        }
    }

    return retVal;
}

static int32_t App_delete(M2MApp_AppObj *appObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t instIdx;
    Fvid2_FrameList inFrmList, outFrmList;
    M2MApp_InstParams *instObj;

    for (instIdx = 0U ; instIdx < appObj->numInst ; instIdx++)
    {
        instObj = &appObj->instParams[instIdx];
        /* Dequeue all the request from the driver */
        while (BTRUE)
        {
            retVal = Fvid2_getProcessedRequest(instObj->drvHandle,
                                               &inFrmList,
                                               &outFrmList,
                                               FVID2_TIMEOUT_NONE);
            if (FVID2_ENO_MORE_BUFFERS == retVal)
            {
                retVal = FVID2_SOK;
                break;
            }
            else if (FVID2_SOK != retVal)
            {
                App_consolePrintf("\nERROR: Fvid2_getProcessedRequest Failed!!!\r\n");
                break;
            }
            else
            {
                /* Do nothing here */
            }
        }

        retVal = FVID2_SOK;

        retVal = Fvid2_delete(instObj->drvHandle, NULL);
        if(FVID2_SOK != retVal)
        {
            App_consolePrintf("\nERROR: M2M Driver Delete Failed!!!\r\n");
            break;
        }
    }

    return retVal;
}

static int32_t App_runTest(M2MApp_AppObj *appObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t instIdx;
    M2MApp_InstParams *instObj;
    Fvid2_FrameList inFrmList, outFrmList;

    for (instIdx = 0U ; instIdx < appObj->numInst ; instIdx++)
    {
        instObj = &appObj->instParams[instIdx];
        /* Start the driver before queuing */
        retVal = Fvid2_start(instObj->drvHandle, NULL);
        if (FVID2_SOK != retVal)
        {
            App_consolePrintf("\nERROR: Fvid2_start Failed!!!\r\n");
            break;
        }

        /* Allocate and submit frames for processing */
        App_allocAndQueueFrames(instObj);
    }

    if (FVID2_SOK == retVal)
    {
        while (appObj->numFramesWb < appObj->numFramesToConvert)
        {
            if (FVID2_SOK == retVal)
            {
                for (instIdx = 0U ; instIdx < appObj->numInst ; instIdx++)
                {
                    instObj = &appObj->instParams[instIdx];
                    /* Wait for Semaphore */
                    (void) SemaphoreP_pend(instObj->syncSem, SemaphoreP_WAIT_FOREVER);
                    /* Get processed frames */
                    retVal = Fvid2_getProcessedRequest(instObj->drvHandle,
                                                       &inFrmList,
                                                       &outFrmList,
                                                       FVID2_TIMEOUT_NONE);
                    if(FVID2_SOK != retVal)
                    {
                        App_consolePrintf("\nERROR: Fvid2_processRequest Failed!!!\r\n");
                        break;
                    }
                    /* Submit back frames for processing */
                    retVal = Fvid2_processRequest(instObj->drvHandle,
                                                  &inFrmList,
                                                  &outFrmList,
                                                  FVID2_TIMEOUT_NONE);
                    if(FVID2_SOK != retVal)
                    {
                        App_consolePrintf("\nERROR: Fvid2_processRequest Failed!!!\r\n");
                        break;
                    }
                }
            }
        }
    }

    for (instIdx = 0U ; instIdx < appObj->numInst ; instIdx++)
    {
        instObj = &appObj->instParams[instIdx];
        /* Stop the driver before queuing */
        retVal = Fvid2_stop(instObj->drvHandle, NULL);
        if (FVID2_SOK != retVal)
        {
            App_consolePrintf("\nERROR: Fvid2_stop Failed!!!\r\n");
            break;
        }
    }

    for (instIdx = 0U ; instIdx < appObj->numInst ; instIdx++)
    {
        /* Get status */
        retVal += Fvid2_control(instObj->drvHandle,
                                IOCTL_DSS_M2M_GET_CURRENT_STATUS,
                                &instObj->wbStatus,
                                NULL);
        if (FVID2_SOK == retVal)
        {
            App_consolePrintf(
                "\n\r==========================================================\r\n");
            App_consolePrintf("Display M2M Status: %d\r\n", instIdx);
            App_consolePrintf(
                "==========================================================\r\n");
            App_consolePrintf("Input Format: 0x%x\r\n", APP_DSS_M2M_IN_FRAME_FORMAT);
            App_consolePrintf("Output Format: 0x%x\r\n", APP_DSS_M2M_OUT_FRAME_FORMAT);
            App_consolePrintf("Frames Submitted: %d\r\n", instObj->wbStatus.queueCount);
            App_consolePrintf("Frames Got Back After Processing: %d\r\n", instObj->wbStatus.dequeueCount);
            App_consolePrintf("Frames Processed: %d\r\n", instObj->wbStatus.wbFrmCount);
            App_consolePrintf("Frames Underflow Count: %d\r\n", instObj->wbStatus.underflowCount);
            App_consolePrintf(
                "==========================================================\r\n");
        }
        else
        {
            App_consolePrintf("\nERROR: Fvid2_control(IOCTL_DSS_M2M_GET_CURRENT_STATUS) Failed!!!\r\n");
            break;
        }
    }

    return retVal;
}

static int32_t App_allocAndQueueFrames(M2MApp_InstParams *instObj)
{
    int32_t  retVal = FVID2_SOK;
    uint32_t pipeIdx;
    Fvid2_Frame *frm;
    Fvid2_FrameList inFrmList, outFrmList;
    char uartInput = '0';

    App_consolePrintf("Load Image using loadRaw command and then press '1'\n");
    App_consolePrintf("Command is:\n");
    App_consolePrintf("loadRaw(0x%x, 0, \"C:\\\\display_yuyv422_prog_packed_1920_1080.tigf\", 32, false);\n",
              APP_DSS_M2M_DDR_LOAD_ADDRESS);
    do
    {
        scanf("%c", &uartInput);
    } while ('1' != uartInput);

    Fvid2FrameList_init(&inFrmList);
    for (pipeIdx = 0U ; pipeIdx < instObj->numPipe ; pipeIdx++)
    {
        frm = inFrmList.frames[pipeIdx];
        /* init Fvid2_Frame to 0's  */
        frm = (Fvid2_Frame *) &instObj->inFrames[pipeIdx];
        frm->chNum = instObj->createParams.pipeId[pipeIdx];
        frm->addr[0U] = (uint64_t)(APP_DSS_M2M_DDR_LOAD_ADDRESS +
               (pipeIdx * APP_DSS_M2M_IN_FRAME_PITCH * APP_DSS_M2M_IN_FRAME_HEIGHT));
        inFrmList.frames[pipeIdx] = frm;
    }
    inFrmList.numFrames = instObj->numPipe;
    frm = (Fvid2_Frame *) &instObj->outFrames[0U];
    /* init Fvid2_Frame to 0's  */
    frm->addr[0U] = (uint64_t)&(gOutFrms[(instObj->instIdx * APP_DSS_M2M_CH_NUM)][0U]);
    frm->addr[1U] = (uint64_t)&(gOutUVComp[(instObj->instIdx * APP_DSS_M2M_CH_NUM)][0U]);
    outFrmList.frames[0U] = frm;
    outFrmList.numFrames = 1U;
    retVal = Fvid2_processRequest(instObj->drvHandle,
                                  &inFrmList,
                                  &outFrmList,
                                  FVID2_TIMEOUT_NONE);
    if(FVID2_SOK != retVal)
    {
        App_consolePrintf("\nERROR: Fvid2_processRequest Failed!!!\r\n");
    }

    return retVal;
}

static int32_t App_pipeCbFxn(Fvid2_Handle handle, void *appData)
{
    int32_t retVal = FVID2_SOK;
    M2MApp_InstParams *instObj;

    GT_assert(DssTrace, (appData != NULL));

    instObj = (M2MApp_InstParams *)appData;
    instObj->appObj->numFramesWb++;
    /* Post semaphore */
    (void) SemaphoreP_post(instObj->syncSem);

    return retVal;
}
