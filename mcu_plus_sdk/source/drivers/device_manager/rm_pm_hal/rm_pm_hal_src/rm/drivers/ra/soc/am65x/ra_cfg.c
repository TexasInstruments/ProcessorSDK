/*
 * Ring Accelerator
 *
 * Ring Accelerator SoC data for AM6
 *
 * Copyright (C) 2018-2020 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <types/short_types.h>
#include <types/sbool.h>
#include <types/array_size.h>
#include <soc/rm_mmr.h>
#include <devices.h>
#include <soc/am65x/resasg_types.h>
#include <soc/am65x/soc_devgrps.h>

#include "../../include/ra_inst.h"
#include <ra_cfg.h>

/* Start of NAVSS0_RINGACC0 instance definition */
static const struct rm_mmr mmr_NAVSS0_UDMASS_RINGACC0_GCFG = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x31160000u,
};
static const struct rm_mmr mmr_NAVSS0_UDMASS_RINGACC0_CFG = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x31080000u,
};
static const struct rm_mmr mmr_NAVSS0_UDMASS_RINGACC0_CFG_RT = {
	.fwl_id		= 4256u,
	.base		= 0x3C000000u,
	.fwl_ch_start	= 0u,
	.fwl_ch_end	= 767u,
};
static const struct rm_mmr mmr_NAVSS0_UDMASS_RINGACC0_CFG_MON = {
	.fwl_id		= 4256u,
	.base		= 0x32000000u,
	.fwl_ch_start	= 818u,
	.fwl_ch_end	= 849u,
};
static const struct rm_mmr mmr_NAVSS0_UDMASS_RINGACC0_SRC_FIFOS = {
	.fwl_id		= 4416u,
	.base		= 0x38000000u,
	.fwl_ch_start	= 0u,
	.fwl_ch_end	= 767u,
};
static const struct ra_ring_type ring_types_NAVSS0_RINGACC0[6] = {
	{
		.start = 0U,
		.end = 7U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_UDMAP_TX_H),
	},
	{
		.start = 8U,
		.end = 119U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_UDMAP_TX),
	},
	{
		.start = 120U,
		.end = 151U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_UDMAP_TX_EXT),
	},
	{
		.start = 152U,
		.end = 159U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_UDMAP_RX_H),
	},
	{
		.start = 160U,
		.end = 301U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_UDMAP_RX),
	},
	{
		.start = 302U,
		.end = 767U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_GP),
	},
};

static const struct ra_ring_type ring_mon_types_NAVSS0_RINGACC0[1] = {
	{
		.start = 0U,
		.end = 31U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_MONITORS),
	},
};

static struct ra_used_mapping rom_usage_NAVSS0_RINGACC[2U] = {
	{
		.ring = 303U,
		.cleared = SFALSE,
	},
	{
		.ring = 304U,
		.cleared = SFALSE,
	},
};

/* Start of MCU_NAVSS0_RINGACC0 instance definition */
static const struct rm_mmr mmr_MCU_NAVSS0_UDMASS_RINGACC0_GCFG = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x285D0000u,
};
static const struct rm_mmr mmr_MCU_NAVSS0_UDMASS_RINGACC0_CFG = {
	.fwl_id = RM_MMR_FWL_INVALID,
	.base	= 0x28440000u,
};
static const struct rm_mmr mmr_MCU_NAVSS0_UDMASS_RINGACC0_CFG_RT = {
	.fwl_id		= 6160u,
	.base		= 0x2B800000u,
	.fwl_ch_start	= 0u,
	.fwl_ch_end	= 255u,
};
static const struct rm_mmr mmr_MCU_NAVSS0_UDMASS_RINGACC0_CFG_MON = {
	.fwl_id		= 6160u,
	.base		= 0x2A280000u,
	.fwl_ch_start	= 286u,
	.fwl_ch_end	= 317u,
};
static const struct rm_mmr mmr_MCU_NAVSS0_UDMASS_RINGACC0_FIFOS = {
	.fwl_id		= 6272u,
	.base		= 0x2B000000u,
	.fwl_ch_start	= 0u,
	.fwl_ch_end	= 255u,
};
static const struct ra_ring_type ring_types_MCU_NAVSS0_RINGACC0[5] = {
	{
		.start = 0U,
		.end = 1U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_UDMAP_TX_H),
	},
	{
		.start = 2U,
		.end = 47U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_UDMAP_TX),
	},
	{
		.start = 48U,
		.end = 49U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_UDMAP_RX_H),
	},
	{
		.start = 50U,
		.end = 95U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_UDMAP_RX),
	},
	{
		.start = 96U,
		.end = 255U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_GP),
	},
};

static const struct ra_ring_type ring_mon_types_MCU_NAVSS0_RINGACC0[1] = {
	{
		.start = 0U,
		.end = 31U,
		.type = RA_STANDARD_RING,
		.utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_MONITORS),
	},
};


