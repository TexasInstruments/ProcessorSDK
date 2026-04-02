/*
 *   Copyright (c) Texas Instruments Incorporated 2019-2020
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
 *  \file     lbist_test_func.c
 *
 *  \brief    This file contains LBIST functional test code. .
 *
 *  \details  LBIST Functional tests
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/csl_lbist.h>
#include <ti/csl/soc.h>
#include <ti/drv/sciclient/sciclient.h>

#ifdef QT_BUILD
#include <stdio.h>
#define UART_printf printf
#else
#include <ti/drv/uart/UART_stdio.h>
#endif

/* Osal API header files */
#include <ti/osal/HwiP.h>
#include <ti/osal/TimerP.h>

#include "lbist_test_cfg.h"

/* #define DEBUG */

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/* HW POST run status definitions */
#define LBIST_POST_COMPLETED_SUCCESS      (0u)
#define LBIST_POST_COMPLETED_FAILURE      (1u)
#define LBIST_POST_ATTEMPTED_TIMEOUT      (2u)
#define LBIST_POST_NOT_RUN                (3u)

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void LBIST_eventHandler( uint32_t coreIndex )
{
    int32_t status;
    Bool isLBISTDone = CSL_FALSE;
    LBIST_TestHandle_t *LBIST_TestHandleArray = LBIST_getTestHandleArray();
    CSL_lbistRegs *pLBISTRegs = LBIST_TestHandleArray[coreIndex].pLBISTRegs;

    /* Double check if the LBIST done flag is set */
    status = CSL_LBIST_isDone(pLBISTRegs, &isLBISTDone);
    if ((status == CSL_PASS) && (isLBISTDone == CSL_TRUE))
    {
        LBIST_TestHandleArray[coreIndex].doneFlag = true;
        /* Need to pull run down to low to clear the done interrupt */
        CSL_LBIST_stop( pLBISTRegs );
    }
    return;

}

