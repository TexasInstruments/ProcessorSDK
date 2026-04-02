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

/** ============================================================================
 *  @file       watchdog_v0.h
 *
 *  @brief      Watchdog driver SoC specific interface.
 *
 *  ============================================================================
 */

#ifndef WATCHDOG_V0_H
#define WATCHDOG_V0_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <ti/drv/watchdog/watchdog.h>
#include <ti/csl/csl_rti.h>

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/

/**
 * @brief   This is the Watchdog Driver registered function table
 */
extern Watchdog_FxnTable gWatchdogFxnTable;

/**
 * @brief
 *  Watchdog Driver HW configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to each driver instance
 */
typedef struct Watchdog_v0_HwAttrs_t
{
    /**
     * @brief   Base address of the Watchdog address space to be used.
     */
    uintptr_t           baseAddr; 

    /**
     * @brief   RTI clock frequency.
     */
    uint32_t            rtiClkFrequency;

    /**
     * @brief   Group Number to which the ESM error belongs
     */
    uint32_t            groupNum;

    /**
     * @brief   ESM error Number
     */
    uint32_t            errorNum;

    /**
     * @brief   ESM handle
     */
    void               *esmHandle;
} Watchdog_v0_HwAttrs;

#ifdef __cplusplus
}
#endif

#endif /* _WATCHDOG_V0_H */
