/*
*  Copyright (c) Texas Instruments Incorporated 2019
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
*  \file vhwa_dof_api.c
*
*  \brief VHWA DOF APIs
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <include/vhwa_m2mDof.h>
#include <examples/include/vhwa_dof_api.h>

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
#if defined VHWA_PDK
static int32_t AppDof_CompareCrc(DofApp_TestParams *tObj,
                                 uint32_t hidx);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

AppDof_TestObject gAppDofTestObj[APP_DOF_MAX_HANDLES];
#if defined VHWA_PDK
AppCrc_hdlPrms gDofCrcChHandle;
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t AppDof_Init(Udma_DrvHandle udmaDrvHndl)
{
    int32_t                 status;
    Vhwa_M2mDofSl2AllocPrms sl2AllocPrms;
    Vhwa_M2mDofInitParams   initPrms;
    #if defined (SOC_J742S2) || defined (SOC_J784S4)
    /* Powerup DMPAC0 module */
    Sciclient_pmSetModuleState (TISCI_DEV_DMPAC0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
            TISCI_MSG_FLAG_AOP,
            0xFFFFFFFFU);
    #endif
    /* Initialize DOF Init parameters */
    Vhwa_m2mDofInitParamsInit(&initPrms);

    /* Set UDMA driver handle */
    initPrms.udmaDrvHndl = udmaDrvHndl;

    status = Vhwa_m2mDofInit(&initPrms);
    if (FVID2_SOK != status)
    {
        Vhwa_print(" DOF_TEST_APP: DOF Init Failed\n");
    }
    else
    {
        /* Initilize SL2 parameters */
        Vhwa_M2mDofSl2AllocPrmsInit(&sl2AllocPrms);

        sl2AllocPrms.maxImgWidth = APP_DOF_DEFAULT_IMG_WIDTH;
        sl2AllocPrms.inCcsf = APP_DOF_DEFAULT_CCSF;
        sl2AllocPrms.topSR = APP_DOF_DEFAULT_TOP_SR;
        sl2AllocPrms.botSR = APP_DOF_DEFAULT_BOT_SR;

        status = Vhwa_m2mDofAllocSl2(&sl2AllocPrms);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" DOF_TEST_APP: SL2 Alloc Failed !!!\n");
        }
    }

    return (status);
}

int32_t AppDofFrameComplCb(Fvid2_Handle handle, void *appData)
{
    AppDof_TestObject *tObj = (AppDof_TestObject *)appData;

    if (NULL != tObj)
    {
        VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
    }

    return FVID2_SOK;
}

void AppDofErrorCb(Fvid2_Handle handle, uint32_t errEvents, void *appData)
{
    AppDof_TestObject *tObj = (AppDof_TestObject *)appData;

    if (NULL != tObj)
    {
        tObj->err_stat |= errEvents;
        if(0u != tObj->err_stat)
        {
            VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
        }
    }
}

void AppDofWdtimerErrorCb(Fvid2_Handle handle, uint32_t wdTimerErrEvents, void *appData)
{
    AppDof_TestObject *tObj = (AppDof_TestObject *)appData;
    if (NULL != tObj)
    {
        tObj->wdTimerErrStatus |= wdTimerErrEvents;
        if(0u != tObj->wdTimerErrStatus)
        {
            VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
        }
    }
}

