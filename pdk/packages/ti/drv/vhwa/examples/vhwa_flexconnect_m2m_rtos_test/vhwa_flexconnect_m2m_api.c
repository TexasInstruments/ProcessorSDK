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
*  \file vhwa_fc_api.c
*
*  \brief VHWA VISS APIs
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <examples/include/vhwa_examples_common.h>
#include <examples/include/vhwa_common_crc.h>


#include <include/vhwa_m2mFlexConnect.h>

#include <examples/include/vhwa_viss_test_api.h>
#include <examples/include/vhwa_msc_api.h>
#include <examples/include/vhwa_ldc_api.h>
#include <examples/include/vhwa_flexconnect_api.h>
#include <examples/include/vhwa_flexconnect_m2m_api.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define MSC_SRC_OFFSET      (5u * 1024u * 1024u)
#define MSC_DST_OFFSET      (5u * 1024u * 1024u)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

int32_t mscSpCoeffSets[][MSC_MAX_SP_COEFF_SET][MSC_MAX_TAP] = {
    {
        {16, 48, 128, 48, 16},
        {32, 32, 128, 32, 32}
    },
};

int32_t mscMpCoeffSets[][MSC_MAX_MP_COEFF_SET][MSC_MAX_TAP * 32U] =
{
    #include "../vhwa_msc_test/coeff.txt"
};

Msc_Coeff gMscCoefTbl[] =
{
    {
        {
            mscSpCoeffSets[0][0], mscSpCoeffSets[0][1]
        },
        {
            mscMpCoeffSets[0][0], mscMpCoeffSets[0][1],
            mscMpCoeffSets[0][2], mscMpCoeffSets[0][3]
        }
    }
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t AppFcM2m_Create(AppFcM2m_TestPrms *tObj, uint32_t hidx)
{
    int32_t status = 0;
    LdcApp_TestParams   *ldcAppTestPrms;
    App_MscTestParams   *msc0AppTestPrms, *msc1AppTestPrms;
    AppViss_TestParams  *vissAppTestPrms;
    AppFc_TestParams    *fcAppTestPrms;

    AppFcM2m_TestConfig *tCfg = &tObj->testCfg[hidx];

    if(tObj->isM2mVissEnabled == UTRUE && tCfg->vissAppTestPrms.isEnableTest == UTRUE)
    {
        vissAppTestPrms = &(tCfg->vissAppTestPrms);
        status |= AppViss_Create(vissAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: VISS Create Failed for handle # %d\n",
                hidx);
            return status;
        }
    }

    if(tObj->isM2mMsc0Enabled == UTRUE && tCfg->msc0AppTestPrms.isEnableTest == UTRUE)
    {
        msc0AppTestPrms = &(tCfg->msc0AppTestPrms);
        status |= AppMsc_Create(msc0AppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: MSC0 Create Failed for handle # %d\n",
                hidx);
            return status;
        }
    }

    if(tObj->isM2mMsc1Enabled == UTRUE && tCfg->msc1AppTestPrms.isEnableTest == UTRUE)
    {
        msc1AppTestPrms = &(tCfg->msc1AppTestPrms);
        status |= AppMsc_Create(msc1AppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: MSC1 Create Failed for handle # %d\n",
                hidx);
            return status;
        }
    }

    if(tObj->isM2mLdcEnabled)
    {
        ldcAppTestPrms = &(tCfg->ldcAppTestPrms);
        status |= AppLdc_Create(ldcAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: LDC Create Failed for handle # %d\n",
                hidx);
            return status;
        }
    }

    if(tObj->isFcEnabled == UTRUE && tCfg->fcAppTestPrms.isEnableTest == UTRUE)
    {
        fcAppTestPrms = &(tCfg->fcAppTestPrms);
        status |= AppFc_Create(fcAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: FC Create Failed for handle # %d\n",
                hidx);
            return status;
        }
    }

    return status;
}

