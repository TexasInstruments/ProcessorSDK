/**
 *  \file   QSPI_v0.c
 *
 *  \brief  QSPI IP Version 0 specific driver APIs implementation.
 *
 *   This file contains the driver APIs for QSPI controller.
 */

/*
 * Copyright (C) 2016 - 2019 Texas Instruments Incorporated - http://www.ti.com/
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
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/src/v0/QSPI_v0.h>
#include <ti/csl/src/ip/qspi/V0/qspi.h>
#include <ti/drv/spi/src/SPI_osal.h>

#define QSPI_MIN_BAUD_RATE_DIVISOR      (2U)
#define QSPI_MAX_BAUD_RATE_DIVISOR      (32U)

/* QSPI AM57x functions */
static void       QSPI_close_v0(SPI_Handle handle);
static void       QSPI_init_v0(SPI_Handle handle);
static SPI_Handle QSPI_open_v0(SPI_Handle handle, const SPI_Params *params);
static bool       QSPI_transfer_v0(SPI_Handle handle,
                                      SPI_Transaction *transaction);
static int32_t    QSPI_primeTransfer_v0(SPI_Handle handle,
                                          SPI_Transaction *transaction);
static void       QSPI_transferCallback_v0(SPI_Handle handle,
                                              SPI_Transaction *msg);
static int32_t QSPI_control_v0(SPI_Handle handle, uint32_t cmd, const void *arg);
static void QSPI_transferCancel_v0(SPI_Handle handle);
static void QSPI_hwiFxn_v0(uintptr_t arg);
static int32_t QSPI_read_v0(SPI_Handle handle,  SPI_Transaction *transaction);
static int32_t QSPI_write_v0(SPI_Handle handle,  SPI_Transaction *transaction);
static int32_t QSPI_ind_xfer_mode_write_v0(SPI_Handle handle,
                                     SPI_Transaction *transaction);
static int32_t QSPI_cmd_mode_write_v0(SPI_Handle handle,
                                      const SPI_Transaction *transaction);
static int32_t QSPI_cmd_mode_read_v0(SPI_Handle handle,
                                   SPI_Transaction *transaction);
static int32_t QSPI_ind_xfer_mode_read_v0(SPI_Handle handle,
                                    SPI_Transaction *transaction);

typedef enum QSPI_intrPollMode_s {
    SPI_OPER_MODE_BLOCKING = 0U,  /*! Interrupt based blocking mode */
    SPI_OPER_MODE_POLLING,        /*! Non interrupt based blocking mode */
    SPI_OPER_MODE_CALLBACK        /*! Interrupt based call back mode */
} QSPI_intrPollMode;



/* SPI function table for QSPI AM57x implementation */
const SPI_FxnTable QSPI_FxnTable_v0 = {
    &QSPI_close_v0,
    &QSPI_control_v0,
    &QSPI_init_v0,
    &QSPI_open_v0,
    &QSPI_transfer_v0,
    &QSPI_transferCancel_v0,
    NULL
};

/*
 *  ======== QSPI_close_v0 ========
 */
static void QSPI_close_v0(SPI_Handle handle)
{
    QSPI_v0_Object        *object = NULL;
    QSPI_v0_HwAttrs const *hwAttrs = NULL;

    /* Input parameter validation */
    if (NULL != handle)
    {
    /* Get the pointer to the object and hwAttrs */
    object = (QSPI_v0_Object*)handle->object;
    hwAttrs = (const QSPI_v0_HwAttrs *)handle->hwAttrs;

    /* Mask I2C interrupts */
    QSPIIntrDisable(hwAttrs->baseAddr, QSPI_INTR_MASK_ALL);

    /* Destruct the Hwi */
    if(SPI_OPER_MODE_POLLING != object->intrPollMode)
    {
        SPI_osalHardwareIntDestruct(object->hwi, hwAttrs->eventId);
    }

    /* Destruct the instance lock */
    SPI_osalDeleteBlockingLock(object->mutex);

    /* Destruct the transfer completion lock */
    if(SPI_OPER_MODE_BLOCKING == object->intrPollMode)
    {
        SPI_osalDeleteBlockingLock(object->transferComplete);
    }

    /* Open flag is set false */
    object->isOpen = BFALSE;
    }

    return;
}

/*
 *  ======== QSPI_hwiFxn_v0 ========
 *  Hwi interrupt handler to service the QSPI peripheral
 *
 *  The handler is a generic handler for a QSPI object.
 */
