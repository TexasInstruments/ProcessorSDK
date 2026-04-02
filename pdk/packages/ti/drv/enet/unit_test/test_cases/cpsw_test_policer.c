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
 * \file     cpsw_test_policer.c
 *
 * \brief    This file contains the enet_policer test implementation.
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

#include "enet_test_base.h"
#include "enet_test_entry.h"
#include "cpsw_test_policer.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define ETH_TEST_IVLAN_ID 100U
#define ETH_TEST_OVLAN_ID 150U

#define ENET_TEST_POLICER_MBPS(x)                   ((x) * 1000000U)

#define RECV_BIT_RATE_PIR_TEST1 ENET_TEST_POLICER_MBPS(500U)
#define RECV_BIT_RATE_CIR_TEST1 ENET_TEST_POLICER_MBPS(300U)

#define RECV_BIT_RATE_PIR_TEST2 ENET_TEST_POLICER_MBPS(700U)
#define RECV_BIT_RATE_CIR_TEST2 ENET_TEST_POLICER_MBPS(500U)

#define RECV_BIT_RATE_PIR_TEST3 ENET_TEST_POLICER_MBPS(300U)
#define RECV_BIT_RATE_CIR_TEST3 ENET_TEST_POLICER_MBPS(150U)

#define RECV_BIT_RATE_RANGE_BASE_PIR ENET_TEST_POLICER_MBPS(950U)
#define RECV_BIT_RATE_RANGE_BASE_CIR ENET_TEST_POLICER_MBPS(900U)
#define RECV_BIT_RATE_RANGE_INTERVAL ENET_TEST_POLICER_MBPS(50U)
#define RECV_BIT_RATE_RANGE_MIN_PIR  ENET_TEST_POLICER_MBPS(50U)


#define ENET_TEST_POLICER_INGRESS_PORT  (ENET_MAC_PORT_4)
#define ENET_TEST_POLICER_EGRESS_PORT   (ENET_MAC_PORT_3)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static void EnetTestPolicer_setPolicer(EnetTestTaskObj *taskObj);

static int32_t EnetTestPolicer_setPolicerAleEntryType1(EnetTestTaskObj *taskObj);

static int32_t EnetTestPolicer_setPolicerAleEntryType2(EnetTestTaskObj *taskObj);

static int32_t EnetTestPolicer_setPolicerAleEntryType3(EnetTestTaskObj *taskObj);

static int32_t EnetTestPolicer_Test(EnetTestTaskObj *taskObj,
                                    uint32_t rxFlowCfgId,
                                    uint32_t recvbitRatePir,
                                    uint32_t recvbitRateCir);
