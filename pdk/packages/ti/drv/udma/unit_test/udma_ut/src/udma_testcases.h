/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2024
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
 *  \file udma_testcases.h
 *
 *  \brief This file defines the test cases for UDMA UT.
 */

#ifndef UDMA_TEST_CASES_H_
#define UDMA_TEST_CASES_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <udma_test.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define UDMA_TEST_NUM_TESTCASES         ((sizeof (gUdmaTestCases)) / \
                                         (sizeof (UdmaTestParams)))

#ifndef KB
#define KB                              (1024U)
#endif

#ifndef MB
#define MB                              (KB * KB)
#endif

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/** \brief Defines the various UDMA test cases. */
static UdmaTestParams gUdmaTestCases[] =
{
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3467U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "Blockcpy DDR to DDR in polling mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3473U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "Blockcpy DDR to DDR in interrupt mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3474U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "Blockcpy DDR to DDR in polling mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_MCU_BC | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3475U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "Blockcpy DDR to DDR",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_MCU_BC | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /* #if (UDMA_SOC_CFG_UDMAP_PRESENT == 1) */
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3476U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "Blockcpy DDR to DDR SW global 0 trigger test in polling mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_TRIGGER_GLOBAL0},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3477U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "Blockcpy DDR to DDR SW global 0 trigger test in interrupt mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_TRIGGER_GLOBAL0_INTR},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3478U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "Blockcpy DDR to DDR SW global 0 trigger test in polling mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_TRIGGER_GLOBAL0},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_MCU_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3479U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "Blockcpy DDR to DDR SW global 0 trigger test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_TRIGGER_GLOBAL0_INTR},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_MCU_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /* #if (UDMA_SOC_CFG_UDMAP_PRESENT == 1) */
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3480U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "Blockcpy circular 1KB DDR to DDR 1KB ICNT1 TR event type test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_EVENTSIZE_ICNT1},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {1*KB, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {1*KB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3481U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "Blockcpy circular 1KB DDR to DDR 1MB ICNT2 TR event type test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_EVENTSIZE_ICNT2},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {1*KB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3482U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "Blockcpy circular 1KB DDR to DDR 1MB ICNT3 TR event type test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_EVENTSIZE_ICNT3},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {1*KB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3516U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "Blockcpy MSMC to MSMC in interrupt mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3483U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "Blockcpy MSMC to MSMC in interrupt mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_MCU_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /* #if (UDMA_SOC_CFG_UDMAP_PRESENT == 1) */
#if (UDMA_TEST_SOC_OCMC_MEM_PRESENT == 1) 
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3484U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "Blockcpy OCMC to OCMC in interrupt mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_INTERNAL},
        .heapIdDest = {UTILS_MEM_HEAP_ID_INTERNAL},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_MCU_BC_INTERNAL_MEM),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /*#if (UDMA_TEST_SOC_OCMC_MEM_PRESENT == 1)  */
