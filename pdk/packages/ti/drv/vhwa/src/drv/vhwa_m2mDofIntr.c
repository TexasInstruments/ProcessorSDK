/**
 *   Copyright (c) Texas Instruments Incorporated 2019
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
 *  \file vhwa_m2mDofIntr.c
 *
 *  \brief DOF Interrupt API Implementation
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "src/drv/vhwa_m2mDofPriv.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   ISR handler for DOF interrupt
 *          Registered to the BIOS
 *
 **/
static void vhwaM2mDofIsr(uintptr_t args);

/**
 * \brief   Frame Complete ISR,
 *          It is called from #vhwaM2mDofIsr when DOF completes a frame
 *          and HTS pipeline is done.
 *
 * \param   instObj             Instance object
 *
 **/
static void vhwaM2mDofFrmDoneIsr(Vhwa_M2mDofInstObj *instObj);

/**
 * \brief   Error Event ISR,
 *          It is called from #vhwaM2mDofIsr when DOF generates error event.
 *
 * \param   instObj             Instance object
 * \param   errStat             Bit mask of occurred error events
 *
 **/
static void vhwaM2mDofErrEventIsr(const Vhwa_M2mDofInstObj *instObj,
    uint32_t errStat);

/**
 * \brief   WdTimer Error Event ISR,
 *          It is called from #vhwaM2mDofIsr when DOF generates Wdtimer error event.
 *
 * \param   instObj             Instance object
 * \param   wdTimer_errStat     Bit mask of occurred error events
 *
 **/
static void vhwam2mDofWdTimerErrEventIsr(Vhwa_M2mDofInstObj *instObj,
                                    uint32_t wdTimer_errStat);
/**
 * \brief   Function to get status of the error and frame done event
 *
 * \param   instObj               Instance object
 * \param   errStat               Bit mask of occurred error events
 * \param   wdTimer_errStat       Status of the DOF watchdog timer error event
 * \param   foco_wdTimer_errStat  Status of the FOCO watchdog timer error event
 * \param   frmDoneStat           Status of the frame done event.
 *
 **/
static void vhwaM2mDofGetIntrStat(const Vhwa_M2mDofInstObj *instObj,
                                    uint32_t *errStat,
                                    uint32_t *wdTimer_errStat,
                                    uint32_t *foco_wdTimer_errStat,
                                    uint32_t *frmDoneStat);


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */



/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Vhwa_m2mDofRegisterIsr(Vhwa_M2mDofInstObj *instObj)
{
    int32_t                          status = FVID2_SOK;
    uint32_t                         events;
    uint32_t                         vhwaIrqNum;
    HwiP_Params                      hwiParams;
    volatile CSL_dmpac_intd_cfgRegs  *intdRegs = NULL;
    uint32_t                         wdtimer_events;

    GT_assert(VhwaDofTrace, (NULL != instObj));

    /* Clear out any pending interrupts */
    VhwaAl_HwiP_clearInterrupt(instObj->irqNum);

    /* Populate the interrupt parameters */
    HwiP_Params_init(&hwiParams);
    #if defined(MCU_PLUS_SDK)
    hwiParams.args = (void*) instObj;
    hwiParams.intNum = (uint32_t) instObj->irqNum;
    hwiParams.callback = (HwiP_FxnCallback) &vhwaM2mDofIsr;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. This failure case is out of scope for the imaging test framework. 
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
    <justification end> */
    if(FVID2_SOK != HwiP_construct(&instObj->intrHndl, &hwiParams))
    #else
    hwiParams.arg = (uintptr_t) instObj;

    /* Register interrupt */
    instObj->intrHndl = HwiP_create(instObj->irqNum,
        &vhwaM2mDofIsr, &hwiParams);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This failure case is out of scope for the imaging test framework.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == instObj->intrHndl)
    #endif
    {
        GT_0trace(VhwaDofTrace, GT_ERR,
            "Interrupt Registration Failed !!\n");
        status = FVID2_EFAIL;
    }
    /* LDRA_JUSTIFY_END */
    /* Enable Error Events */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This failure case is out of scope for the imaging test framework.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
     /* LDRA_JUSTIFY_END */
    {
        vhwaIrqNum  = instObj->vhwaIrqNum;
        intdRegs    = instObj->socInfo.dmpacIntdRegs;

        /* All Error Events are by default enabled */
        /* TODO Enable Error Events */
        events = VHWA_DOF_ALL_ERR_EVENTS;
        /* Enable HTS Interrupt in INTD Module */
        CSL_REG32_WR(&intdRegs->ENABLE_REG_LEVEL_DMPAC_OUT[vhwaIrqNum ][VHWA_DMPAC_INTD_DOF_INTR_REG],
            events);

        /* All Wdtimer Error Events are by default disabled */
        wdtimer_events = VHWA_DOF_ALL_WDTIMER_ERR_EVENTS;
        /* Enable HTS Interrupt in INTD Module */
        CSL_REG32_WR(&intdRegs->ENABLE_CLR_REG_LEVEL_DMPAC_OUT[vhwaIrqNum][VHWA_DMPAC_INTD_WDTIMER_INTR_REG],
            wdtimer_events);
    }

    return (status);
}

