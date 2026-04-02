/**
 *  \file   SPI_v0.c
 *
 *  \brief  Keystone SPI IP specific driver implementation.
 *
 *   This file contains the driver APIs for SPI.
 */

/*
 * Copyright (c) 2015 - 2019, Texas Instruments Incorporated
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

#include <stdint.h>
#include <stdbool.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/src/v0/SPI_v0.h>
#include <ti/csl/src/ip/spi/V0/spi.h>
#include <ti/drv/spi/src/SPI_osal.h>

#define SPI_POLLING_TIMEOUT_LOOP    (1000U)

/* SPITiva functions */
static void          SPI_close_v0(SPI_Handle handle);
static void          SPI_init_v0(SPI_Handle handle);
static SPI_Handle    SPI_open_v0(SPI_Handle handle, const SPI_Params *params);
static bool          SPI_transfer_v0(SPI_Handle handle,
                                          SPI_Transaction *transaction);
static void          SPI_transferCancel_v0(SPI_Handle handle);
static void          SPI_transferCallback_v0(SPI_Handle handle,
                                                  SPI_Transaction *transaction);
static int32_t       SPI_control_v0(SPI_Handle handle, uint32_t cmd, const void *arg);

/* SPI function table for SPI_v0 implementation */
const SPI_FxnTable SPI_FxnTable_v0 = {
    &SPI_close_v0,
    &SPI_control_v0,
    &SPI_init_v0,
    &SPI_open_v0,
    &SPI_transfer_v0,
    &SPI_transferCancel_v0,
    NULL
};

/*
 *  ======== SPI_close_v0 ========
 *  @pre    Function assumes that the handle is not NULL
 */
static void SPI_close_v0(SPI_Handle handle)
{
    SPI_v0_Object  *object = NULL;
    SPI_v0_HWAttrs const *hwAttrs = NULL;

    /* Input parameter validation */
    if (NULL != handle)
    {
    /* Get the pointer to the object and hwAttrs */
    hwAttrs = (const SPI_v0_HWAttrs *)handle->hwAttrs;
    object = (SPI_v0_Object*)handle->object;

    if(SPI_OPER_MODE_POLLING != object->operMode)
    {
        /* Destruct the Hwi */
        SPI_osalHardwareIntDestruct(object->hwi, hwAttrs->eventId);
    }

    /* Destruct the instance lock */
    SPI_osalDeleteBlockingLock(object->mutex);

    if(SPI_OPER_MODE_BLOCKING == object->operMode)
    {
        /* Destruct the semaphore */
        SPI_osalDeleteBlockingLock(object->transferComplete);
    }

#ifdef SPI_DMA_ENABLE
    if (BTRUE == hwAttrs->dmaMode)
    {
        SPI_dmaFreeChannel(handle);
    }
#endif

    object->isOpen = BFALSE;
    }
}

/*
 *  ======== SPI_receiveData_v0 ========
 *
 *  Get received data words from the RX FIFO based on the data size
 *
 *  if dataSize <= 8 bits, read 8-bit word from RX FIFO,
 *  else read 16-bit word from RX FIFO
 */
static void *SPI_receiveData_v0 (uint32_t baseAddr, SPI_v0_FrameSize frameSize, void *dataBuf);  /*for misra warnings*/
static void *SPI_receiveData_v0 (uint32_t baseAddr, SPI_v0_FrameSize frameSize, void *dataBuf)
{
    void *dataPtr;

    if (frameSize == SPI_v0_8bit)
    {
        *(uint8_t *)dataBuf = (uint8_t)SPIReceiveData(baseAddr);
        dataPtr = (void *)(((uint8_t *)dataBuf) + 1);
    }
    else
    {
        *(uint16_t *)dataBuf = (uint16_t)SPIReceiveData(baseAddr);
        dataPtr = (void *)(((uint8_t *)dataBuf) + 2);
    }

    return (dataPtr);
}

/*
 *  ======== SPI_transmitData_v0 ========
 *
 *  Put data to the TX FIFO based on the data size (in # of bits)
 *
 *  if dataSize <= 8 bits, write 8-bit word to TX FIFO,
 *  else write 16-bit word to TX FIFO
 */
void *SPI_transmitData_v0 (uint32_t baseAddr, uint32_t csHold, SPI_v0_FrameSize frameSize, void *dataBuf);  /*for misra warnings*/
void *SPI_transmitData_v0 (uint32_t baseAddr, uint32_t csHold, SPI_v0_FrameSize frameSize, void *dataBuf)
{
    void *dataPtr;

    if (frameSize == SPI_v0_8bit)
    {
        SPITransmitData(baseAddr, csHold, (uint32_t)(*(uint8_t *)dataBuf));
        dataPtr = (void *)(((uint8_t *)dataBuf) + 1);
    }
    else
    {
        SPITransmitData(baseAddr, csHold, (uint32_t)(*(uint16_t *)dataBuf));
        dataPtr = (void *)(((uint8_t *)dataBuf) + 2);
    }

    return (dataPtr);
}

