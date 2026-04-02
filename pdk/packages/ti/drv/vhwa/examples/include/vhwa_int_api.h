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
*  \file vhwa_int_api.h
*
*  \brief VHWA INT Integration test API and struct definitions
*/
#ifndef VPAC_EX_INT_API_H_
#define VPAC_EX_INT_API_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <ti/drv/fvid2/fvid2.h>
#include <include/vhwa_common.h>
#include <include/vhwa_m2mNf.h>
#include <include/vhwa_m2mMsc.h>
#include <include/vhwa_m2mDof.h>
#include <include/vhwa_m2mSde.h>
#include <include/vhwa_m2mLdc.h>
#include <include/vhwa_m2mViss.h>
#include <examples/include/vhwa_examples_common.h>
#include <examples/include/vhwa_ldc_api.h>
#include <examples/include/vhwa_nf_api.h>
#include <examples/include/vhwa_msc_api.h>
#include <examples/include/vhwa_sde_api.h>
#include <examples/include/vhwa_dof_api.h>
#include <examples/include/vhwa_viss_test_api.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*
* Application test parameters
*/

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/*
* Application test parameters
*/
typedef struct
{
    char                    integrationTestName[50];
    uint32_t                enable;
    uint32_t                numHandles;
    /**< Max Handles in this Test */
    uint32_t                repeatCnt;
    /**< Number of times to repeat this test */
    uint32_t                ldcTestEnabled;
    /**< Enable/Disable LDC Test */
    LdcApp_TestParams       ldcAppTestPrms;
    /**< LDC App Test Object */
    uint32_t                nfTestEnabled;
    /**< Enable/Disable NF Test */
    NfApp_TestParams        nfAppTestPrms;
    /**< NF App Test Object */
    uint32_t                msc0TestEnabled;
    /**< Enable/Disable MSC Test */
    App_MscTestParams       msc0AppTestPrms;
    /**< MSC App Test Object */
    uint32_t                msc1TestEnabled;
    /**< Enable/Disable MSC Test */
    App_MscTestParams       msc1AppTestPrms;
    /**< MSC App Test Object */
    uint32_t                sdeTestEnabled;
    /**< Enable/Disable SDE Test */
    AppSde_TestParams       sdeAppTestPrms;
    /**< SDE App Test Object */
    uint32_t                dofTestEnabled;
    /**< Enable/Disable DOF Test */
    DofApp_TestParams       dofAppTestPrms;
    /**< DOF App Test Object */
    uint32_t                vissTestEnabled;
    /**< Enable/Disable VISS Test */
    AppViss_TestParams      vissAppTestPrms;
    /**< VISS App Test Object */

    volatile uint32_t       moduleCnt;

} AppInt_TestPrms;

typedef struct
{
    uint64_t ldcSrcBuf;
    uint32_t ldcSrcBufIdx;
    uint64_t ldcDstBuf;
    uint32_t ldcDstBufIdx;

    uint64_t nfSrcBuf;
    uint32_t nfSrcBufIdx;
    uint64_t nfDstBuf;
    uint32_t nfDstBufIdx;

    uint64_t msc0SrcBuf;
    uint32_t msc0SrcBufIdx;
    uint64_t msc0DstBuf;
    uint32_t msc0DstBufIdx;

    uint64_t msc1SrcBuf;
    uint32_t msc1SrcBufIdx;
    uint64_t msc1DstBuf;
    uint32_t msc1DstBufIdx;

    uint64_t sdeBaseBuf;
    uint32_t sdeBaseBufFreeIdx;
    uint64_t sdeRefBuf;
    uint32_t sdeRefBufFreeIdx;
    uint64_t sdeDstBuf;
    uint32_t sdeDstBufFreeIdx;

    uint64_t dofRefBuf;
    uint32_t dofRefBufFreeIdx;
    uint64_t dofCurBuf;
    uint32_t dofCurBufFreeIdx;
    uint64_t dofTempBuf;
    uint32_t dofTempBufFreeIdx;
    uint64_t dofSofBuf;
    uint32_t dofSofBufFreeIdx;
    uint64_t dofDstBuf;
    uint32_t dofDstBufFreeIdx;
    uint64_t dofInterOddBuf;
    uint64_t dofInterEvenBuf;

    uint64_t vissSrcBuf;
    uint32_t vissSrcBufIdx;
    uint64_t vissDstBuf;
    uint32_t vissDstBufIdx;
} AppInt_TestMemoryObj;

typedef enum
{
    INT_IDX_LDC = 0,
    INT_IDX_NF = 1,
    INT_IDX_MSC0 = 2,
    INT_IDX_MSC1 = 3,
    INT_IDX_SDE = 4,
    INT_IDX_DOF = 5,
    INT_IDX_VISS = 6,
    INT_IDX_MAX,
} INT_IDX_FRM;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t AppInt_Init(Udma_DrvHandle udmaDrvHndl);
int32_t AppInt_Create(AppInt_TestPrms *tObj, uint32_t hndlIdx);
int32_t AppInt_Delete(AppInt_TestPrms *tObj, uint32_t hidx);
int32_t AppInt_SetCoeff(AppInt_TestPrms *tObj, uint32_t hndlIdx);
int32_t AppInt_SetParams(AppInt_TestPrms *tObj, uint32_t hndlIdx);
int32_t AppInt_AllocBuffers(AppInt_TestPrms *tObj, uint32_t hndlIdx ,
                            AppInt_TestMemoryObj * intTestMemObj);

int32_t AppInt_CrcInit(AppInt_TestPrms *tObj, Udma_DrvHandle udmaDrvHndl);
int32_t AppInt_CrcDeinit(AppInt_TestPrms *tObj, Udma_DrvHandle udmaDrvHndl);

#endif /*VPAC_EX_INT_API_H_*/
