/******************************************************************************
 *
 * Copyright (C) 2012-2024 Cadence Design Systems, Inc.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 *
 * dp_sd0801_spec.c
 *
 ******************************************************************************
 */

/* parasoft-begin-suppress METRICS-36-3 "A function should not be called from more than 5 different functions",  DRV-3823 */

#include "dp_sd0801_if.h"
#include "dp_sd0801_priv.h"
#include "dp_sd0801_sanity.h"
#include "dp_regs.h"

#include "dp_sd0801_cfg.h"

#include "cdn_log.h"

#include "dp_sd0801_internal.h"
#include "dp_sd0801_spec.h"

typedef struct phyRegValue_t {
    uint32_t addr;
    uint16_t val;
} phyRegValue;

typedef enum {
    POWERSTATE_A0 = 0U,
    /* Powerstate A1 is unused */
    POWERSTATE_A2 = 2U,
    POWERSTATE_A3 = 3U,
} phyPowerstate;

#define PID_TYPE_SD (uint16_t)0x7364 /* ASCII for "SD", SerDes */
#define PID_NUM_0801 (uint16_t)0x0801

static void mlSetLinkRate(DP_SD0801_PrivateData* pD, const DP_SD0801_LinkState* linkState);
bool isPhySupported(const DP_SD0801_PrivateData* pD);

/**
 * Return true for supported device, false for unsupported one)
 */
bool isPhySupported(const DP_SD0801_PrivateData* pD)
{
    bool retVal = true;
    const uint16_t pidType = afeRead(pD, CMN_PID_TYPE);
    const uint16_t pidNum = afeRead(pD, CMN_PID_NUM);

    if ((pidType != PID_TYPE_SD) || (pidNum != PID_NUM_0801))
    {
        retVal = false;
    }

    return retVal;
}

/**
 * Used to convert number of lanes to bit-field indicator
 */
static uint8_t getLaneCfg(uint8_t mLane, uint8_t laneCount) {

    uint8_t laneCfg = 0U;

    if ((laneCount <= DP_SD0801_MAX_LANE_COUNT) && (laneCount > 0U)) {
        laneCfg = (uint8_t)((1U << (laneCount)) - 1U);
    }

    /* laneCfg for non-zero mLane */
    if (mLane < DP_SD0801_MAX_LANE_COUNT) {
        laneCfg = laneCfg << mLane;
    }
    return laneCfg;
}

/** Returns base address of common PLL register space */
static inline uint32_t getPllCmnM0Base(DP_SD0801_Pll pll) {
    uint32_t pllBase = (pll == DP_SD0801_PLL_0) ? CMN_PLL0_MODE0_BASE : CMN_PLL1_MODE0_BASE;
    return pllBase;
}

/** Returns base address of diagnostic PLL register space */
static inline uint32_t getPllPdiagM0Base(DP_SD0801_Pll pll) {
    uint32_t pllBase = (pll == DP_SD0801_PLL_0) ? CMN_PLL0_PDIAG_MODE0_BASE : CMN_PLL1_PDIAG_MODE0_BASE;
    return pllBase;
}

static void configurePhyPmaPllPdiagCmnCfg(const DP_SD0801_PrivateData* pD, DP_SD0801_Pll pll) {

    /* PLL diagnostic regs */
    static const phyRegValue PhyPmaPllPdiagCmnCfg[] = {
        {.addr = CMN_PDIAG_PLL_CP_PADJ_M0_OFFSET, .val = CMN_PDIAG_PLL_CP_PADJ_M0_VAL},
        {.addr = CMN_PDIAG_PLL_CP_IADJ_M0_OFFSET, .val = CMN_PDIAG_PLL_CP_IADJ_M0_VAL},
        {.addr = CMN_PDIAG_PLL_FILT_PADJ_M0_OFFSET, .val = CMN_PDIAG_PLL_FILT_PADJ_M0_VAL}
    };

    uint32_t i, regCount;

    uint32_t pllPdiagBase = getPllPdiagM0Base(pll);

    /* Write common values to PLL diag registers, for both (PLL 0/1) */
    regCount = (uint32_t)sizeof(PhyPmaPllPdiagCmnCfg) / (uint32_t)sizeof(phyRegValue);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllPdiagBase + PhyPmaPllPdiagCmnCfg[i].addr), PhyPmaPllPdiagCmnCfg[i].val);
    }
}

static void configurePhyPmaPllCmnCfg(const DP_SD0801_PrivateData* pD, DP_SD0801_Pll pll) {

    /* PLL settings for all VCO rates */
    static const phyRegValue PhyPmaPllCmnCfg[] = {
        {.addr = CMN_PLL_DSM_DIAG_M0_OFFSET, .val = CMN_PLL_DSM_DIAG_M0_VAL},
        {.addr = CMN_PLL_VCOCAL_INIT_TMR_OFFSET, .val = CMN_PLL_VCOCAL_INIT_TMR_VAL},
        {.addr = CMN_PLL_VCOCAL_ITER_TMR_OFFSET, .val = CMN_PLL_VCOCAL_ITER_TMR_VAL},
        {.addr = CMN_PLL_VCOCAL_REFTIM_START_OFFSET, .val = CMN_PLL_VCOCAL_REFTIM_START_VAL},
        {.addr = CMN_PLL_VCOCAL_TCTRL_OFFSET, .val = CMN_PLL_VCOCAL_TCTRL_VAL}
    };

    uint32_t i, regCount;

    uint32_t pllBase = getPllCmnM0Base(pll);

    /* Write common values to PLL register, for both (PLL 0/1) */
    regCount = (uint32_t)sizeof(PhyPmaPllCmnCfg) / (uint32_t)sizeof(phyRegValue);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllBase + PhyPmaPllCmnCfg[i].addr), PhyPmaPllCmnCfg[i].val);
    }

    configurePhyPmaPllPdiagCmnCfg(pD, pll);
}

/* parasoft-begin-suppress METRICS-39-3 "The value of VOCF metric for a function should not be higher than 4, DRV-3852" */
static void configurePhyPmaCmnCfg(const DP_SD0801_PrivateData* pD)
{
    /* Values of registers to write are taken from sd0801 PHY programming guide. */
    static const phyRegValue PhyPmaCmnCfg[] = {
        /* Startup state machine registers */
        {.addr = CMN_SSM_BIAS_TMR, .val = CMN_SSM_BIAS_TMR_VAL},
        /* PLL 0 control state machine registers */
        {.addr = CMN_PLLSM0_PLLPRE_TMR, .val = CMN_PLLSM0_PLLPRE_TMR_VAL},
        {.addr = CMN_PLLSM0_PLLLOCK_TMR, .val = CMN_PLLSM0_PLLLOCK_TMR_VAL},
        /* PLL 1 control state machine registers */
        {.addr = CMN_PLLSM1_PLLPRE_TMR, .val = CMN_PLLSM1_PLLPRE_TMR_VAL},
        {.addr = CMN_PLLSM1_PLLLOCK_TMR, .val = CMN_PLLSM1_PLLLOCK_TMR_VAL},
        /* Bandgap calibration registers */
        {.addr = CMN_BGCAL_INIT_TMR, .val = CMN_BGCAL_INIT_TMR_VAL},
        {.addr = CMN_BGCAL_ITER_TMR, .val = CMN_BGCAL_ITER_TMR_VAL},
        /* External bias current calibration registers */
        {.addr = CMN_IBCAL_INIT_TMR, .val = CMN_IBCAL_INIT_TMR_VAL},
        /* Resistor calibration registers */
        {.addr = CMN_TXPUCAL_INIT_TMR, .val = CMN_TXPUCAL_INIT_TMR_VAL},
        {.addr = CMN_TXPUCAL_ITER_TMR, .val = CMN_TXPUCAL_ITER_TMR_VAL},
        {.addr = CMN_TXPDCAL_INIT_TMR, .val = CMN_TXPDCAL_INIT_TMR_VAL},
        {.addr = CMN_TXPDCAL_ITER_TMR, .val = CMN_TXPDCAL_ITER_TMR_VAL},
        {.addr = CMN_RXCAL_INIT_TMR, .val = CMN_RXCAL_INIT_TMR_VAL},
        {.addr = CMN_RXCAL_ITER_TMR, .val = CMN_RXCAL_ITER_TMR_VAL},
        /* Signal detect clock calibration registers */
        {.addr = CMN_SD_CAL_INIT_TMR, .val = CMN_SD_CAL_INIT_TMR_VAL},
        {.addr = CMN_SD_CAL_ITER_TMR, .val = CMN_SD_CAL_ITER_TMR_VAL},
        {.addr = CMN_SD_CAL_REFTIM_START, .val = CMN_SD_CAL_REFTIM_START_VAL},
        {.addr = CMN_SD_CAL_PLLCNT_START, .val = CMN_SD_CAL_PLLCNT_START_VAL}
    };

    uint32_t i, regCount;
    regCount = (uint32_t)sizeof(PhyPmaCmnCfg) / (uint32_t)sizeof(phyRegValue);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, PhyPmaCmnCfg[i].addr, PhyPmaCmnCfg[i].val);
    }

}
/* parasoft-end-suppress METRICS-39-3 */

/**
 * This driver supports only DPTX.
 */
static void disableRx(const DP_SD0801_PrivateData* pD, uint32_t laneOffset)
{
    uint32_t regsToClear[] = {
        /* Powerstate-related */
        RX_PSC_A0,
        RX_PSC_A2,
        RX_PSC_A3,
        RX_PSC_CAL,
        RX_REE_GCSM1_CTRL,
        RX_REE_GCSM2_CTRL,
        RX_REE_PERGCSM_CTRL
    };
    uint8_t i;

    for (i = 0; i < (sizeof(regsToClear) / sizeof(uint32_t)); i++)
    {
        afeWrite(pD, (uint32_t)(regsToClear[i] | laneOffset), 0x0000U);
    }
}

