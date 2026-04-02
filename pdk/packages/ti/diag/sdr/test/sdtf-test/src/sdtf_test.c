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
#include <stdio.h>
#include <string.h>

#include <sdr_common.h>
#include <sdr_ecc.h>
#include <sdr_esm.h>

#include <ti/drv/uart/src/UART_utils_defs.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/UART.h>

#include "sdtf_common.h"
#include "sdtf_esm.h"
#include "sdtf_ecc.h"
#include "sdtf_profile.h"
#include "sdtf_test.h"
#include "sdtf_errortype.h"
#include "sdtf_injecterr.h"
#include "sdtf_wdt.h"
#include "sdtf_rat.h"
#include "sdtf_ccm.h"
#include "sdtf_crc.h"
#include "sdtf_mpu.h"
#include "sdtf_exception.h"

#define SDTF_MAX_PERIODIC_TEST_RUN 10

#define SDTF_PERIODIC_TASK_TRIGGER_OFF (0u)
#define SDTF_PERIODIC_TASK_TRIGGER_ENABLE (1u)

/* Global variables */
static uint32_t SDTF_periodicTaskCount=0;
static uint32_t SDTF_interactiveTaskCount=0;
static uint32_t SDTF_periodicExecutionTimeMin=0xffffffffu, SDTF_periodicExecutionTimeMax=0u;
static uint32_t SDTF_periodicExecutionTime, SDTF_periodicExecutionTimeAvg;
static uint32_t SDTF_periodicExecutionFirstTime=0;

/* Flag to track all tasks ended */
volatile uint32_t SDTF_allTasksEnded=0u;
volatile uint32_t SDTF_periodTaskTrigger = 0u;

#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/*********************************************************************
 * @fn      SDTF_oneShotTestAllModules
 *
  * @brief   Executes all one shot tests
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */

int32_t SDTF_oneShotTestAllModules(void)
{

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;
    SDR_Result result;

    SDTF_printf("\nStarting: One Shot tests");

    SDTF_printf("\n Starting: ATCM Double bit error fixed location once test ");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* -------------- OneShot Tests start ------------------------- */
    /* Run one shot test for ATCM 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x200u);

    injectErrorConfig.flipBitMask = 0x101;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                              SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              1000);
    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS ) {
        SDTF_printf("\n ATCM Double bit error: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
        return -1;
    }
    SDTF_printf("\n ATCM Double bit error: pErrMem 0x%p fixed location once test complete",
                injectErrorConfig.pErrMem);
    SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));

    SDTF_printf("\n Starting: ATCM Single bit error test");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x00);

    /* Run one shot test for ATCM 1 bit error */
    injectErrorConfig.flipBitMask = 0x10;
    result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                    SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                    SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                    &injectErrorConfig,
                                    100000);
    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);
    if (result != SDR_PASS ) {
        SDTF_printf("\n ATCM Single bit error at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
        return -1;
    }
    SDTF_printf("\n ATCM Single bit error  at pErrMem 0x%p: test complete",
                injectErrorConfig.pErrMem);
    SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));

    SDTF_printf("\n Starting: ESM test");
    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot self test of ESM */
    result = SDR_ESM_selfTest(SDR_ESM_INSTANCE_MCU, 1000);
    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);
    if (result != SDR_PASS ) {
        SDTF_printf("\n ESM self test failed");
        return -1;
    }
    SDTF_printf("\n ESM Self test complete");
    SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));

    /* -------------- OneShot Tests end ------------------------- */
    SDTF_printf("\nOne shot tests complete");

    return 0; 
}


