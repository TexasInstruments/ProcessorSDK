/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
 *   \file  board_ethernet_config.c
 *
 *   \brief
 *      This file contains the boards specific Ethernet PHY configurations.
 *
 */

#include "board_ethernet_config.h"
#include "board_internal.h"
#include <ti/csl/soc.h>
#include <ti/csl/csl_mdio.h>

Board_MdioInfo_t  Board_gPruIcssMdioInfo[BOARD_ICSS_EMAC_PORT_MAX] =
                       {{CSL_PRU_ICSSG1_PR1_MDIO_V1P7_MDIO_BASE, BOARD_ICSS_EMAC_PHY0_ADDR},
                        {CSL_PRU_ICSSG1_PR1_MDIO_V1P7_MDIO_BASE, BOARD_ICSS_EMAC_PHY1_ADDR}
                       };

/**
 * \brief  Function to initialize MDIO
 *
 * \param   baseAddr [IN]   MDIO base address
 *
 * \return  uint32_t
            TRUE     Read is successful.
 *          FALSE    Read is not acknowledged properly.
 */
static void Board_mdioInit(uint32_t baseAddr)
{
    HW_WR_REG32((baseAddr + BOARD_MDIO_CTRL_REG_OFFSET),
                (CSL_FMKT(MDIO_CONTROL_REG_ENABLE, YES) |
                CSL_FMK(MDIO_CONTROL_REG_CLKDIV,
                BOARD_MDIO_CLK_DIV_CFG)));
}

/**
 * \brief  Board specific configurations for CPSW Ethernet PHY
 *
 * This function takes care of configuring the internal delays for MCU gigabit
 * Ethernet PHY
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_cpswEthPhyConfig(void)
{
    return BOARD_SOK;
}

/**
 * \brief  Board specific configurations for ICSS EMAC Ethernet PHYs
 *
 * This function takes care of configuring the internal delays for ICSS
 * Ethernet PHY
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_icssEthPhyConfig(void)
{
    uint32_t baseAddr;
    uint32_t index;
    Board_STATUS status = BOARD_SOK;

    for(index = 0; index < BOARD_ICSS_EMAC_PORT_MAX; index++)
    {
        baseAddr = Board_gPruIcssMdioInfo[index].mdioBaseAddrs;

        Board_mdioInit(baseAddr);
    }

    return status;
}

/**
 * \brief  Board specific configurations for CPSW Ethernet ports
 *
 * This function used to configures CPSW Ethernet controllers with the respective modes
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_ethConfigCpsw(void)
{
	return BOARD_SOK;
}

/**
 * \brief  Board specific configurations for ICSS Ethernet ports
 *
 * This function used to configures ICSS Ethernet controllers with the respective modes
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_ethConfigIcss(void)
{
	return BOARD_SOK;
}