static void configurePhyPmaLnDpCfg(const DP_SD0801_PrivateData* pD, uint8_t lane)
{
    /* DP supports lanes 0-3 - wrap. */
    /* uint32_t used for consistency with bitwise operations. */
    const uint32_t i = 0x0003U & (uint32_t)lane;

    /* Bits 9 and 10 of address indicate lane number. */
    const uint32_t laneOffset = (i << 9);

#ifdef TX_RCVDET_ST_TMR_VAL
    const uint16_t rcvdetVal = (uint16_t)TX_RCVDET_ST_TMR_VAL;
    /* Per lane, refclock-dependent receiver detection setting. */
    afeWrite(pD, (uint32_t)(TX_RCVDET_ST_TMR | laneOffset), rcvdetVal);
#endif /* for reflock of 100 MHz, use reset value */

    /* Writing Tx/Rx Power State Controllers Registers */

    /* 2.8.3 Display Port / Embedded Display Port */
    afeWrite(pD, (uint32_t)(TX_PSC_A0 | laneOffset), TX_PSC_A0_VAL);
    afeWrite(pD, (uint32_t)(TX_PSC_A2 | laneOffset), TX_PSC_A2_VAL);
    afeWrite(pD, (uint32_t)(TX_PSC_A3 | laneOffset), TX_PSC_A3_VAL);

    disableRx(pD, laneOffset);

    afeWrite(pD, (uint32_t)(XCVR_DIAG_BIDI_CTRL | laneOffset), XCVR_DIAG_BIDI_CTRL_VAL);
    afeWrite(pD, (uint32_t)(XCVR_DIAG_PLLDRC_CTRL | laneOffset), XCVR_DIAG_PLLDRC_CTRL_VAL);
    afeWrite(pD, (uint32_t)(XCVR_DIAG_HSCLK_SEL | laneOffset), XCVR_DIAG_HSCLK_SEL_VAL);
}

static void configurePhyPmaDpCfg(const DP_SD0801_PrivateData* pD, uint8_t linkCfg)
{
    uint8_t i;
    /* PMA cmn configuration */
    configurePhyPmaCmnCfg(pD);

    /* PLL common configuration for both PLL's (1 link is used) */
    configurePhyPmaPllCmnCfg(pD, DP_SD0801_PLL_0);
    configurePhyPmaPllCmnCfg(pD, DP_SD0801_PLL_1);

    /* PMA lane configuration to deal with multi-link operation */
    for (i = 0; i < 4U; i++) {
        /* Configure lane for DP operation */
        if (0U != (linkCfg & (1U << i))) {
            configurePhyPmaLnDpCfg(pD, i);
        }
    }
}

static ENUM_VCO_FREQ getVcoFreq(DP_SD0801_LinkRate linkRate)
{
    ENUM_VCO_FREQ retVal;

    switch (linkRate)
    {
    case DP_SD0801_LINK_RATE_1_62:
        retVal = VCO_9GHz72_refclk; /* Setting VCO for 9.72GHz */
        break;
    case DP_SD0801_LINK_RATE_2_16:
        retVal = VCO_8GHz64_refclk; /* Setting VCO for 8.64GHz */
        break;
    case DP_SD0801_LINK_RATE_2_43:
        retVal = VCO_9GHz72_refclk; /* Setting VCO for 9.72GHz */
        break;
    case DP_SD0801_LINK_RATE_2_70:
        retVal = VCO_10GHz8_refclk; /* Setting VCO for 10.8GHz */
        break;
    case DP_SD0801_LINK_RATE_3_24:
        retVal = VCO_9GHz72_refclk; /* Setting VCO for 9.72GHz */
        break;
    case DP_SD0801_LINK_RATE_4_32:
        retVal = VCO_8GHz64_refclk; /* Setting VCO for 8.64GHz */
        break;
    case DP_SD0801_LINK_RATE_5_40:
        retVal = VCO_10GHz8_refclk; /* Setting VCO for 10.8GHz */
        break;
    case DP_SD0801_LINK_RATE_8_10:
        retVal = VCO_8GHz1_refclk;  /* Setting VCO for 8.1GHz */
        break;
    default:
        retVal = VCO_9GHz72_refclk;
        break;
    }
    return retVal;
}

static void enableSsc(const DP_SD0801_PrivateData* pD, DP_SD0801_Pll pll, const uint16_t ctrlVal[4])
{
    /* Ssc registers offsets */
    static const uint16_t sscRegs[] = {
        CMN_PLL_SS_CTRL1_M0_OFFSET,
        CMN_PLL_SS_CTRL2_M0_OFFSET,
        CMN_PLL_SS_CTRL3_M0_OFFSET,
        CMN_PLL_SS_CTRL4_M0_OFFSET
    };

    uint32_t i;
    uint32_t regCount = (uint32_t)sizeof(sscRegs) / (uint32_t)sizeof(uint16_t);

    /* Choose base for correct PLL */
    uint32_t pllBase = getPllCmnM0Base(pll);

    /* Configure ssc */
    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllBase + sscRegs[i]), ctrlVal[i]);
    }
}

static void configurePhyPmaCmnVcoCfg10_8(const DP_SD0801_PrivateData* pD, DP_SD0801_Pll pll, bool ssc)
{
    /* Settings for VCO equals 10.8GHz */

    static const uint16_t sscCfg10p8[] = {
        CMN_PLL_SS_CTRL1_M0_VCO_10p8_VAL,
        CMN_PLL_SS_CTRL2_M0_VCO_10p8_VAL,
        CMN_PLL_SS_CTRL3_M0_VCO_10p8_VAL,
        CMN_PLL_SS_CTRL4_M0_VCO_10p8_VAL
    };

    static const phyRegValue pllCmnCfg10p8[] = {
        {.addr = CMN_PLL_INTDIV_M0_OFFSET, .val = CMN_PLL_INTDIV_M0_10p8_VAL},
        {.addr = CMN_PLL_FRACDIVL_M0_OFFSET, .val = CMN_PLL_FRACDIVL_M0_10p8_VAL},
        {.addr = CMN_PLL_FRACDIVH_M0_OFFSET, .val = CMN_PLL_FRACDIVH_M0_10p8_VAL},
        {.addr = CMN_PLL_HIGH_THR_M0_OFFSET, .val = CMN_PLL_HIGH_THR_M0_10p8_VAL},
    };

    uint32_t i;
    uint32_t regCount = (uint32_t)sizeof(pllCmnCfg10p8) / (uint32_t)sizeof(phyRegValue);

    uint32_t pllBase = getPllCmnM0Base(pll);
    uint32_t pllPdiagBase = getPllPdiagM0Base(pll);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllBase + pllCmnCfg10p8[i].addr), pllCmnCfg10p8[i].val);
    }

    /* Write value to PLL PDIAG register */
    afeWrite(pD, (pllPdiagBase + CMN_PDIAG_PLL_CTRL_M0_OFFSET), CMN_PDIAG_PLL_CTRL_M0_10p8_VAL);

    if (ssc) {
        enableSsc(pD, pll, sscCfg10p8); /* values specific for VCO. */
    }
}

static void configurePhyPmaCmnVcoCfg9_72(const DP_SD0801_PrivateData* pD, DP_SD0801_Pll pll, bool ssc)
{
    /* Settings for VCO equals 9.72GHz */

    static const uint16_t sscCfg9p72[] = {
        CMN_PLL_SS_CTRL1_M0_VCO_9p72_VAL,
        CMN_PLL_SS_CTRL2_M0_VCO_9p72_VAL,
        CMN_PLL_SS_CTRL3_M0_VCO_9p72_VAL,
        CMN_PLL_SS_CTRL4_M0_VCO_9p72_VAL
    };

    static const phyRegValue pllCmnCfg9p72[] = {
        {.addr = CMN_PLL_INTDIV_M0_OFFSET, .val = CMN_PLL_INTDIV_M0_9p72_VAL},
        {.addr = CMN_PLL_FRACDIVL_M0_OFFSET, .val = CMN_PLL_FRACDIVL_M0_9p72_VAL},
        {.addr = CMN_PLL_FRACDIVH_M0_OFFSET, .val = CMN_PLL_FRACDIVH_M0_9p72_VAL},
        {.addr = CMN_PLL_HIGH_THR_M0_OFFSET, .val = CMN_PLL_HIGH_THR_M0_9p72_VAL},
    };

    uint32_t i;
    uint32_t regCount = (uint32_t)sizeof(pllCmnCfg9p72) / (uint32_t)sizeof(phyRegValue);

    uint32_t pllBase = getPllCmnM0Base(pll);
    uint32_t pllPdiagBase = getPllPdiagM0Base(pll);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllBase + pllCmnCfg9p72[i].addr), pllCmnCfg9p72[i].val);
    }

    /* Write value to PLL PDIAG register */
    afeWrite(pD, (pllPdiagBase + CMN_PDIAG_PLL_CTRL_M0_OFFSET), CMN_PDIAG_PLL_CTRL_M0_9p72_VAL);

    if (ssc) {
        enableSsc(pD, pll, sscCfg9p72); /* values specific for VCO. */
    }

}

