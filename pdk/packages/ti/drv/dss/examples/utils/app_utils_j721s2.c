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
 *  \file app_utils_j721s2.c
 *
 *  \brief DSS example utility APIs for J7
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/board/src/j721s2_evm/include/board_control.h>
#include <ti/csl/soc/cslr_soc_ctrl_mmr.h>
#include <ti/drv/sciclient/sciclient.h>
#include "app_utils.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

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
    uint32_t clockStatus;
    uint64_t minRate, respClkRate;
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
        /* Power on DSI and DPHY. */
        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleState(
                UTILS_TISCI_DEV_DSS_DSI, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        if(CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleState(
                UTILS_TISCI_DEV_DPHY_TX, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        /* Power off the DSS module for configuring clocks. */
        status = Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
                TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                TISCI_MSG_FLAG_AOP,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        if(status == CSL_PASS)
        {
            printf("\n TISCI_DEV_DSS0 device shutdown successful !\r\n");
        }
        else
        {
            printf("\n TISCI_DEV_DSS0 device shutdown NOT successful !!!\r\n");
        }
        
#if (1U == DSI_INSTANCE_0)
        /* Check if the required clock can be supported by system firmware. */
        minRate = cfgParams.pixelClk;
        status = Sciclient_pmQueryModuleClkFreq(TISCI_DEV_DSS0,
                                            TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                            minRate,
                                            &respClkRate,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
        if(status == CSL_PASS)
        {
            printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK possible rate = %lld Hz\r\n", respClkRate);
        }
        else
        {
            printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK requested rate range NOT possible !!\r\n");
        }
        if(status == CSL_PASS)
        {
            /* Check if the clock is enabled or not */
            status = Sciclient_pmModuleGetClkStatus(TISCI_DEV_DSS0,
                                                    TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                                    &clockStatus,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if ((status == CSL_PASS) && (respClkRate >= minRate))
        {
            /* Set the required DPI clock frequency. */
             status = Sciclient_pmSetModuleClkFreq(
                                      TISCI_DEV_DSS0,
                                      TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                      respClkRate,
                                      TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                      SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status == CSL_PASS)
            {
                if (clockStatus == TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ)
                {
                    /* Enable the clock */
                    status = Sciclient_pmModuleClkRequest(
                                                        TISCI_DEV_DSS0,
                                                        TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                                        TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                                        0U,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
                }
            }
        }

        /* Read back the set clock frequency. */
        uint64_t clkFreq = 0U;
        Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
                                    TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                    &clkFreq,
                                    SCICLIENT_SERVICE_WAIT_FOREVER);
        printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK = %lld Hz\r\n", clkFreq);

        status = Sciclient_pmModuleClkRequest(TISCI_DEV_DSS0,
                                              TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                              TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ,
                                              0U,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);

        /* Set the parent clock for TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK. */
        if (CSL_PASS == status)
        {
            status = Sciclient_pmSetModuleClkParent(TISCI_DEV_DSS0,
                TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK,
                TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        /* Read back the clock of the child that required the clock configuration. */
        if(CSL_PASS == status)
        {
            Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
                                        TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK,
                                        &clkFreq,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_2_IN_2X_CLK: Expected %lld and getting %lld Hz\r\n", cfgParams.pixelClk, clkFreq);
        }

#elif (1U == DSI_INSTANCE_1)
        if(CSL_PASS == status)
        {        
            status = Sciclient_pmSetModuleClkFreq(TISCI_DEV_DSS0,
                        TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK,
                        cfgParams.pixelClk,
                        0,
                        SCICLIENT_SERVICE_WAIT_FOREVER);      
        }
        if (CSL_PASS == status)
        {
            status = Sciclient_pmGetModuleClkFreq(TISCI_DEV_DSS0,
                        TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK,
                        &respClkRate,
                        SCICLIENT_SERVICE_WAIT_FOREVER);

            if (CSL_PASS == status)
            {
                printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK set at required frequency = %lld Hz \n", respClkRate);
            }
            else
            {
                printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_1_IN_2X_CLK not configured at desired frequency, current frequency is  = %lld Hz\n", respClkRate);
            }
        }
#endif
    }
    else
    {
        if(CSL_PASS == status)
        {
            /* Power on SerDes module. */
            status = Sciclient_pmSetModuleState(TISCI_DEV_SERDES_10G0,
                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                TISCI_MSG_FLAG_AOP,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if(CSL_PASS == status)
        {
            /* Power on EDP module. */
            status = Sciclient_pmSetModuleState(TISCI_DEV_DSS_EDP0,
                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                TISCI_MSG_FLAG_AOP,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        /* Power off DSS before configuring the clocks. */
        status = Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
                TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                TISCI_MSG_FLAG_AOP,
                SCICLIENT_SERVICE_WAIT_FOREVER);

        if(status == CSL_PASS)
        {
            printf("\n TISCI_DEV_DSS0 device shutdown successful !\r\n");
        }
        else
        {
            printf("\n TISCI_DEV_DSS0 device shutdown NOT successful !!!\r\n");
        }

        /* Checking if the required pixel clock can be set by the system firmware. */
        minRate = cfgParams.pixelClk;
        status = Sciclient_pmQueryModuleClkFreq(TISCI_DEV_DSS0,
                                            TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                            minRate,
                                            &respClkRate,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);

        if(status == CSL_PASS)
        {
            printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK possible rate = %lld Hz\r\n", respClkRate);
        }
        else
        {
            printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK requested rate range NOT possible !!\r\n");
        }

        if(status == CSL_PASS)
        {
            /* Check if the clock is enabled or not. */
            status = Sciclient_pmModuleGetClkStatus(TISCI_DEV_DSS0,
                                                    TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                                    &clockStatus,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }

        if ((status == CSL_PASS) && (respClkRate >= minRate))
        {
            /* Set the pixel clock. */
            status = Sciclient_pmSetModuleClkFreq(
                                      TISCI_DEV_DSS0,
                                      TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                      respClkRate,
                                      TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                      SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status == CSL_PASS)
            {
                if (clockStatus == TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ)
                {
                    /* Enable the required clock. */
                    status = Sciclient_pmModuleClkRequest(
                                                        TISCI_DEV_DSS0,
                                                        TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_2X_CLK_PARENT_HSDIV1_16FFT_MAIN_16_HSDIVOUT0_CLK,
                                                        TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                                        0U,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
                }
            }
        }

        if(CSL_PASS == status)
        {
            printf("\nTISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_CLK is now ENABLED !\r\n");
            /* Enable the DSS device as clock configuration has been successful. */
            status = Sciclient_pmSetModuleState(TISCI_DEV_DSS0,
                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                TISCI_MSG_FLAG_AOP,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        else
        {
            printf("\n TISCI_DEV_DSS0_DSS_INST0_DPI_0_IN_CLK is STILL DISABLED !!!\r\n");
        }

#if (1U == DO_MULTILINK)
        status = Sciclient_pmSetModuleClkParent(TISCI_DEV_SERDES_10G0,
                TISCI_DEV_SERDES_10G0_CORE_REF_CLK,
                TISCI_DEV_SERDES_10G0_CORE_REF_CLK_PARENT_HSDIV4_16FFT_MAIN_2_HSDIVOUT4_CLK,
                SCICLIENT_SERVICE_WAIT_FOREVER);
        
        if(CSL_PASS == status)
        {
            printf("\nTISCI_DEV_SERDES_10G0_CORE_REF_CLK Parent has been set to 100MHz !\r\n");
        }
        else
        {
            printf("\nFAILED to set TISCI_DEV_SERDES_10G0_CORE_REF_CLK to 100MHz !\r\n");
        }
        uint64_t clkFreq = 0U;
        status = Sciclient_pmGetModuleClkFreq(TISCI_DEV_SERDES_10G0,
                TISCI_DEV_SERDES_10G0_CORE_REF_CLK,
                &clkFreq,
                SCICLIENT_SERVICE_WAIT_FOREVER);

        if(status == CSL_PASS)
        {
            printf("\n Serdes Core RefClk is set to = %lld Hz\r\n", clkFreq);
        }
        else
        {
            printf("\n Failed to get clock frequency !!\r\n");
        }
#endif
    }

}
