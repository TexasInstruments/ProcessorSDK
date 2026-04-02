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
 *  \file   ipc_config.h
 *
 *  \brief  This file has the configuration for specific SOC
 *
 *  \details
 *
 **/
#ifndef IPC_CONFIG_H_
#define IPC_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>

#include <ti/csl/soc.h>
#include <ti/drv/sciclient/sciclient.h>

#if defined (SOC_AM65XX) || defined (SOC_J721E) || (SOC_AM64X)
#include <ti/csl/cslr_gtc.h>
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */
/** \brief Core definitions */
#define    MAILBOX_IPC_MPU1_0           (0)    /**< ARM A53 - VM0 */
#define    MAILBOX_IPC_MCU1_0           (1)    /**< ARM MCU  R5F - core0 */
#define    MAILBOX_IPC_MCU1_1           (2)    /**< ARM MCU  R5F - core1 */
#define    MAILBOX_IPC_MAX_PROCS        (3)    /**< Maximum Processors */

/* In general , Get this from CSL, not available for AM65XX */
#define    MAILBOX_MAX_CLUSTER_CNT              (12U)

#define  MAILBOX_CLUSTER_INVALID   (0xFFU)
#define  MAILBOX_USER_INVALID      (0xFFU)

#ifdef BUILD_MPU
#define MAILBOX_IPC_TIMER_FREQ_IN_HZ 200000000U
#endif

#ifdef BUILD_MCU
#define MAILBOX_IPC_TIMER_FREQ_IN_HZ 400000000U
#endif

#define MAILBOX_SCICLIENT_TIMEOUT 0xffffffffu

#define TEST_VIM_BASE_ADDR (CSL_MCU_DOMAIN_VIM_BASE_ADDR)

/* ========================================================================== */
/*                                 Structures                                 */
/* ========================================================================== */

typedef struct mailboxIpc_MailboxEntry_s
{
    uint32_t    cluster;
    uint32_t    user;
    uint32_t    fifo;
} mailboxIpc_MailboxEntry;

typedef struct mailboxIpc_MailboxInfo_s
{
    mailboxIpc_MailboxEntry    rx;
} mailboxIpc_MailboxInfo;

extern const mailboxIpc_MailboxInfo   gMailboxIpc_MailboxInfo[MAILBOX_IPC_MAX_PROCS][MAILBOX_IPC_MAX_PROCS];
extern const uint32_t gMailboxIpc_MailboxBaseAddressArray[MAILBOX_MAX_CLUSTER_CNT];
extern const uint32_t gMailboxIpc_MailboxClusterIdArray[MAILBOX_MAX_CLUSTER_CNT];
extern uint32_t gMailboxIpc_MailboxInterruptInfo[MAILBOX_IPC_MAX_PROCS];
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MAILBOX_APP_H_ */
/********************************* End of file ******************************/
