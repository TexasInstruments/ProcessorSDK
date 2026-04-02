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
 * dp_sd0801_ml_cfg.c
 *
 ******************************************************************************
 */

#include "dp_sd0801_if.h"
#include "dp_sd0801_priv.h"
#include "dp_sd0801_sanity.h"

#include "dp_sd0801_cfg.h"

#include "cdn_log.h"

#include "dp_sd0801_internal.h"
#include "dp_sd0801_spec.h"

struct phyRegPairs {
    uint16_t val;
    uint32_t addr;
};

struct phyRegVals {
    struct phyRegPairs *regPairs;
    uint32_t numRegs;
};

struct phyRegValsEntry {
    uint32_t key;
    struct phyRegVals *vals;
};

struct phyRegValsTable {
    struct phyRegValsEntry *entries;
    uint32_t numEntries;
};

struct phyRegValsData {
    struct phyRegValsTable linkCmnValsTbl;
    struct phyRegValsTable xcvrDiagValsTbl;
    struct phyRegValsTable phyPcsCmnValsTbl;
    struct phyRegValsTable cmnValsTbl;
    struct phyRegValsTable txLnValsTbl;
    struct phyRegValsTable rxLnValsTbl;
};

/* USB, 100 MHz Ref clk, no SSC */
static struct phyRegPairs usb100NoSscCmnRegs[] = {
    {0x8200, CMN_CDIAG_CDB_PWRI_OVRD},
    {0x8200, CMN_CDIAG_XCVRC_PWRI_OVRD},
    {0x007F, CMN_TXPUCAL_TUNE},
    {0x007F, CMN_TXPDCAL_TUNE}
};

static struct phyRegPairs usb100NoSscTxLnRegs[] = {
    {0x02FF, TX_PSC_A0},
    {0x06AF, TX_PSC_A1},
    {0x06AE, TX_PSC_A2},
    {0x06AE, TX_PSC_A3},
    {0x2A82, TX_TXCC_CTRL},
    {0x0014, TX_TXCC_CPOST_MULT_01},
    {0x0003, XCVR_DIAG_PSC_OVRD}
};

static struct phyRegPairs usb100NoSscRxLnRegs[] = {
    {0x0D1D, RX_PSC_A0},
    {0x0D1D, RX_PSC_A1},
    {0x0D00, RX_PSC_A2},
    {0x0500, RX_PSC_A3},
    {0x0013, RX_SIGDET_HL_FILT_TMR},
    {0x0000, RX_REE_GCSM1_CTRL},
    {0x0C02, RX_REE_ATTEN_THR},
    {0x0330, RX_REE_SMGM_CTRL1},
    {0x0300, RX_REE_SMGM_CTRL2},
    {0x0000, RX_REE_PEAK_UTHR},
    {0x01F5, RX_REE_PEAK_LTHR},
    {0x0019, RX_REE_TAP1_CLIP},
    {0x0019, RX_REE_TAP2TON_CLIP},
    {0x1004, RX_DIAG_SIGDET_TUNE},
    {0x00F9, RX_DIAG_NQST_CTRL},
    {0x0C01, RX_DIAG_DFE_AMP_TUNE_2},
    {0x0002, RX_DIAG_DFE_AMP_TUNE_3},
    {0x0000, RX_DIAG_PI_CAP},
    {0x0031, RX_DIAG_PI_RATE},
    {0x0001, RX_DIAG_ACYA},
    {0x018C, RX_CDRLF_CNFG},
    {0x0003, RX_CDRLF_CNFG3}
};

static struct phyRegVals usb100NoSscCmnVals = {
    .regPairs = usb100NoSscCmnRegs,
    .numRegs = ARRAY_SIZE(usb100NoSscCmnRegs),
};

static struct phyRegVals usb100NoSscTxLnVals = {
    .regPairs = usb100NoSscTxLnRegs,
    .numRegs = ARRAY_SIZE(usb100NoSscTxLnRegs),
};

static struct phyRegVals usb100NoSscRxLnVals = {
    .regPairs = usb100NoSscRxLnRegs,
    .numRegs = ARRAY_SIZE(usb100NoSscRxLnRegs),
};

/* USB and DP link configuration */
static struct phyRegPairs usbDpLinkCmnRegs[] = {
    {0x0002, PHY_PLL_CFG},
    {0x8600, CMN_PDIAG_PLL0_CLK_SEL_M0}
};

