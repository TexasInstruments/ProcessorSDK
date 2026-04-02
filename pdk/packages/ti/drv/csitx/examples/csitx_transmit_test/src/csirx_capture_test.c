/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2019
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
 *  \file csirx_capture_test_main.c
 *
 *  \brief CSI RX Capture Example.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "csitx_transmit_test_cfg.h"

#if (APP_ENABLE_LOOPBACK == 1U)
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define APP_NAME_RX                        "CSITX_CAPT_APP"

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 * \brief   CSIRX Test function for Rx task
 *
 * \param   none.
 *
 * \retval  none.
 */
void Csirx_captureTest(void);

/**
 * \brief   This function is used to initialize test parameters
 *
 * \param   appObj          Type of print message.
 *                          Refer to struct #appTxObj
 *
 * \retval  none.
 */
void App_initCaptParams(appTxObj* appObj);

/**
 * \brief   App Init function.
 *
 * \param   appObj          CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t App_captInit(appTxObj* appObj);

/**
 * \brief   App create function.
 *
 * \param   appObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t App_captCreate(appTxObj* appObj);

/**
 * \brief   App CSI test function: captures frames.
 *
 * \param   appObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t App_csirxTest(appTxObj* appObj);

/**
 * \brief   App delete function.
 *
 * \param   appObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t App_captDelete(appTxObj* appObj);

/**
 * \brief   App Init function.
 *
 * \param   appObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t App_captDeinit(appTxObj* appObj);

/**
 * \brief   App Callback function for frame completion.
 *
 * \param   handle        Fvid2 DRV handle
 *
 * \param   appData       App based back by to CB function
 *
 * \param   reserved      reserved, not used
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t App_captFrameCompletionCb(Fvid2_Handle handle,
                                     Ptr appData,
                                     Ptr reserved);

/**
 * \brief   App Callback function for frame completion.
 *
 * \param   appObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t App_captAllocAndQFrames(appTxObj *appObj);

/**
 * \brief   App Callback function for frame completion.
 *
 * \param   appObj        CSI RX Capture Test Object
 *
 * \retval  status          FVID2_SOK on successful
 *                          else otherwise.
 */
int32_t App_captFreeFrames(appTxObj *appObj);

extern void App_wait(uint32_t wait_in_ms);

/* Functions from CSITX transmit test application file */
extern void App_consolePrintf(const char *pcString, ...);
extern void App_fvidPrint(const char *str, ...);
extern void App_dmaPrint(const char *str);
extern uint32_t App_getCurTimeInMsec(void);
extern uint32_t App_getElapsedTimeInMsec(uint32_t startTime);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
extern appTxObj gAppObj;
/* Memory buffer to hold data */
extern uint8_t gFrmDropBuf[(APP_TX_FRAME_PITCH)];
extern uint8_t gRxFrms[(APP_RX_FRAMES_PER_CH * APP_TX_CH_NUM)][APP_TX_FRAME_SIZE];
uint8_t gCompareFrame = 1;
SemaphoreP_Handle gRxLockSem;

/*
 * UDMA driver objects
 */
extern struct Udma_DrvObj gUdmaDrvObj;
extern SemaphoreP_Handle gSyncSem;
extern SemaphoreP_Handle gRxStartSem;
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Application main
 */
void Csirx_captureTest(void)
{
    int32_t retVal = FVID2_SOK;

    /* Pend sync semaphore here, wait signal for Rx task to start  */
    SemaphoreP_pend(gRxStartSem, SemaphoreP_WAIT_FOREVER);

    /* App CSI test function */
    retVal = App_csirxTest(&gAppObj);
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsitxAppTrace, GT_ERR,
          APP_NAME_RX ": [ERROR]App_csirxTest() FAILED!!!\r\n");
    }
    /* Post sync semaphore here */
    SemaphoreP_post(gSyncSem);
}