static void configurePhyPmaCmnVcoCfg8_64(const DP_SD0801_PrivateData* pD, DP_SD0801_Pll pll, bool ssc)
{
    static const uint16_t sscCfg8p64[] = {
        CMN_PLL_SS_CTRL1_M0_VCO_8p64_VAL,
        CMN_PLL_SS_CTRL2_M0_VCO_8p64_VAL,
        CMN_PLL_SS_CTRL3_M0_VCO_8p64_VAL,
        CMN_PLL_SS_CTRL4_M0_VCO_8p64_VAL
    };

    static const phyRegValue pllCmnCfg8p64[] = {
        {.addr = CMN_PLL_INTDIV_M0_OFFSET, .val = CMN_PLL_INTDIV_M0_8p64_VAL},
        {.addr = CMN_PLL_FRACDIVL_M0_OFFSET, .val = CMN_PLL_FRACDIVL_M0_8p64_VAL},
        {.addr = CMN_PLL_FRACDIVH_M0_OFFSET, .val = CMN_PLL_FRACDIVH_M0_8p64_VAL},
        {.addr = CMN_PLL_HIGH_THR_M0_OFFSET, .val = CMN_PLL_HIGH_THR_M0_8p64_VAL}
    };

    uint32_t i;
    uint32_t regCount = (uint32_t)sizeof(pllCmnCfg8p64) / (uint32_t)sizeof(phyRegValue);

    uint32_t pllBase = getPllCmnM0Base(pll);
    uint32_t pllPdiagBase = getPllPdiagM0Base(pll);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllBase + pllCmnCfg8p64[i].addr), pllCmnCfg8p64[i].val);
    }

    /* Write value to PLL PDIAG register */
    afeWrite(pD, (pllPdiagBase + CMN_PDIAG_PLL_CTRL_M0_OFFSET), CMN_PDIAG_PLL_CTRL_M0_8p64_VAL);

    if (ssc) {
        enableSsc(pD, pll, sscCfg8p64); /* values specific for VCO. */
    }
}

static void configurePhyPmaCmnVcoCfg8_1(const DP_SD0801_PrivateData* pD, DP_SD0801_Pll pll, bool ssc)
{
    static const uint16_t sscCfg8p1[] = {
        CMN_PLL_SS_CTRL1_M0_VCO_8p1_VAL,
        CMN_PLL_SS_CTRL2_M0_VCO_8p1_VAL,
        CMN_PLL_SS_CTRL3_M0_VCO_8p1_VAL,
        CMN_PLL_SS_CTRL4_M0_VCO_8p1_VAL
    };

    static const phyRegValue pllCmnCfg8p1[] = {
        {.addr = CMN_PLL_INTDIV_M0_OFFSET, .val = CMN_PLL_INTDIV_M0_8p1_VAL},
        {.addr = CMN_PLL_FRACDIVL_M0_OFFSET, .val = CMN_PLL_FRACDIVL_M0_8p1_VAL},
        {.addr = CMN_PLL_FRACDIVH_M0_OFFSET, .val = CMN_PLL_FRACDIVH_M0_8p1_VAL},
        {.addr = CMN_PLL_HIGH_THR_M0_OFFSET, .val = CMN_PLL_HIGH_THR_M0_8p1_VAL},
    };

    uint32_t i;
    uint32_t regCount = (uint32_t)sizeof(pllCmnCfg8p1) / (uint32_t)sizeof(phyRegValue);

    uint32_t pllBase = getPllCmnM0Base(pll);
    uint32_t pllPdiagBase = getPllPdiagM0Base(pll);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllBase + pllCmnCfg8p1[i].addr), pllCmnCfg8p1[i].val);
    }

    /* Write value to PLL PDIAG register */
    afeWrite(pD, (pllPdiagBase + CMN_PDIAG_PLL_CTRL_M0_OFFSET), CMN_PDIAG_PLL_CTRL_M0_8p1_VAL);

    if (ssc) {
        enableSsc(pD, pll, sscCfg8p1); /* values specific for VCO. */
    }

}

static void configurePhyPmaCmnVcoCommon(const DP_SD0801_PrivateData* pD, DP_SD0801_Pll pll, bool ssc)
{
    /* Values used to disable ssc */
    static const uint16_t sscCfg[] = {
        CMM_PLL_SS_CTRL1_M0_NO_SSC_VAL,
        CMM_PLL_SS_CTRL2_M0_NO_SSC_VAL,
        CMM_PLL_SS_CTRL3_M0_NO_SSC_VAL,
        CMM_PLL_SS_CTRL4_M0_NO_SSC_VAL
    };

    /* Configuration in case if ssc is not used */
    static const phyRegValue noSscPllCmnCfg[] = {
        {.addr = CMN_PLL_VCOCAL_PLLCNT_START_OFFSET, .val = CMN_PLL_VCOCAL_PLLCNT_START_NO_SSC_VAL},
        {.addr = CMN_PLL_LOCK_PLLCNT_START_OFFSET, .val = CMN_PLL_LOCK_PLLCNT_START_NO_SSC_VAL},
        {.addr = CMN_PLL_LOCK_PLLCNT_THR_OFFSET, .val = CMN_PLL_LOCK_PLLCNT_THR_NO_SSC_VAL},
    };

    /* Configuration in case if ssc is used */
    static const phyRegValue sscPllCmnCfg[] = {
        {.addr = CMN_PLL_VCOCAL_PLLCNT_START_OFFSET, .val = CMN_PLL_VCOCAL_PLLCNT_START_SSC_VAL},
        {.addr = CMN_PLL_LOCK_PLLCNT_START_OFFSET, .val = CMN_PLL_LOCK_PLLCNT_START_SSC_VAL},
        {.addr = CMN_PLL_LOCK_PLLCNT_THR_OFFSET, .val = CMN_PLL_LOCK_PLLCNT_THR_SSC_VAL},
    };

    uint8_t i;
    uint32_t pllBase = getPllCmnM0Base(pll);

    const uint32_t regCount = (uint32_t)sizeof(sscPllCmnCfg) / (uint32_t)(sizeof(phyRegValue));

    const phyRegValue* pllCmnCfg;

    if (ssc) {
        pllCmnCfg = sscPllCmnCfg;
    } else {
        pllCmnCfg = noSscPllCmnCfg;
        /* Set reset register values to disable SSC */
        enableSsc(pD, pll, sscCfg);
    }

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllBase + pllCmnCfg[i].addr), pllCmnCfg[i].val);
    }

    afeWrite(pD, (pllBase + CMN_PLL_LOCK_REFCNT_START_OFFSET), CMN_PLL_LOCK_REFCNT_START_VAL);
}

/* Configure PLL for requested VCO frequency. */
static void configurePhyPmaCmnVcoCfg(const DP_SD0801_PrivateData* pD, DP_SD0801_LinkRate linkRate, DP_SD0801_Pll pll, bool ssc)
{
    const ENUM_VCO_FREQ vco_freq = getVcoFreq(linkRate);

    /* Perform register writes specific to VCO frequency. */
    switch (vco_freq)
    {
    case VCO_10GHz8_refclk:
        configurePhyPmaCmnVcoCfg10_8(pD, pll, ssc);
        break;
    case VCO_9GHz72_refclk:
        configurePhyPmaCmnVcoCfg9_72(pD, pll, ssc);
        break;
    case VCO_8GHz64_refclk:
        configurePhyPmaCmnVcoCfg8_64(pD, pll, ssc);
        break;
    default:
        configurePhyPmaCmnVcoCfg8_1(pD, pll, ssc);
        break;
    }
    /* Write register values common for all VCO frequencies. */
    configurePhyPmaCmnVcoCommon(pD, pll, ssc);
}

static bool isPllSet(uint8_t pllBits, uint8_t pllIdx)
{
    bool result = false;
    if (pllIdx < 2U) {
        /* Check, if bit for particular PLL (0 or 1) is set. */
        if (0U != ((pllBits >> pllIdx) & 1U)) {
            result = true;
        }
    }

    return result;
}

static uint16_t getClkSelM0Val(DP_SD0801_LinkRate linkRate)
{
    uint16_t clkSelM0Val;
    switch (linkRate)
    {
    /* Rate: 1.62G */
    case (DP_SD0801_LINK_RATE_1_62):
        clkSelM0Val = CMN_PDIAG_PLL_CLK_SEL_M0_1p62_VAL;
        break;
    /* Rate: 2.16G */
    case (DP_SD0801_LINK_RATE_2_16):
        clkSelM0Val = CMN_PDIAG_PLL_CLK_SEL_M0_2p16_VAL;
        break;
    /* Rate: 2.43G */
    case (DP_SD0801_LINK_RATE_2_43):
        clkSelM0Val =  CMN_PDIAG_PLL_CLK_SEL_M0_2p43_VAL;
        break;
    /* Rate: 2.7G */
    case (DP_SD0801_LINK_RATE_2_70):
        clkSelM0Val =  CMN_PDIAG_PLL_CLK_SEL_M0_2p70_VAL;
        break;
    /* Rate: 3.24G */
    case (DP_SD0801_LINK_RATE_3_24):
        clkSelM0Val =  CMN_PDIAG_PLL_CLK_SEL_M0_3p24_VAL;
        break;
    /* Rate: 4.32G */
    case (DP_SD0801_LINK_RATE_4_32):
        clkSelM0Val =  CMN_PDIAG_PLL_CLK_SEL_M0_4p32_VAL;
        break;
    /* Rate: 5.4G */
    case (DP_SD0801_LINK_RATE_5_40):
        clkSelM0Val =  CMN_PDIAG_PLL_CLK_SEL_M0_5p40_VAL;
        break;
    /* Rate: 8.1G */
    default:
        clkSelM0Val =  CMN_PDIAG_PLL_CLK_SEL_M0_8p10_VAL;
        break;
    }

    return clkSelM0Val;
}

