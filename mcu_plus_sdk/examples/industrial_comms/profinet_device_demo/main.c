/*!
 *  \example main.c
 *  \ingroup Example1
 *
 *  \brief
 *  Brief description of purpose and functionality.
 *
 *  \author
 *  KUNBUS GmbH
 *
 *  \date
 *  2020-06-19
 *
 *  \copyright 
 *  Copyright (c) 2021, KUNBUS GmbH<br /><br />
 *  All rights reserved.<br />
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:<br />
 *  <ol>
 *  <li>Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.</li>
 *  <li>Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.</li>
 *  <li>Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.</li>
 *  </ol>
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/dpl/TaskP.h>

#include "ti_board_open_close.h"
#include "ti_drivers_open_close.h"

#include <osal.h>
#include <hwal.h>

#include <PN_API_PDEV.h>

#include "phy/appPhyReset.h"
#include <CUST_PHY_base.h>

#include "appStackConfigure.h"
#include "appUserInfo.h"
#include "appLed.h"

#define APP_STACK_MAIN_TASK_STACK_SIZE           4096

static  uint8_t          aTaskStackMainStack_s[APP_STACK_MAIN_TASK_STACK_SIZE] __attribute__((__aligned__ (32)));
static  TaskP_Object     mainTaskHandle_s;
static  char             aOutStream_s[0x200] = {0};

/* For TI eval boards PROFINET stack must use PRU ICSS1 instance */
static  uint8_t          pruLogicalInstance_s = CONFIG_PRU_ICSS1;

