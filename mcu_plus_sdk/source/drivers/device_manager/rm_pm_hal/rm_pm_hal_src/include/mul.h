/*
 * System Firmware Multiplication Utilities
 *
 * Copyright (C) 2019-2020, Texas Instruments Incorporated
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
#ifndef MUL_H
#define MUL_H

#include <types/sbool.h>
#include <types/short_types.h>

/*
 * \brief Multiply and return overflow status.
 *
 * Multiply two 32-bit numbers and return true if the value did not fit in
 * the 32-bit result.
 *
 * Note that ARM does not provide a flag on multiplication overflow. We
 * instead use umull to produce a 64-bit result from two 32-bit operands.
 * umull just takes a couple extra cycles vs a 32-bit umul. We create
 * this as an inline to allow the compiler to optimize it properly.
 *
 * \param a
 * First operand.
 *
 * \param b
 * Second operand.
 *
 * \param c
 * Pointer to location to store result.
 *
 * \return
 * True if overflow occurred, false otherwise
 */
static inline sbool mul32_check_overflow(u32 a, u32 b, u32 *ret)
{
	u64 d = (u64) a * b;

	*ret = (u32) d;
	return d > 0xffffffffULL;
}

#endif
