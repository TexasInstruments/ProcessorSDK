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

/* HW interrupt disable handle */
GateHwi_Handle gateHwi;

#ifdef _TMS320C6X
#pragma DATA_ALIGN(memPaInst, 128)
uint8_t memPaInst[TF_PA_INST_SIZE];

#pragma DATA_ALIGN(memL2Ram, 128)
uint8_t memL2Ram[TF_L2_TABLE_SIZE];

#pragma DATA_ALIGN(memL3Ram, 128);
uint8_t memL3Ram[TF_L3_TABLE_SIZE];
#else
uint8_t memPaInst[TF_PA_INST_SIZE] __attribute__ ((aligned (128)));
uint8_t memL2Ram[TF_L2_TABLE_SIZE] __attribute__ ((aligned (128)));
uint8_t memL3Ram[TF_L3_TABLE_SIZE] __attribute__ ((aligned (128)));
#endif


/* Memory used for the linking RAM and descriptor RAM */
//#pragma DATA_ALIGN(memLinkRam, 16)
//uint64_t memLinkRam[TF_NUM_DESC];

#ifdef _TMS320C6X
#pragma DATA_ALIGN(memDescRam, 128)
uint8_t memDescRam[TF_NUM_DESC * TF_SIZE_DESC];
#else
uint8_t memDescRam[TF_NUM_DESC * TF_SIZE_DESC] __attribute__ ((aligned (128)));
#endif

/* Memory used to store the test packets */
#ifdef _TMS320C6X
#pragma DATA_SECTION(PktRam, ".testPkts")
#pragma DATA_ALIGN(PktRam, 8)
uint8_t PktRam[TEST_PKT_RAM_SIZE];
#else
uint8_t PktRam[TEST_PKT_RAM_SIZE] __attribute__ ((aligned (8)));
#endif
uint8_t *testPktRam;
uint32_t testPktOffset;

/* Queue handles */
int32_t QPaTx[TF_PA_NUM_TX_CPDMA_CHANNELS];
int32_t QfreeDesc;		      	/* Unassociated descriptor queue handle */
int32_t QtDown;				  	/* Teardown queue handle */
int32_t QLinkedBuf1;			  	/* Queue of descriptors with linked buffers */
int32_t QLinkedBuf2;			  	/* Queue of descriptors with linked buffers */
int32_t QLinkedBuf3;			  	/* Queue of descriptors with linked buffers */
int32_t QLinkedBuf4;			  	/* Queue of descriptors with linked buffers */
int32_t QGen[TF_NUM_GEN_QUEUES];	/* General Use queues */


/* PA CPDMA instance and channel handles */
Cppi_Handle tfPaCppiHandle;
Cppi_ChHnd  tfPaTxChHnd[TF_PA_NUM_TX_CPDMA_CHANNELS];
Cppi_ChHnd  tfPaRxChHnd[TF_PA_NUM_RX_CPDMA_CHANNELS];


/* Packet buffers attached to descriptors */
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
#endif


/* Semaphore handles for PA tables */
Semaphore_Handle tfPaTableL2;
Semaphore_Handle tfPaTableL3;

/* Flow handles */
Cppi_FlowHnd tfPaFlowHnd;

/* SA Security Context Memory */
#ifdef _TMS320C6X
#pragma DATA_SECTION(ScBuf, ".scBufs")
#pragma DATA_ALIGN(ScBuf, 128)
uint8_t ScBuf[SALLDSIM_NUM_SC_BUF][SALLDSIM_SC_BUF_SIZE];
#else
uint8_t ScBuf[SALLDSIM_NUM_SC_BUF][SALLDSIM_SC_BUF_SIZE] __attribute__ ((aligned (128)));
#endif
uint8_t (*salldsimScBuf)[SALLDSIM_SC_BUF_SIZE];
void*   salldSimSegments[5];
uint16_t  salldSimSegUsedSizes[5];
uint16_t  salldSimSegAllocSizes[5];