int32_t AppFcM2m_Delete(AppFcM2m_TestPrms *tObj, uint32_t hidx)
{
    int32_t status = 0;
    LdcApp_TestParams   *ldcAppTestPrms;
    App_MscTestParams   *msc0AppTestPrms, *msc1AppTestPrms;
    AppViss_TestParams  *vissAppTestPrms;
    AppFc_TestParams    *fcAppTestPrms;

    AppFcM2m_TestConfig *tCfg = &tObj->testCfg[hidx];

    if(tObj->isM2mVissEnabled == UTRUE && tCfg->vissAppTestPrms.isEnableTest == UTRUE)
    {
        vissAppTestPrms = &(tCfg->vissAppTestPrms);
        AppViss_Delete(vissAppTestPrms, hidx);
    }

    if(tObj->isM2mMsc0Enabled == UTRUE && tCfg->msc0AppTestPrms.isEnableTest == UTRUE)
    {
        msc0AppTestPrms = &(tCfg->msc0AppTestPrms);
        AppMsc_Delete(msc0AppTestPrms, hidx);
    }

    if(tObj->isM2mMsc1Enabled == UTRUE && tCfg->msc1AppTestPrms.isEnableTest == UTRUE)
    {
        msc1AppTestPrms = &(tCfg->msc1AppTestPrms);
        AppMsc_Delete(msc1AppTestPrms, hidx);
    }

    if(tObj->isM2mLdcEnabled == UTRUE && tCfg->ldcAppTestPrms.isEnableTest == UTRUE)
    {
        ldcAppTestPrms = &(tCfg->ldcAppTestPrms);
        AppLdc_Delete(ldcAppTestPrms, hidx);
    }

    if(tObj->isFcEnabled == UTRUE && tCfg->fcAppTestPrms.isEnableTest == UTRUE)
    {
        fcAppTestPrms = &(tCfg->fcAppTestPrms);
        AppFc_Delete(fcAppTestPrms, hidx);
    }

    return status;
}

int32_t AppFcM2m_Init(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = 0;
    Vhwa_M2mLdcSl2AllocPrms ldcSl2Prms;
    Vhwa_M2mLdcInitParams   ldcInitPrms;
    Vhwa_M2mMscInitParams   mscInitPrms;
    Vhwa_M2mVissInitParams  vissInitPrms;
    Vhwa_M2mFcSl2AllocPrms  fcSl2Prms;
    Vhwa_M2mFcInitPrms      fcInitPrms;

    Vhwa_print(" FC_M2M_TEST_APP: Init\n");

    {
        /* Initialize VISS Init parameters */
        Vhwa_m2mVissInitParamsInit(&vissInitPrms);

        /* Set UDMA driver handle */
        vissInitPrms.udmaDrvHndl = udmaDrvHndl;

        status |= Vhwa_m2mVissInit(&vissInitPrms);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: VISS Init Failed\n");
        }
    }

    {
        Vhwa_m2mMscInitParamsInit(&mscInitPrms);

        mscInitPrms.drvHandle = udmaDrvHndl;

        status |= Vhwa_m2mMscInit(&mscInitPrms);

        if(FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: MSC Init Failed\n");
        }
    }

    {
        /* Initialize LDC Init parameters */
        Vhwa_m2mLdcInitParamsInit(&ldcInitPrms);

        /* Set UDMA driver handle */
        ldcInitPrms.udmaDrvHndl = udmaDrvHndl;

        status |= Vhwa_m2mLdcInit(&ldcInitPrms);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: LDC Init Failed\n");
        }
        else
        {
            Vhwa_M2mLdcSl2AllocPrmsInit(&ldcSl2Prms);

            status |= Vhwa_m2mLdcAllocSl2(&ldcSl2Prms);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" FC_M2M_TEST_APP: LDC SL2 Alloc Failed !!!\n");
            }
        }
    }

    {
        /* Initialize Fc Init parameters */
        Vhwa_m2mFcInitPrmsInit(&fcInitPrms);

        /* Set UDMA driver handle */
        fcInitPrms.udmaDrvHndl = udmaDrvHndl;

        status = Vhwa_m2mFcInit(&fcInitPrms);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: FC Init Failed\n");
        }
        else
        {
            Vhwa_m2mFcDrvSl2AllocPrmsInit(&fcSl2Prms);

            status = Vhwa_m2mFcAllocSl2(&fcSl2Prms);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" FC_M2M_TEST_APP: SL2 Alloc Failed !!!\n");
            }
        }
    }

    return (status);
}

int32_t AppFcM2m_CrcInit(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = 0;

    {
        status |= AppViss_CrcInit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: VISS CRC Init Failed\n");
        }
    }

    {
        status |= AppMsc_CrcInit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: MSC CRC Init Failed\n");
        }
    }

    {
        status |= AppLdc_CrcInit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: LDC CRC Init Failed\n");
        }
    }

    {
        status |= AppFc_CrcInit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: FC CRC Init Failed\n");
        }
    }

    return (status);
}

