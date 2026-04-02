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

#include <include/vhwa_m2mLdc.h>
#include <examples/include/vhwa_ldc_api.h>
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
#include <drivers/device_manager/sciclient.h>
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
#define LDC_APP_MAIN_TASKS_PRIORITY (configMAX_PRIORITIES - 1)
#define LDC_APP_TEST_TASKS_PRIORITY (configMAX_PRIORITIES - 2)
#elif defined(VHWA_PDK)
#define APP_TSK_STACK_MAIN (64U * 1024U)
#define LDC_APP_MAIN_TASKS_PRIORITY (7U)
#define LDC_APP_TEST_TASKS_PRIORITY (5U)
#endif

/* Input Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be loaded in one shot */
#define VHWA_LDC_TEST_IN_BUF_OFFSET (8 * 1024 * 1024)

/* Output Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be saved in one shot */
#define VHWA_LDC_TEST_OUT_BUF_OFFSET (16 * 1024 * 1024)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

#if defined(VHWA_MCU_PLUS_SDK)
/* Define task parameters storage once */
typedef struct
{
    LdcApp_TestParams *testObj;
    uint32_t handleIdx;
} taskParams;
#endif

static VhwaAl_SemaphoreP_Struct waitForTaskCmpl[APP_LDC_MAX_HANDLES];

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void LdcApp_mainTaskFunc(void *a0, void *a1);
static void LdcApp_test(void *a0, void *a1);
static int32_t LdcApp_init(void);
static void LdcApp_run(uint32_t testCnt);
static void LdcApp_deInit(void);
#if defined(VHWA_PDK)
static int32_t App_getNum(void);
#endif
#if defined(VHWA_MCU_PLUS_SDK)
static void LdcApp_mainTaskFuncWrapper(void *args);
static void LdcApp_testTaskWrapper(void *args);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
#if defined(VHWA_MCU_PLUS_SDK)
static StackType_t gMainTaskStack[MAIN_TASK_SIZE]
    __attribute__((aligned(32)));
static StaticTask_t gMainTaskObj;
static StackType_t taskStacks[APP_LDC_MAX_HANDLES][MAIN_TASK_SIZE]
    __attribute__((aligned(32)));
static StaticTask_t taskBuffers[APP_LDC_MAX_HANDLES];

#elif defined(SAFERTOS)
static uint8_t gAppTskStackMain[APP_TSK_STACK_MAIN]
    __attribute__((aligned(APP_TSK_STACK_MAIN)));
static uint8_t gAppTskStackTest[APP_LDC_MAX_HANDLES][APP_TSK_STACK_MAIN]
    __attribute__((aligned(APP_TSK_STACK_MAIN)));
#else
static uint8_t gAppTskStackMain[APP_TSK_STACK_MAIN]
    __attribute__((aligned(32)));
static uint8_t gAppTskStackTest[APP_LDC_MAX_HANDLES][APP_TSK_STACK_MAIN]
    __attribute__((aligned(32)));
#endif

/*
* Application Buffers
*/
#if defined(VHWA_MCU_PLUS_SDK)
static uint64_t gLdcTestSrcBuf = (uint64_t)(0xB2000000u);
static uint64_t gLdcTestDestBuf = (uint64_t)(0xB9000000u);
#else
static uint64_t gLdcTestSrcBuf = (uint64_t)(VHWA_EXAMPLE_BUFF_START_ADDR);
static uint64_t gLdcTestDestBuf = (uint64_t)(VHWA_EXAMPLE_BUFF_START_ADDR + 0x10000000u);
#endif

static uint32_t gLdcTestSrcBufFreeIdx = 0u;
static uint32_t gLdcTestDstBufFreeIdx = 0u;

static AppLdc_TestConfig gAppLdcTestCfg[] =
    {
#include <examples/include/vhwa_ldc_test_cfg.h>
};

static LdcApp_TestParams gAppLdcObj[] =
    {
#if defined VHWA_VPAC_IP_REV_VPAC || defined VHWA_VPAC_IP_REV_VPAC3
#include <ti/drv/vhwa/examples/vhwa_ldc_test/vhwa_ldc_cfg.h>
#endif
#if defined VHWA_VPAC_IP_REV_VPAC3L
#include <examples/vhwa_ldc_test/vhwa_ldc_cfg.h>
#endif
};

