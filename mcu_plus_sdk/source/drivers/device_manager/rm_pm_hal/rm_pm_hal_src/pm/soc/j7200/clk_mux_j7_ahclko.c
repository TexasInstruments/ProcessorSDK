/*
 * DMSC firmware
 *
 * Copyright (C) 2019-2024, Texas Instruments Incorporated
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

#include <soc/j7200/clk_mux_j7_ahclko.h>
#include <lib/container_of.h>
#include <ilog.h>
#include <lib/ioremap.h>
#include <clk_mux.h>
#include <compiler.h>
#include <lib/trace.h>

#define J7ES_AUDIO_REFCLKN_CTRL_MUX_MASK        0x1FU
#define J7ES_AUDIO_REFCLKN_CTRL_DIR_BIT         BIT(15)

static const struct clk_parent *clk_mux_j7_ahclko_get_parent(struct clk *clock_ptr)
{
	const struct clk_data *clock_data = clk_get_data(clock_ptr);
	const struct clk_data_mux *mux;
	const struct clk_data_mux_reg *reg;
	u32 v;
	u32 mux_select;
	sbool pin_output;
	u32 parent;
	const struct clk_parent *ret;

	mux = container_of(clock_data->data, const struct clk_data_mux, data);
	reg = container_of(mux, const struct clk_data_mux_reg, data_mux);

	v = readl(reg->reg);
	mux_select = v & J7ES_AUDIO_REFCLKN_CTRL_MUX_MASK;
	pin_output = (v & J7ES_AUDIO_REFCLKN_CTRL_DIR_BIT) != 0UL;

	if (pin_output) {
		parent = mux_select;
	} else {
		parent = 32U;
	}

	if ((parent < mux->n) && (mux->parents[parent].cdiv != 0UL)) {
		ret = mux->parents + parent;
	} else {
		ret = NULL;
	}

	return ret;
}

static sbool clk_mux_j7_ahclko_set_parent(struct clk *clock_ptr, u8 new_parent)
{
	const struct clk_data *clock_data = clk_get_data(clock_ptr);
	const struct clk_data_mux *mux;
	const struct clk_data_mux_reg *reg;
	u32 v;
	sbool ret = STRUE;
	sbool change = SFALSE;
	u32 mux_select;
	sbool pin_output;
	s32 err;

	mux = container_of(clock_data->data, const struct clk_data_mux, data);
	reg = container_of(mux, const struct clk_data_mux_reg, data_mux);

	v = readl(reg->reg);
	mux_select = v & J7ES_AUDIO_REFCLKN_CTRL_MUX_MASK;
	pin_output = (v & J7ES_AUDIO_REFCLKN_CTRL_DIR_BIT) != 0UL;

	v &= ~J7ES_AUDIO_REFCLKN_CTRL_MUX_MASK;

	if (new_parent == 32U) {
		change = (pin_output == STRUE) || (31UL != mux_select);
		v &= ~J7ES_AUDIO_REFCLKN_CTRL_DIR_BIT;
		v |= 31U;
	} else if (new_parent < 32U) {
		change = (pin_output == SFALSE) || (new_parent != mux_select);
		v |= J7ES_AUDIO_REFCLKN_CTRL_DIR_BIT;
		v |= new_parent;
	} else {
		ret = SFALSE;
	}

	if (ret && change) {
		err = pm_writel_verified(v, reg->reg);
		if (err != SUCCESS) {
			ret = SFALSE;
		}
		pm_trace(TRACE_PM_ACTION_CLOCK_SET_PARENT,
			 (((u32) new_parent << TRACE_PM_VAL_CLOCK_VAL_SHIFT) &
			  TRACE_PM_VAL_CLOCK_VAL_MASK) |
			 (((u32) clk_id(clock_ptr) << TRACE_PM_VAL_CLOCK_ID_SHIFT) &
			  TRACE_PM_VAL_CLOCK_ID_MASK));
	}

	return ret;
}

const struct clk_drv_mux clk_drv_mux_reg_j7_ahclko = {
	.set_parent		= clk_mux_j7_ahclko_set_parent,
	.get_parent		= clk_mux_j7_ahclko_get_parent,
};
