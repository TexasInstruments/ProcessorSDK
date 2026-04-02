/*
 *  Copyright (C) 2018-2021 Texas Instruments Incorporated
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
 *  \file   interrupt_freertos.c
 *
 *  \brief  Interrupt related APIs for FreeRTOS.
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <ti/csl/arch/csl_arch.h>
#include "port_Hwi_priv.h"

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/* compile flag to enable VIC mode of operation, UDEF this to use non-VIC mode */
#define VIM_VIC_ENABLE

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/** \brief Base address of VIM */
extern uint32_t gVimBaseAddr;
extern void       *argArray[R5_VIM_INTR_NUM];
extern IntrFuncPtr fxnArray[R5_VIM_INTR_NUM];
extern uint32_t    intrSrcType[R5_VIM_INTR_NUM];
extern uint16_t    intrPri[R5_VIM_INTR_NUM];
extern uint8_t     intrMap[R5_VIM_INTR_NUM];
extern CSL_R5ExptnHandlers gExptnHandlers;
extern volatile uint32_t gCurrentProcessorState;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  Below ifdef __cplusplus is added so that C++ build passes without
 *  typecasting. This is because the prototype is build as C type
 *  whereas this file is build as CPP file. Hence we get C++ build error.
 *  Also if typecasting is used, then we get MisraC error Rule 11.1.
 */
#ifdef __cplusplus
extern "C" {
#endif
static void IntDefaultHandler(void *dummy);
#ifdef __cplusplus
}
#endif

extern void vPortDumpExceptionState( void );

static void IntDefaultHandler(void *dummy)
{
    volatile bool loop = BTRUE;
    while(loop)
    {

    }
}

void Intc_IntRegister(uint16_t intrNum, IntrFuncPtr fptr, void *fun_arg)
{
    if( intrNum < R5_VIM_INTR_NUM )
    {
        fxnArray[intrNum] = fptr;
        argArray[intrNum] = fun_arg;

        if(CSL_VIM_INTR_MAP_FIQ == intrMap[intrNum])
        {
            CSL_vimCfgIntr( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, intrNum, intrPri[intrNum], (CSL_VimIntrMap)intrMap[intrNum], (CSL_VimIntrType)intrSrcType[intrNum], (uint32_t)&HwiP_fiq_handler );
        }
        else
        {
            CSL_vimCfgIntr( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, intrNum, intrPri[intrNum], (CSL_VimIntrMap)intrMap[intrNum], (CSL_VimIntrType)intrSrcType[intrNum], (uint32_t)&HwiP_irq_handler );
        }

    }
}

void Intc_IntUnregister(uint16_t intrNum)
{
    if( intrNum < R5_VIM_INTR_NUM )
    {
        fxnArray[intrNum] = &IntDefaultHandler;
        argArray[intrNum] = NULL;
        CSL_vimCfgIntr( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, intrNum, 0, CSL_VIM_INTR_MAP_IRQ, CSL_VIM_INTR_TYPE_PULSE, (uint32_t)&HwiP_irq_handler );
    }
}

/* IRQ handler starts execution in HwiP_irq_handler, defined in portASM.S 
 * After some initial assembly logic it then branches to this function.
 * After exiting this function it does some more assembly including 
 * doing task switch if requested.
 */
void __attribute__((section(".text.hwi"))) HwiP_irq_handler_c(void)
{
    int32_t  status;
    uint32_t intNum;
    uint32_t priority;

    CSL_vimRegs     *pVimRegs;
    
    /* initialize the address */
    pVimRegs        = (CSL_vimRegs *)(uintptr_t)gVimBaseAddr;
    
#ifndef VIM_VIC_ENABLE
    volatile uint32_t dummy;
    /* Read to force prioritization logic to take effect */
    dummy = CSL_vimGetIrqVectorAddress(pVimRegs);
#endif

    status = CSL_vimGetActivePendingIntr(pVimRegs,
                                        CSL_VIM_INTR_MAP_IRQ,
                                        &intNum,
                                        &priority);
    if(CSL_PASS == status)
    {
        int32_t intrType = CSL_vimGetIntrType(pVimRegs, intNum);
        IntrFuncPtr isr;
        void *args;

        if((int32_t)CSL_VIM_INTR_TYPE_PULSE == intrType)
        {
            CSL_vimClrIntrPending(pVimRegs, intNum);
        }

        isr = fxnArray[intNum];
        args = argArray[intNum];

        /* allow nesting of interrupts */
        //CSL_armR5IntrEnableIrq(1);  /* Enable IRQ interrupt in R5 */

        if(NULL != isr)
        {
            isr(args);
        }

        /* disallow nesting of interrupts */
        //CSL_armR5IntrEnableIrq(0);  /* Disbale IRQ interrupt in R5 */

        if((int32_t)CSL_VIM_INTR_TYPE_LEVEL == intrType)
        {
            CSL_vimClrIntrPending(pVimRegs, intNum);
        }
        CSL_vimAckIntr(pVimRegs, CSL_VIM_INTR_MAP_IRQ);
    }
    else
    {
        /* spurious interrupt */
        CSL_vimAckIntr(pVimRegs, CSL_VIM_INTR_MAP_IRQ);
    }
}

