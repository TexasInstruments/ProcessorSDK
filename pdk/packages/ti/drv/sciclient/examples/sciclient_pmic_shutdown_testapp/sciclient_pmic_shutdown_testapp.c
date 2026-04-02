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
 * \file  sciclient_pmic_shutdown_testapp.c
 *
 * \brief Calls the PMIC shutdown sequence
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/board/board.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/sciclient/examples/common/sci_app_common.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#define SCICLIENT_APP_TSK_STACK_MAIN (16U * 1024U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
/* For SafeRTOS on R5F with FFI Support, task stack should be aligned to the stack size */
#if defined(SAFERTOS) && defined (BUILD_MCU)
static uint8_t  gSciclientAppTskStackMain[SCICLIENT_APP_TSK_STACK_MAIN] __attribute__((aligned(SCICLIENT_APP_TSK_STACK_MAIN))) = { 0 };
#else
static uint8_t  gSciclientAppTskStackMain[SCICLIENT_APP_TSK_STACK_MAIN] __attribute__((aligned(64)));
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Function Declarations                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Internal Function Declarations                     */
/* ========================================================================== */

static void SciclientApp_taskFxn(void* a0, void* a1);
static void SciclientApp_pmicPowerOff(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    TaskP_Handle task;
    TaskP_Params taskParams;

    /* This should be called before any other OS calls (like Task creation, OS_start, etc..) */
    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority     = 2;
    taskParams.stack        = gSciclientAppTskStackMain;
    taskParams.stacksize    = sizeof (gSciclientAppTskStackMain);

    task = TaskP_create(&SciclientApp_taskFxn, &taskParams);
    if(NULL == task)
    {
        OS_stop();
    }

    OS_start();    /* does not return */

    return(0);
}

/* ========================================================================== */
/*                          Internal Function Definitions                     */
/* ========================================================================== */

static void SciclientApp_taskFxn(void* a0, void* a1)
{
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    SciclientApp_pmicPowerOff();

    return;
}

static void SciclientApp_pmicPowerOff(void)
{
    SciApp_printf("\n#### Start of PMIC Poweroff Test ####\n");
    int32_t ret = CSL_PASS;

    /* Set PMIC Shutdown Function Pointer */
    ret = Sciclient_setPmicShutdownCb(SciApp_pmicShutdown);

    /* Poweroff */
    if (ret == CSL_PASS)
    {
        ret = Sciclient_pmSetModuleState(TISCI_DEV_BOARD0,
                                         TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                         TISCI_MSG_FLAG_AOP,
                                         SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    /* Incase of Success, following code should not be excuted */
    SciApp_printf("ERROR!!: Device is still powered ON\n");
    
    #if defined LDRA_DYN_COVERAGE_EXIT
    UART_printf("\n LDRA Entry... \n");
    upload_execution_history();
    UART_printf("\n LDRA Exit... \n");
    #endif
   
    if(ret != CSL_PASS)
    {
        SciApp_printf("Sciclient function call to shutdown PMIC is failed\n");
    }
    else
    {
        SciApp_printf(" Sciclient function call is passed but failed to poweroff the PMIC\n");
    }

    SciApp_printf("Testcase is Failed!!!");

}

