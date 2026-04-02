// *******************************************************************************
// * FILE PURPOSE: Provide Security Accelerator (SA) Packet Header Processor (PHP)
// *               Security Context related defintions
// *
// ********************************************************************************
// * FILE NAME:   pdsp_ctx.h
// *
// * DESCRIPTION: Provide the Security Context related data structures, constants
// *              and MACROs used by the Packet Header Processor (PHP) module in the 
// *              Security Accelerator (SA) for all supported operation modes
// *
// *     0                   1                   2                   3
// *     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ -----
// *   |     Flags     |  F/E control  |           SCID                |
// *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Hardware Control
// *   |                 SCPTR (Security Context Pointer               |
// *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ------
// *   |D|  Pkt Type   |  Flow Index   |   Dest Queue ID               | Software Control               
// *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ------
// *   |                                                               |
// *   |                                                               |
// *   |                                                               |
// *   |                Protocol Specific Parameters                   |
// *   |                (Variable Size up to 116 bytes                 |
// *   ...                                                           ...
// *   |                                                               |
// *   |                                                               |
// *   |                                                               |
// *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// *
// *                    Figure: PHP Security Context Format
// *
// *   
// *
// *
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
//
//
//*****************************************************************************
// * Type:        SA_CTX_PKT_TYPE_T        
// ******************************************************************************
// * Description: This type represents the various packet types to be processed
// *              by the SA. It is used to identify the corresponding PHP 
// *              processing function. 
// *****************************************************************************/
// typedef UINT8 SA_CTX_PKT_TYPE_T; 
#define SA_CTX_PKT_TYPE_3GPP_AIR    0    // 3GPP Air Cipher 
#define SA_CTX_PKT_TYPE_SRTP        1    // SRTP 
#define SA_CTX_PKT_TYPE_IPSEC_AH    2    // IPSec Authentication Header 
#define SA_CTX_PKT_TYPE_IPSEC_ESP   3    // IPSec Encapsulating Security Payload 
#define SA_CTX_PKT_TYPE_DATA_MODE   4    // Data Mode (Null Protocol) 
#define SA_CTX_MAX_PKT_TYPE         4

//******************************************************************************
// * Type:        SA_CTX_COMMON_PARAMS_T  
// ******************************************************************************
// * Description: This structure defines the SA Security Context parameters
// *              which are common to all protocols.
// *              
// *****************************************************************************/
//typedef struct SA_CTX_COMMON_PARAMS_tag
//{
//    /* 8-bytes security context control word (SCCTL) used by hardware/firmware/software */
//    UINT8   ctrl_flags; /* Various control flags */
//    /*
//     *  Bit 0-6: Reserved
//     *  Bit 7:   Owner bit 0: Host 1: cp_ace 
//     *           Host can only set this bit, cp_ace can only clear this bit
//     *           PHP firmware may varify this bit to see whether the security
//     *           context is valid
//     */   
//    #define SA_CTX_SCCTL_SET_OWNER(a,b)    UTL_SET_BITFIELD((a)->ctrl_flags, b, 7, 1)
//    #define SA_CTX_SCCTL_GET_OWNER(a)      UTL_GET_BITFIELD((a)->ctrl_flags, 7, 1)
//    
//    UINT8   dma_info;  /* Fetch/Evict related parameter which details the
//                        * sections within security context that need to be
//                        * fetched /envicted */
//    /*
//     *  Bit 0-1: Fetch PHP Bytes
//     *  Bit 2-3: Fetch Encryption/Air Ciphering Bytes
//     *  Bit 4-5: Fetch Authentication Bytes
//     *  Bit 6-7: Evict PHP Bytes
//     *
//     *  where   00 = 0 bytes
//     *          01 = 64 bytes
//     *          10 = 96 bytes
//     *          11 = 128 bytes
//     */
//    #define SA_CTX_DMA_SIZE_0       0
//    #define SA_CTX_DMA_SIZE_64      1
//    #define SA_CTX_DMA_SIZE_96      2
//    #define SA_CTX_DMA_SIZE_128     3
//    
//    #define SA_CTX_SCCTL_MK_DMA_INFO(php_f, enc, auth, php_e)       ((php_f)        |  \
//                                                                     ((enc) << 2)   |  \
//                                                                     ((auth) << 4)  |  \
//                                                                     ((php_e) << 6))  
//                         
//    UINT16  ctx_id;    /* Security Context ID, filled by Hardware */
//    UINT32  ctx_ptr;   /* Security Context Pointer, filled by Hardware */
//    
//    /*
//     * Packet information type
//     * 7:7 Packet Direction: 0: Tx (To Netwrk) (3GPP: From Air (uplink))
//     *                       1: Rx (From Network) (3GPP: To Air (downlink)
//     * 6:0 Packet Type as specified above 
//     */  
//    #define SA_CTX_GET_PKT_DIR(a)       UTL_GET_BITFIELD((a)->pkt_info_byte, 7, 1)       
//    #define SA_CTX_SET_PKT_DIR(a, b)    UTL_SET_BITFIELD((a)->pkt_info_byte, b, 7, 1)
//    #define SA_CTX_GET_PKT_TYPE(a)      UTL_GET_BITFIELD((a)->pkt_info_byte, 0, 7)
//    #define SA_CTX_SET_PKT_TYPE(a, b)   UTL_SET_BITFIELD((a)->pkt_info_byte, b, 0, 7)
//    UINT8                   pkt_info_byte;  
//    UINT8                   flow_index;     /* Destination flow index */                                     
//    UINT16                  dest_queue_id;  /* Destination Queue ID */
//    UINT8                   thread_id;
//    UINT8                   reserved1;
//    UINT16                  reserved2;  
//    UINT32                  swInfo0;      /* channel identifier: to be written to the swInfo0 in the packet descriptor */
//}   SA_CTX_COMMON_PARAMS_T;
//
//

.struct struct_ctxComm
    .u8     ctrlFlag
#define ctxOwner        t7
    .u8     dmaInfo
    .u16    ctxId           // Security Context ID
    .u32    ctxPtr          // Security Context Pointer
    .u8     pktInfo         
#define flagPktDir          t7 
#define CTX_PKT_DIR_BIT     7
#define CTX_PKT_DIR_RX      0x80
#define flagUseLocDMA       t6
#define CTX_PKT_TYPE_MASK   0x3F
    .u8     flowIndex   
    .u16    destQueueId
    .u32    swInfo0
    .u32    swInfo1
    .u8     instIndex       // Store the packet instance index */
    .u8     rsvd 
.ends     

///******************************************************************************
// * Type:        SA_REPLAY_CTL_T        
// ******************************************************************************
// * Description: This structure specifies the parameters of the IPSEC 
// *              replay control blocks
// *
// * Note: Some parameters may be initialized by SW
// *****************************************************************************/
//#define SA_MAX_REPLAY_WINDOW_SIZE       128
//#define SA_WIN_MASK_SIZE  DIV_ROUND_UP(SA_MAX_REPLAY_WINDOW_SIZE, 32) + 1 
//typedef struct SA_REPLAY_CTL_tag
//{
//    UINT8   win_mask_index;             /* Mask index of window base */
//    UINT8   win_mask_bitoff;            /* Bit offset from "index" to the window base */
//    UINT8   win_size;                   /* replay window size */
//    UINT8   reserved1;                  /* for memory alignment only */
//    UINT32  win_mask[SA_WIN_MASK_SIZE]; /* Bitmask Array  */
//    UINT32  win_base_hi;                /* Upper 32-bit of the win_base when ESN is enabled */
//    UINT32  win_base;                   /* Packet ID of window base (lowest index) */
//} SA_REPLAY_CTL_T; 
//
//typedef UINT8  SA_REPLAY_RC_T;

#define SA_REPLAY_RC_OK     0     // Packet is in window and not previously seen */
#define SA_REPLAY_RC_DUP    1     // Packet is in window and previously seen */
#define SA_REPLAY_RC_OLD    2     // Packet is older than window */
#define SA_REPLAY_RC_NEW    3     // Packet is newer than window */

#define MAX_WINMASK_BYTE_LEN   20
#define MAX_WINMASK_BYTE_LEN2  132  // Large replay windows (1K + 32) 