/*********************************************************************
 * @fn      SDTF_configPeriodicTests
 *
 * @brief   Execute all the configured periodic tests
 *
 * @param   periodicConfig - configuration structure for periodic tests
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_configPeriodicTests(SDTF_periodic_config_t periodic_config)
{
    /* Reset periodic Task count */
    SDTF_periodicTaskCount=0;

    /* Reset flag for periodic tests */
    SDTF_periodTaskTrigger = SDTF_PERIODIC_TASK_TRIGGER_OFF;

    return 0; 
}
/*********************************************************************
 * @fn      SDTF_triggerPeriodicTests
 *
 * @brief   Trigger the periodic tests
 *
 * @param   arg: Input argument
*
* @return  None
*/
void SDTF_triggerPeriodicTests(uintptr_t arg)
{
    volatile uintptr_t storeArg = arg;

    /* Set the flag to trigger periodic tests */
    SDTF_periodTaskTrigger = SDTF_PERIODIC_TASK_TRIGGER_ENABLE;

    /* Feed watchdog timer */
    SDTF_WDT_feedWatchdogTimer();
}
/*********************************************************************
 * @fn      SDTF_runPeriodicTests
 *
 * @brief   Execute all the configured periodic tests
 *
 * @param   None
*
* @return  0 : Success; < 0 for failures
*/
int32_t SDTF_runPeriodicTests(void)
{
    SDR_Result result;

    if (( SDTF_periodTaskTrigger == SDTF_PERIODIC_TASK_TRIGGER_ENABLE)
        && (SDTF_allTasksEnded != 1u) ) {

        /* Increment task count */
        SDTF_periodicTaskCount++;

        /* If max runs are reached print results */
        if (SDTF_periodicTaskCount >= SDTF_MAX_PERIODIC_TEST_RUN) {
            /* Mark all tasks_ended */
            SDTF_allTasksEnded = 1u;
            SDTF_printf("\n Periodic Tests: Cycle measurements");
            SDTF_printf("\n Min: %d, Max %d Avg %d Last %d",
                        SDTF_periodicExecutionTimeMin, SDTF_periodicExecutionTimeMax,
                        SDTF_periodicExecutionTimeAvg, SDTF_periodicExecutionTime);

        }

        SDTF_profileBegin(SDTF_PROFILE_PERIODIC);
        /* -------------- Periodic Test start ------------------------- */

        /* Run test for ECC */
        SDR_ECC_InjectErrorConfig_t injectErrorConfig;

            /* Note the address is relative to start of ram */
        injectErrorConfig.pErrMem = (uint32_t *)(0x00);

        /* Run one shot test for ATCM 1 bit error */
        injectErrorConfig.flipBitMask = 0x10;
        result = SDR_ECC_selfTest(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                        SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                        SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                        &injectErrorConfig,
                                        100000);


        if (result != SDR_PASS ) {
            SDTF_printf("\n ATCM Single bit error at pErrMem 0x%p test failed",
                        injectErrorConfig.pErrMem);
            return -1;
        }

        /* Run esm test 1*/
        result = SDR_ESM_selfTest(SDR_ESM_INSTANCE_MCU,1000);
        if (result != SDR_PASS ) {
            SDTF_printf("\n ESM self test 1 failed");
            return -1;
        }

        /* -------------- Periodic Tests end ------------------------- */
        SDTF_profileEnd(SDTF_PROFILE_PERIODIC);

        /* Get the time taken for periodic tests */
        SDTF_periodicExecutionTime = SDTF_profileDelta(SDTF_PROFILE_PERIODIC);
        /* First time initialiaze average data */
        if (SDTF_periodicExecutionFirstTime == 0u) {
            SDTF_periodicExecutionTimeAvg = SDTF_periodicExecutionFirstTime;
            SDTF_periodicExecutionFirstTime = 1u;
        }
        /* Record the minimum, maximum and average values of time taken */
        if (SDTF_periodicExecutionTime < SDTF_periodicExecutionTimeMin) {
            SDTF_periodicExecutionTimeMin = SDTF_periodicExecutionTime;
        }
        if (SDTF_periodicExecutionTime > SDTF_periodicExecutionTimeMax) {
            SDTF_periodicExecutionTimeMax = SDTF_periodicExecutionTime;
        }
        SDTF_periodicExecutionTimeAvg = (SDTF_periodicExecutionTimeAvg + SDTF_periodicExecutionTime) >> 1u;

        /* Clear flag which triggered periodic task */
        SDTF_periodTaskTrigger = SDTF_PERIODIC_TASK_TRIGGER_OFF;
    }
    return 0; 
}

/* Maxumum number of special commands
 * Should match number of items in the special commands array below
 */
#define SDTF_MAX_SPECIAL_COMMANDS (5u)

#ifdef SOC_AM65XX
/* Number of commands run by the "run_all" command
 * Note: This should match number of entries in the tables below
 */
