/*
 *  Copyright (c) Texas Instruments Incorporated 2020-2025
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
 * \file  enetphy.c
 *
 * \brief This file contains the implementation of the Ethernet PHY driver.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x501

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ti/drv/enet/enet_cfg.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/priv/core/enet_trace_priv.h>
#include <ti/drv/enet/include/phy/enetphy.h>
#include "enetphy_priv.h"
#include "generic_phy.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*! \brief Maximum number of PHYs supported. */
#define ENETPHY_PHY_MAX                       (8U)

/*! \brief Capability string buffer length. */
#define ENETPHY_CAPS_BUF_LEN                  (41U)

/*! \brief PHY mode string buffer length. */
#define ENETPHY_MODE_BUF_LEN                  (25U)

/*! \brief Default FINDING state timeout (in ticks). */
#define ENETPHY_TIMEOUT_FINDING               (20U)

/*! \brief Default RESET_WAIT state timeout (in ticks). */
#define ENETPHY_TIMEOUT_RESET_WAIT            (10U)

/*! \brief Default RESET_WAIT state residence time (in ticks). */
#define ENETPHY_RESIDENCE_RESET_WAIT          (0U)

/*! \brief Default LINK_WAIT state timeout (in ticks). */
#define ENETPHY_TIMEOUT_LINK_WAIT             (50U)

/*! \brief Default NWAY_START state timeout (in ticks). */
#define ENETPHY_TIMEOUT_NWAY_START            (50U)

/*! \brief Default NWAY_WAIT state timeout (in ticks). */
#define ENETPHY_TIMEOUT_NWAY_WAIT             (80U)

/*! \brief Default timeout if MDIX is enabled (in ticks). */
#define ENETPHY_TIMEOUT_MDIX                  (27U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_WARN)
static const char *EnetPhy_getCapsString(uint32_t linkCaps);
#endif

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_DEBUG)
static const char *EnetPhy_getModeString(EnetPhy_Speed speed,
                                         EnetPhy_Duplexity duplexity);
#endif

static EnetPhy_Handle EnetPhy_getHandle(void);

static void EnetPhy_releaseHandle(EnetPhy_Handle hPhy);

static void EnetPhy_setNextState(EnetPhy_Handle hPhy,
                                 EnetPhy_FsmState fsmState);

static void EnetPhy_phyTimeout(EnetPhy_Handle hPhy);

static void EnetPhy_initState(EnetPhy_Handle hPhy);

static void EnetPhy_defaultState(EnetPhy_Handle hPhy);

static void EnetPhy_findingState(EnetPhy_Handle hPhy);

static void EnetPhy_findingState(EnetPhy_Handle hPhy);

static void EnetPhy_foundState(EnetPhy_Handle hPhy);

static void EnetPhy_resetWaitState(EnetPhy_Handle hPhy);

static void EnetPhy_enableState(EnetPhy_Handle hPhy);

static int32_t EnetPhy_setupNway(EnetPhy_Handle hPhy);

static int32_t EnetPhy_setupManual(EnetPhy_Handle hPhy,
                                   bool loopbackEn);

static void EnetPhy_loopbackState(EnetPhy_Handle hPhy);

static void EnetPhy_nwayStartState(EnetPhy_Handle hPhy);

static void EnetPhy_nwayWaitState(EnetPhy_Handle hPhy);

static void EnetPhy_linkWaitState(EnetPhy_Handle hPhy);

static void EnetPhy_linkedState(EnetPhy_Handle hPhy);

static int32_t EnetPhy_isNwayCapable(EnetPhy_Handle hPhy,
                                     bool *isNwayCapable);

static int32_t EnetPhy_getLocalCaps(EnetPhy_Handle hPhy,
                                    uint32_t *caps);

static int32_t EnetPhy_findCommonCaps(EnetPhy_Handle hPhy,
                                      uint32_t *commonCaps);

static int32_t EnetPhy_findCommon1000Caps(EnetPhy_Handle hPhy,
                                          uint32_t *commonCaps);

static int32_t EnetPhy_findCommonNwayCaps(EnetPhy_Handle hPhy,
                                          uint32_t *nwayCaps);

static int32_t EnetPhy_bindDriver(EnetPhy_Handle hPhy);

static void EnetPhy_resetPhy(EnetPhy_Handle hPhy);

static uint32_t EnetPhy_getManualCaps(EnetPhy_Speed speed,
                                      EnetPhy_Duplexity duplexity);

static uint32_t EnetPhy_findBestCap(uint32_t caps);

static void EnetPhy_capToMode(uint32_t caps,
                              EnetPhy_Speed *speed,
                              EnetPhy_Duplexity *duplexity);

static void EnetPhy_showLinkPartnerCompat(EnetPhy_Handle hPhy,
                                          EnetPhy_Speed speed,
                                          EnetPhy_Duplexity duplexity);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*! \brief Array which has driver handles for different PHYs. */
extern EnetPhyDrv_Handle gEnetPhyDrvs[];

/*! \brief Size of the gEnetPhyDrvs array. */
extern const uint32_t gEnetPhyDrvsSize;

/*! \brief Enet MAC port objects. */
static EnetPhy_Obj gEnetPhy_phyObjs[ENET_CFG_ENETPHY_PHY_MAX];

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_DEBUG)
/*! \brief Name of the FSM states. */
static const char *gEnetPhyStateNames[] =
{
    "INIT",
    "FINDING",
    "RESET_WAIT",
    "ENABLE",
    "FOUND",
    "NWAY_START",
    "NWAY_WAIT",
    "LINK_WAIT",
    "LINKED",
    "LOOPBACK",
};
#endif

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_WARN)
/*! \brief Ethernet PHY capabilities string buffer. */
static char gEnetPhyCapsBuf[ENETPHY_CAPS_BUF_LEN];
#endif

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_DEBUG)
/*! \brief Ethernet PHY mode string buffer. */
static char gEnetPhyModeBuf[ENETPHY_MODE_BUF_LEN];
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void EnetPhy_initCfg(EnetPhy_Cfg *phyCfg)
{
    memset(phyCfg, 0, sizeof(*phyCfg));

    phyCfg->phyAddr    = ENETPHY_INVALID_PHYADDR;
    phyCfg->nwayCaps   = ENETPHY_LINK_CAP_ALL;
    phyCfg->mdixEn     = BTRUE;
    phyCfg->isStrapped = BFALSE;
    phyCfg->loopbackEn = BFALSE;
    phyCfg->masterMode = BFALSE;
    phyCfg->extClkSource = BFALSE;
    phyCfg->skipExtendedCfg = BFALSE;
    phyCfg->timeoutCfg.findingStateTicks   = ENETPHY_TIMEOUT_FINDING;
    phyCfg->timeoutCfg.resetWaitStateTicks = ENETPHY_TIMEOUT_RESET_WAIT;
    phyCfg->timeoutCfg.resetWaitStateResidenceTicks = ENETPHY_RESIDENCE_RESET_WAIT;
    phyCfg->timeoutCfg.nwayStartStateTicks = ENETPHY_TIMEOUT_NWAY_START;
    phyCfg->timeoutCfg.nwayWaitStateTicks  = ENETPHY_TIMEOUT_NWAY_WAIT;
    phyCfg->timeoutCfg.linkWaitStateTicks  = ENETPHY_TIMEOUT_LINK_WAIT;
    phyCfg->timeoutCfg.mdixTicks           = ENETPHY_TIMEOUT_MDIX;
}

void EnetPhy_setExtendedCfg(EnetPhy_Cfg *phyCfg,
                            const void *extendedCfg,
                            uint32_t extendedCfgSize)
{
    Enet_devAssert(extendedCfgSize <= ENETPHY_EXTENDED_CFG_SIZE_MAX,
                   "Extended config size is too large: %u", extendedCfgSize);

    if ((extendedCfgSize > 0U) &&
        (extendedCfg != NULL))
    {
        memcpy(phyCfg->extendedCfg, extendedCfg, extendedCfgSize);
    }

    phyCfg->extendedCfgSize = extendedCfgSize;
}

