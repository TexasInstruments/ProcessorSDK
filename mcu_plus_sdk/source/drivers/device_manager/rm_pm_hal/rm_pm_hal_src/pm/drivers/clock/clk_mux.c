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

#include <clk_mux.h>
#include <lib/container_of.h>
#include <ilog.h>
#include <lib/ioremap.h>
#include <lib/trace.h>

static u32 clk_mux_get_parent_value(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_mux *mux;
	const struct clk_data_mux_reg *reg;
	u32 v;
	u32 temp;

	mux = container_of(clk_datap->data, const struct clk_data_mux, data);
	reg = container_of(mux, const struct clk_data_mux_reg, data_mux);

	/*
	 * Return parent 0 for muxes without register
	 * assignments.
	 */
	if (reg->reg == 0U) {
		v = 0;
	} else {
		v = readl(reg->reg);
		v >>= reg->bit;
		temp = mux->n - 1U;
		v &= (u32) ((1U << ilog32(temp)) - 1U);
	}

	return v;
}

static const struct clk_parent *clk_mux_get_parent(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_mux *mux;
	u32 v;

	mux = container_of(clk_datap->data, const struct clk_data_mux, data);

	v = clk_mux_get_parent_value(clkp);

	return ((v < mux->n) && (mux->parents[v].cdiv != 0U)) ?
	       &mux->parents[v] : NULL;
}

static sbool clk_mux_set_parent(struct clk *clkp, u8 new_parent)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_mux *mux;
	const struct clk_data_mux_reg *reg;
	u32 v, temp;
	sbool ret = STRUE;

	mux = container_of(clk_datap->data, const struct clk_data_mux, data);
	reg = container_of(mux, const struct clk_data_mux_reg, data_mux);

	if (reg->reg == 0U) {
		/*
		 * Ignore assignments for muxes without
		 * register assignments.
		 */
	} else {
		s32 err;
		v = readl(reg->reg);
		temp = mux->n - 1U;
		v &= ~(((1U << ilog32(temp)) - 1U) << reg->bit);
		v |= (u32) (new_parent << reg->bit);
		err = pm_writel_verified(v, reg->reg);
		if (err != SUCCESS) {
			ret = SFALSE;
		} else {
			pm_trace(TRACE_PM_ACTION_CLOCK_SET_PARENT,
				 (((u32) new_parent << TRACE_PM_VAL_CLOCK_VAL_SHIFT) &
				  TRACE_PM_VAL_CLOCK_VAL_MASK) |
				 (((u32) clk_id(clkp) << TRACE_PM_VAL_CLOCK_ID_SHIFT) &
				  TRACE_PM_VAL_CLOCK_ID_MASK));
		}
	}

	return ret;
}

#ifdef CONFIG_LPM_CLK
static s32 clk_mux_suspend_save(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_mux *mux;
	struct clk_data_mux_reg *reg;

	mux = container_of(clk_datap->data, const struct clk_data_mux, data);
	reg = container_of(mux, struct clk_data_mux_reg, data_mux);

	reg->saved_parent = clk_mux_get_parent_value(clkp);

	return 0;
}

static s32 clk_mux_resume_restore(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_mux *mux;
	const struct clk_data_mux_reg *reg;
	sbool error;
	s32 ret = SUCCESS;

	mux = container_of(clk_datap->data, const struct clk_data_mux, data);
	reg = container_of(mux, const struct clk_data_mux_reg, data_mux);

	error = clk_mux_set_parent(clkp, reg->saved_parent);
	if (error == SFALSE) {
		ret = -EFAIL;
	}

	return ret;
}
#endif

const struct clk_drv_mux clk_drv_mux_reg_ro = {
	.get_parent	= clk_mux_get_parent,
};

const struct clk_drv_mux clk_drv_mux_reg = {
	.set_parent		= clk_mux_set_parent,
	.get_parent		= clk_mux_get_parent,
#ifdef CONFIG_LPM_CLK
	.drv			= {
		.suspend_save	= clk_mux_suspend_save,
		.resume_restore = clk_mux_resume_restore,
	},
#endif
};

const struct clk_parent *clk_get_parent(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_parent *ret = NULL;

	if (clk_datap->type == CLK_TYPE_MUX) {
		const struct clk_drv_mux *mux;
		mux = container_of(clk_datap->drv, const struct clk_drv_mux,
				   drv);
		ret = mux->get_parent(clkp);
	} else {
		ret = ((clk_datap->parent.cdiv > 0U) ? &clk_datap->parent : NULL);
	}

	return ret;
}

sbool clk_set_parent(struct clk *clkp, u8 new_parent)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_drv_mux *mux_drv = NULL;
	const struct clk_data_mux *mux_data = NULL;
	const struct clk_parent *op;
	struct clk *parent;
	sbool ret = STRUE;
	sbool done = SFALSE;

	if (clk_datap->type != CLK_TYPE_MUX) {
		ret = SFALSE;
		done = STRUE;
	}

	if (!done) {
		mux_data = container_of(clk_datap->data,
					const struct clk_data_mux, data);
		if (new_parent >= mux_data->n) {
			ret = SFALSE;
			done = STRUE;
		} else if (0U == mux_data->parents[new_parent].cdiv) {
			ret = SFALSE;
			done = STRUE;
		} else {
			/* Do Nothing */
		}
	}

	if (!done) {
		mux_drv = container_of(clk_datap->drv,
				       const struct clk_drv_mux, drv);
		if (!mux_drv->set_parent) {
			ret = SFALSE;
			done = STRUE;
		}
	}

	if (!done) {
		op = mux_drv->get_parent(clkp);
		if (op && (op->clk == mux_data->parents[new_parent].clk)
		    && (op->cdiv == mux_data->parents[new_parent].cdiv)) {
			ret = STRUE;
			done = STRUE;
		}
	}

	if (!done) {
		parent = clk_lookup((clk_idx_t) mux_data->parents[new_parent].clk);
		if (!parent) {
			ret = SFALSE;
			done = STRUE;
		} else if ((clkp->flags & CLK_FLAG_INITIALIZED) == 0U) {
			ret = SFALSE;
			done = STRUE;
		} else if (clkp->ref_count == 0U) {
			/* No get neccessary */
		} else if (!clk_get(parent)) {
			ret = SFALSE;
			done = STRUE;
		} else {
			/* Do Nothing */
		}
	}

	if (!done) {
		if (!mux_drv->set_parent(clkp, new_parent)) {
			if (clkp->ref_count != 0U) {
				clk_put(parent);
			}
			ret = SFALSE;
			done = STRUE;
		}
	}

	if (!done) {
		if (op && (clkp->ref_count != 0U)) {
			struct clk *op_parent;
			op_parent = clk_lookup((clk_idx_t) op->clk);
			if (op_parent != NULL) {
				clk_put(op_parent);
			}
		}
	}

	return ret;
}