static void QSPI_hwiFxn_v0(uintptr_t arg)
{
    uint32_t               intrStatus;
    QSPI_v0_Object        *object = NULL;
    QSPI_v0_HwAttrs const *hwAttrs = NULL;
    uint32_t               sramLevel, rdBytes, wrBytes;

    /* Get the pointer to the object and hwAttrs */
    object = (QSPI_v0_Object*)(((SPI_Handle)arg)->object);
    hwAttrs = (const QSPI_v0_HwAttrs *)(((SPI_Handle)arg)->hwAttrs);

    if (INVALID_INTC_MUX_NUM != hwAttrs->intcMuxNum)
    {
        SPI_osalMuxIntcDisableHostInt(hwAttrs->intcMuxNum, hwAttrs->intcMuxOutEvent);
        SPI_osalMuxIntcClearSysInt(hwAttrs->intcMuxNum, hwAttrs->intcMuxInEvent);
    }

    /* Read the interrupt status register */
    intrStatus = QSPIIntrStatus(hwAttrs->baseAddr);

    if (object->readBufIdx)
    {
		/* Indirect read operation */
        if(intrStatus & QSPI_INTR_MASK_IND_XFER)
        {
            if(object->readCountIdx)
            {
                if (intrStatus & QSPI_INTR_MASK_IND_XFER_LVL_BREACH)
                {
                    sramLevel = QSPI_SRAM_WARERMARK_RD_LVL;
                }
                else
                {
                    sramLevel = QSPIGetSramLvl(hwAttrs->baseAddr, 1U);
                }

                rdBytes = sramLevel * QSPI_FIFO_WIDTH;
                rdBytes = rdBytes > object->readCountIdx ? object->readCountIdx : rdBytes;

                /* Read data from FIFO */
                QSPIReadFifoData(hwAttrs->indTrigAddr, object->readBufIdx, rdBytes);

                object->readBufIdx += rdBytes;
                object->readCountIdx -= rdBytes;

                if ((0U < object->readCountIdx) &&
                    ((QSPI_SRAM_WARERMARK_RD_LVL * QSPI_FIFO_WIDTH) > object->readCountIdx))
                {
                    while(BTRUE)
                    {
                        sramLevel = QSPIGetSramLvl(hwAttrs->baseAddr, 1U);
                        rdBytes = sramLevel * QSPI_FIFO_WIDTH;
                        if (rdBytes >= object->readCountIdx)
                        {
    						break;
                        }
                    }
                    rdBytes = object->readCountIdx;
                    QSPIReadFifoData(hwAttrs->indTrigAddr, object->readBufIdx, rdBytes);
                    object->readBufIdx += rdBytes;
                    object->readCountIdx -= rdBytes;
                }
            }

            if((0U == object->readCountIdx) || (0U != (intrStatus & QSPI_INTR_MASK_IND_OP_DONE)))
            {
                /* Clear indirect read operation complete status */
				QSPIClrIndReadComplete(hwAttrs->baseAddr);

                /* Call the call back function */
                object->qspiParams.transferCallbackFxn((SPI_Handle)arg, NULL);
            }
            /* Clear interrupt status */
            QSPIIntrClear(hwAttrs->baseAddr, intrStatus);
        }
    }
    else
    {
		/* Indirect write operation */
        if(intrStatus & QSPI_INTR_MASK_IND_XFER)
        {
            if (object->writeCountIdx)
            {
                sramLevel = QSPI_SRAM_PARTITION_WR - QSPIGetSramLvl(hwAttrs->baseAddr, 1U);
                wrBytes = sramLevel * QSPI_FIFO_WIDTH;
                wrBytes = wrBytes > object->writeCountIdx ? object->writeCountIdx : wrBytes;

                /* Write data to FIFO */
                QSPIWriteFifoData(hwAttrs->indTrigAddr, object->writeBufIdx, wrBytes);

                object->writeBufIdx += wrBytes;
                object->writeCountIdx -= wrBytes;

                sramLevel = QSPI_SRAM_PARTITION_WR - QSPIGetSramLvl(hwAttrs->baseAddr, 1U);
                if ((0U < object->writeCountIdx) &&
                    (object->writeCountIdx <= (sramLevel * QSPI_FIFO_WIDTH)))
                {
                    wrBytes = object->writeCountIdx;
                    QSPIWriteFifoData(hwAttrs->indTrigAddr, object->writeBufIdx, wrBytes);
                    object->writeBufIdx += wrBytes;
                    object->writeCountIdx -= wrBytes;
                }
            }

            if(intrStatus & QSPI_INTR_MASK_IND_OP_DONE)
            {
                /* Clear indirect write operation complete status */
				QSPIClrIndWriteComplete(hwAttrs->baseAddr);

                /* Call the call back function */
                object->qspiParams.transferCallbackFxn((SPI_Handle)arg, NULL);
            }

            /* Clear interrupt status */
            QSPIIntrClear(hwAttrs->baseAddr, intrStatus);
        }
    }

    if (INVALID_INTC_MUX_NUM != hwAttrs->intcMuxNum)
    {
        SPI_osalMuxIntcClearSysInt(hwAttrs->intcMuxNum, hwAttrs->intcMuxInEvent);
        SPI_osalHardwareIntrClear(hwAttrs->eventId,hwAttrs->intrNum);
        SPI_osalMuxIntcEnableHostInt(hwAttrs->intcMuxNum, hwAttrs->intcMuxOutEvent);
    }
}

