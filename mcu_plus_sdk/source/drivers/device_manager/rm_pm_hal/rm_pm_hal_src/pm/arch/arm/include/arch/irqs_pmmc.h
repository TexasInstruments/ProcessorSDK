/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2017-2020, Texas Instruments Incorporated
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

#ifndef ARCH_IRQS_PMMC_H
#define ARCH_IRQS_PMMC_H

#define IRQ_PMMC_IO_SWAKEUP     0       /* Wakeup from IO Power Management */
#define IRQ_PMMC_TIMER1_T12     1       /* Interrupt from timer1 */
#define IRQ_PMMC_TIMER1_T34     2
#define IRQ_PMMC_TIMER2_T12     3       /* Interrupt from timer2 */
#define IRQ_PMMC_TIMER2_T34     4
#define IRQ_PMMC_FAULT_DET      5       /* Fault Detected Interrupt */
#define IRQ_PMMC_RTI_WWDT       6       /* RTI Watchdog interrupt */
#define IRQ_PMMC_RTI_REQ0       7       /* RTI Comparator interrupt 0 */
#define IRQ_PMMC_RTI_REQ1       8
#define IRQ_PMMC_RTI_REQ2       9
#define IRQ_PMMC_RTI_REQ3       10
#define IRQ_PMMC_EXT(N)         (24 + (N))      /* External interrupts */

#endif
