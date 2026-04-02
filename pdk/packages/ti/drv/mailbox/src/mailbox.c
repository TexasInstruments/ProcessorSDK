/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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

/**
 *  \file mailbox.c
 *
 *  \brief The file implements the Mailbox Driver.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <ti/drv/mailbox/mailbox.h>
#include <ti/drv/mailbox/src/mailbox_internal.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/**
 * @brief
 *  Global Variable for tracking information required by the mailbox driver.
 */
Mailbox_MCB gMailboxMCB = {0};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 *  ======== Mailbox_init ========
 */
int32_t Mailbox_init(Mailbox_initParams *initParam)
{
    int32_t    retVal = MAILBOX_SOK;
    int32_t    i;
    int32_t    key;

    if ((initParam == NULL) ||
        (initParam->osalPrms.disableAllIntr == NULL) ||
        (initParam->osalPrms.restoreAllIntr == NULL))
    {
        retVal = MAILBOX_EINVAL;
    }
    if (retVal == MAILBOX_SOK)
    {
        retVal = Mailbox_validateLocalEndPoint(initParam->localEndpoint);
    }
    if (retVal == MAILBOX_SOK)
    {
        /* Critical Section Protection */
        key = initParam->osalPrms.disableAllIntr();
        if(gMailboxMCB.initFlag == 0)
        {
            memcpy(&gMailboxMCB.initParam, initParam, sizeof(gMailboxMCB.initParam));

            /* Initialize global variables */

            /* Set local endpoint which is the same for all instances of the driver in this processor */
            gMailboxMCB.localEndpoint        = initParam->localEndpoint;
            gMailboxMCB.totalInstCnt         = 0U;
            for (i=0; i< MAILBOX_MAX_NUM_REMOTES_ENDPOINTS; i++)
            {
                Mailbox_RemoteCfg_init(&gMailboxMCB.remoteConfig[i]);
            }
            memset((void *)&(gMailboxMCB.errCnt), 0, sizeof(gMailboxMCB.errCnt));
            memset((void *)&(gMailboxMCB.hwiHandles), 0, sizeof(gMailboxMCB.hwiHandles));

            gMailboxMCB.initFlag = 1;
        }
        else
        {
            DebugP_log0("MAILBOX: Mailbox_init error! Mailbox Driver has already been initialized\n");
            retVal = MAILBOX_EINITIALIZED;
        }
        /* Release the critical section: */
        initParam->osalPrms.restoreAllIntr(key);
    }

    return retVal;
}

int32_t Mailbox_deinit(void)
{
    int32_t retVal = MAILBOX_SOK;
    int32_t key;

    if ((gMailboxMCB.initParam.osalPrms.disableAllIntr == NULL) ||
        (gMailboxMCB.initParam.osalPrms.restoreAllIntr == NULL))
    {
        retVal = MAILBOX_EINVAL;
    }

    if (retVal == MAILBOX_SOK)
    {
        key = gMailboxMCB.initParam.osalPrms.disableAllIntr();
        if (gMailboxMCB.initFlag == 1)
        {
            gMailboxMCB.initFlag = 0;
        }
        gMailboxMCB.initParam.osalPrms.restoreAllIntr(key);
    }

    return retVal;
}

/*
 *  ======== Mailbox_open ========
 */
