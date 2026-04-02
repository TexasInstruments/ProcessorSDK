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
 *  \file     stog_test_func.c
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
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/sciclient/sciclient.h>

/* Osal API header files */
#include <ti/osal/HwiP.h>
#include <ti/osal/TimerP.h>

#include "stog_test_cfg.h"
#include "esm_app.h"

#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
void STOG_datAbortExceptionHandler(void *param);

__attribute__((section(".text:STOG_test"))) int32_t STOG_runTest(uint32_t instanceIndex);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
const CSL_R5ExptnHandlers STOG_R5ExptnHandlers =
{
    .udefExptnHandler = NULL,
    .swiExptnHandler = NULL,
    .pabtExptnHandler = NULL,
    .dabtExptnHandler = &STOG_datAbortExceptionHandler,
    .irqExptnHandler = NULL,
    .fiqExptnHandler = NULL,
    .udefExptnHandlerArgs = ((void *)0u),
    .swiExptnHandlerArgs = ((void *)0u),
    .pabtExptnHandlerArgs = ((void *)0u),
    .dabtExptnHandlerArgs = ((void *)0u),
    .irqExptnHandlerArgs = ((void *)0u),
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void STOG_eventHandler( uint32_t instanceIndex )
{
    int32_t status = CSL_PASS;
    uint32_t pendInts;
    uint32_t intCount;
    CSL_ksbus_vbusm_to_gasketRegs *pRegMap = STOG_TestHandleArray[instanceIndex].pRegMap;
    CSL_SlvTogErrInfo errInfo;

    if (STOG_TestHandleArray[instanceIndex].intSrcBitmap != 0U)
    {
        /* Read error info */
        status = CSL_slvTogGetErrInfo(pRegMap, &errInfo);
        TEST_ASSERT_EQUAL_INT32(CSL_PASS, status);
    }
    
    if (STOG_TestHandleArray[instanceIndex].intSrcBitmap
            & CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT)
    {
        /* Get Transaction timeout interrupt count */
        if (status == CSL_PASS)
        {
            status = CSL_slvTogGetIntrCount(pRegMap, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT, &intCount);
            TEST_ASSERT_EQUAL_INT32(CSL_PASS, status);
        }

        /* Clear Transaction timeout interrupt events */
        if ((status == CSL_PASS) && (intCount != 0))
        {
            status = CSL_slvTogAckIntr(pRegMap, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT, intCount);
            TEST_ASSERT_EQUAL_INT32(CSL_PASS, status);
        }
    }

    if (STOG_TestHandleArray[instanceIndex].intSrcBitmap
            & CSL_SLV_TOG_INTRSRC_UNEXPECTED_RESPONSE)
    {
        /* Get Unexpected Response interrupt count */
        if (status == CSL_PASS)
        {
            status = CSL_slvTogGetIntrCount(pRegMap, CSL_SLV_TOG_INTRSRC_UNEXPECTED_RESPONSE, &intCount);

            TEST_ASSERT_EQUAL_INT32(CSL_PASS, status);
        }

        /* Clear Unexpected response interrupt events */
        if ((status == CSL_PASS) && (intCount != 0))
        {
            status = CSL_slvTogAckIntr(pRegMap, CSL_SLV_TOG_INTRSRC_UNEXPECTED_RESPONSE, intCount);
            TEST_ASSERT_EQUAL_INT32(CSL_PASS, status);
        }
    }

    /* Get Pending interrupt count */
    if (status == CSL_PASS)
    {
        status = CSL_slvTogGetIntrPending(pRegMap, &pendInts );
        TEST_ASSERT_EQUAL_INT32(CSL_PASS, status);
    }

    /* Clear Pending interrupt */
    if (status == CSL_PASS)
    {
        status = CSL_slvTogClrIntrPending(pRegMap, CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT);
        TEST_ASSERT_EQUAL_INT32(CSL_PASS, status);
    }

    if (status == CSL_PASS)
    {
        status = CSL_slvTogClrIntrPending(pRegMap, CSL_SLV_TOG_INTRSRC_UNEXPECTED_RESPONSE);
        TEST_ASSERT_EQUAL_INT32(CSL_PASS, status);
    }

    if (status == CSL_PASS)
    {
        STOG_TestHandleArray[instanceIndex].doneFlag = true;
        /* Call CSL API to configure back Timeout Gasket */
        status = CSL_slvTogSetTimeoutVal(pRegMap, STOG_TestHandleArray[instanceIndex].timeoutValue);
        TEST_ASSERT_EQUAL_INT32(CSL_PASS, status);

        /* Stop the Timeout Gasket */
        CSL_slvTogStop( pRegMap );

        /* Reset the Timeout gasket */
        CSL_slvTogReset( pRegMap );

    }

    return;
}

int32_t STOG_runTest(uint32_t instanceIndex)
{
    int32_t testResult = 0;
    int32_t status;
    CSL_ksbus_vbusm_to_gasketRegs *pRegMap;
    uint64_t startTime , testStartTime,  testEndTime, endTime;
    uint64_t prepTime, diffTime, restoreTime;
    volatile uint32_t timeoutCount = 0;

    UART_printf("\n Starting STOG test on %s, index %d...",
                STOG_TestHandleArray[instanceIndex].instanceName,
                instanceIndex);

#ifdef DEBUG
    char inputChar;

    UART_printf("\n Press 'n' to skip..Press any key to continue...");
    inputChar = UART_getc();

    if (inputChar == 'n')
    {
        UART_printf("   Skipping this test. on request \n");
        return 0;
    }
#endif
    /* Populate local variables from instance structure */
    pRegMap = STOG_TestHandleArray[instanceIndex].pRegMap;


#ifdef DEBUG
    UART_printf("\n  HwiP_Params_init complete \n");
#endif

    /* Initialize done flag */
    STOG_TestHandleArray[instanceIndex].doneFlag = false;

    /* Get start time of test */
    startTime = TimerP_getTimeInUsecs();

    /*-- Step 1: Configure ESM handler --*/
    status = cslAppEsmConfig(MCU_ESM_BASE,
                             STOG_TestHandleArray[instanceIndex].ESMEventNumber,
                             ESM_INTR_PRIORITY_LEVEL_HIGH,
                             STOG_TestHandleArray[instanceIndex].handler,
                             instanceIndex);
    if (status != CSL_PASS)
    {
        UART_printf("   cslAppEsmConfig Failed \n");
        testResult = -1;
    }

    if (testResult == 0)
    {
        /* Enable interrupts */
        status = CSL_slvTogSetIntrEnable(pRegMap, CSL_SLV_TOG_INTRSRC_ALL, true);
        if (status != CSL_PASS)
        {
            UART_printf("   CSL_slvTogSetIntrEnable Failed \n");
            testResult = -1;
        }
    }

    /** Step 2: Configure and enable Timeout Gasket */
    if (testResult == 0)
    {
        /* Call CSL API to configure Timeout Gasket */
        status = CSL_slvTogSetTimeoutVal(pRegMap, STOG_TestHandleArray[instanceIndex].timeoutValue);
        if (status != CSL_PASS)
        {
            UART_printf("   CSL_slvTogSetTimeoutVal Failed \n");
            testResult = -1;
        }
    }

    if (testResult == 0)
    {

        /* Call CSL API to Verify Timeout configuration */
        status = CSL_slvTogVerifyTimeoutVal(pRegMap, STOG_TestHandleArray[instanceIndex].timeoutValue);
        if (status != CSL_PASS)
        {
            UART_printf("   CSL_slvTogVerifyTimeoutVal Failed \n");
            testResult = -1;
        }

    }

    if (testResult == 0)
    {
        /* Call CSL API to enable Timeout Gasket */
        status = CSL_slvTogStart(pRegMap);
        if (status != CSL_PASS)
        {
            UART_printf("   CSL_slvTogStart Failed \n");
            testResult = -1;
        }
    }
    /* Get start time of test */
    testStartTime = TimerP_getTimeInUsecs();

    /* Step 3: Inject timeout error */
    if (testResult == 0)
    {
        STOG_TestHandleArray[instanceIndex].injectFunction(instanceIndex);
    }

    /**--- Step 3: Wait for STOG Interrupt ---*/
    if (testResult == 0)
    {
        /* Timeout if exceeds time */
        while ((!STOG_TestHandleArray[instanceIndex].doneFlag)
               && (timeoutCount++ < STOG_MAX_TEST_TIMEOUT_VALUE))
        {
            /* Interrupt handler available */
            if (STOG_TestHandleArray[instanceIndex].handler == NULL)
            {
                /* Use Polling */
                STOG_eventHandler(instanceIndex);
            }
        }

        if (!(STOG_TestHandleArray[instanceIndex].doneFlag))
        {
            CSL_slvTogStop( pRegMap );
            UART_printf("   STOG test timed out \n");
            testResult = -1;
        }
        /* reset Done flag so we can run again */
        STOG_TestHandleArray[instanceIndex].doneFlag = false;
    }

    /* Get end time of test */
    testEndTime = TimerP_getTimeInUsecs();

    /**--- Step 4: Disable ESM ---*/
    if (testResult == 0)
    {
        status = cslAppEsmDisable(MCU_ESM_BASE,
                            STOG_TestHandleArray[instanceIndex].ESMEventNumber);
        if (status != CSL_PASS)
        {
            UART_printf("   cslAppEsmDisable Failed \n");
            testResult = -1;
        }
    }

    /* Here STOG test is complete , get end time of test */
    endTime = TimerP_getTimeInUsecs();

    prepTime = testStartTime - startTime;
    diffTime = testEndTime - testStartTime;
    restoreTime = endTime - testEndTime;
    UART_printf("  Delta STOG prep time in micro secs %d \n", (uint32_t)prepTime );
    UART_printf("  Delta STOG execution time in micro secs %d \n", (uint32_t)diffTime );
    UART_printf("  Delta STOG restore time in micro secs %d \n", (uint32_t)restoreTime );

    UART_printf("  STOG complete for %s \n",
                STOG_TestHandleArray[instanceIndex].instanceName);

    return (testResult);
}


void STOG_datAbortExceptionHandler(void *param)
{
    /* This is a fake exception so return */
}

/* STOG prepare for test */
int32_t STOG_PrepareForTest(void)
{
    int32_t cslRet = CSL_PASS;

    cslRet = cslAppEsmSetup();

    if (cslRet == CSL_PASS)
    {
        /* Register exception handler */
        /* This is needed to handle data abort that can happen in the process of injecting the error */
        Intc_RegisterExptnHandlers(&STOG_R5ExptnHandlers);
    }

    return cslRet;

}

/* STOG Functional test */
int32_t STOG_funcTest(void)
{
    int32_t    testResult = 0;
    int i;

    testResult = STOG_PrepareForTest();
    if (testResult != 0)
    {
        UART_printf("   STOG_PrepareForTest failed \n");
    }

    if (testResult == 0)
    {
        for ( i = 0; i <= STOG_MAX_INSTANCE; i++)
        {
            testResult = STOG_runTest(i);

            if (testResult != 0)
            {
                UART_printf("   STOG functional test failed %d\n", i);
                break;
            }
        }
    }

    return (testResult);
}

/* Nothing past this point */
