/*
 * Device Manager - Manage wakeup domain peripherals during Suspend/Resume
 *
 * Copyright (C) 2024, Texas Instruments Incorporated
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

#include <lib/ioremap.h>
#include "soc_ctrl_mmr.h"
#include "wkup_periph.h"

/* VIM Register Offsets */
#define VIM_IRQVEC              (0x18U)
#define VIM_FIQVEC              (0x1CU)
#define VIM_IRQPRIMASK          (0x28U)
#define VIM_FIQPRIMASK          (0x2CU)
#define VIM_DEDVECMASK          (0x30U)
#define VIM_INT_EN(j)           (0x408U + (j * 0x20U))
#define VIM_INT_MAP(j)          (0x418U + (j * 0x20U))
#define VIM_INT_TYPE(j)         (0x41CU + (j * 0x20U))
#define VIM_INT_PRI             (0x1000U)
#define VIM_INT_VEC             (0x2000U)

/* Timer Register Offsets */
#define TIM_TIOCP_CFG           (0x10U)
#define TIM_EOI                 (0x20U)
#define TIM_SET                 (0x2CU)
#define TIM_IRQ_WAKE_EN         (0x34U)
#define TIM_TCLR                (0x38U)
#define TIM_TCRR                (0x3CU)
#define TIM_TLDR                (0x40U)
#define TIM_TMAR                (0x4CU)
#define TIM_TSICR               (0x54U)
#define TIM_TPIR                (0x5CU)
#define TIM_TNIR                (0x60U)
#define TIM_TCVR                (0x64U)
#define TIM_TOCR                (0x68U)
#define TIM_TCWR                (0x6CU)

static struct vim_intr vim_cfg = { 0 };
static struct timer_cfg timer_config = { 0 };

static void lpm_save_vim_registers(void)
{
	u32 i;
	u32 base_address;

	/* Save IRQ vector addr reg */
	base_address = VIM_BASE_ADDR + VIM_IRQVEC;
	vim_cfg.irq_vec = readl(base_address);

	/* Save FIQ vector addr reg */
	base_address = VIM_BASE_ADDR + VIM_FIQVEC;
	vim_cfg.fiq_vec = readl(base_address);

	/* Save IRQ PRI MASK reg */
	base_address = VIM_BASE_ADDR + VIM_IRQPRIMASK;
	vim_cfg.irq_pri_mask = readl(base_address);

	/* Save FIQ PRI MASK reg */
	base_address = VIM_BASE_ADDR + VIM_FIQPRIMASK;
	vim_cfg.fiq_pri_mask = readl(base_address);

	/* Save DED vector mask reg */
	base_address = VIM_BASE_ADDR + VIM_DEDVECMASK;
	vim_cfg.ded_vec_mask = readl(base_address);

	/* Save the interrupt enable, type and map value */
	for (i = 0; i < NUM_GRPS; i++) {
		base_address = VIM_BASE_ADDR + VIM_INT_EN(i);
		vim_cfg.grp_intr[i].int_en = readl(base_address);

		vim_cfg.grp_intr[i].int_map = readl(base_address + 16U);

		vim_cfg.grp_intr[i].int_type = readl(base_address + 20U);
	}

	/* Save interrupt priority reg */
	base_address = VIM_BASE_ADDR + VIM_INT_PRI;
	for (i = 0; i < NUM_INTERRUPTS; i++) {
		vim_cfg.intr_pri[i] = readl(base_address);
		base_address += 4U;
	}

	/* Save interrupt vector reg */
	base_address = VIM_BASE_ADDR + VIM_INT_VEC;
	for (i = 0; i < NUM_INTERRUPTS; i++) {
		vim_cfg.intr_vec[i] = readl(base_address);
		base_address += 4U;
	}
}

