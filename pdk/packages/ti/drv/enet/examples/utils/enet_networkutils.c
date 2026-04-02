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
 *  \file enet_networkutils.c
 *
 *  \brief File containing the Network functions.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>

/* OSAL Header files */
#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/SemaphoreP.h>

/* CSL Header files */
#include <ti/csl/csl_types.h>

#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
/* Module Header files */
#include "include/enet_networkutils.h"

#if (defined(FREERTOS) || defined(SAFERTOS))
/* LWIP Dependencies */
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/def.h>
#else
/* NDK Dependencies */
#include <ti/ndk/inc/socket.h>
#include <ti/ndk/inc/socketndk.h>
#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/stkmain.h>
#endif

/* ========================================================================== */
/*                          Extern Function Declarations                      */
/* ========================================================================== */

#if !(defined (FREERTOS) || defined(SAFERTOS))

/* The following declarations are part of socketndk.h, but still there are some
 * build errors. As a temporary fix following declarations are added here.
 */
extern int NDK_send(SOCKET s,
                    void *pbuf,
                    int size,
                    int flags);

extern int NDK_recv(SOCKET s,
                    void *pbuf,
                    int size,
                    int flags);

extern SOCKET NDK_socket(int domain,
                         int type,
                         int protocol);

extern int NDK_setsockopt(SOCKET s,
                          int level,
                          int op,
                          void *pbuf,
                          int bufsize);

extern int NDK_bind(SOCKET s,
                    struct sockaddr *pName,
                    int len);

extern int NDK_listen(SOCKET s,
                      int maxcon);

extern SOCKET NDK_accept(SOCKET s,
                         struct sockaddr *pName,
                         int *plen);

extern int fdClose(void *hFd);

extern int fdOpenSession(void *hOwner);

extern void fdCloseSession(void *hTask);

extern int fdPoll(FDPOLLITEM items[],
                  uint32_t itemcnt,
                  int32_t timeout);
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ENET_NETWORKUTILS_INVALID_SOCKET              (0xFFFFFFFFU)
#define ENET_NETWORKUTILS_TX_SERVER_CLOSED            (0U)
#define ENET_NETWORKUTILS_TX_SERVER_LISTEN            (1U)
#define ENET_NETWORKUTILS_TX_SERVER_CONNECTED         (2U)
#define ENET_NETWORKUTILS_TX_SERVER_POLL_TIMEOUT      (10U)
/* Maximum number of connects to queue */
#define ENET_NETWORKUTILS_MAX_CONNECTS                (5)
/* Maximum number of socket instances */
#define ENET_NETWORKUTILS_MAX_SOCKET_INSTANCES        (2)

#if (defined(FREERTOS) || defined(SAFERTOS))
#define EnetNetworkUtils_Socket         int
#define EnetNetworkUtils_FdPoll         struct pollfd

#define EnetNetworkUtils_socket         lwip_socket
#define EnetNetworkUtils_setsockopt     lwip_setsockopt
#define EnetNetworkUtils_send           lwip_send
#define EnetNetworkUtils_recv           lwip_recv
#define EnetNetworkUtils_bind           lwip_bind
#define EnetNetworkUtils_listen         lwip_listen
#define EnetNetworkUtils_accept         lwip_accept
#define EnetNetworkUtils_fdClose        lwip_close
#define EnetNetworkUtils_fdPoll         lwip_poll
#define EnetNetworkUtils_fdOpenSession  NULL
#define EnetNetworkUtils_fdCloseSession NULL
#define ENET_NETWORKUTILS_SOCKET_ERROR  -1
#else
#define EnetNetworkUtils_Socket         SOCKET
#define EnetNetworkUtils_FdPoll         FDPOLLITEM
#define EnetNetworkUtils_socket         NDK_socket
#define EnetNetworkUtils_setsockopt     NDK_setsockopt
#define EnetNetworkUtils_send           NDK_send
#define EnetNetworkUtils_recv           NDK_recv
#define EnetNetworkUtils_bind           NDK_bind
#define EnetNetworkUtils_listen         NDK_listen
#define EnetNetworkUtils_accept         NDK_accept
#define EnetNetworkUtils_fdClose        fdClose
#define EnetNetworkUtils_fdPoll         fdPoll
#define EnetNetworkUtils_fdOpenSession  fdOpenSession
#define EnetNetworkUtils_fdCloseSession fdCloseSession
#define ENET_NETWORKUTILS_SOCKET_ERROR  SOCKET_ERROR
#define htons                           NDK_htons
#define ntohs                           NDK_ntohs
#define htonl                           NDK_htonl
#define ntohl                           NDK_ntohl
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    EnetNetworkUtils_Socket sockFd;
    /**< Server socket handle */

    EnetNetworkUtils_Socket connectedSockFd;
    /**< socket handle of client that is connected */

    uint32_t port;
    /**< port on which server is listening */

    EnetNetworkUtils_FdPoll pollitem[1U];
    /**< Polling structure to use with EnetNetworkUtils_FdPoll() */

    uint32_t state;
    /**< State of server socket */

    bool used;
    /**< the instance is in use or not */
} EnetNetworkUtils_SockObj;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t EnetNetworkUtils_open(EnetNetworkUtils_SockObj *pObj,
                                     uint32_t port);

static int32_t EnetNetworkUtils_waitConnect(EnetNetworkUtils_SockObj *pObj,
                                            uint32_t timeout);

static int32_t EnetNetworkUtils_read(EnetNetworkUtils_SockObj *pObj,
                                     uint8_t *dataBuf,
                                     uint32_t *dataSize);

static int32_t EnetNetworkUtils_write(EnetNetworkUtils_SockObj *pObj,
                                      uint8_t *dataBuf,
                                      uint32_t dataSize);

static int32_t EnetNetworkUtils_openSession(void *handle);

static int32_t EnetNetworkUtils_sessionClose(void *handle);

static int32_t EnetNetworkUtils_waitConnection(EnetNetworkUtils_SockObj *pObj);

static int32_t EnetNetworkUtils_close(EnetNetworkUtils_SockObj *pObj,
                                      bool closeServerSock);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static EnetNetworkUtils_SockObj gNetRxSockObj[ENET_NETWORKUTILS_MAX_SOCKET_INSTANCES];
static uint32_t gNetRxSockObjUseCount = 0;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void* EnetNetworkUtils_createSocket(uint32_t port)
{
    int32_t status = ENET_NETWORKUTILS_SUCCESS;
    EnetNetworkUtils_SockObj *netRxSockObj;
    void *handle = NULL;
    int i = 0;

    /* Pick up the internal static memory block */
    netRxSockObj = (EnetNetworkUtils_SockObj *) &gNetRxSockObj[0];

    if (gNetRxSockObjUseCount == 0U) 
    {
        (void)memset( (void *)gNetRxSockObj,0,sizeof(gNetRxSockObj));
    }
    for (i = 0; i < ENET_NETWORKUTILS_MAX_SOCKET_INSTANCES; i++)
    {
        if (netRxSockObj[i].used == BFALSE)
        {
            netRxSockObj[i].used = BTRUE;
            gNetRxSockObjUseCount++;
            break;
        }
    }

    if (i < ENET_NETWORKUTILS_MAX_SOCKET_INSTANCES)
    {
        /* Grab the object */
        handle = (EnetNetworkUtils_SockObj *)&netRxSockObj[i];

        EnetNetworkUtils_openSession(NULL);

        status = EnetNetworkUtils_open(&netRxSockObj[i], port);

        if (status != ENET_NETWORKUTILS_SUCCESS)
        {
            EnetAppUtils_print("Network_open failed !!!\n");
            handle = NULL;
            netRxSockObj[i].used = BFALSE;
        }
        else
        {
            netRxSockObj[i].state = ENET_NETWORKUTILS_TX_SERVER_LISTEN;
        }
    }
    else
    {
        EnetAppUtils_print("Enet network utils objects are exahused.. failed !!!\n");
        handle = NULL;
    }

    return handle;
}

