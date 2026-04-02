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
 *  \file lbist_utils.c
 *
 *  \brief LBIST utility functions
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <sdl_types.h>
#if defined (SOC_J742S2)
#include <lbist/soc/j784s4/sdl_lbist.h>
#endif
#include <src/sdl/sdl_lbist.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/uart/UART_stdio.h>

/* Osal API header files */
#include <ti/osal/HwiP.h>
#include <ti/osal/TimerP.h>

#include "power_seq.h"

#include "lbist_utils.h"
#include "lbist_defs.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                      Internal Function Declarations                        */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* This function runs the full LBIST test for a particular core.
 * It includes the following steps (each step has many sub-steps specificied
 * in more details within the function).
 * For HW POST LBIST:
 * Step 1: Check POST results (already run at startup)
 *
 * For SW-initiated LBIST:
 * Step 1: Configure processor to correct state
 * Step 2: Run LBIST test
 * Step 3: Restore cores
 * */
int32_t BootApp_lbistRunTest(uint32_t coreIndex)
{
    int32_t testResult = 0;
    int32_t status;
    bool result;
    SDL_LBIST_postResult postResult;

#if defined(PROFILE_LBIST_TIMING)
    uint64_t startTime , testStartTime,  testEndTime, endTime;
    uint64_t prepTime, diffTime, restoreTime;

    UART_printf("\n Starting LBIST test on %s, index %d...",
                    LBIST_TestHandleArray[coreIndex].coreName,
                    coreIndex);
#endif

#if defined(PROFILE_LBIST_TIMING)
    /* Get start time of test */
    startTime = TimerP_getTimeInUsecs();
#endif


    if ((testResult == 0) &&
        (LBIST_TestHandleArray[coreIndex].hwPostCoreCheck == true))
    {
        status = SDL_LBIST_getPOSTStatus(&postResult);
        if (status != SDL_PASS)
        {
            testResult = -1;
            UART_printf("    SDL_LBIST_getPOSTStatus failed: Status %d \n", status);
        }
        else
        {
            LBIST_printPostStatus(&postResult);
        }
    }
    else
    {
        /* SW-initiated LBIST test flow */
        /*-- Step 1: Configure processor to correct state --*/

        /**--- Step 1a: Request Primary core ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("   Primary core: Sciclient_procBootRequestProcessor, ProcId 0x%x...FAILED \n",
                                    LBIST_TestHandleArray[coreIndex].tisciProcId);
                    testResult = -1;
                }
            }
        }

        /**--- Step 1b: Request Secondary core ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_procBootRequestProcessor, ProcId 0x%x...FAILED \n",
                                    LBIST_TestHandleArray[coreIndex].tisciSecProcId);
                    testResult = -1;
                }
            }
        }

        /**--- Step 1c: Place all Auxilliary modules needed to run test into module reset ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                    LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 1d: Put Primary core in module reset and local reset ---*/
        if ((testResult == 0) && (LBIST_TestHandleArray[coreIndex].tisciDeviceId != 0U))
        {
#ifdef DEBUG
            UART_printf("  Primary core: Putting in module and local reset the core %s \n",
                            LBIST_TestHandleArray[coreIndex].coreName);
#endif
            status = Sciclient_pmSetModuleRst(LBIST_TestHandleArray[coreIndex].tisciDeviceId,
                                              0x3, /* Module Reset and Local Reset asserted */
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                 UART_printf("  Sciclient_pmSetModuleRst 0x%x ...FAILED \n",
                                 LBIST_TestHandleArray[coreIndex].tisciDeviceId);
                 testResult = -1;
            }
        }

        /**--- Step 1e: Put Secondary core in module reset and local reset ---*/
        if ((testResult == 0) && (LBIST_TestHandleArray[coreIndex].tisciSecDeviceId != 0U))
        {
#ifdef DEBUG
            UART_printf("  Secondary core: Putting in module and local reset the core %s \n",
                            LBIST_TestHandleArray[coreIndex].secCoreName);
#endif
            status = Sciclient_pmSetModuleRst(LBIST_TestHandleArray[coreIndex].tisciSecDeviceId,
                                              0x3, /* Module Reset and Local Reset asserted */
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                 UART_printf("  Sciclient_pmSetModuleRst 0x%x ...FAILED \n",
                                 LBIST_TestHandleArray[coreIndex].tisciSecDeviceId);
                 testResult = -1;
            }
        }

        /**--- Step 1f: Place all Auxilliary modules needed to run test into retention ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                    LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 1g: Place Primary core into retention ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("   Primary core: Sciclient_pmSetModuleState...FAILED \n");
                    testResult = -1;
                }
            }
        }

        /**--- Step 1h: Place Secondary core into retention ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_pmSetModuleState...FAILED \n");
                    testResult = -1;
                }
            }
        }

        /**-- Step 2: Run LBIST test --*/
#if defined(PROFILE_LBIST_TIMING)
        /* Get start time for LBIST test */
        testStartTime = TimerP_getTimeInUsecs();
#endif

        if (testResult == 0)
        {
            status = SDL_LBIST_selfTest(LBIST_TestHandleArray[coreIndex].instance, SDL_LBIST_TEST, &result);
            if ((status != SDL_PASS) || (result == false))
            {
                UART_printf("    LBIST selfTest failed \n");
                testResult = -1;
            }
        }

#if defined(PROFILE_LBIST_TIMING)
        /* Here LBIST test is complete , get end time of test */
        testEndTime = TimerP_getTimeInUsecs();
#endif

        /**-- Step 3: Restore cores --*/
        /* The following sequence is needed to restore core to normal operation */

        /**--- Step 3a: Switch off Secondary core ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_pmSetModuleState:  Power off FAILED \n");
                    testResult = -1;
                }
            }
        }

        /**--- Step 3b: Switch off Primary core ---*/
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
            if (status != SDL_PASS)
            {
                UART_printf("   Primary core: Sciclient_pmSetModuleState: Power off FAILED \n");
            }
        }

        /**--- Step 3c: Switch off Auxilliary modules ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                    LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 3d: Disable Isolation ---*/
        if (testResult == 0)
        {
#ifdef DEBUG
            UART_printf("  Disabling isolation \n");
#endif
            status = SDL_LBIST_selfTest(LBIST_TestHandleArray[coreIndex].instance, SDL_LBIST_TEST_RELEASE, &result);
            if (status != SDL_PASS)
            {
                UART_printf("   SDL_LBIST_disableIsolation ...FAILED \n");
                testResult = -1;
            }
        }

        /**--- Step 3e: Place all Auxilliary modules into retention ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                    LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 3f: Place Primary core into retention ---*/
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

            if (status != SDL_PASS)
            {
                UART_printf("   Primary core: Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                            LBIST_TestHandleArray[coreIndex].tisciDeviceId);
                testResult = -1;
            }
        }

        /**--- Step 3g: Place Secondary core into retention ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].tisciSecDeviceId);
                    testResult = -1;
                    return testResult;
                }
            }
        }

        /**--- Step 3h: Power off Secondary core ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_pmSetModuleState:  Power off FAILED \n");
                    testResult = -1;
                }
            }
        }

        /**--- Step 3i: Power off Primary core ---*/
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
            if (status != SDL_PASS)
            {
                UART_printf("   Primary core: Sciclient_pmSetModuleState: Power off FAILED \n");
            }
        }

        /**--- Step 3j: Power off Auxilliary modules ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                    LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 3k: Take Primary core out of local reset ---*/
        if ((testResult == 0) && (LBIST_TestHandleArray[coreIndex].tisciDeviceId != 0U))
        {
#ifdef DEBUG
            UART_printf("  Primary core: Taking out of local reset the core %s \n",
                            LBIST_TestHandleArray[coreIndex].coreName);
#endif
            status = Sciclient_pmSetModuleRst(LBIST_TestHandleArray[coreIndex].tisciDeviceId,
                                              0x0,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("  Sciclient_pmSetModuleRst 0x%x ...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].tisciDeviceId);
                testResult = -1;
            }
        }

        /**--- Step 3l: Take Secondary core out of local reset ---*/
        if ((testResult == 0) && (LBIST_TestHandleArray[coreIndex].tisciSecDeviceId != 0U))
        {
#ifdef DEBUG
            UART_printf("  Secondary core: Taking out of local reset the core %s \n",
                            LBIST_TestHandleArray[coreIndex].secCoreName);
#endif
            status = Sciclient_pmSetModuleRst(LBIST_TestHandleArray[coreIndex].tisciSecDeviceId,
                                              0x0,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("  Sciclient_pmSetModuleRst 0x%x ...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].tisciSecDeviceId);
                testResult = -1;
            }
        }

        /**--- Step 3m: Take Auxilliary modules out of module reset ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                    LBIST_TestHandleArray[coreIndex].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 3n: Release Primary core ---*/
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
            if (status != SDL_PASS)
            {
                UART_printf("   Primary core: Sciclient_procBootReleaseProcessor, ProcId 0x%x...FAILED \n",
                                LBIST_TestHandleArray[coreIndex].tisciProcId);
                testResult = -1;
            }
        }

        /**--- Step 3o: Release Secondary core ---*/
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
                if (status != SDL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_procBootReleaseProcessor, ProcId 0x%x...FAILED \n",
                                    LBIST_TestHandleArray[coreIndex].tisciSecProcId);
                    testResult = -1;
                }
            }
        }

