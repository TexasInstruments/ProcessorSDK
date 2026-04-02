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

#include <lib/ioremap.h>
#include <lib/mmr_lock.h>
#include <types/errno.h>
#include <tisci/lpm/tisci_lpm.h>
#include "soc_ctrl_mmr.h"
#include "padcfg.h"

#define DIVIDE_BY_64(x)                 (x >> 6U)
#define MULTIPLY_BY_64(x)               (x << 6U)
#define MULTIPLY_BY_4(x)                (x << 2U)
#define NUM_BITS_U64                    64U

#define NUM_MAIN_PADCFG_IN_U64          (unsigned) ((MAIN_PADCFG_SIZE / NUM_BITS_U64) + (((MAIN_PADCFG_SIZE % NUM_BITS_U64) != 0U) ? 1U : 0U))

static u32 main_padcfg_data[MAIN_PADCFG_SIZE];
static u32 mcu_padcfg_data[MCU_PADCFG_SIZE];

/* Buffer to store wakeup en value for each pad config - enabled or disabled,
 * where bit number corresponds to wake enable value of each pin respectively.
 */
static u64 main_wken_data[NUM_MAIN_PADCFG_IN_U64];
static u64 mcu_wken_data;

static u8 lpm_get_wkup_pin_number_main_padconf(void)
{
	u32 p = MAIN_PADCFG_CTRL_BASE + PADCFG_OFFSET;
	u8 wkup_pin = TISCI_MSG_VALUE_LPM_WAKE_PIN_INVALID;
	u32 i;
	u32 j;

	for (j = 0U; j < NUM_MAIN_PADCFG_IN_U64; j++) {
		for (i = 0U; i < NUM_BITS_U64; i++) {
			/* Check if wkup enable bit is set */
			if (((main_wken_data[j] >> i) & 1U) == 1U) {
				/* Check whether wkup event bit is set or not */
				if (((readl(p + MULTIPLY_BY_4(i))) & PADCFG_WAKE_EVT_BIT) == PADCFG_WAKE_EVT_BIT) {
					wkup_pin = i + MULTIPLY_BY_64(j);
					break;
				}
			}
		}
		/* Update the address of MAIN padconfig base for next set of values */
		p += MULTIPLY_BY_4(NUM_BITS_U64);
	}

	return wkup_pin;
}

static u8 lpm_get_wkup_pin_number_mcu_padconf(void)
{
	u32 p = MCU_PADCFG_CTRL_BASE + PADCFG_OFFSET;
	u8 wkup_pin = TISCI_MSG_VALUE_LPM_WAKE_PIN_INVALID;
	u32 i;


	for (i = 0U; i < MCU_PADCFG_SIZE; i++) {
		/* CAN IO pins in MCU trigger only MCU IO event, skip scanning them */
		if ((i >= MCU_CAN_PADCFG_START) && (i <= MCU_CAN_PADCFG_END)) {
			continue;
		}
		/* Check if wkup enable bit is set */
		if (((mcu_wken_data >> i) & 1U) == 1U) {
			/* Check whether wkup event bit is set or not */
			if (((readl(p + MULTIPLY_BY_4(i))) & PADCFG_WAKE_EVT_BIT) == PADCFG_WAKE_EVT_BIT) {
				wkup_pin = i;
				break;
			}
		}
	}

	return wkup_pin;
}

static u8 lpm_get_wkup_pin_number_mcu_can_padconf(void)
{
	u32 p = MCU_PADCFG_CTRL_BASE + PADCFG_OFFSET;
	u8 wkup_pin = TISCI_MSG_VALUE_LPM_WAKE_PIN_INVALID;
	u32 i;

	for (i = MCU_CAN_PADCFG_START; i <= MCU_CAN_PADCFG_END; i++) {
		/* Check if wkup enable bit is set */
		if (((mcu_wken_data >> i) & 1U) == 1U) {
			/* Check whether wkup event bit is set or not */
			if (((readl(p + MULTIPLY_BY_4(i))) & PADCFG_WAKE_EVT_BIT) == PADCFG_WAKE_EVT_BIT) {
				wkup_pin = i;
				break;
			}
		}
	}

	return wkup_pin;
}

s32 lpm_sleep_save_main_padconf(sbool *wkup_en)
{
	u32 p = MAIN_PADCFG_CTRL_BASE + PADCFG_OFFSET;
	u32 i;
	u32 j;

	for (i = 0U; i < MAIN_PADCFG_SIZE; i++) {
		main_padcfg_data[i] = readl(p);
		p += 4U;

		if ((main_padcfg_data[i] & PADCFG_WAKE_EN_BIT) == PADCFG_WAKE_EN_BIT) {
			/* Find the array index to store padcfg en bit */
			j = DIVIDE_BY_64(i);
			/* Set the bit position for pin having wakeup enabled */
			main_wken_data[j] |= ((u64) 1UL) << (i % NUM_BITS_U64);
			*wkup_en = STRUE;
		}
	}

	return SUCCESS;
}

s32 lpm_sleep_save_mcu_padconf(sbool *wkup_en)
{
	u32 p = MCU_PADCFG_CTRL_BASE + PADCFG_OFFSET;
	u32 i;

	for (i = 0U; i < MCU_PADCFG_SIZE; i++) {
		mcu_padcfg_data[i] = readl(p);
		p += 4U;
		if ((mcu_padcfg_data[i] & PADCFG_WAKE_EN_BIT) == PADCFG_WAKE_EN_BIT) {
			/* Set the bit position for pin having wakeup enabled */
			mcu_wken_data |= ((u64) 1UL) << i;

			/* Skip setting wake enable for CAN pins as CAN IO daisy chain is the default wake up source */
			if ((i >= MCU_CAN_PADCFG_START) && (i <= MCU_CAN_PADCFG_END)) {
				continue;
			} else {
				*wkup_en = STRUE;
			}
		}
	}

	return SUCCESS;
}

s32 lpm_resume_restore_main_padconf(void)
{
	u32 p = MAIN_PADCFG_CTRL_BASE + PADCFG_OFFSET;
	u32 i;

	mmr_unlock(MAIN_PADCFG_CTRL_BASE, 1);

	for (i = 0; i < MAIN_PADCFG_SIZE; i++) {
		writel(main_padcfg_data[i], p);
		p += 4U;
	}

	return SUCCESS;
}

s32 lpm_resume_restore_mcu_padconf(void)
{
	u32 p = MCU_PADCFG_CTRL_BASE + PADCFG_OFFSET;
	u32 i;

	mmr_unlock(MCU_PADCFG_CTRL_BASE, 1);
	for (i = 0; i < MCU_PADCFG_SIZE; i++) {
		writel(mcu_padcfg_data[i], p);
		p += 4U;
	}

	return SUCCESS;
}

u8 lpm_get_wkup_pin_number_padconf(u32 wkup_src)
{
	u8 wkup_pin;
	u32 i;

	if (wkup_src == TISCI_MSG_VALUE_LPM_WAKE_SOURCE_MAIN_IO) {
		wkup_pin = lpm_get_wkup_pin_number_main_padconf();
	} else if (wkup_src == TISCI_MSG_VALUE_LPM_WAKE_SOURCE_MCU_IO) {
		wkup_pin = lpm_get_wkup_pin_number_mcu_padconf();
	} else if (wkup_src == TISCI_MSG_VALUE_LPM_WAKE_SOURCE_CAN_IO) {
		wkup_pin = lpm_get_wkup_pin_number_mcu_can_padconf();
	} else {
		wkup_pin = TISCI_MSG_VALUE_LPM_WAKE_PIN_INVALID;
	}

	/* Clear the saved values for padcfg enable */
	mcu_wken_data = 0UL;
	for (i = 0U; i < NUM_MAIN_PADCFG_IN_U64; i++) {
		main_wken_data[i] = 0UL;
	}

	return wkup_pin;
}
