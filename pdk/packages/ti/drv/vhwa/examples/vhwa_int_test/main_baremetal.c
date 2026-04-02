/**
 *   Copyright (c) Texas Instruments Incorporated 2019
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
 *  \file main_baremetal.c
 *
 *  \brief Main file for baremetal build
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <ti/board/board.h>

#include <examples/include/vhwa_int_api.h>
#include "examples/vhwa_viss_test/vhwa_viss_cfg.h"
#include "vhwa_int_cfg.h"

#include <ti/csl/hw_types.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define LDC_SRC_BUFF_SIZE  (0x08000000u)
#define LDC_DST_BUFF_SIZE  (0x08000000u)

#define NF_SRC_BUFF_SIZE  (0x08000000u)
#define NF_DST_BUFF_SIZE  (0x08000000u)

#define MSC0_SRC_BUFF_SIZE  (0x08000000u)
#define MSC0_DST_BUFF_SIZE  (0x08000000u)

#define MSC1_SRC_BUFF_SIZE  (0x08000000u)
#define MSC1_DST_BUFF_SIZE  (0x08000000u)

#define SDE_SRC_BUFF_SIZE  (0x08000000u)
#define SDE_DST_BUFF_SIZE  (0x08000000u)

#define DOF_SRC_BUFF_SIZE  (0x08000000u)
#define DOF_DST_BUFF_SIZE  (0x08000000u)

#define VISS_SRC_BUFF_SIZE  (0x08000000u)
#define VISS_DST_BUFF_SIZE  (0x08000000u)

/* SDE Buffer offsets */
#define SDE_BASE_BUFF_OFFSET    (0x00000000U)
#define SDE_REF_BUFF_OFFSET     (0x00800000u)
/* DOF Buffer offsets */
#define DOF_REF_BUFF_OFFSET     (0x00000000U)
#define DOF_CUR_BUFF_OFFSET     (0x00300000u)
#define DOF_TEMP_BUFF_OFFSET    (0x00600000u)
#define DOF_SOF_BUFF_OFFSET     (0x00A00000u)

#define DOF_DES_BUFF_OFFSET     (0x00000000U)
#define DOF_ODD_BUFF_OFFSET     (0x00400000u)
#define DOF_EVEN_BUFF_OFFSET    (0x00800000u)


#define INT_SRC_BUFF_SIZE  (\
    LDC_SRC_BUFF_SIZE + NF_SRC_BUFF_SIZE + MSC0_SRC_BUFF_SIZE + \
    MSC1_SRC_BUFF_SIZE + DOF_SRC_BUFF_SIZE + SDE_SRC_BUFF_SIZE + \
    VISS_SRC_BUFF_SIZE)
#define INT_DST_BUFF_SIZE  (\
    LDC_DST_BUFF_SIZE + NF_DST_BUFF_SIZE + MSC0_DST_BUFF_SIZE + \
    MSC1_DST_BUFF_SIZE + SDE_DST_BUFF_SIZE + DOF_DST_BUFF_SIZE + \
    VISS_DST_BUFF_SIZE)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t App_init();
static void App_deInit();
static void AppInt_Test(AppInt_TestPrms *tObj,
                        AppInt_TestMemoryObj * intTestMemObj);
