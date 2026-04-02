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
 * \file  dp83867.c
 *
 * \brief This file contains the implementation of the DP3867 PHY.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x503

#include <stdint.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/priv/core/enet_trace_priv.h>
#include <ti/drv/enet/include/phy/enetphy.h>
#include <ti/drv/enet/include/phy/dp83867.h>
#include "enetphy_priv.h"
#include "generic_phy.h"
#include "dp83867_priv.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define DP83867_OUI                           (0x080028U)
#define DP83867_MODEL                         (0x23U)
#define DP83867_REV                           (0x01U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static bool Dp83867_isPhyDevSupported(EnetPhy_Handle hPhy,
                                      const EnetPhy_Version *version);

static bool Dp83867_isMacModeSupported(EnetPhy_Handle hPhy,
                                       EnetPhy_Mii mii);

static int32_t Dp83867_config(EnetPhy_Handle hPhy,
                              const EnetPhy_Cfg *cfg,
                              EnetPhy_Mii mii);

static void Dp83867_setMiiMode(EnetPhy_Handle hPhy,
                               EnetPhy_Mii mii);

static void Dp83867_setVtmIdleThresh(EnetPhy_Handle hPhy,
                                    uint32_t idleThresh);

static void Dp83867_setDspFFE(EnetPhy_Handle hPhy);

static void Dp83867_fixFldStrap(EnetPhy_Handle hPhy);

static void Dp83867_setLoopbackCfg(EnetPhy_Handle hPhy,
                                   bool enable);

static void Dp83867_enableAutoMdix(EnetPhy_Handle hPhy,
                                   bool enable);

static void Dp83867_setClkShift(EnetPhy_Handle hPhy,
                                bool txShiftEn,
                                bool rxShiftEn);

static int32_t Dp83867_setTxFifoDepth(EnetPhy_Handle hPhy,
                                      uint8_t depth);

static int32_t Dp83867_setClkDelay(EnetPhy_Handle hPhy,
                                   uint32_t txDelay,
                                   uint32_t rxDelay);

static int32_t Dp83867_setOutputImpedance(EnetPhy_Handle hPhy,
                                          uint32_t impedance);

static void Dp83867_setGpioMux(EnetPhy_Handle hPhy,
                               Dp83867_Gpio0Mode gpio0Mode,
                               Dp83867_Gpio1Mode gpio1Mode);

static void Dp83867_setLedMode(EnetPhy_Handle hPhy,
                               const Dp83867_LedMode *ledMode);

static void Dp83867_restart(EnetPhy_Handle hPhy);

static void Dp83867_reset(EnetPhy_Handle hPhy);

static bool Dp83867_isResetComplete(EnetPhy_Handle hPhy);

static void Dp83867_rmwExtReg(EnetPhy_Handle hPhy,
                              uint32_t reg,
                              uint16_t mask,
                              uint16_t val);

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
static void Dp83867_printRegs(EnetPhy_Handle hPhy);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