struct ra_instance ra_inst[2] = {
	{
		/* NAVSS0_RINGACC0 */
		.id = AM6_DEV_NAVSS0_RINGACC0,
		.root_id = AM6_DEV_NAVSS0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_MAIN,
		.gcfg = &mmr_NAVSS0_UDMASS_RINGACC0_GCFG,
		.cfg = &mmr_NAVSS0_UDMASS_RINGACC0_CFG,
		.rt = &mmr_NAVSS0_UDMASS_RINGACC0_CFG_RT,
		.mon = &mmr_NAVSS0_UDMASS_RINGACC0_CFG_MON,
		.fifos = &mmr_NAVSS0_UDMASS_RINGACC0_SRC_FIFOS,
		.n_gp_rings_pre_sp_rings = RA_NAVSS0_RINGACC0_MAX_INDEX_PRE_SP,
		.n_gp_rings_post_sp_rings = RA_NAVSS0_RINGACC0_MAX_INDEX_POST_SP,
		.gp_ring_oes_offset_pre_sp = RA_NAVSS0_RINGACC0_RING_OES_EVT_INDEX_OFFSET_PRE_SP,
		.gp_ring_oes_offset_post_sp = RA_NAVSS0_RINGACC0_RING_OES_EVT_INDEX_OFFSET_POST_SP,
		.ring_types = &ring_types_NAVSS0_RINGACC0[0],
		.n_ring_type = ARRAY_SIZE(ring_types_NAVSS0_RINGACC0),
		.ring_mon_types = &ring_mon_types_NAVSS0_RINGACC0[0U],
		.n_ring_mon_type = ARRAY_SIZE(ring_mon_types_NAVSS0_RINGACC0),
		.err_evt_index = RA_GCFG_ERROR_EVT_INDEX,
		.err_evt_utype = RESASG_UTYPE(AM6_DEV_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_ERROR_OES),
		.virtid_utype = RESASG_UTYPE(AM6_DEV_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_VIRTID),
		.n_ring_mon = AM6_MAIN_NAVSS_RING_MONITOR_MAX_INDEX,
		.ring_mon_utype = RESASG_UTYPE(AM6_DEV_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_MONITORS),
		.ring_mon_oes_offset = RA_MON_OES_EVT_INDEX_OFFSET,
		.rom_usage = &rom_usage_NAVSS0_RINGACC[0U],
		.n_rom_usage = ARRAY_SIZE(rom_usage_NAVSS0_RINGACC),
	},
	{
		/* MCU_NAVSS0_RINGACC0 */
		.id = AM6_DEV_MCU_NAVSS0_RINGACC0,
		.root_id = AM6_DEV_MCU_NAVSS0,
		.initialized = SFALSE,
		.devgrp = SOC_DEVGRP_AM6_DMSC_INTERNAL | SOC_DEVGRP_AM6_MCU_WAKEUP,
		.gcfg = &mmr_MCU_NAVSS0_UDMASS_RINGACC0_GCFG,
		.cfg = &mmr_MCU_NAVSS0_UDMASS_RINGACC0_CFG,
		.rt = &mmr_MCU_NAVSS0_UDMASS_RINGACC0_CFG_RT,
		.mon = &mmr_MCU_NAVSS0_UDMASS_RINGACC0_CFG_MON,
		.fifos = &mmr_MCU_NAVSS0_UDMASS_RINGACC0_FIFOS,
		.n_gp_rings_pre_sp_rings = RA_MCU_NAVSS0_RINGACC0_MAX_INDEX_PRE_SP,
		.n_gp_rings_post_sp_rings = RA_MCU_NAVSS0_RINGACC0_MAX_INDEX_POST_SP,
		.gp_ring_oes_offset_pre_sp = RA_MCU_NAVSS0_RINGACC0_RING_OES_EVT_INDEX_OFFSET_PRE_SP,
		.gp_ring_oes_offset_post_sp = RA_MCU_NAVSS0_RINGACC0_RING_OES_EVT_INDEX_OFFSET_POST_SP,
		.ring_types = &ring_types_MCU_NAVSS0_RINGACC0[0],
		.n_ring_type = ARRAY_SIZE(ring_types_MCU_NAVSS0_RINGACC0),
		.ring_mon_types = &ring_mon_types_MCU_NAVSS0_RINGACC0[0U],
		.n_ring_mon_type = ARRAY_SIZE(ring_mon_types_MCU_NAVSS0_RINGACC0),
		.err_evt_index = RA_GCFG_ERROR_EVT_INDEX,
		.err_evt_utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_ERROR_OES),
		.virtid_utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_VIRTID),
		.n_ring_mon = AM6_MCU_NAVSS_RING_MONITOR_MAX_INDEX,
		.ring_mon_utype = RESASG_UTYPE(AM6_DEV_MCU_NAVSS0_RINGACC0, RESASG_SUBTYPE_RA_MONITORS),
		.ring_mon_oes_offset = RA_MON_OES_EVT_INDEX_OFFSET,
		.rom_usage = NULL,
		.n_rom_usage = 0U,
	},
};

const u32 ra_inst_count = ARRAY_SIZE(ra_inst);
