/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== TimerP_restrictedDMTimers.c ========
 */

#include <stdint.h>
#include <ti/osal/soc/osal_soc.h>

/* Resrved DM Timers for MCU, Main, C7X, C66 cores.
 * Application is best aware of the timers being used as tick timers or
 * being used for other purposes, and hence application should be able to
 * override this definition.
 * Following array denotes {RestrictedByCore, RestrictedTimerID}
 */

#if defined (BUILD_MCU)
__attribute__((weak)) Osal_timerReserved Osal_mcuRestrictedTimerID[TimerP_ANY_MAX_RESTRICTIONS] =
{
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID}
};

__attribute__((weak)) Osal_timerReserved Osal_mainMcuRestrictedTimerID[TimerP_ANY_MAX_RESTRICTIONS] =
{
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID}
};
#endif

#if defined (BUILD_C7X)
__attribute__((weak)) Osal_timerReserved Osal_c7xRestrictedTimerID[TimerP_ANY_MAX_RESTRICTIONS] =
{
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID}
};
#endif

#if defined (BUILD_C66X)
__attribute__((weak)) Osal_timerReserved Osal_c66RestrictedTimerID[TimerP_ANY_MAX_RESTRICTIONS] =
{
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID}
};
#endif

#if defined (BUILD_MPU)
__attribute__((weak)) Osal_timerReserved Osal_mpuRestrictedTimerID[TimerP_ANY_MAX_RESTRICTIONS] =
{
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID},
    {OSAL_INVALID_CORE_ID, TimerP_ID_INVALID}
};
#endif