int32_t AppFcM2m_CrcDeinit(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = 0;

    {
        status |= AppViss_CrcDeinit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: VISS CRC Init Failed\n");
        }
    }

    {
        status |= AppMsc_CrcDeinit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: MSC CRC Init Failed\n");
        }
    }

    {
        status |= AppLdc_CrcDeinit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: LDC CRC Init Failed\n");
        }
    }

    {
        status |= AppFc_CrcDeinit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: FC CRC Init Failed\n");
        }
    }

    return (status);
}

int32_t AppFcM2m_SetParams(AppFcM2m_TestPrms *tObj, uint32_t hndlIdx)
{
    int32_t status = 0;
    LdcApp_TestParams  *ldcAppTestPrms;
    App_MscTestParams  *msc0AppTestPrms, *msc1AppTestPrms;
    AppViss_TestParams *vissAppTestPrms;
    AppFc_TestParams   *fcAppTestPrms;

    AppFcM2m_TestConfig *tCfg = &tObj->testCfg[hndlIdx];

    Vhwa_print(" FC_M2M_TEST_APP: SetParams %s\n", tObj->testName);

    if(tObj->isM2mVissEnabled == UTRUE && tCfg->vissAppTestPrms.isEnableTest == UTRUE)
    {
        vissAppTestPrms = &(tCfg->vissAppTestPrms);
        status = AppViss_SetAllConfig(vissAppTestPrms, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: VISS SetParams Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
    }

    if(tObj->isM2mMsc0Enabled == UTRUE && tCfg->msc0AppTestPrms.isEnableTest == UTRUE)
    {
        msc0AppTestPrms = &(tCfg->msc0AppTestPrms);
        status = AppMsc_SetParams(msc0AppTestPrms, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: MSC0 SetParams Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
    }

    if(tObj->isM2mMsc1Enabled == UTRUE && tCfg->msc1AppTestPrms.isEnableTest == UTRUE)
    {
        msc1AppTestPrms = &(tCfg->msc1AppTestPrms);
        status = AppMsc_SetParams(msc1AppTestPrms, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: MSC1 SetParams Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
    }

    if(tObj->isM2mLdcEnabled == UTRUE && tCfg->ldcAppTestPrms.isEnableTest == UTRUE)
    {
        ldcAppTestPrms = &(tCfg->ldcAppTestPrms);
        status = AppLdc_SetParams(ldcAppTestPrms, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: LDC SetParams Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
    }

    if(tObj->isFcEnabled == UTRUE && tCfg->fcAppTestPrms.isEnableTest == UTRUE)
    {
        fcAppTestPrms = &(tCfg->fcAppTestPrms);
        status = AppFc_SetAllConfig(fcAppTestPrms, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_TEST_APP: SetAllConfig Failed \n");
            status = FVID2_EFAIL;
            return (status);
        }
    }

    return (status);
}

int32_t AppFcM2m_SetCoeff(AppFcM2m_TestPrms *tObj, uint32_t hndlIdx)
{
    int32_t status = 0;
    App_MscTestParams  *mscAppTestPrms = NULL;

    AppFcM2m_TestConfig *tCfg = &tObj->testCfg[hndlIdx];

    Vhwa_print(" FC_M2M_TEST_APP: SetCoeff %s\n", tObj->testName);

    if(tObj->isM2mMsc0Enabled || tObj->isM2mMsc1Enabled)
    {
        if(tObj->isM2mMsc0Enabled == UTRUE && tCfg->msc0AppTestPrms.isEnableTest == UTRUE)
            mscAppTestPrms = &(tCfg->msc0AppTestPrms);
        else if(tObj->isM2mMsc1Enabled == UTRUE && tCfg->msc1AppTestPrms.isEnableTest == UTRUE)
            mscAppTestPrms = &(tCfg->msc1AppTestPrms);

        if(mscAppTestPrms != NULL)
        {
            status = AppMsc_SetCoeff(mscAppTestPrms, hndlIdx, &gMscCoefTbl[0]);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" MSC0_TEST_APP: SetCoeff Failed for handle # %d\n", hndlIdx);
                status = FVID2_EFAIL;
                return (status);
            }
        }
    }

    return (status);
}

int32_t AppFcM2m_AllocBuffers(AppFcM2m_TestPrms *tObj, uint32_t hndlIdx ,
                            AppFcM2m_TestMemoryObj * fcM2mTestMemObj)
{
    int32_t            status = 0;
    uint32_t           inFrameSize = 0;
    uint32_t           outFrameSize = 0;

    AppViss_TestParams *vissAppTestPrms;
    App_MscTestParams  *msc0AppTestPrms, *msc1AppTestPrms;
    LdcApp_TestParams  *ldcAppTestPrms;
    AppFc_TestParams   *fcAppTestPrms;

    AppFcM2m_TestConfig *tCfg = &tObj->testCfg[hndlIdx];

    Vhwa_print(" FC_M2M_TEST_APP: AllocBuffers %s\n", tObj->testName);

    if(tObj->isM2mVissEnabled == UTRUE && tCfg->vissAppTestPrms.isEnableTest == UTRUE)
    {
        vissAppTestPrms = &(tCfg->vissAppTestPrms);
        status |= AppViss_AllocBuffers(vissAppTestPrms, hndlIdx,
            fcM2mTestMemObj->vissSrcBuf, &inFrameSize,
            fcM2mTestMemObj->vissDstBuf, &outFrameSize);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: VISS Alloc Buffers Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
        /* Move Buffer Index */
        fcM2mTestMemObj->vissSrcBufIdx += inFrameSize;
        fcM2mTestMemObj->vissDstBufIdx += outFrameSize;
    }

    if(tObj->isM2mMsc0Enabled == UTRUE && tCfg->msc0AppTestPrms.isEnableTest == UTRUE)
    {
        msc0AppTestPrms = &(tCfg->msc0AppTestPrms);
        status |= AppMsc_AllocBuffers(msc0AppTestPrms, hndlIdx,
            fcM2mTestMemObj->msc0SrcBuf, &inFrameSize, MSC_SRC_OFFSET,
            fcM2mTestMemObj->msc0DstBuf, &outFrameSize, MSC_DST_OFFSET);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: MSC0 Alloc Buffers Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
        /* Move Buffer Index */
        fcM2mTestMemObj->msc0SrcBufIdx += inFrameSize;
        fcM2mTestMemObj->msc0DstBufIdx += outFrameSize;
    }

    if(tObj->isM2mMsc1Enabled == UTRUE && tCfg->msc1AppTestPrms.isEnableTest == UTRUE)
    {
        msc1AppTestPrms = &(tCfg->msc1AppTestPrms);
        status |= AppMsc_AllocBuffers(msc1AppTestPrms, hndlIdx,
            fcM2mTestMemObj->msc1SrcBuf, &inFrameSize, MSC_SRC_OFFSET,
            fcM2mTestMemObj->msc1DstBuf, &outFrameSize, MSC_DST_OFFSET);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: MSC1 Alloc Buffers Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
        /* Move Buffer Index */
        fcM2mTestMemObj->msc1SrcBufIdx += inFrameSize;
        fcM2mTestMemObj->msc1DstBufIdx += outFrameSize;
    }

    if(tObj->isM2mLdcEnabled)
    {
        ldcAppTestPrms = &(tCfg->ldcAppTestPrms);
        status |= AppLdc_AllocBuffers(ldcAppTestPrms, hndlIdx,
            fcM2mTestMemObj->ldcSrcBuf, &inFrameSize,
            fcM2mTestMemObj->ldcDstBuf, &outFrameSize);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: LDC Alloc Buffers Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
        /* Move Buffer Index */
        fcM2mTestMemObj->ldcSrcBufIdx += inFrameSize;
        fcM2mTestMemObj->ldcDstBufIdx += outFrameSize;
    }

    if(tObj->isFcEnabled == UTRUE && tCfg->fcAppTestPrms.isEnableTest == UTRUE)
    {
        fcAppTestPrms = &(tCfg->fcAppTestPrms);
        status |= AppFc_AllocBuffers(fcAppTestPrms, hndlIdx,
            fcM2mTestMemObj->fcSrcBuf, &inFrameSize,
            fcM2mTestMemObj->fcDstBuf, &outFrameSize);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: FC Alloc Buffers Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
        /* Move Buffer Index */
        fcM2mTestMemObj->fcSrcBufIdx += inFrameSize;
        fcM2mTestMemObj->fcDstBufIdx += outFrameSize;
    }

    return (status);
}

