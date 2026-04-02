/*
 * DMSC RM Core Utilities
 *
 * Resource Manager core utility functions used across RM internal subsystems
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

#ifndef RM_CORE_H
#define RM_CORE_H

#include <types/sbool.h>
#include <types/short_types.h>
#include <types/address_types.h>
#include <types/devgrps.h>

#include <boardcfg/boardcfg_rm_data.h>

/**
 * \brief Resource assignment index
 *
 * \param resasg_indexp Pointer to element of boardcfg resasg array
 *
 * \param utype Unique type of the element pointed to
 *
 * \param len Number of contiguous elements, including pointer, with the same
 *            unique type
 */
struct rm_resasg_index {
	const struct boardcfg_rm_resasg_entry	*resasg_indexp;
	u16					utype;
	u8					len;
};

/**
 * \brief Register field make
 *
 * Inserts a value into a register field preserving all other bits
 *
 * \param shift
 * Register field shift offset
 *
 * \param mask
 * Register field mask
 *
 * \param val
 * Field value
 *
 * \return
 * Extracted register field
 */
static inline u32 rm_fmk(u32 shift, u32 mask, u32 val)
{
	return (val << shift) & mask;
}

/**
 * \brief Register field extract
 *
 * Extracts a value from a register field excluding all other bits
 *
 * \param reg
 * Register value
 *
 * \param shift
 * Register field shift offset
 *
 * \param mask
 * Register field mask
 *
 * \return
 * Extracted register field
 */
static inline u32 rm_fext(u32 reg, u32 shift, u32 mask)
{
	return (reg & mask) >> shift;
}

/**
 * \brief Retrieves a list of hosts assigned to a given resource in the
 *        RM board configuration resource assignment array
 *
 * \param utype Unique type of the resource
 *
 * \param resource_index Index of the resource
 *
 * \param n_hosts Pointer to a variable which is populated with the number
 *                of hosts assigned to the resource in the RM board
 *                configuration
 *
 * \param host_array Pointer to a array variable which is populated with the
 *                   hosts assigned to the resource in the RM board
 *                   configuration
 *
 * \param host_array_max Maximum number of elements in the array pointed to by
 *                       host_array.
 *
 * \return SUCCESS if host_array successfully populated, else -EINVAL
 */
s32 rm_core_get_resasg_hosts(u16 utype, u32 resource_index, u8 *n_hosts, u8 *host_array, u8 host_array_max);

/**
 * \brief Initializes core RM
 *
 * Initializes core RM
 *
 * \return SUCCESS if init completed successfully else error
 */
s32 rm_core_init(void);

/**
 * \brief RAT mapping function for RM drivers
 *
 * Returns the RAT mapped address for the input physical address
 *
 * \param phy_addr
 * Physical address to map
 *
 * \return
 * RAT mapped address
 */
mapped_addr_t rm_core_map_region(soc_phys_addr_t phy_addr);

/**
 * \brief RAT unmapping function for RM drivers
 *
 * Unmaps a RAT region used to map a physical address.
 *
 * TBD - Input will be the RAT mapped address once RAT map and unmap is fully
 * implemented
 */
void rm_core_unmap_region(void);

/**
 * \brief Retrieve the index of the type within the resasg array
 *
 * Returns the resasg indexer entry mapped to the provided utype.  The indexer
 * entry contains the subset of elements within the RM boardcfg resource
 * assignment array mapped to the utype.
 *
 * \param utype
 * Unique resource assignment type
 *
 * \param return
 * Pointer to resasg indexer entry mapped to the utype.
 */
const struct rm_resasg_index *rm_core_resasg_get_utype_index(u16 utype);

/**
 * \brief Validates a resource against the board cfg data
 *
 * Validates a resource index of the provided unique type is mapped to the
 * specified host in the stored board configuration data.
 *
 * \param host
 * Host ID
 *
 * \param utype
 * Unique resource assignment type
 *
 * \param res_index
 * Index of resource
 */
s32 rm_core_resasg_validate_resource(u8 host, u16 utype, u16 res_index);

/**
 * \brief Validates a device's devgrp against the current devgrp
 *
 * Validates a device's devgrp against the current devgrp specified in the RM
 * board configuration.
 *
 * \param dev_id
 * SoC device ID associated with the IP being device group validated
 *
 * \param dev_devgrp
 * Device devgrp
 *
 * \return SUCCESS if device devgrp matches current devgrp, else -EINVAL
 */
s32 rm_core_validate_devgrp(u16 dev_id, devgrp_t dev_devgrp);

/**
 * \brief Check if a parameter is valid
 *
 * Uses a valid_params field and the bit mask for a paramter to check if a
 * parameter is valid.
 *
 * \param valid_params Valid parameter bit field
 *
 * \param param_mask Bit mask for the parameter to validate
 *
 * \return STRUE if parameter is valid, else SFALSE
 */
sbool rm_core_param_is_valid(u32 valid_params, u32 param_mask);

#endif /* RM_CORE_H */
