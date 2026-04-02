/*
 * System Firmware TISCI RM Proxy Messaging
 *
 * TISCI Protocol Definitions for proxy messages
 *
 * Copyright (C) 2019-2020, Texas Instruments Incorporated
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

#ifndef RM_TISCI_PROXY_H
#define RM_TISCI_PROXY_H

#include <tisci/tisci_protocol.h>

/**
 * \brief Configures a Navigator Subsystem proxy
 *
 * Configures the channelized firewalls of a Navigator Subsystem
 * proxy.  The firewalls are configured to all the header host access to the
 * backing RAM and target data configuration registers.  The proxy index must
 * be assigned to the host defined in the TISCI header via the RM board
 * configuration resource assignment range list.
 *
 * \param hdr
 * Standard TISCI header
 *
 * \param valid_params
 * Bitfield defining validity of proxy configuration parameters.
 * This field is unused at the moment but exists if expansion of the API is
 * needed in the future.
 *
 * \param nav_id
 * SoC device ID of Navigator Subsystem where proxy is located
 *
 * \param index
 * Proxy index.
 */
struct tisci_msg_rm_proxy_cfg_req {
	struct tisci_header	hdr;
	u32			valid_params;
	u16			nav_id;
	u16			index;
} __attribute__((__packed__));

/**
 * \brief Response to configuring a proxy.
 *
 * \param hdr
 * Standard TISCI header
 */
struct tisci_msg_rm_proxy_cfg_resp {
	struct tisci_header hdr;
} __attribute__((__packed__));

#endif /* RM_TISCI_PROXY_H */
