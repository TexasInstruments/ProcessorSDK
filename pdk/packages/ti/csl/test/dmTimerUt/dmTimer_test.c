/* Copyright (c) 2019-2020 Texas Instruments Incorporated
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
 *  \file     dmTimer_test.c
 *
 *  \brief    This file contains dmTimer test code.
 *
 *  \details  dmTimer API and functional tests
 **/

/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/
#include <dmTimer_test.h>

#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/*===========================================================================*/
/*                         Declarations                                      */
/*===========================================================================*/
/* None */

/*===========================================================================*/
/*                         Macros                                            */
/*===========================================================================*/
/* None */

/*===========================================================================*/
/*                         Internal function declarations                    */
/*===========================================================================*/
static int32_t  cslApp_initBoard(void);

void  test_csl_dmTimer_baremetal_test_app (void);
void  test_csl_dmTimer_baremetal_test_app_runner (void);

/*===========================================================================*/
/*                         Global Variables                                  */
/*===========================================================================*/
cslDmTimerTest_t  cslDmTimerTestList[] = {
    {csldmTimer_funcTest,       "PDK-6041: DMTIMER FUNC TEST", CSL_APP_TEST_NOT_RUN },
    {csldmTimer_apiTest,        "PDK-6052: DMTIMER API TEST" , CSL_APP_TEST_NOT_RUN },
    {csldmTimer_negTest,        "PDK-6052: DMTIMER NEG TEST" , CSL_APP_TEST_NOT_RUN },
    {NULL,                      "TERMINATING CONDITION"        , CSL_APP_TEST_NOT_RUN }
};

/*===========================================================================*/
/*                   Local Function definitions                              */
/*===========================================================================*/

/* initialize the board for the application */
static int32_t  cslApp_initBoard(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    boardStatus = Board_init(boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        cslApp_print("[Error] Board init failed!!\n");
    }
    return (boardStatus);
}

/*===========================================================================*/
/*                         Function definitions                              */
/*===========================================================================*/

void  test_csl_dmTimer_baremetal_test_app (void)
{
    /* Declarations of variables */
    int32_t    testResult = CSL_APP_TEST_PASS;
    int32_t    i;

    /* Init Board */
    cslApp_initBoard();

    cslApp_print("\n DmTimer Test Application\r\n");

    for ( i = 0; cslDmTimerTestList[i].testFunction != NULL; i++)
    {
        testResult = cslDmTimerTestList[i].testFunction();
        cslDmTimerTestList[i].testStatus = testResult;
    }

    testResult = CSL_APP_TEST_PASS;
    for ( i = 0; cslDmTimerTestList[i].testFunction != NULL; i++)
    {
        if (cslDmTimerTestList[i].testStatus != CSL_APP_TEST_PASS)
        {
            UART_printf("Test Name: %s  FAILED \n", cslDmTimerTestList[i].name);
            testResult = CSL_APP_TEST_FAILED;
            break;
        }
        else
        {
            UART_printf("Test Name: %s  PASSED \n", cslDmTimerTestList[i].name);
        }
    }

    if (testResult == CSL_APP_TEST_PASS)
    {
        UART_printStatus("\n All tests have passed. \n");
    }
    else
    {
        UART_printStatus("\n Few/all tests Failed \n");
    }
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_APP_TEST_PASS, testResult);
#endif

    return;
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

void test_csl_dmTimer_baremetal_test_app_runner(void)
{
    /* @description:Test runner for R5 Core tests
       @requirements: PDK-6041, PDK-6052
       @cores: mcu1_0 */

#if defined(UNITY_INCLUDE_CONFIG_H)
    UNITY_BEGIN();
    RUN_TEST (test_csl_dmTimer_baremetal_test_app);
    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
#else
    test_csl_dmTimer_baremetal_test_app();
#endif
    return;
}

int32_t main(void)
{
    test_csl_dmTimer_baremetal_test_app_runner();
    /* Stop the test and wait here */
    while (1);
}
/* Nothing past this point */

