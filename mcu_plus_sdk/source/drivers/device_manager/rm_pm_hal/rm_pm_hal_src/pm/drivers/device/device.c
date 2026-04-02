/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2015-2024, Texas Instruments Incorporated
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

#include <stddef.h>
#include <types/errno.h>
#include <types/short_types.h>
#include <lib/trace.h>
#include <device.h>
#include <device_pm.h>
#include <device_clk.h>
#include <clk.h>
#include <hosts.h>
#ifdef CONFIG_LPM_DM
#include <psc.h>
#endif

/** The bitmask of currently enabled devgroups. */
static u32 pm_devgroups_enabled;

/** True if devices have completed deferred init. */
static sbool devices_rw;

void pm_devgroup_set_enabled(devgrp_t groups)
{
	pm_devgroups_enabled |= (u32) groups;
}

sbool pm_devgroup_is_enabled(devgrp_t groups)
{
	return (pm_devgroups_enabled & groups) != 0U;
}

/**
 * \brief Initialize a device
 *
 * This performs the necessary device initialization step, including syncing
 * the flags on the device with the real hardware state and calling the clock
 * init function for each clock.
 *
 * \param device
 * The device to init.
 *
 * \return
 * 0 on success, <0 on failure.
 */
static s32 device_init(struct device *dev)
{
	const struct dev_data *data = get_dev_data(dev);
	s32 ret = SUCCESS;
	u16 i;

	if ((data != NULL) && ((data->flags & DEVD_FLAG_DRV_DATA) != 0U) &&
	    ((data->flags & DEVD_FLAG_DO_INIT) != 0U)) {
		const struct drv *drvp = to_drv_data(data)->drv;
		if ((drvp != NULL) && (drvp->pre_init != NULL)) {
			ret = drvp->pre_init(dev);
		}
	}

	if (!devices_rw) {
		/* Defer remainder of init */
		data = NULL;
	}

	if ((data != NULL) && (ret == SUCCESS)) {
		ret = soc_device_init(dev);
	}

	/* Calling these multiple times for a deferred device has no effect */
	if ((data != NULL) && (ret == SUCCESS)) {
		for (i = 0U; i < data->n_clocks; i++) {
			device_clk_init(dev, i);
		}

		/* Calling these multiple times for a deferred device has no effect */
		if (device_get_state(dev) != 0U) {
			device_set_state(dev, DEV_POWER_ON_ENABLED_HOST_IDX, STRUE);
			device_set_retention(dev, STRUE);
		}

		if (((data->flags & DEVD_FLAG_DRV_DATA) != 0U) &&
		    ((data->flags & DEVD_FLAG_DO_INIT) != 0U)) {
			const struct drv *drvp = to_drv_data(data)->drv;
			if ((drvp != NULL) && (drvp->post_init != NULL)) {
				ret = drvp->post_init(dev);
			}
		}
	}

	return ret;
}

s32 devices_init(void)
{
	sbool done;
	sbool progress;
	sbool contents;
	sbool errors;
	dev_idx_t idx;
	s32 ret = SUCCESS;

	errors = SFALSE;
	contents = SFALSE;
	do {
		struct device *dev;

		done = STRUE;
		progress = SFALSE;
		dev = soc_devices;

		for (idx = 0U; idx < soc_device_count; idx++) {
			devgrp_t devgrp;
			dev = &soc_devices[idx];
			if (dev->initialized != 0U) {
				continue;
			}

			if (soc_device_data_arr[idx] == NULL) {
				continue;
			}

			/* Translate compressed internal representation to bitfield */
			if (soc_device_data_arr[idx]->pm_devgrp == PM_DEVGRP_DMSC) {
				devgrp = DEVGRP_DMSC;
			} else {
				devgrp = (devgrp_t) BIT(soc_device_data_arr[idx]->pm_devgrp - 1U);
			}

			if (!pm_devgroup_is_enabled(devgrp)) {
				continue;
			}

			contents = STRUE;

			ret = device_init(dev);
			if (ret == -EDEFER) {
				done = SFALSE;
			} else {
				progress = STRUE;
				dev->initialized = 1U;
				if (ret < 0) {
					pm_trace(TRACE_PM_ACTION_FAIL | TRACE_PM_ACTION_DEV_INIT,
						 ((((u32) (-ret)) & TRACE_PM_VAL_DEV_INIT_ERR_MASK) << TRACE_PM_VAL_DEV_INIT_ERR_SHIFT) |
						 ((((u32) idx) & TRACE_PM_VAL_DEV_INIT_DEVICE_ID_MASK) << TRACE_PM_VAL_DEV_INIT_DEVICE_ID_SHIFT));
					errors = STRUE;
				}
			}
		}
	} while (!done && progress);

	if (devices_rw) {
		/* Only necessary after deferred initialization */
		clk_drop_pwr_up_en();
	}

	if (progress) {
		if (devices_rw) {
			/* Only necessary after deferred initialization */
			soc_device_init_complete();
		}

		if (errors == SFALSE) {
			pm_trace(TRACE_PM_ACTION_DEV_INIT, 0x0U);
		}

		ret = SUCCESS;
	} else if (contents) {
		/* We processed at least one device but didn't make progress */
		ret = -EDEFER;
	} else {
		/* We didn't process any devices */
		ret = SUCCESS;
	}

	return ret;
}

s32 devices_init_rw(void)
{
	s32 ret = SUCCESS;

	if (!devices_rw) {
		u32 i;

		/*
		 * Force reinitialization of all devices to get defered
		 * initialization.
		 */
		for (i = 0U; i < soc_device_count; i++) {
			struct device *dev = &soc_devices[i];
			dev->initialized = 0U;
		}

		devices_rw = STRUE;

		/* Perform defered initialization */
		ret = devices_init();
	}

	return ret;
}

extern void device_disable(struct device *dev, sbool domain_reset);
s32 devices_deinit(u8 pm_devgrp)
{
	s32 ret = SUCCESS;
	u32 i;
	sbool flag_enabled;

	for (i = 0U; i < soc_device_count; i++) {
		if (soc_device_data_arr[i] == NULL) {
			continue;
		}
		if (soc_device_data_arr[i]->pm_devgrp == pm_devgrp) {
			struct device *dev = &soc_devices[i];
			const struct dev_data *data = get_dev_data(dev);

			flag_enabled = (dev->flags & DEV_FLAG_ENABLED_MASK) != 0UL;

			dev->flags &= ~DEV_FLAG_RETENTION;

			if ((dev->initialized != 0U) && flag_enabled) {
				device_disable(dev, STRUE);
				dev->initialized = 0;
				dev->flags &= (u32) ~DEV_FLAG_ENABLED_MASK;
			}

			if ((data != NULL) && ((data->flags & DEVD_FLAG_DRV_DATA) != 0U) &&
			    ((data->flags & DEVD_FLAG_DO_INIT) != 0U)) {
				const struct drv *drvp = to_drv_data(data)->drv;
				if ((drvp != NULL) && (drvp->uninit != NULL)) {
					drvp->uninit(dev);
				}
			}
		}
	}

	return ret;
}

extern void device_clear_flags(struct device *dev);
s32 devices_deinit_flags(void)
{
	s32 ret = SUCCESS;
	u32 i;
	struct device *dev;

	for (i = 0U; i < soc_device_count; i++) {
		if (soc_device_data_arr[i] == NULL) {
			continue;
		}
		dev = &soc_devices[i];
		dev->exclusive = 0;
		/* Deinitialize flags only for devices that have been set by a host */
		if ((dev->flags != 0U) && (dev->initialized != 0U)) {
			dev->flags = 0U;
			device_clear_flags(dev);
			dev->initialized = 0;
		}
	}
	return ret;
}

void devices_drop_power_up_ref(void)
{
	dev_idx_t idx;

	for (idx = 0U; idx < soc_device_count; idx++) {
		struct device *dev = soc_devices + idx;
		if (dev->initialized != 0U) {
			device_set_state(dev, DEV_POWER_ON_ENABLED_HOST_IDX, SFALSE);
		}
	}
}

#ifdef CONFIG_LPM_DM
s32 device_id_lookup_devgroup_and_lpsc(u8 id, u16 *devgrp, u8 *lpsc)
{
	s32 ret = SUCCESS;
	struct device *dev = NULL;
	const struct dev_data *data = NULL;

	/* Get the device array from device ID */
	dev = device_lookup(id);

	/* Get the dev_data struct associated with a device */
	data = get_dev_data(dev);
	*devgrp = data->pm_devgrp;

	/* Update the LPSC ID only if Device has a LPSC associated with it */
	if (data->soc.psc_idx != PSC_DEV_NONE) {
		*lpsc = data->soc.mod;
	} else {
		ret = -EFAIL;
	}

	return ret;
}
#endif
