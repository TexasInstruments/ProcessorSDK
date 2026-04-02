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
#include <include/vhwa_m2mDof.h>
#include <examples/include/vhwa_dof_api.h>
#include "vhwa_dof_cfg.h"


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Input Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be loaded in one shot */
#define VHWA_DOF_TEST_IN_BUF_OFFSET     (8*1024*1024)

/* Output Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be saved in one shot */
#define VHWA_DOF_TEST_OUT_BUF_OFFSET    (16*1024*1024)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t DofApp_init(void);
static void DofApp_deInit(void);
static void AppDof_Test(DofApp_TestParams *tObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*
* Application Buffers
*/
/* Buffers to store input and out data */
#if defined VHWA_PDK
static uint64_t gRefSrcBuf      = 0x90000000U;
static uint64_t gCurrSrcBuf     = 0xA0000000U;
static uint64_t gTempSrcBuf     = 0xB0000000U;
static uint64_t gOutBuf         = 0xC0000000U;
static uint64_t gSofSrcBuf      = 0x98000000U;
static uint64_t gInterOddBuf    = 0xD0000000U;
static uint64_t gInterEvenBuf   = 0xE0000000U;
#elif defined VHWA_MCU_PLUS_SDK
#define gDofDDRSection  0xB0000000U
#define buffOffset      0x2000000U
static uint64_t gRefSrcBuf      = gDofDDRSection + (1* buffOffset); //0xC2000000 
static uint64_t gCurrSrcBuf     = gDofDDRSection + (2* buffOffset); //0xC4000000
static uint64_t gTempSrcBuf     = gDofDDRSection + (3* buffOffset); //0xC6000000
static uint64_t gOutBuf         = gDofDDRSection + (4* buffOffset); //0xC8000000
static uint64_t gSofSrcBuf      = gDofDDRSection + (5* buffOffset); //0xCA000000
static uint64_t gInterOddBuf    = gDofDDRSection + (6* buffOffset); //0xCC000000
static uint64_t gInterEvenBuf   = gDofDDRSection + (7* buffOffset); //0xCE000000
#endif
static uint32_t gRefBufFreeIdx = 0u;
static uint32_t gCurrBufFreeIdx = 0u;
static uint32_t gTempBufFreeIdx = 0u;
static uint32_t gSofBufFreeIdx = 0u;
static uint32_t gOutBufFreeIdx = 0u;

static AppDof_TestConfig gAppDofTestCfg[] =
{
    #include <examples/include/vhwa_dof_test_cfg.h>
};

static DofApp_TestParams gAppDofObj[] = VHWA_DOF_TIRTOS_CFG;

static VhwaAl_Udma_DrvObj_Struct gDofAppUdmaDrvObj;



/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    int32_t                 status;
    uint32_t                testCnt;
    DofApp_TestParams      *tObj = NULL;

    #if defined VHWA_MCU_PLUS_SDK
    System_init();
    Board_init();
    #endif
    status = DofApp_init();

    #if defined VHWA_PDK
    App_startTimer();
    #endif

    if (FVID2_SOK == status)
    {
        for (testCnt = 0u; testCnt <
                (sizeof(gAppDofObj) / sizeof(DofApp_TestParams)); testCnt ++)
        {
            tObj = &gAppDofObj[testCnt];

            gRefBufFreeIdx = 0u;
            gCurrBufFreeIdx = 0u;
            gTempBufFreeIdx = 0u;
            gSofBufFreeIdx = 0u;
            gOutBufFreeIdx = 0u;

            /* Only single Testcases are supported */
            if ((UTRUE == tObj->isEnableTest) && (1U == tObj->numHandles))
            {
                Vhwa_print (" Starting Test %s\n", tObj->testName);
                AppDof_Test(tObj);
            }
        }
    }

    Vhwa_print (" Exiting \n");
    DofApp_deInit();
    #if defined VHWA_MCU_PLUS_SDK
    Board_deinit();
    System_deinit();
    #endif

    return(0);
}