int32_t AppDof_Create(DofApp_TestParams *tObj, uint32_t hidx)
{
    int32_t             status;
    AppDof_TestObject      *appObj = &gAppDofTestObj[hidx];

    Fvid2Utils_memset(appObj, 0x0, sizeof(AppDof_TestObject));
    #if defined VHWA_MCU_PLUS_SDK
    #if defined VHWA_FREERTOS_SUPPORT 
    status = SemaphoreP_constructBinary(&appObj->waitForProcessCmpl, 0);
    #else
    status = SemaphoreP_constructMutex(&appObj->waitForProcessCmpl);
    #endif
    #else
    SemaphoreP_Params   params;
    SemaphoreP_Params_init(&params);
    params.mode = SemaphoreP_Mode_BINARY;
    appObj->waitForProcessCmpl = SemaphoreP_create(0U, &params);    
    if (NULL != appObj->waitForProcessCmpl)
    #endif
    #if defined VHWA_MCU_PLUS_SDK
    if (SystemP_SUCCESS ==  status)
    #endif
    {
        if (UTRUE == tObj->testCfg[hidx]->isPsa)
        {
            appObj->createArgs.enablePsa = UTRUE;
        }
        else
        {
            appObj->createArgs.enablePsa = UFALSE;
        }

        #if defined VHWA_PDK
        if(tObj->isPerformanceTest)
        {
            appObj->createArgs.getTimeStamp = Vhwa_getTime;
        }
        #endif

        appObj->cbPrms.cbFxn   = AppDofFrameComplCb;
        appObj->cbPrms.appData = appObj;

        appObj->handle = Fvid2_create(FVID2_VHWA_M2M_DOF_DRV_ID,
            VHWA_M2M_DOF_DRV_INST_ID, (void *)&appObj->createArgs,
            NULL, &appObj->cbPrms);
    }
    else
    {
        Vhwa_print (" Could not Create Semaphore \n");
    }

    if (NULL != appObj->handle)
    {
        status = FVID2_SOK;
    }
    else
    {
        status = FVID2_EFAIL;
    }

    return (status);
}


void AppDof_Delete(DofApp_TestParams *tObj, uint32_t hidx)
{
    AppDof_TestObject      *appObj = &gAppDofTestObj[hidx];

    if (NULL != appObj->handle)
    {
        Fvid2_delete(appObj->handle, NULL);
    }

    #if defined VHWA_PDK
    if (NULL != appObj->waitForProcessCmpl)
    {
        SemaphoreP_delete(appObj->waitForProcessCmpl);
    }
    #else
    SemaphoreP_destruct(&appObj->waitForProcessCmpl);
    #endif
}

