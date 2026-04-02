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
 *  \file cpsw_test_directed_pkt.h
 *
 *  \brief cpsw_test_directed_pkt test header file.
 */

#ifndef ENET_TEST_DIRECTED_PKT_H_
#define ENET_TEST_DIRECTED_PKT_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ENET_TEST_DIRECTED_PKT_SENDCOUNT            (100U)
#define ENET_TEST_DIRECTED_PKT_NUM_LOOP             (1U)
#define ENET_TEST_DIRECTED_PKT_RECVCOUNT            (100U)

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t EnetTestDirectedPkt_Run(EnetTestTaskObj *taskObj);

void EnetTestDirectedPkt_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                           Enet_MacPort portNum);

int32_t EnetTestDirectedPkt_setTxPktInfo(EnetTestTaskObj *taskObj,
                                         uint32_t txChIndex,
                                         uint32_t pktNum,
                                         EnetDma_Pkt *pktInfo,
                                         bool *testComplete);

int32_t EnetTestDirectedPkt_processRxPkt(EnetTestTaskObj *taskObj,
                                         uint32_t rxFlowId,
                                         uint32_t pktNum,
                                         EnetDma_Pkt *pktInfo,
                                         bool *testComplete);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ENET_TEST_DIRECTED_PKT_H_ */
