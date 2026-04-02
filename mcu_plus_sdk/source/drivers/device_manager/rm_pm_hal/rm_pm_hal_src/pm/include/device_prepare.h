/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2019-2023, Texas Instruments Incorporated
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

#ifndef DEVICE_PREPARE_H
#define DEVICE_PREPARE_H

#include <types/short_types.h>

struct device;

/**
 * \brief Prepare to operate on device with exclusive check
 *
 * This function takes the steps necessary for the PM subsystem to prepare
 * to operate on a device when receiving a TISCI request. It ensures that
 * if the device is marked exclusive, we are the owner. This version should
 * be used if device properties will be modified.
 *
 * Appropriate trace messages are produced if errors are encountred.
 *
 * \param host_id
 * The host ID making the request.
 *
 * \param id
 * The device ID to operate on.
 *
 * \param host_idx
 * If this parameter is non-NULL and the host index lookup succeeds, the
 * pointer is filled with the host index value.
 *
 * \param device_ptr
 * If this parameter is non-NULL and the device ID lookup succeeds, the
 * pointer is filled with the device pointer.
 *
 * \return
 * SUCCESS if all checks succeeded, less than zero otherwise.
 */
s32 device_prepare_exclusive(u8 host_id, u32 id, u8 *host_idx, struct device **device_ptr);

/**
 * \brief Prepare to operate on device with exclusive check
 *
 * This function takes the steps necessary for the PM subsystem to prepare
 * to operate on a device when receiving a TISCI request. No check is made
 * in regard to the exclusive state of the device. This version should only
 * be used if the device properties will not be modified.
 *
 * Appropriate trace messages are produced if errors are encountred.
 *
 * \param host_id
 * The host ID making the request.
 *
 * \param id
 * The device ID to operate on.
 *
 * \param host_idx
 * If this parameter is non-NULL and the host index lookup succeeds, the
 * pointer is filled with the host index value.
 *
 * \param device_ptr
 * If this parameter is non-NULL and the device ID lookup succeeds, the
 * pointer is filled with the device pointer.
 *
 * \return
 * SUCCESS if all checks succeeded, less than zero otherwise.
 */
s32 device_prepare_nonexclusive(u8 host_id, u32 id, u8 *host_idx, struct device **device_ptr);

#endif
