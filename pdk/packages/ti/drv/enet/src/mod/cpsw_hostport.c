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
 * \file  cpsw_hostport.c
 *
 * \brief This file contains the implementation of the CPSW host port module.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x203

#include <stdint.h>
#include <stdarg.h>
#include <ti/csl/csl_cpswitch.h>
#include <ti/drv/enet/enet_cfg.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/include/core/enet_soc.h>
#include <ti/drv/enet/include/per/cpsw_clks.h>
#include <ti/drv/enet/include/mod/cpsw_hostport.h>
#include <ti/drv/enet/priv/core/enet_trace_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_hostport_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Supported J721E versions */
#define CPSW_HOSTPORT_VER_REVMAJ_J721E        (0x00000001U)
#define CPSW_HOSTPORT_VER_REVMIN_J721E_2G     (0x00000000U)
#define CPSW_HOSTPORT_VER_REVMIN_J721E_9G     (0x00000001U)
#define CPSW_HOSTPORT_VER_REVRTL_J721E        (0x00000000U)
#define CPSW_HOSTPORT_VER_ID_J721E            (0x00006BA8U)

/* Supported J7200 versions */
#define CPSW_HOSTPORT_VER_REVMAJ_J7200        (0x00000001U)
#define CPSW_HOSTPORT_VER_REVMIN_J7200_2G     (0x00000000U)
#define CPSW_HOSTPORT_VER_REVMIN_J7200_5G     (0x00000002U)
#define CPSW_HOSTPORT_VER_REVRTL_J7200_2G     (0x0000000EU)
#define CPSW_HOSTPORT_VER_REVRTL_J7200_5G     (0x00000004U)
#define CPSW_HOSTPORT_VER_ID_J7200            (0x00006BA8U)

/* Supported J784S4 versions */
#define CPSW_HOSTPORT_VER_REVMAJ_J784S4       (0x00000001U)
#define CPSW_HOSTPORT_VER_REVMIN_J784S4_9G    (0x00000002U)
#define CPSW_HOSTPORT_VER_REVRTL_J784S4_9G    (0x00000005U)
#define CPSW_HOSTPORT_VER_ID_J784S4           (0x00006BA8U)

/* Supported J742S2 versions */
#define CPSW_HOSTPORT_VER_REVMAJ_J742S2       (0x00000001U)
#define CPSW_HOSTPORT_VER_REVMIN_J742S2_9G    (0x00000002U)
#define CPSW_HOSTPORT_VER_REVRTL_J742S2_9G    (0x00000005U)
#define CPSW_HOSTPORT_VER_ID_J742S2           (0x00006BA8U)

/*! \brief Host port start offset in CSL register overlay. */
#define CPSW_HOSTPORT_START_REG_OFFSET        (CSL_XGE_CPSW_P0_CONTROL_REG)

/*! \brief Host port end offset in CSL register overlay. */
#define CPSW_HOSTPORT_END_REG_OFFSET          (CSL_XGE_CPSW_P0_HOST_BLKS_PRI_REG)

/*! \brief Default value used for host port RX MTU. */
#define CPSW_HOSTPORT_RX_MTU_DEFAULT          (1518U)

/*! \brief Rate limiting count to transfer rate divider. */
#define CPSW_HOSTPORT_RATELIM_DIV             (32768U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

#if ENET_CFG_IS_ON(DEV_ERROR)
static int32_t CpswHostPort_isSupported(CSL_Xge_cpswRegs *regs);
#endif

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
static void CpswHostPort_printRegs(CSL_Xge_cpswRegs *regs);
#endif

static int32_t CpswHostPort_setTxPriority(CSL_Xge_cpswRegs *regs,
                                          EnetPort_PriorityMap *txPriority);

static void CpswHostPort_getTxPriority(CSL_Xge_cpswRegs *regs,
                                       EnetPort_PriorityMap *txPriority);

static int32_t CpswHostPort_setRxPriority(CSL_Xge_cpswRegs *regs,
                                          EnetPort_PriorityMap *rxPriority);

static void CpswHostPort_getRxPriority(CSL_Xge_cpswRegs *regs,
                                       EnetPort_PriorityMap *rxPriority);

static int32_t CpswHostPort_setDscpPriority(CSL_Xge_cpswRegs *regs,
                                            EnetPort_DscpPriorityMap *dscpPriority);

static void CpswHostPort_getDscpPriority(CSL_Xge_cpswRegs *regs,
                                         EnetPort_DscpPriorityMap *dscpPriority);

#if ENET_CFG_IS_ON(CPSW_HOSTPORT_TRAFFIC_SHAPING)
static uint64_t CpswHostPort_mapBwToCnt(uint64_t rateInBps,
                                        uint32_t cppiClkFreqHz);

static uint64_t CpswHostPort_mapCntToBw(uint32_t cntVal,
                                        uint32_t cppiClkFreqHz);

