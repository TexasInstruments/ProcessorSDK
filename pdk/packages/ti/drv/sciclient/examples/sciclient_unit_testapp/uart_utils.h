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
 *  \file uart_utils.h
 *
 *  \brief This file contains the header files required for uart_utils.c.
 */

#ifndef UART_UTILS_H_
#define UART_UTILS_H_

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/soc/UART_soc.h>
#include <ti/board/board_cfg.h>
#include <ti/drv/sciclient/sciclient.h>
#if defined(SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_utils.h>
#elif defined(SOC_J7200)
#include <ti/board/src/j7200_evm/include/board_utils.h>
#elif defined(SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_utils.h>
#elif defined(SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_utils.h>
#elif defined(SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_utils.h>
#endif

#define UART_INPUT_CLK_48M              (48000000U)
#define UART_TISCI_INVALID_DEV_ID       (0xFFFFU)

uint16_t App_getUartSrcID(uint32_t baseAddr);
uint32_t App_getMainUartTestInstance();
void App_setMainUartHwAttrs(uint32_t uartInst, struct UART_HWAttrs *UART_HwAttrs);

#endif /* #ifndef UART_UTILS_H_ */
