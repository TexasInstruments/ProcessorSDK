/*
 *  Copyright (c) Texas Instruments Incorporated 2020-2022
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
 *  \file csitx_event.c
 *
 *  \brief File containing the CSITX driver event management functions.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/drv/csitx/src/csitx_drvPriv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 *  \brief Enable the event in event group
 *
 *  \param instObj      CSITX instance object pointer
 *  \param eventGroup   Event group in which desired event is to be enabled.
 *  \param eventType    Event/s to be enabled.
 *
 *  \return FVID2_SOK on success, error otherwise
 */
static int32_t CsitxDrv_eventEnable(CsitxDrv_InstObj *instObj,
                                    uint32_t eventGroup,
                                    uint32_t eventType);

/**
 *  \brief Disable the event in event group
 *
 *  \param instObj      CSITX instance object pointer
 *  \param eventGroup   Event group in which desired event is to be enabled.
 *  \param eventType    Event/s to be enabled.
 *
 *  \return FVID2_SOK on success, error otherwise
 */
static int32_t CsitxDrv_eventDisable(CsitxDrv_InstObj *instObj,
                                     uint32_t eventGroup,
                                     uint32_t eventType);

static void CsitxDrv_txEventIsrFxn(uintptr_t arg);
static int32_t CsitxDrv_eventCheckParams(const Csitx_EventPrms *eventPrms);
static int32_t CsitxDrv_eventGroupAllocResource(CsitxDrv_InstObj *instObj,
                                                Csitx_EventPrms *eventPrms);
static int32_t CsitxDrv_resetModule(CsitxDrv_InstObj *instObj);
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void Csitx_eventPrmsInit(Csitx_EventPrms *eventPrms)
{
    if (NULL != eventPrms)
    {
        eventPrms->eventGroup           = CSITX_EVENT_GROUP_TX_IRQ;
        eventPrms->eventMasks           = CSITX_EVENT_TYPE_ERR_ALL;
        eventPrms->eventCb              = NULL;
        eventPrms->coreIntrNum          = CSITX_INTR_INVALID;
        eventPrms->intrPriority         = 0U;
        eventPrms->appData              = NULL;
    }
}

int32_t CsitxDrv_eventGroupRegister(CsitxDrv_InstObj *instObj,
                                    Csitx_EventPrms *eventPrms)
{
    int32_t retVal = FVID2_SOK;
    uint32_t chIdx;
    CsitxDrv_EventObj *eventObj;
    CsitxDrv_ChObj *chObj;
    OsalRegisterIntrParams_t intrPrms;

    /* Error Check: Parameters */
    GT_assert(CsitxTrace, (NULL_PTR != instObj));
    GT_assert(CsitxTrace, (NULL_PTR != eventPrms));
    if (FVID2_SOK == retVal)
    {
        if (CSITX_DRV_USAGE_STATUS_IN_USE != instObj->inUse)
        {
            retVal = FVID2_EFAIL;
        }
    }

    if (UTRUE == instObj->eventRegDone)
    {
        /* Only allow event registration on first call */
        if (retVal == FVID2_SOK)
        {
            retVal = CsitxDrv_eventCheckParams(eventPrms);
        }
    }

    if (FVID2_SOK == retVal)
    {
        eventObj = &instObj->eventObj[eventPrms->eventGroup];
        /* Copy event configuration parameters to driver object */
        switch (eventPrms->eventGroup)
        {
            case CSITX_EVENT_GROUP_TX_IRQ:
                Fvid2Utils_memcpy(&eventObj->eventPrms, eventPrms, sizeof(Csitx_EventPrms));
            break;
            default:
            break;
        }
        /* Copy driver handle to event object */
        eventObj->drvHandle = instObj;
        /* Allocate resource to the event group */
        retVal = CsitxDrv_eventGroupAllocResource(instObj, eventPrms);
    }

    if (FVID2_SOK == retVal)
    {
        /* Enable respective events */
        retVal = CsitxDrv_eventEnable(instObj,
                                      eventPrms->eventGroup,
                                      eventPrms->eventMasks);
        for (chIdx = 0U ; chIdx < instObj->createParams.numCh ; chIdx++)
        {
            chObj =&instObj->chObj[chIdx];
            if (CSITX_CH_TYPE_LPBK == chObj->chCfg->chType)
            {
                /* Enable Frame events */
                retVal = CsitxDrv_eventEnable(instObj,
                                              CSITX_EVENT_GROUP_TX_IRQ,
                                              CSITX_EVENT_TYPE_FRAME_IRQ);
                break;
            }
        }

    }

    if (FVID2_SOK == retVal)
    {
        /* Register interrupt */
        Osal_RegisterInterrupt_initParams(&intrPrms);
        /* Populate the interrupt parameters */
        intrPrms.corepacConfig.arg              = (uintptr_t) eventObj;
        intrPrms.corepacConfig.isrRoutine       = CsitxDrv_txEventIsrFxn;
        intrPrms.corepacConfig.priority         = eventPrms->intrPriority;
        intrPrms.corepacConfig.corepacEventNum  = 0U;
        intrPrms.corepacConfig.intVecNum        = eventObj->coreIntrNum;
        if (OSAL_INT_SUCCESS != Osal_RegisterInterrupt(&intrPrms, &eventObj->hwiHandle))
        {
            eventObj->hwiHandle = NULL;
            retVal = FVID2_EFAIL;
        }

    }

    if (FVID2_SOK == retVal)
    {
        eventObj->eventInitDone = UTRUE;
        instObj->eventRegDone   = UTRUE;
    }


    return retVal;
}

