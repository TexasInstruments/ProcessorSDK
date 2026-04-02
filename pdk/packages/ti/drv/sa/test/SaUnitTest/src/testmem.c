/*
 *
 * Copyright (C) 2010-2013 Texas Instruments Incorporated - http://www.ti.com/
 *
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



/* Static memory allocation for test framework */

#include "unittest.h"

tFramework_t tFramework;

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
#ifdef _TMS320C6X
#pragma DATA_ALIGN(memPaInst, TF_CACHE_LINESZ)
uint8_t memPaInst[TF_ROUND_UP(TF_PA_INST_SIZE, TF_CACHE_LINESZ)];

#pragma DATA_ALIGN(memL2Ram, TF_CACHE_LINESZ)
uint8_t memL2Ram[TF_ROUND_UP(TF_L2_TABLE_SIZE, TF_CACHE_LINESZ)];

#pragma DATA_ALIGN(memL3Ram, TF_CACHE_LINESZ)
uint8_t memL3Ram[TF_ROUND_UP(TF_L3_TABLE_SIZE, TF_CACHE_LINESZ)];

#pragma DATA_ALIGN(memDescRam, TF_CACHE_LINESZ)
uint8_t memDescRam[TF_NUM_DESC * TF_SIZE_DESC];
#else
uint8_t memPaInst[TF_ROUND_UP(TF_PA_INST_SIZE, TF_CACHE_LINESZ)] __attribute__ ((aligned (TF_CACHE_LINESZ)));
uint8_t memL2Ram[TF_ROUND_UP(TF_L2_TABLE_SIZE, TF_CACHE_LINESZ)] __attribute__ ((aligned (TF_CACHE_LINESZ)));
uint8_t memL3Ram[TF_ROUND_UP(TF_L3_TABLE_SIZE, TF_CACHE_LINESZ)] __attribute__ ((aligned (TF_CACHE_LINESZ)));
uint8_t memDescRam[TF_NUM_DESC * TF_SIZE_DESC] __attribute__ ((aligned (TF_CACHE_LINESZ)));
#endif

#ifdef NETSS_INTERNAL_PKTDMA
uint8_t* passDescRam = (uint8_t*)(CSL_NETCP_CFG_REGS + 0x001c0000);
#endif
#else
#ifdef NSS_LITE2
FW_CPPI_DESC_T memDescRamTx[TF_NUM_DESC] __attribute__ ((aligned (TF_CACHE_LINESZ)));
uint8_t memDescRamRx[TF_NUM_DESC][TF_SIZE_DESC] __attribute__ ((aligned (TF_CACHE_LINESZ)));
uint8_t memBufRamTx[TF_RING_TRCNT][TF_DESC_BUFSIZE] __attribute__ ((aligned (TF_CACHE_LINESZ))) __attribute__((section(".saSrcBuffers")));
uint8_t memBufRamRx[TF_RING_TRCNT][TF_DESC_BUFSIZE] __attribute__ ((aligned (TF_CACHE_LINESZ))) __attribute__((section(".saDstBuffers")));
#else
#ifdef _TMS320C6X
#pragma DATA_ALIGN(memDescRam, TF_CACHE_LINESZ)
uint8_t memDescRam[TF_NUM_DESC * TF_SIZE_DESC];
#else
uint8_t memDescRam[TF_NUM_DESC * TF_SIZE_DESC] __attribute__ ((aligned (TF_CACHE_LINESZ)));
#endif /* _TMS320C6X */
#endif /* NSS_LITE2 */
#endif /* NSS_LITE */

/* Memory used to store the test packets */
#if  defined(SA_GEN_TEST_VECTOR)
#ifdef _TMS320C6X
#pragma DATA_SECTION(testPktRam, ".testPkts")
#pragma DATA_ALIGN(testPktRam, 8)
uint8_t testPktRam[TEST_PKT_RAM_SIZE];
#else
uint8_t testPktRam[TEST_PKT_RAM_SIZE] __attribute__ ((aligned (8)));
#endif
#endif

uint32_t testPktOffset;

/* Packet buffers attached to descriptors */
#ifndef NSS_LITE2
#ifdef _TMS320C6X
#pragma DATA_ALIGN(memQ1, 16)
#pragma DATA_ALIGN(memQ2, 16)
#pragma DATA_ALIGN(memQ3, 16)
#pragma DATA_ALIGN(memQ4, 16)
unsigned char memQ1[TF_LINKED_BUF_Q1_NBUFS][TF_LINKED_BUF_Q1_BUF_SIZE];
unsigned char memQ2[TF_LINKED_BUF_Q2_NBUFS][TF_LINKED_BUF_Q2_BUF_SIZE];
unsigned char memQ3[TF_LINKED_BUF_Q3_NBUFS][TF_LINKED_BUF_Q3_BUF_SIZE];
unsigned char memQ4[TF_LINKED_BUF_Q4_NBUFS][TF_LINKED_BUF_Q4_BUF_SIZE];
#else
unsigned char memQ1[TF_LINKED_BUF_Q1_NBUFS][TF_LINKED_BUF_Q1_BUF_SIZE] __attribute__ ((aligned (16)));
unsigned char memQ2[TF_LINKED_BUF_Q2_NBUFS][TF_LINKED_BUF_Q2_BUF_SIZE] __attribute__ ((aligned (16)));
unsigned char memQ3[TF_LINKED_BUF_Q3_NBUFS][TF_LINKED_BUF_Q3_BUF_SIZE] __attribute__ ((aligned (16)));
unsigned char memQ4[TF_LINKED_BUF_Q4_NBUFS][TF_LINKED_BUF_Q4_BUF_SIZE] __attribute__ ((aligned (16)));
#endif /* _TMS320C6X */

