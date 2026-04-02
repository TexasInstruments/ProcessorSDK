/**
 * @file   emac_hwq.h
 */
/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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

#ifndef EMAC_HWQ_H
#define EMAC_HWQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define EMAC_ICSSG_MGMT_FREE_HWQA_PORT0     ((int32_t)56)
#define EMAC_ICSSG_MGMT_FREE_HWQA_PORT1     ((int32_t)60)

#define EMAC_ICSSG_MGMT_RX_HWQA_PORT0       ((int32_t)58)
#define EMAC_ICSSG_MGMT_RX_HWQA_PORT1       ((int32_t)62)

#define EMAC_ICSSG_MGMT_TX_HWQA_PORT0       ((int32_t)57)
#define EMAC_ICSSG_MGMT_TX_HWQA_PORT1       ((int32_t)61)

#define EMAC_ICSSG_TXTS_FREE_HWQA_PORT0     ((int32_t)40)
#define EMAC_ICSSG_TXTS_FREE_HWQA_PORT1     ((int32_t)41)

#define EMAC_ICSSG_TXTS_RX_HWQA_PORT0       ((int32_t)59)
#define EMAC_ICSSG_TXTS_RX_HWQA_PORT1       ((int32_t)63)

extern void *emac_hwq_pop(int32_t icssg, int32_t qn);
extern void emac_hwq_push(int32_t icssg, int32_t qn, void *p);
extern void *emac_hwq_peek(int32_t icssg, int32_t qn);
extern int32_t emac_hwq_level(int32_t icssg, int32_t qn);


typedef struct EMAC_MGMT_PKT_S{
    uint32_t            link;
    EMAC_IOCTL_CMD_T    payload;
} EMAC_MGMT_PKT_T;

#ifdef __cplusplus
}
#endif

#endif  /* EMAC_HWQ_H */

