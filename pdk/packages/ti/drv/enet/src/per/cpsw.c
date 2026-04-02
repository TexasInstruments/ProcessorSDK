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
 * \file  cpsw.c
 *
 * \brief This file contains the implementation of the CPSW peripheral. This
 *        implementation supports CPSW_2G, CPSW_5G and CPSW_9G.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x101

#include <stdint.h>
#include <stdarg.h>
#include <ti/csl/csl_cpswitch.h>
#include <ti/drv/enet/enet_cfg.h>
#include <ti/drv/enet/priv/mod/cpsw_ale_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_cpts_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_hostport_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_macport_priv.h>
#include <ti/drv/enet/priv/mod/mdio_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_stats_priv.h>
#include <ti/drv/enet/priv/core/enet_rm_priv.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/include/core/enet_osal.h>
#include <ti/drv/enet/include/core/enet_soc.h>
#include <ti/drv/enet/include/core/enet_per.h>
#include <ti/drv/enet/include/per/cpsw.h>
#include <ti/drv/enet/priv/per/cpsw_priv.h>
#include <ti/drv/enet/priv/per/enet_hostport_udma.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/include/common/enet_phymdio_dflt.h>
#include <ti/drv/enet/include/phy/enetphy.h>
#include "cpsw_intervlan.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*! \brief Default common TX MTU. */
#define CPSW_COMMON_TX_MTU_DEFAULT            (2024U)

/*!
 * \brief Priority escalation value for switch scheduler.
 *
 * When a port is in escalate priority, this is the number of higher priority
 * packets sent before the next lower priority is allowed to send a packet.
 * Escalate priority allows lower priority packets to be sent at a fixed rate
 * relative to the next higher priority.  The min value of esc_pri_ld_val = 2
 */
#define CPSW_ESC_PRI_LD_VAL                   (2U)

/*! \brief Number of UDMA RX channels required for CPSW host port */
#define CPSW_UDMA_NUM_RX_CH                   (1U)

/*!
 * \brief RM RX channel index for CPSW with UDMA.
 *
 * Single UDMA RX channel is used for CPSW, RM channel index is 0-relative.
 */
#define CPSW_RM_RX_CH_IDX                     (0U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t Cpsw_openInternal(Cpsw_Handle hCpsw,
                                 Enet_Type enetType,
                                 uint32_t instId,
                                 const Cpsw_Cfg *cfg);

static void Cpsw_closeInternal(Cpsw_Handle hCpsw);

static void Cpsw_saveInternalCtxt(Cpsw_Handle hCpsw);

static int32_t Cpsw_restoreInternalCtxt(Cpsw_Handle hCpsw);

static int32_t Cpsw_ioctlInternal(EnetPer_Handle hPer,
                                  uint32_t cmd,
                                  Enet_IoctlPrms *prms);

static int32_t Cpsw_registerIntrs(Cpsw_Handle hCpsw,
                                  const Cpsw_Cfg *cfg);

static void Cpsw_unregisterIntrs(Cpsw_Handle hCpsw);

static void Cpsw_statsIsr(uintptr_t arg);

static void Cpsw_mdioIsr(uintptr_t arg);

static void Cpsw_cptsIsr(uintptr_t arg);

static void Cpsw_handleMdioLinkStateChange(EnetMdio_Group group,
                                           Mdio_PhyStatus *phyStatus,
                                           void *cbArgs);

static Cpsw_PortLinkState *Cpsw_getPortLinkState(Cpsw_Handle hCpsw,
                                                 uint32_t phyAddr);

static int32_t Cpsw_openPortLinkWithPhy(Cpsw_Handle hCpsw,
                                        Enet_MacPort macPort,
                                        const CpswMacPort_Cfg *macCfg,
                                        const EnetPhy_Cfg *phyCfg,
                                        const EnetMacPort_Interface *mii,
                                        const EnetMacPort_LinkCfg *linkCfg);

static int32_t Cpsw_openPortLinkNoPhy(Cpsw_Handle hCpsw,
                                      Enet_MacPort macPort,
                                      const CpswMacPort_Cfg *macCfg,
                                      const EnetMacPort_Interface *mii,
                                      const EnetMacPort_LinkCfg *linkCfg);

static int32_t Cpsw_openPortLink(Cpsw_Handle hCpsw,
                                 Enet_MacPort macPort,
                                 const CpswMacPort_Cfg *macCfg,
                                 const EnetPhy_Cfg *phyCfg,
                                 const EnetMacPort_Interface *mii,
                                 const EnetMacPort_LinkCfg *linkCfg);

static void Cpsw_closePortLink(Cpsw_Handle hCpsw,
                               Enet_MacPort macPort);

static int32_t Cpsw_handleLinkUp(Cpsw_Handle hCpsw,
                                 Enet_MacPort macPort);

static int32_t Cpsw_handleLinkDown(Cpsw_Handle hCpsw,
                                   Enet_MacPort macPort);

static int32_t Cpsw_getPortLinkCfg(Cpsw_Handle hCpsw,
                                   Enet_MacPort macPort,
                                   EnetMacPort_LinkCfg *linkCfg);

static int32_t Cpsw_isPortLinkUp(Cpsw_Handle hCpsw,
                                 Enet_MacPort macPort,
                                 bool *linked);

#if ENET_CFG_IS_ON(CPSW_SGMII)
static uint32_t Cpsw_mapPort2XgmiiId(Enet_MacPort macPort);

static int32_t Cpsw_setSgmiiMode(Cpsw_Handle hCpsw,
                                 Enet_MacPort macPort,
                                 const EnetMacPort_Interface *mii,
                                 const CpswMacPort_Cfg *macCfg);
#endif

static int32_t Cpsw_validateTxShortIpgCfg(const Cpsw_Handle hCpsw,
                                          const Cpsw_SetTxShortIpgCfgInArgs *inArgs);

static int32_t Cpsw_setTxShortIpgCfg(const Cpsw_Handle hCpsw,
                                     const Cpsw_SetTxShortIpgCfgInArgs *inArgs);

static int32_t Cpsw_getTxShortIpgCfg(const Cpsw_Handle hCpsw,
                                     Cpsw_TxShortIpgCfg *shortIpgCfg);

static int32_t Cpsw_setDfltThreadCfg(Cpsw_Handle hCpsw,
                                     uint32_t flowId);

static int32_t Cpsw_validateDfltFlow(Cpsw_Handle hCpsw,
                                     Enet_DfltFlowInfo *dfltFlowInfo,
                                     uint32_t flowId);

static int32_t Cpsw_validateFlowId(Cpsw_Handle hCpsw,
                                   uint32_t coreKey,
                                   uint32_t startIdx,
                                   uint32_t flowIdx);

static int32_t Cpsw_printRegisters(Cpsw_Handle hCpsw);

#if ENET_CFG_IS_ON(CPSW_IET)
static void Cpsw_enableIet(EnetPer_Handle hPer);

static void Cpsw_disableIet(EnetPer_Handle hPer);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
static const char *Cpsw_gSpeedNames[] =
{
    [ENET_SPEED_10MBIT]  = "10-Mbps",
    [ENET_SPEED_100MBIT] = "100-Mbps",
    [ENET_SPEED_1GBIT]   = "1-Gbps",
    [ENET_SPEED_10GBIT]  = "10-Gbps",
    [ENET_SPEED_AUTO]    = "auto",
};

static const char *Cpsw_gDuplexNames[] =
{
    [ENET_DUPLEX_HALF] = "Half-Duplex",
    [ENET_DUPLEX_FULL] = "Full-Duplex",
    [ENET_DUPLEX_AUTO] = "auto",
};
#endif

/*! \brief Default host and MAC port TX priority MTUs */
static const uint32_t Cpsw_txPriMtuDefault[] =
{
    CPSW_COMMON_TX_MTU_DEFAULT, /* TX Priority 0 MTU */
    CPSW_COMMON_TX_MTU_DEFAULT, /* TX Priority 1 MTU */
    CPSW_COMMON_TX_MTU_DEFAULT, /* TX Priority 2 MTU */
    CPSW_COMMON_TX_MTU_DEFAULT, /* TX Priority 3 MTU */
    CPSW_COMMON_TX_MTU_DEFAULT, /* TX Priority 4 MTU */
    CPSW_COMMON_TX_MTU_DEFAULT, /* TX Priority 5 MTU */
    CPSW_COMMON_TX_MTU_DEFAULT, /* TX Priority 6 MTU */
    CPSW_COMMON_TX_MTU_DEFAULT  /* TX Priority 7 MTU */
};

#if ENET_CFG_IS_ON(DEV_ERROR)
/* Public CPSW peripheral IOCTL validation data. */
static Enet_IoctlValidate gCpsw_ioctlValidate[] =
{
    ENET_IOCTL_VALID_PRMS(CPSW_PER_IOCTL_SET_INTERVLAN_ROUTE_UNI_EGRESS,
                          sizeof(Cpsw_SetInterVlanRouteUniEgressInArgs),
                          sizeof(Cpsw_SetInterVlanRouteUniEgressOutArgs)),

    ENET_IOCTL_VALID_PRMS(CPSW_PER_IOCTL_CLEAR_INTERVLAN_ROUTE_UNI_EGRESS,
                          sizeof(Cpsw_ClearInterVlanRouteUniEgressInArgs),
                          sizeof(CpswMacPort_InterVlanRouteId)),

    ENET_IOCTL_VALID_PRMS(CPSW_PER_IOCTL_SET_INTERVLAN_ROUTE_MULTI_EGRESS,
                          sizeof(Cpsw_SetInterVlanRouteMultiEgressInArgs),
                          sizeof(Cpsw_SetInterVlanRouteMultiEgressOutArgs)),

    ENET_IOCTL_VALID_PRMS(CPSW_PER_IOCTL_CLEAR_INTERVLAN_ROUTE_MULTI_EGRESS,
                          sizeof(Cpsw_ClearInterVlanRouteMultiEgressInArgs),
                          sizeof(CpswMacPort_InterVlanRouteId)),

    ENET_IOCTL_VALID_PRMS(CPSW_PER_IOCTL_SET_SHORT_IPG_CFG,
                          sizeof(Cpsw_SetTxShortIpgCfgInArgs),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_PER_IOCTL_GET_SHORT_IPG_CFG,
                          0U,
                          sizeof(Cpsw_TxShortIpgCfg)),

    ENET_IOCTL_VALID_PRMS(ENET_IOCTL_REGISTER_RX_DEFAULT_FLOW,
                          sizeof(Enet_DfltFlowInfo),
                          0U),

    ENET_IOCTL_VALID_PRMS(ENET_IOCTL_UNREGISTER_RX_DEFAULT_FLOW,
                          sizeof(Enet_DfltFlowInfo),
                          0U),

    ENET_IOCTL_VALID_PRMS(ENET_IOCTL_REGISTER_DSTMAC_RX_FLOW,
                          sizeof(Enet_MacDstFlowInfo),
                          0U),

    ENET_IOCTL_VALID_PRMS(ENET_IOCTL_UNREGISTER_DSTMAC_RX_FLOW,
                          sizeof(Enet_MacDstFlowInfo),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_IOCTL_REGISTER_PORT_RX_FLOW,
                          sizeof(Cpsw_PortRxFlowInfo),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_IOCTL_UNREGISTER_PORT_RX_FLOW,
                          sizeof(Cpsw_PortRxFlowInfo),
                          0U),
};
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Cpsw_initCfg(EnetPer_Handle hPer,
                  Enet_Type enetType,
                  void *cfg,
                  uint32_t cfgSize)
{
    Cpsw_Cfg *cpswCfg = (Cpsw_Cfg *)cfg;
    uint32_t i;

    Enet_devAssert(cfgSize == sizeof(Cpsw_Cfg),
                   "Invalid CPSW peripheral config params size %u (expected %u)",
                   cfgSize, sizeof(Cpsw_Cfg));

    cpswCfg->escalatePriorityLoadVal = CPSW_ESC_PRI_LD_VAL;

    /* VLAN configuration parameters */
    cpswCfg->vlanCfg.vlanAware  = BFALSE;
    cpswCfg->vlanCfg.vlanSwitch = ENET_VLAN_TAG_TYPE_INNER;
    cpswCfg->vlanCfg.outerVlan  = 0x88A8U;
    cpswCfg->vlanCfg.innerVlan  = 0x8100U;
    cpswCfg->eccCrcMode         = PACKET_HEADER_ECC_CRC_MODE;

    ENET_UTILS_COMPILETIME_ASSERT(ENET_ARRAYSIZE(cpswCfg->txMtu) == ENET_PRI_NUM);

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        cpswCfg->txMtu[i] = Cpsw_txPriMtuDefault[i];
    }

    /* Initialize host port config params */
    CpswHostPort_initCfg(&cpswCfg->hostPortCfg);

    /* Initialize CPTS config params */
    CpswCpts_initCfg(&cpswCfg->cptsCfg);

    /* Initialize MDIO config params */
    Mdio_initCfg(&cpswCfg->mdioCfg);

    /* Initialize ALE params */
    CpswAle_initCfg(&cpswCfg->aleCfg);

    cpswCfg->intrPriority              = 1U;
    cpswCfg->mdioLinkStateChangeCb     = NULL;
    cpswCfg->mdioLinkStateChangeCbArg  = NULL;
    cpswCfg->portLinkStatusChangeCb    = NULL;
    cpswCfg->portLinkStatusChangeCbArg = NULL;

    cpswCfg->enableQsgmii0RDC = BFALSE;
    cpswCfg->enableQsgmii1RDC = BFALSE;
}

int32_t Cpsw_open(EnetPer_Handle hPer,
                  Enet_Type enetType,
                  uint32_t instId,
                  const void *cfg,
                  uint32_t cfgSize)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)hPer;
    Cpsw_Cfg *cpswCfg = (Cpsw_Cfg *)cfg;
    Enet_IoctlPrms prms;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hPer->virtAddr;
#if ENET_CFG_IS_ON(CPSW_SGMII)
    CSL_Xge_cpsw_ss_sRegs *ssRegs = (CSL_Xge_cpsw_ss_sRegs *)hPer->virtAddr2;
