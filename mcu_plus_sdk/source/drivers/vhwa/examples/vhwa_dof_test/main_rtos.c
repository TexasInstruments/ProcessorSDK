
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

#include <include/vhwa_m2mDof.h>
#include <examples/include/vhwa_dof_api.h>
#include "vhwa_dof_cfg.h"
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
#define DOF_APP_MAIN_TASKS_PRIORITY (configMAX_PRIORITIES - 1)
#define DOF_APP_TEST_TASKS_PRIORITY (configMAX_PRIORITIES - 2)
#else
#define APP_TSK_STACK_MAIN (64U * 1024U)
#define DOF_APP_MAIN_TASKS_PRIORITY (7U)
#define DOF_APP_TEST_TASKS_PRIORITY (5U)
#endif

/* Input Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be loaded in one shot */
#define VHWA_DOF_TEST_IN_BUF_OFFSET (8 * 1024 * 1024)

/* Output Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be saved in one shot */
#define VHWA_DOF_TEST_OUT_BUF_OFFSET (16 * 1024 * 1024)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

#if defined(VHWA_MCU_PLUS_SDK)
/* Task parameters structure for DOF tasks */
typedef struct
{
    DofApp_TestParams *tObj;
    uint32_t handleIdx;
} taskParams;
#endif

static VhwaAl_SemaphoreP_Struct waitForTaskCmpl[APP_DOF_MAX_HANDLES];

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void DofApp_mainTaskFunc(void *a0, void *a1);
static void DofApp_test(void *a0, void *a1);
static int32_t DofApp_init(void);
static void DofApp_run(uint32_t testCnt);
static void DofApp_deInit(void);
#if defined(VHWA_PDK)
static int32_t App_getNum(void);
#endif
#if defined(VHWA_MCU_PLUS_SDK)
static void DofApp_mainTaskFuncWrapper(void *args);
static void DofApp_testTaskFuncWrapper(void *args);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
#if defined(VHWA_MCU_PLUS_SDK)
/* FreeRTOS specific stack definition */
static StackType_t gMainTaskStack[MAIN_TASK_SIZE]
    __attribute__((aligned(32)));
static StaticTask_t gMainTaskObj;
static StackType_t taskStacks[APP_DOF_MAX_HANDLES][MAIN_TASK_SIZE]
    __attribute__((aligned(32)));
static StaticTask_t taskBuffers[APP_DOF_MAX_HANDLES];

#elif defined(SAFERTOS)
static uint8_t gAppTskStackMain[APP_TSK_STACK_MAIN]
    __attribute__((aligned(APP_TSK_STACK_MAIN)));
static uint8_t gAppTskStackTest[APP_DOF_MAX_HANDLES][APP_TSK_STACK_MAIN]
    __attribute__((aligned(APP_TSK_STACK_MAIN)));
#else
static uint8_t gAppTskStackMain[APP_TSK_STACK_MAIN]
    __attribute__((aligned(32)));
static uint8_t gAppTskStackTest[APP_DOF_MAX_HANDLES][APP_TSK_STACK_MAIN]
    __attribute__((aligned(32)));
#endif

/*
* Application Buffers
*/

#if defined(VHWA_MCU_PLUS_SDK)
#define gDofDDRSection 0xB0000000U
#define buffOffset 0x2000000U

