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
 * \file     enet_switching_logic.c
 *
 * \brief    This file contains the enet_switching_logic test implementation.
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
#include "cpsw_test_aging_maxlength.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ENET_TEST_MACPORT_RX_MTU                        (1000U)
#define ENET_TEST_HOSTPORT_RX_MTU                       (1000U)

/* TX priority MTU should always be higher than mac port MTU */
#define ENET_TXPRI0_7_MTU                          (1200U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test ALE Entry Address */
static uint8_t testAddr[ENET_MAC_ADDR_LEN] =
{
    0x40U, 0xcdU, 0xefU, 0xfeU, 0xdcU, 0xbaU
};

static volatile uint32_t gRxHostMtu = 0U;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void EnetTestAgingMaxPktLength_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                                 Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn      = BFALSE;
    macCfg->rxMtu           = ENET_TEST_MACPORT_RX_MTU;
}

void EnetTestAgingMaxPktLength_setOpenPrms(EnetTestTaskObj *taskObj,
                                           Cpsw_Cfg *pCpswCfg,
                                           EnetUtils_Cfg *pUtilsPrms)
{
    uint32_t i;

    pCpswCfg->aleCfg.agingCfg.autoAgingEn = BTRUE;
    pCpswCfg->aleCfg.agingCfg.agingPeriodInMs = 1000;
    pCpswCfg->hostPortCfg.rxMtu                  = ENET_TEST_HOSTPORT_RX_MTU;
    gRxHostMtu                                      = ENET_TEST_HOSTPORT_RX_MTU;

    /* Set MAC port global config */
    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        pCpswCfg->txMtu[i] = ENET_TXPRI0_7_MTU;
    }
}

static void EnetTestAgingMaxPktLength_showStats(EnetTestTaskObj *taskObj)
{
    Enet_IoctlPrms prms;
    Enet_MacPort macPort;
    CpswStats_PortStats portStats;
    int32_t status = ENET_SOK;
    CpswStats_HostPort_2g *hostPortStats;
    CpswStats_MacPort_2g *macPortStats;

    ENET_IOCTL_SET_OUT_ARGS(&prms, &portStats);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_STATS_IOCTL_GET_HOSTPORT_STATS,
                        &prms);
    if (status == ENET_SOK)
    {
       hostPortStats = (CpswStats_HostPort_2g *)&portStats;

       EnetAppUtils_print("\n Port 0 Statistics\n");
       EnetAppUtils_print("-----------------------------------------\n");
       EnetAppUtils_print(" rxOversizedFrames = %llu\n", hostPortStats->rxOversizedFrames);
       EnetAppUtils_print(" portMaskDrop      = %llu\n", hostPortStats->portMaskDrop);
       EnetAppUtils_print("\n");
    }
    else
    {
       EnetAppUtils_print("%s: Failed to get host stats: %d\n", __func__, status);
    }

    if (status == ENET_SOK)
    {
        EnetTestMacPortList_t enabledPorts;
        EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
        /* Take any port from enabled ports */
        macPort = enabledPorts.macPortList[0];

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &macPort, &portStats);
        status = Enet_ioctl(taskObj->stateObj.hEnet,
                            taskObj->stateObj.coreId,
                            ENET_STATS_IOCTL_GET_MACPORT_STATS,
                            &prms);
        if (status == ENET_SOK)
        {
            macPortStats = (CpswStats_MacPort_2g *)&portStats;
           EnetAppUtils_print("\n Port 1 Statistics\n");
           EnetAppUtils_print("-----------------------------------------\n");
           EnetAppUtils_print("  rxOversizedFrames = %llu\n", macPortStats->rxOversizedFrames);
           EnetAppUtils_print(" portMaskDrop       = %llu\n", macPortStats->portMaskDrop);
           EnetAppUtils_print("\n");
        }
        else
        {
           EnetAppUtils_print("%s: failed to get MAC stats: %d\n", __func__, status);
        }
    }
}

