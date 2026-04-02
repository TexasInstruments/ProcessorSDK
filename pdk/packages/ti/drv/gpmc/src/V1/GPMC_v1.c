/**
 *  \file   GPMC_v1.c
 *
 *  \brief  GPMC IP Version 1 specific driver APIs implementation.
 *
 *   This file contains the driver APIs for GPMC controller.
 */

/*
 * Copyright (C) 2014-2020 Texas Instruments Incorporated - http://www.ti.com/
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
#include <ti/csl/hw_types.h>
#include <ti/drv/gpmc/src/V1/GPMC_v1.h>
#include <ti/drv/gpmc/src/GPMC_osal.h>


/* GPMC functions */
static void GPMC_init_v1(GPMC_Handle handle);
static GPMC_Handle GPMC_open_v1(GPMC_Handle handle, const GPMC_Params *params);
static void GPMC_close_v1(GPMC_Handle handle);
static bool GPMC_transfer_v1(GPMC_Handle handle, GPMC_Transaction *transaction);
static int32_t GPMC_control_v1(GPMC_Handle handle, uint32_t cmd, void *arg);

static int32_t GPMC_sram_read_v1(GPMC_Handle handle, const GPMC_Transaction *transaction);
static int32_t GPMC_sram_write_v1(GPMC_Handle handle, const GPMC_Transaction *transaction);

/* GPMC function table for GPMC AM57x implementation */
const GPMC_FxnTable GPMC_FxnTable_v1 = {
    &GPMC_init_v1,
    &GPMC_open_v1,
    &GPMC_close_v1,
    &GPMC_transfer_v1,
    &GPMC_control_v1
};

/*
 *  ======== GPMC_init_v1 ========
 */
static void GPMC_init_v1(GPMC_Handle handle)
{
    /* Input parameter validation */
    if (handle != NULL)
    {
        /* Mark the object as available */
        ((GPMC_v1_Object *)(handle->object))->isOpen = (bool)false;
    }
    return;
}

/*
 *  ======== GPMC_transferCallback_v1 ========
 */
static void GPMC_transferCallback_v1(GPMC_Handle handle, GPMC_Transaction *msg)
{
    GPMC_v1_Object   *object; /* GPMC object */

    /* Input parameter validation */
    if (handle != NULL)
    {
        /* Get the pointer to the object */
        object = (GPMC_v1_Object *)handle->object;

        /* Indicate transfer complete */
        GPMC_osalPostLock(object->transferComplete);
    }
}

/*
 *  ======== GPMC_hwiFxn_v1 ========
 *  Hwi interrupt handler to service the GPMC peripheral
 *
 *  The handler is a generic handler for a GPMC object.
 */
static void GPMC_hwiFxn_v1(uintptr_t arg)
{
    GPMC_v1_HwAttrs const *hwAttrs = NULL;
    GPMC_Handle            handle = (GPMC_Handle)arg;

    /* Input parameter validation */
    if (NULL != handle)
    {
        /* Get the pointer to the object and hwAttrs */
        hwAttrs = (GPMC_v1_HwAttrs const *)handle->hwAttrs;

        if (hwAttrs->intcMuxNum != INVALID_INTC_MUX_NUM)
        {
            GPMC_osalMuxIntcDisableHostInt(hwAttrs->intcMuxNum, hwAttrs->intcMuxOutEvent);
            GPMC_osalMuxIntcClearSysInt(hwAttrs->intcMuxNum, hwAttrs->intcMuxInEvent);
        }

        if (hwAttrs->intcMuxNum != INVALID_INTC_MUX_NUM)
        {
            GPMC_osalMuxIntcClearSysInt(hwAttrs->intcMuxNum, hwAttrs->intcMuxInEvent);
            GPMC_osalHardwareIntrClear(hwAttrs->intrNum);
            GPMC_osalMuxIntcEnableHostInt(hwAttrs->intcMuxNum, hwAttrs->intcMuxOutEvent);
        }
    }
}

static void GPMCIntDisableAll(uint32_t baseAddr)
{
	GPMCIntDisable(baseAddr, GPMC_FIFOEVENT_INT);
	GPMCIntDisable(baseAddr, GPMC_TERMINALCOUNT_INT);
	GPMCIntDisable(baseAddr, GPMC_WAIT0EDGEDETECTION_INT);
	GPMCIntDisable(baseAddr, GPMC_WAIT1EDGEDETECTION_INT);
}

static void GPMCIntEnableAll(uint32_t baseAddr)
{
	GPMCIntEnable(baseAddr, GPMC_FIFOEVENT_INT);
	GPMCIntEnable(baseAddr, GPMC_TERMINALCOUNT_INT);
	GPMCIntEnable(baseAddr, GPMC_WAIT0EDGEDETECTION_INT);
	GPMCIntEnable(baseAddr, GPMC_WAIT1EDGEDETECTION_INT);
}

static void GPMCIntClearAll(uint32_t baseAddr)
{
	GPMCIntStatusClear(baseAddr, GPMC_FIFOEVENT_INT);
	GPMCIntStatusClear(baseAddr, GPMC_TERMINALCOUNT_INT);
	GPMCIntStatusClear(baseAddr, GPMC_WAIT0EDGEDETECTION_INT);
	GPMCIntStatusClear(baseAddr, GPMC_WAIT1EDGEDETECTION_INT);
}

/*
 *  ======== GPMC_open_v1 ========
 */
static GPMC_Handle GPMC_open_v1(GPMC_Handle handle, const GPMC_Params *params)
{
    SemaphoreP_Params      semParams;
    uint32_t               key;
    GPMC_v1_Object        *object = NULL;
    GPMC_v1_HwAttrs const *hwAttrs = NULL;
    HwiP_Params            hwiInputParams;
    HwiP_Fxn               hwiFxn;
    MuxIntcP_inParams      muxInParams;
    MuxIntcP_outParams     muxOutParams;
    int32_t                retFlag = 0U;
    uint32_t               timeConfig;
    uint32_t               devType;

    /* Input parameter validation */
    if (handle != NULL)
    {
        /* Get the pointer to the object and hwAttrs */
        object = (GPMC_v1_Object *)handle->object;
        hwAttrs = (GPMC_v1_HwAttrs const *)handle->hwAttrs;

        GPMC_osalHwiParamsInit(&hwiInputParams);

        /* Determine if the device index was already opened */
        key = GPMC_osalHardwareIntDisable();
        if(object->isOpen == (bool)true)
        {
            GPMC_osalHardwareIntRestore(key);
            handle = NULL;
        }
        else
        {
            /* Mark the handle as being used */
            object->isOpen = (bool)true;

            GPMC_osalHardwareIntRestore(key);

            /* Store the GPMC parameters */
            if (params == NULL)
            {
                /* No params passed in, so use the defaults */
                GPMC_Params_init(&(object->gpmcParams));
            }
            else
            {
                /* Copy the params contents */
                object->gpmcParams = *params;
            }

            /* Extract GPMC operating mode based on hwAttrs and input parameters */
            if(GPMC_MODE_BLOCKING == object->gpmcParams.transferMode)
            {
                if(true == hwAttrs->intrEnable)
                {
                    object->intrPollMode = GPMC_OPER_MODE_BLOCKING;
                }
#ifdef GPMC_DMA_ENABLE
                else if (hwAttrs->dmaEnable == TRUE)
                {
                    object->intrPollMode = GPMC_OPER_MODE_BLOCKING;
                }
#endif
                else
                {
                    object->intrPollMode = GPMC_OPER_MODE_POLLING;
                }
            }
            else
            {
                object->intrPollMode = GPMC_OPER_MODE_CALLBACK;
            }

            /* Extract the polling mode from hardware attributes. */
            if(GPMC_OPER_MODE_POLLING != object->intrPollMode)
            {
                if (hwAttrs->intcMuxNum != INVALID_INTC_MUX_NUM)
                {
                    /* Setup intc mux */
                    muxInParams.arg         = (uintptr_t)handle;
                    muxInParams.muxNum      = hwAttrs->intcMuxNum;
                    muxInParams.muxInEvent  = hwAttrs->intcMuxInEvent;
                    muxInParams.muxOutEvent = hwAttrs->intcMuxOutEvent;
                    muxInParams.muxIntcFxn  = (MuxIntcFxn)(&GPMC_hwiFxn_v1);
                    GPMC_osalMuxIntcSetup(&muxInParams, &muxOutParams);

                    hwiFxn                  = (HwiP_Fxn)muxOutParams.muxIntcFxn;
                    hwiInputParams.arg      = muxOutParams.arg;
                }
                else
                {
                    hwiFxn                  = (HwiP_Fxn)(&GPMC_hwiFxn_v1);
                    hwiInputParams.arg      = (uintptr_t)handle;
                }

                /* Setup Hardware Interrupt Controller */
                hwiInputParams.name = NULL;
                hwiInputParams.priority = 0x20;
#if defined (__ARM_ARCH_7A__)
                hwiInputParams.evtId = 0; /* Event ID not used in GIC */
                hwiInputParams.triggerSensitivity = 0x3; /* interrupt edge triggered */
#else
                hwiInputParams.evtId = hwAttrs->eventId;
#endif
                object->hwi = GPMC_osalRegisterInterrupt(hwAttrs->intrNum, hwiFxn, &hwiInputParams);

                if(object->hwi == NULL)
                {
                    GPMC_close_v1(handle);
                    handle = NULL;
                    retFlag = 1U;
                }
            }

            if(retFlag == 0U)
            {
                /*
                 * Construct thread safe handles for this GPMC peripheral
                 * Semaphore to provide exclusive access to the GPMC peripheral
                 */
                GPMC_osalSemParamsInit(&semParams);
                semParams.mode = SemaphoreP_Mode_BINARY;
                object->mutex = GPMC_osalCreateBlockingLock(1U, &semParams);

                /*
                 * Store a callback function that posts the transfer complete
                 * semaphore for synchronous mode
                 */
                if (object->intrPollMode == GPMC_OPER_MODE_BLOCKING)
                {
                    /*
                     * Semaphore to cause the waiting task to block for the GPMC
                     * to finish
                     */
                    object->transferComplete = GPMC_osalCreateBlockingLock(0, &semParams);

                    /* Store internal callback function */
                    object->gpmcParams.transferCallbackFxn = &GPMC_transferCallback_v1;
                }

                if(object->intrPollMode == GPMC_OPER_MODE_CALLBACK)
                {
                    /* Check to see if a callback function was defined for async mode */
                    if (object->gpmcParams.transferCallbackFxn == NULL)
                    {
                        GPMC_close_v1(handle);
                        handle = NULL;
                        retFlag = 1U;
                    }
                }
            }

#ifdef GPMC_DMA_ENABLE
            if ((retFlag == 0U) && (hwAttrs->dmaEnable == TRUE))
            {
                retFlag = GPMC_dmaConfig(handle);
            }
#endif

            if(retFlag == 0U)
            {
                /* Reset the transaction pointer */
                object->transaction = NULL;

                /* Reset GPMC */
                GPMCModuleSoftReset(hwAttrs->gpmcBaseAddr);
                while(!GPMCModuleResetStatusGet(hwAttrs->gpmcBaseAddr));

                /* Set SYSCONFIG register to no idle mode */
                GPMCIdleModeSelect(hwAttrs->gpmcBaseAddr, GPMC_IDLEMODE_NOIDLE);

                /* Disable all interrupts */
                GPMCIntDisableAll(hwAttrs->gpmcBaseAddr);

                /* Timeout control disable */
                GPMCTimeOutFeatureConfig(hwAttrs->gpmcBaseAddr, GPMC_TIMEOUTFEATURE_DISABLE);
                GPMCTimeOutStartValSet(hwAttrs->gpmcBaseAddr, 0);

                /* Set the wait pin polarity */
                GPMCWaitPinSelect(hwAttrs->gpmcBaseAddr,
                                  hwAttrs->chipSel,
                                  hwAttrs->waitPinNum);
                GPMCWaitPinPolaritySelect(hwAttrs->gpmcBaseAddr,
                                          hwAttrs->waitPinNum,
                                          hwAttrs->waitPinPol);

                GPMCCSConfig(hwAttrs->gpmcBaseAddr, hwAttrs->chipSel, GPMC_CS_DISABLE);
                GPMCTimeParaGranularitySelect(hwAttrs->gpmcBaseAddr,
                                              hwAttrs->chipSel,
                                              hwAttrs->timeLatency);

                devType = hwAttrs->devType;
                if (devType == GPMC_DEVICETYPE_SRAMLIKE)
                {
                    /* GPMC SRAM and NOR device use the same configuration */
                    devType = GPMC_DEVICETYPE_NORLIKE;
                }
                GPMCDevTypeSelect(hwAttrs->gpmcBaseAddr, hwAttrs->chipSel, devType);

                GPMCDevSizeSelect(hwAttrs->gpmcBaseAddr, hwAttrs->chipSel, hwAttrs->devSize);

                GPMCAddrDataMuxProtocolSelect(hwAttrs->gpmcBaseAddr,
                                              hwAttrs->chipSel,
                                              hwAttrs->addrDataMux);

                /* by default, read/write async single access */
                GPMCReadTypeSelect(hwAttrs->gpmcBaseAddr,
                                   hwAttrs->chipSel,
                                   hwAttrs->readType);
                GPMCWriteTypeSelect(hwAttrs->gpmcBaseAddr,
                                   hwAttrs->chipSel,
                                   hwAttrs->writeType);

                GPMCAccessTypeSelect(hwAttrs->gpmcBaseAddr,
                                   hwAttrs->chipSel,
                                   GPMC_MODE_READ,
                                   hwAttrs->accessType);
                GPMCAccessTypeSelect(hwAttrs->gpmcBaseAddr,
                                   hwAttrs->chipSel,
                                   GPMC_MODE_WRITE,
                                   hwAttrs->accessType);

                /* Set chip select address */
                GPMCBaseAddrSet(hwAttrs->gpmcBaseAddr,
                                hwAttrs->chipSel,
                                hwAttrs->chipSelBaseAddr >> GPMC_CS_BASE_ADDR_SHIFT);
                GPMCMaskAddrSet(hwAttrs->gpmcBaseAddr,
                                hwAttrs->chipSel,
                                hwAttrs->chipSelAddrSize);

                /* CONFIG2 reister timing config, no extra delay */
                timeConfig = GPMC_CS_TIMING_CONFIG(hwAttrs->timingParams.csWrOffTime,
                                                   hwAttrs->timingParams.csRdOffTime,
                                                   hwAttrs->csExDelay,
                                                   hwAttrs->timingParams.csOnTime);
                GPMCCSTimingConfig(hwAttrs->gpmcBaseAddr,
                                   hwAttrs->chipSel,
                                   timeConfig);

                /* CONFIG3 reister timing config, no extra delay */
                timeConfig = GPMC_ADV_TIMING_CONFIG(hwAttrs->timingParams.advAadMuxWrOffTime,
                                                    hwAttrs->timingParams.advAadMuxRdOffTime,
                                                    hwAttrs->timingParams.advWrOffTime,
                                                    hwAttrs->timingParams.advRdOffTime,
                                                    GPMC_ADV_EXTRA_NODELAY,
                                                    hwAttrs->timingParams.advAadMuxOnTime,
                                                    hwAttrs->timingParams.advOnTime);
                GPMCADVTimingConfig(hwAttrs->gpmcBaseAddr,
                                        hwAttrs->chipSel,
                                        timeConfig);

                /* CONFIG4 reister timing config, no extra delay */
                timeConfig = GPMC_WE_OE_TIMING_CONFIG(hwAttrs->timingParams.weOffTime,
                                                      GPMC_WE_EXTRA_NODELAY,
                                                      hwAttrs->timingParams.weOnTtime,
                                                      hwAttrs->timingParams.oeAadMuxOffTime,
                                                      hwAttrs->timingParams.oeOffTime,
                                                      GPMC_OE_EXTRA_NODELAY,
                                                      hwAttrs->timingParams.oeAadMuxOnTime,
                                                      hwAttrs->timingParams.oeOnTime);
                GPMCWEAndOETimingConfig(hwAttrs->gpmcBaseAddr,
                                        hwAttrs->chipSel,
                                        timeConfig);

                /* CONFIG5 reister timing config */
                timeConfig = GPMC_RDACCESS_CYCLETIME_TIMING_CONFIG(hwAttrs->timingParams.rdCycleTime,
                                                                   hwAttrs->timingParams.wrCycleTime,
                                                                   hwAttrs->timingParams.rdAccessTime,
                                                                   hwAttrs->timingParams.pageBurstAccess);
                GPMCRdAccessAndCycleTimeTimingConfig(hwAttrs->gpmcBaseAddr,
                                                     hwAttrs->chipSel,
                                                     timeConfig);

                /* CONFIG6 reister timing config */
                GPMCWrAccessAndWrDataOnADMUXBusTimingConfig(hwAttrs->gpmcBaseAddr,
                                                            hwAttrs->chipSel,
                                                            hwAttrs->timingParams.wrAcessTime,
                                                            hwAttrs->timingParams.wrDataOnMuxBusTime);

                timeConfig = GPMC_CYCLE2CYCLE_BUSTURNAROUND_TIMING_CONFIG(hwAttrs->timingParams.cycle2CycleDelay,
                                                                          hwAttrs->timingParams.cycleDelaySameChipSel,
                                                                          hwAttrs->timingParams.cycleDelayDiffChipSel,
                                                                          hwAttrs->timingParams.busTurnAroundTime);
                GPMCycle2CycleAndTurnArndTimeTimingConfig(hwAttrs->gpmcBaseAddr,
                                                          hwAttrs->chipSel,
                                                          timeConfig);

                GPMCCSConfig(hwAttrs->gpmcBaseAddr, hwAttrs->chipSel, GPMC_CS_ENABLE);
            }
        }
    }
    return(handle);
}

