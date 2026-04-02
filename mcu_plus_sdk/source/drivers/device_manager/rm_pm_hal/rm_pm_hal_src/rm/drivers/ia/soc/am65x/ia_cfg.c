/*
 * IRQ Interrupt Aggregator
 *
 * IRQ Interrupt Aggregator SoC data for AM6
 *
 * Copyright (C) 2018-2020, Texas Instruments Incorporated
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

#include "../../include/ia_inst.h"
#include <ia_cfg.h>

/* Start of navss0_udmass_inta0 instance definition */
static const struct rm_mmr mmr_navss0_udmass_inta0_UDMASS_INTA0_IMAP = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x30940000u,
};
static const struct rm_mmr mmr_navss0_udmass_inta0_UDMASS_INTA0_CFG_INTR = {
	.fwl_id		= 4320u,
	.base		= 0x33d00000u,
	.fwl_ch_start	= 0u,
	.fwl_ch_end	= 255u,
};
static const struct rm_mmr mmr_navss_udmass_inta0_UDMASS_INTA0_CFG_GCNTRTI = {
	.fwl_id		= 4320u,
	.base		= 0x33800000u,
	.fwl_ch_start	= 256u,
	.fwl_ch_end	= 767u,
};
static u8 vint_usage_count_navss0_udmass_inta0[256];

/* Start of navss0_modss_inta0 instance definition */
const struct rm_mmr mmr_navss0_modss_inta0_modss_inta0__cfg__imap = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x30900000u,
};
const struct rm_mmr mmr_navss0_modss_inta0_modss_inta0__cfg__intr = {
	.fwl_id		= 4132u,
	.base		= 0x33c00000u,
	.fwl_ch_start	= 0u,
	.fwl_ch_end	= 63u,
};
static u8 vint_usage_count_navss0_modss_inta0[64];

/* Start of navss0_modss_inta1 instance definition */
static const struct rm_mmr mmr_navss0_modss_inta1_modss_inta1__cfg__imap = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x30908000u,
};
static const struct rm_mmr mmr_navss0_modss_inta1_modss_inta1__cfg__intr = {
	.fwl_id		= 4140u,
	.base		= 0x33c40000u,
	.fwl_ch_start	= 0u,
	.fwl_ch_end	= 63u,
};
static u8 vint_usage_count_navss0_modss_inta1[64];

/* Start of mcu_navss0_intr_aggr_0 instance definition */
static const struct rm_mmr mmr_mcu_navss0_intr_aggr_0_UDMASS_INTA0_IMAP = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x28560000u,
};
static const struct rm_mmr mmr_mcu_navss0_intr_aggr_0_UDMASS_INTA0_INTR = {
	.fwl_id		= 6208u,
	.base		= 0x2a700000u,
	.fwl_ch_start	= 0u,
	.fwl_ch_end	= 255u,
};
static const struct rm_mmr mmr_mcu_navss0_intr_aggr_0__UDMASS_INTA0_GCNTRTI = {
	.fwl_id		= 6208u,
	.base		= 0x2A600000u,
	.fwl_ch_start	= 256u,
	.fwl_ch_end	= 511u,
};
static u8 vint_usage_count_mcu_navss0_intr_aggr_0[256];

static struct ia_used_mapping rom_usage_mcu_navss0_intr_aggr_0[3U] = {
	{
		.event = 16404U,
		.cleared = SFALSE,
	},
	{
		.event = 16405U,
		.cleared = SFALSE,
	},
	{
		.event = 16414U,
		.cleared = SFALSE,
	},
};

struct ia_instance ia_inst[IA_INST_COUNT] = {
	{
		/* navss0_udmass_inta0 */
		.id = AM6_DEV_NAVSS0_UDMASS_INTA0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MAIN,
		.imap = &mmr_navss0_udmass_inta0_UDMASS_INTA0_IMAP,
		.intr = &mmr_navss0_udmass_inta0_UDMASS_INTA0_CFG_INTR,
		.gcntrti = &mmr_navss_udmass_inta0_UDMASS_INTA0_CFG_GCNTRTI,
		.unmap = NULL,
		.sevt_offset = 0u,
		.n_sevt = 4608u,
		.vint_utype = RESASG_UTYPE(AM6_DEV_NAVSS0_UDMASS_INTA0, RESASG_SUBTYPE_IA_VINT),
		.n_vint = 256U,
		.vint_usage_count = &vint_usage_count_navss0_udmass_inta0[0],
		.v0_b0_evt = 0u,
		.rom_usage = NULL,
		.n_rom_usage = 0U,
		.unmapped_events = NULL,
		.n_unmapped_events = 0U,
	},
	{
		/* navss0_modss_inta0 */
		.id = AM6_DEV_NAVSS0_MODSS_INTA0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MAIN,
		.imap = &mmr_navss0_modss_inta0_modss_inta0__cfg__imap,
		.intr = &mmr_navss0_modss_inta0_modss_inta0__cfg__intr,
		.gcntrti = NULL,
		.unmap = NULL,
		.sevt_offset = 20480u,
		.n_sevt = 1024u,
		.vint_utype = RESASG_UTYPE(AM6_DEV_NAVSS0_MODSS_INTA0, RESASG_SUBTYPE_IA_VINT),
		.n_vint = 64U,
		.vint_usage_count = &vint_usage_count_navss0_modss_inta0[0],
		.v0_b0_evt = 0u,
		.rom_usage = NULL,
		.n_rom_usage = 0U,
		.unmapped_events = NULL,
		.n_unmapped_events = 0U,
	},
	{
		/* navss0_modss_inta1 */
		.id = AM6_DEV_NAVSS0_MODSS_INTA1,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MAIN,
		.imap = &mmr_navss0_modss_inta1_modss_inta1__cfg__imap,
		.intr = &mmr_navss0_modss_inta1_modss_inta1__cfg__intr,
		.gcntrti = NULL,
		.unmap = NULL,
		.sevt_offset = 22528u,
		.n_sevt = 1024u,
		.vint_utype = RESASG_UTYPE(AM6_DEV_NAVSS0_MODSS_INTA1, RESASG_SUBTYPE_IA_VINT),
		.n_vint = 64U,
		.vint_usage_count = &vint_usage_count_navss0_modss_inta1[0],
		.v0_b0_evt = 0u,
		.rom_usage = NULL,
		.n_rom_usage = 0U,
		.unmapped_events = NULL,
		.n_unmapped_events = 0U,
	},
	{
		/* mcu_navss0_intr_aggr_0 */
		.id = AM6_DEV_MCU_NAVSS0_INTR_AGGR_0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MCU_WAKEUP,
		.imap = &mmr_mcu_navss0_intr_aggr_0_UDMASS_INTA0_IMAP,
		.intr = &mmr_mcu_navss0_intr_aggr_0_UDMASS_INTA0_INTR,
		.gcntrti = &mmr_mcu_navss0_intr_aggr_0__UDMASS_INTA0_GCNTRTI,
		.unmap = NULL,
		.sevt_offset = 16384u,
		.n_sevt = 1536u,
		.vint_utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_INTR_AGGR_0, RESASG_SUBTYPE_IA_VINT),
		.n_vint = 256U,
		.vint_usage_count = &vint_usage_count_mcu_navss0_intr_aggr_0[0],
		.v0_b0_evt = 0u,
		.rom_usage = &rom_usage_mcu_navss0_intr_aggr_0[0U],
		.n_rom_usage = ARRAY_SIZE(rom_usage_mcu_navss0_intr_aggr_0),
		.unmapped_events = NULL,
		.n_unmapped_events = 0U,
	},
};
