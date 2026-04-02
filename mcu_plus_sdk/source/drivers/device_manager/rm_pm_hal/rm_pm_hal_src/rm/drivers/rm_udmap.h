/*
 * UDMAP
 *
 * UDMAP management infrastructure
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

#ifndef RM_UDMAP_H
#define RM_UDMAP_H

#include <types/sbool.h>
#include <types/short_types.h>

/**
 * Reset value of UDMAP channel's THRD_ID register
 */
#define RM_UDMAP_THRD_ID_RESETVAL    (0x0000u)

/**
 * \brief Confirms managed resource assignment types for UDMAP driver
 *
 * Returns whether or not the provided board cfg resource assignment unique
 * type is managed by UDMAP driver.
 *
 * \param type Resource assignment unique type
 *
 * \return TRUE if resource type is managed by UDMAP driver, else FALSE
 */
sbool rm_udmap_is_managed_resasg_utype(u16 utype);

/**
 * \brief Initializes the UDMAP driver
 *
 * Initializes the UDMAP driver data structures
 *
 * \return
 *      SUCCESS - Initialization completed successfully
 *      -E... - Error occurred during initialization
 */
s32 rm_udmap_init(void);

/**
 * \brief Deinitializes the UDMAP driver for a device group
 *
 * Set the UDMAP driver's initialized flag to false
 *
 * \param devgrp Device Group that has to be uninitialized
 * \param rom_deinit Boolean value to indicate whether to reset
 *                          ROM usage flags or not
 *
 * \return
 *      SUCCESS - Deinitialized the chosen device group successfully
 *      -E... - Failed to deinitialize the chosen device group
 */
s32 rm_udmap_deinit(devgrp_t devgrp, sbool rom_deinit);

/**
 * \brief Get the host ID corresponding to a particular DMA channel
 *
 * \param id Device ID
 *
 * \param index UDMAP tx or rx ch index
 *
 * \param tx_ch Transmit channel if STRUE, Receive channel if SFALSE
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
 *                       host_array
 *
 * \return The host ID value if successful, or -EINVAL
 */
#ifdef CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS
s32 udmap_get_host(u16 id, u16 index, sbool tx_ch, u8 *n_hosts, u8 *hosts_array, u8 host_array_max);
#endif

#endif /* RM_UDMAP_H */