static int32_t EnetTestAgingMaxPktLength_setAleEntry(EnetTestTaskObj *taskObj,
                                                     uint8_t macAddr[],
                                                     uint32_t portNum)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setUcastOutArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;

    memcpy(&setUcastInArgs.addr.addr[0U], macAddr,
           sizeof(setUcastInArgs.addr.addr));
    setUcastInArgs.addr.vlanId  = 0U;
    setUcastInArgs.info.portNum = portNum;
    setUcastInArgs.info.blocked = BFALSE;
    setUcastInArgs.info.secure  = BFALSE;
    setUcastInArgs.info.super   = BFALSE;
    setUcastInArgs.info.ageable = BTRUE;
    setUcastInArgs.info.trunk   = BFALSE;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &setUcastOutArgs);

    status = Enet_ioctl(taskObj->stateObj.hEnet, taskObj->stateObj.coreId, CPSW_ALE_IOCTL_ADD_UCAST,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("%s: failed CPSW_ALE_IOCTL_ADD_UCAST: %d\n",  __func__, status);
    }

    return status;
}

static int32_t EnetTestAgingMaxPktLength_lookupAleEntry(EnetTestTaskObj *taskObj,
                                                        uint8_t
                                                        macAddr[])
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_MacAddrInfo macAddrInfo;
    CpswAle_GetUcastEntryOutArgs getUcastInArgs;

    memcpy(&macAddrInfo.addr[0], macAddr, sizeof(macAddrInfo.addr));
    macAddrInfo.vlanId = 0U;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &macAddrInfo, &getUcastInArgs);

    status = Enet_ioctl(taskObj->stateObj.hEnet, taskObj->stateObj.coreId, CPSW_ALE_IOCTL_LOOKUP_UCAST,
                        &prms);
    return status;
}

static int32_t EnetTestAgingMaxPktLength_validateAging(EnetTestTaskObj *taskObj)
{
    int32_t status;

    status = EnetTestAgingMaxPktLength_setAleEntry(taskObj, &testAddr[0U], 1);

    if (status == ENET_SOK)
    {
        EnetTest_wait(2000U);
        status = EnetTestAgingMaxPktLength_lookupAleEntry(taskObj,
                                                          &testAddr[0U]);
        if (status != ENET_SOK)
        {
            /* If lookup failed, it indcates entry was aged out successfully */
            EnetAppUtils_print("%s: Test Passed: Entry got aged \n", __func__);
            status = ENET_SOK;
        }
        else
        {
            /* If lookup succeeded, it indcates entry was not aged out
             *successfully */
           EnetAppUtils_print("%s: Test Failed: Entry still exists in the ALE table, __func__ \n");
            status = ENET_EFAIL;
        }
    }

    return status;
}

