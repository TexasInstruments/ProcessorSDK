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
 *  \file c7x_cpu_main.c
 *
 *  \brief Main file for example build, 
 *   Please refer to the application note in below link regarding the flow of the code
 *   https://www.ti.com/lit/an/sprads4/sprads4.pdf
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "c7x_illegal_instructions.h"

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

/* Illegal Instructions Sequence Test Cases */
#define ILLEGAL_INSTRUCTION_INJECTION 0x0U
#define PRIVILEGE_VIOLATION_INJECTION 0x1U
#define ALLTESTPASSED 0x2U
#define TESTERROR 0x3U

/* IERR Illegal Instruction Opcode Error Mask */
#define IERR_OPX_MASK     (0x1U << 4)
/* IERR Privilege Exception Error Mask */
#define IERR_PRX_MASK     (0x1U << 7)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

Exception_Hooks Exceptionhooks;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Set the state variable to determine execution of test sequence*/
static uint16_t Illegal_Instructions_current_test = 0U;

/* Variable to store return values of fault injecting functions */
static uint16_t Test_failed = 0U;

/* UART Print Flags set by exception hook */
static uint16_t OPX_printFlag = 0U;
static uint16_t PRX_printFlag = 0U;
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
    if(OPX_printFlag == 1U)
    {
        UART_printf("An exception is triggered due to an illegal opcode.\n");
        OPX_printFlag = 0U;
    }
    else if(PRX_printFlag == 1U)
    {
        UART_printf("An exception is triggered due to an privilege memory access.\n");
        PRX_printFlag = 0U;
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

void Illegal_instructions_sequence_test(void)
{
    /* Write the test functions to be executed here */
    switch(Illegal_Instructions_current_test)
    {
        case ILLEGAL_INSTRUCTION_INJECTION:
            Illegal_Instructions_current_test += 1U; /* Increment Test Number */ 
            Illegal_instruction_injection();  /* Execution Generates Exception */
            break;
        case PRIVILEGE_VIOLATION_INJECTION:
            Check_print_flag();
            Illegal_Instructions_current_test += 1U; /* Increment Test Number */ 
            Privilege_violation_injection();  /* Execution Generates Exception */
            break;
        case ALLTESTPASSED:
            Check_print_flag();
            if(Test_failed == 0U)
            {
                UART_printf("\n All tests have passed\n");
            }
            else
            {
                UART_printf("\nError in FSR Fault Injection or Detection.\n");
            }
            C7x_idle();
            break;
        case TESTERROR:
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

void Exception_return_hook(void)
{
    /* Transfer the Address in memory of the Test Sequence Instructions in register a0 */
    __asm(" addkpc.d1 $PCR_OFFSET(Illegal_instructions_sequence_test), a0");
    /* Save the Exception status in register a1 */   
    __asm(" ldd.d1 *ECSP[1], a1");
    /* Return back to the test sequence start from exception */
    __asm(" rete.s1 a0,a1"); 
}

void Exception_detection_hook(void)
{  
    /* This hook will be called as soon as the exception is hit */
    /* Determine if the exception is caused by a known fault */
    if(__IERR & IERR_OPX_MASK)
    {
        OPX_printFlag = 1U;
    }
    else if(__IERR & IERR_PRX_MASK) 
    {
        PRX_printFlag = 1U;
    }
    else
    {
        UnknownException_printFlag = 1U;
        Illegal_Instructions_current_test = TESTERROR;
    }
}

void Exceptionhook_init(void)
{
    UART_printf("Exception Return hook registration\n");
    Exceptionhooks.exceptionHook = &Exception_detection_hook;
    Exceptionhooks.internalHook = NULL;
    Exceptionhooks.returnHook = &Exception_return_hook;
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
    
    /* Floating Point Illegal Operations Test */
    Test_failed = Floating_point_overflow_injection();
    Test_failed = Floating_point_divide_by_zero();
    
    /* Run Test Sequence */
    Illegal_instructions_sequence_test();

    return(0);
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif
