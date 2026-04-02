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
 * \brief  Board library configurations
 *
 * This file configures the instance numbers, address and gpio reset
 * details of different interfaces of the board.
 *
 */
#ifndef BOARD_CFG_H_
#define BOARD_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/board/src/am64x_svb/include/board_pinmux.h>

/* Board ID information */
#define BOARD_INFO_CPU_NAME     "am64x"
#define BOARD_INFO_BOARD_NAME   "am64x_svb"

#define BOARD_SOC_DDR_START_ADDR                        (0x80000000U)

/* Memory sections */
#define BOARD_DDR_START_ADDR                            (0x80000000U)
#define BOARD_DDR_SIZE                                  (2048 * 1024 * 1024UL)
#define BOARD_DDR_END_ADDR                              (0xFFFFFFFFU)

/* Note with ECC enabled, all memory is not usable: 1/8 memory used for inline ECC */
#define BOARD_DDR_ECC_END_ADDR                          (0xF1FFFFFFU)

/* UART LLD instance number for MAIN UART0 port */
#define BOARD_UART0_INSTANCE                            (0U)
/* UART LLD instance number for MAIN UART1 port */
#define BOARD_UART1_INSTANCE                            (1U)
/* UART LLD instance number for MAIN UART2 port */
#define BOARD_UART2_INSTANCE                            (2U)
/* UART LLD instance number for MAIN UART3 port */
#define BOARD_UART3_INSTANCE                            (3U)
/* UART LLD instance number for MCU UART0 port */
#define BOARD_MCU_UART0_INSTANCE                        (7U)
/* UART LLD instance number for MCU UART1 port */
#define BOARD_MCU_UART1_INSTANCE                        (8U)

/* RS485 instance number for MAIN RS4850 port */
#define BOARD_RS485_INSTANCE1                            (0U)
/* RS485 instance number for MAIN RS4851 port */
#define BOARD_RS485_INSTANCE2                            (1U)

/* UART LLD instance number for primary UART port */
#if defined (BUILD_MCU)
#define BOARD_UART_INSTANCE (BOARD_UART0_INSTANCE)
#elif defined (BUILD_MPU)
#define BOARD_UART_INSTANCE (BOARD_UART1_INSTANCE)
#elif defined (BUILD_M4F)
#define BOARD_UART_INSTANCE (BOARD_MCU_UART0_INSTANCE)
#else
#define BOARD_UART_INSTANCE (BOARD_UART0_INSTANCE)  /* Use MAIN UART0 by default */
#endif

/* Clock frequency for UART module */
#define BOARD_UART_CLK_MAIN                             (48000000U)

/** @brief Number of UART instances */
#define BOARD_UART_PER_CNT                              (6U)

/* ICSSG UART instance number */
#define BOARD_ICSSG_UART_INSTANCE                       (4U)

/* I2C instance connected to EEPROM - WKUP I2C0 */
#define BOARD_I2C_EEPROM_INSTANCE                       (0U)

/* I2C instance for BOOT EEPROM */
#define BOARD_I2C_BOOT_EEPROM_INSTANCE                  (2U)

/* I2C address for Boot EEPROM */
#define BOARD_I2C_BOOT_EEPROM_ADDR                      (0x54U)
#define BOARD_I2C_BOOT_EEPROM_ADDR2                     (0x51U)

/* I2C address for Board Id EEPROM */
#define BOARD_I2C_EEPROM_ADDR                           (0x51U)

/* QSPI instance number */
#define BOARD_SPI_NOR_INSTANCE                          (0U)

/* I2C Instance connected to clock generator */
#define BOARD_CLOCK_GENERATOR_INSTANCE                  (0U)
/* I2C slave address of clock generator */
#define BOARD_I2C_CLOCK_GENERATOR1                      (0)   //Can be used later
#define BOARD_I2C_CLOCK_GENERATOR2_ADDR1                (0x76)
#define BOARD_I2C_CLOCK_GENERATOR2_ADDR2                (0x77)
#define BOARD_I2C_QSGMII_CLOCK_GENERATOR                (0x77)
#define BOARD_I2C_PERI_CLOCK_GENERATOR                  (0x6D)

