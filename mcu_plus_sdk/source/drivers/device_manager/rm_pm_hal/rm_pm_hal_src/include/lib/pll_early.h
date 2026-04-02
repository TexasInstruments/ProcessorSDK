/*
 * System Firmware Early PLL low level PLL init code
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
#ifndef PLL_EARLY_H
#define PLL_EARLY_H
#include <types/address_types.h>
#include <types/short_types.h>
#include <types/devgrps.h>

#define INCLK_MAX 6

struct pll_early_data_entry {
	u16	pllm;
	u32	pllfm;
	u16	plld;
	u8	od;
	u8	post_div2;
};

struct pll_early_data {
	mapped_addr_t			base;
	struct pll_early_data_entry	pll_entries[INCLK_MAX];
	u16				hsdiv_defaults[10];
};

/**
 * \brief Devgrp description for early initialization logic
 * \param devgrp what devgrp this belongs to
 * \param early_pll_pllctrls NULL terminated list of PLL ctrl base
 * \param *early_plls_cfgs NULL terminated list of pll configs
 */
struct pll_early_devgrp_data {
	devgrp_t			devgrp;
	const u32			*early_pll_pllctrls;
	const struct pll_early_data	**early_plls_cfgs[];
};

/* NULL terminated list, should be of size at least 1 and at most MAX_NUM_DEVGRPS */
extern const struct pll_early_devgrp_data *const early_pll_devgrp[];

s32 pll_early_config(devgrp_t devgrp);

#endif /* PLL_EARLY_H */
