/*
 *  Copyright (c) Texas Instruments Incorporated 2022
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
 *  \file SafeRTOS_priv.h
 *
 *  \brief OSAL SafeRTOS private header file.
 */

#ifndef OSAL_SAFERTOS_PRIV_H_
#define OSAL_SAFERTOS_PRIV_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/soc/osal_soc.h>

#include <SafeRTOS_API.h>

#if defined (configINCLUDE_RUNTIMESTATS) && ( configINCLUDE_RUNTIMESTATS == 1 )
#include <runtimestatsAPI.h>
#include <runtimestats.h>
#endif

#include <portable.h>

#if defined (BUILD_C7X)
#include "Hwi.h"
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* Structure to dynamically allocate the value for timer ID,
 * Event ID and Interrupt Number for the cores during run time. */
typedef struct Safertos_OSTimerParams_s
{
    uint32_t timerId;        /* Timer ID */
#if defined (BUILD_C66X) || defined (BUILD_C7X)
    uint32_t eventId;        /* Event Id used by Timer */
    uint32_t intNum;         /* Interrupt Number used by Timer */
#endif   
}Safertos_OSTimerParams;

/* ========================================================================== */
/*                         Global Variables                                   */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

portBaseType prvSetupHardware( void );
void prvGetOSTimerParams( Safertos_OSTimerParams *params);
int32_t Osal_isInISRContext(void);
portBaseType _system_pre_init( void );
void _system_post_cinit( void );
portTaskHandleType xGetIdleTaskHandle( void );
TimerP_Handle TimerP_getTickTimerHandle( void );

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OSAL_SAFERTOS_PRIV_H_ */