.struct struct_replayCtrl
    .u8     winMaskIndex
    .u8     winMaskBitoff
    .u16    winSize
    .u32    winMask0
    .u32    winMask1
    .u32    winMask2
    .u32    winMask3
    .u32    winMask4
    .u32    winBaseHi
    .u32    winBase
.ends

///******************************************************************************
// * Type:        SA_CTX_PROTO_IPSEC_ESP_TX_T        
// ******************************************************************************
// * Description: This structure specifies the IPSEC ESP Tx protocol specific   
// *              Security context parameters which are initialized by SW and used  
// *              by Firmware.
// *
// * Note: The parameters which may be updated by the firmware should be arranged
// *       within the first 52 bytes so that the size of the evicted PHP parameters
// *       is limited to 64 bytes
// *
// *****************************************************************************/
//typedef struct SA_CTX_PROTO_IPSEC_ESP_TX_tag
//{
//    UINT32 num_tx_pkts;         /* statistics: TBD */
//    UINT32 num_tx_pkts_hi;      /* statistics: TBD */
//    
//    SA_IPSEC_ESN_T  esn;        /* extended sequence number of the last tx packet */
//    
//
//    UINT8 ctrl_bitfield;        /* various IPSEC ESP control information */            
//                                    
//    /*
//     *  Bit 0-1: Transport Type (Tunnel, Transport)  (not used)
//     *  Bit 2:   ESN flag        1: Extended sequence number is used                               
//     *  Bit 3-7: Reserved 
//     */   
//    #define SA_CTX_PROTO_IPSEC_ESP_TX_SET_TRANSPORT_TYPE(a,b)      UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 0, 2)
//    #define SA_CTX_PROTO_IPSEC_ESP_TX_GET_TRANSPORT_TYPE(a)        UTL_GET_BITFIELD((a)->ctrl_bitfield, 0, 2)
//    #define SA_CTX_PROTO_IPSEC_ESP_TX_SET_ESN(a,b)                 UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 2, 1)
//    #define SA_CTX_PROTO_IPSEC_ESP_TX_ESN_ENABLED(a)               UTL_GET_BITFIELD((a)->ctrl_bitfield, 2, 1)
//
//    UINT8  pkt_inst_index;      /* Store the packet instance index */
//                                /* Note: It may be part of the packet info
//                                 *       or allocated at SOP
//                                 */  
//
//
//    UINT8   icv_size;            /* Size of the hash data 0: indicate no authentication tag */
//    UINT8   iv_size;             /* Initialization vector size in bytes */  
//    
//    UINT16  reserved1;            
//    
//    UINT8   first_eng_id;        /* Specify the engine id for the first command */
//    UINT8   cmdl_total_len;      /* Specify the size of the multiple command label */
//    UINT32  spi;           /* Security Parameters Index */
//    UINT8   salt[SA_IPSEC_MAX_SALT_SIZE];  /* Record the session salt key */
//    UINT8   iv[SA_IPSEC_MAX_IV_SIZE];
//    UINT8   aux1[SA_IPSEC_MAX_AUX_SIZE];   /* Auxiliary storage space for certain 
//    
//    
//} SA_CTX_PROTO_IPSEC_ESP_TX_T;   /* 48 (+32) bytes */
   
.struct struct_ctxESPTx
    .u32    numTxPkts
    .u32    numTxPktsHi
    .u32    byteCount
    .u32    byteCountHi
    .u32    rollOverCounter
    .u32    esnLo            // Extended Sequence Number
    .u32    esnHi
    .u8     ctrlInfo
#define     flagESPTxRandomIV         t7
#define     flagESPTxNullEnc          t6
#define     flagESPTxAESCTR           t5
#define     flagESPTxCMAC             t4
#define     flagESPTxUseEnc           t3
#define     flagESPTxESN              t2
#define     flagESPTxEngPair2         t1
#define     ESP_TX_TRANSPORT_MASK     0x03
    .u8     opMode     // operation mode
    .u8     icvSize
    .u8     ivSize
    .u32    spi
    .u32    salt
    .u8     firstEngId
    .u8     cmdlTotalLen
.ends

.struct struct_ctxESPTx1
    .u32    esnLo            // Extended Sequence Number
    .u32    esnHi
    .u8     ctrlInfo
    .u8     opMode     // operation mode
    .u8     icvSize
    .u8     ivSize
    .u32    spi
    .u32    salt
    .u8     firstEngId
    .u8     cmdlTotalLen
.ends

.struct struct_ctxESPTxStats
    .u32    numTxPkts
    .u32    numTxPktsHi
    .u32    byteCount
    .u32    byteCountHi
    .u32    rollOverCounter
.ends
///******************************************************************************
// * Type:        SA_CTX_PROTO_IPSEC_ESP_RX_T        
// ******************************************************************************
// * Description: This structure specifies the IPSEC ESP Rx protocol specific   
// *              security context parameters which are initialized by SW and used  
// *              by Firmware.
// *
// * Note: The parameters which may be updated by the firmware should be arranged
// *       within the first 52 bytes so that the size of the evicted PHP parameters
// *       is limited to 64 bytes
// *
// *
// *****************************************************************************/
//typedef struct SA_CTX_PROTO_IPSEC_ESP_RX_tag
//{
//    SA_REPLAY_CTL_T replay_ctl;  /* Replay control block */
//
//    UINT32   num_rx_pkts;        /* statistics: TBD */
//    UINT32   num_rx_pkts_hi;     /* statistics: TBD */
//    UINT32   num_dup_pkts;
//    UINT32   num_old_pkts;
//    UINT32   num_hash_fails;
//    
//    UINT8 ctrl_bitfield;       /* various IPSEC ESP control information */            
//                                    
//    /*
//     *  Bit 0: rsvd (not used)
//     *  Bit 1: flag for second pair of engines
//     *  Bit 2:   ESN flag        1: Extended sequence number is used                               
//     *  Bit 3:   Replay Flag     1: replay check enabled
//     *  Bit 3-7:Reserved 
//     */   
//    #define SA_CTX_PROTO_IPSEC_ESP_RX_SET_TRANSPORT_TYPE(a,b)      UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 0, 2)
//    #define SA_CTX_PROTO_IPSEC_ESP_RX_GET_TRANSPORT_TYPE(a)        UTL_GET_BITFIELD((a)->ctrl_bitfield, 0, 2)
//    #define SA_CTX_PROTO_IPSEC_ESP_RX_SET_ESN(a,b)                 UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 2, 1)
//    #define SA_CTX_PROTO_IPSEC_ESP_RX_ESN_ENABLED(a)               UTL_GET_BITFIELD((a)->ctrl_bitfield, 2, 1)
//    #define SA_CTX_PROTO_IPSEC_ESP_RX_SET_REPLAY(a,b)              UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 3, 1)
//    #define SA_CTX_PROTO_IPSEC_ESP_RX_REPLAY_ENABLED(a)            UTL_GET_BITFIELD((a)->ctrl_bitfield, 3, 1)
//    
//    UINT8  pkt_inst_index;        /* Store the packet instance index */
//                                  /* Note: It may be part of the packet info
//                                   *       or allocated at SOP
//                                   */  
//    
//    UINT8   icv_size;             /* size of the hash data 0: indicate no authentication */
//    UINT8   iv_size;              /* size of the initialization vector */
//    
//    UINT16  reserved2;
//    
//    UINT8   first_eng_id;         /* Specify the engine id for the first command */
//    UINT8   cmdl_total_len;       /* Specify the size of the multiple command label */
//    SA_CMDL_CFG_ENTRY_T     entry[SA_CMDL_MAX_ENTRIES];  /* command label configuration array */
//    UINT32  spi;                  /* Security Parameters Index */
//      
//} SA_CTX_PROTO_IPSEC_ESP_RX_T;  /* 72 bytes */

.struct struct_ctxESPRx
    .u8     winMaskIndex
    .u8     winMaskBitoff
    .u8     winSize
    .u8     ctrlFlag
    .u32    winMask0
    .u32    winMask1
    .u32    winMask2
    .u32    winMask3
    .u32    winMask4
    .u32    winBaseHi
    .u32    winBase
    .u32    numRxPkts
    .u32    numRxPktsHi
    .u32    numDupPkts
    .u32    numOldPkts
    .u32    numHashFails
    .u32    byteCount
    .u32    byteCountHi
    .u8     ctrlInfo