int32_t LBIST_runTest(uint32_t coreIndex)
{
    int32_t testResult = 0;
    int32_t status;
    uint32_t calculatedMISR;
    uint32_t expectedMISR;
    Bool isLBISTRunning = CSL_FALSE;
    CSL_lbistRegs *pLBISTRegs;
    uint32_t *pLBISTSig;
    HwiP_Params       hwiParams;
    HwiP_Handle LBIST_hwiPHandle;
    uint64_t startTime , testStartTime,  testEndTime, endTime;
    uint64_t prepTime, diffTime, restoreTime;
    uint32_t timeoutCount = 0;
    LBIST_TestHandle_t *LBIST_TestHandleArray = LBIST_getTestHandleArray();

    UART_printf("\n Starting LBIST test on %s, index %d...",
                LBIST_TestHandleArray[coreIndex].coreName,
                coreIndex);
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
    pLBISTRegs = LBIST_TestHandleArray[coreIndex].pLBISTRegs;
    pLBISTSig = LBIST_TestHandleArray[coreIndex].pLBISTSig;


#ifdef DEBUG
    UART_printf("\n  HwiP_Params_init complete \n");
#endif
    /*-- Step 1: Configure interrupt handler --*/

    if (LBIST_TestHandleArray[coreIndex].handler != NULL)
    {

        /* Disable interrupt */
        HwiP_disableInterrupt(LBIST_TestHandleArray[coreIndex].interruptNumber);

        /* Default parameter initialization */
        HwiP_Params_init(&hwiParams);

        /* Pass core Index as argument to handler*/
        hwiParams.arg = coreIndex;

        /* Register call back function for LBIST Interrupt */
        LBIST_hwiPHandle = HwiP_create(LBIST_TestHandleArray[coreIndex].interruptNumber,
                                           (HwiP_Fxn)LBIST_TestHandleArray[coreIndex].handler,
                                           (void *)&hwiParams);
        if (LBIST_hwiPHandle == NULL)
        {
            UART_printf("   Interrupt registration failed \n");
            testResult = -1;
        }
    }

    /* Initialize done flag */
    LBIST_TestHandleArray[coreIndex].doneFlag = false;

    /* Get start time of test */
    startTime = TimerP_getTimeInUsecs();

    /* Step 2: (only for POST) Check POST results  */
#ifdef LBIST_POST_CORE_MAX
    if ((testResult == 0) &&
        (LBIST_TestHandleArray[coreIndex].hwPostCoreCheck == true))
    {
        int32_t  postStatus;

        /* HW POST test flow - test already run, checking the result */
        postStatus = LBIST_runPostLbistCheck(LBIST_TestHandleArray[coreIndex].hwPostCoreNum,
                                             LBIST_TestHandleArray[coreIndex].pLBISTRegs,
                                             LBIST_TestHandleArray[coreIndex].pLBISTSig);
#ifdef DEBUG
        UART_printf("   HW POST core: %s: result = %d\n",
                    LBIST_TestHandleArray[coreIndex].coreName,
                    postStatus);
#endif
        if ((postStatus == LBIST_POST_COMPLETED_FAILURE) ||
            (postStatus == LBIST_POST_ATTEMPTED_TIMEOUT))
        {
            /* Only return failure if HW POST was run and it
             * either failed or timed out */
            testResult = -1;
        }
        else if (postStatus == LBIST_POST_COMPLETED_SUCCESS)
        {
            UART_printf("   HW POST core: %s: HW POST was run successfully on this device\n",
                        LBIST_TestHandleArray[coreIndex].coreName);
        }
        else if (postStatus == LBIST_POST_NOT_RUN)
        {
            UART_printf("   HW POST core: %s: HW POST was not run on this device\n",
                        LBIST_TestHandleArray[coreIndex].coreName);
        }
    }
    else
#endif /* LBIST_POST_CORE_MAX */
    {
        /*-- Step 2: Configure processor to correct state --*/
        /* SW-initiated LBIST test flow */

        /**--- Step 2a: Request Primary core ---*/
        if (testResult == 0)
        {
            if (LBIST_TestHandleArray[coreIndex].tisciProcId != 0u)
            {
#ifdef DEBUG
                UART_printf("  Primary core: %s: Requesting processor \n",
                            LBIST_TestHandleArray[coreIndex].coreName);
#endif
                /* Request Primary core */
                status = Sciclient_procBootRequestProcessor(LBIST_TestHandleArray[coreIndex].tisciProcId,
                                                            SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Primary core: Sciclient_procBootRequestProcessor, ProcId 0x%x...FAILED : Status %d\n",
                                LBIST_TestHandleArray[coreIndex].tisciProcId, status);
                    testResult = -1;
                }
            }
        }

        /**--- Step 2b: Request Secondary core ---*/
        if (testResult == 0)
        {
            if ((LBIST_TestHandleArray[coreIndex].secondaryCoreNeeded)
                && (LBIST_TestHandleArray[coreIndex].tisciSecProcId != 0u))
            {

#ifdef DEBUG
                UART_printf("  Secondary core: %s: Requesting processor \n",
                        LBIST_TestHandleArray[coreIndex].secCoreName);
#endif
                /* Request secondary core */
                status = Sciclient_procBootRequestProcessor(LBIST_TestHandleArray[coreIndex].tisciSecProcId,
                                                            SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_procBootRequestProcessor, ProcId 0x%x...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].tisciSecProcId);
                    testResult = -1;
                }
            }
        }

        /**--- Step 2c: Place all Auxilliary modules needed to run test into module reset ---*/
        if (testResult == 0)
        {
            int i;

            /* Place all Auxilliary modules required for test into module reset */
            for ( i = 0; i < LBIST_TestHandleArray[coreIndex].numAuxDevices; i++)
            {
#ifdef DEBUG
                UART_printf("  Putting into module reset Device number %d Device Id %x\n",
                            i,
                            LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
#endif

                status = Sciclient_pmSetModuleRst(LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i],
                                                  0x2, /* Module Reset asserted */
                                                  SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 2d: Put Primary core in module reset and local reset ---*/
        if ((testResult == 0) && (LBIST_TestHandleArray[coreIndex].tisciDeviceId != (uint32_t)NULL))
        {
#ifdef DEBUG
            UART_printf("  Primary core: Putting in module and local reset the core %s \n",
                        LBIST_TestHandleArray[coreIndex].coreName);
#endif
            status = Sciclient_pmSetModuleRst(LBIST_TestHandleArray[coreIndex].tisciDeviceId,
                                              0x3, /* Module Reset and Local Reset asserted */
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                 UART_printf("  Sciclient_pmSetModuleRst 0x%x ...FAILED \n",
                             LBIST_TestHandleArray[coreIndex].tisciDeviceId);
                 testResult = -1;
            }
        }

        /**--- Step 2e: Put Secondary core in module reset and local reset ---*/
        if ((testResult == 0) && (LBIST_TestHandleArray[coreIndex].tisciSecDeviceId != (uint32_t)NULL))
        {
#ifdef DEBUG
            UART_printf("  Secondary core: Putting in module and local reset the core %s \n",
                        LBIST_TestHandleArray[coreIndex].secCoreName);
#endif
            status = Sciclient_pmSetModuleRst(LBIST_TestHandleArray[coreIndex].tisciSecDeviceId,
                                              0x3, /* Module Reset and Local Reset asserted */
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                 UART_printf("  Sciclient_pmSetModuleRst 0x%x ...FAILED \n",
                             LBIST_TestHandleArray[coreIndex].tisciSecDeviceId);
                 testResult = -1;
            }
        }

        /**--- Step 2f: Place all Auxilliary modules needed to run test into retention ---*/
        if (testResult == 0)
        {
            int i;

            /* Place all Auxilliary modules required for test into retention */
            for ( i = 0; i < LBIST_TestHandleArray[coreIndex].numAuxDevices; i++)
            {
#ifdef DEBUG
                UART_printf("  Putting into Retention Device number %d Device Id %x\n",
                            i,
                            LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
#endif

                status = Sciclient_pmSetModuleState(LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i],
                                                    TISCI_MSG_VALUE_DEVICE_SW_STATE_RETENTION,
                                                    TISCI_MSG_FLAG_AOP,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 2g: Place Primary core into retention ---*/
        if (testResult == 0)
        {
            if (LBIST_TestHandleArray[coreIndex].tisciDeviceId != 0u)
            {
                /* Placing Primary core into Retention */
#ifdef DEBUG
                UART_printf("  Primary core: Putting into Retention %s \n",
                            LBIST_TestHandleArray[coreIndex].coreName);
#endif
                status =  Sciclient_pmSetModuleState(LBIST_TestHandleArray[coreIndex].tisciDeviceId,
                                                     TISCI_MSG_VALUE_DEVICE_SW_STATE_RETENTION,
                                                     TISCI_MSG_FLAG_AOP,
                                                     SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Primary core: Sciclient_pmSetModuleState...FAILED \n");
                    testResult = -1;
                }
            }
        }

        /**--- Step 2h: Place Secondary core into retention ---*/
        if (testResult == 0)
        {
            if (LBIST_TestHandleArray[coreIndex].tisciSecDeviceId != 0u)
            {
                /* Placing Secondary core into Retention */
#ifdef DEBUG
                UART_printf("  Secondary core: Putting into Retention %s \n",
                            LBIST_TestHandleArray[coreIndex].secCoreName);
#endif
                status =  Sciclient_pmSetModuleState(LBIST_TestHandleArray[coreIndex].tisciSecDeviceId,
                                                     TISCI_MSG_VALUE_DEVICE_SW_STATE_RETENTION,
                                                     TISCI_MSG_FLAG_AOP,
                                                     SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_pmSetModuleState...FAILED \n");
                    testResult = -1;
                }
            }
        }

        /**-- Step 3: Configure LBIST --*/
        if (testResult == 0)
        {
            /* Configure LBIST */

            status = CSL_LBIST_programConfig(pLBISTRegs, &LBIST_TestHandleArray[coreIndex].LBISTConfig);
            if (status != CSL_PASS)
            {
                UART_printf("    LBIST program config failed \n");
                testResult = -1;
            }
        }

        /**-- Step 4: Run LBIST test --*/
        /* Get start time for LBIST test */
        testStartTime = TimerP_getTimeInUsecs();

        /**--- Step 4a: Enable Isolation ---*/
        if (testResult == 0)
        {

            /* Call CSL API */
            status = CSL_LBIST_enableIsolation(pLBISTRegs);
            if (status != CSL_PASS)
            {
                UART_printf("   CSL_LBIST_enableIsolation...FAILED \n");
                testResult = -1;
            }
        }

        /**--- Step 4b: reset LBIST ---*/
        if (testResult == 0)
        {
            status = CSL_LBIST_reset(pLBISTRegs);
            if (status != CSL_PASS)
            {
                UART_printf("   CSL_LBIST_reset...FAILED \n");
                testResult = -1;
            }
        }

        /**--- Step 4c: Enable Run BIST Mode ---*/
        if (testResult == 0)
        {
            status = CSL_LBIST_enableRunBISTMode(pLBISTRegs);
            if (status != CSL_PASS)
            {
                UART_printf("   CSL_LBIST_enableRunBISTMode...FAILED \n");
                testResult = -1;
            }
        }

        /**--- Step 4d: Start LBIST ---*/
        if (testResult == 0)
        {
            status = CSL_LBIST_start(pLBISTRegs);
            if (status != CSL_PASS)
            {
                UART_printf("   CSL_LBIST_start...FAILED \n");
                testResult = -1;
            }
        }

        /**--- Step 4e: Check LBIST Running status ---*/
        if (testResult == 0)
        {
            status = CSL_LBIST_isRunning(pLBISTRegs, &isLBISTRunning);
            if (status != CSL_PASS)
            {
                UART_printf("   CSL_LBIST_isRunning...FAILED \n");
                testResult = -1;
            }
        }

        /**--- Step 4f: Wait for LBIST Interrupt ---*/
        if (testResult == 0)
        {
            /* Just checking if LBIST in running state */
            if ( isLBISTRunning != CSL_TRUE )
            {
#ifdef DEBUG
                UART_printf("   LBIST not running \n");
#endif
            }

            /* Timeout if exceeds time */
            while ((!LBIST_TestHandleArray[coreIndex].doneFlag)
                   && (timeoutCount++ < LBIST_MAX_TIMEOUT_VALUE))
            {
                /* Interrupt handler available */
                if (LBIST_TestHandleArray[coreIndex].handler == NULL)
                {
                    /* Use Polling */
                    LBIST_eventHandler(coreIndex);
                }
            }

            if (!(LBIST_TestHandleArray[coreIndex].doneFlag))
            {
                UART_printf("   LBIST test timed out \n");
                testResult = -1;
            }
            /* reset Done flag so we can run again */
            LBIST_TestHandleArray[coreIndex].doneFlag = false;
        }

        /**--- Step 4g: Get Signature of test ---*/
        if (testResult == 0)
        {
            status = CSL_LBIST_getMISR(pLBISTRegs, &calculatedMISR);
            if (status != CSL_PASS)
            {
                UART_printf("   Get MISR failed \n");
                testResult = -1;
            }
        }

        /**--- Step 4h: Get Expected Signature ---*/
        if (testResult == 0)
        {
            status = CSL_LBIST_getExpectedMISR(pLBISTSig, &expectedMISR);
            if ( status != CSL_PASS)
            {
                UART_printf("   Get Expected MISR failed \n");
                testResult = -1;
            }
        }

        /**--- Step 4i: Clear Run BIST Mode ---*/
        if (testResult == 0)
        {
            status = CSL_LBIST_clearRunBISTMode(pLBISTRegs);
            if ( status != CSL_PASS)
            {
                UART_printf("   CSL_LBIST_clearRunBISTMode failed \n");
                testResult = -1;
            }
        }

        /**--- Step 4j: Stop LBIST ---*/
        if (testResult == 0)
        {
            status = CSL_LBIST_stop(pLBISTRegs);
            if ( status != CSL_PASS)
            {
                UART_printf("   CSL_LBIST_stop failed \n");
                testResult = -1;
            }
        }

        /**--- Step 4k: Reset LBIST ---*/
        if (testResult == 0)
        {
            status = CSL_LBIST_reset(pLBISTRegs);
            if ( status != CSL_PASS)
            {
                UART_printf("   CSL_LBIST_reset failed \n");
                testResult = -1;
            }
        }
        /* Here LBIST test is complete , get end time of test */
        testEndTime = TimerP_getTimeInUsecs();

        /**-- Step 5: Restore cores --*/
        /* The following sequence is needed to restore core to normal operation */

        /**--- Step 5a: Switch off Secondary core ---*/
        if (testResult == 0)
        {
            if (LBIST_TestHandleArray[coreIndex].secondaryCoreNeeded)
            {
                /* Power off Secondary core */
#ifdef DEBUG
                UART_printf("  Secondary core: Powering off %s \n",
                            LBIST_TestHandleArray[coreIndex].secCoreName);
#endif
                status =  Sciclient_pmSetModuleState(LBIST_TestHandleArray[coreIndex].tisciSecDeviceId,
                                                     TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                     TISCI_MSG_FLAG_AOP,
                                                     SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_pmSetModuleState:  Power off FAILED \n");
                    testResult = -1;
                }
            }
        }

        /**--- Step 5b: Switch off Primary core ---*/
        if (testResult == 0)
        {
            /* Power off Primary core */
#ifdef DEBUG
            UART_printf("  Primary core: Powering off %s \n",
                        LBIST_TestHandleArray[coreIndex].coreName);
#endif
            status =  Sciclient_pmSetModuleState(LBIST_TestHandleArray[coreIndex].tisciDeviceId,
                                                 TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                 TISCI_MSG_FLAG_AOP,
                                                 SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                UART_printf("   Primary core: Sciclient_pmSetModuleState: Power off FAILED \n");
            }
        }

        /**--- Step 5c: Switch off Auxilliary modules ---*/
        if (testResult == 0)
        {
            int i;

            /* Place all Auxilliary modules required for test into retention */
            for ( i = 0; i < LBIST_TestHandleArray[coreIndex].numAuxDevices; i++)
            {
#ifdef DEBUG
                UART_printf("  Powering off Device number %d Device Id %x\n",
                            i,
                            LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
#endif

                status = Sciclient_pmSetModuleState(LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i],
                                                    TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                    TISCI_MSG_FLAG_AOP,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 5d: Disable Isolation ---*/
        if (testResult == 0)
        {
#ifdef DEBUG
            UART_printf("  Disabling isolation \n");
#endif
            status = CSL_LBIST_disableIsolation(pLBISTRegs);
            if (status != CSL_PASS)
            {
                UART_printf("   CSL_LBIST_disableIsolation ...FAILED \n");
                testResult = -1;
            }
        }

        /**--- Step 5e: Place all Auxilliary modules into retention ---*/
        if (testResult == 0)
        {
            int i;

            /* Place all Auxilliary modules required for test into retention */
            for ( i = 0; i < LBIST_TestHandleArray[coreIndex].numAuxDevices; i++)
            {
#ifdef DEBUG
                UART_printf("  Putting into Retention Device number %d Device Id %x\n",
                            i,
                            LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
#endif

                status = Sciclient_pmSetModuleState(LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i],
                                                    TISCI_MSG_VALUE_DEVICE_SW_STATE_RETENTION,
                                                    TISCI_MSG_FLAG_AOP,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 5f: Place Primary core into retention ---*/
        if (testResult == 0)
        {
            /* Placing Primary core into Retention */
#ifdef DEBUG
            UART_printf("  Primary core: Putting into Retention %s \n",
                        LBIST_TestHandleArray[coreIndex].coreName);
#endif
            status = Sciclient_pmSetModuleState(LBIST_TestHandleArray[coreIndex].tisciDeviceId,
                                                TISCI_MSG_VALUE_DEVICE_SW_STATE_RETENTION,
                                                TISCI_MSG_FLAG_AOP,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);

            if (status != CSL_PASS)
            {
                UART_printf("   Primary core: Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                            LBIST_TestHandleArray[coreIndex].tisciDeviceId);
                testResult = -1;
            }
        }

        /**--- Step 5g: Place Secondary core into retention ---*/
        if (testResult == 0)
        {
            if (LBIST_TestHandleArray[coreIndex].secondaryCoreNeeded)
            {
                /* Placing Secondary core into Retention */
#ifdef DEBUG
                UART_printf("  Secondary core: Putting into Retention %s \n",
                            LBIST_TestHandleArray[coreIndex].secCoreName);
#endif
                status = Sciclient_pmSetModuleState(LBIST_TestHandleArray[coreIndex].tisciSecDeviceId,
                                                    TISCI_MSG_VALUE_DEVICE_SW_STATE_RETENTION,
                                                    TISCI_MSG_FLAG_AOP,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].tisciSecDeviceId);
                    testResult = -1;
                    return testResult;
                }
            }
        }

        /**--- Step 5h: Double check LBIST not running ---*/
        if (testResult == 0)
        {
            /* Check to make sure LBIST is not running */
            status = CSL_LBIST_isRunning(pLBISTRegs, &isLBISTRunning);
            if ( status != CSL_PASS )
            {
                UART_printf("\n CSL_LBIST_isRunning failed \n");
                testResult = -1;
            }
        }

        /**--- Step 5i: Power off Secondary core ---*/
        if (testResult == 0)
        {
            if (LBIST_TestHandleArray[coreIndex].secondaryCoreNeeded)
            {
                /* Power off Secondary core */
#ifdef DEBUG
                UART_printf("  Secondary core: Powering off %s \n",
                            LBIST_TestHandleArray[coreIndex].secCoreName);
#endif
                status =  Sciclient_pmSetModuleState(LBIST_TestHandleArray[coreIndex].tisciSecDeviceId,
                                                     TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                     TISCI_MSG_FLAG_AOP,
                                                     SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_pmSetModuleState:  Power off FAILED \n");
                    testResult = -1;
                }
            }
        }

        /**--- Step 5j: Power off Primary core ---*/
        if (testResult == 0)
        {
            /* Power off Primary core */
#ifdef DEBUG
            UART_printf("  Primary core: Powering off %s \n",
                        LBIST_TestHandleArray[coreIndex].coreName);
#endif
            status =  Sciclient_pmSetModuleState(LBIST_TestHandleArray[coreIndex].tisciDeviceId,
                                                 TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                 TISCI_MSG_FLAG_AOP,
                                                 SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                UART_printf("   Primary core: Sciclient_pmSetModuleState: Power off FAILED \n");
            }
        }

        /**--- Step 5k: Power off Auxilliary modules ---*/
        if (testResult == 0)
        {
            int i;

            /* Place all Auxilliary modules required for test into retention */
            for ( i = 0; i < LBIST_TestHandleArray[coreIndex].numAuxDevices; i++)
            {
#ifdef DEBUG
                UART_printf("  Powering off Device number %d Device Id %x\n",
                            i,
                            LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
#endif

                status = Sciclient_pmSetModuleState(LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i],
                                                    TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                    TISCI_MSG_FLAG_AOP,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 5l: Take Primary core out of local reset ---*/
        if ((testResult == 0) && (LBIST_TestHandleArray[coreIndex].tisciDeviceId != (uint32_t)NULL))
        {
#ifdef DEBUG
            UART_printf("  Primary core: Taking out of local reset the core %s \n",
                        LBIST_TestHandleArray[coreIndex].coreName);
#endif
            status = Sciclient_pmSetModuleRst(LBIST_TestHandleArray[coreIndex].tisciDeviceId,
                                              0x0,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                UART_printf("  Sciclient_pmSetModuleRst 0x%x ...FAILED \n",
                            LBIST_TestHandleArray[coreIndex].tisciDeviceId);
                testResult = -1;
            }
        }

        /**--- Step 5m: Take Secondary core out of local reset ---*/
        if ((testResult == 0) && (LBIST_TestHandleArray[coreIndex].tisciSecDeviceId != (uint32_t)NULL))
        {
#ifdef DEBUG
            UART_printf("  Secondary core: Taking out of local reset the core %s \n",
                        LBIST_TestHandleArray[coreIndex].secCoreName);
#endif
            status = Sciclient_pmSetModuleRst(LBIST_TestHandleArray[coreIndex].tisciSecDeviceId,
                                              0x0,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                UART_printf("  Sciclient_pmSetModuleRst 0x%x ...FAILED \n",
                            LBIST_TestHandleArray[coreIndex].tisciSecDeviceId);
                testResult = -1;
            }
        }

        /**--- Step 5n: Take Auxilliary modules out of module reset ---*/
        if (testResult == 0)
        {
            int i;

            /* Place all Auxilliary modules required for test into module reset */
            for ( i = 0; i < LBIST_TestHandleArray[coreIndex].numAuxDevices; i++)
            {
#ifdef DEBUG
                UART_printf("  Putting into module reset Device number %d Device Id %x\n",
                            i,
                            LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
#endif

                status = Sciclient_pmSetModuleRst(LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i],
                                                  0x0, // Need to keep Local Reset too??
                                                  SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 5o: Release Primary core ---*/
        if ((testResult == 0) && (LBIST_TestHandleArray[coreIndex].tisciProcId !=0))
        {
            /* release processor Primary core */
#ifdef DEBUG
            UART_printf("  Primary core: Releasing %s \n",
                        LBIST_TestHandleArray[coreIndex].coreName);
#endif

            status = Sciclient_procBootReleaseProcessor(LBIST_TestHandleArray[coreIndex].tisciProcId,
                                                        TISCI_MSG_FLAG_AOP,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                UART_printf("   Primary core: Sciclient_procBootReleaseProcessor, ProcId 0x%x...FAILED \n",
                            LBIST_TestHandleArray[coreIndex].tisciProcId);
                testResult = -1;
            }
        }

        /**--- Step 5p: Release Secondary core ---*/
        if (testResult == 0)
        {
            if ((LBIST_TestHandleArray[coreIndex].secondaryCoreNeeded)
                && (LBIST_TestHandleArray[coreIndex].tisciSecDeviceId != 0u))
            {
                /* release processor Secondary core */
#ifdef DEBUG
                UART_printf("  Secondary core: Releasing %s \n",
                            LBIST_TestHandleArray[coreIndex].secCoreName);
#endif
                status = Sciclient_procBootReleaseProcessor(LBIST_TestHandleArray[coreIndex].tisciSecProcId,
                                                            TISCI_MSG_FLAG_AOP,
                                                            SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_procBootReleaseProcessor, ProcId 0x%x...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].tisciSecProcId);
                    testResult = -1;
                }
            }
        }

        /**--- Step 5q: Double check LBIST not running ---*/
        if (testResult == 0)
        {
            status = CSL_LBIST_isRunning(pLBISTRegs, &isLBISTRunning);
            if (status != CSL_PASS)
            {
                UART_printf("    CSL_LBIST_isRunning failed\n");
                testResult = -1;
            }
            else
            {
                /* Make sure that the LBIST is not running */
                if (isLBISTRunning == CSL_TRUE)
                {
                    UART_printf("\n   LBIST is found to be still running at the end of the test\n");
                    testResult = -1;
                }
            }
        }

        /* Here LBIST test is complete , get end time of test */
        endTime = TimerP_getTimeInUsecs();

        /**--- Step 6: Check result of LBIST  ---*/
        if (testResult == 0)
        {
            /* TODO: Temporarily hard coding to expected MISR */
            expectedMISR = LBIST_TestHandleArray[coreIndex].expectedMISR;
            if (calculatedMISR != expectedMISR)
            {
                UART_printf("\n   LBIST failed with MISR mismatch: Expected 0x%x got 0x%x \n",
                            expectedMISR, calculatedMISR);
                testResult = -1;
            }
            else
            {
                UART_printf("\n   LBIST MISR matched \n");
            }
        }

        prepTime = testStartTime - startTime;
        diffTime = testEndTime - testStartTime;
        restoreTime = endTime - testEndTime;
        UART_printf("  Delta Cores prep time in micro secs %d \n", (uint32_t)prepTime );
        UART_printf("  Delta LBIST execution time in micro secs %d \n", (uint32_t)diffTime );
        UART_printf("  Delta Cores restore time in micro secs %d \n", (uint32_t)restoreTime );
    } /* if ((testResult == 0) &&
         * (LBIST_TestHandleArray[coreIndex].hwPostCoreCheck == true)) */

    UART_printf("  LBIST complete for %s \n",
                LBIST_TestHandleArray[coreIndex].coreName);

    return (testResult);
}

/* Run all APIs not exercised by functional test */
int32_t LBIST_apiTest(uint32_t coreIndex)
{
    int32_t testResult = 0;
    int32_t status;
    LBIST_TestHandle_t *LBIST_TestHandleArray = LBIST_getTestHandleArray();
    CSL_lbistRegs *pLBISTRegs;

    pLBISTRegs = LBIST_TestHandleArray[coreIndex].pLBISTRegs;

    /* Call CSL APIs not used by functional test */
    status = CSL_LBIST_clearRunBISTMode(pLBISTRegs);
    if (status != CSL_PASS)
    {
        UART_printf("    CSL_LBIST_clearRunBISTMode failed\n");
        testResult = -1;
    }

    if (testResult == CSL_PASS)
    {
        status = CSL_LBIST_stop(pLBISTRegs);
        if (status != CSL_PASS)
        {
            UART_printf("    CSL_LBIST_stop failed\n");
            testResult = -1;
        }
    }

    return (testResult);
}

/* LBIST Functional test */
int32_t LBIST_funcTest(void)
{
    int32_t    testResult = 0;
    int i;

    for ( i = 0; i < LBIST_NUM_TEST_CORES; i++)
    {
        testResult = LBIST_runTest(i);

        if (testResult != 0)
        {
            UART_printf("   LBIST functional test failed %d\n", i);
            break;
        }
    }

    if (testResult == 0)
    {
        /* API test is enough to be run on one instance */
        testResult = LBIST_apiTest(0);
        if (testResult != 0)
        {
            UART_printf("   LBIST API test failed \n");
        }
    }

    return (testResult);
}

/* Nothing past this point */