static int32_t App_getNum(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*
* Application Buffers
*/
uint64_t gIntTestSrcBuf;
uint64_t gIntTestDstBuf;

/*LDC Buffers*/
uint64_t gLdcTestSrcBuf;
uint64_t gLdcTestDstBuf;
uint32_t gLdcTestSrcBufFreeIdx;
uint32_t gLdcTestDstBufFreeIdx;

/*NF Buffers*/
uint64_t gNfTestSrcBuf;
uint64_t gNfTestDstBuf;
uint32_t gNfTestSrcBufFreeIdx;
uint32_t gNfTestDstBufFreeIdx;

/*MSC Buffers*/
uint64_t gMsc0TestSrcBuf;
uint64_t gMsc0TestDstBuf;
uint32_t gMsc0TestSrcBufFreeIdx;
uint32_t gMsc0TestDstBufFreeIdx;

/*MSC Buffers*/
uint64_t gMsc1TestSrcBuf;
uint64_t gMsc1TestDstBuf;
uint32_t gMsc1TestSrcBufFreeIdx;
uint32_t gMsc1TestDstBufFreeIdx;

/* SDE Buffers */
uint64_t gSdeTestBaseBuf;
uint64_t gSdeTestRefBuf;
uint64_t gSdeTestDstBuf;
uint32_t gSdeTestBaseBufFreeIdx;
uint32_t gSdeTestRefBufFreeIdx;
uint32_t gSdeTestDstBufFreeIdx;

/* DOF Buffers */
uint64_t gDofRefBuf;
uint64_t gDofCurBuf;
uint64_t gDofTempBuf;
uint64_t gDofSofBuf;
uint64_t gDofDstBuf;
uint64_t gDofInterOddBuf;
uint64_t gDofInterEvenBuf;
uint32_t gDofRefBufFreeIdx;
uint32_t gDofCurBufFreeIdx;
uint32_t gDofTempBufFreeIdx;
uint32_t gDofSofBufFreeIdx;
uint32_t gDofDstBufFreeIdx;

/* VISS Buffers*/
uint64_t gVissTestSrcBuf;
uint64_t gVissTestDstBuf;
uint32_t gVissTestSrcBufFreeIdx;
uint32_t gVissTestDstBufFreeIdx;

static AppNf_TestConfig gAppNfTestCfg[] =
{
    #include <examples/include/vhwa_nf_test_cfg.h>
};

static AppLdc_TestConfig gAppLdcTestCfg[] =
{
     #include <examples/include/vhwa_ldc_test_cfg.h>
};

static App_MscTestCfg gAppMscTestCfg[] =
{
    #include <examples/include/vhwa_msc_test_cfg.h>
};

static AppSde_TestCfg gAppSdeTestCfg[] =
{
    #include <examples/include/vhwa_sde_test_cfg.h>
};

static AppDof_TestConfig gAppDofTestCfg[] =
{
    #include <examples/include/vhwa_dof_test_cfg.h>
};

extern AppViss_TestConfig gAppVissTestConfig[];

static uint32_t gSdeConfScoreMap[][DMPAC_SDE_NUM_SCORE_MAP] =
{
    /* Conf 0  */
    {0x10, 0x3e, 0x45, 0x49, 0x4b, 0x5a, 0x5e, 0x7F/* unused set to maximum */},
};

static AppInt_TestPrms gAppIntObj[] = VHWA_INT_TIRTOS_CFG;

static struct Udma_DrvObj gIntAppUdmaDrvObj;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    int32_t                 status = FVID2_SOK;
    uint32_t                testCnt = -1;
    AppInt_TestMemoryObj    intTestMemObj;
    AppInt_TestPrms        *tObj = NULL;

    App_startTimer();


    gIntTestSrcBuf = (uint64_t )(0x90000000U);
    /*Mayank : TBD : Add a check to ensure that there is enough memory for DstBuf*/
    gIntTestDstBuf = (uint64_t )(0xC8000000U);

    /*LDC Buffers*/
    gLdcTestSrcBuf = (uint64_t )gIntTestSrcBuf;
    gLdcTestDstBuf = (uint64_t )gIntTestDstBuf;

    /*NF Buffers*/
    gNfTestSrcBuf = (uint64_t )gLdcTestSrcBuf + LDC_SRC_BUFF_SIZE;
    gNfTestDstBuf = (uint64_t )gLdcTestDstBuf + LDC_DST_BUFF_SIZE;

    /*MSC Buffers*/
    gMsc0TestSrcBuf = (uint64_t )gNfTestSrcBuf + NF_SRC_BUFF_SIZE;
    gMsc0TestDstBuf = (uint64_t )gNfTestDstBuf + NF_DST_BUFF_SIZE;

    gMsc1TestSrcBuf = (uint64_t )gMsc0TestSrcBuf + MSC0_SRC_BUFF_SIZE;
    gMsc1TestDstBuf = (uint64_t )gMsc0TestDstBuf + MSC0_DST_BUFF_SIZE;

    /*VISS Buffers*/
    gVissTestSrcBuf = (uint64_t )gMsc1TestSrcBuf + MSC1_SRC_BUFF_SIZE;
    gVissTestDstBuf = (uint64_t )gMsc1TestDstBuf + MSC1_DST_BUFF_SIZE;

    /* SDE Buffers */
    gSdeTestBaseBuf = (uint64_t )gVissTestSrcBuf + VISS_SRC_BUFF_SIZE +
                                SDE_BASE_BUFF_OFFSET;
    gSdeTestRefBuf = (uint64_t )gVissTestSrcBuf + VISS_SRC_BUFF_SIZE +
                                SDE_REF_BUFF_OFFSET;
    gSdeTestDstBuf = (uint64_t )gVissTestDstBuf + VISS_DST_BUFF_SIZE;

    /* DOF Buffers */
    gDofRefBuf = (uint64_t )gSdeTestBaseBuf + SDE_SRC_BUFF_SIZE +
                            DOF_REF_BUFF_OFFSET;
    gDofCurBuf = (uint64_t )gSdeTestBaseBuf + SDE_SRC_BUFF_SIZE +
                            DOF_CUR_BUFF_OFFSET;
    gDofTempBuf = (uint64_t )gSdeTestBaseBuf + SDE_SRC_BUFF_SIZE +
                            DOF_TEMP_BUFF_OFFSET;
    gDofSofBuf = (uint64_t )gSdeTestBaseBuf + SDE_SRC_BUFF_SIZE +
                            DOF_SOF_BUFF_OFFSET;
    gDofDstBuf = (uint64_t )gSdeTestDstBuf + SDE_DST_BUFF_SIZE +
                            DOF_DES_BUFF_OFFSET;
    gDofInterOddBuf = (uint64_t )gSdeTestDstBuf + SDE_DST_BUFF_SIZE +
                            DOF_ODD_BUFF_OFFSET;
    gDofInterEvenBuf = (uint64_t )gSdeTestDstBuf + SDE_DST_BUFF_SIZE +
                            DOF_EVEN_BUFF_OFFSET;

    intTestMemObj.ldcSrcBuf = gLdcTestSrcBuf;
    intTestMemObj.ldcSrcBufIdx = gLdcTestSrcBufFreeIdx;
    intTestMemObj.ldcDstBuf = gLdcTestDstBuf;
    intTestMemObj.ldcDstBufIdx = gLdcTestDstBufFreeIdx;

    intTestMemObj.nfSrcBuf = gNfTestSrcBuf;
    intTestMemObj.nfSrcBufIdx = gNfTestSrcBufFreeIdx;
    intTestMemObj.nfDstBuf = gNfTestDstBuf;
    intTestMemObj.nfDstBufIdx = gNfTestDstBufFreeIdx;

    intTestMemObj.msc0SrcBuf = gMsc0TestSrcBuf;
    intTestMemObj.msc0SrcBufIdx = gMsc0TestSrcBufFreeIdx;
    intTestMemObj.msc0DstBuf = gMsc0TestDstBuf;
    intTestMemObj.msc0DstBufIdx = gMsc0TestDstBufFreeIdx;

    intTestMemObj.msc1SrcBuf = gMsc1TestSrcBuf;
    intTestMemObj.msc1SrcBufIdx = gMsc1TestSrcBufFreeIdx;
    intTestMemObj.msc1DstBuf = gMsc1TestDstBuf;
    intTestMemObj.msc1DstBufIdx = gMsc1TestDstBufFreeIdx;

    intTestMemObj.sdeBaseBuf = gSdeTestBaseBuf;
    intTestMemObj.sdeBaseBufFreeIdx = gSdeTestBaseBufFreeIdx;
    intTestMemObj.sdeRefBuf = gSdeTestRefBuf;
    intTestMemObj.sdeRefBufFreeIdx = gSdeTestRefBufFreeIdx;
    intTestMemObj.sdeDstBuf = gSdeTestDstBuf;
    intTestMemObj.sdeDstBufFreeIdx = gSdeTestDstBufFreeIdx;

    intTestMemObj.dofRefBuf = gDofRefBuf;
    intTestMemObj.dofRefBufFreeIdx = gDofRefBufFreeIdx;
    intTestMemObj.dofCurBuf = gDofCurBuf;
    intTestMemObj.dofCurBufFreeIdx = gDofCurBufFreeIdx;
    intTestMemObj.dofTempBuf = gDofTempBuf;
    intTestMemObj.dofTempBufFreeIdx = gDofTempBufFreeIdx;
    intTestMemObj.dofSofBuf = gDofSofBuf;
    intTestMemObj.dofSofBufFreeIdx = gDofSofBufFreeIdx;
    intTestMemObj.dofDstBuf = gDofDstBuf;
    intTestMemObj.dofDstBufFreeIdx = gDofDstBufFreeIdx;
    intTestMemObj.dofInterOddBuf = gDofInterOddBuf;
    intTestMemObj.dofInterEvenBuf = gDofInterEvenBuf;

    intTestMemObj.vissSrcBuf = gVissTestSrcBuf;
    intTestMemObj.vissSrcBufIdx = gVissTestSrcBufFreeIdx;
    intTestMemObj.vissDstBuf = gVissTestDstBuf;
    intTestMemObj.vissDstBufIdx = gVissTestDstBufFreeIdx;

    status = App_init();

    if (FVID2_SOK == status)
    {
        while(1)
        {
            Vhwa_print("\n Enter Test to Run (Enter '-1' to exit)  \n");
            testCnt = App_getNum();
            if(testCnt == -1)
            {
                break;
            }

            tObj = &gAppIntObj[testCnt];

            gLdcTestSrcBufFreeIdx = 0u;
            gLdcTestDstBufFreeIdx = 0u;
            gNfTestSrcBufFreeIdx = 0u;
            gNfTestDstBufFreeIdx = 0u;
            gMsc0TestSrcBufFreeIdx = 0u;
            gMsc0TestDstBufFreeIdx = 0u;
            gMsc1TestSrcBufFreeIdx = 0u;
            gMsc1TestDstBufFreeIdx = 0u;
            gVissTestSrcBufFreeIdx = 0u;
            gVissTestDstBufFreeIdx = 0u;

            gSdeTestBaseBufFreeIdx = 0U;
            gSdeTestRefBufFreeIdx = 0U;
            gSdeTestDstBufFreeIdx = 0U;
            gDofRefBufFreeIdx = 0U;
            gDofCurBufFreeIdx = 0U;
            gDofTempBufFreeIdx = 0U;
            gDofSofBufFreeIdx = 0U;
            gDofDstBufFreeIdx = 0U;

            /* Only single Testcases are supported */
            Vhwa_print (" Starting Test %s\n", tObj->integrationTestName);

            if((1U == tObj->numHandles) && (UTRUE == tObj->enable))
            {
                AppInt_Test(tObj, &intTestMemObj);
            }

        }
    }

    Vhwa_print (" Exiting \n");
    App_deInit();

    return(0);
}

