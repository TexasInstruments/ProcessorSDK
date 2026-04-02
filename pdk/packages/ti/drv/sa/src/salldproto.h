#ifndef _SALLDPROTO_H
#define _SALLDPROTO_H
/*******************************************************************************
 * FILE PURPOSE: Provide Protocol related defintions
 *
 ********************************************************************************
 * FILE NAME: 	salldproto.h
 *
 * DESCRIPTION: Define the protocol related data structures, MICRO and constands
 *              used by the Security Accelerator (SA)	
 *
 * REVISION HISTORY:
 *
 * (C) Copyright 2009 Texas Instruments, Inc.
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
/******************************************************************************
 * IP Version 4 Protocol Definitions
 ******************************************************************************/
/* IPV4 byte offsets to fields */
#define IPV4_BYTE_OFFSET_VER_HLEN       0
#define IPV4_BYTE_OFFSET_TOS            1
#define IPV4_BYTE_OFFSET_LEN            2
#define IPV4_BYTE_OFFSET_ID             4
#define IPV4_BYTE_OFFSET_FLAGS_FRAGO    6
#define IPV4_BYTE_OFFSET_TTL            8
#define IPV4_BYTE_OFFSET_PROTO          9
#define IPV4_BYTE_OFFSET_HDR_CHKSUM     10
#define IPV4_BYTE_OFFSET_SRC_ADDR       12
#define IPV4_BYTE_OFFSET_DEST_ADDR      16

/* IPV4 definitions */
#define IPV4_VER_MASK                   0xF0
#define IPV4_VER_VALUE                  0x40
#define IPV4_HLEN_MASK                  0x0F
#define IPV4_HLEN_SHIFT                 0
#define IPV4_FRAGO_MASK                 0x1FFF
#define IPV4_FLAGS_MF_MASK              0x2000
#define IPV4_FLAGS_DF_MASK              0x4000
#define IPV4_FLAG_DET_MASK              (IPV4_FLAGS_MF_MASK | \
                                         IPV4_FLAGS_DF_MASK)
#define IPV4_FRAG_DET_VALUE             0x0000
#define IPV4_PROTO_UDP                  0x11
#define IPV4_ADDR_MULTICAST_MASK        0xF0000000ul
#define IPV4_ADDR_MULTICAST_VALUE       0xE0000000ul
#define IPV4_ADDR_BROADCAST_VALUE       0xFFFFFFFFul

#define IPV4_HDR_MIN_SIZE_BYTES         20

#define IP_READ_VER(x)       UTL_GET_BITFIELD(*((x) + IPV4_BYTE_OFFSET_VER_HLEN),4,4)
#define IPV4_READ_IHL(x)     UTL_GET_BITFIELD(*((x) + IPV4_BYTE_OFFSET_VER_HLEN),0,4)

/* IPV4 Option defintions */
#define IPV4_OPTIONS_BYTE_OFFSET_TYPE       0  
#define IPV4_OPTIONS_BYTE_OFFSET_LEN        1
#define IPV4_OPTIONS_BYTE_OFFSET_DATA       2

#define IPV4_OPTIONS_TYPE_COPY_MASK         0x80
#define IPV4_OPTIONS_TYPE_COPY_SHIFT           7
#define IPV4_OPTIONS_TYPE_CLASS_MASK        0x60
#define IPV4_OPTIONS_TYPE_CLASS_SHIFT          5
#define IPV4_OPTIONS_TYPE_NUM_MASK          0x1F
#define IPV4_OPTIONS_TYPE_NUM_SHIFT            5
 
#define IPV4_OPTIONS_GET_TYPE_NUM(x)            ((x) & IPV4_OPTIONS_TYPE_NUM_MASK)
#define IPV4_OPTIONS_MK_TYPE(copy, class, num)  (((copy) << IPV4_OPTIONS_TYPE_COPY_SHIFT)   | \
                                                ((class) << IPV4_OPTIONS_TYPE_CLASS_SHIFT) | \
                                                ((num) & IPV4_OPTIONS_TYPE_NUM_MASK)) 

#define IPV4_OPTIONS_TYPE_END_OPTION            0
#define IPV4_OPTIONS_TYPE_LOOSE_SRC_ROUTE       0x83
#define IPV4_OPTIONS_TYPE_STRICT_SRC_ROUTE      0x89
#define IPV4_OPTIONS_SRC_ROUTE_BYTE_OFFSET_PTR  2


/******************************************************************************
 * IP Version 6 Protocol Definitions
 ******************************************************************************/
/* IPV6 byte offsets to fields */
#define IPV6_BYTE_OFFSET_VER_TC           0
#define IPV6_BYTE_OFFSET_TC_FLH           1
#define IPV6_BYTE_OFFSET_FL_ML            2
#define IPV6_BYTE_OFFSET_PLEN             4
#define IPV6_BYTE_OFFSET_PROTO            6
#define IPV6_BYTE_OFFSET_HOP_LIMIT        7
#define IPV6_BYTE_OFFSET_SRC_ADDR         8
#define IPV6_BYTE_OFFSET_DEST_ADDR        24