#define SDTF_NUM_RUNALL_TEST_COMMANDS (36u)

/* Other commands not covered by run_all */
#define SDTF_NUM_OTHER_TEST_COMMANDS (0u)
#endif

#ifdef SOC_J721E
/* Number of commands run by the "run_all" command
 * Note: This should match number of entries in the tables below
 */
#define SDTF_NUM_RUNALL_TEST_COMMANDS (39u)

/* Other commands not covered by run_all */
#define SDTF_NUM_OTHER_TEST_COMMANDS (0u)
#endif

/* Maxumum number of commands
 * Note: This should match maximumum number of entries in the tables below
 */
#define SDTF_MAX_COMMANDS (SDTF_NUM_RUNALL_TEST_COMMANDS+SDTF_NUM_OTHER_TEST_COMMANDS)

#define SDTF_MAX_COMMAND_LEN (64u)

/* prototype type for test functions */
typedef int32_t (* SDTF_testFunctionPtr_t) (void);

typedef struct SDTF_commandList_s
{
    char commandString[SDTF_MAX_COMMAND_LEN+1];
    SDTF_testFunctionPtr_t commandFunction;
}SDTF_commandList_t;

/* Full list of commands */
SDTF_commandList_t SDTF_commandList[SDTF_MAX_COMMANDS] =
{
    { "esm_selftest_MCU",            SDTF_runESMSelfTest_MCU},
    { "esm_inject_MCU",              SDTF_runESMInject_MCU },
    { "esm_apitest_MCU",             SDTF_runESMAPITest },
    { "esm_negativetest_MCU",        SDTF_runESMNegativeTest},
    { "esm_selftest_WKUP",           SDTF_runESMSelfTest_WKUP},
    { "esm_inject_WKUP",             SDTF_runESMInject_WKUP },
    { "esm_selftest_MAIN",           SDTF_runESMSelfTest_MAIN},
    { "esm_inject_MAIN",             SDTF_runESMInject_MAIN },
    { "ecc1_selftest",               SDTF_runECC1BitSelfTest },
    { "ecc2_selftest",               SDTF_runECC2BitSelfTest },
    { "ecc2_inject",                 SDTF_runECC2BitInjectTest },
    { "ecc1_b0tcm0selftest",         SDTF_runECC1BitB0TCM0Bank0SelfTest },
    { "ecc2_b0tcm0selftest",         SDTF_runECC2BitB0TCM0Bank0SelfTest },
    { "ecc1_b0tcm0_b1selftest",      SDTF_runECC1BitB0TCM0Bank1SelfTest },
    { "ecc2_b0tcm0_b1selftest",      SDTF_runECC2BitB0TCM0Bank1SelfTest },
    { "ecc1_b1tcm0selftest",         SDTF_runECC1BitB1TCM0Bank0SelfTest },
    { "ecc2_b1tcm0selftest",         SDTF_runECC2BitB1TCM0Bank0SelfTest },
    { "ecc1_b1tcm0_b1selftest",      SDTF_runECC1BitB1TCM0Bank1SelfTest },
    { "ecc2_b1tcm0_b1selftest",      SDTF_runECC2BitB1TCM0Bank1SelfTest },
    { "ecc1_vimraminject",           SDTF_runECC1BitVIMRAMInjectTest },
    { "ecc2_vimraminject",           SDTF_runECC2BitVIMRAMInjectTest },
    { "ecc1_vimramselftest",         SDTF_runECC1BitVIMRAMSelfTest },
    { "ecc2_vimramselftest",         SDTF_runECC2BitVIMRAMSelfTest },
    { "crc_selftest",                SDTF_runCRCSelfTest },
    { "crc_negativetest",            SDTF_runCRCNegativeTest },
    { "mpu_selftest",                SDTF_runMPUSelfTest},
    { "wdt_selftestendviolation",    SDTF_runWDTSelfTestEndViolation },
    { "wdt_selftestwindowviolation", SDTF_runWDTSelfTestWindowViolation },
    { "wdt_negativetests",           SDTF_runWDTNegativeTests },
    { "RAT_test",                    SDTF_runtestRAT },
    { "ecc_runnegativetests",        SDTF_ECC_runNegativeTests },
    { "ecc2_programinject",          SDTF_runECC2BitCodeInjectTest },
    { "exception_runapitests",       SDTF_runExceptionApiTests },
    { "ecc1_inject",                 SDTF_runECC1BitInjectTest },
#ifdef SOC_J721E
    { "cbass_1bitinject",            SDTF_runECC1BitCBASSInjectTest },
    { "cbass_1bitselftest",          SDTF_runECC1BitCBASSSelfTest },
    { "cbass_2bitinject",            SDTF_runECC2BitCBASSInjectTest },
    { "cbass_2bitselftest",          SDTF_runECC2BitCBASSSelfTest },
#endif
#ifdef SOC_AM65XX
/*     { "ccm_selftest",                SDTF_runCCMSelfTest },
    { "ccm_selftest_errorforce",     SDTF_runCCMSelfTestErrorForce },
    { "ccm_vimselftest",             SDTF_runCCMVIMSelfTest },
    { "ccm_inactivityselftest",      SDTF_runCCMInactivitySelfTest }, */
    { "ccm_inject",                  SDTF_runCCMInjectError },
    /* { "ccm_selftest_polarityinvert",     SDTF_runCCMSelftestPolarityInvert }, */
#endif
    /* This needs to be last as it is destructive */
    { "ecc2_vimramdedvector",        SDTF_runECC2BitVIMRAMDEDvector },


     /* The following tests are not covered by run all */

};

