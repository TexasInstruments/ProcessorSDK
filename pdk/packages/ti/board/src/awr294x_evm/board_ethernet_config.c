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
 *      This file contains the Ethernet PHY configurations for AWR294X EVM
 *
 */

#include "board_ethernet_config.h"
#include "board_internal.h"
#include "board_cfg.h"
#include <ti/csl/soc.h>

#include "board_internal.h"
#include "board_cfg.h"
#include <ti/csl/soc.h>
#ifdef ENABLE_AWR294X_BOARD_MDIO
#include <ti/csl/cslr_mdio.h>
#endif

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
#ifdef ENABLE_AWR294X_BOARD_MDIO
    HW_WR_REG32((baseAddr + 0x4), (CSL_FMKT(MDIO_CONTROL_REG_ENABLE, YES)  |
                CSL_FMK(MDIO_CONTROL_REG_CLKDIV,0xFF)));
#endif
}

/**
 * \brief  PHY register write function
 *
 * This function is used to writes a PHY register using MDIO.
 *
 * \param   baseAddr [IN]   MDIO base address
 *          phyAddr  [IN]   PHY Address
 *          regAddr  [IN]   Register offset to be written
 *          data     [IN]   Value to be written
 *
 */
static void Board_ethPhyRegWrite(uint32_t baseAddr, uint32_t phyAddr,
                                 uint32_t regAddr, uint16_t data)
{
#ifdef ENABLE_AWR294X_BOARD_MDIO
    uint32_t regVal = 0U;

    /* Wait till transaction completion if any */
    while(HW_RD_FIELD32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U),
          CSL_MDIO_USER_GROUP_USER_ACCESS_REG_GO) == 1)
    {}

    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_GO, 1);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_WRITE, 1);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_PHYADR, phyAddr);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_REGADR, regAddr);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_DATA, data);
    HW_WR_REG32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U), regVal);

    /* wait for command completion */
    while(HW_RD_FIELD32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U),
          CSL_MDIO_USER_GROUP_USER_ACCESS_REG_GO) == 1)
    {}
#endif
}

/**
 * \brief  PHY register read function
 *
 * This function is used to Read a PHY register using MDIO.
 *
 * \param   baseAddr [IN]   MDIO base address
 *          phyAddr  [IN]   PHY Address
 *          regAddr  [IN]   Register offset to be written
 *          regData  [OUT]  Pointer where the read value shall be written
 *
 * \return  uint32_t
            TRUE     Read is successful.
 *          FALSE    Read is not acknowledged properly.
 */
static uint32_t BoardDiag_ethPhyRegRead(uint32_t baseAddr, uint32_t phyAddr,
                                        uint32_t regAddr, uint16_t *regData)
{
    uint32_t retVal = 0U;
#ifdef ENABLE_AWR294X_BOARD_MDIO
    uint32_t regVal = 0U;

    /* Wait till transaction completion if any */
    while(HW_RD_FIELD32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U),
        CSL_MDIO_USER_GROUP_USER_ACCESS_REG_GO) == 1)
    {}
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_GO,1);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_WRITE, 0);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_PHYADR, phyAddr);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_REGADR, regAddr);
    HW_WR_REG32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U), regVal);

    /* wait for command completion */
    while(HW_RD_FIELD32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U),
          CSL_MDIO_USER_GROUP_USER_ACCESS_REG_GO) == 1)
    {}

    /* Store the data if the read is acknowledged */
    if(HW_RD_FIELD32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U),
        CSL_MDIO_USER_GROUP_USER_ACCESS_REG_ACK) == 1)
    {
        *regData = (uint16_t)(HW_RD_FIELD32(baseAddr + \
                    CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U),
                    CSL_MDIO_USER_GROUP_USER_ACCESS_REG_DATA));
        retVal = (uint32_t)TRUE;
    }
    else
    {
        retVal = (uint32_t)FALSE;
    }
#endif

    return(retVal);
}

/**
 * \brief  Function to write extended address registers of Ethernet PHY
 *
 * \param   baseAddr [IN]    MDIO base address
 *          phyAddr  [IN]    Ethernet PHY address
 *          regNum   [IN]    PHY Register address
 *          pData    [OUT]   Values read from register
 *
 */
static void Board_ethPhyExtendedRegRead (uint32_t baseAddr,
                                         uint32_t phyAddr,
                                         uint32_t regNum,
                                         uint16_t *pData)
{
    Board_ethPhyRegWrite(baseAddr, phyAddr,
                         BOARD_ETHPHY_REGCR_REG_ADDR,
                         BOARD_ETHPHY_REGCR_ADDR_EN);
    Board_ethPhyRegWrite(baseAddr, phyAddr,
                         BOARD_ETHPHY_ADDAR_REG_ADDR, regNum);
    Board_ethPhyRegWrite(baseAddr, phyAddr,
                         BOARD_ETHPHY_REGCR_REG_ADDR,
                         BOARD_ETHPHY_REGCR_DATA_EN);
    BoardDiag_ethPhyRegRead(baseAddr, phyAddr,
                            BOARD_ETHPHY_ADDAR_REG_ADDR, pData);
}

/**
 * \brief  Function to write extended address registers of Ethernet PHY
 *
 * \param   baseAddr [IN]    MDIO base address
 * \param   phyAddr  [IN]    Ethernet PHY address
 * \param   regNum   [IN]    PHY Register address
 * \param   regVal   [IN]    Register value to be written
 *
 * \return  none
 */
static void Board_ethPhyExtendedRegWrite(uint32_t baseAddr,
                                         uint32_t phyAddr,
                                         uint32_t regNum,
                                         uint16_t regVal)
{
    Board_ethPhyRegWrite(baseAddr, phyAddr,
                         BOARD_ETHPHY_REGCR_REG_ADDR,
                         BOARD_ETHPHY_REGCR_ADDR_EN);
    Board_ethPhyRegWrite(baseAddr, phyAddr,
                         BOARD_ETHPHY_ADDAR_REG_ADDR, regNum);
    Board_ethPhyRegWrite(baseAddr, phyAddr,
                         BOARD_ETHPHY_REGCR_REG_ADDR,
                         BOARD_ETHPHY_REGCR_DATA_EN);
    Board_ethPhyRegWrite(baseAddr, phyAddr,
                         BOARD_ETHPHY_ADDAR_REG_ADDR, regVal);
}

/**
 * \brief  Function to set the Ethernet PHY speed
 *
 * This is internal function to the board library to configure the
 * Ethernet PHY speed
 *
 * \return  none
 */
static Board_STATUS Board_setEthPhySpeed(uint32_t baseAddr,
                                         uint32_t phyAddr,
                                         uint8_t speed)
{
    uint16_t regData = 0;

    Board_mdioInit(baseAddr);
    BoardDiag_ethPhyRegRead(baseAddr, phyAddr, 0, &regData);

    /* Reset the speed bits and disable the auto negotiation */
    regData &= ~(BOARD_ETH_PHY_SPEED_MASK | BOARD_ETH_PHY_AUTONEG_MASK);

    if(speed)
    {
        /* Set the speed to 1000mbps */
        regData |= BOARD_ETH_PHY_SPEED_1000MPBS;
    }
    else
    {
        /* Set the speed to 100mbps */
        regData |= BOARD_ETH_PHY_SPEED_100MPBS;
    }

    Board_ethPhyRegWrite(baseAddr, phyAddr, 0, regData);

    return (BOARD_SOK);
}

/**
 * \brief  Sets the Ethernet subsytem board specific configurations
 *
 * \param  mode    [IN]    Mode selection for the specified port number
 *                         001 - RMII
 *                         010 - RGMII
 * \return  none
 */
Board_STATUS Board_ethConfig(uint8_t mode)
{
    uint32_t regData;

    Board_unlockMMR();

    regData = CSL_REG32_RD(BOARD_CPSW_CTRL_REG_ADDR);
    regData = (regData & 0x7) | mode;
    CSL_REG32_WR(BOARD_CPSW_CTRL_REG_ADDR, regData);

    return BOARD_SOK;
}

/**
 * \brief  Board specific configurations for Ethernet PHY
 *
 * \return  none
 */
Board_STATUS Board_ethPhyConfig(void)
{
    uint32_t baseAddr;
    uint16_t regData = 0;

    baseAddr = (BOARD_ETH_BASE_ADDR + 0x0F00);

    Board_ethConfig(RGMII);
    Board_mdioInit(baseAddr);

    /* Enable PHY speed LED functionality */
    Board_ethPhyExtendedRegRead(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_ADDR, &regData);
    regData  = (regData & ~0xF) | 0x6;
    Board_ethPhyExtendedRegWrite(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                 BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_ADDR, regData);

    regData = 0;
    BoardDiag_ethPhyRegRead(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                            BOARD_ETHPHY_LEDCR1_REG_ADDR, &regData);
    regData  = (regData & ~0xF000) | 0x8000;
    Board_ethPhyRegWrite(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                         BOARD_ETHPHY_LEDCR1_REG_ADDR, regData);

    /* When the Phy is strapped to enable Fast Link Drop (FLD) feature,
     * the detect threshold value becomes 0x2 in bit 2:0 instead of 0x1
     * in the  FLD_THRESH (0x2e) register  as in non strapped case.
     * This causes the phy link to be unstable.
     * As a workaround, write a value of 0x1 in this bit field if
     * bit 10 of STRAP_STS2 (0x6f) register is set (enable FLD).
     */
     regData = 0;
     Board_ethPhyExtendedRegRead(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                 BOARD_ETHPHY_STRAP_STS2_REG_ADDR, &regData);
     if (regData & 0x0400)
     {
         regData = 0;
         Board_ethPhyExtendedRegRead(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                     BOARD_ETHPHY_FLD_THRESH_REG_ADDR,
                                     &regData);
         if (regData == 0x222)
         {
             regData &= ~0x7;
             Board_ethPhyExtendedRegWrite(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                          BOARD_ETHPHY_FLD_THRESH_REG_ADDR,
                                          (regData | 0x1));
         }
     }

    /* Setting the PHY TX and RX delay configurations */
    Board_ethPhyExtendedRegRead(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                BOARD_ETHPHY_RGMIICTL_REG_ADDR, &regData);
    /* Disable Tx delay and enable Rx delay */
    regData &= ~(BOARD_ETHPHY_RGMIICTL_CLKDELAY_MASK);
    regData |= BOARD_ETHPHY_RGMIICTL_RXDELAY_EN;
    Board_ethPhyExtendedRegWrite(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                 BOARD_ETHPHY_RGMIICTL_REG_ADDR, regData);

    /* No Tx delay and 2ns Rx delay */
    Board_ethPhyExtendedRegWrite(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                 BOARD_ETHPHY_RGMIIDCTL_REG_ADDR, 0x07);

    Board_setEthPhySpeed(baseAddr, BOARD_MCU_EMAC_PHY_ADDR, 0);

    return BOARD_SOK;
}
