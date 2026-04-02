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
 * \file  mdio.c
 *
 * \brief This file contains the implementation of the MDIO module.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x205

#include <stdint.h>
#include <stdarg.h>
#include <ti/csl/csl_cpswitch.h>
#include <ti/drv/enet/enet_cfg.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/include/core/enet_soc.h>
#include <ti/drv/enet/include/per/cpsw_clks.h>
#include <ti/drv/enet/include/mod/mdio.h>
#include <ti/drv/enet/priv/core/enet_trace_priv.h>
#include <ti/drv/enet/priv/mod/mdio_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*! \brief Supported MDIO versions. */
#define MDIO_VER_MODID_J7X                    (0x00000007U)
#define MDIO_VER_REVMAJ_J7X                   (0x00000001U)
#define MDIO_VER_REVMIN_J7X                   (0x00000007U)
#define MDIO_VER_REVRTL_J7X                   (0x00000001U)

/*! \brief Max number of PHYs that can be monitored by MDIO in normal mode. */
#define MDIO_PHY_MONITOR_MAX                  (2U)

/*! \brief Default MDIO bus frequency. */
#define MDIO_MDIOBUS_DFLT_FREQ_HZ             (2200000U)

/*! \brief BMSR register address. */
#define PHY_BMSR                              (0x01U)

/*! \brief Link status bit mask in PHY Basic Mode Status Register (BMSR) */
#define LINK_STATUS_BITMASK                   (0x04U)

/*! \brief Convert the seconds to nanoseconds. */
#define SEC_TO_NANOSEC                        (1000000000ULL)

/*! \brief Maximum number of C22 read/write attempts to do before erroring */
#define MDIO_PHY_REG_RW_NUM_RETRIES           (200000U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

#if ENET_CFG_IS_ON(DEV_ERROR)
static int32_t Mdio_isSupported(CSL_mdioHandle mdioRegs);
#endif

static void Mdio_manualModeFieldSend(EnetMod_Handle hMod,
                                    uint32_t iMsb,
                                    uint32_t iVal);

static uint32_t  Mdio_manualModePhyRegRead22(EnetMod_Handle hMod,
                                        uint32_t phyAddr,
                                        uint32_t regNum,
                                        uint16_t *pData);

static void Mdio_manualModePhyRegWrite22(EnetMod_Handle hMod,
                                  uint32_t phyAddr,
                                  uint32_t regNum,
                                  uint16_t wrVal);

static void Mdio_manualModeToggleMdclk(CSL_mdioHandle hMdioRegs, const uint32_t cpuDelay_ticks);

static int32_t Mdio_setupNormalMode(CSL_mdioHandle mdioRegs,
                                    const Mdio_Cfg *cfg);

static void Mdio_setupStatusChangeMode(CSL_mdioHandle mdioRegs,
                                       const Mdio_Cfg *cfg);

static void Mdio_setupManualMode(CSL_mdioHandle mdioRegs);

static int32_t Mdio_PhyRegReadC22(EnetMod_Handle hMod,
                                   uint32_t userGroup,
                                   uint32_t phyAddr,
                                   uint32_t regAddr,
                                   uint16_t *outVal);

static int32_t Mdio_PhyRegWriteC22(EnetMod_Handle hMod,
                                   uint32_t userGroup,
                                   uint32_t phyAddr,
                                   uint32_t regAddr,
                                   uint16_t val);

#if ENET_CFG_IS_ON(MDIO_CLAUSE45)
static int32_t Mdio_readRegC45(EnetMod_Handle hMod,
                               uint32_t userGroup,
                               uint32_t mmdNum,
                               uint32_t phyAddr,
                               uint32_t regAddr,
                               uint16_t *outVal);

static int32_t Mdio_writeRegC45(EnetMod_Handle hMod,
                                uint32_t userGroup,
                                uint32_t mmdNum,
                                uint32_t phyAddr,
                                uint32_t regAddr,
                                uint16_t val);
#endif

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
static void Mdio_printRegs(CSL_mdioHandle mdioRegs);
#endif

static void Mdio_handleIntr(CSL_mdioHandle mdioRegs,
                            Mdio_Callbacks *callbacks);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if ENET_CFG_IS_ON(DEV_ERROR)
/*! \brief MDIO versions supported by this driver. */
static CSL_MDIO_VERSION gMdio_supportedVer[] =
{
    {   /* J7x devices */
        .modId  = MDIO_VER_MODID_J7X,
        .revMaj = MDIO_VER_REVMAJ_J7X,
        .revMin = MDIO_VER_REVMIN_J7X,
        .revRtl = MDIO_VER_REVRTL_J7X,
    },
};

/* Private MDIO IOCTL validation data. */
static Enet_IoctlValidate gMdio_privIoctlValidate[] =
{
    ENET_IOCTL_VALID_PRMS(MDIO_IOCTL_HANDLE_INTR,
                          sizeof(Mdio_Callbacks),
                          0U),
};
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Mdio_initCfg(Mdio_Cfg *mdioCfg)
{
#if defined(ENABLE_MDIO_MANUAL_MODE)
    mdioCfg->mode               = MDIO_MODE_MANUAL;
    mdioCfg->pollEnMask         = ENET_MDIO_PHY_ADDR_MASK_NONE;
#else
    mdioCfg->mode               = MDIO_MODE_STATE_CHANGE_MON;
    mdioCfg->pollEnMask         = ENET_MDIO_PHY_ADDR_MASK_ALL;
#endif
    mdioCfg->mdioBusFreqHz      = MDIO_MDIOBUS_DFLT_FREQ_HZ;
    mdioCfg->phyStatePollFreqHz = mdioCfg->mdioBusFreqHz;
    mdioCfg->c45EnMask          = ENET_MDIO_PHY_ADDR_MASK_NONE;
    mdioCfg->isMaster           = BTRUE;
    mdioCfg->numRetries    = MDIO_PHY_REG_RW_NUM_RETRIES;
}

