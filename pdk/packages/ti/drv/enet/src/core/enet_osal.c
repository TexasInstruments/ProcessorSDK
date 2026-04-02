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
 * \file  enet_osal.c
 *
 * \brief This file contains the OSAL wrapper implementation.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x002

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ti/osal/osal.h>
#include <ti/drv/enet/include/core/enet_types.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/include/core/enet_osal.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ENET_CFG_USE_OPTIMIZED_IRQ_CRITICAL_SECTION  (1U)

#define __NOP  asm (" NOP ")

#define NOP5   do { __NOP; __NOP; __NOP; __NOP; __NOP; } while (0)

#define NOP10  NOP5; \
               NOP5

#define NOP50  NOP10; \
               NOP10; \
               NOP10; \
               NOP10; \
               NOP10

/* Mutex pool size */
#define ENET_OSAL_DFLT_MUTEX_POOL_SIZE            (20U)

/* Second to nanosecs conversion factor */
#define ENET_OSAL_DFLT_SEC2NANOSEC                (1000000000ULL)

/* Max value reported by CycleprofilerP_getTimeStamp() */
#define ENET_OSAL_DFLT_TIMESTAMP_MAX              (0xFFFFFFFFULL)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Mutex pool */
static EnetOsalDflt_Mutex gEnetOsalDflt_mutexPool[ENET_OSAL_DFLT_MUTEX_POOL_SIZE];

/* CPU frequency*/
static uint32_t gEnetOsalDflt_selfFreqHz;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#if (((__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R')) && (ENET_CFG_USE_OPTIMIZED_IRQ_CRITICAL_SECTION == 1))
/*
 *  ======== Hwi_disable ========
 */
static inline uintptr_t HwiR5_disable()
{
    uintptr_t key;
    /*
     * Note: Inline assembly follows GNU format and may not
     *       be compatible with TI compiler.
     */
    __asm__ __volatile__ (
            "mrs %0, apsr\n\t"
            "cpsid i"
            : "=r" (key)
            :: "cc", "memory"
            );
    return (key);
}

/*
 *  ======== Hwi_enable ========
 */
static inline uintptr_t HwiR5_enable()
{
    uintptr_t key;
    /*
     * Note: Inline assembly follows GNU format and may not
     *       be compatible with TI compiler.
     */
    __asm__ __volatile__ (
            "mrs %0, apsr\n\t"
            "cpsie i"
            : "=r" (key)
            :: "cc", "memory"
            );
    return (key);
}

/*
 *  ======== Hwi_restore ========
 */
static inline void HwiR5_restore(uintptr_t key)
{
    __asm__ __volatile__ (
            "msr cpsr_fc, %0"
            :: "r" (key)
            : "cc", "memory"
            );
}


uintptr_t EnetOsal_disableAllIntr(void)
{
    return HwiR5_disable();
}

void EnetOsal_restoreAllIntr(uintptr_t cookie)
{
    HwiR5_restore(cookie);
}

#else
uintptr_t EnetOsal_disableAllIntr(void)
{
    return HwiP_disable();
}

void EnetOsal_restoreAllIntr(uintptr_t cookie)
{
    HwiP_restore(cookie);
}
#endif

void *EnetOsal_registerIntr(EnetOsal_Isr isrFxn,
                            uint32_t coreIntrNum,
                            uint32_t intrPriority,
                            uint32_t intrTrigType,
                            void *arg)
{
    OsalRegisterIntrParams_t intrPrms;
    OsalInterruptRetCode_e status;
    HwiP_Handle hHwi = NULL;

    Osal_RegisterInterrupt_initParams(&intrPrms);

    /* Populate the interrupt parameters */
    intrPrms.corepacConfig.arg             = (uintptr_t)arg;
    intrPrms.corepacConfig.isrRoutine      = isrFxn;
    intrPrms.corepacConfig.priority        = intrPriority;
    intrPrms.corepacConfig.triggerSensitivity = intrTrigType;
    intrPrms.corepacConfig.corepacEventNum = 0U;
    intrPrms.corepacConfig.intVecNum       = coreIntrNum;

    /* Register interrupts */
    status = Osal_RegisterInterrupt(&intrPrms, &hHwi);
    if (status != OSAL_INT_SUCCESS)
    {
        hHwi = NULL;
    }

    return hHwi;
}

void EnetOsal_unregisterIntr(void *hHwi)
{
    uint32_t corepacEventNum = 0U;

    /* Delete interrupts */
    Osal_DeleteInterrupt((HwiP_Handle)hHwi, corepacEventNum);
}