void App_initCaptParams(appTxObj* appObj)
{
    uint32_t loopCnt = 0U;

    /* set instance to be used for capture */
    appObj->captObj.instId = CSIRX_INSTANCE_ID_0;
    /* set instance initialization parameters */
    /* This will be updated once UDMA init is done */
    Csirx_initParamsInit(&appObj->captObj.initPrms);
    /* set instance configuration parameters */
    Csirx_createParamsInit(&appObj->captObj.createPrms);
    appObj->captObj.createPrms.numCh = APP_TX_CH_NUM;
    /* set channel configuration parameters */
    for (loopCnt = 0U ; loopCnt < appObj->captObj.createPrms.numCh ; loopCnt++)
    {
        appObj->captObj.chFrmCnt[loopCnt] = 0U;
        appObj->captObj.createPrms.chCfg[loopCnt].chId = loopCnt;
        appObj->captObj.createPrms.chCfg[loopCnt].chType = CSIRX_CH_TYPE_CAPT;
        appObj->captObj.createPrms.chCfg[loopCnt].vcNum = loopCnt;
        appObj->captObj.createPrms.chCfg[loopCnt].inCsiDataType = APP_TX_IMAGE_DT;
        appObj->captObj.createPrms.chCfg[loopCnt].outFmt.width = APP_TX_FRAME_WIDTH;
        appObj->captObj.createPrms.chCfg[loopCnt].outFmt.height = APP_TX_FRAME_HEIGHT;
        appObj->captObj.createPrms.chCfg[loopCnt].outFmt.pitch[0U] =
                                                APP_TX_FRAME_PITCH;
        appObj->captObj.createPrms.chCfg[loopCnt].outFmt.dataFormat =
                                                FVID2_DF_BGRX32_8888;
        appObj->captObj.createPrms.chCfg[loopCnt].outFmt.ccsFormat =
                                                APP_TX_IMAGE_STORAGE_FORMAT;
    }
    /* set module configuration parameters */
    appObj->captObj.createPrms.instCfg.enableCsiv2p0Support = UTRUE;
    appObj->captObj.createPrms.instCfg.numDataLanes = 4U;
    appObj->captObj.createPrms.instCfg.enableErrbypass = UFALSE;
    appObj->captObj.createPrms.instCfg.enableStrm[CSIRX_CAPT_STREAM_ID] = 1U;
    for (loopCnt = 0U ;
         loopCnt < appObj->captObj.createPrms.instCfg.numDataLanes ;
         loopCnt++)
    {
        appObj->captObj.createPrms.instCfg.dataLanesMap[loopCnt] = (loopCnt + 1U);
    }
    /* set frame drop buffer parameters */
    appObj->captObj.createPrms.frameDropBufLen =
                                (APP_TX_FRAME_WIDTH * APP_TX_FRAME_BPP);
    appObj->captObj.createPrms.frameDropBuf = (uint64_t)&gFrmDropBuf;
    /* This will be updated once Fvid2_create() is done */
    appObj->captObj.createStatus.retVal = FVID2_SOK;
    appObj->captObj.drvHandle = NULL;
    Fvid2CbParams_init(&appObj->captObj.cbPrms);
    appObj->captObj.cbPrms.cbFxn   = (Fvid2_CbFxn) &App_captFrameCompletionCb;
    appObj->captObj.cbPrms.appData = appObj;

    appObj->captObj.numFramesToCapture = APP_TX_FRAMES_TX;
    appObj->captObj.numFramesRcvd = 0U;
    appObj->captObj.frameErrorCnt = 0U;

    /* Initialize capture instance status */
    Csirx_instStatusInit(&appObj->captObj.captStatus);
}

int32_t App_captInit(appTxObj* appObj)
{
    int32_t retVal = FVID2_SOK;

    /* System init */
    retVal = Csirx_init(&appObj->captObj.initPrms);
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsitxAppTrace, GT_ERR,
                  APP_NAME_RX ": System Init Failed!!!\r\n");
    }

    return (retVal);
}