/* OSPI instance connected to OSPI NOR flash */
#define BOARD_OSPI_NOR_INSTANCE                         (0U)

/* I2C instance connected to IO Expander */
#define BOARD_I2C_IOEXP_DEVICE1_INSTANCE                (1U)
#define BOARD_I2C_IOEXP_DEVICE2_INSTANCE                (0U)
#define BOARD_I2C_IOEXP_DEVICE3_INSTANCE                (3U)
#define BOARD_I2C_IOEXP_DEVICE4_INSTANCE                (0U)
#define BOARD_I2C_IOEXP_DEVICE5_INSTANCE                (1U)
#define BOARD_I2C_AUDIO_IOEXP_DEVICE_INSTANCE           (3U)
#define BOARD_I2C_VIDEO_IOEXP_DEVICE_INSTANCE           (0U)

/* I2C IO Expander Slave devices */
#define BOARD_I2C_IOEXP_DEVICE1_ADDR                    (0x22U)
/* Used to configure industrial LED's */
#define BOARD_I2C_IOEXP_DEVICE2_ADDR                    (0x60U)
/* Used to configure industrial LED's */
#define BOARD_I2C_IOEXP_DEVICE3_ADDR                    (0x61U)

/* I2C instance industrial led */
#define BOARD_I2C_IOEXP_LED_INSTANCE                    (2U)

/* OSPI instance number */
#define BOARD_OSPI_INSTANCE                             (0)

#define BOARD_GPIO_IOEXP_OSPI_RST_PORT_NUM              (0)
#define BOARD_GPIO_IOEXP_OSPI_RST_PIN_NUM               (1)

/* GPIO port & pin numbers for  MMC reset */
#define GPIO_MMC_SDCD_PORT_NUM                          (0) //AM64x_TODO: Need to remove
#define GPIO_MMC_SDCD_PIN_NUM                           (0) //AM64x_TODO: Need to remove

#define BOARD_GPIO_IOEXP_EMMC_RST_PORT_NUM              (0x0)
#define BOARD_GPIO_IOEXP_EMMC_RST_PIN_NUM               (0x0)

/* I2C instance for External RTC */
#define BOARD_I2C_EXT_RTC_INSTANCE                      (0U)

/* I2C address for External RTC */
#define BOARD_I2C_EXT_RTC_ADDR                          (0x6FU)

/* I2C instance Board Presence Circuit */
#define BOARD_PRES_I2C_INSTANCE                         (0U)
/* I2C address Board Presence Circuit */
#define BOARD_PRES_DETECT_SLAVE_ADDR                    (0x38U)

/* Pin and port numbers for TEST_LED1 on CP board */
#define BOARD_GPIO_TEST_LED1_PORT_NUM           	    (0)       /* Port 0 */
#define BOARD_GPIO_TEST_LED1_PIN_NUM                    (0x01)    /* Pin 1  */
/* Pin and port numbers for TEST_LED2 on CP board */
#define BOARD_MCU_GPIO_TEST_LED2_PORT_NUM               (0)       /* Port 0 */
#define BOARD_MCU_GPIO_TEST_LED2_PIN_NUM                (0x05)    /* Pin 5  */

/* User LED Pin Details */
#define BOARD_I2C_USER_LED_INSTANCE                     (0U)

#define BOARD_USER_LED1                                 (1U) /* Main GPIO0_1 */
#define BOARD_USER_LED2                                 (1U) /* MCU GPIO0_1 */

#define BOARD_ICSS_EMAC_PORT_START                      (0x0U)
#define BOARD_ICSS_EMAC_PORT_END                        (0x5U)
#define BOARD_ICSS_EMAC_PORT_MAX                        (0x6U)
#define BOARD_MCU_EMAC_PORT_MAX                         (0x1U)
#define BOARD_MCU_ETH_PORT                              (0x1U) //AM64x_TODO: need to update


/* ICSS2 EMAC PHY register address */
#define BOARD_ICSS2_EMAC_PHY0_ADDR                      (0x0)
#define BOARD_ICSS2_EMAC_PHY1_ADDR                      (0x3u)


/* PRG1_RGMII2_RESETn */
#define BOARD_GPIO_IOEXP_ICSS1_EMAC_RST_PORT_NUM        (0)
#define BOARD_GPIO_IOEXP_ICSS1_EMAC_RST_PIN_NUM         (4)

/* PRG1_RGMII1_RESETn */
#define BOARD_GPIO_IOEXP_ICSS2_EMAC_RST_PORT_NUM        (0)
#define BOARD_GPIO_IOEXP_ICSS2_EMAC_RST_PIN_NUM         (3)

/* PRG1_RGMII_INTn */
#define BOARD_GPIO_ICSS2_EMAC_INT_PORT_NUM              (0) /* GPIO1_70  */
#define BOARD_GPIO_ICSS2_EMAC_INT_PIN_NUM               (0x46)

/* PRG1_ETH1_LED_LINK */
#define BOARD_GPIO_ICSS2_EMAC_PHY0_LED_LINK_PORT_NUM    (0)  /* PRG1_PRU0_GPO8 - GPIO0_53 */
#define BOARD_GPIO_ICSS2_EMAC_PHY0_LED_LINK_PIN_NUM     (35)

/* PRG1_ETH2_LED_LINK */
#define BOARD_GPIO_ICSS2_EMAC_PHY1_LED_LINK_PORT_NUM    (0)  /* PRG1_PRU1_GPO8 - GPIO0_73 */
#define BOARD_GPIO_ICSS2_EMAC_PHY1_LED_LINK_PIN_NUM     (49)

/* GPIO to drive PRG2 LED0 */
#define BOARD_GPIO_ICSS2_EMAC_LED0_PORT_NUM             (0) //AM64x_TODO: need to update
#define BOARD_GPIO_ICSS2_EMAC_LED0_PIN_NUM              (0) //AM64x_TODO: need to update

/* GPIO to drive PRG2 LED1 */
#define BOARD_GPIO_ICSS2_EMAC_LED1_PORT_NUM             (0) //AM64x_TODO: need to update
#define BOARD_GPIO_ICSS2_EMAC_LED1_PIN_NUM              (0) //AM64x_TODO: need to update

/* GPIO to drive PRG2 LED2 */
#define BOARD_GPIO_ICSS2_EMAC_LED2_PORT_NUM             (0) //AM64x_TODO: need to update
#define BOARD_GPIO_ICSS2_EMAC_LED2_PIN_NUM              (0) //AM64x_TODO: need to update

/* GPIO to drive PRG2 LED3 */
#define BOARD_GPIO_ICSS2_EMAC_LED3_PORT_NUM             (0) //AM64x_TODO: need to update
#define BOARD_GPIO_ICSS2_EMAC_LED3_PIN_NUM              (0) //AM64x_TODO: need to update

/* MCU EMAC PHY MDIO address */
#define BOARD_MCU_EMAC_PHY_ADDR                         (0U)

/* MCU EMAC MAX REG DUMP */
#define BOARD_MCU_EMAC_REG_DUMP_MAX                     (16)

/* MCU EMAC PHY register address definitions for reading strap values */
#define BOARD_MCU_EMAC_STRAP_STS1_ADDR                  (0x6EU)
#define BOARD_MCU_EMAC_STRAP_STS2_ADDR                  (0x6FU)

/* MCU EMAC PHY register address definitions for reading strap values */
#define BOARD_ICSS_EMAC_STRAP_STS1_ADDR                 (0x6EU)
#define BOARD_ICSS_EMAC_STRAP_STS2_ADDR                 (0x6FU)

/* MCU_ETH1_RESETn --> GPIO_CPSW1_RST */
#define BOARD_GPIO_IOEXP_MCU_EMAC_RST_PORT_NUM          (0)
#define BOARD_GPIO_IOEXP_MCU_EMAC_RST_PIN_NUM           (2)

