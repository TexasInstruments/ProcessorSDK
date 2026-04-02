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

/*!
 * \file  main.c
 *
 * \brief Main file for testing lwip porting layer
 */

/* OSAL header files */
#include <ti/osal/TaskP.h>
#include <ti/osal/osal.h>

/* lwip includes */
#include <lwip/sys.h>
#include "lwip/memp.h"

#include <arch/sys_arch.h>
#include <ti/board/board.h>
#include <ti/osal/DebugP.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define MAIN_TASK_PRIORITY 1
/* Test application stack */
#define MAIN_TASK_STACK_SZ                     (4U * 1024U)


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if LWIP_TCPIP_CORE_LOCKING
/** The global semaphore to lock the stack. */
sys_mutex_t lock_tcpip_core;
#endif /* LWIP_TCPIP_CORE_LOCKING */


/* ========================================================================== */
/*                          Function Declaration                              */
/* ========================================================================== */

/* Task function */
void sys_arch_test_main( void *arg);


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main()
{
    Board_initCfg boardCfg;
    Board_STATUS  status;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;

    status = Board_init(boardCfg);
    DebugP_assert(status == BOARD_SOK);

    OS_init();
    sys_init();

#if LWIP_CHECK_CORE_LOCKING
#if LWIP_TCPIP_CORE_LOCKING
{
  err_t mutexStatus;
/* type of lock_tcpip_core is sys_mutex_t */
  mutexStatus = sys_mutex_new(&lock_tcpip_core);
  LWIP_ASSERT("failed to create mutex",mutexStatus==ERR_OK);
}
#endif
#endif

    memp_init();
    sys_thread_t task = sys_thread_new((const char *)"UT Task", sys_arch_test_main, NULL, MAIN_TASK_STACK_SZ, MAIN_TASK_PRIORITY);
    if (task.thread_handle==NULL)
    {
        OS_stop();
    }
    OS_start();
}