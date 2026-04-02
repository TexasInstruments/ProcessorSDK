/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2015-2020, Texas Instruments Incorporated
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

#include <clk_rpll028.h>
#include <lib/container_of.h>
#include <lib/ioremap.h>
#include <delay.h>
#include <clk_mux.h>
#include <div64.h>
#include <compiler.h>

/* CTRL0 */
#define RPLL0_PLLD_SHIFT        0UL
#define RPLL0_PLLM_SHIFT        6UL
#define RPLL0_CLKOD_SHIFT       19UL
#define RPLL0_BYPASS            BIT(23)
#define RPLL0_BWADJ0_SHIFT      24UL

#define RPLL0_PLLD_MASK         0x0000003fUL
#define RPLL0_PLLM_MASK         0x0007ffc0UL
#define RPLL0_CLKOD_MASK        0x00780000UL
#define RPLL0_BWADJ0_MASK       0xff000000UL

/* CTRL1 */
#define RPLL1_BWADJ1_SHIFT      0UL
#define RPLL1_ENSAT             BIT(6)
#define RPLL1_PLLRST            BIT(14)
#define RPLL1_INT_BYPASS        BIT(15)

#define RPLL1_BWADJ1_MASK       0x0000000fUL


static sbool rpll028_clkod_valid(struct clk *clk __attribute__(
					 (unused)), u32 clkod)
{
	/* Only 1 and even numbers are allowed */
	return (clkod & 1UL) == 0UL || clkod == 1UL;
}

static u32 rpll028_vco_fitness(struct clk *clk UNUSED, u32 vco, sbool is_frac UNUSED)
{
	/* Prefer higher vco's, there may be a better vco fitness function */
	return vco / 1000UL;
}

static s32 rpll028_bin(struct clk *clk UNUSED, u32 plld UNUSED, u32 pllm,
		       sbool do_frac UNUSED, u32 clkod UNUSED)
{
	/*
	 * Use a pllm value below 512 if a suituble one can be found (bin 1),
	 * otherwise use a pllm value abouve 512 (bin 0).
	 */
	return pllm < 512UL ? 1UL : 0UL;
}

static u32 rpll028_bin_prev_pllm(struct clk	*clk __attribute__(
					 (unused)),
				 u32		plld __attribute__(
					 (unused)),
				 u32		pllm,
				 u32		clkod __attribute__(
					 (unused)))
{
	/*
	 * The previous pllm value in a higher bin is always 511, unless
	 * we are already below 512.
	 */
	return pllm < 512UL ? 0UL : 511UL;
}

static const struct pll_data rpll028_data = {
	.plld_max	= 64UL,
	.pllm_max	= 8192UL,
	.clkod_max	= 16UL,
	.clkod_valid	= rpll028_clkod_valid,
	.vco_fitness	= rpll028_vco_fitness,
	.bin		= rpll028_bin,
	.bin_prev_pllm	= rpll028_bin_prev_pllm,
};

static void clk_rpll028_delay(struct clk *clk, u32 cycles)
{
	const struct clk_data *clk_data = clk_get_data(clk);
	const struct clk_data_pll_rpll028 *pll;
	const struct clk_data_pll *data_pll;
	u32 v0;
	u32 freq;
	u32 delay;

	data_pll = container_of(clk_data->data, const struct clk_data_pll,
				data);
	pll = container_of(data_pll, const struct clk_data_pll_rpll028,
			   data_pll);
	v0 = readl(pll->base + 0UL);

	freq = clk_get_parent_freq(clk);
	if (freq) {
		u32 plld;
		plld = ((v0 & RPLL0_PLLD_MASK) >> RPLL0_PLLD_SHIFT) + 1UL;
		delay = (cycles * 1000000UL + freq * plld - 1UL) / (freq * plld);
	} else {
		/* Unknown freq, use safe factor */
		delay = (cycles + 9UL) / 10UL;
	}

	udelay(delay);
}

static sbool clk_rpll028_check_lock(struct clk *clk)
{
	const struct clk_data *clk_data = clk_get_data(clk);
	const struct clk_data_pll_rpll028 *pll;
	const struct clk_data_pll *data_pll;

	data_pll = container_of(clk_data->data, const struct clk_data_pll, data);
	pll = container_of(data_pll, const struct clk_data_pll_rpll028,
			   data_pll);

	/* Reset stat bit */
	writel(BIT(pll->bit), pll->eval_reg);

	/*
	 * Wait for 500 PLL input clock cycles (reference clock divided
	 * by input divider).
	 */
	clk_rpll028_delay(clk, 500UL);

	/* Check if PLL has remained lock for the last 500 ref cycles. */
	return !!(readl(pll->stat_reg) & BIT(pll->bit));
}

static sbool clk_rpll028_wait_for_lock(struct clk *clk)
{
	u32 i;

	for (i = 0UL; i < 100UL; i++) {
		if (clk_rpll028_check_lock(clk)) {
			break;
		}
	}

	/* FIXME: PLL failed to lock */
	if (i == 100UL) {
		return SFALSE;
	}

	/* Wait another 500 PLL ref clock cycles */
	clk_rpll028_delay(clk, 500UL);

	return STRUE;
}

static sbool clk_rpll028_is_bypass(struct clk *clk)
{
	const struct clk_data *clk_data = clk_get_data(clk);
	const struct clk_data_pll_rpll028 *pll;
	const struct clk_data_pll *data_pll;
	u32 v0, v1;

	data_pll = container_of(clk_data->data, const struct clk_data_pll,
				data);
	pll = container_of(data_pll, const struct clk_data_pll_rpll028,
			   data_pll);

	v0 = readl(pll->base + 0UL);
	v1 = readl(pll->base + 4UL);

	return !!(v0 & RPLL0_BYPASS) || !!(v1 & RPLL1_INT_BYPASS);
}

static sbool clk_rpll028_bypass(struct clk *clk, sbool bypass)
{
	const struct clk_data *clk_data = clk_get_data(clk);
	const struct clk_data_pll_rpll028 *pll;
	const struct clk_data_pll *data_pll;
	u32 v0, v1;
	sbool ret = STRUE;

	if (bypass == clk_rpll028_is_bypass(clk)) {
		return STRUE;
	}

	data_pll = container_of(clk_data->data, const struct clk_data_pll,
				data);
	pll = container_of(data_pll, const struct clk_data_pll_rpll028,
			   data_pll);

	v0 = readl(pll->base + 0UL);
	v1 = readl(pll->base + 4UL);

	if (bypass) {
		/* Put the PLL is in bypass */
		v0 |= RPLL0_BYPASS;
		writel(v0, pll->base + 0UL);

		/* Put PLL into reset (disable) */
		v1 |= RPLL1_PLLRST;
		writel(v1, pll->base + 4UL);

		/* PLL must remain in reset for at least 5us */
		udelay(5UL);
	} else {
		/* Make sure the PLL is in bypass */
		v0 |= RPLL0_BYPASS;
		writel(v0, pll->base + 0UL);

		/* Bring the PLL out of reset (enable) */
		v1 &= ~RPLL1_PLLRST;
		writel(v1, pll->base + 4UL);

		if (!clk_rpll028_wait_for_lock(clk)) {
			/* FIXME: PLL failed to lock */
			ret = SFALSE;
		}

		/* Disable all bypass */
		v1 &= ~RPLL1_INT_BYPASS;
		writel(v1, pll->base + 4UL);
		v0 &= ~RPLL0_BYPASS;
		writel(v0, pll->base + 0UL);
	}

	return ret;
}

