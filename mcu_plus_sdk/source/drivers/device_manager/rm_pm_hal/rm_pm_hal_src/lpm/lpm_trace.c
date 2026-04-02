/*
 * DM Stub Firmware
 *
 * DM Stub Minimal Trace Debug Layer
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
#include <tisci/lpm/tisci_lpm.h>

#include "lpm_trace.h"
#include "lpm_serial_8250.h"
#include "lpm_trace_buffer.h"

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/**
 * \brief Local trace configuration during runtime
 *
 * \param level Global trace output level.
 * \param error Stores any error flags from public trace API.
 */
struct trace_local_cfg {
	u8	level;
	int	error;
	u16	src_enables;
	u16	dst_enables;
};

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/** By default, enable all of trace. */
static volatile struct trace_local_cfg stub_cfg = {
	.src_enables	= (TISCI_BRDCFG_TRACE_SRC_PM | TISCI_BRDCFG_TRACE_SRC_RM |
			   TISCI_BRDCFG_TRACE_SRC_SEC | TISCI_BRDCFG_TRACE_SRC_BASE),
	.dst_enables	= (TISCI_BRDCFG_TRACE_DST_UART0 | TISCI_BRDCFG_TRACE_DST_ITM |
			   TISCI_BRDCFG_TRACE_DST_MEM),
};

/**
 * \brief Convert integer value to hexadecimal string and store output from least significant
 *        nibble to most significant nibble
 * \param value Input integer value to convert
 * \param str   Pointer to location where output hexadecimal string is stored
 */
static u8 lpm_trace_int_to_hex(u32 value, u8 *str)
{
	u32 val_rem;
	u8 idx = 0U;
	u32 int_value = value;

	if (int_value == 0U) {
		str[idx] = (u8) '0';
		idx++;
	} else {
		while (int_value > 0U) {
			val_rem = int_value % TRACE_HEXADECIMAL_BASE;
			if (val_rem < 10U) {
				str[idx] = (u8) (val_rem + (u8) '0');
			} else {
				str[idx] = (u8) ((val_rem - 10U) + (u8) 'A');
			}
			int_value /= TRACE_HEXADECIMAL_BASE;
			idx++;
		}
	}

	str[idx] = (u8) '\0';

	if (idx > 1U) {
		/* Get length of string - NULL terminator */
		idx--;
	}

	return idx;
}

void lpm_trace_init(sbool bypass)
{
	u32 temp_dst_enables = stub_cfg.dst_enables;
	u32 temp_src_enables = stub_cfg.src_enables;

	if (((temp_dst_enables & TISCI_BRDCFG_TRACE_DST_UART0) != 0U) && ((temp_src_enables & TISCI_BRDCFG_TRACE_SRC_PM) != 0U)) {
		if (bypass == SFALSE) {
			lpm_console_init();
		} else {
			lpm_console_bypass_init();
		}
	}
}

void lpm_trace_debug(u32 value)
{
	u8 str[9];
	u8 len;
	u32 temp_dst_enables = stub_cfg.dst_enables;
	u32 temp_src_enables = stub_cfg.src_enables;

	if (((temp_src_enables & TISCI_BRDCFG_TRACE_SRC_PM) != 0U) && (temp_dst_enables != 0U)) {
		len = lpm_trace_int_to_hex(value, str);

		if ((stub_cfg.dst_enables & TISCI_BRDCFG_TRACE_DST_MEM) != 0U) {
			lpm_trace_debug_buffer(str, len);
		}

		if ((stub_cfg.dst_enables & TISCI_BRDCFG_TRACE_DST_UART0) != 0U) {
			lpm_trace_debug_uart(str, len);
		}
	}
}

void lpm_trace_reconfigure(u16 src_enables, u16 dst_enables)
{
	stub_cfg.src_enables = src_enables;
	stub_cfg.dst_enables = dst_enables;
}
