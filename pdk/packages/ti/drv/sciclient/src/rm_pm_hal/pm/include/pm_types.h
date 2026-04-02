/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2018-2020, Texas Instruments Incorporated
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

#ifndef PM_TYPES_H
#define PM_TYPES_H

#include <types/short_types.h>

/**
 * This defines the index of a powerdomain within a PSC. The current PSC
 * register layout limits the max number of powerdomains to 32. This value
 * can be compressed to a 5-bit bitfield.
 *
 * This index is internal to the DSMC firmware.
 */
typedef u8 pd_idx_t;

/**
 * This defines the index of a module within a PSC. The current PSC register
 * layout limits the max number of modules to 128. This value can be
 * compressed to a 7-bit bitfield.
 *
 * This index is internal to the DSMC firmware.
 */
typedef u8 lpsc_idx_t;

/**
 * This defines the arbitrary ID of PSC within an SoC. The ID is assigned
 * by the specdata autogen scripts. We are currently limiting the maximum
 * number of PSCs within an SoC to 6 and this field can be compressed to
 * a 3-bit bitfield. The remaining two values have special meanings:
 *
 * 6 PSC_DEV_MULTIPLE - IP is associated with domains in multiple PSCs.
 * 7 PSC_DEV_NONE -     IP is not associated with any PSC.
 *
 * This index is internal to the DSMC firmware.
 */
typedef u8 psc_idx_t;

/**
 * This defines the arbitrary ID of a clock within the SoC. The ID is
 * assigned by the specdata autogen scripts. We only export clock connections
 * to device IP via the ABI. Each clock connection is then associated
 * internally with an SoC clock. We currently have fewer than 1024 clocks on
 * every SoC so this field can be compressed to a 10-bit bitfield
 *
 * This index is internal to the DSMC firmware.
 */
typedef u16 clk_idx_t;

/**
 * This defines a clock connected to a device IP. IDs are assigned sequentially
 * and their name and ID are exported for use via the ABI.
 *
 * This index has a 1-to-1 correspondence with the clk field in the TISCI
 * device clock messages.
 */
typedef u16 dev_clk_idx_t;

#endif
