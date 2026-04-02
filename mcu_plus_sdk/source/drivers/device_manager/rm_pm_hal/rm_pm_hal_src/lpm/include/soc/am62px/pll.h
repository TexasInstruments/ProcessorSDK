/*
 * DM Stub Firmware
 *
 * am62px soc pll.h
 *
 * Copyright (C) 2023-2024, Texas Instruments Incorporated
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

#ifndef PLL_H_
#define PLL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <types/short_types.h>
#include <baseaddress.h>
#include <pll_16fft_raw.h>

#define PLLOFFSET(idx) (0x1000U * (idx))

enum save_restore_main_pll {
	SAVE_RESTORE_MAIN_PLL0,
	SAVE_RESTORE_MAIN_PLL1,
	SAVE_RESTORE_MAIN_PLL2,
	SAVE_RESTORE_MAIN_PLL5,
	SAVE_RESTORE_MAIN_PLL6,
	SAVE_RESTORE_MAIN_PLL7,
	SAVE_RESTORE_MAIN_PLL8,
	SAVE_RESTORE_MAIN_PLL12,
	SAVE_RESTORE_MAIN_PLL16,
	SAVE_RESTORE_MAIN_PLL17,
	SAVE_RESTORE_MAIN_PLL18,
	SAVE_RESTORE_MAIN_PLL_MAX,
};

enum main_pll_disable {
	MAIN_PLL1_DISABLE,
	MAIN_PLL2_DISABLE,
	MAIN_PLL5_DISABLE,
	MAIN_PLL6_DISABLE,
	MAIN_PLL7_DISABLE,
	MAIN_PLL8_DISABLE,
	MAIN_PLL12_DISABLE,
	MAIN_PLL16_DISABLE,
	MAIN_PLL17_DISABLE,
	MAIN_PLL18_DISABLE,
	MAIN_PLL_DISABLE_MAX,
};

extern struct pll_raw_data mcu_pll;
extern struct pll_raw_data main_pll0;
extern struct pll_raw_data main_pll1;
extern struct pll_raw_data main_pll2;
extern struct pll_raw_data main_pll5;
extern struct pll_raw_data main_pll6;
extern struct pll_raw_data main_pll7;
extern struct pll_raw_data main_pll8;
extern struct pll_raw_data main_pll12;
extern struct pll_raw_data main_pll16;
extern struct pll_raw_data main_pll17;
extern struct pll_raw_data main_pll18;

extern struct pll_raw_data *main_plls_save_rstr[SAVE_RESTORE_MAIN_PLL_MAX];
extern u8 num_main_plls_save_rstr;
extern struct pll_raw_data *main_plls_dis[MAIN_PLL_DISABLE_MAX];
extern u8 num_main_plls_dis;

extern struct pll_raw_data usb_pll;

#ifdef __cplusplus
}
#endif
#endif /* PLL_H_ */
