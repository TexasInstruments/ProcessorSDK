/* Copyright (c) Texas Instruments Incorporated 2023
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
 *  \file     core_r5_pmu_test.c
 *
 *  \brief    This file contains PMU test code for R5 core.
 *
 *  \details  PMU register read/write and configuration tests
 **/

/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/
#include "core_r5_test.h"
#include <kernel/dpl/PmuP_armv7.h>
/*===========================================================================*/
/*                         Declarations                                      */
/*===========================================================================*/


/*===========================================================================*/
/*                         Macros                                            */
/*===========================================================================*/
#define PMU_CNTR_NUM_BRANCH             (2u)
#define PMU_CNTR_NUM_ICACHE_MISS        (1u)
#define PMU_CNTR_NUM_DCAHCE_MISS        (0u)

/*===========================================================================*/
/*                         Internal function declarations                    */
/*===========================================================================*/
static uint32_t core_r5_pmu_test_get_dCache_miss(void);
static uint32_t core_r5_pmu_test_get_iCache_miss(void);
static uint32_t core_r5_pmu_test_get_branch(void);
static uint32_t core_r5_pmu_test_get_cycle(void);

/*===========================================================================*/
/*                         Global Variables                                  */
/*===========================================================================*/

/*===========================================================================*/
/*                   Local Function definitions                              */
/*===========================================================================*/


/*===========================================================================*/
/*                        Test Function definitions                          */
/*===========================================================================*/


static void core_r5_pmu_test_init_pmu(void)
{
    uint32_t val;

    PmuP_armR5Cfg(0, 0, 1);
    /* Clear the overflow */
    val = PmuP_getOverflowStatus();
    val &= 0x80000007;
    PmuP_armR5ClearCntrOverflowStatus(val);
    PmuP_armR5CfgCntr(PmuP_ARM_R5_PMU_CYCLE_COUNTER_NUM, PmuP_ARM_R5_PMU_EVENT_TYPE_CYCLE_CNT);
    /* I-Cache */
    PmuP_armR5CfgCntr(PMU_CNTR_NUM_ICACHE_MISS, PmuP_ARM_R5_PMU_EVENT_TYPE_ICACHE_MISS);
    /* D-Cache */
    PmuP_armR5CfgCntr(PMU_CNTR_NUM_DCAHCE_MISS, PmuP_ARM_R5_PMU_EVENT_TYPE_DCACHE_MISS);
    /* Branch */
    PmuP_armR5CfgCntr(PMU_CNTR_NUM_BRANCH, PmuP_ARM_R5_PMU_EVENT_TYPE_B_IMMEDIATE);
    PmuP_armR5EnableAllCntrs(0);
    PmuP_armR5ResetCycleCnt();      /* Set PMCR C-bit */
    PmuP_armR5ResetCntrs();

    PmuP_armR5EnableCntr(PMU_CNTR_NUM_BRANCH, 1);
    PmuP_armR5EnableCntr(PMU_CNTR_NUM_ICACHE_MISS, 1);
    PmuP_armR5EnableCntr(PMU_CNTR_NUM_DCAHCE_MISS, 1);
    PmuP_armR5EnableAllCntrs(1);
    PmuP_armR5EnableCntrOverflowIntr(PMU_CNTR_NUM_BRANCH, 1U);
    PmuP_armR5EnableCntrOverflowIntr(PMU_CNTR_NUM_BRANCH, 0U);
}

static uint32_t core_r5_pmu_test_get_iCache_miss(void)
{
    uint32_t    temp;
    temp = PmuP_armR5ReadCntr(PMU_CNTR_NUM_ICACHE_MISS);
    return temp;
}

static uint32_t core_r5_pmu_test_get_dCache_miss(void)
{
    uint32_t    temp;
    temp = PmuP_armR5ReadCntr(PMU_CNTR_NUM_ICACHE_MISS);
    return temp;
}

