/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 *  \file ENET_CONFIG_SERVER.c
 *
 *  \brief CPSW configuration server
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/osal/MailboxP.h>
#include <ti/osal/LoadP.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/include/per/cpsw.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_networkutils.h>

#include <lwip/sys.h>
#include "lwip/api.h"

/* API Headers */
#include "priv/inc/jsmn.h"
#include "priv/inc/enet_serial.h"

#include "enet_cfgserver.h"

#if defined(SAFERTOS)
#include "SafeRTOS_API.h"
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Configuration server application stack size */
#define CPSWSERVERAPP_TSK_STACK_SIZE              (16U * 1024U)
#define CPSWSERVERAPP_SOCKETPORTNUM               (5555U)
/*Buffer size for config server*/
#define ENET_CONFIG_SERVER_BUFFER_SIZE            (1024)
#define ENET_CONFIG_SERVER_UART_CFG_BUFFER_SIZE   (1024U * 2U)
#define ENET_CONFIG_SERVER_XMODEM_ACK             (0x06)
#define CPSWDISPATCHER_MBOX_MSG_COUNT             (10U)
#define ENET_CONFIG_SERVER_JSMN_TOKEN_COUNT       (1000U)
#define ENET_CONFIG_SERVER_CPU_LOAD_MAX_LEN       (16U)
#define UART_TRANSPORT_ENABLE                     (0U)

#if defined(SAFERTOS)
#define CPSWSERVERAPP_TSK_STACK_ALIGN             CPSWSERVERAPP_TSK_STACK_SIZE
#define CPSWDISPATCHER_MBOX_SIZE                  (sizeof(CpswDispatcher_MailboxObj) * CPSWDISPATCHER_MBOX_MSG_COUNT + safertosapiQUEUE_OVERHEAD_BYTES)
#else
#define CPSWSERVERAPP_TSK_STACK_ALIGN             (32)
#define CPSWDISPATCHER_MBOX_SIZE                  (sizeof(CpswDispatcher_MailboxObj) * CPSWDISPATCHER_MBOX_MSG_COUNT)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct CpswDispatcher_MailboxObj_s
{
    char *recv;

    char *send;
} CpswDispatcher_MailboxObj;

typedef struct EnetCfgServer_AppObj_s
{
    /* Enet Handle */
    Enet_Handle hEnet;

    /* Enet Type */
    Enet_Type enetType;

    /* Enet Instance */
    uint32_t instId;

    /* Core Id */
    uint32_t coreId;

    /* Dispatch Request Mailbox */
    MailboxP_Handle hMboxIn;

    /* Dispatch Response Mailbox */
    MailboxP_Handle hMboxOut;

    /* Task Handles  */
    sys_thread_t taskNetwork;

    TaskP_Handle taskDispatcher;

    TaskP_Handle taskUart;

    jsmn_parser parser;
} EnetCfgServer_AppObj;
/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* Task functions */
static void    EnetCfgServer_networkTask(void* arg0);

static void    EnetCfgServer_dispatcherTask(void* arg0,
                                            void* arg1);

#if UART_TRANSPORT_ENABLE
static void    EnetCfgServer_uartTask(void* arg0,
                                      void* arg1);

#endif

/* IOCTL Functions */
static void    EnetCfgServer_enabledisableMcast(char *recvBuff,
                                                char *sendBuff);

static void    EnetCfgServer_addVlanEntry(char *recvBuff,
                                          char *sendBuff);

static void    EnetCfgServer_getMacPortStats(char *recvBuff,
                                             char *sendBuff);

static void    EnetCfgServer_getHostPortStats(char *recvBuff,
                                              char *sendBuff);

static void    EnetCfgServer_lookupUnicast(char *recvBuff,
                                           char *sendBuff);

static void    EnetCfgServer_getBcastMcastLimits(char *recvBuff,
                                                 char *sendBuff);

static void EnetCfgServer_dumpAleTables(char *recvBuff,
                                        char *sendBuff);

static void    EnetCfgServer_getVersion(char *recvBuff,
                                        char *sendBuff);

static void    EnetCfgServer_initFxnTable(void);

static void    EnetCfgServer_parseInterVlanRoutingCfg(char *recvBuff,
                                                      EnetCfgServer_InterVlanConfig *pinterVlanCfg);

static void    EnetCfgServer_addIpNxtHdrFilter(char *recvBuff,
                                               char *sendBuff);

static void    EnetCfgServer_enableRateLimiting(char *recvBuff,
                                                char *sendBuff);

static int32_t EnetCfgServer_setPolicerPriorityRateLimiting(EnetCfgServer_RateLimitingCfg *pRateLimitingCfg);

static int32_t EnetCfgServer_addAleEntryRateLimiting(EnetCfgServer_RateLimitingCfg *pRateLimitingCfg);

