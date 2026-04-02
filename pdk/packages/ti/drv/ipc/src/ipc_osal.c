/*
 *  Copyright (c) Texas Instruments Incorporated 2020
 *  All rights reserved.
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
 *  \file ipc_osal.c
 *
 *  \brief Implementation of ipc osal
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "ipc_osal.h"

#include <ti/drv/ipc/ipc.h>
#include <ti/drv/ipc/src/ipc_mailbox.h>
#include <ti/drv/ipc/soc/ipc_soc.h>
#include <ti/drv/ipc/include/ipc_types.h>

#include "ipc_priv.h"

#if defined(BUILD_M4F)
static uint32_t Ipc_HwiDisableCount = 0;
static uintptr_t Ipc_HwiDisableKey = 0;

static uintptr_t Ipc_osalHwiDisable(void)
{
    if (Ipc_HwiDisableCount++ == 0)
    {
        Ipc_HwiDisableKey = HwiP_disable();
    }
    return Ipc_HwiDisableCount;
}

static void Ipc_osalHwiRestore(uintptr_t key)
{
    if (--Ipc_HwiDisableCount == 0)
    {
        HwiP_restore(Ipc_HwiDisableKey);
    }
}
#endif

/* GateSwi related */
static void *Ipc_osalHIsrGateCreate(void)
{
    /* Nothing required here because interrupt locking is sufficient. */
    return (void *) 1U;
}

static void Ipc_osalHIsrGateDelete(Ipc_OsalHIsrGateHandle *handle)
{
    /* Nothing required here because interrupt locking is sufficient. */
    return;
}

static int32_t Ipc_osalHIsrGateEnter(Ipc_OsalHIsrGateHandle handle)
{
#if defined(BUILD_M4F)
    return (int32_t)Ipc_osalHwiDisable();
#else
    return (int32_t)HwiP_disable();
#endif
}

static void Ipc_osalHIsrGateExit(Ipc_OsalHIsrGateHandle handle, int32_t key)
{
#if defined(BUILD_M4F)
    Ipc_osalHwiRestore((uintptr_t)key);
#else
    HwiP_restore((uintptr_t)key);
#endif
    return;
}

static int32_t Ipc_osalHIsrCreate(Ipc_OsalHIsrHandle *handle,
                                    Ipc_OsalHIsrFxn fxn, void *arg)
{
    int32_t rtnVal = IPC_EBADARGS;

    if ((NULL != handle) && ((NULL != fxn) && (NULL != arg)))
    {
        handle->arg0    = (uintptr_t)arg;
        handle->arg1    = (uintptr_t)NULL;
        handle->hIsrFxn = fxn;
        handle->hLosHisrHandle = NULL;

        rtnVal = IPC_SOK;
    }

    return (rtnVal);
}

static void Ipc_osalHIsrDelete(Ipc_OsalHIsrHandle *handle)
{
    if (NULL != handle)
    {
        handle->arg0    = (uintptr_t)NULL;
        handle->arg1    = (uintptr_t)NULL;
        handle->hIsrFxn = NULL;
        handle->hLosHisrHandle = NULL;
    }
}

static int32_t Ipc_osalHIsrPost(Ipc_OsalHIsrHandle *handle)
{
    int32_t rtnVal = IPC_EBADARGS;

    if (NULL != handle)
    {
        handle->hIsrFxn(handle->arg0, (uintptr_t)NULL);
        rtnVal = IPC_SOK;
    }
    return (rtnVal);
}

static void *Ipc_osalMutexCreate(void)
{
    SemaphoreP_Params   semPrms;
    SemaphoreP_Handle   mutexHandle;

    SemaphoreP_Params_init(&semPrms);
    mutexHandle = (void *) SemaphoreP_create(0U, &semPrms);

    return ((void*) mutexHandle);
}

static void Ipc_osalMutexDelete(void *mutexHandle)
{
    SemaphoreP_delete((SemaphoreP_Handle) mutexHandle);
}

static int32_t Ipc_osalMutexLock(void *mutexHandle, uint32_t timeout)
{
    int32_t retVal = IPC_SOK;
    SemaphoreP_Status  semStatus;

    semStatus = SemaphoreP_pend((SemaphoreP_Handle) mutexHandle, timeout);
    if (SemaphoreP_OK != semStatus)
    {
        retVal = IPC_ETIMEOUT;
    }

    return retVal;
}

static void Ipc_osalMutexUnlock(void *mutexHandle)
{
    SemaphoreP_post((SemaphoreP_Handle) mutexHandle);
}


void IpcOsalPrms_init (Ipc_OsalPrms *initPrms)
{
    if (NULL != initPrms)
    {
#if defined(BUILD_M4F)
        initPrms->disableAllIntr = &Ipc_osalHwiDisable;
        initPrms->restoreAllIntr = &Ipc_osalHwiRestore;
#else
        initPrms->disableAllIntr = &HwiP_disable;
        initPrms->restoreAllIntr = &HwiP_restore;
#endif

        initPrms->createHIsr    = &Ipc_osalHIsrCreate;
        initPrms->deleteHIsr    = &Ipc_osalHIsrDelete;
        initPrms->postHIsr      = &Ipc_osalHIsrPost;

        initPrms->createHIsrGate = &Ipc_osalHIsrGateCreate;
        initPrms->deleteHIsrGate = &Ipc_osalHIsrGateDelete;
        initPrms->lockHIsrGate   = &Ipc_osalHIsrGateEnter;
        initPrms->unLockHIsrGate = &Ipc_osalHIsrGateExit;

        initPrms->createMutex   = &Ipc_osalMutexCreate;
        initPrms->deleteMutex   = &Ipc_osalMutexDelete;
        initPrms->lockMutex     = &Ipc_osalMutexLock;
        initPrms->unlockMutex   = &Ipc_osalMutexUnlock;

        initPrms->registerIntr  = &Mailbox_plugInterrupt;
    }
    return;
}
