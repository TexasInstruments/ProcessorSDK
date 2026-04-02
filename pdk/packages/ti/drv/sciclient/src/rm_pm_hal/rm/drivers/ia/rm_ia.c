/*
 * IRQ Interrupt Aggregator
 *
 * IRQ management interrupt aggregator infrastructure
 *
 * Copyright (C) 2018-2025, Texas Instruments Incorporated
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

#include <types/sbool.h>
#include <types/short_types.h>
#include <types/errno.h>
#include <types/address_types.h>
#include <lib/io.h>
#include <lib/trace.h>

#include <rm_core.h>
#include <rm_ia.h>
#include <tisci/rm/tisci_rm_irq.h>
#include <hosts.h>

#include <ia_inst.h>
#include <ia_init.h>
#include <ia_cfg.h>

#ifdef CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS
#include <security/rm_int_firewall.h>
#include <tifs/src/security/secure_rm/sec_rm.h>
#endif

#define IA_VINT_MAX_BITS             64u

#define IA_VINT_NULL                 0xFFFFu

/*
 * IA Register Macros
 */
#define IA_ENTRY_INTMAP_LO(e)        ((e) * 0x8u)
#define IA_ENTRY_INTMAP_HI(e)        (((e) * 0x8u) + 0x4u)

#define IA_RT_VINT_BASE(v)           (v * 0x00001000u)

#ifdef CONFIG_INTERRUPT_AGGREGATOR_UNMAPPED_EVENTS
#define IA_UENTRY_MAP_LO(e)          ((e) * 0x8U)
#endif

#define IA_RT_VINT_ENABLE_SET_LO     0x00000000u
#define IA_RT_VINT_ENABLE_SET_HI     0x00000004u
#define IA_RT_VINT_ENABLE_CLR_LO     0x00000008u
#define IA_RT_VINT_ENABLE_CLR_HI     0x0000000Cu
#define IA_RT_VINT_ENABLE_CLR_RESET  0x00000000u

/*
 * IA Register Fields
 */
#define IA_ENTRY_INTMAP_REGNUM_MASK  0x0001FF00u
#define IA_ENTRY_INTMAP_REGNUM_SHIFT 0x00000008u
#define IA_ENTRY_INTMAP_REGNUM_RESET 0x00000000u
#define IA_ENTRY_INTMAP_BITNUM_MASK  0x0000003Fu
#define IA_ENTRY_INTMAP_BITNUM_SHIFT 0x00000000u
#define IA_ENTRY_INTMAP_BITNUM_RESET 0x00000000u

#ifdef CONFIG_INTERRUPT_AGGREGATOR_UNMAPPED_EVENTS
#define IA_UENTRY_MAP_IRQMODE_MASK   0x80000000U
#define IA_UENTRY_MAP_IRQMODE_SHIFT  0x0000001FU
#define IA_UENTRY_MAP_UMAPIDX_MASK   0x0000FFFFU
#define IA_UENTRY_MAP_UMAPIDX_SHIFT  0x00000000U
#endif

#define IA_GENERIC_EVT_RESETVAL      0xFFFFU

/**
 * \brief Get IA instance mapped to provided device ID
 *
 * \param id IA device ID
 *
 * \return Pointer to IA instance, NULL if no instance mapped to ID
 */
static struct ia_instance *ia_get_inst(u16 id)
{
	struct ia_instance *inst = NULL;
	u16 i;

	for (i = 0u; i < IA_INST_COUNT; i++) {
		if (ia_inst[i].id == id) {
			inst = &ia_inst[i];

			if (rm_core_validate_devgrp(inst->id, inst->devgrp) !=
			    SUCCESS) {
				/*
				 * Return NULL instance if instance devgrp
				 * doesn't align with boardcfg devgrp
				 */
				inst = NULL;
			}
			break;
		}
	}

	return inst;
}

/**
 * \brief Validates the virtual interrupt status bit index
 *
 * Verifies the virtual interrupt status bit index does not exceed the max
 * number of status bits allowed for a virtual interrupt.
 *
 * \param vint_sb_index Virtual interrupt status bit index
 *
 * \return SUCCESS if resource assignment is valid else -EINVAL
 */
static s32 ia_validate_vint_sb_index(u8 vint_sb_index)
{
	s32 r = SUCCESS;

	if (vint_sb_index >= IA_VINT_MAX_BITS) {
		r = -EINVAL;
	}

	return r;
}

/**
 * \brief Get usage count for IA VINT status bits
 *
 * \param inst Pointer to IA instance
 *
 * \param vint IA VINT index
 *
 * \return Usage count for VINT
 */
static u8 ia_get_vint_usage_count(const struct ia_instance *inst, u16 vint)
{
	return inst->vint_usage_count[vint];
}

/**
 * \brief Configure IA VINT
 *
 * \param inst Pointer to IA instance
 *
 * \param evt IA event index
 *
 * \param vint IA VINT index
 *
 * \param vint_sb_index VINT status bit to map to event
 *
 * \return SUCCESS if VINT configured, else error
 */
static s32 ia_configure_vint(struct ia_instance *inst, u16 evt, u16 vint,
			     u8 vint_sb_index)
{
	s32 r = SUCCESS;
	mapped_addr_t maddr;
	u32 entry_int_map_lo;

	maddr = rm_core_map_region(inst->imap->base);
	entry_int_map_lo = rm_fmk(IA_ENTRY_INTMAP_REGNUM_SHIFT,
				  IA_ENTRY_INTMAP_REGNUM_MASK, vint);
	entry_int_map_lo |= rm_fmk(IA_ENTRY_INTMAP_BITNUM_SHIFT,
				   IA_ENTRY_INTMAP_BITNUM_MASK, vint_sb_index);
	if (writel_verified(entry_int_map_lo,
			    maddr + (u32) (IA_ENTRY_INTMAP_LO(evt))) != SUCCESS) {
		/* Readback of write failed: halt */
		r = -EFAILVERIFY;
	}
	rm_core_unmap_region();

	/*
	 * Status bit in the VINT's real-time VINT_ENABLE_SET
	 * register is enabled as part of the firewall configuration
	 * call to Secure RM
	 */

	if (r == SUCCESS) {
		/* Increment vint usage count */
		inst->vint_usage_count[vint]++;

		if ((vint == 0u) && (vint_sb_index == 0u)) {
			/*
			 * Store event associated with VINT 0 bit 0
			 * since all INTMAP registers default to
			 * 0x0000
			 */
			inst->v0_b0_evt = evt;
		}
	}

	return r;
}

/**
 * \brief Clear event to VINT status bit mapping
 *
 * Clears the specified event to VINT mapping.
 *
 * \param inst Pointer to IA instance
 *
 * \param evt IA event index
 *
 * \param vint IA VINT index
 *
 * \param vint_sb_index VINT status bit to unmap from global event
 *
 * \return SUCCESS if VINT cleared, else error
 */
#ifdef CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS
static s32 ia_clear_vint(const struct ia_instance *inst, u16 evt, u16 vint,
			 u8 vint_sb_index)
#else
static s32 ia_clear_vint(const struct ia_instance *inst, u16 evt, u16 vint __attribute__((unused)),
			 u8 vint_sb_index __attribute__((unused)))
#endif
{
	s32 r = SUCCESS;
	mapped_addr_t maddr;
	u32 entry_int_map_lo;

#ifdef CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS
	u32 vint_enable_clr;
	u32 vint_enable_clr_addr;

	/*
	 * Disable the status bit via the VINT's real-time VINT_ENABLE_CLR
	 * register
	 */
	maddr = rm_core_map_region(inst->intr->base);
	if (vint_sb_index < 32u) {
		vint_enable_clr_addr = IA_RT_VINT_BASE(vint) +
				       IA_RT_VINT_ENABLE_CLR_LO;
		vint_enable_clr = 0x1u << vint_sb_index;
	} else {
		vint_enable_clr_addr = IA_RT_VINT_BASE(vint) +
				       IA_RT_VINT_ENABLE_CLR_HI;
		vint_enable_clr = 0x1u << (vint_sb_index - 32u);
	}
	writel(vint_enable_clr, maddr + vint_enable_clr_addr);
	/*
	 * Clearing the enable of the status bit will cause a readback to
	 * return the status of the internal enable register.  The cleared
	 * status bit must be zero.
	 */
	if ((readl(maddr + vint_enable_clr_addr) & vint_enable_clr) !=
	    0U) {
		/* Readback of cleared enable failed: halt */
		r = -EFAILVERIFY;
	}
	rm_core_unmap_region();
#endif
	if (r == SUCCESS) {
		maddr = rm_core_map_region(inst->imap->base);
		entry_int_map_lo = rm_fmk(IA_ENTRY_INTMAP_REGNUM_SHIFT,
					  IA_ENTRY_INTMAP_REGNUM_MASK, 0u);
		entry_int_map_lo |= rm_fmk(IA_ENTRY_INTMAP_BITNUM_SHIFT,
					   IA_ENTRY_INTMAP_BITNUM_MASK, 0u);

		if (writel_verified(entry_int_map_lo,
				    maddr + (u32) (IA_ENTRY_INTMAP_LO(evt))) != SUCCESS) {
			/* Readback of write failed: halt */
			r = -EFAILVERIFY;
		}
		rm_core_unmap_region();
	}

	return r;
}

/**
 * \brief Clear an event's previously existing ROM mapping
 *
 * \param inst Pointer to IA instance
 *
 * \param evt IA event index
 *
 * \return SUCCESS if no clear performed or if clear succeeds, else -EFAIL
 */
static s32 ia_clear_rom_mapping(const struct ia_instance *inst, u16 evt)
{
	s32 r = SUCCESS;
	u8 i;
	struct ia_used_mapping *used_mapping;
	mapped_addr_t maddr;
	u32 entry_int_map_lo;

	for (i = 0U; i < inst->n_rom_usage; i++) {
		used_mapping = &inst->rom_usage[i];
		if ((used_mapping->cleared == SFALSE) &&
		    ((used_mapping->event - inst->sevt_offset) == evt)) {
			maddr = rm_core_map_region(inst->imap->base);
			entry_int_map_lo = rm_fmk(IA_ENTRY_INTMAP_REGNUM_SHIFT,
						  IA_ENTRY_INTMAP_REGNUM_MASK, 0u);
			entry_int_map_lo |= rm_fmk(IA_ENTRY_INTMAP_BITNUM_SHIFT,
						   IA_ENTRY_INTMAP_BITNUM_MASK, 0u);

			if (writel_verified(entry_int_map_lo,
					    maddr + (u32) (IA_ENTRY_INTMAP_LO(evt))) != SUCCESS) {
				/* Readback of write failed: halt */
				r = -EFAILVERIFY;
			}
			rm_core_unmap_region();

			if (r == SUCCESS) {
				used_mapping->cleared = STRUE;
			}
		}
	}

	return r;
}

#ifdef CONFIG_INTERRUPT_AGGREGATOR_UNMAPPED_EVENTS
/**
 * \brief Clear an unmapped event's previously existing ROM mapping
 *
 * \param inst Pointer to IA instance
 *
 * \param evt IA unmapped event index
 *
 * \return SUCCESS if no clear performed or if clear succeeds, else -EFAIL
 */
static s32 ia_unmap_clear_rom_mapping(const struct ia_instance *inst, u16 evt)
{
	s32 r = SUCCESS;
	u8 i;
	struct ia_used_mapping *used_mapping;
	mapped_addr_t maddr;
	u32 evt_reg;

	for (i = 0U; i < inst->n_rom_usage_unmapped_events; i++) {
		used_mapping = &inst->rom_usage_unmapped_events[i];
		if ((used_mapping->cleared == SFALSE) &&
		    ((used_mapping->event - inst->sevt_offset) == evt)) {
			maddr = rm_core_map_region(inst->unmap->base);
			evt_reg = rm_fmk(IA_UENTRY_MAP_IRQMODE_SHIFT,
					 IA_UENTRY_MAP_IRQMODE_MASK, 0U);
			evt_reg |= rm_fmk(IA_UENTRY_MAP_UMAPIDX_SHIFT,
					  IA_UENTRY_MAP_UMAPIDX_MASK, 0xFFFFU);

			if (writel_verified(evt_reg,
					    maddr + (u32) (IA_UENTRY_MAP_LO(evt))) != SUCCESS) {
				/* Readback of write failed: halt */
				r = -EFAILVERIFY;
			}
			rm_core_unmap_region();

			if (r == SUCCESS) {
				used_mapping->cleared = STRUE;
			}
		}
	}

	return r;
}
#endif /* CONFIG_INTERRUPT_AGGREGATOR_UNMAPPED_EVENTS */

/**
 * \brief Validate IA event for in use or free cases
 *
 * \param inst Pointer to IA instance
 *
 * \param evt IA input event
 *
 * \param vint Virtual interrupt.  Only applicable for "in_use" check
 *
 * \param vint_sb_index VINT status bit.  Only applicable for "in_use" check
 *
 * \param in_use Validate for in use case if STRUE, else validate for free case
 *
 * \return SUCCESS if validation succeeds, else -EINVAL
 */
static s32 ia_validate_evt(const struct ia_instance *inst, u16 evt, u16 vint,
			   u8 vint_sb_index, sbool in_use)
{
	s32 r = SUCCESS;
	mapped_addr_t maddr;
	u32 entry_int_map_lo;
	u16 reg_vint, reg_sb;

	if (evt >= inst->n_sevt) {
		r = -EINVAL;
	}

	if ((r == SUCCESS) &&
	    (in_use == SFALSE) &&
	    (inst->rom_usage != NULL)) {
		/*
		 * Did ROM use the event to VINT mapping during boot?
		 * The mapping is cleared if it was used by ROM.  The clear
		 * only occurs once for each mapping.
		 */
		r = ia_clear_rom_mapping(inst, evt);
	}

	if (r == SUCCESS) {
		/* The RM initialization can be called 2 times by
		 * independent applications like bootloader and then the
		 * Sciserver. When running like this the bootloader would
		 * have already initialized the hardware state of the device
		 * which is read in entry_int_map_lo.
		 * However, when the Sciserver is run after this the
		 * hardware state will not match the software init state.
		 * Hence we read the hardware state of the register and
		 * match with what we are trying to set, and if this is
		 * matching then we set the software state accordingly.
		 */
		maddr = rm_core_map_region(inst->imap->base);

		entry_int_map_lo = readl(maddr + (u32) IA_ENTRY_INTMAP_LO(evt));

		reg_vint = (u16) rm_fext(entry_int_map_lo,
					 IA_ENTRY_INTMAP_REGNUM_SHIFT,
					 IA_ENTRY_INTMAP_REGNUM_MASK);
		reg_sb = (u16) rm_fext(entry_int_map_lo,
				       IA_ENTRY_INTMAP_BITNUM_SHIFT,
				       IA_ENTRY_INTMAP_BITNUM_MASK);
		if (in_use == STRUE) {
			/* Check if event is in use */
			if ((reg_vint == 0u) && (reg_sb == 0u)) {
				/*
				 * INTMAP register's default value is zero
				 * which signifies VINT 0 bit 0.  Validate
				 * against the instance's saved view of
				 * VINT 0 bit 0.
				 */
				if ((vint != reg_vint) &&
				    (vint_sb_index != reg_sb) &&
				    (evt != inst->v0_b0_evt)) {
					r = -EINVAL;
				}
			} else {
				if ((vint != reg_vint) &&
				    (vint_sb_index != reg_sb)) {
					r = -EINVAL;
				}
			}
		} else {
			/*
			 * Check if the register is non zero and if not equal to
			 * what is being set.
			 */
			if (((entry_int_map_lo != 0u) &&
			     (vint != reg_vint) &&
			     (vint_sb_index != reg_sb)) ||
			    (evt == inst->v0_b0_evt)) {
				r = -EINVAL;
			}
		}

		rm_core_unmap_region();
	}

	return r;
}

#ifdef CONFIG_INTERRUPT_AGGREGATOR_UNMAPPED_EVENTS
/**
 * \brief Gets the global event in an unmapped event UENTRY_MAP register
 *
 * \param host Host ID of processing entity attempting to get the event.
 *
 * \param id SoC device ID in which the OES index resides
 *
 * \param oes_index Flattened OES register index.  Needs to be decoded based on
 *                  IA instance unmapped event offsets.
 *
 * \param evt Returns the global event value within the OES index's UENTRY_MAP
 *            register
 *
 * \return SUCCESS if global event retrieved successfully, else error
 */
static s32 ia_get_oes_evt(u8 host, u16 id, u16 oes_index, u16 *evt)
{
	s32 r = SUCCESS;
	s32 clear = SUCCESS;
	const struct ia_instance *inst = NULL;
	u8 trace_action = TRACE_RM_ACTION_IRQ_IA_OES_GET;
	u8 i;
	mapped_addr_t maddr;
	u32 evt_addr;
	u32 evt_reg;

	if (evt == NULL) {
		r = -EINVAL;
	} else {
		inst = ia_get_inst(id);
		if (inst == NULL) {
			trace_action |= TRACE_RM_ACTION_FAIL;
			r = -EINVAL;
		}

		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_DEVICE_ID,
			     id);
	}

	if (r == SUCCESS) {
		r = -EINVAL;

		for (i = 0U; i < inst->n_unmapped_events; i++) {
			if ((oes_index >= inst->unmapped_events[i].start) &&
			    (oes_index <= inst->unmapped_events[i].end)) {
				if (inst->rom_usage_unmapped_events != NULL) {
					/*
					 * Did ROM use the unmapped event during boot?
					 * The mapping is cleared if it was used by ROM.
					 * The clear only occurs once for each mapping.
					 */
					clear = ia_unmap_clear_rom_mapping(inst, oes_index);
				}
				if (clear == SUCCESS) {
					r = rm_core_resasg_validate_resource(
						host,
						inst->unmapped_events[i].utype,
						oes_index);
				} else {
					r = clear;
				}
				break;
			}
		}

		if (r != SUCCESS) {
			trace_action |= TRACE_RM_ACTION_FAIL;
		}

		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_OES_REG_INDEX,
			     oes_index);
	}

	if (r == SUCCESS) {
		maddr = rm_core_map_region(inst->unmap->base);
		evt_addr = maddr + (u32) (IA_UENTRY_MAP_LO(oes_index));

		evt_reg = readl(evt_addr);
		rm_core_unmap_region();

		*evt = (u16) rm_fext(evt_reg, IA_UENTRY_MAP_UMAPIDX_SHIFT,
				     IA_UENTRY_MAP_UMAPIDX_MASK);
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_EVENT,
			     *evt);
	}

	return r;
}

/**
 * \brief Sets the global event in an unmapped event UENTRY_MAP register
 *
 * \param host Host ID of processing entity attempting to set the event.
 *
 * \param id SoC device ID in which the OES index resides
 *
 * \param oes_index Flattened OES register index.  Needs to be decoded based on
 *                  IA instance unmapped event offsets.
 *
 * \param evt Global event value to program into the OES index's UENTRY_MAP
 *            register
 *
 * \return SUCCESS if global event programmed successfully, else error
 */