int32_t EnetTestPolicer_setPolicerRange(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
static uint8_t testSrcAddr[ENET_MAC_ADDR_LEN] =
{
    0x02, 0x00, 0x00, 0x00, 0x00, 0x08
};

static uint8_t testHostPortDestAddr_Policer1[ENET_MAC_ADDR_LEN] =
{
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t testEgressPortDestAddr_Policer1[ENET_MAC_ADDR_LEN] =
{
    0x04, 0x00, 0x00, 0x00, 0x00, 0x02
};

static uint8_t testHostPortDestAddr_Policer2[ENET_MAC_ADDR_LEN] =
{
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t testEgressPortDestAddr_Policer2[ENET_MAC_ADDR_LEN] =
{
    0x08, 0x00, 0x00, 0x00, 0x00, 0x02
};

static uint8_t testOuiAddr[ENET_OUI_ADDR_LEN] =
{
    0x02, 0x00, 0x00
};

static uint8_t testSrcIpv4Addr[ENET_IPv4_ADDR_LEN] =
{
    172, 24, 190, 113
};

static uint8_t testDestIpv4Addr[ENET_IPv4_ADDR_LEN] =
{
    172, 24, 190, 112
};


static uint8_t testSrcIpv4AddrRange[ENET_IPv4_ADDR_LEN] =
{
    172, 24, 180, 1
};

static uint8_t testSrcAddrRange[ENET_MAC_ADDR_LEN] =
{
    0x02, 0x00, 0x00, 0x00, 0x11, 0x11
};

static uint8_t testEgressPortDestAddrRange[ENET_MAC_ADDR_LEN] =
{
    0x04, 0x00, 0x00, 0x00, 0x11, 0x11
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestPolicer_Run(EnetTestTaskObj *taskObj)
{
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    EnetTestPolicer_setPolicer(taskObj);
    status = EnetTestPolicer_setPolicerAleEntryType1(taskObj);
    if (ENET_SOK == status)
    {
        status = EnetTestPolicer_setPolicerAleEntryType2(taskObj);
    }

    if (ENET_SOK == status)
    {
        status = EnetTestPolicer_setPolicerAleEntryType3(taskObj);
    }

    if (ENET_SOK == status)
    {
        //status = EnetTestPolicer_setPolicerRange(taskObj);
    }
    if (ENET_SOK == status)
    {
        Enet_IoctlPrms prms;

        ENET_IOCTL_SET_NO_ARGS(&prms);

        status = Enet_ioctl(stateObj->hEnet,
                            stateObj->coreId,
                            CPSW_ALE_IOCTL_DUMP_TABLE,
                            &prms);
       EnetAppUtils_assert(status == ENET_SOK);

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet,
                            stateObj->coreId,
                            CPSW_ALE_IOCTL_DUMP_POLICER_ENTRIES,
                            &prms);
       EnetAppUtils_assert(status == ENET_SOK);

        status = EnetTestCommon_Run(taskObj);
    }

    if (status == ENET_SOK)
    {
        status = EnetTestPolicer_Test(taskObj, ENETTESTPOLICER_TYPE1_RXFLOWCFGID, RECV_BIT_RATE_PIR_TEST1, RECV_BIT_RATE_CIR_TEST1);
    }

    if (status == ENET_SOK)
    {
        status = EnetTestPolicer_Test(taskObj, ENETTESTPOLICER_TYPE2_RXFLOWCFGID, RECV_BIT_RATE_PIR_TEST2, RECV_BIT_RATE_CIR_TEST2);
    }

    if (status == ENET_SOK)
    {
        status = EnetTestPolicer_Test(taskObj, ENETTESTPOLICER_TYPE3_RXFLOWCFGID, RECV_BIT_RATE_PIR_TEST3, RECV_BIT_RATE_CIR_TEST3);
    }

    return status;
}

void EnetTestPolicer_setOpenPrms(EnetTestTaskObj *taskObj,
                                 Cpsw_Cfg *pCpswCfg,
                                 EnetUtils_Cfg *pUtilsPrms)
{
    CpswAle_Cfg *aleCfg;

    aleCfg = &pCpswCfg->aleCfg;

    aleCfg->policerGlobalCfg.policingEn     = BTRUE;
    aleCfg->policerGlobalCfg.yellowDropEn   = BFALSE;
    aleCfg->policerGlobalCfg.redDropEn      = BTRUE;
    aleCfg->policerGlobalCfg.policerNoMatchMode = CPSW_ALE_POLICER_NOMATCH_MODE_GREEN;
    aleCfg->vlanCfg.aleVlanAwareMode            = BTRUE;
    aleCfg->nwSecCfg.hostOuiNoMatchDeny         = BTRUE;
}

static void EnetTestPolicer_setPolicer(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_SetPolicerEntryOutArgs setPolicerOutArgs;
    CpswAle_SetPolicerEntryInArgs setPolicerInArgs;
    CpswAle_PolicerMatchParams getPolicerInArgs;
    CpswAle_PolicerEntryOutArgs getPolicerOutArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memset(&setPolicerInArgs, 0, sizeof(setPolicerInArgs));
    setPolicerInArgs.policerMatch.policerMatchEnMask = (CPSW_ALE_POLICER_MATCH_PORT |
                                                            CPSW_ALE_POLICER_MATCH_PRIORITY |
                                                            CPSW_ALE_POLICER_MATCH_OUI |
                                                            CPSW_ALE_POLICER_MATCH_MACSRC |
                                                            CPSW_ALE_POLICER_MATCH_MACDST |
                                                            CPSW_ALE_POLICER_MATCH_IVLAN |
                                                            CPSW_ALE_POLICER_MATCH_OVLAN |
                                                            CPSW_ALE_POLICER_MATCH_ETHERTYPE |
                                                            CPSW_ALE_POLICER_MATCH_IPSRC |
                                                            CPSW_ALE_POLICER_MATCH_IPDST);
    setPolicerInArgs.policerMatch.portNum     = 1U;
    setPolicerInArgs.policerMatch.portIsTrunk = BFALSE;
    setPolicerInArgs.policerMatch.priority    = 2U;
    memcpy(&setPolicerInArgs.policerMatch.ouiInfo.ouiAddr[0U], testOuiAddr,
           sizeof(setPolicerInArgs.policerMatch.ouiInfo.ouiAddr));
    memcpy(&setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr[0U], testSrcAddr,
           sizeof(setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr));
    setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.vlanId    = 0U;
    setPolicerInArgs.policerMatch.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_POLICER_INGRESS_PORT);
    memcpy(&setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.addr[0U], testHostPortDestAddr_Policer1,
           sizeof(setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.addr));
    setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.vlanId   = 0U;
    setPolicerInArgs.policerMatch.dstMacAddrInfo.portNum = 0;
    setPolicerInArgs.policerMatch.ivlanId                  = ETH_TEST_IVLAN_ID;
    setPolicerInArgs.policerMatch.ovlanId                  = ETH_TEST_OVLAN_ID;
    setPolicerInArgs.policerMatch.etherType                = ETHERTYPE_EXPERIMENTAL1;
    memcpy(&setPolicerInArgs.policerMatch.srcIpInfo.ipv4Info.ipv4Addr[0U], testSrcIpv4Addr,
           sizeof(setPolicerInArgs.policerMatch.srcIpInfo.ipv4Info.ipv4Addr));
    setPolicerInArgs.policerMatch.srcIpInfo.ipv4Info.numLSBIgnoreBits = 0U;
    setPolicerInArgs.policerMatch.srcIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;
    memcpy(&setPolicerInArgs.policerMatch.dstIpInfo.ipv4Info.ipv4Addr[0U], testDestIpv4Addr,
           sizeof(setPolicerInArgs.policerMatch.dstIpInfo.ipv4Info.ipv4Addr));
    setPolicerInArgs.policerMatch.dstIpInfo.ipv4Info.numLSBIgnoreBits = 0U;
    setPolicerInArgs.policerMatch.dstIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;

    setPolicerInArgs.threadIdEn         = BTRUE;
    setPolicerInArgs.threadId               = 10U;
    setPolicerInArgs.peakRateInBitsPerSec   = RECV_BIT_RATE_PIR_TEST1;
    setPolicerInArgs.commitRateInBitsPerSec = RECV_BIT_RATE_CIR_TEST1;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_POLICER,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("%s: failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",  __func__,
                           status);
    }

    memset(&getPolicerInArgs, 0, sizeof(getPolicerInArgs));
    getPolicerInArgs.policerMatchEnMask = (CPSW_ALE_POLICER_MATCH_PORT |
                                               CPSW_ALE_POLICER_MATCH_PRIORITY |
                                               CPSW_ALE_POLICER_MATCH_OUI |
                                               CPSW_ALE_POLICER_MATCH_MACSRC |
                                               CPSW_ALE_POLICER_MATCH_MACDST |
                                               CPSW_ALE_POLICER_MATCH_IVLAN |
                                               CPSW_ALE_POLICER_MATCH_OVLAN |
                                               CPSW_ALE_POLICER_MATCH_ETHERTYPE |
                                               CPSW_ALE_POLICER_MATCH_IPSRC |
                                               CPSW_ALE_POLICER_MATCH_IPDST);
    getPolicerInArgs.portNum     = 1U;
    getPolicerInArgs.portIsTrunk = BFALSE;
    getPolicerInArgs.priority    = 2U;
    memcpy(&getPolicerInArgs.ouiInfo.ouiAddr[0U], testOuiAddr,
           sizeof(getPolicerInArgs.ouiInfo.ouiAddr));
    memcpy(&getPolicerInArgs.srcMacAddrInfo.addr.addr[0U], testSrcAddr,
           sizeof(getPolicerInArgs.srcMacAddrInfo.addr.addr));
    getPolicerInArgs.srcMacAddrInfo.addr.vlanId    = 0U;
    getPolicerInArgs.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_POLICER_INGRESS_PORT);
    memcpy(&getPolicerInArgs.dstMacAddrInfo.addr.addr[0U], testHostPortDestAddr_Policer1,
           sizeof(getPolicerInArgs.dstMacAddrInfo.addr.addr));
    getPolicerInArgs.dstMacAddrInfo.addr.vlanId   = 0U;
    getPolicerInArgs.dstMacAddrInfo.portNum = 0;
    getPolicerInArgs.ivlanId                  = ETH_TEST_IVLAN_ID;
    getPolicerInArgs.ovlanId                  = ETH_TEST_OVLAN_ID;
    getPolicerInArgs.etherType                = ETHERTYPE_EXPERIMENTAL1;
    memcpy(&getPolicerInArgs.srcIpInfo.ipv4Info.ipv4Addr[0U], testSrcIpv4Addr,
           sizeof(getPolicerInArgs.srcIpInfo.ipv4Info.ipv4Addr));
    getPolicerInArgs.srcIpInfo.ipv4Info.numLSBIgnoreBits = 0U;
    getPolicerInArgs.srcIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;
    memcpy(&getPolicerInArgs.dstIpInfo.ipv4Info.ipv4Addr[0U], testDestIpv4Addr,
           sizeof(getPolicerInArgs.dstIpInfo.ipv4Info.ipv4Addr));
    getPolicerInArgs.dstIpInfo.ipv4Info.numLSBIgnoreBits = 0U;
    getPolicerInArgs.dstIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &getPolicerInArgs, &getPolicerOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_GET_POLICER,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("%s: failed CPSW_ALE_IOCTL_GET_POLICER: %d\n",  __func__,
                           status);
    }
    else
    {
       EnetAppUtils_assert(setPolicerOutArgs.policerEntryIdx ==
                            getPolicerOutArgs.policerEntryIdx);
       EnetAppUtils_assert(setPolicerInArgs.policerMatch.policerMatchEnMask ==
                            getPolicerOutArgs.policerMatchEnMask);
       EnetAppUtils_assert(setPolicerInArgs.policerMatch.portNum ==
                            getPolicerOutArgs.port);
       EnetAppUtils_assert(setPolicerInArgs.policerMatch.portIsTrunk ==
                            getPolicerOutArgs.portIsTrunk);
       EnetAppUtils_assert(setPolicerInArgs.policerMatch.priority ==
                            getPolicerOutArgs.priority);
       EnetAppUtils_assert(setPolicerOutArgs.ouiAleEntryIdx ==
                            getPolicerOutArgs.ouiAleEntryIdx);
       EnetAppUtils_assert(setPolicerOutArgs.srcMacAleEntryIdx ==
                            getPolicerOutArgs.srcMacAleEntryIdx);
       EnetAppUtils_assert(setPolicerOutArgs.dstMacAleEntryIdx ==
                            getPolicerOutArgs.dstMacAleEntryIdx);
       EnetAppUtils_assert(setPolicerOutArgs.ivlanAleEntryIdx ==
                            getPolicerOutArgs.ivlanAleEntryIdx);
       EnetAppUtils_assert(setPolicerOutArgs.ovlanAleEntryIdx ==
                            getPolicerOutArgs.ovlanAleEntryIdx);
       EnetAppUtils_assert(setPolicerOutArgs.etherTypeAleEntryIdx ==
                            getPolicerOutArgs.etherTypeAleEntryIdx);
       EnetAppUtils_assert(setPolicerOutArgs.srcIpAleEntryIdx ==
                            getPolicerOutArgs.srcIpAleEntryIdx);
       EnetAppUtils_assert(setPolicerOutArgs.dstIpAleEntryIdx ==
                            getPolicerOutArgs.dstIpAleEntryIdx);
       EnetAppUtils_assert(setPolicerInArgs.threadIdEn ==
                            getPolicerOutArgs.threadIdEn);
       EnetAppUtils_assert(setPolicerInArgs.threadId ==
                            getPolicerOutArgs.threadId);
        if (setPolicerInArgs.peakRateInBitsPerSec != 0)
        {
           EnetAppUtils_assert((setPolicerInArgs.peakRateInBitsPerSec / getPolicerOutArgs.peakRateInBitsPerSec) == 1);
        }
        else
        {
           EnetAppUtils_assert(getPolicerOutArgs.peakRateInBitsPerSec == 0);
        }

        if (setPolicerInArgs.commitRateInBitsPerSec != 0)
        {
           EnetAppUtils_assert((setPolicerInArgs.commitRateInBitsPerSec / getPolicerOutArgs.commitRateInBitsPerSec) == 1);
        }
        else
        {
           EnetAppUtils_assert(getPolicerOutArgs.commitRateInBitsPerSec == 0);
        }
    }
}

