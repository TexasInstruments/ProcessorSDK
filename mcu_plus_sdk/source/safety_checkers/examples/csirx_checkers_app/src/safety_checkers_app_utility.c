/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2024
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
 *  \file safety_checkers_app_csirx_utility.c
 *
 *  \brief This file contains CSIRX safety checkers application utility functions
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#if defined (SOC_J722S)
#include <csirx.h>
#include <drivers/i2c.h>
#include <board/board_control.h>
#else
#include <ti/drv/csirx/csirx.h>
#include <ti/drv/i2c/i2c.h>
#include <ti/board/board.h>
#include <ti/board/src/devices/board_devices.h>
#include "ti/safety_checkers/src/safety_checkers_csirx.h"
#include "ti/safety_checkers/src/safety_checkers_common.h"
#endif
#include "safety_checkers_csirx.h"
#include "safety_checkers_common.h"
#include "safety_checkers_app_csirx.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*None*/

/* ========================================================================== */
/*                         Global Variables                                   */
/* ========================================================================== */

/* Memory buffer to hold data */
uint8_t gSafetyCheckersAppCsirxFrmDropBuf[4U] __attribute__(( aligned(128), section(".data_buffer")));
static uint8_t gSafetyCheckersAppCsirxFrms[(4U*SAFETY_CHECKERS_APP_CSIRX_CH_NUM)][SAFETY_CHECKERS_APP_CSIRX_FRAME_SIZE] 
__attribute__(( aligned(128), section(".data_buffer")));

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/*None*/

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*None*/

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/*None*/

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#if !defined(SOC_J722S)
void SafetyCheckersApp_csirxSetupTimer(SafetyCheckersApp_CsirxCommonObj *appCommonObj)
{
    TimerP_Params timerParams;

    TimerP_Params_init(&timerParams);
    timerParams.runMode    = TimerP_RunMode_ONESHOT;
    timerParams.startMode  = TimerP_StartMode_USER;
    timerParams.periodType = TimerP_PeriodType_MICROSECS;
    timerParams.period     = ((SAFETY_CHECKERS_APP_CSIRX_TEST_PERIOD_IN_SEC) * (1000000));
    timerParams.arg        = (void*)appCommonObj;
    /* Creating a timer */
    appCommonObj->timerHandle= TimerP_create(0x1, (TimerP_Fxn)&SafetyCheckersApp_csirxTimerIsr, &timerParams);
    if (NULL_PTR == appCommonObj->timerHandle)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: Timer Create error\r\n");
    }
}
#endif

int32_t SafetyCheckersApp_csirxSetupApp(SafetyCheckersApp_CsirxCommonObj *appCommonObj)
{
#if defined (SOC_J722S)
    SemaphoreP_constructBinary(&appCommonObj->completionSem,0U);
#else
    SemaphoreP_Params semParams;
    /* Creating semaphore to indicate application completion of each Instance */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    appCommonObj->completionSem = SemaphoreP_create(0U, &semParams);
#endif
    return FVID2_SOK;
}

int32_t SafetyCheckersApp_csirxSetupI2CInst(SafetyCheckersApp_CsirxInstObj* appInstObj)
{   
    int32_t retVal = FVID2_SOK;
#if !defined (SOC_J722S)	
    uint8_t i2cInst = 0U, i2cAddr = 0U;
    I2C_Params i2cParams;

    /* Initializes the I2C Parameters */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz; /* 400KHz */

    Board_fpdU960GetI2CAddr(&i2cInst, &i2cAddr, appInstObj->boardCsiInstID);

    if ((0U == i2cInst) && (0U == i2cAddr))
    {
        retVal = FVID2_EFAIL;
    }
    else
    {
        /* Configures the I2C instance with the passed parameters*/
        appInstObj->i2cHandle = I2C_open(i2cInst, &i2cParams);
        if(NULL == appInstObj->i2cHandle)
        {
            SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: I2C Open failed!\n");
            retVal = FVID2_EFAIL;
        }
    }
#else
    Board_enableCSII2c(BOARD_CSI_I2C_MUX_INSTANCE);
    appInstObj->i2cHandle = I2C_getHandle(BOARD_CSI_I2C_SWITCH_INSTANCE);
#endif
    return retVal;
}

