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
 *  \file   spi_norflash_test.c
 *
 *  \brief   spi norflash diagnostic test file
 *
 *  Targeted Functionality: Verification of spi norflash
 *
 *  Operation: This test verifies the SPI flash memory read and write
 *             access by writing a test pattern to a memory page and
 *             reading the same page for data verification.
 *
 *  Supported SoCs : J7200
 *
 *  Supported Platforms: j7200_evm
 *
 */

#include "spi_norflash_test.h"

uint32_t gChNum  = 0;
uint8_t cmdBuf[BOARD_DIAG_MAX_BUFF_SIZE];

/* Buffer containing the known data that needs to be written to flash */
uint8_t txBuf[BOARD_DIAG_MAX_BUFF_SIZE];
/* Buffer containing the received data */
uint8_t rxBuf[BOARD_DIAG_MAX_BUFF_SIZE];

/**
 *  \brief    This function enables SPI for data transfers
 *
 */
static void BoardDiag_spiNorflashStartTransfer(void)
{
    /* Enable the transmitter FIFO of McSPI peripheral.*/
    McSPITxFIFOConfig(BOARD_DIAG_SPI_BASE_ADDR, MCSPI_TX_FIFO_ENABLE, BOARD_DIAG_SPI_CH);

    /* Enable the receiver FIFO of McSPI peripheral.*/
    McSPIRxFIFOConfig(BOARD_DIAG_SPI_BASE_ADDR, MCSPI_RX_FIFO_ENABLE, BOARD_DIAG_SPI_CH);

    /* Enable the McSPI channel for communication.*/
    McSPIChannelEnable(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH);

    /* SPIEN line is forced to low state.*/
    McSPICSAssert(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH);
}

/**
 *  \brief    This function disables SPI for data transfers
 *
 */
static void BoardDiag_spiNorflashStopTransfer(void)
{
    /* Force SPIEN line to the inactive state */
    McSPICSDeAssert(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH);
    /* Disable the McSPI channel */
    McSPIChannelDisable(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH);
}

/**
 *  \brief    This function writes data to SPI module.
 *
 *  \param    dataBuf    [IN]   Buffer with data to write
 *  \param    length     [IN]   Length of data to write
 *
 */
static void BoardDiag_spiNorflashWriteData(uint8_t *dataBuf, uint16_t length)
{
    volatile uint32_t chSts = 0;
    uint16_t data_count = 0;

    while(data_count < length)
    {
        chSts = McSPIChannelStatusGet(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH);
        if ((chSts & CSL_MCSPI_CH0STAT_TXS_MASK) == CSL_MCSPI_CH0STAT_TXS_MASK)
        {
            McSPITransmitData(BOARD_DIAG_SPI_BASE_ADDR, (uint32_t) (*(uint8_t *)dataBuf), BOARD_DIAG_SPI_CH);
            data_count++;
            dataBuf++;

            /* Wait till the transfer is complete */
            while((McSPIChannelStatusGet(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH) & CSL_MCSPI_CH0STAT_EOT_MASK) != CSL_MCSPI_CH0STAT_EOT_MASK);
        }

        /* Flush the dummy data from the Rx buffer */
        if((chSts & CSL_MCSPI_CH0STAT_RXS_MASK) == CSL_MCSPI_CH0STAT_RXS_MASK)
        {
            McSPIReceiveData(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH);
        }
    }
}

/**
 *  \brief    This function reads data from SPI module
 *
 *  \param    dataBuf    [IN]   Buffer to store the data read
 *  \param    length     [IN]   Length of data to read
 *
 */
