/*
 * Copyright (C) 2014-2024 Texas Instruments Incorporated - http://www.ti.com/
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

/**
 *  \file   i2c_drv.c
 *
 *  \brief  I2C Driver high level APIs implementation.
 *
 *   This file contains the driver APIs for I2C controller.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/src/i2c_osal.h>

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Used to check status and initialization */
static uint32_t gisI2CInitDone = UFALSE;

/* Default I2C parameters structure */
const I2C_Params I2C_defaultParams = {
    I2C_MODE_BLOCKING,  /* transferMode */
    NULL,               /* transferCallbackFxn */
    I2C_100kHz,         /* bitRate */
    NULL
};

/* Default UART transaction parameters structure */
const I2C_Transaction I2C_defaultTransaction = {
    (uint32_t)(I2C_TRANS_VALID_PARAM_MASTER_MODE | I2C_TRANS_VALID_PARAM_EXPAND_SA),  /* validParams */
    NULL,                         /* writeBuf */
    0,                            /* writeCount */
    NULL,                         /* readBuf */
    0,                            /* readCount */
    0,                            /* slaveAddress */
    NULL,                         /* arg */
    NULL,                         /* nextPtr */
    I2C_WAIT_FOREVER,             /* timeout */
    BTRUE,                        /* masterMode */
    BFALSE                        /* expandSA */
};

/* Externs */
extern I2C_config_list I2C_config;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

#if defined (BUILD_MCU)
extern void I2C_socInit(void);
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 *  ======== I2C_close ========
 */
void I2C_close(I2C_Handle handle)
{
    /* Input parameter validation */
    if (NULL != handle)
    {
        handle->fxnTablePtr->closeFxn(handle);
    }
}

/*
 *  ======== I2C_control ========
 */
int32_t I2C_control(I2C_Handle handle, uint32_t cmd, void *arg)
{
    int32_t retVal = I2C_STATUS_ERROR;

    /* Input parameter validation */
    if (NULL != handle)
    {
        retVal = handle->fxnTablePtr->controlFxn(handle, cmd, arg);
    }

    return (retVal);
}

/*
 *  ======== I2C_init ========
 */
void I2C_init(void)
{
    uint32_t I2C_count = 0U;

    if(UFALSE == gisI2CInitDone)
    {
        /* Call each driver's init function */
        for (I2C_count = 0U; (NULL != I2C_config[I2C_count].fxnTablePtr); I2C_count++) {
            I2C_config[I2C_count].fxnTablePtr->initFxn((I2C_Handle)&(I2C_config[I2C_count]));
        }

#if defined (BUILD_MCU)
    I2C_socInit();
#endif

        gisI2CInitDone = UTRUE;
    }
}

/*
 *  ======== I2C_open ========
 */
I2C_Handle I2C_open(uint32_t idx, I2C_Params *params)
{
    I2C_Handle handle = NULL;

    /* Input parameter validation */
    if ((NULL != params) && (idx < (sizeof(I2C_config)/sizeof(I2C_config[0]))))
    {
        /* Input parameter validation for NULL callback in case of I2C_MODE_CALLBACK mode */
        if((I2C_MODE_CALLBACK != params->transferMode) || ((I2C_MODE_CALLBACK == params->transferMode) && (NULL != params->transferCallbackFxn)))
        {
            handle = (I2C_Handle)&(I2C_config[idx]);
            handle = handle->fxnTablePtr->openFxn(handle, params);
        }
    }

    return (handle);
}

/*
 *  ======== I2C_Params_init =======
 */
void I2C_Params_init(I2C_Params *params)
{
    /* Input parameter validation */
    if (NULL != params)
    {
        *params = I2C_defaultParams;
    }
}

/*
 *  ======== I2C_transfer ========
 */
int16_t I2C_transfer(I2C_Handle handle, I2C_Transaction *transaction)
{
    int16_t status = I2C_STS_ERR;

    /* Input parameter validation */
    if ((NULL != handle) && (NULL != transaction))
    {
        status = handle->fxnTablePtr->transferFxn(handle, transaction);
    }

    return (status);
}

/*
 *  ======== I2C_transactionInit ========
 */
void I2C_transactionInit(I2C_Transaction *transaction)
{
    *transaction = I2C_defaultTransaction;
}

