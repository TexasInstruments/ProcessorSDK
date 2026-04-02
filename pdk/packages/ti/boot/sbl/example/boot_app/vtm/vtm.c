/*
 *  Copyright (c) Texas Instruments Incorporated 2024
 *  All rights reserved.
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
 *  \file     vtm.c
 *
 *  \brief    This file implements VTM task function
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* For Timer functions */
#include <ti/osal/osal.h>

#include "vtm.h"
#include <src/sdl/sdl_esm.h>
#include <stdbool.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define USE_CASES_RUN         (2)
#define USE_CASES             (2)
#define START_USE_CASE        (0)
#define MAX_ESM_EVENTS_LOGGED (20)
#define SDLR_WKUP_ESM0_ESM_LVL_EVENT_WKUP_VTM0_THERM_LVL_GT_TH1_INTR_0                             (8U)
#define SDLR_WKUP_ESM0_ESM_LVL_EVENT_WKUP_VTM0_THERM_LVL_LT_TH0_INTR_0                             (9U)
#define APP_ARG (1)

/* ========================================================================== */
/*                    Internal Function Declarations                          */
/* ========================================================================== */

static int32_t BootApp_deactivateTrigger(SDL_ESM_Inst esmInstType,
                                 SDL_ESM_IntType esmIntType,
                                 uint32_t intEsmSrc);

static const char *BootApp_printTestCaseStepResult(uint32_t result);
void BootApp_vtmTestPrintSummary(void);
int32_t BootApp_vtmEsmInit (void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static uint32_t totalEventsLogged   = 0;
static uint32_t totalHiEventsLogged = 0;
static uint32_t totalLoEventsLogged = 0;
static int32_t  thresholdsReset     = 0;
int32_t apparg = APP_ARG;

/* ESM event log entry */
typedef struct
{
    SDL_ESM_Inst esmInstance;
    SDL_ESM_IntType      intType;
    uint32_t             grpChannel;
    uint32_t             index;
    uint32_t             intSrc;
    uint8_t              useCaseNum;
} VTM_Example_log_entry_t;

static VTM_Example_log_entry_t esmEventLog[MAX_ESM_EVENTS_LOGGED];


/* State variable for each test case indicating input event trigger
 * has been completed */
volatile uint32_t vtmEventInputTrig[USE_CASES] = {USE_CASE_STATUS_NOT_RUN,
                                                  USE_CASE_STATUS_NOT_RUN};

/* State variable for each test case indicating the ISR for the test case
 * has been completed */
volatile uint32_t vtmOutputResult[USE_CASES] = {USE_CASE_STATUS_NOT_RUN,
                                                USE_CASE_STATUS_NOT_RUN};

volatile uint8_t currTestCase = START_USE_CASE;

/* Initialization structure for WKUP ESM instance */
static SDL_ESM_config VTM_esmInitConfig_WKUP =
{
    .esmErrorConfig = {0u, 0u}, /* Self test error config */
    .enableBitmap = {0xffffffffu, 0x00180003u, 0xffffffffu, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                },
     /**< All events enable: except clkstop events for unused clocks */
    .priorityBitmap = {0xfffffcffu, 0x00180003u, 0xffffffffu, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                        },
    /**< All events high priority: except clkstop events for unused clocks,
     *   and make VTM WKUP_VTM0_THERM_LVL_GT_TH1_INTR_0 and
     *   WKUP_VTM0_THERM_LVL_LT_TH0_INTR_0 events low priority */
    .errorpinBitmap = {0xfffffcffu, 0x00180003u, 0xffffffffu, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                      },
    /**< All events high priority: except clkstop for unused clocks
     *   and make VTM WKUP_VTM0_THERM_LVL_GT_TH1_INTR_0 and
     *   WKUP_VTM0_THERM_LVL_LT_TH0_INTR_0 events not output to pin */
};

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static const char *BootApp_printEsmIntType(SDL_ESM_IntType esmIntType)
{
    char *pStr;

    switch(esmIntType)
    {
        case SDL_ESM_INT_TYPE_HI:
            pStr = "High Priority ESM event";
            break;
        case SDL_ESM_INT_TYPE_LO:
            pStr = "Low Priority ESM event";
            break;
        case SDL_ESM_INT_TYPE_CFG:
            pStr = "Config ESM event";
            break;
        default:
            pStr = NULL;
            break;
    }

    return pStr;
}

int32_t BootApp_esmApplicationCallbackFunction(SDL_ESM_Inst esmInstType,
                                         SDL_ESM_IntType esmIntType,
                                         uint32_t grpChannel,
                                         uint32_t index,
                                         uint32_t intSrc,
                                         void *arg)
{
    /* Log the event */
    esmEventLog[totalEventsLogged].useCaseNum  = currTestCase;
    esmEventLog[totalEventsLogged].esmInstance = esmInstType;
    esmEventLog[totalEventsLogged].intType     = esmIntType;
    esmEventLog[totalEventsLogged].grpChannel  = grpChannel;
    esmEventLog[totalEventsLogged].index       = index;
    esmEventLog[totalEventsLogged].intSrc      = intSrc;

    totalEventsLogged++;
    if (esmIntType == SDL_ESM_INT_TYPE_HI) {
        totalHiEventsLogged++;
    } else if (esmIntType == SDL_ESM_INT_TYPE_LO) {
        totalLoEventsLogged++;
    }

    /* Any additional customer-specific actions to address ESM event
     * can be added here */

    BootApp_deactivateTrigger(esmInstType, esmIntType, intSrc);

    /* Print information to screen */
    UART_printf("\n  ESM Call back function called : instType 0x%x, intType 0x%x, " \
                "grpChannel 0x%x, index 0x%x, intSrc 0x%x \n",
                esmInstType, esmIntType, grpChannel, index, intSrc);
    UART_printf("  Take action \n");

    UART_printf("  ESM instance #%d, ESM interrupt type = %s\n",
                esmInstType, BootApp_printEsmIntType(esmIntType));

    vtmOutputResult[currTestCase]= USE_CASE_STATUS_COMPLETED_SUCCESS;

    return SDL_PASS;
}

/*********************************************************************
* @fn      BootApp_vtmEsmInit
*
* @brief   Initializes Board, Timers, and ESM module
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t BootApp_vtmEsmInit (void)
{
    int32_t retValue=0;
    int32_t result;
        /* Initialize WKUP ESM module */
        result = SDL_ESM_init(SDL_ESM_INST_WKUP_ESM0, &VTM_esmInitConfig_WKUP, BootApp_esmApplicationCallbackFunction, &apparg);
        if (result != SDL_PASS) {
            /* print error and quit */
            UART_printf("VTM_ESM_init: Error initializing WKUP ESM: result = %d\n", result);

            retValue = -1;
        } else {
            UART_printf("\nVTM_ESM_init: Init WKUP ESM complete \n");
        }
    return retValue;
}

