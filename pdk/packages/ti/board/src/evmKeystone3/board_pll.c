/******************************************************************************
 * Copyright (c) 2018 Texas Instruments Incorporated - http://www.ti.com
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
 * \file   board_pll.c
 *
 * \brief  board pll configurations
 *
 * Phase-Locked Loop circuitries (PLLs) in the device are clock generator PLLs,
 * which multiply the lowerfrequency reference clock up to the operating 
 * frequency of the respective subsystem(s).
 * There are total two PLLs domains in the device:
 * (1) WKUP/MCU PLL domain
 *       - Again WKUP/MCU PLL is categorized as MCU PLL0 and MCU PLL1 
 * (2) MAIN PLL Domain
 *       - MAIN PLL is categorized as seven PLLs. i.e,MAIN PLL, PER0 PLL,
 *         PER1 PLL, DDR PLL, DSS PLL, ARM0 PLL and ARM1 PLL.
 * To generate high-frequency clocks, the device supports multiple on-chip 
 * PLLs controlled directly by the Clocking Management. 
 * They are of two types:
 * (1) Type A PLLs
 * (2) Type B PLLs
 * Board PLL configures the Main PLLs and MCU PLLs controller modules based on
 * different multplier and divider configurations.  
 *
 */

#include "board_internal.h"
#include "keystone3_pll.h"
#include <ti/drv/sciclient/sciclient.h>

/**
 * \brief  PLL clock enable
 *
 * This function is used to set the PLL Module clock frequency
 *
 * \param  moduleId [IN]  Module for which the state should be set.
 *                        Refer Sciclient_PmDeviceIds in sciclient_fmwMsgParams.h
 * \param  clockId  [IN]  Clock Id for the module.
 *                        Refer Sciclient_PmModuleClockIds in sciclient_fmwMsgParams.h
 * \param  clkRate  [IN]  Value of the clock frequency to be set
 *
 * \return int32_t
 *                CSL_PASS - on Success
 *                CSL_EFAIL - on Failure
 *
 */
