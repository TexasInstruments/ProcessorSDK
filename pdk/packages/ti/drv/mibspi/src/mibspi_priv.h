/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef MIBSPI_PRIV_H
#define MIBSPI_PRIV_H

#include <ti/drv/mibspi/MIBSPI.h>
#include <ti/drv/mibspi/soc/MIBSPI_soc.h>
#include <ti/drv/edma/edma.h>

#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief
 *  MibSPI RAM buffer mode
 */
#define MIBSPI_RAM_BUFFER_MODE              7U

/**
 * @brief
 *  Invalide DMA reqline value
 */
#define MIBSPI_INVALID_DMA_REQLINE          (uint8_t)0xFFU

/**
 * @brief
 *  Transfer group used in slave mode
 */
#define MIBSPI_SLAVEMODE_TRANS_GROUP        0U

/**
 * @brief
 *  MIBSPI interrupt level
 */
#define MIBSPI_INT_LEVEL                    1U

/**
 * @brief
 *  MibSPI Driver DMA call back funtion
 */
typedef void (*t_callBackFuncPTR)(void *transaction);

/**
 * @brief
 *  MibSPI Driver DMA interrupt handler function
 */
typedef void (*t_intHandlerFunc)(void);

/**
 * @brief
 *  SPI Driver Transfer type
 *
 * @details
 *  The structure is used to store the internal transfer type
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef enum MibSpi_transferType_e
{
    /* MIBSPI transfer to send data */
    MIBSPI_TRANSFER_TX = 0,

    /* MIBSPI transfer to receive data */
    MIBSPI_TRANSFER_RX,

    /* MIBSPI transfer to send dummy data */
    MIBSPI_TRANSFER_DUMMY_TX,

    /* MIBSPI transfer to put recevied data in scratch buffer */
    MIBSPI_TRANSFER_DUMMY_RX
}MibSpi_transferType;

/**
 * @brief
 *  Enum used to track status of EDMA callbacks internally in driver
 *
 */
enum MibSpi_edmaCBFlag_e
{
    MIBSPI_NONE_EDMA_CALLBACK_OCCURED  =   0x0U,
    /**< Flag to indicate that Transmission EDMA callback has occured         */

    MIBSPI_TX_EDMA_CALLBACK_OCCURED    =   0x1U,
    /**< Flag to indicate that Transmission EDMA callback has occured         */

    MIBSPI_RX_EDMA_CALLBACK_OCCURED    =   0x2U,
    /**< Flag to indicate that Reception EDMA callback has occured            */
};

/*!
 *  @brief  SPI_v3 transfer error code
 */
enum MibSpi_xferErr_e {
    MIBSPI_XFER_ERR_NONE,   /* No transfer error */
    MIBSPI_XFER_ERR_RXOR,   /* Receiver overrun error */
    MIBSPI_XFER_ERR_BE,     /* Bit error */
    MIBSPI_XFER_ERR_TIMEOUT /* Transfer timeout error */
};


/**
 * @brief
 *  SPI Driver Statistics
 *
 * @details
 *  The structure is used to store driver statistics
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MibSpiDriverHWStats_t
{
    /**
     * @brief   Number of data length error interrupts
     */
    uint32_t    dlenErr;

    /**
     * @brief   Number of timeout interrupts
     */
    uint32_t    timeout;

    /**
     * @brief   Number of Parity Error interrupts
     */
    uint32_t    parErr;

    /**
     * @brief   Number of De-synchronization of slave device interrupts
     */
    uint32_t    desync;

    /**
     * @brief   Number of mismatch of internal transmit data and transmittted data error interrupts
     */
    uint32_t    bitErr;

    /**
     * @brief   Number of RX Overrun Error Interrupts
     */
    uint32_t    rxOvrnErr;

    /**
     * @brief   Number of RX Full Interrupts
     */
    uint32_t    rxFull;

    /**
     * @brief   Number of TX Empty Interrupts
     */
    uint32_t    txEmpty;

    /**
     * @brief   Number of TG Complete interrupts
     */
    uint32_t    TGComplete[MIBSPI_NUM_TRANS_GROUP];
}MibSpiDriverHWStats;

