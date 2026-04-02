/******************************************************************************
 * Copyright (c) 2019 Texas Instruments Incorporated - http://www.ti.com
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

#ifndef _BOARD_PINMUX_H_
#define _BOARD_PINMUX_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file   pinmux.h
 *
 * \brief  Board pinmux header file
 *
 * This file includes the structures to enable the pinmux configurations
 *
 */

#include <ti/csl/tistdtypes.h>
#include <ti/board/src/am64x_svb/AM64x_pinmux.h>

#define BOARD_GPIO_PIN_MUX_CFG         (0x50007U)

#define BOARD_PINMUX_ICSS_RGMII            (0U)  /* Default */
#define BOARD_PINMUX_ICSS_MII              (1U)

#define BOARD_PINMUX_CUSTOM                (0)
#define BOARD_PINMUX_DEFAULT               (1U)   /* Default */

#define BOARD_PINMUX_EXP_NONE              (0)    /* Default */
#define BOARD_PINMUX_EXP_GPMC              (1U)

/* Structure to set the board pinmux configuration */
typedef struct Board_PinmuxConfig_s
{
    /**
     * Pinmux config control
     *  BOARD_PINMUX_CUSTOM(0) - Pinmux is based custom pinmux config set from apps
     *  BOARD_PINMUX_DEFAULT(1) - Pinmux is based on default board pinmux config
     */
    uint8_t muxCfg;

    /**
     * Pinmux config control for ICSS interface
     *  BOARD_PINMUX_ICSS_RGMII(0) - Enables ICSS RGMII mode
     *  BOARD_PINMUX_ICSS_MII(1) - Enables ICSS MII mode
     */
    uint8_t icssMux;

    /**
     * Pinmux config control for application cards
     *  BOARD_PINMUX_EXP_NONE(0) - No expansion board pinmux config
     *  BOARD_PINMUX_EXP_GPMC(1) - Enables pinmux for GPMC
     */
    uint8_t expBoardMux;

} Board_PinmuxConfig_t;

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

void Board_uartTxPinmuxConfig(void);

/**
 *  \brief Sets padconfig register of a pin at given offset
 *
 *  Configures whole padconfig register of the pin at given offset
 *  with the value in 'muxData'.
 *
 *  \param   domain  [IN]  SoC domain for pinmux
 *  \n                      BOARD_SOC_MAIN_DOMAIN - Main domain
 *  \n                      BOARD_SOC_MCU_DOMAIN - MCU domain
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
Board_STATUS Board_pinmuxSetCfg(Board_PinmuxConfig_t *pinmuxCfg);

/**
 *  \brief Gets the board pinmux configuration.
 *
 *  \param   pinmuxCfg [IN]  Pinmux configurations
 *
 *  \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxGetCfg(Board_PinmuxConfig_t *pinmuxCfg);

/**
 * \brief  Board pinmuxing update function
 *
 * Provides the option to configure/update the pinmux.
 * This function can be used to change the pinmux set by
 * Board_init by default.
 *
 * \param   pinmuxData [IN]  Pinmux data structure
 * \param   domain     [IN]  SoC domain for pinmux
 *  \n                        BOARD_SOC_MAIN_DOMAIN - Main domain
 *  \n                        BOARD_SOC_WKUP_DOMAIN - Wakeup domain
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxUpdate (pinmuxBoardCfg_t *pinmuxData,
                                 uint32_t domain);

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* _BOARD_PINMUX_H_ */