int32_t App_captCreate(appTxObj* appObj)
{
    int32_t retVal = FVID2_SOK;
    SemaphoreP_Params semParams;
    Fvid2_TimeStampParams tsParams;
    Csirx_DPhyCfg dphyCfg;

    /* Fvid2_create() */
    appObj->captObj.drvHandle = Fvid2_create(
        CSIRX_CAPT_DRV_ID,
        appObj->captObj.instId,
        &appObj->captObj.createPrms,
        &appObj->captObj.createStatus,
        &appObj->captObj.cbPrms);

    if ((NULL == appObj->captObj.drvHandle) ||
        (FVID2_SOK != appObj->captObj.createStatus.retVal))
    {
        GT_0trace(CsitxAppTrace, GT_ERR,
                  APP_NAME_RX ": Capture Create Failed!!!\r\n");
        retVal = appObj->captObj.createStatus.retVal;
    }
    if (FVID2_SOK == retVal)
    {
        /* Set CSIRX D-PHY configuration parameters */
        Csirx_initDPhyCfg(&dphyCfg);
        dphyCfg.inst               = CSIRX_INSTANCE_ID_0;
        dphyCfg.leftLaneBandSpeed  = CSIRX_LANE_BAND_SPEED_800_TO_880_MBPS;
        dphyCfg.rightLaneBandSpeed = CSIRX_LANE_BAND_SPEED_800_TO_880_MBPS;
        retVal = Fvid2_control(appObj->captObj.drvHandle,
                                IOCTL_CSIRX_SET_DPHY_CONFIG,
                                &dphyCfg,
                                NULL);
        if(FVID2_SOK != retVal)
        {
            GT_0trace(CsitxAppTrace,
                      GT_INFO,
                      APP_NAME_RX
                      ":Set D-PHY Configuration FAILED!!!\r\n");
        }
        else
        {
            GT_0trace(CsitxAppTrace,
                      GT_INFO,
                      APP_NAME_RX
                      ":Set D-PHY Configuration Successful!!!\r\n");
        }
    }
    if (FVID2_SOK == retVal)
    {
        /* Allocate instance semaphore */
        SemaphoreP_Params_init(&semParams);
        semParams.mode = SemaphoreP_Mode_BINARY;
        gRxLockSem = SemaphoreP_create(0U, &semParams);
        if (NULL == gRxLockSem)
        {
            GT_0trace(
                CsitxAppTrace, GT_ERR,
                "Instance semaphore create failed!!\r\n");
            retVal = FVID2_EALLOC;
        }
    }

    if (FVID2_SOK == retVal)
    {
        tsParams.timeStampFxn = (Fvid2_TimeStampFxn)&TimerP_getTimeInUsecs;
        /* register time stamping function */
        retVal = Fvid2_control(appObj->captObj.drvHandle,
                               FVID2_REGISTER_TIMESTAMP_FXN,
                               &tsParams,
                               NULL);
    }
    if (FVID2_SOK == retVal)
    {
        GT_0trace(CsitxAppTrace,
                  GT_INFO,
                  APP_NAME_RX ": CSIRX Capture created\r\n");
    }
    return (retVal);
}

int32_t App_csirxTest(appTxObj* appObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t startTime, elapsedTime, fps;
    uint64_t tempVar;

    startTime = App_getCurTimeInMsec();
    /* Wait for reception completion */
    if (0U == appObj->captObj.numFramesToCapture)
    {
        /* indefinite capture */
        /* TODO: Press any key to stop capture */
    }
    else
    {
        while (appObj->captObj.numFramesRcvd < appObj->captObj.numFramesToCapture)
        {
            /* Pend on semaphore until last frame is received */
            SemaphoreP_pend(gRxLockSem, SemaphoreP_WAIT_FOREVER);
            /* Check for Image data here */
        }
    }
    elapsedTime = App_getElapsedTimeInMsec(startTime);
    /* fps calculation and some x100 for precision */
    tempVar = ((uint64_t)(appObj->captObj.numFramesRcvd * 100000U)) / (elapsedTime);
    fps = (uint32_t)tempVar;

    GT_4trace(CsitxAppTrace, GT_INFO,
          APP_NAME_RX ": %d frames captured in %d msec"
          " at the rate of %d.%2d frames/sec.\r\n",
          appObj->captObj.numFramesRcvd,
          elapsedTime,
          (fps / 100U),
          (fps % 100U));

    return (retVal);
}