/*
 *  ======== GPMC_close_v1 ========
 */
static void GPMC_close_v1(GPMC_Handle handle)
{
    GPMC_v1_Object        *object = NULL;
    GPMC_v1_HwAttrs const *hwAttrs = NULL;

    /* Input parameter validation */
    if (handle != NULL)
    {
        /* Get the pointer to the object and hwAttrs */
        object = (GPMC_v1_Object *)handle->object;
        hwAttrs = (GPMC_v1_HwAttrs const *)handle->hwAttrs;

        /* Mask I2C interrupts */
        GPMCIntDisableAll(hwAttrs->gpmcBaseAddr);

        /* Destruct the Hwi */
        if(GPMC_OPER_MODE_POLLING != object->intrPollMode)
        {
            GPMC_osalHardwareIntDestruct(object->hwi);
        }

        /* Destruct the instance lock */
        GPMC_osalDeleteBlockingLock(object->mutex);

        /* Destruct the transfer completion lock */
        if(GPMC_OPER_MODE_BLOCKING == object->intrPollMode)
        {
            GPMC_osalDeleteBlockingLock(object->transferComplete);
        }

#ifdef GPMC_DMA_ENABLE
        if (hwAttrs->dmaEnable == TRUE)
        {
            GPMC_dmaFreeChannel(handle);
        }
#endif
        /* Open flag is set false */
        object->isOpen = (bool)false;
    }

    return;
}