#ifdef _TMS320C6X
#pragma DATA_SECTION (memLocQ1, ".osrBufs")
#pragma DATA_SECTION (memLocQ2, ".osrBufs")
#pragma DATA_SECTION (memLocQ3, ".osrBufs")
#pragma DATA_SECTION (memLocQ4, ".osrBufs")
#pragma DATA_ALIGN(memLocQ1, 16)
#pragma DATA_ALIGN(memLocQ2, 16)
#pragma DATA_ALIGN(memLocQ3, 16)
#pragma DATA_ALIGN(memLocQ4, 16)
unsigned char memLocQ1[TF_LINKED_BUF_Q1_NBUFS][TF_LINKED_BUF_Q1_BUF_SIZE];
unsigned char memLocQ2[TF_LINKED_BUF_Q2_NBUFS][TF_LINKED_BUF_Q2_BUF_SIZE];
unsigned char memLocQ3[TF_LINKED_BUF_Q3_NBUFS][TF_LINKED_BUF_Q3_BUF_SIZE];
unsigned char memLocQ4[TF_LINKED_BUF_Q4_NBUFS][TF_LINKED_BUF_Q4_BUF_SIZE];
#else
unsigned char memLocQ1[TF_LINKED_BUF_Q1_NBUFS][TF_LINKED_BUF_Q1_BUF_SIZE] __attribute__ ((aligned (16)));
unsigned char memLocQ2[TF_LINKED_BUF_Q2_NBUFS][TF_LINKED_BUF_Q2_BUF_SIZE] __attribute__ ((aligned (16)));
unsigned char memLocQ3[TF_LINKED_BUF_Q3_NBUFS][TF_LINKED_BUF_Q3_BUF_SIZE] __attribute__ ((aligned (16)));
unsigned char memLocQ4[TF_LINKED_BUF_Q4_NBUFS][TF_LINKED_BUF_Q4_BUF_SIZE] __attribute__ ((aligned (16)));
#endif /* _TMS320C6X */

#ifdef _TMS320C6X
#pragma DATA_ALIGN(memHostQ1, 16)
#pragma DATA_ALIGN(memHostQ2, 16)
#pragma DATA_ALIGN(memHostQ3, 16)
#pragma DATA_ALIGN(memHostQ4, 16)
unsigned char memHostQ1[TF_HOST_LINKED_BUF_Q1_NBUFS][TF_HOST_LINKED_BUF_Q1_BUF_SIZE];
unsigned char memHostQ2[TF_HOST_LINKED_BUF_Q2_NBUFS][TF_HOST_LINKED_BUF_Q2_BUF_SIZE];
unsigned char memHostQ3[TF_HOST_LINKED_BUF_Q3_NBUFS][TF_HOST_LINKED_BUF_Q3_BUF_SIZE];
unsigned char memHostQ4[TF_HOST_LINKED_BUF_Q4_NBUFS][TF_HOST_LINKED_BUF_Q4_BUF_SIZE];
#else
unsigned char memHostQ1[TF_HOST_LINKED_BUF_Q1_NBUFS][TF_HOST_LINKED_BUF_Q1_BUF_SIZE] __attribute__ ((aligned (16)));
unsigned char memHostQ2[TF_HOST_LINKED_BUF_Q2_NBUFS][TF_HOST_LINKED_BUF_Q2_BUF_SIZE] __attribute__ ((aligned (16)));
unsigned char memHostQ3[TF_HOST_LINKED_BUF_Q3_NBUFS][TF_HOST_LINKED_BUF_Q3_BUF_SIZE] __attribute__ ((aligned (16)));
unsigned char memHostQ4[TF_HOST_LINKED_BUF_Q4_NBUFS][TF_HOST_LINKED_BUF_Q4_BUF_SIZE] __attribute__ ((aligned (16)));
#endif /* _TMS320C6X */
#endif /* NSS_LITE2 */

/* SA Security Context Memory */
#ifdef _TMS320C6X
#pragma DATA_SECTION(salldsimScBuf, ".scBufs")
#pragma DATA_ALIGN(salldsimScBuf, 128)
uintptr_t * salldsimScBuf[SALLDSIM_NUM_SC_BUF][SALLDSIM_SC_BUF_SIZE];
#else
uintptr_t * salldsimScBuf[SALLDSIM_NUM_SC_BUF][SALLDSIM_SC_BUF_SIZE] __attribute__ ((aligned (TF_CACHE_LINESZ))) __attribute__((section(".scBufs")));
#endif

#ifdef NSS_LITE2
/* For Cred mode, the TRCNT is essentially doubled, so make room for it */
uint8_t  memTxRing[TF_RING_TRSIZE * TF_RING_TRCNT*2] __attribute__ ((aligned (UDMA_CACHELINE_ALIGNMENT)));
uint8_t  memRxFreeRing[TF_RING_TRSIZE * TF_RING_TRCNT*2] __attribute__ ((aligned (UDMA_CACHELINE_ALIGNMENT)));
uint8_t  memTxCompRing[TF_RING_TRSIZE * TF_RING_TRCNT*2] __attribute__ ((aligned (UDMA_CACHELINE_ALIGNMENT)));
uint8_t  memRxRing[TF_RING_TRSIZE * TF_RING_TRCNT*2] __attribute__ ((aligned (UDMA_CACHELINE_ALIGNMENT)));

#endif


void*   salldSimSegments[SALLD_UNIT_TEST_MAX_SEGMENTS];
uint16_t  salldSimSegUsedSizes[SALLD_UNIT_TEST_MAX_SEGMENTS];
uint16_t  salldSimSegAllocSizes[SALLD_UNIT_TEST_MAX_SEGMENTS];

