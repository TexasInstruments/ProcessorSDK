
/*
 *  Copyright (c) Texas Instruments Incorporated 2019-2021
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
#include <string.h>
#include <stdlib.h>



#include <include/vhwa_m2mSde.h>
#include <examples/include/vhwa_sde_api.h>
#include "vhwa_sde_cfg.h"
#if defined(LDRA_COVERAGE)
#include <examples/include/ldra_vhwa_coverage_main.h>
#endif

#if defined(VHWA_PDK)
#include "ti/osal/osal.h"
#include "ti/osal/TaskP.h"
#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#elif defined(VHWA_MCU_PLUS_SDK)
#include <kernel/dpl/DebugP.h>
#include "ti_drivers_config.h"
#include "ti_board_config.h"
#include <kernel/dpl/SemaphoreP.h>
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
#define SDE_APP_MAIN_TASKS_PRIORITY (configMAX_PRIORITIES - 1)
#define SDE_APP_TEST_TASKS_PRIORITY (configMAX_PRIORITIES - 2)
#else
#define APP_TSK_STACK_MAIN (64U * 1024U)
#define SDE_APP_MAIN_TASKS_PRIORITY (7U)
#define SDE_APP_TEST_TASKS_PRIORITY (5U)
#endif

/* Input Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be loaded in one shot */
#define VHWA_SDE_TEST_IN_BUF_OFFSET (8 * 1024 * 1024)

/* Output Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be saved in one shot */
#define VHWA_SDE_TEST_OUT_BUF_OFFSET (16 * 1024 * 1024)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
#if defined(VHWA_MCU_PLUS_SDK)
/* Task parameters structure for SDE tasks */
typedef struct
{
    AppSde_TestParams *tObj;
    uint32_t handleIdx;
} taskParams;
#endif

static VhwaAl_SemaphoreP_Struct waitForTaskCmpl[APP_SDE_MAX_HANDLES];

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void App_mainTaskFunc(void *a0, void *a1);
static void App_test(void *a0, void *a1);
static int32_t App_init(void);
static void App_run(uint32_t testCnt);
static void App_deInit(void);
#if defined(VHWA_PDK)
static int32_t App_getNum(void);
#endif
#if defined(VHWA_MCU_PLUS_SDK)
static void SdeApp_mainTaskFuncWrapper(void *args);
static void SdeApp_testTaskFuncWrapper(void *args);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
#if defined(VHWA_MCU_PLUS_SDK)
static StackType_t gMainTaskStack[MAIN_TASK_SIZE]
    __attribute__((aligned(32)));
static StaticTask_t gMainTaskObj;
static StackType_t taskStacks[APP_SDE_MAX_HANDLES][MAIN_TASK_SIZE]
    __attribute__((aligned(32)));
static StaticTask_t taskBuffers[APP_SDE_MAX_HANDLES];

#elif defined(SAFERTOS)
static uint8_t gAppTskStackMain[APP_TSK_STACK_MAIN]
    __attribute__((aligned(APP_TSK_STACK_MAIN)));
static uint8_t gAppTskStackTest[APP_SDE_MAX_HANDLES][APP_TSK_STACK_MAIN]
    __attribute__((aligned(APP_TSK_STACK_MAIN)));
#else
static uint8_t gAppTskStackMain[APP_TSK_STACK_MAIN]
    __attribute__((aligned(32)));
static uint8_t gAppTskStackTest[APP_SDE_MAX_HANDLES][APP_TSK_STACK_MAIN]
    __attribute__((aligned(32)));
#endif

/*
* Application Buffers
*/
#if defined VHWA_MCU_PLUS_SDK
#define gSdeDDRSection 0xB0000000U
#define buffOffset 0x2000000U

static uint64_t gSdeBaseSrcBuf = (uint64_t)(gSdeDDRSection + 1 * buffOffset); // 0xC2000000
static uint64_t gSdeRefSrcBuf = (uint64_t)(gSdeDDRSection + 2 * buffOffset);  // 0xC4000000
static uint64_t gSdeOutBuf = (uint64_t)(gSdeDDRSection + 3 * buffOffset);     // 0xC6000000
#else
static uint64_t gSdeBaseSrcBuf = (uint64_t)(0x90000000U);
static uint64_t gSdeRefSrcBuf = (uint64_t)(0xA0000000U);
static uint64_t gSdeOutBuf = (uint64_t)(0xB0000000U);
#endif
static uint32_t gSdeBaseSrcBufFreeIdx = 0u;
static uint32_t gSdeRefSrcBufFreeIdx = 0u;
static uint32_t gSdeOutBufFreeIdx = 0u;

