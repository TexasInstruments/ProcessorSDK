/*
 * IRQ Interrupt Router
 *
 * IRQ Interrupt Router SoC data for AM6
 *
 * Copyright (C) 2017-2020, Texas Instruments Incorporated
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
#include <soc/am65x/resasg_types.h>
#include <soc/am65x/soc_devgrps.h>

#include "../../include/ir_inst.h"
#include "ir_cfg.h"

/* Start of MAIN2MCU_LVL_INTRTR0 instance definition */
static const struct rm_mmr mmr_MAIN2MCU_LVL_INTRTR0_CFG = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0xa10000,
};

static struct ir_used_mapping rom_usage_MAIN2MCU_LVL_INTRTR0_CFG[2U] = {
	{
		.inp_start = 64U,
		.outp_start = 0U,
		.length = 32U,
		.cleared = SFALSE,
	},
	{
		.inp_start = 28U,
		.outp_start = 32U,
		.length = 2U,
		.cleared = SFALSE,
	},
};

/* Start of MAIN2MCU_PLS_INTRTR0 instance definition */
static const struct rm_mmr mmr_MAIN2MCU_PLS_INTRTR0_CFG = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0xa20000,
};

/* Start of main_gpiomux_introuter_main_gpiomux_introuter_main_0 instance
 *definition */
static const struct rm_mmr
	mmr_main_gpiomux_introuter_main_gpiomux_introuter_main_0_intr_router_cfg
	= {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0xa00000,
	};

/* Start of WKUP_GPIOMUX_INTRTR0 instance definition */
static const struct rm_mmr mmr_WKUP_GPIOMUX_INTRTR0_CFG = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x42200000,
};

/* Start of cmp_event_introuter_cmp_event_introuter_main_0 instance definition
 **/
static const struct rm_mmr
	mmr_cmp_event_introuter_cmp_event_introuter_main_0_intr_router_cfg = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0xa30000,
};

/* Start of TIMESYNC_INTRTR0 instance definition */
static const struct rm_mmr mmr_TIMESYNC_INTRTR0_intr_router_cfg = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0xa40000,
};

/* Start of NAVSS0_INTR_ROUTER_0 instance definition */
static const struct rm_mmr mmr_NAVSS0_INTR_ROUTER_0_intr_router_cfg = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x310e0000,
};

/* Start of MCU_NAVSS0_INTR_ROUTER_0 instance definition */
static const struct rm_mmr mmr_MCU_NAVSS0_INTR_ROUTER_0_intr_router_cfg = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x28540000,
};

static struct ir_used_mapping rom_usage_MCU_NAVSS0_INTR_ROUTER_0_intr_router_cfg[1U] = {
	{
		.inp_start = 1U,
		.outp_start = 0U,
		.length = 2U,
		.cleared = SFALSE,
	},
};


struct ir_instance ir_inst[IR_INST_COUNT] = {
	{
		/* MAIN2MCU_LVL_INTRTR0 */
		.id = AM6_DEV_MAIN2MCU_LVL_INTRTR0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MAIN,
		.int_enable = SFALSE,
		.cfg = &mmr_MAIN2MCU_LVL_INTRTR0_CFG,
		.n_inp = 192u,
		.n_outp = 64u,
		.inp0_mapping = 0u,
		.outp_utype = RESASG_UTYPE(AM6_DEV_MAIN2MCU_LVL_INTRTR0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = &rom_usage_MAIN2MCU_LVL_INTRTR0_CFG[0U],
		.n_rom_usage = 2U,
	},
	{
		/* MAIN2MCU_PLS_INTRTR0 */
		.id = AM6_DEV_MAIN2MCU_PLS_INTRTR0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MAIN,
		.int_enable = SFALSE,
		.cfg = &mmr_MAIN2MCU_PLS_INTRTR0_CFG,
		.n_inp = 32U,
		.n_outp = 48u,
		.inp0_mapping = 0u,
		.outp_utype = RESASG_UTYPE(AM6_DEV_MAIN2MCU_PLS_INTRTR0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0u,
	},
	{
		/* main_gpiomux_introuter_main_gpiomux_introuter_main_0 */
		.id = AM6_DEV_GPIOMUX_INTRTR0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MAIN,
		.int_enable = SFALSE,
		.cfg = &mmr_main_gpiomux_introuter_main_gpiomux_introuter_main_0_intr_router_cfg,
		.n_inp = 208u,
		.n_outp = 32u,
		.inp0_mapping = 0u,
		.outp_utype = RESASG_UTYPE(AM6_DEV_GPIOMUX_INTRTR0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0u,
	},
	{
		/* WKUP_GPIOMUX_INTRTR0 */
		.id = AM6_DEV_WKUP_GPIOMUX_INTRTR0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MCU_WAKEUP,
		.int_enable = SFALSE,
		.cfg = &mmr_WKUP_GPIOMUX_INTRTR0_CFG,
		.n_inp = 64u,
		.n_outp = 16u,
		.inp0_mapping = 0u,
		.outp_utype = RESASG_UTYPE(AM6_DEV_WKUP_GPIOMUX_INTRTR0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0u,
	},
	{
		/* cmp_event_introuter_cmp_event_introuter_main_0 */
		.id = AM6_DEV_CMPEVENT_INTRTR0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MAIN,
		.int_enable = SFALSE,
		.cfg = &mmr_cmp_event_introuter_cmp_event_introuter_main_0_intr_router_cfg,
		.n_inp = 128u,
		.n_outp = 32u,
		.inp0_mapping = 0u,
		.outp_utype = RESASG_UTYPE(AM6_DEV_CMPEVENT_INTRTR0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0u,
	},
	{
		/* TIMESYNC_INTRTR0 */
		.id = AM6_DEV_TIMESYNC_INTRTR0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MAIN,
		.int_enable = SFALSE,
		.cfg = &mmr_TIMESYNC_INTRTR0_intr_router_cfg,
		.n_inp = 48u,
		.n_outp = 40u,
		.inp0_mapping = 0u,
		.outp_utype = RESASG_UTYPE(AM6_DEV_TIMESYNC_INTRTR0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0u,
	},
	{
		/* NAVSS0_INTR_ROUTER */
		.id = AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MAIN,
		.int_enable = SFALSE,
		.cfg = &mmr_NAVSS0_INTR_ROUTER_0_intr_router_cfg,
		.n_inp = 440u,
		.n_outp = 152u,
		.inp0_mapping = 0u,
		.outp_utype = RESASG_UTYPE(AM6_DEV_NAVSS0_INTR_ROUTER_0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = NULL,
		.n_rom_usage = 0u,
	},
	{
		/* MCU_NAVSS0_INTR_ROUTER */
		.id = AM6_DEV_MCU_NAVSS0_INTR_ROUTER_0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MCU_WAKEUP,
		.int_enable = SFALSE,
		.cfg = &mmr_MCU_NAVSS0_INTR_ROUTER_0_intr_router_cfg,
		.n_inp = 261u,
		.n_outp = 64u,
		.inp0_mapping = 0u,
		.outp_utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_INTR_ROUTER_0, RESASG_SUBTYPE_IR_OUTPUT),
		.rom_usage = &rom_usage_MCU_NAVSS0_INTR_ROUTER_0_intr_router_cfg[0U],
		.n_rom_usage = 1u,
	},
};
