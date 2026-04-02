/*
 *  Copyright ( C ) 2022 Texas Instruments Incorporated
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
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/* SafeRTOS Includes */
#include "SafeRTOS_API.h"
#include "aborts.h"

#include "ti/csl/arch/r5/csl_arm_r5.h"
#include "ti/csl/arch/r5/csl_vim.h"
#include "ti/csl/arch/r5/interrupt.h"

/* Hook function handlers targeting the TI PDK libraries. */


/* Timer interrupt handler function. */
void vApplicationInterruptHandlerHook( void );

/*-------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * TI PDK variables required by the interrupt handler.
 *---------------------------------------------------------------------------*/

extern uint32_t             gVimBaseAddr;
extern uint8_t              intrMap[ R5_VIM_INTR_NUM ];
extern void                 *argArray[ R5_VIM_INTR_NUM ];
extern IntrFuncPtr          fxnArray[ R5_VIM_INTR_NUM ];
extern uint32_t             intrSrcType[ R5_VIM_INTR_NUM ];

/*-----------------------------------------------------------------------------
 * Public function definitions.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Copy of the TI master ISR handler.
 * This function is not compiled as an interrupt handler, so won't change stack
 * or enable interrupts on exit.
 *---------------------------------------------------------------------------*/

void vApplicationInterruptHandlerHook( void )
{
    IntrFuncPtr        fxnPtr;
    volatile uint32_t  intNum;

    /* Read to force prioritization logic to take effect */
    CSL_vimGetIrqVectorAddress( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr);

    /* Process a pending FIQ interrupt before a pending IRQ interrupt */
    if( ( CSL_vimGetActivePendingIntr( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, CSL_VIM_INTR_MAP_FIQ, (uint32_t *)&intNum, (uint32_t *)0 ) == 0 )       ||
        ( CSL_vimGetActivePendingIntr( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, CSL_VIM_INTR_MAP_IRQ, (uint32_t *)&intNum, (uint32_t *)0 ) == 0 ) )
    {
        /* Clear pulse-type interrupt before calling ISR */
        if( intrSrcType[intNum] == (uint32_t)CSL_VIM_INTR_TYPE_PULSE )
        {
            CSL_vimClrIntrPending( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, intNum );
        }
        /* Call ISR */
        if( fxnArray[intNum] != NULL )
        {
            fxnPtr = fxnArray[intNum];
            fxnPtr(argArray[intNum]);
        }
        /* Clear level-type interrupt after calling ISR */
        if( intrSrcType[intNum] == (uint32_t)CSL_VIM_INTR_TYPE_LEVEL )
        {
            CSL_vimClrIntrPending( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, intNum );
        }
        /* Acknowledge interrupt servicing */
        CSL_vimAckIntr( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, (CSL_VimIntrMap)intrMap[intNum] );
    }
}

/*-------------------------------------------------------------------------*/