int32_t App_captDelete(appTxObj* appObj)
{
    int32_t retVal = FVID2_SOK;
    Fvid2_FrameList frmList;

    Fvid2FrameList_init(&frmList);
    /* Dequeue all the request from the driver */
    retVal = Fvid2_dequeue(
                    appObj->captObj.drvHandle,
                    &frmList,
                    0U,
                    FVID2_TIMEOUT_NONE);

    if ((FVID2_SOK != retVal) && (FVID2_ENO_MORE_BUFFERS != retVal))
    {
        GT_0trace(CsitxAppTrace, GT_ERR,
                  APP_NAME_RX ": Capture De-queue Failed!!!\r\n");
    }
    else
    {
        retVal = Fvid2_delete(appObj->captObj.drvHandle, NULL);
        if (FVID2_SOK != retVal)
        {
            GT_0trace(CsitxAppTrace, GT_ERR,
                      APP_NAME_RX ": FVID2 Delete Failed!!!\r\n");
        }
        else
        {
            appObj->captObj.drvHandle = NULL;
        }

        if (FVID2_SOK == retVal)
        {
            /* Delete semaphore */
            SemaphoreP_delete(gRxLockSem);
            GT_0trace(CsitxAppTrace, GT_INFO, APP_NAME_RX ": Capture Driver deleted\r\n");
        }
    }

    return (retVal);
}

int32_t App_captDeinit(appTxObj* appObj)
{
    int32_t retVal = FVID2_SOK;

    retVal = Csirx_deInit();

    return (retVal);
}

int32_t App_captFrameCompletionCb(Fvid2_Handle handle,
                                     Ptr appData,
                                     Ptr reserved)
{
    int32_t  retVal = FVID2_SOK;
    uint32_t frmIdx = 0U, idx = 0U;
    Fvid2_FrameList frmList;
    Fvid2_Frame *pFrm;
    appTxObj *appObj = (appTxObj *) appData;

    GT_assert(CsitxAppTrace, (NULL != appData));

    Fvid2FrameList_init(&frmList);
    retVal = Fvid2_dequeue(
        appObj->captObj.drvHandle,
        &frmList,
        0U,
        FVID2_TIMEOUT_NONE);
    if (FVID2_SOK == retVal)
    {
        appObj->captObj.numFramesRcvd += frmList.numFrames;
        for (frmIdx = 0U; frmIdx < frmList.numFrames; frmIdx++)
        {
            uint8_t *pData;
            int32_t chIndex;
            pFrm = frmList.frames[frmIdx];
            pData = (uint8_t *)pFrm->addr[0];
            appObj->captObj.chFrmCnt[pFrm->chNum]++;
            if (FVID2_FRAME_STATUS_COMPLETED != pFrm->status)
            {
                idx = (appObj->captObj.frameErrorCnt % APP_ERR_FRAME_LOG_MAX);
                appObj->captObj.errFrmCh[idx] = pFrm->chNum;
                appObj->captObj.errFrmNo[idx] = appObj->captObj.chFrmCnt[pFrm->chNum];
                appObj->captObj.errFrmTs[idx] = (uint32_t)(pFrm->timeStamp64 / 1000U);
                appObj->captObj.frameErrorCnt++;
            }
            CacheP_Inv(pData, APP_TX_FRAME_SIZE);
            chIndex = (((uint32_t)pData[0])<<24) | (((uint32_t)pData[1])<<16) | (((uint32_t)pData[2]) << 8) | ((uint32_t)pData[3]);
            if (0 != memcmp(pData, &gTxFrms[chIndex][0], APP_TX_FRAME_SIZE))
            {
                gCompareFrame = 0;
                printf("Channel->%d, NumTx in Channel->%d, numFrames->%d!!!\r\n", pFrm->chNum, appObj->captObj.chFrmCnt[pFrm->chNum], frmList.numFrames);
                GT_0trace(CsitxAppTrace, GT_ERR,
                      APP_NAME_RX ": Data does not match!!!\r\n");
            }
        }

        /* Queue back de-queued frames,
           last param i.e. streamId is unused in DRV */
        retVal = Fvid2_queue(appObj->captObj.drvHandle, &frmList, 0U);
        if (FVID2_SOK != retVal)
        {
            GT_0trace(CsitxAppTrace, GT_ERR,
                      APP_NAME_RX ": Capture Queue Failed!!!\r\n");
        }
    }

    /* always return 'FVID2_SOK' */
    /* Post semaphore here for sending next Image */
    SemaphoreP_post(gRxLockSem);

    return FVID2_SOK;
}