static struct phyRegPairs usbDpXcvrDiagLnRegs[] = {
    {0x0041, XCVR_DIAG_PLLDRC_CTRL}
};

static struct phyRegPairs dpUsbXcvrDiagLnRegs[] = {
    {0x0001, XCVR_DIAG_HSCLK_SEL},
    {0x0019, XCVR_DIAG_PLLDRC_CTRL}
};

static struct phyRegVals usbDpLinkCmnVals = {
    .regPairs = usbDpLinkCmnRegs,
    .numRegs = ARRAY_SIZE(usbDpLinkCmnRegs),
};

static struct phyRegVals usbDpXcvrDiagLnVals = {
    .regPairs = usbDpXcvrDiagLnRegs,
    .numRegs = ARRAY_SIZE(usbDpXcvrDiagLnRegs),
};

static struct phyRegVals dpUsbXcvrDiagLnVals = {
    .regPairs = dpUsbXcvrDiagLnRegs,
    .numRegs = ARRAY_SIZE(dpUsbXcvrDiagLnRegs),
};

/* USB PHY PCS common configuration */
static struct phyRegPairs usbPhyPcsCmnRegs[] = {
    {0x0A0A, PHY_PIPE_USB3_GEN2_PRE_CFG0},
    {0x1000, PHY_PIPE_USB3_GEN2_POST_CFG0},
    {0x0010, PHY_PIPE_USB3_GEN2_POST_CFG1}
};

static struct phyRegVals usbPhyPcsCmnVals = {
    .regPairs = usbPhyPcsCmnRegs,
    .numRegs = ARRAY_SIZE(usbPhyPcsCmnRegs),
};

/* PCIe, 100 MHz Ref clk, no SSC & external SSC */
static struct phyRegPairs pcie100ExtNoSscRxLnRegs[] = {
    {0x0019, RX_REE_TAP1_CLIP},
    {0x0019, RX_REE_TAP2TON_CLIP},
    {0x0001, RX_DIAG_ACYA}
};

static struct phyRegVals pcie100NoSscRxLnVals = {
    .regPairs = pcie100ExtNoSscRxLnRegs,
    .numRegs = ARRAY_SIZE(pcie100ExtNoSscRxLnRegs),
};

/* PCIe and DP link configuration */
static struct phyRegPairs pcieDpLinkCmnRegs[] = {
    {0x0003, PHY_PLL_CFG},
    {0x0601, CMN_PDIAG_PLL0_CLK_SEL_M0},
    {0x0400, CMN_PDIAG_PLL0_CLK_SEL_M1}
};

static struct phyRegPairs pcieDpXcvrDiagLnRegs[] = {
    {0x0000, XCVR_DIAG_HSCLK_SEL},
    {0x0001, XCVR_DIAG_HSCLK_DIV},
    {0x0012, XCVR_DIAG_PLLDRC_CTRL}
};

static struct phyRegPairs dpPcieXcvrDiagLnRegs[] = {
    {0x0001, XCVR_DIAG_HSCLK_SEL},
    {0x0009, XCVR_DIAG_PLLDRC_CTRL}
};

static struct phyRegVals pcieDpLinkCmnVals = {
    .regPairs = pcieDpLinkCmnRegs,
    .numRegs = ARRAY_SIZE(pcieDpLinkCmnRegs),
};

static struct phyRegVals pcieDpXcvrDiagLnVals = {
    .regPairs = pcieDpXcvrDiagLnRegs,
    .numRegs = ARRAY_SIZE(pcieDpXcvrDiagLnRegs),
};

static struct phyRegVals dpPcieXcvrDiagLnVals = {
    .regPairs = dpPcieXcvrDiagLnRegs,
    .numRegs = ARRAY_SIZE(dpPcieXcvrDiagLnRegs),
};

/* DP Multilink, 100 MHz Ref clk, no SSC */
static struct phyRegPairs dp100NoSscCmnRegs[] = {
    {0x007F, CMN_TXPUCAL_TUNE},
    {0x007F, CMN_TXPDCAL_TUNE}
};

/* DP Single link, 100 MHz Ref clk, no SSC */
static struct phyRegPairs slDp100NoSscCmnRegs[] = {
    {0x0003, CMN_PLL0_VCOCAL_TCTRL},
    {0x0003, CMN_PLL1_VCOCAL_TCTRL}
};

static struct phyRegPairs dp100NoSscTxLnRegs[] = {
    {0x00FB, TX_PSC_A0},
    {0x04AA, TX_PSC_A2},
    {0x04AA, TX_PSC_A3},
    {0x000F, XCVR_DIAG_BIDI_CTRL}
};