/* MCU_ETH1_INTn -> CPSW_RGMII_INTn */
#define BOARD_GPIO_MCU_EMAC_INT_PORT_NUM                (1) /* EXTINTn - GPIO1_70 */
#define BOARD_GPIO_MCU_EMAC_INT_PIN_NUM                 (46)

/* AUTOMATION HEADER */
#define BOARD_TEST_HEADER_I2C_INSTANCE                  (1U)
#define BOARD_I2C_BOOT_MODE_SW_ADDR                     (0x22U)

/* TEST_GPIO2 */
#define BOARD_GPIO_IOEXP_TEST_PORT_NUM                  (1)
#define BOARD_GPIO_IOEXP_TEST_PIN_NUM                   (5)

/* Temperature sensor i2c instance */
#define BOARD_TEMP_SENSOR_I2C_INSTANCE                  (1U)

/* Temperature sensor slave device addresses */
#define BOARD_TEMP_SENSOR_I2C_SLAVE_DEVICE1_ADDR        (0x48U)
#define BOARD_TEMP_SENSOR_I2C_SLAVE_DEVICE2_ADDR        (0x49U)

#define BOARD_I2C_CURRENT_MONITOR_INSTANCE              (1U)

/* Instance for interfaces connected to MMCSD */
#define BOARD_MMCSD_SD_INSTANCE                         (1U)
#define BOARD_MMCSD_EMMC_INSTANCE                       (0)

/* GPMC instance connected to the flash */
#define BOARD_GPMC_INSTANCE                             (0U)

/* Number of LEDS connected to GPIO */
#define BOARD_GPIO_LED_NUM                              (2U)

/* SPI instance connected to EEPROM */
#define BOARD_EEPROM_SPI0_PORT                          (0U)

/* Enable NOR flash driver */
#define BOARD_NOR_FLASH_IN

/* Enable NAND flash driver */
#define BOARD_NAND_FLASH_IN

/* McSPI instance for master and slave test */
#define BOARD_MCSPI_MASTER_INSTANCE                     (1) //J7ES_TODO: need to update
#define BOARD_MCSPI_SLAVE_INSTANCE                      (1) //J7ES_TODO: need to update

/* Maximum possible buffer length */
#define BOARD_EEPROM_MAX_BUFF_LENGTH                    (0) //J7ES_TODO: need to update

/* Maximum number of SoC domains */
#define BOARD_SOC_DOMAIN_MAX                            (2U)
/* Value for indicating SoC main domain */
#define BOARD_SOC_DOMAIN_MAIN                           (0)
/* Value for indicating SoC MCU domain */
#define BOARD_SOC_DOMAIN_MCU                            (1U)
/* Maximum I2C instance number common across the domain */
#define BOARD_I2C_DOMAIN_INSTANCE_MAX                   (2U)

/* SoC domain used by UART module */
#if defined (BUILD_M4F)
#define BOARD_UART_SOC_DOMAIN    (BOARD_SOC_DOMAIN_MCU)
#else
#define BOARD_UART_SOC_DOMAIN    (BOARD_SOC_DOMAIN_MAIN)
#endif

/* EEPROM board ID information */
#define BOARD_EEPROM_HEADER_FIELD_SIZE                  (7U)
#define BOARD_EEPROM_TYPE_SIZE                          (1U)
#define BOARD_EEPROM_STRUCT_LENGTH_SIZE                 (2U)
#define BOARD_EEPROM_MAGIC_NUMBER                       (0xEE3355AA)

#define BOARD_BOARD_FIELD_TYPE                          (0x10)
#define BOARD_DDR_FIELD_TYPE                            (0x11)
#define BOARD_MACINFO_FIELD_TYPE                        (0x13)
#define BOARD_ENDLIST                                   (0xFE)

#define BOARD_EEPROM_HEADER_ADDR                        (0U)