#define     flagESPRxReplay   t7    
#define     flagESPRxNullEnc  t6
#define     flagESPRxAESCTR   t5
#define     flagESPRxCMAC     t4
#define     flagESPRxUseEnc   t3
#define     flagESPRxESN      t2
#define     flagESPRxEngPair2 t1
#define     ESP_RX_TRANSPORT_MASK     0x03
    .u8     opMode     // operation mode
    .u8     icvSize
    .u8     ivSize
    .u32    spi
    .u32    salt
    .u8     firstEngId
    .u8     cmdlTotalLen
    .u32    aux1
.ends

.struct struct_ctxESPRxStats
    .u32    numRxPkts
    .u32    numRxPktsHi
    .u32    numDupPkts
    .u32    numOldPkts
    .u32    numHashFails
    .u32    byteCount
    .u32    byteCountHi
.ends

// struct_replayCtrl
 
.struct struct_ctxESPRx1
    .u8     ctrlInfo
    .u8     opMode     // operation mode
    .u8     icvSize
    .u8     ivSize
    .u32    spi
    .u32    salt
    .u8     firstEngId
    .u8     cmdlTotalLen
.ends

// Combine statistics and replay control for ESN update
.struct struct_ctxESPRxStatsReplay
    .u32    winBaseHi 
    .u32    winBase
    .u32    numRxPkts
    .u32    numRxPktsHi
    .u32    numDupPkts
    .u32    numOldPkts
    .u32    numHashFails
.ends

///******************************************************************************
// * Type:        SA_CTX_PROTO_IPSEC_AH_TX_T        
// ******************************************************************************
// * Description: This structure specifies the IPSEC AH Tx protocol specific Security 
// *              context parameters which are initialized by SW and used by 
// *              Firmware.
// *
// * Note: The parameters which may be updated by the firmware should be arranged
// *       within the first 52 bytes so that the size of the evicted PHP parameters
// *       is limited to 64 bytes
// *
// *
// *****************************************************************************/
//typedef struct SA_CTX_PROTO_IPSEC_AH_TX_tag
//{
//    UINT32 num_tx_pkts;    /* statistics: TBD */
//    UINT32 num_tx_pkts_hi; /* statistics: TBD */
//    SA_IPSEC_ESN_T  esn;   /* extended sequence number of the last tx packet */
//    
//    
//    UINT8 ctrl_bitfield;  /* various IPSEC AH control information */            
//                                    
//    /*
//     *  Bit 0-1: Transport Type (Tunnel, Transport) (not used)
//     *  Bit 2:   ESN flag
//     *  Bit 3-15: Reserved 
//     */   
//    #define SA_CTX_PROTO_IPSEC_AH_TX_SET_TRANSPORT_TYPE(a,b)  UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 0, 2)
//    #define SA_CTX_PROTO_IPSEC_AH_TX_GET_TRANSPORT_TYPE(a)    UTL_GET_BITFIELD((a)->ctrl_bitfield, 0, 2)
//    #define SA_CTX_PROTO_IPSEC_AH_TX_SET_ESN(a, b)            UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 2, 1)
//    #define SA_CTX_PROTO_IPSEC_AH_TX_ESN_ENABLED(a)           UTL_GET_BITFIELD((a)->ctrl_bitfield, 2, 1)
//    
//    UINT8  pkt_inst_index; /* Store the packet instance index */
//                           /* Note: It may be part of the packet info
//                            *       or allocated at SOP
//                            */  
//
//    UINT8   iv_size;       /* size of the initialization vector 0: no IV required */  
//    UINT8   icv_size;      /* size of the hash data 0: indicate no authentication */
//    UINT16  reserved; 
//    UINT8   first_eng_id;  /* Specify the engine id for the first command */
//    UINT8   cmdl_total_len;/* Specify the size of the multiple command label */
//    UINT32  spi;           /* Security Parameters Index */
//    SA_CMDL_CFG_ENTRY_T   entry[SA_CMDL_MAX_ENTRIES];  /* command label configuration array */
//    
//} SA_CTX_PROTO_IPSEC_AH_TX_T;   /* 48 bytes */
//
.struct struct_ctxAHTx
    .u32    numTxPkts
    .u32    numTxPktsHi
    .u32    byteCount
    .u32    byteCountHi
    .u32    rollOverCounter
    .u32    esnLo            // Extended Sequence Number
    .u32    esnHi
    .u8     ctrlInfo
#define     flagAHTxCMAC             t4
#define     flagAHTxUseEnc           t3
#define     flagAHTxESN              t2
#define     AH_TX_TRANSPORT_MASK     0x03
    .u8     opMode     // operation mode
    .u8     icvSize
    .u8     ivSize
    //.u16    reserved1
    .u32    spi
    .u32    salt
    .u8     firstEngId
    .u8     cmdlTotalLen
    .u32    aux1
.ends

.struct struct_ctxAHTx1
    .u32    esnLo            // Extended Sequence Number
    .u32    esnHi
    .u8     ctrlInfo
    .u8     opMode     // operation mode
    .u8     icvSize
    .u8     ivSize
    //.u16    reserved1
    .u32    spi
    .u32    salt
    .u8     firstEngId
    .u8     cmdlTotalLen
.ends

.struct struct_ctxAHTxStats
    .u32    numTxPkts
    .u32    numTxPktsHi
    .u32    byteCount
    .u32    byteCountHi
    .u32    rollOverCounter
.ends


//#define SA_CMDL_MK_OPT_CTRL(offset, len)    ((offset) + ((len) >> 3))  

#define SA_ENC_KEY_OFFSET         0x20
#define SA_ENC_KEY2_OFFSET        0x30
#define SA_ENC_AUX1_OFFSET        0x40
#define SA_ENC_AUX1_2_OFFSET      0x50
#define SA_ENC_AUX2_OFFSET        0x60
#define SA_ENC_AUX3_OFFSET        0x70  
#define SA_ENC_AUX4_OFFSET        0x80 

