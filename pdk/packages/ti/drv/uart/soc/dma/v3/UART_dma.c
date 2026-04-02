/**
 *   @file  UART_dma.c
 *
 *   @brief
 *      The file implements the UART(SCI) driver EDMA Interface
 *      functionality. This file is renamed from uartsci_edma.c
 *      to be consistent with UART DMA naming convention.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/src/v3/uartsci.h>
#include <ti/drv/uart/src/UART_osal.h>
#include <ti/csl/cslr_sci.h>
#include <ti/drv/uart/src/UART_drv_log.h>

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/
#define UART_DMA_TX         0
#define UART_DMA_RX         1

/**************************************************************************
 ************************** Local Structures ******************************
 **************************************************************************/

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/
static void UartSci_TxEDMACallbackFxn(uintptr_t arg, uint8_t transferCompletionCode);
static void UartSci_RxEDMACallbackFxn(uintptr_t arg, uint8_t transferCompletionCode);

/**************************************************************************
 ************************** UART EDMA Functions ***************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked after
 *      the Transmit DMA has completed its operation.
 *
 *  @param[in]  arg
 *      Argument registered during callback function which has been set to the
 *      UART Driver Instance.
 *  @param[in]  transferCompletionCode
 *      Transfer completion code
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_TxEDMACallbackFxn(uintptr_t arg, uint8_t transferCompletionCode)
{
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_DriverEDMA* ptrDriverEDMA;

    /* Get the UART Driver: */
    ptrUartSciDriver = (UartSci_Driver*)arg;

    /* Get the DMA Block: */
    ptrDriverEDMA = &ptrUartSciDriver->dmaInfo[UART_DMA_TX];

    /* Disable the UART Transmit DMA mode */
    UartSci_disableTxDMA(ptrDriverEDMA->ptrHwCfg->ptrSCIRegs);

    /* Increment the number of transmit DMA interrupts */
    ptrUartSciDriver->stats.numTxDMAInterrupts++;

    /* Write operation for the driver is complete */
    ptrUartSciDriver->writeCount = ptrUartSciDriver->writeSize;
    ptrUartSciDriver->writeSize  = 0U;

    /* The callee was blocked so post the semaphore to wakeup the callee */
    UartSci_callback(ptrUartSciDriver->handle, BFALSE);

    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked after
 *      the Receive DMA has completed its operation.
 *
 *  @param[in]  arg
 *      Argument registered during callback function which has been set to the
 *      UART Driver Instance.
 *  @param[in]  transferCompletionCode
 *      Transfer completion code
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_RxEDMACallbackFxn(uintptr_t arg, uint8_t transferCompletionCode)
{
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_DriverEDMA* ptrDriverEDMA;

    /* Get the UART Driver: */
    ptrUartSciDriver = (UartSci_Driver*)arg;

    /* Get the DMA Block: */
    ptrDriverEDMA = &ptrUartSciDriver->dmaInfo[UART_DMA_RX];

    /* Disable the UART Receive DMA mode */
    UartSci_disableRxDMA(ptrDriverEDMA->ptrHwCfg->ptrSCIRegs);

    /* Increment the number of receive DMA interrupts */
    ptrUartSciDriver->stats.numRxDMAInterrupts++;

    /* Read operation for the driver is complete */
    ptrUartSciDriver->readCount = ptrUartSciDriver->readSize;
    ptrUartSciDriver->readSize  = 0U;

    /* The callee was blocked so post the semaphore to wakeup the callee */
    UartSci_callback(ptrUartSciDriver->handle, BTRUE);

    return;
}

/**
 *  @b Description
 *  @n
 *      The function returns the status of the DMA configuration.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      BTRUE    - DMA services are enabled
 *  @retval
 *      BFALSE   - DMA services are not enabled
 */