static uint32_t core_r5_pmu_test_get_branch(void)
{
    uint32_t    temp;
    temp = PmuP_armR5ReadCntr(PMU_CNTR_NUM_BRANCH);
    return temp;
}

unsigned int core_r5_pmu_test_get_cycle(void) {
    uint32_t    temp;
    temp = PmuP_armR5ReadCntr(PmuP_ARM_R5_PMU_CYCLE_COUNTER_NUM);
    return temp;
}

/* This test demonstrates the APIs to enable the use of
 *             PMU counters
 */
int32_t core_r5_pmuTest(void)
{
    uint32_t   numCntrs, dCacheMissCnt, iCacheMissCnt, branchCnt, cycleCnt;
    /* Declarations of variables */
    int32_t    testResult = CORE_R5_APP_TEST_PASS;

    /* Some negative tests for code coverage on PMU */
    PmuP_armR5Cfg(1, 1, 0);
    PmuP_armR5EnableCntr(PMU_CNTR_NUM_BRANCH, 0);
    PmuP_armR5EnableCntr(PMU_CNTR_NUM_ICACHE_MISS, 0);
    PmuP_armR5EnableCntr(PMU_CNTR_NUM_DCAHCE_MISS, 0);
    PmuP_armR5EnableAllCntrs(0);

    core_r5_pmu_test_init_pmu();

   /* Report number of counters implemented */
    numCntrs = PmuP_armR5GetNumCntrs();
    DebugP_log("Number of PMU counters implemented is: ");
    DebugP_log("0x%x \n",numCntrs);
    DebugP_log(" \n\r ");

   /* Report number of branch, icache miss and dcache miss */
   dCacheMissCnt = core_r5_pmu_test_get_dCache_miss();
   iCacheMissCnt = core_r5_pmu_test_get_iCache_miss();
   branchCnt     = core_r5_pmu_test_get_branch();
   cycleCnt      = core_r5_pmu_test_get_cycle();

   DebugP_log("iCacheMissCnt is: ");
   DebugP_log("0x%x \n",iCacheMissCnt);
   DebugP_log(" \n\r ");

   DebugP_log("dCacheMissCnt is: ");
   DebugP_log("0x%x \n",dCacheMissCnt);
   DebugP_log(" \n\r ");

   DebugP_log("branchCnt is: ");
   DebugP_log("0x%x \n",branchCnt);
   DebugP_log(" \n\r ");

   DebugP_log("cycleCnt is: ");
   DebugP_log("0x%x \n",cycleCnt);
   DebugP_log(" \n\r ");


   DebugP_log("Setting all PMU counters to new values: ");
   DebugP_log(" \n\r ");

   /* Set and Read the Counter */
   PmuP_armR5SetCntr(PmuP_ARM_R5_PMU_CYCLE_COUNTER_NUM, 0x3000U );
   PmuP_armR5SetCntr(PMU_CNTR_NUM_ICACHE_MISS, 1U );
   PmuP_armR5SetCntr(PMU_CNTR_NUM_DCAHCE_MISS, 1U );
   PmuP_armR5SetCntr(PMU_CNTR_NUM_BRANCH, 1U );

   dCacheMissCnt = core_r5_pmu_test_get_dCache_miss();
   iCacheMissCnt = core_r5_pmu_test_get_iCache_miss();
   branchCnt     = core_r5_pmu_test_get_branch();
   cycleCnt      = core_r5_pmu_test_get_cycle();

   DebugP_log("New iCacheMissCnt is: ");
   DebugP_log("0x%x \n\r",iCacheMissCnt);
   DebugP_log(" \n ");

   DebugP_log("New dCacheMissCnt is: ");
   DebugP_log("0x%x \n\r",dCacheMissCnt);
   DebugP_log(" \n ");

   DebugP_log("New branchCnt is: ");
   DebugP_log("0x%x \n\r",branchCnt);
   DebugP_log(" \n ");

   DebugP_log("New cycleCnt is: ");
   DebugP_log("0x%x \n\r",cycleCnt);
   DebugP_log(" \n ");


   return (testResult);
}