static void APP_taskStackMain (void *pTaskArg_p);
static void APP_taskStackMainFunc (void *pTaskArg_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Printf implementation.
 *
 *  \details
 *  Implementation to be done within Application and to be registered in Application to OS Abstraction Layer.
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pFormat_p       Format string.
 *  \param[in]  ...             Parameter list.
 *  \return     Return code
 *
 *  <!-- Example: -->
 *
 *  \par Example
 *  \code{.c}
 *  
 *  // required variables
 *  int32_t retVal = 0;
 *  va_list vaArg;
 *
 *  retVal = APP_printf(NULL, "Hello %s %d", vaArg);
 *  \endcode
 *
 *  <!-- References: -->
 *
 *  \sa OSAL_vprintf
 *
 *
 */

/* Referenced from EtherCAT ServiceApps/common/os/freertos/ESL_OS_os.c */

void APP_printf(void* pContext_p, const char* __restrict pFormat_p, va_list arg_p)
{
    /* @cppcheck_justify{unusedVariable} false-positive: variable is used */
    //cppcheck-suppress unusedVariable
    int32_t             transferOK;
    /* @cppcheck_justify{unusedVariable} false-positive: variable is used */
    //cppcheck-suppress unusedVariable
    UART_Transaction    transaction;
    
    OSALUNREF_PARM(pContext_p);
    
    UART_Transaction_init(&transaction);
    
    memset(aOutStream_s, 0, sizeof(aOutStream_s));
    (void)vsnprintf(aOutStream_s, sizeof(aOutStream_s), pFormat_p, arg_p);
    
    transaction.count   = strlen(aOutStream_s);
    transaction.buf     = (void *)aOutStream_s;
    transaction.args    = NULL;
    transferOK = UART_write(gUartHandle[CONFIG_UART_CONSOLE], &transaction);
    
    (void)transferOK;
}

/*!
*  <!-- Description: -->
* \brief
*  Main entry point.
*
* \details
* In the main function, we configure SoC hardware and specific OSAL and HWAL modules, which are essential for PROFINET communication.
* Afterwards, we create and start the main application task, which is responsible for configuring and running PROFINET stack.
* We expect that main will never return, in case everything went well.
*
* <!-- Parameters and return values: -->
*
*  \param[in]  argc                            Number of command line arguments.
*  \param[in]  *argv                           Pointer to an array of command line arguments.
*
*  \return Error code of int type.
*
*  \retval    0                                Success.
*  \retval    -1                               Failure during initialization.
*
*/
int main (
    int argc,
    char* argv[])
{
    TaskP_Params    mainTaskParam   =   {0};

    int32_t         systemRetVal    =    SystemP_SUCCESS;
    uint32_t        osalRetVal      =    OSAL_eERR_NOERROR;

    /* Initialize SoC specific modules. */
    System_init ();
    Board_init ();

    /*
     * Both HWAL and OSAL use OSAL error handler for error reporting.
     * Therefore error handler should be registered prior the initialization.
     * 
     * OSAL_init() must be called PRIOR to HWAL_init()
     */
    OSAL_registerErrorHandler (APP_UI_cbErrorHandler);
    
    osalRetVal = OSAL_init();
    if (OSAL_eERR_NOERROR != osalRetVal)
    {
        goto laError;
    }

    osalRetVal = HWAL_init ();
    if (OSAL_eERR_NOERROR != osalRetVal)
    {
        goto laError;
    }

    OSAL_registerPrintOut(NULL, APP_printf);

    /* Create main application task. */
    TaskP_Params_init (&mainTaskParam);
    mainTaskParam.name        = "stack_main";                     /* Name of the task.  For debugging only. */
    mainTaskParam.stackSize   = sizeof(aTaskStackMainStack_s);    /* Task stack size in bytes. */
    mainTaskParam.stack       = (uint8_t*)aTaskStackMainStack_s;  /* Pointer to the task stack base. */
    mainTaskParam.priority    = OSAL_TASK_ePRIO_Normal;           /* Task priority. */
    mainTaskParam.taskMain    = (TaskP_FxnMain)APP_taskStackMain; /* Pointer to the companion function of the task. */
    mainTaskParam.args        = NULL;                             /* We do not pass any arguments to the task. */

    systemRetVal = TaskP_construct(&mainTaskHandle_s, &mainTaskParam);
    if (SystemP_SUCCESS != systemRetVal)
    {
        OSAL_printf("[APP] ERROR: Failed to create task %s (%ld)\r\n", mainTaskParam.name, systemRetVal);
        goto laError;
    }

    /* Start the scheduler. Scheduler will start the main task. */
    OSAL_startOs ();

    /* The following line should never be reached. OSAL_startOs()
    will only return in case of failure. */
    return (0);

/* We get here only in case of initialization errors. */
laError:
    return (-1);
}

/*!
*  <!-- Description: -->
* \brief
* Main application task.
*
* \details
* Used only to invoke the main PROFINET stack function, which should never return.
* We expect that the stack function will never return.
* It will return in case of PROFINET stack errors only.
*
* <!-- Parameters and return values: -->
*
*  \param[in]  pTaskArg_p                      Arguments passed to the task.
*
*  \return
*
*/
void APP_taskStackMain (
    void *pTaskArg_p)
{
    APP_taskStackMainFunc (pTaskArg_p);

    /*Get here only if stack has crashed.*/
    TaskP_destruct(&mainTaskHandle_s);
}

/*!
*  <!-- Description: -->
* \brief
* Main PROFINET stack function.
*
* \details
* Start all relevant SoC and board peripherals and configure the PROFINET stack.
* On successful start, execute the stack run function (APP_runProfinetStack) periodically in infinite loop.
*
* The stack run function can be used as "application" from the stack point of view.
*
* \remarks
* Relevant peripherals:
* - PRU and PHY for communication;
* - OSPI and flash driver for storing permanent data on the on-board OSPI Flash;
* - UART for terminal connection;
*
* <!-- Parameters and return values: -->
*
*  \param[in]  pTaskArg_p                      Arguments passed to the function.
*
*  \return
*
*/
void APP_taskStackMainFunc (
    void *pTaskArg_p)
{
    uint32_t systemRetVal = SystemP_FAILURE;
    uint32_t osalRetVal = OSAL_eERR_NOERROR;
    bool ledTaskRetVal = true;

    /* Open SoC and board drivers. */
    Drivers_open();
    systemRetVal = Board_driversOpen();
    DebugP_assert(systemRetVal==SystemP_SUCCESS);

    /* Init board-specific PHY reset driver. */
    APP_PHY_RESET_open();
    APP_PHY_RESET_all();
    CUST_PHY_CBregisterLibDetect(CUST_PHY_detect, NULL);

    /*Configure and start PROFINET stack.*/
    OSAL_printf ("\r[APP] INFO: Configuring and starting PROFINET stack...\n");
    osalRetVal = APP_startProfinetStack (pruLogicalInstance_s);
    if (OSAL_eERR_NOERROR != osalRetVal)
    {
        OSAL_printf ("\r[APP] ERROR: Failed to configure the PROFINET stack. Error code: 0x%8.8x\n", osalRetVal);
        return;
    }

    /* Start LED task after the stack has been configured */
    ledTaskRetVal = APP_startLedTask();
    if (false == ledTaskRetVal)
    {
        OSAL_printf ("\r[APP] ERROR: Failed to start LED task.\n");
        return;
    }

    print_dumpVersions();

    OSAL_printf("\r[APP] INFO: Stack configured successfully! Stack runs.\n");
    while (1)
    {
        osalRetVal = APP_runProfinetStack ();
        if (OSAL_eERR_NOERROR != osalRetVal)
        {
            OSAL_printf ("\r[APP] ERROR: Error during stack execution. Error code:  0x%8.8x\n", osalRetVal);
            return;
        }

        OSAL_SCHED_sleep (1);
    }
}
