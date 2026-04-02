/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2020, Texas Instruments Incorporated
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

#include <clk_pllctrl.h>
#include <lib/container_of.h>
#include <lib/ioremap.h>

#define PLLCTRL_PLLCTRL                 0x100U
#define PLLCTRL_PLLCTRL_PLLEN           BIT(0)
#define PLLCTRL_PLLCTRL_PLLENSRC        BIT(5)

#define PLLCTRL_PLLSTAT                 0x13cU
#define PLLCTRL_PLLSTAT_LOCK            BIT(1)

static const struct clk_parent *clk_pllctrl_mux_get_parent(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_mux *mux;
	const struct clk_data_mux_reg *reg;
	const struct clk_parent *parent = NULL;
	u32 v;

	mux = container_of(clk_datap->data, const struct clk_data_mux, data);
	reg = container_of(mux, const struct clk_data_mux_reg, data_mux);

	v = readl(reg->reg + PLLCTRL_PLLCTRL);
	if ((v & PLLCTRL_PLLCTRL_PLLENSRC) != 0U) {
		/* When set, mux is controlled by lock state of PLL */
		v = readl(reg->reg + PLLCTRL_PLLSTAT);
		if ((v & PLLCTRL_PLLSTAT_LOCK) != 0U) {
			/* PLL is locked */
			parent = &mux->parents[1];
		} else {
			/* PLL is in bypass */
			parent = &mux->parents[0];
		}
	} else {
		/* When cleaned, mux is controlled by PLLEN bit */
		if ((v & PLLCTRL_PLLCTRL_PLLEN) != 0U) {
			/* Use pll clock */
			parent = &mux->parents[1];
		} else {
			/* Use bypass clock */
			parent = &mux->parents[0];
		}
	}

	/* If div is 0, parent clock is not connected */
	if ((parent != NULL) && (parent->cdiv == 0U)) {
		parent = NULL;
	}

	return parent;
}

const struct clk_drv_mux clk_drv_pllctrl_mux_reg_ro = {
	.get_parent	= clk_pllctrl_mux_get_parent,
};