extern Mbox_Handle Mailbox_open(Mailbox_openParams *openParam,  int32_t* errCode)
{
    Mailbox_Driver*         mailboxDriver;
    Mbox_Handle             retHandle = NULL;
    int32_t                 key;
    void*                   mailboxHwCfg = NULL;
    int32_t                 retVal = MAILBOX_SOK;
    Mailbox_RemoteCfg*      remoteCfg = NULL;
    uint32_t                i;
    uint8_t                 invalidOsal = FALSE;

    *errCode = 0;

    if ((gMailboxMCB.initParam.osalPrms.disableAllIntr == NULL) ||
        (gMailboxMCB.initParam.osalPrms.restoreAllIntr == NULL))
    {
        *errCode = MAILBOX_EINVAL;
        invalidOsal = TRUE;
        goto exit;
    }

    /* Critical Section Protection*/
    key = gMailboxMCB.initParam.osalPrms.disableAllIntr();

    /* Sanity Check: Validate the arguments */
    if((gMailboxMCB.initFlag == 0) || (openParam == NULL))
    {
        DebugP_log0("MAILBOX: Mailbox_open error! Mailbox Driver open failed due to bad input argument.\n");
        /* Error: Invalid argument or usage */
        *errCode = MAILBOX_EINVAL;
        goto exit;
    }

    retVal = Mailbox_validateRemoteEndPoint(gMailboxMCB.localEndpoint, openParam->remoteEndpoint);
    if (retVal != MAILBOX_SOK)
    {
        DebugP_log0("MAILBOX: Mailbox_open error! Mailbox Driver open failed due to bad input argument.\n");
        /* Error: Invalid argument or usage */
        *errCode = MAILBOX_EINVAL;
        goto exit;
    }

    /* Check if the driver for this remote is already opened and get the handle. */
    for (i=0; i<MAILBOX_MAX_NUM_REMOTES_ENDPOINTS; i++)
    {
        if(gMailboxMCB.remoteConfig[i].remoteEndpoint == openParam->remoteEndpoint)
        {
            break;
        }
    }
    if(i == MAILBOX_MAX_NUM_REMOTES_ENDPOINTS)
    {
        /* Driver opened first time for this remote. */
        for (i=0; i<MAILBOX_MAX_NUM_REMOTES_ENDPOINTS; i++)
        {
            if (gMailboxMCB.remoteConfig[i].remoteEndpoint == MAILBOX_INST_INVALID)
            {
                break;
            }
        }
        if (i == MAILBOX_MAX_NUM_REMOTES_ENDPOINTS)
        {
            /* Error: Invalid argument or usage */
            *errCode = MAILBOX_EINVAL;
            goto exit;
        }
        else
        {
            gMailboxMCB.remoteConfig[i].remoteEndpoint = openParam->remoteEndpoint;
        }
        remoteCfg = &(gMailboxMCB.remoteConfig[i]);
    }
    else
    {
        /* Driver is already opened for this remote. */
        remoteCfg = &(gMailboxMCB.remoteConfig[i]);
    }

    /* Check if this is a valid open of a new instance of the driver.
       check if the remote supports multiple instances of the driver.*/

    /*Check if the channel type is valid*/
    if(openParam->cfg.chType > MAILBOX_CHTYPE_MAX)
    {
        DebugP_log1("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad Channle type parameter (%d).\n",openParam->cfg.chType);
        /* Error: Invalid configuration */
        *errCode = MAILBOX_EBADCHTYPE;
        goto exit;
    }

    if(openParam->cfg.chType == MAILBOX_CHTYPE_MULTI)
    {
        retVal = Mailbox_isMultiChannelSupported(gMailboxMCB.localEndpoint, openParam->remoteEndpoint);
        if (retVal != MAILBOX_SOK)
        {
            DebugP_log1("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad Channel type parameter (%d).\n",openParam->cfg.chType);
            *errCode = MAILBOX_EBADCHTYPE;
            goto exit;
        }
    }

    /*Check if channel type is consistent*/
    if(remoteCfg->instCount == 0)
    {
        /*First instance of remote. Set global variable to store channel type that must be used for all channel instances.*/
        remoteCfg->chType = openParam->cfg.chType;
    }
    else
    {
        if((remoteCfg->chType != MAILBOX_CHTYPE_MULTI) || (openParam->cfg.chType != MAILBOX_CHTYPE_MULTI))
        {
            DebugP_log1("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad Channel type parameter (%d).\n",openParam->cfg.chType);
            DebugP_log0("MAILBOX: When using multiple instances all channels must be of type MAILBOX_CHTYPE_MULTI.\n");
            /* Error: Invalid configuration */
            *errCode = MAILBOX_EBADCHTYPE;
            goto exit;
        }
    }

    /*Check if channel ID is consistent. Note that chId is only used for multi-channel.
      Single-channel does not use header or channel ID.*/
    if(openParam->cfg.chType == MAILBOX_CHTYPE_MULTI)
    {
        /*For multi-channel need to check ID value*/
        /*Check if the ID is valid*/
        if(openParam->cfg.chId > MAILBOX_CH_ID_MAX)
        {
            DebugP_log1("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad Channle ID parameter (%d).\n",openParam->cfg.chId);
            /* Error: Invalid configuration */
            *errCode = MAILBOX_EBADCHID;
            goto exit;
        }

        /*Check if the ID is already in use*/
        if(remoteCfg->handleArray[openParam->cfg.chId] != NULL)
        {
            DebugP_log1("MAILBOX: Mailbox_open error! Mailbox Driver open failed. Channel ID (%d) is already in use.\n",openParam->cfg.chId);
            /* Error: Invalid configuration */
            *errCode = MAILBOX_EBADCHID;
            goto exit;
        }
    }


    /* Check if configuration is valid */
    retVal = Mailbox_validateDataTransferMode(openParam->cfg.dataTransferMode);
    if (retVal != MAILBOX_SOK)
    {
        *errCode = MAILBOX_EINVALCFG;
        goto exit;
    }
    retVal = Mailbox_validateReadWriteMode(openParam->cfg.readMode, openParam->cfg.writeMode);
    if (retVal != MAILBOX_SOK)
    {
        *errCode = MAILBOX_EINVALCFG;
        goto exit;
    }

    if (openParam->cfg.writeMode == MAILBOX_MODE_BLOCKING ||
        openParam->cfg.readMode == MAILBOX_MODE_BLOCKING)
    {
        if ((gMailboxMCB.initParam.osalPrms.createMutex == NULL) ||
            (gMailboxMCB.initParam.osalPrms.deleteMutex == NULL) ||
            (gMailboxMCB.initParam.osalPrms.lockMutex == NULL) ||
            (gMailboxMCB.initParam.osalPrms.unlockMutex == NULL))
        {
            *errCode = MAILBOX_EINVALCFG;
            goto exit;
        }
    }

    mailboxHwCfg = Mailbox_getHwCfg (openParam->remoteEndpoint);
    if (mailboxHwCfg == NULL)
    {
        DebugP_log0("MAILBOX: Unable to find hardware cfg params.\n");
        /* Error: Invalid configuration */
        *errCode = MAILBOX_EINVALCFG;
        goto exit;
    }

    /* Allocate memory for the Mailbox Driver */
    mailboxDriver = (Mailbox_Driver *)Mailbox_allocDriver (openParam->remoteEndpoint);
    if(mailboxDriver == NULL)
    {
        DebugP_log0("MAILBOX: Mailbox_open error! Mailbox Driver memory allocation failed.\n");
        /* Error: No memory available. */
        *errCode = MAILBOX_ENOMEM;
        goto exit;
    }

    /* Initialize the allocated memory: */
    memset((void *)mailboxDriver, 0, sizeof(Mailbox_Driver));

    /* Populate the driver with initial configuration */
    memcpy((void *)&mailboxDriver->cfg, (const void *)&openParam->cfg, sizeof(Mailbox_Config));

    mailboxDriver->remoteEndpoint = openParam->remoteEndpoint;
    mailboxDriver->localEndpoint  = gMailboxMCB.localEndpoint;

    /* Populate hardware configuration */
    mailboxDriver->hwCfg = mailboxHwCfg;
    remoteCfg->hwCfgPtr  = mailboxHwCfg;

    mailboxDriver->remoteCfgPtr = remoteCfg;

    /* Setup the return handle: */
    retHandle = (Mbox_Handle)mailboxDriver;

    /* Is the write mode blocking? */
    if(mailboxDriver->cfg.writeMode == MAILBOX_MODE_BLOCKING)
    {
        /* YES: Create a binary semaphore which is used to handle the Blocking operation. */
        mailboxDriver->writeSem = gMailboxMCB.initParam.osalPrms.createMutex();
    }

    /* Is the read mode blocking? */
    if(mailboxDriver->cfg.readMode == MAILBOX_MODE_BLOCKING)
    {
        /* YES: Create a binary semaphore which is used to handle the Blocking operation. */
        mailboxDriver->readSem = gMailboxMCB.initParam.osalPrms.createMutex();
    }

    /* Instance of the driver for the specific endpoint is created */
    if (openParam->cfg.chType == MAILBOX_CHTYPE_SINGLE)
    {
        /* Store the handle of driver in first channel.*/
        remoteCfg->handleArray[0U] = retHandle;
    }
    else
    {
        /* Store handle of driver into hash array */
        remoteCfg->handleArray[openParam->cfg.chId] = retHandle;
    }

    *errCode = Mailbox_registerInterrupts(retHandle);
    if (*errCode != MAILBOX_SOK)
    {
        /* Instance of the driver for the specific endpoint is created */
        if (openParam->cfg.chType == MAILBOX_CHTYPE_SINGLE)
        {
            /* Store the handle of driver in first channel.*/
            remoteCfg->handleArray[0U] = NULL;
        }
        else
        {
            /* Store handle of driver into hash array */
            remoteCfg->handleArray[openParam->cfg.chId] = NULL;
        }
        goto exit;
    }

    gMailboxMCB.totalInstCnt++;
    remoteCfg->instCount++;

    DebugP_log1("MAILBOX: Mailbox Driver (%p) open successful.\n", (uintptr_t)mailboxDriver);

exit:
    if (*errCode != 0)
    {
        /* Restore partially initialized global driver data. */
        if ((remoteCfg != NULL) && (remoteCfg->instCount == 0))
        {
            /* This is first driver open which resulted in error. */
            Mailbox_RemoteCfg_init(remoteCfg);
        }
        if (retHandle != NULL)
        {
            Mailbox_freeDriver(retHandle);
            retHandle = NULL;
        }
    }
    if (invalidOsal == FALSE)
    {
        /* Release the critical section: */
        gMailboxMCB.initParam.osalPrms.restoreAllIntr(key);
    }
    return retHandle;
}

