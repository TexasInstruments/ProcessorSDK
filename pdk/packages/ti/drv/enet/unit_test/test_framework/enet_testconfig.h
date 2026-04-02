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

/**
 *  \file enet_testconfig.h
 *
 *  \brief This file defines the common configurations like driver config etc...
 */

#ifndef ENET_TEST_CONFIG_H_
#define ENET_TEST_CONFIG_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "enet_test.h"
#include "enet_test_base.h"
#include "cpsw_test_basicswitching.h"
#include "cpsw_test_autolearn.h"
#include "cpsw_test_stats.h"
#include "cpsw_test_vlan.h"
#include "cpsw_test_autolearn_vlan.h"
#include "cpsw_test_aging_maxlength.h"
#include "cpsw_test_crcstrip.h"
#include "cpsw_test_fifostats.h"
#include "cpsw_test_ale_srcaddr_update.h"
#include "cpsw_test_ale_tablefull.h"
#include "cpsw_test_vlan_dropuntagged.h"
#include "cpsw_test_multicast.h"
#include "cpsw_test_policer.h"
#include "cpsw_test_network_security.h"
#include "cpsw_test_hostport_rxfilter.h"
#include "cpsw_test_policer_nomatch.h"
#include "cpsw_test_intervlan.h"
#include "cpsw_test_pvid.h"
#include "cpsw_test_sanity.h"
#include "cpsw_test_outer_vlan.h"
#include "cpsw_test_traffic_shaping.h"
#include "cpsw_test_nway.h"
#include "cpsw_test_rxflow_mtu.h"
#include "cpsw_test_phystrapping.h"
#include "cpsw_test_priority_regeneration.h"
#include "cpsw_test_bcastmcast_limit.h"
#include "cpsw_test_directed_pkt.h"

#if defined(SOC_J721E) || defined(SOC_J7200)
#include "cpsw_test_default_priority.h"
#include "cpsw_test_sgmii_lpbk.h"
#include "cpsw_test_cpts_event.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/* Numbe of task configuration available */
#define ENET_TEST_NUM_TASK_CFG_PARAMS           ((sizeof(gTestTaskCfg)) / \
                                                 (sizeof(EnetTestTaskCfg)))

#define ENET_TEST_NUM_TX_CH_CFG_PARAMS          ((sizeof(gTestTxChCfg)) / \
                                                 (sizeof(EnetTestTxChCfgInfo)))

#define ENET_TEST_NUM_RX_FLOW_CFG_PARAMS        ((sizeof(gTestRxFlowCfg)) / \
                                                 (sizeof(EnetTestRxFlowCfgInfo)))

/* IDs for test task configuration */
#define TASK_CFG_ID_SWITCH_TEST                     ((uint32_t)0U)
#define TASK_CFG_ID_ALE_AUTO_LEARN_TEST             ((uint32_t)1U)
#define TASK_CFG_ID_ALE_AUTO_LEARN_VLAN_TEST        ((uint32_t)2U)
#define TASK_CFG_ID_MAC_SPEED_TEST                  ((uint32_t)3U)
#define TASK_CFG_ID_STATS_TEST                      ((uint32_t)4U)
#define TASK_CFG_ID_VLAN_TEST                       ((uint32_t)5U)
#define TASK_CFG_ID_AGING_MAXLEN_TEST               ((uint32_t)6U)
#define TASK_CFG_ID_CRCSTRIP_TEST                   ((uint32_t)7U)
#define TASK_CFG_ID_FIFOSTATS_TEST                  ((uint32_t)8U)
#define TASK_CFG_ID_SRCADDRUPDATE_TEST              ((uint32_t)9U)
#define TASK_CFG_ID_TABLEFULL_TEST                  ((uint32_t)10U)
#define TASK_CFG_ID_VLAN_DROP_UNTAGGED_TEST         ((uint32_t)11U)
#define TASK_CFG_ID_MULTICAST_TEST                  ((uint32_t)12U)
#define TASK_CFG_ID_POLICER_TEST                    ((uint32_t)13U)
#define TASK_CFG_ID_SECURITY_TEST                   ((uint32_t)14U)
#define TASK_CFG_ID_RXFILTER_TEST                   ((uint32_t)15U)
#define TASK_CFG_ID_POLICER_NOMATCH_TEST            ((uint32_t)16U)
#define TASK_CFG_ID_INTERVLAN_TEST                  ((uint32_t)17U)
#define TASK_CFG_ID_DEFAULT_PRI_TEST                ((uint32_t)18U)
#define TASK_CFG_ID_PVID_TEST                       ((uint32_t)19U)
#define TASK_CFG_ID_SANITY_TEST                     ((uint32_t)20U)
#define TASK_CFG_ID_OUTERVLAN_TEST                  ((uint32_t)21U)
#define TASK_CFG_ID_TRAFFICSHAPING_TEST             ((uint32_t)22U)
#define TASK_CFG_ID_AUTONEGOTIATION_1G_FD_TEST      ((uint32_t)23U)
#define TASK_CFG_ID_AUTONEGOTIATION_100M_FD_TEST    ((uint32_t)24U)
#define TASK_CFG_ID_AUTONEGOTIATION_100M_HD_TEST    ((uint32_t)25U)
#define TASK_CFG_ID_RXFLOW_MTU_TEST                 ((uint32_t)26U)
#define TASK_CFG_ID_PHY_STRAPPING_TEST              ((uint32_t)27U)
#define TASK_CFG_ID_PRI_REGEN_TEST                  ((uint32_t)28U)
#define TASK_CFG_ID_SGMII_LPBK_TEST                 ((uint32_t)29U)
#define TASK_CFG_ID_QSGMII_SWITCHING_TEST           ((uint32_t)30U)
#define TASK_CFG_ID_ALL_PORTS_SWITCH_TEST           ((uint32_t)31U)
#define TASK_CFG_ID_CPTS_EVENT_TEST                 ((uint32_t)32U)
#define TASK_CFG_ID_BCASTMCAST_LIMIT_TEST           ((uint32_t)33U)
#define TASK_CFG_ID_DIRECTED_PKT_TEST               ((uint32_t)34U)

