/******************************************************************************
Copyright (c) Copyright 2019 Oculii.

  Description:
      This file contains the Oculii SPI driver source code

******************************************************************************/

/* Include files */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

#include <ti/drv/mibspi/MIBSPI.h>
#include <ti/drv/edma/edma.h>

/* Demo Include Files */
#include "mss_Utils.h"


/* Macro Define */
#define MIBSPI_BLOCK_SIZE           (MIBSPI_RAM_MAX_ELEM*2U)    /* Maximum DMA transfer bytes */

extern EDMA_Handle          gDmaHandle;
MIBSPI_Handle               gMibspiHandle;

/*******************************************************************************
 * Initializes and opens the SPI Instances.
 *
 * Parameters
 *  - None
 *
 * Return
 *  - None
 *******************************************************************************/
void MSS_UtilsSPIInit(void)
{
//#ifdef USE_SPI_A_AS_SLAVE
    MIBSPI_Params  stSPIParams;

    /**************************************************
     * Configure and open MSS SPI A
     **************************************************/
    /* Setup the default MSS SPI A Parameters
     *      - mode                 = MIBSPI_SLAVE,
     *      - dataSize (Bits)      = 16U,
     *      - frameFormat          = MIBSPI_POL0_PHA0,
     *      - transferMode         = MIBSPI_MODE_BLOCKING,
     *      - transferTimeout      = MIBSPI_WAIT_FOREVER,
     *      - transferCallbackFxn  = NULL,
     *      - pinMode              = MIBSPI_PINMODE_4PIN_CS,
     *      - shiftFormat          = MIBSPI_MSB_FIRST,
     *      - dmaEnable            = (uint8_t)1U,
     *      - eccEnable            = (uint8_t)0U,
     *      - csHold               = (uint8_t)0U,
     *      - txDummyValue         = (uint16_t)0xFFFFU,
     *      - compatibilityMode    = FALSE,
     *      - dmaHandle            = NULL,
     *
     */
    MIBSPI_Params_init(&stSPIParams);

    stSPIParams.mode        = MIBSPI_SLAVE;
    stSPIParams.frameFormat = MIBSPI_POL1_PHA1;
    stSPIParams.eccEnable   = 1U;
    stSPIParams.csHold      = 1U;

    /* Enable DMA and set DMA channels */
    if (gDmaHandle != NULL)
    {
        stSPIParams.dmaEnable = 1U;                         /* Enable eDMA */
    }
    else
    {
        stSPIParams.dmaEnable = 0U;                         /* Disable eDMA */
    }
    stSPIParams.dmaHandle = gDmaHandle;                 /* MSS_SPIA and MSS_SPIB are connected to the same eDMA instance (MSS_EDMAA).
                                                               The MSS eDMAA instance should have been already initialized before this. */

    stSPIParams.u.slaveParams.dmaReqLine = 0U;          /* This parameter is not used in the slave mode. The DMA req line is forced to '0' during transfer. */
    stSPIParams.u.slaveParams.chipSelect = 0U;


    /* Open MSS_SPIA */
    gMibspiHandle = MIBSPI_open(MIBSPI_INST_ID_MSS_SPIA, &stSPIParams);
    if (gMibspiHandle == NULL) {
        /* Error: Display the error message: */
        System_printf ("Error: MSS cannot open MSS SPI A.\n");
        /* Not expected */
    } else {
        /* SPI A is successfully opened. */
    }
//#endif
}

/*******************************************************************************
 * SPI read only function.
 *
 * Parameters
 *  - stHandle          SPI driver handle
 *  - u32DataLen        Number of bytes need to be transferred
 *  - pvBuffer          Pointer of the buffer
 *
 * Return
 *  - Successful        = 0
 *  - Failed            < 0
 *******************************************************************************/