#endif
    CSL_CPSW_PTYPE pType;
    uintptr_t key;
    uint32_t i;
    int32_t status = ENET_SOK;

    Enet_devAssert(cfgSize == sizeof(Cpsw_Cfg),
                   "Invalid CPSW peripheral config params size %u (expected %u)",
                   cfgSize, sizeof(Cpsw_Cfg));

    /* Save EnetMod handles for easy access */
    for (i = 0U; i < hCpsw->macPortNum; i++)
    {
        hCpsw->hMacPort[i] = ENET_MOD(&hCpsw->macPortObj[i]);
    }

    hCpsw->hHostPort = ENET_MOD(&hCpsw->hostPortObj);
    hCpsw->hStats    = ENET_MOD(&hCpsw->statsObj);
    hCpsw->hAle      = ENET_MOD(&hCpsw->aleObj);
    hCpsw->hCpts     = ENET_MOD(&hCpsw->cptsObj);
    hCpsw->hMdio     = ENET_MOD(&hCpsw->mdioObj);
    hCpsw->hRm       = ENET_MOD(&hCpsw->rmObj);

    /* Set escalate priority value (number of higher priority packets to be sent
     * before next lower priority is allowed to send a packet */
    CSL_CPSW_getPTypeReg(regs, &pType);
    pType.escPriLoadVal = cpswCfg->escalatePriorityLoadVal;
    CSL_CPSW_setPTypeReg(regs, &pType);

    /* Set VLAN config: aware/non-aware, inner/outer tag */
    if (cpswCfg->vlanCfg.vlanAware)
    {
        CSL_CPSW_enableVlanAware(regs);
    }
    else
    {
        CSL_CPSW_disableVlanAware(regs);
    }

    CSL_CPSW_setEccCrcMode(regs, cpswCfg->eccCrcMode);

    CSL_CPSW_setVlanType(regs, (uint32_t)cpswCfg->vlanCfg.vlanSwitch);
    CSL_CPSW_setVlanLTypeReg(regs, cpswCfg->vlanCfg.innerVlan, cpswCfg->vlanCfg.outerVlan);

#if ENET_CFG_IS_ON(CPSW_IET)
    /* Enable IET global control */
     Cpsw_enableIet(hPer);
#endif

#if ENET_CFG_IS_ON(CPSW_EST)
    /* Enable EST global control */
    if (ENET_FEAT_IS_EN(hPer->features, CPSW_FEATURE_EST))
    {
        Cpsw_enableEst(hPer);
    }
#endif

    /* Set port global config */
    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        CSL_CPSW_setTxMaxLenPerPriority(regs, i, cpswCfg->txMtu[i]);

        /* Save largest of per priority port egress MTUs to check against
         * host port and MAC port RX MTUs */
        if (cpswCfg->txMtu[i] > hCpsw->maxPerPrioMtu)
        {
            hCpsw->maxPerPrioMtu = cpswCfg->txMtu[i];
        }
    }

#if ENET_CFG_IS_ON(CPSW_SGMII)
    /* TODO: This should be moved to CPSW MAC port */
    /* Configure QSGMII RDCD */
    CSL_CPSW_SS_setQSGMIIControlRdCd(ssRegs, 0U, cpswCfg->enableQsgmii0RDC ? 1U : 0U);
    CSL_CPSW_SS_setQSGMIIControlRdCd(ssRegs, 1U, cpswCfg->enableQsgmii1RDC ? 1U : 0U);
#endif

    /* Initialize all CPSW modules */
    status = Cpsw_openInternal(hCpsw, enetType, instId, cpswCfg);
    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to open CPSW modules");

    /* Save CPSW configurations for context */
    memcpy(&hCpsw->ctxt.cpswCfg, cfg, sizeof(Cpsw_Cfg));

    /* Set host port flowIdOffset which is added to the switch transmit
     * (egress/host receive) flow Id */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &hCpsw->dmaResInfo.rxStartIdx);
        status = EnetMod_ioctl(hCpsw->hHostPort, CPSW_HOSTPORT_SET_FLOW_ID_OFFSET, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to set flow ID");
    }

    key = EnetOsal_disableAllIntr();

    /* Register interrupts */
    if (status == ENET_SOK)
    {
        status = Cpsw_registerIntrs(hCpsw, cpswCfg);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to register interrupts");
    }

    /* Enable CPTS interrupt */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = EnetMod_ioctl(hCpsw->hCpts, CPSW_CPTS_IOCTL_ENABLE_INTR, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to enable CPTS interrupt");
    }

    /* All initialization is complete */
    if (status == ENET_SOK)
    {
        hCpsw->selfCoreId = cpswCfg->resCfg.selfCoreId;
#if ENET_CFG_IS_ON(CPSW_EST)
        hCpsw->cptsRftClkFreq = cpswCfg->cptsCfg.cptsRftClkFreq;
#endif
    }
    else if (status != ENET_EALREADYOPEN)
    {
        /* Rollback if any error other than trying to open while already open */
        Cpsw_unregisterIntrs(hCpsw);
        Cpsw_closeInternal(hCpsw);
    }
    else
    {
        ENETTRACE_ERR(status, "Unexpected status");
    }

    EnetOsal_restoreAllIntr(key);

    return status;
}

int32_t Cpsw_rejoin(EnetPer_Handle hPer,
                    Enet_Type enetType,
                    uint32_t instId)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)hPer;
    uint32_t i;

    /* Save EnetMod handles for easy access */
    for (i = 0U; i < hCpsw->macPortNum; i++)
    {
        hCpsw->hMacPort[i] = ENET_MOD(&hCpsw->macPortObj[i]);
    }

    hCpsw->hHostPort = ENET_MOD(&hCpsw->hostPortObj);
    hCpsw->hStats    = ENET_MOD(&hCpsw->statsObj);
    hCpsw->hAle      = ENET_MOD(&hCpsw->aleObj);
    hCpsw->hCpts     = ENET_MOD(&hCpsw->cptsObj);
    hCpsw->hMdio     = ENET_MOD(&hCpsw->mdioObj);
    hCpsw->hRm       = ENET_MOD(&hCpsw->rmObj);

    return ENET_SOK;
}

void Cpsw_close(EnetPer_Handle hPer)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)hPer;
    Enet_IoctlPrms prms;
    uintptr_t key;
    int32_t status;

    key = EnetOsal_disableAllIntr();

    /* Disable CPTS interrupt */
    ENET_IOCTL_SET_NO_ARGS(&prms);
    status = EnetMod_ioctl(hCpsw->hCpts, CPSW_CPTS_IOCTL_DISABLE_INTR, &prms);
    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to disable CPTS interrupt");

    /* Unregister interrupts */
    if (status == ENET_SOK)
    {
        Cpsw_unregisterIntrs(hCpsw);
        Cpsw_setDfltThreadCfg(hCpsw, hCpsw->rsvdFlowId);
        Cpsw_closeInternal(hCpsw);
    }

#if ENET_CFG_IS_ON(CPSW_IET)
    /* Disable IET global control */
     Cpsw_disableIet(hPer);
#endif

#if ENET_CFG_IS_ON(CPSW_EST)
    /* Disable EST global control */
    if (ENET_FEAT_IS_EN(hPer->features, CPSW_FEATURE_EST))
    {
        Cpsw_disableEst(hPer);
    }
#endif

   EnetOsal_restoreAllIntr(key);
}

void Cpsw_saveCtxt(EnetPer_Handle hPer)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)hPer;
    Enet_IoctlPrms prms;
    uintptr_t key;
    int32_t status = ENET_SOK;

    key = EnetOsal_disableAllIntr();

    /* Disable CPTS interrupt */
    ENET_IOCTL_SET_NO_ARGS(&prms);
    status = EnetMod_ioctl(hCpsw->hCpts, CPSW_CPTS_IOCTL_DISABLE_INTR, &prms);
    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to disable CPTS interrupt");

    Cpsw_unregisterIntrs(hCpsw);

#if ENET_CFG_IS_ON(CPSW_EST)
    /* Disable EST global control */
    if (ENET_FEAT_IS_EN(hPer->features, CPSW_FEATURE_EST))
    {
        Cpsw_disableEst(hPer);
    }
#endif

    Cpsw_saveInternalCtxt(hCpsw);

    EnetOsal_restoreAllIntr(key);
}

int32_t Cpsw_restoreCtxt(EnetPer_Handle hPer)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)hPer;
    Enet_IoctlPrms prms;
    uintptr_t key;
    int32_t status = ENET_SOK;

    key = EnetOsal_disableAllIntr();

    status = Cpsw_restoreInternalCtxt(hCpsw);
    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to restore CPSW context");

#if ENET_CFG_IS_ON(CPSW_EST)
        /* Enable EST global control */
        if (ENET_FEAT_IS_EN(hPer->features, CPSW_FEATURE_EST))
        {
            Cpsw_enableEst(hPer);
        }
#endif

    /* Register interrupts */
    if (status == ENET_SOK)
    {
        status = Cpsw_registerIntrs(hCpsw, &hCpsw->ctxt.cpswCfg);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to register interrupts");
    }

    /* Enable CPTS interrupt */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = EnetMod_ioctl(hCpsw->hCpts, CPSW_CPTS_IOCTL_ENABLE_INTR, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to enable CPTS interrupt");
    }

    EnetOsal_restoreAllIntr(key);

    return status;
}

static void Cpsw_saveInternalCtxt(Cpsw_Handle hCpsw)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hPer->virtAddr;
    EnetRm_FreeInternalRxFlowInArgs freeRsvdFlowInArgs;
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;
    uint32_t i;

    hCpsw->ctxt.control =  CSL_REG32_RD(&regs->CONTROL_REG);
    hCpsw->ctxt.statPortEn =  CSL_REG32_RD(&regs->STAT_PORT_EN_REG);
    hCpsw->ctxt.thruRate =  CSL_REG32_RD(&regs->THRU_RATE_REG);
    hCpsw->ctxt.gapThresh =  CSL_REG32_RD(&regs->GAP_THRESH_REG);

    hCpsw->ctxt.txPriMaxLen[0U] =  CSL_REG32_RD(&regs->TX_PRI0_MAXLEN_REG);
    hCpsw->ctxt.txPriMaxLen[1U] =  CSL_REG32_RD(&regs->TX_PRI1_MAXLEN_REG);
    hCpsw->ctxt.txPriMaxLen[2U] =  CSL_REG32_RD(&regs->TX_PRI2_MAXLEN_REG);
    hCpsw->ctxt.txPriMaxLen[3U] =  CSL_REG32_RD(&regs->TX_PRI3_MAXLEN_REG);
    hCpsw->ctxt.txPriMaxLen[4U] =  CSL_REG32_RD(&regs->TX_PRI4_MAXLEN_REG);
    hCpsw->ctxt.txPriMaxLen[5U] =  CSL_REG32_RD(&regs->TX_PRI5_MAXLEN_REG);
    hCpsw->ctxt.txPriMaxLen[6U] =  CSL_REG32_RD(&regs->TX_PRI6_MAXLEN_REG);
    hCpsw->ctxt.txPriMaxLen[7U] =  CSL_REG32_RD(&regs->TX_PRI7_MAXLEN_REG);

    for (i = 0U; i< CPSW_MAC_PORT_NUM; i++)
    {
        hCpsw->ctxt.portCtxt[i].tsCtl       = CSL_REG32_RD(&regs->ENETPORT[i].PN_TS_CTL_REG);
        hCpsw->ctxt.portCtxt[i].tsSeqLtype  = CSL_REG32_RD(&regs->ENETPORT[i].PN_TS_SEQ_LTYPE_REG);
        hCpsw->ctxt.portCtxt[i].tsVlanLtype = CSL_REG32_RD(&regs->ENETPORT[i].PN_TS_VLAN_LTYPE_REG);
        hCpsw->ctxt.portCtxt[i].tsCtlLtype2 = CSL_REG32_RD(&regs->ENETPORT[i].PN_TS_CTL_LTYPE2_REG);
        hCpsw->ctxt.portCtxt[i].tsCtl2      = CSL_REG32_RD(&regs->ENETPORT[i].PN_TS_CTL2_REG);
    }

    /* Set CPSW to Soft Idle state to issue CPSW software idle where no packets
     * will bne started to be unloaded from port 0(HostPort) */
    CSL_REG32_WR(&regs->SOFT_IDLE_REG, 1U);

    /* Save Host Port Context */
    EnetMod_saveCtxt(hCpsw->hHostPort);

    /* Save ALE Context */
    EnetMod_saveCtxt(hCpsw->hAle);

    /* Save MDIO Context */
    EnetMod_saveCtxt(hCpsw->hMdio);

    /* Save CPTS Context */
    EnetMod_saveCtxt(hCpsw->hCpts);

    /* Close the reserved flow */
    Enet_assert (hCpsw->hRxRsvdFlow != NULL);
    status = EnetHostPortDma_closeRsvdFlow(hCpsw->hRxRsvdFlow);
    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to close reserved flow");
    Enet_assert(status == ENET_SOK);

    freeRsvdFlowInArgs.chIdx   = CPSW_RM_RX_CH_IDX;
    freeRsvdFlowInArgs.flowIdx = hCpsw->rsvdFlowId;
    freeRsvdFlowInArgs.coreId  = hCpsw->selfCoreId;
    ENET_IOCTL_SET_IN_ARGS(&prms, &freeRsvdFlowInArgs);

    status = EnetMod_ioctl(hCpsw->hRm, ENET_RM_IOCTL_INTERNAL_FREE_RX_FLOW, &prms);
    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to free reserved flow");
    Enet_assert(status == ENET_SOK);

    Enet_assert (hCpsw->hDma != NULL);
    EnetHostPortDma_close(hCpsw->hDma);
    hCpsw->hDma = NULL;
}

