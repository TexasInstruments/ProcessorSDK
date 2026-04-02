/*
 * SDR TEST
 *
 * Software Diagnostics Reference Test
 *
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
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

#include "sdtf_common.h"
#include "sdr_exception.h"

#include <ti/csl/arch/r5/interrupt.h>

/* This is the list of exception handle and the parameters */
const CSL_R5ExptnHandlers SDTF_R5ExptnHandlers =
{
    .udefExptnHandler = &SDR_EXCEPTION_undefInstructionExptnHandler,
    .swiExptnHandler = &SDR_EXCEPTION_swIntrExptnHandler,
    .pabtExptnHandler = &SDR_EXCEPTION_prefetchAbortExptnHandler,
    .dabtExptnHandler = &SDR_EXCEPTION_dataAbortExptnHandler,
    .irqExptnHandler = &SDR_EXCEPTION_irqExptnHandler,
    .fiqExptnHandler = &SDR_EXCEPTION_fiqExptnHandler,
    .udefExptnHandlerArgs = ((void *)0u),
    .swiExptnHandlerArgs = ((void *)0u),
    .pabtExptnHandlerArgs = ((void *)0u),
    .dabtExptnHandlerArgs = ((void *)0u),
    .irqExptnHandlerArgs = ((void *)0u),
};

void SDTF_undefInstructionExptnCallback(void)
{
    SDTF_printf("\n Undefined Instruction exception");
}

void SDTF_swIntrExptnCallback(void)
{
    SDTF_printf("\n Software interrupt exception");
}

void SDTF_prefetchAbortExptnCallback(void)
{
    SDTF_printf("\n Prefetch Abort exception");
}
void SDTF_dataAbortExptnCallback(void)
{
    SDTF_printf("\n Data Abort exception");
}
void SDTF_irqExptnCallback(void)
{
    SDTF_printf("\n Irq exception");
}

void SDTF_fiqExptnCallback(void)
{
    SDTF_printf("\n Fiq exception");
}

void SDTF_exceptionInit(void)
{

    SDR_EXCEPTION_CallbackFunctions_t exceptionCallbackFunctions =
            {
             .udefExptnCallback = SDTF_undefInstructionExptnCallback,
             .swiExptnCallback = SDTF_swIntrExptnCallback,
             .pabtExptnCallback = SDTF_prefetchAbortExptnCallback,
             .dabtExptnCallback = SDTF_dataAbortExptnCallback,
             .irqExptnCallback = SDTF_irqExptnCallback,
             .fiqExptnCallback = SDTF_fiqExptnCallback,
            };

    /* Initialize SDR exception handler */
    SDR_EXCEPTION_init(&exceptionCallbackFunctions);
    /* Register SDR exception handler */
    Intc_RegisterExptnHandlers(&SDTF_R5ExptnHandlers);

    return;
}

int32_t SDTF_runExceptionApiTests(void)
{
    void *param = NULL;

    SDTF_printf("\n Exception API tests: starting");

    /* SDR_EXCEPTION_undefInstructionExptnHandler API test */
    SDR_EXCEPTION_undefInstructionExptnHandler(param);

    /* SDTF_swIntrExptnCallback API test */
    SDR_EXCEPTION_swIntrExptnHandler(param);

    /* SDTF_prefetchAbortExptnCallback API test */
    SDR_EXCEPTION_prefetchAbortExptnHandler(param);

    /* SDTF_dataAbortExptnCallback API test */
    SDR_EXCEPTION_dataAbortExptnHandler(param);

    /* SDTF_irqExptnCallback API test */
    SDR_EXCEPTION_irqExptnHandler(param);

    /* SDTF_fiqExptnCallback API test */
    SDR_EXCEPTION_fiqExptnHandler(param);

    SDTF_printf("\n Exception API tests complete");
    return 0;
}