void EnetOsal_enableIntr(uint32_t coreIntrNum)
{
    uint32_t corepacEventNum = 0U;

    Osal_EnableInterrupt(corepacEventNum, coreIntrNum);
}

void EnetOsal_disableIntr(uint32_t coreIntrNum)
{
    uint32_t corepacEventNum = 0U;

    Osal_DisableInterrupt(corepacEventNum, coreIntrNum);
}

EnetOsalDflt_Mutex *EnetOsal_createMutex(void)
{
    EnetOsalDflt_Mutex *mutex = NULL;
    uintptr_t key;
    uint32_t i;

    key = HwiP_disable();

    for (i = 0U; i < ENET_OSAL_DFLT_MUTEX_POOL_SIZE; i++)
    {
        if (!gEnetOsalDflt_mutexPool[i].used)
        {
            gEnetOsalDflt_mutexPool[i].used = BTRUE;
            break;
        }
    }

    HwiP_restore(key);

    if (i < ENET_OSAL_DFLT_MUTEX_POOL_SIZE)
    {
        mutex = &gEnetOsalDflt_mutexPool[i];

        mutex->handle = MutexP_create(&mutex->mutexObj);
        if (mutex->handle == NULL)
        {
            key = HwiP_disable();
            mutex->used = BFALSE;
            HwiP_restore(key);
            mutex = NULL;
        }
    }

    return mutex;
}

void EnetOsal_deleteMutex(EnetOsalDflt_Mutex *hMutex)
{
    uintptr_t key;

    DebugP_assert(hMutex != NULL);

    if (hMutex->used)
    {
        MutexP_delete(hMutex->handle);

        key = HwiP_disable();
        hMutex->used = BFALSE;
        HwiP_restore(key);
    }
}

void EnetOsal_lockMutex(EnetOsalDflt_Mutex *hMutex)
{
    DebugP_assert(hMutex != NULL);

    MutexP_lock(hMutex->handle, MutexP_WAIT_FOREVER);
}

void EnetOsal_unlockMutex(EnetOsalDflt_Mutex *hMutex)
{
    DebugP_assert(hMutex != NULL);

    MutexP_unlock(hMutex->handle);
}

bool EnetOsal_isCacheCoherent(void)
{
    bool isCoherent = BFALSE;

#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#if defined(BUILD_MPU1_0)
    isCoherent = BTRUE;
#endif
#endif

    return isCoherent;
}

void EnetOsal_cacheInv(void *addr,
                       int32_t size)
{
    bool isCacheCoherent = EnetOsal_isCacheCoherent(); //check

    if (isCacheCoherent != BTRUE)
    {
        /* PDK OSAL function is called explicitly for compiler optimizations
         * which cannot happen with function pointer */
        CacheP_Inv(addr, size);
    }
}

void EnetOsal_cacheWb(void *addr,
                      int32_t size)
{
    bool isCacheCoherent = EnetOsal_isCacheCoherent(); //check

    if (isCacheCoherent != BTRUE)
    {
        CacheP_wb(addr, size);
    }
}

static uint32_t EnetOsalDflt_convertNanosec2Ticks(uint32_t delayInNsecs)
{
    return (((uint64_t)delayInNsecs*gEnetOsalDflt_selfFreqHz) / ENET_OSAL_DFLT_SEC2NANOSEC);
}

void EnetOsal_cacheWbInv(void *addr,
                         int32_t size)
{
    bool isCacheCoherent = EnetOsal_isCacheCoherent(); //check

    if (isCacheCoherent != BTRUE)
    {
        CacheP_wbInv(addr, size);
    }
}

uint32_t EnetOsal_timerRead(void)
{
    return 0U;
}

uint32_t EnetOsal_timerGetDiff(uint32_t startTime)
{
    uint32_t currTime;

    currTime = EnetOsal_timerRead();

    return(currTime - startTime);
}

void EnetOsal_delay(uint32_t delayInNsecs)
{
   uint64_t delayTicks = EnetOsalDflt_convertNanosec2Ticks(delayInNsecs);
   uint64_t currentTick = (uint64_t)CycleprofilerP_getTimeStamp();
   const uint64_t endTick = currentTick + (uint64_t)delayTicks;

   while (currentTick < endTick)
   {
       NOP50;
       currentTick = (uint64_t)CycleprofilerP_getTimeStamp();
       if ((currentTick < endTick) &&
           ((endTick - currentTick) > delayTicks))
       {
           currentTick += ENET_OSAL_DFLT_TIMESTAMP_MAX + 1ULL;
       }
   }
}