/* IDs for Packet Tx  configuration */
#define TXCH_CFG_ID_COMMON_TEST                      ((uint32_t)0U)
#define TXCH_CFG_ID_STATS_TEST                       ((uint32_t)1U)
#define TXCH_CFG_ID_CRCSTRIP_TEST                    ((uint32_t)2U)
#define TXCH_CFG_ID_SRCADDRUPDATE_TEST               ((uint32_t)3U)
#define TXCH_CFG_ID_TABLEFULL_TEST                   ((uint32_t)4U)
#define TXCH_CFG_ID_FIFOSTATS_TEST                   ((uint32_t)5U)
#define TXCH_CFG_ID_POLICER_TEST                     ((uint32_t)6U)
#define TXCH_CFG_ID_INTERVLAN_TEST                   ((uint32_t)7U)
#define TXCH_CFG_ID_DEFAULT_PRI_TEST                 ((uint32_t)8U)
#define TXCH_CFG_ID_RXFLOW_MTU_TEST                  ((uint32_t)9U)
#define TXCH_CFG_ID_CPTS_EVENT_TEST                  ((uint32_t)10U)
#define TXCH_CFG_ID_DIRECTED_PKT_TEST                ((uint32_t)11U)

/* IDs for Packet Rx  configuration */
#define RXFLOW_CFG_ID_COMMON_TEST                       ((uint32_t)0U)
#define RXFLOW_CFG_ID_SWITCHING_TEST                    ((uint32_t)1U)
#define RXFLOW_CFG_ID_CRCSTRIP_TEST                     ((uint32_t)2U)
#define RXFLOW_CFG_ID_FIFOSTATS_TEST                    ((uint32_t)3U)
#define RXFLOW_CFG_ID_MULTICAST_TEST                    ((uint32_t)4U)
#define RXFLOW_CFG_ID_STATS_TEST                        ((uint32_t)5U)
#define RXFLOW_CFG_ID_SECURITY_TEST                     ((uint32_t)6U)
#define RXFLOW_CFG_ID_RXFILTER_TEST                     ((uint32_t)7U)
#define RXFLOW_CFG_ID_POLICER1_TEST                     ((uint32_t)8U)
#define RXFLOW_CFG_ID_POLICER2_TEST                     ((uint32_t)9U)
#define RXFLOW_CFG_ID_POLICER_NOMATCH_TEST              ((uint32_t)10U)
#define RXFLOW_CFG_ID_INTERVLAN_TEST                    ((uint32_t)11U)
#define RXFLOW_CFG_ID_DEFAULT_PRI_TEST                  ((uint32_t)12U)
#define RXFLOW_CFG_ID_NONDEFAULT_POLICER_NOMATCH_TEST   ((uint32_t)13U)
#define RXFLOW_CFG_ID_PVID_TEST                         ((uint32_t)14U)
#define RXFLOW_CFG_ID_OUTERVLAN_TEST                    ((uint32_t)15U)
#define RXFLOW_CFG_ID_RXFLOW_MTU_TEST                   ((uint32_t)16U)
#define RXFLOW_CFG_ID_CPTS_EVENT_TEST                   ((uint32_t)17U)
#define RXFLOW_CFG_ID_DIRECTED_PKT_TEST                 ((uint32_t)18U)

#define ENET_TEST_ALL_MACPORT_MASK (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)) | \
                                    ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_2)) | \
                                    ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_3)) | \
                                    ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)) | \
                                    ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_5)) | \
                                    ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_6)) | \
                                    ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_7)) | \
                                    ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_8)))

#define ENET_TEST_EVM_QSGMII_MACPORT_MASK (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_5)) | \
                                       ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_6)) | \
                                       ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_7)))

