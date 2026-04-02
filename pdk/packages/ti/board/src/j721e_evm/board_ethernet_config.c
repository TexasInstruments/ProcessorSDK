/******************************************************************************
 * Copyright (c) 2019-2022 Texas Instruments Incorporated - http://www.ti.com
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
#include <ti/csl/cslr_mdio.h>


Board_pruicssMdioInfo  Board_gPruicssMdioInfo[BOARD_ICSS_EMAC_PORT_MAX] =
                       {{CSL_PRU_ICSSG0_PR1_MDIO_V1P7_MDIO_BASE, BOARD_ICSS0_EMAC_PHY0_ADDR},
                        {CSL_PRU_ICSSG0_PR1_MDIO_V1P7_MDIO_BASE, BOARD_ICSS0_EMAC_PHY1_ADDR},
                        {CSL_PRU_ICSSG1_PR1_MDIO_V1P7_MDIO_BASE, BOARD_ICSS1_EMAC_PHY0_ADDR},
                        {CSL_PRU_ICSSG1_PR1_MDIO_V1P7_MDIO_BASE, BOARD_ICSS1_EMAC_PHY1_ADDR},
                       };

Board_pruicssMdioInfo  Board_cpswMdioInfo[BOARD_CPSW9G_EMAC_PORT_MAX] =
                       {{(CSL_CPSW0_NUSS_BASE + BOARD_CPSW_MDIO_REG_OFFSET), BOARD_ICSS0_EMAC_PHY0_ADDR},
                        {(CSL_CPSW0_NUSS_BASE + BOARD_CPSW_MDIO_REG_OFFSET), BOARD_ICSS0_EMAC_PHY1_ADDR},
                        {(CSL_CPSW0_NUSS_BASE + BOARD_CPSW_MDIO_REG_OFFSET), BOARD_ICSS1_EMAC_PHY0_ADDR},
                        {(CSL_CPSW0_NUSS_BASE + BOARD_CPSW_MDIO_REG_OFFSET), BOARD_ICSS1_EMAC_PHY1_ADDR},
                       };

/**
 * \brief  Configures kick registers for Ethernet MMR access
 *
 * \param   domain   [IN]   MMR register domain
 * \param   lockCtrl [IN]   Register lock/unlock control
 *                          0 - Unlocks the MMR register write access
 *                          1 - Locks the MMR register write access
 *
 * \return  Board_STATUS
 */
static Board_STATUS Board_ethCfgKickCtrl(uint32_t domain, uint32_t lockCtrl)
{
    Board_STATUS status;

    if(lockCtrl)
    {
        status = Board_lockMMRPartition(domain, BOARD_MMR_PARTITION1);
    }
    else
    {
        status = Board_unlockMMRPartition(domain, BOARD_MMR_PARTITION1);
    }

    return (status);
}

/**
 * \brief  Function to initialize MDIO
 *
 * \param   baseAddr [IN]   MDIO base address
 *
 * \return  uint32_t
            UTRUE     Read is successful.
 *          UFALSE    Read is not acknowledged properly.
 */
static void Board_mdioInit(uint32_t baseAddr)
{
    HW_WR_REG32((baseAddr + BOARD_MDIO_CTRL_REG_OFFSET),
                (CSL_FMKT(MDIO_CONTROL_REG_ENABLE, YES) |
                CSL_FMK(MDIO_CONTROL_REG_CLKDIV,
                BOARD_MDIO_CLK_DIV_CFG)));
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
    uint32_t regVal = 0U;

    /* Wait till transaction completion if any */
    while(1U == HW_RD_FIELD32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U),
          CSL_MDIO_USER_GROUP_USER_ACCESS_REG_GO))
    {}

    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_GO, 1);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_WRITE, 1);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_PHYADR, phyAddr);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_REGADR, regAddr);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_DATA, data);
    HW_WR_REG32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U), regVal);

    /* wait for command completion */
    while(1U == HW_RD_FIELD32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U),
          CSL_MDIO_USER_GROUP_USER_ACCESS_REG_GO))
    {}
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
            UTRUE     Read is successful.
 *          UFALSE    Read is not acknowledged properly.
 */
static uint32_t BoardDiag_ethPhyRegRead(uint32_t baseAddr, uint32_t phyAddr,
                                        uint32_t regAddr, uint16_t *regData)
{
    uint32_t regVal = 0U;
    uint32_t retVal = 0U;

    /* Wait till transaction completion if any */
    while(1U == HW_RD_FIELD32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U),
               CSL_MDIO_USER_GROUP_USER_ACCESS_REG_GO))
    {}
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_GO,1);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_WRITE, 0);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_PHYADR, phyAddr);
    HW_SET_FIELD(regVal, CSL_MDIO_USER_GROUP_USER_ACCESS_REG_REGADR, regAddr);
    HW_WR_REG32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U), regVal);

    /* wait for command completion */
    while(1U == HW_RD_FIELD32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U),
               CSL_MDIO_USER_GROUP_USER_ACCESS_REG_GO))
    {}

    /* Store the data if the read is acknowledged */
    if(1U == HW_RD_FIELD32(baseAddr + CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U),
            CSL_MDIO_USER_GROUP_USER_ACCESS_REG_ACK))
    {
        *regData = (uint16_t)(HW_RD_FIELD32(baseAddr + \
                    CSL_MDIO_USER_GROUP_USER_ACCESS_REG(0U),
                    CSL_MDIO_USER_GROUP_USER_ACCESS_REG_DATA));
        retVal = UTRUE;
    }
    else
    {
        retVal = UFALSE;
    }

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
 * \brief  Disables ICSSG MAC internal delay
 *
 * \return  none
 */
static void Board_disableIcssEmacDelay(void)
{
    Board_ethCfgKickCtrl(BOARD_SOC_DOMAIN_MAIN, 0);

    HW_WR_REG32((CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_ICSSG0_CTRL0), BOARD_EMAC_DELAY_CFG);
    HW_WR_REG32((CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_ICSSG0_CTRL1), BOARD_EMAC_DELAY_CFG);
    HW_WR_REG32((CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_ICSSG1_CTRL0), BOARD_EMAC_DELAY_CFG);
    HW_WR_REG32((CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_ICSSG1_CTRL1), BOARD_EMAC_DELAY_CFG);

    Board_ethCfgKickCtrl(BOARD_SOC_DOMAIN_MAIN, 1);
}

/**
 * \brief  Enable/Disable PHY reset for ENET EXP boards PHY
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_cpswEnetExpPhyReset(bool enableFlag)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status = BOARD_SOK;

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = THREE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_2;
    ioExpCfg.pinNum      = PIN_NUM_1;

    if (BTRUE == enableFlag)
    {
        /* EXP_ENET_RSTz - set to 0 for PHY reset */
        ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;
    }
    else
    {
        /* EXP_ENET_RSTz - set to 1 to take PHY out of reset (normal operation)*/
        ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;
    }

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);

    return status;
}

/**
 * \brief  Enable/Disable COMA_MODE for ENET EXP boards PHY
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_cpswEnetExpComaModeCfg(bool enableFlag)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status = BOARD_SOK;

    if (BTRUE == Board_detectBoard(BOARD_ID_ENET))
    {
        ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
        ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
        ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR;
        ioExpCfg.enableIntr  = BFALSE;
        ioExpCfg.ioExpType   = THREE_PORT_IOEXP;
        ioExpCfg.portNum     = PORTNUM_2;
        ioExpCfg.pinNum      = PIN_NUM_0;

        if (BTRUE == enableFlag)
        {
            /* ENET_EXP_PWRDN - set to 1 for device power down */
            ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;
        }
        else
        {
            /* ENET_EXP_PWRDN - set to 0 for normal operation */
            ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;
        }

        status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    }

    return status;
}

/**
 * \brief  Board specific configurations for SGMII Ethernet PHYs
 *
 * This function takes care of configuring the internal delays for SGMII
 * Ethernet PHYs
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_sgmiiEthPhyConfig(void)
{
    return BOARD_SOK;
}

/**
 * \brief  Board specific configurations for CPSW9G Ethernet PHYs
 *
 * This function takes care of configuring the internal delays for CPSW9G
 * Ethernet PHYs
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_cpsw9gEthPhyConfig(void)
{
    Board_STATUS status;
    uint32_t baseAddr;
    uint8_t  phyAddr;
    uint32_t index;
    uint16_t regData = 0;
    bool isAlpha = BFALSE;

    /*
     * MDIO stability issue due to ENET card is resolved in Beta HW revision.
     * Disabling ENET card is needed only for Alpha CP boards.
     */
    isAlpha = Board_isAlpha(BOARD_ID_CP);

    if(BTRUE == isAlpha)
    {
        /* CPSW9G MDIO access is unstable when ENET card is connected.
           Keeping the ENET PHY in reset as a temporary workaround */
        status = Board_cpswEnetExpComaModeCfg(BTRUE);
        status = Board_cpswEnetExpPhyReset(BTRUE);
        if (BOARD_SOK != status)
        {
            return status;
        }
    }

    for(index = 0U; index < BOARD_CPSW9G_EMAC_PORT_MAX; index++)
    {
        baseAddr = Board_cpswMdioInfo[index].mdioBaseAddrs;
        phyAddr  = Board_cpswMdioInfo[index].phyAddrs;

        Board_mdioInit(baseAddr);

        /* Enable PHY speed LED functionality */
        Board_ethPhyExtendedRegRead(baseAddr, phyAddr,
                                    BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_ADDR,
                                    &regData);
        regData  = (regData & ~(BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_MASK)) |
                    BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_CFG;
        Board_ethPhyExtendedRegWrite(baseAddr, phyAddr,
                                     BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_ADDR,
                                     regData);

        regData = 0;
        BoardDiag_ethPhyRegRead(baseAddr, phyAddr,
                                BOARD_ETHPHY_LEDCR1_REG_ADDR, &regData);
        regData  = (regData & ~(BOARD_ETHPHY_LEDCR1_REG_MASK)) |
                    BOARD_ETHPHY_LEDCR1_REG_CFG;
        Board_ethPhyRegWrite(baseAddr, phyAddr,
                             BOARD_ETHPHY_LEDCR1_REG_ADDR, regData);

       /* When the Phy is strapped to enable Fast Link Drop (FLD) feature,
        * the detect threshold value becomes 0x2 in bit 2:0 instead of 0x1
        * in the  FLD_THRESH (0x2e) register  as in non strapped case.
        * This causes the phy link to be unstable.
        * As a workaround, write a value of 0x1 in this bit field if
        * bit 10 of STRAP_STS2 (0x6f) register is set (enable FLD).
        */
        regData = 0;
        Board_ethPhyExtendedRegRead(baseAddr, phyAddr,
                                    BOARD_ETHPHY_STRAP_STS2_REG_ADDR,
                                    &regData);
        if (regData & BOARD_ETHPHY_STRAP_FLD_MASK)
        {
            regData = 0;
            Board_ethPhyExtendedRegRead(baseAddr, phyAddr,
                                        BOARD_ETHPHY_FLD_THRESH_REG_ADDR,
                                        &regData);
            if (BOARD_ETHPHY_STRAP_FLD_THS_CHECK_FLAG == regData)
            {
                regData &= ~0x7U;
                Board_ethPhyExtendedRegWrite(baseAddr, phyAddr,
                                             BOARD_ETHPHY_FLD_THRESH_REG_ADDR,
                                             (regData | 0x1U));
            }
        }

        /*Setting IO impedance to 35ohms  */
        Board_ethPhyExtendedRegWrite(baseAddr, phyAddr,
                                     BOARD_ETHPHY_GPIO_MUX_CFG_REG_ADDR,
                                     BOARD_ETHPHY_IO_IMPEDANCE);

        /* Enable the PHY delay configurations */
        Board_ethPhyExtendedRegWrite(baseAddr, phyAddr, BOARD_ETHPHY_RGMIICTL_REG_ADDR,
                                     BOARD_ETHPHY_DELAY_CTRL);

        /* Setting delay */
        Board_ethPhyExtendedRegWrite(baseAddr, phyAddr,
                                     BOARD_ETHPHY_RGMIIDCTL_REG_ADDR,
                                     BOARD_ETHPHY_CPSW9G_DELAY);
    }

    return BOARD_SOK;
}

/**
 * \brief  Board specific configurations for CPSW2G Ethernet PHY
 *
 * This function takes care of configuring the internal delays for MCU gigabit
 * Ethernet PHY
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_cpsw2gEthPhyConfig(void)
{
    uint32_t baseAddr;
    uint16_t regData = 0;

    baseAddr = (CSL_MCU_CPSW0_NUSS_BASE + 0x0F00U);

    Board_mdioInit(baseAddr);

    /* Enable PHY speed LED functionality */
    Board_ethPhyExtendedRegRead(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_ADDR,
                                &regData);
    regData  = (regData & ~(BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_MASK)) |
                BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_CFG;
    Board_ethPhyExtendedRegWrite(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                 BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_ADDR,
                                 regData);

    regData = 0;
    BoardDiag_ethPhyRegRead(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                            BOARD_ETHPHY_LEDCR1_REG_ADDR, &regData);
    regData  = (regData & ~(BOARD_ETHPHY_LEDCR1_REG_MASK)) |
                BOARD_ETHPHY_LEDCR1_REG_CFG;
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
                                 BOARD_ETHPHY_STRAP_STS2_REG_ADDR,
                                 &regData);
     if (regData & BOARD_ETHPHY_STRAP_FLD_MASK)
     {
         regData = 0;
         Board_ethPhyExtendedRegRead(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                     BOARD_ETHPHY_FLD_THRESH_REG_ADDR,
                                     &regData);
         if (BOARD_ETHPHY_STRAP_FLD_THS_CHECK_FLAG == regData)
         {
             regData &= ~0x7U;
             Board_ethPhyExtendedRegWrite(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                          BOARD_ETHPHY_FLD_THRESH_REG_ADDR,
                                          (regData | 0x1U));
         }
     }

    /* Enabling the TX and RX delay */
    Board_ethPhyExtendedRegRead(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                BOARD_ETHPHY_RGMIICTL_REG_ADDR, &regData);
    regData = regData | 0x3U;
    Board_ethPhyExtendedRegWrite(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                 BOARD_ETHPHY_RGMIICTL_REG_ADDR, regData);

    /* Setting delay */
    Board_ethPhyExtendedRegWrite(baseAddr, BOARD_MCU_EMAC_PHY_ADDR,
                                 BOARD_ETHPHY_RGMIIDCTL_REG_ADDR,
                                 BOARD_ETHPHY_CPSW2G_DELAY);

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
    uint8_t  phyAddr;
    uint32_t index;
    uint16_t regData = 0;
    Board_STATUS status = BOARD_SOK;

    status = Board_control(BOARD_CTRL_CMD_SET_ICSSG_MDIO_MUX, NULL);
    if(BOARD_SOK == status)
    {
        for(index = 0U; index < BOARD_ICSS_EMAC_PORT_MAX; index++)
        {
            baseAddr = Board_gPruicssMdioInfo[index].mdioBaseAddrs;
            phyAddr  = Board_gPruicssMdioInfo[index].phyAddrs;

            Board_mdioInit(baseAddr);

            /* Enable the PHY delay configurations */
            Board_ethPhyExtendedRegWrite(baseAddr, phyAddr,
                                         BOARD_ETHPHY_RGMIICTL_REG_ADDR,
                                         BOARD_ETHPHY_DELAY_CTRL);

            /* Setting delay */
            Board_ethPhyExtendedRegWrite(baseAddr, phyAddr,
                                         BOARD_ETHPHY_RGMIIDCTL_REG_ADDR,
                                         BOARD_ETHPHY_ICSSG_DELAY);

            /*Setting IO impedance to 35ohms  */
            Board_ethPhyExtendedRegWrite(baseAddr, phyAddr,
                                         BOARD_ETHPHY_GPIO_MUX_CFG_REG_ADDR,
                                         BOARD_ETHPHY_IO_IMPEDANCE);

            /* Enable PHY speed LED functionality */
            Board_ethPhyExtendedRegRead(baseAddr, phyAddr,
                                        BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_ADDR,
                                        &regData);
            regData  = (regData & ~(BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_MASK)) | 0x6U;
            Board_ethPhyExtendedRegWrite(baseAddr, phyAddr,
                                         BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_ADDR,
                                         regData);

            regData = 0;
            BoardDiag_ethPhyRegRead(baseAddr, phyAddr,
                                    BOARD_ETHPHY_LEDCR1_REG_ADDR, &regData);
            regData  = (regData & ~(BOARD_ETHPHY_LEDCR1_REG_MASK)) |
                        BOARD_ETHPHY_LEDCR1_REG_CFG;
            Board_ethPhyRegWrite(baseAddr, phyAddr,
                                 BOARD_ETHPHY_LEDCR1_REG_ADDR, regData);

           /* When the Phy is strapped to enable Fast Link Drop (FLD) feature,
            * the detect threshold value becomes 0x2 in bit 2:0 instead of 0x1
            * in the  FLD_THRESH (0x2e) register  as in non strapped case.
            * This causes the phy link to be unstable.
            * As a workaround, write a value of 0x1 in this bit field if
            * bit 10 of STRAP_STS2 (0x6f) register is set (enable FLD).
            */
            regData = 0;
            Board_ethPhyExtendedRegRead(baseAddr, phyAddr,
                                        BOARD_ETHPHY_STRAP_STS2_REG_ADDR,
                                        &regData);
            if (regData & BOARD_ETHPHY_STRAP_FLD_MASK)
            {
                regData = 0;
                Board_ethPhyExtendedRegRead(baseAddr, phyAddr,
                                            BOARD_ETHPHY_FLD_THRESH_REG_ADDR,
                                            &regData);
                if (BOARD_ETHPHY_STRAP_FLD_THS_CHECK_FLAG == regData)
                {
                    regData &= ~0x7U;
                    Board_ethPhyExtendedRegWrite(baseAddr, phyAddr,
                                                 BOARD_ETHPHY_FLD_THRESH_REG_ADDR,
                                                 (regData | 0x1U));
                }
            }
        }
    }

    return status;
}

