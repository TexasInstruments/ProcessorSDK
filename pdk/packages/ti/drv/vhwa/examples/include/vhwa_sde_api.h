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
*  \file vhwa_sde_api.h
*
*  \brief VHWA SDE API and struct definitions
*/
#ifndef VPAC_EX_SDE_API_H_
#define VPAC_EX_SDE_API_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <include/vhwa_m2mSde.h>
#include <examples/include/vhwa_examples_common.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*
* Application test parameters
*/

#define APP_SDE_MAX_HANDLES         (4u)

/* Default init parameters for memory allocation */
#define APP_SDE_DEFAULT_IMG_WIDTH   (2048U)
#define APP_SDE_DEFAULT_CCSF        (FVID2_CCSF_BITS12_UNPACKED16)
#define APP_SDE_DEFAULT_SR          (SDE_SR_192)

#define APP_SDE_CRC_CHANNEL         (CRC_CHANNEL_1)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */


typedef struct
{
    /* FOCO params to be configured */
    uint32_t shiftM1;
    uint32_t dir;
    uint32_t round;
}AppSde_FocoCfgPrms;

typedef struct
{
    /* Input Frame Parameter */
    uint32_t imgStrFmt;
    uint32_t basePitch;
    uint32_t refPitch;

    /* output buffer Parameter */
    /* Output buffer pitch should be atleast (imgWidth*2) to multiple of 128
     * is transfered in block of width 128
     */
    uint32_t outPitch;


    /* SDE Parameters */
    uint32_t enableSDE;
    uint32_t medianFilter;
    uint32_t imgWidth;
    uint32_t imgHeight;
    uint32_t minDisparity;
    uint32_t searchRange;
    uint32_t lrThreshold;
    uint32_t enableTextureFilter;
    uint32_t textureFilterThreshold;
    uint32_t penaltyP1;
    uint32_t penaltyP2;
    uint32_t enablePsa;
    uint32_t enableHistogram;
    uint32_t isCrcAvail;
    uint64_t crcSign;
    uint32_t psaSign;

    /* valid only when the Image storage format is not 12 bit packed */
    AppSde_FocoCfgPrms focoCfg;

    /* WDTimer configuration */
    /* SDE watchdog timer timeout cycles: 0=64K, 1=128K */
    uint32_t wdTimeoutCycles;
    /* FOCO watchdog timer timeout cycles: 0=64K, 1=128K */
    uint32_t focoWdTimeoutCycles;
    /* Bitmask of error events (both regular and watchdog timer errors) */
    uint32_t enableErrorEvents;
} AppSde_TestCfg;

typedef struct
{
    uint32_t                enableBwLimit;
    uint32_t                cycleCnt, tokenCnt;

    Fvid2_Handle            handle;
    /**< FVID2 Driver Handle */

    Vhwa_M2mSdeCreateArgs   createArgs;
    /**< FVID2 Create Arguments */

    Vhwa_M2mSdePrms         sdePrms;
    /**< SDE Configuration */
    Fvid2_Frame             inFrm[VHWA_M2M_SDE_MAX_IN_BUFFER];
    /**< Input Frames */
    Fvid2_Frame             outFrm;
    /**< Output Frames */
    Fvid2_CbParams          cbPrms;
    /**< Callback params */
    Fvid2_FrameList         inFrmList;
    /**< Input Frame List */
    Fvid2_FrameList         outFrmList;
    /**< Output Frame List */
    uint32_t                csHistogram[128];
    /**< CS histraom value will be filled for base frame only when
         enableCsHistogram in Vhwa_m2mSdeConfig is set */
    VhwaAl_SemaphoreP_Struct       waitForProcessCmpl;
    /**< Semaphore to wait for completion */
    uint32_t                    err_stat;
    /**<Stores error status value*/
    uint32_t                       wdTimerErrStatus;
    /**< Store watchdog timer error interrupt status*/
} AppSde_TestObj;

typedef struct
{
    char                    testName[50];
    /**< Name of the Test */
    uint32_t                isEnableTest;
    /**< Flag to Bypass Test */
    uint32_t                numHandles;
    /**< Max Handles in this Test */
    uint32_t                repeatCnt;
    /**< Number of times to repeat this test */
    uint32_t                isPerformanceTest;
    /**< Flag to indicate if this is performance test */
    AppSde_TestCfg         *testCfg[APP_SDE_MAX_HANDLES];
    /**< Pointer to the Test Config */
    uint32_t               *sdeConfScore[APP_SDE_MAX_HANDLES];
    /**< Pointer to the Confidence score paramters */
} AppSde_TestParams;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t AppSde_Init(Udma_DrvHandle udmaDrvHndl);
int32_t AppSde_SetParams(AppSde_TestParams *tObj, uint32_t hidx);
int32_t AppSde_SetConfScoreParam(AppSde_TestParams *tObj, uint32_t hidx);

int32_t AppSde_AllocBuffers(AppSde_TestParams *tObj, uint32_t hidx,
                         uint64_t sdeBaseSrcBuf, uint32_t *baseSrcBuffSize,
                         uint64_t sdeRefSrcBuf, uint32_t *refSrcBuffSize,
                         uint64_t sdeDesBuf, uint32_t *desBuffSize);

int32_t AppSde_Create(AppSde_TestParams *tObj, uint32_t hidx);
void AppSde_Delete(AppSde_TestParams *tObj, uint32_t hidx);
int32_t AppSde_CompareLsePsa(AppSde_TestParams *tObj, uint32_t hidx);

void AppSde_PrepareRequest(AppSde_TestParams *tObj, uint32_t hidx);
int32_t AppSde_SubmitRequest(AppSde_TestParams *tObj, uint32_t hidx);
int32_t AppSde_WaitForComplRequest(AppSde_TestParams *tObj, uint32_t hidx);
#if defined VHWA_PDK
int32_t AppSde_CrcInit(Udma_DrvHandle udmaDrvHndl);
int32_t AppSde_CrcDeinit(Udma_DrvHandle udmaDrvHndl);
#endif

void AppSde_SyncStart(AppSde_TestParams *tObj, uint32_t hidx);

#endif /*VPAC_EX_SDE_API_H_*/
