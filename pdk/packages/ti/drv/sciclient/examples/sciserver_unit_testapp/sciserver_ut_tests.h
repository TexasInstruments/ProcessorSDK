/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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
 *  \file   sciserver_ut_tests.h
 *
 *  \brief  This file defines the test cases for Sciserver Unit Test application
 */

#ifndef SCISERVER_UT_TESTS_H_
#define SCISERVER_UT_TESTS_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define SCISERVER_UT_NUM_TESTCASES   (sizeof (gSciApp_TestcaseParams) / \
                                      sizeof (SciApp_TestParams_t))

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/** \brief Defines the various Sciserver test cases. */
SciApp_TestParams_t gSciApp_TestcaseParams[] =
{
    {
        /** enableTest **/
        SCI_APP_TEST_ENABLE,

        /** testCaseId **/
        1U,

        /** *reqId **/
        "PDK-14050",

        /** *testCaseName **/
        "Sciserver Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "Sciserver functions should fail when \
         called with invalid parameters and pass when \
         called with valid parameters ",

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
        "PDK-14051",

        /** *testCaseName **/
        "Sciserver secProxyTransfer Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "Sciserver_secProxyTransfer functions should fail when \
         called with invalid parameters and should pass when \
         called with valid parameters ",

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
        "PDK-14052",

        /** *testCaseName **/
        "Sciserver secproxyRoutingDescription Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "Sciserver_secproxyRoutingDescription functions should fail when \
         called with invalid parameters and should pass when called with \
         valid parameters. ",

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
        "PDK-14227",

        /** *testCaseName **/
        "Sciserver tirtos Testcase",

        /** *userInfo **/
        "None",

        /** *disableReason **/
        "None",

        /** *passFailCriteria **/
        "Sciserver tirtos functions should fail when \
         called with invalid parameters and should pass when \
         called with valid parameters. ",

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

#endif /* #ifndef SCISERVER_UT_TESTS_H_ */

