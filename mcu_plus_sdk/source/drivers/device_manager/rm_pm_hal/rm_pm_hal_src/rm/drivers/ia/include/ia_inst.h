/*
 * Interrupt Aggregator
 *
 * Interrupt aggregator driver instance definition
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
#ifndef IA_INST_H
#define IA_INST_H

#include <types/short_types.h>
#include <types/sbool.h>
#include <types/devgrps.h>
#include <soc/rm_mmr.h>
#include <ia_cfg.h>
#include <rm_irq.h>

/**
 * \brief
 * Describes the IA events used by ROM and if they've been cleared back to
 * hardware reset values
 *
 * \param event
 * IA event used by ROM
 *
 * \param cleared
 * Boolean storing whether or not the event and the IA VINT status bit its
 * been mapped to has been cleared
 */
struct ia_used_mapping {
	u16	event;
	sbool	cleared;
};

/**
 * \brief
 * Resource type and range information
 *
 * \param start
 * Resource start index
 *
 * \param end
 * Resource end index
 *
 * \param utype
 * Resource resource assignment type
 */
struct ia_resource_type {
	const u16	start;
	const u16	end;
	const u16	utype;
};

/**
 * \brief
 * Interrupt aggregator instance containing all data required to manage a
 * Navigator Subsystem interrupt aggregator
 *
 * \param id
 * Device ID of NavSS IA
 *
 * \param initialized
 * Instance initialization flag
 *
 * \param devgrp
 * Device group to which instance belongs
 *
 * \param imap
 * IA subsystem's event interrupt mapping MMR region
 *
 * \param intr
 * IA subsystem's virtual interrupt real time MMR region
 *
 * \param gcntrti
 * IA subsystem's real-time global event RTI MMR region
 *
 * \param unmap
 * IA subsystem's unmapped event MMR region
 *
 * \param sevt_offset
 * SEVI global event offset for IA
 *
 * \param n_sevt
 * Number of IA SEVIs
 *
 * \param vint_utype
 * Resource assignment type mapped to Ia instance's virtual interrupts
 *
 * \param n_vint
 * Number of IA virtual interrupts
 *
 * \param vint_usage_count
 * Number of VINT status bits used for event mapping.  The usage count for
 * each VINT cannot exceed the number of maximum supported event mappings.
 *
 * \param v0_b0_evt
 * A standalone element is needed to track usage of VINT 0 bit 0.  The INTMAP
 * registers are read to determine VINT bit usage.  The default state for
 * INTMAP registers is 0x0000 which translates to the INTMAP using VINT 0 bit
 * 0.  This field allows the IA driver to determine if VINT 0 bit 0 is truly
 * mapped to an IA event.
 *
 * \param rom_usage
 * IA event to VINT mappings used by ROM during boot that need to be reset
 * to hardware reset values
 *
 * \param n_rom_usage
 * Number of entries in the rom_usage array
 *
 * \param unmapped_events
 * Pointer to array of unmapped event types
 *
 * \param n_unmapped_events
 * Number of unmapped event types
 *
 * \param oes_handler
 * Output event steering register handler
 */
struct ia_instance {
	const u16				id;
	sbool					initialized;
	const devgrp_t				devgrp;
	const struct rm_mmr *const		imap;
	const struct rm_mmr *const		intr;
	const struct rm_mmr *const		gcntrti;
	const struct rm_mmr *const		unmap;
	const u16				sevt_offset;
	const u16				n_sevt;
	const u16				vint_utype;
	const u16				n_vint;
	u8 *const				vint_usage_count;
	u16					v0_b0_evt;
	struct ia_used_mapping *const		rom_usage;
	const u8				n_rom_usage;
	struct ia_used_mapping *const		rom_usage_unmapped_events;
	const u8				n_rom_usage_unmapped_events;
	const struct ia_resource_type *const	unmapped_events;
	const u8				n_unmapped_events;
	struct rm_irq_oes_src			oes_handler;
};

/**
 * Extern for IA subsystem instances array
 */
extern struct ia_instance ia_inst[IA_INST_COUNT];


#endif /* IA_INST_H */
