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
 * \file     cpsw_test_policer_nomatch.c
 *
 * \brief    This file contains the enet_policer nomatch test implementation.
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
#include "cpsw_test_intervlan.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define ENET_TEST_INTERVLAN_INGRESS_PORT_NUM (ENET_MAC_PORT_4)
#define ENET_TEST_INTERVLAN_EGRESS_PORT_NUM  (ENET_MAC_PORT_3)

#define ENET_TEST_INTERVLAN_INGRESS_VLANID    (100)
#define ENET_TEST_INTERVLAN_EGRESS_VLANID     (200)
#define ENET_TEST_INTERVLAN_HOSTPORT_PVID     (300)
#define ENET_TEST_INTERVLAN_MACPORT_PVID_BASE (400)
#define ENET_TEST_INTERVLAN_INTRA_VLANID      (1000)
#define ENET_TEST_INTERVLAN_INTRA_VLAN_EGRESS_STRIP_ID      (1200)

#define ENET_TEST_INTERVLAN_IPV6_HOP_LIMIT_OFFSET (7)
#define ENET_TEST_INTERVLAN_IPV4_TTL_OFFSET       (8)
#define ENET_TEST_INTERVLAN_IPV6_ETHERTYPE (0x86DD)
#define ENET_TEST_INTERVLAN_IPV4_ETHERTYPE (0x0800)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static int32_t EnetTestInterVlan_setInterVlanMultiEgress(EnetTestTaskObj *taskObj,
                                                         CpswMacPort_InterVlanRouteId expectedAllocRouteId,
                                                         uint32_t *pNumRoutesUsed);

static int32_t EnetTestInterVlan_setInterVlanUniEgress(EnetTestTaskObj *taskObj,
                                                       CpswMacPort_InterVlanRouteId expectedAllocRouteId,
                                                       uint32_t *pNumRoutesUsed);

static int32_t EnetTestInterVlan_setShortIPG(EnetTestTaskObj *taskObj);

static int32_t EnetTestInterVlan_setBlacklistHostportClassifier(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
#define ENET_TEST_IPV6_OCTET2ARRAY(x)  0x00, x
#define ENET_TEST_IPV6_HEXTET2ARRAY(x) (x & 0xFF00) >> 8, (x & 0xFF)

uint8_t testDstIpv6Addr[16] = {ENET_TEST_IPV6_HEXTET2ARRAY(0x2000),
                               ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                               ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                               ENET_TEST_IPV6_OCTET2ARRAY(0x2),
                               ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                               ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                               ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                               ENET_TEST_IPV6_OCTET2ARRAY(0x2)};
uint8_t testDstIpv6Addr2[16] = {ENET_TEST_IPV6_HEXTET2ARRAY(0x2000),
                                ENET_TEST_IPV6_OCTET2ARRAY(0x1),
                                ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                ENET_TEST_IPV6_OCTET2ARRAY(0x4),
                                ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                ENET_TEST_IPV6_OCTET2ARRAY(0x2)};
uint8_t testDstIpv6AddrMcast[] = {ENET_TEST_IPV6_HEXTET2ARRAY(0x2FFF),
                                  ENET_TEST_IPV6_OCTET2ARRAY(0x1),
                                  ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                  ENET_TEST_IPV6_OCTET2ARRAY(0x4),
                                  ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                  ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                  ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                  ENET_TEST_IPV6_OCTET2ARRAY(0x2)};

static uint8_t testDstMacAddr[] = {0x00, 0x11, 0x02, 0x00, 0x00, 0x01};
static uint8_t testDstMcastMacAddr[] = {0x01, 0x00, 0x00, 0x01, 0x02, 0x08};

static uint8_t testHostMacAddr[] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x02};

static uint8_t testSrcIpv6Addr[16] = {ENET_TEST_IPV6_HEXTET2ARRAY(0x2000),
                                      ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                      ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                      ENET_TEST_IPV6_OCTET2ARRAY(0x1),
                                      ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                      ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                      ENET_TEST_IPV6_OCTET2ARRAY(0x0),
                                      ENET_TEST_IPV6_OCTET2ARRAY(0x2)};
static uint8_t testSrcIpv4Addr[4] = {172,
                                     24,
                                     106,
                                     128};

static uint8_t testDstIpv4Addr[4] = {172,
                                     24,
                                     108,
                                     128};

static uint8_t testSrcMacAddr[] = {0x00, 0x11, 0x01, 0x00, 0x00, 0x01};
static uint8_t testBlackListSrcMacAddr[] = {0x00, 0x11, 0x01, 0x00, 0x00, 0x03};

static Enet_MacPort testMultiEgressPortList[] = {ENET_TEST_INTERVLAN_INGRESS_PORT_NUM, ENET_TEST_INTERVLAN_EGRESS_PORT_NUM};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestInterVlan_Run(EnetTestTaskObj *taskObj)
{
    int32_t status              = ENET_SOK;
    uint32_t numRoutesAllocated = 0;
    EnetTestStateObj *stateObj  = &taskObj->stateObj;

    status = EnetTestInterVlan_setShortIPG(taskObj);

    if (ENET_SOK == status)
    {
        status = EnetTestInterVlan_setInterVlanUniEgress(taskObj, CPSW_MACPORT_INTERVLAN_ROUTEID_FIRST, &numRoutesAllocated);
    }

    if (ENET_SOK == status)
    {
        /* EnetTestInterVlan_setInterVlanUniEgress will use two routes */
        status = EnetTestInterVlan_setInterVlanMultiEgress(taskObj, (CpswMacPort_InterVlanRouteId)(CPSW_MACPORT_INTERVLAN_ROUTEID_FIRST + numRoutesAllocated), &numRoutesAllocated);
    }

    if (ENET_SOK == status)
    {
        status = EnetTestInterVlan_setBlacklistHostportClassifier(taskObj);
    }

    if (ENET_SOK == status)
    {
        Enet_IoctlPrms prms;

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_TABLE,
                            &prms);
        EnetAppUtils_assert(status == ENET_SOK);

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_POLICER_ENTRIES,
                            &prms);
        EnetAppUtils_assert(status == ENET_SOK);

#if FIX_LATER
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_IOCTL_PRINT_REGISTERS,
                            &prms);
        EnetAppUtils_assert(status == ENET_SOK);
#endif

        status = EnetTestCommon_Run(taskObj);
    }

    return status;
}