///* GCM Operation related definitions */
#define SA_GCM_CMDL_SIZE                    48    
//                                                  /* GCM Command Label Including
//                                                   * - Basic Parameters (8 byte)
//                                                   * - Length C         (8 byte)
//                                                   * - AAD (Additional Authentication Data) (16 byte)
//                                                   * - AES-CTR IV       (16 byte)
//                                                   */
//#define SA_GCM_ENC_SC_SIZE                  SA_CTX_ENC_TYPE2_SIZE /* Including Key and Aux1 */
//#define SA_GCM_IV_SIZE                      16    /* AES-CTR IV */
//#define SA_GCM_ENG_ID                       SA_CMDL_ENGINE_ID_ES1                                                     
//
//#define SA_GCM_ENC_AUX1_LENA_OFFSET         23    /* Aux1 location to store the GCM Length A */
#define SA_GCM_ESP_AAD_LEN1                  8    
//                                                  /* GCM IPSEC Length A with ESN Disabled */
//                                                  /* AAD: SPI + SN */
#define SA_GCM_ESP_AAD_LEN2                 12    
//                                                  /* GCM IPSEC Length A with ESN Enabled */ 
//                                                  /* AAD: SPI + ESN */ 
//#define SA_GCM_CMDL_OPT1                    SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX1_OFFSET + 24), 8)
#define SA_GCM_CMDL_OPT1                      0x59  
//#define SA_GCM_CMDL_OPT2                    SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX2_OFFSET), 16)
#define SA_GCM_CMDL_OPT2                      0x62
//#define SA_GCM_CMDL_OPT3                    SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX3_OFFSET), 16)
#define SA_GCM_CMDL_OPT3                      0x72
//                               
//                                                  
///* CCM Operation related definitions */
#define SA_CCM_CMDL_SIZE                    56    
//                                                  /* CCM Command Label Including
//                                                   * - Basic Parameters (8 byte)
//                                                   * - CCM B0           (16 byte)
//                                                   * - CCM B1 (AAD and etc.) (16 byte)
//                                                   * - CCM Counter      (16 byte)
//                                                   */
//#define SA_CCM_ENC_SC_SIZE                  SA_CTX_ENC_TYPE1_SIZE /* Including Key only */
//#define SA_CCM_IV_SIZE                      16    /* AES-CTR IV */
//#define SA_CCM_ENG_ID                       SA_CMDL_ENGINE_ID_ES1  
//
#define SA_CCM_ESP_AAD_LEN1                   8    
//                                                  /* CCM B1 */
//                                                  /* AAD: SPI + SN */
#define SA_CCM_ESP_AAD_LEN2                  12    
//                                                  /* CCM B1 */ 
//                                                  /* AAD: SPI + ESN */ 
//
//#define SA_CCM_CMDL_OPT1                    SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX2_OFFSET), 16)
//#define SA_CCM_CMDL_OPT2                    SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX3_OFFSET), 16)
//#define SA_CCM_CMDL_OPT3                    SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX4_OFFSET), 16)
#define SA_CCM_CMDL_OPT1                      0x62
#define SA_CCM_CMDL_OPT2                      0x72
#define SA_CCM_CMDL_OPT3                      0x82
//                                                   
//
///* AES-CTR Operation related definitions */
#define SA_AESCTR_CMDL_SIZE                  24    
//                                                  /* AES-CTR Command Label Including
//                                                   * - Basic Parameters (8 byte)
//                                                   * - AEs-CTR IV      (16 byte)
//                                                   */
//#define SA_AESCTR_ENC_SC_SIZE               SA_CTX_ENC_TYPE1_SIZE /* Including Key only */
//#define SA_AESCTR_IV_SIZE                   16    /* AES-CTR IV */
//#define SA_AESCTR_ENG_ID                    SA_CMDL_ENGINE_ID_ES1                                                     
//
//
///* GMAC Operation related definitions */
#define SA_GMAC_CMDL_SIZE                     48    
//                                                  /* GMAC Command Label Including
//                                                   * - Basic Parameters (8 byte)
//                                                   * - Length A         (8 byte)
//                                                   * - AAD (Additional Authentication Data) (16 byte)
//                                                   * - AES-CTR IV       (16 byte)
//                                                   */
#define SA_GMAC_CMDL_SIZE_AH                  32    
//                                                  /* GMAC Command Label Including
//                                                   * - Basic Parameters (8 byte)
//                                                   * - Length A         (8 byte)
//                                                   * - AES-CTR IV       (16 byte)
//                                                   */
//                                                   
//#define SA_GMAC_ENC_SC_SIZE                 SA_CTX_ENC_TYPE2_SIZE /* Including Key and Aux1 */
//#define SA_GMAC_IV_SIZE                     16    /* AES-CTR IV */
//#define SA_GMAC_ENG_ID                      SA_CMDL_ENGINE_ID_ES2                                                     
//
#define SA_GMAC_ESP_AAD_LEN1                  8    
//                                                  /* GMAC IPSEC Length A with ESN Disabled */
//                                                  /* AAD: SPI + SN */
#define SA_GMAC_ESP_AAD_LEN2                  12    
//                                                  /* GMAC IPSEC Length A with ESN Enabled */ 
//                                                  /* AAD: SPI + ESN */ 
//                                                  
//#define SA_GMAC_CMDL_OPT1                    SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX1_OFFSET + 16), 8)
//#define SA_GMAC_CMDL_OPT2                    SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX2_OFFSET), 16)
//#define SA_GMAC_CMDL_OPT3                    SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX3_OFFSET), 16)
#define SA_GMAC_CMDL_OPT1                      0x51
#define SA_GMAC_CMDL_OPT2                      0x62
#define SA_GMAC_CMDL_OPT3                      0x72

//                                                   
//                                                  
///* CMAC Operation related definitions */
#define SA_CMAC_CMDL_SIZE                     24    
//                                                  /* CMAC Command Label Including
//                                                   * - Basic Parameters (8 byte)
//                                                   * - K1/K2            (16 byte)
//                                                   */
//#define SA_CMAC_ENC_SC_SIZE                 SA_CTX_ENC_TYPE1_SIZE /* Including Key */
//#define SA_CMAC_IV_SIZE                     0     /* IV is not used */
//#define SA_CMAC_ENG_ID                      SA_CMDL_ENGINE_ID_ES2                                                     
//
//
// 3GPP Related definition
// KEY1: All Mode
// KEY2: Snow3G and ZUC F9
// Modify Key: Kasumi F8/F9
// IV1: all mode except Snow3G and ZUC F9
// IV2: Snow3G and ZUC F9 only
// MISC: Sonw3G/ZUC F* and F9
//#define SA_3GPP_KEY1_CMDL_OPT                SA_CMDL_MK_OPT_CTRL((SA_ENC_KEY_OFFSET),  16)
//#define SA_3GPP_KEY2_CMDL_OPT                SA_CMDL_MK_OPT_CTRL((SA_ENC_KEY_OFFSET+ 16),  16)
//#define SA_3GPP_MOD_KEY_CMDL_OPT             SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX1_OFFSET),  16)
//#define SA_3GPP_IV1_CMDL_OPT                 SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX2_OFFSET), 16)
//#define SA_3GPP_IV2_CMDL_OPT                 SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX1_OFFSET), 16)
//#define SA_3GPP_MISC_CMDL_OPT                SA_CMDL_MK_OPT_CTRL((SA_ENC_AUX1_OFFSET + 16), 16)
// Aux1[127:126] = 'M' bypass. Aux1[125] = T bit Aux1[31:0] = pkt length in bytes

#define SA_3GPP_KEY1_CMDL_OPT                 0x22     // Encryption Key
#define SA_3GPP_KEY2_CMDL_OPT                 0x32     // Authentication Key
#define SA_3GPP_KEY_CMDL_OPT                  0x24     // Both Keys  
  
#define SA_3GPP_MOD_KEY_CMDL_OPT              0x42  
#define SA_3GPP_IV1_CMDL_OPT                  0x62     // IV1 (encrytion IV)
#define SA_3GPP_KASUMI_IV1_CMDL_OPT           0x61  
#define SA_3GPP_IV2_CMDL_OPT                  0x42   
#define SA_3GPP_MISC_CMDL_OPT                 0x52   
#define SA_3GPP_AUX1_CMDL_OPT                 0x44     // IV2 (Authentication IV) and Misc 
//
///******************************************************************************
// * Type:        SA_CTX_PROTO_IPSEC_AH_RX_T        
// ******************************************************************************
// * Description: This structure specifies the IPSEC AH Rx protocol specific Security 
// *              context parameters which are initialized by SW and used by 
// *              Firmware.
// *
// * Note: The parameters which may be updated by the firmware should be arranged
// *       within the first 52 bytes so that the size of the evicted PHP parameters
// *       is limited to 64 bytes
// *
// *****************************************************************************/
//typedef struct SA_CTX_PROTO_IPSEC_AH_RX_tag
//{
//    SA_REPLAY_CTL_T replay_ctl; /* Replay control block */
//
//    UINT32   num_rx_pkts;        /* statistics: TBD */
//    UINT32   num_rx_pkts_hi;     /* statistics: TBD */
//    UINT32   num_dup_pkts;
//    UINT32   num_old_pkts;
//    UINT32   num_hash_fails;
//    
//    UINT8 ctrl_bitfield;        /* various IPSEC AH control information */            
//                                    
//    /*
//     *  Bit 0-1: Transport Type (Tunnel, Transport)
//     *  Bit 2:   ESN flag         1: ESN enabled
//     *  Bit 3:   Replay Flag      1: replay check enabled
//     *  Bit 4-7: Reserved 
//     */   
//    #define SA_CTX_PROTO_IPSEC_AH_RX_SET_TRANSPORT_TYPE(a,b)  UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 0, 2)
//    #define SA_CTX_PROTO_IPSEC_AH_RX_GET_TRANSPORT_TYPE(a)    UTL_GET_BITFIELD((a)->ctrl_bitfield, 0, 2)
//    #define SA_CTX_PROTO_IPSEC_AH_RX_SET_ESN(a,b)             UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 2, 1)
//    #define SA_CTX_PROTO_IPSEC_AH_RX_ESN_ENABLED(a)           UTL_GET_BITFIELD((a)->ctrl_bitfield, 2, 1)
//    #define SA_CTX_PROTO_IPSEC_AH_RX_SET_REPLAY(a,b)          UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 3, 1)
//    #define SA_CTX_PROTO_IPSEC_AH_RX_REPLAY_ENABLED(a)        UTL_GET_BITFIELD((a)->ctrl_bitfield, 3, 1)
//
//    UINT8  pkt_inst_index;        /* Store the packet instance index */
//                                  /* Note: It may be part of the packet info
//                                   *       or allocated at SOP
//                                   */
//                                     
//
//    UINT8   icv_size;             /* size of the hash data 0: indicate no authentication */
//    UINT8   iv_size;              /* size of the initialization vector 0: no IV required */  
//    UINT16  reserved1;
//    UINT8   first_eng_id;         /* Specify the engine id for the first command */
//    UINT8   cmdl_total_len;       /* Specify the size of the multiple command label */
//    SA_CMDL_CFG_ENTRY_T   entry[SA_CMDL_MAX_ENTRIES];  /* command label configuration array */
//    UINT32  spi;                  /* Security Parameters Index */
//      
//} SA_CTX_PROTO_IPSEC_AH_RX_T;  /* 72 bytes */ 
//

.struct struct_ctxAHRx
    .u8     winMaskIndex
    .u8     winMaskBitoff
    .u8     winSize
    .u8     ctrlFlag
    .u32    winMask0
    .u32    winMask1
    .u32    winMask2
    .u32    winMask3
    .u32    winMask4
    .u32    winBaseHi
    .u32    winBase
    .u32    numRxPkts
    .u32    numRxPktsHi
    .u32    numDupPkts
    .u32    numOldPkts
    .u32    numHashFails
    .u32    byteCount
    .u32    byteCountHi
    .u8     ctrlInfo
#define     flagAHRxReplay   t7    
#define     flagAHRxCMAC     t4
#define     flagAHRxUseEnc   t3
#define     flagAHRxESN      t2
#define     AH_RX_TRANSPORT_MASK     0x03
    .u8     opMode     // operation mode
    .u8     icvSize
    .u8     ivSize
    //.u16    reserved1
    .u32    spi
    .u32    salt
    .u8     firstEngId
    .u8     cmdlTotalLen
    .u32    aux1
.ends

.struct struct_ctxAHRxStats
    .u32    numRxPkts
    .u32    numRxPktsHi
    .u32    numDupPkts
    .u32    numOldPkts
    .u32    numHashFails
    .u32    byteCount
    .u32    byteCountHi
.ends

// struct_replayCtrl
 
.struct struct_ctxAHRx1
    .u8     ctrlInfo
    .u8     opMode      // Store the packet instance Index
    .u8     icvSize
    .u8     ivSize
    //.u16    reserved1
    .u32    spi
    .u32    salt
    .u8     firstEngId
    .u8     cmdlTotalLen
.ends

// Combine statistics and replay control for ESN update
.struct struct_ctxAHRxStatsReplay
    .u32    winBaseHi 
    .u32    winBase
    .u32    numRxPkts
    .u32    numRxPktsHi
    .u32    numDupPkts
    .u32    numOldPkts
    .u32    numHashFails
.ends

///******************************************************************************
// * Type:        SA_CTX_PROTO_SRTP_TX_T        
// ******************************************************************************
// * Description: This structure specifies the SRTP Tx protocol specific Security  
// *              context parameters which are initialized by SW and used by 
// *              Firmware. 
// *
// * Note: The parameters which may be updated by the firmware should be arranged
// *       within the first 52 bytes so that the size of the evicted PHP parameters
// *       is limited to 64 bytes
// *
// *****************************************************************************/
//typedef struct SA_CTX_PROTO_SRTP_TX_tag
//{
//
//    UINT32 num_tx_pkts_lo;      /* statistics: TBD */
//    UINT16 num_tx_pkts_hi; 
//    UINT16 reserved1;
//    
//    UINT16 ctrl_bitfield;       /* various SRTP control information */  
//    /*
//     *  Bit 0-2: Encryption mode (CTR, F8, NULL)
//     *  Bit 3: RTCP flag (not used) 
//     *  Bit 4: MKI flag 
//     *  Bit 5: Use indication flag (inform the SW that this context has been used by Firmware)
//     *  Bit 6-7: Reserved 
//     */   
//    #define SA_CTX_PROTO_SRTP_TX_SET_ENCRYPT_MODE(a,b)  UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 0, 3)
//    #define SA_CTX_PROTO_SRTP_TX_GET_ENCRYPT_MODE(a)    UTL_GET_BITFIELD((a)->ctrl_bitfield, 0, 3)
//    #define SA_CTX_PROTO_SRTP_TX_TEST_RTCP(a)           UTL_GET_BITFIELD((a)->ctrl_bitfield, 3, 1)
//    #define SA_CTX_PROTO_SRTP_TX_SET_RTCP(a,b)          UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 3, 1)
//    #define SA_CTX_PROTO_SRTP_TX_TEST_MKI(a)            UTL_GET_BITFIELD((a)->ctrl_bitfield, 4, 1)
//    #define SA_CTX_PROTO_SRTP_TX_SET_MKI(a,b)           UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 4, 1)
//    #define SA_CTX_PROTO_SRTP_TEST_USE_INDICATION(a)    UTL_GET_BITFIELD((a)->ctrl_bitfield, 5, 1)
//    #define SA_CTX_PROTO_SRTP_SET_USE_INDICATION(a,b)   UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 5, 1)
//    UINT8  pkt_inst_index;      /* Store the packet instance index */
//                                /* Note: It may be part of the packet info
//                                 *       or allocated at SOP
//                                 */  
//
//
//    UINT16 last_seq_num;   /* sequence number of the last tx packet */
//    UINT32 roc;            /* rollover counter */
//    
//
//    UINT8  icv_size;       /* size of the authentication tag 0: indicate no authentication */
//    UINT8  iv_size;        /* size of the initialization vector */
//    UINT8  iv_offset;      /* IV offset in bytes in the Encryption security context in unit of 8 bytes */
//                           /* change to ivOpt: IV option control byte = SA_CMDL_MK_OPT_CTRL(offset, len) */
//    UINT8  mki_size;       /* size of the MKI value 0: indicate no MKI (TBD: Is it fixed size?) */
//    
//    UINT8  mki[SA_SRTP_MAX_MKI_SIZE];            /* Record the MKI value to be inserted into the SRTP packet */   
//     
//    UINT8  salt_key[SA_SRTP_MAX_SALT_KEY_SIZE];  /* Record the session salt key */
//    UINT8  salt_key_size;  /* size of the salt key */
//    UINT8  reserved2; 
//    
//} SA_CTX_PROTO_SRTP_TX_T;   /* 40 bytes */
///******************************************************************************
// * Type:        SA_SRTP_ENCRYPT_MODE_T        
// ******************************************************************************
// * Description: Define the SRTP Encryption modes
// *****************************************************************************/
#define SA_SRTP_ENCRYPT_MODE_NULL       0   // No encryption */
#define SA_SRTP_ENCRYPT_MODE_CTR        1   // SRTP Counter Mode */
#define SA_SRTP_ENCRYPT_MODE_F8         2   // SRTP F8 mode */

#define SA_SRTP_IV_SIZE                16
#define SA_SRTP_CMDL_ENC_OPT1          0x62     

.struct struct_ctxSRTPTxStats
    .u32    numTxPktsLo
    .u16    numTxPktsHi
    .u16    reserved 
.ends

.struct struct_ctxSRTPTx1
    .u16    ctrlInfo
#define     flagSRTPTxRTCP              t3
#define     flagSRTPTxMKI               t4
#define     flagSRTPTxEnc               t5
#define     SRTP_TX_ENCRYPT_MODE_MASK     0x07
//    .u8     pktInstIndex    // Store the packet instance Index
    .u16    lastSeqNum
    .u32    roc
    .u8     firstEngId
    .u8     cmdlTotalLen
    .u8     icvSize
    .u8     mkiSize
    .u32    mki
    .u32    saltKey1
    .u32    saltKey2
    .u32    saltKey3
    .u16    saltKey4
    .u8     salkKeySize
    .u8     reserved4 
