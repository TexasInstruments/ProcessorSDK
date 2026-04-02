/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2022
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
#include "ti/osal/osal.h"
#include "ti/osal/TaskP.h"
#include "ti/osal/LoadP.h"

#include <ti/board/board.h>
#include <ti/drv/dss/examples/utils/app_utils.h>
#include "dss_display_test.h"
#include <ti/drv/sciclient/sciclient.h>
#include <ti/csl/soc.h>

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
extern int32_t Dss_displayTest(void);
#if(1U == DISP_APP_TEST_OVERLAY_VP_4)
static void App_clkRateSet(uint32_t moduleId,
                           uint32_t clkId,
                           uint64_t clkRateHz);
#endif
#if defined(FREERTOS)
void App_printLoad(void);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
static uint8_t gDispAppTskStackMain[DISP_APP_TSK_STACK_MAIN];
/* Task handle */
TaskP_Handle gTask;

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
    taskParams.priority  = 2;
    taskParams.stack     = gDispAppTskStackMain;
    taskParams.stacksize = sizeof(gDispAppTskStackMain);

    gTask = TaskP_create(&taskFxn, &taskParams);
    if(NULL == gTask)
    {
        OS_stop();
    }
    OS_start();    /* does not return */

    return(0);
}

static void taskFxn(void* a0, void* a1)
{
    int32_t retVal = CSL_PASS;
#if(1U == DISP_APP_TEST_MULTISYNC)
    uint32_t regVal;
#endif
#if defined(FREERTOS)
    LoadP_reset();
#endif
    Board_initCfg boardCfg;
    App_utilsLcdCfgParams lcdParams;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
                BOARD_INIT_UNLOCK_MMR |
                BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

#if defined (SOC_AM65XX)
    lcdParams.outType  = APP_OUTPUT_OLDI;
    lcdParams.pixelClk = (uint64_t)497500000;
    App_configureLCD(lcdParams);
#else
    #if (1U == DISP_APP_TEST_DSI)
        lcdParams.outType  = APP_OUTPUT_DSI;
#if defined (SOC_J721E)
        lcdParams.pixelClk = (uint64_t)74250000ULL;
#else
        lcdParams.pixelClk = (uint64_t)64000000ULL;
#endif
        App_configureLCD(lcdParams);
    #elif (1U == DISP_APP_TEST_EDP)
        lcdParams.outType  = APP_OUTPUT_EDP;
        lcdParams.pixelClk = (uint64_t)148500000ULL;
        if(DISP_APP_USE_TEST_PARAMS == DISP_APP_BGR24)
        {
            lcdParams.pixelClk = (uint64_t)74250000U;
        }
        App_configureLCD(lcdParams);
    #else
        lcdParams.outType  = APP_OUTPUT_HDMI;
        lcdParams.pixelClk = (uint64_t)148500000ULL;
        if(DISP_APP_USE_TEST_PARAMS == DISP_APP_BGR24)
        {
            lcdParams.pixelClk = (uint64_t)74250000U;
        }
        App_configureLCD(lcdParams);
        #if (1U == DISP_APP_TEST_MULTISYNC)
            /* HSYNC mode 14 <- VP0 HSYNC */
            regVal = CSL_REG32_RD(CSL_CTRL_MMR0_CFG0_BASE +
                    CSL_MAIN_CTRL_MMR_CFG0_PADCONFIG38);
            CSL_FINS(regVal,
                    MAIN_CTRL_MMR_CFG0_PADCONFIG38_MUXMODE,
                    0xEU);
            CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE +
                    CSL_MAIN_CTRL_MMR_CFG0_PADCONFIG38, regVal);

            /* DE MODE 14 <- VP0 DE */
            regVal = CSL_REG32_RD(CSL_CTRL_MMR0_CFG0_BASE +
                    CSL_MAIN_CTRL_MMR_CFG0_PADCONFIG39);
            CSL_FINS(regVal,
                    MAIN_CTRL_MMR_CFG0_PADCONFIG39_MUXMODE,
                    0xEU);
            CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE +
                    CSL_MAIN_CTRL_MMR_CFG0_PADCONFIG39, regVal);

            /* VSYNC MODE 14 <- VP0 VSYNC */
            regVal = CSL_REG32_RD(CSL_CTRL_MMR0_CFG0_BASE +
                    CSL_MAIN_CTRL_MMR_CFG0_PADCONFIG40);
            CSL_FINS(regVal,
                    MAIN_CTRL_MMR_CFG0_PADCONFIG40_MUXMODE,
                    0xEU);
            CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE +
                    CSL_MAIN_CTRL_MMR_CFG0_PADCONFIG40, regVal);

            /* DPI_0_PCLK <- DPI_1_PCLK */
            regVal = CSL_REG32_RD(CSL_CTRL_MMR0_CFG0_BASE +
                    CSL_MAIN_CTRL_MMR_CFG0_DSS_DISPC0_CLKSEL3);
            CSL_FINS(regVal,
                    MAIN_CTRL_MMR_CFG0_DSS_DISPC0_CLKSEL3_DPI3_PCLK,
                    0x5U);
            CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE +
                    CSL_MAIN_CTRL_MMR_CFG0_DSS_DISPC0_CLKSEL3, regVal);
        #endif
    #endif
