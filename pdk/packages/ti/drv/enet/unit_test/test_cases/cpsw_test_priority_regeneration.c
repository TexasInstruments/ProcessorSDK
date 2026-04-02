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
 * \file     cpsw_test_priority_regeneration.c
 *
 * \brief    This file contains the cpsw_test_priority_regeneration test implementation.
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
#include <ti/drv/enet/examples/utils/include/enet_ethpatterns.h>
#include <ti/osal/osal.h>
#include <ti/board/board.h>

#include "enet_test_base.h"
#include "enet_test_entry.h"
#include "cpsw_test_priority_regeneration.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ETH_TEST_MAC_PORT_VLAN_ID_BASE                              (100U)
#define ETH_TEST_HOST_PORT_VLAN_ID                                  (200U)
#define FRAME_HEADER_NB_TO_SHIFT                                    (13U)
#define FRAME_HEADER_PRIORITY                                       (0x7U)

#define ENETTEST_PRIORITYREGENERATION_RX_RECV_COUNT                 (10000)

#define ENETTEST_PRIORITYREGENERATION_EGRESS_PORT                   (ENET_MAC_PORT_3)
#define ENETTEST_PRIORITYREGENERATION_INGRESS_PORT                  (ENET_MAC_PORT_4)
#define ENETTEST_PRIORITYREGENERATION_VLANID                        (1200U)
#define ENETTEST_PRIORITYREGENERATION_DEFAULT_SHORTIPG_THRESHOLD    (11)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

static void EnetTestPriorityRegeneration_validateTosMap(const uint32_t *tosMap, uint32_t alePortNum);

static void EnetTestPriorityRegeneration_validateRxPri2HdrPktPri(const uint32_t *priorityMap, uint32_t alePortNum);

static void EnetTestPriorityRegeneration_validateHdrPktPri2SwitchPri(const uint32_t *priorityMap, uint32_t alePortNum);

static int32_t EnetTestPriorityRegeneration_getHostRxPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestPriorityRegeneration_getMacRxPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestPriorityRegeneration_getHostRxDscpPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestPriorityRegeneration_getMacRxDscpPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestPriorityRegeneration_getHostTxPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestPriorityRegeneration_getMacTxPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestPriorityRegeneration_remapHostRxPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestPriorityRegeneration_remapMacRxPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestPriorityRegeneration_remapHostRxDscpPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestPriorityRegeneration_remapMacRxDscpPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestPriorityRegeneration_remapHostTxPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestPriorityRegeneration_remapMacTxPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestPriorityRegeneration_xmitTest(EnetTestTaskObj *taskObj,
                                                     uint32_t txCfgIndex,
                                                     uint32_t num,
                                                     const uint8_t *dstAddr,
                                                     const uint8_t *srcAddr,
                                                     uint16_t pcp,
                                                     uint16_t vid,
                                                     uint16_t etherType,
                                                     uint16_t length);

static int32_t EnetTestPriorityRegeneration_transmitPkts(EnetTestTaskObj *taskObj,
                                                         EnetDma_PktQ *pTxSubmitQ,
                                                         uint32_t txCfgIndex);

static void EnetTestPriorityRegeneration_transmitTest(EnetTestTaskObj *taskObj,
                                                      uint32_t txCfgIndex,
                                                      uint16_t pcp);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Broadcast address */
static uint8_t bcastAddr[ENET_MAC_ADDR_LEN] =
{
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU
};

/* "RX Packet Priority" to "Header Packet Priority" mapping */
static uint32_t mapRxPktPri2HdrPktPri[ENET_PRI_NUM] =
{
    7/*ENET_HEADER_PACKET_PRI_7*/,  /* 0 -> 7 */
    6/*ENET_HEADER_PACKET_PRI_6*/,  /* 1 -> 6 */
    5/*ENET_HEADER_PACKET_PRI_5*/,  /* 2 -> 5 */
    4/*ENET_HEADER_PACKET_PRI_4*/,  /* 3 -> 4 */
    3/*ENET_HEADER_PACKET_PRI_3*/,  /* 4 -> 3 */
    2/*ENET_HEADER_PACKET_PRI_2*/,  /* 5 -> 2 */
    1/*ENET_HEADER_PACKET_PRI_1*/,  /* 6 -> 1 */
    0/*ENET_HEADER_PACKET_PRI_0*/,  /* 7 -> 0 */
};

#define ENET_HEADER_PACKET_PRI_NUM 8U

/* "Header Packet Priority" to "Switch Priority" mapping for host port */
static uint32_t mapHostHdrPktPri2SwitchPri[ENET_HEADER_PACKET_PRI_NUM] =
{
    1 /*ENET_SWITCH_PRI_1*/, /* 0 -> 1 */
    1 /*ENET_SWITCH_PRI_1*/,
    3 /*ENET_SWITCH_PRI_3*/, /* 2 -> 3 */
    3 /*ENET_SWITCH_PRI_3*/,
    5 /*ENET_SWITCH_PRI_5*/, /* 4 -> 5 */
    5 /*ENET_SWITCH_PRI_5*/,
    7 /*ENET_SWITCH_PRI_7*/, /* 6 -> 7 */
    7 /*ENET_SWITCH_PRI_7*/,
};