static int32_t Board_PLLSetModuleClkFreq(uint32_t modId,
                                         uint32_t clkId,
                                         uint64_t clkRate)
{
    uint8_t parentId = 0;
    uint32_t i = 0U;
    int32_t status   = 0;
    uint64_t respClkRate = 0;
    uint32_t clockBaseId = 0U;
    uint32_t numParents = 0U;
    uint32_t clockStatus;

    status = Sciclient_pmQueryModuleClkFreq(modId,
                                        clkId,
                                        clkRate,
                                        &respClkRate,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(status == CSL_PASS)
    {
        /* Check if the clock is enabled or not */
        status = Sciclient_pmModuleGetClkStatus(modId,
                                                clkId,
                                                &clockStatus,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if(status == CSL_PASS)
    {
        /* Check if the clock is enabled or not */
        status = Sciclient_pmGetModuleClkNumParent(modId,
                                                clkId,
                                                &numParents,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if ((status == CSL_PASS) && (respClkRate == clkRate))
    {
        status = Sciclient_pmSetModuleClkFreq(
                                modId,
                                clkId,
                                clkRate,
                                TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                SCICLIENT_SERVICE_WAIT_FOREVER);
        if (status == CSL_PASS)
        {
            if (clockStatus == TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ)
            {
                /* Enable the clock */
                status = Sciclient_pmModuleClkRequest(
                                                    modId,
                                                    clkId,
                                                    TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                                    0U,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
            }
        }
    }
    else if (status == CSL_PASS)
    {
        /* Try to loop and change parents of the clock */
        for(i = 0U; i < numParents; i++)
        {
            /* Disabling the clock */
            status = Sciclient_pmModuleClkRequest(modId,
                                                  clkId,
                                                  TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ,
                                                  0U,
                                                  SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status == CSL_PASS)
            {
                clockStatus = TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ;
                /* Setting the new parent */
                status = Sciclient_pmSetModuleClkParent(
                                        modId,
                                        clkId,
                                        clkId+i+1,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            }
            /* Check if the clock can be set to desirable freq. */
            if (status == CSL_PASS)
            {
                status = Sciclient_pmQueryModuleClkFreq(modId,
                                                        clkId,
                                                        clkRate,
                                                        &respClkRate,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            }
            if (status == CSL_PASS)
            {
                if(respClkRate == clkRate)
                {
                    break;
                }
                else
                {
                    status = CSL_EFAIL;
                }
            }
            parentId++;
            clockBaseId++;
        }

        if (status == CSL_PASS)
        {   
            /* Set the clock at the desirable frequency*/
            status = Sciclient_pmSetModuleClkFreq(
                                    modId,
                                    clkId,
                                    clkRate,
                                    TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        if((status == CSL_PASS) && (clockStatus == TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ))
        {
            /*Enable the clock again */
            status = Sciclient_pmModuleClkRequest(
                                                modId,
                                                clkId,
                                                TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                                0U,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
        }
    }

    return status;
}

/**
 * \brief  Function to initialize module clock frequency
 *
 * \param  moduleId [IN]  Module for which the state should be set.
 *                        Refer Sciclient_PmDeviceIds in sciclient_fmwMsgParams.h
 * \param  clockId  [IN]  Clock Id for the module.
 *                        Refer Sciclient_PmModuleClockIds in sciclient_fmwMsgParams.h
 * \param  clkRate  [IN]  Value of the clock frequency to be set

 * \return Board_STATUS
 */
Board_STATUS Board_PLLInit(uint32_t modId,
                           uint32_t clkId,
                           uint64_t clkRate)
{
    int32_t  status = CSL_EFAIL;

    status = Board_PLLSetModuleClkFreq(modId, clkId, clkRate);
    if(status != CSL_PASS)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

/**
 * \brief  Function to initialize all the PLL clocks with default values
 *
 * \return Board_STATUS
 */
Board_STATUS Board_PLLInitAll(void)
{
    Board_STATUS  status = CSL_PASS;
    uint8_t index;
    uint32_t loopCount;

    const uint32_t dev_id[] =
    {
        TISCI_DEV_SERDES0,             //MAIN PLL*/
        TISCI_DEV_DDRSS0,              //DDR PLL
        TISCI_DEV_SA2_UL0,             //PER1 PLL
        TISCI_DEV_OLDI_TX_CORE_MAIN_0, //DSS PLL
        TISCI_DEV_MCU_ADC1,            //MCU PLL - MCUHSDIV_CLKOUT1
        TISCI_DEV_MCU_MCAN0,           //MCU PLL - MCUHSDIV_CLKOUT2
        TISCI_DEV_MCU_I2C0,            //MCU PLL - MCUHSDIV_CLKOUT3
        TISCI_DEV_MCU_FSS0_OSPI_0,     //MCU PLL - MCUHSDIV_CLKOUT4,
        TISCI_DEV_MCU_CPSW0,           //CPSW PLL
        TISCI_DEV_USB3SS1              //PER0 PLL
    };

    const uint32_t clock_id[] =
    {
        TISCI_DEV_SERDES0_BUS_LI_REFCLK,
        TISCI_DEV_DDRSS0_BUS_DDRSS_BYP_4X_CLK,
        TISCI_DEV_SA2_UL0_BUS_PKA_IN_CLK,
        TISCI_DEV_OLDI_TX_CORE_MAIN_0_BUS_OLDI_PLL_CLK,
        TISCI_DEV_MCU_ADC1_BUS_ADC_CLK,
        TISCI_DEV_MCU_MCAN0_BUS_MCANSS_CCLK_CLK,
        TISCI_DEV_MCU_I2C0_BUS_PISYS_CLK,
        TISCI_DEV_MCU_FSS0_OSPI_0_BUS_OSPI0_RCLK_CLK,
        TISCI_DEV_MCU_CPSW0_BUS_RGMII_MHZ_250_CLK,
        TISCI_DEV_USB3SS1_BUS_PHY2_REFCLK960M_CLK,
    };

    const uint32_t pll_freq_hz[] =
    {
        BOARD_MAIN_PLL_CLK_FREQ,
        BOARD_DDR_PLL_CLK_FREQ,
        BOARD_PER1_PLL_CLK_FREQ,
        BOARD_DSS_PLL_CLK_FREQ,
        BOARD_MCUHSDIV_CLKOUT1_CLK_FREQ,
        BOARD_MCUHSDIV_CLKOUT2_CLK_FREQ,
        BOARD_MCUHSDIV_CLKOUT3_CLK_FREQ,
        BOARD_MCUHSDIV_CLKOUT4_CLK_FREQ,
        BOARD_CPSW_PLL_CLK_FREQ,
        BOARD_PER0_PLL_CLK_FREQ
    };

    loopCount = sizeof (pll_freq_hz)/sizeof(uint32_t);

    for (index = 0; index < loopCount; index++)
    {
        status = Board_PLLInit(dev_id[index],
                               clock_id[index],
                               pll_freq_hz[index]);
        if(status != BOARD_SOK)
        {
            return BOARD_FAIL;
        }
    }

    return status;
}
