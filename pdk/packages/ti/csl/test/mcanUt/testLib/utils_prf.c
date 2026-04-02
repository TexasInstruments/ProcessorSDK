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

/**
 *  \file utils_prf.c
 *
 *  \brief Profiling API utility file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <ti/csl/csl_types.h>

#include "ti/osal/LoadP.h"
#include "utils_prf.h"
#include <st_mcan.h>
#include <ti/osal/src/nonos/Nonos_config.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    uint32_t        isAlloc;
    char            name[32];
    TaskP_Handle    pTsk;
    uint64_t        totalTskThreadTime;
} Utils_PrfLoadObj;

typedef struct
{
    Utils_PrfTsHndl  tsObj[UTILS_PRF_MAX_HNDL];
    Utils_PrfLoadObj loadObj[UTILS_PRF_MAX_HNDL];
} Utils_PrfObj;

typedef struct
{
    uint64_t totalSwiThreadTime;
    uint64_t totalHwiThreadTime;
    uint64_t totalTime;
    uint64_t totalIdlTskTime;
} Utils_AccPrfLoadObj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void App_ConsolePrintf(uint32_t type, uint32_t baseAddr, const char *pcString, ...);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static Utils_PrfObj        gUtils_prfObj;
static Utils_AccPrfLoadObj gUtils_accPrfLoadObj;
extern uint32_t            uartBaseAddr;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Utils_prfInit(void)
{
    memset(&gUtils_prfObj, 0, sizeof (gUtils_prfObj));
    memset(&gUtils_accPrfLoadObj, 0, sizeof (Utils_AccPrfLoadObj));

    return (0);
}

int32_t Utils_prfDeInit(void)
{
    return (0);
}

Utils_PrfTsHndl *Utils_prfTsCreate(const char *name)
{
    uint32_t        hndlId;
    uint32_t        cookie;
    Utils_PrfTsHndl *pHndl = NULL;

    cookie = HwiP_disable();

    for (hndlId = 0; hndlId < UTILS_PRF_MAX_HNDL; hndlId++)
    {
        pHndl = &gUtils_prfObj.tsObj[hndlId];

        if(FALSE == pHndl->isAlloc)
        {
            /* One less for NULL character */
            strncpy(pHndl->name, name, ((uint32_t) sizeof (pHndl->name) - 1U));
            pHndl->name[sizeof (pHndl->name) - 1U] = (UInt8) '\0';
            pHndl->isAlloc = (uint32_t) TRUE;
            Utils_prfTsReset(pHndl);
            break;
        }
    }

    HwiP_restore(cookie);

    return (pHndl);
}

int32_t Utils_prfTsDelete(Utils_PrfTsHndl *pHndl)
{
    pHndl->isAlloc = (uint32_t) FALSE;
    return (0);
}

uint64_t Utils_prfTsBegin(Utils_PrfTsHndl *pHndl)
{
    pHndl->startTs = Utils_prfTsGet64();

    return (pHndl->startTs);
}

uint64_t Utils_prfTsEnd(Utils_PrfTsHndl *pHndl, uint32_t numFrames)
{
    return (Utils_prfTsDelta(pHndl, pHndl->startTs, numFrames));
}

uint64_t Utils_prfTsDelta(Utils_PrfTsHndl *pHndl,
                           uint64_t              startTime,
                           uint32_t              numFrames)
{
    uint64_t endTs;
    uint32_t cookie;

    endTs = Utils_prfTsGet64();

    cookie = HwiP_disable();

    pHndl->totalTs += (endTs - pHndl->startTs);
    pHndl->count++;
    pHndl->numFrames += numFrames;

    HwiP_restore(cookie);

    return (endTs);
}

int32_t Utils_prfTsReset(Utils_PrfTsHndl *pHndl)
{
    uint32_t cookie;

    cookie = HwiP_disable();

    pHndl->totalTs   = 0;
    pHndl->count     = 0;
    pHndl->numFrames = 0;

    HwiP_restore(cookie);

    return (0);
}

uint64_t Utils_prfTsGet64(void)
{
    uint64_t retVal;
    TimeStamp_Struct timeStamp;
    osalArch_TimestampGet64(&timeStamp);
    retVal = (uint64_t) timeStamp.hi << 32U;
    retVal |= timeStamp.lo;
    return retVal;
}

uint32_t Utils_prfTsGetFreq(void)
{
    return (osalArch_TimeStampGetFreqKHz());
}

int32_t Utils_prfTsPrint(Utils_PrfTsHndl *pHndl, uint32_t resetAfterPrint)
{
    int32_t       cpuKhz;
    uint32_t       timeMs, fps, fpc;

    cpuKhz = osalArch_TimeStampGetFreqKHz();

    timeMs = pHndl->totalTs / cpuKhz;

    if(0U == timeMs)
    {
        fps = 0U;
    }
    else
    {
        fps = (pHndl->numFrames * (uint32_t) 1000U) / timeMs;
    }
    if(0U == pHndl->count)
    {
        fpc = 0U;
    }
    else
    {
        fpc = pHndl->numFrames / pHndl->count;
    }

    App_ConsolePrintf((uint32_t)PRINT_MSG_TYPE_NORMAL, uartBaseAddr,
        " %d: PRF : %s : t: %d ms, count: %d, frames: %d, fps: %d, fpc: %d \r\n",
        Utils_getCurTimeInMsec(),
        pHndl->name,
        timeMs,       /* in msecs    */
        pHndl->count,
        pHndl->numFrames,
        fps,       /* frames per second */
        fpc        /* frames per count */
        );

    if(resetAfterPrint)
    {
        Utils_prfTsReset(pHndl);
    }

    return (0);
}

int32_t Utils_prfTsPrintAll(uint32_t resetAfterPrint)
{
    uint32_t        hndlId;
    Utils_PrfTsHndl *pHndl;

    App_ConsolePrintf((uint32_t)PRINT_MSG_TYPE_NORMAL, uartBaseAddr, "\r\n");

    for (hndlId = 0; hndlId < UTILS_PRF_MAX_HNDL; hndlId++)
    {
        pHndl = &gUtils_prfObj.tsObj[hndlId];

        if(TRUE == pHndl->isAlloc)
        {
            Utils_prfTsPrint(pHndl, resetAfterPrint);
        }
    }

    App_ConsolePrintf((uint32_t)PRINT_MSG_TYPE_NORMAL, uartBaseAddr, "\r\n");

    return (0);
}


uint32_t Utils_getCurTimeInMsec(void)
{
    uint64_t curTimeMsec, curTimeUsec;

    curTimeUsec = TimerP_getTimeInUsecs();
    curTimeMsec = (curTimeUsec / 1000U);

    return ((uint32_t) curTimeMsec);
}
