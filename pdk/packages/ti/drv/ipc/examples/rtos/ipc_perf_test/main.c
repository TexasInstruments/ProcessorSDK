/*
 *  Copyright (c) Texas Instruments Incorporated 2019
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
 *  \file main_tirtos.c
 *
 *  \brief Main file for TI-RTOS build
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>

/* OSAL Header files */
#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/board/board.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include "ipc_apputils.h"

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J72IS2) || defined (SOC_J784S4)))
#include <ti/drv/sciclient/sciserver_tirtos.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#ifdef BUILD_C7X
/* Test application stack size */
#define APP_TSK_STACK_MAIN              (32U * 1024U)
#else
/* Test application stack size */
#define APP_TSK_STACK_MAIN              (16U * 1024U)
#endif
/* Priority for IPC task */
#define IPC_SETUP_TASK_PRI                  (3)
/* High Priority for SCI Server */
#define IPC_SETUP_SCISERVER_TASK_PRI_HIGH   (IPC_SETUP_TASK_PRI + 1)
/* Low Priority for SCI Server */
#define IPC_SETUP_SCISERVER_TASK_PRI_LOW    (1)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void taskFxn(void* a0, void* a1);
extern int32_t Ipc_perf_test(void);
extern void ipc_cacheMarInit(void);
/* Initialize SCI Server, to process RM/PM Requests by other cores */
void Ipc_setupSciServer(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] 
__attribute__ ((aligned(8192)));

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


void ipc_initSciclient()
{
    int32_t ret = CSL_PASS;
    Sciclient_ConfigPrms_t        config;

    /* Now reinitialize it as default parameter */
    ret = Sciclient_configPrmsInit(&config);

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J72IS2) || defined (SOC_J784S4)))
    if (ret == CSL_PASS)
    {
        ret = Sciclient_boardCfgParseHeader(
            (uint8_t *)SCISERVER_COMMON_X509_HEADER_ADDR,
            &config.inPmPrms, &config.inRmPrms);
    }
    else
    {
        UART_printf("Sciclient_configPrmsInit Failed\n");
    }
#endif

    if (ret == CSL_PASS)
    {
        Sciclient_init(&config);
    }
#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J72IS2) || defined (SOC_J784S4)))
    else
    {
        UART_printf("Sciclient_boardCfgParseHeader Failed\n");
    }
#endif
}

int main(void)
{
    TaskP_Handle      task;
    TaskP_Params      taskParams;
    Board_initCfg     boardCfg;

    /* It must be called before BoardInit() */
    ipc_initSciclient();

    /* Initialize SCI Client Server */
    Ipc_setupSciServer();

#if defined (_TMS320C6X) && defined FREERTOS
    ipc_cacheMarInit();
#endif

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);
    
    /*  This should be called before any other OS calls (like Task creation, OS_start, etc..) */
    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);
    
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority     = IPC_SETUP_TASK_PRI;
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
    Ipc_perf_test();
    return;
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

void Ipc_setupSciServer(void)
{

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J72IS2) || defined (SOC_J784S4)))
    Sciserver_TirtosCfgPrms_t appPrms;
    int32_t ret = CSL_PASS;

    ret = Sciserver_tirtosInitPrms_Init(&appPrms);

    appPrms.taskPriority[SCISERVER_TASK_USER_LO] =
                                            IPC_SETUP_SCISERVER_TASK_PRI_LOW;
    appPrms.taskPriority[SCISERVER_TASK_USER_HI] =
                                            IPC_SETUP_SCISERVER_TASK_PRI_HIGH;

    if (ret == CSL_PASS)
    {
        ret = Sciserver_tirtosInit(&appPrms);
    }

    if (ret == CSL_PASS)
    {
        UART_printf("Starting Sciserver..... PASSED\n");
    }
    else
    {
        UART_printf("Starting Sciserver..... FAILED\n");
    }

#endif
    return;
}
