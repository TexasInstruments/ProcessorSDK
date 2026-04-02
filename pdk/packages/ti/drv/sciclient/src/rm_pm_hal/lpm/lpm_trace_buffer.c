/*
 * DM Stub Firmware
 *
 * DM Stub Memory Trace Buffer Layer
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

#include "lpm_trace_buffer.h"

/* Memory buffer to save low power mode trace messages */
u8 lpm_trace_logbuf[LPM_TRACE_LOG_BUF_SIZE] __attribute__((section(".lpm_trace_buf"), aligned(4)));
static u8 *lpm_logbuf_pos = &lpm_trace_logbuf[0];

/**
 * \brief Puts a u8 character into the trace buffer
 * \param ch character to write
 */
static void lpm_trace_print_buffer(u8 ch)
{
	if (lpm_logbuf_pos == NULL) {
		lpm_logbuf_pos = &lpm_trace_logbuf[0];
	}

	*lpm_logbuf_pos = ch;
	lpm_logbuf_pos++;
	if (lpm_logbuf_pos == &lpm_trace_logbuf[LPM_TRACE_LOG_BUF_SIZE]) {
		lpm_logbuf_pos = &lpm_trace_logbuf[0];
	}
}

void lpm_trace_debug_buffer(u8 *str, u8 len)
{
	u32 i;

	/* Output "0x" at start of hex */
	lpm_trace_print_buffer('0');
	lpm_trace_print_buffer('x');

	/*
	 * Output string backwards as we converted from low
	 * digit to high
	 */
	for (i = 0U; i <= len; i++) {
		lpm_trace_print_buffer(str[len - i]);
	}

	/* Add a carriage return to support inflexible terminals. */
	lpm_trace_print_buffer('\r');

	/* Move the cursor to new line. */
	lpm_trace_print_buffer('\n');
}