static void AppDof_Test(DofApp_TestParams *tObj)
{
    int32_t            status;
    uint32_t           rCnt, pyrCnt;
     uint64_t          timeCount1 = 0;
    uint64_t           timeCount2;
    uint64_t           srTime[6] = {0,0,0,0,0,0}, wtTime[6]= {0,0,0,0,0,0}, prTime = 0, pyrSrTime = 0, pyrWtTime = 0;

    status = AppDof_Create(tObj, 0U);
    if (FVID2_SOK != status)
    {
        Vhwa_print(" DOF_TEST_APP: Create Failed\n");
        status = FVID2_EFAIL;
    }

    status = AppDof_SetParams(tObj, 0U);
    if (FVID2_SOK != status)
    {
        Vhwa_print(" DOF_TEST_APP: SetParams Failed \n");
    }

    status = AppDof_SetConfScoreParam(tObj, 0U);
    if (FVID2_SOK != status)
    {
        Vhwa_print("DOF_TEST_APP: SetConfScoreParam Failed \n");
    }

    if (FVID2_SOK == status)
    {
        tObj->buffAddr.interOddBuff = gInterOddBuf;
        tObj->buffAddr.interEvenBuff = gInterEvenBuf;

        tObj->buffAddr.refBuff = gRefSrcBuf + gRefBufFreeIdx;
        tObj->buffAddr.currBuff = gCurrSrcBuf + gCurrBufFreeIdx;
        tObj->buffAddr.tempBuff = gTempSrcBuf + gTempBufFreeIdx;
        tObj->buffAddr.sofBuff = gSofSrcBuf + gSofBufFreeIdx;
        tObj->buffAddr.outBuff = gOutBuf + gOutBufFreeIdx;
        tObj->buffAddr.refBuffIdx = &gRefBufFreeIdx;
        tObj->buffAddr.currBuffIdx = &gCurrBufFreeIdx;
        tObj->buffAddr.tempBuffIdx = &gTempBufFreeIdx;
        tObj->buffAddr.sofBuffIdx = &gSofBufFreeIdx;
        tObj->buffAddr.outBuffIdx = &gOutBufFreeIdx;

        AppDof_AllocBuffers(tObj, 0U, &tObj->buffAddr);
    }

    if(UTRUE == tObj->isPerformanceTest)
    {
        #if defined VHWA_PDK
        App_startTimer();
        #elif defined VHWA_MCU_PLUS_SDK
        ClockP_init();
        #endif
        timeCount1 = Vhwa_getTime();
    }
    for (rCnt = 0u; (rCnt < tObj->repeatCnt) && (FVID2_SOK == status); rCnt ++)
    {
        if(UTRUE == tObj->isPerformanceTest)
        {
            timeCount2 = Vhwa_getTime();
        }
        AppDof_PrepareRequest(tObj, 0U);
        
        if(UTRUE == tObj->isPerformanceTest)
        {
                prTime = prTime + (Vhwa_getTime() - timeCount2);
        }

        for (pyrCnt = tObj->testCfg[0]->tPyrLvl; pyrCnt > 0; pyrCnt--)
        {
            if(UTRUE == tObj->isPerformanceTest)
            {
                timeCount2 = Vhwa_getTime();
            }
            /* Submit Request*/
            status = AppDof_SubmitRequest(tObj, 0U);
            if(UTRUE == tObj->isPerformanceTest)
            {
                srTime[pyrCnt-1] =srTime[pyrCnt-1] + (Vhwa_getTime() - timeCount2);
            }

            if (FVID2_SOK == status)
            {
                if(UTRUE == tObj->isPerformanceTest)
                {
                    timeCount2 = Vhwa_getTime();
                }
                /* Wait for Request Completion */
                status = AppDof_WaitForComplRequest(tObj, 0U);
                if(UTRUE == tObj->isPerformanceTest)
                {
                    wtTime[pyrCnt-1] = wtTime[pyrCnt-1] + (Vhwa_getTime() - timeCount2);
                }
                if (FVID2_SOK != status)
                {
                    Vhwa_print (" Failed to wait for request completion \n");
                }
            }
            else
            {
                Vhwa_print (" Failed to Submit Request \n");
            }
        }

        if (FVID2_SOK != status)
        {
            break;
        }
        else
        {
            if(UFALSE == tObj->isPerformanceTest)
            {
                Vhwa_print (" Completed RepeatCnt %d\n\n", rCnt);
            }
        }
    }

    if(UTRUE == tObj->isPerformanceTest)
    {
        timeCount1 = Vhwa_getTime() - timeCount1;
        
        Vhwa_print ("Performance:\n\t FrameCount: %d, Ticks: %llu, Time in uSec: %.2f\n",
                    rCnt, timeCount1, (((float)timeCount1 )* 0.004f));
        for (pyrCnt = tObj->testCfg[0]->tPyrLvl; pyrCnt > 0; pyrCnt--)
        {
            Vhwa_print ("Pyramid level:%d\n\t Submit request time:: Ticks: %llu, Time in uSec: %.2f\n\t Wait  time:: Ticks: %llu, Time in uSec: %.2f\n", 
            pyrCnt, srTime[pyrCnt-1], (((float)srTime[pyrCnt-1])* 0.004f), wtTime[pyrCnt-1], (((float)wtTime[pyrCnt-1])* 0.004f));
            pyrSrTime = pyrSrTime + srTime[pyrCnt-1];
            pyrWtTime = pyrWtTime + wtTime[pyrCnt-1];
        }
        Vhwa_print ("Total times:\n\t Request preparation time: Ticks: %llu, Time in uSec: %.2f\n\t Request submission time: Ticks: %llu, Time in uSec: %.2f\n\t Processing competion wait time: Ticks: %llu, Time in uSec: %.2f\n", 
        prTime, (((float)prTime)* 0.004f), pyrSrTime, (((float)pyrSrTime)* 0.004f), pyrWtTime, (((float)pyrWtTime)* 0.004f));
    }
    AppDof_Delete(tObj, 0U);
}