static void GPMC_ctrlNandReadData(GPMC_v1_HwAttrs const *hwAttrs,
                                  uint8_t *pRxData,
                                  uint32_t size)
{
    uint16_t *pData16 = (uint16_t *)pRxData;
    uint8_t  *pData8 = pRxData;

    while(size > 0U)
    {
        if(hwAttrs->devSize == GPMC_DEVICESIZE_32BITS)
        {
            *pData16 = HW_RD_REG16(hwAttrs->gpmcBaseAddr + GPMC_NAND_DATA_N(hwAttrs->chipSel));
            pData16++;
			if (size == 1)
            {
                size = 0;
            }
			else
            {
                size -= 2;
            }
        }
        else
        {
			*pData8 = HW_RD_REG8(hwAttrs->gpmcBaseAddr + GPMC_NAND_DATA_N(hwAttrs->chipSel));
            pData8++;
            size--;
        }
    }
}

static int32_t GPMC_nand_read_v1(GPMC_Handle handle,
                                 const GPMC_Transaction *transaction)
{
    GPMC_v1_Object *object = NULL;
    GPMC_v1_HwAttrs const *hwAttrs = NULL;
    int32_t         retVal = GPMC_STATUS_ERROR;

    /* Input parameter validation */
    if ((handle != NULL) && (transaction != NULL))
    {
        object = (GPMC_v1_Object *)handle->object;
        hwAttrs = (GPMC_v1_HwAttrs const *)handle->hwAttrs;

        if(GPMC_OPER_MODE_POLLING == object->intrPollMode)
        {
            GPMC_ctrlNandReadData(hwAttrs, object->readBufIdx,
                                  object->readCountIdx);
        }
        else
        {
        }
        retVal = GPMC_STATUS_SUCCESS;
    }
    return(retVal);
}