void Vhwa_m2mDofUnregisterIsr(Vhwa_M2mDofInstObj *instObj)
{
    uint32_t                         events;
    uint32_t                         vhwaIrqNum;
    CSL_dmpac_intd_cfgRegs           *intdRegs = NULL;
    uint32_t                         wdtimer_events;

    /* LDRA_JUSTIFY_START
    <metric start>  branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If control reaches here, the code base is expected to prevent undefined behaviour by avoiding dereferencing a NULL pointer.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL != instObj)
    /* LDRA_JUSTIFY_END */
    {
        vhwaIrqNum  = instObj->vhwaIrqNum;
        intdRegs    = instObj->socInfo.dmpacIntdRegs;

        /* If not already done, disable HTS Interrupt */
        Vhwa_disableDmpacHtsIntr(intdRegs, instObj->vhwaIrqNum, instObj->pipeline);

        /* All Error Events are by default enabled */
        events = VHWA_DOF_ALL_ERR_EVENTS;
        /* Disable HTS Interrupt in INTD Module */
        CSL_REG32_WR(&intdRegs->ENABLE_CLR_REG_LEVEL_DMPAC_OUT[vhwaIrqNum][VHWA_DMPAC_INTD_DOF_INTR_REG],
            events);

         /* All Wdtimer Error Events are by default enabled */
        wdtimer_events = VHWA_DOF_ALL_WDTIMER_ERR_EVENTS;
        /* Disable HTS Interrupt in INTD Module */
        CSL_REG32_WR(&intdRegs->ENABLE_CLR_REG_LEVEL_DMPAC_OUT[vhwaIrqNum][VHWA_DMPAC_INTD_WDTIMER_INTR_REG],
            wdtimer_events);

#if defined(MCU_PLUS_SDK)
        (void)HwiP_destruct(&instObj->intrHndl);
#else
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> Rationale: The negative test framework and test apps cannot reach this portion.
        The interrupt handle is registered here: Vhwa_m2mDofRegisterIsr
        This will happen regardless of the test configuration. If interrupt is null during register function call, an error is thrown there.
        Therefore, the test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If control reaches here, the code base is expected to prevent undefined behaviour by avoiding dereferencing a NULL pointer.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if (NULL != instObj->intrHndl)
        /* LDRA_JUSTIFY_END */
        {
            /* Un-register interrupt */
            (void)HwiP_delete(instObj->intrHndl);
            instObj->intrHndl = NULL;
        }
#endif
    }
}

static void vhwaM2mDofIsr(uintptr_t args)
{
    uint32_t            errStat;
    uint32_t            frmDoneStat;
    Vhwa_M2mDofInstObj *instObj = NULL;
    uint32_t            wdTimer_errStat;
    uint32_t            foco_wdTimer_errStat;
    Vhwa_M2mDofHandleObj            *hObj = NULL;
    uint32_t            pyrLvl;

    GT_assert(VhwaDofTrace, (NULL != (Vhwa_M2mDofInstObj *)args));

    instObj = (Vhwa_M2mDofInstObj *)args;

    instObj->totalIntrCnt ++;
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameter is expected to be pre-validated in the imaging layer before reaching vhwa.
    Effect on this unit: If control reaches here, the code base is expected to prevent undefined behaviour by avoiding dereferencing a NULL pointer.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL != instObj->actQObj) && (NULL != instObj->actQObj->hObj))
    /* LDRA_JUSTIFY_END */
    {
        hObj = instObj->actQObj->hObj;
    }

    vhwaM2mDofGetIntrStat(instObj, &errStat, &wdTimer_errStat, &foco_wdTimer_errStat, &frmDoneStat);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> Rationale: The test framework and test apps cannot reach this portion. Covering in NEGATIVE_TEST build.
    Effect on this unit: This portion is expected to trigger the error event ISR.
    <justification end> */
    if (0U != errStat)
    {
        /* First check if any of the registered error events has come,
           then first call the callback for the error events */
        vhwaM2mDofErrEventIsr(instObj, errStat);
    }

    if (0U != wdTimer_errStat)
    {
        /* Set stalled flag to prevent further process requests */
        instObj->isStalled = (uint32_t)UTRUE;

        if(NULL != hObj)
        {
            for(pyrLvl = 0u; pyrLvl < hObj->dofPrms.tPrmdLvl; pyrLvl++)
            {
                /* Disable watchdog timer after its first trigger*/
                hObj->htsCfg[pyrLvl].enableWdTimer = UFALSE;

                CSL_dmpacHtsSchConfigInit(&hObj->htsCfg[pyrLvl]);
            }
        }
        /* First check if any of the registered error events has come,
           then first call the callback for the error events */
        vhwam2mDofWdTimerErrEventIsr(instObj, wdTimer_errStat);
    }

    if (0U != foco_wdTimer_errStat)
    {
        /* Set stalled flag to prevent further process requests */
        instObj->isStalled = (uint32_t)UTRUE;

        if(NULL != hObj)
        {
            for(pyrLvl = 0u; pyrLvl < hObj->dofPrms.tPrmdLvl; pyrLvl++)
            {
                /* Disable watchdog timer after its first trigger*/
                hObj->focoHtsCfg[pyrLvl].enableWdTimer = UFALSE;

                CSL_dmpacHtsSchConfigInit(&hObj->focoHtsCfg[pyrLvl]);
            }
        }
        /* First check if any of the registered error events has come,
           then first call the callback for the error events */
        vhwam2mDofWdTimerErrEventIsr(instObj, foco_wdTimer_errStat);
    }
    /* LDRA_JUSTIFY_END */

    /* LDRA_JUSTIFY_START
    <metric start>  branch <metric end>
    <justification start> Rationale: The test framework and test apps cannot reach this portion.
    frmDoneStat is never be zero, as it is getting nonzero value from function vhwaM2mDofGetIntrStat before reaching here
    Effect on this unit: No else statement is required as the invalid status flag is incremented at the end of the function.
    <justification end> */
    if (0U != frmDoneStat)
     /* LDRA_JUSTIFY_END */
    {
        /* Now process the Frame Done ISR */
        vhwaM2mDofFrmDoneIsr(instObj);
    }
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The test framework and test apps cannot reach this portion. frmDoneStat is never be zero.
    Effect on this unit: This portion is expected to increment the invalid status flag.
    <justification end> */
    if((0u == errStat) && (0U == wdTimer_errStat) && (0U == frmDoneStat))
    {
        instObj->errIntrInvalidStatus ++;
    }
    /* LDRA_JUSTIFY_END */
}

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> Rationale: The test framework and test apps cannot reach this portion. Covering in NEGATIVE_TEST build.
Effect on this unit: This function is the error event ISR for DOF.
It is expected to be called when DOF generates an error event.
<justification end> */
static void vhwaM2mDofErrEventIsr(const Vhwa_M2mDofInstObj *instObj,
    uint32_t errStat)
{
    Dof_ErrEventParams              *eePrms = NULL;
    Vhwa_M2mDofHandleObj            *hObj = NULL;
    uint32_t                        errStatus;

    if (NULL != instObj)
    {
        /* Active Handle Object must not be null */
        GT_assert(VhwaDofTrace, (NULL != instObj->actQObj));
        /* Handle Object in QObject must not be null */
        GT_assert(VhwaDofTrace, (NULL != instObj->actQObj->hObj));

        hObj   = instObj->actQObj->hObj;
        eePrms = &hObj->eePrms;

        /* Check if any of the error event has occurred */
        errStatus = errStat & eePrms->errEvents;

        /* Process only if error events are enabled for this handle */
        if (0U != errStatus)
        {
            /* Call Application callback, for the completed handle object */
            if(NULL != eePrms->cbFxn)
            {
                eePrms->cbFxn((Fvid2_Handle)hObj, errStatus, eePrms->appData);
            }
        }
    }
}
/* LDRA_JUSTIFY_END */

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> Rationale: The test framework and test apps cannot reach this portion. Covering in NEGATIVE_TEST build.
Effect on this unit: This function is the watchdog timer error event ISR for DOF.
It is expected to be called when DOF generates a watchdog timer error event.
<justification end> */
static void vhwam2mDofWdTimerErrEventIsr(Vhwa_M2mDofInstObj *instObj,
    uint32_t wdTimer_errStat)
{
    uint32_t                        errStatus;
    Dof_WdTimerErrEventParams       *wdTimerEePrms = NULL;
    Vhwa_M2mDofHandleObj            *hObj = NULL;
    if (NULL != instObj)
    {
        if ((NULL == instObj->actQObj) || (NULL == instObj->actQObj->hObj))
        {
            instObj->errNoActQObj ++;
            if (NULL != instObj->lastHndlObj)
            {
                hObj = instObj->lastHndlObj;
                wdTimerEePrms = &hObj->wdTimerEePrms;
            }
            else
            {
                hObj = NULL;
                instObj->errNoLastHndlObj ++;
            }
        }
        else
        {
            hObj   = instObj->actQObj->hObj;
            wdTimerEePrms = &hObj->wdTimerEePrms;
        }
        if(NULL != hObj)
        {
            /* Check if any of the error event has occurred */
            errStatus = wdTimer_errStat & wdTimerEePrms->wdTimerErrEvents;

            /* Process only if error events are enabled for this handle */
            if (0U != errStatus)
            {
                /* Call Application callback, for the completed handle object */
                if (NULL != wdTimerEePrms->cbFxn)
                {
                    wdTimerEePrms->cbFxn((Fvid2_Handle)hObj, errStatus, wdTimerEePrms->appData);
                }
            }
        }
    }
}
/* LDRA_JUSTIFY_END */