static int32_t Cpsw_restoreInternalCtxt(Cpsw_Handle hCpsw)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hPer->virtAddr;
    EnetRm_AllocInternalRxFlowInArgs inArgs;
    EnetRm_AllocRxFlow rxFlowPrms;
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;
    uint32_t i;

    CSL_REG32_WR(&regs->CONTROL_REG, hCpsw->ctxt.control);
    CSL_REG32_WR(&regs->TX_PRI0_MAXLEN_REG, hCpsw->ctxt.txPriMaxLen[0U]);
    CSL_REG32_WR(&regs->TX_PRI1_MAXLEN_REG, hCpsw->ctxt.txPriMaxLen[1U]);
    CSL_REG32_WR(&regs->TX_PRI2_MAXLEN_REG, hCpsw->ctxt.txPriMaxLen[2U]);
    CSL_REG32_WR(&regs->TX_PRI3_MAXLEN_REG, hCpsw->ctxt.txPriMaxLen[3U]);
    CSL_REG32_WR(&regs->TX_PRI4_MAXLEN_REG, hCpsw->ctxt.txPriMaxLen[4U]);
    CSL_REG32_WR(&regs->TX_PRI5_MAXLEN_REG, hCpsw->ctxt.txPriMaxLen[5U]);
    CSL_REG32_WR(&regs->TX_PRI6_MAXLEN_REG, hCpsw->ctxt.txPriMaxLen[6U]);
    CSL_REG32_WR(&regs->TX_PRI7_MAXLEN_REG, hCpsw->ctxt.txPriMaxLen[7U]);
    CSL_REG32_WR(&regs->STAT_PORT_EN_REG, hCpsw->ctxt.statPortEn);
    CSL_REG32_WR(&regs->THRU_RATE_REG, hCpsw->ctxt.thruRate);
    CSL_REG32_WR(&regs->GAP_THRESH_REG, hCpsw->ctxt.gapThresh);

    for (i = 0U; i< CPSW_MAC_PORT_NUM; i++)
    {
        CSL_REG32_WR(&regs->ENETPORT[i].PN_TS_CTL_REG, hCpsw->ctxt.portCtxt[i].tsCtl);
        CSL_REG32_WR(&regs->ENETPORT[i].PN_TS_SEQ_LTYPE_REG, hCpsw->ctxt.portCtxt[i].tsSeqLtype);
        CSL_REG32_WR(&regs->ENETPORT[i].PN_TS_VLAN_LTYPE_REG, hCpsw->ctxt.portCtxt[i].tsVlanLtype);
        CSL_REG32_WR(&regs->ENETPORT[i].PN_TS_CTL_LTYPE2_REG, hCpsw->ctxt.portCtxt[i].tsCtlLtype2);
        CSL_REG32_WR(&regs->ENETPORT[i].PN_TS_CTL2_REG,  hCpsw->ctxt.portCtxt[i].tsCtl2);
    }

    /* Restore Host Port Context */
    if (status == ENET_SOK)
    {
        status = EnetMod_restoreCtxt(hCpsw->hHostPort);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to restore host port context");
    }

    /* Restore ALE Context */
    if (status == ENET_SOK)
    {
        status = EnetMod_restoreCtxt(hCpsw->hAle);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to restore ALE context");
    }

    /* Restore MDIO Context */
    if (status == ENET_SOK)
    {
        status = EnetMod_restoreCtxt(hCpsw->hMdio);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to restore MDIO context");
    }

    /* Restore CPTS Context */
    if (status == ENET_SOK)
    {
        status = EnetMod_restoreCtxt(hCpsw->hCpts);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to restore CPTS context");
    }

    /* Open UDMA for CPSW NAVSS instance type */
    hCpsw->hDma = EnetHostPortDma_open(hPer, hCpsw->ctxt.cpswCfg.dmaCfg, &hCpsw->ctxt.cpswCfg.resCfg);
    if (NULL == hCpsw->hDma)
    {
        status = ENET_EFAIL;
        ENETTRACE_ERR(status, "Failed to open CPSW DMA");
    }

    /* Open the reserved flow */
    inArgs.coreId = hCpsw->ctxt.cpswCfg.resCfg.selfCoreId;
    inArgs.chIdx  = CPSW_RM_RX_CH_IDX;
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &rxFlowPrms);

    status = EnetMod_ioctl(hCpsw->hRm, ENET_RM_IOCTL_INTERNAL_ALLOC_RX_FLOW, &prms);
    if (status == ENET_SOK)
    {
        hCpsw->hRxRsvdFlow = EnetHostPortDma_openRsvdFlow(hCpsw->ctxt.cpswCfg.dmaCfg,
                                                          hCpsw->dmaResInfo.rxStartIdx,
                                                          rxFlowPrms.flowIdx);
        ENETTRACE_ERR_IF((NULL == hCpsw->hRxRsvdFlow), ENET_EFAIL, "Failed to open default flow");
        if (NULL != hCpsw->hRxRsvdFlow)
        {
            hCpsw->rsvdFlowId = rxFlowPrms.flowIdx;
            status = Cpsw_setDfltThreadCfg(hCpsw, hCpsw->rsvdFlowId);
            ENETTRACE_ERR_IF((status != ENET_SOK), status, "Failed to set default thread config");
        }
    }

    return status;
}

int32_t Cpsw_ioctl(EnetPer_Handle hPer,
                   uint32_t cmd,
                   Enet_IoctlPrms *prms)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)hPer;
    uint32_t major;
    int32_t status = ENET_SOK;

    if (ENET_IOCTL_GET_TYPE(cmd) != ENET_IOCTL_TYPE_PUBLIC)
    {
        status = ENET_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "%s: IOCTL cmd %u is not public", hPer->name);
    }

    /* Route IOCTL command to Per or Mod */
    if (status == ENET_SOK)
    {
        major = ENET_IOCTL_GET_MAJ(cmd);
        switch (major)
        {
            case ENET_IOCTL_PER_BASE:
            {
                status = Cpsw_ioctlInternal(hPer, cmd, prms);
            }
            break;

            case ENET_IOCTL_FDB_BASE:
            {
                status = EnetMod_ioctl(hCpsw->hAle, cmd, prms);
            }
            break;

            case ENET_IOCTL_TIMESYNC_BASE:
            {
                status = EnetMod_ioctl(hCpsw->hCpts, cmd, prms);
            }
            break;

            case ENET_IOCTL_HOSTPORT_BASE:
            {
                status = EnetMod_ioctl(hCpsw->hHostPort, cmd, prms);
            }
            break;

            case ENET_IOCTL_TAS_BASE:
                        {
#if ENET_CFG_IS_ON(CPSW_EST)
#if ENET_CFG_IS_OFF(CPSW_MACPORT_EST)
#error "CPSW EST feature requires ENET_CFG_CPSW_MACPORT_EST"
#endif
                if (ENET_FEAT_IS_EN(hPer->features, CPSW_FEATURE_EST))
                {
                    status = Cpsw_ioctlEst(hPer, cmd, prms);
                }
                else
                {
                    status = ENET_ENOTSUPPORTED;
                }
#else
                status = ENET_ENOTSUPPORTED;
#endif
                        }
                        break;

            case ENET_IOCTL_MACPORT_BASE:
            {
                /* Note: Typecast to GenericInArgs is possible because all public
                 * MAC port IOCTL input args have macPort as their first member */
                EnetMacPort_GenericInArgs *inArgs = (EnetMacPort_GenericInArgs *)prms->inArgs;
                uint32_t portNum = ENET_MACPORT_NORM(inArgs->macPort);

                if (portNum < EnetSoc_getMacPortMax(hPer->enetType, hPer->instId))
                {
                    status = EnetMod_ioctl(hCpsw->hMacPort[portNum], cmd, prms);
                }
                else
                {
                    status = ENET_EINVALIDPARAMS;
                    ENETTRACE_ERR(status, "Invalid MAC port id %u", ENET_MACPORT_ID(inArgs->macPort));
                }
            }
            break;

            case ENET_IOCTL_MDIO_BASE:
            {
                status = EnetMod_ioctl(hCpsw->hMdio, cmd, prms);
            }
            break;

            case ENET_IOCTL_STATS_BASE:
            {
                status = EnetMod_ioctl(hCpsw->hStats, cmd, prms);
            }
            break;

            case ENET_IOCTL_PHY_BASE:
            {
                /* Note: Typecast to GenericInArgs is possible because all public
                 * MAC port IOCTL input args have macPort as their first member */
                EnetPhy_GenericInArgs *inArgs = (EnetPhy_GenericInArgs *)prms->inArgs;
                uint32_t portNum = ENET_MACPORT_NORM(inArgs->macPort);
                EnetPhy_Handle hPhy = hCpsw->hPhy[portNum];

                if (hPhy != NULL)
                {
                    status = EnetPhyMdioDflt_ioctl(hCpsw->hPhy[portNum], cmd, prms);
                }
                else
                {
                    status = ENET_EFAIL;
                    ENETTRACE_ERR(status, "No PHY for MAC port %u", ENET_MACPORT_ID(inArgs->macPort));
                }
            }
            break;

            case ENET_IOCTL_RM_BASE:
            {
                status = EnetMod_ioctl(hCpsw->hRm, cmd, prms);
            }
            break;

            default:
            {
                status = ENET_ENOTSUPPORTED;
                break;
            }
        }
    }

    return status;
}

void Cpsw_poll(EnetPer_Handle hPer,
               Enet_Event evt,
               const void *arg,
               uint32_t argSize)
{
    /* Nothing to do, not supported */
    ENETTRACE_WARN("Poll feature is not supported by CPSW Per");
}

void Cpsw_periodicTick(EnetPer_Handle hPer)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)hPer;
    EnetPhy_Handle hPhy;
    Enet_MacPort macPort;
    Cpsw_PortLinkState *portLinkState;
    EnetPhy_LinkStatus linkStatus;
    Enet_IoctlPrms prms;
    CpswAle_SetPortStateInArgs setPortStateInArgs;
    bool linked = BFALSE;
    uint32_t maxPorts = 0U;
    uint32_t portId;
    uint32_t i;
    int32_t status;
    uint32_t numPortCb;
    struct Cpsw_PortLinkCbInfoList_s
    {
        bool linked;
        Enet_MacPort macPort;
    } portCbInfoList[CPSW_MAC_PORT_NUM];

    numPortCb = 0;

    /* Get the max number of ports */
    maxPorts = EnetSoc_getMacPortMax(hPer->enetType, hPer->instId);

    /* Run PHY tick and handle link up/down events */
    for (i = 0U; i < maxPorts; i++)
    {
        macPort = ENET_MACPORT_DENORM(i);
        portId = ENET_MACPORT_ID(macPort);
        linked = BFALSE;

        ENETTRACE_VAR(portId);
        hPhy = hCpsw->hPhy[i];
        portLinkState = &hCpsw->portLinkState[i];

        /* Check if the corresponding PHY is enabled */
        if (portLinkState->isTickEnabled)
        {
            /* TODO: Need to make lock more granular */
            //EnetOsal_lockMutex(hCpsw->lock);

            if (hPhy != NULL)
            {
                /* Run PHY tick */
                linkStatus = EnetPhy_tick(hPhy);

                /* Handle link up/down events */
                if ((linkStatus == ENETPHY_GOT_LINK) ||
                    (linkStatus == ENETPHY_LOST_LINK))
                {
                    linked = (linkStatus == ENETPHY_GOT_LINK);
                    status = linked ? Cpsw_handleLinkUp(hCpsw, macPort) :
                                      Cpsw_handleLinkDown(hCpsw, macPort);
                    ENETTRACE_ERR_IF(status != ENET_SOK, status,
                                     "Port %u: Failed to handle link change", portId);

                    /* Call application callback when port link is up - at this point app can
                     * start data flow */
                    if ((status == ENET_SOK) && (hCpsw->portLinkStatusChangeCb != NULL))
                    {
                        /* Add port to callback info list.
                         * All portLinkStatus Cb functions are invoked at the
                         * end of function after relinquishing locks */
                        Enet_devAssert(numPortCb < ENET_ARRAYSIZE(portCbInfoList),
                                       "Invalid port number %u, expected < %u",
                                       numPortCb, ENET_ARRAYSIZE(portCbInfoList));
                        portCbInfoList[numPortCb].macPort = macPort;
                        portCbInfoList[numPortCb].linked  = linked;
                        numPortCb++;
                    }

                    /* All checks cleared, link state can be updated now */
                    if (status == ENET_SOK)
                    {
                        portLinkState->isLinkUp = linked;

                        /* Disable periodic tick while link is up. It will be re-enabled
                         * by MDIO_LINKINT upon link down detection */
                        if (portLinkState->isPollEnabled && linked)
                        {
                            portLinkState->isTickEnabled = BFALSE;
                        }
                    }
                }
            }
            else if (EnetMod_isOpen(hCpsw->hMacPort[i]))
            {
                /* Check link up status for modes that support link status query */
                status = Cpsw_isPortLinkUp(hCpsw, macPort, &linked);

                if ((status == ENET_SOK) && (linked == BTRUE))
                {
                    /* Set ALE port in forward state */
                    setPortStateInArgs.portNum   = CPSW_ALE_MACPORT_TO_ALEPORT(i);
                    setPortStateInArgs.portState = CPSW_ALE_PORTSTATE_FORWARD;
                    ENET_IOCTL_SET_IN_ARGS(&prms, &setPortStateInArgs);

                    status = EnetMod_ioctl(hCpsw->hAle, CPSW_ALE_IOCTL_SET_PORT_STATE, &prms);
                    ENETTRACE_ERR_IF(status != ENET_SOK, status,
                                    "Port %u: Failed to set ALE port %u to forward state",
                                    portId, setPortStateInArgs.portNum);

                    /* If port is in NO-PHY mode, invoke the portLinkUp Cb */
                    portLinkState->isLinkUp      = BTRUE;
                    portLinkState->isTickEnabled = BFALSE;
                    if (hCpsw->portLinkStatusChangeCb != NULL)
                    {
                        /* Add port to callback info list.
                         * All portLinkStatus Cb functions are invoked at the
                         * end of function after relinquishing locks */
                        Enet_devAssert(numPortCb < ENET_ARRAYSIZE(portCbInfoList),
                                    "Invalid port number %u, expected < %u",
                                    numPortCb, ENET_ARRAYSIZE(portCbInfoList));
                        portCbInfoList[numPortCb].macPort = macPort;
                        portCbInfoList[numPortCb].linked  = BTRUE;
                        numPortCb++;
                    }
                }
            }

            //EnetOsal_unlockMutex(hCpsw->lock);
        }
    }

    for (i = 0U; i < numPortCb; i++)
    {
        if (hCpsw->portLinkStatusChangeCb != NULL)
        {
            /* Call application's port link status change callback */
            hCpsw->portLinkStatusChangeCb(portCbInfoList[i].macPort,
                                          portCbInfoList[i].linked,
                                          hCpsw->portLinkStatusChangeCbArg);
        }
    }

    return;
}

