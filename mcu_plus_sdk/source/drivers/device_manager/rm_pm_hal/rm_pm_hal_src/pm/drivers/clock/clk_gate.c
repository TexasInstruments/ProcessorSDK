/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2015-2023, Texas Instruments Incorporated
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

#include <clk_gate.h>
#include <clk_mux.h>
#include <lib/container_of.h>
#include <lib/ioremap.h>

static sbool clk_gate_set_state(struct clk *clkp, sbool enable)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_reg *reg;
	u32 v;
	s32 err;
	sbool ret = STRUE;

	reg = container_of(clk_datap->data, const struct clk_data_reg, data);

	v = readl(reg->reg);
	if (enable) {
		v |= BIT(reg->bit);
	} else {
		v &= ~BIT(reg->bit);
	}
	err = pm_writel_verified(v, reg->reg);
	if (err != SUCCESS) {
		ret = SFALSE;
	}

	return ret;
}

static u32 clk_gate_get_state(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_parent *p;
	struct clk *clk_parentp = NULL;
	u32 ret = CLK_HW_STATE_ENABLED;

	p = clk_get_parent(clkp);
	if (p != NULL) {
		clk_parentp = clk_lookup((clk_idx_t) p->clk);
	}

	if (clk_parentp != NULL) {
		ret = clk_get_state(clk_parentp);
	} else {
		/* No parent...cannot function */
		ret = CLK_HW_STATE_DISABLED;
	}

	if (ret == CLK_HW_STATE_ENABLED) {
		const struct clk_data_reg *reg;

		/* Parent is enabled, are we gating it? */
		reg = container_of(clk_datap->data, const struct clk_data_reg,
				   data);
		if (0U == (readl(reg->reg) & BIT(reg->bit))) {
			ret = CLK_HW_STATE_DISABLED;
		}
	}

	return ret;
}

const struct clk_drv clk_gate_drv_reg = {
	.set_state	= clk_gate_set_state,
	.get_state	= clk_gate_get_state,
};