static void vhwaM2mDofFrmDoneIsr(Vhwa_M2mDofInstObj *instObj)
{
    Vhwa_M2mDofHandleObj *doneHndlObj = NULL;
    Vhwa_M2mDofQueueObj  *qObj = NULL;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> Rationale: The test framework and test apps cannot reach this portion. instObj can't be NULL as it is pre-validated in vhwaM2mDofIsr
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != instObj)
    /* LDRA_JUSTIFY_END */
    {
        /**
         * Steps
         * 1, Clear the interrupt
         * 2, Move the queue object to doneQ
         * 3, Mark Active Queue Object to NULL and lastHandle to
         *    completed handle
         * 4, If the reqQ is not empty, Remove one object from reqQ
         * 5, if the last handle is same as this handle, submit the TR
         * 6, If they are not same, set the config and then submit the TR
         * 7, move the new object to actQ
         * 8, Call the application callback
         */
        /* Active Handle Object must not be null */
        GT_assert(VhwaDofTrace, (NULL != instObj->actQObj));

        qObj = instObj->actQObj;

        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> Rationale: The test framework and test apps cannot reach this portion. qObj can't be NULL as it is pre-validated above
        Effect on this unit: If control reaches here, the code base is expected to prevent undefined behaviour by avoiding dereferencing a NULL pointer.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if (NULL != qObj)
        /* LDRA_JUSTIFY_END */
        {
            /* Handle Object in QObject must not be null */
            GT_assert(VhwaDofTrace, (NULL != qObj->hObj));

            doneHndlObj = qObj->hObj;
            /* LDRA_JUSTIFY_START
            <metric start> branch <metric end>
            <justification start> Rationale: The negative test framework and test apps cannot reach this portion.
            vhwaM2mDofFrmDoneIsr is called when a frame is done processing. Therefore, the pipeline is stopped and the enable bit is set to 0.
            If vhwaM2mDofFrmDoneIsr call is made when the pipeline is still enabled, the errInvalidHtsPipelineStatus is incremented.
            This behaviour can't be produced through a test configuration.
            Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            if (0u == CSL_dmpacHtsPipelineStatus(instObj->socInfo.htsRegs,
                &doneHndlObj->htsCfg[doneHndlObj->curPyrLvl]))
            /* LDRA_JUSTIFY_END */
            {
                /* Get the timestmap for performance numbers */
                /* LDRA_JUSTIFY_START
                <metric start> statement branch <metric end>
                <justification start>
                Rationale: The test framework and test apps cannot reach this portion
                This portion is unused because of an un-implemented debug feature, this code base is not being used/enabled with the current kernel support.
                Effect on this unit: None; Unused feature; If the control reaches here the code is expected to calculate the performance numbers.
                However, due to the stated rationale, this is not tested.
                <justification end> */
                if (NULL != doneHndlObj->createArgs.getTimeStamp)
                {
                    doneHndlObj->perfNum =
                        doneHndlObj->createArgs.getTimeStamp() -
                            doneHndlObj->perfNum;
                }
                /* LDRA_JUSTIFY_END */

                /* Move completed qObject to done queue */
                Fvid2Utils_queue(doneHndlObj->doneQ, &qObj->qElem, qObj);

                instObj->lastHndlObj = doneHndlObj;
                instObj->actQObj = NULL;


                /* Call Application callback, for the completed handle object */
                /* LDRA_JUSTIFY_START
                <metric start> branch <metric end>
                <justification start> Rationale:The negative test framework and test apps cannot reach this portion.
                fdmCbFxn is populated in the fvid2_create function.fdmCbFxn will be NULL if cbParams→cbFxn is null.
                fdmCbFxn is always populated in the create call.
                Effect on this unit: None as no need to call the fdmCbFxn when it is NULL.
                <justification end> */
                if(NULL != doneHndlObj->cbPrms.fdmCbFxn)
                /* LDRA_JUSTIFY_END */
                {
                    (void)doneHndlObj->cbPrms.fdmCbFxn(doneHndlObj->cbPrms.fdmData);
                }
            }
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start> Rationale: The negative test framework and test apps cannot reach this portion.
            vhwaM2mDofFrmDoneIsr is called when a frame is done processing. Therefore, the pipeline is stopped and the enable bit is set to 0.
            If vhwaM2mDofFrmDoneIsr call is made when the pipeline is still enabled, the errInvalidHtsPipelineStatus is incremented.
            This behaviour can't be produced through a test configuration.
            Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            else
            {
                instObj->errInvalidHtsPipelineStatus ++;
            }
            /* LDRA_JUSTIFY_END */
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> Rationale: The test framework and test apps cannot reach this portion. else cant be reached as qObj can't be NULL and it is pre-validated above
        Effect on this unit: This portion is expected to increment the errIntrRepeateIntr.
        <justification end> */
        else
        {
            instObj->errIntrRepeateIntr ++;
        }
        /* LDRA_JUSTIFY_END */
    }
}

