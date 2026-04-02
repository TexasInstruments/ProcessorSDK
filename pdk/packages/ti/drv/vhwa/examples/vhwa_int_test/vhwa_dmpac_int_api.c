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
#include <examples/include/vhwa_examples_common.h>
#include <examples/include/vhwa_int_api.h>



/* ========================================================================== */
/*                          Function Definitions                        */
/* ========================================================================== */

int32_t AppInt_Create(AppInt_TestPrms *tObj, uint32_t hidx)
{
    int32_t status = 0;
    AppSde_TestParams *sdeAppTestPrms;
    DofApp_TestParams *dofAppTestPrms;

    if(tObj->sdeTestEnabled)
    {
        sdeAppTestPrms = &(tObj->sdeAppTestPrms);
        status |= AppSde_Create(sdeAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: SDE Create Failed for handle # %d\n",
                hidx);
            return status;
        }
    }

    if(tObj->dofTestEnabled)
    {
        dofAppTestPrms = &(tObj->dofAppTestPrms);
        status |= AppDof_Create(dofAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: DOF Create Failed for handle # %d\n",
                hidx);
            return status;
        }
    }

    return status;
}

int32_t AppInt_Delete(AppInt_TestPrms *tObj, uint32_t hidx)
{
    int32_t status = 0;
    AppSde_TestParams *sdeAppTestPrms;
    DofApp_TestParams *dofAppTestPrms;

    if(tObj->sdeTestEnabled)
    {
        sdeAppTestPrms = &(tObj->sdeAppTestPrms);
        AppSde_Delete(sdeAppTestPrms, hidx);
    }

    if(tObj->dofTestEnabled)
    {
        dofAppTestPrms = &(tObj->dofAppTestPrms);
        AppDof_Delete(dofAppTestPrms, hidx);
    }

    return status;
}

int32_t AppInt_Init(Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = 0;
    Vhwa_M2mDofSl2AllocPrms dofSl2Prms;
    Vhwa_M2mDofInitParams   dofInitPrms;
    Vhwa_M2mSdeSl2AllocPrms sdeSl2Prms;
    Vhwa_M2mSdeInitParams   sdeInitPrms;

    Vhwa_print(" INT_TEST_APP: Init\n");

        /* Initialize SDE Init parameters */
        Vhwa_M2mSdeInitParamsInit(&sdeInitPrms);

        /* Set UDMA driver handle */
        sdeInitPrms.udmaDrvHndl = udmaDrvHndl;

        status |= Vhwa_m2mSdeInit(&sdeInitPrms);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: SDE Init Failed\n");
        }
        else
        {
            /* Initilize SL2 parameters */
            Vhwa_M2mSdeSl2AllocPrmsInit(&sdeSl2Prms);

            status |= Vhwa_m2mSdeAllocSl2(&sdeSl2Prms);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" INT_TEST_APP: SDE SL2 Alloc Failed !!!\n");
            }
        }

        /* Initialize DOF Init parameters */
        Vhwa_m2mDofInitParamsInit(&dofInitPrms);

        /* Set UDMA driver handle */
        dofInitPrms.udmaDrvHndl = udmaDrvHndl;

        status |= Vhwa_m2mDofInit(&dofInitPrms);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: DOF Init Failed\n");
        }
        else
        {
            /* Initilize SL2 parameters */
            Vhwa_M2mDofSl2AllocPrmsInit(&dofSl2Prms);

            status |= Vhwa_m2mDofAllocSl2(&dofSl2Prms);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" INT_TEST_APP: DOF SL2 Alloc Failed !!!\n");
            }
        }


    return (status);
}

int32_t AppInt_CrcInit(AppInt_TestPrms *tObj, Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = 0;


    if(tObj->sdeTestEnabled)
    {
        status |= AppSde_CrcInit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: SDE CRC Init Failed\n");
        }
    }

    if(tObj->dofTestEnabled)
    {
        status |= AppDof_CrcInit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: DOF CRC Init Failed\n");
        }
    }


    return (status);
}

int32_t AppInt_CrcDeinit(AppInt_TestPrms *tObj, Udma_DrvHandle udmaDrvHndl)
{
    int32_t status = 0;

    if(tObj->sdeTestEnabled)
    {
        status |= AppSde_CrcDeinit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: SDE CRC Init Failed\n");
        }
    }

    if(tObj->dofTestEnabled)
    {
        status |= AppDof_CrcDeinit(udmaDrvHndl);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: DOF CRC Init Failed\n");
        }
    }


    return (status);
}

