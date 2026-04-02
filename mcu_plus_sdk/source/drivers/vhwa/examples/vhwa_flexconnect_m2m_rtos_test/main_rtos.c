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
 *  \file main_rtos.c
 *
 *  \brief Main file for RTOS builds
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdio.h>
#include "ti/osal/osal.h"
#include "ti/osal/TaskP.h"
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <examples/include/vhwa_examples_common.h>
#include <examples/include/vhwa_common_crc.h>
#include <include/vhwa_m2mFlexConnect.h>

#include <examples/include/vhwa_viss_test_api.h>
#include <examples/vhwa_viss_test/vhwa_viss_cfg.h>
#include <examples/include/vhwa_msc_api.h>
#include <examples/include/vhwa_ldc_api.h>
#include <examples/include/vhwa_flexconnect_api.h>
#include <examples/include/vhwa_flexconnect_m2m_api.h>

#include <examples/vhwa_flexconnect_test/vhwa_flexconnect_viss_cfg.h>
#include <examples/vhwa_flexconnect_test/vhwa_flexconnect_msc_cfg.h>
#include <examples/vhwa_flexconnect_test/vhwa_flexconnect_cfg.h>
#include <examples/vhwa_flexconnect_m2m_rtos_test/vhwa_flexconnect_m2m_cfg.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#define APP_FC_M2M_TSK_STACK_MAIN          (64U * 1024U)

/* Set the task priority higher than the default priority (1) */
#define APP_FC_M2M_MAIN_TASKS_PRIORITY     (7U)

/* Set the task priority higher than the default priority (1) */
#define APP_FC_M2M_TEST_TASKS_PRIORITY     (5U)

#define LDC_SRC_BUFF_SIZE  (0x08000000u)
#define LDC_DST_BUFF_SIZE  (0x08000000u)

#define MSC0_SRC_BUFF_SIZE  (0x08000000u)
#define MSC0_DST_BUFF_SIZE  (0x08000000u)

#define MSC1_SRC_BUFF_SIZE  (0x08000000u)
#define MSC1_DST_BUFF_SIZE  (0x08000000u)

#define VISS_SRC_BUFF_SIZE  (0x08000000u)
#define VISS_DST_BUFF_SIZE  (0x08000000u)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

static VhwaAl_SemaphoreP_Struct waitForTaskCmpl[APP_FC_M2M_MAX_HANDLES];

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void FcM2mApp_mainTaskFunc(void* a0, void* a1);
static void FcM2mApp_test(void* a0, void* a1);
static int32_t FcM2mApp_init(void);
static void FcM2mApp_run(uint32_t testCnt);
static void FcM2mApp_deInit(void);
static int32_t App_getNum(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
#if defined(SAFERTOS)
static uint8_t  gAppTskStackMain[APP_FC_TSK_STACK_MAIN]
    __attribute__((aligned(APP_FC_TSK_STACK_MAIN))) = { 0 };
static uint8_t  gAppTskStackTest[APP_FC_M2M_MAX_HANDLES][APP_FC_TSK_STACK_MAIN]
    __attribute__((aligned(APP_FC_TSK_STACK_MAIN))) = { 0 };
#else
static uint8_t  gAppTskStackMain[APP_FC_M2M_TSK_STACK_MAIN]
    __attribute__((aligned(32)));
static uint8_t  gAppTskStackTest[APP_FC_M2M_MAX_HANDLES][APP_FC_M2M_TSK_STACK_MAIN]
    __attribute__((aligned(32)));
#endif

/*
* Application Buffers
*/
static uint64_t gFcM2mTestSrcBuf =
    (uint64_t )(0x90000000U);
static uint64_t gFcM2mTestDstBuf =
    (uint64_t )(0xC0000000u);

/* VISS Buffers*/
uint64_t gVissTestSrcBuf;
uint64_t gVissTestDstBuf;
uint32_t gVissTestSrcBufFreeIdx;
uint32_t gVissTestDstBufFreeIdx;

/*LDC Buffers*/
uint64_t gLdcTestSrcBuf;
uint64_t gLdcTestDstBuf;
uint32_t gLdcTestSrcBufFreeIdx;
uint32_t gLdcTestDstBufFreeIdx;

/*MSC0 Buffers*/
uint64_t gMsc0TestSrcBuf;
uint64_t gMsc0TestDstBuf;
uint32_t gMsc0TestSrcBufFreeIdx;
uint32_t gMsc0TestDstBufFreeIdx;

/*MSC1 Buffers*/
uint64_t gMsc1TestSrcBuf;
uint64_t gMsc1TestDstBuf;
uint32_t gMsc1TestSrcBufFreeIdx;
uint32_t gMsc1TestDstBufFreeIdx;

/*LDC Buffers*/
uint64_t gLdcTestSrcBuf;
uint64_t gLdcTestDstBuf;
uint32_t gLdcTestSrcBufFreeIdx;
uint32_t gLdcTestDstBufFreeIdx;

/*FC Buffers*/
uint64_t gFcTestSrcBuf;
uint64_t gFcTestDstBuf;
uint32_t gFcTestSrcBufFreeIdx;
uint32_t gFcTestDstBufFreeIdx;

static VhwaAl_SemaphoreP_Struct gAllocLock = {NULL};
static VhwaAl_SemaphoreP_Struct gSemHandle[APP_FC_M2M_MAX_HANDLES] = {NULL};
StaticSemaphore_t gPingSemObj[APP_FC_M2M_MAX_HANDLES - 1U];
SemaphoreHandle_t gPingSem[APP_FC_M2M_MAX_HANDLES - 1U];

static struct Udma_DrvObj gFcM2mAppUdmaDrvObj;

AppFcM2m_TestMemoryObj  gFcM2mTestMemObj;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    TaskP_Handle task;
    TaskP_Params taskParams;

    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);

    taskParams.priority     = APP_FC_M2M_MAIN_TASKS_PRIORITY;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    task = TaskP_create(&FcM2mApp_mainTaskFunc, &taskParams);
    if(NULL == task)
    {
        OS_stop();
    }

    OS_start();    /* does not return */

    return(0);
}

