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

#include <ti/board/board.h>
#include <ti/board/src/j7200_evm/include/board_pinmux.h>
#include <ti/csl/soc/j7200/src/cslr_wkup_ctrl_mmr.h>

/* Board ID information */
#define BOARD_INFO_CPU_NAME     "j7200"
#define BOARD_INFO_BOARD_NAME   "j7200_evm"

/* Memory sections */
#define BOARD_DDR_START_ADDR                            (0x80000000U)
#define BOARD_DDR_SIZE                                  (2048 * 1024 * 1024UL)
#define BOARD_DDR_END_ADDR                              (0xFFFFFFFFU)
/* Note with ECC enabled, all memory is not usable: 1/8 memory used for inline ECC */
#define BOARD_DDR_ECC_END_ADDR                          (0xF1C71C00U)
/* Enable/Disable DDR Memory Prime for ECC. Define the following macro to enable. */ 
#undef BOARD_DDR_ENABLE_DDR_MEM_PRIME

/* MAIN I2C0 instance number */
#define BOARD_MAIN_I2C0_INSTANCE                        (0U)
/* MAIN I2C1 instance number */
#define BOARD_MAIN_I2C1_INSTANCE                        (1U)
/* MAIN I2C2 instance number */
#define BOARD_MAIN_I2C2_INSTANCE                        (2U)
/* MAIN I2C3 instance number */
#define BOARD_MAIN_I2C3_INSTANCE                        (3U)
/* MCU I2C0 instances number */
#define BOARD_MCU_I2C0_INSTANCE                         (0U)
/* WKUP I2C0 instance number */
#define BOARD_WKUP_I2C0_INSTANCE                        (0U)

/* MCU I3C0 instance */
#define BOARD_MCU_I3C0_INSTANCE							(0U)

/* UART LLD instance number for primary UART port */
#define BOARD_UART_INSTANCE                             (0U)
/* UART LLD instance number for MAIN UART1 port */
#define BOARD_UART1_INSTANCE                            (1U)
/* UART LLD instance number for MAIN UART3 port */
#define BOARD_UART3_INSTANCE                            (3U)
/* UART LLD instance number for MAIN UART5 port */
#define BOARD_UART5_INSTANCE                            (5U)
/* UART LLD instance number for MAIN UART6 port */
#define BOARD_UART6_INSTANCE                            (6U)
/* UART LLD instance number for MAIN UART7 port */
#define BOARD_UART7_INSTANCE                            (7U)
/* UART LLD instance number for MAIN UART9 port */
#define BOARD_UART9_INSTANCE                            (9U)
/* MCU UART instance number */
#define BOARD_MCU_UART_INSTANCE                         (0U)
/* WKUP UART LLD instance number */
#define BOARD_WKUP_UART_INSTANCE                        (0U)

/* I2C instance connected to EEPROM */
#define BOARD_I2C_EEPROM_INSTANCE                       (BOARD_WKUP_I2C0_INSTANCE)
/* I2C instance connected to BOOT EEPROM */
#define BOARD_I2C_BOOT_EEPROM_INSTANCE                  (BOARD_MCU_I2C0_INSTANCE)
/* I2C instance connected to PMIC */
#define BOARD_I2C_PMIC_INSTANCE                         (BOARD_MCU_I2C0_INSTANCE)
/* I2C instance connected to Temperature sensor */
#define BOARD_TEMP_SENSOR_I2C_INSTANCE                  (BOARD_MCU_I2C0_INSTANCE)
/* I2C Instance connected to clock generator */
#define BOARD_CLOCK_GENERATOR_INSTANCE                  (BOARD_MAIN_I2C0_INSTANCE)
/* I2C instance connected to External RTC */
#define BOARD_I2C_EXT_RTC_INSTANCE                      (BOARD_MAIN_I2C0_INSTANCE)
/* I2C instance connected to IO Expanders */
#define BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE            (BOARD_MAIN_I2C0_INSTANCE)
#define BOARD_I2C_IOEXP_DEVICE1_INSTANCE                (BOARD_MAIN_I2C0_INSTANCE)
#define BOARD_I2C_IOEXP_DEVICE2_INSTANCE                (BOARD_MAIN_I2C0_INSTANCE)
#define BOARD_I2C_IOEXP_DEVICE3_INSTANCE                (BOARD_MAIN_I2C1_INSTANCE)
#define BOARD_I2C_AUDIO_IOEXP_DEVICE_INSTANCE           (BOARD_MAIN_I2C1_INSTANCE)

/* I2C instance connected to User LED */
#define BOARD_I2C_USER_LED_INSTANCE                     (BOARD_I2C_IOEXP_DEVICE2_INSTANCE)
/* I2C instance connected to Automation header */
#define BOARD_TEST_HEADER_I2C_INSTANCE                  (BOARD_MAIN_I2C2_INSTANCE)
/* I2C instance connected to Current monitor*/
#define BOARD_I2C_CURRENT_MONITOR_INSTANCE              (BOARD_MAIN_I2C2_INSTANCE)

/* SOM Board ID EEPROM slave address */
#define BOARD_SOM_EEPROM_SLAVE_ADDR                     (0x50U)
/* CPB Board ID EEPROM slave address */
#define BOARD_CP_EEPROM_SLAVE_ADDR                      (0x51U)
/* GESI Board ID EEPROM slave address */
#define BOARD_GESI_EEPROM_SLAVE_ADDR                    (0x52U)
/* QSGMII Board ID EEPROM slave address */
#define BOARD_ENET_EEPROM_SLAVE_ADDR                    (0x54U)

/* I2C slave address for Boot EEPROM */
#define BOARD_I2C_BOOT_EEPROM_ADDR                      (0x50U)
#define BOARD_I2C_BOOT_EEPROM_ADDR2                     (0x51U)

/* I2C slave address of clock generator */
#define BOARD_I2C_CLOCK_GENERATOR1                      (0x00U)
#define BOARD_I2C_CLOCK_GENERATOR2_ADDR1                (0x76U)
#define BOARD_I2C_CLOCK_GENERATOR2_ADDR2                (0x77U)
#define BOARD_I2C_QSGMII_CLOCK_GENERATOR_ADDR1          (0x76U)
#define BOARD_I2C_QSGMII_CLOCK_GENERATOR_ADDR2          (0x77U)
#define BOARD_I2C_PERI_CLOCK_GENERATOR                  (0x6DU)

/* I2C IO Expander Slave devices */
#define BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR                (0x21U)
#define BOARD_I2C_IOEXP_DEVICE1_ADDR                    (0x20U)
#define BOARD_I2C_IOEXP_DEVICE2_ADDR                    (0x22U)
#define BOARD_I2C_IOEXP_DEVICE3_ADDR                    (0x20U)
#define BOARD_I2C_AUDIO_IOEXP_DEVICE_ADDR               (0x21U)

#define BOARD_I2C_FPD_UB926_INSTANCE                    (1U)
#define BOARD_I2C_FPD_UB926_ADDR                        (0x2CU)

/* I2C address for External RTC */
#define BOARD_I2C_EXT_RTC_ADDR                          (0x6FU)

/* I2C address for Automation header */
#define BOARD_I2C_BOOT_MODE_SW_ADDR                     (0x22U)

/* I2C address for Temperature sensor */
#define BOARD_TEMP_SENSOR_I2C_SLAVE_DEVICE1_ADDR        (0x48U)
#define BOARD_TEMP_SENSOR_I2C_SLAVE_DEVICE2_ADDR        (0x49U)

/* I2C slave address of Leo PMICs */
#define BOARD_I2C_LEO_PMIC_A_ADDR                       (0x48U)
#define BOARD_I2C_LEO_PMIC_B_ADDR                       (0x4CU)
#define BOARD_I2C_LEO_PMIC_A_WDT_ADDR                   (0x12U)
#define BOARD_I2C_LEO_PMIC_B_WDT_ADDR                   (0x13U)

/* Number of LEDS connected to IO expander on CP board */
#define BOARD_GPIO_LED_NUM                              (2U)

/* Instance for interfaces connected to MMCSD */
#define BOARD_MMCSD_SD_INSTANCE                         (1U)
#define BOARD_MMCSD_EMMC_INSTANCE                       (0U)

/* MCU EMAC PHY MDIO address */
#define BOARD_MCU_EMAC_PHY_ADDR                         (0U)