static int32_t EnetTestAgingMaxPktLength_pktRxTx(EnetTestTaskObj *taskObj,
                                                 uint32_t txCfgIdx,
                                                 uint32_t rxCfgIdx)
{
    EnetDma_PktQ txSubmitQ;
    EnetDma_Pkt *pktInfo;
    EthFrame *frame;
    uint32_t txRetrievePktCnt;
    uint32_t rxReadyCnt;
    uint32_t pktCnt, loopRxPktCnt, loopTxPktCnt;
    int32_t status               = ENET_SOK;
    EnetTestStateObj *stateObj   = &taskObj->stateObj;
    EnetTestRxFlowObj *rxFlowObj = &stateObj->rxFlowObj[rxCfgIdx];
    EnetTestTxChObj *txFlowObj   = &stateObj->txChObj[txCfgIdx];

    /* Broadcast address */
    uint8_t bcastAddr[ENET_MAC_ADDR_LEN] = {0xffU, 0xffU, 0xffU, 0xffU, 0xffU, 0xffU};

    pktCnt = 0U;
    while (pktCnt < stateObj->txChCfgInfo[txCfgIdx]->pktSendCount)
    {
        loopRxPktCnt = loopTxPktCnt = 0U;
        /* Transmit a single packet */
        EnetQueue_initQ(&txSubmitQ);

        /* Dequeue one free TX Eth packet */
        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFlowObj->txFreePktInfoQ);

        while (NULL != pktInfo)
        {
            pktCnt++;
            /* Fill the TX Eth frame with test content */
            frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
            memcpy(frame->hdr.dstMac, bcastAddr, ENET_MAC_ADDR_LEN);
            memcpy(frame->hdr.srcMac, &stateObj->hostMacAddr[0U],
                   ENET_MAC_ADDR_LEN);
            frame->hdr.etherType = Enet_htons(ETHERTYPE_EXPERIMENTAL1);
            memset(&frame->payload[0U], (uint8_t)(0xA5 + pktCnt),
                   stateObj->txChCfgInfo[txCfgIdx]->maxTxPktLen);

            /* send bigger packet so host port truncates it */
            pktInfo->sgList.list[0].segmentFilledLen = gRxHostMtu + 100U;
            pktInfo->appPriv    = taskObj;
            /* Enqueue the packet for later transmission */
            EnetQueue_enq(&txSubmitQ, &pktInfo->node);

            if (pktCnt >= stateObj->txChCfgInfo[txCfgIdx]->pktSendCount)
            {
                break;
            }

            /* Dequeue one free TX Eth packet */
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFlowObj->txFreePktInfoQ);
        }

        loopTxPktCnt = EnetQueue_getQCount(&txSubmitQ);
        while (0U != EnetQueue_getQCount(&txSubmitQ))
        {
            uint32_t txCnt = EnetQueue_getQCount(&txSubmitQ);
            status =EnetDma_submitTxPktQ(txFlowObj->hTxCh, &txSubmitQ);
            /* Retrieve TX free packets */
            if (status == ENET_SOK)
            {
                txCnt            = txCnt - EnetQueue_getQCount(&txSubmitQ);
                txRetrievePktCnt = 0U;
                while (txRetrievePktCnt != txCnt)
                {
                    /* This is not failure as HW is busy sending packets, we need
                     * to wait and again call retrieve packets */
                    EnetTest_wait(1U);
                    txRetrievePktCnt += EnetTestCommon_retrieveFreeTxPkts(taskObj,
                                                                          txCfgIdx);
#if DEBUG
                   EnetAppUtils_print("Failed to retrieve consumed transmit packets: %d\n", status);
#endif
                }
            }
            else
            {
                break;
            }
        }

        /* Wait for packet reception */
        do
        {
            /* Get the packets received so far */
            rxReadyCnt = EnetTestCommon_receivePkts(taskObj, rxCfgIdx);
            if (rxReadyCnt > 0U)
            {
                /* Consume the received packets and release them */
                pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxFlowObj->rxReadyQ);

                while (NULL != pktInfo)
                {
                    EnetAppUtils_assert(pktInfo->sgList.numScatterSegments == 1U);
                    /* Consume the packet by just printing its content */
                    frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
#ifdef ENABLE_PRINTFRAME
                   EnetAppUtils_printFrame(frame, (pktInfo->sgList.list[0].segmentFilledLen - sizeof(EthFrameHeader)));
#endif
                    /* Release the received packet */
                    EnetQueue_enq(&rxFlowObj->rxFreeQ, &pktInfo->node);
                    pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxFlowObj->rxReadyQ);
                }

                /*Submit now processed buffers */
                if (status == ENET_SOK)
                {
                   EnetDma_submitRxPktQ(rxFlowObj->hRxFlow, &rxFlowObj->rxFreeQ);
                }
            }

            loopRxPktCnt += rxReadyCnt;
        }
        while (loopRxPktCnt < loopTxPktCnt);

       EnetAppUtils_print("Transmitted %d & Received %d packets\n", loopTxPktCnt, loopRxPktCnt);
    }

    return status;
}

int32_t EnetTestAgingMaxPktLength_Run(EnetTestTaskObj *taskObj)
{
    int32_t status;

    EnetTestCommon_waitForPortLink(taskObj);

    status = EnetTestAgingMaxPktLength_validateAging(taskObj);
    if (status == ENET_SOK)
    {
       EnetAppUtils_assert(taskObj->taskCfg->numTxCh == 1U);
       EnetAppUtils_assert(taskObj->taskCfg->numRxFlow == 1U);

        status = EnetTestAgingMaxPktLength_pktRxTx(taskObj, 0, 0);
        if (status == ENET_SOK)
        {
            /* Print ENET statistics of all ports */
            EnetTestAgingMaxPktLength_showStats(taskObj);
        }
    }

    return status;
}
