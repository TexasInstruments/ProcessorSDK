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

#ifndef BOARD_INTERNAL_H_
#define BOARD_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Include Files                                                             *
 *****************************************************************************/
#include <ti/csl/csl_types.h>
#include <ti/csl/cslr_device.h>

#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/soc/UART_soc.h>

#include <ti/board/board.h>
#include <ti/csl/tistdtypes.h>
#include <stdio.h>
#include <stdbool.h>

#undef ENABLE_LOGS

#if !defined(BOARD_DEBUG_LOG)
#if defined(ENABLE_LOGS)
#define BOARD_DEBUG_LOG     UART_printf
#else
#define BOARD_DEBUG_LOG(x, ...)
#endif
#endif  /* #if !defined(BOARD_DEBUG_LOG) */

#define MODE_PIN_MASK                        (0xFU)
#define PINMUX_BIT_MASK                      (0xFFF8FFF0U)
#define GPIO_PIN_MUX_CFG                     (0x50007U)

#ifdef BUILD_M4F
#define BOARD_PADCFG_PMUX_OFFSET             (0x4000U + 0x60000000U)
#else
#define BOARD_PADCFG_PMUX_OFFSET             (0x4000U)
#endif

/* MAIN CTRL pinmux base address */
#define BOARD_MAIN_PMUX_CTRL	             (CSL_PADCFG_CTRL0_CFG0_BASE + BOARD_PADCFG_PMUX_OFFSET)

/* WKUP CTRL pinmux base address */
#define BOARD_WKUP_PMUX_CTRL	              (CSL_MCU_PADCFG_CTRL0_CFG0_BASE + BOARD_PADCFG_PMUX_OFFSET)


/* Ethenet board library MACROs */
#define BOARD_ETHPHY_PHYID1_REG_ADDR            (0x2U)
#define BOARD_ETHPHY_PHYID2_REG_ADDR            (0x3U)
#define BOARD_ETHPHY_REGCR_REG_ADDR             (0xDU)
#define BOARD_ETHPHY_REGCR_ADDR_EN              (0x1FU)
#define BOARD_ETHPHY_REGCR_DATA_EN              (0x401FU)
#define BOARD_ETHPHY_ADDAR_REG_ADDR             (0xEU)

#define BOARD_ETHPHY_LEDCR1_REG_ADDR            (0x18U)

#define BOARD_ETHPHY_FLD_THRESH_REG_ADDR        (0x2EU)

#define BOARD_ETHPHY_RGMIICTL_REG_ADDR          (0x32U)
#define BOARD_ETHPHY_RGMIICTL_CLKDELAY_MASK     (0x3U)
#define BOARD_ETHPHY_RGMIICTL_TXDELAY_EN        (0x2U)
#define BOARD_ETHPHY_RGMIICTL_RXDELAY_EN        (0x1U)

#define BOARD_ETHPHY_STRAP_STS1_REG_ADDR        (0x6EU)
#define BOARD_ETHPHY_STRAP_STS2_REG_ADDR        (0x6FU)

#define BOARD_ETHPHY_RGMIIDCTL_REG_ADDR         (0x86U)

#define BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_ADDR    (0x172U)

#define BOARD_ETHPHY_GPIO_MUX_CFG_REG_ADDR      (0x170U)

#define BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_MASK    (0xFU)
#define BOARD_ETHPHY_GPIO_MUX_CTRL2_REG_CFG     (0x6U)

#define BOARD_ETHPHY_LEDCR1_REG_MASK            (0xF000U)
#define BOARD_ETHPHY_LEDCR1_REG_CFG             (0x8000U)

#define BOARD_ETHPHY_ICSSG_DELAY                (0xA9U)
#define BOARD_ETHPHY_CPSW9G_DELAY               (0xA9U)
#define BOARD_ETHPHY_CPSW2G_DELAY               (0x77U)
#define BOARD_ETHPHY_DELAY_CTRL                 (0xD3U)
#define BOARD_ETHPHY_IO_IMPEDANCE               (0x0C1FU)
#define BOARD_CPSW_MDIO_REG_OFFSET              (0xF00U)

#define BOARD_MDIO_CTRL_REG_OFFSET              (0x4U)
#define BOARD_MDIO_CLK_DIV_CFG                  (0xFFU)
#define BOARD_EMAC_DELAY_CFG                    (0x01000000U)

#define BOARD_ETHPHY_STRAP_FLD_MASK             (0x0400U)
#define BOARD_ETHPHY_STRAP_FLD_THS_CHECK_FLAG   (0x222U)

/*****************************************************************************
 * Internal Objects                                                          *
 *****************************************************************************/
extern I2C_config_list I2C_config;

typedef struct Board_I2cObj_s
{
    I2C_Handle    i2cHandle;
    uint8_t       i2cDomain;
    uint32_t      instNum;
    uint32_t      i2cBaseAddr;
} Board_I2cObj_t;

/****************************************************************************/

#define KICK0_UNLOCK_VAL                     (0x68EF3490U)
#define KICK1_UNLOCK_VAL                     (0xD172BC5AU)

#define BOARD_I2C_PORT_CNT                   (I2C_HWIP_MAX_CNT)

/*****************************************************************************
 * Function Prototypes                                                       *
 *****************************************************************************/

