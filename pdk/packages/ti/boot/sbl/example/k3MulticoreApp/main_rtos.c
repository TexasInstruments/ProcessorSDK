/*
 *  Copyright (c) Texas Instruments Incorporated 2023
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
 *  \brief Main file for RTOS build
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/csl/soc.h>
#include <ti/board/board.h>
#include <ti/csl/csl_types.h>

/* UART Header files */
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#if defined (BUILD_MCU1_0)
#include <ti/drv/sciclient/sciserver_tirtos.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#if defined (BUILD_C7X)
/* Temp workaround to avoid assertion failure: A_stackSizeTooSmall : Task stack size must be >= 16KB.
  * until the Bug PDK-7605 is resolved */
#define APP_TSK_STACK_MAIN              (32U * 1024U)
#else
#define APP_TSK_STACK_MAIN              (16U * 1024U)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void taskFxn(void* a0, void* a1);
extern int SBLApp_main(void);

#if defined (BUILD_MCU1_0)
/**< Initialize SCI Server, to process RM/PM Requests by other cores */
void ampMulticore_setupSciServer(void *arg0, void *arg1);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(64)));

#if defined(BUILD_MCU1_0)
static uint8_t  gSciserverTestStack[APP_TSK_STACK_MAIN] __attribute__((aligned(64)));
void ampMulticore_initSciclient();
#endif

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
    taskParams.priority     = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    task = TaskP_create(&taskFxn, &taskParams);
    if(NULL == task)
    {
        OS_stop();
    }

    OS_start();    /* does not return */

    return(0);
}

static void taskFxn(void* a0, void* a1)
{  
#if defined(BUILD_MCU1_0)
    Board_initCfg           boardCfg;
    boardCfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG;
    TaskP_Handle sciserverInitTask;
    TaskP_Params sciserverInitTaskParams;
    /* Initialize SCI Client - It must be called before board init */
    ampMulticore_initSciclient();

    Board_init(boardCfg);

    /* Initialize SCI Client Server */
    TaskP_Params_init(&sciserverInitTaskParams);
    sciserverInitTaskParams.priority     = 6;
    sciserverInitTaskParams.stack        = gSciserverTestStack;
    sciserverInitTaskParams.stacksize    = sizeof (gSciserverTestStack);

    sciserverInitTask = TaskP_create(&ampMulticore_setupSciServer, &sciserverInitTaskParams);
    if(NULL == sciserverInitTask)
    {
        UART_printf("Task creation failed ..\n");
        OS_stop();
    }
#endif

    SBLApp_main();

    return;
}

#if defined (BUILD_MCU1_0)
void ampMulticore_setupSciServer(void *arg0, void *arg1)
{
    Sciserver_TirtosCfgPrms_t appPrms;
    int32_t ret = CSL_PASS;

    ret = Sciserver_tirtosInitPrms_Init(&appPrms);

    appPrms.taskPriority[SCISERVER_TASK_USER_LO] = 4;
    appPrms.taskPriority[SCISERVER_TASK_USER_HI] = 5;

    if (ret == CSL_PASS)
    {
        ret = Sciserver_tirtosInit(&appPrms);
    }

    if (ret != CSL_PASS)
    {
        UART_printf("Starting Sciserver..... FAILED\n");
    }

    return;
}

void ampMulticore_initSciclient()
{
    int32_t ret = CSL_PASS;
    Sciclient_ConfigPrms_t        config;

    /* Now reinitialize it as default parameter */
    ret = Sciclient_configPrmsInit(&config);
    if (ret != CSL_PASS)
    {
        UART_printf("Sciclient_configPrmsInit Failed\n");
    }

#if defined (BUILD_MCU1_0)
    if (ret == CSL_PASS)
    {
        ret = Sciclient_boardCfgParseHeader((uint8_t *)SCISERVER_COMMON_X509_HEADER_ADDR, &config.inPmPrms, &config.inRmPrms);
        if (ret != CSL_PASS)
        {
            UART_printf("Sciclient_boardCfgParseHeader Failed\n");
        }
    }
#endif

    if (ret == CSL_PASS)
    {
        ret = Sciclient_init(&config);
        if (ret != CSL_PASS)
        {
            UART_printf("Sciclient_init Failed\n");
        }
    }
}
#endif

#if defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif
