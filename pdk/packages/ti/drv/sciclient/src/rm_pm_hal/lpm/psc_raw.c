/*
 * DM Stub Firmware
 *
 * PSC Raw driver for direct PSC manipulation
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

#include <config.h>
#include <lib/bitops.h>
#include <types/errno.h>
#include <types/short_types.h>
#include "timeout.h"
#include "lpm_io.h"
#include "psc_raw.h"

#define PSC_PID                 0x000U
#define PSC_GBLCTL              0x010U
#define PSC_GBLSTAT             0x014U
#define PSC_INTEVAL             0x018U
#define PSC_IPWKCNT             0x01cU
#define PSC_MERRPR0             0x040U
#define PSC_MERRPR1             0x044U
#define PSC_MERRCR0             0x050U
#define PSC_MERRCR1             0x054U
#define PSC_PERRPR              0x060U
#define PSC_PERRCR              0x068U
#define PSC_EPCPR               0x070U
#define PSC_EPCRR               0x078U
#define PSC_RAILSTAT            0x100U
#define PSC_RAILCTL             0x104U
#define PSC_RAILSET             0x108U
#define PSC_PTCMD               0x120U
#define PSC_PTSTAT              0x128U
#define PSC_PDSTAT(domain)      (0x200U + (4U * (domain)))
#define PSC_PDCTL(domain)       (0x300U + (4U * (domain)))
#define PSC_PDCFG(domain)       (0x400U + (4U * (domain)))
#define PSC_MDCFG(id)           (0x600U + (4U * (id)))
#define PSC_MDSTAT(id)          (0x800U + (4U * (id)))
#define PSC_MDCTL(id)           (0xa00U + (4U * (id)))

#define MDSTAT_STATE_MASK               0x3fU
#define MDSTAT_BUSY_MASK                0x30U
#define MDSTAT_STATE_SWRSTDISABLE       0x00U
#define MDSTAT_STATE_SYNCRST            0x01U
#define MDSTAT_STATE_DISABLE            0x02U
#define MDSTAT_STATE_ENABLE             0x03U
#define MDSTAT_STATE_AUTO_SLEEP         0x04U
#define MDSTAT_STATE_AUTO_WAKE          0x05U
#define MDSTAT_STATE_DISABLE_CLK_ON     0x21U
#define MDSTAT_STATE_DISABLE_IN_PROG    0x22U
#define MDSTAT_STATE_RETRY_DISABLE      0x23U
#define MDSTAT_STATE_ENABLE_IN_PROG     0x24U
#define MDSTAT_STATE_SLEEP_IN_PROG      0x25U
#define MDSTAT_STATE_CLK_ON1            0x26U
#define MDSTAT_STATE_CLK_OFF1           0x27U
#define MDSTAT_STATE_CLK_ON2            0x28U
#define MDSTAT_STATE_CLK_OFF2           0x29U
#define MDSTAT_STATE_CLK_ON3            0x2aU
#define MDSTAT_STATE_CLK_OFF3           0x2bU

#define MDSTAT_EMUIHB           BIT(17) /* EMU alters module state */
#define MDSTAT_EMURST           BIT(16) /* EMU alters reset to module */
#define MDSTAT_MCKOUT           BIT(12) /* Module clock output status */
#define MDSTAT_MRSTDONE         BIT(11) /* Module reset done */
#define MDSTAT_MRST             BIT(10) /* Module reset de asserted */
#define MDSTAT_LRSTDONE         BIT(9)  /* Local reset done */
#define MDSTAT_LRST             BIT(8)  /* Local reset de asserted */

#define MDCTL_STATE_MASK        0x3fU
#define MDCTL_FORCE             BIT(31)
#define MDCTL_RESET_ISO         BIT(12)         /* Enable reset isolation */
#define MDCTL_EMUIHBIE          BIT(10)         /* EMU alters module IE */
#define MDCTL_EMURSTIE          BIT(9)          /* EMU alter reset IE */
#define MDCTL_LRST              BIT(8)          /* Assert local reset when 0 */

#define PDSTAT_EMUIHB                   BIT(11) /* EMU alters domain state */
#define PDSTAT_PWRBAD                   BIT(10) /* Power bad error */
#define PDSTAT_PORDONE                  BIT(9)  /* Power on reset done */
#define PDSTAT_POR                      BIT(8)  /* Power on reset de asserted */
#define PDSTAT_STATE_MASK               0x1fU
#define PDSTAT_STATE_OFF                0x00U
#define PDSTAT_STATE_ON                 0x01U
#define PDSTAT_STATE_ON_ARB             0x10U
#define PDSTAT_STATE_SWITCH_ON          0x11U
#define PDSTAT_STATE_ON_RAIL_CNT        0x12U
#define PDSTAT_STATE_WAIT_ALL_SYNC_RST  0x13U
#define PDSTAT_STATE_STRETCH_POR        0x14U
#define PDSTAT_STATE_WAIT_POR_DONE      0x15U
#define PDSTAT_STATE_COUNT16            0x16U
#define PDSTAT_STATE_WAIT_ALL_SWRST_DIS 0x17U
#define PDSTAT_STATE_OFF_ARB            0x18U
#define PDSTAT_STATE_SWITCH_OFF         0x19U
#define PDSTAT_STATE_OFF_RAIL_CNT       0x1aU

#define PDCTL_STATE_MASK        BIT(0)
#define PDCTL_EPCGOOD           BIT(8)  /* External power control on */
#define PDCTL_EMUIHBIE          BIT(9)  /* EMU alters domain state IE */
#define PDCTL_FORCE             BIT(31)

#define PSC_TRANSITION_TIMEOUT  100000

#define psc_raw_read readl
#define psc_raw_write writel

s32 psc_raw_pd_wait(u32 psc_base, u8 pd)
{
	s32 ret = SUCCESS;
	s32 i = PSC_TRANSITION_TIMEOUT;

	while (((psc_raw_read(psc_base + PSC_PTSTAT) & BIT(pd)) != 0U) && (i != 0)) {
		delay_1us();
		--i;
	}

	if (i == 0) {
		ret = -ETIMEDOUT;
	}

	return ret;
}

void psc_raw_pd_initiate(u32 psc_base, u8 pd)
{
	psc_raw_write(BIT(pd), psc_base + PSC_PTCMD);
}

void psc_raw_pd_set_state(u32 psc_base, u8 pd, u32 state, sbool force)
{
	u32 pdctl = psc_raw_read(psc_base + PSC_PDCTL(pd));

	pdctl &= ~PDCTL_STATE_MASK;
	pdctl |= state;

	if (force) {
		pdctl |= PDCTL_FORCE;
	} else {
		pdctl &= ~PDCTL_FORCE;
	}

	psc_raw_write(pdctl, psc_base + PSC_PDCTL(pd));
}

void psc_raw_lpsc_set_state(u32 psc_base, u8 lpsc, u32 state, sbool force)
{
	u32 mdctl = psc_raw_read(psc_base + PSC_MDCTL(lpsc));

	mdctl &= ~MDCTL_STATE_MASK;
	mdctl |= state;

	if (force) {
		mdctl |= MDCTL_FORCE;
	} else {
		mdctl &= ~MDCTL_FORCE;
	}

	psc_raw_write(mdctl, psc_base + PSC_MDCTL(lpsc));
}

u8 psc_raw_lpsc_get_state(u32 psc_base, u8 lpsc)
{
	return (u8) (MDSTAT_STATE_MASK & psc_raw_read(psc_base + PSC_MDSTAT(lpsc)));
}