static int32_t CpswHostPort_setTrafficShaping(CSL_Xge_cpswRegs *regs,
                                              const EnetPort_TrafficShapingCfg *cfg,
                                              uint32_t cppiClkFreqHz);

static void CpswHostPort_disableTrafficShaping(CSL_Xge_cpswRegs *regs);

static int32_t CpswHostPort_getTrafficShaping(CSL_Xge_cpswRegs *regs,
                                              EnetPort_TrafficShapingCfg *cfg,
                                              uint32_t cppiClkFreqHz);
#endif

static void CpswHostPort_getMaxLen(CSL_Xge_cpswRegs *regs,
                                   EnetPort_MaxLen *maxLen);

static void CpswHostPort_getFifoStats(CSL_Xge_cpswRegs *regs,
                                      CpswHostPort_FifoStats *fifoStats);

static void CpswHostPort_setEccErrTestMode(CSL_Xge_cpswRegs *regs,
                                           bool txEccErrEn,
                                           bool rxEccErrEn);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if ENET_CFG_IS_ON(DEV_ERROR)
/*! \brief CPSW hostport versions supported by this driver. */
static CSL_CPSW_VERSION CpswHostPort_gSupportedVer[] =
{
    {   /* J721E CPSW_2G */
        .majorVer = CPSW_HOSTPORT_VER_REVMAJ_J721E,
        .minorVer = CPSW_HOSTPORT_VER_REVMIN_J721E_2G,
        .rtlVer   = CPSW_HOSTPORT_VER_REVRTL_J721E,
        .id       = CPSW_HOSTPORT_VER_ID_J721E,
    },
    {   /* J721E CPSW_9G */
        .majorVer = CPSW_HOSTPORT_VER_REVMAJ_J721E,
        .minorVer = CPSW_HOSTPORT_VER_REVMIN_J721E_9G,
        .rtlVer   = CPSW_HOSTPORT_VER_REVRTL_J721E,
        .id       = CPSW_HOSTPORT_VER_ID_J721E,
    },
    {   /* J7200 CPSW_2G */
        .majorVer = CPSW_HOSTPORT_VER_REVMAJ_J7200,
        .minorVer = CPSW_HOSTPORT_VER_REVMIN_J7200_2G,
        .rtlVer   = CPSW_HOSTPORT_VER_REVRTL_J7200_2G,
        .id       = CPSW_HOSTPORT_VER_ID_J7200,
    },
    {   /* J7200 CPSW_5G */
        .majorVer = CPSW_HOSTPORT_VER_REVMAJ_J7200,
        .minorVer = CPSW_HOSTPORT_VER_REVMIN_J7200_5G,
        .rtlVer   = CPSW_HOSTPORT_VER_REVRTL_J7200_5G,
        .id       = CPSW_HOSTPORT_VER_ID_J7200,
    },
    {   /* J784S4 CPSW_9G */
        .majorVer = CPSW_HOSTPORT_VER_REVMAJ_J784S4,
        .minorVer = CPSW_HOSTPORT_VER_REVMIN_J784S4_9G,
        .rtlVer   = CPSW_HOSTPORT_VER_REVRTL_J784S4_9G,
        .id       = CPSW_HOSTPORT_VER_ID_J784S4,
    },
    {   /* J742S2 CPSW_9G */
        .majorVer = CPSW_HOSTPORT_VER_REVMAJ_J742S2,
        .minorVer = CPSW_HOSTPORT_VER_REVMIN_J742S2_9G,
        .rtlVer   = CPSW_HOSTPORT_VER_REVRTL_J742S2_9G,
        .id       = CPSW_HOSTPORT_VER_ID_J742S2,
    },
};

/* Public host port IOCTL validation data. */
static Enet_IoctlValidate gCpswHostPort_ioctlValidate[] =
{
    ENET_IOCTL_VALID_PRMS(CPSW_HOSTPORT_IOCTL_GET_FIFO_STATS,
                          0U,
                          sizeof(CpswHostPort_FifoStats)),

    ENET_IOCTL_VALID_PRMS(CPSW_HOSTPORT_GET_FLOW_ID_OFFSET,
                          0U,
                          sizeof(uint32_t)),
};

/* Private host port IOCTL validation data. */
static Enet_IoctlValidate gCpswHostPort_privIoctlValidate[] =
{
    ENET_IOCTL_VALID_PRMS(CPSW_HOSTPORT_SET_FLOW_ID_OFFSET,
                          sizeof(uint32_t),
                          0U),
};
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void CpswHostPort_initCfg(CpswHostPort_Cfg *hostPortCfg)
{
    hostPortCfg->crcType           = ENET_CRC_ETHERNET;
    hostPortCfg->removeCrc         = BFALSE;
    hostPortCfg->padShortPacket    = BTRUE; /* Short packets were dropped by default */
    hostPortCfg->passCrcErrors     = BFALSE;
    hostPortCfg->passPriorityTaggedUnchanged = BFALSE;
    hostPortCfg->rxVlanRemapEn     = BFALSE;
    hostPortCfg->rxDscpIPv4RemapEn = BFALSE;
    hostPortCfg->rxDscpIPv6RemapEn = BFALSE;
    hostPortCfg->rxMtu             = CPSW_HOSTPORT_RX_MTU_DEFAULT;
    hostPortCfg->vlanCfg.portPri   = 0U;
    hostPortCfg->vlanCfg.portCfi   = 0U;
    hostPortCfg->vlanCfg.portVID   = 0U;
    hostPortCfg->rxPriorityType    = ENET_INGRESS_PRI_TYPE_FIXED;
    hostPortCfg->txPriorityType    = ENET_EGRESS_PRI_TYPE_FIXED;

    /* Keep checksum offload enabled by default as no side-effect if app doesn't use it */
    hostPortCfg->csumOffloadEn     = BTRUE;
}