static void BoardDiag_spiNorflashReadData(uint8_t *dataBuf, uint16_t length)
{
    uint16_t data_count = 0;
    uint8_t dummyByte = 0;
    volatile uint32_t chSts = 0;

    /* Flush the data dummy data from the Rx buffer */
    chSts = McSPIChannelStatusGet(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH);
    while((chSts & CSL_MCSPI_CH0STAT_RXS_MASK) == CSL_MCSPI_CH0STAT_RXS_MASK)
    {
        dummyByte = McSPIReceiveData(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH);
        chSts = McSPIChannelStatusGet(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH);
    }

    dummyByte = 0;

    while(data_count < length)
    {
        chSts = McSPIChannelStatusGet(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH);
        if ((chSts & CSL_MCSPI_CH0STAT_RXS_MASK) == CSL_MCSPI_CH0STAT_RXS_MASK)
        {
            *(uint8_t *)dataBuf = (uint8_t)McSPIReceiveData(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH);
            data_count++;
            dataBuf++;
        }

        if ((chSts & CSL_MCSPI_CH0STAT_TXS_MASK) == CSL_MCSPI_CH0STAT_TXS_MASK)
        {
            McSPITransmitData(BOARD_DIAG_SPI_BASE_ADDR, dummyByte, BOARD_DIAG_SPI_CH);

            /* Wait till the transfer is complete */
            while((McSPIChannelStatusGet(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_SPI_CH) & CSL_MCSPI_CH0STAT_EOT_MASK) != CSL_MCSPI_CH0STAT_EOT_MASK);
        }        
    }
}

/**
 *  \brief    This function writes data into flash pages
 *
 *  \param    dataBuf    [IN]   Buffer with data to write
 *  \param    pageOffset [IN]   NOR flash page offset
 *  \param    length     [IN]   Length of data to write
 *
 */
static void BoardDiag_spiNorflashWritePage(uint8_t *dataBuf, uint32_t pageOffset, uint16_t length)
{
    uint8_t cmd[4];

    BoardDiag_spiNorflashStartTransfer();

    cmd[0] = BOARD_DIAG_NOR_CMD_PAGE_PROG;
    cmd[1] = ((pageOffset >> 16) & 0xFF);
    cmd[2] = ((pageOffset >> 8) & 0xFF);
    cmd[3] = (pageOffset & 0xFF);

    BoardDiag_spiNorflashWriteData(cmd, 4);
    BoardDiag_spiNorflashWriteData(dataBuf, length);

    BoardDiag_spiNorflashStopTransfer();
}

/**
 *  \brief    This function reads data from flash page
 *
 *  \param    dataBuf    [IN]   Buffer to store the data read
 *  \param    pageOffset [IN]   NOR flash page offset
 *  \param    length     [IN]   Length of data to read
 *
 */
static void BoardDiag_spiNorflashReadPage(uint8_t *dataBuf, uint32_t pageOffset, uint16_t length)
{
    uint8_t cmd[4];

    BoardDiag_spiNorflashStartTransfer();

    cmd[0] = BOARD_DIAG_NOR_CMD_READ;
    cmd[1] = ((pageOffset >> 16) & 0xFF);
    cmd[2] = ((pageOffset >> 8) & 0xFF);
    cmd[3] = (pageOffset & 0xFF);

    BoardDiag_spiNorflashWriteData(cmd, 4);
    BoardDiag_spiNorflashReadData(dataBuf, length);

    BoardDiag_spiNorflashStopTransfer();
}

/**
 *  \brief    This function waits till flash is in busy state
 *
 */
static void BoardDiag_spiNorflashBusyWait(void)
{
    uint8_t data = 0;

    do
    {
        BoardDiag_spiNorflashStartTransfer();
        data = BOARD_DIAG_NOR_CMD_RDSR;
        BoardDiag_spiNorflashWriteData(&data, 1);
        BoardDiag_spiNorflashReadData(&data, 1);
        BoardDiag_spiNorflashStopTransfer();
    } while(data & 0x01);
}

/**
 *  \brief    This function sends a command to flash
 *
 *  \param    cmdBuf [IN]   Command buffer pointer
 *  \param    length [IN]   Length of command in bytes
 *
 */
