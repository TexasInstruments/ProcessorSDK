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
 *  \file enet_test_common.c
 *
 *  \brief Common code that can be shared across test case files.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "enet_test.h"
#include "trace.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define APPUTILS_UART_INPUT

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

static uint32_t gEnetTestCounter = 0U;
static uint32_t gCpswPassCounter = 0U;
static uint32_t gCpswFailCounter = 0U;

/** \brief Log enable for CPSW Unit Test  application */
extern uint32_t gAppTrace;
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  enetTestInitVariables
 *  Initialize the global variables and pointers.
 */
int32_t enetTestInitVariables(const EnetTestParams *testParams)
{
    int32_t retVal = ENET_SOK;

    return retVal;
}

/**
 *  enetTestFreeVariables
 *  Free the allocated handles and buffers.
 */
void enetTestFreeVariables(const EnetTestParams *testParams)
{
    return;
}

void enetTestLogTestResult(const EnetTestParams *testParams,
                           int32_t testResult,
                           uint32_t testCaseId,
                           char *testcaseInfo)
{
    if (ENET_SOK == testResult)
    {
        gCpswPassCounter++;
        GT_1trace(gAppTrace, GT_INFO,
                  " |TEST RESULT|PASS|%d|\r\n", testCaseId);
    }
    else
    {
        gCpswFailCounter++;
        GT_1trace(gAppTrace, GT_INFO,
                  " |TEST RESULT|FAIL|%d|\r\n", testCaseId);
    }

    gEnetTestCounter++;
    GT_1trace(gAppTrace, GT_INFO,
              " |TEST END|:: %d ::\r\n", testCaseId);

    return;
}

void enetTestPrintTestResult(const EnetTestParams *testParams,
                             uint32_t skipCount,
                             uint32_t disableCount)
{
    GT_0trace(gAppTrace, GT_INFO,
              "\r\n *********Test Case Statistics*****************");
    GT_1trace(gAppTrace, GT_INFO,
              "\r\n      Total Test Case         : %d",
              (gEnetTestCounter + skipCount + disableCount));
    GT_1trace(gAppTrace, GT_INFO,
              "\r\n      Total Test Case Executed: %d", gEnetTestCounter);
    GT_1trace(gAppTrace, GT_INFO,
              "\r\n      Total Test Case Passed  : %d", gCpswPassCounter);
    GT_1trace(gAppTrace, GT_INFO,
              "\r\n      Total Test Case Failed  : %d", gCpswFailCounter);
    GT_1trace(gAppTrace, GT_INFO,
              "\r\n      Total Test Case Skipped : %d", skipCount);
    GT_1trace(gAppTrace, GT_INFO,
              "\r\n      Total Test Case Disabled: %d", disableCount);
    GT_0trace(gAppTrace, GT_INFO,
              "\r\n *********************************************\r\n \r\n");

    if (gCpswFailCounter > 0U)
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

void enetTestResetTestResult(void)
{
    gEnetTestCounter = 0U;
    gCpswPassCounter = 0U;
    gCpswFailCounter = 0U;

    return;
}

int32_t AppUtils_getCharTimeout(char *ch,
                                uint32_t msec)
{
    int32_t retVal = ENET_EFAIL;

#if defined(APPUTILS_UART_INPUT)
/* Timeout not yet supported in UART console */
#if 0
    uint32_t startTime, elaspsedTime;

    startTime = AppUtils_getCurTimeInMsec();
    do
    {
        retVal = UARTGetcNonBlocking((uint8 *)ch);
        if (retVal == 0U)
        {
            retVal = ENET_SOK;
            break;
        }

        retVal       = ENET_EFAIL;
        elaspsedTime = AppUtils_getElapsedTimeInMsec(startTime);
    }
    while (elaspsedTime <= msec);

#else
    UART_scanFmt("%c", ch);
    retVal = ENET_SOK;
#endif
#else
    /* Timeout not supported by stdio scanf */
    scanf("%c", ch);
    retVal = ENET_SOK;
#endif

    return retVal;
}

uint32_t AppUtils_getCurTimeInMsec(void)
{
    uint64_t curTimeMsec, curTimeUsec;

    curTimeUsec = TimerP_getTimeInUsecs();
    curTimeMsec = (curTimeUsec / 1000U);

    return((uint32_t)curTimeMsec);
}

uint32_t AppUtils_getElapsedTimeInMsec(uint32_t startTime)
{
    uint32_t elapsedTimeMsec = 0U, currTime;

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

    return(elapsedTimeMsec);
}