int32_t CpswHostPort_open(EnetMod_Handle hMod,
                          Enet_Type enetType,
                          uint32_t instId,
                          const void *cfg,
                          uint32_t cfgSize)
{
    CpswHostPort_Handle hPort = (CpswHostPort_Handle)hMod;
    const CpswHostPort_Cfg *hostPortCfg = (const CpswHostPort_Cfg *)cfg;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    CSL_CPSW_CONTROL control;
    CSL_CPSW_PTYPE pType;
    CSL_CPSW_CPPI_P0_CONTROL cppiP0ControlCfg;
    uint32_t status = ENET_SOK;

    Enet_devAssert(cfgSize == sizeof(CpswHostPort_Cfg),
                   "Invalid host port config params size %u (expected %u)",
                   cfgSize, sizeof(CpswHostPort_Cfg));

    Enet_devAssert(regs != NULL, "CPSW hostport regs address is not valid");

    /* Check supported host port module versions */
#if ENET_CFG_IS_ON(DEV_ERROR)
    status = CpswHostPort_isSupported(regs);
    Enet_devAssert(status == ENET_SOK, "Host port version is not supported");
#endif

    /* Save peripheral info to use it later to query SoC parameters */
    hPort->enetType = enetType;
    hPort->instId = instId;

    if (hostPortCfg->crcType == ENET_CRC_ETHERNET)
    {
        CSL_CPSW_disableP0TxCastagnoliCRC(regs);
    }
    else
    {
        CSL_CPSW_enableP0TxCastagnoliCRC(regs);
    }

    CSL_CPSW_getCpswControlReg(regs, &control);
    control.p0Enable       = UFALSE;
    control.p0PassPriTag   = hostPortCfg->passPriorityTaggedUnchanged;
    control.p0TxCrcRemove  = hostPortCfg->removeCrc;
    control.p0RxPad        = hostPortCfg->padShortPacket;
    control.p0RxPassCrcErr = hostPortCfg->passCrcErrors;
    CSL_CPSW_setCpswControlReg(regs, &control);

    CSL_CPSW_setPort0VlanReg(regs,
                             hostPortCfg->vlanCfg.portVID,
                             hostPortCfg->vlanCfg.portCfi,
                             hostPortCfg->vlanCfg.portPri);

    CSL_CPSW_setPort0RxMaxLen(regs, hostPortCfg->rxMtu);

    CSL_CPSW_getPTypeReg(regs, &pType);
    if (hostPortCfg->txPriorityType == ENET_EGRESS_PRI_TYPE_FIXED)
    {
        pType.port0PriorityTypeEscalateEnable = UFALSE;
    }
    else
    {
        pType.port0PriorityTypeEscalateEnable = UTRUE;
    }

    CSL_CPSW_setPTypeReg(regs, &pType);

    CSL_CPSW_getCppiP0Control(regs, &cppiP0ControlCfg);

    if (hostPortCfg->csumOffloadEn)
    {
        cppiP0ControlCfg.p0RxChksumEn = UTRUE;
    }
    else
    {
        cppiP0ControlCfg.p0RxChksumEn = UFALSE;
    }

    cppiP0ControlCfg.p0RxRemapVlan     = hostPortCfg->rxVlanRemapEn     ? UTRUE : UFALSE;
    cppiP0ControlCfg.p0RxRemapDscpIpv4 = hostPortCfg->rxDscpIPv4RemapEn ? UTRUE : UFALSE;
    cppiP0ControlCfg.p0RxRemapDscpIpv6 = hostPortCfg->rxDscpIPv6RemapEn ? UTRUE : UFALSE;

    CSL_CPSW_setCppiP0Control(regs, &cppiP0ControlCfg);

    return status;
}

int32_t CpswHostPort_rejoin(EnetMod_Handle hMod,
                            Enet_Type enetType,
                            uint32_t instId)
{
    CpswHostPort_Handle hPort = (CpswHostPort_Handle)hMod;

    /* Save peripheral info to use it later to query SoC parameters */
    hPort->enetType = enetType;
    hPort->instId = instId;

    return ENET_SOK;
}

