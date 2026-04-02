/*
 * Copyright (c) 2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  \file ipc_setup.h
 *
 *  \brief Define the macros and functions for common IPC test
 *
 */

#ifndef IPC_SETUP_H_
#define IPC_SETUP_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SYSBIOS
  /* To print to SysMin Buffer(view from CCS ROV) */
  #include <xdc/runtime/System.h>
  #define App_printf System_printf
#else
  #include "ipc_trace.h"
  #define App_printf  Ipc_Trace_printf
#endif

/* this should be >= RPMessage_getObjMemRequired() */
#define IPC_RPMESSAGE_OBJ_SIZE  256U

/* this should be >= RPMessage_getMessageBufferSize() */
#define IPC_RPMESSAGE_MSG_BUFFER_SIZE  (496U + 32U)

#define RPMSG_DATA_SIZE         (256U*IPC_RPMESSAGE_MSG_BUFFER_SIZE + IPC_RPMESSAGE_OBJ_SIZE)
#define VQ_BUF_SIZE             2048U

/* Vring start address for each device */
#ifdef SOC_AM65XX
#define VRING_BASE_ADDRESS      0xA2000000U
#elif defined (SOC_J7200)
#define VRING_BASE_ADDRESS      0xA4000000U
#elif defined (SOC_AM64X)
#define VRING_BASE_ADDRESS      0xA5000000U
#elif defined (SOC_J721S2)
#define VRING_BASE_ADDRESS      0xA8000000U
#elif defined (SOC_J784S4) || defined (SOC_J742S2)
#define VRING_BASE_ADDRESS      0xAC000000U
#else
#define VRING_BASE_ADDRESS      0xAA000000U
#endif

int32_t Ipc_echo_test(void);

#ifdef __cplusplus
}
#endif

#endif /* IPC_SETUP_H_ */