int32_t App_captAllocAndQFrames(appTxObj *appObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t chIdx = 0U, frmIdx = 0U;
    Fvid2_FrameList frmList;
    Fvid2_Frame  *pFrm;

    /* for every channel in a capture handle,
       allocate memory for and queue frames */
    Fvid2FrameList_init(&frmList);
    frmList.numFrames = 0U;
    for (chIdx = 0U; chIdx < appObj->captObj.createPrms.numCh ; chIdx++)
    {
        for (frmIdx = 0U; frmIdx < APP_RX_FRAMES_PER_CH ; frmIdx++)
        {
            /* assign frames memory */
            /* Only following fields are used in CSIRX DRV */
            pFrm = (Fvid2_Frame *)
                    &appObj->captObj.frames[(chIdx * APP_RX_FRAMES_PER_CH) + frmIdx];
            pFrm->addr[0U] =
               (uint64_t)&gRxFrms[(chIdx * APP_RX_FRAMES_PER_CH) + frmIdx][0U];
            pFrm->chNum = appObj->captObj.createPrms.chCfg[chIdx].chId;
            pFrm->appData = appObj;
            frmList.frames[frmList.numFrames] = pFrm;
            frmList.numFrames++;
        }
    }
    /* queue the frames in frmList
     * All allocated frames are queued here as an example.
     * In general at least 2 frames per stream/channel need to queued
     * before capture can be started.
     * Failing which, frame could be dropped.
     */
    /* last parameter, i.e. streamId is unused in CSIRX DRV */
    retVal = Fvid2_queue(appObj->captObj.drvHandle, &frmList, 0U);
    if (FVID2_SOK != retVal)
    {
        GT_0trace(CsitxAppTrace, GT_ERR,
                  APP_NAME_RX ": Capture Queue Failed!!!\r\n");
    }

    return retVal;
}

int32_t App_captFreeFrames(appTxObj *appObj)
{
    int32_t retVal = FVID2_SOK;
    Fvid2_FrameList frmList;

    /* for every stream and channel in a capture handle */
    Fvid2FrameList_init(&frmList);

    /* Deq-queue any frames queued more than needed */
    retVal = Fvid2_dequeue(
                    appObj->captObj.drvHandle,
                    &frmList,
                    0U,
                    FVID2_TIMEOUT_NONE);
    if (FVID2_ENO_MORE_BUFFERS == retVal)
    {
        /* All buffer might be de-queued during stop,
           in this case no error shall be returned */
        retVal = FVID2_SOK;
    }
    /* TODO: Free up frame allocated memories here */
    /* it is global variable here so not needed */

    return (retVal);
}
#endif
