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
 *  \ingroup DRV_MAILBOX_MODULE
 *  \defgroup DRV_MAILBOX_OSAL_MODULE MAILBOX Driver OSAL API
 *            This is MAILBOX driver OSAL related configuration parameters and
 *            API
 *
 *  @{
 */

/**
 *  \file mailbox_osal.h
 *
 *  \brief Mailbox Osal definitions
 *
 */

#ifndef MAILBOX_OSAL_H_
#define MAILBOX_OSAL_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include "ti/osal/osal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \brief Mailbox OSAL interrupt disable function prototype.
 *
 *  \return Cookie to be passed back to enable interrupt function
 */
typedef uintptr_t (*Mbox_OsalDisableAllIntrFxn)(void);

/**
 *  \brief Mailbox OSAL interrupt restore function prototype.
 *
 *  \param cookie       [IN] This is returned in disable interrupt function
 */
typedef void (*Mbox_OsalRestoreAllIntrFxn)(uintptr_t cookie);

/**
 *  \brief Mailbox OSAL intr disable function prototype.
 *
 *  \param coreIntrNum  [IN] Interrupt to disable
 */
typedef void (*Mbox_OsalEnableIntrFxn)(uint32_t coreIntrNum);

/**
 *  \brief Mailbox OSAL intr disable function prototype.
 *
 *  \param coreIntrNum  [IN] Interrupt to disable
 */
typedef void (*Mbox_OsalDisableIntrFxn)(uint32_t coreIntrNum);

/**
 *  \brief Mailbox OSAL mutex create function prototype to protect critical section.
 *
 *  \return Pointer to mutex object
 */
typedef void * (*Mbox_OsalMutexCreateFxn)(void);

/**
 *  \brief Mailbox OSAL mutex delete function prototype.
 *
 *  \param mutexHandle  [IN] Pointer to mutex object returned during create
 */
typedef void (*Mbox_OsalMutexDeleteFxn)(void *mutexHandle);

/**
 *  \brief Mailbox OSAL mutex lock function prototype.
 *
 *  \param mutexHandle  [IN] Pointer to mutex object returned during create
 */
typedef int32_t (*Mbox_OsalMutexLockFxn)(void *mutexHandle, uint32_t timeout);

/**
 *  \brief Mailbox OSAL mutex lock function prototype.
 *
 *  \param mutexHandle  [IN] Pointer to mutex object returned during create
 */
typedef void (*Mbox_OsalMutexUnlockFxn)(void *mutexHandle, bool isISRContext);

/**
 *  \brief Mailbox OSAL ISR callback function prototype.
 *
 *  \param arg          [IN] App data
 */
typedef void (*Mbox_OsalIsrFxn)(uintptr_t arg);

/**
 *  \brief Mailbox OSAL Direct ISR callback function prototype.
 */
typedef void (*Mbox_OsalDirectIsrFxn)(void);

/**
 *  \brief Mailbox OSAL ISR register function prototype.
 *
 *  \param isrFxn       [IN] ISR callback fxn pointer
 *  \param coreIntrNum  [IN] Core interrupt number to register
 *  \param intrPriority [IN] Priority
 *  \param arg          [IN] Arg that will be passed back in the ISR
 *  \param name         [IN] Name for the ISR
 *
 *  \return Created HWI handle
 */
typedef void *(*Mbox_OsalRegisterIntrFxn)(Mbox_OsalIsrFxn isrFxn,
                                          uint32_t coreIntrNum,
                                          uint32_t intrPriority,
                                          void *arg,
                                          char *name);

/**
 *  \brief Mailbox OASL Direct ISR register function prototype for R5F
 *         direct VIM registration.
 *
 *  \param isrFxn       [IN] ISR callback fxn pointer
 *  \param coreIntrNum  [IN] Core interrupt number to register
 *  \param intrPriority [IN] Priority
 *
 *  \return Created HWI handle
 */
typedef void *(*Mbox_OsalRegisterDirectIntrFxn)(Mbox_OsalDirectIsrFxn isrFxn,
                                                uint32_t coreIntrNum,
                                                uint32_t intrPriority);

/**
 *  \brief Mailbox OSAL ISR unregister function prototype.
 *
 *  \param hwiHandle    [IN] HWI handle
 */
typedef void (*Mbox_OsalUnRegisterIntrFxn)(void *hwiHandle,
                                           uint32_t coreIntrNum);


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief Mailbox driver OSAL function pointers.
 */
typedef struct Mbox_OsalPrms_s
{
    Mbox_OsalDisableAllIntrFxn   disableAllIntr;
    /**< OSAL all interrupt disable function pointer */
    Mbox_OsalRestoreAllIntrFxn   restoreAllIntr;
    /**< OSAL all interrupt restore function pointer */
    Mbox_OsalEnableIntrFxn     enableIntr;
    /**< OSAL interrupt disable function pointer */
    Mbox_OsalDisableIntrFxn     disableIntr;
    /**< OSAL interrupt disable function pointer */

    Mbox_OsalMutexCreateFxn      createMutex;
    /**< Create mutex function pointer */
    Mbox_OsalMutexDeleteFxn      deleteMutex;
    /**< Delete mutex function pointer */
    Mbox_OsalMutexLockFxn        lockMutex;
    /**< Lock mutex function pointer */
    Mbox_OsalMutexUnlockFxn      unlockMutex;
    /**< Unlock mutex function pointer */

    Mbox_OsalRegisterIntrFxn     registerIntr;
    /**< Register interrupt function pointer */
    Mbox_OsalRegisterDirectIntrFxn registerDirectIntr;
    /**< Register direct interrupt function pointer - used only for R5F for certain SoCs */
    Mbox_OsalUnRegisterIntrFxn   unRegisterIntr;
    /**< Unregister interrupt function pointer */
} Mbox_OsalPrms;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*
 * Structure Init functions
 */
/**
 *  \brief Mbox_OsalPrms structure init function.
 *
 *  \param osalPrms     [IN] Pointer to #Mbox_OsalPrms structure.
 *
 */
void MboxOsalPrms_init (Mbox_OsalPrms *osalPrms);

/* ========================================================================== */
/*                       Static Function Definition                           */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* MAILBOX_OSAL_H_ */

/* @} */
