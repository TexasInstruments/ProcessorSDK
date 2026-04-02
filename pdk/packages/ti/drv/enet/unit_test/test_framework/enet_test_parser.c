/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 *  \file enet_test_parser.c
 *
 *  \brief CPSW test application parser file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "enet_testcases.h"
#include "enet_test_entry.h"
#include "trace.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#define APP_TSK_STACK_MAIN              (20U * 1024U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t enetTestInit(EnetTestParams *testParams);

static int32_t enetTestDeinit(EnetTestParams *testParams);

static int32_t enetTestRunTc(EnetTestParams *testParams);

static int32_t enetTestCreateTestTasks(EnetTestParams *testParams,
                                       uint32_t iteration);

static int32_t enetTestDeleteTestTasks(EnetTestParams *testParams);

static void enetTestTask(void *arg0,
                         void *arg1);

static void enetTestInitTestObj(EnetTestParams *testParams);

static void enetTestFillTestObj(EnetTestParams *testParams);

static int32_t enetTestGetTcIdx(uint32_t tcId,
                                bool isSearchForward);

static bool enetTestCheckIfTestToBeSkipped(EnetTestParams *testParams,
                                           uint32_t tcType);

static int32_t enetTestDisplayTestInfo(EnetTestParams *testParams);

static int32_t enetTestGenerateTestReports(EnetTestParams *testParams);

static void enetTestMenuSettings(EnetTestParams *testParams);

static void enetTestMenuMainShow(EnetTestParams *testParams);

static void enetTestMenuSettingsShow(EnetTestParams *testParams);

static void enetTestMenuCurrentSettingsShow(EnetTestParams *testParams);

static void enetTestSetDefaultCfg(EnetTestParams *testParams);

static uint32_t enetTestGetTestId(EnetTestParams *testParams,
                                  uint32_t tcType);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Task stack */
static uint8_t gCpswParserTskStack[ENET_TEST_MAX_TASKS][
    APP_TSK_STACK_MAIN] __attribute__ ((aligned(32)));;

/* Main menu 0 string */
static const char gEnetTestMenuMain0[] =
{
    "\r\n "
    "\r\n ==============="
    "\r\n CPSW Test Select"
    "\r\n ==============="
    "\r\n "
};

/* Main menu string */
/* Only manual testing is supported */
static const char gEnetTestMenuMain1[] =
{
    "\r\n 1: Manual testing (select specific test case to run)"

/*    "\r\n 2: Sanity (BFT) testing"
 *  "\r\n 3: Regression testing"
 *  "\r\n 4: Full testing"
 *  "\r\n 5: Performance testing"
 *  "\r\n d: Display test cases"
 *  "\r\n g: Generate test report"*/
    "\r\n "
    "\r\n s: System Settings"
    "\r\n "
    "\r\n q: Quit "
    "\r\n "
    "\r\n Enter Choice: "
    "\r\n"
};

static const char gEnetTestMenuSettings0[] =
{
    "\r\n ==============="
    "\r\n System Settings"
    "\r\n ==============="
    "\r\n "
};

static const char gEnetTestMenuSettings1[] =
{
    "\r\n "
    "\r\n l: Loop Count"
    "\r\n d: Queue Depth (How many outstanding transfer per channel)"
    "\r\n r: Runtime Print Enable"
    "\r\n "
    "\r\n q: Quit "
    "\r\n"
};

uint32_t gSkipCount;
/**< Number of test cases skipped because of platform/user settings. */
uint32_t gDisableCount;
/**< Number of test cases disabled because of any bug etc. */

/** \brief Log enable for CPSW Unit Test  application */
extern uint32_t gAppTrace;

extern struct Udma_DrvObj gUdmaDrvObj;
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  enetTestParser
 */
