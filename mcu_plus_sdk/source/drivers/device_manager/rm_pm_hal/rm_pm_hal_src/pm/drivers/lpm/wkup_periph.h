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

#ifndef __LPM_WKUP_PERIPH_H__
#define __LPM_WKUP_PERIPH_H__

#include <types/short_types.h>
#include <types/errno.h>

#define NUM_INTERRUPTS          (256U)
#define NUM_INTR_PER_GRP        32U
#define NUM_GRPS                (NUM_INTERRUPTS / NUM_INTR_PER_GRP)

struct vim_intr_grp {
	u32	int_en;
	u32	int_map;
	u32	int_type;
};

struct vim_intr {
	u32			irq_vec;
	u32			fiq_vec;
	u32			irq_pri_mask;
	u32			fiq_pri_mask;
	u32			ded_vec_mask;
	struct vim_intr_grp	grp_intr[NUM_GRPS];
	u32			intr_pri[NUM_INTERRUPTS];
	u32			intr_vec[NUM_INTERRUPTS];
};

struct timer_cfg {
	u32	tiocp_cfg;
	u32	eoi;
	u32	set;
	u32	irq_wake_en;
	u32	tclr;
	u32	tcrr;
	u32	tldr;
	u32	tmar;
	u32	tsicr;
	u32	tpir;
	u32	tnir;
	u32	tcvr;
	u32	tocr;
	u32	tcwr;
};

/**
 *  \brief  Save WKUP domain VIM and WKUP_TIMER1 configuration
 *          during suspend
 *
 *  \return ret      SUCCESS
 */
s32 lpm_sleep_save_wkup_periph_config(void);

/**
 *  \brief  Restore WKUP domain VIM and WKUP_TIMER1 configuration
 *          during resume
 *
 *  \return ret      SUCCESS
 */
s32 lpm_resume_restore_wkup_periph(void);

#endif /* __LPM_WKUP_PERIPH_H__ */
