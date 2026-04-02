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
 *  \file  sci_app_common.h
 *
 *  \brief This file contains common structures, macros and function declarations
 *         used by the SCI applications.
 * 
 */

#ifndef SCI_APP_COMMON_H_
#define SCI_APP_COMMON_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/csl/tistdtypes.h>
#include <ti/drv/uart/UART_stdio.h>

#if defined(SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_power.h>
#elif defined(SOC_J7200)
#include <ti/board/src/j7200_evm/include/board_power.h>
#elif defined(SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_power.h>
#elif defined(SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_power.h>
#elif defined(SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_power.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \anchor Sciclient_TestEnables
 *  \name Sciclient Test Enable and Disable indexes
 *  @{
 *  Macros to enable or disable tests and prints.
 */
#define SCI_APP_TEST_ENABLE                 (TRUE)
#define SCI_APP_TEST_DISABLE                (FALSE)
#define SCI_APP_PRINT_ENABLE                (TRUE)
#define SCI_APP_PRINT_DISABLE               (FALSE)
/* @} */

/**
 *  \anchor Sciclient_ParserIds
 *  \name Sciclient Test case Parser Ids
 *  @{
 *  Parser ID used to identify the different test types.
 */
#define SCI_APP_RUN_TEST_ID                 ('1')
#define SCI_APP_RUN_TESTS_SANITY            ('2')
#define SCI_APP_RUN_TESTS_ALL               ('A')
#define SCI_APP_PRINT_ALLTESTS              ('d')
#define SCI_APP_PRINT_TEST_ID               ('t')
#define SCI_APP_PRINT_RESULTS               ('g')
#define SCI_APP_PARSER_QUIT                 ('q')
/* @} */

#define SciApp_printf                       UART_printf
#define SciApp_getChar                      UART_getc

/* Change this to 0 for interactive UT */
#define SCI_APP_AUTORUN                     1

/* CPU Id of the test to be run on */
#define SCI_APP_CORE_TYPE_R5F                 0
/**< Select R5F for execution */

/* Test types */
#define SCI_APP_TEST_TYPE_SANITY            0x01
/**< Sanity testing */

#define SCI_APP_TEST_TYPE_NEGATIVE          0x02
/**< Negative testing */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 * \brief Typedef for test case parameters.
 */
typedef struct SciApp_TestParams SciApp_TestParams_t;

/**
 * \brief Typedef for test case type function pointer.
 */
typedef void (*SciApp_testCaseFxnPtr)(SciApp_TestParams_t *testPrms);

/**
 *  \brief Test case parameter structure.
 */
struct SciApp_TestParams
{
    Bool                       enableTest;
    /**< Whether test case should be executed or not. */
    uint32_t                   testcaseId;
    /**< Test case ID. */
    char                      *reqId;
    /**< Requirements covered by this test case. */
    char                      *testCaseName;
    /**< Test case name. */
    char                      *userInfo;
    /**< Test case user Info. */
    char                      *disableReason;
    /**< Reason string for disabling a test case. */
    char                      *passFailCriteria;
    /**< Test case pass/fail criteria. */
    uint32_t                  cpuID;
    /**< Test case CPU ID field */
    Bool                       printEnable;
    /**< Enable/disable print statements, used for stress testing. */
    uint32_t                   testType;
    /**< Refer #Test types */
    /*
     * Below variables are initialized in code and not in table!!
     */
    int32_t                    isRun;
    /**< Flag to indicate whether the test case is run or not. */
    int32_t                    testResult;
    /**< Test result. */
};

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   Parse the testcase data.
 *
 * \param   None.
 *
 * \return  None.
 */
void SciApp_parser(void);

/**
 * \brief   Fetch testcase data and run the testcase.
 *
 * \param   testParams        Structure to details of testcase to be run.
 *
 * \return  None.
 */
int32_t SciApp_testMain(SciApp_TestParams_t *testParams);

/**
 * \brief   Fetch testcase number to be run.
 *
 * \param   None.
 *
 * \return  Testcase number to be run.
 */
uint32_t SciApp_getNum(void);

/**
 * \brief   Initializes UART to print testcase details.
 *
 * \param   None.
 *
 * \return  None.
 */
void SciApp_consoleInit(void);

/**
 * \brief   Shutdown the pmic.
 * 
 * \param   None.
 * 
 * \return  Success or failure.
*/
int32_t SciApp_pmicShutdown(void);

/**
 * \brief   Fetches total number of test cases.
 *
 * \param   None.
 *
 * \return  Total number of test cases.
 */
uint32_t SciApp_getNumTests(void);

/* ========================================================================== */
/*                        Internal Function Declarations                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Function Definitions                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Internal Function Definitions                       */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* SCI_APP_COMMON_H_ */
