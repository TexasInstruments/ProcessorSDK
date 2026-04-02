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
 *  \file   mmc_profiling_test.c
 *
 *  \brief  Test application main file. The test application demonstrates
 *          raw data write & read on a MMCSD device.
 *
 */

#include <stdio.h>
#include <string.h>


#include <ti/osal/osal.h>
#include <ti/csl/cslr_device.h>
#include "MMCSD_log.h"
#include "mmc_profiling_test.h"
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/sciclient/sciclient.h>

MmcApp_ProfileContext mmcApp_profileContext;

static int32_t mmcApp_setGTCClk(uint32_t moduleId,
                      uint32_t clkId,
                      uint64_t clkRateHz)
{
    int32_t retVal;
    uint64_t currClkFreqHz;
#if defined(SOC_J784S4) || defined(SOC_J742S2)
    uint32_t origParent                 = 0U;
#endif

    retVal = Sciclient_pmGetModuleClkFreq(moduleId,
                                          clkId,
                                          &currClkFreqHz,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if ((CSL_PASS == retVal) &&
        (currClkFreqHz != clkRateHz))
    {
/* PDK-13708 : Unable to set GTC clock frequency to 200 MHz on J784S4 
   As a workaround by changing the parent clock, set the GTC clock frequency 
   to 200 MHz */
#if defined(SOC_J784S4) || defined(SOC_J742S2)
        retVal = Sciclient_pmGetModuleClkParent(moduleId, clkId, &origParent, SCICLIENT_SERVICE_WAIT_FOREVER);
        if (CSL_PASS != retVal)
        {
            MMCSD_log("Error : Failed to get parent number !! \n");
        }
        retVal = Sciclient_pmSetModuleClkParent(moduleId, clkId, origParent+1, SCICLIENT_SERVICE_WAIT_FOREVER);
        if (CSL_PASS != retVal)
        {
            MMCSD_log("Error : Failed to set parent number !! \n");
        }
#endif
        retVal = Sciclient_pmSetModuleClkFreq(moduleId,
                                         clkId,
                                         clkRateHz,
                                         TISCI_MSG_FLAG_AOP,
                                         SCICLIENT_SERVICE_WAIT_FOREVER);
        if (CSL_PASS != retVal)
        {
            MMCSD_log("Error : Failed to set GTC clock frequency !! \n");
        }
    }

    /* Enable GTC */
    HW_WR_REG32(CSL_GTC0_GTC_CFG1_BASE + 0x0U, 0x1);

    return (retVal);
}

void mmcApp_profileInit()	
{
    /* Configure GTC Timer for profiling */
    mmcApp_setGTCClk(MMC_APP_PROFILE_GTC_MOD_ID,
                           MMC_APP_PROFILE_GTC_CLK_ID,
                           MMC_APP_PROFILE_GTC_CLK_FREQ);
}

void mmcApp_profileStartPoint(uint32_t profileId)
{
	mmcApp_profileContext.profile_Points[profileId].startTick=mmcProfile_getGTCTimerTicks();
}

void mmcApp_profileEndPoint(uint32_t profileId)
{
	uint32_t ticks_diff;
	mmcApp_profileContext.profile_Points[profileId].endTick=mmcProfile_getGTCTimerTicks();
	if(mmcApp_profileContext.profile_Points[profileId].endTick > mmcApp_profileContext.profile_Points[profileId].startTick) {
	   ticks_diff=mmcApp_profileContext.profile_Points[profileId].endTick-mmcApp_profileContext.profile_Points[profileId].startTick;
	} else {
	   ticks_diff = (0xFFFFFFFF - mmcApp_profileContext.profile_Points[profileId].startTick) + mmcApp_profileContext.profile_Points[profileId].endTick;
	}
    mmcApp_profileContext.profile_Points[profileId].totalTicks = ticks_diff;
	mmcApp_profileContext.profile_Points[profileId].numIterations++;
}

void mmcApp_profileResetResults()
{
   int i;
   for(i=0;i<MMC_APP_PROFILE_MAX_POINTS;i++) {
     mmcApp_profileContext.profile_Points[i].startTick=0;
     mmcApp_profileContext.profile_Points[i].endTick=0;
     mmcApp_profileContext.profile_Points[i].totalTicks=0;
     mmcApp_profileContext.profile_Points[i].numIterations=0;
     mmcApp_profileContext.profile_Points[i].totalBytesTransferred=0;
  }   
}

void mmcApp_displayBenchmarks(mmcsdTestBenchmarks_t *pBenchmarks, uint32_t testId)
{
   int32_t i;
   char strReadBandWidth[20];
   char strWriteBandWidth[20];
  
   if(NULL == pBenchmarks)
     return;
     
   MMCSD_log ("\n ----- RAW Read/Write Throughput measurements -------\n");
   MMCSD_log ("Throughput is measured by reading/writing a contiguous block of memory of varying sizes using MMCSD_Write/Read() APIs \n");
   MMCSD_log ("Profiling numbers(RAW) for test ID : %d\n", testId);
   MMCSD_log ("----------------------------------------------\n" );

   MMCSD_log("| %12s | %12s | %12s |", "Size(KB)","Write(MB/s)","Read(MB/s)");
   MMCSD_log ("\n----------------------------------------------\n" );
   for(i=0;i<MMC_APP_PROFILE_MAX_NUM_BENCHMARKS;i++) 
   {
      if(pBenchmarks->RAW_measurements[i].valid)
      {
        /* Convert floating point numbers into strings since MMCSD_log (UART_printf) 
           doesn't support floating point printing floating point numbers */
        sprintf(strReadBandWidth, "%.3f", pBenchmarks->RAW_measurements[i].read.bandwidthMbytesPerSec);
        sprintf(strWriteBandWidth, "%.3f", pBenchmarks->RAW_measurements[i].write.bandwidthMbytesPerSec);

		MMCSD_log("| %12d | %12s | %12s | \n",
            (uint32_t)( pBenchmarks->RAW_measurements[i].read.numBytesTransferred/1024),
            strWriteBandWidth, strReadBandWidth);
 
      }
   }
   MMCSD_log ("----------------------------------------------\n" );

#ifdef MMCSD_TEST_FATFS_BENCHMARK_ENABLED    
   MMCSD_log ("\n\n ---------------- FATFS Read/Write Throughput Measurements ----------------\n");
   MMCSD_log ("Throughput is measured by reading/writing a file of various sizes using FAT32 \n");
   MMCSD_log ("Profiling numbers(FATFS) for test ID : %d\n", testId);
   MMCSD_log ("----------------------------------------------\n" );

   MMCSD_log("| %12s | %12s | %12s |", "Size(KB)","Write(MB/s)","Read(MB/s)");
   MMCSD_log ("\n----------------------------------------------\n" );
   for(i=0;i<MMC_APP_PROFILE_MAX_NUM_BENCHMARKS;i++) 
   {
      if(pBenchmarks->FATFS_measurements[i].valid) 
      {
          /* Convert floating point numbers into strings since MMCSD_log (UART_printf) 
             doesn't support floating point printing floating point numbers */
          sprintf(strReadBandWidth, "%.3f", pBenchmarks->FATFS_measurements[i].read.bandwidthMbytesPerSec);
          sprintf(strWriteBandWidth, "%.3f", pBenchmarks->FATFS_measurements[i].write.bandwidthMbytesPerSec);
 		  
          MMCSD_log("| %12d | %12s | %12s | \n",
             (uint32_t)( pBenchmarks->FATFS_measurements[i].read.numBytesTransferred/1024),
             strWriteBandWidth, strReadBandWidth);
      }
   }
   MMCSD_log ("----------------------------------------------\n" );
#endif      
}
void mmcApp_resetBenchmarks(mmcsdTestBenchmarkElem_t *pBenchmarkElem)
{
   if(NULL == pBenchmarkElem)
     return;

    pBenchmarkElem->valid = BFALSE; 
    pBenchmarkElem->write.numBytesTransferred = 0U;
    pBenchmarkElem->write.bandwidthMbytesPerSec = 0;
    pBenchmarkElem->read.numBytesTransferred = 0U;
    pBenchmarkElem->read.bandwidthMbytesPerSec = 0;
}

void mmcApp_storeBenchmarks(mmcsdTestBenchmarkElem_t *pBenchmarkElem,uint32_t bytesTransferred)
{   
    volatile float totalTimeInSec;
	/* Store the benchmark information */
    pBenchmarkElem->valid = BTRUE;

    totalTimeInSec = (float) mmcApp_profileContext.profile_Points[MMC_APP_PROFILE_MMCSD_WRITE].totalTicks/(MMC_APP_PROFILE_GTC_CLK_FREQ);
    pBenchmarkElem->write.bandwidthMbytesPerSec = (float)(((float)bytesTransferred)/(1024*1024))/
	                                                        (float)(totalTimeInSec);
    pBenchmarkElem->write.numBytesTransferred = bytesTransferred;

    totalTimeInSec = (float) mmcApp_profileContext.profile_Points[MMC_APP_PROFILE_MMCSD_READ].totalTicks/(MMC_APP_PROFILE_GTC_CLK_FREQ);
    pBenchmarkElem->read.bandwidthMbytesPerSec = (float)(((float)bytesTransferred)/(1024*1024))/
	                                                        (float)(totalTimeInSec); 
    pBenchmarkElem->read.numBytesTransferred = bytesTransferred;
}
