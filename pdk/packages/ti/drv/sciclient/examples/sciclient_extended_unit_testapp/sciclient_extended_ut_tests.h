/*
 *  Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
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
  *
  *  \file  sciclient_extended_ut_tests.h
  *
  *  \brief This file defines the test cases for Sciclient Extended Unit Test 
  *         application
  *
  */

 #ifndef SCICLIENT_EXTENDED_UT_TESTS_H_
 #define SCICLIENT_EXTENDED_UT_TESTS_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define SCICLIENT_NUM_TESTCASES   (sizeof (gSciApp_TestcaseParams) / \
                                   sizeof (SciApp_TestParams_t))

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/** \brief  Defines the various Sciclient extended unit test cases. */
SciApp_TestParams_t gSciApp_TestcaseParams[] =
{
   {
      /** enableTest **/
      SCI_APP_TEST_ENABLE,

      /** testCaseId **/
      1U,

      /** *reqId **/
      "PDK-14042",

      /** *testCaseName **/
      "Testcase for functions in sciclient.c file",

      /** *userInfo **/
      "None",

      /** *disableReason **/
      "None",

      /** *passFailCriteria **/
      "sciclient.c related functions should Pass when called \
       with valid parameters and fail when called with invalid \
       parameters",

      /** cpuID **/
      SCI_APP_CORE_TYPE_R5F,

      /** printEnable **/
      SCI_APP_PRINT_ENABLE,

      /** testType **/
      (SCI_APP_TEST_TYPE_SANITY)
    },
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        2U,

        /** *reqId **/
        "PDK-14045",

        /** *testCaseName **/
        "Sciclient Rm Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "sciclient Rm functions should Pass when called \
         with valid parameters and fail when called with invalid \
         parameters",

        /** cpuID **/
        SCI_APP_CORE_TYPE_R5F,

        /** printEnable **/
        SCI_APP_PRINT_ENABLE,

        /** testType **/
        (SCI_APP_TEST_TYPE_SANITY)
    },
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        3U,

        /** *reqId **/
        "PDK-14047",

        /** *testCaseName **/
        "Sciclient Firewall Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "sciclient Firewall functions should Pass when called \
         with valid parameters and fail when called with invalid \
         parameters",

        /** cpuID **/
        SCI_APP_CORE_TYPE_R5F,

        /** printEnable **/
        SCI_APP_PRINT_ENABLE,

        /** testType **/
        (SCI_APP_TEST_TYPE_SANITY)
    },
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        4U,

        /** *reqId **/
        "PDK-14049",

        /** *testCaseName **/
        "Sciclient GenericMsgs Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "sciclient GenericMsgs functions should Pass when called \
         with valid parameters and fail when called with invalid \
         parameters",

        /** cpuID **/
        SCI_APP_CORE_TYPE_R5F,

        /** printEnable **/
        SCI_APP_PRINT_ENABLE,

        /** testType **/
        (SCI_APP_TEST_TYPE_SANITY)
    },
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        5U,

        /** *reqId **/
        "PDK-14053",

        /** *testCaseName **/
        "Sciclient RmIrq Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "sciclient RmIrq functions should Pass when called \
         with valid parameters and fail when called with invalid \
         parameters",

        /** cpuID **/
        SCI_APP_CORE_TYPE_R5F,

        /** printEnable **/
        SCI_APP_PRINT_ENABLE,

        /** testType **/
        (SCI_APP_TEST_TYPE_SANITY)
    },
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        6U,

        /** *reqId **/
        "PDK-14054",

        /** *testCaseName **/
        "Sciclient Procboot Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "sciclient Procboot functions should Pass when called \
         with valid parameters and fail when called with invalid \
         parameters",

        /** cpuID **/
        SCI_APP_CORE_TYPE_R5F,

        /** printEnable **/
        SCI_APP_PRINT_ENABLE,

        /** testType **/
        (SCI_APP_TEST_TYPE_SANITY)
    },
#if !defined(BUILD_MCU1_1)
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        7U,

        /** *reqId **/
        "PDK-14048",

        /** *testCaseName **/
        "Sciclient Pm Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "sciclient Pm functions should Pass when called \
         with valid parameters and fail when called with invalid \
         parameters",

        /** cpuID **/
        SCI_APP_CORE_TYPE_R5F,

        /** printEnable **/
        SCI_APP_PRINT_ENABLE,

        /** testType **/
        (SCI_APP_TEST_TYPE_NEGATIVE)
    },
#endif
#if defined (BUILD_MCU1_0)
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        8U,

        /** *reqId **/
        "PDK-14044",

        /** *testCaseName **/
        "SCICLIENT Boardcfg Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "sciclient Boardcfg functions should Pass when called \
         with valid parameters and fail when called with invalid \
         parameters",

        /** cpuID **/
        SCI_APP_CORE_TYPE_R5F,

        /** printEnable **/
        SCI_APP_PRINT_ENABLE,

        /** testType **/
        (SCI_APP_TEST_TYPE_SANITY)
    },
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        9U,

        /** *reqId **/
        "PDK-14046",

        /** *testCaseName **/
        "Sciclient Direct Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "sciclient Direct functions should Pass when called \
         with valid parameters and fail when called with invalid \
         parameters",

        /** cpuID **/
        SCI_APP_CORE_TYPE_R5F,

        /** printEnable **/
        SCI_APP_PRINT_ENABLE,

        /** testType **/
        (SCI_APP_TEST_TYPE_SANITY)
    },
#endif
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        10U,

        /** *reqId **/
        "PDK-14055",

        /** *testCaseName **/
        "Sciclient Secureproxy Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "sciclient Secureproxy functions should Pass when called \
         with valid parameters and fail when called with invalid \
         parameters",

        /** cpuID **/
        SCI_APP_CORE_TYPE_R5F,

        /** printEnable **/
        SCI_APP_PRINT_ENABLE,

        /** testType **/
        (SCI_APP_TEST_TYPE_SANITY)
    },
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        11U,

        /** *reqId **/
        "PDK-14060",

        /** *testCaseName **/
        "Sciclient Dkek Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "sciclient Dkek functions should Pass when called \
         with valid parameters and fail when called with invalid \
         parameters",

        /** cpuID **/
        SCI_APP_CORE_TYPE_R5F,

        /** printEnable **/
        SCI_APP_PRINT_ENABLE,

        /** testType **/
        (SCI_APP_TEST_TYPE_SANITY)
    },
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        12U,

        /** *reqId **/
        "PDK-16938",

        /** *testCaseName **/
        "Sciclient secureproxy Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "sciclient secureproxy functions should Pass when called \
         with valid parameters and fail when called with invalid \
         parameters",

        /** cpuID **/
        SCI_APP_CORE_TYPE_R5F,

        /** printEnable **/
        SCI_APP_PRINT_ENABLE,

        /** testType **/
        (SCI_APP_TEST_TYPE_SANITY)
    },
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        13U,

        /** *reqId **/
        "PDK-14058",

        /** *testCaseName **/
        "Sciclient Rom Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "sciclient Rom functions should Pass when called \
         with valid parameters and fail when called with invalid \
         parameters",

        /** cpuID **/
        SCI_APP_CORE_TYPE_R5F,

        /** printEnable **/
        SCI_APP_PRINT_ENABLE,

        /** testType **/
        (SCI_APP_TEST_TYPE_SANITY)
    }
};

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Function Declarations                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Internal Function Declarations                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Internal Function Definitions                     */
/* ========================================================================== */

/* None */

#endif /* SCICLIENT_EXTENDED_UT_TESTS_H_ */