static void FcM2mApp_mainTaskFunc(void* a0, void* a1)
{
    int32_t                 status;
    bool                    done = UFALSE;
    int32_t                 testIdx;

    status = FcM2mApp_init();

    Vhwa_print("\n Starting FC+M2M Test ..\n");

    App_startTimer();

    /*VISS Buffers*/
    gVissTestSrcBuf = (uint64_t )gFcM2mTestSrcBuf;
    gVissTestDstBuf = (uint64_t )gFcM2mTestDstBuf;

    /*MSC Buffers*/
    gMsc0TestSrcBuf = (uint64_t )gVissTestSrcBuf + VISS_SRC_BUFF_SIZE;
    gMsc0TestDstBuf = (uint64_t )gVissTestDstBuf + VISS_DST_BUFF_SIZE;

    gMsc1TestSrcBuf = (uint64_t )gMsc0TestSrcBuf + MSC0_SRC_BUFF_SIZE;
    gMsc1TestDstBuf = (uint64_t )gMsc0TestDstBuf + MSC0_DST_BUFF_SIZE;

    /*LDC Buffers*/
    gLdcTestSrcBuf = (uint64_t )gMsc1TestSrcBuf + MSC1_SRC_BUFF_SIZE;
    gLdcTestDstBuf = (uint64_t )gMsc1TestDstBuf + MSC1_SRC_BUFF_SIZE;

    /*FC Buffers*/
    gFcTestSrcBuf = (uint64_t )gLdcTestSrcBuf + LDC_SRC_BUFF_SIZE;
    gFcTestDstBuf = (uint64_t )gLdcTestDstBuf + LDC_DST_BUFF_SIZE;

    gFcM2mTestMemObj.vissSrcBuf = gVissTestSrcBuf;
    gFcM2mTestMemObj.vissSrcBufIdx = gVissTestSrcBufFreeIdx;
    gFcM2mTestMemObj.vissDstBuf = gVissTestDstBuf;
    gFcM2mTestMemObj.vissDstBufIdx = gVissTestDstBufFreeIdx;

    gFcM2mTestMemObj.msc0SrcBuf = gMsc0TestSrcBuf;
    gFcM2mTestMemObj.msc0SrcBufIdx = gMsc0TestSrcBufFreeIdx;
    gFcM2mTestMemObj.msc0DstBuf = gMsc0TestDstBuf;
    gFcM2mTestMemObj.msc0DstBufIdx = gMsc0TestDstBufFreeIdx;

    gFcM2mTestMemObj.msc1SrcBuf = gMsc1TestSrcBuf;
    gFcM2mTestMemObj.msc1SrcBufIdx = gMsc1TestSrcBufFreeIdx;
    gFcM2mTestMemObj.msc1DstBuf = gMsc1TestDstBuf;
    gFcM2mTestMemObj.msc1DstBufIdx = gMsc1TestDstBufFreeIdx;

    gFcM2mTestMemObj.ldcSrcBuf = gLdcTestSrcBuf;
    gFcM2mTestMemObj.ldcSrcBufIdx = gLdcTestSrcBufFreeIdx;
    gFcM2mTestMemObj.ldcDstBuf = gLdcTestDstBuf;
    gFcM2mTestMemObj.ldcDstBufIdx = gLdcTestDstBufFreeIdx;

    gFcM2mTestMemObj.fcSrcBuf = gFcTestSrcBuf;
    gFcM2mTestMemObj.fcSrcBufIdx = gFcTestSrcBufFreeIdx;
    gFcM2mTestMemObj.fcDstBuf = gFcTestDstBuf;
    gFcM2mTestMemObj.fcDstBufIdx = gFcTestDstBufFreeIdx;

    if (FVID2_SOK == status)
    {
        while(!done)
        {
            Vhwa_print("\n Enter Test to Run (Enter '-1' to exit)  \n");
            testIdx = App_getNum();

            switch(testIdx)
            {
                case -1:
                    done = UTRUE;
                    break;

                default:
                    if((uint32_t)testIdx < (sizeof(gAppFcM2mTestPrms)/ sizeof(AppFcM2m_TestPrms)))
                    {
                        FcM2mApp_run(testIdx);
                    }
                    else
                    {
                        Vhwa_print("Invalid Test ID. Enter Again!!\n");
                    }
                    break;
            }
        }
    }
    
    Vhwa_print (" Exiting \n");
    FcM2mApp_deInit();

    return;
}

