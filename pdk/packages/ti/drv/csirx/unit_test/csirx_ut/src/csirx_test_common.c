/*
 *  Copyright (c) Texas Instruments Incorporated 2019
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
 *  \file csirx_test_common.c
 *
 *  \brief Common code that can be shared across test case files.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <csirx_test.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if !defined (QT_BUILD)
#define APPUTILS_UART_INPUT
#endif

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

static uint32_t gCsirxTestCounter = 0U;
static uint32_t gCsirxPassCounter = 0U;
static uint32_t gCsirxFailCounter = 0U;

/** \brief Log enable for CSIRX Unit Test  application */
extern uint32_t gAppTrace;
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  csirxTestInitVariables
 *  Initialize the global variables and pointers.
 */
int32_t csirxTestInitVariables(const CsirxTestParams *testParams)
{
    int32_t retVal = FVID2_SOK;

    return retVal;
}

/**
 *  csirxTestFreeVariables
 *  Free the allocated handles and buffers.
 */
void csirxTestFreeVariables(const CsirxTestParams *testParams)
{
    return;
}

void csirxTestLogTestResult(const CsirxTestParams *testParams,
                           int32_t                testResult,
                           uint32_t               testCaseId,
                           char                   *testcaseInfo)
{
    if(FVID2_SOK == testResult)
    {
        gCsirxPassCounter++;
        GT_1trace(gAppTrace, GT_INFO,
                  " |TEST RESULT|PASS|%d|\r\n", testCaseId);
    }
    else
    {
        gCsirxFailCounter++;
        GT_1trace(gAppTrace, GT_INFO,
                  " |TEST RESULT|FAIL|%d|\r\n", testCaseId);
    }

    gCsirxTestCounter++;
    GT_1trace(gAppTrace, GT_INFO,
              " |TEST END|:: %d ::\r\n", testCaseId);

    return;
}

void csirxTestPrintTestResult(const CsirxTestParams *testParams,
                             uint32_t           skipCount,
                             uint32_t           disableCount)
{
    GT_0trace(gAppTrace, GT_INFO,
              "\r\n *********Test Case Statistics*****************");
    GT_1trace(gAppTrace, GT_INFO,
              "\r\n      Total Test Case         : %d",
              (gCsirxTestCounter + skipCount + disableCount));
    GT_1trace(gAppTrace, GT_INFO,
              "\r\n      Total Test Case Executed: %d", gCsirxTestCounter);
    GT_1trace(gAppTrace, GT_INFO,
              "\r\n      Total Test Case Passed  : %d", gCsirxPassCounter);
    GT_1trace(gAppTrace, GT_INFO,
              "\r\n      Total Test Case Failed  : %d", gCsirxFailCounter);
    GT_1trace(gAppTrace, GT_INFO,
              "\r\n      Total Test Case Skipped : %d", skipCount);
    GT_1trace(gAppTrace, GT_INFO,
              "\r\n      Total Test Case Disabled: %d", disableCount);
    GT_0trace(gAppTrace, GT_INFO,
              "\r\n *********************************************\r\n \r\n");

    if(gCsirxFailCounter > 0U)
    {
        GT_0trace(gAppTrace, GT_INFO,
                  "\r\n Some tests have failed. \r\n");
    }
    else
    {
        GT_0trace(gAppTrace, GT_INFO,
                  "\r\n All tests have passed. \r\n");
    }

    return;
}

void csirxTestResetTestResult(void)
{
    gCsirxTestCounter = 0U;
    gCsirxPassCounter = 0U;
    gCsirxFailCounter = 0U;

    return;
}

char AppUtils_getChar(void)
{
    char ch;

#if defined (APPUTILS_UART_INPUT)
    UART_scanFmt("%c", &ch);
#else
    scanf("%c", &ch);
#endif

    return (ch);
}

int32_t AppUtils_getCharTimeout(char *ch, uint32_t msec)
{
    int32_t retVal = FVID2_EFAIL;
#if defined (APPUTILS_UART_INPUT)
/* Timeout not yet supported in UART console */
#if 0
    uint32_t startTime, elaspsedTime;

    startTime = AppUtils_getCurTimeInMsec();
    do
    {
        retVal = UARTGetcNonBlocking((uint8 *) ch);
        if(FVID2_SOK == retVal)
        {
            retVal = FVID2_SOK;
            break;
        }
        retVal       = FVID2_EFAIL;
        elaspsedTime = AppUtils_getElapsedTimeInMsec(startTime);
    } while(elaspsedTime <= msec);
#else
    UART_scanFmt("%c", ch);
    retVal = FVID2_SOK;
#endif
#else
    /* Timeout not supported by stdio scanf */
    scanf("%c", ch);
    retVal = FVID2_SOK;
#endif

    return retVal;
}

int32_t AppUtils_getNum(void)
{
    int32_t num;

#if defined (APPUTILS_UART_INPUT)
    UART_scanFmt("%d", &num);
#else
    scanf("%d", &num);
#endif

    return (num);
}

uint32_t AppUtils_getCurTimeInMsec(void)
{
    uint64_t curTimeMsec, curTimeUsec;

    curTimeUsec = TimerP_getTimeInUsecs();
    curTimeMsec = (curTimeUsec / 1000U);

    return ((uint32_t) curTimeMsec);
}

uint32_t AppUtils_getElapsedTimeInMsec(uint32_t startTime)
{
    uint32_t     elapsedTimeMsec = 0U, currTime;

    currTime = AppUtils_getCurTimeInMsec();
    if (currTime < startTime)
    {
        /* Counter overflow occured */
        elapsedTimeMsec = (0xFFFFFFFFU - startTime) + currTime + 1U;
    }
    else
    {
        elapsedTimeMsec = currTime - startTime;
    }

    return (elapsedTimeMsec);
}

void App_consolePrintf(const char *pcString, ...)
{
    char printBuffer[CSIRX_TEST_PRINT_BUFSIZE];
    va_list arguments;

    /* Start the var args processing. */
    va_start(arguments, pcString);
    vsnprintf (printBuffer, sizeof(printBuffer), pcString, arguments);
    printf("%s",printBuffer);
#if !defined (QT_BUILD)
    UART_printf(printBuffer);
#endif
    /* End the var args processing. */
    va_end(arguments);
}

void App_fvidPrint(const char *str, ...)
{
    App_consolePrintf(str);

    return;
}

void App_dmaPrint(const char *str)
{
    App_consolePrintf(str);

    return;
}

void AppUtils_cacheWbInv(const void * addr, uint32_t size)
{
    uint32_t    isCacheCoherent = Udma_isCacheCoherent();

    if(UTRUE != isCacheCoherent)
    {
        CacheP_wbInv(addr, size);
    }

    return;
}
