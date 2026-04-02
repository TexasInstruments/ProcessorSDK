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
 *  \file memcpy_tricore_main_rtos.c
 *
 *  \brief Main file for tricore RTOS build
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/csl/soc.h>
#include <ti/board/board.h>

#include <ti/drv/udma/examples/udma_apputils/udma_apputils.h>

#include <ti/drv/sciclient/sciserver_tirtos.h>
#include <ti/drv/sciclient/examples/common/sci_app_common.h>

#include <ti/csl/arch/r5/csl_arm_r5.h>

#if defined (j721e_evm)
#include <ti/board/src/j721e_evm/include/board_utils.h>
#elif defined (j784s4_evm)
#include <ti/board/src/j784s4_evm/include/board_utils.h>
#elif defined (j721s2_evm)
#include <ti/board/src/j721s2_evm/include/board_utils.h>
#elif defined (j742s2_evm)
#include <ti/board/src/j742s2_evm/include/board_utils.h>
#elif defined (j7200_evm)
#include <ti/board/src/j7200_evm/include/board_utils.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size and priority */
#define APP_TSK_STACK_MAIN                  (16U * 1024U)

#define APP_INIT_TASK_PRI                   (2)

/* Test application stack size and priority */
#define SCISERVER_TSK_STACK_MAIN            (32U * 1024U)

#define SCISERVER_INIT_TASK_PRI             (5)
#define SCISERVER_SETUP_TASK_PRI_HIGH       (4)
#define SCISERVER_SETUP_TASK_PRI_LOW        (3)

#if defined (j721e_evm)
#define UART_COM2_INST 1U
#elif defined (j784s4_evm)
#define UART_COM2_INST 5U
#elif defined (j721s2_evm)
#define UART_COM2_INST 5U
#elif defined (j7200_evm)
#define UART_COM2_INST 1U
#elif defined (j742s2_evm)
#define UART_COM2_INST 5U
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void appTaskFxn(void* a0, void* a1);
static void sciServerTaskFxn(void* a0, void* a1);
extern int32_t Udma_memcpyTest(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(0x2000)));

/* Sciserver application stack */
static uint8_t  gSciserverTskStackMain[SCISERVER_TSK_STACK_MAIN] __attribute__((aligned(0x2000)));

/* Core awareness */
CSL_ArmR5CPUInfo cpuInfo;

extern Board_initParams_t gBoardInitParams;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    /*  This should be called before any other OS calls (like Task creation, OS_start, etc..) */
    OS_init();

    CSL_armR5GetCpuID(&cpuInfo);

    /* Init Sciserver if core is MCU1_0 */
    if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_0 && cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
    {
        TaskP_Handle sciServerTask;
        TaskP_Params sciServerTaskParams;

        /* Initialize the sciServerTask params */
        TaskP_Params_init(&sciServerTaskParams);
        /* Set the sciServerTask priority higher than the default priority (1) */
        sciServerTaskParams.priority     = SCISERVER_INIT_TASK_PRI;
        sciServerTaskParams.stack        = gSciserverTskStackMain;
        sciServerTaskParams.stacksize    = sizeof (gSciserverTskStackMain);

        sciServerTask = TaskP_create(&sciServerTaskFxn, &sciServerTaskParams);
        if(NULL == sciServerTask)
        {
            OS_stop();
        }
    }

    TaskP_Handle appTask;
    TaskP_Params appTaskParams;
    
    /* Initialize the appTask params */
    TaskP_Params_init(&appTaskParams);
    /* Set the appTask priority higher than the default priority (1) */
    appTaskParams.priority     = APP_INIT_TASK_PRI;
    appTaskParams.stack        = gAppTskStackMain;
    appTaskParams.stacksize    = sizeof (gAppTskStackMain);

    appTask = TaskP_create(&appTaskFxn, &appTaskParams);
    if(NULL == appTask)
    {
        OS_stop();
    }
    OS_start();    /* does not return */

    return(0);
}

static void appTaskFxn(void* a0, void* a1)
{
    Board_initCfg           boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;

    CSL_ArmR5CPUInfo cpuInfo;
    CSL_armR5GetCpuID(&cpuInfo);
    if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_2)
    {
        /* uartInst is by default set to the UART instance of COM1 port. */
        gBoardInitParams.uartInst = UART_COM2_INST;
    }
               
    Board_init(boardCfg);
    
    Udma_memcpyTest();

    return;
}

static void sciServerTaskFxn(void* a0, void* a1)
{
    int32_t ret                = CSL_PASS;
    Sciclient_ConfigPrms_t clientPrms;
    Sciserver_TirtosCfgPrms_t appPrms;
    char    *versionStr        = NULL;
    char    *rmpmhalVersionStr = NULL;

    /* Sciclient needs to be initialized before Sciserver. Sciserver depends on
     * Sciclient API to execute message forwarding */
    ret = Sciclient_configPrmsInit(&clientPrms);

    if (ret == CSL_PASS)
    {
        ret = Sciclient_boardCfgParseHeader(
            (uint8_t *) SCISERVER_COMMON_X509_HEADER_ADDR,
            &clientPrms.inPmPrms, &clientPrms.inRmPrms);
    }

    if (ret == CSL_PASS)
    {
        ret = Sciclient_init(&clientPrms);
    }

    /* Enable UART console print*/
    if (ret == CSL_PASS)
    {
        SciApp_consoleInit();
    }

    if (ret == CSL_PASS)
    {
        ret = Sciserver_tirtosInitPrms_Init(&appPrms);
        appPrms.taskPriority[SCISERVER_TASK_USER_LO] = SCISERVER_SETUP_TASK_PRI_LOW;
        appPrms.taskPriority[SCISERVER_TASK_USER_HI] = SCISERVER_SETUP_TASK_PRI_HIGH;        
    }

    if (ret == CSL_PASS)
    {
        ret = Sciserver_tirtosInit(&appPrms);
    }

    /* Set PMIC Shutdown Function Pointer */
    if (ret == CSL_PASS)
    {
        ret = Sciclient_setPmicShutdownCb(SciApp_pmicShutdown);
    }

    if (ret == CSL_PASS)
    {
        versionStr = Sciserver_getVersionStr();
        rmpmhalVersionStr = Sciserver_getRmPmHalVersionStr();
        SciApp_printf("Sciserver Testapp Built On: %s %s\n", __DATE__, __TIME__);
        SciApp_printf("Sciserver Version: %s\n", versionStr);
        SciApp_printf("RM_PM_HAL Version: %s\n", rmpmhalVersionStr);
    }

    #if defined LDRA_DYN_COVERAGE_EXIT
    UART_printf("\n LDRA Entry... \n");
    /* Add 3 minutes delay before printing execution history to make sure extended ut running on 
       mcu2_0 will complete it's execution, this will introduce additional code coverage by 
       sending requests to sciserver.
     */
    Osal_delay(180*1000);
    upload_execution_history();
    UART_printf("\n LDRA Exit... \n");
    #endif

    if (ret == CSL_PASS)
    {
        SciApp_printf("Starting Sciserver..... PASSED\n");
    }
    else
    {
        SciApp_printf("Starting Sciserver..... FAILED\n");
    }
}

