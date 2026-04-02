/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2024
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
 *  \file safety_checkers_app_csirx_main.c
 *
 *  \brief Main file for RTOS builds
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/osal/osal.h>
#include <ti/board/board.h>
#include <ti/drv/sciclient/sciclient.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#define SAFETY_CHECKERS_APP_CSIRX_TSK_STACK_MAIN         (10U * 1024U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

extern int SafetyCheckersApp_csirxMain(void);
static void taskFxn(void* a0, void* a1);
void SafetyCheckersApp_csirxWait(uint32_t wait_in_ms);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
static uint8_t gSafetyCheckersAppCsirxTskStackMain[SAFETY_CHECKERS_APP_CSIRX_TSK_STACK_MAIN];

/* Task handle */
TaskP_Handle gSafetyCheckersAppCsirxTask;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    TaskP_Params taskParams;

    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority = 2;
    taskParams.stack = gSafetyCheckersAppCsirxTskStackMain;
    taskParams.stacksize = sizeof(gSafetyCheckersAppCsirxTskStackMain);

    gSafetyCheckersAppCsirxTask = TaskP_create(&taskFxn, &taskParams);

    OS_start();    /* does not return */

    return(0);
}

static void taskFxn(void* a0, void* a1)
{
    int32_t retVal = CSL_PASS;
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UNLOCK_MMR |
	       BOARD_INIT_UART_STDIO;

    if (BOARD_SOK == Board_init(boardCfg))
    {
        Sciclient_init(NULL);


        retVal += Sciclient_pmSetModuleState(TISCI_DEV_CSI_PSILSS0,
                                             TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                             TISCI_MSG_FLAG_AOP,
                                             SCICLIENT_SERVICE_WAIT_FOREVER);
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_CSI_RX_IF0,
                                             TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                             TISCI_MSG_FLAG_AOP,
                                             SCICLIENT_SERVICE_WAIT_FOREVER);
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_CSI_RX_IF1,
                                             TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                             TISCI_MSG_FLAG_AOP,
                                             SCICLIENT_SERVICE_WAIT_FOREVER);
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_DPHY_RX0,
                                             TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                             TISCI_MSG_FLAG_AOP,
                                             SCICLIENT_SERVICE_WAIT_FOREVER);
        retVal += Sciclient_pmSetModuleState(TISCI_DEV_DPHY_RX1,
                                             TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                             TISCI_MSG_FLAG_AOP,
                                             SCICLIENT_SERVICE_WAIT_FOREVER);
        if (CSL_PASS == retVal)
        {
            SafetyCheckersApp_csirxMain();
        }
    }

    return;
}

void SafetyCheckersApp_csirxWait(uint32_t wait_in_ms)
{
    TaskP_sleep(wait_in_ms);
}
