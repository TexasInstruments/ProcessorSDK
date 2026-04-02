/*
 * DM Stub Firmware
 *
 * VIM Raw driver for direct interrupt manipulation
 *
 * Copyright (C) 2021-2024, Texas Instruments Incorporated
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

#include <lib/bitops.h>
#include <baseaddress.h>

#include "lpm_io.h"
#include "vim_raw.h"

#define NUM_INTR_PER_GRP        32U
#define VGRP(i)                 (i / NUM_INTR_PER_GRP)
#define VINTR(i)                (i & (NUM_INTR_PER_GRP - 1U))

#define VIM_IRQ_VECTOR_ADDRESS  0x18U
#define VIM_ACTIVE_IRQ          0x20U
#define VIM_GRP_STS_CLEAR(i)    (0x400U + (VGRP(i) * 0x20U) + 0x04U)
#define VIM_GRP_EN_SET(i)       (0x400U + (VGRP(i) * 0x20U) + 0x08U)
#define VIM_GRP_EN_CLEAR(i)     (0x400U + (VGRP(i) * 0x20U) + 0x0cU)
#define VIM_GRP_INTR_STAT(i)    (0x400U + (VGRP(i) * 0x20U) + 0x00U)

/* VIM_ACTIVE_IRQ register */
#define VIM_ACTIVE_IRQ_VALID    BIT(31)
#define VIM_ACTIVE_IRQ_NUM_MASK 0x3FFU

void vim_set_intr_enable(u32 intr, int enable)
{
	u32 mask = ((u32) 1U << VINTR(intr));

	if (enable == INTR_ENABLE) {
		writel(mask, VIM_BASE + VIM_GRP_EN_SET(intr));
	} else {
		writel(mask, VIM_BASE + VIM_GRP_EN_CLEAR(intr));
	}
}

u32 vim_get_intr_status(u32 intr)
{
	u32 val = 0;
	u32 mask = ((u32) 1U << VINTR(intr));

	val = readl(VIM_BASE + VIM_GRP_INTR_STAT(intr));

	return val & mask;
}

void vim_clear_intr(u32 intr)
{
	writel((u32) 1U << VINTR(intr), VIM_BASE + VIM_GRP_STS_CLEAR(intr));
}

s32 vim_get_intr_number(void)
{
	u32 val;
	s32 ret = -EINVAL;

	readl(VIM_BASE + VIM_IRQ_VECTOR_ADDRESS);
	val = readl(VIM_BASE + VIM_ACTIVE_IRQ);

	if ((val & VIM_ACTIVE_IRQ_VALID) == VIM_ACTIVE_IRQ_VALID) {
		ret = val & VIM_ACTIVE_IRQ_NUM_MASK;
	}

	return ret;
}

void vim_irq_complete(void)
{
	writel(0, VIM_BASE + VIM_IRQ_VECTOR_ADDRESS);
}
