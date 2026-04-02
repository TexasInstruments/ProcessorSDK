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
#include <string.h>
#include <stdlib.h>

#include <drivers/vhwa/include/vhwa_m2mDof.h>
#include "vhwa_dof_api.h"
#include "vhwa_dof_cfg.h"

#include <kernel/dpl/DebugP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include <drivers/gtc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Input Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be loaded in one shot */
#define VHWA_DOF_TEST_IN_BUF_OFFSET     (8*1024*1024)

/* Output Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be saved in one shot */
#define VHWA_DOF_TEST_OUT_BUF_OFFSET    (16*1024*1024)
#define J7AEN_AVV_RELEASE 0

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t DofApp_init(void);
static void DofApp_deInit(void);
static void App_udmaPrint(const char *str);
static void AppDof_Test(DofApp_TestParams *tObj);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*
* Application Buffers
*/
/* Buffers to store input and out data */

    #define gDofDDRSection  0xB0000000U
    #define buffOffset      0x2000000U
    
    //uint64_t gLogBuffer      = gDofDDRSection + (0x1000000U);
    static uint64_t gRefSrcBuf      = gDofDDRSection + (1* buffOffset); //0xC2000000 
    static uint64_t gCurrSrcBuf     = gDofDDRSection + (2* buffOffset); //0xC4000000
    static uint64_t gTempSrcBuf     = gDofDDRSection + (3* buffOffset); //0xC6000000
    static uint64_t gOutBuf         = gDofDDRSection + (4* buffOffset); //0xC8000000
    static uint64_t gSofSrcBuf      = gDofDDRSection + (5* buffOffset); //0xCA000000
    static uint64_t gInterOddBuf    = gDofDDRSection + (6* buffOffset); //0xCC000000
    static uint64_t gInterEvenBuf   = gDofDDRSection + (7* buffOffset); //0xCE000000
    static uint32_t gRefBufFreeIdx  = 0u;
    static uint32_t gCurrBufFreeIdx = 0u;
    static uint32_t gTempBufFreeIdx = 0u;
    static uint32_t gSofBufFreeIdx  = 0u;
    static uint32_t gOutBufFreeIdx  = 0u;

static AppDof_TestConfig gAppDofTestCfg[] = {
#include "vhwa_dof_test_cfg.h" 
};

static DofApp_TestParams gAppDofObj[] = VHWA_DOF_TIRTOS_CFG;

static Udma_DrvObjectInt gDofAppUdmaDrvObj;


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void AppDofMain(void *args)
{
    int32_t                 status;
    uint32_t                testCnt;
    DofApp_TestParams      *tObj = NULL;
    int testCaseID = -1;

    DebugP_log (" Enter test ID to run [-1 for all tests, 0 >=  Specific test case]: ");
    scanf("%d", &testCaseID);

    status = DofApp_init();
    
    //App_startTimer();
    if (FVID2_SOK == status)
    {
        if(testCaseID == -1) 
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
                if ((TRUE == tObj->isEnableTest) && (1U == tObj->numHandles))
                {
                    DebugP_log (" Starting Test %s\n", tObj->testName);
                    AppDof_Test(tObj);
                }
            }
        } 
        else if (testCaseID >= 0  && testCaseID < (sizeof(gAppDofObj) / sizeof(DofApp_TestParams)))
        {
                testCnt = testCaseID; 
                tObj = &gAppDofObj[testCnt];
    
                gRefBufFreeIdx = 0u;
                gCurrBufFreeIdx = 0u;
                gTempBufFreeIdx = 0u;
                gSofBufFreeIdx = 0u;
                gOutBufFreeIdx = 0u;
    
                /* Only single Testcases are supported */
                if ((TRUE == tObj->isEnableTest) && (1U == tObj->numHandles))
                {
                    DebugP_log (" Starting Test %s\n", tObj->testName);
                    AppDof_Test(tObj);
                }
        }
        else
        {
            DebugP_log (" Invalid test ID: %s\n", testCaseID);
        }
    }
    
    DebugP_log (" Exiting \n");
    DofApp_deInit();
    return;
}