static int32_t EnetCfgServer_setPolicerGlobalConfig(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

EnetCfgServer_AppObj gEnetCfgServerObj;

/* IOCTL Function Table */
void(*EnetCfgServer_fxn_table[ENET_CONFIG_SERVER_FXN_TBL_SIZE]) (char *recvBuff, char *sendBuff);

/* Objects required for init using UART*/
EnetSerial_ContextObj *gSerialContextObj;

/* Required for communication with clients*/
jsmntok_t token[ENET_CONFIG_SERVER_JSMN_TOKEN_COUNT];
uint8_t gNwRecvBuff[ENET_CONFIG_SERVER_BUFFER_SIZE];
uint8_t gNwSendBuff[ENET_CONFIG_SERVER_BUFFER_SIZE];
uint8_t gUartRecvBuff[ENET_CONFIG_SERVER_BUFFER_SIZE];
uint8_t gUartSendBuff[ENET_CONFIG_SERVER_BUFFER_SIZE];

static uint8_t gCpswDispatcherTskStackMain[CPSWSERVERAPP_TSK_STACK_SIZE]
__attribute__ ((aligned(CPSWSERVERAPP_TSK_STACK_ALIGN)));
#if UART_TRANSPORT_ENABLE

static uint8_t gCpswUartTskStackMain[CPSWSERVERAPP_TSK_STACK_SIZE]
__attribute__ ((aligned(CPSWSERVERAPP_TSK_STACK_ALIGN)));
#endif

static uint8_t dispatchRequestMbxBuf[CPSWDISPATCHER_MBOX_SIZE] __attribute__ ((aligned(32)));

static uint8_t dispatchResponseMbxBuf[CPSWDISPATCHER_MBOX_SIZE] __attribute__ ((aligned(32)));

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int32_t EnetCfgServer_init(Enet_Type enetType, uint32_t instId)
{
    int32_t status = ENET_SOK;
    TaskP_Params params;
    MailboxP_Params mboxParams;

    gEnetCfgServerObj.hEnet    = Enet_getHandle(enetType, instId);
    gEnetCfgServerObj.coreId   = EnetSoc_getCoreId();
    gEnetCfgServerObj.enetType = enetType;
    gEnetCfgServerObj.instId   = instId;

    if (gEnetCfgServerObj.hEnet == NULL)
    {
        status = ENET_EFAIL;
    }

    /* Create Dispatch Request Mailbox */
    if (status == ENET_SOK)
    {
        MailboxP_Params_init(&mboxParams);
        mboxParams.name = (uint8_t *)"Dispatch Request MailBox";
        mboxParams.size =  sizeof(CpswDispatcher_MailboxObj);
        mboxParams.count = CPSWDISPATCHER_MBOX_MSG_COUNT;
        mboxParams.buf = (void *)dispatchRequestMbxBuf;
        mboxParams.bufsize = sizeof(dispatchRequestMbxBuf);

        gEnetCfgServerObj.hMboxIn = MailboxP_create(&mboxParams);

        if (gEnetCfgServerObj.hMboxIn == NULL)
        {
            status = ENET_EFAIL;
        }
    }

    if (status == ENET_SOK)
    {
        /* Create Dispatch Response Mailbox */
        MailboxP_Params_init(&mboxParams);
        mboxParams.name  = (uint8_t *)"Dispatch Response MailBox";
        mboxParams.size =  sizeof(CpswDispatcher_MailboxObj);
        mboxParams.count = CPSWDISPATCHER_MBOX_MSG_COUNT;
        mboxParams.buf = (void *)dispatchResponseMbxBuf;
        mboxParams.bufsize = sizeof(dispatchResponseMbxBuf);

        gEnetCfgServerObj.hMboxOut = MailboxP_create(&mboxParams);
        if (gEnetCfgServerObj.hMboxOut == NULL)
        {
            status = ENET_EFAIL;
        }
    }

    if (status == ENET_SOK)
    {
        /* Initialize dispatch function table */
        EnetCfgServer_initFxnTable();
    }

    if (status == ENET_SOK)
    {
        /* Create Network Listener Task.  */
        gEnetCfgServerObj.taskNetwork = sys_thread_new((const char *)"Network Task",
                                                       &EnetCfgServer_networkTask,
                                                       NULL,
                                                       CPSWSERVERAPP_TSK_STACK_SIZE,
                                                       7U);
        if (gEnetCfgServerObj.taskNetwork.thread_handle == NULL)
        {
            status = ENET_EFAIL;
        }
    }

    if (status == ENET_SOK)
    {
        /* Create Dispatcher Task.  */
        TaskP_Params_init(&params);
        params.priority                  = 7U;
        params.stack                     = gCpswDispatcherTskStackMain;
        params.stacksize                 = sizeof(gCpswDispatcherTskStackMain);
        params.name                      = (const char *)"Dispatcher Task";
        gEnetCfgServerObj.taskDispatcher = TaskP_create(&EnetCfgServer_dispatcherTask, &params);
        if (gEnetCfgServerObj.taskDispatcher == NULL)
        {
            status = ENET_EFAIL;
        }
    }

#if UART_TRANSPORT_ENABLE
    if (status == ENET_SOK)
    {
        /* Create UART Listener task.  */
        TaskP_Params_init(&params);
        params.priority            = 14U;
        params.stack               = gCpswUartTskStackMain;
        params.stacksize           = sizeof(gCpswUartTskStackMain);
        params.name                = (const char *)"UART Task";
        gEnetCfgServerObj.taskUart = TaskP_create(&EnetCfgServer_uartTask, &params);
        if (gEnetCfgServerObj.taskUart == NULL)
        {
            status = ENET_EFAIL;
        }
    }
#endif

    return status;
}

static void EnetCfgServer_getCpuLoad(char *recvBuff,
                                     char *sendBuff)
{
    char outString[ENET_CONFIG_SERVER_BUFFER_SIZE];
    uint8_t cpu_load = 0U;

    /* LoadP_getCPULoad() currently supported only for FreeRTOS */
#if defined(FREERTOS)
    cpu_load = LoadP_getCPULoad();
    LoadP_reset();
#endif

    memset(&outString[0], 0, sizeof(outString));
    memset(&sendBuff[0U], 0, ENET_CONFIG_SERVER_BUFFER_SIZE * sizeof(sendBuff[0]));

    snprintf(outString, (ENET_CONFIG_SERVER_CPU_LOAD_MAX_LEN - 1U), "{\"load\":%d}", cpu_load);
    memcpy(&sendBuff[0U], outString, sizeof(outString));
}

static void EnetCfgServer_initFxnTable(void)
{
    EnetCfgServer_fxn_table[0] = NULL;
    EnetCfgServer_fxn_table[1] = &EnetCfgServer_getVersion;
    EnetCfgServer_fxn_table[2] = &EnetCfgServer_getBcastMcastLimits;
    EnetCfgServer_fxn_table[3] = &EnetCfgServer_lookupUnicast;
    EnetCfgServer_fxn_table[4] = &EnetCfgServer_getHostPortStats;
    EnetCfgServer_fxn_table[5] = &EnetCfgServer_getMacPortStats;
    EnetCfgServer_fxn_table[6] = &EnetCfgServer_addVlanEntry;
    EnetCfgServer_fxn_table[7] = &EnetCfgServer_getCpuLoad;
    EnetCfgServer_fxn_table[8] = &EnetCfgServer_enabledisableMcast;
    /* Cmd 9 and 10 are reserved for SW & HW Inter VLAN Routing */
    EnetCfgServer_fxn_table[11] = &EnetCfgServer_addIpNxtHdrFilter;
    EnetCfgServer_fxn_table[12] = &EnetCfgServer_enableRateLimiting;
    EnetCfgServer_fxn_table[13] = &EnetCfgServer_dumpAleTables;
}

void EnetCfgServer_getCpswCfg(Cpsw_Cfg *cpswCfg)
{
    EnetSerial_ConfigObj cfg;
    int32_t maxLen;
    int tokenCount, val, i = 0;
    char recvBuff[ENET_CONFIG_SERVER_UART_CFG_BUFFER_SIZE];
    int32_t status = EnetSerial_invalid_read_len;

    cfg.magic                               = ENET_SERIAL_MODULE_MAGIC;
    cfg.protocol                            = ENET_SERIAL_PROTOCOL_XMODEM;
    cfg.protocolConfig.xmodem.maxErrorCount = 10U;
    cfg.protocolConfig.xmodem.ackTimeoutSec = 3U;
    cfg.protocolConfig.xmodem.charTimeoutMs = 2U;

    gSerialContextObj = (EnetSerial_ContextObj *)EnetSerial_init(&cfg);
    EnetSerial_open(gSerialContextObj);
    EnetSerial_start(gSerialContextObj);

    status = EnetSerial_read(gSerialContextObj, (uint8_t *)recvBuff, ENET_ARRAYSIZE(recvBuff));

    if (status == EnetSerial_success)
    {
        jsmn_init(&gEnetCfgServerObj.parser);
        tokenCount = jsmn_parse(&gEnetCfgServerObj.parser,
                                (char *)recvBuff,
                                ENET_CONFIG_SERVER_UART_CFG_BUFFER_SIZE,
                                token,
                                ENET_ARRAYSIZE(token));

        for (i = 1U; i < tokenCount; i++)
        {
            char choice[ENET_CONFIG_SERVER_TOKEN_SIZE];
            if (jsoneq((char *)recvBuff, &token[i], "vlanAware") == 0)
            {
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       ENET_CONFIG_SERVER_TOKEN_SIZE);
                snprintf(choice, maxLen, "%s", recvBuff + token[i + 1].start);
                val                           = atoi(choice);
                cpswCfg->vlanCfg.vlanAware = val;
            }

            if (jsoneq((char *)recvBuff, &token[i], "removeCrc") == 0)
            {
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       ENET_CONFIG_SERVER_TOKEN_SIZE);
                snprintf(choice, maxLen, "%s", recvBuff + token[i + 1].start);
                val                               = atoi(choice);
                cpswCfg->hostPortCfg.removeCrc = val;
            }

            if (jsoneq((char *)recvBuff, &token[i], "padShortPacket") == 0)
            {
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       ENET_CONFIG_SERVER_TOKEN_SIZE);
                snprintf(choice, maxLen, "%s", recvBuff + token[i + 1].start);
                val                                    = atoi(choice);
                cpswCfg->hostPortCfg.padShortPacket = val;
            }

            if (jsoneq((char *)recvBuff, &token[i], "passCrcErrors") == 0)
            {
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       ENET_CONFIG_SERVER_TOKEN_SIZE);
                snprintf(choice, maxLen, "%s", recvBuff + token[i + 1].start);
                val                                   = atoi(choice);
                cpswCfg->hostPortCfg.passCrcErrors = val;
            }
        }
    }
}

static void EnetCfgServer_networkTask(void* arg0)
{
    CpswDispatcher_MailboxObj msg;

    /* Network socket object for RX */
    void *netRxSockObjHandle;

#if (LWIP_SOCKET || LWIP_NETCONN) && LWIP_NETCONN_SEM_PER_THREAD
    netconn_thread_init();
#endif

    netRxSockObjHandle = EnetNetworkUtils_createSocket(CPSWSERVERAPP_SOCKETPORTNUM);
    EnetAppUtils_assert(netRxSockObjHandle != NULL);

    msg.recv = (char *)gNwRecvBuff;
    msg.send = (char *)gNwSendBuff;

    while (BTRUE)
    {
        if (EnetNetworkUtils_readPayload(netRxSockObjHandle, &gNwRecvBuff[0U], 1024) == 0U)
        {
            MailboxP_post(gEnetCfgServerObj.hMboxIn, &msg, MailboxP_WAIT_FOREVER);
            MailboxP_pend(gEnetCfgServerObj.hMboxOut, &msg, MailboxP_WAIT_FOREVER);
            EnetNetworkUtils_writePayload(netRxSockObjHandle, &gNwSendBuff[0U], 1024);
        }
        else
        {
            TaskP_sleep(10U);
        }
    }
}

#if UART_TRANSPORT_ENABLE
static void EnetCfgServer_uartTask(void* arg0,
                                   void* arg1)
{
    EnetSerial_ConfigObj cfg;
    CpswDispatcher_MailboxObj msg;
    int32_t status = Serial_invalid_read_len;

    msg.recv = (char *)gUartRecvBuff;
    msg.send = (char *)gUartSendBuff;

    cfg.magic                               = ENET_SERIAL_MODULE_MAGIC;
    cfg.protocol                            = ENET_SERIAL_PROTOCOL_XMODEM;
    cfg.protocolConfig.xmodem.maxErrorCount = 10U;
    cfg.protocolConfig.xmodem.ackTimeoutSec = 3U;
    cfg.protocolConfig.xmodem.charTimeoutMs = 2U;

    gSerialContextObj = (Serial_Context_Obj *)Serial_init((void *)&cfg);
    Serial_open(gSerialContextObj);
    Serial_start((void *)gSerialContextObj);

    status = Serial_read((void *)gSerialContextObj, gUartRecvBuff, ENET_ARRAYSIZE(gUartRecvBuff));

    if (status == Serial_success)
    {
        MailboxP_post(gEnetCfgServerObj.hMboxIn, &msg, MailboxP_WAIT_FOREVER);
        MailboxP_pend(gEnetCfgServerObj.hMboxOut, &msg, MailboxP_WAIT_FOREVER);
    }
}

#endif

static void EnetCfgServer_dispatcherTask(void* arg0,
                                         void* arg1)
{
    int i, tokenCount, cmd;
    char choice[5U];
    int32_t maxLen;

    while (BTRUE)
    {
        CpswDispatcher_MailboxObj msg;
        MailboxP_pend(gEnetCfgServerObj.hMboxIn, &msg, MailboxP_WAIT_FOREVER);

        jsmn_init(&gEnetCfgServerObj.parser);
        tokenCount = jsmn_parse(&gEnetCfgServerObj.parser,
                                msg.recv,
                                ENET_CONFIG_SERVER_BUFFER_SIZE,
                                token,
                                ENET_ARRAYSIZE(token));
        cmd = -1;
        for (i = 0; i < tokenCount; i++)
        {
            if (jsoneq(msg.recv, &token[i], "cmd") == 0)
            {
                memset(choice, 0U, sizeof(choice));
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       sizeof(choice));
                snprintf(choice, maxLen, "%s", msg.recv + token[i + 1].start);
                cmd = atoi(choice);
                i++;
                break;
            }
        }

        if (cmd != -1 && cmd < ENET_CONFIG_SERVER_FXN_TBL_SIZE && NULL != EnetCfgServer_fxn_table[cmd])
        {
            if (cmd == 9U || cmd == 10U)
            {
                EnetCfgServer_InterVlanConfig interVlanCfg;

                EnetCfgServer_parseInterVlanRoutingCfg(msg.recv,
                                                       &interVlanCfg);
                EnetCfgServer_fxn_table[cmd]((char *)&interVlanCfg, msg.send);
                MailboxP_post(gEnetCfgServerObj.hMboxOut, &msg, MailboxP_WAIT_FOREVER);
            }
            else
            {
                EnetCfgServer_fxn_table[cmd](msg.recv, msg.send);
                MailboxP_post(gEnetCfgServerObj.hMboxOut, &msg, MailboxP_WAIT_FOREVER);
            }
        }
        else
        {
            char errorString[ENET_CONFIG_SERVER_BUFFER_SIZE] = {"{\"Error\":\"No cmd\"}"};
            memcpy(msg.send, &errorString[0], sizeof(errorString));
            MailboxP_post(gEnetCfgServerObj.hMboxOut, &msg, MailboxP_WAIT_FOREVER);
        }
    }
}

