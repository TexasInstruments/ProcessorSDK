/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
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

/**
 *  \file   osal_extended_test.h
 *
 *  \brief  Osal extended test header file.
 *
 */

#ifndef _OSAL_EXTENDED_TEST_H_
#define _OSAL_EXTENDED_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <ti/csl/soc.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/osal.h>
#include <ti/osal/SwiP.h>
#include <ti/osal/soc/osal_soc.h>
#include <ti/osal/src/nonos/Nonos_config.h>
#include "OSAL_log.h"
#include <ti/board/board.h>

#if defined(SAFERTOS)
#include <SafeRTOS_API.h>
#endif

#if defined(_TMS320C6X) || defined(BUILD_C66X)
#include <ti/osal/EventCombinerP.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* Top level function for Heap tests */
int32_t OsalApp_heapFreertosTest(void);

/* Top level function for Hwi tests */
int32_t OsalApp_hwiTests(void);

/* Top level function for Mutex tests */
int32_t OsalApp_mutexTests(void);

/* Top level function for Queue tests */
int32_t OsalApp_queueTests(void);

/* Top level function for Cache tests */
int32_t OsalApp_cacheTests(void);

/* Top level function for Mailbox tests */
int32_t OsalApp_mailboxTests(void);

/* Top level function for Task tests */
int32_t OsalApp_taskTests(void);

/* Top level function for Memory tests */
int32_t OsalApp_memoryTests(void);

/* Top level function for Event tests */
int32_t OsalApp_eventTests(void);

/* Top level function for Semaphore tests */
int32_t OsalApp_semaphoreTests(void);

/* Top level function for utils tests */
int32_t OsalApp_utilsTests(void);

/* Top level function for Cycleprofiler test */
int32_t OsalApp_cycleProfilerTest(void);

/* Top level function for load tests */
int32_t OsalApp_freertosLoadTests(void);

/* Top level function for c7x and c66x Arch utils tests */
int32_t OsalApp_ArchutilsTests(void);

/* Top level function for Register interrupt tests */
int32_t OsalApp_registerIntrTests(void);

/* Top level function for clock tests */
int32_t OsalApp_clockTests(void);

/* Tests for CSL Arch for C7X */
int32_t OsalApp_c7xArchTests(void);

/* Top level function for freertos portable tests */
int32_t OsalApp_freertosPortableTests(void);

/* Top level function for timer tests */
int32_t OsalApp_timerTests(void);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif
#endif /* _OSAL_EXTENDED_TEST_H_ */