void FcM2mApp_run(uint32_t testCnt)
{
    uint32_t            hndlCnt;
    TaskP_Handle        task[APP_FC_M2M_MAX_HANDLES];
    TaskP_Params        taskParams[APP_FC_M2M_MAX_HANDLES];
    AppFcM2m_TestPrms   *tPrms = NULL;
        
    tPrms = &gAppFcM2mTestPrms[testCnt];

    gVissTestSrcBufFreeIdx = 0u;
    gVissTestDstBufFreeIdx = 0u;
    gMsc0TestSrcBufFreeIdx = 0u;
    gMsc0TestDstBufFreeIdx = 0u;
    gMsc1TestSrcBufFreeIdx = 0u;
    gMsc1TestDstBufFreeIdx = 0u;
    gLdcTestSrcBufFreeIdx  = 0u;
    gLdcTestDstBufFreeIdx  = 0u;
    gFcTestSrcBufFreeIdx   = 0u;
    gFcTestDstBufFreeIdx   = 0u;

    /*VISS Buffers*/
    gVissTestSrcBuf = (uint64_t )gFcM2mTestSrcBuf;
    gVissTestDstBuf = (uint64_t )gFcM2mTestDstBuf;

    /*MSC Buffers*/
    gMsc0TestSrcBuf = (uint64_t )gVissTestSrcBuf + VISS_SRC_BUFF_SIZE;
    gMsc0TestDstBuf = (uint64_t )gVissTestDstBuf + VISS_DST_BUFF_SIZE;

    gMsc1TestSrcBuf = (uint64_t )gMsc0TestSrcBuf + MSC0_SRC_BUFF_SIZE;
    gMsc1TestDstBuf = (uint64_t )gMsc0TestDstBuf + MSC0_DST_BUFF_SIZE;

    /*LDC Buffers*/
    gLdcTestSrcBuf = (uint64_t )gMsc1TestSrcBuf + MSC1_SRC_BUFF_SIZE;
    gLdcTestDstBuf = (uint64_t )gMsc1TestDstBuf + MSC1_SRC_BUFF_SIZE;

    /*FC Buffers*/
    gFcTestSrcBuf = (uint64_t )gLdcTestSrcBuf + LDC_SRC_BUFF_SIZE;
    gFcTestDstBuf = (uint64_t )gLdcTestDstBuf + LDC_DST_BUFF_SIZE;

    gFcM2mTestMemObj.vissSrcBuf = gVissTestSrcBuf;
    gFcM2mTestMemObj.vissSrcBufIdx = gVissTestSrcBufFreeIdx;
    gFcM2mTestMemObj.vissDstBuf = gVissTestDstBuf;
    gFcM2mTestMemObj.vissDstBufIdx = gVissTestDstBufFreeIdx;

    gFcM2mTestMemObj.msc0SrcBuf = gMsc0TestSrcBuf;
    gFcM2mTestMemObj.msc0SrcBufIdx = gMsc0TestSrcBufFreeIdx;
    gFcM2mTestMemObj.msc0DstBuf = gMsc0TestDstBuf;
    gFcM2mTestMemObj.msc0DstBufIdx = gMsc0TestDstBufFreeIdx;

    gFcM2mTestMemObj.msc1SrcBuf = gMsc1TestSrcBuf;
    gFcM2mTestMemObj.msc1SrcBufIdx = gMsc1TestSrcBufFreeIdx;
    gFcM2mTestMemObj.msc1DstBuf = gMsc1TestDstBuf;
    gFcM2mTestMemObj.msc1DstBufIdx = gMsc1TestDstBufFreeIdx;

    gFcM2mTestMemObj.ldcSrcBuf = gLdcTestSrcBuf;
    gFcM2mTestMemObj.ldcSrcBufIdx = gLdcTestSrcBufFreeIdx;
    gFcM2mTestMemObj.ldcDstBuf = gLdcTestDstBuf;
    gFcM2mTestMemObj.ldcDstBufIdx = gLdcTestDstBufFreeIdx;

    gFcM2mTestMemObj.fcSrcBuf = gFcTestSrcBuf;
    gFcM2mTestMemObj.fcSrcBufIdx = gFcTestSrcBufFreeIdx;
    gFcM2mTestMemObj.fcDstBuf = gFcTestDstBuf;
    gFcM2mTestMemObj.fcDstBufIdx = gFcTestDstBufFreeIdx;

    if (UTRUE == tPrms->isEnableTest)
    {
        Vhwa_print ("\n Starting Test %s\n", tPrms->testName);

        for (hndlCnt = 0U; hndlCnt < tPrms->numHandles;
                hndlCnt ++)
        {
            /* Initialize the task params */
            TaskP_Params_init(&taskParams[hndlCnt]);

            /* Set the task priority higher than the
                default priority (1) */
            taskParams[hndlCnt].priority  = APP_FC_M2M_TEST_TASKS_PRIORITY;
            taskParams[hndlCnt].stack     = gAppTskStackTest[hndlCnt];
            taskParams[hndlCnt].stacksize = sizeof (gAppTskStackTest[hndlCnt]);
            taskParams[hndlCnt].arg0      = (void*)tPrms;
            taskParams[hndlCnt].arg1      = (void*)hndlCnt;

            /* Start a new Task */
            task[hndlCnt] = TaskP_create(&FcM2mApp_test,
                                            &taskParams[hndlCnt]);
            if(NULL == task[hndlCnt])
            {
                Vhwa_print (" Task%d create failed... \n", hndlCnt);
                OS_stop();
            }
        }

        /* Wait for all tesks to get completed */
        for (hndlCnt = 0U; hndlCnt < tPrms->numHandles; hndlCnt ++)
        {
            VhwaAl_SemaphoreP_pend(waitForTaskCmpl[hndlCnt],
                SYSTEMP_WAIT_FOREVER);

            Vhwa_print (" Task%d Completed \n", hndlCnt);
            /* Delete Task */
            if (NULL != task[hndlCnt])
            {
                TaskP_delete(&task[hndlCnt]);
            }
        }
    }
    else
    {
        Vhwa_print ("\n Test %d - %s is Disabled \n", testCnt, tPrms->testName);
    }

    return;
}