static void vhwaM2mDofGetIntrStat(const Vhwa_M2mDofInstObj *instObj,
                                    uint32_t *errStat,
                                    uint32_t *wdTimer_errStat,
                                    uint32_t *foco_wdTimer_errStat,
                                    uint32_t *frmDoneStat)
{
    uint32_t                        vhwaIrqNum;
    volatile uint32_t               regVal;
    volatile CSL_dmpac_intd_cfgRegs *intdRegs = NULL;
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> Rationale: The test framework and test apps cannot reach this portion. instObj can't be NULL as it is pre-validated in vhwaM2mDofIsr
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != instObj)
    /* LDRA_JUSTIFY_END */
    {
        vhwaIrqNum  = instObj->vhwaIrqNum;
        intdRegs    = instObj->socInfo.dmpacIntdRegs;

        vhwa_GetErrIntrStat(&intdRegs->STATUS_REG_LEVEL_DMPAC_OUT[vhwaIrqNum][VHWA_DMPAC_INTD_DOF_INTR_REG],
                        &intdRegs->STATUS_CLR_REG_LEVEL_DMPAC_OUT[vhwaIrqNum][VHWA_DMPAC_INTD_DOF_INTR_REG], errStat, VHWA_DMPAC_INTD_DOF_ERR_MASK);

        vhwa_GetWdtimerErrIntrStat(&intdRegs->STATUS_REG_LEVEL_DMPAC_OUT[vhwaIrqNum][VHWA_DMPAC_INTD_WDTIMER_INTR_REG],
                        &intdRegs->STATUS_CLR_REG_LEVEL_DMPAC_OUT[vhwaIrqNum][VHWA_DMPAC_INTD_WDTIMER_INTR_REG], wdTimer_errStat, VHWA_DMPAC_INTD_DOF_WDTIMER_ERR_BIT);

        vhwa_GetWdtimerErrIntrStat(&intdRegs->STATUS_REG_LEVEL_DMPAC_OUT[vhwaIrqNum][VHWA_DMPAC_INTD_WDTIMER_INTR_REG],
                        &intdRegs->STATUS_CLR_REG_LEVEL_DMPAC_OUT[vhwaIrqNum][VHWA_DMPAC_INTD_WDTIMER_INTR_REG], foco_wdTimer_errStat, VHWA_DMPAC_INTD_DOF_FOCO_WDTIMER_ERR_BIT);

        regVal = CSL_REG32_RD(
            &intdRegs->STATUS_REG_LEVEL_DMPAC_OUT[vhwaIrqNum][VHWA_DMPAC_INTD_PIPE_INTR_REG]);

        *frmDoneStat = 0U;
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> Rationale: The test framework and test apps cannot reach this portion.
        false case of this condition is not possible as the register read will clear the interrupt bit value
        Effect on this unit: This unit clears the interrupt bit value and returns the frame done status. If no interrupt is pending, the frame done status will be 0.
        Therefore, no else statement is required.
        <justification end> */
        if (0U != (regVal & ((uint32_t)1u << instObj->pipeline)))
        /* LDRA_JUSTIFY_END */
        {
            /* Clear the interrupt */
            regVal = ((uint32_t)1u << instObj->pipeline);
            CSL_REG32_WR(
                &intdRegs->STATUS_CLR_REG_LEVEL_DMPAC_OUT[vhwaIrqNum][VHWA_DMPAC_INTD_PIPE_INTR_REG],
                regVal);

            *frmDoneStat = 1U;
        }
    }
}
