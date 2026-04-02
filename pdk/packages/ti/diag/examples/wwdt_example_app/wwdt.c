/*
 * WWDT Functions
 *
 * WWDT Functionality for the Windowed Watchdog Timer (WWDT) Example
 * Application
 *
 *  Copyright (c) 2020 Texas Instruments Incorporated
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
 *  \file wwdt.c
 *
 *  \brief This file contains functions that provide WWDT setup, servicing,
 *          and WWDT utility functions for the Windowed Watchdog Timer (WWDT)
 *          application.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* For interrupt configuration */

#include "wwdt.h"

#include <ti/csl/hw_types.h>
#include <ti/csl/csl_rti.h>
#include <ti/csl/csl_esm.h>

#include <sdr_common.h>
#include <sdr_wdt.h>
#include <sdr_esm.h>

/* Diagnostic example utility functions, e.g. print outputs */
#include <diag_utils.h>

#include <ti/csl/soc.h>

/* For interrupt ID numbers */
#include <ti/csl/soc/cslr_soc_intr.h>

/* For Mailbox functions */
#include <ti/csl/csl_mailbox.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define MAX_WWDT_INSTANCE      (SDR_ESM_INSTANCE_MAIN)
#define MAX_WWDT_INT_TYPE      (SDR_ESM_INT_TYPE_CFG)

#define RTI_CLOCK_SOURCE_12MHZ (2U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

WWDT_example_ESM_log_entry_t wwdtEventLog[MAX_WWDT_EVENTS_LOGGED];
uint32_t totalEventsLogged = 0;
uint32_t wwdtTotalEventsLogged = 0U;

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


/*********************************************************************
* @fn      WWDT_Example_WwdtInitHandlerInit
*
* @brief   Clear all interrupts, register callbacks, re-enable interrupts
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t WWDT_Example_WwdtInitHandlerInit(void)
{
    int32_t result = 0;
    OsalRegisterIntrParams_t intrPrms;
    OsalInterruptRetCode_e osalRetVal;
    HwiP_Handle hwiHandle;
    uint32_t rtiIntNum;
    uint32_t intrStatus;

    /* Add RTI handling through direct CPU interrupt */
    Osal_RegisterInterrupt_initParams(&intrPrms);
    intrPrms.corepacConfig.arg          = (uintptr_t)0;
    intrPrms.corepacConfig.priority     = 1U;
    intrPrms.corepacConfig.corepacEventNum = 0U;

    rtiIntNum = CSLR_R5FSS0_CORE0_INTR_RTI28_INTR_WWD_0;

    intrPrms.corepacConfig.isrRoutine = &SDR_WWDT_applicationCallbackFunction_violation;
    intrPrms.corepacConfig.intVecNum  = rtiIntNum;

    osalRetVal = Osal_RegisterInterrupt(&intrPrms, &hwiHandle);

    if (OSAL_INT_SUCCESS != osalRetVal)
    {
        result = -1;
    }

    /* Clear any pending interrupt */
    RTIDwwdGetStatus(CSL_RTI28_CFG_BASE, &intrStatus);
    RTIDwwdClearStatus(CSL_RTI28_CFG_BASE, intrStatus);

    /* Select RTI module clock source */
    DIAG_printf("WWDT Example: Write %x at CLKSEL Addr 0x%x \n",
        RTI_CLOCK_SOURCE_12MHZ,
        CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_WWD28_CLKSEL);
    HW_WR_FIELD32(
        CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_WWD28_CLKSEL,
        CSL_MAIN_CTRL_MMR_CFG0_WWD28_CLKSEL_CLK_SEL,
        RTI_CLOCK_SOURCE_12MHZ);

    return result;
}

/*********************************************************************
* @fn      SDR_WWDT_applicationCallbackFunction_violation
*
* @brief   Failure interrupt handler for WWDT
*
* @param   Handle corresponding to this interrupt (not used, required
*          for callback signature)
*
* @return    None
*/
void SDR_WWDT_applicationCallbackFunction_violation(uintptr_t handle)
{
    wwdtTotalEventsLogged++;
}

/*********************************************************************
*
* @brief   WDT call back function registered with ESM handler
*
* @return  None
*/
void SDR_ESM_applicationCallbackFunction(SDR_ESM_InstanceType esmInstType,
                                         SDR_ESM_IntType esmIntType,
                                         uint32_t grpChannel,
                                         uint32_t index,
                                         uint32_t intSrc)
{
    rtiDwwdStatus_t intrStatus;

    /* Log the event */
    RTIDwwdGetStatus(CSL_RTI28_CFG_BASE, &intrStatus);
    if ((intrStatus & RTI_DWWD_STATUS_KEY_SEQ_VIOLATION) != 0U)
    {
        wwdtEventLog[totalEventsLogged].keyViolation = 1U;
    }
    else
    {
        wwdtEventLog[totalEventsLogged].keyViolation = 0U;
    }
    if ((intrStatus & RTI_DWWD_STATUS_TIME_WINDOW_VIOLATION) != 0U)
    {
        wwdtEventLog[totalEventsLogged].windowViolation = 1U;
    }
    else
    {
        wwdtEventLog[totalEventsLogged].windowViolation = 0U;
    }
    if ((intrStatus & RTI_DWWD_STATUS_ENDTIME_WINDOW_VIOLATION) != 0U)
    {
        wwdtEventLog[totalEventsLogged].endViolation = 1U;
    }
    else
    {
        wwdtEventLog[totalEventsLogged].endViolation = 0U;
    }
    if ((intrStatus & RTI_DWWD_STATUS_STARTTIME_WINDOW_VIOLATION) != 0U)
    {
        wwdtEventLog[totalEventsLogged].startViolation = 1U;
    }
    else
    {
        wwdtEventLog[totalEventsLogged].startViolation = 0U;
    }

    totalEventsLogged++;

    RTIDwwdClearStatus(CSL_RTI28_CFG_BASE, intrStatus);

    /* Signal callback completion */
    MailboxSendMessage(MAILBOX_BASE_ADDRESS, MAILBOX_QUEUE_0, 1U);
}

/*********************************************************************
* @fn      GetWwdtEventLog
*
* @brief   Getter for event log entry
*
* @param   The id of the desired event
*
* @return    Event log entry corresponding to id
*/
WWDT_example_ESM_log_entry_t GetWwdtEventLog(uint32_t id)
{
    return wwdtEventLog[id];
}

/*********************************************************************
* @fn      GetTotalEventsLogged
*
* @brief   Getter for number of events logged
*
* @param   None
*
* @return    Number of events logged
*/
uint32_t GetTotalEventsLogged(void)
{
    return totalEventsLogged;
}

/*********************************************************************
*
* @brief   SDR assert function
*
* @return  None
*/
void SDR_assertExternalFunction(SDR_assertErrorNumber errorNumber)
{

    DIAG_printf("\n  Assert called with error Number %d \n",
                errorNumber);
    /* Asserts are fatal: Currently just wait for ever */
    while(1);
}