int32_t Mdio_open(EnetMod_Handle hMod,
                  Enet_Type enetType,
                  uint32_t instId,
                  const void *cfg,
                  uint32_t cfgSize)
{
    Mdio_Handle hMdio = (Mdio_Handle)hMod;
    const Mdio_Cfg *mdioCfg = (const Mdio_Cfg *)cfg;
    CSL_mdioHandle mdioRegs = (CSL_mdioHandle)hMod->virtAddr;
    uint32_t cppiClkFreqHz;
    uint32_t clkdiv;
    uint32_t ipgRatio;
    int32_t status = ENET_SOK;

    Enet_devAssert(cfgSize == sizeof(Mdio_Cfg),
                   "Invalid MDIO config params size %u (expected %u)",
                   cfgSize, sizeof(Mdio_Cfg));

    Enet_devAssert(mdioRegs != NULL, "MDIO reg address is not valid");

    /* Check supported MDIO module versions */
#if ENET_CFG_IS_ON(DEV_ERROR)
    status = Mdio_isSupported(mdioRegs);
    Enet_devAssert(status == ENET_SOK, "MDIO version is not supported");
#endif

    hMdio->isMaster = mdioCfg->isMaster;
    hMdio->numRetries = mdioCfg->numRetries;

    /* Perform global MDIO configuration only for module in master role */
    if (hMdio->isMaster)
    {
        /* Compute MDIO clock divider */
        if (status == ENET_SOK)
        {
            if (mdioCfg->mdioBusFreqHz != 0U)
            {
                cppiClkFreqHz = EnetSoc_getClkFreq(enetType, instId, CPSW_CPPI_CLK);

                clkdiv = (cppiClkFreqHz / mdioCfg->mdioBusFreqHz) - 1U;
                if (clkdiv > 0xFFFFU)
                {
                    status = ENET_EFAIL;
                    ENETTRACE_ERR(status,
                                  "Unsupported clk div %u (CPPI %u Hz, MDIO bus %u Hz)",
                                  clkdiv, cppiClkFreqHz, mdioCfg->mdioBusFreqHz);
                }

                ipgRatio = mdioCfg->mdioBusFreqHz / mdioCfg->phyStatePollFreqHz;
                if (ipgRatio > 0xFFU)
                {
                    status = ENET_EFAIL;
                    ENETTRACE_ERR(status,
                                  "Unsupported IPG ratio %u (MDIO bus %u Hz, PHY poll %u Hz)",
                                  ipgRatio, mdioCfg->mdioBusFreqHz, mdioCfg->phyStatePollFreqHz);
                }
            }
            else
            {
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status, "Invalid MDIO bus freq %u Hz", mdioCfg->mdioBusFreqHz);
            }
        }

        /* Setup MDIO mode: normal, monitor or manual mode */
        if (status == ENET_SOK)
        {
            CSL_MDIO_clearUnmaskedLinkStatusChangeInt(mdioRegs, 0U);
            CSL_MDIO_clearUnmaskedLinkStatusChangeInt(mdioRegs, 1U);

            switch (mdioCfg->mode)
            {
                case MDIO_MODE_NORMAL:
                    status = Mdio_setupNormalMode(mdioRegs, mdioCfg);
                    break;

                case MDIO_MODE_STATE_CHANGE_MON:
                    Mdio_setupStatusChangeMode(mdioRegs, mdioCfg);
                    break;

                case MDIO_MODE_MANUAL:
                    if (mdioCfg->c45EnMask != ENET_MDIO_PHY_ADDR_MASK_NONE)
                    {
                        status = ENET_ENOTSUPPORTED;
                        ENETTRACE_WARN("MDIO Clause 45 is not supported");
                    }
                    else
                    {
                        ENETTRACE_INFO("MDIO manual mode enabled");
                        hMdio->mdc_halfPeriodInNsec = (SEC_TO_NANOSEC / mdioCfg->mdioBusFreqHz);
                        hMdio->mdc_halfPeriodInNsec /= 2;
                        Mdio_setupManualMode(mdioRegs);
                    }
                    break;
            }
        }

        /* Write MDIO configuration */
        if ((status == ENET_SOK) &&
            (mdioCfg->mode != MDIO_MODE_MANUAL))
        {
            CSL_MDIO_setPollIPG(mdioRegs, (uint8_t)ipgRatio);
            CSL_MDIO_setClkDivVal(mdioRegs, (uint16_t)clkdiv);
            CSL_MDIO_enableStateMachine(mdioRegs);
#if ENET_CFG_IS_ON(MDIO_CLAUSE45)
            if (ENET_FEAT_IS_EN(hMod->features, MDIO_FEATURE_CLAUSE45))
            {
                CSL_MDIO_setClause45EnableMask(mdioRegs, mdioCfg->c45EnMask);
            }
#endif
        }
    }
    return status;
}

int32_t Mdio_rejoin(EnetMod_Handle hMod,
                    Enet_Type enetType,
                    uint32_t instId)
{
    return ENET_SOK;
}

void Mdio_close(EnetMod_Handle hMod)
{
    Mdio_Handle hMdio = (Mdio_Handle)hMod;
    CSL_mdioHandle mdioRegs = (CSL_mdioHandle)hMod->virtAddr;
    uint32_t i;

    Enet_devAssert(mdioRegs != NULL, "MDIO reg address is not valid");

    if (hMdio->isMaster)
    {
        for (i = 0U; i < MDIO_PHY_MONITOR_MAX; i++)
        {
            CSL_MDIO_disableLinkStatusChangeInterrupt(mdioRegs, i, 0U);
        }

        CSL_MDIO_disableStatusChangeModeInterrupt(mdioRegs);
        CSL_MDIO_disableStateMachine(mdioRegs);
        CSL_MDIO_clearUnmaskedLinkStatusChangeInt(mdioRegs, 0U);
        CSL_MDIO_clearUnmaskedLinkStatusChangeInt(mdioRegs, 1U);
    }
}

