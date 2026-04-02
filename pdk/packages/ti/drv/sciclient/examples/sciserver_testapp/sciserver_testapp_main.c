/*
 *  Copyright (C) 2024-2025 Texas Instruments Incorporated
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
 *
 */

/**
 *  \file  sciserver_testapp_main.c
 *
 *  \brief Example Application for sciserver
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/board/board.h>
#include <ti/drv/sciclient/sciserver_tirtos.h>
#include <ti/drv/sciclient/examples/common/sci_app_common.h>
#include <ti/drv/sciclient/src/sciclient/sciclient_trace_internal.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define APP_TSK_STACK_MAIN                  (32U * 1024U)
#define APP_SCISERVER_INIT_TSK_STACK        (32U * 1024U)

#define SCISERVER_INIT_TASK_PRI             (4)
#define SCISERVER_SETUP_TASK_PRI_HIGH       (3)
#define SCISERVER_SETUP_TASK_PRI_LOW        (2)
#if defined (ENABLE_DM_TRACE)
/*
 * Low priority for DM trace task - must be least than all the tasks to print trace logs.
 * This is perferred because whenever DM receives a service request
 * that request should be excuted first.
 */
#define SCISERVER_DM_TRACE_TASK_PRI      (1)
#endif

#if defined (ENABLE_DM_TRACE)
/* DM_TRACE_LOG_BUF_SIZE is of size 8MB
 * which is the size of tracelog_dm to store trace logs
 */
#define DM_TRACE_LOG_BUF_SIZE              (8U*1024U*1024U)
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined (ENABLE_DM_TRACE)
char tracelog_dm[DM_TRACE_LOG_BUF_SIZE] __attribute__((__section__(".bss.DM_TRACE_LOG_BUFFER"))) = { '\0' };
extern uint32_t gDMTraceBufIndex;
extern uint32_t gDMTraceBufCount;
TaskP_Handle gTrace_task;
TaskP_Params gTrace_taskParams;
static uint8_t  gTrace_TskStackMain[APP_TSK_STACK_MAIN];
#endif

/* Test application stack */
/* For SafeRTOS on R5F with FFI Support, task stack should be aligned to the stack size */
#if defined(SAFERTOS)
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN]
__attribute__ ((aligned(APP_TSK_STACK_MAIN)));
#else
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN]
__attribute__ ((aligned(8192)));
#endif

/* ========================================================================== */
/*                            Structure Declarations                          */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Function Declarations                           */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Internal Function Declarations                  */
/* ========================================================================== */

static void taskFxn(void* a0, void* a1);
#if defined (ENABLE_DM_TRACE)
/* Prints the logs stored in tracelog_dm buffer when DM is waiting for message request */
static void Sciclient_dmTrace(void* a0, void* a1);
#endif

/* ========================================================================== */
/*                            Function Definitions                            */
/* ========================================================================== */

int main(void)
{
    TaskP_Handle task;
    TaskP_Params taskParams;

    /*  This should be called before any other OS calls (like Task creation, OS_start, etc..) */
    OS_init();

    #if defined (ENABLE_DM_TRACE)
    /* Giving access  of user defined tracelog_dm buffer to debug application */
    Sciclient_setTracelogBuffer(tracelog_dm, DM_TRACE_LOG_BUF_SIZE);
    #endif

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority     = SCISERVER_INIT_TASK_PRI;
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

/* ========================================================================== */
/*                 Internal Function Definitions                              */
/* ========================================================================== */

static void taskFxn(void* a0, void* a1)
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
#if defined (ENABLE_DM_TRACE)
    /* Initialize the task params */
    TaskP_Params_init(&gTrace_taskParams);
    gTrace_taskParams.priority     = SCISERVER_DM_TRACE_TASK_PRI;
    gTrace_taskParams.stack        = gTrace_TskStackMain;
    gTrace_taskParams.stacksize    = sizeof (gTrace_TskStackMain);
    gTrace_task = TaskP_create(&Sciclient_dmTrace, &gTrace_taskParams);
#endif
}

#if defined (ENABLE_DM_TRACE)
static void Sciclient_dmTrace(void* a0, void* a1)
{
    uint32_t index;
    uint32_t count = 0;
    SciApp_printf("--- Start of DM trace ----\n");

    /* Loop through tracelog_dm buffer to print the DM trace logs */
    for (index = 0; index <= DM_TRACE_LOG_BUF_SIZE; index++)
    {
        /* Check if we have reached the end of the buffer
         * and reset index to start while incrementing count
         * so that we can print all the logs without missing */
        if(DM_TRACE_LOG_BUF_SIZE == index)
        {
            index = 0;
            count++;
        }

        /* Wait till gDMTraceBufIndex or gDMTraceBufCount is updated */
        while(index == gDMTraceBufIndex && count == gDMTraceBufCount)
        {
            Osal_delay(1);
        }
        SciApp_printf("%c", tracelog_dm[index]);
    }

     return;
}
#endif
