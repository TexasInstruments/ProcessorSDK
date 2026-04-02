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
 *  \file     hostapp_utils.c
 *
 *  \brief    This file contains the implementation of miscellaneous Ethernet
 *            test utilities.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "hostapp_patterns.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* \brief Octets of payload per console row */
#define OCTETS_PER_ROW                  16

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                  Internal Function Definitions                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t HostappUtils_checkPayload(DataFramePayload *payload)
{
    uint16_t len   = ntohs(payload->len);
    uint8_t *ref   = NULL;
    int32_t status = ETH_TEST_PKT_SOK;

    /* Verify the test packet type id */
    switch (payload->type)
    {
        case ETH_TEST_TYPE_PATTERN_1:
            ref = HostappUtils_DataPattern1;
            break;

        case ETH_TEST_TYPE_PATTERN_2:
            ref = HostappUtils_DataPattern2;
            break;

        case ETH_TEST_TYPE_PATTERN_3:
            ref = HostappUtils_DataPattern3;
            break;

        case ETH_TEST_TYPE_PATTERN_4:
            ref = HostappUtils_DataPattern4;
            break;

        default:
            status = ETH_TEST_PKT_ETYPE;
            break;
    }

    /* Check that pattern buffer has enough data to be validated */
    if (ETH_TEST_PKT_SOK == status)
    {
        if (len > (ETH_PAYLOAD_LEN - ETH_TEST_DATA_HDR_LEN))
        {
            status = ETH_TEST_PKT_ESIZE;
        }
    }

    /* Verify packet content */
    if (ETH_TEST_PKT_SOK == status)
    {
        if (0 != memcmp(payload->data, ref, len))
        {
            status = ETH_TEST_PKT_ECONTENT;
        }
    }

    return status;
}

int32_t HostappUtils_checkVlanPayload(VlanDataFramePayload *payload)
{
    return HostappUtils_checkPayload(&payload->payload);
}

int32_t HostappUtils_checkVlanTagAndPayload(VlanDataFramePayload *payload,
                                            uint8_t pcp,
                                            uint16_t vid,
                                            uint16_t etherType)
{
    uint16_t tci   = ntohs(payload->tci);
    int32_t status = ETH_TEST_PKT_SOK;

    /* Check packet's PCP, VID and EtherType */
    if ((pcp != ((tci >> 13) & 0x7)) ||
        (vid != (tci & 0xFFF)) ||
        (etherType != ntohs(payload->etherType)))
    {
        status = ETH_TEST_PKT_ECONTENT;
    }

    if (ETH_TEST_PKT_SOK == status)
    {
        status = HostappUtils_checkPayload(&payload->payload);
    }

    return status;
}

int32_t HostappUtils_fillPayload(DataFramePayload *payload,
                                 uint16_t type,
                                 uint16_t len)
{
    uint8_t *ref   = NULL;
    int32_t status = ETH_TEST_PKT_SOK;

    /* Check that requested type id */
    switch (type)
    {
        case ETH_TEST_TYPE_PATTERN_1:
            ref = HostappUtils_DataPattern1;
            break;

        case ETH_TEST_TYPE_PATTERN_2:
            ref = HostappUtils_DataPattern2;
            break;

        case ETH_TEST_TYPE_PATTERN_3:
            ref = HostappUtils_DataPattern3;
            break;

        case ETH_TEST_TYPE_PATTERN_4:
            ref = HostappUtils_DataPattern4;
            break;

        default:
            status = ETH_TEST_PKT_ETYPE;
            break;
    }

    /* Check that pattern buffer has enough data and copy packet */
    if (ETH_TEST_PKT_SOK == status)
    {
        if ((len >= ETH_TEST_DATA_HDR_LEN) &&
            (len <= ETH_PAYLOAD_LEN))
        {
            len          -= ETH_TEST_DATA_HDR_LEN;
            payload->type = type;
            payload->len  = htons(len);
            memcpy(payload->data, ref, len);
        }
        else
        {
            status = ETH_TEST_PKT_ESIZE;
        }
    }

    return status;
}

int32_t HostappUtils_fillVlanPayload(VlanDataFramePayload *payload,
                                     uint16_t type,
                                     uint16_t len,
                                     uint8_t pcp,
                                     uint16_t vid,
                                     uint16_t etherType)
{
    int32_t status = ETH_TEST_PKT_SOK;

    payload->tci       = htons((pcp << 13) | vid);
    payload->etherType = htons(etherType);

    if (len < (ETH_TEST_DATA_HDR_LEN + ETH_VLAN_TAG_LEN))
    {
        status = ETH_TEST_PKT_ESIZE;
    }

    if (ETH_TEST_PKT_SOK == status)
    {
        len   -= ETH_VLAN_TAG_LEN;
        status = HostappUtils_fillPayload(&payload->payload, type, len);
    }

    return status;
}

void HostappUtils_printFrame(EthFrame *frame,
                             int len)
{
    uint8_t *payload;
    int i;

    HostappUtils_print("Dst addr : %02x:%02x:%02x:%02x:%02x:%02x\n",
                       frame->hdr.dstMac[0] & 0xFF,
                       frame->hdr.dstMac[1] & 0xFF,
                       frame->hdr.dstMac[2] & 0xFF,
                       frame->hdr.dstMac[3] & 0xFF,
                       frame->hdr.dstMac[4] & 0xFF,
                       frame->hdr.dstMac[5] & 0xFF);

    HostappUtils_print("Src addr : %02x:%02x:%02x:%02x:%02x:%02x\n",
                       frame->hdr.srcMac[0] & 0xFF,
                       frame->hdr.srcMac[1] & 0xFF,
                       frame->hdr.srcMac[2] & 0xFF,
                       frame->hdr.srcMac[3] & 0xFF,
                       frame->hdr.srcMac[4] & 0xFF,
                       frame->hdr.srcMac[5] & 0xFF);

    if (frame->hdr.etherType == htons(ETHERTYPE_VLAN_TAG))
    {
        EthVlanFrame *vlanFrame = (EthVlanFrame *)frame;

        HostappUtils_print("TPID     : 0x%04x\n", ntohs(vlanFrame->hdr.tpid) & 0xFFFFU);
        HostappUtils_print("Priority : %d\n", (ntohs(vlanFrame->hdr.tci) & 0xFFFFU) >> 13);
        HostappUtils_print("VLAN Id  : %d\n", ntohs(vlanFrame->hdr.tci) & 0xFFFU);
        HostappUtils_print("EtherType: 0x%04x\n", ntohs(vlanFrame->hdr.etherType) & 0xFFFFU);
        payload = vlanFrame->payload;
        len    -= ETH_VLAN_TAG_LEN;
    }
    else
    {
        HostappUtils_print("EtherType: 0x%04x\n", ntohs(frame->hdr.etherType) & 0xFFFFU);
        payload = frame->payload;
    }

    HostappUtils_print("Payload  : ");
    for (i = 0; i < len; i++)
    {
        HostappUtils_print("0x%02x ", payload[i]);
        if (i && (((i + 1) % OCTETS_PER_ROW) == 0))
        {
            HostappUtils_print("\n           ");
        }
    }

    if (len && ((len % OCTETS_PER_ROW) != 0))
    {
        HostappUtils_print("\n");
    }

    HostappUtils_print("\n");
}
