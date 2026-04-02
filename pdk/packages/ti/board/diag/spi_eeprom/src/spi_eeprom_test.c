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
 * \file   spi_eeprom_test.c
 *
 * \brief  spi eeprom diagnostic test file.
 *
 * Targeted Functionality: Verifies the spi eeprom.
 *
 * Operation: This test verifies by writing and reading the data
 *             to eeprom through SPI.
 *
 * Supported SoCs: AM64X.
 *
 * Supported Platforms: am64x_evm.
 */

#include "spi_eeprom_test.h"

uint32_t gChNum  = 0;
/* Buffer containing the known data that needs to be written to Eeprom */
uint16_t txBuf[BOARD_DIAG_MAX_BUFF_SIZE];
/* Buffer containing the received data */
uint16_t rxBuf[BOARD_DIAG_MAX_BUFF_SIZE];
uint16_t cmdBuf[BOARD_DIAG_MAX_BUFF_SIZE];

/**
 *  \brief    Function to enable SPI for data transfers
 *
 */
static void BoardDiag_spiEepromStartTransfer(void)
{
    /* Enable the transmitter FIFO of McSPI peripheral.*/
    McSPITxFIFOConfig(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_TX_FIFO_ENABLE, MCSPI_CHANNEL_0);

    /* Enable the receiver FIFO of McSPI peripheral.*/
    McSPIRxFIFOConfig(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_RX_FIFO_ENABLE, MCSPI_CHANNEL_0);

    /* Enable the McSPI channel for communication.*/
    McSPIChannelEnable(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0);

    /* SPIEN line is forced to low state.*/
    McSPICSAssert(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0);
}

/**
 *  \brief    Function to disable SPI for data transfers
 *
 */
static void BoardDiag_spiEepromStopTransfer(void)
{
    /* Force SPIEN line to the inactive state.*/
    McSPICSDeAssert(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0);
    /* Disable the McSPI channel.*/
    McSPIChannelDisable(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0);
}

/**
 *  \brief    Writes a block of data to SPI module
 *
 *  \param    dataBuf    [IN]   Buffer with data to write
 *  \param    length     [IN]   Length of data to write
 *
 * \return
 * \n      0   - Success
 * \n      -1  - Failure
 */
static int8_t BoardDiag_spiEepromWriteData(uint16_t *dataBuf, uint16_t length)
{
    uint16_t dataCount = 0;
    uint32_t dummyByte = 0;
    volatile uint32_t channelStatus = 0;

    dummyByte = dummyByte;

    while( dataCount < length)
    {
        channelStatus = McSPIChannelStatusGet(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0);
        if ((channelStatus & CSL_MCSPI_CH0STAT_TXS_MASK) == CSL_MCSPI_CH0STAT_TXS_MASK)
        {
            McSPITransmitData(BOARD_DIAG_SPI0_BASE_ADDRESS, *dataBuf, MCSPI_CHANNEL_0);
            dataCount++;
            dataBuf++;

            /* Wait till the transfer is complete */
            while((McSPIChannelStatusGet(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0) & CSL_MCSPI_CH0STAT_EOT_MASK) != CSL_MCSPI_CH0STAT_EOT_MASK);
        }
        else
        {
            UART_printf("Getting SPI channel status failed\n");
            return (int8_t)channelStatus;
        }

        /* Flush the data dummy data from the Rx buffer */
        if((channelStatus & CSL_MCSPI_CH0STAT_RXS_MASK) == CSL_MCSPI_CH0STAT_RXS_MASK)
        {
            dummyByte = McSPIReceiveData(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0);
        }
    }

    return 0;
}

/**
 *  \brief    Reads a block of data from SPI module
 *
 *  \param    dataBuf    [IN]   Buffer to store the data read
 *  \param    length     [IN]   Length of data to read
 *
 */