void EnetTestInterVlan_setOpenPrms(EnetTestTaskObj *taskObj,
                                   Cpsw_Cfg *pCpswCfg,
                                   EnetUtils_Cfg *pUtilsPrms)
{
    Enet_MacPort i;

    /* pCpswCfg->aleCfg.policerGlobalCfg.policingEn SHOULD BE TRUE for interVLan.
     * Set to FALSE to exercise driver internal logic to auto enable policer when interVLan API
     * is invoked
     */
    pCpswCfg->aleCfg.policerGlobalCfg.policingEn = BFALSE;

    pCpswCfg->aleCfg.policerGlobalCfg.redDropEn      = BFALSE;
    pCpswCfg->aleCfg.policerGlobalCfg.yellowDropEn   = BFALSE;
    pCpswCfg->aleCfg.policerGlobalCfg.policerNoMatchMode = CPSW_ALE_POLICER_NOMATCH_MODE_GREEN;
    pCpswCfg->aleCfg.vlanCfg.aleVlanAwareMode            = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.cpswVlanAwareMode           = BTRUE;
    pCpswCfg->aleCfg.nwSecCfg.vid0ModeEn                 = BTRUE;

    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].learningCfg.noLearn              = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].vlanCfg.dropUntagged             = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.unregMcastFloodMask      = 0x0;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.regMcastFloodMask        = CPSW_ALE_ALL_PORTS_MASK;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.forceUntaggedEgressMask  = 0U;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.noLearnMask              = 0x0;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vidIngressCheck          = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.limitIPNxtHdr            = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.disallowIPFrag           = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vlanIdInfo.vlanId        = ENET_TEST_INTERVLAN_HOSTPORT_PVID;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vlanMemberList           = CPSW_ALE_ALL_PORTS_MASK;

    for (i = ENET_MAC_PORT_FIRST; i < CPSW_ALE_NUM_MAC_PORTS; i++)
    {
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].learningCfg.noLearn              = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].vlanCfg.dropUntagged             = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.unregMcastFloodMask      = 0x0;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.regMcastFloodMask        = CPSW_ALE_ALL_PORTS_MASK;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.forceUntaggedEgressMask  = 0U;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.noLearnMask              = 0x0;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vidIngressCheck          = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.limitIPNxtHdr            = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.disallowIPFrag           = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vlanIdInfo.vlanId        = ENET_TEST_INTERVLAN_MACPORT_PVID_BASE + ENET_MACPORT_NORM(i);
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vlanMemberList           = CPSW_ALE_ALL_PORTS_MASK;
    }

    pCpswCfg->hostPortCfg.vlanCfg.portPri = 7U;
    pCpswCfg->hostPortCfg.vlanCfg.portCfi = 0U;
    pCpswCfg->hostPortCfg.vlanCfg.portVID = ENET_TEST_INTERVLAN_HOSTPORT_PVID;
    pCpswCfg->vlanCfg.vlanAware           = BTRUE;
}

void EnetTestInterVlan_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                         Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg      = (CpswMacPort_Cfg*)pLinkArgs->macCfg;

    macCfg->loopbackEn = BFALSE;
    macCfg->vlanCfg.portPri = ENET_MACPORT_NORM(portNum);
    macCfg->vlanCfg.portCfi = 0U;
    macCfg->vlanCfg.portVID = ENET_TEST_INTERVLAN_MACPORT_PVID_BASE + ENET_MACPORT_NORM(portNum);
}

static uint32_t EnetTestInterVlan_getIngressVlanMembershipMask(void)
{
    uint32_t memberShipMask;

    memberShipMask =
        (1 << CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_INGRESS_PORT_NUM));
    memberShipMask |= CPSW_ALE_HOST_PORT_MASK;
    return memberShipMask;
}

static uint32_t EnetTestInterVlan_getEgressVlanMembershipMask(void)
{
    uint32_t memberShipMask;

    memberShipMask =
        (1 << CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_EGRESS_PORT_NUM));
    memberShipMask |= CPSW_ALE_HOST_PORT_MASK;
    return memberShipMask;
}

static uint32_t EnetTestInterVlan_getIntraVlanMembershipMask(void)
{
    uint32_t memberShipMask;

    memberShipMask =
        (1 << CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_INGRESS_PORT_NUM));
    memberShipMask |=
        (1 << CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_EGRESS_PORT_NUM));
    memberShipMask |= CPSW_ALE_HOST_PORT_MASK;
    return memberShipMask;
}

static int32_t EnetTestInterVlan_addUniEgressAleTableEntries(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setUcastOutArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memcpy(&setUcastInArgs.addr.addr[0U], testHostMacAddr,
           sizeof(setUcastInArgs.addr.addr));
    setUcastInArgs.addr.vlanId  = 0U;
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
       EnetAppUtils_print("%s() failed CPSW_ALE_IOCTL_ADD_UCAST: %d\n",
                           __func__, status);
    }

    if (status == ENET_SOK)
    {
        CpswAle_VlanEntryInfo inArgs;
        uint32_t outArgs;

        inArgs.vlanIdInfo.vlanId        = ENET_TEST_INTERVLAN_INGRESS_VLANID;
        inArgs.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
        inArgs.vlanMemberList           = EnetTestInterVlan_getIngressVlanMembershipMask();
        inArgs.unregMcastFloodMask      = EnetTestInterVlan_getIngressVlanMembershipMask();
        inArgs.regMcastFloodMask        = EnetTestInterVlan_getIngressVlanMembershipMask();
        inArgs.forceUntaggedEgressMask  = 0U;
        inArgs.noLearnMask              = 0U;
        inArgs.vidIngressCheck          = BFALSE;
        inArgs.limitIPNxtHdr            = BFALSE;
        inArgs.disallowIPFrag           = BFALSE;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_VLAN, &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("%s() failed ADD_VLAN ioctl failed: %d\n",
                               __func__, status);
        }
    }

    if (status == ENET_SOK)
    {
        CpswAle_VlanEntryInfo inArgs;
        uint32_t outArgs;

        inArgs.vlanIdInfo.vlanId        = ENET_TEST_INTERVLAN_EGRESS_VLANID;
        inArgs.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
        inArgs.vlanMemberList           = EnetTestInterVlan_getEgressVlanMembershipMask();
        inArgs.unregMcastFloodMask      = EnetTestInterVlan_getEgressVlanMembershipMask();
        inArgs.regMcastFloodMask        = EnetTestInterVlan_getEgressVlanMembershipMask();
        inArgs.forceUntaggedEgressMask  = 0U;
        inArgs.noLearnMask              = 0U;
        inArgs.vidIngressCheck          = BFALSE;
        inArgs.limitIPNxtHdr            = BFALSE;
        inArgs.disallowIPFrag           = BFALSE;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_VLAN, &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("%s() failed ADD_VLAN ioctl failed: %d\n",
                               __func__, status);
        }
    }

    return status;
}

