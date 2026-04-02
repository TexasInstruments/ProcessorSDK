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
 * \file     cpsw_test_vlan.c
 *
 * \brief    This file contains the cpsw vlan test implementation.
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
#include <ti/drv/enet/examples/utils/include/enet_ethpatterns.h>

#include <ti/osal/osal.h>

#include <ti/board/board.h>

#include "enet_test_entry.h"
#include "enet_test_base.h"
#include "cpsw_test_multicast.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ETH_TEST_VLAN_ID                (100U)
#define ETH_TEST_VLAN_PRI               (3U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static int32_t EnetTest_xmitTest(EnetTestTaskObj *taskObj,
                                  uint32_t txCfgIndex,
                                  uint32_t num,
                                  const uint8_t *dstAddr,
                                  const uint8_t *srcAddr,
                                  uint16_t etherType,
                                  uint16_t length);

static int32_t EnetTest_transmitPkts(EnetTestTaskObj *taskObj,
                                      EnetDma_PktQ *pTxSubmitQ,
                                      uint32_t txCfgIndex);

static void EnetTest_transmitTest(EnetTestTaskObj *taskObj,
                                 uint32_t txCfgIndex,
                                 uint32_t rxCfgIndex);

static void EnetTest_addAleVlanEntry(EnetTestTaskObj *taskObj);

static void EnetTest_removeAleVlanEntry(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* Broadcast address */
static uint8_t mcastAddr[ENET_MAC_ADDR_LEN] =
{
    0x01U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x01U
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestMulticast_Run(EnetTestTaskObj *taskObj)
{
    int32_t status      = ENET_SOK;
    uint32_t numIgnBits = 2U;
    uint32_t portMask   = (EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK);
    uint32_t vlanId     = 0U;

    status = EnetTestCommon_setAleMulticastEntry(taskObj, &mcastAddr[0],
                                                 vlanId, /* Vlan Id */
                                                 numIgnBits,
                                                 portMask);
    if (ENET_SOK == status)
    {
        status = EnetTestCommon_getAleMulticastEntry(taskObj, &mcastAddr[0],
                                                     vlanId, /* Vlan Id */
                                                     numIgnBits,
                                                     &portMask);
        if (ENET_SOK == status)
        {
           EnetAppUtils_assert(portMask == (EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK));
        }
    }

    if (ENET_SOK == status)
    {
        status = EnetTestCommon_Run(taskObj);
    }

    if (ENET_SOK == status)
    {
        /* Transmit untagged packets
         * VLAN tag will be inserted from port 0 default settings */
       EnetAppUtils_print("Transmit untagged packets\n");
       EnetAppUtils_print("  VLAN will be inserted from port 0 settings:\n");
       EnetAppUtils_print("  - VLAN ID  = %d\n", ETH_TEST_VLAN_ID);
       EnetAppUtils_print("  - VLAN Pri = %d\n", ETH_TEST_VLAN_PRI);
       EnetAppUtils_assert(taskObj->taskCfg->numTxCh == 1U);
       EnetAppUtils_assert(taskObj->taskCfg->numRxFlow == 1U);

        EnetTest_transmitTest(taskObj, 0U, 0U);

        /* Add ALE entry to force untagged packets on ETH_TEST_VLAN_ID */
       EnetAppUtils_print("Add ALE entry to force untagged packets on VLAN ID %d\n",
                           ETH_TEST_VLAN_ID);
        EnetTest_addAleVlanEntry(taskObj);

        /* Transmit untagged packets
         * VLAN tag inserted from port 0 default settings to be dropped by ALE's
         *force untagged settings */
       EnetAppUtils_print("Transmit untagged packets\n");
       EnetAppUtils_print("  VLAN will be inserted from port 0 settings, but shall be forced untagged by ALE\n");
        EnetTest_transmitTest(taskObj, 0U, 0U);

        /* Remove ALE entry for ETH_TEST_VLAN_ID */
       EnetAppUtils_print("Remove ALE entry for VLAN ID %d\n", ETH_TEST_VLAN_ID);
        EnetTest_removeAleVlanEntry(taskObj);

        /* Transmit untagged packets
         * VLAN tag will be inserted from port 0 default settings */
       EnetAppUtils_print("Transmit untagged packets\n");
       EnetAppUtils_print("  VLAN will be inserted from port 0 settings:\n");
       EnetAppUtils_print("  - VLAN ID  = %d\n", ETH_TEST_VLAN_ID);
       EnetAppUtils_print("  - VLAN Pri = %d\n", ETH_TEST_VLAN_PRI);
        EnetTest_transmitTest(taskObj, 0U, 0U);
    }

    return status;
}

void EnetTestMulticast_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                         Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg      = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn           = BFALSE;
}

void EnetTestMulticast_setOpenPrms(EnetTestTaskObj *taskObj,
                                   Cpsw_Cfg *pCpswCfg,
                                   EnetUtils_Cfg *pUtilsPrms)
{
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    pCpswCfg->aleCfg.vlanCfg.aleVlanAwareMode           = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.unknownRegMcastFloodMask   = (EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK);
    pCpswCfg->aleCfg.vlanCfg.unknownUnregMcastFloodMask = 0x0;
    pCpswCfg->aleCfg.vlanCfg.unknownVlanMemberListMask  = (EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK);

    if ((stateObj->curIteration % 2U) != 0U)
    {
        pCpswCfg->vlanCfg.vlanSwitch = ENET_VLAN_TAG_TYPE_OUTER;
    }
    else
    {
        pCpswCfg->vlanCfg.vlanSwitch = ENET_VLAN_TAG_TYPE_INNER;
    }
}

static void EnetTest_addAleVlanEntry(EnetTestTaskObj *taskObj)
{
    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj   = &taskObj->stateObj;
    CpswAle_VlanEntryInfo inArgs =
    {
        .vlanIdInfo.vlanId       = ETH_TEST_VLAN_ID,
        .vlanMemberList          = CPSW_ALE_ALL_PORTS_MASK,
        .unregMcastFloodMask     = 2U,
        .regMcastFloodMask       = 3U,
        .forceUntaggedEgressMask = CPSW_ALE_ALL_PORTS_MASK,
        .noLearnMask             = 0U,
        .vidIngressCheck         = BFALSE,
        .limitIPNxtHdr           = BFALSE,
        .disallowIPFrag          = BFALSE,
    };
    uint32_t outArgs;
    int32_t status;

    if ((stateObj->curIteration % 2U) != 0U)
    {
        inArgs.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_OUTER;
    }
    else
    {
        inArgs.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
    }

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_VLAN, &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_addAleVlanEntry() ADD_VLAN ioctl failed: %d\n",
                           status);
    }
}