int32_t AppDof_SetParams(DofApp_TestParams *tObj, uint32_t hidx)
{
    int32_t status;
    uint32_t pyrLvl, tempPyrLvl;
    uint32_t tempWidth;
    AppDof_TestConfig *tCfg;
    Vhwa_M2mDofPrms *dofPrms;
    AppDof_TestObject  *appObj = &gAppDofTestObj[hidx];
    Dof_ErrEventParams  errPrms;
    Dof_WdTimerErrEventParams  wdTimerErrEvtPrms;
    Vhwa_HtsLimiter  htsLimitEVTPrms;

    tCfg = tObj->testCfg[hidx];
    dofPrms = &appObj->dofPrms;

    appObj->curPyrLvl = tCfg->tPyrLvl;

    dofPrms->tPrmdLvl = tCfg->tPyrLvl;
    dofPrms->skipTempPredLine = tCfg->skipTempPredLine;
    dofPrms->flowVectorHeight = tCfg->fvHeight;
    dofPrms->tPredictor  = tCfg->tPredictor;
    dofPrms->mPredictor1 = tCfg->mPredictor1;
    dofPrms->mPredictor2 = tCfg->mPredictor2;
    dofPrms->bPredictor1 = tCfg->bPredictor1;
    dofPrms->bPredictor2 = tCfg->bPredictor2;

    dofPrms->coreCfg.enableDOF   = tCfg->enableDOF;
    dofPrms->coreCfg.width   = tCfg->width;
    dofPrms->coreCfg.height   = tCfg->height;
    dofPrms->coreCfg.horizontalSearchRange   = tCfg->HorizontalSearchRange;
    dofPrms->coreCfg.topSearchRange   = tCfg->TopSearchRange;
    dofPrms->coreCfg.bottomSearchRange   = tCfg->BottomSearchRange;
    dofPrms->coreCfg.medianFilter   = tCfg->medianFilter;
    dofPrms->coreCfg.currentCensusTransform   = tCfg->currentCensusTransform;
    dofPrms->coreCfg.referenceCensusTransform   = tCfg->referenceCensusTransform;
    dofPrms->coreCfg.maxMVsInSof   = tCfg->maxMVsInSof;
    dofPrms->coreCfg.motionSmoothnessFactor   = tCfg->motionSmoothnessFactor;
    dofPrms->coreCfg.iirFilterAlpha   = tCfg->iirFilterAlpha;

    dofPrms->coreCfg.lkConfidanceScore   = tCfg->LkConfidanceScore;
    dofPrms->coreCfg.enableSof   = tCfg->enableSof;

    for(pyrLvl=0; pyrLvl < tCfg->tPyrLvl; pyrLvl++)
    {
        tempPyrLvl = pyrLvl;
        tempWidth = tCfg->width;

        while(tempPyrLvl-- > 0)
        {
            tempWidth = tempWidth/2U;
        }

        dofPrms->inOutImgFmt[pyrLvl][DOF_INPUT_CURRENT_IMG].pitch[0U] =
                                                    tCfg->curPitch[pyrLvl];
        dofPrms->inOutImgFmt[pyrLvl][DOF_INPUT_CURRENT_IMG].ccsFormat =
                                                    tCfg->strFmt[pyrLvl];
        dofPrms->inOutImgFmt[pyrLvl][DOF_INPUT_REFERENCE_IMG].pitch[0U] =
                                                    tCfg->refPitch[pyrLvl];
        dofPrms->inOutImgFmt[pyrLvl][DOF_INPUT_REFERENCE_IMG].ccsFormat =
                                                    tCfg->strFmt[pyrLvl];;
        if(0 ==pyrLvl)
        {
            dofPrms->inOutImgFmt[pyrLvl][DOF_INPUT_TEMPORAL_PRED].pitch[0U] =
                                                        tCfg->temPredPitch;
            dofPrms->inOutImgFmt[pyrLvl][DOF_INPUT_SOF].pitch[0U] =
                                                        tCfg->sofPitch;
        }
        else
        {
            dofPrms->inOutImgFmt[pyrLvl][DOF_INPUT_TEMPORAL_PRED].pitch[0U] = 0U;
            dofPrms->inOutImgFmt[pyrLvl][DOF_INPUT_SOF].pitch[0U] = 0U;
        }

        dofPrms->inOutImgFmt[pyrLvl][DOF_INPUT_PYRAMID_PRED].pitch[0U] =
                                    (tempWidth + 15u) & 0xFFFFFFF0u;

        if(0U == pyrLvl)
        {
            dofPrms->inOutImgFmt[pyrLvl][DOF_OUTPUT].pitch[0U] =
                                                        tCfg->outPitch;
        }
        else
        {
            /* Needed due to an bug in Custom IP Zebu */
            dofPrms->inOutImgFmt[pyrLvl][DOF_OUTPUT].pitch[0U] =
                                    ((tempWidth * 2U) + 15u) & 0xFFFFFFF0u;
        }

        if(tCfg->strFmt[pyrLvl] == FVID2_CCSF_BITS12_UNPACKED16 ||
           tCfg->strFmt[pyrLvl] == FVID2_CCSF_BITS8_PACKED)
        {
            dofPrms->focoPrms.shiftM1 = tCfg->focoCfg.shiftM1;
            dofPrms->focoPrms.dir = tCfg->focoCfg.dir;
            dofPrms->focoPrms.round = tCfg->focoCfg.round;
        }
    }

    status = Fvid2_control(appObj->handle, VHWA_M2M_IOCTL_DOF_SET_PARAMS,
        dofPrms, NULL);

    if (FVID2_SOK == status)
    {
        /* Mask out watchdog timer events for regular error callback */
        errPrms.errEvents = tCfg->enableErrorEvents & ~VHWA_DOF_ALL_WDTIMER_ERR_EVENTS;

        errPrms.cbFxn = AppDofErrorCb;

        errPrms.appData = appObj;

        status = Fvid2_control(appObj->handle,
            VHWA_M2M_IOCTL_DOF_REGISTER_ERR_CB, &errPrms, NULL);
    }
    if (FVID2_SOK == status)
    {
        wdTimerErrEvtPrms.wdTimeoutCycles = tCfg->wdTimeoutCycles;
        wdTimerErrEvtPrms.focoWdTimeoutCycles = tCfg->focoWdTimeoutCycles;
        /* Mask to only watchdog timer events */
        wdTimerErrEvtPrms.wdTimerErrEvents = tCfg->enableErrorEvents & VHWA_DOF_ALL_WDTIMER_ERR_EVENTS;
        wdTimerErrEvtPrms.cbFxn = AppDofWdtimerErrorCb;
        wdTimerErrEvtPrms.appData = appObj;
        status = Fvid2_control(appObj->handle,
            VHWA_M2M_IOCTL_DOF_REGISTER_WDTIMER_ERR_CB, &wdTimerErrEvtPrms, NULL);
    }

    if (FVID2_SOK == status)
    {
        htsLimitEVTPrms.enableBwLimit = 1;  // Enable the bandwidth limiter (set to 1 for enabled, 0 for disabled)
        htsLimitEVTPrms.cycleCnt = 100;     // Example value, set appropriate cycle count
        htsLimitEVTPrms.tokenCnt = 10;      // Example token count, set based on requirements
    
 
        status = Fvid2_control(appObj->handle,
            VHWA_M2M_IOCTL_DOF_SET_HTS_LIMIT, &htsLimitEVTPrms, NULL);
    }

    return (status);
}


