/*
 *  Copyright (C) 2018-2021 Texas Instruments Incorporated
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

#include <stdlib.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/TaskP.h>
#include <ti/board/board.h>
#include "FreeRTOS.h"
#include "task.h"

#define MAIN_TASK_PRI  (configMAX_PRIORITIES-1)

#define MAIN_TASK_SIZE (4096u)
StackType_t gMainTaskStack[MAIN_TASK_SIZE] __attribute__((aligned(32)));

TaskP_Handle gMainTask;

void task_switch_main(void *arg0, void* arg1);
void c66xIntrConfig(void);

int32_t main()
{
    TaskP_Params      taskParams;
#if !defined(BUILD_MCU1_0)
    Board_STATUS  status;
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;

    status = Board_init(boardCfg);

    DebugP_assert(BOARD_SOK == status);
#endif

    OS_init();

    c66xIntrConfig();

    TaskP_Params_init(&taskParams);
    taskParams.name = "freertos_main";
    taskParams.stacksize = sizeof(gMainTaskStack);
    taskParams.stack = gMainTaskStack;
    taskParams.priority = MAIN_TASK_PRI;
    taskParams.arg0 = NULL;
    taskParams.arg1 = NULL;

	/* This task is created at highest priority, it should create more tasks and then delete itself */
    gMainTask = TaskP_create(&task_switch_main, &taskParams);
    configASSERT(NULL != gMainTask);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    /* The following line should never be reached because vTaskStartScheduler()
       will only return if there was not enough FreeRTOS heap memory available to
       create the Idle and (if configured) Timer tasks.  Heap management, and
       techniques for trapping heap exhaustion, are described in the book text. */
    DebugP_assert(BFALSE);

    return 0;
}

void c66xIntrConfig(void)
{
#if defined (_TMS320C6X) && defined (SOC_J721E)
    /* On J721E C66x builds we define timer tick in the configuration file to
     * trigger event #21 for C66x_1(from DMTimer0) and #20 for C66x_2(from DMTimer1). 
     * Map DMTimer interrupts to these events through DMSC RM API.
     */
    #include <ti/drv/sciclient/sciclient.h>

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

    Sciclient_rmIrqSet(&rmIrqReq, &rmIrqResp, SCICLIENT_SERVICE_WAIT_FOREVER);
#endif

    return;
}

