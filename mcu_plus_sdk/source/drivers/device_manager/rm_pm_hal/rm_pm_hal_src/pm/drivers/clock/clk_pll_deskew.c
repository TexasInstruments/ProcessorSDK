/*
 * DMSC firmware
 *
 * Copyright (C) 2018-2024, Texas Instruments Incorporated
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

#include <clk_pll_deskew.h>
#include <device.h>
#include <lib/container_of.h>
#include <lib/ioremap.h>
#include <clk_pll.h>
#include <clk_mux.h>
#include <div64.h>
#include <compiler.h>
#include <lib/trace.h>
#include <delay.h>
#include <osal/osal_clock_user.h>

#define PLL_DESKEW_PID  (idx)                   ((0x1000UL * (idx)) + 0x00UL)
#define PLL_DESKEW_CFG(idx)                     ((0x1000UL * (idx)) + 0x08UL)

#define PLL_DESKEW_LOCKKEY0(idx)                ((0x1000UL * (idx)) + 0x10UL)
#define PLL_DESKEW_LOCKKEY0_VALUE               0x68EF3490UL

#define PLL_DESKEW_LOCKKEY1(idx)                ((0x1000UL * (idx)) + 0x14UL)
#define PLL_DESKEW_LOCKKEY1_VALUE               0xD172BC5AUL

#define PLL_DESKEW_CTRL(idx)                    ((0x1000UL * (idx)) + 0x20UL)
#define PLL_DESKEW_CTRL_BYPASS_EN               BIT(31)
#define PLL_DESKEW_CTRL_BYP_ON_LOCKLOSS         BIT(16)
#define PLL_DESKEW_CTRL_INTL_BYP_EN             BIT(8)
#define PLL_DESKEW_CTRL_PD_EN                   BIT(4)
#define PLL_DESKEW_CTRL_VCO_SEL                 BIT(0)

#define PLL_DESKEW_STAT(idx)                    ((0x1000UL * (idx)) + 0x24UL)
#define PLL_DESKEW_STAT_LOCK                    BIT(0)

#define PLL_DESKEW_DIV_CTRL(idx)                ((0x1000UL * (idx)) + 0x38UL)
#define PLL_DESKEW_DIV_CTRL_FB_DIV_SHIFT        12UL
#define PLL_DESKEW_DIV_CTRL_FB_DIV_MASK         (0x3UL << 12UL)
#define PLL_DESKEW_DIV_CTRL_POST_DIV_SHIFT      8UL
#define PLL_DESKEW_DIV_CTRL_POST_DIV_MASK       (0x7UL << 8UL)
#define PLL_DESKEW_DIV_CTRL_REF_DIV_SHIFT       0UL
#define PLL_DESKEW_DIV_CTRL_REF_DIV_MASK        (0x3UL << 0UL)

#define PLL_DESKEW_TEST_CTRL(idx)               ((0x1000UL * (idx)) + 0x50UL)
#define PLL_DESKEW_CAL_CTRL(idx)                ((0x1000UL * (idx)) + 0x60UL)
#define PLL_DESKEW_CAL_STAT(idx)                ((0x1000UL * (idx)) + 0x64UL)

#define PLL_DESKEW_HSDIV_CTRL(idx, n)           ((0x1000UL * (idx)) + 0x80UL + ((n) * 4UL))
#define PLL_DESKEW_HSDIV_CTRL_RESET             BIT(31)
#define PLL_DESKEW_HSDIV_CTRL_CLKOUT_EN         BIT(15)
#define PLL_DESKEW_HSDIV_CTRL_SYNC_DIS          BIT(8)
#define PLL_DESKEW_HSDIV_CTRL_HSDIV_SHIFT       0UL
#define PLL_DESKEW_HSDIV_CTRL_HSDIV_MASK        (0x7fUL << 0UL)

/*
 * \brief Check if the pllm value is valid
 *
 * \param clk The PLL clock.
 * \param pllm The multiplier value
 *
 * \return STRUE if pllm value is valid, SFALSE otherwise
 */
static sbool pll_deskew_pllm_valid(struct clk *clock_ptr UNUSED, u32 pllm, sbool is_frac UNUSED)
{
	return (pllm == 1U) || (pllm == 2U) || (pllm == 4U);
}

static sbool pll_deskew_plld_valid(struct clk *clock_ptr UNUSED, u32 plld)
{
	return (plld == 1U) || (plld == 2U) || (plld == 4U);
}

static sbool pll_deskew_clkod_valid(struct clk *clock_ptr UNUSED, u32 clkod)
{
	return (clkod == 4U) || (clkod == 8U) || (clkod == 16U) ||
	       (clkod == 32U) || (clkod == 64U) || (clkod == 128U) ||
	       (clkod == 256U) || (clkod == 512U);
}

/* Binning not yet supported/needed */
static s32 pll_deskew_bin(struct clk *clock_ptr UNUSED, u32 plld UNUSED,
			  u32 pllm UNUSED, sbool is_frac UNUSED, u32 clkod UNUSED)
{
	return 0;
}

/* Prefer higher VCO frequencies */
static u32 pll_deskew_vco_fitness(struct clk *clock_ptr UNUSED, u32 vco, sbool is_frac UNUSED)
{
	return vco;
}

static const struct pll_data pll_deskew_data = {
	.plld_max	= 4U,
	.pllm_max	= 4U,
	.pllfm_bits	= 0U,
	.clkod_max	= 512U,
	.pllm_valid	= pll_deskew_pllm_valid,
	.plld_valid	= pll_deskew_plld_valid,
	.clkod_valid	= pll_deskew_clkod_valid,
	.bin		= pll_deskew_bin,
	.vco_fitness	= pll_deskew_vco_fitness,
};

/*
 * \brief Check if the PLL VCO/DCO is locked.
 *
 * \param clk The PLL clock.
 *
 * \return STRUE if VCO/DCO is locked, SFALSE otherwise
 */
static sbool clk_pll_deskew_check_lock(struct clk *clock_ptr)
{
	const struct clk_data *clock_data;
	const struct clk_data_pll_deskew *pll;
	const struct clk_data_pll *data_pll;
	u32 stat;

	clock_data = clk_get_data(clock_ptr);
	data_pll = container_of(clock_data->data, const struct clk_data_pll,
				data);
	pll = container_of(data_pll, const struct clk_data_pll_deskew,
			   data_pll);

	stat = readl(pll->base + (u32) PLL_DESKEW_STAT(pll->idx));
	return (stat & PLL_DESKEW_STAT_LOCK) != 0UL;
}

static sbool clk_pll_deskew_wait_for_lock(struct clk *clock_ptr)
{
	u64 i;

	/*
	 * Minimum VCO input freq is 2.08MHz, and the longest a lock should
	 * be consider to be timed out after 120 cycles. That's 58.69us. Be
	 * conservative and assume each loop takes 10 cycles and we run at a
	 * max of 1GHz. That gives 5869 loop cycles. We may end up waiting
	 * longer than neccessary for timeout, but that should be ok.
	 */
	for (i = 0UL; i < 5869UL; i++) {
		if (clk_pll_deskew_check_lock(clock_ptr)) {
			break;
		}
	}

	return (i != 5869UL);
}

/*
 * \brief Query bypass state of PLL
 *
 * \param clk The PLL clock.
 *
 * \return STRUE if PLL is in bypass.
 */
static sbool clk_pll_deskew_is_bypass(struct clk *clock_ptr)
{
	const struct clk_data *clock_data;
	const struct clk_data_pll_deskew *pll;
	const struct clk_data_pll *data_pll;
	sbool ret = SFALSE;
	u32 ctrl;

	clock_data = clk_get_data(clock_ptr);
	data_pll = container_of(clock_data->data, const struct clk_data_pll,
				data);
	pll = container_of(data_pll, const struct clk_data_pll_deskew,
			   data_pll);

	/* IDLE Bypass */
	ctrl = readl(pll->base + (u32) PLL_DESKEW_CTRL(pll->idx));
	ret = (ctrl & PLL_DESKEW_CTRL_BYPASS_EN) != 0UL;

	return ret;
}

static s32 clk_pll_deskew_bypass(struct clk *clock_ptr, sbool bypass)
{
	const struct clk_data *clock_data;
	const struct clk_data_pll_deskew *pll;
	const struct clk_data_pll *data_pll;
	u32 ctrl;

	clock_data = clk_get_data(clock_ptr);
	data_pll = container_of(clock_data->data, const struct clk_data_pll,
				data);
	pll = container_of(data_pll, const struct clk_data_pll_deskew,
			   data_pll);

	ctrl = readl(pll->base + (u32) PLL_DESKEW_CTRL(pll->idx));
	if (bypass) {
		/* Enable bypass */
		ctrl |= PLL_DESKEW_CTRL_BYPASS_EN;
	} else {
		/* Disable bypass */
		ctrl &= ~PLL_DESKEW_CTRL_BYPASS_EN;
	}
	return pm_writel_verified(ctrl, pll->base + (u32) PLL_DESKEW_CTRL(pll->idx));
}

static sbool clk_pll_deskew_program_freq(struct clk				*clock_ptr,
					 const struct clk_data_pll_deskew	*pll,
					 u32					plld,
					 u32					pllm,
					 u32					clkod)
{
	sbool ret = STRUE;
	s32 err;
	u64 div_ctrl;
	u32 ctrl;
	u64 i;
	u64 pllm_val_p = (u64) pllm;
	u64 plld_val_p = (u64) plld;
	u64 clkod_val_p = (u64) clkod;

	clock_ptr->flags &= (u8) ~CLK_FLAG_CACHED;

	if (!clk_pll_deskew_is_bypass(clock_ptr)) {
		/* Put the PLL into bypass */
		err = clk_pll_deskew_bypass(clock_ptr, STRUE);
		if (err != SUCCESS) {
			ret = SFALSE;
		}
	}

	/* Program the new rate */
	ctrl = readl(pll->base + (u32) PLL_DESKEW_CTRL(pll->idx));
	div_ctrl = readl(pll->base + (u32) PLL_DESKEW_DIV_CTRL(pll->idx));

	div_ctrl &= ~PLL_DESKEW_DIV_CTRL_FB_DIV_MASK;
	switch (pllm_val_p) {
	case 1UL:
		pllm_val_p = 2UL;
		break;
	case 2UL:
		pllm_val_p = 1UL;
		break;
	case 4UL:
		pllm_val_p = 0UL;
		break;
	default:
		/* Do Nothing */
		break;
	}
	div_ctrl |= pllm_val_p << PLL_DESKEW_DIV_CTRL_FB_DIV_SHIFT;

	div_ctrl &= ~PLL_DESKEW_DIV_CTRL_REF_DIV_MASK;
	switch (plld_val_p) {
	case 1UL:
		plld_val_p = 2UL;
		break;
	case 2UL:
		plld_val_p = 1UL;
		break;
	case 4UL:
		plld_val_p = 0UL;
		break;
	default:
		/* Do Nothing */
		break;
	}
	div_ctrl |= plld_val_p << PLL_DESKEW_DIV_CTRL_REF_DIV_SHIFT;

	/* Start with vco output divider */
	if (clkod_val_p > 2UL) {
		clkod_val_p /= 2UL;
		ctrl &= ~PLL_DESKEW_CTRL_VCO_SEL;
	} else {
		ctrl |= PLL_DESKEW_CTRL_VCO_SEL;
	}

	/* Find the log2 for clkod setting */
	div_ctrl &= ~PLL_DESKEW_DIV_CTRL_POST_DIV_MASK;
	for (i = 0UL; (((u64) (2UL << i) & clkod_val_p) == 0UL); i++) {
		/* No loop action needed */
	}
	div_ctrl |= i << PLL_DESKEW_DIV_CTRL_POST_DIV_SHIFT;

	if (ret) {
		err = pm_writel_verified(ctrl, pll->base + (u32) PLL_DESKEW_CTRL(pll->idx));
		if (err != SUCCESS) {
			ret = SFALSE;
		}
	}

	if (ret) {
		ret = (((pm_writel_verified((u32) div_ctrl, pll->base + (u32) PLL_DESKEW_DIV_CTRL(pll->idx))) > 0) ? STRUE : SFALSE);
		if (err != SUCCESS) {
			ret = SFALSE;
		}
	}

	if (ret && (clock_ptr->ref_count != 0U)) {
		/* Take the PLL out of bypass */
		ret = clk_pll_deskew_wait_for_lock(clock_ptr);
		if (ret) {
			err = clk_pll_deskew_bypass(clock_ptr, SFALSE);
			if (err != SUCCESS) {
				ret = SFALSE;
			}
		}
	}

	return ret;
}

static u32 clk_pll_deskew_set_freq(struct clk *clock_ptr,
				   u32 target_hz,
				   u32 min_hz,
				   u32 max_hz,
				   sbool query, sbool *changed)
{
	const struct clk_data *pll_clk_data;
	const struct clk_data_pll_deskew *pll;
	const struct clk_data_pll *data_pll;
	u32 ctrl;
	u32 div_ctrl;
	u64 prev_pllm;
	u64 prev_plld;
	u64 prev_clkod;
	u32 pllm = 0U;
	u32 pllfm = 0U;
	u32 plld = 0U;
	u32 clkod = 0U;
	u32 freq;
	u32 parent_freq_hz;
	sbool was_bypass;

	was_bypass = clk_pll_deskew_is_bypass(clock_ptr);

	pll_clk_data = clk_get_data(clock_ptr);
	data_pll = container_of(pll_clk_data->data, const struct clk_data_pll,
				data);
	pll = container_of(data_pll, const struct clk_data_pll_deskew,
			   data_pll);

	ctrl = readl(pll->base + (u32) PLL_DESKEW_CTRL(pll->idx));
	div_ctrl = readl(pll->base + (u32) PLL_DESKEW_DIV_CTRL(pll->idx));

	/* Check current values */
	prev_pllm = (u64) div_ctrl & PLL_DESKEW_DIV_CTRL_FB_DIV_MASK;
	prev_pllm >>= PLL_DESKEW_DIV_CTRL_FB_DIV_SHIFT;
	switch (prev_pllm) {
	case 0UL:
		prev_pllm = 4UL;
		break;
	case 1UL:
		prev_pllm = 2UL;
		break;
	default:
		prev_pllm = 1UL;
		break;
	}

	prev_plld = (u64) div_ctrl & PLL_DESKEW_DIV_CTRL_REF_DIV_MASK;
	prev_plld >>= PLL_DESKEW_DIV_CTRL_REF_DIV_SHIFT;
	switch (prev_plld) {
	case 0UL:
		prev_plld = 4UL;
		break;
	case 1UL:
		prev_plld = 2UL;
		break;
	default:
		prev_plld = 1UL;
		break;
	}

	prev_clkod = (u64) div_ctrl & PLL_DESKEW_DIV_CTRL_POST_DIV_MASK;
	prev_clkod >>= PLL_DESKEW_DIV_CTRL_POST_DIV_SHIFT;
	prev_clkod = (u64) 2UL << prev_clkod;
	if ((ctrl & PLL_DESKEW_CTRL_VCO_SEL) == 0UL) {
		prev_clkod *= 2UL;
	}

	/* Compute new values */
	parent_freq_hz = clk_get_parent_freq(clock_ptr);
	freq = pll_calc(clock_ptr, &pll_deskew_data, parent_freq_hz,
			target_hz, min_hz, max_hz,
			&plld, &pllm, &pllfm, &clkod);

	*changed = STRUE;
	/* No need to do anything if they haven't changed */
	if ((plld == (u32) prev_plld) && (pllm == (u32) prev_pllm) && (clkod == (u32) prev_clkod) &&
	    !was_bypass) {
		*changed = SFALSE;
	}

	if ((freq != 0U) && !query && *changed) {
		if (!clk_pll_deskew_program_freq(clock_ptr, pll, plld, pllm, clkod)) {
			freq = 0U;
		}
	}

	return freq;
}

