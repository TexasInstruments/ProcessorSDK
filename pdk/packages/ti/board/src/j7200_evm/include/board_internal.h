/******************************************************************************
 * Copyright (c) 2020-2022 Texas Instruments Incorporated - http://www.ti.com
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

#define BOARD_I2C_PORT_CNT    (I2C_HWIP_MAX_CNT)

#define BOARD_SOC_DIE_ID0     (CSL_WKUP_CTRL_MMR0_CFG0_BASE + CSL_WKUP_CTRL_MMR_CFG0_DIE_ID0)
#define BOARD_SOC_DIE_ID1     (CSL_WKUP_CTRL_MMR0_CFG0_BASE + CSL_WKUP_CTRL_MMR_CFG0_DIE_ID1)

#define BOARD_SOC_DESIGN_REV_MASK      (0x1FU)
#define BOARD_SOC_DESIGN_REV_SHIFT     (0x0U)
#define BOARD_SOC_FT_FPC1_REV_MASK     (0xFF000000U)
#define BOARD_SOC_FT_FPC1_REV_SHIFT    (0x18U)

#undef ENABLE_LOGS

#if !defined(BOARD_DEBUG_LOG)
#if defined(ENABLE_LOGS)
#define BOARD_DEBUG_LOG     UART_printf
#else
#define BOARD_DEBUG_LOG(x, ...)
#endif
#endif  /* #if !defined(BOARD_DEBUG_LOG) */

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

#define BOARD_KICK0_UNLOCK_VAL                  (0x68EF3490U)
#define BOARD_KICK1_UNLOCK_VAL                  (0xD172BC5AU)

#define BOARD_KICK0_LOCK_VAL                    (0U)
#define BOARD_KICK1_LOCK_VAL                    (0U)

#define BOARD_MAC_COUNT_SHIFT                   (3U)
#define BOARD_MAC_COUNT_MASK                    (0x00F8U)

/* Ethenet board library MACROs */
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
#define BOARD_ETHPHY_CPSW5G_DELAY               (0xA9U)
#define BOARD_ETHPHY_CPSW2G_DELAY               (0x77U)
#define BOARD_ETHPHY_DELAY_CTRL                 (0xD3U)
#define BOARD_ETHPHY_IO_IMPEDANCE               (0x0C1FU)
#define BOARD_CPSW_MDIO_REG_OFFSET              (0xF00U)

#define BOARD_MDIO_CTRL_REG_OFFSET              (0x4U)
#define BOARD_MDIO_CLK_DIV_CFG                  (0xFFU)
#define BOARD_EMAC_DELAY_CFG                    (0x01000000U)

#define BOARD_ETHPHY_STRAP_FLD_MASK             (0x0400U)
#define BOARD_ETHPHY_STRAP_FLD_THS_CHECK_FLAG   (0x222U)

#define BOARD_MMR_PARTITION_MAX                 (8U)
#define BOARD_MMR_PARTITION_SIZE                (0x4000U)

#define BOARD_MMR_PARTITION0                    (0x0U)
#define BOARD_MMR_PARTITION1                    (0x1U)
#define BOARD_MMR_PARTITION2                    (0x2U)
#define BOARD_MMR_PARTITION3                    (0x3U)
#define BOARD_MMR_PARTITION4                    (0x4U)
#define BOARD_MMR_PARTITION5                    (0x5U)
#define BOARD_MMR_PARTITION6                    (0x6U)
#define BOARD_MMR_PARTITION7                    (0x7U)

/*****************************************************************************
 * Function Prototypes                                                       *
 *****************************************************************************/


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
 *
 * \brief  Board pinmuxing enable function
 *
 * Enables pinmux for the Maxwell idk board interfaces. Pin mux is done based on the
 * default/primary functionality of the board. Any pins shared by multiple
 * interfaces need to be reconfigured to access the secondary functionality.
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxConfig(void);

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

