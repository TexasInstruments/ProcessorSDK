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
*  \file vhwa_ldc_api.c
*
*  \brief VHWA LDC APIs
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <include/vhwa_m2mLdc.h>
#include <examples/include/vhwa_ldc_api.h>
#include "include/ldc_cfg.h"
#include <include/vhwa_common.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/*None*/

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
#if defined(VHWA_PDK)
static int32_t AppLdc_CompareCrc(LdcApp_TestParams *tObj, uint32_t hIdx);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
AppLdc_TestObject gAppLdcTestObj[APP_LDC_MAX_HANDLES];
#if defined(VHWA_PDK)
AppCrc_hdlPrms gLdcCrcChHandle;
#endif

uint16_t gLut[] = {
    #include "lut_12to8.txt"
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t AppLdc_Init(Udma_DrvHandle udmaDrvHndl)
{
    int32_t                 status;
    Vhwa_M2mLdcSl2AllocPrms sl2AllocPrms;
    Vhwa_M2mLdcInitParams   initPrms;
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
    /* Initialize LDC Init parameters */
    Vhwa_m2mLdcInitParamsInit(&initPrms);

    /* Set UDMA driver handle */
    initPrms.udmaDrvHndl = udmaDrvHndl;

    status = Vhwa_m2mLdcInit(&initPrms);
    
    if (FVID2_SOK != status)
    {
        Vhwa_print(" LDC_TEST_APP: LDC Init Failed\n");
    }
    else
    {
        Vhwa_M2mLdcSl2AllocPrmsInit(&sl2AllocPrms);

        status = Vhwa_m2mLdcAllocSl2(&sl2AllocPrms);
        
        if (FVID2_SOK != status)
        {
            Vhwa_print(" LDC_TEST_APP: SL2 Alloc Failed !!!\n");
        }
    }

    return (status);
}

int32_t AppLdcFrameComplCb(Fvid2_Handle handle, void *appData)
{
    AppLdc_TestObject *tObj = (AppLdc_TestObject *)appData;

    if (NULL != tObj)
    {
        VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
    }

    return FVID2_SOK;
}

void AppLdcErrorCb(Fvid2_Handle handle, uint32_t errEvents, void *appData)
{
    uint32_t errId;
    AppLdc_TestObject *tObj = (AppLdc_TestObject *)appData;

    if (NULL != tObj)
    {
        errId = 0x1U;
        while (errId <= VHWA_LDC_IFR_OUTOFBOUND)
        {
            switch (errEvents & errId)
            {
                case VHWA_LDC_PIX_IBLK_OUTOFBOUND_ERR:
                    break;
                case VHWA_LDC_IFR_OUTOFBOUND:
                    break;
            }

            errEvents = (errEvents & (~errId));

            errId = errId << 1U;
        }
        tObj->err_stat |= errEvents;
        if(0u != tObj->err_stat)
        {
            VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
        }
    }
}

void AppLdcWdtimerErrorCb(Fvid2_Handle handle, uint32_t wdTimerErrEvents, void *appData)
{
    AppLdc_TestObject *tObj = (AppLdc_TestObject *)appData;
    if (NULL != tObj)
    {
        tObj->wdTimerErrStatus |= wdTimerErrEvents;
        if(0u != tObj->wdTimerErrStatus)
        {
            VhwaAl_SemaphoreP_post(tObj->waitForProcessCmpl);
        }
    }
}
int32_t AppLdc_Create(LdcApp_TestParams *tObj, uint32_t hidx)
{
    int32_t             status;
    uint32_t            instId;
    AppLdc_TestObject      *appObj = &gAppLdcTestObj[hidx];

    Fvid2Utils_memset(appObj, 0x0, sizeof(AppLdc_TestObject));

    #if defined (VHWA_MCU_PLUS_SDK)
    #if defined (VHWA_FREERTOS_SUPPORT)
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
    #if defined (VHWA_MCU_PLUS_SDK)
    if (SYSTEMP_SUCCESS == status)
    #endif
    {
        if (UTRUE == tObj->testCfg[hidx]->isLsePsa)
        {
            appObj->createArgs.enablePsa = UTRUE;
        }
        else
        {
            appObj->createArgs.enablePsa = UFALSE;
        }
        #if defined(VHWA_PDK)
        if(tObj->isPerformanceTest)
        {
            appObj->createArgs.getTimeStamp = Vhwa_getTime;
        }
        #endif
        appObj->cbPrms.cbFxn   = AppLdcFrameComplCb;
        appObj->cbPrms.appData = appObj;

#if (VPAC_TEST_INSTANCE == 0)
        instId = VHWA_M2M_VPAC_0_LDC_DRV_INST_ID_0;
#endif
#if (VPAC_TEST_INSTANCE == 1)
        instId = VHWA_M2M_VPAC_1_LDC_DRV_INST_ID_0;
#endif

        appObj->handle = Fvid2_create(FVID2_VHWA_M2M_LDC_DRV_ID,
            instId, (void *)&appObj->createArgs,
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


void AppLdc_Delete(LdcApp_TestParams *tObj, uint32_t hidx)
{
    AppLdc_TestObject      *appObj = &gAppLdcTestObj[hidx];

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

int32_t AppLdc_SetParams(LdcApp_TestParams *tObj, uint32_t hidx)
{
    int32_t status;
    uint32_t cnt;
    AppLdc_TestConfig *cfg;
    Ldc_Config *ldcCfg;
    AppLdc_TestObject  *appObj = &gAppLdcTestObj[hidx];
    Ldc_ErrEventParams  errPrms;
    Ldc_RemapLutCfg     lutCfg;
    Ldc_WdTimerErrEventParams  wdTimerErrEvtPrms;

    cfg = tObj->testCfg[hidx];
    ldcCfg = &appObj->ldcCfg;

    ldcCfg->inFmt.width = cfg->inWidth;
    ldcCfg->inFmt.height = cfg->inHeight;
    ldcCfg->inFmt.dataFormat = cfg->inFmt;
    ldcCfg->inFmt.pitch[0U] = cfg->inPitch;
    ldcCfg->inFmt.pitch[1U] = cfg->inPitch;
    ldcCfg->inFmt.ccsFormat = cfg->inCcsf;

#if defined (VHWA_VPAC_IP_REV_VPAC3) || defined (VHWA_VPAC_IP_REV_VPAC3L)
    ldcCfg->indChPrms.enable = cfg->inIndFrm.enable;
    ldcCfg->indChPrms.pitch = cfg->inIndFrm.inPitch;
    ldcCfg->indChPrms.ccsf = cfg->inIndFrm.inCcsf;
#endif

    /* Output Format is same as input format */
    for (cnt = 0u; cnt < LDC_MAX_OUTPUT; cnt ++)
    {
        ldcCfg->outFmt[cnt].width = cfg->fmt[cnt].outWidth;
        ldcCfg->outFmt[cnt].height = cfg->fmt[cnt].outHeight;
        ldcCfg->outFmt[cnt].dataFormat = cfg->fmt[cnt].outFmt;
        ldcCfg->outFmt[cnt].pitch[0U] = cfg->fmt[cnt].outPitch;

#if defined (VHWA_VPAC_IP_REV_VPAC3) || defined (VHWA_VPAC_IP_REV_VPAC3L)
        if(UTRUE == cfg->inIndFrm.enable)
        {
            ldcCfg->outFmt[cnt].pitch[1U] = cfg->indChFmt[cnt].indChOutPitch;
        }
        else
        {
            ldcCfg->outFmt[cnt].pitch[1U] = cfg->fmt[cnt].outPitch;
        }

        if(FVID2_DF_RGB == ldcCfg->outFmt[cnt].dataFormat)
        {
            ldcCfg->outFmt[cnt].pitch[2U] = cfg->indChFmt[cnt].indCh2OutPitch;
        }
        ldcCfg->indOutChCcsf[cnt] = cfg->indChFmt[cnt].indChOutCcsf;
#else
        ldcCfg->outFmt[cnt].pitch[1U] = cfg->fmt[cnt].outPitch;
#endif

        ldcCfg->outFmt[cnt].ccsFormat = cfg->fmt[cnt].outCcsf;
    }

    ldcCfg->enableOutput[0u] = cfg->enableOutput[0u];
    ldcCfg->enableOutput[1u] = cfg->enableOutput[1u];
    ldcCfg->enableBackMapping = cfg->enableBackmapping;
    ldcCfg->lumaIntrType = cfg->lumaInterpolation;

    ldcCfg->perspTrnsformCfg.enableWarp = cfg->isAffineEn;
    ldcCfg->perspTrnsformCfg.coeffA = cfg->affineCoeff[0u];
    ldcCfg->perspTrnsformCfg.coeffB = cfg->affineCoeff[1u];
    ldcCfg->perspTrnsformCfg.coeffC = cfg->affineCoeff[2u];
    ldcCfg->perspTrnsformCfg.coeffD = cfg->affineCoeff[3u];
    ldcCfg->perspTrnsformCfg.coeffE = cfg->affineCoeff[4u];
    ldcCfg->perspTrnsformCfg.coeffF = cfg->affineCoeff[5u];
    ldcCfg->perspTrnsformCfg.coeffG = cfg->affineCoeff[6u];
    ldcCfg->perspTrnsformCfg.coeffH = cfg->affineCoeff[7u];

    ldcCfg->lutCfg.address           = cfg->lutPtr;
    ldcCfg->lutCfg.lineOffset        = cfg->lutPitch;
    ldcCfg->lutCfg.dsFactor          = cfg->lutDs;
    ldcCfg->lutCfg.width             = cfg->lutWidth;
    ldcCfg->lutCfg.height            = cfg->lutHeight;

    ldcCfg->pixelPad                 = cfg->pixPad;
    ldcCfg->outputBlockWidth         = cfg->blockWidth;
    ldcCfg->outputBlockHeight        = cfg->blockHeight;
    /* Out Frame size has to be same for both the outputs */
    ldcCfg->outputFrameWidth         = cfg->fmt[0u].outWidth;
    ldcCfg->outputFrameHeight        = cfg->fmt[0u].outHeight;
    ldcCfg->outputStartX             = 0u;
    ldcCfg->outputStartY             = 0u;

    ldcCfg->enableMultiRegions = cfg->isMultiRegMode;
    if (UTRUE == cfg->isMultiRegMode)
    {
        memcpy(&ldcCfg->regCfg, &cfg->regCfg, sizeof(ldcCfg->regCfg));
    }

    ldcCfg->rdBwLimitCfg.rdBwLimit          = (uint32_t)UFALSE;
    ldcCfg->rdBwLimitCfg.rdTagCnt           = 31U;
    ldcCfg->rdBwLimitCfg.rdMaxBurstLength   = 1U;

    status = Fvid2_control(appObj->handle, IOCTL_VHWA_M2M_LDC_SET_PARAMS,
        ldcCfg, NULL);

    if (FVID2_SOK == status)
    {
        /* Mask out watchdog timer events for regular error callback */
        errPrms.errEvents = cfg->enableErrorEvents & ~VHWA_LDC_WDTIMER_ERR;

        errPrms.cbFxn = AppLdcErrorCb;

        errPrms.appData = appObj;

        status = Fvid2_control(appObj->handle,
            IOCTL_VHWA_M2M_LDC_REGISTER_ERR_CB, &errPrms, NULL);
    }
    if (FVID2_SOK == status)
    {
        wdTimerErrEvtPrms.wdTimeoutCycles = cfg->wdTimeoutCycles;
        /* Mask to only watchdog timer events */
        wdTimerErrEvtPrms.wdTimerErrEvents = cfg->enableErrorEvents & VHWA_LDC_WDTIMER_ERR;
        wdTimerErrEvtPrms.cbFxn = AppLdcWdtimerErrorCb;
        wdTimerErrEvtPrms.appData = appObj;
        status = Fvid2_control(appObj->handle,
            VHWA_M2M_IOCTL_LDC_REGISTER_WDTIMER_ERR_CB, &wdTimerErrEvtPrms, NULL);
    }

    if (UTRUE == cfg->isLutEnable)
    {
        if (FVID2_SOK == status)
        {
            lutCfg.enable = UTRUE;
            lutCfg.inputBits = 12u;
            lutCfg.outputBits = 8u;
            lutCfg.tableAddr = gLut;
            status = Fvid2_control(appObj->handle,
                IOCTL_LDC_SET_LUMA_TONEMAP_LUT_CFG, &lutCfg, NULL);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" Failed to set Remap config \n");
            }
        }
        if (FVID2_SOK == status)
        {
            lutCfg.enable = UTRUE;
            lutCfg.inputBits = 12u;
            lutCfg.outputBits = 8u;
            lutCfg.tableAddr = gLut;
            status = Fvid2_control(appObj->handle,
                IOCTL_LDC_SET_CHROMA_TONEMAP_LUT_CFG, &lutCfg, NULL);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" Failed to set Remap config \n");
            }
        }
    }

    return (status);
}


int32_t AppLdc_AllocBuffers(LdcApp_TestParams *tObj, uint32_t hidx,
    uint64_t ldcTestSrcBuf, uint32_t *inFrameSize, uint64_t ldcTestDstBuf,
    uint32_t *outFrameSize)
{
    uint32_t cnt;
    uint32_t chromaOffset = 0;
    uint32_t plane3Offset = 0;
    uint32_t frameEnd = 0;
    Fvid2_Format *inFmt, *outFmt;
    Fvid2_Frame *inFrm, *outFrm;
    AppLdc_TestObject  *appObj = &gAppLdcTestObj[hidx];

    inFmt = &appObj->ldcCfg.inFmt;
    inFrm = &appObj->inFrm;

    switch (inFmt->dataFormat)
    {
        case FVID2_DF_YUV422I_UYVY:
            chromaOffset = 0;
            frameEnd = inFmt->pitch[0] * inFmt->height;
            break;

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
#if defined (VHWA_VPAC_IP_REV_VPAC3) || defined (VHWA_VPAC_IP_REV_VPAC3L)
        case FVID2_DF_YUV422SP_UV:
            switch (inFmt->ccsFormat)
            {
                case FVID2_CCSF_BITS12_PACKED:
                    chromaOffset = (inFmt->pitch[0]) * inFmt->height;
                    frameEnd = (inFmt->pitch[0] * inFmt->height) * 2;
                    break;
                case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
                case FVID2_CCSF_BITS12_UNPACKED16:
                    chromaOffset = inFmt->pitch[0] * inFmt->height;
                    frameEnd = (inFmt->pitch[0] * inFmt->height) * 2;
                    break;
                case FVID2_CCSF_BITS8_PACKED:
                    chromaOffset = inFmt->pitch[0] * inFmt->height;
                    frameEnd = inFmt->pitch[0] * inFmt->height * 2u;
                    break;
            }
            break;
        case FVID2_DF_2PLANES:
            switch (inFmt->ccsFormat)
            {
                case FVID2_CCSF_BITS12_PACKED:
                    chromaOffset = (inFmt->pitch[0]) * inFmt->height;
                    frameEnd = (inFmt->pitch[0] * inFmt->height) +
                                (inFmt->pitch[1] * inFmt->height);
                    break;
                case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
                case FVID2_CCSF_BITS12_UNPACKED16:
                    chromaOffset = inFmt->pitch[0] * inFmt->height;
                    frameEnd = (inFmt->pitch[0] * inFmt->height) +
                                (inFmt->pitch[1] * inFmt->height);
                    break;
                case FVID2_CCSF_BITS8_PACKED:
                    chromaOffset = inFmt->pitch[0] * inFmt->height;
                    frameEnd = (inFmt->pitch[0] * inFmt->height) +
                                (inFmt->pitch[1] * inFmt->height);
                    break;
            }
            break;
        case FVID2_DF_R_GBI:
            switch (inFmt->ccsFormat)
            {
                case FVID2_CCSF_BITS12_PACKED:
                    chromaOffset = (inFmt->pitch[0]) * inFmt->height;
                    frameEnd = (inFmt->pitch[0] * inFmt->height) +
                                (inFmt->pitch[1] * inFmt->height);
                    break;
                case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
                case FVID2_CCSF_BITS12_UNPACKED16:
                    chromaOffset = inFmt->pitch[0] * inFmt->height;
                    frameEnd = (inFmt->pitch[0] * inFmt->height) +
                                (inFmt->pitch[1] * inFmt->height);
                    break;
                case FVID2_CCSF_BITS8_PACKED:
                    chromaOffset = inFmt->pitch[0] * inFmt->height;
                    frameEnd = (inFmt->pitch[0] * inFmt->height) +
                                (inFmt->pitch[1] * inFmt->height);
                    break;
            }
            break;
#endif
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

    inFrm->addr[0U] = ldcTestSrcBuf;
    inFrm->addr[1U] = ldcTestSrcBuf + chromaOffset;

    *inFrameSize = frameEnd;

    Vhwa_print(" LDC InBuff Addr 0x%x 0x%x \n",
        (uint32_t)inFrm->addr[0U], (uint32_t)inFrm->addr[1U]);

    *outFrameSize = 0;
    for (cnt = 0u; cnt < LDC_MAX_OUTPUT; cnt ++)
    {
        outFrm = &appObj->outFrm[cnt];
        outFmt = &appObj->ldcCfg.outFmt[cnt];

        if (UFALSE == appObj->ldcCfg.enableOutput[cnt])
        {
            continue;
        }

        switch (outFmt->dataFormat)
        {
            case FVID2_DF_YUV422I_UYVY:
                chromaOffset = outFmt->pitch[0] * outFmt->height;
                frameEnd = outFmt->pitch[0] * outFmt->height;
                break;
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
#if defined (VHWA_VPAC_IP_REV_VPAC3) || defined (VHWA_VPAC_IP_REV_VPAC3L)
            case FVID2_DF_YUV422SP_UV:
                switch (outFmt->ccsFormat)
                {
                    case FVID2_CCSF_BITS12_PACKED:
                        chromaOffset = (outFmt->pitch[0]) * outFmt->height;
                        frameEnd = (outFmt->pitch[0] * outFmt->height) * 2;
                        break;
                    case FVID2_CCSF_BITS10_UNPACKED16_MSB_ALIGNED:
                    case FVID2_CCSF_BITS10_UNPACKED16:
                    case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
                    case FVID2_CCSF_BITS12_UNPACKED16:
                        chromaOffset = outFmt->pitch[0] * outFmt->height;
                        frameEnd = (outFmt->pitch[0] * outFmt->height) * 2;
                        break;
                    case FVID2_CCSF_BITS8_PACKED:
                        chromaOffset = outFmt->pitch[0] * outFmt->height;
                        frameEnd = outFmt->pitch[0] * outFmt->height * 2u;
                        break;
                }
                break;

            case FVID2_DF_RGI_B:
                switch (outFmt->ccsFormat)
                {
                    case FVID2_CCSF_BITS12_PACKED:
                        chromaOffset = (outFmt->pitch[0]) * outFmt->height;
                        frameEnd = (outFmt->pitch[0] * outFmt->height) +
                                    (outFmt->pitch[1] * outFmt->height);
                        break;
                    case FVID2_CCSF_BITS10_UNPACKED16_MSB_ALIGNED:
                    case FVID2_CCSF_BITS10_UNPACKED16:
                    case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
                    case FVID2_CCSF_BITS12_UNPACKED16:
                        chromaOffset = outFmt->pitch[0] * outFmt->height;
                        frameEnd = (outFmt->pitch[0] * outFmt->height) +
                                    (outFmt->pitch[1] * outFmt->height);
                        break;
                    case FVID2_CCSF_BITS8_PACKED:
                        chromaOffset = outFmt->pitch[0] * outFmt->height;
                        frameEnd = (outFmt->pitch[0] * outFmt->height) +
                                    (outFmt->pitch[1] * outFmt->height);
                        break;
                }
                break;
            case FVID2_DF_RGB:
                switch (outFmt->ccsFormat)
                {
                    case FVID2_CCSF_BITS12_PACKED:
                        chromaOffset = (outFmt->pitch[0]) * outFmt->height;
                        plane3Offset = (outFmt->pitch[0] * outFmt->height) +
                                       (outFmt->pitch[1] * outFmt->height);
                        frameEnd     = (outFmt->pitch[0] * outFmt->height) +
                                       (outFmt->pitch[1] * outFmt->height) +
                                       (outFmt->pitch[2] * outFmt->height);
                        break;
                    case FVID2_CCSF_BITS10_UNPACKED16_MSB_ALIGNED:
                    case FVID2_CCSF_BITS10_UNPACKED16:
                    case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
                    case FVID2_CCSF_BITS12_UNPACKED16:
                        chromaOffset = outFmt->pitch[0] * outFmt->height;
                        plane3Offset = (outFmt->pitch[0] * outFmt->height) +
                                       (outFmt->pitch[1] * outFmt->height);
                        frameEnd     = (outFmt->pitch[0] * outFmt->height) +
                                       (outFmt->pitch[1] * outFmt->height) +
                                       (outFmt->pitch[2] * outFmt->height);
                        break;
                    case FVID2_CCSF_BITS8_PACKED:
                        chromaOffset = outFmt->pitch[0] * outFmt->height;
                        plane3Offset = (outFmt->pitch[0] * outFmt->height) +
                                       (outFmt->pitch[1] * outFmt->height);
                        frameEnd     = (outFmt->pitch[0] * outFmt->height) +
                                       (outFmt->pitch[1] * outFmt->height) +
                                       (outFmt->pitch[2] * outFmt->height);
                        break;
                }
                break;
#endif
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

        outFrm->addr[0U] = (ldcTestDstBuf);
        outFrm->addr[1U] = (ldcTestDstBuf + chromaOffset);
        outFrm->addr[2U] = (ldcTestDstBuf + plane3Offset); /* Not applicable for VPAC(J721E)*/

        /* Move Buffer Index */
        *outFrameSize += frameEnd;
        ldcTestDstBuf += frameEnd;

        Vhwa_print(" LDC OutBuff Addr 0x%x 0x%x \n",
            (uint32_t)outFrm->addr[0U], (uint32_t)outFrm->addr[1U]);
    }

    return (FVID2_SOK);
}
#if defined (VHWA_PDK)
int32_t AppLdc_CrcInit(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = FVID2_SOK;

    gLdcCrcChHandle.crcChannel = APP_LDC_CRC_CHANNEL;
    gLdcCrcChHandle.udmaCrcChHandle = App_crcCreate(udmaDrvHndl,
                                                 &gLdcCrcChHandle.crcChannel);
    if(gLdcCrcChHandle.udmaCrcChHandle == NULL)
    {
        Vhwa_print(" App_crcCreate Error\n");
        status = FVID2_EFAIL;
    }

    return status;
}

int32_t AppLdc_CrcDeinit(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = FVID2_SOK;

    App_crcDelete(udmaDrvHndl, &gLdcCrcChHandle);

    return status;
}

static int32_t AppLdc_CompareCrc(LdcApp_TestParams *tObj, uint32_t hIdx)
{
    int32_t status = FVID2_SOK;
    uint64_t crcSignVal = 0;
    uint32_t outCnt;
    AppLdc_TestConfig *tCfg;
    AppCrc_inPrms srcPrms;
    AppLdc_TestObject  *appObj = &gAppLdcTestObj[hIdx];

    tCfg = tObj->testCfg[hIdx];

    if (UTRUE == tCfg->isCrcAvail)
    {
        for (outCnt = 0u; outCnt < LDC_MAX_OUTPUT && status == FVID2_SOK; outCnt ++)
        {
            if(tCfg->enableOutput[outCnt] == UTRUE)
            {
                srcPrms.width = Vhwa_calcHorzSizeInBytes(tCfg->fmt[outCnt].outWidth,
                                                    tCfg->fmt[outCnt].outCcsf);
                srcPrms.pitch = tCfg->fmt[outCnt].outPitch;

                if(tCfg->fmt[outCnt].outFmt == FVID2_DF_YUV420SP_UV ||
                    tCfg->fmt[outCnt].outFmt == FVID2_DF_YUV420SP_VU)
                {
                    srcPrms.height = tCfg->fmt[outCnt].outHeight*3/2;
                }
                else
                {
                    srcPrms.height = tCfg->fmt[outCnt].outHeight;
                }

                status = App_getCrc(&gLdcCrcChHandle,
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
void AppLdc_PrepareRequest(LdcApp_TestParams *tObj, uint32_t hIdx)
{
    uint32_t ldcOutCnt;
    AppLdc_TestConfig *tCfg;
    Fvid2_FrameList   *inFrmList;
    Fvid2_FrameList   *outFrmList;
    AppLdc_TestObject  *appObj = &gAppLdcTestObj[hIdx];

    tCfg = tObj->testCfg[hIdx];
    inFrmList = &appObj->inFrmList;
    outFrmList = &appObj->outFrmList;

    if(UFALSE == tObj->isPerformanceTest)
    {
        Vhwa_print(
            " LDC Handle %d, Width = %d Height = %d\n",
            hIdx, tCfg->inWidth, tCfg->inHeight);
    }

    /* Initialize LDC Input Frame List */
    inFrmList->frames[0U] = &appObj->inFrm;
    inFrmList->numFrames = 1U;

    /* Initialize LDC Output Frame List */
    outFrmList->numFrames = 0u;
    for (ldcOutCnt = 0u; ldcOutCnt < LDC_MAX_OUTPUT; ldcOutCnt++)
    {
        outFrmList->frames[ldcOutCnt] = &appObj->outFrm[ldcOutCnt];
        if (UTRUE == tCfg->enableOutput[ldcOutCnt])
        {
            outFrmList->numFrames ++;
        }
    }
}

int32_t AppLdc_SubmitRequest(LdcApp_TestParams *tObj, uint32_t hIdx)
{
    int32_t            status;
    Fvid2_FrameList   *inFrmList;
    Fvid2_FrameList   *outFrmList;
    AppLdc_TestObject  *appObj = &gAppLdcTestObj[hIdx];

    inFrmList = &appObj->inFrmList;
    outFrmList = &appObj->outFrmList;

    /* Submit LDC Request*/
    status = Fvid2_processRequest(appObj->handle, inFrmList,
        outFrmList, FVID2_TIMEOUT_FOREVER);
    if (FVID2_SOK != status)
    {
        Vhwa_print
            (" LDC: Fvid2_processRequest returned %x for LDC Handle # %d\n",
            status, hIdx);
    }

    return (status);
}

int32_t AppLdc_WaitForComplRequest(LdcApp_TestParams *tObj, uint32_t hIdx)
{
    int32_t          status;
    Fvid2_FrameList *inFrmList;
    Fvid2_FrameList *outFrmList;
    AppLdc_TestObject  *appObj = &gAppLdcTestObj[hIdx];
    AppLdc_TestConfig *tCfg;
    Vhwa_M2mLdcPsaSign psa;
    uint32_t            cnt;

    tCfg = tObj->testCfg[hIdx];

    VhwaAl_SemaphoreP_pend(appObj->waitForProcessCmpl, SYSTEMP_WAIT_FOREVER);

    inFrmList = &appObj->inFrmList;
    outFrmList = &appObj->outFrmList;

    if((0u == appObj->wdTimerErrStatus) && (0u == appObj->err_stat))
    {
        status = Fvid2_getProcessedRequest(appObj->handle,
            inFrmList, outFrmList, 0);
        if (FVID2_SOK != status)
        {
            Vhwa_print (" LDC: Failed LDC Handle Cnt %d; status = %x\n",
                hIdx, status);
            return (status);
        }
        else
        {
            #if defined (VHWA_PDK)
            status = AppLdc_CompareCrc(tObj, hIdx);
            if (FVID2_SOK != status)
            {
                Vhwa_print (" LDC: CRC Check Failed Handle Cnt %d\n", hIdx);
            }
            #endif
            if (UTRUE == tCfg->isLsePsa)
            {
                status = Fvid2_control(appObj->handle,
                    VHWA_M2M_IOCTL_LDC_GET_PSA_SIGN,
                    &psa, NULL);
                if (FVID2_SOK == status)
                {
                    for (cnt = 0u; cnt < LDC_MAX_OUTPUT; cnt ++)
                    {
                        if ((0 != tCfg->psaSign[cnt]) &&
                            (tCfg->psaSign[cnt] != psa.psaSign[cnt]))
                        {
                            Vhwa_print("LDC PSA mismatch\n");
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
        if(0u != appObj->wdTimerErrStatus)
        {
            Vhwa_print("IP stall: Watchdog timer error interrupt triggered \n");
            status = FVID2_ETIMEOUT;
        }
        if(0u != appObj->err_stat)
        {
            Vhwa_print("Error interrupt: LDC error interrupt triggered \n");
            status = FVID2_EFAIL;
        }
    }


    return (status);
}

void AppLdc_SyncStart(LdcApp_TestParams *tObj, uint32_t hidx)
{
    uint32_t status;
    AppLdc_TestObject  *appObj = &gAppLdcTestObj[hidx];

    status = Fvid2_control(appObj->handle,
                            IOCTL_VHWA_M2M_LDC_SYNC_START,
                            NULL, NULL);
    if(FVID2_SOK != status)
    {
        Vhwa_print(" LDC Sync Start Failed \n");
    }
}