uint32_t gConfScoreMap[][DMPAC_SDE_NUM_SCORE_MAP] =
    {
        /* CS MAP 1  */
        {0x10, 0x3e, 0x45, 0x49, 0x4b, 0x5a, 0x5e, 0x7F /* unused set to maximum */},
};

static AppSde_TestCfg gAppSdeTestCfg[] =
    {
#include <examples/include/vhwa_sde_test_cfg.h>
};

static AppSde_TestParams gAppSdeObj[] = VHWA_SDE_TIRTOS_CFG;

static VhwaAl_Udma_DrvObj_Struct gSdeAppUdmaDrvObj;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
#if defined(VHWA_MCU_PLUS_SDK)
    /* Initialize MCU+ SDK system components */
    System_init();
    Board_init();
    Drivers_open();
    Board_driversOpen();

    TaskHandle_t gMainTask;

    /* Create the main task using FreeRTOS API */
    gMainTask = xTaskCreateStatic(
        SdeApp_mainTaskFuncWrapper,  /* Function that implements the task */
        "sde_main_task",             /* Text name for debugging */
        MAIN_TASK_SIZE,              /* Stack size in words */
        NULL,                        /* No parameters needed */
        SDE_APP_MAIN_TASKS_PRIORITY, /* Priority */
        gMainTaskStack,              /* High priority */
        &gMainTaskObj                /* Stack buffer */
    );
    configASSERT(gMainTask != NULL);

    /* Start the scheduler */
    vTaskStartScheduler();

    /* Should never reach here */
    DebugP_assertNoLog(0);
#else
    TaskP_Handle task;
    TaskP_Params taskParams;

    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);

    taskParams.priority = SDE_APP_MAIN_TASKS_PRIORITY;
    taskParams.stack = gAppTskStackMain;
    taskParams.stacksize = sizeof(gAppTskStackMain);

    task = TaskP_create(&App_mainTaskFunc, &taskParams);
    if (NULL == task)
    {
        OS_stop();
    }

    OS_start(); /* does not return */
#endif
    return (0);
}

static void App_mainTaskFunc(void *a0, void *a1)
{
    int32_t status;
    bool done = UFALSE;
    int32_t testIdx;
    #if defined (LDRA_COVERAGE)
    /*Expand gApprovedTestCases[] to include future testcases*/
    static const int32_t gApprovedPositiveTestCases [] = {0,19,20,21,22,2,4,5,6,1,3};
    static const int32_t gApprovedNegativeTestCases [] = {23,24,25,-1};
    uint32_t i = 0,j =0, PositiveTestcasesMax = 0; 
    PositiveTestcasesMax = sizeof(gApprovedPositiveTestCases)/sizeof(gApprovedPositiveTestCases[0]);
    #endif

    Vhwa_print("\n Starting SDE Test ..\n");
#if defined(LDRA_COVERAGE)
    Vhwa_print("################### DYNAMIC CODE COVERAGE INITIALIZATION STARTED ######################\n");
    ldra_initialize();
    Vhwa_print("################### DYNAMIC CODE COVERAGE INITIALIZATION FINISHED ######################\n");
#endif

    status = App_init();

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
            Vhwa_print("\n Enter Test to Run (Enter '-1' to exit)  \n");
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
                if ((uint32_t)testIdx < (sizeof(gAppSdeObj) / sizeof(AppSde_TestParams)))
                {
                    App_run(testIdx);
                }
                else
                {
                    Vhwa_print("Invalid Test ID. Enter Again!!\n");
                }
                break;
            }
        }
    }

    App_deInit();

#if defined(LDRA_COVERAGE)
    Vhwa_print("################### DYNAMIC CODE COVERAGE TERMINATION STARTED ######################\n");
    ldra_terminate();
    Vhwa_print(" Exiting_SDE \n");
    Vhwa_print("################### DYNAMIC CODE COVERAGE TERMINATION FINISHED ######################\n");
#endif

    Vhwa_print(" Exiting \n");

    return;
}

