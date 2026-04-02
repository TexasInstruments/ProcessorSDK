/*
 * DM Stub Firmware
 *
 * am62x lpm wkup_ctrl_mmr.h
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

#ifndef LPM_WKUP_CTRL_MMR_H_
#define LPM_WKUP_CTRL_MMR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define WKUP_CLKSEL                     (0x00008010U)
#define DDR16SS_PMCTRL                  (0x000080d0U)
#define WKUP_WWD0_CTRL                  (0x00008340U)
#define CHNG_DDR4_FSP_REQ               (0x00014000U)
#define CHNG_DDR4_FSP_ACK               (0x00014004U)
#define DDR4_FSP_CLKCHNG_REQ            (0x00014080U)
#define DDR4_FSP_CLKCHNG_ACK            (0x000140C0U)
#define PMCTRL_SYS                      (0x00018080U)
#define PMCTRL_IO_0                     (0x00018084U)
#define PMCTRL_MOSC                     (0x00018090U)
#define RST_CTRL                        (0x00018170U)
#define SLEEP_STATUS                    (0x00018410U)
#define DS_MAGIC_WORD                   (0x00018418U)
#define DS_MAIN                         (0x00018420U)
#define DS_DM_RESET                     (0x00018440U)
#define DS_DDR0_RESET                   (0x00018450U)
#define DS_USB0_RESET                   (0x00018460U)
#define DS_USB1_RESET                   (0x00018464U)
#define DM_CLKSTOP_EN                   (0x00018480U)
#define DM_CLKSTOP_ACK                  (0x00018490U)
#define DM_GRP_CLKSTOP_REQ              (0x000184A0U)
#define DM_GRP_CLKSTOP_ACK              (0x000184A4U)

/* WKUP0_EN */
#define WKUP0_EN                        (0x00018180U)

#define WKUP0_EN_WKUP_I2C0              BIT(0)
#define WKUP0_EN_WKUP_USART0            BIT(1)
#define WKUP0_EN_MCU_GPIO0              BIT(2)
#define WKUP0_EN_DEBUG                  BIT(3)
#define WKUP0_EN_MCU_M4F_INT            BIT(4)
#define WKUP0_EN_WKUP_DMTIMER0          BIT(5)
#define WKUP0_EN_WKUP_DMTIMER1          BIT(6)
#define WKUP0_EN_WKUP_RTC               BIT(7)
#define WKUP0_EN_MAIN_RESET_REQZ        BIT(8)
#define WKUP0_EN_USB0                   BIT(9)
#define WKUP0_EN_USB1                   BIT(10)
#define WKUP0_EN_MAIN_IO_DAISY_CHAIN    BIT(16)
#define WKUP0_EN_MCU_IO_DAISY_CHAIN     BIT(17)
#define WKUP0_EN_CANUART_IO_DAISY_CHAIN BIT(18)

/* WKUP_EN_CLKSTOP */
#define WKUP_EN_CLKSTOP_ALL             (0x00030009U)

#define WKUP_EN_CLKSTOP_WKUP_I2C0       BIT(0)
#define WKUP_EN_CLKSTOP_WKUP_USART0     BIT(3)
#define WKUP_EN_CLKSTOP_WKUP_DMTIMER0   BIT(16)
#define WKUP_EN_CLKSTOP_WKUP_DMTIMER1   BIT(17)
#define WKUP_EN_GRP_CLKSTOP_REQ         BIT(0)
#define WKUP_DIS_GRP_CLKSTOP_REQ        0U
#define WKUP_EN_GRP_CLKSTOP_ACK         BIT(0)
#define WKUP_DIS_GRP_CLKSTOP_ACK        0U

/* WKUP_CLKSEL */
#define WKUP_CLKSEL_MAIN                0U
#define WKUP_CLKSEL_MCU                 1U

/* DDR16SS_PMCTRL */
#define DDR16SS_RETENTION_DIS           0U
#define DDR16SS_RETENTION_EN            0x6U

/* WKUP_WWD0_CTRL */
#define WWD_RUN                         0U
#define WWD_STOP                        0xAU

/* CHNG_DDR4_FSP_REQ */
#define CHNG_DDR4_FSP_REQ_SET           BIT(8)
#define CHNG_DDR4_FSP_REQ_CLR           0U
#define CHNG_DDR4_FSP_REQ_TYPE_MASK     0x3U
#define CHNG_DDR4_FSP_REQ_TYPE_FSP0     0x0U
#define CHNG_DDR4_FSP_REQ_TYPE_FSP2     0x2U

/* CHNG_DDR4_FSP_ACK */
#define CHNG_DDR4_FSP_CHNG_ACK          BIT(7)
#define CHNG_DDR4_FSP_CHNG_ERR          BIT(0)

/* DDR4_FSP_CLKCHNG_REQ */
#define DDR4_FSP_CLKCHNG_REQ_SET        BIT(7)
#define DDR4_FSP_CLKCHNG_REQ_CLR        0U
#define DDR4_FSP_CLKCHNG_REQ_TYPE_MASK  0x3U
#define DDR4_FSP_CLKCHNG_REQ_TYPE_FSP0  0x0U
#define DDR4_FSP_CLKCHNG_REQ_TYPE_FSP2  0x2U

/* DDR4_FSP_CLKCHNG_ACK */
#define DDR4_FSP_CLKCHNG_REQ_ACK        BIT(0)

/* WKUP_PMCTRL_SYS */
#define WKUP0_PMCTRL_SYS_LPM_EN_DIS     (0x5U)
#define WKUP0_PMCTRL_SYS_LPM_EN_PMIC    (0x0U)
#define WKUP0_LPM_PMIC_OUT_EN           (0x10U)
#define WKUP0_LPM_PMIC_OUT_DIS          (0x0U)

/* PMCTRL_IO_0 */
#define PMCTRL_IO_0_IO_ISO_STATUS       BIT(25)
#define PMCTRL_IO_0_IO_ISO_CTRL         BIT(24)
#define PMCTRL_IO_0_IO_ON_STATUS        BIT(5)

/* PMCTRL_MOSC */
#define PMCTRL_MOSC_OSC_CG_ON_WFI       BIT(31)

/* RST_CTRL */
#define RST_CTRL_MAIN_RST_ISO_DONE_Z    BIT(18)

/* SLEEP_STATUS */
#define SLEEP_STATUS_MAIN_RESETSTATZ    BIT(8)

/* DS_MAGIC_WORD */
#define DS_MAGIC_WORD_RESUME_TIFS           0xdee51ee5U
#define DS_MAGIC_WORD_RESUME_ROM            0x00d5d02eU

/* DS_MAIN */
#define DS_MAIN_OFF                     0x6U
#define DS_MAIN_ON                      0xFU

/* Deep Sleep reset mask and unmask */
#define DS_RESET_MASK                   0x6U
#define DS_RESET_UNMASK                 0xFU

#ifdef __cplusplus
}
#endif
#endif /* LPM_WKUP_CTRL_MMR_H_ */
