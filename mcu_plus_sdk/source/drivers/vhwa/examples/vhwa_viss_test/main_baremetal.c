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
#include <stdlib.h>
#include <examples/include/vhwa_examples_common.h>
#include <include/vhwa_m2mViss.h>
#include <examples/include/vhwa_viss_test_api.h>
#include "vhwa_viss_cfg.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Input Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be loaded in one shot */
#define VHWA_VISS_TEST_IN_BUF_OFFSET     (8*1024*1024)

/* Output Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be saved in one shot */
#define VHWA_VISS_TEST_OUT_BUF_OFFSET    (16*1024*1024)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */



/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
static int32_t AppViss_test(AppViss_TestParams *tPrms);
static int32_t VissApp_init();
#if defined VHWA_PDK
static int32_t App_getNum(void);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*
* Application Buffers
*/
#if defined VHWA_VPAC_IP_REV_VPAC || defined VHWA_VPAC_IP_REV_VPAC3
static uint64_t gVissTestSrcBuf =
    (uint64_t )(VHWA_EXAMPLE_BUFF_START_ADDR);
static uint64_t gVissTestDestBuf =
    (uint64_t )(VHWA_EXAMPLE_BUFF_START_ADDR + 0x10000000u);
static uint32_t *gVissTestConfigBuf =
        (uint32_t*) (VHWA_EXAMPLE_CONFIG_BUFF_START_ADDR);
static uint32_t gVissTestConfigBufFreeIdx = 0u;

#elif defined VHWA_VPAC_IP_REV_VPAC3L
static uint64_t gVissTestSrcBuf = (uint64_t )(0xB0000000u);
static uint64_t gVissTestDestBuf = (uint64_t )(0xB8000000u);
#endif
static uint32_t gVissTestSrcBufFreeIdx = 0u;
static uint32_t gVissTestDstBufFreeIdx = 0u;
static VhwaAl_Udma_DrvObj_Struct gVissAppUdmaDrvObj;
#if 0
static AppViss_TestConfig gAppVissTestCfg[] =
{
     #include <examples/include/vhwa_viss_test_cfg.h>
};

static VissApp_TestParams gAppVissObj[] = VHWA_VISS_TIRTOS_CFG;
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    int32_t                 status;
    Udma_DrvHandle          drvHandle = &gVissAppUdmaDrvObj;
    AppViss_TestParams     *tPrms;

    #if defined VHWA_MCU_PLUS_SDK
    System_init();
    Board_init();
    Drivers_open();
    Board_driversOpen();
    #endif
    status = VissApp_init();

    #if defined VHWA_PDK
    App_startTimer();
    #endif
    int32_t testCaseID = -1;
    if (FVID2_SOK == status)
    {
        while(1)
        {
            Vhwa_print (" Enter test ID to run [-1 for all tests, 0 >=  Specific test case]: ");
            #if defined VHWA_PDK
            testCaseID = App_getNum();
            #else
            scanf("%d", &testCaseID);
            #endif
            
            tPrms = &gAppVissTestPrms[testCaseID];

            gVissTestSrcBufFreeIdx = 0u;
            gVissTestDstBufFreeIdx = 0u;

            if (UTRUE == tPrms->isEnableTest)
            {
                Vhwa_print (" Starting Test %s\n", tPrms->testName);
                status = AppViss_test(tPrms);

                if (FVID2_SOK != status)
                {
                    Vhwa_print ("Error Running TestCase: %s\n", tPrms->testName);
                    break;
                }
            }
            else
            {
                Vhwa_print ("\n Test %d - %s is Disabled \n", testCaseID, tPrms->testName);
            }

        }
    }

    #if defined VHWA_PDK
    AppViss_CrcDeinit(drvHandle);
    #endif

    AppViss_deInit(drvHandle);

    Vhwa_print (" Exiting \n");
    #if defined VHWA_MCU_PLUS_SDK
    Board_driversClose();
    Drivers_close();
    Board_deinit();
    System_deinit();
    #endif

    return(status);
}

static int32_t VissApp_init()
{
    int32_t        status;
    uint32_t       instId;
    Udma_InitPrms  udmaInitPrms;
    Udma_DrvHandle drvHandle = &gVissAppUdmaDrvObj;
    #if defined VHWA_PDK
    Board_initCfg  boardCfg;
    Fvid2_InitPrms initPrmsFvid2;

    boardCfg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    Fvid2InitPrms_init(&initPrmsFvid2);
    initPrmsFvid2.printFxn = Vhwa_print;
    status = Fvid2_init(&initPrmsFvid2);
    #else
    Sciclient_pmSetModuleState(TISCI_DEV_VPAC0,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SystemP_WAIT_FOREVER);

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
            Vhwa_print("[Error] UDMA prms init failed!!\n");
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

    status = AppViss_Init(drvHandle);
    #if defined VHWA_PDK
    if (FVID2_SOK == status)
    {
        /* Initialize the UDMA channel for CRC */
        status = AppViss_CrcInit(drvHandle);
    }
    #endif

    return (status);
}

