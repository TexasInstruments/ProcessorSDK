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

/*!
 * \file ENET_CONFIG_SERVER.h
 *
 * \brief  CPSW Configuration server header file
 */

#ifndef ENET_CONFIG_SERVER_H_
#define ENET_CONFIG_SERVER_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ENET_CONFIG_SERVER_FXN_TBL_SIZE     (20U)
#define ENET_CONFIG_SERVER_TOKEN_SIZE       (100U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct EnetCfgServer_InterVlanConfig_s
{
    uint8_t srcMacAddr[ENET_MAC_ADDR_LEN];

    uint8_t srcIpv4Addr[ENET_IPv4_ADDR_LEN];

    uint32_t ingVlanId;

    uint32_t ingPortNum;

    uint8_t dstMacAddr[ENET_MAC_ADDR_LEN];

    uint8_t dstIpv4Addr[ENET_IPv4_ADDR_LEN];

    uint32_t egrVlanId;

    uint32_t egrPortNum;
}EnetCfgServer_InterVlanConfig;

typedef struct EnetCfgServer_RateLimitingCfg_s
{
    uint32_t ingPortNum;

    uint32_t egrPortNum;

    uint8_t srcMacAddr[ENET_MAC_ADDR_LEN];

    uint8_t dstMacAddr[ENET_MAC_ADDR_LEN];

    uint32_t vlanId;

    uint32_t pir;

    uint32_t cir;
}EnetCfgServer_RateLimitingCfg;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t EnetCfgServer_init(Enet_Type enetType, uint32_t instId);

void    EnetCfgServer_getCpswCfg(Cpsw_Cfg *cpswCfg);

/* IOCTL Function Table */
extern void(*EnetCfgServer_fxn_table[ENET_CONFIG_SERVER_FXN_TBL_SIZE]) (char *recvBuff, char *sendBuff);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* ENET_CONFIG_SERVER_H_ */