void SafetyCheckersApp_csirxInitParams(SafetyCheckersApp_CsirxInstObj* appInstObj)
{
    uint32_t loopCnt = 0U;

    /* Set instance configuration parameters */
    Csirx_createParamsInit(&appInstObj->createPrms);
    appInstObj->createPrms.numCh = SAFETY_CHECKERS_APP_CSIRX_CH_NUM;
    appInstObj->boardCsiInstID = appInstObj->instId;
    /* Set channel configuration parameters */
    for (loopCnt = 0U ; loopCnt < appInstObj->createPrms.numCh ; loopCnt++)
    {
        appInstObj->chFrmCnt[loopCnt] = 0U;
        appInstObj->createPrms.chCfg[loopCnt].chId = loopCnt;
        appInstObj->createPrms.chCfg[loopCnt].chType = CSIRX_CH_TYPE_CAPT;
        appInstObj->createPrms.chCfg[loopCnt].vcNum = loopCnt;
        appInstObj->createPrms.chCfg[loopCnt].inCsiDataType = FVID2_CSI2_DF_RAW12;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.width = 1936U;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.height = 1100U;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.pitch[0U] = 3872U;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.dataFormat = FVID2_DF_BGRX32_8888;
        appInstObj->createPrms.chCfg[loopCnt].outFmt.ccsFormat = FVID2_CCSF_BITS12_UNPACKED16;
    }

    /* Set module configuration parameters */
    appInstObj->createPrms.instCfg.enableCsiv2p0Support = UTRUE;
    appInstObj->createPrms.instCfg.numDataLanes = 4U;
    appInstObj->createPrms.instCfg.enableErrbypass = UFALSE;
    appInstObj->createPrms.instCfg.enableStrm[CSIRX_CAPT_STREAM_ID] = 1U;
    for (loopCnt = 0U ;
         loopCnt < appInstObj->createPrms.instCfg.numDataLanes ;
         loopCnt++)
    {
        appInstObj->createPrms.instCfg.dataLanesMap[loopCnt] = (loopCnt + 1U);
    }

    /* Set frame drop buffer parameters */
    appInstObj->createPrms.frameDropBufLen = 3872U;
    appInstObj->createPrms.frameDropBuf = (uint64_t)&gSafetyCheckersAppCsirxFrmDropBuf;
    /* This will be updated once Fvid2_create() is done */
    appInstObj->createStatus.retVal = FVID2_SOK;
    appInstObj->drvHandle = NULL;
    Fvid2CbParams_init(&appInstObj->cbPrms);
    appInstObj->cbPrms.cbFxn   = (Fvid2_CbFxn) &SafetyCheckersApp_csirxFrameCompletionCb;
    appInstObj->cbPrms.appData = appInstObj;

    appInstObj->numFramesRcvd = 0U;
    appInstObj->frameErrorCnt = 0U;
    appInstObj->maxWidth = 1936U;
    appInstObj->maxHeight = 1100U;

    /* Initialize capture instance status */
    Csirx_instStatusInit(&appInstObj->captStatus);
}

int32_t SafetyCheckersApp_csirxInit(SafetyCheckersApp_CsirxCommonObj* appCommonObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t instId;
#if !defined (SOC_J722S)
    uint32_t loopCnt;
    I2C_HwAttrs i2cConfig;
#endif
    Fvid2_InitPrms initPrms;
    Udma_InitPrms   udmaInitPrms;
    Udma_DrvHandle drvHandle;
    
    /* Set instance initialization parameters */
    /* This will be updated once UDMA init is done */
    Csirx_initParamsInit(&appCommonObj->initPrms);
    drvHandle = &appCommonObj->udmaDrvObj;
    appCommonObj->initPrms.drvHandle = drvHandle;
    /* Fvid2 init */
    Fvid2InitPrms_init(&initPrms);
    retVal |= Fvid2_init(&initPrms);

    /* Do UDMA init before CSIRX Init */
    /* UDMA driver init */
#if defined (SOC_J722S)
    instId = UDMA_INST_ID_BCDMA_1;
#elif defined (SOC_J721E)
    instId = UDMA_INST_ID_MAIN_0;
#else
    instId = UDMA_INST_ID_BCDMA_0;
#endif
    UdmaInitPrms_init(instId, &udmaInitPrms);
    Udma_init(drvHandle, &udmaInitPrms);
    retVal |= Csirx_init(&appCommonObj->initPrms);

#if !defined (SOC_J722S)
    if (FVID2_SOK == retVal)
    {
        /* Initialize I2C Driver */
        for(loopCnt = 0U; loopCnt < I2C_HWIP_MAX_CNT; loopCnt++)
        {
            I2C_socGetInitCfg(loopCnt, &i2cConfig);
            i2cConfig.enableIntr = BFALSE;
            I2C_socSetInitCfg(loopCnt, &i2cConfig);
        }

        /* Initializes the I2C */
        I2C_init();
    }
#endif

    return (retVal);
}

