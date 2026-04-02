/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2014-2020, Texas Instruments Incorporated
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
#ifndef PRU_H
#define PRU_H

#include <lib/bitops.h>
#include <config.h>

#define PRU_RAM0                0x00000
#define PRU_RAM1                0x02000
#define PRU_RAM3                0x10000
#define PRU_INTC                0x20000
#define PRU_PRU0_CONTROL        0x22000
#define PRU_PRU0_DEBUG          0x22400
#define PRU_PRU1_CONTROL        0x24000
#define PRU_PRU1_DEBUG          0x22400
#define PRU_CFG                 0x26000
#define PRU_ECC_CFG             0x27000
#define PRU_UART0               0x28000
#define PRU_IEP                 0x2e000
#define PRU_ECAP0               0x30000
#define PRU_MII_RT_CFG          0x32000
#define PRU_MII_MDIO            0x32400
#define PRU_PRU0_IRAM           0x34000
#define PRU_PRU1_IRAM           0x38000
#define PRU_VBUSP               0x40000

#define PRU_CFG_REVID           (PRU_CFG + 0x000)
#define PRU_CFG_GPCFG0          (PRU_CFG + 0x008)
#define PRU_CFG_GPCFG1          (PRU_CFG + 0x00c)
#define PRU_CFG_CGR             (PRU_CFG + 0x010)
#define PRU_CFG_SCRP            (PRU_CFG + 0x024)
#define PRU_CFG_PMAO            (PRU_CFG + 0x028)
#define PRU_CFG_MIIRTEEN        (PRU_CFG + 0x02c)
#define PRU_CFG_IEPCLK          (PRU_CFG + 0x030)
#define PRU_CFG_SPP             (PRU_CFG + 0x034)
#define PRU_CFG_P2PCFG          (PRU_CFG + 0x038)
#define PRU_CFG_SAMSEL          (PRU_CFG + 0x040)
#define PRU_CFG_SDCLK(P, SD)     (P0SD0CLK + (P) * 0x4c + (SD) * 8)
#define PRU_CFG_P0SD0CLK        (PRU_CFG + 0x048)
#define PRU_CFG_P0SD0SS         (PRU_CFG + 0x04c)
#define PRU_CFG_P0SD1CLK        (PRU_CFG + 0x050)
#define PRU_CFG_P0SD1SS         (PRU_CFG + 0x054)
#define PRU_CFG_P0SD2CLK        (PRU_CFG + 0x058)
#define PRU_CFG_P0SD2SS         (PRU_CFG + 0x05c)
#define PRU_CFG_P0SD3CLK        (PRU_CFG + 0x060)
#define PRU_CFG_P0SD3SS         (PRU_CFG + 0x064)
#define PRU_CFG_P0SD4CLK        (PRU_CFG + 0x068)
#define PRU_CFG_P0SD4SS         (PRU_CFG + 0x06c)
#define PRU_CFG_P0SD5CLK        (PRU_CFG + 0x070)
#define PRU_CFG_P0SD5SS         (PRU_CFG + 0x074)
#define PRU_CFG_P0SD6CLK        (PRU_CFG + 0x078)
#define PRU_CFG_P0SD6SS         (PRU_CFG + 0x07c)
#define PRU_CFG_P0SD7CLK        (PRU_CFG + 0x080)
#define PRU_CFG_P0SD7SS         (PRU_CFG + 0x084)
#define PRU_CFG_P0SD8CLK        (PRU_CFG + 0x088)
#define PRU_CFG_P0SD8SS         (PRU_CFG + 0x08c)
#define PRU_CFG_P1SD0CLK        (PRU_CFG + 0x094)
#define PRU_CFG_P1SD0SS         (PRU_CFG + 0x098)
#define PRU_CFG_P1SD1CLK        (PRU_CFG + 0x09c)
#define PRU_CFG_P1SD1SS         (PRU_CFG + 0x0a0)
#define PRU_CFG_P1SD2CLK        (PRU_CFG + 0x0a4)
#define PRU_CFG_P1SD2SS         (PRU_CFG + 0x0a8)
#define PRU_CFG_P1SD3CLK        (PRU_CFG + 0x0ac)
#define PRU_CFG_P1SD3SS         (PRU_CFG + 0x0b0)
#define PRU_CFG_P1SD4CLK        (PRU_CFG + 0x0b4)
#define PRU_CFG_P1SD4SS         (PRU_CFG + 0x0b8)
#define PRU_CFG_P1SD5CLK        (PRU_CFG + 0x0bc)
#define PRU_CFG_P1SD5SS         (PRU_CFG + 0x0c0)
#define PRU_CFG_P1SD6CLK        (PRU_CFG + 0x0c4)
#define PRU_CFG_P1SD6SS         (PRU_CFG + 0x0c8)
#define PRU_CFG_P1SD7CLK        (PRU_CFG + 0x0cc)
#define PRU_CFG_P1SD7SS         (PRU_CFG + 0x0d0)
#define PRU_CFG_P1SD8CLK        (PRU_CFG + 0x0d4)
#define PRU_CFG_P1SD8SS         (PRU_CFG + 0x0d8)
#define PRU_CFG_EDRXCFG(P)      (P0EDRXCFG + (P) * 0x20)
#define PRU_CFG_P0EDRXCFG       (PRU_CFG + 0x0e0)
#define PRU_CFG_EDTXCFG(P)      (P0EDTXCFG + (P) * 0x20)
#define PRU_CFG_P0EDTXSCFG      (PRU_CFG + 0x0e4)
#define PRU_CFG_ED0CFG(P, E)     (P0ED0CFG0 + (P) * 0x20 + (E) * 4)
#define PRU_CFG_P0ED0CFG0       (PRU_CFG + 0x0e8)
#define PRU_CFG_P0ED0CFG1       (PRU_CFG + 0x0ec)
#define PRU_CFG_P1EDRXCFG       (PRU_CFG + 0x100)
#define PRU_CFG_P1EDTXSCFG      (PRU_CFG + 0x104)
#define PRU_CFG_P1ED0CFG0       (PRU_CFG + 0x108)
#define PRU_CFG_P1ED0CFG1       (PRU_CFG + 0x10c)