/* Size of IPV6 basic header */
#define IPV6_HDR_SIZE_BYTES               40

/* IPV6 Definitions  */
#define IPV6_VER_MASK                     0xF0
#define IPV6_VER_VALUE                    0x60
#define IPV6_ADDR_SIZE                    16  

/* Most significant byte check for IPv6 multicast address */
#define IPV6_ADDR_MULTICAST_MSB_OFFSET    0
#define IPV6_ADDR_MULTICAST_MSB           0xFF

/******************************************************************************
 * IP Common Protocol Definitions
 ******************************************************************************/

/* IPv4/IPV6 type identifiers */
#define IP_TYPE_IPV4        4
#define IP_TYPE_IPV6        6

/* Protocol field values (IPV4) / next header (IPV6) */
#define IP_PROTO_IPV6_HOP_BY_HOP    0   /* IPv6 extension header - hop by hop */
#define IP_PROTO_ICMP               1
#define IP_PROTO_IGMP               2 
#define IP_PROTO_GGP                3 
#define IP_PROTO_IP_IN_IP           4   /* IP tunneling */
#define IP_PROTO_TCP                6 
#define IP_PROTO_EGP                8 
#define IP_PROTO_UDP               17 
#define IP_PROTO_IPV6_IN_IPV4      41   /* IP tunneling */
#define IP_PROTO_IPV6_ROUTE        43   /* IPv6 extension header - route */
#define IP_PROTO_IPV6_FRAG         44   /* IPv6 extension header - fragmentation */
#define IP_PROTO_GRE               47
#define IP_PROTO_ESP               50   /* Encapsulating security payload */
#define IP_PROTO_AUTH              51   /* Authentication header (ipv4) */
#define IP_PROTO_IPV6_NO_NEXT      59   /* IPv6 extention header - no next header      */
#define IP_PROTO_IPV6_DEST_OPT     60   /* IPv6 extension header - destination options */
#define IP_PROTO_IPCOMP           108   /* IP Compression Protocol */

/* IPv6 extension header offsets */
#define IPV6_OPT_HEADER_OFFSET_PROTO        0
#define IPV6_OPT_HEADER_OFFSET_LEN          1
#define IPV6_OPT_HEADER_LEN_UNIT_IN_BYTES   8

/* IPv6 hop by hop and destination options */
#define IPV6_EXT_HDR_OPT_OFFSET_TYPE        0
#define IPV6_EXT_HDR_OPT_OFFSET_DATA_LEN    1
#define IPV6_EXT_HDR_OPT_OFFSET_DATA        2
#define IPV6_EXT_HDR_OPT_HDR_SIZE           2

#define IPV6_EXT_HDR_OPT_TYPE_PAD0          0
#define IPV6_EXT_HDR_OPT_TYPE_PADN          1
#define IPV6_EXT_HDR_OPT_TYPE_JUMBO         0xc2

#define IPV6_EXT_HDR_OPT_TYPE_CHNAGE_MASK   0x20

#define IPV6_EXT_HDR_OPT_TYPE_IS_MUTABLE(x) ((x) & IPV6_EXT_HDR_OPT_TYPE_CHNAGE_MASK)

/* IPv6 Routing header definitions */
#define IPV6_OPT_ROUTE_HDR_OFFSET_TYPE          2 
#define IPV6_OPT_ROUTE_HDR_OFFSET_SEG_LEFT      3
#define IPV6_OPT_ROUTE_HDR_OFFSET_TYPE0_ADDR    8 

#define IPV6_OPT_ROUTE_HDR_TYPE_0          0 

/* Fixed length IPv6 extension header options */
#define IPV6_OPT_FRAG_EXTENSION_LEN_BYTES  8

/******************************************************************************
 * UDP Protocol Definitions
 ******************************************************************************/
/* UDP byte offsets to fields */
#define UDP_BYTE_OFFSET_SRC_PORT       0
#define UDP_BYTE_OFFSET_DEST_PORT      2
#define UDP_BYTE_OFFSET_LEN            4
#define UDP_BYTE_OFFSET_CKSUM          6

#define UDP_HDR_SIZE_BYTES             8

/******************************************************************************
 * RTP Protocol Definitions
 ******************************************************************************/
/* RTP byte offsets to fields */
#define RTP_BYTE_OFFSET_VPXCC          0    /* V(2)|P(1)|X(1)|CC(4) */
#define RTP_BYTE_OFFSET_M_PT           1    /* M(1)|PT(7) */
#define RTP_BYTE_OFFSET_SEQ_NUM        2
#define RTP_BYTE_OFFSET_TIMESTAMP      4
#define RTP_BYTE_OFFSET_SSRC           8

#define RTP_HDR_BASIC_SIZE_BYTES       12
#define RTP_GET_HDR_SIZE(x)           (RTP_HDR_BASIC_SIZE_BYTES + (((x) & 0xF) << 2))


/******************************************************************************
 * IPSEC Authentication Header Definitions
 ******************************************************************************/
/* IPSEC AH byte offsets to fields */
#define IPSEC_AH_OFFSET_NEXT_HEADER     0
#define IPSEC_AH_OFFSET_PAYLOAD_LEN     1
#define IPSEC_AH_OFFSET_RESERVED1       2
#define IPSEC_AH_OFFSET_SPI             4
#define IPSEC_AH_OFFSET_SEQ_NUM         8
#define IPSEC_AH_OFFSET_AUTH_DATA      12

#define IPSEC_AH_BASIC_SIZE_BYTES      12


/* 
 *  The IPSEC Authentication Header in Data Structure Format. 
 *
 *  Note: It is defined here for the reference purpose since it works at a Big-Endian processor only.
 *
 */
typedef  struct  SA_IPSEC_AH_tag
{
    uint8_t  next_header;         /*  identifies the next payload after the authentication payload.
                                 *  refer to the IP protocol number defined above */

    uint8_t  auth_data_len;       /* the length of authentication data field in 32-bit words 
                                 * 0: null authentication algorithm
                                 * Note: this value is equal to the total AH length minus 2? */
                                 
    uint16_t reserved;            /* reserved for the future use */

    uint32_t spi;                 /* Security Parameters Index (SPI). 0: no security associated */

    uint32_t seq_num;             /* Sequence Number */
    
    uint8_t  auth_data[1];        /* Place holder for the authentication dtata */

} SA_IPSEC_AH_T;
 
/******************************************************************************
 * IPSEC Encapsulating Security Payload (ESP) Definitions
 ******************************************************************************/
/* IPSEC ESP Header byte offsets to fields */
#define IPSEC_ESP_HDR_OFFSET_SPI         0
#define IPSEC_ESP_HDR_OFFSET_SEQ_NUM     4
#define IPSEC_ESP_HDR_OFFSET_IV          8

#define IPSEC_ESP_HDR_BASIC_SIZE_BYTES   8


/* 
 *  The IPSEC ESP Header in Data Structure Format. 
 *
 *  Note: It is defined here for the reference purpose since it works at a Big-Endian processor only.
 *
 */
typedef  struct  SA_IPSEC_ESP_HDR_tag
{
    uint32_t spi;                 /* Security Parameters Index (SPI). 0: no security associated */

    uint32_t seq_num;             /* Sequence Number */
    
    uint8_t  iv_data[1];          /* Place holder for the initialization vector */

} SA_IPSEC_ESP_HDR_T;


/* IPSEC ESP Tail byte offsets to fields */
#define IPSEC_ESP_TAIL_OFFSET_PADDING_LEN       0
#define IPSEC_ESP_TAIL_OFFSET_NEXT_HEADER       1

#define IPSEC_ESP_TAIL_SIZE_BYTES               2

/* 
 *  The IPSEC ESP Tail in Data Structure Format. 
 *
 *  Note: It is defined here for the reference purpose since it works at a Big-Endian processor only.
 *
 */
typedef  struct  SA_IPSEC_ESP_TAIL_tag
{
    uint8_t  padding_len;         /*  number of bytes that was padded to the payload data */

    uint8_t  next_header;         /*  identifies the next payload after the authentication payload.
                                 *  refer to the IP protocol number defined above */

} SA_IPSEC_ESP_TAIL_T;

/******************************************************************************
 * Air Cipher Header Definitions
 ******************************************************************************/
/* Air Cipher Header byte offsets to fields */
#define AC_AMD_HDR_OFFSET_DC_SEQNUM         0
#define AC_AMD_HDR_OFFSET_SEQNUM_P_HE       1
#define AC_AMD_HDR_DC_MASK                  0x80 
#define AC_AMD_HDR_SEQNUM_H_MASK            0x7F
#define AC_AMD_HDR_SEQNUM_L_MASK            0xF8
#define AC_AMD_HDR_P_MASK                   0x04
#define AC_AMD_HDR_HE_MASK                  0x03

#define AC_AMD_HDR_GET_SEQ_NUM(x)           (((uint16_t)(x[AC_AMD_HDR_OFFSET_DC_SEQNUM] & AC_AMD_HDR_SEQNUM_H_MASK) << 5) |  \
                                             ((uint16_t)(x[AC_AMD_HDR_SEQNUM_L_MASK] & AC_AMD_HDR_SEQNUM_L_MASK) >> 3))   

#define AC_AMD_HDR_SIZE_BYTES               2

#define AC_UMD_HDR_OFFSET_SEQNUM_E          0
#define AC_UMD_HDR_SEQNUM_MASK              0xF7
#define AC_UMD_HDR_SEQNUM_SHIFT             1    
#define AC_UMD_HDR_E_MASK                   0x01

#define AC_UMD_HDR_GET_SEQ_NUM(x)           ((x[AC_UMD_HDR_OFFSET_SEQNUM_E] & AC_UMD_HDR_SEQNUM_MASK) \
                                             >> AC_UMD_HDR_SEQNUM_SHIFT)

#define AC_UMD_HDR_SIZE_BYTES               1



#endif /* _SAPROTO_H */