static int32_t EnetTestInterVlan_setInterVlanUniEgress(EnetTestTaskObj *taskObj,
                                                       CpswMacPort_InterVlanRouteId expectedAllocRouteId,
                                                       uint32_t *pNumRoutesUsed)
{
    int32_t status;
    Enet_IoctlPrms prms;
    Cpsw_SetInterVlanRouteUniEgressInArgs inArgs;
    Cpsw_SetInterVlanRouteUniEgressOutArgs outArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    *pNumRoutesUsed = 0;
    status          = EnetTestInterVlan_addUniEgressAleTableEntries(taskObj);

    if (ENET_SOK == status)
    {
        /* Set to invalid id and confirm outArgs populated correctly after IOCTL
         * called
         */
        outArgs.egressPortRouteId = CPSW_MACPORT_INTERVLAN_ROUTEID_LAST;

        inArgs.inPktMatchCfg.packetMatchEnMask = 0;

        inArgs.inPktMatchCfg.ingressPort            = ENET_TEST_INTERVLAN_INGRESS_PORT_NUM;
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_PORT;

        inArgs.inPktMatchCfg.dstIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV6;
        inArgs.inPktMatchCfg.dstIpInfo.ipv6Info.numLSBIgnoreBits = 8U;
        memcpy(inArgs.inPktMatchCfg.dstIpInfo.ipv6Info.ipv6Addr, testDstIpv6Addr, sizeof(inArgs.inPktMatchCfg.dstIpInfo.ipv6Info.ipv6Addr));
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_IPDST;

        memcpy(inArgs.inPktMatchCfg.dstMacAddrInfo.addr.addr, testHostMacAddr, sizeof(inArgs.inPktMatchCfg.dstMacAddrInfo.addr.addr));
        inArgs.inPktMatchCfg.dstMacAddrInfo.addr.vlanId   = 0U;
        inArgs.inPktMatchCfg.dstMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_EGRESS_PORT_NUM);

        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_MACDST;

        inArgs.inPktMatchCfg.srcIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV6;
        inArgs.inPktMatchCfg.srcIpInfo.ipv6Info.numLSBIgnoreBits = 8U;
        memcpy(inArgs.inPktMatchCfg.srcIpInfo.ipv6Info.ipv6Addr, testSrcIpv6Addr, sizeof(inArgs.inPktMatchCfg.srcIpInfo.ipv6Info.ipv6Addr));
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_IPSRC;

        memcpy(inArgs.inPktMatchCfg.srcMacAddrInfo.addr.addr, testSrcMacAddr, sizeof(inArgs.inPktMatchCfg.srcMacAddrInfo.addr.addr));
        inArgs.inPktMatchCfg.srcMacAddrInfo.addr.vlanId    = 0U;
        inArgs.inPktMatchCfg.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_INGRESS_PORT_NUM);
        inArgs.inPktMatchCfg.packetMatchEnMask    |= CPSW_INTERVLAN_INGRESSPKT_MATCH_MACSRC;

        inArgs.inPktMatchCfg.etherType    = ENET_TEST_INTERVLAN_IPV6_ETHERTYPE;
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_ETHERTYPE;

        inArgs.inPktMatchCfg.vlanId         = ENET_TEST_INTERVLAN_INGRESS_VLANID;
        inArgs.inPktMatchCfg.ttlCheckEn = BTRUE;

        inArgs.egressCfg.egressPort                       = ENET_TEST_INTERVLAN_EGRESS_PORT_NUM;
        inArgs.egressCfg.outPktModCfg.decrementTTL        = BTRUE;
        inArgs.egressCfg.outPktModCfg.forceUntaggedEgress = BFALSE;
        inArgs.egressCfg.outPktModCfg.replaceDASA         = BTRUE;
        memcpy(inArgs.egressCfg.outPktModCfg.srcAddr, testHostMacAddr, sizeof(inArgs.egressCfg.outPktModCfg.srcAddr));
        memcpy(inArgs.egressCfg.outPktModCfg.dstAddr, testDstMacAddr, sizeof(inArgs.egressCfg.outPktModCfg.dstAddr));
        inArgs.egressCfg.outPktModCfg.vlanId = ENET_TEST_INTERVLAN_EGRESS_VLANID;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_PER_IOCTL_SET_INTERVLAN_ROUTE_UNI_EGRESS,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestInterVlan_setInterVlanUniEgress() failed CPSW_PER_IOCTL_CLEAR_INTERVLAN_ROUTE_UNI_EGRESS: %d\n",
                               status);
        }
    }

    if (status == ENET_SOK)
    {
        *pNumRoutesUsed += 1;
       EnetAppUtils_assert(outArgs.egressPortRouteId == expectedAllocRouteId);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.port == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_INGRESS_PORT_NUM));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.portIsTrunk == BFALSE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.egressOpEn == BTRUE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.egressOpcode == (1 + (outArgs.egressPortRouteId - CPSW_MACPORT_INTERVLAN_ROUTEID_FIRST)));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.ttlCheckEn == BTRUE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.dstPortMask == (1 << CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_EGRESS_PORT_NUM)));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.policerMatchEnMask == (CPSW_ALE_POLICER_MATCH_PORT |
                                                                                           CPSW_ALE_POLICER_MATCH_MACSRC |
                                                                                           CPSW_ALE_POLICER_MATCH_MACDST |
                                                                                           CPSW_ALE_POLICER_MATCH_IVLAN |
                                                                                           CPSW_ALE_POLICER_MATCH_ETHERTYPE |
                                                                                           CPSW_ALE_POLICER_MATCH_IPSRC |
                                                                                           CPSW_ALE_POLICER_MATCH_IPDST));
    }

    if (status == ENET_SOK)
    {
        /* Add another route to send packet out on ingress port.
         * Only destIP changes. Rest of params remain same
         */
        inArgs.inPktMatchCfg.dstIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV6;
        inArgs.inPktMatchCfg.dstIpInfo.ipv6Info.numLSBIgnoreBits = 8U;
        memcpy(inArgs.inPktMatchCfg.dstIpInfo.ipv6Info.ipv6Addr, testDstIpv6Addr2, sizeof(inArgs.inPktMatchCfg.dstIpInfo.ipv6Info.ipv6Addr));
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_IPDST;

        inArgs.egressCfg.egressPort = ENET_TEST_INTERVLAN_INGRESS_PORT_NUM;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_PER_IOCTL_CLEAR_INTERVLAN_ROUTE_UNI_EGRESS,
                            &prms);

        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestInterVlan_setInterVlanUniEgress() failed CPSW_PER_IOCTL_CLEAR_INTERVLAN_ROUTE_UNI_EGRESS: %d\n",
                               status);
        }
    }

    if (status == ENET_SOK)
    {
        /* Do not increment numRoutesUsed as it is a different egress port */
       EnetAppUtils_assert(outArgs.egressPortRouteId == expectedAllocRouteId);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.port == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_INGRESS_PORT_NUM));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.portIsTrunk == BFALSE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.egressOpEn == BTRUE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.egressOpcode == (1 + (outArgs.egressPortRouteId - CPSW_MACPORT_INTERVLAN_ROUTEID_FIRST)));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.ttlCheckEn == BTRUE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.dstPortMask == (1 << CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_INGRESS_PORT_NUM)));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.policerMatchEnMask == (CPSW_ALE_POLICER_MATCH_PORT |
                                                                                           CPSW_ALE_POLICER_MATCH_MACSRC |
                                                                                           CPSW_ALE_POLICER_MATCH_MACDST |
                                                                                           CPSW_ALE_POLICER_MATCH_IVLAN |
                                                                                           CPSW_ALE_POLICER_MATCH_ETHERTYPE |
                                                                                           CPSW_ALE_POLICER_MATCH_IPSRC |
                                                                                           CPSW_ALE_POLICER_MATCH_IPDST));
    }

    /* Add a route in opposite direction to enable bidirectional intervlan switching */
    if (ENET_SOK == status)
    {
        /* Set to invalid id and confirm outArgs populated correctly after IOCTL
         * called
         */
        outArgs.egressPortRouteId = CPSW_MACPORT_INTERVLAN_ROUTEID_LAST;

        inArgs.inPktMatchCfg.packetMatchEnMask = 0;

        inArgs.inPktMatchCfg.ingressPort            = ENET_TEST_INTERVLAN_EGRESS_PORT_NUM;
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_PORT;

        inArgs.inPktMatchCfg.dstIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV6;
        inArgs.inPktMatchCfg.dstIpInfo.ipv6Info.numLSBIgnoreBits = 8U;
        memcpy(inArgs.inPktMatchCfg.dstIpInfo.ipv6Info.ipv6Addr, testSrcIpv6Addr, sizeof(inArgs.inPktMatchCfg.dstIpInfo.ipv6Info.ipv6Addr));
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_IPDST;

        memcpy(inArgs.inPktMatchCfg.dstMacAddrInfo.addr.addr, testHostMacAddr, sizeof(inArgs.inPktMatchCfg.dstMacAddrInfo.addr.addr));
        inArgs.inPktMatchCfg.dstMacAddrInfo.addr.vlanId   = 0U;
        inArgs.inPktMatchCfg.dstMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_EGRESS_PORT_NUM);

        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_MACDST;

        inArgs.inPktMatchCfg.srcIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV6;
        inArgs.inPktMatchCfg.srcIpInfo.ipv6Info.numLSBIgnoreBits = 8U;
        memcpy(inArgs.inPktMatchCfg.srcIpInfo.ipv6Info.ipv6Addr, testDstIpv6Addr, sizeof(inArgs.inPktMatchCfg.srcIpInfo.ipv6Info.ipv6Addr));
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_IPSRC;

        memcpy(inArgs.inPktMatchCfg.srcMacAddrInfo.addr.addr, testDstMacAddr, sizeof(inArgs.inPktMatchCfg.srcMacAddrInfo.addr.addr));
        inArgs.inPktMatchCfg.srcMacAddrInfo.addr.vlanId    = 0U;
        inArgs.inPktMatchCfg.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_EGRESS_PORT_NUM);
        inArgs.inPktMatchCfg.packetMatchEnMask    |= CPSW_INTERVLAN_INGRESSPKT_MATCH_MACSRC;

        inArgs.inPktMatchCfg.etherType    = 0x86DD;
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_ETHERTYPE;

        inArgs.inPktMatchCfg.vlanId         = ENET_TEST_INTERVLAN_EGRESS_VLANID;
        inArgs.inPktMatchCfg.ttlCheckEn = BTRUE;

        inArgs.egressCfg.egressPort                       = ENET_TEST_INTERVLAN_INGRESS_PORT_NUM;
        inArgs.egressCfg.outPktModCfg.decrementTTL        = BTRUE;
        inArgs.egressCfg.outPktModCfg.forceUntaggedEgress = BFALSE;
        inArgs.egressCfg.outPktModCfg.replaceDASA         = BTRUE;
        memcpy(inArgs.egressCfg.outPktModCfg.srcAddr, testHostMacAddr, sizeof(inArgs.egressCfg.outPktModCfg.srcAddr));
        memcpy(inArgs.egressCfg.outPktModCfg.dstAddr, testSrcMacAddr, sizeof(inArgs.egressCfg.outPktModCfg.dstAddr));
        inArgs.egressCfg.outPktModCfg.vlanId = ENET_TEST_INTERVLAN_INGRESS_VLANID;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_PER_IOCTL_CLEAR_INTERVLAN_ROUTE_UNI_EGRESS,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestInterVlan_setInterVlanUniEgress() failed CPSW_PER_IOCTL_CLEAR_INTERVLAN_ROUTE_UNI_EGRESS: %d\n",
                               status);
        }
    }

    if (status == ENET_SOK)
    {
       EnetAppUtils_assert(outArgs.egressPortRouteId == (expectedAllocRouteId + 1));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.port == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_EGRESS_PORT_NUM));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.portIsTrunk == BFALSE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.egressOpEn == BTRUE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.egressOpcode == (1 + (outArgs.egressPortRouteId - CPSW_MACPORT_INTERVLAN_ROUTEID_FIRST)));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.ttlCheckEn == BTRUE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.dstPortMask == (1 << CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_INGRESS_PORT_NUM)));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.policerMatchEnMask == (CPSW_ALE_POLICER_MATCH_PORT |
                                                                                           CPSW_ALE_POLICER_MATCH_MACSRC |
                                                                                           CPSW_ALE_POLICER_MATCH_MACDST |
                                                                                           CPSW_ALE_POLICER_MATCH_IVLAN |
                                                                                           CPSW_ALE_POLICER_MATCH_ETHERTYPE |
                                                                                           CPSW_ALE_POLICER_MATCH_IPSRC |
                                                                                           CPSW_ALE_POLICER_MATCH_IPDST));
    }

    /* Add IPv4 intervlan switching route */
    if (ENET_SOK == status)
    {
        /* Set to invalid id and confirm outArgs populated correctly after IOCTL
         * called
         */
        outArgs.egressPortRouteId = CPSW_MACPORT_INTERVLAN_ROUTEID_LAST;

        inArgs.inPktMatchCfg.packetMatchEnMask = 0;

        inArgs.inPktMatchCfg.ingressPort            = ENET_TEST_INTERVLAN_INGRESS_PORT_NUM;
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_PORT;

        inArgs.inPktMatchCfg.srcIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;
        inArgs.inPktMatchCfg.srcIpInfo.ipv4Info.numLSBIgnoreBits = 8U;
        memcpy(inArgs.inPktMatchCfg.srcIpInfo.ipv4Info.ipv4Addr, testSrcIpv4Addr, sizeof(inArgs.inPktMatchCfg.srcIpInfo.ipv4Info.ipv4Addr));
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_IPSRC;

        inArgs.inPktMatchCfg.dstIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;
        inArgs.inPktMatchCfg.dstIpInfo.ipv4Info.numLSBIgnoreBits = 8U;
        memcpy(inArgs.inPktMatchCfg.dstIpInfo.ipv4Info.ipv4Addr, testDstIpv4Addr, sizeof(inArgs.inPktMatchCfg.dstIpInfo.ipv4Info.ipv4Addr));
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_IPDST;

        memcpy(inArgs.inPktMatchCfg.dstMacAddrInfo.addr.addr, testHostMacAddr, sizeof(inArgs.inPktMatchCfg.dstMacAddrInfo.addr.addr));
        inArgs.inPktMatchCfg.dstMacAddrInfo.addr.vlanId   = 0U;
        inArgs.inPktMatchCfg.dstMacAddrInfo.portNum = CPSW_ALE_HOST_PORT_NUM;

        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_MACDST;

        memcpy(inArgs.inPktMatchCfg.srcMacAddrInfo.addr.addr, testSrcMacAddr, sizeof(inArgs.inPktMatchCfg.srcMacAddrInfo.addr.addr));
        inArgs.inPktMatchCfg.srcMacAddrInfo.addr.vlanId    = 0U;
        inArgs.inPktMatchCfg.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_INGRESS_PORT_NUM);
        inArgs.inPktMatchCfg.packetMatchEnMask    |= CPSW_INTERVLAN_INGRESSPKT_MATCH_MACSRC;

        inArgs.inPktMatchCfg.vlanId         = ENET_TEST_INTERVLAN_INGRESS_VLANID;
        inArgs.inPktMatchCfg.ttlCheckEn = BTRUE;

        inArgs.egressCfg.egressPort                       = ENET_TEST_INTERVLAN_EGRESS_PORT_NUM;
        inArgs.egressCfg.outPktModCfg.decrementTTL        = BTRUE;
        inArgs.egressCfg.outPktModCfg.forceUntaggedEgress = BTRUE;
        inArgs.egressCfg.outPktModCfg.replaceDASA         = BTRUE;
        memcpy(inArgs.egressCfg.outPktModCfg.srcAddr, testHostMacAddr, sizeof(inArgs.egressCfg.outPktModCfg.srcAddr));
        memcpy(inArgs.egressCfg.outPktModCfg.dstAddr, testDstMacAddr, sizeof(inArgs.egressCfg.outPktModCfg.dstAddr));
        inArgs.egressCfg.outPktModCfg.vlanId = 0;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_PER_IOCTL_CLEAR_INTERVLAN_ROUTE_UNI_EGRESS,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestInterVlan_setInterVlanUniEgress() failed CPSW_PER_IOCTL_CLEAR_INTERVLAN_ROUTE_UNI_EGRESS: %d\n",
                               status);
        }
    }

    if (status == ENET_SOK)
    {
        *pNumRoutesUsed += 1;
       EnetAppUtils_assert(outArgs.egressPortRouteId == (expectedAllocRouteId + 1));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.port == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_INGRESS_PORT_NUM));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.portIsTrunk == BFALSE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.egressOpEn == BTRUE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.egressOpcode == (1 + (outArgs.egressPortRouteId - CPSW_MACPORT_INTERVLAN_ROUTEID_FIRST)));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.ttlCheckEn == BTRUE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.dstPortMask == (1 << CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_EGRESS_PORT_NUM)));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.policerMatchEnMask == (CPSW_ALE_POLICER_MATCH_PORT |
                                                                                           CPSW_ALE_POLICER_MATCH_MACSRC |
                                                                                           CPSW_ALE_POLICER_MATCH_MACDST |
                                                                                           CPSW_ALE_POLICER_MATCH_IVLAN |
                                                                                           CPSW_ALE_POLICER_MATCH_IPSRC |
                                                                                           CPSW_ALE_POLICER_MATCH_IPDST));
    }

    return status;
}