EnetPhy_Handle EnetPhy_open(const EnetPhy_Cfg *phyCfg,
                            EnetPhy_Mii mii,
                            const EnetPhy_LinkCfg *linkCfg,
                            uint32_t macPortCaps,
                            EnetPhy_MdioHandle hMdio,
                            void *mdioArgs)
{
    EnetPhy_Handle hPhy = EnetPhy_getHandle();
    bool manualMode = BFALSE;
    bool isAlive;
    int32_t status = ENETPHY_SOK;

    if (hPhy == NULL)
    {
        status = ENETPHY_EALLOC;
        ENETTRACE_ERR(status, "PHY %u: Failed to allocate PHY object", phyCfg->phyAddr);
    }

    if (status == ENET_SOK)
    {
        hPhy->hDrv    = NULL;
        hPhy->hMdio   = hMdio;
        hPhy->mdioArgs = mdioArgs;
        hPhy->macCaps = macPortCaps;
        hPhy->phyCfg  = *phyCfg;
        hPhy->mii     = mii;
        hPhy->linkCfg = *linkCfg;
        hPhy->addr    = phyCfg->phyAddr;
        hPhy->group   = phyCfg->phyGroup;

        /* State's linkCaps will be a mask for NWAY path and single bit for manual settings */
        if ((linkCfg->speed == ENETPHY_SPEED_AUTO) ||
            (linkCfg->duplexity == ENETPHY_DUPLEX_AUTO))
        {
            hPhy->reqLinkCaps = phyCfg->nwayCaps;
            if (hPhy->reqLinkCaps == 0U)
            {
                hPhy->reqLinkCaps = ENETPHY_LINK_CAP_ALL;
            }
        }
        else
        {
            hPhy->reqLinkCaps = EnetPhy_getManualCaps(linkCfg->speed, linkCfg->duplexity);
            manualMode = BTRUE;
        }

        hPhy->state.loopbackEn = phyCfg->loopbackEn;

        /* Take a shortcut to FOUND state if PHY is already alive, will save some
         * hundreds of msecs.  Take a shorter shortcut to LINK_WAIT state if PHY
         * is strap configured */
        status = EnetPhy_isAlive(hPhy, &isAlive);
        if (isAlive)
        {
            EnetPhy_initState(hPhy);
            if (hPhy->phyCfg.isStrapped)
            {
                EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_LINK_WAIT);
            }
            else
            {
                EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_FOUND);
            }

            status = EnetPhy_bindDriver(hPhy);
        }
        else
        {
            EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_INIT);
        }

        ENETTRACE_DBG_IF(status == ENETPHY_SOK, "PHY %u: open", hPhy->addr);

        /* Show compatible link partner configuration when local PHY is in manual mode */
        if (manualMode && !phyCfg->loopbackEn)
        {
            EnetPhy_showLinkPartnerCompat(hPhy, linkCfg->speed, linkCfg->duplexity);
        }

        /* Release allocated object in case of any errors */
        if (status != ENET_SOK)
        {
            EnetPhy_releaseHandle(hPhy);
        }
    }

    return (status == ENETPHY_SOK) ? hPhy : NULL;
}

void EnetPhy_close(EnetPhy_Handle hPhy)
{
    /* Power down and isolate the PHY if not strapped */
    if (!hPhy->phyCfg.isStrapped)
    {
        ENETTRACE_DBG("PHY %u: disable", hPhy->addr);
        EnetPhy_rmwReg(hPhy, PHY_BMCR,
                       BMCR_ISOLATE | BMCR_PWRDOWN,
                       BMCR_ISOLATE | BMCR_PWRDOWN);
    }

    EnetPhy_releaseHandle(hPhy);
}

EnetPhy_LinkStatus EnetPhy_tick(EnetPhy_Handle hPhy)
{
    EnetPhy_State *state = &hPhy->state;
    EnetPhy_FsmState prevFsmState;
    EnetPhy_FsmState currFsmState;
    EnetPhy_LinkStatus status;

    /* TODO: Check if pending state transitions needs to be applied,
     * i.e. from EnetPhy_setPhyMode() */

    /* State before tick */
    prevFsmState = state->fsmState;

    /*
     * Act on current state of the PHY.
     * The following fields of the FSM state get updated by the end of this
     * switch-case block:
     *  - fsmState: with the state for next tick: new or same state
     *  - fsmStateChanged: whether FSM will go to a new state or same state in
     *    next tick
     *  - timeout: Number of ticks remaning:
     *     o One tick less if still at the same state
     *     o The number of ticks for the new state as per open-time config
     */
    switch (state->fsmState)
    {
        case ENETPHY_FSM_STATE_INIT:
            ENETTRACE_VERBOSE("PHY %u: INIT", hPhy->addr);
            EnetPhy_initState(hPhy);
            break;

        case ENETPHY_FSM_STATE_FINDING:
            ENETTRACE_VERBOSE("PHY %u: FINDING (rem ticks %u)",
                              hPhy->addr, state->timeout);
            EnetPhy_findingState(hPhy);
            break;

        case ENETPHY_FSM_STATE_FOUND:
            ENETTRACE_VERBOSE("PHY %u: FOUND", hPhy->addr);
            EnetPhy_foundState(hPhy);
            break;

        case ENETPHY_FSM_STATE_RESET_WAIT:
            ENETTRACE_VERBOSE("PHY %u: RESET_WAIT", hPhy->addr);
            EnetPhy_resetWaitState(hPhy);
            break;

        case ENETPHY_FSM_STATE_ENABLE:
            ENETTRACE_VERBOSE("PHY %u: ENABLE", hPhy->addr);
            EnetPhy_enableState(hPhy);
            break;

        case ENETPHY_FSM_STATE_NWAY_START:
            ENETTRACE_VERBOSE("PHY %u: NWAY_START (rem ticks %u)", hPhy->addr, state->timeout);
            EnetPhy_nwayStartState(hPhy);
            break;

        case ENETPHY_FSM_STATE_NWAY_WAIT:
            ENETTRACE_VERBOSE("PHY %u: NWAY_WAIT (rem ticks %u)", hPhy->addr, state->timeout);
            EnetPhy_nwayWaitState(hPhy);
            break;

        case ENETPHY_FSM_STATE_LINK_WAIT:
            ENETTRACE_VERBOSE("PHY %u: LINK_WAIT (rem ticks %u)", hPhy->addr, state->timeout);
            EnetPhy_linkWaitState(hPhy);
            break;

        case ENETPHY_FSM_STATE_LINKED:
            ENETTRACE_VERBOSE("PHY %u: LINKED", hPhy->addr);
            EnetPhy_linkedState(hPhy);
            break;

        case ENETPHY_FSM_STATE_LOOPBACK:
            ENETTRACE_VERBOSE("PHY %u: LOOPBACK", hPhy->addr);
            EnetPhy_loopbackState(hPhy);
            break;

        case ENETPHY_FSM_STATE_QUARANTINE:
            ENETTRACE_VERBOSE("PHY %u: QUARANTINE", hPhy->addr);
            break;

        default:
            ENETTRACE_VERBOSE("PHY %u: DEFAULT", hPhy->addr);
            EnetPhy_defaultState(hPhy);
            break;
    }

    /* State after tick */
    currFsmState = state->fsmState;

    /*
     * New link status:
     *  - Just got link (ENETPHY_GOT_LINK)
     *  - Already had link (ENETPHY_LINK_UP)
     *  - Just lost link (ENETPHY_LOST_LINK)
     *  - Link is still down (ENETPHY_LINK_DOWN)
     */
    switch (currFsmState)
    {
        case ENETPHY_FSM_STATE_LINKED:
        case ENETPHY_FSM_STATE_LOOPBACK:
            if (prevFsmState != currFsmState)
            {
                status = ENETPHY_GOT_LINK;
            }
            else
            {
                status = ENETPHY_LINK_UP;
            }
            break;

        case ENETPHY_FSM_STATE_INIT:
        case ENETPHY_FSM_STATE_FINDING:
        case ENETPHY_FSM_STATE_FOUND:
        case ENETPHY_FSM_STATE_RESET_WAIT:
        case ENETPHY_FSM_STATE_ENABLE:
        case ENETPHY_FSM_STATE_NWAY_START:
        case ENETPHY_FSM_STATE_NWAY_WAIT:
        case ENETPHY_FSM_STATE_LINK_WAIT:
        default:
            if ((prevFsmState == ENETPHY_FSM_STATE_LINKED) ||
                (prevFsmState == ENETPHY_FSM_STATE_LOOPBACK))
            {
                status = ENETPHY_LOST_LINK;
            }
            else
            {
                status = ENETPHY_LINK_DOWN;
            }
            break;
    }

    return status;
}

