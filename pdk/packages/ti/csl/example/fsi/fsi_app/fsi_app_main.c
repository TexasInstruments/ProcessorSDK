/**
 *  \file   fsi_app_main.c
 *
 *  \brief  FSI test application main file.
 *
 */

/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "fsi_app_test.h"
#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/*
 *  Test function table
 */
FSI_APP_Tests FSI_APP_tests[] =
{
    {
        FSI_App_cpu_control_test,                  /* testFunc */
        FSI_APP_TEST_ID_CPU_CTRL_LOOPBACK_POLLING, /* testID */
        TRUE,                                      /* lbEnable */
        FALSE,                                     /* intEnable */
        "\r\n FSI loopback test in polling mode \r\n "  /* testDesc */
    },
#if defined (BUILD_MCU)
    {
        FSI_App_cpu_control_test,
        FSI_APP_TEST_ID_CPU_CTRL_LOOPBACK_INT,
        TRUE,
        TRUE, /* no FSI interrupt routed to A53/M4, interrupt only supported on R5  */
        "\r\n FSI loopback test in interrupt mode\r\n "
    },
#endif
    {
        NULL,
    }
};

/*
 *  Print test description function
 */
static void FSI_App_print_test_desc(FSI_APP_Tests *pTest)
{
    char testId[16] = {0, };

    /* Print unit test ID */
    sprintf(testId, "%d", pTest->testId);
    FSI_log("\r\n FSI App Test %s\r\n", testId);

    /* Print test description */
    FSI_log("\r\n %s\r\n", pTest->testDesc);
}

/*
 *  Main test function
 */
static void test_csl_fsi_test_app()
{
    uint32_t           i;
    int32_t            testFail = FALSE;
    FSI_APP_Tests     *pTest;

    for (i = 0U; ; i++)
    {
        pTest = &FSI_APP_tests[i];
        if (pTest->testFunc == NULL)
        {
            break;
        }

        FSI_App_print_test_desc(pTest);
        if (pTest->testFunc((void *)pTest) == FSI_APP_STATUS_SUCCESS)
        {
            FSI_log("\r\n %s have passed\r\n", pTest->testDesc);
        }
        else
        {
            FSI_log("\r\n %s have failed\r\n", pTest->testDesc);
            testFail = TRUE;
            break;
        }
    }

    if(testFail == TRUE)
    {
        FSI_log("\n Some tests have failed. \n");
#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_FAIL();
#endif
    }
    else
    {
        FSI_log("\n All tests have passed. \n");
#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_PASS();
#endif
    }

    FSI_log("Done\n");

    return;
}

void test_csl_fsi_test_app_runner(void)
{
    /* @description:Test runner for FSI App tests

       @requirements: PRSDK-5446

       @cores: mcu1_0 */
#if defined(UNITY_INCLUDE_CONFIG_H)
    UNITY_BEGIN();
    RUN_TEST (test_csl_fsi_test_app);
    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
#else
    test_csl_fsi_test_app();
#endif
    return;
}

/*
 *  ======== main ========
 */
int main(void)
{
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    test_csl_fsi_test_app_runner();

    return (0);
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