static int32_t GPMC_nor_read_v1(GPMC_Handle handle,
                                const GPMC_Transaction *transaction)
{
    GPMC_v1_Object        *object = NULL;
    GPMC_v1_HwAttrs const *hwAttrs = NULL;
    uint16_t              *pData16, *pAddr16;
    uint8_t               *pData8, *pAddr8;
    uint32_t               size;
    int32_t                retVal = GPMC_STATUS_ERROR;

    /* Input parameter validation */
    if ((handle != NULL) && (transaction != NULL))
    {
        object = (GPMC_v1_Object *)handle->object;
        hwAttrs = (GPMC_v1_HwAttrs const *)handle->hwAttrs;

        size = object->readCountIdx;
        pData8 = (uint8_t *)(object->readBufIdx);
        pAddr8 = (uint8_t *)(hwAttrs->chipSelBaseAddr + transaction->offset);
        pData16 = (uint16_t *)(object->readBufIdx);
        pAddr16 = (uint16_t *)(hwAttrs->chipSelBaseAddr + transaction->offset);

        while(size > 0U)
        {
            if(hwAttrs->devSize == GPMC_DEVICESIZE_16BITS)
            {
                *pData16++ = *pAddr16++;
                if (size == 1)
                {
                    size = 0;
                }
                else
                {
                    size -= 2;
                }
            }
            else
            {
                *pData8++ = *pAddr8++;
                size--;
            }
        }
        retVal = GPMC_STATUS_SUCCESS;
    }
    return(retVal);
}

static void GPMC_ctrlNandWriteData(GPMC_v1_HwAttrs const *hwAttrs,
                                   uint8_t *pTxData,
                                   uint32_t size)
{
    uint16_t *pData16 = (uint16_t *)pTxData;
    uint8_t  *pData8 = pTxData;

    while(size > 0U)
    {
        /* Check if writer buffer is empty */
        while(!GPMCEmptyWriteBuffStatusGet(hwAttrs->gpmcBaseAddr));

        if(hwAttrs->devSize == GPMC_DEVICESIZE_16BITS)
        {
			HW_WR_REG16(hwAttrs->gpmcBaseAddr + GPMC_NAND_DATA_N(hwAttrs->chipSel), *pData16);
			pData16++;
			if (size == 1U)
            {
                size = 0U;
            }
			else
            {
                size -= 2U;
            }
        }
        else
        {
            HW_WR_REG8(hwAttrs->gpmcBaseAddr + GPMC_NAND_DATA_N(hwAttrs->chipSel), *pData8);
            pData8++;
            size--;
        }
    }
}

static int32_t GPMC_nand_write_v1(GPMC_Handle handle,
                                  const GPMC_Transaction *transaction)
{
    GPMC_v1_Object *object = NULL;
    GPMC_v1_HwAttrs const *hwAttrs = NULL;
    int32_t         retVal = GPMC_STATUS_ERROR;

    /* Input parameter validation */
    if ((handle != NULL) && (transaction != NULL))
    {
        object = (GPMC_v1_Object *)handle->object;
        hwAttrs = (GPMC_v1_HwAttrs const *)handle->hwAttrs;

        if(GPMC_OPER_MODE_POLLING == object->intrPollMode)
        {
            GPMC_ctrlNandWriteData(hwAttrs, object->writeBufIdx,
                                   object->writeCountIdx);
        }
        else
        {
        }
        retVal = GPMC_STATUS_SUCCESS;
    }
    return(retVal);
}

static int32_t GPMC_nor_write_v1(GPMC_Handle handle,
                                 const GPMC_Transaction *transaction)
{
    GPMC_v1_Object        *object = NULL;
    GPMC_v1_HwAttrs const *hwAttrs = NULL;
    uint16_t              *pData16, *pAddr16;
    uint8_t               *pData8, *pAddr8;
    uint32_t               size;
    int32_t                retVal = GPMC_STATUS_ERROR;

    /* Input parameter validation */
    if ((handle != NULL) && (transaction != NULL))
    {
        object = (GPMC_v1_Object *)handle->object;
        hwAttrs = (GPMC_v1_HwAttrs const *)handle->hwAttrs;

        size = object->writeCountIdx;
        pData8 = (uint8_t *)(object->writeBufIdx);
        pAddr8 = (uint8_t *)(hwAttrs->chipSelBaseAddr + transaction->offset);
        pData16 = (uint16_t *)(object->writeBufIdx);
        pAddr16 = (uint16_t *)(hwAttrs->chipSelBaseAddr + transaction->offset);

        while(size > 0U)
        {
            if(hwAttrs->devSize == GPMC_DEVICESIZE_16BITS)
            {
                *pAddr16++ = *pData16++;
                if (size == 1)
                {
                    size = 0U;
                }
                else
                {
                    size -= 2U;
                }
            }
            else
            {
                *pAddr8++ = *pData8++;
                size--;
            }
        }
        retVal = GPMC_STATUS_SUCCESS;
    }

    return(retVal);
}

