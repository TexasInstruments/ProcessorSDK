/*
 * System Firmware Source File
 *
 * Board Configuration Data Structures
 *
 * Copyright (C) 2018-2024, Texas Instruments Incorporated
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

#ifndef BOARD_CFG_RM_H
#define BOARD_CFG_RM_H

#include <lib/bitops.h>
#include <types/ftbool.h>
#include <types/devgrps.h>
#include <types/short_types.h>
#include <hosts.h>

#define BOARDCFG_RM_HOST_CFG_MAGIC_NUM          (0x4C41U)
#define BOARDCFG_RM_RESASG_MAGIC_NUM            (0x7B25U)

#define BOARDCFG_RM_ABI_MAJ_VALUE               (0x00)
#define BOARDCFG_RM_ABI_MIN_VALUE               (0x01)

/**
 * \brief Contains a unique magic number for each substructure and the size
 *	  of the associated superstructure for data validation/API
 *	  compatibility checks.
 *
 * \param magic Unique magic number for data integrity check.
 * \param size `sizeof(superstructure containing this header)` for data
 *	       integrity check.
 */
struct boardcfg_substructure_rm_header {
	u16	magic;
	u16	size;
} __attribute__((__packed__));

/**
 * \brief Board Config data ABI version.
 *
 * \param boardcfg_abi_maj Major Board Config data ABI version.
 * \param boardcfg_abi_min Minor Board Config data ABI version.
 */
struct boardcfg_abi_rm_rev {
	u8	boardcfg_abi_maj;
	u8	boardcfg_abi_min;
} __attribute__((__packed__));

/**
 * \brief Resource Management host ID configuration entry describing a
 *        host's allowed system priorities as well as a host's supervisor
 *
 * \param host_id Processing entity host ID
 *
 * \param allowed_qos Deprecated. No longer used by SYSFW
 *
 * \param allowed_orderid Deprecated. No longer used by SYSFW
 *
 * \param allowed_atype Deprecated. No longer used by SYSFW
 *
 * \param allowed_priority Deprecated. No longer used by SYSFW
 *
 * \param allowed sched_priority Deprecated. No longer used by SYSFW
 */
struct boardcfg_rm_host_cfg_entry {
	u8	host_id;
	u8	allowed_atype;
	u16	allowed_qos;
	u32	allowed_orderid;
	u16	allowed_priority;
	u8	allowed_sched_priority;
} __attribute__((__packed__));

/* \brief How many host configuration list entries */
#define BOARDCFG_RM_HOST_CFG_ENTRIES (32U)

/**
 * \brief Resource Management host ID configuration describing a host's allowed
 *        system priorities
 *
 * \param subhdr Magic and size for integrity check.
 * \param host_cfg_entries @ref boardcfg_rm_host_cfg_entry entries up to
 *                         @ref BOARDCFG_RM_HOST_CFG_ENTRIES
 */
struct boardcfg_rm_host_cfg {
	struct boardcfg_substructure_rm_header	subhdr;
	struct boardcfg_rm_host_cfg_entry	host_cfg_entries[
		BOARDCFG_RM_HOST_CFG_ENTRIES];
};

/**
 * \brief One contiguous block of any resource to be assigned to one
 *	  processing entity.
 *
 * \param start_resource First resource number (inclusive) for a host_id
 *			 and resource.
 * \param num_resource Number of continuous resources allocated to a
 *		       host_id and resource. 0 means unused.
 * \param type Unique type value identifying the specific resource to which the
 *             range assignment applies
 * \param host_id Processing entity host ID to which the resource range is
 *                assigned
 */
struct boardcfg_rm_resasg_entry {
	u16	start_resource;
	u16	num_resource;
	u16	type;
	u8	host_id;
	u8	reserved;
};

/**
 * \brief RM boardcfg resource assignment configuration.
 *
 * \param subhdr Magic and size for integrity check.
 * \param resasg_entries_size Size of resasg_entries array in bytes
 * \param resasg_entries Array of @ref boardcfg_rm_resasg_entry containing
 *                       resource range assignments to hosts.
 */
struct boardcfg_rm_resasg {
	struct boardcfg_substructure_rm_header	subhdr;
	u16					resasg_entries_size;
	u16					reserved;
} __attribute__((__packed__));

/**
 * \brief Resource Management configuration.
 *
 * \param rev Resource Management board config ABI version (separate from Board
 *            Config ABI version)
 * \param host_cfg Array of host configuration structures.
 * \param resasg Resource assignment configuration structure.
 */
struct boardcfg_rm {
	struct boardcfg_abi_rm_rev	rev;
	struct boardcfg_rm_host_cfg	host_cfg;
	struct boardcfg_rm_resasg	resasg;
} __attribute__((__packed__));

/**
 * \brief Get a pointer to the local copy of the boardcfg_rm_host_cfg data.
 *
 * \return NULL if the local RM board data has not yet been received or is
 *         invalid, pointer otherwise.
 */
const struct boardcfg_rm_host_cfg *boardcfg_get_rm_host_cfg(void);

/**
 * \brief Get a the size of the local copy of the boardcfg_rm_resasg_entry data.
 *
 * \return Zero if the local RM board data has not yet been received or is
 *         invalid, greater than zero otherwise.
 */
u16 boardcfg_get_rm_resasg_size(void);

/**
 * \brief Get a pointer to the local copy of the boardcfg_rm_resasg data.
 *
 * \return NULL if the local RM board data has not yet been received or is
 *         invalid, pointer otherwise.
 */
const struct boardcfg_rm_resasg_entry *boardcfg_get_rm_resasg(void);

/**
 * \brief Get the RM device group from the local boardcfg_rm data.
 *
 * \param devgrp Pointer to a device group variable in which the device group
 *        is returned on SUCCESS
 *
 * \return EINIT if the local RM board data has not yet been received or is
 *         invalid, SUCCESS if in the board data has been received and is valid.
 */
s32 boardcfg_get_rm_devgrp(devgrp_t *devgrp);

#endif          /* BOARD_CFG_RM_H */
