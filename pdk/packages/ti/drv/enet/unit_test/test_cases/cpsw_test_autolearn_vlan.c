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
 * \file     cpsw_test_autolearn_vlan.c
 *
 * \brief    This file contains the cpsw_test_autolearn_vlan  implementation.
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
#include "cpsw_test_autolearn_vlan.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define ENET_TEST_VLAN_ID               (0x10)
#define ENET_TEST_VLAN_MEMBER_MASK      (3U)
#define ENET_TEST_ETHERTYPE             (0x800)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */
/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

static void EnetTest_setOUIEntry(EnetTestTaskObj *taskObj,
                                  uint8_t macAddr[]);

static void EnetTest_setIpv4Entry(EnetTestTaskObj *taskObj,
                                   uint8_t ipv4Addr[]);

static void EnetTest_setIpv6Entry(EnetTestTaskObj *taskObj,
                                   uint8_t ipv6Addr[]);

static void EnetTest_setEtherTypeEntry(EnetTestTaskObj *taskObj,
                                        uint16_t etherType);

static void EnetTest_setAleMulticastEntry(EnetTestTaskObj *taskObj,
                                           uint8_t macAddr[]);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* Test ALE Entry Multicast Address */
static uint8_t testMCastAddr[ENET_MAC_ADDR_LEN] =
{
    0x01, 0x80, 0xC2, 0x00, 0x00, 0x02
};

static uint8_t testOuiAddr[ENET_OUI_ADDR_LEN] =
{
    0xab, 0xcd, 0xef
};

static uint8_t testIpv4Addr[ENET_IPv4_ADDR_LEN] =
{
    172, 24, 190, 113
};

static uint8_t testIpv6Addr[ENET_IPv6_ADDR_LEN] =
{
    0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0x00, 0x01, 0x02,
    0x03, 0x04, 0x05, 0x06
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void EnetTestAutoLearnVlan_setVLANentry(EnetTestTaskObj *taskObj,
                                        uint32_t vlanId,
                                        uint32_t vlanMemberMask)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_VlanEntryInfo vlanEntry;
    uint32_t aleIdx;
    CpswAle_GetVlanEntryOutArgs getVlanOutArgs;
    CpswAle_VlanIdInfo getVlanInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    vlanEntry.disallowIPFrag           = BFALSE;
    vlanEntry.forceUntaggedEgressMask  = 0x0;
    vlanEntry.limitIPNxtHdr            = BFALSE;
    vlanEntry.noLearnMask              = 0x0;
    vlanEntry.regMcastFloodMask        = EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK;
    vlanEntry.unregMcastFloodMask      = EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK;
    vlanEntry.vidIngressCheck          = BFALSE;
    vlanEntry.vlanMemberList           = vlanMemberMask;
    vlanEntry.vlanIdInfo.vlanId        = vlanId;
    vlanEntry.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &vlanEntry, &aleIdx);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_VLAN,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_setAleEntry() failed CPSW_ALE_IOCTL_ADD_UCAST: %d\n",
                           status);
    }

    getVlanInArgs.vlanId        = vlanId;
    getVlanInArgs.tagType       = ENET_VLAN_TAG_TYPE_INNER;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &getVlanInArgs, &getVlanOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_LOOKUP_VLAN,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestAutoLearnVlan_setVLANentry() failed CPSW_ALE_IOCTL_LOOKUP_VLAN: %d\n",
                           status);
    }
    else
    {
       EnetAppUtils_assert(aleIdx == getVlanOutArgs.aleEntryIdx);
       EnetAppUtils_assert(vlanEntry.disallowIPFrag == getVlanOutArgs.disallowIPFrag);
       EnetAppUtils_assert(vlanEntry.forceUntaggedEgressMask == getVlanOutArgs.forceUntaggedEgressMask);
       EnetAppUtils_assert(vlanEntry.limitIPNxtHdr == getVlanOutArgs.limitIPNxtHdr);
       EnetAppUtils_assert(vlanEntry.noLearnMask == getVlanOutArgs.noLearnMask);
       EnetAppUtils_assert(vlanEntry.regMcastFloodMask == getVlanOutArgs.regMcastFloodMask);
       EnetAppUtils_assert(vlanEntry.unregMcastFloodMask == getVlanOutArgs.unregMcastFloodMask);
       EnetAppUtils_assert(vlanEntry.vidIngressCheck == getVlanOutArgs.vidIngressCheck);
       EnetAppUtils_assert(vlanEntry.vlanMemberList == getVlanOutArgs.vlanMemberList);
    }
}

