/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   gpmc_test.c
 *
 *  \brief  gpmc diagnostic test main file
 *
 *  Targeted Functionality: This file implements gpmc diagnostic test.
 *
 *  Operation: Verification of gpmc by accessing the device connected to it.
 *
 *  Supported SoCs : AM64x AM640x
 *
 *  Supported Platforms: am64x_svb am640x_svb
 *
 */

#include "gpmc_test.h"

/**
 * \brief   This function reads data from connected GPMC memory device.
 *
 * \param   handle     [IN]    GPMC handle
 * \param   addr       [IN]    Offset to read the data
 * \param   len        [IN]    Number of bytes to read
 * \param   buf        [IN]    Buffer to store the data
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_gpmcAsramRead(GPMC_Handle handle, uint32_t addr,
                                      uint32_t len, uint8_t *buf)
{
    GPMC_Transaction  transaction;
    bool              ret;

    transaction.offset = addr;
    transaction.txBuf  = NULL;
    transaction.rxBuf  = (void *)buf;
    transaction.count  = len;

    ret = GPMC_transfer(handle, &transaction);
    if (ret == true)
    {
        return 0;
    }
	else
    {
        return -1;
    }
}

/**
 * \brief   This function writes data to connected GPMC memory device.
 *
 * \param   handle     [IN]    GPMC handle
 * \param   addr       [IN]    Offset to write the data
 * \param   len        [IN]    Number of bytes to write
 * \param   buf        [IN]    Buffer to store the data
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_gpmcAsramWrite(GPMC_Handle handle, uint32_t addr,
                                       uint32_t len, uint8_t *buf)
{
    GPMC_Transaction  transaction;
    bool              ret;

    transaction.offset = addr;
    transaction.txBuf  = (void *)buf;
    transaction.rxBuf  = NULL;
    transaction.count  = len;

    ret = GPMC_transfer(handle, &transaction);
    if (ret == true)
    {
        return 0;
    }
	else
    {
        return -1;
    }
}

/**
 * \brief   This function writes data, reads back the written data and compares.
 *
 * \param   offset      [IN]    Offset Address to write
 * \param   numOfBytes  [IN]    Number of bytes to write.
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_gpmcNorReadWriteTest(Board_flashHandle handle,
                                             uint32_t offset,
                                             uint32_t numOfBytes)
{
    Board_flash_STATUS retVal;
    uint32_t blockNum, pageNum;
    uint32_t failIndex;
    uint8_t txBuf[BOARD_DIAG_GPMC_NOR_TEST_BYTES];
    uint8_t rxBuf[BOARD_DIAG_GPMC_NOR_TEST_BYTES];

    retVal = Board_flashOffsetToBlkPage(handle, offset, &blockNum, &pageNum);
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashOffsetToBlkPage failed\n");
        return -1;
    }

    /* Erase block, to which data has to be written */
    retVal = Board_flashEraseBlk(handle, blockNum);
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashEraseBlk failed\n");
        return -1;
    }

    BoardDiag_genPattern(&txBuf[0], numOfBytes,
                         BOARD_DIAG_TEST_PATTERN_RANDOM);

    /* Write buffer to NOR flash */
    retVal = Board_flashWrite(handle, offset,
                              &txBuf[0], numOfBytes, NULL);
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashWrite failed\n");
        return -1;
    }

    /* Read the NOR flash memory */
    retVal = Board_flashRead(handle, offset,
                             &rxBuf[0], numOfBytes, NULL);
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashRead failed\n");
        return -1;
    }

    if (BoardDiag_memCompare(&txBuf[0], &rxBuf[0], numOfBytes, &failIndex)
                                                                    == false)
    {
        UART_printf("\nData mismatch at offset = 0x%x\n", failIndex);
        return -1;
    }
    return 0;
}

/**
 * \brief   This function writes data, reads back the written data and compares.
 *
 * \param   offset      [IN]    Offset Address to write
 * \param   numOfBytes  [IN]    Number of bytes to write.
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_gpmcNandReadWriteTest(Board_flashHandle handle,
                                              uint32_t offset,
                                              uint32_t numOfBytes)
{
    Board_flash_STATUS retVal;
    uint32_t blockNum, pageNum;
    uint32_t failIndex;
    uint8_t txBuf[BOARD_DIAG_GPMC_NAND_TEST_BYTES];
    uint8_t rxBuf[BOARD_DIAG_GPMC_NAND_TEST_BYTES];

    retVal = Board_flashOffsetToBlkPage(handle, offset, &blockNum, &pageNum);
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashOffsetToBlkPage failed\n");
        return -1;
    }

    /* Erase block, to which data has to be written */
    retVal = Board_flashEraseBlk(handle, blockNum);
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashEraseBlk failed\n");
        return -1;
    }

    BoardDiag_genPattern(&txBuf[0], numOfBytes,
                         BOARD_DIAG_TEST_PATTERN_RANDOM);

    /* Write buffer to NAND flash */
    retVal = Board_flashWrite(handle, offset,
                              &txBuf[0], numOfBytes, NULL);
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashWrite failed\n");
        return -1;
    }

    /* Read the NAND flash memory */
    retVal = Board_flashRead(handle, offset,
                             &rxBuf[0], numOfBytes, NULL);
    if (retVal != BOARD_FLASH_EOK)
    {
        UART_printf("\nBoard_flashRead failed\n");
        return -1;
    }

    if (BoardDiag_memCompare(&txBuf[0], &rxBuf[0], numOfBytes, &failIndex)
                                                                    == false)
    {
        UART_printf("\nData mismatch at offset = 0x%x\n", failIndex);
        return -1;
    }
    return 0;
}

/**
 * \brief   This function runs GPMC ASRAM flash boundary verification test.
 *
 * \param   deviceId     [IN]    Board flash device ID
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_gpmcRunAsramTest(uint32_t deviceId)
{
    GPMC_Params gpmcParams;     /* GPMC params structure */
    GPMC_Handle gpmcHandle;     /* GPMC handle */
    uint32_t failIndex;
    uint8_t txBuf[BOARD_DIAG_GPMC_ASRAM_TEST_BYTES];
    uint8_t rxBuf[BOARD_DIAG_GPMC_ASRAM_TEST_BYTES];
    int8_t ret;
    GPMC_v1_HwAttrs gpmc_cfg;

    /* Get the default GPMC Init configurations */
    GPMC_socGetInitCfg(BOARD_GPMC_INSTANCE, &gpmc_cfg);

    /* Modify the default GPMC configurations if necessary */
    gpmc_cfg.devType = GPMC_DEVICETYPE_SRAMLIKE;
    gpmc_cfg.addrDataMux = GPMC_MUXADDDATA_NOMUX;
    gpmc_cfg.readType = GPMC_READTYPE_ASYNC;
    gpmc_cfg.writeType = GPMC_WRITETYPE_ASYNC;

    /* GPMC Timing parameters */
    gpmc_cfg.csExDelay = GPMC_CS_EXTRA_NODELAY;
    gpmc_cfg.timingParams.csOnTime = 1;
    gpmc_cfg.timingParams.csRdOffTime = 0;
    gpmc_cfg.timingParams.csWrOffTime = 1;
    gpmc_cfg.timingParams.weOnTtime = 5;
    gpmc_cfg.timingParams.weOffTime = 0;
    gpmc_cfg.timingParams.oeOnTime = 6;
    gpmc_cfg.timingParams.oeOffTime = 0;
    gpmc_cfg.timingParams.oeAadMuxOnTime = 1;
    gpmc_cfg.timingParams.oeAadMuxOffTime = 7;
    gpmc_cfg.timingParams.pageBurstAccess = 1;
    gpmc_cfg.timingParams.rdAccessTime = 15;
    gpmc_cfg.timingParams.wrAcessTime = 15;
    gpmc_cfg.timingParams.rdCycleTime = 17;
    gpmc_cfg.timingParams.wrCycleTime = 17;

    /* Set the default GPMC init configurations */
    GPMC_socSetInitCfg(BOARD_GPMC_INSTANCE, &gpmc_cfg);

    GPMC_init();
    GPMC_Params_init(&gpmcParams);

    /* Default devType is selected to ASRAM */
    gpmcHandle = (GPMC_Handle)GPMC_open(BOARD_DIAG_GPMC_ASRAM_INST, &gpmcParams);

    BoardDiag_genPattern(&txBuf[0],
                         BOARD_DIAG_GPMC_ASRAM_TEST_BYTES,
                         BOARD_DIAG_TEST_PATTERN_RANDOM);

    ret = BoardDiag_gpmcAsramWrite(gpmcHandle,
                                   BOARD_DIAG_GPMC_ASRAM_FIRST_VERIFY_ADDR,
                                   BOARD_DIAG_GPMC_ASRAM_TEST_BYTES,
                                   &txBuf[0]);
    if (ret != 0)
    {
        UART_printf("Writing on ASRAM failed\n");
        return -1;
    }

    ret = BoardDiag_gpmcAsramRead(gpmcHandle,
                                  BOARD_DIAG_GPMC_ASRAM_FIRST_VERIFY_ADDR,
                                  BOARD_DIAG_GPMC_ASRAM_TEST_BYTES,
                                  &rxBuf[0]);
    if (ret != 0)
    {
        UART_printf("Reading ASRAM failed\n");
        return -1;
    }

    if (BoardDiag_memCompare(&txBuf[0], &rxBuf[0],
                             BOARD_DIAG_GPMC_ASRAM_TEST_BYTES, &failIndex)
                                                                    == false)
    {
        UART_printf("\nData mismatch at offset = 0x%x\n", failIndex);
        return -1;
    }

    return 0;
}

/**
 * \brief   This function runs GPMC NOR flash boundary verification test.
 *
 * \param   deviceId     [IN]    Board flash device ID
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_gpmcRunNorflashTest(uint32_t deviceId)
{
    Board_flashHandle boardHandle;
    Board_FlashInfo *flashInfo;
    int8_t ret;
    GPMC_v1_HwAttrs gpmc_cfg;

    /* Get the default GPMC Init configurations */
    GPMC_socGetInitCfg(BOARD_GPMC_INSTANCE, &gpmc_cfg);

    /* Modify the default GPMC configurations if necessary */
    gpmc_cfg.devType = GPMC_DEVICETYPE_NORLIKE;
    gpmc_cfg.addrDataMux = GPMC_MUXADDDATA_NOMUX;
    gpmc_cfg.readType = GPMC_READTYPE_ASYNC;
    gpmc_cfg.writeType = GPMC_WRITETYPE_ASYNC;
    gpmc_cfg.chipSelAddrSize = GPMC_CS_SIZE_32MB;

    /* GPMC Timing parameters */
    gpmc_cfg.csExDelay = GPMC_CS_EXTRA_NODELAY;
    gpmc_cfg.timingParams.csOnTime = 1;
    gpmc_cfg.timingParams.csRdOffTime = 0;
    gpmc_cfg.timingParams.csWrOffTime = 1;
    gpmc_cfg.timingParams.weOnTtime = 5;
    gpmc_cfg.timingParams.weOffTime = 0;
    gpmc_cfg.timingParams.oeOnTime = 6;
    gpmc_cfg.timingParams.oeOffTime = 0;
    gpmc_cfg.timingParams.oeAadMuxOnTime = 1;
    gpmc_cfg.timingParams.oeAadMuxOffTime = 7;
    gpmc_cfg.timingParams.pageBurstAccess = 1;
    gpmc_cfg.timingParams.rdAccessTime = 15;
    gpmc_cfg.timingParams.wrAcessTime = 15;
    gpmc_cfg.timingParams.rdCycleTime = 17;
    gpmc_cfg.timingParams.wrCycleTime = 17;

    /* Set the default GPMC init configurations */
    GPMC_socSetInitCfg(BOARD_GPMC_INSTANCE, &gpmc_cfg);

    boardHandle = Board_flashOpen(deviceId,
                                  BOARD_GPMC_INSTANCE,
                                  NULL);
    if (!boardHandle)
    {
        UART_printf("Board_flashOpen Failed\n");
        return -1;
    }
    else
    {
        flashInfo = (Board_FlashInfo *)boardHandle;
        UART_printf("\nNOR Flash Device ID: 0x%x, Manufacturer ID: 0x%x \n",
                            flashInfo->device_id, flashInfo->manufacturer_id);
    }

    UART_printf("\n\nVerifying First sector...");
    ret = BoardDiag_gpmcNorReadWriteTest(boardHandle,
                                         BOARD_DIAG_GPMC_NOR_FIRST_VERIFY_ADDR,
                                         BOARD_DIAG_GPMC_NOR_TEST_BYTES);
    if (ret != 0)
    {
        UART_printf("Verifying first sector failed\n");
        Board_flashClose(boardHandle);
        return -1;
    }
    UART_printf("\nVerification of first sector was successful");

    UART_printf("\n\nVerifying Last sector...");
    ret = BoardDiag_gpmcNorReadWriteTest(boardHandle,
                                         BOARD_DIAG_GPMC_NOR_LAST_VERIFY_ADDR,
                                         BOARD_DIAG_GPMC_NOR_TEST_BYTES);
    if (ret != 0)
    {
        UART_printf("Verifying last sector failed\n");
        Board_flashClose(boardHandle);
        return -1;
    }
    UART_printf("\nVerification of last sector was successful\n");
    Board_flashClose(boardHandle);

    return 0;
}

