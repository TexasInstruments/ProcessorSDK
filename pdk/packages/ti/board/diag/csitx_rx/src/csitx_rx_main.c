/******************************************************************************
 * Copyright (c) 2022 Texas Instruments Incorporated - http://www.ti.com
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
 *****************************************************************************/

/**
 *  \file   csitx_rx_main.c
 *
 *  \brief  csitx_rx diagnostic test file.
 *
 *  Targeted Functionality: Verification of csitx and csirx interface.
 *
 *  Operation: This test verifies CSIRX interface by capturing the frames 
 *              transmitted from CSITX interface.
 *
 *  Test Setup: Connect CSI loopback cable from CSI-TX port to CSI-RX port on 
 *              the EVM board.
 *
 *  Supported SoCs: J784S4.
 *
 *  Supported Platforms: j784s4_evm.
 */

#include <stdint.h>
#include <stdio.h>

#include "csitx_rx_cfg.h"

#define CSITX_ESC_CLK_FREQ_HZ         (20000000U)
#define CSITX_MAIN_CLK_HZ             (500000000U)

#if defined (SOC_J721S2) || defined (SOC_J784S4)
#define CSITX_PSIL_MOD                          TISCI_DEV_CSI_PSILSS0
#if (CSITX_INSTANCE == 0U)
#define CSITX_MOD                               TISCI_DEV_CSI_TX_IF_V2_0
#define CSITX_DPHY_MOD                          TISCI_DEV_DPHY_TX0
#define CSITX_MOD_ESC_CLK                       TISCI_DEV_CSI_TX_IF_V2_0_ESC_CLK_CLK
#define CSITX_MOD_MAIN_CLK                      TISCI_DEV_CSI_TX_IF_V2_0_MAIN_CLK_CLK
#define CSITX_MOD_DPHY_TXBYTECLKHS_CLK          TISCI_DEV_CSI_TX_IF_V2_0_DPHY_TXBYTECLKHS_CL_CLK
#define CSITX_MOD_VBUS_CLK                      TISCI_DEV_CSI_TX_IF_V2_0_VBUS_CLK_CLK
#elif (CSITX_INSTANCE == 1U)
#define CSITX_MOD                               TISCI_DEV_CSI_TX_IF_V2_1
#define CSITX_DPHY_MOD                          TISCI_DEV_DPHY_TX1
#define CSITX_MOD_ESC_CLK                       TISCI_DEV_CSI_TX_IF_V2_1_ESC_CLK_CLK
#define CSITX_MOD_MAIN_CLK                      TISCI_DEV_CSI_TX_IF_V2_1_MAIN_CLK_CLK
#define CSITX_MOD_DPHY_TXBYTECLKHS_CLK          TISCI_DEV_CSI_TX_IF_V2_1_DPHY_TXBYTECLKHS_CL_CLK
#define CSITX_MOD_VBUS_CLK                      TISCI_DEV_CSI_TX_IF_V2_1_VBUS_CLK_CLK
#endif
#endif

extern int32_t BoardDiag_csitxTransmit(void);

void BoardDiag_clkRateSet(uint32_t moduleId,
                           uint32_t clkId,
                           uint64_t clkRateHz)
{
    int32_t status;
    uint64_t currClkFreqHz;

    status = PMLIBClkRateGet(moduleId, clkId, &currClkFreqHz);
    if ((FVID2_SOK == status) &&
        (currClkFreqHz != clkRateHz))
    {
        status = PMLIBClkRateSet(moduleId, clkId, clkRateHz);
        if (FVID2_SOK == status)
        {
            UART_printf("\nPMLIBClkRateSet Passed for clock Id = %d\n", clkId);
        }
        else
        {
            UART_printf("\nPMLIBClkRateSet failed for clock Id = %d\n", clkId);
        }
    }
}

int BoardDiag_CsiTxRx(void)
{
    int32_t retVal = FVID2_SOK;
    uint64_t clkFreq = 0U;

    UART_printf("\n*********************************************\n"); 
	UART_printf("*             CSI TX-RX loopback Test         *\n");
	UART_printf("***********************************************\n"); 

    UART_printf("\nRunning CSI TX-RX loopback Test...\n");

    Sciclient_init(NULL);
    
    retVal += Sciclient_pmSetModuleState(CSITX_MOD,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
    retVal += Sciclient_pmSetModuleState(CSITX_DPHY_MOD,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
    retVal += Sciclient_pmSetModuleState(CSITX_PSIL_MOD,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
#if (BOARD_DIAG_ENABLE_LOOPBACK == 1U)
    retVal += Sciclient_pmSetModuleState(TISCI_DEV_CSI_RX_IF0,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
    retVal += Sciclient_pmSetModuleState(TISCI_DEV_CSI_RX_IF1,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
    retVal += Sciclient_pmSetModuleState(TISCI_DEV_DPHY_RX0,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
    retVal += Sciclient_pmSetModuleState(TISCI_DEV_DPHY_RX1,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
#endif

    /* set CSITX clocks */
    BoardDiag_clkRateSet(CSITX_MOD,
                   CSITX_MOD_ESC_CLK,
                   CSITX_ESC_CLK_FREQ_HZ);
    BoardDiag_clkRateSet(CSITX_MOD,
                   CSITX_MOD_MAIN_CLK,
                   CSITX_MAIN_CLK_HZ);

    if (FVID2_SOK == retVal)
    {
        /* Get the CSITX clock frequencies */
        retVal += PMLIBClkRateGet(CSITX_MOD,
                                  CSITX_MOD_ESC_CLK,
                                  &clkFreq);
        retVal += PMLIBClkRateGet(CSITX_MOD,
                                  CSITX_MOD_DPHY_TXBYTECLKHS_CLK,
                                  &clkFreq);
        retVal += PMLIBClkRateGet(CSITX_MOD,
                                  CSITX_MOD_VBUS_CLK,
                                  &clkFreq);
        retVal += PMLIBClkRateGet(CSITX_MOD,
                                  CSITX_MOD_MAIN_CLK,
                                  &clkFreq);

        retVal = BoardDiag_csitxTransmit();
    }

    return retVal;
}

/**
 *  \brief   CSITX-RX Diagnostic test main function
 *
 *  \return  int - CSITX-RX Diagnostic test status.
 *             0 - in case of success
 *            -1 - in case of failure.
 */
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    int8_t ret;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO |
               BOARD_INIT_UNLOCK_MMR;;

    status = Board_init(boardCfg);
    if(BOARD_SOK != status)
    {
        return -1;
    }

    ret = BoardDiag_CsiTxRx();
    if(0 != ret)
    {
        UART_printf("\nCSI-TX-RX Test Failed\n");
        return -1;
    }
    else
    {
        UART_printf("\nCSI-TX-RX Test Passed\n");
    }

    return ret;
}
