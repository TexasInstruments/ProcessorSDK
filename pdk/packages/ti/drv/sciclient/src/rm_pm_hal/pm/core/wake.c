/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2014-2024, Texas Instruments Incorporated
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

#include <wake.h>
#include <types/short_types.h>
#include <types/array_size.h>
#include <lib/ioremap.h>
#include <lib/trace.h>
#include <remote.h>
#include <types/errno.h>
#include <lib/bitops.h>
#include <sleep.h>

#include <tisci/pm/tisci_pm_core.h>

/*
 * Wake interrupts are not for waking the PMMC, they are for removing the ARM
 * from disabled state. Its likely that each master will need its own set of
 * wake interrupts.
 */
s32 wake_finish(void)
{
	return -ENODEV;
}

void wake_check(void)
{
}

void wake_notify_mode(const struct sleep_mode *mode __attribute__((unused)))
{
}

const struct sleep_mode *wake_deepest_mode(void)
{
	return NULL;
}

void wake_arm(s32 host __attribute__((unused)))
{
}

s32 wake_sources_assign(const struct resource_irq *irq __attribute__((unused)), size_t n __attribute__((unused)))
{
	return -ENODEV;
}