static u32 clk_rpll028_set_freq(struct clk *clk,
				u32 target_hz,
				u32 min_hz,
				u32 max_hz, sbool query,
				sbool *changed)
{
	const struct clk_data *clk_data = clk_get_data(clk);
	const struct clk_data_pll_rpll028 *pll;
	const struct clk_data_pll *data_pll;
	u32 v0, v1;
	u32 prev_plld, prev_pllm, prev_clkod, prev_bwadj;
	u32 plld, pllm, clkod, bwadj;
	u32 input;
	u32 freq;
	sbool was_bypass = clk_rpll028_is_bypass(clk);

	/*
	 * FIXME: Propogating frequency change to parent may be useful if
	 * we can set the parent so the bypass may be used or set the parent
	 * based on a given static table. For now, we never propogate.
	 */

	*changed = SFALSE;

	/* Check if we can use bypass */
	input = clk_get_parent_freq(clk);
	if (input == target_hz) {
		if (!query) {
			clk_rpll028_bypass(clk, STRUE);
			clk->flags |= CLK_FLAG_PLL_BYPASS_FREQ;
		}
		*changed = !was_bypass;
		return input;
	}

	data_pll = container_of(clk_data->data, const struct clk_data_pll, data);
	pll = container_of(data_pll, const struct clk_data_pll_rpll028,
			   data_pll);

	v0 = readl(pll->base + 0UL);
	v1 = readl(pll->base + 4UL);

	/* Check current values */
	prev_plld = ((v0 & RPLL0_PLLD_MASK) >> RPLL0_PLLD_SHIFT) + 1UL;
	prev_pllm = ((v0 & RPLL0_PLLM_MASK) >> RPLL0_PLLM_SHIFT) + 1UL;
	prev_clkod = ((v0 & RPLL0_CLKOD_MASK) >> RPLL0_CLKOD_SHIFT) + 1UL;
	prev_bwadj = (v0 & RPLL0_BWADJ0_MASK) >> RPLL0_BWADJ0_SHIFT;
	prev_bwadj |= (v1 & RPLL1_BWADJ1_MASK) << 8U;
	prev_bwadj += 1UL;

	/* Compute new values */
	freq = pll_calc(clk, &rpll028_data, input, target_hz, min_hz, max_hz,
			&plld, &pllm, &clkod);
	if (!freq) {
		return 0UL;
	}

	bwadj = pllm / 2UL;

	/* No need to do anything if they haven't changed */
	if (plld == prev_plld && pllm == prev_pllm && clkod == prev_clkod &&
	    bwadj == prev_bwadj && !was_bypass) {
		return freq;
	}

	*changed = STRUE;
	if (query) {
		return freq;
	}

	clk->flags &= ~CLK_FLAG_PLL_BYPASS_FREQ;

	if (!was_bypass) {
		/* Bypass the PLL */
		v0 |= RPLL0_BYPASS;
		writel(v0, pll->base + 0UL);
	}

	/* Program the new rate */
	v0 &= ~(RPLL0_PLLD_MASK | RPLL0_PLLM_MASK | RPLL0_CLKOD_MASK |
		RPLL0_BWADJ0_MASK);
	v0 |= ((plld - 1UL) << RPLL0_PLLD_SHIFT) |
	      ((pllm - 1UL) << RPLL0_PLLM_SHIFT) |
	      ((clkod - 1UL) << RPLL0_CLKOD_SHIFT) |
	      ((bwadj - 1UL) << RPLL0_BWADJ0_SHIFT);
	v1 &= ~RPLL1_BWADJ1_MASK;
	v1 |= (bwadj - 1UL) >> 8UL;
	writel(v0, pll->base + 0UL);
	writel(v1, pll->base + 4UL);

	/* Put the PLL into reset */
	v1 |= RPLL1_PLLRST;
	writel(v1, pll->base + 4UL);

	/* We must wait at least 5us */
	udelay(5UL);

	/* Clock is currently disabled, leave in bypass */
	if (!clk->ref_count) {
		return freq;
	}

	/* Make sure internal bypass is disabled */
	v1 &= ~RPLL1_INT_BYPASS;

	/* Renable the PLL */
	v1 &= ~RPLL1_PLLRST;
	writel(v1, pll->base + 4UL);

	/* Wait for PLL to lock */
	if (!clk_rpll028_wait_for_lock(clk)) {
		/* FIXME: PLL failed to lock */
	}

	/* Disable bypass */
	v0 &= ~RPLL0_BYPASS;
	writel(v0, pll->base + 0UL);

	return freq;
}

static u32 clk_rpll028_get_freq(struct clk *clk)
{
	const struct clk_data *clk_data = clk_get_data(clk);
	const struct clk_data_pll_rpll028 *pll;
	const struct clk_data_pll *data_pll;
	u32 v0;
	u32 plld, pllm, clkod;
	u32 parent_freq_hz = clk_get_parent_freq(clk);

	/*
	 * Always return the frequency the PLL would run at if it were enabled.
	 * Only return the bypass freq if that is the desired freq.
	 */
	if (clk->flags & CLK_FLAG_PLL_BYPASS_FREQ) {
		return parent_freq_hz;
	}

	data_pll = container_of(clk_data->data, const struct clk_data_pll,
				data);
	pll = container_of(data_pll, const struct clk_data_pll_rpll028,
			   data_pll);

	v0 = readl(pll->base + 0UL);

	plld = ((v0 & RPLL0_PLLD_MASK) >> RPLL0_PLLD_SHIFT) + 1UL;
	pllm = ((v0 & RPLL0_PLLM_MASK) >> RPLL0_PLLM_SHIFT) + 1UL;
	clkod = ((v0 & RPLL0_CLKOD_MASK) >> RPLL0_CLKOD_SHIFT) + 1UL;

	return div64((u64) parent_freq_hz * pllm, plld * clkod);
}

