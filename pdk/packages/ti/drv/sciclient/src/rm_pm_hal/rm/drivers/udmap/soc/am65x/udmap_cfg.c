/*
 * UDMAP
 *
 * UDMAP SoC data for AM6
 *
 * Copyright (C) 2018-2023, Texas Instruments Incorporated
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
#include <soc/rm_mmr.h>
#include <resasg_types.h>
#include <devices.h>
#include <soc/am65x/resasg_types.h>
#include <soc/am65x/soc_devgrps.h>

#include "../../include/udmap_inst.h"
#include <udmap_cfg.h>

/* Start of NAVSS0_UDMAP0 instance definition */
static const struct rm_mmr mmr_NAVSS0_UDMASS_UDMAP0_CFG = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x31150000u,
};
static const struct rm_mmr mmr_NAVSS0_UDMASS_UDMAP0_CFG_TCHAN = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x30B00000u,
};
static const struct rm_mmr mmr_NAVSS0_UDMASS_UDMAP0_CFG_RCHAN = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x30C00000u,
};
static const struct rm_mmr mmr_NAVSS0_UDMASS_UDMAP0_CFG_RFLOW = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x30D00000u,
};
static const struct rm_mmr mmr_NAVSS0_UDMASS_UDMAP0_CFG_TCHANRT = {
	.fwl_id		= 4368u,
	.base		= 0x35000000u,
	.fwl_ch_start	= 0u,
	.fwl_ch_end	= 151u,
};
static const struct rm_mmr mmr_NAVSS0_UDMASS_UDMAP0_CFG_RCHANRT = {
	.fwl_id		= 4368u,
	.base		= 0x34000000u,
	.fwl_ch_start	= 152u,
	.fwl_ch_end	= 301u,
};
static const struct udmap_ch_type tx_ch_types_NAVSS0_UDMAP0[] = {
	{
		.start = 0u,
		.end = 7u,
		.flow_start = 0U,
		.flow_end = 0U,
		.flow_utype = 0U,
		.oes_offset = UDMAP_TCHAN_OES_EVT_INDEX_OFFSET,
		.err_oes_offset = UDMAP_TCHAN_EOES_EVT_INDEX_OFFSET,
		.type = UDMAP_TX_HCHAN,
		.utype = RESASG_UTYPE(AM6_DEV_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_TX_HCHAN),
	},
	{
		.start = 8u,
		.end = 119u,
		.flow_start = 0U,
		.flow_end = 0U,
		.flow_utype = 0U,
		.oes_offset = UDMAP_TCHAN_OES_EVT_INDEX_OFFSET,
		.err_oes_offset = UDMAP_TCHAN_EOES_EVT_INDEX_OFFSET,
		.type = UDMAP_TX_CHAN,
		.utype = RESASG_UTYPE(AM6_DEV_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_TX_CHAN),
	},
	{
		.start = 120u,
		.end = 151u,
		.flow_start = 0U,
		.flow_end = 0U,
		.oes_offset = UDMAP_TCHAN_OES_EVT_INDEX_OFFSET,
		.err_oes_offset = UDMAP_TCHAN_EOES_EVT_INDEX_OFFSET,
		.type = UDMAP_TX_ECHAN,
		.utype = RESASG_UTYPE(AM6_DEV_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_TX_ECHAN),
	},
};
static const struct udmap_ch_type rx_ch_types_NAVSS0_UDMAP0[] = {
	{
		.start = 0u,
		.end = 7u,
		.flow_start = 0U,
		.flow_end = 7U,
		.flow_utype = RESASG_UTYPE(AM6_DEV_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_RX_HCHAN),
		.oes_offset = UDMAP_RCHAN_OES_EVT_INDEX_OFFSET,
		.err_oes_offset = UDMAP_RCHAN_EOES_EVT_INDEX_OFFSET,
		.type = UDMAP_RX_HCHAN,
		.utype = RESASG_UTYPE(AM6_DEV_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_RX_HCHAN),
	},
	{
		.start = 8u,
		.end = 149u,
		.flow_start = 8U,
		.flow_end = 149U,
		.flow_utype = RESASG_UTYPE(AM6_DEV_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_RX_CHAN),
		.oes_offset = UDMAP_RCHAN_OES_EVT_INDEX_OFFSET,
		.err_oes_offset = UDMAP_RCHAN_EOES_EVT_INDEX_OFFSET,
		.type = UDMAP_RX_CHAN,
		.utype = RESASG_UTYPE(AM6_DEV_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_RX_CHAN),
	},
};

static u8 flow_delegates_NAVSS0_UDMAP0[150U];

