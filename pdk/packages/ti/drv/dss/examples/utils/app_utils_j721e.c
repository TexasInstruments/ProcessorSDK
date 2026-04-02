/*
 *  Copyright (c) Texas Instruments Incorporated 2022
 *  All rights reserved.
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
 */

/**
 *  \file app_utils_j721e.c
 *
 *  \brief DSS example utility APIs for J7
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/board/src/j721e_evm/include/board_control.h>
#include <ti/csl/soc/cslr_soc_ctrl_mmr.h>
#include <ti/drv/sciclient/sciclient.h>
#include "app_utils.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void App_configureSoC(void)
{
    /* Set drive strength */
    CSL_REG32_WR(CSL_WKUP_CTRL_MMR0_CFG0_BASE +
                 CSL_WKUP_CTRL_MMR_CFG0_H_IO_DRVSTRNGTH0_PROXY, 0xFU);

    CSL_REG32_WR(CSL_WKUP_CTRL_MMR0_CFG0_BASE +
                 CSL_WKUP_CTRL_MMR_CFG0_V_IO_DRVSTRNGTH0_PROXY, 0xFU);
}

void App_configureLCD(App_utilsLcdCfgParams cfgParams)
{
    int32_t status = CSL_PASS;

    if(APP_OUTPUT_HDMI == cfgParams.outType)
    {
        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleClkParent(TISCI_DEV_DSS0,
                    TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK,
                    TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK_PARENT_DPI0_EXT_CLKSEL_OUT0,
                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
                    TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                    TISCI_MSG_FLAG_AOP,
                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if(CSL_PASS == status)
        {
            status = Sciclient_pmModuleClkRequest(TISCI_DEV_DSS0,
                    TISCI_DEV_DSS0_DSS_FUNC_CLK,
                    TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                    0,
                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleClkFreq(TISCI_DEV_DSS0,
                    TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK,
                    cfgParams.pixelClk,
                    0,
                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if(CSL_PASS == status)
        {
            status = Sciclient_pmModuleClkRequest(TISCI_DEV_DSS0,
                    TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK,
                    TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                    0,
                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        if(CSL_PASS == status)
        {
            Board_control(BOARD_CTRL_CMD_SET_HDMI_MUX, (void*) 0U);
            Board_control(BOARD_CTRL_CMD_SET_HDMI_PD_HIGH, (void*) 0U);
        }
    }
    else if (APP_OUTPUT_DSI == cfgParams.outType)
    {
        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleState(
                TISCI_DEV_DSS_DSI0, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleState(
                TISCI_DEV_DPHY_TX0, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
                    TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                    TISCI_MSG_FLAG_AOP,
                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        if(CSL_PASS == status)
        {
            status = Sciclient_pmModuleClkRequest(TISCI_DEV_DSS0,
                    TISCI_DEV_DSS0_DSS_FUNC_CLK,
                    TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                    0,
                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        if (CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleClkParent(TISCI_DEV_DSS0,
                TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK,
                TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_18_HSDIVOUT0_CLK,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        if (CSL_PASS == status)
        {
            /* Set the clock at the desirable frequency*/
            status = Sciclient_pmSetModuleClkFreq(TISCI_DEV_DSS0,
                TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK,
                cfgParams.pixelClk,
                TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }
    }
    else
    {
        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleState(TISCI_DEV_SERDES_10G0,
                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                TISCI_MSG_FLAG_AOP,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleState(TISCI_DEV_DSS_EDP0,
                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                TISCI_MSG_FLAG_AOP,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                TISCI_MSG_FLAG_AOP,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleClkParent(TISCI_DEV_DSS0,
                TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK,
                TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_18_HSDIVOUT0_CLK,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleClkParent(TISCI_DEV_DSS0,
                TISCI_DEV_DSS0_DSS_INST0_DPI_3_IN_2X_CLK,
                TISCI_DEV_DSS0_DSS_INST0_DPI_3_IN_2X_CLK_PARENT_DPI1_EXT_CLKSEL_OUT0,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleClkParent(TISCI_DEV_DSS0,
                TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK,
                TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleClkFreq(TISCI_DEV_DSS0,
                TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK,
                cfgParams.pixelClk,
                0U,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if(CSL_PASS == status)
        {
            status = Sciclient_pmModuleClkRequest(TISCI_DEV_DSS0,
                TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK,
                TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                0U,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }
    }

}
