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

#ifndef RESOURCE_H
#define RESOURCE_H

#include <types/short_types.h>
#include <lib/ioremap.h>
#include <pm_types.h>
#include <soc/device.h>
#include <device_clk.h>

/*
 * A compact method of storing device resources. These are meant to allow
 * SoC independent interaction with devices. For instance, for device foo,
 * clk 0 is always the functional clock and clk 1 is always the interface
 * clock, even though their device/clock framework IDs may differ. Only
 * resources utilized by drivers within the System Firmware are included.
 *
 * Not all devices will have the full set of resources supported by a driver,
 * if the missing resources are at the end of the resource list, the
 * resoure table can just define fewer resources. If the resource is in
 * the middle of the resource list, the resource table should include
 * RESOURCE_xxx_NONE for the missing resources.
 */

#define RESOURCE_IRQ    0x00U
#define RESOURCE_CLK    0x40U
#define RESOURCE_MEM    0x80U
#define RESOURCE_RST    0xc0U

/*
 * Each resource set includes a set of resources including a resource
 * header followed by count resources. The last resource header has
 * a it's last flag set. Resource types can be defined in any order.
 */
#define RESOURCE_TYPE_MASK      0xc0U
#define RESOURCE_LAST           0x20U
#define RESOURCE_COUNT_MASK     0x1fU

#define RESOURCE_CLK_NONE       DEV_CLK_ID_NONE

struct resource_clk {
	dev_clk_idx_t clk_id;
} __attribute__((__packed__));

#define RESOURCE_MEM_NONE       0xffffffffU

struct resource_mem {
	u32 addr;
} __attribute__((__packed__));

#define RESOURCE_RST_NONE       255U

struct resource_rst {
	/**
	 * Bitfield index of device reset for this reset resource.
	 * Set to 255 if this reset is not present.
	 */
	u8 bit;
} __attribute__((__packed__));

#define RDAT_HDR(type, count, last) ((u8) (type) | (u8) (count) |	\
				     (u8) ((last) ? RESOURCE_LAST : 0U))
#define RDAT_CLK(clk)           (clk)

#define RDAT_MEM(mem)           (u8) ((u32) (mem) & 0xffU),	\
	(u8) (((u32) (mem) >> 8) & 0xffU),		\
	(u8) (((u32) (mem) >> 16) & 0xffU),		\
	(u8) ((u32) (mem) >> 24)

#define RDAT_RST(rst)           (rst)

/*
 * The stucts these return are not aligned and packed. They return NULL if
 * no such resource is present, never resouces with the RESOURCE_xxx_NONE
 * value.
 */
struct device;

/**
 * \brief Return a clock device resource.
 *
 * \param dev
 * Which device the resource table is attached to.
 *
 * \param idx
 * There can be multiple resources of each type. This indicates which one
 * to return.
 *
 * \return
 * The desired clock resource, or NULL if the device has no such resource.
 * Device resources are packed data and the alignment of the returned data
 * will not be natural.
 */
const struct resource_clk *device_resource_clk(struct device *dev, u8 idx);

/**
 * \brief Return a memory device resource.
 *
 * \param dev
 * Which device the resource table is attached to.
 *
 * \param idx
 * There can be multiple resources of each type. This indicates which one
 * to return.
 *
 * \return
 * The desired memory resource, or NULL if the device has no such resource.
 * Device resources are packed data and the alignment of the returned data
 * will not be natural.
 */
const struct resource_mem *device_resource_mem(struct device *dev, u8 idx);

/**
 * \brief Return a reset device resource.
 *
 * \param dev
 * Which device the resource table is attached to.
 *
 * \param idx
 * There can be multiple resources of each type. This indicates which one
 * to return.
 *
 * \return
 * The desired reset resource, or NULL if the device has no such resource.
 * Device resources are packed data and the alignment of the returned data
 * will not be natural.
 */
const struct resource_rst *device_resource_rst(struct device *dev, u8 idx);

static inline u32 mem_readl(const struct resource_mem *mem, u32 off)
{
	return readl(mem->addr + off);
}

static inline void mem_writel(const struct resource_mem *mem, u32 a, u32 off)
{
	writel(a, mem->addr + off);
}

static inline u16 mem_readw(const struct resource_mem *mem, u32 off)
{
	return readw(mem->addr + off);
}

static inline void mem_writew(const struct resource_mem *mem, u16 a, u32 off)
{
	writew(a, mem->addr + off);
}

static inline u8 mem_readb(const struct resource_mem *mem, u32 off)
{
	return readb(mem->addr + off);
}

static inline void mem_writeb(const struct resource_mem *mem, u8 a, u32 off)
{
	writeb(a, mem->addr + off);
}

#endif