static uint32_t EnetTestInterVlan_getMultiEgressDestPortMask(void)
{
    uint32_t i;
    uint32_t dstPortMask;

    dstPortMask = 0;
    for (i = 0; i < ENET_ARRAYSIZE(testMultiEgressPortList); i++)
    {
        dstPortMask |= (1 << CPSW_ALE_MACPORT_TO_ALEPORT(testMultiEgressPortList[i]));
    }

    return dstPortMask;
}

static int32_t EnetTestInterVlan_addMultiEgressAleTableEntries(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setUcastOutArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    CpswAle_VlanEntryInfo inArgs;

    memcpy(&setUcastInArgs.addr.addr[0U], testHostMacAddr,
           sizeof(setUcastInArgs.addr.addr));
    setUcastInArgs.addr.vlanId  = 0U;
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
       EnetAppUtils_print("%s() failed CPSW_ALE_IOCTL_ADD_UCAST: %d\n",
                           __func__, status);
    }

    if (status == ENET_SOK)
    {
        uint32_t outArgs;

        inArgs.vlanIdInfo.vlanId        = ENET_TEST_INTERVLAN_INGRESS_VLANID;
        inArgs.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
        inArgs.vlanMemberList           = EnetTestInterVlan_getIngressVlanMembershipMask();
        inArgs.unregMcastFloodMask      = EnetTestInterVlan_getIngressVlanMembershipMask();
        inArgs.regMcastFloodMask        = EnetTestInterVlan_getIngressVlanMembershipMask();
        inArgs.forceUntaggedEgressMask  = 0U;
        inArgs.noLearnMask              = 0U;
        inArgs.vidIngressCheck          = BFALSE;
        inArgs.limitIPNxtHdr            = BFALSE;
        inArgs.disallowIPFrag           = BFALSE;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_VLAN, &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("%s() failed ADD_VLAN ioctl failed: %d\n",
                               __func__, status);
        }
    }

    if (ENET_SOK == status)
    {
        status = EnetTestCommon_setAleMulticastEntry(taskObj,
                                                     testDstMcastMacAddr,
                                                     0,
                                                     0,
                                                     EnetTestInterVlan_getIngressVlanMembershipMask());
    }

    return status;
}

