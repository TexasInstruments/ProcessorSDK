/**
 *  \file   MIBSPI_dma.c
 *
 *  \brief  EDMA based MIBSPI driver for IP verion 3
 *
 */

/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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

#include <string.h>
#include <ti/csl/soc.h>
#include <ti/drv/mibspi/MIBSPI.h>
#include <ti/drv/mibspi/src/mibspi_priv.h>
#include <ti/drv/mibspi/src/mibspi_utils.h>

/**<  Transmit EDMA channel event queue number                            */
#define MIBSPI_TXEVENTQUE                  (0U)

/**<  Receive EDMA channel event queue number                             */
#define MIBSPI_RXEVENTQUE                  (1U)


static void MIBSPI_edmaCallback(MibSpiDriver_Object*    ptrMibSpiDriver);
static int32_t MIBSPI_edmaChannelConfig(EDMA_Handle hEdma,
                                        uint8_t chId,
                                        uint8_t queueId,
                                        uint8_t xferType);
static void MIBSPI_edmaDoNothing(uintptr_t appData, uint8_t tcc);
static void MIBSPI_edmaParamInit(EDMA_paramSetConfig_t *param, uint8_t tcc, uint8_t xferType);
static int32_t MIBSPI_edmaRamTransfer(MibSpiDriver_Object*    ptrMibSpiDriver,
                                   MibSpi_dmaXferInfo_t *xferInfo);
static void MIBSPI_edmaRamUpdateRxParams(const MibSpiDriver_Object*    ptrMibSpiDriver,
                                         const MibSpi_dmaXferAddrInfo_t * xferAddrInfo,
                                         const MibSpi_dmaXferSizeInfo_t * xferSizeInfo,
                                         uint32_t dmaReqLine,
                                         EDMA_paramConfig_t   * rxParamSet);
static void MIBSPI_edmaRamUpdateTxParams(const MibSpiDriver_Object*    ptrMibSpiDriver,
                                         const MibSpi_dmaXferAddrInfo_t * xferAddrInfo,
                                         const MibSpi_dmaXferSizeInfo_t * xferSizeInfo,
                                         uint32_t dmaReqLine,
                                         EDMA_paramConfig_t   *txParamSet);
static int32_t MIBSPI_edmaRegTransfer(MibSpiDriver_Object*    ptrMibSpiDriver,
                                      MibSpi_dmaXferInfo_t *xferInfo);
static void MIBSPI_edmaRegUpdateRxParams(const MibSpiDriver_Object*    ptrMibSpiDriver,
                                         const MibSpi_dmaXferAddrInfo_t * xferAddrInfo,
                                         const MibSpi_dmaXferSizeInfo_t * xferSizeInfo,
                                         uint32_t dmaReqLine,
                                         EDMA_paramConfig_t   * rxParamSet);
static void MIBSPI_edmaRegUpdateTxParams(const MibSpiDriver_Object*    ptrMibSpiDriver,
                                         const MibSpi_dmaXferAddrInfo_t * xferAddrInfo,
                                         const MibSpi_dmaXferSizeInfo_t * xferSizeInfo,
                                         uint32_t dmaReqLine,
                                         EDMA_paramConfig_t   *txParamSet);
static void MIBSPI_edmaRxIsrHandler(uintptr_t appData, uint8_t tcc);
static void MIBSPI_edmaTxIsrHandler(uintptr_t appData, uint8_t tcc);


int32_t MIBSPI_dmaConfig(MIBSPI_Handle handle, uint32_t dmaReqLine)
{
    MibSpi_HwCfg const   *hwAttrs;
    int32_t              spiStatus = MIBSPI_STATUS_SUCCESS;
    MibSpiDriver_Object*    ptrMibSpiDriver = NULL;
    MIBSPI_Config*             ptrSPIConfig;
    int32_t edmaStatus = EDMA_NO_ERROR;

    /* Get the SPI driver Configuration: */
    ptrSPIConfig = (MIBSPI_Config*)handle;

    ptrMibSpiDriver = (MibSpiDriver_Object*)ptrSPIConfig->object;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = ptrMibSpiDriver->ptrHwCfg;


    /* Check EDMA handle is not NULL */
    if (ptrMibSpiDriver->params.dmaHandle == NULL)
    {
        spiStatus = MIBSPI_STATUS_ERROR;
    }

    if (dmaReqLine >= hwAttrs->numDmaReqLines)
    {
        spiStatus = MIBSPI_STATUS_ERROR;
    }
    /* RX Section Configuration */
    if (MIBSPI_STATUS_SUCCESS == spiStatus)
    {

        edmaStatus = MIBSPI_edmaChannelConfig(ptrMibSpiDriver->params.dmaHandle,
                                              hwAttrs->dmaReqlineCfg[dmaReqLine].rxDmaReqLine,
                                              MIBSPI_RXEVENTQUE,
                                              EDMA3_SYNC_AB);
        /* TODO:Acquire the  PaRAM entries used for EDMA transfers linking */
        if (EDMA_NO_ERROR != edmaStatus)
        {
            spiStatus = MIBSPI_STATUS_ERROR;
        }
    }
    if (MIBSPI_STATUS_SUCCESS == spiStatus)
    {
        edmaStatus = MIBSPI_edmaChannelConfig(ptrMibSpiDriver->params.dmaHandle, 
                                              hwAttrs->dmaReqlineCfg[dmaReqLine].txDmaReqLine,
                                              MIBSPI_TXEVENTQUE,
                                              EDMA3_SYNC_AB);
        /* TODO:Acquire the  PaRAM entries used for EDMA transfers linking */
        if (EDMA_NO_ERROR != edmaStatus)
        {
            spiStatus = MIBSPI_STATUS_ERROR;
        }
    }
    return(spiStatus);
}

int32_t MIBSPI_dmaTransfer(MIBSPI_Handle handle, MibSpi_dmaXferInfo_t *xferInfo)
{
    MibSpiDriver_Object*    ptrMibSpiDriver = NULL;
    MIBSPI_Config*             ptrSPIConfig;
    int32_t spiStatus = MIBSPI_STATUS_SUCCESS;

    /* Get the SPI driver Configuration: */
    ptrSPIConfig = (MIBSPI_Config*)handle;

    ptrMibSpiDriver = (MibSpiDriver_Object*)ptrSPIConfig->object;

    Mibspi_assert(ptrMibSpiDriver->params.dmaHandle != NULL);

    if (ptrMibSpiDriver->params.compatibilityMode)
    {
        spiStatus = MIBSPI_edmaRegTransfer(ptrMibSpiDriver, xferInfo);
    }
    else
    {
        spiStatus = MIBSPI_edmaRamTransfer(ptrMibSpiDriver, xferInfo);
    }
    return spiStatus;
}


int32_t MIBSPI_dmaFreeChannel(const MIBSPI_Handle handle, uint32_t dmaReqLine)
{
    MibSpi_HwCfg const   *hwAttrs;
    MibSpiDriver_Object*    ptrMibSpiDriver = NULL;
    MIBSPI_Config*             ptrSPIConfig;
    EDMA_paramConfig_t dummyParamSet;
    int32_t edmaStatus, spiStatus = MIBSPI_STATUS_SUCCESS;

    /* Get the SPI driver Configuration: */
    ptrSPIConfig = (MIBSPI_Config*)handle;

    ptrMibSpiDriver = (MibSpiDriver_Object*)ptrSPIConfig->object;

    hwAttrs = ptrMibSpiDriver->ptrHwCfg;

    Mibspi_assert(ptrMibSpiDriver->params.dmaHandle != NULL);

    Mibspi_assert(dmaReqLine < hwAttrs->numDmaReqLines);

    if (ptrMibSpiDriver->params.compatibilityMode)
    {
        /* Dummy paramSet Configuration */
        MIBSPI_edmaParamInit(&dummyParamSet.paramSetConfig, ptrMibSpiDriver->params.edmaLinkParamId, EDMA3_SYNC_A);

        /* Unlink and free the channels */
        edmaStatus = EDMA_configParamSet(ptrMibSpiDriver->params.dmaHandle,
                                         ptrMibSpiDriver->params.edmaLinkParamId,
                                         &dummyParamSet);

        if (EDMA_NO_ERROR != edmaStatus)
        {
            spiStatus = MIBSPI_STATUS_ERROR;
        }
    }
    if (MIBSPI_STATUS_SUCCESS == spiStatus)
    {
        edmaStatus = EDMA_disableChannel(ptrMibSpiDriver->params.dmaHandle, hwAttrs->dmaReqlineCfg[dmaReqLine].rxDmaReqLine, EDMA3_CHANNEL_TYPE_DMA);
        if (EDMA_NO_ERROR != edmaStatus)
        {
            spiStatus = MIBSPI_STATUS_ERROR;
        }
    }
    if (MIBSPI_STATUS_SUCCESS == spiStatus)
    {
        edmaStatus = EDMA_disableChannel(ptrMibSpiDriver->params.dmaHandle, hwAttrs->dmaReqlineCfg[dmaReqLine].txDmaReqLine, EDMA3_CHANNEL_TYPE_DMA);
        if (EDMA_NO_ERROR != edmaStatus)
        {
            spiStatus = MIBSPI_STATUS_ERROR;
        }
    }
    return spiStatus;
}

static void MIBSPI_edmaParamInit(EDMA_paramSetConfig_t *param, uint8_t tcc, uint8_t xferType)
{
    memset(param, 0, sizeof(*param));

    param->transferType = xferType;

    param->destinationAddressingMode = EDMA3_ADDRESSING_MODE_LINEAR;
    param->sourceAddressingMode = EDMA3_ADDRESSING_MODE_LINEAR;
    param->fifoWidth = 0;

    param->transferCompletionCode = tcc;

    param->isEarlyCompletion = false;
    param->isFinalTransferInterruptEnabled = false;
    param->isIntermediateChainingEnabled = false;
    param->isStaticSet = false;
    param->linkAddress = EDMA_NULL_LINK_ADDRESS;

    param->sourceAddress = 0;
    param->destinationAddress = 0;

    param->aCount = 0;
    
    param->bCount = 0;
    param->sourceBindex = 0;
    param->destinationBindex = 0;
    param->bCountReload = 0;

    param->cCount = 0;
    param->sourceCindex = 0;
    param->destinationCindex = 0;
}


static int32_t MIBSPI_edmaChannelConfig(EDMA_Handle hEdma,
                                        uint8_t chId,
                                        uint8_t queueId,
                                        uint8_t xferType)
{
    int32_t status;
    EDMA_channelConfig_t    chConfig;

    chConfig.channelType  = EDMA3_CHANNEL_TYPE_DMA;

    chConfig.channelId = chId;
    chConfig.eventQueueId = queueId;
    chConfig.paramId      = chId;
    
    MIBSPI_edmaParamInit(&chConfig.paramSetConfig, chId, xferType);

    chConfig.paramSetConfig.isFinalTransferInterruptEnabled = false;

    chConfig.transferCompletionCallbackFxn = NULL;
    chConfig.transferCompletionCallbackFxnArg = NULL;

    status = EDMA_configChannel(hEdma, &chConfig,false);
    return status;
}

static uint32_t MIBSPI_edmaMapTcc2DmaReqLine(const MibSpi_HwCfg *ptrHwCfg, uint32_t tcc, bool isRxCh)
{
    uint32_t i;
    uint32_t dmaReqLine;

    for (i = 0; i < ptrHwCfg->numDmaReqLines; i++)
    {
        if (isRxCh)
        {
            if (ptrHwCfg->dmaReqlineCfg[i].rxDmaReqLine == tcc)
            {
                break;
            }
        }
        else
        {
            if (ptrHwCfg->dmaReqlineCfg[i].txDmaReqLine == tcc)
            {
                break;
            }
        }
    }
    if (i < ptrHwCfg->numDmaReqLines)
    {
        dmaReqLine = i;
    }
    else
    {
        dmaReqLine = MIBSPI_INVALID_DMA_REQLINE;
    }
    return dmaReqLine;
}

static void MIBSPI_edmaRxIsrHandler(uintptr_t appData, uint8_t tcc)
{
    MibSpiDriver_Object*    ptrMibSpiDriver = NULL;
    int32_t status;
    uint32_t dmaReqLine;

    /* Get the pointer to the object and hwAttrs */
    ptrMibSpiDriver   = (MibSpiDriver_Object * )appData;

    dmaReqLine = MIBSPI_edmaMapTcc2DmaReqLine(ptrMibSpiDriver->ptrHwCfg, tcc, true);
    Mibspi_assert((dmaReqLine < ptrMibSpiDriver->ptrHwCfg->numDmaReqLines)
                  &&
                  (dmaReqLine < MIBSPI_UTILS_ARRAYSIZE(ptrMibSpiDriver->ptrHwCfg->dmaReqlineCfg)));
    ptrMibSpiDriver->dmaInfo[dmaReqLine].rxDmaIntCnt++;

    /* EDMA Rx is done disable the Rx DMA channel */
    status = EDMA_disableChannel(ptrMibSpiDriver->params.dmaHandle,
                                 tcc,
                                 EDMA3_CHANNEL_TYPE_DMA);

    Mibspi_assert(status == EDMA_NO_ERROR);

    if (MIBSPI_TX_EDMA_CALLBACK_OCCURED == ptrMibSpiDriver->transactionState.edmaCbCheck)
    {
        ptrMibSpiDriver->transactionState.edmaCbCheck = MIBSPI_NONE_EDMA_CALLBACK_OCCURED;

        /* Call the completion function */
        MIBSPI_edmaCallback(ptrMibSpiDriver);
    }
    else
    {
        ptrMibSpiDriver->transactionState.edmaCbCheck = MIBSPI_RX_EDMA_CALLBACK_OCCURED;
    }
}