void __attribute__((section(".text.hwi"))) HwiP_fiq_handler_c(void)
{
    int32_t status;
    uint32_t intNum;
    uint32_t priority;

    CSL_vimRegs     *pVimRegs;
    
    /* initialize the address */
    pVimRegs        = (CSL_vimRegs *)(uintptr_t)gVimBaseAddr;

    /* Read to force prioritization logic to take effect */
    CSL_vimGetFiqVectorAddress(pVimRegs);

    status = CSL_vimGetActivePendingIntr(pVimRegs,
                                        CSL_VIM_INTR_MAP_FIQ,
                                        &intNum,
                                        &priority);
    if(CSL_PASS == status)
    {
        int32_t intrType = CSL_vimGetIntrType(pVimRegs, intNum);
        IntrFuncPtr isr;
        void *args;

        if((int32_t)CSL_VIM_INTR_TYPE_PULSE == intrType)
        {
            CSL_vimClrIntrPending(pVimRegs, intNum);
        }

        isr = fxnArray[intNum];
        args = argArray[intNum];

        /* allow nesting of interrupts */
        CSL_armR5IntrEnableFiq(1);  /* Enable FIQ interrupt in R5 */

        if(NULL != isr)
        {
            isr(args);
        }

        /* disallow nesting of interrupts */
        CSL_armR5IntrEnableFiq(0);  /* Disable FIQ interrupt in R5 */

        if((int32_t)CSL_VIM_INTR_TYPE_LEVEL == intrType)
        {
            CSL_vimClrIntrPending(pVimRegs, intNum);
        }
        CSL_vimAckIntr(pVimRegs, CSL_VIM_INTR_MAP_FIQ);
    }
    else
    {
        /* spurious interrupt */
        CSL_vimAckIntr(pVimRegs, CSL_VIM_INTR_MAP_FIQ);
    }
}

void __attribute__((interrupt("UNDEF"), section(".text.hwi"))) HwiP_reserved_handler(void)
{
    /* Go into an infinite loop.*/
    volatile bool loop = BTRUE;
    while(loop)
    {
    }
}

void __attribute__((section(".text.hwi"))) HwiP_undefined_handler_c(void)
{
    /* Call registered call back */
    gCurrentProcessorState = CSL_ARM_R5_ABORT_MODE;
    vPortDumpExceptionState();
    if ((exptnHandlerPtr)NULL != gExptnHandlers.udefExptnHandler)
    {
        gExptnHandlers.udefExptnHandler(gExptnHandlers.udefExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile bool loop = BTRUE;
        while(loop)
        {
        }
    }
}

void __attribute__((section(".text.hwi"))) HwiP_prefetch_abort_handler_c(void)
{
    /* Call registered call back */
    gCurrentProcessorState = CSL_ARM_R5_ABORT_MODE;
    vPortDumpExceptionState();
    if ((exptnHandlerPtr)NULL != gExptnHandlers.pabtExptnHandler)
    {
        gExptnHandlers.pabtExptnHandler(gExptnHandlers.pabtExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile bool loop = BTRUE;
        while(loop)
        {
        }
    }
}

/* Data Abort handler starts execution in HwiP_data_abort_handler, defined in portASM.S 
 * After some initial assembly logic it then branches to this function.
 * After exiting this function it does some more assembly to return to the next instruction
 * following the one which caused the exception.
 */
void __attribute__((section(".text.hwi"))) HwiP_data_abort_handler_c(void)
{
    /* Call registered call back */
    gCurrentProcessorState = CSL_ARM_R5_ABORT_MODE;
    vPortDumpExceptionState();
    if ((exptnHandlerPtr)NULL != gExptnHandlers.dabtExptnHandler)
    {
        gExptnHandlers.dabtExptnHandler(gExptnHandlers.dabtExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile bool loop = BTRUE;
        while(loop)
        {
        }
    }
}

/********************************* End of file ******************************/
