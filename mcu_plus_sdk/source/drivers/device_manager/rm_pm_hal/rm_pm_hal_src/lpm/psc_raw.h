/*
 * DM Stub Firmware
 *
 * PSC Raw driver for direct PSC manipulation
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

#define MDCTL_STATE_SWRSTDISABLE       0x00U
#define MDCTL_STATE_SYNCRST            0x01U
#define MDCTL_STATE_DISABLE            0x02U
#define MDCTL_STATE_ENABLE             0x03U
#define MDCTL_STATE_AUTO_SLEEP         0x04U
#define MDCTL_STATE_AUTO_WAKE          0x05U

#define PDCTL_STATE_OFF         0U
#define PDCTL_STATE_ON          1U

/**
 * @brief Structure to store power domain and corresponding lpsc
 * \param pd Power domain index of LPSC
 * \param lpsc LPSC index value
 */
struct pd_lpsc {
	u8	pd;
	u8	lpsc;
};

/**
 * @brief Structure to store USB LPSC state, power domain, LPSC ID and ISO LPSC ID
 * \param pd Power domain index of LPSC
 * \param lpsc LPSC index value
 * \param iso_lpsc Isolation LPSC index value
 * \param state Current state/ resume state of USB LPSC
 */
struct usb_lpsc {
	u8	pd;
	u8	lpsc;
	u8	iso_lpsc;
	u8	state;
};

/**
 * \brief Wait for a psc transition to complete, or timeout if it does not
 * \param psc_base Base address of the psc to wait for
 * \param pd Power Domain index to wait for a transition on
 *
 * \return SUCCESS on success, -ETIMEDOUT if timeout occurs
 */
s32 psc_raw_pd_wait(u32 psc_base, u8 pd);

/**
 * \brief Initiate a psc transition for a power domain
 * \param psc_base Base address of the psc to transition
 * \param pd Power Domain index to start transition for
 */
void psc_raw_pd_initiate(u32 base, u8 pd);

/**
 * \brief Set next state for a PDCTL
 * \param psc_base Base address of the psc
 * \param pd Power Domain index to set next state for
 * \param state PDCTL_STATE to set
 * \param force Boolean indicating whether to set FORCE bit
 */
void psc_raw_pd_set_state(u32 psc_base, u8 pd, u32 state, sbool force);

/**
 * \brief Set next state for an MDCTL
 * \param psc_base Base address of the psc
 * \param lpsc LPSC index to set next state for
 * \param state MDCTL_STATE to set
 * \param force Boolean indicating whether to set FORCE bit
 */
void psc_raw_lpsc_set_state(u32 psc_base, u8 lpsc, u32 state, sbool force);

/**
 * \brief Get current state of a module
 * \param psc_base Base address of the psc
 * \param lpsc LPSC index to get next state
 *
 * \return Current state of LPSC. 0x3 if LPSC is enabled
 */
u8 psc_raw_lpsc_get_state(u32 psc_base, u8 lpsc);