static void EnetTest_removeAleVlanEntry(EnetTestTaskObj *taskObj)
{
    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    CpswAle_VlanIdInfo inArgs  =
    {
        .vlanId = ETH_TEST_VLAN_ID,
    };
    int32_t status;

    if ((stateObj->curIteration % 2U) != 0U)
    {
        inArgs.tagType       = ENET_VLAN_TAG_TYPE_OUTER;
    }
    else
    {
        inArgs.tagType       = ENET_VLAN_TAG_TYPE_INNER;
    }

    ENET_IOCTL_SET_IN_ARGS(&prms, &inArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_REMOVE_VLAN, &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_removeAleVlanEntry() REMOVE_VLAN ioctl failed: %d\n",
                           status);
    }
}

static int32_t EnetTest_transmitPkts(EnetTestTaskObj *taskObj,
                                      EnetDma_PktQ *pTxSubmitQ,
                                      uint32_t txCfgIndex)
{
    uint32_t txRetrievePktCnt, txCnt;
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    while (0U != EnetQueue_getQCount(pTxSubmitQ))
    {
        txCnt  = EnetQueue_getQCount(pTxSubmitQ);
        status =EnetDma_submitTxPktQ(stateObj->txChObj[txCfgIndex].hTxCh,
                                              pTxSubmitQ);
        /* Retrieve TX free packets */
        if (status == ENET_SOK)
        {
            txCnt            = txCnt - EnetQueue_getQCount(pTxSubmitQ);
            txRetrievePktCnt = 0U;
            while (txRetrievePktCnt != txCnt)
            {
                /* This is not failure as HW is busy sending packets, we need
                 * to wait and again call retrieve packets */
                EnetTest_wait(1U);
                txRetrievePktCnt += EnetTestCommon_retrieveFreeTxPkts(taskObj, txCfgIndex);
#if DEBUG
               EnetAppUtils_print("Failed to retrieve consumed transmit packets: %d\n",
                                   status);
#endif
            }
        }
        else
        {
            break;
        }
    }

    return status;
}

