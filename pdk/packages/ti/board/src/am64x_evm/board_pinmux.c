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
 *  \file board_pinmux.c
 *
 *  \brief This file Enables pinmux for the board 
 *
 */

#include <ti/csl/soc.h>				
#include "board_internal.h"
#include "board_pinmux.h"

#ifdef BUILD_M4F
#define MAIN_PADCONFIG_CTRL_BASE    CSL_PADCFG_CTRL0_CFG0_BASE + 0x60000000
#define MCU_PADCONFIG_CTRL_BASE     CSL_MCU_PADCFG_CTRL0_CFG0_BASE + 0x60000000
#else
#define MAIN_PADCONFIG_CTRL_BASE    CSL_PADCFG_CTRL0_CFG0_BASE
#define MCU_PADCONFIG_CTRL_BASE     CSL_MCU_PADCFG_CTRL0_CFG0_BASE
#endif
#define CTRL_MMR0_PARTITION_SIZE    0x4000
#define MAIN_CTRL_PINCFG_BASE       (MAIN_PADCONFIG_CTRL_BASE + (1 * CTRL_MMR0_PARTITION_SIZE))

/* Default pinmux configuration of UART Tx pin used by ROM/SBL */
#define BOARD_UART_TX_PINMUX_VAL            (PIN_MODE(0) | ((PIN_PULL_DISABLE) & \
                                                 (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE)))
#define BOARD_SYSFW_UART_TX_PINMUX_ADDR           (MAIN_PADCONFIG_CTRL_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG141)
#define BOARD_SBL_UART_TX_PINMUX_ADDR           (MAIN_PADCONFIG_CTRL_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG145)
#define BOARD_UART_TX_LOCK_KICK_ADDR        (MAIN_PADCONFIG_CTRL_BASE + \
                                                 CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK1_KICK0)

static Board_PinmuxConfig_t gBoardPinmuxCfg = {BOARD_PINMUX_DEFAULT,
                                               BOARD_PINMUX_ICSS_RGMII,
                                               BOARD_PINMUX_EXP_NONE};

/**
 *  \brief  Gets base address of padconfig registers
 *
 *  \param   domain [IN]  SoC domain for pinmux
 *  \n                     BOARD_SOC_DOMAIN_MAIN - Main domain
 *  \n                     BOARD_SOC_DOMAIN_MCU  - MCU domain
 *
 *  \return   Valid address in case success or 0 in case of failure
 */
static uint32_t Board_pinmuxGetBaseAddr(uint8_t domain)
{
    uint32_t baseAddr;

    switch(domain)
    {
        case BOARD_SOC_DOMAIN_MAIN:
            baseAddr = BOARD_MAIN_PMUX_CTRL;
        break;
        case BOARD_SOC_DOMAIN_MCU:
            baseAddr = BOARD_WKUP_PMUX_CTRL;
        break;
        default:
            baseAddr = 0;
        break;
    }

    return baseAddr;
}

