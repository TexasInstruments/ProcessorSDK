/*
 * DM Stub Firmware
 *
 * PLL Raw driver for direct PLL manipulation
 *
 * Copyright (C) 2021-2024, Texas Instruments Incorporated
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
#include <types/sbool.h>

#define PLL_DISABLE    0U
#define PLL_ENABLE     1U

struct pll_raw_data {
	u32	base;
	u32	freq_ctrl0;
	u32	freq_ctrl1;
	u32	div_ctrl;
	u32	hsdiv[16];
};

/**
 * \brief Restore a saved pll configuration from pll_save
 * \param pll Pointer to location to restore saved PLL context
 */
s32 pll_restore(struct pll_raw_data *pll);

/**
 * \brief Save a pll configuration from pll_save
 * \param pll Pointer to location to save PLL context, with base set
 *            to address for PLL to save.
 */
void pll_save(struct pll_raw_data *pll);

/**
 * \brief Disable a PLL after bypassing
 * \param pll Pointer to PLL context, with base set to address for pll to
 *            manipulate
 */
void pll_disable(struct pll_raw_data *pll);

/**
 * \brief Disable a hsdiv of PLL
 * \param pll_base PLL base set to address for pll to manipulate
 * \param hsdiv Bit-field indicating which hsdiv should be
 *			    disabled.
 * \param enable Control value of PLL. A value of 1 means hsdiv will
 *               be enabled and 0 means it will be disabled
 */
void pll_hsdiv_ctrl(u32 pll_base, u8 hsdiv, u8 enable);

/**
 * \brief Bypass a PLL
 * \param pll Pointer to PLL context, with base set to address for pll to
 *            manipulate
 */
void pll_bypass(struct pll_raw_data *pll, sbool enable);
