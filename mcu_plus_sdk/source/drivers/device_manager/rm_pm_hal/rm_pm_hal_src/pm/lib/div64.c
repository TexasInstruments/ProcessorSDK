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

#include <div64.h>

u64 pm_div64(u64 *remainder64, u32 b32)
{
	u32 ah32, al32;
	u64 a64;
	u64 b64;
	u64 bit64;
	u64 result64;

	if (*remainder64 == 0ULL) {
		result64 = 0ULL;
	} else {
		if (b32 == 0U) {
			/* Generate exception */
			result64 = 1ULL / (u64) b32;
		} else {
			a64 = *remainder64;

			ah32 = (u32) (a64 >> 32U);
			al32 = (u32) (a64 & 0xffffffffULL);

			result64 = 0ULL;

			/* Check if b * 2^32 fits at least once */
			if (ah32 >= b32) {
				/* Divide out b * 2^32 */
				ah32 /= b32;
				result64 = ((u64) ah32) << 32U;
				a64 -= result64 * b32;
			}

			/* 32 bit division possible */
			if (ah32 == 0U) {
				a64 = (u64) al32 % (u64) b32;
				result64 += (u64) al32 / (u64) b32;
			} else {
				/* Time for long division */
				b64 = ((u64) b32) << 32U;
				bit64 = 1ULL << 32;

				/* Shift divisor up to line up with remaining dividend */
				while (b64 < a64) {
					b64 <<= 1U;
					bit64 <<= 1U;
				}

				/* Long division loop */
				do {
					if (b64 <= a64) {
						a64 -= b64;
						result64 += bit64;
					}
					b64 >>= 1U;
					bit64 >>= 1U;
				} while (bit64 != 0ULL);
			}

			*remainder64 = a64;
		}
	}

	return result64;
}