static void AppInt_Test(AppInt_TestPrms *tObj,
                        AppInt_TestMemoryObj * intTestMemObj)
{
    int32_t             status = FVID2_SOK;
    uint32_t            numHandles, repCnt, hIdx, rIdx;
    LdcApp_TestParams  *tLdcPrms = &(tObj->ldcAppTestPrms);
    NfApp_TestParams      *tNfPrms = &(tObj->nfAppTestPrms);
    App_MscTestParams     *tMsc0Prms = &(tObj->msc0AppTestPrms);
    App_MscTestParams     *tMsc1Prms = &(tObj->msc1AppTestPrms);
    AppSde_TestParams     *tSdePrms = &(tObj->sdeAppTestPrms);
    DofApp_TestParams     *tDofPrms = &(tObj->dofAppTestPrms);
    AppViss_TestParams *tVissObj = &(tObj->vissAppTestPrms);

    numHandles = tObj->numHandles;
    repCnt = tObj->repeatCnt;

    tObj->moduleCnt = 0u;

    /*Handle Count and Repeat assumed same for all components in an integration test*/
    Vhwa_print(" VHWA_INT_APP: handle count = %d, Repeat Count = %d \n", numHandles, repCnt);
    if(tObj->ldcTestEnabled)
    {
        tObj->moduleCnt ++;
        Vhwa_print(" VHWA_INT_APP: Creating TestCase %s\n", tObj->ldcAppTestPrms.testName);
    }
    if(tObj->nfTestEnabled)
    {
        tObj->moduleCnt ++;
        Vhwa_print(" VHWA_INT_APP: Creating TestCase %s\n", tObj->nfAppTestPrms.testName);
    }
    if(tObj->msc0TestEnabled)
    {
        tObj->moduleCnt ++;
        Vhwa_print(" VHWA_INT_APP: Creating TestCase %s\n", tObj->msc0AppTestPrms.testName);
    }
    if(tObj->msc1TestEnabled)
    {
        tObj->moduleCnt ++;
        Vhwa_print(" VHWA_INT_APP: Creating TestCase %s\n", tObj->msc1AppTestPrms.testName);
    }
    if(tObj->sdeTestEnabled)
    {
        tObj->moduleCnt ++;
        Vhwa_print(" VHWA_INT_APP: Creating TestCase %s\n", tObj->sdeAppTestPrms.testName);
    }
    if(tObj->dofTestEnabled)
    {
        tObj->moduleCnt ++;
        Vhwa_print(" VHWA_INT_APP: Creating TestCase %s\n", tObj->dofAppTestPrms.testName);
    }
    if(tObj->vissTestEnabled)
    {
        tObj->moduleCnt ++;
        Vhwa_print(" VHWA_INT_APP: Creating TestCase %s\n", tObj->vissAppTestPrms.testName);
    }

    /*Handle Count assumed same for all components in an integration test*/
    tLdcPrms->numHandles = numHandles;
    tLdcPrms->repeatCnt = repCnt;
    tNfPrms->numHandles = numHandles;
    tNfPrms->repeatCnt = repCnt;
    tMsc0Prms->numHandles = numHandles;
    tMsc0Prms->repeatCnt = repCnt;
    tMsc1Prms->numHandles = numHandles;
    tMsc1Prms->repeatCnt = repCnt;
    tSdePrms->numHandles = numHandles;
    tSdePrms->repeatCnt = repCnt;
    tDofPrms->numHandles = numHandles;
    tDofPrms->repeatCnt = repCnt;
    tVissObj->numHandles = numHandles;
    tVissObj->repeatCnt = repCnt;

    for (hIdx = 0u; (hIdx < numHandles) && (FVID2_SOK == status); hIdx ++)
    {
        status |= AppInt_Create(tObj, hIdx);
        if(FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: Create Failed for handle # %d\n", hIdx);
            status = FVID2_EFAIL;
            break;
        }

        status |= AppInt_SetCoeff(tObj, hIdx);
        if(FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: SetCoeff Failed for handle # %d\n", hIdx);
            status = FVID2_EFAIL;
            break;
        }

        status |= AppInt_SetParams(tObj, hIdx);
        if(FVID2_SOK != status)
        {
            Vhwa_print(" VHWA_INT_APP: AppInt_SetParams returned %x for handle # %d\n", status, hIdx);
            return;
        }

        if(tObj->ldcTestEnabled)
        {
            intTestMemObj->ldcSrcBuf = gLdcTestSrcBuf + intTestMemObj->ldcSrcBufIdx;
            intTestMemObj->ldcDstBuf = gLdcTestDstBuf + intTestMemObj->ldcDstBufIdx;
        }
        if(tObj->nfTestEnabled)
        {
            intTestMemObj->nfSrcBuf = gNfTestSrcBuf + intTestMemObj->nfSrcBufIdx;
            intTestMemObj->nfDstBuf = gNfTestDstBuf + intTestMemObj->nfDstBufIdx;
        }
        if(tObj->msc0TestEnabled)
        {
            intTestMemObj->msc0SrcBuf = gMsc0TestSrcBuf + intTestMemObj->msc0SrcBufIdx;
            intTestMemObj->msc0DstBuf = gMsc0TestDstBuf + intTestMemObj->msc0DstBufIdx;
        }

        if(tObj->msc1TestEnabled)
        {
            intTestMemObj->msc1SrcBuf = gMsc1TestSrcBuf + intTestMemObj->msc1SrcBufIdx;
            intTestMemObj->msc1DstBuf = gMsc1TestDstBuf + intTestMemObj->msc1DstBufIdx;
        }
        if(tObj->sdeTestEnabled)
        {
            intTestMemObj->sdeBaseBuf = gSdeTestBaseBuf + intTestMemObj->sdeBaseBufFreeIdx;
            intTestMemObj->sdeRefBuf = gSdeTestRefBuf + intTestMemObj->sdeRefBufFreeIdx;
            intTestMemObj->sdeDstBuf = gSdeTestDstBuf + intTestMemObj->sdeDstBufFreeIdx;
        }
        if(tObj->dofTestEnabled)
        {
            intTestMemObj->dofRefBuf = gDofRefBuf + intTestMemObj->dofRefBufFreeIdx;
            intTestMemObj->dofCurBuf = gDofCurBuf + intTestMemObj->dofCurBufFreeIdx;
            intTestMemObj->dofTempBuf = gDofTempBuf + intTestMemObj->dofTempBufFreeIdx;
            intTestMemObj->dofSofBuf = gDofSofBuf + intTestMemObj->dofSofBufFreeIdx;
            intTestMemObj->dofDstBuf = gDofDstBuf + intTestMemObj->dofDstBufFreeIdx;
            intTestMemObj->dofInterOddBuf = gDofInterOddBuf;
            intTestMemObj->dofInterEvenBuf = gDofInterEvenBuf;
        }

        if(tObj->vissTestEnabled)
        {
            intTestMemObj->vissSrcBuf = gVissTestSrcBuf + intTestMemObj->vissSrcBufIdx;
            intTestMemObj->vissDstBuf = gVissTestDstBuf + intTestMemObj->vissDstBufIdx;
        }

        status |= AppInt_AllocBuffers(tObj, hIdx, intTestMemObj);
        if(FVID2_SOK != status)
        {
            Vhwa_print(" VHWA_INT_APP: AppInt_AllocBuffers returned %x\n", status);
        }
        Vhwa_print(" Load buffers and press any key to continue\n");
        UART_getc();
    }

    for (rIdx = 0u; (rIdx < repCnt) && (FVID2_SOK == status); rIdx ++)
    {
        Vhwa_print ("Iteration Count = %d\n", rIdx);

        for (hIdx = 0u; (hIdx < numHandles) && (FVID2_SOK == status); hIdx ++)
        {
            if(tObj->ldcTestEnabled)
            {
                AppLdc_PrepareRequest(tLdcPrms, hIdx);
            }

            if(tObj->nfTestEnabled)
            {
                AppNf_PrepareRequest(tNfPrms, hIdx);
            }

            if(tObj->msc0TestEnabled)
            {
                AppMsc_PrepareRequest(tMsc0Prms, hIdx);
            }

            if(tObj->msc1TestEnabled)
            {
                AppMsc_PrepareRequest(tMsc1Prms, hIdx);
            }
            if(tObj->sdeTestEnabled)
            {
                AppSde_PrepareRequest(tSdePrms, hIdx);
            }
            if(tObj->dofTestEnabled)
            {
                AppDof_PrepareRequest(tDofPrms, hIdx);
            }
            if(tObj->vissTestEnabled)
            {
                AppViss_PrepareRequest(tVissObj, hIdx);
            }

            if(tObj->ldcTestEnabled)
            {
                status = AppLdc_SubmitRequest(tLdcPrms, hIdx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print
                        (" INT_TEST_APP: Failed to SubmitRequest for LDC\n");
                    return;
                }
            }

            if(tObj->nfTestEnabled)
            {
                status = AppNf_SubmitRequest(tNfPrms, hIdx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print
                        (" INT_TEST_APP: Failed to SubmitRequest for NF\n");
                    return;
                }
            }

            if(tObj->msc0TestEnabled)
            {
                /* Submit MSC0 Request*/
                status = AppMsc_SubmitRequest(tMsc0Prms, hIdx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print
                        (" INT_TEST_APP: Failed to SubmitRequest for MSC0\n");
                    return;
                }
            }

            if(tObj->msc1TestEnabled)
            {
                /* Submit MSC0 Request*/
                status = AppMsc_SubmitRequest(tMsc1Prms, hIdx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print
                        (" INT_TEST_APP: Failed to SubmitRequest for MSC1\n");
                    return;
                }
            }

            if(tObj->sdeTestEnabled)
            {
                /* Submit SDE Request*/
                status = AppSde_SubmitRequest(tSdePrms, hIdx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print
                        (" INT_TEST_APP: Failed to SubmitRequest for SDE\n");
                    return;
                }
            }

            if(tObj->vissTestEnabled)
            {
                /* Submit VISS Request*/
                status = AppViss_SubmitRequest(tVissObj, hIdx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print
                        (" INT_TEST_APP: Failed to SubmitRequest for VISS\n");
                    return;
                }
            }

            if(tObj->dofTestEnabled)
            {
                /* Submit DOF Request*/
                status = AppDof_SubmitRequest(tDofPrms, hIdx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print
                        (" INT_TEST_APP: Failed to SubmitRequest for DOF\n");
                    return;
                }
            }

            Vhwa_print("Starting \n");
#ifdef VHWA_USE_PIPELINE_COMMON_ENABLE

            if(tObj->dofTestEnabled)
            {
                AppDof_SyncStart(tDofPrms, hIdx);
            }
            if(tObj->sdeTestEnabled)
            {
                AppSde_SyncStart(tSdePrms, hIdx);
            }
            if(tObj->ldcTestEnabled)
            {
                AppLdc_SyncStart(tLdcPrms, hIdx);
            }
            if(tObj->nfTestEnabled)
            {
                AppNf_SyncStart(tNfPrms, hIdx);
            }
            if(tObj->msc0TestEnabled)
            {
                AppMsc_SyncStart(tMsc0Prms, hIdx);
            }
            if(tObj->msc1TestEnabled)
            {
                AppMsc_SyncStart(tMsc1Prms, hIdx);
            }
            if(tObj->vissTestEnabled)
            {
                AppViss_SyncStart(tVissObj, hIdx);
            }
#else
            uint32_t pyrCnt;

            if(tObj->dofTestEnabled)
            {
                /* Loop is for total pyramid count - 1 request is submitted once
                   already */
                for(pyrCnt = (tDofPrms->testCfg[hIdx]->tPyrLvl - 1); pyrCnt > 0;
                    pyrCnt--)
                {
                    status = AppDof_WaitForComplRequest(tDofPrms, hIdx);
                    if ((FVID2_SOK != status) && (FVID2_EAGAIN != status))
                    {
                        Vhwa_print
                            (" INT_TEST_APP: Failed to GetRequest for Dof\n");
                        return;
                    }

                    /* Submit DOF Request*/
                    status = AppDof_SubmitRequest(tDofPrms, hIdx);
                    if (FVID2_SOK != status)
                    {
                        Vhwa_print
                            (" INT_TEST_APP: Failed to SubmitRequest for DOF\n");
                        return;
                    }
                }
            }
#endif

            if (FVID2_SOK == status)
            {
                if(tObj->dofTestEnabled)
                {
                    status = AppDof_WaitForComplRequest(tDofPrms, hIdx);
                    if ((FVID2_SOK != status) && (FVID2_EAGAIN != status))
                    {
                        Vhwa_print
                            (" INT_TEST_APP: Failed to GetRequest for Dof\n");
                        return;
                    }
                }
                if(tObj->sdeTestEnabled)
                {
                    status = AppSde_WaitForComplRequest(tSdePrms, hIdx);
                    if (FVID2_SOK != status && (FVID2_EAGAIN != status))
                    {
                        Vhwa_print
                            (" INT_TEST_APP: Failed to GetRequest for SDE\n");
                        return;
                    }
                }
                if(tObj->ldcTestEnabled)
                {
                    status = AppLdc_WaitForComplRequest(tLdcPrms, hIdx);
                    if (FVID2_SOK != status)
                    {
                        Vhwa_print
                            (" INT_TEST_APP: Failed to GetRequest for LDC\n");
                        return;
                    }
                }
                if(tObj->nfTestEnabled)
                {
                    status = AppNf_WaitForComplRequest(tNfPrms, hIdx);
                    if (FVID2_SOK != status)
                    {
                        Vhwa_print
                            (" INT_TEST_APP: Failed to GetRequest for NF\n");
                        return;
                    }
                }
                if(tObj->msc0TestEnabled)
                {
                    status = AppMsc_WaitForComplRequest(tMsc0Prms, hIdx);
                    if (FVID2_SOK != status)
                    {
                        Vhwa_print
                            (" INT_TEST_APP: Failed to GetRequest for MSC0\n");
                        return;
                    }
                }
                if(tObj->msc1TestEnabled)
                {
                    status = AppMsc_WaitForComplRequest(tMsc1Prms, hIdx);
                    if (FVID2_SOK != status)
                    {
                        Vhwa_print
                            (" INT_TEST_APP: Failed to GetRequest for MSC1\n");
                        return;
                    }
                }
                if(tObj->vissTestEnabled)
                {
                    status = AppViss_WaitForCompRequest(tVissObj, hIdx, 0U);
                    if (FVID2_SOK != status)
                    {
                        Vhwa_print
                            (" INT_TEST_APP: Failed to GetRequest for VISS\n");
                        return;
                    }
                }

                Vhwa_print("Completed \n");
            }
        }
    }

    for (hIdx = 0u; (hIdx < numHandles) && (FVID2_SOK == status); hIdx ++)
    {
        AppInt_Delete(tObj, hIdx);
    }
}