static struct udmap_flow_common common_flows_NAVSS0_UDMAP0 = {
	.start		= AM6_MAIN_NAVSS_UDMAP_RX_CHAN_MAX_INDEX,
	.end		= AM6_MAIN_NAVSS_UDMAP_RX_FLOW_MAX_INDEX - 1U,
	.utype		= RESASG_UTYPE(AM6_DEV_NAVSS0_UDMAP0,	      RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
	.delegates	= &flow_delegates_NAVSS0_UDMAP0[0U],
};

/* Start of MCU_NAVSS0_UDMAP0 instance definition */
static const struct rm_mmr mmr_MCU_NAVSS0_UDMASS_UDMAP0_CFG_GCFG = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x285C0000u,
};
static const struct rm_mmr mmr_MCU_NAVSS0_UDMASS_UDMAP0_TCHAN = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x284A0000u,
};
static const struct rm_mmr mmr_MCU_NAVSS0_UDMASS_UDMAP0_RCHAN = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x284C0000u,
};
static const struct rm_mmr mmr_MCU_NAVSS0_UDMASS_UDMAP0_CFG_RFLOW = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x28400000u,
};
static const struct rm_mmr mmr_MCU_NAVSS0_UDMASS_UDMAP0_TCHANRT = {
	.fwl_id		= 6244u,
	.base		= 0x2AA00000u,
	.fwl_ch_start	= 0u,
	.fwl_ch_end	= 47u,
};
static const struct rm_mmr mmr_MCU_NAVSS0_UDMASS_UDMAP0_RCHANRT = {
	.fwl_id		= 6244u,
	.base		= 0x2A800000u,
	.fwl_ch_start	= 48u,
	.fwl_ch_end	= 95u,
};
static const struct udmap_ch_type tx_ch_types_MCU_NAVSS0_UDMAP0[] = {
	{
		.start = 0u,
		.end = 1u,
		.flow_start = 0U,
		.flow_end = 0U,
		.flow_utype = 0U,
		.oes_offset = UDMAP_TCHAN_OES_EVT_INDEX_OFFSET,
		.err_oes_offset = UDMAP_TCHAN_EOES_EVT_INDEX_OFFSET,
		.type = UDMAP_TX_HCHAN,
		.utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_TX_HCHAN),
	},
	{
		.start = 2u,
		.end = 47u,
		.flow_start = 0U,
		.flow_end = 0U,
		.flow_utype = 0U,
		.oes_offset = UDMAP_TCHAN_OES_EVT_INDEX_OFFSET,
		.err_oes_offset = UDMAP_TCHAN_EOES_EVT_INDEX_OFFSET,
		.type = UDMAP_TX_CHAN,
		.utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_TX_CHAN),
	},
};
static const struct udmap_ch_type rx_ch_types_MCU_NAVSS0_UDMAP0[] = {
	{
		.start = 0u,
		.end = 1u,
		.flow_start = 0U,
		.flow_end = 1U,
		.flow_utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_RX_HCHAN),
		.oes_offset = UDMAP_RCHAN_OES_EVT_INDEX_OFFSET,
		.err_oes_offset = UDMAP_RCHAN_EOES_EVT_INDEX_OFFSET,
		.type = UDMAP_RX_HCHAN,
		.utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_RX_HCHAN),
	},
	{
		.start = 2u,
		.end = 47u,
		.flow_start = 2U,
		.flow_end = 47U,
		.flow_utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_RX_CHAN),
		.oes_offset = UDMAP_RCHAN_OES_EVT_INDEX_OFFSET,
		.err_oes_offset = UDMAP_RCHAN_EOES_EVT_INDEX_OFFSET,
		.type = UDMAP_RX_CHAN,
		.utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_RX_CHAN),
	},
};

static u8 flow_delegates_MCU_NAVSS0_UDMAP0[48U];

static struct udmap_flow_common common_flows_MCU_NAVSS0_UDMAP0 = {
	.start		= AM6_MCU_NAVSS_UDMAP_RX_CHAN_MAX_INDEX,
	.end		= AM6_MCU_NAVSS_UDMAP_RX_FLOW_MAX_INDEX - 1U,
	.utype		= RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_UDMAP0,    RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
	.delegates	= &flow_delegates_MCU_NAVSS0_UDMAP0[0U],
};

/*
 * Resources used by ROM which must have their OES and EOES registers reset
 */
static struct udmap_used_mapping rom_usage_mcu_navss0_udmap0[4U] = {
	{
		.ch = 0U,
		.tx_ch = STRUE,
		.err_evt = SFALSE,
		.cleared = SFALSE,
	},
	{
		.ch = 0U,
		.tx_ch = STRUE,
		.err_evt = STRUE,
		.cleared = SFALSE,
	},
	{
		.ch = 0U,
		.tx_ch = SFALSE,
		.err_evt = SFALSE,
		.cleared = SFALSE,
	},
	{
		.ch = 0U,
		.tx_ch = SFALSE,
		.err_evt = STRUE,
		.cleared = SFALSE,
	},
};

