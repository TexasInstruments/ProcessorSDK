/*
 * WWDT Example
 *
 * Windowed Watchdog Timer (WWDT) Example Application
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
 *  \file main_r5f_mcu1_0_early.c
 *
 *  \brief This file sets up the Windowed Watchdog Timer (WWDT), Error
 *         Signaling Module (ESM), and handles ESM events in order to detect
 *         a watchdog early service failure.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>

/* For Timer functions */
#include <ti/osal/osal.h>
#include <ti/csl/soc/cslr_soc_intr.h>

/* Diagnostic example utility functions, e.g. print outputs */
#include <diag_utils.h>
#include <sdr_esm.h>

#include "wwdt.h"
#include "esm.h"

#if defined(UNITY_INCLUDE_CONFIG_H)
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Usecase-specific macros */
#define EARLY_USECASE

#if defined(PERMANENT_USECASE)
#define EXPECTED_EVENTS                  (1U)
#elif defined(PERMANENT_FIFTY_USECASE)
#define EXPECTED_EVENTS                  (1U)
#elif defined(EARLY_USECASE)
#define EXPECTED_EVENTS                  (1U)
#elif defined(LATE_USECASE)
#define EXPECTED_EVENTS                  (2U)
#endif

#define MAX_BUFF_LEN                     (4096U)

#define WRONG_KEY_ERROR_EVENT            (0U)
#define EARLY_KEY_ERROR_EVENT            (1U)
#define WRITE_STOP_ERROR_EVENT           (2U)
#define LATE_KEY_ERROR_EVENT             (3U)

#define NUM_EVENT_TYPES                  (4U)

/* Additional compile-time macros used:
 * UART_PRINTF - Prints test execution state and errors to UART when enabled
 * UNITY_INCLUDE_CONFIG_H - Enables Unity for test automation
 */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void checkUsecase(uint32_t num, WWDT_example_ESM_log_entry_t logEntry);

/* Unity functions */
void diag_ex_wwdt_example_app_runner(void);
void wwdt_example_app(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Shows which types of failures are expected for which events */
/* {key, window, start, end} */
#if defined(PERMANENT_USECASE)
uint32_t expectedEventCombinations[EXPECTED_EVENTS][NUM_EVENT_TYPES] = {{0U, 1U, 0U, 1U}};
#elif defined(PERMANENT_FIFTY_USECASE)
uint32_t expectedEventCombinations[EXPECTED_EVENTS][NUM_EVENT_TYPES] = {{0U, 1U, 0U, 1U}};
#elif defined(EARLY_USECASE)
uint32_t expectedEventCombinations[EXPECTED_EVENTS][NUM_EVENT_TYPES] = {{0U, 1U, 1U, 0U}};
#elif defined(LATE_USECASE)
uint32_t expectedEventCombinations[EXPECTED_EVENTS][NUM_EVENT_TYPES] = {{0U, 1U, 0U, 1U},
                                      {0U, 1U, 1U, 0U}};
#endif

/* Flags indicating which events returned the correct combination of events */
#if defined(PERMANENT_USECASE)
uint32_t correctEventFlags[EXPECTED_EVENTS] = {0u};
#elif defined(PERMANENT_FIFTY_USECASE)
uint32_t correctEventFlags[EXPECTED_EVENTS] = {0u};
#elif defined(EARLY_USECASE)
uint32_t correctEventFlags[EXPECTED_EVENTS] = {0u};
#elif defined(LATE_USECASE)
uint32_t correctEventFlags[EXPECTED_EVENTS] = {0u, 0u};
#endif

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


/*****************************************************************************
 * This is the main function for the Windowed Watchdog Timer (WWDT) example
 * application.
 * It runs through the early key written usecase (UC-3) to demonstrate usage
 * of the WWDT module for detecting and handling early key writes (i.e.
 * service before window opens).
 */
void wwdt_example_app(void)
{
    int32_t result;
    uint32_t i;
    uint32_t totalEvents;
    WWDT_example_ESM_log_entry_t logEntry;

#ifdef UART_PRINTF
    /* Initialize UART instance */
    DIAG_uartStdioInit();
#endif

    /* Initialize ESM instance and interrupt handler */
    result = ESM_init();
#if defined(UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_MESSAGE(SDR_PASS, result,
                                               "WWDT Example: ESM init failed!");
#else
    if (SDR_PASS != result)
    {
        /* print error and quit */
        DIAG_printf("\nWWDT Example: ESM init failed! \n");

    }
#endif

    /* Initialize main WWDT and interrupt handler */
    result = WWDT_Example_WwdtInitHandlerInit();
#if defined(UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_MESSAGE(SDR_PASS, result,
                                               "WWDT Example: WWDT handler init failed!");
#else
    if (result != SDR_PASS)
    {
        /* print error and quit */
        DIAG_printf("\nWWDT Example: WWDT handler init failed! \n");
    }