static struct phyRegPairs dp100NoSscRxLnRegs[] = {
    {0x0000, RX_PSC_A0},
    {0x0000, RX_PSC_A2},
    {0x0000, RX_PSC_A3},
    {0x0000, RX_PSC_CAL},
    {0x0000, RX_REE_GCSM1_CTRL},
    {0x0000, RX_REE_GCSM2_CTRL},
    {0x0000, RX_REE_PERGCSM_CTRL}
};

static struct phyRegVals dp100NoSscCmnVals = {
    .regPairs = dp100NoSscCmnRegs,
    .numRegs = ARRAY_SIZE(dp100NoSscCmnRegs),
};

static struct phyRegVals slDp100NoSscCmnVals = {
    .regPairs = slDp100NoSscCmnRegs,
    .numRegs = ARRAY_SIZE(slDp100NoSscCmnRegs),
};

static struct phyRegVals dp100NoSscTxLnVals = {
    .regPairs = dp100NoSscTxLnRegs,
    .numRegs = ARRAY_SIZE(dp100NoSscTxLnRegs),
};

static struct phyRegVals dp100NoSscRxLnVals = {
    .regPairs = dp100NoSscRxLnRegs,
    .numRegs = ARRAY_SIZE(dp100NoSscRxLnRegs),
};

static struct phyRegValsEntry linkCmnValsEntries[] = {
    {DP_SD0801_KEY_ANYCLK(DP_SD0801_PHY_TYPE_PCIE, DP_SD0801_PHY_TYPE_DP), &pcieDpLinkCmnVals},
    {DP_SD0801_KEY_ANYCLK(DP_SD0801_PHY_TYPE_DP, DP_SD0801_PHY_TYPE_PCIE), &pcieDpLinkCmnVals},

    {DP_SD0801_KEY_ANYCLK(DP_SD0801_PHY_TYPE_USB, DP_SD0801_PHY_TYPE_DP), &usbDpLinkCmnVals},
    {DP_SD0801_KEY_ANYCLK(DP_SD0801_PHY_TYPE_DP, DP_SD0801_PHY_TYPE_USB), &usbDpLinkCmnVals},
};

static struct phyRegValsEntry xcvrDiagValsEntries[] = {
    {DP_SD0801_KEY_ANYCLK(DP_SD0801_PHY_TYPE_PCIE, DP_SD0801_PHY_TYPE_DP), &pcieDpXcvrDiagLnVals},
    {DP_SD0801_KEY_ANYCLK(DP_SD0801_PHY_TYPE_DP, DP_SD0801_PHY_TYPE_PCIE), &dpPcieXcvrDiagLnVals},

    {DP_SD0801_KEY_ANYCLK(DP_SD0801_PHY_TYPE_USB, DP_SD0801_PHY_TYPE_DP), &usbDpXcvrDiagLnVals},
    {DP_SD0801_KEY_ANYCLK(DP_SD0801_PHY_TYPE_DP, DP_SD0801_PHY_TYPE_USB), &dpUsbXcvrDiagLnVals},
};

static struct phyRegValsEntry phyPcsCmnValsEntries[] = {
    {DP_SD0801_KEY_ANYCLK(DP_SD0801_PHY_TYPE_USB, DP_SD0801_PHY_TYPE_DP), &usbPhyPcsCmnVals},
};

static struct phyRegValsEntry cmnValsEntries[] = {
    {DP_SD0801_KEY(DP_SD0801_CLK_100_MHZ, DP_SD0801_PHY_TYPE_PCIE, DP_SD0801_PHY_TYPE_DP, DP_SD0801_NO_SSC), NULL},
    {DP_SD0801_KEY(DP_SD0801_CLK_100_MHZ, DP_SD0801_PHY_TYPE_DP, DP_SD0801_PHY_TYPE_PCIE, DP_SD0801_NO_SSC), &dp100NoSscCmnVals},

    {DP_SD0801_KEY(DP_SD0801_CLK_100_MHZ, DP_SD0801_PHY_TYPE_USB, DP_SD0801_PHY_TYPE_DP, DP_SD0801_NO_SSC), &usb100NoSscCmnVals},
    {DP_SD0801_KEY(DP_SD0801_CLK_100_MHZ, DP_SD0801_PHY_TYPE_DP, DP_SD0801_PHY_TYPE_USB, DP_SD0801_NO_SSC), &slDp100NoSscCmnVals},
};