static VhwaAl_Udma_DrvObj_Struct gLdcAppUdmaDrvObj;

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
        LdcApp_mainTaskFuncWrapper,  /* Function that implements the task */
        "ldc_main_task",             /* Text name for debugging */
        MAIN_TASK_SIZE,              /* Stack size in words */
        NULL,                        /* No parameters needed */
        LDC_APP_MAIN_TASKS_PRIORITY, /* High priority */
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

    taskParams.priority = LDC_APP_MAIN_TASKS_PRIORITY;
    taskParams.stack = gAppTskStackMain;
    taskParams.stacksize = sizeof(gAppTskStackMain);

    task = TaskP_create(&LdcApp_mainTaskFunc, &taskParams);
    if (NULL == task)
    {
        OS_stop();
    }

    OS_start(); /* does not return */

#endif

    return (0);
}

static void LdcApp_mainTaskFunc(void *a0, void *a1)
{
    int32_t status;
    bool done = UFALSE;
    int32_t testIdx;
    #if defined (LDRA_COVERAGE)
     /*Expand gApprovedTestCases[] to include future testcases*/
    static const int32_t gApprovedPositiveTestCases [] = {35,45,43,39,40,41,19,44,37,38,56,59,20,57,58,36,42,48};
    static const int32_t gApprovedNegativeTestCases [] = {-1};
    uint32_t i = 0,j =0, PositiveTestcasesMax = 0; 
    PositiveTestcasesMax = sizeof(gApprovedPositiveTestCases)/sizeof(gApprovedPositiveTestCases[0]);
    #endif

    Vhwa_print("\n Starting LDC Test ..\n");

#if defined(LDRA_COVERAGE)
    Vhwa_print("################### DYNAMIC CODE COVERAGE INITIALIZATION STARTED ######################\n");
    ldra_initialize();
    Vhwa_print("################### DYNAMIC CODE COVERAGE INITIALIZATION FINISHED ######################\n");
#endif

    status = LdcApp_init();

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
#elif defined(VHWA_PDK)
            testIdx = App_getNum();
#endif
#endif

            switch (testIdx)
            {
            case -1:
                done = UTRUE;
                break;

            default:
                if ((uint32_t)testIdx < (sizeof(gAppLdcObj) / sizeof(LdcApp_TestParams)))
                {
                    LdcApp_run(testIdx);
                }
                else
                {
                    Vhwa_print("Invalid Test ID. Enter Again!!\n");
                }
                break;
            }
        }
    }

    LdcApp_deInit();
    
#if defined(LDRA_COVERAGE)
    Vhwa_print("################### DYNAMIC CODE COVERAGE TERMINATION STARTED ######################\n");
    ldra_terminate();
    Vhwa_print(" Exiting_LDC \n");
    Vhwa_print("################### DYNAMIC CODE COVERAGE TERMINATION FINISHED ######################\n");
#endif

    Vhwa_print(" Exiting \n");

    return;
}

