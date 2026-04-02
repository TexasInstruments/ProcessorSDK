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
 * \file     cpsw_test_network_security.c
 *
 * \brief    This file contains the cpsw_test_network_security test implementation.
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
#include "cpsw_test_network_security.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define IP_V4_NXT_HDR_TCP           6U
#define IP_V4_NXT_HDR_UDP           17U
#define IP_V4_NXT_HDR_ICMP          1U
#define IP_V4_NXT_HDR_IGMP          2U

#define ETH_TEST_VLAN_ID           100U

#define ENET_TEST_SECURITY_INGRESS_PORT  (ENET_MAC_PORT_4)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static int32_t EnetTestSecurity_setVLANentry(EnetTestTaskObj *taskObj);

static int32_t EnetTestSecurity_setOUIEntry(EnetTestTaskObj *taskObj);

static int32_t EnetTestSecurity_SetAleEntry(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static uint8_t testSrcValidOUIAddr[ENET_MAC_ADDR_LEN] =
{
    0x02, 0x00, 0x00, 0x00, 0x00, 0x02
};

static uint8_t testSrcValidOUIAddr2[ENET_MAC_ADDR_LEN] =
{
    0x02, 0x00, 0x00, 0x00, 0x00, 0x04
};

static uint8_t testSrcInvalidOUIAddr[ENET_MAC_ADDR_LEN] =
{
    0x02, 0x02, 0x00, 0x00, 0x00, 0x00
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestSecurity_Run(EnetTestTaskObj *taskObj)
{
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    if (ENET_SOK == status)
    {
        status = EnetTestSecurity_SetAleEntry(taskObj);
    }

    if (ENET_SOK == status)
    {
        status = EnetTestSecurity_setVLANentry(taskObj);
    }

    if (ENET_SOK == status)
    {
        status = EnetTestSecurity_setOUIEntry(taskObj);
    }

    if (ENET_SOK == status)
    {
        Enet_IoctlPrms prms;

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_TABLE,
                            &prms);

       EnetAppUtils_assert(status == ENET_SOK);

        /* Use default mac address instead of board specific mac address so that IXIA test does not change per board */
        EnetTestCommon_getDefaultHostMacAddr(&stateObj->hostMacAddr[0U]);
        status = EnetTestCommon_Run(taskObj);
    }

    return status;
}

void EnetTestSecurity_setOpenPrms(EnetTestTaskObj *taskObj,
                                  Cpsw_Cfg *pCpswCfg,
                                  EnetUtils_Cfg *pUtilsPrms)
{
    Enet_MacPort i;

    // VLAN configurations
    pCpswCfg->aleCfg.vlanCfg.aleVlanAwareMode               = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.cpswVlanAwareMode              = BFALSE;
    pCpswCfg->aleCfg.vlanCfg.autoLearnWithVlan              = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.unknownVlanNoLearn             = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.unknownForceUntaggedEgressMask = 0U;
    pCpswCfg->aleCfg.vlanCfg.unknownRegMcastFloodMask       = 0U;
    pCpswCfg->aleCfg.vlanCfg.unknownUnregMcastFloodMask     = 0U;
    pCpswCfg->aleCfg.vlanCfg.unknownVlanMemberListMask      = CPSW_ALE_ALL_PORTS_MASK;

    // Network security configurations
    pCpswCfg->aleCfg.nwSecCfg.hostOuiNoMatchDeny                = BTRUE;
    pCpswCfg->aleCfg.nwSecCfg.vid0ModeEn                        = BFALSE;
    pCpswCfg->aleCfg.nwSecCfg.malformedPktCfg.srcMcastDropDis   = BFALSE;
    pCpswCfg->aleCfg.nwSecCfg.malformedPktCfg.badLenPktDropEn   = BTRUE;
    pCpswCfg->aleCfg.nwSecCfg.ipPktCfg.dfltNoFragEn             = BTRUE;
    pCpswCfg->aleCfg.nwSecCfg.ipPktCfg.dfltNxtHdrWhitelistEn    = BTRUE;
    pCpswCfg->aleCfg.nwSecCfg.ipPktCfg.ipNxtHdrWhitelistCnt       = 4U;
    pCpswCfg->aleCfg.nwSecCfg.ipPktCfg.ipNxtHdrWhitelist[0]         = IP_V4_NXT_HDR_TCP;
    pCpswCfg->aleCfg.nwSecCfg.ipPktCfg.ipNxtHdrWhitelist[1]         = IP_V4_NXT_HDR_UDP;
    pCpswCfg->aleCfg.nwSecCfg.ipPktCfg.ipNxtHdrWhitelist[2]         = IP_V4_NXT_HDR_ICMP;
    pCpswCfg->aleCfg.nwSecCfg.ipPktCfg.ipNxtHdrWhitelist[3]         = IP_V4_NXT_HDR_IGMP;
    pCpswCfg->aleCfg.nwSecCfg.macAuthCfg.authModeEn             = BTRUE;
    pCpswCfg->aleCfg.nwSecCfg.macAuthCfg.macAuthDisMask         = 0U;

    for (i = ENET_MAC_PORT_FIRST; i < CPSW_ALE_NUM_MAC_PORTS; i++)
    {
        // Port configurations
        pCpswCfg->aleCfg.portCfg[i].learningCfg.noLearn          = BTRUE;
        pCpswCfg->aleCfg.portCfg[i].learningCfg.noSaUpdateEn     = BTRUE;
        pCpswCfg->aleCfg.portCfg[i].vlanCfg.vidIngressCheck      = BTRUE;
        pCpswCfg->aleCfg.portCfg[i].vlanCfg.dropUntagged         = BTRUE;
        pCpswCfg->aleCfg.portCfg[i].vlanCfg.dropDualVlan         = BTRUE;
        pCpswCfg->aleCfg.portCfg[i].vlanCfg.dropDoubleVlan       = BTRUE;
    }
}

void EnetTestSecurity_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                        Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg      = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn           = BFALSE;
}