static u32 clk_pll_deskew_get_freq_internal(struct clk *clock_ptr)
{
	const struct clk_data *clock_data = clk_get_data(clock_ptr);
	const struct clk_data_pll_deskew *pll;
	const struct clk_data_pll *data_pll;
	u32 ret = 0U;
	u32 ctrl;
	u32 div_ctrl;
	u64 pllm;
	u64 plld;
	u64 clkod;
	u64 clkod_plld;

	data_pll = container_of(clock_data->data, const struct clk_data_pll,
				data);
	pll = container_of(data_pll, const struct clk_data_pll_deskew,
			   data_pll);
	ctrl = readl(pll->base + (u32) PLL_DESKEW_CTRL(pll->idx));
	div_ctrl = readl(pll->base + (u32) PLL_DESKEW_DIV_CTRL(pll->idx));

	pllm = (u64) div_ctrl & PLL_DESKEW_DIV_CTRL_FB_DIV_MASK;
	pllm >>= PLL_DESKEW_DIV_CTRL_FB_DIV_SHIFT;
	switch (pllm) {
	case 0UL:
		pllm = 4UL;
		break;
	case 1UL:
		pllm = 2UL;
		break;
	default:
		pllm = 1UL;
		break;
	}

	plld = (u64) div_ctrl & PLL_DESKEW_DIV_CTRL_REF_DIV_MASK;
	plld >>= PLL_DESKEW_DIV_CTRL_REF_DIV_SHIFT;
	switch (plld) {
	case 0UL:
		plld = 4UL;
		break;
	case 1UL:
		plld = 2UL;
		break;
	default:
		plld = 1UL;
		break;
	}

	clkod = (u64) div_ctrl & PLL_DESKEW_DIV_CTRL_POST_DIV_MASK;
	clkod >>= PLL_DESKEW_DIV_CTRL_POST_DIV_SHIFT;
	clkod = (u64) 2UL << clkod;
	if ((ctrl & (u32) PLL_DESKEW_CTRL_VCO_SEL) != 0U) {
		clkod *= 2UL;
	}

	clkod_plld = clkod * plld;

	if (clkod_plld != 0UL) {
		u64 parent_freq_hz;
		u64 ret64;
		u64 rem;

		/* Calculate non-fractional part */
		parent_freq_hz = (u64) clk_get_parent_freq(clock_ptr);
		ret64 = (parent_freq_hz / clkod_plld) * pllm;
		rem = (parent_freq_hz % clkod_plld) * pllm;
		if (rem > (u64) ULONG_MAX) {
			/*
			 * Remainder should always fit within 32 bits.
			 * We add this in case of a programming error
			 * or unexpected input.
			 *
			 * clkod_plld - 13 bits
			 * pllm -       3 bits
			 * total -	16 bits (should not need div64)
			 */
			ret64 += pm_div64(&rem, (u32) clkod_plld);
		} else {
			ret64 += rem / clkod_plld;
			rem = rem % clkod_plld;
		}

		if (ret64 > (u64) ULONG_MAX) {
			/* Cap overflow */
			ret = (u32) ULONG_MAX;
		} else {
			ret = (u32) ret64;
		}
	}

	return ret;
}

/*
 * \brief Return the DESKEW PLL DCO frequency.
 *
 * \param clk The DESKEW PLL clock.
 *
 * \return The DCO frequency of the DESKEW in Hz.
 */
static u32 clk_pll_deskew_get_freq(struct clk *clock_ptr)
{
	const struct clk_data *clock_data = clk_get_data(clock_ptr);
	u32 ret;

	/*
	 * If disabled, return the frequency we would be running at once
	 * we bring it out of bypass. If enabled and in bypass, return 0.
	 */
	if (clock_ptr->ref_count == 0U) {
		ret = 0U;
	} else if ((clock_ptr->flags & CLK_FLAG_CACHED) != 0U) {
		ret = soc_clock_values[clock_data->freq_idx];
	} else {
		if (clk_pll_deskew_is_bypass(clock_ptr)) {
			ret = 0U;
		} else {
			ret = clk_pll_deskew_get_freq_internal(clock_ptr);
		}
		soc_clock_values[clock_data->freq_idx] = ret;
		clock_ptr->flags |= CLK_FLAG_CACHED;
	}

	return ret;
}

static sbool clk_pll_deskew_set_state(struct clk *clock_ptr, sbool enabled)
{
	sbool ret = STRUE;

	clock_ptr->flags &= (u8) ~CLK_FLAG_CACHED;

	if (enabled) {
		ret = clk_pll_deskew_wait_for_lock(clock_ptr);
	}

	if (ret) {
		s32 err;
		err = clk_pll_deskew_bypass(clock_ptr, !enabled);
		if (err != SUCCESS) {
			ret = SFALSE;
		}
	}

	return ret;
}

/*
 * \brief Get the state of the DESKEW PLL.
 *
 * \param clk The DESKEW PLL clock.
 *
 * \return Returns HW state using the CLK_HW_STATE macro.
 */
static u32 clk_pll_deskew_get_state(struct clk *clock_ptr)
{
	struct clk *clock_parent = NULL;
	u32 ret = CLK_HW_STATE_ENABLED;

	if (clock_ptr->ref_count == 0U) {
		ret = CLK_HW_STATE_DISABLED;
	}

	/* PLLs can't function without an enabled parent */
	if (ret == CLK_HW_STATE_ENABLED) {
		const struct clk_parent *p;
		p = clk_get_parent(clock_ptr);
		if (p != NULL) {
			clock_parent = clk_lookup(p->clk);
		}
		if (clock_parent != NULL) {
			ret = clk_get_state(clock_parent);
		} else {
			ret = CLK_HW_STATE_DISABLED;
		}
	}

	if (ret == CLK_HW_STATE_ENABLED) {
		if (!clk_pll_deskew_is_bypass(clock_ptr) && !clk_pll_deskew_check_lock(clock_ptr)) {
			ret = CLK_HW_STATE_TRANSITION;
		}
	}

	return ret;
}