static void EnetCfgServer_dumpAleTables(char *recvBuff,
                                        char *sendBuff)
{
    char outString[ENET_CONFIG_SERVER_BUFFER_SIZE];
    Enet_IoctlPrms prms;
    int32_t status;

    memset(&outString[0], 0, sizeof(outString));
    memset(&sendBuff[0U], 0, ENET_CONFIG_SERVER_BUFFER_SIZE * sizeof(sendBuff[0]));

    ENET_IOCTL_SET_NO_ARGS(&prms);

    /* Dump ALE table */
    status = Enet_ioctl(gEnetCfgServerObj.hEnet, gEnetCfgServerObj.coreId, CPSW_ALE_IOCTL_DUMP_TABLE, &prms);
    if (status != ENET_SOK)
    {
        sprintf(outString, "\"Error\":%d", status);
    }

    /* Dump policer table */
    if (status == ENET_SOK)
    {
        status = Enet_ioctl(gEnetCfgServerObj.hEnet, gEnetCfgServerObj.coreId, CPSW_ALE_IOCTL_DUMP_POLICER_ENTRIES, &prms);
        if (status != ENET_SOK)
        {
            sprintf(outString, "\"Error\":%d", status);
        }
    }

    if (status == ENET_SOK)
    {
        sprintf(outString, "{\"Status\":%d}", status);
    }

    memcpy(sendBuff, outString, sizeof(outString));
}

static void EnetCfgServer_getVersion(char *recvBuff,
                                     char *sendBuff)
{
    int32_t status;
    Enet_IoctlPrms prms;
    Enet_Version outArgs;
    char outString[ENET_CONFIG_SERVER_BUFFER_SIZE];

    memset(&outString[0], 0, sizeof(outString));
    memset(&sendBuff[0U], 0, ENET_CONFIG_SERVER_BUFFER_SIZE * sizeof(sendBuff[0]));

    ENET_IOCTL_SET_OUT_ARGS(&prms, &outArgs);
    status = Enet_ioctl(gEnetCfgServerObj.hEnet, gEnetCfgServerObj.coreId, ENET_PER_IOCTL_GET_VERSION, &prms);
    if (status != ENET_SOK)
    {
        sprintf(outString, "\"Error\":%d", status);
        memcpy(&sendBuff[0U], outString, sizeof(outString));
        return;
    }

#if TODO
        sprintf(outString, "{\"ss\":{\"major\":%u,\"minor\":%u,\"rtl\":%u,\"id\":%u},"
            "\"cpsw\":{\"major\":%u,\"minor\":%u,\"rtl\":%u,\"id\":%u},"
            "\"ale\":{\"major\":%u,\"minor\":%u,\"rtl\":%u,\"id\":%u},"
            "\"cpts\":{\"major\":%u,\"minor\":%u,\"rtl\":%u,\"id\":%u},"
            "\"mdio\":{\"major\":%u,\"minor\":%u,\"rtl\":%u,\"id\":%u,\"scheme\":%u,\"bu\":%u}}",
            outArgs.ss.major, outArgs.ss.minor, outArgs.ss.rtl, outArgs.ss.id,
            outArgs.cpsw.major, outArgs.cpsw.minor, outArgs.cpsw.rtl, outArgs.cpsw.id,
            outArgs.ale.major, outArgs.ale.minor, outArgs.ale.rtl, outArgs.ale.id,
            outArgs.cpts.major, outArgs.cpts.minor, outArgs.cpts.rtl, outArgs.cpts.id,
            outArgs.mdio.major, outArgs.mdio.minor, outArgs.mdio.rtl, outArgs.mdio.id,
            outArgs.mdio.scheme, outArgs.mdio.bu);
#endif
    memcpy(&sendBuff[0U], outString, sizeof(outString));
}

static void EnetCfgServer_getBcastMcastLimits(char *recvBuff,
                                              char *sendBuff)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_GetBcastMcastRateLimitOutArgs outArgs;
    char outString[ENET_CONFIG_SERVER_BUFFER_SIZE];

    memset(&outString[0], 0, sizeof(outString));
    memset(&sendBuff[0U], 0, ENET_CONFIG_SERVER_BUFFER_SIZE * sizeof(sendBuff[0]));

    ENET_IOCTL_SET_OUT_ARGS(&prms, &outArgs);
    status = Enet_ioctl(gEnetCfgServerObj.hEnet,
                        gEnetCfgServerObj.coreId,
                        CPSW_ALE_IOCTL_GET_BCAST_MCAST_LIMIT,
                        &prms);
    if (status != ENET_SOK)
    {
        sprintf(outString, "\"Error\":%d", status);
        memcpy(&sendBuff[0U], outString, sizeof(outString));
        return;
    }

    sprintf(outString, "{\"rateLimitEn\":%d,\"rateLimitAtTxPort\":%d,\"numPorts\":%d,"
            "\"portPrms\":[{\"portNum\":%d,\"bcastRateLimitForPortEn\":%d,"
            "\"mcastRateLimitForPortEn\":%d,\"bcastLimitNumPktsPerSec\":%d,"
            "\"mcastLimitNumPktsPerSec\":%d},{\"portNum\":%d,"
            "\"bcastRateLimitForPortEn\":%d,\"mcastRateLimitForPortEn\":%d,"
            "\"bcastLimitNumPktsPerSec\":%d,\"mcastLimitNumPktsPerSec\":%d}]}",
            outArgs.rateLimitEn, outArgs.rateLimitAtTxPort, outArgs.numPorts,
            outArgs.portPrms[0].portNum, outArgs.portPrms[0].bcastRateLimitForPortEn,
            outArgs.portPrms[0].mcastRateLimitForPortEn, outArgs.portPrms[0].bcastLimitNumPktsPerSec,
            outArgs.portPrms[0].mcastLimitNumPktsPerSec, outArgs.portPrms[1].portNum,
            outArgs.portPrms[1].bcastRateLimitForPortEn, outArgs.portPrms[1].mcastRateLimitForPortEn,
            outArgs.portPrms[1].bcastLimitNumPktsPerSec, outArgs.portPrms[1].mcastLimitNumPktsPerSec);

    memcpy(&sendBuff[0U], outString, sizeof(outString));
}

static void EnetCfgServer_lookupUnicast(char *recvBuff,
                                        char *sendBuff)
{
    int32_t status, i, tokenCount, maxLen;
    char outString[ENET_CONFIG_SERVER_BUFFER_SIZE];
    Enet_IoctlPrms prms;
    CpswAle_MacAddrInfo inArgs;
    CpswAle_GetUcastEntryOutArgs outArgs;

    jsmn_init(&gEnetCfgServerObj.parser);
    tokenCount = jsmn_parse(&gEnetCfgServerObj.parser, recvBuff, ENET_CONFIG_SERVER_BUFFER_SIZE, token, ENET_ARRAYSIZE(token));
    memset(&outString[0], 0, ENET_ARRAYSIZE(outString));
    memset(&sendBuff[0U], 0, ENET_CONFIG_SERVER_BUFFER_SIZE * sizeof(sendBuff[0]));

    for (i = 1U; i < tokenCount; i++)
    {
        char value[ENET_CONFIG_SERVER_TOKEN_SIZE];
        if (jsoneq(recvBuff, &token[i], "vlanId") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s", recvBuff + token[i + 1].start);
            inArgs.vlanId = atoi(value);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "addr") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s", recvBuff + token[i + 1].start);
            status = EnetAppUtils_macAddrAtoI(value, &inArgs.addr[0U]);
            EnetAppUtils_assert(status == ENET_SOK);
            i++;
        }
    }

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);
    status = Enet_ioctl(gEnetCfgServerObj.hEnet, gEnetCfgServerObj.coreId, CPSW_ALE_IOCTL_LOOKUP_UCAST, &prms);
    if (status != ENET_SOK)
    {
        sprintf(outString, "\"Error\":%d", status);
        memcpy(&sendBuff[0U], outString, sizeof(outString));
        return;
    }

    sprintf(outString, "{\"touched\":%d,\"aleEntryIdx\":%d,"
            "\"info\":{\"portNum\":%d,\"blocked\":%d,\"secure\":%d,"
            "\"super\":%d,\"ageable\":%d,\"trunk\":%d}}",
            outArgs.touched, outArgs.aleEntryIdx,
            outArgs.info.portNum, outArgs.info.blocked, outArgs.info.secure,
            outArgs.info.super, outArgs.info.ageable, outArgs.info.trunk);

    memcpy(&sendBuff[0U], outString, sizeof(outString));
}