static int32_t DofApp_init(void)
{
    int32_t                 status;
    uint32_t                instId;
    Udma_DrvHandle          drvHandle = &gDofAppUdmaDrvObj;
    Udma_InitPrms           udmaInitPrms;
    #if defined VHWA_PDK
    Board_initCfg           boardCfg;
    Fvid2_InitPrms initPrmsFvid2;

    boardCfg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    Fvid2InitPrms_init(&initPrmsFvid2);
    initPrmsFvid2.printFxn = Vhwa_print;
    status = Fvid2_init(&initPrmsFvid2);
    #else

    #if defined (SOC_J722S)
    Sciclient_pmSetModuleState(TISCI_DEV_DMPAC0,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SystemP_WAIT_FOREVER);
    #endif
    status = Fvid2_init(NULL);
    #endif

    if (FVID2_SOK != status)
    {
        Vhwa_print(" main: FVID2_Init Failed !!!\r\n");
    }

    if (FVID2_SOK == status)
    {
        /* Initialize UDMA and get the handle, it will be used in both CRC layer,
           as well as in the driver */
        /* UDMA driver init */
        instId = VHWA_UDMA_INST_ID_0;
        status = UdmaInitPrms_init(instId, &udmaInitPrms);
        if(UDMA_SOK != status)
        {
            Vhwa_print ("[Error] UDMA prms init failed!!\n");
            status = FVID2_EFAIL;
        }
        #if defined VHWA_PDK
        udmaInitPrms.printFxn = &App_udmaPrint;
        #else
        udmaInitPrms.instId = VHWA_UDMA_INST_ID_0;
        udmaInitPrms.enableUtc = UTRUE;
        #endif
        status = Udma_init(drvHandle, &udmaInitPrms); 
        if(UDMA_SOK != status)
        {
            Vhwa_print("[Error] UDMA init failed!!\n");
            status = FVID2_EFAIL;
        }
    }

    status = AppDof_Init(drvHandle);
    if (FVID2_SOK == status)
    {
        /* Initialize the UDMA channel for CRC */
        #if defined VHWA_PDK
        status = AppDof_CrcInit(drvHandle);
        #endif
    }

    return (status);
}

static void DofApp_deInit(void)
{
    int32_t         status;
    Udma_DrvHandle  drvHandle = &gDofAppUdmaDrvObj;

    Vhwa_m2mDofDeInit();

    #if defined VHWA_PDK
    AppDof_CrcDeinit(drvHandle);
    #endif

    status = Udma_deinit(drvHandle);
    if(UDMA_SOK != status)
    {
        Vhwa_print("[Error] UDMA deinit failed!!\n");
    }

    Fvid2_deInit(NULL);
}