static struct phyRegValsEntry txLnValsEntries[] = {
    {DP_SD0801_KEY(DP_SD0801_CLK_100_MHZ, DP_SD0801_PHY_TYPE_PCIE, DP_SD0801_PHY_TYPE_DP, DP_SD0801_NO_SSC), NULL},
    {DP_SD0801_KEY(DP_SD0801_CLK_100_MHZ, DP_SD0801_PHY_TYPE_DP, DP_SD0801_PHY_TYPE_PCIE, DP_SD0801_NO_SSC), &dp100NoSscTxLnVals},

    {DP_SD0801_KEY(DP_SD0801_CLK_100_MHZ, DP_SD0801_PHY_TYPE_USB, DP_SD0801_PHY_TYPE_DP, DP_SD0801_NO_SSC), &usb100NoSscTxLnVals},
    {DP_SD0801_KEY(DP_SD0801_CLK_100_MHZ, DP_SD0801_PHY_TYPE_DP, DP_SD0801_PHY_TYPE_USB, DP_SD0801_NO_SSC), &dp100NoSscTxLnVals},
};

static struct phyRegValsEntry rxLnValsEntries[] = {
    {DP_SD0801_KEY(DP_SD0801_CLK_100_MHZ, DP_SD0801_PHY_TYPE_PCIE, DP_SD0801_PHY_TYPE_DP, DP_SD0801_NO_SSC), &pcie100NoSscRxLnVals},
    {DP_SD0801_KEY(DP_SD0801_CLK_100_MHZ, DP_SD0801_PHY_TYPE_DP, DP_SD0801_PHY_TYPE_PCIE, DP_SD0801_NO_SSC), &dp100NoSscRxLnVals},

    {DP_SD0801_KEY(DP_SD0801_CLK_100_MHZ, DP_SD0801_PHY_TYPE_USB, DP_SD0801_PHY_TYPE_DP, DP_SD0801_NO_SSC), &usb100NoSscRxLnVals},
    {DP_SD0801_KEY(DP_SD0801_CLK_100_MHZ, DP_SD0801_PHY_TYPE_DP, DP_SD0801_PHY_TYPE_USB, DP_SD0801_NO_SSC), &dp100NoSscRxLnVals},
};

static const struct phyRegValsData TiJ721ePhyInitData = {
    .linkCmnValsTbl = {
        .entries = linkCmnValsEntries,
        .numEntries = ARRAY_SIZE(linkCmnValsEntries),
    },
    .xcvrDiagValsTbl = {
        .entries = xcvrDiagValsEntries,
        .numEntries = ARRAY_SIZE(xcvrDiagValsEntries),
    },
    .phyPcsCmnValsTbl = {
        .entries = phyPcsCmnValsEntries,
        .numEntries = ARRAY_SIZE(phyPcsCmnValsEntries),
    },
    .cmnValsTbl = {
        .entries = cmnValsEntries,
        .numEntries = ARRAY_SIZE(cmnValsEntries),
    },
    .txLnValsTbl = {
        .entries = txLnValsEntries,
        .numEntries = ARRAY_SIZE(txLnValsEntries),
    },
    .rxLnValsTbl = {
        .entries = rxLnValsEntries,
        .numEntries = ARRAY_SIZE(rxLnValsEntries),
    },
};

/**
 * Select appropriate register values table based on key
 */
static struct phyRegVals *sd0801PhyGetTblVals(const struct phyRegValsTable* tbl,
                                              DP_SD0801_PhyRefClk           refClk,
                                              DP_SD0801_PhyType             link0,
                                              DP_SD0801_PhyType             link1,
                                              DP_SD0801_PhySscMode          ssc)
{
    uint32_t i;
    /* Calculate key based on input parameters */
/* parasoft-begin-suppress MISRAC2012-RULE_12_2-a-2, "Constant used as the right-hand operand of a shift operator shall be limited", DRV-6294 */

    uint32_t key = DP_SD0801_KEY(refClk, link0, link1, ssc);
/* parasoft-end-suppress MISRAC2012-RULE_12_2-a-2 */

    struct phyRegVals *tbl_vals = NULL;

    for (i = 0U; i < tbl->numEntries; i++) {
        if (tbl->entries[i].key == key) {
            tbl_vals = tbl->entries[i].vals;
            break;
        }
    }
    return tbl_vals;
}