void CpswHostPort_close(EnetMod_Handle hMod)
{
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;

    Enet_devAssert(regs != NULL, "CPSW hostport regs address is not valid");

    /* Disable host port */
    CSL_CPSW_disablePort0(regs);
}

void CpswHostPort_saveCtxt(EnetMod_Handle hMod)
{
    CpswHostPort_Handle hPort = (CpswHostPort_Handle)hMod;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    uint32_t i;

    hPort->ctxt.control      =  CSL_REG32_RD(&regs->P0_CONTROL_REG);
    hPort->ctxt.flowIdOffset =  CSL_REG32_RD(&regs->P0_FLOW_ID_OFFSET_REG);
    hPort->ctxt.portVlan     =  CSL_REG32_RD(&regs->P0_PORT_VLAN_REG);
    hPort->ctxt.txPriMap     =  CSL_REG32_RD(&regs->P0_TX_PRI_MAP_REG);
    hPort->ctxt.priCtl       =  CSL_REG32_RD(&regs->P0_PRI_CTL_REG);
    hPort->ctxt.rxPriMap     =  CSL_REG32_RD(&regs->P0_RX_PRI_MAP_REG);
    hPort->ctxt.rxMaxlen     =  CSL_REG32_RD(&regs->P0_RX_MAXLEN_REG);
    hPort->ctxt.rxPktsPri    =  CSL_REG32_RD(&regs->P0_RX_PKTS_PRI_REG);
    hPort->ctxt.rxGap        =  CSL_REG32_RD(&regs->P0_RX_GAP_REG);

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        hPort->ctxt.rxDscpMap[i] =  CSL_REG32_RD(&regs->P0_RX_DSCP_MAP_REG[i]);
        hPort->ctxt.priCir[i]    =  CSL_REG32_RD(&regs->P0_PRI_CIR_REG[i]);
        hPort->ctxt.priEir[i]    =  CSL_REG32_RD(&regs->P0_PRI_EIR_REG[i]);
    }

    CSL_CPSW_disablePort0(regs);
}

int32_t CpswHostPort_restoreCtxt(EnetMod_Handle hMod)
{
    CpswHostPort_Handle hPort = (CpswHostPort_Handle)hMod;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    uint32_t i;

    CSL_CPSW_enablePort0(regs);

    CSL_REG32_WR(&regs->P0_CONTROL_REG, hPort->ctxt.control);
    CSL_REG32_WR(&regs->P0_PORT_VLAN_REG, hPort->ctxt.portVlan);
    CSL_REG32_WR(&regs->P0_RX_MAXLEN_REG, hPort->ctxt.rxMaxlen);
    CSL_REG32_WR(&regs->P0_FLOW_ID_OFFSET_REG, hPort->ctxt.flowIdOffset);
    CSL_REG32_WR(&regs->P0_TX_PRI_MAP_REG, hPort->ctxt.txPriMap);
    CSL_REG32_WR(&regs->P0_PRI_CTL_REG, hPort->ctxt.priCtl);
    CSL_REG32_WR(&regs->P0_RX_PRI_MAP_REG, hPort->ctxt.rxPriMap);
    CSL_REG32_WR(&regs->P0_RX_PKTS_PRI_REG, hPort->ctxt.rxPktsPri);
    CSL_REG32_WR(&regs->P0_RX_GAP_REG,  hPort->ctxt.rxGap);

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        CSL_REG32_WR(&regs->P0_RX_DSCP_MAP_REG[i],  hPort->ctxt.rxDscpMap[i]);
        CSL_REG32_WR(&regs->P0_PRI_CIR_REG[i],  hPort->ctxt.priCir[i]);
        CSL_REG32_WR(&regs->P0_PRI_EIR_REG[i],  hPort->ctxt.priEir[i]);
    }

    return ENET_SOK;
}

