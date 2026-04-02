/*
 * DM Stub Firmware
 *
 * PLL Raw driver for direct PLL manipulation
 *
 * Copyright (C) 2021-2024, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <config.h>
#include <lib/bitops.h>
#include <types/errno.h>
#include <types/short_types.h>

#include "lpm_io.h"
#include "pll_16fft_raw.h"
#include "timeout.h"

/* PLL CTRL Registers */
#define PLLCTRL_RSCTRL_OFFSET                           0x0E8U
#define PLLCTRL_RSCTRL_KEY                              0x15a69U
#define PLLCTRL_RSISO_OFFSET                            0x0F0U
#define PLLCTRL_PLLCTL_OFFSET                           0x100U

/* 16FFT PLL Registers */
#define PLL_16FFT_PID_OFFSET                    ((u32) 0x00UL)

#define PLL_16FFT_CFG_OFFSET                    ((u32) 0x08UL)
#define PLL_16FFT_CFG_PLL_TYPE_SHIFT            ((u32) 0UL)
#define PLL_16FFT_CFG_PLL_TYPE_MASK             ((u32) (0x3UL << 0UL))
#define PLL_16FFT_CFG_PLL_TYPE_FRACF            1UL

#define PLL_16FFT_LOCKKEY0_OFFSET               ((u32) 0x10UL)
#define PLL_16FFT_LOCKKEY0_VALUE                ((u32) 0x68EF3490UL)

#define PLL_16FFT_LOCKKEY1_OFFSET               ((u32) 0x14UL)
#define PLL_16FFT_LOCKKEY1_VALUE                ((u32) 0xD172BC5AUL)

#define PLL_16FFT_CTRL_OFFSET                   ((u32) 0x20UL)
#define PLL_16FFT_CTRL_BYPASS_EN                BIT(31)
#define PLL_16FFT_CTRL_BYP_ON_LOCKLOSS          BIT(16)
#define PLL_16FFT_CTRL_PLL_EN                   BIT(15)
#define PLL_16FFT_CTRL_INTL_BYP_EN              BIT(8)
#define PLL_16FFT_CTRL_CLK_4PH_EN               BIT(5)
#define PLL_16FFT_CTRL_CLK_POSTDIV_EN           BIT(4)
#define PLL_16FFT_CTRL_DSM_EN                   BIT(1)
#define PLL_16FFT_CTRL_DAC_EN                   BIT(0)

#define PLL_16FFT_STAT_OFFSET                   ((u32) 0x24UL)
#define PLL_16FFT_STAT_LOCK                     BIT(0)

#define PLL_16FFT_FREQ_CTRL0_OFFSET             ((u32) 0x30UL)
#define PLL_16FFT_FREQ_CTRL0_FB_DIV_INT_SHIFT   ((u32) 0UL)
#define PLL_16FFT_FREQ_CTRL0_FB_DIV_INT_MASK    ((u32) (0xfffUL << 0UL))

#define PLL_16FFT_FREQ_CTRL1_OFFSET             ((u32) 0x34UL)
#define PLL_16FFT_FREQ_CTRL1_FB_DIV_FRAC_SHIFT  ((u32) 0UL)
#define PLL_16FFT_FREQ_CTRL1_FB_DIV_FRAC_MASK   ((u32) (0xffffffUL << 0UL))
#define PLL_16FFT_FREQ_CTRL1_FB_DIV_FRAC_BITS   ((u32) 24UL)

#define PLL_16FFT_DIV_CTRL_OFFSET               ((u32) 0x38UL)
#define PLL_16FFT_DIV_CTRL_POST_DIV2_SHIFT      ((u32) 24UL)
#define PLL_16FFT_DIV_CTRL_POST_DIV2_MASK       ((u32) (0x7UL << 24UL))
#define PLL_16FFT_DIV_CTRL_POST_DIV1_SHIFT      ((u32) 16UL)
#define PLL_16FFT_DIV_CTRL_POST_DIV1_MASK       ((u32) (0x7UL << 16UL))
#define PLL_16FFT_DIV_CTRL_REF_DIV_SHIFT        ((u32) 0UL)
#define PLL_16FFT_DIV_CTRL_REF_DIV_MASK         ((u32) (0x3fUL << 0UL))

#define PLL_16FFT_SS_CTRL_OFFSET                0x40UL
#define PLL_16FFT_SS_SPREAD_OFFSET              0x44UL

#define PLL_16FFT_CAL_CTRL_OFFSET               ((u32) 0x60UL)
#define PLL_16FFT_CAL_CTRL_CAL_EN               BIT(31)
#define PLL_16FFT_CAL_CTRL_FAST_CAL             BIT(20)
#define PLL_16FFT_CAL_CTRL_CAL_CNT_SHIFT        ((u32) 16UL)
#define PLL_16FFT_CAL_CTRL_CAL_CNT_MASK         ((u32) (0x7UL << 16UL))
#define PLL_16FFT_CAL_CTRL_CAL_BYP              BIT(15)
#define PLL_16FFT_CAL_CTRL_CAL_IN_SHIFT         ((u32) 0UL)
#define PLL_16FFT_CAL_CTRL_CAL_IN_MASK          ((u32) (0xFFFUL << 0UL))

#define PLL_16FFT_CAL_STAT_OFFSET               ((u32) 0x64UL)
#define PLL_16FFT_CAL_STAT_CAL_LOCK             BIT(31)
#define PLL_16FFT_CAL_STAT_LOCK_CNT_SHIFT       ((u32) 16UL)
#define PLL_16FFT_CAL_STAT_LOCK_CNT_MASK        ((u32) (0xFU << 16U))
#define PLL_16FFT_CAL_STAT_CAL_OUT_SHIFT        ((u32) 0UL)
#define PLL_16FFT_CAL_STAT_CAL_OUT_MASK         ((u32) (0xFFFUL))

#define PLL_16FFT_HSDIV_CTRL_OFFSET             ((u32) 0x80UL)
#define PLL_16FFT_HSDIV_CTRL_RESET              BIT(31)
#define PLL_16FFT_HSDIV_CTRL_CLKOUT_EN          BIT(15)
#define PLL_16FFT_HSDIV_CTRL_SYNC_DIS           BIT(8)
#define PLL_16FFT_HSDIV_CTRL_HSDIV_SHIFT        ((u32) 0UL)
#define PLL_16FFT_HSDIV_CTRL_HSDIV_MASK         ((u32) (0x7fUL << 0UL))

#define PLL_16FFT_RAW_LOCK_TIMEOUT              ((u32) 15000UL)
#define PLL_16FFT_CAL_LOCK_TIMEOUT              ((u32) 435000UL)

#define pll_readl readl
#define pll_writel writel

static void pll_cal_option3(u32 pll_base)
{
	u32 cal;

	cal = readl(pll_base + (u32) PLL_16FFT_CAL_CTRL_OFFSET);

	/* Enable fast cal mode */
	cal |= PLL_16FFT_CAL_CTRL_FAST_CAL;

	/* Disable calibration bypass */
	cal &= ~PLL_16FFT_CAL_CTRL_CAL_BYP;

	/* Set CALCNT to 2 */
	cal &= ~PLL_16FFT_CAL_CTRL_CAL_CNT_MASK;
	cal |= (u32) 2U << (u32) PLL_16FFT_CAL_CTRL_CAL_CNT_SHIFT;

	/* Set CAL_IN to 0 */
	cal &= ~PLL_16FFT_CAL_CTRL_CAL_IN_MASK;

	/* Note this register does not readback the written value. */
	writel(cal, pll_base + (u32) PLL_16FFT_CAL_CTRL_OFFSET);

	/* Wait 1us before enabling the CAL_EN field */
	delay_1us();

	cal = readl(pll_base + (u32) PLL_16FFT_CAL_CTRL_OFFSET);

	/* Enable calibration for FRACF */
	cal |= PLL_16FFT_CAL_CTRL_CAL_EN;

	/* Note this register does not readback the written value. */
	writel(cal, pll_base + (u32) PLL_16FFT_CAL_CTRL_OFFSET);
}

static void pll_enable(u32 pll_base, sbool enable)
{
	u32 ctrl = 0U;

	ctrl = pll_readl(pll_base + PLL_16FFT_CTRL_OFFSET);
	if (enable == STRUE) {
		ctrl |= PLL_16FFT_CTRL_PLL_EN;
	} else {
		ctrl &= ~PLL_16FFT_CTRL_PLL_EN;
	}
	pll_writel(ctrl, pll_base + PLL_16FFT_CTRL_OFFSET);
	delay_1us();
}