static void EnetCfgServer_getHostPortStats(char *recvBuff,
                                           char *sendBuff)
{
    int32_t status;
    uint8_t i;
    Enet_IoctlPrms prms;
    CpswStats_PortStats outArgs;

    ENET_IOCTL_SET_OUT_ARGS(&prms, &outArgs);
    char outString[ENET_CONFIG_SERVER_BUFFER_SIZE];
    memset(&outString[0], 0, sizeof(outString));
    memset(&sendBuff[0U], 0, ENET_CONFIG_SERVER_BUFFER_SIZE * sizeof(sendBuff[0]));

    status = Enet_ioctl(gEnetCfgServerObj.hEnet, gEnetCfgServerObj.coreId, ENET_STATS_IOCTL_GET_HOSTPORT_STATS, &prms);
    if (status != ENET_SOK)
    {
        sprintf(outString, "\"Error\":%d", status);
        memcpy(&sendBuff[0U], outString, sizeof(outString));
        return;
    }

    sprintf(outString, "{\"val\":[");

    for (i = 0U; i < CPSW_STATS_BLOCK_ELEM_NUM; i++)
    {
#if defined(BUILD_MPU1_0)
        if (i != (CPSW_STATS_BLOCK_ELEM_NUM - 1U))
        {
            sprintf(outString + strlen(outString), "%lu,", outArgs.val[i]);
        }
        else
        {
            sprintf(outString + strlen(outString), "%lu", outArgs.val[i]);
        }

#else
        if (i != (CPSW_STATS_BLOCK_ELEM_NUM - 1U))
        {
            sprintf(outString + strlen(outString), "%llu,", outArgs.val[i]);
        }
        else
        {
            sprintf(outString + strlen(outString), "%llu", outArgs.val[i]);
        }
#endif
    }

    sprintf(outString + strlen(outString), "]}");
    memcpy(&sendBuff[0U], outString, sizeof(outString));
}

static void EnetCfgServer_getMacPortStats(char *recvBuff,
                                          char *sendBuff)
{
    int32_t status, maxLen;
    Enet_IoctlPrms prms;
    CpswStats_PortStats outArgs;
    Enet_MacPort macPort;
    uint32_t i, tokenCount;
    char outString[ENET_CONFIG_SERVER_BUFFER_SIZE];

    jsmn_init(&gEnetCfgServerObj.parser);
    tokenCount = jsmn_parse(&gEnetCfgServerObj.parser, recvBuff, ENET_CONFIG_SERVER_BUFFER_SIZE, token, ENET_ARRAYSIZE(token));

    for (i = 1U; i < tokenCount; i++)
    {
        if (jsoneq(recvBuff, &token[i], "portNum") == 0)
        {
            char value[ENET_CONFIG_SERVER_TOKEN_SIZE];
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s", recvBuff + token[i + 1].start);
            int8_t val = atoi(value);
            switch (val)
            {
                case 0:
                    macPort = ENET_MAC_PORT_1;
                    break;

                case 1:
                    macPort = ENET_MAC_PORT_2;
                    break;

                case 2:
                    macPort = ENET_MAC_PORT_3;
                    break;

                case 3:
                    macPort = ENET_MAC_PORT_4;
                    break;

                case 4:
                    macPort = ENET_MAC_PORT_5;
                    break;

                case 5:
                    macPort = ENET_MAC_PORT_6;
                    break;

                case 6:
                    macPort = ENET_MAC_PORT_7;
                    break;

                case 7:
                    macPort = ENET_MAC_PORT_8;
                    break;
            }

            i++;
        }
    }

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &macPort, &outArgs);
    memset(&outString[0], 0, sizeof(outString));
    memset(&sendBuff[0U], 0, ENET_CONFIG_SERVER_BUFFER_SIZE * sizeof(sendBuff[0]));

    status = Enet_ioctl(gEnetCfgServerObj.hEnet, gEnetCfgServerObj.coreId, ENET_STATS_IOCTL_GET_MACPORT_STATS, &prms);
    if (status != ENET_SOK)
    {
        sprintf(outString, "\"Error\":%d", status);
        memcpy(&sendBuff[0U], outString, sizeof(outString));
        return;
    }

    sprintf(outString, "{\"val\":[");

    for (i = 0U; i < CPSW_STATS_BLOCK_ELEM_NUM; i++)
    {
#if defined(BUILD_MPU1_0)
        if (i != (CPSW_STATS_BLOCK_ELEM_NUM - 1U))
        {
            sprintf(outString + strlen(outString), "%lu,", outArgs.val[i]);
        }
        else
        {
            sprintf(outString + strlen(outString), "%lu", outArgs.val[i]);
        }

#else
        if (i != (CPSW_STATS_BLOCK_ELEM_NUM - 1U))
        {
            sprintf(outString + strlen(outString), "%llu,", outArgs.val[i]);
        }
        else
        {
            sprintf(outString + strlen(outString), "%llu", outArgs.val[i]);
        }
#endif
    }

    sprintf(outString + strlen(outString), "]}");
    memcpy(&sendBuff[0U], outString, sizeof(outString));
}