int32_t EnetPhy_isAlive(EnetPhy_Handle hPhy,
                        bool *isAlive)
{
    EnetPhy_MdioHandle hMdio = hPhy->hMdio;
    uint32_t phyGroup = hPhy->group;
    uint32_t phyAddr = hPhy->addr;
    uint16_t val = 0U;
    int32_t status = ENET_SOK;

    /* Get PHY alive status */
    if (hMdio->isAlive != NULL)
    {
        /* Get alive status from MDIO driver (i.e. hardware assisted) */
        status = hMdio->isAlive(phyAddr, isAlive, hPhy->mdioArgs);
        ENETTRACE_ERR_IF(status != ENETPHY_SOK, status,
                         "PHY %u: Failed to get alive status: %d", phyAddr, status);
    }
    else
    {
        /* Alternatively, read BMSR - PHY is alive if transaction is successful */
        status = hMdio->readC22(phyGroup, phyAddr, PHY_BMSR, &val, hPhy->mdioArgs);
        ENETTRACE_ERR_IF(status != ENETPHY_SOK, status,
                         "PHY %u: Failed to read reg %u: %d", phyAddr, PHY_BMSR, status);
        if (status == ENETPHY_SOK)
        {
            *isAlive = BTRUE;
        }
    }

    return status;
}

int32_t EnetPhy_getId(EnetPhy_Handle hPhy,
                      EnetPhy_Version *version)
{
    uint32_t phyAddr = hPhy->addr;
    uint16_t id1, id2;
    int32_t status = ENETPHY_SOK;
    bool isAlive;

    ENETTRACE_VAR(phyAddr);
    status = EnetPhy_isAlive(hPhy, &isAlive);
    if (status == ENETPHY_SOK)
    {
        if (isAlive == BFALSE)
        {
            status = ENETPHY_EFAIL;
        }
    }

    if (status == ENETPHY_SOK)
    {
        status = EnetPhy_readReg(hPhy, PHY_PHYIDR1, &id1);
        ENETTRACE_ERR_IF(status != ENETPHY_SOK, status,
                         "PHY %u: Failed to read ID1 register", phyAddr);
    }

    if (status == ENETPHY_SOK)
    {
        status = EnetPhy_readReg(hPhy, PHY_PHYIDR2, &id2);
        ENETTRACE_ERR_IF(status != ENETPHY_SOK, status,
                         "PHY %u: Failed to read ID2 register", phyAddr);
    }

    if (status == ENETPHY_SOK)
    {
        version->oui      = ((uint32_t)id1 << PHYIDR1_OUI_OFFSET);
        version->oui     |= (id2 & PHYIDR2_OUI_MASK)  >> PHYIDR2_OUI_OFFSET;
        version->model    = (id2 & PHYIDR2_VMDL_MASK) >> PHYIDR2_VMDL_OFFSET;
        version->revision = (id2 & PHYIDR2_VREV_MASK) >> PHYIDR2_VREV_OFFSET;
    }

    return status;
}

bool EnetPhy_isLinked(EnetPhy_Handle hPhy)
{
    EnetPhy_State *state = &hPhy->state;
    EnetPhy_FsmState fsmState = state->fsmState;
    EnetPhy_FsmState linkupFsmState;

    /* Link up is reached in LINKED or LOOPBACK states */
    linkupFsmState = state->loopbackEn ?
                     ENETPHY_FSM_STATE_LOOPBACK : ENETPHY_FSM_STATE_LINKED;

    return (fsmState == linkupFsmState);
}

int32_t EnetPhy_getLinkCfg(EnetPhy_Handle hPhy,
                           EnetPhy_LinkCfg *linkCfg)
{
    EnetPhy_State *state = &hPhy->state;
    bool isLinked;
    int32_t status = ENETPHY_SOK;

    isLinked = EnetPhy_isLinked(hPhy);
    if (isLinked)
    {
        linkCfg->speed = state->speed;
        linkCfg->duplexity = state->duplexity;
    }
    else
    {
        ENETTRACE_WARN("PHY %u: PHY is not linked, can't get link config", hPhy->addr);
        status = ENETPHY_EPERM;
    }

    return status;
}

int32_t EnetPhy_readReg(EnetPhy_Handle hPhy,
                        uint32_t reg,
                        uint16_t *val)
{
    EnetPhy_MdioHandle hMdio = hPhy->hMdio;
    uint32_t phyGroup = hPhy->group;
    uint32_t phyAddr = hPhy->addr;
    int32_t status;

    status = hMdio->readC22(phyGroup, phyAddr, reg, val, hPhy->mdioArgs);
    ENETTRACE_ERR_IF(status != ENETPHY_SOK, status,
                     "PHY %u: Failed to read reg %u", phyAddr, reg);
    ENETTRACE_VERBOSE_IF(status == ENETPHY_SOK,
                         "PHY %u: reg %u val 0x%04x %s", phyAddr, reg, *val);

    return status;
}

int32_t EnetPhy_writeReg(EnetPhy_Handle hPhy,
                         uint32_t reg,
                         uint16_t val)
{
    EnetPhy_MdioHandle hMdio = hPhy->hMdio;
    uint32_t phyGroup = hPhy->group;
    uint32_t phyAddr = hPhy->addr;
    int32_t status;

    status = hMdio->writeC22(phyGroup, phyAddr, reg, val, hPhy->mdioArgs);
    ENETTRACE_ERR_IF(status != ENETPHY_SOK, status,
                     "PHY %u: Failed to write reg %u", phyAddr, reg);
    ENETTRACE_VERBOSE_IF(status == ENETPHY_SOK,
                         "PHY %u: reg %u val 0x%04x", phyAddr, reg, val);

    return status;
}

int32_t EnetPhy_rmwReg(EnetPhy_Handle hPhy,
                       uint32_t reg,
                       uint16_t mask,
                       uint16_t val)
{
    EnetPhy_MdioHandle hMdio = hPhy->hMdio;
    uint32_t phyGroup = hPhy->group;
    uint32_t phyAddr = hPhy->addr;
    int32_t status;
    uint16_t data = 0U;

    status = hMdio->readC22(phyGroup, phyAddr, reg, &data, hPhy->mdioArgs);
    ENETTRACE_ERR_IF(status != ENETPHY_SOK, status,
                     "PHY %u: Failed to read reg %u: %d", phyAddr, reg);
    ENETTRACE_VERBOSE_IF(status == ENETPHY_SOK,
                         "PHY %u: read reg %u val 0x%04x", phyAddr, reg, data);

    if (status == ENETPHY_SOK)
    {
        data = (data & ~mask) | (val & mask);

        status = hMdio->writeC22(phyGroup, phyAddr, reg, data, hPhy->mdioArgs);
        ENETTRACE_ERR_IF(status != ENETPHY_SOK, status,
                         "PHY %u: Failed to write reg %u", phyAddr, reg);
        ENETTRACE_VERBOSE_IF(status == ENETPHY_SOK,
                             "PHY %u: write reg %u val 0x%04x", phyAddr, reg, data);
    }

    return status;
}

int32_t EnetPhy_readExtReg(EnetPhy_Handle hPhy,
                           uint32_t reg,
                           uint16_t *val)
{
    int32_t status = ENETPHY_ENOTSUPPORTED;

    if ((hPhy->hDrv != NULL) &&
        (hPhy->hDrv->readExtReg != NULL))
    {
        status = hPhy->hDrv->readExtReg(hPhy, reg, val);
    }

    return status;
}

int32_t EnetPhy_writeExtReg(EnetPhy_Handle hPhy,
                            uint32_t reg,
                            uint16_t val)
{
    int32_t status = ENETPHY_ENOTSUPPORTED;

    if ((hPhy->hDrv != NULL) &&
        (hPhy->hDrv->writeExtReg != NULL))
    {
        status = hPhy->hDrv->writeExtReg(hPhy, reg, val);
    }

    return status;
}

int32_t EnetPhy_rmwExtReg(EnetPhy_Handle hPhy,
                          uint32_t reg,
                          uint16_t mask,
                          uint16_t val)
{
    int32_t status = ENETPHY_ENOTSUPPORTED;
    uint16_t data = 0U;

    if ((hPhy->hDrv != NULL) &&
        (hPhy->hDrv->readExtReg != NULL) &&
        (hPhy->hDrv->writeExtReg != NULL))
    {
        status = hPhy->hDrv->readExtReg(hPhy, reg, &data);

        if (status == ENETPHY_SOK)
        {
            data = (data & ~mask) | (val & mask);

            status = hPhy->hDrv->writeExtReg(hPhy, reg, data);
        }
    }

    return status;
}

int32_t EnetPhy_readC45Reg(EnetPhy_Handle hPhy,
                           uint8_t mmd,
                           uint32_t reg,
                           uint16_t *val)
{
    EnetPhy_MdioHandle hMdio = hPhy->hMdio;
    uint32_t phyGroup = hPhy->group;
    uint32_t phyAddr = hPhy->addr;
    int32_t status;

    status = hMdio->readC45(phyGroup, phyAddr, mmd, reg, val, hPhy->mdioArgs);
    ENETTRACE_ERR_IF(status != ENETPHY_SOK, status,
                     "PHY %u: Failed to read MMD %u reg %u", phyAddr, mmd, reg);
    ENETTRACE_VERBOSE_IF(status == ENETPHY_SOK,
                         "PHY %u: MMD %u reg %u val 0x%04x %s", phyAddr, mmd, reg, *val);

    return status;
}

