/******************************************************************************
 * Copyright (c) 2018-2022 Texas Instruments Incorporated - http://www.ti.com
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
 *
 * \file   eeprom_test_v2.h
 *
 * \brief  This is the header file for Board ID eeprom diagnostic test.
 *
 */

#ifndef _EEPROM_TEST_V2_H_
#define _EEPROM_TEST_V2_H_

#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/csl/soc.h>
#include <soc.h>

#include "board.h"
#include "board_cfg.h"
#if (defined(j721e_evm) || defined(j7200_evm) || defined(j721s2_evm) || defined(j784s4_evm))
#include "board_utils.h"
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "diag_common_cfg.h"
#if defined(SOC_AM65XX)
#include <ti/board/src/evmKeystone3/include/board_i2c_io_exp.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(am65xx_evm) || defined(am65xx_idk)
#define I2C_INSTANCE                            (0U)
#define MAX_NUM_OF_BOARDS                       (4U)

#define CP_EEPROM_SLAVE_ADDR                    (0x50U)
#define APP_EEPROM_SLAVE_ADDR                   (0x52U)
#define LCD_EEPROM_SLAVE_ADDR                   (0x55U)
#define SERDES_EEPROM_SLAVE_ADDR                (0x54U)

#define NUM_OF_CP_MAC_ADDR                      (2U)
#define NUM_OF_IDK_MAC_ADDR                     (4U)
#define NUM_OF_SERDES_MAC_ADDR                  (1U)

#define STARTING_BOARD_NUM                      (1U)

#elif defined(j7200_evm)
#define MAX_NUM_OF_BOARDS                       (4U)
#define STARTING_BOARD_NUM                      (2U)

#elif defined(tpr12_evm) || defined(awr294x_evm)
#define MAX_NUM_OF_BOARDS						(BOARD_ID_MAX_BOARDS)
#elif defined(am64x_evm)
#define I2C_INSTANCE                            (0U)
#define MAX_NUM_OF_BOARDS                       (2U)

#define EVM_EEPROM_SLAVE_ADDR                   (0x50U)
#define IOLINK_EEPROM_SLAVE_ADDR                (0x52U)
#define STARTING_BOARD_NUM                      (1U)

#elif defined(am64x_svb)
#define STARTING_BOARD_NUM                      (0U)
#define MAX_NUM_OF_BOARDS                       (10U)
#define EEPROM_SLAVE_ADDR                       (BOARD_DUT_EEPROM_SLAVE_ADDR)
#define FMZ_EEPROM_SLAVE_ADDR                   (BOARD_FMZ_EEPROM_SLAVE_ADDR)
#define VPS_EEPROM_SLAVE_ADDR                   (BOARD_VPS_EEPROM_SLAVE_ADDR)
#define DDR4_EEPROM_SLAVE_ADDR                  (BOARD_DDR_DIMM_EEPROM_SLAVE_ADDR)
#define SERDES_EEPROM_SLAVE_ADDR                (BOARD_SERDES_EEPROM_SLAVE_ADDR) // J17 in PCIe x1 SMA SERDES 1-2 SHORT

#elif defined(j721s2_evm)
#define MAX_NUM_OF_BOARDS                       (3U)
#define STARTING_BOARD_NUM                      (2U)

#define SOM_EEPROM_SLAVE_ADDR                   (0x50U)
#define CP_EEPROM_SLAVE_ADDR                    (0x51U)
#define EXP_CONN_EEPROM_SLAVE_ADDR              (0x52U)
#define CSI2_EXP_EEPROM_SLAVE_ADDR              (0x52U)
#define ENET_EXP_EEPROM_SLAVE_ADDR              (0x54U)

#elif defined(j784s4_evm)
#define I2C_INSTANCE                            (0U)
#define MAX_NUM_OF_BOARDS                       (BOARD_ID_MAX_BOARDS)
#define STARTING_BOARD_NUM                      (0U)

#define EVM_EEPROM_SLAVE_ADDR                   (BOARD_EVM_EEPROM_SLAVE_ADDR)
#define EXP_CONN_EEPROM_SLAVE_ADDR              (BOARD_GESI_EEPROM_SLAVE_ADDR)
#define CSI2_EXP_EEPROM_SLAVE_ADDR              (BOARD_CSI2_EEPROM_SLAVE_ADDR)
#define ENET_EXP_EEPROM_SLAVE_ADDR              (BOARD_ENET1_EEPROM_SLAVE_ADDR)

#else  /* j721e_evm */
#define MAX_NUM_OF_BOARDS                       (9U)
#define STARTING_BOARD_NUM                      (2U)

#define SOM_EEPROM_SLAVE_ADDR                   (0x50U)
#define CP_EEPROM_SLAVE_ADDR                    (0x51U)
#define EXP_CONN_EEPROM_SLAVE_ADDR              (0x52U)
#define CSI2_EXP_EEPROM_SLAVE_ADDR              (0x52U)
#define ENET_EXP_EEPROM_SLAVE_ADDR              (0x54U)
#define DISPLAY_ADP_EEPROM_SLAVE_ADDR           (0x00U) //J7ES_TBD: Need to update salve addr
#endif

#define BOARD_NAME_LENGTH                       (7U)

typedef struct boardProgInfo
{
    char              boardName[50];
    uint8_t           slaveAddr;
    bool              isBoardPresent;
}boardProgInfo_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EEPROM_TEST_V2_H_ */
