/*
 *   @file  mibspi_test_common.h
 *
 *   @brief
 *      Unit Test common code for the MibSpi Driver
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2020 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef MIBSPI_TEST_COMMON_H_
#define MIBSPI_TEST_COMMON_H_


#ifdef __cplusplus
extern "C" {
#endif


/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include <ti/drv/mibspi/MIBSPI.h>
#include <ti/drv/edma/edma.h>

#define MIBSPI_TEST_NUM_SPIINSTANCES                                   (2U)

/* System DMA handle, created in init Task */
extern EDMA_Handle          gDmaHandle[MIBSPI_TEST_NUM_SPIINSTANCES];
extern enum MibSpi_InstanceId gMibspiInst[MIBSPI_TEST_NUM_SPIINSTANCES];

/* Test case global variables */
extern bool gXWR1xxxLoopbackTest;
extern bool gXWR1xxxSlaveReady;

/**************************************************************************
 *************************** Common test functions ***************************
 **************************************************************************/
extern void Test_spiAPI_twoInstance(void);
extern void Test_spiAPI_oneInstance(uint8_t inst);
extern void Test_loopback_oneInstance(uint32_t inst, uint8_t slaveIndex);
extern void Test_loopback_oneInstance_callback(uint32_t inst, uint8_t slaveIndex);
extern void Test_loopbackSlave_oneInstance(uint32_t inst);

#ifdef __cplusplus
}
#endif

#endif  /* MIBSPI_TEST_COMMON_H_ */