static void EnetCfgServer_addVlanEntry(char *recvBuff,
                                       char *sendBuff)
{
    uint32_t vlanId = 0U, vlanMemberMask = 0U, isEnable = 0U;
    int32_t status, i, tokenCount, maxLen;
    Enet_IoctlPrms prms;
    CpswAle_VlanEntryInfo inArgs;
    uint32_t aleEntryIdx;
    CpswAle_VlanIdInfo getVlanInArgs;
    CpswAle_GetVlanEntryOutArgs getVlanOutArgs;
    char outString[ENET_CONFIG_SERVER_BUFFER_SIZE];

    jsmn_init(&gEnetCfgServerObj.parser);
    tokenCount = jsmn_parse(&gEnetCfgServerObj.parser, recvBuff, ENET_CONFIG_SERVER_BUFFER_SIZE, token, ENET_ARRAYSIZE(token));

    for (i = 1; i < tokenCount; i++)
    {
        char value[ENET_CONFIG_SERVER_TOKEN_SIZE];
        if (jsoneq(recvBuff, &token[i], "vlanId") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s", recvBuff + token[i + 1].start);
            vlanId = atoi(value);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "vlanMemberMask") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s", recvBuff + token[i + 1].start);
            vlanMemberMask = atoi(value);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "isEnable") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s", recvBuff + token[i + 1].start);
            isEnable = atoi(value);
            i++;
        }
    }

    if (isEnable)
    {
        inArgs.disallowIPFrag           = BFALSE;
        inArgs.forceUntaggedEgressMask  = 0U;
        inArgs.limitIPNxtHdr            = BFALSE;
        inArgs.noLearnMask              = 0U;
        inArgs.regMcastFloodMask        = vlanMemberMask;
        inArgs.unregMcastFloodMask      = 0U;
        inArgs.vidIngressCheck          = BFALSE;
        inArgs.vlanMemberList           = vlanMemberMask;
        inArgs.vlanIdInfo.vlanId        = vlanId;
        inArgs.vlanIdInfo.tagType = ENET_VLAN_TAG_TYPE_INNER;
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &aleEntryIdx);

        status = Enet_ioctl(gEnetCfgServerObj.hEnet,
                            gEnetCfgServerObj.coreId,
                            CPSW_ALE_IOCTL_ADD_VLAN,
                            &prms);
        if (status != ENET_SOK)
        {
            getVlanInArgs.vlanId        = vlanId;
            getVlanInArgs.tagType = ENET_VLAN_TAG_TYPE_INNER;
            ENET_IOCTL_SET_INOUT_ARGS(&prms, &getVlanInArgs, &getVlanOutArgs);

            status = Enet_ioctl(gEnetCfgServerObj.hEnet,
                                gEnetCfgServerObj.coreId,
                                CPSW_ALE_IOCTL_LOOKUP_VLAN,
                                &prms);
            if (status != ENET_SOK)
            {
                memset(&outString[0], 0, sizeof(outString));
                sprintf(outString, "{\"aleEntryIdx\":\"NA\"}"); // Not Added
            }
            else
            {
                memset(&outString[0], 0, sizeof(outString));
                sprintf(outString, "{\"aleEntryIdx\":%d}", getVlanOutArgs.aleEntryIdx); // Already Existed
            }
        }
        else
        {
            memset(&outString[0], 0, sizeof(outString));
            sprintf(outString, "{\"aleEntryIdx\":%d}", aleEntryIdx); // Added normally
        }
    }
    else
    {
        getVlanInArgs.vlanId        = vlanId;
        getVlanInArgs.tagType = ENET_VLAN_TAG_TYPE_INNER;
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &getVlanInArgs, &getVlanOutArgs);

        status = Enet_ioctl(gEnetCfgServerObj.hEnet,
                            gEnetCfgServerObj.coreId,
                            CPSW_ALE_IOCTL_LOOKUP_VLAN,
                            &prms);
        if (status != ENET_SOK)
        {
            memset(&outString[0], 0, sizeof(outString));
            sprintf(outString, "{\"aleEntryIdx\":\"NF\"}"); // Not Found
        }
        else
        {
            ENET_IOCTL_SET_IN_ARGS(&prms, &getVlanInArgs);
            status = Enet_ioctl(gEnetCfgServerObj.hEnet,
                                gEnetCfgServerObj.coreId,
                                CPSW_ALE_IOCTL_REMOVE_VLAN,
                                &prms);
            if (status != ENET_SOK)
            {
                memset(&outString[0], 0, sizeof(outString));
                sprintf(outString, "{\"aleEntryIdx\":\"NR\"}"); // Not Removed
            }
            else
            {
                memset(&outString[0], 0, sizeof(outString));
                sprintf(outString, "{\"aleEntryIdx\":\"R\"}"); // Removed
            }
        }
    }

    memset(&sendBuff[0U], 0, ENET_CONFIG_SERVER_BUFFER_SIZE * sizeof(sendBuff[0]));
    memcpy(&sendBuff[0U], outString, sizeof(outString));
}

static void EnetCfgServer_enabledisableMcast(char *recvBuff,
                                             char *sendBuff)
{
    uint8_t macAddr[6] = {0};
    uint32_t vlanId = 0U, vlanMemberMask = 0U, isEnable = 0U;
    int32_t status, i, tokenCount, maxLen;
    Enet_IoctlPrms prms;
    uint32_t setMcastOutArgs;
    CpswAle_SetMcastEntryInArgs setMcastInArgs;
    CpswAle_GetMcastEntryInArgs getMcastInArgs;
    CpswAle_GetMcastEntryOutArgs getMcastOutArgs;
    char outString[ENET_CONFIG_SERVER_BUFFER_SIZE];

    jsmn_init(&gEnetCfgServerObj.parser);
    tokenCount = jsmn_parse(&gEnetCfgServerObj.parser, recvBuff, ENET_CONFIG_SERVER_BUFFER_SIZE, token, ENET_ARRAYSIZE(token));

    for (i = 1; i < tokenCount; i++)
    {
        char value[ENET_CONFIG_SERVER_TOKEN_SIZE];
        if (jsoneq(recvBuff, &token[i], "addr") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s", recvBuff + token[i + 1].start);
            status = EnetAppUtils_macAddrAtoI(value, &macAddr[0U]);
            EnetAppUtils_assert(status == ENET_SOK);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "vlanId") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s", recvBuff + token[i + 1].start);
            vlanId = atoi(value);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "vlanMemberMask") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s", recvBuff + token[i + 1].start);
            vlanMemberMask = atoi(value);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "isEnable") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s", recvBuff + token[i + 1].start);
            isEnable = atoi(value);
            i++;
        }
    }

    memcpy(&setMcastInArgs.addr.addr[0], macAddr, sizeof(setMcastInArgs.addr.addr));
    setMcastInArgs.addr.vlanId = vlanId;

    if (isEnable)
    {
        setMcastInArgs.info.super      = BFALSE;
        setMcastInArgs.info.fwdState   = CPSW_ALE_FWDSTLVL_FWD_LRN;
        setMcastInArgs.info.portMask   = vlanMemberMask;
        setMcastInArgs.info.numIgnBits = 0U;
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &setMcastInArgs, &setMcastOutArgs);

        status = Enet_ioctl(gEnetCfgServerObj.hEnet,
                            gEnetCfgServerObj.coreId,
                            CPSW_ALE_IOCTL_ADD_MCAST,
                            &prms);
        if (status != ENET_SOK)
        {
            memset(&outString[0], 0, sizeof(outString));
            sprintf(outString, "{\"aleEntryIdx\":\"NA\"}"); // Not Added
        }
        else
        {
            memcpy(&getMcastInArgs.addr.addr[0U], macAddr, sizeof(macAddr));
            getMcastInArgs.addr.vlanId = vlanId;
            getMcastInArgs.numIgnBits  = 0U;
            ENET_IOCTL_SET_INOUT_ARGS(&prms, &getMcastInArgs, &getMcastOutArgs);

            status = Enet_ioctl(gEnetCfgServerObj.hEnet,
                                gEnetCfgServerObj.coreId,
                                CPSW_ALE_IOCTL_LOOKUP_MCAST,
                                &prms);
            if (status == ENET_SOK)
            {
                memset(&outString[0], 0, sizeof(outString));
                sprintf(outString, "{\"aleEntryIdx\":%d}", getMcastOutArgs.aleEntryIdx); // Added
            }
            else
            {
                memset(&outString[0], 0, sizeof(outString));
                sprintf(outString, "{\"aleEntryIdx\":\"NF\"}"); // Not Found
            }
        }
    }
    else
    {
        memcpy(&getMcastInArgs.addr.addr[0U], macAddr, sizeof(macAddr));
        getMcastInArgs.addr.vlanId = vlanId;
        getMcastInArgs.numIgnBits  = 0U;
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &getMcastInArgs, &getMcastOutArgs);

        status = Enet_ioctl(gEnetCfgServerObj.hEnet,
                            gEnetCfgServerObj.coreId,
                            CPSW_ALE_IOCTL_LOOKUP_MCAST,
                            &prms);
        if (status != ENET_SOK)
        {
            memset(&outString[0], 0, sizeof(outString));
            sprintf(outString, "{\"aleEntryIdx\":\"NF\"}");  // Not Found
        }
        else
        {
            CpswAle_MacAddrInfo removeInArgs;
            memcpy(&removeInArgs.addr[0U], &getMcastInArgs.addr.addr[0U], sizeof(macAddr));
            removeInArgs.vlanId = getMcastInArgs.addr.vlanId;

            ENET_IOCTL_SET_IN_ARGS(&prms, &removeInArgs);
            status = Enet_ioctl(gEnetCfgServerObj.hEnet,
                                gEnetCfgServerObj.coreId,
                                CPSW_ALE_IOCTL_REMOVE_ADDR,
                                &prms);
            if (status == ENET_SOK)
            {
                memset(&outString[0], 0, sizeof(outString));
                sprintf(outString, "{\"aleEntryIdx\":\"R\"}"); // Removed
            }
            else
            {
                memset(&outString[0], 0, sizeof(outString));
                sprintf(outString, "{\"aleEntryIdx\":\"NR\"}"); // Not Removed
            }
        }
    }

    memset(&sendBuff[0U], 0, ENET_CONFIG_SERVER_BUFFER_SIZE * sizeof(sendBuff[0]));
    memcpy(&sendBuff[0U], outString, sizeof(outString));
}

