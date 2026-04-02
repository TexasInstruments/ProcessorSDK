/*
 * Proxy
 *
 * proxy management infrastructure
 *
 * Copyright (C) 2019-2023, Texas Instruments Incorporated
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
#include <types/address_types.h>
#include <types/errno.h>
#include <lib/io.h>
#include <lib/trace.h>

#include <config.h>
#include <hosts.h>

#include <tisci/rm/tisci_rm_proxy.h>
#include <rm.h>

#include <rm_core.h>
#include <rm_proxy.h>

#include <proxy_inst.h>
#include <proxy_cfg.h>

#ifdef CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS
#include <security/rm_int_firewall.h>
#include <tifs/src/security/secure_rm/sec_rm.h>
#endif

/*
 * Proxy Configuration Register Macros
 */
#define PROXY_CFG_PROXY_EVT_REG                    (0x00000000U)
#define PROXY_CFG_MULTIPLIER                       (0x00001000U)

/*
 * Proxy Register Fields
 */
#define PROXY_EVENT_MASK                           (0x0000FFFFU)
#define PROXY_EVENT_SHIFT                          (0x00000000U)
#define PROXY_EVENT_RESETVAL                       (0x0000FFFFU)

/**
 * \brief Get proxy instance mapped to provided device ID
 *
 * \param id proxy device ID
 *
 * \param trace_action proxy action to trace
 *
 * \return Pointer to proxy instance, NULL if no instance mapped to ID
 */
static const struct proxy_instance *proxy_get_inst(u16 id, u8 trace_action)
{
	const struct proxy_instance *inst = NULL;
	u32 i;
	/* Local version to satisfy MISRA */
	u8 local_trace_action = trace_action;

	for (i = 0U; i < PROXY_INST_COUNT; i++) {
		if (proxy_inst[i].id == id) {
			inst = &proxy_inst[i];

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

	if (inst == NULL) {
		local_trace_action |= TRACE_RM_ACTION_FAIL;
	}

	rm_trace_sub(local_trace_action,
		     TRACE_RM_SUB_ACTION_DEVICE_ID,
		     id);

	return inst;
}

/**
 * \brief Get proxy cfg region address offset
 *
 * \param index Proxy index
 *
 * \return Offset of proxy cfg region
 */
static u32 proxy_get_cfg_offset(u16 index)
{
	return (u32) index * PROXY_CFG_MULTIPLIER;
}

/**
 * \brief Validate proxy index against the board cfg resasg data
 *
 * \param inst proxy instance
 *
 * \param host Host ID
 *
 * \param index Proxy index
 *
 * \param utype Returns the proxy utype if not NULL
 *
 * \param trace_action proxy action to trace
 *
 * \return SUCCESS if proxy index is valid, else -EINVAL
 */
static s32 proxy_check_index_range(
	const struct proxy_instance	*inst,
	u8				host,
	u16				index,
	u16				*utype,
	u8				trace_action)
{
	s32 r = -EINVAL;
	/* Local version to satisfy MISRA */
	u8 local_trace_action = trace_action;

	if (index < inst->n_proxies) {
		r = rm_core_resasg_validate_resource(
			host,
			inst->proxy_utype,
			index);
	}

	if (r != SUCCESS) {
		local_trace_action |= TRACE_RM_ACTION_FAIL;
	} else {
		if (utype != NULL) {
			*utype = inst->proxy_utype;
		}
	}

	rm_trace_sub(local_trace_action,
		     TRACE_RM_SUB_ACTION_INDEX,
		     index);

	return r;
}

/**
 * \brief Gets the global event in a proxy event register
 *
 * \param inst proxy instance
 *
 * \param oes_index OES event index
 *
 * \return Global event value within the OES index's event register
 */
static u16 proxy_get_evt(const struct proxy_instance *inst, u16 oes_index)
{
	mapped_addr_t maddr;
	u32 evt_addr;
	u32 evt_reg;

	maddr = rm_core_map_region(inst->cfg->base);
	evt_addr = maddr + proxy_get_cfg_offset(oes_index) +
		   PROXY_CFG_PROXY_EVT_REG;

	evt_reg = readl(evt_addr);
	rm_core_unmap_region();

	return (u16) rm_fext(evt_reg, PROXY_EVENT_SHIFT, PROXY_EVENT_MASK);
}

/**
 * \brief Gets the global event within a proxy's OES register
 *
 * \param host Host ID of processing entity attempting to get the event.
 *
 * \param id SoC device ID in which the proxy OES index resides
 *
 * \param oes_index Flattened OES register index.
 *
 * \param evt Returns the global event value within the OES index's OES register
 *
 * \return SUCCESS if global event retrieved successfully, else error
 */
static s32 proxy_get_ring_evt(u8 host, u16 id, u16 oes_index, u16 *evt)
{
	s32 r = SUCCESS;
	const struct proxy_instance *inst = NULL;
	u16 utype;
	u8 trace_action = TRACE_RM_ACTION_PROXY_OES_GET;

	if (evt == NULL) {
		r = -EINVAL;
	} else {
		inst = proxy_get_inst(id, trace_action);
		if (inst == NULL) {
			r = -EINVAL;
		}
	}

	if (r == SUCCESS) {
		if (oes_index < inst->n_proxies) {
			r = proxy_check_index_range(inst, host,
						    oes_index, &utype,
						    trace_action);
		} else {
			trace_action |= TRACE_RM_ACTION_FAIL;
			r = -EINVAL;
		}

		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_OES_REG_INDEX,
			     oes_index);
	}

	if (r == SUCCESS) {
		/*
		 * This will result in access exception if IRQ with proxy
		 * as OES source is configured prior to the proxy being
		 * configured.  This is because the channelized firewall
		 * covering the proxy OES register isn't set to RW for
		 * RM core until proxy_cfg message is sent
		 */
		*evt = proxy_get_evt(inst, oes_index);
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_EVENT,
			     *evt);
	}

	return r;
}

/**
 * \brief Sets the global event within a proxy's OES register
 *
 * \param host Host ID of processing entity attempting to set the event.
 *
 * \param id SoC device ID in which the proxy OES index resides
 *
 * \param oes_index Flattened OES register index.
 *
 * \param evt Global event value to program into the OES index's register
 *
 * \return SUCCESS if global event programmed successfully, else error
 */
static s32 proxy_set_ring_evt(u8 host, u16 id, u16 oes_index, u16 evt)
{
	s32 r = SUCCESS;
	const struct proxy_instance *inst = NULL;
	mapped_addr_t maddr;
	u16 utype;
	u32 evt_reg;
	u32 evt_addr;
	u8 trace_action = TRACE_RM_ACTION_PROXY_OES_SET;

	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_EVENT,
		     evt);

	inst = proxy_get_inst(id, trace_action);
	if (inst == NULL) {
		r = -EINVAL;
	}

	if (r == SUCCESS) {
		if (oes_index < inst->n_proxies) {
			r = proxy_check_index_range(inst, host,
						    oes_index, &utype,
						    trace_action);
		} else {
			trace_action |= TRACE_RM_ACTION_FAIL;
			r = -EINVAL;
		}

		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_OES_REG_INDEX,
			     oes_index);
	}

	if (r == SUCCESS) {
		/*
		 * This will result in access exception if IRQ with proxy
		 * as OES source is configured prior to the proxy being
		 * configured.  This is because the channelized firewall
		 * covering the proxy OES register isn't set to RW for
		 * RM core until proxy_cfg message is sent
		 */

		/* oes_index verified already at this point */
		maddr = rm_core_map_region(inst->cfg->base);
		evt_reg = rm_fmk(PROXY_EVENT_SHIFT,
				 PROXY_EVENT_MASK, evt);
		evt_addr = maddr + proxy_get_cfg_offset(oes_index) +
			   PROXY_CFG_PROXY_EVT_REG;

		if (writel_verified(evt_reg, evt_addr) != SUCCESS) {
			/* Readback of write failed: halt */
			r = -EFAILVERIFY;
		}
		rm_core_unmap_region();
	}

	return r;
}

