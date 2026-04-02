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

#include <clk_dev.h>
#include <lib/container_of.h>
#include <device.h>
#include <device_pm.h>
#include <device_clk.h>
#include <clk.h>

static sbool clk_from_device_notify_freq(struct clk	*clkp __attribute__(
						 (unused)),
					 u32		parent_freq
					 __attribute__(
						 (unused)),
					 sbool		query __attribute__(
						 (unused)))
{
	return SFALSE;
}

/* It might be useful to prevent the device from disabling in this case */
static sbool clk_from_device_set_state(struct clk	*clkp __attribute__(
					       (unused)),
				       sbool		enabled __attribute__(
					       (unused)))
{
	return STRUE;
}

static u32 clk_from_device_get_state(struct clk *clkp)
{
	const struct clk_data *clk_datap = clk_get_data(clkp);
	const struct clk_data_from_dev *from_device;
	struct device *dev;
	u32 ret;

	from_device = container_of(clk_datap->data,
				   const struct clk_data_from_dev, data);

	dev = device_lookup(from_device->dev);

	if ((NULL == dev) || (0U == dev->initialized)) {
		ret = CLK_HW_STATE_DISABLED;
	} else {
		u32 state;

		state = device_get_state(dev);
		if (state == 0U) {
			ret = CLK_HW_STATE_DISABLED;
		} else if (state == 2U) {
			ret = CLK_HW_STATE_TRANSITION;
		} else {
			struct dev_clk *dev_clkp;

			dev_clkp = get_dev_clk(dev, from_device->clk_idx);
			if (!dev_clkp) {
				ret = CLK_HW_STATE_DISABLED;
			} else if ((dev_clkp->flags & DEV_CLK_FLAG_DISABLE) != 0U) {
				ret = CLK_HW_STATE_DISABLED;
			} else {
				ret = CLK_HW_STATE_ENABLED;
			}
		}
	}

	return ret;
}

const struct clk_drv clk_drv_from_device = {
	.get_freq	= clk_value_get_freq,
	.notify_freq	= clk_from_device_notify_freq,
	.set_state	= clk_from_device_set_state,
	.get_state	= clk_from_device_get_state,
};