static int32_t EnetTestPolicer_setPolicerAleEntryType1(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_SetPolicerEntryOutArgs setPolicerOutArgs;
    CpswAle_SetPolicerEntryInArgs setPolicerInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memset(&setPolicerInArgs, 0, sizeof(setPolicerInArgs));
    setPolicerInArgs.policerMatch.policerMatchEnMask = (CPSW_ALE_POLICER_MATCH_PORT |
                                                            CPSW_ALE_POLICER_MATCH_MACSRC |
                                                            CPSW_ALE_POLICER_MATCH_MACDST);
    setPolicerInArgs.policerMatch.portNum     = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_POLICER_INGRESS_PORT);
    setPolicerInArgs.policerMatch.portIsTrunk = BFALSE;
    memcpy(&setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr[0U], testSrcAddr,
           sizeof(setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr));
    setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.vlanId    = 0U;
    setPolicerInArgs.policerMatch.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_POLICER_INGRESS_PORT);
    memcpy(&setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.addr[0U], testHostPortDestAddr_Policer1,
           sizeof(setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.addr));
    setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.vlanId   = 0U;
    setPolicerInArgs.policerMatch.dstMacAddrInfo.portNum = CPSW_ALE_HOST_PORT_NUM;
    setPolicerInArgs.threadIdEn                        = BTRUE;
    setPolicerInArgs.threadId                              = taskObj->stateObj.rxFlowObj[ENETTESTPOLICER_TYPE1_RXFLOWCFGID].rxFlowIdx;
    setPolicerInArgs.peakRateInBitsPerSec                  = RECV_BIT_RATE_PIR_TEST1;
    setPolicerInArgs.commitRateInBitsPerSec                = RECV_BIT_RATE_CIR_TEST1;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_POLICER,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_openCpsw() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                           status);
    }

    if (ENET_SOK == status)
    {
        memset(&setPolicerInArgs, 0, sizeof(setPolicerInArgs));
        setPolicerInArgs.policerMatch.policerMatchEnMask = (CPSW_ALE_POLICER_MATCH_PORT |
                                                                CPSW_ALE_POLICER_MATCH_MACSRC |
                                                                CPSW_ALE_POLICER_MATCH_MACDST);
        setPolicerInArgs.policerMatch.portNum     = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_POLICER_INGRESS_PORT);
        setPolicerInArgs.policerMatch.portIsTrunk = BFALSE;
        memcpy(&setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr[0U], testSrcAddr,
               sizeof(setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr));
        setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.vlanId    = 0U;
        setPolicerInArgs.policerMatch.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_POLICER_INGRESS_PORT);
        memcpy(&setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.addr[0U], testEgressPortDestAddr_Policer1,
               sizeof(setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.addr));
        setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.vlanId   = 0U;
        setPolicerInArgs.policerMatch.dstMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_POLICER_EGRESS_PORT);
        setPolicerInArgs.threadIdEn                        = BFALSE;
        setPolicerInArgs.peakRateInBitsPerSec                  = RECV_BIT_RATE_PIR_TEST1;
        setPolicerInArgs.commitRateInBitsPerSec                = RECV_BIT_RATE_CIR_TEST1;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_POLICER,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTest_openCpsw() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                               status);
        }
    }

    return status;
}

