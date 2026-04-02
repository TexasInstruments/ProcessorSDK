// ******************************************************************************
// * FILE PURPOSE: Protocol header definitions
// ******************************************************************************
// * FILE NAME: pdsp_proto.h
// *
// * DESCRIPTION: Defines values used for ipv4 and ipv6 parsing
// *
// ******************************************************************************
//
//  TEXAS INSTRUMENTS TEXT FILE LICENSE
// 
//   Copyright (c) 2016 Texas Instruments Incorporated
// 
//  All rights reserved not granted herein.
//  
//  Limited License.  
// 
//  Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive 
//  license under copyrights and patents it now or hereafter owns or controls to 
//  make, have made, use, import, offer to sell and sell ("Utilize") this software 
//  subject to the terms herein.  With respect to the foregoing patent license, 
//  such license is granted  solely to the extent that any such patent is necessary 
//  to Utilize the software alone.  The patent license shall not apply to any 
//  combinations which include this software, other than combinations with devices 
//  manufactured by or for TI (“TI Devices”).  No hardware patent is licensed hereunder.
// 
//  Redistributions must preserve existing copyright notices and reproduce this license 
//  (including the above copyright notice and the disclaimer and (if applicable) source 
//  code license limitations below) in the documentation and/or other materials provided 
//  with the distribution.
//  
//  Redistribution and use in binary form, without modification, are permitted provided 
//  that the following conditions are met:
// 	No reverse engineering, decompilation, or disassembly of this software is 
//   permitted with respect to any software provided in binary form.
// 	Any redistribution and use are licensed by TI for use only with TI Devices.
// 	Nothing shall obligate TI to provide you with source code for the software 
//   licensed and provided to you in object code.
//  
//  If software source code is provided to you, modification and redistribution of the 
//  source code are permitted provided that the following conditions are met:
// 	Any redistribution and use of the source code, including any resulting derivative 
//   works, are licensed by TI for use only with TI Devices.
// 	Any redistribution and use of any object code compiled from the source code
//   and any resulting derivative works, are licensed by TI for use only with TI Devices.
// 
//  Neither the name of Texas Instruments Incorporated nor the names of its suppliers 
//  may be used to endorse or promote products derived from this software without 
//  specific prior written permission.
// 
//  DISCLAIMER.
// 
//  THIS SOFTWARE IS PROVIDED BY TI AND TI’S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED 
//  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
//  AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI’S 
//  LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
//  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// 
//

// Ipv4
.struct struct_Ip
    .u8     VerLen
    .u8     Tos
    .u16    TotalLen
    .u16    Id
    .u16    FragOff
    .u8     Ttl
    .u8     Protocol
    .u16    Checksum
    .u32    SrcIp
    .u32    DstIp
.ends

#define IPV4_FRAG_MASK 0x3fff
#define IPV4_VER_MASK  0xf0
#define IPV4_LEN_MASK  0x0f

.struct struct_IpOpt
    .u8     opt
    .u8     len
    .u8     pointer
    .u8     rsvd
.ends

// IPv6
.struct struct_Ip6
    .u8     VerDsHi
    .u8     DsHiFlowIDHi
    .u16    FlowIDLo
    .u16    PayloadLen
    .u8     NextHeader
    .u8     HopLimit
    .u32    SrcIp0
    .u32    SrcIp1
    .u32    SrcIp2
    .u32    SrcIp3
    .u32    DstIp0
    .u32    DstIp1
    .u32    DstIp2
    .u32    DstIp3
.ends

.struct struct_Ip6_1stHalf
    .u8     VerDsHi
    .u8     DsHiFlowIDHi
    .u16    FlowIDLo
    .u16    PayloadLen
    .u8     NextHeader
    .u8     HopLimit
    .u32    SrcIp0
    .u32    SrcIp1
    .u32    SrcIp2
.ends

.struct struct_Ip6_2ndHalf
    .u32    SrcIp3
    .u32    DstIp0
    .u32    DstIp1
    .u32    DstIp2
    .u32    DstIp3
.ends

.struct struct_Ipv6Ext
    .u8  proto
    .u8  hdrLen
    .u8  data1
    .u8  data2
.ends

.struct struct_Ipv6Opt
    .u8  type
    .u8  len
    .u8  data1
    .u8  data2
.ends

.struct struct_Ipv6ExtRt
    .u8  proto
    .u8  hdrLen
    .u8  routeType
    .u8  segsLeft
    .u32 rsvd
.ends


#define IPV6_VER_MASK  0xf0

// IPv4/IPV6 type identifiers 
#define IP_TYPE_IPV4        0x40
#define IP_TYPE_IPV6        0x60

