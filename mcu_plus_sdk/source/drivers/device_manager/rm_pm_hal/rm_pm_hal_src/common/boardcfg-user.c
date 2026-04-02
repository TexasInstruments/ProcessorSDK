/*
 * System Firmware Source File
 *
 * Boardcfg API for receiving and storing board configuration
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

#include <resasg_types.h>
#include <hosts.h>
#include <hosts_internal.h>
#include <osal/osal_core.h>
#include <drivers/rat.h>
#include <stdbool.h>
#include <types/address_types.h>
#include <types/devgrps.h>
#include <types/errno.h>
#include <stddef.h>
#include <string.h>

#include <config.h>

#include <boardcfg/boardcfg.h>
#include <boardcfg/boardcfg_user_copy.h>
#include <extboot/extboot.h>
#include <tisci/tisci_protocol.h>
#include <rm.h>

#include <device.h>

#include <lib/ioremap.h>

/**
 * \brief Local data for RM board config data
 *
 * \param config_rm_received Represents whether or not the RM portion of board
 *                           configuration has been received.
 * \param config_rm_valid Represents whether or not the RM portion of board
 *                        configuration has passed validation.
 * \param config_rm_size Size of the RM board configuration data.
 * \param config_rm_devgrp Cumulative device group for the RM configuration.
 * \param config_rm_reserved Realign config_rm_cfg to even byte since
 *                           config_rm_devgrp is only one byte.
 * \param config_rm_cfg Local static copy of all RM board configuration data.
 * \param config_rm_resasg_entries Local static, fixed size resasg array
 */
struct boardcfg_rm_local {
	ftbool				config_rm_received;
	ftbool				config_rm_valid;
	u16				config_rm_size;
	devgrp_t			config_rm_devgrp;
	u8				config_rm_reserved;
	struct boardcfg_rm		config_rm_cfg;
	struct boardcfg_rm_resasg_entry config_rm_resasg_entries[RESASG_ENTRIES_MAX];
};


/* \brief File local static location for RM board config */
static struct boardcfg_rm_local local_rm_config = {
	.config_rm_received	= FT_FALSE,
	.config_rm_valid	= FT_FALSE,
	.config_rm_size		= 0u,
	.config_rm_devgrp	= DEVGRP_ALL,
};

static inline size_t boardcfg_rm_max_size(void)
{
	size_t avail_size;

	avail_size = sizeof(struct boardcfg_rm_local) - offsetof(struct boardcfg_rm_local, config_rm_cfg);

	return avail_size;
}

/**
 * \brief Compare the size value passed to the known size value of boardcfg.
 *
 * \param size Size obtained from substructure header to be compared.
 * \return true if size matches, false otherwise.
 */
static bool boardcfg_validate_size(u16 size, const struct boardcfg_substructure_rm_header *subhdr)
{
	bool ret = true;

	if (size != subhdr->size) {
		ret = false;
	}

	return ret;
}

const struct boardcfg_rm_host_cfg *boardcfg_get_rm_host_cfg(void)
{
	struct boardcfg_rm_host_cfg *host_cfg = NULL;

	if (ft_is_true(local_rm_config.config_rm_valid)) {
		host_cfg = &local_rm_config.config_rm_cfg.host_cfg;
	}

	return host_cfg;
}

/**
 * \brief Ensure the RM host configuration struct in provided RM boardcfg is
 *        valid.
 *
 * Ensure the host configuration portion of the RM boardcfg is valid by
 * checking the magic number and validating the size provided in the
 * substructure header.
 *
 * \param host_cfg Host configuration Struct from provided RM boardcfg.
 *
 * \return true on success, false otherwise.
 */
static bool boardcfg_validate_rm_host_cfg(
	const struct boardcfg_rm_host_cfg *host_cfg)
{
	bool ret = true;

	if (host_cfg == NULL) {
		ret = false;
	}

	if ((ret == true) &&
	    (host_cfg->subhdr.magic != BOARDCFG_RM_HOST_CFG_MAGIC_NUM)) {
		ret = false;
	}

	if (ret == true) {
		ret = boardcfg_validate_size((u16) sizeof(*host_cfg),
					     &host_cfg->subhdr);
	}

	return ret;
}


/**
 * \brief Ensure the RM resasg struct in provided RM boardcfg is valid.
 *
 * Ensure the resasg portion of the RM boardcfg is valid by checking the magic
 * number and validating the size provided in the substructure header.
 *
 * \param resasg Resasg Struct from provided RM boardcfg.
 *
 * \return true on success, false otherwise.
 */