static int32_t EnetTestPolicer_setPolicerAleEntryType2(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_SetPolicerEntryOutArgs setPolicerOutArgs;
    CpswAle_SetPolicerEntryInArgs setPolicerInArgs;
    uint32_t setUcastOutArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memset(&setUcastInArgs, 0, sizeof(setUcastInArgs));
    memcpy(&setUcastInArgs.addr.addr[0U], testHostPortDestAddr_Policer2,
           sizeof(setUcastInArgs.addr.addr));
    setUcastInArgs.addr.vlanId  = ETH_TEST_IVLAN_ID;
    setUcastInArgs.info.portNum = CPSW_ALE_HOST_PORT_NUM;
    setUcastInArgs.info.blocked = BFALSE;
    setUcastInArgs.info.secure  = BFALSE;
    setUcastInArgs.info.super   = BFALSE;
    setUcastInArgs.info.ageable = BFALSE;
    setUcastInArgs.info.trunk   = BFALSE;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &setUcastOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_UCAST,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_openCpsw() failed CPSW_ALE_IOCTL_ADD_UCAST: %d\n",
                           status);
    }

    memset(&setUcastInArgs, 0, sizeof(setUcastInArgs));
    memcpy(&setUcastInArgs.addr.addr[0U], testEgressPortDestAddr_Policer2,
           sizeof(setUcastInArgs.addr.addr));
    setUcastInArgs.addr.vlanId  = ETH_TEST_IVLAN_ID;
    setUcastInArgs.info.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_POLICER_EGRESS_PORT);
    setUcastInArgs.info.blocked = BFALSE;
    setUcastInArgs.info.secure  = BFALSE;
    setUcastInArgs.info.super   = BFALSE;
    setUcastInArgs.info.ageable = BFALSE;
    setUcastInArgs.info.trunk   = BFALSE;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &setUcastOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_UCAST,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_openCpsw() failed CPSW_ALE_IOCTL_ADD_UCAST: %d\n",
                           status);
    }

    if (ENET_SOK == status)
    {
        memset(&setPolicerInArgs, 0, sizeof(setPolicerInArgs));
        setPolicerInArgs.policerMatch.policerMatchEnMask = (CPSW_ALE_POLICER_MATCH_IVLAN |
                                                                CPSW_ALE_POLICER_MATCH_ETHERTYPE);
        setPolicerInArgs.policerMatch.ivlanId             = ETH_TEST_IVLAN_ID;
        setPolicerInArgs.policerMatch.etherType           = ETHERTYPE_EXPERIMENTAL1;
        setPolicerInArgs.threadIdEn                       = BTRUE;
        setPolicerInArgs.threadId                         = taskObj->stateObj.rxFlowObj[ENETTESTPOLICER_TYPE2_RXFLOWCFGID].rxFlowIdx;
        setPolicerInArgs.peakRateInBitsPerSec             = RECV_BIT_RATE_PIR_TEST2;
        setPolicerInArgs.commitRateInBitsPerSec           = RECV_BIT_RATE_CIR_TEST2;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_POLICER,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTest_openCpsw() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                               status);
        }
    }

    return status;
}

