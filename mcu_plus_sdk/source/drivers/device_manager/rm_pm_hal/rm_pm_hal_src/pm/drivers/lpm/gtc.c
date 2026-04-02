/*
 * Device Manager - LPM GTC Driver
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
#include <lib/ioremap.h>
#include <lib/mmr_lock.h>
#include <types/errno.h>
#include "rtc.h"
#include "soc_ctrl_mmr.h"
#include "gtc.h"

#define GTC_CFG0_PUSHEVT       (0x08U)
#define GTC_CFG1_CNTCR         (0x00U)
#define GTC_CFG1_CNT_LO        (0x08U)
#define GTC_CFG1_CNT_HI        (0x0cU)
#define GTC_CFG1_CNTFID0       (0x20U)

#define GTC_CFG1_CNTCR_HDBG    BIT(1)
#define GTC_CFG1_CNTCR_EN      BIT(0)

static struct gtc_cfg_data {
	struct rtc_time suspend;
	struct rtc_time resume;
	u32		pushevt;
	u32		hdbg;
	u32		cnt_lo;
	u32		cnt_hi;
	u32		fid0;
} gtc;

s32 lpm_sleep_suspend_gtc(void)
{
	u32 reg;

	/* Disable GTC */
	reg = readl(GTC_CFG1_BASE + GTC_CFG1_CNTCR);
	reg &= ~GTC_CFG1_CNTCR_EN;
	writel(reg, GTC_CFG1_BASE + GTC_CFG1_CNTCR);

	lpm_rtc_read_time(&gtc.suspend);

	gtc.hdbg    = reg & GTC_CFG1_CNTCR_HDBG;
	gtc.pushevt = readl(GTC_CFG0_BASE + GTC_CFG0_PUSHEVT);
	gtc.cnt_lo  = readl(GTC_CFG1_BASE + GTC_CFG1_CNT_LO);
	gtc.cnt_hi  = readl(GTC_CFG1_BASE + GTC_CFG1_CNT_HI);
	gtc.fid0    = readl(GTC_CFG1_BASE + GTC_CFG1_CNTFID0);

	return SUCCESS;
}

s32 lpm_resume_gtc(void)
{
	u64 rtc1, rtc2, delta;
	s32 ret = SUCCESS;

	writel(gtc.pushevt, GTC_CFG0_BASE + GTC_CFG0_PUSHEVT);
	writel(gtc.fid0, GTC_CFG1_BASE + GTC_CFG1_CNTFID0);

	ret = lpm_rtc_sync_core_and_on_domain();
	if (ret == SUCCESS) {
		lpm_rtc_read_time(&gtc.resume);

		/* Calculate the time elapsed from suspend to resume */
		rtc1 = (u64) (((u64) gtc.suspend.sec_hi << 32) + gtc.suspend.sec_lo);
		rtc2 = (u64) (((u64) gtc.resume.sec_hi << 32) + gtc.resume.sec_lo);
		delta = (rtc2 - rtc1) * gtc.fid0;

		/* Add the time difference to update system count (GTC) correctly */
		delta += (u64) (((u64) gtc.cnt_hi << 32) + gtc.cnt_lo);
		gtc.cnt_hi = (u32) (delta >> 32);
		gtc.cnt_lo = (u32) delta;

		writel(gtc.cnt_lo, GTC_CFG1_BASE + GTC_CFG1_CNT_LO);
		writel(gtc.cnt_hi, GTC_CFG1_BASE + GTC_CFG1_CNT_HI);

		/* Enable GTC */
		writel(gtc.hdbg | GTC_CFG1_CNTCR_EN, GTC_CFG1_BASE + GTC_CFG1_CNTCR);
	}

	return ret;
}
