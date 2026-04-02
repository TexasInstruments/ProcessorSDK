/*
 * Device Manager - LPM RTC Driver
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

#include <lib/ioremap.h>
#include <lib/bitops.h>
#include "rtc.h"
#include "soc_ctrl_mmr.h"


#define RTC_SUB_S_CNT           (0x04U)
#define RTC_S_CNT_LSW           (0x08U)
#define RTC_S_CNT_MSW           (0x0cU)
#define RTC_SYNCPEND            (0x68U)

/* RTC_SYNCPEND register bits */
#define RELOAD_FROM_BBD         BIT(31)
#define RD_PEND                 BIT(1)

/* RD_PEND bit field values */
#define RD_PEND_COMPLETE        0x0U
#define RD_PEND_ACTIVE          0x1U

#define TIMEOUT_10ms            10000U

void lpm_rtc_read_time(struct rtc_time *rtc)
{
	if (rtc != NULL) {
		rtc->sub_sec = readl(RTC_BASE + RTC_SUB_S_CNT);
		rtc->sec_lo = readl(RTC_BASE + RTC_S_CNT_LSW);
		rtc->sec_hi = readl(RTC_BASE + RTC_S_CNT_MSW);
	} else {
		/* do nothing */
	}
}

s32 lpm_rtc_sync_core_and_on_domain(void)
{
	u32 val = 0U;
	u32 timeout = TIMEOUT_10ms;
	s32 ret = SUCCESS;

	/* Trigger the registers to be reloaded from the battery backed domain */
	val = readl(RTC_BASE + RTC_SYNCPEND);
	writel((val | RELOAD_FROM_BBD), RTC_BASE + RTC_SYNCPEND);

	/* Wait for synchronization to get completed */
	while ((timeout > 0U) && ((readl(RTC_BASE + RTC_SYNCPEND) & RD_PEND) != RD_PEND_COMPLETE)) {
		--timeout;
	}
	if (timeout == 0U) {
		ret = -ETIMEDOUT;
	}

	return ret;
}
