/******************************************************************************
 * Copyright (c) 2023 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file   board_info_ddr.h
 *
 *  \brief  Board utility functions header file
 *
 */

#ifndef _BOARD_INFO_DDR_H_
#define _BOARD_INFO_DDR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Include Files                                                             *
 *****************************************************************************/
#include <ti/csl/csl_types.h>
#include <ti/csl/cslr_device.h>

#include <ti/board/board.h>
#include <ti/csl/tistdtypes.h>
#include <stdio.h>
#include <stdbool.h>

/* Board ID DDR dump offset for CP Board */
#define BOARD_ID_DUMP_CP              (0x0U)
/* Board ID DDR dump offset for GESI */
#define BOARD_ID_DUMP_GESI            (0x1U)

/* Maximum number of supporting board ID DDR dump */
#define BOARD_ID_DUMP_MAX_BOARDS      (0x2U)

#define BOARD_ID_DUMP_MAX_SIZE        (1024U)

#define BOARD_ID_TYPE_FULL                    (0x0U)
#define BOARD_ID_TYPE_BASIC                   (0x1U)
#define BOARD_ID_TYPE_MEM                     (0x2U)
#define BOARD_ID_TYPE_ETH                     (0x3U)

#define BOARD_ENABLE_ID_MEMORY_DUMP           (BTRUE)
#define BOARD_ID_MEMORY_DUMP_ERR_FALLBACK     (BTRUE)

/**
 *  @brief This structure contains board specific information
 *
 *  This is the board ID info structure for the boards which
 *  does not contain DDR memory or Ethernet MAC ID information.
 */
typedef struct Board_IDInfoBasic_s
{
    /* Header info of the board */
    Board_headerInfo headerInfo;
    /* Board id Info of the board */
    Board_boardInfo boardInfo;
    /* End of the structure */
    uint8_t endList;
}  __attribute__((packed)) Board_IDInfoBasic_t;

/**
 *  @brief This structure contains board specific information
 *
 *  This is the board ID info structure for the boards which
 *  contain DDR memory information.
 */
typedef struct Board_IDInfoMem_s
{
    /* Header info of the board */
    Board_headerInfo headerInfo;
    /* Board id Info of the board */
    Board_boardInfo boardInfo;
    /* DDR info of the board */
    Board_ddrInfo ddrInfo;
    /* End of the structure */
    uint8_t endList;
}  __attribute__((packed)) Board_IDInfoMem_t;

/**
 *  @brief This structure contains board specific information
 *
 *  This is the board ID info structure for the boards which
 *  contain Ethernet MAC ID information.
 */
typedef struct Board_IDInfoEth_s
{
    /* Header info of the board */
    Board_headerInfo headerInfo;
    /* Board id Info of the board */
    Board_boardInfo boardInfo;
    /* Mac Info of the board */
    Board_macInfo macInfo;
    /* End of the structure */
    uint8_t endList;
}  __attribute__((packed)) Board_IDInfoEth_t;

/**
 *  @brief This structure contains board ID DDR dump
 *
 */
typedef struct Board_BoardIDShadow_s
{
    /* Board ID dump for CP board */
    Board_IDInfoBasic_t *cpBoardID;
    /* Board ID dump for GESI expansion board  */
    Board_IDInfoEth_t *gesiBoardID;
    /* Board ID dump for enet expansion board  */
    Board_IDInfoEth_t *enetBoardID;
}  Board_BoardIDShadow_t;

/**
 * \brief Function to read board ID data from memory dump
 *
 * \param   info     [IN]  Board info structure
 * \param   boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_GESI - GESI Board
 * \n                      BOARD_ID_ENET - Quad ENET expansion
 * \n                      BOARD_ID_CP - CP Board
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_getBoardIdData(Board_IDInfo_v2 *info, uint32_t boardID);

/**
 * \brief Function to read board ID data from EEPROM to memory
 *
 * This function is supposed to be called in the environment where
 * board library need to read the board info from EEPROM and populate
 * it in the DDR memory.
 *
 * \param   boardIDWrAddr [IN] Pointer to the address where board ID
 *                             data to be populated
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_initBoardIdData(uint8_t *boardIDWrAddr);

/**
 * \brief Function to set the board ID data address in DDR memory
 *
 * This function is supposed to be called in the environment where
 * board info is read from EEPROM and populated the DDR memory outside
 * board library. Pointer passed as parameter should hold the board
 * info data in DDR which will be used by the board library.
 *
 * \param   boardIDRdAddr [IN] Pointer to the address where board ID
 *                             data is populated
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_setBoardIdDataAddr(uint8_t *boardIDRdAddr);

/**
 * \brief Function to check the board info DDR dump status
 *
 * \return   BTRUE if board ID dump is DDR is valid, else BFALSE
 *
 */
bool Board_isBoardDDRIdDataValid(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BOARD_INFO_DDR_H_ */
