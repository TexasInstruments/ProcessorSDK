/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2014-2020, Texas Instruments Incorporated
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
#ifndef MLB_H
#define MLB_H

#include <lib/bitops.h>

#define MLB_REV                         0x000
#define MLB_PWR                         0x004
#define MLB_PERF                        0x100

#define MLB_PWR_MSTANDBY                BIT(0)

#define MLB_PERF_WMP                    BIT(16)
#define MLB_PERF_ASYNC_PRI(N)           ((N) << 12)
#define MLB_PERF_ASYNC_PRI_MASK         (7 << 12)
#define MLB_PERF_SYNC_PRI(N)            ((N) << 8)
#define MLB_PERF_SYNC_PRI_MASK          (7 << 8)
#define MLB_PERF_ASYNC_FLAG_LOW         0
#define MLB_PERF_ASYNC_FLAG_MED         (1 << 4)
#define MLB_PERF_ASYNC_FLAG_HIGH        (3 << 4)
#define MLB_PERF_ASYNC_FLAG_MASK        (3 << 4)
#define MLB_PERF_SYNC_FLAG_LOW          0
#define MLB_PERF_SYNC_FLAG_MED          1
#define MLB_PERF_SYNC_FLAG_HIGH         3
#define MLB_PERF_SYNC_FLAG_MASK         3

struct drv;

extern const struct drv mlb_drv;

#endif