static int32_t EnetTest_xmitTest(EnetTestTaskObj *taskObj,
                                  uint32_t txCfgIndex,
                                  uint32_t num,
                                  const uint8_t *dstAddr,
                                  const uint8_t *srcAddr,
                                  uint16_t etherType,
                                  uint16_t length)
{
    int32_t retVal = ENET_SOK;
    EnetDma_Pkt *pktInfo;
    uint16_t len    = length - ETH_TEST_DATA_HDR_LEN;
    uint32_t pktCnt = 0U;
    DataFramePayload *payload;
    EnetDma_PktQ txSubmitQ;
    EthFrame *frame;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    while (pktCnt < num)
    {
        EnetQueue_initQ(&txSubmitQ);

        /* Dequeue one free TX Eth packet */
        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->txChObj[txCfgIndex].txFreePktInfoQ);

        while (NULL != pktInfo)
        {
            pktCnt++;
            /* Fill the TX Eth frame with test content */
            frame   = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
            payload = (DataFramePayload *)frame->payload;
            memcpy(frame->hdr.dstMac, mcastAddr, ENET_MAC_ADDR_LEN);
            memcpy(frame->hdr.srcMac, &stateObj->hostMacAddr[0U],
                   ENET_MAC_ADDR_LEN);
            frame->hdr.etherType = Enet_htons(etherType);

            payload->type = 0U; /* DataPattern1 */
            payload->len  = Enet_htons(len);
            memcpy(payload->data, Enet_DataPattern1, len);

            pktInfo->sgList.list[0].segmentFilledLen = length + sizeof(EthFrameHeader);
            pktInfo->appPriv    = &stateObj;

            /* Enqueue the packet for later transmission */
            EnetQueue_enq(&txSubmitQ, &pktInfo->node);

            if (pktCnt >= num)
            {
                break;
            }

            /* Dequeue one free TX Eth packet */
            pktInfo =
                (EnetDma_Pkt *)EnetQueue_deq(&stateObj->txChObj[txCfgIndex].txFreePktInfoQ);
        }

        retVal = EnetTest_transmitPkts(taskObj, &txSubmitQ, txCfgIndex);
    }

    return retVal;
}

static void EnetTest_discardRxPkts(EnetTestTaskObj *taskObj,
                                    uint32_t rxCfgIndex)
{
    EnetDma_Pkt *pkt;
    uint32_t count;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    EnetTestCommon_receivePkts(taskObj, rxCfgIndex);
    /* Discard any packets sitting in rxReadyQ before the test starts */
    count = EnetQueue_getQCount(&stateObj->rxFlowObj[rxCfgIndex].rxReadyQ);
    if (count != 0U)
    {
       EnetAppUtils_print("EnetTest_discardRxPkts() rxReadyQ has %d packets, discarding all\n",
                           count);
        pkt = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->rxFlowObj[rxCfgIndex].rxReadyQ);
        while (pkt != NULL)
        {
            /* Release the received packet */
            EnetQueue_enq(&stateObj->rxFlowObj[rxCfgIndex].rxFreeQ,
                          &pkt->node);
            pkt = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->rxFlowObj[rxCfgIndex].rxReadyQ);
        }
    }
}

static void EnetTest_transmitTest(EnetTestTaskObj *taskObj,
                                 uint32_t txCfgIndex,
                                 uint32_t rxCfgIndex)
{
    uint32_t iterations = ETH_TEST_ITER_S_COUNT;
    uint16_t len        = 500U;
    uint32_t retVal;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    EnetTest_discardRxPkts(taskObj, rxCfgIndex);

    EnetTest_wait(10U);

    /* Transmit frames */
   EnetAppUtils_print("Test_transmit: transmitting %d packets\n", iterations);
    retVal = EnetTest_xmitTest(taskObj,
                                txCfgIndex,
                                iterations,
                                mcastAddr,
                                &stateObj->hostMacAddr[0U],
                                ETHERTYPE_EXPERIMENTAL1,
                                len);
    if (retVal != ENET_SOK)
    {
       EnetAppUtils_print("Test_transmit: failed to transmit %d packets\n",
                           iterations);
    }
}
