/*
*  Copyright (c) Texas Instruments Incorporated 2025
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
*  \file vhwa_flexconnect_m2m_api.h
*
*  \brief VHWA Flexconnect+M2M API and struct definitions
*/
#ifndef VPAC_EX_FC_M2M_TEST_API_H_
#define VPAC_EX_FC_M2M_TEST_API_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <ti/drv/fvid2/fvid2.h>
#include <include/vhwa_common.h>
#include <include/vhwa_m2mFlexConnect.h>
#include <examples/include/vhwa_examples_common.h>
#include <examples/include/vhwa_common_crc.h>
#include <examples/include/vhwa_msc_api.h>
#include <examples/include/vhwa_ldc_api.h>
#include <examples/include/vhwa_viss_test_api.h>
#include <examples/include/vhwa_flexconnect_api.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define APP_FC_M2M_MAX_HANDLES              (4U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct 
{
    AppViss_TestParams      vissAppTestPrms;
    /**< VISS App Test Object */

    App_MscTestParams       msc0AppTestPrms;
    /**< MSC App Test Object */

    App_MscTestParams       msc1AppTestPrms;
    /**< MSC App Test Object */

    LdcApp_TestParams       ldcAppTestPrms;
    /**< LDC App Test Object */

    AppFc_TestParams        fcAppTestPrms;
    /**< FC App Test Object */

}AppFcM2m_TestConfig;


/*
* Application test parameters
*/
typedef struct
{
    char                    testName[50];

    uint32_t                numHandles;
    /**< Max Handles in this Test */
    uint32_t                repeatCnt;
    /**< Number of times to repeat this test */

    uint32_t                isM2mVissEnabled;
    uint32_t                isM2mMsc0Enabled;
    uint32_t                isM2mMsc1Enabled;
    uint32_t                isM2mLdcEnabled;
    uint32_t                isFcEnabled;

    uint32_t                isParallel;
    /* is Flexconnect and M2M running in parallel */

    AppFcM2m_TestConfig     testCfg[APP_FC_M2M_MAX_HANDLES];
    /**< Pointer to the Test Config */

    uint32_t                isEnableTest;

} AppFcM2m_TestPrms;

typedef struct
{
    uint64_t vissSrcBuf;
    uint32_t vissSrcBufIdx;
    uint64_t vissDstBuf;
    uint32_t vissDstBufIdx;

    uint64_t msc0SrcBuf;
    uint32_t msc0SrcBufIdx;
    uint64_t msc0DstBuf;
    uint32_t msc0DstBufIdx;

    uint64_t msc1SrcBuf;
    uint32_t msc1SrcBufIdx;
    uint64_t msc1DstBuf;
    uint32_t msc1DstBufIdx;

    uint64_t ldcSrcBuf;
    uint32_t ldcSrcBufIdx;
    uint64_t ldcDstBuf;
    uint32_t ldcDstBufIdx;

    uint64_t fcSrcBuf;
    uint32_t fcSrcBufIdx;
    uint64_t fcDstBuf;
    uint32_t fcDstBufIdx;

} AppFcM2m_TestMemoryObj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t AppFcM2m_Create(AppFcM2m_TestPrms *tObj, uint32_t hndlIdx);
int32_t AppFcM2m_Delete(AppFcM2m_TestPrms *tObj, uint32_t hidx);
int32_t AppFcM2m_Init(Udma_DrvHandle udmaDrvHndl);
int32_t AppFcM2m_CrcInit(Udma_DrvHandle udmaDrvHndl);
int32_t AppFcM2m_CrcDeinit(Udma_DrvHandle udmaDrvHndl);
int32_t AppFcM2m_SetParams(AppFcM2m_TestPrms *tObj, uint32_t hndlIdx);
int32_t AppFcM2m_SetCoeff(AppFcM2m_TestPrms *tObj, uint32_t hndlIdx);
int32_t AppFcM2m_AllocBuffers(AppFcM2m_TestPrms *tObj, uint32_t hndlIdx ,
                            AppFcM2m_TestMemoryObj *fcM2mTestMemObj);

#endif /*VPAC_EX_FC_M2M_TEST_API_H_*/