int32_t EnetPhy_writeC45Reg(EnetPhy_Handle hPhy,
                            uint8_t mmd,
                            uint32_t reg,
                            uint16_t val)
{
    EnetPhy_MdioHandle hMdio = hPhy->hMdio;
    uint32_t phyGroup = hPhy->group;
    uint32_t phyAddr = hPhy->addr;
    int32_t status;

    status = hMdio->writeC45(phyGroup, phyAddr, mmd, reg, val, hPhy->mdioArgs);
    ENETTRACE_ERR_IF(status != ENETPHY_SOK, status,
                     "PHY %u: Failed to write MMD %u reg %u", phyAddr, mmd, reg);
    ENETTRACE_VERBOSE_IF(status == ENETPHY_SOK,
                         "PHY %u: MMD %u reg %u val 0x%04x", phyAddr, mmd, reg, val);

    return status;
}

int32_t EnetPhy_rmwC45Reg(EnetPhy_Handle hPhy,
                          uint8_t mmd,
                          uint32_t reg,
                          uint16_t mask,
                          uint16_t val)
{
    EnetPhy_MdioHandle hMdio = hPhy->hMdio;
    uint32_t phyGroup = hPhy->group;
    uint32_t phyAddr = hPhy->addr;
    int32_t status;
    uint16_t data = 0U;

    status = hMdio->readC45(phyGroup, phyAddr, mmd, reg, &data, hPhy->mdioArgs);
    ENETTRACE_ERR_IF(status != ENETPHY_SOK, status,
                     "PHY %u: Failed to read MMD %u reg %u", phyAddr, mmd, reg);
    ENETTRACE_VERBOSE_IF(status == ENETPHY_SOK,
                         "PHY %u: read MMD %u reg %u val 0x%04x", phyAddr, mmd, reg, data);

    if (status == ENETPHY_SOK)
    {
        data = (data & ~mask) | (val & mask);

        status = hMdio->writeC45(phyGroup, phyAddr, mmd, reg, data, hPhy->mdioArgs);
        ENETTRACE_ERR_IF(status != ENETPHY_SOK, status,
                         "PHY %u: Failed to write MMD %u reg %u", phyAddr, mmd, reg);
        ENETTRACE_VERBOSE_IF(status == ENETPHY_SOK,
                             "PHY %u: write MMD %u reg %u val 0x%04x", phyAddr, mmd, reg, data);
    }

    return status;
}

void EnetPhy_printRegs(EnetPhy_Handle hPhy)
{
    if ((hPhy->hDrv != NULL) &&
        (hPhy->hDrv->printRegs != NULL))
    {
        hPhy->hDrv->printRegs(hPhy);
    }
}

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_WARN)
static const char *EnetPhy_getCapsString(uint32_t linkCaps)
{
    snprintf(gEnetPhyCapsBuf, ENETPHY_CAPS_BUF_LEN, "%s%s%s%s%s%s%s",
             ((linkCaps & ENETPHY_LINK_CAP_FD1000) != 0U) ? "FD1000 " : "",
             ((linkCaps & ENETPHY_LINK_CAP_HD1000) != 0U) ? "HD1000 " : "",
             ((linkCaps & ENETPHY_LINK_CAP_FD100) != 0U) ? "FD100 " : "",
             ((linkCaps & ENETPHY_LINK_CAP_HD100) != 0U) ? "HD100 " : "",
             ((linkCaps & ENETPHY_LINK_CAP_FD10) != 0U) ? "FD10 " : "",
             ((linkCaps & ENETPHY_LINK_CAP_HD10) != 0U) ? "HD10 " : "",
             ((linkCaps & ENETPHY_LINK_CAP_ALL) == 0U) ? "None" : "");

    return gEnetPhyCapsBuf;
}
#endif

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_DEBUG)
static const char *EnetPhy_getModeString(EnetPhy_Speed speed,
                                         EnetPhy_Duplexity duplexity)
{
    snprintf(gEnetPhyModeBuf, ENETPHY_MODE_BUF_LEN, "%sbps %s-duplex",
             (speed == ENETPHY_SPEED_1GBIT) ? "1 G" :
             (speed == ENETPHY_SPEED_100MBIT) ? "100 M" : "10 M",
             (duplexity == ENETPHY_DUPLEX_FULL) ? "full" : "half");

    return gEnetPhyModeBuf;
}
#endif

static EnetPhy_Handle EnetPhy_getHandle(void)
{
    EnetPhy_Handle hPhy = NULL;
    uint32_t i;

    for (i = 0U; i < ENETPHY_ARRAYSIZE(gEnetPhy_phyObjs); i++)
    {
        if (gEnetPhy_phyObjs[i].magic != ENETPHY_MAGIC)
        {
            hPhy = &gEnetPhy_phyObjs[i];
            memset(hPhy, 0, sizeof(*hPhy));
            hPhy->magic = ENETPHY_MAGIC;
            break;
        }
    }

    return hPhy;
}

static void EnetPhy_releaseHandle(EnetPhy_Handle hPhy)
{
    hPhy->magic = ENETPHY_NO_MAGIC;
}

static void EnetPhy_setNextState(EnetPhy_Handle hPhy,
                                 EnetPhy_FsmState fsmState)
{
    EnetPhy_FsmTimeoutCfg *timeoutCfg = &hPhy->phyCfg.timeoutCfg;
    EnetPhy_FsmState prevFsmState = hPhy->state.fsmState;
    uint32_t timeout;
    uint32_t residenceTime = 0U;

    switch (fsmState)
    {
        case ENETPHY_FSM_STATE_INIT:
            timeout = 0U;
            break;

        case ENETPHY_FSM_STATE_FINDING:
            timeout = timeoutCfg->findingStateTicks;
            break;

        case ENETPHY_FSM_STATE_FOUND:
            timeout = 0U;
            break;

        case ENETPHY_FSM_STATE_RESET_WAIT:
            timeout = timeoutCfg->resetWaitStateTicks;
            residenceTime = timeoutCfg->resetWaitStateResidenceTicks;
            break;

        case ENETPHY_FSM_STATE_ENABLE:
            timeout = 0U;
            break;

        case ENETPHY_FSM_STATE_NWAY_START:
            timeout = timeoutCfg->nwayStartStateTicks;
            break;

        case ENETPHY_FSM_STATE_NWAY_WAIT:
            timeout = timeoutCfg->nwayWaitStateTicks;
            break;

        case ENETPHY_FSM_STATE_LINK_WAIT:
            timeout = timeoutCfg->linkWaitStateTicks;
            break;

        case ENETPHY_FSM_STATE_LINKED:
            timeout = 0U;
            break;

        case ENETPHY_FSM_STATE_LOOPBACK:
            timeout = 0U;
            break;

        case ENETPHY_FSM_STATE_QUARANTINE:
            ENETTRACE_ERR(ENET_EUNEXPECTED, "PHY %u: Entering disabled (quarantined) state", hPhy->addr);
            timeout = 0U;
            break;

        default:
            timeout = 0U;
            break;
    }

    ENETTRACE_DBG("PHY %u: %s -> %s (%u ticks)",
                  hPhy->addr,
                  gEnetPhyStateNames[hPhy->state.fsmState],
                  gEnetPhyStateNames[fsmState],
                  timeout);

    hPhy->state.fsmState = fsmState;
    hPhy->state.timeout = timeout;
    hPhy->state.residenceTime = residenceTime;
    hPhy->state.fsmStateChanged = (fsmState != prevFsmState);
}

static void EnetPhy_phyTimeout(EnetPhy_Handle hPhy)
{
    ENETTRACE_DBG("PHY %u: timeout has occurred", hPhy->addr);

    /* Reset state machine to FOUND for non-strapped PHY, or to LINK_WAIT for
     * state for strapped PHY as most FSM states are bypassed */
    if (hPhy->phyCfg.isStrapped)
    {
        EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_LINK_WAIT);
    }
    else
    {
        EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_FOUND);
    }
}

static void EnetPhy_initState(EnetPhy_Handle hPhy)
{
    EnetPhy_State *state = &hPhy->state;

    state->speed = ENETPHY_SPEED_10MBIT;
    state->duplexity = ENETPHY_DUPLEX_HALF;
    state->phyLinkCaps = 0U;

    if (hPhy->phyCfg.isStrapped)
    {
        state->needsManualCfg = BFALSE;
        state->needsNwayCfg   = BFALSE;
    }
    else
    {
        state->needsManualCfg = BTRUE;
        state->needsNwayCfg   = BTRUE;
    }

    EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_FINDING);
}