/**
 *  \brief Sets the board pinmux configuration.
 *
 *  This API allows to change the default pinmux configurations
 *  in the board library.
 *
 *  \n Usage:
 *  \n - Call Board_pinmuxGetCfg to get default pinmux config
 *  \n - Call Board_pinmuxSetCfg to change pinmux config
 *  \n - Call Board_init with pinmux flag to apply the updated pinmux config
 *
 *  \param   pinmuxCfg [IN]  Pinmux configurations
 *
 *  \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxSetCfg(Board_PinmuxConfig_t *pinmuxCfg)
{
    gBoardPinmuxCfg = *pinmuxCfg;

    return BOARD_SOK;
}

/**
 *  \brief Gets the board pinmux configuration.
 *
 *  \param   pinmuxCfg [IN]  Pinmux configurations
 *
 *  \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxGetCfg(Board_PinmuxConfig_t *pinmuxCfg)
{
    *pinmuxCfg = gBoardPinmuxCfg;

    return BOARD_SOK;
}

/**
 * \brief  Board pinmuxing update function
 *
 * Provides the option to configure/update the pinmux.
 * This function can be used to change the pinmux set by
 * Board_init by default.
 *
 * \param   pinmuxData [IN]  Pinmux data structure
 * \param   domain     [IN]  SoC domain for pinmux
 *  \n                        BOARD_SOC_DOMAIN_MAIN - Main domain
 *  \n                        BOARD_SOC_DOMAIN_MCU - MCU domain
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxUpdate (pinmuxBoardCfg_t *pinmuxData,
                                 uint32_t domain)
{
    pinmuxModuleCfg_t *pModuleData = NULL;
    pinmuxPerCfg_t *pInstanceData = NULL;
    int32_t i, j, k;
    uint32_t baseAddr;
    Board_STATUS status = BOARD_SOK;

    Board_unlockMMR();

    baseAddr = Board_pinmuxGetBaseAddr(domain);
    if(baseAddr != 0)
    {
        for(i = 0; PINMUX_END != pinmuxData[i].moduleId; i++)
        {
            pModuleData = pinmuxData[i].modulePinCfg;
            for(j = 0; (PINMUX_END != pModuleData[j].modInstNum); j++)
            {
                if(pModuleData[j].doPinConfig == TRUE)
                {
                    pInstanceData = pModuleData[j].instPins;
                    for(k = 0; (PINMUX_END != pInstanceData[k].pinOffset); k++)
                    {
                        HW_WR_REG32((baseAddr + pInstanceData[k].pinOffset),
                                    (pInstanceData[k].pinSettings));
                    }
                }
            }
        }
    }
    else
    {
        status = BOARD_INVALID_PARAM;
    }

    Board_lockMMR();

    return status;
}

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
Board_STATUS Board_pinmuxConfig (void)
{
    Board_STATUS status = BOARD_SOK;

    Board_pinmuxUpdate(gAM64x_MainPinmuxData,
                       BOARD_SOC_DOMAIN_MAIN);
    Board_pinmuxUpdate(gAM64x_WkupPinmuxData,
                       BOARD_SOC_DOMAIN_MCU);

    /* Note: EVM Specific config.
     * Code below be removed for custom boards */
    if(gBoardPinmuxCfg.muxCfg == BOARD_PINMUX_CUSTOM)
    {
        if(gBoardPinmuxCfg.icssMux == BOARD_PINMUX_ICSS_MII)
        {
            Board_pinmuxUpdate(gAM64x_MainPinmuxDataIcssMII,
                               BOARD_SOC_DOMAIN_MAIN);
        }

        if(gBoardPinmuxCfg.expBoardMux == BOARD_PINMUX_EXP_GPMC)
        {
            Board_pinmuxUpdate(gAM64x_MainPinmuxDataGPMC,
                               BOARD_SOC_DOMAIN_MAIN);
        }
    }

    return status;
}

/**
 * \brief  Function to configure UART Tx pinmux
 *
 */
void Board_uartTxPinmuxConfig(void)
{
    /* Board_unlockMMR */
    HW_WR_REG32(BOARD_UART_TX_LOCK_KICK_ADDR, KICK0_UNLOCK_VAL);
    HW_WR_REG32(BOARD_UART_TX_LOCK_KICK_ADDR + 4U, KICK1_UNLOCK_VAL);

    /* Configure pinmux for UART Tx pin */
    HW_WR_REG32(BOARD_SBL_UART_TX_PINMUX_ADDR, BOARD_UART_TX_PINMUX_VAL);     

    /* Configure pinmux for SYSFW Tx pin */
    HW_WR_REG32(BOARD_SYSFW_UART_TX_PINMUX_ADDR, BOARD_UART_TX_PINMUX_VAL);
    
    /* Board_lockMMR */
    HW_WR_REG32(BOARD_UART_TX_LOCK_KICK_ADDR + 4U, 0);
    HW_WR_REG32(BOARD_UART_TX_LOCK_KICK_ADDR, 0);
}

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
                                uint32_t muxData)
{
    uint32_t baseAddr;
    Board_STATUS status = BOARD_SOK;

    Board_unlockMMR();

    baseAddr = Board_pinmuxGetBaseAddr(domain);
    if(baseAddr != 0)
    {
        HW_WR_REG32((baseAddr + offset), muxData);
    }
    else
    {
        status = BOARD_INVALID_PARAM;
    }

    return status;
}