#define ENET_TEST_EVM_RGMII_MACPORT_MASK (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)) | \
                                      ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_3)) | \
                                      ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)) | \
                                      ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_8)))

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
static inline EnetTestTaskCfg *Test_getTaskCfgParams(uint32_t taskCfgId);

static inline EnetTestTxChCfgInfo *Test_getTxChCfgParams(uint32_t txChCfgId);

static inline EnetTestRxFlowCfgInfo *Test_getRxFlowCfgParams(uint32_t rxFlowCfgId);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

EnetTestTaskCfg gTestTaskCfg[] =
{
    /* Switch logic test */
    {
        .taskCfgId          = TASK_CFG_ID_SWITCH_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_SWITCHING_TEST},
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = NULL,
        .runTest            = &EnetTestSwitching_RunWithIPG,
        .deInitTest         = NULL,
        .taskId             = 0U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .macPortMask        = (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)) |
                               ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_3))),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* ALE auto learn */
    {
        .taskCfgId          = TASK_CFG_ID_ALE_AUTO_LEARN_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 1U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestAutoLearn_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestAutoLearn_updatePortLinkCfg,
        .runTest            = &EnetTestAutoLearn_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* ALE auto learn VLAN*/
    {
        .taskCfgId          = TASK_CFG_ID_ALE_AUTO_LEARN_VLAN_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 2U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestAutoLearnVlan_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestAutoLearnVlan_updatePortLinkCfg,
        .runTest            = &EnetTestAutoLearnVlan_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Mac Speed test */
    {
        .taskCfgId          = TASK_CFG_ID_MAC_SPEED_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 3U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = &EnetTestSanity_updatePortLinkCfg100Mbps,
        .runTest            = &EnetTestMacSpeed_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Statistics test */
    {
        .taskCfgId          = TASK_CFG_ID_STATS_TEST,
        .txChCfgId          = {TXCH_CFG_ID_STATS_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_STATS_TEST},
        .taskId             = 4U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = &EnetTestStats_updatePortLinkCfg,
        .runTest            = &EnetTestCommon_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Vlan test */
    {
        .taskCfgId          = TASK_CFG_ID_VLAN_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 5U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestVlan_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestVlan_updatePortLinkCfg,
        .runTest            = &EnetTestVlan_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Aging and Max Packet length */
    {
        .taskCfgId          = TASK_CFG_ID_AGING_MAXLEN_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 6U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestAgingMaxPktLength_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestAgingMaxPktLength_updatePortLinkCfg,
        .runTest            = &EnetTestAgingMaxPktLength_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_2)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* CRC Strip */
    {
        .taskCfgId          = TASK_CFG_ID_CRCSTRIP_TEST,
        .txChCfgId          = {TXCH_CFG_ID_CRCSTRIP_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_CRCSTRIP_TEST},
        .taskId             = 7U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestCrcStrip_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestCrcStrip_updatePortLinkCfg,
        .runTest            = &EnetTestCommon_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* FIFO Stats */
    {
        .taskCfgId          = TASK_CFG_ID_FIFOSTATS_TEST,
        .txChCfgId          = {TXCH_CFG_ID_FIFOSTATS_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_FIFOSTATS_TEST},
        .taskId             = 8U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = &EnetTestFifoStats_updatePortLinkCfg,
        .runTest            = &EnetTestFifoStats_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Source Address update */
    {
        .taskCfgId          = TASK_CFG_ID_SRCADDRUPDATE_TEST,
        .txChCfgId          = {TXCH_CFG_ID_SRCADDRUPDATE_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 9U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestAleSrcAddrUpdate_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestAleSrcAddrUpdate_updatePortLinkCfg,
        .runTest            = &EnetTestAleSrcAddrUpdate_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* ALE Table full */
    {
        .taskCfgId          = TASK_CFG_ID_TABLEFULL_TEST,
        .txChCfgId          = {TXCH_CFG_ID_TABLEFULL_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 10U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestAleTableFull_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestAleTableFull_updatePortLinkCfg,
        .runTest            = &EnetTestAleTableFull_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* VALN Drop untagged */
    {
        .taskCfgId          = TASK_CFG_ID_VLAN_DROP_UNTAGGED_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 11U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestVlanDropUntagged_setOpenPrms,
        .portLinkPrmsUpdate = NULL,
        .runTest            = &EnetTestCommon_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Multicast support */
    {
        .taskCfgId          = TASK_CFG_ID_MULTICAST_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_MULTICAST_TEST},
        .taskId             = 12U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestMulticast_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestMulticast_updatePortLinkCfg,
        .runTest            = &EnetTestMulticast_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Policer support */
    {
        .taskCfgId = TASK_CFG_ID_POLICER_TEST,
        .txChCfgId =
        {
            [ENETTESTPOLICER_DEFAULT_TXCHCFGID] = TXCH_CFG_ID_COMMON_TEST,
            [ENETTESTPOLICER_TYPE1_TXCHCFGID]   = TXCH_CFG_ID_POLICER_TEST,
            [ENETTESTPOLICER_TYPE2_TXCHCFGID]   = TXCH_CFG_ID_COMMON_TEST
        },
        .rxFlowCfgId =
        {
            [ENETTESTPOLICER_DEFAULT_RXFLOWCFGID] = RXFLOW_CFG_ID_POLICER1_TEST,
            [ENETTESTPOLICER_TYPE1_RXFLOWCFGID]   = RXFLOW_CFG_ID_POLICER2_TEST,
            [ENETTESTPOLICER_TYPE2_RXFLOWCFGID]   = RXFLOW_CFG_ID_POLICER2_TEST,
            [ENETTESTPOLICER_TYPE3_RXFLOWCFGID]   = RXFLOW_CFG_ID_POLICER2_TEST,
        },
        .taskId             = 13U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestPolicer_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestPolicer_updatePortLinkCfg,
        .runTest            = &EnetTestPolicer_Run,
        .deInitTest         = NULL,
        .macPortMask        = (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)) |
                               ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_3))),
        .numTxCh            = 3U,
        .numRxFlow          = 4U,
    },
    /* Network security */
    {
        .taskCfgId          = TASK_CFG_ID_SECURITY_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_SECURITY_TEST},
        .taskId             = 14U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestSecurity_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestSecurity_updatePortLinkCfg,
        .runTest            = &EnetTestSecurity_Run,
        .deInitTest         = NULL,
        .macPortMask        = (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)) |
                               ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_3))),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Host port Rx Filter */
    {
        .taskCfgId          = TASK_CFG_ID_RXFILTER_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_RXFILTER_TEST},
        .taskId             = 15U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestRxFilter_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestRxFilter_updatePortLinkCfg,
        .runTest            = &EnetTestRxFilter_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Policer nomatch */
    {
        .taskCfgId   = TASK_CFG_ID_POLICER_NOMATCH_TEST,
        .txChCfgId   = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId =
        {
            [ENETTESTPOLICER_NOMATCH_DEFAULT_RXFLOWCFGID] = RXFLOW_CFG_ID_POLICER_NOMATCH_TEST,
            [ENETTESTPOLICER_NOMATCH_TYPE1_RXFLOWCFGID]   = RXFLOW_CFG_ID_NONDEFAULT_POLICER_NOMATCH_TEST
        },
        .taskId             = 16U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestPolicerNomatch_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestPolicerNomatch_updatePortLinkCfg,
        .runTest            = &EnetTestPolicerNomatch_Run,
        .deInitTest         = NULL,
        .macPortMask        = (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)) |
                               ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_3))),
        .numTxCh            = 1U,
        .numRxFlow          = 2U,
    },
    /* Intervlan  */
    {
        .taskCfgId          = TASK_CFG_ID_INTERVLAN_TEST,
        .txChCfgId          = {TXCH_CFG_ID_INTERVLAN_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_INTERVLAN_TEST},
        .taskId             = 17U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestInterVlan_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestInterVlan_updatePortLinkCfg,
        .runTest            = &EnetTestInterVlan_Run,
        .deInitTest         = NULL,
        .macPortMask        = (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)) |
                               ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_3))),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