int32_t MSS_UtilsSPIRead(const MIBSPI_Handle stHandle, uint32_t u32DataLen, uint8_t * pu8Buffer)
{
    MIBSPI_Transaction stTransaction;
    uint32_t u32RemainingBytes;
    uint32_t u32ChunkSize;
    int32_t  i32RetVal;
    uint8_t *pu8BuffPtr;

    /* Initialize local variables */
    i32RetVal         = 0;
    u32RemainingBytes = u32DataLen;
    pu8BuffPtr = pu8Buffer;

    /* It is important to invalidate the cache because the SPI driver will use eDMA transfer
    *   between the memory and the internal SPI RAM buffer. */
    CacheP_wbInv((void *) pu8BuffPtr, (int32_t) u32DataLen);

    stTransaction.slaveIndex = 0U;
    stTransaction.txBuf      = NULL;

    /* Full chunk of SPI transfer */
    while(u32RemainingBytes > 0U)
    {
        if(u32RemainingBytes >= MIBSPI_BLOCK_SIZE)
        {
            u32ChunkSize = MIBSPI_BLOCK_SIZE;
        }
        else
        {
            u32ChunkSize = u32RemainingBytes;
        }

        /* Configure Data Transfer */
        stTransaction.count = u32ChunkSize;
        stTransaction.rxBuf = (void *) pu8BuffPtr;

        if(MIBSPI_transfer(stHandle, &stTransaction) != true)
        {
            u32RemainingBytes = 0U;
            i32RetVal         = -1;
            break;
        }
        else
        {
            /* Prepare next chunk */
            pu8BuffPtr         += u32ChunkSize;
            u32RemainingBytes -= u32ChunkSize;
        }
    }

    return i32RetVal;
}

/*******************************************************************************
 * SPI write only function.
 *
 * Parameters
 *  - stHandle          SPI driver handle
 *  - u32DataLen        Number of bytes need to be transferred
 *  - pvBuffer          Pointer of the buffer
 *
 * Return
 *  - Successful        = 0
 *  - Failed            < 0
 *******************************************************************************/
int32_t MSS_UtilsSPIWrite(const MIBSPI_Handle stHandle, uint32_t u32DataLen, uint8_t * pu8Buffer)
{
    MIBSPI_Transaction stTransaction;
    uint32_t u32RemainingBytes;
    uint32_t u32ChunkSize;
    int32_t  i32RetVal;
    uint8_t *pu8BuffPtr;

    /* Initialize local variables */
    i32RetVal         = 0;
    u32RemainingBytes = u32DataLen;
    pu8BuffPtr = pu8Buffer;

    /* It is important to invalidate the cache because the SPI driver will use eDMA transfer
    *   between the memory and the internal SPI RAM buffer. */
    CacheP_wbInv((void *) pu8BuffPtr, (int32_t) u32DataLen);

    stTransaction.slaveIndex = 0U;
    stTransaction.rxBuf      = NULL;

    /* Full chunk of SPI transfer */
    while(u32RemainingBytes > 0U)
    {
        if(u32RemainingBytes >= MIBSPI_BLOCK_SIZE)
        {
            u32ChunkSize = MIBSPI_BLOCK_SIZE;
        }
        else
        {
            u32ChunkSize = u32RemainingBytes;
        }

        /* Configure Data Transfer */
        stTransaction.count = u32ChunkSize;
        stTransaction.txBuf = (void *) pu8BuffPtr;

        if(MIBSPI_transfer(stHandle, &stTransaction) != true)
        {
            u32RemainingBytes = 0U;
            i32RetVal         = -1;
            break;
        }
        else
        {
            /* Prepare next chunk */
            pu8BuffPtr         += u32ChunkSize;
            u32RemainingBytes -= u32ChunkSize;
        }
    }

    return i32RetVal;
}

/**********************************************************************
  @! This function will swap bytes
 **********************************************************************/
void swapSPIBytes(uint8_t * pu8Buffer, uint32_t u32Bytes)
{
    uint32_t u32Idx;
    uint8_t u8Tmp;
    for(u32Idx = 0U; u32Idx < u32Bytes; u32Idx += 2U)
    {
        u8Tmp                   = pu8Buffer[u32Idx];
        pu8Buffer[u32Idx]       = pu8Buffer[u32Idx+1U];
        pu8Buffer[u32Idx+1U]    = u8Tmp;
    }
}