static void configureLinkSpecVals(const DP_SD0801_PrivateData* pD,
                                  DP_SD0801_PhyType            phyT1,
                                  DP_SD0801_PhyType            phyT2,
                                  uint32_t                     mLaneT1,
                                  uint32_t                     numLanesT1)
{
    const struct phyRegValsData *initData = &TiJ721ePhyInitData;
    struct phyRegVals *linkCmnVals, *xcvrDiagVals;
    struct phyRegPairs *regPairs;
    uint32_t i, j, numRegs;
    uint32_t laneOffset;

    /* linkCmnVals */
    linkCmnVals = sd0801PhyGetTblVals(&initData->linkCmnValsTbl, DP_SD0801_CLK_ANY, phyT1, phyT2, DP_SD0801_ANY_SSC);
    if (linkCmnVals != NULL) {
        regPairs = linkCmnVals->regPairs;
        numRegs = linkCmnVals->numRegs;

        for (i = 0U; i < numRegs; i++) {
            afeWrite(pD, regPairs[i].addr, regPairs[i].val);
        }
    }

    /* xcvrDiagVals */
    xcvrDiagVals = sd0801PhyGetTblVals(&initData->xcvrDiagValsTbl, DP_SD0801_CLK_ANY, phyT1, phyT2, DP_SD0801_ANY_SSC);
    if (xcvrDiagVals != NULL) {
        regPairs = xcvrDiagVals->regPairs;
        numRegs = xcvrDiagVals->numRegs;

        for (i = 0U; i < numLanesT1; i++) {
            /* Bits 9 and 10 of address indicate lane number. */
            laneOffset = ((i + mLaneT1) << 9);
            for (j = 0U; j < numRegs; j++) {
                afeWrite(pD, (regPairs[j].addr | laneOffset), regPairs[j].val);
            }
        }
    }
}

static void configurePhySpecVals(const DP_SD0801_PrivateData* pD,
                                 DP_SD0801_PhyType            phyT1,
                                 DP_SD0801_PhyType            phyT2,
                                 uint32_t                     mLaneT1,
                                 uint32_t                     numLanesT1)
{
    const struct phyRegValsData *initData = &TiJ721ePhyInitData;
    struct phyRegVals *phyPcsCmnVals;
    struct phyRegPairs *regPairs;
    uint32_t i, numRegs;

    /* phyPcsCmnVals */
    phyPcsCmnVals = sd0801PhyGetTblVals(&initData->phyPcsCmnValsTbl, DP_SD0801_CLK_ANY, phyT1, phyT2, DP_SD0801_ANY_SSC);
    if (phyPcsCmnVals != NULL) {
        regPairs = phyPcsCmnVals->regPairs;
        numRegs = phyPcsCmnVals->numRegs;

        for (i = 0U; i < numRegs; i++) {
            afeWrite(pD, regPairs[i].addr, regPairs[i].val);
        }
    }
}

static void configurePmaTxLaneVals(const DP_SD0801_PrivateData* pD,
                                   DP_SD0801_PhyType            phyT1,
                                   DP_SD0801_PhyType            phyT2,
                                   uint32_t                     mLaneT1,
                                   uint32_t                     numLanesT1)
{
    const struct phyRegValsData *initData = &TiJ721ePhyInitData;
    struct phyRegPairs *regPairs;
    struct phyRegVals *txLnVals;
    uint32_t i, j, numRegs;
    uint32_t laneOffset;

    /* PMA TX lane registers configurations */
    txLnVals = sd0801PhyGetTblVals(&initData->txLnValsTbl, pD->refClk, phyT1, phyT2, DP_SD0801_NO_SSC);
    if (txLnVals != NULL) {
        regPairs = txLnVals->regPairs;
        numRegs = txLnVals->numRegs;

        for (i = 0U; i < numLanesT1; i++) {
            /* Bits 9 and 10 of address indicate lane number. */
            laneOffset = ((i + mLaneT1) << 9);
            for (j = 0U; j < numRegs; j++) {
                afeWrite(pD, (regPairs[j].addr | laneOffset), regPairs[j].val);
            }
        }
    }
}

