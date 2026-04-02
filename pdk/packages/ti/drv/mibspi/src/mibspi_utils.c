/*
 *  Copyright (c) Texas Instruments Incorporated 2018
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
 *  \file cpsw_utils.c
 *
 *  \brief File containing the CPSW driver utility functions.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* This is needed for vsnprintf */
#include <stdio.h>
#include <stdarg.h>

/* This is needed for memset/memcpy */
#include <string.h>

#include <ti/csl/soc.h>
#include <ti/osal/DebugP.h>
#include <ti/drv/mibspi/MIBSPI.h>
#include <ti/drv/mibspi/src/MIBSPI_osal.h>
#include "mibspi_trace_config.h"

#include "mibspi_priv.h"
#include "mibspi_trace_priv.h"
#include "mibspi_utils.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct MibspiUtils_Obj_s
{
    void *printLock;

    char printBuf[MIBSPI_CFG_PRINT_BUF_LEN];

    void *traceLock;

    char traceBuf[MIBSPI_CFG_TRACE_BUF_LEN];
} MibspiUtils_Obj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static SemaphoreP_Handle MibspiUtils_mutexCreate(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

MibspiUtils_Obj gMibspiUtilsObj;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* We need only mutex in CPSW driver. Use semaphore in binary mode */
static SemaphoreP_Handle MibspiUtils_mutexCreate(void)
{
    SemaphoreP_Params semPrms;
    SemaphoreP_Handle mutexHandle;

    SemaphoreP_Params_init(&semPrms);
    semPrms.mode = SemaphoreP_Mode_BINARY;
    mutexHandle  = (void *)SemaphoreP_create(1U, &semPrms);

    return(mutexHandle);
}



void MibspiUtils_init(void)
{
    memset(&gMibspiUtilsObj, 0U, sizeof(MibspiUtils_Obj));

    gMibspiUtilsObj.printLock = MibspiUtils_mutexCreate();
    gMibspiUtilsObj.traceLock = MibspiUtils_mutexCreate();

    return;
}

void MibspiUtils_deInit(void)
{
    MIBSPI_osalDeleteBlockingLock(gMibspiUtilsObj.printLock);

    MIBSPI_osalDeleteBlockingLock(gMibspiUtilsObj.traceLock);

    return;
}



void MibspiUtils_printf(const char *format,
                        ...)
{
#if defined(MIBSPI_CFG_PRINT_ENABLE)
    va_list vaArgPtr;
    char *buf;

    MIBSPI_osalPendLock(gMibspiUtilsObj.printLock, SemaphoreP_WAIT_FOREVER);

    buf = &gMibspiUtilsObj.printBuf[0];
    (void)va_start(vaArgPtr, format);
    (void)vsnprintf(buf, MIBSPI_CFG_PRINT_BUF_LEN, (const char *)format, vaArgPtr);
    va_end(vaArgPtr);

    DebugP_log0(buf);

    MIBSPI_osalPostLock(gMibspiUtilsObj.printLock);
#endif

    return;
}

void MibspiUtils_trace(const char *format,
                       va_list vaArgPtr)
{
#if (MIBSPI_TRACE_CFG_TRACE_LEVEL > MIBSPI_TRACE_CFG_LEVEL_NONE)
    char *buf;

    MIBSPI_osalPendLock(gMibspiUtilsObj.traceLock, SemaphoreP_WAIT_FOREVER);

    buf = &gMibspiUtilsObj.traceBuf[0];
    vsnprintf(buf, MIBSPI_CFG_TRACE_BUF_LEN, (const char *)format, vaArgPtr);

    DebugP_log0(buf);

    MIBSPI_osalPostLock(gMibspiUtilsObj.traceLock);
#endif
}

uint32_t MibspiUtils_virtToPhy (const void *virtAddr)
{
    uint32_t phyAddr;

    phyAddr = (uint32_t) (CSL_locToGlobAddr((uintptr_t)virtAddr));
    return phyAddr;
}


/* end of file */