int32_t AppDof_AllocBuffers(DofApp_TestParams *tObj, uint32_t hidx,
                                    AppDof_BuffAddr *buffAddr)
{
    uint32_t pyrLvl = 0;
    uint32_t height;
    uint32_t curBufOffset = 0;
    uint32_t refBufOffset = 0;
    AppDof_TestObject  *appObj = &gAppDofTestObj[hidx];
    AppDof_TestConfig  *tCfg = tObj->testCfg[hidx];

    height = tCfg->height;

    for(pyrLvl = 0; pyrLvl < tCfg->tPyrLvl; pyrLvl++)
    {
        appObj->inFrm[pyrLvl][DOF_INPUT_CURRENT_IMG].addr[0] =
                     (uint64_t)(buffAddr->currBuff + curBufOffset);
        appObj->inFrm[pyrLvl][DOF_INPUT_REFERENCE_IMG].addr[0] =
                        (uint64_t)(buffAddr->refBuff + refBufOffset);

        if(pyrLvl == 0)
        {
            /* Base pyramid Level */
            appObj->inFrm[pyrLvl][DOF_INPUT_TEMPORAL_PRED].addr[0] =
                        (uint64_t)buffAddr->tempBuff;
            appObj->inFrm[pyrLvl][DOF_INPUT_SOF].addr[0] =
                        (uint64_t)buffAddr->sofBuff;
            appObj->inFrm[pyrLvl][DOF_INPUT_PYRAMID_PRED].addr[0] =
                        (uint64_t)buffAddr->interOddBuff;
            appObj->outFrm[pyrLvl].addr[0] =
                        (uint64_t)buffAddr->outBuff;
        }
        else
        {
            appObj->inFrm[pyrLvl][DOF_INPUT_TEMPORAL_PRED].addr[0] =
                        (uint64_t)0U;
            appObj->inFrm[pyrLvl][DOF_INPUT_SOF].addr[0] =
                        (uint64_t)0U;
            if(pyrLvl % 2 == 0)
            {
                appObj->inFrm[pyrLvl][DOF_INPUT_PYRAMID_PRED].addr[0] =
                        (uint64_t)buffAddr->interOddBuff;;
                appObj->outFrm[pyrLvl].addr[0] =
                        (uint64_t)buffAddr->interEvenBuff;;
            }
            else
            {
                appObj->inFrm[pyrLvl][DOF_INPUT_PYRAMID_PRED].addr[0] =
                        (uint64_t)buffAddr->interEvenBuff;;
                appObj->outFrm[pyrLvl].addr[0] =
                        (uint64_t)buffAddr->interOddBuff;;
            }
        }

        refBufOffset += height * tCfg->refPitch[pyrLvl];
        curBufOffset += height * tCfg->curPitch[pyrLvl];

        height = height/2;
    }

    Vhwa_print(" DOF Buffers \n");
	Vhwa_print(" All the pyramid buffers should be in continues memory ");
	Vhwa_print(" starting with base buffer \n");
    Vhwa_print("Ref Buff 0x%x\nCurr Buff 0x%x\n",
            (uint32_t)appObj->inFrm[0][DOF_INPUT_REFERENCE_IMG].addr[0],
            (uint32_t)appObj->inFrm[0][DOF_INPUT_CURRENT_IMG].addr[0]);
    Vhwa_print("Temp Buff 0x%x\nSOF Buff 0x%x\n",
            (uint32_t)appObj->inFrm[0][DOF_INPUT_TEMPORAL_PRED].addr[0],
            (uint32_t)appObj->inFrm[0][DOF_INPUT_SOF].addr[0]);
    Vhwa_print("OUT Buff 0x%x\n",
            (uint32_t)appObj->outFrm[0].addr[0]);

    *(buffAddr->refBuffIdx) += refBufOffset;
    *(buffAddr->currBuffIdx) += curBufOffset;
    if(tCfg->bPredictor1 == DOF_PREDICTOR_TEMPORAL ||
       tCfg->bPredictor2 == DOF_PREDICTOR_TEMPORAL)
    {
        *(buffAddr->tempBuffIdx) += tCfg->height * tCfg->temPredPitch;
    }
    if(tCfg->enableSof == UTRUE)
    {
        *(buffAddr->sofBuffIdx) += tCfg->sofPitch * tCfg->height;
    }
    *(buffAddr->outBuffIdx) += tCfg->height * tCfg->outPitch;


    return (FVID2_SOK);
}