void Mdio_saveCtxt(EnetMod_Handle hMod)
{
    Mdio_Handle hMdio = (Mdio_Handle)hMod;
    CSL_mdioHandle regs = (CSL_mdioHandle)hMod->virtAddr;
    uint32_t i;

    hMdio->ctxt.control       =  CSL_REG32_RD(&regs->CONTROL_REG);
    hMdio->ctxt.linkIntMasked =  CSL_REG32_RD(&regs->LINK_INT_MASKED_REG);
    hMdio->ctxt.poll          =  CSL_REG32_RD(&regs->POLL_REG);
    hMdio->ctxt.pollEn        =  CSL_REG32_RD(&regs->POLL_EN_REG);
    hMdio->ctxt.claus45       =  CSL_REG32_RD(&regs->CLAUS45_REG);

    for (i = 0U; i < MDIO_USER_GROUP_MAX; i++)
    {
        hMdio->ctxt.userPhySel[i] =  CSL_REG32_RD(&regs->USER_GROUP[i].USER_PHY_SEL_REG);
    }

    /* Disable the MDIO interupts */
    CSL_REG32_WR(&regs->LINK_INT_MASK_CLEAR_REG, 1U);
}

int32_t Mdio_restoreCtxt(EnetMod_Handle hMod)
{
    Mdio_Handle hMdio = (Mdio_Handle)hMod;
    CSL_mdioHandle regs = (CSL_mdioHandle)hMod->virtAddr;
    uint32_t i;

    for (i = 0U; i < MDIO_USER_GROUP_MAX; i++)
    {
        CSL_REG32_WR(&regs->USER_GROUP[i].USER_PHY_SEL_REG,  hMdio->ctxt.userPhySel[i]);
    }

    CSL_REG32_WR(&regs->POLL_EN_REG, hMdio->ctxt.pollEn);
    CSL_REG32_WR(&regs->POLL_REG, hMdio->ctxt.poll);
    CSL_REG32_WR(&regs->CONTROL_REG, hMdio->ctxt.control);
    CSL_REG32_WR(&regs->CLAUS45_REG, hMdio->ctxt.claus45);
    CSL_REG32_WR(&regs->LINK_INT_MASKED_REG, hMdio->ctxt.linkIntMasked);

    return ENET_SOK;
}

int32_t Mdio_ioctlManualMode(EnetMod_Handle hMod,
                             uint32_t cmd,
                             Enet_IoctlPrms *prms)
{
    CSL_mdioHandle mdioRegs = (CSL_mdioHandle)hMod->virtAddr;
    int32_t status = ENET_SOK;

#if ENET_CFG_IS_ON(DEV_ERROR)
    /* Validate MDIO IOCTL parameters */
    if (ENET_IOCTL_GET_PER(cmd) == ENET_IOCTL_PER_CPSW)
    {
        if (ENET_IOCTL_GET_TYPE(cmd) == ENET_IOCTL_TYPE_PRIVATE)
        {
            status = Enet_validateIoctl(cmd, prms,
                                        gMdio_privIoctlValidate,
                                        ENET_ARRAYSIZE(gMdio_privIoctlValidate));

            ENETTRACE_ERR_IF(status != ENET_SOK, status, "IOCTL 0x%08x params are not valid", cmd);
        }
    }
#endif

    if (status == ENET_SOK)
    {
        Enet_devAssert(mdioRegs != NULL, "MDIO reg address is not valid");

        switch (cmd)
        {
            case ENET_MDIO_IOCTL_GET_VERSION:
            {
                Enet_Version *version = (Enet_Version *)prms->outArgs;
                CSL_MDIO_VERSION ver;

                CSL_MDIO_getVersionInfo(mdioRegs, &ver);
                version->maj    = ver.revMaj;
                version->min    = ver.revMin;
                version->rtl    = ver.revRtl;
                version->id     = ver.modId;
                version->other1 = ver.scheme;
                version->other2 = ver.bu;
            }
            break;

            case ENET_MDIO_IOCTL_PRINT_REGS:
            {
#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
                Mdio_printRegs(mdioRegs);
#endif
            }
            break;

            case ENET_MDIO_IOCTL_IS_ALIVE:
            {
                uint8_t *phyAddr = (uint8_t *)prms->inArgs;
                bool *alive = (bool *)prms->outArgs;
                uint16_t bmsrVal;

                *alive = (Mdio_manualModePhyRegRead22(hMod, *phyAddr, PHY_BMSR, &bmsrVal) == CSL_PASS);
            }
            break;

            case ENET_MDIO_IOCTL_IS_LINKED:
            {
                uint8_t *phyAddr = (uint8_t *)prms->inArgs;
                bool *linked = (bool *)prms->outArgs;
                uint16_t bmsrVal;

                *linked = BFALSE;
                if (Mdio_manualModePhyRegRead22(hMod, *phyAddr, PHY_BMSR, &bmsrVal) == CSL_PASS)
                {
                    *linked = ((bmsrVal & LINK_STATUS_BITMASK) != 0U);
                }
            }
            break;

            case ENET_MDIO_IOCTL_IS_POLL_ENABLED:
            {
                bool *pIsEnabled = (bool *)prms->outArgs;

                *pIsEnabled = BFALSE;
            }
            break;

            case ENET_MDIO_IOCTL_C22_READ:
            {
                EnetMdio_C22ReadInArgs *inArgs = (EnetMdio_C22ReadInArgs *)prms->inArgs;
                uint16_t *val = (uint16_t *)prms->outArgs;
                uint32_t ack= Mdio_manualModePhyRegRead22(hMod,
                                                          inArgs->phyAddr,
                                                          inArgs->reg,
                                                          val);

                status = (ack == CSL_PASS) ? ENET_SOK : ENET_EFAIL;
                ENETTRACE_ERR_IF(status != ENET_SOK, status,
                                 "failed to read PHY %u C22 reg %u",
                                 inArgs->phyAddr, inArgs->reg);
            }
            break;

            case ENET_MDIO_IOCTL_C22_WRITE:
            {
                EnetMdio_C22WriteInArgs *inArgs = (EnetMdio_C22WriteInArgs *)prms->inArgs;

                Mdio_manualModePhyRegWrite22(hMod,
                                             inArgs->phyAddr,
                                             inArgs->reg,
                                             inArgs->val);
            }
            break;

            case ENET_MDIO_IOCTL_C45_READ:
            {
                status = ENET_ENOTSUPPORTED;
                ENETTRACE_ERR(status, "C45 support is not enabled");
            }
            break;

            case ENET_MDIO_IOCTL_C45_WRITE:
            {
                status = ENET_ENOTSUPPORTED;
                ENETTRACE_ERR(status, "C45 support is not enabled");
            }
            break;

            case ENET_MDIO_IOCTL_DISABLE_INTR:
            {
                /* Disable the MDIO interupts */
                CSL_REG32_WR(&mdioRegs->LINK_INT_MASK_CLEAR_REG, 1U);
            }
            break;

            default:
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status, "Invalid IOCTL cmd 0x%08x", cmd);
                break;
        }
    }

    return status;
}