static int32_t CsitxDrv_eventCheckParams(const Csitx_EventPrms *eventPrms)
{
    int32_t retVal = FVID2_SOK;

    if ((0U == eventPrms->eventMasks) &&
        (CSITX_EVENT_GROUP_MAX <= eventPrms->eventGroup))
    {
        retVal = FVID2_EBADARGS;
    }

    return retVal;
}

static int32_t CsitxDrv_eventGroupAllocResource(CsitxDrv_InstObj *instObj,
                                                Csitx_EventPrms *eventPrms)
{
    int32_t retVal = FVID2_SOK;

    eventPrms->coreIntrNum = CSITX_INTR_INVALID;
    switch (instObj->drvInstId)
    {
        case CSITX_INSTANCE_ID_0:
            switch (eventPrms->eventGroup)
            {
                case CSITX_EVENT_GROUP_TX_IRQ:
                    eventPrms->coreIntrNum = CSITX_CORE_INTR_NUM_MOD_0_TX_INTR;
                break;
                default:
                    retVal = FVID2_EBADARGS;
                break;
            }
            break;
#if defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)
	    case CSITX_INSTANCE_ID_1:
            switch (eventPrms->eventGroup)
            {
                case CSITX_EVENT_GROUP_TX_IRQ:
                    eventPrms->coreIntrNum = CSITX_CORE_INTR_NUM_MOD_1_TX_INTR;
                break;
                default:
                    retVal = FVID2_EBADARGS;
                break;
            }
            break;
#endif            
        default:
            retVal = FVID2_EBADARGS;
        break;
    }
    instObj->eventObj[eventPrms->eventGroup].coreIntrNum =
                                                    eventPrms->coreIntrNum;

    return retVal;
}

static int32_t CsitxDrv_eventEnable(CsitxDrv_InstObj *instObj,
                                    uint32_t eventGroup,
                                    uint32_t eventType)
{
    int32_t retVal = FVID2_SOK;
    uint32_t regVal = 0U;

    switch (eventGroup)
    {
        case CSITX_EVENT_GROUP_TX_IRQ:
            /* Enable Interrupt */
            if (CDN_EOK != CSITX_SetInterruptsEnable(&instObj->cslObj.cslCfgData,
                                          BTRUE))
            {
                retVal = FVID2_EFAIL;
            }

            if (FVID2_SOK == retVal)
            {
                regVal = 0U;
                /* TODO: Direct register read/write cause current CSL-FL does
                   not support programming of this registers */
                if(CSITX_INSTANCE_ID_0 == instObj->drvInstId)
                {
                    regVal = CSL_REG32_RD(
                        (CSI_TX_IF0_VBUS2APBCSI2TX_BASE +
                        CSL_CSITX_IRQ_MASK));
                }
#if defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)
                else if(CSITX_INSTANCE_ID_1 == instObj->drvInstId)
                {
                    regVal = CSL_REG32_RD(
                        (CSI_TX_IF1_VBUS2APBCSI2TX_BASE +
                        CSL_CSITX_IRQ_MASK));
                }
#endif
                else
                {
                    /* Do nothing */
                }
            }

            if (FVID2_SOK == retVal)
            {
                regVal |= eventType;
                if(CSITX_INSTANCE_ID_0 == instObj->drvInstId)
                {
                    CSL_REG32_WR(
                        (CSI_TX_IF0_VBUS2APBCSI2TX_BASE +
                        CSL_CSITX_IRQ_MASK),
                        regVal);
                }
#if defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)
                else if(CSITX_INSTANCE_ID_1 == instObj->drvInstId)
                {
                    CSL_REG32_WR(
                        (CSI_TX_IF1_VBUS2APBCSI2TX_BASE +
                        CSL_CSITX_IRQ_MASK),
                        regVal);
                }
#endif
                else
                {
                    /* Do nothing */
                }
            }
            
        break;
        default:
            retVal = FVID2_EBADARGS;
        break;
    }

    return retVal;
}