#ifdef BOARD_DDR_ENABLE_DDR_MEM_PRIME
/**
 *
 * \brief DDR4 Prime with UDMA Function
 *
 * Prime DDR memory using UDMA for ECC 
 * 
 * \param   startAddr [in] Base Address 
 * \param   size      [in] Size in bytes
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS BOARD_udmaPrimeDDR(void *startAddr, uint32_t size);
#endif

/**
 * \brief clock Initialization function for MCU domain
 *
 * Enables different power domains and peripheral clocks of the MCU.
 * Some of the power domains and peripherals will be OFF by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK              - Clock initialization successful.
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
 * Disables different power domains and peripheral clocks of the MCU.
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
 * \brief  Board specific configurations for CPSW2G Ethernet PHY
 *
 * This function takes care of configuring the internal delays for MCU gigabit
 * Ethernet PHY
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_cpsw2gEthPhyConfig(void);

/**
 * \brief  Board specific configurations for ICSS EMAC Ethernet PHYs
 *
 * This function takes care of configuring the internal delays for ICSS
 * Ethernet PHY
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_icssEthPhyConfig(void);

/**
 * \brief   This function initializes the default UART instance for use for
 *          console operations.
 *
 * \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_uartStdioInit(void);

/**
 * \brief  Locks MMR registers
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_lockMMR(void);

/**
 * \brief  Unlocks MMR registers
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_unlockMMR(void);

/**
 * \brief  Locks MMR register partition
 *
 *  \param   domain  [IN]  SoC domain for MMR register space
 *  \n                      BOARD_SOC_DOMAIN_MAIN - Main domain
 *  \n                      BOARD_SOC_DOMAIN_MCU  - MCU domain
 *  \n                      BOARD_SOC_DOMAIN_WKUP - Wakeup domain
 *
 *  \param   partNum [IN]  Partition number
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_lockMMRPartition(uint32_t domain, uint32_t partNum);

/**
 * \brief  Unlocks MMR register partition
 *
 *  \param   domain  [IN]  SoC domain for MMR register space
 *  \n                      BOARD_SOC_DOMAIN_MAIN - Main domain
 *  \n                      BOARD_SOC_DOMAIN_MCU  - MCU domain
 *  \n                      BOARD_SOC_DOMAIN_WKUP - Wakeup domain
 *
 *  \param   partNum [IN]  Partition number
 *
 * \return  Board_STATUS
 */

Board_STATUS Board_unlockMMRPartition(uint32_t domain, uint32_t partNum);

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
 * \brief  Board PLL initialization function for MCU domain
 *
 *  Configures different PLL controller modules.  This enables all the PLL
 *  controllers in the MCU domain with default configurations.
 *
 * \return BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_PLLInitMcu(void);

/**
 *
 * \brief  Board PLL initialization function for MAIN domain
 *
 *  Configures different PLL controller modules.  This enables all the PLL
 *  controllers in the MAIN domain with default configurations.
 *
 * \return BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_PLLInitMain(void);

/**
 * \brief   Print out PLL configurations- all multiplier and divider values.
 *
 * \param   none
 *
 * \return  none
 */
Board_STATUS Board_getAllPLLConfigurations();

/**
 * \brief Board detect function
 *
 *  Checks if the board with given 'boardID' is connected to the
 *  base board.
 *
 *  \n Note: Board ID EEPROM should be programmed for this function
 *           to work properly.
 *
 * \param   boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_SOM(0x0) - Dual PMIC SoM Board
 * \n                      BOARD_ID_GESI(0x1) - GESI Board
 * \n                      BOARD_ID_INFOTAINMENT(0x2) - Infotainment board
 * \n                      BOARD_ID_FUSION2(0x3) - Fusion 2 Board
 * \n                      BOARD_ID_MV(0x4) - MV expansion
 * \n                      BOARD_ID_LI(0x5) - LI expansion
 * \n                      BOARD_ID_ENET(0x6) - Quad ENET expansion
 * \n                      BOARD_ID_DISPLAY(0x7) - Display adapter board 
 *
 * \return   BTRUE if the given board is detected else BFALSE.
 *           SoM board will be always connected to the base board.
 *           For SoM boardID return value BTRUE indicates dual PMIC
 *           SoM and BFALSE indicates alternate PMIC SoM
 *
 */
bool Board_detectBoard(uint32_t boardID);

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
 *  \brief   This function is used to close all the initialized board I2C handles.
 *
 *  \return  Board_STATUS in case of success or appropriate error code.
 */
Board_STATUS Board_i2cDeInit(void);

/**
 *  \brief    Function to programs VDD_CORE to 0.9V.
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pmVDDCoreVoltageCfg(void);

/**
 *  \brief    Function to configure SD card voltage.
 *
 *  \param vsel [IN] SD voltage selection. 0 for 1.8v, 1 for 3.3v
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pmSdVoltageCtrl(uint8_t vsel);

/**
 *  \brief   This function is used to de-initialize board UART handles.
 */
Board_STATUS Board_uartDeInit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BOARD_INTERNAL_H_ */