/**
 * \brief  Configures the CPSW9G Subsytem for RGMII and RMII mode
 *
 * \param  portNum [IN]    EMAC port number
 * \param  mode    [IN]    Mode selection for the specified port number
 *                         000 - GMII
 *                         001 - RMII
 *                         010 - RGMII
 *                         011 - SGMII
 *                         100 - QSGMII
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_cpsw9gEthConfig(uint32_t portNum, uint8_t mode)
{
    uint32_t status;
    uintptr_t modeSel;
    uint32_t regData;

    Board_ethCfgKickCtrl(BOARD_SOC_DOMAIN_MAIN, 0);

    modeSel = CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_ENET1_CTRL + (portNum * 0x04U);
    regData = CSL_REG32_RD(modeSel);
    regData = mode;
    if (RGMII == mode)
    {
        regData |= (RGMII_ID_DISABLE_MASK);
    }
    CSL_REG32_WR(modeSel , regData);
    status = CSL_REG32_RD(modeSel);
    if (status != regData)
    {
        return BOARD_FAIL;
    }

    Board_ethCfgKickCtrl(BOARD_SOC_DOMAIN_MAIN, 1);

    return BOARD_SOK;
}

/**
 * \brief  Board specific configurations for ICSSG RGMII Ethernet
 *
 * This function takes care of configuring the internal delays for
 * ICSSG RGMII Ethernet PHY
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
static Board_STATUS Board_icssEthConfig(uint32_t portNum, uint8_t mode)
{
    Board_disableIcssEmacDelay();
    return BOARD_SOK;
}

/**
 * \brief  Configures the CPSW2G Subsytem for RGMII mode
 *
 * \param  mode    [IN]    Mode selection for the specified port number
 *                         00 - GMII
 *                         01 - RMII
 *                         10 - RGMII
 *                         11 - SGMII
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_cpsw2gMacModeConfig(uint8_t mode)
{
    uint32_t status;
    uintptr_t ethModeCtrl;
    uint32_t regData;

    Board_ethCfgKickCtrl(BOARD_SOC_DOMAIN_MCU, 0);

    ethModeCtrl = CSL_MCU_CTRL_MMR0_CFG0_BASE + CSL_MCU_CTRL_MMR_CFG0_MCU_ENET_CTRL;
    regData = CSL_REG32_RD(ethModeCtrl);
    regData = mode;
    if (RGMII == mode)
    {
        regData |= (RGMII_ID_DISABLE_MASK);
    }

    CSL_REG32_WR(ethModeCtrl , regData);

    Board_ethCfgKickCtrl(BOARD_SOC_DOMAIN_MCU, 1);

    status = CSL_REG32_RD(ethModeCtrl);
    if (status != regData)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

/**
 * \brief  Board specific configurations for CPSW2G Ethernet ports
 *
 * This function used to configures CPSW2G Ethernet controllers with the respective modes
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_ethConfigCpsw2g(void)
{
    Board_STATUS status = BOARD_SOK;

    /* Configures the MCU Ethernet */
    status = Board_cpsw2gMacModeConfig(RGMII);
    if(BOARD_SOK != status)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

/**
 * \brief  Board specific configurations for CPSW9G Ethernet ports
 *
 * This function used to configures CPSW9G Ethernet controllers with the respective modes
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_ethConfigCpsw9g(void)
{
    Board_STATUS status = BOARD_SOK;
    uint8_t portNum;

    /* On J721E EVM to use all 8 ports simultaneously, we use below configuration
       RGMII Ports - 1,3,4,8. QSGMII ports - 2 (main),5,6,7 (sub)*/

    /* Configures the CPSW9G RGMII ports */
    for(portNum = 0; portNum < BOARD_CPSW9G_PORT_MAX; portNum++)
    {
        if ( (0U == portNum) ||
             (2U == portNum) ||
             (3U == portNum) ||
             (7U == portNum) )
        {
            status = Board_cpsw9gEthConfig(portNum, RGMII);
        }
        else
        {
            if (1U == portNum)
            {
                status = Board_cpsw9gEthConfig(portNum, QSGMII);
            }
            else
            {
                status = Board_cpsw9gEthConfig(portNum, QSGMII_SUB);
            }
        }

        if(BOARD_SOK != status)
        {
            return BOARD_FAIL;
        }
    }

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
    Board_STATUS status = BOARD_SOK;
    uint8_t portNum;

    /* Configures ICSSG Ethernet */
    for(portNum = 0; portNum < BOARD_ICSS_EMAC_PORT_MAX; portNum++)
    {
        status = Board_icssEthConfig(portNum, RGMII);
        if(BOARD_SOK != status)
        {
            return BOARD_FAIL;
        }
    }

    return BOARD_SOK;
}