static uint16_t getHsclkDivVal(DP_SD0801_LinkRate linkRate)
{
    uint16_t hsclkDivVal;

    switch (linkRate)
    {
    /* ******* Writing XCVR_DIAG_HSCLK_DIV Register for Lane %d ******* */
    /* Rate: 1.62G */
    case (DP_SD0801_LINK_RATE_1_62):
        hsclkDivVal = XCVR_DIAG_HSCLK_DIV_1p62_VAL;
        break;
    /* Rate: 2.16G */
    case (DP_SD0801_LINK_RATE_2_16):
        hsclkDivVal = XCVR_DIAG_HSCLK_DIV_2p16_VAL;
        break;
    /* Rate: 2.43G */
    case (DP_SD0801_LINK_RATE_2_43):
        hsclkDivVal = XCVR_DIAG_HSCLK_DIV_2p43_VAL;
        break;
    /* Rate: 2.7G */
    case (DP_SD0801_LINK_RATE_2_70):
        hsclkDivVal = XCVR_DIAG_HSCLK_DIV_2p70_VAL;
        break;
    /* Rate: 3.24G */
    case (DP_SD0801_LINK_RATE_3_24):
        hsclkDivVal = XCVR_DIAG_HSCLK_DIV_3p24_VAL;
        break;
    /* Rate: 4.32G */
    case (DP_SD0801_LINK_RATE_4_32):
        hsclkDivVal = XCVR_DIAG_HSCLK_DIV_4p32_VAL;
        break;
    /* Rate: 5.4G */
    case (DP_SD0801_LINK_RATE_5_40):
        hsclkDivVal = XCVR_DIAG_HSCLK_DIV_5p40_VAL;
        break;
    /* Rate: 8.1G */
    default:
        hsclkDivVal = XCVR_DIAG_HSCLK_DIV_8p10_VAL;
        break;
    }

    return hsclkDivVal;
}

static void configurePhyPmaCmnDpRate(const DP_SD0801_PrivateData* pD, uint8_t linkCfg, DP_SD0801_LinkRate dpRate, uint8_t dpPll)
{
    uint16_t hsclkDivVal = getHsclkDivVal(dpRate);
    /* uint32_t used for consistency with bitwise operations. */
    uint32_t i;

    /* 16’h0000 for single DP link configuration */
    /* 16’h0002 for 2 DP link configuration */
    afeWrite(pD, PHY_PLL_CFG, 0x0000);

    /* Configure appropriate PLL (0 / 1) */
    if (isPllSet(dpPll, 0)) {
        afeWrite(pD, (CMN_PLL0_PDIAG_MODE0_BASE + CMN_PDIAG_PLL_CLK_SEL_M0_OFFSET), getClkSelM0Val(dpRate));
    }
    if (isPllSet(dpPll, 1)) {
        afeWrite(pD, (CMN_PLL1_PDIAG_MODE0_BASE + CMN_PDIAG_PLL_CLK_SEL_M0_OFFSET), getClkSelM0Val(dpRate));
    }

    /* PMA lane configuration to deal with multi-link operation */
    for (i = 0; i < 4U; i++) /* depends of active lane. If not it results in timeout */
    {
        if (0U != (linkCfg & (1U << i)))
        {
            /* ******* Writing XCVR_DIAG_HSCLK_DIV Register for Lane 'i' ******* */
            afeWrite(pD, (XCVR_DIAG_HSCLK_DIV | (i << 9U)), hsclkDivVal);
        }
    }
}

static void setA0PowerRegPwrState(uint32_t* pwrState, uint8_t laneCount)
{
    /* lane 0 */
    *pwrState = CPS_FLD_WRITE(DP__DP_REGS__PMA_POWER_STATE_REQ_P, PMA_XCVR_POWER_STATE_REQ_LN_0, *pwrState, 0x00);

    if (laneCount > 1U)
    {
        /* lane 1 */
        *pwrState = CPS_FLD_WRITE(DP__DP_REGS__PMA_POWER_STATE_REQ_P, PMA_XCVR_POWER_STATE_REQ_LN_1, *pwrState, 0x00);
    }

    if (laneCount > 2U)
    {
        /* lanes 2 and 3 */
        *pwrState = CPS_FLD_WRITE(DP__DP_REGS__PMA_POWER_STATE_REQ_P, PMA_XCVR_POWER_STATE_REQ_LN_2, *pwrState, 0x00);
        *pwrState = CPS_FLD_WRITE(DP__DP_REGS__PMA_POWER_STATE_REQ_P, PMA_XCVR_POWER_STATE_REQ_LN_3, *pwrState, 0x00);
    }
}

static void setA0PowerRegPllclkEn(uint32_t* pllclkEn, uint8_t laneCount)
{
    /* lane 0 */
    *pllclkEn = CPS_FLD_WRITE(DP__DP_REGS__PMA_PLLCLK_EN_P, PMA_XCVR_PLLCLK_EN_LN_0, *pllclkEn, 0x00);

    if (laneCount > 1U)
    {
        /* lane 1 */
        *pllclkEn = CPS_FLD_WRITE(DP__DP_REGS__PMA_PLLCLK_EN_P, PMA_XCVR_PLLCLK_EN_LN_1, *pllclkEn, 0x00);
    }

    if (laneCount > 2U)
    {
        /* lanes 2 and 3 */
        *pllclkEn = CPS_FLD_WRITE(DP__DP_REGS__PMA_PLLCLK_EN_P, PMA_XCVR_PLLCLK_EN_LN_2, *pllclkEn, 0x00);
        *pllclkEn = CPS_FLD_WRITE(DP__DP_REGS__PMA_PLLCLK_EN_P, PMA_XCVR_PLLCLK_EN_LN_3, *pllclkEn, 0x00);
    }
}

/**
 * Set lines power state to A0
 * Set lines pll clk enable to 0
 */
static void setPowerA0(const DP_SD0801_PrivateData* pD, uint8_t laneCount)
{
    uint32_t pwrState = CPS_REG_READ(&pD->regBaseDp->dp_regs.PMA_POWER_STATE_REQ_p);
    uint32_t pllclkEn = CPS_REG_READ(&pD->regBaseDp->dp_regs.PMA_PLLCLK_EN_p);

    setA0PowerRegPwrState(&pwrState, laneCount);
    setA0PowerRegPllclkEn(&pllclkEn, laneCount);

    CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PMA_POWER_STATE_REQ_p, pwrState);
    CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PMA_PLLCLK_EN_p, pllclkEn);
}

uint32_t DP_SD0801_RegisterCb(DP_SD0801_PrivateData* pD, const DP_SD0801_Callbacks* callbacks) {

    uint32_t retVal;

    retVal = DP_SD0801_RegisterCbSF(pD, callbacks);

    if (retVal == CDN_EOK) {
        /* Save callbacks references */
        pD->callbacks = *callbacks;
    }

    return retVal;
}

uint32_t DP_SD0801_PhyInit(DP_SD0801_PrivateData* pD, uint8_t mLane, uint8_t laneCount, DP_SD0801_LinkRate linkRate)
{
    uint32_t regTmp;
    uint8_t laneCfg = 0U;
    uint8_t mLaneDp = 0U;
    uint8_t dpLaneCfg = 0U;
#ifdef IS_DEMO_TB
    uint8_t dpPll = (uint8_t)DP_SD0801_PLL_0;
#else
    /* For a single DP link, only PLL0 is used. But at POR, both PLLs must be programmed in the same way */
    uint8_t dpPll = 0x3U; /* (DP_SD0801_PLL_0 | DP_SD0801_PLL_1) */
#endif
    uint32_t retVal = CDN_EOK;

    retVal = DP_SD0801_PhyInitSF(pD, mLane, laneCount, linkRate);

    if (CDN_EOK == retVal)
    {
        laneCfg = getLaneCfg(mLane, laneCount);
        dpLaneCfg = getLaneCfg(mLaneDp, laneCount);

        /* PHY PMA registers configuration function */
        configurePhyPmaDpCfg(pD, laneCfg);

        setPowerA0(pD, laneCount);

        /* release phy_l0*_reset_n and pma_tx_elec_idle_ln_* based on used laneCount */
        regTmp = ((0x000FU & ~(uint32_t)dpLaneCfg) << 4U) | (0x000FU & (uint32_t)dpLaneCfg);
        CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PHY_RESET_p, regTmp);

        /* release pma_xcvr_pllclk_en_ln_*, only for the master lane */
        CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PMA_PLLCLK_EN_p, 0x0001);

        /* PHY PMA registers configuration functions */
        /* Set SSC disabled on init, can be enabled on link rate change. */
        /* If 1 link is used both PLL's should be confiured as same */
        configurePhyPmaCmnVcoCfg(pD, linkRate, DP_SD0801_PLL_0, false);
        configurePhyPmaCmnVcoCfg(pD, linkRate, DP_SD0801_PLL_1, false);

        configurePhyPmaCmnDpRate(pD, laneCfg, linkRate, dpPll);

        pD->linkState.linkRate  = linkRate;
        pD->linkState.mLane = mLane;
        pD->linkState.laneCount = laneCount;
    }

    return retVal;
}

