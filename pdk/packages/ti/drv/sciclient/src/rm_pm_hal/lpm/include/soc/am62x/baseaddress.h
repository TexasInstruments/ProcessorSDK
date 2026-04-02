/*
 * DM Stub Firmware
 *
 * am62x soc baseaddress.h
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

#ifndef BASEADDRESS_H_
#define BASEADDRESS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PLLMMR1_CFG_BASE                (0x00680000U)
#define MAIN_PLL_MMR_BASE               PLLMMR1_CFG_BASE
#define PLLMMR0_CFG_BASE                (0x04040000U)
#define MCU_PLL_MMR_BASE                PLLMMR0_CFG_BASE
#define MMR1_CFG_BASE                   (0x04500000U)
#define MCU_CTRL_MMR_BASE               MMR1_CFG_BASE
#define MMR0_CFG_BASE                   (0x43000000U)
#define WKUP_CTRL_MMR_BASE              MMR0_CFG_BASE

#define MAIN_PSC_BASE                   (0x00400000U)
#define MCU_PSC_BASE                    (0x04000000U)

#define WKUP_UART0_BASE                 (0x2b300000U)
#define DDR_CTRL_BASE                   (0x0f308000U)
#define VIM_BASE                        (0x2fff0000U)
#define ROM_SEC_PROXY_RT_ADDRESS        (0x44880000U)
#define ROM_SEC_PROXY_TARGET_ADDRESS    (0x43600000U)
#define TIFS_SEC_PROXY_RT_ADDRESS       (0x4a600000U)
#define TIFS_SEC_PROXY_TARGET_ADDRESS   (0x4d000000U)
#define SEC_PROXY_MSG_RX_TID             0
#define SEC_PROXY_MSG_TX_TID             1
#define TIFS_SEC_PROXY_MSG_RX_TID        69
#define TIFS_SEC_PROXY_MSG_TX_TID        61
#define DM2DMSC_SEC_PROXY_MSG_RX_TID     22
#define DM2DMSC_SEC_PROXY_MSG_TX_TID     23
#define DM_R5_CORE_FREQUENCY_MHZ         400

#ifdef __cplusplus
}
#endif
#endif /* BASEADDRESS_H_ */