static int32_t EnetTestAutoLearnVlan_pktRxTx(EnetTestTaskObj *taskObj,
                                             uint32_t txCfgIndex,
                                             uint32_t rxCfgIndex)
{
    EnetDma_PktQ txSubmitQ;
    EnetDma_Pkt *pktInfo;
    uint32_t txRetrievePktCnt, loopTxPktCnt, totalTxCnt;
    uint32_t rxReadyCnt;
    uint32_t loopCnt, pktCnt, loopRxPktCnt, zeroRxCount;
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    bool testComplete;
    uint32_t rxPktNum;

    totalTxCnt = 0U;
    for (loopCnt = 0U;
         loopCnt < stateObj->txChCfgInfo[txCfgIndex]->pktSendLoopCount;
         loopCnt++)
    {
        pktCnt = 0U;

        loopRxPktCnt = loopTxPktCnt = 0U;
        /* Transmit a single packet */
        EnetQueue_initQ(&txSubmitQ);

        /* Dequeue one free TX Eth packet */
        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->txChObj[txCfgIndex].txFreePktInfoQ);

        while (NULL != pktInfo)
        {
            pktCnt++;
            /* Fill the TX Eth frame with test content */
            EnetTestCommon_setTxPktInfo(taskObj, txCfgIndex, pktCnt,
                                        pktInfo, &testComplete);

            /* Enqueue the packet for later transmission */
            EnetQueue_enq(&txSubmitQ, &pktInfo->node);

            if (pktCnt >= stateObj->txChCfgInfo[txCfgIndex]->pktSendCount)
            {
                break;
            }

            /* Dequeue one free TX Eth packet */
            pktInfo =
                (EnetDma_Pkt *)EnetQueue_deq(&stateObj->txChObj[txCfgIndex].txFreePktInfoQ);
        }

        loopTxPktCnt = EnetQueue_getQCount(&txSubmitQ);
        while (0U != EnetQueue_getQCount(&txSubmitQ))
        {
            uint32_t txCnt = EnetQueue_getQCount(&txSubmitQ);
            status =EnetDma_submitTxPktQ(stateObj->txChObj[txCfgIndex].hTxCh,
                                                  &txSubmitQ);
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

        EnetTestAutoLearnVlan_setVLANentry(taskObj, ENET_TEST_VLAN_ID, ENET_TEST_VLAN_MEMBER_MASK);
        EnetTest_setAleMulticastEntry(taskObj, &testMCastAddr[0]);
        EnetTest_setEtherTypeEntry(taskObj, ENET_TEST_ETHERTYPE);
        EnetTest_setIpv4Entry(taskObj, testIpv4Addr);
        EnetTest_setIpv6Entry(taskObj, testIpv6Addr);
        EnetTest_setOUIEntry(taskObj, testOuiAddr);

        rxPktNum = 0;
        /* Wait for packet reception */
        do
        {
            /* Get the packets received so far */
            rxReadyCnt = EnetTestCommon_receivePkts(taskObj, rxCfgIndex);
            if (rxReadyCnt > 0U)
            {
                /* Consume the received packets and release them */
                pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->rxFlowObj[rxCfgIndex].rxReadyQ);
                while (NULL != pktInfo)
                {
                    EnetTestCommon_processRxPkt(taskObj,
                                                rxCfgIndex,
                                                rxPktNum,
                                                pktInfo,
                                                &testComplete);
                    rxPktNum++;
                    /* Release the received packet */
                    EnetQueue_enq(&stateObj->rxFlowObj[rxCfgIndex].rxFreeQ,
                                  &pktInfo->node);
                    pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->rxFlowObj[rxCfgIndex].rxReadyQ);
                }

                /*Submit now processed buffers */
                if (status == ENET_SOK)
                {
                   EnetDma_submitRxPktQ(stateObj->rxFlowObj[rxCfgIndex].hRxFlow,
                                                 &stateObj->rxFlowObj[rxCfgIndex].rxFreeQ);
                }
            }

            loopRxPktCnt += rxReadyCnt;
        }
        while (loopRxPktCnt < 500);

        EnetTestAutoLearnVlan_setVLANentry(taskObj, 0x10, 2);
       EnetAppUtils_print("Transmitted & Received 500 packets & VLAN Entry modified\n");
        loopRxPktCnt = 0;
        zeroRxCount  = 0;

        /* Wait for packet reception */
        do
        {
            /* Get the packets received so far */
            rxReadyCnt = EnetTestCommon_receivePkts(taskObj, rxCfgIndex);
            if (rxReadyCnt > 0U)
            {
                /* Consume the received packets and release them */
                pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->rxFlowObj[rxCfgIndex].rxReadyQ);
                while (NULL != pktInfo)
                {
                    EnetTestCommon_processRxPkt(taskObj, rxCfgIndex,  rxPktNum,
                                                pktInfo, &testComplete);
                    rxPktNum++;
                    /* Release the received packet */
                    EnetQueue_enq(&stateObj->rxFlowObj[rxCfgIndex].rxFreeQ,
                                  &pktInfo->node);
                    pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&stateObj->rxFlowObj[rxCfgIndex].rxReadyQ);
                }

                /*Submit now processed buffers */
                if (status == ENET_SOK)
                {
                   EnetDma_submitRxPktQ(stateObj->rxFlowObj[rxCfgIndex].hRxFlow,
                                                 &stateObj->rxFlowObj[rxCfgIndex].
                                                 rxFreeQ);
                }
            }
            else
            {
                zeroRxCount++;
            }

            loopRxPktCnt += rxReadyCnt;
        }
        while (zeroRxCount < 1000);

        /* Reset ready count */
        totalTxCnt += loopTxPktCnt;
    }

   EnetAppUtils_print("VLAN Test Succes \n ");
    ENET_IOCTL_SET_NO_ARGS(&prms);
    Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_TABLE,
               &prms);

    if (status == ENET_SOK)
    {
       EnetAppUtils_print("Transmitted & Received %d packets\n", totalTxCnt);
    }
    else
    {
       EnetAppUtils_print("Failed to transmit/receive packets: %d, transmitted: %d \n",
                           stateObj->txChCfgInfo[txCfgIndex]->pktSendLoopCount, totalTxCnt);
    }

    return status;
}

