/*
 *  Copyright (C) 2024-2025 Texas Instruments Incorporated
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
 *  \file  sci_app_common.c
 *
 *  \brief Implementation of common functions used by the SCI applications.
 * 
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#if defined (BUILD_C7X)
#include <ti/csl/arch/csl_arch.h>
#endif
#include <ti/board/board.h>
#include <ti/drv/sciclient/examples/common/sci_app_common.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define SCI_APP_PRINT_BUFFER_SIZE   (256U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

extern SciApp_TestParams_t gSciApp_TestcaseParams[];

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Function Declarations                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                     Internal Function Declarations                         */
/* ========================================================================== */

/**
 * \brief   Get the index of the testcase to run from respective SCI application.
 *
 * \param   testcaseId        Testcase Id to run.
 *
 * \return  testcaseIdx       Testcase index to run
 */
static int32_t SciApp_getIndex(uint32_t testcaseId);

/**
 * \brief   Print main menu.
 *
 * \param   None.
 *
 * \return  None
 */
static void SciApp_showMainMenu(void);

/**
 * \brief   Print sciclient testcase names.
 *
 * \param   None.
 *
 * \return  None
 */
static void SciApp_printTestName(void);

/**
 * \brief   Print sciclient testcase details.
 *
 * \param   testId        TestcaseID for which details are to be printed.
 *
 * \return  None
 */
static void SciApp_printTestDetails(uint32_t testId);

/**
 * \brief   Initialize the details of sciclient testcase results.
 *
 * \param   None.
 *
 * \return  None
 */
static void SciApp_resultInit(void);

/**
 * \brief   Print sciclient Test Case Results.
 *
 * \param   None.
 *
 * \return  None
 */
static void SciApp_printResults(void);

/**
 * \brief   Fetch testcase data and run the testcase.
 *
 * \param   testParams        Structure to details of testcase to be run.
 *
 * \return  None.
 */
static void SciApp_run(SciApp_TestParams_t *testParams);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void SciApp_parser(void)
{
    uint32_t testcaseId;
    uint32_t numTCPass, numTCFail, totalTCRun, done = 0U;
    int32_t  testcaseIdx, testIp;
    uint8_t  option;
    SciApp_TestParams_t *testParams;

    SciApp_consoleInit();
    SciApp_resultInit();

    while (!done)
    {
        SciApp_showMainMenu();
        #if (SCI_APP_AUTORUN == 1)
        option = SCI_APP_RUN_TESTS_ALL;
        #else
        SciApp_printf("\nEnter your choice: ");
        option = SciApp_getChar();
        SciApp_printf("%c\n", option);
        #endif

        numTCPass  = 0U;
        numTCFail  = 0U;
        totalTCRun = 0U;
        switch (option)
        {
            case SCI_APP_RUN_TEST_ID:
                SciApp_printf("\nEnter testcase ID to run: ");
                testIp      = SciApp_getNum();
                testcaseIdx = SciApp_getIndex(testIp);
                if (testcaseIdx < 0U)
                {
                    SciApp_printf("Invalid test case Id.");
                }
                else if (gSciApp_TestcaseParams[testcaseIdx].enableTest == SCI_APP_TEST_ENABLE)
                {
                    testParams = &gSciApp_TestcaseParams[testcaseIdx];
                    if (testParams->cpuID == SCI_APP_CORE_TYPE_R5F)
                    {
                        totalTCRun++;
                        SciApp_run(testParams);
                        if (testParams->testResult == CSL_PASS)
                        {
                            numTCPass++;
                        }
                        else
                        {
                            numTCFail++;
                        }
                    }
                }
                SciApp_printf("\nSCI: Total Testcases run:%u/%u\n", totalTCRun, SciApp_getNumTests());
                SciApp_printf("SCI: Number of Testcases Passed:%u\n", numTCPass);
                SciApp_printf("SCI: Number of Testcases Failed:%u\n", numTCFail);
                break;
            case SCI_APP_RUN_TESTS_SANITY:
                for (testcaseId = 0U; testcaseId < SciApp_getNumTests(); testcaseId++)
                {
                    testParams = &gSciApp_TestcaseParams[testcaseId];
                    if (testParams->enableTest == SCI_APP_TEST_ENABLE)
                    {
                        if ((testParams->testType & SCI_APP_TEST_TYPE_SANITY) == SCI_APP_TEST_TYPE_SANITY) 
                        {
                            if (testParams->cpuID == SCI_APP_CORE_TYPE_R5F)
                            {
                                totalTCRun++;
                                SciApp_run(testParams);
                                if (testParams->testResult == CSL_PASS)
                                {
                                    numTCPass++;
                                }
                                else
                                {
                                    numTCFail++;
                                }
                            }
                        }
                    }
                }
                SciApp_printf("\nSCI: Total Testcases run:%u/%u\n", totalTCRun, SciApp_getNumTests());
                SciApp_printf("SCI: Number of Testcases Passed:%u\n", numTCPass);
                SciApp_printf("SCI: Number of Testcases Failed:%u\n", numTCFail);
                SciApp_printResults();
                break;
            case SCI_APP_RUN_TESTS_ALL:
                for (testcaseId = 0U; testcaseId < SciApp_getNumTests(); testcaseId++)
                {
                    testParams = &gSciApp_TestcaseParams[testcaseId];
                    if (testParams->enableTest == SCI_APP_TEST_ENABLE)
                    {
                        totalTCRun++;
                        SciApp_run(testParams);
                        if (testParams->testResult == CSL_PASS)
                        {
                            numTCPass++;
                        }
                        else
                        {
                            numTCFail++;
                        }
                    }
                }
                SciApp_printf("\nSCI: Total Testcases run:%u/%u\n", totalTCRun, SciApp_getNumTests());
                SciApp_printf("SCI: Number of Testcases Passed:%u\n", numTCPass);
                SciApp_printf("SCI: Number of Testcases Failed:%u\n", numTCFail);
                SciApp_printResults();
                done = 1U;
                break;
            case SCI_APP_PRINT_ALLTESTS:
                SciApp_printTestName();
                break;
            case SCI_APP_PRINT_TEST_ID:
                SciApp_printf("\nEnter testcase ID: ");
                testIp      = SciApp_getNum();
                testcaseIdx = SciApp_getIndex(testIp);
                if (testcaseIdx < 0U)
                {
                    SciApp_printf("Invalid test case Id.");
                }
                else
                {
                    SciApp_printTestDetails(testcaseIdx);
                }
                break;
            case SCI_APP_PRINT_RESULTS:
                SciApp_printResults();
                break;
            case SCI_APP_PARSER_QUIT:
                SciApp_printf("Exiting SCI Test application.\n");
                done = 1U;
                break;
            default: 
                SciApp_printf("Unsupported Option. Please try again.\n");
                break;
        }
        
        #if defined LDRA_DYN_COVERAGE_EXIT
        UART_printf("\n LDRA Entry... \n");
        upload_execution_history();
        UART_printf("\n LDRA Exit... \n");
        #endif
     
        if (numTCFail == 0U)
        {
            SciApp_printf("\nAll tests have PASSED.\n");
        }
        else
        {
            SciApp_printf("\nSome of the Test-cases have FAILED.\n");
        }

    }
}

