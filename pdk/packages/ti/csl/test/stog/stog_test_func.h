/*
 *   Copyright (c) Texas Instruments Incorporated 2020
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
 *  \file     stog_test_func.h
 *
 *  \brief    This file contains STOG test function structures
 *
 *  \details  Slave TOG Test function structures
 **/
#ifndef STOG_TEST_FUNC_H
#define STOG_TEST_FUNC_H

#ifdef __cplusplus
extern "C"
{
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl_tog.h>
#include <ti/csl/soc.h>

/* ========================================================================== */
/*                                Data Structures                             */
/* ========================================================================== */

typedef void (*STOG_handlerPtr)(uint32_t instanceIndex);
typedef void (*STOG_injectFunction)(uint32_t instanceIndex);


typedef struct STOG_TestHandle_s
{
    /** Instance name */
    char instanceName[16];
    /** Pointer to STOG register map */
    CSL_ksbus_vbusm_to_gasketRegs *pRegMap;
    /** STOG event handler function */
    STOG_handlerPtr handler;
    /** STOG error inject function */
    STOG_injectFunction injectFunction;
    /** Interrupt number for error event */
    uint32_t interruptNumber;
    /** ESM Instance to be used */
    uint32_t ESMInstance;
    /** ESM error event number */
    uint32_t ESMEventNumber;
    /** Flag to indicate test done, will be set when interrupt event comes */
    volatile bool doneFlag;
    /** Timeout configuration for test */
    uint32_t timeoutValue;
    /** Interrupt source for test */
    uint32_t intSrcBitmap;
    /** Flush mode for test */
    Bool flushMode;
} STOG_TestHandle_t;

__attribute__((section(".text:STOG_test"))) void STOG_eventHandler(uint32_t instanceIndex);

#ifdef __cplusplus
}
#endif

#endif /* STOG_TEST_FUNC_H */

/* Nothing past this point */
