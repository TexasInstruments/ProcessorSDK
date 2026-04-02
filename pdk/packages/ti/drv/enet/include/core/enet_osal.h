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

/*!
 * \file  enet_osal.h
 *
 * \brief This file contains the OSAL API of the Enet driver.
 */

#ifndef ENET_OSAL_H_
#define ENET_OSAL_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdbool.h>
#include <ti/osal/MutexP.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

typedef struct EnetOsalDflt_Mutex_s
{
    bool used;
    MutexP_Object mutexObj;
    MutexP_Handle handle;
} EnetOsalDflt_Mutex;

/*!
 * \brief Enet OSAL ISR callback function prototype.
 *
 * \param arg     App data
 */
typedef void (*EnetOsal_Isr)(uintptr_t arg);

/* ========================================================================== */
/*                         Global Variables Declarations                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*!
 * \brief Enet OSAL intr disable function prototype.
 *
 * \return Cookie to be passed back to enable interrupt function
 */
uintptr_t EnetOsal_disableAllIntr(void);

/*!
 * \brief Enet OSAL intr restore function prototype.
 *
 * \param cookie  This is returned in disable interrupt function
 */
void EnetOsal_restoreAllIntr(uintptr_t cookie);

/*!
 * \brief Register an ISR for an interrupt.
 *
 * \param isrFxn        Interrupt service routine
 * \param coreIntrNum   Interrupt number
 * \param intrPriority  Interrupt priority
 * \param intrTrigType  Interrupt trigger type
 * \param arg           Argument to ISR function
 *
 * \return Interrupt handle pointer
 */
void *EnetOsal_registerIntr(EnetOsal_Isr isrFxn,
                            uint32_t coreIntrNum,
                            uint32_t intrPriority,
                            uint32_t intrTrigType,
                            void *arg);

/*!
 * \brief Unregister an interrupt.
 *
 * \param hHwi    Interrupt handle pointer
 */
void EnetOsal_unregisterIntr(void *hHwi);

/*!
 * \brief Enable interrupt.
 *
 * \param coreIntrNum   Interrupt number
 */
void EnetOsal_enableIntr(uint32_t coreIntrNum);

/*!
 * \brief Disable interrupt.
 *
 * \param coreIntrNum   Interrupt number
 */
void EnetOsal_disableIntr(uint32_t coreIntrNum);

/*!
 * \brief Create a mutex.
 *
 * \return Mutex handle
 */
EnetOsalDflt_Mutex *EnetOsal_createMutex(void);

/*!
 * \brief Delete a mutex.
 *
 * \param hMutex   Mutex handle
 */
void EnetOsal_deleteMutex(EnetOsalDflt_Mutex *hMutex);

/*!
 * \brief Lock a mutex.
 *
 * \param hMutex   Mutex handle
 */
void EnetOsal_lockMutex(EnetOsalDflt_Mutex *hMutex);

/*!
 * \brief Unlock a mutex.
 *
 * \param hMutex   Mutex handle
 */
void EnetOsal_unlockMutex(EnetOsalDflt_Mutex *hMutex);

/*!
 * \brief Invalidate cache.
 *
 * Invalidates cache in a range of memory.
 *
 * \param addr    Start address of the cache line(s)
 * \param size    Size (in bytes) of the memory to invalidate
 */
void EnetOsal_cacheInv(void *addr,
                       int32_t size);

/*!
 * \brief Write-back cache.
 *
 * Writes back cache a range of memory from cache.
 *
 * \param addr    Start address of the cache line(s)
 * \param size    Size (in bytes) of the memory to be written back
 */
void EnetOsal_cacheWb(void *addr,
                      int32_t size);

/*!
 * \brief Write-back and invalidate cache.
 *
 * Writes back and invalidates a range of memory.
 *
 * \param addr   Start address of the cache line/s
 * \param size   Size (in bytes) of the memory to be written back
 */
void EnetOsal_cacheWbInv(void *addr,
                         int32_t size);

/*!
 * \brief Get the time difference with respect to timestamp.
 *
 * Gets the difference between time value passed to this function and current
 * time from timer.
 *
 * \return Time difference
 */
uint32_t EnetOsal_timerGetDiff(uint32_t startTime);

/*!
 * \brief Read current timer value.
 *
 * \return Timer's read value
 */
uint32_t EnetOsal_timerRead(void);

/*!
 * \brief Delay function
 *
 * \param delayInNsecs    Delay value in nanosecond
 */
void EnetOsal_delay(uint32_t delayInNsecs);

/* ========================================================================== */
/*                        Deprecated Function Declarations                    */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* ENET_OSAL_H_ */