/* "Header Packet Priority" to "Switch Priority" mapping for MAC ports */
static uint32_t mapHdrPktPri2SwitchPri[ENET_MAC_PORT_NUM][ENET_HEADER_PACKET_PRI_NUM] =
{
    {   /* ENET_MAC_PORT_1 */
        0 /*ENET_SWITCH_PRI_0*/,
        0 /*ENET_SWITCH_PRI_0*/, /* 1 -> 0 */
        0 /*ENET_SWITCH_PRI_0*/, /* 2 -> 0 */
        3 /*ENET_SWITCH_PRI_3*/,
        4 /*ENET_SWITCH_PRI_4*/,
        5 /*ENET_SWITCH_PRI_5*/,
        6 /*ENET_SWITCH_PRI_6*/,
        7 /*ENET_SWITCH_PRI_7*/,
    },
    {   /* ENET_MAC_PORT_2 */
        0 /*ENET_SWITCH_PRI_0*/,
        1 /*ENET_SWITCH_PRI_1*/,
        1 /*ENET_SWITCH_PRI_1*/, /* 2 -> 1 */
        1 /*ENET_SWITCH_PRI_1*/, /* 3 -> 1 */
        4 /*ENET_SWITCH_PRI_4*/,
        5 /*ENET_SWITCH_PRI_5*/,
        6 /*ENET_SWITCH_PRI_6*/,
        7 /*ENET_SWITCH_PRI_7*/,
    },
    {   /* ENET_MAC_PORT_3 */
        0 /*ENET_SWITCH_PRI_0*/,
        1 /*ENET_SWITCH_PRI_1*/,
        2 /*ENET_SWITCH_PRI_2*/,
        2 /*ENET_SWITCH_PRI_2*/, /* 3 -> 2 */
        2 /*ENET_SWITCH_PRI_2*/, /* 4 -> 2 */
        5 /*ENET_SWITCH_PRI_5*/,
        6 /*ENET_SWITCH_PRI_6*/,
        7 /*ENET_SWITCH_PRI_7*/,
    },
    {   /* ENET_MAC_PORT_4 */
        0 /*ENET_SWITCH_PRI_0*/,
        1 /*ENET_SWITCH_PRI_1*/,
        2 /*ENET_SWITCH_PRI_2*/,
        3 /*ENET_SWITCH_PRI_3*/,
        3 /*ENET_SWITCH_PRI_3*/, /* 4 -> 3 */
        3 /*ENET_SWITCH_PRI_3*/, /* 5 -> 3 */
        6 /*ENET_SWITCH_PRI_6*/,
        7 /*ENET_SWITCH_PRI_7*/,
    },
    {   /* ENET_MAC_PORT_5 */
        0 /*ENET_SWITCH_PRI_0*/,
        1 /*ENET_SWITCH_PRI_1*/,
        2 /*ENET_SWITCH_PRI_2*/,
        3 /*ENET_SWITCH_PRI_3*/,
        4 /*ENET_SWITCH_PRI_4*/,
        4 /*ENET_SWITCH_PRI_4*/, /* 5 -> 4 */
        4 /*ENET_SWITCH_PRI_4*/, /* 6 -> 4 */
        7 /*ENET_SWITCH_PRI_7*/,
    },
    {   /* ENET_MAC_PORT_6 */
        0 /*ENET_SWITCH_PRI_0*/,
        1 /*ENET_SWITCH_PRI_1*/,
        2 /*ENET_SWITCH_PRI_2*/,
        3 /*ENET_SWITCH_PRI_3*/,
        4 /*ENET_SWITCH_PRI_4*/,
        5 /*ENET_SWITCH_PRI_5*/,
        5 /*ENET_SWITCH_PRI_5*/, /* 6 -> 5 */
        5 /*ENET_SWITCH_PRI_5*/, /* 7 -> 5 */
    },
    {   /* ENET_MAC_PORT_7 */
        6 /*ENET_SWITCH_PRI_6*/, /* 0 -> 6 */
        1 /*ENET_SWITCH_PRI_1*/,
        2 /*ENET_SWITCH_PRI_2*/,
        3 /*ENET_SWITCH_PRI_3*/,
        4 /*ENET_SWITCH_PRI_4*/,
        5 /*ENET_SWITCH_PRI_5*/,
        6 /*ENET_SWITCH_PRI_6*/,
        6 /*ENET_SWITCH_PRI_6*/, /* 7 -> 6 */
    },
    {   /* ENET_MAC_PORT_8 */
        0 /*ENET_SWITCH_PRI_0*/, /* 0 -> 7 */
        1 /*ENET_SWITCH_PRI_1*/, /* 1 -> 7 */
        2 /*ENET_SWITCH_PRI_2*/,
        3 /*ENET_SWITCH_PRI_3*/,
        4 /*ENET_SWITCH_PRI_4*/,
        5 /*ENET_SWITCH_PRI_5*/,
        6 /*ENET_SWITCH_PRI_6*/,
        7 /*ENET_SWITCH_PRI_7*/,
    },
};

