/*
 *   @file  mibspi_test_common.c
 *
 *   @brief
 *      Unit Test common code for the MibSpi Driver
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2020 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "MIBSPI_log.h"

#if defined (USE_BIOS)
#ifdef BUILD_MCU1_0
#include <ti/sysbios/family/arm/v7a/Pmu.h>
#endif
#endif

#include <ti/csl/soc.h>

#include <ti/drv/mibspi/MIBSPI.h>
#include <ti/drv/edma/edma.h>
#include <ti/osal/HwiP.h>

#include "mibspi_test_common.h"

/**************************************************************************
 *************************** Local Definitions *********************************
 **************************************************************************/

/* Test flag for multiple icount */
#define SPI_MULT_ICOUNT_SUPPORT

/* VBUSP Frequency in MHz */
#define VBUSP_FREQ              MSS_SYS_VCLK /1000000U

/* Number of Loops of Data echo test when XWR1xxx is configured as Master */
#if defined (SIM_BUILD)
#define SPI_TEST_LOOP           2
#else
#define SPI_TEST_LOOP           1000
#endif

#ifdef SPI_MULT_ICOUNT_SUPPORT
 /* Block size used for the test*/
#define SPI_DATA_BLOCK_SIZE     1024
#else
 /* Block size used for the test*/
#define SPI_DATA_BLOCK_SIZE     128
#endif

/* Test message definitio for SPI test as a slave with external devices */
#define MAGIC_NUMBER            0x3456

#ifdef SPI_MULT_ICOUNT_SUPPORT
#define SPI_TEST_MSGLEN         1024
#else
#define SPI_TEST_MSGLEN         128
#endif
#define SPI_TEST_SYNC_MSGLEN    16
#define SWAP_BYTES(x)           (( (x&0xff) << 8) | ((x >>8) & 0xff))

/**************************************************************************
 *************************** Local Data Structure ******************************
 **************************************************************************/

/* SPI Test message definition */
typedef struct spiTestMsg
{
    /* Test message MAGIC number */
    uint16_t magicNumber;

    /* Test message sequence number */
    uint16_t seqNo;

    /* Test message valid data length */
    uint16_t dataLen;

    /* Test message checksum */
    uint16_t checksum;

    /* Test message data payload */
    uint8_t data[SPI_TEST_MSGLEN - 8];
}spiTestMsg;

/**************************************************************************
 *************************** Global Definitions ********************************
 **************************************************************************/
 /* Global data buffers used for SPI test */
volatile uint8_t    txBuf[SPI_DATA_BLOCK_SIZE] __attribute__ ((aligned(128)));

volatile uint8_t    rxBuf[SPI_DATA_BLOCK_SIZE] __attribute__ ((aligned(128)));

/* Debug flag */
uint32_t            gPrintHwStats = 1;

/**************************************************************************
 *************************** SPI Test Functions *******************************
 **************************************************************************/
static int32_t Test_spiLoopbackSlave(const MIBSPI_Handle handle, uint32_t maxElem, uint8_t dataSize);

/**
 *  @b Description
 *  @n
 *      This function delay by running in tight loops.
 *
 *   @param[in] count             count for the delay loop
 *
 *  @retval
 *      Not Applicable.
 */
void Test_delay(uint32_t count)
{
    volatile uint32_t i;

    for(i=0; i<count; i++);
}

/**
 *  @b Description
 *  @n
 *      This function starts a PMU counter .
 *
 *   @param[in] counter             Counter id used for benchmarking
 *
 *  @retval
 *      Not Applicable.
 */
void Test_benchmarkStart(uint32_t counter)
{
#if (defined (USE_BIOS) && (BUILD_MCU1_0))
    /* Initialize counter to count cycles */
    Pmu_configureCounter(counter, 0x11, FALSE);

    /* Reset PMU counter */
    Pmu_resetCount(counter);

    /* Start PMU counter */
    Pmu_startCounter(counter);
#endif
}

/**
 *  @b Description
 *  @n
 *      This function stops a PMU counter and returns the current counter value.
 *
 *   @param[in] counter             Counter id used for benchmarking
 *
 *  @retval
 *      Current PMU counter value.
 */
uint32_t Test_benchmarkStop(uint32_t counter)
{
#if (defined (USE_BIOS) && (BUILD_MCU1_0))
    /* Stop PMU counter */
    Pmu_stopCounter(counter);

    /* Read PMU counter */
    return (Pmu_getCount(counter));
#else
    return (0); //TODO
#endif
}

/**
 *  @b Description
 *  @n
 *      SPI get hardware statistics function.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] printStats        Flag to print the stats
 *
 *   @retval    Successful        =0
 *                 Failed         <0
 */
static int32_t Test_getSpiStats(const MIBSPI_Handle handle, bool printStats)
{
    MIBSPI_Stats   stats;
    int32_t     retVal;

    retVal = MIBSPI_getStats(handle, &stats);
    if(retVal == 0)
    {
        if(printStats)
        {
            MIBSPI_log("SPI hardware stats:\n");
            MIBSPI_log("datalen Error    = %d\n", stats.dlenErr);
            MIBSPI_log("timeout          = %d\n", stats.timeout);
            MIBSPI_log("Parity Error     = %d\n", stats.parErr);
            MIBSPI_log("Desync error     = %d\n", stats.desync);
            MIBSPI_log("Tx bit error     = %d\n", stats.bitErr);
            MIBSPI_log("RX overrun       = %d\n", stats.rxOvrnErr);
        }
    }

    return retVal;
}

/**
 *  @b Description
 *  @n
 *      SPI read only function.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] dataLen           Number of bytes need to be transferred
 *   @param[in] buffer            Pointer of the buffer
 *   @param[in] slaveIndex        Slave index for the SPI transfer
 *
 *  @retval    Successful         =0
 *                 Failed         <0
 */
static int32_t Test_spiRead(const MIBSPI_Handle handle, uint32_t dataLen, void* buffer, uint8_t slaveIndex)
{
    MIBSPI_Transaction transaction;

    /* Configure Data Transfer */
    transaction.count = dataLen;
    transaction.txBuf = NULL;
    transaction.rxBuf = buffer;
    transaction.slaveIndex = slaveIndex;

    /* Start Data Transfer */
    if (MIBSPI_transfer(handle, &transaction) != true)
    {
        return -1;
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      SPI write only function.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] dataLen           Number of bytes need to be transferred
 *   @param[in] buffer            Pointer of the buffer
 *   @param[in] slaveIndex        Slave index for the SPI transfer
 *
 *  @retval    Successful         =0
 *                 Failed         <0
 */
static int32_t Test_spiWrite(const MIBSPI_Handle handle, uint32_t dataLen, void* buffer, uint8_t slaveIndex)
{
    MIBSPI_Transaction transaction;

    /* Configure Data Transfer */
    transaction.count = dataLen;
    transaction.txBuf = buffer;
    transaction.rxBuf = NULL;
    transaction.slaveIndex = slaveIndex;

    /* Start Data Transfer */
    if (MIBSPI_transfer(handle, &transaction) != true)
    {
        return -1;
    }
    return 0;
}

/* Test_spiReadWrite is called for the callback mode test also.
 * So keep the MIBSPI_Transaction object persistant till the transfer is complete.
 * So use the global variable insted of using the local variable
 * which is initialized in stack.
 */
static MIBSPI_Transaction  gTransactionRdWr;

/**
 *  @b Description
 *  @n
 *      SPI read and write function.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] dataLen           Number of bytes need to be transferred
 *   @param[in] inBuffer          Pointer of the read buffer
 *   @param[in] outBuffer         Pointer of the write buffer
 *   @param[in] slaveIndex        Slave index for the SPI transfer
 *
 *  @retval    Successful         =0
 *                 Failed         <0
 */
static int32_t Test_spiReadWrite(const MIBSPI_Handle handle, uint32_t dataLen, void* inBuffer, void* outBuffer, uint8_t slaveIndex)
{
    /* Configure Data Transfer */
    gTransactionRdWr.count = dataLen;
    gTransactionRdWr.txBuf = outBuffer;
    gTransactionRdWr.rxBuf = inBuffer;
    gTransactionRdWr.slaveIndex = slaveIndex;

    /* Start Data Transfer */
    if (MIBSPI_transfer(handle, &gTransactionRdWr) != true)
    {
        return -1;
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver in Digital Loopback mode.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] slaveIndex        Flag for internal/external loopback
 *   @param[in] maxElem           Maxim data element
 *   @param[in] dataSize          Data size in number of bytes
 *
 *  @retval    Successful                   =0
 *             Number of transfer failures  >0
 *             API failures                 <0
 */
static int32_t Test_spiLoopback(const MIBSPI_Handle handle, uint8_t slaveIndex, uint32_t maxElem, uint8_t dataSize)
{
    MibSpi_LoopBackType            loopback;
    uint32_t           loop;
    uint32_t           idx;
    uint32_t           failed = 0;
    uint32_t           len=0;

    /* Only dataSize of 1 byte or 2 bytes are supported */
    if ((dataSize != (uint8_t)1U) && (dataSize != (uint8_t)2U))
        return -1;

    /* Enable digital loopback */
    loopback = MIBSPI_LOOPBK_DIGITAL;
    if(MIBSPI_control(handle, MIBSPI_CMD_LOOPBACK_ENABLE, (void *)&loopback) < 0)
        return -1;
    for(loop=0; loop < maxElem; loop++)
    {
        len = (maxElem - loop) * dataSize;

        /* Prepare Tx/Rx Buffer */
        for(idx=0; idx<maxElem * dataSize; idx++)
        {
            txBuf[idx] = (loop * 0x10 + 0x55 + idx) & 0xFF;
        }

        /* Clear receive buffer */
        memset((void *)&rxBuf[0], 0x0, SPI_DATA_BLOCK_SIZE);

        CacheP_wbInv((Ptr)txBuf, sizeof(txBuf));
        CacheP_wbInv((Ptr)rxBuf, sizeof(rxBuf));
        if(Test_spiReadWrite(handle, len, (void *)rxBuf, (void *)txBuf, slaveIndex) == 0)
        {
            /* Check data integrity */
            if (memcmp((void *)txBuf, (void *)rxBuf, len) != 0)
            {
                MIBSPI_log("Error: MIBSPI_transfer is successful with incorrect data(0x%x), length = %d\n", rxBuf[0], len);
                failed++;
            }
        }
        else
        {
            MIBSPI_log("Debug: MIBSPI_transfer failed for length = %d\n", len);
            failed++;
        }
    }
    MIBSPI_log("Debug: Finished Digital loopback with various length test,  failed %d out of %d times\n", failed, loop);

    /* Disable digital loopback */
    loopback = MIBSPI_LOOPBK_NONE;
    if(MIBSPI_control(handle, MIBSPI_CMD_LOOPBACK_ENABLE, (void *)&loopback) < 0)
        return -1;

    /* Return number of failures */
    return failed;
}

#ifdef MIBSPI_DMA_ENABLE
volatile uint32_t gTestCallbackDone = 0;

void Test_loopback_callbackFxn (MIBSPI_Handle handle,
                                MIBSPI_Transaction * transaction)
{
    gTestCallbackDone = 1;
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver in Digital Loopback mode.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] slaveIndex        Flag for internal/external loopback
 *   @param[in] maxElem           Maxim data element
 *   @param[in] dataSize          Data size in number of bytes
 *
 *  @retval    Successful                   =0
 *             Number of transfer failures  >0
 *             API failures                 <0
 */
static int32_t Test_spiLoopback_callback(const MIBSPI_Handle handle, uint8_t slaveIndex, uint32_t maxElem, uint8_t dataSize)
{
    MibSpi_LoopBackType            loopback;
    uint32_t           loop;
    uint32_t           idx;
    uint32_t           failed = 0;
    uint32_t           len=0;

    /* Only dataSize of 1 byte or 2 bytes are supported */
    if ((dataSize != (uint8_t)1U) && (dataSize != (uint8_t)2U))
        return -1;

    /* Enable digital loopback */
    loopback = MIBSPI_LOOPBK_DIGITAL;
    if(MIBSPI_control(handle, MIBSPI_CMD_LOOPBACK_ENABLE, (void *)&loopback) < 0)
        return -1;
    for(loop=0; loop < maxElem; loop++)
    {
        len = (maxElem - loop) * dataSize;

        /* Prepare Tx/Rx Buffer */
        for(idx=0; idx<maxElem * dataSize; idx++)
        {
            txBuf[idx] = (loop * 0x10 + 0x55 + idx) & 0xFF;
        }

        /* Clear receive buffer */
        memset((void *)&rxBuf[0], 0x0, SPI_DATA_BLOCK_SIZE);

        CacheP_wbInv((Ptr)txBuf, sizeof(txBuf));
        CacheP_wbInv((Ptr)rxBuf, sizeof(rxBuf));
        gTestCallbackDone = 0;
        if(Test_spiReadWrite(handle, len, (void *)rxBuf, (void *)txBuf, slaveIndex) == 0)
        {
            /* Wait for the callback function. */
            while (gTestCallbackDone == 0)
            {
                /* waiting for the callback funciton to be called. */
                /* Need Nop so that interrupts get enabled atleast for a few cycles in C66x case */
                asm(" nop ");
                asm(" nop ");
                asm(" nop ");
                asm(" nop ");
                asm(" nop ");
                asm(" nop ");
                asm(" nop ");
                asm(" nop ");
            }
            /* Check data integrity */
            if (memcmp((void *)txBuf, (void *)rxBuf, len) != 0)
            {
                MIBSPI_log("Error: MIBSPI_transfer is successful with incorrect data(0x%x), length = %d\n", rxBuf[0], len);
                failed++;
            }
        }
        else
        {
            MIBSPI_log("Debug: MIBSPI_transfer failed for length = %d\n", len);
            failed++;
        }
    }
    MIBSPI_log("Debug: Finished Digital loopback with various length test,  failed %d out of %d times\n", failed, loop);

    /* Disable digital loopback */
    loopback = MIBSPI_LOOPBK_NONE;
    if(MIBSPI_control(handle, MIBSPI_CMD_LOOPBACK_ENABLE, (void *)&loopback) < 0)
        return -1;

    /* Return number of failures */
    return failed;
}
#endif

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver throughput in Loopback mode.
 *      Test is performed with following configurations:
 *       a. 16bits data size
 *       b. one slave
 *       c. Maximum data buffer
 *
 *   @param[in] inst             SPI instance (0=MibSPIA, 1=MibSPIB)
 *   @param[in] bitRate          SPI clock bit rate
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_spiLoopBackDataThroughput(uint32_t inst, uint32_t bitRate)
{
    MIBSPI_Params       params;
    MIBSPI_Handle       handle;
    MIBSPI_Transaction  transaction;
    MibSpi_LoopBackType loopback;
    uint32_t         failed = 0;
    uint32_t         idx=0;
    uint32_t         loop;
    volatile uint32_t  cycles;
    float            throughput = 0;

    /**************************************************************************
     * Test: SPI Open
     **************************************************************************/
    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);
    params.frameFormat = MIBSPI_POL0_PHA0;

    /* Enable DMA and set DMA channels to be used */
#ifdef MIBSPI_DMA_ENABLE
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle[inst];
#else
    params.dmaEnable = 0;
    params.dmaHandle = NULL;
#endif
    params.eccEnable = 0;

    params.mode = MIBSPI_MASTER;
    params.u.masterParams.bitRate = bitRate;

    params.u.masterParams.numSlaves = 1U;
    params.u.masterParams.slaveProf[0].chipSelect = 0U;
    params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM;
    params.u.masterParams.slaveProf[0].dmaReqLine = 0U;

    handle = MIBSPI_open(gMibspiInst[inst], &params);
    if (handle == NULL)
    {
        MIBSPI_log("Error: Unable to open the SPI Instance\n");

        return;
    }

    /* Set SPI Digital loopback mode */
    loopback = MIBSPI_LOOPBK_DIGITAL;
    if(MIBSPI_control(handle, MIBSPI_CMD_LOOPBACK_ENABLE, (void *)&loopback) < 0)
    {
        return;
    }

    /* Prepare data in TX Buffer */
    for(idx=0; idx<SPI_DATA_BLOCK_SIZE; idx++)
    {
        txBuf[idx] =  (0x55 + idx) & 0xFF;
    }

    memset((void *)&rxBuf[0], 0x0, SPI_DATA_BLOCK_SIZE);

    CacheP_wbInv((Ptr)txBuf, sizeof(txBuf));
    CacheP_wbInv((Ptr)rxBuf, sizeof(rxBuf));

    // Start the counter
    Test_benchmarkStart(0);

    for(loop=0; loop < 10; loop++)
    {
        /* Configure Data Transfer */
        transaction.count = SPI_DATA_BLOCK_SIZE;
        transaction.txBuf = (void *)txBuf;
        transaction.rxBuf = (void *)rxBuf;
        transaction.slaveIndex = 0U;

        /* Start Data Transfer */
        if (MIBSPI_transfer(handle, &transaction) != true)
        {
            MIBSPI_log("Debug: MIBSPI_transfer failed for , loop=%d\n", loop);
            failed++;
        }
    }

    // Stop the counter
    cycles = Test_benchmarkStop(0);
    throughput = 8.0 * SPI_DATA_BLOCK_SIZE * loop  * VBUSP_FREQ / cycles;

    MIBSPI_log("Debug: Finished Digital loopback througput test,  failed %d out of %d times\n", failed, loop);

    /* Disable SPI Digital loopback mode */
    loopback = MIBSPI_LOOPBK_NONE;
    if (MIBSPI_control(handle, MIBSPI_CMD_LOOPBACK_ENABLE, (void *)&loopback) < 0)
    {
        return;
    }

    /* Check failures and return results */
    if(failed == 0)
    {
        MIBSPI_log("SPI throughput at bitRate %d Kbps : %f Mbps", bitRate/1000, throughput);
    }
    else
    {

    }

    /* Print hardware error stats */
    Test_getSpiStats(handle, gPrintHwStats);

    /* Close the driver: */
    MIBSPI_close(handle);
}

/**
 *  @b Description
 *  @n
 *       This function generates the test pattern for read/write test.
 *
 *   @param[in] buffer           Buffer pointer for the generated data
 *   @param[in] dataLen          Buffer len (in bytes) for pattern generation
 *
 *  @retval    None
 */
static void Test_generateTestPattern(volatile uint8_t * buffer, uint32_t dataLen)
{
    uint32_t               index;
    volatile uint8_t       *ptrBuff;

    ptrBuff =  buffer;
    for(index=0; index<dataLen; index++)
    {
        *ptrBuff = (index+0x55) & 0xFF;
        ptrBuff++;
    }
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver in SPI master mode.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] slaveIndex        Slave index for the SPI transfer
 *
 *  @retval
 *      Not Applicable.
 */
void Test_spiMasterWrite(const MIBSPI_Handle handle, uint8_t slaveIndex)
{
    uint16_t    loop;
    uint32_t    cycles;
    uint32_t    delayCycles;
    float       throughput;
    uint32_t    delay = 8U * SPI_TEST_MSGLEN;

    /* Initialize txBuf */
    Test_generateTestPattern(txBuf, SPI_TEST_MSGLEN);

    Test_benchmarkStart(0);
    Test_benchmarkStart(1);

    for (loop = 0; loop < SPI_TEST_LOOP; loop++)
    {
        /* Write data to slave */
        txBuf[0] = loop;
        Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, slaveIndex);

#if (defined (USE_BIOS) && (BUILD_MCU1_0))
        Pmu_startCounter(1);
#endif
        Test_delay(delay);
#if (defined (USE_BIOS) && (BUILD_MCU1_0))
        Pmu_stopCounter(1);
#endif
    }

    cycles = Test_benchmarkStop(0);
#if (defined (USE_BIOS) && (BUILD_MCU1_0))
    delayCycles = Pmu_getCount(1);
#else
    delayCycles = 1000; //TODO
#endif

    throughput = 8.0 * SPI_TEST_MSGLEN * loop  * VBUSP_FREQ / (cycles - delayCycles);

    // Read and print the count
    MIBSPI_log("Master write Throughput = %.2f Mbps\n", throughput);
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver master mode Read data from MSP  SPI slave.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] slaveIndex        Slave index for the SPI transfer
 *
 *  @retval
 *      Not Applicable.
 */
