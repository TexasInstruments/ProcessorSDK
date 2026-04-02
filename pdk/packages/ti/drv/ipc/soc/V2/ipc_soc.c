/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 *  \file ipc_soc.c
 *
 *  \brief File containing the IPC driver - soc specific implementation.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/drv/ipc/ipc.h>
#include <ti/drv/ipc/soc/ipc_soc.h>
#include <ti/drv/ipc/src/ipc_priv.h>
#include <ti/drv/ipc/src/ipc_mailbox_lld.h>
#include <ti/drv/mailbox/mailbox.h>

/**
 *  \brief Processor IDs to name mapping for all processor in AM64x
 */
static Ipc_ProcInfo g_Ipc_mp_procInfo[IPC_MAX_PROCS] =
{
    {IPC_MPU1_0,      "mpu1_0"},      /**< ARM A53 - VM0 */
    {IPC_MCU1_0,      "mcu1_0"},      /**< ARM Main R5F0 - core0 */
    {IPC_MCU1_1,      "mcu1_1"},      /**< ARM Main R5F0 - core1 */
    {IPC_MCU2_0,      "mcu2_0"},      /**< ARM Main R5F1 - core0 */
    {IPC_MCU2_1,      "mcu2_1"},      /**< ARM Main R5F1 - core1 */
    {IPC_M4F_0,       "m4f_0"}        /**< ARM M4F */
};

const char* Ipc_getCoreName(uint32_t procId)
{
    char*     p = (char*)0;
    uint32_t id = procId;

    if(id < IPC_MAX_PROCS)
    {
       p = g_Ipc_mp_procInfo[id].name;
    }
    return p;
}

uint32_t Ipc_rprocIdToMboxId(uint32_t id)
{
    uint32_t mbId = MAILBOX_INST_INVALID;

    if (id == IPC_MPU1_0)
    {
        mbId = MAILBOX_INST_MPU1_0;
    }
    else if (id == IPC_MCU1_0)
    {
        mbId = MAILBOX_INST_MCU1_0;
    }
    else if (id == IPC_MCU1_1)
    {
        mbId = MAILBOX_INST_MCU1_1;
    }
    else if (id == IPC_MCU2_0)
    {
        mbId = MAILBOX_INST_MCU2_0;
    }
    else if (id == IPC_MCU2_1)
    {
        mbId = MAILBOX_INST_MCU2_1;
    }
    else if (id == IPC_M4F_0)
    {
        mbId = MAILBOX_INST_M4F_0;
    }

    return mbId;
}

uint32_t Ipc_mboxIdToRprocId(uint32_t id)
{
    uint32_t procId = IPC_INVALID_PROCID;

    if (id == MAILBOX_INST_MPU1_0)
    {
        procId = IPC_MPU1_0;
    }
    else if (id == MAILBOX_INST_MCU1_0)
    {
        procId = IPC_MCU1_0;
    }
    else if (id == MAILBOX_INST_MCU1_1)
    {
        procId = IPC_MCU1_1;
    }
    else if (id == MAILBOX_INST_MCU2_0)
    {
        procId = IPC_MCU2_0;
    }
    else if (id == MAILBOX_INST_MCU2_1)
    {
        procId = IPC_MCU2_1;
    }
    else if (id == MAILBOX_INST_M4F_0)
    {
        procId = IPC_M4F_0;
    }

    return procId;
}

uint32_t Ipc_getCoreId(void)
{
    uint32_t selfId =  IPC_INVALID_PROCID;

#if defined(BUILD_MPU1_0)
    selfId = IPC_MPU1_0;
#elif defined(BUILD_MCU1_0)
    selfId = IPC_MCU1_0;
#elif defined(BUILD_MCU1_1)
    selfId = IPC_MCU1_1;
#elif defined(BUILD_MCU2_0)
    selfId = IPC_MCU2_0;
#elif defined(BUILD_MCU2_1)
    selfId = IPC_MCU2_1;
#elif defined(BUILD_M4F)
    selfId = IPC_M4F_0;
#else
#error "Unsupported core Id"
#endif

    return (selfId);
}

uint32_t Ipc_isCacheCoherent(void)
{
    uint32_t isCacheCoherent;

#if defined (BUILD_MPU1_0)
    isCacheCoherent = TRUE;
#else
    isCacheCoherent = FALSE;
#endif

    return (isCacheCoherent);
}
