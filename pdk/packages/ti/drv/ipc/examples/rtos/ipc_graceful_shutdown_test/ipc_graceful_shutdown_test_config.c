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
 *  \file ipc_graceful_shutdown_test_config.c
 *
 *  \brief IPC graceful shutdown test configurations.
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

/* Number of processors under test */
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
 * IPC stack buffers
 */
uint8_t  gIpcApp_TaskStackBuf[(IPC_APP_NUM_CORE_IN_TEST+2)*IPC_TASK_STACKSIZE];
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
    IPC_MCU2_0, IPC_MCU2_1
};
#endif

#ifdef BUILD_MCU2_0
uint32_t gIpcApp_SelfProcId = IPC_MCU2_0;
uint32_t gIpcApp_RemoteProc[] =
{
    IPC_MCU1_0, IPC_MCU2_1
};
#endif

#ifdef BUILD_MCU2_1
uint32_t gIpcApp_SelfProcId = IPC_MCU2_1;
uint32_t gIpcApp_RemoteProc[] =
{
    IPC_MCU1_0, IPC_MCU2_0
};
#endif

uint32_t *gIpcApp_RemoteProcArray = gIpcApp_RemoteProc;
uint32_t  gIpcApp_NumRemoteProc = sizeof(gIpcApp_RemoteProc)/sizeof(gIpcApp_RemoteProc[0]);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* None */