int32_t Mdio_ioctl(EnetMod_Handle hMod,
                   uint32_t cmd,
                   Enet_IoctlPrms *prms)
{
    CSL_mdioHandle mdioRegs = (CSL_mdioHandle)hMod->virtAddr;
    int32_t status = ENET_SOK;

#if ENET_CFG_IS_ON(DEV_ERROR)
    /* Validate MDIO IOCTL parameters */
    if (ENET_IOCTL_GET_PER(cmd) == ENET_IOCTL_PER_CPSW)
    {
        if (ENET_IOCTL_GET_TYPE(cmd) == ENET_IOCTL_TYPE_PRIVATE)
        {
            status = Enet_validateIoctl(cmd, prms,
                                        gMdio_privIoctlValidate,
                                        ENET_ARRAYSIZE(gMdio_privIoctlValidate));

            ENETTRACE_ERR_IF(status != ENET_SOK, status, "IOCTL 0x%08x params are not valid", cmd);
        }
    }
#endif

    if (status == ENET_SOK)
    {
        Enet_devAssert(mdioRegs != NULL, "MDIO reg address is not valid");

        switch (cmd)
        {
            case ENET_MDIO_IOCTL_GET_VERSION:
            {
                Enet_Version *version = (Enet_Version *)prms->outArgs;
                CSL_MDIO_VERSION ver;

                CSL_MDIO_getVersionInfo(mdioRegs, &ver);
                version->maj    = ver.revMaj;
                version->min    = ver.revMin;
                version->rtl    = ver.revRtl;
                version->id     = ver.modId;
                version->other1 = ver.scheme;
                version->other2 = ver.bu;
            }
            break;

            case ENET_MDIO_IOCTL_PRINT_REGS:
            {
#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
                Mdio_printRegs(mdioRegs);
#endif
            }
            break;

            case ENET_MDIO_IOCTL_IS_ALIVE:
            {
                uint8_t *phyAddr = (uint8_t *)prms->inArgs;
                bool *alive = (bool *)prms->outArgs;

                *alive = (CSL_MDIO_isPhyAlive(mdioRegs, *phyAddr) == UFALSE) ? BFALSE : BTRUE;
            }
            break;

            case ENET_MDIO_IOCTL_IS_LINKED:
            {
                uint8_t *phyAddr = (uint8_t *)prms->inArgs;
                bool *linked = (bool *)prms->outArgs;

                *linked = (CSL_MDIO_isPhyLinked(mdioRegs, *phyAddr) == UFALSE) ? BFALSE : BTRUE;
            }
            break;

            case ENET_MDIO_IOCTL_IS_POLL_ENABLED:
            {
                uint8_t *phyAddr = (uint8_t *)prms->inArgs;
                bool *enabled = (bool *)prms->outArgs;
                uint8_t monPhyAddr;
                uint32_t isStatusChangeMode;
                uint32_t pollMask = 0U;
                uint32_t i;

                isStatusChangeMode = CSL_MDIO_isStateChangeModeEnabled(mdioRegs);
                if (isStatusChangeMode == 1U)
                {
                    pollMask = CSL_MDIO_getPollEnableMask(mdioRegs);
                }
                else
                {
                    for (i = 0U; i < MDIO_PHY_MONITOR_MAX; i++)
                    {
                        monPhyAddr = CSL_MDIO_getLinkStatusChangePhyAddr(mdioRegs, i);
                        pollMask |= ENET_MDIO_PHY_ADDR_MASK(monPhyAddr);
                    }
                }

                *enabled = ENET_IS_BIT_SET(pollMask, *phyAddr);
            }
            break;

            case ENET_MDIO_IOCTL_C22_READ:
            {
                EnetMdio_C22ReadInArgs *inArgs = (EnetMdio_C22ReadInArgs *)prms->inArgs;
                uint16_t *val = (uint16_t *)prms->outArgs;
#if ENET_CFG_IS_ON(MDIO_CLAUSE45)
                uint32_t c45EnMask;

                if (ENET_FEAT_IS_EN(hMod->features, MDIO_FEATURE_CLAUSE45))
                {
                    c45EnMask = CSL_MDIO_getClause45EnableMask(mdioRegs);
                    if (ENET_IS_BIT_SET(c45EnMask, inArgs->phyAddr))
                    {
                        status = ENET_EPERM;
                        ENETTRACE_ERR(status, "PHY %u is not configured for C22 access", inArgs->phyAddr);
                    }
                }
#endif

                if (status == ENET_SOK)
                {
                    status = Mdio_PhyRegReadC22(hMod,
                                                inArgs->group,
                                                inArgs->phyAddr,
                                                inArgs->reg,
                                                val);
                    ENETTRACE_ERR_IF(status != ENET_SOK, status,
                                     "failed to read PHY %u C22 reg %u",
                                     inArgs->phyAddr, inArgs->reg);
                }
            }
            break;

            case ENET_MDIO_IOCTL_C22_WRITE:
            {
                EnetMdio_C22WriteInArgs *inArgs = (EnetMdio_C22WriteInArgs *)prms->inArgs;
#if ENET_CFG_IS_ON(MDIO_CLAUSE45)
                uint32_t c45EnMask;

                if (ENET_FEAT_IS_EN(hMod->features, MDIO_FEATURE_CLAUSE45))
                {
                    c45EnMask = CSL_MDIO_getClause45EnableMask(mdioRegs);
                    if (ENET_IS_BIT_SET(c45EnMask, inArgs->phyAddr))
                    {
                        status = ENET_EPERM;
                        ENETTRACE_ERR(status, "PHY %u is not configured for C22 access", inArgs->phyAddr);
                    }
                }
#endif

                if (status == ENET_SOK)
                {
                    status = Mdio_PhyRegWriteC22(hMod,
                                                 inArgs->group,
                                                 inArgs->phyAddr,
                                                 inArgs->reg,
                                                 inArgs->val);
                    ENETTRACE_ERR_IF(status != ENET_SOK, status,
                                     "failed to write PHY %u C22 reg %u",
                                     inArgs->phyAddr, inArgs->reg);
                }
            }
            break;

            case ENET_MDIO_IOCTL_C45_READ:
            {
#if ENET_CFG_IS_ON(MDIO_CLAUSE45)
                EnetMdio_C45ReadInArgs *inArgs = (EnetMdio_C45ReadInArgs *)prms->inArgs;
                uint16_t *val = (uint16_t *)prms->outArgs;

                if (ENET_FEAT_IS_EN(hMod->features, MDIO_FEATURE_CLAUSE45))
                {
                    status = Mdio_readRegC45(hMod,
                                             inArgs->group,
                                             inArgs->mmd,
                                             inArgs->phyAddr,
                                             inArgs->reg,
                                             val);
                    ENETTRACE_ERR_IF(status != ENET_SOK, status,
                                     "failed to read PHY %u C45 MMD %u reg %u",
                                     inArgs->phyAddr, inArgs->mmd, inArgs->reg);
                }
                else
                {
                    status = ENET_ENOTSUPPORTED;
                    ENETTRACE_ERR(status, "C45 support is not supported");
                }
#else
                status = ENET_ENOTSUPPORTED;
                ENETTRACE_ERR(status, "C45 support is not enabled");
#endif
            }
            break;

            case ENET_MDIO_IOCTL_C45_WRITE:
            {
#if ENET_CFG_IS_ON(MDIO_CLAUSE45)
                EnetMdio_C45WriteInArgs *inArgs = (EnetMdio_C45WriteInArgs *)prms->inArgs;

                if (ENET_FEAT_IS_EN(hMod->features, MDIO_FEATURE_CLAUSE45))
                {
                    status = Mdio_writeRegC45(hMod,
                                              inArgs->group,
                                              inArgs->mmd,
                                              inArgs->phyAddr,
                                              inArgs->reg,
                                              inArgs->val);
                    ENETTRACE_ERR_IF(status != ENET_SOK, status,
                                     "failed to write PHY %u C45 MMD %u reg %u",
                                     inArgs->phyAddr, inArgs->mmd, inArgs->reg);
                }
                else
                {
                    status = ENET_ENOTSUPPORTED;
                    ENETTRACE_ERR(status, "C45 support is not supported");
                }
#else
                status = ENET_ENOTSUPPORTED;
                ENETTRACE_ERR(status, "C45 support is not enabled");
#endif
            }
            break;

            case ENET_MDIO_IOCTL_DISABLE_INTR:
            {
                /* Disable the MDIO interupts */
                CSL_REG32_WR(&mdioRegs->LINK_INT_MASK_CLEAR_REG, 1U);
            }
            break;

            case MDIO_IOCTL_HANDLE_INTR:
            {
                Mdio_Callbacks *callbacks = (Mdio_Callbacks *)prms->inArgs;

                Mdio_handleIntr(mdioRegs, callbacks);
            }
            break;

            default:
                status = ENET_EINVALIDPARAMS;
                break;
        }
    }

    return status;
}

