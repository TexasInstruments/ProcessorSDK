/*
 *  Copyright (C) 2025 Texas Instruments Incorporated
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
 *  \file c7x_mma_main.c
 *
 *  \brief Main file for example build, 
 *   Please refer to the application note in below link regarding the flow of the code
 *   https://www.ti.com/lit/an/sprads4/sprads4.pdf
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/csl/soc.h>
#include <ti/board/board.h>
#include <Exception.h>
#include <c7x_mma_error.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define MMA_OVERFLOW       0x05
#define MMA_UNDERFLOW      0x04
#define MMA_OFFSET_PARITY  0x0A
#define MMA_CONFIG_PARITY  0x09
#define MMA_C_MATRIX_READ  0x02
#define MMA_C_MATRIX_WRITE 0x03
#define IESR_STATUS_MASK  0x000000FF    
#define IAER_MMA_MASK     (0x1<<14)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

Exception_Hooks Exceptionhooks;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile uint8_t MMA_Expected_error = 0;
uint32_t MMA_Execution_log = 0;
uint32_t MMA_Execution_verify = ((1<<MMA_OVERFLOW_EXCEPTION)|(1<<MMA_UNDERFLOW_EXCEPTION)|(1<<MMA_OFFSET_PARITY_ERROR)|(1<<MMA_AFSM_CONFIG_PARITY_ERROR)|
(1<<MMA_BFSM_CONFIG_PARITY_ERROR)|(1<<MMA_CFSM_CONFIG_PARITY_ERROR)|(1<<MMA_XFSM_CONFIG_PARITY_ERROR)|(1<<MMA_C_MATRIX_READ_COLLISION)|(1<<MMA_C_MATRIX_WRITE_COLLISION));
static uint16_t MMA_current_exception = 0;
uint8_t C7xIdle = TRUE;
extern volatile __cregister unsigned long __IERR;
extern volatile __cregister unsigned long __IESR;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


void C7x_idle(void)
{
    UART_printf("Placing C7x in idle Condition\n");
   __asm(" IDLE");
   while (C7xIdle)
   {
    /* code */
   }
}

void MMA_Sequence_test(void)
{

    switch(MMA_current_exception)
        {
            case MMA_OVERFLOW_EXCEPTION:
                MMA_Expected_error = MMA_OVERFLOW;
                MMA_Execution_log|=(uint32_t)(1<<MMA_OVERFLOW_EXCEPTION);
                ClearMMA(); //this cleares the previous mma error code, definition //is given above
                MMA_current_exception = MMA_current_exception+1;//increment track exception 
                MMA_overflowError_injection();//generate exception
                break;

            case MMA_UNDERFLOW_EXCEPTION:
                MMA_Expected_error = MMA_UNDERFLOW;
                MMA_Execution_log|=(uint32_t)(1<<MMA_UNDERFLOW_EXCEPTION);
                ClearMMA();
                MMA_current_exception = MMA_current_exception+1;
                MMA_underflowError_injection(); // generate exception
                break;

            case MMA_C_MATRIX_WRITE_COLLISION:
                MMA_Expected_error = MMA_C_MATRIX_WRITE;
                MMA_Execution_log|=(uint32_t)(1<<MMA_C_MATRIX_WRITE_COLLISION);
                ClearMMA();
                MMA_current_exception = MMA_current_exception+1;
                MMA_cMatrixWrite_collision();
                break;

            case MMA_C_MATRIX_READ_COLLISION:
                MMA_Expected_error = MMA_C_MATRIX_READ;
                MMA_Execution_log|=(uint32_t)(1<<MMA_C_MATRIX_READ_COLLISION);
                ClearMMA();
                MMA_current_exception = MMA_current_exception+1;
                MMA_cMatrixRead_collision();
                break;

            case MMA_AFSM_CONFIG_PARITY_ERROR:
                MMA_Expected_error = MMA_CONFIG_PARITY;
                MMA_Execution_log|=(uint32_t)(1<<MMA_AFSM_CONFIG_PARITY_ERROR);
                ClearMMA(); 
                MMA_current_exception = MMA_current_exception+1;
                MMA_AFSM_configParityError_injection();
                break;
            

            case MMA_BFSM_CONFIG_PARITY_ERROR:
                MMA_Expected_error = MMA_CONFIG_PARITY;
                MMA_Execution_log|=(uint32_t)(1<<MMA_BFSM_CONFIG_PARITY_ERROR);
                ClearMMA(); 
                MMA_current_exception = MMA_current_exception+1;
                MMA_BFSM_configParityError_injection();
                break;

            
            case MMA_CFSM_CONFIG_PARITY_ERROR:
                MMA_Expected_error = MMA_CONFIG_PARITY;
                MMA_Execution_log|=(uint32_t)(1<<MMA_CFSM_CONFIG_PARITY_ERROR);
                ClearMMA(); 
                MMA_current_exception = MMA_current_exception+1;
                MMA_CFSM_configParityError_injection();
                break;
            
            case MMA_XFSM_CONFIG_PARITY_ERROR:
                MMA_Expected_error = MMA_CONFIG_PARITY;
                MMA_Execution_log|=(uint32_t)(1<<MMA_XFSM_CONFIG_PARITY_ERROR);
                ClearMMA(); 
                MMA_current_exception = MMA_current_exception+1;
                MMA_XFSM_configParityError_injection();
                break;

            case MMA_OFFSET_PARITY_ERROR:
                MMA_Expected_error = MMA_OFFSET_PARITY;
                MMA_Execution_log|=(uint32_t)(1<<MMA_OFFSET_PARITY_ERROR);
                ClearMMA();
                MMA_current_exception = MMA_current_exception+1;
                MMA_offsetPartiyError_injection();//generate excpetion
                break;

            case MMA_TEST_DONE:
                if((MMA_Execution_verify & MMA_Execution_log) == (MMA_Execution_verify))
                {       
                    UART_printf("All test have been Executed\n");
                }
                else
                {
                    UART_printf("Did not execute all the test\n");
                } 
                C7x_idle();
                break;
            default:
                UART_printf("Test missed a seqeunce\n");
    
                break;
        }
}

