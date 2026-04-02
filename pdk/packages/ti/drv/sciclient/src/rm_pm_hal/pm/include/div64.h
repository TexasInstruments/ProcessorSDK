/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2015-2020, Texas Instruments Incorporated
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
#ifndef DIV64_H
#define DIV64_H

#include <types/short_types.h>
#include <compiler.h>

/**
 * 64-bit division/modulo.
 *
 * Perform a 64-bit division/modulo operation. This divides a 64-bit numerator
 * by a 32-bit denominator and provides the result and remainder. Note that
 * passing the value 0 for denominator will result in an exception.
 *
 * @param rem The numerator should be passed in this memory location. The
 * remainder is returned in this memory location.
 *
 * @param b The denominator.
 *
 * @return The result of the division.
 */
u64 pm_div64(u64 *rem, u32 b);

/**
 * 64-bit division.
 *
 * Perform a 64-bit division operation. This divides a 64-bit numerator
 * by a 32-bit denominator and provides the result. Note that passing
 * the value 0 for denominator will result in an exception.
 *
 * @param a The numerator.
 *
 * @param b The denominator.
 *
 * @return The result of the division.
 */
static inline u64 div64(u64 a, u32 b)
{
	return pm_div64(&a, b);
}

/**
 * 64-bit modulo.
 *
 * Perform a 64-bit division operation. This divides a 64-bit numerator
 * by a 32-bit denominator and provides the remainder. Note that passing
 * the value 0 for denominator will result in an exception.
 *
 * @param a The numerator.
 *
 * @param b The denominator.
 *
 * @return The remainder of the division.
 */
static inline u64 mod64(u64 a, u32 b)
{
	u64 temp UNUSED;

	temp = pm_div64(&a, b);
	return a;
}


#endif
