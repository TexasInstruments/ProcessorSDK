/*
 * Proxy
 *
 * proxy driver instance definition
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
#ifndef PROXY_INST_H
#define PROXY_INST_H

#include <types/short_types.h>
#include <types/devgrps.h>
#include <soc/rm_mmr.h>
#include <rm_irq.h>

/**
 * \brief
 * proxy instance containing all data required to manage a
 * Navigator Subsystem proxy
 *
 * \param id
 * Device ID of NavSS proxy
 *
 * \param initialized
 * Instance initialization flag
 *
 * \param devgrp
 * Device group to which instance belongs
 *
 * \param cfg
 * Proxy subsystem's OES MMR region
 *
 * \param target0_data
 * Proxy subsystem's target data control MMR region
 *
 * \param n_proxies
 * Total number of proxies in the subsystem
 *
 * \param proxy_utype
 * Resource assignment type mapped to proxies
 *
 * \param oes_handler
 * IRQ event source handler
 */
struct proxy_instance {
	const u16			id;
	sbool				initialized;
	const devgrp_t			devgrp;
	const struct rm_mmr *const	cfg;
	const struct rm_mmr *const	target0_data;
	const u16			n_proxies;
	const u16			proxy_utype;
	struct rm_irq_oes_src		oes_handler;
};

#endif /* PROXY_INST_H */