char SDTF_specialCommands[SDTF_MAX_SPECIAL_COMMANDS][SDTF_MAX_COMMAND_LEN+1] =
{
    /* The following are special commands */
    "read_reg",
    "write_reg",
    "debug_test",
    "run_all",
    "help",
};

/*********************************************************************
 * @fn      SDTF_runInteractiveTests
 *
 * @brief   Execute interactive tests
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runInteractiveTests(void)
{
    char buffPointer[256];
    char echoPrompt[] = "\n Interactive test :Enter a command or Esc to quit >";
    char echoPrompt1[] = "Data received is:";
    uint32_t endInteractiveTestsFlag=0;
    int32_t retVal = 0;
    uint32_t i;

    SDTF_interactiveTaskCount++;

    while(endInteractiveTestsFlag == 0)
    {
        /* Print prompt to UART port */
        UART_printf(echoPrompt);

        /* Scan input word from user */
        if (UART_scanFmt("%s", &buffPointer) != S_PASS)
        {
            SDTF_printf("\n ERROR: UART_scanFmt failed");
            endInteractiveTestsFlag = 1;
            continue;
        }

        /* Check on word entered here */
        /* If needed a command parser can be added here */
        switch(buffPointer[0]) {
        case '\0': /*Ignore empty string */
            break;

        case 27: /* Exit on ESC character */
            endInteractiveTestsFlag = 1;
            break;

        default:
            /* Display prompt  */
            UART_printf(echoPrompt1);
            /* Display received word */
            UART_printf(buffPointer);
            /* Print new line */
            UART_printf("\n");
            break;
        }

        /* Help command to display all commands */
        if (strncmp(buffPointer, "help", SDTF_MAX_COMMAND_LEN) == 0) {
            uint32_t i;

            SDTF_printf("\n Here are the commands supported");
            for(i=0u; i< SDTF_NUM_RUNALL_TEST_COMMANDS; i++) {
                SDTF_printf("\n %s", SDTF_commandList[i].commandString);
            }
            SDTF_printf("\n --------------------------------");
            SDTF_printf("\n Other commands");
            for(; i< SDTF_MAX_COMMANDS; i++) {
                SDTF_printf("\n %s", SDTF_commandList[i].commandString);
            }
            SDTF_printf("\n --------------------------------");
            SDTF_printf("\n Special commands");
            for(i=0u; i< SDTF_MAX_SPECIAL_COMMANDS; i++) {
                SDTF_printf("\n %s", SDTF_specialCommands[i]);
            }
            SDTF_printf("\n --------------------------------");
            SDTF_printf("\n [ESC]<enter> to quit");
        }


        /* Help command to display all commands */
        if ((strncmp(buffPointer, "read_reg", SDTF_MAX_COMMAND_LEN) == 0)
            || (strncmp(buffPointer, "write_reg", SDTF_MAX_COMMAND_LEN) == 0)) {
            char buffPointer2[256];
            uintptr_t regAddress;

            /* Print prompt to UART port */
            UART_printf("Enter address in hex: ");

            /* Scan input word from user */
            if (UART_scanFmt("%s", &buffPointer2) != S_PASS)
            {
                SDTF_printf("\n ERROR: UART_scanFmt failed");
                endInteractiveTestsFlag = 1;
                continue;
            }
            sscanf(buffPointer2, "%x", &regAddress);

            if (strncmp(buffPointer, "read_reg", SDTF_MAX_COMMAND_LEN) == 0) {
                SDTF_printf("\n Read value at 0x%x is 0x%x", regAddress, *((volatile uint32_t *)regAddress));
            } else {
                char buffPointer3[256];
                uint32_t regValue;

                /* Print prompt to UART port */
                UART_printf("Enter value: ");

                /* Scan input word from user */
                if (UART_scanFmt("%s", &buffPointer3) != S_PASS)
                {
                    SDTF_printf("\n ERROR: UART_scanFmt failed");
                    endInteractiveTestsFlag = 1;
                    continue;
                }
                sscanf(buffPointer3, "%x", &regValue);
                *((volatile uint32_t *)regAddress) = regValue;
                SDTF_printf("\n Written address 0x%x with value: 0x%x read value 0x%x",
                            regAddress, regValue, *((volatile uint32_t *)regAddress));
            }

        }

        /* Command to Run all commands */
        if (strncmp(buffPointer, "debug_test", SDTF_MAX_COMMAND_LEN) == 0) {
            /* Add any debug test here */

        }
        /* Command to Run all commands */
        if (strncmp(buffPointer, "run_all", SDTF_MAX_COMMAND_LEN) == 0) {

            SDTF_printf("\n Running all test commands supported");
            for(i=0u; i< SDTF_NUM_RUNALL_TEST_COMMANDS; i++) {
                if (SDTF_commandList[i].commandFunction!= ((void *)(0u))) {
                    retVal = (*SDTF_commandList[i].commandFunction)();
                    if ( retVal != 0) {
                        break;
                    }
                }
            }
        }

        /* Search through all commands to execute appropriate functions */
        for(i=0u; i< SDTF_MAX_COMMANDS; i++) {
            if (strncmp(buffPointer, SDTF_commandList[i].commandString, SDTF_MAX_COMMAND_LEN) == 0) {
                if (SDTF_commandList[i].commandFunction != ((void *)(0u))) {
                    retVal = (*SDTF_commandList[i].commandFunction)();
                    break;
                }
            }
        }
        if (retVal < 0) {
            /* Print error message and check to continue*/
            UART_printf("\n Error occurred: Enter \"continue\" command to continue");
            /* Scan input word from user */
            if (UART_scanFmt("%s", &buffPointer) != S_PASS)
            {
                SDTF_printf("\n ERROR: UART_scanFmt failed");
                endInteractiveTestsFlag = 1;
                continue;
            }
            /* If continue in spite of error */
             if (strncmp(buffPointer, "continue", SDTF_MAX_COMMAND_LEN) == 0) {
                 /* Reset return value, so that test can continue*/
                 retVal = 0;
                 continue;
             } else {
                 /* Else end test */
                 endInteractiveTestsFlag = 1;
                 continue;
             }
        }
    }

    return retVal;
}

/*********************************************************************
 * @fn      test_sdr_test
 *
 * @brief   Execute all sdr tests
 *
 * @param   None
 *
 * @return  None
 */
void test_sdr_test(void)
{
    /* Declarations of variables */
    int32_t retVal = 0;
    int32_t i;

    SDTF_printf("\n Running all sdr test commands supported");
    for(i = 0u; i< SDTF_NUM_RUNALL_TEST_COMMANDS; i++) {
        if (SDTF_commandList[i].commandFunction!= ((void *)(0u))) {
            retVal = (*SDTF_commandList[i].commandFunction)();
            if ( retVal != 0) {
                break;
            }
        }
    }

    if (retVal == 0)
    {
        UART_printStatus("\n All tests have passed. \n");
    }
    else
    {
        UART_printStatus("\n Few/all tests Failed \n");
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(0, retVal);
#endif
}