static void LdcApp_run(uint32_t testCnt)
{
    uint32_t hndlCnt;
    LdcApp_TestParams *tObj = NULL;

#if defined(VHWA_MCU_PLUS_SDK)
    /* FreeRTOS task handles */
    TaskHandle_t taskHandles[APP_LDC_MAX_HANDLES];

    taskParams taskParams[APP_LDC_MAX_HANDLES];

#else
    TaskP_Handle task[APP_LDC_MAX_HANDLES];
    TaskP_Params taskParams[APP_LDC_MAX_HANDLES];
#endif

    tObj = &gAppLdcObj[testCnt];

    gLdcTestSrcBufFreeIdx = 0U;
    gLdcTestDstBufFreeIdx = 0U;

    if (UTRUE == tObj->isEnableTest)
    {
        Vhwa_print("\n Starting Test %s\n", tObj->testName);

        for (hndlCnt = 0U; hndlCnt < tObj->numHandles; hndlCnt++)
        {
#if defined(VHWA_MCU_PLUS_SDK)
            /* Set task parameters */
            taskParams[hndlCnt].testObj = tObj;
            taskParams[hndlCnt].handleIdx = hndlCnt;

            /* Create FreeRTOS task */
            taskHandles[hndlCnt] = xTaskCreateStatic(
                LdcApp_testTaskWrapper,      /* Task function */
                "LDC_TEST",                  /* Task name */
                MAIN_TASK_SIZE,              /* Stack size */
                &taskParams[hndlCnt],        /* Parameters */
                LDC_APP_TEST_TASKS_PRIORITY, /* Priority */
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
            /* Original Initialization code for PDK platforms */
            TaskP_Params_init(&taskParams[hndlCnt]);

            /* Set the task priority higher than the
                default priority (1) */
            taskParams[hndlCnt].priority = LDC_APP_TEST_TASKS_PRIORITY;
            taskParams[hndlCnt].stack = gAppTskStackTest[hndlCnt];
            taskParams[hndlCnt].stacksize = sizeof(gAppTskStackTest[hndlCnt]);
            taskParams[hndlCnt].arg0 = (void *)tObj;
            taskParams[hndlCnt].arg1 = (void *)hndlCnt;

            /* Start a new Task */
            task[hndlCnt] = TaskP_create(&LdcApp_test, &taskParams[hndlCnt]);
            if (NULL == task[hndlCnt])
            {
                Vhwa_print(" Task%d create failed... \n", hndlCnt);
                OS_stop();
            }
#endif
        }

        /* Wait for all tasks to complete */
        for (hndlCnt = 0U; hndlCnt < tObj->numHandles; hndlCnt++)
        {
            VhwaAl_SemaphoreP_pend(waitForTaskCmpl[hndlCnt], SYSTEMP_WAIT_FOREVER);

            Vhwa_print(" Handle %d processing Completed \n", hndlCnt);

#if defined(VHWA_MCU_PLUS_SDK)
            /* Delete FreeRTOS task */
            vTaskDelete(taskHandles[hndlCnt]);
#else
            /* Delete Task in original code */
            if (NULL != task[hndlCnt])
            {
                TaskP_delete(&task[hndlCnt]);
            }
#endif
        }
        Vhwa_print(" Test %s Completed \n", tObj->testName);
    }
    else
    {
        Vhwa_print ("\n Test %d - %s is Disabled \n", testCnt, tObj->testName);
    }

    return;
}

static void LdcApp_test(void *a0, void *a1)
{
    int32_t                 status = FVID2_SOK;
    uint32_t                hndlIdx = (uint32_t)a1;
    uint32_t                repCnt;
    uint64_t                timeCount;
    uint64_t                perf;
    LdcApp_TestParams       *tObj = (LdcApp_TestParams *)a0;
    uint32_t                inFrmSize, outFrmSize;
    uint64_t                srcBufOffset, dstBufOffset;

    status = AppLdc_Create(tObj, hndlIdx);
    if (FVID2_SOK != status)
    {
        Vhwa_print(" LDC_TEST_APP: Create Failed for %d\n", hndlIdx);
        status = FVID2_EFAIL;
    }

    if (FVID2_SOK == status)
    {
        status = AppLdc_SetParams(tObj, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" LDC_TEST_APP: SetParams Failed \n");
            status = FVID2_EFAIL;
        }
    }

    if (FVID2_SOK == status)
    {
        srcBufOffset = hndlIdx * VHWA_LDC_TEST_IN_BUF_OFFSET;
        dstBufOffset = hndlIdx * VHWA_LDC_TEST_OUT_BUF_OFFSET;
        AppLdc_AllocBuffers(tObj, hndlIdx,
                            (gLdcTestSrcBuf + srcBufOffset), &inFrmSize,
                            (gLdcTestDestBuf + dstBufOffset), &outFrmSize);

        AppLdc_PrepareRequest(tObj, hndlIdx);

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
            /* Submit Request*/
            status = AppLdc_SubmitRequest(tObj, hndlIdx);

            if (FVID2_SOK == status)
            {
                /* Wait for Request Completion */
                status = AppLdc_WaitForComplRequest(tObj, hndlIdx);
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

            perf = (uint64_t)tObj->testCfg[hndlIdx]->inWidth * (uint64_t)tObj->testCfg[hndlIdx]->inHeight * (uint64_t)tObj->repeatCnt;

            Vhwa_print("\t MPix/s: %d.%d\n",
                       (uint32_t)(perf / timeCount),
                       (uint32_t)(((perf * (uint64_t)100) / timeCount) % 100));
        }

        if (FVID2_SOK == status)
        {
            Vhwa_print(" Completed Handle %d RepeatCnt %d\n",
                       hndlIdx, repCnt);
        }
    }

    AppLdc_Delete(tObj, hndlIdx);

    VhwaAl_SemaphoreP_post(waitForTaskCmpl[hndlIdx]);
}

