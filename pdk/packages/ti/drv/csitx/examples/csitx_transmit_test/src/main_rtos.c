/*
 *  Copyright (c) Texas Instruments Incorporated 2020-2022
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
#include <stdint.h>
#include <stdio.h>

#include "csitx_transmit_test_cfg.h"
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/* Test application stack size */
#define TX_APP_TSK_STACK_MAIN         (16U * 1024U)

#define CSITX_ESC_CLK_FREQ_HZ         (20000000U)
#define CSITX_MAIN_CLK_HZ             (500000000U)

#define CSITX_PSIL_MOD                          TISCI_DEV_CSI_PSILSS0
#if defined (SOC_J721E)
#define CSITX_MOD                               TISCI_DEV_CSI_TX_IF0
#define CSITX_DPHY_MOD                          TISCI_DEV_DPHY_TX0
#define CSITX_MOD_ESC_CLK                       TISCI_DEV_CSI_TX_IF0_ESC_CLK_CLK
#define CSITX_MOD_MAIN_CLK                      TISCI_DEV_CSI_TX_IF0_MAIN_CLK_CLK
#define CSITX_MOD_DPHY_TXBYTECLKHS_CLK          TISCI_DEV_CSI_TX_IF0_DPHY_TXBYTECLKHS_CL_CLK
#define CSITX_MOD_VBUS_CLK                      TISCI_DEV_CSI_TX_IF0_VBUS_CLK_CLK
#endif

#if defined (SOC_J784S4) || defined(SOC_J742S2)
#if (CSITX_INSTANCE == 0U)
#define CSITX_MOD                               TISCI_DEV_CSI_TX_IF0
#define CSITX_DPHY_MOD                          TISCI_DEV_DPHY_TX0
#define CSITX_MOD_ESC_CLK                       TISCI_DEV_CSI_TX_IF0_ESC_CLK_CLK
#define CSITX_MOD_MAIN_CLK                      TISCI_DEV_CSI_TX_IF0_MAIN_CLK_CLK
#define CSITX_MOD_DPHY_TXBYTECLKHS_CLK          TISCI_DEV_CSI_TX_IF0_DPHY_TXBYTECLKHS_CL_CLK
#define CSITX_MOD_VBUS_CLK                      TISCI_DEV_CSI_TX_IF0_VBUS_CLK_CLK
#elif (CSITX_INSTANCE == 1U)
#define CSITX_MOD                               TISCI_DEV_CSI_TX_IF1
#define CSITX_DPHY_MOD                          TISCI_DEV_DPHY_TX1
#define CSITX_MOD_ESC_CLK                       TISCI_DEV_CSI_TX_IF1_ESC_CLK_CLK
#define CSITX_MOD_MAIN_CLK                      TISCI_DEV_CSI_TX_IF1_MAIN_CLK_CLK
#define CSITX_MOD_DPHY_TXBYTECLKHS_CLK          TISCI_DEV_CSI_TX_IF1_DPHY_TXBYTECLKHS_CL_CLK
#define CSITX_MOD_VBUS_CLK                      TISCI_DEV_CSI_TX_IF1_VBUS_CLK_CLK
#endif
#elif defined (SOC_J721S2)
#if (CSITX_INSTANCE == 0U)
#define CSITX_MOD                               TISCI_DEV_CSI_TX_IF_V2_0
#define CSITX_DPHY_MOD                          TISCI_DEV_DPHY_TX0
#define CSITX_MOD_ESC_CLK                       TISCI_DEV_CSI_TX_IF_V2_0_ESC_CLK_CLK
#define CSITX_MOD_MAIN_CLK                      TISCI_DEV_CSI_TX_IF_V2_0_MAIN_CLK_CLK
#define CSITX_MOD_DPHY_TXBYTECLKHS_CLK          TISCI_DEV_CSI_TX_IF_V2_0_DPHY_TXBYTECLKHS_CL_CLK
#define CSITX_MOD_VBUS_CLK                      TISCI_DEV_CSI_TX_IF_V2_0_VBUS_CLK_CLK
#elif (CSITX_INSTANCE == 1U)
#define CSITX_MOD                               TISCI_DEV_CSI_TX_IF_V2_1
#define CSITX_DPHY_MOD                          TISCI_DEV_DPHY_TX1
#define CSITX_MOD_ESC_CLK                       TISCI_DEV_CSI_TX_IF_V2_1_ESC_CLK_CLK
#define CSITX_MOD_MAIN_CLK                      TISCI_DEV_CSI_TX_IF_V2_1_MAIN_CLK_CLK
#define CSITX_MOD_DPHY_TXBYTECLKHS_CLK          TISCI_DEV_CSI_TX_IF_V2_1_DPHY_TXBYTECLKHS_CL_CLK
#define CSITX_MOD_VBUS_CLK                      TISCI_DEV_CSI_TX_IF_V2_1_VBUS_CLK_CLK
#endif
#endif
/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
static void taskFxn(void* a0, void* a1);
void App_wait(uint32_t wait_in_ms);
static void App_clkRateSet(uint32_t moduleId,
                           uint32_t clkId,
                           uint64_t clkRateHz);
