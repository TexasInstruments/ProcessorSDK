/**
 *   Copyright (c) Texas Instruments Incorporated 2018-2021
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
 *  \file main_rtos.c
 *
 *  \brief Main file for RTOS builds
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdio.h>

#include <include/vhwa_m2mMsc.h>
#include <examples/include/vhwa_msc_api.h>
#if defined (LDRA_COVERAGE)
#include <examples/include/ldra_vhwa_coverage_main.h>
#endif 

#if defined(VHWA_PDK)
#include "ti/osal/osal.h"
#include "ti/osal/TaskP.h"
#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#elif defined(VHWA_MCU_PLUS_SDK)
#include <drivers/device_manager/sciclient.h>
#include <kernel/dpl/DebugP.h>
#include "ti_drivers_config.h"
#include "ti_board_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include <drivers/uart.h>
#include <kernel/dpl/TimerP.h>
#endif


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Set test application stack size and task priority 
 * higher than the default priority (1) */
#if defined(VHWA_MCU_PLUS_SDK)
#define MAIN_TASK_SIZE (16384U / sizeof(configSTACK_DEPTH_TYPE))
#define MSC_APP_MAIN_TASKS_PRIORITY (configMAX_PRIORITIES - 1)
#define MSC_APP_TEST_TASKS_PRIORITY (configMAX_PRIORITIES - 2)
#elif defined(VHWA_PDK)
#define APP_TSK_STACK_MAIN (64U * 1024U)
#define MSC_APP_MAIN_TASKS_PRIORITY (7U)
#define MSC_APP_TEST_TASKS_PRIORITY (5U)
#endif

/* Input Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be loaded in one shot */
#define VHWA_MSC_TEST_IN_BUF_OFFSET (8 * 1024 * 1024)

/* Output Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be saved in one shot */
#define VHWA_MSC_TEST_OUT_BUF_OFFSET (16 * 1024 * 1024)

/* Offset for Each Input buffer in DDR */
#define APP_MSC_IN_BUFF_OFFSET (0x800000u)
/* Offset for Each Output buffer in DDR */
#define APP_MSC_OUT_BUFF_OFFSET (0x800000u)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
#if defined(VHWA_MCU_PLUS_SDK)
/* Define task parameters storage once */
typedef struct
{
    App_MscTestParams *testObj;
    uint32_t handleIdx;
} taskParams;
#endif

static VhwaAl_SemaphoreP_Struct waitForTaskCmpl[APP_MSC_MAX_HANDLES];

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void App_MscmainTaskFunc(void *a0, void *a1);
static void App_MscTest(void *a0, void *a1);
static int32_t App_MscInit(void);
static void App_MscRun(uint32_t testCnt);
static void App_MscDeInit(void);
#if defined(VHWA_PDK)
static int32_t App_getNum(void);
#endif
#if defined(VHWA_MCU_PLUS_SDK)
static void App_MscmainTaskFuncWrapper(void *args);
static void App_MscTestTaskWrapper(void *args);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
#if defined(VHWA_MCU_PLUS_SDK)
static StackType_t gMainTaskStack[MAIN_TASK_SIZE]
    __attribute__((aligned(32)));
static StaticTask_t gMainTaskObj;
static StackType_t taskStacks[APP_MSC_MAX_HANDLES][MAIN_TASK_SIZE]
    __attribute__((aligned(32)));
static StaticTask_t taskBuffers[APP_MSC_MAX_HANDLES];
#elif defined(SAFERTOS)
static uint8_t gAppTskStackMain[APP_TSK_STACK_MAIN]
    __attribute__((aligned(APP_TSK_STACK_MAIN)));
static uint8_t gAppTskStackTest[APP_MSC_MAX_HANDLES]
                               [APP_TSK_STACK_MAIN]
    __attribute__((aligned(APP_TSK_STACK_MAIN)));
#else
static uint8_t gAppTskStackMain[APP_TSK_STACK_MAIN]
    __attribute__((aligned(32)));
static uint8_t gAppTskStackTest[APP_MSC_MAX_HANDLES]
                               [APP_TSK_STACK_MAIN]
    __attribute__((aligned(32)));
#endif

