/*
 * Non Secure PSIL
 *
 * Non Secure PSIL management infrastructure
 *
 * Copyright (C) 2022, Texas Instruments Incorporated
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
#include <hosts_internal.h>

#include <config.h>

#include <osal/osal_core.h>
#include <tisci/rm/tisci_rm_psil.h>

#include <rm_core.h>
#include <rm_psil.h>
#include <rm_udmap.h>

#include <tifs/src/security/secure_rm/sec_rm.h>

/**
 * \brief Verify the host ID of a message matches the host ID of the resource in the boardcfg
 *
 * \param msg_recv Pointer to the message
 *
 * \param source_thread A boolean to use source or destination threads
 *
 * \param pairing A boolean to use the PAIR/UNPAIR actions, or READ/WRITE. Used with "primary"
 *
 * \param primary A boolean to use PAIR/READ or UNPAIR/WRITE. Used with "pairing"
 *
 * \return -EINVAL if an error occurs, -ENODEV if it's an IP thread, or SUCCESS if it's a valid host ID for a DMA thread
 */
static s32 rm_psil_verify_host(u32 *msg_recv, sbool source_thread, sbool pairing, sbool primary)
{
	struct tisci_header *hdr = (struct tisci_header *) msg_recv;
	s32 r = SUCCESS;
	s32 dev_id;
	s32 index;
	u8 host_array_max = RES_MAX_HOST_CNT;
	u8 hosts[RES_MAX_HOST_CNT];
	u8 n_hosts = 0U;
	u32 trace_action = TRACE_RM_ACTION_PSIL_HOST_ID_CHECK;


	/* Get the device ID from the secure PSIL driver */
	dev_id = sec_rm_psil_get_dev_id(msg_recv, source_thread, pairing, primary);

	/* Device ID may be invalid, or not a DMA device */
	if ((dev_id == -EINVAL) || (dev_id == -ENODEV)) {
		r = dev_id;
	}

	if (r == SUCCESS) {
		/* Now get the index */
		index = sec_rm_psil_get_index(msg_recv, source_thread, pairing, primary);

		/* Index may be invalid, or not from a DMA device */
		if ((index == -EINVAL) || (index == -ENODEV)) {
			r = index;
		}
	}

	/*
	 * Get the host ID(s) corresponding to
	 * the resource in the boardcfg
	 */
	if (r == SUCCESS) {
		r = udmap_get_host((u16) dev_id, (u16) index, source_thread, &n_hosts, &hosts[0], host_array_max);
	}

	/*
	 * Check if the host in the message matches
	 * the host that owns that resource
	 */
	if (r == SUCCESS) {
		/*
		 * Besides the cases below,
		 * all other cases are invalid
		 */
		r = -EINVAL;

		if (n_hosts == 1) {
			if ((hosts[0] == hdr->host) || (hosts[0] == HOST_ID_ALL)) {
				r = SUCCESS;
			}
		} else if (n_hosts == 2) {
			if ((hosts[0] == hdr->host) || (hosts[0] == HOST_ID_ALL)) {
				r = SUCCESS;
			} else if ((hosts[1] == hdr->host) || (hosts[1] == HOST_ID_ALL)) {
				r = SUCCESS;
			}
		}
	}

	if (r == -EINVAL) {
		trace_action |= TRACE_RM_ACTION_FAIL;
	}

	rm_trace_sub(trace_action,
		     TRACE_RM_SUB_ACTION_PSIL_MSG_HOST_ID,
		     hdr->host);

	if (n_hosts == 1) {
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_PSIL_HOST_ID_BCFG,
			     hosts[0]);
	} else if (n_hosts == 2) {
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_PSIL_HOST_ID_BCFG,
			     hosts[0]);
		rm_trace_sub(trace_action,
			     TRACE_RM_SUB_ACTION_PSIL_HOST_ID_BCFG,
			     hosts[1]);
	}

	return r;
}

/**
 * \brief Verify both the source thread and the destination thread are valid
 *
 * \param msg_recv Pointer to the message
 *
 * \param pairing A boolean to use the PAIR/UNPAIR actions, or READ/WRITE. Used with "primary"
 *
 * \param primary A boolean to use PAIR/READ or UNPAIR/WRITE. Used with "pairing"
 *
 * \return -EINVAL for an invalid thread or host ID, or SUCCESS if both threads are valid
 */
static s32 rm_psil_verify_src_dst_host(u32 *msg_recv, sbool pairing, sbool primary)
{
	s32 r = SUCCESS;
	sbool source_thread;

	/* Verify as a source thread */
	source_thread = STRUE;

	r = rm_psil_verify_host(msg_recv, source_thread, pairing, primary);

	/*
	 * Depending on the status,
	 * the source thread is the following:
	 *
	 * SUCCESS: A DMA thread with a valid host ID
	 *
	 * -ENODEV: An IP thread (host ID not checked)
	 *
	 * -EINVAL: An invalid thread or host ID
	 */
	if (r != -EINVAL) {
		/* Verify as a destination thread */
		source_thread = SFALSE;

		r = rm_psil_verify_host(msg_recv, source_thread, pairing, primary);

		if (r == -ENODEV) {
			/*
			 * Several cases are handled here:
			 *
			 * 1. This means both source and destination threads
			 *    are IP threads. Unable to check host IDs of
			 *    an IP to IP connection, so return SUCCESS
			 *
			 * 2. The source thread must've been a DMA thread with
			 *    a valid host ID to get here, so return SUCCESS
			 *    as this is a DMA to IP connection
			 */

			r = SUCCESS;
		}
	}

	return r;
}

/**
 * \brief Verify if either the source thread or the destination thread are valid
 *
 * \param msg_recv Pointer to the message
 *
 * \param pairing A boolean to use the PAIR/UNPAIR actions, or READ/WRITE. Used with "primary"
 *
 * \param primary A boolean to use PAIR/READ or UNPAIR/WRITE. Used with "pairing"
 *
 * \return -EINVAL for an invalid thread or host ID, or SUCCESS if one of the threads are valid
 */
static s32 rm_psil_verify_single_host(u32 *msg_recv, sbool pairing, sbool primary)
{
	s32 r = SUCCESS;
	sbool source_thread;

	/*
	 * Thread type is unknown, so see if it's a source thread.
	 * But if that fails, try again as a destination thread.
	 */

	/* Check as a source thread */
	source_thread = STRUE;

	r = rm_psil_verify_host(msg_recv, source_thread, pairing, primary);

	/*
	 * Depending on the status,
	 * the thread is the following:
	 *
	 * SUCCESS: A DMA thread with a valid host ID
	 *
	 * -ENODEV: An IP thread (host ID not checked)
	 *
	 * -EINVAL: An invalid thread or host ID, or not a source thread
	 */
	if (r == -EINVAL) {
		/* Check as a destination thread */
		source_thread = SFALSE;

		r = rm_psil_verify_host(msg_recv, source_thread, pairing, primary);

		/*
		 * Just like earlier, depending
		 * on the status, the thread is
		 * the following:
		 *
		 * SUCCESS: A DMA thread with a valid host ID
		 *
		 * -ENODEV: An IP thread (host ID not checked)
		 *
		 * -EINVAL: An invalid thread or host ID
		 */
	}

	/*
	 * This status means it's an IP thread. So return SUCCESS
	 * since we are unable to check the host ID
	 */
	if (r == -ENODEV) {
		r = SUCCESS;
	}

	return r;
}

s32 rm_psil_pair(u32 *msg_recv)
{
	s32 r;

	r = rm_psil_verify_src_dst_host(msg_recv, STRUE, STRUE);

	/* Send message to secure RM */
	if (r == SUCCESS) {
		r = sec_rm_psil_pair(msg_recv);
	}

	return r;
}

s32 rm_psil_unpair(u32 *msg_recv)
{
	s32 r;

	r = rm_psil_verify_src_dst_host(msg_recv, STRUE, SFALSE);

	/* Send message to secure RM */
	if (r == SUCCESS) {
		r = sec_rm_psil_unpair(msg_recv);
	}

	return r;
}

s32 rm_psil_read(u32 *msg_recv, u32 *msg_resp)
{
	s32 r;

	r = rm_psil_verify_single_host(msg_recv, SFALSE, STRUE);

	/* Send message to secure RM */
	if (r == SUCCESS) {
		r = sec_rm_psil_read(msg_recv, msg_resp);
	}

	return r;
}

s32 rm_psil_write(u32 *msg_recv)
{
	s32 r;

	r = rm_psil_verify_single_host(msg_recv, SFALSE, SFALSE);

	/* Send message to secure RM */
	if (r == SUCCESS) {
		r = sec_rm_psil_write(msg_recv);
	}

	return r;
}
