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
*  \file vhwa_int_api.c
*
*  \brief VHWA Integration sample application
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <ti/drv/fvid2/fvid2.h>
#include <include/vhwa_common.h>
#include <include/vhwa_m2mNf.h>
#include <include/vhwa_m2mViss.h>
#include <examples/include/vhwa_examples_common.h>
#include <examples/include/vhwa_int_api.h>
#include <examples/include/vhwa_viss_test_api.h>

#define MSC_SRC_OFFSET      (5u * 1024u * 1024u)
#define MSC_DST_OFFSET      (5u * 1024u * 1024u)

static Nf_WgtTableConfig nfWgtTbl = {
    NF_FILTER_MODE_BILATERAL,
    {
        #include "../vhwa_nf_test/coeff.txt"
    },
    {
        0x0
    }
};

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
/*                          Function Definitions                        */
/* ========================================================================== */

int32_t AppInt_Create(AppInt_TestPrms *tObj, uint32_t hidx)
{
    int32_t status = 0;
    NfApp_TestParams  *nfAppTestPrms;
    LdcApp_TestParams *ldcAppTestPrms;
    App_MscTestParams *msc0AppTestPrms, *msc1AppTestPrms;
    AppViss_TestParams *vissAppTestPrms;

    if(tObj->ldcTestEnabled)
    {
        ldcAppTestPrms = &(tObj->ldcAppTestPrms);
        status |= AppLdc_Create(ldcAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: LDC Create Failed for handle # %d\n",
                hidx);
            return status;
        }
    }

    if(tObj->nfTestEnabled)
    {
        nfAppTestPrms = &(tObj->nfAppTestPrms);
        status |= AppNf_Create(nfAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: NF Create Failed for handle # %d\n",
                hidx);
            return status;
        }
    }

    if(tObj->msc0TestEnabled)
    {
        msc0AppTestPrms = &(tObj->msc0AppTestPrms);
        status |= AppMsc_Create(msc0AppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: MSC0 Create Failed for handle # %d\n",
                hidx);
            return status;
        }
    }

    if(tObj->msc1TestEnabled)
    {
        msc1AppTestPrms = &(tObj->msc1AppTestPrms);
        status |= AppMsc_Create(msc1AppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: MSC1 Create Failed for handle # %d\n",
                hidx);
            return status;
        }
    }

    if(tObj->vissTestEnabled)
    {
        vissAppTestPrms = &(tObj->vissAppTestPrms);
        status |= AppViss_Create(vissAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: VISS Create Failed for handle # %d\n",
                hidx);
            return status;
        }
    }

    return status;
}

int32_t AppInt_Delete(AppInt_TestPrms *tObj, uint32_t hidx)
{
    int32_t status = 0;
    NfApp_TestParams  *nfAppTestPrms;
    LdcApp_TestParams *ldcAppTestPrms;
    App_MscTestParams *msc0AppTestPrms, *msc1AppTestPrms;
    AppViss_TestParams *vissAppTestPrms;

    if(tObj->ldcTestEnabled)
    {
        ldcAppTestPrms = &(tObj->ldcAppTestPrms);
        AppLdc_Delete(ldcAppTestPrms, hidx);
    }

    if(tObj->nfTestEnabled)
    {
        nfAppTestPrms = &(tObj->nfAppTestPrms);
        AppNf_Delete(nfAppTestPrms, hidx);
    }

    if(tObj->msc0TestEnabled)
    {
        msc0AppTestPrms = &(tObj->msc0AppTestPrms);
        AppMsc_Delete(msc0AppTestPrms, hidx);
    }

    if(tObj->msc1TestEnabled)
    {
        msc1AppTestPrms = &(tObj->msc1AppTestPrms);
        AppMsc_Delete(msc1AppTestPrms, hidx);
    }

    if(tObj->vissTestEnabled)
    {
        vissAppTestPrms = &(tObj->vissAppTestPrms);
        AppViss_Delete(vissAppTestPrms, hidx);
    }

    return status;
}