static uint8_t testIngressPortMacAddr[] = {0x00, 0x11, 0x01, 0x00, 0x00, 0x01};
static uint8_t testEgressPortMacAddr[] = {0x00, 0x11, 0x02, 0x00, 0x00, 0x01};
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
static uint32_t EnetTestPriorityRegeneration_getVlanMembershipMask(void)
{
    uint32_t memberShipMask;

    memberShipMask =
        (1 << CPSW_ALE_MACPORT_TO_ALEPORT(ENETTEST_PRIORITYREGENERATION_INGRESS_PORT));
    memberShipMask |=
        (1 << CPSW_ALE_MACPORT_TO_ALEPORT(ENETTEST_PRIORITYREGENERATION_EGRESS_PORT));
    memberShipMask |= CPSW_ALE_HOST_PORT_MASK;
    return memberShipMask;
}


static int32_t EnetTestPriorityRegeneration_setShortIPG(EnetTestTaskObj *taskObj)
{
    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    Cpsw_SetTxShortIpgCfgInArgs setShortIPGInArgs;
    int32_t status;

    ENET_IOCTL_SET_IN_ARGS(&prms, &setShortIPGInArgs);
    setShortIPGInArgs.configureGapThresh                               = BTRUE;
    setShortIPGInArgs.ipgTriggerThreshBlkCnt                           = 0U;
    setShortIPGInArgs.numMacPorts                                      = 2U;
    setShortIPGInArgs.portShortIpgCfg[0].macPort                       = ENETTEST_PRIORITYREGENERATION_INGRESS_PORT;
    setShortIPGInArgs.portShortIpgCfg[0].shortIpgCfg.txShortGapEn      = BTRUE;
    setShortIPGInArgs.portShortIpgCfg[0].shortIpgCfg.txShortGapLimitEn = BFALSE;

    setShortIPGInArgs.portShortIpgCfg[1].macPort                           = ENETTEST_PRIORITYREGENERATION_EGRESS_PORT;
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
                if ((ipgCfg->macPort == ENETTEST_PRIORITYREGENERATION_INGRESS_PORT) ||
                    (ipgCfg->macPort == ENETTEST_PRIORITYREGENERATION_EGRESS_PORT))
                {
                    EnetAppUtils_assert(ipgCfg->shortIpgCfg.txShortGapEn == BTRUE);
                    EnetAppUtils_assert(ipgCfg->shortIpgCfg.txShortGapLimitEn == BFALSE);
                }
            }
        }
    }

    return status;
}

static int32_t EnetTestPriorityRegeneration_addAleEntries(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setUcastOutArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    int32_t dumpStatus;

    memset(&setUcastInArgs,0,sizeof(setUcastInArgs));
    memcpy(&setUcastInArgs.addr.addr[0U], testEgressPortMacAddr,
           sizeof(setUcastInArgs.addr.addr));
    setUcastInArgs.addr.vlanId  = 0U;
    setUcastInArgs.info.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENETTEST_PRIORITYREGENERATION_EGRESS_PORT);
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
        memset(&setUcastInArgs,0,sizeof(setUcastInArgs));
        memcpy(&setUcastInArgs.addr.addr[0U], testIngressPortMacAddr,
               sizeof(setUcastInArgs.addr.addr));
        setUcastInArgs.addr.vlanId  = 0U;
        setUcastInArgs.info.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENETTEST_PRIORITYREGENERATION_INGRESS_PORT);
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
    }
    if (status == ENET_SOK)
    {
        CpswAle_VlanEntryInfo inArgs;
        uint32_t outArgs;

        memset(&inArgs,0,sizeof(inArgs));
        inArgs.vlanIdInfo.vlanId        = ENETTEST_PRIORITYREGENERATION_VLANID;
        inArgs.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
        inArgs.vlanMemberList           = EnetTestPriorityRegeneration_getVlanMembershipMask();
        inArgs.unregMcastFloodMask      = EnetTestPriorityRegeneration_getVlanMembershipMask();
        inArgs.regMcastFloodMask        = EnetTestPriorityRegeneration_getVlanMembershipMask();
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

    ENET_IOCTL_SET_NO_ARGS(&prms);
    dumpStatus = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_TABLE,
                            &prms);
   EnetAppUtils_assert(dumpStatus == ENET_SOK);

    return status;
}



