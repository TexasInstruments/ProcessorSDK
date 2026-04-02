/*
 * Ring Accelerator
 *
 * Ring accelerator driver instance definition
 *
 * Copyright (C) 2018-2023, Texas Instruments Incorporated
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
#ifndef RA_INST_H
#define RA_INST_H

#include <types/short_types.h>
#include <types/sbool.h>
#include <types/devgrps.h>
#include <soc/rm_mmr.h>
#include <rm_irq.h>
#include <ra_cfg.h>

/**
 * Ring type for NAVSS rings (those that support message/exposed/QM/cred)
 */
#define RA_STANDARD_RING  (0U)
/**
 * Ring type for DMSS dual rings
 */
#define RA_DMSS_RING      (1U)
/**
 * Total number of ring types
 */
#define RA_NUM_RING_TYPES (2U)

/**
 * \brief
 * Ring type information
 *
 * \param start
 * Ring type start index
 *
 * \param end
 * Ring type end index
 *
 * \param type
 * Ring type
 *
 * \param utype
 * Ring type resource assignment type
 */
struct ra_ring_type {
	const u16	start;
	const u16	end;
	const u8	type;
	const u16	utype;
};

/**
 * \brief
 * Describes the rings used by ROM and if they've been cleared back to
 * hardware reset values
 *
 * \param ring
 * ring used by ROM
 *
 * \param cleared
 * Boolean storing whether or not the ring has been cleared
 */
struct ra_used_mapping {
	const u16	ring;
	sbool		cleared;
};

/**
 * \brief
 * Ring accelerator instance containing all data required to manage a
 * Navigator Subsystem ring accelerator
 *
 * \param id
 * Device ID of NavSS RA
 *
 * \param root_id
 * Device ID of NavSS in which RA is located
 *
 * \param initialized
 * Instance initialization flag
 *
 * \param devgrp
 * Device group to which instance belongs
 *
 * \param gcfg
 * RA subsystem's global control and status MMR region
 *
 * \param cfg
 * RA subsystem's control and status MMR region
 *
 * \param rt
 * RA subsystem's real-time MMR region
 *
 * \param mon
 * RA subsystem's monitor MMR region
 *
 * \param fifos
 * RA subsystem's fifos MMR region
 *
 * \param n_ring
 * Total number of rings in the subsystem
 *
 * \param ring_oes_offset
 * Event index offset for ring OES registers
 *
 * \param ring_types
 * Pointer to array of ring types
 *
 * \param n_ring_type
 * Number of ring types
 *
 * \param err_evt_index
 * Event index mapped to RA instance's global error event
 *
 * \param err_evt_utype
 * Resource assignment type mapped to RA instance's global error event
 *
 * \param oes_handler
 * IRQ event source handler
 *
 * \param virtid_utype
 * Resource assignment type mapped to ring virtids
 *
 * \param n_ring_mon
 * Total number of ring monitors in the subsystem
 *
 * \param ring_mon_utype
 * Resource assignment type mapped to ring monitors
 *
 * \param ring_mon_oes_offset
 * Event index offset for ring monitor OES registers
 *
 * \param rom_usage
 * Rings used by ROM during boot that need their OES registers reset to hardware
 * reset values
 *
 * \param n_rom_usage
 * Number of entries in the rom_usage array
 */
struct ra_instance {
	const u16				id;
	const u16				root_id;
	sbool					initialized;
	const devgrp_t				devgrp;
	const struct rm_mmr *const		gcfg;
	const struct rm_mmr *const		cfg;
	const struct rm_mmr *const		rt;
	const struct rm_mmr *const		mon;
	const struct rm_mmr *const		fifos;
	const u16				n_gp_rings_pre_sp_rings;
	const u16				n_gp_rings_post_sp_rings;
	const u16				gp_ring_oes_offset_pre_sp;
	const u16				gp_ring_oes_offset_post_sp;
	const struct ra_ring_type *const	ring_types;
	const u8				n_ring_type;
	const struct ra_ring_type *const	ring_mon_types;
	const u8				n_ring_mon_type;
	const u16				err_evt_index;
	const u16				err_evt_utype;
	struct rm_irq_oes_src			oes_handler;
	const u16				virtid_utype;
	const u8				n_ring_mon;
	const u16				ring_mon_utype;
	const u16				ring_mon_oes_offset;
	struct ra_used_mapping *const		rom_usage;
	const u8				n_rom_usage;
};

/**
 * Extern for RA subsystem instances array
 */
extern struct ra_instance ra_inst[RA_INST_RANGE];

/**
 * Extern for number of RA subsystem instances
 */
extern const u32 ra_inst_count;


#endif /* RA_INST_H */