int32_t CpswHostPort_ioctl(EnetMod_Handle hMod,
                           uint32_t cmd,
                           Enet_IoctlPrms *prms)
{
#if ENET_CFG_IS_ON(CPSW_HOSTPORT_TRAFFIC_SHAPING)
    CpswHostPort_Handle hPort = (CpswHostPort_Handle)hMod;
#endif
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    int32_t status = ENET_SOK;

#if ENET_CFG_IS_ON(DEV_ERROR)
    /* Validate CPSW host port IOCTL parameters */
    if (ENET_IOCTL_GET_PER(cmd) == ENET_IOCTL_PER_CPSW)
    {
        if (ENET_IOCTL_GET_TYPE(cmd) == ENET_IOCTL_TYPE_PUBLIC)
        {
            status = Enet_validateIoctl(cmd, prms,
                                        gCpswHostPort_ioctlValidate,
                                        ENET_ARRAYSIZE(gCpswHostPort_ioctlValidate));
        }
        else
        {
            status = Enet_validateIoctl(cmd, prms,
                                        gCpswHostPort_privIoctlValidate,
                                        ENET_ARRAYSIZE(gCpswHostPort_privIoctlValidate));
        }

        ENETTRACE_ERR_IF(status != ENET_SOK, status, "IOCTL 0x%08x params are not valid", cmd);
    }
#endif

    if (status == ENET_SOK)
    {
        Enet_devAssert(regs != NULL, "CPSW hostport regs address is not valid");

        switch (cmd)
        {
            case ENET_HOSTPORT_IOCTL_GET_VERSION:
            {
                Enet_Version *version = (Enet_Version *)prms->outArgs;
                CSL_CPSW_VERSION ver;

                /* Report CPSW Control version as ours */
                CSL_CPSW_getCpswVersionInfo(regs, &ver);
                version->maj = ver.majorVer;
                version->min = ver.minorVer;
                version->rtl = ver.rtlVer;
                version->id  = ver.id;
                version->other1 = ENET_VERSION_NONE;
                version->other2 = ENET_VERSION_NONE;
            }
            break;

            case ENET_HOSTPORT_IOCTL_PRINT_REGS:
            {
#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
                CpswHostPort_printRegs(regs);
#endif
            }
            break;

            case ENET_HOSTPORT_IOCTL_ENABLE:
            {
                CSL_CPSW_enablePort0(regs);
            }
            break;

            case ENET_HOSTPORT_IOCTL_DISABLE:
            {
                CSL_CPSW_disablePort0(regs);
            }
            break;

            case ENET_HOSTPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP:
            {
                EnetPort_DscpPriorityMap *dscpPriority = (EnetPort_DscpPriorityMap *)prms->inArgs;

                status = CpswHostPort_setDscpPriority(regs, dscpPriority);
                ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to set DSCP priority");
            }
            break;

            case ENET_HOSTPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP:
            {
                EnetPort_DscpPriorityMap *dscpPriority = (EnetPort_DscpPriorityMap *)prms->outArgs;

                CpswHostPort_getDscpPriority(regs, dscpPriority);
            }
            break;

            case ENET_HOSTPORT_IOCTL_SET_PRI_REGEN_MAP:
            {
                EnetPort_PriorityMap *rxPriority = (EnetPort_PriorityMap *)prms->inArgs;

                status = CpswHostPort_setRxPriority(regs, rxPriority);
                ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to set RX priority");
            }
            break;

            case ENET_HOSTPORT_IOCTL_GET_PRI_REGEN_MAP:
            {
                EnetPort_PriorityMap *rxPriority = (EnetPort_PriorityMap *)prms->outArgs;

                CpswHostPort_getRxPriority(regs, rxPriority);
            }
            break;

            case ENET_HOSTPORT_IOCTL_SET_EGRESS_QOS_PRI_MAP:
            {
                EnetPort_PriorityMap *txPriority = (EnetPort_PriorityMap *)prms->inArgs;

                status = CpswHostPort_setTxPriority(regs, txPriority);
                ENETTRACE_ERR_IF(status != ENET_SOK, status, "failed to set TX priority");
            }
            break;

            case ENET_HOSTPORT_IOCTL_GET_EGRESS_QOS_PRI_MAP:
            {
                EnetPort_PriorityMap *txPriority = (EnetPort_PriorityMap *)prms->outArgs;

                CpswHostPort_getTxPriority(regs, txPriority);
            }
            break;

            case ENET_HOSTPORT_IOCTL_ENABLE_INGRESS_TRAFFIC_SHAPING:
            {
#if ENET_CFG_IS_ON(CPSW_HOSTPORT_TRAFFIC_SHAPING)
                EnetPort_TrafficShapingCfg *shapingCfg = (EnetPort_TrafficShapingCfg *)prms->inArgs;
                uint32_t cppiClkFreqHz;

                cppiClkFreqHz = EnetSoc_getClkFreq(hPort->enetType, hPort->instId, CPSW_CPPI_CLK);

                status = CpswHostPort_setTrafficShaping(regs, shapingCfg, cppiClkFreqHz);
                ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to set traffic shaping");
#else
                status = ENET_ENOTSUPPORTED;
                ENETTRACE_ERR(status, "Traffic shapping is not enabled (see CPSW_HOSTPORT_TRAFFIC_SHAPING)");
#endif
            }
            break;

            case ENET_HOSTPORT_IOCTL_DISABLE_INGRESS_TRAFFIC_SHAPING:
            {
#if ENET_CFG_IS_ON(CPSW_HOSTPORT_TRAFFIC_SHAPING)
                CpswHostPort_disableTrafficShaping(regs);
#else
                status = ENET_ENOTSUPPORTED;
                ENETTRACE_ERR(status, "Traffic shapping is not enabled (see CPSW_HOSTPORT_TRAFFIC_SHAPING)");
#endif
            }
            break;

            case ENET_HOSTPORT_IOCTL_GET_INGRESS_TRAFFIC_SHAPING:
            {
#if ENET_CFG_IS_ON(CPSW_HOSTPORT_TRAFFIC_SHAPING)
                EnetPort_TrafficShapingCfg *shapingCfg = (EnetPort_TrafficShapingCfg *)prms->outArgs;
                uint32_t cppiClkFreqHz;

                cppiClkFreqHz = EnetSoc_getClkFreq(hPort->enetType, hPort->instId, CPSW_CPPI_CLK);

                status = CpswHostPort_getTrafficShaping(regs, shapingCfg, cppiClkFreqHz);
                ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to get traffic shaping");
#else
                status = ENET_ENOTSUPPORTED;
                ENETTRACE_ERR(status, "Traffic shapping is not enabled (see CPSW_HOSTPORT_TRAFFIC_SHAPING)");
#endif
            }
            break;

            case ENET_HOSTPORT_IOCTL_GET_MAXLEN:
            {
                EnetPort_MaxLen *maxLen = (EnetPort_MaxLen *)prms->inArgs;

                CpswHostPort_getMaxLen(regs, maxLen);
            }
            break;

            case ENET_HOSTPORT_IS_CSUM_OFFLOAD_ENABLED:
            {
                bool *csumEn = (bool *)prms->outArgs;
                CSL_CPSW_CPPI_P0_CONTROL cppiP0ControlCfg;

                CSL_CPSW_getCppiP0Control(regs, &cppiP0ControlCfg);
                *csumEn = (cppiP0ControlCfg.p0RxChksumEn != 0U);
            }
            break;

            case CPSW_HOSTPORT_IOCTL_GET_FIFO_STATS:
            {
                CpswHostPort_FifoStats *fifoStats = (CpswHostPort_FifoStats *)prms->outArgs;

                CpswHostPort_getFifoStats(regs, fifoStats);
            }
            break;

            case CPSW_HOSTPORT_GET_FLOW_ID_OFFSET:
            {
                uint32_t *flowIdOffset = (uint32_t *)prms->outArgs;

                *flowIdOffset = CSL_CPSW_getPort0FlowIdOffset(regs);
            }
            break;

            case CPSW_HOSTPORT_SET_FLOW_ID_OFFSET:
            {
                uint32_t *flowIdOffset = (uint32_t *)prms->inArgs;

                CSL_CPSW_setPort0FlowIdOffset(regs, *flowIdOffset);
            }
            break;

            case ENET_HOSTPORT_IOCTL_SET_ECC_TEST_MODE:
            {
                EnetHostPort_SetEccTestModeInArgs *inArgs =
                    (EnetHostPort_SetEccTestModeInArgs *)prms->inArgs;

                /* Enable or disable ECC error injection test capability */
                CpswHostPort_setEccErrTestMode(regs,
                                              inArgs->txEccErrEn,
                                              inArgs->rxEccErrEn);
            }
            break;

            default:
            {
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status, "Invalid IOCTL cmd 0x%08x", cmd);
            }
            break;
        }
    }

    return status;
}