/**
 * \brief   This function runs GPMC NAND flash boundary verification test.
 *
 * \param   deviceId     [IN]    Board flash device ID
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_gpmcRunNandflashTest(uint32_t deviceId)
{
    Board_flashHandle boardHandle;
    Board_FlashInfo *flashInfo;
    int8_t ret;
    GPMC_v1_HwAttrs gpmc_cfg;

    /* Get the default GPMC Init configurations */
    GPMC_socGetInitCfg(BOARD_GPMC_INSTANCE, &gpmc_cfg);

    /* Modify the default GPMC configurations if necessary */
    gpmc_cfg.devType = GPMC_DEVICETYPE_NANDLIKE;
    gpmc_cfg.addrDataMux = GPMC_MUXADDDATA_ADMUX;
    gpmc_cfg.readType = GPMC_READTYPE_ASYNC;
    gpmc_cfg.writeType = GPMC_WRITETYPE_ASYNC;
    gpmc_cfg.waitPinNum = 1;

    /* GPMC Timing parameters */
    gpmc_cfg.csExDelay = GPMC_CS_EXTRA_NODELAY;
    gpmc_cfg.timingParams.csOnTime = 1;
    gpmc_cfg.timingParams.csRdOffTime = 0;
    gpmc_cfg.timingParams.csWrOffTime = 1;
    gpmc_cfg.timingParams.weOnTtime = 5;
    gpmc_cfg.timingParams.weOffTime = 0;
    gpmc_cfg.timingParams.oeOnTime = 6;
    gpmc_cfg.timingParams.oeOffTime = 0;
    gpmc_cfg.timingParams.oeAadMuxOnTime = 1;
    gpmc_cfg.timingParams.oeAadMuxOffTime = 7;
    gpmc_cfg.timingParams.pageBurstAccess = 1;
    gpmc_cfg.timingParams.rdAccessTime = 15;
    gpmc_cfg.timingParams.wrAcessTime = 15;
    gpmc_cfg.timingParams.rdCycleTime = 17;
    gpmc_cfg.timingParams.wrCycleTime = 17;

    /* Set the default GPMC init configurations */
    GPMC_socSetInitCfg(BOARD_GPMC_INSTANCE, &gpmc_cfg);

    boardHandle = Board_flashOpen(deviceId,
                                  BOARD_GPMC_INSTANCE,
                                  NULL);
    if (!boardHandle)
    {
        UART_printf("Board_flashOpen Failed\n");
        return -1;
    }
    else
    {
        flashInfo = (Board_FlashInfo *)boardHandle;
        UART_printf("\nNAND Flash Device ID: 0x%x, Manufacturer ID: 0x%x \n",
                            flashInfo->device_id, flashInfo->manufacturer_id);
    }

    UART_printf("\n\nVerifying First page...");
    ret = BoardDiag_gpmcNandReadWriteTest(boardHandle,
                                          BOARD_DIAG_GPMC_NAND_FIRST_VERIFY_ADDR,
                                          BOARD_DIAG_GPMC_NAND_TEST_BYTES);
    if (ret != 0)
    {
        UART_printf("Verifying first page failed\n");
        Board_flashClose(boardHandle);
        return -1;
    }
    UART_printf("\nVerification of first page was successful");

    Board_flashClose(boardHandle);

    return 0;
}