static s32 ia_set_oes_evt(u8 host, u16 id, u16 oes_index, u16 evt)
{
	s32 r = SUCCESS;
	s32 clear = SUCCESS;
	const struct ia_instance *inst = NULL;
	u8 i;
	mapped_addr_t maddr;
	u32 evt_reg;
	u32 evt_addr;
	u8 trace_action = TRACE_RM_ACTION_IRQ_IA_OES_SET;

	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_EVENT,
		     evt);

	inst = ia_get_inst(id);
	if (inst == NULL) {
		trace_action |= TRACE_RM_ACTION_FAIL;
		r = -EINVAL;
	}
	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_DEVICE_ID,
		     id);

	if (r == SUCCESS) {
		r = -EINVAL;

		for (i = 0U; i < inst->n_unmapped_events; i++) {
			if ((oes_index >= inst->unmapped_events[i].start) &&
			    (oes_index <= inst->unmapped_events[i].end)) {
				if (inst->rom_usage_unmapped_events != NULL) {
					/*
					 * Did ROM use the unmapped event during boot?
					 * The mapping is cleared if it was used by ROM.
					 * The clear only occurs once for each mapping.
					 */
					clear = ia_unmap_clear_rom_mapping(inst, oes_index);
				}
				if (clear == SUCCESS) {
					r = rm_core_resasg_validate_resource(
						host,
						inst->unmapped_events[i].utype,
						oes_index);
				} else {
					r = clear;
				}
				break;
			}
		}

		if (r != SUCCESS) {
			trace_action |= TRACE_RM_ACTION_FAIL;
		}

		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_OES_REG_INDEX,
			     oes_index);
	}

	if (r == SUCCESS) {
		/* oes_index verified already at this point */
		maddr = rm_core_map_region(inst->unmap->base);
		evt_reg = rm_fmk(IA_UENTRY_MAP_IRQMODE_SHIFT,
				 IA_UENTRY_MAP_IRQMODE_MASK, 0U);
		evt_reg |= rm_fmk(IA_UENTRY_MAP_UMAPIDX_SHIFT,
				  IA_UENTRY_MAP_UMAPIDX_MASK, evt);
		evt_addr = maddr + (u32) (IA_UENTRY_MAP_LO(oes_index));

		if (writel_verified(evt_reg, evt_addr) != SUCCESS) {
			/* Readback of write failed: halt */
			r = -EFAILVERIFY;
		}
		rm_core_unmap_region();
	}

	return r;
}
#endif /* CONFIG_INTERRUPT_AGGREGATOR_UNMAPPED_EVENTS */

sbool rm_ia_is_managed_resasg_utype(u16 utype)
{
	sbool r = SFALSE;
	u16 i;

#ifdef CONFIG_INTERRUPT_AGGREGATOR_UNMAPPED_EVENTS
	u8 j;
#endif

	for (i = 0U; i < IA_INST_COUNT; i++) {
		if (utype == ia_inst[i].vint_utype) {
			r = STRUE;
		}

#ifdef CONFIG_INTERRUPT_AGGREGATOR_UNMAPPED_EVENTS
		if (r == SFALSE) {
			for (j = 0U; j < ia_inst[i].n_unmapped_events; j++) {
				if (utype == ia_inst[i].unmapped_events[j].utype) {
					r = STRUE;
					break;
				}
			}
		}
#endif

		if (r == STRUE) {
			break;
		}
	}

	return r;
}

s32 rm_ia_validate_vint(u8 host, u16 id, u16 vint)
{
	s32 r = SUCCESS;
	const struct ia_instance *inst = NULL;

	inst = ia_get_inst(id);
	if (inst == NULL) {
		r = -EINVAL;
	} else {
		if (vint >= inst->n_vint) {
			r = -EINVAL;
		}
	}

	if (r == SUCCESS) {
		r = rm_core_resasg_validate_resource(host, inst->vint_utype,
						     vint);
	}

	return r;
}

#ifdef CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS
s32 rm_ia_vint_map(u16 id, u16 vint, u16 global_evt, u8 vint_sb_index,
		   sbool fwl_cfg_dmsc_only, sbool validate_sec_rm_devgrp)
#else
s32 rm_ia_vint_map(u16 id, u16 vint, u16 global_evt, u8 vint_sb_index,
		   sbool fwl_cfg_dmsc_only __attribute__((unused)),
		   sbool validate_sec_rm_devgrp __attribute__((unused)))