#if ENET_CFG_IS_ON(DEV_ERROR)
static int32_t CpswHostPort_isSupported(CSL_Xge_cpswRegs *regs)
{
    CSL_CPSW_VERSION version;
    uint32_t i;
    int32_t status = ENET_ENOTSUPPORTED;

    CSL_CPSW_getCpswVersionInfo(regs, &version);

    for (i = 0U; i < ENET_ARRAYSIZE(CpswHostPort_gSupportedVer); i++)
    {
        if ((version.majorVer == CpswHostPort_gSupportedVer[i].majorVer) &&
            (version.minorVer == CpswHostPort_gSupportedVer[i].minorVer) &&
            (version.rtlVer == CpswHostPort_gSupportedVer[i].rtlVer) &&
            (version.id == CpswHostPort_gSupportedVer[i].id))
        {
            status = ENET_SOK;
            break;
        }
    }

    return status;
}
#endif

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
static void CpswHostPort_printRegs(CSL_Xge_cpswRegs *regs)
{
    uint32_t *regAddr = (uint32_t *)((uintptr_t)regs + CPSW_HOSTPORT_START_REG_OFFSET);
    uint32_t regIdx = 0;

    while ((uintptr_t)regAddr <= ((uintptr_t)regs + CPSW_HOSTPORT_END_REG_OFFSET))
    {
        if (*regAddr != 0U)
        {
            ENETTRACE_INFO("HOSTPORT: %u: 0x%08x", regIdx, *regAddr);
        }

        regAddr++;
        regIdx++;
    }
}
#endif

static int32_t CpswHostPort_setTxPriority(CSL_Xge_cpswRegs *regs,
                                          EnetPort_PriorityMap *txPriority)
{
    int32_t status = ENET_SOK;
    uint32_t i;

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        if (txPriority->priorityMap[i] > ENET_PRI_MAX)
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status, "Invalid priority map %u -> %u", i, txPriority->priorityMap[i]);
            break;
        }
    }

    if (status == ENET_SOK)
    {
        CSL_CPSW_setPortTxPriMapReg(regs, 0U, txPriority->priorityMap);
    }

    return status;
}

static void CpswHostPort_getTxPriority(CSL_Xge_cpswRegs *regs,
                                       EnetPort_PriorityMap *txPriority)
{
    CSL_CPSW_getPortTxPriMapReg(regs, 0U, txPriority->priorityMap);
}

