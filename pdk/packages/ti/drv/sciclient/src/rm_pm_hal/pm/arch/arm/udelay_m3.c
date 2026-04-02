/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2015-2020, Texas Instruments Incorporated
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

#include <delay.h>
#include <clk.h>
#include <lib/ioremap.h>
#include <div64.h>
#include <device.h>
#include <resource.h>
#include <device_clk.h>
#include <arch/regs_cm3.h>

#define SYS_STCSR_COUNTFLAG     BIT(16)
#define SYS_STCSR_CLKSOURCE     BIT(2)
#define SYS_STCSR_TICKINT       BIT(1)
#define SYS_STCSR_ENABLE        BIT(0)

void udelay(u32 us)
{
	static unsigned long pmmc_ref_freq_hz;
	unsigned long long cycles;
	unsigned long v;
	unsigned long long freq;

	if (!us) {
		return;
	}

	if (!pmmc_ref_freq_hz) {
		const struct resource_clk *clk;

		clk = device_resource_clk(this_dev, 0);
		if (clk) {
			pmmc_ref_freq_hz = device_clk_get_freq(this_dev,
							       clk->clk_id);
		}
	}

	freq = pmmc_ref_freq_hz ? : 0;
	if (!freq) {
		/* Max possible is 50MHz */
		freq = FREQ_MHZ(50.0);
	}

	/* Calculate number of cycles required, rounding up */
	cycles = div64(freq * us + 999999ULL, 1000000);

	/* Use external clock source */
	v = readl(SYS_STCSR);
	v &= ~SYS_STCSR_CLKSOURCE;

	/* Don't generate an interrupt */
	v &= ~SYS_STCSR_TICKINT;

	/* Make sure counter is disabled */
	v &= ~SYS_STCSR_ENABLE;
	writel(v, SYS_STCSR);

	while (cycles) {
		/* Write number of clock cycles to count */
		if (cycles > 0x00ffffff) {
			v = 0x00ffffff;
		} else {
			v = cycles;
		}
		cycles -= v;
		writel(v, SYS_STRVR);

		/* Extra read to ensure COUNTFLAG clears */
		v = readl(SYS_STCSR);

		/* Start counting */
		v |= SYS_STCSR_ENABLE;
		writel(v, SYS_STCSR);

		/* Wait for timer to finish counting down */
		while (!(readl(SYS_STCSR) & SYS_STCSR_COUNTFLAG)) {
		}

		/* Disable counter */
		v &= ~SYS_STCSR_ENABLE;
		writel(v, SYS_STCSR);
	}
}