int32_t EnetTestPriorityRegeneration_run(EnetTestTaskObj *taskObj)
{
    int32_t status = ENET_SOK;
    uint32_t rxReadyCnt = 0U;

    status = EnetTestPriorityRegeneration_setShortIPG(taskObj);
   EnetAppUtils_assert(status == ENET_SOK);

    status = EnetTestPriorityRegeneration_addAleEntries(taskObj);
   EnetAppUtils_assert(status == ENET_SOK);

    EnetTestCommon_waitForPortLink(taskObj);

    /* Remap Host port Rx packet priority */
    status = EnetTestPriorityRegeneration_remapHostRxPriority(taskObj);
    if (status == ENET_SOK)
    {
        status = EnetTestPriorityRegeneration_getHostRxPriority(taskObj);
    }

    /* Remap Mac port Rx packet priority */
    if (status == ENET_SOK)
    {
        status = EnetTestPriorityRegeneration_remapMacRxPriority(taskObj);
        if (status == ENET_SOK)
        {
            status = EnetTestPriorityRegeneration_getMacRxPriority(taskObj);
        }
    }

    /* Remap Host port IPDSCP Rx packet priority */
    if (status == ENET_SOK)
    {
        status = EnetTestPriorityRegeneration_remapHostRxDscpPriority(taskObj);
        if (status == ENET_SOK)
        {
            status = EnetTestPriorityRegeneration_getHostRxDscpPriority(taskObj);
        }
    }

    /* Remap MAC port IPDSCP Rx packet priority */
    if (status == ENET_SOK)
    {
        status = EnetTestPriorityRegeneration_remapMacRxDscpPriority(taskObj);
        if (status == ENET_SOK)
        {
            status = EnetTestPriorityRegeneration_getMacRxDscpPriority(taskObj);
        }
    }

    /* Remap Host port Tx packet priority */
    if (status == ENET_SOK)
    {
        status = EnetTestPriorityRegeneration_remapHostTxPriority(taskObj);
        if (status == ENET_SOK)
        {
            status = EnetTestPriorityRegeneration_getHostTxPriority(taskObj);
        }
    }

    /* Remap Mac port Tx packet priority */
    if (status == ENET_SOK)
    {
        status = EnetTestPriorityRegeneration_remapMacTxPriority(taskObj);
        if (status == ENET_SOK)
        {
            status = EnetTestPriorityRegeneration_getMacTxPriority(taskObj);
        }
    }

   EnetAppUtils_assert(taskObj->taskCfg->numRxFlow == 1U);
    do
    {
        rxReadyCnt += EnetTestCommon_receivePkts(taskObj, 0U);
        EnetTestCommon_discardRxPkts(taskObj, 0);
        EnetTest_wait(10U);
    } while (rxReadyCnt < ENETTEST_PRIORITYREGENERATION_RX_RECV_COUNT);

    /* Transmit packets with all PCP values.Packets should be captured on egress port and analyzed for correct PCP values */
    EnetTestPriorityRegeneration_transmitTest(taskObj, 0U, 0U);
    EnetTestPriorityRegeneration_transmitTest(taskObj, 0U, 1U);
    EnetTestPriorityRegeneration_transmitTest(taskObj, 0U, 2U);
    EnetTestPriorityRegeneration_transmitTest(taskObj, 0U, 3U);
    EnetTestPriorityRegeneration_transmitTest(taskObj, 0U, 4U);
    EnetTestPriorityRegeneration_transmitTest(taskObj, 0U, 5U);
    EnetTestPriorityRegeneration_transmitTest(taskObj, 0U, 6U);
    EnetTestPriorityRegeneration_transmitTest(taskObj, 0U, 7U);

    return status;
}

void EnetTestPriorityRegeneration_setOpenPrms(EnetTestTaskObj *taskObj,
                                              Cpsw_Cfg *pCpswCfg,
                                              EnetUtils_Cfg *pUtilsPrms)
{
    pCpswCfg->vlanCfg.vlanAware = BTRUE;

    pCpswCfg->aleCfg.vlanCfg.aleVlanAwareMode          = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.cpswVlanAwareMode         = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.unknownVlanMemberListMask = CPSW_ALE_ALL_PORTS_MASK;

    pCpswCfg->hostPortCfg.vlanCfg.portPri       = 3U;
    pCpswCfg->hostPortCfg.vlanCfg.portCfi       = 0U;
    pCpswCfg->hostPortCfg.vlanCfg.portVID       = ETH_TEST_HOST_PORT_VLAN_ID;
    pCpswCfg->hostPortCfg.rxVlanRemapEn         = BTRUE;
    pCpswCfg->hostPortCfg.rxDscpIPv4RemapEn     = BTRUE;
    pCpswCfg->hostPortCfg.rxDscpIPv6RemapEn     = BTRUE;
}

void EnetTestPriorityRegeneration_setMacConfig(EnetPer_PortLinkCfg *pLinkArgs,
                                               Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg     = (CpswMacPort_Cfg*)pLinkArgs->macCfg;

    macCfg->loopbackEn      = BFALSE;
    macCfg->vlanCfg.portPri = 3U;
    macCfg->vlanCfg.portCfi = 0U;
    macCfg->vlanCfg.portVID = ETH_TEST_MAC_PORT_VLAN_ID_BASE +
                              ENET_MACPORT_NORM(portNum);
}