/*
* Application Buffers
*/
#if defined(VHWA_MCU_PLUS_SDK)
static uint64_t gMscTestSrcBuf = (uint64_t)(0xB0000000u);
static uint64_t gMscTestDestBuf = (uint64_t)(0xB2000000u);
#else
static uint64_t gMscTestSrcBuf =
    (uint64_t)(VHWA_EXAMPLE_BUFF_START_ADDR);
static uint64_t gMscTestDestBuf =
    (uint64_t)(VHWA_EXAMPLE_BUFF_START_ADDR + 0x10000000u);
#endif

static volatile uint32_t gMscTestSrcBufFreeIdx = 0u;
static volatile uint32_t gMscTestDstBufFreeIdx = 0u;

#if defined(VHWA_MCU_PLUS_SDK)
static VhwaAl_SemaphoreP_Struct gAllocLock; /* Will be constructed in App_MscInit */
#else
static VhwaAl_SemaphoreP_Struct gAllocLock = NULL; /* Will be created in App_MscInit */
#endif

int32_t gSpCoeffSets[][MSC_MAX_SP_COEFF_SET][MSC_MAX_TAP] = {
    {{16, 48, 128, 48, 16},
     {32, 32, 128, 32, 32}},
};

int32_t gMpCoeffSets[][MSC_MAX_MP_COEFF_SET][MSC_MAX_TAP * 32U] = {
#include "coeff.txt"
};

Msc_Coeff gCoefTbl[] = {
    {{gSpCoeffSets[0][0], gSpCoeffSets[0][1]},
     {gMpCoeffSets[0][0], gMpCoeffSets[0][1],
      gMpCoeffSets[0][2], gMpCoeffSets[0][3]}}};

static App_MscTestCfg gAppMscTestCfg[] =
    {
#include <examples/include/vhwa_msc_test_cfg.h>
};

static App_MscTestParams gAppMscObj[] =
    {
#if defined VHWA_VPAC_IP_REV_VPAC || defined VHWA_VPAC_IP_REV_VPAC3
#include <ti/drv/vhwa/examples/vhwa_msc_test/vhwa_msc_cfg.h>
#elif defined VHWA_VPAC_IP_REV_VPAC3L
#include <examples/vhwa_msc_test/vhwa_msc_cfg.h>
#endif
};

static VhwaAl_Udma_DrvObj_Struct gMscAppUdmaDrvObj;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
#if defined(VHWA_MCU_PLUS_SDK)
    /* Initialize MCU+ SDK system components */
    System_init();
    Board_init();

#if defined(SOC_AM62AX)
    Sciclient_pmSetModuleState(TISCI_DEV_VPAC0,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                               TISCI_MSG_FLAG_AOP,
                               0xFFFFFFFFU);
#endif

    Drivers_open();
    Board_driversOpen();

#if defined(SOC_AM62AX)
    volatile int loop = 1;
    while (loop);
#endif

    TaskHandle_t gMainTask;

    /* Create main task */
    gMainTask = xTaskCreateStatic(
        App_MscmainTaskFuncWrapper,  /* Function that implements the task */
        "msc_main_task",             /* Text name for debugging */
        MAIN_TASK_SIZE,              /* Stack size in words */
        NULL,                        /* No parameters needed */
        MSC_APP_MAIN_TASKS_PRIORITY, /* High priority */
        gMainTaskStack,              /* Stack buffer */
        &gMainTaskObj                /* Static task buffer */
    );
    configASSERT(gMainTask != NULL);

    /* Start the scheduler */
    vTaskStartScheduler();

    /* Should never reach here */
    DebugP_assertNoLog(0);
#else
    /* Original code for PDK platforms */
    TaskP_Handle task;
    TaskP_Params taskParams;

    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);

    taskParams.priority = MSC_APP_MAIN_TASKS_PRIORITY;
    taskParams.stack = gAppTskStackMain;
    taskParams.stacksize = sizeof(gAppTskStackMain);

    task = TaskP_create(&App_MscmainTaskFunc, &taskParams);
    if (NULL == task)
    {
        OS_stop();
    }

    OS_start(); /* does not return */
#endif

    return (0);
}