static void EnetCfgServer_parseInterVlanRoutingCfg(char *recvBuff,
                                                   EnetCfgServer_InterVlanConfig *pinterVlanCfg)
{
    int32_t i, tokenCount, status, maxLen;

    if (pinterVlanCfg != NULL && recvBuff != NULL)
    {
        jsmn_init(&gEnetCfgServerObj.parser);
        tokenCount = jsmn_parse(&gEnetCfgServerObj.parser, recvBuff, ENET_CONFIG_SERVER_BUFFER_SIZE, token, ENET_ARRAYSIZE(token));
        for (i = 1; i < tokenCount; i++)
        {
            char value[ENET_CONFIG_SERVER_TOKEN_SIZE];
            if (jsoneq(recvBuff, &token[i], "src_mac_addr") == 0)

            {
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       ENET_CONFIG_SERVER_TOKEN_SIZE);
                snprintf(value, maxLen, "%s", recvBuff + token[i + 1].start);
                status = EnetAppUtils_macAddrAtoI(value, &pinterVlanCfg->srcMacAddr[0U]);
                EnetAppUtils_assert(status == ENET_SOK);
                i++;
            }
            if (jsoneq(recvBuff, &token[i], "src_IpAddr") == 0)
            {
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       ENET_CONFIG_SERVER_TOKEN_SIZE);
                snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
                status = EnetAppUtils_ipAddrAtoI(value, &pinterVlanCfg->srcIpv4Addr[0]);
                EnetAppUtils_assert(status == ENET_SOK);
                i++;
            }
            if (jsoneq(recvBuff, &token[i], "ing_vlanId") == 0)
            {
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       ENET_CONFIG_SERVER_TOKEN_SIZE);
                snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
                pinterVlanCfg->ingVlanId = atoi(value);
                i++;
            }

            if (jsoneq(recvBuff, &token[i], "ing_portNum") == 0)
            {
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       ENET_CONFIG_SERVER_TOKEN_SIZE);
                snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
                pinterVlanCfg->ingPortNum = atoi(value);
                i++;
            }

            if (jsoneq(recvBuff, &token[i], "dst_mac_addr") == 0)
            {
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       ENET_CONFIG_SERVER_TOKEN_SIZE);
                snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
                status = EnetAppUtils_macAddrAtoI(value, &pinterVlanCfg->dstMacAddr[0U]);
                EnetAppUtils_assert(status == ENET_SOK);
                i++;
            }
            if (jsoneq(recvBuff, &token[i], "dst_IpAddr") == 0)
            {
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       ENET_CONFIG_SERVER_TOKEN_SIZE);
                snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
                status = EnetAppUtils_ipAddrAtoI(value, &pinterVlanCfg->dstIpv4Addr[0]);
                EnetAppUtils_assert(status == ENET_SOK);
                i++;
            }

            if (jsoneq(recvBuff, &token[i], "egr_vlanId") == 0)
            {
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       ENET_CONFIG_SERVER_TOKEN_SIZE);
                snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
                pinterVlanCfg->egrVlanId = atoi(value);
                i++;
            }

            if (jsoneq(recvBuff, &token[i], "egr_portNum") == 0)
            {
                maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                       ENET_CONFIG_SERVER_TOKEN_SIZE);
                snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
                pinterVlanCfg->egrPortNum = atoi(value);
                i++;
            }
        }
    }
}

static void EnetCfgServer_addIpNxtHdrFilter(char *recvBuff,
                                            char *sendBuff)
{
    uint32_t vlanId = 0U, vlanMemberMask = 0U;
    int32_t status, i, tokenCount, maxLen;
    Enet_IoctlPrms prms;
    CpswAle_VlanEntryInfo inArgs;
    uint32_t outArgs;
    char outString[ENET_CONFIG_SERVER_BUFFER_SIZE];

    jsmn_init(&gEnetCfgServerObj.parser);
    tokenCount = jsmn_parse(&gEnetCfgServerObj.parser, recvBuff, ENET_CONFIG_SERVER_BUFFER_SIZE, token, ENET_ARRAYSIZE(token));

    for (i = 1; i < tokenCount; i++)
    {
        char value[ENET_CONFIG_SERVER_TOKEN_SIZE];
        if (jsoneq(recvBuff, &token[i], "vlanId") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
            vlanId = atoi(value);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "vlanMemberMask") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
            vlanMemberMask = atoi(value);
            i++;
        }
    }

    inArgs.disallowIPFrag           = BFALSE;
    inArgs.forceUntaggedEgressMask  = 0U;
    inArgs.limitIPNxtHdr            = BTRUE;
    inArgs.noLearnMask              = 0U;
    inArgs.regMcastFloodMask        = vlanMemberMask;
    inArgs.unregMcastFloodMask      = 0U;
    inArgs.vidIngressCheck          = BFALSE;
    inArgs.vlanMemberList           = vlanMemberMask;
    inArgs.vlanIdInfo.vlanId        = vlanId;
    inArgs.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &outArgs);

    status = Enet_ioctl(gEnetCfgServerObj.hEnet,
                        gEnetCfgServerObj.coreId,
                        CPSW_ALE_IOCTL_ADD_VLAN,
                        &prms);
    if (status != ENET_SOK)
    {
        /* Entry not added */
        memset(&outString[0], 0, sizeof(outString));
        sprintf(outString, "{\"aleEntryIdx\":\"NA\"}");
    }
}