static uint64_t gRefSrcBuf = gDofDDRSection + (1 * buffOffset);    // 0xC2000000
static uint64_t gCurrSrcBuf = gDofDDRSection + (2 * buffOffset);   // 0xC4000000
static uint64_t gTempSrcBuf = gDofDDRSection + (3 * buffOffset);   // 0xC6000000
static uint64_t gOutBuf = gDofDDRSection + (4 * buffOffset);       // 0xC8000000
static uint64_t gSofSrcBuf = gDofDDRSection + (5 * buffOffset);    // 0xCA000000
static uint64_t gInterOddBuf = gDofDDRSection + (6 * buffOffset);  // 0xCC000000
static uint64_t gInterEvenBuf = gDofDDRSection + (7 * buffOffset); // 0xCE000000
#else
static uint64_t gRefSrcBuf = 0x90000000U;
static uint64_t gCurrSrcBuf = 0xA0000000U;
static uint64_t gTempSrcBuf = 0xB0000000U;
static uint64_t gOutBuf = 0xC0000000U;
static uint64_t gSofSrcBuf = 0x98000000U;
static uint64_t gInterOddBuf = 0xD0000000U;
static uint64_t gInterEvenBuf = 0xE0000000U;
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
#if defined(VHWA_MCU_PLUS_SDK)
    /* Initialize MCU+ SDK system components */
    System_init();
    Board_init();
    Drivers_open();
    Board_driversOpen();

    TaskHandle_t gMainTask;

    /* Create the main task using FreeRTOS API */
    gMainTask = xTaskCreateStatic(
        DofApp_mainTaskFuncWrapper,  /* Function that implements the task */
        "dof_main_task",             /* Text name for debugging */
        MAIN_TASK_SIZE,              /* Stack size in words */
        NULL,                        /* No parameters needed */
        DOF_APP_MAIN_TASKS_PRIORITY, /* High priority */
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

    taskParams.priority = DOF_APP_MAIN_TASKS_PRIORITY;
    taskParams.stack = gAppTskStackMain;
    taskParams.stacksize = sizeof(gAppTskStackMain);

    task = TaskP_create(&DofApp_mainTaskFunc, &taskParams);
    if (NULL == task)
    {
        OS_stop();
    }

    OS_start(); /* does not return */
#endif

    return (0);
}

static void DofApp_mainTaskFunc(void *a0, void *a1)
{
    int32_t status;
    bool done = UFALSE;
    int32_t testIdx;
    #if defined (LDRA_COVERAGE)
    /*Expand gApprovedTestCases[] to include future testcases*/
    static const int32_t gApprovedPositiveTestCases [] = {0,10,11,12,13,14,28,2,6,7,8,9,1,3};
    static const int32_t gApprovedNegativeTestCases [] = {16,17,18,19,20,21,22,23,24,25,26,27,29,31,33,34,-1};
    uint32_t i = 0,j =0, PositiveTestcasesMax = 0; 
    PositiveTestcasesMax = sizeof(gApprovedPositiveTestCases)/sizeof(gApprovedPositiveTestCases[0]);
    #endif

    status = DofApp_init();

    Vhwa_print("\n Starting DOF Test ..\n");

#if defined(LDRA_COVERAGE)
    Vhwa_print("################### DYNAMIC CODE COVERAGE INITIALIZATION STARTED ######################\n");
    ldra_initialize();
    Vhwa_print("################### DYNAMIC CODE COVERAGE INITIALIZATION FINISHED ######################\n");
#endif

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
                if ((uint32_t)testIdx < (sizeof(gAppDofObj) / sizeof(DofApp_TestParams)))
                {
                    DofApp_run(testIdx);
                }
                else
                {
                    Vhwa_print("Invalid Test ID. Enter Again!!\n");
                }
                break;
            }
        }
    }

    
    DofApp_deInit();

#if defined(LDRA_COVERAGE)
    Vhwa_print("################### DYNAMIC CODE COVERAGE TERMINATION STARTED ######################\n");
    ldra_terminate();
    Vhwa_print(" Exiting_DOF \n");
    Vhwa_print("################### DYNAMIC CODE COVERAGE TERMINATION FINISHED ######################\n");
#endif

    Vhwa_print(" Exiting \n");
    return;
}