static void App_MscmainTaskFunc(void *a0, void *a1)
{
    int32_t status;
    bool done = UFALSE;
    int32_t testIdx;
    #if defined (LDRA_COVERAGE)
    /*Expand gApprovedTestCases[] to include future testcases*/
    static const int32_t gApprovedPositiveTestCases [] = {0,13,10,11,14,15,16,17,62,19,1,4,20,21,22,23,32,33,31,30,29,28,27,24,43,42,41,34,35,36,37,38,39,40,26,25,48,49,50,51,52,53,58,59,44,45,46,47,54,55,56,57,60,61};
    static const int32_t gApprovedNegativeTestCases [] = {65,66,67,74,75,76,77,-1};
    uint32_t i = 0,j =0, PositiveTestcasesMax = 0; 
    PositiveTestcasesMax = sizeof(gApprovedPositiveTestCases)/sizeof(gApprovedPositiveTestCases[0]);
    #endif

    Vhwa_print("\n Starting MSC Test ..\n");
    #if defined (LDRA_COVERAGE)
    Vhwa_print("################### DYNAMIC CODE COVERAGE INITIALIZATION STARTED ######################\n");
    ldra_initialize();
    Vhwa_print("################### DYNAMIC CODE COVERAGE INITIALIZATION FINISHED ######################\n");
    #endif 

    status = App_MscInit();

#if !defined(VHWA_MCU_PLUS_SDK)
    App_startTimer();
#endif

    if (FVID2_SOK == status)
    {
        while (!done)
        {
#if defined (LDRA_COVERAGE)
/* In LDRA mode, fetch testIdx from predefined list */
if(i < PositiveTestcasesMax)
{
    testIdx = gApprovedPositiveTestCases[i];
    i++;
}
else
{
    testIdx = gApprovedNegativeTestCases[j];
    j++;
}
#else
            Vhwa_print("\n Enter Test to Run (Enter '-1' to exit):  \n");
#if defined(VHWA_MCU_PLUS_SDK)
            scanf("%d", &testIdx);
#else
            testIdx = App_getNum();
#endif
#endif

            switch (testIdx)
            {
            case -1:
                done = UTRUE;
                break;

            default:
                if ((uint32_t)testIdx < (sizeof(gAppMscObj) / sizeof(App_MscTestParams)))
                {
                    App_MscRun(testIdx);
                }
                else
                {
                    Vhwa_print("Invalid Test ID. Enter Again!!\n");
                }
                break;
            }
        }
    }
    App_MscDeInit();

    #if defined (LDRA_COVERAGE)
    Vhwa_print("################### DYNAMIC CODE COVERAGE TERMINATION STARTED ######################\n");
    ldra_terminate();
    Vhwa_print(" Exiting_MSC \n");
    Vhwa_print("################### DYNAMIC CODE COVERAGE TERMINATION FINISHED ######################\n");
    #endif 

    Vhwa_print(" Exiting \n");

    return;
}

