/*
 * DMSC firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2017-2023, Texas Instruments Incorporated
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

#include <sleep.h>
#include <types/array_size.h>
#include <dmsc.h>
#include <soc/am65x_sr2/devices.h>
#include <lib/ioremap.h>
#include <sys-reset.h>
#include <device_pm.h>
#include <lib/trace.h>

#define WKUP_CTRL_BASE  0x43000000U

#define CTRLMMR_WKUP_MCU_WARM_RST_CTRL                  0x1817cU

static const struct sleep_mode am6_sleep_modes[] = {
	{
		.name = (char *) "WFI",
	},
};

static u8 am6_sleep_block[1];

static s32 am6_sys_reset_handler(domgrp_t domain __attribute__((unused)))
{
	struct device *dev;
	u8 trace_action = TRACE_PM_ACTION_SYSRESET;
	u32 trace_val = TRACE_PM_ACTION_SYSRESET_ERR_VAL_SUCCESS;

	/* PSC0: Disable MAIN2MCU bridge */
	dev = device_lookup(AM6_DEV_DUMMY_IP_LPSC_MCU2MAIN_VD);
	soc_device_ret_enable(dev);
	soc_device_disable(dev, SFALSE);

	/* WKUP_PSC0: Disable MCU2MAIN bridge */
	dev = device_lookup(AM6_DEV_DUMMY_IP_LPSC_MAIN2MCU_VD);
	soc_device_ret_enable(dev);
	soc_device_disable(dev, SFALSE);

	/* Issue warm reset */
	writel(0, WKUP_CTRL_BASE + CTRLMMR_WKUP_MCU_WARM_RST_CTRL);

	pm_trace(trace_action,
		 ((domain << TRACE_PM_ACTION_SYSRESET_DOMAIN_SHIFT) &
		  TRACE_PM_ACTION_SYSRESET_DOMAIN_MASK) | trace_val);

	return SUCCESS;
}

s32 dmsc_init(void)
{
	sys_reset_handler_register(am6_sys_reset_handler);

	return sleep_modes_register(am6_sleep_modes, am6_sleep_block,
				    ARRAY_SIZE(am6_sleep_modes));
}