static s32 clk_pll_deskew_init_internal(struct clk *clock_ptr)
{
	const struct clk_data *clock_data = clk_get_data(clock_ptr);
	const struct clk_data_pll_deskew *pll;
	const struct clk_data_pll *data_pll;
	u32 ctrl;
	u32 cfg;
	u32 i;
	s32 ret = SUCCESS;

	data_pll = container_of(clock_data->data, const struct clk_data_pll,
				data);
	pll = container_of(data_pll, const struct clk_data_pll_deskew,
			   data_pll);

	/*
	 * Unlock write access. Note this register does not readback the
	 * written value.
	 */
	writel((u32) PLL_DESKEW_LOCKKEY0_VALUE, pll->base + (u32) PLL_DESKEW_LOCKKEY0(pll->idx));
	writel((u32) PLL_DESKEW_LOCKKEY1_VALUE, pll->base + (u32) PLL_DESKEW_LOCKKEY1(pll->idx));

	ctrl = readl(pll->base + (u32) PLL_DESKEW_CTRL(pll->idx));

	/* Make sure PLL is enabled */
	if ((ctrl & PLL_DESKEW_CTRL_PD_EN) == 0U) {
		ctrl |= PLL_DESKEW_CTRL_PD_EN;
		ret = pm_writel_verified(ctrl, pll->base + (u32) PLL_DESKEW_CTRL(pll->idx));
		if (ret == SUCCESS) {
			osal_delay(1U); /* Wait 1us */
		}
	}

	/* Always bypass if we lose lock */
	ctrl |= PLL_DESKEW_CTRL_BYP_ON_LOCKLOSS;

	/* Prefer glitchless bypass */
	if ((ctrl & PLL_DESKEW_CTRL_INTL_BYP_EN) != 0U) {
		ctrl |= PLL_DESKEW_CTRL_BYPASS_EN;
		ctrl &= ~PLL_DESKEW_CTRL_INTL_BYP_EN;
	}

	if (ret == SUCCESS) {
		ret = pm_writel_verified(ctrl, pll->base + (u32) PLL_DESKEW_CTRL(pll->idx));
	}

	/* Enable all HSDIV outputs */
	cfg = readl(pll->base + (u32) PLL_DESKEW_CFG(pll->idx));
	for (i = 0U; (i < 16U) && (ret == SUCCESS); i++) {
		/* Enable HSDIV output if present */
		if (((1UL << (i + 16UL)) & cfg) != 0UL) {
			ctrl = readl(pll->base + (u32) PLL_DESKEW_HSDIV_CTRL(pll->idx, i));
			ctrl |= PLL_DESKEW_HSDIV_CTRL_CLKOUT_EN;
			ret = pm_writel_verified(ctrl, pll->base + (u32) PLL_DESKEW_HSDIV_CTRL(pll->idx, i));
		}
	}

	if (ret == SUCCESS) {
		ret = pll_init(clock_ptr);
	}

	clock_ptr->flags &= (u8) ~CLK_FLAG_PWR_UP_EN;

	return ret;
}

static s32 clk_pll_deskew_init(struct clk *clock_ptr)
{
	const struct clk_data *clock_data = clk_get_data(clock_ptr);
	const struct clk_data_pll *data_pll;
	s32 ret;

	data_pll = container_of(clock_data->data, const struct clk_data_pll,
				data);

	if (pm_devgroup_is_enabled(data_pll->devgrp)) {
		ret = clk_pll_deskew_init_internal(clock_ptr);
	} else {
		ret = -EDEFER;
	}

	return ret;
}

const struct clk_drv clk_drv_pll_deskew = {
	.init		= clk_pll_deskew_init,
	.get_freq	= clk_pll_deskew_get_freq,
	.set_freq	= clk_pll_deskew_set_freq,
	.get_state	= clk_pll_deskew_get_state,
	.set_state	= clk_pll_deskew_set_state,
};