static void FcM2mApp_test(void* a0, void* a1)
{
    int32_t                 status = FVID2_SOK;
    uint32_t                hndlIdx = (uint32_t )a1;
    uint32_t                repCnt;
    AppFcM2m_TestPrms       *tPrms = (AppFcM2m_TestPrms *)a0;
    AppFcM2m_TestConfig     *tCfg = tPrms->testCfg;
    AppViss_TestParams *tVissPrms   = &tCfg[hndlIdx].vissAppTestPrms;
    App_MscTestParams  *tMsc0Prms   = &tCfg[hndlIdx].msc0AppTestPrms;
    App_MscTestParams  *tMsc1Prms   = &tCfg[hndlIdx].msc1AppTestPrms;
    LdcApp_TestParams  *tLdcPrms    = &tCfg[hndlIdx].ldcAppTestPrms;
    AppFc_TestParams  *tFcPrms      = &tCfg[hndlIdx].fcAppTestPrms;

    uint64_t                visstimeCount;
    uint64_t                msc0timeCount;
    uint64_t                msc1timeCount;
    uint64_t                ldctimeCount;

    if(tPrms->isM2mVissEnabled == UTRUE && tVissPrms->isEnableTest == UTRUE)
    {
        Vhwa_print (" VISS Test Enabled %s\n", tVissPrms->testName);
    }
    if(tPrms->isM2mMsc0Enabled == UTRUE && tMsc0Prms->isEnableTest == UTRUE)
    {
        Vhwa_print (" MSC0 Test Enabled %s\n", tMsc0Prms->testName);
    }
    if(tPrms->isM2mMsc1Enabled == UTRUE && tMsc1Prms->isEnableTest == UTRUE)
    {
        Vhwa_print (" MSC1 Test Enabled %s\n", tMsc1Prms->testName);
    }
    if(tPrms->isM2mLdcEnabled == UTRUE && tLdcPrms->isEnableTest == UTRUE)
    {
        Vhwa_print (" LDC Test Enabled %s\n", tLdcPrms->testName);
    }
    if(tPrms->isFcEnabled == UTRUE && tFcPrms->isEnableTest == UTRUE)
    {
        Vhwa_print (" FC Test Enabled %s\n", tFcPrms->testName);
    }

    status = AppFcM2m_Create(tPrms, hndlIdx);
    if (FVID2_SOK != status)
    {
        Vhwa_print(" FC_M2M_TEST_APP: Create Failed for %d\n", hndlIdx);
        status = FVID2_EFAIL;
    }

    status = AppFcM2m_SetCoeff(tPrms, hndlIdx);
    if(FVID2_SOK != status)
    {
        Vhwa_print(" FC_M2M_TEST_APP: SetCoeff Failed for handle # %d\n", hndlIdx);
        status = FVID2_EFAIL;
    }

    if (FVID2_SOK == status)
    {
        status = AppFcM2m_SetParams(tPrms, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print (" FC_M2M_TEST_APP: Failed to Set all Config\n");
            status = FVID2_EFAIL;
        }
    }

    if(tPrms->isM2mVissEnabled == UTRUE && tVissPrms->isEnableTest == UTRUE)
    {
        gFcM2mTestMemObj.vissSrcBuf = gVissTestSrcBuf + gFcM2mTestMemObj.vissSrcBufIdx;
        gFcM2mTestMemObj.vissDstBuf = gVissTestDstBuf + gFcM2mTestMemObj.vissDstBufIdx;
    }
    if(tPrms->isM2mMsc0Enabled == UTRUE && tMsc0Prms->isEnableTest == UTRUE)
    {
        gFcM2mTestMemObj.msc0SrcBuf = gMsc0TestSrcBuf + gFcM2mTestMemObj.msc0SrcBufIdx;
        gFcM2mTestMemObj.msc0DstBuf = gMsc0TestDstBuf + gFcM2mTestMemObj.msc0DstBufIdx;
    }
    if(tPrms->isM2mMsc1Enabled == UTRUE && tMsc1Prms->isEnableTest == UTRUE)
    {
        gFcM2mTestMemObj.msc1SrcBuf = gMsc1TestSrcBuf + gFcM2mTestMemObj.msc1SrcBufIdx;
        gFcM2mTestMemObj.msc1DstBuf = gMsc1TestDstBuf + gFcM2mTestMemObj.msc1DstBufIdx;
    }
    if(tPrms->isM2mLdcEnabled  == UTRUE && tLdcPrms->isEnableTest == UTRUE)
    {
        gFcM2mTestMemObj.ldcSrcBuf = gLdcTestSrcBuf + gFcM2mTestMemObj.ldcSrcBufIdx;
        gFcM2mTestMemObj.ldcDstBuf = gLdcTestDstBuf + gFcM2mTestMemObj.ldcDstBufIdx;
    }
    if(tPrms->isFcEnabled == UTRUE && tFcPrms->isEnableTest == UTRUE)
    {
        gFcM2mTestMemObj.fcSrcBuf = gFcTestSrcBuf + gFcM2mTestMemObj.fcSrcBufIdx;
        gFcM2mTestMemObj.fcDstBuf = gFcTestDstBuf + gFcM2mTestMemObj.fcDstBufIdx;
    }

    if (FVID2_SOK == status)
    {
        VhwaAl_SemaphoreP_pend(gAllocLock, SYSTEMP_WAIT_FOREVER);

        status = AppFcM2m_AllocBuffers(tPrms, hndlIdx, &gFcM2mTestMemObj);
        if(FVID2_SOK != status)
        {
            Vhwa_print(" FC_M2M_TEST_APP: AppfcM2m_AllocBuffers returned %x\n", status);
        }

        VhwaAl_SemaphoreP_post(gAllocLock);

        if (FVID2_SOK != status)
        {
            Vhwa_print (" FC_M2M_TEST_APP: Failed to Set all Config\n");
        }
    }

    if(tPrms->isM2mVissEnabled == UTRUE && tVissPrms->isEnableTest == UTRUE)
    {
        AppViss_PrepareRequest(tVissPrms, hndlIdx);

        if(tVissPrms->isPerformanceTest)
        {
            visstimeCount = TimerP_getTimeInUsecs();
        }
    }

    if(tPrms->isM2mMsc0Enabled == UTRUE && tMsc0Prms->isEnableTest == UTRUE)
    {
        AppMsc_PrepareRequest(tMsc0Prms, hndlIdx);

        if(tMsc0Prms->isPerformanceTest)
        {
            msc0timeCount = TimerP_getTimeInUsecs();
        }        
    }

    if(tPrms->isM2mMsc1Enabled == UTRUE && tMsc1Prms->isEnableTest == UTRUE)
    {
        AppMsc_PrepareRequest(tMsc1Prms, hndlIdx);

        if(tMsc1Prms->isPerformanceTest)
        {
            msc1timeCount = TimerP_getTimeInUsecs();
        }
    }

    if(tPrms->isM2mLdcEnabled == UTRUE && tLdcPrms->isEnableTest == UTRUE)
    {
        AppLdc_PrepareRequest(tLdcPrms, hndlIdx);

        if(tLdcPrms->isPerformanceTest)
        {
            ldctimeCount = TimerP_getTimeInUsecs();
        }
    }

    if(tPrms->isFcEnabled == UTRUE && tFcPrms->isEnableTest == UTRUE)
    {
        AppFc_PrepareRequest(tFcPrms, hndlIdx);
    }

    Vhwa_print(" Load buffers and press any key to continue\n");
    UART_getc();

    if(tPrms->numHandles > 1U)
    {
        if(hndlIdx < (tPrms->numHandles - 1U))
        {
            xSemaphoreTake(gPingSem[hndlIdx], portMAX_DELAY); /* wait for last task to signal */
        }
        if(hndlIdx == (tPrms->numHandles - 1U))
        {
            uint32_t hndlIdxCnt = hndlIdx;
            do
            {
                hndlIdxCnt-- ;
                xSemaphoreGive(gPingSem[hndlIdxCnt]);         /* wake up previous tasks */
            }while(hndlIdxCnt > 0U);
        }
    }

    if (FVID2_SOK == status)
    {
        for (repCnt = 0U; repCnt < tPrms->repeatCnt; repCnt ++)
        {            
            if(tPrms->numHandles > 1U)
            {
                VhwaAl_SemaphoreP_pend(gSemHandle[hndlIdx], SYSTEMP_WAIT_FOREVER);
            }

            if(tPrms->isM2mVissEnabled == UTRUE && tVissPrms->isEnableTest == UTRUE)
            {
                /* Submit VISS Request*/
                status = AppViss_SubmitRequest(tVissPrms, hndlIdx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print
                        (" FC_M2M_TEST_APP: Failed to SubmitRequest for VISS\n");
                    break;
                }
                else
                {
                    status = AppViss_WaitForCompRequest(tVissPrms, hndlIdx, 0U);
                    if (FVID2_SOK != status)
                    {
                        Vhwa_print
                            (" FC_M2M_TEST_APP: Failed to GetRequest for VISS\n");
                        break;
                    }
                }
            }

            if(tPrms->isM2mMsc0Enabled == UTRUE && tMsc0Prms->isEnableTest == UTRUE)
            {
                /* Submit MSC0 Request*/
                status = AppMsc_SubmitRequest(tMsc0Prms, hndlIdx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print
                        (" FC_M2M_TEST_APP: Failed to SubmitRequest for MSC0\n");
                    break;
                }
                else
                {
                    status = AppMsc_WaitForComplRequest(tMsc0Prms, hndlIdx);
                    if (FVID2_SOK != status)
                    {
                        Vhwa_print
                            (" FC_M2M_TEST_APP: Failed to GetRequest for MSC0\n");
                        break;
                    } 
                }
            }

            if(tPrms->isM2mMsc1Enabled == UTRUE && tMsc1Prms->isEnableTest == UTRUE)
            {
                /* Submit MSC1 Request*/
                status = AppMsc_SubmitRequest(tMsc1Prms, hndlIdx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print
                        (" FC_M2M_TEST_APP: Failed to SubmitRequest for MSC1\n");
                    break;
                }
                else
                {
                    status = AppMsc_WaitForComplRequest(tMsc1Prms, hndlIdx);
                    if (FVID2_SOK != status)
                    {
                        Vhwa_print
                            (" FC_M2M_TEST_APP: Failed to GetRequest for MSC1\n");
                        break;
                    }
                }
            }

            if(tPrms->isM2mLdcEnabled == UTRUE && tLdcPrms->isEnableTest == UTRUE)
            {
                status = AppLdc_SubmitRequest(tLdcPrms, hndlIdx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print
                        (" FC_M2M_TEST_APP: Failed to SubmitRequest for LDC\n");
                    break;
                }
                else
                {
                    status = AppLdc_WaitForComplRequest(tLdcPrms, hndlIdx);
                    if (FVID2_SOK != status)
                    {
                        Vhwa_print
                            (" FC_M2M_TEST_APP: Failed to GetRequest for LDC\n");
                        break;
                    }
                }
            }

            if(tPrms->isFcEnabled == UTRUE && tFcPrms->isEnableTest == UTRUE)
            {
                status = AppFc_SubmitRequest(tFcPrms, hndlIdx);
                if (FVID2_SOK != status)
                {
                    Vhwa_print
                        (" FC_M2M_TEST_APP: Failed to SubmitRequest for FC\n");
                    break;
                }
                else
                {
                    status = AppFc_WaitForCompRequest(tFcPrms, hndlIdx);
                    if (FVID2_SOK != status)
                    {
                        Vhwa_print
                            (" FC_M2M_TEST_APP: Failed to GetRequest for LDC\n");
                        break;
                    }
                }
            }

            if(tPrms->numHandles > 1U)
            {
                VhwaAl_SemaphoreP_post(gSemHandle[(tPrms->numHandles - 1U) - hndlIdx]);
            }
        }
        if (FVID2_SOK == status)
        {
            if(tPrms->isM2mVissEnabled)
            {
                if(tVissPrms->isPerformanceTest)
                {
                    visstimeCount = TimerP_getTimeInUsecs() - visstimeCount;
                    Vhwa_print ("VISS Performance:\n\t FrameCount: %d: Time in uSec: %d\n",
                                tVissPrms->repeatCnt, visstimeCount);

                    uint64_t perf = (uint64_t)tVissPrms->testCfg[hndlIdx]->vissPrms.inFmt.width
                        *(uint64_t)tVissPrms->testCfg[hndlIdx]->vissPrms.inFmt.height
                        *(uint64_t)tVissPrms->repeatCnt;

                    Vhwa_print ("\t VISS MPix/s: %d.%d\n",
                        (uint32_t)(perf/visstimeCount),
                        (uint32_t)(((perf*(uint64_t)100)/visstimeCount)%100));
                }         
            }
            if(tPrms->isM2mMsc0Enabled)
            {
                if(tMsc0Prms->isPerformanceTest)
                {
                    msc0timeCount = TimerP_getTimeInUsecs() - msc0timeCount;
                    Vhwa_print ("Performance:\n\t FrameCount: %d: Time in uSec: %d\n",
                                tMsc0Prms->repeatCnt, msc0timeCount);

                    uint64_t perf = (uint64_t)tMsc0Prms->testCfg[hndlIdx]->inFrm.inWidth
                        *(uint64_t)tMsc0Prms->testCfg[hndlIdx]->inFrm.inHeight
                        *(uint64_t)tMsc0Prms->repeatCnt;
                    if(FVID2_DF_YUV420SP_UV == tMsc0Prms->testCfg[hndlIdx]->inFrm.inDataFmt)
                    {
                        perf = perf*3U/2U;
                    }
                    Vhwa_print ("\tMSC0 MPix/s: %d.%d\n",
                        (uint32_t)(perf/msc0timeCount),
                        (uint32_t)(((perf*(uint64_t)100)/msc0timeCount)%100));
                }     
            }
            if(tPrms->isM2mMsc1Enabled)
            {
                if(tMsc1Prms->isPerformanceTest)
                {
                    msc1timeCount = TimerP_getTimeInUsecs() - msc1timeCount;
                    Vhwa_print ("Performance:\n\t FrameCount: %d: Time in uSec: %d\n",
                                tMsc1Prms->repeatCnt, msc1timeCount);

                    uint64_t perf = (uint64_t)tMsc1Prms->testCfg[hndlIdx]->inFrm.inWidth
                        *(uint64_t)tMsc1Prms->testCfg[hndlIdx]->inFrm.inHeight
                        *(uint64_t)tMsc1Prms->repeatCnt;
                    if(FVID2_DF_YUV420SP_UV == tMsc1Prms->testCfg[hndlIdx]->inFrm.inDataFmt)
                    {
                        perf = perf*3U/2U;
                    }
                    Vhwa_print ("\tMSC1 MPix/s: %d.%d\n",
                        (uint32_t)(perf/msc1timeCount),
                        (uint32_t)(((perf*(uint64_t)100)/msc1timeCount)%100));
                }               
            }
            if(tPrms->isM2mLdcEnabled)
            {
                if(tLdcPrms->isPerformanceTest)
                {
                    ldctimeCount = TimerP_getTimeInUsecs() - ldctimeCount;
                    Vhwa_print ("Performance:\n\t FrameCount: %d: Time in uSec: %d\n",
                                tLdcPrms->repeatCnt, ldctimeCount);

                    uint64_t perf = (uint64_t)tLdcPrms->testCfg[hndlIdx]->inWidth
                        *(uint64_t)tLdcPrms->testCfg[hndlIdx]->inHeight
                        *(uint64_t)tLdcPrms->repeatCnt;
                    Vhwa_print ("\tLDC MPix/s: %d.%d\n",
                        (uint32_t)(perf/ldctimeCount),
                        (uint32_t)(((perf*(uint64_t)100)/ldctimeCount)%100));
                }                   
            }

        }
    }

    AppFcM2m_Delete(tPrms, hndlIdx);

    VhwaAl_SemaphoreP_post(waitForTaskCmpl[hndlIdx]);
}

