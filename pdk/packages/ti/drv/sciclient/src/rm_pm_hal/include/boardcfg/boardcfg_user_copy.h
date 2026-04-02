/*
 * System Firmware Source File
 *
 * Board configuration internal copy API on HS devices
 *
 * Copyright (C) 2019-2024, Texas Instruments Incorporated
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
#ifndef BOARDCFG_COPY_USER_H
#define BOARDCFG_COPY_USER_H

#include <config.h>
#include <types/address_types.h>

/**
 * \brief on GP, copy the boardcfg after size check. On HS, validate the binary and copy to destination.
 *
 * \param host ID of the host sending the message
 * \param to address in DMSC IRAM where the boardcfg needs to be copied to
 * \param from SOC address from which boardcfg needs to be picked up
 * \param size Size of the board configuration to be copied
 * \param max_size maximum size of board configuration buffer
 *
 * \return SUCCESS on pass, error code on fail
 */
s32 boardcfg_memcpy_rm(u8 host, local_phys_addr_t to, soc_phys_addr_t from, u32 size, u32 max_size);

/**
 * \brief on GP, copy the boardcfg after size check. On HS, validate the binary and copy to destination.
 *        WARNING: Only call this function from a secure context.
 *
 * \param host ID of the host sending the message
 * \param to address in DMSC IRAM where the boardcfg needs to be copied to
 * \param from SOC address from which boardcfg needs to be picked up
 * \param size Size of the board configuration to be copied
 * \param max_size maximum size of board configuration buffer
 *
 * \return SUCCESS on pass, error code on fail
 */
s32 boardcfg_memcpy_rm_core(u8 host, local_phys_addr_t to, soc_phys_addr_t from, u32 size, u32 max_size);

#ifdef CONFIG_SECURE_RM_DM_SUPPORT
/**
 * \brief on GP, copy the boardcfg after size check. On HS, validate the binary and copy to destination.
 *
 * \param host ID of the host sending the message
 * \param to SOC address where boardcfg will be overwritten
 * \param local_cfg local address from which boardcfg needs to be picked up
 * \param size Size of the board configuration to be copied
 *
 * \return SUCCESS on pass, error code on fail
 */
#ifdef CONFIG_DEVICE_TYPE_HS
s32 boardcfg_rm_overwrite_hdr(u8 host, soc_phys_addr_t to, local_phys_addr_t local_cfg, u16 size);
#else
static inline s32 boardcfg_rm_overwrite_hdr(u8 host __attribute__((unused)), soc_phys_addr_t to __attribute__((unused)), local_phys_addr_t local_cfg __attribute__((unused)), u16 size __attribute__(
						    (unused)))
{
	return SUCCESS;
}
#endif  /* CONFIG_DEVICE_TYPE_HS */
#endif  /* CONFIG_SECURE_RM_DM_SUPPORT */

#endif