static bool boardcfg_validate_rm_resasg(const struct boardcfg_rm_resasg *resasg)
{
	bool ret = true;

	if (resasg == NULL) {
		ret = false;
	}

	if ((ret == true) &&
	    (resasg->subhdr.magic != BOARDCFG_RM_RESASG_MAGIC_NUM)) {
		ret = false;
	}

	if (ret == true) {
		ret = boardcfg_validate_size((u16) sizeof(*resasg), &resasg->subhdr);
	}

	if (ret == true) {
		if ((resasg->resasg_entries_size == 0u) ||
		    ((resasg->resasg_entries_size >
		      (RESASG_ENTRIES_MAX *
		       sizeof(struct boardcfg_rm_resasg_entry))) ||
		     ((resasg->resasg_entries_size %
		       sizeof(struct boardcfg_rm_resasg_entry)) > 0u))) {
			ret = false;
		}
	}

	return ret;
}

/**
 * \brief Ensure that the boardcfg ABI revision matches what is expected.
 *
 * \param rev Struct from provided boardcfg.
 *
 * \param maj Major board Config data ABI version
 *
 * \param min Minor board Config data ABI version
 *
 * \return true on success, false otherwise.
 */
static bool boardcfg_validate_abi_rev(const struct boardcfg_abi_rm_rev *rev,
				      u8 maj, u8 min)
{
	bool ret = true;

	if ((rev->boardcfg_abi_maj != maj) ||
	    (rev->boardcfg_abi_min != min)) {
		ret = false;
	}

	return ret;
}

#if defined(CONFIG_DEVICE_TYPE_GP) || defined(CONFIG_LIBRARY_BUILD_DM)
/**
 * \brief memcpy to be used specifically for copying boardcfg binary
 *
 * This is used in the following build configurations:
 * - GP SYSFW, to copy RM boardcfg data to local DMSC memory
 * - GP and HS DM to copy RM boardcfg data to local DM memory. On HS, boardcfg
 *   data is authenticated in TIFS prior to this copy.
 *
 * \return SUCCESS always.
 */
static s32 boardcfg_memcpy(local_phys_addr_t to, soc_phys_addr_t from, u32 size)
{
	s32 ret;
	ftbool mapped = FT_TRUE;
	mapped_addr_t from_mapped = 0U;
	volatile u32 from_mapped_addr = 0U;

	ret = rat_map_tmp_region_user(from, &from_mapped);

	/* EINVAL would mean that the address is already in the mapped regions
	 */
	if (ret == -EINVAL) {
		ret = SUCCESS;
		mapped = FT_FALSE;
	}

	from_mapped_addr = ioremap(soc_phys_low_u32(from));

	memcpy((void *) to, (void *) from_mapped_addr, size);

	if (mapped == FT_TRUE) {
		ret = rat_unmap_tmp_region_user(from);
	}

	return ret;
}

/*
 * NOTE: The HS implementation of boardcfg_memcpy_rm is in the file
 * common/boardcfg.c as the function needs to access static data
 * structure defined in that file.
 */
s32 boardcfg_memcpy_rm(u8 host __attribute__((unused)),
		       local_phys_addr_t to, soc_phys_addr_t from,
		       u32 size, u32 max_size)
{
	s32 ret;

	if (size <= max_size) {
		ret = boardcfg_memcpy(to, from, size);
	} else {
		ret = -EFAIL;
	}
	return ret;
}
#endif

s32 boardcfg_rm_receive_and_validate(u8		host,
				     u32	boardcfg_rmp_low,
				     u32	boardcfg_rmp_high,
				     u16	boardcfg_rm_size,
				     devgrp_t	boardcfg_rm_devgrp)
{
	s32 ret = SUCCESS;
	bool valid_check;
	soc_phys_addr_t boardcfg_addr;
	size_t bcfg_rm_max_size = boardcfg_rm_max_size();

	u8 *validp = NULL;

	if (local_rm_config.config_rm_received == FT_FALSE) {
		local_rm_config.config_rm_received = FT_TRUE;

		/*
		* switch to legacy mode if extended boot information is not present
		*/
		if (ft_is_false(extboot_is_present())) {
			boardcfg_addr = soc_phys_create(boardcfg_rmp_low,
							boardcfg_rmp_high);

			ret = boardcfg_memcpy_rm(host,
						 (local_phys_addr_t) &local_rm_config.config_rm_cfg,
						 boardcfg_addr, boardcfg_rm_size, bcfg_rm_max_size);
		}
		/*
		* When combined image format config is enabled and extended boot
		* information is present, ROM has already copied boardcfg to DMSC IRAM.
		* Just do a memcpy, but after a bound check
		*
		* boardcfg_rm_size includes boardcfg_rm and resource entry.
		*/
		else if (ft_is_true(extboot_is_valid()) &&
			 ft_is_true(extboot_boardcfg_is_present(TISCI_MSG_BOARD_CONFIG_RM)) &&
			 (boardcfg_rm_size <= (sizeof(struct boardcfg_rm) +
					       (sizeof(struct boardcfg_rm_resasg_entry) * RESASG_ENTRIES_MAX)))) {
			validp = (u8 *) memcpy(&local_rm_config.config_rm_cfg, (struct boardcfg_rm *) boardcfg_rmp_low,
					       boardcfg_rm_size);
			if (validp == NULL) {
				ret = -EINVAL;
			}
		} else {
			ret = -EINVAL;
		}

		if (ret == SUCCESS) {
			valid_check = boardcfg_validate_abi_rev(
				&local_rm_config.config_rm_cfg.rev,
				BOARDCFG_RM_ABI_MAJ_VALUE,
				BOARDCFG_RM_ABI_MIN_VALUE);
			if (valid_check == false) {
				ret = -EINVAL;
			}
		}

		if (ret == SUCCESS) {
			valid_check = boardcfg_validate_rm_host_cfg(
				&local_rm_config.config_rm_cfg.host_cfg);
			if (valid_check == false) {
				ret = -EINVAL;
			}
		}

		if (ret == SUCCESS) {
			valid_check = boardcfg_validate_rm_resasg(
				&local_rm_config.config_rm_cfg.resasg);
			if (valid_check == false) {
				ret = -EINVAL;
			}
		}

		if (ret == SUCCESS) {
			local_rm_config.config_rm_valid = FT_TRUE;
		}
	}

	if ((ret == SUCCESS) &&
	    (local_rm_config.config_rm_valid == FT_TRUE)) {
		/* Enable all bits in rm_devgrp for DEVGRP_ALL board config parameter */
		if (boardcfg_rm_devgrp == DEVGRP_ALL) {
			local_rm_config.config_rm_devgrp = DEVGRP_DMSC_ALL;
		} else {
			local_rm_config.config_rm_devgrp |= boardcfg_rm_devgrp;
		}
	}

	return ret;
}