/**
 * \brief   The function runs the GPMC device test based on the user input.
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_gpmcRunTest(void)
{
    uint8_t userInput;
    int8_t ret;

    while(1)
    {
        UART_printf("\nSelect the below option to execute the test\n");
        UART_printf("\n1. NAND\n");
        UART_printf("\n2. NAND Socket\n");
        UART_printf("\n3. NOR\n");
        UART_printf("\n4. ASRAM\n");
        UART_printf("\n0. Exit\n");
        UART_printf("\n Enter the option: ");
        UART_scanFmt("%c", &userInput);

        switch(userInput)
        {
            case '1':
                ret = BoardDiag_gpmcRunNandflashTest(BOARD_FLASH_ID_MT29F4G16ABAFAH);
                break;

            case '2':
                ret = BoardDiag_gpmcRunNandflashTest(BOARD_FLASH_ID_MT29F8G16ABACAWP);
                break;

            case '3':
                ret = BoardDiag_gpmcRunNorflashTest(BOARD_FLASH_ID_MT28EW256ABA);
                break;

            case '4':
                ret = BoardDiag_gpmcRunAsramTest(BOARD_FLASH_ID_CY7C10612G);
                break;

            case '0':
                return(0);

            default:
                UART_printf("\nInvalid option... Try again...\n");
                break;
        }

        if (ret)
        {
            UART_printf("\nTest Failed!!\n");
            return -1;
        }
        else
        {
            UART_printf("\nTest Passed!!\n");
        }
    }

}

/**
 * \brief   The function performs the GPMC Diagnostic test.
 *
 * \return  int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_GpmcTest(void)
{
    int8_t ret;

    ret = BoardDiag_gpmcRunTest();
    if (ret)
    {
        UART_printf("\nGPMC Test Failed!!\n");
    }
    else
    {
        UART_printf("\nGPMC Flash Test Passed!\n");
    }

    return ret;
}

/**
 *  \brief   GPMC Diagnostic test main function
 *
 *  \return  int - GPMC Diagnostic test status.
 *             0 - in case of success
 *            -1 - in case of failure.
 *
 */
#ifndef SPI_BOOT_FRAMEWORK
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    int ret = 0;
    Board_PinmuxConfig_t gpmcPrg0Pinmux;

    Board_pinmuxGetCfg(&gpmcPrg0Pinmux);
    gpmcPrg0Pinmux.muxCfg = BOARD_PINMUX_CUSTOM;
    gpmcPrg0Pinmux.expBoardMux = BOARD_PINMUX_EXP_GPMC;
    Board_pinmuxSetCfg(&gpmcPrg0Pinmux);

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO | 
               BOARD_INIT_PINMUX_CONFIG;
#endif

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    UART_printf("\n*********************************************\n");
    UART_printf  ("*                GPMC Test                  *\n");
    UART_printf  ("*********************************************\n");

    ret = BoardDiag_GpmcTest();

    return ret;
}
#endif
