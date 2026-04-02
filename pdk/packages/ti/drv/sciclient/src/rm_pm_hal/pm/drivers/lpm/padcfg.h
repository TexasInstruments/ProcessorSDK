/*
 * Device Manager - Manage PADCFG Ctrl MMR during Suspend/Resume
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

#ifndef __LPM_PADCFG_H__
#define __LPM_PADCFG_H__

#include <types/short_types.h>
#include <types/sbool.h>

#define MAIN_PADCFG_SIZE        (((MAIN_PADCFG_REG_END - PADCFG_OFFSET) >> 2) + 1U)
#define MCU_PADCFG_SIZE         (((MCU_PADCFG_REG_END - PADCFG_OFFSET) >> 2) + 1U)

#define MCU_CAN_PADCFG_START    ((PADCFG_CAN_START_OFFSET - PADCFG_OFFSET) >> 2U)
#define MCU_CAN_PADCFG_END      ((PADCFG_CAN_END_OFFSET - PADCFG_OFFSET) >> 2U)

#define PADCFG_WAKE_EN_BIT      (1U << 29U)
#define PADCFG_WAKE_EVT_BIT     (1U << 30U)

/**
 *  \brief  Save MAIN padcfg Ctrl MMR during suspend
 *
 *  \param wkup_en Boolean pointer to store whether wakeup
 *                 is enabled for any MAIN padcfg pin
 *
 *  \return ret      SUCCESS
 */
s32 lpm_sleep_save_main_padconf(sbool *wkup_en);

/**
 *  \brief  Save MCU padcfg Ctrl MMR during suspend
 *
 *  \param wkup_en Boolean pointer to store whether wakeup
 *                 is enabled for any MCU padcfg pin
 *
 *  \return ret      SUCCESS
 */
s32 lpm_sleep_save_mcu_padconf(sbool *wkup_en);

/**
 *  \brief  Restore MAIN padcfg Ctrl MMR during resume
 *
 *  \return ret      SUCCESS
 */
s32 lpm_resume_restore_main_padconf(void);

/**
 *  \brief  Restore MCU padcfg Ctrl MMR during resume
 *
 *  \return ret      SUCCESS
 */
s32 lpm_resume_restore_mcu_padconf(void);

/**
 *  \brief  Get the MAIN / MCU / CAN pad number that triggered wakeup
 *
 *  \param wkup_src The wake up source for which pin number has to be
 *                  calculated.
 *
 *  \return ret      Pad number
 */
u8 lpm_get_wkup_pin_number_padconf(u32 wkup_src);

#endif /* __LPM_PADCFG_H__ */
