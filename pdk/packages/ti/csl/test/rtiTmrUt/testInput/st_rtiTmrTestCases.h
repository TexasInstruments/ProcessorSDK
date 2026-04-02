/*
 *  Copyright (c) 2020 Texas Instruments Incorporated
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
 *  \file stw_rtiTmrTestcases.h
 *
 *  \brief This file defines the test cases for RTI Timer UT.
 */

#ifndef STW_RTI_TMR_TEST_CASES_H_
#define STW_RTI_TMR_TEST_CASES_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <st_rtiTmr.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define RTI_TMR_NUM_TESTCASES               (sizeof (gRTITmrTestcaseParams) / sizeof (st_RTITmrTestcaseParams_t))
#define APP_RTI_TMR_CODE_COVERAGE_TEST (30)

#if defined(SOC_TPR12) || defined (SOC_AWR294X)
#define QT_BUILD
#endif
#if !defined(QT_BUILD)
#define RTI_TMR_TIMEOUT     (1000U)
#else
#define RTI_TMR_TIMEOUT     (10U)
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/** \brief Defines the various RTI test cases. */
st_RTITmrTestcaseParams_t gRTITmrTestcaseParams[] =
{
    {
        .enableTest         = TEST_ENABLE,
        .testcaseId         = 1U,
        .testCaseName       = "MCU RTITMR0 with block (1, 1)",
        .disableInfo        = "None",
        .rtiTmrConfigParams = {
                                CSL_MCU_RTITMR0_CFG_BASE,
                                RTI_TMR_CLOCK_SOURCE_200MHZ,
                                RTI_TMR_CNT_BLK_INDEX_1,
                                RTI_TMR_CMP_BLK_INDEX_1,
                                1,
                                RTI_TMR_TIMEOUT,
                                5
                            },
        .printEnable        = PRINT_ENABLE,
    },
    {
        .enableTest         = TEST_ENABLE,
        .testcaseId         = 2U,
        .testCaseName       = "MCU RTITMR1 with block (0, 2)",
        .disableInfo        = "None",
        .rtiTmrConfigParams = {
                                CSL_MCU_RTITMR1_CFG_BASE,
                                RTI_TMR_CLOCK_SOURCE_200MHZ,
                                RTI_TMR_CNT_BLK_INDEX_0,
                                RTI_TMR_CMP_BLK_INDEX_2,
                                3,
                                RTI_TMR_TIMEOUT,
                                5
                            },
        .printEnable        = PRINT_ENABLE,
    },
    {
        .enableTest         = TEST_ENABLE,
        .testcaseId         = 3U,
        .testCaseName       = "MCU RTITMR2 with block (1, 3)",
        .disableInfo        = "None",
        .rtiTmrConfigParams = {
                                CSL_MCU_RTITMR2_CFG_BASE,
                                RTI_TMR_CLOCK_SOURCE_200MHZ,
                                RTI_TMR_CNT_BLK_INDEX_1,
                                RTI_TMR_CMP_BLK_INDEX_3,
                                9,
                                RTI_TMR_TIMEOUT,
                                5
                            },
        .printEnable        = PRINT_ENABLE,
    },
    {
         .enableTest        = TEST_ENABLE,
         .testcaseId        = APP_RTI_TMR_CODE_COVERAGE_TEST, /* 30U */
         .testCaseName      = "MCU RTI-Timer Code Coverage",
         .disableInfo       = "None",
        .rtiTmrConfigParams = {
                                CSL_MCU_RTITMR0_CFG_BASE,
                                RTI_TMR_CLOCK_SOURCE_200MHZ,
                                RTI_TMR_CNT_BLK_INDEX_1,
                                RTI_TMR_CMP_BLK_INDEX_1,
                                1,
                                RTI_TMR_TIMEOUT,
                                5
                            },
         .printEnable       = PRINT_ENABLE,
    }
};

#endif /* #ifndef ST_RTI_TMR_TEST_CASES_H_ */