static void MIBSPI_edmaTxIsrHandler(uintptr_t appData, uint8_t tcc)
{
    MibSpiDriver_Object*    ptrMibSpiDriver = NULL;
    int32_t status;
    uint32_t dmaReqLine;

    /* Get the pointer to the object and hwAttrs */
    ptrMibSpiDriver = (MibSpiDriver_Object*)appData;

    dmaReqLine = MIBSPI_edmaMapTcc2DmaReqLine(ptrMibSpiDriver->ptrHwCfg, tcc, false);
    Mibspi_assert((dmaReqLine < ptrMibSpiDriver->ptrHwCfg->numDmaReqLines)
                  &&
                  (dmaReqLine < MIBSPI_UTILS_ARRAYSIZE(ptrMibSpiDriver->ptrHwCfg->dmaReqlineCfg)));


    ptrMibSpiDriver->dmaInfo[dmaReqLine].txDmaIntCnt++;

    /* EDMA Rx is done disable the Rx DMA channel */
    status = EDMA_disableChannel(ptrMibSpiDriver->params.dmaHandle,
                                 tcc,
                                 EDMA3_CHANNEL_TYPE_DMA);
    Mibspi_assert(status == EDMA_NO_ERROR);

    if (MIBSPI_RX_EDMA_CALLBACK_OCCURED == ptrMibSpiDriver->transactionState.edmaCbCheck)
    {
        ptrMibSpiDriver->transactionState.edmaCbCheck = MIBSPI_NONE_EDMA_CALLBACK_OCCURED;

        /* Call the completion function */
        MIBSPI_edmaCallback(ptrMibSpiDriver);
    }
    else
    {
        ptrMibSpiDriver->transactionState.edmaCbCheck = MIBSPI_TX_EDMA_CALLBACK_OCCURED;
    }
}

static void MIBSPI_edmaDoNothing(uintptr_t appData, uint8_t tcc)
{
    /* No implementation required */
}

static void MIBSPI_edmaRegUpdateRxParams(const MibSpiDriver_Object*    ptrMibSpiDriver,
                                         const MibSpi_dmaXferAddrInfo_t * xferAddrInfo,
                                         const MibSpi_dmaXferSizeInfo_t * xferSizeInfo,
                                         uint32_t dmaReqLine,
                                         EDMA_paramConfig_t   * rxParamSet)
{
    MibSpi_HwCfg const   *hwAttrs;
    bool dummyRxXfer;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = ptrMibSpiDriver->ptrHwCfg;
    if (xferAddrInfo->daddr == NULL)
    {
        dummyRxXfer = true;
    }
    else
    {
        dummyRxXfer = false;
    }

    /* Receive param set configuration */
    MIBSPI_edmaParamInit(&rxParamSet->paramSetConfig, hwAttrs->dmaReqlineCfg[dmaReqLine].rxDmaReqLine, EDMA3_SYNC_A);

    /* Source address */
    rxParamSet->paramSetConfig.sourceAddress = xferAddrInfo->saddr;

    if (!dummyRxXfer)
    {
        /* destinationAddress is address of memory location named buffer.*/
        rxParamSet->paramSetConfig.destinationAddress = xferAddrInfo->daddr;
    }
    else
    {
        /* destinationAddress is address of memory location named buffer.*/
        rxParamSet->paramSetConfig.destinationAddress = CSL_locToGlobAddr((uintptr_t)(&ptrMibSpiDriver->rxScratchBuffer));
    }
    
    Mibspi_assert(xferSizeInfo->elemSize <= 2);
    rxParamSet->paramSetConfig.aCount = xferSizeInfo->elemSize;

    /* bCount holds the number of such arrays to be transferred.*/
    rxParamSet->paramSetConfig.bCount = xferSizeInfo->elemCnt;

    /* cCount holds the number of frames of aCount*bbCount bytes to be transferred.*/
    Mibspi_assert(xferSizeInfo->frameCnt == 1);
    rxParamSet->paramSetConfig.cCount = xferSizeInfo->frameCnt;

    /* The sourceBindex should not be incremented since it is a h/w register.*/
    rxParamSet->paramSetConfig.sourceBindex = 0;

    if(!dummyRxXfer)
    {
        /* The destinationBindex should be incremented aCount number of bytes.*/
        rxParamSet->paramSetConfig.destinationBindex = rxParamSet->paramSetConfig.aCount;
    }
    else
    {
        /* The destinationBindex should not be incremented.*/
        rxParamSet->paramSetConfig.destinationBindex = 0;
    }

    /* A sync Transfer Mode. */
    /* srCIdx and destinationCindex set to zero since ASYNC Mode is used.*/
    rxParamSet->paramSetConfig.sourceCindex = 0;
    rxParamSet->paramSetConfig.destinationCindex = 0;

    /* Linking transfers in EDMA3 are not used.*/
    rxParamSet->paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;

    rxParamSet->paramSetConfig.bCountReload = rxParamSet->paramSetConfig.bCount;


    /* EDMA3_DRV_OPT_FIELD_TCINTEN */
    rxParamSet->paramSetConfig.isFinalTransferInterruptEnabled = true;
    rxParamSet->transferCompletionCallbackFxn = MIBSPI_edmaRxIsrHandler;
    rxParamSet->transferCompletionCallbackFxnArg = (uintptr_t)ptrMibSpiDriver;

}

