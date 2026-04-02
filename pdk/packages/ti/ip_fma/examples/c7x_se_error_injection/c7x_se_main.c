/*
 *  Copyright (C) 2026 Texas Instruments Incorporated
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
 *  \file c7x_se_main.c
 *
 *  \brief Main file for streamming error injection example build. 
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "c7x_se_error.h"

#include <c7x.h>
#include <Exception.h>
#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/csl/soc.h>
#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

/* ========================================================================== */
/*                         Macros & Typedefs                                  */
/* ========================================================================== */

/* Streaming Engine Sequence Test Cases */
#define SE_OPENINGERROR 0x0U
#define SE_SAVINGERROR 0x1U
#define SE_RESTOREINGERROR 0x2U
#define SE_RESOURCEACCESSERROR 0x3U
#define SE_ALLTESTPASSED 0x4U
#define SE_TESTERROR 0x5U

/* IERR Streaming Engine Error Mask */
#define IERR_SEX_MASK     (0x1U << 11)
#define IERR_RAX_MASK     (0x1U << 6)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

Exception_Hooks Exceptionhooks;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Set the state variable to determine execution of test sequence*/
static uint16_t SE_current_test = 0U;

/* UART Print Flags set by exception hook */
static uint16_t SEX_printFlag = 0U;
static uint16_t RAX_printFlag = 0U;
static uint16_t UnknownException_printFlag = 0U;

/* IERR status register import */
extern volatile __cregister unsigned long __IERR;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void C7x_idle(void)
{
    UART_printf("Placing C7x in idle Condition\n");
   __asm(" IDLE");
   while (1)
   {
    /* code */
   }
}

void Check_print_flag(void)
{
    if(SEX_printFlag == 1U)
    {
        UART_printf("Exception is triggered by Streaming Engine.\n");
        SEX_printFlag = 0U;
    }
    else if(RAX_printFlag == 1U)
    {
        UART_printf("A Resource Access Exception was triggered.\n");
        RAX_printFlag = 0U;
    }
    else if(UnknownException_printFlag == 1U)
    {
        UART_printf("\nException is not triggered by a known source.\n");
        UnknownException_printFlag = 0U;
    }
    else
    {
        UART_printf("\nNo Exception has occured.\n");
    }
}

void SE_sequence_test(void)
{
    /* Write the test functions to be executed here */
    switch(SE_current_test)
    {
        case SE_OPENINGERROR:
            SE_current_test = SE_current_test + 1U; /* Increment Test Number */ 
            SE_OpeningError_Injection();  /* Execution Generates Exception */
            break;
        case SE_SAVINGERROR:
            Check_print_flag();
            SE_current_test = SE_current_test + 1U; /* Increment Test Number */ 
            SE_SavingError_Injection();  /* Execution Generates Exception */
            break;
        case SE_RESTOREINGERROR:
            Check_print_flag();
            SE_current_test = SE_current_test + 1U; /* Increment Test Number */ 
            SE_RestoringError_Injection();  /* Execution Generates Exception */
            break;
        case SE_RESOURCEACCESSERROR:
            Check_print_flag();
            SE_current_test = SE_current_test + 1U; /* Increment Test Number */ 
            SE_ResourceAccessError_Injection();  /* Execution Generates Exception */
            break;
        case SE_ALLTESTPASSED:
            Check_print_flag();
            UART_printf("\n All tests have passed\n");
            C7x_idle();
            break;
        case SE_TESTERROR:
            Check_print_flag();
            UART_printf("\n Error encountered while testapp execution\n");
            C7x_idle();
            break;
        default:
            Check_print_flag();
            UART_printf("Unknown error\n");
            C7x_idle();
            break;
    }
}

void SE_exception_return_hook(void)
{
    /* Transfer the Address in memory of the Test Sequence Instructions in register a0 */
    __asm(" addkpc.d1 $PCR_OFFSET(SE_sequence_test), a0");
    /* Save the Exception status in register a1 */   
    __asm(" ldd.d1 *ECSP[1], a1");
    /* Return back to the test sequence start from exception */
    __asm(" rete.s1 a0,a1"); 
}

void SE_exception_hook(void)
{  
    /* This hook will be called as soon as the exception is hit */
    /* Determine if the exception is caused by the Streaming Engine */
    if(__IERR & IERR_SEX_MASK)
    {
        SEX_printFlag = 1U;
    }
    else if(__IERR & IERR_RAX_MASK)
    {
        RAX_printFlag = 1U;
    }
    else
    {
        UnknownException_printFlag = 1U;
        SE_current_test = SE_TESTERROR;
    }
}

void Exceptionhook_init(void)
{
    UART_printf("Exception Return hook registration\n");
    Exceptionhooks.exceptionHook = &SE_exception_hook;
    Exceptionhooks.internalHook = NULL;
    Exceptionhooks.returnHook = &SE_exception_return_hook;
}

/* ========================================================================== */
/*                          Main Function                                     */
/* ========================================================================== */

int main(void)
{
    /* Initialize OS */
    OS_init();

    /* Configure the Pin Mux and UART */
    Board_initCfg boardCfg;
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    /* Register the Exception Hooks */
    Exceptionhook_init();
    Exception_registerHooks(&Exceptionhooks);

    /* Run Test Sequence */
    SE_sequence_test();

    C7x_idle();
    return(0);
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif