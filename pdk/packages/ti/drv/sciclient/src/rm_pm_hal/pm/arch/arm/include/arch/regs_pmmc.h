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

#ifndef ARCH_REGS_PMMC_H
#define ARCH_REGS_PMMC_H

#define PMMC_CONTROL_BASE       0x44030000
#define PMMC_TIMER1_BASE        0x44033000
#define PMMC_TIMER2_BASE        0x44034000
#define PMMC_WWDT_BASE          0x44035000

#define PMMC_MAPPING0_BASE      0x60000000UL
#define PMMC_MAPPING1_BASE      0x70000000UL
#define PMMC_MAPPING2_BASE      0x80000000UL
#define PMMC_MAPPING3_BASE      0x90000000UL
#define PMMC_MAPPING4_BASE      0xa0000000UL
#define PMMC_MAPPING5_BASE      0xb0000000UL
#define PMMC_MAPPING6_BASE      0xc0000000UL
#define PMMC_MAPPING7_BASE      0xd0000000UL

#define PMMC_MAPPING_SHIFT      28

#define PMMC_EXT_REG_BASE                       (0x000000000000 >> \
						 (PMMC_MAPPING_SHIFT))

#define PMMC_EXT_REG(o)                         ((o) + (PMMC_MAPPING0_BASE))

#define PMMC_CTRL_REVISION                      (PMMC_CONTROL_BASE + 0x0000)
#define PMMC_CTRL_LOCK0                         (PMMC_CONTROL_BASE + 0x0020)
#define PMMC_CTRL_LOCK1                         (PMMC_CONTROL_BASE + 0x0024)
#define PMMC_CTRL_LOCK2                         (PMMC_CONTROL_BASE + 0x0028)
#define PMMC_CTRL_FW_CTRL_OUT0                  (PMMC_CONTROL_BASE + 0x0040)
#define PMMC_CTRL_FW_CTRL_OUT0_SET              (PMMC_CONTROL_BASE + 0x0044)
#define PMMC_CTRL_FW_CTRL_OUT0_CLR              (PMMC_CONTROL_BASE + 0x0048)
#define PMMC_CTRL_FW_STS_IN0                    (PMMC_CONTROL_BASE + 0x004c)
#define PMMC_CTRL_FW_CTRL_OUT1                  (PMMC_CONTROL_BASE + 0x0050)
#define PMMC_CTRL_FW_CTRL_OUT1_SET              (PMMC_CONTROL_BASE + 0x0054)
#define PMMC_CTRL_FW_CTRL_OUT1_CLR              (PMMC_CONTROL_BASE + 0x0058)
#define PMMC_CTRL_FW_STS_IN1                    (PMMC_CONTROL_BASE + 0x005c)
#define PMMC_CTRL_PMCTRL_SYS_ULPM_EN            BIT(0)
#define PMMC_CTRL_PMCTRL_SYS                    (PMMC_CONTROL_BASE + 0x0080)
#define PMMC_CTRL_PMCTRL_IO                     (PMMC_CONTROL_BASE + 0x0084)
#define PMMC_CTRL_PMCTRL_DDR                    (PMMC_CONTROL_BASE + 0x0088)
#define PMMC_CTRL_PMCTRL_MOSC_PWRDWN_ENABLE     BIT(16)
#define PMMC_CTRL_PMCTRL_MOSC                   (PMMC_CONTROL_BASE + 0x008c)
#define PMMC_CTRL_PMCTRL_PMMC                   (PMMC_CONTROL_BASE + 0x0090)
#define PMMC_CTRL_CPU_STCALIB                   (PMMC_CONTROL_BASE + 0x0140)
#define PMMC_CTRL_WKUP0_EN                      (PMMC_CONTROL_BASE + 0x0180)
#define PMMC_CTRL_WKUP1_EN                      (PMMC_CONTROL_BASE + 0x0184)
#define PMMC_CTRL_WKUP2_EN                      (PMMC_CONTROL_BASE + 0x0188)
#define PMMC_CTRL_WKUP3_EN                      (PMMC_CONTROL_BASE + 0x018c)
#define PMMC_CTRL_WKUP4_EN                      (PMMC_CONTROL_BASE + 0x0190)
#define PMMC_CTRL_WKUP5_EN                      (PMMC_CONTROL_BASE + 0x0194)
#define PMMC_CTRL_WKUP6_EN                      (PMMC_CONTROL_BASE + 0x0198)
#define PMMC_CTRL_WKUP7_EN                      (PMMC_CONTROL_BASE + 0x019c)
#define PMMC_CTRL_MADDR_BASE(N)                 (PMMC_CONTROL_BASE + 0x01a0 + \
						 (N) * 4)
#define PMMC_CTRL_MADDR_BASE0                   (PMMC_CONTROL_BASE + 0x01a0)
#define PMMC_CTRL_MADDR_BASE1                   (PMMC_CONTROL_BASE + 0x01a4)
#define PMMC_CTRL_MADDR_BASE2                   (PMMC_CONTROL_BASE + 0x01a8)
#define PMMC_CTRL_MADDR_BASE3                   (PMMC_CONTROL_BASE + 0x01ac)
#define PMMC_CTRL_MADDR_BASE4                   (PMMC_CONTROL_BASE + 0x01b0)
#define PMMC_CTRL_MADDR_BASE5                   (PMMC_CONTROL_BASE + 0x01b4)
#define PMMC_CTRL_MADDR_BASE6                   (PMMC_CONTROL_BASE + 0x01b8)
#define PMMC_CTRL_MADDR_BASE7                   (PMMC_CONTROL_BASE + 0x01bc)
#define PMMC_CTRL_SPARE0                        (PMMC_CONTROL_BASE + 0x0400)
#define PMMC_CTRL_SPARE1                        (PMMC_CONTROL_BASE + 0x0404)
#define PMMC_CTRL_SPARE2                        (PMMC_CONTROL_BASE + 0x0408)
#define PMMC_CTRL_LOCK3                         (PMMC_CONTROL_BASE + 0x1000)
#define PMMC_CTRL_LOCK4                         (PMMC_CONTROL_BASE + 0x1004)
#define PMMC_CTRL_LOCK5                         (PMMC_CONTROL_BASE + 0x1008)
#define PMMC_CTRL_INTR_EOI                      (PMMC_CONTROL_BASE + 0x101c)
#define PMMC_CTRL_INTR0_OUT_IRQSTATUS_RAW       (PMMC_CONTROL_BASE + 0x1020)
#define PMMC_CTRL_INTR0_OUT_IRQSTATUS           (PMMC_CONTROL_BASE + 0x1024)
#define PMMC_CTRL_INTR0_OUT_IRQENABLE_SET       (PMMC_CONTROL_BASE + 0x1028)
#define PMMC_CTRL_INTR0_OUT_IRQENABLE_CLR       (PMMC_CONTROL_BASE + 0x102c)
#define PMMC_CTRL_INTR1_OUT_IRQSTATUS_RAW       (PMMC_CONTROL_BASE + 0x1030)
#define PMMC_CTRL_INTR1_OUT_IRQSTATUS           (PMMC_CONTROL_BASE + 0x1034)
#define PMMC_CTRL_INTR1_OUT_IRQENABLE_SET       (PMMC_CONTROL_BASE + 0x1038)
#define PMMC_CTRL_INTR1_OUT_IRQENABLE_CLR       (PMMC_CONTROL_BASE + 0x103c)
#define PMMC_CTRL_INTR_FAULTDET_IRQSTATUS_RAW   (PMMC_CONTROL_BASE + 0x1080)
#define PMMC_CTRL_INTR_FAULTDET_IRQSTATUS       (PMMC_CONTROL_BASE + 0x1084)
#define PMMC_CTRL_INTR_FAULTDET_IRQENABLE_SET   (PMMC_CONTROL_BASE + 0x1088)
#define PMMC_CTRL_INTR_FAULTDET_IRQENABLE_CLR   (PMMC_CONTROL_BASE + 0x108c)

#define PMMC_CTRL_WKUP_EN(N)                    (PMMC_CTRL_WKUP0_EN + (N) * 4)


#endif