.ends

///******************************************************************************
// * Type:        SA_CTX_PROTO_SRTP_RX_T        
// ******************************************************************************
// * Description: This structure specifies the SRTP Rx protocol specific Security  
// *              context parameters which are initialized by SW and used by 
// *              Firmware.
// *
// * Note: The parameters which may be updated by the firmware should be arranged
// *       within the first 52 bytes so that the size of the evicted PHP parameters
// *       is limited to 64 bytes
// *
// *****************************************************************************/
//typedef struct SA_CTX_PROTO_SRTP_RX_tag
//{
//    /* Replay Control (32-bytes) */
//    SA_REPLAY_CTL_T replay_ctl; /* Replay control block */

//    /* Statistics (14 bytes) */
//    UINT32 num_dup_pkts;
//    UINT16 num_old_pkts;
//    UINT16 mum_hash_fails;
//    UINT32 num_rx_pkts_lo;      /* statistics: TBD */
//    UINT16 num_rx_pkts_hi; 
//    
//
//    /* Rekey Control (28-bytes including ctrl-bitfilelds) */
//    /* Key life time check */
//    UINT16  key_lifetime_hi;    /* The upper 16-bit of the key lifetime */
//    UINT32  key_lifetime_lo;    /* The lower 32-bit of the key lifetime */
//    
//    /* From to range check */
//    UINT32  from_index_hi;      /* The upper 32-bit of the from index (ROC)*/
//    UINT32  to_index_hi;        /* The upper 32-bit of the to index (ROC) */
//    UINT16  from_index_lo;      /* The lower 16-bit of the from index (seq_num)*/
//    UINT16  to_index_lo;        /* The lower 16-bit of the to index (seq_num)*/  
//    
//    /* Key Derivation check */
//    UINT32  key_deriv_rem_lo;   /* The lower 32-bit of the key derivation reminder */
//    UINT16  key_deriv_rem_hi;   /* The upper 16-bit of the key derivation reminder */
//    UINT8   key_deriv_rate;     /* key derivation rate */ 
//    UINT8   reserved1;
//    
//    /* General Operation: 32-bytes */
//    UINT16   ctrl_bitfield;       /* various SRTP control information */            
//                                    
//    /*
//     *  Bit 0-2: Encryption mode (CTR, F8, NULL)
//     *  Bit 3: RTCP flag  (not used)
//     *  Bit 4: MKI flag   (not used)
//     *  Bit 5: Use indication flag (inform the SW that this context has been used by Firmware)
//     *  Bit 6: From-to flag
//     *  Bit 7: REPLAY flag 
//     *  Bit 8: First packet indication
//     *  Bit 9-15: reserved
//     */   
//    #define SA_CTX_PROTO_SRTP_RX_SET_ENCRYPT_MODE(a,b)      UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 0, 3)
//    #define SA_CTX_PROTO_SRTP_RX_GET_ENCRYPT_MODE(a)        UTL_GET_BITFIELD((a)->ctrl_bitfield, 0, 3)
//    #define SA_CTX_PROTO_SRTP_RX_TEST_RTCP(a)               UTL_GET_BITFIELD((a)->ctrl_bitfield, 3, 1)
//    #define SA_CTX_PROTO_SRTP_RX_SET_RTCP(a,b)              UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 3, 1)
//    #define SA_CTX_PROTO_SRTP_RX_TEST_MKI(a)                UTL_GET_BITFIELD((a)->ctrl_bitfield, 4, 1)
//    #define SA_CTX_PROTO_SRTP_RX_SET_MKI(a,b)               UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 4, 1)
//    #define SA_CTX_PROTO_SRTP_RX_TEST_USE_INDICATION(a)     UTL_GET_BITFIELD((a)->ctrl_bitfield, 5, 1)
//    #define SA_CTX_PROTO_SRTP_RX_SET_USE_INDICATION(a,b)    UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 5, 1)
//    #define SA_CTX_PROTO_SRTP_RX_TEST_FROMTO(a)             UTL_GET_BITFIELD((a)->ctrl_bitfield, 6, 1)
//    #define SA_CTX_PROTO_SRTP_RX_SET_FROMTO(a,b)            UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 6, 1)
//    #define SA_CTX_PROTO_SRTP_RX_TEST_FIRST_PKT(a)          UTL_GET_BITFIELD((a)->ctrl_bitfield, 8, 1)
//    #define SA_CTX_PROTO_SRTP_RX_SET_FIRST_PKT(a,b)         UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 8, 1)
//    #define SA_CTX_PROTO_SRTP_RX_REPLAY_ENABLED(a)          UTL_GET_BITFIELD((a)->ctrl_bitfield, 7, 1)
//    #define SA_CTX_PROTO_SRTP_RX_SET_REPLAY(a,b)            UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 7, 1)
//    
// 
//    UINT16 last_seq_num;        /* s_l: sequence number of the last rx packet */
//    UINT32 roc;                 /* rollover counter */
//    
///* Used in getting the right ROC/SEQ number */
//#define SA_SRTP_ROC_NOCHANGE    0
//#define SA_SRTP_ROC_MINUS_1     1
//#define SA_SRTP_ROC_PLUS_1      2
//    
//    
//    /* All the parameters above (46 bytes) should be evicted */
//    
//    #if 0
//    UINT8   pkt_inst_index;     /* Store the packet instance index */
//                                /* Note: It may be part of the packet info
//                                 *       or allocated at SOP
//                                 */  
//    
//    #endif
//    UINT8   icv_size;           /* size of the authentication tag 0: indicate no authentication */
//    UINT8   iv_size;            /* size of the initialization vector ==> move iv to loacl instance*/
//                                /* simce it can be derived from the ivOpt */
//    UINT8   iv_offset;          /* IV offset in bytes in the Encryption security context */
//                                /* change to ivOpt: IV option control byte = SA_CMDL_MK_OPT_CTRL(offset, len) */
//    UINT8   mki_size;           /* TBD: size of the MKI value (may be fixed size) */
//    
//    
//    
//    UINT8   mki[SA_SRTP_MAX_MKI_SIZE];            /* Record the MKI value to be verified against the one
//                                                     in the SRTP packet */    
//    UINT8   salt_key[SA_SRTP_MAX_SALT_KEY_SIZE];  /* Record the session salt key */
//    UINT8   salt_key_size;                        /* size of the salt key */
//      
//} SA_CTX_PROTO_SRTP_RX_T;  /* 100 bytes */

.struct struct_ctxSRTPRx
    .u8     winMaskIndex
    .u8     winMaskBitoff
    .u8     winSize
    .u8     ctrlFlag
    .u32    winMask0
    .u32    winMask1
    .u32    winMask2
    .u32    winMask3
    .u32    winMask4
    .u32    winBaseHi
    .u32    winBase
    .u32    numDupPkts
    .u16    numOldPkts
    .u16    numHashFails
    .u32    numRxPktsLo
    .u16    numRxPktsHi
    .u16    keyLifetimeHi
    .u32    keyLifetimeLo
    .u32    fromIndexHi
    .u32    toIndexHi
    .u16    fromIndexLo
    .u16    toIndexLo
    .u32    keyDerivRemLo
    .u16    keyDerivRemHi
    .u8     keyDerivRate
    .u8     reserved
    .u16    ctrlInfo
    .u16    lastSeqNum  // (too many element for compiler)
    .u32    roc
    .u8     pktInstIndex    // Store the packet instance Index
    //.u8     icvSize
    //.u8     ivSize
    //.u8     ivOpt
    //.u8     mkiSize
    //.u32    mki
    //.u32    saltKey1
    //.u32    saltKey2
   // .u32    saltKey3
   // .u16    saltKey4
   // .u8     salkKeySize
   // .u8     reserved2 
.ends


#define SRTP_CTX_RX_STATS_SIZE    14

.struct struct_ctxSRTPRxStats
    .u32    numDupPkts
    .u16    numOldPkts
    .u16    numHashFails
    .u32    numRxPktsLo
    .u16    numRxPktsHi
.ends

// Combine statistics and replay control for ESN update
.struct struct_ctxSRTPRxStatsReplay
    .u32    winbaseHi 
    .u32    winbase
    .u32    numDupPkts
    .u16    numOldPkts
    .u16    numHashFails
    .u32    numRxPktsLo
    .u16    numRxPktsHi
