/**
 *   Copyright (c) Texas Instruments Incorporated 2018
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
 */

/**
*  \file vhwa_msc_api.c
*
*  \brief VHWA MSC sample application
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <vhwa_msc.h>
#include <examples/include/vhwa_msc_api.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
#if defined(VHWA_PDK)
static int32_t AppMsc_CompareCrc(App_MscTestParams *tObj, uint32_t hIdx);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
#if defined(VHWA_PDK)
AppCrc_hdlPrms gMscCrcChHandle;
#endif
App_MscTestObj gAppMscTestObj[APP_MSC_MAX_INSTANCES][APP_MSC_MAX_HANDLES];

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t AppMsc_Init(Udma_DrvHandle drvHandle)
{
    int32_t status = FVID2_EBADARGS;
    uint32_t cnt, idx;
    Vhwa_M2mMscInitParams initPrms;
    Vhwa_M2mMscSl2AllocPrms sl2Prms;
    #if defined (SOC_J742S2)
    /* Powerup VPAC0 module */
    Sciclient_pmSetModuleState (TISCI_DEV_VPAC0,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
            TISCI_MSG_FLAG_AOP,
            0xFFFFFFFFU);
    /* Powerup VPAC1 module */
    Sciclient_pmSetModuleState (TISCI_DEV_VPAC1,
            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
            TISCI_MSG_FLAG_AOP,
            0xFFFFFFFFU);
    #endif
    Vhwa_m2mMscInitParamsInit(&initPrms);

    initPrms.drvHandle = drvHandle;

    status = Vhwa_m2mMscInit(&initPrms);

    if (FVID2_SOK == status)
    {
        for(cnt = 0; cnt < VHWA_M2M_MSC_MAX_INST; cnt++)
        {
            for(idx = 0; idx < VHWA_M2M_MSC_MAX_IN_CHANNEL; idx++)
            {
                sl2Prms.maxInWidth[cnt][idx]    = APP_MAX_IN_IMG_WIDTH;
                sl2Prms.inCcsf[cnt][idx]        = APP_IN_IMG_CCSF;
                sl2Prms.inBuffDepth[cnt][idx]   = APP_MAX_IN_IMG_BUFF_DEPTH;
            }
        }

        for(cnt = 0; cnt < MSC_MAX_OUTPUT; cnt++)
        {
            sl2Prms.maxOutWidth[cnt]   = APP_MAX_OUT_IMG_WIDTH;
            sl2Prms.outCcsf[cnt]       = APP_OUT_IMG_CCSF;
            sl2Prms.outBuffDepth[cnt]  = APP_MAX_OUT_IMG_BUFF_DEPTH;
        }
        status = Vhwa_m2mMscAllocSl2(&sl2Prms);
    }

    if(FVID2_SOK != status)
    {
        Vhwa_print(" MSC_TEST_APP: MSC Init Failed\n");
    }

    return (status);
}

int32_t AppMsc_DeInit(void)
{
    int32_t retVal;

    retVal = Vhwa_m2mMscDeInit();

    return (retVal);
}

int32_t AppMscFrameComplCb0(Fvid2_Handle handle, void *appData)
{
    App_MscTestObj *tObj = (App_MscTestObj *)appData;

    if (NULL != tObj)
    {
        VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
    }

    return FVID2_SOK;
}

int32_t AppMscFrameComplCb1(Fvid2_Handle handle, void *appData)
{
    App_MscTestObj *tObj = (App_MscTestObj *)appData;

    if (NULL != tObj)
    {
        VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
    }

    return FVID2_SOK;
}

