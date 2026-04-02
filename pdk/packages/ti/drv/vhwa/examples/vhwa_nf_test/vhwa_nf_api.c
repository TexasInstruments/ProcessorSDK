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
*  \file vhwa_nf_api.c
*
*  \brief VHWA NF APIs
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <include/vhwa_m2mNf.h>
#include <examples/include/vhwa_nf_api.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define MAX_PERF_BUFF   (20u)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
static int32_t AppNfFrameComplCb(Fvid2_Handle handle, void *appData);
static void AppNfErrorCb(Fvid2_Handle handle, uint32_t errEvents, void *appData);
static int32_t AppNf_CompareCrc(NfApp_TestParams *tObj,
                                uint32_t hidx);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

AppNf_TestObject gAppNfTestObj[APP_NF_MAX_HANDLES];

AppCrc_hdlPrms gNfCrcChHandle;

Vhwa_M2mNfPerf   gNfPerfNum[MAX_PERF_BUFF];
uint32_t         nfPerfIdx = 0;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t AppNf_Init(Udma_DrvHandle udmaDrvHndl)
{
    int32_t                 status;
    Vhwa_M2mNfSl2AllocPrms  sl2AllocPrms;
    Vhwa_M2mNfInitPrms      initPrms;
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
    /* Initialize NF Init parameters */
    Vhwa_m2mNfInitPrmsInit(&initPrms);

    /* Set UDMA driver handle */
    initPrms.udmaDrvHndl = udmaDrvHndl;

    status = Vhwa_m2mNfInit(&initPrms);
    if (FVID2_SOK != status)
    {
        Vhwa_print(" NF_TEST_APP: NF Init Failed\n");
    }
    else
    {
        /* Initilize SL2 parameters */
        sl2AllocPrms.maxImgWidth = APP_MAX_IMG_WIDTH;
        sl2AllocPrms.inCcsf = APP_INPUT_STORAGE_FORMAT;
        sl2AllocPrms.inBuffDepth = APP_MAX_SL2_IN_BUFF_DEPTH;
        sl2AllocPrms.outBuffDepth = APP_MAX_SL2_OUT_BUFF_DEPTH;
        sl2AllocPrms.outCcsf = APP_OUTPUT_STORAGE_FORMAT;

        status = Vhwa_m2mNfAllocSl2(&sl2AllocPrms);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" NF_TEST_APP: SL2 Alloc Failed !!!\n");
        }
    }

    return (status);
}

static int32_t AppNfFrameComplCb(Fvid2_Handle handle, void *appData)
{
    AppNf_TestObject *tObj = (AppNf_TestObject *)appData;

    if (NULL != tObj)
    {
        VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
    }

    return FVID2_SOK;
}

static void AppNfErrorCb(Fvid2_Handle handle, uint32_t errEvents, void *appData)
{
    AppNf_TestObject *tObj = (AppNf_TestObject *)appData;

    if (NULL != tObj)
    {
        tObj->err_stat |= errEvents;
        if(0u != tObj->err_stat)
        {
            VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
        }

    }
}

void AppNfWdtimerErrorCb(Fvid2_Handle handle, uint32_t wdTimerErrEvents, void *appData)
{
    AppNf_TestObject *tObj = (AppNf_TestObject *)appData;
    if (NULL != tObj)
    {
        tObj->wdTimerErrStatus |= wdTimerErrEvents;
        if(0u != tObj->wdTimerErrStatus)
        {
            VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
        }
    }
}