static int32_t LdcApp_init(void)
{
    int32_t status;
    uint32_t instId;
    uint32_t hndlCnt;
    Udma_DrvHandle drvHandle = &gLdcAppUdmaDrvObj;
    Udma_InitPrms udmaInitPrms;

#if defined(VHWA_PDK)
    Board_initCfg boardCfg;
    Fvid2_InitPrms initPrmsFvid2;
    SemaphoreP_Params params[APP_LDC_MAX_HANDLES];

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
            status = UDMA_EFAIL;
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

    status = AppLdc_Init(drvHandle);
#if defined VHWA_PDK
    if (FVID2_SOK == status)
    {
        /* Initialize the UDMA channel for CRC */
        status = AppLdc_CrcInit(drvHandle);
    }
#endif

    if (FVID2_SOK == status)
    {
        /* Initialize semaphores for task synchronization */
#if defined(VHWA_MCU_PLUS_SDK)
        for (hndlCnt = 0U; hndlCnt < APP_LDC_MAX_HANDLES; hndlCnt++)
        {
            /* Create binary semaphores using static allocation */
            status = SemaphoreP_constructBinary(&waitForTaskCmpl[hndlCnt], 0);
            if (status != SystemP_SUCCESS)
            {
                Vhwa_print("Error: Failed to create semaphore %d\n", hndlCnt);
                status = FVID2_EFAIL;
                break;
            }
        }
#else
        /* Original semaphore initialization code */
        for (hndlCnt = 0U; hndlCnt < APP_LDC_MAX_HANDLES; hndlCnt++)
        {
            SemaphoreP_Params_init(&params[hndlCnt]);
            params[hndlCnt].mode = SemaphoreP_Mode_BINARY;
            waitForTaskCmpl[hndlCnt] = SemaphoreP_create(0, &params[hndlCnt]);
            if (NULL == waitForTaskCmpl[hndlCnt])
            {
                Vhwa_print(" Could not create Semaphore \n");
                status = FVID2_EFAIL;
                break;
            }
        }
#endif
    }

    return status;
}

static void LdcApp_deInit()
{
    int32_t status;
    uint32_t cnt;
    Udma_DrvHandle drvHandle = &gLdcAppUdmaDrvObj;

    Vhwa_m2mLdcDeInit();

#if defined(VHWA_PDK)
    AppLdc_CrcDeinit(drvHandle);
#endif

    status = Udma_deinit(drvHandle);
    if (UDMA_SOK != status)
    {
        Vhwa_print("[Error] UDMA deinit failed!!\n");
    }

    /* Clean up semaphores */
    for (cnt = 0U; cnt < APP_LDC_MAX_HANDLES; cnt++)
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
static void LdcApp_mainTaskFuncWrapper(void *args)
{
    /* Call the original function with the expected parameters */
    LdcApp_mainTaskFunc(NULL, NULL);

    /* Delete the task when done */
    vTaskDelete(NULL);
}

/* Wrapper function for the test task to match FreeRTOS signature */
static void LdcApp_testTaskWrapper(void *args)
{
    taskParams *params = (taskParams *)args;

    /* Call the original test function with the expected parameters */
    LdcApp_test(params->testObj, (void *)params->handleIdx);

    /* Delete the task when done */
    vTaskDelete(NULL);
}

#endif