#if ENET_CFG_IS_ON(DEV_ERROR)
static int32_t Mdio_isSupported(CSL_mdioHandle mdioRegs)
{
    CSL_MDIO_VERSION version;
    uint32_t i;
    int32_t status = ENET_ENOTSUPPORTED;

    CSL_MDIO_getVersionInfo(mdioRegs, &version);

    for (i = 0U; i < ENET_ARRAYSIZE(gMdio_supportedVer); i++)
    {
        if ((version.revMaj == gMdio_supportedVer[i].revMaj) &&
            (version.revMin == gMdio_supportedVer[i].revMin) &&
            (version.revRtl == gMdio_supportedVer[i].revRtl) &&
            (version.modId  == gMdio_supportedVer[i].modId))
        {
            status = ENET_SOK;
            break;
        }
    }

    return status;
}
#endif

static int32_t Mdio_setupNormalMode(CSL_mdioHandle mdioRegs,
                                    const Mdio_Cfg *cfg)
{
    uint32_t phyMonIndex = 0U;
    uint32_t i;
    int32_t status = ENET_SOK;

    for (i = 0U; i <= MDIO_MAX_PHY_CNT; i++)
    {
        if (ENET_IS_BIT_SET(cfg->pollEnMask, i))
        {
            if (phyMonIndex < MDIO_PHY_MONITOR_MAX)
            {
                CSL_MDIO_enableLinkStatusChangeInterrupt(mdioRegs, phyMonIndex, i);
            }

            phyMonIndex++;
        }
    }

    if (phyMonIndex <= MDIO_PHY_MONITOR_MAX)
    {
        /* Normal Mode implies State Change Mode is disabled */
        CSL_MDIO_disableStateChangeMode(mdioRegs);
    }
    else
    {
        ENETTRACE_ERR(status, "Invalid PHY monitor count %d", phyMonIndex);
        for (i = 0U; i < MDIO_PHY_MONITOR_MAX; i++)
        {
            CSL_MDIO_disableLinkStatusChangeInterrupt(mdioRegs, i, 0U);
        }

        CSL_MDIO_clearUnmaskedLinkStatusChangeInt(mdioRegs, 0U);
        CSL_MDIO_clearUnmaskedLinkStatusChangeInt(mdioRegs, 1U);
        status = ENET_EINVALIDPARAMS;
    }

    return status;
}