int32_t AppNf_Create(NfApp_TestParams *tObj, uint32_t hidx)
{
    int32_t             status;
    #if !defined(MCU_PLUS_SDK)    
    SemaphoreP_Params   params;
    #endif
    uint32_t            instId;
    AppNf_TestObject      *appObj = &gAppNfTestObj[hidx];

    Fvid2Utils_memset(appObj, 0x0, sizeof(AppNf_TestObject));

    SemaphoreP_Params_init(&params);
    params.mode = SemaphoreP_Mode_BINARY;
    appObj->waitForProcessCmpl = SemaphoreP_create(0U, &params);
    if (NULL != appObj->waitForProcessCmpl)
    {
        if (UTRUE == tObj->testCfg[hidx]->isLsePsaAvail)
        {
            appObj->createPrms.enablePsa = UTRUE;
        }
        else
        {
            appObj->createPrms.enablePsa = UFALSE;
        }

        if(tObj->isPerformanceTest)
        {
            appObj->createPrms.getTimeStamp = App_getTimerTicks;
        }

        appObj->cbPrms.cbFxn   = AppNfFrameComplCb;
        appObj->cbPrms.appData = appObj;

#if (VPAC_TEST_INSTANCE == 0)
        instId = VHWA_M2M_VPAC_0_NF_DRV_INST_ID_0;
#endif
#if (VPAC_TEST_INSTANCE == 1)
        instId = VHWA_M2M_VPAC_1_NF_DRV_INST_ID_0;
#endif

        appObj->handle = Fvid2_create(FVID2_VHWA_M2M_NF_DRV_ID,
            instId, (void *)&appObj->createPrms,
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


void AppNf_Delete(NfApp_TestParams *tObj, uint32_t hidx)
{
    AppNf_TestObject      *appObj = &gAppNfTestObj[hidx];

    if (NULL != appObj->handle)
    {
        Fvid2_delete(appObj->handle, NULL);
    }

    if (NULL != appObj->waitForProcessCmpl)
    {
        SemaphoreP_delete(appObj->waitForProcessCmpl);
    }
}

int32_t AppNf_SetParams(NfApp_TestParams *tObj, uint32_t hidx)
{
    int32_t status;
    AppNf_TestConfig  *tCfg;
    Vhwa_M2mNfConfig  *nfCfg;
    AppNf_TestObject  *appObj = &gAppNfTestObj[hidx];
    Nf_ErrEventParams  errPrms;
    Nf_WdTimerErrEventParams  wdTimerErrEvtPrms;
    Vhwa_HtsLimiter  htsLimitEVTPrms;

    tCfg = tObj->testCfg[hidx];
    nfCfg = &appObj->nfCfg;

    nfCfg->inFmt.width = tCfg->inWidth;
    nfCfg->inFmt.height = tCfg->inHeight;
    nfCfg->inFmt.dataFormat = tCfg->dataFormat;
    nfCfg->inFmt.pitch[0U] = tCfg->inPitch;
    nfCfg->inFmt.pitch[1U] = tCfg->inPitch;
    nfCfg->inFmt.ccsFormat = tCfg->inStrFmt;

    /* Output Format is same as input format */
    nfCfg->outFmt.width = tCfg->outWidth;
    nfCfg->outFmt.height = tCfg->outHeight;
    nfCfg->outFmt.dataFormat = tCfg->dataFormat;
    nfCfg->outFmt.pitch[0U] = tCfg->outPitch;
    nfCfg->outFmt.pitch[1U] = tCfg->outPitch;
    nfCfg->outFmt.ccsFormat = tCfg->outStrFmt;

    nfCfg->nfCfg.filterMode = tCfg->filterMode;
    nfCfg->nfCfg.tableMode = tCfg->tableMode;
    nfCfg->nfCfg.skipMode = tCfg->skipMode;

    nfCfg->nfCfg.outputShift = tCfg->outputShift;
    nfCfg->nfCfg.outputOffset = tCfg->outputOffset;
    nfCfg->nfCfg.numSubTables = tCfg->numSubTables;
    nfCfg->nfCfg.subTableIdx = tCfg->subTableIdx;
    nfCfg->nfCfg.centralPixelWeight = tCfg->centralPixelWeight;

    status = Fvid2_control(appObj->handle, IOCTL_VHWA_M2M_NF_SET_PARAMS,
                           nfCfg, NULL);

    if (FVID2_SOK == status)
    {
        /* Mask out watchdog timer events for regular error callback */
        errPrms.errEvents = tCfg->enableErrorEvents & ~VHWA_NF_WDTIMER_ERR;

        errPrms.cbFxn = AppNfErrorCb;

        errPrms.appData = appObj;

        status = Fvid2_control(appObj->handle,
                   IOCTL_VHWA_M2M_NF_REGISTER_ERR_CB, &errPrms, NULL);
    }
    if (FVID2_SOK == status)
    {
        wdTimerErrEvtPrms.wdTimeoutCycles = tCfg->wdTimeoutCycles;
        /* Mask to only watchdog timer events */
        wdTimerErrEvtPrms.wdTimerErrEvents = tCfg->enableErrorEvents & VHWA_NF_WDTIMER_ERR;
        wdTimerErrEvtPrms.cbFxn = AppNfWdtimerErrorCb;
        wdTimerErrEvtPrms.appData = appObj;
        status = Fvid2_control(appObj->handle,
            VHWA_M2M_IOCTL_NF_REGISTER_WDTIMER_ERR_CB, &wdTimerErrEvtPrms, NULL);
    }
    if (FVID2_SOK == status)
    {
        htsLimitEVTPrms.enableBwLimit = 1;  // Enable the bandwidth limiter (set to 1 for enabled, 0 for disabled)
        htsLimitEVTPrms.cycleCnt = 100;     // Example value, set appropriate cycle count
        htsLimitEVTPrms.tokenCnt = 10;      // Example token count, set based on requirements
    
 
        status = Fvid2_control(appObj->handle,
            IOCTL_VHWA_M2M_NF_SET_HTS_LIMIT, &htsLimitEVTPrms, NULL);
    }

    return (status);
}


int32_t AppNf_AllocBuffers(NfApp_TestParams *tObj, uint32_t hidx ,
                           uint64_t srcBuf, uint32_t * srcFrameSize,
                           uint64_t dstBuf, uint32_t * dstFrameSize)
{
    uint32_t chromaOffset = 0;
    uint32_t frameEnd = 0;
    Fvid2_Format *inFmt, *outFmt;
    Fvid2_Frame *inFrm, *outFrm;
    AppNf_TestObject  *appObj = &gAppNfTestObj[hidx];

    inFmt = &appObj->nfCfg.inFmt;
    inFrm = &appObj->inFrm;

    switch (inFmt->dataFormat)
    {
        case FVID2_DF_YUV420SP_UV:
            switch (inFmt->ccsFormat)
            {
                case FVID2_CCSF_BITS12_PACKED:
                    chromaOffset = (inFmt->pitch[0]) * inFmt->height;
                    frameEnd = ((inFmt->pitch[0] * inFmt->height) * 3)/2;
                    break;
                case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
                case FVID2_CCSF_BITS12_UNPACKED16:
                    chromaOffset = inFmt->pitch[0] * inFmt->height;
                    frameEnd = ((inFmt->pitch[0] * inFmt->height) * 3)/2;
                    break;
                case FVID2_CCSF_BITS8_PACKED:
                    chromaOffset = inFmt->pitch[0] * inFmt->height;
                    frameEnd = (inFmt->pitch[0] * inFmt->height * 3u) / 2u;
                    break;
            }
            break;
        case FVID2_DF_LUMA_ONLY:
            switch (inFmt->ccsFormat)
            {
                case FVID2_CCSF_BITS12_PACKED:
                    chromaOffset = 0;
                    frameEnd = (inFmt->pitch[0] * inFmt->height);
                    break;
                case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
                case FVID2_CCSF_BITS12_UNPACKED16:
                    chromaOffset = 0;
                    frameEnd = (inFmt->pitch[0] * inFmt->height);
                    break;
                case FVID2_CCSF_BITS8_PACKED:
                    chromaOffset = 0;
                    frameEnd = inFmt->pitch[0] * inFmt->height;
                    break;
            }
            break;
        case FVID2_DF_CHROMA_ONLY:
            switch (inFmt->ccsFormat)
            {
                case FVID2_CCSF_BITS8_PACKED:
                case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
                case FVID2_CCSF_BITS12_UNPACKED16:
                case FVID2_CCSF_BITS12_PACKED:
                    chromaOffset = 0;
                    frameEnd = (inFmt->pitch[0] * inFmt->height);
                    break;
            }
            break;
    }

    inFrm->addr[0U] = srcBuf;
    inFrm->addr[1U] = srcBuf + chromaOffset;

    *srcFrameSize = frameEnd;

    Vhwa_print(" NF InBuff Addr 0x%x 0x%x \n",
        (uint32_t)inFrm->addr[0U], (uint32_t)inFrm->addr[1U]);

    *dstFrameSize = 0;

    outFrm = &appObj->outFrm;
    outFmt = &appObj->nfCfg.outFmt;

    switch (outFmt->dataFormat)
    {
        case FVID2_DF_YUV420SP_UV:
            switch (outFmt->ccsFormat)
            {
                case FVID2_CCSF_BITS12_PACKED:
                    chromaOffset = (outFmt->pitch[0]) * outFmt->height;
                    frameEnd = ((outFmt->pitch[0] * outFmt->height) * 3)/2;
                    break;
                case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
                case FVID2_CCSF_BITS12_UNPACKED16:
                    chromaOffset = outFmt->pitch[0] * outFmt->height;
                    frameEnd = ((outFmt->pitch[0] * outFmt->height) * 3)/2;
                    break;
                case FVID2_CCSF_BITS8_PACKED:
                    chromaOffset = outFmt->pitch[0] * outFmt->height;
                    frameEnd = (outFmt->pitch[0] * outFmt->height * 3u) / 2u;
                    break;
            }
            break;
        case FVID2_DF_LUMA_ONLY:
            switch (outFmt->ccsFormat)
            {
                case FVID2_CCSF_BITS12_PACKED:
                    chromaOffset = 0;
                    frameEnd = (outFmt->pitch[0] * outFmt->height);
                    break;
                case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
                case FVID2_CCSF_BITS12_UNPACKED16:
                    chromaOffset = 0;
                    frameEnd = (outFmt->pitch[0] * outFmt->height);
                    break;
                case FVID2_CCSF_BITS8_PACKED:
                    chromaOffset = 0;
                    frameEnd = outFmt->pitch[0] * outFmt->height;
                    break;
            }
            break;
        case FVID2_DF_CHROMA_ONLY:
            switch (outFmt->ccsFormat)
            {
                case FVID2_CCSF_BITS8_PACKED:
                case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
                case FVID2_CCSF_BITS12_UNPACKED16:
                case FVID2_CCSF_BITS12_PACKED:
                    chromaOffset = 0;
                    frameEnd = (outFmt->pitch[0] * outFmt->height);
                    break;
            }
            break;
    }

    outFrm->addr[0U] = (dstBuf);
    outFrm->addr[1U] = (dstBuf + chromaOffset);

    /* Move Buffer Index */
    *dstFrameSize += frameEnd;
    dstBuf += frameEnd;

    Vhwa_print(" NF OutBuff Addr 0x%x 0x%x \n",
              (uint32_t)outFrm->addr[0U], (uint32_t)outFrm->addr[1U]);

    return (FVID2_SOK);
}

int32_t AppNf_SetCoeff(Nf_WgtTableConfig * wgtTbl, uint32_t hidx)
{
    int32_t status;
    AppNf_TestObject  *appObj = &gAppNfTestObj[hidx];

    status = Fvid2_control(appObj->handle, IOCTL_VHWA_M2M_NF_SET_FILTER_COEFF,
                           wgtTbl, NULL);
    return (status);
}

int32_t AppNf_CrcInit(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = FVID2_SOK;

    gNfCrcChHandle.crcChannel = APP_NF_CRC_CHANNEL;
    gNfCrcChHandle.udmaCrcChHandle = App_crcCreate(udmaDrvHndl,
                                                 &gNfCrcChHandle.crcChannel);
    if(gNfCrcChHandle.udmaCrcChHandle == NULL)
    {
        Vhwa_print(" App_crcCreate Error\n");
        status = FVID2_EFAIL;
    }

    return status;
}

int32_t AppNf_CrcDeinit(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = FVID2_SOK;

    App_crcDelete(udmaDrvHndl, &gNfCrcChHandle);

    return status;
}


static int32_t AppNf_CompareCrc(NfApp_TestParams *tObj,
                                uint32_t hidx)
{
    int32_t status = FVID2_SOK;
    uint64_t crcSignVal = 0;
    AppNf_TestConfig *tCfg;
    AppCrc_inPrms srcPrms;
    AppNf_TestObject  *appObj = &gAppNfTestObj[hidx];


    tCfg = tObj->testCfg[hidx];

    if (UTRUE == tCfg->isCrcAvail)
    {
        srcPrms.width = Vhwa_calcHorzSizeInBytes(tCfg->outWidth,
                                                tCfg->outStrFmt);
        srcPrms.pitch = tCfg->outPitch;

        if(tCfg->dataFormat == FVID2_DF_YUV420SP_UV ||
            tCfg->dataFormat == FVID2_DF_YUV420SP_VU)
        {
            srcPrms.height = tCfg->outHeight*3/2;
        }
        else
        {
            srcPrms.height = tCfg->outHeight;
        }

        status = App_getCrc(&gNfCrcChHandle,
                            appObj->outFrm.addr[0],
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

void AppNf_PrepareRequest(NfApp_TestParams *tObj, uint32_t hidx)
{
    Fvid2_FrameList *inFrmList;
    Fvid2_FrameList *outFrmList;

    AppNf_TestObject *appObj = &gAppNfTestObj[hidx];

    inFrmList = &appObj->inFrmList;
    outFrmList = &appObj->outFrmList;

    /* Initialize SDE Input Frame List */
    inFrmList->frames[0] = &appObj->inFrm;
    inFrmList->numFrames = 1U;

    /* Initialize SDE Output Frame List */
    outFrmList->frames[0U] = &appObj->outFrm;
    outFrmList->numFrames = 1U;
}

int32_t AppNf_SubmitRequest(NfApp_TestParams *tObj, uint32_t hidx)
{
    int32_t status;
    AppNf_TestObject *appObj = &gAppNfTestObj[hidx];


    /* Submit Request*/
    status = Fvid2_processRequest(appObj->handle,
                                  &appObj->inFrmList,
                                  &appObj->outFrmList,
                                  FVID2_TIMEOUT_FOREVER);

    if (FVID2_SOK != status)
    {
        Vhwa_print
            (" NF: Failed to submit new Frame returned %x for NF Handle # %d\n",
            status, hidx);
    }

    return (status);
}

int32_t AppNf_WaitForComplRequest(NfApp_TestParams *tObj, uint32_t hidx)
{
    int32_t            status;
    Fvid2_FrameList   *inFrmList;
    Fvid2_FrameList   *outFrmList;
    AppNf_TestObject  *appObj = &gAppNfTestObj[hidx];
    AppNf_TestConfig  *nfCfg = tObj->testCfg[hidx];
    Vhwa_M2mNfPsaSign  psa;
    uint32_t           cnt;

    VhwaAl_SemaphoreP_pend(appObj->waitForProcessCmpl, SYSTEMP_WAIT_FOREVER);

    inFrmList = &appObj->inFrmList;
    outFrmList = &appObj->outFrmList;

    if((0u == appObj->wdTimerErrStatus) && (0u == appObj->err_stat))
    {
        status = Fvid2_getProcessedRequest(appObj->handle,
            inFrmList, outFrmList, 0);
        if (FVID2_SOK == status)
        {
            status = AppNf_CompareCrc(tObj, hidx);
            if (FVID2_SOK != status)
            {
                Vhwa_print (" NF: CRC Check Failed Handle Cnt %d\n", hidx);
            }
            else
            {
                if (UTRUE == nfCfg->isLsePsaAvail)
                {
                    status = Fvid2_control(appObj->handle, IOCTL_VHWA_M2M_NF_GET_PSA_SIGN,
                            &psa, NULL);
                    if(FVID2_SOK == status)
                    {
                        for(cnt = 0U; cnt < VHWA_M2M_NF_MAX_COMP; cnt++)
                        {
                            if ((0u != nfCfg->psaSign[cnt]) && (psa.psaSign[cnt] != nfCfg->psaSign[cnt]))
                            {
                                Vhwa_print (" NF PSA mismatch!");
                                status = FVID2_EFAIL;
                                break;
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
            Vhwa_print (" NF: Failed NF Handle Cnt %d; status = %d\n", hidx, status);
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
            Vhwa_print("Error interrupt: NF error interrupt triggered \n");
            status = FVID2_EFAIL;
        }
    }

    return (status);
}

void AppNf_SyncStart(NfApp_TestParams *tObj, uint32_t hidx)
{
    uint32_t status;
    AppNf_TestObject  *appObj = &gAppNfTestObj[hidx];

    status = Fvid2_control(appObj->handle,
                            IOCTL_VHWA_M2M_NF_SYNC_START,
                            NULL, NULL);
    if(FVID2_SOK != status)
    {
        Vhwa_print(" NF Sync Start Failed \n");
    }
}
