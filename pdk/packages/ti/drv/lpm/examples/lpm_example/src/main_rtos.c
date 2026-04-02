/*
*
* Copyright (c) 2021 Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

/**
 *  \file main_rtos.c
 *
 *  \brief Main file for TI-RTOS build
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/soc.h>
#include <ti/csl/cslr.h>
#include <ti/board/board.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

#include <ti/drv/sciclient/sciserver_tirtos.h>

#include <ti/drv/lpm/lpm.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#define APP_TASK_STACK                  (10U * 1024U)
/**< Stack required for the stack */
#define MAIN_APP_TASK_PRIORITY          (2)
/**< Task Priority Levels */

#define APP_SCISERVER_INIT_TSK_STACK    (32U * 1024U)

#define LPM_INIT_SCISERVER_TASK_PRI     (8)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void MainApp_TaskFxn(void* a0, void* a1);
static void SetupSciServer(void* a0, void* a1);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

TaskP_Handle mainAppTask;
TaskP_Params mainAppTaskParams;
static uint8_t MainApp_TaskStack[APP_TASK_STACK] __attribute__((aligned(32)));
/**< Stack for the Main task */

static uint8_t  gSciserverInitTskStack[APP_SCISERVER_INIT_TSK_STACK]
__attribute__ ((aligned(8192)));

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int main(void)
{
    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&mainAppTaskParams);
    mainAppTaskParams.priority       = MAIN_APP_TASK_PRIORITY;
    mainAppTaskParams.stack          = MainApp_TaskStack;
    mainAppTaskParams.stacksize      = sizeof (MainApp_TaskStack);

    mainAppTask = TaskP_create(&MainApp_TaskFxn, &mainAppTaskParams);
    if (NULL == mainAppTask)
    {
        OS_stop();
    }

    OS_start();    /* does not return */

    return(0);
}

static void MainApp_TaskFxn(void* a0, void* a1)
{
#if defined(UART_PRINT_DEBUG)
    uint64_t timeIPCStart, timeIPCFinish;
    uint64_t timeBootAppStart, timeBootAppFinish;
    uint64_t timeMcuOnlyAppStart, timeMcuOnlyAppFinish;
#endif
    uint32_t i, numBoots=5;
    int32_t ret = CSL_PASS;

    Board_init(BOARD_INIT_UART_STDIO);

    Sciclient_init(NULL_PTR);

    TaskP_Handle sciserverInitTask;
    TaskP_Params sciserverInitTaskParams;

    /* Initialize SCI Client Server */
    TaskP_Params_init(&sciserverInitTaskParams);
    sciserverInitTaskParams.priority     = LPM_INIT_SCISERVER_TASK_PRI;
    sciserverInitTaskParams.stack        = gSciserverInitTskStack;
    sciserverInitTaskParams.stacksize    = sizeof (gSciserverInitTskStack);

    sciserverInitTask = TaskP_create(&SetupSciServer, &sciserverInitTaskParams);
    if(NULL == sciserverInitTask)
    {
        OS_stop();
    }

    if(ret != CSL_PASS)
    {
        OS_stop();
    }

    AppUtils_Printf(MSG_NORMAL, "\nMCU R5F App started at %d usecs\r\n", (uint32_t)TimerP_getTimeInUsecs());

    Lpm_bootAppInit();
    Lpm_pmicInit();
    AppUtils_Printf(MSG_NORMAL, "\nPMIC initialization done.\r\n");

    for(i=0; i<numBoots; i++)
    {
#if defined(UART_PRINT_DEBUG)
        AppUtils_Printf(MSG_NORMAL, "\nLoop %d starts!\n",i);
        timeIPCStart = TimerP_getTimeInUsecs();
#endif

        Lpm_ipcEchoApp();

#if defined(UART_PRINT_DEBUG)
        timeIPCFinish = TimerP_getTimeInUsecs();
        AppUtils_Printf(MSG_NORMAL, "\nIPC Task started at %d usecs and finished at %d usecs\r\n",
                        (uint32_t)timeIPCStart,
                        (uint32_t)timeIPCFinish);
        AppUtils_Printf(MSG_NORMAL, "************* Sleeping for 1 sec after IPC Task ***********\n");
        TaskP_sleep(1000);
        timeBootAppStart = TimerP_getTimeInUsecs();
#endif
        
        Lpm_bootApp();
        
#if defined(UART_PRINT_DEBUG)
        timeBootAppFinish = TimerP_getTimeInUsecs();
        AppUtils_Printf(MSG_NORMAL, "\nBootApp Task started at %d usecs and finished at %d usecs\r\n",
                        (uint32_t)timeBootAppStart,
                        (uint32_t)timeBootAppFinish);
        AppUtils_Printf(MSG_NORMAL, "************* Sleeping for 1 sec after MCU Boot Task***********\n");
        TaskP_sleep(1000);
        AppUtils_Printf(MSG_NORMAL, "De-initializing the IPC driver and deleting the tasks\n");
#endif

        Lpm_ipcExitResponseTask();
        TaskP_sleep(1000);
        
#if defined(UART_PRINT_DEBUG)
        AppUtils_Printf(MSG_NORMAL, "************* Slept for 1 sec after IPC de-initializing Task. IPC should have deinitialized now. ***********\n");
        timeMcuOnlyAppStart = TimerP_getTimeInUsecs();
#endif

        Lpm_pmicApp();

#if defined(UART_PRINT_DEBUG)
        timeMcuOnlyAppFinish = TimerP_getTimeInUsecs();
        AppUtils_Printf(MSG_NORMAL, "\nMCU Only Task started at %d usecs and finished at %d usecs\r\n",
                        (uint32_t)timeMcuOnlyAppStart,
                        (uint32_t)timeMcuOnlyAppFinish);
        AppUtils_Printf(MSG_NORMAL, "\nLoop %d ends!\n",i);
#endif
    }

    Lpm_bootAppDeInit();

    return;
}

void SetupSciServer(void* a0, void* a1)
{

    Sciserver_TirtosCfgPrms_t appPrms;
    Sciclient_ConfigPrms_t clientPrms;
    int32_t ret = CSL_PASS;

    appPrms.taskPriority[SCISERVER_TASK_USER_LO] = 6;
    appPrms.taskPriority[SCISERVER_TASK_USER_HI] = 7;

    /* Sciclient needs to be initialized before Sciserver. Sciserver depends on
     * Sciclient API to execute message forwarding */
    ret = Sciclient_configPrmsInit(&clientPrms);

    if (ret == CSL_PASS)
    {
        ret = Sciclient_init(&clientPrms);
    }

    if (ret == CSL_PASS)
    {
        ret = Sciserver_tirtosInit(&appPrms);
    }

    if (ret == CSL_PASS)
    {
        AppUtils_Printf(MSG_NORMAL, "Starting Sciserver..... PASSED\n");
    }
    else
    {
        AppUtils_Printf(MSG_NORMAL, "Starting Sciserver..... FAILED\n");
    }

    return ;
}