static int32_t CsitxDrv_eventDisable(CsitxDrv_InstObj *instObj,
                                     uint32_t eventGroup,
                                     uint32_t eventType)
{
    int32_t retVal = FVID2_SOK;
    uint32_t regVal;

    switch (eventGroup)
    {
        case CSITX_EVENT_GROUP_TX_IRQ:
            /* Enable Interrupt */
            if (CDN_EOK != CSITX_SetInterruptsEnable(&instObj->cslObj.cslCfgData,
                                          BFALSE))
            {
                retVal = FVID2_EFAIL;
            }

            if (FVID2_SOK == retVal)
            {
                regVal = 0U;
                /* TODO: Direct register read/write cause current CSL-FL does
                   not support programming of this registers */
                if(CSITX_INSTANCE_ID_0 == instObj->drvInstId)
                {
                    regVal = CSL_REG32_RD(
                        (CSI_TX_IF0_VBUS2APBCSI2TX_BASE +
                        CSL_CSITX_IRQ_MASK));
                }
#if defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)
                else if(CSITX_INSTANCE_ID_1 == instObj->drvInstId)
                {
                    regVal = CSL_REG32_RD(
                        (CSI_TX_IF1_VBUS2APBCSI2TX_BASE +
                        CSL_CSITX_IRQ_MASK));
                }
#endif                
                else
                {
                    /* Do nothing */
                }
            }

            if (FVID2_SOK == retVal)
            {
                regVal &= (~eventType);
                if(CSITX_INSTANCE_ID_0 == instObj->drvInstId)
                {
                    CSL_REG32_WR(
                        (CSI_TX_IF0_VBUS2APBCSI2TX_BASE +
                        CSL_CSITX_IRQ_MASK),
                        regVal);
                }
#if defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)
                else if(CSITX_INSTANCE_ID_1 == instObj->drvInstId)
                {
                    CSL_REG32_WR(
                        (CSI_TX_IF1_VBUS2APBCSI2TX_BASE +
                        CSL_CSITX_IRQ_MASK),
                        regVal);
                }
#endif                
                else
                {
                    /* Do nothing */
                }
            }
        break;
        default:
            retVal = FVID2_EBADARGS;
        break;
    }

    return retVal;
}