static int32_t CpswHostPort_setRxPriority(CSL_Xge_cpswRegs *regs,
                                          EnetPort_PriorityMap *rxPriority)
{
    int32_t status = ENET_SOK;
    uint32_t i;

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        if (rxPriority->priorityMap[i] > ENET_PRI_MAX)
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status, "Invalid priority map %u -> %u", i, rxPriority->priorityMap[i]);
            break;
        }
    }

    if (status == ENET_SOK)
    {
        CSL_CPSW_setPortRxPriMapReg(regs, 0U, rxPriority->priorityMap);
    }

    return status;
}

static void CpswHostPort_getRxPriority(CSL_Xge_cpswRegs *regs,
                                       EnetPort_PriorityMap *rxPriority)
{
    CSL_CPSW_getPortRxPriMapReg(regs, 0U, rxPriority->priorityMap);
}

static int32_t CpswHostPort_setDscpPriority(CSL_Xge_cpswRegs *regs,
                                            EnetPort_DscpPriorityMap *dscpPriority)
{
    CSL_CPSW_PORT_CONTROL control;
    int32_t status = ENET_SOK;
    uint32_t i;

    for (i = 0U; i < ENET_TOS_PRI_NUM; i++)
    {
        if (dscpPriority->tosMap[i] > ENET_PRI_MAX)
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status, "Invalid priority map %u -> %u", i, dscpPriority->tosMap[i]);
            break;
        }
    }

    if (status == ENET_SOK)
    {
        CSL_CPSW_setPortRxDscpMap(regs, 0U, dscpPriority->tosMap);

        CSL_CPSW_getPortControlReg(regs, 0U, &control);

        control.dscpIpv4Enable = dscpPriority->dscpIPv4En;
        control.dscpIpv6Enable = dscpPriority->dscpIPv6En;

        CSL_CPSW_setPortControlReg(regs, 0U, &control);
    }

    return status;
}

static void CpswHostPort_getDscpPriority(CSL_Xge_cpswRegs *regs,
                                         EnetPort_DscpPriorityMap *dscpPriority)
{
    CSL_CPSW_PORT_CONTROL control;

    CSL_CPSW_getPortRxDscpMap(regs, 0U, dscpPriority->tosMap);

    CSL_CPSW_getPortControlReg(regs, 0U, &control);

    dscpPriority->dscpIPv4En = control.dscpIpv4Enable;
    dscpPriority->dscpIPv6En = control.dscpIpv6Enable;
}

#if ENET_CFG_IS_ON(CPSW_HOSTPORT_TRAFFIC_SHAPING)
static uint64_t CpswHostPort_mapBwToCnt(uint64_t rateInBps,
                                        uint32_t cppiClkFreqHz)
{
    /*
     * Transfer rate to CIR or EIR count value equation:
     *
     *                Transfer rate (Mbps) * 32768
     * Count value = ------------------------------
     *                   CPPI_ICLK freq (MHz)
     *
     * Round division to prevent transfer rates below traffic shaping resolution
     * end up with no shaping at all.
     */
    return ENET_DIV_ROUNDUP(rateInBps * CPSW_HOSTPORT_RATELIM_DIV, cppiClkFreqHz);
}

static uint64_t CpswHostPort_mapCntToBw(uint32_t cntVal,
                                        uint32_t cppiClkFreqHz)
{
    /*
     * CIR or EIR count value to transfer rate equation:
     *
     *                         count value * CPPI_ICLK freq (MHz)
     * Transfer rate (Mbps) = ------------------------------------
     *                                      32768
     */
    return ((uint64_t)cntVal * cppiClkFreqHz) / CPSW_HOSTPORT_RATELIM_DIV;
}

static int32_t CpswHostPort_setTrafficShaping(CSL_Xge_cpswRegs *regs,
                                              const EnetPort_TrafficShapingCfg *cfg,
                                              uint32_t cppiClkFreqHz)
{
    uint64_t cirBps, eirBps;
    uint64_t cir, eir;
    uint32_t i;
    int32_t status = ENET_SOK;
    bool rateLimEnabled = BFALSE;

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        cirBps = cfg->rates[i].committedRateBitsPerSec;
        eirBps = cfg->rates[i].excessRateBitsPerSec;
        cir = CpswHostPort_mapBwToCnt(cirBps, cppiClkFreqHz);
        eir = CpswHostPort_mapBwToCnt(eirBps, cppiClkFreqHz);

        /* CIR must be non-zero if EIR is non-zero */
        if ((cir == 0ULL) && (eir != 0ULL))
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status,
                          "EIR is enabled (%llubps = %llu) but CIR is not (%llubps = %llu) for priority %u",
                          eirBps, eir, cirBps, cir, i);
            break;
        }

        /* Rate limit must be the highest priority channels */
        if ((cir == 0ULL) && rateLimEnabled)
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status, "Rate limiting disabled for priority %u", i);
            break;
        }

        /* Out of range */
        if ((cir > 0x0FFFFFFFULL) || (eir > 0x0FFFFFFFULL))
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status, "Invalid CIR=%llu (%llubps) EIR=%llu (%llubps) for priority %u",
                          cir, cirBps, eir, eirBps, i);
            break;
        }

        ENETTRACE_DBG("Rate limiting %s for priority %u, CIR=%ubps (%llu) EIR=%ubps (%llu)",
                      (cir != 0ULL) ? "enabled" : "disabled",
                      i, cirBps, cir, eirBps, eir);

        CSL_CPSW_setCppiPriCirEir(regs, i, (uint32_t)cir, (uint32_t)eir);

        if (cir != 0ULL)
        {
            rateLimEnabled = BTRUE;
        }
    }

    return status;
}