static int32_t Cpsw_openInternal(Cpsw_Handle hCpsw,
                                 Enet_Type enetType,
                                 uint32_t instId,
                                 const Cpsw_Cfg *cfg)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    Cpsw_MdioLinkIntCtx *linkIntCtx = &hCpsw->mdioLinkIntCtx;
    Cpsw_PortLinkState *portLinkState;
    uint32_t i;
    int32_t status = ENET_SOK;

    /* Clear PHY link state */
    for (i = 0U; i < ENET_ARRAYSIZE(hCpsw->portLinkState); i++)
    {
        portLinkState = &hCpsw->portLinkState[i];

        portLinkState->phyAddr       = ENETPHY_INVALID_PHYADDR;
        portLinkState->isOpen        = BFALSE;
        portLinkState->isLinkUp      = BFALSE;
        portLinkState->isPollEnabled = BFALSE;
        portLinkState->isTickEnabled = BTRUE;
    }

    linkIntCtx->aliveMask            = ENET_MDIO_PHY_ADDR_MASK_NONE;
    linkIntCtx->linkedMask           = ENET_MDIO_PHY_ADDR_MASK_NONE;
    linkIntCtx->pollEnableMask       = cfg->mdioCfg.pollEnMask;
    linkIntCtx->linkStateChangeCb    = cfg->mdioLinkStateChangeCb;
    linkIntCtx->linkStateChangeCbArg = cfg->mdioLinkStateChangeCbArg;
    hCpsw->portLinkStatusChangeCb    = cfg->portLinkStatusChangeCb;
    hCpsw->portLinkStatusChangeCbArg = cfg->portLinkStatusChangeCbArg;

    /* Host port and MAC port MTUs should not be greater than largest of the port
     * egress per priority MTU as packet would get dropped by switch.
     * Though it is valid from hardware configuration, we return an error as it
     * serves no purpose */
    if (cfg->hostPortCfg.rxMtu > hCpsw->maxPerPrioMtu)
    {
        status = ENET_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "Host Port RX MTU (%d) exceeds max of TX Priority 0-7 MTU (%d)",
                      cfg->hostPortCfg.rxMtu, hCpsw->maxPerPrioMtu);
    }

    /* Open host port */
    if (status == ENET_SOK)
    {
        status = EnetMod_open(hCpsw->hHostPort, enetType, instId, &cfg->hostPortCfg, sizeof(cfg->hostPortCfg));
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to open host port");
    }

    /* Open ALE */
    if (status == ENET_SOK)
    {
        status = EnetMod_open(hCpsw->hAle, enetType, instId, &cfg->aleCfg, sizeof(cfg->aleCfg));
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to open ALE");
    }

    /* Open CPTS */
    if (status == ENET_SOK)
    {
        status = EnetMod_open(hCpsw->hCpts, enetType, instId, &cfg->cptsCfg, sizeof(cfg->cptsCfg));
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to open CPTS");
    }

    /* Open MDIO */
    if (status == ENET_SOK)
    {
        status = EnetMod_open(hCpsw->hMdio, enetType, instId, &cfg->mdioCfg, sizeof(cfg->mdioCfg));
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to open MDIO");
    }

    /* Open statistics */
    if (status == ENET_SOK)
    {
        status = EnetMod_open(hCpsw->hStats, enetType, instId, NULL, 0U);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to open stats");
    }

    /* Open DMA */
    if (status == ENET_SOK)
    {
        if (NULL != cfg->dmaCfg)
        {
            /* Open UDMA for CPSW NAVSS instance type */
            hCpsw->hDma = EnetHostPortDma_open(hPer, cfg->dmaCfg, &cfg->resCfg);
            if (NULL == hCpsw->hDma)
            {
                status = ENET_EFAIL;
                ENETTRACE_ERR(status, "CPSW: Failed to open CPSW DMA");
            }
        }
        else
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status, "CPSW: DMA open config is NULL");
        }
    }

    /* Open Resource Manager if Rx Channel open succeeded */
    if (status == ENET_SOK)
    {
        EnetRm_Cfg rmCfg;

        EnetHostPortDma_getDmaResInfo(hCpsw->hDma, &hCpsw->dmaResInfo, 0U);

        rmCfg.enetType              = enetType;
        rmCfg.instId                = instId;
        rmCfg.numRxCh               = CPSW_UDMA_NUM_RX_CH;
        rmCfg.rxStartFlowIdx[0U]    = hCpsw->dmaResInfo.rxStartIdx;
        rmCfg.rxFlowIdxCnt[0U]      = hCpsw->dmaResInfo.rxIdxCnt;
        /* TODO - move this inside ALE */
        Enet_assert(rmCfg.rxFlowIdxCnt[0U] <= (CSL_ALE_THREADMAPVAL_THREADVAL_MAX + 1));

        rmCfg.ioctlPermissionInfo   = cfg->resCfg.ioctlPermissionInfo;
        rmCfg.macList               = cfg->resCfg.macList;
        rmCfg.resPartInfo = cfg->resCfg.resPartInfo;

        status = EnetMod_open(hCpsw->hRm, enetType, instId, &rmCfg, sizeof(rmCfg));
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to open RM");
    }

    /* Open reserved flow */
    if (status == ENET_SOK)
    {
        EnetRm_AllocInternalRxFlowInArgs inArgs;
        EnetRm_AllocRxFlow rxFlowPrms;
        Enet_IoctlPrms prms;

        inArgs.coreId = cfg->resCfg.selfCoreId;
        inArgs.chIdx  = CPSW_RM_RX_CH_IDX;
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &rxFlowPrms);

        status = EnetMod_ioctl(hCpsw->hRm, ENET_RM_IOCTL_INTERNAL_ALLOC_RX_FLOW, &prms);
        if (status == ENET_SOK)
        {
            hCpsw->hRxRsvdFlow = EnetHostPortDma_openRsvdFlow(cfg->dmaCfg,
                                                               hCpsw->dmaResInfo.rxStartIdx,
                                                               rxFlowPrms.flowIdx);
            ENETTRACE_ERR_IF((NULL == hCpsw->hRxRsvdFlow), status, "Failed to open default flow");
            if (NULL != hCpsw->hRxRsvdFlow)
            {
                hCpsw->rsvdFlowId = rxFlowPrms.flowIdx;
                status = Cpsw_setDfltThreadCfg(hCpsw, hCpsw->rsvdFlowId);
                ENETTRACE_ERR_IF((status != ENET_SOK), status, "Failed to set default thread config");
            }
        }
        else
        {
            ENETTRACE_ERR(status, "Failed to allocate RX flow via RM");
            hCpsw->rsvdFlowId = ENET_RM_RXFLOWIDX_INVALID;
        }
    }

    if (status != ENET_SOK)
    {
        Cpsw_closeInternal(hCpsw);
    }

    return status;
}

static void Cpsw_closeInternal(Cpsw_Handle hCpsw)
{
    EnetMod_close(hCpsw->hHostPort);
    EnetMod_close(hCpsw->hAle);
    EnetMod_close(hCpsw->hCpts);
    EnetMod_close(hCpsw->hMdio);
    EnetMod_close(hCpsw->hStats);

    {
        int32_t status;
        EnetRm_FreeInternalRxFlowInArgs freeRsvdFlowInArgs;
        Enet_IoctlPrms prms;

        Enet_assert (hCpsw->hRxRsvdFlow != NULL);
        status = EnetHostPortDma_closeRsvdFlow(hCpsw->hRxRsvdFlow);
        Enet_assert(status == ENET_SOK);

        freeRsvdFlowInArgs.chIdx   = CPSW_RM_RX_CH_IDX;
        freeRsvdFlowInArgs.flowIdx = hCpsw->rsvdFlowId;
        freeRsvdFlowInArgs.coreId  = hCpsw->selfCoreId;
        ENET_IOCTL_SET_IN_ARGS(&prms, &freeRsvdFlowInArgs);

        status = EnetMod_ioctl(hCpsw->hRm, ENET_RM_IOCTL_INTERNAL_FREE_RX_FLOW, &prms);
        Enet_assert(status == ENET_SOK);

        Enet_assert (hCpsw->hDma != NULL);
        EnetHostPortDma_close(hCpsw->hDma);
        hCpsw->hDma = NULL;
    }

    EnetMod_close(hCpsw->hRm);
}

static uint32_t Cpsw_getTxMtuPerPriority(Cpsw_Handle hCpsw,
                                         uint32_t priority)
{
    uint32_t txMtu = 0U;
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hPer->virtAddr;

    if (hCpsw != NULL)
    {
        txMtu = CSL_CPSW_getTxMaxLenPerPriority(regs, priority);
    }

    return txMtu;
}

static uint32_t Cpsw_getRxMtuPort0(Cpsw_Handle hCpsw)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    EnetPort_MaxLen maxLen;

    maxLen.mru = 0U;
    ENET_IOCTL_SET_IN_ARGS(&prms, &maxLen);
    status = CpswHostPort_ioctl(hCpsw->hHostPort,
                                ENET_HOSTPORT_IOCTL_GET_MAXLEN,
                                &prms);
    if (status != ENET_SOK)
    {
        ENETTRACE_ERR(status, "Failed to get host port's RX MTU");
    }

    return maxLen.mru;
}
static int32_t Cpsw_ioctlInternal(EnetPer_Handle hPer,
                                  uint32_t cmd,
                                  Enet_IoctlPrms *prms)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)hPer;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hPer->virtAddr;
    int32_t status = ENET_SOK;

#if ENET_CFG_IS_ON(DEV_ERROR)
    /* Validate CPSW peripheral IOCTL parameters */
    if ((ENET_IOCTL_GET_PER(cmd) == ENET_IOCTL_PER_CPSW) &&
        (ENET_IOCTL_GET_TYPE(cmd) == ENET_IOCTL_TYPE_PUBLIC))
    {
        status = Enet_validateIoctl(cmd, prms,
                                    gCpsw_ioctlValidate,
                                    ENET_ARRAYSIZE(gCpsw_ioctlValidate));
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "IOCTL 0x%08x params are not valid", cmd);
    }