static int32_t FcM2mApp_init(void)
{
    int32_t             status;
    uint32_t            instId, hndlCnt;
    Udma_InitPrms       udmaInitPrms;
    Board_initCfg       boardCfg;
    Udma_DrvHandle      drvHandle = &gFcM2mAppUdmaDrvObj;
    SemaphoreP_Params   params[APP_FC_M2M_MAX_HANDLES];
    SemaphoreP_Params   semAllocPrms;
    Fvid2_InitPrms      initPrmsFvid2;

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

    status = AppFcM2m_Init(drvHandle);
    if (FVID2_SOK == status)
    {
        /* Initialize the UDMA channel for CRC */
        status = AppFcM2m_CrcInit(drvHandle);
    }

    if (FVID2_SOK == status)
    {
        for (hndlCnt = 0U; hndlCnt < APP_FC_M2M_MAX_HANDLES; hndlCnt ++)
        {
            SemaphoreP_Params_init(&params[hndlCnt]);
            params[hndlCnt].mode = SemaphoreP_Mode_BINARY;
            waitForTaskCmpl[hndlCnt] =
                SemaphoreP_create(0U, &params[hndlCnt]);
            if (NULL == waitForTaskCmpl[hndlCnt])
            {
                Vhwa_print (" Could not create Semaphore \n");
                status = FVID2_EFAIL;
            }
        }

        SemaphoreP_Params_init(&semAllocPrms);
        semAllocPrms.mode = SemaphoreP_Mode_BINARY;

        gAllocLock = SemaphoreP_create(1U, &semAllocPrms);
        if (NULL == gAllocLock)
        {
            Vhwa_print("[Error] Sem create failed!!\n");
            status = FVID2_EFAIL;
        }

        for (hndlCnt = 0U; hndlCnt < APP_FC_M2M_MAX_HANDLES; hndlCnt ++)
        {
            gSemHandle[hndlCnt] = SemaphoreP_create(1U, &semAllocPrms);
            if (NULL == gSemHandle[hndlCnt])
            {
                Vhwa_print("[Error] SemHandle %d create failed!!\n", hndlCnt);
                status = FVID2_EFAIL;
            }
        }
        for (hndlCnt = 0U; hndlCnt < (APP_FC_M2M_MAX_HANDLES - 1U); hndlCnt ++)
        {        
            gPingSem[hndlCnt] = xSemaphoreCreateBinaryStatic(&gPingSemObj[hndlCnt]);
            if (NULL == gPingSem[hndlCnt])
            {
                Vhwa_print("[Error] gPingSem %d create failed!!\n", hndlCnt);
                status = FVID2_EFAIL;
            }
        }
    }
    return (status);
}

