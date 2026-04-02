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
 *  \file c7x_mmu_main.c
 *
 *  \brief Main file demonstrating MMU error injection  
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "c7x_mmu_error.h"

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

/* MMU Test Sequence Cases */
#define MMU_UNINITMEMACCESS 0x0U
#define MMU_ACCESSPERMVIOLATION 0x1U
#define MMU_ALLTESTPASSED 0x2U
#define MMU_TESTERROR 0x3U

/* IERR Page Fault Error Mask */
#define IERR_MMU_MASK     (0x1U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

Exception_Hooks Exceptionhooks;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Set the state variable to determine execution of test sequence */
static uint16_t MMU_current_test = 0U;

/* UART Print Flags set by exception hook */
static uint16_t MMU_printFlag = 0U;
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
    if(MMU_printFlag == 1U)
    {
        UART_printf("An exception was triggered by MMU.\n");
        MMU_printFlag = 0U;
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

void MMU_sequence_test(void)
{
    /* Write the test functions to be executed here */
    switch(MMU_current_test)
    {
        case MMU_UNINITMEMACCESS:
            MMU_current_test = MMU_current_test + 1U; /* Increment Test Number */
            MMU_unInitMemAccess();  /* Execution Generates Exception */
            break;
        case MMU_ACCESSPERMVIOLATION:
            Check_print_flag();
            MMU_current_test = MMU_current_test + 1U; /* Increment Test Number */ 
            MMU_accessPermViolation(); /* Execution Generates Exception */
            break;
        case MMU_ALLTESTPASSED:
            Check_print_flag();
            UART_printf("\n All tests have passed\n");
            C7x_idle();
            break;
        case MMU_TESTERROR:
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

void MMU_exception_return_hook(void)
{
    /* Transfer the Address in memory of the Test Sequence Instructions in register a0 */
    __asm(" addkpc.d1 $PCR_OFFSET(MMU_sequence_test), a0");
    /* Save the Exception status in register a1 */   
    __asm(" ldd.d1 *ECSP[1], a1");
    /* Return back to the test sequence start from exception */
    __asm(" rete.s1 a0,a1"); 
}

void MMU_exception_hook(void)
{  
    /* This hook will be called as soon as the exception is hit */
    /* Determine if the exception is caused by the MMU */
    if(__IERR & IERR_MMU_MASK)
    {
        MMU_printFlag = 1U;
    }else
    {   
        UnknownException_printFlag = 1U;
        MMU_current_test = MMU_TESTERROR;
    }
}

void Exceptionhook_init(void)
{
    UART_printf("Exception Return hook registration\n");
    Exceptionhooks.exceptionHook = &MMU_exception_hook;
    Exceptionhooks.internalHook = NULL;
    Exceptionhooks.returnHook = &MMU_exception_return_hook;
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
    MMU_sequence_test();

 
    return(0);
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif
