/*
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
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

#include "../../unittest.h"
#include "fw_mem_allocator.h"

uint8_t *testPktRam;
uint32_t testPktOffset;

uint8_t (*salldsimScBuf)[SALLDSIM_SC_BUF_SIZE];
void*     salldSimSegments[5];
uint16_t  salldSimSegUsedSizes[5];
uint16_t  salldSimSegAllocSizes[5];


/* Initialize memory for the test framework */
int fw_test_mem_init(void)
{

    /* Allocate memory for the Packet buffers */
    testPktRam = (uint8_t *) fw_memAlloc(TEST_PKT_RAM_SIZE, CACHE_LINESZ);
    if(testPktRam == NULL) {
        printf ("fw_test_mem_init: mem alloc failed for testPktRam\n");
        return -1;
    }

    /* Allocate memory for the Security context buffers */
    salldsimScBuf = (uint8_t (*)[SALLDSIM_SC_BUF_SIZE]) 
    fw_memAlloc((SALLDSIM_NUM_SC_BUF * SALLDSIM_SC_BUF_SIZE), CACHE_LINESZ);
    if(salldsimScBuf == NULL) {
        printf ("fw_test_mem_init: mem alloc failed for salldsimScBuf\n");
        return -1;
    }

    return 0;
}