#endif
    App_configureSoC();

    Sciclient_init(NULL);
    retVal += Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
#if defined (SOC_J721E) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#if (1U == DISP_APP_TEST_EDP)    
    retVal += Sciclient_pmSetModuleState(TISCI_DEV_DSS_EDP0,
                                   TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                   TISCI_MSG_FLAG_AOP,
                                   SCICLIENT_SERVICE_WAIT_FOREVER);
#endif                                   
#if (1U == DISP_APP_TEST_DSI)
    retVal += Sciclient_pmSetModuleState(TISCI_DEV_DPHY_TX0,
                                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                TISCI_MSG_FLAG_AOP,
                                SCICLIENT_SERVICE_WAIT_FOREVER);

    retVal += Sciclient_pmSetModuleState(TISCI_DEV_DSS_DSI0,
                                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                TISCI_MSG_FLAG_AOP,
                                SCICLIENT_SERVICE_WAIT_FOREVER);
#endif
#endif
#if(1U == DISP_APP_TEST_OVERLAY_VP_4)
    uint64_t clkFreq = 0U;

    /* set CSITX clocks */
    App_clkRateSet(TISCI_DEV_DSS0,
                   TISCI_DEV_DSS0_DSS_INST0_DPI_3_IN_2X_CLK,
                   148500000U);
    /* Get the CSITX clock frequencies */
    retVal += Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
                                        TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK,
                                        &clkFreq,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    App_print("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK = %lld Hz\n", clkFreq);
    retVal += Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
                                        TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK,
                                        &clkFreq,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);

    App_print("\n TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK = %lld Hz\n", clkFreq);
    retVal += Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
                                        TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK,
                                        &clkFreq,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    App_print("\n TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK = %lld Hz\n", clkFreq);
    retVal += Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
                                        TISCI_DEV_DSS0_DSS_INST0_DPI_3_IN_2X_CLK,
                                        &clkFreq,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    App_print("\n TISCI_DEV_DSS0_DSS_INST0_DPI_3_IN_2X_CLK = %lld Hz\n", clkFreq);
#endif
    if (CSL_PASS == retVal)
    {
        App_print("Before calling Dss_displayTest\n");
        App_printLoad();
        Dss_displayTest();
    }
    App_print("After calling Dss_displayTest\n");
    App_printLoad();
    return;
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);

void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

void App_wait(uint32_t wait_in_ms)
{
    TaskP_sleep(wait_in_ms);
}

#if defined(FREERTOS)
void App_printLoad(void)
{
    LoadP_Stats loadStatsTask;
    uint32_t cpuLoad;

    /* Query CPU Load */
    cpuLoad = LoadP_getCPULoad();
    App_print("CPU Load is %d percent \n", cpuLoad);
    /* Get task loads */
    LoadP_getTaskLoad(gTask, &loadStatsTask);
    if(loadStatsTask.percentLoad > 0U)
    {
        App_print("Task Load is: %d percent \n", loadStatsTask.percentLoad);
    }
    else
    {
        App_print("Task Load is: < 1 percent \n");
    }
}
#endif

#if(1U == DISP_APP_TEST_OVERLAY_VP_4)
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
            App_print("\nSciclient_pmSetModuleClkFreq Passed for clock Id = %d\n", clkId);
        }
        else
        {
            App_print("\nSciclient_pmSetModuleClkFreq failed for clock Id = %d\n", clkId);
        }
    }
}
#endif
