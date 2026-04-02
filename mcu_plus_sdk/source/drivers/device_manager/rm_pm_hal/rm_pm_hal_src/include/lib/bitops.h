/*
 * System Firmware
 *
 * Functions for bit and bitfield operations
 *
 * Copyright (C) 2016-2023, Texas Instruments Incorporated
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

#ifndef BITOPS_H
#define BITOPS_H

#include <types/short_types.h>

/** BIT macro for easily calculating a bit at offset (n) */
#define BIT(n)  (((u32) 1U) << (n))

#define ORDER_1B        0U
#define ORDER_2B        1U
#define ORDER_4B        2U
#define ORDER_8B        3U
#define ORDER_16B       4U
#define ORDER_32B       5U
#define ORDER_64B       6U
#define ORDER_128B      7U
#define ORDER_256B      8U
#define ORDER_512B      9U
#define ORDER_1KB       10U
#define ORDER_2KB       11U
#define ORDER_4KB       12U
#define ORDER_8KB       13U
#define ORDER_16KB      14U
#define ORDER_32KB      15U
#define ORDER_64KB      16U
#define ORDER_128KB     17U
#define ORDER_256KB     18U
#define ORDER_512KB     19U
#define ORDER_1MB       20U
#define ORDER_2MB       21U
#define ORDER_4MB       22U
#define ORDER_8MB       23U
#define ORDER_16MB      24U
#define ORDER_32MB      25U
#define ORDER_64MB      26U
#define ORDER_128MB     27U
#define ORDER_256MB     28U
#define ORDER_512MB     29U
#define ORDER_1GB       30U
#define ORDER_2GB       31U

static inline u32 get_field(u32 val, u32 end, u32 start);
static inline u32 set_field(u32 tgt, u32 end, u32 start, u32 val);
static inline u32 get_mask(u32 end, u32 start);

/**
 * \brief Generate a mask for the give bit field [end:start]
 *
 * All the bits between the two specified bit indices including
 * the specified indices are set to 1. Rest of the bits are set
 * to zero.
 *
 * \param end upper bit index of the mask
 * \param start lower bit index of the mask
 *
 * \return generated mask
 */
static inline u32 get_mask(u32 end, u32 start)
{
	return (u32) ((1U << (end - start + 1U)) - 1U) << start;
}

/**
 * \brief extract the specified bit field from a 32 bit unsigned integer.
 *
 * \param val input from which the bitfield is extracted
 * \param end upper bit index of the bitfield
 * \param start lower bit index of the bitfield
 *
 * \return extracted bit field, right shifted by start bits.
 */
static inline u32 get_field(u32 val, u32 end, u32 start)
{
	u32 mask = get_mask(end, start);

	return (val & mask) >> start;
}

/**
 * \brief set the specified bit field from a 32 bit unsigned integer
 * to provided value
 *
 * \param tgt input in which the bitfield is to be set
 * \param end upper bit index of the bitfield
 * \param start lower bit index of the bitfield
 * \param val  value to which the bit field is to be set.
 *
 * \return input value with bitfield updated as specified.
 */
static inline u32 set_field(u32 tgt, u32 end, u32 start, u32 val)
{
	u32 ret = 0U;
	u32 mask = get_mask(end, start);

	ret = (tgt & (~mask));

	ret = (ret | ((val << start) & mask));

	return ret;
}

#endif
