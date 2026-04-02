/*
 * DMSC RM IRQ
 *
 * Resource Manager IRQ infrastructure
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

#include <types/short_types.h>
#include <types/errno.h>
#include <lib/container_of.h>
#include <lib/trace.h>
#include <hosts_internal.h>
#include <devices_internal.h>

#include <osal/osal_core.h>
#include <tisci/rm/tisci_rm_irq.h>
#include <rm.h>

#include <rm_core.h>
#include <rm_irq.h>
#include <rm_ir.h>
#include <rm_ia.h>

#include "include/irq_resasg.h"

#include <irq_cfg.h>

/**
 * Reset value for device output event (OES) registers
 */
#define IRQ_OES_RESETVAL (0xFFFFu)

/**
 * \brief IRQ route configuration
 *
 * \param valid_params Validity information for optional parameters
 *
 * \param host IRQ route destination host
 *
 * \param s_id Device ID of peripheral sourcing the IRQ
 *
 * \param s_idx Peripheral resource index sourcing the IRQ
 *
 * \param d_id Device ID of IRQ route destination host
 *
 * \param d_irq Destination interrupt controller input IRQ
 *
 * \param oes_src Pointer to the source device's OES handler if it exists -
 *                applicable if source peripheral to destination host route
 *                requires use of an IA
 *
 * \param global_evt Global event - applicable if source peripheral to
 *                   destination host route requires use of an IA
 *
 * \param s_ia Source IA - applicable if source peripheral to destination host
 *             route requires use of an IA
 *
 * \param vint VINT - applicable if source peripheral to destination
 *             host route requires use of an IA
 *
 * \param vint_sb VINT status bit - applicable if source peripheral to
 *                destination host route requires use of an IA
 */
struct irq_cfg {
	u32			valid_params;
	u8			host;
	u16			s_id;
	u16			s_idx;
	u16			d_id;
	u16			d_irq;
	struct rm_irq_oes_src	*oes_src;
	u16			global_evt;
	u16			s_ia;
	u16			vint;
	u8			vint_sb;
};

/**
 * Output event source map
 */
static struct map *oes_src_map;

/**
 * \brief Lookup and return an rm_irq_oes_src that was registered with a SoC
 *        device ID
 *
 * This function uses a map to lookup the OES register handlers registered using
 * rm_irq_oes_src_register.
 *
 * \param id SoC device ID to return rm_irq_oes_src for.
 *
 * \return Pointer to the rm_irq_oes_src mapped to id or NULL if a src was not
 *         registered for SoC device id.
 */
static struct rm_irq_oes_src *irq_oes_src_lookup(u16 id)
{
	struct map_node *n;
	struct rm_irq_oes_src *src;

	/* Not finding a node mapped to input id is not a failure.
	 * It just means the SoC device ID does not have any registered
	 * output event handlers */
	n = u32map_get(oes_src_map, id);
	if (n != NULL) {
		src = container_of(n, struct rm_irq_oes_src, node);
	} else {
		src = NULL;
	}

	return src;
}

/**
 * \brief Check if valid parameter configuration is for a programming only an
 *        interrupt router MUX
 *
 * \param valid_params Valid params field from message
 *
 * \return STRUE if cfg is for IR configuration, else SFALSE
 */
static sbool irq_cfg_is_ir_cfg(u32 valid_params)
{
	sbool r = SFALSE;

	/*
	 * The interrupt configuration is for configuring an IR input to output
	 * connection
	 */
	if ((rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_DST_ID_VALID) == STRUE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID) == STRUE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_IA_ID_VALID) == SFALSE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_VINT_VALID) == SFALSE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_GLOBAL_EVENT_VALID) == SFALSE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_VINT_STATUS_BIT_INDEX_VALID) == SFALSE)) {
		r = STRUE;
	}

	return r;
}

/**
 * \brief Check if valid parameter configuration is for configuring an IA event
 *        to virtual interrupt.
 *
 * \param valid_params Valid params field from message
 *
 * \return STRUE if cfg is for IA configuration, else SFALSE
 */
static sbool irq_cfg_is_ia_cfg(u32 valid_params)
{
	sbool r = SFALSE;

	/*
	 * The interrupt configuration is for configuring an OES source to IA
	 * VINT status bit.
	 */
	if ((rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_DST_ID_VALID) == SFALSE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID) == SFALSE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_IA_ID_VALID) == STRUE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_VINT_VALID) == STRUE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_GLOBAL_EVENT_VALID) == STRUE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_VINT_STATUS_BIT_INDEX_VALID) == STRUE)) {
		r = STRUE;
	}

	return r;
}

/**
 * \brief Check if valid parameter configuration is for OES register
 *        programming only.
 *
 * \param valid_params Valid params field from message
 *
 * \return STRUE if cfg is for OES programming only, else SFALSE
 */
static sbool irq_cfg_is_oes_only(u32 valid_params)
{
	sbool r = SFALSE;

	/*
	 * The interrupt configuration provided to only program the OES
	 * register of a peripheral capable of generating events.
	 */
	if ((rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_DST_ID_VALID) == SFALSE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID) == SFALSE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_IA_ID_VALID) == SFALSE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_VINT_VALID) == SFALSE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_GLOBAL_EVENT_VALID) == STRUE) &&
	    (rm_core_param_is_valid(valid_params,
				    TISCI_MSG_VALUE_RM_VINT_STATUS_BIT_INDEX_VALID) == SFALSE)) {
		r = STRUE;
	}

	return r;
}

/**
 * \brief Validates a global event against the board cfg resasg data
 *
 * \param host Host ID
 *
 * \param global_evt Global event
 *
 * \param trace_action RA action to trace
 *
 * \return SUCCESS if global event is valid, else -EINVAL
 */
static s32 irq_validate_global_event(u8 host, u16 global_evt, u8 trace_action)
{
	s32 r = -EINVAL;
	u8 i;
	u16 start, end;
	u16 utype;
	u8 trace_action_val_p = trace_action;

	for (i = 0u; i < IRQ_GLOBAL_EVENT_TYPES_ID_MAX; i++) {
		start = irq_global_event_types[i].start;
		end = irq_global_event_types[i].start +
		      irq_global_event_types[i].num;
		utype = irq_global_event_types[i].utype;

		if ((global_evt >= start) && (global_evt < end)) {
			r = rm_core_resasg_validate_resource(host, utype,
							     global_evt);
			break;
		}
	}

	if (r != SUCCESS) {
		trace_action_val_p |= TRACE_RM_ACTION_FAIL;
	}

	rm_trace_sub(trace_action_val_p,
		     TRACE_RM_SUB_ACTION_IRQ_GLOBAL_EVENT,
		     global_evt);

	return r;
}

/**
 * \brief Trace out untraced interrupt route parameters based on the valid
 *        parameter settings.
 *
 * \param cfg IRQ route configuration
 *
 * \param trace_action RA action to trace
 */
#ifdef CONFIG_TRACE
static void irq_trace_remaining_params(struct irq_cfg *cfg, u8 trace_action)
#else
static void irq_trace_remaining_params(struct irq_cfg *cfg, u8 trace_action __attribute__((unused)))
#endif
{
	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_VALID_PARAM_HI,
		     ((cfg->valid_params >> 16U) & TRACE_DEBUG_SUB_ACTION_VAL_MASK));
	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_VALID_PARAM_LO,
		     (cfg->valid_params & TRACE_DEBUG_SUB_ACTION_VAL_MASK));

	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_IRQ_SRC_ID,
		     cfg->s_id);
	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_IRQ_SRC_INDEX,
		     cfg->s_idx);

	if (rm_core_param_is_valid(cfg->valid_params,
				   TISCI_MSG_VALUE_RM_DST_ID_VALID) == STRUE) {
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IRQ_DST_ID,
			     cfg->d_id);
	}
	if (rm_core_param_is_valid(cfg->valid_params,
				   TISCI_MSG_VALUE_RM_VINT_VALID) == STRUE) {
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IRQ_VINT,
			     cfg->vint);
	}
	if (rm_core_param_is_valid(cfg->valid_params,
				   TISCI_MSG_VALUE_RM_VINT_STATUS_BIT_INDEX_VALID) == STRUE) {
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IRQ_VINT_STATUS_BIT_INDEX,
			     cfg->vint_sb);
	}
}

/**
 * \brief Configures an interrupt router input to output MUX based on cfg info
 *
 * \param cfg IRQ configuration
 *
 * \return SUCCESS if IR input to output mapping configured, else error
 */
static s32 irq_ir_cfg_mux(struct irq_cfg *cfg)
{
	s32 r;

	if (cfg->s_id != cfg->d_id) {
		/*
		 * Source and destination IDs must be the same when
		 * programming the IR
		 */
		r = -EINVAL;
	} else {
		r = rm_ir_configure(cfg->d_id, cfg->s_idx, cfg->d_irq, STRUE);
	}

	return r;
}

/**
 * \brief Checks if the IRQ source device is a global event source
 *
 * \param id IRQ source device ID
 *
 * \return STRUE if source device can source global events, else SFALSE
 */
static sbool irq_is_evt_src(u16 id)
{
	sbool evt_src = SFALSE;
	u8 i;

	for (i = 0u; i < evt_rt_srcs_count; i++) {
		if (id == evt_rt_srcs[i]) {
			evt_src = STRUE;
			break;
		}
	}

	return evt_src;
}

/**
 * \brief Maps an event to an IA virtual interrupt and programs the event
 *        into the source peripheral IRQ's OES register
 *
 * \param cfg Pointer to the IA configuration
 *
 * \return SUCCESS if event mapping to VINT is successful, else error
 */
static s32 irq_vint_map(struct irq_cfg *cfg)
{
	s32 r = SUCCESS;
	u16 oes_evt;

	if (irq_is_evt_src(cfg->s_id) == STRUE) {
		cfg->oes_src = irq_oes_src_lookup(cfg->s_id);

		if (cfg->oes_src != NULL) {
			r = cfg->oes_src->oes_get(cfg->host, cfg->s_id,
						  cfg->s_idx, &oes_evt);
			if ((r == SUCCESS) && (oes_evt != IRQ_OES_RESETVAL)) {
				/* Event source in use already */
				r = -EINVAL;
			}
		}
	} else {
		r = -EINVAL;
	}

	if (r == SUCCESS) {
		r = rm_ia_vint_map(cfg->s_ia, cfg->vint, cfg->global_evt,
				   cfg->vint_sb, SFALSE, STRUE);
	}

	if ((r == SUCCESS) && (cfg->oes_src != NULL)) {
		/*
		 * Program device IRQ's OES register.  Not a failure
		 * if device has no OES handlers registered.  Just let
		 * user program OES in this case.
		 */
		if (cfg->oes_src->oes_set(cfg->host, cfg->s_id,
					  cfg->s_idx,
					  cfg->global_evt) !=
		    SUCCESS) {
			/*
			 * Corruption occurred.  OES register set
			 * should not fail after the register was
			 * already verified as available.
			 */
			r = -ECONFUSED;
		}
	}

	return r;
}

/**
 * \brief Program a global event value into a peripheral's OES register
 *
 * \param cfg Pointer to the IRQ route configuration.  In this case, just the
 *            OES configuration.
 *
 * \return SUCCESS if OES register programming is successful, else error
 */
static s32 irq_program_oes_register(struct irq_cfg *cfg)
{
	s32 r = SUCCESS;
	u16 oes_evt;

	if (irq_is_evt_src(cfg->s_id) == STRUE) {
		cfg->oes_src = irq_oes_src_lookup(cfg->s_id);

		if (cfg->oes_src != NULL) {
			r = cfg->oes_src->oes_get(cfg->host, cfg->s_id,
						  cfg->s_idx, &oes_evt);
			if ((r == SUCCESS) && (oes_evt != IRQ_OES_RESETVAL)) {
				/* Event source in use already */
				r = -EINVAL;
			}
		}
	} else {
		r = -EINVAL;
	}

	if ((r == SUCCESS) && (cfg->oes_src != NULL)) {
		/* Program device's OES register. */
		if (cfg->oes_src->oes_set(cfg->host, cfg->s_id,
					  cfg->s_idx,
					  cfg->global_evt) !=
		    SUCCESS) {
			/*
			 * Corruption occurred.  OES register set
			 * should not fail after the register was
			 * already verified as available.
			 */
			r = -ECONFUSED;
		}
	}

	return r;
}

/**
 * \brief Clears an interrupt router input to output MUX based on cfg info
 *
 * \param cfg IRQ configuration
 *
 * \return SUCCESS if IR input to output mapping cleared, else error
 */
static s32 irq_ir_clear_mux(struct irq_cfg *cfg)
{
	s32 r;
	u16 loc_outp;

	if (cfg->s_id != cfg->d_id) {
		/*
		 * Source and destination IDs must be the same when
		 * programming the IR
		 */
		r = -EINVAL;
	} else {
		r = rm_ir_get_outp(cfg->d_id, cfg->s_idx, &loc_outp);

		if ((r == SUCCESS) && (loc_outp == cfg->d_irq)) {
			r = rm_ir_clear(cfg->d_id, cfg->s_idx, cfg->d_irq);
		} else {
			r = -EINVAL;
		}
	}

	return r;
}

/**
 * \brief Deletes an event from the specified virtual interrupt and removes the
 *        event from the source peripheral IRQ's OES register
 *
 * \param cfg Pointer to the IRQ route configuration
 *
 * \return SUCCESS if removal of VINT is successful, else error
 */
static s32 irq_vint_unmap(struct irq_cfg *cfg)
{
	s32 r = SUCCESS;
	u8 num_evts;
	u16 oes_evt = IRQ_OES_RESETVAL;

	if (irq_is_evt_src(cfg->s_id) == STRUE) {
		cfg->oes_src = irq_oes_src_lookup(cfg->s_id);

		if (cfg->oes_src != NULL) {
			r = cfg->oes_src->oes_get(cfg->host, cfg->s_id,
						  cfg->s_idx, &oes_evt);
			if ((r == SUCCESS) && (oes_evt != cfg->global_evt)) {
				/*
				 * Event source does not match the
				 * global event supplied by user
				 */
				r = -EINVAL;
			}
		}
	} else {
		r = -EINVAL;
	}

	if (r == SUCCESS) {
		r = rm_ia_vint_get_info(cfg->s_ia, cfg->vint, &num_evts);
	}

	if ((r == SUCCESS) && (num_evts > 0U)) {
		if (cfg->oes_src != NULL) {
			/* Reset source peripheral's OES register. */
			r = cfg->oes_src->oes_set(cfg->host, cfg->s_id,
						  cfg->s_idx, IRQ_OES_RESETVAL);
		}

		if (r == SUCCESS) {
			r = rm_ia_vint_unmap(cfg->s_ia, cfg->vint,
					     cfg->global_evt, cfg->vint_sb);

			if ((r != SUCCESS) && (cfg->oes_src != NULL)) {
				/*
				 * Corruption occurred.  VINT event unmap
				 * should not fail after OES register was
				 * cleared successfully.
				 */
				r = -ECONFUSED;
			}
		}
	} else {
		/* No events to unmap */
		r = -EINVAL;
	}

	return r;
}

/**
 * \brief Clear global event value from a peripheral's OES register
 *
 * \param cfg Pointer to the IRQ route configuration.  In this case, just the
 *            OES configuration.
 *
 * \return SUCCESS if OES register clear is successful, else error
 */
static s32 irq_clear_oes_register(struct irq_cfg *cfg)
{
	s32 r = SUCCESS;
	u16 oes_evt = IRQ_OES_RESETVAL;

	if (irq_is_evt_src(cfg->s_id) == STRUE) {
		cfg->oes_src = irq_oes_src_lookup(cfg->s_id);

		if (cfg->oes_src != NULL) {
			r = cfg->oes_src->oes_get(cfg->host, cfg->s_id,
						  cfg->s_idx, &oes_evt);
			if ((r == SUCCESS) && (oes_evt != cfg->global_evt)) {
				/*
				 * Event source does not match the
				 * global event supplied by user
				 */
				r = -EINVAL;
			}
		}
	} else {
		r = -EINVAL;
	}

	if ((r == SUCCESS) && (cfg->oes_src != NULL)) {
		/* Reset source peripheral's OES register. */
		r = cfg->oes_src->oes_set(cfg->host, cfg->s_id,
					  cfg->s_idx, IRQ_OES_RESETVAL);
	}

	return r;
}

s32 rm_irq_oes_src_register(u16 id, struct rm_irq_oes_src *src)
{
	sbool duplicate = SFALSE;
	struct map *map_root;
	struct map_node *n;
	s32 r = SUCCESS;

	if (irq_is_evt_src(id) == STRUE) {
		/* Accept duplicate registrations for same OES src */
		n = u32map_get(oes_src_map, id);
		if ((n != NULL) && (n == &src->node)) {
			duplicate = STRUE;
		}

		if (duplicate != STRUE) {
			map_root = u32map_add(oes_src_map, id, &src->node);
			if (!map_root) {
				r = -EBUSY;
			}

			oes_src_map = map_root;
		}
	}

	return r;
}

s32 rm_irq_set(u32 *msg_recv)
{
	s32 r = SUCCESS;
	struct tisci_msg_rm_irq_set_req *msg =
		(struct tisci_msg_rm_irq_set_req *) msg_recv;
	u8 dst_host;
	struct irq_cfg cfg;
	u8 trace_action = TRACE_RM_ACTION_IRQ_SET;

	memset(&cfg, 0, sizeof(cfg));

	if (rm_core_param_is_valid(msg->valid_params,
				   TISCI_MSG_VALUE_RM_SECONDARY_HOST_VALID) ==
	    STRUE) {
		r = osal_core_verify_host_id(msg->secondary_host);
		if (r != SUCCESS) {
			trace_action |= TRACE_RM_ACTION_FAIL;
		} else {
			dst_host = msg->secondary_host;
		}
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IRQ_SECONDARY_HOST,
			     msg->secondary_host);
	} else {
		dst_host = msg->hdr.host;
	}

	if ((r == SUCCESS) &&
	    (rm_core_param_is_valid(msg->valid_params,
				    TISCI_MSG_VALUE_RM_GLOBAL_EVENT_VALID) ==
	     STRUE)) {
		r = irq_validate_global_event(dst_host, msg->global_event,
					      trace_action);
	}

	if ((r == SUCCESS) &&
	    (rm_core_param_is_valid(msg->valid_params,
				    TISCI_MSG_VALUE_RM_VINT_VALID) ==
	     STRUE)) {
		r = rm_ia_validate_vint(dst_host, msg->ia_id, msg->vint);
	}

	if ((r == SUCCESS) &&
	    (rm_core_param_is_valid(msg->valid_params,
				    TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID) ==
	     STRUE)) {
		r = rm_ir_validate_output(dst_host, msg->dst_id,
					  msg->dst_host_irq);
	}

	if (r == SUCCESS) {
		cfg.valid_params = msg->valid_params;
		cfg.host = dst_host;
		cfg.s_id = msg->src_id;
		cfg.s_idx = msg->src_index;
		cfg.d_id = msg->dst_id;
		cfg.d_irq = msg->dst_host_irq;
		cfg.global_evt = msg->global_event;
		cfg.s_ia = msg->ia_id;
		cfg.vint = msg->vint;
		cfg.vint_sb = msg->vint_status_bit_index;

		if (irq_cfg_is_ir_cfg(cfg.valid_params) == STRUE) {
			r = irq_ir_cfg_mux(&cfg);
		} else if (irq_cfg_is_ia_cfg(cfg.valid_params) ==
			   STRUE) {
			r = irq_vint_map(&cfg);
		} else if (irq_cfg_is_oes_only(cfg.valid_params) == STRUE) {
			/*
			 * Just program OES register.  No IA associated with
			 * event.  For example, a DMA trigger event
			 */
			r = irq_program_oes_register(&cfg);
		} else {
			r = -EINVAL;
		}

		if (r != SUCCESS) {
			trace_action |= TRACE_RM_ACTION_FAIL;
		}
		irq_trace_remaining_params(&cfg, trace_action);
	}

	return r;
}