void Test_spiMasterRead(const MIBSPI_Handle handle, uint8_t slaveIndex)
{
    uint16_t    loop;
    uint32_t    cycles;
    uint32_t    delayCycles;
    float       throughput;
    uint32_t    delay = 8U * SPI_TEST_MSGLEN;

    Test_benchmarkStart(0);
    Test_benchmarkStart(1);

    for (loop=0; loop<SPI_TEST_LOOP; loop++)
    {
        /* Read data from slave */
        Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)rxBuf, slaveIndex);
#if (defined (USE_BIOS) && (BUILD_MCU1_0))
        Pmu_startCounter(1);
#endif
        Test_delay(delay);
#if (defined (USE_BIOS) && (BUILD_MCU1_0))
        Pmu_stopCounter(1);
#endif
        rxBuf[0] = loop;
    }
    cycles = Test_benchmarkStop(0);
#if (defined (USE_BIOS) && (BUILD_MCU1_0))
    delayCycles = Pmu_getCount(1);
#else
    delayCycles = 1000;
#endif

    throughput = 8.0 * SPI_TEST_MSGLEN * loop  * VBUSP_FREQ / (cycles -delayCycles) ;

    // Read and print the count
    MIBSPI_log("Debug: Master read Throughput = %.2f Mbps\n", throughput);

}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver in slave  mode
 *
 *   @param[in] handle        SPI driver handle
 *
 *  @retval
 *      Not Applicable.
 */
