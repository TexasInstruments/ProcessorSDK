/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2023
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
#include "ti/osal/osal.h"
#include "ti/osal/TaskP.h"
#include "ti/osal/LoadP.h"
#include "udma_test.h"

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

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static Utils_PrfObj        gUtils_prfObj;
static LoadP_Stats         gLoadStatsTask;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

Utils_PrfTsHndl *Utils_prfTsCreate(const char *name)
{
    uint32_t        hndlId;
    uint32_t        cookie;
    Utils_PrfTsHndl *pHndl = NULL;

    cookie = HwiP_disable();

    for (hndlId = 0; hndlId < UTILS_PRF_MAX_HNDL; hndlId++)
    {
        pHndl = &gUtils_prfObj.tsObj[hndlId];

        if(UFALSE == pHndl->isAlloc)
        {
            /* One less for NULL character */
            strncpy(pHndl->name, name, ((uint32_t) sizeof (pHndl->name) - 1U));
            pHndl->name[sizeof (pHndl->name) - 1U] = (uint8_t) '\0';
            pHndl->isAlloc = UTRUE;
            Utils_prfTsReset(pHndl);
            break;
        }
    }

    HwiP_restore(cookie);

    return (pHndl);
}

int32_t Utils_prfTsDelete(Utils_PrfTsHndl *pHndl)
{
    pHndl->isAlloc = UFALSE;
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
    uint64_t curTs = (uint64_t) AppUtils_getCurTimeInMsec();
    return (curTs);
}

int32_t Utils_prfTsPrint(Utils_PrfTsHndl *pHndl, uint32_t resetAfterPrint, uint32_t trace)
{
    uint32_t       timeMs, fps, fpc;


    timeMs = pHndl->totalTs;

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

    GT_7trace(
        trace, GT_INFO,
        " %d: PRF : %s : t: %d ms, count: %d, frames: %d, fps: %d, fpc: %d \r\n",
        AppUtils_getCurTimeInMsec(),
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

int32_t Utils_prfTsPrintAll(uint32_t resetAfterPrint, uint32_t trace)
{
    uint32_t        hndlId;
    Utils_PrfTsHndl *pHndl;

    GT_0trace(trace, GT_INFO, "\r\n");

    for (hndlId = 0; hndlId < UTILS_PRF_MAX_HNDL; hndlId++)
    {
        pHndl = &gUtils_prfObj.tsObj[hndlId];

        if(UTRUE == pHndl->isAlloc)
        {
            Utils_prfTsPrint(pHndl, resetAfterPrint, trace);
        }
    }

    GT_0trace(trace, GT_INFO, "\r\n");

    return (0);
}

int32_t Utils_prfLoadRegister(TaskP_Handle pTsk, const char *name)
{
    uint32_t            hndlId;
    uint32_t            cookie;
    int32_t             status = CSL_EFAIL;
    Utils_PrfLoadObj   *pHndl;

    cookie = HwiP_disable();

    for (hndlId = 0; hndlId < UTILS_PRF_MAX_HNDL; hndlId++)
    {
        pHndl = &gUtils_prfObj.loadObj[hndlId];

        if(UFALSE == pHndl->isAlloc)
        {
            pHndl->isAlloc = UTRUE;
            pHndl->pTsk    = pTsk;
            /* One less for NULL character */
            strncpy(pHndl->name, name, ((uint32_t) sizeof (pHndl->name) - 1U));
            pHndl->name[sizeof (pHndl->name) - 1U] = (uint8_t) '\0';
            status = CSL_SOK;
            break;
        }
    }

    HwiP_restore(cookie);

    return (status);
}

int32_t Utils_prfLoadUnRegister(TaskP_Handle pTsk)
{
    uint32_t            hndlId;
    uint32_t            cookie;
    int32_t             status = CSL_EFAIL;
    Utils_PrfLoadObj   *pHndl;

    cookie = HwiP_disable();

    for (hndlId = 0; hndlId < UTILS_PRF_MAX_HNDL; hndlId++)
    {
        pHndl = &gUtils_prfObj.loadObj[hndlId];

        if((UTRUE == pHndl->isAlloc) && (pHndl->pTsk == pTsk))
        {
            pHndl->isAlloc = UFALSE;
            status         = CSL_SOK;
            break;
        }
    }

    HwiP_restore(cookie);

    return (status);
}

int32_t Utils_prfLoadPrintAll(uint32_t printTskLoad, uint32_t trace)
{
    uint32_t            tskLoad, hndlId, cpuLoad;
    Utils_PrfLoadObj   *pHndl;

    LoadP_Status status = LoadP_OK;

    /* Query CPU Load */
    cpuLoad = LoadP_getCPULoad();

    GT_0trace(trace, GT_INFO, "\r\n");
    GT_2trace(trace, GT_INFO,
              " %d: LOAD: CPU: %d%% \r\n",
              AppUtils_getCurTimeInMsec(),
              cpuLoad);

    if(UTRUE == printTskLoad)
    {
        for (hndlId = 0; hndlId < UTILS_PRF_MAX_HNDL; hndlId++)
        {
            pHndl = &gUtils_prfObj.loadObj[hndlId];

            if(UTRUE == pHndl->isAlloc)
            {
                status += LoadP_getTaskLoad(pHndl->pTsk, &gLoadStatsTask);
                tskLoad = gLoadStatsTask.percentLoad;

                GT_3trace(trace, GT_INFO,
                          " %d: LOAD: TSK: %s: %d%% \r\n",
                          AppUtils_getCurTimeInMsec(),
                          pHndl->name,
                          tskLoad);
            }
        }
    }

    GT_0trace(trace, GT_INFO, "\r\n");

    return status;
}

void Utils_prfLoadCalcStart(void)
{
    memset(&gUtils_prfObj, 0U, sizeof(gUtils_prfObj));
    LoadP_reset();

    return;
}

void Utils_prfLoadCalcStop(void)
{
    return;
}

void Utils_prfLoadCalcReset(void)
{
    uint32_t            hndlId;
    Utils_PrfLoadObj   *pHndl;

    /* Reset the performace loads accumulator */
    for (hndlId = 0; hndlId < UTILS_PRF_MAX_HNDL; hndlId++)
    {
        pHndl = &gUtils_prfObj.loadObj[hndlId];

        if((UTRUE == pHndl->isAlloc) &&
            (NULL != pHndl->pTsk))
        {
            pHndl->totalTskThreadTime = 0;
        }
    }

    return;
}

/* Function called by Loadupdate for each update cycle */
void Utils_prfLoadUpdate(void)
{
    return;
}