static int32_t EnetTestSecurity_setVLANentry(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_VlanEntryInfo vlanEntry;
    uint32_t vlanOutInfo;
    CpswAle_GetVlanEntryOutArgs getVlanOutArgs;
    CpswAle_VlanIdInfo getVlanInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memset(&vlanEntry, 0, sizeof(vlanEntry));
    vlanEntry.disallowIPFrag           = BTRUE;
    vlanEntry.forceUntaggedEgressMask  = 0x0;
    vlanEntry.limitIPNxtHdr            = BTRUE;
    vlanEntry.noLearnMask              = 0x0;
    vlanEntry.regMcastFloodMask        = EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK;
    vlanEntry.unregMcastFloodMask      = EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK;
    vlanEntry.vidIngressCheck          = BTRUE;
    vlanEntry.vlanMemberList           = EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK;
    vlanEntry.vlanIdInfo.vlanId        = ETH_TEST_VLAN_ID;
    vlanEntry.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &vlanEntry, &vlanOutInfo);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_VLAN,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("CPSW_ALE_IOCTL_ADD_UCAST failed: %d\n", status);
    }

    memset(&getVlanInArgs, 0, sizeof(getVlanInArgs));
    getVlanInArgs.vlanId        = ETH_TEST_VLAN_ID;
    getVlanInArgs.tagType       = ENET_VLAN_TAG_TYPE_INNER;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &getVlanInArgs, &getVlanOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_LOOKUP_VLAN,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("CPSW_ALE_IOCTL_LOOKUP_VLAN failed: %d\n", status);
    }

    return status;
}

static int32_t EnetTestSecurity_setOUIEntry(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setOuiOutArgs;
    CpswAle_OuiEntryInfo setOuiInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memcpy(&setOuiInArgs.ouiAddr[0], testSrcValidOUIAddr, sizeof(setOuiInArgs.ouiAddr));

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setOuiInArgs, &setOuiOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_OUI,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("%s: failed : %d\n",  __func__, status);
    }

    return status;
}

static int32_t EnetTestSecurity_SetAleEntry(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setUcastOutArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memset(&setUcastInArgs, 0, sizeof(setUcastInArgs));
    memcpy(&setUcastInArgs.addr.addr[0U], testSrcValidOUIAddr,
           sizeof(setUcastInArgs.addr.addr));
    setUcastInArgs.addr.vlanId  = ETH_TEST_VLAN_ID;
    setUcastInArgs.info.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_SECURITY_INGRESS_PORT);
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

    if (status == ENET_SOK)
    {
        memset(&setUcastInArgs, 0, sizeof(setUcastInArgs));
        memcpy(&setUcastInArgs.addr.addr[0U], testSrcValidOUIAddr2,
               sizeof(setUcastInArgs.addr.addr));
        setUcastInArgs.addr.vlanId  = 0U;
        setUcastInArgs.info.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_SECURITY_INGRESS_PORT);
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
    }

    if (status == ENET_SOK)
    {
        memset(&setUcastInArgs, 0, sizeof(setUcastInArgs));
        memcpy(&setUcastInArgs.addr.addr[0U], testSrcInvalidOUIAddr,
               sizeof(setUcastInArgs.addr.addr));
        setUcastInArgs.addr.vlanId  = 0U;
        setUcastInArgs.info.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_SECURITY_INGRESS_PORT);
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
    }

    return status;
}

int32_t EnetTestSecurity_processRxPkt(EnetTestTaskObj *taskObj,
                                      uint32_t rxFlowId,
                                      uint32_t pktNum,
                                      EnetDma_Pkt *pktInfo,
                                      bool *testComplete)
{
    EthFrame *frame;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    int32_t status             = ENET_SOK;
    EthVlanFrame *vlanFrame;

    EnetAppUtils_assert(pktInfo->sgList.numScatterSegments == 1U);
    /* Consume the packet by just printing its content */
    frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;

    if (memcmp(&frame->hdr.dstMac[0], &stateObj->hostMacAddr[0U], sizeof(frame->hdr.dstMac)) != 0)
    {
        status = ENET_EFAIL;
    }

    if (memcmp(&frame->hdr.srcMac[0], &testSrcValidOUIAddr, ENET_OUI_ADDR_LEN) != 0)
    {
        status = ENET_EFAIL;
    }

    vlanFrame = (EthVlanFrame *)frame;
    if ((Enet_ntohs(vlanFrame->hdr.tci) & 0xFFFU) != ETH_TEST_VLAN_ID)
    {
        // status = ENET_EFAIL;
    }

    *testComplete = BFALSE;
    return status;
}