s32 rm_irq_release(u32 *msg_recv)
{
	s32 r = SUCCESS;
	struct tisci_msg_rm_irq_release_req *msg =
		(struct tisci_msg_rm_irq_release_req *) msg_recv;
	u8 dst_host;
	struct irq_cfg cfg;
	u8 trace_action = TRACE_RM_ACTION_IRQ_RELEASE;

	memset(&cfg, 0, sizeof(cfg));

	if (rm_core_param_is_valid(msg->valid_params,
				   TISCI_MSG_VALUE_RM_SECONDARY_HOST_VALID) ==
	    STRUE) {
		r = osal_core_verify_host_id(msg->secondary_host);
		if (r != SUCCESS) {
			trace_action |= TRACE_RM_ACTION_FAIL;
		} else {
			dst_host = msg->secondary_host;
		}
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_IRQ_SECONDARY_HOST,
			     msg->secondary_host);
	} else {
		dst_host = msg->hdr.host;
	}

	if ((r == SUCCESS) &&
	    (rm_core_param_is_valid(msg->valid_params,
				    TISCI_MSG_VALUE_RM_VINT_VALID) ==
	     STRUE)) {
		r = rm_ia_validate_vint(dst_host, msg->ia_id, msg->vint);
	}

	if ((r == SUCCESS) &&
	    (rm_core_param_is_valid(msg->valid_params,
				    TISCI_MSG_VALUE_RM_GLOBAL_EVENT_VALID) ==
	     STRUE)) {
		r = irq_validate_global_event(dst_host, msg->global_event,
					      trace_action);
	}

	if ((r == SUCCESS) &&
	    (rm_core_param_is_valid(msg->valid_params,
				    TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID) ==
	     STRUE)) {
		r = rm_ir_validate_output(dst_host, msg->dst_id,
					  msg->dst_host_irq);
	}

	if (r == SUCCESS) {
		cfg.valid_params = msg->valid_params;
		cfg.host = dst_host;
		cfg.s_id = msg->src_id;
		cfg.s_idx = msg->src_index;
		cfg.d_id = msg->dst_id;
		cfg.d_irq = msg->dst_host_irq;
		cfg.global_evt = msg->global_event;
		cfg.s_ia = msg->ia_id;
		cfg.vint = msg->vint;
		cfg.vint_sb = msg->vint_status_bit_index;

		if (irq_cfg_is_ir_cfg(cfg.valid_params) == STRUE) {
			r = irq_ir_clear_mux(&cfg);
		} else if (irq_cfg_is_ia_cfg(cfg.valid_params) == STRUE) {
			r = irq_vint_unmap(&cfg);
		} else if (irq_cfg_is_oes_only(cfg.valid_params) == STRUE) {
			/*
			 * Just reset OES register.  No interrupt route
			 * associated with event.  For example, a UDMAP
			 * trigger event
			 */
			r = irq_clear_oes_register(&cfg);
		} else {
			r = -EINVAL;
		}

		if (r != SUCCESS) {
			trace_action |= TRACE_RM_ACTION_FAIL;
		}
		irq_trace_remaining_params(&cfg, trace_action);
	}

	return r;
}

sbool rm_irq_is_managed_resasg_utype(u16 utype)
{
	sbool r;
	u16 i;

	r = rm_ir_is_managed_resasg_utype(utype);

	if (r == SFALSE) {
		r = rm_ia_is_managed_resasg_utype(utype);
	}

	if (r == SFALSE) {
		for (i = 0u; i < IRQ_GLOBAL_EVENT_TYPES_ID_MAX; i++) {
			if (utype == irq_global_event_types[i].utype) {
				r = STRUE;
				break;
			}
		}
	}

	return r;
}

s32 rm_irq_init(void)
{
	s32 r = SUCCESS;

	r = rm_ir_init();
	if (r == SUCCESS) {
		r = rm_ia_init();
	}

	if (r != SUCCESS) {
		rm_trace_sub((TRACE_RM_ACTION_IRQ_INIT |
			      TRACE_RM_ACTION_FAIL), 0U, 0U);
	}

	return r;
}

s32 rm_irq_deinit(devgrp_t devgrp, sbool rom_deinit)
{
	s32 r = SUCCESS;

	r = rm_ir_deinit(devgrp, rom_deinit);

	if (r == SUCCESS) {
		r = rm_ia_deinit(devgrp, rom_deinit);
	}

	return r;
}