int32_t SafetyCheckersApp_csirxDelete(SafetyCheckersApp_CsirxInstObj* appInstObj)
{
    int32_t retVal = FVID2_SOK;
    static Fvid2_FrameList frmList;

    Fvid2FrameList_init(&frmList);
    /* Dequeue all the request from the driver */
    retVal += Fvid2_dequeue(appInstObj->drvHandle,
                           &frmList,
                           0U,
                           FVID2_TIMEOUT_NONE);

    if ((FVID2_SOK == retVal) || (FVID2_ENO_MORE_BUFFERS == retVal))
    {
        retVal = FVID2_SOK;
        /* Disable Error Events */
        retVal += Fvid2_control(appInstObj->drvHandle,
                               IOCTL_CSIRX_UNREGISTER_EVENT,
                               (void *)CSIRX_EVENT_GROUP_ERROR,
                               NULL);
    }

    if (FVID2_SOK == retVal)
    {
        retVal += Fvid2_delete(appInstObj->drvHandle, NULL);
    }
    else
    {
        appInstObj->drvHandle = NULL;
    }

    return (retVal);
}

int32_t SafetyCheckersApp_csirxDeinit(SafetyCheckersApp_CsirxCommonObj *appCommonObj)
{
    int32_t retVal = FVID2_SOK;
    Udma_DrvHandle drvHandle = &appCommonObj->udmaDrvObj;

    retVal += Csirx_deInit();
    /* System de-init */
    if(UDMA_SOK != Udma_deinit(drvHandle))
    {
        retVal += FVID2_EFAIL;
    }

    Fvid2_deInit(NULL);
    /* Close I2C channel */
    I2C_close(appCommonObj->appInstObj.i2cHandle);
    /* Delete semaphore */
#if defined(SOC_J722S)
    SemaphoreP_destruct(&appCommonObj->completionSem);
#else
    SemaphoreP_delete(appCommonObj->completionSem);
    /* Delete Timer */
    TimerP_delete(appCommonObj->timerHandle);
#endif
    return (retVal);
}

int32_t SafetyCheckersApp_csirxCreate(SafetyCheckersApp_CsirxInstObj* appInstObj)
{
    int32_t retVal = FVID2_SOK;
    Csirx_DPhyCfg dphyCfg;
    Csirx_EventPrms eventPrms;

    /* Fvid2_create() */
    appInstObj->drvHandle = Fvid2_create(CSIRX_CAPT_DRV_ID,
                                         appInstObj->instId,
                                         &appInstObj->createPrms,
                                         &appInstObj->createStatus,
                                         &appInstObj->cbPrms);

    if ((NULL == appInstObj->drvHandle) ||
        (FVID2_SOK != appInstObj->createStatus.retVal))
    {
        retVal += appInstObj->createStatus.retVal;
    }

    if (FVID2_SOK == retVal)
    {
        /* Set CSIRX D-PHY configuration parameters */
        Csirx_initDPhyCfg(&dphyCfg);
        dphyCfg.inst = appInstObj->instId;
        retVal += Fvid2_control(appInstObj->drvHandle,
                               IOCTL_CSIRX_SET_DPHY_CONFIG,
                               &dphyCfg,
                               NULL);

    }

    if (FVID2_SOK == retVal)
    {
        /* Register Error Events */
        Csirx_eventPrmsInit(&eventPrms);
        retVal += Fvid2_control(appInstObj->drvHandle,
                               IOCTL_CSIRX_REGISTER_EVENT,
                               &eventPrms,
                               NULL);
    }

    if (FVID2_SOK == retVal)
    {
        retVal += SafetyCheckersApp_csirxSetupI2CInst(appInstObj);
    }

    /* Allocate and queue all available frames */
    retVal += SafetyCheckersApp_csirxAllocAndQFrames(appInstObj);
    /* Configure sensor here */
    retVal += SafetyCheckersApp_csirxSensorConfig(appInstObj);
    return (retVal);
}

