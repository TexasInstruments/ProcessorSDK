/**
 *  \file   SPI_drv.c
 *
 *  \brief  SPI Driver high level APIs.
 *
 *   This file contains the driver APIs for SPI.
 */

/*
 * Copyright (C) 2014-2017 Texas Instruments Incorporated - http://www.ti.com/
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

#include <ti/drv/spi/MCSPI.h>
#include <stdint.h>
#include <stdlib.h>
#include <ti/drv/spi/src/SPI_osal.h>


/* Externs */
extern const SPI_config_list SPI_config;
extern const OSPI_config_list OSPI_config;
extern MCSPI_config_list MCSPI_config;

/* Used to check status and initialization */
static int32_t SPI_count = (-((int32_t)1));
static int32_t SPI_domain = (-((int32_t)1));
static int32_t OSPI_count = (-((int32_t)1));
static int32_t OSPI_domain = (-((int32_t)1));

static void   *spiMutex = NULL;

/* Default SPI parameters structure */
const SPI_Params SPI_defaultParams = {
    SPI_MODE_BLOCKING,  /* transferMode */
    0U,                 /* transferTimeout */
    NULL,               /* transferCallbackFxn */
    SPI_MASTER,         /* mode */
    1000000,            /* bitRate */
    8U,                 /* dataSize */
    SPI_POL0_PHA0,      /* frameFormat */
    NULL                /* custom */
};

/* Default MCSPI parameters structure */
const MCSPI_Params MCSPI_defaultParams = {
    SPI_MODE_BLOCKING,  /* transferMode */
    0U,                 /* transferTimeout */
    NULL,               /* transferCallbackFxn */
    SPI_MASTER,         /* mode */
    1000000,            /* bitRate */
    8U,                 /* dataSize */
    SPI_POL0_PHA0,      /* frameFormat */
    NULL                /* custom */
};

/* Default OSPI parameters structure */
const OSPI_Params OSPI_defaultParams = {
    OSPI_MODE_BLOCKING, /* transferMode */
    0U,                 /* transferTimeout */
    NULL,               /* transferCallbackFxn */
    OSPI_MASTER,        /* mode */
    1000000,            /* bitRate */
    8U,                 /* dataSize */
    OSPI_POL0_PHA0,     /* frameFormat */
    NULL                /* custom */
};

/*
 *  ======== SPI_close ========
 */
void SPI_close(SPI_Handle handle)
{
    if (NULL != handle)
    {
        if (NULL != spiMutex)
        {
            /* Acquire the the SPI driver semaphore */
            (void)SPI_osalPendLock(spiMutex, SemaphoreP_WAIT_FOREVER);
        }

        handle->fxnTablePtr->closeFxn(handle);

        if (NULL != spiMutex)
        {
            /* Release the the SPI driver semaphorel */
            (void)SPI_osalPostLock(spiMutex);
        }
    }
}

/*
 *  ======== SPI_control ========
 */
int32_t SPI_control(SPI_Handle handle, uint32_t cmd, void *arg)
{
    int32_t retVal = SPI_STATUS_ERROR;

    if (NULL != handle)
    {
        retVal = handle->fxnTablePtr->controlFxn(handle, cmd, arg);
    }

    return (retVal);
}

/*
 *  ======== SPI_init ========
 */
void SPI_init(void)
{
    SemaphoreP_Params     semParams;

    if ((-((int32_t)1)) == SPI_count) {
        /* Call each driver's init function */
        for (SPI_domain = 0; SPI_domain < (int32_t)SPI_MAX_DOMAIN_CNT; SPI_domain++) {
            for (SPI_count = 0; (SPI_count < (int32_t)SPI_MAX_CONFIG_CNT) && (SPI_config[SPI_domain][SPI_count].fxnTablePtr != NULL); SPI_count++) {
                SPI_config[SPI_domain][SPI_count].fxnTablePtr->spiInitFxn((SPI_Handle)&(SPI_config[SPI_domain][SPI_count]));
            }
        }
    }

    /*
     * Construct thread safe handles for SPI driver level
     * Semaphore to provide exclusive access to the SPI APIs
     */
    if (NULL == spiMutex)
    {
        SPI_osalSemParamsInit(&semParams);
        semParams.mode = SemaphoreP_Mode_BINARY;
        spiMutex = SPI_osalCreateBlockingLock(1U, &semParams);
    }
}

/*
 *  ======== SPI_open ========
 */
SPI_Handle SPI_open(uint32_t domain, uint32_t index, SPI_Params *params)
{
    SPI_Handle handle = NULL;

    if (NULL != (SPI_Handle)&(SPI_config[domain][index]))
    {
        /* Get handle for this driver instance */
        handle = (SPI_Handle)&(SPI_config[domain][index]);

        if (NULL != spiMutex)
        {
            /* Acquire the the SPI driver semaphore */
            (void)SPI_osalPendLock(spiMutex, SemaphoreP_WAIT_FOREVER);
        }

        handle = handle->fxnTablePtr->openFxn(handle, params);

        if (NULL != spiMutex)
        {
            /* Release the the SPI driver semaphorel */
            (void)SPI_osalPostLock(spiMutex);
        }
    }

    return (handle);
}

/*
 *  ======== OSPI_close ========
 */
void OSPI_close(OSPI_Handle handle)
{
    if (NULL != handle)
    {
        if (NULL != spiMutex)
        {
            /* Acquire the the SPI driver semaphore */
            (void)SPI_osalPendLock(spiMutex, SemaphoreP_WAIT_FOREVER);
        }

        handle->fxnTablePtr->closeFxn(handle);

        if (NULL != spiMutex)
        {
            /* Release the the SPI driver semaphorel */
            (void)SPI_osalPostLock(spiMutex);
        }
    }
}

/*
 *  ======== OSPI_control ========
 */
int32_t OSPI_control(OSPI_Handle handle, uint32_t cmd, void *arg)
{
    int32_t retVal = SPI_STATUS_ERROR;

    if (NULL != handle)
    {
        retVal = handle->fxnTablePtr->controlFxn(handle, cmd, arg);
    }

    return (retVal);
}

/*
 *  ======== OSPI_init ========
 */
void OSPI_init(void)
{
    SemaphoreP_Params     semParams;

    if ((-((int32_t)1)) == OSPI_count) {
        /* Call each driver's init function */
        for (OSPI_domain = 0; OSPI_domain < (int32_t)OSPI_MAX_DOMAIN_CNT; OSPI_domain++) {
            for (OSPI_count = 0; (OSPI_count < (int32_t)OSPI_MAX_CONFIG_CNT) && (OSPI_config[OSPI_domain][OSPI_count].fxnTablePtr != NULL); OSPI_count++) {
                OSPI_config[OSPI_domain][OSPI_count].fxnTablePtr->spiInitFxn((OSPI_Handle)&(OSPI_config[OSPI_domain][OSPI_count]));
            }
        }
    }

    /*
     * Construct thread safe handles for SPI driver level
     * Semaphore to provide exclusive access to the SPI APIs
     */
    if (NULL == spiMutex)
    {
        SPI_osalSemParamsInit(&semParams);
        semParams.mode = SemaphoreP_Mode_BINARY;
        spiMutex = SPI_osalCreateBlockingLock(1U, &semParams);
    }
}

/*
 *  ======== OSPI_open ========
 */
OSPI_Handle OSPI_open(uint32_t domain, uint32_t index, OSPI_Params *params)
{
    OSPI_Handle handle = NULL;

    if (NULL != (OSPI_Handle)&(OSPI_config[domain][index]))
    {
        /* Get handle for this driver instance */
        handle = (OSPI_Handle)&(OSPI_config[domain][index]);

        if (NULL != spiMutex)
        {
            /* Acquire the the SPI driver semaphore */
            (void)SPI_osalPendLock(spiMutex, SemaphoreP_WAIT_FOREVER);
        }

        handle = handle->fxnTablePtr->openFxn(handle, params);

        if (NULL != spiMutex)
        {
            /* Release the the SPI driver semaphorel */
            (void)SPI_osalPostLock(spiMutex);
        }
    }

    return (handle);
}


/*
 *  ======== OSPI_Params_init ========
 */
void OSPI_Params_init(OSPI_Params *params)
{
    if (NULL != params)
    {
        *params = OSPI_defaultParams;
    }
}

/*
 *  ======== OSPI_serviceISR ========
 */
void OSPI_serviceISR(OSPI_Handle handle)
{
    if (NULL != handle)
    {
        handle->fxnTablePtr->serviceISRFxn(handle);
    }
}

/*
 *  ======== OSPI_transfer ========
 */
bool OSPI_transfer(OSPI_Handle handle, OSPI_Transaction *transaction)
{
    bool retVal = BFALSE;

    if ((NULL != handle) && (NULL != transaction))
    {
        retVal = handle->fxnTablePtr->transferFxn(handle, transaction);
    }

    return (retVal);
}

/*
 *  ======== OSPI_transferCancel ========
 */
void OSPI_transferCancel(OSPI_Handle handle)
{
    if (NULL != handle)
    {
        handle->fxnTablePtr->transferCancelFxn(handle);
    }
}

/*
 *  ======== SPI_Params_init ========
 */
void SPI_Params_init(SPI_Params *params)
{
    if (NULL != params)
    {
        *params = SPI_defaultParams;
    }
}

/*
 *  ======== SPI_serviceISR ========
 */
void SPI_serviceISR(SPI_Handle handle)
{
    if (NULL != handle)
    {
        handle->fxnTablePtr->serviceISRFxn(handle);
    }
}

/*
 *  ======== SPI_transfer ========
 */
bool SPI_transfer(SPI_Handle handle, SPI_Transaction *transaction)
{
    bool retVal = BFALSE;

    if ((NULL != handle) && (NULL != transaction))
    {
        retVal = handle->fxnTablePtr->transferFxn(handle, transaction);
    }

    return (retVal);
}

/*
 *  ======== SPI_transferCancel ========
 */
void SPI_transferCancel(SPI_Handle handle)
{
    if (NULL != handle)
    {
        handle->fxnTablePtr->transferCancelFxn(handle);
    }
}

/*
 *  ======== MCSPI_close ========
 */
void MCSPI_close(MCSPI_Handle handle)
{
    if (NULL != handle)
    {
        if (NULL != spiMutex)
        {
            /* Acquire the the SPI driver semaphore */
            (void)SPI_osalPendLock(spiMutex, SemaphoreP_WAIT_FOREVER);
        }

        handle->fxnTablePtr->closeFxn(handle);

        if (NULL != spiMutex)
        {
            /* Release the the SPI driver semaphorel */
            (void)SPI_osalPostLock(spiMutex);
        }
    }
}

/*
 *  ======== MCSPI_control ========
 */
int32_t MCSPI_control(MCSPI_Handle handle, uint32_t cmd, void *arg)
{
    int32_t retVal = SPI_STATUS_ERROR;

    if (NULL != handle)
    {
        retVal = handle->fxnTablePtr->controlFxn(handle, cmd, arg);
    }

    return (retVal);
}

/*
 *  ======== MCSPI_init ========
 */
void MCSPI_init(void)
{
    SPI_init();
}

/*
 *  ======== MCSPI_open ========
 */
MCSPI_Handle MCSPI_open(uint32_t domain, uint32_t index, uint32_t channel, MCSPI_Params *params)
{
    MCSPI_Handle handle = NULL;

    if (NULL != (MCSPI_Handle)&(MCSPI_config[domain][index][channel]))
    {
        /* Get handle for this driver instance */
        handle = (MCSPI_Handle)&(MCSPI_config[domain][index][channel]);

        if (NULL != spiMutex)
        {
            /* Acquire the the SPI driver semaphore */
            (void)SPI_osalPendLock(spiMutex, SemaphoreP_WAIT_FOREVER);
        }

        handle = handle->fxnTablePtr->openFxn(handle, params);

        if (NULL != spiMutex)
        {
            /* Release the the SPI driver semaphorel */
            (void)SPI_osalPostLock(spiMutex);
        }
    }

    return (handle);
}

/*
 *  ======== MCSPI_Params_init ========
 */
void MCSPI_Params_init(MCSPI_Params *params)
{
    if (NULL != params)
    {
        *params = MCSPI_defaultParams;
    }
}

/*
 *  ======== _transfer ========
 */
bool MCSPI_transfer(MCSPI_Handle handle, SPI_Transaction *transaction)
{
    bool retVal = BFALSE;

    if ((NULL != handle) && (NULL != transaction))
    {
        retVal = handle->fxnTablePtr->transferFxn(handle, transaction);
    }

    return (retVal);
}

/*
 *  ======== _transferCancel ========
 */
void MCSPI_transferCancel(MCSPI_Handle handle)
{
    if (NULL != handle)
    {
        handle->fxnTablePtr->transferCancelFxn(handle);
    }
}