static int32_t EnetTestPolicer_setPolicerAleEntryType3(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_SetPolicerEntryOutArgs setPolicerOutArgs;
    CpswAle_SetPolicerEntryInArgs setPolicerInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    /* Set Policer entry */
    memset(&setPolicerInArgs, 0, sizeof(setPolicerInArgs));
    setPolicerInArgs.policerMatch.policerMatchEnMask = (CPSW_ALE_POLICER_MATCH_OUI |
                                                            CPSW_ALE_POLICER_MATCH_IPDST);
    memcpy(&setPolicerInArgs.policerMatch.ouiInfo.ouiAddr[0U], testOuiAddr,
           sizeof(setPolicerInArgs.policerMatch.ouiInfo.ouiAddr));
    memcpy(&setPolicerInArgs.policerMatch.dstIpInfo.ipv4Info.ipv4Addr[0U], testDestIpv4Addr,
           sizeof(setPolicerInArgs.policerMatch.dstIpInfo.ipv4Info.ipv4Addr));
    setPolicerInArgs.policerMatch.dstIpInfo.ipv4Info.numLSBIgnoreBits = 0U;
    setPolicerInArgs.policerMatch.dstIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;

    setPolicerInArgs.threadIdEn         = BTRUE;
    setPolicerInArgs.threadId               = taskObj->stateObj.rxFlowObj[ENETTESTPOLICER_TYPE3_RXFLOWCFGID].rxFlowIdx;
    setPolicerInArgs.peakRateInBitsPerSec   = RECV_BIT_RATE_PIR_TEST3;
    setPolicerInArgs.commitRateInBitsPerSec = RECV_BIT_RATE_CIR_TEST3;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_POLICER,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("%s: failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",  __func__,
                           status);
    }

    return status;
}