static void App_run(uint32_t testCnt)
{
    uint32_t hndlCnt;
    AppSde_TestParams *tObj = NULL;

#if defined(VHWA_MCU_PLUS_SDK)
    TaskHandle_t taskHandles[APP_SDE_MAX_HANDLES];
    taskParams taskParams[APP_SDE_MAX_HANDLES];
#else
    TaskP_Handle task[APP_SDE_MAX_HANDLES];
    TaskP_Params taskParams[APP_SDE_MAX_HANDLES];
#endif

    tObj = &gAppSdeObj[testCnt];

    gSdeBaseSrcBufFreeIdx = 0u;
    gSdeRefSrcBufFreeIdx = 0u;
    gSdeOutBufFreeIdx = 0u;

    if (UTRUE == tObj->isEnableTest)
    {
        Vhwa_print("\n Starting Test %s\n", tObj->testName);

        for (hndlCnt = 0U; hndlCnt < tObj->numHandles;
             hndlCnt++)
        {
#if defined(VHWA_MCU_PLUS_SDK)
            /* Set task parameters*/
            taskParams[hndlCnt].tObj = tObj;
            taskParams[hndlCnt].handleIdx = hndlCnt;

            /* Create the task */
            taskHandles[hndlCnt] = xTaskCreateStatic(
                SdeApp_testTaskFuncWrapper,  /* Function that implements the task */
                "SDE_TEST",                  /* Task name */
                MAIN_TASK_SIZE,              /* Stack size in words */
                &taskParams[hndlCnt],        /* parameters needed */
                SDE_APP_TEST_TASKS_PRIORITY, /* High priority */
                taskStacks[hndlCnt],         /* Stack buffer */
                &taskBuffers[hndlCnt]        /* Static task buffer */
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
            /* Initialize the task params */
            TaskP_Params_init(&taskParams[hndlCnt]);

            /* Set the task priority higher than the
                default priority (1) */
            taskParams[hndlCnt].priority = SDE_APP_TEST_TASKS_PRIORITY;
            taskParams[hndlCnt].stack = gAppTskStackTest[hndlCnt];
            taskParams[hndlCnt].stacksize = sizeof(gAppTskStackTest[hndlCnt]);
            taskParams[hndlCnt].arg0 = (void *)tObj;
            taskParams[hndlCnt].arg1 = (void *)hndlCnt;

            /* Start a new Task */
            task[hndlCnt] = TaskP_create(&App_test,
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
            /* Delete the task */
            vTaskDelete(taskHandles[hndlCnt]);
#else
            if (NULL != task[hndlCnt])
            {
                TaskP_delete(&task[hndlCnt]);
            }
#endif
        }
        Vhwa_print("\n Test %s Completed \n", tObj->testName);
    }
    else
    {
        Vhwa_print ("\n Test %d - %s is Disabled \n", testCnt, tObj->testName);
    }

    return;
}

static void App_test(void *a0, void *a1)
{
    int32_t                 status = FVID2_SOK;
    uint32_t                hndlIdx = (uint32_t)a1;
    uint32_t                repCnt;
    uint32_t                refBuffSize = 0;
    uint32_t                baseBuffSize = 0;
    uint32_t                outBuffSize = 0;
    uint64_t                timeCount = 0;
    uint64_t                baseOffset, refOffset, outOffset;
    AppSde_TestParams       *tObj = (AppSde_TestParams *)a0;

    status = AppSde_Create(tObj, hndlIdx);
    if (FVID2_SOK != status)
    {
        Vhwa_print(" SDE_TEST_APP: Create Failed for %d\n", hndlIdx);
        status = FVID2_EFAIL;
    }

    if (FVID2_SOK == status)
    {
        status = AppSde_SetParams(tObj, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" SDE_TEST_APP: SetParams Failed \n");
            status = FVID2_EFAIL;
        }
    }

    if (FVID2_SOK == status)
    {
        /* Setup buffer addresses with offsets based on handle index to avoid buffer overlap */
        baseOffset = hndlIdx * VHWA_SDE_TEST_IN_BUF_OFFSET;
        refOffset = hndlIdx * VHWA_SDE_TEST_IN_BUF_OFFSET;
        outOffset = hndlIdx * VHWA_SDE_TEST_OUT_BUF_OFFSET;

        AppSde_AllocBuffers(tObj, hndlIdx,
                            gSdeBaseSrcBuf + gSdeBaseSrcBufFreeIdx + baseOffset, &baseBuffSize,
                            gSdeRefSrcBuf + gSdeRefSrcBufFreeIdx + refOffset, &refBuffSize,
                            gSdeOutBuf + gSdeOutBufFreeIdx + outOffset, &outBuffSize);

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
            AppSde_PrepareRequest(tObj, hndlIdx);

            /* Submit Request*/
            status = AppSde_SubmitRequest(tObj, hndlIdx);

            if (FVID2_SOK == status)
            {
                /* Wait for Request Completion */
                status = AppSde_WaitForComplRequest(tObj, hndlIdx);
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
            Vhwa_print("\t MPix/s: %d.%d\n",
                       (uint32_t)(((uint64_t)tObj->testCfg[hndlIdx]->imgWidth * (uint64_t)tObj->testCfg[hndlIdx]->imgHeight * (uint64_t)tObj->repeatCnt) / timeCount),
                       (uint32_t)((((uint64_t)tObj->testCfg[hndlIdx]->imgWidth * (uint64_t)tObj->testCfg[hndlIdx]->imgHeight * (uint64_t)tObj->repeatCnt * (uint64_t)100) / timeCount) % 100));
        }

        if (FVID2_SOK == status)
        {
            Vhwa_print(" Completed HandleCnt %d \n", hndlIdx);
        }
    }

    AppSde_Delete(tObj, hndlIdx);

    VhwaAl_SemaphoreP_post(waitForTaskCmpl[hndlIdx]);
}

static int32_t App_init(void)
{
    int32_t                 status;
    uint32_t                instId;
    uint32_t                hndlCnt;
    Udma_DrvHandle          drvHandle = &gSdeAppUdmaDrvObj;
    Udma_InitPrms           udmaInitPrms;
    
#if defined VHWA_PDK
    Board_initCfg boardCfg;
    Fvid2_InitPrms initPrmsFvid2;
    SemaphoreP_Params params[APP_SDE_MAX_HANDLES];
    boardCfg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    Fvid2InitPrms_init(&initPrmsFvid2);
    initPrmsFvid2.printFxn = Vhwa_print;
    status = Fvid2_init(&initPrmsFvid2);
#else
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
        if (UDMA_SOK != status)
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
        if (UDMA_SOK != status)
        {
            Vhwa_print("[Error] UDMA init failed!!\n");
            status = FVID2_EFAIL;
        }
    }

    status = AppSde_Init(drvHandle);
#if defined VHWA_PDK
    if (FVID2_SOK == status)
    {
        /* Initialize the UDMA channel for CRC */
        status = AppSde_CrcInit(drvHandle);
    }
#endif

    if (FVID2_SOK == status)
    {
        for (hndlCnt = 0U; hndlCnt < APP_SDE_MAX_HANDLES; hndlCnt++)
        {
#if defined(VHWA_MCU_PLUS_SDK)
            /* Create binary semaphores using static allocation */
            status = SemaphoreP_constructBinary(&waitForTaskCmpl[hndlCnt], 0);
            if (status != FVID2_SOK)
            {
                Vhwa_print(" Could not create Semaphore for handle %d\n", hndlCnt);
                status = FVID2_EFAIL;
                break;
            }
#else
            /* Create binary semaphores using dynamic allocation for PDK */
            SemaphoreP_Params_init(&params[hndlCnt]);
            params[hndlCnt].mode = SemaphoreP_Mode_BINARY;
            waitForTaskCmpl[hndlCnt] =
                SemaphoreP_create(0U, &params[hndlCnt]);
            if (NULL == waitForTaskCmpl[hndlCnt])
            {
                Vhwa_print(" Could not create Semaphore \n");
                status = FVID2_EFAIL;
                break;
            }
#endif
        }
    }

    return (status);
}

static void App_deInit()
{
    int32_t status;
    uint32_t cnt;
    Udma_DrvHandle drvHandle = &gSdeAppUdmaDrvObj;

    Vhwa_m2mSdeDeInit();

#if defined(VHWA_PDK)
    AppSde_CrcDeinit(drvHandle);
#endif

    status = Udma_deinit(drvHandle);
    if (UDMA_SOK != status)
    {
        Vhwa_print("[Error] UDMA deinit failed!!\n");
    }

    for (cnt = 0U; cnt < APP_SDE_MAX_HANDLES; cnt++)
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
static void SdeApp_mainTaskFuncWrapper(void *args)
{
    App_mainTaskFunc(NULL, NULL);

    vTaskDelete(NULL);
}

static void SdeApp_testTaskFuncWrapper(void *args)
{
    taskParams *params = (taskParams *)args;

    App_test((void *)params->tObj, (void *)params->handleIdx);

    vTaskDelete(NULL);
}
#endif