s32 pll_restore(struct pll_raw_data *pll)
{
	u8 i;
	u32 ctrl, cfg, timeout, cal;
	s32 ret = SUCCESS;

	/* Unlock write access */
	pll_writel(PLL_16FFT_LOCKKEY0_VALUE, pll->base + PLL_16FFT_LOCKKEY0_OFFSET);
	pll_writel(PLL_16FFT_LOCKKEY1_VALUE, pll->base + PLL_16FFT_LOCKKEY1_OFFSET);

	/* Make sure that PLL is in bypass mode */
	pll_bypass(pll, STRUE);

	/* Make sure that PLL is disabled */
	pll_enable(pll->base, SFALSE);

	/* Restore the divider values */
	pll_writel(pll->freq_ctrl0, pll->base + PLL_16FFT_FREQ_CTRL0_OFFSET);
	pll_writel(pll->freq_ctrl1, pll->base + PLL_16FFT_FREQ_CTRL1_OFFSET);
	pll_writel(pll->div_ctrl, pll->base + PLL_16FFT_DIV_CTRL_OFFSET);

	ctrl = pll_readl(pll->base + PLL_16FFT_CTRL_OFFSET);

	/* Always bypass if we lose lock */
	ctrl |= PLL_16FFT_CTRL_BYP_ON_LOCKLOSS;

	/* Prefer glitchless bypass */
	ctrl &= ~PLL_16FFT_CTRL_INTL_BYP_EN;

	/* Always enable output of PLL */
	ctrl |= PLL_16FFT_CTRL_CLK_POSTDIV_EN;

	/* Currently unused by all PLLs */
	ctrl &= ~PLL_16FFT_CTRL_CLK_4PH_EN;

	/* Make sure we have fractional support if required */
	if (pll->freq_ctrl1 != 0UL) {
		ctrl |= PLL_16FFT_CTRL_DSM_EN;
		ctrl |= PLL_16FFT_CTRL_DAC_EN;
	} else {
		ctrl &= ~PLL_16FFT_CTRL_DSM_EN;
		ctrl &= ~PLL_16FFT_CTRL_DAC_EN;
	}

	pll_writel(ctrl, pll->base + PLL_16FFT_CTRL_OFFSET);

	/* Program all HSDIV outputs */
	cfg = pll_readl(pll->base + PLL_16FFT_CFG_OFFSET);
	for (i = 0U; i < 16U; i++) {
		/* Program HSDIV output if present */
		if (((1UL << (i + 16UL)) & cfg) != 0UL) {
			pll_writel(pll->hsdiv[i], pll->base + PLL_16FFT_HSDIV_CTRL_OFFSET + (i * 0x4U));
		}
	}

	if (pll->freq_ctrl1 == 0UL) {
		/* Enable Calibration in Integer mode */
		pll_cal_option3(pll->base);
	} else {
		/* Disable Calibration in Fractional mode */
		cal = readl(pll->base + (u32) PLL_16FFT_CAL_CTRL_OFFSET);
		cal &=  (u32) ~PLL_16FFT_CAL_CTRL_CAL_EN;
		writel(cal, (u32) pll->base + (u32) PLL_16FFT_CAL_CTRL_OFFSET);
		timeout = PLL_16FFT_CAL_LOCK_TIMEOUT;
		while ((timeout > 0U) && (((pll_readl(pll->base + PLL_16FFT_CAL_STAT_OFFSET) & ((u32) PLL_16FFT_CAL_STAT_CAL_LOCK)) == 0U) == SFALSE)) {
			--timeout;
		}
		if (timeout == 0U) {
			ret = -EFAIL;
		}
	}

	if (ret == SUCCESS) {
		/*
		 * Wait at least 1 ref cycle before enabling PLL.
		 * Minimum VCO input frequency is 5MHz, therefore maximum
		 * wait time for 1 ref clock is 0.2us.
		*/
		delay_1us();

		/* Make sure PLL is enabled */
		pll_enable(pll->base, STRUE);

		/* Wait for the PLL lock */
		timeout = PLL_16FFT_RAW_LOCK_TIMEOUT;
		while ((timeout > 0U) && (((pll_readl(pll->base + PLL_16FFT_STAT_OFFSET) & ((u32) PLL_16FFT_STAT_LOCK)) == 1U) == SFALSE)) {
			--timeout;
		}
		if (timeout == 0U) {
			ret = -EFAIL;
		}
	}

	if ((pll->freq_ctrl1 == 0UL) && (ret == SUCCESS)) {
		/* Wait for calibration lock */
		timeout = PLL_16FFT_CAL_LOCK_TIMEOUT;
		while ((timeout > 0U) && (((pll_readl(pll->base + PLL_16FFT_CAL_STAT_OFFSET) & ((u32) PLL_16FFT_CAL_STAT_CAL_LOCK)) != 0U) == SFALSE)) {
			--timeout;
		}
		if (timeout == 0U) {
			ret = -EFAIL;
		}

		/* In case of cal lock failure, operate without calibration */
		if (ret != SUCCESS) {
			ret = SUCCESS;
			/* Disable PLL */
			pll_enable(pll->base, SFALSE);

			/* Disable Calibration */
			cal = readl(pll->base + (u32) PLL_16FFT_CAL_CTRL_OFFSET);
			cal &= ~PLL_16FFT_CAL_CTRL_CAL_EN;
			writel(cal, (u32) pll->base + (u32) PLL_16FFT_CAL_CTRL_OFFSET);
			timeout = PLL_16FFT_CAL_LOCK_TIMEOUT;
			while ((timeout > 0U) && (((pll_readl(pll->base + PLL_16FFT_CAL_STAT_OFFSET) & ((u32) PLL_16FFT_CAL_STAT_CAL_LOCK)) == 0U) == SFALSE)) {
				--timeout;
			}
			if (timeout == 0U) {
				ret = -EFAIL;
			}

			if (ret == SUCCESS) {
				/* Enable PLL */
				pll_enable(pll->base, STRUE);

				/* Wait for PLL Lock */
				timeout = PLL_16FFT_RAW_LOCK_TIMEOUT;
				while ((timeout > 0U) && (((pll_readl(pll->base + PLL_16FFT_STAT_OFFSET) & ((u32) PLL_16FFT_STAT_LOCK)) == 1U) == SFALSE)) {
					--timeout;
				}
				if (timeout == 0U) {
					ret = -EFAIL;
				}
			}
		}
	}

	if (ret == SUCCESS) {
		/*
		 * The PLL_CTRL BYPASS_EN should be cleared to make sure the clocks are running
		 * at the locked PLL frequency.
		 */
		pll_bypass(pll, SFALSE);
	}

	return ret;
}