void DofApp_run(uint32_t testCnt)
{

    uint32_t hndlCnt;
    DofApp_TestParams *tObj = NULL;

#if defined(VHWA_MCU_PLUS_SDK)
    TaskHandle_t taskHandles[APP_DOF_MAX_HANDLES];
    taskParams taskParams[APP_DOF_MAX_HANDLES];
#else
    TaskP_Handle task[APP_DOF_MAX_HANDLES];
    TaskP_Params taskParams[APP_DOF_MAX_HANDLES];
#endif

    tObj = &gAppDofObj[testCnt];

    /* Reset buffer indices... */
    gRefBufFreeIdx = 0u;
    gCurrBufFreeIdx = 0u;
    gTempBufFreeIdx = 0u;
    gSofBufFreeIdx = 0u;
    gOutBufFreeIdx = 0u;

    if (UTRUE == tObj->isEnableTest)
    {
        Vhwa_print("\n Starting Test %s\n", tObj->testName);

        for (hndlCnt = 0U; hndlCnt < tObj->numHandles; hndlCnt++)
        {
#if defined(VHWA_MCU_PLUS_SDK)
            /* Set task parameters */
            taskParams[hndlCnt].tObj = tObj;
            taskParams[hndlCnt].handleIdx = hndlCnt;

            /* Create the task using FreeRTOS static allocation */
            taskHandles[hndlCnt] = xTaskCreateStatic(
                DofApp_testTaskFuncWrapper,  /* Function that implements the task */
                "DOF_Task",                  /* Text name for the task */
                MAIN_TASK_SIZE,              /* Stack size in words, not bytes */
                &taskParams[hndlCnt],        /* Parameter passed into the task */
                DOF_APP_TEST_TASKS_PRIORITY, /* Priority at which the task is created */
                taskStacks[hndlCnt],         /* Array to use as the task's stack */
                &taskBuffers[hndlCnt]);      /* Variable to hold the task's data structure */

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
            /* Initialize the task params for PDK platforms */
            TaskP_Params_init(&taskParams[hndlCnt]);

            /* Set the task priority higher than the
                default priority (1) */
            taskParams[hndlCnt].priority = DOF_APP_TEST_TASKS_PRIORITY;
            taskParams[hndlCnt].stack = gAppTskStackTest[hndlCnt];
            taskParams[hndlCnt].stacksize = sizeof(gAppTskStackTest[hndlCnt]);
            taskParams[hndlCnt].arg0 = (void *)tObj;
            taskParams[hndlCnt].arg1 = (void *)hndlCnt;

            /* Start a new Task */
            task[hndlCnt] = TaskP_create(&DofApp_test,
                                         &taskParams[hndlCnt]);
            if (NULL == task[hndlCnt])
            {
                Vhwa_print(" Task%d create failed... \n", hndlCnt);
                OS_stop();
            }
#endif
        }

        /* Wait for all tasks to get completed */
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
        Vhwa_print("\n Test %d - %s is Disabled \n", testCnt, tObj->testName);
    }
}

