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
 * \file     cpsw_test_pvid.c
 *
 * \brief    This file contains the cpsw_test_pvid test implementation.
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
#include "cpsw_test_pvid.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define ETH_TEST_MAC_PORT_VLAN_ID_BASE            (100U)
#define ETH_TEST_HOST_PORT_VLAN_ID                (200U)

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

int32_t EnetTestPvid_Run(EnetTestTaskObj *taskObj)
{
    int32_t status = ENET_SOK;

    status = EnetTestCommon_Run(taskObj);

    return status;
}

void EnetTestPvid_setOpenPrms(EnetTestTaskObj *taskObj,
                              Cpsw_Cfg *pCpswCfg,
                              EnetUtils_Cfg *pUtilsPrms)
{
    Enet_MacPort i;

    pCpswCfg->aleCfg.modeFlags =
        (CPSW_ALE_CFG_MODULE_EN | CPSW_ALE_CFG_UNKNOWN_UCAST_FLOOD2HOST);
    pCpswCfg->aleCfg.vlanCfg.unknownUnregMcastFloodMask = 0x0;
    pCpswCfg->aleCfg.vlanCfg.unknownRegMcastFloodMask   = 0x0;
    pCpswCfg->aleCfg.vlanCfg.unknownVlanMemberListMask  = CPSW_ALE_ALL_PORTS_MASK;
    pCpswCfg->aleCfg.vlanCfg.aleVlanAwareMode           = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.autoLearnWithVlan          = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.cpswVlanAwareMode          = BTRUE;
    pCpswCfg->aleCfg.nwSecCfg.vid0ModeEn                = BTRUE;

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
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vlanIdInfo.vlanId        = ETH_TEST_HOST_PORT_VLAN_ID;
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
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vlanIdInfo.vlanId        = ETH_TEST_MAC_PORT_VLAN_ID_BASE + ENET_MACPORT_NORM(i);
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vlanMemberList           = CPSW_ALE_ALL_PORTS_MASK;
    }

    pCpswCfg->hostPortCfg.vlanCfg.portPri = 7U;
    pCpswCfg->hostPortCfg.vlanCfg.portCfi = 0U;
    pCpswCfg->hostPortCfg.vlanCfg.portVID = ETH_TEST_HOST_PORT_VLAN_ID;
    pCpswCfg->vlanCfg.vlanAware           = BTRUE;
}

void EnetTestPvid_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                    Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg     = (CpswMacPort_Cfg*)pLinkArgs->macCfg;

    macCfg->loopbackEn      = BFALSE;
    macCfg->vlanCfg.portPri = ENET_MACPORT_NORM(portNum);
    macCfg->vlanCfg.portCfi = 0U;
    macCfg->vlanCfg.portVID = ETH_TEST_MAC_PORT_VLAN_ID_BASE +
                                          ENET_MACPORT_NORM(portNum);
}

int32_t EnetTestPvid_processRxPkt(EnetTestTaskObj *taskObj,
                                  uint32_t rxFlowId,
                                  uint32_t pktNum,
                                  EnetDma_Pkt *pktInfo,
                                  bool *testComplete)
{
    int32_t status = ENET_SOK;
    EthVlanFrame *frame;
    uint32_t vid;
    uint32_t pcp;

    EnetAppUtils_assert(pktInfo->sgList.numScatterSegments == 1U);
    /* Consume the packet by just printing its content */
    frame = (EthVlanFrame *)pktInfo->sgList.list[0].bufPtr;

    vid = (Enet_ntohs(frame->hdr.tci) & 0xFFFU);
    pcp = (((Enet_htons(frame->hdr.tci)) >> 13) & 0x7);

    if ((vid == ETH_TEST_MAC_PORT_VLAN_ID_BASE + ENET_MACPORT_NORM(ENET_MAC_PORT_4)) && (pcp == ENET_MACPORT_NORM(ENET_MAC_PORT_4)))
    {
        status = ENET_SOK;
    }
    else
    {
        Enet_IoctlPrms prms;
        EnetTestStateObj *stateObj = &taskObj->stateObj;

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_TABLE,
                            &prms);
       EnetAppUtils_assert(status == ENET_SOK);

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_POLICER_ENTRIES,
                            &prms);

       EnetAppUtils_assert(status == ENET_SOK);
        status = ENET_EFAIL;
    }

    *testComplete = BFALSE;
    return status;
}
