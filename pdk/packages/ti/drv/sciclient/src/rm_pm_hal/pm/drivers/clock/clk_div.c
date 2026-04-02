/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2015-2024, Texas Instruments Incorporated
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

#include <clk_div.h>
#include <clk_mux.h>
#include <lib/container_of.h>
#include <ilog.h>
#include <lib/ioremap.h>

u32 clk_get_div(struct clk *clkp)
{
	u32 ret = 0U;
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_drv_div *divp;

	if (clk_datap->type != CLK_TYPE_DIV) {
		ret = 1U;
	} else {
		divp = container_of(clk_datap->drv, const struct clk_drv_div, drv);
		ret = divp->get_div(clkp);
	}
	return ret;
}

sbool clk_div_notify_freq(struct clk *clkp, u32 parent_freq_hz,
			  sbool query)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_div *data_div;
	const struct clk_drv_div *drv_div;
	u32 divp = clk_get_div(clkp);
	u32 i;
	sbool found = SFALSE;

	data_div = container_of(clk_datap->data, const struct clk_data_div,
				data);
	drv_div = container_of(clk_datap->drv, const struct clk_drv_div, drv);

	/* Just find a frequency that works for all children */

	/* Try the current divisor and lower frequencies */
	for (i = divp; !found && (i < (data_div->n + 1UL)); i++) {
		if (!drv_div->valid_div || drv_div->valid_div(clkp, i)) {
			found = clk_notify_children_freq(clkp,
							 parent_freq_hz / i, STRUE);
		}
	}

	/* Try larger frequencies */
	for (i = divp - 1U; !found && (i > 0UL); i--) {
		if (!drv_div->valid_div || drv_div->valid_div(clkp, i)) {
			found = clk_notify_children_freq(clkp,
							 parent_freq_hz / i, STRUE);
		}
	}

	if (found && !query) {
		if (i > divp) {
			/* Frequency going down */
			drv_div->set_div(clkp, i);
		}
		clk_notify_children_freq(clkp, parent_freq_hz / i, SFALSE);
		if (i < divp) {
			/* Frequency going up */
			drv_div->set_div(clkp, i);
		}
	}

	return found;
}

static u32 clk_div_set_freq_dyn_parent(struct clk *clkp,
				       u32 target_hz, u32 min_hz, u32 max_hz,
				       sbool query, sbool *changed)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_parent *p = clk_get_parent(clkp);
	const struct clk_data_div *data_div;
	const struct clk_drv_div *drv_div;
	u32 old_div;
	u32 best_div = 0U;
	u32 best_parent_freq = 0U;
	u32 best_min_hz = 0U;
	u32 best_max_hz = 0U;
	u32 updated_min_hz = min_hz;
	u32 updated_max_hz = max_hz;
	sbool best_changed = SFALSE;
	u32 min_delta = (u32) UINT_MAX;
	u32 i;
	u32 ret;
	struct clk *parent = NULL;
	sbool flag_break = SFALSE;

	data_div = container_of(clk_datap->data, const struct clk_data_div,
				data);
	drv_div = container_of(clk_datap->drv, const struct clk_drv_div, drv);

	/* p and clk_lookup verified by caller */
	if (p != NULL) {
		parent = clk_lookup((clk_idx_t) p->clk);
	}

	old_div = drv_div->get_div(clkp);

	for (i = 1U; (i <= data_div->n) && (p != NULL) && (min_delta != 0U); i++) {
		u32 new_target, new_min, new_max, new_parent_freq;
		u32 delta;
		u32 divider;
		sbool new_target_overflow;

		if (drv_div->valid_div && !drv_div->valid_div(clkp, i)) {
			continue;
		}

		divider = i * p->cdiv;

		/* Make sure target fits within out clock frequency type */
		if (((u32) ULONG_MAX / divider) < min_hz) {
			continue;
		}

		new_min = updated_min_hz * divider;
		new_target = target_hz * divider;
		new_max = updated_max_hz * divider;

		/* If an overflow occurs in min, we are outside the range */
		if (new_min < updated_min_hz) {
			flag_break = STRUE;
		}

		if (flag_break == SFALSE) {
			/* Cap overflow in target */
			if (new_target < target_hz) {
				if (best_div != 0U) {
					/*
					 * Already found a working combination, don't
					 * bother with target overflows.
					 */
					flag_break = STRUE;
				} else {
					new_target = ULONG_MAX;
					new_target_overflow = STRUE;
				}
			} else {
				new_target_overflow = SFALSE;
			}
		}
		if (flag_break == STRUE) {
			break;
		}

		/* Cap overflow in max */
		if (new_max < updated_max_hz) {
			new_max = (u32) ULONG_MAX;
		}
		if (parent != NULL) {
			new_parent_freq = clk_set_freq(parent, new_target, new_min,
						       new_max, STRUE, changed);
		} else {
			new_parent_freq = 0U;
		}

		if (0U == new_parent_freq) {
			continue;
		}

		/* Check that any siblings can handle the new freq */
		if (*changed && !(clk_notify_sibling_freq(clkp, parent,
							  new_parent_freq,
							  STRUE))) {
			continue;
		}

		if (new_target_overflow) {
			u32 new_parent_freq_div;
			/*
			 * The target frequency was capped to ULONG_MAX, our
			 * delta will be slightly less accurate.
			 */
			new_parent_freq_div = new_parent_freq / divider;
			if (new_parent_freq_div > target_hz) {
				delta = new_parent_freq_div - target_hz;
			} else {
				delta = target_hz - new_parent_freq_div;
			}
		} else {
			if (new_parent_freq > new_target) {
				delta = new_parent_freq - new_target;
			} else {
				delta = new_target - new_parent_freq;
			}
			delta /= divider;
		}
		if (delta >= min_delta) {
			continue;
		}

		min_delta = delta;
		best_div = i;
		best_changed = *changed;
		best_parent_freq = new_parent_freq;
		best_min_hz = new_min;
		best_max_hz = new_max;

		/*
		 * Tighten min/max to decrease search space.
		 * Any new frequency must be an improvement by at least 1Hz.
		 * Note that we stop searching when min_delta reaches zero.
		 * Ensure that subtraction for min and addition for max do
		 * not overflow.
		 */
		if (min_delta != 0U) {
			u32 hz;

			hz = target_hz - (min_delta - 1U);
			if ((hz <= target_hz) && (updated_min_hz < hz)) {
				updated_min_hz = hz;
			}

			hz = target_hz + (min_delta - 1U);
			if ((hz >= target_hz) && (updated_max_hz > hz)) {
				updated_max_hz = hz;
			}
		}
	}

	if ((best_div != 0U) && !query) {
		/* Actually program out parents */
		if (best_changed) {
			clk_set_freq(parent, best_parent_freq, best_min_hz,
				     best_max_hz, SFALSE, changed);
		}

		/* Actually program our own register */
		if (old_div != best_div) {
			drv_div->set_div(clkp, best_div);
		}

		/* Assign new rate to siblings */
		if (best_changed) {
			clk_notify_sibling_freq(clkp, parent,
						best_parent_freq, SFALSE);
		}

		*changed = best_changed || (old_div != best_div);
	}

	if (best_div != 0U) {
		ret = best_parent_freq / (best_div * p->cdiv);
	} else {
		ret = 0U;
	}

	return ret;
}

u32 clk_div_set_freq_static_parent(
	struct clk	*clkp,
	u32		target_hz,
	u32		min_hz,
	u32		max_hz,
	sbool		query,
	sbool		*changed
	__attribute__((unused)))
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_div *data_div;
	const struct clk_drv_div *drv_div;
	u32 parent_freq_hz = clk_get_parent_freq(clkp);
	u32 div0, div1;
	sbool div0_ok, div1_ok;
	u32 div0_delta, div1_delta;
	u32 div0_hz, div1_hz;
	u32 n;

	/* Calculate 2 best potential frequencies */
	div0 = parent_freq_hz / target_hz;

	data_div = container_of(clk_datap->data, const struct clk_data_div,
				data);
	drv_div = container_of(clk_datap->drv, const struct clk_drv_div, drv);

	n = data_div->n;

	/*
	 * Prevent out-of-bounds divider value. Rest of the code in the
	 * function will check if the resulting divider value is within
	 * the allowable min/max range.
	 */
	if (div0 > (n - 1U)) {
		div0 = n - 1U;
	}

	div1 = div0 + 1U;

	if (drv_div->valid_div != NULL) {
		for (; (div0 > 0UL) && !drv_div->valid_div(clkp, div0); div0--) {
			/* Step through loop until valid div is found */
		}

		for (; (div1 <= n) && !drv_div->valid_div(clkp, div1); div1++) {
			/* Step through loop until valid div is found */
		}
	}

	div0_ok = SFALSE;
	div0_hz = 0U;
	if (div0 != 0UL) {
		div0_hz = parent_freq_hz / div0;
		/* Check for in range */
		if (div0_hz <= max_hz) {
			div0_ok = STRUE;
			div0_delta = div0_hz - target_hz;
		} else {
			div0_hz = 0U;
		}
	}

	div1_ok = SFALSE;
	div1_hz = 0U;
	if (div1 <= n) {
		div1_hz = parent_freq_hz / div1;
		if ((div1_hz >= min_hz) && (div1_hz <= max_hz)) {
			div1_ok = STRUE;
			div1_delta = target_hz - div1_hz;
		} else {
			div1_hz = 0U;
		}
	}

	/* Make sure at least one of them is acceptable */
	if (div1_ok && (!div0_ok || (div1_delta < div0_delta))) {
		div0_ok = STRUE;
		div0 = div1;
		div0_hz = div1_hz;
	}

	if (div0_ok && !query) {
		/* Actually program our own register */
		drv_div->set_div(clkp, div0);
	}

	return div0_hz;
}