int32_t AppInt_Init(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = 0;
    Vhwa_M2mLdcSl2AllocPrms ldcSl2Prms;
    Vhwa_M2mLdcInitParams   ldcInitPrms;
    Vhwa_M2mMscInitParams   mscInitPrms;
    Vhwa_M2mMscSl2AllocPrms mscSl2Prms;
    Vhwa_M2mVissSl2Params   vissSl2Prms;
    Vhwa_M2mVissInitParams  vissInitPrms;
    Vhwa_M2mNfSl2AllocPrms  nfSl2Prms;
    Vhwa_M2mNfInitPrms      nfInitPrms;

    Vhwa_print(" INT_TEST_APP: Init\n");

        /* Initialize LDC Init parameters */
        Vhwa_m2mLdcInitParamsInit(&ldcInitPrms);

        /* Set UDMA driver handle */
        ldcInitPrms.udmaDrvHndl = udmaDrvHndl;

        status |= Vhwa_m2mLdcInit(&ldcInitPrms);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: LDC Init Failed\n");
        }
        else
        {
            Vhwa_M2mLdcSl2AllocPrmsInit(&ldcSl2Prms);

            status |= Vhwa_m2mLdcAllocSl2(&ldcSl2Prms);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" INT_TEST_APP: LDC SL2 Alloc Failed !!!\n");
            }
        }

        Vhwa_m2mNfInitPrmsInit(&nfInitPrms);

        /* Set UDMA driver handle */
        nfInitPrms.udmaDrvHndl = udmaDrvHndl;

        status |= Vhwa_m2mNfInit(&nfInitPrms);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" NF_TEST_APP: NF Init Failed\n");
        }
        else
        {
            /* Initilize SL2 parameters */
            Vhwa_M2mNfSl2AllocPrmsInit(&nfSl2Prms);

            status |= Vhwa_m2mNfAllocSl2(&nfSl2Prms);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" INT_TEST_APP: NF SL2 Alloc Failed !!!\n");
            }
        }

        Vhwa_m2mMscInitParamsInit(&mscInitPrms);

        mscInitPrms.drvHandle = udmaDrvHndl;

        status |= Vhwa_m2mMscInit(&mscInitPrms);

        if (FVID2_SOK == status)
        {
            Vhwa_M2mMscSl2AllocPrmsInit(&mscSl2Prms);

            status |= Vhwa_m2mMscAllocSl2(&mscSl2Prms);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" INT_TEST_APP: MSC SL2 Alloc Failed !!!\n");
            }
        }

        if(FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: MSC Init Failed\n");
        }

        /* Initialize VISS Init parameters */
        Vhwa_m2mVissInitParamsInit(&vissInitPrms);

        /* Set UDMA driver handle */
        vissInitPrms.udmaDrvHndl = udmaDrvHndl;

        status |= Vhwa_m2mVissInit(&vissInitPrms);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: VISS Init Failed\n");
        }
        else
        {
            Vhwa_m2mVissSl2ParamsInit(&vissSl2Prms);

            status |= Vhwa_m2mVissAllocSl2(&vissSl2Prms);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" INT_TEST_APP: VISS SL2 Alloc Failed !!!\n");
            }
        }

    return (status);
}

int32_t AppInt_CrcInit(AppInt_TestPrms *tObj, Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = 0;

    if(tObj->ldcTestEnabled)
    {
        status |= AppLdc_CrcInit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: LDC CRC Init Failed\n");
        }
    }

    if(tObj->nfTestEnabled)
    {
        status |= AppNf_CrcInit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: NF CRC Init Failed\n");
        }
    }

    if(tObj->msc0TestEnabled || tObj->msc1TestEnabled)
    {
        status |= AppMsc_CrcInit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: MSC CRC Init Failed\n");
        }
    }

    if(tObj->vissTestEnabled)
    {
        status |= AppViss_CrcInit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: VISS CRC Init Failed\n");
        }
    }

    return (status);
}

int32_t AppInt_CrcDeinit(AppInt_TestPrms *tObj, Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = 0;

    if(tObj->ldcTestEnabled)
    {
        status |= AppLdc_CrcDeinit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: LDC CRC Init Failed\n");
        }
    }

    if(tObj->nfTestEnabled)
    {
        status |= AppNf_CrcDeinit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: NF CRC Init Failed\n");
        }
    }

    if(tObj->msc0TestEnabled || tObj->msc1TestEnabled)
    {
        status |= AppMsc_CrcDeinit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: MSC CRC Init Failed\n");
        }
    }

    if(tObj->vissTestEnabled)
    {
        status |= AppViss_CrcDeinit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: VISS CRC Init Failed\n");
        }
    }

    return (status);
}

