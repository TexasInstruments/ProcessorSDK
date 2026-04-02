/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
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

/**
 *  \file   mmc_profiling_test.h
 *
 *  \brief  MMC Profiling API/interface file
 *
 */

#ifndef MMC_PROFILING_TEST_H_
#define MMC_PROFILING_TEST_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <ti/csl/cslr_gtc.h>
#include "MMCSD_log.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define MMC_APP_PROFILE_MMCSD_WRITE               (0U)
#define MMC_APP_PROFILE_MMCSD_READ                (1U)
#define MMC_APP_PROFILE_MAX_POINTS                (2U)

#define MMC_APP_PROFILE_MAX_NUM_BENCHMARKS        (5U) /* 0.25 MB, 0.5 MB, 1 MB , 2MB, 5 MB*/

/* Get GTC Timer Ticks */
#define mmcProfile_getGTCTimerTicks()             (*((uint64_t *)(CSL_GTC0_GTC_CFG1_BASE + CSL_GTC_CFG1_CNTCV_LO)))
/* GTC Timer Definitions */
#define MMC_APP_PROFILE_GTC_MOD_ID                (TISCI_DEV_GTC0)
#define MMC_APP_PROFILE_GTC_CLK_ID                (TISCI_DEV_GTC0_GTC_CLK)
#define MMC_APP_PROFILE_GTC_CLK_FREQ              (200000000U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct {
   char name[64];
   uint64_t startTick;
   uint64_t endTick;
   uint64_t totalTicks;
   uint32_t numIterations;
   uint32_t totalBytesTransferred;
} MmcApp_ProfilePoint;

typedef struct {
   uint32_t ticksPerMilliSec;
   MmcApp_ProfilePoint profile_Points[MMC_APP_PROFILE_MAX_POINTS];
} MmcApp_ProfileContext;
extern MmcApp_ProfileContext mmcApp_profileContext;

typedef struct {
 	float bandwidthMbytesPerSec;
	uint64_t numBytesTransferred;
} mmcsdTestBenchmarkMeasurements_t;

typedef struct {
	bool valid;
	mmcsdTestBenchmarkMeasurements_t read;
	mmcsdTestBenchmarkMeasurements_t write;
} mmcsdTestBenchmarkElem_t;

typedef struct {
	mmcsdTestBenchmarkElem_t RAW_measurements[MMC_APP_PROFILE_MAX_NUM_BENCHMARKS];
	mmcsdTestBenchmarkElem_t FATFS_measurements[MMC_APP_PROFILE_MAX_NUM_BENCHMARKS];
} mmcsdTestBenchmarks_t;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void mmcApp_profileStartPoint(uint32_t profileId);
void mmcApp_profileEndPoint(uint32_t profileId);
void mmcApp_profileResetResults();
void mmcApp_profileInit();
void mmcApp_displayBenchmarks(mmcsdTestBenchmarks_t *pBenchmarks, uint32_t testId);
void mmcApp_resetBenchmarks(mmcsdTestBenchmarkElem_t *pBenchmarkElem);
void mmcApp_storeBenchmarks(mmcsdTestBenchmarkElem_t *pBenchmarkElem,uint32_t bytesTransferred);

#ifdef __cplusplus
}
#endif

#endif /* MMC_PROFILING_TEST_H_ */