#ifndef UDMA_TEST_SOC_PRESILICON
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3485U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "Blockcpy DDR 1MB to DDR 1MB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U},
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*MB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3486U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "2D Blockcpy MSMC circular 1KB to DDR 1MB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*KB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3487U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "2D Blockcpy DDR 1MB to MSMC circular 1KB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*MB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 12918U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "2D Blockcpy OCMC circular 1KB to DDR 1MB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_INTERNAL},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*KB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 12919U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "2D Blockcpy DDR 1MB to OCMC circular 1KB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_INTERNAL},
        .srcBufSize = {1*MB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3488U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "2D Blockcpy MSMC circular 1KB to MSMC circular 1KB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*KB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#if (UDMA_TEST_SOC_OCMC_MEM_PRESENT == 1) 
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3489U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "2D Blockcpy OCMC circular 1KB to OCMC circular 1KB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_INTERNAL},
        .heapIdDest = {UTILS_MEM_HEAP_ID_INTERNAL},
        .srcBufSize = {1*KB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC_INTERNAL_MEM),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /* #if (UDMA_TEST_SOC_OCMC_MEM_PRESENT == 1)  */
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3490U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "Blockcpy DDR 1MB to DDR 1MB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U},
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*MB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3491U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "2D Blockcpy MSMC circular 1KB to DDR 1MB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*KB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3492U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "2D Blockcpy DDR 1MB to MSMC circular 1KB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*MB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3493U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "2D Blockcpy MSMC circular 1KB to MSMC circular 1KB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*KB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /* #if (UDMA_SOC_CFG_UDMAP_PRESENT == 1) */
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3494U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "Blockcpy DDR 1MB to DDR 1MB from multiple tasks",
        .disableInfo= NULL,
        .printEnable= PRINT_DISABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_LOOP_CNT_MT_SOC,
        .numTasks   = UDMA_TEST_MAX_MAIN_BC_CH,
        .testType   = {UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE, PACING_NONE, PACING_NONE, PACING_NONE},
        .numCh      = {1U, 1U, 1U, 1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*MB, 1*MB, 1*MB, 1*MB},
        .destBufSize= {1*MB, 1*MB, 1*MB, 1*MB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC_MT),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3495U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "Blockcpy DDR 1MB to DDR 1MB from multiple tasks",
        .disableInfo= NULL,
        .printEnable= PRINT_DISABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = UDMA_TEST_MAX_MCU_BC_CH,
        .testType   = {UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE, PACING_NONE, PACING_NONE, PACING_NONE},
        .numCh      = {1U, 1U, 1U, 1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC, UDMA_TEST_INST_ID_MCU_BC, UDMA_TEST_INST_ID_MCU_BC, UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*MB, 1*MB, 1*MB, 1*MB},
        .destBufSize= {1*MB, 1*MB, 1*MB, 1*MB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC_MT),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /* #if (UDMA_SOC_CFG_UDMAP_PRESENT == 1) */
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3496U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "Blockcpy MSMC to MSMC 1KBx1K (1MB) circular from multiple tasks",
        .disableInfo= NULL,
        .printEnable= PRINT_DISABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_LOOP_CNT_MT_SOC,
        .numTasks   = UDMA_TEST_MAX_MAIN_BC_CH,
        .testType   = {UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE, PACING_NONE, PACING_NONE, PACING_NONE},
        .numCh      = {1U, 1U, 1U, 1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                      },
        .ddim       = {
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*KB, 1*KB, 1*KB, 1*KB},
        .destBufSize= {1*KB, 1*KB, 1*KB, 1*KB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC_MT),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3497U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "Blockcpy MSMC to MSMC circular 1KBx1K (1MB) from multiple tasks",
        .disableInfo= NULL,
        .printEnable= PRINT_DISABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = UDMA_TEST_MAX_MCU_BC_CH,
        .testType   = {UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE, PACING_NONE, PACING_NONE, PACING_NONE},
        .numCh      = {1U, 1U, 1U, 1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC, UDMA_TEST_INST_ID_MCU_BC, UDMA_TEST_INST_ID_MCU_BC, UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                      },
        .ddim       = {
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*KB, 1*KB, 1*KB, 1*KB},
        .destBufSize= {1*KB, 1*KB, 1*KB, 1*KB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC_MT),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /* #if (UDMA_SOC_CFG_UDMAP_PRESENT == 1) */
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3498U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "2D Blockcpy DDR 4MB to MSMC circular 4KB at 20ms pacing for 10 seconds",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 500U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {DEF_PACING},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 4U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 4U, 1*KB, 1U}
                      },
        .dim        = {
                        {1*KB, 4*KB, 4*MB}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {4U*MB},
        .destBufSize= {4U*KB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC_PACING),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /* #ifndef UDMA_TEST_SOC_PRESILICON */
#ifdef UDMA_UTC_ID_MSMC_DRU0
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3499U,
        .tcName     = "DRU Indirect Blockcpy DDR to DDR in polling mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DRU_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3500U,
        .tcName     = "DRU Indirect Blockcpy DDR to DDR in interrupt mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DRU_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#ifndef UDMA_TEST_SOC_PRESILICON    
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3501U,
        .tcName     = "DRU Indirect Blockcpy DDR 1MB to DDR 1MB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DRU_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U},
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*MB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_DRU),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3502U,
        .tcName     = "DRU Indirect 2D Blockcpy MSMC circular 1KB to DDR 1MB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DRU_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*KB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_DRU),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3503U,
        .tcName     = "DRU Indirect 2D Blockcpy DDR 1MB to MSMC circular 1KB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DRU_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*MB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_DRU),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3504U,
        .tcName     = "DRU Indirect 2D Blockcpy MSMC circular 1KB to MSMC circular 1KB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DRU_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*KB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_DRU),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3505U,
        .tcName     = "DRU Indirect Blockcpy DDR 1MB to DDR 1MB from multiple tasks",
        .disableInfo= NULL,
        .printEnable= PRINT_DISABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = UDMA_TEST_MAX_DRU_CH,
        .testType   = {UDMA_TT_DRU_INDIRECT, UDMA_TT_DRU_INDIRECT, UDMA_TT_DRU_INDIRECT, UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE, PACING_NONE, PACING_NONE, PACING_NONE},
        .numCh      = {1U, 1U, 1U, 1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DRU_INTR_DEF, UDMA_TEST_CH_PRMID_DRU_INTR_DEF, UDMA_TEST_CH_PRMID_DRU_INTR_DEF, UDMA_TEST_CH_PRMID_DRU_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*MB, 1*MB, 1*MB, 1*MB},
        .destBufSize= {1*MB, 1*MB, 1*MB, 1*MB},
        .runFlag    = (UDMA_TEST_RF_DRU_MT),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3506U,
        .tcName     = "DRU Indirect Blockcpy MSMC to MSMC 1KBx1K (1MB) circular from multiple tasks",
        .disableInfo= NULL,
        .printEnable= PRINT_DISABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = UDMA_TEST_MAX_DRU_CH,
        .testType   = {UDMA_TT_DRU_INDIRECT, UDMA_TT_DRU_INDIRECT, UDMA_TT_DRU_INDIRECT, UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE, PACING_NONE, PACING_NONE, PACING_NONE},
        .numCh      = {1U, 1U, 1U, 1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DRU_INTR_DEF, UDMA_TEST_CH_PRMID_DRU_INTR_DEF, UDMA_TEST_CH_PRMID_DRU_INTR_DEF, UDMA_TEST_CH_PRMID_DRU_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                      },
        .ddim       = {
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*KB, 1*KB, 1*KB, 1*KB},
        .destBufSize= {1*KB, 1*KB, 1*KB, 1*KB},
        .runFlag    = (UDMA_TEST_RF_DRU_MT),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /* #ifndef UDMA_TEST_SOC_PRESILICON */