static int32_t EnetTestInterVlan_setInterVlanMultiEgress(EnetTestTaskObj *taskObj,
                                                         CpswMacPort_InterVlanRouteId expectedAllocRouteId,
                                                         uint32_t *pNumRoutesUsed)
{
    int32_t status;
    Enet_IoctlPrms prms;
    Cpsw_SetInterVlanRouteMultiEgressInArgs inArgs;
    Cpsw_SetInterVlanRouteMultiEgressOutArgs outArgs;
    uint32_t i;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    *pNumRoutesUsed = 0;
    status          = EnetTestInterVlan_addMultiEgressAleTableEntries(taskObj);

    if (ENET_SOK == status)
    {
        inArgs.inPktMatchCfg.packetMatchEnMask = 0;

        inArgs.inPktMatchCfg.ingressPort            = ENET_TEST_INTERVLAN_INGRESS_PORT_NUM;
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_PORT;

        inArgs.inPktMatchCfg.dstIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV6;
        inArgs.inPktMatchCfg.dstIpInfo.ipv6Info.numLSBIgnoreBits = 8U;
        memcpy(inArgs.inPktMatchCfg.dstIpInfo.ipv6Info.ipv6Addr, testDstIpv6AddrMcast, sizeof(inArgs.inPktMatchCfg.dstIpInfo.ipv6Info.ipv6Addr));
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_IPDST;

        memcpy(inArgs.inPktMatchCfg.dstMacAddrInfo.addr.addr, testDstMcastMacAddr, sizeof(inArgs.inPktMatchCfg.dstMacAddrInfo.addr.addr));
        inArgs.inPktMatchCfg.dstMacAddrInfo.addr.vlanId   = 0U;
        inArgs.inPktMatchCfg.dstMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_EGRESS_PORT_NUM);
        inArgs.inPktMatchCfg.packetMatchEnMask   |= CPSW_INTERVLAN_INGRESSPKT_MATCH_MACDST;

        inArgs.inPktMatchCfg.srcIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV6;
        inArgs.inPktMatchCfg.srcIpInfo.ipv6Info.numLSBIgnoreBits = 8U;
        memcpy(inArgs.inPktMatchCfg.srcIpInfo.ipv6Info.ipv6Addr, testSrcIpv6Addr, sizeof(inArgs.inPktMatchCfg.srcIpInfo.ipv6Info.ipv6Addr));
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_IPSRC;

        memcpy(inArgs.inPktMatchCfg.srcMacAddrInfo.addr.addr, testSrcMacAddr, sizeof(inArgs.inPktMatchCfg.srcMacAddrInfo.addr.addr));
        inArgs.inPktMatchCfg.srcMacAddrInfo.addr.vlanId    = 0U;
        inArgs.inPktMatchCfg.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_INGRESS_PORT_NUM);

        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_MACSRC;

        inArgs.inPktMatchCfg.etherType    = 0x86DD;
        inArgs.inPktMatchCfg.packetMatchEnMask |= CPSW_INTERVLAN_INGRESSPKT_MATCH_ETHERTYPE;

        inArgs.inPktMatchCfg.vlanId         = ENET_TEST_INTERVLAN_INGRESS_VLANID;
        inArgs.inPktMatchCfg.ttlCheckEn = BTRUE;

        inArgs.numEgressPorts = ENET_ARRAYSIZE(testMultiEgressPortList);

        for (i = 0; i < inArgs.numEgressPorts; i++)
        {
            inArgs.egressCfg[i].egressPort                       = testMultiEgressPortList[i];
            inArgs.egressCfg[i].outPktModCfg.decrementTTL        = BTRUE;
            inArgs.egressCfg[i].outPktModCfg.forceUntaggedEgress = BFALSE;
            inArgs.egressCfg[i].outPktModCfg.replaceDASA         = BTRUE;
            memcpy(inArgs.egressCfg[i].outPktModCfg.srcAddr, testHostMacAddr, sizeof(inArgs.egressCfg[i].outPktModCfg.srcAddr));
            memcpy(inArgs.egressCfg[i].outPktModCfg.dstAddr, testDstMcastMacAddr, sizeof(inArgs.egressCfg[i].outPktModCfg.dstAddr));
            inArgs.egressCfg[i].outPktModCfg.vlanId = ENET_TEST_INTERVLAN_EGRESS_VLANID + 1 + i;
        }

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_PER_IOCTL_SET_INTERVLAN_ROUTE_MULTI_EGRESS,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestInterVlan_setInterVlanUniEgress() failed CPSW_PER_IOCTL_CLEAR_INTERVLAN_ROUTE_UNI_EGRESS: %d\n",
                               status);
        }
    }

    if (status == ENET_SOK)
    {
        *pNumRoutesUsed += 1;
       EnetAppUtils_assert(outArgs.egressPortRouteId == expectedAllocRouteId);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.port == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_INGRESS_PORT_NUM));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.portIsTrunk == BFALSE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.egressOpEn == BTRUE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.egressOpcode == (1 + (outArgs.egressPortRouteId - CPSW_MACPORT_INTERVLAN_ROUTEID_FIRST)));
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.ttlCheckEn == BTRUE);
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.dstPortMask == EnetTestInterVlan_getMultiEgressDestPortMask());
       EnetAppUtils_assert(outArgs.ingressPacketClassifierInfo.policerMatchEnMask == (CPSW_ALE_POLICER_MATCH_PORT |
                                                                                           CPSW_ALE_POLICER_MATCH_MACSRC |
                                                                                           CPSW_ALE_POLICER_MATCH_MACDST |
                                                                                           CPSW_ALE_POLICER_MATCH_IVLAN |
                                                                                           CPSW_ALE_POLICER_MATCH_ETHERTYPE |
                                                                                           CPSW_ALE_POLICER_MATCH_IPSRC |
                                                                                           CPSW_ALE_POLICER_MATCH_IPDST));
    }

    return status;
}