static sbool clk_rpll028_notify_freq(struct clk *clk,
				     u32 parent_freq_hz, sbool query)
{
	if (!clk_notify_children_freq(clk, parent_freq_hz, STRUE)) {
		return SFALSE;
	}

	if (query) {
		return STRUE;
	}

	/* Just drop into bypass to be safe */
	clk_rpll028_bypass(clk, STRUE);
	clk->flags |= CLK_FLAG_PLL_BYPASS_FREQ;

	clk_notify_children_freq(clk, parent_freq_hz, SFALSE);

	return STRUE;
}

static sbool clk_rpll028_set_state(struct clk *clk, sbool enabled)
{
	sbool bypass;

	/* Check if we can use bypass */
	bypass = !enabled || clk->flags & CLK_FLAG_PLL_BYPASS_FREQ;
	return clk_rpll028_bypass(clk, bypass);
}

static u32 clk_rpll028_get_state(struct clk *clk)
{
	const struct clk_parent *p = NULL;
	struct clk *clk_parent = NULL;
	u32 ret = CLK_HW_STATE_ENABLED;

	if (!clk->ref_count) {
		ret = CLK_HW_STATE_DISABLED;
	}

	if (ret == CLK_HW_STATE_ENABLED) {
		/* PLLs can't function without an enabled parent */
		p = clk_get_parent(clk);
		if (p) {
			clk_parent = clk_lookup((clk_idx_t) p->clk);
		}
		if (!clk_parent) {
			ret = CLK_HW_STATE_DISABLED;
		}
	}

	if (ret == CLK_HW_STATE_ENABLED) {
		ret = clk_get_state(clk_parent);
	}

	if (ret == CLK_HW_STATE_ENABLED) {
		if (clk_rpll028_is_bypass(clk)) {
			/* Return state of parent */
			ret = CLK_HW_STATE_ENABLED;
		} else if (!clk_rpll028_check_lock(clk)) {
			ret = CLK_HW_STATE_TRANSITION;
		}
	}

	return ret;
}

static s32 clk_rpll028_init(struct clk *clk)
{
	const struct clk_data *clk_data = clk_get_data(clk);
	const struct clk_data_pll_rpll028 *pll;
	const struct clk_data_pll *data_pll;
	u32 v0, v1;
	sbool bypass;
	sbool enable;

	data_pll = container_of(clk_data->data, const struct clk_data_pll, data);
	pll = container_of(data_pll, const struct clk_data_pll_rpll028,
			   data_pll);

	v0 = readl(pll->base + 0UL);
	v1 = readl(pll->base + 4UL);

	bypass = !!(v0 & RPLL0_BYPASS) || !!(v1 & RPLL1_INT_BYPASS);
	enable = !(v1 & RPLL1_PLLRST);

	/* Make sure saturation bit is set */
	v1 |= RPLL1_ENSAT;
	writel(v1, pll->base + 4UL);

	/*
	 * Only allow two states, enabled and no bypass, or disabled
	 * and bypassed
	 */
	if (!enable || bypass) {
		/* Only use external bypass */
		v0 |= RPLL0_BYPASS;
		writel(v0, pll->base + 0UL);
		v1 &= ~RPLL1_INT_BYPASS;
		writel(v1, pll->base + 4UL);

		v1 |= RPLL1_PLLRST;
		writel(v1, pll->base + 4UL);

		/* PLL must remain in reset for at least 5us */
		udelay(5UL);

		clk->flags |= CLK_FLAG_PLL_BYPASS_FREQ;
	} else {
		if (!clk_rpll028_wait_for_lock(clk)) {
			/* FIXME: PLL failed to lock */
		}
	}

	return pll_init(clk);
}

const struct clk_drv clk_drv_rpll028 = {
	.init		= clk_rpll028_init,
	.set_freq	= clk_rpll028_set_freq,
	.get_freq	= clk_rpll028_get_freq,
	.notify_freq	= clk_rpll028_notify_freq,
	.set_state	= clk_rpll028_set_state,
	.get_state	= clk_rpll028_get_state,
};
