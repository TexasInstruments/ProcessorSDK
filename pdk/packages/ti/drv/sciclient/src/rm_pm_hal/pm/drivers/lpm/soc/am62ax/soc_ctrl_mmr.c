/*
 * DM Firmware
 *
 * am62ax soc_ctrl_mmr.h
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

#include <config.h>
#include <types/short_types.h>
#include <ctrl_mmr.h>
#include "soc_ctrl_mmr.h"

struct ctrl_mmr ctrl_mmr_data [MAX_MMR_DATA] = {
	[0] =  {
		.base		= WKUP_CTRL_BASE,
		.partition	= 0,
		.is_locked	= STRUE
	},
	[1] =  {
		.base		= WKUP_CTRL_BASE,
		.partition	= 1,
		.is_locked	= STRUE
	},
	[2] =  {
		.base		= WKUP_CTRL_BASE,
		.partition	= 2,
		.is_locked	= STRUE
	},
	[3] =  {
		.base		= WKUP_CTRL_BASE,
		.partition	= 3,
		.is_locked	= STRUE
	},
	[4] =  {
		.base		= WKUP_CTRL_BASE,
		.partition	= 4,
		.is_locked	= STRUE
	},
	[5] =  {
		.base		= WKUP_CTRL_BASE,
		.partition	= 5,
		.is_locked	= STRUE
	},
	[6] =  {
		.base		= WKUP_CTRL_BASE,
		.partition	= 6,
		.is_locked	= STRUE
	},
	[7] =  {
		.base		= WKUP_CTRL_BASE,
		.partition	= 7,
		.is_locked	= STRUE
	},
	[8] =  {
		.base		= MAIN_CTRL_BASE,
		.partition	= 0,
		.is_locked	= STRUE
	},
	[9] =  {
		.base		= MAIN_CTRL_BASE,
		.partition	= 1,
		.is_locked	= STRUE
	},
	[10] = {
		.base		= MAIN_CTRL_BASE,
		.partition	= 2,
		.is_locked	= STRUE
	},
	[11] = {
		.base		= MAIN_CTRL_BASE,
		.partition	= 4,
		.is_locked	= STRUE
	},
	[12] = {
		.base		= MAIN_CTRL_BASE,
		.partition	= 6,
		.is_locked	= STRUE
	},
	[13] = {
		.base		= MCU_CTRL_BASE,
		.partition	= 0,
		.is_locked	= STRUE
	},
	[14] = {
		.base		= MCU_CTRL_BASE,
		.partition	= 1,
		.is_locked	= STRUE
	},
	[15] = {
		.base		= MCU_CTRL_BASE,
		.partition	= 2,
		.is_locked	= STRUE
	},
	[16] = {
		.base		= MCU_CTRL_BASE,
		.partition	= 3,
		.is_locked	= STRUE
	},
	[17] = {
		.base		= MCU_CTRL_BASE,
		.partition	= 4,
		.is_locked	= STRUE
	},
	[18] = {
		.base		= MCU_CTRL_BASE,
		.partition	= 6,
		.is_locked	= STRUE
	}
};