struct udmap_instance udmap_inst[2] = {
	{
		/* NAVSS0_UDMAP0 */
		.id = AM6_DEV_NAVSS0_UDMAP0,
		.root_id = AM6_DEV_NAVSS0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MAIN,
		.gcfg = &mmr_NAVSS0_UDMASS_UDMAP0_CFG,
		.tchan = &mmr_NAVSS0_UDMASS_UDMAP0_CFG_TCHAN,
		.rchan = &mmr_NAVSS0_UDMASS_UDMAP0_CFG_RCHAN,
		.bchan = NULL,
		.rflow = &mmr_NAVSS0_UDMASS_UDMAP0_CFG_RFLOW,
		.tchanrt = &mmr_NAVSS0_UDMASS_UDMAP0_CFG_TCHANRT,
		.rchanrt = &mmr_NAVSS0_UDMASS_UDMAP0_CFG_RCHANRT,
		.bchanrt = NULL,
		.n_tx_ch = AM6_MAIN_NAVSS_UDMAP_TX_CHAN_MAX_INDEX,
		.n_rx_ch = AM6_MAIN_NAVSS_UDMAP_RX_CHAN_MAX_INDEX,
		.n_bc_ch = 0U,
		.n_rx_flow = AM6_MAIN_NAVSS_UDMAP_RX_FLOW_MAX_INDEX,
		.bc_ch_offset = 0U,
		.tx_ch_types = &tx_ch_types_NAVSS0_UDMAP0[0],
		.rx_ch_types = &rx_ch_types_NAVSS0_UDMAP0[0],
		.bc_ch_types = NULL,
		.common_flow = &common_flows_NAVSS0_UDMAP0,
		.n_tx_ch_type = ARRAY_SIZE(tx_ch_types_NAVSS0_UDMAP0),
		.n_rx_ch_type = ARRAY_SIZE(rx_ch_types_NAVSS0_UDMAP0),
		.n_bc_ch_type = 0U,
		.invalid_flow_oes_offset = UDMAP_GCFG_INVALID_FLOW_EVT_INDEX,
		.invalid_flow_oes_utype = RESASG_UTYPE(AM6_DEV_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_INVALID_FLOW_OES),
		.gcfg_type = UDMAP_GCFG,
		.gcfg_utype = RESASG_UTYPE(AM6_DEV_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_GLOBAL_CONFIG),
		.rom_usage = NULL,
		.n_rom_usage = 0U,
		.dru_ch0_dst_thread_offset = 0xc800U,
	},
	{
		/* MCU_NAVSS0_UDMAP0 */
		.id = AM6_DEV_MCU_NAVSS0_UDMAP0,
		.root_id = AM6_DEV_MCU_NAVSS0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MCU_WAKEUP,
		.gcfg = &mmr_MCU_NAVSS0_UDMASS_UDMAP0_CFG_GCFG,
		.tchan = &mmr_MCU_NAVSS0_UDMASS_UDMAP0_TCHAN,
		.rchan = &mmr_MCU_NAVSS0_UDMASS_UDMAP0_RCHAN,
		.bchan = NULL,
		.rflow = &mmr_MCU_NAVSS0_UDMASS_UDMAP0_CFG_RFLOW,
		.tchanrt = &mmr_MCU_NAVSS0_UDMASS_UDMAP0_TCHANRT,
		.rchanrt = &mmr_MCU_NAVSS0_UDMASS_UDMAP0_RCHANRT,
		.bchanrt = NULL,
		.n_tx_ch = AM6_MCU_NAVSS_UDMAP_TX_CHAN_MAX_INDEX,
		.n_rx_ch = AM6_MCU_NAVSS_UDMAP_RX_CHAN_MAX_INDEX,
		.n_bc_ch = 0U,
		.n_rx_flow = AM6_MCU_NAVSS_UDMAP_RX_FLOW_MAX_INDEX,
		.bc_ch_offset = 0U,
		.tx_ch_types = &tx_ch_types_MCU_NAVSS0_UDMAP0[0],
		.rx_ch_types = &rx_ch_types_MCU_NAVSS0_UDMAP0[0],
		.bc_ch_types = NULL,
		.common_flow = &common_flows_MCU_NAVSS0_UDMAP0,
		.n_tx_ch_type = ARRAY_SIZE(tx_ch_types_MCU_NAVSS0_UDMAP0),
		.n_rx_ch_type = ARRAY_SIZE(rx_ch_types_MCU_NAVSS0_UDMAP0),
		.n_bc_ch_type = 0U,
		.invalid_flow_oes_offset = UDMAP_GCFG_INVALID_FLOW_EVT_INDEX,
		.invalid_flow_oes_utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_INVALID_FLOW_OES),
		.gcfg_type = UDMAP_GCFG,
		.gcfg_utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_UDMAP0, RESASG_SUBTYPE_UDMAP_GLOBAL_CONFIG),
		.rom_usage = &rom_usage_mcu_navss0_udmap0[0U],
		.n_rom_usage = ARRAY_SIZE(rom_usage_mcu_navss0_udmap0),
		.dru_ch0_dst_thread_offset = 0xc800U,
	},
};

const u32 udmap_inst_count = ARRAY_SIZE(udmap_inst);