static void MIBSPI_edmaRegUpdateTxParams(const MibSpiDriver_Object*    ptrMibSpiDriver,
                                         const MibSpi_dmaXferAddrInfo_t * xferAddrInfo,
                                         const MibSpi_dmaXferSizeInfo_t * xferSizeInfo,
                                         uint32_t dmaReqLine,
                                         EDMA_paramConfig_t   *txParamSet)
{
    MibSpi_HwCfg const   *hwAttrs;
    bool dummyTxXfer;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = ptrMibSpiDriver->ptrHwCfg;
    if (xferAddrInfo->saddr == NULL)
    {
        dummyTxXfer = true;
    }
    else
    {
        dummyTxXfer = false;
    }


    /* Transmit param set configuration */
    /* transmit parameter set */
    MIBSPI_edmaParamInit(&txParamSet->paramSetConfig, hwAttrs->dmaReqlineCfg[dmaReqLine].txDmaReqLine, EDMA3_SYNC_A);

    if(!dummyTxXfer)
    {
        /* sourceAddress holds address of memory location buffer. */
        txParamSet->paramSetConfig.sourceAddress = xferAddrInfo->saddr;
    }
    else
    {
        /* sourceAddress holds address of memory location buffer. */
        txParamSet->paramSetConfig.sourceAddress = CSL_locToGlobAddr((uintptr_t) &ptrMibSpiDriver->txScratchBuffer);
    }
    /* destinationAddress holds address of SPIDAT1 register. */
    txParamSet->paramSetConfig.destinationAddress = xferAddrInfo->daddr;

    /* 8-bit word length */
    txParamSet->paramSetConfig.aCount = xferSizeInfo->elemSize;

    /* bCount holds the number of such arrays to be transferred. */
    txParamSet->paramSetConfig.bCount = xferSizeInfo->elemCnt;

    /* cCount holds the number of frames of aCount*bbCount bytes to be transferred. */
    txParamSet->paramSetConfig.cCount = xferSizeInfo->frameCnt;

    if(!dummyTxXfer)
    {
        /*
        ** The sourceBindex should be incremented by aCount number of bytes since the
        ** source used here is memory.
        */
        txParamSet->paramSetConfig.sourceBindex = txParamSet->paramSetConfig.aCount;
    }
    else
    {
        /* If Tx scratch buffer do not increment sourceBindex */
        txParamSet->paramSetConfig.sourceBindex = 0;
    }
    txParamSet->paramSetConfig.destinationBindex = 0;

    /* Async Transfer Mode is set in OPT.*/
    /* srCIdx and destinationCindex set to zero since ASYNC Mode is used. */
    txParamSet->paramSetConfig.sourceCindex = 0;
    txParamSet->paramSetConfig.destinationCindex = 0;

    /* Linking transfers in EDMA3 are not used. */
    txParamSet->paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
    txParamSet->paramSetConfig.bCountReload = txParamSet->paramSetConfig.bCount;


    /* EDMA3_DRV_OPT_FIELD_TCINTEN */
    txParamSet->paramSetConfig.isFinalTransferInterruptEnabled = true;
    txParamSet->transferCompletionCallbackFxn = MIBSPI_edmaTxIsrHandler;
    txParamSet->transferCompletionCallbackFxnArg = (uintptr_t)ptrMibSpiDriver;
}

static void MIBSPI_edmaCallback(MibSpiDriver_Object*    ptrMibSpiDriver)
{
    MIBSPI_dmaDoneCb(ptrMibSpiDriver->mibspiHandle);
}


