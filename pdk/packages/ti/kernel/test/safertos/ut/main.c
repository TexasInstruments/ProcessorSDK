/*
 *  Copyright ( C ) 2022 Texas Instruments Incorporated
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
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <string.h>
#include <ti/osal/DebugP.h>
#include <ti/board/board.h>
#include "SafeRTOS_API.h"

#if defined (BUILD_C66X)
#include "aborts.h"

#include "ti/drv/sciclient/sciclient.h"
#include "ti/csl/src/intc/csl_intc.h"
#endif

#if defined (BUILD_C7X)
#include "Hwi.h"
#endif

#define MAIN_TASK_PRI  ( 4U )

#if !defined (BUILD_C7X)
#define MAIN_TASK_SIZE ( 4096U )
#else
#define MAIN_TASK_SIZE ( 32U * 1024U)
#endif

/* For SafeRTOS on R5F with FFI Support, task stack should be aligned to the stack size */
#if defined (BUILD_MCU)
static portInt8Type  gMainTaskStack[MAIN_TASK_SIZE] __attribute__( ( aligned( MAIN_TASK_SIZE ) ) );
#else
static portInt8Type  gMainTaskStack[MAIN_TASK_SIZE] __attribute__( ( aligned(0x2000) ) );
#endif
/* Declare task TCB:
 * Due to the use of the MPU background region, by default, all RAM can only
 * be accessed in privileged mode unless a specific MPU region has been setup
 * allowing unprivileged access. */
static xTCB xMainTaskTCB = { 0 };

/* The call to xTaskInitializeScheduler is included within a wrapper
 * initialisation function. */
portBaseType xInitializeScheduler( void );

void test_safertos_main( void *args );

#if defined (BUILD_MCU) || defined (BUILD_C7X)
portBaseType prvSetupHardware( void )
{
    return pdPASS;
}
#endif

#if defined (BUILD_C66X)
extern void vPortSetInterruptVectors( void );
/*
 * These combinations of timers and interrupts will only
 * work if this application is running on a single core.
 * To run applications on both c66x cores ensure the
 * timer/irq combinations are all different.
 */
portBaseType prvC66xTickInterruptConfig( void )
{
    struct tisci_msg_rm_irq_set_req     rmIrqReq;
    struct tisci_msg_rm_irq_set_resp    rmIrqResp;

    rmIrqReq.valid_params           = TISCI_MSG_VALUE_RM_DST_ID_VALID |
                                      TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
    rmIrqReq.src_index              = 0U;
#if defined (BUILD_C66X_1)
    rmIrqReq.src_id                 = TISCI_DEV_TIMER0;
    rmIrqReq.dst_id                 = TISCI_DEV_C66SS0_CORE0;
    rmIrqReq.dst_host_irq           = 21U;
#endif
#if defined (BUILD_C66X_2)
    rmIrqReq.src_id                 = TISCI_DEV_TIMER1;
    rmIrqReq.dst_id                 = TISCI_DEV_C66SS1_CORE0;
    rmIrqReq.dst_host_irq           = 20U;
#endif
    /* Unused params */
    rmIrqReq.global_event           = 0U;
    rmIrqReq.ia_id                  = 0U;
    rmIrqReq.vint                   = 0U;
    rmIrqReq.vint_status_bit_index  = 0U;
    rmIrqReq.secondary_host         = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

    return ( ( Sciclient_rmIrqSet(&rmIrqReq, &rmIrqResp, SCICLIENT_SERVICE_WAIT_FOREVER) == CSL_PASS )? pdPASS : pdFAIL );
}
/*-------------------------------------------------------------------------*/
/* Hardware setup using the TI PDK libraries. */
portBaseType prvSetupHardware( void )
{
    portBaseType xStatus = pdPASS;
    int32_t ret;
    Sciclient_ConfigPrms_t config;

    Sciclient_configPrmsInit(&config);

    ret = Sciclient_init(&config);
    if( ret == 0 )
    {
        xStatus = pdFAIL;
    }

    xStatus = prvC66xTickInterruptConfig();

    vPortSetInterruptVectors();

    return xStatus;
}
/*-------------------------------------------------------------------------*/
#endif

int main( void )
{
    portBaseType xStatus;

#if !defined(BUILD_MCU1_0)
    Board_initCfg boardCfg;
    Board_STATUS  status;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;

    status = Board_init(boardCfg);

    DebugP_assert(status == BOARD_SOK);
#endif

    /* Setup Board Hardware. */
    xStatus = prvSetupHardware();
    DebugP_assert(xStatus != pdFAIL);

    /* Initialise the Kernel Scheduler. */
    xStatus = xInitializeScheduler();

    /* Everything OK? */
    if( pdPASS == xStatus )
    {
        /* The structure passed to xTaskCreate() to create the check task. */
        static const xTaskParameters xMainTaskParams =
        {
            &test_safertos_main,                  /* The function that implements the task being created. */
            "SafeRTOS_main",                    /* The name of the task being created. The kernel does not use this itself, its just to assist debugging. */
            &xMainTaskTCB,                      /* TCB for the check task. */
            gMainTaskStack,                     /* The buffer allocated for use as the task stack. */
            MAIN_TASK_SIZE,                     /* The size of the buffer allocated for use as the task stack - note this is in BYTES! */
            NULL,                               /* The task parameter, not used in this case. */
            MAIN_TASK_PRI,                      /* The priority to assigned to the task being created. */
            NULL,                               /* Thread Local Storage not used. */
#if defined (BUILD_MCU)
            pdFALSE,                            /* Check task does not use the FPU. */
            {                                   /* MPU task parameters. */
                mpuPRIVILEGED_TASK,             /* Check task is privileged. */
                {
                    { NULL, 0U, 0U, 0U },       /* No additional region definitions are required. */
                    { NULL, 0U, 0U, 0U },
                }
            }
#endif

#if defined (BUILD_C66X)
            safertosapiPRIVILEGED_TASK      /* Check task is privileged. */
#endif
        };

        /* Create the main task. */
        xStatus = xTaskCreate( &xMainTaskParams,       /* The structure containing the task parameters created at the start of this function. */
                                    NULL );            /* This parameter can be used to receive a handle to the created task, but is not used in this case. */
    }
    /* Everything OK? */
    if( pdPASS == xStatus )
    {
#if defined (BUILD_C7X)
        Hwi_switchFromBootStack();
#endif
        /* Yes, try to start the Scheduler. */
        xStatus = xTaskStartScheduler();
    }

    /* The Scheduler should now be running the Demonstration tasks.
     * So this line should never be reached, if it is,
     * then the Scheduler initialisation has failed! */
    DebugP_assert( xStatus );

    return 0;
}