static void App_MscRun(uint32_t testCnt)
{
    uint32_t hndlCnt;
    App_MscTestParams *tObj = NULL;

#if defined(VHWA_MCU_PLUS_SDK)
    /* FreeRTOS task handles */
    TaskHandle_t taskHandles[APP_MSC_MAX_HANDLES];
    taskParams taskParams[APP_MSC_MAX_HANDLES];

#else
    TaskP_Handle task[APP_MSC_MAX_HANDLES];
    TaskP_Params taskParams[APP_MSC_MAX_HANDLES];
#endif

    tObj = &gAppMscObj[testCnt];

    gMscTestSrcBufFreeIdx = 0U;
    gMscTestDstBufFreeIdx = 0U;

    if (UTRUE == tObj->isEnableTest)
    {
        Vhwa_print("\n Starting Test %s\n", tObj->testName);

        for (hndlCnt = 0U; hndlCnt < tObj->numHandles;
             hndlCnt++)
        {
#if defined(VHWA_MCU_PLUS_SDK)
            /* Set task parameters */
            taskParams[hndlCnt].testObj = tObj;
            taskParams[hndlCnt].handleIdx = hndlCnt;

            /* Create FreeRTOS task */
            taskHandles[hndlCnt] = xTaskCreateStatic(
                App_MscTestTaskWrapper,      /* Function that implements the task */
                "MSC_TEST",                  /* Task name */
                MAIN_TASK_SIZE,              /* Stack size in words */
                &taskParams[hndlCnt],        /* Parameters */
                MSC_APP_TEST_TASKS_PRIORITY, /* Task priority */
                taskStacks[hndlCnt],         /* Stack buffer */
                &taskBuffers[hndlCnt]        /* Task buffer */
            );

            if (NULL == taskHandles[hndlCnt])
            {
                Vhwa_print(" Task%d create failed... \n", hndlCnt);
                /* Delete previously created tasks before returning */
                while (hndlCnt > 0U)
                {
                    hndlCnt--;
                    vTaskDelete(taskHandles[hndlCnt]);
                }
                return;
            }

#else
            /* Initialization of task parameters for PDK platforms */
            TaskP_Params_init(&taskParams[hndlCnt]);

            /* Set the task priority higher than the
                default priority (1) */
            taskParams[hndlCnt].priority = MSC_APP_TEST_TASKS_PRIORITY;
            taskParams[hndlCnt].stack = gAppTskStackTest[hndlCnt];
            taskParams[hndlCnt].stacksize = sizeof(gAppTskStackTest[hndlCnt]);
            taskParams[hndlCnt].arg0 = (void *)tObj;
            taskParams[hndlCnt].arg1 = (void *)hndlCnt;

            /* Start a new Task */
            task[hndlCnt] = TaskP_create(&App_MscTest,
                                         &taskParams[hndlCnt]);
            if (NULL == task[hndlCnt])
            {
                Vhwa_print(" Task%d create failed... \n", hndlCnt);
                OS_stop();
            }
#endif
        }

        /* Wait for all tesks to get completed */
        for (hndlCnt = 0U; hndlCnt < tObj->numHandles; hndlCnt++)
        {
            VhwaAl_SemaphoreP_pend(waitForTaskCmpl[hndlCnt],
                                   SYSTEMP_WAIT_FOREVER);

            Vhwa_print(" Handle %d processing Completed \n", hndlCnt);

#if defined(VHWA_MCU_PLUS_SDK)
            /* Delete FreeRTOS task */
            vTaskDelete(taskHandles[hndlCnt]);
#else
            /* Delete Task */
            if (NULL != task[hndlCnt])
            {
                TaskP_delete(&task[hndlCnt]);
            }
#endif
        }
        Vhwa_print(" Test %s Completed \n", tObj->testName);
    }

    return;
}

static void App_MscTest(void *a0, void *a1)
{
    int32_t                 status = FVID2_SOK;
    uint32_t                hndlIdx = (uint32_t)a1;
    uint32_t                repCnt;
    uint64_t                timeCount;
    uint64_t                perf;
    uint64_t                srcBufOffset, dstBufOffset;
    App_MscTestParams       *tObj = (App_MscTestParams *)a0;
    uint32_t                inFrmSize = 0, outFrmSize = 0;

    status = AppMsc_Create(tObj, hndlIdx);
    if (FVID2_SOK != status)
    {
        Vhwa_print(" MSC_TEST_APP: Create Failed for %d\n", hndlIdx);
        status = FVID2_EFAIL;
    }

    /* Update to set coefficient onlu once per test */
    status = AppMsc_SetCoeff(tObj, hndlIdx, &gCoefTbl[0]);
    if (FVID2_SOK != status)
    {
        Vhwa_print(" MSC_TEST_APP: SetCoeff Failed for %d\n", hndlIdx);
        status = FVID2_EFAIL;
    }

    if (FVID2_SOK == status)
    {
        status = AppMsc_SetParams(tObj, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" MSC_TEST_APP: SetParams Failed \n");
            status = FVID2_EFAIL;
        }
    }

    if (FVID2_SOK == status)
    {
        VhwaAl_SemaphoreP_pend(gAllocLock, SYSTEMP_WAIT_FOREVER);

        srcBufOffset = hndlIdx * VHWA_MSC_TEST_IN_BUF_OFFSET;
        dstBufOffset = hndlIdx * VHWA_MSC_TEST_OUT_BUF_OFFSET;

        AppMsc_AllocBuffers(tObj, hndlIdx,
                            (gMscTestSrcBuf + gMscTestSrcBufFreeIdx + srcBufOffset), &inFrmSize,
                            APP_MSC_IN_BUFF_OFFSET,
                            (gMscTestDestBuf + gMscTestDstBufFreeIdx + dstBufOffset), &outFrmSize,
                            APP_MSC_OUT_BUFF_OFFSET);

        VhwaAl_SemaphoreP_post(gAllocLock);
    }

