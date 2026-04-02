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
 *  \file     stog_test_err.c
 *
 *  \brief    This file contains Slave TOG module error tests.
 *
 *  \details  Slave TOG Error tests
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/csl/csl_types.h>
#include <ti/csl/csl_tog.h>
#include <ti/drv/uart/UART_stdio.h>

#include "stog_test_cfg.h"

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static int32_t STOG_errNegativeTest(uint32_t instanceIndex)
{
    int32_t testResult = 0;
    int32_t cslRet;
    CSL_ksbus_vbusm_to_gasketRegs *pRegMap;
    volatile uint32_t readValue;
    uint32_t readValue1,  readValue2;
    CSL_SlvTogErrInfo errInfo;

    pRegMap = STOG_TestHandleArray[instanceIndex].pRegMap;

    /* Call CSL API */
    cslRet = CSL_slvTogGetRevision(NULL, NULL);
    if (cslRet == CSL_PASS)
    {
        UART_printf("\n  CSL_slvTogGetRevision negative test failed on line no: %d \n", __LINE__);
        testResult = -1;
    }

    if (testResult == 0)
    {

        cslRet = CSL_slvTogGetRevision(pRegMap, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetRevision negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {

        cslRet = CSL_slvTogGetRevision(NULL, (uint32_t *)&readValue);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetRevision negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetCfg(NULL, NULL, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetCfg negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetCfg(NULL, &readValue1, &readValue2);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetCfg negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetCfg(pRegMap, NULL, &readValue2);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetCfg negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetCfg(pRegMap, &readValue1, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetCfg negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetStatus(NULL, NULL, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetStatus negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetStatus(NULL, &readValue1, &readValue2);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetStatus negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetStatus(pRegMap, NULL, &readValue2);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetStatus negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetStatus(pRegMap, &readValue1, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetStatus negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetErrInfo(NULL, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetErrInfo negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetErrInfo(pRegMap, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetErrInfo negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetErrInfo(NULL, (CSL_SlvTogErrInfo *)&readValue);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetErrInfo negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        /* Getting the error when there is no actual error */
        cslRet = CSL_slvTogGetErrInfo(pRegMap, &errInfo);
        if (cslRet != CSL_EFAIL)
        {
            UART_printf("\n  CSL_slvTogGetErrInfo negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetIntrPending(NULL, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetIntrPending negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetIntrPending(pRegMap, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetIntrPending negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetIntrPending(NULL, (uint32_t *)&readValue);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetIntrPending negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetRawIntrPending(NULL, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetRawIntrPending negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetRawIntrPending(pRegMap, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetRawIntrPending negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetRawIntrPending(NULL, (uint32_t *)&readValue);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetRawIntrPending negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogSetIntrPending(NULL,CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogSetIntrPending negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogSetIntrPending(NULL, 9U);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogSetIntrPending negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogClrIntrPending(NULL,CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogClrIntrPending negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogClrIntrPending(pRegMap, 9U);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogClrIntrPending negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogClrIntrPending(NULL,CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogClrIntrPending negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogClrIntrPending(pRegMap, 9U);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogClrIntrPending negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogSetIntrEnable(NULL,CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT, true);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogSetIntrEnable negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogSetIntrEnable(pRegMap, 9U, true);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogSetIntrEnable negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogVerifyIntrEnable(NULL,CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT, true);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogVerifyIntrEnable negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogVerifyIntrEnable(pRegMap, 9U, true);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogVerifyIntrEnable negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetIntrCount(NULL, 5U, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetIntrCount negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetIntrCount(NULL, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT, (uint32_t *)&readValue);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetIntrCount negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetIntrCount(pRegMap, 5U, (uint32_t *)&readValue);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetIntrCount negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetIntrCount(pRegMap, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetIntrCount negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogAckIntr(NULL, 5U, 0U);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogAckIntr negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogAckIntr(NULL, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT, 1U);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogAckIntr negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        /* Try to Ack interrupt when there is not actual interrupt event */
        cslRet = CSL_slvTogAckIntr(pRegMap, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT, 1U );
        if (cslRet != CSL_EFAIL)
        {
            UART_printf("\n  CSL_slvTogAckIntr negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogAckIntr(pRegMap, 5U, 1U);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogAckIntr negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogAckIntr(pRegMap, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT, 0U);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogAckIntr negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogSetTimeoutVal(NULL, 10U);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogSetTimeoutVal negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogVerifyTimeoutVal(NULL, 10U);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogVerifyTimeoutVal negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogStart(NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogStart negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogStop(NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogStop negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogReset(NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogReset negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetCurrTimerCnt(NULL, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetCurrTimerCnt negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetCurrTimerCnt(NULL, (uint32_t *)&readValue);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetCurrTimerCnt negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogGetCurrTimerCnt(pRegMap, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogGetCurrTimerCnt negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogSetFlushModeEnable(NULL, true);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogSetFlushModeEnable negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogVerifyFlushModeEnable(NULL, true);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogVerifyFlushModeEnable negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogReadBackStaticRegisters(NULL, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogReadBackStaticRegisters negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogReadBackStaticRegisters(pRegMap, NULL);
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogReadBackStaticRegisters negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    if (testResult == 0)
    {
        cslRet = CSL_slvTogReadBackStaticRegisters(NULL, (CSL_SlvTog_staticRegs *)(&readValue));
        if (cslRet == CSL_PASS)
        {
            UART_printf("\n  CSL_slvTogReadBackStaticRegisters negative test failed on line no: %d \n", __LINE__);
            testResult = -1;
        }
    }

    return (testResult);
}


/* STOG Error module test */
int32_t STOG_errTest(void)
{
    int32_t testResult;

    testResult = STOG_errNegativeTest(0U);

    return (testResult);
}

/* Nothing past this point */