/* Board detect ID for FMZ board */
#define BOARD_ID_FMZ                                    (1U)
/* Board detect ID for VPS board */
#define BOARD_ID_VPS                                    (2U)
/* Board detect ID for Serdes SMA board */
#define BOARD_ID_SERDES_SMA                             (3U)
/* Board detect ID for SVB Controller board */
#define BOARD_ID_SVB_CNTR                               (4U)
/* Board detect ID for DDR4 board */
#define BOARD_ID_DDR4                                   (5U)
/* Board detect ID for DUT board */
#define BOARD_ID_DUT                                    (6U)
/* Board detect ID for LPDDR4 board */
#define BOARD_ID_LPDDR4                                 (7U)
/* Board detect ID for Serdes PCIe X1 board */
#define BOARD_ID_SERDES_PCIE_X1                         (8U)
/* Board detect ID for Serdes USB Type A board */
#define BOARD_ID_SERDES_USBA                            (9U)
/* Board detect ID for Serdes USB Type AB board */
#define BOARD_ID_SERDES_USBAB                           (10U)

/* Maximum number of supporting board ID */
#define BOARD_ID_MAX_BOARDS      (0x10U)

/* I2C instance connected to OLED DISPLAY */
#define BOARD_OLED_DISPLAY_INSTANCE                     (1)

/* PinMux data to be programmed to configure a pin to be a GPIO */
#define PINMUX_GPIO_CFG                                 (0x00050007U)

/* Clock frequency for UART module */
#define BOARD_UART_CLK_MAIN                             (48000000U) //AM64x_TODO: Need to cross check

/** @brief Number of UART instances */
#define BOARD_UART_PER_CNT                              (6U) //AM64x_TODO: Need to verify the Value

/* PinMux data to be programmed to configure a pin to be a GPIO */
#define PINMUX_GPIO_CFG                                 (0x00050007U)

typedef enum domainType
{
    MAIN_DOMAIN = 0U,
    WKUP_DOMAIN
}domainType_t;

typedef enum
{
	FMZ_BRD_DETECT = 1,
	VPS_BRD_DETECT = 2,
	SERDES_BRD_DETECT = 3,
	SVB_CNTR_BRD_DETECT = 4,
    DDR_DIMM_BRD_DETECT = 5,
    DUT_BRD_DETECT = 6
}boardPresDetect_t;

#define BOARD_EEPROM_HEADER_LENGTH                      (4U)
#define BOARD_EEPROM_BOARD_NAME_LENGTH                  (16U)
#define BOARD_EEPROM_VERSION_LENGTH                     (2U)
#define BOARD_EEPROM_SERIAL_NO_LENGTH                   (12U)
#define BOARD_EEPROM_CONFIG_LENGTH                      (32U)

#define BOARD_EEPROM_BOARD_NAME_ADDR                    (BOARD_EEPROM_HEADER_ADDR + BOARD_EEPROM_HEADER_LENGTH)
#define BOARD_EEPROM_VERSION_ADDR                       (BOARD_EEPROM_BOARD_NAME_ADDR + BOARD_EEPROM_BOARD_NAME_LENGTH)
#define BOARD_EEPROM_SERIAL_NO_ADDR                     (BOARD_EEPROM_VERSION_ADDR + BOARD_EEPROM_VERSION_LENGTH)
#define BOARD_EEPROM_CONFIG_ADDR                        (BOARD_EEPROM_SERIAL_NO_ADDR + BOARD_EEPROM_SERIAL_NO_LENGTH)

#define BOARD_DUT_EEPROM_SLAVE_ADDR                     (0x50)
#define BOARD_VPS_EEPROM_SLAVE_ADDR                     (0x31)
#define BOARD_FMZ_EEPROM_SLAVE_ADDR                     (0x32)
#define BOARD_SERDES_EEPROM_SLAVE_ADDR                  (0x55)
#define BOARD_DDR_DIMM_EEPROM_SLAVE_ADDR                (0x51)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif   /* BOARD_CFG_H_ */