// IPv4 options 
#define IPV4_OPT_MASK                   0x1f
#define IPV4_OPT_END_OF_LIST            0
#define IPV4_OPT_NOP                    1
#define IPV4_OPT_LOOSE_SOURCE_ROUTE     3
#define IPV4_OPT_STRICT_SOURCE_ROUTE    9

// IPV4 Mutable option bitMap
// immutable options: 0, 1, 2, 5, 6, 20 and 21 only
// 0: mutable
// 1: immutable
#define IPV4_OPT_MUTABILITY_BITMAP  0x00300067   

// Protocol field values (IPV4) / next header (IPV6) 
#define IP_PROTO_NEXT_IPV6_HOP_BY_HOP    0   // IPv6 extension header - hop by hop 
#define IP_PROTO_NEXT_IP_IN_IP           4   // IP tunneling 
#define IP_PROTO_NEXT_TCP                6 
#define IP_PROTO_NEXT_UDP               17
#define IP_PROTO_NEXT_IPV6_IN_IPV4      41   // IP tunneling 
#define IP_PROTO_NEXT_IPV6_ROUTE        43   // IPv6 extension header - route 
#define IP_PROTO_NEXT_IPV6_FRAG         44   // IPv6 extension header - fragmentation 
#define IP_PROTO_NEXT_GRE               47
#define IP_PROTO_NEXT_ESP               50   // Encapsulating security payload 
#define IP_PROTO_NEXT_AUTH              51   // Authentication header (ipv4) 
#define IP_PROTO_NEXT_IPV6_NO_NEXT      59   // IPv6 extention header - no next header      
#define IP_PROTO_NEXT_IPV6_DEST_OPT     60   // IPv6 extension header - destination options 
#define IP_PROTO_NEXT_UDP_LITE         136

// IPv6 hop by hop options
#define IPV6_OPT_HOP_BY_HOP_OPT_PAD0    0
#define IPV6_OPT_HOP_BY_HOP_OPT_PADN    1   

#define ipv6_opt_flag_mutable           t5


//******************************************************************************
// * UDP Protocol Definitions
// ******************************************************************************/
// UDP byte offsets to fields 
#define UDP_BYTE_OFFSET_SRC_PORT       0
#define UDP_BYTE_OFFSET_DEST_PORT      2
#define UDP_BYTE_OFFSET_LEN            4
#define UDP_BYTE_OFFSET_CKSUM          6

#define UDP_HDR_SIZE_BYTES             8

//******************************************************************************
// * RTP Protocol Definitions
// ******************************************************************************/
//* RTP byte offsets to fields */
#define RTP_BYTE_OFFSET_VPXCC          0    // V(2)|P(1)|X(1)|CC(4) */
#define RTP_BYTE_OFFSET_M_PT           1    // M(1)|PT(7) */
#define RTP_BYTE_OFFSET_SEQ_NUM        2
#define RTP_BYTE_OFFSET_TIMESTAMP      4
#define RTP_BYTE_OFFSET_SSRC           8

#define RTP_HDR_BASIC_SIZE_BYTES       12
// #define RTP_GET_HDR_SIZE(x)           (RTP_HDR_BASIC_SIZE_BYTES + (((x) & 0xF) << 2))

.struct struct_rtpHdr
    .u8     vpxcc
    .u8     mPt
    .u16    seqNum
    .u32    timestamp
    .u32    ssrc
.ends

#define RTP_HDR_CC_MASK         0x0f



///******************************************************************************
// * IPSEC Authentication Header Definitions
// ******************************************************************************/
///* IPSEC AH byte offsets to fields */
#define IPSEC_AH_OFFSET_NEXT_HEADER     0
#define IPSEC_AH_OFFSET_PAYLOAD_LEN     1
#define IPSEC_AH_OFFSET_SPI             4
#define IPSEC_AH_OFFSET_SEQ_NUM         8
#define IPSEC_AH_OFFSET_AUTH_DATA      12

#define IPSEC_AH_BASIC_SIZE_BYTES      12

///* 
// *  The IPSEC Authentication Header in Data Structure Format. 
// *
// *  Note: It is defined here for the reference purpose since it works at a Big-Endian processor only.
// *
// */
//typedef  struct  SA_IPSEC_AH_tag
//{
//    UINT8  next_header;         /*  identifies the next payload after the authentication payload.
//                                 *  refer to the IP protocol number defined above */
//
//    UINT8  auth_data_len;       /* the length of authentication data field in 32-bit words 
//                                 * 0: null authentication algorithm
//                                 * Note: this value is equal to the total AH length minus 2? */
//                                 
//    UINT16 reserved;            /* reserved for the future use */
//
//    UINT32 spi;                 /* Security Parameters Index (SPI). 0: no security associated */
//
//    UINT32 seq_num;             /* Sequence Number */
//    
//    UINT8  auth_data[1];        /* Place holder for the authentication dtata */
//
//} SA_IPSEC_AH_T;