static void BoardDiag_spiEepromReadData(uint16_t *dataBuf, uint16_t length)
{
    uint16_t dataCount = 0;
    uint32_t dummyByte = 0;
    volatile uint32_t channelStatus = 0;

    /* Flush the dummy data from the Rx buffer */
    channelStatus = McSPIChannelStatusGet(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0);
    while((channelStatus & CSL_MCSPI_CH0STAT_RXS_MASK) == CSL_MCSPI_CH0STAT_RXS_MASK)
    {
        dummyByte = McSPIReceiveData(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0);
        channelStatus = McSPIChannelStatusGet(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0);
    }

    dummyByte = 0;

    while( dataCount < length)
    {
        channelStatus = McSPIChannelStatusGet(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0);
        if ((channelStatus & CSL_MCSPI_CH0STAT_RXS_MASK) == CSL_MCSPI_CH0STAT_RXS_MASK)
        {
            *(uint16_t *)dataBuf = (uint16_t)McSPIReceiveData(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0);
            dataCount++;
            dataBuf++;
        }

        if ((channelStatus & CSL_MCSPI_CH0STAT_TXS_MASK) == CSL_MCSPI_CH0STAT_TXS_MASK)
        {
            McSPITransmitData(BOARD_DIAG_SPI0_BASE_ADDRESS, dummyByte, MCSPI_CHANNEL_0);

            /* Wait till the transfer is complete */
            while((McSPIChannelStatusGet(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_CHANNEL_0) & CSL_MCSPI_CH0STAT_EOT_MASK) != CSL_MCSPI_CH0STAT_EOT_MASK);
        }
    }
}

/**
 *  \brief    Writes a SPI Eeprom page
 *
 *  \param    dataBuf    [IN]   Buffer with data to write
 *  \param    pageOffset [IN]   Eeprom page offset
 *  \param    length     [IN]   Length of data to write
 *
 * \return
 * \n      0   - Success
 * \n      -1  - Failure
 */
static int8_t BoardDiag_spiEepromWritePage(uint16_t *dataBuf, uint16_t pageOffset, uint16_t length)
{
    uint16_t cmd[1];
    int8_t testStatus = 0;

    BoardDiag_spiEepromStartTransfer();

    cmd[0] = BOARD_DIAG_NOR_CMD_PAGE_PROG | pageOffset;
    testStatus = BoardDiag_spiEepromWriteData(cmd, 1);
    if(testStatus != 0)
    {
        UART_printf("\nSPI Eeprom page programming Failed\n");
        return -1;
    }

    /* 16-bit data transfer */
    McSPIWordLengthSet(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_WORD_LENGTH(16), gChNum);
    testStatus = BoardDiag_spiEepromWriteData(dataBuf, length);
    if(testStatus != 0)
    {
        UART_printf("\nSPI Eeprom write data Failed\n");
        return -1;
    }
    BoardDiag_spiEepromStopTransfer();
    McSPIWordLengthSet(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_WORD_LENGTH(9), gChNum);
    return 0;
}

/**
 *  \brief    Reads a SPI Eeprom page
 *
 *  \param    dataBuf    [IN]   Buffer to store the data read
 *  \param    pageOffset [IN]   Eeprom page offset
 *  \param    length     [IN]   Length of data to read
 *
 */
static void BoardDiag_spiEepromReadPage(uint16_t *dataBuf, uint16_t pageOffset, uint16_t length)
{
    uint16_t cmd[1];

    BoardDiag_spiEepromStartTransfer();

    cmd[0] = BOARD_DIAG_NOR_CMD_READ | pageOffset;

    McSPIWordLengthSet(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_WORD_LENGTH(9), gChNum);
    BoardDiag_spiEepromWriteData(cmd, 1);
    BOARD_delay(50000); /* 50msec delay to complete write */

    /* 17 clock cycle requires to read data (Dummy 0 bit is preceding the data */
    McSPIWordLengthSet(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_WORD_LENGTH(17), gChNum);
    BoardDiag_spiEepromReadData(dataBuf, length);

    /* Setting bit length to 9 */
    McSPIWordLengthSet(BOARD_DIAG_SPI0_BASE_ADDRESS, MCSPI_WORD_LENGTH(9), gChNum);

    BoardDiag_spiEepromStopTransfer();
}

/**
 *  \brief    Sends a command to Eeprom
 *
 *  \param    cmdBuf [IN]   Command buffer pointer
 *  \param    length [IN]   Length of command in bytes
 *
 * \return
 * \n      0   - Success
 * \n      -1  - Failure
 */
