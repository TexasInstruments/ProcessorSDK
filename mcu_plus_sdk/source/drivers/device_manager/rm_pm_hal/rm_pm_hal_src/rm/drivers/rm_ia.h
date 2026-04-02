/*
 * IRQ Interrupt Aggregator
 *
 * IRQ management interrupt aggregator infrastructure
 *
 * Copyright (C) 2018-2025, Texas Instruments Incorporated
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

#ifndef RM_IA_H
#define RM_IA_H

#include <types/sbool.h>
#include <types/short_types.h>

/**
 * \brief Confirms managed resource assignment types for IA driver
 *
 * Returns whether or not the provided board cfg resource assignment unique
 * type is managed by IA driver.
 *
 * \param type Resource assignment unique type
 *
 * \return TRUE if resource type is managed by IA driver, else FALSE
 */
sbool rm_ia_is_managed_resasg_utype(u16 utype);

/**
 * \brief Checks if an IA virtual interrupt event mapping is available
 *
 * Validates the VINT against the board configuration resasg data.  Then
 * checks the IA virtual interrupt usage count to see if an event to status
 * bit mapping is available.
 *
 * \param host
 * Host ID
 *
 * \param id
 * IA SoC device ID.
 *
 * \param vint
 * IA virtual interrupt output
 *
 * \return SUCCESS if a mapping is available, else -EINVAL
 */
s32 rm_ia_validate_vint(u8 host, u16 id, u16 vint);

/**
 * \brief Maps an unused IA SEVI to a existing virtual interrupt
 *
 * Adds an additional SEVI to the specified virtual interrupt as
 * long as the specified status but is unused.
 *
 * \param id
 * IA SoC device ID
 *
 * \param vint
 * IA virtual interrupt
 *
 * \param global_evt
 * Global event to map to VINT status bit
 *
 * \param vint_sb_index
 * Virtual interrupt status bit to map to global event
 *
 * \param fwl_cfg_dmsc_only STRUE to explicitly program only DMSC with RW access
 *
 * \param validate_sec_rm_devgrp STRUE to validate against the configured
 *                               Secure RM devgrp
 *
 * \return SUCCESS if addition to VINT is successful, else error
 */
s32 rm_ia_vint_map(u16 id, u16 vint, u16 global_evt, u8 vint_sb_index, sbool fwl_cfg_dmsc_only, sbool validate_sec_rm_devgrp);

/**
 * \brief Unmaps an IA SEVI from an existing virtual interrupt
 *
 * Removes the specified global event to status bit mapping from the
 * specified virtual interrupt.
 *
 * \param id
 * IA SoC device ID
 *
 * \param vint
 * IA virtual interrupt
 *
 * \param global_evt
 * global event within vint
 *
 * \param vint_sb_index
 * Virtual interrupt status bit to unmap from global event
 *
 * \return SUCCESS if removal from VINT is successful, else error
 */
s32 rm_ia_vint_unmap(u16 id, u16 vint, u16 global_evt, u8 vint_sb_index);

/**
 * \brief Retrieves virtual interrupt info
 *
 * Returns a virtual interrupt's number of events currently mapped to status
 * bits.
 *
 * \param id
 * IA SoC device ID
 *
 * \param vint
 * IA virtual interrupt
 *
 * \param n_evts
 * Pointer to current number of events mapped by the virtual interrupt
 *
 * \return
 *      SUCCESS - Get info was successful, else error
 */
s32 rm_ia_vint_get_info(u16 id, u16 vint, u8 *n_evts);

/**
 * \brief Initializes the IA driver
 *
 * Initializes the IA driver data structures
 *
 * \return
 * Status of initialization
 *      SUCCESS - Initialization completed successfully
 *      -E... - Initialization completed unsuccessfully.
 */
s32 rm_ia_init(void);

/**
 * \brief Deinitializes the IA driver for a device group
 *
 * Set the IA driver's initialized flag to false
 *
 * \param devgrp Device Group that has to be uninitialized
 * \param rom_deinit Boolean value to indicate whether to reset
 *                          ROM usage flags or not
 *
 * \return
 *      SUCCESS - Deinitialized the chosen device group successfully
 *      -E... - Failed to de-initialize the chosen device group
 */
s32 rm_ia_deinit(devgrp_t devgrp, sbool rom_deinit);

#endif /* RM_IA_H */