int32_t SafetyCheckersApp_csirxFrameCompletionCb(Fvid2_Handle handle,
                                     Ptr appData,
                                     Ptr reserved)
{
    int32_t  retVal = FVID2_SOK;
    uint32_t frmIdx = 0U;
    static Fvid2_FrameList frmList;
    Fvid2_Frame *pFrm;

    SafetyCheckersApp_CsirxInstObj *appInstObj = (SafetyCheckersApp_CsirxInstObj *) appData;
    GT_assert(gSafetyCheckersAppCsirxTrace, (NULL != appData));
    Fvid2FrameList_init(&frmList);
    retVal += Fvid2_dequeue(appInstObj->drvHandle, &frmList, 0U, FVID2_TIMEOUT_NONE);
    if (FVID2_SOK == retVal)
    {
        appInstObj->numFramesRcvd += frmList.numFrames;
        for (frmIdx = 0U; frmIdx < frmList.numFrames; frmIdx++)
        {
            pFrm = frmList.frames[frmIdx];
            appInstObj->chFrmCnt[pFrm->chNum]++;
            if (FVID2_FRAME_STATUS_COMPLETED != pFrm->status)
            {
                appInstObj->frameErrorCnt++;
            }

        }

        /* Queue back de-queued frames, last param i.e. streamId is unused in DRV */
        retVal += Fvid2_queue(appInstObj->drvHandle, &frmList, 0U);
        if (FVID2_SOK != retVal)
        {
            SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: Capture Queue Failed!!!\r\n");
        }

    }

    /* Always return 'FVID2_SOK' */

    return FVID2_SOK;
}

int32_t SafetyCheckersApp_csirxAllocAndQFrames(SafetyCheckersApp_CsirxInstObj *appInstObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t chIdx = 0U, frmIdx = 0U;
    static Fvid2_FrameList frmList;
    Fvid2_Frame  *pFrm;

    /* For every channel in a capture handle,
     * allocate memory for and queue frames 
     */
    Fvid2FrameList_init(&frmList);
    frmList.numFrames = 0U;
    for (chIdx = 0U; chIdx < appInstObj->createPrms.numCh ; chIdx++)
    {
        for (frmIdx = 0U; frmIdx < 4U ; frmIdx++)
        {
            /* Assign frames memory */
            /* Only following fields are used in CSIRX DRV */
            pFrm = (Fvid2_Frame *)
                    &appInstObj->frames[(chIdx * 4U) + frmIdx];
            pFrm->addr[0U] =
               (uint64_t)&gSafetyCheckersAppCsirxFrms[(chIdx * 4U) + frmIdx][0U];
            pFrm->chNum = appInstObj->createPrms.chCfg[chIdx].chId;
            pFrm->appData = appInstObj;
            frmList.frames[frmList.numFrames] = pFrm;
            frmList.numFrames++;
        }

    }

    retVal = Fvid2_queue(appInstObj->drvHandle, &frmList, 0U);
    if (FVID2_SOK != retVal)
    {
        SafetyCheckersApp_csirxlog("SAFETY_CHECKERS_CSIRX_APP: Capture Queue Failed!!!\r\n");
    }

    return retVal;
}

int32_t SafetyCheckersApp_csirxFreeFrames(SafetyCheckersApp_CsirxInstObj *appInstObj)
{
    int32_t retVal = FVID2_SOK;
    static Fvid2_FrameList frmList;

    /* For every stream and channel in a capture handle */
    Fvid2FrameList_init(&frmList);

    /* Deq-queue any frames queued more than needed */
    retVal = Fvid2_dequeue(appInstObj->drvHandle,
                           &frmList,
                           0U,
                           FVID2_TIMEOUT_NONE);
    if (FVID2_ENO_MORE_BUFFERS == retVal)
    {
        /* All buffer might be de-queued during stop, in this case no error shall be returned */
        retVal = FVID2_SOK;
    }

    return (retVal);
}

#ifdef __cplusplus
}
#endif

/* @} */