static int32_t BootApp_deactivateTrigger(SDL_ESM_Inst esmInstType,
                                 SDL_ESM_IntType esmIntType,
                                 uint32_t intEsmSrc)
{
    int32_t retVal = 0;

    if ((esmInstType == SDL_ESM_INST_WKUP_ESM0) && (esmIntType == SDL_ESM_INT_TYPE_LO)) {
        /* UC-1: Low Priority interrupt on WKUP ESM -
        * VTM greater than THR1 */
        if (intEsmSrc ==
            SDLR_WKUP_ESM0_ESM_LVL_EVENT_WKUP_VTM0_THERM_LVL_GT_TH1_INTR_0)
        {
            if (currTestCase == 0)
            {
                if (thresholdsReset == 0)
                {
                    /* Simulate thresholds as if temperature is going to be reduced
                     * below lt_Thr0 */
                   BootApp_vtmSetNormalThresholds();

                    thresholdsReset = 1;
                }
                BootApp_vtmIntrruptGtThr1();
           } else if (currTestCase == 1)
            {
                if (thresholdsReset == 0)
                {
                    /* Simulate thresholds as if temperature continues to increase
                     * toward gt_Thr2 */
                    BootApp_vtmSetThresholdsForCriticalTrigger();

                    thresholdsReset = 1;
                }
                BootApp_vtmIntrruptGtThr1();
            }
        } else if (intEsmSrc ==
                   SDLR_WKUP_ESM0_ESM_LVL_EVENT_WKUP_VTM0_THERM_LVL_LT_TH0_INTR_0)
        {
           BootApp_vtmIntrruptLtThr0();

            thresholdsReset = 0;
            if (currTestCase == 0) {
                /* At end of this test case, clear the Pin that was left on
                 * throughout the test case*/
                Osal_delay(4);
            }
        }
    } else if ((esmInstType == SDL_ESM_INST_WKUP_ESM0) &&
               (esmIntType == SDL_ESM_INT_TYPE_HI)) {

        if (currTestCase == 1) {
            /* UC-2 High Priority interrupt on WKUP ESM -
             * VTM greater than THR2 with clearing
             * of MCU_SAFETY_ERRORn pin */
            if (thresholdsReset == 1)
            {
                /* Simulate thresholds as if temperature is going to be reduced
                 * below lt_Thr0 */
                BootApp_vtmSetNormalThresholds();

                thresholdsReset = 2;
            }
            if (currTestCase == 1) {
                SDL_ESM_resetErrPin(SDL_WKUP_ESM0_CFG_BASE);
            }
            BootApp_vtmIntrruptGtThr2();
        } else {
            retVal = -1;
        }
    } else {
        UART_printf("ERR: Unexpected ESM Instance %d and ESM Interrupt Type %d \n",
                    esmInstType, esmIntType);
        retVal = -1;
    }

   return (retVal);
}

