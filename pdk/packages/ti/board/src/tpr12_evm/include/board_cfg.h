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
 * details of different interfaces of TPR12 EVM.
 *
 */
#ifndef BOARD_CFG_H_
#define BOARD_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Board ID information */
#define BOARD_INFO_CPU_NAME     "tpr12"
#define BOARD_INFO_BOARD_NAME   "tpr12_evm"

/* UART LLD instance number for MSS UART port */
#define BOARD_MSS_UART_INSTANCE                 (0U)

#define BOARD_MSS_UARTB_INSTANCE                (1U)

/* UART LLD instance number for DSS UART port */
#define BOARD_DSS_UART_INSTANCE                 (0U)

/* I2C instance as master for master/slave test example */
#define BOARD_I2C_MASTER_INSTANCE              (0U)
/* I2C instance as salve for master/slave test example */
#define BOARD_I2C_SLAVE_INSTANCE               (0U)

/* UART LLD instance number for primary UART port */
#if defined (__TI_ARM_V7R4__)
#define BOARD_UART_INSTANCE                     (BOARD_MSS_UART_INSTANCE)
#else
#define BOARD_UART_INSTANCE                     (BOARD_DSS_UART_INSTANCE)
#endif

/* SPI instance connected to PMIC - MSS SPIB */
#define BOARD_SPI_PMIC_INSTANCE                 (1U)

/* QSPI instance connected to OSPI NOR flash */
#define BOARD_QSPI_NOR_INSTANCE                 (0U)

/* Number of LEDS connected to GPIO */
#define BOARD_GPIO_LED_NUM                      (1U)

/* GPIO instance connected to LED */
#define BOARD_GPIO_LED_PORT_NUM                 (0U)

/* GPIO pin connected to LED */
#define BOARD_GPIO_LED_PIN_NUM                  (2U)

/* GPIO instance connected to push button */
#define BOARD_GPIO_SWITCH_PIN_NUM               (28U)

/* MSS EMAC Port number */
#define BOARD_EMAC_PORT_MAX                     (0x1U)
#define BOARD_ETH_PORT                          (0x0U)

/* MCU EMAC PHY MDIO address */
#define BOARD_MCU_EMAC_PHY_ADDR                 (0U)

/* MSS EMAC MAX REG DUMP */
#define BOARD_EMAC_REG_DUMP_MAX                 (16U)

/* MSS EMAC PHY register address definitions for reading strap values */
#define BOARD_EMAC_STRAP_STS1_ADDR              (0x6EU)
#define BOARD_EMAC_STRAP_STS2_ADDR              (0x6FU)

/* MSS_ETH_INTn */
#define BOARD_GPIO_EMAC_INT_PIN_NUM             (2U)        /* MSS GPIO_2 */

/* MSS_ETH_INTn */
#define BOARD_GPIO_EMAC_RST_PIN_NUM             (28U)        /* MSS GPIO_28 */

/* Temperature sensor i2c instance */
#define BOARD_TEMP_SENSOR_I2C_INSTANCE          (0U)

/* Temperature sensor slave device address */
#define BOARD_TEMP_SENSOR_I2C_SLAVE_DEVICE_ADDR (0x49U)

#define BOARD_I2C_CURRENT_MONITOR_INSTANCE      (0U)

/* I2C instance connected to EEPROM */
#define BOARD_I2C_EEPROM_INSTANCE               (0U)

/* I2C address for EEPROM */
#define BOARD_I2C_EEPROM_ADDR                   (0x50U)

/* Number of TPR12 boards supported */
#define BOARD_ID_MAX_BOARDS                     (0x1U)

/* Different SoC domains */
#define BOARD_SOC_DOMAIN_MSS                    (0U)
#define BOARD_SOC_DOMAIN_RCSS                   (1U)
#define BOARD_SOC_DOMAIN_DSS                    (2U)

/* Maximum possible buffer length */
#define BOARD_EEPROM_MAX_BUFF_LENGTH                    (197U)

/* EEPROM board ID information */
#define BOARD_EEPROM_HEADER_FIELD_SIZE          (7U)
#define BOARD_EEPROM_TYPE_SIZE                  (1U)
#define BOARD_EEPROM_STRUCT_LENGTH_SIZE         (2U)
#define BOARD_EEPROM_MAGIC_NUMBER               (0xEE3355AAU)

#define BOARD_BOARD_FIELD_TYPE                  (0x10)
#define BOARD_DDR_FIELD_TYPE                    (0x11)
#define BOARD_MACINFO_FIELD_TYPE                (0x13)
#define BOARD_ENDLIST                           (0xFE)

#define BOARD_EEPROM_HEADER_LENGTH              (4U)
#define BOARD_EEPROM_BOARD_NAME_LENGTH          (8U)
#define BOARD_EEPROM_VERSION_LENGTH             (4U)
#define BOARD_EEPROM_SERIAL_NO_LENGTH           (12U)
#define BOARD_EEPROM_CONFIG_LENGTH              (32U)

#define BOARD_EEPROM_BOARD_NAME_ADDR            (BOARD_EEPROM_HEADER_ADDR + BOARD_EEPROM_HEADER_LENGTH)
#define BOARD_EEPROM_VERSION_ADDR               (BOARD_EEPROM_BOARD_NAME_ADDR + BOARD_EEPROM_BOARD_NAME_LENGTH)
#define BOARD_EEPROM_SERIAL_NO_ADDR             (BOARD_EEPROM_VERSION_ADDR + BOARD_EEPROM_VERSION_LENGTH)
#define BOARD_EEPROM_CONFIG_ADDR                (BOARD_EEPROM_SERIAL_NO_ADDR + BOARD_EEPROM_SERIAL_NO_LENGTH)

#define BOARD_EEPROM_HEADER_ADDR                (0U)

/* Enable NOR flash driver */
#define BOARD_NOR_FLASH_IN

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif   /* BOARD_CFG_H_ */
