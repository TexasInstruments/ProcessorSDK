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
 *  \file c7x_illegal_instructions.c
 *
 *  \brief Implementation of illegal operations and 
 *         Instruction trapping which cause exceptions
 */

/*===========================================================================*/
/*                              Include Files                                */
/*===========================================================================*/

#include "c7x_illegal_instructions.h"

#include <c7x.h>
#include <float.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

/*===========================================================================*/
/*                        Private Function Definitions                       */
/*===========================================================================*/

void Illegal_instruction_injection()
{
    UART_printf("Beginning Illegal Instruction Opcode Test.\n");
    
    void (*func_ptr)() = (void (*)())0x00000000;
    func_ptr();
}

void Privilege_violation_injection()
{
    UART_printf("Beginning Access Privilege violation Test.\n");

    /* Accessing the Exception Context Save Pointer of the secure
     * supervisor is a privilege violation */
    __ECSP_SS = 0;

    /* observe IERR and IESR at this point 
     * can be implemented in the exception hook */
}

#pragma FUNCTION_OPTIONS(Floating_point_overflow_injection,"--opt_level=off")
uint16_t Floating_point_overflow_injection()
{
    uint16_t status = 0U;
    
    UART_printf("Beginning Floating Point Overflow Injection Test.\n");

    /* Set the status of all saturation operations to normal */
    __FSR = 0;            /* Flag Status Register */           
    
    /* Set the rounding mode for floating point operations to default */
    __FPCR = 0ULL;        /* Floating Point Configuration Register */

    /* Floating Point Overflow Flag Enable */
    __FPCR |= (1ULL << 10);

    float a = (float) 5;
    float b = FLT_MAX;
    
    volatile float c= a*b;

    UART_printf("FSR value : 0x%x\n", __FSR);

    uint8_t fsr_val = __get_FSR(float4);

    if (fsr_val & (1 << 2)) {
        UART_printf("Exception Caught: OverFlow.\n");
    } else {
        UART_printf("No exception.\n");
        status = 1U;
    }

    return status;
}

uint16_t Floating_point_divide_by_zero()
{
    uint16_t status = 0U;

    UART_printf("Beginning Floating Point Divide by Zero Test.\n");

    /* Divide by zero Flag Enable*/
    __FPCR |= (1ULL << 9); 

    /* Set the status of all saturation operations to normal */
    __FSR=0;

    /* Trigger an exception by taking*/
    volatile float4 a = float4(0);
    volatile float4 b = __recip(a); 

    /* Operation present to avoid compiler optimization of definition of float b */
    a = b + a;

    UART_printf("FSR value : 0x%x\n", __FSR);

    uint8_t fsr_val = __get_FSR(float4);

    if (fsr_val & (1 << 1)) {
        UART_printf("Exception Caught: Divide By Zero\n");
    } else {
        UART_printf("No exception");
        status = 1U;
    }

    return status;
}
