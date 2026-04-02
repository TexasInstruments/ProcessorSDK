/*
 * DM Stub Firmware
 *
 * am62ax lpm mcu_ctrl_mmr.h
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

#ifndef LPM_MCU_CTRL_MMR_H_
#define LPM_MCU_CTRL_MMR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define HFOSC0_CTRL                     (0x00008010U)
#define MCU_PLL_CLKSEL                  (0x00008050U)

/* HFOSC0_CTRL */
#define HFOSC0_CTRL_PD_C                BIT(7)

/* MCU_PLL_CLKSEL */
#define MCU_PLL_CLKSEL_CLKLOSS_SWTCH_EN BIT(8)

#define MCU_CTRL_MMR_CFG0_MCU_GPIO_CLKSEL               (0x8070U)
#define MCU_CTRL_MMR_CFG0_MCU_GPIO_CLKSEL_MCU_SYSCLK0   0U
#define MCU_CTRL_MMR_CFG0_MCU_GPIO_CLKSEL_CLK_32K       2U
#define MCU_CTRL_MMR_CFG0_MCU_GPIO_WKUP_CTRL            (0x4020U)
#define MCU_CTRL_MMR_CFG0_MCU_GPIO_WKUP_CTRL_ENABLE     1U
#define MCU_CTRL_MMR_CFG0_MCU_GPIO_WKUP_CTRL_DISABLE    0U

#define MCU_IPC_INTERRUPT_NUMBER                        0U
#define MCU_CTRL_MMR_IPC_CLR0                           (0x180U)
#define MCU_CTRL_MMR_IPC_CLR0_CLEAR                     (0x11U)

#ifdef __cplusplus
}
#endif
#endif /* LPM_MCU_CTRL_MMR_H_ */