static void EnetPhy_defaultState(EnetPhy_Handle hPhy)
{
    hPhy->state.fsmStateChanged = BTRUE;
}

static void EnetPhy_findingState(EnetPhy_Handle hPhy)
{
    EnetPhy_State *state = &hPhy->state;
    uint32_t phyAddr = hPhy->addr;
    int32_t status;
    bool isAlive;

    ENETTRACE_VAR(phyAddr);
    status = EnetPhy_isAlive(hPhy, &isAlive);
    if (status == ENETPHY_SOK)
    {
        if (isAlive)
        {
            /* PHY found */
            ENETTRACE_DBG("PHY %u: PHY has been found", phyAddr);

            status = EnetPhy_bindDriver(hPhy);
            if (status == ENETPHY_SOK)
            {
                if (hPhy->phyCfg.isStrapped)
                {
                    EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_LINK_WAIT);
                }
                else
                {
                    EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_FOUND);
                }
            }
            else
            {
                /* TODO: Go to a 'disable' state */
                ENETTRACE_ERR(status, "PHY %u: couldn't be bound to any driver", phyAddr);
            }
        }
        else
        {
            if (state->timeout != 0U)
            {
                state->timeout--;
            }
            else
            {
                ENETTRACE_DBG("PHY %u: timed out", phyAddr);
                state->timeout = ENETPHY_TIMEOUT_FINDING;
            }
        }
    }
    
    if (status == ENETPHY_EUNEXPECTED)
    {
        EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_QUARANTINE);
    }
}

static void EnetPhy_foundState(EnetPhy_Handle hPhy)
{
    Enet_devAssert(!hPhy->phyCfg.isStrapped,
                   "PHY %u: unexpected state for strapped PHY", hPhy->addr);

    EnetPhy_resetPhy(hPhy);

    EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_RESET_WAIT);
}

static void EnetPhy_resetWaitState(EnetPhy_Handle hPhy)
{
    EnetPhy_State *state = &hPhy->state;
    bool complete;

    Enet_devAssert(hPhy->hDrv->isResetComplete != NULL,
                   "PHY %u: isResetComplete callback not implemented", hPhy->addr);
    Enet_devAssert(!hPhy->phyCfg.isStrapped,
                   "PHY %u: unexpected state for strapped PHY", hPhy->addr);

    /* Wait for PHY reset to complete */
    complete = hPhy->hDrv->isResetComplete(hPhy);
    if (complete)
    {
        if (state->residenceTime != 0U)
        {
            state->residenceTime--;
        }
        else
        {
            EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_ENABLE);
        }
    }
    else
    {
        if (state->timeout != 0U)
        {
            state->timeout--;
        }
        else
        {
            EnetPhy_phyTimeout(hPhy);
        }
    }
}

static void EnetPhy_enableState(EnetPhy_Handle hPhy)
{
    EnetPhy_State *state = &hPhy->state;
    uint32_t socLinkCaps;
    uint32_t commonLinkCaps = 0U;
    int32_t status = ENET_SOK;

    ENETTRACE_VAR(status);

    Enet_devAssert(!hPhy->phyCfg.isStrapped,
                   "PHY %u: unexpected state for strapped PHY", hPhy->addr);

    /* Enable PHY (resetting it if applicable) can be done only when
     * entering the ENABLE state for the first time */
    if (state->needsNwayCfg ||
        state->needsManualCfg)
    {
        /* Set PHY in normal mode */
        ENETTRACE_DBG("PHY %u: enable", hPhy->addr);
        status = EnetPhy_rmwReg(hPhy, PHY_BMCR, BMCR_ISOLATE | BMCR_PWRDOWN, 0U);
        
        if (status == ENETPHY_SOK)
        {
            /* PHY-specific 'extended' configuration */
            if ((hPhy->hDrv->config != NULL) &&
                !hPhy->phyCfg.skipExtendedCfg)
            {
                hPhy->hDrv->config(hPhy, &hPhy->phyCfg, hPhy->mii);
            }
        }
    }

    if (status == ENETPHY_SOK)
    {
        /* Find common capabilities among app's, PHY's and SoC's */
        ENETTRACE_DBG("PHY %u: req caps: %s",
                      hPhy->addr, EnetPhy_getCapsString(hPhy->reqLinkCaps));

        status = EnetPhy_getLocalCaps(hPhy, &state->phyLinkCaps);
    }

    if (status == ENETPHY_SOK)
    {
        ENETTRACE_DBG("PHY %u: PHY caps: %s",
                      hPhy->addr, EnetPhy_getCapsString(state->phyLinkCaps));

        socLinkCaps = hPhy->macCaps;
        ENETTRACE_DBG("PHY %u: MAC caps: %s",
                      hPhy->addr, EnetPhy_getCapsString(socLinkCaps));

        commonLinkCaps = hPhy->reqLinkCaps &
                         state->phyLinkCaps &
                         socLinkCaps;
        ENETTRACE_DBG("PHY %u: refined caps: %s",
                      hPhy->addr, EnetPhy_getCapsString(commonLinkCaps));
    }

    if (commonLinkCaps != 0U)
    {
        state->linkCaps = commonLinkCaps;

        /* Enable NWAY if requested by app (via auto-speed or auto-duplex) and
         * supported by PHY */
        status = EnetPhy_isNwayCapable(hPhy, &state->isNwayCapable);
        if (status == ENETPHY_SOK)
        {
            ENETTRACE_DBG("PHY %u: PHY is %sNWAY-capable",
                          hPhy->addr, state->isNwayCapable ? "" : "not ");
            
            state->enableNway = (hPhy->linkCfg.speed == ENETPHY_SPEED_AUTO) ||
                                (hPhy->linkCfg.duplexity == ENETPHY_DUPLEX_AUTO);
            if (state->enableNway && !state->isNwayCapable)
            {
                /* If PHY is not capable of auto negotiation, fallback to manual mode
                 * with the highest refined capability */
                ENETTRACE_WARN("PHY %u: falling back to manual mode", hPhy->addr);
                state->enableNway = BFALSE;

                commonLinkCaps = EnetPhy_findBestCap(commonLinkCaps);
                ENETTRACE_WARN("PHY %u: new link caps: %s",
                               hPhy->addr, EnetPhy_getCapsString(commonLinkCaps));
                state->linkCaps = commonLinkCaps;
            }
        }
    }

    if (status == ENETPHY_SOK)
    {
        if (commonLinkCaps == 0U)
        {
            status = ENET_ENOTSUPPORTED;
            ENETTRACE_ERR(status, "PHY %u: no supported caps found", hPhy->addr);
        }
        else if (state->loopbackEn)
        {
            ENETTRACE_DBG("PHY %u: setup loopback", hPhy->addr);
            status = EnetPhy_setupManual(hPhy, BTRUE);
            if (status == ENETPHY_EUNEXPECTED) 
            {
                EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_QUARANTINE);
            }
            else
            {
                EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_LOOPBACK);
            }
        }
        else if (state->isNwayCapable && state->enableNway)
        {
            ENETTRACE_DBG("PHY %u: setup NWAY", hPhy->addr);
            status = EnetPhy_setupNway(hPhy);
            if (status == ENETPHY_EUNEXPECTED)
            {
                EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_QUARANTINE);
            }
            else
            {
                EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_NWAY_START);
            }
        }
        else
        {
            ENETTRACE_DBG("PHY %u: manual setup", hPhy->addr);
            status = EnetPhy_setupManual(hPhy, BFALSE);
            if (status == ENETPHY_EUNEXPECTED) 
            {
                EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_QUARANTINE);
            }
            else
            {
                EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_LINK_WAIT);
            }
        }
    }
    else if (status == ENETPHY_EUNEXPECTED)
    {
        EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_QUARANTINE);
    }
}