/* MCU EMAC MAX REG DUMP */
#define BOARD_MCU_EMAC_REG_DUMP_MAX                     (16U)

/* MCU EMAC PHY register address definitions for reading strap values */
#define BOARD_MCU_EMAC_STRAP_STS1_ADDR                  (0x6EU)
#define BOARD_MCU_EMAC_STRAP_STS2_ADDR                  (0x6FU)

#define BOARD_SGMII_PORT_MAX                            (4U)

/* Maximum number of SoC domains */
#define BOARD_SOC_DOMAIN_MAX                            (3U)
/* Value for indicating SoC main domain */
#define BOARD_SOC_DOMAIN_MAIN                           (0U)
/* Value for indicating SoC wake-up domain */
#define BOARD_SOC_DOMAIN_WKUP                           (1U)
/* Value for indicating SoC MCU domain */
#define BOARD_SOC_DOMAIN_MCU                            (2U)
/* Maximum I2C instance number common across the domain */
#define BOARD_I2C_DOMAIN_INSTANCE_MAX                   (2U)

/* SoC domain used by UART module */
#if (__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R') && defined(__ARM_FEATURE_IDIV)
#define BOARD_UART_SOC_DOMAIN                           (BOARD_SOC_DOMAIN_MCU)
#else
#define BOARD_UART_SOC_DOMAIN                           (BOARD_SOC_DOMAIN_MAIN)
#endif

/* Clock frequency for UART module */
#define BOARD_UART_CLK_MAIN                             (48000000U)
#define BOARD_UART_CLK_WKUP                             (96000000U)


/* Board detect ID for GESI */
#define BOARD_ID_GESI                                   (0x0U)
/* Board detect ID for Ethernet expansion board */
#define BOARD_ID_ENET                                   (0x1U)
/* Board detect ID for dual PMIC SoM */
#define BOARD_ID_SOM                                    (0x2U)
/* Board detect ID for CP Board */
#define BOARD_ID_CP                                     (0x3U)

/* Maximum possible buffer length */
#define BOARD_EEPROM_MAX_BUFF_LENGTH                    (197U)

/* EEPROM board ID information */
#define BOARD_EEPROM_HEADER_FIELD_SIZE                  (7U)
#define BOARD_EEPROM_TYPE_SIZE                          (1U)
#define BOARD_EEPROM_STRUCT_LENGTH_SIZE                 (2U)
#define BOARD_EEPROM_MAGIC_NUMBER                       (0xEE3355AAU)

#define BOARD_BOARD_FIELD_TYPE                          (0x10)
#define BOARD_DDR_FIELD_TYPE                            (0x11)
#define BOARD_MACINFO_FIELD_TYPE                        (0x13)
#define BOARD_ENDLIST                                   (0xFEU)

#define BOARD_EEPROM_HEADER_ADDR                        (0U)

#define BOARD_EEPROM_HEADER_LENGTH                      (4U)
#define BOARD_EEPROM_BOARD_NAME_LENGTH                  (8U)
#define BOARD_EEPROM_VERSION_LENGTH                     (4U)
#define BOARD_EEPROM_SERIAL_NO_LENGTH                   (12U)
#define BOARD_EEPROM_CONFIG_LENGTH                      (32U)

#define BOARD_EEPROM_BOARD_NAME_ADDR                    (BOARD_EEPROM_HEADER_ADDR + BOARD_EEPROM_HEADER_LENGTH)
#define BOARD_EEPROM_VERSION_ADDR                       (BOARD_EEPROM_BOARD_NAME_ADDR + BOARD_EEPROM_BOARD_NAME_LENGTH)
#define BOARD_EEPROM_SERIAL_NO_ADDR                     (BOARD_EEPROM_VERSION_ADDR + BOARD_EEPROM_VERSION_LENGTH)
#define BOARD_EEPROM_CONFIG_ADDR                        (BOARD_EEPROM_SERIAL_NO_ADDR + BOARD_EEPROM_SERIAL_NO_LENGTH)

#define BOARD_MMC_VOLTAGESWITCH_FN                      Board_mmc_voltageSwitchFxn

/* QSPI instance number */
#define BOARD_SPI_NOR_INSTANCE                          (5U)
/* OSPI instance connected to OSPI NOR flash */
#define BOARD_OSPI_NOR_INSTANCE                         (0U)
/* HyperFlash instance number */
#define BOARD_HPF_INSTANCE                              (0U)
/* OSPI instance number */
#define BOARD_OSPI_INSTANCE                             (0U)
/* OSPI Domain number */
#define BOARD_OSPI_DOMAIN                               (0U)
/* Enable NOR flash driver */
#define BOARD_NOR_FLASH_IN

/* CPSW ports on GESI board */
#define BOARD_GESI_CPSW_PORT_MAX                        (1U)
/* CPSW max ports supported */
#define BOARD_CPSW_PORT_MAX                             (4U)

#define BOARD_GESI_CPSW_PHY_ADDR                        (0x0U)

#define BOARD_CPSW5G_EMAC_PORT_MAX                      (4U)

/* I2C address for Board Id EEPROM */
#define BOARD_I2C_EEPROM_ADDR                           (0x50U)

/* MCU_ETH1_RESETn */
#define BOARD_GPIO_IOEXP_MCU_EMAC_RST_PORT_NUM          (0U) /* WKUP_GPIO0_3 */
#define BOARD_GPIO_IOEXP_MCU_EMAC_RST_PIN_NUM           (0x03U)

/* MCU_ETH1_INTn */
#define BOARD_GPIO_MCU_EMAC_INT_PORT_NUM                (0U) /* WKUP_GPIO0_59 */
#define BOARD_GPIO_MCU_EMAC_INT_PIN_NUM                 (0x3BU)

#define BOARD_USER_LED1                                 (6U)
#define BOARD_USER_LED2                                 (7U)
#define BOARD_USER_LED_IOEXP_PORT                       (2U)

#define BOARD_COMMON_EEPROM_I2C_INST                    (0U)

/* Maximum number of supporting board ID */
#define BOARD_ID_MAX_BOARDS                             (4U)

/* GPIO port & pin numbers for  MMC reset */
#define GPIO_MMC_SDCD_PORT_NUM                          (0U)
#define GPIO_MMC_SDCD_PIN_NUM                           (0U)

#define BOARD_GPIO_IOEXP_EMMC_RST_PORT_NUM              (0x00U)
#define BOARD_GPIO_IOEXP_EMMC_RST_PIN_NUM               (0x00U)

/* McSPI instance for master and slave test */
#define BOARD_MCSPI_MASTER_INSTANCE                     (1U)
#define BOARD_MCSPI_SLAVE_INSTANCE                      (1U)

/* LIN uart instance */
#define BOARD_LIN1_UART_INSTANCE                        (5U)
#define BOARD_LIN1_UART_BASE                            (CSL_UART5_BASE)

#define BOARD_LIN2_UART_INSTANCE                        (6U)
#define BOARD_LIN2_UART_BASE                            (CSL_UART6_BASE)

#define BOARD_LIN3_UART_INSTANCE                        (9U)
#define BOARD_LIN3_UART_BASE                            (CSL_UART9_BASE)

#define BOARD_LIN4_UART_INSTANCE                        (3U)
#define BOARD_LIN4_UART_BASE                            (CSL_UART3_BASE)

#define BOARD_LIN5_UART_INSTANCE                        (7U)
#define BOARD_LIN5_UART_BASE                            (CSL_UART7_BASE)

#define BOARD_LIN6_UART_INSTANCE                        (1U)
#define BOARD_LIN6_UART_BASE                            (CSL_UART1_BASE)


/* Default pinmux configuration of UART Tx pin used by ROM/SBL */
#define BOARD_MCU_UART_TX_PINMUX_VAL                    (PIN_MODE(0U) | ((PIN_PULL_DISABLE) & \
                                                         (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE)))
#define BOARD_MCU_UART_TX_PINMUX_ADDR                   (BOARD_WKUP_PMUX_CTRL_ADDR + PIN_WKUP_GPIO0_12)
#define BOARD_MCU_UART_TX_LOCK_KICK_ADDR                (CSL_WKUP_CTRL_MMR0_CFG0_BASE + \
                                                         CSL_WKUP_CTRL_MMR_CFG0_LOCK7_KICK0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif   /* BOARD_CFG_H_ */