static bool SPI_pollingXferTimeout_v0(uint32_t *timeout, uint32_t *timeoutLoop, uint32_t timeoutVal);
static bool SPI_pollingXferTimeout_v0(uint32_t *timeout, uint32_t *timeoutLoop, uint32_t timeoutVal)
{
    bool     timeoutFlag = BFALSE;

    if (0U < *timeout)
    {
        *timeout -= 1U;
        if (0U == *timeout)
        {
            if (0U < *timeoutLoop)
            {
                *timeoutLoop -= 1U;
                *timeout = timeoutVal;
            }
        }
    }
    else
    {
        if (0U == *timeoutLoop)
        {
            /* Polling transfer timed out */
            timeoutFlag = BTRUE;
        }
    }

    return (timeoutFlag);
}

/*
 *  ======== SPI_primeTransfer_v0 ========
 *  This functions configures the transmit and receive channels for a given
 *  SPI_Handle and SPI_Transaction
 *
 *  @pre    Function assumes that the handle and transaction is not NULL
 */
static void SPI_primeTransfer_v0(SPI_Handle handle,
                                 SPI_Transaction *transaction); /*for misra warnings*/
static void SPI_primeTransfer_v0(SPI_Handle handle,
                                 SPI_Transaction *transaction)
{
    SPI_v0_Object           *object = NULL;
    SPI_v0_HWAttrs const    *hwAttrs = NULL;
    uint32_t                 csHold = SPI_CSHOLD_ON;
    uint32_t                 terminateXfer;
    uint32_t                 intCode;
    uint32_t                 timeout;
    uint32_t                 timeoutLoop;
    bool                     timeoutFlag = BFALSE;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = (const SPI_v0_HWAttrs *)handle->hwAttrs;
    object  = (SPI_v0_Object*)handle->object;

    object->writeBufIdx = (uint8_t*)transaction->txBuf;
    object->writeCountIdx = transaction->count;

    object->readBufIdx = (uint8_t*)transaction->rxBuf;
    object->readCountIdx = transaction->count;

    /* Reset the transfer error code to no error */
    object->transferErr = SPI_XFER_ERR_NONE;

    terminateXfer = *(uint32_t *)object->transaction->arg;

	transaction->status = SPI_TRANSFER_STARTED;
    /* Clear out any pending read data */
    do {
		SPIReceiveData(hwAttrs->baseAddr);
    } while(SPIRxFull(hwAttrs->baseAddr));

    if(SPI_OPER_MODE_POLLING != object->operMode)
    {
        SPIIntStatusClear(hwAttrs->baseAddr, SPI_INT_RX_FULL | SPI_INT_DMA_ENABLE |
                          SPI_INT_RX_OVERRUN | SPI_INT_TX_BITERR);
#ifdef SPI_DMA_ENABLE
        if (BTRUE == hwAttrs->dmaMode)
        {
            SPIIntEnable(hwAttrs->baseAddr, 
                         SPI_INT_RX_OVERRUN | SPI_INT_TX_BITERR);
            SPI_dmaTransfer(handle, transaction);
        }
        else
#endif
        {
            SPIIntEnable(hwAttrs->baseAddr, SPI_INT_TX_EMPTY |
                SPI_INT_RX_FULL | SPI_INT_RX_OVERRUN | SPI_INT_TX_BITERR);
        }
    }
    else
    {
        /* Initialize the timeout value and loop count in polling mode */
        timeout = object->waitTimeout;
        timeoutLoop = SPI_POLLING_TIMEOUT_LOOP;

        /* Polling mode transfer */
        while ((0U != object->writeCountIdx) && (BFALSE == timeoutFlag))
        {
            intCode = SPIIntStatusGet(hwAttrs->baseAddr);

            /* For the last write byte, release the hold if no data read */
            if ((1U == object->writeCountIdx) && (0U != terminateXfer))
            {
                csHold = SPI_CSHOLD_OFF;
            }

            /* TX bit error, abort transaction, disable interrupts */
            if (intCode & SPI_INT_TX_BITERR)
            {
                SPIXferDisable(hwAttrs->baseAddr);
                object->transferErr = SPI_XFER_ERR_BE;
                break;
            }

            /* Wait until TX buffer is empty */
            while (UFALSE == SPITxEmpty(hwAttrs->baseAddr))
            {
                timeoutFlag = SPI_pollingXferTimeout_v0(&timeout,
                                                        &timeoutLoop,
                                                        object->waitTimeout);
                if (BTRUE == timeoutFlag)
                {
                    break;
                }
            }

            /* Write data to TX buffer, if no write data, write 0 */
            if (BFALSE == timeoutFlag)
            {
                if (object->writeBufIdx)
                {
                    object->writeBufIdx = (uint8_t *)SPI_transmitData_v0 (hwAttrs->baseAddr,
                                                               csHold,
                                                               object->frameSize,
                                                               object->writeBufIdx);
                }
                else
                {
                    SPITransmitData(hwAttrs->baseAddr, csHold, 0);
                }
                object->writeCountIdx--;
            }

            /* Wait until RX buffer is full */
            while(UFALSE == SPIRxFull(hwAttrs->baseAddr))
            {
                timeoutFlag = SPI_pollingXferTimeout_v0(&timeout,
                                                        &timeoutLoop,
                                                        object->waitTimeout);
                if (BTRUE == timeoutFlag)
                {
                    break;
                }
            }

            if (intCode & SPI_INT_RX_OVERRUN)
            {
                /* RX overrun, need to read SPIBUF twice to get to the overrun buffer */
                SPIReceiveData(hwAttrs->baseAddr);
                SPIIntStatusClear(hwAttrs->baseAddr, SPI_INT_RX_OVERRUN);
                object->transferErr = SPI_XFER_ERR_RXOR;
            }

            if ((object->readCountIdx) && (BFALSE == timeoutFlag))
            {
                if (object->readBufIdx)
                {
                    object->readBufIdx = (uint8_t *) SPI_receiveData_v0(hwAttrs->baseAddr,
                                                            object->frameSize,
                                                            object->readBufIdx);
                }
                else
                {
                    /* Dummy read to flush the data in Rx Buffer */
                    SPIReceiveData(hwAttrs->baseAddr);
                }
                object->readCountIdx--;
            }

            timeoutFlag = SPI_pollingXferTimeout_v0(&timeout,
                                                    &timeoutLoop,
                                                    object->waitTimeout);

            if ((0U != object->readCountIdx) || (0U != object->writeCountIdx))
            {
                if (BTRUE == timeoutFlag)
                {
                    object->transferErr = SPI_XFER_ERR_TIMEOUT;
                }
            }
        }
        if (SPI_XFER_ERR_NONE == object->transferErr)
        {
            transaction->status = SPI_TRANSFER_COMPLETED;
        }
        else if (SPI_XFER_ERR_TIMEOUT == object->transferErr)
        {
            transaction->status = SPI_TRANSFER_TIMEOUT;
        }
        else
        {
            transaction->status = SPI_TRANSFER_FAILED;
        }
    }
}

