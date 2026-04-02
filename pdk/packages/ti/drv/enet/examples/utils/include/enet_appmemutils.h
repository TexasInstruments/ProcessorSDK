/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 *  \file enet_appmemutils.h
 *
 *  \brief Enet DMA memutils header file.
 */

#ifndef ENET_MEM_H_
#define ENET_MEM_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/enet/include/core/enet_dma.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*! Ring memory allocation function  */
uint8_t *EnetMem_allocRingMem(void *appPriv,
                                         uint32_t numRingEle,
                                         uint32_t alignSize);

/*! Ring memory free function  */
void EnetMem_freeRingMem(const void *appPriv,
                                    void *ringMemPtr,
                                    uint32_t numRingEle);

/*! DMA packet allocation function  */
EnetUdma_DmaDesc *EnetMem_allocDmaDesc(void *appPriv,
                                                 uint32_t alignSize);

/*! DMA packet free function  */
void EnetMem_freeDmaDesc(void *appPriv,
                                    EnetUdma_DmaDesc *dmaDescPtr);

/*! Ethernet packet allocation function  */
EnetDma_Pkt *EnetMem_allocEthPkt(void *appPriv,
                                 uint32_t alignSize,
                                 uint32_t numScatterSegments,
                                 uint32_t scatterSegmentSize[]);

/*! Ethernet packet free function  */
void EnetMem_freeEthPkt(EnetDma_Pkt *pPktInfo);

/*! Initialize Cpsw memutils module
 * Note - This function should be called after Cpsw is opened as it uses CpswUtils_Q
 * functions */
int32_t EnetMem_init(void);

/*! Mem utils deinit  */
void EnetMem_deInit(void);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* ENET_MEM_H_ */