int32_t enetTestParser(void)
{
    char option;
    bool done;
    int32_t retVal = ENET_SOK, getCharRetVal = ENET_SOK;
    int32_t tcIdx, startIdx;
    uint32_t testCnt, tcType, tcId;
    uint32_t startTime, elapsedTime, startTime1, elapsedTime1;
    uint32_t hrs, mins, durationInSecs, secs;

    EnetTestParams *testParams;
    uint32_t isValidInput = UFALSE;

    testParams = &gEnetTestCases[0U];
    enetTestSetDefaultCfg(testParams);
    enetTestResetTestResult();

    retVal = enetTestInit(testParams);
    if (retVal != ENET_SOK)
    {
        GT_0trace(gAppTrace, GT_ERR, " CPSW Test Init failed!!\n");
    }

    if (ENET_SOK != retVal)
    {
        return retVal;
    }

    startTime     = AppUtils_getCurTimeInMsec();
    done          = BFALSE;
    gSkipCount    = 0U;
    gDisableCount = 0U;
    while (!done)
    {
        enetTestMenuMainShow(testParams);
        if (isValidInput != UTRUE)
        {
            getCharRetVal = AppUtils_getCharTimeout(&option,
                                                    ENET_TEST_UART_TIMEOUT_MSEC);
            if (ENET_SOK != getCharRetVal)
            {
                /* Timeout */
                GT_0trace(gAppTrace, GT_INFO,
                          " UART read timeout (10 sec)!!\r\n");
                /* Default to full test */
                GT_0trace(gAppTrace, GT_INFO, " Automating FULL test!!\r\n");
                option = '4';
                /* Set done flag to end test */
                done = BTRUE;
            }
            else
            {
                isValidInput = UTRUE;
            }
        }
        else
        {
            option = EnetAppUtils_getChar();
        }

        GT_0trace(gAppTrace, GT_INFO, " \r\n");

        tcType = ENET_TCT_SANITY;
        switch (option)
        {
            case 's':
            case 'S':
                enetTestMenuSettings(testParams);
                break;

            case 'q':
            case 'Q':
                done = BTRUE;
                break;

            case '1':
                GT_0trace(gAppTrace, GT_INFO, " Manual testing \r\n");
                tcId = enetTestGetTestId(testParams, ENET_TCT_ALL);
                /* Find the test case to run */
                tcIdx = enetTestGetTcIdx(tcId, BFALSE);
                if (tcIdx < 0)
                {
                    GT_0trace(gAppTrace, GT_INFO,
                              " Test case ID not found\r\n");
                }
                else
                {
                    testParams = &gEnetTestCases[tcIdx];
                    enetTestRunTc(testParams);
                }

                break;

            case '4':
                tcType |= ENET_TCT_FULL;

            case '3':
                tcType |= ENET_TCT_REGRESSION;

            case '2':
                if (BTRUE == done)
                {
                    /* Auto run, start from 1st test case */
                    tcId = 0U;
                }
                else
                {
                    GT_0trace(gAppTrace, GT_INFO,
                              " Enter Start Test Case Id "
                              "(Enter 0 to start from first): \r\n");
                    tcId = enetTestGetTestId(testParams, tcType);
                }

                startIdx = enetTestGetTcIdx(tcId, BTRUE);
                if (startIdx < 0)
                {
                    GT_0trace(gAppTrace, GT_INFO,
                              " Test case ID not found\r\n");
                    continue;
                }

                /* Run all test cases one after the other depending on selected
                 * flag */
                startTime1 = AppUtils_getCurTimeInMsec();
                gSkipCount = gDisableCount = 0U;

                for (testCnt = startIdx;
                     testCnt < ENET_TEST_NUM_TESTCASES;
                     testCnt++)
                {
                    testParams = &gEnetTestCases[testCnt];

                    /* Check whether to execute test or not */
                    if (enetTestCheckIfTestToBeSkipped(testParams,
                                                       tcType))
                    {
                        continue;       /* Skip test */
                    }

                    enetTestRunTc(testParams);
                }

                /* Print test results */
                enetTestPrintTestResult(testParams, gSkipCount, gDisableCount);

                elapsedTime1   = AppUtils_getElapsedTimeInMsec(startTime1);
                durationInSecs = ((elapsedTime1) / 1000U);
                hrs            = durationInSecs / (60U * 60U);
                mins           = (durationInSecs / 60U) - (hrs * 60U);
                secs           = durationInSecs - (hrs * 60U * 60U) - (mins * 60U);
                GT_3trace(gAppTrace, GT_INFO,
                          " |TOTAL TEST DURATION|: %d hrs %d mins %d secs\r\n",
                          hrs, mins, secs);
                break;

            case '5':
                tcType   = ENET_TCT_PERFORMANCE;
                startIdx = enetTestGetTcIdx(0U, BTRUE);
                if (startIdx < 0)
                {
                    GT_0trace(gAppTrace, GT_INFO,
                              " Test case ID not found\r\n");
                    continue;
                }

                /* Run all test cases one after the other depending on selected
                 * flag */
                startTime1 = AppUtils_getCurTimeInMsec();
                gSkipCount = gDisableCount = 0U;
                for (testCnt = startIdx; testCnt < ENET_TEST_NUM_TESTCASES; testCnt++)
                {
                    testParams = &gEnetTestCases[testCnt];

                    /* Check whether to execute test or not */
                    if (enetTestCheckIfTestToBeSkipped(testParams, tcType))
                    {
                        continue;       /* Skip test */
                    }

                    enetTestRunTc(testParams);
                }

                /* Print test results */
                enetTestPrintTestResult(testParams, gSkipCount, gDisableCount);

                elapsedTime1   = AppUtils_getElapsedTimeInMsec(startTime1);
                durationInSecs = ((elapsedTime1) / 1000U);
                hrs            = durationInSecs / (60U * 60U);
                mins           = (durationInSecs / 60U) - (hrs * 60U);
                secs           = durationInSecs - (hrs * 60U * 60U) - (mins * 60U);
                GT_3trace(gAppTrace, GT_INFO,
                          " |TOTAL TEST DURATION|: %d hrs %d mins %d secs\r\n",
                          hrs, mins, secs);
                break;

            case 'd':
            case 'D':
                /* Display test info */
                enetTestDisplayTestInfo(testParams);
                break;

            case 'g':
            case 'G':
                /* Generate test report */
                enetTestGenerateTestReports(testParams);
                break;

            default:
                GT_0trace(gAppTrace, GT_INFO,
                          " Invalid option try again!!\r\n");
                break;
        }
    }

    enetTestDisplayTestInfo(testParams);
    enetTestGenerateTestReports(testParams);

    elapsedTime    = AppUtils_getElapsedTimeInMsec(startTime);
    durationInSecs = ((elapsedTime) / 1000U);
    hrs            = durationInSecs / (60U * 60U);
    mins           = (durationInSecs / 60U) - (hrs * 60U);
    secs           = durationInSecs - (hrs * 60U * 60U) - (mins * 60U);
    GT_3trace(gAppTrace, GT_INFO,
              " |TOTAL UT DURATION|: %d hrs %d mins %d secs\r\n",
              hrs, mins, secs);

    /* Check if all testcases passed and accordingly return pass/fail */
    for (testCnt = 0U; testCnt < ENET_TEST_NUM_TESTCASES; testCnt++)
    {
        testParams = &gEnetTestCases[testCnt];
        if (BTRUE == testParams->isRun)
        {
            if (ENET_SOK != testParams->testResult)
            {
                retVal = testParams->testResult;
            }
        }
    }

    retVal = enetTestDeinit(testParams);
    if (ENET_SOK != retVal)
    {
        GT_0trace(gAppTrace, GT_ERR, " CPSW Test deinit failed!!\n");
    }

    /* Print test results */
    enetTestPrintTestResult(testParams, gSkipCount, gDisableCount);

    return retVal;
}