int32_t EnetNetworkUtils_deleteSocket(void* handle)
{
    EnetNetworkUtils_SockObj *pObj = (EnetNetworkUtils_SockObj *)handle;

    if (pObj != NULL)
    {
        EnetNetworkUtils_close(pObj, BTRUE);

        EnetNetworkUtils_sessionClose(NULL);

        pObj->state = ENET_NETWORKUTILS_TX_SERVER_CLOSED;
        pObj->used  = BFALSE;

        EnetAppUtils_print("Network Utils: Server Closed (port=%d) !!!\n",
                       pObj->port);
    }

    return 0U;
}

int32_t EnetNetworkUtils_writePayload(void* handle,
                                      uint8_t *bufAddr,
                                      uint32_t bufSize)
{
    int32_t status = ENET_NETWORKUTILS_SUCCESS;
    EnetNetworkUtils_SockObj *pObj = (EnetNetworkUtils_SockObj *)handle;

    if (pObj == NULL)
    {
        EnetAppUtils_print("Bad network utils object handle passed.. failed !!!\n");
        status = ENET_NETWORKUTILS_ERROR;
    }
    else
    {
        status = EnetNetworkUtils_waitConnection(pObj);

        if (pObj->state == ENET_NETWORKUTILS_TX_SERVER_CONNECTED)
        {
            /* write header to client, if write failed, then client
             * disconnected, so go back to listening
             */
            status = EnetNetworkUtils_write(pObj,
                                            bufAddr,
                                            bufSize);

            if (status != ENET_NETWORKUTILS_SUCCESS)
            {
                EnetNetworkUtils_close(pObj, BFALSE);

                pObj->state = ENET_NETWORKUTILS_TX_SERVER_LISTEN;
            }
        }
    }

    return status;
}

int32_t EnetNetworkUtils_readPayload(void* handle,
                                     uint8_t *bufAddr,
                                     uint32_t bufSize)
{
    int32_t status = ENET_NETWORKUTILS_SUCCESS;
    EnetNetworkUtils_SockObj *pObj = (EnetNetworkUtils_SockObj *)handle;

    if (pObj == NULL)
    {
        EnetAppUtils_print("Bad network utils object handle passed.. failed !!!\n");
        status = ENET_NETWORKUTILS_ERROR;
    }
    else
    {
        status = EnetNetworkUtils_waitConnection(pObj);

        if (pObj->state == ENET_NETWORKUTILS_TX_SERVER_CONNECTED)
        {
            /* write header to client, if write failed, then client
             * disconnected, so go back to listening
             */
            status = EnetNetworkUtils_read(pObj,
                                           bufAddr,
                                           &bufSize);

            if (status != 0)
            {
                EnetNetworkUtils_close(pObj, BFALSE);

                pObj->state = ENET_NETWORKUTILS_TX_SERVER_LISTEN;
            }
        }

#if (defined(DEBUG))
        EnetAppUtils_print("NETWORK_RX: NETWORK_RX: BUF%d: %d bytes recevied (port=%d)!!!\n",
                       i,
                       bufSize[i],
                       pObj->createArgs.networkServerPort
                       );
#endif
    }

    return status;
}

/* ========================================================================== */
/*                          Static Function Definitions                       */
/* ========================================================================== */

static int32_t EnetNetworkUtils_waitConnection(EnetNetworkUtils_SockObj *pObj)
{
    int32_t status = ENET_NETWORKUTILS_SUCCESS;
    int32_t isConnected;

    if (pObj->state == ENET_NETWORKUTILS_TX_SERVER_LISTEN)
    {
        isConnected = EnetNetworkUtils_waitConnect(pObj,
                                                   ENET_NETWORKUTILS_TX_SERVER_POLL_TIMEOUT);

        if (ENET_NETWORKUTILS_SUCCESS == isConnected)
        {
            /* connected to client */
            pObj->state = ENET_NETWORKUTILS_TX_SERVER_CONNECTED;
        }
        else
        {
            status = ENET_NETWORKUTILS_ERROR;
        }
    }

    return status;
}