#endif
{
	s32 r = SUCCESS;
	struct ia_instance *inst;
	u16 evt;
	u8 trace_action = TRACE_RM_ACTION_IRQ_IA_MAP_VINT;

#ifdef CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS
	u8 hosts[FWL_MAX_PRIVID_SLOTS];
	u8 n_hosts = 0U;
#endif

	inst = ia_get_inst(id);
	if (inst == NULL) {
		trace_action |= TRACE_RM_ACTION_FAIL;
		r = -EINVAL;
	}
	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_DEVICE_ID,
		     id);

	if (r == SUCCESS) {
		r = ia_validate_vint_sb_index(vint_sb_index);
		if (r != SUCCESS) {
			trace_action |= TRACE_RM_ACTION_FAIL;
		}
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IA_VINT_STATUS_BIT,
			     vint_sb_index);
	}

	if (r == SUCCESS) {
		evt = global_evt - inst->sevt_offset;
		/* Check if event is free */
		r = ia_validate_evt(inst, evt, 0U, 0U, SFALSE);
		if (r != SUCCESS) {
			trace_action |= TRACE_RM_ACTION_FAIL;
		}
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IA_GLOBAL_EVENT,
			     global_evt);
	}

	if (r == SUCCESS) {
		if (vint >= inst->n_vint) {
			r = -EINVAL;
		} else {
			/* Check if VINT mapping is full */
			if (ia_get_vint_usage_count(inst, vint) >=
			    IA_VINT_MAX_BITS) {
				r = -EINVAL;
			}
		}

		if (r != SUCCESS) {
			trace_action |= TRACE_RM_ACTION_FAIL;
		}
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IA_VINT,
			     vint);
	}

#ifdef CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS
	if (r == SUCCESS) {
		/* Call Secure RM to configure VINT firewalls */

		r = rm_core_get_resasg_hosts(inst->vint_utype,
					     vint,
					     &n_hosts,
					     &hosts[0U],
					     FWL_MAX_PRIVID_SLOTS);

		if (r == SUCCESS) {
			/*
			 * Status bit in the VINT's real-time VINT_ENABLE_SET
			 * register is enabled as part of the firewall
			 * configuration call to Secure RM
			 */
			r = sec_rm_ia_vint_fwl_cfg(inst->id, vint,
						   vint_sb_index,
						   hosts, n_hosts,
						   fwl_cfg_dmsc_only,
						   validate_sec_rm_devgrp);
		}
	}
#endif

	if (r == SUCCESS) {
		r = ia_configure_vint(inst, evt, vint, vint_sb_index);
	}

	return r;
}

s32 rm_ia_vint_unmap(u16 id, u16 vint, u16 global_evt, u8 vint_sb_index)
{
	s32 r = SUCCESS;
	struct ia_instance *inst;
	u16 evt;
	u8 trace_action = TRACE_RM_ACTION_IRQ_IA_UNMAP_VINT;

	inst = ia_get_inst(id);
	if (inst == NULL) {
		trace_action |= TRACE_RM_ACTION_FAIL;
		r = -EINVAL;
	}
	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_DEVICE_ID,
		     id);

	if (r == SUCCESS) {
		if (vint >= inst->n_vint) {
			r = -EINVAL;
		} else {
			/* Check if VINT mapping is full */
			if (ia_get_vint_usage_count(inst, vint) == 0u) {
				r = -EINVAL;
			}
		}
		if (r != SUCCESS) {
			trace_action |= TRACE_RM_ACTION_FAIL;
		}
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IA_VINT,
			     vint);
	}

	if (r == SUCCESS) {
		r = ia_validate_vint_sb_index(vint_sb_index);
		if (r != SUCCESS) {
			trace_action |= TRACE_RM_ACTION_FAIL;
		}
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IA_VINT_STATUS_BIT,
			     vint_sb_index);
	}

	if (r == SUCCESS) {
		evt = global_evt - inst->sevt_offset;
		/* Check if event is mapped to vint status bit */
		r = ia_validate_evt(inst, evt, vint, vint_sb_index, STRUE);
		if (r != SUCCESS) {
			trace_action |= TRACE_RM_ACTION_FAIL;
		}
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IA_GLOBAL_EVENT,
			     global_evt);
	}

	if (r == SUCCESS) {
		r = ia_clear_vint(inst, evt, vint, vint_sb_index);
	}
	if (r == SUCCESS) {
		inst->vint_usage_count[vint]--;

		if (inst->v0_b0_evt == evt) {
			/* Cleared event was mapped to VINT 0 bit 0 */
			inst->v0_b0_evt = IA_GENERIC_EVT_RESETVAL;
		}
	}

	return r;
}

