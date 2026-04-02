/*
 * Interrupt Management
 *
 * Interrupt management driver instance data definition
 *
 * Data version: 250223_004013
 *
 * Copyright (C) 2022-2025 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <types/short_types.h>
#include <types/array_size.h>
#include <devices.h>
#include <devices_internal.h>
#include <hosts.h>
#include <hosts_internal.h>
#include <soc/j784s4/resasg_types.h>

#include "../../include/irq_resasg.h"
#include <irq_cfg.h>

/*
 * SoC devices capable of sourcing global events
 */
const u16 evt_rt_srcs[10] = {
	J784S4_DEV_NAVSS0_PROXY_0,
	J784S4_DEV_NAVSS0_RINGACC_0,
	J784S4_DEV_NAVSS0_TIMERMGR_0,
	J784S4_DEV_NAVSS0_TIMERMGR_1,
	J784S4_DEV_NAVSS0_UDMAP_0,
	J784S4_DEV_NAVSS0_UDMASS_INTA_0,
	J784S4_DEV_MCU_NAVSS0_PROXY0,
	J784S4_DEV_MCU_NAVSS0_RINGACC0,
	J784S4_DEV_MCU_NAVSS0_UDMAP_0,
	J784S4_DEV_MCU_NAVSS0_UDMASS_INTA_0,
};

const u32 evt_rt_srcs_count = ARRAY_SIZE(evt_rt_srcs);

const struct irq_global_event_type irq_global_event_types[IRQ_GLOBAL_EVENT_TYPES_ID_MAX] = {
	{
		.start = 0U,
		.num = 4608U,
		.utype = RESASG_UTYPE(J784S4_DEV_NAVSS0_UDMASS_INTA_0, RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
	},
	{
		.start = 16384U,
		.num = 1536U,
		.utype = RESASG_UTYPE(J784S4_DEV_MCU_NAVSS0_UDMASS_INTA_0, RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
	},
	{
		.start = 20480U,
		.num = 1024U,
		.utype = RESASG_UTYPE(J784S4_DEV_NAVSS0_MODSS_INTA_0, RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
	},
	{
		.start = 22528U,
		.num = 1024U,
		.utype = RESASG_UTYPE(J784S4_DEV_NAVSS0_MODSS_INTA_1, RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
	},
	{
		.start = 32768U,
		.num = 512U,
		.utype = RESASG_UTYPE(J784S4_DEV_NAVSS0_UDMASS_INTA_0, RESASG_SUBTYPE_GLOBAL_EVENT_MEVT),
	},
	{
		.start = 34816U,
		.num = 128U,
		.utype = RESASG_UTYPE(J784S4_DEV_MCU_NAVSS0_UDMASS_INTA_0, RESASG_SUBTYPE_GLOBAL_EVENT_MEVT),
	},
	{
		.start = 36864U,
		.num = 512U,
		.utype = RESASG_UTYPE(J784S4_DEV_NAVSS0_UDMASS_INTA_0, RESASG_SUBTYPE_GLOBAL_EVENT_GEVT),
	},
	{
		.start = 39936U,
		.num = 256U,
		.utype = RESASG_UTYPE(J784S4_DEV_MCU_NAVSS0_UDMASS_INTA_0, RESASG_SUBTYPE_GLOBAL_EVENT_GEVT),
	},
	{
		.start = 49152U,
		.num = 1024U,
		.utype = RESASG_UTYPE(J784S4_DEV_NAVSS0_UDMAP_0, RESASG_SUBTYPE_GLOBAL_EVENT_TRIGGER),
	},
	{
		.start = 50176U,
		.num = 96U,
		.utype = RESASG_UTYPE(J784S4_DEV_NAVSS0_BCDMA_0, RESASG_SUBTYPE_GLOBAL_EVENT_TRIGGER),
	},
	{
		.start = 56320U,
		.num = 256U,
		.utype = RESASG_UTYPE(J784S4_DEV_MCU_NAVSS0_UDMAP_0, RESASG_SUBTYPE_GLOBAL_EVENT_TRIGGER),
	},
};