static void Mdio_setupStatusChangeMode(CSL_mdioHandle mdioRegs,
                                       const Mdio_Cfg *cfg)
{
    CSL_MDIO_setPollEnableMask(mdioRegs, cfg->pollEnMask);
    CSL_MDIO_enableStatusChangeModeInterrupt(mdioRegs);
    CSL_MDIO_enableStateChangeMode(mdioRegs);
}

static void Mdio_setupManualMode(CSL_mdioHandle mdioRegs)
{
    CSL_MDIO_clearPollEnableMask(mdioRegs);
    CSL_MDIO_enableManualMode(mdioRegs);
}

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
static void Mdio_printRegs(CSL_mdioHandle mdioRegs)
{
    uint32_t *regAddr = (uint32_t *)mdioRegs;
    uint32_t regIdx = 0U;

    while ((uintptr_t)regAddr < ((uintptr_t)mdioRegs + sizeof(*mdioRegs)))
    {
        if (*regAddr != 0U)
        {
            ENETTRACE_INFO("MDIO: %u: 0x%08x", regIdx, *regAddr);
        }

        regAddr++;
        regIdx++;
    }
}
#endif

static int32_t Mdio_PhyRegReadC22(EnetMod_Handle hMod,
                                  uint32_t userGroup,
                                  uint32_t phyAddr,
                                  uint32_t regAddr,
                                  uint16_t *outVal)
{
    CSL_mdioHandle mdioRegs = (CSL_mdioHandle)hMod->virtAddr;
    Mdio_Handle hMdio = (Mdio_Handle)hMod;
    uint32_t timeoutCount = 0U;
    int32_t status = ENET_SOK;
    int32_t ack = 0U;

    /* Check if previous access succeeded */
    while (timeoutCount < hMdio->numRetries)
    {
        if (CSL_MDIO_isPhyRegAccessComplete(mdioRegs, userGroup) == TRUE)
        {
            break;
        } 

        timeoutCount++;
    }

    /* Initiate new C22 read access */
    if (timeoutCount < hMdio->numRetries)
    {
        ack = CSL_MDIO_phyInitiateRegReadC22(mdioRegs,
                                             userGroup,
                                             phyAddr,
                                             regAddr);
    }

    while ((timeoutCount < hMdio->numRetries) &&
           (ack == CSL_PASS))
    {
        if (CSL_MDIO_isPhyRegAccessComplete(mdioRegs, userGroup) == TRUE)
        {
            break;
        } 

        timeoutCount++;
    }

    if (timeoutCount < hMdio->numRetries)
    {
        ack = CSL_MDIO_phyGetRegReadVal(mdioRegs, userGroup, outVal);
    }

    if (timeoutCount >= hMdio->numRetries)
    {
        status = ENETPHY_EUNEXPECTED;
    }
    else
    {
        status = ack;
    }

    return status;
}

static int32_t Mdio_PhyRegWriteC22(EnetMod_Handle hMod,
                                   uint32_t userGroup,
                                   uint32_t phyAddr,
                                   uint32_t regAddr,
                                   uint16_t val)
{
    CSL_mdioHandle mdioRegs = (CSL_mdioHandle)hMod->virtAddr;
    Mdio_Handle hMdio = (Mdio_Handle)hMod;
    uint32_t timeoutCount = 0U;
    int32_t status = ENET_SOK;
    int32_t ack = 0U;

    /* Check if previous access succeeded */
    while (timeoutCount < hMdio->numRetries)
    {
        if (CSL_MDIO_isPhyRegAccessComplete(mdioRegs, userGroup) == TRUE)
        {
            break;
        } 

        timeoutCount++;
    }

    /* Initiate new C22 write access */
    if (timeoutCount < hMdio->numRetries)
    {
        ack = CSL_MDIO_phyRegInitiateWriteC22(mdioRegs,
                                              userGroup,
                                              phyAddr,
                                              regAddr,
                                              val);
    }

    while ((timeoutCount < hMdio->numRetries) &&
           (ack == CSL_PASS))
    {
        if (CSL_MDIO_isPhyRegAccessComplete(mdioRegs, userGroup) == TRUE)
        {
            break;
        } 

        timeoutCount++;
    }

    if (timeoutCount >= hMdio->numRetries)
    {
        status = ENETPHY_EUNEXPECTED;
    }
    else
    {
        status = ack;
    }

    return status;
}

