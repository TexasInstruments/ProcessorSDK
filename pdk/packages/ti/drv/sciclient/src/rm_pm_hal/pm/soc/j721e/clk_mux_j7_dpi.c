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

#include <soc/j721e/clk_mux_j7_dpi.h>
#include <lib/container_of.h>
#include <ilog.h>
#include <lib/ioremap.h>
#include <clk_mux.h>
#include <compiler.h>
#include <lib/trace.h>

#define J7_MAIN_CTRL_MMR        0x00100000U

/*
 * 1'b0 - MAIN_PLL16_HSDIV0_CLKOUT
 * 1'b1 - MAIN_PLL18_HSDIV0_CLKOUT
 */
#define J7_DSS_DISPC0_CLKSEL2   0x8328U

/*
 * 3'b000 - MAIN_PLL16_HSDIV1_CLKOUT
 * 3'b001 - MAIN_PLL17_HSDIV1_CLKOUT
 * 3'b010 - MAIN_PLL18_HSDIV1_CLKOUT
 * 3'b011 - MAIN_PLL19_HSDIV0_CLKOUT_EXTPCLKIN
 * 3'b100 - MAIN_PLL23_HSDIV0_CLKOUT_EXTPCLKIN
 * 3'b101 - MAIN_PLL23_HSDIV0_CLKOUT_EXTPCLKIN and dpi0_pclk = dpi1_pclk
 * 3'b110 - MAIN_PLL23_HSDIV0_CLKOUT_EXTPCLKIN and dpi2_pclk = MAIN_PLL19_HSDIV0
 * 3'b111 - MAIN_PLL23_HSDIV0_CLKOUT_EXTPCLKIN and dpi0_pclk = dpi1_pclk
 *                                             and dpi2_pclk = MAIN_PLL19_HSDIV0
 */
#define J7_DSS_DISPC0_CLKSEL3   0x832cU

#ifdef CONFIG_TRACE
#define TRACE_ONLY
#else
#define TRACE_ONLY __attribute__((unused))
#endif


static const struct clk_parent *clk_mux_j7_dpi_0_pclk_get_parent(struct clk *clock_ptr)
{
	const struct clk_data *clock_data = clk_get_data(clock_ptr);
	const struct clk_data_mux *mux;
	u32 v;

	mux = container_of(clock_data->data, const struct clk_data_mux, data);

	v = readl(J7_MAIN_CTRL_MMR + J7_DSS_DISPC0_CLKSEL3);

	/* Output 1 if bits 2 and 0 are set, 0 otherwise */
	v = (((v & 0x5U) == 0x5U) ? 1U : 0U);

	return ((v < mux->n) && (mux->parents[v].cdiv > 0U)) ? &mux->parents[v] : NULL;
}

static sbool clk_mux_j7_dpi_0_pclk_set_parent(struct clk *clock_ptr	TRACE_ONLY,
					      u8			new_parent)
{
	u32 v;
	sbool ret;
	s32 err;

	v = readl(J7_MAIN_CTRL_MMR + J7_DSS_DISPC0_CLKSEL3);

	if ((v & 0x4UL) == 0UL) {
		if (new_parent == 1U) {
			/* Cannot switch to 1 with out changing other clocks */
			ret = SFALSE;
		} else if (new_parent == 0U) {
			/* Already on parent 0, nothing to do */
			ret = STRUE;
		} else {
			ret = SFALSE;
		}
	} else {
		if (new_parent == 1U) {
			v |= 0x1U;
			ret = STRUE;
		} else if (new_parent == 0U) {
			v &= ~0x1U;
			ret = STRUE;
		} else {
			ret = SFALSE;
		}
		if (ret) {
			err = pm_writel_verified(v, J7_MAIN_CTRL_MMR + J7_DSS_DISPC0_CLKSEL3);
			if (err != SUCCESS) {
				ret = SFALSE;
			}
		}
	}

	if (ret) {
		pm_trace(TRACE_PM_ACTION_CLOCK_SET_PARENT,
			 (((u32) new_parent << TRACE_PM_VAL_CLOCK_VAL_SHIFT) &
			  TRACE_PM_VAL_CLOCK_VAL_MASK) |
			 (((u32) clk_id(clock_ptr) << TRACE_PM_VAL_CLOCK_ID_SHIFT) &
			  TRACE_PM_VAL_CLOCK_ID_MASK));
	}

	return ret;
}

