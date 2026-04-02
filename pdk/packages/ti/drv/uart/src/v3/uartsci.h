/**
 *   @file  uartsci.h
 *
 *   @brief
 *      This is the internal Header for the UART SCI Driver. This header
 *      file should *NOT* be directly included by applications.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016-2020 Texas Instruments, Inc.
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

/** @defgroup UART_SCI      UART Driver
 */
#ifndef UART_SCI_H
#define UART_SCI_H

#include <ti/osal/osal.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/edma/edma.h>
#include <ti/csl/src/ip/sci/V0/cslr_sci.h>

#include <ti/drv/uart/src/v3/uartsci_soc.h>

/**
@defgroup UART_SCI_INTERNAL_FUNCTION            UART Driver Internal Functions
@ingroup UART_SCI
*/
/**
@defgroup UART_SCI_INTERNAL_DATA_STRUCTURE      UART Driver Internal Data Structures
@ingroup UART_SCI
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup UART_SCI_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  UART SCI Driver Duplexity capability
 *
 * @details
 *  The enumeration describes the duplexity capabilities of the UART
 *  SCI Driver
 */
typedef enum UartSci_Duplexity_e
{
    /**
     * @brief   Driver is capable of sending/receiving data
     */
    UartSci_Duplexity_FULL              = 0x1,

    /**
     * @brief   Driver is only capable of sending data
     */
    UartSci_Duplexity_TX_ONLY
}UartSci_Duplexity;

/**
 * @brief
 *  UART SCI Driver Status
 *
 * @details
 *  The enumeration describes the status of the UART Driver Instance
 */
typedef enum UartSci_DriverStatus_e
{
    /**
     * @brief   Driver is uninitialized.
     */
    UartSci_DriverStatus_UNINITIALIZED  = 0x0,

    /**
     * @brief   Driver is operational.
     */
    UartSci_DriverStatus_OPERATIONAL
}UartSci_DriverStatus;

typedef struct UartSci_HwCfg_t  UartSci_HwCfg;


/**
 * @brief
 *  SCI Driver EDMA Block
 *
 * @details
 *  The structure is used to hold all the pertinent information with respect
 *  to the UART DMA.
 */
typedef struct UartSci_DriverEDMA_t
{
    /**
     * @brief   This is a pointer to the hardware configuration.
     */
    UartSci_HwCfg*          ptrHwCfg;

    /**
     * @brief   EDMA Channel configuration which is populated to setup
     * the transfer
     */
    EDMA_channelConfig_t    config;

    /**
     * @brief   Dummy EDMA Param configuration for the last transfer
     */
    EDMA_paramConfig_t      dummyParamConfig;

}UartSci_DriverEDMA;


/**
 * @brief
 *  SCI Driver Master Control Block
 *
 * @details
 *  The structure is used to hold all the pertinent information with respect
 *  to the UART Driver.
 */
typedef struct UartSci_Driver_t
{
    /**
     * @brief   Driver handle
     */
    UART_Handle             handle;

    /**
     * @brief   Status of the Driver Instance
     */
    UartSci_DriverStatus    status;

    /**
     * @brief   Pointer to the data buffer which is to be sent out by the UART
     * Driver.
     */
    uint8_t*                ptrWriteBuffer;

    /**
     * @brief   This is the number of bits the data needs to be shifted. This is
     * calculated using the data length.
     */
    uint8_t                 shiftJustification;

    /**
     * @brief   Number of characters which have been sent out.
     */
    int32_t                 writeCount;

    /**
     * @brief   Number of characters which remain in the write buffer.
     */
    int32_t                 writeSize;

    /**
     * @brief   Flag which indicates if a return character is to be written.
     * This is used only while operating in the TEXT Mode.
     */
    uint32_t                writeCR;

    /**
     * @brief   Handle to the Write semaphore.
     */
    SemaphoreP_Handle       writeSem;

    /**
     * @brief   Pointer to the data buffer where the recevied data is placed
     */
    uint8_t*                ptrReadBuffer;

    /**
     * @brief   Number of characters which have been read.
     */
    int32_t                 readCount;

    /**
     * @brief   Number of characters which remain in the read buffer.
     */
    int32_t                 readSize;

    /**
     * @brief   Handle to the Read semaphore.
     */
    SemaphoreP_Handle       readSem;

    /**
     * @brief   Registered Interrupt Handler.
     */
    HwiP_Handle             hwiHandle;

    /**
     * @brief   UART Parameters which were used to initialize the driver instance
     */
    UART_Params             params;

    /**
     * @brief   Rx/Tx EDMA information
     */
    UartSci_DriverEDMA      dmaInfo[2];

    /**
     * @brief   Flag which indicates if DMA is enabled
     */
    bool                    isDMAEnabled;

    /**
     * @brief   Statistics associated with the driver instance:
     */
    UART_Stats              stats;
}UartSci_Driver;