bool UartSci_isEDMAEnabled (UartSci_Driver* ptrUartSciDriver)
{
    return ptrUartSciDriver->isDMAEnabled;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initiate the receive DMA. Since the UART
 *      on the DSS is Transmit only this function should never be invoked
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *  @param[in]  dstAddress
 *      Destination address of the buffer where the data is to be placed
 *  @param[in]  numBytes
 *      Number of bytes to be transmitted
 *  @param[in]  pollingMode
 *      Set to BTRUE to indicate that we need to poll for completion else
 *      set to BFALSE
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns error
 */
int32_t UartSci_initiateRxEDMA
(
    UartSci_Driver* ptrUartSciDriver,
    uint32_t        dstAddress,
    uint32_t        numBytes,
    bool            pollingMode
)
{
    UartSci_DriverEDMA*     ptrDriverEDMA;
    EDMA_channelConfig_t*   ptrEDMACfg;
    int32_t                 retVal = -1;
    int32_t                 errCode;
    bool                    isTransferComplete = BFALSE;

    /* Get the DMA Block: */
    ptrDriverEDMA = &ptrUartSciDriver->dmaInfo[UART_DMA_RX];

    /* Disable the UART Receive DMA mode */
    UartSci_disableRxDMA(ptrDriverEDMA->ptrHwCfg->ptrSCIRegs);

    /* Get the pointer to the EDMA configuration */
    ptrEDMACfg      = &ptrDriverEDMA->config;

    /* Populate the remaining fields in the EDMA configuration: */
    ptrEDMACfg->paramSetConfig.destinationAddress = dstAddress;
    ptrEDMACfg->paramSetConfig.bCount = numBytes;

    /* Is Polling Mode enabled? */
    if (BTRUE == pollingMode)
    {
        /* Polling Mode: There is no need to register the callback function */
        ptrEDMACfg->transferCompletionCallbackFxn    = NULL;
        ptrEDMACfg->transferCompletionCallbackFxnArg = 0U;
    }
    else
    {
        /* Blocking Mode: Register the callback function to be invoked on interrupt */
        ptrEDMACfg->transferCompletionCallbackFxn    = UartSci_RxEDMACallbackFxn;
        ptrEDMACfg->transferCompletionCallbackFxnArg = (uintptr_t)ptrUartSciDriver;
    }

    /* Configure the EDMA Channel: */
    errCode = EDMA_configChannel(ptrDriverEDMA->ptrHwCfg->swCfg.edmaHandle, ptrEDMACfg, BTRUE);
    if (EDMA_NO_ERROR != errCode)
    {
        /* Error: Unable to configure the EDMA channel. Setup the return value */
        UART_drv_log1 ("Error: UART EDMA configuration failed [Error code %d]\n", errCode);
        retVal = errCode;
        goto exit;
    }

    /* Enable the UART to operate in receive DMA mode */
    UartSci_enableRxDMA(ptrDriverEDMA->ptrHwCfg->ptrSCIRegs);

    /* Are we operating in polled mode or blocking mode? */
    if (BTRUE == pollingMode)
    {
        /* Polling Mode: Poll for the DMA completion status here */
        while (BFALSE == isTransferComplete)
        {
            errCode = EDMA_isTransferComplete(ptrDriverEDMA->ptrHwCfg->swCfg.edmaHandle,
                                              ptrDriverEDMA->ptrHwCfg->rxDMARequestLine,
                                              &isTransferComplete);

            /* Sanity Check: Ensure that there is no EDMA Error. */
            UART_osalAssert (errCode == EDMA_NO_ERROR);
        }

        /* Write operation for the driver is complete */
        ptrUartSciDriver->readCount = ptrUartSciDriver->readSize;
        ptrUartSciDriver->readSize  = 0U;
    }
    else
    {
        /* Blocking Mode: Fall through and allow the UART Driver to pend on the semaphore
         * The Rx DMA interrupt has been enabled which will post the semaphore. */
    }

    /* We have successfully completed the EDMA operation */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initiate the DMA for the UART transmit
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *  @param[in]  srcAddress
 *      Source address of the buffer which is to be transmitted
 *  @param[in]  numBytes
 *      Number of bytes to be transmitted
 *  @param[in]  pollingMode
 *      Set to BTRUE to indicate that we need to poll for completion else
 *      set to BFALSE
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error Code  -   <0
 */
int32_t UartSci_initiateTxEDMA
(
    UartSci_Driver* ptrUartSciDriver,
    uint32_t        srcAddress,
    uint32_t        numBytes,
    bool            pollingMode
)
{
    UartSci_DriverEDMA*     ptrDriverEDMA;
    EDMA_channelConfig_t*   ptrEDMACfg;
    EDMA_paramConfig_t*     ptrDummyEDMACfg;
    int32_t                 retVal = -1;
    int32_t                 errCode;
    bool                    isTransferComplete = BFALSE;

    /* Get the DMA Block: */
    ptrDriverEDMA = &ptrUartSciDriver->dmaInfo[UART_DMA_TX];

    /* Disable the UART Transmit DMA mode */
    UartSci_disableTxDMA(ptrDriverEDMA->ptrHwCfg->ptrSCIRegs);

    /* Get the pointer to the EDMA configuration */
    ptrEDMACfg      = &ptrDriverEDMA->config;
    ptrDummyEDMACfg = &ptrDriverEDMA->dummyParamConfig;

    /* Populate the remaining fields in the EDMA configuration: */
    ptrEDMACfg->paramSetConfig.sourceAddress = srcAddress;
    ptrEDMACfg->paramSetConfig.bCount        = numBytes;

    /* Configure the EDMA Channel: */
    errCode = EDMA_configChannel(ptrDriverEDMA->ptrHwCfg->swCfg.edmaHandle, ptrEDMACfg, BTRUE);
    if (EDMA_NO_ERROR != errCode)
    {
        /* Error: Unable to configure the EDMA channel. Setup the return value */
        UART_drv_log1 ("Error: UART EDMA configuration failed [Error code %d]\n", errCode);
        retVal = errCode;
        goto exit;
    }

    /* Is Polling Mode enabled? */
    if (BTRUE == pollingMode)
    {
        /* Polling Mode: There is no need to register the callback function */
        ptrDummyEDMACfg->transferCompletionCallbackFxn    = NULL;
        ptrDummyEDMACfg->transferCompletionCallbackFxnArg = 0U;
    }
    else
    {
        /* Blocking Mode: Register the callback function to be invoked on interrupt */
        ptrDummyEDMACfg->transferCompletionCallbackFxn    = UartSci_TxEDMACallbackFxn;
        ptrDummyEDMACfg->transferCompletionCallbackFxnArg = (uintptr_t)ptrUartSciDriver;
    }

    /* Configure the PaRAM set: */
    errCode = EDMA_configParamSet (ptrDriverEDMA->ptrHwCfg->swCfg.edmaHandle,
                                   ptrDriverEDMA->ptrHwCfg->swCfg.paramSetId,
                                   ptrDummyEDMACfg);
    if (EDMA_NO_ERROR != errCode)
    {
        /* Error: Unable to configure the param set. Setup the return value */
        UART_drv_log1 ("Error: UART EDMA configuration failed [Error code %d]\n", errCode);
        retVal = errCode;
        goto exit;
    }

    /* Link the PaRAM sets: */
    errCode = EDMA_linkParamSets (ptrDriverEDMA->ptrHwCfg->swCfg.edmaHandle,
                                  ptrEDMACfg->channelId,
                                  ptrDriverEDMA->ptrHwCfg->swCfg.paramSetId);
    if (EDMA_NO_ERROR != errCode)
    {
        /* Error: Unable to link the param set. Setup the return value */
        UART_drv_log1 ("Error: UART EDMA configuration failed [Error code %d]\n", errCode);
        retVal = errCode;
        goto exit;
    }

    /* Enable the UART to operate in Transmit DMA mode */
    UartSci_enableTxDMA(ptrDriverEDMA->ptrHwCfg->ptrSCIRegs);

    /* Are we operating in polled mode or blocking mode? */
    if (BTRUE == pollingMode)
    {
        /* Polling Mode: Poll for the DMA completion status here */
        while (BFALSE == isTransferComplete)
        {
            errCode = EDMA_isTransferComplete(ptrDriverEDMA->ptrHwCfg->swCfg.edmaHandle,
                                              ptrDriverEDMA->ptrHwCfg->txDMARequestLine,
                                              &isTransferComplete);

            /* Sanity Check: Ensure that there is no EDMA Error. */
            UART_osalAssert (EDMA_NO_ERROR == errCode);
        }

        /* Write operation for the driver is complete */
        ptrUartSciDriver->writeCount = ptrUartSciDriver->writeSize;
        ptrUartSciDriver->writeSize  = 0U;
    }
    else
    {
        /* Blocking Mode: Fall through and allow the UART Driver to pend on the semaphore
         * The Tx DMA interrupt has been enabled which will post the semaphore. */
    }

    /* We have successfully completed the EDMA operation */
    retVal = 0;

exit:
    return retVal;
}


/**
 *  @b Description
 *  @n
 *      The function is used to initialize the EDMA info
 *
 *  @param[in]  ptrDriverEDMA
 *      Pointer to the EDMA Info
 *  @param[in]  ptrHwCfg
 *      Pointer to the UART SCI Hardware configuration
 *  @param[in]  fTx
 *      Indicate whether it is tx channel
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 */
static void UartSci_initEDMAInfo(UartSci_DriverEDMA* ptrDriverEDMA,
                                 UartSci_HwCfg* ptrHwCfg,
                                 bool isTx)
{

    uint8_t chanIdTcc;
    EDMA_channelConfig_t*   ptrEDMACfg;
    EDMA_paramConfig_t*     ptrDummyEDMACfg;

    /* Initialize the EDMA Info */
    memset ((void *)ptrDriverEDMA, 0, sizeof(UartSci_DriverEDMA));

    /* Populate the DMA Information Block: */
    ptrDriverEDMA->ptrHwCfg      = ptrHwCfg;

    /* Get the pointer to the EDMA configuration */
    ptrEDMACfg      = &ptrDriverEDMA->config;
    ptrDummyEDMACfg = &ptrDriverEDMA->dummyParamConfig;

    /*********************************************************************************
     * Initialize the EDMA configuration:
     * - We dont initialize all the fields here but most of them dont change in
     *   the data path.
     *********************************************************************************/
    if (isTx)
    {
        chanIdTcc = ptrHwCfg->txDMARequestLine;
        ptrEDMACfg->paramSetConfig.destinationAddress = (uint32_t)&ptrHwCfg->ptrSCIRegs->SCITD;
        ptrEDMACfg->paramSetConfig.sourceBindex       = 1U;
        ptrEDMACfg->paramSetConfig.destinationBindex  = 0U;
        ptrEDMACfg->paramSetConfig.isFinalTransferInterruptEnabled = BFALSE;
    }
    else
    {
        chanIdTcc = ptrHwCfg->rxDMARequestLine;
        ptrEDMACfg->paramSetConfig.sourceAddress       = (uint32_t)&ptrHwCfg->ptrSCIRegs->SCIRD;
        ptrEDMACfg->paramSetConfig.sourceBindex        = 0U;
        ptrEDMACfg->paramSetConfig.destinationBindex   = 1U;
        ptrEDMACfg->paramSetConfig.isFinalTransferInterruptEnabled = BTRUE;
    }

    ptrEDMACfg->channelId                                             = chanIdTcc;
    ptrEDMACfg->channelType                                           = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
    ptrEDMACfg->paramId                                               = chanIdTcc;
    ptrEDMACfg->eventQueueId                                          = 0;
    ptrEDMACfg->transferCompletionCallbackFxn                         = NULL;
    ptrEDMACfg->transferCompletionCallbackFxnArg                      = 0U;
    ptrEDMACfg->paramSetConfig.aCount                                 = 1U;
    ptrEDMACfg->paramSetConfig.cCount                                 = 1U;
    ptrEDMACfg->paramSetConfig.bCountReload                           = 0U;
    ptrEDMACfg->paramSetConfig.sourceCindex                           = 0U;
    ptrEDMACfg->paramSetConfig.destinationCindex                      = 0U;
    ptrEDMACfg->paramSetConfig.linkAddress                            = EDMA_NULL_LINK_ADDRESS;
    ptrEDMACfg->paramSetConfig.transferType                           = (uint8_t)EDMA3_SYNC_A;
    ptrEDMACfg->paramSetConfig.transferCompletionCode                 = chanIdTcc;
    ptrEDMACfg->paramSetConfig.sourceAddressingMode                   = (uint8_t)EDMA3_ADDRESSING_MODE_LINEAR;
    ptrEDMACfg->paramSetConfig.destinationAddressingMode              = (uint8_t)EDMA3_ADDRESSING_MODE_LINEAR;
    ptrEDMACfg->paramSetConfig.fifoWidth                              = (uint8_t)EDMA3_FIFO_WIDTH_128BIT;
    ptrEDMACfg->paramSetConfig.isStaticSet                            = BFALSE;
    ptrEDMACfg->paramSetConfig.isEarlyCompletion                      = BFALSE;
    ptrEDMACfg->paramSetConfig.isIntermediateTransferInterruptEnabled = BFALSE;
    ptrEDMACfg->paramSetConfig.isFinalChainingEnabled                 = BFALSE;
    ptrEDMACfg->paramSetConfig.isIntermediateChainingEnabled          = BFALSE;

    if (isTx)
    {
        /* Copy over the EDMA configuration into the dummy configuration:
        *  This will reduce the code size instead of explicitly initializing all
        *  the fields. We will now modify only the fields which are different */
        memcpy ((void *)&ptrDummyEDMACfg->paramSetConfig, (void*)&ptrEDMACfg->paramSetConfig,
                sizeof(EDMA_paramSetConfig_t));

        /* Modify the fields which are different between the EDMA Transfers:-
        * - Set the bCount to be 0 to indicate that this is a Dummy Transfer
        * - Generate an interrupt once the final transfer for Tx is done. */
        ptrDummyEDMACfg->paramSetConfig.bCount                          = 0U;
        ptrDummyEDMACfg->paramSetConfig.isFinalTransferInterruptEnabled = BTRUE;
    }
}

/**
 *  @b Description
 *  @n
 *      The function is used to open the EDMA
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART SCI Driver
 *  @param[in]  ptrHwCfg
 *      Pointer to the UART SCI Hardware configuration
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t UartSci_openEDMA (UartSci_Driver* ptrUartSciDriver, UartSci_HwCfg* ptrHwCfg)
{
    /* Do we need to use the EDMA? */
    if (ptrHwCfg->swCfg.edmaHandle != NULL)
    {
        /* Extract and setup transmit EDMA Information Block */
        UartSci_initEDMAInfo(&ptrUartSciDriver->dmaInfo[UART_DMA_TX],
                             ptrHwCfg, BTRUE);

        /* Extract and setup receive EDMA Information Block */
        UartSci_initEDMAInfo(&ptrUartSciDriver->dmaInfo[UART_DMA_RX],
                             ptrHwCfg, BFALSE);

        /* Register the DMA Information Block with the UART Driver: */
        ptrUartSciDriver->isDMAEnabled = BTRUE;

    }
    else
    {
        ptrUartSciDriver->isDMAEnabled = BFALSE;
    }

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the EDMA for the specific UART Instance
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART driver for which the DMA is being closed
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t UartSci_closeEDMA (UartSci_Driver* ptrUartSciDriver)
{
    int32_t              retVal = -1;
    UartSci_DriverEDMA*  ptrDriverEDMA;

    /* Were we using the DMA? */
    if (ptrUartSciDriver->isDMAEnabled)
    {
        /* Disable the transmit EDMA channel */
        ptrDriverEDMA = &ptrUartSciDriver->dmaInfo[UART_DMA_TX];
        retVal = EDMA_disableChannel(ptrDriverEDMA->ptrHwCfg->swCfg.edmaHandle,
                                     ptrDriverEDMA->ptrHwCfg->txDMARequestLine,
                                     (uint8_t)EDMA3_CHANNEL_TYPE_DMA);
        if (retVal != EDMA_NO_ERROR)
        {
            goto exit;
        }

        /* Disable the receive EDMA channel */
        ptrDriverEDMA = &ptrUartSciDriver->dmaInfo[UART_DMA_RX];
        retVal = EDMA_disableChannel(ptrDriverEDMA->ptrHwCfg->swCfg.edmaHandle,
                                     ptrDriverEDMA->ptrHwCfg->rxDMARequestLine,
                                     (uint8_t)EDMA3_CHANNEL_TYPE_DMA);
        if (retVal != EDMA_NO_ERROR)
        {
            goto exit;
        }

        /* Deregister the DMA Information: */
        ptrUartSciDriver->isDMAEnabled = BFALSE;
        retVal = 0;
    }
exit:
    return retVal;
}