static void setPowerState(const DP_SD0801_PrivateData* pD, phyPowerstate pwrState, uint8_t laneCount)
{
    uint32_t regTmp;

    /* Register value for power state for a single byte. */
    uint32_t pmaPowerStateValPart;

    uint32_t pmaPowerStateVal;
    uint32_t pmaPowerStateMask;

    switch (pwrState)
    {
    case (POWERSTATE_A0):
        pmaPowerStateValPart = 0x01U;
        break;
    case (POWERSTATE_A2):
        pmaPowerStateValPart = 0x04U;
        break;
    default:
        pmaPowerStateValPart = 0x08U;
        break;
    }

    /* Select values of registers and mask, depending on enabled lane count. */
    switch (laneCount)
    {
    /* lane 0 */
    case (0x0001):
        pmaPowerStateVal = pmaPowerStateValPart;
        pmaPowerStateMask = 0x0000003FU;
        break;
    /* lanes 0-1 */
    case (0x0002):
        pmaPowerStateVal = (pmaPowerStateValPart
                            | (pmaPowerStateValPart << 8));
        pmaPowerStateMask = 0x00003F3FU;
        break;
    /* lanes 0-3, all */
    default:
        pmaPowerStateVal = (pmaPowerStateValPart
                            | (pmaPowerStateValPart << 8)
                            | (pmaPowerStateValPart << 16)
                            | (pmaPowerStateValPart << 24));
        pmaPowerStateMask = 0x3F3F3F3FU;
        break;
    }

    /* Set power state A<n> */
    CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PMA_POWER_STATE_REQ_p, pmaPowerStateVal);
    /* Wait, until PHY acknowledges power state completion */
    do {
        regTmp = CPS_REG_READ(&pD->regBaseDp->dp_regs.PMA_POWER_STATE_ACK_p);
    } while (((regTmp) & pmaPowerStateMask) != pmaPowerStateVal);
    CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PMA_POWER_STATE_REQ_p, 0x00000000U);
    CPS_DelayNs(100);
}

/**
 * Version of function DP_SD0801_PhyRun internal to driver.
 * To be used, if parameters (sanity) are ensured to be correct by caller.
 */
static void phyRun(const DP_SD0801_PrivateData* pD, uint8_t laneCount)
{
    uint32_t regTmp;

    /* waiting for ACK of pma_xcvr_pllclk_en_ln_*, only for the master lane */
    do {
        regTmp = CPS_REG_READ(&pD->regBaseDp->dp_regs.PMA_PLLCLK_EN_ACK_p);
    } while (((regTmp) & 0x0001U) == 0x0000U);

    CPS_DelayNs(100);

    setPowerState(pD, POWERSTATE_A2, laneCount);
    setPowerState(pD, POWERSTATE_A0, laneCount);
}

/**
 * Enable DP Main Link lanes, after releasing PHY reset and waiting for PHY to
 * get ready.
 */
uint32_t DP_SD0801_PhyRun(const DP_SD0801_PrivateData* pD, uint8_t laneCount)
{
    uint32_t retVal = CDN_EOK;

    retVal = DP_SD0801_PhyRunSF(pD);

    if (CDN_EOK == retVal)
    {
        phyRun(pD, laneCount);
    }
    return retVal;
}

/**
 * Initialize part of PHY responsible for AUX channel.
 */
uint32_t DP_SD0801_ConfigurePhyAuxCtrl(const DP_SD0801_PrivateData* pD)
{
    uint32_t retVal = CDN_EOK;

    retVal = DP_SD0801_ConfigurePhyAuxCtrSF(pD);

    if (CDN_EOK == retVal)
    {
        CPS_REG_WRITE(&pD->regBaseDp->dp_regs.AUX_CTRL_p, 0x0003);
    }
    return retVal;
}

/**
 * Version of function DP_SD0801_waitPmaCmnReady internal to driver.
 * To be used, if parameter (sanity) is ensured to be correct by caller.
 */
static void waitPmaCmnReady(const DP_SD0801_PrivateData* pD)
{
    uint32_t regTmp;

    do {
        regTmp = CPS_REG_READ(&pD->regBaseDp->dp_regs.PMA_CMN_READY_p);
    } while (((regTmp) & 1U) == 0U);
}

/**
 * Wait, until PHY gets ready after releasing PHY reset signal.
 */
uint32_t DP_SD0801_WaitPmaCmnReady(const DP_SD0801_PrivateData* pD)
{
    uint32_t retVal = CDN_EOK;

    retVal = DP_SD0801_WaitPmaCmnReadySF(pD);
    if (CDN_EOK == retVal)
    {
        waitPmaCmnReady(pD);
    }

    return retVal;
}

/* ------------------------------------------------------------------------------------------------------------------- */

uint32_t DP_SD0801_ConfigLane(DP_SD0801_PrivateData* pD, uint8_t lane, const DP_SD0801_LinkState* linkState)
{
    /* set voltage swing level (0 - 3) for lane (passed as parameter "lane", */
    /* ranging 0-3) as in linkState->voltageSwing[lane] */
    /* set pre-emphasis level (0 - 3) for lane (passed as parameter "lane", */
    /* ranging 0-3) as in linkState->preEmphasis[lane] */

    /* 3.5 Procedure: Change PMA TX Emphasis */
    /* 2.8.3 Display Port / Embedded Display Port */
    /* Torrent16FFC_Programmers_Guide_v0.6.pdf */

    uint32_t retVal;
    uint8_t voltageSwing;
    uint8_t preEmphasis;
    uint16_t regTmp;
    DP_SD0801_VoltageCoefficients* coeffs;

    retVal = DP_SD0801_ConfigLaneSF(pD, linkState);
    if (CDN_EOK == retVal) {

        uint8_t laneNew = pD->linkState.mLane + lane;
        /* Bits 9 and 10 of address indicate lane number. */
        const uint32_t laneOffset = ((uint32_t)laneNew << 9);
        uint32_t DiagAcyaAddr = (TX_DIAG_ACYA | laneOffset);

        voltageSwing = linkState->voltageSwing[lane];
        preEmphasis = linkState->preEmphasis[lane];

        /* Store new settings in pD. */
        pD->linkState.voltageSwing[laneNew] = voltageSwing;
        pD->linkState.preEmphasis[laneNew] = preEmphasis;

        /* Write register bit TX_DIAG_ACYA[0] to 1'b1 to freeze the current state of the analog TX driver. */
        regTmp = afeRead(pD, DiagAcyaAddr);
        regTmp |= TX_DIAG_ACYA_HBDC_MASK;
        afeWrite(pD, DiagAcyaAddr, regTmp);

        if ((voltageSwing + preEmphasis) <= 3U)
        {
            coeffs = &(pD->vCoeffs[voltageSwing][preEmphasis]);
            afeWriteChanged(pD, (TX_TXCC_CTRL | laneOffset), 0x08A4);
            afeWriteChanged(pD, (DRV_DIAG_TX_DRV | laneOffset), coeffs->DiagTxDrv);
            afeWriteChanged(pD, (TX_TXCC_MGNFS_MULT_000 | laneOffset), coeffs->MgnfsMult);
            afeWriteChanged(pD, (TX_TXCC_CPOST_MULT_00 | laneOffset), coeffs->CpostMult);
        }

        /* Write register bit TX_DIAG_ACYA[0] to 1'b0 */
        /* to allow the state of the analog TX driver to reflect the new programmed. */
        regTmp = afeRead(pD, DiagAcyaAddr);
        regTmp &= ~(TX_DIAG_ACYA_HBDC_MASK);
        afeWrite(pD, DiagAcyaAddr, regTmp);
    }

    return retVal;
}

/* Wait for PLL ready de-assertion */
static void waitForPllReadyDeAssert(const DP_SD0801_PrivateData* pD, uint8_t dpPll)
{
    uint16_t regTmp;

    /**
     * For PLL0 - PHY_PMA_CMN_CTRL2[2] == 1
     * For PLL1 - PHY_PMA_CMN_CTRL2[3] == 1
     */
    if (isPllSet(dpPll, 0)) {
        do {
            regTmp = afeRead(pD, PHY_PMA_CMN_CTRL2);
        } while (((regTmp >> 2U) & 1U) == 0U);
    }
    if (isPllSet(dpPll, 1)) {
        do {
            regTmp = afeRead(pD, PHY_PMA_CMN_CTRL2);
        } while (((regTmp >> 3U) & 1U) == 0U);
    }
}

/* Wait for PLL ready assertion */
static void waitForPllReadyAssert(const DP_SD0801_PrivateData* pD, uint8_t dpPll)
{
    uint16_t regTmp;

    /**
     * For PLL0 - PHY_PMA_CMN_CTRL2[0] == 1
     * For PLL1 - PHY_PMA_CMN_CTRL2[1] == 1
     */
    if (isPllSet(dpPll, 0)) {
        do {
            regTmp = afeRead(pD, PHY_PMA_CMN_CTRL2);
        } while (((regTmp) & 1U) == 0U);
    }
    if (isPllSet(dpPll, 1)) {
        do {
            regTmp = afeRead(pD, PHY_PMA_CMN_CTRL2);
        } while (((regTmp >> 1U) & 1U) == 0U);
    }
}

/**
 * Enable or disable PLL for selected lanes)
 */
static void setPllEnable(const DP_SD0801_PrivateData* pD, uint8_t laneCount, bool enable)
{
    uint32_t regTmp;
    /* used to determine, which bits to check for or enable in PMA_PLLCLK_EN register */
    uint32_t pllRegBits;
    /* used to enable or disable lanes */
    uint32_t pllRegWriteVal;

    /* Select values of registers and mask, depending on enabled lane count. */
    switch (laneCount)
    {
    /* lane 0 */
    case (0x0001):
        pllRegBits = DP__DP_REGS__PMA_PLLCLK_EN_P__PMA_XCVR_PLLCLK_EN_LN_0_MASK;
        break;
    /* lanes 0-1 */
    case (0x0002):
        pllRegBits = DP__DP_REGS__PMA_PLLCLK_EN_P__PMA_XCVR_PLLCLK_EN_LN_0_MASK
                     | DP__DP_REGS__PMA_PLLCLK_EN_P__PMA_XCVR_PLLCLK_EN_LN_1_MASK;
        break;
    /* lanes 0-3, all */
    default:
        pllRegBits = DP__DP_REGS__PMA_PLLCLK_EN_P__PMA_XCVR_PLLCLK_EN_LN_0_MASK
                     | DP__DP_REGS__PMA_PLLCLK_EN_P__PMA_XCVR_PLLCLK_EN_LN_1_MASK
                     | DP__DP_REGS__PMA_PLLCLK_EN_P__PMA_XCVR_PLLCLK_EN_LN_2_MASK
                     | DP__DP_REGS__PMA_PLLCLK_EN_P__PMA_XCVR_PLLCLK_EN_LN_3_MASK;
        break;
    }

    if (enable) {
        pllRegWriteVal = pllRegBits;
    } else {
        pllRegWriteVal = 0x00000000U;
    }

    CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PMA_PLLCLK_EN_p, pllRegWriteVal); /* Enable / disable PLL */
    do {
        regTmp = CPS_REG_READ(&pD->regBaseDp->dp_regs.PMA_PLLCLK_EN_ACK_p);
    } while (((regTmp) & pllRegBits) != pllRegWriteVal);
    CPS_DelayNs(100);
}