EnetPhy_Drv gEnetPhyDrvDp83867 =
{
    .name               = "dp83867",
    .isPhyDevSupported  = Dp83867_isPhyDevSupported,
    .isMacModeSupported = Dp83867_isMacModeSupported,
    .config             = Dp83867_config,
    .reset              = Dp83867_reset,
    .isResetComplete    = Dp83867_isResetComplete,
    .readExtReg         = GenericPhy_readExtReg,
    .writeExtReg        = GenericPhy_writeExtReg,
    .isPhyLinked        = GenericPhy_isPhyLinked,
#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
    .printRegs          = Dp83867_printRegs,
#endif
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Dp83867_initCfg(Dp83867_Cfg *cfg)
{
    cfg->txClkShiftEn         = BFALSE;
    cfg->rxClkShiftEn         = BFALSE;
    cfg->txDelayInPs          = 2000U;  /* 2.00 ns */
    cfg->rxDelayInPs          = 2000U;  /* 2.00 ns */
    cfg->txFifoDepth          = 4U;     /* 4 bytes/nibbles */
    cfg->impedanceInMilliOhms = 50000U; /* 50 ohms */
    cfg->idleCntThresh        = 5U;
    cfg->gpio0Mode            = DP83867_GPIO0_RXERR;
    cfg->gpio1Mode            = DP83867_GPIO1_COL;
    cfg->ledMode[0]           = DP83867_LED_LINKED;
    cfg->ledMode[1]           = DP83867_LED_LINKED_1000BT;
    cfg->ledMode[2]           = DP83867_LED_RXTXACT;
    cfg->ledMode[3]           = DP83867_LED_LINKED_100BTX;
}

static bool Dp83867_isPhyDevSupported(EnetPhy_Handle hPhy,
                                      const EnetPhy_Version *version)
{
    bool supported = BFALSE;

    if ((version->oui == DP83867_OUI) &&
        (version->model == DP83867_MODEL) &&
        (version->revision == DP83867_REV))
    {
        supported = BTRUE;
    }

    return supported;
}

static bool Dp83867_isMacModeSupported(EnetPhy_Handle hPhy,
                                       EnetPhy_Mii mii)
{
    bool supported;

    switch (mii)
    {
        case ENETPHY_MAC_MII_MII:
        case ENETPHY_MAC_MII_GMII:
        case ENETPHY_MAC_MII_RGMII:
            supported = BTRUE;
            break;

        default:
            supported = BFALSE;
            break;
    }

    return supported;
}

static int32_t Dp83867_config(EnetPhy_Handle hPhy,
                              const EnetPhy_Cfg *cfg,
                              EnetPhy_Mii mii)
{
    const Dp83867_Cfg *extendedCfg = (const Dp83867_Cfg *)cfg->extendedCfg;
    uint32_t extendedCfgSize = cfg->extendedCfgSize;
    bool enableAutoMdix = BTRUE;
    int32_t status = ENETPHY_SOK;

    if ((extendedCfg == NULL) ||
        (extendedCfgSize != sizeof(*extendedCfg)))
    {
        status = ENETPHY_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "PHY %u: invalid config params (cfg=%p, size=%u)",
                      hPhy->addr, extendedCfg, extendedCfgSize);
    }

    /* Set Viterbi detector idle count threshold and DSP FFE Equalizer */
    if (status == ENETPHY_SOK)
    {
        Dp83867_setVtmIdleThresh(hPhy, extendedCfg->idleCntThresh);
        Dp83867_setDspFFE(hPhy);
    }

    /* Apply workaround for FLD threshold when using bootstrap */
    if (status == ENETPHY_SOK)
    {
        Dp83867_fixFldStrap(hPhy);
    }

    /* Set loopback configuration: enable or disable */
    if (status == ENETPHY_SOK)
    {
        /* To maintain the desired operating mode, Auto-Negotiation should be
         * disabled before enabling the near-end loopback mode (MII loopback is
         * the only mode supported by the driver).  That's taken care by the
         * PHY framework when setting PHY's manual mode parameters */
        Dp83867_setLoopbackCfg(hPhy, cfg->loopbackEn);

        /* Auto-MDIX should be disabled before selecting the Near-End Loopback
         * mode. MDI or MDIX configuration should be manually configured */
        enableAutoMdix = !cfg->loopbackEn;
    }

    /* Software restart is required after Viterbi idle detector and loopback
     * configuration change */
    if (status == ENETPHY_SOK)
    {
        Dp83867_restart(hPhy);
    }

    /* Enable Auto-MDIX and Robust Auto-MDIX */
    if (status == ENETPHY_SOK)
    {
        Dp83867_enableAutoMdix(hPhy, enableAutoMdix);
    }

    /* Set MII mode: MII or RGMII */
    if (status == ENETPHY_SOK)
    {
        Dp83867_setMiiMode(hPhy, mii);
    }

    /* Config RGMII TX and RX clock shift and clock delay */
    if ((status == ENETPHY_SOK) &&
        (mii == ENETPHY_MAC_MII_RGMII))
    {
        Dp83867_setClkShift(hPhy,
                            extendedCfg->txClkShiftEn,
                            extendedCfg->rxClkShiftEn);

        status = Dp83867_setClkDelay(hPhy,
                                     extendedCfg->txDelayInPs,
                                     extendedCfg->rxDelayInPs);
    }

    /* Config TX FIFO depth */
    if (status == ENETPHY_SOK)
    {
        status = Dp83867_setTxFifoDepth(hPhy, extendedCfg->txFifoDepth);
    }

    /* Set output impedance */
    if (status == ENETPHY_SOK)
    {
        status = Dp83867_setOutputImpedance(hPhy, extendedCfg->impedanceInMilliOhms);
    }

    /* Set GPIO mux control (RGZ devices only) */
    if (status == ENETPHY_SOK)
    {
        Dp83867_setGpioMux(hPhy,
                           extendedCfg->gpio0Mode,
                           extendedCfg->gpio1Mode);
    }

    /* Set LED configuration */
    if (status == ENETPHY_SOK)
    {
        Dp83867_setLedMode(hPhy, extendedCfg->ledMode);
    }

    return status;
}