/**
 *  enetTestRunTc
 */
static int32_t enetTestRunTc(EnetTestParams *testParams)
{
    static char tempString[ENET_TEST_PRINT_BUFSIZE];
    int32_t retVal    = ENET_SOK;
    int32_t retValDel = ENET_SOK;
    uint32_t startTime, hrs, mins, secs, msecs, loopCnt;
    uint32_t durationInMsecs, durationInSecs;
    static char *enableDisableName[] = {"Disabled", "Enabled"};

    /* NULL pointer check */
    GT_assert(gAppTrace, (NULL != testParams));
    GT_assert(gAppTrace, (NULL != testParams->tcName));

    GT_0trace(gAppTrace, GT_INFO, " \r\n");
    GT_1trace(gAppTrace, GT_INFO, " |TEST START|:: %d ::\r\n", testParams->tcId);
    GT_1trace(gAppTrace, GT_INFO, " |TEST NAME| :: %s ::\r\n", testParams->tcName);
    GT_1trace(gAppTrace, GT_INFO, " |TEST INFO| :: Num Tasks         : %d ::\r\n",
              testParams->numTasks);
    GT_1trace(gAppTrace, GT_INFO, " |TEST INFO| :: Data Check        : %s ::\r\n",
              enableDisableName[testParams->dcEnable]);
    GT_1trace(gAppTrace, GT_INFO, " |TEST INFO| :: Profiling         : %s ::\r\n",
              enableDisableName[testParams->profilingEnable]);
    GT_1trace(gAppTrace, GT_INFO, " |TEST INFO| :: Print             : %s ::\r\n",
              enableDisableName[testParams->printEnable]);
    GT_1trace(gAppTrace, GT_INFO, " |TEST INFO| :: Run Instructions  : %s ::\r\n\n\n",
              testParams->tcRunInfo);

    if (enetTestCheckIfTestToBeSkipped(testParams, ENET_TCT_FUNCTIONAL) == BFALSE)
    {
        enetTestInitTestObj(testParams);
        /* Start the load calculation */
        // Utils_prfLoadCalcStart();

        testParams->isRun = BTRUE;
        durationInMsecs   = 0;
        for (loopCnt = 0U; loopCnt < testParams->iterationCnt; loopCnt++)
        {
            GT_1trace(gAppTrace, GT_INFO,
                      " |TEST ITEARATION NUMBER| ::      : %d ::\r\n", (loopCnt + 1U));

            startTime       = AppUtils_getCurTimeInMsec();
            retVal          = enetTestCreateTestTasks(testParams, loopCnt);
            durationInMsecs = AppUtils_getElapsedTimeInMsec(startTime);
            if (retVal != ENET_SOK)
            {
                GT_0trace(gAppTrace, GT_ERR, APP_NAME ": Failed in task creation!!!\r\n");
            }

            // Utils_prfLoadCalcStop();
            // Utils_prfLoadPrintAll(TRUE, gAppTrace);
            // Utils_prfLoadCalcReset();
            /* Delete task even if test failed to avoid memory leak */
            retValDel = enetTestDeleteTestTasks(testParams);
            if (retValDel != ENET_SOK)
            {
                GT_0trace(gAppTrace, GT_ERR, APP_NAME ": Failed in task deletion!!!\r\n");
                break;
            }

            if ((retVal != ENET_SOK) || (retValDel != ENET_SOK))
            {
                break;
            }
        }

        durationInSecs = (durationInMsecs / 1000U);
        hrs            = durationInSecs / (60U * 60U);
        mins           = (durationInSecs / 60U) - (hrs * 60U);
        secs           = durationInSecs - (hrs * 60U * 60U) - (mins * 60U);
        msecs          = durationInMsecs - ((hrs * 60U * 60U) + (mins * 60U) + secs) * 1000U;
        GT_4trace(gAppTrace, GT_INFO, " |TEST DURATION|:: %d:%0.2d:%0.2d:%0.3d ::\r\n",
                  hrs, mins, secs, msecs);

        if ((BTRUE == testParams->profilingEnable) && (testParams->numTasks > 1U))
        {
            /* Total performance applicable only for multi thread.
             * Incase of multi thread, individual performance doesn't makes sense
             * Incase of single thread, the thread itself will print the accurate
             * performance. Total is not very accurate. Hence disable from here */
            // enetTestCalcTotalPerformance(testParams, durationInMsecs);
        }

        GT_assert(gAppTrace, (NULL != testParams->tcName));
        snprintf(tempString, sizeof(tempString), "%s", testParams->tcName);
        enetTestLogTestResult(testParams, retVal, testParams->tcId, tempString);

        if (ENET_SOK == retVal)
        {
            GT_1trace(gAppTrace, GT_INFO, " |TEST RESULT|:: CPSW Test Case %d Successful!! ::\r\n",
                      testParams->tcId);
        }
        else
        {
            GT_1trace(gAppTrace, GT_INFO, " |TEST RESULT|:: CPSW Test Case %d Failed!! ::\r\n",
                      testParams->tcId);
        }

        /* Store the test result */
        testParams->testResult = retVal;
    }
    else
    {
        GT_1trace(gAppTrace, GT_INFO, "|TEST RESULT|:: CPSW Test Case %d Skipped!! ::\r\n",
                  testParams->tcId);
        testParams->testResult = ENET_SOK;
    }

    return retVal;
}

