/*
 *  Copyright (C) 2021-2022 Texas Instruments Incorporated
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
 *  \file   SafeRTOS_aborts_r5.c
 *
 *  \brief The file implements safertos abort exception handlers for R5F.
 *
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>

#include "SafeRTOS_API.h"
#include "Safertos_Aborts.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* Following functions are defined in SafeRTOS_utils_r5f.asm */
portUInt32Type ulGetDataFaultStatusRegister( void );
portUInt32Type ulGetDataFaultAddressRegister( void );
portUInt32Type ulGetInstructionFaultStatusRegister( void );
portUInt32Type ulGetInstructionFaultAddressRegister( void );
portUInt32Type ulGetCPSR( void );
void vUndefAbort_c(void);
void vPrefetchAbort_c(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Faulty Stack Pointer at Data Abort. */
uint32_t FaultySP;

/* Faulty Link Register at Data Abort. */
uint32_t FaultyLR;

/* Faulty General Purpose Registers at Data Abort. */
uint32_t FaultyGPR[13];

extern CSL_R5ExptnHandlers gExptnHandlers;
extern volatile uint32_t gCurrentProcessorState;

/* ========================================================================== */
/*                          Function Defintions                               */
/* ========================================================================== */

void vDumpExceptionState( void )
{
    volatile portUInt32Type DFSR, DFAR, IFSR, IFAR, CPSR;
    DFSR = ulGetDataFaultStatusRegister();
    DFAR = ulGetDataFaultAddressRegister();
    IFSR = ulGetInstructionFaultStatusRegister();
    IFAR = ulGetInstructionFaultAddressRegister();
    CPSR = ulGetCPSR();
    DebugP_exceptionLog("[FATAL]: Core has Aborted!!!\nDFAR =0x%x DFSR =0x%x\n", (uintptr_t)DFAR, (uintptr_t)DFSR);
    DebugP_exceptionLog("IFAR =0x%x IFSR =0x%x\n", (uintptr_t)IFAR, (uintptr_t)IFSR);
    DebugP_exceptionLog("CPSR =0x%x SP =0x%x\n", (uintptr_t)CPSR, (uintptr_t)FaultySP);
    DebugP_exceptionLog("LR =0x%x R0 =0x%x\n", (uintptr_t)FaultyLR, (uintptr_t)FaultyGPR[0]);
    DebugP_exceptionLog("R1 =0x%x R2 =0x%x\n", (uintptr_t)FaultyGPR[1], (uintptr_t)FaultyGPR[2]);
    DebugP_exceptionLog("R3 =0x%x R4 =0x%x\n", (uintptr_t)FaultyGPR[3], (uintptr_t)FaultyGPR[4]);
    DebugP_exceptionLog("R5 =0x%x R6 =0x%x\n", (uintptr_t)FaultyGPR[5], (uintptr_t)FaultyGPR[6]);
    DebugP_exceptionLog("R7 =0x%x R8 =0x%x\n", (uintptr_t)FaultyGPR[7], (uintptr_t)FaultyGPR[8]);
    DebugP_exceptionLog("R9 =0x%x R10 =0x%x\n", (uintptr_t)FaultyGPR[9], (uintptr_t)FaultyGPR[10]);
    DebugP_exceptionLog("R11 =0x%x R12 =0x%x\n", (uintptr_t)FaultyGPR[11], (uintptr_t)FaultyGPR[12]);
}

/*---------------------------------------------------------------------------*/
void vUndefAbort_c(void)
{
    gCurrentProcessorState = CSL_ARM_R5_ABORT_MODE;
    /* Call registered call back */
    if ((exptnHandlerPtr)NULL != gExptnHandlers.udefExptnHandler)
    {
        gExptnHandlers.udefExptnHandler(gExptnHandlers.udefExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile uint32_t loop = 1U;
        while(1U == loop)
        {

        }
    }
}

/*---------------------------------------------------------------------------*/

void vPrefetchAbort_c(void)
{
    gCurrentProcessorState = CSL_ARM_R5_ABORT_MODE;
    /* Call registered call back */
    if ((exptnHandlerPtr)NULL != gExptnHandlers.pabtExptnHandler)
    {
        gExptnHandlers.pabtExptnHandler(gExptnHandlers.pabtExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile uint32_t loop = 1U;
        while(1U == loop)
        {

        }
    }
}

/*---------------------------------------------------------------------------*/

/* Data Abort handler starts execution in vDataAbort, defined in 
 * SafeRTOS_utils_r5f.asm. After some initial assembly logic it then branches 
 * to this function. After exiting this function it does some more assembly to 
 * return to the next instruction following the one which caused the exception.
 */
void vDataAbort_c(void)
{
    gCurrentProcessorState = CSL_ARM_R5_ABORT_MODE;
    vDumpExceptionState();
    /* Call registered call back */
    if((exptnHandlerPtr)NULL != gExptnHandlers.dabtExptnHandler)
    {
        gExptnHandlers.dabtExptnHandler(gExptnHandlers.dabtExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile uint32_t loop = 1U;
        while(1U == loop)
        {

        }
    }
}

/*---------------------------------------------------------------------------*/
