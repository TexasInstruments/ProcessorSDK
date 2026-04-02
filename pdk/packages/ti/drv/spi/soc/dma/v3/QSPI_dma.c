/**
 *  \file   QSPI_dma.c
 *
 *  \brief  EDMA based QSPI driver for IP verion 1.
 *
 *   This file contains the EDMA driver APIs for QSPI.
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
#include <ti/drv/spi/soc/SPI_soc.h>
#include <ti/drv/edma/edma.h>

#define QSPI_EVENTQUE                    (0U)
#define QSPI_DMA_XFER_SIZE_ALIGN         (4U)

#define QSPI_DMA_MAX_XFER_CHUNK_SIZE (31U * 1024U)

/* DMA functions */
static void QSPI_dmaIsrHandler (uintptr_t appData, uint8_t tcc);

int32_t QSPI_dmaConfig(SPI_Handle handle);
int32_t QSPI_dmaTransfer(SPI_Handle handle, const SPI_Transaction *transaction);
int32_t QSPI_dmaFreeChannel(SPI_Handle handle);

static void    QSPI_edmaParamInit(EDMA_paramSetConfig_t *param, uint8_t tcc, uint8_t xferType);
static int32_t QSPI_edmaChannelConfig(EDMA_Handle hEdma,
                                        uint8_t chId,
                                        uint8_t queueId,
                                        uint8_t xferType);
static int32_t QSPI_dmaMemcpy(SPI_Handle     handle,
                              uintptr_t      destBuf,
                              uintptr_t      srcBuf,
                              uint32_t       length);


int32_t QSPI_dmaConfig(SPI_Handle handle)
{
    QSPI_HwAttrs         *hwAttrs;
    int32_t               status  = SPI_STATUS_SUCCESS;
    int32_t               edmaStatus = EDMA_NO_ERROR;
    QSPI_v1_Object        *object;

    /* Get the pointer to the object and hwAttrs */
    object  = (QSPI_v1_Object *)handle->object;
    hwAttrs = (QSPI_HwAttrs*)handle->hwAttrs;

    /* Check EDMA handle is not NULL */
    if (NULL == hwAttrs->edmaHandle)
    {
        status = SPI_STATUS_ERROR;
    }

    if (SPI_STATUS_SUCCESS == status)
    {
        edmaStatus = QSPI_edmaChannelConfig((EDMA_Handle)hwAttrs->edmaHandle,
                                            hwAttrs->edmaChId,
                                            QSPI_EVENTQUE,
                                            EDMA3_SYNC_AB);
        if (EDMA_NO_ERROR != edmaStatus)
        {
            status = SPI_STATUS_ERROR;
        }
    }
    object->reminderBytes = 0;
    return status;
}

int32_t QSPI_dmaTransfer(SPI_Handle             handle,
                         const SPI_Transaction *transaction)
{
    QSPI_v1_Object        *object;
    QSPI_HwAttrs const    *hwAttrs;
    uintptr_t              dataPtr;
    int32_t                status = SPI_STATUS_SUCCESS;

    /* Get the pointer to the hwAttrs */
    object  = (QSPI_v1_Object *)handle->object;
    hwAttrs = (QSPI_HwAttrs *)handle->hwAttrs;

    dataPtr = ((uintptr_t)hwAttrs->memMappedBaseAddr + (uintptr_t)transaction->txBuf);

    if(SPI_TRANSACTION_TYPE_READ == object->transactionType)
    {
        uint32_t xferLen;
        if (transaction->count > QSPI_DMA_MAX_XFER_CHUNK_SIZE)
        {
            object->reminderBytes = transaction->count % QSPI_DMA_MAX_XFER_CHUNK_SIZE;
            xferLen = transaction->count - object->reminderBytes;
        }
        else
        {
            object->reminderBytes = 0;
            xferLen = transaction->count;
        }
        /* QSPI EDMA read transfer should be 4 byte aligned else will result in corrupted tranfer.
           Align xferLen and reminderBytes to 4 bytes. */
        xferLen = CSL_NEXT_MULTIPLE_OF(xferLen, QSPI_DMA_XFER_SIZE_ALIGN);
        object->reminderBytes = CSL_NEXT_MULTIPLE_OF(object->reminderBytes, QSPI_DMA_XFER_SIZE_ALIGN);

        /* RX Mode */
        status = QSPI_dmaMemcpy(handle,
                                ((uintptr_t)transaction->rxBuf),
                                ((uintptr_t)dataPtr),
                                xferLen);
    }
    else
    {
        object->reminderBytes = 0;
        /* TX Mode */
        status  = QSPI_dmaMemcpy(handle,
                                (uintptr_t)dataPtr,
                                (uintptr_t)transaction->rxBuf,
                                transaction->count);
    }

    return (status);
}

