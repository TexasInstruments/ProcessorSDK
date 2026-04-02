/*
 * DM Stub Firmware
 *
 * DM Stub Minimal Trace Debug Layer
 *
 * Copyright (C) 2021-2023, Texas Instruments Incorporated
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

#include <lib/trace_protocol.h>

#define TRACE_HEXADECIMAL_BASE            16U

#define TISCI_BRDCFG_TRACE_SRC_PM         BIT(0)
#define TISCI_BRDCFG_TRACE_SRC_RM         BIT(1)
#define TISCI_BRDCFG_TRACE_SRC_SEC        BIT(2)
#define TISCI_BRDCFG_TRACE_SRC_BASE       BIT(3)
#define TISCI_BRDCFG_TRACE_DST_UART0      BIT(0)
#define TISCI_BRDCFG_TRACE_DST_ITM        BIT(2)
#define TISCI_BRDCFG_TRACE_DST_MEM        BIT(3)

/**
 * \brief Initialize console by configuring UART at clock frequency
 * \param bypass Bit-field indicating whether clock frequency should be
 *               used when PLL is in bypass mode or not. When enabled,
 *               bypass clock frequency is used. When disabled, normal clock
 *               frequency is used.
 */
void lpm_trace_init(sbool bypass);

/**
 * \brief Outputs u32 trace debug value to configured trace destinations.
 * \param value Trace debug value to output.
 */
void lpm_trace_debug(u32 value);

/**
 * \brief Provide boardcfg values for trace src_enables and dst_enables
 * \param src_enables Value from boardcfg_dbg_cfg data structure.
 * \param dst_enables Value from boardcfg_dbg_cfg data structure.
 */
void lpm_trace_reconfigure(u16 src_enables, u16 dst_enables);

#ifdef CONFIG_LPM_DM_TRACE
#define lpm_seq_trace(step) lpm_trace_debug((TRACE_DEBUG_CHANNEL_PM << TRACE_DEBUG_DOMAIN_SHIFT) \
					    | (((u32) TRACE_PM_ACTION_LPM_SEQUENCE) << TRACE_DEBUG_ACTION_SHIFT) \
					    | (((u32) step) << TRACE_PM_ACTION_LPM_SEQ_SHIFT)			\
					    | (0U))


#define lpm_seq_trace_fail(step) lpm_trace_debug((TRACE_DEBUG_CHANNEL_PM << TRACE_DEBUG_DOMAIN_SHIFT) \
						 | (((u32) TRACE_PM_ACTION_LPM_SEQUENCE | TRACE_PM_ACTION_FAIL) << TRACE_DEBUG_ACTION_SHIFT)  \
						 | (((u32) step) << TRACE_PM_ACTION_LPM_SEQ_SHIFT)		     \
						 | (0U))

#define lpm_seq_trace_val(step, val) lpm_trace_debug((TRACE_DEBUG_CHANNEL_PM << TRACE_DEBUG_DOMAIN_SHIFT) \
						     | (((u32) TRACE_PM_ACTION_LPM_SEQUENCE) << TRACE_DEBUG_ACTION_SHIFT) \
						     | (((u32) step) << TRACE_PM_ACTION_LPM_SEQ_SHIFT)			 \
						     | (val))
#else
#define lpm_seq_trace(step)
#define lpm_seq_trace_fail(step)
#define lpm_seq_trace_val(step, val)
#endif