static uint32_t  Mdio_manualModePhyRegRead22(EnetMod_Handle hMod,
                                        uint32_t phyAddr,
                                        uint32_t regNum,
                                        uint16_t *pData)
{
    uint32_t i, sts;
    uint16_t tmp;
    char ack;

    Mdio_Handle hMdio = (Mdio_Handle)hMod;
    CSL_mdioHandle hMdioRegs = (CSL_mdioHandle)hMod->virtAddr;
    CSL_MDIO_setMdclkLow(hMdioRegs); /* Disable Phy Interrupt driver */

    CSL_MDIO_setMdoOutputEnable(hMdioRegs); /* Enable our drive capability */

    Mdio_manualModeFieldSend(hMod, 0x80000000, 0xffffffff); /* 32-bit preamble */
    Mdio_manualModeFieldSend(hMod, 0x8, 0x6); /* Issue clause 22 MII read function {0,1,1,0}*/
    Mdio_manualModeFieldSend(hMod, 0x10, phyAddr); /* Send the device number MSB first */
    Mdio_manualModeFieldSend(hMod, 0x10, regNum); /* Send the register number MSB first */

    CSL_MDIO_setMdoInputEnable(hMdioRegs); /* send Turn-arround cycles */
    Mdio_manualModeToggleMdclk(hMdioRegs, hMdio->mdc_halfPeriodInNsec);

    ack = CSL_MDIO_readMdi(hMdioRegs); /* Get PHY Ack */
    Mdio_manualModeToggleMdclk(hMdioRegs, hMdio->mdc_halfPeriodInNsec);

    if (ack == 0) /* If Acked read the data */
    {
        for (tmp = 0, i = 0x8000; i; i >>= 1)
        {
            if (CSL_MDIO_readMdi(hMdioRegs))
            {
                tmp |= i;
            }
            Mdio_manualModeToggleMdclk(hMdioRegs, hMdio->mdc_halfPeriodInNsec);
        }
        sts = CSL_PASS;
    }
    else
    {
        for (tmp = 0, i = 0x8000; i; i >>= 1)
        {
            Mdio_manualModeToggleMdclk(hMdioRegs, hMdio->mdc_halfPeriodInNsec);
        }
        tmp = 0xffff;
        sts = CSL_ETIMEOUT;
    }

    CSL_MDIO_setMdclkLow(hMdioRegs); /* Give time for pull-up to work */
    CSL_MDIO_setMdclkLow(hMdioRegs);
    CSL_MDIO_setMdclkLow(hMdioRegs);
    Mdio_manualModeToggleMdclk(hMdioRegs, hMdio->mdc_halfPeriodInNsec); /* re-enable PHY Interrupt function */
    *pData = tmp;
    return (sts);
}

static void Mdio_manualModePhyRegWrite22(EnetMod_Handle hMod,
                                  uint32_t phyAddr,
                                  uint32_t regNum,
                                  uint16_t wrVal)
{
    Mdio_Handle hMdio = (Mdio_Handle)hMod;
    CSL_mdioHandle hMdioRegs = (CSL_mdioHandle)hMod->virtAddr;

    CSL_MDIO_setMdclkLow(hMdioRegs);          /* Disable Phy Interrupt driver */
    CSL_MDIO_setMdoOutputEnable(hMdioRegs);          /* Enable our drive capability */

    Mdio_manualModeFieldSend(hMod, 0x80000000, 0xffffffff); /* 32-bit preamble */
    Mdio_manualModeFieldSend(hMod, 0x8,0x5);     /* Issue clause 22 MII write function {0,1,0,1}*/
    Mdio_manualModeFieldSend(hMod, 0x10,phyAddr);    /* Send the device number MSB first */
    Mdio_manualModeFieldSend(hMod, 0x10,regNum);    /* Send the register number MSB first */

    CSL_MDIO_setMdoHigh(hMdioRegs);          /* send Turn-arround cycles */
    Mdio_manualModeToggleMdclk(hMdioRegs, hMdio->mdc_halfPeriodInNsec);
    CSL_MDIO_setMdoLow(hMdioRegs);
    Mdio_manualModeToggleMdclk(hMdioRegs, hMdio->mdc_halfPeriodInNsec);
    Mdio_manualModeFieldSend(hMod, 0x08000, wrVal); /* Send Register data MSB first */
    CSL_MDIO_setMdoInputEnable(hMdioRegs);

    CSL_MDIO_setMdclkLow(hMdioRegs);          /* Give time for pull-up to work */
    CSL_MDIO_setMdclkLow(hMdioRegs);
    CSL_MDIO_setMdclkLow(hMdioRegs);
    Mdio_manualModeToggleMdclk(hMdioRegs, hMdio->mdc_halfPeriodInNsec);       /* re-enable PHY Interrupt function */
}

static void Mdio_manualModeToggleMdclk(CSL_mdioHandle hMdioRegs, uint32_t delayInNs)
{
    CSL_MDIO_setMdclkLow(hMdioRegs);
    EnetUtils_delay(delayInNs);
    CSL_MDIO_setMdclkHigh(hMdioRegs);
    EnetUtils_delay(delayInNs);
}

static void Mdio_manualModeFieldSend(EnetMod_Handle hMod,
                                    uint32_t iMsb,
                                    uint32_t iVal)
{
    Mdio_Handle hMdio = (Mdio_Handle)hMod;
    CSL_mdioHandle hMdioRegs = (CSL_mdioHandle)hMod->virtAddr;

    for (uint32_t i = iMsb; i; i >>= 1U)
    {
        if (i & iVal)
        {
            CSL_MDIO_setMdoHigh(hMdioRegs);
        }
        else
        {
            CSL_MDIO_setMdoLow(hMdioRegs);
        }

        EnetUtils_delay(hMdio->mdc_halfPeriodInNsec);
        Mdio_manualModeToggleMdclk(hMdioRegs, hMdio->mdc_halfPeriodInNsec);
    }
}

#if ENET_CFG_IS_ON(MDIO_CLAUSE45)
static int32_t Mdio_readRegC45(EnetMod_Handle hMod,
                               uint32_t userGroup,
                               uint32_t mmdNum,
                               uint32_t phyAddr,
                               uint32_t regAddr,
                               uint16_t *outVal)
{
    CSL_mdioHandle mdioRegs = (CSL_mdioHandle)hMod->virtAddr;
    Mdio_Handle hMdio = (Mdio_Handle)hMod;
    uint32_t timeoutCount = 0U;
    int32_t status = ENET_SOK;
    uint32_t ack = ENET_EFAIL;
    uint32_t c45EnMask;

    c45EnMask = CSL_MDIO_getClause45EnableMask(mdioRegs);
    if (!ENET_IS_BIT_SET(c45EnMask, phyAddr))
    {
        status = ENET_EPERM;
        ENETTRACE_ERR(status, "PHY %u is not configured for C45 access\n", phyAddr);
    }

    if (status == ENET_SOK)
    {
            while (timeoutCount < hMdio->numRetries)
            {
                if (CSL_MDIO_isPhyRegAccessComplete(mdioRegs, userGroup) == TRUE)
                {
                    break;
                } 

                timeoutCount++;
            }

            if (timeoutCount < hMdio->numRetries)
            {
                ack = CSL_MDIO_phyInitiateRegReadC45(mdioRegs,
                                                     userGroup,
                                                     phyAddr,
                                                     mmdNum,
                                                     regAddr);
            }

            while ((timeoutCount < hMdio->numRetries) &&
                   (ack == CSL_PASS))
            {
                if (CSL_MDIO_isPhyRegAccessComplete(mdioRegs, userGroup) == TRUE)
                {
                    break;
                } 

                timeoutCount++;
            }

            if (timeoutCount < hMdio->numRetries)
            {
                ack = CSL_MDIO_phyGetRegReadVal(mdioRegs,
                                                userGroup,
                                                outVal);
            }
    }

    if (timeoutCount >= hMdio->numRetries)
    {
        status = ENETPHY_EUNEXPECTED;
    }
    else if (status == ENET_SOK)
    {
        status = ack;
    }

    return status;
}

