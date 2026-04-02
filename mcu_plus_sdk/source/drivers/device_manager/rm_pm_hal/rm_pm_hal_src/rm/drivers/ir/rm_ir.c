/*
 * IRQ Interrupt Router
 *
 * IRQ management interrupt router infrastructure
 *
 * Copyright (C) 2017-2025, Texas Instruments Incorporated
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
#include <lib/io.h>
#include <lib/trace.h>
#include <hosts_internal.h>

#include <config.h>

#include <rm_core.h>
#include <rm_ir.h>
#include <rm_lpm.h>

#include <ir_inst.h>
#include <ir_init.h>

#include "ir_cfg.h"

#define IR_MAPPING_FREE                  0xFFFFU

/*
 * IR Register Macros
 */
#define IR_INT_CONTROL_REG_OFFSET        0x00000004U
#define IR_INT_CONTROL_REG_STEP          0x00000004U
#define IR_INT_CONTROL_REG_RESET         0x00000000U

/*
 * IR Register Fields
 */
#define IR_INT_CONTROL_MUX_CONTROL_MASK  0x00000FFFU
#define IR_INT_CONTROL_MUX_CONTROL_SHIFT 0x00000000U
#define IR_INT_CONTROL_MUX_CONTROL_RESET 0x00000000U

#define IR_INT_CONTROL_INT_ENABLE_MASK   0x00010000U
#define IR_INT_CONTROL_INT_ENABLE_SHIFT  0x00000010U
#define IR_INT_CONTROL_INT_ENABLE_RESET  0x00000000U

/**
 * \brief Returns the control register off for an IR output
 *
 * \param IR output index
 *
 * \return register offset for the output
 */
static u32 ir_int_control_reg(u16 outp)
{
	u32 ret;

	ret = ((u32) outp * IR_INT_CONTROL_REG_STEP) + IR_INT_CONTROL_REG_OFFSET;
	return ret;
}

/**
 * \brief Get IR instance mapped to provided device ID
 *
 * \param id IR device ID
 *
 * \return Pointer to IR instance, NULL if no instance mapped to ID
 */
static struct ir_instance *ir_get_inst(u16 id)
{
	struct ir_instance *inst = NULL;
	u16 i;

	for (i = 0U; i < IR_INST_COUNT; i++) {
		if (ir_inst[i].id == id) {
			inst = &ir_inst[i];

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
 * \brief Get IR input mapped to IR output
 *
 * \param inst Pointer to IR instance
 *
 * \param outp IR output index
 *
 * \return The IR input mapped within the IR output control register
 */
static u16 ir_get_inp(const struct ir_instance *inst, u16 outp)
{
	mapped_addr_t maddr = rm_core_map_region(inst->cfg->base);
	u32 int_ctrl_reg;
	u16 inp;

	/* Get the input by reading the output control register */
	int_ctrl_reg = readl(maddr + ir_int_control_reg(outp));
	inp = (u16) rm_fext(int_ctrl_reg, IR_INT_CONTROL_MUX_CONTROL_SHIFT,
			    IR_INT_CONTROL_MUX_CONTROL_MASK);

	rm_core_unmap_region();

	return inp;
}

/**
 * \brief Configures usage of an IR output
 *
 * \param inst Pointer to IR instance
 *
 * \param inp IR input index
 *
 * \param outp IR output index
 *
 * \return SUCCESS if programming succeeded, else -EFAILVERIFY
 */
static s32 ir_configure_outp(struct ir_instance *inst, u16 inp, u16 outp)
{
	s32 r = SUCCESS;
	mapped_addr_t maddr;
	u32 int_ctrl_reg;

	maddr = rm_core_map_region(inst->cfg->base);
	int_ctrl_reg = rm_fmk(IR_INT_CONTROL_MUX_CONTROL_SHIFT,
			      IR_INT_CONTROL_MUX_CONTROL_MASK, (u32) inp);
	if (inst->int_enable == STRUE) {
		int_ctrl_reg |= rm_fmk(IR_INT_CONTROL_INT_ENABLE_SHIFT,
				       IR_INT_CONTROL_INT_ENABLE_MASK, (u32) 1U);
	}
	if (writel_verified(int_ctrl_reg,
			    maddr + ir_int_control_reg(outp)) != SUCCESS) {
		r = -EFAILVERIFY;
	}
	rm_core_unmap_region();

	if ((r == SUCCESS) && (inp == 0u)) {
		/*
		 * Special instance storage for mapping of input zero since
		 * output CONTROL register reset values are zero
		 */
		inst->inp0_mapping = outp;
	}

	return r;
}

/**
 * \brief Clear usage of an IR output
 *
 * \param inst Pointer to IR instance
 *
 * \param outp IR output index
 *
 * \return SUCCESS if clear succeeded, else -EFAILVERIFY
 */
static s32 ir_clear_outp(const struct ir_instance *inst, u16 outp)
{
	s32 r = SUCCESS;
	mapped_addr_t maddr;
	u32 int_ctrl_reg;

	maddr = rm_core_map_region(inst->cfg->base);
	int_ctrl_reg = rm_fmk(IR_INT_CONTROL_MUX_CONTROL_SHIFT,
			      IR_INT_CONTROL_MUX_CONTROL_MASK, 0u);
	if (inst->int_enable == STRUE) {
		int_ctrl_reg |= rm_fmk(IR_INT_CONTROL_INT_ENABLE_SHIFT,
				       IR_INT_CONTROL_INT_ENABLE_MASK, (u32) 0U);
	}
	if (writel_verified(int_ctrl_reg, maddr +
			    ir_int_control_reg(outp)) != SUCCESS) {
		/* Readback of write failed: halt */
		r = -EFAILVERIFY;
	}
	rm_core_unmap_region();

	return r;
}

/**
 * \brief Clear an input to output mapping previously existing ROM mapping
 *
 * \param inst Pointer to IR instance
 *
 * \param inp IR input
 *
 * \param inp_valid IR input valid boolean
 *
 * \param outp IR output
 *
 * \param outp_valid IR output valid boolean
 *
 * \return SUCCESS if clear succeeded, else -EFAILVERIFY
 */
static s32 ir_clear_rom_mapping(const struct ir_instance	*inst,
				u16				inp,
				sbool				inp_valid,
				u16				outp,
				sbool				outp_valid)
{
	s32 r = SUCCESS;
	u8 i, j;
	struct ir_used_mapping *used_mapping;

	for (i = 0U; i < inst->n_rom_usage; i++) {
		used_mapping = &inst->rom_usage[i];
		if (((used_mapping->cleared == SFALSE) &&
		     ((inp_valid == STRUE) &&
		      (inp >= used_mapping->inp_start) &&
		      (inp < (used_mapping->inp_start + used_mapping->length)))) ||
		    (((outp_valid == STRUE) &&
		      (outp >= used_mapping->outp_start) &&
		      (outp < (used_mapping->outp_start + used_mapping->length))))) {
			for (j = 0U;
			     (j < used_mapping->length) && (r == SUCCESS);
			     j++) {
				if ((used_mapping->inp_start + j) ==
				    ir_get_inp(inst,
					       (used_mapping->outp_start + j))) {
					/*
					 * Only clear the mapping if it matches
					 * the entry.  Otherwise, the clear is
					 * wiping out a SYSFW configuration
					 */
					r = ir_clear_outp(inst,
							  (used_mapping->outp_start + j));
				}
			}
			used_mapping->cleared = STRUE;
			break;
		}
	}

	return r;
}

/**
 * \brief Checks the IR output control registers to see if the input has been
 *        mapped to an output.
 *
 * \param id IR SoC device ID.
 *
 * \param inp Input line to IR
 *
 * \return SUCCESS if input line is free, else -EFAIL
 */
static s32 ir_inp_is_free(const struct ir_instance	*inst,
			  u16				inp)
{
	s32 r = SUCCESS;
	u16 i;
	mapped_addr_t maddr;
	u32 int_ctrl_reg;
	u16 extracted_inp;

	if (inp >= inst->n_inp) {
		r = -EINVAL;
	}

	if ((r == SUCCESS) && (inst->rom_usage != NULL)) {
		/*
		 * Did ROM use the input during boot?  The input to
		 * output mapping is cleared if it was used by ROM.
		 * The clear only occurs once for each mapping.
		 */
		r = ir_clear_rom_mapping(inst, inp, STRUE, 0, SFALSE);
	}

	if (r == SUCCESS) {
		if (inp == 0u) {
			if (inst->inp0_mapping != ((u16) IR_MAPPING_FREE)) {
				r = -EFAIL;
			}
		} else {
			maddr = rm_core_map_region(inst->cfg->base);

			/*
			 * Check IR control registers to see if inp has been
			 * mapped to an outp
			 */
			for (i = 0u; i < inst->n_outp; i++) {
				int_ctrl_reg = readl(maddr +
						     ir_int_control_reg(i));
				extracted_inp = (u16) rm_fext(
					int_ctrl_reg,
					IR_INT_CONTROL_MUX_CONTROL_SHIFT,
					IR_INT_CONTROL_MUX_CONTROL_MASK);

				if (extracted_inp == inp) {
					/* Input in use */
					r = -EFAIL;
					break;
				}
			}

			rm_core_unmap_region();
		}
	}

	return r;
}

/**
 * \brief Checks the output's control register to see if the output line has an
 *        input line mapped to it.
 *
 * \param id IR SoC device ID.
 *
 * \param outp Output line from IR
 *
 * \return SUCCESS if output line is free, else -EFAIL
 */
static s32 ir_outp_is_free(const struct ir_instance	*inst,
			   u16				outp)
{
	s32 r = SUCCESS;

	if (outp >= inst->n_outp) {
		r = -EINVAL;
	}

	if ((r == SUCCESS) && (inst->rom_usage != NULL)) {
		/*
		 * Did ROM use the output during boot?  The input to
		 * output mapping is cleared if it was used by ROM.
		 * The clear only occurs once for each mapping.
		 */
		r = ir_clear_rom_mapping(inst, 0, SFALSE, outp, STRUE);
	}

	if (r == SUCCESS) {
		if ((ir_get_inp(inst, outp) != 0u) ||
		    (outp == inst->inp0_mapping)) {
			/*
			 * MUX CONTROL register's default value is zero which
			 * also signifies IR input zero.  Check the instance
			 * for the output that has been routed input zero.
			 */
			r = -EFAIL;
		}
	}

	return r;
}

sbool rm_ir_is_managed_resasg_utype(u16 utype)
{
	sbool r = SFALSE;
	u16 i;

	for (i = 0U; i < IR_INST_COUNT; i++) {
		if (utype == ir_inst[i].outp_utype) {
			r = STRUE;
			break;
		}
	}

	return r;
}

s32 rm_ir_get_outp(u16 id, u16 inp, u16 *outp)
{
	s32 r = SUCCESS;
	const struct ir_instance *inst = NULL;
	u16 i;
	mapped_addr_t maddr;
	u32 int_ctrl_reg;
	u16 extracted_inp;

	if (outp == NULL) {
		r = -EINVAL;
	} else {
		inst = ir_get_inst(id);
		if (inst == NULL) {
			r = -EINVAL;
		} else {
			if (inp >= inst->n_inp) {
				r = -EINVAL;
			}
		}
	}

	if ((r == SUCCESS) && (inp == 0u)) {
		/* Use instance's input zero mapping to find output */
		if (inst->inp0_mapping != IR_MAPPING_FREE) {
			/* Input zero mapping is valid.  Return the output. */
			*outp = inst->inp0_mapping;
		} else {
			r = -EFAIL;
		}
	}

	if ((r == SUCCESS) && (inp != 0u)) {
		maddr = rm_core_map_region(inst->cfg->base);
		r = -EFAIL;

		/*
		 * Use IR control registers to find the output mapped to the
		 * input
		 */
		for (i = 0u; i < inst->n_outp; i++) {
			int_ctrl_reg = readl(maddr + ir_int_control_reg(i));
			extracted_inp = (u16) rm_fext(
				int_ctrl_reg,
				IR_INT_CONTROL_MUX_CONTROL_SHIFT,
				IR_INT_CONTROL_MUX_CONTROL_MASK);

			if (inp == extracted_inp) {
				*outp = i;
				r = SUCCESS;
				break;
			}
		}

		rm_core_unmap_region();
	}

	return r;
}

s32 rm_ir_validate_output(u8 host, u16 id, u16 outp)
{
	s32 r = -EINVAL;
	const struct ir_instance *inst = NULL;

	inst = ir_get_inst(id);
	if (inst != NULL) {
		if (outp < inst->n_outp) {
			r = rm_core_resasg_validate_resource(host,
							     inst->outp_utype,
							     outp);
		}
	}

	return r;
}

s32 rm_ir_configure(u16 id, u16 inp, u16 outp, sbool validate)
{
	s32 r = SUCCESS;
	struct ir_instance *inst = NULL;
	u8 trace_action = TRACE_RM_ACTION_IRQ_IR_CFG;

	inst = ir_get_inst(id);
	if (inst == NULL) {
		trace_action |= TRACE_RM_ACTION_FAIL;
		r = -EINVAL;
	}
	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_DEVICE_ID,
		     id);

	if (r == SUCCESS) {
		if (validate == STRUE) {
			r = ir_inp_is_free(inst, inp);
			if (r != SUCCESS) {
				trace_action |= TRACE_RM_ACTION_FAIL;
			}
		}
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IR_INPUT,
			     inp);
	}

	if (r == SUCCESS) {
		if (validate == STRUE) {
			r = ir_outp_is_free(inst, outp);
			if (r != SUCCESS) {
				trace_action |= TRACE_RM_ACTION_FAIL;
			}
		}
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IR_OUTPUT,
			     outp);
	}

	if (r == SUCCESS) {
		r = ir_configure_outp(inst, inp, outp);
	}

	if (r == SUCCESS) {
		r = lpm_sleep_save_ir_config(id, inp, outp, validate);
	}

	return r;
}

s32 rm_ir_clear(u16 id, u16 inp, u16 outp)
{
	s32 r = SUCCESS;
	struct ir_instance *inst = NULL;
	sbool clear_outp = SFALSE;
	u8 trace_action = TRACE_RM_ACTION_IRQ_IR_CLR;

	inst = ir_get_inst(id);
	if (inst == NULL) {
		trace_action |= TRACE_RM_ACTION_FAIL;
		r = -EINVAL;
	}
	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_DEVICE_ID,
		     id);

	if (r == SUCCESS) {
		if ((inp < inst->n_inp) && (outp < inst->n_outp)) {
			if ((inp == 0u) && (outp == inst->inp0_mapping)) {
				clear_outp = STRUE;
			}

			if ((inp != 0u) && (inp == ir_get_inp(inst, outp))) {
				clear_outp = STRUE;
			}

			if (clear_outp == SFALSE) {
				trace_action |= TRACE_RM_ACTION_FAIL;
				r = -EINVAL;
			}
		} else {
			trace_action |= TRACE_RM_ACTION_FAIL;
			r = -EINVAL;
		}

		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IR_INPUT,
			     inp);
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IR_OUTPUT,
			     outp);
	}

	if (clear_outp == STRUE) {
		ir_clear_outp(inst, outp);

		if (inp == 0u) {
			/*
			 * Special instance storage for mapping of input zero
			 * since output CONTROL register reset values are zero
			 */
			inst->inp0_mapping = IR_MAPPING_FREE;
		}
	}

	if (r == SUCCESS) {
		r = lpm_sleep_save_ir_unconfig(id, inp, outp);
	}

	return r;
}

s32 rm_ir_init(void)
{
	s32 r = SUCCESS;
	u16 i;

#if (IR_SOC_PE_INIT_NUM > 0U)
	u16 j;
#endif

	for (i = 0U; i < IR_INST_COUNT; i++) {
		if ((rm_core_validate_devgrp(ir_inst[i].id, ir_inst[i].devgrp) ==
		     SUCCESS) &&
		    (ir_inst[i].initialized == SFALSE)) {
			ir_inst[i].inp0_mapping = IR_MAPPING_FREE;

#if (IR_SOC_PE_INIT_NUM > 0U)
			for (j = 0U;
			     (j < IR_SOC_PE_INIT_NUM) && (r == SUCCESS);
			     j++) {
				if (ir_soc_pe_init_list[j].id == ir_inst[i].id) {
					r = rm_ir_configure(
						ir_soc_pe_init_list[j].id,
						ir_soc_pe_init_list[j].inp,
						ir_soc_pe_init_list[j].outp,
						SFALSE);
				}
			}
#endif

			ir_inst[i].initialized = STRUE;
		}

		if (r != SUCCESS) {
			break;
		}
	}

	if (r != SUCCESS) {
		rm_trace_sub((TRACE_RM_ACTION_IRQ_IR_INIT |
			      TRACE_RM_ACTION_FAIL), 0U, 0U);
	}

	return r;
}

s32 rm_ir_deinit(devgrp_t devgrp, sbool rom_deinit)
{
	s32 r = -EFAIL;
	u8 i, j;

	for (i = 0U; i < IR_INST_COUNT; i++) {
		if ((rm_core_validate_devgrp(ir_inst[i].id, ir_inst[i].devgrp) ==
		     SUCCESS) &&
		    (ir_inst[i].initialized == STRUE) &&
		    (ir_inst[i].devgrp == devgrp)) {
			ir_inst[i].initialized = SFALSE;

			if (rom_deinit == STRUE) {
				/* Since ROM will be using these resources again, reset the ROM usage flags,
				 * so that way these resources are able to be cleared again during RM init.
				 */
				for (j = 0U; j < (ir_inst[i].n_rom_usage); j++) {
					ir_inst[i].rom_usage[j].cleared = SFALSE;
				}
			}

			r = SUCCESS;
		}
	}

	return r;
}