static void AppDof_Test(DofApp_TestParams *tObj)
{
    int32_t            status;
    uint32_t           rCnt, pyrCnt;

    status = AppDof_Create(tObj, 0U);
    if (FVID2_SOK != status)
    {
        DebugP_log(" DOF_TEST_APP: Create Failed\n");
        status = FVID2_EFAIL;
    }

    status = AppDof_SetParams(tObj, 0U);
    if (FVID2_SOK != status)
    {
        DebugP_log(" DOF_TEST_APP: SetParams Failed \n");
    }

    status = AppDof_SetConfScoreParam(tObj, 0U);
    if (FVID2_SOK != status)
    {
        DebugP_log("DOF_TEST_APP: SetConfScoreParam Failed \n");
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
    


    for (rCnt = 0u; (rCnt < tObj->repeatCnt) && (FVID2_SOK == status); rCnt ++)
    {

        AppDof_PrepareRequest(tObj, 0U);

        for (pyrCnt = tObj->testCfg[0]->tPyrLvl; pyrCnt > 0; pyrCnt--)
        {
            
            /* Submit Request*/
            status = AppDof_SubmitRequest(tObj, 0U);

            if (FVID2_SOK == status)
            {

                /* Wait for Request Completion */
                status = AppDof_WaitForComplRequest(tObj, 0U);

                
                if (FVID2_SOK != status)
                {
                    DebugP_log (" Failed to wait for request completion \n");
                }
            }
            else
            {
                DebugP_log (" Failed to Submit Request \n");
            }
        }

        if (FVID2_SOK != status)
        {
            break;
        }
    }

    if (FVID2_SOK == status)
    {
        DebugP_log (" Completed RepeatCnt %d\n", rCnt);
    }

    DebugP_log("\n");
    AppDof_Delete(tObj, 0U);
}

static int32_t DofApp_init(void)
{
    int32_t                 status;
    uint32_t                instId;
    Udma_DrvHandle          drvHandle = &gDofAppUdmaDrvObj;
    Udma_InitPrms           udmaInitPrms;
    
    #if !defined(MCU_PLUS_SDK)
        Board_initCfg           boardCfg;
        boardCfg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO;
        Board_init(boardCfg);
    #endif
    status = Fvid2_init(NULL);
    if (FVID2_SOK != status)
    {
        DebugP_log(" main: FVID2_Init Failed !!!\r\n");
        
    }

    if (FVID2_SOK == status)
    {
        /* Initialize UDMA and get the handle, it will be used in both CRC layer,
           as well as in the driver */
        /* UDMA driver init */
        instId = UDMA_INST_ID_0;
        status = UdmaInitPrms_init(instId, &udmaInitPrms);
        if(UDMA_SOK != status)
        {
            DebugP_log("[Error] UDMA prms init failed!!\n");
            status = FVID2_EFAIL;
        }
        udmaInitPrms.instId = UDMA_INST_ID_0;
        udmaInitPrms.enableUtc = UTRUE;
        status = Udma_init(drvHandle, &udmaInitPrms);
        if(UDMA_SOK != status)
        {
            DebugP_log("[Error] UDMA init failed!!\n");
            status = FVID2_EFAIL;
        }        
    }

    status = AppDof_Init(drvHandle);
    if (FVID2_SOK == status)
    {
        /* Initialize the UDMA channel for CRC */
        //status = AppDof_CrcInit(drvHandle);
    }

    return (status);
}

static void DofApp_deInit(void)
{
    int32_t         status;
    Udma_DrvHandle  drvHandle = &gDofAppUdmaDrvObj;

    Vhwa_m2mDofDeInit();

    //AppDof_CrcDeinit(drvHandle);

    status = Udma_deinit(drvHandle);
    if(UDMA_SOK != status)
    {
        DebugP_log("[Error] UDMA deinit failed!!\n");
    }

    Fvid2_deInit(NULL);
}

static void App_udmaPrint(const char *str)
{
    ////App_print(str);
    return;
}