uint32_t SciApp_getNum(void)
{
    uint32_t number = 0U, numberOfBytes = 0U;
    char buffer[SCI_APP_PRINT_BUFFER_SIZE];
    
    numberOfBytes = UART_gets(buffer, SCI_APP_PRINT_BUFFER_SIZE);
    if (numberOfBytes != 0U)
    {
        sscanf(buffer, "%u", &number);
    }
    
    return number;
}

void SciApp_consoleInit(void)
{
    Board_initCfg   boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);
}


int32_t SciApp_pmicShutdown(void)
{
    int32_t ret = CSL_PASS;
    Board_STATUS status = -1;

    status = Board_pmPowerOff(0x48);
    if (status != 0) {
        ret = CSL_EFAIL;
    }

#if defined(SOC_J721E)
    if (ret == CSL_PASS) {
        status = Board_pmPowerOff(0x4C);
        if (status != 0) {
            ret = CSL_EFAIL;
        }
    }
#endif

    return ret;
}
/* ========================================================================== */
/*                     Internal Function Definitions                          */
/* ========================================================================== */

static int32_t SciApp_getIndex(uint32_t testcaseId)
{
    int32_t  testcaseIdx = -1;
    uint32_t testCnt;
    const SciApp_TestParams_t *testParams;

    testParams = &gSciApp_TestcaseParams[0u];
    for (testCnt = 0U; testCnt < SciApp_getNumTests(); testCnt++)
    {
        if (testParams[testCnt].testcaseId == testcaseId)
        {
            testcaseIdx = testCnt;
            break;
        }
    }
    
    return testcaseIdx;
}