int32_t AppInt_SetParams(AppInt_TestPrms *tObj, uint32_t hidx)
{
    int32_t status = 0;
    AppSde_TestParams  *sdeAppTestPrms;
    DofApp_TestParams  *dofAppTestPrms;

    Vhwa_print(" INT_TEST_APP: SetParams %s\n", tObj->integrationTestName);

    if(tObj->sdeTestEnabled)
    {
        sdeAppTestPrms = &(tObj->sdeAppTestPrms);
        status = AppSde_SetParams(sdeAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: SDE SetParams Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
    }

    if(tObj->dofTestEnabled)
    {
        dofAppTestPrms = &(tObj->dofAppTestPrms);
        status = AppDof_SetParams(dofAppTestPrms, hidx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: DOF SetParams Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
    }

    return (status);
}

int32_t AppInt_SetCoeff(AppInt_TestPrms *tObj, uint32_t hndlIdx)
{
    int32_t status = 0;
    DofApp_TestParams *dofAppTestPrms;

    Vhwa_print(" INT_TEST_APP: SetCoeff %s\n", tObj->integrationTestName);

    if(tObj->dofTestEnabled)
    {
        dofAppTestPrms = &(tObj->dofAppTestPrms);
        if (0 == hndlIdx)
        {
            status = AppDof_SetConfScoreParam(dofAppTestPrms, hndlIdx);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" DOF_TEST_APP: SetConfScore Failed for handle # %d\n", hndlIdx);
                status = FVID2_EFAIL;
                return (status);
            }
        }
    }

    return (status);
}

int32_t AppInt_AllocBuffers(AppInt_TestPrms *tObj, uint32_t hndlIdx ,
                            AppInt_TestMemoryObj * intTestMemObj)
{
    int32_t status = 0;
    uint32_t           inFrameSize = 0;
    uint32_t           inBaseBuffSize = 0;
    uint32_t           outFrameSize = 0;
    AppSde_TestParams *sdeAppTestPrms;
    DofApp_TestParams *dofAppTestPrms;

    Vhwa_print(" INT_TEST_APP: AllocBuffers %s\n", tObj->integrationTestName);

    if(tObj->sdeTestEnabled)
    {
        sdeAppTestPrms = &(tObj->sdeAppTestPrms);
        status |= AppSde_AllocBuffers(sdeAppTestPrms, hndlIdx,
            intTestMemObj->sdeBaseBuf, &inBaseBuffSize,
            intTestMemObj->sdeRefBuf, &inFrameSize,
            intTestMemObj->sdeDstBuf, &outFrameSize);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: SDE Alloc Buffers Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
        /* Move Buffer Index */
        intTestMemObj->sdeBaseBufFreeIdx += inBaseBuffSize;
        intTestMemObj->sdeRefBufFreeIdx += inFrameSize;
        intTestMemObj->sdeDstBufFreeIdx += outFrameSize;
    }

    if(tObj->dofTestEnabled)
    {
        dofAppTestPrms = &(tObj->dofAppTestPrms);

        dofAppTestPrms->buffAddr.refBuff = intTestMemObj->dofRefBuf;
        dofAppTestPrms->buffAddr.currBuff = intTestMemObj->dofCurBuf;
        dofAppTestPrms->buffAddr.tempBuff = intTestMemObj->dofTempBuf;
        dofAppTestPrms->buffAddr.sofBuff = intTestMemObj->dofSofBuf;
        dofAppTestPrms->buffAddr.outBuff = intTestMemObj->dofDstBuf;
        dofAppTestPrms->buffAddr.interOddBuff = intTestMemObj->dofInterOddBuf;
        dofAppTestPrms->buffAddr.interEvenBuff = intTestMemObj->dofInterEvenBuf;
        dofAppTestPrms->buffAddr.refBuffIdx = &intTestMemObj->dofRefBufFreeIdx;
        dofAppTestPrms->buffAddr.currBuffIdx = &intTestMemObj->dofCurBufFreeIdx;
        dofAppTestPrms->buffAddr.tempBuffIdx = &intTestMemObj->dofTempBufFreeIdx;
        dofAppTestPrms->buffAddr.sofBuffIdx = &intTestMemObj->dofSofBufFreeIdx;
        dofAppTestPrms->buffAddr.outBuffIdx = &intTestMemObj->dofDstBufFreeIdx;

        status |= AppDof_AllocBuffers(dofAppTestPrms, hndlIdx,
                                      &dofAppTestPrms->buffAddr);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" INT_TEST_APP: DOF Alloc Buffers Failed \n");
            status = FVID2_EFAIL;
            return status;
        }
    }

    return (status);
}