extern int32_t Csitx_transmitTest(void);
extern void App_consolePrintf(const char *pcString, ...);
#if (APP_ENABLE_LOOPBACK == 1U)
static void rxTaskFxn(void* a0, void* a1);
extern void Csirx_captureTest(void);
#endif
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* Test application stack */
static uint8_t gTxAppTskStackMain[TX_APP_TSK_STACK_MAIN]__attribute__((aligned(TX_APP_TSK_STACK_MAIN)));

#if (APP_ENABLE_LOOPBACK == 1U)
static uint8_t gCaptAppTskStackMain[TX_APP_TSK_STACK_MAIN]__attribute__((aligned(TX_APP_TSK_STACK_MAIN)));
SemaphoreP_Handle gSyncSem;
SemaphoreP_Handle gRxStartSem;
#endif
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int main(void)
{
    TaskP_Params taskParams;
#if (APP_ENABLE_LOOPBACK == 1U)
    SemaphoreP_Params semParams;
#endif

    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority = 2;
    taskParams.stack = gTxAppTskStackMain;
    taskParams.stacksize = sizeof(gTxAppTskStackMain);

    TaskP_create(&taskFxn, &taskParams);

#if (APP_ENABLE_LOOPBACK == 1U)
    /* Create Rx Task */
    /* Initialize the task params */
    TaskP_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority = 2;
    taskParams.stack = gCaptAppTskStackMain;
    taskParams.stacksize = sizeof(gCaptAppTskStackMain);
    TaskP_create(&rxTaskFxn, &taskParams);

    /* Create sync semaphore */
    /* Allocate instance semaphore */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    gSyncSem = SemaphoreP_create(0U, &semParams);
    if (NULL == gSyncSem)
    {
        OS_stop();
    }

    gRxStartSem = SemaphoreP_create(0U, &semParams);
    if (NULL == gSyncSem)
    {
        OS_stop();
    }
#endif

    OS_start();    /* does not return */

    return(0);
}

static void taskFxn(void* a0, void* a1)
{
    int32_t retVal = CSL_PASS;
    Board_initCfg boardCfg = BOARD_SOK;
    uint64_t clkFreq = 0U;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UNLOCK_MMR;
#if !defined (QT_BUILD)
    boardCfg |= BOARD_INIT_UART_STDIO;
#endif

    if (BOARD_SOK == Board_init(boardCfg))
    {
        Sciclient_init(NULL);

        retVal += Sciclient_pmSetModuleState(CSITX_MOD,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
        retVal += Sciclient_pmSetModuleState(CSITX_DPHY_MOD,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
        retVal += Sciclient_pmSetModuleState(CSITX_PSIL_MOD,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
#if (APP_ENABLE_LOOPBACK == 1U)
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
#endif

        /* set CSITX clocks */
        App_clkRateSet(CSITX_MOD,
                       CSITX_MOD_ESC_CLK,
                       CSITX_ESC_CLK_FREQ_HZ);
        App_clkRateSet(CSITX_MOD,
                       CSITX_MOD_MAIN_CLK,
                       CSITX_MAIN_CLK_HZ);

       if (CSL_PASS == retVal)
        {
            /* Get the CSITX clock frequencies */
            retVal += Sciclient_pmGetModuleClkFreq(CSITX_MOD,
                                                CSITX_MOD_ESC_CLK,
                                                &clkFreq,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
            App_consolePrintf("\nClock Freq: CSITX_MOD_ESC_CLK = %lldHz", clkFreq);
            retVal += Sciclient_pmGetModuleClkFreq(CSITX_MOD,
                                                CSITX_MOD_DPHY_TXBYTECLKHS_CLK,
                                                &clkFreq,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
            App_consolePrintf("\nClock Freq: CSITX_MOD_DPHY_TXBYTECLKHS_CLK = %lldHz", clkFreq);
            retVal += Sciclient_pmGetModuleClkFreq(CSITX_MOD,
                                                CSITX_MOD_VBUS_CLK,
                                                &clkFreq,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
            App_consolePrintf("\nClock Freq: CSITX_MOD_VBUS_CLK = %lldHz", clkFreq);
            retVal += Sciclient_pmGetModuleClkFreq(CSITX_MOD,
                                                CSITX_MOD_MAIN_CLK,
                                                &clkFreq,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
            App_consolePrintf("\nClock Freq: CSITX_MOD_MAIN_CLK = %lldHz\n", clkFreq);
            Csitx_transmitTest();
        }
    }

    return;
}

#if (APP_ENABLE_LOOPBACK == 1U)
static void rxTaskFxn(void* a0, void* a1)
{
    Csirx_captureTest();

    return;
}
#endif

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
                                            0U, 
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