//Enable after adding testcase in Qmetry
#if 0
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 9000U,
        .tcName     = "DRU Blockcpy circular 1KB DDR to DDR 1KB ICNT1 TR event type test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DRU_EVENTSIZE_ICNT1},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {1*KB, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {1*KB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_DRU),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 9001U,
        .tcName     = "DRU Blockcpy circular 1KB DDR to DDR 1KB ICNT2 TR event type test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DRU_EVENTSIZE_ICNT2},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {1*KB, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {1*KB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_DRU),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 9002U,
        .tcName     = "DRU Blockcpy circular 1KB DDR to DDR 1KB ICNT3 TR event type test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DRU_EVENTSIZE_ICNT3},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {1*KB, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {1*KB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_DRU),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 9003U,
        .tcName     = "DRU DDR to DDR SW global 0 trigger test in polling mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_INST_ID_MAIN_0},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DRU_TRIGGER_GLOBAL0},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 9004U,
        .tcName     = "DRU DDR to DDR SW global 0 trigger test in interrupt mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_DRU_INDIRECT},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_INST_ID_MAIN_0},
        .chPrmId    = {UDMA_TEST_CH_PRMID_TRIGGER_GLOBAL0_INTR},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /* #if 0 */
#endif  /* #if defined (UDMA_UTC_ID_MSMC_DRU0) */
#if (UDMA_SOC_CFG_PROXY_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3507U,
        .tcName     = "Proxy queue and dequeue testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 5U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingProxyTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif /* #if (UDMA_SOC_CFG_PROXY_PRESENT == 1) */
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3508U,
        .tcName     = "Ring flush API testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingFlushTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14530U,
        .tcName     = "Ring flush All API testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingFlushAllTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)    
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3509U,
        .tcName     = "Ring event testcase in interrupt mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 10U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingEventTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_INTR,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3510U,
        .tcName     = "Ring event testcase in event polled mode",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 10U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingEventTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
#endif /* #if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1) */
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3511U,
        .tcName     = "Ring parameter check test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingParamCheckTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3512U,
        .tcName     = "Ring Utils Mem Size test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingUtilsMemSizeTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3682U,
        .tcName     = "Ring Mem Pointer test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingMemPtrTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3726U,
        .tcName     = "Ring attach and detach testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 5U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingAttachTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4644U,
        .tcName     = "Ring reset workaround test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingResetTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_AM65XX | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4693U,
        .tcName     = "Ring prime test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingPrimeTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif /* #if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1) */