static void configurePmaRxLaneVals(const DP_SD0801_PrivateData* pD,
                                   DP_SD0801_PhyType            phyT1,
                                   DP_SD0801_PhyType            phyT2,
                                   uint32_t                     mLaneT1,
                                   uint32_t                     numLanesT1)
{
    const struct phyRegValsData *initData = &TiJ721ePhyInitData;
    struct phyRegPairs *regPairs;
    struct phyRegVals *rxLnVals;
    uint32_t i, j, numRegs;
    uint32_t laneOffset;

    /* PMA RX lane registers configurations */
    rxLnVals = sd0801PhyGetTblVals(&initData->rxLnValsTbl, pD->refClk, phyT1, phyT2, DP_SD0801_NO_SSC);
    if (rxLnVals != NULL) {
        regPairs = rxLnVals->regPairs;
        numRegs = rxLnVals->numRegs;

        for (i = 0U; i < numLanesT1; i++) {
            /* Bits 9 and 10 of address indicate lane number. */
            laneOffset = ((i + mLaneT1) << 9);
            for (j = 0U; j < numRegs; j++) {
                afeWrite(pD, (regPairs[j].addr | laneOffset), regPairs[j].val);
            }
        }
    }
}

static void configurePmaSpecVals(const DP_SD0801_PrivateData* pD,
                                 DP_SD0801_PhyType            phyT1,
                                 DP_SD0801_PhyType            phyT2,
                                 uint32_t                     mLaneT1,
                                 uint32_t                     numLanesT1)
{
    const struct phyRegValsData *initData = &TiJ721ePhyInitData;
    struct phyRegPairs *regPairs;
    struct phyRegVals *cmnVals;
    uint32_t i, numRegs;

    /* PMA common registers configurations */
    cmnVals = sd0801PhyGetTblVals(&initData->cmnValsTbl, pD->refClk, phyT1, phyT2, DP_SD0801_NO_SSC);
    if (cmnVals != NULL) {
        regPairs = cmnVals->regPairs;
        numRegs = cmnVals->numRegs;

        for (i = 0U; i < numRegs; i++) {
            afeWrite(pD, regPairs[i].addr, regPairs[i].val);
        }
    }

    /* PMA lane registers configurations */
    configurePmaTxLaneVals(pD, phyT1, phyT2, mLaneT1, numLanesT1);
    configurePmaRxLaneVals(pD, phyT1, phyT2, mLaneT1, numLanesT1);
}

/**
 * PHY configuration for multi-link operation is done in two steps.
 * e.g. Consider a case for a 4 lane PHY with PCIe using 2 lanes and DP other 2 lanes.
 * Torrent PHY has 2 PLLs, viz. PLL0 and PLL1. So in this case, PLL0 is used for PCIe
 * and PLL1 is used for DP. PHY is configured in two steps as described below.
 *
 * [1] For first step, phyT1 = TYPE_PCIE and phyT2 = TYPE_DP
 *     So the register values are selected as [TYPE_PCIE][TYPE_DP][ssc].
 *     This will configure PHY registers associated for PCIe (i.e. first protocol)
 *     involving PLL0 registers and registers for first 2 lanes of PHY.
 * [2] In second step, the variables phyT1 and phyT2 are swapped. So now,
 *     phyT1 = TYPE_DP and phyT2 = TYPE_PCIE. And the register values are selected as
 *     [TYPE_DP][TYPE_PCIE][ssc].
 *     This will configure PHY registers associated for DP (i.e. second protocol)
 *     involving PLL1 registers and registers for other 2 lanes of PHY.
 *
 * This completes the PHY configuration for multilink operation. This approach enables
 * dividing the large number of PHY register configurations into protocol specific
 * smaller groups.
 */
void mlConfigurePhyPmaCfg(const DP_SD0801_PrivateData*   pD,
                          const DP_SD0801_MlPhyInstance* phyInst1,
                          const DP_SD0801_MlPhyInstance* phyInst2)
{
    DP_SD0801_PhyType phyT1, phyT2, phyTmp;
    uint32_t n, mLane, numLanes;

    phyT1 = phyInst1->phyType;
    phyT2 = phyInst2->phyType;

    mLane = phyInst1->mLane;
    numLanes = phyInst1->numLanes;

    for (n = 0U; n < 2U; n++) {
        if (n == 1U) {
            phyTmp = phyT1;
            phyT1 = phyT2;
            phyT2 = phyTmp;

            mLane = phyInst2->mLane;
            numLanes = phyInst2->numLanes;
        }

        /* Configure link specific values */
        configureLinkSpecVals(pD, phyT1, phyT2, mLane, numLanes);

        /* Configure PHY specific values */
        configurePhySpecVals(pD, phyT1, phyT2, mLane, numLanes);

        /* Configure PMA specific values */
        configurePmaSpecVals(pD, phyT1, phyT2, mLane, numLanes);
    }
}