static int32_t Mdio_writeRegC45(EnetMod_Handle hMod,
                                uint32_t userGroup,
                                uint32_t mmdNum,
                                uint32_t phyAddr,
                                uint32_t regAddr,
                                uint16_t val)
{
    CSL_mdioHandle mdioRegs = (CSL_mdioHandle)hMod->virtAddr;
    Mdio_Handle hMdio = (Mdio_Handle)hMod;
    uint32_t timeoutCount = 0U;
    int32_t status = ENET_SOK;
    uint32_t ack = ENET_EFAIL;
    uint32_t c45EnMask;

    c45EnMask = CSL_MDIO_getClause45EnableMask(mdioRegs);
    if (!ENET_IS_BIT_SET(c45EnMask, phyAddr))
    {
        status = ENET_EPERM;
        ENETTRACE_ERR(status, "PHY %u is not configured for C45 access\n", phyAddr);
    }

    if (status == ENET_SOK)
    {
            while (timeoutCount < hMdio->numRetries)
            {
                if (CSL_MDIO_isPhyRegAccessComplete(mdioRegs, userGroup) == TRUE)
                {
                    break;
                } 

                timeoutCount++;
            }

            if (timeoutCount < hMdio->numRetries)
            {
                ack = CSL_MDIO_phyInitiateRegWriteC45(mdioRegs,
                                                      userGroup,
                                                      phyAddr,
                                                      mmdNum,
                                                      regAddr,
                                                      val);
            }

            while ((timeoutCount < hMdio->numRetries) &&
                   (ack == CSL_PASS))
            {
                if (CSL_MDIO_isPhyRegAccessComplete(mdioRegs, userGroup) == TRUE)
                {
                    break;
                } 

                timeoutCount++;
            }
    }

    if (timeoutCount >= hMdio->numRetries)
    {
        status = ENETPHY_EUNEXPECTED;
    }
    else if (status == ENET_SOK)
    {
        status = ack;
    }

    return status;
}
#endif /* MDIO_CLAUSE45 */

static void Mdio_handleIntr(CSL_mdioHandle mdioRegs,
                            Mdio_Callbacks *callbacks)
{
    Mdio_PhyStatus phyStatus;
    uint32_t phyAddr;
    uint32_t isStatusChangeMode;
    uint32_t pollEnMask = ENET_MDIO_PHY_ADDR_MASK_NONE;
    uint32_t linkChanged;
    uint32_t i;
    bool linkInt0Changed = BFALSE;
    bool linkInt1Changed = BFALSE;

    linkChanged = CSL_MDIO_isUnmaskedLinkStatusChangeIntSet(mdioRegs, 0U);
    if (linkChanged == 1U)
    {
        CSL_MDIO_clearUnmaskedLinkStatusChangeInt(mdioRegs, 0U);
        linkInt0Changed = BTRUE;
    }

    linkChanged = CSL_MDIO_isUnmaskedLinkStatusChangeIntSet(mdioRegs, 1U);
    if (linkChanged == 1U)
    {
        CSL_MDIO_clearUnmaskedLinkStatusChangeInt(mdioRegs, 1U);
        linkInt1Changed = BTRUE;
    }

    /* Only report state change of PHYs being polled */
    isStatusChangeMode = CSL_MDIO_isStateChangeModeEnabled(mdioRegs);
    if (isStatusChangeMode == 0)
    {
        for (i = 0U; i < MDIO_PHY_MONITOR_MAX; i++)
        {
            phyAddr = CSL_MDIO_getLinkStatusChangePhyAddr(mdioRegs, i);

            pollEnMask |= ENET_MDIO_PHY_ADDR_MASK(phyAddr);
        }

        phyStatus.aliveMask  = ENET_MDIO_PHY_ADDR_MASK_NONE;
        phyStatus.linkedMask = mdioRegs->LINK_REG & pollEnMask;
    }
    else
    {
        pollEnMask = CSL_MDIO_getPollEnableMask(mdioRegs);

        phyStatus.aliveMask  = mdioRegs->ALIVE_REG & pollEnMask;
        phyStatus.linkedMask = mdioRegs->LINK_REG & pollEnMask;
    }

    /* MDIO_LINKINT[0] event handling */
    if (linkInt0Changed)
    {
        if (callbacks->linkStateCb != NULL)
        {
            callbacks->linkStateCb(ENET_MDIO_GROUP_0, &phyStatus, callbacks->cbArgs);
        }
    }

    /* MDIO_LINKINT[1] event handling */
    if (linkInt1Changed)
    {
        if (isStatusChangeMode == 0U)
        {
            if (callbacks->linkStateCb != NULL)
            {
                callbacks->linkStateCb(ENET_MDIO_GROUP_1, &phyStatus, callbacks->cbArgs);
            }
        }
        else
        {
            /* MDIO_LINKINT[1] is unexpected in State Change Mode */
            /* TODO: Report an error message */
        }
    }
}
