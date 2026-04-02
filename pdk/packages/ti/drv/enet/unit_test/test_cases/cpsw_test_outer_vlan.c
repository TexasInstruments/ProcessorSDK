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
 * \file     cpsw_test_outer_vlan.c
 *
 * \brief    This file contains the cpsw outer vlan test implementation.
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
#include "cpsw_test_outer_vlan.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ETH_TEST_VLAN_ID                      (100U)
#define ETHERTYPE_OUTER_VLAN_TAG              (0x88a8U)
#define ETH_TEST_HOST_PORT_VLAN_ID            (1000U)
#define ETH_TEST_MAC_PORT_VLAN_ID_BASE        (500U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */
/* Test ALE Entry Multicast Address */
static uint8_t testMCastAddr[ENET_MAC_ADDR_LEN] =
{
    0x01, 0x80, 0xC2, 0x00, 0x00, 0x02
};

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static int EnetTestOuterVlan_addAleVlanEntry(EnetTestTaskObj *taskObj);

static int EnetTestOuterVlan_setAleMulticastEntry(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestOuterVlan_Run(EnetTestTaskObj *taskObj)
{
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    /* Add ALE entry to force untagged packets on ETH_TEST_VLAN_ID */
    status = EnetTestOuterVlan_addAleVlanEntry(taskObj);
    if (ENET_SOK == status)
    {
        status = EnetTestOuterVlan_setAleMulticastEntry(taskObj);
    }

    if (ENET_SOK == status)
    {
        Enet_IoctlPrms prms;

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_TABLE,
                            &prms);
       EnetAppUtils_assert(status == ENET_SOK);

        status = EnetTestCommon_Run(taskObj);
    }

    return status;
}

void EnetTestOuterVlan_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                         Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg     = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn = BFALSE;
}

void EnetTestOuterVlan_setOpenPrms(EnetTestTaskObj *taskObj,
                                   Cpsw_Cfg *pCpswCfg,
                                   EnetUtils_Cfg *pUtilsPrms)
{
    pCpswCfg->aleCfg.vlanCfg.aleVlanAwareMode               = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.unknownVlanMemberListMask      = 0;
    pCpswCfg->aleCfg.vlanCfg.unknownForceUntaggedEgressMask = 0U;
    pCpswCfg->aleCfg.vlanCfg.unknownRegMcastFloodMask       = 0;
    pCpswCfg->aleCfg.vlanCfg.unknownUnregMcastFloodMask     = 0;
    pCpswCfg->aleCfg.vlanCfg.unknownVlanNoLearn             = 0;

    pCpswCfg->vlanCfg.vlanSwitch = ENET_VLAN_TAG_TYPE_OUTER;
}

static int EnetTestOuterVlan_addAleVlanEntry(EnetTestTaskObj *taskObj)
{
    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj   = &taskObj->stateObj;
    CpswAle_VlanEntryInfo inArgs =
    {
        .vlanIdInfo.vlanId        = ETH_TEST_VLAN_ID,
        .vlanMemberList           = CPSW_ALE_ALL_PORTS_MASK,
        .unregMcastFloodMask      = 0U,
        .regMcastFloodMask        = CPSW_ALE_ALL_PORTS_MASK,
        .forceUntaggedEgressMask  = 0U,
        .noLearnMask              = 0U,
        .vidIngressCheck          = BTRUE,
        .limitIPNxtHdr            = BFALSE,
        .disallowIPFrag           = BFALSE,
        .vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_OUTER,
    };
    uint32_t outArgs;
    int32_t status;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_VLAN, &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestOuterVlan_addAleVlanEntry() ADD_VLAN ioctl failed: %d\n",
                           status);
    }

    return status;
}

static int EnetTestOuterVlan_setAleMulticastEntry(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setMcastOutArgs;
    CpswAle_SetMcastEntryInArgs setMcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memcpy(&setMcastInArgs.addr.addr[0], testMCastAddr,
           sizeof(setMcastInArgs.addr.addr));
    setMcastInArgs.addr.vlanId = 0U;

    setMcastInArgs.info.super      = BFALSE;
    setMcastInArgs.info.fwdState   = CPSW_ALE_FWDSTLVL_FWD_LRN;
    setMcastInArgs.info.portMask   = CPSW_ALE_ALL_PORTS_MASK;
    setMcastInArgs.info.numIgnBits = 0U;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setMcastInArgs, &setMcastOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_MCAST,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestOuterVlan_setAleMulticastEntry() failed CPSW_ALE_IOCTL_ADD_MCAST: %d\n",
                           status);
    }

    return status;
}

int32_t EnetTestOuterVlan_processRxPkt(EnetTestTaskObj *taskObj,
                                       uint32_t rxFlowId,
                                       uint32_t pktNum,
                                       EnetDma_Pkt *pktInfo,
                                       bool *testComplete)
{
    int32_t status = ENET_SOK;

    EthVlanFrame *frame;
    uint32_t vid;
    uint16_t etherType;
    uint32_t tpid;

    EnetAppUtils_assert(pktInfo->sgList.numScatterSegments == 1U);
    /* Consume the packet by just printing its content */
    frame = (EthVlanFrame *)pktInfo->sgList.list[0].bufPtr;

    tpid      = (Enet_ntohs(frame->hdr.tpid) & 0xFFFFU);
    vid       = (Enet_ntohs(frame->hdr.tci) & 0xFFFU);
    etherType = Enet_ntohs(frame->hdr.etherType);

    if ((vid == ETH_TEST_VLAN_ID) && (etherType == ETHERTYPE_VLAN_TAG) && (tpid == ETHERTYPE_OUTER_VLAN_TAG))
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