s32 rm_ia_vint_get_info(u16 id, u16 vint, u8 *n_evts)
{
	s32 r = SUCCESS;
	const struct ia_instance *inst;

	if (n_evts == NULL) {
		r = -EINVAL;
	} else {
		inst = ia_get_inst(id);
		if (inst == NULL) {
			r = -EINVAL;
		} else {
			if (vint >= inst->n_vint) {
				r = -EINVAL;
			}
		}
	}

	if (r == SUCCESS) {
		*n_evts = inst->vint_usage_count[vint];
	}

	return r;
}

s32 rm_ia_init(void)
{
	s32 r = SUCCESS;
	u32 i, j;

	for (i = 0U; i < IA_INST_COUNT; i++) {
		if ((rm_core_validate_devgrp(ia_inst[i].id, ia_inst[i].devgrp) ==
		     SUCCESS) &&
		    (ia_inst[i].initialized == SFALSE)) {
#if defined(CONFIG_RM_IRQ) && defined(CONFIG_INTERRUPT_AGGREGATOR_UNMAPPED_EVENTS)
			ia_inst[i].oes_handler.oes_get = ia_get_oes_evt;
			ia_inst[i].oes_handler.oes_set = ia_set_oes_evt;

			r = rm_irq_oes_src_register(ia_inst[i].id,
						    &ia_inst[i].oes_handler);
#endif

			for (j = 0U; j < ia_inst[i].n_vint; j++) {
				ia_inst[i].vint_usage_count[j] = 0U;
			}

			ia_inst[i].v0_b0_evt = IA_GENERIC_EVT_RESETVAL;

			for (j = 0U;
			     (j < IA_SOC_PE_INIT_NUM) && (r == SUCCESS);
			     j++) {
				if (ia_soc_pe_init_list[j].id == ia_inst[i].id) {
#ifdef CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS
					r = rm_ia_vint_map(
						ia_soc_pe_init_list[j].id,
						ia_soc_pe_init_list[j].vint,
						ia_soc_pe_init_list[j].event_id,
						0U,
						STRUE,
						SFALSE);
#else
					struct ia_instance *inst;
					u16 evt;
					u16 vint_sb_index = 0U;
					u16 vint = ia_soc_pe_init_list[j].vint;
					inst = ia_get_inst(ia_soc_pe_init_list[j].id);
					if (inst == NULL) {
						r = -EINVAL;
					}
					if (r == SUCCESS) {
						evt = ia_soc_pe_init_list[j].event_id - inst->sevt_offset;
						/* Increment vint usage count */
						inst->vint_usage_count[vint]++;
						if ((vint == 0u) && (vint_sb_index == 0u)) {
							/*
							 * Store event associated with VINT 0 bit 0
							 * since all INTMAP registers default to
							 * 0x0000
							 */
							inst->v0_b0_evt = evt;
						}
					}
#endif
				}
			}

			if (r != SUCCESS) {
				break;
			} else {
				ia_inst[i].initialized = STRUE;
			}
		}
	}


	if (r != SUCCESS) {
		rm_trace_sub((TRACE_RM_ACTION_IRQ_IA_INIT |
			      TRACE_RM_ACTION_FAIL), 0U, 0U);
	}

	return r;
}

s32 rm_ia_deinit(devgrp_t devgrp, sbool rom_deinit)
{
	s32 r = -EFAIL;
	u8 i, j;

	for (i = 0U; i < IA_INST_COUNT; i++) {
		if ((rm_core_validate_devgrp(ia_inst[i].id, ia_inst[i].devgrp) ==
		     SUCCESS) &&
		    (ia_inst[i].initialized == STRUE) &&
		    (ia_inst[i].devgrp == devgrp)) {
			ia_inst[i].initialized = SFALSE;

			if (rom_deinit == STRUE) {
				/* Since ROM will be using these resources again, reset the ROM usage flags,
				 * so that way these resources are able to be cleared again during RM init.
				 */
				for (j = 0U; j < (ia_inst[i].n_rom_usage); j++) {
					ia_inst[i].rom_usage[j].cleared = SFALSE;
				}

				for (j = 0U; j < (ia_inst[i].n_rom_usage_unmapped_events); j++) {
					ia_inst[i].rom_usage_unmapped_events[j].cleared = SFALSE;
				}
			}
			r = SUCCESS;
		}
	}

	return r;
}