int32_t QSPI_dmaFreeChannel(SPI_Handle handle)
{
    QSPI_HwAttrs const *hwAttrs;
    int32_t             status = SPI_STATUS_SUCCESS;
    int32_t             edmaStatus;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = (QSPI_HwAttrs*)handle->hwAttrs;

    edmaStatus =  EDMA_disableChannel(hwAttrs->edmaHandle,
                                      hwAttrs->edmaChId,
                                      EDMA3_CHANNEL_TYPE_DMA);

    if (EDMA_NO_ERROR != edmaStatus)
    {
        status = SPI_STATUS_ERROR;
    }

    return (status);
}

static void QSPI_dmaIsrHandler (uintptr_t appData, uint8_t tcc)
{
    SPI_Handle            handle;
    QSPI_v1_Object       *object;
    QSPI_HwAttrs const   *hwAttrs;

    /* Get the pointer to the object and hwAttrs */
    handle   = (SPI_Handle)appData;
    object   = (QSPI_v1_Object*)handle->object;
    hwAttrs  = (QSPI_HwAttrs*)handle->hwAttrs;

    /* EDMA is done - disable the DMA channel */
    EDMA_disableChannel(hwAttrs->edmaHandle,
                        tcc,
                        EDMA3_CHANNEL_TYPE_DMA);

    if (0 == object->reminderBytes)
    {
        object->transaction->status = SPI_TRANSFER_COMPLETED;
        /* Call the transfer completion callback function */
        object->qspiParams.transferCallbackFxn(handle, object->transaction);
    }
    else
    {
        /* Initiate the last chunk of transfer. */
        uint32_t transferredBytes = object->transaction->count - object->reminderBytes;
        uintptr_t dataPtr = ((uintptr_t)hwAttrs->memMappedBaseAddr + (uintptr_t)object->transaction->txBuf);
        /* RX Mode */
        QSPI_dmaMemcpy(handle,
                       ((uintptr_t)object->transaction->rxBuf + transferredBytes),
                       ((uintptr_t)dataPtr + transferredBytes),
                       object->reminderBytes);
        object->reminderBytes = 0;
    }
}

