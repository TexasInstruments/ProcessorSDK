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

#include <stdio.h>
#include "port_Hwi.h"
#include <FreeRTOS.h>
#include <task.h>
#include <ti/csl/src/intc/csl_intc.h>
#include <ti/csl/src/intc/_csl_intc.h>
#include "portmacro.h"

/* --> __TI_STATIC_BASE */
extern void* __TI_STATIC_BASE;
extern void* _Hwi_intcVectorTable;

ti_sysbios_family_c64p_Hwi_Module_State__ ti_sysbios_family_c64p_Hwi_Module__state__V = {
    (uint16_t)0x4003,  /* ierMask */
    (int32_t)0x0,  /* intNum */
    ((char*)NULL),  /* taskSP */
    ((char*)NULL),  /* isrStack */
    ((void *)((void*)&_Hwi_intcVectorTable)),  /* vectorTableBase */
    ((void *)((void*)&__TI_STATIC_BASE)),  /* bss */
    (int32_t)0x0,  /* scw */
};

extern void ti_sysbios_family_c64p_Hwi_switchAndDispatch__I(int32_t intNum);


void ti_sysbios_family_c64p_Hwi_dispatchC__I(int32_t intNum)
{

    extern BaseType_t ulPortInterruptNesting;
    extern BaseType_t ulPortYieldRequired;

    ulPortInterruptNesting++;
    ti_sysbios_family_c64p_Hwi_switchAndDispatch__I(intNum);

    ulPortInterruptNesting--;
    if (pdFALSE == ulPortInterruptNesting)
    {
        if (pdFALSE != ulPortYieldRequired)
        {
            ulPortYieldRequired = pdFALSE;
            vPortYieldAsyncFromISR();
        }
    
    }
}

typedef void (CslIntDispatchFxn_t)(void);

typedef struct CslIntDispatchTable_s
{
    CslIntDispatchFxn_t * intHandler;
    CslIntDispatchFxn_t * intHandlerCore;
} CslIntDispatchTable_t;

static const CslIntDispatchTable_t intDispatchTable[] =
{
    [0] = 
    {
        .intHandler     = &CSL_intcDispatcher,
        .intHandlerCore = &CSL_intcDispatcherCore,
    },
    [1] = 
    {
        .intHandler     = &CSL_intcEvent0Dispatcher,
        .intHandlerCore = &CSL_intcEvent0DispatcherCore,
    },
    [2] = 
    {
        .intHandler     = &CSL_intcEvent1Dispatcher,
        .intHandlerCore = &CSL_intcEvent1DispatcherCore,
    },
    [3] = 
    {
        .intHandler     = &CSL_intcEvent2Dispatcher,
        .intHandlerCore = &CSL_intcEvent2DispatcherCore,
    },
    [4] = 
    {
        .intHandler     = &CSL_intcEvent3Dispatcher,
        .intHandlerCore = &CSL_intcEvent3DispatcherCore,
    },
};

void ti_sysbios_family_c64p_Hwi_dispatchCore__I(int32_t intNum)
{
    CslIntDispatchFxn_t * intDispatch;

    uintptr_t dispatchAddress;
    uint32_t *cpuIntrTable = (uint32_t *)&CSL_intcCpuIntrTable;
    uint32_t i;

    dispatchAddress = (uintptr_t)(cpuIntrTable + (1 + intNum));
    CSL_intcCpuIntrTable.currentVectId = dispatchAddress;
    intDispatch = * ((CslIntDispatchFxn_t ** ) dispatchAddress);
    for (i = 0U; i < sizeof(intDispatchTable)/sizeof(intDispatchTable[0]); i++)
    {
        if (intDispatchTable[i].intHandler == intDispatch)
        {
            break;
        }
    }
    DebugP_assert(i < sizeof(intDispatchTable)/sizeof(intDispatchTable[0]));
    intDispatchTable[i].intHandlerCore();
}



/*
 *  ======== Hwi_getIsrStackAddress ========
 */
Char *Hwi_getIsrStackAddress()
{
    UInt32 isrStack;
    static StackType_t uxHwiStack[ configHWI_TASK_STACK_DEPTH ];

    isrStack = (UInt32)uxHwiStack;
    isrStack += (UInt32)sizeof(uxHwiStack);

    isrStack &= ~0xF;   /* align to long word */

    return ((Char *)isrStack);
}

/*
 *  ======== Hwi_Module_startup ========
 */
void Hwi_Module_startup(void)
{
    /*
     * Initialize the pointer to the isrStack. These symbols are part of the
     * Hwi_module (instead of floating) in order to support ROM.
     * Leave room for one 32-bit value pushed by xdc_runtime_Startup_reset()
     * (for cases where intentionally reset as resume from power down),
     * and maintain double word alignment.
     */
    ti_sysbios_family_c64p_Hwi_Module__state__V.isrStack = Hwi_getIsrStackAddress() - 8;

    ti_sysbios_family_c64p_Hwi_Module__state__V.taskSP = (Char *)-1;/* signal that we're executing on the */
                                        /* ISR stack */
    /* start with a clean slate after initializing INTMUX */
    //ICR = 0xffff;

    //Hwi_enableIER(Hwi_module->ierMask); /* IER per static Hwi settings */

}

/*
 *  ======== switchFromBootStack ========
 *  Indicate that we are leaving the boot stack and
 *  are about to switch to a task stack.
 */
void Hwi_switchFromBootStack(void)
{
    ti_sysbios_family_c64p_Hwi_Module__state__V.taskSP = 0;
}

bool Hwi_isInIsr()
{
    if (((Char *)0  != ti_sysbios_family_c64p_Hwi_Module__state__V.taskSP) &&
        ((Char *)-1 != ti_sysbios_family_c64p_Hwi_Module__state__V.taskSP))
    {
        return BTRUE;
    }
    else
    {
        return BFALSE;
    }
}

