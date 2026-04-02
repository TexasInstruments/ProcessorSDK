/*
 * DM Stub Firmware
 *
 * Minimal io library with readl and writel
 *
 * Copyright (C) 2021-2024, Texas Instruments Incorporated
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

#include <types/errno.h>
#include <types/short_types.h>
#include <types/address_types.h>

/**
 * \brief provide a simplified read u32 operation
 * \param a address to read from
 *
 * \return u32 value at the address pointed by a
 */
static inline u32 readl(local_phys_addr_t a)
{
	return *(volatile u32 *) (a);
}

/**
 * \brief provide a simplified write u32 operation
 * \param v value
 * \param a address to write to
 */
static inline void writel(u32 v, local_phys_addr_t a)
{
	*(volatile u32 *) (a) = v;
}

#ifdef CONFIG_LPM_32_BIT_DDR
static inline void SOC_write32(u32 a, u32 v)
{
	writel(v, a);
}

static inline u32 SOC_read32(u32 a)
{
	return readl(a);
}

/**
 * \brief Write to a specific field in an MMR.
 * \param mmr_address MMR address
 * \param field_value value to be written
 * \param width width of the field
 * \param leftshift the number of bit fields the value has to be left shifted
 */
static void Write_MMR_Field(uint32_t mmr_address, uint32_t field_value, uint32_t width, uint32_t leftshift)
{
	uint32_t *p_mmr;
	uint32_t mask;

	p_mmr = (uint32_t *) mmr_address;                                       /* Grab the MMR value */
	mask = (((uint32_t) 1U << width) - ((uint32_t) 1U << leftshift));       /* Build a mask of 1s for the field. */
	mask = ~(mask);                                                         /* Invert the mask so that the field will be zero'd out with the AND operation. */
	*p_mmr &= mask;                                                         /* Zero out the field in the register. */
	*p_mmr |= (field_value << leftshift);                                   /* Assign the value to that specific field. */
}
#endif