#endif

    if (result == SDR_PASS)
    {
        MailboxSendMessage(MAILBOX_BASE_ADDRESS, MAILBOX_QUEUE_0, 0U);
        /* Can't detect early key within 100% window, but expect
            multiple events from completely stopping key writes
            and some late key writes */
        totalEvents = GetTotalEventsLogged();
        while (totalEvents < EXPECTED_EVENTS)
        {
            totalEvents = GetTotalEventsLogged();
        }

        DIAG_printf("\nWWDT Events: \n");

        for (i = 0; i < EXPECTED_EVENTS; i++)
        {
            logEntry = GetWwdtEventLog(i);
            /* No entry expected, key violations are not detected on J721E */
            if (0U != logEntry.keyViolation)
            {
                DIAG_printf("\nEvent %d key violation \n", i);
            }
            /* Entry expected on "Do Not Service Watchdog", "Early Key Written",
                "Late Key Written", and three entries on "Writing Key Completely
                Stops" */
            if (0U != logEntry.windowViolation)
            {
                DIAG_printf("\nEvent %d window violation \n", i);
            }
            /* Entry expected on "Do Not Service Watchdog", "Late Key Written", and
               three entries on "Writing Key Completely Stops" */
            if (0U != logEntry.endViolation)
            {
                DIAG_printf("\nEvent %d end violation \n", i);
            }
            /* Entry expected on "Early Key Written" */
            if (0U != logEntry.startViolation)
            {
                DIAG_printf("\nEvent %d start violation \n", i);
            }

            /* Check usecase correctness */
            /*checkUsecase(i, logEntry);*/
        }
    }

    DIAG_printf("\nWWDT Example: main_r5f_mcu1_0 finished \n");

    MailboxSendMessage(MAILBOX_BASE_ADDRESS, MAILBOX_QUEUE_0, 0U);
}

/*****************************************************************************
 * This is the usecase correctness check function of the watchdog example. It
 * compares expected event types to actual event types and determines whether
 * usecases generated the correct event types.
 */
void checkUsecase(uint32_t eventNum, WWDT_example_ESM_log_entry_t logEntry)
{
    if ((expectedEventCombinations[eventNum][WRONG_KEY_ERROR_EVENT] == logEntry.keyViolation) &&
        (expectedEventCombinations[eventNum][EARLY_KEY_ERROR_EVENT] == logEntry.windowViolation) &&
        (expectedEventCombinations[eventNum][WRITE_STOP_ERROR_EVENT] == logEntry.startViolation) &&
        (expectedEventCombinations[eventNum][LATE_KEY_ERROR_EVENT] == logEntry.endViolation))
    {
        correctEventFlags[eventNum] = 1U;
    }
    else
    {
        DIAG_printf("\nEvent %d mismatch! \n", eventNum);
        DIAG_printf("\nExpected: ");
        if (0U != expectedEventCombinations[eventNum][WRONG_KEY_ERROR_EVENT])
        {
            DIAG_printf("key ");
        }
        if (0U != expectedEventCombinations[eventNum][EARLY_KEY_ERROR_EVENT])
        {
            DIAG_printf("window ");
        }
        if (0U != expectedEventCombinations[eventNum][WRITE_STOP_ERROR_EVENT])
        {
            DIAG_printf("start ");
        }
        if (0U != expectedEventCombinations[eventNum][LATE_KEY_ERROR_EVENT])
        {
            DIAG_printf("end ");
        }
        DIAG_printf(" \n\nActual  : ");
        if (0U != logEntry.keyViolation)
        {
            DIAG_printf("key ");
        }
        if (0U != logEntry.windowViolation)
        {
            DIAG_printf("window ");
        }
        if (0U != logEntry.startViolation)
        {
            DIAG_printf("start ");
        }
        if (0U != logEntry.endViolation)
        {
            DIAG_printf("end ");
        }
        DIAG_printf(" \n");
    }
#if defined(UNITY_INCLUDE_CONFIG_H)
#if defined(PERMANENT_USECASE)
    TEST_ASSERT_NOT_EQUAL_MESSAGE(0U, correctEventFlags[eventNum],
                                           "(UC-1) Permanent failure usecase incorrect event(s)!");
#elif defined(PERMANENT_FIFTY_USECASE)
    TEST_ASSERT_NOT_EQUAL_MESSAGE(0U, correctEventFlags[eventNum],
                                           "(UC-2) Permanent failure (windowed) usecase incorrect event(s)!");
#elif defined(EARLY_USECASE)
    TEST_ASSERT_NOT_EQUAL_MESSAGE(0U, correctEventFlags[eventNum],
                                           "(UC-3) Early key written usecase failed!");
#elif defined(LATE_USECASE)
    if (1U == eventNum)
    {
        TEST_ASSERT_NOT_EQUAL_MESSAGE(0U, correctEventFlags[eventNum - 1U],
                                               "(UC-4) Late key written usecase incorrect event(s)!");
        TEST_ASSERT_NOT_EQUAL_MESSAGE(0U, correctEventFlags[eventNum],
                                               "(UC-4) Late key written usecase incorrect event(s)!");
    }
#endif
#endif
}

int32_t main(void)
{
    (void) diag_ex_wwdt_example_app_runner();

    while (true)
    {
    }
}

#if defined(UNITY_INCLUDE_CONFIG_H)
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

void diag_ex_wwdt_example_app_runner(void)
{
#if defined(UNITY_INCLUDE_CONFIG_H)
    UNITY_BEGIN();
    RUN_TEST (wwdt_example_app);
    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
#else
    wwdt_example_app();
#endif
    return;
}
