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


/**
 * \file address_types.h
 *
 * \brief Include file to indicate various valid address types for System
 *	  firmware. These types are expected to be used to refer to addressing
 *	  schemes expected to access various entities.
 */

#ifndef ADDRESS_TYPES_H
#define ADDRESS_TYPES_H

#include <types/short_types.h>

/**
 * \brief Physical address representing SoC Bus level physical address.
 *
 * This may be used for register programming (example buffer addresses) OR be
 * used to map back to addresses visible to System Control Processor (such as
 * DMSC). See \ref mapped_addr_t for a remapped address
 */
typedef u64 soc_phys_addr_t;

/** Get the lower u32 from a \ref soc_phys_addr_t address */
static inline u32 soc_phys_low_u32(soc_phys_addr_t w)
{
	return (((u32) (w)) >> 0) & 0xFFFFFFFFU;
}

/** Get the Higher u32 from a \ref soc_phys_addr_t address */
static inline u32 soc_phys_high_u32(soc_phys_addr_t w)
{
	return ((u32) ((w) >> 32)) & 0xFFFFFFFFU;
}

/** Create soc_phys_addr_t address from 32-bit lo and hi portions */
static inline soc_phys_addr_t soc_phys_create(u32 l, u32 h)
{
	return (soc_phys_addr_t) (((u64) ((u64) (h) << 32)) | (u64) l);
}

/**
 * \brief Represents the local addressing type for System Control processor.
 *
 * This is expected to be usable without any remapping and indicates that this
 * address map belongs to local address map of System Control Processor (such
 * as DMSC).
 */
typedef u32 local_phys_addr_t;

/**
 * \brief Represents the remapped address for System Control processor.
 *
 * This address is used to represent a remapped address which is visible over
 * a remapping scheme implemented by System control processor. Typically this
 * may be implemented via RAT - See \ref soc_phys_addr_t for further details.
 */
typedef u32 mapped_addr_t;

#endif