static void EnetTestPriorityRegeneration_validateTosMap(const uint32_t *tosMap, uint32_t alePortNum)
{
    uint32_t i;

    for (i = 0U; i < ENET_TOS_PRI_NUM; i++)
    {
       EnetAppUtils_assert(tosMap[i] == (i % 8U));
    }

}

static void EnetTestPriorityRegeneration_validateRxPri2HdrPktPri(const uint32_t *priorityMap, uint32_t alePortNum)
{
    uint32_t i;

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
       EnetAppUtils_assert(priorityMap[i] == mapRxPktPri2HdrPktPri[i]);
    }
}

static void EnetTestPriorityRegeneration_validateHdrPktPri2SwitchPri(const uint32_t *priorityMap, uint32_t alePortNum)
{
    uint32_t i;

    for (i = 0U; i < ENET_HEADER_PACKET_PRI_NUM; i++)
    {
        if (alePortNum == CPSW_ALE_HOST_PORT_NUM)
        {
           EnetAppUtils_assert(priorityMap[i] == mapHostHdrPktPri2SwitchPri[i]);
        }
        else
        {
          EnetAppUtils_assert(alePortNum > CPSW_ALE_MACPORT_BASE);
          EnetAppUtils_assert((alePortNum - CPSW_ALE_MACPORT_BASE) < ENET_ARRAYSIZE(mapHdrPktPri2SwitchPri));
          EnetAppUtils_assert(priorityMap[i] == mapHdrPktPri2SwitchPri[alePortNum - CPSW_ALE_MACPORT_BASE][i]);
        }
    }

}

/* Get Host port Rx Packet priority */
static int32_t EnetTestPriorityRegeneration_getHostRxPriority(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *testStateObj = &taskObj->stateObj;
    EnetPort_PriorityMap getHostOutArgs;
    Enet_IoctlPrms prms;
    int32_t status;

    ENET_IOCTL_SET_OUT_ARGS(&prms, &getHostOutArgs);

    status = Enet_ioctl(testStateObj->hEnet,
                        testStateObj->coreId,
                        ENET_HOSTPORT_IOCTL_GET_PRI_REGEN_MAP,
                        &prms);
    if (status == ENET_SOK)
    {
        EnetTestPriorityRegeneration_validateRxPri2HdrPktPri(getHostOutArgs.priorityMap, CPSW_ALE_HOST_PORT_NUM);
    }
    else
    {
       EnetAppUtils_print("getHostRxPriority() ENET_HOSTPORT_IOCTL_GET_PRI_REGEN_MAP failed: %d\n",
                           status);
    }

    return status;
}

/* Packet priority to header packet priority map */
static int32_t EnetTestPriorityRegeneration_remapHostRxPriority(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *testStateObj = &taskObj->stateObj;
    EnetPort_PriorityMap setHostInArgs;
    Enet_IoctlPrms prms;
    int32_t status;

    /* Copy the test "RX Packet Priority" to "Header Packet Priority" mapping */
    memcpy(setHostInArgs.priorityMap,
           mapRxPktPri2HdrPktPri,
           sizeof(mapRxPktPri2HdrPktPri));
    ENET_IOCTL_SET_IN_ARGS(&prms, &setHostInArgs);

    status = Enet_ioctl(testStateObj->hEnet,
                        testStateObj->coreId,
                        ENET_HOSTPORT_IOCTL_SET_PRI_REGEN_MAP,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("remapHostRxPriority() ENET_HOSTPORT_IOCTL_SET_PRI_REGEN_MAP failed: %d\n",
                           status);
    }

    return status;
}

/* Get MAC port Rx Packet priority */
static int32_t EnetTestPriorityRegeneration_getMacRxPriority(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *testStateObj = &taskObj->stateObj;
    EnetTestMacPortList_t enabledPorts;
    EnetMacPort_GenericInArgs getMacInArgs;
    EnetPort_PriorityMap getMacOutArgs;
    Enet_IoctlPrms prms;
    uint32_t i;
    int32_t status = ENET_SOK;

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);

    for (i = 0U; i < enabledPorts.numMacPorts; i++)
    {
        getMacInArgs.macPort = enabledPorts.macPortList[i];
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &getMacInArgs, &getMacOutArgs);

        status = Enet_ioctl(testStateObj->hEnet,
                            testStateObj->coreId,
                            ENET_MACPORT_IOCTL_GET_PRI_REGEN_MAP,
                            &prms);
        if (status == ENET_SOK)
        {
            EnetTestPriorityRegeneration_validateRxPri2HdrPktPri(getMacOutArgs.priorityMap, CPSW_ALE_MACPORT_TO_ALEPORT(enabledPorts.macPortList[i]));
        }
        else
        {
           EnetAppUtils_print("getMacRxPriority() ENET_MACPORT_IOCTL_GET_PRI_REGEN_MAP failed: %d\n",
                               status);
        }
    }

    return status;
}