static void CsitxDrv_txEventIsrFxn(uintptr_t arg)
{
    CsitxDrv_InstObj *instObj;
    CsitxDrv_EventObj *eventObj;
    Csitx_EventStatus status;
    uint32_t cookie, intrStatus = 0U, issueCb = UFALSE, errFlag = UFALSE, frmStatFlag = UFALSE;
    uint32_t Csitx_frameStart[CSITX_NUM_PIXEL_IF];
    uint32_t Csitx_frameEnd[CSITX_NUM_PIXEL_IF];
    uint32_t Csitx_fifoUnderflow[CSITX_NUM_PIXEL_IF];
    uint32_t Csitx_dataFlowErr[CSITX_NUM_PIXEL_IF];
    uint32_t Csitx_byteCntMismatch[CSITX_NUM_PIXEL_IF];
    uint32_t Csitx_lineNumberErr[CSITX_NUM_PIXEL_IF];

    eventObj = (CsitxDrv_EventObj *)arg;
    instObj = (CsitxDrv_InstObj *)eventObj->drvHandle;
    /* Disable HW interrupts here */
    status.eventGroup = CSITX_EVENT_GROUP_TX_IRQ;
    status.drvHandle  = eventObj->drvHandle;
    cookie = HwiP_disable();
    /* TODO: Direct register read/write as current CSL-FL does not support this */
    /* Get the interrupt status */
    if(CSITX_INSTANCE_ID_0 == instObj->drvInstId)
    {
        intrStatus = CSL_REG32_RD(
                        (CSI_TX_IF0_VBUS2APBCSI2TX_BASE +
                        CSL_CSITX_IRQ));
        /* Clear Event status */
        CSL_REG32_WR((CSI_TX_IF0_VBUS2APBCSI2TX_BASE +
                    CSL_CSITX_IRQ),
                    intrStatus);
        /* Get status for enabled interrupt only */
        intrStatus &= CSL_REG32_RD(
                        (CSI_TX_IF0_VBUS2APBCSI2TX_BASE +
                        CSL_CSITX_IRQ_MASK));
    }
#if defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)
    else if(CSITX_INSTANCE_ID_1 == instObj->drvInstId)
    {
        intrStatus = CSL_REG32_RD(
                        (CSI_TX_IF1_VBUS2APBCSI2TX_BASE +
                        CSL_CSITX_IRQ));
        /* Clear Event status */
        CSL_REG32_WR((CSI_TX_IF1_VBUS2APBCSI2TX_BASE +
                    CSL_CSITX_IRQ),
                    intrStatus);
        /* Get status for enabled interrupt only */
        intrStatus &= CSL_REG32_RD(
                        (CSI_TX_IF1_VBUS2APBCSI2TX_BASE +
                        CSL_CSITX_IRQ_MASK));
    }
#endif
    else
    {
        /* Do nothing */
    }
    status.eventMasks = intrStatus;

    /* Check the events occured */
    for(uint32_t i = 0U; i < CSITX_NUM_PIXEL_IF; i++)
    {
        Csitx_frameStart[i]      = ((intrStatus >> (CSITX_EVENT_TYPE_TX_IRQ_STRM_0_FRAME_START_SHIFT + (CSITX_EVENT_PIXEL_IF_SHIFT_WIDTH*i))) & 0x1U);
        Csitx_frameEnd[i]        = ((intrStatus >> (CSITX_EVENT_TYPE_TX_IRQ_STRM_0_FRAME_END_SHIFT + (CSITX_EVENT_PIXEL_IF_SHIFT_WIDTH*i))) & 0x1U);
        Csitx_fifoUnderflow[i]   = ((intrStatus >> (CSITX_EVENT_TYPE_TX_IRQ_STRM_0_FIFO_UNDERFLOW_SHIFT + (CSITX_EVENT_PIXEL_IF_SHIFT_WIDTH*i))) & 0x1U);
        Csitx_dataFlowErr[i]     = ((intrStatus >> (CSITX_EVENT_TYPE_TX_IRQ_STRM_0_DATA_FLOW_ERR_SHIFT + (CSITX_EVENT_PIXEL_IF_SHIFT_WIDTH*i))) & 0x1U);
        Csitx_byteCntMismatch[i] = ((intrStatus >> (CSITX_EVENT_TYPE_TX_IRQ_STRM_0_BYTE_CNT_MISMATCH_SHIFT + (CSITX_EVENT_PIXEL_IF_SHIFT_WIDTH*i))) & 0x1U);
        Csitx_lineNumberErr[i]   = ((intrStatus >> (CSITX_EVENT_TYPE_TX_IRQ_STRM_0_FIFO_UNDERFLOW_SHIFT + (CSITX_EVENT_PIXEL_IF_SHIFT_WIDTH*i))) & 0x1U);
    }

    for(uint32_t i = 0U; i < CSITX_NUM_PIXEL_IF; i++)
    {

        if(((1U == Csitx_fifoUnderflow[i]) || (1U == Csitx_dataFlowErr[i]) ||
            (1U == Csitx_byteCntMismatch[i]) || (1U == Csitx_lineNumberErr[i])) && (!errFlag))
        {
            errFlag = UTRUE;
            /* These are error interrupts, module needs soft reset in order to recover*/
            /* reset the controller stream */
            /* Reset streams */
            GT_assert(CsitxTrace,
                      (FVID2_SOK == CsitxDrv_resetModule(instObj)));
        }

        if((1U == Csitx_frameStart[i]) || (1U == Csitx_frameEnd[i]))
        {
            frmStatFlag = UTRUE;
        }

        /* update the event count in driver instance object */
        instObj->status.frameEndCount[i]         += Csitx_frameEnd[i];
        instObj->status.frameStartCount[i]       += Csitx_frameStart[i];
        instObj->status.fifoUnderflowCount[i]    += Csitx_fifoUnderflow[i];
        instObj->status.dataFlowErrCount[i]      += Csitx_dataFlowErr[i];
        instObj->status.byteCountMismatchCount[i]+= Csitx_byteCntMismatch[i];
        instObj->status.lineNumerrorCount[i]     += Csitx_lineNumberErr[i];

    }

    /* Issue callback for registered events */
    if ((eventObj->eventPrms.eventMasks & CSITX_EVENT_TYPE_TX_IRQ_ALL) || 
        ((eventObj->eventPrms.eventMasks & CSITX_EVENT_TYPE_FRAME_IRQ) && (UTRUE == frmStatFlag)) ||
        ((eventObj->eventPrms.eventMasks & CSITX_EVENT_TYPE_ERR_ALL) && (UTRUE == errFlag)))
    {
        issueCb = UTRUE;
    }

    /* Enable HW interrupts here */
    HwiP_restore(cookie);

    if ((NULL != eventObj->eventPrms.eventCb) && (UTRUE == issueCb))
    {
        /* Issue a CB to application */
        eventObj->eventPrms.eventCb(status, eventObj->eventPrms.appData);
    }
}

