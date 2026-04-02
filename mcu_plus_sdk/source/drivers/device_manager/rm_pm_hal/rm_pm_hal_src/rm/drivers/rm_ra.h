/*
 * Ring Accelerator
 *
 * Ring Accelerator management infrastructure
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

#ifndef RM_RA_H
#define RM_RA_H

#include <types/sbool.h>
#include <types/short_types.h>

/**
 * \brief Validate ring index
 *
 * Verifies the ring index is valid for the Navigator RA ID and for the
 * resource assignment data
 *
 * \param nav_id RA device ID
 *
 * \param udma_chan_type UDMA channel type
 *
 * \param host Host ID
 *
 * \param index Ring index
 *
 * \return SUCCESS if ring index is valid, else -EINVAL
 */
s32 rm_ra_validate_ring_index(u16 nav_id, u8 udma_chan_type, u8 host, u16 index);

/**
 * \brief Confirms managed resource assignment types for RA driver
 *
 * Returns whether or not the provided board cfg resource assignment unique
 * type is managed by RA driver.
 *
 * \param type Resource assignment unique type
 *
 * \return TRUE if resource type is managed by RA driver, else FALSE
 */
sbool rm_ra_is_managed_resasg_utype(u16 utype);

/**
 * \brief Initializes the RA driver
 *
 * Initializes the RA driver data structures
 *
 * \return
 *      SUCCESS - Initialization completed successfully
 *      -E... - Error occurred during initialization
 */
s32 rm_ra_init(void);

/**
 * \brief Deinitializes the RA driver for a device group
 *
 * Set the RA driver's initialized flag to false
 *
 * \param devgrp Device Group that has to be uninitialized
 * \param rom_deinit Boolean value to indicate whether to reset
 *                          ROM usage flags or not
 *
 * \return
 *      SUCCESS - Deinitialized the chosen device group successfully
 *      -E... - Failed to deinitialize the chosen device group
 */
s32 rm_ra_deinit(devgrp_t devgrp, sbool rom_deinit);

#endif /* RM_RA_H */