static void CpswHostPort_disableTrafficShaping(CSL_Xge_cpswRegs *regs)
{
    uint32_t i;

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        CSL_CPSW_setCppiPriCirEir(regs, i, 0U, 0U);
    }
}

static int32_t CpswHostPort_getTrafficShaping(CSL_Xge_cpswRegs *regs,
                                              EnetPort_TrafficShapingCfg *cfg,
                                              uint32_t cppiClkFreqHz)
{
    uint32_t cir, eir;
    uint64_t cirBps, eirBps;
    uint32_t i;
    int32_t status = ENET_SOK;

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        CSL_CPSW_getCppiPriCirEir(regs, i, &cir, &eir);

        cirBps = CpswHostPort_mapCntToBw(cir, cppiClkFreqHz);
        eirBps = CpswHostPort_mapCntToBw(eir, cppiClkFreqHz);

        /* CIR must be non-zero if EIR is non-zero */
        if ((cirBps == 0ULL) && (eirBps != 0ULL))
        {
            status = ENET_EUNEXPECTED;
            ENETTRACE_ERR(status,
                          "EIR is enabled (%llubps = %u) but CIR is not (%llubps = %u) for priority %u",
                          eirBps, eir, cirBps, cir, i);
            break;
        }

        ENETTRACE_DBG("Rate limiting %s for priority %u, CIR=%llubps (%u) EIR=%llubps (%u)",
                      (cir != 0U) ? "enabled" : "disabled",
                      i, cirBps, cir, eirBps, eir);

        cfg->rates[i].committedRateBitsPerSec = cirBps;
        cfg->rates[i].excessRateBitsPerSec = eirBps;
    }

    return status;
}
#endif

static void CpswHostPort_getMaxLen(CSL_Xge_cpswRegs *regs,
                                   EnetPort_MaxLen *maxLen)
{
    uint32_t i;

    maxLen->mru = CSL_CPSW_getPort0RxMaxLen(regs);

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        maxLen->mtu[i] = CSL_CPSW_getTxMaxLenPerPriority(regs, i);
    }
}

static void CpswHostPort_getFifoStats(CSL_Xge_cpswRegs *regs,
                                      CpswHostPort_FifoStats *fifoStats)
{
    CSL_CPSW_THRURATE thruRate;
    CSL_CPSW_CPPI_P0_FIFOSTATUS p0FifoStatus;

    CSL_CPSW_getPortBlockCountReg(regs, 0U,
                                  &fifoStats->rxBlockCountExpress,
                                  &fifoStats->rxBlockCountPreempt,
                                  &fifoStats->txBlockCount);

    CSL_CPSW_getThruRateReg(regs, &thruRate);

    fifoStats->rxThroughputRate = thruRate.cppiRxThruRate;

    CSL_CPSW_getP0FifoStatus(regs, &p0FifoStatus);

    fifoStats->txActiveFifo[0U] = p0FifoStatus.p0TxPriActivePri0;
    fifoStats->txActiveFifo[1U] = p0FifoStatus.p0TxPriActivePri1;
    fifoStats->txActiveFifo[2U] = p0FifoStatus.p0TxPriActivePri2;
    fifoStats->txActiveFifo[3U] = p0FifoStatus.p0TxPriActivePri3;
    fifoStats->txActiveFifo[4U] = p0FifoStatus.p0TxPriActivePri4;
    fifoStats->txActiveFifo[5U] = p0FifoStatus.p0TxPriActivePri5;
    fifoStats->txActiveFifo[6U] = p0FifoStatus.p0TxPriActivePri6;
    fifoStats->txActiveFifo[7U] = p0FifoStatus.p0TxPriActivePri7;
}

static void CpswHostPort_setEccErrTestMode(CSL_Xge_cpswRegs *regs,
                                           bool txEccErrEn,
                                           bool rxEccErrEn)
{
    CSL_CPSW_PORT_CONTROL control;

    CSL_CPSW_getPortControlReg(regs, 0U, &control);
    control.rxEccErrEnable = rxEccErrEn;
    control.txEccErrEnable = txEccErrEn;
    CSL_CPSW_setPortControlReg(regs, 0U, &control);
}