static void SciApp_run(SciApp_TestParams_t *testParams)
{
    SciApp_printf( "\n|TEST START|:: %u ::\n", testParams->testcaseId);
    SciApp_printf( "|TEST PARAM|:: %s ::\n", testParams->testCaseName);
    SciApp_printf( "|SR|:: %s ::\n", testParams->reqId);
    SciApp_printf("--------------------------------------------------------------\n");
    SciApp_printf("=========================RUNNING==============================\n");
    SciApp_testMain(testParams);
    testParams->isRun = CSL_PASS;
    SciApp_printf("\n======================FINISHED==========================\n");
    if (testParams->testResult == 0U)
    {
        SciApp_printf( "|TEST RESULT|PASS|%u|\n", testParams->testcaseId);
    }
    else
    {
        SciApp_printf( "|TEST RESULT|FAIL|%u|\n", testParams->testcaseId);
    }
    SciApp_printf( "|TEST INFO|:: %s ::\n", testParams->testCaseName);
    SciApp_printf( "|TEST END|:: %u ::\n", testParams->testcaseId);
}

static void SciApp_showMainMenu(void)
{
    SciApp_printf("\n=====================\n");
    SciApp_printf("SCI UT Select\n");
    SciApp_printf("=======================\n");
    SciApp_printf("\nSCI UT main menu:\n");
    SciApp_printf("1: Manual testing (select specific test case to run)\n");
    SciApp_printf("2. Sanity testing.\n");
    SciApp_printf("d: Display test cases.\n");
    SciApp_printf("t: Display test case Details.\n");
    SciApp_printf("g: Generate test report.\n");
    SciApp_printf("q: Quit.\n");
}

static void SciApp_printTestName(void)
{
    uint32_t loopCnt;
    const SciApp_TestParams_t *testParams;

    SciApp_printf( "SCI Testcase:\n");
    SciApp_printf("--------------------------------------------------------------\n");
    SciApp_printf( "TC Id\tTC name\n");
    SciApp_printf("--------------------------------------------------------------\n");

    for (loopCnt = 0U; loopCnt < SciApp_getNumTests(); loopCnt++)
    {
        testParams = &gSciApp_TestcaseParams[loopCnt];
        SciApp_printf( "%d\t%s\n", testParams->testcaseId, testParams->testCaseName);
    }

    SciApp_printf("--------------------------------------------------------------\n");
}

static void SciApp_printTestDetails(uint32_t testId)
{
    const SciApp_TestParams_t *testParams;

    testParams = &gSciApp_TestcaseParams[testId];
    SciApp_printf("\nTestcase Id : %d\n", testParams->testcaseId);
    SciApp_printf("Testcase Name : %s", testParams->testCaseName);
    SciApp_printf("\nTestcase Enabled(0-Disabled/1-Enabled) : %d", testParams->enableTest);
    SciApp_printf("\nTestcase Req. Id : %s", testParams->reqId);
    SciApp_printf("\nTestcase User Info. : %s", testParams->userInfo);
    SciApp_printf("\nTestcase Disable Reason : %s", testParams->disableReason);
    SciApp_printf("\nTestcase CPU ID : %d", testParams->cpuID);
    SciApp_printf("\nTestcase Type : %d", testParams->testType);
    SciApp_printf("\nTestcase Print Enable(0-Disabled/1-Enabled) : %d\n", testParams->printEnable);
}

static void SciApp_resultInit(void)
{
    uint32_t loopCnt;
    SciApp_TestParams_t *testParams;

    for (loopCnt = 0U; loopCnt < SciApp_getNumTests(); loopCnt++)
    {
        testParams             = &gSciApp_TestcaseParams[loopCnt];
        testParams->isRun      = CSL_EFAIL;
        testParams->testResult = CSL_EFAIL;
    }
}

static void SciApp_printResults(void)
{
    uint32_t loopCnt;
    int32_t  testcaseIdx;
    SciApp_TestParams_t *testParams;
    char  testResult[10];

    SciApp_printf("-----------------------------------------------------------------------------------------------------------\r\n");
    SciApp_printf("ID\t         Description\t\t\t                         Status\r\n");
    SciApp_printf("-----------------------------------------------------------------------------------------------------------\r\n");
    for (loopCnt = 0U; loopCnt < SciApp_getNumTests(); loopCnt++)
    {
        testParams  = &gSciApp_TestcaseParams[loopCnt];
        testcaseIdx = testParams->testcaseId;
        if (CSL_PASS == testParams->testResult)
        {
            strcpy(testResult, "PASS");
        }
        else if (CSL_PASS == testParams->isRun)
        {
            strcpy(testResult, "FAIL");
        }
        else
        {
            strcpy(testResult, "NRY");
        }
        SciApp_printf("%d\t         %s\t                         ",testcaseIdx, testParams->testCaseName);
        SciApp_printf("%s\r\n", testResult);
    }

    SciApp_printf("\n-----------------------------------------------------------------------------------------------------------\r\n");
}