int32_t EnetTestAutoLearnVlan_Run(EnetTestTaskObj *taskObj)
{
    int32_t status;

    EnetTestCommon_waitForPortLink(taskObj);

   EnetAppUtils_assert(taskObj->taskCfg->numTxCh == 1U);
   EnetAppUtils_assert(taskObj->taskCfg->numRxFlow == 1U);

    status = EnetTestAutoLearnVlan_pktRxTx(taskObj, 0, 0);

    return status;
}

void EnetTestAutoLearnVlan_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                             Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg      = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn           = BFALSE;
}

void EnetTestAutoLearnVlan_setOpenPrms(EnetTestTaskObj *taskObj,
                                       Cpsw_Cfg *pCpswCfg,
                                       EnetUtils_Cfg *pUtilsPrms)
{
    pCpswCfg->aleCfg.modeFlags =
        (CPSW_ALE_CFG_MODULE_EN |
         CPSW_ALE_CFG_UNKNOWN_UCAST_FLOOD2HOST);
    pCpswCfg->aleCfg.agingCfg.autoAgingEn           = BFALSE;
    pCpswCfg->aleCfg.portCfg[0].learningCfg.noLearn        = BFALSE;
    pCpswCfg->aleCfg.portCfg[1].learningCfg.noLearn        = BFALSE;
    pCpswCfg->aleCfg.portCfg[0].learningCfg.noLearn        = BTRUE;
    pCpswCfg->aleCfg.portCfg[1].learningCfg.noLearn        = BTRUE;
    pCpswCfg->aleCfg.portCfg[1].vlanCfg.dropUntagged       = BTRUE;
    pCpswCfg->aleCfg.portCfg[0].vlanCfg.dropUntagged       = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.unknownUnregMcastFloodMask = 0x0;
    pCpswCfg->aleCfg.vlanCfg.unknownRegMcastFloodMask   = 0x0;
    pCpswCfg->aleCfg.vlanCfg.unknownVlanMemberListMask  = 0x0;
    pCpswCfg->aleCfg.vlanCfg.aleVlanAwareMode           = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.autoLearnWithVlan          = BTRUE;
    pCpswCfg->aleCfg.nwSecCfg.vid0ModeEn               = BFALSE;
}