/**
 * @brief
 * This is the function prototype which is used by the UART driver
 * to open and setup the DMA services.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver MCB
 *  @param[in]  ptrHwCfg
 *      Pointer to the UART Driver Hardware configuration
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
typedef int32_t (*UartSci_openDMAFxn) (UartSci_Driver* ptrUartSciDriver, UartSci_HwCfg* ptrHwCfg);

/**
 * @brief
 * This is the function prototype which is used by the UART driver
 * to close the DMA services
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver MCB
 *  @param[in]  ptrHwCfg
 *      Pointer to the UART Driver Hardware configuration
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
typedef int32_t (*UartSci_closeDMAFxn) (UartSci_Driver* ptrUartSciDriver);

/**
 * @brief
 *  This is the function prototype which is used by the UART driver
 *  to check if the DMA services are available.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver MCB
 *
 *  @retval
 *      true    - DMA Services are available
 *  @retval
 *      false   - DMA Services are not available
 */
typedef bool (*UartSci_isDMAEnabledFxn) (UartSci_Driver* ptrUartSciDriver);

/**
 * @brief
 * This is the function prototype which is used by the UART driver
 * to initiate the receive DMA
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver MCB
 *  @param[in]  dstAddress
 *      Destination address where the data is placed after it is received
 *      by the UART driver.
 *  @param[in]  numBytes
 *      Number of bytes to be transmitted
 *  @param[in]  pollingMode
 *      Set to true to indicate that we need to poll for completion else
 *      set to false
 *
 *  @retval
 *      Not applicable
 */
typedef int32_t (*UartSci_initiateRxDMAFxn)
(
    UartSci_Driver* ptrUartSciDriver,
    uint32_t        dstAddress,
    uint32_t        numBytes,
    bool            pollingMode
);

/**
 * @brief
 * This is the function prototype which is used by the UART driver
 * to initiate the transmit DMA
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver MCB
 *  @param[in]  srcAddress
 *      Source address of the buffer which is to be transmitted
 *  @param[in]  numBytes
 *      Number of bytes to be transmitted
 *  @param[in]  pollingMode
 *      Set to true to indicate that we need to poll for completion else
 *      set to false
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error Code  -   <0
 */
typedef int32_t (*UartSci_initiateTxDMAFxn)
(
    UartSci_Driver* ptrUartSciDriver,
    uint32_t        srcAddress,
    uint32_t        numBytes,
    bool            pollingMode
);

/**
 * @brief
 *  SCI Driver HW configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to each driver instance
 */
struct UartSci_HwCfg_t
{

    /**
     * @brief   Base address of the SCI registers to be used
     */
    CSL_sciRegs*                ptrSCIRegs;

    /**
     * @brief   Duplexity
     */
    UartSci_Duplexity           duplexity;


    /**
     * @brief   Interrupt Number or Event Id for C6x
     */
    uint32_t                    interruptNum;

    /**
     * @brief   Transmit DMA Request Line or EMDA event number
     */
    uint32_t                    txDMARequestLine;

    /**
     * @brief   Receive DMA Request Line or EDMA event number
     */
    uint32_t                    rxDMARequestLine;

    /**
     * @brief   Registered DMA Open Fxn
     */
    UartSci_openDMAFxn          openDMAFxn;

    /**
     * @brief   Registered DMA Close Fxn
     */
    UartSci_closeDMAFxn         closeDMAFxn;

    /**
     * @brief   Registered Is DMA Enabled Fxn
     */
    UartSci_isDMAEnabledFxn     isDMAEnabledFxn;

    /**
     * @brief   Registered Initiate Receive DMA Fxn
     */
    UartSci_initiateRxDMAFxn    initiateRxDMAFxn;

    /**
     * @brief   Registered Initiate Transmit DMA Fxn
     */
    UartSci_initiateTxDMAFxn    initiateTxDMAFxn;

    /**
     * @brief   IP-specific sftware configurations
     */
    UartSci_SwCfg               swCfg;
};

/*********************************************************************
 * UART SCI DMA Enable/Disable Functions
 *********************************************************************/
extern void UartSci_enableTxDMA  (CSL_sciRegs* ptrSCIRegs);
extern void UartSci_disableTxDMA (CSL_sciRegs* ptrSCIRegs);
extern void UartSci_disableRxDMA (CSL_sciRegs* ptrSCIRegs);
extern void UartSci_enableRxDMA  (CSL_sciRegs* ptrSCIRegs);

/*********************************************************************
 * Port of the UART-DMA Layer:
 *  - These use the EDMA Driver to provide the functionality to
 *    the UART Driver.
 *********************************************************************/
extern int32_t  UartSci_openEDMA   (UartSci_Driver* ptrUartSciDriver, UartSci_HwCfg* ptrHwCfg);
extern int32_t  UartSci_closeEDMA  (UartSci_Driver* ptrUartSciDriver);
extern bool     UartSci_isEDMAEnabled (UartSci_Driver* ptrUartSciDriver);
extern int32_t  UartSci_initiateTxEDMA
(
    UartSci_Driver* ptrUartSciDriver,
    uint32_t        srcAddress,
    uint32_t        numBytes,
    bool            pollingMode
);
extern int32_t  UartSci_initiateRxEDMA
(
    UartSci_Driver* ptrUartSciDriver,
    uint32_t        dstAddress,
    uint32_t        numBytes,
    bool            pollingMode
);

/*********************************************************************
 * UART SCI Utility Functions
 *********************************************************************/
extern void UartSci_callback (UART_Handle handle, bool isRx);

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* UART_SCI_H */