/**
 * Reconfigure VCO to required one and configure Link Rate, while PLLs are
 * disabled
 */
static void reconfigureLinkRate(const DP_SD0801_PrivateData* pD, DP_SD0801_LinkRate linkRate, uint8_t dpPll, uint8_t linkCfg, bool ssc)
{
    /* Disable the cmn_pll0_en before re-programming the new data rate */
    afeWrite(pD, PHY_PMA_PLL_RAW_CTRL, 0x0000U);

    waitForPllReadyDeAssert(pD, dpPll);

    CPS_DelayNs(200);
    /* DP Rate Change - VCO Output setting */
    /* used 1 link, then both should be set as same */
    configurePhyPmaCmnVcoCfg(pD, linkRate, DP_SD0801_PLL_0, ssc);
    configurePhyPmaCmnVcoCfg(pD, linkRate, DP_SD0801_PLL_1, ssc);

    configurePhyPmaCmnDpRate(pD, linkCfg, linkRate, dpPll);

    /* Enable the cmn_pll0_en */
    afeWrite(pD, PHY_PMA_PLL_RAW_CTRL, 0x0003U);

    waitForPllReadyAssert(pD, dpPll);
}

static void setLinkRate(DP_SD0801_PrivateData* pD, const DP_SD0801_LinkState* linkState)
{
    const DP_SD0801_LinkRate linkRate = linkState->linkRate;
    const uint8_t laneCount = linkState->laneCount;
    uint8_t dpPll = (uint8_t)DP_SD0801_PLL_0; /* TBD how to set */
    uint8_t mLane = pD->linkState.mLane;
    const bool ssc = linkState->ssc;
    uint8_t linkCfg = 0U;

    /* Store new settings in pD. */
    pD->linkState.linkRate = linkRate;
    pD->linkState.laneCount = laneCount;
    pD->linkState.ssc = ssc;

    linkCfg = getLaneCfg(mLane, laneCount);

    setPowerState(pD, POWERSTATE_A3, laneCount);

    /* Disable PLLs */
    setPllEnable(pD, laneCount, false);
    CPS_DelayNs(100);

    reconfigureLinkRate(pD, linkRate, dpPll, linkCfg, ssc);
    /* No need as far as pma_xcvr_standard_mode_ln_* and pma_xcvr_data_width_ln_* are IPS fixed */
    CPS_DelayNs(200);

    /* Enable PLLs */
    setPllEnable(pD, laneCount, true);

    setPowerState(pD, POWERSTATE_A2, laneCount);
    setPowerState(pD, POWERSTATE_A0, laneCount);
    CPS_DelayNs(900); /* 100ns in total with delay in setPowerState */
}

uint32_t DP_SD0801_SetLinkRate(DP_SD0801_PrivateData* pD, const DP_SD0801_LinkState* linkState)
{
    /* set link rate as in linkState->linkRate */

    /* Both pma_xcvr_standard_mode_ln_* and pma_xcvr_data_width_ln_* are IPS fixed */
    /* According to above there's no need to place link in A3 power state */

    uint32_t retVal;

    retVal = DP_SD0801_SetLinkRateSF(pD, linkState);

    if (CDN_EOK == retVal) {
        if (pD->isMultilink) {
            /* For multilink configuration */
            mlSetLinkRate(pD, linkState);
        } else {
            setLinkRate(pD, linkState);
        }
    }

    return retVal;
}

static void setPhyIdleBits(uint32_t *regTmp, uint8_t laneCount)
{
    /* always enable lane 0 */
    *regTmp = CPS_FLD_WRITE(DP__DP_REGS__PHY_RESET_P, PMA_TX_ELEC_IDLE_LN_0, *regTmp, 0);

    /* Enable lane 1 for > 1 lanes */
    *regTmp = CPS_FLD_WRITE(DP__DP_REGS__PHY_RESET_P, PMA_TX_ELEC_IDLE_LN_1, *regTmp, ((laneCount > 1U) ? 0U : 1U));

    /* Enable lanes 2 and 3 for > 2 lanes */
    *regTmp = CPS_FLD_WRITE(DP__DP_REGS__PHY_RESET_P, PMA_TX_ELEC_IDLE_LN_2, *regTmp, ((laneCount > 2U) ? 0U : 1U));
    *regTmp = CPS_FLD_WRITE(DP__DP_REGS__PHY_RESET_P, PMA_TX_ELEC_IDLE_LN_3, *regTmp, ((laneCount > 2U) ? 0U : 1U));
}

/**
 * Assert lane reset (Active low) on lane 0, among disabled lanes.
 */
static void resetLane0(const DP_SD0801_PrivateData* pD, uint8_t dpLaneCfg)
{
    uint32_t regTmp;

    /* Assert lane reset low so that unused lanes remain in reset and powered down when re-enable the link */
    regTmp = CPS_REG_READ(&pD->regBaseDp->dp_regs.PHY_RESET_p);
    CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PHY_RESET_p, ((regTmp & 0x0000FFF0U) | (0x0000000EU & (uint32_t)dpLaneCfg)));
}

static void startupLanes(const DP_SD0801_PrivateData* pD, uint8_t laneCount)
{
    uint32_t regTmp;
    uint8_t mLaneDp = 0U;
    uint8_t dpLaneCfg = 0U;

    dpLaneCfg = getLaneCfg(mLaneDp, laneCount);

    /* Assert lane reset (Active low) on lane 0, among disabled lanes. */
    resetLane0(pD, dpLaneCfg);

    /* Set lanes into power state A0 */
    setPowerA0(pD, laneCount);

    /* release phy_l0*_reset_n based on used laneCount */
    regTmp = CPS_REG_READ(&pD->regBaseDp->dp_regs.PHY_RESET_p);
    CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PHY_RESET_p, ((regTmp & 0x0000FFF0U) | (0x0000000FU & (uint32_t)dpLaneCfg)));
}

/**
 * Start-up PHY lanes and wait for pma_cmn_ready, then delay for 100 ns
 */
static void startupLanesAndWait(const DP_SD0801_PrivateData* pD, uint8_t laneCount)
{
    startupLanes(pD, laneCount);

    /* Checking pma_cmn_ready */
    waitPmaCmnReady(pD);

    CPS_DelayNs(100);
}

uint32_t DP_SD0801_EnableLanes(DP_SD0801_PrivateData* pD, const DP_SD0801_LinkState* linkState)
{
    /* set active lane count (1, 2 or 4) as in linkState->laneCount. */

    uint32_t retVal;
    uint32_t regTmp;

    retVal = DP_SD0801_EnableLanesSF(pD, linkState);
    if (CDN_EOK == retVal) {

        const uint8_t laneCount = linkState->laneCount;

        /* Store new setting in pD. */
        pD->linkState.laneCount = laneCount;

        /* Assert pma_tx_elec_idle_ln_* for disabled lanes */
        regTmp = CPS_REG_READ(&pD->regBaseDp->dp_regs.PHY_RESET_p);
        setPhyIdleBits(&regTmp, laneCount);
        CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PHY_RESET_p, regTmp);

        /* reset the link by asserting phy_l00_reset_n low */
        regTmp = CPS_REG_READ(&pD->regBaseDp->dp_regs.PHY_RESET_p);
        CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PHY_RESET_p, (regTmp & 0x0000FFFEU));

        startupLanesAndWait(pD, laneCount);

        /* release pma_xcvr_pllclk_en_ln_*, only for the master lane */
        CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PMA_PLLCLK_EN_p, 0x0001);

        phyRun(pD, laneCount);
    }

    return retVal;
}

/* parasoft-begin-suppress METRICS-39-3 "The value of VOCF metric for a function should not be higher than 4, DRV-3852" */
/**
 * Get default values of voltage-related PHY registers, for given
 * voltage swing and pre-emphasis.
 */
