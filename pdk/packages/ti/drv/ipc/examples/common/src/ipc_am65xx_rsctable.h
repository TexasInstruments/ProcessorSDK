/*
 * Copyright (c) 2017-2020, Texas Instruments Incorporated
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
 *  \file ipc_am65xx_rsctable.h
 *
 *  \brief Define the resource table entries for all R5F cores. This will be
 *  incorporated into corresponding base images, and used by the remoteproc
 *  on the host-side to allocated/reserve resources.
 *
 */

#ifndef IPC_AM65XX_RSCTABLE_H_
#define IPC_AM65XX_RSCTABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/drv/ipc/include/ipc_rsctypes.h>
#ifdef SYSBIOS
  #include <xdc/runtime/SysMin.h>
#else
  #include "ipc_trace.h"
#endif

/*
 * Sizes of the virtqueues (expressed in number of buffers supported,
 * and must be power of 2)
 */
#define R5F_RPMSG_VQ0_SIZE             256
#define R5F_RPMSG_VQ1_SIZE             256

/* flip up bits whose indices represent features we support */
#define RPMSG_R5F_C0_FEATURES          1

#ifdef SYSBIOS
  #define IPC_TRACE_BUFFER_MAX_SIZE     (0x80000)
  extern __T1_xdc_runtime_SysMin_Module_State__outbuf xdc_runtime_SysMin_Module_State_0_outbuf__A[];
  #define TRACEBUFADDR ((uintptr_t)&xdc_runtime_SysMin_Module_State_0_outbuf__A)
#else
  #define TRACEBUFADDR ((uintptr_t)&Ipc_traceBuffer)
#endif

#define RPMSG_VRING_ADDR_ANY FW_RSC_ADDR_ANY

const Ipc_ResourceTable ti_ipc_remoteproc_ResourceTable __attribute__ ((section (".resource_table"), aligned (4096))) = 
{
    1,                   /* we're the first version that implements this */
    NUM_ENTRIES,         /* number of entries in the table */
    0, 0,                /* reserved, must be zero */

    /* offsets to entries */
    {
        offsetof(Ipc_ResourceTable, rpmsg_vdev),
        offsetof(Ipc_ResourceTable, trace),
    },

    /* rpmsg vdev entry */
    {
        TYPE_VDEV, VIRTIO_ID_RPMSG, 0,
        RPMSG_R5F_C0_FEATURES, 0, 0, 0, 2, { 0, 0 },
        /* no config data */
    },
    /* the two vrings */
#if defined (BUILD_MCU1_0)
    { RPMSG_VRING_ADDR_ANY, 4096, R5F_RPMSG_VQ0_SIZE, 1, 0 },
    { RPMSG_VRING_ADDR_ANY, 4096, R5F_RPMSG_VQ1_SIZE, 2, 0 },
#elif defined (BUILD_MCU1_1)
    { RPMSG_VRING_ADDR_ANY, 4096, R5F_RPMSG_VQ0_SIZE, 1, 0 },
    { RPMSG_VRING_ADDR_ANY, 4096, R5F_RPMSG_VQ1_SIZE, 2, 0 },
#else
    { RPMSG_VRING_ADDR_ANY, 4096, R5F_RPMSG_VQ0_SIZE, 1, 0 },
    { RPMSG_VRING_ADDR_ANY, 4096, R5F_RPMSG_VQ1_SIZE, 2, 0 },
#endif

    {
        (TRACE_INTS_VER0 | TYPE_TRACE), TRACEBUFADDR, IPC_TRACE_BUFFER_MAX_SIZE, 0, "trace:r5f0",
    },
};


#ifdef __cplusplus
}
#endif

#endif /* IPC_AM65XX_RSCTABLE_H_ */
