/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "hsr_prp_soc.h"
#include "ti_board_open_close.h"
#include <drivers/hw_include/hw_types.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* EEPROM data offset in I2C EEPROM Flash */
#define I2C_EEPROM_DATA_OFFSET          (0x8000)
#define I2C_EEPROM_MAC_DATA_OFFSET      (0x3D)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

extern PRUICSS_Handle prusshandle;
extern TimeSync_ParamsHandle_t timeSyncHandle;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void hsrprp_configureInterrupts(ICSS_EMAC_Handle emachandle)
{
    /*Configure Time Sync interrupts*/
    PRUICSS_HwAttrs const *pruicssHwAttrs = PRUICSS_getAttrs(CONFIG_PRU_ICSS1);

    if(pruicssHwAttrs->instance == 0)
        timeSyncHandle->timeSyncConfig.txIntNum = CSLR_R5FSS0_CORE0_INTR_PRU_ICSSG0_PR1_HOST_INTR_PEND_0 + 3;
    else if(pruicssHwAttrs->instance == 1)
        timeSyncHandle->timeSyncConfig.txIntNum = CSLR_R5FSS0_CORE0_INTR_PRU_ICSSG1_PR1_HOST_INTR_PEND_0 + 3;
}

void hsrprp_socgetMACAddress(uint8_t *lclMac)
{
    EEPROM_read(gEepromHandle[CONFIG_EEPROM0],  I2C_EEPROM_MAC_DATA_OFFSET, lclMac, 6U);
}

void hsrprp_configure_rat()
{
    uintptr_t icssgBaseAddr = (((PRUICSS_HwAttrs *)((prusshandle)->hwAttrs))->baseAddr);

    /*ICSSG0 and ICSSG1 RAT0 Entry 0*/

    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_RAT_REGS_0_BASE + 0x24, 0xE0000000); //rat0 base0
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_RAT_REGS_0_BASE + 0x28, 0x70000000); //rat0 trans_low0
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_RAT_REGS_0_BASE + 0x2C, 0x00000000); //rat0 trans_low0
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_RAT_REGS_0_BASE + 0x20, (1u << 31) | (22)); //rat0 ctrl0

    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_RAT_REGS_1_BASE + 0x24, 0xE0000000); //rat0 base0
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_RAT_REGS_1_BASE + 0x28, 0x70000000); //rat0 trans_low0
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_RAT_REGS_1_BASE + 0x2C, 0x00000000); //rat0 trans_low0
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_RAT_REGS_1_BASE + 0x20, (1u << 31) | (22)); //rat0 ctrl0
}

void hsrprp_mii_init()
{
    ETHPHY_DP83869_LedSourceConfig ledConfig;

    /* PHY pin LED_1 as 1G link established */
    ledConfig.ledNum = ETHPHY_DP83869_LED1;
    ledConfig.mode = ETHPHY_DP83869_LED_MODE_1000BT_LINK_UP;
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY1], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));

    /* PHY pin LED_2 as Rx/Tx Activity */
    ledConfig.ledNum = ETHPHY_DP83869_LED2;
    ledConfig.mode = ETHPHY_DP83869_LED_MODE_RX_TX_ACTIVITY;
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY1], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));

    /* PHY pin LED_3 as 100M link established */
    ledConfig.ledNum = ETHPHY_DP83869_LED_GPIO;
    ledConfig.mode = ETHPHY_DP83869_LED_MODE_10_OR_100BT_LINK_UP;
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY1], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));

   /* Select MII mode */
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_ENABLE_MII, NULL, 0);
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY1], ETHPHY_CMD_ENABLE_MII, NULL, 0);

    /* Disable 1G advertisement */
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_DISABLE_1000M_ADVERTISEMENT, NULL, 0);
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY1], ETHPHY_CMD_DISABLE_1000M_ADVERTISEMENT, NULL, 0);

    /* Soft-reset PHY */
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_SOFT_RESTART, NULL, 0);
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY1], ETHPHY_CMD_SOFT_RESTART, NULL, 0);

    hsrprp_configure_rat();
}

void hsrprp_rgmii_init()
{
    ETHPHY_DP83869_LedSourceConfig ledConfig;

    /* PHY pin LED_1 as 1G link established */
    ledConfig.ledNum = ETHPHY_DP83869_LED1;
    ledConfig.mode = ETHPHY_DP83869_LED_MODE_1000BT_LINK_UP;
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY1], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));

    /* PHY pin LED_2 as Rx/Tx Activity */
    ledConfig.ledNum = ETHPHY_DP83869_LED2;
    ledConfig.mode = ETHPHY_DP83869_LED_MODE_RX_TX_ACTIVITY;
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY1], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));

    /* PHY pin LED_3 as 100M link established */
    ledConfig.ledNum = ETHPHY_DP83869_LED_GPIO;
    ledConfig.mode = ETHPHY_DP83869_LED_MODE_10_OR_100BT_LINK_UP;
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY1], ETHPHY_CMD_CONFIGURE_LED_SOURCE, (void *)&ledConfig, sizeof(ledConfig));

    /* Disable 1G advertisement */
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_DISABLE_1000M_ADVERTISEMENT, NULL, 0);
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY1], ETHPHY_CMD_DISABLE_1000M_ADVERTISEMENT, NULL, 0);

    /* Soft-reset PHY */
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY0], ETHPHY_CMD_SOFT_RESTART, NULL, 0);
    ETHPHY_command(gEthPhyHandle[CONFIG_ETHPHY1], ETHPHY_CMD_SOFT_RESTART, NULL, 0);

    hsrprp_configure_rat();
}