/*
 *  ======== SPI_v0_hwiFxn ========
 *  ISR for the SPI when we use the interrupts
 */
static void SPI_v0_hwiFxn (void* arg); /*for misra warnings*/
static void SPI_v0_hwiFxn (void* arg)
{
    SPI_v0_Object         *object = NULL;
    SPI_v0_HWAttrs const  *hwAttrs = NULL;
    uint32_t               intCode = 0U;
    uint32_t               csHold = SPI_CSHOLD_ON;
    uint32_t               terminateXfer;
    bool                   loop = BTRUE;

    /* Get the pointer to the object and hwAttrs */
    object = (SPI_v0_Object*)(((SPI_Handle)arg)->object);
    hwAttrs = (const SPI_v0_HWAttrs *)(((SPI_Handle)arg)->hwAttrs);

    if (INVALID_INTC_MUX_NUM != hwAttrs->intcMuxNum)
    {
        SPI_osalMuxIntcDisableHostInt(hwAttrs->intcMuxNum, hwAttrs->intcMuxOutEvent);
        SPI_osalMuxIntcClearSysInt(hwAttrs->intcMuxNum, hwAttrs->intcMuxInEvent);
    }

    terminateXfer = *(uint32_t *)object->transaction->arg;

    intCode = SPIIntStatusGet(hwAttrs->baseAddr);

    /* Loop till all the pending interrupts are serviced */
    while ((0U != (intCode & SPI_INT_MASK)) && (BTRUE == loop))
    {
        /*
         * Refill the TX FIFO if an TX-empty interrupt has occurred & there is more
         * data to transmit.
         */
        if (intCode & SPI_INT_TX_EMPTY)
        {
            if (object->writeCountIdx)
            {
                /* For the last write byte, release the hold if no data read */
                if ((1U == object->writeCountIdx) &&
                    (0U != terminateXfer) &&
                    (SPI_MASTER == object->spiMode))
                {
                    csHold = SPI_CSHOLD_OFF;
                }
                /* Write data to TX buffer, if no write data, write 0 */
                if (object->writeBufIdx)
                {
                    object->writeBufIdx = (uint8_t *) SPI_transmitData_v0 (hwAttrs->baseAddr,
                                                               csHold,
                                                               object->frameSize,
                                                               object->writeBufIdx);
                    object->writeCountIdx--;
                }
                else
                {
                    /* Write the dummy byte only when read and write byte
                       count is equal to ensure that data read is complete
                       for previous dummy write. Observed mutiple dummy writes
                       happening before the data is read on OMAPL13x platfrom
                       which is making the read operation to hang.
                     */
                    if(object->readCountIdx == object->writeCountIdx)
                    {
                        SPITransmitData(hwAttrs->baseAddr, csHold, 0);
                        object->writeCountIdx--;
                    }
                }

                if (0U == object->writeCountIdx)
                {
                    SPIIntDisable(hwAttrs->baseAddr, SPI_INT_TX_EMPTY);
                }
            }
            SPIIntStatusClear(hwAttrs->baseAddr, SPI_INT_TX_EMPTY);

            intCode = intCode & ~SPI_INT_TX_EMPTY;
        }

        /* RX FIFO is full, empty the FIFO to receive more data if necessary */
        if (intCode & SPI_INT_RX_FULL) {
            if (object->readCountIdx)
            {
                /* Read from the SPIBUF */
                if (object->readBufIdx)
                {
                    object->readBufIdx = (uint8_t *) SPI_receiveData_v0(hwAttrs->baseAddr,
                                                            object->frameSize,
                                                            object->readBufIdx);
                }

                object->readCountIdx--;
                if (0U == object->readCountIdx)
                {
                    SPIIntDisable(hwAttrs->baseAddr, SPI_INT_RX_FULL);
                }
            }

            SPIIntStatusClear(hwAttrs->baseAddr, SPI_INT_RX_FULL);

            intCode = intCode & ~SPI_INT_RX_FULL;
        }

        /* RX overrun, read SPIBUF twice to get to the overrun buffer */
        if (intCode & SPI_INT_RX_OVERRUN)
        {
            SPIReceiveData(hwAttrs->baseAddr);
            if (object->readCountIdx)
            {
                if (object->readBufIdx)
                {
                    object->readBufIdx = (uint8_t *) SPI_receiveData_v0(hwAttrs->baseAddr,
                                                            object->frameSize,
                                                            object->readBufIdx);
                }
                object->readCountIdx--;
            }
            SPIIntStatusClear(hwAttrs->baseAddr, SPI_INT_RX_OVERRUN);

            intCode = intCode & ~SPI_INT_RX_OVERRUN;
            object->transferErr = SPI_XFER_ERR_RXOR;
        }

        /* TX bit error, abort transaction, disable interrupts */
        if (intCode & SPI_INT_TX_BITERR)
        {
            SPIXferDisable(hwAttrs->baseAddr);

            SPIIntInit2(hwAttrs->baseAddr, hwAttrs->intrLine);

            object->transaction->status=SPI_TRANSFER_FAILED;
            object->transferErr = SPI_XFER_ERR_BE;
            object->transferCallbackFxn((SPI_Handle)arg, object->transaction);
            if (SPI_OPER_MODE_CALLBACK == object->operMode)
            {
                object->transaction = NULL;
            }

            loop = BFALSE;
        }

        if ((BTRUE == loop) && (0U == object->readCountIdx) && (0U == object->writeCountIdx))
        {
            SPIIntInit2(hwAttrs->baseAddr, hwAttrs->intrLine);
            if (SPI_XFER_ERR_NONE == object->transferErr)
            {
                object->transaction->status = SPI_TRANSFER_COMPLETED;
            }
            else
            {
                object->transaction->status = SPI_TRANSFER_FAILED;
            }
            object->transferCallbackFxn((SPI_Handle)arg, object->transaction);
            if (SPI_OPER_MODE_CALLBACK == object->operMode)
            {
                object->transaction = NULL;
            }

            loop = BFALSE;
        }

        if (BTRUE == loop)
        {
            /* Read the interrupt status to check if there are any pending interrupts */
            intCode = SPIIntStatusGet(hwAttrs->baseAddr);

            /* Check if transmit is completed but data read is pending */
            if((0U == object->writeCountIdx) && !(intCode & SPI_INT_RX_FULL) && (object->readCountIdx))
            {
                /* Send a dummy transfer to trigger a receive interrupt to handle delayed RX
                 * interrupt to read the missed data during transmit.
                 */
                if (intCode & SPI_INT_TX_EMPTY)
                {
                    SPITransmitData(hwAttrs->baseAddr, csHold, 0);
                }
            }
        }
    }

    if (INVALID_INTC_MUX_NUM != hwAttrs->intcMuxNum)
    {
        SPI_osalMuxIntcClearSysInt(hwAttrs->intcMuxNum, hwAttrs->intcMuxInEvent);
        SPI_osalHardwareIntrClear(hwAttrs->eventId,hwAttrs->intNum);
        SPI_osalMuxIntcEnableHostInt(hwAttrs->intcMuxNum, hwAttrs->intcMuxOutEvent);
    }
}