static void MIBSPI_edmaRamUpdateRxParams(const MibSpiDriver_Object*    ptrMibSpiDriver,
                                         const MibSpi_dmaXferAddrInfo_t * xferAddrInfo,
                                         const MibSpi_dmaXferSizeInfo_t * xferSizeInfo,
                                         uint32_t dmaReqLine,
                                         EDMA_paramConfig_t   * rxParamSet)
{
    MibSpi_HwCfg const   *hwAttrs;
    bool dummyRxXfer;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = ptrMibSpiDriver->ptrHwCfg;
    if (xferAddrInfo->daddr == NULL)
    {
        dummyRxXfer = true;
    }
    else
    {
        dummyRxXfer = false;
    }

    /* Receive param set configuration */
    MIBSPI_edmaParamInit(&rxParamSet->paramSetConfig, hwAttrs->dmaReqlineCfg[dmaReqLine].rxDmaReqLine, EDMA3_SYNC_AB);

    rxParamSet->paramSetConfig.sourceAddress = xferAddrInfo->saddr;

    if (!dummyRxXfer)
    {
        /* destinationAddress is address of memory location named buffer.*/
        rxParamSet->paramSetConfig.destinationAddress = xferAddrInfo->daddr;
    }
    else
    {
        /* Source address */
        rxParamSet->paramSetConfig.destinationAddress = CSL_locToGlobAddr((uintptr_t) &ptrMibSpiDriver->rxScratchBuffer);
    }

    /* aCount holds the number of bytes in an array.*/
    rxParamSet->paramSetConfig.aCount = xferSizeInfo->elemSize;

    /* bCount holds the number of such arrays to be transferred.*/
    rxParamSet->paramSetConfig.bCount = xferSizeInfo->elemCnt;

    /* cCount holds the number of frames of aCount*bbCount bytes to be transferred.*/
    rxParamSet->paramSetConfig.cCount = xferSizeInfo->frameCnt;

    /* The sourceBindex is MIBSPI Ram and has index of 4.*/
    rxParamSet->paramSetConfig.sourceBindex = 4;

    if(!dummyRxXfer)
    {
        /* The destinationBindex should be incremented aCount number of bytes.*/
        rxParamSet->paramSetConfig.destinationBindex = rxParamSet->paramSetConfig.aCount;
    }
    else
    {
        /* The destinationBindex should not be incremented.*/
        rxParamSet->paramSetConfig.destinationBindex = 0;
    }

    /* AB sync Transfer Mode. */
    rxParamSet->paramSetConfig.sourceCindex = 0;
    rxParamSet->paramSetConfig.destinationCindex = rxParamSet->paramSetConfig.bCount * rxParamSet->paramSetConfig.aCount;

    /* Linking transfers in EDMA3 are not used.*/
    rxParamSet->paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;

    rxParamSet->paramSetConfig.bCountReload = rxParamSet->paramSetConfig.bCount;


    /* EDMA3_DRV_OPT_FIELD_TCINTEN */
    rxParamSet->paramSetConfig.isFinalTransferInterruptEnabled = true;
    rxParamSet->transferCompletionCallbackFxn = MIBSPI_edmaRxIsrHandler;
    rxParamSet->transferCompletionCallbackFxnArg = (uintptr_t) ptrMibSpiDriver;
}

static void MIBSPI_edmaRamUpdateTxParams(const MibSpiDriver_Object*    ptrMibSpiDriver,
                                         const MibSpi_dmaXferAddrInfo_t * xferAddrInfo,
                                         const MibSpi_dmaXferSizeInfo_t * xferSizeInfo,
                                         uint32_t dmaReqLine,
                                         EDMA_paramConfig_t   *txParamSet)
{
    MibSpi_HwCfg const   *hwAttrs;
    bool dummyTxXfer;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = ptrMibSpiDriver->ptrHwCfg;
    if (xferAddrInfo->saddr == NULL)
    {
        dummyTxXfer = true;
    }
    else
    {
        dummyTxXfer = false;
    }
    /* Transmit param set configuration */
    /* transmit parameter set */
    MIBSPI_edmaParamInit(&txParamSet->paramSetConfig, hwAttrs->dmaReqlineCfg[dmaReqLine].txDmaReqLine, EDMA3_SYNC_AB);

    if (!dummyTxXfer)
    {
        /* sourceAddress holds address of memory location buffer. */
        txParamSet->paramSetConfig.sourceAddress = xferAddrInfo->saddr;
    }
    else
    {
        txParamSet->paramSetConfig.sourceAddress = CSL_locToGlobAddr((uintptr_t)(&ptrMibSpiDriver->txScratchBuffer));
    }

    /* destinationAddress holds address of SPIDAT1 register. */
    txParamSet->paramSetConfig.destinationAddress = xferAddrInfo->daddr;

    /* aCount holds the number of bytes in an array. */
    txParamSet->paramSetConfig.aCount = xferSizeInfo->elemSize;

    /* bCount holds the number of such arrays to be transferred. */
    txParamSet->paramSetConfig.bCount = xferSizeInfo->elemCnt;

    /* cCount holds the number of frames of aCount*bbCount bytes to be transferred. */
    txParamSet->paramSetConfig.cCount = xferSizeInfo->frameCnt;

    /*
    ** The sourceBindex should be incremented by aCount number of bytes since the
    ** source used here is memory.
    */
    if(!dummyTxXfer)
    {
        txParamSet->paramSetConfig.sourceBindex = txParamSet->paramSetConfig.aCount;
    }
    else
    {
        txParamSet->paramSetConfig.sourceBindex = 0;
    }
    txParamSet->paramSetConfig.destinationBindex = 4;

    /* AB-sync Transfer Mode is set in OPT.*/
    txParamSet->paramSetConfig.sourceCindex = txParamSet->paramSetConfig.bCount * txParamSet->paramSetConfig.aCount;
    txParamSet->paramSetConfig.destinationCindex = 0;

    /* Linking transfers in EDMA3 are not used. */
    txParamSet->paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
    txParamSet->paramSetConfig.bCountReload = txParamSet->paramSetConfig.bCount;


    /* EDMA3_DRV_OPT_FIELD_TCINTEN */
    txParamSet->paramSetConfig.isFinalTransferInterruptEnabled = true;
    txParamSet->transferCompletionCallbackFxn = MIBSPI_edmaTxIsrHandler;
    txParamSet->transferCompletionCallbackFxnArg = (uintptr_t)ptrMibSpiDriver;

}

