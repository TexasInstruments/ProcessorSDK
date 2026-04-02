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

#ifndef ARCH_REGS_CM3_H
#define ARCH_REGS_CM3_H

#define SYS_CONTROL_BASE        0xE000E000U

#define SYS_ICTR                (SYS_CONTROL_BASE + 0x004U)
#define SYS_ACTLR               (SYS_CONTROL_BASE + 0x008U)
#define SYS_STCSR               (SYS_CONTROL_BASE + 0x010U)
#define SYS_STRVR               (SYS_CONTROL_BASE + 0x014U)
#define SYS_STCVR               (SYS_CONTROL_BASE + 0x018U)
#define SYS_STCR                (SYS_CONTROL_BASE + 0x01cU)
#define NVIC_ISER(N)            (SYS_CONTROL_BASE + 0x100U + ((unsigned) (N)) * 4U)
#define NVIC_ICER(N)            (SYS_CONTROL_BASE + 0x180U + ((unsigned) (N)) * 4U)
#define NVIC_ISPR(N)            (SYS_CONTROL_BASE + 0x200U + ((unsigned) (N)) * 4U)
#define NVIC_ICPR(N)            (SYS_CONTROL_BASE + 0x280U + ((unsigned) (N)) * 4U)
#define NVIC_IABR(N)            (SYS_CONTROL_BASE + 0x300U + ((unsigned) (N)) * 4U)
#define NVIC_IPR(N)             (SYS_CONTROL_BASE + 0x400U + ((unsigned) (N)) * 4U)
#define SYS_CPUID               (SYS_CONTROL_BASE + 0xd00U)
#define SYS_ICSR                (SYS_CONTROL_BASE + 0xd04U)
#define SYS_VTOR                (SYS_CONTROL_BASE + 0xd08U)
#define SYS_AIRCR               (SYS_CONTROL_BASE + 0xd0cU)

#define SYS_SCR                 (SYS_CONTROL_BASE + 0xd10U)
#define SYS_SCR_SD_OFFSET       0x2
#define SYS_SCR_SOE_OFFSET      0x1

#define SYS_CCR                 (SYS_CONTROL_BASE + 0xd14U)
#define SYS_SHPR1               (SYS_CONTROL_BASE + 0xd18U)
#define SYS_SHPR2               (SYS_CONTROL_BASE + 0xd1cU)
#define SYS_SHPR3               (SYS_CONTROL_BASE + 0xd20U)
#define SYS_SHCSR               (SYS_CONTROL_BASE + 0xd24U)
#define SYS_CFSR                (SYS_CONTROL_BASE + 0xd28U)
#define SYS_HFSR                (SYS_CONTROL_BASE + 0xd2cU)
#define SYS_DFSR                (SYS_CONTROL_BASE + 0xd30U)
#define SYS_MMFAR               (SYS_CONTROL_BASE + 0xd34U)
#define SYS_BFAR                (SYS_CONTROL_BASE + 0xd38U)
#define SYS_AFSR                (SYS_CONTROL_BASE + 0xd3cU)
#define SYS_PFR0                (SYS_CONTROL_BASE + 0xd40U)
#define SYS_PFR1                (SYS_CONTROL_BASE + 0xd44U)
#define SYS_DFR0                (SYS_CONTROL_BASE + 0xd48U)
#define SYS_AFR0                (SYS_CONTROL_BASE + 0xd4cU)
#define SYS_MMFR0               (SYS_CONTROL_BASE + 0xd50U)
#define SYS_MMFR1               (SYS_CONTROL_BASE + 0xd54U)
#define SYS_MMFR2               (SYS_CONTROL_BASE + 0xd58U)
#define SYS_MMFR3               (SYS_CONTROL_BASE + 0xd5cU)
#define SYS_ISAR0               (SYS_CONTROL_BASE + 0xd60U)
#define SYS_ISAR1               (SYS_CONTROL_BASE + 0xd64U)
#define SYS_ISAR2               (SYS_CONTROL_BASE + 0xd68U)
#define SYS_ISAR3               (SYS_CONTROL_BASE + 0xd6cU)
#define SYS_ISAR4               (SYS_CONTROL_BASE + 0xd70U)
#define SYS_CPACR               (SYS_CONTROL_BASE + 0xd88U)
#define SYS_STIR                (SYS_CONTROL_BASE + 0xf00U)

void scr_enable_sleepdeep(void);
void scr_enable_sleeponexit(void);

#endif