volatile static uint32_t gEnetTestDumpPolicer = UFALSE;
uint32_t gEnetTestInterVlanTTL[1000];
static uint32_t gEnetTestInterVlanIndex = 0;

int32_t EnetTestInterVlan_processRxPkt(EnetTestTaskObj *taskObj,
                                       uint32_t rxFlowId,
                                       uint32_t pktNum,
                                       EnetDma_Pkt *pktInfo,
                                       bool *testComplete)
{
    int32_t status = ENET_SOK;
    EthVlanFrame *frame;

    EnetAppUtils_assert(pktInfo->sgList.numScatterSegments == 1U);
    frame = (EthVlanFrame *)pktInfo->sgList.list[0].bufPtr;
    /* Check for blacklist classifier not being received on host port */
    if (memcmp(&frame->hdr.dstMac[0], testHostMacAddr, sizeof(frame->hdr.dstMac)) == 0)
    {
        if ((Enet_ntohs(frame->hdr.tci) & 0xFFFU) == ENET_TEST_INTERVLAN_INTRA_VLANID)
        {
            /* If IPv4 packet */
            if (Enet_ntohs(frame->hdr.etherType) == ENET_TEST_INTERVLAN_IPV4_ETHERTYPE)
            {
                if (memcmp(&frame->hdr.srcMac[0], testBlackListSrcMacAddr, sizeof(frame->hdr.srcMac)) == 0)
                {
                    /* We should not get blacklisted classifier on host port */
                    status = ENET_EFAIL;
                }
            }
        }
    }

    if (status == ENET_SOK)
    {
        if ((memcmp(&frame->hdr.dstMac[0], testHostMacAddr, sizeof(frame->hdr.dstMac)) == 0)
            ||
            (memcmp(&frame->hdr.dstMac[0], testDstMcastMacAddr, sizeof(frame->hdr.dstMac)) == 0))
        {
            if (((Enet_ntohs(frame->hdr.tci) & 0xFFFU) == ENET_TEST_INTERVLAN_INGRESS_VLANID)
                ||
                ((Enet_ntohs(frame->hdr.tci) & 0xFFFU) == ENET_TEST_INTERVLAN_EGRESS_VLANID))
            {
                if (Enet_ntohs(frame->hdr.etherType) == ENET_TEST_INTERVLAN_IPV6_ETHERTYPE)
                {
                    if (frame->payload[ENET_TEST_INTERVLAN_IPV6_HOP_LIMIT_OFFSET] <= 1)
                    {
                        status = ENET_SOK;
                    }
                    else
                    {
                        gEnetTestInterVlanIndex                        = (gEnetTestInterVlanIndex + 1) % ENET_ARRAYSIZE(gEnetTestInterVlanTTL);
                        gEnetTestInterVlanTTL[gEnetTestInterVlanIndex] = frame->payload[ENET_TEST_INTERVLAN_IPV6_HOP_LIMIT_OFFSET];
                        status                                         = ENET_EFAIL;
                    }
                }
                else
                {
                    if (Enet_ntohs(frame->hdr.etherType) == ENET_TEST_INTERVLAN_IPV4_ETHERTYPE)
                    {
                        if (frame->payload[ENET_TEST_INTERVLAN_IPV4_TTL_OFFSET] <= 1)
                        {
                            status = ENET_SOK;
                        }
                        else
                        {
                            gEnetTestInterVlanIndex                        = (gEnetTestInterVlanIndex + 1) % ENET_ARRAYSIZE(gEnetTestInterVlanTTL);
                            gEnetTestInterVlanTTL[gEnetTestInterVlanIndex] = frame->payload[ENET_TEST_INTERVLAN_IPV4_TTL_OFFSET];
                            status                                         = ENET_EFAIL;
                        }
                    }
                }
            }
        }
    }

    if (gEnetTestDumpPolicer)
    {
        Enet_IoctlPrms prms;
        EnetTestStateObj *stateObj = &taskObj->stateObj;
        int32_t dumpStatus;

        ENET_IOCTL_SET_NO_ARGS(&prms);
        dumpStatus = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_TABLE,
                                &prms);
       EnetAppUtils_assert(dumpStatus == ENET_SOK);

        ENET_IOCTL_SET_NO_ARGS(&prms);
        dumpStatus = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_POLICER_ENTRIES,
                                &prms);

       EnetAppUtils_assert(dumpStatus == ENET_SOK);
    }

    *testComplete = BFALSE;

    return status;
}

