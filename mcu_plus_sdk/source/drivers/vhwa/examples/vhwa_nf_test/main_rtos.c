
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
#include "ti/osal/osal.h"
#include "ti/osal/TaskP.h"

#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <include/vhwa_m2mNf.h>
#include <examples/include/vhwa_nf_api.h>
#include "vhwa_nf_cfg.h"
#if defined (LDRA_COVERAGE)
#include <examples/include/ldra_vhwa_coverage_main.h>
#endif 

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#define APP_TSK_STACK_MAIN              (64U * 1024U)

/* Set the task priority higher than the default priority (1) */
#define NF_APP_MAIN_TASKS_PRIORITY     (7U)

/* Set the task priority higher than the default priority (1) */
#define NF_APP_TEST_TASKS_PRIORITY     (5U)

/* Input Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be loaded in one shot */
#define VHWA_NF_TEST_IN_BUF_OFFSET     (8*1024*1024)

/* Output Offset between two test cases, used mainly for zebu/qt,
 * so that buffers can be saved in one shot */
#define VHWA_NF_TEST_OUT_BUF_OFFSET    (16*1024*1024)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

static VhwaAl_SemaphoreP_Struct waitForTaskCmpl[APP_NF_MAX_HANDLES];

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void NfApp_mainTaskFunc(void* a0, void* a1);
static void NfApp_test(void* a0, void* a1);
static int32_t NfApp_init(void);
static void NfApp_run(uint32_t testCnt);
static void NfApp_deInit(void);
static int32_t App_getNum(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
#if defined(SAFERTOS)
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN]
    __attribute__((aligned(APP_TSK_STACK_MAIN)));
static uint8_t  gAppTskStackTest[APP_NF_MAX_HANDLES][APP_TSK_STACK_MAIN]
    __attribute__((aligned(APP_TSK_STACK_MAIN)));
#else
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN]
    __attribute__((aligned(32)));
static uint8_t  gAppTskStackTest[APP_NF_MAX_HANDLES][APP_TSK_STACK_MAIN]
    __attribute__((aligned(32)));
#endif

/*
* Application Buffers
*/
static uint64_t gNfTestSrcBuf =
                        (uint64_t)(VHWA_EXAMPLE_BUFF_START_ADDR);
static uint64_t gNfTestDestBuf =
                        (uint64_t)(VHWA_EXAMPLE_BUFF_START_ADDR + 0x10000000u);
static uint32_t gNfTestSrcBufFreeIdx = 0u;
static uint32_t gNfTestDstBufFreeIdx = 0u;

static AppNf_TestConfig gAppNfTestCfg[] =
{
    #include <examples/include/vhwa_nf_test_cfg.h>
};

static Nf_WgtTableConfig gWgtTbl = {
    NF_FILTER_MODE_BILATERAL,
    {
        #include <examples/vhwa_nf_test/coeff.txt>
    },
    {
        0x0
    }
};

static NfApp_TestParams gAppNfObj[] = VHWA_NF_TIRTOS_CFG;

static VhwaAl_Udma_DrvObj_Struct gNfAppUdmaDrvObj;

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

    taskParams.priority     = NF_APP_MAIN_TASKS_PRIORITY;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    task = TaskP_create(&NfApp_mainTaskFunc, &taskParams);
    if(NULL == task)
    {
        OS_stop();
    }

    OS_start();    /* does not return */

    return(0);
}

