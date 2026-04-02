/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2019-2020
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

/*
 * main_r5f.c
 */
#include <stdio.h>
#include <stdint.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/hw_types.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/osal/osal.h>
#include <ti/board/board.h>

#include "ecc_ddr.h"
#include "ecc_msmc_mem_parity.h"
#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/* ----------------- Constant definitions ----------------- */

/* The following statements define Menu options */
#define MAIN_DDR_ECC_TEST            ('1')
#define BOARD_INIT_WAIT_DELAY        1000
/* ------------------------------------------------------- */

/* ----------------- Function prototypes ----------------- */

static void mainMenu(char *option);


/* --------------------------------------------------- */

/* ----------------- Global variables ----------------- */

volatile uint32_t *gTest_Addr    = NULL;
volatile uint32_t *gTest_Addr1   = NULL;

int32_t gDdrEccTestResult = CSL_EFAIL;
int32_t gMsmcMemParityResult = CSL_EFAIL;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* Unity functions */
void test_csl_ecc_test_app_runner(void);
void test_csl_ecc_test_app (void);
void test_csl_ecc_msmc_mem_parity_app(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
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

/* --------------------------------------------------- */
void test_csl_ecc_msmc_mem_parity_app(void)
{
    /* Need not do Board Init as it is done in previous test */
    gMsmcMemParityResult = msmcEccMemParityTest();
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, gMsmcMemParityResult);
#endif
    return;
}
void test_csl_ecc_test_app(void)
{
    char              mainMenuOption;

    /* Do main command line processing */
    while (1)
    {
        mainMenu(&mainMenuOption);
        if (MAIN_DDR_ECC_TEST == mainMenuOption)
        {
            /* Handle MSMC ECC test menu */
            gDdrEccTestResult = DDREccTest();
#if defined (UNITY_INCLUDE_CONFIG_H)
            TEST_ASSERT_EQUAL_INT32(CSL_PASS, gDdrEccTestResult);
#else
            if (gDdrEccTestResult != CSL_PASS)
            {
                UART_printf("\r\n DDR ECC test failed \n");
            }
#endif
            /* exit the test */
           break;
        }
        else
        {
            UART_printf("\r\n  ECC test exiting...");
            break;
        }
    };

    return;
}

/* The following routine handles main menu options*/
static void mainMenu(char *option)
{
    while (1)
    {
        *option = MAIN_DDR_ECC_TEST;
        break;
    }
}

void test_csl_ecc_test_app_runner(void)
{
    /* @description:Test runner for ECC AGGR tests

       @requirements: PDK-2433 PDK-5879

       @cores: mcu1_0 */

#if defined(UNITY_INCLUDE_CONFIG_H)
    UNITY_BEGIN();
    /* DDR ECC test for R5F */
    RUN_TEST (test_csl_ecc_test_app);

    /* MSM Memory Parity Test */
#if defined (SOC_J721E)
#if defined (BUILD_MCU)
    RUN_TEST (test_csl_ecc_msmc_mem_parity_app);
#endif
#endif
    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
#else
    test_csl_ecc_test_app();
#endif
    return;
}

/* The main code here initializes the platform and processes
 * commands through UART port.
 * The menu commands are hierarchical and starts with the main menu with
 * the class of tests. The test classes include OCMC ecc test, EMIF ecc test
 * and DSP ECC test.
 * The OCMC err test & EMIF err test functions are in thier respective
 * separate c files and are used by all cores.
 * Once the class of tests are selected, then the submenu options for
 * the chosen class, are processed.
 */
int main (void)
{
    Board_initCfg     boardCfg;
    Board_STATUS  status = BOARD_SOK;

    boardCfg = BOARD_INIT_MODULE_CLOCK | BOARD_INIT_UART_STDIO;
    status = Board_init(boardCfg);
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(BOARD_SOK, status);
#else
    if (status != BOARD_SOK)
    {
        UART_printf("\n Board Init failed \n");
        return;
    }
#endif /* (UNITY_INCLUDE_CONFIG_H) */

#ifdef NO_SBL
    if (status == BOARD_SOK)
    {
        Osal_delay(BOARD_INIT_WAIT_DELAY);
        boardCfg = BOARD_INIT_PINMUX_CONFIG;
        status = Board_init(boardCfg);
#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_ASSERT_EQUAL_INT32(BOARD_SOK, status);
#else
        if (status != BOARD_SOK)
        {
            UART_printf("\n Board Init Pinmux Config failed \n");
            return;
        }
#endif /* (UNITY_INCLUDE_CONFIG_H) */
    }
    if (status == BOARD_SOK)
    {
        Osal_delay(BOARD_INIT_WAIT_DELAY);
        boardCfg = BOARD_INIT_DDR |
                   BOARD_INIT_DDR_ECC;
        status = Board_init(boardCfg);
#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_ASSERT_EQUAL_INT32(BOARD_SOK, status);
#else
        if (status != BOARD_SOK)
        { 
            UART_printf("\n Board Init DDR with ECC failed \n");
            return;
        }
#endif /* (UNITY_INCLUDE_CONFIG_H) */
    }
#endif /* NO_SBL */ 

    UART_printf("\n Board init complete \n");

    test_csl_ecc_test_app_runner();
    if (gDdrEccTestResult == CSL_PASS)
    {
#if defined (SOC_J721E)
        if(gMsmcMemParityResult == CSL_PASS)
#endif
        {
           UART_printf("\r\nAll tests have passed.");
        }
    }
    while (1);
}

