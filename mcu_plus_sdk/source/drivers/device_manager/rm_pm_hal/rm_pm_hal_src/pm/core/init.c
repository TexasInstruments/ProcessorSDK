/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2014-2020, Texas Instruments Incorporated
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

#include <types/errno.h>
#include <stddef.h>
#include <types/sbool.h>
#include <startup.h>
#include <lib/trace.h>
#include <types/array_size.h>

#include <clk.h>
#include <device.h>
#include <tisci/pm/tisci_pm_clock.h>
#include <tisci/pm/tisci_pm_core.h>
#include <tisci/pm/tisci_pm_device.h>

#include <dmsc.h>

static s32(*const startups[]) (void) = {
	&dmsc_init,
#ifdef CONFIG_CLOCK
	&clk_init,
#endif
	&devices_init,
#ifdef CONFIG_TESTS
	&test_timer64p_init,
#endif
};

s32 pm_init(void)
{
	sbool startup_done[ARRAY_SIZE(startups)];
	sbool done;
	sbool progress;
	sbool errors;
	u32 i;
	s32 ret = -EFAIL;

	for (i = 0; i < ARRAY_SIZE(startups); i++) {
		startup_done[i] = SFALSE;
	}

	errors = SFALSE;
	do {
		done = STRUE;
		progress = SFALSE;
		for (i = 0; i < ARRAY_SIZE(startups); i++) {
			s32 st_ret;
			if (startup_done[i]) {
				continue;
			}
			st_ret = (*startups[i])();
			if (st_ret == -EDEFER) {
				pm_trace(TRACE_PM_ACTION_FAIL | TRACE_PM_ACTION_INIT,
					 ((((u32) EDEFER) & TRACE_PM_VAL_INIT_ERR_MASK) << TRACE_PM_VAL_INIT_ERR_SHIFT) |
					 ((i & TRACE_PM_VAL_INIT_IDX_MASK) << TRACE_PM_VAL_INIT_IDX_SHIFT));
				done = SFALSE;
			} else {
				progress = STRUE;
				if (st_ret < 0) {
					pm_trace(TRACE_PM_ACTION_FAIL | TRACE_PM_ACTION_INIT,
						 ((((u32) (-st_ret)) & TRACE_PM_VAL_INIT_ERR_MASK) << TRACE_PM_VAL_INIT_ERR_SHIFT) |
						 ((i & TRACE_PM_VAL_INIT_IDX_MASK) << TRACE_PM_VAL_INIT_IDX_SHIFT));
					errors = STRUE;
				}
				startup_done[i] = STRUE;
			}
		}
	} while (!done && progress);

	if (!progress) {
		pm_trace(TRACE_PM_ACTION_FAIL | TRACE_PM_ACTION_INIT, (u32) TRACE_PM_VAL_INIT_DEFER);
	} else if (errors) {
		pm_trace(TRACE_PM_ACTION_FAIL | TRACE_PM_ACTION_INIT, (u32) TRACE_PM_VAL_INIT_ERROR);
	} else {
		pm_trace(TRACE_PM_ACTION_INIT, 0x0U);
		ret = SUCCESS;
	}

	return ret;
}