static void BoardDiag_spiNorflashSendCmd(uint8_t *cmdBuf, uint16_t length)
{
    BoardDiag_spiNorflashStartTransfer();
    BoardDiag_spiNorflashWriteData(cmdBuf, length);
    BoardDiag_spiNorflashStopTransfer();
}

/**
 *  \brief    This function reads Norflash device ID.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_spiNorFlashReadDevID(void)
{
    UART_printf("Reading Flash Device ID...\n");

    BoardDiag_spiNorflashStartTransfer();
    cmdBuf[0] = BOARD_DIAG_NOR_CMD_RDID;
    BoardDiag_spiNorflashWriteData(cmdBuf, 1);
    BoardDiag_spiNorflashReadData(rxBuf, 3);

    BoardDiag_spiNorflashStopTransfer();

    UART_printf("Device ID 0 - 0x%X\n", rxBuf[0]);
    UART_printf("Device ID 1 - 0x%X\n", rxBuf[1]);
    UART_printf("Device ID 2 - 0x%X\n", rxBuf[2]);

    if((rxBuf[0] == BOARD_DIAG_SPI_FLASH_DEVID_BYTE0) &&
       (rxBuf[1] == BOARD_DIAG_SPI_FLASH_DEVID_BYTE1) &&
       (rxBuf[2] == BOARD_DIAG_SPI_FLASH_DEVID_BYTE2))
    {
        UART_printf("Norflash Device ID Match!\n");
    }
    else
    {
        UART_printf("Norflash Device ID Mismatch!\n");
        return -1;
    }

    return 0;
}

/**
 *  \brief    This function executes SPI Norflash memory test
 *
 *  \param    pageOffset [IN]   NOR flash page offset
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 */
static int8_t BoardDiag_spiNorflashRunTest(uint32_t  pageOffset)
{
    uint32_t failIndex;
    uint16_t count;

    BoardDiag_genPattern((uint8_t *)cmdBuf, BOARD_DIAG_MAX_BUFF_SIZE,
                         BOARD_DIAG_TEST_PATTERN_NULL);

    BoardDiag_genPattern((uint8_t *)txBuf, BOARD_DIAG_MAX_BUFF_SIZE,
                         BOARD_DIAG_TEST_PATTERN_INC);

    BoardDiag_genPattern((uint8_t *)rxBuf, BOARD_DIAG_MAX_BUFF_SIZE,
                         BOARD_DIAG_TEST_PATTERN_NULL);

    /* Sector erase */
    if(!(pageOffset % BOARD_DIAG_SECTOR_SIZE))
    {
        /* Write Enable */
        cmdBuf[0] = BOARD_DIAG_NOR_CMD_WREN;
        count = 1U;
        BoardDiag_spiNorflashSendCmd(cmdBuf, count);

        cmdBuf[0] = BOARD_DIAG_NOR_CMD_SECTOR_ERASE;
        cmdBuf[1] = ((pageOffset >> 16) & 0xFF);
        cmdBuf[2] = ((pageOffset >> 8) & 0xFF);
        cmdBuf[3] = (pageOffset & 0xFF);
        count = 4U;
        BoardDiag_spiNorflashSendCmd(cmdBuf, count);

        /* Check for the busy state */
        BoardDiag_spiNorflashBusyWait();
    }

    /* Write Enable */
    cmdBuf[0] = BOARD_DIAG_NOR_CMD_WREN;
    count = 1U;
    BoardDiag_spiNorflashSendCmd(cmdBuf, count);

    /* Write to SPI Flash */
    BoardDiag_spiNorflashWritePage(txBuf, pageOffset, BOARD_DIAG_MAX_BUFF_SIZE);

    /*  Check for Flash Busy */
    BoardDiag_spiNorflashBusyWait();

    /* Read from SPI flash */
    BoardDiag_spiNorflashReadPage(rxBuf, pageOffset, BOARD_DIAG_MAX_BUFF_SIZE);

    if(BoardDiag_memCompare(txBuf, rxBuf, BOARD_DIAG_MAX_BUFF_SIZE, &failIndex) == BTRUE)
    {
        if(!(pageOffset % BOARD_DIAG_SECTOR_SIZE))
        {
#if !defined(DIAG_STRESS_TEST)
            UART_printf("Data Read matches with Data written\n");
            UART_printf("SPI Flash Test Passed!\n");
#endif
        }
    }
    else
    {
        UART_printf("SPI Flash Test Failed at index = 0x%x\n", failIndex);
        return -1;
    }

    return 0;
}

/**
 *  \brief    This function initialze spi.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_spiNorflashSpiInit(void)
{
    int8_t status;

    /* Reset the McSPI instance.*/
    McSPIReset(BOARD_DIAG_SPI_BASE_ADDR);

    /* AUTOIDLE      bit - Disable (OCP clock is running free, no gating)     */
    MCSPISysConfigSetup(BOARD_DIAG_SPI_BASE_ADDR, MCSPI_CLOCKS_OCP_ON_FUNC_ON,
                        MCSPI_SIDLEMODE_NO, MCSPI_WAKEUP_DISABLE,
                        MCSPI_AUTOIDLE_OFF);

    /* Enable chip select pin.*/
    McSPICSEnable(BOARD_DIAG_SPI_BASE_ADDR);

    /* Enable master mode of operation.*/
    McSPIMasterModeEnable(BOARD_DIAG_SPI_BASE_ADDR);

    /* Perform the necessary configuration for master mode. */
    status = McSPIMasterModeConfig(BOARD_DIAG_SPI_BASE_ADDR, MCSPI_SINGLE_CH,
                                   MCSPI_TX_RX_MODE,
                                   MCSPI_DATA_LINE_COMM_MODE_6,
                                   gChNum);
    if(status == 0)
    {
        UART_printf("MCSPI communication failed\n");
        return -1;
    }

    /* Configure the McSPI bus clock depending on clock mode. */
    McSPIClkConfig(BOARD_DIAG_SPI_BASE_ADDR, BOARD_DIAG_MCSPI_IN_CLK, BOARD_DIAG_MCSPI_OUT_FREQ, gChNum,
                   MCSPI_CLK_MODE_0);

    /* Configure the word length.*/
    McSPIWordLengthSet(BOARD_DIAG_SPI_BASE_ADDR, MCSPI_WORD_LENGTH(8), gChNum);

    /* Set polarity of SPIEN to low.*/
    McSPICSPolarityConfig(BOARD_DIAG_SPI_BASE_ADDR,
                          MCSPI_CS_POL_LOW, gChNum);

    return 0;
}