#if defined(VHWA_PDK)
    Vhwa_print(" Load buffers and press any key to continue\n");
    UART_getc();
#endif

    if (tObj->isPerformanceTest)
    {
        timeCount = Vhwa_getTimeUsec();
    }

    for (repCnt = 0u; (repCnt < tObj->repeatCnt) &&
                      (FVID2_SOK == status);
         repCnt++)
    {

        AppMsc_PrepareRequest(tObj, hndlIdx);

        /* Submit Request*/
        status = AppMsc_SubmitRequest(tObj, hndlIdx);

        if (FVID2_SOK == status)
        {
            /* Wait for Request Completion */
            status = AppMsc_WaitForComplRequest(tObj, hndlIdx);
            if (FVID2_SOK != status)
            {
                Vhwa_print(" Failed to wait for request completion \n");
            }
        }
        else
        {
            Vhwa_print(" Failed to Submit Request \n");
        }
    }

    if (tObj->isPerformanceTest)
    {
        timeCount = Vhwa_getTimeUsec() - timeCount;
        Vhwa_print("Performance:\n\t FrameCount: %d: Time in uSec: %d\n",
                   tObj->repeatCnt, timeCount);

        perf = (uint64_t)tObj->testCfg[hndlIdx]->inFrm.inWidth * (uint64_t)tObj->testCfg[hndlIdx]->inFrm.inHeight * (uint64_t)tObj->repeatCnt;
        if (FVID2_DF_YUV420SP_UV == tObj->testCfg[hndlIdx]->inFrm.inDataFmt)
        {
            perf = perf * 3U / 2U;
        }
        Vhwa_print("\t MPix/s: %d.%d\n",
                   (uint32_t)(perf / timeCount),
                   (uint32_t)(((perf * (uint64_t)100) / timeCount) % 100));
    }

    if (FVID2_SOK == status)
    {
        Vhwa_print(" Completed HandleCnt %d RepeatCnt %d\n",
                   hndlIdx, repCnt);
    }

    AppMsc_Delete(tObj, hndlIdx);

    VhwaAl_SemaphoreP_post(waitForTaskCmpl[hndlIdx]);
}

static int32_t App_MscInit(void)
{
    int32_t                 status;
    uint32_t                instId;
    uint32_t                hndlCnt;
    Udma_DrvHandle          drvHandle = &gMscAppUdmaDrvObj;
    Udma_InitPrms           udmaInitPrms;

#if defined(VHWA_MCU_PLUS_SDK)
    /* Initialize FVID2 */
    status = Fvid2_init(NULL);
    if (FVID2_SOK != status)
    {
        Vhwa_print(" main: FVID2_Init Failed !!!\r\n");
    }

    if (FVID2_SOK == status)
    {
        /* Initialize UDMA and get the handle */
        instId = VHWA_UDMA_INST_ID_0;
        status = UdmaInitPrms_init(instId, &udmaInitPrms);
        if (UDMA_SOK != status)
        {
            Vhwa_print("[Error] UDMA prms init failed!!\n");
            status = UDMA_EFAIL;
        }

        udmaInitPrms.instId = VHWA_UDMA_INST_ID_0;
        udmaInitPrms.enableUtc = UTRUE;

        status = Udma_init(drvHandle, &udmaInitPrms);
        if (UDMA_SOK != status)
        {
            Vhwa_print("[Error] UDMA init failed!!\n");
            status = FVID2_EFAIL;
        }
    }
#else
    /* PDK initialization */
    Board_initCfg boardCfg;
    SemaphoreP_Params params[APP_MSC_MAX_HANDLES];
    SemaphoreP_Params semAllocPrms;
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
        if (UDMA_SOK != status)
        {
            Vhwa_print("[Error] UDMA init failed!!\n");
            status = FVID2_EFAIL;
        }
    }
#endif

    status = AppMsc_Init(drvHandle);
#if defined(VHWA_PDK)
    if (FVID2_SOK == status)
    {
        /* Initialize the UDMA channel for CRC */
        status = AppMsc_CrcInit(drvHandle);
    }