#if defined(PROFILE_LBIST_TIMING)
        prepTime = testStartTime - startTime;
        diffTime = testEndTime - testStartTime;
        restoreTime = endTime - testEndTime;

        UART_printf("  Delta Cores prep time in micro secs %d \n",
                        (uint32_t)prepTime);
        UART_printf("  Delta LBIST execution time in micro secs %d \n",
                        (uint32_t)diffTime);
        UART_printf("  Delta Cores restore time in micro secs %d \n",
                        (uint32_t)restoreTime);
        UART_printf("  LBIST complete for %s \n",
                        LBIST_TestHandleArray[coreIndex].coreName);
#endif
    } /* else clause of "if ((testResult == 0) &&
       *     (LBIST_TestHandleArray[coreIndex].hwPostCoreCheck == true))" */

    return (testResult);
}

const char *BootApp_lbistHwPostStatusPrint(int32_t postStatus)
{
    char *name;

    switch(postStatus)
    {
        case (LBIST_POST_COMPLETED_SUCCESS):
            name="LBIST_POST_COMPLETED_SUCCESS";
            break;
        case (LBIST_POST_COMPLETED_FAILURE):
            name="LBIST_POST_COMPLETED_FAILURE";
            break;
        case (LBIST_POST_ATTEMPTED_TIMEOUT):
            name="LBIST_POST_ATTEMPTED_TIMEOUT";
            break;
        case (LBIST_POST_NOT_RUN):
            name="LBIST_POST_NOT_RUN";
            break;
        case (LBIST_POST_INVALID_MISR_SEED_INPUT):
            name="LBIST_POST_INVALID_MISR_SEED_INPUT";
            break;
        default:
            name="INVALID ID";
            break;
    }

    return name;
}
