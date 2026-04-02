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
#include <apputils_net.h>

#define APPUTIL_MAX_NUM_FD  128

int32_t
AppUtil_netReadUnixSock(int32_t     sockFd,
                        void       *buff,
                        uint32_t    size,
                        int32_t     fd[],
                        uint32_t   *numFd)
{
    struct msghdr   msg;
    union {
        struct cmsghdr cm;
        uint8_t pktinfo_sizer[sizeof(struct cmsghdr) + sizeof(int32_t)*APPUTIL_MAX_NUM_FD];
    } control_un;
    struct iovec    iov[1];
    int32_t         status;

    msg.msg_name    = NULL;
    msg.msg_namelen = 0;

    iov[0].iov_base = buff;
    iov[0].iov_len  = size;

    msg.msg_iov    = iov;
    msg.msg_iovlen = 1;

    if (numFd)
    {
        msg.msg_control    = &control_un;
        msg.msg_controllen = sizeof(control_un);
    }
    else
    {
        msg.msg_control    = NULL;
        msg.msg_controllen = 0;
    }

    status = recvmsg(sockFd, &msg, 0);

    if (status > 0)
    {
        /* Check if any file descriptors were transferred, along with the
         * payload.
         */
        if (numFd)
        {
            struct cmsghdr *cmsg;
            int32_t        *fdPtr;

            *numFd = 0;

            cmsg = CMSG_FIRSTHDR(&msg);

            if (cmsg != NULL)
            {
                int32_t     len;
                uint32_t    i;

                /* We do not know exactly how many descriptors are being sent.
                 * We can derive that from the length of the command buffer,
                 * taking care of any padding.
                 */
                /* Get the true length of the payload. */
                len = cmsg->cmsg_len -
                      CMSG_LEN(sizeof(int32_t)) +
                      sizeof(uint32_t);

                /* payloadLen/sizeof(int32_t) should give us the number of
                 * descriptors sent.
                 */
                *numFd = len/sizeof(int32_t);

                /* Extract the descriptor Ids. */
                fdPtr = (int32_t *)CMSG_DATA(cmsg);

                for (i = 0; i < *numFd; i++)
                {
                    fd[i] = fdPtr[i];
                }
            }
        }

        status = APPUTIL_NET_RET_SUCCESS;
    }
    else if (status == 0)
    {
        /* The peer has closed the connection. */
        status = APPUTIL_NET_SOCK_RCV_ERROR;
        NET_PRINT("Peer closed the connection.\n");
    }
    else
    {
        status = APPUTIL_NET_SOCK_RCV_ERROR;
        NET_PRINT("recvmsg() failed\n");
        perror(__FUNCTION__);
    }

    return status;
}

int32_t
AppUtil_netWriteUnixSock(int32_t    sockFd,
                         void      *buff,
                         int32_t    size,
                         int32_t    fd[],
                         uint32_t   numFd)
{
    struct msghdr   msg;
    struct iovec    iov[1];
    char            buf[CMSG_SPACE(sizeof(int32_t)*APPUTIL_MAX_NUM_FD)];
    int32_t         status = APPUTIL_NET_RET_SUCCESS;

    iov[0].iov_base = buff;
    iov[0].iov_len  = size;
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov     = iov;
    msg.msg_iovlen  = 1;

    /* Check if any file descriptors need to be transferred, along with the
     * payload.
     */
    if (numFd)
    {
        struct cmsghdr *cmsg;
        int32_t        *fdPtr;

        msg.msg_control    = buf;
        msg.msg_controllen = CMSG_SPACE(sizeof(int32_t) * numFd);
        cmsg               = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level   = SOL_SOCKET;
        cmsg->cmsg_type    = SCM_RIGHTS;
        cmsg->cmsg_len     = CMSG_LEN(sizeof(int32_t) * numFd);
        fdPtr              = (int32_t *)CMSG_DATA(cmsg);

        memcpy(fdPtr, fd, numFd * sizeof(int32_t));
    }
    else
    {
        msg.msg_control    = NULL;
        msg.msg_controllen = 0;
    }

    status = sendmsg(sockFd, &msg, 0);

    if (status > 0)
    {
        status = APPUTIL_NET_RET_SUCCESS;
    }
    else
    {
        NET_PRINT("sendmsg() failed\n");
        perror(__FUNCTION__);
    }

    return status;
}