void Test_spiSlaveRead(const MIBSPI_Handle handle)
{
    uint16_t    loop;

    for (loop=0; loop<SPI_TEST_LOOP; loop++)
    {
        /* Read two byte at a time */
        {
            Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)rxBuf, 0U);
        }
    }
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver in slave mode.
 *
 *   @param[in] handle      SPI driver handle
 *
 *  @retval
 *      Not Applicable.
 */
void Test_spiSlaveWrite(const MIBSPI_Handle handle)
{
    uint16_t    loop;

    Test_generateTestPattern(txBuf, SPI_TEST_MSGLEN);

    for (loop=0; loop<SPI_TEST_LOOP; loop++)
    {
        txBuf[0]= loop;

        Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, 0U);
    }
}



/**
 *  @b Description
 *  @n
 *      SPI API test for both instance - MibSPIA & MibSPIB.
 *
 *  @retval
 *      Not Applicable.
 */
void Test_spiAPI_twoInstance(void)
{
    MIBSPI_Params     params;
    MIBSPI_Handle     handleA = NULL;
    MIBSPI_Handle     handleB = NULL;
    uint32_t       instA = 0;
    uint32_t       instB = 1;
    char           testCase[64];

    /**************************************************************************
     * Test: Verify SPI open/close API
     **************************************************************************/
    snprintf(testCase, 64, "MIBSPI_open API test for two instances");

    /* Setup the default SPIA Parameters */
    MIBSPI_Params_init(&params);

#ifdef MIBSPI_DMA_ENABLE
    /* Enable DMA and set DMA channels */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle[instA];
#else
    params.dmaEnable = 0;
    params.dmaHandle = NULL;
#endif

    params.mode = MIBSPI_SLAVE;
    params.u.slaveParams.dmaReqLine = 0;

    /* Open the SPI Instance for MibSpiA */
    handleA = MIBSPI_open(gMibspiInst[instA], &params);
    if (handleA == NULL)
    {
        MIBSPI_log("Error: Unable to open the SPI Instance\n");
        goto exit;
    }
    MIBSPI_log("Debug: SPI Instance(%d) %p has been opened successfully\n", instA, handleA);

    /* Setup the default SPIB Parameters */
    MIBSPI_Params_init(&params);

#ifdef MIBSPI_DMA_ENABLE
    /* Enable DMA and set DMA channels */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle[instB];
#else
    params.dmaEnable = 0;
    params.dmaHandle = NULL;
#endif

    params.mode = MIBSPI_SLAVE;
    params.u.slaveParams.dmaReqLine = 0;

    /* Open the SPI Instance for MibSpiB */
    handleB = MIBSPI_open(gMibspiInst[instB], &params);
    if (handleB == NULL)
    {
        MIBSPI_log("Error: Unable to open the SPI Instance\n");
        goto exit;
    }
    MIBSPI_log("Debug: SPI Instance(1) %p has been opened successfully\n", handleB);

    /* Close SPIA */
    MIBSPI_close(handleA);
    MIBSPI_log("Debug: SPI Instance(0) %p has been closed successfully\n", handleA);
    handleA= NULL;

    /* Close SPIB */
    MIBSPI_close(handleB);
    MIBSPI_log("Debug: SPI Instance(1) %p has been closed successfully\n", handleB);
    handleB = NULL;

    /**************************************************************************
     * Test: Verify SPI API parameters for DMA channel configuration
     **************************************************************************/
    snprintf(testCase, 64, "MIBSPI_open API test for two instances: DMA channel validation");

    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);

#ifdef MIBSPI_DMA_ENABLE
    /* Enable DMA and set DMA channels */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle[instA];
#else
    params.dmaEnable = 0;
    params.dmaHandle = NULL;
#endif

    params.mode = MIBSPI_SLAVE;
    params.u.slaveParams.dmaReqLine = 0U;


    /* Open the SPI Instance for MibSpiA */
    handleA = MIBSPI_open(gMibspiInst[instA], &params);
    if (handleA != NULL)
    {
        MIBSPI_log("Debug: SPI Instance(0) %p has been opened successfully\n", handleA);
    }
    else
    {
        MIBSPI_log("Error: Failed to open SPI Instance(0)\n");
        goto exit;
    }

exit:
    if(handleA != NULL)
    {
        MIBSPI_close(handleA);
        MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handleA);
    }

    if(handleB != NULL)
    {
        MIBSPI_close(handleB);
        MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handleB);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      SPI API test for one instance.
 *
 *   @param[in] inst               SPI instance: 0-SPIA, 1-SPIB
 *
 *  @retval
 *      Not Applicable.
 */
void Test_spiAPI_oneInstance(uint8_t inst)
{
    MIBSPI_Params     params;
    MIBSPI_Handle     handle;
    char           testCase[64];

    /**************************************************************************
     * Test: Verify SPI open/close API
     **************************************************************************/
    snprintf(testCase, 64, "MIBSPI_open API test (instance=%d)", inst);

    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);

    params.mode = MIBSPI_SLAVE;

    /* Enable DMA and set DMA channels */
#ifdef MIBSPI_DMA_ENABLE
    params.dmaEnable = (uint8_t)1U;
    params.dmaHandle = gDmaHandle[inst];
#else
    params.dmaEnable = 0;
    params.dmaHandle = NULL;
#endif

    params.u.slaveParams.dmaReqLine = 0U;

    /* Open the SPI Instance for MibSpi */
    handle = MIBSPI_open(gMibspiInst[inst], &params);
    if (handle == NULL)
    {
        MIBSPI_log("Error: Unable to open the SPI Instance\n");
        return;
    }
    MIBSPI_log("Debug: SPI Instance %p has been opened successfully\n", handle);

    /* Close SPI */
    MIBSPI_close(handle);
    MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handle);

    /**************************************************************************
     * Test: Verify SPI parameters check::chipSelect
     **************************************************************************/
    snprintf(testCase, 64, "MIBSPI_open API test - chip select validation (instance=%d)", inst);

    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);

    /* Enable DMA and set DMA channels */
#ifdef MIBSPI_DMA_ENABLE
    params.dmaEnable = (uint8_t)1U;
    params.dmaHandle = gDmaHandle[inst];
#else
    params.dmaEnable = 0;
    params.dmaHandle = NULL;
#endif
    params.u.slaveParams.dmaReqLine = 0U;


    params.mode = MIBSPI_SLAVE;

    /* Incorrect chip select */
    params.u.slaveParams.chipSelect = (uint8_t)10U;

    /* Open the SPI Instance for MibSpiA */
    handle = MIBSPI_open(gMibspiInst[inst], &params);
    if (handle == NULL)
    {
    }
    else
    {
        /* Graceful shutdown */
        MIBSPI_close(handle);
        MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handle);
    }

    /**************************************************************************
     * Test: Verify SPI open API for parameters check
     **************************************************************************/
    snprintf(testCase, 64, "MIBSPI_open API test - data size validation (instance=%d)", inst);

    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);

    /* Enable DMA and set DMA channels to be used */
#ifdef MIBSPI_DMA_ENABLE
    params.dmaEnable = (uint8_t)1U;
    params.dmaHandle = gDmaHandle[inst];
#else
    params.dmaEnable = 0;
    params.dmaHandle = NULL;
#endif
    params.u.slaveParams.dmaReqLine  = 0U;


    params.mode = MIBSPI_SLAVE;

    /* Incorrect data Size */
    params.dataSize = 7U;

    /* Open the SPI Instance */
    handle = MIBSPI_open(gMibspiInst[inst], &params);
    if (handle == NULL)
    {

    }
    else
    {

        /* Graceful shutdown */
        MIBSPI_close(handle);
        MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handle);
    }

    /**************************************************************************
     * Test: Verify SPI parameters check::master clock
     **************************************************************************/
    snprintf(testCase, 64, "MIBSPI_open API test - bit rate validation (instance=%d)", inst);

    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);

    /* Enable DMA and set DMA channels */
#ifdef MIBSPI_DMA_ENABLE
    params.dmaEnable = (uint8_t)1U;
    params.dmaHandle = gDmaHandle[0];
#else
    params.dmaEnable = 0;
    params.dmaHandle = NULL;