s32 rm_proxy_cfg(u32 *msg_recv)
{
	s32 r = SUCCESS;
	struct tisci_msg_rm_proxy_cfg_req *msg =
		(struct tisci_msg_rm_proxy_cfg_req *) msg_recv;
	const struct proxy_instance *inst = NULL;
	u16 utype;
	u8 trace_action = TRACE_RM_ACTION_PROXY_CFG;

#ifdef CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS
	u8 hosts[FWL_MAX_PRIVID_SLOTS];
	u8 n_hosts = 0U;
#endif

	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_VALID_PARAM_HI,
		     ((msg->valid_params >> 16U) & TRACE_DEBUG_SUB_ACTION_VAL_MASK));
	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_VALID_PARAM_LO,
		     (msg->valid_params & TRACE_DEBUG_SUB_ACTION_VAL_MASK));

	inst = proxy_get_inst(msg->nav_id, trace_action);
	if (inst == NULL) {
		r = -EINVAL;
	}

	if (r == SUCCESS) {
		r = proxy_check_index_range(inst, msg->hdr.host,
					    msg->index, &utype,
					    trace_action);
	}

#ifdef CONFIG_RM_LOCAL_SUBSYSTEM_REQUESTS
	if (r == SUCCESS) {
		/* Call Secure RM to configure proxy firewalls */

		r = rm_core_get_resasg_hosts(utype,
					     msg->index,
					     &n_hosts,
					     &hosts[0U],
					     FWL_MAX_PRIVID_SLOTS);

		if (r == SUCCESS) {
			r = sec_rm_proxy_fwl_cfg(msg_recv, hosts, n_hosts);
		}
	}
#endif

	return r;
}

sbool rm_proxy_is_managed_resasg_utype(u16 utype)
{
	sbool r = SFALSE;
	u16 i;

	for (i = 0U; i < PROXY_INST_COUNT; i++) {
		if (utype == proxy_inst[i].proxy_utype) {
			r = STRUE;
			break;
		}
	}

	return r;
}

s32 rm_proxy_init(void)
{
	s32 r = SUCCESS;
	u16 i;

	for (i = 0U; i < PROXY_INST_COUNT; i++) {
		if ((rm_core_validate_devgrp(proxy_inst[i].id,
					     proxy_inst[i].devgrp) ==
		     SUCCESS) &&
		    (proxy_inst[i].initialized == SFALSE)) {
#ifdef CONFIG_RM_IRQ
			if (r == SUCCESS) {
				proxy_inst[i].oes_handler.oes_get = proxy_get_ring_evt;
				proxy_inst[i].oes_handler.oes_set = proxy_set_ring_evt;

				r = rm_irq_oes_src_register(proxy_inst[i].id,
							    &proxy_inst[i].oes_handler);
			}
#endif

			if (r != SUCCESS) {
				break;
			} else {
				proxy_inst[i].initialized = STRUE;
			}
		}
	}

	if (r != SUCCESS) {
		rm_trace_sub((u32) (TRACE_RM_ACTION_PROXY_INIT |
				    TRACE_RM_ACTION_FAIL), 0U, 0U);
	}

	return r;
}

s32 rm_proxy_deinit(devgrp_t devgrp)
{
	s32 r = -EFAIL;
	u8 i;

	for (i = 0U; i < PROXY_INST_COUNT; i++) {
		if ((rm_core_validate_devgrp(proxy_inst[i].id, proxy_inst[i].devgrp) ==
		     SUCCESS) &&
		    (proxy_inst[i].initialized == STRUE) &&
		    (proxy_inst[i].devgrp == devgrp)) {
			proxy_inst[i].initialized = SFALSE;
			r = SUCCESS;
		}
	}

	return r;
}