static void Dp83867_setMiiMode(EnetPhy_Handle hPhy,
                               EnetPhy_Mii mii)
{
    uint16_t val = 0U;

    ENETTRACE_DBG("PHY %u: MII mode: %u", hPhy->addr, mii);

    if (mii == ENETPHY_MAC_MII_RGMII)
    {
        val = RGMIICTL_RGMIIEN;
    }

    Dp83867_rmwExtReg(hPhy, DP83867_RGMIICTL, val, RGMIICTL_RGMIIEN);
}

static void Dp83867_setVtmIdleThresh(EnetPhy_Handle hPhy,
                                     uint32_t idleThresh)
{
    ENETTRACE_DBG("PHY %u: Viterbi detector idle count thresh: %u", hPhy->addr, idleThresh);

    Dp83867_rmwExtReg(hPhy, DP83867_VTMCFG, VTMCFG_IDLETHR_MASK, idleThresh);
}

static void Dp83867_setDspFFE(EnetPhy_Handle hPhy)
{
    ENETTRACE_DBG("PHY %u: DSP FFE Equalizer: %u", hPhy->addr, DSPFFECFG_FFEEQ_SHORTCABLE);

    /* As per datasheet, it improves Short Cable performance and will not effect
     * Long Cable performance */
    Dp83867_rmwExtReg(hPhy, DP83867_DSPFFECFG,
                      DSPFFECFG_FFEEQ_MASK,
                      DSPFFECFG_FFEEQ_SHORTCABLE);
}

static void Dp83867_fixFldStrap(EnetPhy_Handle hPhy)
{
    uint16_t val;

    /* As per datasheet, when using strap to enable FLD feature, this bit defaults to 0x2.
     * Register write is needed to change it to 0x1 */
    EnetPhy_readExtReg(hPhy, DP83867_STRAPSTS2, &val);
    if ((val & STRAPSTS2_FLD_MASK) != 0U)
    {
        ENETTRACE_DBG("PHY %u: Apply FLD threshold workaround", hPhy->addr);

        Dp83867_rmwExtReg(hPhy, DP83867_FLDTHRCFG, FLDTHRCFG_FLDTHR_MASK, 1U);
    }
}

static void Dp83867_setLoopbackCfg(EnetPhy_Handle hPhy,
                                   bool enable)
{
    uint16_t val;

    ENETTRACE_DBG("PHY %u: %s loopback", hPhy->addr, enable ? "enable" : "disable");

    if (enable)
    {
        val = LOOPCR_CFG_LOOPBACK;
    }
    else
    {
        val = LOOPCR_CFG_NORMAL;
    }

    /* Specific predefined loopback configuration values are required for
     * normal mode or loopback mode */
    GenericPhy_writeExtReg(hPhy, DP83867_LOOPCR, val);
}

static void Dp83867_enableAutoMdix(EnetPhy_Handle hPhy,
                                   bool enable)
{
    uint16_t val;

    if (enable)
    {
        val = PHYCR_MDICROSSOVER_AUTO;
    }
    else
    {
        val = PHYCR_MDICROSSOVER_MDI;
    }

    ENETTRACE_DBG("PHY %u: %s automatic cross-over",
                  hPhy->addr, enable ? "enable" : "disable");
    EnetPhy_rmwReg(hPhy, DP83867_PHYCR,
                   PHYCR_MDICROSSOVER_MASK,
                   val);

    if (enable)
    {
        ENETTRACE_DBG("PHY %u: enable Robust Auto-MDIX", hPhy->addr);
        EnetPhy_rmwReg(hPhy, DP83867_CFG3,
                       CFG3_ROBUSTAUTOMDIX,
                       CFG3_ROBUSTAUTOMDIX);
    }
}

static void Dp83867_setClkShift(EnetPhy_Handle hPhy,
                                bool txShiftEn,
                                bool rxShiftEn)
{
    uint16_t val;

    ENETTRACE_DBG("PHY %u: clock shift TX:%s RX:%s",
                  hPhy->addr,
                  txShiftEn ? "enable" : "disable",
                  rxShiftEn ? "enable" : "disable");

    val  = (txShiftEn == BTRUE) ? RGMIICTL_TXCLKDLY : 0U;
    val |= (rxShiftEn == BTRUE) ? RGMIICTL_RXCLKDLY : 0U;
    Dp83867_rmwExtReg(hPhy, DP83867_RGMIICTL,
                      RGMIICTL_TXCLKDLY | RGMIICTL_RXCLKDLY,
                      val);
}

