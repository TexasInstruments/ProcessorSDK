/*
 *  Copyright (C) 2017-2018 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file   interrupt.c
 *
 *  \brief  Interrupt related common APIs for Nonos and FreeRTOS.
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <interrupt.h>
#include <csl_arm_r5.h>
#include <csl_types.h>
#include <csl_vim.h>
#include "interrupt_priv.h"

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
/* None */


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Current Process Status */
volatile uint32_t gCurrentProcessorState=CSL_ARM_R5_SYS_MODE;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

__attribute__((section(".text.hwi"), aligned(32)))      void masterIsr_c(void);
__attribute__((section(".text.hwi"), aligned(32)))      void undefInstructionExptnHandler_c(void);
__attribute__((section(".text.hwi"), aligned(32)))      void swIntrExptnHandler_c(void);
__attribute__((section(".text.hwi"), aligned(32)))      void prefetchAbortExptnHandler_c(void);
__attribute__((section(".text.hwi"), aligned(32)))      void dataAbortExptnHandler_c(void);
__attribute__((section(".text.hwi"), aligned(32)))      void irqExptnHandler_c(void);
__attribute__((section(".text.hwi"), aligned(32)))      void fiqExptnHandler_c(void);

void masterIsr_c(void)
{
    IntrFuncPtr        fxnPtr;
    volatile uint32_t  intNum, irqNum, fiqNum;
    int32_t            irqStat, fiqStat, processInt = 0;
     
    fiqStat = CSL_vimGetActivePendingIntr((CSL_vimRegs *)gVimBaseAddr, CSL_VIM_INTR_MAP_FIQ,
                (uint32_t *)&fiqNum, (uint32_t *)0);
    irqStat = CSL_vimGetActivePendingIntr( (CSL_vimRegs *)gVimBaseAddr, CSL_VIM_INTR_MAP_IRQ, 
                (uint32_t *)&irqNum, (uint32_t *)0);   

    if (fiqStat == 0)
    {
        intNum = fiqNum;
        processInt = 1;
    }
    else if (irqStat == 0)
    {
        intNum = irqNum;
        processInt = 1;
    }
    else
    {
        /* Do Nothing */
    }

    /* Process a pending FIQ interrupt before a pending IRQ interrupt */
    if(1 == processInt)
    {
        /* Clear pulse-type interrupt before calling ISR */
        if(CSL_VIM_INTR_TYPE_PULSE == intrSrcType[intNum])
        {
            CSL_vimClrIntrPending( (CSL_vimRegs *)gVimBaseAddr, intNum );
        }
        /* Call ISR */
        if(NULL != fxnArray[intNum])
        {
            fxnPtr = fxnArray[intNum];
            fxnPtr(argArray[intNum]);
        }
        /* Clear level-type interrupt after calling ISR */
        if(CSL_VIM_INTR_TYPE_LEVEL == intrSrcType[intNum])
        {
            CSL_vimClrIntrPending( (CSL_vimRegs *)gVimBaseAddr, intNum );
        }
        /* Acknowledge interrupt servicing */
        CSL_vimAckIntr( (CSL_vimRegs *)gVimBaseAddr, (CSL_VimIntrMap)intrMap[intNum] );
    }
}


void undefInstructionExptnHandler_c(void)
{
    /* Call registered call back */
    if (gExptnHandlers.udefExptnHandler != (exptnHandlerPtr)NULL)
    {
        gExptnHandlers.udefExptnHandler(gExptnHandlers.udefExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile uint32_t loop = 1U;
        while (1U == loop)
        {
            ;
        }
    }
}

void swIntrExptnHandler_c(void)
{
    /* Call registered call back */
    if (gExptnHandlers.swiExptnHandler != (exptnHandlerPtr)NULL)
    {
        gExptnHandlers.swiExptnHandler(gExptnHandlers.swiExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile uint32_t loop = 1U;
        while (1U == loop)
        {
            ;
        }
    }
}

void prefetchAbortExptnHandler_c(void)
{
    gCurrentProcessorState=CSL_ARM_R5_ABORT_MODE;
    /* Call registered call back */
    if (gExptnHandlers.pabtExptnHandler != (exptnHandlerPtr)NULL)
    {
        gExptnHandlers.pabtExptnHandler(gExptnHandlers.pabtExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile uint32_t loop = 1U;
        while (1U == loop)
        {
            ;
        }
    }
}

void dataAbortExptnHandler_c(void)
{
    gCurrentProcessorState=CSL_ARM_R5_ABORT_MODE;
    /* Call registered call back */
    if (gExptnHandlers.dabtExptnHandler != (exptnHandlerPtr)NULL)
    {
        gExptnHandlers.dabtExptnHandler(gExptnHandlers.dabtExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile uint32_t loop = 1U;
        while (1U == loop)
        {
            ;
        }
    }
}

void irqExptnHandler_c(void)
{
    /* Call registered call back */
    if (gExptnHandlers.irqExptnHandler != (exptnHandlerPtr)NULL)
    {
        gExptnHandlers.irqExptnHandler(gExptnHandlers.irqExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile uint32_t loop = 1U;
        while (1U == loop)
        {
            ;
        }
    }
}

void fiqExptnHandler_c(void)
{
    /* Call registered call back */
    if ((exptnHandlerPtr)NULL != gExptnHandlers.fiqExptnHandler)
    {
        gExptnHandlers.fiqExptnHandler(gExptnHandlers.fiqExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile uint32_t loop = 1U;
        while (1U == loop)
        {
            ;
        }
    }
}

/********************************* End of file ******************************/


