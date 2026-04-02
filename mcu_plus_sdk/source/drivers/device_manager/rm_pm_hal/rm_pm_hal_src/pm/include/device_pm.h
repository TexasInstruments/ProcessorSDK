/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2015-2023, Texas Instruments Incorporated
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

#ifndef DEVICE_PM_H
#define DEVICE_PM_H

#include <types/short_types.h>
#include <types/sbool.h>
#include <config.h>

struct notifier;

#ifdef CONFIG_PM

/** Register a notifier for when the device enables/disables */
void register_device_notifier(struct notifier *n);

/**
 * \brief Get the current device state (SoC specific impl.)
 *
 * \param dev
 * The device to query.
 *
 * \return
 * True if the device is configured to be enabled, SFALSE if the device is
 * configured to be disabled.
 */
extern u32 soc_device_get_state(struct device *dev);

/**
 * \brief Set the reset isolation flag for a device (SoC specific impl.)
 *
 * \param dev
 * The device to modify.
 *
 * \param enable
 * True to enable reset isolation, SFALSE to disable.
 */
extern void soc_device_set_reset_iso(struct device *dev, sbool enable);

/**
 * \brief Get the reset isolation flag for a device (SoC specific impl.)
 *
 * \param dev
 * The device to query.
 *
 * \return
 * True if reset isolation flag is set, SFALSE otherwise
 */
extern sbool soc_device_get_reset_iso(struct device *dev);

/**
 * \brief Set the state of the device resets (SoC specific impl.)
 *
 * \param dev
 * The device to modify.
 *
 * \param resets
 * Bit-field representing desired reset state.
 */
extern void soc_device_set_resets(struct device *dev, u32 resets);

/**
 * \brief Get the current state of the device resets (SoC specific impl.)
 *
 * \param device
 * The device to query.
 *
 * \return
 * The current bitfield of device resets, as set by device_set_resets.
 */
extern u32 soc_device_get_resets(struct device *dev);

/**
 * \brief Get the current device context loss count (SoC specific impl.)
 *
 * \param device
 * The device to query.
 *
 * \return
 * The current loss count of the device.
 */
extern u32 soc_device_get_context_loss_count(struct device *dev);

/**
 * \brief Enable a device (SoC specific impl.)
 *
 * \param device
 * The device to modify.
 */
extern void soc_device_enable(struct device *dev);

/**
 * \brief Enable a device (SoC specific impl.)
 *
 * \param dev The device to modify.
 */
extern void device_enable(struct device *dev);

/**
 * \brief Disable a device (SoC specific impl.)
 *
 * \param device
 * The device to modify.
 * \param domain_reset
 * True if the device is being disabled due to a domain reset.
 */
extern void soc_device_disable(struct device *dev, sbool domain_reset);

/**
 * \brief Clear a device initialization flags
 *
 * \param dev The device to modify.
 *
 */
extern void soc_device_clear_flags(struct device *dev);

/**
 * \brief Enable retention on a device (SoC specific impl.)
 *
 * \param device
 * The device to modify.
 */
extern void soc_device_ret_enable(struct device *dev);

/**
 * \brief Disable retention on a device (SoC specific impl.)
 *
 * \param device
 * The device to modify.
 */
extern void soc_device_ret_disable(struct device *dev);

/**
 * \brief Perform SoC level initialization of a device.
 *
 * \param
 * The device to init.
 *
 * \return
 * 0 on success, -EDEFER if device is not yet ready to be initialized.
 */
extern s32 soc_device_init(struct device *dev);

/**
 * \brief Notify SoC device impl. that device init is complete.
 *
 * This allows the SoC implementation to drop any extra references it's been
 * holding during init.
 */
extern void soc_device_init_complete(void);

/**
 * \brief Disables a device.
 *
 * Performs the steps necessary to disable a device.
 *
 * \param device
 * The device to modify.
 * \param domain_reset
 * True if the device is being disabled due to a domain reset.
 */
extern void device_disable(struct device *dev, sbool domain_reset);

/**
 * \brief Set the reset isolation flag for a device.
 *
 * The effect of reset isolation is device and SoC specific, but it generally
 * prevents the device from undergoing reset with the rest of the SoC.
 *
 * \param device
 * The device to modify.
 *
 * \param enable
 * True to enable reset isolation, SFALSE to disable.
 */
static inline void device_set_reset_iso(struct device *dev, sbool enable)
{
	soc_device_set_reset_iso(dev, enable);
}

/**
 * \brief Get the reset isolation flag for a device.
 *
 * \param device
 * The device to query.
 *
 * \return
 * True if the reset isolation flag is set, SFALSE otherwise.
 */
static inline sbool device_get_reset_iso(struct device *dev)
{
	return soc_device_get_reset_iso(dev);
}

/**
 * \brief Set the state of the device resets.
 *
 * This sets the state of the device resets via bitfield. The meaning of each
 * bit is device and SoC specific.
 *
 * \param device
 * The device to modify.
 *
 * \param resets
 * Bit-field representing desired reset state.
 */
static inline void device_set_resets(struct device *dev, u32 resets)
{
	soc_device_set_resets(dev, resets);
}

/**
 * \brief Get the current state of the device resets.
 *
 * \param device
 * The device to query.
 *
 * \return
 * The current bitfield of device resets, as set by device_set_resets.
 */
static inline u32 device_get_resets(struct device *dev)
{
	return soc_device_get_resets(dev);
}

/**
 * \brief Get the current device context loss count.
 *
 * The context loss count of the device is tracked by the PMMC. Whenever the
 * device loses context, the context loss counter is incremented. The device
 * can only lose context when disabled. When enabling a device, the HLOS can
 * query the context loss count and compare it with the context loss count
 * read the last time the device was enabled. If the count differs, context
 * has been lost and must be restored.
 *
 * \param device
 * The device to query.
 *
 * \return
 * The current loss count of the device.
 */
static inline u32 device_get_context_loss_count(struct device *dev)
{
	return soc_device_get_context_loss_count(dev);
}

/**
 * \brief Call the suspend callback for a device.
 *
 * This calls the suspend callback for a device if it has one. It properly
 * checks for the DEVD_FLAG_DRV_DATA to ensure the device has extra driver
 * specific data (including a drv pointer) before attempting to access the
 * drv pointer. It also ensures the drv pointer and the suspend callback
 * are non-NULL
 *
 * \param device
 * Which device to call the suspend callback on. This argument is also passed
 * to the callback.
 */
void device_suspend(struct device *dev);

/**
 * \brief Enable/disable a device.
 *
 * This indicates the desired state of a device by the HLOS. If a device is
 * enabled, the PMMC will make sure the device and it's dependencies are
 * ready. If the device is not enabled, the PMMC will opportunistically
 * utilize power management modes of that device and it's dependencies.
 *
 * Enabling a device (if disabled) increments the device's reference count.
 * Disabling a device (if enabled) decrements the devices' reference count.
 *
 * \param device
 * The device to modify.
 *
 * \param host_idx
 * The index of the host making the requset.
 *
 * \param enable
 * True to enable the device, SFALSE to allow the PMMC to power down the device.
 */
void device_set_state(struct device *dev, u8 host_idx, sbool enable);

/**
 * \brief Get the current device state.
 *
 * Returns the current device state as configured by device_set_state.
 *
 * \param device
 * The device to query.
 *
 * \return
 * True if the device is configured to be enabled, SFALSE if the device is
 * configured to be disabled.
 */
static inline u32 device_get_state(struct device *dev)
{
	return soc_device_get_state(dev);
}

/**
 * \brief Enable/disable retention on a device.
 *
 * When a device is in retention, but disabled, the PMMC can still perform
 * power management tasks, but the device must keep it's context. Enabling
 * retention is a way save power, but still be able to bring the device back
 * to full functionality quickly.
 *
 * \param device
 * The device to modify.
 *
 * \param retention
 * True to enable retention, SFALSE to disable it.
 */
void device_set_retention(struct device *dev, sbool retention);

/**
 * \brief Clear a device initialization flags
 *
 * \param dev The device to modify.
 *
 */
void device_clear_flags(struct device *dev);

#else

static inline void device_suspend(struct device *dev)
{
}

static inline u32 device_get_state(struct device *dev)
{
	return 0;
}

static inline void device_set_state(struct device *dev, u8 host_idx, sbool enable)
{
}
static inline void device_set_retention(struct device *dev, sbool ret)
{
}

static s32 soc_device_init(struct device *dev)
{
	return 0;
}

static inline void soc_device_init_complete(void)
{
}

static inline void device_clear_flags(struct device *dev)
{
}
#endif

#endif
