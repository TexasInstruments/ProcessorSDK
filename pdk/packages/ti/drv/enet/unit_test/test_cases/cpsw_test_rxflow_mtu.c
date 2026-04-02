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

/*!
 * \file     cpsw_test_rxflow_maxlength.c
 *
 * \brief    This file contains the cpsw_test_rxflow_maxlength test implementation.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <ti/drv/uart/UART_stdio.h>
#include <ti/csl/csl_cpswitch.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_ethutils.h>

#include <ti/osal/osal.h>

#include <ti/board/board.h>

#include "enet_test_entry.h"
#include "enet_test_base.h"
#include "cpsw_test_rxflow_mtu.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void EnetTestRxFlowMtu_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                         Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg     = (CpswMacPort_Cfg*)pLinkArgs->macCfg;

    EnetAppUtils_setNoPhyCfgRgmii(&pLinkArgs->mii, &pLinkArgs->phyCfg);
    macCfg->loopbackEn           = BTRUE;
    pLinkArgs->linkCfg.speed     = ENET_SPEED_1GBIT;
    pLinkArgs->linkCfg.duplexity = ENET_DUPLEX_FULL;
}

int32_t EnetTestRxFlowMtu_setTxPktInfo(EnetTestTaskObj *taskObj,
                                       uint32_t txChIndex,
                                       uint32_t pktNum,
                                       EnetDma_Pkt *pktInfo,
                                       bool *testComplete)
{
    EthFrame *frame;
    /* Broadcast address */
    uint8_t bcastAddr[ENET_MAC_ADDR_LEN] =
    {
        0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU
    };
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
    memcpy(frame->hdr.dstMac, bcastAddr, ENET_MAC_ADDR_LEN);
    memcpy(frame->hdr.srcMac, &stateObj->hostMacAddr[0U], ENET_MAC_ADDR_LEN);
    frame->hdr.srcMac[5] += pktNum;
    frame->hdr.etherType  = Enet_htons(ETHERTYPE_EXPERIMENTAL1);
    memset(&frame->payload[0U], (uint8_t)(0xA5 + pktNum), stateObj->txChCfgInfo[txChIndex]->maxTxPktLen);

    if (pktNum % 2U)
    {
        /* send bigger packet so it goes to flows drop ring  */
        pktInfo->sgList.list[0].segmentFilledLen = ENET_TEST_RXFLOW_MTU_PKT_LEN + 100U;
    }
    else
    {
        pktInfo->sgList.list[0].segmentFilledLen = ENET_TEST_RXFLOW_MTU_PKT_LEN - 100U;
    }

    pktInfo->appPriv = stateObj;
    *testComplete    = BFALSE;

    return ENET_SOK;
}