int32_t CsitxDrv_eventGroupUnRegister(CsitxDrv_InstObj *instObj,
                                      uint32_t eventGroup)
{
    int32_t retVal = FVID2_SOK;
    CsitxDrv_EventObj *eventObj;

    /* Error Check: Parameters */
    GT_assert(CsitxTrace, (NULL_PTR != instObj));
    if (CSITX_DRV_USAGE_STATUS_IN_USE != instObj->inUse)
    {
        retVal = FVID2_EFAIL;
    }
    if (FVID2_SOK == retVal)
    {
        /* Disable All HW events for the given group first */
        retVal = CsitxDrv_eventDisable(instObj,
                                       eventGroup,
                                       CSITX_EVENT_TYPE_TX_IRQ_ALL);
    }
    if (FVID2_SOK == retVal)
    {
        eventObj = &instObj->eventObj[eventGroup];
        /* Disable Core interrupts */
        /* Delete interrupts: TODO: 0U in following call should be core based */
        (void) Osal_DeleteInterrupt((HwiP_Handle) eventObj->hwiHandle, 0);
        eventObj = &instObj->eventObj[eventGroup];
        eventObj->coreIntrNum   = CSITX_INTR_INVALID;
        eventObj->eventInitDone = UFALSE;
        eventObj->hwiHandle     = NULL;
        instObj->eventRegDone   = 0U;
    }

    return retVal;
}

static int32_t CsitxDrv_resetModule(CsitxDrv_InstObj *instObj)
{
    int32_t retVal = FVID2_SOK;
    uint32_t regVal = 0U, currTimeout = 0U, waitFlag = UTRUE;

    /* Reset module as stream level reset is not available */
    GT_assert(CsitxTrace,
              (CDN_EOK == CSITX_SetSoftResetRequest(&instObj->cslObj.cslCfgData,
                                         BTRUE)));
    /* Wait till reset goes through */
    currTimeout = 0U;
    waitFlag = UTRUE;
    while ((UTRUE     == waitFlag) &&
           (FVID2_SOK == retVal))
    {
        GT_assert(CsitxTrace,
                 (CDN_EOK == CSITX_GetStatusRegister(&instObj->cslObj.cslCfgData,
                                          &regVal)));
        if (CSL_CSITX_STATUS_SOFT_RESET_ACTIVE_MASK ==
                (regVal & CSL_CSITX_STATUS_SOFT_RESET_ACTIVE_MASK))
        {
            waitFlag = UFALSE;
        }
        if (currTimeout > CSITX_TIMEOUT_VAL_MS)
        {
            retVal = FVID2_ETIMEOUT;
        }
        else
        {
            (void)Osal_delay(1U);
            currTimeout++;
        }
    }

    return retVal;
}
