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
 * \file     cpsw_test_directed_pkt.c
 *
 * \brief    This file contains the directed packet test implementation.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_ethutils.h>

#include "enet_test_entry.h"
#include "enet_test_base.h"
#include "cpsw_test_directed_pkt.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ENET_DIRECTED_PKT_TEST_ACTUAL_PORT               (ENET_MAC_PORT_3)
#define ENET_DIRECTED_PKT_TEST_DIRECTED_PORT             (ENET_MAC_PORT_4)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Destinaion Multicast MAC address */
uint8_t dstMultMacAddr[ENET_MAC_ADDR_LEN] =
{
    0x01U, 0xFEU, 0xEDU, 0xC0U, 0xFFU, 0xEEU
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void EnetTestDirectedPkt_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                           Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg      = (CpswMacPort_Cfg*)pLinkArgs->macCfg;

    if (portNum == ENET_DIRECTED_PKT_TEST_DIRECTED_PORT)
    {
       EnetAppUtils_setNoPhyCfgRgmii(&pLinkArgs->mii, &pLinkArgs->phyCfg);
       macCfg->loopbackEn            = BTRUE;
        pLinkArgs->linkCfg.speed     = ENET_SPEED_1GBIT;
        pLinkArgs->linkCfg.duplexity = ENET_DUPLEX_FULL;
    }
}

int32_t EnetTestDirectedPkt_setTxPktInfo(EnetTestTaskObj *taskObj,
                                         uint32_t txChIndex,
                                         uint32_t pktNum,
                                         EnetDma_Pkt *pktInfo,
                                         bool *testComplete)
{
    EthFrame *frame;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
    memcpy(frame->hdr.dstMac, dstMultMacAddr, ENET_MAC_ADDR_LEN);
    memcpy(frame->hdr.srcMac, &stateObj->hostMacAddr[0U], ENET_MAC_ADDR_LEN);
    frame->hdr.etherType = Enet_htons(ETHERTYPE_EXPERIMENTAL1);
    memset(&frame->payload[0U], (uint8_t)(0xA5 + pktNum), stateObj->txChCfgInfo[txChIndex]->maxTxPktLen);
    pktInfo->sgList.list[0].segmentFilledLen  = taskObj->stateObj.txChCfgInfo[txChIndex]->maxTxPktLen +
                           sizeof(EthFrameHeader);

    /* Set directed packet port number as MAC port 3 */
    pktInfo->txPortNum = ENET_DIRECTED_PKT_TEST_DIRECTED_PORT;
    pktInfo->appPriv = stateObj;
    *testComplete = BFALSE;

    return ENET_SOK;
}

int32_t EnetTestDirectedPkt_Run(EnetTestTaskObj *taskObj)
{
    int32_t status = ENET_SOK;
    uint32_t portMask;

    EnetTestCommon_waitForPortLink(taskObj);
   EnetAppUtils_assert(taskObj->taskCfg->numTxCh == 1U);
   EnetAppUtils_assert(taskObj->taskCfg->numRxFlow == 1U);

    /* Add ALE entry for destination Multicast MAC address with port numbers set
     * to MAC port 2 and host port. */
    portMask = (ENET_BIT(CPSW_ALE_MACPORT_TO_ALEPORT(ENET_DIRECTED_PKT_TEST_ACTUAL_PORT)) |
                CPSW_ALE_HOST_PORT_MASK);

    status = EnetTestCommon_setAleMulticastEntry(taskObj,
                                                 dstMultMacAddr,
                                                 0U,
                                                 2U,
                                                 portMask);
    if (status == ENET_SOK)
    {
        status = EnetTestCommon_pktTxRx(taskObj);
    }

    return status;
}

int32_t EnetTestDirectedPkt_processRxPkt(EnetTestTaskObj *taskObj,
                                         uint32_t rxFlowId,
                                         uint32_t pktNum,
                                         EnetDma_Pkt *pktInfo,
                                         bool *testComplete)
{
    int32_t status = ENET_SOK;

    /* Test logic:
     * - Loopback is enabled on MAC port 3.
     * - ALE entry is added for dstMultMacAddr with MAC port 2 and host port as members.
     * - Packets are sent out from host port with dstMultMacAddr, but with directed packet port number
     *   set to MAC port 3.
     * - Directed packet feature takes precedence over ALE lookup and reaches MAC port 3 and gets looped back.
     * - Packet reaches this function only if directed packet feature worked.
     * - Received packet's received port number is checked against MAC port 3.
     * - If all packets are looped back, test is passed else failed. */
    if (pktInfo->rxPortNum == ENET_DIRECTED_PKT_TEST_DIRECTED_PORT)
    {
        status = ENET_SOK;
    }
    else
    {
        status = ENET_EFAIL;
    }

    *testComplete = BFALSE;

    return status;
}