/*******************************************************************************
 * SPI read and write function.
 *
 * Parameters
 *  - stHandle          SPI driver handle
 *  - u32DataLen        Number of bytes need to be transferred
 *  - pvBuffer          Pointer of the buffer
 *
 * Return
 *  - Successful        = 0
 *  - Failed            < 0
 *******************************************************************************/
int32_t MSS_UtilsSPIReadWrite(const MIBSPI_Handle stHandle, uint32_t u32DataLen, uint8_t * pu8TxBuffer, uint8_t * pu8RxBuffer)
{
    MIBSPI_Transaction stTransaction;
    uint32_t u32RemainingBytes;
    uint32_t u32ChunkSize;
    int32_t  i32RetVal;
    uint8_t *pu8WrPtr;
    uint8_t *pu8RdPtr;

    /* Initialize local variables */
    i32RetVal         = 0;
    u32RemainingBytes = u32DataLen;
    pu8WrPtr = pu8TxBuffer;
    pu8RdPtr = pu8RxBuffer;

    /* It is important to invalidate the cache because the SPI driver will use eDMA transfer
    *   between the memory and the internal SPI RAM buffer. */
    CacheP_wbInv((void *) pu8WrPtr, (int32_t) u32DataLen);
    CacheP_wbInv((void *) pu8RdPtr, (int32_t) u32DataLen);

    stTransaction.slaveIndex = 0U;

    /* Full chunk of SPI transfer */
    while(u32RemainingBytes > 0U)
    {
        if(u32RemainingBytes >= MIBSPI_BLOCK_SIZE)
        {
            u32ChunkSize = MIBSPI_BLOCK_SIZE;
        }
        else
        {
            u32ChunkSize = u32RemainingBytes;
        }

        /* Configure Data Transfer */
        stTransaction.count = u32ChunkSize;
        stTransaction.txBuf = (void *) pu8WrPtr;
        stTransaction.rxBuf = (void *) pu8RdPtr;

        if(MIBSPI_transfer(stHandle, &stTransaction) != true)
        {
            u32RemainingBytes = 0U;
            i32RetVal         = -1;
            break;
        }
        else
        {
            /* Prepare next chunk */
            pu8WrPtr         += u32ChunkSize;
            pu8RdPtr         += u32ChunkSize;
            u32RemainingBytes   -= u32ChunkSize;
        }
//        System_printf("transfer done. remaining %u\n", u32RemainingBytes);    // laffer - debug
    }

    return i32RetVal;
}

#define SPI_SLAVE_TEST_SIZE     12000U
uint8_t u8rxDut[SPI_SLAVE_TEST_SIZE];
uint8_t u8txDut[SPI_SLAVE_TEST_SIZE];

/* PC read, Sensor Write */
void spiReadTest()
{
    System_printf("SPI read test\n");
    uint32_t u32Idx;
    int32_t  i32RetVal;
    for (u32Idx = 0U; u32Idx < SPI_SLAVE_TEST_SIZE; u32Idx++)
    {
        u8txDut[u32Idx] = u32Idx;
    }
    while (1)
    {
        i32RetVal = MSS_UtilsSPIWrite(gMibspiHandle, SPI_SLAVE_TEST_SIZE, u8txDut);
        if (i32RetVal < 0) {
            System_printf("SPI write failed!!\n");
            break;
        }
    }

}