static int32_t EnetTestPolicer_Test(EnetTestTaskObj *taskObj,
                                    uint32_t rxFlowCfgId,
                                    uint32_t recvbitRatePir,
                                    uint32_t recvbitRateCir)
{
    EnetTestRxFlowObj *rxFlowObj;

    rxFlowObj = &taskObj->stateObj.rxFlowObj[rxFlowCfgId];
    if ((rxFlowObj->receivedBitRate >= recvbitRateCir)
        &&
        (rxFlowObj->receivedBitRate <= (recvbitRatePir * 2)))
    {
        return ENET_SOK;
    }
    else
    {
        /* The bitrate calculation is not correct when done from host so
         * disable returning failure for now
         */
       EnetAppUtils_print("EnetTestPolicer_Test: Rate Limit check fail."
                           "FlowCfgId:%d, PIR:%d, CIR:%d, Measured:%d \n",
                           rxFlowCfgId, recvbitRatePir, recvbitRateCir, rxFlowObj->receivedBitRate);
        return ENET_SOK;
    }
}

void EnetTestPolicer_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                       Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg     = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn          = BFALSE;
}

int32_t EnetTestPolicer_setRxOpenParams(EnetUdma_OpenRxFlowPrms *pRxFlowPrms,
                                        EnetTestTaskObj *taskObj,
                                        uint32_t rxFlowCfgIdx)
{
    pRxFlowPrms->useProxy = BTRUE;

    return ENET_SOK;
}