int32_t enetTestCreateTestTasks(EnetTestParams *testParams,
                                uint32_t iteration)
{
    int32_t retVal = ENET_SOK;
    uint32_t taskCnt, createdTsk = 0U;
    TaskP_Params taskPrms;
    EnetTestTaskObj *taskObj;
    SemaphoreP_Params semPrms;

    SemaphoreP_Params_init(&semPrms);
    semPrms.mode                = SemaphoreP_Mode_COUNTING;
    testParams->taskCompleteSem = SemaphoreP_create(0U, &semPrms);
    if (NULL == testParams->taskCompleteSem)
    {
        GT_0trace(gAppTrace, GT_ERR, " Sem create failed\r\n");
        retVal = ENET_EALLOC;
    }

    if (ENET_SOK == retVal)
    {
        for (taskCnt = 0U; taskCnt < testParams->numTasks; taskCnt++)
        {
            taskObj                        = &testParams->taskObj[taskCnt];
            taskObj->stateObj.curIteration = iteration;

            TaskP_Params_init(&taskPrms);
            taskPrms.arg0       = taskObj;
            taskPrms.stack      = &gCpswParserTskStack[0U];
            taskPrms.stacksize  = APP_TSK_STACK_MAIN;
            taskObj->taskHandle = TaskP_create(&enetTestTask,
                                               &taskPrms);
            if (NULL == taskObj->taskHandle)
            {
                retVal = ENET_EALLOC;
                break;
            }

            createdTsk++;
            /* Register the task to the load module for calculating the load */
            // snprintf(taskObj->prfTsName, sizeof (taskObj->prfTsName),
            // "Task%d", taskCnt);
            // Utils_prfLoadRegister(taskObj->taskHandle, taskObj->prfTsName);
        }
    }

    if (ENET_SOK == retVal)
    {
        for (taskCnt = 0U; taskCnt < createdTsk; taskCnt++)
        {
            /* Wait for tasks to complete */
            SemaphoreP_pend(testParams->taskCompleteSem, SemaphoreP_WAIT_FOREVER);
        }
    }

    if (ENET_SOK == retVal)
    {
        for (taskCnt = 0U; taskCnt < createdTsk; taskCnt++)
        {
            retVal += testParams->taskObj[taskCnt].testResult;
        }
    }

    if (NULL != testParams->taskCompleteSem)
    {
        SemaphoreP_delete(testParams->taskCompleteSem);
        testParams->taskCompleteSem = NULL;
    }

    return retVal;
}