static void QSPI_edmaParamInit(EDMA_paramSetConfig_t *param, uint8_t tcc, uint8_t xferType)
{
    memset(param, 0, sizeof(*param));

    param->transferType = xferType;

    param->destinationAddressingMode = EDMA3_ADDRESSING_MODE_LINEAR;
    param->sourceAddressingMode = EDMA3_ADDRESSING_MODE_LINEAR;
    param->fifoWidth = 0;

    param->transferCompletionCode = tcc;

    param->isEarlyCompletion = BFALSE;
    param->isFinalTransferInterruptEnabled = BFALSE;
    param->isIntermediateChainingEnabled = BFALSE;
    param->isStaticSet = BFALSE;
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


static int32_t QSPI_edmaChannelConfig(EDMA_Handle hEdma,
                                      uint8_t chId,
                                      uint8_t queueId,
                                      uint8_t xferType)
{
    int32_t status;
    EDMA_channelConfig_t    chConfig;

    chConfig.channelType  = EDMA3_CHANNEL_TYPE_DMA;
    chConfig.channelId    = chId;
    chConfig.eventQueueId = queueId;
    chConfig.paramId      = chId;

    QSPI_edmaParamInit(&chConfig.paramSetConfig, chId, xferType);

    chConfig.paramSetConfig.isFinalTransferInterruptEnabled = BFALSE;

    chConfig.transferCompletionCallbackFxn    = NULL;
    chConfig.transferCompletionCallbackFxnArg = NULL;

    status = EDMA_configChannel(hEdma, &chConfig, BFALSE);

    return status;
}

static int32_t QSPI_dmaMemcpy(SPI_Handle     handle,
                              uintptr_t      destBuf,
                              uintptr_t      srcBuf,
                              uint32_t       length)

{
    QSPI_HwAttrs const   *hwAttrs;
    EDMA_paramConfig_t    paramSet = {0};
    int32_t               cIdx;
    int32_t               status = SPI_STATUS_SUCCESS;
    int32_t               edmaStatus;

    /* Get the pointer to the hwAttrs */
    hwAttrs = (QSPI_HwAttrs *)handle->hwAttrs;

    /* Initialize param set configuration */
    QSPI_edmaParamInit(&paramSet.paramSetConfig, hwAttrs->edmaChId, EDMA3_SYNC_AB);

    /* Set source address */
    paramSet.paramSetConfig.sourceAddress = (uint32_t)srcBuf;

    /* set destination address */
    paramSet.paramSetConfig.destinationAddress = (uint32_t)destBuf;

    /*
     * aCount holds the number of bytes in an array.
     * bCnt holds the number of such arrays to be transferred.
     */
    if (length < QSPI_DMA_MAX_XFER_CHUNK_SIZE)
    {
        paramSet.paramSetConfig.aCount = 1;
        paramSet.paramSetConfig.bCount = (uint16_t)(length);
    }
    else
    {
        paramSet.paramSetConfig.aCount = (uint16_t)(QSPI_DMA_MAX_XFER_CHUNK_SIZE);
        paramSet.paramSetConfig.bCount = ((uint32_t)(length / QSPI_DMA_MAX_XFER_CHUNK_SIZE));
    }

    /* cCnt holds the number of frames of aCnt*bBcnt bytes to be transferred */

    paramSet.paramSetConfig.cCount = (uint16_t)1;
    /* cIdx will not be used as the cCount is always set to 1. */
    cIdx = (int32_t)paramSet.paramSetConfig.bCount;

    /**
     * Be Careful !!!
     * Valid values for SRCBIDX/DSTBIDX are between -32768 and 32767
     * Valid values for SRCCIDX/DSTCIDX are between -32768 and 32767
     */
    paramSet.paramSetConfig.sourceBindex      = (int32_t)paramSet.paramSetConfig.aCount;
    paramSet.paramSetConfig.destinationBindex = (int32_t)paramSet.paramSetConfig.aCount;
    paramSet.paramSetConfig.sourceCindex      = cIdx;
    paramSet.paramSetConfig.destinationCindex = cIdx;

    /* Linking transfers in EDMA3 are not used. */
    paramSet.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;

    paramSet.paramSetConfig.bCountReload = paramSet.paramSetConfig.bCount;

    /* EDMA3_DRV_OPT_FIELD_TCINTEN */
    paramSet.paramSetConfig.isFinalTransferInterruptEnabled = BTRUE;

    paramSet.transferCompletionCallbackFxn = QSPI_dmaIsrHandler;
    paramSet.transferCompletionCallbackFxnArg = (uintptr_t)handle;

    /* Write param set */
    edmaStatus = EDMA_configParamSet(hwAttrs->edmaHandle,
                                     hwAttrs->edmaChId,
                                     &paramSet);

    if (EDMA_NO_ERROR == edmaStatus)
    {
        /* Enable the EDMA channel */
        edmaStatus =  EDMA_enableChannel(hwAttrs->edmaHandle,
                                         hwAttrs->edmaChId,
                                         EDMA3_CHANNEL_TYPE_DMA);
    }

    if (EDMA_NO_ERROR == edmaStatus)
    {
        /* Start the transfer */
        edmaStatus =  EDMA_startTransfer(hwAttrs->edmaHandle,
                                         hwAttrs->edmaChId,
                                         EDMA3_CHANNEL_TYPE_DMA);
    }

    if (EDMA_NO_ERROR != edmaStatus)
    {
        status = SPI_STATUS_ERROR;
    }

    return (status);
}