/* Packet priority to header packet priority map */
static int32_t EnetTestPriorityRegeneration_remapMacRxPriority(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *testStateObj = &taskObj->stateObj;
    EnetTestMacPortList_t enabledPorts;
    EnetMacPort_SetPriorityRegenMapInArgs setMacInArgs;
    Enet_IoctlPrms prms;
    uint32_t i;
    int32_t status = ENET_SOK;

    /* Copy the test "RX Packet Priority" to "Header Packet Priority" mapping */
    memcpy(setMacInArgs.priorityRegenMap.priorityMap,
           mapRxPktPri2HdrPktPri,
           sizeof(mapRxPktPri2HdrPktPri));

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);

    /* Set "RX Packet Priority" to "Header Packet Priority" mapping for all MAC ports */
    for (i = 0U; i < enabledPorts.numMacPorts; i++)
    {
        setMacInArgs.macPort = enabledPorts.macPortList[i];
        ENET_IOCTL_SET_IN_ARGS(&prms, &setMacInArgs);

        status = Enet_ioctl(testStateObj->hEnet,
                            testStateObj->coreId,
                            ENET_MACPORT_IOCTL_SET_PRI_REGEN_MAP,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("remapMacRxPriority() ENET_MACPORT_IOCTL_SET_PRI_REGEN_MAP failed: %d\n",
                               status);
        }
    }

    return status;
}

/* Get Host port IPDSCP Rx Packet priority */
static int32_t EnetTestPriorityRegeneration_getHostRxDscpPriority(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *testStateObj = &taskObj->stateObj;
    EnetPort_DscpPriorityMap getHostOutArgs;
    Enet_IoctlPrms prms;
    int32_t status;

    ENET_IOCTL_SET_OUT_ARGS(&prms, &getHostOutArgs);

    status = Enet_ioctl(testStateObj->hEnet,
                        testStateObj->coreId,
                        ENET_HOSTPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP,
                        &prms);
    if (status == ENET_SOK)
    {
        EnetTestPriorityRegeneration_validateTosMap(getHostOutArgs.tosMap, CPSW_ALE_HOST_PORT_NUM);
    }
    else
    {
       EnetAppUtils_print("getHostRxDscpPriority() ENET_HOSTPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP failed: %d\n",
                           status);
    }

    return status;
}

/* Packet priority to header packet priority map */
static int32_t EnetTestPriorityRegeneration_remapHostRxDscpPriority(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *testStateObj = &taskObj->stateObj;
    EnetPort_DscpPriorityMap setHostInArgs;
    Enet_IoctlPrms prms;
    uint32_t i;
    int32_t status;

    setHostInArgs.dscpIPv4En = BTRUE;
    setHostInArgs.dscpIPv6En = BTRUE;

    for (i = 0U; i < ENET_ARRAYSIZE(setHostInArgs.tosMap); i++)
    {
        setHostInArgs.tosMap[i] = (i % 8U);
    }

    ENET_IOCTL_SET_IN_ARGS(&prms, &setHostInArgs);

    status = Enet_ioctl(testStateObj->hEnet,
                        testStateObj->coreId,
                        ENET_HOSTPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("remapHostRxDscpPriority() ENET_HOSTPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP failed: %d\n",
                           status);
    }

    return status;
}

/* Get MAC port IPDSCP Rx Packet priority */
static int32_t EnetTestPriorityRegeneration_getMacRxDscpPriority(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *testStateObj = &taskObj->stateObj;
    EnetTestMacPortList_t enabledPorts;
    EnetMacPort_GenericInArgs getMacInArgs;
    EnetPort_DscpPriorityMap getMacOutArgs;
    Enet_IoctlPrms prms;
    uint32_t i;
    int32_t status = ENET_SOK;

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
    for (i = 0U; i < enabledPorts.numMacPorts; i++)
    {
        getMacInArgs.macPort = enabledPorts.macPortList[i];
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &getMacInArgs, &getMacOutArgs);

        status = Enet_ioctl(testStateObj->hEnet,
                            testStateObj->coreId,
                            ENET_MACPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP,
                            &prms);
        if (status == ENET_SOK)
        {
            EnetTestPriorityRegeneration_validateTosMap(getMacOutArgs.tosMap, CPSW_ALE_MACPORT_TO_ALEPORT(enabledPorts.macPortList[i]));
        }
        else
        {
           EnetAppUtils_print("getMacRxDscpPriorit() ENET_MACPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP failed: %d\n",
                               status);
        }
    }

    return status;
}