u16 boardcfg_get_rm_resasg_size(void)
{
	u16 resasg_size = 0u;

	if (ft_is_true(local_rm_config.config_rm_valid)) {
		resasg_size = local_rm_config.config_rm_cfg.resasg.resasg_entries_size;
	}

	return resasg_size;
}

const struct boardcfg_rm_resasg_entry *boardcfg_get_rm_resasg(void)
{
	struct boardcfg_rm_resasg_entry *resasg = NULL;

	if (ft_is_true(local_rm_config.config_rm_valid)) {
		resasg = &local_rm_config.config_rm_resasg_entries[0];
	}

	return resasg;
}

s32 boardcfg_get_rm_devgrp(devgrp_t *devgrp)
{
	s32 r = -EINIT;

	if (devgrp == NULL) {
		r = -EINVAL;
	} else {
		if (ft_is_true(local_rm_config.config_rm_valid)) {
			*devgrp = local_rm_config.config_rm_devgrp;
			r = SUCCESS;
		}
	}

	return r;
}

s32 boardcfg_pm_receive_and_validate(u8		host __attribute__((unused)),
				     u32	boardcfg_pmp_low __attribute__((unused)),
				     u32	boardcfg_pmp_high __attribute__((unused)),
				     u16	boardcfg_pm_size __attribute__((unused)),
				     devgrp_t	boardcfg_pm_devgrp)
{
	s32 ret = SUCCESS;
	devgrp_t devgrp = boardcfg_pm_devgrp;

	if ((devgrp & (DEVGRP_04 | DEVGRP_05 | DEVGRP_HSM)) != 0U) {
		ret = -EINVAL;
	} else if (devgrp == DEVGRP_ALL) {
		devgrp = DEVGRP_DMSC_ALL;
	} else {
		if ((devgrp & DEVGRP_03) != 0U) {
			devgrp |= DEVGRP_02;
		}
		if ((devgrp & DEVGRP_02) != 0U) {
			devgrp |= DEVGRP_01;
		}
		if ((devgrp & DEVGRP_01) != 0U) {
			devgrp |= DEVGRP_00;
		}
		if ((devgrp & DEVGRP_00) != 0U) {
			devgrp |= DEVGRP_DMSC;
		}
	}

	if (ret == SUCCESS) {
		pm_devgroup_set_enabled(devgrp);
	}

	/*
	 * We do not do any validation of the PM boardcfg data on HS devices
	 * as the data is not consumed. if and when the PM data is consumed,
	 * the same functions used for RM can be used for validating PM data
	 * as well.
	 *
	 */
	return ret;
}

/* \brief Wrapper function to trigger auto boardcfg processing */
s32 boardcfg_user_auto(u16 type, boardcfg_process_fxn fxn)
{
	s32 ret = SUCCESS;
	u32 base = 0x0;
	const struct extboot_boardcfg_desc *desc = NULL;

	base = extboot_get_base();
	if (base == 0U) {
		ret = -EINVAL;
	}

	if (ret == SUCCESS) {
		desc = extboot_boardcfg_get_desc(type);
	}

	if (desc == NULL) {
		ret = -EINVAL;
	}

	if (ret == SUCCESS) {
		ret = fxn(HOST_ID_TIFS, base + desc->offset, 0x0,
			  desc->size, desc->devgrp);
	}

	if (ret == SUCCESS) {
		extboot_boardcfg_set_valid(type);
	}

	return ret;
}