#endif

    switch (cmd)
    {
        case ENET_PER_IOCTL_GET_VERSION:
        {
            Enet_Version *version = (Enet_Version *)prms->outArgs;
            CSL_CPSW_VERSION ver;

            CSL_CPSW_getCpswVersionInfo(regs, &ver);
            version->maj = ver.majorVer;
            version->min = ver.minorVer;
            version->rtl = ver.rtlVer;
            version->id  = ver.id;
            version->other1 = ENET_VERSION_NONE;
            version->other2 = ENET_VERSION_NONE;
        }
        break;

        case ENET_PER_IOCTL_PRINT_REGS:
        {
            status = Cpsw_printRegisters(hCpsw);
        }
        break;

        case ENET_PER_IOCTL_OPEN_PORT_LINK:
        {
            const EnetPer_PortLinkCfg *portLinkCfg = (const EnetPer_PortLinkCfg *)prms->inArgs;
            Enet_MacPort macPort = portLinkCfg->macPort;
            const CpswMacPort_Cfg *macCfg = (CpswMacPort_Cfg *)portLinkCfg->macCfg;
            const EnetPhy_Cfg *phyCfg = &portLinkCfg->phyCfg;
            const EnetMacPort_Interface *mii = &portLinkCfg->mii;
            const EnetMacPort_LinkCfg *linkCfg = &portLinkCfg->linkCfg;

            if (macCfg->rxMtu > hCpsw->maxPerPrioMtu)
            {
                /* Host port and MAC port MTU should not be greater than largest of the port
                 * egress per priority MTU as packet would get dropped by Switch.
                 * Though it is valid from HW configuration, we return an error as it serves
                 * no purpose */
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status,
                              "Port %u: RX MTU (%d) exceeds max of TX Priority 0-7 MTU (%u)",
                              ENET_MACPORT_ID(macPort), macCfg->rxMtu, hCpsw->maxPerPrioMtu);
            }
            else
            {
                status = Cpsw_openPortLink(hCpsw, macPort, macCfg, phyCfg, mii, linkCfg);
                ENETTRACE_ERR_IF(status != ENET_SOK, status,
                                 "Port %u: Failed to open port link", ENET_MACPORT_ID(macPort));
            }
        }
        break;

        case ENET_PER_IOCTL_CLOSE_PORT_LINK:
        {
            Enet_MacPort macPort = *(Enet_MacPort *)prms->inArgs;

            Cpsw_closePortLink(hCpsw, macPort);
        }
        break;

        case ENET_PER_IOCTL_IS_PORT_LINK_UP:
        {
            Enet_MacPort macPort = *(Enet_MacPort *)prms->inArgs;
            bool *linkUp = (bool *)prms->outArgs;
            uint32_t portNum = ENET_MACPORT_NORM(macPort);

            *linkUp = hCpsw->portLinkState[portNum].isLinkUp;
        }
        break;

        case ENET_PER_IOCTL_GET_PORT_LINK_CFG:
        {
            Enet_MacPort macPort = *(Enet_MacPort *)prms->inArgs;
            EnetMacPort_LinkCfg *linkCfg = (EnetMacPort_LinkCfg *)prms->outArgs;

            status = Cpsw_getPortLinkCfg(hCpsw, macPort, linkCfg);
            ENETTRACE_ERR_IF(status != ENET_SOK, status,
                             "Port %u: Failed to get link config", ENET_MACPORT_ID(macPort));
        }
        break;

        case ENET_PER_IOCTL_ATTACH_CORE:
        {
            uint32_t coreId = *(uint32_t *)prms->inArgs;
            EnetPer_AttachCoreOutArgs *outArgs = (EnetPer_AttachCoreOutArgs *)prms->outArgs;
            Enet_IoctlPrms rmPrms;
            uint32_t i;

            ENET_IOCTL_SET_INOUT_ARGS(&rmPrms, &coreId, &outArgs->coreKey);

            status = EnetMod_ioctl(hCpsw->hRm, ENET_RM_IOCTL_ATTACH, &rmPrms);
            if (status == ENET_SOK)
            {
                /* Get MTU values */
                for (i = 0U; i < ENET_PRI_NUM; i++)
                {
                    outArgs->txMtu[i] = Cpsw_getTxMtuPerPriority(hCpsw, i);
                }

                outArgs->rxMtu = Cpsw_getRxMtuPort0(hCpsw);
            }
            else
            {
                ENETTRACE_ERR(status, "Failed to attach core %u", coreId);
            }
        }
        break;

        case ENET_PER_IOCTL_DETACH_CORE:
        {
            uint32_t coreKey = *((uint32_t *)prms->inArgs);
            Enet_IoctlPrms rmPrms;

            ENET_IOCTL_SET_IN_ARGS(&rmPrms, &coreKey);

            status = EnetMod_ioctl(hCpsw->hRm, ENET_RM_IOCTL_DETACH, &rmPrms);
            ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to detach core");
        }
        break;

        case CPSW_PER_IOCTL_SET_INTERVLAN_ROUTE_UNI_EGRESS:
        case CPSW_PER_IOCTL_CLEAR_INTERVLAN_ROUTE_UNI_EGRESS:
        case CPSW_PER_IOCTL_SET_INTERVLAN_ROUTE_MULTI_EGRESS:
        case CPSW_PER_IOCTL_CLEAR_INTERVLAN_ROUTE_MULTI_EGRESS:
        {
#if ENET_CFG_IS_ON(CPSW_INTERVLAN)
#if ENET_CFG_IS_OFF(CPSW_MACPORT_INTERVLAN)
#error "CPSW interVLAN feature requires ENET_CFG_CPSW_MACPORT_INTERVLAN"
#endif
            if (ENET_FEAT_IS_EN(hPer->features, CPSW_FEATURE_INTERVLAN))
            {
                status = Cpsw_ioctlInterVlan(hPer, cmd, prms);
            }
            else
            { 
                status = ENET_ENOTSUPPORTED;
                ENETTRACE_ERR(status, "InterVLAN is not supported");
            }
#else
            status = ENET_ENOTSUPPORTED;
            ENETTRACE_ERR(status, "InterVLAN is not enabled (see CPSW_INTERVLAN)");
#endif
        }
        break;

        case CPSW_PER_IOCTL_SET_SHORT_IPG_CFG:
        {
            const Cpsw_SetTxShortIpgCfgInArgs *inArgs = (const Cpsw_SetTxShortIpgCfgInArgs *)prms->inArgs;

            status = Cpsw_setTxShortIpgCfg(hCpsw, inArgs);
            ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to set TX short IPG config");
        }
        break;

        case CPSW_PER_IOCTL_GET_SHORT_IPG_CFG:
        {
            Cpsw_TxShortIpgCfg *shortIpgCfg = (Cpsw_TxShortIpgCfg *)prms->outArgs;

            status = Cpsw_getTxShortIpgCfg(hCpsw, shortIpgCfg);
            ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to get TX short IPG config");
        }
        break;

        case ENET_IOCTL_REGISTER_RX_DEFAULT_FLOW:
        {
            Enet_DfltFlowInfo *dfltFlowInfo = (Enet_DfltFlowInfo *)prms->inArgs;

            status = Cpsw_validateDfltFlow(hCpsw, dfltFlowInfo, hCpsw->rsvdFlowId);
            ENETTRACE_ERR_IF((status != ENET_SOK), status, "Invalid dflt flow %u", dfltFlowInfo->flowIdx);

            if (status == ENET_SOK)
            {
                status = Cpsw_setDfltThreadCfg(hCpsw, dfltFlowInfo->flowIdx);
                ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to set dflt thread cfg");
            }
        }
        break;

        case ENET_IOCTL_UNREGISTER_RX_DEFAULT_FLOW:
        {
            Enet_DfltFlowInfo *dfltFlowInfo = (Enet_DfltFlowInfo *)prms->inArgs;

            status = Cpsw_validateDfltFlow(hCpsw, dfltFlowInfo, dfltFlowInfo->flowIdx);
            ENETTRACE_ERR_IF((status != ENET_SOK), status, "Invalid dflt flow: %d", dfltFlowInfo->flowIdx);

            if (status == ENET_SOK)
            {
                status = Cpsw_setDfltThreadCfg(hCpsw, hCpsw->rsvdFlowId);
                ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to set dflt thread to rsvd flow");
            }
        }
        break;

        case ENET_IOCTL_REGISTER_DSTMAC_RX_FLOW:
        {
            Enet_MacDstFlowInfo *flowInfo = (Enet_MacDstFlowInfo *)prms->inArgs;

            status = Cpsw_validateFlowId(hCpsw, flowInfo->coreKey, flowInfo->startIdx, flowInfo->flowIdx);
            ENETTRACE_ERR_IF((status != ENET_SOK), status, "Invalid flow %d", flowInfo->flowIdx);

            if (status == ENET_SOK)
            {
                Enet_IoctlPrms alePrms;
                CpswAle_SetPolicerEntryOutArgs poliOutArgs;
                CpswAle_SetPolicerEntryInArgs poliInArgs;

                memset(&poliInArgs, 0, sizeof(poliInArgs));

                poliInArgs.policerMatch.policerMatchEnMask = CPSW_ALE_POLICER_MATCH_MACDST;
                memcpy(&poliInArgs.policerMatch.dstMacAddrInfo.addr.addr[0U], flowInfo->macAddress,
                       sizeof(poliInArgs.policerMatch.dstMacAddrInfo.addr.addr));
                poliInArgs.policerMatch.dstMacAddrInfo.addr.vlanId = 0U;
                poliInArgs.policerMatch.dstMacAddrInfo.portNum = CPSW_ALE_HOST_PORT_NUM;
                poliInArgs.threadIdEn             = BTRUE;
                poliInArgs.threadId               = flowInfo->flowIdx;
                poliInArgs.peakRateInBitsPerSec   = 0;
                poliInArgs.commitRateInBitsPerSec = 0;

                ENET_IOCTL_SET_INOUT_ARGS(&alePrms, &poliInArgs, &poliOutArgs);

                status = CpswAle_ioctl(hCpsw->hAle, CPSW_ALE_IOCTL_SET_POLICER, &alePrms);
                ENETTRACE_ERR_IF((status != ENET_SOK), status, "Failed to set DSTMAC policer");
                ENETTRACE_INFO_IF((status == ENET_SOK),
                                  "Registered MAC address (ALE entry=%u, policer entry=%u)",
                                  poliOutArgs.dstMacAleEntryIdx, poliOutArgs.policerEntryIdx);
            }
        }
        break;

        case ENET_IOCTL_UNREGISTER_DSTMAC_RX_FLOW:
        {
            Enet_IoctlPrms alePrms;
            CpswAle_DelPolicerEntryInArgs delPolicerInArgs;
            CpswAle_PolicerMatchParams policerMatchPrms;
            CpswAle_PolicerEntryOutArgs policerEntryOutArgs;
            Enet_MacDstFlowInfo *flowInfo = (Enet_MacDstFlowInfo *)prms->inArgs;

            status = Cpsw_validateFlowId(hCpsw, flowInfo->coreKey, flowInfo->startIdx, flowInfo->flowIdx);
            ENETTRACE_ERR_IF((status != ENET_SOK), status, "Invalid flow %d", flowInfo->flowIdx);

            if (status == ENET_SOK)
            {
                memset(&policerMatchPrms, 0, sizeof(policerMatchPrms));
                policerMatchPrms.policerMatchEnMask = CPSW_ALE_POLICER_MATCH_MACDST;
                memcpy(&policerMatchPrms.dstMacAddrInfo.addr.addr[0U], flowInfo->macAddress,
                       sizeof(policerMatchPrms.dstMacAddrInfo.addr.addr));
                policerMatchPrms.dstMacAddrInfo.addr.vlanId   = 0U;
                policerMatchPrms.dstMacAddrInfo.portNum = CPSW_ALE_HOST_PORT_NUM;

                ENET_IOCTL_SET_INOUT_ARGS(&alePrms, &policerMatchPrms, &policerEntryOutArgs);

                status = CpswAle_ioctl(hCpsw->hAle, CPSW_ALE_IOCTL_GET_POLICER, &alePrms);
                if (status == ENET_SOK)
                {
                    if ((policerEntryOutArgs.threadIdEn == BTRUE) &&
                        (policerEntryOutArgs.threadId == flowInfo->flowIdx))
                    {
                        status = ENET_SOK;
                    }
                    else
                    {
                        status = ENET_EINVALIDPARAMS;
                        ENETTRACE_ERR(status, "Invalid policer thread cfg (threadIdEn=%u threadId=%u)",
                                      policerEntryOutArgs.threadIdEn,
                                      policerEntryOutArgs.threadId);
                    }
                }
            }

            if (status == ENET_SOK)
            {
                memset(&delPolicerInArgs, 0, sizeof(delPolicerInArgs));

                delPolicerInArgs.policerMatch = policerMatchPrms;
                delPolicerInArgs.aleEntryMask = CPSW_ALE_POLICER_TABLEENTRY_DELETE_MACDST;

                ENET_IOCTL_SET_IN_ARGS(&alePrms, &delPolicerInArgs);

                status = CpswAle_ioctl(hCpsw->hAle, CPSW_ALE_IOCTL_DEL_POLICER, &alePrms);
                ENETTRACE_ERR_IF((status != ENET_SOK), status, "Invalid flow %d", flowInfo->flowIdx);
            }
        }
        break;

        case CPSW_IOCTL_REGISTER_PORT_RX_FLOW:
        {
            Cpsw_PortRxFlowInfo *flowInfo = (Cpsw_PortRxFlowInfo *)prms->inArgs;

            status = Cpsw_validateFlowId(hCpsw, flowInfo->coreKey, flowInfo->startIdx, flowInfo->flowIdx);
            ENETTRACE_ERR_IF((status != ENET_SOK), status, "Invalid port policer flow %d", flowInfo->flowIdx);

            if (status == ENET_SOK)
            {
                Enet_IoctlPrms alePrms;
                CpswAle_SetPolicerEntryOutArgs polOutArgs;
                CpswAle_SetPolicerEntryInArgs polInArgs;

                memset(&polInArgs, 0, sizeof(polInArgs));

                polInArgs.policerMatch.policerMatchEnMask = CPSW_ALE_POLICER_MATCH_PORT;
                polInArgs.policerMatch.portNum   = CPSW_ALE_MACPORT_TO_ALEPORT(flowInfo->macPort);
                polInArgs.threadIdEn             = BTRUE;
                polInArgs.threadId               = flowInfo->flowIdx;
                polInArgs.peakRateInBitsPerSec   = 0;
                polInArgs.commitRateInBitsPerSec = 0;

                ENET_IOCTL_SET_INOUT_ARGS(&alePrms, &polInArgs, &polOutArgs);

                status = CpswAle_ioctl(hCpsw->hAle, CPSW_ALE_IOCTL_SET_POLICER, &alePrms);
                ENETTRACE_ERR_IF((status != ENET_SOK), status, "Failed to set PORT policer");
            }
        }
        break;

        case CPSW_IOCTL_UNREGISTER_PORT_RX_FLOW:
        {
            Enet_IoctlPrms alePrms;
            CpswAle_DelPolicerEntryInArgs delPolInArgs;
            CpswAle_PolicerMatchParams polMatch;
            CpswAle_PolicerEntryOutArgs polOutArgs;
            Cpsw_PortRxFlowInfo *flowInfo = (Cpsw_PortRxFlowInfo *)prms->inArgs;

            status = Cpsw_validateFlowId(hCpsw, flowInfo->coreKey, flowInfo->startIdx, flowInfo->flowIdx);
            ENETTRACE_ERR_IF((status != ENET_SOK), status, "Invalid port policer flow %d", flowInfo->flowIdx);

            if (status == ENET_SOK)
            {
                memset(&polMatch, 0, sizeof(polMatch));
                polMatch.policerMatchEnMask = CPSW_ALE_POLICER_MATCH_PORT;
                polMatch.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(flowInfo->macPort);

                ENET_IOCTL_SET_INOUT_ARGS(&alePrms, &polMatch, &polOutArgs);

                status = CpswAle_ioctl(hCpsw->hAle, CPSW_ALE_IOCTL_GET_POLICER, &alePrms);
                if (status == ENET_SOK)
                {
                    if ((polOutArgs.threadIdEn == BTRUE) &&
                        (polOutArgs.threadId == flowInfo->flowIdx))
                    {
                        status = ENET_SOK;
                    }
                    else
                    {
                        status = ENET_EINVALIDPARAMS;
                        ENETTRACE_ERR(status, "Invalid policer thread cfg (threadIdEn=%u threadId=%u)",
                                      polOutArgs.threadIdEn, polOutArgs.threadId);
                    }
                }
            }

            if (status == ENET_SOK)
            {
                memset(&delPolInArgs, 0, sizeof(delPolInArgs));
                delPolInArgs.policerMatch = polMatch;

                ENET_IOCTL_SET_IN_ARGS(&alePrms, &delPolInArgs);

                status = CpswAle_ioctl(hCpsw->hAle, CPSW_ALE_IOCTL_DEL_POLICER, &alePrms);
                ENETTRACE_ERR_IF((status != ENET_SOK), status,
                                 "Invalid port policer flow %d", flowInfo->flowIdx);
            }
        }
        break;

        default:
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status, "Invalid IOCTL cmd 0x%08x", cmd);
        }
        break;
    }

    return status;
}

static int32_t Cpsw_registerIntrs(Cpsw_Handle hCpsw,
                                  const Cpsw_Cfg *cfg)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    Enet_Type enetType = hPer->enetType;
    uint32_t instId = hPer->instId;
    uint32_t trigType;
    uint32_t statIntrNum;
    uint32_t mdioIntrNum;
    uint32_t cptsIntrNum;
    bool statsIntrSetup = BFALSE;
    bool mdioIntrSetup = BFALSE;
    bool cptsIntrSetup = BFALSE;
    int32_t status;

    statIntrNum = EnetSoc_getIntrNum(enetType, instId, CPSW_INTR_STATS_PEND0);
    mdioIntrNum = EnetSoc_getIntrNum(enetType, instId, CPSW_INTR_MDIO_PEND);
    cptsIntrNum = EnetSoc_getIntrNum(enetType, instId, CPSW_INTR_EVNT_PEND);

    /* Setup stats interrupt */
    status = EnetSoc_setupIntrCfg(enetType, instId, CPSW_INTR_STATS_PEND0);
    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to setup stats interrupt");
    statsIntrSetup = (status == ENET_SOK);

    /* Setup MDIO interrupt */
    if (status == ENET_SOK)
    {
        status = EnetSoc_setupIntrCfg(enetType, instId, CPSW_INTR_MDIO_PEND);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to setup MDIO interrupt");
        mdioIntrSetup = (status == ENET_SOK);
    }

    /* Setup CPTS interrupt */
    if (status == ENET_SOK)
    {
        status = EnetSoc_setupIntrCfg(enetType, instId, CPSW_INTR_EVNT_PEND);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to setup CPTS interrupt");
        cptsIntrSetup = (status == ENET_SOK);
    }

    /* Register statistics interrupt */
    if (status == ENET_SOK)
    {
        trigType = EnetSoc_getIntrTriggerType(enetType, instId, CPSW_INTR_STATS_PEND0);

        hCpsw->hStatsIntr = EnetOsal_registerIntr(Cpsw_statsIsr,
                                                  statIntrNum,
                                                  cfg->intrPriority,
                                                  trigType,
                                                  hCpsw->hStats);
        if (hCpsw->hStatsIntr == NULL)
        {
            status = ENET_EFAIL;
            ENETTRACE_ERR(status, "Failed to register stats interrupt");
        }
    }

    /* Register MDIO interrupt */
    if ((status == ENET_SOK) &&
        (hCpsw->mdioLinkIntCtx.linkStateChangeCb != NULL))
    {
        trigType = EnetSoc_getIntrTriggerType(enetType, instId, CPSW_INTR_MDIO_PEND);

        hCpsw->hMdioIntr = EnetOsal_registerIntr(Cpsw_mdioIsr,
                                                 mdioIntrNum,
                                                 cfg->intrPriority,
                                                 trigType,
                                                 hCpsw);
        if (hCpsw->hMdioIntr == NULL)
        {
            status = ENET_EFAIL;
            ENETTRACE_ERR(status, "Failed to register MDIO interrupt");
        }
    }

    /* Register CPTS interrupt */
    if (status == ENET_SOK)
    {
        trigType = EnetSoc_getIntrTriggerType(enetType, instId, CPSW_INTR_EVNT_PEND);

        hCpsw->hCptsIntr = EnetOsal_registerIntr(Cpsw_cptsIsr,
                                                 cptsIntrNum,
                                                 cfg->intrPriority,
                                                 trigType,
                                                 hCpsw);
        if (hCpsw->hCptsIntr == NULL)
        {
            status = ENET_EFAIL;
            ENETTRACE_ERR(status, "Failed to register CPTS interrupt");
        }
    }

    /* Unwind in case of any error */
    if (status != ENET_SOK)
    {
        if (hCpsw->hCptsIntr != NULL)
        {
            EnetOsal_unregisterIntr(hCpsw->hCptsIntr);
        }

        if (hCpsw->hMdioIntr != NULL)
        {
            EnetOsal_unregisterIntr(hCpsw->hMdioIntr);
        }

        if (hCpsw->hStatsIntr != NULL)
        {
            EnetOsal_unregisterIntr(hCpsw->hStatsIntr);
        }

        if (statsIntrSetup)
        {
            status = EnetSoc_releaseIntrCfg(enetType, instId, CPSW_INTR_STATS_PEND0);
            Enet_assert(status == ENET_SOK, "Failed to release stats interrupt: %d", status);
        }

        if (mdioIntrSetup)
        {
            status = EnetSoc_releaseIntrCfg(enetType, instId, CPSW_INTR_MDIO_PEND);
            Enet_assert(status == ENET_SOK, "Failed to release MDIO interrupt: %d", status);
        }

        if (cptsIntrSetup)
        {
            status = EnetSoc_releaseIntrCfg(enetType, instId, CPSW_INTR_EVNT_PEND);
            Enet_assert(status == ENET_SOK, "Failed to release CPTS interrupt: %d", status);
        }
    }

    return status;
}

static void Cpsw_unregisterIntrs(Cpsw_Handle hCpsw)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    Enet_Type enetType = hPer->enetType;
    uint32_t instId = hPer->instId;
    Enet_IoctlPrms prms;
    int32_t status;

    /* Disable the interrupts before unregistering */
    /* Disable CPTS interrupt */
    ENET_IOCTL_SET_NO_ARGS(&prms);
    status = EnetMod_ioctl(hCpsw->hCpts, CPSW_CPTS_IOCTL_DISABLE_INTR, &prms);
    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to disable CPTS interrupt");

    if (status == ENET_SOK)
    {
        /* Disable MDIO interrupt */
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = EnetMod_ioctl(hCpsw->hMdio, ENET_MDIO_IOCTL_DISABLE_INTR, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to disable MDIO interrupt");
    }

    if (status == ENET_SOK)
    {
        /* Disable Stats interrupt */
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = EnetMod_ioctl(hCpsw->hStats, ENET_STATS_IOCTL_DISABLE_INTR, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to disable MDIO interrupt");
    }

    /* Unregister MDIO interrupt */
    if ((hCpsw->hMdioIntr != NULL) && (status == ENET_SOK))
    {
        EnetOsal_unregisterIntr(hCpsw->hMdioIntr);
        status = EnetSoc_releaseIntrCfg(enetType, instId, CPSW_INTR_STATS_PEND0);
        Enet_assert(status == ENET_SOK, "Failed to release stats interrupt: %d", status);
        hCpsw->hMdioIntr = NULL;
    }

    /* Unregister statistics interrupt */
    if ((hCpsw->hStatsIntr != NULL) && (status == ENET_SOK))
    {
        EnetOsal_unregisterIntr(hCpsw->hStatsIntr);
        status = EnetSoc_releaseIntrCfg(enetType, instId, CPSW_INTR_MDIO_PEND);
        Enet_assert(status == ENET_SOK, "Failed to release MDIO interrupt: %d", status);
        hCpsw->hStatsIntr = NULL;
    }

    /* Unregister CPTS interrupt */
    if ((hCpsw->hCptsIntr != NULL) && (status == ENET_SOK))
    {
        EnetOsal_unregisterIntr(hCpsw->hCptsIntr);
        status = EnetSoc_releaseIntrCfg(enetType, instId, CPSW_INTR_EVNT_PEND);
        Enet_assert(status == ENET_SOK, "Failed to release CPTS interrupt: %d", status);
        hCpsw->hCptsIntr = NULL;
    }
}

static void Cpsw_statsIsr(uintptr_t arg)
{
    EnetMod_Handle hStats = (EnetMod_Handle)arg;
    Enet_IoctlPrms prms;
    int32_t status;

    ENET_IOCTL_SET_NO_ARGS(&prms);
    status = EnetMod_ioctlFromIsr(hStats, CPSW_STATS_IOCTL_SYNC, &prms);

    /* TODO: Add ISR safe error:
     * ("Failed to sync statistics counters: %d", status); */
    ENET_UNUSED(status);
}

static void Cpsw_mdioIsr(uintptr_t arg)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)arg;
    EnetMod_Handle hMdio = hCpsw->hMdio;
    Enet_IoctlPrms prms;
    Mdio_Callbacks callbacks =
    {
        .linkStateCb  = Cpsw_handleMdioLinkStateChange,
        .userAccessCb = NULL,
        .cbArgs       = hCpsw,
    };
    int32_t status;

    ENET_IOCTL_SET_IN_ARGS(&prms, &callbacks);
    status = EnetMod_ioctlFromIsr(hMdio, MDIO_IOCTL_HANDLE_INTR, &prms);

    /* TODO: Add ISR safe error:
     * ("Failed to handle MDIO intr: %d", status); */
    ENET_UNUSED(status);
}

static void Cpsw_cptsIsr(uintptr_t arg)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)arg;
    EnetMod_Handle hCpts = hCpsw->hCpts;
    Enet_IoctlPrms prms;
    int32_t status;

    ENET_IOCTL_SET_NO_ARGS(&prms);
    status = EnetMod_ioctlFromIsr(hCpts, CPSW_CPTS_IOCTL_HANDLE_INTR, &prms);

    /* TODO: Add ISR safe error:
     * ("Failed to handle CPTS intr: %d", status); */
    ENET_UNUSED(status);
}

static void Cpsw_handleMdioLinkStateChange(EnetMdio_Group group,
                                           Mdio_PhyStatus *phyStatus,
                                           void *cbArgs)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)cbArgs;
    Cpsw_MdioLinkIntCtx *linkIntCtx = &hCpsw->mdioLinkIntCtx;
    Cpsw_PortLinkState *portLinkState;
    Cpsw_MdioLinkStateChangeInfo info;
    uint32_t aliveMaskChange;
    uint32_t linkedMaskChange;
    uint32_t i;

    aliveMaskChange  = phyStatus->aliveMask ^ linkIntCtx->aliveMask;
    linkedMaskChange = phyStatus->linkedMask ^ linkIntCtx->linkedMask;

    for (i = 0U; i <= MDIO_MAX_PHY_CNT; i++)
    {
        info.aliveChanged = ENET_IS_BIT_SET(aliveMaskChange, i);
        info.linkChanged  = ENET_IS_BIT_SET(linkedMaskChange, i);

        if (info.aliveChanged || info.linkChanged)
        {
            info.phyAddr  = i;
            info.isAlive  = ENET_IS_BIT_SET(phyStatus->aliveMask, i);
            info.isLinked = ENET_IS_BIT_SET(phyStatus->linkedMask, i);

            /* Re-enable periodic tick when link down is detected */
            if (info.linkChanged && !info.isLinked)
            {
                portLinkState = Cpsw_getPortLinkState(hCpsw, i);
                if (portLinkState != NULL)
                {
                    portLinkState->isTickEnabled = BTRUE;
                }
            }

            linkIntCtx->linkStateChangeCb(&info, linkIntCtx->linkStateChangeCbArg);
        }
    }

    linkIntCtx->aliveMask  = phyStatus->aliveMask;
    linkIntCtx->linkedMask = phyStatus->linkedMask;
}

static Cpsw_PortLinkState *Cpsw_getPortLinkState(Cpsw_Handle hCpsw,
                                                 uint32_t phyAddr)
{
    Cpsw_PortLinkState *portLinkState = NULL;
    uint32_t i;

    for (i = 0U; i < CPSW_MAC_PORT_NUM; i++)
    {
        if (hCpsw->portLinkState[i].isOpen &&
            (phyAddr == hCpsw->portLinkState[i].phyAddr))
        {
            portLinkState = &hCpsw->portLinkState[i];
            break;
        }
    }

    return portLinkState;
}

static int32_t Cpsw_openPortLinkWithPhy(Cpsw_Handle hCpsw,
                                        Enet_MacPort macPort,
                                        const CpswMacPort_Cfg *macCfg,
                                        const EnetPhy_Cfg *phyCfg,
                                        const EnetMacPort_Interface *mii,
                                        const EnetMacPort_LinkCfg *linkCfg)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t portId = ENET_MACPORT_ID(macPort);
    EnetMod_Handle hMacPort = hCpsw->hMacPort[portNum];
    EnetPhy_MdioHandle hPhyMdio = EnetPhyMdioDflt_getPhyMdio();
    EnetPhy_Mii phyMii;
    EnetPhy_LinkCfg phyLinkCfg;
    CpswMacPort_ModCfg macModCfg;
    uint32_t macPortCaps;
    int32_t status;

    ENETTRACE_VAR(portId);
    /* Enet module takes a single config structure, which in CPSW MAC port case is
     * EnetMacPort_ModCfg. This structure contains the initial MAC specific configuration
     * and also the link configuration (speed/duplexity) and MII interface type. */
    macModCfg.macCfg = *macCfg;
    macModCfg.mii = *mii;
    macModCfg.linkCfg = *linkCfg;

    /* Open MAC port */
    status = EnetMod_open(hMacPort, hPer->enetType, hPer->instId, &macModCfg, sizeof(macModCfg));
    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Port %u: Failed to open MAC", portId);

    /* Open PHY */
    if (status == ENET_SOK)
    {
        /* Convert MII and link configuration from Enet to ENETPHY types */
        phyMii = EnetUtils_macToPhyMii(mii);
        phyLinkCfg.speed = (EnetPhy_Speed)linkCfg->speed;
        phyLinkCfg.duplexity = (EnetPhy_Duplexity)linkCfg->duplexity;

        /* Get MAC port capabilities from SoC standpoint */
        macPortCaps = EnetSoc_getMacPortCaps(hPer->enetType, hPer->instId, macPort);

        /* Open ENETPHY driver */
        hCpsw->hPhy[portNum] = EnetPhy_open(phyCfg, phyMii, &phyLinkCfg, macPortCaps, hPhyMdio, hCpsw->hMdio);
        if (hCpsw->hPhy[portNum] == NULL)
        {
            status = ENET_EFAIL;
            ENETTRACE_ERR(status, "Port %u: Failed to open PHY", portId);
            CpswMacPort_close(hMacPort);
        }
    }

    return status;
}

static int32_t Cpsw_openPortLinkNoPhy(Cpsw_Handle hCpsw,
                                      Enet_MacPort macPort,
                                      const CpswMacPort_Cfg *macCfg,
                                      const EnetMacPort_Interface *mii,
                                      const EnetMacPort_LinkCfg *linkCfg)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t portId = ENET_MACPORT_ID(macPort);
    EnetMod_Handle hMacPort = hCpsw->hMacPort[portNum];
    CpswMacPort_ModCfg macModCfg;
    EnetMacPort_LinkCfg *macLinkCfg = &macModCfg.linkCfg;
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;

    ENETTRACE_VAR(portId);
    hCpsw->hPhy[portNum] = NULL;

    /* Enet module takes a single config structure, which in CPSW MAC port case is
     * EnetMacPort_ModCfg. This structure contains the initial MAC specific configuration
     * and also the link configuration (speed/duplexity) and MII interface type. */
    macModCfg.macCfg = *macCfg;
    macModCfg.mii = *mii;
    macModCfg.linkCfg = *linkCfg;

    /* Explicit speed and duplexity must be provided, can't be discovered */
    if ((macLinkCfg->speed == ENET_SPEED_AUTO) ||
        (macLinkCfg->duplexity == ENET_DUPLEX_AUTO))
    {
        status = ENET_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "Port %u: Auto-speed/duplexity is not valid for PHY-less links", portId);
    }

    /* Open MAC port in force mode */
    if (status == ENET_SOK)
    {
        if (EnetMacPort_isRgmii(mii))
        {
            if ((macCfg->loopbackEn == BTRUE) &&
                (macLinkCfg->speed != ENET_SPEED_1GBIT))
            {
                /* PDK-4633 - RGMII loopback fails in 100Mbps mode  */
                ENETTRACE_WARN("Port %u: RGMII loopback in 1G mode not supported, "
                               "overriding speed to 1Gbps", portId);
                macLinkCfg->speed = ENET_SPEED_1GBIT;
            }
        }

        status = EnetMod_open(hMacPort, hPer->enetType, hPer->instId, &macModCfg, sizeof(macModCfg));
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Port %u: Failed to open MAC", portId);
    }

    /* Enable MAC port with requested speed/duplexity */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, macLinkCfg);
        status = EnetMod_ioctl(hMacPort, CPSW_MACPORT_IOCTL_ENABLE, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Port %u: Failed to enable MAC port", portId);
    }

    /* Print the forced link speed and duplexity.
     * Note: Port link status detection occurs in Cpsw_periodicTick(), which doesn't 
     * have access to the linkCfg data, so the link config details are printed here.*/
    ENETTRACE_INFO_IF(status == ENET_SOK,
                      "Port %d: Forced link speed is %s, duplexity is %s",
                      portId,
                      Cpsw_gSpeedNames[linkCfg->speed],
                      Cpsw_gDuplexNames[linkCfg->duplexity]);

    if (status != ENET_SOK)
    {
        Cpsw_closePortLink(hCpsw, macPort);
    }

    return status;
}

static int32_t Cpsw_openPortLink(Cpsw_Handle hCpsw,
                                 Enet_MacPort macPort,
                                 const CpswMacPort_Cfg *macCfg,
                                 const EnetPhy_Cfg *phyCfg,
                                 const EnetMacPort_Interface *mii,
                                 const EnetMacPort_LinkCfg *linkCfg)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t portId = ENET_MACPORT_ID(macPort);
    EnetMod_Handle hMacPort = hCpsw->hMacPort[portNum];
    EnetPhy_Handle hPhy = hCpsw->hPhy[portNum];
    Cpsw_PortLinkState *portLinkState = &hCpsw->portLinkState[portNum];
    uint32_t pollEnMask = hCpsw->mdioLinkIntCtx.pollEnableMask;
    uint32_t phyAddr = phyCfg->phyAddr;
    int32_t status = ENET_SOK;

    ENETTRACE_VAR(portId);
    if ((portNum >= EnetSoc_getMacPortMax(hPer->enetType, hPer->instId)) ||
        EnetMod_isOpen(hMacPort) ||
        (hPhy != NULL))
    {
        status = ENET_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "Port %u: Failed to open port (MAC port %s, PHY %s)",
                      portId,
                      EnetMod_isOpen(hMacPort) ? "open" : "closed",
                      (hPhy != NULL) ? "open" : "closed");
    }

