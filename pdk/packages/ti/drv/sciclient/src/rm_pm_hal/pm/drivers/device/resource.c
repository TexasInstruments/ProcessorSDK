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

#include <resource.h>
#include <device.h>
#include <stddef.h>
#include <limits.h>

#define RESOURCE_MAX2(a, b) (((a) > (b)) ? (a) : (b))
#define RESOURCE_MAX3(A, B, C) RESOURCE_MAX2(A, (RESOURCE_MAX2(B, C)))

/**
 * \brief Lookup a resource table entry.
 *
 * This function walks the resource table, first finding the array of correct
 * resource type, and then returning the resource at the appropriate index.
 *
 * \param dev
 * The device to which the resource table belongs.
 *
 * \param type
 * The type of resource table entry (clock, mem, reset).
 *
 * \param idx
 * Returns the nth entry of the desired type, where n is specified by idx.
 *
 * \return
 * The desired resource entry, or NULL if no such entry exists.
 */
static const void *resource_get(struct device *dev, u8 type, u8 idx)
{
	/*
	 * The downshift here is just because the macro that defines the type
	 * starts at bit 6.
	 */
	static const u8 sizes[RESOURCE_MAX3((RESOURCE_CLK >> 6U),
					    (RESOURCE_MEM >> 6U),
					    (RESOURCE_RST >> 6U)) + 1U] = {
		[RESOURCE_IRQ >> 6U] = 0,
		[RESOURCE_CLK >> 6U] = (u8) sizeof(struct resource_clk),
		[RESOURCE_MEM >> 6U] = (u8) sizeof(struct resource_mem),
		[RESOURCE_RST >> 6U] = (u8) sizeof(struct resource_rst),
	};
	const struct dev_data *ddata = get_dev_data(dev);
	const u8 *data;
	const u8 *r;
	u8 hdr;

	BUILD_ASSERT((((sizeof(struct resource_clk)) <= ((size_t) UCHAR_MAX)) &&
		      ((sizeof(struct resource_mem)) <= ((size_t) UCHAR_MAX))) &&
		     ((sizeof(struct resource_rst)) <= ((size_t) UCHAR_MAX)));
	/*
	 * If the device does not have drv_data, it does not have resources.
	 * Return NULL
	 */
	r = (((u32) (ddata->flags) & DEVD_FLAG_DRV_DATA) == DEVD_FLAG_DRV_DATA) ? to_drv_data(ddata)->r : NULL;

	while (r != NULL) {
		hdr = r[0];
		data = r + 1;
		/* Is this the right resource type? */
		if ((hdr & RESOURCE_TYPE_MASK) == type) {
			/* Is the array long enough? */
			if (idx < (hdr & RESOURCE_COUNT_MASK)) {
				/* Return our resource */
				r = &data[idx * sizes[type >> 6]];
				break;
			} else {
				r = NULL;
			}
		} else if ((hdr & RESOURCE_LAST) != 0U) {
			/* We've reached the end of the table, stop walking */
			r = NULL;
		} else {
			/* Walk to the next entry */
			r = &data[(hdr & RESOURCE_COUNT_MASK) *
				  sizes[hdr >> 6]];
		}
	}

	return r;
}

const struct resource_clk *device_resource_clk(struct device *dev, u8 idx)
{
	const struct resource_clk *ret = (const struct resource_clk *) resource_get(dev, RESOURCE_CLK, idx);

	return (ret && (ret->clk_id != RESOURCE_CLK_NONE)) ? ret : NULL;
}

const struct resource_mem *device_resource_mem(struct device *dev, u8 idx)
{
	const struct resource_mem *ret = (const struct resource_mem *) resource_get(dev, RESOURCE_MEM, idx);

	return (ret && (ret->addr != RESOURCE_MEM_NONE)) ? ret : NULL;
}

const struct resource_rst *device_resource_rst(struct device *dev, u8 idx)
{
	const struct resource_rst *ret = (const struct resource_rst *) resource_get(dev, RESOURCE_RST, idx);

	return (ret && (ret->bit != RESOURCE_RST_NONE)) ? ret : NULL;
}
