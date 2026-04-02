/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file		soc.c
 *
 *  \brief		SoC specific configurations done for flash programmer 
 *              are done in this file
 *
 */
 
#include <ti/csl/soc.h>
#include <ti/board/board_cfg.h>
#include <ti/board/board.h>
#include <ti/board/src/flash/include/board_flash.h>
#include <ti/osal/DebugP.h>
#include "flash_programmer.h"
#include <ti/board/src/flash/nor/qspi/nor_qspi.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/soc/UART_soc.h>

static void SOC_ConfigQSPIInputFrequency(void);
static void SOC_ConfigUARTInputFrequency(void);
static void SOC_ConfigCPUFreq(void);

uint8_t uart_inst = BOARD_UART_INSTANCE;
uint32_t uart_baseAddr = CSL_MSS_SCIA_U_BASE;

/**
 * @brief	This function initializes settings based on soc.
 *
 * \param   cfg		[IN]	Board initializing value
 *
 * @return	int8_t
 *      0 	- Init completed successfully
 *     -1 	- Error occurred
 *
 */
int8_t UFP_socInit(Board_initCfg *cfg)
{
    Board_initCfg boardCfg;

    if (cfg == NULL)
    {
        boardCfg = BOARD_INIT_PINMUX_CONFIG;
    }
    else
    {
        boardCfg = *cfg;
    }

    /* Board Library Init. */
    if (Board_init(boardCfg))
    {
        return -1;
    }
    SOC_ConfigCPUFreq();
    SOC_ConfigQSPIInputFrequency();
    SOC_ConfigUARTInputFrequency();
    UFP_uartConfig(UFP_BAUDRATE_115200);

    return 0;
}

/**
* FROM     ROW   Fields               Start Bit   End Bit  # of bits
* FROM1    43    BOOT_FREQUENCY_SELECT   8          11       4 
*/
#define SBL_EFUSE_FIELD_EXTRACT(topCtrlReg, fromNum, row, startBit, endBit)  CSL_extract8((topCtrlReg)->EFUSE##fromNum##_ROW_##row, endBit, startBit)
#define SBL_EFUSE_FIELD_EXTRACT_BOOT_FREQ(topCtrlReg)                SBL_EFUSE_FIELD_EXTRACT(topCtrlReg, 1, 43,  8, 11)
#define RCM_EFUSE_R5_CLK_300_SYS_CLK_150MHz                               (0x0U)
#define RCM_EFUSE_R5_CLK_200_SYS_CLK_200MHz                               (0x5U)
#define RCM_EFUSE_R5_CLK_400_SYS_CLK_200MHz                               (0xAU)
#define SBL_FREQ_MHZ2HZ(x)          ((x) * 1000 * 1000)
#define SBL_FREQ_HZ2MHZ(x)          ((x) / (1000 * 1000))

static uint8_t CSL_extract8 (volatile uint32_t reg, uint8_t msb, uint8_t lsb)
{
    uint32_t    mask;
    uint8_t     bits;
    uint8_t     value;

    /* Compute the mask: */
    bits = (msb - lsb + 1U);
    mask = (uint32_t)((uint32_t)1U << bits);
    mask = mask - 1U;

    value = (uint8_t)((reg >> lsb) & mask);
    return value;
}


static CSL_top_ctrlRegs* CSL_TopCtrl_getBaseAddress (void)
{
    return (CSL_top_ctrlRegs*)CSL_TOP_CTRL_U_BASE;
}

static uint8_t CSL_TopCtrl_readR5ClkFreqEfuse(const CSL_top_ctrlRegs * ptrTopCtrlRegs)
{
    return (SBL_EFUSE_FIELD_EXTRACT_BOOT_FREQ(ptrTopCtrlRegs));
}

static uint32_t SOC_RcmGetSysclkFrequency(void)
{
    CSL_top_ctrlRegs* ptrTopCtrlRegs;
    uint8_t bootFreqValEfuse;
    uint32_t sysClkFreq = 0;

    ptrTopCtrlRegs = CSL_TopCtrl_getBaseAddress ();

    bootFreqValEfuse = CSL_TopCtrl_readR5ClkFreqEfuse (ptrTopCtrlRegs);
    switch (bootFreqValEfuse)
    {
        case RCM_EFUSE_R5_CLK_300_SYS_CLK_150MHz:
        {
            sysClkFreq = SBL_FREQ_MHZ2HZ(150U);
            break;
        }
        case RCM_EFUSE_R5_CLK_200_SYS_CLK_200MHz:
        {
            sysClkFreq = SBL_FREQ_MHZ2HZ(200U);
            break;
        }
        case RCM_EFUSE_R5_CLK_400_SYS_CLK_200MHz:
        {
            sysClkFreq = SBL_FREQ_MHZ2HZ(200U);
            break;
        }
        default:
            DebugP_assert(FALSE);
    }
    return sysClkFreq;
}

static void SOC_ConfigUARTInputFrequency(void)
{
    UART_HwAttrs uartCfg;
    
    UART_socGetInitCfg(uart_inst, &uartCfg);
    uartCfg.clockFrequency = SOC_RcmGetSysclkFrequency();
    UART_socSetInitCfg(uart_inst, &uartCfg);
}

static void SOC_ConfigQSPIInputFrequency(void)
{
    QSPI_HwAttrs qspi_cfg;
    uint32_t sysClkFreq;

    /* Get the default QSPI init configurations */
    QSPI_socGetInitCfg(BOARD_QSPI_NOR_INSTANCE, &qspi_cfg);

    sysClkFreq = SOC_RcmGetSysclkFrequency();
    if (SBL_FREQ_MHZ2HZ(200U) == sysClkFreq)
    {
        qspi_cfg.funcClk = SBL_FREQ_MHZ2HZ(80U);
    }
    else
    {
        qspi_cfg.funcClk = SBL_FREQ_MHZ2HZ(60U);
    }
    /* Set the default QSPI init configurations */
    QSPI_socSetInitCfg(BOARD_QSPI_NOR_INSTANCE, &qspi_cfg);
}

static void SOC_ConfigCPUFreq(void)
{
    CSL_top_ctrlRegs* ptrTopCtrlRegs;
    uint8_t bootFreqValEfuse;
    uint32_t cpuFreq = 0;

    ptrTopCtrlRegs = CSL_TopCtrl_getBaseAddress ();

    bootFreqValEfuse = CSL_TopCtrl_readR5ClkFreqEfuse (ptrTopCtrlRegs);
    switch (bootFreqValEfuse)
    {
        case RCM_EFUSE_R5_CLK_300_SYS_CLK_150MHz:
        {
            cpuFreq = SBL_FREQ_MHZ2HZ(300U);
            break;
        }
        case RCM_EFUSE_R5_CLK_200_SYS_CLK_200MHz:
        {
            cpuFreq = SBL_FREQ_MHZ2HZ(200U);
            break;
        }
        case RCM_EFUSE_R5_CLK_400_SYS_CLK_200MHz:
        {
            cpuFreq = SBL_FREQ_MHZ2HZ(400U);
            break;
        }
        default:
            DebugP_assert(FALSE);
    }
    gOsal_HwAttrs.cpuFreqKHz = (cpuFreq / 1000U);
}

/* Returns the default frquency lower 32 bits */
int32_t TimerP_getDefaultFreqLo(uint32_t timerId)
{
    /* Workaround to resolve the Misra-C 2012 Rule 2.7 issue */
    (void)timerId;

    /* return the same default frequency for all timer Ids */
    return(SOC_RcmGetSysclkFrequency());
}