u32 clk_div_set_freq(struct clk *clkp, u32 target_hz,
		     u32 min_hz, u32 max_hz,
		     sbool query, sbool *changed)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_parent *p = clk_get_parent(clkp);
	u32 ret = 0;

	*changed = SFALSE;

	if (!p || !clk_lookup((clk_idx_t) p->clk)) {
		/* Cannot function without parent */
	} else if ((clk_datap->flags & CLK_DATA_FLAG_MODIFY_PARENT_FREQ) != 0U) {
		ret = clk_div_set_freq_dyn_parent(clkp, target_hz, min_hz,
						  max_hz, query, changed);
	} else {
		ret = clk_div_set_freq_static_parent(clkp, target_hz, min_hz,
						     max_hz, query, changed);
	}

	return ret;
}

u32 clk_div_get_freq(struct clk *clkp)
{
	u32 parent_freq_hz = clk_get_parent_freq(clkp);

	return parent_freq_hz / clk_get_div(clkp);
}

s32 clk_div_init(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_div *data_div;
	const struct clk_drv_div *drv_div;
	s32 ret = SUCCESS;
	sbool skip_hw_init = SFALSE;

	data_div = container_of(clk_datap->data, const struct clk_data_div,
				data);
	drv_div = container_of(clk_datap->drv, const struct clk_drv_div, drv);

	if ((clk_datap->flags & CLK_DATA_FLAG_NO_HW_REINIT) != 0U) {
		if (drv_div->get_div != NULL) {
			if (drv_div->get_div(clkp) != 1U) {
				skip_hw_init = STRUE;
			}
		}
	}

	if (skip_hw_init == SFALSE) {
		if (data_div->default_div && drv_div->set_div) {
			if (!drv_div->set_div(clkp, data_div->default_div)) {
				ret = -EINVAL;
			}
		}
	}

	return ret;
}

u32 clk_div_reg_get_div(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_div *data_div;
	const struct clk_data_div_reg *data_reg;
	u32 v;

	data_div = container_of(clk_datap->data, const struct clk_data_div,
				data);
	data_reg = container_of(data_div, const struct clk_data_div_reg,
				data_div);

	/*
	 * Return parent 0 for muxes without register
	 * assignments.
	 */
	if (data_reg->reg == 0U) {
		v = 1U;
	} else {
		u32 n = data_div->n;
		if (0U == data_reg->start_at_1) {
			n -= 1U;
		}
		v = readl(data_reg->reg) >> data_reg->bit;
		v &= (u32) ((1U << ilog32(n)) - 1U);
		if (0U == data_reg->start_at_1) {
			v += 1U;
		}
	}

	return v;
}

sbool clk_div_reg_set_div(struct clk *clkp, u32 d)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_div *data_div;
	const struct clk_data_div_reg *data_reg;
	const struct clk_drv_div *drv_div;
	sbool ret = SFALSE;
	u32 n;
	s32 err;
	u32 d_val_p = d;

	data_div = container_of(clk_datap->data, const struct clk_data_div,
				data);
	data_reg = container_of(data_div, const struct clk_data_div_reg,
				data_div);
	drv_div = container_of(clk_datap->drv, const struct clk_drv_div, drv);

	n = data_div->n;
	if ((d_val_p <= n) && (!drv_div->valid_div || drv_div->valid_div(clkp, d_val_p))) {
		u32 v;

		if (0U == data_reg->start_at_1) {
			d_val_p -= 1U;
			n -= 1U;
		}

		v = readl(data_reg->reg);
		v &= (u32) ~(((1U << (u32) ilog32(n)) - 1U) << data_reg->bit);
		v |= d_val_p << data_reg->bit;
		err = pm_writel_verified(v, (u32) data_reg->reg);
		if (err == SUCCESS) {
			ret = STRUE;
		}
	}

	return ret;
}