int32_t AppDof_SetConfScoreParam(DofApp_TestParams *tObj, uint32_t hidx)
{
    int32_t status;
    AppDof_TestObject  *appObj = &gAppDofTestObj[hidx];

    Vhwa_m2mConfScoreParamInit(&tObj->dofConfScore[hidx]);

    status = Fvid2_control(appObj->handle, VHWA_M2M_IOCTL_DOF_SET_CONF_SCORE_PARAMS,
        &tObj->dofConfScore[hidx], NULL);

    return (status);
}

#if defined VHWA_PDK
int32_t AppDof_CrcInit(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = FVID2_SOK;

    gDofCrcChHandle.crcChannel = APP_DOF_CRC_CHANNEL;
    gDofCrcChHandle.udmaCrcChHandle = App_crcCreate(udmaDrvHndl,
                                                 &gDofCrcChHandle.crcChannel);
    if(gDofCrcChHandle.udmaCrcChHandle == NULL)
    {
        Vhwa_print(" App_crcCreate Error\n");
        status = FVID2_EFAIL;
    }

    return status;
}

int32_t AppDof_CrcDeinit(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = FVID2_SOK;

    App_crcDelete(udmaDrvHndl, &gDofCrcChHandle);

    return status;
}


static int32_t AppDof_CompareCrc(DofApp_TestParams *tObj,
                                 uint32_t hidx)
{
    int32_t status = FVID2_SOK;
    uint64_t crcSignVal = 0;
    AppDof_TestConfig *tCfg;
    AppCrc_inPrms srcPrms;
    AppDof_TestObject  *appObj = &gAppDofTestObj[hidx];


    tCfg = tObj->testCfg[hidx];

    if (UTRUE == tCfg->isCrcAvail)
    {
        if(tCfg->enableSof == UTRUE)
        {
            srcPrms.width = tCfg->maxMVsInSof*4;
            srcPrms.pitch = tCfg->outPitch;
            srcPrms.height = tCfg->fvHeight;
        }
        else
        {
            srcPrms.width = tCfg->width * 4;
            srcPrms.pitch = tCfg->outPitch;
            srcPrms.height = tCfg->height;
        }

        status = App_getCrc(&gDofCrcChHandle,
                            appObj->outFrm[0].addr[0],
                            &srcPrms, &crcSignVal);
        if (FVID2_SOK == status)
        {
            if (crcSignVal != tCfg->crcSign)
            {
                status = FVID2_EFAIL;
            }
        }
    }

    return (status);
}
#endif
void AppDof_PrepareRequest(DofApp_TestParams *tObj, uint32_t hidx)
{
    uint32_t cnt;
    Fvid2_FrameList *inFrmList;
    Fvid2_FrameList *outFrmList;
    AppDof_TestObject *appObj = &gAppDofTestObj[hidx];

    for(cnt = 0; cnt < tObj->testCfg[hidx]->tPyrLvl; cnt++)
    {
        inFrmList = &appObj->inFrmList[cnt];
        outFrmList = &appObj->outFrmList[cnt];

        /* Initialize SDE Input Frame List */
        inFrmList->frames[DOF_INPUT_REFERENCE_IMG] =
                                &appObj->inFrm[cnt][DOF_INPUT_REFERENCE_IMG];
        inFrmList->frames[DOF_INPUT_CURRENT_IMG] =
                                &appObj->inFrm[cnt][DOF_INPUT_CURRENT_IMG];
        inFrmList->frames[DOF_INPUT_TEMPORAL_PRED] =
                                &appObj->inFrm[cnt][DOF_INPUT_TEMPORAL_PRED];
        inFrmList->frames[DOF_INPUT_PYRAMID_PRED] =
                                &appObj->inFrm[cnt][DOF_INPUT_PYRAMID_PRED];
        inFrmList->frames[DOF_INPUT_SOF] = &appObj->inFrm[cnt][DOF_INPUT_SOF];
        inFrmList->numFrames = 5U;

        /* Initialize SDE Output Frame List */
        outFrmList->frames[0U] = &appObj->outFrm[cnt];
        outFrmList->numFrames = 1U;
    }
}

