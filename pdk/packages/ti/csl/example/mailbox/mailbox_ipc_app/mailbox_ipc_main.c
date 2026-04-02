/*
 *  Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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
 * \file  mailbox_ipc_main.c
 * \brief This is the main file for mailbox ipc application that runs
 *        on each of the cores
**/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdio.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/csl_mailbox.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include "mailbox_app.h"
#if defined (SOC_AM65XX) || defined (SOC_J721E) || defined (SOC_AM64X)
#include <ti/board/board.h>
#endif
#if defined (SOC_AM65XX) || defined (SOC_J721E) || defined (SOC_AM64X)
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#endif

#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif
/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
int32_t testResult = 0;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
#ifdef UNITY_INCLUDE_CONFIG_H
/*
 *  ======== Unity set up and tear down ========
 */
void setUp(void)
{
    /* Do nothing */
}

void tearDown(void)
{
    /* Do nothing */
}
#endif

void padConfig_prcmEnable()
{
    Board_initCfg boardCfg;
#if defined (SOC_AM65XX) || defined (SOC_J721E)
    /*Pad configurations */
    boardCfg = BOARD_INIT_UNLOCK_MMR | BOARD_INIT_UART_STDIO |
               BOARD_INIT_MODULE_CLOCK | BOARD_INIT_PINMUX_CONFIG;
#elif defined (SOC_AM64X)
    boardCfg = BOARD_INIT_UART_STDIO |
               BOARD_INIT_MODULE_CLOCK | BOARD_INIT_PINMUX_CONFIG;
#endif
    Board_init(boardCfg);

}

void test_CSLMailbox_runIPCTests(void)
{
    /* @description: Test runner for IPC test between multiple cores

       @requirements: PRSDK-6317

       @cores: all_cores */

    int32_t retValue;

    retValue = ipcAppInit();
    if (retValue != 0)
    {
        MailboxAppPrint("IPC App init failed\n");
    }
    if (retValue == 0)
    {
        retValue = ipcAppRun();
#ifdef SYSTEM_MASTER
        if (retValue == 0)
        {
            MailboxAppPrint("Polling mode: IPC App Run Passed\n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_PASS();
#endif
        }
        else
        {
            MailboxAppPrint("Polling mode: IPC App Run failed\n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_FAIL();
#endif
            testResult = -1;
        }
#endif
    }
}
void test_CSLMailbox_runInterruptModeIPCTests(void)
{
    /* @description: Test runner for IPC test in Interrupt between multiple cores

       @requirements: PRSDK-6317

       @cores: all_cores */

    int32_t retValue;

    retValue = ipcAppInterruptModeInit();
    if (retValue != 0)
    {
        MailboxAppPrint("Interrupt mode: IPC App init failed\n");
    }
    if (retValue == 0)
    {
        retValue = ipcAppRun();
#ifdef SYSTEM_MASTER
        if (retValue == 0)
        {
            MailboxAppPrint("Interrupt mode: IPC App Run Passed\n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_PASS();
#endif
        }
        else
        {
            MailboxAppPrint("Interrupt mode: IPC App Run failed\n");
#ifdef UNITY_INCLUDE_CONFIG_H
        TEST_FAIL();
#endif
            testResult = -1;
        }
#endif
    }
}
void test_csl_mailbox_ipc_app_runner(void)
{
    /* @description: Test runner for IPC tests

       @requirements: PRSDK-6317

       @cores: all_cores */

#if defined(UNITY_INCLUDE_CONFIG_H) && defined(SYSTEM_MASTER)

    UNITY_BEGIN();

    RUN_TEST(test_CSLMailbox_runIPCTests);

    RUN_TEST(test_CSLMailbox_runInterruptModeIPCTests);

    UNITY_END();

    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
#else
    test_CSLMailbox_runIPCTests();
    test_CSLMailbox_runInterruptModeIPCTests();
#endif
    if (testResult == 0)
    {
        UART_printStatus("\r\nAll tests have passed. \r\n");
    }
    else
    {
        UART_printStatus("\r\nSome tests have failed. \r\n");
    }

}

int main(void)
{
    /*Pad configuration and PRCM enable*/
    padConfig_prcmEnable();
#if (defined (SOC_AM65XX) || defined (SOC_J721E) || defined(SOC_AM64X))
#ifdef BUILD_MCU
    {
        uint32_t val;

        /* Initialize and start PMU timer */

        CSL_armR5PmuCfg(0, 0, 1);
        /* Clear the overflow */
        val = CSL_armR5PmuReadCntrOverflowStatus();
        val &= 0x80000000;
        CSL_armR5PmuClearCntrOverflowStatus(val);
        CSL_armR5PmuCfgCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM, CSL_ARM_R5_PMU_EVENT_TYPE_CYCLE_CNT);
        CSL_armR5PmuEnableAllCntrs(1);
        CSL_armR5PmuEnableCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM, 1);
    }
#endif
#endif
#ifdef SYSTEM_MASTER
    MailboxAppPrint("\nMaster: Mailbox Application starting\n");
#endif
    test_csl_mailbox_ipc_app_runner();

    return 0;
}

void MailboxAppPrint(const char * str)
{
#ifdef USE_STD_PRINTF
    printf(str);
#else
#if defined (SOC_AM65XX) || defined (SOC_J721E) || defined (SOC_AM64X)
    UART_printf(str);
#else
    UARTConfigPuts(uartBaseAddr, str, -1);
#endif
#endif
}

void MailboxAppPrintf(const char *pFormat, ...)
{
    char     printString[256];
    va_list  vaArgP;

    /* Start the varargs processing. */
    va_start(vaArgP, pFormat);

    vsnprintf(&printString[0U], sizeof(printString), pFormat, vaArgP);

    MailboxAppPrint(printString);
    /* End the varargs processing. */
    va_end(vaArgP);

    return;
}

/********************************* End of file ******************************/