#ifdef CONFIG_LPM_CLK
s32 clk_div_suspend_save(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_div *data_div;
	struct clk_data_div_reg *data_reg;

	data_div = container_of(clk_datap->data, const struct clk_data_div,
				data);
	data_reg = container_of(data_div, struct clk_data_div_reg,
				data_div);

	data_reg->saved_div = clk_div_reg_get_div(clkp);

	return 0;
}

s32 clk_div_resume_restore(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_div *data_div;
	const struct clk_data_div_reg *data_reg;

	data_div = container_of(clk_datap->data, const struct clk_data_div,
				data);
	data_reg = container_of(data_div, const struct clk_data_div_reg,
				data_div);

	clk_div_reg_set_div(clkp, data_reg->saved_div);

	return SUCCESS;
}
#endif

const struct clk_drv_div clk_drv_div_reg_ro = {
	.drv			= {
		.get_freq	= clk_div_get_freq,
	},
	.get_div		= clk_div_reg_get_div,
};

const struct clk_drv_div clk_drv_div_reg = {
	.drv			= {
		.notify_freq	= clk_div_notify_freq,
		.set_freq	= clk_div_set_freq,
		.get_freq	= clk_div_get_freq,
		.init		= clk_div_init,
#ifdef CONFIG_LPM_CLK
		.suspend_save	= clk_div_suspend_save,
		.resume_restore = clk_div_resume_restore,
#endif
	},
	.set_div		= clk_div_reg_set_div,
	.get_div		= clk_div_reg_get_div,
};

u32 clk_div_reg_go_get_div(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_div *data_div;
	const struct clk_data_div_reg_go *data_reg;
	u32 v;

	data_div = container_of(clk_datap->data, const struct clk_data_div,
				data);
	data_reg = container_of(data_div, const struct clk_data_div_reg_go,
				data_div);

	/*
	 * Return parent 0 for muxes without register
	 * assignments.
	 */
	if (data_reg->reg == 0U) {
		v = 1;
	} else {
		u32 n = data_div->n;
		if (0U == data_reg->start_at_1) {
			n -= 1U;
		}
		v = readl(data_reg->reg) >> data_reg->bit;
		v &= (u32) ((1U << ilog32(n)) - 1U);
		v += 1U;
	}

	return v;
}

sbool clk_div_reg_go_set_div(struct clk *clkp, u32 d)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_div *data_div;
	const struct clk_data_div_reg_go *data_reg;
	const struct clk_drv_div *drv_div;
	sbool ret = SFALSE;
	u32 n;
	s32 err;
	u32 d_val_p = d;

	data_div = container_of(clk_datap->data, const struct clk_data_div,
				data);
	data_reg = container_of(data_div, const struct clk_data_div_reg_go,
				data_div);
	drv_div = container_of(clk_datap->drv, const struct clk_drv_div, drv);

	n = data_div->n;
	if ((d_val_p <= n) && (!drv_div->valid_div || drv_div->valid_div(clkp, d_val_p))) {
		u32 v;

		if (0U == data_reg->start_at_1) {
			d_val_p -= 1U;
			n -= 1U;
		}

		v = readl(data_reg->reg);
		v &= (u32) ~(((1U << ilog32(n)) - 1U) << data_reg->bit);
		v &= (u32) ~BIT(data_reg->go);
		v |= d_val_p << data_reg->bit;
		err = pm_writel_verified(v, data_reg->reg);
		if (err == SUCCESS) {
			ret = STRUE;

			/* Go bit registers typically do not read back */
			v |= BIT(data_reg->go);
			writel(v, (u32) data_reg->reg);
			v &= (u32) ~BIT(data_reg->go);
			writel(v, (u32) data_reg->reg);
		}
	}

	return ret;
}

const struct clk_drv_div clk_drv_div_reg_go = {
	.drv			= {
		.notify_freq	= clk_div_notify_freq,
		.set_freq	= clk_div_set_freq,
		.get_freq	= clk_div_get_freq,
		.init		= clk_div_init,
	},
	.set_div		= clk_div_reg_go_set_div,
	.get_div		= clk_div_reg_go_get_div,
};

static u32 clk_div_fixed_get_div(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_div *data_div;

	data_div = container_of(clk_datap->data, const struct clk_data_div,
				data);
	return (u32) data_div->n;
}

const struct clk_drv_div clk_drv_div_fixed = {
	.drv			= {
		.get_freq	= clk_div_get_freq,
	},
	.get_div		= clk_div_fixed_get_div,
};