static int8_t BoardDiag_spiEepromSendCmd(uint16_t *cmdBuf, uint16_t length)
{
    BoardDiag_spiEepromStartTransfer();
    BoardDiag_spiEepromWriteData(cmdBuf, length);
    BoardDiag_spiEepromStopTransfer();
    return 0;
}

/**
 *  \brief    Executes SPI Eeprom memory test
 *
 *  \param    pageOffset [IN]   Eeprom page offset
 *
 * \return
 * \n      0   - Success
 * \n      -1  - Failure
 */
static int8_t BoardDiag_spiEepromRunTest(uint16_t  pageOffset)
{
    uint8_t count;
    uint8_t index;
    uint16_t pageOff;
    uint32_t failIndex;
    int8_t testStatus = 0;

    BoardDiag_genPattern((uint8_t *)cmdBuf, BOARD_DIAG_MAX_BUFF_SIZE,
                         BOARD_DIAG_TEST_PATTERN_NULL);
#if defined(DIAG_COMPLIANCE_TEST)
    BoardDiag_genPattern((uint8_t *)txBuf, BOARD_DIAG_MAX_BUFF_SIZE,
                         BOARD_DIAG_TEST_PATTERN_AA_55);
#else
    /* Data byte is 2, but buffer size converter into 1 byte */
    BoardDiag_genPattern((uint8_t *)txBuf, (BOARD_DIAG_MAX_BUFF_SIZE * 2),
                         BOARD_DIAG_TEST_PATTERN_RANDOM);
#endif
    BoardDiag_genPattern((uint8_t *)rxBuf, BOARD_DIAG_MAX_BUFF_SIZE,
                         BOARD_DIAG_TEST_PATTERN_NULL);

    /* Write Enable */
    cmdBuf[0] = BOARD_DIAG_NOR_CMD_WREN;
    count = 1U;
    testStatus = BoardDiag_spiEepromSendCmd(cmdBuf, count);
    if(testStatus != 0)
    {
        UART_printf("\nSPI Eeprom write enable Failed\n");
        return -1;
    }

    /* Erasing the complete memory */
    cmdBuf[0] = BOARD_DIAG_NOR_CMD_ERASE_ALL;
    count = 1U;

    testStatus = BoardDiag_spiEepromSendCmd(cmdBuf, count);
    if(testStatus != 0)
    {
        UART_printf("\nSPI Eeprom erase all Failed\n");
        return -1;
    }
    BOARD_delay(50000); /* 50msec delay to complete erase */

    pageOff = pageOffset;
    /* Write to SPI Eeprom */
    UART_printf("\nWriting EEPROM...\n");
    for (index = 0; index < BOARD_DIAG_MAX_BUFF_SIZE; index++)
    {
        testStatus = BoardDiag_spiEepromWritePage(&(txBuf[index]), pageOff, 1U);
        if(testStatus != 0)
        {
            UART_printf("\nSPI Eeprom write page Failed\n");
            return -1;
        }

        pageOff++;
        BOARD_delay(50000); /* 50msec delay to complete write */
    }

    UART_printf("\nReading EEPROM...\n");
    /* Read from SPI Eeprom */
    for (index = 0; index < BOARD_DIAG_MAX_BUFF_SIZE; index++)
    {
        BoardDiag_spiEepromReadPage(&(rxBuf[index]), pageOffset, 1U);
        BOARD_delay(50000);

        pageOffset++;
    }

    if(BoardDiag_memCompare((uint8_t *)txBuf, (uint8_t *)rxBuf, (BOARD_DIAG_MAX_BUFF_SIZE * 2), &failIndex) == true)
    {
        UART_printf("\nData Read matches with Data written to page\n");
    }
    else
    {
        UART_printf("\nSPI Eeprom Data mismatch at index = 0x%x\n", failIndex);
        return -1;
    }

    return 0;
}

/**
 *  \brief    Function to initialize SPI module
 *
 * \return
 * \n      0   - Success
 * \n      -1  - Failure
 */
