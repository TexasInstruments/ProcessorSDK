/**
 *   Copyright (c) Texas Instruments Incorporated 2021
 *   All rights reserved.
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
 *  \file vhwa_m2mFcEventManager.c
 *
 *  \brief
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "src/drv/vhwa_m2mFcDrvPriv.h"


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
static void Vhwa_m2mFcDoneIsr(Vhwa_M2mFcDrvInstObj *instObj);

int32_t Vhwa_m2mVissFrameComplCb(Fvid2_Handle handle, void *appData)
{
    Vhwa_M2mFcDrvInstObj   *instObj = (Vhwa_M2mFcDrvInstObj *)appData;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The test framework does not support the configuration required to trigger this error scenario. 
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application. This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if(NULL != instObj->actQObj)
    /* LDRA_JUSTIFY_END */
    {
        if(VHWA_FC_NODE_VISS == (uint32_t)instObj->actQObj->hObj->lastNode)
        {
            Vhwa_m2mFcDoneIsr(instObj);
        }
    }

    return FVID2_SOK;
}

int32_t Vhwa_m2mMsc1FrameComplCb(Fvid2_Handle handle, void *appData)
{
    Vhwa_M2mFcDrvInstObj   *instObj = (Vhwa_M2mFcDrvInstObj *)appData;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The test framework does not support the configuration required to trigger this error scenario. 
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application. This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if(NULL != instObj->actQObj)
    /* LDRA_JUSTIFY_END */
    {
        if(VHWA_FC_NODE_MSC1 == (uint32_t)instObj->actQObj->hObj->lastNode)
        {
            Vhwa_m2mFcDoneIsr(instObj);
        }
    }

    return FVID2_SOK;
}

int32_t Vhwa_m2mMsc0FrameComplCb(Fvid2_Handle handle, void *appData)
{
    Vhwa_M2mFcDrvInstObj   *instObj = (Vhwa_M2mFcDrvInstObj *)appData;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The test framework does not support the configuration required to trigger this error scenario. 
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application. This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if(NULL != instObj->actQObj)
    /* LDRA_JUSTIFY_END */
    {
        if(VHWA_FC_NODE_MSC0 == (uint32_t)instObj->actQObj->hObj->lastNode)
        {
            Vhwa_m2mFcDoneIsr(instObj);
        }
    }

    return FVID2_SOK;
}

static void Vhwa_m2mFcDoneIsr(Vhwa_M2mFcDrvInstObj *instObj)
{
    Vhwa_M2mFcHandleObj *hObj = NULL;
    Vhwa_M2mFcQueueObj *qObj = NULL;
    uint32_t hwaLockIdx = 0;

    qObj = instObj->actQObj;
    hObj = qObj->hObj;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The test framework does not support the configuration required to trigger this error scenario. 
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application. This behaviour is part of the application design. However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL != hObj->createPrms.getTimeStamp)
    {
       hObj->perfNum = hObj->createPrms.getTimeStamp() - hObj->perfNum;
    }
    /* LDRA_JUSTIFY_END */

    /* Move completed qObject to done queue */
    Fvid2Utils_queue(hObj->doneQ, &qObj->qElem, qObj);

    instObj->lastHndlObj = hObj;
    instObj->actQObj = NULL;

#if defined(VHWA_M2M_VPAC_INSTANCE)
#if (VHWA_M2M_VPAC_INSTANCE == 0)
    hwaLockIdx = VHWA_VPAC0_VISS_LOCK_IDX;
#elif defined(VHWA_M2M_VPAC_INSTANCE) && (VHWA_M2M_VPAC_INSTANCE == 1)
    hwaLockIdx = VHWA_VPAC1_VISS_LOCK_IDX;
#endif
#endif
    /* New request can now be submitted to the VISS IP */
    Vhwa_commonHwaLockRelease(hwaLockIdx);

#if defined(VHWA_M2M_VPAC_INSTANCE)
#if (VHWA_M2M_VPAC_INSTANCE == 0)
    hwaLockIdx = VHWA_VPAC0_MSC0_LOCK_IDX;
#elif defined(VHWA_M2M_VPAC_INSTANCE) && (VHWA_M2M_VPAC_INSTANCE == 1)
    hwaLockIdx = VHWA_VPAC1_MSC0_LOCK_IDX;
#endif
#endif
    /* New request can now be submitted to the MSC IP */
    if(UTRUE == hObj->msc0FcPrms.isFlexConnect)
    {
        Vhwa_commonHwaLockRelease(hwaLockIdx);
    }
    if(UTRUE == hObj->msc1FcPrms.isFlexConnect)
    {
        Vhwa_commonHwaLockRelease(hwaLockIdx + 1U);
    }

    /* New request can now be submitted to the FC */
    (void)VhwaAl_SemaphoreP_post(instObj->pLock);

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The parameters are pre-validated by the caller before the control reaches here. 
    Effect on this unit: If control reaches here, the code base is expected to prevent undefined behaviour by avoiding dereferencing a NULL pointer. However, due to the stated rationale, this is not tested.
    <justification end> */
    if(NULL != hObj->cbPrms.fdmCbFxn)
    /* LDRA_JUSTIFY_END */
    {
        (void)hObj->cbPrms.fdmCbFxn(hObj->cbPrms.fdmData);
    }
}