int32_t AppDof_SubmitRequest(DofApp_TestParams *tObj, uint32_t hidx)
{
    int32_t status;
    AppDof_TestObject *appObj = &gAppDofTestObj[hidx];

    if(0 == appObj->curPyrLvl)
    {
        appObj->curPyrLvl = tObj->testCfg[hidx]->tPyrLvl - 1;
    }
    else
    {
        appObj->curPyrLvl--;
    }

    /* Set pyramid level to be processed */
    status = Fvid2_control(appObj->handle,
            VHWA_M2M_IOCTL_DOF_SET_NEXT_PYR, &appObj->curPyrLvl, NULL);

    if(FVID2_SOK == status)
    {
        /* Submit Request*/
        status = Fvid2_processRequest(appObj->handle,
            &appObj->inFrmList[appObj->curPyrLvl],
            &appObj->outFrmList[appObj->curPyrLvl],
            FVID2_TIMEOUT_FOREVER);
    }

    if (FVID2_SOK != status)
    {
        Vhwa_print
            (" DOF: Failed to submit new Frame returned %x for DOF Handle # %d\n",
            status, hidx);
    }

    return (status);
}

int32_t AppDof_WaitForComplRequest(DofApp_TestParams *tObj, uint32_t hidx)
{
    int32_t            status;
    Fvid2_FrameList    *inFrmList;
    Fvid2_FrameList    *outFrmList;
    AppDof_TestObject  *appObj = &gAppDofTestObj[hidx];
    uint32_t            psa[tObj->testCfg[hidx]->tPyrLvl];

    VhwaAl_SemaphoreP_pend(appObj->waitForProcessCmpl, SYSTEMP_WAIT_FOREVER);

    inFrmList = &appObj->inFrmList[appObj->curPyrLvl];
    outFrmList = &appObj->outFrmList[appObj->curPyrLvl];

    if((0u == appObj->wdTimerErrStatus) && (0u == appObj->err_stat))
    {
        status = Fvid2_getProcessedRequest(appObj->handle,
            inFrmList, outFrmList, 0);
        if (FVID2_SOK == status)
        {
            if(0 == appObj->curPyrLvl)
            {
                #if defined VHWA_PDK
                status = AppDof_CompareCrc(tObj, hidx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print (" DOF: CRC Check Failed Handle Cnt %d\n", hidx);
                }
                #endif
            }
        }
        if (FVID2_SOK == status)
        {
            if(UTRUE == tObj->testCfg[hidx]->isPsa)
            {
                status = Fvid2_control(appObj->handle,
                    VHWA_M2M_IOCTL_DOF_GET_PSA_SIGN,
                    &psa[tObj->testCfg[hidx]->tPyrLvl], NULL);
                if (FVID2_SOK == status)
                {
                    if ((0u != tObj->testCfg[hidx]->psaSign[tObj->testCfg[hidx]->tPyrLvl]) && 
                        (psa[tObj->testCfg[hidx]->tPyrLvl] != tObj->testCfg[hidx]->psaSign[tObj->testCfg[hidx]->tPyrLvl]))
                    {
                        Vhwa_print(" PSA mismatch error\n");
                        status = FVID2_EFAIL;
                    }

                }
                else
                {
                    Vhwa_print(" Get PSA Error\n");
                }
            }
        }

        if (FVID2_SOK == status)
        {
            if((tObj->testCfg[hidx]->isHistogram) && (0 == appObj->curPyrLvl))
            {
                status = Fvid2_control(appObj->handle, VHWA_M2M_IOCTL_DOF_GET_HISTOGRAM,
                            appObj->csHistogram, NULL);
                if(FVID2_SOK != status)
                {
                    Vhwa_print (" DOF: getHistogram failed Handle Cnt %d, status = %x\n",
                                hidx, status);
                }
            }
        }
        else
        {
            Vhwa_print (" DOF: Failed DOF Handle Cnt %d, Pyramid %d; status = %x\n",
                hidx, appObj->curPyrLvl,  status);
        }
    }
    else
    {
        if(0u != appObj->wdTimerErrStatus)
        {
            Vhwa_print("IP stall: Watchdog timer error interrupt triggered \n");
            status = FVID2_ETIMEOUT;
        }
        if(0u != appObj->err_stat)
        {
            Vhwa_print("Error interrupt: DOF error interrupt triggered \n");
            status = FVID2_EFAIL;
        }
    }

    return (status);
}

void AppDof_SyncStart(DofApp_TestParams *tObj, uint32_t hidx)
{
    uint32_t status;
    uint32_t pryLvl = tObj->testCfg[hidx]->tPyrLvl - 1;
    AppDof_TestObject  *appObj = &gAppDofTestObj[hidx];

    status = Fvid2_control(appObj->handle,
                            VHWA_M2M_IOCTL_DOF_SYNC_START,
                            &pryLvl, NULL);
    if(FVID2_SOK != status)
    {
        Vhwa_print(" DOF Sync Start Failed \n");
    }
}
