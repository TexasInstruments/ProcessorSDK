/*
 *  Copyright (c) Texas Instruments Incorporated 2018
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

#if !defined(_COMMON_IPC_H_)
#define _COMMON_IPC_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NET_PRINT(msg, ...)         printf("[%s:%d] "msg, __FUNCTION__, __LINE__, ##__VA_ARGS__)

/* Maximum Simultaneous client connections. */
#define AppUtil_MAX_NET_LISTEN_CNT            5

/* Maximum allowed payload size - 1 MB */
#define APPUTIL_NET_MAX_PAYLOAD_SIZE        (1024 * 1024)

/* Maximum transfer size - 256 KB */
#define APPUTIL_NET_MAX_XFER_SIZE           (256 * 1024)

#define APPUTIL_NET_RET_PENDING             1
#define APPUTIL_NET_RET_SUCCESS             0
#define APPUTIL_NET_RET_FAILURE             (-1)
#define APPUTIL_NET_SOCK_CREATE_ERROR       (APPUTIL_NET_RET_FAILURE-2)
#define APPUTIL_NET_SOCK_BIND_ERROR         (APPUTIL_NET_RET_FAILURE-3)
#define APPUTIL_NET_SOCK_LISTEN_ERROR       (APPUTIL_NET_RET_FAILURE-4)
#define APPUTIL_NET_SOCK_CONNECT_ERROR      (APPUTIL_NET_RET_FAILURE-5)
#define APPUTIL_NET_SOCK_RCV_ERROR          (APPUTIL_NET_RET_FAILURE-6)
#define APPUTIL_NET_INVALID_PAYLOAD_SIZE    (APPUTIL_NET_RET_FAILURE-7)
#define APPUTIL_NET_INVALID_HDR_INFO        (APPUTIL_NET_RET_FAILURE-7)
#define APPUTIL_NET_SOCK_SEND_ERROR         (APPUTIL_NET_RET_FAILURE-9)
#define APPUTIL_NET_SOCK_FCNTL_ERROR        (APPUTIL_NET_RET_FAILURE-10)

typedef void (*AppUtil_netCliHdlr)(void *cntxt, void *auxData);

typedef struct
{
    /** Server state. */
    uint8_t                     state;

    /** Flag to control the server exit. */
    uint8_t                     exitFlag;

    /** Server socket Id. */
    int32_t                     svrSock;

    /** Generic data type to carry port/pathname. */
    char                       *pathName;

    /** Server Processing function. */
    AppUtil_netCliHdlr          cliHdlr;

    /** Number of concurrent clients to handle. */
    uint32_t                    listenCnt;

    /** Application specific auxiliary data. */
    void                       *auxData;

} AppUtil_NetSrvCntxt;

int32_t AppUtil_netCreateSvrSocket(AppUtil_NetSrvCntxt *svrCntxt);
int32_t AppUtil_netUnixConnect(const char  *pathName);
int32_t AppUtil_netWriteUnixSock(int32_t    sockFd,
                                 void      *buff,
                                 int32_t    size,
                                 int32_t    fd[],
                                 uint32_t   numFd);

int32_t AppUtil_netReadUnixSock(int32_t     sockFd,
                                void       *buff,
                                uint32_t    size,
                                int32_t     fd[],
                                uint32_t   *numFd);

void AppUtil_netIterSvr(AppUtil_NetSrvCntxt *svrCntxt);

#ifdef __cplusplus
}
#endif

#endif /* _COMMON_IPC_H_ */