const struct clk_drv_mux clk_drv_mux_j7_dpi_0_pclk = {
	.get_parent		= clk_mux_j7_dpi_0_pclk_get_parent,
	.set_parent		= clk_mux_j7_dpi_0_pclk_set_parent,
};

static const struct clk_parent *clk_mux_j7_dpi_2_pclk_get_parent(struct clk *clock_ptr)
{
	const struct clk_data *clock_data = clk_get_data(clock_ptr);
	const struct clk_data_mux *mux;
	u32 v;

	mux = container_of(clock_data->data, const struct clk_data_mux, data);

	v = readl(J7_MAIN_CTRL_MMR + J7_DSS_DISPC0_CLKSEL3);

	/*
	 * Our mux is expanded to include the parent mux, input 0 is the
	 * parent mux. We shift input 1 to input 2
	 */
	if ((v & 0x6UL) == 0x6UL) {
		/* Output 1 if bits 2 and 1 are set, 0 otherwise */
		v = 2U;
	} else {
		v = readl(J7_MAIN_CTRL_MMR + J7_DSS_DISPC0_CLKSEL2);
		if ((v & 0x1UL) == 0x1UL) {
			v = 1U;
		} else {
			v = 0U;
		}
	}

	return ((v < mux->n) && (mux->parents[v].cdiv > 0U)) ? &mux->parents[v] : NULL;
}

static sbool clk_mux_j7_dpi_2_pclk_set_parent(struct clk *clock_ptr	TRACE_ONLY,
					      u8			new_parent)
{
	u32 v;
	sbool ret = SFALSE;
	sbool switch_parent = SFALSE;
	s32 err;

	v = readl(J7_MAIN_CTRL_MMR + J7_DSS_DISPC0_CLKSEL3);

	if ((v & 0x4UL) == 0UL) {
		if (new_parent == 2U) {
			/* Cannot switch to 2 with out changing other clocks */
			ret = SFALSE;
		} else if ((new_parent == 0U) || (new_parent == 1U)) {
			ret = STRUE;
			switch_parent = STRUE;
		} else {
			ret = SFALSE;
		}
	} else {
		if (new_parent == 2U) {
			v |= 0x2U;
			ret = STRUE;
		} else if ((new_parent == 0U) || (new_parent == 1U)) {
			v &= ~0x2U;
			ret = STRUE;
			switch_parent = STRUE;
		} else {
			ret = SFALSE;
		}
		if (ret) {
			err = pm_writel_verified(v, J7_MAIN_CTRL_MMR + J7_DSS_DISPC0_CLKSEL3);
			if (err != SUCCESS) {
				ret = SFALSE;
			}
		}
	}

	if (switch_parent && ret) {
		v = readl(J7_MAIN_CTRL_MMR + J7_DSS_DISPC0_CLKSEL2);
		if (new_parent == 1U) {
			v |= 1U;
		} else {
			v &= ~1U;
		}
		err = pm_writel_verified(v, J7_MAIN_CTRL_MMR + J7_DSS_DISPC0_CLKSEL2);
		if (err != SUCCESS) {
			ret = SFALSE;
		}
	}

	if (ret) {
		pm_trace(TRACE_PM_ACTION_CLOCK_SET_PARENT,
			 (((u32) new_parent << TRACE_PM_VAL_CLOCK_VAL_SHIFT) &
			  TRACE_PM_VAL_CLOCK_VAL_MASK) |
			 (((u32) clk_id(clock_ptr) << TRACE_PM_VAL_CLOCK_ID_SHIFT) &
			  TRACE_PM_VAL_CLOCK_ID_MASK));
	}

	return ret;
}

const struct clk_drv_mux clk_drv_mux_j7_dpi_2_pclk = {
	.get_parent		= clk_mux_j7_dpi_2_pclk_get_parent,
	.set_parent		= clk_mux_j7_dpi_2_pclk_set_parent,
};

static const struct clk_parent *clk_mux_j7_dpi_3_pclk_get_parent(struct clk *clock_ptr)
{
	const struct clk_data *clock_data = clk_get_data(clock_ptr);
	const struct clk_data_mux *mux;
	const struct clk_data_mux_reg *reg;
	u32 v, temp;

	mux = container_of(clock_data->data, const struct clk_data_mux, data);
	reg = container_of(mux, const struct clk_data_mux_reg, data_mux);

	v = readl(reg->reg);
	v >>= reg->bit;
	temp = mux->n - 1U;
	v &= ((u32) (1U << ilog32(temp)) - 1U);

	/* Output 4 if bits 2 is set, low bits otherwise */
	if (v > 4UL) {
		v = 4U;
	}

	return ((v < mux->n) && (mux->parents[v].cdiv > 0U)) ? &mux->parents[v] : NULL;
}

static sbool clk_mux_j7_dpi_3_pclk_set_parent(struct clk *clock_ptr, u8 new_parent)
{
	const struct clk_data *clock_data = clk_get_data(clock_ptr);
	const struct clk_data_mux *mux;
	const struct clk_data_mux_reg *reg;
	u32 v, temp;
	u32 curr_parent;
	sbool ret;
	sbool change = STRUE;
	u32 mask;
	s32 err;

	mux = container_of(clock_data->data, const struct clk_data_mux, data);
	reg = container_of(mux, const struct clk_data_mux_reg, data_mux);

	temp = mux->n - 1U;
	mask = (u32) (((u32) (1U << ilog32(temp)) - 1U) << reg->bit);
	v = readl(reg->reg);
	curr_parent = (v & mask) >> reg->bit;

	if (new_parent < 4U) {
		if ((curr_parent & 0x4UL) == 0UL) {
			/*
			 * Original parent is 0-3, new parent is 0-3
			 * No special action needed
			 */
			ret = STRUE;
		} else {
			/* Original parent is 4 */
			if ((curr_parent & 0x3UL) != 0UL) {
				/*
				 * dpi_0 or dpi_2 is using high clk, cannot
				 * switch without changing their setting
				 */
				ret = SFALSE;
			} else {
				/*
				 * We can switch to 0-3 without changing
				 * the dpi_0/dpi_2 output since they are both
				 * already low.
				 */
				ret = STRUE;
			}
		}
	} else if (new_parent == 4U) {
		if ((curr_parent & 0x4UL) == 0UL) {
			/*
			 * Original parent is 0-3. Select proper mux parennt 4.
			 * with the dpi_0/dpi_2 bits cleared to keep them from
			 * switching.
			 */
			ret = STRUE;
		} else {
			/*
			 * Original parent is 4, new parent is 4. Extra mux
			 * bits contain state of dpi_0/dpi_2, don't change
			 * them.
			 */
			change = SFALSE;
			ret = STRUE;
		}
	} else {
		/* Invalid setting */
		ret = SFALSE;
	}

	if (ret && change) {
		v &= ~mask;
		v |= (u32) new_parent << reg->bit;
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

const struct clk_drv_mux clk_drv_mux_reg_j7_dpi_3_pclk = {
	.get_parent		= clk_mux_j7_dpi_3_pclk_get_parent,
	.set_parent		= clk_mux_j7_dpi_3_pclk_set_parent,
};
