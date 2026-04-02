/*
 * Device Manager - LPM MMR Driver
 *
 * Copyright (C) 2023, Texas Instruments Incorporated
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
#include <lib/mmr_lock.h>
#include "ctrl_mmr.h"

#define CTRL_MMR0_PARTITION_SIZE                (0x4000U)
#define CTRLMMR_LOCK_KICK0                      (0x01008U)
#define CTRLMMR_LOCK_KICK0_UNLOCK_VAL           (0x68ef3490U)
#define CTRLMMR_LOCK_KICK0_LOCK_VAL             (0x0U)
#define CTRLMMR_LOCK_KICK0_UNLOCKED_MASK        BIT(0)
#define CTRLMMR_LOCK_KICK0_UNLOCKED_SHIFT       (0U)
#define CTRLMMR_LOCK_KICK1                      (0x0100cU)
#define CTRLMMR_LOCK_KICK1_UNLOCK_VAL           (0xd172bc5aU)
#define CTRLMMR_LOCK_KICK1_LOCK_VAL             (0x0U)


static sbool is_mmr_locked(u32 base, u32 partition)
{
	u32 part_base = base + (partition * CTRL_MMR0_PARTITION_SIZE);

	return (readl(part_base + CTRLMMR_LOCK_KICK0) & CTRLMMR_LOCK_KICK0_UNLOCKED_MASK) != CTRLMMR_LOCK_KICK0_UNLOCKED_MASK;
}

s32 lpm_save_mmr_lock(void)
{
	for (u32 i = 0; i < MAX_MMR_DATA; i++) {
		if (ctrl_mmr_data[i].base != 0U) {
			ctrl_mmr_data[i].is_locked = is_mmr_locked(ctrl_mmr_data[i].base, ctrl_mmr_data[i].partition);
		}
	}
	return SUCCESS;
}

s32 lpm_restore_mmr_lock(void)
{
	for (u32 i = 0; i < MAX_MMR_DATA; i++) {
		if ((ctrl_mmr_data[i].base != 0U) && (ctrl_mmr_data[i].is_locked == SFALSE)) {
			mmr_unlock(ctrl_mmr_data[i].base, ctrl_mmr_data[i].partition);
		}
	}
	return SUCCESS;
}
