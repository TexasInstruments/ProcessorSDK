/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2014-2023, Texas Instruments Incorporated
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

#include <device_prepare.h>
#include <device.h>
#include <lib/trace.h>
#include <types/errno.h>
#include <soc/host_idx_mapping.h>

static s32 device_prepare(u8 host_id, u32 id, u8 *host_idx, struct device **dev, sbool exclusive)
{
	s32 ret = SUCCESS;
	struct device *local_device = NULL;
	u8 local_host_idx = HOST_IDX_NONE;

	local_device = device_api_lookup(id);
	if (!local_device) {
		pm_trace(TRACE_PM_ACTION_BAD_DEVICE, id);
		ret = -EINVAL;
	}

	if (ret == SUCCESS) {
		if (local_device->initialized == 0U) {
			ret = -EINVAL;
		}
	}

	if (ret == SUCCESS) {
		local_host_idx = host_idx_lookup(host_id);
		if (local_host_idx == HOST_IDX_NONE) {
			ret = -EINVAL;
		}
	}

	if ((ret == SUCCESS) && exclusive) {
		if ((local_device->exclusive != 0U) && (local_device->exclusive != local_host_idx)) {
			pm_trace(TRACE_PM_ACTION_EXCLUSIVE_DEVICE,
				 (((u32) id << TRACE_PM_VAL_EXCLUSIVE_DEVICE_ID_SHIFT) &
				  TRACE_PM_VAL_EXCLUSIVE_DEVICE_ID_MASK) |
				 (((u32) host_id << TRACE_PM_VAL_EXCLUSIVE_HOST_ID_SHIFT) &
				  TRACE_PM_VAL_EXCLUSIVE_HOST_ID_MASK));
			ret = -EINVAL;
		}
	}

	if (dev != NULL) {
		*dev = local_device;
	}
	if (host_idx != NULL) {
		*host_idx = local_host_idx;
	}

	return ret;
}

s32 device_prepare_exclusive(u8 host_id, u32 id, u8 *host_idx, struct device **device_ptr)
{
	s32 ret;

	/* Ensure devices are fully initialized to allow modification */
	ret = devices_init_rw();

	if (ret == SUCCESS) {
		ret = device_prepare(host_id, id, host_idx, device_ptr, STRUE);
	}
	return ret;
}

s32 device_prepare_nonexclusive(u8 host_id, u32 id, u8 *host_idx, struct device **device_ptr)
{
	return device_prepare(host_id, id, host_idx, device_ptr, SFALSE);
}
