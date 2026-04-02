 /*
 *  Copyright (c) Texas Instruments Incorporated 2020
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

 /**
 *  \file   mcspiMasterPerformance.c
 *
 *  \brief This file contains the Master application which demonstrates
 *         data transfer in master mode with performance measurment.
 *
 *         McSPI is configured in Tx Only mode with FIFO enabled for Tx.
 *         Word Length set to 8 bits.
 *         Data is transmitted on D0 pin.
 *         Data transmission is in polled mode.
 *         Pad configuration for D1 pin is done to GPIO mode and toggle the pin
 *         different point in transfer so that it can be probed for measurment.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdio.h>
#include <ti/csl/csl_mcspi.h>
#include <ti/csl/csl_gpio.h>
#include <ti/csl/soc.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>
#include "mcspiAppUtils.h"
#include "mcspiAppUtils_soc.h"

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */
#define MCSPI_IN_CLK                (48000000U)

#define McSPI_DATA_COUNT_1          (5U)
#define McSPI_DATA_COUNT_2          (32U)
#define McSPI_DATA_COUNT_3          (256U)
#define McSPI_DATA_COUNT_4          (512U)
#define McSPI_DATA_COUNT_MAX        (1024U)

#define MCSPI_NUM_DATA_LENGTH       (5U)
#define MCSPI_NUM_OUT_CLK_FREQ      (5U)

typedef struct App_timeStampObj_s
{
    uint64_t start;
    uint64_t end;
    uint64_t elapsedTime;
}App_timeStampObj;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
static uint32_t  gMcSPIBaseAddr = MCSPI_BASE_ADDRESS;
static uint32_t  gChNum  = 0U;

static uint8_t   gTxBuffer[McSPI_DATA_COUNT_MAX];
static uint32_t  gDataLength[MCSPI_NUM_DATA_LENGTH] = {McSPI_DATA_COUNT_1,
                                                McSPI_DATA_COUNT_2,
                                                McSPI_DATA_COUNT_3,
                                                McSPI_DATA_COUNT_4,
                                                McSPI_DATA_COUNT_MAX};

static uint32_t gOutClkFreq[MCSPI_NUM_OUT_CLK_FREQ] = {MCSPI_IN_CLK,   /* 48 MHz */
                                                (MCSPI_IN_CLK / 2U),   /* 24 MHz */
                                                (MCSPI_IN_CLK / 4U),   /* 12 MHz */
                                                (MCSPI_IN_CLK / 8U),   /* 6  MHz */
                                                (MCSPI_IN_CLK / 48U)}; /* 1  MHz */

volatile static App_timeStampObj gTimeStampSetup = {0};
volatile static App_timeStampObj gTimeStampTransfer = {0};

volatile uint32_t gChEnableRegVal, gChDisableRegVal;
volatile uint32_t gCsAssertRegVal, gCsDeAssertRegVal;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
static void App_McSPIMasterTest(uint32_t outClk,
                                uint32_t dataLength,
                                volatile App_timeStampObj *setup,
                                volatile App_timeStampObj *transfer);
static void App_McSPISetUp(uint32_t baseAddr,
                           uint32_t chNum,
                           uint32_t wordLength,
                           uint32_t outClk);
static void App_McSPIPolledModeFifoTransfer(uint32_t baseAddr,
                                            uint32_t chNum,
                                            uint8_t *txBuff,
                                            uint32_t length);
static void App_McSPIInitializeBuffers(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void App_McSPIPerformanceTest(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;
    int32_t status;

    /* initialize the board module. */
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_UART_STDIO;
    boardStatus = Board_init(boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        McSPIAppUtilsPrint("[Error] Board init failed!!\n");
    }

    McSPIAppUtilsPrint("\nMaster Performance Application Started\n");

    /* Configure GTC Timer for profiling */
    status = McSPIAppUtilsSetGTCClk(MCSPI_GTC_MOD_ID,
                                 MCSPI_GTC_CLK_ID,
                                 MCSPI_GTC_CLK_FREQ);
    if (CSL_PASS != status)
    {
        McSPIAppUtilsPrint("[Error] Configure GTC Timer failed!!\n");
    }

    /* Configure and perform SPI Tx with different clock frequencies. */
    for (uint32_t i = 0; i < MCSPI_NUM_OUT_CLK_FREQ; i++)
    {
        McSPIAppUtilsPrint("--------------------------------------\n");
        McSPIAppUtilsPrint("SPI Clock %d Hz\n", gOutClkFreq[i]);
        McSPIAppUtilsPrint("--------------------------------------\n");
        McSPIAppUtilsPrint("Num Bytes \tSetup Time (ns)\tTransfer Time (ns)\n");

        for (uint32_t j = 0; j < MCSPI_NUM_DATA_LENGTH; j++)
        {
            App_McSPIMasterTest(gOutClkFreq[i], gDataLength[j], &gTimeStampSetup, &gTimeStampTransfer);

            gTimeStampSetup.elapsedTime = McSPIAppUtilsGetElapsedTime(gTimeStampSetup.start, gTimeStampSetup.end);
            gTimeStampTransfer.elapsedTime = McSPIAppUtilsGetElapsedTime(gTimeStampTransfer.start, gTimeStampTransfer.end);

            McSPIAppUtilsPrint("%d\t\t%d\t\t%d\n", gDataLength[j],
                                    (uint32_t)gTimeStampSetup.elapsedTime,
                                    (uint32_t)gTimeStampTransfer.elapsedTime);
        }
        McSPIAppUtilsPrint("--------------------------------------\n\n");
    }

    McSPIAppUtilsPrint("\nMaster Performance Application Completed\n");
    McSPIAppUtilsPrint("All tests have passed..\n");
}

static void App_McSPIMasterTest(uint32_t outClk,
                                uint32_t dataLength,
                                volatile App_timeStampObj *setup,
                                volatile App_timeStampObj *transfer)
{
    uint32_t chConfRegVal, chCtrlRegVal;
    App_McSPIInitializeBuffers();

    /* Do the necessary set up configurations for McSPI.*/
    setup->start = McSPIGetGTCTimerTicks();
    App_McSPISetUp(gMcSPIBaseAddr, gChNum, MCSPI_WORD_LENGTH(8), outClk);
    setup->end = McSPIGetGTCTimerTicks();

    /*
     * Channel Control and config registers are updated after App_McSPISetUp.
     * Channel enable/disable and CS assert/deassert require updation of bits in
     * control and config registers. Also these registers will not be updated
     * during data transfer. So reg read modify write operations can be updated
     * to write only operations.
     * Store ch enable/disable reg val and cs assert/deassert reg vals.
     */
    chCtrlRegVal = McSPIGetChannelCtrl(gMcSPIBaseAddr, gChNum);
    gChEnableRegVal = chCtrlRegVal | MCSPI_CH0CTRL_EN_MASK;
    gChDisableRegVal = chCtrlRegVal & (~MCSPI_CH0CTRL_EN_MASK);

    chConfRegVal = McSPIGetChannelConf(gMcSPIBaseAddr, gChNum);
    gCsAssertRegVal = chConfRegVal | MCSPI_CH0CONF_FORCE_MASK;
    gCsDeAssertRegVal = chConfRegVal & (~MCSPI_CH0CONF_FORCE_MASK);

    transfer->start = McSPIGetGTCTimerTicks();
    App_McSPIPolledModeFifoTransfer(gMcSPIBaseAddr, gChNum, &gTxBuffer[0], dataLength);
    transfer->end = McSPIGetGTCTimerTicks();
}

/*
 * This function will call the necessary McSPI APIs which will configure the
 * McSPI controller.
 * Currently word length is set to 8 bits, when this is changed to say 32 bit,
 * The writing to transmit register needs to be updated to 32 bit access. Also
 * available FIFO is 64 bytes, so the number of SPI words to be written to FIFO
 * will reduce accordingly.
 */
static void App_McSPISetUp(uint32_t baseAddr,
                           uint32_t chNum,
                           uint32_t wordLength,
                           uint32_t outClk)
{
    /* Reset the McSPI instance.*/
    McSPIReset(baseAddr);

    /* CLOCKACTIVITY - OCP and Functional clocks are maintained             */
    /* SIDLEMODE     - Ignore the idle request and configure in normal mode */
    /* AUTOIDLE      - Disable (OCP clock is running free, no gating)       */
    MCSPISysConfigSetup(baseAddr, MCSPI_CLOCKS_OCP_ON_FUNC_ON,
                        MCSPI_SIDLEMODE_NO, MCSPI_WAKEUP_DISABLE,
                        MCSPI_AUTOIDLE_OFF);

    /* Enable chip select pin (McSPI configured in 4 pin mode). */
    McSPICSEnable(baseAddr);

    /* Enable master mode of operation.*/
    McSPIMasterModeEnable(baseAddr);

    /* master mode configuration. Single channel, Tx only mode with
       transmission and reception enabled on DAT0. */
    McSPIMasterModeConfig(baseAddr, MCSPI_SINGLE_CH,
                                   MCSPI_TX_ONLY_MODE,
                                   MCSPI_DATA_LINE_COMM_MODE_2,
                                   chNum);

    /* Configure the McSPI bus clock depending on clock mode. */
    McSPIClkConfig(baseAddr, MCSPI_IN_CLK, outClk, chNum,
                   MCSPI_CLK_MODE_0);

    /* Configure the word length.*/
    McSPIWordLengthSet(baseAddr, wordLength, chNum);

    /* Set polarity of SPIEN to low.*/
    McSPICSPolarityConfig(baseAddr,
                          (MCSPI_CH0CONF_EPOL_ACTIVELOW <<
                           MCSPI_CH0CONF_EPOL_SHIFT), chNum);

    /* Enable the transmitter FIFO of McSPI peripheral. */
    McSPITxFIFOConfig(baseAddr, MCSPI_TX_FIFO_ENABLE, chNum);

    /* Disable the receiver FIFO of McSPI peripheral for Tx only mode. */
    McSPIRxFIFOConfig(baseAddr, MCSPI_RX_FIFO_DISABLE, chNum);

    /* Set Tx FIFO trigger level to Full FIFO length. */
    McSPIFIFOTrigLvlSet(baseAddr, 0U, MCSPI_FIFO_DEPTH, MCSPI_TX_ONLY_MODE);
}

static void App_McSPIPolledModeFifoTransfer(uint32_t baseAddr,
                                            uint32_t chNum,
                                            uint8_t *txBuff,
                                            uint32_t length)
{
    uint32_t transferLength = MCSPI_FIFO_DEPTH;

    /* Enable the McSPI channel for communication.*/
    /* Updated for write only operation. */
    McSPISetChannelCtrl(baseAddr, chNum, gChEnableRegVal);

    /* SPIEN line is forced to low state.*/
    /* Updated for write only operation. */
    McSPISetChannelConf(baseAddr, chNum, gCsAssertRegVal);

    while (length != 0)
    {
        if (length < MCSPI_FIFO_DEPTH)
        {
            transferLength = length;
        }
        while (0 == (McSPIChannelStatusGet(baseAddr, chNum) & MCSPI_CH0STAT_TXFFE_MASK))
        {
            /* Wait fot Tx FIFO to be empty before writing the data. */
        }
        /* Write the data in Tx FIFO. */
        for (uint16_t i = 0; i< transferLength; i++)
        {
            McSPITransmitData(baseAddr, (uint32_t) (*txBuff++), chNum);
        }
        length -= transferLength;
    }

    while (0 == (McSPIChannelStatusGet(baseAddr, chNum) & MCSPI_CH0STAT_TXFFE_MASK))
    {
        /* Wait fot Tx FIFO to be empty for the last set of data. */
    }
    while (0 == (McSPIChannelStatusGet(baseAddr, chNum) & MCSPI_CH0STAT_EOT_MASK))
    {
        /* Tx FIFO Empty is triggered when last word from FIFO is written to
           internal shift register. SO wait for the end of transfer of last word.
           The EOT gets set after every word when the transfer from shift register is complete
           and is reset when the transmission starts. So FIFO empty check is required
           to make sure the data in FIFO is sent out then wait for EOT for the last word.*/
    }

    /* Force SPIEN line to the inactive state.*/
    /* Updated for write only operation. */
    McSPISetChannelConf(baseAddr, chNum, gCsDeAssertRegVal);

    /* Disable the McSPI channel.*/
    /* Updated for write only operation. */
    McSPISetChannelCtrl(baseAddr, chNum, gChDisableRegVal);
}

static void App_McSPIInitializeBuffers(void)
{
    uint32_t index = 0;

    for (index = 0; index < McSPI_DATA_COUNT_MAX; index++)
    {
        /* Initialize the gTxBuffer McSPI1 with a known pattern of data */
        gTxBuffer[index] = (uint8_t) index + 1U;
    }
}
/********************************* End Of File ******************************/