#endif

    params.u.slaveParams.dmaReqLine  =0U;

    params.mode = MIBSPI_MASTER;
    params.u.masterParams.bitRate = 0;

    /* Open the SPI Instance for MibSpiA */
    handle = MIBSPI_open(gMibspiInst[0], &params);
    if (handle == NULL)
    {

    }
    else
    {


        /* Graceful shutdown */
        MIBSPI_close(handle);
        MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handle);
    }

    /**************************************************************************
     * Test: Verify SPI transfer API for parameters check
     **************************************************************************/

    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);

    /* Enable DMA and set DMA channels to be used */
#ifdef MIBSPI_DMA_ENABLE
    params.dmaEnable = (uint8_t)1U;
    params.dmaHandle = gDmaHandle[inst];
#else
    params.dmaEnable = 0;
    params.dmaHandle = NULL;
#endif

    params.dataSize = 16U;

    params.mode = MIBSPI_SLAVE;
    params.u.slaveParams.dmaReqLine  = 0U;

    MIBSPI_log("Debug: Open the SPI Instance for SPI transfer parameter check test\n");

    /* Open the SPI Instance for MibSpi */
    handle = MIBSPI_open(gMibspiInst[inst], &params);
    if (handle == NULL)
    {
        MIBSPI_log("Error: Unable to open the SPI Instance\n");
    }
    else
    {
        MIBSPI_Transaction transaction;
        bool          ret;

        /* Configure Data Transfer */
        transaction.count = (uint16_t)1U;
        transaction.txBuf = (void *)txBuf;
        transaction.rxBuf = (void *)rxBuf;
        transaction.slaveIndex = (uint16_t)0U;

        snprintf(testCase, 64, "MIBSPI_transfer API test - data size=%d validation (instance=%d)", transaction.count, inst);

        /* Start Data Transfer */
        ret = MIBSPI_transfer(handle, &transaction);
        if (ret != true)
        {
            MIBSPI_log("Debug: MIBSPI_transfer with data size = %d failed with error=%d.\n", transaction.count, transaction.status );
        }
        else
        {
            MIBSPI_log("Error: MIBSPI_transfer with data size = %d should fail.\n", transaction.count);
        }

        /* Configure Data Transfer */
        transaction.count = (uint16_t)11U;
        transaction.txBuf = (void *)txBuf;
        transaction.rxBuf = (void *)rxBuf;
        transaction.slaveIndex = (uint16_t)0U;

        snprintf(testCase, 64, "MIBSPI_transfer API test - data size=%d validation (instance=%d)",transaction.count, inst);

        /* Start Data Transfer */
        ret = MIBSPI_transfer(handle, &transaction);
        if (ret != true)
        {
            MIBSPI_log("Debug: MIBSPI_transfer with data size = %d failed with error=%d. \n", transaction.count, transaction.status);
        }
        else
        {
            MIBSPI_log("Error: MIBSPI_transfer with data size = %d should fail.\n", transaction.count);
        }

        /* Configure Data Transfer */
        transaction.count = (uint16_t)10U;
        transaction.txBuf = (void *)NULL;
        transaction.rxBuf = (void *)NULL;
        transaction.slaveIndex = (uint16_t)0U;

        snprintf(testCase, 64, "MIBSPI_transfer API test - buffer address validation (instance=%d)", inst);

        /* Start Data Transfer */
        ret = MIBSPI_transfer(handle, &transaction);
        if (ret != true)
        {
            MIBSPI_log("Debug: MIBSPI_transfer failed with NULL pointers for both TX and RX with error=%d. \n", transaction.status);
        }
        else
        {
            MIBSPI_log("Error: MIBSPI_transfer with NULL pointers for both TX and RX should fail.\n");
        }

        /* Graceful shutdown */
        MIBSPI_close(handle);
        MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handle);
    }
}

/**
 *  @b Description
 *  @n
 *      SPI loopback test.
 *
 *   @param[in] inst               SPI instance: 0-SPIA, 1-SPIB
 *
 *  @retval
 *      Not Applicable.
 */
void Test_loopback_oneInstance(uint32_t inst, uint8_t slaveIndex)
{
    MIBSPI_Params     params;
    MIBSPI_Handle     handle;
    char testCase[64];

    snprintf(testCase, 64, "SPI loopback test - instance(%d), 16bits DMA mode", inst);

    /**************************************************************************
     * Test: Open the driver in master mode for loopback test
     **************************************************************************/
    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);
    params.frameFormat = MIBSPI_POL0_PHA0;

    /* Enable DMA and set DMA channels to be used */
#ifdef MIBSPI_DMA_ENABLE
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle[inst];
#else
    params.dmaEnable = 0;
    params.dmaHandle = NULL;