static void DofApp_test(void *a0, void *a1)
{
    int32_t                 status = FVID2_SOK;
    uint32_t                hndlIdx = (uint32_t)a1;
    uint32_t                repCnt, pyrCnt;
    uint64_t                timeCount = 0;
    uint64_t                refOffset, currOffset, tempOffset, sofOffset, outOffset;
    DofApp_TestParams       *tObj = (DofApp_TestParams *)a0;

    status = AppDof_Create(tObj, hndlIdx);
    if (FVID2_SOK != status)
    {
        Vhwa_print(" DOF_TEST_APP: Create Failed for %d\n", hndlIdx);
        status = FVID2_EFAIL;
    }

    if (FVID2_SOK == status)
    {
        status = AppDof_SetParams(tObj, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" DOF_TEST_APP: SetParams Failed \n");
            status = FVID2_EFAIL;
        }
    }

    if (FVID2_SOK == status)
    {
        status = AppDof_SetConfScoreParam(tObj, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print("DOF_TEST_APP: SetConfScoreParam Failed \n");
        }
    }

    if (FVID2_SOK == status)
    {
        /* Setup buffer addresses with offsets based on handle index to avoid buffer overlap */
        refOffset = hndlIdx * VHWA_DOF_TEST_IN_BUF_OFFSET;
        currOffset = hndlIdx * VHWA_DOF_TEST_IN_BUF_OFFSET;
        tempOffset = hndlIdx * VHWA_DOF_TEST_IN_BUF_OFFSET;
        sofOffset = hndlIdx * VHWA_DOF_TEST_IN_BUF_OFFSET;
        outOffset = hndlIdx * VHWA_DOF_TEST_OUT_BUF_OFFSET;

        tObj->buffAddr.interOddBuff = gInterOddBuf;
        tObj->buffAddr.interEvenBuff = gInterEvenBuf;

        tObj->buffAddr.refBuff = gRefSrcBuf + gRefBufFreeIdx + refOffset;
        tObj->buffAddr.currBuff = gCurrSrcBuf + gCurrBufFreeIdx + currOffset;
        tObj->buffAddr.tempBuff = gTempSrcBuf + gTempBufFreeIdx + tempOffset;
        tObj->buffAddr.sofBuff = gSofSrcBuf + gSofBufFreeIdx + sofOffset;
        tObj->buffAddr.outBuff = gOutBuf + gOutBufFreeIdx + outOffset;
        tObj->buffAddr.refBuffIdx = &gRefBufFreeIdx;
        tObj->buffAddr.currBuffIdx = &gCurrBufFreeIdx;
        tObj->buffAddr.tempBuffIdx = &gTempBufFreeIdx;
        tObj->buffAddr.sofBuffIdx = &gSofBufFreeIdx;
        tObj->buffAddr.outBuffIdx = &gOutBufFreeIdx;

        AppDof_AllocBuffers(tObj, hndlIdx, &tObj->buffAddr);

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
            AppDof_PrepareRequest(tObj, hndlIdx);

            for (pyrCnt = tObj->testCfg[hndlIdx]->tPyrLvl; pyrCnt > 0; pyrCnt--)
            {
                /* Submit Request*/
                status = AppDof_SubmitRequest(tObj, hndlIdx);

                if (FVID2_SOK == status)
                {
                    /* Wait for Request Completion */
                    status = AppDof_WaitForComplRequest(tObj, hndlIdx);
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
        }

        if (tObj->isPerformanceTest)
        {
            timeCount = Vhwa_getTimeUsec() - timeCount;
            Vhwa_print("Performance:\n\t FrameCount: %d: Time in uSec: %d\n",
                       tObj->repeatCnt, timeCount);
            Vhwa_print("\t MPix/s: %d.%d\n",
                       (uint32_t)(((uint64_t)tObj->testCfg[hndlIdx]->width * (uint64_t)tObj->testCfg[hndlIdx]->height * (uint64_t)tObj->repeatCnt) / timeCount),
                       (uint32_t)((((uint64_t)tObj->testCfg[hndlIdx]->width * (uint64_t)tObj->testCfg[hndlIdx]->height * (uint64_t)tObj->repeatCnt * (uint64_t)100) / timeCount) % 100));
        }
        if (FVID2_SOK == status)
        {
            Vhwa_print(" Completed HandleCnt %d RepeatCnt %d\n",
                       hndlIdx, repCnt);
        }
    }

    AppDof_Delete(tObj, hndlIdx);

    VhwaAl_SemaphoreP_post(waitForTaskCmpl[hndlIdx]);
}

static int32_t DofApp_init(void)
{
    int32_t                 status;
    uint32_t                instId;
    Udma_DrvHandle          drvHandle = &gDofAppUdmaDrvObj;
    Udma_InitPrms           udmaInitPrms;
    uint32_t                hndlCnt;

#if defined VHWA_PDK
    Board_initCfg boardCfg;
    Fvid2_InitPrms initPrmsFvid2;
    SemaphoreP_Params params[APP_DOF_MAX_HANDLES];
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

    status = AppDof_Init(drvHandle);

#if defined(VHWA_PDK)
    if (FVID2_SOK == status)
    {
        /* Initialize the UDMA channel for CRC */
        status = AppDof_CrcInit(drvHandle);
    }
#endif

    if (FVID2_SOK == status)
    {
        for (hndlCnt = 0U; hndlCnt < APP_DOF_MAX_HANDLES; hndlCnt++)
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

static void DofApp_deInit()
{
    int32_t status;
    uint32_t cnt;
    Udma_DrvHandle drvHandle = &gDofAppUdmaDrvObj;

    Vhwa_m2mDofDeInit();
#if defined VHWA_PDK
    AppDof_CrcDeinit(drvHandle);
#endif
    status = Udma_deinit(drvHandle);
    if (UDMA_SOK != status)
    {
        Vhwa_print("[Error] UDMA deinit failed!!\n");
    }

    for (cnt = 0U; cnt < APP_DOF_MAX_HANDLES; cnt++)
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
static void DofApp_mainTaskFuncWrapper(void *args)
{
    DofApp_mainTaskFunc(NULL, NULL);

    vTaskDelete(NULL);
}

static void DofApp_testTaskFuncWrapper(void *args)
{
    taskParams *params = (taskParams *)args;

    /* Call the original test function with the expected parameters */
    DofApp_test(params->tObj, (void *)params->handleIdx);

    /* Delete the task when done */
    vTaskDelete(NULL);
}
#endif