static void EnetCfgServer_enableRateLimiting(char *recvBuff,
                                             char *sendBuff)
{
    int32_t status, i, tokenCount, maxLen;
    EnetCfgServer_RateLimitingCfg rateLimitingCfg = {0};
    char outString[ENET_CONFIG_SERVER_BUFFER_SIZE];

    memset(&rateLimitingCfg, 0, sizeof(rateLimitingCfg));
    jsmn_init(&gEnetCfgServerObj.parser);
    tokenCount = jsmn_parse(&gEnetCfgServerObj.parser, recvBuff, ENET_CONFIG_SERVER_BUFFER_SIZE, token, ENET_ARRAYSIZE(token));

    for (i = 1; i < tokenCount; i++)
    {
        char value[ENET_CONFIG_SERVER_TOKEN_SIZE];
        if (jsoneq(recvBuff, &token[i], "src_mac_addr") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
            status = EnetAppUtils_macAddrAtoI(value, &rateLimitingCfg.srcMacAddr[0]);
            EnetAppUtils_assert(status == ENET_SOK);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "ing_portNum") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
            rateLimitingCfg.ingPortNum = atoi(value);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "dst_mac_addr") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
            status = EnetAppUtils_macAddrAtoI(value, &rateLimitingCfg.dstMacAddr[0]);
            EnetAppUtils_assert(status == ENET_SOK);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "egr_portNum") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
            rateLimitingCfg.egrPortNum = atoi(value);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "vlan_id") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
            rateLimitingCfg.vlanId = atoi(value);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "PIR") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
            rateLimitingCfg.pir = atoi(value);
            i++;
        }

        if (jsoneq(recvBuff, &token[i], "CIR") == 0)
        {
            maxLen = EnetUtils_min(token[i + 1].end - token[i + 1].start + 1U,
                                   ENET_CONFIG_SERVER_TOKEN_SIZE);
            snprintf(value, maxLen, "%s",recvBuff + token[i + 1].start);
            rateLimitingCfg.cir = atoi(value);
            i++;
        }
    }

    status = EnetCfgServer_addAleEntryRateLimiting(&rateLimitingCfg);

    if (status == ENET_SOK)
    {
        status = EnetCfgServer_setPolicerPriorityRateLimiting(&rateLimitingCfg);
    }

    if (status == ENET_SOK)
    {
        EnetCfgServer_setPolicerGlobalConfig();
    }

    if (status != ENET_SOK)
    {
        /* Entry not added */
        memset(&outString[0], 0, sizeof(outString));
        sprintf(outString, "{\"RateLimitingStatus\":\"Failed\"}");
    }

}

static int32_t EnetCfgServer_addAleEntryRateLimiting(EnetCfgServer_RateLimitingCfg *pRateLimitingCfg)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setUcastOutArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;

    memset(&setUcastInArgs, 0, sizeof(setUcastInArgs));
    memcpy(&setUcastInArgs.addr.addr[0U], &pRateLimitingCfg->dstMacAddr[0U],
           sizeof(setUcastInArgs.addr.addr));
    setUcastInArgs.addr.vlanId  = pRateLimitingCfg->vlanId;
    setUcastInArgs.info.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(pRateLimitingCfg->egrPortNum);
    setUcastInArgs.info.blocked = BFALSE;
    setUcastInArgs.info.secure  = BFALSE;
    setUcastInArgs.info.super   = BFALSE;
    setUcastInArgs.info.ageable = BFALSE;
    setUcastInArgs.info.trunk   = BFALSE;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &setUcastOutArgs);

    status = Enet_ioctl(gEnetCfgServerObj.hEnet, gEnetCfgServerObj.coreId, CPSW_ALE_IOCTL_ADD_UCAST,
                        &prms);

    EnetAppUtils_assert(status == ENET_SOK);

    return status;
}

static int32_t EnetCfgServer_setPolicerPriorityRateLimiting(EnetCfgServer_RateLimitingCfg *pRateLimitingCfg)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_SetPolicerEntryOutArgs setPolicerOutArgs;
    CpswAle_SetPolicerEntryInArgs setPolicerInArgs;
    uint32_t i;

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        setPolicerInArgs.policerMatch.policerMatchEnMask = CPSW_ALE_POLICER_MATCH_PRIORITY;
        setPolicerInArgs.policerMatch.priority               = i;

        setPolicerInArgs.policerMatch.policerMatchEnMask |= CPSW_ALE_POLICER_MATCH_MACSRC;
        memcpy(&setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr[0U], &pRateLimitingCfg->srcMacAddr[0U],
               sizeof(setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr));
        setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.vlanId = pRateLimitingCfg->vlanId;
        setPolicerInArgs.policerMatch.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(pRateLimitingCfg->ingPortNum);

        setPolicerInArgs.threadIdEn         = BFALSE;
        setPolicerInArgs.threadId               = 0U;
        setPolicerInArgs.peakRateInBitsPerSec   = pRateLimitingCfg->pir;
        setPolicerInArgs.commitRateInBitsPerSec = pRateLimitingCfg->cir;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

        status = Enet_ioctl(gEnetCfgServerObj.hEnet, gEnetCfgServerObj.coreId, CPSW_ALE_IOCTL_SET_POLICER,
                            &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("EnetCfgServer_setPolicerPriorityRateLimiting() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                               status);
            break;
        }
    }

    return status;
}

static int32_t EnetCfgServer_setPolicerGlobalConfig(void)
{
    CpswAle_PolicerGlobalCfg policerGlobalCfg;
    Enet_IoctlPrms prms;
    int32_t status;

    /* Get current ALE policer global configuration */
    ENET_IOCTL_SET_OUT_ARGS(&prms, &policerGlobalCfg);

    status = Enet_ioctl(gEnetCfgServerObj.hEnet,
                        gEnetCfgServerObj.coreId,
                        CPSW_ALE_IOCTL_GET_POLICER_GLOBAL_CFG,
                        &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("EnetCfgServer_setPolicerGlobalConfig() failed to get policer global cfg: %d\n", status);
    }

    /* Enable yellow/red drop and update ALE policer global configuration */
    if (status == ENET_SOK)
    {
        policerGlobalCfg.policingEn   = BTRUE;
        policerGlobalCfg.yellowDropEn = BTRUE;
        policerGlobalCfg.redDropEn    = BTRUE;

        ENET_IOCTL_SET_IN_ARGS(&prms, &policerGlobalCfg);

        status = Enet_ioctl(gEnetCfgServerObj.hEnet,
                            gEnetCfgServerObj.coreId,
                            CPSW_ALE_IOCTL_SET_POLICER_GLOBAL_CFG,
                            &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("EnetCfgServer_setPolicerGlobalConfig() failed to set policer global cfg: %d\n", status);
        }
    }

    return status;
}