static int32_t enetTestDeleteTestTasks(EnetTestParams *testParams)
{
    int32_t retVal = ENET_SOK;
    uint32_t taskCnt;
    EnetTestTaskObj *taskObj;

    /* Delete all created tasks and semephores */
    for (taskCnt = 0U; taskCnt < testParams->numTasks; taskCnt++)
    {
        taskObj = &testParams->taskObj[taskCnt];
        if (NULL != taskObj->taskHandle)
        {
            // Utils_prfLoadUnRegister(taskObj->taskHandle);
            TaskP_delete(&taskObj->taskHandle);
            taskObj->taskHandle = NULL;
        }
    }

    return retVal;
}

static void enetTestTask(void *arg0,
                         void *arg1)
{
    int32_t retVal;
    EnetTestTaskObj *taskObj;
    uint32_t totalFreeSpacePreTest;
    uint32_t totalFreeSpacePostTest;

    /* Run the test */
    taskObj = (EnetTestTaskObj *)arg0;

    totalFreeSpacePreTest = Utils_memGetAllHeapFreeSpace();
    /* Run the test */
    retVal                 = CpswUt_testEntry(taskObj);
    totalFreeSpacePostTest = Utils_memGetAllHeapFreeSpace();

    GT_assert(gAppTrace, (totalFreeSpacePostTest == totalFreeSpacePreTest));
    // TODO: we do not have support for Task Stat in FreeRTOS OSAL
    taskObj->testResult = retVal;

    /* Test complete. Signal it */
    SemaphoreP_post(taskObj->testParams->taskCompleteSem);

    return;
}

static int32_t enetTestInit(EnetTestParams *testParams)
{
    int32_t retVal = ENET_SOK;

    /* setup memories */
    retVal = Utils_memInit();

    return retVal;
}

static int32_t enetTestDeinit(EnetTestParams *testParams)
{
    int32_t retVal = ENET_SOK;

    retVal = Utils_memDeInit();

    return retVal;
}

static void enetTestInitTestObj(EnetTestParams *testParams)
{
    uint32_t taskCnt;

    gAppTrace = (GT_INFO1 | GT_TraceState_Enable);
    if (BFALSE == testParams->printEnable)
    {
        /* Restrict the number of prints when print is disabled */
        gAppTrace = (GT_INFO | GT_TraceState_Enable);
    }

    GT_assert(gAppTrace, (testParams->numTasks <= ENET_TEST_MAX_TASKS));
    testParams->taskCompleteSem = NULL;

    /* Prepare task object here,
     * return values if ignored as it is always PASS */
    enetTestFillTestObj(testParams);
    for (taskCnt = 0U; taskCnt < testParams->numTasks; taskCnt++)
    {
        /* Initialize synch semaphore to NULL */
        testParams->taskObj[taskCnt].syncSem = NULL;
        /* reference to test parameters */
        testParams->taskObj[taskCnt].testParams = testParams;

        testParams->taskObj[taskCnt].taskHandle = NULL;
    }

    return;
}

/**
 *  enetTestGetTcIdx
 */
