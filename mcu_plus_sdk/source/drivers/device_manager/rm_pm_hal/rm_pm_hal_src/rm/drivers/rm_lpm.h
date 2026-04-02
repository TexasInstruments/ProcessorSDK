/*
 * RM Low Power Mode
 *
 * RM management low power mode infrastructure
 *
 * Copyright (C) 2024, Texas Instruments Incorporated
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

#ifndef RM_LPM_H
#define RM_LPM_H

#include <config.h>
#include <types/short_types.h>

/**
 * \brief Saves the parameters for a single IR configuration
 *
 * \param id
 * IR SoC device ID.
 *
 * \param inp
 * Input line to specified IR
 *
 * \param outp
 * Output line from specified IR
 *
 * \param validate
 * Validate the inp and outp prior to programming mux if STRUE, do not validate
 * if SFALSE.
 *
 * \return SUCCESS if IR configuration is saved
 */
#ifdef CONFIG_LPM_DM
s32 lpm_sleep_save_ir_config(u16 id, u16 inp, u16 outp, sbool validate);
#else
static inline s32 lpm_sleep_save_ir_config(u16		id __attribute__((unused)),
					   u16		inp __attribute__((unused)),
					   u16		outp __attribute__((unused)),
					   sbool	validate __attribute__((unused)))
{
	return SUCCESS;
}
#endif

/**
 * \brief Removes a single IR configuration from the list
 *
 * \param id
 * IR SoC device ID.
 *
 * \param inp
 * Input line to specified IR
 *
 * \param outp
 * Output line from specified IR
 *
 * \return SUCCESS if IR configuration is removed
 */
#ifdef CONFIG_LPM_DM
s32 lpm_sleep_save_ir_unconfig(u16 id, u16 inp, u16 outp);
#else
static inline s32 lpm_sleep_save_ir_unconfig(u16	id __attribute__((unused)),
					     u16	inp __attribute__((unused)),
					     u16	outp __attribute__((unused)))
{
	return SUCCESS;
}
#endif

/**
 *  \brief  Restore all IR configurations
 *
 *  Iterate through the list and restore all configurations.
 *  Remove the entry from the list once re-configured
 *
 *  \return SUCCESS
 */
#ifdef CONFIG_LPM_DM
s32 lpm_resume_restore_ir_config(void);
#else
static inline s32 lpm_resume_restore_ir_config(void)
{
	return SUCCESS;
}
#endif

#endif /* RM_LPM_H */
