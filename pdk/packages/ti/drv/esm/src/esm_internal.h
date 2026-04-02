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

#ifndef ESM_INTERNAL_H
#define ESM_INTERNAL_H

#include <ti/osal/HwiP.h>
#include <ti/drv/esm/esm.h>
#include <ti/drv/esm/soc/esm_soc.h>

/* Flag to enable/disable ESM debug functionality */
#define ESM_DEBUG                   1

/* Maximum number of notify callbacks supported */
#define ESM_MAX_NOTIFIERS           4

/* Maximum number of ESM groups with 32 errors for each group */
#define ESM_MAX_GROUPS              5

#ifdef ESM_DEBUG
#define ESM_MAX_ISR_COUNT           4
#endif

#define     MINUS_ONE           -((int32_t)1)

/**
@defgroup ESM_DRIVER_INTERNAL_DATA_STRUCTURE       ESM Driver Internal Data Structures
@ingroup ESM_DRIVER
@brief
*   The section has a list of all the data structures which are used internally by the driver
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup ESM_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */


/**
 * @brief
 *  ESM Driver Master Control Block
 *
 * @details
 *  The structure is used to hold all the pertinent information with respect
 *  to the ESM Driver.
 */
typedef struct ESM_DriverMCB_t
{
    /**
     * @brief   Base address of the ESM address space to be used.
     */
    uint32_t                    esmBaseAddr;

    /**
     * @brief   Registered high priority Interrupt Handler.
     */
    HwiP_Handle                 hwiHandleHi;

    /**
     * @brief   Registered low priority Interrupt Handler.
     */
    HwiP_Handle                 hwiHandleLo;

    /**
     * @brief   Registered notify function. Currently only upto 4 notify functions are supported.
     */
    ESM_NotifyParams            notifyParams[ESM_MAX_NOTIFIERS];

    /**
     * @brief   The number of Group1 errors supported.
     */
    uint32_t                    numGroup1Err;

#ifdef ESM_DEBUG
    /**
     * @brief   DEBUG: to keep track of various ESM interrupts received by the system.
     */
    uint32_t                    debugEsmISRCount[ESM_MAX_ISR_COUNT];
#endif
} ESM_DriverMCB;

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* ESM_INTERNAL_H */