static int32_t enetTestGetTcIdx(uint32_t tcId,
                                bool isSearchForward)
{
    int32_t tcIdx = -1;
    uint32_t testCnt;
    EnetTestParams *testParams;

    testParams = &gEnetTestCases[0U];
    for (testCnt = 0U; testCnt < ENET_TEST_NUM_TESTCASES; testCnt++)
    {
        if (BTRUE == isSearchForward)
        {
            if (testParams->tcId >= tcId)
            {
                tcIdx = testCnt;
                break;
            }
        }
        else
        {
            if (testParams->tcId == tcId)
            {
                tcIdx = testCnt;
                break;
            }
        }

        testParams++;
    }

    return(tcIdx);
}


static bool enetTestCheckCpswTypeSupported(EnetTestParams *testParams)
{
    EnetTestTaskCfg *taskCfg;
    bool supported = BFALSE;

    taskCfg = Test_getTaskCfgParams(testParams->taskCfgId[0]);
    if (NULL != taskCfg)
    {
        supported = EnetSoc_isIpSupported(taskCfg->enetType, taskCfg->instId);
    }

    return supported;
}

/**
 *  enetTestCheckIfTestToBeSkipped
 */
static bool enetTestCheckIfTestToBeSkipped(EnetTestParams *testParams,
                                           uint32_t tcType)
{
    bool skipTest = BFALSE;

    /* Check whether test case is disabled */
    if (BFALSE == testParams->enableTest)
    {
        GT_assert(gAppTrace, (NULL != testParams->tcName));
        GT_0trace(gAppTrace, GT_INFO, " \r\n");
        GT_1trace(gAppTrace, GT_INFO, " |TEST DISABLED|:: %d ::\r\n", testParams->tcId);
        GT_1trace(gAppTrace, GT_INFO, " |TEST PARAM|:: %s ::\r\n", testParams->tcName);
        if (NULL != testParams->disableInfo)
        {
            GT_1trace(gAppTrace, GT_INFO, " |TEST DISABLE REASON|:: %s ::\r\n",
                      testParams->disableInfo);
        }
        else
        {
            GT_0trace(gAppTrace, GT_INFO, " |TEST DISABLE REASON|:: Not Provided!! ::\r\n");
        }

        gDisableCount++;
        skipTest = BTRUE;        /* Skip test */
    }

    /* Ignore test case depending on test flag and selected option */
    if ((BFALSE == skipTest) && (ENET_TCT_MISC != testParams->tcType))
    {
        if (!(tcType & testParams->tcType))
        {
            gSkipCount++;
            skipTest = BTRUE;    /* Skip test */
        }

        if (enetTestCheckCpswTypeSupported(testParams) != BTRUE)
        {
            gSkipCount++;
            skipTest = BTRUE;    /* Skip test */
        }
    }

    /* Ignore test case depending on run flag */
    if (BFALSE == skipTest)
    {
    }

    return(skipTest);
}

/**
 *  enetTestDisplayTestInfo
 */
static int32_t enetTestDisplayTestInfo(EnetTestParams *testParams)
{
    uint32_t sCnt, testCnt;
    char *runStatus;
    EnetTestParams *testPrms;
    static char *enableDisableName[] = {"Disabled", "Enabled"};
    static char printBuf[ENET_TEST_PRINT_BUFSIZE];

    /* Display test info */
    sCnt = 1U;
    GT_0trace(gAppTrace, GT_INFO, " \r\n");
    GT_0trace(gAppTrace, GT_INFO,
              " S.No        ID         Description                                   "
              "                              Status    Auto Run\r\n");
    GT_0trace(gAppTrace, GT_INFO, " ----------------------------------------------------"
              "-----------------------------------------------------\r\n");
    for (testCnt = 0U; testCnt < ENET_TEST_NUM_TESTCASES; testCnt++)
    {
        testPrms = &gEnetTestCases[testCnt];

        runStatus = "NRY";
        if (BFALSE == testPrms->isRun)
        {
            if (BFALSE == testPrms->enableTest)
            {
                runStatus = "NRQ";
            }
        }
        else
        {
            if (ENET_SOK == testPrms->testResult)
            {
                runStatus = "PASS";
            }
            else
            {
                runStatus = "FAIL";
            }
        }

        GT_assert(gAppTrace, (NULL != testPrms->tcName));
        snprintf(printBuf, sizeof(printBuf),
                 "  %3d  PDK-%-4d  %-75.75s  %-8.8s  %-8.8s",
                 sCnt,
                 testPrms->tcId,
                 testPrms->tcName,
                 runStatus,
                 enableDisableName[testPrms->enableTest]);
        GT_1trace(gAppTrace, GT_INFO, "%s\r\n", printBuf);

        sCnt++;
    }

    GT_0trace(gAppTrace, GT_INFO, " \r\n");

    return(ENET_SOK);
}

