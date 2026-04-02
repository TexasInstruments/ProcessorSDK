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
 * \file  mailbox_perf_main.c
 * \brief This is the main file for mailbox perf application that runs
 *        on each of the cores
**/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdio.h>
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

//#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include "mailbox_app.h"
#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/* Test application stack size */
#define APP_TSK_STACK_MAIN              (16U * 1024U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN]
__attribute__ ((aligned(8192)));

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

static Void taskFxn(UArg a0, UArg a1);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void padConfig_prcmEnable()
{
    /*Pad configurations */
    Board_initCfg boardCfg;
#if defined (SOC_AM65XX)
    boardCfg = BOARD_INIT_UNLOCK_MMR | BOARD_INIT_UART_STDIO |
               BOARD_INIT_MODULE_CLOCK | BOARD_INIT_PINMUX_CONFIG;
#else
    boardCfg = BOARD_INIT_MODULE_CLOCK  |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#endif
    Board_init(boardCfg);

}

int32_t test_Mailbox_runPerfTests(void)
{
    int32_t retValue;

    retValue = perfTestInit();
    if (retValue != 0)
    {
        MailboxAppPrint("Perf Test init failed\n");
    }
    if (retValue == 0)
    {
        retValue = perfTestRun();
#ifdef SYSTEM_MASTER
        if (retValue == 0)
        {
            MailboxAppPrint("Polling mode: Perf Test Run Passed\n");
        }
        else
        {
            MailboxAppPrint("Polling mode: Perf Test Run failed\n");
        }
#endif
    }
    return retValue;
}
int32_t test_Mailbox_runInterruptModePerfTests(void)
{
    int32_t retValue;

    retValue = perfTestInterruptModeInit();
    if (retValue != 0)
    {
        MailboxAppPrint("Interrupt mode: Perf Test init failed\n");
    }
    if (retValue == 0)
    {
        retValue = perfTestRun();
#ifdef SYSTEM_MASTER
        if (retValue == 0)
        {
            MailboxAppPrint("Interrupt mode: Perf Test Run Passed\n");
        }
        else
        {
            MailboxAppPrint("Interrupt mode: Perf Test Run failed\n");
        }
#endif
    }
    return retValue;
}
void test_mailbox_perf_app_runner(void)
{
    int32_t retValue = 0;

    retValue = test_Mailbox_runPerfTests();
    if (retValue == 0)
    {
        retValue = test_Mailbox_runInterruptModePerfTests();
    }
#ifdef SYSTEM_MASTER
    if (retValue == 0)
    {
        MailboxAppPrint("\nAll tests have passed. \n");
    }
    else
    {
        MailboxAppPrint("Test failed\n");
    }

#endif
}

int main(void)
{
    Task_Handle task;
    Error_Block eb;
    Task_Params taskParams;

    Error_init(&eb);

    /* Initialize the task params */
    Task_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stackSize    = sizeof (gAppTskStackMain);

    task = Task_create(taskFxn, &taskParams, &eb);
    if(NULL == task)
    {
        BIOS_exit(0);
    }
    BIOS_start();    /* does not return */

    return(0);
}

static Void taskFxn(UArg a0, UArg a1)
{

    /*Pad configuration and PRCM enable*/
    padConfig_prcmEnable();
#if (defined (SOC_AM65XX) || defined(SOC_AM64X))
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
    test_mailbox_perf_app_runner();
}

void MailboxAppPrint(const char * str)
{
#ifdef USE_STD_PRINTF
    printf(str);
#else
    UART_printf(str);
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

#if defined(BUILD_MPU)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

/********************************* End of file ******************************/
