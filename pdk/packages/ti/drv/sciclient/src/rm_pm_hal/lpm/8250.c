/*
 * DM Stub Firmware
 *
 * Minimal driver for UART access
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

#include <config.h>
#include <uart_config.h>

#include "lpm_io.h"

#define lpm_serial_8250_readl   readl
#define lpm_serial_8250_writel  writel

/**
 * \brief Configure UART for asynchronous transfer
 * \param uart_clk UART Clock frequency
 */
static void lpm_serial_8250_init(const u32 uart_clk)
{
	u32 val, clkdiv;

	clkdiv = uart_clk / (16U * UART_BAUD_RATE);

	/* This read operation also acts as a fence */
	val = lpm_serial_8250_readl(UART_BASE_ADDRESS + UART_16550_LCR);
	val |= UART_16550_LCR_DLAB;
	lpm_serial_8250_writel(val, (UART_BASE_ADDRESS + UART_16550_LCR));

	val = clkdiv & 0xFFU;
	lpm_serial_8250_writel(val, (UART_BASE_ADDRESS + UART_16550_DLL));
	val = (clkdiv >> 8U) & 0xFFU;
	lpm_serial_8250_writel(val, (UART_BASE_ADDRESS + UART_16550_DLH));

	/*
	 * This read operation also acts as a fence to make sure that
	 * DLL DLH values have actually been "stuck"
	 */
	val = lpm_serial_8250_readl(UART_BASE_ADDRESS + UART_16550_LCR);
	val &= (~UART_16550_LCR_DLAB);
	lpm_serial_8250_writel(val, (UART_BASE_ADDRESS + UART_16550_LCR));
	lpm_serial_8250_writel(UART_16550_LCR_WORDSZ_8, (UART_BASE_ADDRESS + UART_16550_LCR));
	lpm_serial_8250_writel(0, (UART_BASE_ADDRESS + UART_16550_IER));
	/*
	 * Force a posted read to make sure things are in-order before we
	 * enable UART via MDR1 - we ignore result
	 */
	lpm_serial_8250_readl(UART_BASE_ADDRESS + UART_16550_IER);

	lpm_serial_8250_writel(0x0, (UART_BASE_ADDRESS + UART_16550_MDR1));
	/*
	 * Force a posted read to make sure things are in-order before we
	 * enable FIFO - we ignore result
	 */
	lpm_serial_8250_readl(UART_BASE_ADDRESS + UART_16550_MDR1);

	lpm_serial_8250_writel(UART_16550_FCR_FIFOEN, (UART_BASE_ADDRESS + UART_16550_FCR));
	lpm_serial_8250_writel((UART_16550_MCR_RTS | UART_16550_MCR_DTR), (UART_BASE_ADDRESS + UART_16550_MCR));
	/*
	 * Force a posted read to make sure FIFO is enabled before we send
	 * data - we ignore result
	 */
	lpm_serial_8250_readl(UART_BASE_ADDRESS + UART_16550_MCR);
}

/**
 * \brief Put a u8 character into the UART register
 * \param data character to write
 */
static int lpm_console_tx(u8 data)
{
	u32 val;
	u32 i = 0U;

	/*
	* Poll the Line Status Register to ensure FIFO space is
	* available before writing to avoid dropping chars.
	*/
	do {
		val = lpm_serial_8250_readl(UART_BASE_ADDRESS + UART_16550_LSR);
	} while ((i++ < 10000U) && ((val & UART_16550_LSR_TX_FIFO_E) == 0U));

	lpm_serial_8250_writel(data, (UART_BASE_ADDRESS + UART_16550_THR));

	return 0;
}

int lpm_puts(char const *str)
{
	char const *start = str;

	while (*start != (char) 0) {
		lpm_console_tx(*start);
		start = start + 1;
	}
	return str - start;
}

void lpm_console_init(void)
{
	lpm_serial_8250_init(UART_CLK_FREQ);
}

void lpm_console_bypass_init(void)
{
	lpm_serial_8250_init(UART_CLK_FREQ_BYPASS_STATE);
}

void lpm_trace_debug_uart(u8 *str, u8 len)
{
	u32 i;

	/* Output "0x" at start of hex */
	lpm_console_tx('0');
	lpm_console_tx('x');

	/*
	 * Output string backwards as we converted from low
	 * digit to high
	 */
	for (i = 0U; i <= len; i++) {
		lpm_console_tx(str[len - i]);
	}

	/* Add a carriage return to support inflexible terminals. */
	lpm_console_tx('\r');

	/* Move the cursor to new line. */
	lpm_console_tx('\n');
}
