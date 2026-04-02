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
#include "testconn.h"

tFramework_t tFramework;

#pragma DATA_SECTION (memDescRam, ".sharedDDR")
#pragma DATA_ALIGN(memDescRam, 128)
uint8_t memDescRam[TF_NUM_DESC * TF_SIZE_DESC];

/* Huge memory block reserved for shared memory use */
#pragma DATA_ALIGN   (mem_blk_shm, CACHE_LINESZ)
#pragma DATA_SECTION (mem_blk_shm, ".sharedDDR")
far sa_Example_MemBlk_t mem_blk_shm;

#pragma DATA_ALIGN(shObj, 128)
#pragma DATA_SECTION (shared_Objs, ".sharedDDR")
far sa_Example_MemBlk_t* shared_ObjsBase;

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
#pragma DATA_ALIGN(memQ1, 16)
#pragma DATA_ALIGN(memQ2, 16)
#pragma DATA_ALIGN(memQ3, 16)
#pragma DATA_ALIGN(memQ4, 16)
#pragma DATA_SECTION (memQ1, ".sharedDDR")
#pragma DATA_SECTION (memQ2, ".sharedDDR")
#pragma DATA_SECTION (memQ3, ".sharedDDR")
#pragma DATA_SECTION (memQ4, ".sharedDDR")
unsigned char memQ1[TF_LINKED_BUF_Q1_NBUFS][TF_LINKED_BUF_Q1_BUF_SIZE];
unsigned char memQ2[TF_LINKED_BUF_Q2_NBUFS][TF_LINKED_BUF_Q2_BUF_SIZE];
unsigned char memQ3[TF_LINKED_BUF_Q3_NBUFS][TF_LINKED_BUF_Q3_BUF_SIZE];
unsigned char memQ4[TF_LINKED_BUF_Q4_NBUFS][TF_LINKED_BUF_Q4_BUF_SIZE];


/* Flow handles */
Cppi_FlowHnd tfPaFlowHnd;

/* SA Security Context Memory */
#pragma DATA_SECTION(ScBuf, ".scBufs")
#pragma DATA_ALIGN(ScBuf, 128)
uint8_t ScBuf[SALLDSIM_NUM_SC_BUF][SALLDSIM_SC_BUF_SIZE];
uint8_t (*salldsimScBuf)[SALLDSIM_SC_BUF_SIZE];
void*   salldSimSegments[5];
uint16_t  salldSimSegUsedSizes[5];
uint16_t  salldSimSegAllocSizes[5];

/* Indicates the core or logical task ID test is running on */
uint32_t                coreNum;

