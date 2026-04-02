/******************************************************************************
* Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
*****************************************************************************/

/**
 * \file   exp_header_tpr12.c
 *
 * \brief  HD connector test file.
 *
 *  Targeted Functionality: Verification of FE HD connector control 
 *                          lines as gpio through spi interface for 
 *                            handshake mechanism.
 *
 *  Operation: This application toggles the control signal as gpio 
 *               lines connected to HD connector FE1 and FE2 which 
 *               are connected to externak IWR1443 radar sensor module.
 *
 *  Supported SoCs: TPR12.
 *
 *  Supported Platforms: tpr12_evm.
 *
 */

#include "exp_header_tpr12.h"

extern GPIO_v2_Config GPIO_v2_config;

/**
 *  \brief    This function used to receive data from slave
 *            device.
 *
 *  \param    MIBSPI_Handle  handle [IN]  MIBSPI_Handle
 *
 */
bool BoardDiag_spiReceiveData(MIBSPI_Handle handle, uint8_t count, bool slvAck)
{
    char ch = ' ' ;
    uint32_t failIndex = 0;
    bool status = 0;
    uint8_t rxBuf[BOARD_DIAG_TEST_SIZE] = {0};
    uint8_t txBuf[BOARD_DIAG_TEST_SIZE] = {0};
    MIBSPI_Transaction transaction;

    memset(&transaction, 0, sizeof(transaction));

    transaction.txBuf = &txBuf;
    transaction.rxBuf = &rxBuf;
    transaction.count = count;
    transaction.slaveIndex = 0;

    UART_printf("Press y to receive the data\n");
    UART_scanFmt("%c",&ch);
    if(ch == 'y' || ch == 'Y')
    {
        /* Delay needed to sync with slave prints.
           we can change this delay based on the time
           taken by the prints added in the slave device code */
        BOARD_delay(2000000);
        status = MIBSPI_transfer(handle, &transaction);
        if(status == false)
        {
            UART_printf("SPI Transfer failed..\n");
            return status;
        }
    }
    else
    {
        UART_printf("Exiting from the test\n");
        return status;
    }
    if(slvAck == true)
    {
        if(rxBuf[0] != BOARD_DIAG_SPI_SLAVE_ACK)
        {
            status = false;
            UART_printf("Received wrong acknowledgment!\n\n");
        }
    }
    else
    {
        BoardDiag_genPattern(&txBuf[0],
                         count,
                         BOARD_DIAG_TEST_PATTERN_55);
        /* Data comparison */
        status = BoardDiag_memCompare(&txBuf[0], &rxBuf[0], count,
                    &failIndex);
        if(!(status))
        {
            UART_printf("\nData comparison failed at - %d location\n",
            failIndex);
        }
    }
    return status;
}

/**
 *  \brief    This function used to send data to slave
 *            device.
 *
 *  \param    handle            [IN]     MIBSPI Handler
 *            txBuf             [IN]     Data buffer
 *            count             [IN]     Data count
 *
 */
bool BoardDiag_spiTransferData(MIBSPI_Handle handle, uint8_t *txBuf,
                               uint8_t count)
{
    uint8_t ch = ' ';
    bool status = true;
    MIBSPI_Transaction transaction;

    memset(&transaction, 0, sizeof(transaction));

    transaction.txBuf = txBuf;
    transaction.rxBuf = NULL;
    transaction.count = count;
    transaction.slaveIndex = 0;

    UART_printf("Press y to send the data\n");
    UART_scanFmt("%c",&ch);

    if(ch == 'y' || ch == 'Y')
    {
        /* Delay needed to sync with slave prints.
           we can change this delay based on the time
           taken by the prints added in the slave device code */
        BOARD_delay(2000000);
        status = MIBSPI_transfer(handle, &transaction);
        if(status == false)
        {
            UART_printf("SPI Transfer failed..\n");
        }
    }
    else
    {
        UART_printf("Exiting from the test..\n");
        status = false;
    }
    return status;
}

/**
 *  \brief    The function performs Expansion header verification test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_expansionHeaderTest(void)
{
    MIBSPI_Params     params;
    MIBSPI_Handle     handle;

    enum MibSpi_InstanceId mibspiInstance[BOARD_DIAG_NUM_MIBSPI_INSTANCES] =
                          {MIBSPI_INST_ID_RCSS_SPIA, MIBSPI_INST_ID_RCSS_SPIB};
    uint8_t txBuf[BOARD_DIAG_TEST_SIZE], spiInst, gpioPinNum, gpioPinIndex, ch;

    uint16_t pinMuxOffset[BOARD_DIAG_TOTAL_NUM_GPIO_LINES] = 
    {
        BOARD_DIAG_PAD_CY_OFFSET_ADDRESS, BOARD_DIAG_PAD_CS_OFFSET_ADDRESS,
        BOARD_DIAG_PAD_CZ_OFFSET_ADDRESS, BOARD_DIAG_PAD_CX_OFFSET_ADDRESS
    };
    /* gpio Mux mode for each pin */
    uint8_t pinGpioMuxMode[BOARD_DIAG_TOTAL_NUM_GPIO_LINES] =
    {
        BOARD_DIAG_GPIO_42_MUX_MODE, BOARD_DIAG_GPIO_34_MUX_MODE,
        BOARD_DIAG_GPIO_43_MUX_MODE, BOARD_DIAG_GPIO_35_MUX_MODE
    };

    /* Pin mux for GPIO signal */
    for(gpioPinNum = 0; gpioPinNum < BOARD_DIAG_TOTAL_NUM_GPIO_LINES;
        gpioPinNum++)
    {
        Board_pinMuxSetMode(pinMuxOffset[gpioPinNum],
                            pinGpioMuxMode[gpioPinNum]);
    }

    /* Initialize the SPI */
    MIBSPI_init();

    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);

    /* Disble DMA */
    params.dmaEnable = 0;
    params.dmaHandle = NULL;

    /* Set SPI in master mode */
    params.mode = MIBSPI_MASTER;
    params.dataSize = 8U;
    params.u.masterParams.bitRate = 5000000U;
    /* No need to set the delay of csHold bit is 1 */
    params.csHold = 1;
    params.u.masterParams.numSlaves = 1U;
    params.u.masterParams.t2cDelay  = 0,                   /* t2cDelay 20*/
    params.u.masterParams.c2tDelay  = 0,                   /* c2tDelay 10*/
    params.u.masterParams.wDelay    = 0,                   /* wDelay */
    params.u.masterParams.slaveProf[0].chipSelect = 0U;
    params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM;
    params.u.masterParams.slaveProf[0].dmaReqLine = 0U;

#if !defined(BOARD_DIAG_SPI_A_TEST)
    spiInst = 0;
#else
    spiInst = 1;
#endif
    /* MIBSPIA for HD-connector-1 and MIBSPIB for HD-connector-2 */
    /* Open the SPI Instance for MibSpi */
    handle = MIBSPI_open(mibspiInstance[spiInst], &params);
    if(handle == NULL)
    {
        UART_printf("Failed to open MIBPSI instance - %d\n", spiInst);
        return -1;
    }

    UART_printf("Please run the SPI slave test from the external device and press Y\n");
    UART_scanFmt("%c\n", &ch);

    if(ch == 'y' || ch == 'Y')
    {
        /* Send START command to indicate the start of transaction */
        /* Transfer start Command */
        txBuf[0] = BOARD_DIAG_SPI_MASTER_START_COMMAND;
        UART_printf("Transmitting the start command to slave device..\n");
        if(!(BoardDiag_spiTransferData(handle, &txBuf[0], 1)))
        {
            MIBSPI_close(handle);
            return -1;
        }
        UART_printf("Transmitted the start command to slave device\n");
        UART_printf("Waiting for slave acknowledgment..\n");
        /* Wait till it receives the acknowledgment from slave device */
        if(!(BoardDiag_spiReceiveData(handle, 1, true)))
        {
            MIBSPI_close(handle);
            return -1;
        }
        UART_printf("Received the slave acknowledgment..\n\n");

        UART_printf("Sending %d bytes of data to slave\n", BOARD_DIAG_TEST_SIZE);
        BoardDiag_genPattern(&txBuf[0],
                         BOARD_DIAG_TEST_SIZE,
                         BOARD_DIAG_TEST_PATTERN_AA);
        if(!(BoardDiag_spiTransferData(handle, &txBuf[0], BOARD_DIAG_TEST_SIZE)))
        {
            MIBSPI_close(handle);
            return -1;
        }
        UART_printf("Sent %d bytes of data to slave\n", BOARD_DIAG_TEST_SIZE);

        UART_printf("Waiting to receive the data from Slave..\n");
        /* Wait till it receives the acknowledgment from slave device */
        if(!(BoardDiag_spiReceiveData(handle, BOARD_DIAG_TEST_SIZE, false)))
        {
            MIBSPI_close(handle);
            return -1;
        }
        UART_printf("Received the %d bytes of data from slave..\n\n", BOARD_DIAG_TEST_SIZE);

        UART_printf("Controlling gpio signals from HD connector - %d\n",
                spiInst);

        GPIO_v2_updateConfig(&GPIO_v2_config);
        /* Start toggling gpio control lines */
        GPIO_init();

        for(gpioPinNum = 0; gpioPinNum < BOARD_DIAG_MAX_GPIO_LINES_PER_SPI_INST;
            gpioPinNum++)
        {
            if(spiInst == 0)
            {
                gpioPinIndex = gpioPinNum;
            }
            else
            {
                gpioPinIndex = gpioPinNum +
                                BOARD_DIAG_MAX_GPIO_LINES_PER_SPI_INST;
            }
            UART_printf("Making gpio line - %d as high\n", gpioPinNum);
            GPIO_write(GPIO_v2_config.pinConfigs[gpioPinIndex].pinIndex, BOARD_DIAG_GPIO_PIN_HIGH);

            /* Wait till it receives the acknowledgment from slave device */
            UART_printf("waiting for acknowledgment from the slave\n");

            if(!(BoardDiag_spiReceiveData(handle, 1, true)))
            {
                MIBSPI_close(handle);
                return -1;
            }

            UART_printf("Received acknowledgment from the slave "
                        "successfully\n\n");

            UART_printf("Making gpio line - %d as low\n", gpioPinNum);
            GPIO_write(GPIO_v2_config.pinConfigs[gpioPinIndex].pinIndex, BOARD_DIAG_GPIO_PIN_LOW);

            /* Wait till it receives the acknowledgment from slave device */
            UART_printf("waiting for acknowledgment from the slave\n");

            if(!(BoardDiag_spiReceiveData(handle, 1, true)))
            {
                MIBSPI_close(handle);
                return -1;
            }

            UART_printf("Received acknowledgment from the slave successfully\n\n");
        }
    }
    else
    {
        UART_printf("Exiting from the test..\n");
        MIBSPI_close(handle);
        return -1;
    }
    UART_printf("Expansion header test passed\n");
    MIBSPI_close(handle);
    return 0;
}

/**
 * \brief  main function
 *
 *  This function performs board intializations and calls expansion
 *  header test
 *
 * \return  int
 *              0  - in case of success
 *             -1  - in case of failure
 */
int main(void)
{
    uint32_t status;
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;

    status = Board_init(boardCfg);
    if (status != BOARD_SOK)
    {
        return -1;
    }

    return BoardDiag_expansionHeaderTest();
}