/**
 *  enetTestGenerateTestReports
 */
static int32_t enetTestGenerateTestReports(EnetTestParams *testParams)
{
    uint32_t sCnt, testCnt;
    char *runStatus, *category, *adequacy;
    EnetTestParams *testPrms;
    static char printBuf[ENET_TEST_PRINT_BUFSIZE];

    sCnt = 1U;
    GT_0trace(gAppTrace, GT_INFO, " \r\n");
    GT_0trace(gAppTrace, GT_INFO,
              "S.No;ID;Requirement Mapping;Description;Pass Fail Criteria;"
              "IR;Category;Test Adequacy;Test Result;\r\n");
    for (testCnt = 0U; testCnt < ENET_TEST_NUM_TESTCASES; testCnt++)
    {
        testPrms = &gEnetTestCases[testCnt];

        runStatus = "NRY";
        if (BFALSE == testPrms->isRun)
        {
            if (BFALSE == testPrms->enableTest)
            {
                runStatus = "NRQ";
            }
        }
        else
        {
            if (ENET_SOK == testPrms->testResult)
            {
                runStatus = "PASS";
            }
            else
            {
                runStatus = "FAIL";
            }
        }

        if (testPrms->tcType & ENET_TCT_FULL)
        {
            category = "Full";
        }
        else if (testPrms->tcType & ENET_TCT_REGRESSION)
        {
            category = "Regression";
        }
        else
        {
            category = "Sanity";
        }

        if (testPrms->tcType & ENET_TCT_STRESS)
        {
            adequacy = "Stress";
        }
        else if (testPrms->tcType & ENET_TCT_NEGATIVE)
        {
            adequacy = "Negative";
        }
        else if (testPrms->tcType & ENET_TCT_PERFORMANCE)
        {
            adequacy = "Performance";
        }
        else if (testPrms->tcType & ENET_TCT_MISC)
        {
            adequacy = "Misc";
        }
        else if (testPrms->tcType & ENET_TCT_API)
        {
            adequacy = "Api";
        }
        else
        {
            adequacy = "Functional";
        }

        GT_assert(gAppTrace, (NULL != testPrms->tcName));
        snprintf(printBuf, sizeof(printBuf),
                 "%d;"
                 "PDK-%d;"
                 "%s;"
                 "%s;"
                 "%s;"
                 "%s;"
                 "%s",
                 sCnt,
                 testPrms->tcId,
                 testPrms->reqMapping,
                 testPrms->tcName,
                 category,
                 adequacy,
                 runStatus);
        GT_1trace(gAppTrace, GT_INFO, "%s\r\n", printBuf);
        sCnt++;
    }

    GT_0trace(gAppTrace, GT_INFO, " \r\n");

    return(ENET_SOK);
}

/**
 *  enetTestMenuSettings
 */
static void enetTestMenuSettings(EnetTestParams *testParams)
{
    enetTestMenuSettingsShow(testParams);

    return;
}

/**
 *  enetTestMenuMainShow
 */
static void enetTestMenuMainShow(EnetTestParams *testParams)
{
    GT_0trace(gAppTrace, GT_INFO, gEnetTestMenuMain0);
    enetTestMenuCurrentSettingsShow(testParams);
    GT_0trace(gAppTrace, GT_INFO, gEnetTestMenuMain1);

    return;
}

/**
 *  enetTestMenuSettingsShow
 */
static void enetTestMenuSettingsShow(EnetTestParams *testParams)
{
    GT_0trace(gAppTrace, GT_INFO, gEnetTestMenuSettings0);
    enetTestMenuCurrentSettingsShow(testParams);
    GT_0trace(gAppTrace, GT_INFO, gEnetTestMenuSettings1);

    return;
}

/**
 *  enetTestMenuCurrentSettingsShow
 */
static void enetTestMenuCurrentSettingsShow(EnetTestParams *testParams)
{
    // static char       *enableDisableName[] = {"OFF", "ON"};

    GT_0trace(gAppTrace, GT_INFO, "\r\n Current/Default System Settings:");
    GT_0trace(gAppTrace, GT_INFO, "\r\n ------------------------");

    GT_1trace(gAppTrace, GT_INFO, "\r\n Uart timemout          : %d msec",
              ENET_TEST_UART_TIMEOUT_MSEC);
    GT_1trace(gAppTrace, GT_INFO, "\r\n Default Iteration count            : %d",
              ENET_TEST_DEF_ITERATION_CNT);
    GT_0trace(gAppTrace, GT_INFO, "\r\n ");

    return;
}