#if (UDMA_SOC_CFG_RA_LCDMA_PRESENT == 1) && (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        /* For LCDMA with Dual ring, 
         * Ring Prime Read checks for Reverse occupancy.
         * In this case, actual transfer should happen to populate
         * reverse occupancy count and successfully do a ring prime read.
         * This testcase, implements block copy using ring prime API's 
         * This tests only Ring Prime API's.
         * Data check and TR Responce checks are NOT carried out.*/
        .enableTest = TEST_ENABLE,
        .tcId       = 8837U,
        .tcName     = "LCDMA Ring Prime Test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_BCDMA_BC},
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingPrimeLcdmaTc},
        .qdepth     = {500U},
        .pacingTime = {PACING_NONE},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .icnt       = {
                        {16U, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {16U, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA_BC | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif /* #if (UDMA_SOC_CFG_RA_LCDMA_PRESENT == 1) && (UDMA_SOC_CFG_BCDMA_PRESENT == 1) */
#if (UDMA_SOC_CFG_RING_MON_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4636U,
        .tcName     = "Ring Monitor Push and Pop Mode test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingMonPushPopTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14986U,
        .tcName     = "Ring Monitor Watermark Mode test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingMonWatermarkTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14987U,
        .tcName     = "Ring Monitor Starvation Mode test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingMonStarvationTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4637U,
        .tcName     = "Ring Monitor Low Threshold Mode test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingMonLowThresholdTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4638U,
        .tcName     = "Ring Monitor High Threshold Mode test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingMonHighThresholdTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif /* #if (UDMA_SOC_CFG_RING_MON_PRESENT == 1) */
#if (UDMA_SOC_CFG_PROXY_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4238U,
        .tcName     = "Proxy queue and dequeue testcase with allocated proxy",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestProxyPerformanceTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_MCU_PROXY | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif /* #if (UDMA_SOC_CFG_PROXY_PRESENT == 1) */
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 7034U,
        .tcName     = "Mapped Flow attach and detach testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 5U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestFlowAttachMappedTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_FLOW},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_FLOW | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#else /* #if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0) */
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3729U,
        .tcName     = "Flow attach and detach testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 5U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestFlowAttachTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_FLOW | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        /* This tests the failure for allocating flows more than actual count.
         * So this is not applicable in case of mapped flows, which allocates only one at a time.
         * Hence the testcase is not appicable for mapped flows. */
        .enableTest = TEST_ENABLE,
        .tcId       = 4154U,
        .tcName     = "Flow alloc and free testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestFlowAllocTc},
        /* All other below parameters not used in this testcase except ring params */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_FLOW | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif /* #if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0) */
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3513U,
        .tcName     = "PSIL and PDMA macro verification testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestPsilMacroTc},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3514U,
        .tcName     = "OSAL params testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestOsalPrmsTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3515U,
        .tcName     = "TR make utility testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestTrMakeTc},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3733U,
        .tcName     = "UDMA structure size print testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestStructSizeTc},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 9595U,
        .tcName     = "Event Disable and Enable Sanity Check Testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestEventDisableEnableSanity},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3707U,
        .tcName     = "Out of range flow ID Event testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 100U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestEventOutOfRangeFlow},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3905,
        .tcName     = "PDK-3863 bug testcase: Udma_chDequeueTdResponse NULL check",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBugTcPDK_3863},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /* #if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1) */
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 3965,
        .tcName     = "Channel pause and resume testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyPauseResumeTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC_PAUSE | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
#ifndef UDMA_TEST_SOC_PRESILICON  
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4100U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "HC Blockcpy DDR 1MB to DDR 1MB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U},
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*MB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC_HC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4101U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "2D HC Blockcpy MSMC circular 1KB to DDR 1MB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*KB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC_HC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4102U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "2D HC Blockcpy DDR 1MB to MSMC circular 1KB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*MB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC_HC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4103U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "2D HC Blockcpy MSMC circular 1KB to MSMC circular 1KB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*KB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC_HC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4104U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "HC Blockcpy DDR 1MB to DDR 1MB from multiple tasks",
        .disableInfo= NULL,
        .printEnable= PRINT_DISABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = UDMA_TEST_MAX_MAIN_BC_HC_CH,
        .testType   = {UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE, PACING_NONE, PACING_NONE, PACING_NONE},
        .numCh      = {1U, 1U, 1U, 1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF, UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF, UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF, UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*MB, 1*MB, 1*MB, 1*MB},
        .destBufSize= {1*MB, 1*MB, 1*MB, 1*MB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC_HC_MT),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4105U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "HC Blockcpy MSMC to MSMC 1KBx1K (1MB) circular from multiple tasks",
        .disableInfo= NULL,
        .printEnable= PRINT_DISABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = UDMA_TEST_MAX_MAIN_BC_HC_CH,
        .testType   = {UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc, &udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE, PACING_NONE, PACING_NONE, PACING_NONE},
        .numCh      = {1U, 1U, 1U, 1U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF, UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF, UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF, UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                      },
        .ddim       = {
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*KB, 1*KB, 1*KB, 1*KB},
        .destBufSize= {1*KB, 1*KB, 1*KB, 1*KB},
        .runFlag    = (UDMA_TEST_RF_MAIN_BC_HC_MT),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4106U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "HC Blockcpy DDR 1MB to DDR 1MB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U},
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*MB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC_HC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4107U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "2D HC Blockcpy MSMC circular 1KB to DDR 1MB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*KB},
        .destBufSize= {1*MB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC_HC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4108U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "2D HC Blockcpy DDR 1MB to MSMC circular 1KB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*MB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC_HC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4109U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "2D HC Blockcpy MSMC circular 1KB to MSMC circular 1KB performance test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF},
        .qdepth     = {UDMA_TEST_DEF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*KB},
        .destBufSize= {1*KB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC_HC),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4110U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "HC Blockcpy DDR 1MB to DDR 1MB from multiple tasks",
        .disableInfo= NULL,
        .printEnable= PRINT_DISABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = UDMA_TEST_MAX_MCU_BC_HC_CH,
        .testType   = {UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc, &udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE, PACING_NONE},
        .numCh      = {1U, 1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC, UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF, UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                      },
        .ddim       = {
                        {1*KB, 1*KB, 0U},
                        {1*KB, 1*KB, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR},
        .heapIdDest = {UTILS_MEM_HEAP_ID_DDR, UTILS_MEM_HEAP_ID_DDR},
        .srcBufSize = {1*MB, 1*MB},
        .destBufSize= {1*MB, 1*MB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC_HC_MT),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4111U,
        .tcName     = UDMA_TEST_MCU_BC_TCNAME_PREFIX "HC Blockcpy MSMC to MSMC 1KBx1K (1MB) circular from multiple tasks",
        .disableInfo= NULL,
        .printEnable= PRINT_DISABLE,
        .prfEnable  = PRF_ENABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_FUNCTIONAL | UDMA_TCT_PERFORMANCE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = UDMA_TEST_PERF_LOOP_CNT,
        .numTasks   = UDMA_TEST_MAX_MCU_BC_HC_CH,
        .testType   = {UDMA_TT_BLK_CPY, UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyTc, &udmaTestBlkcpyTc},
        .pacingTime = {PACING_NONE, PACING_NONE},
        .numCh      = {1U, 1U},
        .instId     = {UDMA_TEST_INST_ID_MCU_BC, UDMA_TEST_INST_ID_MCU_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF, UDMA_TEST_CH_PRMID_BLKCPY_HC_INTR_DEF},
        .qdepth     = {UDMA_TEST_PERF_QDEPTH, UDMA_TEST_PERF_QDEPTH},
        .icnt       = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dicnt      = {
                        {1*KB, 1U, 1*KB, 1U},
                        {1*KB, 1U, 1*KB, 1U},
                      },
        .dim        = {
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                      },
        .ddim       = {
                        {0U, 0U, 0U},
                        {0U, 0U, 0U},
                      },
        .heapIdSrc  = {UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC},
        .heapIdDest = {UTILS_MEM_HEAP_ID_MSMC, UTILS_MEM_HEAP_ID_MSMC},
        .srcBufSize = {1*KB, 1*KB},
        .destBufSize= {1*KB, 1*KB},
        .runFlag    = (UDMA_TEST_RF_MCU_BC_HC_MT),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#endif /* #ifndef UDMA_TEST_SOC_PRESILICON   */
#endif /* #if (UDMA_SOC_CFG_UDMAP_PRESENT == 1) */
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4656U,
        .tcName     = "PDK-4654 bug testcase: UDMA deinit resource check",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_FULL | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBugTcPDK_4654},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 4841U,
        .tcName     = UDMA_TEST_MAIN_BC_TCNAME_PREFIX "Blockcpy DDR to DDR in interrupt mode chaining test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_BLK_CPY},
        .testFxnPtr = {&udmaTestBlkcpyChainingTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {2U},
        .instId     = {UDMA_TEST_INST_ID_MAIN_BC, UDMA_TEST_INST_ID_MAIN_BC},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF, UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH, USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U},
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U},
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U},
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U},
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID, DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID, DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0, UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0, UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_CHAIN | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
#if (UDMA_SOC_CFG_RA_LCDMA_PRESENT == 1) && (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 12498U,
        .tcName     = "BCDMA Channel Paramter Check test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestBcdmaParamCheckTc},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 12499U,
        .tcName     = "BCDMA Channel API's test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestBcdmaChApiTc},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