void pll_save(struct pll_raw_data *pll)
{
	u8 i;
	u32 cfg;

	pll->freq_ctrl0 = pll_readl(pll->base + PLL_16FFT_FREQ_CTRL0_OFFSET);
	pll->freq_ctrl1 = pll_readl(pll->base + PLL_16FFT_FREQ_CTRL1_OFFSET);
	pll->div_ctrl = pll_readl(pll->base + PLL_16FFT_DIV_CTRL_OFFSET);

	/* Enable all HSDIV outputs */
	cfg = pll_readl(pll->base + PLL_16FFT_CFG_OFFSET);
	for (i = 0U; i < 16U; i++) {
		/* Read HSDIV output if present */
		if (((1UL << (i + 16UL)) & cfg) != 0UL) {
			pll->hsdiv[i] = pll_readl(pll->base + PLL_16FFT_HSDIV_CTRL_OFFSET + (i * 0x4U));
		}
	}
}

void pll_disable(struct pll_raw_data *pll)
{
	/* Select reference clk for PLL and HSDIV clk outputs */
	pll_bypass(pll, STRUE);

	/* Disable the PLL */
	pll_enable(pll->base, SFALSE);
}

void pll_hsdiv_ctrl(u32 pll_base, u8 hsdiv, u8 enable)
{
	u32 ctrl;

	ctrl = pll_readl(pll_base + PLL_16FFT_HSDIV_CTRL_OFFSET + (hsdiv * 0x4U));

	if (enable == 1U) {
		ctrl |= PLL_16FFT_HSDIV_CTRL_CLKOUT_EN;
	} else {
		ctrl &= ~PLL_16FFT_HSDIV_CTRL_CLKOUT_EN;
	}

	pll_writel(ctrl, pll_base + PLL_16FFT_HSDIV_CTRL_OFFSET + (hsdiv * 0x4U));
}

void pll_bypass(struct pll_raw_data *pll, sbool enable)
{
	u32 ctrl;

	ctrl = pll_readl(pll->base + PLL_16FFT_CTRL_OFFSET);
	if (enable) {
		ctrl |= PLL_16FFT_CTRL_BYPASS_EN;
	} else {
		ctrl &= ~PLL_16FFT_CTRL_BYPASS_EN;
	}
	pll_writel(ctrl, pll->base + PLL_16FFT_CTRL_OFFSET);
}
