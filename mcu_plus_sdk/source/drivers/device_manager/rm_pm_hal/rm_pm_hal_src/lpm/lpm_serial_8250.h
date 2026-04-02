/*
 * DM Stub Firmware
 *
 * Minimal driver for UART access
 *
 * Copyright (C) 2021-2023, Texas Instruments Incorporated
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

/* Register Definitions */

#define UART_16550_THR                  0x00U
#define UART_16550_RHR                  0x00U
#define UART_16550_DLL                  0x00U

#define UART_16550_IER                  0x04U
#define UART_16550_DLH                  0x04U

#define UART_16550_IIR                  0x08U
#define UART_16550_FCR                  0x08U

#define UART_16550_LCR                  0x0CU

#define UART_16550_MCR                  0x10U

#define UART_16550_LSR                  0x14U
#define UART_16550_MSR_TCR              0x18U
#define UART_16550_SPR_TLR              0x1CU
#define UART_16550_MDR1                 0x20U
#define UART_16550_MDR2                 0x24U
#define UART_16550_BLR_UASR             0x38U
#define UART_16550_ACREG                0x3CU
#define UART_16550_SCR                  0x40U
#define UART_16550_SSR                  0x44U
#define UART_16550_EBLR                 0x48U
#define UART_16550_MVR                  0x50U
#define UART_16550_SYSC                 0x54U
#define UART_16550_SYSS                 0x58U
#define UART_16550_WER                  0x5CU
#define UART_16550_CFPS                 0x60U
#define UART_16550_RXFIFO_LVL           0x64U
#define UART_16550_TXFIFO_LVL           0x68U
#define UART_16550_IER2                 0x6CU
#define UART_16550_ISR2                 0x70U
#define UART_16550_MDR3                 0x80U
#define UART_16550_TDT                  0x84U
#define UART_16550_MDR4                 0x88U
#define UART_16550_EFR2                 0x8CU
#define UART_16550_ECR                  0x90U
#define UART_16550_SCCR                 0xA0U
#define UART_16550_ERHR_ETHR            0xA4U
#define UART_16550_MAR                  0xA8U
#define UART_16550_MMR                  0xACU
#define UART_16550_MBR                  0xB0U

/* Line Control Register bits */
#define UART_16550_LCR_DLAB             (1U << 7U)      /* Divisor Latch Access */
#define UART_16550_LCR_SETB             (1U << 6U)      /* Set BREAK Condition */
#define UART_16550_LCR_SETP             (1U << 5U)      /* Set Parity to LCR[4] */
#define UART_16550_LCR_EVEN             (1U << 4U)      /* Even Parity Format */
#define UART_16550_LCR_PAR              (1U << 3U)      /* Parity */
#define UART_16550_LCR_STOP             (1U << 2U)      /* Stop Bit */
#define UART_16550_LCR_WORDSZ_5         0               /* Word Length of 5 */
#define UART_16550_LCR_WORDSZ_6         1               /* Word Length of 6 */
#define UART_16550_LCR_WORDSZ_7         2               /* Word Length of 7 */
#define UART_16550_LCR_WORDSZ_8         3               /* Word Length of 8 */

/* FIFO Control Register bits */
#define UART_16550_FCR_RXTRIG_8         (0U << 6U)
#define UART_16550_FCR_RXTRIG_16        (1U << 6U)
#define UART_16550_FCR_RXTRIG_56        (2U << 6U)
#define UART_16550_FCR_RXTRIG_60        (3U << 6U)
#define UART_16550_FCR_TXTRIG_8         (0U << 4U)
#define UART_16550_FCR_TXTRIG_16        (1U << 4U)
#define UART_16550_FCR_TXTRIG_56        (2U << 4U)
#define UART_16550_FCR_TXTRIG_60        (3U << 4U)
#define UART_16550_FCR_DMAEN            (1U << 3U)              /* Enable DMA mode */
#define UART_16550_FCR_TXCLR            (1U << 2U)              /* Clear contents of Tx FIFO */
#define UART_16550_FCR_RXCLR            (1U << 1U)              /* Clear contents of Rx FIFO */
#define UART_16550_FCR_FIFOEN           (1U << 0U)              /* Enable the Tx/Rx FIFO */

/* Line Status Register bits */
#define UART_16550_LSR_SR_E             (1U << 6U)              /* Shift register empty */
#define UART_16550_LSR_TX_FIFO_E        (1U << 5U)              /* TX FIFO empty */

/* Modem Control Register bits */
#define UART_16550_MCR_RTS              (1U << 0U)              /* Request to Send */
#define UART_16550_MCR_DTR              (1U << 1U)              /* Data Terminal Ready */

#ifdef CONFIG_LPM_DM_TRACE_UART
int lpm_puts(const char *str);
/**
 * \brief Configure UART to print at clock frequency
 */
void lpm_console_init(void);
/**
 * \brief Configure UART to print at clock bypass frequency
 */
void lpm_console_bypass_init(void);
/**
 * \brief Print the input buffer at UART console with "0x" prepended to indicate hexadecimal representation
 * \param str Pointer to location where input debug buffer is placed
 * \param len Length of buffer to print
 */
void lpm_trace_debug_uart(u8 *str, u8 len);
#else
static inline int lpm_puts(const char *str __attribute__((unused)))
{
	return 0;
}
static inline void lpm_console_init(void)
{
	return;
}
static inline void lpm_console_bypass_init(void)
{
	return;
}
static inline void lpm_trace_debug_uart(u8 *str __attribute__((unused)), u8 len __attribute__((unused)))
{
	return;
}
#endif