static int32_t EnetNetworkUtils_read(EnetNetworkUtils_SockObj *pObj,
                                     uint8_t *dataBuf,
                                     uint32_t *dataSize)
{
    int32_t status = ENET_NETWORKUTILS_SUCCESS;

    uint32_t tmpDataSize;
    int32_t actDataSize = 0;

    tmpDataSize = *dataSize;

    while (tmpDataSize > 0U)
    {
        actDataSize = EnetNetworkUtils_recv(pObj->connectedSockFd, (void *)dataBuf, tmpDataSize, 0);
        if (actDataSize <= 0)
        {
            *dataSize = 0U;
            status    = ENET_NETWORKUTILS_ERROR;
            break;
        }
        else
        {
            dataBuf     += actDataSize;
            tmpDataSize -= (uint32_t)actDataSize;
        }
    }

    return status;
}

static int32_t EnetNetworkUtils_write(EnetNetworkUtils_SockObj *pObj,
                                      uint8_t *dataBuf,
                                      uint32_t dataSize)
{
    int32_t status = ENET_NETWORKUTILS_SUCCESS;

    int32_t actDataSize = 0;

    while (dataSize > 0)
    {
        actDataSize = (int32_t)EnetNetworkUtils_send(pObj->connectedSockFd, dataBuf, (size_t)dataSize, 0);

        if (actDataSize <= 0)
        {
            break;
        }
        else
        {
            dataBuf  += actDataSize;
            dataSize -= (uint32_t)actDataSize;
        }
    }

    if (dataSize > 0)
    {
        status = ENET_NETWORKUTILS_ERROR;
    }

    return status;
}

