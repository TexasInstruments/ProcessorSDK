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
#ifndef INCLUDE_WWDT_H_
#define INCLUDE_WWDT_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>

/* For Timer functions */
#include <ti/osal/osal.h>

/* For Mailbox functions */
#include <ti/csl/csl_mailbox.h>
#include <ti/csl/soc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define MAX_WWDT_EVENTS_LOGGED (20)

#define MAILBOX_BASE_ADDRESS    CSL_NAVSS0_MAILBOX_REGS0_BASE

/* WWDT event log entry */
typedef struct{
    /* Flag indicating whether this event was a key violation, i.e. incorrect
       key was used to service the watchdog - NOT supported on J721E */
    uint32_t             keyViolation;
    /* Flag indicating whether this event was a window violation i.e. either
       an end or start violation */
    uint32_t             windowViolation;
    /* Flag indicating whether this event was a end violation, i.e. if the
       watchdog was not serviced by the end of the window */
    uint32_t             endViolation;
    /* Flag indicating whether this event was a start violation i.e. if the
       watchdog was serviced before the start of the window */
    uint32_t             startViolation;
} WWDT_example_ESM_log_entry_t;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/*===========================================================================*/
/*                         Internal function declarations                    */
/*===========================================================================*/

/*********************************************************************
* @fn      WWDT_Example_WwdtInitHandlerInit
*
* @brief   Clear all interrupts, register callbacks, re-enable interrupts
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t WWDT_Example_WwdtInitHandlerInit(void);

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
void SDR_WWDT_applicationCallbackFunction_violation(uintptr_t handle);

/*********************************************************************
* @fn      GetWwdtEventLog
*
* @brief   Getter for event log entry
*
* @param   The id of the desired event
*
* @return    Event log entry corresponding to id
*/
WWDT_example_ESM_log_entry_t GetWwdtEventLog(uint32_t id);

/*********************************************************************
* @fn      GetTotalEventsLogged
*
* @brief   Getter for number of events logged
*
* @param   None
*
* @return    Number of events logged
*/
uint32_t GetTotalEventsLogged(void);

#endif /*  INCLUDE_WWDT_H_ */