#if defined(SOC_J721E)
    /* Default Priority test  */
    {
        .taskCfgId = TASK_CFG_ID_DEFAULT_PRI_TEST,
        .txChCfgId =
        {
            [ENETTESTDEFAULTPRIORITY_DEFAULTCHID] = TXCH_CFG_ID_COMMON_TEST,
            [ENETTESTDEFAULTPRIORITY1_TXCHID]     = TXCH_CFG_ID_DEFAULT_PRI_TEST,
            [ENETTESTDEFAULTPRIORITY2_TXCHID]     = TXCH_CFG_ID_DEFAULT_PRI_TEST,
            [ENETTESTDEFAULTPRIORITY3_TXCHID]     = TXCH_CFG_ID_DEFAULT_PRI_TEST,
        },
        .rxFlowCfgId =
        {
            [ENETTESTDEFAULTPRIORITY_DEFAULTFLOWID] = RXFLOW_CFG_ID_COMMON_TEST,
            [ENETTESTDEFAULTPRIORITY1_RXFLOWID]     = RXFLOW_CFG_ID_DEFAULT_PRI_TEST,
            [ENETTESTDEFAULTPRIORITY2_RXFLOWID]     = RXFLOW_CFG_ID_DEFAULT_PRI_TEST,
            [ENETTESTDEFAULTPRIORITY3_RXFLOWID]     = RXFLOW_CFG_ID_DEFAULT_PRI_TEST,
        },
        .taskId             = 18U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestDefaultPriority_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestDefaultPriority_updatePortLinkCfg,
        .runTest            = &EnetTestDefaultPriority_Run,
        .deInitTest         = NULL,
        .macPortMask        = (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)) |
                               ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_3))),
        .numTxCh            = 4U,
        .numRxFlow          = 4U,
    },