.ends

#define SRTP_CTX_RX_REKEY_SIZE   32

.struct struct_ctxSRTPReKey
    .u32    numRxPktsLo
    .u16    numRxPktsHi
    .u16    keyLifetimeHi
    .u32    keyLifetimeLo
    .u32    fromIndexHi
    .u32    toIndexHi
    .u16    fromIndexLo
    .u16    toIndexLo
    .u32    keyDerivRemLo
    .u16    keyDerivRemHi
    .u8     keyDerivRate
    .u8     reserved
.ends

#define SRTP_CTX_RX1_SIZE       32
 
.struct struct_ctxSRTPRx1
    .u16    ctrlInfo
#define     flagSRTPRx1stPkt   t8
#define     flagSRTPRxReplay   t7    
#define     flagSRTPRxFromTo   t6
#define     flagSRTPRxEnc      t5
#define     flagSRTPRxMKI      t4    
#define     flagSRTPRxRTCP     t3
#define     SRTP_RX_ENCRYPT_MODE_MASK     0x07
    .u16    lastSeqNum
    .u32    roc
    .u8     firstEngId
    .u8     cmdlTotalLen
    .u8     icvSize
    .u8     mkiSize
    .u32    mki
    .u32    saltKey1
    .u32    saltKey2
    .u32    saltKey3
    .u16    saltKey4
    .u8     salkKeySize
    .u8     reserved4 
.ends



///******************************************************************************
// * Type:        SA_CTX_PROTO_AC_T        
// ******************************************************************************
// * Description: This structure specifies the Air Cipher protocol specific  
// *              security context parameters which are initialized by SW and used  
// *              by Firmware.
// *
// * Note: The parameters which may be updated by the firmware should be arranged
// *       within the first 52 bytes so that the size of the evicted PHP parameters
// *       is limited to 64 bytes
// *
// *****************************************************************************/
//typedef struct SA_CTX_PROTO_AC_tag
//{
//    UINT32 num_pkts;            /* statistics: */
//    UINT32 num_pkts_hi;
//    UINT32 num_hash_fails;
//
//    UINT32 count_c;             /* The high bits, HFN, for the frame counter 
//                                 * RLC AM: the high 20 bits are used
//                                 * RLC UM: the high 25 bits are used
//                                 * RLC TM: the high 25 bits are used
//                                 */
//    UINT32 fresh;               /* 32-bit random number required for some
//                                 * integrity check algorithm 
//                                 */  
//    UINT16 ctrl_bitfield;       /* various Air Cipher control information */            
//    /*
//     *  Bit 0-2: Cor Algorithm (0=GSM A5/3, 1=Kasumi 2=SNOW 3G, 3=AES CTR)
//     *  Bit 3 : IV present in payload  (For GSM PDUs)
//     *  Bit 4 : Header present in payload (For WCDMA RLC UMD/AMD PDUs)
//     *  Bit 5 : Count-C present in payload (For WDCMA MAC TMD PUD and LTE PDCP PDUs)
//     *  Bit 6 : Insert Count-C into PDU (For WCDMA and LTE in to-air direction)
//     *  Bit 7 : Direction (0:UE to RNC(uplink);1:RNC to UE(downlink)) 
//     *  Bit 8 : Encryption flag (0: No Encryption)
//     *  Bit 9 : Authentication flag (0: No Authentication)
//     *  Bit 10: CMAC (CMAC mode, K1/k2 is required)
//     *  Bit 11: KASUMI_F9 (KASUMI F9, Padding isertion may be required)
//     *  Bit 12: Key in scratch
//     *  Bit 13: Kasumi-F8 operation (modKey needs to be supplied by command label)
//     *  Bit 14: Snow3G_ZUC
//     *  Bit 15: F8F9 (single-PASS operation)
//     */ 
//    #define SA_CTX_PROTO_AC_SET_ALGORITHM(a,b)     UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 0, 3)
//    #define SA_CTX_PROTO_AC_GET_ALGORITHM(a)       UTL_GET_BITFIELD((a)->ctrl_bitfield, 0, 3)
//    #define SA_CTX_PROTO_AC_SET_FLAG_IV(a,b)       UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 3, 1)
//    #define SA_CTX_PROTO_AC_GET_FLAG_IV(a)         UTL_GET_BITFIELD((a)->ctrl_bitfield, 3, 1)
//    #define SA_CTX_PROTO_AC_SET_FLAG_HDR(a,b)      UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 4, 1)
//    #define SA_CTX_PROTO_AC_GET_FLAG_HDR(a)        UTL_GET_BITFIELD((a)->ctrl_bitfield, 4, 1)
//    #define SA_CTX_PROTO_AC_SET_FLAG_COUNT_C(a,b)  UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 5, 1)
//    #define SA_CTX_PROTO_AC_GET_FLAG_COUNT_C(a)    UTL_GET_BITFIELD((a)->ctrl_bitfield, 5, 1)
//    #define SA_CTX_PROTO_AC_SET_FLAG_INS_COUNT_C(a,b)  UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 6, 1)
//    #define SA_CTX_PROTO_AC_GET_FLAG_INS_COUNT_C(a)    UTL_GET_BITFIELD((a)->ctrl_bitfield, 6, 1)
//    #define SA_CTX_PROTO_AC_SET_DIR(a,b)           UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 7, 1)
//    #define SA_CTX_PROTO_AC_GET_DIR(a)             UTL_GET_BITFIELD((a)->ctrl_bitfield, 7, 1)
//    #define SA_CTX_PROTO_AC_SET_FLAG_ENC(a,b)      UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 8, 1)
//    #define SA_CTX_PROTO_AC_GET_FLAG_ENC(a)        UTL_GET_BITFIELD((a)->ctrl_bitfield, 8, 1)
//    #define SA_CTX_PROTO_AC_SET_FLAG_AUTH(a,b)     UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 9, 1)
//    #define SA_CTX_PROTO_AC_GET_FLAG_AUTH(a)       UTL_GET_BITFIELD((a)->ctrl_bitfield, 9, 1)
//    #define SA_CTX_PROTO_AC_SET_FLAG_CMAC(a,b)     UTL_SET_BITFIELD((a), b, 10, 1)
//    #define SA_CTX_PROTO_AC_GET_FLAG_CMAC(a)       UTL_GET_BITFIELD((a), 10, 1)
//    #define SA_CTX_PROTO_AC_SET_FLAG_KASUMI_F9(a,b)     UTL_SET_BITFIELD((a), b, 11, 1)
//    #define SA_CTX_PROTO_AC_GET_FLAG_KASUMI_F9(a)       UTL_GET_BITFIELD((a), 11, 1)
//	  #define SA_CTX_PROTO_AC_SET_FLAG_KEY_IN_SCRATCH(a,b)  UTL_SET_BITFIELD((a), b, 12, 1)
//	  #define SA_CTX_PROTO_AC_GET_FLAG_KEY_IN_SCRATCH(a)	UTL_GET_BITFIELD((a), 12, 1)
//	  #define SA_CTX_PROTO_AC_SET_FLAG_KASUMI_F8(a,b)	UTL_SET_BITFIELD((a), b, 13, 1)
//	  #define SA_CTX_PROTO_AC_GET_FLAG_KASUMI_F8(a)		UTL_GET_BITFIELD((a), 13, 1)	
//	  #define SA_CTX_PROTO_AC_SET_FLAG_SNOW3G_ZUC(a,b)	UTL_SET_BITFIELD((a), b, 14, 1)
//	  #define SA_CTX_PROTO_AC_GET_FLAG_SNOW3G_ZUC(a)	UTL_GET_BITFIELD((a), 14, 1)	
//	  #define SA_CTX_PROTO_AC_SET_FLAG_F8F9(a,b)	    UTL_SET_BITFIELD((a), b, 15, 1)
//	  #define SA_CTX_PROTO_AC_GET_FLAG_F8F9(a)		    UTL_GET_BITFIELD((a), 15, 1)	
//     
//    UINT16 ctrl_bitfield2;       /* various Air Cipher control information */            
//    /*
//     *  Bit 0 : Copy Count-C into timestamp filed at the descriptor
//     *  Bit 1-15 : Reserved
//     */ 
//    #define SA_CTX_PROTO_AC_SET_FLAG_COPY_COUNT_C(a,b)  UTL_SET_BITFIELD((a), b, 0, 1)
//    #define SA_CTX_PROTO_AC_GET_FLAG_COPY_COUNT_C(a)    UTL_GET_BITFIELD((a), 0, 1)
//    #define SA_CTX_PROTO_AC_FLAG_COUNTC_COPY       0x0001
//
//    UINT8  first_eng_id;         /* Specify the engine id for the first command */
//
//    UINT8  cmd_len;              /* Specify the size of the multiple command label */
//
//    /*
//     * The following parameters are used to extract the sequence number from the PDU header
//     *   hdr_size: size ofPDU Header in bytes
//     *   seq_num_size: size of the sequence number inside the header in bits
//     *   seq_num_shift:        
//     */
//                                  
//    UINT8   hdr_size;  
//    UINT8   auth_hdr_size;    /* additional PCDP header to be authenticated, but not encrypted */ 
//    UINT8   seq_num_size; 
//    UINT8   seq_num_shift; 
//
//    UINT8   bearer;     /* 5-bit Bearer identity */
//    UINT8   iv_size;     
//
//    /*
//     * F8F9 option type
//     * 7:6  M: Number of option bytes in the beginning of payload
//     * 5:5  Tx bit: 0: F8 followed by F9
//     *              1: F9 followed by F8
//     * 6:0 Packet Type as specified above 
//     */  
//    #define SA_F8F9_OPT_M_SHIFT         6
//    #define SA_F8F9_OPT_M_MASK          0x03
//    #define SA_F8F9_OPT_TX              0x20
//    #define SA_F8F9_OPT_RX              0x00
//
//    UINT8   iv_opt;     /* The IV command option (or F8F9 option) */
//    UINT8   icv_size;   /* The size of authentication tag in bytes. */
//    UINT32  iv_low26;   /* The low 26-bits for the initialization vector  
//                         * for 3GPP F8 these should be set to zero */
//
//	/* Note: encKeyOffset and macKeyOffset must be set in conjunction 
//			 with SA_CTX_PROTO_AC_GET_FLAG_KEY_IN_SCRATCH */
//    UINT16  enc_key_offset;	  /* enc key byte offset in scratch memory */
//	  UINT16  mac_key_offset;     /* mac key byte offset in scratch memory */                 
//    
//} SA_CTX_PROTO_AC_T;  /* 42 bytes */