int32_t EnetTestPolicer_setTxOpenParams(EnetUdma_OpenTxChPrms *pTxChPrms,
                                        EnetTestTaskObj *taskObj,
                                        uint32_t txPSILThreadId,
                                        uint32_t txChCfgIdx)
{
    pTxChPrms->useProxy = BTRUE;

    return ENET_SOK;
}

int32_t EnetTestPolicer_setPolicerRange(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_SetPolicerEntryOutArgs setPolicerOutArgs;
    CpswAle_SetPolicerEntryInArgs setPolicerInArgs;
    CpswAle_PolicerMatchParams getPolicerInArgs;
    CpswAle_PolicerEntryOutArgs getPolicerOutArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t rangeId = 0;

    status = ENET_SOK;
    while ((RECV_BIT_RATE_RANGE_BASE_CIR > (rangeId * RECV_BIT_RATE_RANGE_INTERVAL))
           &&
           (ENET_SOK == status))
    {
        memset(&setPolicerInArgs, 0, sizeof(setPolicerInArgs));
        memset(&setPolicerOutArgs, 0, sizeof(setPolicerOutArgs));
        setPolicerInArgs.policerMatch.policerMatchEnMask = (CPSW_ALE_POLICER_MATCH_IPSRC |
                                                                CPSW_ALE_POLICER_MATCH_MACSRC |
                                                                CPSW_ALE_POLICER_MATCH_MACDST);

        memcpy(&setPolicerInArgs.policerMatch.srcIpInfo.ipv4Info.ipv4Addr[0U], testSrcIpv4AddrRange,
               sizeof(setPolicerInArgs.policerMatch.srcIpInfo.ipv4Info.ipv4Addr));
        setPolicerInArgs.policerMatch.srcIpInfo.ipv4Info.ipv4Addr[3U] += rangeId;
        setPolicerInArgs.policerMatch.srcIpInfo.ipv4Info.numLSBIgnoreBits = 0U;
        setPolicerInArgs.policerMatch.srcIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;

        memcpy(&setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr , testSrcAddrRange,
               sizeof(setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr));
        setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.vlanId = 0U;
        setPolicerInArgs.policerMatch.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_POLICER_INGRESS_PORT);

        memcpy(&setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.addr , testEgressPortDestAddrRange,
               sizeof(setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.addr));
        setPolicerInArgs.policerMatch.dstMacAddrInfo.addr.vlanId = 0U;
        setPolicerInArgs.policerMatch.dstMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_POLICER_EGRESS_PORT);

        setPolicerInArgs.peakRateInBitsPerSec   = (RECV_BIT_RATE_RANGE_BASE_PIR - (rangeId * RECV_BIT_RATE_RANGE_INTERVAL));
        setPolicerInArgs.commitRateInBitsPerSec = (RECV_BIT_RATE_RANGE_BASE_CIR - (rangeId * RECV_BIT_RATE_RANGE_INTERVAL));

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_POLICER,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("%s: failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",  __func__,
                               status);
        }
        if (ENET_SOK == status)
        {
            memset(&getPolicerInArgs, 0, sizeof(getPolicerInArgs));
            memset(&getPolicerOutArgs, 0, sizeof(getPolicerOutArgs));
            getPolicerInArgs.policerMatchEnMask = (CPSW_ALE_POLICER_MATCH_IPSRC |
                                                       CPSW_ALE_POLICER_MATCH_MACSRC |
                                                       CPSW_ALE_POLICER_MATCH_MACDST);
            memcpy(&getPolicerInArgs.srcIpInfo.ipv4Info.ipv4Addr[0U], testSrcIpv4AddrRange,
                   sizeof(getPolicerInArgs.srcIpInfo.ipv4Info.ipv4Addr));
            getPolicerInArgs.srcIpInfo.ipv4Info.ipv4Addr[3U] += rangeId;
            getPolicerInArgs.srcIpInfo.ipv4Info.numLSBIgnoreBits = 0U;
            getPolicerInArgs.srcIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;

            memcpy(&getPolicerInArgs.srcMacAddrInfo.addr.addr , testSrcAddrRange,
                   sizeof(getPolicerInArgs.srcMacAddrInfo.addr.addr));
            getPolicerInArgs.srcMacAddrInfo.addr.vlanId = 0U;
            getPolicerInArgs.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_POLICER_INGRESS_PORT);

            memcpy(&getPolicerInArgs.dstMacAddrInfo.addr.addr , testEgressPortDestAddrRange,
                   sizeof(getPolicerInArgs.dstMacAddrInfo.addr.addr));
            getPolicerInArgs.dstMacAddrInfo.addr.vlanId = 0U;
            getPolicerInArgs.dstMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_POLICER_EGRESS_PORT);

            ENET_IOCTL_SET_INOUT_ARGS(&prms, &getPolicerInArgs, &getPolicerOutArgs);

            status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_GET_POLICER,
                                &prms);
            if (status != ENET_SOK)
            {
               EnetAppUtils_print("%s: failed CPSW_ALE_IOCTL_GET_POLICER: %d\n",  __func__,
                                   status);
            }
            else
            {
               EnetAppUtils_assert(setPolicerOutArgs.policerEntryIdx ==
                                    getPolicerOutArgs.policerEntryIdx);
               EnetAppUtils_assert(setPolicerInArgs.policerMatch.policerMatchEnMask ==
                                    getPolicerOutArgs.policerMatchEnMask);
               EnetAppUtils_assert(setPolicerOutArgs.srcIpAleEntryIdx ==
                                    getPolicerOutArgs.srcIpAleEntryIdx);
               EnetAppUtils_assert(setPolicerOutArgs.srcMacAleEntryIdx ==
                                    getPolicerOutArgs.srcMacAleEntryIdx);
               EnetAppUtils_assert(setPolicerOutArgs.dstMacAleEntryIdx ==
                                    getPolicerOutArgs.dstMacAleEntryIdx);

                if (setPolicerInArgs.peakRateInBitsPerSec != 0)
                {
                   EnetAppUtils_assert((setPolicerInArgs.peakRateInBitsPerSec / getPolicerOutArgs.peakRateInBitsPerSec) == 1);
                }
                else
                {
                   EnetAppUtils_assert(getPolicerOutArgs.peakRateInBitsPerSec == 0);
                }

                if (setPolicerInArgs.commitRateInBitsPerSec != 0)
                {
                   EnetAppUtils_assert((setPolicerInArgs.commitRateInBitsPerSec / getPolicerOutArgs.commitRateInBitsPerSec) == 1);
                }
                else
                {
                   EnetAppUtils_assert(getPolicerOutArgs.commitRateInBitsPerSec == 0);
                }
            }
        }
        rangeId++;
    }
    return status;
}