/* Packet priority to header packet priority map */
static int32_t EnetTestPriorityRegeneration_remapMacRxDscpPriority(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *testStateObj = &taskObj->stateObj;
    EnetTestMacPortList_t enabledPorts;
    EnetMacPort_SetIngressDscpPriorityMapInArgs setMacInArgs;
    Enet_IoctlPrms prms;
    uint32_t i;
    uint32_t j;
    int32_t status = ENET_SOK;

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);

    /* Set Mac Port DSCP priority mapping for all mac ports */
    for (i = 0U; i < enabledPorts.numMacPorts; i++)
    {
        setMacInArgs.macPort                     = enabledPorts.macPortList[i];
        setMacInArgs.dscpPriorityMap.dscpIPv4En = BTRUE;
        setMacInArgs.dscpPriorityMap.dscpIPv6En = BTRUE;

        for (j = 0U; j < ENET_ARRAYSIZE(setMacInArgs.dscpPriorityMap.tosMap); j++)
        {
            setMacInArgs.dscpPriorityMap.tosMap[j] = (j % ENET_PRI_NUM);
        }

        ENET_IOCTL_SET_IN_ARGS(&prms, &setMacInArgs);

        status = Enet_ioctl(testStateObj->hEnet,
                            testStateObj->coreId,
                            ENET_MACPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("remapMacRxDscpPriority() ENET_MACPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP failed: %d\n",
                               status);
        }
    }

    return status;
}

/* Get Host port Header packet priority */
static int32_t EnetTestPriorityRegeneration_getHostTxPriority(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *testStateObj = &taskObj->stateObj;
    EnetPort_PriorityMap getHostOutArgs;
    Enet_IoctlPrms prms;
    int32_t status;

    ENET_IOCTL_SET_OUT_ARGS(&prms, &getHostOutArgs);

    status = Enet_ioctl(testStateObj->hEnet,
                        testStateObj->coreId,
                        ENET_HOSTPORT_IOCTL_GET_EGRESS_QOS_PRI_MAP,
                        &prms);
    if (status == ENET_SOK)
    {
        EnetTestPriorityRegeneration_validateHdrPktPri2SwitchPri(getHostOutArgs.priorityMap, CPSW_ALE_HOST_PORT_NUM);
    }
    else
    {
       EnetAppUtils_print("getHostTxPriority() ENET_HOSTPORT_IOCTL_GET_EGRESS_QOS_PRI_MAP failed: %d\n",
                           status);
    }

    return status;
}

/* Header packet priority to switch priority map */
static int32_t EnetTestPriorityRegeneration_remapHostTxPriority(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *testStateObj = &taskObj->stateObj;
    EnetPort_PriorityMap setHostInArgs;
    Enet_IoctlPrms prms;
    int32_t status;

    /* Copy the test "Header Packet Priority" to "Switch Priority" mapping */
    memcpy(setHostInArgs.priorityMap,
           mapHostHdrPktPri2SwitchPri,
           sizeof(mapHostHdrPktPri2SwitchPri));
    ENET_IOCTL_SET_IN_ARGS(&prms, &setHostInArgs);

    status = Enet_ioctl(testStateObj->hEnet,
                        testStateObj->coreId,
                        ENET_HOSTPORT_IOCTL_SET_EGRESS_QOS_PRI_MAP,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("remapHostTxPriority() ENET_HOSTPORT_IOCTL_SET_EGRESS_QOS_PRI_MAP failed: %d\n",
                           status);
    }

    return status;
}

/* Get MAC port Header packet priority */
static int32_t EnetTestPriorityRegeneration_getMacTxPriority(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *testStateObj = &taskObj->stateObj;
    EnetTestMacPortList_t enabledPorts;
    EnetMacPort_GenericInArgs getMacInArgs;
    EnetPort_PriorityMap getMacOutArgs;
    Enet_IoctlPrms prms;
    uint32_t i;
    int32_t status = ENET_SOK;

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);

    for (i = 0U; i < enabledPorts.numMacPorts; i++)
    {
        getMacInArgs.macPort = enabledPorts.macPortList[i];
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &getMacInArgs, &getMacOutArgs);

        status = Enet_ioctl(testStateObj->hEnet,
                            testStateObj->coreId,
                            ENET_MACPORT_IOCTL_GET_EGRESS_QOS_PRI_MAP,
                            &prms);
        if (status == ENET_SOK)
        {
            EnetTestPriorityRegeneration_validateHdrPktPri2SwitchPri(getMacOutArgs.priorityMap, CPSW_ALE_MACPORT_TO_ALEPORT(enabledPorts.macPortList[i]));
        }
        else
        {
           EnetAppUtils_print("getMacTxPriority() ENET_MACPORT_IOCTL_GET_EGRESS_QOS_PRI_MAP failed: %d\n",
                               status);
        }
    }

    return status;
}