#endif
#if (UDMA_SOC_CFG_RA_LCDMA_PRESENT == 1) && (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 6282U,
        .tcName     = "PKTDMA Channel Paramter Check test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestChPktdmaParamCheckTc},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 6279U,
        .tcName     = "PKTDMA Channel API's test",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestChPktdmaChApiTc},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
#endif /* #if (UDMA_SOC_CFG_RA_LCDMA_PRESENT == 1) */
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13372U,
        .tcName     = "Test UDMA Init function for invalid input parameters",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaInitNegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13373U,
        .tcName     = "RmLock params test for UDMA Init function",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaInitRmLockNegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13374U,
        .tcName     = "PrintLock params test for UDMA Init function",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaInitPrintLockNegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13375U,
        .tcName     = "Test UDMA Init function for invalid RmSetProxy",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaInitRmSetProxyNegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13378U,
        .tcName     = "Test UDMA Params Init function",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaPrmsInitNegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13377U,
        .tcName     = "Test UDMA Deinit function",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaDeinitNegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13376U,
        .tcName     = "Event Registration test for UDMA Init function",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaInitEventRegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13370U,
        .tcName     = "Negative Test for UDMA Utility Functions",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaPrinfNegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13515U,
        .tcName     = "Negative Test for UDMA Utility Functions",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaGetTrSizeNegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13516U,
        .tcName     = "Negative Test for UDMA Utility Functions",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaPhyToVirtNegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13518U,
        .tcName     = "Negative Test for UDMA Utility Functions",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaVirtToPhyNegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13517U,
        .tcName     = "Negative Test for UDMA Utility Functions",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaPrinftMutexNegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13384U,
        .tcName     = "Test UDMA Init and Deinit function when Global Event Reg value is True",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestUdmaInitSkipEventRegTc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13405U,
        .tcName     = "UDMA flow get num function negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestFlowGetNumNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_FLOW | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13398U,
        .tcName     = "UDMA ring get write index normal function negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingGetWrIdxNormal},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13397U,
        .tcName     = "UDMA ring get Read index function negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingGetRdIdxNormal},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13401U,
        .tcName     = "UDMA ring get ring occupancy function negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingGetRingOccNormal},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13402U,
        .tcName     = "UDMA ring get mode function negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingGetModeNormal},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13404U,
        .tcName     = "UDMA ring get element count function negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingGetElementCntNormal},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13406U,
        .tcName     = "UDMA ring get Mem ptr function negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingGetMemPtrNormal},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13424U,
        .tcName     = "Test UDMA Test ring attach negative test case",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingAttachNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                         {0U, 0U, 0U}
                      },
        .ddim       = {
                         {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13426U,
        .tcName     = "Test UDMA ring get number negative testcases",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingGetNumNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13427U,
        .tcName     = "Test ring Dequeue raw negative testcases",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingDequeueRawNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13428U,
        .tcName     = "Test ring queue raw negative testcases",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingQueueRawNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13425U,
        .tcName     = "Test UDMA ring Detach neagtive test cases",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = UDMA_TEST_DEF_LOOP_CNT,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingDetachNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_INTR_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_INVALID,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13393U,
        .tcName     = "UDMA ring free function negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingFreeNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13386U,
        .tcName     = "UDMA Flow free function negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestFlowFreeNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_FLOW | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13392U,
        .tcName     = "UDMA Flow get count function negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestFlowGetCountNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_FLOW | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13549U,
        .tcName     = "UDMA flow params init negative function testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestFlowPrmsInitNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_FLOW | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13547U,
        .tcName     = "UDMA ring params init negative function testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingPrmsInitNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13548U,
        .tcName     = "UDMA ring Mon params init negative function testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingMonPrmsInitNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13554U,
        .tcName     = "UDMA ring alloc negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestRingAllocNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13553U,
        .tcName     = "UDMA ring flush raw negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingFlushRawNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13681U,
        .tcName     = "UDMA proxy alloc negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTest_proxyAllocNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13683U,
        .tcName     = "UDMA proxy config negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTest_proxyConfigNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13682U,
        .tcName     = "UDMA proxy free negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTest_proxyFreeNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13583U,
        .tcName     = "UDMA ChPause negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChPauseNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13584U,
        .tcName     = "UDMA ChOpen negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChOpenNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13585U,
        .tcName     = "UDMA ChClose negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChCloseNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13586U,
        .tcName     = "UDMA ChConfigTx negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChConfigTxNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13587U,
        .tcName     = "UdmaTestChGetDefaultFlowHandle negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChGetDefaultFlowHandleNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13588U,
        .tcName     = "UdmaTestChGetTdCqRingHandle negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChGetTdCqRingHandleNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13589U,
        .tcName     = "UdmaTestChGetFqRingNum negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChGetFqRingNumNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13691U,
        .tcName     = "UdmachGetCqRingNum negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChGetCqRingNumNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13693U,
        .tcName     = "Udma chGetTriggerEvent negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChGetTriggerEventNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13694U,
        .tcName     = "Udma chSetSwTrigger negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChSetSwTriggerNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13695U,
        .tcName     = "Udma chSetChaining negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChSetChainingNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13696U,
        .tcName     = "Udma chBreakChaining negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChBreakChainingNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13697U,
        .tcName     = "Udma chGetNumTest negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChGetNumNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13701U,
        .tcName     = "Udma chConfigUtcTest negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChConfigUtcTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13703U,
        .tcName     = "Udma chSetSwTrigger negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChSetSwTriggerChTypeNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13736U,
        .tcName     = "Udma udmaChPause positive testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChPause},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13737U,
        .tcName     = "Udma ChGetDefaultFlowHandle positive testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChGetDefaultFlowHandle},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #endif
    #if (UDMA_NUM_UTC_INSTANCE > 0)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13771U,
        .tcName     = "Udma Channel Pause Dru negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChPauseDruNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13772U,
        .tcName     = "Udma channel ConfigUtc Dru negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChConfigUtcDruTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13702U,
        .tcName     = "Udma chDisableTest Negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChDisableDruNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13773U,
        .tcName     = "Udma channel SetSwTrigger Dru negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChSetSwTriggerDruNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13686U,
        .tcName     = "UDMA dru get num queue negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestDruGetNumQueueNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU  | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13687U,
        .tcName     = "UDMA dru queue config negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestDruQueueConfigNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                       {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU  | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13688U,
        .tcName     = "UDMA dru get trigger addr negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestDruGetTriggerRegAddrNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13755U,
        .tcName     = "UDMA dru get trigger addr testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestDruGetTriggerRegAddr},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    #endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13776U,
        .tcName     = "UDMA rmFreeBlkCopyUhcCh testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmFreeBlkCopyUhcChTest},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_UHC | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13777U,
        .tcName     = "UDMA rmFree Tx and Rx HcCh testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmAllocFreeTest},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_MCU2_0 | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13778U,
        .tcName     = "UDMA rmFreeTxUhcCh testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmFreeAllocTxChUhcHcChTest},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_UHC | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13779U,
        .tcName     = "UDMA rmFreeRxUhcCh testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmFreeAllocRxChUhcHcChTest},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_UHC | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13780U,
        .tcName     = "UDMA RmInitPrms_init testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmInitPrmsinitTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13689U,
        .tcName     = "Udma channel GetStats negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChGetStatsTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13690U,
        .tcName     = "Udma channel disable negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChDisableTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13826U,
        .tcName     = "Udma channel disable testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChDisableTest},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13692U,
        .tcName     = "Udma channel Resume negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChResumeTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13699U,
        .tcName     = "Udma channel ConfigPdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChConfigPdmaTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13834U,
        .tcName     = "Udma channel ConfigPdma testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChConfigPdmaTest},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13700U,
        .tcName     = "Udma channel ConfigRx negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChConfigRxTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13698U,
        .tcName     = "Udma channel enable negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChEnableTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13738U,
        .tcName     = "Udma channel GetTdCqRingHandle testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChGetTdCqRingHandle},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13739U,
        .tcName     = "Udma channel GetFqRingNum testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChFqRing},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13572U,
        .tcName     = "UDMA Flow Alloc function testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestFlowAllocNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_FLOW | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13571U,
        .tcName     = "UDMA Flow Attach function testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestFlowAttachNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_FLOW | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13570U,
        .tcName     = "UDMA Flow Detach function testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestFlowDetachNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_FLOW | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13567U,
        .tcName     = "UDMA Flow Config function testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestFlowConfigNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_FLOW | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13851U,
        .tcName     = "Udma channel InitPrms negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaInitPrmsNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13852U,
        .tcName     = "Udma channel GetSwTriggerRegister negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChGetSwTriggerRegisterTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #if (UDMA_NUM_UTC_INSTANCE > 0)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13827U,
        .tcName     = "Udma Channel Pause Dru negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChPauseDru},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #endif
    #if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13853U,
        .tcName     = "Udma ChSetSwTriggerRegister positive testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChSetSwTriggerRegister},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_BCDMA_0},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13853U,
        .tcName     = "Udma ChSetSwTriggerRegister positive testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChSetSwTriggerRegister},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13740U,
        .tcName     = "Udma channel DequeueTdResponse negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChDequeueTdResponseTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13893U,
        .tcName     = "Udma channel GetStats testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChGetStatsTest},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13898U,
        .tcName     = "Udma channel Disable for Bcdma Inst negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChBcdmaInstTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #endif 
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13843U,
        .tcName     = "UDMA Udma_eventRegister testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestEventRegisterNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13844U,
        .tcName     = "UDMA Udma_eventUnRegister testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestEventUnRegisterNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13845U,
        .tcName     = "UDMA Udma_eventGetId testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestEventGetIdNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13846U,
        .tcName     = "UDMA Udma_eventDisable testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestEventDisableNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13847U,
        .tcName     = "UDMA Udma_eventEnable testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestEvenEnableNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13848U,
        .tcName     = "UDMA Udma_eventGetGlobalHandle testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestEventGetGlobalNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13850U,
        .tcName     = "UDMA Udma_eventGetRxFlowIdFwStatus testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestEventGetRxFlowStatusNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13914U,
        .tcName     = "UDMA Udma_eventRegister Bcdma instance testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestEventRegisterBcdmaTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13915U,
        .tcName     = "UDMA Udma_eventUnRegister Bcdma instance testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestEventUnRegisterBcdmaTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13963U,
        .tcName     = "UDMA RingGetModeLcdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingGetModeLcdmaNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13964U,
        .tcName     = "UDMA RingGetElementCntLcdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingGetElementCntLcdmaNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13965U,
        .tcName     = "UDMA RingGetForwardRingOccLcdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingGetForwardRingOccLcdmaNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13966U,
        .tcName     = "UDMA RingGetReverseRingOccLcdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingGetReverseRingOccLcdmaNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13967U,
        .tcName     = "UDMA RingGetWrIdxLcdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingGetWrIdxLcdmaNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13968U,
        .tcName     = "UDMA RingGetRdIdxLcdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingGetRdIdxLcdmaNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13969U,
        .tcName     = "UDMA RingGetMemPtrLcdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingGetMemPtrLcdmaNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14079U,
        .tcName     = "UDMA Ring Lcdma positive testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingLcdma},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14077U,
        .tcName     = "UDMA ring lcdma DoorBell positive testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingLcdmaDoorBell},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14104U,
        .tcName     = "UDMA Ring get forward ring occ lcdma positive testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingGetForwardRingOccLcdma},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14105U,
        .tcName     = "UDMA Ring queue and dequeue lcdma postive testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestQueueDequeueLcdma},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14190U,
        .tcName     = "UDMA RingDequeueRawLcdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingDequeueRawLcdmaNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14189U,
        .tcName     = "UDMA RingFlushRawLcdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingFlushRawLcdmaNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14191U,
        .tcName     = "UDMA RingPrimeReadLcdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingPrimeReadLcdmaNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14188U,
        .tcName     = "UDMA RingSetCfgLcdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingSetCfgLcdmaNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14117U,
        .tcName     = "UDMA Osal Set Cache Params negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestOsalSetCachePrmsNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14118U,
        .tcName     = "UDMA Osal negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestOsalNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14119U,
        .tcName     = "UDMA Flow alloc function",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestFlowAllocfreeFlowCntNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_MCU2_1 | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13995U,
        .tcName     = "UDMA rmAllocBlkCopy Hc and Uhc negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmAllocBlkCopyChTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13994U,
        .tcName     = "UDMA rmAllocTx Ch, Hch and Uhch negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmAllocTxChTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13993U,
        .tcName     = "UDMA rmAllocRx Ch, Hch and Uhch negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmAllocRxChTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13992U,
        .tcName     = "UDMA Rm Alloc negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmAllocTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13990U,
        .tcName     = "UDMA Rm Translate Core Intr Input testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmTranslateCoreIntrInputTest},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#if (!_TMS320C6X)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13991U,
        .tcName     = "UDMA Rm Translate Core Intr Input Negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmTranslateCoreIntrInputTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14156U,
        .tcName     = "UDMA Rm Set Shared ResRmInit Prms Negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmSetSharedResRmInitPrmsTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13795U,
        .tcName     = "UDMA ring Mon Free negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingMonFreeNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13796U,
        .tcName     = "UDMA ring Mon Alloc negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingMonAllocNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13794U,
        .tcName     = "UDMA ring Mon config negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingMonConfigNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13797U,
        .tcName     = "UDMA ring mon get data negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingMonGetDataNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13799U,
        .tcName     = "UDMA ring mon get num negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingMonGetNumNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13820U,
        .tcName     = "UDMA ring proxy queue negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingProxyQueueRawNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13281U,
        .tcName     = "UDMA ring proxy Dequeue negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingProxyDequeueRawNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13282U,
        .tcName     = "UDMA ring Mon Get Num positive testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingMonGetNum},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    #if (UDMA_NUM_UTC_INSTANCE > 0)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 13892U,
        .tcName     = "Udma Channel ChConfigUtc Dru Vhwa negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaDruVhwaTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC | UDMA_TEST_RF_CORE_MCU2_0 | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14252U,
        .tcName     = "UDMA ring Get RdIdx normal positive testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingGetRdIdxNormal},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14255U,
        .tcName     = "UDMA ring prime read normal testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingPrimeReadNormalNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14253U,
        .tcName     = "UDMA ring check params BCDMA instance testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingCheckParamsNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_INST_ID_BCDMA_0},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA| UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14253U,
        .tcName     = "UDMA ring check params Normal instance testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingCheckParamsNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14251U,
        .tcName     = "UDMA ring get RdIdx positive testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingGetRdIdx},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14527U,
        .tcName     = "UDMA ring Mon Alloc positive testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingMonAlloc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14281U,
        .tcName     = "UDMA Ring Flush raw function",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingFlushRawLcdmaCslNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14282U,
        .tcName     = "UDMA proxy queue raw normal negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingProxyQueueRawNormalNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14283U,
        .tcName     = "Udma ring proxy dequeue raw normal negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingProxyDequeueRawNormalNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14284U,
        .tcName     = "Udma ring FlushRawLcdma testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingFlushRawLcdma},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14261U,
        .tcName     = "UDMA event register negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaEventRegisterTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14266U,
        .tcName     = "UDMA event type Dma completion negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaEventTypeDmaCompletionTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    #if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14262U,
        .tcName     = "UDMA Event Bcdma Inst Negative Testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaEventBcdmaInstTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    #endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14263U,
        .tcName     = "UDMA event config negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaEventConfigTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14264U,
        .tcName     = "UDMA event register testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaEventRegisterTest},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    #if (UDMA_NUM_UTC_INSTANCE > 0)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14265U,
        .tcName     = "Udma event program steering negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaEventProgramSteeringTestNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #endif
    #if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14364U,
        .tcName     = "UDMA CSL LCDMA RingAcc Test",
        .testFxnPtr = {&UdmaTestCslRingLcdma},
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    #endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14374U,
        .tcName     = "CSL ring monitor testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCslRingMonitor},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14373U,
        .tcName     = "CSL ring testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCslRing},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14365U,
        .tcName     = "UDMA CSL LCDMA RingAcc Negative Test",
        .testFxnPtr = {&UdmaTestCSL_lcdma_ringaccPeekData},
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    #endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14366U,
        .tcName     = "UDMA CSL Ring Acc Testcast",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestCslRingAcc},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14350U,
        .tcName     = "Udma CSL proxy testcases",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRingCSLProxy},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14345U,
        .tcName     = "Udma CSL Udmap testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCslUdmap},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #if (UDMA_NUM_UTC_INSTANCE > 0)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14325U,
        .tcName     = "CSL DRU channel set global trigger1 testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCSLDruChSetGlobalTrigger1},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14344U,
        .tcName     = "Udma CSL Udmap negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCslUdmapNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #if (UDMA_NUM_UTC_INSTANCE > 0)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14332U,
        .tcName     = "CSL DRU channel set global tigger0 testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCSLDruChSetGlobalTrigger0},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14333U,
        .tcName     = "CSL DRU channel set local trigger0 testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCSLDruChSetLocalTrigger0},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14324U,
        .tcName     = "CSL DRU get capabilities testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCSLDruGetCapabilities},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14334U,
        .tcName     = "CSL DRU channel get queue status testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCSLDruChGetQueueStatus},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14335U,
        .tcName     = "CSL DRU channel resume testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCSLDruChResume},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14336U,
        .tcName     = "CSL DRU channel pause testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCSLDruChPause},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14337U,
        .tcName     = "CSL DRU channel teardown complete testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCSLDruChIsTeardownComplete},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14338U,
        .tcName     = "CSL DRU channel set event testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCSLDruChSetEvent},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14340U,
        .tcName     = "CSL DRU Queue config testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCSLDruQueueConfig},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14339U,
        .tcName     = "CSL DRU Channel config testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCSLDruChConfig},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    #endif
    #if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14367U,
        .tcName     = "Csl Bcdma testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCslBcdma},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14368U,
        .tcName     = "Csl Bcdma negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestCslBcdmaNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    #endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14369U,
        .tcName     = "Csl Intaggr testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestCslIntaggr},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#if defined (BUILD_C7X)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14370U,
        .tcName     = "Csl Clec testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&udmaTestCslClec},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_C7X_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
#endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14515U,
        .tcName     = "Udma Rm negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmTestNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    #if (UDMA_NUM_UTC_INSTANCE > 0)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14519U,
        .tcName     = "Udma Rm Alloc Ext negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestRmAllocExtChNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_DRU | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    #endif
    #if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14521U,
        .tcName     = "Udma channel get trigger event testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChGetTriggerEventTest},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_BCDMA | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    #endif
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 14770U,
        .tcName     = "UDMA ring Flush All negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestringFlushAllNeg},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 15069U,
        .tcName     = "UDMA rm Alloc testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_ENABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaRmAllocTest},
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_C7X_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_NONE,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 15100U,
        .tcName     = "Udma channel ConfigRx negative testcase",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaChConfigRx},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .chPrmId    = {UDMA_TEST_CH_PRMID_DEF},
        .qdepth     = {USE_DEF_QDEPTH},
        .icnt       = {
                        {UDMA_TEST_DEF_ICNT0, 1U, 1U, 1U}
                      },
        .dicnt      = {
                        {UDMA_TEST_DEF_DICNT0, 1U, 1U, 1U}
                      },
        .dim        = {
                        {0U, 0U, 0U}
                      },
        .ddim       = {
                        {0U, 0U, 0U}
                      },
        .heapIdSrc  = {DEF_HEAP_ID},
        .heapIdDest = {DEF_HEAP_ID},
        .srcBufSize = {UDMA_TEST_DEF_ICNT0},
        .destBufSize= {UDMA_TEST_DEF_DICNT0},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_C7X_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 17406U,
        .tcName     = "Udma Test Tx Channel Disable Polling",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_FUNCTIONAL),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChDisablePolling},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_MCU2_0 | UDMA_TEST_RF_CORE_C7X_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },
    {
        .enableTest = TEST_ENABLE,
        .tcId       = 17528U,
        .tcName     = "Udma Test Tx Channel Disable Polling Neg",
        .disableInfo= NULL,
        .printEnable= PRINT_ENABLE,
        .prfEnable  = PRF_DISABLE,
        .tcType     = (UDMA_TCT_SANITY | UDMA_TCT_NEGATIVE),
        .dcEnable   = DATA_CHECK_DISABLE,
        .loopCnt    = 1U,
        .numTasks   = 1U,
        .testType   = {UDMA_TT_MISC},
        .testFxnPtr = {&UdmaTestChDisablePollingNeg},
        /* All other below parameters not used in this testcase */
        .pacingTime = {PACING_NONE},
        .numCh      = {1U},
        .instId     = {UDMA_TEST_DEFAULT_UDMA_INST},
        .runFlag    = (UDMA_TEST_RF_SOC_ALL | UDMA_TEST_RF_CORE_MCU2_0 | UDMA_TEST_RF_CORE_C7X_ALL | UDMA_TEST_RF_CFG_DEF | UDMA_TEST_RF_CFG_DYN),
        .ringPrmId  = UDMA_TEST_RING_PRMID_EVENT_POLLED,
    },

};

#ifdef __cplusplus
}
#endif

#endif  /* UDMA_TEST_CASES_H_ */