static int32_t Dp83867_setTxFifoDepth(EnetPhy_Handle hPhy,
                                      uint8_t depth)
{
    uint16_t val = 0U;
    int32_t status = ENETPHY_SOK;

    switch (depth)
    {
        case 3U:
            val = PHYCR_TXFIFODEPTH_3B;
            break;

        case 4U:
            val = PHYCR_TXFIFODEPTH_4B;
            break;

        case 6U:
            val = PHYCR_TXFIFODEPTH_6B;
            break;

        case 8U:
            val = PHYCR_TXFIFODEPTH_8B;
            break;

        default:
            status = ENETPHY_EINVALIDPARAMS;
            break;
    }

    if (status == ENETPHY_SOK)
    {
        ENETTRACE_DBG("PHY %u: set FIFO depth %u", hPhy->addr, depth);
        EnetPhy_rmwReg(hPhy, DP83867_PHYCR, PHYCR_TXFIFODEPTH_MASK, val);
    }
    else
    {
        ENETTRACE_ERR(status, "PHY %u: invalid FIFO depth %u", hPhy->addr, depth);
    }

    return status;
}

static int32_t Dp83867_setClkDelay(EnetPhy_Handle hPhy,
                                   uint32_t txDelay,
                                   uint32_t rxDelay)
{
    uint16_t val;
    uint32_t delay;
    uint32_t delayCtrl;
    int32_t status = ENETPHY_SOK;

    if ((txDelay <= RGMIIDCTL_DELAY_MAX) &&
        (rxDelay <= RGMIIDCTL_DELAY_MAX))
    {
        ENETTRACE_DBG("PHY %u: set delay %u ps TX, %u ps RX", hPhy->addr, txDelay, rxDelay);

        /* Avoids wrong value of delayCtrl if txDelay is 0 */
        delay     = (txDelay > 0U) ? txDelay : 1U;
        delayCtrl = ENETPHY_DIV_ROUNDUP(delay, RGMIIDCTL_DELAY_STEP) - 1U;
        val       = (uint16_t)((delayCtrl << RGMIIDCTL_TXDLYCTRL_OFFSET) & RGMIIDCTL_TXDLYCTRL_MASK);

        /* Avoids wrong value of delayCtrl if rxDelay is 0 */
        delay     = (rxDelay > 0U) ? rxDelay : 1U;
        delayCtrl = ENETPHY_DIV_ROUNDUP(delay, RGMIIDCTL_DELAY_STEP) - 1U;
        val      |= (uint16_t)((delayCtrl << RGMIIDCTL_RXDLYCTRL_OFFSET) & RGMIIDCTL_RXDLYCTRL_MASK);

        GenericPhy_writeExtReg(hPhy, DP83867_RGMIIDCTL, val);
    }
    else
    {
        status = ENETPHY_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "PHY %u: invalid delay (TX=%u RX=%u)", hPhy->addr, txDelay, rxDelay);
    }

    return status;
}

static int32_t Dp83867_setOutputImpedance(EnetPhy_Handle hPhy,
                                          uint32_t impedance)
{
    int32_t status = ENETPHY_SOK;
    uint32_t val;

    if ((impedance >= IOMUXCFG_IOIMPEDANCE_MIN) &&
        (impedance <= IOMUXCFG_IOIMPEDANCE_MAX))
    {
        ENETTRACE_DBG("PHY %u: set output impedance to %u milli-ohms", hPhy->addr, impedance);

        val = (IOMUXCFG_IOIMPEDANCE_MAX - impedance) * IOMUXCFG_IOIMPEDANCE_MASK;
        val = (val + IOMUXCFG_IOIMPEDANCE_RANGE / 2) / IOMUXCFG_IOIMPEDANCE_RANGE;

        Dp83867_rmwExtReg(hPhy, DP83867_IOMUXCFG,
                          IOMUXCFG_IOIMPEDANCE_MASK,
                          val);
    }
    else
    {
        status = ENETPHY_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "PHY %u: out-of-range impedance %u", hPhy->addr, impedance);
    }

    return status;
}