static void EnetTest_setOUIEntry(EnetTestTaskObj *taskObj,
                                  uint8_t macAddr[])
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setOuiOutArgs;
    CpswAle_OuiEntryInfo setOuiInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memcpy(&setOuiInArgs.ouiAddr[0], macAddr, sizeof(setOuiInArgs.ouiAddr));

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setOuiInArgs, &setOuiOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_OUI,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_setOUIEntry() failed : %d\n", status);
    }
}

static void EnetTest_setIpv4Entry(EnetTestTaskObj *taskObj,
                                   uint8_t ipv4Addr[])
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setIpv4OutArgs;
    CpswAle_IPv4EntryInfo setIpv4InArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memcpy(&setIpv4InArgs.ipv4Addr[0], ipv4Addr, sizeof(setIpv4InArgs.ipv4Addr));
    setIpv4InArgs.numLSBIgnoreBits = 0U;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setIpv4InArgs, &setIpv4OutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_IPV4ADDR,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_setIpv4Entry() failed : %d\n", status);
    }
}

static void EnetTest_setIpv6Entry(EnetTestTaskObj *taskObj,
                                   uint8_t ipv6Addr[])
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setIpv6OutArgs;
    CpswAle_IPv6EntryInfo setIpv6InArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memcpy(&setIpv6InArgs.ipv6Addr[0], ipv6Addr, sizeof(setIpv6InArgs.ipv6Addr));
    setIpv6InArgs.numLSBIgnoreBits = 0U;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setIpv6InArgs, &setIpv6OutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_IPV6ADDR,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_setIpv6Entry() failed : %d\n", status);
    }
}

static void EnetTest_setEtherTypeEntry(EnetTestTaskObj *taskObj,
                                        uint16_t etherType)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setEtherTypeOutArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &etherType, &setEtherTypeOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_ETHERTYPE,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_setEtherTypeEntry() failed : %d\n", status);
    }
}

static void EnetTest_setAleMulticastEntry(EnetTestTaskObj *taskObj,
                                           uint8_t macAddr[])
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setMcastOutArgs;
    CpswAle_SetMcastEntryInArgs setMcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memcpy(&setMcastInArgs.addr.addr[0], macAddr,
           sizeof(setMcastInArgs.addr.addr));
    setMcastInArgs.addr.vlanId = 0U;

    setMcastInArgs.info.super      = BFALSE;
    setMcastInArgs.info.fwdState   = CPSW_ALE_FWDSTLVL_FWD_LRN;
    setMcastInArgs.info.portMask   = EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK;
    setMcastInArgs.info.numIgnBits = 0U;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setMcastInArgs, &setMcastOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_MCAST,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_setAleMulticastEntry() failed CPSW_ALE_IOCTL_ADD_MCAST: %d\n",
                           status);
    }
}

/* end of file */