int32_t
AppUtil_netUnixConnect(const char  *pathName)
{
    struct sockaddr_un  svrAddr;
    int32_t             status;
    int32_t             sockId;

    status = APPUTIL_NET_RET_SUCCESS;
    sockId = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sockId < 0)
    {
        NET_PRINT("socket() failed\n");
        perror(__FUNCTION__);
        status = APPUTIL_NET_SOCK_CREATE_ERROR;
    }

    if (status == 0)
    {
        memset(&svrAddr, 0, sizeof(struct sockaddr_un));
        svrAddr.sun_family = AF_UNIX;
        strncpy(svrAddr.sun_path, pathName, sizeof(svrAddr.sun_path)-1);

        status = connect(sockId,
                         (struct sockaddr *)&svrAddr,
                         sizeof(svrAddr));

        if (status < 0)
        {
            NET_PRINT("connect() failed\n");
            perror(__FUNCTION__);
            status = APPUTIL_NET_SOCK_CONNECT_ERROR;
        }
    }

    if ((status != APPUTIL_NET_RET_SUCCESS) && (sockId >= 0))
    {
        close(sockId);
        sockId = -1;
    }

    return sockId;
}

int32_t
AppUtil_netCreateSvrSocket(AppUtil_NetSrvCntxt *svrCntxt)
{
    struct sockaddr_un  unet;
    int32_t             sockId;
    int32_t             sockFamily;
    int32_t             sockType;
    int32_t             protocol;
    int32_t             svrAddrSize;
    int32_t             status;

    status = APPUTIL_NET_RET_SUCCESS;
    sockId = -1;

    memset(&unet, 0, sizeof(struct sockaddr_un));

    sockFamily        = AF_UNIX;
    sockType          = SOCK_STREAM;
    protocol          = 0;
    svrAddrSize       = sizeof(struct sockaddr_un);
    unet.sun_family  = AF_UNIX;

    strncpy(unet.sun_path, svrCntxt->pathName, sizeof(unet.sun_path)-1);
    unlink(svrCntxt->pathName);

    /* Open a socket. */
    sockId = socket(sockFamily, sockType, protocol);

    if (sockId < 0)
    {
        NET_PRINT("socket() failed\n");
        perror(__FUNCTION__);
        status = APPUTIL_NET_SOCK_CREATE_ERROR;
    }

    /* Bind the socket. */
    if (status == APPUTIL_NET_RET_SUCCESS)
    {
        status = bind(sockId, (struct sockaddr*)&unet, svrAddrSize);

        if (status < 0)
        {
            NET_PRINT("bind() failed\n");
            perror(__FUNCTION__);
            status = APPUTIL_NET_SOCK_BIND_ERROR;
        }
    }

    if (status == APPUTIL_NET_RET_SUCCESS)
    {
        status = listen(sockId, svrCntxt->listenCnt);

        if (status < 0)
        {
            NET_PRINT("listen failed\n");
            status = APPUTIL_NET_SOCK_LISTEN_ERROR;
        }
    }

    if ((status != APPUTIL_NET_RET_SUCCESS) && (sockId >= 0))
    {
        close(sockId);
        sockId = -1;
    }

    svrCntxt->svrSock = sockId;

    return sockId;

}

void
AppUtil_netIterSvr(AppUtil_NetSrvCntxt *svrCntxt)
{
    while (svrCntxt->exitFlag == 0)
    {
        struct sockaddr svrAddr;
        socklen_t       addrLen;
        int32_t         cliSock;

        addrLen = sizeof(struct sockaddr);

        cliSock = accept(svrCntxt->svrSock,
                         (struct sockaddr *)&svrAddr,
                         &addrLen);

        if (cliSock < 0)
        {
            NET_PRINT("accept() failed\n");
            break;
        }

        /* Handle the incoming connection. */
        svrCntxt->cliHdlr(svrCntxt->auxData, &cliSock);

        close(cliSock);

    }

    return;
}

