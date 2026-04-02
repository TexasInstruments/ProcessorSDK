/**
 *  \file   MIBSPI_drv.c
 *
 *  \brief  MIBSPI Driver high level APIs.
 *
 *   This file contains the driver APIs for MIBSPI.
 */

/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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
#include <stdlib.h>

#include <ti/drv/mibspi/MIBSPI.h>
#include <ti/drv/mibspi/src/MIBSPI_osal.h>
#include <ti/drv/mibspi/src/mibspi_priv.h>
#include <ti/drv/mibspi/src/mibspi_utils.h>

/* Externs */

/* Default SPI parameters structure */
const MIBSPI_Params MIBSPI_defaultParams = {
    .mode               = MIBSPI_MASTER,
    .dataSize           = 16U,
    .frameFormat        = MIBSPI_POL0_PHA0,
    .transferMode       = MIBSPI_MODE_BLOCKING,
    .transferTimeout    = MIBSPI_WAIT_FOREVER,
    .transferCallbackFxn  = NULL,
    .pinMode            = MIBSPI_PINMODE_4PIN_CS,
    .shiftFormat        = MIBSPI_MSB_FIRST,
    .dmaEnable          = (uint8_t)1U,
    .eccEnable          = (uint8_t)0U,
    .csHold             = (uint8_t)0U,
    .txDummyValue       = (uint16_t)0xFFFFU,
    .compatibilityMode  = FALSE,
    .dmaHandle          = NULL,
};

/* Default Master params */
const MIBSPI_MasterModeParams MIBSPI_defaultMasterParams =
{
    /* Master Params */
    .bitRate   = 5000000U,            /* bitRate */
    .numSlaves = 1,                   /* numSlaves */
    .t2cDelay  = 0,                   /* t2cDelay */
    .c2tDelay  = 0,                   /* c2tDelay */
    .wDelay    = 0,                   /* wDelay */
    .slaveProf = 
    {
        [0] = 
        /* Slave - 0 */
        {
            .chipSelect = (uint8_t)0U,                    /* CS0 */
            .ramBufLen  = (uint8_t)MIBSPI_RAM_MAX_ELEM,   /* ramBufLen in number of dataSize */
            .dmaReqLine = 0U,
        },
        [1] = 
        /* Slave - 1 - not valid */
        {
            (uint8_t)MIBSPI_CS_NONE,               /* CS_NONE */
            (uint8_t)0U,                    /* ramBufLen in number of dataSize */
            .dmaReqLine = 0U,
        },
        [2] = 
        /* Slave - 2 - not valid */
        {
            (uint8_t)MIBSPI_CS_NONE,        /* CS_NONE */
            (uint8_t)0U,                    /* ramBufLen in number of dataSize */
            .dmaReqLine = 0U,
        }
    }
};

/* Default Slave params */
const MIBSPI_SlaveModeParams MIBSPI_defaultSlaveParams =
{
    /* Slave Params */
    .dmaReqLine  = 0U, /*!< DMA request line to be used for Slave */
    .chipSelect  = 0U, /* CS0 */               /*!< CS0-CS7 signal number from 0 -7 */
};

static void   *mibspiMutex = NULL;



/*
 *  ======== MIBSPI_close ========
 */
void MIBSPI_close(MIBSPI_Handle handle)
{
    if (handle != NULL)
    {
        if (mibspiMutex != NULL)
        {
            /* Acquire the the SPI driver semaphore */
            (void)MIBSPI_osalPendLock(mibspiMutex, SemaphoreP_WAIT_FOREVER);
        }

        MIBSPI_closeCore(handle);

        if (mibspiMutex != NULL)
        {
            /* Release the the SPI driver semaphorel */
            (void)MIBSPI_osalPostLock(mibspiMutex);
        }
    }
}


/*
 *  ======== MIBSPI_control ========
 */
int32_t MIBSPI_control(MIBSPI_Handle handle, uint32_t cmd, void *arg)
{
    int32_t retVal = MIBSPI_STATUS_ERROR;

    if (handle != NULL)
    {
        retVal = MIBSPI_controlCore(handle, cmd, arg);
    }

    return (retVal);
}


/*
 *  ======== MIBSPI_init ========
 */
void MIBSPI_init(void)
{
    SemaphoreP_Params     semParams;

    /*
     * Construct thread safe handles for SPI driver level
     * Semaphore to provide exclusive access to the SPI APIs
     */
    if (mibspiMutex == NULL)
    {
        MIBSPI_osalSemParamsInit(&semParams);
        semParams.mode = SemaphoreP_Mode_BINARY;
        mibspiMutex = MIBSPI_osalCreateBlockingLock(1U, &semParams);

        MIBSPI_socInit();
        MibspiUtils_init();
    }
}

/*
 *  ======== MIBSPI_open ========
 */
MIBSPI_Handle MIBSPI_open(enum MibSpi_InstanceId mibspiInstanceId, MIBSPI_Params *params)
{
    MIBSPI_Handle hSpi;

    hSpi = MIBSPI_socGetInstHandle(mibspiInstanceId);
    if (hSpi != NULL)
    {
        if (mibspiMutex != NULL)
        {
            /* Acquire the the SPI driver semaphore */
            (void)MIBSPI_osalPendLock(mibspiMutex, SemaphoreP_WAIT_FOREVER);
        }

        hSpi = MIBSPI_openCore(hSpi, params);

        if (mibspiMutex != NULL)
        {
            /* Release the the SPI driver semaphorel */
            (void)MIBSPI_osalPostLock(mibspiMutex);
        }
    }
    return hSpi;
}

/*
 *  ======== MIBSPI_Params_init ========
 */
void MIBSPI_Params_init(MIBSPI_Params *params)
{
    if (params != NULL)
    {
        *params = MIBSPI_defaultParams;
        if(params->mode == MIBSPI_MASTER)
        {
            /* Set Master Params */
            params->u.masterParams = MIBSPI_defaultMasterParams;
        }
        else
        {
            /* Set Slave Params */
            params->u.slaveParams = MIBSPI_defaultSlaveParams;
        }
    }
}

/*
 *  ======== MIBSPI_transfer ========
 */
bool MIBSPI_transfer(MIBSPI_Handle handle, MIBSPI_Transaction *transaction)
{
    bool status = (bool)false;

    if ((handle != NULL) && (transaction != NULL))
    {
        status = MIBSPI_transferCore(handle, transaction);
    }
    return status;
}

/*
 *  ======== MIBSPI_transferCancel ========
 */
void MIBSPI_transferCancel(MIBSPI_Handle handle)
{
    if (handle != NULL)
    {
        MIBSPI_transferCancelCore(handle);
    }
}


/*
 *  ======== MIBSPI_getStats ========
 */
int32_t MIBSPI_getStats(MIBSPI_Handle handle, MIBSPI_Stats *ptrStats)
{
    return (MIBSPI_control(handle, MIBSPI_CMD_GET_STATS, ptrStats));

}


/*!
 * \brief De-initialize utils module
 */
void MIBSPI_deInit(void)
{
    /*
     * Construct thread safe handles for SPI driver level
     * Semaphore to provide exclusive access to the SPI APIs
     */
    if (mibspiMutex != NULL)
    {
        MIBSPI_osalDeleteBlockingLock(mibspiMutex);
        mibspiMutex = NULL;
        MibspiUtils_deInit();
    }
}

