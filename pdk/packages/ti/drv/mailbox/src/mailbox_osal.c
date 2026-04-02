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
 *  \file mailbox_osal.c
 *
 *  \brief The file implements the osal for the Mailbox Driver.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/* Standard Include Files. */
#include <string.h>

#include "mailbox.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* We need only mutex in Mailbox driver. Use semaphore in binary mode */
static void *Mbox_osalMutexCreate(void)
{
    SemaphoreP_Params   semPrms;
    SemaphoreP_Handle   mutexHandle;

    SemaphoreP_Params_init(&semPrms);
    semPrms.mode = SemaphoreP_Mode_BINARY;
    mutexHandle = (void *) SemaphoreP_create(0U, &semPrms);

    return ((void*) mutexHandle);
}

static void Mbox_osalMutexDelete(void *mutexHandle)
{
    SemaphoreP_delete((SemaphoreP_Handle) mutexHandle);
}

static int32_t Mbox_osalMutexLock(void *mutexHandle, uint32_t timeout)
{
    int32_t retVal = MAILBOX_SOK;
    SemaphoreP_Status  semStatus;
    uint32_t semTimeout = timeout;

    if (timeout == MAILBOX_WAIT_FOREVER)
    {
        semTimeout = SemaphoreP_WAIT_FOREVER;
    }

    semStatus = SemaphoreP_pend((SemaphoreP_Handle) mutexHandle, semTimeout);

    if (SemaphoreP_OK != semStatus)
    {
        retVal = MAILBOX_ETIMEOUT;
    }

    return retVal;
}

static void Mbox_osalMutexUnlock(void *mutexHandle, bool isISRContext)
{
    if (isISRContext == true)
    {
        SemaphoreP_postFromISR((SemaphoreP_Handle) mutexHandle);
    }
    else
    {
        SemaphoreP_post((SemaphoreP_Handle) mutexHandle);
    }
}

static void *Mbox_osalRegisterIntr(Mbox_OsalIsrFxn isrFxn,
                                   uint32_t coreIntrNum,
                                   uint32_t intrPriority,
                                   void *arg,
                                   char *name)
{
    OsalRegisterIntrParams_t    intrPrms;
    OsalInterruptRetCode_e      osalRetVal;
    HwiP_Handle                 hwiHandle = NULL;

    Osal_RegisterInterrupt_initParams(&intrPrms);

    /* Populate the interrupt parameters */
    intrPrms.corepacConfig.arg              = (uintptr_t) arg;
    intrPrms.corepacConfig.isrRoutine       = isrFxn;
    if (intrPriority != MAILBOX_OSAL_DEFAULT_PRIORITY)
    {
        intrPrms.corepacConfig.priority = intrPriority;
    }
    if (name != NULL)
    {
        intrPrms.corepacConfig.name = name;
    }

#if defined (_TMS320C6X)
    /* On C66x, we use Event Combiner to map the interrupt to the CPU Intc.  To
     * do this, OSAL expects that event number holds the interrupt number and we
     * use the macro for interrupt number to specify we wish to use Event
     * Combiner.
     */
    intrPrms.corepacConfig.corepacEventNum  = (int32_t)coreIntrNum;
    intrPrms.corepacConfig.intVecNum        = OSAL_REGINT_INTVEC_EVENT_COMBINER;
#else
    /* Other (non-C66x) CPUs don't use event number and interrupt number is
     * passed in and programmed to CPU Intc directly.
     */
    intrPrms.corepacConfig.corepacEventNum  = (int32_t)0;
    intrPrms.corepacConfig.intVecNum        = (int32_t)coreIntrNum;
#endif

    /* Register interrupts */
    osalRetVal = Osal_RegisterInterrupt(&intrPrms, &hwiHandle);
    if(OSAL_INT_SUCCESS != osalRetVal)
    {
        hwiHandle = NULL;
    }

    return (hwiHandle);
}

static void *Mbox_osalRegisterDirectIntr(Mbox_OsalDirectIsrFxn isrFxn,
                                         uint32_t coreIntrNum,
                                         uint32_t intrPriority)
{
#if defined(SOC_AM64X) && defined(BUILD_MCU)
    OsalRegisterIntrParams_t    intrPrms;
    OsalInterruptRetCode_e      osalRetVal;
    HwiP_Handle                 hwiHandle = NULL;

    Osal_RegisterInterrupt_initParams(&intrPrms);

    /* Populate the interrupt parameters */
    intrPrms.corepacConfig.isrRoutine       = NULL;
    if (intrPriority != MAILBOX_OSAL_DEFAULT_PRIORITY)
    {
        intrPrms.corepacConfig.priority = intrPriority;
    }

    intrPrms.corepacConfig.corepacEventNum = (int32_t)0;
    intrPrms.corepacConfig.intVecNum       = (int32_t)coreIntrNum;

    /* Register interrupt */
    osalRetVal = Osal_RegisterInterruptDirect(&intrPrms, isrFxn, &hwiHandle);
    if (OSAL_INT_SUCCESS != osalRetVal)
    {
        hwiHandle = NULL;
    }

    return (hwiHandle);
#else
    return NULL;
#endif
}

static void Mbox_osalUnRegisterIntr(void *hwiHandle, uint32_t coreIntrNum)
{
    int32_t     corepacEventNum = (int32_t)0;

#if defined (_TMS320C6x)
    corepacEventNum = (int32_t)coreIntrNum;
#endif

    /* Delete interrupts */
    (void) Osal_DeleteInterrupt((HwiP_Handle) hwiHandle, corepacEventNum);

    return;
}

static void Mbox_osalEnableIntr(uint32_t coreIntrNum)
{
    int32_t     corepacEventNum = (int32_t)0;

#if defined (_TMS320C6X)
    corepacEventNum = (int32_t)coreIntrNum;
    coreIntrNum = OSAL_REGINT_INTVEC_EVENT_COMBINER;
#endif
    Osal_EnableInterrupt(corepacEventNum, (int32_t)coreIntrNum);

    return;
}

static void Mbox_osalDisableIntr(uint32_t coreIntrNum)
{
    int32_t     corepacEventNum = (int32_t)0;

#if defined (_TMS3206X)
    corepacEventNum = (int32_t)coreIntrNum;
    coreIntrNum = OSAL_REGINT_INTVEC_EVENT_COMBINER;
#endif
    Osal_DisableInterrupt(corepacEventNum, (int32_t)coreIntrNum);

    return;
}

void MboxOsalPrms_init(Mbox_OsalPrms *osalPrms)
{
    if (NULL != osalPrms)
    {
        osalPrms->disableAllIntr = &HwiP_disable;
        osalPrms->restoreAllIntr = &HwiP_restore;
        osalPrms->enableIntr     = &Mbox_osalEnableIntr;
        osalPrms->disableIntr    = &Mbox_osalDisableIntr;

        osalPrms->createMutex   = &Mbox_osalMutexCreate;
        osalPrms->deleteMutex   = &Mbox_osalMutexDelete;
        osalPrms->lockMutex     = &Mbox_osalMutexLock;
        osalPrms->unlockMutex   = &Mbox_osalMutexUnlock;

        osalPrms->registerIntr   = &Mbox_osalRegisterIntr;
        osalPrms->registerDirectIntr = &Mbox_osalRegisterDirectIntr;
        osalPrms->unRegisterIntr = &Mbox_osalUnRegisterIntr;
    }
    return;
}