#endif
    /* Port Vlan Id  */
    {
        .taskCfgId          = TASK_CFG_ID_PVID_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_PVID_TEST},
        .taskId             = 19U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestPvid_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestPvid_updatePortLinkCfg,
        .runTest            = &EnetTestPvid_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Sanity Test */
    {
        .taskCfgId          = TASK_CFG_ID_SANITY_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 20U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = &EnetTestSanity_updatePortLinkCfgAutoNeg,
        .runTest            = &EnetTestSanity_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Outer VLAN */
    {
        .taskCfgId          = TASK_CFG_ID_OUTERVLAN_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_OUTERVLAN_TEST},
        .taskId             = 21U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestOuterVlan_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestOuterVlan_updatePortLinkCfg,
        .runTest            = &EnetTestOuterVlan_Run,
        .deInitTest         = NULL,
        .macPortMask        = (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)) |
                               ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_3))),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Traffic shaping */
    {
        .taskCfgId          = TASK_CFG_ID_TRAFFICSHAPING_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 22U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestTrafficShaping_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestTrafficShaping_updatePortLinkCfg,
        .runTest            = &EnetTestTrafficShaping_Run,
        .deInitTest         = NULL,
        .macPortMask        = (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)) |
                               ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_3))),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* 1-Gbps full-duplex auto-negotiation test */
    {
        .taskCfgId          = TASK_CFG_ID_AUTONEGOTIATION_1G_FD_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 23U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = &EnetTestNway_updatePortLinkCfg1GFd,
        .runTest            = &EnetTestNway_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* 100-Mbps full-duplex auto-negotiation test */
    {
        .taskCfgId          = TASK_CFG_ID_AUTONEGOTIATION_100M_FD_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 24U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = &EnetTestNway_updatePortLinkCfg100MFd,
        .runTest            = &EnetTestNway_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* 100-Mbps half-duplex auto-negotiation test */
    {
        .taskCfgId          = TASK_CFG_ID_AUTONEGOTIATION_100M_HD_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 25U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = &EnetTestNway_updatePortLinkCfg100MHd,
        .runTest            = &EnetTestNway_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* PDK-3723 - DMA Rx flow Max. length test - RX Flow to drop packet if exceed rxFlowMtu */

    /* In this test, we configure port in loopback mode and from host port send alternate packets
     * with correct RX flow MTU & packets greater than RX flow MTU. Since packets > RX flow MTU gets
     * dropped in drop ring using "RX flow size threshold" configuration, we expect to receive only
     * half packets. We mark test passed if we don't receive all packets transmitted */
    {
        .taskCfgId          = TASK_CFG_ID_RXFLOW_MTU_TEST,
        .txChCfgId          = {TXCH_CFG_ID_RXFLOW_MTU_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_RXFLOW_MTU_TEST},
        .taskId             = 26U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = EnetTestRxFlowMtu_updatePortLinkCfg,
        .runTest            = EnetTestCommon_Run, // &EnetTestRxFlowMtu_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* PDK-3722 - PHY strapping mode - PHY LLD to bypass the some states in FSM for faster link up */
    {
        .taskCfgId          = TASK_CFG_ID_PHY_STRAPPING_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 27U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestPhyStrap_openPrmsUpdate,
        .portLinkPrmsUpdate = &EnetTestPhyStrap_updatePortLinkCfg,
        .runTest            = &EnetTestCommon_Run,
        .deInitTest         = NULL,

        /* We use MAC PORT 7 with RMII PHY for strap test as this PHY doesn't need any additional
         * configuration via extendedConfig. We can't use any of RGMII PHY's as they need this extendedConfig
         * for delay, drive strength settings */
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_8)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Priority Regeneration test  */
    {
        .taskCfgId = TASK_CFG_ID_PRI_REGEN_TEST,
        .txChCfgId = {[ENETTESTPRIORITYREGENERATION_DEFAULTCHID] = TXCH_CFG_ID_COMMON_TEST, },
        .rxFlowCfgId = {[ENETTESTPRIORITYREGENERATION_DEFAULTFLOWID] = RXFLOW_CFG_ID_COMMON_TEST, },
        .taskId             = 28U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestPriorityRegeneration_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestPriorityRegeneration_setMacConfig,
        .runTest            = &EnetTestPriorityRegeneration_run,
        .deInitTest         = NULL,
        .macPortMask        = (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)) |
                               ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_3))),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
#if defined(SOC_J721E)
    /* PDK- - SGMII loopback test - SGMII internal/digital loopback(before the SERDES) from the CPSGMII
     * transmit to the CPSGMII receive */
    {
        .taskCfgId          = TASK_CFG_ID_SGMII_LPBK_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 29U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = EnetTestSgmiiLpbk_updatePortLinkCfg,
        .runTest            = &EnetTestCommon_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_2)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
#endif
    /* Switch logic test for QSGMII DB (ENET_EXP) ports*/
    {
        .taskCfgId          = TASK_CFG_ID_QSGMII_SWITCHING_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_SWITCHING_TEST},
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = NULL,
        .runTest            = &EnetTestSwitching_Run,
        .deInitTest         = NULL,
        .taskId             = 30U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .macPortMask        = ENET_TEST_EVM_QSGMII_MACPORT_MASK,
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
    /* Switch logic test with all 8 ports*/
    {
        .taskCfgId          = TASK_CFG_ID_ALL_PORTS_SWITCH_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_SWITCHING_TEST},
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = NULL,
        .runTest            = &EnetTestSwitching_Run,
        .deInitTest         = NULL,
        .taskId             = 31U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .macPortMask        = ENET_TEST_ALL_MACPORT_MASK,
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
#if defined(SOC_J721E)
    /* CPTS Event test */
    {
        .taskCfgId          = TASK_CFG_ID_CPTS_EVENT_TEST,
        .txChCfgId          = {TXCH_CFG_ID_CPTS_EVENT_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_CPTS_EVENT_TEST},
        .taskId             = 32U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestCptsEvent_setOpenPrms,
        .portLinkPrmsUpdate = &EnetTestCptsEvent_updatePortLinkCfg,
        .runTest            = &EnetTestCptsEvent_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_1)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
#endif
    /* BCast/MCast Rate Limit test  */
    {
        .taskCfgId          = TASK_CFG_ID_BCASTMCAST_LIMIT_TEST,
        .txChCfgId          = {TXCH_CFG_ID_COMMON_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_COMMON_TEST},
        .taskId             = 33U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = &EnetTestBcastMcastLimit_setOpenPrms,
        .portLinkPrmsUpdate = NULL,
        .runTest            = &EnetTestBcastMcastLimit_Run,
        .deInitTest         = NULL,
        .macPortMask        = (ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)) |
                               ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_3))),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