int32_t AppInt_SetParams(AppInt_TestPrms *tObj, uint32_t hidx)
{
    int32_t status = 0;
    NfApp_TestParams   *nfAppTestPrms;
    LdcApp_TestParams  *ldcAppTestPrms;
    App_MscTestParams  *msc0AppTestPrms, *msc1AppTestPrms;
    AppViss_TestParams *vissAppTestPrms;

    Vhwa_print(" INT_TEST_APP: SetParams %s\n", tObj->integrationTestName);

    if(tObj->ldcTestEnabled)
    {
        ldcAppTestPrms = &(tObj->ldcAppTestPrms);
        status = AppLdc_SetParams(ldcAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: LDC SetParams Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
    }

    if(tObj->nfTestEnabled)
    {
        nfAppTestPrms = &(tObj->nfAppTestPrms);
        status = AppNf_SetParams(nfAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: NF SetParams Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
    }

    if(tObj->msc0TestEnabled)
    {
        msc0AppTestPrms = &(tObj->msc0AppTestPrms);
        status = AppMsc_SetParams(msc0AppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: MSC0 SetParams Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
    }

    if(tObj->msc1TestEnabled)
    {
        msc1AppTestPrms = &(tObj->msc1AppTestPrms);
        status = AppMsc_SetParams(msc1AppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: MSC1 SetParams Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
    }

    if(tObj->vissTestEnabled)
    {
        vissAppTestPrms = &(tObj->vissAppTestPrms);
        status = AppViss_SetAllConfig(vissAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: VISS SetParams Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
    }

    return (status);
}

int32_t AppInt_SetCoeff(AppInt_TestPrms *tObj, uint32_t hndlIdx)
{
    int32_t status = 0;
    App_MscTestParams *mscAppTestPrms;
    AppViss_TestParams *vissAppTestPrms;

    Vhwa_print(" INT_TEST_APP: SetCoeff %s\n", tObj->integrationTestName);

    if(tObj->nfTestEnabled)
    {
        if (0 == hndlIdx)
        {
            status = AppNf_SetCoeff(&nfWgtTbl,hndlIdx);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" NF_TEST_APP: SetCoeff Failed for handle # %d\n", hndlIdx);
                status = FVID2_EFAIL;
                return (status);
            }
        }
    }

    if(tObj->msc0TestEnabled || tObj->msc1TestEnabled)
    {
        if (tObj->msc0TestEnabled)
            mscAppTestPrms = &(tObj->msc0AppTestPrms);
        else if (tObj->msc1TestEnabled)
            mscAppTestPrms = &(tObj->msc1AppTestPrms);

        if (0 == hndlIdx)
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

    if(tObj->vissTestEnabled)
    {
        vissAppTestPrms = &(tObj->vissAppTestPrms);
        status = AppViss_SetAllConfig(vissAppTestPrms, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" VISS_TEST_APP: SetAllConfig Failed \n");
            status = FVID2_EFAIL;
            return (status);
        }
    }

    return (status);
}

int32_t AppInt_AllocBuffers(AppInt_TestPrms *tObj, uint32_t hndlIdx ,
                            AppInt_TestMemoryObj * intTestMemObj)
{
    int32_t status = 0;
    uint32_t           inFrameSize = 0;
    uint32_t           outFrameSize = 0;
    NfApp_TestParams  *nfAppTestPrms;
    LdcApp_TestParams *ldcAppTestPrms;
    App_MscTestParams *msc0AppTestPrms, *msc1AppTestPrms;
    AppViss_TestParams *vissAppTestPrms;

    Vhwa_print(" INT_TEST_APP: AllocBuffers %s\n", tObj->integrationTestName);

    if(tObj->ldcTestEnabled)
    {
        ldcAppTestPrms = &(tObj->ldcAppTestPrms);
        status |= AppLdc_AllocBuffers(ldcAppTestPrms, hndlIdx,
            intTestMemObj->ldcSrcBuf, &inFrameSize,
            intTestMemObj->ldcDstBuf, &outFrameSize);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: LDC Alloc Buffers Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
        /* Move Buffer Index */
        intTestMemObj->ldcSrcBufIdx += inFrameSize;
        intTestMemObj->ldcDstBufIdx += outFrameSize;
    }

    if(tObj->nfTestEnabled)
    {
        nfAppTestPrms = &(tObj->nfAppTestPrms);
        status |= AppNf_AllocBuffers(nfAppTestPrms, hndlIdx,
            intTestMemObj->nfSrcBuf, &inFrameSize,
            intTestMemObj->nfDstBuf, &outFrameSize);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: NF Alloc Buffers Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
        /* Move Buffer Index */
        intTestMemObj->nfSrcBufIdx += inFrameSize;
        intTestMemObj->nfDstBufIdx += outFrameSize;
    }

    if(tObj->msc0TestEnabled)
    {
        msc0AppTestPrms = &(tObj->msc0AppTestPrms);
        status |= AppMsc_AllocBuffers(msc0AppTestPrms, hndlIdx,
            intTestMemObj->msc0SrcBuf, &inFrameSize, MSC_SRC_OFFSET,
            intTestMemObj->msc0DstBuf, &outFrameSize, MSC_DST_OFFSET);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: MSC0 Alloc Buffers Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
        /* Move Buffer Index */
        intTestMemObj->msc0SrcBufIdx += inFrameSize;
        intTestMemObj->msc0DstBufIdx += outFrameSize;
    }

    if(tObj->msc1TestEnabled)
    {
        msc1AppTestPrms = &(tObj->msc1AppTestPrms);
        status |= AppMsc_AllocBuffers(msc1AppTestPrms, hndlIdx,
            intTestMemObj->msc1SrcBuf, &inFrameSize, MSC_SRC_OFFSET,
            intTestMemObj->msc1DstBuf, &outFrameSize, MSC_DST_OFFSET);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: MSC1 Alloc Buffers Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
        /* Move Buffer Index */
        intTestMemObj->msc1SrcBufIdx += inFrameSize;
        intTestMemObj->msc1DstBufIdx += outFrameSize;
    }

    if(tObj->vissTestEnabled)
    {
        vissAppTestPrms = &(tObj->vissAppTestPrms);
        status |= AppViss_AllocBuffers(vissAppTestPrms, hndlIdx,
            intTestMemObj->vissSrcBuf, &inFrameSize,
            intTestMemObj->vissDstBuf, &outFrameSize);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: VISS Alloc Buffers Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
        /* Move Buffer Index */
        intTestMemObj->vissSrcBufIdx += inFrameSize;
        intTestMemObj->vissDstBufIdx += outFrameSize;
    }

    return (status);
}