static int32_t App_init()
{
    int32_t                 status;
    uint32_t                instId;
    Udma_DrvHandle          drvHandle = &gIntAppUdmaDrvObj;
    Udma_InitPrms           udmaInitPrms;
    Board_initCfg           boardCfg;
    Fvid2_InitPrms initPrmsFvid2;

    boardCfg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    Fvid2InitPrms_init(&initPrmsFvid2);
    initPrmsFvid2.printFxn = Vhwa_print;
    status = Fvid2_init(&initPrmsFvid2);

    if (FVID2_SOK != status)
    {
        Vhwa_print(" main: FVID2_Init Failed !!!\r\n");
    }

    if (FVID2_SOK == status)
    {
        /* Initialize UDMA and get the handle, it will be used in both CRC layer,
           as well as in the driver */
        /* UDMA driver init */
        instId = UDMA_INST_ID_MAIN_0;
        UdmaInitPrms_init(instId, &udmaInitPrms);
        udmaInitPrms.printFxn = &App_udmaPrint;
        status = Udma_init(drvHandle, &udmaInitPrms);
        if(UDMA_SOK != status)
        {
            Vhwa_print("[Error] UDMA init failed!!\n");
            status = FVID2_EFAIL;
        }
    }

    status = AppInt_Init(drvHandle);

    return (status);
}

static void App_deInit()
{
    int32_t         status;
    Udma_DrvHandle  drvHandle = &gIntAppUdmaDrvObj;

    status = Udma_deinit(drvHandle);
    if(UDMA_SOK != status)
    {
        Vhwa_print("[Error] UDMA deinit failed!!\n");
    }

    Fvid2_deInit(NULL);
}

int32_t App_getNum(void)
{
    int32_t num;

    UART_scanFmt("%d", &num);

    return (num);
}
