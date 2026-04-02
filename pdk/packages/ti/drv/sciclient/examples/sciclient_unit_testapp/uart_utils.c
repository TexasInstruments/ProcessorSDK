/*
 *  Copyright (C) 2018-2023 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file uart_utils.c
 *
 *  \brief Contains implementation of UART functions.
 *
 */
#include <ti/drv/sciclient/examples/sciclient_unit_testapp/uart_utils.h>

uint16_t App_getUartSrcID(uint32_t baseAddr)
{
    uint16_t srcID = UART_TISCI_INVALID_DEV_ID;

    switch (baseAddr)
    {
        case (uint32_t)CSL_UART0_BASE:
            srcID = TISCI_DEV_UART0;
            break;
        case (uint32_t)CSL_UART1_BASE:
            srcID = TISCI_DEV_UART1;
            break;
        case (uint32_t)CSL_UART2_BASE:
            srcID = TISCI_DEV_UART2;
            break;
        case (uint32_t)CSL_UART3_BASE:
            srcID = TISCI_DEV_UART3;
            break;
        case (uint32_t)CSL_UART4_BASE:
            srcID = TISCI_DEV_UART4;
            break;
        case (uint32_t)CSL_UART5_BASE:
            srcID = TISCI_DEV_UART5;
            break;
        case (uint32_t)CSL_UART6_BASE:
            srcID = TISCI_DEV_UART6;
            break;
        case (uint32_t)CSL_UART7_BASE:
            srcID = TISCI_DEV_UART7;
            break;
        case (uint32_t)CSL_UART8_BASE:
            srcID = TISCI_DEV_UART8;
            break;
        case (uint32_t)CSL_UART9_BASE:
            srcID = TISCI_DEV_UART9;
            break;
        case (uint32_t)CSL_MCU_UART0_BASE:
			srcID = TISCI_DEV_MCU_UART0;
            break;
        case (uint32_t)CSL_WKUP_UART0_BASE:
			srcID = TISCI_DEV_WKUP_UART0;
            break;
        default:
            srcID = UART_TISCI_INVALID_DEV_ID;
            break;
    }
    return srcID;
}

#if defined(SOC_J7200)
uint32_t App_getMainUartTestInstance()
{
    uint32_t mainUartPinmuxData[]={9,7,6,5,3,1,0};
    return mainUartPinmuxData[0];
}
void App_setMainUartHwAttrs(uint32_t uartInst, struct UART_HWAttrs *uartHwAttrs)
{
    uint32_t i = uartInst;
    (*uartHwAttrs).baseAddr = (uint32_t)CSL_UART0_BASE + (0x10000U * i);
    (*uartHwAttrs).frequency        = UART_INPUT_CLK_48M;
    (*uartHwAttrs).rxDmaEventNumber = CSL_PDMA_CH_MAIN_UART0_CH0_RX + i;
    (*uartHwAttrs).txDmaEventNumber = CSL_PDMA_CH_MAIN_UART0_CH0_TX + i;
}
#endif

#if defined(SOC_J721E)
uint32_t App_getMainUartTestInstance()
{
    uint32_t mainUartPinmuxData[]={4,2,1,0};
    return mainUartPinmuxData[0];
}
void App_setMainUartHwAttrs(uint32_t uartInst, struct UART_HWAttrs *uartHwAttrs)
{
    uint32_t i = uartInst;
    (*uartHwAttrs).baseAddr = (uint32_t)CSL_UART0_BASE + (0x10000U * i);
    (*uartHwAttrs).frequency        = UART_INPUT_CLK_48M;
    (*uartHwAttrs).rxDmaEventNumber = CSL_PDMA_CH_MAIN_UART0_CH0_RX + i;
    (*uartHwAttrs).txDmaEventNumber = CSL_PDMA_CH_MAIN_UART0_CH0_TX + i;
}
#endif

#if defined(SOC_J721S2)
uint32_t App_getMainUartTestInstance()
{
    uint32_t mainUartPinmuxData[]={9,8,6,5,3,2};
    return mainUartPinmuxData[0];
}
void App_setMainUartHwAttrs(uint32_t uartInst, struct UART_HWAttrs *uartHwAttrs)
{
    uint32_t i = uartInst;
    (*uartHwAttrs).baseAddr = (uint32_t)CSL_UART0_BASE + (0x10000U * i);
    (*uartHwAttrs).frequency        = UART_INPUT_CLK_48M;
    (*uartHwAttrs).rxDmaEventNumber = CSL_PDMA_CH_MAIN_UART0_CH0_RX + i;
    (*uartHwAttrs).txDmaEventNumber = CSL_PDMA_CH_MAIN_UART0_CH0_TX + i;
}
#endif

#if defined(SOC_J784S4) || defined(SOC_J742S2)
uint32_t App_getMainUartTestInstance()
{
    uint32_t mainUartPinmuxData[]={9,8,6,5,3,2};
    return mainUartPinmuxData[0];
}
void App_setMainUartHwAttrs(uint32_t uartInst, struct UART_HWAttrs *uartHwAttrs)
{
    uint32_t i = uartInst;
    (*uartHwAttrs).baseAddr = (uint32_t)CSL_UART0_BASE + (0x10000U * i);
    (*uartHwAttrs).frequency        = UART_INPUT_CLK_48M;
    (*uartHwAttrs).rxDmaEventNumber = CSL_PDMA_CH_MAIN_UART0_CH0_RX + i;
    (*uartHwAttrs).txDmaEventNumber = CSL_PDMA_CH_MAIN_UART0_CH0_TX + i;
}
#endif