#if defined(SOC_J721E)
    /* Directed packet test  */
    {
        .taskCfgId          = TASK_CFG_ID_DIRECTED_PKT_TEST,
        .txChCfgId          = {TXCH_CFG_ID_DIRECTED_PKT_TEST},
        .rxFlowCfgId        = {RXFLOW_CFG_ID_DIRECTED_PKT_TEST},
        .taskId             = 34U,
        .enetType           = ENET_CPSW_9G,
        .instId             = 0U,
        .openPrmsUpdate     = NULL,
        .portLinkPrmsUpdate = &EnetTestDirectedPkt_updatePortLinkCfg,
        .runTest            = &EnetTestDirectedPkt_Run,
        .deInitTest         = NULL,
        .macPortMask        = ENET_BIT(ENET_MACPORT_NORM(ENET_MAC_PORT_4)),
        .numTxCh            = 1U,
        .numRxFlow          = 1U,
    },
#endif
};

EnetTestTxChCfgInfo gTestTxChCfg[] =
{
    {
        .txChCfgId        = TXCH_CFG_ID_COMMON_TEST,
        .updateTxChPrms   = NULL,
        .setTxPktInfo     = NULL,
        .maxTxPktLen      = ENET_TEST_COMMON_PKT_LEN,
        .pktSendCount     = ENET_TEST_COMMON_PKT_SENDCOUNT,
        .pktSendLoopCount = ENET_TEST_COMMON_NUM_LOOP,
    },
    {
        .txChCfgId        = TXCH_CFG_ID_STATS_TEST,
        .updateTxChPrms   = NULL,
        .setTxPktInfo     = NULL,
        .maxTxPktLen      = ENET_TEST_COMMON_PKT_LEN,
        .pktSendCount     = ENET_TEST_STATS_PKT_COUNT,
        .pktSendLoopCount = ENET_TEST_COMMON_NUM_LOOP,
    },
    {
        .txChCfgId        = TXCH_CFG_ID_CRCSTRIP_TEST,
        .updateTxChPrms   = NULL,
        .setTxPktInfo     = &EnetTestCrcStrip_setTxPktInfo,
        .maxTxPktLen      = ENET_TEST_COMMON_PKT_LEN,
        .pktSendCount     = ENET_TEST_COMMON_PKT_SENDCOUNT,
        .pktSendLoopCount = ENET_TEST_COMMON_NUM_LOOP,
    },
    {
        .txChCfgId        = TXCH_CFG_ID_SRCADDRUPDATE_TEST,
        .updateTxChPrms   = NULL,
        .setTxPktInfo     = &EnetTestAleSrcAddrUpdate_setTxPktInfo,
        .maxTxPktLen      = ENET_TEST_COMMON_PKT_LEN,
        .pktSendCount     = ENET_TEST_SRCADDRUPDATE_PKT_SENDCOUNT,
        .pktSendLoopCount = ENET_TEST_COMMON_NUM_LOOP,
        .postTxSend       = &EnetTestAleSrcAddrUpdate_postTxSend,
    },
    {
        .txChCfgId        = TXCH_CFG_ID_TABLEFULL_TEST,
        .updateTxChPrms   = NULL,
        .setTxPktInfo     = NULL,
        .maxTxPktLen      = ENET_TEST_COMMON_PKT_LEN,
        .pktSendCount     = ENET_TEST_TABLEFULL_PKT_SENDCOUNT,
        .pktSendLoopCount = ENET_TEST_COMMON_NUM_LOOP,
    },
    {
        .txChCfgId        = TXCH_CFG_ID_FIFOSTATS_TEST,
        .updateTxChPrms   = NULL,
        .setTxPktInfo     = NULL,
        .maxTxPktLen      = ENET_TEST_COMMON_PKT_LEN,
        .pktSendCount     = ENET_TEST_FIFISTATS_PKT_COUNT,
        .pktSendLoopCount = ENET_TEST_COMMON_NUM_LOOP,
    },
    {
        .txChCfgId        = TXCH_CFG_ID_POLICER_TEST,
        .updateTxChPrms   = &EnetTestPolicer_setTxOpenParams,
        .setTxPktInfo     = NULL,
        .maxTxPktLen      = ENET_TEST_COMMON_PKT_LEN,
        .pktSendCount     = ENET_TEST_COMMON_PKT_SENDCOUNT,
        .pktSendLoopCount = ENET_TEST_COMMON_NUM_LOOP,
    },
    {
        .txChCfgId        = TXCH_CFG_ID_INTERVLAN_TEST,
        .updateTxChPrms   = NULL,
        .setTxPktInfo     = NULL,
        .maxTxPktLen      = ENET_TEST_COMMON_PKT_LEN,
        .pktSendCount     = ENET_TEST_INTERVLAN_PKT_SENDCOUNT,
        .pktSendLoopCount = ENET_TEST_COMMON_NUM_LOOP,
    },
    {
        .txChCfgId        = TXCH_CFG_ID_RXFLOW_MTU_TEST,
        .updateTxChPrms   = NULL,
        .setTxPktInfo     = &EnetTestRxFlowMtu_setTxPktInfo,
        .maxTxPktLen      = ENET_TEST_COMMON_PKT_LEN,
        .pktSendCount     = ENET_TEST_RXFLOW_MTU_PKT_SENDCOUNT,
        .pktSendLoopCount = ENET_TEST_COMMON_NUM_LOOP,
    },
#if defined(SOC_J721E)
    {
        .txChCfgId        = TXCH_CFG_ID_DEFAULT_PRI_TEST,
        .updateTxChPrms   = NULL,
        .setTxPktInfo     = &EnetTestDefaultPriority_setTxPktInfo,
        .maxTxPktLen      = ENET_TEST_COMMON_PKT_LEN,
        .pktSendCount     = ENET_TEST_COMMON_PKT_SENDCOUNT,
        .pktSendLoopCount = ENET_TEST_COMMON_NUM_LOOP,
    },
    {
        .txChCfgId        = TXCH_CFG_ID_CPTS_EVENT_TEST,
        .updateTxChPrms   = NULL,
        .setTxPktInfo     = &EnetTestCptsEvent_setTxPktInfo,
        .maxTxPktLen      = ENET_TEST_COMMON_PKT_LEN,
        .pktSendCount     = ENET_TEST_CPTS_EVENT_SENDCOUNT,
        .pktSendLoopCount = ENET_TEST_CPTS_EVENT_NUM_LOOP,
    },
    {
        .txChCfgId        = TXCH_CFG_ID_DIRECTED_PKT_TEST,
        .updateTxChPrms   = NULL,
        .setTxPktInfo     = &EnetTestDirectedPkt_setTxPktInfo,
        .maxTxPktLen      = ENET_TEST_COMMON_PKT_LEN,
        .pktSendCount     = ENET_TEST_DIRECTED_PKT_SENDCOUNT,
        .pktSendLoopCount = ENET_TEST_DIRECTED_PKT_NUM_LOOP,
    },
#endif
};