static int32_t EnetPhy_setupNway(EnetPhy_Handle hPhy)
{
    EnetPhy_State *state = &hPhy->state;
    uint16_t nwayAdvertise;
    uint16_t nway1000Advertise = 0U;
    int32_t status = ENET_SOK;

    ENETTRACE_DBG("PHY %u: NWAY advertising: %s",
                  hPhy->addr, EnetPhy_getCapsString(state->linkCaps));

    ENETTRACE_DBG("PHY %u: config is %sneeded",
                  hPhy->addr, state->needsNwayCfg ? "" : "not ");

    if (state->needsNwayCfg)
    {
        nwayAdvertise = ANAR_802P3;
        if ((state->linkCaps & ENETPHY_LINK_CAP_FD100) != 0U)
        {
            nwayAdvertise |= ANAR_100FD;
        }

        if ((state->linkCaps & ENETPHY_LINK_CAP_HD100) != 0U)
        {
            nwayAdvertise |= ANAR_100HD;
        }

        if ((state->linkCaps & ENETPHY_LINK_CAP_FD10) != 0U)
        {
            nwayAdvertise |= ANAR_10FD;
        }

        if ((state->linkCaps & ENETPHY_LINK_CAP_HD10) != 0U)
        {
            nwayAdvertise |= ANAR_10HD;
        }

        if ((state->linkCaps & ENETPHY_LINK_CAP_FD1000) != 0U)
        {
            nway1000Advertise |= GIGCR_1000FD;
        }

        if ((state->linkCaps & ENETPHY_LINK_CAP_HD1000) != 0U)
        {
            nway1000Advertise |= GIGCR_1000HD;
        }

        status = EnetPhy_rmwReg(hPhy, PHY_ANAR, ANAR_100 | ANAR_10, nwayAdvertise);

        if (status == ENETPHY_SOK)
        {
            if ((state->phyLinkCaps & ENETPHY_LINK_CAP_1000) != 0U)
            {
                status = EnetPhy_rmwReg(hPhy, PHY_GIGCR, GIGCR_1000, nway1000Advertise);
            }
        }

        if (status == ENETPHY_SOK)
        {
            state->needsNwayCfg = BFALSE;
        }
    }

    if (status == ENETPHY_SOK)
    {
        /* Restart auto-negotiation */
        ENETTRACE_DBG("PHY %u: restart autonegotiation", hPhy->addr);
        status = EnetPhy_rmwReg(hPhy, PHY_BMCR, BMCR_ANEN, BMCR_ANEN);
    }

    if (status == ENETPHY_SOK)
    {
        status = EnetPhy_rmwReg(hPhy, PHY_BMCR,
                                BMCR_ANRESTART | BMCR_FD,
                                BMCR_ANRESTART | BMCR_FD);
    }

    return status;
}

static int32_t EnetPhy_setupManual(EnetPhy_Handle hPhy,
                                bool loopbackEn)
{
    EnetPhy_State *state = &hPhy->state;
    uint16_t val;
    int32_t status = ENET_SOK;

    /* Manual config is needed if entered the FOUND state for the first time
     * or if an explicit mode change was requested via EnetPhy_setPhyMode() */
    if (state->needsManualCfg)
    {
        /* Disable auto-negotiation if supported by PHY */
        if (state->isNwayCapable)
        {
            ENETTRACE_DBG("PHY %u: disable NWAY", hPhy->addr);
            status = EnetPhy_rmwReg(hPhy, PHY_BMCR, BMCR_ANEN, 0U);
        }

        if (status == ENET_SOK)
        {
            switch (state->linkCaps)
            {
                case ENETPHY_LINK_CAP_FD1000:
                    val = BMCR_SPEED1000 | BMCR_FD;
                    break;

                case ENETPHY_LINK_CAP_HD1000:
                    val = BMCR_SPEED1000;
                    break;

                case ENETPHY_LINK_CAP_FD100:
                    val = BMCR_SPEED100 | BMCR_FD;
                    break;

                case ENETPHY_LINK_CAP_HD100:
                    val = BMCR_SPEED100;
                    break;

                case ENETPHY_LINK_CAP_FD10:
                    val = BMCR_FD;
                    break;

                case ENETPHY_LINK_CAP_HD10:
                default:
                    val = 0U;
                    break;
            }

            if (loopbackEn)
            {
                val |= BMCR_LOOPBACK;
            }

            status = EnetPhy_rmwReg(hPhy, PHY_BMCR,
                                    BMCR_LOOPBACK | BMCR_SPEED1000 | BMCR_SPEED100 | BMCR_FD,
                                    val);
        }

        if (status == ENET_SOK)
        {
            /* Clear advertised capabilities values if supported */
            if (state->isNwayCapable)
            {
                status = EnetPhy_rmwReg(hPhy, PHY_ANAR, ANAR_100 | ANAR_10, 0U);
                
                if (status == ENET_SOK)
                {
                    if ((state->phyLinkCaps & ENETPHY_LINK_CAP_1000) != 0U)
                    {
                        status = EnetPhy_rmwReg(hPhy, PHY_GIGCR, GIGCR_1000, 0U);
                    }
                }
            }
        }
        
        if (status == ENET_SOK)
        {
            state->needsManualCfg = BFALSE;
        }
    }

    if (status == ENET_SOK)
    {
        EnetPhy_capToMode(state->linkCaps, &state->speed, &state->duplexity);

        ENETTRACE_DBG("PHY %u: requested mode: %s",
                      hPhy->addr, EnetPhy_getModeString(state->speed, state->duplexity));
    }

    return status;
}

static void EnetPhy_loopbackState(EnetPhy_Handle hPhy)
{
    bool loopback = BFALSE;
    int32_t status;
    uint16_t val;

    status = EnetPhy_readReg(hPhy, PHY_BMCR, &val);
    if (status == ENETPHY_SOK)
    {
        loopback = ((val & BMCR_LOOPBACK) != 0U);
    }
    else
    {
        ENETTRACE_ERR(status, "PHY %u: Failed to read BMCR register", hPhy->addr);
    }

    if (status == ENET_SOK)
    {
        if (!loopback)
        {
            EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_FOUND);
        }
    }
    else if (status == ENETPHY_EUNEXPECTED)
    {
        EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_QUARANTINE);
    }
    else
    {
        /* Do nothing, retry in next tick */
    }
}

static void EnetPhy_nwayStartState(EnetPhy_Handle hPhy)
{
    EnetPhy_State *state = &hPhy->state;
    uint16_t mode;
    uint16_t val;
    int32_t status;

    Enet_devAssert(!hPhy->phyCfg.isStrapped,
                   "PHY %u: unexpected state for strapped PHY", hPhy->addr);

    /* Wait for NWAY to start */
    status = EnetPhy_readReg(hPhy, PHY_BMCR, &mode);

    if (status == ENETPHY_SOK)
    {
        if ((mode & BMCR_ANRESTART) == 0U)
        {
            /* Flush pending latch bits */
            status = EnetPhy_readReg(hPhy, PHY_BMSR, &val);
            if (status == ENETPHY_SOK)
            {
                EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_NWAY_WAIT);
            }
        }
        else
        {
            if (state->timeout != 0U)
            {
                state->timeout--;
            }
            else
            {
                EnetPhy_phyTimeout(hPhy);
            }
        }
    }

    if (status == ENETPHY_EUNEXPECTED)
    {
        EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_QUARANTINE);
    }

    if (status != ENETPHY_SOK)
    {
        ENETTRACE_ERR(status, "PHY %u: error in nwayStartState", hPhy->addr);
    }
}

static void EnetPhy_nwayWaitState(EnetPhy_Handle hPhy)
{
    EnetPhy_State *state = &hPhy->state;
    uint32_t nwayCaps;
    uint16_t bmsrVal;
    int32_t status = ENETPHY_SOK;

    Enet_devAssert(!hPhy->phyCfg.isStrapped,
                   "PHY %u: unexpected state for strapped PHY", hPhy->addr);

    status = EnetPhy_readReg(hPhy, PHY_BMSR, &bmsrVal);

    if (status == ENETPHY_SOK)
    {
        if (0U != (bmsrVal & BMSR_ANCOMPLETE))
        {
            status = EnetPhy_findCommonNwayCaps(hPhy, &nwayCaps);
            if (status == ENETPHY_SOK)
            {
                if (nwayCaps != 0U)
                {
                    EnetPhy_capToMode(nwayCaps, &state->speed, &state->duplexity);

                    ENETTRACE_DBG("PHY %u: negotiated mode: %s",
                                  hPhy->addr, EnetPhy_getModeString(state->speed, state->duplexity));

                    if ((bmsrVal & BMSR_LINKSTS) != 0U)
                    {
                        EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_LINKED);
                    }
                    else
                    {
                        EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_LINK_WAIT);
                    }
                }
            }
        }
        else
        {
            if (state->timeout != 0U)
            {
                state->timeout--;
            }
            else
            {
                EnetPhy_phyTimeout(hPhy);
            }
        }
    }

    if (status == ENETPHY_EUNEXPECTED)
    {
        EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_QUARANTINE);
    }
}