void AppMscErrorCb(Fvid2_Handle handle, uint32_t errEvents, void *appData)
{
    App_MscTestObj *tObj = (App_MscTestObj *)appData;

    if (NULL != tObj)
    {
        tObj->err_stat |= errEvents;
        if(0u != tObj->err_stat)
        {
            VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
        }
    }
}
void AppMscWdtimerErrorCb(Fvid2_Handle handle, uint32_t wdTimerErrEvents, void *appData)
{
    App_MscTestObj *tObj = (App_MscTestObj *)appData;

    if (NULL != tObj)
    {
        tObj->wdTimerErrStatus |= wdTimerErrEvents;
        if(0u != tObj->wdTimerErrStatus)
        {
            VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
        }
    }
}
int32_t AppMsc_Create(App_MscTestParams *tObj, uint32_t hndlIdx)
{
    int32_t             status;
    App_MscTestObj *appObj = &gAppMscTestObj[tObj->testCfg[hndlIdx]->mscThreadId][hndlIdx];

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
    if (SYSTEMP_SUCCESS == status)
    #endif
    {
        if (UTRUE == tObj->testCfg[hndlIdx]->isLsePsaAvail)
        {
            appObj->mscPrms[hndlIdx].enablePsa = UTRUE;
        }
        else
        {
            appObj->mscPrms[hndlIdx].enablePsa = UFALSE;
        }

        if(tObj->testCfg[hndlIdx]->mscThreadId == (uint32_t)VPAC_MSC_TEST_INST_ID_0)
        {
            appObj->cbPrms.cbFxn   = AppMscFrameComplCb0;
            appObj->cbPrms.appData = appObj;
        }
        else
        {
            appObj->cbPrms.cbFxn   = AppMscFrameComplCb1;
            appObj->cbPrms.appData = appObj;
        }
        #if defined(VHWA_PDK)
        if(tObj->isPerformanceTest)
        {
            appObj->createArgs.getTimeStamp = Vhwa_getTime;
        }
        #endif
        appObj->handle = Fvid2_create(FVID2_VHWA_M2M_MSC_DRV_ID,
                            tObj->testCfg[hndlIdx]->mscThreadId,
                            &appObj->createArgs,
                            NULL,
                            &appObj->cbPrms);
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

void AppMsc_Delete(App_MscTestParams *tObj, uint32_t hndlIdx)
{
    App_MscTestObj *appObj = &gAppMscTestObj[tObj->testCfg[hndlIdx]->mscThreadId][hndlIdx];

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

int32_t AppMsc_SetParams(App_MscTestParams *tObj, uint32_t hndlIdx)
{
    int32_t status, cnt;
    App_MscTestCfg *tCfg = tObj->testCfg[hndlIdx];
    App_MscTestObj *appObj = &gAppMscTestObj[tObj->testCfg[hndlIdx]->mscThreadId][hndlIdx];
    Vhwa_M2mMscParams *mscPrms;
    Msc_ScConfig *scCfg;
    Msc_ErrEventParams  errPrms;
    Msc_WdTimerErrEventParams  wdTimerErrEvtPrms;
    uint32_t wdTimerErrorMask;

    mscPrms = &appObj->mscPrms[hndlIdx];

    mscPrms->loopBack = tCfg->loopBack;

    mscPrms->isEnableSimulProcessing = tCfg->isEnableSimulProcessing;
    mscPrms->enableLineSkip = tCfg->inFrm.skipInputLine;
    mscPrms->inFmt.width = tCfg->inFrm.inWidth;
    mscPrms->inFmt.height = tCfg->inFrm.inHeight;
    mscPrms->inFmt.dataFormat = tCfg->inFrm.inDataFmt;
    mscPrms->inFmt.ccsFormat = tCfg->inFrm.inCcsf;
    mscPrms->inFmt.pitch[0U] = tCfg->inFrm.inPitch;
    mscPrms->mscCfg.tapSel = tCfg->inFrm.tapSel;

#if defined VHWA_VPAC_IP_REV_VPAC3 || defined VHWA_VPAC_IP_REV_VPAC3L
    mscPrms->secChPrms.enable = tCfg->inSecFrm.enable;
    mscPrms->secChPrms.pitch = tCfg->inSecFrm.inPitch;
    mscPrms->secChPrms.ccsf = tCfg->inSecFrm.inCcsf;
#endif

    for(cnt = 0; cnt < MSC_MAX_OUTPUT;cnt++)
    {
        scCfg = &mscPrms->mscCfg.scCfg[cnt];
        if(tCfg->mscCfgPrms[cnt].enable == UTRUE)
        {
            scCfg->enable = UTRUE;

            mscPrms->outFmt[cnt].width = tCfg->outFrm[cnt].outWidth;
            mscPrms->outFmt[cnt].height = tCfg->outFrm[cnt].outHeight;
            mscPrms->outFmt[cnt].dataFormat = tCfg->outFrm[cnt].outDataFmt;
            mscPrms->outFmt[cnt].ccsFormat = tCfg->outFrm[cnt].outCcsf;
            mscPrms->outFmt[cnt].pitch[0U] = tCfg->outFrm[cnt].outPitch;

            scCfg->inRoi.cropStartX = tCfg->mscCfgPrms[cnt].inPos.startX;
            scCfg->inRoi.cropStartY = tCfg->mscCfgPrms[cnt].inPos.startY;
            scCfg->inRoi.cropWidth = tCfg->outFrm[cnt].roiWidth;
            scCfg->inRoi.cropHeight = tCfg->outFrm[cnt].roiHeight;
            scCfg->outWidth = tCfg->outFrm[cnt].outWidth;
            scCfg->outHeight = tCfg->outFrm[cnt].outHeight;
            scCfg->horzAccInit = tCfg->mscCfgPrms[cnt].horzAccInit;
            scCfg->vertAccInit = tCfg->mscCfgPrms[cnt].vertAccInit;
            scCfg->filtMode = tCfg->mscCfgPrms[cnt].filtMode;
            scCfg->phaseMode = tCfg->mscCfgPrms[cnt].phaseMode;
            scCfg->hsSpCoeffSel = tCfg->mscCfgPrms[cnt].hsSpCoeffSel;
            scCfg->vsSpCoeffSel = tCfg->mscCfgPrms[cnt].vsSpCoeffSel;
            scCfg->hsMpCoeffSel = tCfg->mscCfgPrms[cnt].hsMpCoeffSel;
            scCfg->vsMpCoeffSel = tCfg->mscCfgPrms[cnt].vsMpCoeffSel;
            scCfg->coeffShift = tCfg->mscCfgPrms[cnt].coeffShift;
            scCfg->isSignedData = tCfg->mscCfgPrms[cnt].isSignedData;
            scCfg->isEnableFiltSatMode = tCfg->mscCfgPrms[cnt].isEnableFiltSatMode;
        }
        else
        {
            scCfg->enable = UFALSE;
        }
    }

    mscPrms->loopBack = tCfg->loopBack;

    status = Fvid2_control(appObj->handle, VHWA_M2M_IOCTL_MSC_SET_PARAMS,
                            mscPrms, NULL);
    if (FVID2_SOK == status)
    {
        /* Mask out watchdog timer events for regular error callback */
        errPrms.errEvents = tCfg->enableErrorEvents & ~(VHWA_MSC0_WDTIMER_ERR | VHWA_MSC1_WDTIMER_ERR);

        errPrms.cbFxn = AppMscErrorCb;

        errPrms.appData = appObj;

        status = Fvid2_control(appObj->handle,
            VHWA_M2M_IOCTL_MSC_REGISTER_ERR_CB, &errPrms, NULL);
    }

    if (FVID2_SOK == status)
    {
        wdTimerErrorMask = (VPAC_MSC_TEST_INST_ID_0 == tCfg->mscThreadId) ?
        VHWA_MSC0_WDTIMER_ERR : VHWA_MSC1_WDTIMER_ERR;

        wdTimerErrEvtPrms.wdTimerErrEvents = tCfg->enableErrorEvents & wdTimerErrorMask;
        wdTimerErrEvtPrms.cbFxn = AppMscWdtimerErrorCb;
        wdTimerErrEvtPrms.appData = appObj;

        status = Fvid2_control(appObj->handle,
            VHWA_M2M_IOCTL_MSC_REGISTER_WDTIMER_ERR_CB, &wdTimerErrEvtPrms, NULL);
    }

    return (status);
}

int32_t AppMsc_AllocBuffers(App_MscTestParams *tObj, uint32_t hndlIdx,
    uint64_t srcBuf, uint32_t *srcBufSize, uint32_t srcOffset,
    uint64_t dstBuf, uint32_t *dstBufSize, uint32_t dstOffset)
{
    uint32_t  cnt;
    App_MscTestCfg *tCfg = tObj->testCfg[hndlIdx];
    App_MscTestObj *appObj = &gAppMscTestObj[tObj->testCfg[hndlIdx]->mscThreadId][hndlIdx];

    /* Change to allocation based on image size after bring up */
    *srcBufSize = 0u;

    appObj->inFrm.addr[0U] = srcBuf;
    appObj->inFrm.addr[1U] = (srcBuf +
                        (tCfg->inFrm.inPitch * tCfg->inFrm.inHeight));

    srcBuf += srcOffset;
    *srcBufSize += srcOffset;

    Vhwa_print(" MSC Input BufferAddress 0x%x, 0x%x\n",
            (uint32_t)appObj->inFrm.addr[0U],
            (uint32_t)appObj->inFrm.addr[1U]);

    *dstBufSize = 0u;
    

    for(cnt = 0; cnt < MSC_MAX_OUTPUT; cnt++)
    {
        if(tCfg->mscCfgPrms[cnt].enable == UTRUE)
        {
            GT_assert(VhwaMscTrace, (((tCfg->outFrm[cnt].outPitch *
                        tCfg->outFrm[cnt].outHeight * 3) / 2) <= dstOffset));

            appObj->outFrm[cnt].addr[0U] = dstBuf;

            if(FVID2_DF_YUV420SP_UV == tCfg->outFrm[cnt].outDataFmt ||
               FVID2_DF_YUV420SP_VU == tCfg->outFrm[cnt].outDataFmt)
            {
                appObj->outFrm[cnt].addr[1U] = (dstBuf +
                        (tCfg->outFrm[cnt].outPitch * tCfg->outFrm[cnt].outHeight));

            }
            *dstBufSize += dstOffset;
            dstBuf += dstOffset;

            Vhwa_print(" MSC Output BufferAddress 0x%x, 0x%x\n",
                    (uint32_t)appObj->outFrm[cnt].addr[0U],
                    (uint32_t)appObj->outFrm[cnt].addr[1U]);

        }
    }

    return (FVID2_SOK);
}

int32_t AppMsc_SetCoeff(App_MscTestParams *tObj, uint32_t hndlIdx,
                        Msc_Coeff * coefTbl)
{
    int32_t status;
    App_MscTestObj *appObj = &gAppMscTestObj[tObj->testCfg[hndlIdx]->mscThreadId][hndlIdx];

    status = Fvid2_control(appObj->handle, VHWA_M2M_IOCTL_MSC_SET_COEFF,
        coefTbl, NULL);

    return (status);
}
#if defined VHWA_PDK
int32_t AppMsc_CrcInit(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = FVID2_SOK;

    gMscCrcChHandle.crcChannel = APP_MSC_CRC_CHANNEL;
    gMscCrcChHandle.udmaCrcChHandle = App_crcCreate(udmaDrvHndl,
                                                 &gMscCrcChHandle.crcChannel);
    if(gMscCrcChHandle.udmaCrcChHandle == NULL)
    {
        Vhwa_print(" App_crcCreate Error\n");
        status = FVID2_EFAIL;
    }

    return status;
}

int32_t AppMsc_CrcDeinit(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = FVID2_SOK;

    App_crcDelete(udmaDrvHndl, &gMscCrcChHandle);

    return status;
}

static int32_t AppMsc_CompareCrc(App_MscTestParams *tObj, uint32_t hIdx)
{
    int32_t status = FVID2_SOK;
    uint64_t crcSignVal = 0;
    uint32_t outCnt;
    App_MscTestCfg *tCfg;
    AppCrc_inPrms srcPrms;
    App_MscTestObj *appObj = &gAppMscTestObj[tObj->testCfg[hIdx]->mscThreadId][hIdx];

    tCfg = tObj->testCfg[hIdx];

    if (UTRUE == tCfg->isCrcAvail)
    {
        for (outCnt = 0u; outCnt < 10u && status == FVID2_SOK; outCnt ++)
        {
            if(tCfg->mscCfgPrms[outCnt].enable == UTRUE)
            {
                srcPrms.width = Vhwa_calcHorzSizeInBytes(tCfg->outFrm[outCnt].outWidth,
                                                    tCfg->outFrm[outCnt].outCcsf);
                srcPrms.pitch = tCfg->outFrm[outCnt].outPitch;

                if(tCfg->outFrm[outCnt].outDataFmt == FVID2_DF_YUV420SP_UV ||
                    tCfg->outFrm[outCnt].outDataFmt == FVID2_DF_YUV420SP_VU)
                {
                    srcPrms.height = tCfg->outFrm[outCnt].outHeight*3/2;
                }
                else if(tCfg->outFrm[outCnt].outDataFmt == FVID2_DF_YUV422I_UYVY ||
                    tCfg->outFrm[outCnt].outDataFmt == FVID2_DF_YUV422I_YUYV ||
                    tCfg->outFrm[outCnt].outDataFmt == FVID2_DF_YUV422SP_UV ||
                    tCfg->outFrm[outCnt].outDataFmt == FVID2_DF_YUV422SP_VU)
                {
                    srcPrms.height = tCfg->outFrm[outCnt].outHeight*2;
                }
                else
                {
                    srcPrms.height = tCfg->outFrm[outCnt].outHeight;
                }

                status = App_getCrc(&gMscCrcChHandle,
                                    (uint64_t)appObj->outFrm[outCnt].addr[0U],
                                    &srcPrms, &crcSignVal);
                if (FVID2_SOK == status)
                {
                    if (crcSignVal != tCfg->crcSign[outCnt])
                    {
                        status = FVID2_EFAIL;
                    }
                }
            }
        }
    }

    return (status);
}
#endif

void AppMsc_PrepareRequest(App_MscTestParams *tObj, uint32_t hndlIdx)
{
    uint32_t            mscCnt;
    Fvid2_FrameList    *inFrmList;
    Fvid2_FrameList    *outFrmList;
    App_MscTestObj *appObj = &gAppMscTestObj[tObj->testCfg[hndlIdx]->mscThreadId][hndlIdx];

    if(UFALSE == tObj->isPerformanceTest)
    {
        App_MscTestCfg     *mCfg;
        mCfg = tObj->testCfg[hndlIdx];

        Vhwa_print(" MSC%d Handle %d, Width = %d",
            mCfg->mscThreadId, hndlIdx,
            mCfg->inFrm.inWidth);
        Vhwa_print(", Height = %d\n",
            mCfg->inFrm.inHeight);
    }
    inFrmList = &appObj->inFrmList;
    outFrmList = &appObj->outFrmList;
    inFrmList->frames[0] = &appObj->inFrm;

    outFrmList->numFrames = 0u;
    for(mscCnt = 0; mscCnt < 10; mscCnt ++)
    {
        outFrmList->frames[mscCnt] = &appObj->outFrm[mscCnt];
    }

    inFrmList->numFrames = 1U;
    outFrmList->numFrames = 10u;
}

int32_t AppMsc_SubmitRequest(App_MscTestParams *tObj, uint32_t hndlIdx)
{
    int32_t status;
    Fvid2_FrameList    *inFrmList;
    Fvid2_FrameList    *outFrmList;
    App_MscTestObj *appObj = &gAppMscTestObj[tObj->testCfg[hndlIdx]->mscThreadId][hndlIdx];

    inFrmList  = &appObj->inFrmList;
    outFrmList = &appObj->outFrmList;

    /* Submit MSC Request*/
    status = Fvid2_processRequest(appObj->handle, inFrmList, outFrmList,
                                  FVID2_TIMEOUT_FOREVER);
    if (FVID2_SOK != status)
    {
        Vhwa_print
            (" MSC: Fvid2_processRequest returned %x for MSC Handle # %d\n",
            status, hndlIdx);
    }

    return (status);
}

int32_t AppMsc_WaitForComplRequest(App_MscTestParams *tObj,
                                    uint32_t hndlIdx)
{
    int32_t status;
    Fvid2_FrameList    *inFrmList;
    Fvid2_FrameList    *outFrmList;
    Vhwa_M2mMscPsaSign psaSign;
    App_MscTestObj     *appObj = &gAppMscTestObj[tObj->testCfg[hndlIdx]->mscThreadId][hndlIdx];
    uint32_t           outCnt,compCnt;

    VhwaAl_SemaphoreP_pend(appObj->waitForProcessCmpl, SYSTEMP_WAIT_FOREVER);

    inFrmList = &appObj->inFrmList;
    outFrmList = &appObj->outFrmList;

    if((0u == appObj->wdTimerErrStatus) && (0u == appObj->err_stat))
    {
        status = Fvid2_getProcessedRequest(appObj->handle,
            inFrmList, outFrmList, 0);
        if (FVID2_SOK != status)
        {
            Vhwa_print
                (" MSC: Failed MSC Handle Cnt %d; status = %x\n", hndlIdx, status);
        }
        else
        {
            #if defined(VHWA_PDK)
            status = AppMsc_CompareCrc(tObj, hndlIdx);
            if (FVID2_SOK != status)
            {
                Vhwa_print (" MSC: CRC Check Failed Handle Cnt %d\n", hndlIdx);
            }
            #endif
            if (UTRUE == tObj->testCfg[hndlIdx]->isLsePsaAvail)
            {
                status = Fvid2_control(appObj->handle, VHWA_M2M_IOCTL_MSC_GET_PSA_SIGN,
                        &psaSign, NULL);
                if(FVID2_SOK == status)
                {
                    if ((FVID2_SOK == status) && (UTRUE == tObj->testCfg[hndlIdx]->isLsePsaAvail))
                    {
                        for (compCnt = 0u; compCnt < 2u; compCnt ++)
                        {
                            for (outCnt = 0u; outCnt < 10u; outCnt ++)
                            {
                                if ((0 != tObj->testCfg[hndlIdx]->psaSign[compCnt][outCnt]) &&
                                    (tObj->testCfg[hndlIdx]->psaSign[compCnt][outCnt] !=
                                            psaSign.psaSign[compCnt][outCnt]))
                                {
                                    Vhwa_print("MSC PSA mismatch\n");
                                    status = FVID2_EFAIL;
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {
                    Vhwa_print(" Get PSA Error\n");
                }
            }
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
            Vhwa_print("Error interrupt: MSC error interrupt triggered \n");
            status = FVID2_EFAIL;
        }
    }

    return (status);
}

void AppMsc_SyncStart(App_MscTestParams *tObj, uint32_t hidx)
{
    uint32_t status;
    App_MscTestObj  *appObj = &gAppMscTestObj[tObj->testCfg[hidx]->mscThreadId][hidx];

    status = Fvid2_control(appObj->handle,
                            VHWA_M2M_IOCTL_MSC_SYNC_START,
                            NULL, NULL);
    if(FVID2_SOK != status)
    {
        Vhwa_print(" MSC Sync Start Failed \n");
    }
}