EnetTestRxFlowCfgInfo gTestRxFlowCfg[] =
{
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_COMMON_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = NULL,
        .pktRecvCount     = ENET_TEST_COMMON_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_SWITCHING_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = NULL,
        .pktRecvCount     = ENET_TEST_SWITCHING_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_CRCSTRIP_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = &EnetTestCrcStrip_processRxPkt,
        .pktRecvCount     = ENET_TEST_COMMON_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_FIFOSTATS_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = &EnetTestFifoStats_processRxPkt,
        .pktRecvCount     = ENET_TEST_FIFISTATS_PKT_COUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_MULTICAST_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = NULL,
        .pktRecvCount     = ENET_TEST_MULTICAST_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_STATS_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = NULL,
        .pktRecvCount     = ENET_TEST_STATS_PKT_COUNT,
        .useDefaultFlow   = BTRUE,
        .postRxProcess    = &EnetTestStats_PostProcessRxPkt,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_SECURITY_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = &EnetTestSecurity_processRxPkt,
        .pktRecvCount     = ENET_TEST_COMMON_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_RXFILTER_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = &EnetTestRxFilter_processRxPkt,
        .pktRecvCount     = ENET_TEST_RXFILTER_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_POLICER1_TEST,
        .updateRxFlowPrms = &EnetTestPolicer_setRxOpenParams,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = NULL,
        .pktRecvCount     = ENET_TEST_POLICER_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_POLICER2_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = NULL,
        .pktRecvCount     = ENET_TEST_POLICER_PKT_RECVCOUNT,
        .useDefaultFlow   = BFALSE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_POLICER_NOMATCH_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = &EnetTestPolicerNomatch_processRxPkt,
        .pktRecvCount     = ENET_TEST_POLICER_NOMATCH_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_NONDEFAULT_POLICER_NOMATCH_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = &EnetTestPolicerNomatch_processRxPkt,
        .pktRecvCount     = ENET_TEST_POLICER_NOMATCH_PKT_RECVCOUNT,
        .useDefaultFlow   = BFALSE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_INTERVLAN_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = &EnetTestInterVlan_processRxPkt,
        .pktRecvCount     = ENET_TEST_INTERVLAN_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_PVID_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = &EnetTestPvid_processRxPkt,
        .pktRecvCount     = ENET_TEST_COMMON_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_OUTERVLAN_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = &EnetTestOuterVlan_processRxPkt,
        .pktRecvCount     = ENET_TEST_OUTERVLAN_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_RXFLOW_MTU_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_RXFLOW_MTU_PKT_LEN,
        .processRxPkt     = NULL,
        /* We drop half of transmitted packets */
        .pktRecvCount     = ENET_TEST_RXFLOW_MTU_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
#if defined(SOC_J721E)
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_DEFAULT_PRI_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = &EnetTestDefaultPriority_processRxPkt,
        .pktRecvCount     = ENET_TEST_COMMON_PKT_RECVCOUNT,
        .useDefaultFlow   = BFALSE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_CPTS_EVENT_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = NULL,
        .pktRecvCount     = ENET_TEST_CPTS_EVENT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
    {
        .rxFlowCfgId      = RXFLOW_CFG_ID_DIRECTED_PKT_TEST,
        .updateRxFlowPrms = NULL,
        .rxFlowMtu        = ENET_TEST_COMMON_PKT_LEN,
        .processRxPkt     = &EnetTestDirectedPkt_processRxPkt,
        .pktRecvCount     = ENET_TEST_DIRECTED_PKT_RECVCOUNT,
        .useDefaultFlow   = BTRUE,
    },
#endif
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
static inline EnetTestTaskCfg *Test_getTaskCfgParams(uint32_t taskCfgId)
{
    uint32_t loopCnt;

    for (loopCnt = 0U; loopCnt < ENET_TEST_NUM_TASK_CFG_PARAMS; loopCnt++)
    {
        if (gTestTaskCfg[loopCnt].taskCfgId == taskCfgId)
        {
            break;
        }
    }

    if (loopCnt < ENET_TEST_NUM_TASK_CFG_PARAMS)
    {
        return(&gTestTaskCfg[loopCnt]);
    }
    else
    {
        return(NULL);
    }
}

static inline EnetTestTxChCfgInfo *Test_getTxChCfgParams(uint32_t txChCfgId)
{
    uint32_t loopCnt;

    for (loopCnt = 0U; loopCnt < ENET_TEST_NUM_TX_CH_CFG_PARAMS; loopCnt++)
    {
        if (gTestTxChCfg[loopCnt].txChCfgId == txChCfgId)
        {
            break;
        }
    }

    if (loopCnt < ENET_TEST_NUM_TX_CH_CFG_PARAMS)
    {
        return(&gTestTxChCfg[loopCnt]);
    }
    else
    {
        return(NULL);
    }
}

static inline EnetTestRxFlowCfgInfo *Test_getRxFlowCfgParams(uint32_t rxFlowCfgId)
{
    uint32_t loopCnt;

    for (loopCnt = 0U; loopCnt < ENET_TEST_NUM_RX_FLOW_CFG_PARAMS; loopCnt++)
    {
        if (gTestRxFlowCfg[loopCnt].rxFlowCfgId == rxFlowCfgId)
        {
            break;
        }
    }

    if (loopCnt < ENET_TEST_NUM_RX_FLOW_CFG_PARAMS)
    {
        return(&gTestRxFlowCfg[loopCnt]);
    }
    else
    {
        return(NULL);
    }
}

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ENET_TEST_CONFIG_H_ */
