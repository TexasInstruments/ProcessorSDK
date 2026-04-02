/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 *  \file main_rtos.c
 *
 *  \brief Main file for RTOS builds
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/board/board.h>
#include <ti/drv/dss/examples/utils/app_utils.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/csl/soc.h>
#include "dss_m2m_test_cfg.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#define DISP_APP_TSK_STACK_MAIN         (10U * 1024U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void taskFxn(void* a0, void* a1);
extern int32_t Dss_displayM2MTest(void);
static void App_clkRateSet(uint32_t moduleId,
                           uint32_t clkId,
                           uint64_t clkRateHz);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
static uint8_t gDispAppTskStackMain[DISP_APP_TSK_STACK_MAIN];

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
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority = 2;
    taskParams.stack = gDispAppTskStackMain;
    taskParams.stacksize = sizeof(gDispAppTskStackMain);

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
    int32_t retVal = CSL_PASS;
    uint64_t clkFreq = 0U;
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
                BOARD_INIT_UNLOCK_MMR |
                BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    Sciclient_init(NULL);
    retVal += Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
    retVal += Sciclient_pmSetModuleState(TISCI_DEV_DSS_DSI0,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
    retVal += Sciclient_pmSetModuleState(TISCI_DEV_DSS_EDP0,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);

    /* set DSS VP4 clocks */
    App_clkRateSet(TISCI_DEV_DSS0,
                   TISCI_DEV_DSS0_DSS_INST0_DPI_3_IN_2X_CLK,
                   148500000U);
    /* Get the DSS VP4 clock frequencies */
    retVal += Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
                                        TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK,
                                        &clkFreq,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    App_consolePrintf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK = %lld Hz\n", clkFreq);
    retVal += Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
                                        TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK,
                                        &clkFreq,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);

    App_consolePrintf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK = %lld Hz\n", clkFreq);
    retVal += Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
                                        TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK,
                                        &clkFreq,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    App_consolePrintf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK = %lld Hz\n", clkFreq);
    retVal += Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
                                        TISCI_DEV_DSS0_DSS_INST0_DPI_3_IN_2X_CLK,
                                        &clkFreq,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    App_consolePrintf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_3_IN_2X_CLK = %lld Hz\n", clkFreq);
    if (CSL_PASS == retVal)
    {
        Dss_displayM2MTest();
    }

    return;
}


void App_wait(uint32_t wait_in_ms)
{
    TaskP_sleep(wait_in_ms);
}

static void App_clkRateSet(uint32_t moduleId,
                           uint32_t clkId,
                           uint64_t clkRateHz)
{
    int32_t status;
    uint64_t currClkFreqHz;

    status = Sciclient_pmGetModuleClkFreq(moduleId,
                                        clkId,
                                        &currClkFreqHz,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if ((CSL_PASS == status) &&
        (currClkFreqHz != clkRateHz))
    {
        status = Sciclient_pmSetModuleClkFreq(moduleId,
                                            clkId,
                                            clkRateHz,
                                            0,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);      
        if (CSL_PASS == status)
        {
            App_consolePrintf("\nSciclient_pmSetModuleClkFreq Passed for clock Id = %d\n", clkId);
        }
        else
        {
            App_consolePrintf("\nSciclient_pmSetModuleClkFreq failed for clock Id = %d\n", clkId);
        }
    }
}
