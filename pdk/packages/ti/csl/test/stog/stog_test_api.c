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
 *  \file     stog_test_api.c
 *
 *  \brief    This file contains STOG functional test code. .
 *
 *  \details  STOG Functional tests
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/csl_tog.h>
#include <ti/csl/soc.h>
#include <ti/drv/uart/UART_stdio.h>

#include "stog_test_cfg.h"

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* Run all APIs not exercised by functional test */
static int32_t STOG_apiTestLocal(uint32_t instanceIndex)
{
    int32_t testResult = 0;
    int32_t status;
    CSL_ksbus_vbusm_to_gasketRegs *pRegMap;
    uint32_t readValue;
    uint32_t readValue1,  readValue2;

    pRegMap = STOG_TestHandleArray[instanceIndex].pRegMap;
    CSL_SlvTog_staticRegs staticRegs;

    /* Call CSL APIs not used by functional test */
    status = CSL_slvTogGetRevision(pRegMap, &readValue);
    if (status != CSL_PASS)
    {
        UART_printf("    CSL_slvTogGetRevision failed\n");
        testResult = -1;
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogGetCfg(pRegMap, &readValue1, &readValue2);
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogGetCfg failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogGetStatus(pRegMap, &readValue1, &readValue2);
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogGetStatus failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogGetIntrPending(pRegMap, &readValue);
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogGetIntrPending failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogGetRawIntrPending(pRegMap, &readValue);
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogGetRawIntrPending failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogSetIntrPending(pRegMap, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT );
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogSetIntrPending failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogClrIntrPending(pRegMap, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT );
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogClrIntrPending failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogSetIntrEnable(pRegMap, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT, true );
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogSetIntrEnable failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogVerifyIntrEnable(pRegMap, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT, true );
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogVerifyIntrEnable failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogGetIntrCount(pRegMap, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT, &readValue );
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogGetIntrCount failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogSetTimeoutVal(pRegMap, 0x10000000U );
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogSetTimeoutVal failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogVerifyTimeoutVal(pRegMap, 0x10000000U );
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogVerifyTimeoutVal failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogStart(pRegMap);
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogStart failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogStop(pRegMap);
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogStop failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogReset(pRegMap);
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogReset failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogGetCurrTimerCnt(pRegMap, &readValue);
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogGetCurrTimerCnt failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogSetFlushModeEnable(pRegMap, false);
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogSetFlushModeEnable failed\n");
            testResult = -1;
        }
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogVerifyFlushModeEnable(pRegMap, false);
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogVerifyFlushModeEnable failed\n");
            testResult = -1;
        }
    }


    if (testResult == CSL_PASS)
    {
        status = CSL_slvTogReadBackStaticRegisters(pRegMap, &staticRegs);
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_slvTogReadBackStaticRegisters failed\n");
            testResult = -1;
        }
    }

    return (testResult);
}

/* STOG Error module test */
int32_t STOG_apiTest(void)
{
    int32_t testResult;

    testResult = STOG_apiTestLocal(0);

    return (testResult);
}
/* Nothing past this point */
