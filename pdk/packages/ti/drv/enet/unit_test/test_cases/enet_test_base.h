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
 *  \file enet_test_base.h
 *
 *  \brief cpsw_test_base test private header file.
 */

#ifndef ENET_TEST_BASE_H_
#define ENET_TEST_BASE_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/include/per/cpsw.h>
#include "enet_test.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ENET_TEST_COMMON_PKT_LEN               (1500U)

#define ENET_TEST_COMMON_NUM_LOOP               (10U)
#define ENET_TEST_COMMON_PKT_SENDCOUNT          (1000U)
#define ENET_TEST_COMMON_PKT_RECVCOUNT          (1000U)

typedef struct EnetTestMacPortList_s
{
    uint32_t numMacPorts;

    Enet_MacPort macPortList[ENET_MAC_PORT_NUM];
} EnetTestMacPortList_t;

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

int32_t EnetTestCommon_Init(EnetTestTaskObj *taskObj);

int32_t EnetTestCommon_Run(EnetTestTaskObj *taskObj);

int32_t EnetTestCommon_DeInit(EnetTestTaskObj *taskObj);

int32_t EnetTestCommon_processRxPkt(EnetTestTaskObj *taskObj,
                                    uint32_t rxFlowId,
                                    uint32_t pktNum,
                                    EnetDma_Pkt *pktInfo,
                                    bool *testComplete);

Enet_Handle EnetTestCommon_getCpswHandle(EnetTestTaskObj *taskObj);

uint32_t EnetTestCommon_retrieveFreeTxPkts(EnetTestTaskObj *taskObj,
                                           uint32_t txCfgIndex);

uint32_t EnetTestCommon_receivePkts(EnetTestTaskObj *taskObj,
                                    uint32_t rxCfgIndex);

extern Udma_DrvHandle gUdmaDrvHandle[ENET_TYPE_NUM];
void EnetTestCommon_disableHostAleEntrySecure(EnetTestTaskObj *taskObj);

int32_t EnetTestCommon_getAleMulticastEntry(EnetTestTaskObj *taskObj,
                                            uint8_t* macAddr,
                                            uint32_t vlanId,
                                            uint32_t numIgnBits,
                                            uint32_t *pPortMask);

int32_t EnetTestCommon_setAleMulticastEntry(EnetTestTaskObj *taskObj,
                                            uint8_t* macAddr,
                                            uint32_t vlanId,
                                            uint32_t numIgnBits,
                                            uint32_t portMask);

void  EnetTestCommon_createRxSem(EnetTestTaskObj *taskObj,
                                 uint32_t rxCfgIndex);

void EnetTestCommon_rxIsrFxn(void *appData);

void EnetTestCommon_discardRxPkts(EnetTestTaskObj *taskObj,
                                  uint32_t rxCfgIndex);

void  EnetTestCommon_getMacPortList(uint32_t macPortBitMask,
                                    EnetTestMacPortList_t *macPortList);

void    EnetTestCommon_waitForPortLink(EnetTestTaskObj *taskObj);

uint32_t EnetTestCommon_getMacPortAleMask(EnetTestTaskObj *taskObj);

int32_t EnetTestCommon_setTxPktInfo(EnetTestTaskObj *taskObj,
                                    uint32_t txChIndex,
                                    uint32_t pktNum,
                                    EnetDma_Pkt *pktInfo,
                                    bool *testComplete);

int32_t EnetTestCommon_pktTxRx(EnetTestTaskObj *taskObj);

void EnetTestCommon_getDefaultHostMacAddr(uint8_t *macAddr);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef ENET_TEST_BASE_H_ */