#ifdef DIAG_STRESS_TEST
/**
 *  \brief    This function runs spi norflash stress test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_spiNorFlashStressTest(void)
{
    uint32_t pageNum;
    int8_t testStatus;
    char rdBuf = 'y';

    testStatus = BoardDiag_spiNorflashSpiInit();
    if(testStatus != 0)
    {
        UART_printf("\nSPI NOR Flash init Failed\n");
        return -1;
    }

    testStatus = BoardDiag_spiNorFlashReadDevID();
    if(testStatus != 0)
    {
        UART_printf("\nFlash Device ID Read Failed!!\n");
        return -1;
    }
    else
    {
        UART_printf("\nFlash Device ID Read Passed!\n\n");
    }

    for(pageNum = 0; pageNum < BOARD_DIAG_SPI_FLASH_TEST_PAGES; pageNum += BOARD_DIAG_TESTPAGES)
    {
        if(!(pageNum & 0xff))
        {
            UART_printf("\nVerifying Sector - %d\n", (pageNum/BOARD_DIAG_PAGES_PER_SECTOR));
        }

        testStatus = BoardDiag_spiNorflashRunTest((BOARD_DIAG_NOR_PAGE_SIZE * pageNum));
        if(testStatus != 0)
        {
            UART_printf("\nSPI NOR Flash Test Failed\n");
            return -1;
        }

        /* Check if there a input from console to break the test */
        rdBuf = (char)BoardDiag_getUserInput(BOARD_UART_INSTANCE);
        if((rdBuf == 'b') || (rdBuf == 'B'))
        {
            UART_printf("Received Test Termination... Exiting the Test\n");
            pageNum += BOARD_DIAG_TESTPAGES;
            UART_printf("SPI NOR Flash Stress Test Status\n");
            UART_printf("============================================\n");
            UART_printf("\nSPI NOR Flash Verified up-to Page - %d\n", pageNum);
            break;
        }
    }

	return testStatus;
}
#else
/**
 *  \brief    This function runs spi norflash functional test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_spiNorFlashFunctionalTest(void)
{
    uint32_t pageNum;
    int8_t testStatus;

    testStatus = BoardDiag_spiNorflashSpiInit();
    if(testStatus != 0)
    {
        UART_printf("\nSPI NOR Flash init Failed\n");
        return -1;
    }

    testStatus = BoardDiag_spiNorFlashReadDevID();
    if(testStatus != 0)
    {
        UART_printf("\nFlash Device ID Read Failed!!\n");
        return -1;
    }
    else
    {
        UART_printf("\nFlash Device ID Read Passed!\n");
    }

    for(pageNum = 0; pageNum < BOARD_DIAG_SPI_FLASH_TEST_PAGES; pageNum += BOARD_DIAG_TESTPAGES)
    {
        if(!(pageNum & 0xff))
        {
            UART_printf("\nVerifying Sector - %d\n", (pageNum/BOARD_DIAG_PAGES_PER_SECTOR));
        }

        testStatus = BoardDiag_spiNorflashRunTest((BOARD_DIAG_NOR_PAGE_SIZE * pageNum));
        if(testStatus != 0)
        {
            UART_printf("\nSPI NOR Flash Test Failed\n");
            return -1;
        }
    }

	return testStatus;
}
#endif

/**
 *  \brief    This function executes spi norflash test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_spiNorflashTest(void)
{
   int8_t ret;

#ifdef DIAG_STRESS_TEST
    UART_printf("\n****************************************************\n");
    UART_printf  ("*             NOR FLASH Stress Test                *\n");
    UART_printf  ("****************************************************\n");
    ret = BoardDiag_spiNorFlashStressTest();
#else
    UART_printf("\n*********************************************\n");
    UART_printf  ("*             NOR FLASH Test                *\n");
    UART_printf  ("*********************************************\n");
    ret = BoardDiag_spiNorFlashFunctionalTest();
#endif /* #ifndef DIAG_STRESS_TEST */

    return ret;
}

/**
 * \brief  SPI Norflash Diagnostic test main function
 *
 * \return  int - SPI Norflash Diagnostic test status
 *            0 - in case of success
 *           -1 - in case of failure
 */
#ifndef SPI_BOOT_FRAMEWORK
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    int8_t ret = 0;

#if defined(j7200_evm)
    Board_PinmuxConfig_t pinmuxCfg;
    Board_pinmuxGetCfg(&pinmuxCfg);
    pinmuxCfg.somMux = BOARD_PINMUX_SOM_PROFIBUS; /* Enables SPI pinmux */
    Board_pinmuxSetCfg(&pinmuxCfg);
#endif

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG;
#endif

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    Board_control(BOARD_CTRL_CMD_SET_SOM_PROFIBUS_MUX, NULL);

    ret = BoardDiag_spiNorflashTest();
    if(ret != 0)
    {
        UART_printf("\nSPI Norflash Test Failed\n");
        return -1;
    }
    else
    {
        UART_printf("\nSPI Norflash Test Passed\n");
    }

    return 0;
}
#endif