/*
 *  ======== SPI_init_v0 ========
 *  @pre    Function assumes that the handle is not NULL
 */
static void SPI_init_v0(SPI_Handle handle)
{
    /* Input parameter validation */
    if (NULL != handle)
    {
        /* Mark the object as available */
        ((SPI_v0_Object *)(handle->object))->isOpen = BFALSE;
    }
}

/*
 *  ======== SPI_open_v0 ========
 *  @pre    Function assumes that the handle is not NULL
 */
static SPI_Handle SPI_open_v0(SPI_Handle handle, const SPI_Params *params)
{
    SemaphoreP_Params      semParams;
    uint32_t               key;
    SPI_Params             spiParams;
    SPI_v0_Object          *object = NULL;
    SPI_v0_HWAttrs const   *hwAttrs = NULL;
    MuxIntcP_inParams      muxInParams;
    MuxIntcP_outParams     muxOutParams;
	OsalRegisterIntrParams_t interruptRegParams;
    uint8_t                ret_flag = UFALSE;

    /* Input parameter validation */
    if (NULL != handle)
    {
    /* Get the pointer to the object and hwAttrs */
    object = (SPI_v0_Object*)handle->object;
    hwAttrs = (const SPI_v0_HWAttrs *)handle->hwAttrs;


    /* Determine if the device index was already opened */
    key = SPI_osalHardwareIntDisable();
    if (BTRUE == object->isOpen) {
        SPI_osalHardwareIntRestore(key);
        handle = (NULL);
    }
    else {
        /* Mark the handle as being used */
        object->isOpen = BTRUE;
        SPI_osalHardwareIntRestore(key);

        /* Store the SPI parameters */
        if (NULL == params) {
            /* No params passed in, so use the defaults */
            SPI_Params_init(&spiParams);
            params = &spiParams;
        }

        if ((2U > params->dataSize) || (16U < params->dataSize))
        {
            handle = NULL;
            ret_flag = UTRUE;
        }
        else
        {
            /* Determine if we need to use an 8-bit or 16-bit frame size */
            object->frameSize = (9U > params->dataSize) ? SPI_v0_8bit : SPI_v0_16bit;
        }

        /* Store the current mode. Extract operating mode from hwAttrs and params */
        if(SPI_MODE_BLOCKING == params->transferMode)
        {
            if(BTRUE == hwAttrs->enableIntr)
            {
                object->operMode = SPI_OPER_MODE_BLOCKING;
            }
#ifdef SPI_DMA_ENABLE
            else if (BTRUE == hwAttrs->dmaMode)
            {
                object->operMode = SPI_OPER_MODE_BLOCKING;
            }
#endif
            else
            {
                object->operMode = SPI_OPER_MODE_POLLING;
            }
        }
        else
        {
            object->operMode = SPI_OPER_MODE_CALLBACK;
            /* Check to see if a callback function was defined for async mode */
            if (NULL == params->transferCallbackFxn)
            {
                handle = NULL;
                ret_flag = UTRUE;
            }
        }

        /* store the SPI transfer mode (master or slave */
        object->spiMode = params->mode;
        object->waitTimeout = params->transferTimeout;

        if(UFALSE == ret_flag)
        {
            if(SPI_OPER_MODE_POLLING != object->operMode)
            {
                /* Initialize with defaults */
                Osal_RegisterInterrupt_initParams(&interruptRegParams);             
                if (INVALID_INTC_MUX_NUM != hwAttrs->intcMuxNum)
                {
                    /* Setup intc mux */
                    muxInParams.arg         = (uintptr_t)handle;
                    muxInParams.muxNum      = hwAttrs->intcMuxNum;
                    muxInParams.muxInEvent  = hwAttrs->intcMuxInEvent;
                    muxInParams.muxOutEvent = hwAttrs->intcMuxOutEvent;
                    muxInParams.muxIntcFxn  = (MuxIntcFxn)(&SPI_v0_hwiFxn);
                    SPI_osalMuxIntcSetup(&muxInParams, &muxOutParams);

                    interruptRegParams.corepacConfig.isrRoutine  = (void (*)(uintptr_t))muxOutParams.muxIntcFxn;
                    interruptRegParams.corepacConfig.arg         = (uintptr_t)muxOutParams.arg;
                }
                else
                {
                    interruptRegParams.corepacConfig.isrRoutine  = (void (*)(uintptr_t))(&SPI_v0_hwiFxn);
                    interruptRegParams.corepacConfig.arg         = (uintptr_t)handle;
                }

#if defined (__ARM_ARCH_7A__)
                interruptRegParams.corepacConfig.corepacEventNum = 0U; /* Event ID not used in GIC */
                interruptRegParams.corepacConfig.triggerSensitivity = 0x1; /* interrupt active high level triggered */
#else
                interruptRegParams.corepacConfig.corepacEventNum = hwAttrs->eventId;
#endif
               interruptRegParams.corepacConfig.name = NULL;
#if defined (__TI_ARM_V5__)
               interruptRegParams.corepacConfig.priority = hwAttrs->intNum;
#else
               interruptRegParams.corepacConfig.priority = 0x20;
#endif
               interruptRegParams.corepacConfig.intVecNum=hwAttrs->intNum; /* Host Interrupt vector */
               SPI_osalRegisterInterrupt(&interruptRegParams,&(object->hwi));
                if(NULL == object->hwi) {
                    SPI_close_v0(handle);
                    ret_flag = UTRUE;
                    handle = NULL;
                }
            }
        }
        if(UFALSE == ret_flag)
        {
            /*
             * Construct thread safe handles for this SPI peripheral
             * Semaphore to provide exclusive access to the SPI peripheral
             */
            SPI_osalSemParamsInit(&semParams);
            semParams.mode = SemaphoreP_Mode_BINARY;
            object->mutex = SPI_osalCreateBlockingLock(1U, &semParams);

            if (SPI_OPER_MODE_BLOCKING == object->operMode) {
                /*
                 * Construct a semaphore to block task execution for the duration of the
                 * SPI transfer
                 */
                object->transferComplete = SPI_osalCreateBlockingLock(0, &semParams);

                /* Store internal callback function */
                object->transferCallbackFxn = &SPI_transferCallback_v0;
            }
            else {
                if (SPI_OPER_MODE_CALLBACK == object->operMode)
                {
                    /* Save the callback function pointer */
                    object->transferCallbackFxn = params->transferCallbackFxn;
                }
            }
            object->transaction = NULL;

            /* Extract clock mode from the frame format */
            switch(params->frameFormat)
            {
                case SPI_POL0_PHA0:
                    object->clockMode = SPI_CLK_MODE_0;
                break;

                case SPI_POL0_PHA1:
                    object->clockMode = SPI_CLK_MODE_1;
                break;

                case SPI_POL1_PHA0:
                    object->clockMode = SPI_CLK_MODE_2;
                break;

                case SPI_POL1_PHA1:
                    object->clockMode = SPI_CLK_MODE_3;
                break;

                default:
                    object->clockMode = SPI_CLK_MODE_2;
                break;
            }

#ifdef SPI_DMA_ENABLE
            if (BTRUE == hwAttrs->dmaMode)
            {
                SPI_dmaConfig(handle);
            }
#endif
            /* Reset SPI Peripheral */
            SPIReset(hwAttrs->baseAddr);

            /* Setup global control */
            if(SPI_MASTER == params->mode)
            {
                SPIGlobalControlSetup(hwAttrs->baseAddr, hwAttrs->loopback,
                                      SPI_POWERDOWN_OFF, SPI_MASTER_MODE);
            }
            else
            {
                SPIGlobalControlSetup(hwAttrs->baseAddr, SPI_LOOPBACK_OFF,
                                      SPI_POWERDOWN_OFF, SPI_SLAVE_MODE);
            }

            SPIXferDisable(hwAttrs->baseAddr);

            /* Initialize SPI interrupts, by default all disabled */
            SPIIntInit2(hwAttrs->baseAddr, hwAttrs->intrLine);

            SPIPinControlSetup(hwAttrs->baseAddr, SPI_SOMIFUN_ON,
                               SPI_SIMOFUN_ON, SPI_CLKFUN_ON);

            /* Configure 3 pin or 4 pin mode */
            if(SPI_PINMODE_3_PIN == hwAttrs->pinMode)
            {
                /* Disable chip select pin.*/
                SPICSDisable(hwAttrs->baseAddr, hwAttrs->csNum);
            }
            else
            {
                /* Enable chip select pin.*/
                SPICSEnable(hwAttrs->baseAddr, hwAttrs->csNum);
            }

            /* Configure data format */
            if(SPI_MASTER == params->mode)
            {
                SPIDataFormatSetup(hwAttrs->baseAddr,
                                   SPI_DFSEL_0,
                                   hwAttrs->inputClkFreq,
                                   params->bitRate,
                                   object->clockMode,
                                   params->dataSize);
                SPIDelaySetup(hwAttrs->baseAddr, SPI_C2TDELAY(8U), SPI_T2CDELAY(8U));
                SPIData1Setup(hwAttrs->baseAddr, SPI_DELAY_ON, hwAttrs->csNum);
            }
            else
            {
                SPIDataFormatSetupSlave(hwAttrs->baseAddr,
                                        SPI_DFSEL_0,
                                        params->dataSize);
                SPIClockModeSetup(hwAttrs->baseAddr,
                                      SPI_DFSEL_0,
                                      object->clockMode);
            }

        }
    }
    }
    return (handle);
}