static int32_t AppViss_test(AppViss_TestParams *tPrms)
{
    int32_t     status;
    uint32_t    repCnt;
    uint32_t    hCnt;
    uint64_t    timeCount;
    uint64_t    perf;
    #if defined VHWA_PDK
    Vhwa_M2mVissConfigAppBuff appBuffConfig;
    #endif

    for(hCnt = 0U; hCnt < tPrms->numHandles; hCnt++)
    {
        status = AppViss_Create(tPrms, hCnt);
        if (FVID2_SOK != status)
        {
            Vhwa_print (" Failed to create\n");
        }
        #if defined VHWA_PDK
        status = AppViss_GetConfigBufInfo(0U, &appBuffConfig);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" VISS_TEST_APP: Failed to Get config buf info\n");
        }
        else
        {
            /* configThroughUdmaFlag should not be changed by application */
            if (true == appBuffConfig.configThroughUdmaFlag)
            {
                /* update config buffer pointer */
                appBuffConfig.bufferPtr = gVissTestConfigBuf
                        + gVissTestConfigBufFreeIdx;

                status = AppViss_SetConfigBufInfo(0U, &appBuffConfig);
                if (FVID2_SOK != status)
                {
                    Vhwa_print(" VISS_TEST_APP: Failed to Set config buf info\n");
                }
            }
        }
        #endif
        if ((uint32_t)TRUE != tPrms->chCfgOnEachIter)
        {
            status = AppViss_SetAllConfig(tPrms, hCnt);
            if (FVID2_SOK != status)
            {
                Vhwa_print (" Failed to Set all Config\n");
            }
        }

        status = AppViss_AllocBuffers(tPrms, hCnt,
            gVissTestSrcBuf, &gVissTestSrcBufFreeIdx,
            gVissTestDestBuf, &gVissTestDstBufFreeIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print (" Failed to Set all Config\n");
        }
    }

    if(UTRUE == tPrms->isPerformanceTest)
    {
        timeCount = Vhwa_getTimeUsec();
    }

    for (repCnt = 0U; repCnt < tPrms->repeatCnt; repCnt ++)
    {
        for(hCnt = 0U; hCnt < tPrms->numHandles; hCnt++)
        {
            if ((uint32_t)TRUE == tPrms->chCfgOnEachIter)
            {
                status = AppViss_SetAllConfig(tPrms, hCnt);
                if (FVID2_SOK != status)
                {
                    Vhwa_print(" SetAllConfig Failed \n");
                    status = FVID2_EFAIL;
                }
            }
            AppViss_PrepareRequest(tPrms, hCnt);

            status = AppViss_SubmitRequest(tPrms, 0U);
            if (FVID2_SOK != status)
            {
                Vhwa_print (" Failed to Submit Request\n");
            }

            status = AppViss_WaitForCompRequest(tPrms, 0U, repCnt);
            if (FVID2_SOK != status)
            {
                Vhwa_print (" Failed to Get back Request\n");
            }
            else
            {
                if(UFALSE == tPrms->isPerformanceTest)
                {
                    Vhwa_print (" Completed RepeatCnt = %d\n", repCnt);
                }
            }
        }
    }
    if(UTRUE == tPrms->isPerformanceTest)
    {
        timeCount = Vhwa_getTimeUsec() - timeCount;
        Vhwa_print ("Performance:\n\t FrameCount: %d: Time in uSec: %d\n",
                    tPrms->repeatCnt, timeCount);

        perf = (uint64_t)tPrms->testCfg[0]->vissPrms.inFmt.width
                *(uint64_t)tPrms->testCfg[0]->vissPrms.inFmt.height
                *(uint64_t)tPrms->repeatCnt;

        Vhwa_print ("\t MPix/s: %d.%d\n",
            (uint32_t)(perf/timeCount),
                (uint32_t)(((perf*(uint64_t)100)/timeCount)%100));
    }

    AppViss_Delete(tPrms, 0U);

    return (status);
}

#if defined (VHWA_PDK)
int32_t App_getNum(void)
{
    int32_t num;

    UART_scanFmt("%d", &num);

    return (num);
}
#endif