static void Dp83867_setGpioMux(EnetPhy_Handle hPhy,
                               Dp83867_Gpio0Mode gpio0Mode,
                               Dp83867_Gpio1Mode gpio1Mode)
{
    int16_t gpio0;
    int16_t gpio1;

    ENETTRACE_DBG("PHY %u: set gpio0 = mode%u, gpio1 = mode%u", hPhy->addr, gpio0Mode, gpio1Mode);

    gpio0  = (uint16_t)gpio0Mode << GPIOMUXCTRL_GPIO0_OFFSET;
    gpio0 &= GPIOMUXCTRL_GPIO0_MASK;

    gpio1  = (uint16_t)gpio1Mode << GPIOMUXCTRL_GPIO1_OFFSET;
    gpio1 &= GPIOMUXCTRL_GPIO1_MASK;

    Dp83867_rmwExtReg(hPhy, DP83867_GPIOMUXCTRL,
                      GPIOMUXCTRL_GPIO1_MASK | GPIOMUXCTRL_GPIO0_MASK,
                      gpio1 | gpio0);
}

static void Dp83867_setLedMode(EnetPhy_Handle hPhy,
                               const Dp83867_LedMode *ledMode)
{
    uint16_t val;

    ENETTRACE_DBG("PHY %u: set LED0 = mode%u, LED1 = mode%u, LED2 = mode%u, LED3 = mode%u",
                  hPhy->addr, ledMode[0], ledMode[1], ledMode[2], ledMode[3]);

    val  = ((uint16_t)ledMode[0] << LEDCR1_LED0SEL_OFFSET) & LEDCR1_LED0SEL_MASK;
    val |= ((uint16_t)ledMode[1] << LEDCR1_LED1SEL_OFFSET) & LEDCR1_LED1SEL_MASK;
    val |= ((uint16_t)ledMode[2] << LEDCR1_LED2SEL_OFFSET) & LEDCR1_LED2SEL_MASK;
    val |= ((uint16_t)ledMode[3] << LEDCR1_LED3SEL_OFFSET) & LEDCR1_LED3SEL_MASK;

    EnetPhy_writeReg(hPhy, DP83867_LEDCR1, val);
}

static void Dp83867_restart(EnetPhy_Handle hPhy)
{
    /* Software restart: full reset, not including registers */
    ENETTRACE_DBG("PHY %u: soft-restart", hPhy->addr);
    EnetPhy_rmwReg(hPhy, DP83867_CTRL, CTRL_SWRESTART, CTRL_SWRESTART);
}

static void Dp83867_reset(EnetPhy_Handle hPhy)
{
    /* Global software reset: all PHY internal circuits including IEEE-defined
     * registers and all extended registers are reset */
    ENETTRACE_DBG("PHY %u: global soft-reset", hPhy->addr);
    EnetPhy_rmwReg(hPhy, DP83867_CTRL, CTRL_SWRESET, CTRL_SWRESET);
}

static bool Dp83867_isResetComplete(EnetPhy_Handle hPhy)
{
    int32_t status;
    uint16_t val;
    bool complete = BFALSE;

    /* Reset is complete when RESET bit has self-cleared */
    status = EnetPhy_readReg(hPhy, DP83867_CTRL, &val);
    if (status == ENETPHY_SOK)
    {
        complete = ((val & CTRL_SWRESET) == 0U);
    }

    ENETTRACE_DBG("PHY %u: global soft-reset is %scomplete", hPhy->addr, complete ? "" : "not");

    return complete;
}