uint32_t DP_SD0801_GetDefaultCoeffs(const DP_SD0801_PrivateData*   pD,
                                    uint8_t                        voltageSwing,
                                    uint8_t                        preEmphasis,
                                    DP_SD0801_VoltageCoefficients* coefficients)
{
    /* Array consist of default values of voltage-related registers for sd0801 PHY. */
    static const DP_SD0801_VoltageCoefficients defaults[DP_SD0801_SWING_LEVEL_COUNT][DP_SD0801_EMPHASIS_LEVEL_COUNT] = \
        /* voltage swing 0, pre-emphasis 0->3 */
    {{{.DiagTxDrv = 0x0003, .MgnfsMult = 0x002A, .CpostMult = 0x0000},
      {.DiagTxDrv = 0x0003, .MgnfsMult = 0x001F, .CpostMult = 0x0014},
      {.DiagTxDrv = 0x0003, .MgnfsMult = 0x0012, .CpostMult = 0x0020},
      {.DiagTxDrv = 0x0003, .MgnfsMult = 0x0000, .CpostMult = 0x002A}},

     /* voltage swing 1, pre-emphasis 0->3 */
     {{.DiagTxDrv = 0x0003, .MgnfsMult = 0x001F, .CpostMult = 0x0000},
      {.DiagTxDrv = 0x0003, .MgnfsMult = 0x0013, .CpostMult = 0x0012},
      {.DiagTxDrv = 0x0003, .MgnfsMult = 0x0000, .CpostMult = 0x001F},
      {.DiagTxDrv = 0xFFFF, .MgnfsMult = 0xFFFF, .CpostMult = 0xFFFF}},

     /* voltage swing 2, pre-emphasis 0->3 */
     {{.DiagTxDrv = 0x0003, .MgnfsMult = 0x0013, .CpostMult = 0x0000},
      {.DiagTxDrv = 0x0003, .MgnfsMult = 0x0000, .CpostMult = 0x0013},
      {.DiagTxDrv = 0xFFFF, .MgnfsMult = 0xFFFF, .CpostMult = 0xFFFF},
      {.DiagTxDrv = 0xFFFF, .MgnfsMult = 0xFFFF, .CpostMult = 0xFFFF}},

     /* voltage swing 3, pre-emphasis 0->3 */
     {{.DiagTxDrv = 0x0003, .MgnfsMult = 0x0000, .CpostMult = 0x0000},
      {.DiagTxDrv = 0xFFFF, .MgnfsMult = 0xFFFF, .CpostMult = 0xFFFF},
      {.DiagTxDrv = 0xFFFF, .MgnfsMult = 0xFFFF, .CpostMult = 0xFFFF},
      {.DiagTxDrv = 0xFFFF, .MgnfsMult = 0xFFFF, .CpostMult = 0xFFFF}}};

    uint32_t retVal;
    retVal = DP_SD0801_GetDefaultCoeffsSF(pD, voltageSwing, preEmphasis, coefficients);

    if (CDN_EOK == retVal)
    {
        /* Fill structure with default values for voltage swing and pre-emphasis */
        /* taken from the table. */
        coefficients->DiagTxDrv = defaults[voltageSwing][preEmphasis].DiagTxDrv;
        coefficients->MgnfsMult = defaults[voltageSwing][preEmphasis].MgnfsMult;
        coefficients->CpostMult = defaults[voltageSwing][preEmphasis].CpostMult;
    }

    return retVal;
}

/* parasoft-end-suppress METRICS-39-3 */

/*--------------------------------------- Multilink configuration-------------------------------------------------*/

static void ml100MhzPhyPmaCmnVcoCfg10_8(const DP_SD0801_PrivateData* pD, DP_SD0801_Pll dpPll, bool ssc)
{
    /* Settings for VCO equals 10.8GHz */

    if (isPllSet((uint8_t)dpPll, 0)) {
        afeWrite(pD, (CMN_PLL0_MODE0_BASE + CMN_PLL_DSM_FBH_OVRD_M0_OFFSET), 0x0022);
    }
    if (isPllSet((uint8_t)dpPll, 1)) {
        afeWrite(pD, (CMN_PLL1_PDIAG_MODE0_BASE + CMN_PDIAG_PLL_CP_PADJ_M0_OFFSET), 0x0028);
        afeWrite(pD, (CMN_PLL1_MODE0_BASE + CMN_PLL_DSM_FBH_OVRD_M0_OFFSET), 0x0022);
        afeWrite(pD, (CMN_PLL1_MODE0_BASE + CMN_PLL_DSM_FBL_OVRD_M0_OFFSET), 0x000C);
    }

    if (ssc) {
        /* Do nothing */
    }
}

/* parasoft-begin-suppress METRICS-39-3 "The value of VOCF metric for a function should not be higher than 4, DRV-3852" */
static void ml100MhzPhyPmaCmnVcoCfg9_72(const DP_SD0801_PrivateData* pD, DP_SD0801_Pll dpPll, bool ssc)
{
    /* Settings for VCO equals 9.72GHz */
    static const phyRegValue mlPllCmnCfg9p72[] = {
        {.addr = CMN_PLL_DSM_DIAG_M0_OFFSET, .val = 0x0004},
        {.addr = CMN_PLL_INTDIV_M0_OFFSET, .val = 0x0061},
        {.addr = CMN_PLL_FRACDIVL_M0_OFFSET, .val = 0x3333},
        {.addr = CMN_PLL_FRACDIVH_M0_OFFSET, .val = 0x0002},
        {.addr = CMN_PLL_HIGH_THR_M0_OFFSET, .val = 0x0042},
    };

    /* PLL diagnostic regs */
    static const phyRegValue pllPdiagCmnCfg9p72[] = {
        {.addr = CMN_PDIAG_PLL_CP_PADJ_M0_OFFSET, .val = 0x0509},
        {.addr = CMN_PDIAG_PLL_CP_IADJ_M0_OFFSET, .val = 0x0F00},
        {.addr = CMN_PDIAG_PLL_FILT_PADJ_M0_OFFSET, .val = 0x0F08},
        {.addr = CMN_PDIAG_PLL_CTRL_M0_OFFSET, .val = 0x0002}
    };

    uint32_t i;
    uint32_t regCount;
    uint32_t pllBase = getPllCmnM0Base(dpPll);
    uint32_t pllPdiagBase = getPllPdiagM0Base(dpPll);

    regCount = ARRAY_SIZE(mlPllCmnCfg9p72);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllBase + mlPllCmnCfg9p72[i].addr), mlPllCmnCfg9p72[i].val);
    }

    regCount = ARRAY_SIZE(pllPdiagCmnCfg9p72);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllPdiagBase + pllPdiagCmnCfg9p72[i].addr), pllPdiagCmnCfg9p72[i].val);
    }

    if (ssc) {
        /* Do nothing */
    }
}

static void ml100MhzPhyPmaCmnVcoCfg8_64(const DP_SD0801_PrivateData* pD, DP_SD0801_Pll dpPll, bool ssc)
{
    /* Settings for VCO equals 8.64GHz */
    static const phyRegValue mlPllCmnCfg8p64[] = {
        {.addr = CMN_PLL_DSM_DIAG_M0_OFFSET, .val = 0x0004},
        {.addr = CMN_PLL_INTDIV_M0_OFFSET, .val = 0x0056},
        {.addr = CMN_PLL_FRACDIVL_M0_OFFSET, .val = 0x6666},
        {.addr = CMN_PLL_FRACDIVH_M0_OFFSET, .val = 0x0002},
        {.addr = CMN_PLL_HIGH_THR_M0_OFFSET, .val = 0x003A},
    };

    /* PLL diagnostic regs */
    static const phyRegValue pllPdiagCmnCfg8p64[] = {
        {.addr = CMN_PDIAG_PLL_CP_PADJ_M0_OFFSET, .val = 0x0509},
        {.addr = CMN_PDIAG_PLL_CP_IADJ_M0_OFFSET, .val = 0x0F00},
        {.addr = CMN_PDIAG_PLL_FILT_PADJ_M0_OFFSET, .val = 0x0F08},
        {.addr = CMN_PDIAG_PLL_CTRL_M0_OFFSET, .val = 0x0002}
    };

    uint32_t i;
    uint32_t regCount;
    uint32_t pllBase = getPllCmnM0Base(dpPll);
    uint32_t pllPdiagBase = getPllPdiagM0Base(dpPll);

    regCount = ARRAY_SIZE(mlPllCmnCfg8p64);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllBase + mlPllCmnCfg8p64[i].addr), mlPllCmnCfg8p64[i].val);
    }

    regCount = ARRAY_SIZE(pllPdiagCmnCfg8p64);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllPdiagBase + pllPdiagCmnCfg8p64[i].addr), pllPdiagCmnCfg8p64[i].val);
    }

    if (ssc) {
        /* Do nothing */
    }
}

static void ml100MhzPhyPmaCmnVcoCfg8_1(const DP_SD0801_PrivateData* pD, DP_SD0801_Pll dpPll, bool ssc)
{
    /* Settings for VCO equals 8.1GHz */
    static const phyRegValue mlPllCmnCfg8p1[] = {
        {.addr = CMN_PLL_DSM_DIAG_M0_OFFSET, .val = 0x0004},
        {.addr = CMN_PLL_INTDIV_M0_OFFSET, .val = 0x0051},
        {.addr = CMN_PLL_FRACDIVH_M0_OFFSET, .val = 0x0002},
        {.addr = CMN_PLL_HIGH_THR_M0_OFFSET, .val = 0x0036},
    };

    /* PLL diagnostic regs */
    static const phyRegValue pllPdiagCmnCfg8p1[] = {
        {.addr = CMN_PDIAG_PLL_CP_PADJ_M0_OFFSET, .val = 0x0509},
        {.addr = CMN_PDIAG_PLL_CP_IADJ_M0_OFFSET, .val = 0x0F00},
        {.addr = CMN_PDIAG_PLL_FILT_PADJ_M0_OFFSET, .val = 0x0F08},
        {.addr = CMN_PDIAG_PLL_CTRL_M0_OFFSET, .val = 0x0002}
    };

    uint32_t i;
    uint32_t regCount;
    uint32_t pllBase = getPllCmnM0Base(dpPll);
    uint32_t pllPdiagBase = getPllPdiagM0Base(dpPll);

    regCount = ARRAY_SIZE(mlPllCmnCfg8p1);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllBase + mlPllCmnCfg8p1[i].addr), mlPllCmnCfg8p1[i].val);
    }

    regCount = ARRAY_SIZE(pllPdiagCmnCfg8p1);

    for (i = 0U; i < regCount; i++) {
        afeWrite(pD, (pllPdiagBase + pllPdiagCmnCfg8p1[i].addr), pllPdiagCmnCfg8p1[i].val);
    }

    if (ssc) {
        /* Do nothing */
    }
}
/* parasoft-end-suppress METRICS-39-3 */

