/*
 * Interrupt Router
 *
 * Interrupt router driver instance data definition
 *
 * Data version: 220525_125254
 *
 * Copyright (C) 2021-2025, Texas Instruments Incorporated
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

#include <types/short_types.h>
#include <types/array_size.h>
#include <types/sbool.h>
#include <soc/rm_mmr.h>
#include <devices.h>
#include <soc/j721s2/resasg_types.h>
#include <soc/j721s2/soc_devgrps.h>

#include "../../include/ir_inst.h"
#include "ir_cfg.h"

static const struct rm_mmr mmr_MAIN2MCU_LVL_INTRTR0_CFG = {
	.fwl_id		= RM_MMR_FWL_INVALID,
	.fwl_ch_start	= 0U,
	.fwl_ch_end	= 0U,
	.base		= 0xa10000U,
};

static struct ir_used_mapping rom_usage_MAIN2MCU_LVL_INTRTR0[3U] = {
	{
		.inp_start = 28U,
		.outp_start = 32U,
		.length = 2U,
		.cleared = SFALSE,
	},
	{
		.inp_start = 128U,
		.outp_start = 34U,
		.length = 8U,
		.cleared = SFALSE,
	},
	{
		.inp_start = 218U,
		.outp_start = 50U,
		.length = 1U,
		.cleared = SFALSE,
	},
};

static const struct rm_mmr mmr_MAIN2MCU_PLS_INTRTR0_CFG = {
	.fwl_id		= RM_MMR_FWL_INVALID,
	.fwl_ch_start	= 0U,
	.fwl_ch_end	= 0U,
	.base		= 0xa20000U,
};

static const struct rm_mmr mmr_TIMESYNC_INTRTR0_INTR_ROUTER_CFG = {
	.fwl_id		= RM_MMR_FWL_INVALID,
	.fwl_ch_start	= 0U,
	.fwl_ch_end	= 0U,
	.base		= 0xa40000U,
};

static const struct rm_mmr mmr_WKUP_GPIOMUX_INTRTR0_CFG = {
	.fwl_id		= RM_MMR_FWL_INVALID,
	.fwl_ch_start	= 0U,
	.fwl_ch_end	= 0U,
	.base		= 0x42200000U,
};

static const struct rm_mmr mmr_GPIOMUX_INTRTR0_CFG = {
	.fwl_id		= RM_MMR_FWL_INVALID,
	.fwl_ch_start	= 0U,
	.fwl_ch_end	= 0U,
	.base		= 0xa00000U,
};

static const struct rm_mmr mmr_CMPEVENT_INTRTR0_CFG = {
	.fwl_id		= RM_MMR_FWL_INVALID,
	.fwl_ch_start	= 0U,
	.fwl_ch_end	= 0U,
	.base		= 0xa30000U,
};

static const struct rm_mmr mmr_NAVSS0_INTR_0_INTR0_INTR_ROUTER_CFG = {
	.fwl_id		= RM_MMR_FWL_INVALID,
	.fwl_ch_start	= 0U,
	.fwl_ch_end	= 0U,
	.base		= 0x310e0000U,
};

static const struct rm_mmr mmr_MCU_NAVSS0_INTR_ROUTER_0_INTR0_CFG = {
	.fwl_id		= RM_MMR_FWL_INVALID,
	.fwl_ch_start	= 0U,
	.fwl_ch_end	= 0U,
	.base		= 0x28540000U,
};

struct ir_instance ir_inst[IR_INST_COUNT] = {
	{
		.id = J721S2_DEV_MAIN2MCU_LVL_INTRTR0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_J721S2_MAIN,
		.int_enable = STRUE,
		.cfg = &mmr_MAIN2MCU_LVL_INTRTR0_CFG,
		.n_inp = 320U,
		.n_outp = 64U,
		.inp0_mapping = 0U,
		.outp_utype = RESASG_UTYPE(J721S2_DEV_MAIN2MCU_LVL_INTRTR0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = &rom_usage_MAIN2MCU_LVL_INTRTR0[0U],
		.n_rom_usage = ARRAY_SIZE(rom_usage_MAIN2MCU_LVL_INTRTR0),
	},
	{
		.id = J721S2_DEV_MAIN2MCU_PLS_INTRTR0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_J721S2_MAIN,
		.int_enable = STRUE,
		.cfg = &mmr_MAIN2MCU_PLS_INTRTR0_CFG,
		.n_inp = 112U,
		.n_outp = 48U,
		.inp0_mapping = 0U,
		.outp_utype = RESASG_UTYPE(J721S2_DEV_MAIN2MCU_PLS_INTRTR0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0U,
	},
	{
		.id = J721S2_DEV_TIMESYNC_INTRTR0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_J721S2_MAIN,
		.int_enable = STRUE,
		.cfg = &mmr_TIMESYNC_INTRTR0_INTR_ROUTER_CFG,
		.n_inp = 56U,
		.n_outp = 48U,
		.inp0_mapping = 0U,
		.outp_utype = RESASG_UTYPE(J721S2_DEV_TIMESYNC_INTRTR0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0U,
	},
	{
		.id = J721S2_DEV_WKUP_GPIOMUX_INTRTR0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_J721S2_MCU_WAKEUP,
		.int_enable = STRUE,
		.cfg = &mmr_WKUP_GPIOMUX_INTRTR0_CFG,
		.n_inp = 120U,
		.n_outp = 32U,
		.inp0_mapping = 0U,
		.outp_utype = RESASG_UTYPE(J721S2_DEV_WKUP_GPIOMUX_INTRTR0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0U,
	},
	{
		.id = J721S2_DEV_GPIOMUX_INTRTR0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_J721S2_MAIN,
		.int_enable = STRUE,
		.cfg = &mmr_GPIOMUX_INTRTR0_CFG,
		.n_inp = 180U,
		.n_outp = 64U,
		.inp0_mapping = 0U,
		.outp_utype = RESASG_UTYPE(J721S2_DEV_GPIOMUX_INTRTR0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0U,
	},
	{
		.id = J721S2_DEV_CMPEVENT_INTRTR0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_J721S2_MAIN,
		.int_enable = STRUE,
		.cfg = &mmr_CMPEVENT_INTRTR0_CFG,
		.n_inp = 16U,
		.n_outp = 16U,
		.inp0_mapping = 0U,
		.outp_utype = RESASG_UTYPE(J721S2_DEV_CMPEVENT_INTRTR0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0U,
	},
	{
		.id = J721S2_DEV_NAVSS0_INTR_0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_J721S2_MAIN,
		.int_enable = STRUE,
		.cfg = &mmr_NAVSS0_INTR_0_INTR0_INTR_ROUTER_CFG,
		.n_inp = 490U,
		.n_outp = 512U,
		.inp0_mapping = 0U,
		.outp_utype = RESASG_UTYPE(J721S2_DEV_NAVSS0_INTR_0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0U,
	},
	{
		.id = J721S2_DEV_MCU_NAVSS0_INTR_ROUTER_0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_J721S2_MCU_WAKEUP,
		.int_enable = STRUE,
		.cfg = &mmr_MCU_NAVSS0_INTR_ROUTER_0_INTR0_CFG,
		.n_inp = 261U,
		.n_outp = 64U,
		.inp0_mapping = 0U,
		.outp_utype = RESASG_UTYPE(J721S2_DEV_MCU_NAVSS0_INTR_ROUTER_0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0U,
	},
};