static int32_t EnetNetworkUtils_open(EnetNetworkUtils_SockObj *pObj,
                                     uint32_t port)
{
    int32_t status = ENET_NETWORKUTILS_SUCCESS;

    struct sockaddr_in sin1;
    int32_t option = 1;

    pObj->connectedSockFd = (EnetNetworkUtils_Socket)ENET_NETWORKUTILS_INVALID_SOCKET;
    pObj->port            = port;
    pObj->sockFd          = (EnetNetworkUtils_Socket)EnetNetworkUtils_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (pObj->sockFd == (EnetNetworkUtils_Socket)ENET_NETWORKUTILS_INVALID_SOCKET)
    {
        EnetAppUtils_print("Network Utils: Unable to open socket (port=%d)!!!\n", port);
        status = ENET_NETWORKUTILS_ERROR;
    }
    else
    {
        /* Bind to the specified Server port */
        memset(&sin1, 0U, sizeof(struct sockaddr_in));
        sin1.sin_family      = AF_INET;
        sin1.sin_addr.s_addr = INADDR_ANY;
        sin1.sin_port        = (uint16_t)(htons((uint16_t)(pObj->port)));

        EnetNetworkUtils_setsockopt(pObj->sockFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

        if (EnetNetworkUtils_bind(pObj->sockFd, (struct sockaddr *)&sin1, sizeof(sin1)) < 0)
        {
            EnetAppUtils_print("Network Utils: Unable to bind() (port=%d) !!!\n", port);
            EnetNetworkUtils_fdClose(pObj->sockFd);
            pObj->sockFd = (EnetNetworkUtils_Socket)ENET_NETWORKUTILS_INVALID_SOCKET;
            status       = ENET_NETWORKUTILS_ERROR;
        }
        else
        {
            if (EnetNetworkUtils_listen(pObj->sockFd, ENET_NETWORKUTILS_MAX_CONNECTS) < 0)
            {
                EnetNetworkUtils_fdClose(pObj->sockFd);
                pObj->sockFd = (EnetNetworkUtils_Socket)ENET_NETWORKUTILS_INVALID_SOCKET;
                status       = ENET_NETWORKUTILS_ERROR;
            }
        }
    }

    return status;
}

static int32_t EnetNetworkUtils_close(EnetNetworkUtils_SockObj *pObj,
                                      bool closeServerSock)
{
    if (pObj->connectedSockFd != (EnetNetworkUtils_Socket)ENET_NETWORKUTILS_INVALID_SOCKET)
    {
        EnetNetworkUtils_fdClose(pObj->connectedSockFd);
        pObj->connectedSockFd = (EnetNetworkUtils_Socket)ENET_NETWORKUTILS_INVALID_SOCKET;
    }

    if (closeServerSock)
    {
        if (pObj->sockFd != (EnetNetworkUtils_Socket)ENET_NETWORKUTILS_INVALID_SOCKET)
        {
            EnetNetworkUtils_fdClose(pObj->sockFd);
            pObj->sockFd = (EnetNetworkUtils_Socket)ENET_NETWORKUTILS_INVALID_SOCKET;
        }
    }

    return 0;
}

static int32_t EnetNetworkUtils_waitConnect(EnetNetworkUtils_SockObj *pObj,
                                            uint32_t timeout)
{
    int32_t status = ENET_NETWORKUTILS_SUCCESS;

    pObj->pollitem[0].fd              = pObj->sockFd;
#if (defined(FREERTOS) || defined(SAFERTOS))
    pObj->pollitem[0].events = POLLIN;
#else
    pObj->pollitem[0].eventsRequested = 0x1 /* POLLIN */;
#endif

    if (EnetNetworkUtils_fdPoll(pObj->pollitem, 1U, timeout) == ENET_NETWORKUTILS_SOCKET_ERROR)
    {
        EnetAppUtils_print("Network Utils: fdPoll() failed with ENET_NETWORKUTILS_SOCKET_ERROR (port=%d) !!!\n", pObj->port);
        status = ENET_NETWORKUTILS_ERROR;
    }
    else
    {
#if (defined(FREERTOS) || defined(SAFERTOS))
        if (pObj->pollitem[0].revents == 0)
#else
        if (pObj->pollitem[0].eventsDetected == 0)
#endif
        {
            /* NO connection, retry */
            status = ENET_NETWORKUTILS_SUCCESS;
        }
        else
        {
#if (defined(FREERTOS) || defined(SAFERTOS))
            if (pObj->pollitem[0].revents & POLLNVAL)
#else
            if ((uint32_t)(pObj->pollitem[0].eventsDetected) & (uint32_t)0x08 /* POLLNVAL */)
#endif
            {
                EnetAppUtils_print("Network Utils: fdPoll() failed with POLLNVAL (port=%d) !!!\n", pObj->port);
                status = ENET_NETWORKUTILS_ERROR;
            }
            else
            {
#if (defined(FREERTOS) || defined(SAFERTOS))
                if (pObj->pollitem[0].revents & POLLIN)
#else
                if ((uint32_t)(pObj->pollitem[0].eventsDetected) & (uint32_t)0x1)
#endif
                {
                    pObj->connectedSockFd = (EnetNetworkUtils_Socket)EnetNetworkUtils_accept(pObj->sockFd, 0, 0);

                    if (pObj->connectedSockFd != (EnetNetworkUtils_Socket)ENET_NETWORKUTILS_INVALID_SOCKET)
                    {
                        status = 1;
                    }
                    else
                    {
                        EnetAppUtils_print("Not able to connect to PC client !!!\n");
                    }
                }
            }
        }
    }

    /* NO connection, retry */
    return status;
}

static int32_t EnetNetworkUtils_openSession(void *handle)
{
#if !(defined(FREERTOS) || defined(SAFERTOS))
    if (handle == NULL)
    {
        handle = TaskSelf();
    }

    /* Allocate the file environment for this task */
    EnetNetworkUtils_fdOpenSession(handle);
#endif

    return 0;
}

static int32_t EnetNetworkUtils_sessionClose(void *handle)
{
#if !(defined(FREERTOS) || defined(SAFERTOS))
    if (handle == NULL)
    {
        handle = TaskSelf();
    }

    EnetNetworkUtils_fdCloseSession(handle);
#endif

    return 0;
}

/* end of file */