static void lpm_restore_vim_registers(void)
{
	u32 i;
	u32 base_address;

	/* Restore IRQ vector addr reg */
	base_address = VIM_BASE_ADDR + VIM_IRQVEC;
	writel(vim_cfg.irq_vec, base_address);

	/* Restore FIQ vector addr reg */
	base_address = VIM_BASE_ADDR + VIM_FIQVEC;
	writel(vim_cfg.fiq_vec, base_address);

	/* Restore IRQ PRI MASK reg */
	base_address = VIM_BASE_ADDR + VIM_IRQPRIMASK;
	writel(vim_cfg.irq_pri_mask, base_address);

	/* Restore FIQ PRI MASK reg */
	base_address = VIM_BASE_ADDR + VIM_FIQPRIMASK;
	writel(vim_cfg.fiq_pri_mask, base_address);

	/* Restore DED vector mask reg */
	base_address = VIM_BASE_ADDR + VIM_DEDVECMASK;
	writel(vim_cfg.ded_vec_mask, base_address);

	/* Restore the interrupt enable, type and map value */
	for (i = 0; i < NUM_GRPS; i++) {
		base_address = VIM_BASE_ADDR + VIM_INT_EN(i);
		writel(vim_cfg.grp_intr[i].int_en, base_address);

		writel(vim_cfg.grp_intr[i].int_map, base_address + 16U);

		writel(vim_cfg.grp_intr[i].int_type, base_address + 20U);
	}

	/* Restore interrupt priority reg */
	base_address = VIM_BASE_ADDR + VIM_INT_PRI;
	for (i = 0; i < NUM_INTERRUPTS; i++) {
		writel(vim_cfg.intr_pri[i], base_address);
		base_address += 4U;
	}

	/* Restore interrupt vector reg */
	base_address = VIM_BASE_ADDR + VIM_INT_VEC;
	for (i = 0; i < NUM_INTERRUPTS; i++) {
		writel(vim_cfg.intr_vec[i], base_address);
		base_address += 4U;
	}
}

static void lpm_save_wkup_timer_registers(void)
{
	u32 base_address;

	/* Save timer OCP reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TIOCP_CFG;
	timer_config.tiocp_cfg = readl(base_address);

	/* Save EOI reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_EOI;
	timer_config.eoi = readl(base_address);

	/* Save IRQ PRI MASK reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_SET;
	timer_config.set = readl(base_address);

	/* Save irq wake enable reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_IRQ_WAKE_EN;
	timer_config.irq_wake_en = readl(base_address);

	/* Save timer TCLR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TCLR;
	timer_config.tclr = readl(base_address);

	/* Save timer TCRR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TCRR;
	timer_config.tcrr = readl(base_address);

	/* Save timer TLDR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TLDR;
	timer_config.tldr = readl(base_address);

	/* Save timer TMAR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TMAR;
	timer_config.tmar = readl(base_address);

	/* Save timer TSICR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TSICR;
	timer_config.tsicr = readl(base_address);

	/* Save timer TPIR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TPIR;
	timer_config.tpir = readl(base_address);

	/* Save timer TNIR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TNIR;
	timer_config.tnir = readl(base_address);

	/* Save timer TCVR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TCVR;
	timer_config.tcvr = readl(base_address);

	/* Save timer TOCR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TOCR;
	timer_config.tocr = readl(base_address);

	/* Save timer TCWR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TCWR;
	timer_config.tcwr = readl(base_address);
}

static void lpm_restore_wkup_timer_registers(void)
{
	u32 base_address;

	/* Restore timer OCP reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TIOCP_CFG;
	writel(timer_config.tiocp_cfg, base_address);

	/* Restore EOI reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_EOI;
	writel(timer_config.eoi, base_address);

	/* Restore IRQ PRI MASK reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_SET;
	writel(timer_config.set, base_address);

	/* Restore irq wake enable reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_IRQ_WAKE_EN;
	writel(timer_config.irq_wake_en, base_address);

	/* Restore timer TCLR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TCLR;
	writel(timer_config.tclr, base_address);

	/* Restore timer TCRR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TCRR;
	writel(timer_config.tcrr, base_address);

	/* Restore timer TLDR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TLDR;
	writel(timer_config.tldr, base_address);

	/* Restore timer TMAR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TMAR;
	writel(timer_config.tmar, base_address);

	/* Restore timer TSICR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TSICR;
	writel(timer_config.tsicr, base_address);

	/* Restore timer TPIR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TPIR;
	writel(timer_config.tpir, base_address);

	/* Restore timer TNIR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TNIR;
	writel(timer_config.tnir, base_address);

	/* Restore timer TCVR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TCVR;
	writel(timer_config.tcvr, base_address);

	/* Restore timer TOCR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TOCR;
	writel(timer_config.tocr, base_address);

	/* Restore timer TCWR reg */
	base_address = WKUP_TIMER1_BASE_ADDR + TIM_TCWR;
	writel(timer_config.tcwr, base_address);
}

s32 lpm_sleep_save_wkup_periph_config(void)
{
	lpm_save_vim_registers();
	lpm_save_wkup_timer_registers();

	return SUCCESS;
}

s32 lpm_resume_restore_wkup_periph(void)
{
	lpm_restore_vim_registers();
	lpm_restore_wkup_timer_registers();

	return SUCCESS;
}