static void EnetPhy_linkWaitState(EnetPhy_Handle hPhy)
{
    EnetPhy_State *state = &hPhy->state;
    uint32_t nwayCaps;
    uint16_t bsmrVal;
    int32_t status;

    status = EnetPhy_readReg(hPhy, PHY_BMSR, &bsmrVal);

    if (status == ENETPHY_SOK)
    {
        if ((bsmrVal & BMSR_LINKSTS) != 0U)
        {
            /* Populate FSM state now as most FSM states are bypassed for strapped PHYs */
            if (hPhy->phyCfg.isStrapped)
            {
                status = EnetPhy_getLocalCaps(hPhy, &state->phyLinkCaps);
                if (status == ENETPHY_SOK)
                {
                    status = EnetPhy_findCommonNwayCaps(hPhy, &nwayCaps);
                    if (status == ENETPHY_SOK)
                    {
                        if (nwayCaps != 0U)
                        {
                            EnetPhy_capToMode(nwayCaps, &state->speed, &state->duplexity);

                            ENETTRACE_DBG("PHY %u: negotiated mode: %s",
                                          hPhy->addr, EnetPhy_getModeString(state->speed, state->duplexity));
                        }
                    }
                }
            }
            if (status == ENETPHY_SOK)
            {
                EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_LINKED);
            }
        }
        else
        {
            if (state->timeout != 0U)
            {
                state->timeout--;
            }
            else
            {
                EnetPhy_phyTimeout(hPhy);
            }
        }
    }

    if (status == ENETPHY_EUNEXPECTED)
    {
        EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_QUARANTINE);
    }
}

static void EnetPhy_linkedState(EnetPhy_Handle hPhy)
{
    bool linked = BFALSE;
    int32_t status = ENETPHY_SOK;

    if ((hPhy->hDrv != NULL) && (hPhy->hDrv->isPhyLinked != NULL))
    {
        status = hPhy->hDrv->isPhyLinked(hPhy, &linked);
    }
    else if((hPhy->hDrv != NULL) && (hPhy->hDrv->isPhyLinked == NULL))
    {
        ENETTRACE_DBG("isPhyLinked function not defined in driver. "
                      "Falling back to Generic PHY's definition.");
        status = GenericPhy_isPhyLinked(hPhy, &linked);
    }
    else
    {
        status = ENETPHY_EUNEXPECTED;
        ENETTRACE_ERR(status, "Cannot find link status. No supported PHY Driver");
    }

    if (status == ENETPHY_SOK)
    {
        /* Not linked */
        if (!linked)
        {
            if (hPhy->phyCfg.isStrapped)
            {
                EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_LINK_WAIT);
            }
            else
            {
                EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_FOUND);
            }
        }
    }

    if (status == ENETPHY_EUNEXPECTED)
    {
        EnetPhy_setNextState(hPhy, ENETPHY_FSM_STATE_QUARANTINE);
    }
}


static int32_t EnetPhy_isNwayCapable(EnetPhy_Handle hPhy, bool *isNwayCapable)
{
    uint16_t val = 0U;
    int32_t status;

    /* Get the PHY Status */
    status = EnetPhy_readReg(hPhy, PHY_BMSR, &val);
    *isNwayCapable = ((val & BMSR_ANCAPABLE) != 0U);

    return status;
}

static int32_t EnetPhy_getLocalCaps(EnetPhy_Handle hPhy, uint32_t *caps)
{
    *caps = 0U;
    uint16_t val  = 0U;
    int32_t status = ENETPHY_SOK;

    /* Get 10/100 Mbps capabilities */
    status = EnetPhy_readReg(hPhy, PHY_BMSR, &val);

    if (status == ENETPHY_SOK)
    {
        if ((val & BMSR_100FD) != 0U)
        {
            *caps |= ENETPHY_LINK_CAP_FD100;
        }

        if ((val & BMSR_100HD) != 0U)
        {
            *caps |= ENETPHY_LINK_CAP_HD100;
        }

        if ((val & BMSR_10FD) != 0U)
        {
            *caps |= ENETPHY_LINK_CAP_FD10;
        }

        if ((val & BMSR_10HD) != 0U)
        {
            *caps |= ENETPHY_LINK_CAP_HD10;
        }

        /* Get extended (1 Gbps) capabilities if supported */
        if ((val & BMSR_GIGEXTSTS) != 0U)
        {
            status = EnetPhy_readReg(hPhy, PHY_GIGESR, &val);
            
            if (status == ENETPHY_SOK)
            {
                if ((val & GIGESR_1000FD) != 0U)
                {
                    *caps |= ENETPHY_LINK_CAP_FD1000;
                }

                if ((val & GIGESR_1000HD) != 0U)
                {
                    *caps |= ENETPHY_LINK_CAP_HD1000;
                }
            }
        }
    }

    return status;
}

static int32_t EnetPhy_findCommonCaps(EnetPhy_Handle hPhy, uint32_t *commonCaps)
{
    uint32_t localCaps = 0U;
    uint32_t partnerCaps = 0U;
    uint16_t val = 0U;
    int32_t status = ENETPHY_SOK;

    /* Get local device capabilities */
    status = EnetPhy_readReg(hPhy, PHY_ANAR, &val);

    if (status == ENETPHY_SOK)
    {
        if ((val & ANAR_100FD) != 0U)
        {
            localCaps |= ENETPHY_LINK_CAP_FD100;
        }

        if ((val & ANAR_100HD) != 0U)
        {
            localCaps |= ENETPHY_LINK_CAP_HD100;
        }

        if ((val & ANAR_10FD) != 0U)
        {
            localCaps |= ENETPHY_LINK_CAP_FD10;
        }

        if ((val & ANAR_10HD) != 0U)
        {
            localCaps |= ENETPHY_LINK_CAP_HD10;
        }
        
        /* Get link partner capabilities */
        val = 0U;
        status = EnetPhy_readReg(hPhy, PHY_ANLPAR, &val);
    }

    if (status == ENETPHY_SOK)
    {
        if ((val & ANLPAR_100FD) != 0U)
        {
            partnerCaps |= ENETPHY_LINK_CAP_FD100;
        }

        if ((val & ANLPAR_100HD) != 0U)
        {
            partnerCaps |= ENETPHY_LINK_CAP_HD100;
        }

        if ((val & ANLPAR_10FD) != 0U)
        {
            partnerCaps |= ENETPHY_LINK_CAP_FD10;
        }

        if ((val & ANLPAR_10HD) != 0U)
        {
            partnerCaps |= ENETPHY_LINK_CAP_HD10;
        }

        ENETTRACE_DBG("PHY %u: local caps: %s",
                      hPhy->addr, EnetPhy_getCapsString(localCaps));
        ENETTRACE_DBG("PHY %u: partner caps: %s",
                      hPhy->addr, EnetPhy_getCapsString(partnerCaps));
        ENETTRACE_DBG("PHY %u: common caps: %s",
                      hPhy->addr, EnetPhy_getCapsString(localCaps & partnerCaps));
    }
    else
    {
        ENETTRACE_ERR(status, "PHY %u: Error getting common capabilities", hPhy->addr);
    }

    *commonCaps = ENETPHY_SOK ? (localCaps & partnerCaps) : 0;

    return status;
}

static int32_t EnetPhy_findCommon1000Caps(EnetPhy_Handle hPhy, uint32_t *commonCaps)
{
    uint32_t localCaps = 0U;
    uint32_t partnerCaps = 0U;
    uint16_t val;
    int32_t status;

    /* Get local device capabilities */
    status = EnetPhy_readReg(hPhy, PHY_GIGCR, &val);
    
    if (status == ENETPHY_SOK)
    {
        if ((val & GIGCR_1000FD) != 0U)
        {
            localCaps |= ENETPHY_LINK_CAP_FD1000;
        }

        if ((val & GIGCR_1000HD) != 0U)
        {
            localCaps |= ENETPHY_LINK_CAP_HD1000;
        }

        /* Get link partner capabilities */
        status = EnetPhy_readReg(hPhy, PHY_GIGSR, &val);
    }

    if (status == ENETPHY_SOK)
    {
        if ((val & GIGSR_1000FD) != 0U)
        {
            partnerCaps |= ENETPHY_LINK_CAP_FD1000;
        }

        if ((val & GIGSR_1000FD) != 0U)
        {
            partnerCaps |= ENETPHY_LINK_CAP_HD1000;
        }

        ENETTRACE_DBG("PHY %u: local caps: %s",
                      hPhy->addr, EnetPhy_getCapsString(localCaps));
        ENETTRACE_DBG("PHY %u: partner caps: %s",
                      hPhy->addr, EnetPhy_getCapsString(partnerCaps));
        ENETTRACE_DBG("PHY %u: common caps: %s",
                      hPhy->addr, EnetPhy_getCapsString(localCaps & partnerCaps));
    }
    else
    {
        ENETTRACE_ERR(status, "PHY %u: Error getting common 1000 capabilities", hPhy->addr);
    }

    *commonCaps = status == ENETPHY_SOK ? (localCaps & partnerCaps) : 0U;
    return status;
}

