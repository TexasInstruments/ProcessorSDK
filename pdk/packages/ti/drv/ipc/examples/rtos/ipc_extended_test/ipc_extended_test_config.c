/*
 *  Copyright (c) Texas Instruments Incorporated 2024
 *  All rights reserved.
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
 *  \file ipc_extended_test_config.c
 *
 *  \brief IPC extended test configurations.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/drv/ipc/ipc.h>
#include <ti/drv/ipc/examples/common/src/ipc_setup.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Number of processors under test; dummy for this app */
#define IPC_APP_NUM_CORE_IN_TEST            3

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

/*
 * In the cfg file of R5F, default heap is 48K which is not
 * enough for 9 task_stack, so creating task_stack on global.
 * C7x cfg has 256k default heap, so no need to put task_stack on global
 */
#if !defined(BUILD_C7X)

uint8_t  gIpcApp_TaskStackBuf[(IPC_APP_NUM_CORE_IN_TEST+2)*IPC_TASK_STACKSIZE];

#else

/* IMPORTANT NOTE: For C7x,
 * - stack size and stack ptr MUST be 8KB aligned
 * - AND min stack size MUST be 16KB
 * - AND stack assigned for task context is "size - 8KB"
*       - 8KB chunk for the stack area is used for interrupt handling in this task context
*/
uint8_t gIpcApp_TaskStackBuf[(IPC_APP_NUM_CORE_IN_TEST+2)*IPC_TASK_STACKSIZE]
__attribute__ ((section(".bss:taskStackSection")))
__attribute__ ((aligned(8192)));
#endif

uint8_t  gIpcApp_CntrlBuf[RPMSG_DATA_SIZE] __attribute__ ((section("ipc_data_buffer"), aligned (8)));
uint8_t  gIpcApp_SysVqBuf[VQ_BUF_SIZE]  __attribute__ ((section ("ipc_data_buffer"), aligned (8)));
uint8_t  gIpcApp_SendBuf[RPMSG_DATA_SIZE * IPC_APP_NUM_CORE_IN_TEST]  __attribute__ ((section ("ipc_data_buffer"), aligned (8)));
uint8_t  gIpcApp_RspBuf[RPMSG_DATA_SIZE]  __attribute__ ((section ("ipc_data_buffer"), aligned (8)));
uint8_t  gIpcApp_TimeoutBuf[RPMSG_DATA_SIZE] __attribute__ ((section ("ipc_data_buffer"), aligned (8)));

uint8_t *gIpcApp_CntrlBufPtr     = gIpcApp_CntrlBuf;
uint8_t *gIpcApp_TaskStackBufPtr = gIpcApp_TaskStackBuf;
uint8_t *gIpcApp_SendBufPtr      = gIpcApp_SendBuf;
uint8_t *gIpcApp_RspBufPtr       = gIpcApp_RspBuf;
uint8_t *gIpcApp_TimeoutBufPtr   = gIpcApp_TimeoutBuf;
uint8_t *gIpcApp_SysVqBufPtr     = gIpcApp_SysVqBuf;

#ifdef BUILD_MCU1_0
uint32_t gIpcApp_SelfProcId = IPC_MCU1_0;
uint32_t gIpcApp_RemoteProc[] =
{
    IPC_MCU2_0
};
#endif

#ifdef BUILD_C7X_1
uint32_t gIpcApp_SelfProcId = IPC_C7X_1;
uint32_t gIpcApp_RemoteProc[] =
{
    IPC_MCU2_0
};
#endif

uint32_t *gIpcApp_RemoteProcArray = gIpcApp_RemoteProc;
uint32_t  gIpcApp_NumRemoteProc = sizeof(gIpcApp_RemoteProc)/sizeof(gIpcApp_RemoteProc[0]);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* None */