static void NfApp_mainTaskFunc(void* a0, void* a1)
{
    int32_t             status;
    bool                done = UFALSE;
    int32_t             testIdx;

    status = NfApp_init();

    Vhwa_print("\n Starting NF Test ..\n");
    #if defined (LDRA_COVERAGE)
    Vhwa_print("################### DYNAMIC CODE COVERAGE INITIALIZATION STARTED ######################\n");
    ldra_initialize();
    Vhwa_print("################### DYNAMIC CODE COVERAGE INITIALIZATION FINISHED ######################\n");
    #endif 

    App_startTimer();

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
                    if((uint32_t)testIdx < (sizeof(gAppNfObj)/ sizeof(NfApp_TestParams)))
                    {
                        NfApp_run(testIdx);
                    }
                    else
                    {
                        Vhwa_print("Invalid Test ID. Enter Again!!\n");
                    }
                    break;
            }
        }
    }

    NfApp_deInit();

    #if defined (LDRA_COVERAGE)
    Vhwa_print("################### DYNAMIC CODE COVERAGE TERMINATION STARTED ######################\n");
    ldra_terminate();
    Vhwa_print("################### DYNAMIC CODE COVERAGE TERMINATION FINISHED ######################\n");
    #endif 

    Vhwa_print (" Exiting \n");

    return;
}

static void NfApp_run(uint32_t testCnt)
{
    uint32_t            hndlCnt;
    TaskP_Handle        task[APP_NF_MAX_HANDLES];
    TaskP_Params        taskParams[APP_NF_MAX_HANDLES];
    NfApp_TestParams    *tObj = NULL;

    tObj = &gAppNfObj[testCnt];

    gNfTestSrcBufFreeIdx = 0U;
    gNfTestDstBufFreeIdx = 0U;

    if (UTRUE == tObj->isEnableTest)
    {
        Vhwa_print ("\n Starting Test %s\n", tObj->testName);

        for (hndlCnt = 0U; hndlCnt < tObj->numHandles;
                hndlCnt ++)
        {
            /* Initialize the task params */
            TaskP_Params_init(&taskParams[hndlCnt]);

            /* Set the task priority higher than the
                default priority (1) */
            taskParams[hndlCnt].priority  = NF_APP_TEST_TASKS_PRIORITY;
            taskParams[hndlCnt].stack     = gAppTskStackTest[hndlCnt];
            taskParams[hndlCnt].stacksize = sizeof (gAppTskStackTest[hndlCnt]);
            taskParams[hndlCnt].arg0      = (void*)tObj;
            taskParams[hndlCnt].arg1      = (void*)hndlCnt;

            /* Start a new Task */
            task[hndlCnt] = TaskP_create(&NfApp_test,
                                            &taskParams[hndlCnt]);
            if(NULL == task[hndlCnt])
            {
                Vhwa_print (" Task%d create failed... \n", hndlCnt);
                OS_stop();
            }
        }

        /* Wait for all tesks to get completed */
        for (hndlCnt = 0U; hndlCnt < tObj->numHandles; hndlCnt ++)
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
        Vhwa_print ("\n Test %d - %s is Disabled \n", testCnt, tObj->testName);
    }
    
    return;
}