/* Header packet priority to switch priority map */
static int32_t EnetTestPriorityRegeneration_remapMacTxPriority(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *testStateObj = &taskObj->stateObj;
    EnetTestMacPortList_t enabledPorts;
    EnetMacPort_SetEgressPriorityMapInArgs setMacInArgs;
    Enet_MacPort portNum;
    Enet_IoctlPrms prms;
    uint32_t i;
    int32_t status = ENET_SOK;

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);

    /* Set "Header Packet Priority" to "Switch Priority" mapping for all MAC ports */
    for (i = 0U; i < enabledPorts.numMacPorts; i++)
    {
        portNum = enabledPorts.macPortList[i];

        /* Copy the test "Header Packet Priority" to "Switch Priority" mapping */
        memcpy(setMacInArgs.priorityMap.priorityMap,
               mapHdrPktPri2SwitchPri[portNum],
               sizeof(mapHdrPktPri2SwitchPri[portNum]));
        setMacInArgs.macPort = portNum;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setMacInArgs);

        status = Enet_ioctl(testStateObj->hEnet,
                            testStateObj->coreId,
                            ENET_MACPORT_IOCTL_SET_EGRESS_QOS_PRI_MAP,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("remapMacTxPriority() ENET_MACPORT_IOCTL_SET_EGRESS_QOS_PRI_MAP failed: %d\n",
                               status);
        }
    }

    return status;
}

static int32_t EnetTestPriorityRegeneration_transmitPkts(EnetTestTaskObj *taskObj,
                                                         EnetDma_PktQ *pTxSubmitQ,
                                                         uint32_t txCfgIndex)
{
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t txRetrievePktCnt;
    uint32_t txCnt;
    int32_t status = ENET_SOK;

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
               EnetAppUtils_print("failed to retrieve consumed transmit packets: %d\n", status);
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

static int32_t EnetTestPriorityRegeneration_xmitTest(EnetTestTaskObj *taskObj,
                                                     uint32_t txCfgIndex,
                                                     uint32_t num,
                                                     const uint8_t *dstAddr,
                                                     const uint8_t *srcAddr,
                                                     uint16_t pcp,
                                                     uint16_t vid,
                                                     uint16_t etherType,
                                                     uint16_t length)
{
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint16_t len               = length - ETH_TEST_DATA_HDR_LEN;
    uint32_t pktCnt            = 0U;
    EthVlanFrame *frame;
    DataFramePayload *payload;
    EnetDma_Pkt *pktInfo;
    EnetDma_PktQ txSubmitQ;
    EnetDma_PktQ *txFreePktInfoQ = &stateObj->txChObj[txCfgIndex].txFreePktInfoQ;
    int32_t status                   = ENET_SOK;

    while (pktCnt < num)
    {
        EnetQueue_initQ(&txSubmitQ);

        /* Dequeue one free TX Eth packet */
        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(txFreePktInfoQ);

        while (NULL != pktInfo)
        {
            /* Fill the TX Eth frame with test content */
            frame = (EthVlanFrame *)pktInfo->sgList.list[0].bufPtr;
            memcpy(frame->hdr.dstMac, bcastAddr, ENET_MAC_ADDR_LEN);
            memcpy(frame->hdr.srcMac, &stateObj->hostMacAddr[0U], ENET_MAC_ADDR_LEN);
            frame->hdr.tpid      = Enet_htons(ETHERTYPE_VLAN_TAG);
            frame->hdr.tci       = Enet_htons((pcp << 13) | vid);
            frame->hdr.etherType = Enet_htons(etherType);

            payload       = (DataFramePayload *)frame->payload;
            payload->type = 0U; /* DataPattern1 */
            payload->len  = Enet_htons(len);
            memcpy(payload->data, Enet_DataPattern1, len);

            pktInfo->sgList.list[0].segmentFilledLen = length + sizeof(EthVlanFrameHeader);
            pktInfo->appPriv    = &stateObj;

            /* Enqueue the packet for later transmission */
            EnetQueue_enq(&txSubmitQ, &pktInfo->node);

            pktCnt++;
            if (pktCnt >= num)
            {
                break;
            }

            /* Dequeue one free TX Eth packet */
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(txFreePktInfoQ);
        }

        status = EnetTestPriorityRegeneration_transmitPkts(taskObj, &txSubmitQ, txCfgIndex);
    }

    return status;
}

static void EnetTestPriorityRegeneration_transmitTest(EnetTestTaskObj *taskObj,
                                                      uint32_t txCfgIndex,
                                                      uint16_t pcp)
{
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t count             = 5U;
    uint16_t len               = 500U;
    uint32_t status;

    EnetTest_wait(10U);

    /* Transmit frames */
   EnetAppUtils_print("transmitTest: transmitting %d packets with PCP %u\n", count, pcp);

    status = EnetTestPriorityRegeneration_xmitTest(taskObj,
                                                   txCfgIndex,
                                                   count,
                                                   bcastAddr,
                                                   &stateObj->hostMacAddr[0U],
                                                   pcp,
                                                   ETH_TEST_VLAN_VID,
                                                   ETHERTYPE_EXPERIMENTAL1,
                                                   len);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("transmitTest: failed to transmit %d packets\n", count);
    }
}
