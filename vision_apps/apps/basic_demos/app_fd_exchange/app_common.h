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
#if !defined(_APP_COMMON_H_)
#define _APP_COMMON_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <tivx_utils_ipc_ref_xfer.h>

#include <apputils_net.h>

#ifdef __cplusplus
extern "C" {
#endif

#define APP_PRINT(msg, ...)         printf("[%s:%d] "msg, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define APP_UNIX_STRM_PATH_NAME     ("/tmp/linux_fd_exchange")
#define APP_MAX_MSG_SIZE            (sizeof(App_BuffDesc))
#define APP_MAX_MSG_BUFF_SIZE       (APP_MAX_MSG_SIZE)

/* Message types. */
#define APP_MSGTYPE_HELLO_CMD       (1U)
#define APP_MSGTYPE_HELLO_RSP       (1U)

#define APP_MSGTYPE_IMAGE_BUF_CMD   (2U)
#define APP_MSGTYPE_IMAGE_BUF_RSP   (2U)

#define APP_MAX_NUM_REFS            (48U)

#define APP_CMD_STATUS_SUCCESS      (0U)
#define APP_CMD_STATUS_FAILURE      (1U)

#define APP_TEST_PATTERN_INIT       (0x11223344)

/* Hello message structure. */
typedef struct
{
    /** Type of message. */
    uint8_t     msgId;

} App_MsgHdr;

typedef struct
{
    /** Type of message. */
    uint8_t     msgId;

    /** Status of operation. Allowed values:
     *  - APP_CMD_STATUS_SUCCESS
     *  - APP_CMD_STATUS_FAILURE
     */
    uint8_t     status;

} App_GenericRspMsg;

typedef struct
{
    /** Type of message. */
    uint8_t                     msgId;

    /** Flag to indicate if this is the last object. */
    uint8_t                     lastObj;

    tivx_utils_ref_ipc_msg_t    ipcMsg;

} App_BuffDesc;

vx_reference App_Common_MemAllocObject(vx_context context, vx_enum type, uint32_t  aux);

int32_t App_Common_DeAllocImageObjects(vx_reference  ref[], uint32_t numRefs);

#ifdef __cplusplus
}
#endif

#endif /* _APP_COMMON_H_ */