static void FcM2mApp_deInit()
{
    int32_t         status;
    uint32_t        cnt;
    Udma_DrvHandle  drvHandle = &gFcM2mAppUdmaDrvObj;

    Vhwa_m2mVissDeInit();
    Vhwa_m2mMscDeInit();
    Vhwa_m2mLdcDeInit();
    Vhwa_m2mFcDeInit();

    AppFcM2m_CrcDeinit(drvHandle);

    status = Udma_deinit(drvHandle);
    if(UDMA_SOK != status)
    {
        Vhwa_print("[Error] UDMA deinit failed!!\n");
    }

    for (cnt = 0U; cnt < APP_FC_M2M_MAX_HANDLES; cnt ++)
    {
        if (NULL != waitForTaskCmpl[cnt])
        {
            SemaphoreP_delete(waitForTaskCmpl[cnt]);
        }
    }

    if (gAllocLock != NULL)
    {
        SemaphoreP_delete(gAllocLock);
        gAllocLock = NULL;
    }

    for (cnt = 0U; cnt < APP_FC_M2M_MAX_HANDLES; cnt ++)
    {
        if(NULL != gSemHandle[cnt])
        {
            SemaphoreP_delete(gSemHandle[cnt]);
            gSemHandle[cnt] = NULL;
        }
    }

    Fvid2_deInit(NULL);
}

int32_t App_getNum(void)
{
    int32_t num;

    UART_scanFmt("%d", &num);

    return (num);
}


