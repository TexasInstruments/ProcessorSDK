/*
*  Copyright (c) Texas Instruments Incorporated 2018
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
*  \file vhwa_flexconnect_api.h
*
*  \brief VHWA Flex connect API and struct definitions
*/
#ifndef VPAC_EX_FC_TEST_API_H_
#define VPAC_EX_FC_TEST_API_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <include/vhwa_common.h>
#include <include/vhwa_m2mFlexConnect.h>
#include <examples/include/vhwa_examples_common.h>
#include <examples/include/vhwa_msc_api.h>
#include <examples/include/vhwa_viss_test_api.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    Vhwa_M2mFcGraphPathInfo     *fcPathInfo;

    AppViss_TestConfig          *vissTestCfg;
    App_MscTestCfg              *msc0TestCfg;
    App_MscTestCfg              *msc1TestCfg;
} AppFc_TestConfig;



typedef struct
{
    Fvid2_Handle                handle;
    /**< FVID2 Driver Handle */

    Vhwa_M2mFcCreatePrms        createArgs;

    App_MscTestObj              msc0TestObj;
    App_MscTestObj              msc1TestObj;
    AppViss_TestObject          vissTestObj;

    Fvid2_CbParams              cbPrms;
    /**< Callback params */

    Fvid2_FrameList         inFrmList;
    /**< Input Frame List Dummy list currently unused */
    Fvid2_FrameList         outFrmList;
    /**< Output Frame List Dummy list currently unused */

    VhwaAl_SemaphoreP_Struct       waitForProcessCmpl;
} AppFc_TestObject;

typedef struct
{
    char                    testName[100];
    /**< Test Name */
    uint32_t                numHandles;
    /**< Max Handles in this Test */
    uint32_t                repeatCnt;
    /**< Number of times to repeat this test */
    uint32_t                isPerformanceTest;
    /**< Flag to indicate if this is performance test */
    uint32_t                isVissEnabled;
    uint32_t                isMsc0Enabled;
    uint32_t                isMsc1Enabled;

    AppFc_TestConfig        testCfg[VHWA_FC_DRV_MAX_HANDLES];
    /**< Pointer to the Test Config */
    uint32_t                isEnableTest;
    /**< Flag to enable/disable Test */
    uint32_t                vissIsSwitchGlbceCtx;
    /* GLBCE CTX Switch */
    uint32_t                vissChCfgOnEachIter;
    /**< Flag to change Config on each iteration */
} AppFc_TestParams;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t AppFc_Init(Udma_DrvHandle udmaDrvHndl);
void AppFc_deInit(Udma_DrvHandle udmaDrvHndl);
int32_t AppFc_Create(AppFc_TestParams *tPrms, uint32_t hidx);
void AppFc_Delete(AppFc_TestParams *tPrms, uint32_t hidx);

int32_t AppFc_SetAllConfig(AppFc_TestParams *tPrms, uint32_t hidx);

int32_t AppFc_AllocBuffers(AppFc_TestParams *tPrms, uint32_t hidx,
    uint64_t srcBuf, uint32_t *inFrameSize,
    uint64_t dstBuf, uint32_t *outFrameSize);

void AppFc_PrepareRequest(AppFc_TestParams *tObj, uint32_t hIdx);
int32_t AppFc_SubmitRequest(AppFc_TestParams *tObj, uint32_t hIdx);
int32_t AppFc_WaitForCompRequest(AppFc_TestParams *tObj, uint32_t hIdx);
#if defined (VHWA_VPAC_IP_REV_VPAC) || defined (VHWA_VPAC_IP_REV_VPAC3)
int32_t AppFc_CrcInit(Udma_DrvHandle udmaDrvHndl);
int32_t AppFc_CrcDeinit(Udma_DrvHandle udmaDrvHndl);
#endif
int32_t AppFcFrameComplCb(Fvid2_Handle handle, void *appData);

#endif /*VPAC_EX_VISS_TEST_API_H_*/