/**
 *  enetTestSetDefaultCfg
 */
static void enetTestSetDefaultCfg(EnetTestParams *testParams)
{
    uint32_t testCnt;

    /* Mark all test cases as not run and set result to PASS */
    for (testCnt = 0U; testCnt < ENET_TEST_NUM_TESTCASES; testCnt++)
    {
        testParams             = &gEnetTestCases[testCnt];
        testParams->isRun      = BFALSE;
        testParams->testResult = ENET_SOK;
    }

    return;
}

/**
 *  \brief Return the test ID to run.
 */
static uint32_t enetTestGetTestId(EnetTestParams *testParams,
                                  uint32_t tcType)
{
    uint32_t testCnt;
    static int32_t testId = 0U;
    EnetTestParams *testPrms;

    GT_0trace(gAppTrace, GT_INFO, "\r\n");
    GT_0trace(gAppTrace, GT_INFO, " --------------------------------------\n");
    GT_0trace(gAppTrace, GT_INFO, " Select test to run as per below table:\n");
    GT_0trace(gAppTrace, GT_INFO, " --------------------------------------\n");
    GT_0trace(gAppTrace, GT_INFO, " \n");
    for (testCnt = 0U; testCnt < ENET_TEST_NUM_TESTCASES; testCnt++)
    {
        testPrms = &gEnetTestCases[testCnt];
        if (BFALSE != testPrms->enableTest && (tcType & testPrms->tcType))
        {
            GT_assert(gAppTrace, (NULL != testPrms->tcName));
            GT_2trace(gAppTrace, GT_INFO, "%5d: %s\n", gEnetTestCases[testCnt].tcId,
                      gEnetTestCases[testCnt].tcName);
        }
    }

    GT_0trace(gAppTrace, GT_INFO, " \n");
    GT_0trace(gAppTrace, GT_INFO,
              " Enter Test to Run (Use UART1 console for all cores or MCU_UART1 console for MCU) \n");

    while (UTRUE)
    {
        testId = EnetAppUtils_getNum();
        GT_1trace(gAppTrace, GT_INFO, "%d\n", testId);
        for (testCnt = 0U; testCnt < ENET_TEST_NUM_TESTCASES; testCnt++)
        {
            if ((tcType & ENET_TCT_FULL) && (tcType != ENET_TCT_ALL))
            {
                if (0U == testId)
                {
                    break;
                }
            }

            if (testId == gEnetTestCases[testCnt].tcId)
            {
                break;
            }
        }

        if (testCnt == ENET_TEST_NUM_TESTCASES)
        {
            GT_0trace(gAppTrace, GT_INFO,
                      "Invalid Test ID. Enter Again!!\n");
        }
        else
        {
            break;
        }
    }

    return(testId);
}

static void enetTestFillTestObj(EnetTestParams *testParams)
{
    uint32_t taskIdx;
    uint32_t i;
    EnetTestTaskObj *taskObj;
    EnetTestStateObj *stateObj;
    EnetTestTaskCfg *taskCfg;

    for (taskIdx = 0U; taskIdx < testParams->numTasks; taskIdx++)
    {
        taskObj             = &testParams->taskObj[taskIdx];
        stateObj            = &taskObj->stateObj;
        taskObj->testParams = testParams;
        taskObj->taskCfg    = Test_getTaskCfgParams(testParams->taskCfgId[taskIdx]);

        if (NULL != taskObj->taskCfg)
        {
            taskCfg = taskObj->taskCfg;
            GT_assert(gAppTrace, (NULL != taskCfg));
            for (i = 0U; i < (taskCfg->numTxCh) && (i < ENET_TEST_CFG_MAX_NUM_TX_CH); i++)
            {
                stateObj->txChCfgInfo[i] = Test_getTxChCfgParams(taskCfg->txChCfgId[i]);
                GT_assert(gAppTrace, (NULL != stateObj->txChCfgInfo[i]));
            }

            for (i = 0U; (i < taskCfg->numRxFlow) && (i < ENET_TEST_CFG_MAX_NUM_RX_FLOWS); i++)
            {
                stateObj->rxFlowCfgInfo[i] = Test_getRxFlowCfgParams(taskCfg->rxFlowCfgId[i]);
                GT_assert(gAppTrace, (NULL != stateObj->rxFlowCfgInfo[i]));
            }
        }
    }
}