static int32_t MIBSPI_edmaRegTransfer(MibSpiDriver_Object*    ptrMibSpiDriver,
                                      MibSpi_dmaXferInfo_t *xferInfo)
{
    MibSpi_HwCfg const   *hwAttrs;
    int32_t              spiStatus = MIBSPI_STATUS_SUCCESS;
    int32_t              edmaStatus;
    EDMA_paramConfig_t rxParamSet;
    EDMA_paramConfig_t txParamSet;
    EDMA_paramConfig_t dummyParamSet;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = ptrMibSpiDriver->ptrHwCfg;

    ptrMibSpiDriver->transactionState.edmaCbCheck = MIBSPI_NONE_EDMA_CALLBACK_OCCURED;

    /* Update param sets based on the transaction parameters */
    MIBSPI_edmaRegUpdateRxParams(ptrMibSpiDriver, &xferInfo->rx, &xferInfo->size, 
                                 xferInfo->dmaReqLine, &rxParamSet);

    MIBSPI_edmaRegUpdateTxParams(ptrMibSpiDriver, &xferInfo->tx, &xferInfo->size, 
                                 xferInfo->dmaReqLine, &txParamSet);

    /* Dummy paramSet Configuration */
    MIBSPI_edmaParamInit(&dummyParamSet.paramSetConfig, ptrMibSpiDriver->params.edmaLinkParamId, EDMA3_SYNC_A);

    dummyParamSet.paramSetConfig.aCount     = 1;
    dummyParamSet.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
    dummyParamSet.paramSetConfig.isStaticSet     = true;
    dummyParamSet.paramSetConfig.isFinalTransferInterruptEnabled = true;
    dummyParamSet.transferCompletionCallbackFxn = &MIBSPI_edmaDoNothing;

    /* Now, write the PaRAM Set. */
    edmaStatus = EDMA_configParamSet(ptrMibSpiDriver->params.dmaHandle,
                                     ptrMibSpiDriver->params.edmaLinkParamId,
                                     &dummyParamSet);

    if (EDMA_NO_ERROR == edmaStatus)
    {
        /* Write Rx param set */
        edmaStatus = EDMA_configParamSet(ptrMibSpiDriver->params.dmaHandle,
                                         hwAttrs->dmaReqlineCfg[xferInfo->dmaReqLine].rxDmaReqLine,
                                         &rxParamSet);
    }
    
    if (EDMA_NO_ERROR == edmaStatus)
    {
        /* Write Tx param set */
        edmaStatus = EDMA_configParamSet(ptrMibSpiDriver->params.dmaHandle,
                                         hwAttrs->dmaReqlineCfg[xferInfo->dmaReqLine].txDmaReqLine,
                                         &txParamSet);
    }
    if (EDMA_NO_ERROR == edmaStatus)
    {
        /* Link the Dummy paramset */
        edmaStatus = EDMA_linkParamSets(ptrMibSpiDriver->params.dmaHandle,
                                        hwAttrs->dmaReqlineCfg[xferInfo->dmaReqLine].txDmaReqLine,
                                        ptrMibSpiDriver->params.edmaLinkParamId);
    }
    if (EDMA_NO_ERROR == edmaStatus)
    {
        /* Program the RX side */
        edmaStatus = EDMA_enableChannel(ptrMibSpiDriver->params.dmaHandle,
                                        hwAttrs->dmaReqlineCfg[xferInfo->dmaReqLine].rxDmaReqLine,
                                        EDMA3_CHANNEL_TYPE_DMA);
    }
    if (EDMA_NO_ERROR == edmaStatus)
    {
        /* Program the TX side */
        edmaStatus = EDMA_enableChannel(ptrMibSpiDriver->params.dmaHandle,
                                        hwAttrs->dmaReqlineCfg[xferInfo->dmaReqLine].txDmaReqLine,
                                        EDMA3_CHANNEL_TYPE_DMA);
    }

    if (EDMA_NO_ERROR == edmaStatus)
    {
        /* Enable SPI DMA for transaction */
        SPIIntEnable((uintptr_t)hwAttrs->ptrSpiRegBase, SPI_INT_DMA_ENABLE);
    }
    else
    {
        spiStatus = MIBSPI_STATUS_ERROR;
    
    }
    return spiStatus;
}