static int32_t EnetPhy_findCommonNwayCaps(EnetPhy_Handle hPhy, uint32_t *nwayCaps)
{
    EnetPhy_State *state = &hPhy->state;
    uint32_t nway1000Caps = 0U;
    int32_t status = ENETPHY_SOK;

    status = EnetPhy_findCommonCaps(hPhy, nwayCaps);

    if (status == ENETPHY_SOK)
    {
        /* Find common gigabit capabilities if supported by PHY */
        if ((state->phyLinkCaps & ENETPHY_LINK_CAP_1000) != 0U)
        {
            status = EnetPhy_findCommon1000Caps(hPhy, &nway1000Caps);
            if (status == ENETPHY_SOK)
            {
                *nwayCaps |= nway1000Caps;
            }
        }
    }

    if (status == ENETPHY_SOK)
    {
        ENETTRACE_DBG("PHY %u: common caps: %s",
                      hPhy->addr, EnetPhy_getCapsString(*nwayCaps));

        if (*nwayCaps != 0U)
        {
            /* Find the highest performance protocol from the common
            * capabilities of local device and link partner */
            *nwayCaps = EnetPhy_findBestCap(*nwayCaps);
        }
        else
        {
            /* Non-compiant PHY? Flag the negotiation error */
            ENETTRACE_ERR(status, "PHY %u: no common caps found", hPhy->addr);
        }
    }

    return status;

}

static int32_t EnetPhy_bindDriver(EnetPhy_Handle hPhy)
{
    EnetPhyDrv_Handle hDrv = NULL;
    EnetPhy_Version version;
    bool match;
    bool macSupported;
    uint32_t i;
    int32_t status = ENETPHY_SOK;

    hPhy->hDrv = NULL;

    status = EnetPhy_getId(hPhy, &version);
    if (status == ENETPHY_SOK)
    {
        for (i = 0U; i < gEnetPhyDrvsSize; i++)
        {
            hDrv = gEnetPhyDrvs[i];

            Enet_devAssert(hDrv != NULL,
                           "PHY %u: No PHY driver handle at index %u",
                           hPhy->addr, i);
            Enet_devAssert(hDrv->isPhyDevSupported != NULL,
                           "PHY %u: PHY driver '%s' doesn't provide isPhyDevSupported callback",
                           hPhy->addr, hDrv->name);

            ENETTRACE_DBG("PHY %u: OUI:%06x Model:%02x Ver:%02x <-> '%s'",
                          hPhy->addr,
                          version.oui, version.model, version.revision,
                          hDrv->name);

            /* Check if driver supports detected PHY device */
            match = hDrv->isPhyDevSupported(hPhy, &version);

            /* Check if driver supports the MAC mode */
            if (match)
            {
                macSupported = hDrv->isMacModeSupported(hPhy, hPhy->mii);
                if (!macSupported)
                {
                    ENETTRACE_WARN("PHY %u: '%s' doesn't support MAC mode %d",
                                   hPhy->addr, hDrv->name, hPhy->mii);
                }
            }

            /* Bind the device <-> driver now */
            if (match)
            {
                hPhy->hDrv = hDrv;
                break;
            }
        }

        ENETTRACE_INFO_IF(hPhy->hDrv != NULL,
                          "PHY %u: OUI:%06x Model:%02x Ver:%02x <-> '%s' : OK",
                          hPhy->addr,
                          version.oui, version.model, version.revision,
                          hDrv->name);
    }

    if (hPhy->hDrv == NULL)
    {
        if (status != ENETPHY_EUNEXPECTED)
        {
            status = ENETPHY_EFAIL;
        }
    }

    return status;
}

static void EnetPhy_resetPhy(EnetPhy_Handle hPhy)
{
    EnetPhy_State *state = &hPhy->state;

    ENETTRACE_WARN_IF(hPhy->hDrv == NULL,
                      "PHY %u: not bound to a driver, can't be reset", hPhy->addr);

    /* PHY specific reset */
    if ((hPhy->hDrv != NULL) &&
        (hPhy->hDrv->reset != NULL))
    {
        hPhy->hDrv->reset(hPhy);

        if (hPhy->phyCfg.isStrapped)
        {
            /* Strapped PHY doesn't need either Auto-Negotiation or manual configuration */
            state->needsManualCfg = BFALSE;
            state->needsNwayCfg   = BFALSE;
        }
        else
        {
            /* Typically manual/auto-negotiation should be reconfigured after PHY reset.
             * Certain reset/restart implementations only reset the logic not registers,
             * but it's safer to assume reconfiguration is needed */
            state->needsManualCfg = BTRUE;
            state->needsNwayCfg   = BTRUE;
        }
    }
}

static uint32_t EnetPhy_getManualCaps(EnetPhy_Speed speed,
                                      EnetPhy_Duplexity duplexity)
{
    uint32_t caps = 0U;

    Enet_devAssert((speed != ENETPHY_SPEED_AUTO) &&
                   (duplexity != ENETPHY_DUPLEX_AUTO),
                   "Invalid speed/duplex for manual mode");

    switch (speed)
    {
        case ENETPHY_SPEED_10MBIT:
            caps = (duplexity == ENETPHY_DUPLEX_FULL) ?
                   ENETPHY_LINK_CAP_FD10 :
                   ENETPHY_LINK_CAP_HD10;
            break;

        case ENETPHY_SPEED_100MBIT:
            caps = (duplexity == ENETPHY_DUPLEX_FULL) ?
                   ENETPHY_LINK_CAP_FD100 :
                   ENETPHY_LINK_CAP_HD100;
            break;

        case ENETPHY_SPEED_1GBIT:
            caps = (duplexity == ENETPHY_DUPLEX_FULL) ?
                   ENETPHY_LINK_CAP_FD1000 :
                   ENETPHY_LINK_CAP_HD1000;
            break;

        default:
            break;
    }

    return caps;
}

static uint32_t EnetPhy_findBestCap(uint32_t caps)
{
    uint32_t i;

    for (i = ENETPHY_LINK_CAP_FD1000; i >= ENETPHY_LINK_CAP_HD10; i >>= 1U)
    {
        if ((caps & i) != 0U)
        {
            break;
        }
    }

    return i;
}

static void EnetPhy_capToMode(uint32_t caps,
                              EnetPhy_Speed *speed,
                              EnetPhy_Duplexity *duplexity)
{
    if (caps & ENETPHY_LINK_CAP_FD1000)
    {
        *speed  = ENETPHY_SPEED_1GBIT;
        *duplexity = ENETPHY_DUPLEX_FULL;
    }
    else if (caps & ENETPHY_LINK_CAP_HD1000)
    {
        *speed  = ENETPHY_SPEED_1GBIT;
        *duplexity = ENETPHY_DUPLEX_HALF;
    }
    else if (caps & ENETPHY_LINK_CAP_FD100)
    {
        *speed  = ENETPHY_SPEED_100MBIT;
        *duplexity = ENETPHY_DUPLEX_FULL;
    }
    else if (caps & ENETPHY_LINK_CAP_HD100)
    {
        *speed  = ENETPHY_SPEED_100MBIT;
        *duplexity = ENETPHY_DUPLEX_HALF;
    }
    else if (caps & ENETPHY_LINK_CAP_FD10)
    {
        *speed  = ENETPHY_SPEED_10MBIT;
        *duplexity = ENETPHY_DUPLEX_FULL;
    }
    else
    {
        *speed  = ENETPHY_SPEED_10MBIT;
        *duplexity = ENETPHY_DUPLEX_HALF;
    }
}

static void EnetPhy_showLinkPartnerCompat(EnetPhy_Handle hPhy,
                                          EnetPhy_Speed speed,
                                          EnetPhy_Duplexity duplexity)
{
    switch (speed)
    {
        case ENETPHY_SPEED_10MBIT:
        case ENETPHY_SPEED_100MBIT:
            if (duplexity == ENETPHY_DUPLEX_HALF)
            {
                ENETTRACE_DBG("PHY %u: recommended link partner config: "
                              "manual mode at %s or auto-negotiation",
                              hPhy->addr, EnetPhy_getModeString(speed, duplexity));
            }
            else
            {
                ENETTRACE_DBG("PHY %u: recommended link partner config: "
                              "manual mode at %s",
                              hPhy->addr, EnetPhy_getModeString(speed, duplexity));
            }

            break;

        case ENETPHY_SPEED_1GBIT:
            ENETTRACE_DBG("PHY %u: recommended link partner config: "
                          "manual mode at %s",
                          hPhy->addr, EnetPhy_getModeString(speed, duplexity));
            break;

        default:
            break;
    }
}
