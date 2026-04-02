/*
 * DMSC firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2023-2024, Texas Instruments Incorporated
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
#include <soc/j722s/devices.h>
#include <lib/ioremap.h>
#include <sys-reset.h>
#include <device_pm.h>

#define MCU_CTRL_BASE   0x4500000UL
#define WKUP_CTRL_BASE  0x43000000UL

#define CTRL_MMR0_PARTITION_SIZE                         0x4000

#define CTRLMMR_MCU_RST_CTRL                             0x18170U
#define CTRLMMR_MCU_RST_CTRL_SW_MCU_WARMRST_OFFSET       8U
#define CTRLMMR_MCU_RST_CTRL_SW_MCU_WARMRST_MASK         0xFU
#define CTRLMMR_MCU_RST_CTRL_SW_MCU_WARMRST_VAL          0x6U

#define CTRLMMR_WKUP_RST_CTRL                            0x18170U
#define CTRLMMR_WKUP_RST_CTRL_SW_MAIN_WARMRST_OFFSET     0U
#define CTRLMMR_WKUP_RST_CTRL_SW_MAIN_WARMRST_MASK       0xFU
#define CTRLMMR_WKUP_RST_CTRL_SW_MAIN_WARMRST_VAL        0x6U

#define CTRLMMR_RST_MAGIC_WORD                           0x1817cU

static const struct sleep_mode j722s_sleep_modes[] = {
	{
		.name = (char *) "WFI",
	},
};

static u8 j722s_sleep_block[1];

static s32 j722s_sys_reset_handler(domgrp_t domain)
{
	u32 v;
	u32 mcu_magic_word;
	s32 ret = SUCCESS;

	/*
	 * domain group is unused for this device. Do some basic sanity
	 * checking on the parameter for only valid value
	 */
	if ((domain != DOMGRP_COMPATIBILITY)) {
		ret = -EINVAL;
	}

	/* Issue warm reset */

	/*
	 * The magic word register determines the state of the MCU island.
	 * When the M4FSS is isolated from the MAIN domain, the magic word
	 * is programmed a non-zero value;
	 *
	 * Valid sequence as per reset spec:
	 * - When MCU_ISO_EN is disabled(default): ONLY register permitted is
	 *   MCU_RST_CTRL MMR (SW_MCU_WARMRESET)
	 * - When MCU_ISO_EN is enabled: Main RST_CTRL MMR is permitted
	 *   (SW_MAIN_WARMRESET)
	 */

	if (ret == SUCCESS) {
		mcu_magic_word = readl(WKUP_CTRL_BASE + CTRLMMR_RST_MAGIC_WORD);

		if (mcu_magic_word == 0U) {
			v = readl(MCU_CTRL_BASE + CTRLMMR_MCU_RST_CTRL);
			v &= ~(CTRLMMR_MCU_RST_CTRL_SW_MCU_WARMRST_MASK <<
			       CTRLMMR_MCU_RST_CTRL_SW_MCU_WARMRST_OFFSET);
			v |= CTRLMMR_MCU_RST_CTRL_SW_MCU_WARMRST_VAL <<
			     CTRLMMR_MCU_RST_CTRL_SW_MCU_WARMRST_OFFSET;
			writel(v, MCU_CTRL_BASE + CTRLMMR_MCU_RST_CTRL);
		} else {
			v = readl(WKUP_CTRL_BASE + CTRLMMR_WKUP_RST_CTRL);
			v &= ~(CTRLMMR_WKUP_RST_CTRL_SW_MAIN_WARMRST_MASK <<
			       CTRLMMR_WKUP_RST_CTRL_SW_MAIN_WARMRST_OFFSET);
			v |= CTRLMMR_WKUP_RST_CTRL_SW_MAIN_WARMRST_VAL <<
			     CTRLMMR_WKUP_RST_CTRL_SW_MAIN_WARMRST_OFFSET;
			writel(v, WKUP_CTRL_BASE + CTRLMMR_WKUP_RST_CTRL);
		}
	}

	return ret;
}

s32 dmsc_init(void)
{
	sys_reset_handler_register(j722s_sys_reset_handler);

	return sleep_modes_register(j722s_sleep_modes, j722s_sleep_block,
				    ARRAY_SIZE(j722s_sleep_modes));
}