/*
 *  ======== QSPI_init_v0 ========
 */
static void QSPI_init_v0(SPI_Handle handle)
{
    /* Input parameter validation */
    if (NULL != handle)
    {
        /* Mark the object as available */
        ((QSPI_v0_Object *)(handle->object))->isOpen = BFALSE;
    }
}


/*
 *  ======== QSPI_open_v0 ========
 */
static SPI_Handle QSPI_open_v0(SPI_Handle handle, const SPI_Params *params)
{
    SemaphoreP_Params      semParams;
    uint32_t               key;
    QSPI_v0_Object        *object = NULL;
    QSPI_v0_HwAttrs const *hwAttrs = NULL;
    MuxIntcP_inParams      muxInParams;
    MuxIntcP_outParams     muxOutParams;
	OsalRegisterIntrParams_t interruptRegParams;
    uint32_t                retFlag = UFALSE;
    uint32_t               clkDiv;

    /* Input parameter validation */
    if (NULL != handle)
    {
    /* Get the pointer to the object and hwAttrs */
    object = (QSPI_v0_Object*)handle->object;
    hwAttrs = (const QSPI_v0_HwAttrs *)handle->hwAttrs;


    /* Determine if the device index was already opened */
    key = SPI_osalHardwareIntDisable();
    if (BTRUE == object->isOpen) {
        SPI_osalHardwareIntRestore(key);
        handle = NULL;
    }
    else
    {
        /* Mark the handle as being used */
        object->isOpen = BTRUE;
        SPI_osalHardwareIntRestore(key);

        /* Store the QSPI parameters */
        if (NULL == params) {
            /* No params passed in, so use the defaults */
            SPI_Params_init(&(object->qspiParams));
        }
        else {
            /* Copy the params contents */
            object->qspiParams = *params;
        }

        /* Copy the controller mode from hardware attributes to object */
        object->qspiMode = hwAttrs->operMode;
        object->xferLines  = hwAttrs->xferLines;

        /* Extract QSPI operating mode based on hwAttrs and input parameters */
        if(SPI_MODE_BLOCKING == object->qspiParams.transferMode)
        {
            if(BTRUE == hwAttrs->intrEnable)
            {
                object->intrPollMode = SPI_OPER_MODE_BLOCKING;
            }
            else
            {
                object->intrPollMode = SPI_OPER_MODE_POLLING;
            }
        }
        else
        {
            object->intrPollMode = SPI_OPER_MODE_CALLBACK;
        }

        /* Extract the polling mode from hardware attributes. */
        if(SPI_OPER_MODE_POLLING != object->intrPollMode)
        {
            Osal_RegisterInterrupt_initParams(&interruptRegParams);             
            if (INVALID_INTC_MUX_NUM != hwAttrs->intcMuxNum)
            {
                /* Setup intc mux */
                muxInParams.arg         = (uintptr_t)handle;
                muxInParams.muxNum      = hwAttrs->intcMuxNum;
                muxInParams.muxInEvent  = hwAttrs->intcMuxInEvent;
                muxInParams.muxOutEvent = hwAttrs->intcMuxOutEvent;
                muxInParams.muxIntcFxn  = (MuxIntcFxn)(&QSPI_hwiFxn_v0);
                SPI_osalMuxIntcSetup(&muxInParams, &muxOutParams);

                interruptRegParams.corepacConfig.isrRoutine  = (void (*)(uintptr_t))muxOutParams.muxIntcFxn;
                interruptRegParams.corepacConfig.arg         = (uintptr_t)muxOutParams.arg;
            }
            else
            {
                interruptRegParams.corepacConfig.isrRoutine  = (void (*)(uintptr_t))(&QSPI_hwiFxn_v0);
                interruptRegParams.corepacConfig.arg         = (uintptr_t)handle;
            }

#if defined (__ARM_ARCH_7A__)
            interruptRegParams.corepacConfig.triggerSensitivity = 0x3; /* interrupt edge triggered */
#else
            interruptRegParams.corepacConfig.corepacEventNum = hwAttrs->eventId;
#endif
            interruptRegParams.corepacConfig.name = NULL;
            interruptRegParams.corepacConfig.priority = 0x20;
            interruptRegParams.corepacConfig.intVecNum=hwAttrs->intrNum; /* Host Interrupt vector */
            SPI_osalRegisterInterrupt(&interruptRegParams,&(object->hwi));
            if(NULL == object->hwi) {
                QSPI_close_v0(handle);
                handle = NULL;
                retFlag = UTRUE;
            }
        }
        
        if(UFALSE == retFlag)
        {
            /*
             * Construct thread safe handles for this QSPI peripheral
             * Semaphore to provide exclusive access to the QSPI peripheral
             */
            SPI_osalSemParamsInit(&semParams);
            semParams.mode = SemaphoreP_Mode_BINARY;
            object->mutex = SPI_osalCreateBlockingLock(1U, &semParams);

            /*
             * Store a callback function that posts the transfer complete
             * semaphore for synchronous mode
             */
            if (SPI_OPER_MODE_BLOCKING == object->intrPollMode) {
            /*
             * Semaphore to cause the waiting task to block for the QSPI
             * to finish
             */
                object->transferComplete = SPI_osalCreateBlockingLock(0, &semParams);

                /* Store internal callback function */
                object->qspiParams.transferCallbackFxn = &QSPI_transferCallback_v0;
            }
            if(SPI_OPER_MODE_CALLBACK == object->intrPollMode){
                /* Check to see if a callback function was defined for async mode */
                OSAL_Assert(NULL == object->qspiParams.transferCallbackFxn);
            }

            /* Disable QSPI controller */
            QSPIDisable(hwAttrs->baseAddr);

            /* Disable DAC */
            QSPIDacDisable(hwAttrs->baseAddr);

            /* Wait until Serial Interface and QSPI pipeline is IDLE. */
            if (QSPIWaitIdle(hwAttrs->baseAddr))
            {
                QSPI_close_v0(handle);
                handle = NULL;
                retFlag = UTRUE;
            }
        }

        if(UFALSE == retFlag)
        {
            /* Set clock mode */
            QSPISetClkMode(hwAttrs->baseAddr, hwAttrs->frmFmt);

            /* Delay Setup */
            QSPISetDevDelay(hwAttrs->baseAddr, hwAttrs->devDelays);

            /* Chip select setup */
            QSPISetChipSelect(hwAttrs->baseAddr, hwAttrs->chipSelect, QSPI_DECODER_SELECT4);

            /* Set default baud rate divider value */
            if (hwAttrs->clkDiv > 0U)
            {
                /* use SoC specific clock divider if configured */
                clkDiv = hwAttrs->clkDiv;
            }
            else
            {
                /*
                 * if SoC specific clock divider not configured
                 * calculate the clock divider using QSPI clock and baud rate
                 */
                clkDiv = hwAttrs->funcClk / object->qspiParams.bitRate;
            }
            if (QSPI_MAX_BAUD_RATE_DIVISOR < clkDiv)
            {
                clkDiv = QSPI_MAX_BAUD_RATE_DIVISOR;
            }

            if (QSPI_MIN_BAUD_RATE_DIVISOR > clkDiv)
            {
                clkDiv = QSPI_MIN_BAUD_RATE_DIVISOR;
            }
            QSPISetPreScaler(hwAttrs->baseAddr, QSPI_BAUD_RATE_DIVISOR(clkDiv));

            /* Disable the loopback mode */
            QSPILoopbackDisable(hwAttrs->baseAddr);

            /* Set device size cofigurations */
            QSPISetDevSize(hwAttrs->baseAddr,
                           QSPI_MEM_MAP_NUM_ADDR_BYTES_THREE,
                           hwAttrs->pageSize,
                           hwAttrs->blkSize);

            /* Set indirect trigger address */
            QSPISetIndTrigAddr(hwAttrs->baseAddr, hwAttrs->indTrigAddr);

            /* Disable write completion auto polling */
            QSPISetWrCompAutoPolling(hwAttrs->baseAddr, QSPI_WRITE_COMP_AUTO_POLLING_DISABLE);

            /* Set SRAM partition configuration */
            QSPISetSramPartition(hwAttrs->baseAddr, QSPI_SRAM_PARTITION_DEFAULT);

            /* Clear the interrupts and interrupt status */
            QSPIIntrDisable(hwAttrs->baseAddr, QSPI_INTR_MASK_ALL);
            QSPIIntrClear(hwAttrs->baseAddr, QSPI_INTR_MASK_ALL);

            /* Enable QSPI controller */
            QSPIEnable(hwAttrs->baseAddr);
        }
    }
    }

    return(handle);
}