/*
 *  ======== GPMC_primeTransfer_v1 =======
 */
static int32_t GPMC_primeTransfer_v1(GPMC_Handle handle,
                                     const GPMC_Transaction *transaction)
{
    GPMC_v1_Object        *object = NULL;
    GPMC_v1_HwAttrs const *hwAttrs = NULL;
    int32_t                retVal = GPMC_STATUS_ERROR;

    /* Input parameter validation */
    if ((handle != NULL) && (transaction != NULL))
    {
        retVal = GPMC_STATUS_SUCCESS;

        /* Get the pointer to the object and hwAttrs */
        object = (GPMC_v1_Object *)handle->object;
        hwAttrs = (GPMC_v1_HwAttrs const *)handle->hwAttrs;

        /* Disable and clear the interrupts */
        GPMCIntDisableAll(hwAttrs->gpmcBaseAddr);
        GPMCIntClearAll(hwAttrs->gpmcBaseAddr);

        /* Interrupt mode */
        if(object->intrPollMode != GPMC_OPER_MODE_POLLING)
        {
            GPMCIntEnableAll(hwAttrs->gpmcBaseAddr);
        }

        /* Identify the direction of transfer (whether read/write) */
        if(transaction->rxBuf)
        {
            if(hwAttrs->devType == GPMC_DEVICETYPE_NANDLIKE)
            {
                retVal = GPMC_nand_read_v1(handle, transaction);
            }
            else if(hwAttrs->devType == GPMC_DEVICETYPE_NORLIKE)
            {
                retVal = GPMC_nor_read_v1(handle, transaction);
            }
            else
            {
                retVal = GPMC_sram_read_v1(handle, transaction);
            }
        }
        else if(transaction->txBuf)
        {
            if(hwAttrs->devType == GPMC_DEVICETYPE_NANDLIKE)
            {
                retVal = GPMC_nand_write_v1(handle, transaction);
            }
            else if(hwAttrs->devType == GPMC_DEVICETYPE_NORLIKE)
            {
                retVal = GPMC_nor_write_v1(handle, transaction);
            }
            else
            {
                retVal = GPMC_sram_write_v1(handle, transaction);
            }
        }
        else
        {
            retVal = GPMC_STATUS_ERROR;
        }
    }

    return(retVal);
}

static bool GPMC_transfer_v1(GPMC_Handle handle, GPMC_Transaction *transaction)
{
    GPMC_v1_Object        *object;      /* GPMC object */
    GPMC_v1_HwAttrs const *hwAttrs;     /* GPMC hardware attributes */
    bool                   ret = (bool)false; /* return value */
    uintptr_t              key;
    int32_t                status;

    /* Input parameter validation */
    if ((handle != NULL) && (transaction != NULL) && (0U != (uint32_t)transaction->count))
    {
        /* Get the pointer to the object and hwAttrs */
        object = (GPMC_v1_Object *)handle->object;
        hwAttrs = (GPMC_v1_HwAttrs const *)handle->hwAttrs;

        /* Check if a transfer is in progress */
        key = GPMC_osalHardwareIntDisable();
        if (object->transaction != NULL)
        {
            GPMC_osalHardwareIntRestore(key);
            /* Transfer is in progress */
            transaction->status = GPMC_TRANSFER_CANCELED;
        }
        else
        {
            /* Save the pointer to the transaction */
            object->transaction = transaction;
            GPMC_osalHardwareIntRestore(key);

            /* Acquire the lock for this particular GPMC handle */
            GPMC_osalPendLock(object->mutex, SemaphoreP_WAIT_FOREVER);

            /* Book keeping of transmit and receive buffers. */
            object->writeBufIdx = (uint8_t *)transaction->txBuf;
            object->writeCountIdx = transaction->count;
            object->readBufIdx =  (uint8_t *)transaction->rxBuf;
            object->readCountIdx = transaction->count;

            /*
             * GPMC_primeTransfer_v1 is a longer process and
             * protection is needed from the GPMC interrupt
             */
            if (GPMC_OPER_MODE_POLLING != object->intrPollMode)
            {
                GPMC_osalHardwareIntrEnable(hwAttrs->intrNum);
            }

            status = GPMC_primeTransfer_v1(handle, transaction);
            if (status == 0)
            {
                if (object->intrPollMode == GPMC_OPER_MODE_BLOCKING)
                {
                    /* Blocking transfer is completed and semaphore is posted. */
                    GPMC_osalPendLock(object->transferComplete, SemaphoreP_WAIT_FOREVER);
                }

                /* Set status to completed for all the modes */
                transaction->status = GPMC_TRANSFER_COMPLETED;
                ret = (bool)true;

                /* Release the lock for this particular GPMC handle */
                GPMC_osalPostLock(object->mutex);
            }
            else
            {
                transaction->status = GPMC_TRANSFER_FAILED;
            }

            if (object->intrPollMode != (uint32_t)GPMC_OPER_MODE_CALLBACK)
            {
                /* Reset the transaction pointer in block mode */
                object->transaction = NULL;
            }
        }
    }
    else
    {
        if (transaction != NULL)
        {
            transaction->status = GPMC_TRANSFER_CANCELED;
        }
    }

    /* Return the number of bytes transferred by the I2C */
    return (ret);
}

static void GPMC_eccSizeSet(GPMC_v1_HwAttrs const *hwAttrs,
                            uint32_t *size)
{
    GPMCECCSizeValSet(hwAttrs->gpmcBaseAddr, GPMC_ECC_SIZE_0, size[0]);
    GPMCECCSizeValSet(hwAttrs->gpmcBaseAddr, GPMC_ECC_SIZE_1, size[1]);
}

static int32_t GPMC_control_v1(GPMC_Handle handle, uint32_t cmd, void *arg)
{
    GPMC_v1_HwAttrs const *hwAttrs;     /* GPMC hardware attributes */
    uint32_t              *params = (uint32_t *)arg;
    uint32_t               dataAddr;
    uint32_t               dataSize;
    int32_t                retVal = GPMC_STATUS_ERROR;

    /* Input parameter validation */
    if ((handle != NULL) && (arg != NULL))
    {
        retVal = GPMC_STATUS_SUCCESS;

        /* Get the pointer to the hwAttrs */
        hwAttrs = (GPMC_v1_HwAttrs const *)handle->hwAttrs;

        switch (cmd)
        {
            case GPMC_V1_CMD_GETDEVSIZE:
            {
                *params = hwAttrs->devSize;
                break;
            }

            case GPMC_V1_CMD_GETDEVADDR:
            {
                *params = hwAttrs->chipSelBaseAddr;
                break;
            }

            /* NAND device commands */
            case GPMC_V1_CMD_SETNANDCMD:
            {
                HW_WR_REG8(hwAttrs->gpmcBaseAddr + GPMC_NAND_COMMAND_N(hwAttrs->chipSel),
                           (uint8_t)(*params));
                break;
            }

            case GPMC_V1_CMD_SETNANDADDR:
            {
                HW_WR_REG8(hwAttrs->gpmcBaseAddr + GPMC_NAND_ADDRESS_N(hwAttrs->chipSel),
                           (uint8_t)(*params));
                break;
            }

            case GPMC_V1_CMD_WRNANDDATA:
            {
                dataAddr = *params++;
                dataSize = *params;

                GPMC_ctrlNandWriteData(hwAttrs, (uint8_t *)dataAddr, dataSize);
                break;
            }

            case GPMC_V1_CMD_RDNANDDATA:
            {
                dataAddr = *params++;
                dataSize = *params;

                GPMC_ctrlNandReadData(hwAttrs, (uint8_t *)dataAddr, dataSize);
                break;
            }

            case GPMC_V1_CMD_GETWAITPINSTATUS:
            {
                *params = GPMCWaitPinStatusGet(hwAttrs->gpmcBaseAddr, hwAttrs->waitPinNum);
                break;
            }

            case GPMC_V1_CMD_ECCCONTROL:
            {
                if (*params)
                {
                    GPMCECCEnable(hwAttrs->gpmcBaseAddr);
                }
                else
                {
                    GPMCECCDisable(hwAttrs->gpmcBaseAddr);
                }
                break;
            }

            case GPMC_V1_CMD_ECCGETINFO:
            {
                *params++ = hwAttrs->eccAlgo;
                break;
            }

            case GPMC_V1_CMD_ECCSETSIZE:
            {
                GPMC_eccSizeSet(hwAttrs, params);
                break;
            }

            case GPMC_V1_CMD_ECCGETRESULT:
            {
                uint32_t eccResIdx;

                eccResIdx = *params++;
                *params = GPMCECCResultGet(hwAttrs->gpmcBaseAddr, eccResIdx);
                break;
            }


            case GPMC_V1_CMD_ECCGETBCHRESULT:
            {
                uint32_t eccResIdx;

                eccResIdx = *params++;
                *params = GPMCECCBCHResultGet(hwAttrs->gpmcBaseAddr,
                                              eccResIdx, hwAttrs->chipSel);
                break;
            }

            case GPMC_V1_CMD_ELMSETSYNDFRGMT:
            {
                uint32_t synFrgmtId = *params++;
                uint32_t synFrgmtVal = *params;

                elmSyndromeFrgmtSet(hwAttrs->elmBaseAddr, synFrgmtId,
                                    synFrgmtVal, hwAttrs->chipSel);
                break;
            }

            case GPMC_V1_CMD_ELMSTARTERRLOCPROC:
            {
                elmErrLocProcessingStart(hwAttrs->elmBaseAddr, hwAttrs->chipSel);
                break;
            }

            case GPMC_V1_CMD_ELMGETINTSTATUS:
            {
                params[1] = elmIntStatusGet(hwAttrs->elmBaseAddr, params[0]);
                break;
            }

            case GPMC_V1_CMD_ELMCLRINTSTATUS:
            {
                elmIntStatusClear(hwAttrs->elmBaseAddr, *params);
                break;
            }

            case GPMC_V1_CMD_ELMGETERRLOCPROCSTATUS:
            {
                *params = elmErrLocProcessingStatusGet(hwAttrs->elmBaseAddr,
                                                       hwAttrs->chipSel);
                break;
            }

            case GPMC_V1_CMD_ELMGETNUMERRS:
            {
                *params = elmNumOfErrsGet(hwAttrs->elmBaseAddr,
                                          hwAttrs->chipSel);
                break;
            }

            case GPMC_V1_CMD_ELMGETERRLOCADDR:
            {
                uint32_t errNum;

                errNum = *params++;
                *params = elmErrLocBitAddrGet(hwAttrs->elmBaseAddr,
                                              errNum,
                                              hwAttrs->chipSel);
                break;
            }

            default:
                retVal = GPMC_STATUS_UNDEFINEDCMD;
            break;
        }
    }

    return retVal;
}

static int32_t GPMC_sram_read_v1(GPMC_Handle handle,
                                 const GPMC_Transaction *transaction)
{
    GPMC_v1_HwAttrs const *hwAttrs = (GPMC_v1_HwAttrs const *)handle->hwAttrs;
    int32_t                retVal = GPMC_STATUS_ERROR;
    uint32_t               size =  transaction->count;

#ifdef GPMC_DMA_ENABLE
    if (hwAttrs->dmaEnable == TRUE)
    {
        retVal = GPMC_dmaTransfer(handle, transaction);
    }
    else
#endif
    {
        if(hwAttrs->devSize == GPMC_DEVICESIZE_32BITS)
        {
            uint32_t *pSrc = (uint32_t *)(hwAttrs->dataBaseAddr + transaction->offset);
            uint32_t *pDst = (uint32_t *)(transaction->rxBuf);
            uint32_t  remain = size & 0x3;

            if (remain != 0U)
            {
                size = size - remain + 4U;
            }
            while (size != 0U)
            {
                *pDst = *pSrc;
                pSrc++;
                pDst++;
                size -= 4U;
            }
        }
        else if(hwAttrs->devSize == GPMC_DEVICESIZE_16BITS)
        {
            uint16_t *pSrc = (uint16_t *)(hwAttrs->dataBaseAddr + transaction->offset);
            uint16_t *pDst = (uint16_t *)(transaction->rxBuf);
            uint32_t  remain = size & 0x1;

            if (remain != 0U)
            {
                size = size - remain + 2U;
            }
            while (size != 0U)
            {
                *pDst = *pSrc;
                pSrc++;
                pDst++;
                size -= 2U;
            }
        }
        else
        {
            uint8_t *pSrc = (uint8_t *)(hwAttrs->dataBaseAddr + transaction->offset);
            uint8_t *pDst = (uint8_t *)(transaction->rxBuf);

            while (size != 0U)
            {
                *pDst = *pSrc;
                pSrc++;
                pDst++;
                size--;
            }
        }

        retVal = GPMC_STATUS_SUCCESS;
    }
    return(retVal);
}

static int32_t GPMC_sram_write_v1(GPMC_Handle handle,
                                  const GPMC_Transaction *transaction)
{
    GPMC_v1_HwAttrs const *hwAttrs = (GPMC_v1_HwAttrs const *)handle->hwAttrs;
    int32_t                retVal = GPMC_STATUS_ERROR;
    uint32_t               size =  transaction->count;

#ifdef GPMC_DMA_ENABLE
    if (hwAttrs->dmaEnable == TRUE)
    {
        retVal = GPMC_dmaTransfer(handle, transaction);
    }
    else
#endif
    {
        if(hwAttrs->devSize == GPMC_DEVICESIZE_32BITS)
        {
            uint32_t *pSrc = (uint32_t *)(transaction->txBuf);
            uint32_t *pDst = (uint32_t *)(hwAttrs->dataBaseAddr + transaction->offset);
            uint32_t  remain = size & 0x3;

            if (remain != 0U)
            {
                size = size - remain + 4U;
            }
            while (size != 0U)
            {
                *pDst = *pSrc;
                pSrc++;
                pDst++;
                size -= 4U;
            }
        }
        else if(hwAttrs->devSize == GPMC_DEVICESIZE_16BITS)
        {
            uint16_t *pSrc = (uint16_t *)(transaction->txBuf);
            uint16_t *pDst = (uint16_t *)(hwAttrs->dataBaseAddr + transaction->offset);
            uint32_t  remain = size & 0x1;

            if (remain != 0U)
            {
                size = size - remain + 2U;
            }
            while (size != 0U)
            {
                *pDst = *pSrc;
                pSrc++;
                pDst++;
                size -= 2U;
            }
        }
        else
        {
            uint8_t *pSrc = (uint8_t *)(transaction->txBuf);
            uint8_t *pDst = (uint8_t *)(hwAttrs->dataBaseAddr + transaction->offset);

            while (size != 0U)
            {
                *pDst = *pSrc;
                pSrc++;
                pDst++;
                size--;
            }
        }

        retVal = GPMC_STATUS_SUCCESS;
    }
    return(retVal);
}