#endif

    params.eccEnable = 0;
    params.mode = MIBSPI_MASTER;
    params.u.masterParams.bitRate = 1000000U;

    /* mibSPIA support only one slave */
    if(inst == 0)
    {
        params.u.masterParams.numSlaves = 1;
        params.u.masterParams.slaveProf[0].chipSelect = 0;
        params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM;
        params.u.masterParams.slaveProf[0].dmaReqLine = 0U;
    }
    else if(inst == 1)
    {
        /* The total element size of 3 slaves is MIBSPI_RAM_MAX_ELEM
         * In this example, it is distributed among 3 slaves by MIBSPI_RAM_MAX_ELEM - 6U, 4U, and 2U
         */
        memset((void *)&params.u.masterParams.slaveProf[0], 0, sizeof(params.u.masterParams.slaveProf));

        params.u.masterParams.numSlaves = 3;
        params.u.masterParams.slaveProf[0].chipSelect = 0;
        params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM/2;
        params.u.masterParams.slaveProf[0].dmaReqLine = 0U;
        params.u.masterParams.slaveProf[1].chipSelect = 1;
        params.u.masterParams.slaveProf[1].ramBufLen = MIBSPI_RAM_MAX_ELEM/4;
        params.u.masterParams.slaveProf[1].dmaReqLine = 1U;
        params.u.masterParams.slaveProf[2].chipSelect = 2;
        params.u.masterParams.slaveProf[2].ramBufLen = MIBSPI_RAM_MAX_ELEM/4;
        params.u.masterParams.slaveProf[2].dmaReqLine = 2U;
    }
    else
    {
        printf("Error: Invalid instance(=%d) of MibSPI\n", inst);
    }

    handle = MIBSPI_open(gMibspiInst[inst], &params);
    if (handle == NULL)
    {
        MIBSPI_log("Error: Unable to open the SPI Instance\n");
        return;
    }
    MIBSPI_log("Debug: SPI Instance %p has been reopened in master mode successfully\n", handle);

    /* Start Internal Loopback Test in master mode */
    if(Test_spiLoopback(handle, slaveIndex, params.u.masterParams.slaveProf[slaveIndex].ramBufLen, 2) == 0)
    {

    }
    else
    {

    }

    /* Close the driver: */
    MIBSPI_close(handle);
    MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handle);

    /**************************************************************************
     * Test: Reopen the driver in master mode for loopback test without DMA
     **************************************************************************/
    snprintf(testCase, 64, "SPI loopback test - instance(%d), 16bits non-DMA mode", inst);

    /* Change dma configuration */
    params.dmaEnable = 0;

    handle = MIBSPI_open(gMibspiInst[inst], &params);
    if (handle == NULL)
    {
        MIBSPI_log("Error: Unable to open the SPI Instance\n");
        return;
    }
    MIBSPI_log("Debug: SPI Instance %p has been opened successfully\n", handle);

    /* Start Internal Loopback Test in master mode */
    if(Test_spiLoopback(handle, slaveIndex, params.u.masterParams.slaveProf[slaveIndex].ramBufLen, 2) == 0U)
    {

    }
    else
    {

    }

    /* Close the driver: */
    MIBSPI_close(handle);
    MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handle);

    /**************************************************************************
     * Test: Reopen the driver in master mode for loopback test DMA enabled 8bits
     **************************************************************************/
    snprintf(testCase, 64, "SPI loopback test - instance(%d), 8bits DMA mode", inst);

    params.dataSize =8;
#ifdef MIBSPI_DMA_ENABLE
    /* Change dma configuration */
    params.dmaEnable = 1;
#else
    params.dmaEnable = 0;
    params.dmaHandle = NULL;
#endif


    handle = MIBSPI_open(gMibspiInst[inst], &params);
    if (handle == NULL)
    {
        MIBSPI_log("Error: Unable to open the SPI Instance\n");
        return;
    }
    MIBSPI_log("Debug: SPI Instance %p has been opened successfully\n", handle);

    /* Start Internal Loopback Test in master mode */
    if(Test_spiLoopback(handle, slaveIndex, params.u.masterParams.slaveProf[slaveIndex].ramBufLen, 1) == 0U)
    {

    }
    else
    {

    }

    /* Close the driver: */
    MIBSPI_close(handle);
    MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handle);

    /**************************************************************************
     * Test: Reopen the driver in master mode for loopback test DMA enabled 8bits
     **************************************************************************/
    snprintf(testCase, 64, "SPI loopback test - instance(%d), 8bits non-DMA mode", inst);

    /* Change dma configuration */
    params.dmaEnable = 0;
    params.dataSize =8;

    handle = MIBSPI_open(gMibspiInst[inst], &params);
    if (handle == NULL)
    {
        MIBSPI_log("Error: Unable to open the SPI Instance\n");
        return;
    }
    MIBSPI_log("Debug: SPI Instance %p has been opened successfully\n", handle);

    /* Start Internal Loopback Test in master mode */
    if(Test_spiLoopback(handle, slaveIndex, params.u.masterParams.slaveProf[slaveIndex].ramBufLen, 1) == 0U)
    {

    }
    else
    {

    }

    /* Close the driver: */
    MIBSPI_close(handle);
    MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handle);

    /**************************************************************************
     * Test: Reopen the driver in master mode for loopback throughput test
     **************************************************************************/
    /* Start Loopback throughput Test in master mode */
    Test_spiLoopBackDataThroughput(inst, 1000000U);

    /* Start Loopback throughput Test in master mode */
    Test_spiLoopBackDataThroughput(inst, 2000000U);

    /* Start Loopback throughput Test in master mode */
    Test_spiLoopBackDataThroughput(inst, 6000000U);

    /* Start Loopback throughput Test in master mode */
    Test_spiLoopBackDataThroughput(inst, 10000000U);

    /* Start Loopback throughput Test in master mode */
    Test_spiLoopBackDataThroughput(inst, 20000000U);

    /* Start Loopback throughput Test in master mode */
    Test_spiLoopBackDataThroughput(inst, 40000000U);
}

#ifdef MIBSPI_DMA_ENABLE
/**
 *  @b Description
 *  @n
 *      SPI loopback test.
 *
 *   @param[in] inst               SPI instance: 0-SPIA, 1-SPIB
 *
 *  @retval
 *      Not Applicable.
 */