#endif

    /* Create synchronization semaphores */
    if (FVID2_SOK == status)
    {
        /* Create semaphores for task completion */
        for (hndlCnt = 0U; hndlCnt < APP_MSC_MAX_HANDLES; hndlCnt++)
        {
#if defined(VHWA_MCU_PLUS_SDK)
            /* Use static initialization for FreeRTOS */
            status = SemaphoreP_constructBinary(&waitForTaskCmpl[hndlCnt], 0);
            if (status != SystemP_SUCCESS)
            {
                Vhwa_print(" Could not Create Semaphore for handle %d\n", hndlCnt);
                status = FVID2_EFAIL;
                break;
            }
#else
            /* Dynamic allocation for PDK */
            SemaphoreP_Params_init(&params[hndlCnt]);
            params[hndlCnt].mode = SemaphoreP_Mode_BINARY;
            waitForTaskCmpl[hndlCnt] = SemaphoreP_create(0U, &params[hndlCnt]);
            if (NULL == waitForTaskCmpl[hndlCnt])
            {
                Vhwa_print(" Could not Create Semaphore for handle %d\n", hndlCnt);
                status = FVID2_EFAIL;
                break;
            }
#endif
        }
    }

    /* Create allocation lock semaphore */
    if (FVID2_SOK == status)
    {
#if defined(VHWA_MCU_PLUS_SDK)
        status = SemaphoreP_constructBinary(&gAllocLock, 1);
        if (status != SystemP_SUCCESS)
        {
            Vhwa_print(" Could not Create Allocation Lock Semaphore\n");
            status = FVID2_EFAIL;
        }
#else
        SemaphoreP_Params_init(&semAllocPrms);
        semAllocPrms.mode = SemaphoreP_Mode_BINARY;
        gAllocLock = SemaphoreP_create(1U, &semAllocPrms);
        if (NULL == gAllocLock)
        {
            Vhwa_print(" Could not Create Allocation Lock Semaphore\n");
            status = FVID2_EFAIL;
        }
#endif
    }

    return (status);
}

static void App_MscDeInit()
{
    int32_t status;
    uint32_t cnt;
    Udma_DrvHandle drvHandle = &gMscAppUdmaDrvObj;

    /* De-initialize MSC module */
    AppMsc_DeInit();

    /* De-initialize CRC functionality */
#if defined VHWA_PDK
    AppMsc_CrcDeinit(drvHandle);
#endif

    /* De-initialize UDMA */
    status = Udma_deinit(drvHandle);
    if (UDMA_SOK != status)
    {
        Vhwa_print("[Error] UDMA deinit failed!!\n");
    }

    /* Clean up semaphores */
    for (cnt = 0U; cnt < APP_MSC_MAX_HANDLES; cnt++)
    {
#if defined(VHWA_MCU_PLUS_SDK)
        SemaphoreP_destruct(&waitForTaskCmpl[cnt]);
#else
        if (NULL != waitForTaskCmpl[cnt])
        {
            SemaphoreP_delete(waitForTaskCmpl[cnt]);
        }
#endif
    }

    /* Clean up allocation lock */
#if defined(VHWA_MCU_PLUS_SDK)
    SemaphoreP_destruct(&gAllocLock);
#else
    if (gAllocLock != NULL)
    {
        SemaphoreP_delete(gAllocLock);
        gAllocLock = NULL;
    }
#endif

    /* De-initialize FVID2 */
    Fvid2_deInit(NULL);

#if defined(VHWA_MCU_PLUS_SDK)
    /* For MCU+ SDK, close drivers and deinitialize board */
    Board_driversClose();
    Drivers_close();
    Board_deinit();
    System_deinit();
#endif
}

#if defined(VHWA_PDK)
int32_t App_getNum(void)
{
    int32_t num;

    UART_scanFmt("%d", &num);

    return (num);
}
#endif

#if defined(VHWA_MCU_PLUS_SDK)
/* Wrapper function to adapt to FreeRTOS task signature */
static void App_MscmainTaskFuncWrapper(void *args)
{
    /* Call the original function with the expected parameters */
    App_MscmainTaskFunc(NULL, NULL);

    /* Delete the task when done */
    vTaskDelete(NULL);
}

/* Wrapper function for the test task to match FreeRTOS signature */
static void App_MscTestTaskWrapper(void *args)
{
    taskParams *params = (taskParams *)args;

    /* Call the original test function with the expected parameters */
    App_MscTest(params->testObj, (void *)params->handleIdx);

    /* Delete the task when done */
    vTaskDelete(NULL);
}
#endif