#define ENET_TEST_INTERVLAN_DEFAULT_SHORTIPG_THRESHOLD                    (11)

static int32_t EnetTestInterVlan_setShortIPG(EnetTestTaskObj *taskObj)
{
    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    Cpsw_SetTxShortIpgCfgInArgs setShortIPGInArgs;
    int32_t status;

    ENET_IOCTL_SET_IN_ARGS(&prms, &setShortIPGInArgs);
    setShortIPGInArgs.configureGapThresh                               = BTRUE;
    setShortIPGInArgs.ipgTriggerThreshBlkCnt                           = 0U;
    setShortIPGInArgs.numMacPorts                                      = 2U;
    setShortIPGInArgs.portShortIpgCfg[0].macPort                       = ENET_TEST_INTERVLAN_INGRESS_PORT_NUM;
    setShortIPGInArgs.portShortIpgCfg[0].shortIpgCfg.txShortGapEn      = BTRUE;
    setShortIPGInArgs.portShortIpgCfg[0].shortIpgCfg.txShortGapLimitEn = BFALSE;

    setShortIPGInArgs.portShortIpgCfg[1].macPort                           = ENET_TEST_INTERVLAN_EGRESS_PORT_NUM;
    setShortIPGInArgs.portShortIpgCfg[1].shortIpgCfg.txShortGapEn      = BTRUE;
    setShortIPGInArgs.portShortIpgCfg[1].shortIpgCfg.txShortGapLimitEn = BFALSE;

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_PER_IOCTL_SET_SHORT_IPG_CFG,
                        &prms);
    if (ENET_SOK == status)
    {
        Cpsw_TxShortIpgCfg getShortIPGOutArgs;

        ENET_IOCTL_SET_OUT_ARGS(&prms, &getShortIPGOutArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_PER_IOCTL_GET_SHORT_IPG_CFG,
                            &prms);
        if (ENET_SOK == status)
        {
            CpswMacPort_PortTxShortIpgCfg *ipgCfg;
            uint32_t i;

            EnetAppUtils_assert(getShortIPGOutArgs.ipgTriggerThreshBlkCnt == 0U);

            for (i = 0U; i < getShortIPGOutArgs.numMacPorts; i++)
            {
                ipgCfg = &getShortIPGOutArgs.portShortIpgCfg[i];
                if ((ipgCfg->macPort == ENET_TEST_INTERVLAN_INGRESS_PORT_NUM) ||
                    (ipgCfg->macPort == ENET_TEST_INTERVLAN_EGRESS_PORT_NUM))
                {
                    EnetAppUtils_assert(ipgCfg->shortIpgCfg.txShortGapEn == BTRUE);
                    EnetAppUtils_assert(ipgCfg->shortIpgCfg.txShortGapLimitEn == BFALSE);
                }
            }
        }
    }

    return status;
}