void Test_loopback_oneInstance_callback(uint32_t inst, uint8_t slaveIndex)
{
    MIBSPI_Params     params;
    MIBSPI_Handle     handle;
    char testCase[64];

    snprintf(testCase, 64, "SPI loopback test callback mode - instance(%d), 16bits DMA mode", inst);

    /**************************************************************************
     * Test: Open the driver in master mode for loopback test
     **************************************************************************/
    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);
    params.frameFormat = MIBSPI_POL0_PHA0;

    /* Configure for the callback mode. */
    params.transferMode = MIBSPI_MODE_CALLBACK;
    params.transferCallbackFxn = &Test_loopback_callbackFxn;

    /* Enable DMA and set DMA channels to be used */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle[inst];

    params.eccEnable = 0;
    params.mode = MIBSPI_MASTER;
    params.u.masterParams.bitRate = 1000000U;

    /* mibSPIA support only one slave */
    if(inst == 0)
    {
        params.u.masterParams.numSlaves = 1;
        params.u.masterParams.slaveProf[0].chipSelect = 0;
        params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM;
        params.u.masterParams.slaveProf[0].dmaReqLine = 0U;
    }
    else if(inst == 1)
    {
        /* The total element size of 3 slaves is MIBSPI_RAM_MAX_ELEM
         * In this example, it is distributed among 3 slaves by MIBSPI_RAM_MAX_ELEM - 6U, 4U, and 2U
         */
        memset((void *)&params.u.masterParams.slaveProf[0], 0, sizeof(params.u.masterParams.slaveProf));

        params.u.masterParams.numSlaves = 3;
        params.u.masterParams.slaveProf[0].chipSelect = 0;
        params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM/2;
        params.u.masterParams.slaveProf[0].dmaReqLine = 0U;
        params.u.masterParams.slaveProf[1].chipSelect = 1;
        params.u.masterParams.slaveProf[1].ramBufLen = MIBSPI_RAM_MAX_ELEM/4;
        params.u.masterParams.slaveProf[1].dmaReqLine = 1U;
        params.u.masterParams.slaveProf[2].chipSelect = 2;
        params.u.masterParams.slaveProf[2].ramBufLen = MIBSPI_RAM_MAX_ELEM/4;
        params.u.masterParams.slaveProf[2].dmaReqLine = 2U;
    }
    else
    {
        printf("Error: Invalid instance(=%d) of MibSPI\n", inst);
    }

    handle = MIBSPI_open(gMibspiInst[inst], &params);
    if (handle == NULL)
    {
        MIBSPI_log("Error: Unable to open the SPI Instance\n");
        return;
    }
    MIBSPI_log("Debug: SPI Instance %p has been reopened in master mode successfully\n", handle);

    /* Start Internal Loopback Test in master mode */
    if(Test_spiLoopback_callback(handle, slaveIndex, 128, 2) == 0)
    {

    }
    else
    {

    }

    /* Close the driver: */
    MIBSPI_close(handle);
    MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handle);
}
#endif

/**
 *  @b Description
 *  @n
 *      SPI loopback test.
 *
 *   @param[in] inst               SPI instance: 0-SPIA, 1-SPIB
 *
 *  @retval
 *      Not Applicable.
 */
void Test_loopbackSlave_oneInstance(uint32_t inst)
{
    MIBSPI_Params     params;
    MIBSPI_Handle     handle;
    char testCase[64];

    snprintf(testCase, 64, "SPI loopback test - instance(%d), 16bits DMA mode", inst);

    /**************************************************************************
     * Test: Open the driver in master mode for loopback test
     **************************************************************************/
    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);
    params.frameFormat = MIBSPI_POL0_PHA0;

    /* Enable DMA and set DMA channels to be used */
#ifdef MIBSPI_DMA_ENABLE
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle[inst];
#else
    params.dmaEnable = 0;
    params.dmaHandle = NULL;
#endif

    params.eccEnable = 0;
    params.mode = MIBSPI_SLAVE;

    params.u.slaveParams.dmaReqLine = 0;
    params.u.slaveParams.chipSelect = 0;

    handle = MIBSPI_open(gMibspiInst[inst], &params);
    if (handle == NULL)
    {
        MIBSPI_log("Error: Unable to open the SPI Instance\n");
        return;
    }
    MIBSPI_log("Debug: SPI Instance %p has been reopened in master mode successfully\n", handle);

    /* Start Internal Loopback Test in master mode */
    if(Test_spiLoopbackSlave(handle, MIBSPI_RAM_MAX_ELEM, 2) == 0)
    {

    }
    else
    {

    }

    /* Close the driver: */
    MIBSPI_close(handle);
    MIBSPI_log("Debug: SPI Instance %p has been closed successfully\n", handle);
}


/**
 *  @b Description
 *  @n
 *      This function tests SPI driver in Digital Loopback mode.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] slaveIndex        Flag for internal/external loopback
 *   @param[in] maxElem           Maxim data element
 *   @param[in] dataSize          Data size in number of bytes
 *
 *  @retval    Successful                   =0
 *             Number of transfer failures  >0
 *             API failures                 <0
 */
static int32_t Test_spiLoopbackSlave(const MIBSPI_Handle handle, uint32_t maxElem, uint8_t dataSize)
{
    MibSpi_LoopBackType            loopback;
    uint32_t           loop;
    uint32_t           idx;
    uint32_t           failed = 0;
    uint32_t           len=0;

    /* Only dataSize of 1 byte or 2 bytes are supported */
    if ((dataSize != (uint8_t)1U) && (dataSize != (uint8_t)2U))
        return -1;

    /* Enable digital loopback */
    loopback = MIBSPI_LOOPBK_ANALOG;
    if(MIBSPI_control(handle, MIBSPI_CMD_LOOPBACK_ENABLE, (void *)&loopback) < 0)
        return -1;
    for(loop=0; loop < maxElem; loop++)
    {
        len = (maxElem - loop) * dataSize;

        /* Prepare Tx/Rx Buffer */
        for(idx=0; idx<maxElem * dataSize; idx++)
        {
            txBuf[idx] = (loop * 0x10 + 0x55 + idx) & 0xFF;
        }

        /* Clear receive buffer */
        memset((void *)&rxBuf[0], 0x0, SPI_DATA_BLOCK_SIZE);

        CacheP_wbInv((Ptr)txBuf, sizeof(txBuf));
        CacheP_wbInv((Ptr)rxBuf, sizeof(rxBuf));
        if(Test_spiReadWrite(handle, len, (void *)rxBuf, (void *)txBuf, 0) == 0)
        {
            /* Check data integrity */
            if (memcmp((void *)txBuf, (void *)rxBuf, len) != 0)
            {
                #if 0 /* Temporarily disable integrity check for MIBSPI SLAVE mode analog loopback until test setup issue is resolved */
                MIBSPI_log("Error: MIBSPI_transfer is successful with incorrect data(0x%x), length = %d\n", rxBuf[0], len);
                #endif
                failed++;
            }
        }
        else
        {
            MIBSPI_log("Debug: MIBSPI_transfer failed for length = %d\n", len);
            failed++;
        }
    }
    MIBSPI_log("Debug: Finished Analog loopback with various length test,  failed %d out of %d times\n", failed, loop);

    /* Disable digital loopback */
    loopback = MIBSPI_LOOPBK_NONE;
    if(MIBSPI_control(handle, MIBSPI_CMD_LOOPBACK_ENABLE, (void *)&loopback) < 0)
        return -1;

    /* Return number of failures */
    return failed;
}
