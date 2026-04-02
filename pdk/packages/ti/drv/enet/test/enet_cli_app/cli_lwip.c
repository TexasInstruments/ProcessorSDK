/*
 * Copyright (C) 2024 Texas Instruments Incorporated
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * \file  cli_lwip.c
 *
 * \brief this file contains all lwip functions
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "cli_common.h"
#include "cli_lwip.h"

#include <enet_apputils.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ENET_SYSCFG_DEFAULT_NETIF_IDX              (NETIF_INST_ID0)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef void* LwipifEnetApp_Handle;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void Lwip_setupNetworkStack();

static void Lwip_tcpipInitCompleteCb(void *pArg);

static void Lwip_setupNetif();

static void Lwip_allocateIPAddress();

static void Lwip_netifStatusChangeCb(struct netif *pNetif);

static void Lwip_netifLinkChangeCb(struct netif *pNetif);

static int32_t Lwip_isNetworkUp(struct netif *netif_);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static struct dhcp g_netifDhcp[ENET_SYSCFG_NETIF_COUNT];
static struct netif *g_pNetif[ENET_SYSCFG_NETIF_COUNT];
LwipifEnetApp_Handle hlwipIfApp = NULL;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

BaseType_t EnetCLI_lwipShell(char *writeBuffer, size_t writeBufferLen,
        const char *commandString)
{
    char *parameter;
    BaseType_t paramLen;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);
    if ((strncmp(parameter, "start", paramLen) == 0)
            && EnetApp_inst.shellFlag == SHELL_IDLE)
    {
        Lwip_setupNetworkStack();
        uint32_t netupMask = 0;
        while (netupMask == 0)
        {
            for (uint32_t netifIdx = 0; netifIdx < ENET_SYSCFG_NETIF_COUNT;
                    netifIdx++)
            {
                if (Lwip_isNetworkUp(g_pNetif[netifIdx]))
                {
                    netupMask |= (1 << netifIdx);
                }
                TaskP_sleep(2000U);
            }
        }

        shell_init_app();
        EnetApp_inst.shellFlag = SHELL_RUNNING;
        snprintf(writeBuffer, writeBufferLen, "Shell Opened\r\n");
    }
    else if ((strncmp(parameter, "stop", paramLen) == 0)
            && EnetApp_inst.shellFlag == SHELL_RUNNING)
    {
        EnetApp_inst.shellFlag = SHELL_IDLE;
        snprintf(writeBuffer, writeBufferLen, "Shell Closed\r\n");
    }
    return pdFALSE;
}

/* ========================================================================== */
/*                   Static Function Definitions                              */
/* ========================================================================== */

static void Lwip_setupNetworkStack()
{
    sys_sem_t pInitSem;
    const err_t err = sys_sem_new(&pInitSem, 0);
    EnetAppUtils_assert(err == ERR_OK);

    tcpip_init(Lwip_tcpipInitCompleteCb, &pInitSem);

    /* wait for TCP/IP initialization to complete */
    sys_sem_wait(&pInitSem);sys_sem_free(&pInitSem);

    return;
}

static void Lwip_tcpipInitCompleteCb(void *pArg)
{
    sys_sem_t *pSem = (sys_sem_t*) pArg;
    EnetAppUtils_assert(pArg != NULL);

    /* init randomizer again (seed per thread) */
    srand((unsigned int) sys_now() / 1000);

    Lwip_setupNetif();

    Lwip_allocateIPAddress();

    sys_sem_signal(pSem);
}

static void Lwip_setupNetif()
{
    ip4_addr_t ipaddr, netmask, gw;

    ip4_addr_set_zero(&gw);
    ip4_addr_set_zero(&ipaddr);
    ip4_addr_set_zero(&netmask);

    DebugP_log0("Starting lwIP, local interface IP is dhcp-enabled\r\n");
    hlwipIfApp = NULL;
    for (uint32_t netifIdx = 0U; netifIdx < ENET_SYSCFG_NETIF_COUNT; netifIdx++)
    {
        /* Open the netif and get it populated*/
        g_pNetif[netifIdx] = NULL;
        
        #if LWIP_NETIF_STATUS_CALLBACK
        netif_set_status_callback(g_pNetif[netifIdx], Lwip_netifStatusChangeCb);
        #endif

        #if LWIP_NETIF_LINK_CALLBACK
        netif_set_link_callback(g_pNetif[netifIdx], Lwip_netifLinkChangeCb);
        #endif
        
        netif_set_up(g_pNetif[NETIF_INST_ID0 + netifIdx]);
    }
}

static void Lwip_allocateIPAddress()
{

#if LWIP_FREERTOS_CHECK_CORE_LOCKING
#if LWIP_TCPIP_CORE_LOCKING
sys_lock_tcpip_core();
#endif
#endif

for (uint32_t netifIdx = 0U; netifIdx < ENET_SYSCFG_NETIF_COUNT; netifIdx++)
{
    dhcp_set_struct(g_pNetif[NETIF_INST_ID0 + netifIdx],
            &g_netifDhcp[NETIF_INST_ID0 + netifIdx]);

    const err_t err = dhcp_start(g_pNetif[NETIF_INST_ID0 + netifIdx]);
    EnetAppUtils_assert(err == ERR_OK);
}
#if LWIP_FREERTOS_CHECK_CORE_LOCKING
#if LWIP_TCPIP_CORE_LOCKING
sys_unlock_tcpip_core();
#endif
#endif
return;
}

static void Lwip_netifStatusChangeCb(struct netif *pNetif)
{
if (netif_is_up(pNetif))
{
    DebugP_log1("[%d]Enet IF UP Event. Local interface IP:%s\r\n", pNetif->num);
    DebugP_log0(ip4addr_ntoa(netif_ip4_addr(pNetif)));
}
else
{
    DebugP_log1("[%d]Enet IF DOWN Event\r\n", pNetif->num);
}
return;
}

static void Lwip_netifLinkChangeCb(struct netif *pNetif)
{
if (netif_is_link_up(pNetif))
{
    DebugP_log1("[%d]Network Link UP Event\r\n", pNetif->num);
}
else
{
    DebugP_log1("[%d]Network Link DOWN Event\r\n", pNetif->num);
}
return;
}

static int32_t Lwip_isNetworkUp(struct netif *netif_)
{
return (netif_is_up(netif_) && netif_is_link_up(netif_)
        && !ip4_addr_isany_val(*netif_ip4_addr(netif_)));
}
