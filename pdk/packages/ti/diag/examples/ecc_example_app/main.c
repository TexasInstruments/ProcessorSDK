/*
 * ECC Example
 *
 * Error Correcting Code (ECC) Example Application
 *
 *  Copyright (c) 2020 Texas Instruments Incorporated
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
 *  \file main.c
 *
 *  \brief This file demonstrates using the Error Correcting Code Module (ECC),
 *         utilizing the ECC and ESM Software Diagnostic Reference (SDR) functions.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* For Timer functions */
#include <ti/osal/osal.h>

#include <ti/board/board.h>

/* Diagnostic example utility functions, e.g. print outputs */
#include <diag_utils.h>

#include "ecc_inject.h"
#include "esm.h"

#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define USE_CASES_RUN         (6)
#define USE_CASES             (6)
#define START_USE_CASE        (0)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* Unity functions */
void diag_ex_ecc_example_app_runner(void);
void ecc_example_app(void);

static const char *printUseCaseStepResult(uint32_t result);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* State variable for each use case indicating input event trigger
 * has been completed */
volatile uint32_t eccEventInputTrig[USE_CASES] = {USE_CASE_STATUS_NOT_RUN,
                                                  USE_CASE_STATUS_NOT_RUN,
                                                  USE_CASE_STATUS_NOT_RUN,
                                                  USE_CASE_STATUS_NOT_RUN,
                                                  USE_CASE_STATUS_NOT_RUN,
                                                  USE_CASE_STATUS_NOT_RUN};

/* State variable for each use case indicating the ISR for the use case
 * has been completed */
volatile uint32_t eccOutputResult[USE_CASES] = {USE_CASE_STATUS_NOT_RUN,
                                                USE_CASE_STATUS_NOT_RUN,
                                                USE_CASE_STATUS_NOT_RUN,
                                                USE_CASE_STATUS_NOT_RUN,
                                                USE_CASE_STATUS_NOT_RUN,
                                                USE_CASE_STATUS_NOT_RUN};
volatile uint8_t currUseCase = START_USE_CASE;

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*****************************************************************************
 * This is the main function for the Error Correcting Code (ECC) example
 * application.
 * It runs through 5 use cases to demonstrate usage of the ECC modules
 * for receiving and interpretting ECC errors.
 */
void ecc_example_app(void)
{
    int32_t testErrCount = 0;
    int32_t retValue;
    uint8_t i;

    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    boardStatus = Board_init(boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        DIAG_printf("[Error] Board init failed!!\n");
    } else {

        /* Initialize the ESM instances and handlers */
        retValue = ECC_Example_esmInit();
#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_ASSERT_GREATER_OR_EQUAL_INT32_MESSAGE(0, retValue,
                                                   "\nERR: ECC_example_esmInit failed");
#else
        if (retValue < 0) {
            /* print and exit */
            DIAG_printf("\nERR: ECC_example_init failed");
            testErrCount++;
        }
#endif
        retValue = diag_ecc_initUseCases();

#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_ASSERT_GREATER_OR_EQUAL_INT32_MESSAGE(0, retValue,
                                                   "\nERR: diag_ecc_initUseCases failed");
#else
        if (retValue < 0) {
            /* print and exit */
            DIAG_printf("\nERR: ECC_example_init failed");
            testErrCount++;
        }
#endif
        DIAG_printf("\nECC example init complete");

        /* Trigger each Use Case */
        for (i = START_USE_CASE; i < USE_CASES_RUN; i++) {
            retValue = diag_ecc_runUseCaseTrigger(i);
#if defined (UNITY_INCLUDE_CONFIG_H)
            TEST_ASSERT_EQUAL_MESSAGE(0, retValue,
                                      "\nERR: Use Case Trigger for Use Case %d failed \n");
#else
            if (retValue != 0) {
                DIAG_printf("\nERR: Use Case Trigger for Use Case %d failed \n",
                            retValue);
                break;
            }
#endif
            while((eccEventInputTrig[i] == USE_CASE_STATUS_NOT_RUN) ||
                  (eccOutputResult[i] == USE_CASE_STATUS_NOT_RUN))
            {
#ifdef DEBUG_PRINT
                DIAG_printf("InputTrig = %d, OutputResult = %d\n",
                            eccEventInputTrig[i],
                            eccOutputResult[i]);
#endif
            }
            DIAG_printf("\nUse Case %d completed: Input Event Trigger = %s, \n" \
                        "                       Event Handler Complete = %s, \n",
                        i,
                        printUseCaseStepResult(eccEventInputTrig[i]),
                        printUseCaseStepResult(eccOutputResult[i]));
            currUseCase++;
        }

        /* Check results of all the tests */
        for (i = 0; i < USE_CASES_RUN; i++) {
            if ((eccEventInputTrig[i] != USE_CASE_STATUS_COMPLETED_SUCCESS) ||
                (eccOutputResult[i] != USE_CASE_STATUS_COMPLETED_SUCCESS)) {
                testErrCount++;
            }
        }

        /* Print results and logs of the Use Cases */
        ECC_Example_printSummary();
        DIAG_printf("\nECC Example Application: Complete");

        if (testErrCount == 0)
        {
            DIAG_printf("\n All tests have passed. \n");
        }
        else
        {
            DIAG_printf("\n ESM Example app failed. \n");
        }

#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_ASSERT_EQUAL_INT32(0, testErrCount);
#endif

    } /* else clause for if (boardStatus != BOARD_SOK) */

    return;
}

static const char *printUseCaseStepResult(uint32_t result)
{
    char *pStr;

    switch(result)
    {
        case USE_CASE_STATUS_NOT_RUN:
            pStr = "Step Not yet run";
            break;
        case USE_CASE_STATUS_COMPLETED_SUCCESS:
            pStr = "Step completed successfully";
            break;
        case USE_CASE_STATUS_COMPLETED_FAILURE:
            pStr = "Step completed with failure";
            break;
        default:
            pStr = NULL;
            break;
    }

    return pStr;
}

int32_t main(void)
{
    (void) diag_ex_ecc_example_app_runner();

    while (true)
    {
    }
}

#ifdef UNITY_INCLUDE_CONFIG_H
/*
 *  ======== Unity set up and tear down ========
 */
void setUp(void)
{
    /* Do nothing */
}

void tearDown(void)
{
    /* Do nothing */
}
#endif

void diag_ex_ecc_example_app_runner(void)
{
#if defined(UNITY_INCLUDE_CONFIG_H)
    UNITY_BEGIN();
    RUN_TEST (ecc_example_app);
    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
#else
    ecc_example_app();
#endif
    return;
}