static int32_t MIBSPI_edmaRamTransfer(MibSpiDriver_Object*    ptrMibSpiDriver,
                                      MibSpi_dmaXferInfo_t *xferInfo)
{
    MibSpi_HwCfg const   *hwAttrs;
    int32_t              spiStatus = MIBSPI_STATUS_SUCCESS;
    int32_t              edmaStatus;
    EDMA_paramConfig_t rxParamSet;
    EDMA_paramConfig_t txParamSet;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = ptrMibSpiDriver->ptrHwCfg;

    Mibspi_assert(ptrMibSpiDriver->params.dmaHandle != NULL);

    ptrMibSpiDriver->transactionState.edmaCbCheck = MIBSPI_NONE_EDMA_CALLBACK_OCCURED;

    /* Update param sets based on the transaction parameters */
    MIBSPI_edmaRamUpdateRxParams(ptrMibSpiDriver, &xferInfo->rx, &xferInfo->size, 
                                 xferInfo->dmaReqLine, &rxParamSet);

    MIBSPI_edmaRamUpdateTxParams(ptrMibSpiDriver, &xferInfo->tx, &xferInfo->size, 
                                 xferInfo->dmaReqLine, &txParamSet);

    /* Write Rx param set */
    edmaStatus = EDMA_configParamSet(ptrMibSpiDriver->params.dmaHandle,
                                     hwAttrs->dmaReqlineCfg[xferInfo->dmaReqLine].rxDmaReqLine,
                                     &rxParamSet);

    if (EDMA_NO_ERROR == edmaStatus)
    {
        /* Write Tx param set */
        edmaStatus =  EDMA_configParamSet(ptrMibSpiDriver->params.dmaHandle,
                                          hwAttrs->dmaReqlineCfg[xferInfo->dmaReqLine].txDmaReqLine,
                                          &txParamSet);
    }
    if (EDMA_NO_ERROR == edmaStatus)
    {
        /* Program the RX side */
        edmaStatus =  EDMA_disableChannel(ptrMibSpiDriver->params.dmaHandle,
                                         hwAttrs->dmaReqlineCfg[xferInfo->dmaReqLine].rxDmaReqLine,
                                         EDMA3_CHANNEL_TYPE_DMA);
    }
    if (EDMA_NO_ERROR == edmaStatus)
    {
        /* Program the TX side */
        edmaStatus =  EDMA_disableChannel(ptrMibSpiDriver->params.dmaHandle,
                                         hwAttrs->dmaReqlineCfg[xferInfo->dmaReqLine].txDmaReqLine,
                                         EDMA3_CHANNEL_TYPE_DMA);
    }
    if (EDMA_NO_ERROR != edmaStatus)
    {
        spiStatus = MIBSPI_STATUS_ERROR;
    }
    return spiStatus;
}

int32_t MIBSPI_dmaStartTransfer(MIBSPI_Handle handle, uint32_t dmaReqLine)
{
    MibSpiDriver_Object*    ptrMibSpiDriver;
    int32_t              spiStatus = MIBSPI_STATUS_SUCCESS;
    int32_t              edmaStatus;
    MibSpi_HwCfg const   *hwAttrs;
    MIBSPI_Config*             ptrSPIConfig;


    /* Get the SPI driver Configuration: */
    ptrSPIConfig = (MIBSPI_Config*)handle;

    ptrMibSpiDriver = (MibSpiDriver_Object*)ptrSPIConfig->object;

    hwAttrs = ptrMibSpiDriver->ptrHwCfg;

    /* Program the RX side */
    edmaStatus =  EDMA_enableChannel(ptrMibSpiDriver->params.dmaHandle,
                                     hwAttrs->dmaReqlineCfg[dmaReqLine].rxDmaReqLine,
                                     EDMA3_CHANNEL_TYPE_DMA);
    if (EDMA_NO_ERROR == edmaStatus)
    {
        /* Program the TX side */
        edmaStatus =  EDMA_enableChannel(ptrMibSpiDriver->params.dmaHandle,
                                         hwAttrs->dmaReqlineCfg[dmaReqLine].txDmaReqLine,
                                         EDMA3_CHANNEL_TYPE_DMA);
    }
    if (EDMA_NO_ERROR != edmaStatus)
    {
        spiStatus = MIBSPI_STATUS_ERROR;
    }
    return spiStatus;
}
