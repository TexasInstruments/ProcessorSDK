/*
 *  Copyright (c) Texas Instruments Incorporated 2024
 *  All rights reserved.
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
 *  \file ipc_graceful_shutdown_main.c
 *
 *  \brief Main file for IPC graceful shutdown test application
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/board/board.h>
#if defined (BUILD_MCU1_0)
#include <ti/drv/sciclient/src/sciclient/sciclient_priv.h>
#include <ti/drv/sciclient/sciserver_tirtos.h>
#endif
#include <ti/drv/ipc/examples/common/src/ipc_setup.h>
#include <ti/drv/ipc/examples/rtos/ipc_graceful_shutdown_test/ipc_graceful_shutdown_setup.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#define IPC_APP_TSK_STACK_MAIN                  (32U * 1024U)

/* High Priority for SCI Server - must be higher than low priority task */
#define IPC_APP_SETUP_SCISERVER_TASK_PRI_HIGH   (5U)

/* Low Priority for SCI Server - must be higher than IPC tasks
 * to prevent delay in handling Sciserver requests
 */
#define IPC_APP_SETUP_SCISERVER_TASK_PRI_LOW    (4U)

#if defined (BUILD_MCU1_0)
/* SCI Server Init Task stack size */
#define IPC_APP_SCISERVER_INIT_TSK_STACK        (32U * 1024U)
/* SCI Server Init Task Priority - must be higher than High priority Sciserver task */
#define IPC_APP_INIT_SCISERVER_TASK_PRI         (6U)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void IpcApp_boardInit(void);
static void IpcApp_initSciclient(void);
static void IpcApp_taskFxn(void* a0, void* a1);

#if defined (BUILD_MCU1_0)
/* Initialize SCI Server, to process RM/PM Requests by other cores */
static void IpcApp_setupSciServer(void *arg0, void *arg1);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
static uint8_t  gIpcApp_TskStackMain[IPC_APP_TSK_STACK_MAIN] __attribute__ ((aligned(8192)));
#if defined (BUILD_MCU1_0)
/* Sciserver init task stack */
static uint8_t  gIpcApp_SciserverInitTskStack[IPC_APP_SCISERVER_INIT_TSK_STACK] __attribute__ ((aligned(8192)));
extern Sciclient_ServiceHandle_t gSciclientHandle;
#endif

/* Variable to check if gBoardInit has completed or not*/
uint32_t  gBoardinit = 0U;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    TaskP_Handle task;
    TaskP_Params taskParams;

    /*  This should be called before any other OS calls (like Task creation, OS_start, etc..) */
    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority  = 2;
    taskParams.stack     = gIpcApp_TskStackMain;
    taskParams.stacksize = sizeof (gIpcApp_TskStackMain);

    task = TaskP_create(&IpcApp_taskFxn, &taskParams);
    if(NULL == task)
    {
        OS_stop();
    }
    OS_start();    /* Does not return */

    return(0);
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

static void IpcApp_initSciclient(void)
{
    int32_t ret = CSL_PASS;
    Sciclient_ConfigPrms_t config;

    ret = Sciclient_configPrmsInit(&config);
    if (CSL_PASS != ret)
    {
        App_printf("Sciclient_configPrmsInit Failed\n");
    }

#if defined (BUILD_MCU1_0)
    if (CSL_PASS == ret)
    {
        ret = Sciclient_boardCfgParseHeader((uint8_t *)SCISERVER_COMMON_X509_HEADER_ADDR,
                                            &config.inPmPrms,
                                            &config.inRmPrms);
        if (CSL_PASS != ret)
        {
            App_printf("Sciclient_boardCfgParseHeader Failed\n");
        }
    }
#endif

    if (CSL_PASS == ret)
    {
        ret = Sciclient_init(&config);
        if (CSL_PASS != ret)
        {
            App_printf("Sciclient_init Failed\n");
        }
#if defined (BUILD_MCU1_0)
        if (gSciclientHandle.rmBoardConfigComplete == SCICLIENT_FT_PASS)
        {
            App_printf("Sciclient_boardCfgRm init Passed\n");
        }
        else
        {
            App_printf("Sciclient_boardCfgRm init FAILED!\n");
        }
#endif
    }
}

static void IpcApp_boardInit(void)
{
    Board_initCfg  boardCfg;

    boardCfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG;

    Board_init(boardCfg);

    /* Mark Board_init() has been called */
    gBoardinit = 1;
}

static void IpcApp_taskFxn(void* a0, void* a1)
{
    /* Initialize SCI Client - It must be called before board init */
    IpcApp_initSciclient();

    IpcApp_boardInit();

#if defined (BUILD_MCU1_0)
    TaskP_Handle sciserverInitTask;
    TaskP_Params sciserverInitTaskParams;

    /* Initialize SCI Client Server */
    TaskP_Params_init(&sciserverInitTaskParams);
    sciserverInitTaskParams.priority  = IPC_APP_INIT_SCISERVER_TASK_PRI;
    sciserverInitTaskParams.stack     = gIpcApp_SciserverInitTskStack;
    sciserverInitTaskParams.stacksize = sizeof (gIpcApp_SciserverInitTskStack);

    sciserverInitTask = TaskP_create(&IpcApp_setupSciServer, &sciserverInitTaskParams);
    if(NULL == sciserverInitTask)
    {
        OS_stop();
    }
#endif

    IpcApp_gracefulShutdownTest();

}

#if defined (BUILD_MCU1_0)
static void IpcApp_setupSciServer(void *arg0, void *arg1)
{
    Sciserver_TirtosCfgPrms_t appPrms;
    int32_t ret = CSL_PASS;
    char *sci_version  = NULL;
    char *rmpm_version = NULL;

    ret = Sciserver_tirtosInitPrms_Init(&appPrms);

    appPrms.taskPriority[SCISERVER_TASK_USER_LO] = IPC_APP_SETUP_SCISERVER_TASK_PRI_LOW;
    appPrms.taskPriority[SCISERVER_TASK_USER_HI] = IPC_APP_SETUP_SCISERVER_TASK_PRI_HIGH;

    if (CSL_PASS == ret)
    {
        ret = Sciserver_tirtosInit(&appPrms);
    }

    sci_version = Sciserver_getVersionStr();
    rmpm_version = Sciserver_getRmPmHalVersionStr();
    App_printf("DM Built On: %s %s\n", __DATE__, __TIME__);
    App_printf("Sciserver Version: %s\n", sci_version);
    App_printf("RM_PM_HAL Version: %s\n", rmpm_version);

    if (CSL_PASS == ret)
    {
        App_printf("Starting Sciserver..... PASSED\n");
    }
    else
    {
        App_printf("Starting Sciserver..... FAILED\n");
    }

    return;
}
#endif