static void NfApp_test(void* a0, void* a1)
{
    int32_t                 status = FVID2_SOK;
    uint32_t                hndlIdx = (uint32_t )a1;
    uint32_t                repCnt;
    uint32_t                inFrameSize;
    uint32_t                outFrameSize;
    uint64_t                timeCount;
    uint64_t                perf;
    NfApp_TestParams      *tObj = (NfApp_TestParams *)a0;

    status = AppNf_Create(tObj, hndlIdx);
    if (FVID2_SOK != status)
    {
        Vhwa_print(" NF_TEST_APP: Create Failed for %d\n", hndlIdx);
        status = FVID2_EFAIL;
    }

    if (FVID2_SOK == status)
    {
        status = AppNf_SetParams(tObj, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print(" NF_TEST_APP: SetParams Failed \n");
            status = FVID2_EFAIL;
        }
    }

    if(FVID2_SOK == status)
    {
        AppNf_SetCoeff(&gWgtTbl, hndlIdx);
        if (FVID2_SOK != status)
        {
            Vhwa_print("NF_TEST_APP: SetConfScoreParam Failed \n");
        }
    }

    if (FVID2_SOK == status)
    {
        AppNf_AllocBuffers(tObj, hndlIdx,
            gNfTestSrcBuf+gNfTestSrcBufFreeIdx, &inFrameSize,
            gNfTestDestBuf+gNfTestDstBufFreeIdx, &outFrameSize);

        /* Move Buffer Index */
        gNfTestSrcBufFreeIdx += inFrameSize;
        gNfTestDstBufFreeIdx += outFrameSize;
    }

    AppNf_PrepareRequest(tObj, hndlIdx);

    Vhwa_print(" Load buffers and press any key to continue\n");
    UART_getc();

    if(tObj->isPerformanceTest)
    {
        timeCount = TimerP_getTimeInUsecs();
    }

    if (FVID2_SOK == status)
    {
        for (repCnt = 0u; (repCnt < tObj->repeatCnt) &&
                (FVID2_SOK == status); repCnt ++)
        {

            /* Submit Request*/
            status = AppNf_SubmitRequest(tObj, hndlIdx);

            if (FVID2_SOK == status)
            {
                /* Wait for Request Completion */
                status = AppNf_WaitForComplRequest(tObj, hndlIdx);
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

        if(tObj->isPerformanceTest)
        {
            timeCount = TimerP_getTimeInUsecs() - timeCount;
            Vhwa_print ("Performance:\n\t FrameCount: %d: Time in uSec: %d\n",
                        tObj->repeatCnt, timeCount);

            perf = (uint64_t)tObj->testCfg[hndlIdx]->inWidth
                   *(uint64_t)tObj->testCfg[hndlIdx]->inHeight
                   *(uint64_t)tObj->repeatCnt;
            if(FVID2_DF_YUV420SP_UV == tObj->testCfg[hndlIdx]->dataFormat)
            {
                perf = perf*3U/2U;
            }
            Vhwa_print ("\t MPix/s: %d.%d\n",
                (uint32_t)(perf/timeCount),
                 (uint32_t)(((perf*(uint64_t)100)/timeCount)%100));
        }

        if (FVID2_SOK == status)
        {
            Vhwa_print (" Completed HandleCnt %d RepeatCnt %d\n",
                    hndlIdx, repCnt);
        }
    }

    AppNf_Delete(tObj, hndlIdx);

    VhwaAl_SemaphoreP_post(waitForTaskCmpl[hndlIdx]);
}

static int32_t NfApp_init(void)
{
    int32_t                 status;
    uint32_t                instId;
    uint32_t                hndlCnt;
    Udma_DrvHandle          drvHandle = &gNfAppUdmaDrvObj;
    Udma_InitPrms           udmaInitPrms;
    Board_initCfg           boardCfg;
    SemaphoreP_Params       params[APP_NF_MAX_HANDLES];
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
        if(UDMA_SOK != status)
        {
            Vhwa_print("[Error] UDMA init failed!!\n");
            status = FVID2_EFAIL;
        }
    }

    status = AppNf_Init(drvHandle);
    if (FVID2_SOK == status)
    {
        /* Initialize the UDMA channel for CRC */
        status = AppNf_CrcInit(drvHandle);
    }

    if (FVID2_SOK == status)
    {
        for (hndlCnt = 0U; hndlCnt < APP_NF_MAX_HANDLES; hndlCnt ++)
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
    }

    return (status);
}

static void NfApp_deInit()
{
    int32_t         status;
    uint32_t        cnt;
    Udma_DrvHandle  drvHandle = &gNfAppUdmaDrvObj;

    Vhwa_m2mNfDeInit();

    AppNf_CrcDeinit(drvHandle);

    status = Udma_deinit(drvHandle);
    if(UDMA_SOK != status)
    {
        Vhwa_print("[Error] UDMA deinit failed!!\n");
    }

    for (cnt = 0U; cnt < APP_NF_MAX_HANDLES; cnt ++)
    {
        if (NULL != waitForTaskCmpl[cnt])
        {
            SemaphoreP_delete(waitForTaskCmpl[cnt]);
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