/*********************************************************************
* @fn      BootApp_vtmTestPrintSummary
*
* @brief   Print summary of all the test cases run
*
* @param   None
*
* @return  None
*/
void BootApp_vtmTestPrintSummary(void)
{
    int32_t i;

    UART_printf("\n\n");
    UART_printf("ESM Example Application summary\n");
    UART_printf("-------------------------------\n");
    UART_printf("Completed %d Test Cases\n", currTestCase);
    UART_printf("Received %d High Priority Interrupts\n", totalHiEventsLogged);
    UART_printf("Received %d Low Priority Interrupts\n", totalLoEventsLogged);

    UART_printf("\nTest Case Event Log\n");
    UART_printf("------------------\n");
    for (i = 0; i < totalEventsLogged; i++) {
        UART_printf("\nTest Case %d: ESM Call back function called : grpChannel 0x%x, " \
                    "index 0x%x, intSrc 0x%x \n",
                    esmEventLog[i].useCaseNum,
                    esmEventLog[i].grpChannel,
                    esmEventLog[i].index,
                    esmEventLog[i].intSrc);
        UART_printf("  ESM instance #%d, ESM interrupt type = %s\n",
                    esmEventLog[i].esmInstance,
                    BootApp_printEsmIntType(esmEventLog[i].intType));

    }
}



/*****************************************************************************
 * This is the main function for the Voltage and Thermal Monitor (VTM) example
 * application.
 * It runs through 2 test cases to demonstrate usage of the VTM modules
 * for receiving errors and controlling the error pin.
 */
void BootApp_vtmFxn(void)
{
    int32_t testErrCount = 0;
    int32_t retValue;
    uint8_t i;

    /* Initialize the ESM instances and handlers */
    retValue = BootApp_vtmEsmInit();

    if (retValue < 0) {
        /* print and exit */
        UART_printf("\nERR: VTM_ESM_init failed");
        testErrCount++;
    }

    UART_printf("\n VTM_ESM_init complete");

    /* Trigger each Test Case */
    for (i = START_USE_CASE; i < 2; i++) {
        retValue = BootApp_vtmRunTestCaseTrigger(i);

        if (retValue != 0) {
            UART_printf("\nERR: Use Case Trigger for Use Case %d failed \n",
                        retValue);
            break;
        }

        UART_printf("\n Use Case %d completed: Input Event Trigger = %s, \n",
                    i,
                    BootApp_printTestCaseStepResult(vtmEventInputTrig[i]));
        currTestCase++;
    }

    /* Check results of all the tests */
    for (i = 0; i < 2; i++) {
        if ((vtmEventInputTrig[i] != USE_CASE_STATUS_COMPLETED_SUCCESS) ||
            (vtmOutputResult[i] != USE_CASE_STATUS_COMPLETED_SUCCESS)) {
            testErrCount++;
        }
   }

    /* Print results and logs of the Test Cases */
    BootApp_vtmTestPrintSummary();
    UART_printf("\n VTM Example Application: Complete");

    if (testErrCount == 0)
    {
        UART_printf("\n All Use cases have passed. \n");
    }
    else
    {
        UART_printf("\n VTM Example app failed. \n");
    }
    return;
}

static const char *BootApp_printTestCaseStepResult(uint32_t result)
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
/* Nothing past this point */