/**
 * \brief  Board pinmuxing enable function
 *
 * Enables pinmux for the board interfaces. Pin mux is done based
 * on the default/primary functionality of the board. Any pins shared by
 * multiple interfaces need to be reconfigured to access the secondary
 * functionality.
 *
 * \param   void
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxConfig (void);

/**
 *
 * \brief  Board PLL initialization function
 *
 *  Configures different PLL controller modules. This enables all the PLL
 *  controllers on the SoC with default configurations.
 *
 * \return BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_PLLInit(uint32_t modId, uint32_t clkId, uint64_t clkRate);

/**
 *
 * \brief DDR4 Initialization function
 *
 * Initializes the DDR timing parameters. Sets the DDR timing parameters
 * based in the DDR PLL controller configuration done by the board library.
 * Any changes to DDR PLL requires change to DDR timing. Also supports
 * enabling ECC
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_DDRInit(Bool eccEnable);

/**
 * \brief clock Initialization function for MCU domain
 *
 * Enables different power domains and peripheral clocks of the MCU.
 * Some of the power domains and peripherals will be OFF by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK              - Clock initialization sucessful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
Board_STATUS Board_moduleClockInitMcu(void);

/**
 * \brief clock Initialization function for MAIN domain
 *
 * Enables different power domains and peripheral clocks of the SoC.
 * Some of the power domains and peripherals will be OFF by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK              - Clock initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
Board_STATUS Board_moduleClockInitMain(void);

/**
 * \brief clock de-initialization function for MCU domain
 *
 * Disables different power domains and peripheral clocks of the SoC.
 *
 * \return  BOARD_SOK              - Clock de-initialization successful. 
 *          BOARD_INIT_CLOCK_FAIL  - Clock de-initialization failed.
 *
 */
Board_STATUS Board_moduleClockDeinitMcu(void);

/**
 * \brief clock de-initialization function for MAIN domain
 *
 * Disables different power domains and peripheral clocks of the SoC.
 *
 * \return  BOARD_SOK              - Clock de-initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock de-initialization failed.
 *
 */
Board_STATUS Board_moduleClockDeinitMain(void);

/**
 * \brief  Board specific configurations for Gigabit Ethernet PHYs
 *
 * This function takes care of configuring the internal delays for gigabit
 * Ethernet PHY. 2.25ns delay is configured for Rx, and .25ns delay is
 * configured for Tx
 *
 * \return  none
 */
Board_STATUS Board_mcuEthConfig(void);

/**
 * \brief  Board specific configurations for ICSS EMAC Ethernet PHYs
 *
 *  This function takes care of making several board level configurations
 *  required for ICSS EMAC PHY as listed below.
 *   - Disabling internal SoC pull up/down for the pins used for strapping
 *   - Setting the GPIOs for PHY reset, routing ICSS signals to PHYs,
 *     PHY interrupt lines.
 *   - Resetting the PHYs for proper address latching
 *   - MDIO initialization
 *   - MDIO configuration for setting MII mode
 *
 * \return  none
 */
Board_STATUS Board_icssEthConfig(void);

/**
 * \brief   This function initializes the default UART instance for use for
 *          console operations.
 *
 * \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_uartStdioInit(void);

/**
 *  \brief   This function is used to de-initialize board UART handles.
 */
Board_STATUS Board_uartDeInit(void);

/**
 * \brief   This function initializes the i2c instance connected to the
 *          board Id EEPROM.
 * This function disables the interrupt mode as the Board i2c instance
 * doesn't require interrupt mode and restores back original at the end.
 *
 * \param   i2cInst  [IN]  i2c instance connected to board Id EEPROM and
 *                         IO expander device.
 *
 * \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_internalInitI2C(uint8_t i2cInst);

/**
 *  \brief   This function is used to close all the initialized board I2C handles.
 *
 *  \return  Board_STATUS in case of success or appropriate error code.
 */
Board_STATUS Board_i2cDeInit(void);

/**
  *  \brief   This function initializes the i2c instance connected to
  *           different control modules on the board
  *
  *  This function disables the interrupt mode as the Board i2c instance
  *  doesn't require interrupt mode and restores back original at the end.
  *
  *  \return  Board_STATUS in case of success or appropriate error code.
  *
  */
Board_STATUS Board_i2cInit(void);

/**
 *  \brief   This function is to get the i2c handle of the requested
 *           instance of the specifed domain
 *
 *  \param   domainType [IN] Domain of I2C controller
 *                             BOARD_SOC_DOMAIN_MAIN - Main Domain
 *                             BOARD_SOC_DOMAIN_WKUP - Wakeup domain
 *                             BOARD_SOC_DOMAIN_MCU - MCU domain
 *
 *  \param   i2cInst    [IN]        I2C instance
 *
 *  \return  Board_STATUS in case of success or appropriate error code.
 *
 */
I2C_Handle Board_getI2CHandle(uint8_t domainType,
                              uint32_t i2cInst);

/**
 * \brief  Unlocks MMR registers
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_unlockMMR(void);

/**
 * \brief  Locks MMR registers
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_lockMMR(void);

/**
 *  \brief Serdes configurations
 *
 *  The function detects the personality boards connected and configures the
 *  respective module.
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfg(void);

/**
 *
 * \brief  Board PLL initialization function
 *
 *  Configures different PLL controller modules. This enables all the PLL
 *  controllers on the SoC with default configurations.
 *
 * \return BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_PLLInitAll(void);

/**
 *  \brief Sets padconfig register of a pin at given offset
 *
 *  Configures whole padconfig register of the pin at given offset
 *  with the value in 'muxData'.
 *
 *  \param   domain  [IN]  SoC domain for pinmux
 *  \n                      BOARD_SOC_DOMAIN_MAIN - Main domain
 *
 *  \param   offset  [IN]  Pad config offset of the pin
 *  \param   muxData [IN]  Value to be written to padconfig register
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxSetReg(uint8_t  domain,
                                uint32_t offset,
                                uint32_t muxData);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BOARD_INTERNAL_H_ */
