/*
 *  Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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
 *
 */
 /**
 *  \file   ipc_config.c
 *
 *  \brief  This file has the configuration for specific SOC
 *
 *  \details
 *
 **/

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "ipc_config.h"

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */


/* ========================================================================== */
/*                                 Structures                                 */
/* ========================================================================== */
const mailboxIpc_MailboxInfo   gMailboxIpc_MailboxInfo[MAILBOX_IPC_MAX_PROCS][MAILBOX_IPC_MAX_PROCS] =
{
    /* Host Processor - A53-vm0	*/
    {
        { { MAILBOX_CLUSTER_INVALID, MAILBOX_USER_INVALID, 0U }},  /* Self - A53-vm0 */
        { {    2U,    2U,  2U}},                                   /* mcu-r5f0 */
        { {    3U,    2U,  2U}},                                   /* mcu-r5f1 */
    },
    /* Host Processor - mcu1_0 	*/
    {
        { {    3U,    0U,  0U }},                                  /* A53-vm0 */
        { { MAILBOX_CLUSTER_INVALID, MAILBOX_USER_INVALID, 0U }},  /* Self - mcu-r5f0 */
        { {    0U,    0U,  0U }},                                  /* mcu-r5f1 */
    },
    /* Host Processor - mcu1_1 */
    {
        { {    3U,    1U,  1U }},                                 /* A53-vm0 */
        { {    0U,    1U,  1U }},                                 /* mcu-r5f0 */
        { { MAILBOX_CLUSTER_INVALID, MAILBOX_USER_INVALID, 0U }}, /* Self - mcu-r5f1 */
    }
};

const uint32_t gMailboxIpc_MailboxBaseAddressArray[MAILBOX_MAX_CLUSTER_CNT] =
{
    CSL_MAILBOX0_REGS0_BASE,
    CSL_MAILBOX0_REGS1_BASE,
    CSL_MAILBOX0_REGS2_BASE,
    CSL_MAILBOX0_REGS3_BASE,
    CSL_MAILBOX0_REGS4_BASE,
    CSL_MAILBOX0_REGS5_BASE,
    CSL_MAILBOX0_REGS6_BASE,
    CSL_MAILBOX0_REGS7_BASE,

};

#ifdef BUILD_MPU
/* Host Processor - A53-vm0	*/
const uint32_t gMailboxIpc_MailboxInterruptInfo[MAILBOX_IPC_MAX_PROCS] =
{
    0xffffffffU,  /* Self - A53-vm0 */
    CSLR_GICSS0_SPI_MAILBOX0_MAILBOX_CLUSTER_2_MAILBOX_CLUSTER_PEND_2, /* mcu-r5f0 */
    CSLR_GICSS0_SPI_MAILBOX0_MAILBOX_CLUSTER_3_MAILBOX_CLUSTER_PEND_2, /* mcu-r5f1 */
};
#endif

#ifdef BUILD_MCU1_0
/* Host Processor - mcu1_0 	*/
const uint32_t gMailboxIpc_MailboxInterruptInfo[MAILBOX_IPC_MAX_PROCS] =
{
    CSLR_R5FSS0_CORE0_INTR_MAILBOX0_MAILBOX_CLUSTER_3_MAILBOX_CLUSTER_PEND_0, /* A53-vm0 */
    0xffffffffU,  /* Self - mcu-r5f0 */
    CSLR_R5FSS0_CORE0_INTR_MAILBOX0_MAILBOX_CLUSTER_0_MAILBOX_CLUSTER_PEND_0, /* mcu-r5f1 */
};
#endif
#ifdef BUILD_MCU1_1
    /* Host Processor - mcu1_1 */
const uint32_t gMailboxIpc_MailboxInterruptInfo[MAILBOX_IPC_MAX_PROCS] =
{
    CSLR_R5FSS0_CORE1_INTR_MAILBOX0_MAILBOX_CLUSTER_3_MAILBOX_CLUSTER_PEND_1, /* A53-vm0 */
    CSLR_R5FSS0_CORE1_INTR_MAILBOX0_MAILBOX_CLUSTER_0_MAILBOX_CLUSTER_PEND_1, /* mcu-r5f0 */
    0xffffffffU, /* Self - mcu-r5f1 */
};
#endif

#ifdef __cplusplus
}
#endif /* extern "C" */

/********************************* End of file ******************************/