.struct struct_ahHdr
    .u8     nextHeader
    .u8     authDataLen
    .u16    reserved
    .u32    spi
    .u32    seqNum
    .u32    authData1
    .u32    authData2
    .u32    authData3
    .u32    authData4
.ends 

.struct struct_ahHdrIvAuth
    .u32    iv1
    .u32    iv2
    .u32    authData1
    .u32    authData2
    .u32    authData3
    .u32    authData4
.ends 

.struct struct_ahHdrAuth
    .u32    authData1
    .u32    authData2
    .u32    authData3
    .u32    authData4
.ends 


    
///******************************************************************************
// * IPSEC Encapsulating Security Payload (ESP) Definitions
// ******************************************************************************/
///* IPSEC ESP Header byte offsets to fields */
#define IPSEC_ESP_HDR_OFFSET_SPI         0
#define IPSEC_ESP_HDR_OFFSET_SEQ_NUM     4
#define IPSEC_ESP_HDR_OFFSET_IV          8

#define IPSEC_ESP_HDR_BASIC_SIZE_BYTES   8


///* 
// *  The IPSEC ESP Header in Data Structure Format. 
// *
// *  Note: It is defined here for the reference purpose since it works at a Big-Endian processor only.
// *
// */
//typedef  struct  SA_IPSEC_ESP_HDR_tag
//{
//    UINT32 spi;                 /* Security Parameters Index (SPI). 0: no security associated */
//
//    UINT32 seq_num;             /* Sequence Number */
//    
//    UINT8  iv_data[1];          /* Place holder for the initialization vector */
//
//} SA_IPSEC_ESP_HDR_T;
//

.struct struct_espHdr
    .u32    spi
    .u32    seqNum
    .u32    iv1
    .u32    iv2
    .u32    iv3
    .u32    iv4
.ends    

// IPSEC ESP Tail byte offsets to fields */
#define IPSEC_ESP_TAIL_OFFSET_PADDING_LEN       0
#define IPSEC_ESP_TAIL_OFFSET_NEXT_HEADER       1

#define IPSEC_ESP_TAIL_SIZE_BYTES               2

///* 
// *  The IPSEC ESP Tail in Data Structure Format. 
// *
// *  Note: It is defined here for the reference purpose since it works at a Big-Endian processor only.
// *
// */
//typedef  struct  SA_IPSEC_ESP_TAIL_tag
//{
//    UINT8  padding_len;         /*  number of bytes that was padded to the payload data */
//
//    UINT8  next_header;         /*  identifies the next payload after the authentication payload.
//                                 *  refer to the IP protocol number defined above */
//
//} SA_IPSEC_ESP_TAIL_T;

.struct struct_espTail
    .u8 paddingLen
    .u8 nextHeader
.ends    

///******************************************************************************
// * Air Cipher Header Definitions
// ******************************************************************************/
///* Air Cipher Header byte offsets to fields */
#define AC_AMD_HDR_OFFSET_DC_SEQNUM         0
#define AC_AMD_HDR_OFFSET_SEQNUM_P_HE       1
#define AC_AMD_HDR_DC_MASK                  0x80 
#define AC_AMD_HDR_SEQNUM_H_MASK            0x7F
#define AC_AMD_HDR_SEQNUM_L_MASK            0xF8
#define AC_AMD_HDR_P_MASK                   0x04
#define AC_AMD_HDR_HE_MASK                  0x03

//#define AC_AMD_HDR_GET_SEQ_NUM(x)           (((UINT16)(x[AC_AMD_HDR_OFFSET_DC_SEQNUM] & AC_AMD_HDR_SEQNUM_H_MASK) << 5) |  \
//                                             ((UINT16)(x[AC_AMD_HDR_SEQNUM_L_MASK] & AC_AMD_HDR_SEQNUM_L_MASK) >> 3))   

#define AC_AMD_HDR_SIZE_BYTES               2

#define AC_UMD_HDR_OFFSET_SEQNUM_E          0
#define AC_UMD_HDR_SEQNUM_MASK              0xF7
#define AC_UMD_HDR_SEQNUM_SHIFT             1    
#define AC_UMD_HDR_E_MASK                   0x01

//#define AC_UMD_HDR_GET_SEQ_NUM(x)           ((x[AC_UMD_HDR_OFFSET_SEQNUM_E] & AC_UMD_HDR_SEQNUM_MASK) \
//                                             >> AC_UMD_HDR_SEQNUM_SHIFT)

#define AC_UMD_HDR_SIZE_BYTES               1