/* Sensor read, PC Write */
void spiWriteTest()
{
    System_printf("SPI write test\n");
    uint32_t u32Idx;
    int32_t  i32RetVal;
    uint32_t u32NbrErr = 0U;

    while (1)
    {
        u32NbrErr = 0U;
        i32RetVal = MSS_UtilsSPIRead(gMibspiHandle, SPI_SLAVE_TEST_SIZE, u8rxDut);
        if (i32RetVal < 0) {
            System_printf("SPI read failed!!\n");
            break;
        }
        else {
            for (u32Idx = 0U; u32Idx < SPI_SLAVE_TEST_SIZE; u32Idx++)
            {
                if (u8rxDut[u32Idx] != (u32Idx & 0xFFU)) {
                    u32NbrErr++;
    //                System_printf("txBuff[%u] = %u, rxBuff[%u] = %u\n", u32Idx, u8txDut[u32Idx], u32Idx, u8rxDut[u32Idx]);
                }
                u8rxDut[u32Idx] = 0U;
            }
//            System_printf("NumErr = %u\n", u32NbrErr);
            if (u32NbrErr > 0U) {
                System_printf("NumErr = %u\n", u32NbrErr);
                break;
            }
        }
    }
}

/* Read and Write Test */
void spiReadWriteTest()
{
    System_printf("SPI read and write test\n");
    uint32_t u32Idx;
    int32_t  i32RetVal;
    uint32_t u32NbrErr = 0U;

    for (u32Idx = 0U; u32Idx < SPI_SLAVE_TEST_SIZE; u32Idx++)
    {
        u8txDut[u32Idx] = u32Idx;
    }

    while (1)
    {
        i32RetVal = MSS_UtilsSPIReadWrite(gMibspiHandle, SPI_SLAVE_TEST_SIZE, u8txDut, u8rxDut);

        if (i32RetVal < 0) {
            System_printf("SPI read and write failed!!\n");
            break;
        }
        else {
            for (u32Idx = 0U; u32Idx < SPI_SLAVE_TEST_SIZE; u32Idx++)
            {
                if (u8rxDut[u32Idx] != (u32Idx & 0xFFU)) {
                    u32NbrErr++;
//                    System_printf("txBuff[%u] = %u, rxBuff[%u] = %u\n", u32Idx, u8txDut[u32Idx], u32Idx, u8rxDut[u32Idx]);
                }
                u8rxDut[u32Idx] = 0U;
            }
//            System_printf("NumErr = %u\n", u32NbrErr);
            if (u32NbrErr > 0U) {
                System_printf("NumErr = %u\n", u32NbrErr);
                break;
            }
        }
    }
}

/* random number test */
void spiRandomNumberTest()
{
    System_printf("SPI loop test\n");
    uint32_t u32Idx;
    int32_t  i32RetVal;
    uint32_t u32NbrErr = 0U;

    while (1)
    {
        u32NbrErr = 0U;
        i32RetVal = MSS_UtilsSPIRead(gMibspiHandle, SPI_SLAVE_TEST_SIZE, u8txDut);
        if (i32RetVal < 0) {
            System_printf("SPI read failed!!\n");
            break;
        }

        i32RetVal = MSS_UtilsSPIWrite(gMibspiHandle, SPI_SLAVE_TEST_SIZE, u8txDut);
        if (i32RetVal < 0) {
            System_printf("SPI write failed!!\n");
            break;
        }

        i32RetVal = MSS_UtilsSPIReadWrite(gMibspiHandle, SPI_SLAVE_TEST_SIZE, u8txDut, u8rxDut);
        if (i32RetVal < 0) {
            System_printf("SPI read and write failed!!\n");
            break;
        }

        for (u32Idx = 0U; u32Idx < SPI_SLAVE_TEST_SIZE; u32Idx++)
        {
            if (u8txDut[u32Idx] != u8rxDut[u32Idx]) {
                u32NbrErr++;
//                    System_printf("txBuff[%u] = %u, rxBuff[%u] = %u\n", u32Idx, u8txDut[u32Idx], u32Idx, u8rxDut[u32Idx]);
            }
            u8rxDut[u32Idx] = 0U;
            u8txDut[u32Idx] = 0U;
        }
//            System_printf("NumErr = %u\n", u32NbrErr);
        if (u32NbrErr > 0U) {
            System_printf("NumErr = %u\n", u32NbrErr);
            break;
        }
    }

}