#if ENET_CFG_IS_ON(CPSW_SGMII)
    if (status == ENET_SOK)
    {
        if (EnetMacPort_isSgmii(mii) ||
            EnetMacPort_isQsgmii(mii))
        {
            /* Configure SGMII mode in CPSW SS */
            status = Cpsw_setSgmiiMode(hCpsw, macPort, mii, macCfg);

            /* Make sure the QSGMII ports are configured correctly */
            if (EnetMacPort_isQsgmii(mii))
            {
                status += EnetSoc_validateQsgmiiCfg(hPer->enetType, hPer->instId);
            }
        }

        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Invalid Q/SGMII port %u configuration", portId);
    }
#endif

    if (ENET_SOK == status)
    {
        if (phyAddr == ENETPHY_INVALID_PHYADDR)
        {
            status = Cpsw_openPortLinkNoPhy(hCpsw, macPort, macCfg, mii, linkCfg);
        }
        else
        {
            status = Cpsw_openPortLinkWithPhy(hCpsw, macPort, macCfg, phyCfg, mii, linkCfg);
        }
    }

    if (status == ENET_SOK)
    {
        portLinkState->isOpen  = BTRUE;
        portLinkState->phyAddr = phyAddr;

        if ((hCpsw->hMdioIntr != NULL) &&
            ENETPHY_IS_ADDR_VALID(phyAddr))
        {
            portLinkState->isPollEnabled = ENET_MDIO_IS_PHY_ADDR_SET(pollEnMask, phyAddr);
        }
    }

    return status;
}

static void Cpsw_closePortLink(Cpsw_Handle hCpsw,
                               Enet_MacPort macPort)
{
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    EnetMod_Handle hMacPort = hCpsw->hMacPort[portNum];
    EnetPhy_Handle hPhy = hCpsw->hPhy[portNum];
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    bool linked;

    if (portNum < EnetSoc_getMacPortMax(hPer->enetType, hPer->instId))
    {
        if (hPhy == NULL)
        {
            /* Link is always true for no-PHY mode */
            linked = BTRUE;
        }
        else
        {
            linked = hCpsw->portLinkState[portNum].isLinkUp;
        }

        /* Force link-down handling to undo settings done for this port link
         * i.e. ALE port state, MAC port disable, etc */
        if (linked)
        {
            Cpsw_handleLinkDown(hCpsw, macPort);
        }

        if (EnetMod_isOpen(hMacPort))
        {
            EnetMod_close(hMacPort);
        }

        if (hPhy != NULL)
        {
            EnetPhy_close(hPhy);
            hCpsw->hPhy[portNum] = NULL;
        }

        hCpsw->portLinkState[portNum].isOpen        = BFALSE;
        hCpsw->portLinkState[portNum].isPollEnabled = BFALSE;
        hCpsw->portLinkState[portNum].isTickEnabled = BTRUE;
        hCpsw->portLinkState[portNum].isLinkUp      = BFALSE;
    }
}

static int32_t Cpsw_handleLinkUp(Cpsw_Handle hCpsw,
                                 Enet_MacPort macPort)
{
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t portId = ENET_MACPORT_ID(macPort);
    EnetMod_Handle hMacPort = hCpsw->hMacPort[portNum];
    EnetPhy_Handle hPhy = hCpsw->hPhy[portNum];
    Enet_IoctlPrms prms;
    CpswAle_SetPortStateInArgs setPortStateInArgs;
    EnetPhy_LinkCfg phyLinkCfg;
    EnetMacPort_LinkCfg macLinkCfg;
    bool isPortLinked = BFALSE;
    int32_t status;

    ENETTRACE_VAR(portId);
    /* Check that port status also detected link up */
    do
    {
        /* Need to repeatedly check port status as it does not reflect
         * PHY status immediately */
        status = Cpsw_isPortLinkUp(hCpsw, macPort, &isPortLinked);
    }
    while ((status == ENET_SOK) && !isPortLinked);

    if (status == ENET_SOK)
    {
        if (!isPortLinked)
        {
            status = ENET_EUNEXPECTED;
            ENETTRACE_ERR(status, "Port %d: Link indicator detected link down", portId);
            Enet_devAssert(BFALSE, "Port %d: Link indicator detected link down", portId);
        }
    }
    else if (status == ENET_ENOTSUPPORTED)
    {
        /* It's not an error if port interface doesn't support link indicator */
        status = ENET_SOK;
    }
    else
    {
        ENETTRACE_ERR(status, "Port %u: Failed to get link indicator", portId);
    }

    /* Get link parameters (speed/duplexity) from PHY state machine */
    if (status == ENET_SOK)
    {
        status = EnetPhy_getLinkCfg(hPhy, &phyLinkCfg);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Port %u: Failed to get PHY link config", portId);
    }

    /* Enable MAC port */
    if (status == ENET_SOK)
    {
        macLinkCfg.speed = (Enet_Speed)phyLinkCfg.speed;
        macLinkCfg.duplexity = (Enet_Duplexity)phyLinkCfg.duplexity;
        ENET_IOCTL_SET_IN_ARGS(&prms, &macLinkCfg);

        status = EnetMod_ioctl(hMacPort, CPSW_MACPORT_IOCTL_ENABLE, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Port %u: Failed to enable MAC", portId);
    }

    /* Set ALE port state to 'Forward' */
    if (status == ENET_SOK)
    {
        setPortStateInArgs.portNum   = CPSW_ALE_MACPORT_TO_ALEPORT(portNum);
        setPortStateInArgs.portState = CPSW_ALE_PORTSTATE_FORWARD;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setPortStateInArgs);

        status = EnetMod_ioctl(hCpsw->hAle, CPSW_ALE_IOCTL_SET_PORT_STATE, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status,
                         "Port %u: Failed to set ALE port %u to forward state",
                         portId, setPortStateInArgs.portNum);
    }

    ENETTRACE_INFO_IF(status == ENET_SOK,
                      "Port %d: Link up: %s %s",
                      portId,
                      Cpsw_gSpeedNames[phyLinkCfg.speed],
                      Cpsw_gDuplexNames[phyLinkCfg.duplexity]);

    return status;
}

static int32_t Cpsw_handleLinkDown(Cpsw_Handle hCpsw,
                                   Enet_MacPort macPort)
{
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t portId = ENET_MACPORT_ID(macPort);
    EnetMod_Handle hMacPort = hCpsw->hMacPort[portNum];
    Enet_IoctlPrms prms;
    CpswAle_SetPortStateInArgs setPortStateInArgs;
#if ENET_CFG_IS_ON(CPSW_EST)
    EnetTas_SetStateInArgs estSetStateInArgs;
#endif
    uint32_t alePortNum;
    uint32_t numEntries = 0U;
    int32_t status;

    ENETTRACE_VAR(portId);
    ENETTRACE_INFO("Port %d: Link down", portId);

    ENET_IOCTL_SET_NO_ARGS(&prms);
    status = EnetMod_ioctl(hMacPort, CPSW_MACPORT_IOCTL_DISABLE, &prms);
    ENETTRACE_ERR_IF(status != ENET_SOK, status,
                     "Port %u: Failed to disable MAC port", portId);

    if (status == ENET_SOK)
    {
        setPortStateInArgs.portNum   = CPSW_ALE_MACPORT_TO_ALEPORT(portNum);
        setPortStateInArgs.portState = CPSW_ALE_PORTSTATE_DISABLED;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setPortStateInArgs);

        status = EnetMod_ioctl(hCpsw->hAle, CPSW_ALE_IOCTL_SET_PORT_STATE, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status,
                         "Port %u: Failed to set ALE port %u to disabled state",
                         portId, setPortStateInArgs.portNum);
    }

#if ENET_CFG_IS_ON(CPSW_EST)
    if (status == ENET_SOK)
    {
        EnetPer_Handle hPer = (EnetPer_Handle) hCpsw;
        estSetStateInArgs.macPort = macPort;
        estSetStateInArgs.state   = ENET_TAS_RESET;
        ENET_IOCTL_SET_IN_ARGS(&prms, &estSetStateInArgs);
        status = Cpsw_ioctlEst(hPer, ENET_TAS_IOCTL_SET_STATE, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status,
                         "Port %u: Failed to disable EST", portId);
    }
#endif

    if (status == ENET_SOK)
    {
        alePortNum = CPSW_ALE_MACPORT_TO_ALEPORT(portNum);
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &alePortNum, &numEntries);

        status = EnetMod_ioctl(hCpsw->hAle, CPSW_ALE_IOCTL_REMOVE_LEARNED_ENTRIES, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status,
                         "Port %u: Failed to delete learned ALE entries", portId);
    }

    return status;
}

static int32_t Cpsw_getPortLinkCfg(Cpsw_Handle hCpsw,
                                   Enet_MacPort macPort,
                                   EnetMacPort_LinkCfg *linkCfg)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    EnetMac_LayerType enetLayer;
    EnetMac_SublayerType enetSublayer;
    EnetMacPort_Interface mii;
    CSL_Xge_cpsw_ss_sRegs *ssRegs = (CSL_Xge_cpsw_ss_sRegs *)hPer->virtAddr2;
    CSL_CPSW_SS_RGMIISTATUS rgmiiStatus;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t portId = ENET_MACPORT_ID(macPort);
    int32_t status;

    ENETTRACE_VAR(portId);
    status = EnetSoc_getMacPortMii(hPer->enetType, hPer->instId, macPort, &mii);
    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Port %u: Failed to get ENET_CTRL", portId);

    if (status == ENET_SOK)
    {
        enetLayer = mii.layerType;
        enetSublayer = mii.sublayerType;

        /* Link configuration is only available for RGMII */
        if ((enetLayer == ENET_MAC_LAYER_GMII) &&
            (enetSublayer == ENET_MAC_SUBLAYER_REDUCED))
        {
            CSL_CPSW_SS_getRGMIIStatus(ssRegs, portNum, &rgmiiStatus);

            /* Get link speed */
            if (rgmiiStatus.speed == 0U)
            {
                linkCfg->speed = ENET_SPEED_10MBIT;
            }
            else if (rgmiiStatus.speed == 1U)
            {
                linkCfg->speed = ENET_SPEED_100MBIT;
            }
            else if (rgmiiStatus.speed == 2U)
            {
                linkCfg->speed = ENET_SPEED_1GBIT;
            }
            else
            {
                ENETTRACE_ERR(status, "Port %u: Invalid RGMII speed %d", portId, rgmiiStatus.speed);
                Enet_assert(BFALSE, "Invalid RGMII speed value %d", rgmiiStatus.speed);
            }

            /* Get link duplexity */
            if (rgmiiStatus.fullDuplex == 0U)
            {
                linkCfg->duplexity = ENET_DUPLEX_HALF;
            }
            else
            {
                linkCfg->duplexity = ENET_DUPLEX_FULL;
            }
        }
        else
        {
            status = ENET_ENOTSUPPORTED;
            ENETTRACE_ERR(status, "Port %u: Link config not available for layer %u sublayer %u",
                          portId, enetLayer, enetSublayer);
        }
    }
    else
    {
        ENETTRACE_ERR(status, "Port %u: Failed to get ENET_CTRL %u", portId);
    }

    return status;
}

static int32_t Cpsw_isPortLinkUp(Cpsw_Handle hCpsw,
                                 Enet_MacPort macPort,
                                 bool *linked)
{
    int32_t status = ENET_SOK;
#if ENET_CFG_IS_ON(CPSW_SGMII)
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    CSL_CPSW_SS_RGMIISTATUS rgmiiStatus;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hPer->virtAddr;
    CSL_Xge_cpsw_ss_sRegs *ssRegs = (CSL_Xge_cpsw_ss_sRegs *)hPer->virtAddr2;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t portId = ENET_MACPORT_ID(macPort);
    EnetMod_Handle hMacPort = hCpsw->hMacPort[portNum];
    Enet_IoctlPrms prms;
    EnetMacPort_GenericInArgs macPortInArgs;
    EnetMacPort_Interface mii;
    EnetMac_LayerType enetLayer;
    EnetMac_SublayerType enetSublayer;
    bool sgmiiLink;
    uint32_t qsgmiiId;

    ENETTRACE_VAR(portId);
    status = EnetSoc_getMacPortMii(hPer->enetType, hPer->instId, macPort, &mii);
    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Port %u: Failed to get ENET_CTRL", portId);

    if (status == ENET_SOK)
    {
        enetLayer = mii.layerType;
        enetSublayer = mii.sublayerType;

        if (enetLayer == ENET_MAC_LAYER_GMII)
        {
            if (enetSublayer == ENET_MAC_SUBLAYER_SERIAL)
            {
                /* We have link status in CPSW subsystem regs as well as in SGMII regs,
                 * check SGMII regs as link status in CPSW subsystem regs is only for
                 * QSGMII link status */
                macPortInArgs.macPort = macPort;
                ENET_IOCTL_SET_INOUT_ARGS(&prms, &macPortInArgs, &sgmiiLink);
                status = EnetMod_ioctl(hMacPort, CPSW_MACPORT_IOCTL_GET_SGMII_LINK_STATUS, &prms);
                ENETTRACE_ERR_IF(status != ENET_SOK, status,
                                 "Port %u: Failed to get SGMII link state", portId);
                *linked = sgmiiLink;
            }
            else if ((enetSublayer == ENET_MAC_SUBLAYER_QUAD_SERIAL_MAIN) ||
                     (enetSublayer == ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB))
            {
                status = EnetSoc_mapPort2QsgmiiId(hPer->enetType, hPer->instId, macPort, &qsgmiiId);
                ENETTRACE_ERR_IF(status != ENET_SOK, status,
                                 "Port %u: Failed to map QSGMII Id", portId);
                Enet_devAssert(status == ENET_SOK, "Port %u: QSGMII map failed", portId);

                if (ENET_SOK == status)
                {
                    *linked = CSL_CPSW_SS_getQSGMIIStatusRxSync(ssRegs, qsgmiiId);

                    /* Check SGMII link status as well for QSGMII as it is possible that rx sync is set
                     * because of single port */
                    macPortInArgs.macPort = macPort;
                    ENET_IOCTL_SET_INOUT_ARGS(&prms, &macPortInArgs, &sgmiiLink);

                    status = EnetMod_ioctl(hMacPort, CPSW_MACPORT_IOCTL_GET_SGMII_AUTONEG_LINK_STATUS, &prms);
                    ENETTRACE_ERR_IF(status != ENET_SOK, status,
                                     "Port %u: Failed to get SGMII link state", portId);
                    if (!sgmiiLink)
                    {
                        ENETTRACE_WARN("Port %u: SGMII link not up", portId);
                        *linked = BFALSE;
                    }
                }
            }
            else if (enetSublayer == ENET_MAC_SUBLAYER_REDUCED)
            {
                /* Check if MAC Port is in loopback mode, if yes, set the link directly to true,
                 * because mac_loopback enabled port, doesn't depend on RGMII path. */
                if (CSL_CPGMAC_SL_isLoopbackModeEnabled(regs, portNum) == BFALSE)
                {
                    CSL_CPSW_SS_getRGMIIStatus(ssRegs, portNum, &rgmiiStatus);

                    *linked = rgmiiStatus.link;
                }
                else
                {
                    *linked = BTRUE;
                }
            }
            else
            {
                ENETTRACE_DBG("Port %u: Sublayer %u doesn't support link status", portId, enetSublayer);
                status = ENET_ENOTSUPPORTED;
            }
        }
        else if (enetLayer == ENET_MAC_LAYER_XGMII)
        {
            *linked = CSL_CPSW_SS_getXGMIILinkStatus(ssRegs, Cpsw_mapPort2XgmiiId(macPort));
        }
        else
        {
            ENETTRACE_DBG("Port %u: Layer %u doesn't support link status", portId, enetLayer);
            status = ENET_ENOTSUPPORTED;
        }
    }
#else
    *linked = BTRUE;
#endif

    return status;
}