///******************************************************************************
// * Type:        SA_AC_ALGORITHM_T        
// ******************************************************************************
// * Description: Define the Air Cipher Core Alogritms
// *****************************************************************************/
#define SA_AC_ALGORITHM_GSM_A53       0   //* GSM A5/3 encryption */
#define SA_AC_ALGORITHM_KASUMI        1   //* Kasumi algorithm */
#define SA_AC_ALGORITHM_SNOW3G        2   //* Snow 3G algorithm */
#define SA_AC_ALGORITHM_AES           3   //* AES CTR/CMAC algorithm */
#define SA_AC_ALGORITHM_ZUC           4   //* ZUC algorithm */

.struct struct_ctxAC
    .u32    numPkts
    .u32    numPktsHi
    .u32    numHashFails
    .u32    countC
    .u32    fresh
    .u16    ctrlInfo
    .u16    ctrlInfo2
    .u8     firstEngId      // Store the first engine Id
    .u8     cmdlLen         // Store the total Command label length
    .u8     hdrSize
    .u8     authHdrSize
    .u8     seqNumSize
    .u8     seqNumShift
    .u8     bearer
    .u8     ivSize
    .u8     ivOpt
    .u8     icvSize
    .u16    encKeyOffset
    .u32    ivLow26
    .u16    macKeyOffset
    .u16    aux
.ends

#define AC_CTX_STATS_SIZE    12

.struct struct_ctxACStats
    .u32    numPkts
    .u32    numPktsHi
    .u32    numHashFails
.ends

.struct struct_ctxAC1
    .u32    countC
    .u32    fresh
    .u16    ctrlInfo
#define 	flagACF8F9		      t15
#define 	flagACSnow3GZuc		  t14
#define     flagACKasumiF8        t13
#define     flagACKeyInScratch    t12
#define     flagACKasumiF9        t11
#define     flagACCMAC            t10
#define     flagACAuth            t9
#define     flagACEnc             t8
#define     flagACDirDownlink     t7
#define     flagACCountCInsert    t6
#define     flagACCountCPresent   t5
#define     flagACHdrPresent      t4
#define     flagACIVPresent       t3
#define     AC_CORE_ALGO_MASK     0x07
    .u16    ctrlInfo2
#define     flagACCountCCopy      t0
    .u8     firstEngId      // Store the first engine Id
    .u8     cmdlLen         // Store the total Command label length
    .u8     hdrSize
    .u8     authHdrSize
    .u8     seqNumSize
    .u8     seqNumShift
    .u8     bearer
    .u8     ivSize
    .u8     ivOpt
    .u8     icvSize
    .u16    encKeyOffset
    .u32    ivLow26
    .u16    macKeyOffset
.ends

//
//  There are two blocks of 1k key storage area
//  block1: 0x2c000 - 0x2ffff (all keystone2 device)
//  block2: 0x14000 - 0x17fff (only available at netcp1.5 device
//  
//  The key offset specified by LLD assumes continuous key blocks and 
//  therefore b14 indicates whether the key should be placed at the 
//  second block as defined below:
//

#define t_flag_key_at_2nd_block     t14      

///******************************************************************************
// * Type:        SA_CTX_PROTO_DM_T        
// ******************************************************************************
// * Description: This structure specifies the Data Mode protocol specific  
// *              security context parameters which are initialized by SW and used  
// *              by Firmware.
// *
// * Note: The parameters which may be updated by the firmware should be arranged
// *       within the first 42 bytes so that the size of the evicted PHP parameters
// *       is limited to 64 bytes
// *
// *****************************************************************************/
//typedef struct SA_CTX_PROTO_AC_tag
//{
//    UINT32 num_pkts;            /* statistics: */
//    UINT32 num_pkts_hi;
//    UINT8  first_eng_id;         /* Specify the engine id for the first command */
//
//    UINT8  pkt_inst_index;       /* Store the packet instance index */
//                                 /* Note: It may be part of the packet info
//                                  *       or allocated at SOP
//                                  */  
//} SA_CTX_PROTO_DM_T;  /* 10 bytes */

.struct struct_ctxDataMode
    .u32    numPkts
    .u32    numPktsHi
    .u8     firstEngId      // Store the first engine Id
    .u8     tagSize         // Store the aligned tag Size 
    .u16    rsvd2
.ends


#define SA_CMDL_HEADER_SIZE_BYTES        8

.struct struct_cmdlCfgEntry
    .u8     nextEngId
    .u8     len
    .u8     ctrlFlag
    #define cmdlCfgLastCmd      t0
    #define cmdlCfgAuth         t1
    .u8     contrlInfo 
//    /*
//     *  command label control information:
//     *  Bit 0-1: Option code of the first option 
//     *  Bit 2-3: Option code of the second option 
//     *  Bit 4-5: Option code of the third option
//     */   
   

//    /* Define option code for the options */
      #define SA_CMDL_CFG_OPTION_CODE_NONE    0     // Option is not used */
      #define SA_CMDL_CFG_OPTION_CODE_IV      1     // Option is initialization vector */
      #define SA_CMDL_CFG_OPTION_CODE_AUX     2     // Option is auxiliary data such as ADD */
      #define SA_CMDL_CFG_OPCODE_SHIFT        2 
      #define SA_CMDL_CFG_OPCODE_MASK         0x03
    .u8     reserved
    .u8     optctrl1
    .u8     optctrl2
    .u8     optctrl3
.ends    


//
//  Different view of the Command label Entry to faciliate register copy
//  The inconsistent parameters (ctrlFlag, controlInfo and reserved) will be
//  overritten by the SOP and Length to be processed
//
.struct struct_cmdlCfgEntryReg
    .u32    reg1
    .u32    reg2
.ends    