static int32_t EnetTestInterVlan_addBlackListHostPortAleTableEntries(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    CpswAle_VlanEntryInfo inArgs;
    uint32_t outArgs;

    memset(&inArgs, 0, sizeof(inArgs));
    inArgs.vlanIdInfo.vlanId        = ENET_TEST_INTERVLAN_INTRA_VLANID;
    inArgs.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
    inArgs.vlanMemberList           = EnetTestInterVlan_getIntraVlanMembershipMask();
    inArgs.unregMcastFloodMask      = EnetTestInterVlan_getIntraVlanMembershipMask();
    inArgs.regMcastFloodMask        = EnetTestInterVlan_getIntraVlanMembershipMask();
    inArgs.forceUntaggedEgressMask  = 0U;
    inArgs.noLearnMask              = 0U;
    inArgs.vidIngressCheck          = BFALSE;
    inArgs.limitIPNxtHdr            = BFALSE;
    inArgs.disallowIPFrag           = BFALSE;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_VLAN, &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("%s() failed ADD_VLAN ioctl failed: %d\n",
                           __func__, status);
    }

    memset(&inArgs, 0, sizeof(inArgs));
    inArgs.vlanIdInfo.vlanId        = ENET_TEST_INTERVLAN_INTRA_VLAN_EGRESS_STRIP_ID;
    inArgs.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
    inArgs.vlanMemberList           = EnetTestInterVlan_getIntraVlanMembershipMask();
    inArgs.unregMcastFloodMask      = EnetTestInterVlan_getIntraVlanMembershipMask();
    inArgs.regMcastFloodMask        = EnetTestInterVlan_getIntraVlanMembershipMask();
    inArgs.forceUntaggedEgressMask  = EnetTestInterVlan_getIntraVlanMembershipMask();
    inArgs.noLearnMask              = 0U;
    inArgs.vidIngressCheck          = BFALSE;
    inArgs.limitIPNxtHdr            = BFALSE;
    inArgs.disallowIPFrag           = BFALSE;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_VLAN, &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("%s() failed ADD_VLAN ioctl failed: %d\n",
                           __func__, status);
    }

    return status;
}

static int32_t EnetTestInterVlan_setBlacklistHostportClassifier(EnetTestTaskObj *taskObj)
{
    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    int32_t status;
    CpswAle_PolicerMatchParams blackListTrafficCfg;
    CpswAle_PolicerEntryOutArgs outArgs;

    status = EnetTestInterVlan_addBlackListHostPortAleTableEntries(taskObj);
    if (status == ENET_SOK)
    {
        memset(&blackListTrafficCfg, 0, sizeof(blackListTrafficCfg));

        /* Clear classifier mask to 0 */
        blackListTrafficCfg.policerMatchEnMask = 0U;

        /* Blacklist IPv4 Src address */
        blackListTrafficCfg.srcIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;
        blackListTrafficCfg.srcIpInfo.ipv4Info.numLSBIgnoreBits = 8U;
        memcpy(blackListTrafficCfg.srcIpInfo.ipv4Info.ipv4Addr, testSrcIpv4Addr, sizeof(blackListTrafficCfg.srcIpInfo.ipv4Info.ipv4Addr));
        blackListTrafficCfg.policerMatchEnMask |= CPSW_ALE_POLICER_MATCH_IPSRC;

        /* Blacklist IPv4 Dst address */
        blackListTrafficCfg.dstIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;
        blackListTrafficCfg.dstIpInfo.ipv4Info.numLSBIgnoreBits = 8U;
        memcpy(blackListTrafficCfg.dstIpInfo.ipv4Info.ipv4Addr, testDstIpv4Addr, sizeof(blackListTrafficCfg.dstIpInfo.ipv4Info.ipv4Addr));
        blackListTrafficCfg.policerMatchEnMask |= CPSW_ALE_POLICER_MATCH_IPDST;

        /* Blacklist Src Mac address */
        blackListTrafficCfg.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_INTERVLAN_INGRESS_PORT_NUM);
        ENET_UTILS_ARRAY_COPY(blackListTrafficCfg.srcMacAddrInfo.addr.addr, testBlackListSrcMacAddr);
        blackListTrafficCfg.policerMatchEnMask |= CPSW_ALE_POLICER_MATCH_MACSRC;

        /* Blacklist IVLAN id */
        blackListTrafficCfg.ivlanId                 = ENET_TEST_INTERVLAN_INTRA_VLANID;
        blackListTrafficCfg.policerMatchEnMask |= CPSW_ALE_POLICER_MATCH_IVLAN;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &blackListTrafficCfg, &outArgs);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId,
                            CPSW_ALE_IOCTL_BLOCK_CLASSIFIER_HOSTPORT,
                            &prms);
        if (ENET_SOK == status)
        {
            /* Assert main configuration is correct,
             * Egress op should be enabled with dstPortMask of 0
             * Egress Op code should be non-zero
             */
           EnetAppUtils_assert((outArgs.egressOpEn == BTRUE)
                                &&
                                (outArgs.dstPortMask == 0)
                                &&
                                (outArgs.egressOpcode == 1));

            /* Assert other policer configuration is correct
             */
           EnetAppUtils_assert((outArgs.egressTrunkIdx == 0)
                                &&
                                (outArgs.ttlCheckEn == BFALSE)
                                &&
                                (outArgs.threadIdEn == BFALSE)
                                &&
                                (outArgs.commitRateInBitsPerSec == 0)
                                &&
                                (outArgs.peakRateInBitsPerSec == 0));
        }
    }

    return status;
}