/*
 *  ======== Mailbox_close ========
 */
int32_t Mailbox_close(Mbox_Handle handle)
{
    int32_t            retVal = MAILBOX_SOK;
    Mailbox_Driver*    driver;
    int32_t            key;
    Mailbox_RemoteCfg* remoteCfg;

    /* Get the mailbox driver */
    driver = (Mailbox_Driver*)handle;

    if(handle == NULL)
    {
        /* Error: Invalid Arguments */
        DebugP_log0("MAILBOX: Mailbox_close Error! Null handle");
        retVal = MAILBOX_EINVAL;
    }
    if ((gMailboxMCB.initParam.osalPrms.disableAllIntr == NULL) ||
        (gMailboxMCB.initParam.osalPrms.restoreAllIntr == NULL))
    {
        retVal = MAILBOX_EINVAL;
    }

    if (retVal == MAILBOX_SOK)
    {
        remoteCfg = driver->remoteCfgPtr;
        /* Critical Section Protection:
         * close needs to be protected against multiple threads */
        key = gMailboxMCB.initParam.osalPrms.disableAllIntr();
        /* Reset the array for this instance of the driver */
        remoteCfg->handleArray[driver->cfg.chId] = NULL;

        remoteCfg->instCount--;
        gMailboxMCB.totalInstCnt--;

        /*Unregister ISRs if there are no more instances */
        Mailbox_unregisterInterrupts(handle);

        /* Release the critical section: */
        gMailboxMCB.initParam.osalPrms.restoreAllIntr(key);

        /* Was the driver operating in Write Blocking mode? */
        if(driver->writeSem)
        {
            /* YES: Delete the write semaphore */
            gMailboxMCB.initParam.osalPrms.deleteMutex(driver->writeSem);
        }

        /* Was the driver operating in Read Blocking mode? */
        if(driver->readSem)
        {
            /* YES: Delete the read semaphore */\
            gMailboxMCB.initParam.osalPrms.deleteMutex(driver->readSem);
        }

        /* Cleanup the memory: */
        Mailbox_freeDriver(handle);
    }

    return retVal;
}

/*
 *  ======== Mailbox_getStats ========
 */
int32_t Mailbox_getStats(Mbox_Handle handle, Mailbox_Stats * stats)
{
    int32_t             retVal = 0;
    Mailbox_Driver*     driver;

    /* Get the mailbox driver */
    driver = (Mailbox_Driver*)handle;

    if(handle == NULL)
    {
        /* Error: Invalid Arguments */
        DebugP_log0("MAILBOX: Mailbox_getStats Error! Null handle");
        retVal = MAILBOX_EINVAL;
    }
    else if(stats == NULL)
    {
        /* Error: Invalid Arguments */
        DebugP_log0("MAILBOX: Mailbox_getStats Error! Null pointer");
        retVal = MAILBOX_EINVAL;
    }
    else
    {
        stats->txBoxStatus         = (uint32_t) driver->txBoxStatus;
        stats->txCount             = driver->txCount;
        stats->rxCount             = driver->rxCount;
        stats->newMessageFlag      = (uint32_t) driver->newMessageFlag;
        stats->numBytesRead        = driver->numBytesRead;
        stats->boxFullIsrCount     = driver->boxFullIsrCount;
        stats->boxEmptyIsrCount    = driver->boxEmptyIsrCount;
        stats->readFlushCount      = driver->readFlushCount;
        stats->boxEmptyIsrErrCount = gMailboxMCB.errCnt.mailboxEmpty;
        stats->boxFullIsrErrCount  = gMailboxMCB.errCnt.mailboxFull;
    }

    return retVal;
}