/*
 *  ======== QSPI_primeTransfer_v0 =======
 */
static int32_t QSPI_primeTransfer_v0(SPI_Handle handle,
                                     SPI_Transaction *transaction)
{
    QSPI_v0_Object        *object = NULL;
    QSPI_v0_HwAttrs const *hwAttrs = NULL;
    int32_t                retVal = SPI_STATUS_SUCCESS;

    /* Get the pointer to the object and hwAttrs */
    object = (QSPI_v0_Object*)handle->object;
    hwAttrs = (const QSPI_v0_HwAttrs *)handle->hwAttrs;

    /* Store the new internal counters and pointers */
    object->writeBufIdx = (uint8_t*)transaction->txBuf;
    object->writeCountIdx = transaction->count;

    object->readBufIdx = (uint8_t*)transaction->rxBuf;
    object->readCountIdx = transaction->count;

    /* Disable and clear the interrupts */
    QSPIIntrDisable(hwAttrs->baseAddr, QSPI_INTR_MASK_ALL);
    QSPIIntrClear(hwAttrs->baseAddr, QSPI_INTR_MASK_ALL);

    /* Interrupt mode */
    if(SPI_OPER_MODE_POLLING != object->intrPollMode)
    {
        QSPIIntrEnable(hwAttrs->baseAddr, QSPI_INTR_MASK_ALL);
    }

    /* Identify the direction of transfer (whether read/write) */
    if(SPI_TRANSACTION_TYPE_READ == object->transactionType)
    {
        retVal = (QSPI_read_v0(handle, transaction));
    }
    else if(SPI_TRANSACTION_TYPE_WRITE == object->transactionType)
    {
        retVal = (QSPI_write_v0(handle, transaction));
    }
    else
    {
        transaction->status = SPI_TRANSFER_CANCELED;
        retVal = SPI_STATUS_ERROR;
    }
    return(retVal);
}

static int32_t QSPIWaitReadSramLvl(uint32_t baseAddr, uint32_t *rdLevel); /* for misra warnings */
static int32_t QSPIWaitReadSramLvl(uint32_t baseAddr, uint32_t *rdLevel)
{
    uint32_t retry = QSPI_REG_RETRY;
    uint32_t sramLevel;
    int32_t  retVal = SPI_STATUS_SUCCESS;
    while(retry--)
    {
        sramLevel = QSPIGetSramLvl(baseAddr, 1U);
        if (sramLevel)
        {
            *rdLevel = sramLevel;
            break;
        }
        QSPIdelay(QSPI_POLL_IDLE_DELAY);
    }

    if (retry)
    {
        retVal = SPI_STATUS_SUCCESS;
    }
    else
    {
        retVal = SPI_STATUS_ERROR;
    }
    return(retVal);
}

static int32_t QSPI_ind_xfer_mode_read_v0(SPI_Handle handle,
                                    SPI_Transaction *transaction); /* for misra warnings */
static int32_t QSPI_ind_xfer_mode_read_v0(SPI_Handle handle,
                                    SPI_Transaction *transaction)
{
    QSPI_v0_HwAttrs const *hwAttrs;      /* QSPI hardware attributes */
    QSPI_v0_Object        *object;       /* QSPI object */
    uint8_t               *pDst;         /* Destination address */
    uint32_t               count;        /* transaction length */
    uint32_t               offset;       /* QSPI flash offset address */
    uint32_t               remaining;
    uint32_t               sramLevel = 0U, rdBytes = 0U;
    uint32_t               retFlag = UFALSE;
    int32_t                retVal = SPI_STATUS_SUCCESS;

    /* Copy flash transaction parameters to local variables */
    offset = (uint32_t)((uintptr_t)transaction->arg); /* NOR Flash offset address to read */
    pDst = (uint8_t *)transaction->rxBuf;
    count = transaction->count;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = (const QSPI_v0_HwAttrs *)handle->hwAttrs;
    object = (QSPI_v0_Object*)handle->object;

    /* Setup indirect read transfer */
    QSPIIndReadSetup(hwAttrs->baseAddr, offset, object->transferCmd,
                     object->xferLines, object->rdDummyClks);
    QSPIIndReadExecute(hwAttrs->baseAddr, count);

    if (SPI_OPER_MODE_POLLING == object->intrPollMode)
    {
		remaining = count;
        while(0 < remaining)
        {
            /* Wait indirect read SRAM fifo has data */
            if (QSPIWaitReadSramLvl(hwAttrs->baseAddr, &sramLevel))
            {
                retFlag = UTRUE;
                retVal = SPI_STATUS_ERROR;
                transaction->status = SPI_TRANSFER_FAILED;
                break;
            }

            rdBytes = sramLevel * QSPI_FIFO_WIDTH;
            rdBytes = rdBytes > remaining ? remaining : rdBytes;

            /* Read data from FIFO */
            QSPIReadFifoData(hwAttrs->indTrigAddr, pDst, rdBytes);

            pDst += rdBytes;
            remaining -= rdBytes;
        }
        if(UFALSE == retFlag)
        {
            if (QSPIWaitIndReadComplete(hwAttrs->baseAddr))
            {
                retFlag = UTRUE;
                retVal = SPI_STATUS_ERROR;
                transaction->status=SPI_TRANSFER_FAILED;
            }
        }
    }

    return (retVal);
}

static int32_t QSPI_cmd_mode_read_v0(SPI_Handle handle,
                                  SPI_Transaction *transaction)
{
    QSPI_v0_HwAttrs const *hwAttrs;    /* QSPI hardware attributes */
    uint8_t               *cmd;

    cmd = (uint8_t *)transaction->txBuf;
    /* Get the pointer to the hwAttrs */
    hwAttrs = (const QSPI_v0_HwAttrs *)handle->hwAttrs;

    return (QSPICmdRead(hwAttrs->baseAddr,
                        (uint32_t)cmd[0],
                        (uint8_t *)transaction->rxBuf,
                        transaction->count - QSPI_CMD_LEN_DEFAULT));
}


static int32_t QSPI_read_v0(SPI_Handle handle, SPI_Transaction *transaction)
{
    QSPI_v0_Object *object = NULL;
    int32_t         retVal = SPI_STATUS_SUCCESS;

    object = (QSPI_v0_Object*)handle->object;

    if(QSPI_OPER_MODE_IND_XFER == object->qspiMode)
    {
        retVal = (QSPI_ind_xfer_mode_read_v0(handle, transaction));
    }
    else if(QSPI_OPER_MODE_CFG == object->qspiMode)
    {
        retVal = (QSPI_cmd_mode_read_v0(handle, transaction));
    }
    else
    {
        transaction->status = SPI_TRANSFER_CANCELED;
        retVal = SPI_STATUS_ERROR;
    }
    return(retVal);
}

static int32_t QSPIWaitWriteSramLvl(uint32_t baseAddr, uint32_t *sramLvl); /* for misra warnings */
static int32_t QSPIWaitWriteSramLvl(uint32_t baseAddr, uint32_t *sramLvl)
{
    uint32_t retry = QSPI_REG_RETRY;
    uint32_t sramLevel;
    int32_t  retVal = SPI_STATUS_SUCCESS;

    while(retry--)
    {
        sramLevel = QSPIGetSramLvl(baseAddr, 0);
        if (QSPI_SRAM_WATERMARK_WR_LVL >= sramLevel)
        {
			*sramLvl = sramLevel;
            break;
        }
    }

    if (retry)
    {
        retVal = SPI_STATUS_SUCCESS;
    }
    else
    {
        retVal = SPI_STATUS_ERROR;
    }
    return(retVal);
}

static int32_t QSPI_ind_xfer_mode_write_v0(SPI_Handle handle,
                                    SPI_Transaction *transaction)
{
    QSPI_v0_HwAttrs const *hwAttrs;      /* QSPI hardware attributes */
    QSPI_v0_Object        *object;       /* QSPI object */
    uint8_t               *pSrc;         /* Destination address */
    uint32_t               count;        /* transaction length */
    uint32_t               offset;       /* QSPI flash offset address */
    uint32_t               remaining;
    uint32_t               sramLevel, wrBytes;
    uint32_t               retFlag = UFALSE;
    int32_t                retVal = SPI_STATUS_SUCCESS;

    object = (QSPI_v0_Object*)handle->object;

    /* Copy flash transaction parameters to local variables */
    offset = (uint32_t)((uintptr_t)transaction->arg); /* NOR Flash offset address to write */
    pSrc = (uint8_t*)transaction->txBuf;
    count = transaction->count;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = (const QSPI_v0_HwAttrs *)handle->hwAttrs;
    object = (QSPI_v0_Object*)handle->object;

    /* Setup indirect write transfer */
    QSPIIndWriteSetup(hwAttrs->baseAddr, offset, object->transferCmd, object->xferLines);
    QSPIIndWriteExecute(hwAttrs->baseAddr, count);

    if (SPI_OPER_MODE_POLLING == object->intrPollMode)
    {
        /* Wait Indirect Write  SRAM fill level below the threshold */
        if (QSPIWaitWriteSramLvl(hwAttrs->baseAddr, &sramLevel))
        {
            retFlag = UTRUE;
            retVal = SPI_STATUS_ERROR;
            transaction->status = SPI_TRANSFER_FAILED;
        }
        else
        {
		    remaining = count;
            while(0 < remaining)
            {
                /* Wait indirect write SRAM fifo level below watermark */
                if (QSPIWaitWriteSramLvl(hwAttrs->baseAddr, &sramLevel))
                {
                    retFlag = UTRUE;
                    retVal = SPI_STATUS_ERROR;
                    break;
                }

                wrBytes = (QSPI_SRAM_PARTITION_WR - sramLevel) * QSPI_FIFO_WIDTH;
                wrBytes = wrBytes > remaining ? remaining : wrBytes;

                /* Write data to FIFO */
                QSPIWriteFifoData(hwAttrs->indTrigAddr, pSrc, wrBytes);

                pSrc += wrBytes;
                remaining -= wrBytes;
            }

            if(UFALSE == retFlag)
            {
                if (QSPIWaitIndWriteComplete(hwAttrs->baseAddr))
                {
                    retFlag = UTRUE;
                    retVal = SPI_STATUS_ERROR;
                }
            }
        }
    }

    if(UTRUE == retFlag)
    {
        transaction->status = SPI_TRANSFER_FAILED;
        QSPIIndWriteCancel(hwAttrs->baseAddr);
    }
    return (retVal);
}


static int32_t QSPI_cmd_mode_write_v0(SPI_Handle handle,
                                      const SPI_Transaction *transaction)
{
    QSPI_v0_HwAttrs const *hwAttrs; /* QSPI hardware attributes */
    uint8_t               *txBuf;
    uint32_t               dataLen;
    uint32_t               cmdLen;
    int32_t                retVal = SPI_STATUS_SUCCESS;

    txBuf = (uint8_t *)transaction->txBuf;
    dataLen = (uint32_t)((uintptr_t)transaction->arg);
    cmdLen = transaction->count - dataLen;
    /* Get the pointer to the object and hwAttrs */
    hwAttrs = (const QSPI_v0_HwAttrs *)handle->hwAttrs;

    if (dataLen)
    {
        retVal = (QSPICmdWrite(hwAttrs->baseAddr,
                             txBuf,
                             cmdLen,
                             &txBuf[cmdLen],
                             dataLen));
    }
    else
    {
        retVal = (QSPICmdWrite(hwAttrs->baseAddr,
                             txBuf,
                             cmdLen,
                             NULL,
                             0));
    }
    return (retVal);
}


static int32_t QSPI_write_v0(SPI_Handle handle, SPI_Transaction *transaction)
{
    QSPI_v0_Object  *object = NULL; /* QSPI object */
    int32_t          retVal = SPI_STATUS_SUCCESS;

    /* Get the pointer to the object and hwAttrs */
    object = (QSPI_v0_Object*)handle->object;

    if(QSPI_OPER_MODE_IND_XFER == object->qspiMode)
    {
        retVal = (QSPI_ind_xfer_mode_write_v0(handle, transaction));
    }
    else if(QSPI_OPER_MODE_CFG == object->qspiMode)
    {
        retVal = (QSPI_cmd_mode_write_v0(handle, transaction));
    }
    else
    {
        transaction->status = SPI_TRANSFER_CANCELED;
        retVal = SPI_STATUS_ERROR;
    }
    return (retVal);
}


/*
 *  ======== QSPI_transfer_v0 ========
 */
static bool QSPI_transfer_v0(SPI_Handle handle, SPI_Transaction *transaction)
{
    bool                  ret = BFALSE; /* return value */
    QSPI_v0_Object        *object;      /* QSPI object */
    QSPI_v0_HwAttrs const *hwAttrs;     /* QSPI hardware attributes */

    /* Input parameter validation */
    if ((NULL != handle) && (NULL != transaction))
    {
    /* Get the pointer to the object and hwAttrs */
    object = (QSPI_v0_Object*)handle->object;
    hwAttrs = (const QSPI_v0_HwAttrs *)handle->hwAttrs;

    /* Check if anything needs to be written or read */
    if (0 != transaction->count)
    {
        /* Acquire the lock for this particular I2C handle */
        SPI_osalPendLock(object->mutex, SemaphoreP_WAIT_FOREVER);

        /* Book keeping of transmit and receive buffers. */
        object->writeBufIdx = (uint8_t *)transaction->txBuf;
        object->writeCountIdx = transaction->count;
        object->readBufIdx =  (uint8_t *)transaction->rxBuf;
        object->readCountIdx = transaction->count;


         /*
         * QSPI_primeTransfer_v0 is a longer process and
         * protection is needed from the QSPI interrupt
         */
        if (SPI_OPER_MODE_POLLING != object->intrPollMode)
        {
            SPI_osalHardwareIntrEnable(hwAttrs->intrNum,hwAttrs->eventId);
        }

        if (SPI_STATUS_SUCCESS == QSPI_primeTransfer_v0(handle, transaction))
        {
            if (SPI_OPER_MODE_BLOCKING == object->intrPollMode)
            {
                if (QSPI_OPER_MODE_IND_XFER == object->qspiMode)
                {
                    /* Only in indirect transfer mode, wait for the lock
                       posted form the word completion interrupt */
                    SPI_osalPendLock(object->transferComplete, SemaphoreP_WAIT_FOREVER);
                }

                /* transfer is completed and semaphore is posted. */
                ret = BTRUE;
            }
            else
            {
                /* Always return BTRUE if in Asynchronous mode */
                ret = BTRUE;
            }
            transaction->status = SPI_TRANSFER_COMPLETED; 
            /* Release the lock for this particular I2C handle */
            SPI_osalPostLock(object->mutex);
        } else {
		    transaction->status = SPI_TRANSFER_FAILED;
		}
    } else {
	    transaction->status = SPI_TRANSFER_CANCELED;
	}
    }

    /* Return the number of bytes transferred by the I2C */
    return (ret);
}

/*
 *  ======== QSPI_transferCallback_v0 ========
 */
static void QSPI_transferCallback_v0(SPI_Handle handle, SPI_Transaction *msg)
{
    QSPI_v0_Object   *object; /* QSPI object */

    /* Get the pointer to the object */
    object = (QSPI_v0_Object*)handle->object;

    /* Indicate transfer complete */
    SPI_osalPostLock(object->transferComplete);
}

/*
 *  ======== QSPI_control_v0 ========
 */
static int32_t QSPI_control_v0(SPI_Handle handle, uint32_t cmd, const void *arg)
{
    QSPI_v0_Object   *object; /* QSPI object */
    int32_t           retVal = SPI_STATUS_ERROR;

    /* Input parameter validation */
    if (NULL != handle)
    {
    /* Get the pointer to the object */
    object = (QSPI_v0_Object*)handle->object;

    switch (cmd)
    {
        case SPI_V0_CMD_IND_TRANSFER_CMD:
        {
            object->transferCmd = *(uint32_t *)arg;
            retVal = SPI_STATUS_SUCCESS;
            break;
        }

        case SPI_V0_CMD_TRANSFERMODE_RW:
        {
            object->transactionType = *(uint32_t *)arg;
            retVal = SPI_STATUS_SUCCESS;
            break;
        }

        case SPI_V0_CMD_SETCONFIGMODE:
        {
            object->qspiMode = QSPI_OPER_MODE_CFG;
            retVal = SPI_STATUS_SUCCESS;
            break;
        }

        case SPI_V0_CMD_SETINDXFERMODE:
        {
            object->qspiMode = QSPI_OPER_MODE_IND_XFER;
            retVal = SPI_STATUS_SUCCESS;
            break;
        }

        case SPI_V0_CMD_SETXFERLINES:
        {
            object->xferLines = *(uint32_t *)arg;
            retVal = SPI_STATUS_SUCCESS;
            break;
        }

        case SPI_V0_CMD_RD_DUMMY_CLKS:
        {
            object->rdDummyClks = *(uint32_t *)arg;
            retVal = SPI_STATUS_SUCCESS;
            break;
        }

        default:
        retVal = SPI_STATUS_UNDEFINEDCMD;
        break;
    }
    }

    return retVal;
}


/*
 *  ======== QSPI_transferCancel_v0 ========
 */
static void QSPI_transferCancel_v0(SPI_Handle handle)
{
    return;
}