static int8_t BoardDiag_spiEepromInit(void)
{
    int8_t status = 1U;

    /* Reset the McSPI instance.*/
    McSPIReset(BOARD_DIAG_SPI0_BASE_ADDRESS);

    /* CLOCKACTIVITY bit - OCP and Functional clocks are maintained           */
    /* SIDLEMODE     bit - Ignore the idle request and configure in normal mode
     */
    /* AUTOIDLE      bit - Disable (OCP clock is running free, no gating)     */
    MCSPISysConfigSetup(BOARD_DIAG_SPI0_BASE_ADDRESS,
                        MCSPI_CLOCKS_OCP_ON_FUNC_ON,
                        MCSPI_SIDLEMODE_NO,
                        MCSPI_WAKEUP_DISABLE,
                        MCSPI_AUTOIDLE_OFF);

    /* Enable chip select pin.*/
    McSPICSEnable(BOARD_DIAG_SPI0_BASE_ADDRESS);

    /* Enable master mode of operation.*/
    McSPIMasterModeEnable(BOARD_DIAG_SPI0_BASE_ADDRESS);

    /* Perform the necessary configuration for master mode. */
    status = McSPIMasterModeConfig(BOARD_DIAG_SPI0_BASE_ADDRESS,
                                   MCSPI_SINGLE_CH,
                                   MCSPI_TX_RX_MODE,
                                   MCSPI_DATA_LINE_COMM_MODE_6,
                                   gChNum);
    if(status == 0)
    {
        UART_printf("MCSPI communication failed\n");
        return -1;
    }

    /* Configure the McSPI bus clock depending on clock mode. */
    McSPIClkConfig(BOARD_DIAG_SPI0_BASE_ADDRESS,
                   BOARD_DIAG_MCSPI_IN_CLK,
                   BOARD_DIAG_MCSPI_OUT_FREQ,
                   gChNum,
                   MCSPI_CLK_MODE_0);

    /* Configure the word length.*/
    McSPIWordLengthSet(BOARD_DIAG_SPI0_BASE_ADDRESS,
                       MCSPI_WORD_LENGTH(9),
                       gChNum);

    /* Set polarity of SPIEN to low.*/
    McSPICSPolarityConfig(BOARD_DIAG_SPI0_BASE_ADDRESS,
                          MCSPI_CS_POL_HIGH,
                          gChNum);

    /* SPI_CS1 is connected to a level translator on IDK application card
       which should be pulled high by default for proper operation of Eeprom
     */
    McSPICSPolarityConfig(BOARD_DIAG_SPI0_BASE_ADDRESS,
                          MCSPI_CS_POL_LOW,
                          1);
    return 0;
}

/**
 *  \brief    Executes SPI Eeprom test
 *
 * \return
 * \n      0   - Test Passed
 * \n      -1  - Test Failed
 */
int8_t BoardDiag_spiEepromTest(void)
{
    uint16_t pageOffset = 0;
    int8_t testStatus;

#ifdef DIAG_STRESS_TEST
    UART_printf("\n************************************************\n");
    UART_printf  ("*            SPI EEPROM Stress Test             *\n");
    UART_printf  ("************************************************\n");
#else
    UART_printf("\n***************************************\n");
    UART_printf  ("*            SPI EEPROM Test          *\n");
    UART_printf  ("***************************************\n");
#endif

    testStatus = BoardDiag_spiEepromInit();
    if(testStatus != 0)
    {
        UART_printf("\nSPI Eeprom init Failed\n");
        return -1;
    }

    UART_printf("\nVerifying %d Bytes from offset - 0x%x\n", BOARD_DIAG_MAX_BUFF_SIZE, pageOffset);

    testStatus = BoardDiag_spiEepromRunTest(pageOffset);
    if(testStatus != 0)
    {
        UART_printf("\nSPI EEPROM Test Failed\n");
        return -1;
    }
    else
    {
        UART_printf("\nSPI EEPROM Test Passed\n");
    }
    return testStatus;
}

/**
 * \brief  main function
 *
 *  This function performs board intializations and calls spi test
 *
 * \return  int
 *              0  - in case of success
 *             -1  - in case of failure
 */
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK  |
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

    return BoardDiag_spiEepromTest();
}
