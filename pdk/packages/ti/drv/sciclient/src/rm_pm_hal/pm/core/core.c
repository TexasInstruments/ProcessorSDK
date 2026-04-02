/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2020-2021, Texas Instruments Incorporated
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
#include <lib/str.h>
#include <lib/strncpy.h>
#include <lib/mmr_lock.h>
#include <osal/osal_core.h>
#include <sleep.h>
#include <startup.h>
#include <tisci/tisci_protocol.h>
#include <tisci/pm/tisci_pm_core.h>
#include <types/errno.h>
#include <wake.h>
#include <osal/osal_clock_user.h>
#include <sys-reset.h>
/* This header is in awkward place, but moving it breaks sysfw build */
#include <../pm/pm.h>

s32 wake_reason_handler(u32 *msg_recv)
{
	struct tisci_msg_wake_reason_resp *resp =
		(struct tisci_msg_wake_reason_resp *) msg_recv;
	const struct sleep_mode *mode;
	const char *reason = "unk";
	u32 time_ms;
	s32 ret = SUCCESS;

	mmr_unlock_all();
	time_ms = 0;
	mode = wake_deepest_mode();

	resp->hdr.flags = 0U;


	resp->time_ms = time_ms;
	if (mode != NULL) {
		lib_strncpy(resp->mode, mode->name, sizeof(resp->mode));
	}
	lib_strncpy(resp->reason, reason, sizeof(resp->reason));

	mmr_lock_all();

	return ret;
}

s32 wake_reset_handler(u32 *msg_recv)
{
	s32 ret = SUCCESS;
	struct tisci_msg_wake_reset_req *req =
		(struct tisci_msg_wake_reset_req *) msg_recv;
	struct tisci_msg_wake_reset_resp *resp =
		(struct tisci_msg_wake_reset_resp *) msg_recv;
	u8 host = req->hdr.host;

	resp->hdr.flags = 0U;

	mmr_unlock_all();

	if (ret == SUCCESS) {
		wake_arm((s32) host);
	}

	mmr_lock_all();

	return ret;
}

s32 enable_wdt_handler(u32 *msg_recv)
{
	struct tisci_msg_enable_wdt_resp *resp =
		(struct tisci_msg_enable_wdt_resp *) msg_recv;
	s32 ret = EFAIL;

	resp->hdr.flags = 0U;

	return ret;
}

s32 goodbye_handler(u32 *msg_recv)
{
	struct tisci_msg_goodbye_resp *resp =
		(struct tisci_msg_goodbye_resp *) msg_recv;
	s32 ret = EFAIL;

	resp->hdr.flags = 0U;

	return ret;
}

s32 sys_reset_handler(u32 *msg_recv)
{
	struct tisci_msg_sys_reset_req *req =
		(struct tisci_msg_sys_reset_req *) msg_recv;
	struct tisci_msg_sys_reset_resp *resp =
		(struct tisci_msg_sys_reset_resp *) msg_recv;
	domgrp_t domain = req->domain;
	s32 ret = SUCCESS;

	mmr_unlock_all();

	resp->hdr.flags = 0U;
	ret = system_reset(domain);

	mmr_lock_all();

	return ret;
}