#if ENET_CFG_IS_ON(CPSW_SGMII)
static uint32_t Cpsw_mapPort2XgmiiId(Enet_MacPort macPort)
{
    /* One-to-one mapping between MAC port number and XGMII_LINK status register bits
     * for ports that support XGMII */
    return ENET_MACPORT_NORM(macPort);
}

static int32_t Cpsw_setSgmiiMode(Cpsw_Handle hCpsw,
                                 Enet_MacPort macPort,
                                 const EnetMacPort_Interface *mii,
                                 const CpswMacPort_Cfg *macCfg)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    EnetMod_Handle hMacPort = hCpsw->hMacPort[portNum];
    CSL_Xge_cpsw_ss_sRegs *ssRegs = (CSL_Xge_cpsw_ss_sRegs *)hPer->virtAddr2;
    int32_t status = ENET_EINVALIDPARAMS;

    if (EnetMacPort_isSgmii(mii) &&
        ENET_FEAT_IS_EN(hMacPort->features, CPSW_MACPORT_FEATURE_SGMII))
    {
        status = ENET_SOK;
    }
    else if (EnetMacPort_isQsgmii(mii) &&
             ENET_FEAT_IS_EN(hMacPort->features, CPSW_MACPORT_FEATURE_SGMII)) /* TODO: Add QSGMII feature flag */
    {
        status = ENET_SOK;
    }
    else
    {
        /* Invalid configuration */
        status = ENET_EINVALIDPARAMS;
    }

    if (status == ENET_SOK)
    {
        if (macCfg->sgmiiMode == ENET_MAC_SGMIIMODE_FIBER_WITH_PHY)
        {
            CSL_CPSW_SS_setSGMIIMode(ssRegs, portNum, CSL_SGMII_MODE_FIBER);
        }
        else
        {
            CSL_CPSW_SS_setSGMIIMode(ssRegs, portNum, CSL_SGMII_MODE_SGMII);
        }
    }

    return status;
}
#endif

static int32_t Cpsw_validateTxShortIpgCfg(const Cpsw_Handle hCpsw,
                                          const Cpsw_SetTxShortIpgCfgInArgs *inArgs)
{
    const CpswMacPort_PortTxShortIpgCfg *portShortIpgCfg;
    uint32_t macPortList = 0U;
    uint32_t portNum;
    uint32_t portId;
    uint32_t i;
    int32_t status = ENET_SOK;

    if (inArgs->configureGapThresh &&
        (inArgs->ipgTriggerThreshBlkCnt > CSL_XGE_CPSW_GAP_THRESH_REG_GAP_THRESH_MAX))
    {
        status = ENET_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "IPG trigger threshold block count %u exceeded max",
                      inArgs->ipgTriggerThreshBlkCnt);
    }

    if (ENET_SOK == status)
    {
        if (inArgs->numMacPorts > ENET_ARRAYSIZE(inArgs->portShortIpgCfg))
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status, "Invalid number of MAC ports %u exceeded %u",
                          inArgs->numMacPorts, ENET_ARRAYSIZE(inArgs->portShortIpgCfg));
        }
    }

    if (ENET_SOK == status)
    {
        for (i = 0U; i < inArgs->numMacPorts; i++)
        {
            portShortIpgCfg = &inArgs->portShortIpgCfg[i];
            portNum = ENET_MACPORT_NORM(portShortIpgCfg->macPort);
            ENETTRACE_VAR(portId);
            portId = ENET_MACPORT_ID(portShortIpgCfg->macPort);

            if ((portNum >= hCpsw->macPortNum) ||
                !EnetMod_isOpen(hCpsw->hMacPort[portNum]))
            {
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status, "Invalid MAC port %u", portId);
                break;
            }

            if (ENET_IS_BIT_SET(macPortList, portNum))
            {
                /* inArgs->portIpgCfg entries should have list of unique MAC ports.
                 * Flag error as previously seen macPort in inArgs->portIpgCfg
                 * is encountered again */
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status, "Multiple occurence of MAC port %u", portId);
                break;
            }

            if ((portShortIpgCfg->shortIpgCfg.txShortGapEn == BFALSE) &&
                (portShortIpgCfg->shortIpgCfg.txShortGapLimitEn == BTRUE))
            {
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status,
                              "MAC port %u's short gap limit can be enabled only if short gap is enabled",
                              portId);
                break;
            }

            macPortList |= ENET_BIT(portNum);
        }
    }

    return status;
}

static int32_t Cpsw_setTxShortIpgCfg(const Cpsw_Handle hCpsw,
                                     const Cpsw_SetTxShortIpgCfgInArgs *inArgs)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hPer->virtAddr;
    const CpswMacPort_PortTxShortIpgCfg *portShortIpgCfg;
    Enet_IoctlPrms prms;
    uint32_t portNum;
    uint32_t portId;
    uint32_t i;
    int32_t status;

    status = Cpsw_validateTxShortIpgCfg(hCpsw, inArgs);
    if (status == ENET_SOK)
    {
        if (inArgs->configureGapThresh)
        {
            CSL_CPSW_setGapThreshold(regs, inArgs->ipgTriggerThreshBlkCnt);
        }

        for (i = 0U; i < inArgs->numMacPorts; i++)
        {
            portShortIpgCfg = &inArgs->portShortIpgCfg[i];
            portNum = ENET_MACPORT_NORM(portShortIpgCfg->macPort);
            portId = ENET_MACPORT_ID(portShortIpgCfg->macPort);
            ENETTRACE_VAR(portId);

            ENET_IOCTL_SET_IN_ARGS(&prms, portShortIpgCfg);

            status = EnetMod_ioctl(hCpsw->hMacPort[portNum], CPSW_MACPORT_IOCTL_SET_SHORT_IPG, &prms);
            if (status != ENET_SOK)
            {
                ENETTRACE_ERR(status, "Failed to set MAC port %u's short IPG", portId);
                break;
            }
        }
    }
    else
    {
        ENETTRACE_ERR(status, "Failed to validated short IPG config");
    }

    return status;
}

static int32_t Cpsw_getTxShortIpgCfg(const Cpsw_Handle hCpsw,
                                     Cpsw_TxShortIpgCfg *shortIpgCfg)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hPer->virtAddr;
    EnetMod_Handle hMacPort;
    Enet_IoctlPrms prms;
    uint32_t i;
    int32_t status = ENET_SOK;

    shortIpgCfg->ipgTriggerThreshBlkCnt = CSL_CPSW_getGapThreshold(regs);
    shortIpgCfg->numMacPorts = 0U;

    for (i = 0U; i < hCpsw->macPortNum; i++)
    {
        hMacPort = hCpsw->hMacPort[i];

        if (EnetMod_isOpen(hMacPort))
        {
            EnetMacPort_GenericInArgs inArgs;
            CpswMacPort_PortTxShortIpgCfg *portIpgCfg;

            inArgs.macPort = ENET_MACPORT_DENORM(i);
            portIpgCfg = &shortIpgCfg->portShortIpgCfg[shortIpgCfg->numMacPorts];
            portIpgCfg->macPort = inArgs.macPort;
            ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &portIpgCfg->shortIpgCfg);

            status = EnetMod_ioctl(hMacPort, CPSW_MACPORT_IOCTL_GET_SHORT_IPG, &prms);
            if (status != ENET_SOK)
            {
                ENETTRACE_ERR(status, "Port %u: Failed to get short IPG",
                              ENET_MACPORT_ID(inArgs.macPort));
                break;
            }

            shortIpgCfg->numMacPorts++;
        }
    }

    return status;
}

static int32_t Cpsw_setDfltThreadCfg(Cpsw_Handle hCpsw,
                                     uint32_t flowId)
{
    CpswAle_DfltThreadCfg dfltThreadCfg;
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;

    ENET_IOCTL_SET_OUT_ARGS(&prms, &dfltThreadCfg);

    status = EnetMod_ioctl(hCpsw->hAle, CPSW_ALE_IOCTL_GET_DEFAULT_THREADCFG, &prms);
    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to get ALE default thread config");

    if (status == ENET_SOK)
    {
        dfltThreadCfg.dfltThreadEn = BTRUE;
        dfltThreadCfg.threadId     = flowId;
        ENET_IOCTL_SET_IN_ARGS(&prms, &dfltThreadCfg);

        status = EnetMod_ioctl(hCpsw->hAle, CPSW_ALE_IOCTL_SET_DEFAULT_THREADCFG, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to set ALE default thread config");
    }

    return status;
}

static int32_t Cpsw_validateFlowId(Cpsw_Handle hCpsw,
                                   uint32_t coreKey,
                                   uint32_t startIdx,
                                   uint32_t flowIdx)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t p0FlowIdOffset;

    ENET_IOCTL_SET_OUT_ARGS(&prms, &p0FlowIdOffset);
    status = CpswHostPort_ioctl(hCpsw->hHostPort,
                                CPSW_HOSTPORT_GET_FLOW_ID_OFFSET,
                                &prms);

    if (status == ENET_SOK)
    {
        if (startIdx != p0FlowIdOffset)
        {
            status = ENET_EINVALIDPARAMS;
        }
    }

    if (status == ENET_SOK)
    {
        EnetRm_ValidateRxFlowInArgs rmInArgs;

        rmInArgs.chIdx   = CPSW_RM_RX_CH_IDX;
        rmInArgs.coreKey = coreKey;
        rmInArgs.flowIdx = flowIdx;

        ENET_IOCTL_SET_IN_ARGS(&prms, &rmInArgs);

        status = EnetMod_ioctl(hCpsw->hRm, ENET_RM_IOCTL_VALIDATE_RX_FLOW, &prms);
    }

    return status;
}

static int32_t Cpsw_validateDfltFlow(Cpsw_Handle hCpsw,
                                     Enet_DfltFlowInfo *dfltFlowInfo,
                                     uint32_t flowId)
{
    int32_t status;
    Enet_IoctlPrms prms;

    status = Cpsw_validateFlowId(hCpsw,
                                 dfltFlowInfo->coreKey,
                                 dfltFlowInfo->startIdx,
                                 dfltFlowInfo->flowIdx);
    if (status == ENET_SOK)
    {
        CpswAle_DfltThreadCfg defaultThreadCfg;

        ENET_IOCTL_SET_OUT_ARGS(&prms, &defaultThreadCfg);

        status = CpswAle_ioctl(hCpsw->hAle,
                               CPSW_ALE_IOCTL_GET_DEFAULT_THREADCFG,
                               &prms);
        if (status == ENET_SOK)
        {
            if ((defaultThreadCfg.dfltThreadEn == BTRUE)
                &&
                (defaultThreadCfg.threadId == flowId))
            {
                status = ENET_SOK;
            }
            else
            {
                status = ENET_EINVALIDPARAMS;
            }
        }
    }

    return status;
}

static int32_t Cpsw_printRegisters(Cpsw_Handle hCpsw)
{
    EnetPer_Handle hPer = (EnetPer_Handle)hCpsw;
    EnetMod_Handle hMacPort;
    Enet_IoctlPrms prms;
    Enet_MacPort macPort;
    EnetMacPort_GenericInArgs inArgs;
    uint32_t i;
    int32_t status = ENET_SOK;

    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = CpswAle_ioctl(hCpsw->hAle, ENET_FDB_IOCTL_PRINT_REGS, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to print ALE regs");
    }

    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = CpswHostPort_ioctl(hCpsw->hHostPort, ENET_HOSTPORT_IOCTL_PRINT_REGS, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to print host port regs");
    }

    if (status == ENET_SOK)
    {
        for (i = 0U; i < EnetSoc_getMacPortMax(hPer->enetType, hPer->instId); i++)
        {
            hMacPort = hCpsw->hMacPort[i];
            macPort = ENET_MACPORT_DENORM(i);

            if (EnetMod_isOpen(hMacPort))
            {
                inArgs.macPort = macPort;
                ENET_IOCTL_SET_IN_ARGS(&prms, &inArgs);
                status = CpswMacPort_ioctl(hMacPort, ENET_MACPORT_IOCTL_PRINT_REGS, &prms);
                if (status != ENET_SOK)
                {
                    ENETTRACE_ERR(status, "Failed to print MAC port %u regs", i);
                    break;
                }
            }
        }
    }

    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Mdio_ioctl(hCpsw->hMdio, ENET_MDIO_IOCTL_PRINT_REGS, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to print MDIO regs");
    }

    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = CpswCpts_ioctl(hCpsw->hCpts, ENET_TIMESYNC_IOCTL_PRINT_REGS, &prms);
        ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to print CPTS regs");
    }

    return status;
}

#if ENET_CFG_IS_ON(CPSW_IET)
static void Cpsw_enableIet(EnetPer_Handle hPer)
{
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hPer->virtAddr;
    CSL_CPSW_CONTROL cpswControl;

    /* Enable IET in CPSW Control Reg */
    CSL_CPSW_getCpswControlReg(regs, &cpswControl);
    cpswControl.ietEnable = TRUE;
    CSL_CPSW_setCpswControlReg(regs, &cpswControl);
}

static void Cpsw_disableIet(EnetPer_Handle hPer)
{
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hPer->virtAddr;
    CSL_CPSW_CONTROL cpswControl;

    /* Disable IET in CPSW Control Reg */
    CSL_CPSW_getCpswControlReg(regs, &cpswControl);
    cpswControl.ietEnable = FALSE;
    CSL_CPSW_setCpswControlReg(regs, &cpswControl);
}
#endif