/* Configure PLL for requested VCO frequency. */
static void ml100MhzPhyPmaCmnVcoCfg(const DP_SD0801_PrivateData* pD,
                                    DP_SD0801_LinkRate linkRate,
                                    uint8_t pll, bool ssc)
{
    const ENUM_VCO_FREQ vco_freq = getVcoFreq(linkRate);
    DP_SD0801_Pll dpPll;

    /* Check PLL used for DP */
    if (isPllSet(pll, 0)) {
        dpPll = DP_SD0801_PLL_0;
    }
    if (isPllSet(pll, 1)) {
        dpPll = DP_SD0801_PLL_1;
    }

    /* Perform register writes specific to VCO frequency. */
    switch (vco_freq)
    {
    case VCO_10GHz8_refclk:
        ml100MhzPhyPmaCmnVcoCfg10_8(pD, dpPll, ssc);
        break;
    case VCO_9GHz72_refclk:
        ml100MhzPhyPmaCmnVcoCfg9_72(pD, dpPll, ssc);
        break;
    case VCO_8GHz64_refclk:
        ml100MhzPhyPmaCmnVcoCfg8_64(pD, dpPll, ssc);
        break;
    default:
        ml100MhzPhyPmaCmnVcoCfg8_1(pD, dpPll, ssc);
        break;
    }
}

static void mlConfigurePhyPmaCmnDpRate(const DP_SD0801_PrivateData* pD, uint8_t linkCfg,
                                       DP_SD0801_LinkRate dpRate, uint8_t dpPll)
{
    uint16_t hsclkDivVal = getHsclkDivVal(dpRate);
    /* uint32_t used for consistency with bitwise operations. */
    uint32_t i;

    /* Configure appropriate PLL (0 / 1) */
    if (isPllSet(dpPll, 0)) {
        afeWrite(pD, (CMN_PLL0_PDIAG_MODE0_BASE + CMN_PDIAG_PLL_CLK_SEL_M0_OFFSET), getClkSelM0Val(dpRate));
    }
    if (isPllSet(dpPll, 1)) {
        afeWrite(pD, (CMN_PLL1_PDIAG_MODE0_BASE + CMN_PDIAG_PLL_CLK_SEL_M0_OFFSET), getClkSelM0Val(dpRate));
    }

    /* PMA lane configuration to deal with multi-link operation */
    for (i = 0; i < 4U; i++)
    {
        if (0U != (linkCfg & (1U << i)))
        {
            /* ******* Writing XCVR_DIAG_HSCLK_DIV Register for Lane 'i' ******* */
            afeWrite(pD, (XCVR_DIAG_HSCLK_DIV | (i << 9U)), hsclkDivVal);
        }
    }
}

/* Get PLL used for DP for single link and multi-link configurations */
static void mlGetDpPll(uint8_t* dpPll, DP_SD0801_PhyType otherphyType)
{
    switch (otherphyType) {
    case DP_SD0801_PHY_TYPE_PCIE:
    case DP_SD0801_PHY_TYPE_USB:
        *dpPll = (uint8_t)DP_SD0801_PLL_1;
        break;
    default:    /* Single link case */
        *dpPll = 0x3U; /* (DP_SD0801_PLL_0 | DP_SD0801_PLL_1) */
        break;
    }
}

static void mlPhyInitDP(DP_SD0801_PrivateData*         pD,
                        const DP_SD0801_MlPhyInstance* dpPhyInst,
                        DP_SD0801_LinkRate             linkRate)
{
    uint32_t regTmp;
    uint8_t laneCfg = 0U;
    uint8_t mLaneDp = 0U;
    uint8_t dpLaneCfg = 0U;

    /* For PHY APB register addresses */
    laneCfg = getLaneCfg(dpPhyInst->mLane, dpPhyInst->numLanes);

    /* For DPTX controller's register addresses */
    dpLaneCfg = getLaneCfg(mLaneDp, dpPhyInst->numLanes);

    setPowerA0(pD, dpPhyInst->numLanes);

    /* release phy_l0*_reset_n and pma_tx_elec_idle_ln_* based on used laneCount */
    regTmp = CPS_REG_READ(&pD->regBaseDp->dp_regs.PHY_RESET_p);

    regTmp |= (0x000FU & (uint32_t)dpLaneCfg);
    regTmp &= ~((uint32_t)dpLaneCfg << 4U);

    CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PHY_RESET_p, regTmp);

    /* release pma_xcvr_pllclk_en_ln_*, only for the master lane */
    regTmp = CPS_REG_READ(&pD->regBaseDp->dp_regs.PMA_PLLCLK_EN_p);

    regTmp |= 0x1U;

    CPS_REG_WRITE(&pD->regBaseDp->dp_regs.PMA_PLLCLK_EN_p, regTmp);

    /* PHY PMA registers configuration functions */
    /* Set SSC disabled on init, can be enabled on link rate change. */
    if (pD->refClk == DP_SD0801_CLK_100_MHZ) {
        ml100MhzPhyPmaCmnVcoCfg(pD, linkRate, pD->dpPll, false);
    }

    mlConfigurePhyPmaCmnDpRate(pD, laneCfg, linkRate, pD->dpPll);
}

/**
 * Part of PHY initialization for multilink configuration. Performs
 * operations to be done before releasing PHY reset.
 */
uint32_t DP_SD0801_MlPhyInit(DP_SD0801_PrivateData*         pD,
                             const DP_SD0801_MlPhyInstance* dpPhyInst,
                             DP_SD0801_LinkRate             linkRate,
                             const DP_SD0801_MlPhyInstance* otherPhyInst)
{
    uint32_t retVal = CDN_EOK;

    retVal = DP_SD0801_MlPhyInitSF(pD, dpPhyInst, linkRate, otherPhyInst);

    if (CDN_EOK == retVal)
    {
        /* Configure PHY PMA multilink configuration */
        mlConfigurePhyPmaCfg(pD, otherPhyInst, dpPhyInst);

        mlGetDpPll(&pD->dpPll, otherPhyInst->phyType);

        /* DP specific configuration */
        mlPhyInitDP(pD, dpPhyInst, linkRate);

        pD->linkState.linkRate  = linkRate;
        pD->linkState.mLane = dpPhyInst->mLane;
        pD->linkState.laneCount = dpPhyInst->numLanes;
    }

    return retVal;
}

/**
 * Reconfigure VCO to required one and configure Link Rate, while PLLs are
 * disabled
 */
static void mlReconfigureLinkRate(const DP_SD0801_PrivateData* pD,
                                  DP_SD0801_LinkRate linkRate,
                                  uint8_t dpPll, uint8_t linkCfg, bool ssc)
{
    uint16_t regTmp;

    /**
     * Disable the associated PLL (cmn_pll0_en or cmn_pll1_en)
     * before re-programming the new data rate.
     */
    regTmp = afeRead(pD, PHY_PMA_PLL_RAW_CTRL);
    regTmp &= ~dpPll;
    afeWrite(pD, PHY_PMA_PLL_RAW_CTRL, regTmp);

    waitForPllReadyDeAssert(pD, dpPll);

    CPS_DelayNs(200);

    /* DP Rate Change - VCO Output setting, 100MHz refclk */
    if (pD->refClk == DP_SD0801_CLK_100_MHZ) {
        ml100MhzPhyPmaCmnVcoCfg(pD, linkRate, dpPll, false);
    }

    mlConfigurePhyPmaCmnDpRate(pD, linkCfg, linkRate, dpPll);

    /* Enable the associated PLL (cmn_pll0_en or cmn_pll1_en) */
    regTmp = afeRead(pD, PHY_PMA_PLL_RAW_CTRL);
    regTmp |= dpPll;
    afeWrite(pD, PHY_PMA_PLL_RAW_CTRL, regTmp);

    waitForPllReadyAssert(pD, dpPll);

    if (ssc) {
        /* Do nothing */
    }
}

static void mlSetLinkRate(DP_SD0801_PrivateData* pD, const DP_SD0801_LinkState* linkState)
{
    const DP_SD0801_LinkRate linkRate = linkState->linkRate;
    const uint8_t laneCount = linkState->laneCount;
    uint8_t mLane = pD->linkState.mLane;
    bool ssc = false;   /* linkState->ssc */
    uint8_t dpPll = pD->dpPll;
    uint8_t linkCfg = 0U;

    /* Store new settings in pD. */
    pD->linkState.linkRate = linkRate;
    pD->linkState.laneCount = laneCount;
    pD->linkState.ssc = ssc;

    linkCfg = getLaneCfg(mLane, laneCount);

    setPowerState(pD, POWERSTATE_A3, laneCount);

    /* Disable PLLs */
    setPllEnable(pD, laneCount, false);
    CPS_DelayNs(100);

    mlReconfigureLinkRate(pD, linkRate, dpPll, linkCfg, ssc);
    /* No need as far as pma_xcvr_standard_mode_ln_* and pma_xcvr_data_width_ln_* are IPS fixed */
    CPS_DelayNs(200);

    /* Enable PLLs */
    setPllEnable(pD, laneCount, true);

    setPowerState(pD, POWERSTATE_A2, laneCount);
    setPowerState(pD, POWERSTATE_A0, laneCount);
    CPS_DelayNs(900); /* 100ns in total with delay in setPowerState */
}

/*---------------------------------------------------------------------------------------------------------------*/

/* parasoft-end-suppress METRICS-36-3 */
