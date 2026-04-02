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
 *  \file board_info_ddr.c
 *
 *  \brief This file contains the functions to read board info data from DDR
 *
 */

#include "board_utils.h"
#include "board_internal.h"
#include "board_info_ddr.h"
#include "board_cfg.h"
#include <stdio.h>
#include <string.h>

extern Board_I2cInitCfg_t gBoardI2cInitCfg;
extern Board_DetectCfg_t  gBoardDetCfg[BOARD_ID_MAX_BOARDS];
uint8_t *gBoardIDDump = NULL;
static Board_BoardIDShadow_t gBoardIDShadow = {NULL, NULL, NULL};

/**
 *  @brief      Get board ID data.
 *
 *  Reads the board ID data into an array provided. Programs calling this
 *  function should make sure enough memory is allocated for reading
 *  the board ID content.
 *
 *  This function requires the information of I2C instance and domain
 *  to which board ID EEPROM is connected. This need to be set using
 *  Board_setI2cInitConfig() before calling this function.
 *
 *  @param[out] Board_STATUS
 *    Returns status on API call
 *  @param[out] info
 *    This buffer will have board ID data on return
 *  @param[in] slaveAddress
 *    I2C slave address of EEPROM to be read
 *
 */
static Board_STATUS Board_getIDData(uint8_t *info, uint8_t slaveAddress)
{
    Board_STATUS ret = BOARD_SOK;
    I2C_Transaction i2cTransaction;
    I2C_Handle handle = NULL;
    Board_headerInfo headerInfo;
    uint16_t offsetAddress = BOARD_EEPROM_HEADER_ADDR;
    char txBuf[2] = {0x00, 0x00};
    int16_t status;

    I2C_transactionInit(&i2cTransaction);

    handle = Board_getI2CHandle(gBoardI2cInitCfg.socDomain,
                                gBoardI2cInitCfg.i2cInst);
    if(handle == NULL)
    {
        ret = BOARD_I2C_OPEN_FAIL;
    }

    i2cTransaction.slaveAddress = slaveAddress;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = 2;

    /* Get header info */
    txBuf[0] = (char)(((uint32_t) 0xFF00 & offsetAddress)>>8);
    txBuf[1] = (char)((uint32_t) 0xFF & offsetAddress);
    i2cTransaction.readBuf   = &headerInfo;
    i2cTransaction.readCount = BOARD_EEPROM_HEADER_FIELD_SIZE;

    status = I2C_transfer(handle, &i2cTransaction);
    if (I2C_STS_ERR == status)
    {
        ret = BOARD_I2C_TRANSFER_FAIL;
        Board_i2cDeInit();
        return ret;
    }

    /* Checking whether the board contents are flashed or not */
    if (headerInfo.magicNumber == BOARD_EEPROM_MAGIC_NUMBER)
    {
        txBuf[0] = (char)(((uint32_t) 0xFF00 & offsetAddress) >> 8);
        txBuf[1] = (char)((uint32_t) 0xFF & offsetAddress);
        i2cTransaction.readBuf   = info;
        i2cTransaction.readCount = headerInfo.payloadSize +
                                   BOARD_EEPROM_HEADER_FIELD_SIZE;
        status = I2C_transfer(handle, &i2cTransaction);
        if (I2C_STS_ERR == status)
        {
            ret = BOARD_I2C_TRANSFER_FAIL;
            Board_i2cDeInit();
            return ret;
        }
    }
    else
    {
        ret = BOARD_INVALID_PARAM;
        Board_i2cDeInit();
        return ret;
    }

    Board_i2cDeInit();

    return ret;
}

/**
 * \brief Function to populate board ID data into info structure
 *
 * \param   info     [IN]  Board info structure
 * \param   boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_GESI - GESI Board
 * \n                      BOARD_ID_ENET - Quad ENET expansion
 * \n                      BOARD_ID_EVM - EVM Board
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_copyBoardDataMem(void *srcInfo, Board_IDInfo_v2 *dstInfo, uint32_t memType)
{
    Board_IDInfoBasic_t *basicInfo;
    Board_IDInfoEth_t *ethInfo;
    Board_STATUS status = BOARD_SOK;

    switch(memType)
    {
        case BOARD_ID_TYPE_BASIC:
            basicInfo = (Board_IDInfoBasic_t *)srcInfo;
            if(basicInfo->headerInfo.magicNumber == BOARD_EEPROM_MAGIC_NUMBER)
            {
                memcpy((void *)&dstInfo->headerInfo, (const void *)&basicInfo->headerInfo, sizeof(Board_headerInfo));
                memcpy((void *)&dstInfo->boardInfo, (const void *)&basicInfo->boardInfo, sizeof(Board_boardInfo));
                dstInfo->endList = basicInfo->endList;
            }
            else
            {
                status = BOARD_INVALID_PARAM;
            }
            break;

        case BOARD_ID_TYPE_ETH:
            ethInfo = (Board_IDInfoEth_t *)srcInfo;
            if(ethInfo->headerInfo.magicNumber == BOARD_EEPROM_MAGIC_NUMBER)
            {
                memcpy((void *)&dstInfo->headerInfo, (const void *)&ethInfo->headerInfo, sizeof(Board_headerInfo));
                memcpy((void *)&dstInfo->boardInfo, (const void *)&ethInfo->boardInfo, sizeof(Board_boardInfo));
                memcpy((void *)&dstInfo->macInfo, (const void *)&ethInfo->macInfo, sizeof(Board_macInfo));
                dstInfo->endList = ethInfo->endList;
            }
            else
            {
                status = BOARD_INVALID_PARAM;
            }
            break;

        default:
            status = BOARD_INVALID_PARAM;
            break;
    }

    return status;
}

/**
 * \brief Function to read board ID data from memory dump
 *
 * \param   info     [IN]  Board info structure
 * \param   boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_GESI - GESI Board
 * \n                      BOARD_ID_ENET - Quad ENET expansion
 * \n                      BOARD_ID_EVM - EVM Board
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_getBoardIdData(Board_IDInfo_v2 *info, uint32_t boardID)
{
    Board_STATUS status = BOARD_FAIL;

    switch(boardID)
    {
        case BOARD_ID_EVM:
            if (gBoardIDShadow.evmBoardID != NULL)
            {
                status = Board_copyBoardDataMem(gBoardIDShadow.evmBoardID,
                                                info,
                                                BOARD_ID_TYPE_BASIC);
            }
            break;
        case BOARD_ID_ENET:
            if (gBoardIDShadow.enetBoardID != NULL)
            {
                status = Board_copyBoardDataMem(gBoardIDShadow.enetBoardID,
                                                info,
                                                BOARD_ID_TYPE_ETH);
            }
            break;

        case BOARD_ID_GESI:
            if (gBoardIDShadow.gesiBoardID != NULL)
            {
                status = Board_copyBoardDataMem(gBoardIDShadow.gesiBoardID,
                                                info,
                                                BOARD_ID_TYPE_ETH);
            }
            break;

        default:
            status = BOARD_INVALID_PARAM;
            break;
    }

    return status;
}

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
Board_STATUS Board_initBoardIdData(uint8_t *boardIDWrAddr)
{
    Board_I2cInitCfg_t i2cCfg;
    Board_STATUS status = BOARD_SOK;
    uint32_t offset   = 0;

    i2cCfg.i2cInst    = BOARD_COMMON_EEPROM_I2C_INST;
    i2cCfg.socDomain  = BOARD_SOC_DOMAIN_WKUP;
    i2cCfg.enableIntr = BFALSE;
    Board_setI2cInitConfig(&i2cCfg);

    if(boardIDWrAddr != NULL)
    {
        gBoardIDDump = boardIDWrAddr;

        gBoardIDShadow.evmBoardID = (Board_IDInfoBasic_t *)&gBoardIDDump[offset];
        /* Check if Board ID data is already populated in memory */
        if(gBoardIDShadow.evmBoardID->headerInfo.magicNumber == BOARD_EEPROM_MAGIC_NUMBER)
        {
            offset   = gBoardIDShadow.evmBoardID->headerInfo.payloadSize + BOARD_EEPROM_HEADER_FIELD_SIZE;

            gBoardIDShadow.enetBoardID = (Board_IDInfoEth_t *)&gBoardIDDump[offset];
            if(gBoardIDShadow.enetBoardID->headerInfo.magicNumber != BOARD_EEPROM_MAGIC_NUMBER)
            {
                /* Read ENET Board EEPROM data */
                status = Board_getIDData(&gBoardIDDump[offset],
                                         gBoardDetCfg[BOARD_ID_ENET].slaveAddr);
                gBoardIDShadow.enetBoardID = (Board_IDInfoEth_t *)&gBoardIDDump[offset];
                if(gBoardIDShadow.enetBoardID->headerInfo.magicNumber != BOARD_EEPROM_MAGIC_NUMBER)
                {
                    gBoardIDShadow.enetBoardID = NULL;
                }
            }
        }
        else
        {
            /* Read EVM Board EEPROM data */
            status = Board_getIDData(&gBoardIDDump[offset],
                                     gBoardDetCfg[BOARD_ID_EVM].slaveAddr);
            gBoardIDShadow.evmBoardID = (Board_IDInfoBasic_t *)&gBoardIDDump[offset];
            if(gBoardIDShadow.evmBoardID->headerInfo.magicNumber == BOARD_EEPROM_MAGIC_NUMBER)
            {
                offset   = gBoardIDShadow.evmBoardID->headerInfo.payloadSize + BOARD_EEPROM_HEADER_FIELD_SIZE;

                /* Read ENET Board EEPROM data */
                status = Board_getIDData(&gBoardIDDump[offset],
                                         gBoardDetCfg[BOARD_ID_ENET].slaveAddr);
                gBoardIDShadow.enetBoardID = (Board_IDInfoEth_t *)&gBoardIDDump[offset];
                if(gBoardIDShadow.enetBoardID->headerInfo.magicNumber != BOARD_EEPROM_MAGIC_NUMBER)
                {
                    gBoardIDShadow.enetBoardID = NULL;
                }

            }
            else
            {
                gBoardIDShadow.evmBoardID = NULL;
                status = BOARD_FAIL;
            }
        }
    }
    else
    {
        status = BOARD_INVALID_PARAM;
    }

    return status;
}

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
Board_STATUS Board_setBoardIdDataAddr(uint8_t *boardIDRdAddr)
{
    Board_STATUS status = BOARD_SOK;
    uint32_t evmIDSize = 0;
    uint32_t offset   = 0;

    if(boardIDRdAddr != NULL)
    {
        gBoardIDDump = boardIDRdAddr;

        gBoardIDShadow.evmBoardID = (Board_IDInfoBasic_t *)&gBoardIDDump[offset];
        if(gBoardIDShadow.evmBoardID->headerInfo.magicNumber == BOARD_EEPROM_MAGIC_NUMBER)
        {
            offset   = gBoardIDShadow.evmBoardID->headerInfo.payloadSize + BOARD_EEPROM_HEADER_FIELD_SIZE;
            evmIDSize = offset;

            gBoardIDShadow.gesiBoardID = (Board_IDInfoEth_t *)&gBoardIDDump[offset];
            if(gBoardIDShadow.gesiBoardID->headerInfo.magicNumber == BOARD_EEPROM_MAGIC_NUMBER)
            {
                if(strncmp(gBoardIDShadow.gesiBoardID->boardInfo.boardName,
                           gBoardDetCfg[BOARD_ID_GESI].bName,
                           strlen(gBoardDetCfg[BOARD_ID_GESI].bName)))
                {
                    /* GESI board data is not available on board not connected */
                    gBoardIDShadow.enetBoardID = (Board_IDInfoEth_t *)&gBoardIDDump[offset];
                    gBoardIDShadow.gesiBoardID = NULL;
                }
                else
                {
                    /* GESI board data is available. */
                    offset = evmIDSize + gBoardIDShadow.gesiBoardID->headerInfo.payloadSize + BOARD_EEPROM_HEADER_FIELD_SIZE;
                    gBoardIDShadow.enetBoardID = (Board_IDInfoEth_t *)&gBoardIDDump[offset];
                }
            }
            else
            {
                gBoardIDShadow.gesiBoardID = NULL;
            }
        }
        else
        {
            /* Board ID data should have EVM board info by default */
            gBoardIDShadow.evmBoardID = NULL;
            status = BOARD_FAIL;
        }
    }
    else
    {
        status = BOARD_INVALID_PARAM;
    }

    return status;
}

/**
 * \brief Function to check the board info DDR dump status
 *
 * \return   BTRUE if board ID dump is DDR is valid, else BFALSE
 *
 */
bool Board_isBoardDDRIdDataValid(void)
{
    bool status = BFALSE; 

    if(gBoardIDDump != NULL)
    {
        status = BTRUE;
    }

    return status;
}