static void Dp83867_rmwExtReg(EnetPhy_Handle hPhy,
                              uint32_t reg,
                              uint16_t mask,
                              uint16_t val)
{
    uint16_t devad = MMD_CR_DEVADDR;
    uint16_t data;
    int32_t status;

    ENETTRACE_VERBOSE("PHY %u: write reg %u mask 0x%04x val 0x%04x",
                      hPhy->addr, reg, mask, val);

    EnetPhy_writeReg(hPhy, PHY_MMD_CR, devad | MMD_CR_ADDR);
    EnetPhy_writeReg(hPhy, PHY_MMD_DR, reg);
    EnetPhy_writeReg(hPhy, PHY_MMD_CR, devad | MMD_CR_DATA_NOPOSTINC);
    status = EnetPhy_readReg(hPhy, PHY_MMD_DR, &data);

    if (status == ENETPHY_SOK)
    {
        data = (data & ~mask) | (val & mask);
        EnetPhy_writeReg(hPhy, PHY_MMD_CR, devad | MMD_CR_DATA_NOPOSTINC);
        EnetPhy_writeReg(hPhy, PHY_MMD_DR, data);
    }
}

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
static void Dp83867_printRegs(EnetPhy_Handle hPhy)
{
    uint32_t phyAddr = hPhy->addr;
    uint16_t val;

    EnetPhy_readReg(hPhy, PHY_BMCR, &val);
    ENETTRACE_INFO("PHY %u: BMCR        = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_BMSR, &val);
    ENETTRACE_INFO("PHY %u: BMSR        = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_PHYIDR1, &val);
    ENETTRACE_INFO("PHY %u: PHYIDR1     = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_PHYIDR2, &val);
    ENETTRACE_INFO("PHY %u: PHYIDR2     = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_ANAR, &val);
    ENETTRACE_INFO("PHY %u: ANAR        = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_ANLPAR, &val);
    ENETTRACE_INFO("PHY %u: ANLPAR      = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_ANER, &val);
    ENETTRACE_INFO("PHY %u: ANER        = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_ANNPTR, &val);
    ENETTRACE_INFO("PHY %u: ANNPTR      = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_ANNPRR, &val);
    ENETTRACE_INFO("PHY %u: ANNPRR      = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_GIGCR, &val);
    ENETTRACE_INFO("PHY %u: CFG1        = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_GIGSR, &val);
    ENETTRACE_INFO("PHY %u: STS1        = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_GIGESR, &val);
    ENETTRACE_INFO("PHY %u: 1KSCR       = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_PHYCR, &val);
    ENETTRACE_INFO("PHY %u: PHYCR       = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_PHYSTS, &val);
    ENETTRACE_INFO("PHY %u: PHYSTS      = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_MICR, &val);
    ENETTRACE_INFO("PHY %u: MICR        = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_ISR, &val);
    ENETTRACE_INFO("PHY %u: ISR         = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_CFG2, &val);
    ENETTRACE_INFO("PHY %u: CFG2        = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_RECR, &val);
    ENETTRACE_INFO("PHY %u: RECR        = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_BISCR, &val);
    ENETTRACE_INFO("PHY %u: BISCR       = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_STS2, &val);
    ENETTRACE_INFO("PHY %u: STS2        = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_LEDCR1, &val);
    ENETTRACE_INFO("PHY %u: LEDCR1      = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_LEDCR2, &val);
    ENETTRACE_INFO("PHY %u: LEDCR2      = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_LEDCR3, &val);
    ENETTRACE_INFO("PHY %u: LEDCR3      = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_CFG3, &val);
    ENETTRACE_INFO("PHY %u: CFG3        = 0x%04x", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83867_CTRL, &val);
    ENETTRACE_INFO("PHY %u: CTRL        = 0x%04x", phyAddr, val);
    EnetPhy_readExtReg(hPhy, DP83867_RGMIICTL, &val);
    ENETTRACE_INFO("PHY %u: RGMIICTL    = 0x%04x", phyAddr, val);
    EnetPhy_readExtReg(hPhy, DP83867_FLDTHRCFG, &val);
    ENETTRACE_INFO("PHY %u: FLDTHRCFG   = 0x%04x", phyAddr, val);
    EnetPhy_readExtReg(hPhy, DP83867_VTMCFG, &val);
    ENETTRACE_INFO("PHY %u: VTMCFG      = 0x%04x", phyAddr, val);
    EnetPhy_readExtReg(hPhy, DP83867_STRAPSTS2, &val);
    ENETTRACE_INFO("PHY %u: STRAPSTS2   = 0x%04x", phyAddr, val);
    EnetPhy_readExtReg(hPhy, DP83867_RGMIIDCTL, &val);
    ENETTRACE_INFO("PHY %u: RGMIIDCTL   = 0x%04x", phyAddr, val);
    EnetPhy_readExtReg(hPhy, DP83867_LOOPCR, &val);
    ENETTRACE_INFO("PHY %u: LOOPCR      = 0x%04x", phyAddr, val);
    EnetPhy_readExtReg(hPhy, DP83867_DSPFFECFG, &val);
    ENETTRACE_INFO("PHY %u: DSPFFECFG   = 0x%04x", phyAddr, val);
    EnetPhy_readExtReg(hPhy, DP83867_IOMUXCFG, &val);
    ENETTRACE_INFO("PHY %u: IOMUXCFG    = 0x%04x", phyAddr, val);
    EnetPhy_readExtReg(hPhy, DP83867_GPIOMUXCTRL, &val);
    ENETTRACE_INFO("PHY %u: GPIOMUXCTRL = 0x%04x", phyAddr, val);
}
#endif