/*
 *  ======== SPI_transfer_v0 ========
 *  @pre    Function assumes that handle and transaction is not NULL
 */
static bool SPI_transfer_v0(SPI_Handle handle, SPI_Transaction *transaction)
{
    bool         ret = BFALSE;                /* return value */
    uint32_t     key;
    SPI_v0_Object    *object = NULL;
    SPI_v0_HWAttrs const    *hwAttrs = NULL;
    SemaphoreP_Status   semStatus = SemaphoreP_OK;

    /* Input parameter validation */
    if ((NULL != handle) && (NULL != transaction))
    {
    if (0U != transaction->count) {
        /* Get the pointer to the object and hwAttrs */
        hwAttrs = (const SPI_v0_HWAttrs *)handle->hwAttrs;
        object = (SPI_v0_Object*)handle->object;
        if (0U == object->waitTimeout)
        {
            /* timeout cannot be NO_WAIT, set it to default value */
            object->waitTimeout = SPI_WAIT_FOREVER;
        }

        /* Check if a transfer is in progress */
        key = SPI_osalHardwareIntDisable();
        if (object->transaction) {
            SPI_osalHardwareIntRestore(key);
            /* Transfer is in progress */
			transaction->status = SPI_TRANSFER_CANCELED;
            ret = BFALSE;
        }
        else {
            /* Save the pointer to the transaction */
            object->transaction = transaction;

            /* Acquire the lock for this particular SPI handle */
            SPI_osalPendLock(object->mutex, SemaphoreP_WAIT_FOREVER);

            SPI_primeTransfer_v0(handle, transaction);

            SPI_osalHardwareIntRestore(key);

            if (SPI_OPER_MODE_BLOCKING == object->operMode) {
                semStatus = SPI_osalPendLock(object->transferComplete, object->waitTimeout);
            }

            /* Release the lock for this particular SPI handle */
            SPI_osalPostLock(object->mutex);

            if (SPI_OPER_MODE_CALLBACK != object->operMode)
            {
                object->transaction = NULL;
            }
            if (semStatus == SemaphoreP_TIMEOUT)
            {
#ifdef SPI_DMA_ENABLE
                if (BTRUE == hwAttrs->dmaMode)
                {
                    SPIIntDisable(hwAttrs->baseAddr, SPI_INT_RX_OVERRUN |
                                SPI_INT_TX_BITERR);
                }
                else
#endif
                {
                    SPIIntDisable(hwAttrs->baseAddr, SPI_INT_TX_EMPTY |
                                SPI_INT_RX_FULL | SPI_INT_RX_OVERRUN | SPI_INT_TX_BITERR);
                }
                SPIIntStatusClear(hwAttrs->baseAddr,
                                  SPI_INT_RX_FULL | SPI_INT_DMA_ENABLE |
                                  SPI_INT_RX_OVERRUN | SPI_INT_TX_BITERR);

                /* Transaction timed out */
                transaction->status = SPI_TRANSFER_TIMEOUT;
                object->transferErr = SPI_XFER_ERR_TIMEOUT;

                ret = BFALSE;
            }
            else
            {
                if (SPI_OPER_MODE_POLLING == object->operMode)
                {
                    /*
                     * Polling transaction status is set in SPI_primeTransfer_v0
                     */
                    if (SPI_TRANSFER_COMPLETED == transaction->status)
                    {
                        ret = BTRUE;
                    }
                    else
                    {
                        ret = BFALSE;
                    }
                }
                else
                {
                    /* Callback transaction or interrupt/dma (no timeout) transaction */
                    ret = BTRUE;
                }
            }
        }        
    } else {
	    transaction->status = SPI_TRANSFER_CANCELED;
	}
    }
    return (ret);
}

/*
 *  ======== SPI_transferCallback_v0 ========
 *  Callback function for when the SPI is in SPI_MODE_BLOCKING
 *
 *  @pre    Function assumes that the handle is not NULL
 */
static void SPI_transferCallback_v0(SPI_Handle handle,
                                    SPI_Transaction *transaction)
{
    SPI_v0_Object    *object = NULL;

    /* Get the pointer to the object */
    object = (SPI_v0_Object*)handle->object;

    SPI_osalPostLock(object->transferComplete);
}


/*
 *  ======== SPI_transferCancel_v0 ========
 *  V0 specific SPI transferCancel function
 *
 *  @pre    Function assumes that the handle is not NULL
 */
static void SPI_transferCancel_v0(SPI_Handle handle)
{
    SPI_v0_Object    *object = NULL;
    SPI_v0_HWAttrs const    *hwAttrs = NULL;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = (const SPI_v0_HWAttrs *)handle->hwAttrs;
    object = (SPI_v0_Object*)handle->object;
    if (BTRUE == hwAttrs->enableIntr)
    {

#ifdef SPI_DMA_ENABLE
        if (BTRUE == hwAttrs->dmaMode)
        {
            SPIIntDisable(hwAttrs->baseAddr, SPI_INT_RX_OVERRUN |
                          SPI_INT_TX_BITERR);
            SPI_dmaDisableChannel(handle, BTRUE);
        }
        else
#endif
        {
            SPIIntDisable(hwAttrs->baseAddr, SPI_INT_TX_EMPTY |
                          SPI_INT_RX_FULL | SPI_INT_RX_OVERRUN | SPI_INT_TX_BITERR);
        }
        SPIIntStatusClear(hwAttrs->baseAddr, SPI_INT_RX_FULL | SPI_INT_DMA_ENABLE |
                          SPI_INT_RX_OVERRUN | SPI_INT_TX_BITERR);
    }

    object->transaction->status = SPI_TRANSFER_CANCELED;
    if (SPI_OPER_MODE_BLOCKING == object->operMode)
    {
        SPI_osalPostLock(object->transferComplete);
    }
    if (SPI_OPER_MODE_CALLBACK == object->operMode)
    {
        if (NULL != object->transferCallbackFxn)
        {
            object->transferCallbackFxn(handle, object->transaction);
        }
    }
    return;
}


/*
 *  ======== SPI_control_v0 ========
 *  V0 specific SPI control function
 *
 *  @pre    Function assumes that the handle is not NULL
 */
static int32_t SPI_control_v0(SPI_Handle handle, uint32_t cmd, const void *arg)
{
    SPI_v0_HWAttrs const   *hwAttrs = NULL;
    SPI_v0_Object          *object;
    uint32_t               *cmd_args = (uint32_t *)arg;
    int32_t                retVal = SPI_STATUS_SUCCESS;

    /* Input parameter validation */
    if (NULL != handle)
    {
    /* Get the pointer to the object and hwAttrs */
    hwAttrs = (const SPI_v0_HWAttrs *)handle->hwAttrs;
    object = (SPI_v0_Object*)handle->object;

    switch (cmd)
    {
        case SPI_V0_CMD_XFER_ACTIVATE:
            if (cmd_args[0])
            {
                SPIXferEnable(hwAttrs->baseAddr);
            }
            else
            {
                /* Disable All SPI interrupts */
                SPIIntDisable(hwAttrs->baseAddr, SPI_INT_DMA_ENABLE | SPI_INT_TX_EMPTY |
                              SPI_INT_RX_FULL | SPI_INT_RX_OVERRUN | SPI_INT_TX_BITERR);

                SPIXferDisable(hwAttrs->baseAddr);
            }
            break;

        case SPI_V0_CMD_LOOPBACK:
            if (cmd_args[0])
            {
                SPILoopbackEnable(hwAttrs->baseAddr);
            }
            else
            {
                SPILoopbackDisable(hwAttrs->baseAddr);
            }
            break;

        case SPI_V0_CMD_PWRDOWN:
            if (cmd_args[0])
            {
                SPIPwrDownEnable(hwAttrs->baseAddr);
            }
            else
            {
                SPIPwrDownDisable(hwAttrs->baseAddr);
            }
            break;

        case SPI_V0_CMD_DELAY:
            SPIDelaySetup(hwAttrs->baseAddr, cmd_args[0], cmd_args[1]);
            break;

        case SPI_V0_CMD_SHIFT_DIR:
            SPISetShiftDir(hwAttrs->baseAddr, cmd_args[0]);
            break;

        case SPI_V0_CMD_GET_XFER_ERR:
            *cmd_args = object->transferErr;
            break;

        default:
            retVal = SPI_STATUS_UNDEFINEDCMD;
            break;
    }
    }
    return retVal;
}