#define PRU_CFG_CGR_ICSS_STOP_ACK       BIT(31)
#define PRU_CFG_CGR_ICSS_STOP_REQ       BIT(30)
#define PRU_CFG_CGR_IEP_CLK_EN          BIT(17)
#define PRU_CFG_CGR_IEP_CLK_STOP_ACK    BIT(16)
#define PRU_CFG_CGR_IEP_CLK_STOP_REQ    BIT(15)
#define PRU_CFG_CGR_ECAP_CLK_EN         BIT(14)
#define PRU_CFG_CGR_ECAP_CLK_STOP_ACK   BIT(13)
#define PRU_CFG_CGR_ECAP_CLK_STOP_REQ   BIT(12)
#define PRU_CFG_CGR_UART_CLK_EN         BIT(11)
#define PRU_CFG_CGR_UART_CLK_STOP_ACK   BIT(10)
#define PRU_CFG_CGR_UART_CLK_STOP_REQ   BIT(9)
#define PRU_CFG_CGR_INTC_CLK_EN         BIT(8)
#define PRU_CFG_CGR_INTC_CLK_STOP_ACK   BIT(7)
#define PRU_CFG_CGR_INTC_CLK_STOP_REQ   BIT(6)
#define PRU_CFG_CGR_PRU1_CLK_EN         BIT(5)
#define PRU_CFG_CGR_PRU1_CLK_STOP_ACK   BIT(4)
#define PRU_CFG_CGR_PRU1_CLK_STOP_REQ   BIT(3)
#define PRU_CFG_CGR_PRU0_CLK_EN         BIT(2)
#define PRU_CFG_CGR_PRU0_CLK_STOP_ACK   BIT(1)
#define PRU_CFG_CGR_PRU0_CLK_STOP_REQ   BIT(0)

struct drv;
extern const struct drv icss_drv;

#endif