/**
 * @brief
 *  SPI Driver Info for Master
 *
 * @details
 *  The structure is used to store the driver info for Master.
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MibSpi_driverDmaInfo_t
{
    /**
     * @brief   Number of DMA Recieve Interrupts
     */
    uint32_t    rxDmaIntCnt;

    /**
     * @brief   Number of DMA Transmit Interrupts
     */
    uint32_t    txDmaIntCnt;
} MibSpi_driverDmaInfo;

typedef struct Mibspi_transactionState_s
{
    volatile enum MibSpi_edmaCBFlag_e edmaCbCheck;

    enum MibSpi_xferErr_e    transferErr;

    MIBSPI_Transaction         *transaction;

    volatile uint16_t remainSize;
    volatile uint16_t dataLength;
    volatile uint16_t dataSizeInBytes;
} Mibspi_transactionState_t;


/*!
 *  @brief      SPI Driver Object
 *
 * @details
 *  The structure is used to store the SPI driver internal variables.
 *  The application must not access any member variables of this structure!
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MibSpiDriver_Object_t
{
    /**
     * @brief   SPI driver handle.
     */
    MIBSPI_Handle             mibspiHandle;

    /**
     * @brief   Pointer to MibSpi driver Hardware Configuration
     */
    MibSpi_HwCfg  const      *ptrHwCfg;

    /**
     * @brief   MibSpi driver parameters
     */
    MIBSPI_Params              params;

    /**
     * @brief   Registered Interrupt Handler.
     */
    HwiP_Handle             hwiHandle;


    /**
     * @brief   Handle to the Transfer complete semaphore.
     */
    SemaphoreP_Handle       transferCompleteSem;

    /**
     * @brief   DMA information used in transfer for remote peers
     */
    MibSpi_driverDmaInfo    dmaInfo[MIBSPI_SLAVE_MAX];

    /**
     * @brief   MibSPI mode RAM offset start settings for remote peers
     */
    uint8_t                 rambufStart[MIBSPI_SLAVE_MAX];

    /**
     * @brief   MibSPI mode RAM offset end settings for remote peers
     */
    uint8_t                 rambufEnd[MIBSPI_SLAVE_MAX];

    /**
     * @brief   Rx Scratch buffer, used as scratch buffer to dump received
     *          data from SPI transfer when application does not provide
     *          receive buffer.
     */
    uint16_t                rxScratchBuffer;

    /**
     * @brief   Tx Scratch buffer, used when TX data is not provided for SPI_transfer()
     *          The driver transmits txDummyValue from ::SPI_Params_t
     */
    uint16_t                txScratchBuffer;

    /**
     * @brief   MibSpi driver stats.
     */
    MibSpiDriverHWStats     hwStats;

    /**
     * @brief  State info on current transaction
     */
    Mibspi_transactionState_t transactionState;

} MibSpiDriver_Object, *MibSpiDriver_Handle;

/**
 * \brief  This API gets the SPI instance id  matching the SPI instance name
 *
 * \param  mibspiInstName [in] Unique string identifying the MIBSPI instance
 * \param  idx[out]  SPI instance index.
 *
 * \return           SPI_SUCCESS success: MIBSPI_STATUS_ERROR : error
 *
 */
int32_t MIBSPI_socGetInstIndex(enum MibSpi_InstanceId mibspiInstId, uint32_t *idx);
MIBSPI_Handle MIBSPI_socGetInstHandle(enum MibSpi_InstanceId mibspiInstanceId);
void MIBSPI_socMemInit(enum MibSpi_InstanceId mibspiInstanceId);

MIBSPI_Handle MIBSPI_openCore(MIBSPI_Handle handle, const MIBSPI_Params *params);
void MIBSPI_closeCore(MIBSPI_Handle handle);
int32_t MIBSPI_controlCore(MIBSPI_Handle handle, uint32_t cmd, const void *arg);
bool MIBSPI_transferCore(MIBSPI_Handle handle, MIBSPI_Transaction *transaction);
void MIBSPI_transferCancelCore(MIBSPI_Handle handle);

/**@}*/
#ifdef __cplusplus
}
#endif

#endif