void MMA_exception_return_hook(void)
{
    __asm(" addkpc.d1 $PCR_OFFSET(MMA_Sequence_test), a0"); //Transfers the test function      
    __asm(" ldd.d1 *ECSP[1], a1");//Status is saved in a1 
    __asm(" rete.s1 a0,a1"); //rete, will return from the exception to the beginning of the test function, as //a0 contains the test function address
}

void MMA_exception_hook(void)
{  
  if(__IERR & IAER_MMA_MASK)
  {
  uint8_t mma_int_recv = (__IESR & IESR_STATUS_MASK);
  switch(mma_int_recv)
  {
    case MMA_OVERFLOW:
         UART_printf("MMA Overflow Error received\n");
         break;
    case MMA_UNDERFLOW:
         UART_printf("MMA Underflow Error received\n");
         break;
    case MMA_OFFSET_PARITY:
         UART_printf("MMA Offset Parity Error received\n");
         break;
    case MMA_CONFIG_PARITY:
         UART_printf("MMA Config parity Error received\n");
         break;
    case MMA_C_MATRIX_READ:
         UART_printf("MMA C Matrix Read Error received\n");
         break;
    case MMA_C_MATRIX_WRITE:
         UART_printf("MMA C Matrix Write Error received\n");
         break;
    default:
        UART_printf("Unknown error\n");
    break;

  }
  if(mma_int_recv == MMA_Expected_error)
  {
    UART_printf("Error received matches with the injected error\n");
  }
  else{
   UART_printf("Error received does not match with the expected error\n");
  }
  }
  else{
    UART_printf("Not a MMA error\n");
  }
}

void Exceptionhook_init(void)
{
    UART_printf("Exception Return hook registration\n");
    Exceptionhooks.exceptionHook = &MMA_exception_hook;
    Exceptionhooks.internalHook = NULL;
    Exceptionhooks.returnHook = &MMA_exception_return_hook;
}

int main(void)
{
    OS_init();
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);
    Exceptionhook_init(); //Initialises Exception hook
    Exception_registerHooks(&Exceptionhooks);
    MMA_Sequence_test();  //Starts MMA sequence test

    return(0);
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif



