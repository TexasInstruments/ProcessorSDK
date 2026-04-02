#ifndef _SALLDCTX_H
#define _SALLDCTX_H
/*******************************************************************************
 * FILE PURPOSE: Provide Security Accelerator (SA) Packet Header Processor (PHP)
 *               Security Context related defintions (not applicable for sa2ul
 *               Please see new layout for sa2ul)
 *
 ********************************************************************************
 * FILE NAME: 	salldctx.h
 *
 * DESCRIPTION: Provide the Security Context related data structures, constants
 *              and MACROs used by the Packet Header Processor (PHP) module in the 
 *              Security Accelerator (SA) for all supported operation modes
 *
 *     0                   1                   2                   3
 *     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ -----
 *   |     Flags     |  F/E control  |           SCID                |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Hardware Control
 *   |                 SCPTR (Security Context Pointer               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ------
 *   |D|  Pkt Type   |  Flow Index   |   Dest Queue ID               | Software Control               
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ------
 *   |                                                               |
 *   |                                                               |
 *   |                                                               |
 *   |                Protocol Specific Parameters                   |
 *   |                (Variable Size up to 116 bytes                 |
 *   ...                                                           ...
 *   |                                                               |
 *   |                                                               |
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *                    Figure: PHP Security Context Format
 *
 *  New Layout for SA2UL:
 *
 *     0                   1                   2                   3
 *     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ -----
 *   |     Flags     |  F/E control  |           SCID                |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Hardware Control
 *   |                 Additional flags for SA2UL                    +
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+------
 *   |   | Flow Id                   + High SCPTR (SecurityContextPtr| HW/Software
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ------
 *   |                 Low SCPTR (Security Context Ptr)              | Software Control
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ------
 *   |                                                               |
 *   |                                                               |
 *   |                                                               |
 *   |                Protocol Specific Parameters                   |
 *   |                (Variable Size up to 116 bytes                 |
 *   |                                                               |
 *   |                                                               |
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *                    Figure: SA2UL Security Context Format
 *
 *
 *
 * (C) Copyright 2008-2018 Texas Instruments, Inc.
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

#include "src/salldport.h"
#include "src/salldcmdl.h"
 
/* 
 * Data structures used by multiple protocols
 */

/******************************************************************************
 * Type:        SA_REPLAY_CTL_T        
 ******************************************************************************
 * Description: This structure specifies the parameters of the IPSEC 
 *              replay control blocks
 *
 * Note: Some parameters may be initialized by SW
 *****************************************************************************/
/*
 * SASS supports two ranges of replay window size
 * Normal replay window: size <= 128, normal PHP context size (<= 128)
 * Large  replay window: size in (128, 1024], PHP context size = 256 
 *
 */ 
#define SA_CTX_MAX_REPLAY_WINDOW_SIZE       128
#define SA_CTX_WIN_MASK_SIZE  SALLD_DIV_ROUND_UP(SA_CTX_MAX_REPLAY_WINDOW_SIZE, 32) + 1 

#define SA_CTX_MAX_REPLAY_WINDOW_SIZE2     1024 
#define SA_CTX_WIN_MASK_SIZE2  SALLD_DIV_ROUND_UP(SA_CTX_MAX_REPLAY_WINDOW_SIZE2, 32) + 1 


/* Additional flags for SA2UL */
#define SA_CTX_SA2UL_SECURE             0x0001
#define SA_CTX_SA2UL_ALLOW_PROMOTE      0x0002
#define SA_CTX_SA2UL_ALLOW_DEMOTE       0x0004
#define SA_CTX_SA2UL_ALLOW_NONSEC       0x0008
#define SA_CTX_SA2UL_SET_PRIVID         0x0010
#define SA_CTX_SA2UL_SET_PRIV           0x0020
#define SA_CTX_SA2UL_OVERWRITE_FLOWID   0x0040

typedef struct SA_REPLAY_CTL_tag
{
    uint16_t  winMaskIndexBitoff;         /* Mask index of window base */
                                        /* Bit offset from "index" to the window base */
    uint16_t  winSize;                    /* replay window size */
                                        /* for memory alignment only */
    uint32_t  winMask[SA_CTX_WIN_MASK_SIZE];  /* Bitmask Array  */
    uint32_t  winBaseHi;                  /* Upper 32-bit of the win_base when ESN is enabled */
    uint32_t  winBase;                    /* Packet ID of window base (lowest index) */
} saReplayCtl_t; 

/* SRTP operation realted definitions */

/* SRTP operation related constant definitions */
#define SA_SRTP_MAX_MKI_SIZE            4
#define SA_SRTP_MAX_AUTH_TAG_SIZE       16
#define SA_SRTP_MAX_SALT_SIZE           14
#define SA_SRTP_MAX_REPLAY_WINDOW_SIZE  64
#define SA_SRTP_MAX_CMDL_SIZE           80

#define SA_SRTP_MAX_MKI_SIZE_IN_TUINT        SALLD_BYTE_TO_TUINT(SA_SRTP_MAX_MKI_SIZE)
#define SA_SRTP_MAX_SALT_SIZE_IN_TUINT       SALLD_BYTE_TO_TUINT(SA_SRTP_MAX_SALT_SIZE)


/******************************************************************************
 * Type:        SA_SRTP_ENCRYPT_MODE_T        
 ******************************************************************************
 * Description: Define the SRTP Encryption modes
 *****************************************************************************/
#define SA_SRTP_ENCRYPT_MODE_NULL       0   /* No encryption */
#define SA_SRTP_ENCRYPT_MODE_CTR        1   /* SRTP Counter Mode */
#define SA_SRTP_ENCRYPT_MODE_F8         2   /* SRTP F8 mode */

/******************************************************************************
 * Type:        SA_CTX_PROTO_SRTP_TX_T        
 ******************************************************************************
 * Description: This structure specifies the SRTP Tx protocol specific Security  
 *              context parameters which are initialized by SW and used by 
 *              Firmware. 
 *
 * Note: The parameters which may be updated by the firmware should be arranged
 *       within the first 52 bytes so that the size of the evicted PHP parameters
 *       is limited to 64 bytes
 *
 *****************************************************************************/
typedef struct 
{

    uint32_t numTxPktsLo;      
    uint16_t numTxPktsHi; 
    uint16_t reserved1;
    
    uint16_t ctrlBitfield;       /* various SRTP control information */  
    /*
     *  Bit 0-2: Encryption mode (CTR, F8, NULL)
     *  Bit 3: RTCP flag (not used) 
     *  Bit 4: MKI flag 
     *  Bit 5: Encryption flag (0: No Encryption)
     *  Bit 6-15: Reserved 
     */   
    #define SA_CTX_PROTO_SRTP_TX_SET_ENCRYPT_MODE(a,b)  UTL_SET_BITFIELD((a), b, 0, 3)
    #define SA_CTX_PROTO_SRTP_TX_GET_ENCRYPT_MODE(a)    UTL_GET_BITFIELD((a), 0, 3)
    #define SA_CTX_PROTO_SRTP_TX_TEST_RTCP(a)           UTL_GET_BITFIELD((a), 3, 1)
    #define SA_CTX_PROTO_SRTP_TX_SET_RTCP(a,b)          UTL_SET_BITFIELD((a), b, 3, 1)
    #define SA_CTX_PROTO_SRTP_TX_TEST_MKI(a)            UTL_GET_BITFIELD((a), 4, 1)
    #define SA_CTX_PROTO_SRTP_TX_SET_MKI(a,b)           UTL_SET_BITFIELD((a), b, 4, 1)
    #define SA_CTX_PROTO_SRTP_TX_TEST_ENC_FLAG(a)       UTL_GET_BITFIELD((a), 5, 1)
    #define SA_CTX_PROTO_SRTP_TX_SET_ENC_FLAG(a,b)      UTL_SET_BITFIELD((a), b, 5, 1)

    uint16_t lastSeqNum;     /* sequence number of the last tx packet */
    uint32_t roc;            /* rollover counter */
    
    uint16_t firstEngIdCmdlLen;  /* Specify the engine id for the first command */
                                 /* Specify the size of the multiple command label */
    uint16_t icvMkiSize;    /* size of the authentication tag 0: indicate no authentication */
                            /* size of MKI */
    
    uint16_t mki[SA_SRTP_MAX_MKI_SIZE_IN_TUINT];   /* Record the MKI value to be inserted into the SRTP packet */   
     
    uint16_t saltKey[SA_SRTP_MAX_SALT_SIZE_IN_TUINT];  /* Record the session salt key */
    uint16_t saltKeySize;  /* size of the salt key in upper 8-bit */

} saCtxProtoSrtpTx_t;   /* 40 bytes */


/******************************************************************************
 * Type:        SA_CTX_PROTO_SRTP_RX_T        
 ******************************************************************************
 * Description: This structure specifies the SRTP Rx protocol specific Security  
 *              context parameters which are initialized by SW and used by 
 *              Firmware.
 *
 * Note: The parameters which may be updated by the firmware should be arranged
 *       within the first 52 bytes so that the size of the evicted PHP parameters
 *       is limited to 64 bytes
 *
 *****************************************************************************/
typedef struct 
{

    /* Replay Control (32-bytes) */
    saReplayCtl_t replayCtl; /* Replay control block */

    /* Statistics (14 bytes) */
    uint32_t numDupPkts;
    uint16_t numOldPkts;
    uint16_t numHashFails;
    uint32_t numRxPktsLo;      
    uint16_t numRxPktsHi; 

    /* Rekey Control (28-bytes including ctrl-bitfilelds) */
    /* Key life time check */
    uint16_t  keyLifetimeHi;    /* The upper 16-bit of the key lifetime */
    uint32_t  keyLifetimeLo;    /* The lower 32-bit of the key lifetime */
    
    /* From to range check */
    uint32_t  fromIndexHi;      /* The upper 32-bit of the from index (ROC)*/
    uint32_t  toIndexHi;        /* The upper 32-bit of the to index (ROC) */
    uint16_t  fromIndexLo;      /* The lower 16-bit of the from index (seq_num)*/
    uint16_t  toIndexLo;        /* The lower 16-bit of the to index (seq_num)*/  
    
    /* Key Derivation check */
    uint32_t  keyDerivRemLo;    /* The lower 32-bit of the key derivation reminder */
    uint16_t  keyDerivRemHi;    /* The upper 16-bit of the key derivation reminder */
    uint16_t  keyDerivRate;     /* key derivation rate in upper 8-bit */
     
    /* General Operation: 32-bytes */
    uint16_t   ctrlBitfield;       /* various SRTP control information */            
                                    
    /*
     *  Bit 0-2: Encryption mode (CTR, F8, NULL)
     *  Bit 3: RTCP flag  (not used)
     *  Bit 4: MKI flag   (not used)
     *  Bit 5: Encryption flag (0: No Encryption)
     *  Bit 6: From-to flag
     *  Bit 7: REPLAY flag 
     *  Bit 8: First packet indication 1:Wait for the first packet
     *  Bit 9-15: reserved
     */   
    #define SA_CTX_PROTO_SRTP_RX_SET_ENCRYPT_MODE(a,b)      UTL_SET_BITFIELD((a), b, 0, 3)
    #define SA_CTX_PROTO_SRTP_RX_GET_ENCRYPT_MODE(a)        UTL_GET_BITFIELD((a), 0, 3)
    #define SA_CTX_PROTO_SRTP_RX_TEST_RTCP(a)               UTL_GET_BITFIELD((a), 3, 1)
    #define SA_CTX_PROTO_SRTP_RX_SET_RTCP(a,b)              UTL_SET_BITFIELD((a), b, 3, 1)
    #define SA_CTX_PROTO_SRTP_RX_TEST_MKI(a)                UTL_GET_BITFIELD((a), 4, 1)
    #define SA_CTX_PROTO_SRTP_RX_SET_MKI(a,b)               UTL_SET_BITFIELD((a), b, 4, 1)
    #define SA_CTX_PROTO_SRTP_RX_TEST_FROMTO(a)             UTL_GET_BITFIELD((a), 6, 1)
    #define SA_CTX_PROTO_SRTP_RX_SET_FROMTO(a,b)            UTL_SET_BITFIELD((a), b, 6, 1)
    #define SA_CTX_PROTO_SRTP_RX_TEST_ENC_FLAG(a)           UTL_GET_BITFIELD((a), 5, 1)
    #define SA_CTX_PROTO_SRTP_RX_SET_ENC_FLAG(a,b)          UTL_SET_BITFIELD((a), b, 5, 1)
    #define SA_CTX_PROTO_SRTP_RX_TEST_FIRST_PKT(a)          UTL_GET_BITFIELD((a), 8, 1)
    #define SA_CTX_PROTO_SRTP_RX_SET_FIRST_PKT(a,b)         UTL_SET_BITFIELD((a), b, 8, 1)
    #define SA_CTX_PROTO_SRTP_RX_REPLAY_ENABLED(a)          UTL_GET_BITFIELD((a), 7, 1)
    #define SA_CTX_PROTO_SRTP_RX_SET_REPLAY(a,b)            UTL_SET_BITFIELD((a), b, 7, 1)
    
 
    uint16_t lastSeqNum;        /* s_l: sequence number of the last rx packet */
    uint32_t roc;               /* rollover counter */
 
    /* All the parameters above (46 bytes) should be evicted */
    uint16_t firstEngIdCmdlLen;  /* Specify the engine id for the first command */
                                 /* Specify the size of the multiple command label */
    uint16_t icvMkiSize;    /* size of the authentication tag 0: indicate no authentication */
                            /* size of MKI in bytes */
    
    uint16_t mki[SA_SRTP_MAX_MKI_SIZE_IN_TUINT];   /* Record the MKI value to be inserted into the SRTP packet */   
     
    uint16_t salt[SA_SRTP_MAX_SALT_SIZE_IN_TUINT];  /* Record the session salt key */
    uint16_t saltSize;                              /* size of the salt in upper 8-bit */
      
} saCtxProtoSrtpRx_t;  /* 106 bytes */


/* IPSEC operation related definitions */

/* IPSEC operation related constant definitions */
#define SA_IPSEC_MAX_HASH_DATA_SIZE         16 
#define SA_IPSEC_MAX_IV_SIZE                16
#define SA_IPSEC_MAX_AUX_DATA_SIZE          12
#define SA_IPSEC_MAX_IPHDR_OPT_SIZE         100
#define SA_IPSEC_MAX_SALT_SIZE              4
#define SA_IPSEC_MAX_AUX_SIZE               32  /* store 16-byte K1 and K2 for CMAC */
#define SA_IPSEC_MAX_SALT_SIZE_IN_TUINT     SALLD_BYTE_TO_TUINT(SA_IPSEC_MAX_SALT_SIZE)
#define SA_IPSEC_MAX_AUX_SIZE_IN_TUINT      SALLD_BYTE_TO_TUINT(SA_IPSEC_MAX_AUX_SIZE)
#define SA_IPSEC_MAX_IV_SIZE_IN_TUINT       SALLD_BYTE_TO_TUINT(SA_IPSEC_MAX_IV_SIZE)

/******************************************************************************
 * Type:        SA_IPSEC_TRANSPORT_TYPE_T        
 ******************************************************************************
 * Description: Define the IPSec Transport Types
 *****************************************************************************/
#define SA_IPSEC_TRANSPORT_TYPE_TRANSPORT       0   
#define SA_IPSEC_TRANSPORT_TYPE_TUNNEL          1  

/******************************************************************************
 * Type:        SA_IPSEC_ESP_CMDL_MODE_T        
 ******************************************************************************
 * Description: Define the IPSEC Command Label Processing modes for ESP operation
 *
 * Note: It can be used by the PHP as index to the command label processing
 *       dispatch table
 *****************************************************************************/
#define SA_IPSEC_ESP_CMDL_MODE_GEN        0   /* No special processing is required */
#define SA_IPSEC_ESP_CMDL_MODE_GCM        1   /* Combined Mode */
#define SA_IPSEC_ESP_CMDL_MODE_GMAC       2   /* Combined Mode */
#define SA_IPSEC_ESP_CMDL_MODE_CCM        3   /* Combined Mode */
#define SA_IPSEC_ESP_CMDL_MODE_CMAC       4   /* Cipher-based authentication code */

/******************************************************************************
 * Type:        SA_IPSEC_AH_CMDL_MODE_T        
 ******************************************************************************
 * Description: Define the IPSEC Command Label Processing modes for AH operation
 *
 * Note: It can be used by the PHP as index to the command label processing
 *       dispatch table
 *****************************************************************************/
#define SA_IPSEC_AH_CMDL_MODE_GEN        0   /* No special processing is required */
#define SA_IPSEC_AH_CMDL_MODE_GMAC       1   /* Combined Mode */
#define SA_IPSEC_AH_CMDL_MODE_CMAC       2   /* Cipher-based authentication code */

/******************************************************************************
 * Type:        saIpsecEsn_t        
 ******************************************************************************
 * Description: This structure contains the extended sequence number  
 *****************************************************************************/
typedef struct SA_IPSEC_ESN_tag
{
    uint32_t lo;
    uint32_t hi;    
} saIpsecEsn_t; 

#define SA_IPSEC_INC_ESN(esn)       if(++(esn).lo == 0)(esn).hi++

/******************************************************************************
 * Type:        SA_CTX_PROTO_IPSEC_AH_TX_T        
 ******************************************************************************
 * Description: This structure specifies the IPSEC AH Tx protocol specific Security 
 *              context parameters which are initialized by SW and used by 
 *              Firmware.
 *
 * Note: The parameters which may be updated by the firmware should be arranged
 *       within the first 52 bytes so that the size of the evicted PHP parameters
 *       is limited to 64 bytes
 *
 *
 *****************************************************************************/
typedef struct SA_CTX_PROTO_IPSEC_AH_TX_tag
{
    uint32_t numTxPkts;      
    uint32_t numTxPktsHi;    
    uint32_t byteCount;
    uint32_t byteCountHi;
    uint32_t rollOverCounter;
    saIpsecEsn_t  esn;   /* extended sequence number of the last tx packet */

    uint16_t ctrlBitfield;  /* various IPSEC AH control information in upper 8-bit*/            
                                    
    /*
     *  Bit 0-7: Operation Mode
     *  Bit 8-9: Transport Type (Tunnel, Transport) (not used)
     *  Bit 10:  ESN flag
     *  Bit 11:  Use ENC  (Use Encryption Engine for authentication)
     *  Bit 12:  CMAC     (CMAC mode, K1/k2 is required)
     *  Bit 13-15: Reserved 
     */   
    #define SA_CTX_PROTO_IPSEC_AH_TX_SET_TRANSPORT_TYPE(a,b)  UTL_SET_BITFIELD((a), b, 8, 2)
    #define SA_CTX_PROTO_IPSEC_AH_TX_GET_TRANSPORT_TYPE(a)    UTL_GET_BITFIELD((a), 8, 2)
    #define SA_CTX_PROTO_IPSEC_AH_TX_SET_ESN(a, b)            UTL_SET_BITFIELD((a), b, 10, 1)
    #define SA_CTX_PROTO_IPSEC_AH_TX_ESN_ENABLED(a)           UTL_GET_BITFIELD((a), 10, 1)
    #define SA_CTX_PROTO_IPSEC_AH_TX_SET_USEENC(a, b)         UTL_SET_BITFIELD((a), b, 11, 1)
    #define SA_CTX_PROTO_IPSEC_AH_TX_TEST_USEENC(a)           UTL_GET_BITFIELD((a), 11, 1)
    #define SA_CTX_PROTO_IPSEC_AH_TX_SET_CMAC(a, b)           UTL_SET_BITFIELD((a), b, 12, 1)
    #define SA_CTX_PROTO_IPSEC_AH_TX_TEST_CMAC(a)             UTL_GET_BITFIELD((a), 12, 1)
    
    uint16_t  icvIvSize;   /* size of the hash data 0: indicate no authentication */
                           /* size of the initialization vector 0: no IV required */  
    //uint16_t  reserved1; 
    uint32_t  spi;           /* Security Parameters Index */    
    uint16_t  salt1;  /* Record the session salt key Lo*/
    uint16_t  salt2;  /* Record the session salt key Hi*/
    uint16_t  firstEngIdCmdlLen;  /* Specify the engine id for the first command */
                                  /* Specify the size of the multiple command label */
    uint16_t  aux1[SA_IPSEC_MAX_AUX_SIZE_IN_TUINT];   /* Auxiliary storage space for certain 
                                                         encryption/authentication algorithm */ 
} saCtxProtoIpsecAhTx_t;   /* 42/74 bytes */


/******************************************************************************
 * Type:        SA_CTX_PROTO_IPSEC_AH_RX_T        
 ******************************************************************************
 * Description: This structure specifies the IPSEC AH Rx protocol specific Security 
 *              context parameters which are initialized by SW and used by 
 *              Firmware.
 *
 * Note: The parameters which may be updated by the firmware should be arranged
 *       within the first 52 bytes so that the size of the evicted PHP parameters
 *       is limited to 64 bytes
 *
 *****************************************************************************/
typedef struct SA_CTX_PROTO_IPSEC_AH_RX_tag
{
    saReplayCtl_t replayCtl; /* Replay control block */
    
    uint32_t numRxPkts;         /* statistics */
    uint32_t numRxPktsHi;       
    uint32_t numDupPkts;
    uint32_t numOldPkts;
    uint32_t numHashFails;
    uint32_t byteCount;
    uint32_t byteCountHi;
    uint16_t ctrlBitfield;  /* various IPSEC AH control information in upper 8-bit*/            
                                    
    /*
     *  Bit 0-7: Operation mode
     *  Bit 8-9: Transport Type (Tunnel, Transport) (not used)
     *  Bit 10:  ESN flag
     *  Bit 11:  Use ENC  (Use Encryption Engine for authentication)
     *  Bit 12:  CMAC     (CMAC mode, K1/k2 is required)
     *  Bit 13-14: Reserved   
     *  Bit 15:  Replay Flag      1: replay check enabled
     */   
    #define SA_CTX_PROTO_IPSEC_AH_RX_SET_TRANSPORT_TYPE(a,b)  UTL_SET_BITFIELD((a), b, 8, 2)
    #define SA_CTX_PROTO_IPSEC_AH_RX_GET_TRANSPORT_TYPE(a)    UTL_GET_BITFIELD((a), 8, 2)
    #define SA_CTX_PROTO_IPSEC_AH_RX_SET_ESN(a,b)             UTL_SET_BITFIELD((a), b, 10, 1)
    #define SA_CTX_PROTO_IPSEC_AH_RX_ESN_ENABLED(a)           UTL_GET_BITFIELD((a), 10, 1)
    #define SA_CTX_PROTO_IPSEC_AH_RX_SET_USEENC(a, b)         UTL_SET_BITFIELD((a), b, 11, 1)
    #define SA_CTX_PROTO_IPSEC_AH_RX_TEST_USEENC(a)           UTL_GET_BITFIELD((a), 11, 1)
    #define SA_CTX_PROTO_IPSEC_AH_RX_SET_CMAC(a, b)           UTL_SET_BITFIELD((a), b, 12, 1)
    #define SA_CTX_PROTO_IPSEC_AH_RX_TEST_CMAC(a)             UTL_GET_BITFIELD((a), 12, 1)
    #define SA_CTX_PROTO_IPSEC_AH_RX_SET_REPLAY(a,b)          UTL_SET_BITFIELD((a), b, 15, 1)
    #define SA_CTX_PROTO_IPSEC_AH_RX_REPLAY_ENABLED(a)        UTL_GET_BITFIELD((a), 15, 1)

    uint16_t  icvIvSize;   /* size of the hash data 0: indicate no authentication */
                           /* size of the initialization vector 0: no IV required */  
    //uint16_t  reserved1; 
    uint32_t  spi;                /* Security Parameters Index */
    uint16_t  salt1;  /* Record the session salt key Lo*/
    uint16_t  salt2;  /* Record the session salt key Hi*/
    uint16_t  firstEngIdCmdlLen;  /* Specify the engine id for the first command */
                                /* Specify the size of the multiple command label */
    uint16_t  aux1[SA_IPSEC_MAX_AUX_SIZE_IN_TUINT];   /* Auxiliary storage space for certain 
                                                       encryption/authentication algorithm */ 
} saCtxProtoIpsecAhRx_t;  /* 74/106*/ 

/******************************************************************************
 * Type:        SA_CTX_PROTO_IPSEC_ESP_TX_T        
 ******************************************************************************
 * Description: This structure specifies the IPSEC ESP Tx protocol specific   
 *              Security context parameters which are initialized by SW and used  
 *              by Firmware.
 *
 * Note: The parameters which may be updated by the firmware should be arranged
 *       within the first 52 bytes so that the size of the evicted PHP parameters
 *       is limited to 64 bytes
 *
 *****************************************************************************/
typedef struct SA_CTX_PROTO_IPSEC_ESP_TX_tag
{
    uint32_t numTxPkts;           /* statistics */
    uint32_t numTxPktsHi;         
    uint32_t byteCount;
    uint32_t byteCountHi;
    uint32_t rollOverCounter;
    saIpsecEsn_t  esn;          /* extended sequence number of the last tx packet */
    
    uint16_t ctrlBitfield;  /* various IPSEC AH control information in upper 8-bit*/            
                                    
    /*
     *  Bit 0-7: Operation Mode
     *  Bit 8:   (not used)
     *  Bit 9:   Use 2nd Pair Auth/Enc Engine
     *  Bit 10:  ESN flag
     *  Bit 11:  Use ENC  (Use Encryption Engine for authentication)
     *  Bit 12:  CMAC     (CMAC mode, K1/k2 is required)
     *  Bit 13:  AES-CTR  (AES-CTR is used, need to construct the 16-byte IV)  
     *  Bit 14:  NULL_ENC (no encryption is required)
     *  Bit 15:  Random IV (Random IV is required for the encryption operation) 
     */   
    #define SA_CTX_PROTO_IPSEC_ESP_TX_SET_2ND_PAIR_ENG(a,b)    UTL_SET_BITFIELD((a), b, 9, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_GET_2ND_PAIR_ENG(a)      UTL_GET_BITFIELD((a), 9, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_SET_ESN(a, b)            UTL_SET_BITFIELD((a), b, 10, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_ESN_ENABLED(a)           UTL_GET_BITFIELD((a), 10, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_SET_USEENC(a, b)         UTL_SET_BITFIELD((a), b, 11, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_TEST_USEENC(a)           UTL_GET_BITFIELD((a), 11, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_SET_CMAC(a, b)           UTL_SET_BITFIELD((a), b, 12, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_TEST_CMAC(a)             UTL_GET_BITFIELD((a), 12, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_SET_AESCTR(a, b)         UTL_SET_BITFIELD((a), b, 13, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_TEST_AESCTR(a)           UTL_GET_BITFIELD((a), 13, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_SET_NULLENC(a, b)        UTL_SET_BITFIELD((a), b, 14, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_TEST_NULLENC(a)          UTL_GET_BITFIELD((a), 14, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_SET_RANDOM_IV(a, b)      UTL_SET_BITFIELD((a), b, 15, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_TX_TEST_RANDOM_IV(a)        UTL_GET_BITFIELD((a), 15, 1)
    
    uint16_t  icvIvSize;   /* size of the hash data 0: indicate no authentication */
                           /* size of the initialization vector 0: no IV required */  
/*    uint16_t  reserved1; */
/*    uint8_t   encEngId; */
    /** Possible Values for engine id: 
    ** 2    -   Enc Pass1
    ** 3    -   Enc Pass2
    ** 4    -   Auth Pass1
    ** 5    -   Auth Pass2
    ** 6    -   Enc1 Pass1
    ** 7    -   Enc1 Pass2
    ** 10   -   Auth1 Pass1
    ** 11   -   Auth1 Pass2
    ** */
    //uint16_t   engPairSel; /* selected engine id: first byte - encryption; second byte - authentication */
    uint32_t  spi;                 /* Security Parameters Index */
    uint16_t  salt1;  /* Record the session salt key Lo*/
    uint16_t  salt2;  /* Record the session salt key Hi*/
    uint16_t  firstEngIdCmdlLen;  /* Specify the engine id for the first command */
                                /* Specify the size of the multiple command label */
    uint16_t  aux1[SA_IPSEC_MAX_AUX_SIZE_IN_TUINT];   /* Auxiliary storage space for certain 
                                                       encryption/authentication algorithm */ 
    
} saCtxProtoIpsecEspTx_t;   /* 42/74 bytes */


/******************************************************************************
 * Type:        SA_CTX_PROTO_IPSEC_ESP_RX_T        
 ******************************************************************************
 * Description: This structure specifies the IPSEC ESP Rx protocol specific   
 *              security context parameters which are initialized by SW and used  
 *              by Firmware.
 *
 * Note: The parameters which may be updated by the firmware should be arranged
 *       within the first 52 bytes so that the size of the evicted PHP parameters
 *       is limited to 64 bytes
 *
 *
 *****************************************************************************/
typedef struct SA_CTX_PROTO_IPSEC_ESP_RX_tag
{
    saReplayCtl_t replayCtl;  /* Replay control block */
    
    uint32_t numRxPkts;         
    uint32_t numRxPktsHi;       
    uint32_t numDupPkts;
    uint32_t numOldPkts;
    uint32_t numHashFails;
    uint32_t byteCount;
    uint32_t byteCountHi;
    uint16_t ctrlBitfield;  /* various IPSEC AH control information in upper 8-bit*/            
                                    
    /*
     *  Bit 0-7: Operation Mode
     *  Bit 8:   (not used)
     *  Bit 9:   Use 2nd Pair Auth/Enc Engine
     *  Bit 10:  ESN flag
     *  Bit 11:  Use ENC  (Use Encryption Engine for authentication)
     *  Bit 12:  CMAC     (CMAC mode, K1/k2 is required)
     *  Bit 13:  AES-CTR  (AES-CTR is used, need to construct the 16-byte IV)  
     *  Bit 14:  NULL_ENC (no encryption is required)
     *  Bit 15:  Replay Flag      1: replay check enabled
     */   
    #define SA_CTX_PROTO_IPSEC_ESP_RX_SET_2ND_PAIR_ENG(a,b)  UTL_SET_BITFIELD((a), b, 9, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_GET_2ND_PAIR_ENG(a)    UTL_GET_BITFIELD((a), 9, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_SET_ESN(a,b)             UTL_SET_BITFIELD((a), b, 10, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_ESN_ENABLED(a)           UTL_GET_BITFIELD((a), 10, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_SET_USEENC(a, b)         UTL_SET_BITFIELD((a), b, 11, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_TEST_USEENC(a)           UTL_GET_BITFIELD((a), 11, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_SET_CMAC(a, b)           UTL_SET_BITFIELD((a), b, 12, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_TEST_CMAC(a)             UTL_GET_BITFIELD((a), 12, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_SET_AESCTR(a, b)         UTL_SET_BITFIELD((a), b, 13, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_TEST_AESCTR(a)           UTL_GET_BITFIELD((a), 13, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_SET_NULLENC(a, b)        UTL_SET_BITFIELD((a), b, 14, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_TEST_NULLENC(a)          UTL_GET_BITFIELD((a), 14, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_SET_REPLAY(a,b)          UTL_SET_BITFIELD((a), b, 15, 1)
    #define SA_CTX_PROTO_IPSEC_ESP_RX_REPLAY_ENABLED(a)        UTL_GET_BITFIELD((a), 15, 1)

    uint16_t  icvIvSize;          /* size of the hash data 0: indicate no authentication */
                                /* size of the initialization vector 0: no IV required */  
/*    uint16_t  reserved1; */
    //uint16_t   engPairSel; /* selected engine id: first byte - encryption; second byte - authentication */
    /** Possible Values for engine id: 
    ** 2    -   Enc Pass1
    ** 3    -   Enc Pass2
    ** 4    -   Auth Pass1
    ** 5    -   Auth Pass2
    ** 6    -   Enc1 Pass1
    ** 7    -   Enc1 Pass2
    ** 10   -   Auth1 Pass1
    ** 11   -   Auth1 Pass2
    ** */
    uint32_t  spi;                /* Security Parameters Index */
    uint16_t  salt1;  /* Record the session salt key Lo*/
    uint16_t  salt2;  /* Record the session salt key Hi*/
    uint16_t  firstEngIdCmdlLen;  /* Specify the engine id for the first command */
                                /* Specify the size of the multiple command label */
    uint16_t  aux1[SA_IPSEC_MAX_AUX_SIZE_IN_TUINT];   /* Auxiliary storage space for certain 
                                                       encryption/authentication algorithm */ 
    
} saCtxProtoIpsecEspRx_t;  /* (74/106) 70 bytes + salt(GCM, CCM)  or aux1(CMAC only) */

/* Air Cipher operation related definitions */
#define SA_AC_MAX_HDR_SIZE           2

/******************************************************************************
 * Type:        SA_AC_ENCRYPT_MODE_T        
 ******************************************************************************
 * Description: Define the Air Cipher Encryption modes
 *****************************************************************************/
#define SA_AC_ENCRYPT_MODE_NULL       0   /* No encryption */
#define SA_AC_ENCRYPT_MODE_F8         1   /* Air Cipher F8 mode */

/******************************************************************************
 * Type:        SA_AC_ALGORITHM_T        
 ******************************************************************************
 * Description: Define the Air Cipher Core Alogritms
 *****************************************************************************/
#define SA_AC_ALGORITHM_GSM_A53       0   /* GSM A5/3 encryption */
#define SA_AC_ALGORITHM_KASUMI        1   /* Kasumi algorithm */
#define SA_AC_ALGORITHM_SNOW3G        2   /* Snow 3G algorithm */
#define SA_AC_ALGORITHM_AES           3   /* AES CTR/CMAC algorithm */
#define SA_AC_ALGORITHM_ZUC           4   /* ZUC algorithm */


/******************************************************************************
 * Type:        SA_AC_SN_TYPE_T        
 ******************************************************************************
 * Description: Define the Air Cipher Sequence Number Type
 *****************************************************************************/
#define SA_AC_SN_TYPE_NONE       0   /* No sequence number */
#define SA_AC_SN_TYPE_RLC_UM     1   /* RLC UM */
#define SA_AC_SN_TYPE_RLC_AM     2   /* RLC AM */
#define SA_AC_SN_TYPE_RLC_TM     3   /* RLC TM */

/* AC operation related constant definitions */
#define SA_AC_MAX_AUX_SIZE               32  /* store 16-byte K1 and K2 for CMAC */
#define SA_AC_MAX_AUX_SIZE_IN_TUINT      SALLD_BYTE_TO_TUINT(SA_AC_MAX_AUX_SIZE)


/******************************************************************************
 * Type:        SA_CTX_PROTO_AC_T        
 ******************************************************************************
 * Description: This structure specifies the Air Cipher protocol specific  
 *              security context parameters which are initialized by SW and used  
 *              by Firmware.
 *
 * Note: The parameters which may be updated by the firmware should be arranged
 *       within the first 42 bytes so that the size of the evicted PHP parameters
 *       is limited to 64 bytes
 *
 *****************************************************************************/
typedef struct SA_CTX_PROTO_AC_tag
{
    uint32_t numPkts;             
    uint32_t numPktsHi;
    uint32_t numHashFails;        

    uint32_t countC;              /* The high bits, HFN, for the frame counter 
                                   * RLC AM: the high 20 bits are used
                                   * RLC UM: the high 25 bits are used
                                   * RLC TM: the high 25 bits are used
                                   */
    uint32_t fresh;               /* 32-bit random number required for some
                                   * integrity check algorithm 
                                   */  
    uint16_t ctrlBitfield;        /* various Air Cipher control information */            
    /*
     *  Bit 0-2: Cor Algorithm (0=GSM A5/3, 1=Kasumi 2=SNOW 3G, 3=AES CTR)
     *  Bit 3 : IV present in payload  (For GSM PDUs)
     *  Bit 4 : Header present in payload (For WCDMA RLC UMD/AMD PDUs)
     *  Bit 5 : Count-C present in payload (For WDCMA MAC TMD PUD and LTE PDCP PDUs)
     *  Bit 6 : Insert Count-C into PDU (For WCDMA and LTE in to-air direction)
     *  Bit 7 : Direction (0:UE to RNC(uplink);1:RNC to UE(downlink)) 
     *  Bit 8 : Encryption flag (0: No Encryption)
     *  Bit 9 : Authentication flag (0: No Authentication)
     *  Bit 10: CMAC (CMAC mode, K1/k2 is required)
     *  Bit 11: KASUMI_F9 (KASUMI F9, Padding isertion may be required)
     *  Bit 12: Key in scratch
     *  Bit 13: Kasumi-F8 operation (modKey needs to be supplied by command label)
     *  Bit 14: Snow3G_ZUC
     *  Bit 15: F8F9 (single-PASS operation)
     */ 
    #define SA_CTX_PROTO_AC_SET_ALGORITHM(a,b)     UTL_SET_BITFIELD((a), b, 0, 3)
    #define SA_CTX_PROTO_AC_GET_ALGORITHM(a)       UTL_GET_BITFIELD((a), 0, 3)
    #define SA_CTX_PROTO_AC_SET_FLAG_IV(a,b)       UTL_SET_BITFIELD((a), b, 3, 1)
    #define SA_CTX_PROTO_AC_GET_FLAG_IV(a)         UTL_GET_BITFIELD((a), 3, 1)
    #define SA_CTX_PROTO_AC_SET_FLAG_HDR(a,b)      UTL_SET_BITFIELD((a), b, 4, 1)
    #define SA_CTX_PROTO_AC_GET_FLAG_HDR(a)        UTL_GET_BITFIELD((a), 4, 1)
    #define SA_CTX_PROTO_AC_SET_FLAG_COUNT_C(a,b)  UTL_SET_BITFIELD((a), b, 5, 1)
    #define SA_CTX_PROTO_AC_GET_FLAG_COUNT_C(a)    UTL_GET_BITFIELD((a), 5, 1)
    #define SA_CTX_PROTO_AC_SET_FLAG_INS_COUNT_C(a,b)  UTL_SET_BITFIELD((a), b, 6, 1)
    #define SA_CTX_PROTO_AC_GET_FLAG_INS_COUNT_C(a)    UTL_GET_BITFIELD((a), 6, 1)
    #define SA_CTX_PROTO_AC_SET_DIR(a,b)           UTL_SET_BITFIELD((a), b, 7, 1)
    #define SA_CTX_PROTO_AC_GET_DIR(a)             UTL_GET_BITFIELD((a), 7, 1)
    #define SA_CTX_PROTO_AC_SET_FLAG_ENC(a,b)      UTL_SET_BITFIELD((a), b, 8, 1)
    #define SA_CTX_PROTO_AC_GET_FLAG_ENC(a)        UTL_GET_BITFIELD((a), 8, 1)
    #define SA_CTX_PROTO_AC_SET_FLAG_AUTH(a,b)     UTL_SET_BITFIELD((a), b, 9, 1)
    #define SA_CTX_PROTO_AC_GET_FLAG_AUTH(a)       UTL_GET_BITFIELD((a), 9, 1)
    #define SA_CTX_PROTO_AC_SET_FLAG_CMAC(a,b)     UTL_SET_BITFIELD((a), b, 10, 1)
    #define SA_CTX_PROTO_AC_GET_FLAG_CMAC(a)       UTL_GET_BITFIELD((a), 10, 1)
    #define SA_CTX_PROTO_AC_SET_FLAG_KASUMI_F9(a,b)     UTL_SET_BITFIELD((a), b, 11, 1)
    #define SA_CTX_PROTO_AC_GET_FLAG_KASUMI_F9(a)       UTL_GET_BITFIELD((a), 11, 1)
	#define SA_CTX_PROTO_AC_SET_FLAG_KEY_IN_SCRATCH(a,b)	UTL_SET_BITFIELD((a), b, 12, 1)
	#define SA_CTX_PROTO_AC_GET_FLAG_KEY_IN_SCRATCH(a)		UTL_GET_BITFIELD((a), 12, 1)
	#define SA_CTX_PROTO_AC_SET_FLAG_KASUMI_F8(a,b)	    UTL_SET_BITFIELD((a), b, 13, 1)
	#define SA_CTX_PROTO_AC_GET_FLAG_KASUMI_F8(a)		UTL_GET_BITFIELD((a), 13, 1)	
	#define SA_CTX_PROTO_AC_SET_FLAG_SNOW3G_ZUC(a,b)	UTL_SET_BITFIELD((a), b, 14, 1)
	#define SA_CTX_PROTO_AC_GET_FLAG_SNOW3G_ZUC(a)		UTL_GET_BITFIELD((a), 14, 1)	
	#define SA_CTX_PROTO_AC_SET_FLAG_F8F9(a,b)	        UTL_SET_BITFIELD((a), b, 15, 1)
	#define SA_CTX_PROTO_AC_GET_FLAG_F8F9(a)		    UTL_GET_BITFIELD((a), 15, 1)	
    #define SA_CTX_PROTO_AC_FLAG_IV_PRESENT        0x0008
    #define SA_CTX_PROTO_AC_FLAG_HDR_PRESENT       0x0010
    #define SA_CTX_PROTO_AC_FLAG_COUNTC_PRESENT    0x0020
    #define SA_CTX_PROTO_AC_FLAG_COUNTC_INSERT     0x0040
    
    uint16_t ctrlBitfield2;        /* various Air Cipher control information */            
    /*
     *  Bit 0 : Copy Count-C into timestamp filed at the descriptor
     *  Bit 1-15 : Reserved
     */ 
    #define SA_CTX_PROTO_AC_SET_FLAG_COPY_COUNT_C(a,b)  UTL_SET_BITFIELD((a), b, 0, 1)
    #define SA_CTX_PROTO_AC_GET_FLAG_COPY_COUNT_C(a)    UTL_GET_BITFIELD((a), 0, 1)
    #define SA_CTX_PROTO_AC_FLAG_COUNTC_COPY       0x0001
    
    uint16_t  firstEngIdCmdlLen;  /* Specify the engine id for the first command */
                                  /* Specify the size of the multiple command label */

    /*
     * The following parameters are used to extract the sequence number from the PDU header
     *   hdr_size: size ofPDU Header in bytes
     *   seq_num_size: size of the sequence number inside the header in bits
     *   seq_num_shift:        
     */
                                  
    uint16_t  hdrSizeAuthHdrSize;  
                                /* additional PCDP header to be authenticated, but not encrypted */ 
    uint16_t  seqNumSizeShift; 
                                /*  uint8_t   seq_num_shift;  */

    uint16_t  bearerIvSize;     /* 5-bit Bearer identity */
                                /* uint8_t   iv_size;  */           
    /*
     * F8F9 option type
     * 7:6  M: Number of option bytes in the beginning of payload
     * 5:5  Tx bit: 0: F8 followed by F9
     *              1: F9 followed by F8
     * 6:0 Packet Type as specified above 
     */  
    #define SA_F8F9_OPT_M_SHIFT         6
    #define SA_F8F9_OPT_M_MASK          0x03
    #define SA_F8F9_OPT_TX              0x20
    #define SA_F8F9_OPT_RX              0x00
                                  
    uint16_t  ivOptIcvSize;     /* The IV command option (or F8F9 option) */
                                /* The size of authentication tag in bytes. */


	/* Note: encKeyOffset and macKeyOffset must be set in conjunction 
			 with SA_CTX_PROTO_AC_GET_FLAG_KEY_IN_SCRATCH */
    uint16_t  encKeyOffset;		/* enc key byte offset in scratch memory */
    uint32_t  ivLow26;    /* The low 26-bits for the initialization vector  
                           * for 3GPP F8 these should be set to zero */
	uint16_t  macKeyOffset;     /* mac key byte offset in scratch memory */                 
    uint16_t  aux1[SA_AC_MAX_AUX_SIZE_IN_TUINT];   /* Auxiliary storage space for certain 
                                                      authentication algorithm */ 
    
} saCtxProtoAc_t;  /* 42/74 bytes */

/******************************************************************************
 * Type:        SA_CTX_PROTO_DM_T        
 ******************************************************************************
 * Description: This structure specifies the Data Mode specific  
 *              security context parameters which are initialized by SW and used  
 *              by Firmware.
 *
 * Note: The parameters which may be updated by the firmware should be arranged
 *       within the first 42 bytes so that the size of the evicted PHP parameters
 *       is limited to 64 bytes
 *
 *****************************************************************************/
typedef struct SA_CTX_PROTO_DM_tag
{
    uint32_t numPkts;             
    uint32_t numPktsHi;
    
    uint16_t  firstEngIdTagSize;  /* Specify the engine id for the first command and tag Size (round to 8 byte alignement) */
    uint16_t  rsvd;
    
} saCtxProtoDm_t;  /* 12 bytes */

/******************************************************************************
 * Type:        SA_CTX_PROTO_PARAMS_T        
 ******************************************************************************
 * Description: This is a simple union of the proto specific security 
 *              context parameter
 *              
 *****************************************************************************/
typedef union
{
    saCtxProtoSrtpTx_t        srtpTx;
    saCtxProtoSrtpRx_t        srtpRx;
    saCtxProtoIpsecAhTx_t     ipsecAhTx;
    saCtxProtoIpsecAhRx_t     ipsecAhRx;
    saCtxProtoIpsecEspTx_t    ipsecEspTx;
    saCtxProtoIpsecEspRx_t    ipsecEspRx;
    saCtxProtoAc_t            airCipher;
    saCtxProtoDm_t            dataMode;
    uint16_t                  data[54]; 
    
} SA_CTX_PROTO_PARAMS_T;


/******************************************************************************
 * Type:        SA_CTX_PKT_TYPE_T        
 ******************************************************************************
 * Description: This type represents the various packet types to be processed
 *              by the SA. It is used to identify the corresponding PHP 
 *              processing function. 
 *****************************************************************************/
typedef uint8_t SA_CTX_PKT_TYPE_T; 
#define SA_CTX_PKT_TYPE_3GPP_AIR    0    /* 3GPP Air Cipher */
#define SA_CTX_PKT_TYPE_SRTP        1    /* SRTP */
#define SA_CTX_PKT_TYPE_IPSEC_AH    2    /* IPSec Authentication Header */
#define SA_CTX_PKT_TYPE_IPSEC_ESP   3    /* IPSec Encapsulating Security Payload */
#define SA_CTX_PKT_TYPE_NONE        4    /* Indicates that it is in data mode,
                                            It may not be used by PHP */
#define SA_CTX_PKT_TYPE_MAX         SA_CTX_PKT_TYPE_NONE
                                            

/******************************************************************************
 * Type:        SA_CTX_COMMON_PARAMS_T  
 ******************************************************************************
 * Description: This structure defines the SA Security Context parameters
 *              which are common to all protocols.
 *              
 *****************************************************************************/
typedef struct SA_CTX_COMMON_PARAMS_tag
{
    uint16_t  ctrlFlagsDmaInfo;
    
    #define SA_CTX_SCCTL_FLAG_OWNER         0x8000
    /*
     * The following bit will be set by the host to indicate that it is waiting
     * for SA to write back the latest scurity context with this bit cleared. 
     *
     * It is primarily used for statistics query
     */
    
    #define SA_CTX_SCCTL_FLAG_WAIT_UPDATE   0x4000
    
    uint16_t  ctxId;    /* Security Context ID, filled by Hardware */
    uint32_t  ctxPtr;   /* Security Context Pointer, filled by Hardware */
    
    /*
     * Packet information type
     * 7:7 Packet Direction: 0: Tx (To Netwrk) (3GPP: From Air (uplink))
     *                       1: Rx (From Network) (3GPP: To Air (downlink)
     * 6:6 Use Local DMA   : 0: Use global DMA (default)
     *                       1: Use local DMA (thread Id |= 0x08 thread Id
     * 5:0 Packet Type as specified above 
     */  
    #define SA_CTX_PKT_DIR_RX           0x80
    #define SA_CTX_PKT_DIR_TX           0x00
    #define SA_CTX_CTRL_USE_LOC_DMA     0x40
    uint16_t  pktInfoByteFlowIndex;  
    uint16_t  destQueueId;  /* Destination Queue ID */
    uint32_t  swInfo0;      /* channel identifier: to be written to the swInfo0 in the packet descriptor */       
    uint32_t  swInfo1;      /* channel identifier: to be written to the swInfo0 in the packet descriptor */       
    uint16_t  pktId;        /* Packet ID: upper 8-bit during PHP Pass1 */
                            /* Lower 8-bit reserved for PDSP */
}   SA_CTX_COMMON_PARAMS_T;

#define SA_CTX_SCCTL_SIZE        8
//#define SA_CTX_PHP_COMMON_SIZE   (sizeof(SA_CTX_COMMON_PARAMS_T)*SALLD_SIZE_OF_WORD_IN_BYTE) 
#define SA_CTX_PHP_COMMON_SIZE   22
/*
*   These offsets indicate the offset in security context, from the end of common ctx to the 
*   aux1 field. 
*/
#define SA_CTX_ESP_AUX_OFFSET_TX    42
#define SA_CTX_ESP_AUX_OFFSET_RX    74
#define SA_CTX_AH_AUX_OFFSET_TX     42
#define SA_CTX_AH_AUX_OFFSET_RX     74
/*
 *  Bit 0-1: Fetch PHP Bytes
 *  Bit 2-3: Fetch Encryption/Air Ciphering Bytes
 *  Bit 4-5: Fetch Authentication Bytes for non sa2ul generations
 *  Bit 6-7: Evict PHP Bytes for non sa2ul generations
 *  Bit 4-6: Fetch Auth Bytes Applicable for sa2ul
 *  Bit 7  : Evict SCCTL word Applicable for sa2ul
 *
 *  where   00 = 0 bytes where it also means 256 bytes for PHP
 *          01 = 64 bytes
 *          10 = 96 bytes
 *          11 = 128 bytes
 *         100 = 160 bytes (applicable for SA2UL only)
 */
#define SA_CTX_DMA_SIZE_0       0
#define SA_CTX_DMA_SIZE_64      1
#define SA_CTX_DMA_SIZE_96      2
#define SA_CTX_DMA_SIZE_128     3

#if defined (NSS_LITE2)
#define SA_CTX_DMA_SIZE_160     4
#define SA_CTX_SCCTL_MK_DMA_INFO(fetch, enc, auth, evict)       ((fetch)        |  \
                                                                 ((enc) << 2)   |  \
                                                                 ((auth) << 4)  |  \
                                                                 ((evict) << 7))
#else
#define SA_CTX_SCCTL_MK_DMA_INFO(php_f, enc, auth, php_e)       ((php_f)        |  \
                                                                 ((enc) << 2)   |  \
                                                                 ((auth) << 4)  |  \
                                                                 ((php_e) << 6)) 
#endif
/*
 * Assumption: CTX size is mutilpe of 32
 */                                                                 
#define SA_CTX_SIZE_TO_DMA_SIZE(ctxSize)            ((ctxSize)?((ctxSize)/32 - 1):0)                                                                 
                                                                 
typedef struct
{
    uint16_t phpFetchSize;
    uint16_t engFetchSize[2];
    uint16_t phpEvictSize;

} saDMAReqInfo_t;

#define SA_CTX_PHP_SRTP_TX_SIZE              64
#define SA_CTX_PHP_SRTP_RX_SIZE             128
#define SA_CTX_PHP_IPSEC_TX_TYPE1_SIZE       64
#define SA_CTX_PHP_IPSEC_TX_TYPE2_SIZE       96   /* Including CMAC K1/K2 */
#define SA_CTX_PHP_IPSEC_RX_TYPE1_SIZE       96
#define SA_CTX_PHP_IPSEC_RX_TYPE2_SIZE      128   /* Including CMAC K1/K2 */  

#define SA_CTX_PHP_AC_SIZE                   64
#define SA_CTX_PHP_AC_TYPE1_SIZE             64
#define SA_CTX_PHP_AC_TYPE2_SIZE             96   /* Including CMAC K1/K2 */

#define SA_CTX_PHP_DATA_MODE_SIZE            64


/******************************************************************************
 * Type:        SA_CTX_HPS_T  
 ******************************************************************************
 * Description: This structure defines the SA Security Context parameters of
 *              the packet Header Processing Subsystem 
 *           
 * Note: This data structure is defined here for reference in concept.
 *       It is not used in the code.
 *       The Ctx command parameters and protocol-specific parameters are constructed
 *       independently as two big-endian byte arrays. There is no need to consider
 *       the alignment and padding between parameter common and proto.  
 *   
 *****************************************************************************/
typedef struct SA_CTX_HPS_tag
{
    SA_CTX_COMMON_PARAMS_T  common;     /* Common Parameters */
    SA_CTX_PROTO_PARAMS_T   proto;      /* Protocol Specific Parameters */
}   SA_CTX_HPS_T;


#define SA_MODE_CTRL_WORD_SIZE_IN_BYTE      27  

#define SA_ENC_KEY_SIZE_IN_UINT32           8
#define SA_ENC_AUX1_SIZE_IN_UINT32          8
#define SA_ENC_AUX2_SIZE_IN_UINT32          4
#define SA_ENC_AUX3_SIZE_IN_UINT32          4
#define SA_ENC_AUX4_SIZE_IN_UINT32          4
#define SA_ENC_RESERVED2_SIZE_IN_UINT32     4

#define SA_ENC_KEY_OFFSET                   32
#define SA_ENC_AUX1_OFFSET                  64
#define SA_ENC_AUX2_OFFSET                  96
#define SA_ENC_AUX3_OFFSET                  112 
#define SA_ENC_AUX4_OFFSET                  128 


#define SA_ENC_KEY_OFFSET_IN_UINT64         4
#define SA_ENC_AUX1_OFFSET_IN_UINT64        8
#define SA_ENC_AUX2_OFFSET_IN_UINT64        12
#define SA_ENC_AUX3_OFFSET_IN_UINT64        14 
#define SA_ENC_AUX4_OFFSET_IN_UINT64        16

#define SA_CTX_ENC_TYPE1_SIZE               64    /* Including Key only */
#define SA_CTX_ENC_TYPE2_SIZE               96    /* Including Key and Aux1 */


/* GCM Operation related definitions */
#define SA_GCM_CMDL_SIZE                    48    /* GCM Command Label Including
                                                   * - Basic Parameters (8 byte)
                                                   * - Length C         (8 byte)
                                                   * - AAD (Additional Authentication Data) (16 byte)
                                                   * - AES-CTR IV       (16 byte)
                                                   */
#define SA_GCM_ENC_SC_SIZE                  SA_CTX_ENC_TYPE2_SIZE /* Including Key and Aux1 */
#define SA_GCM_IV_SIZE                      16    /* AES-CTR IV */
#define SA_GCM_ENG_ID                       SALLD_CMDL_ENGINE_ID_ES1                                                     

#define SA_GCM_ENC_AUX1_LENA_OFFSET         23    /* Aux1 location to store the GCM Length A */
#define SA_GCM_ESP_AAD_LEN1                  8    /* GCM IPSEC Length A with ESN Disabled */
                                                  /* AAD: SPI + SN */
#define SA_GCM_ESP_AAD_LEN2                 12    /* GCM IPSEC Length A with ESN Enabled */ 
                                                  /* AAD: SPI + ESN */ 
#define SA_GCM_CMDL_OPT1                    SALLD_CMDL_MK_OPTION_CTRL((SA_ENC_AUX1_OFFSET + 24), 8)
#define SA_GCM_CMDL_OPT2                    SALLD_CMDL_MK_OPTION_CTRL((SA_ENC_AUX2_OFFSET), 16)
#define SA_GCM_CMDL_OPT3                    SALLD_CMDL_MK_OPTION_CTRL((SA_ENC_AUX3_OFFSET), 16)
                                                  
/* CCM Operation related definitions */
#define SA_CCM_CMDL_SIZE                    56    /* CCM Command Label Including
                                                   * - Basic Parameters (8 byte)
                                                   * - CCM B0           (16 byte)
                                                   * - CCM B1 (AAD and etc.) (16 byte)
                                                   * - CCM Counter      (16 byte)
                                                   */
#define SA_CCM_ENC_SC_SIZE                  SA_CTX_ENC_TYPE1_SIZE /* Including Key only */
#define SA_CCM_IV_SIZE                      16    /* AES-CTR IV */
#define SA_CCM_ENG_ID                       SALLD_CMDL_ENGINE_ID_ES1 

#define SA_CCM_CMDL_OPT1                    SALLD_CMDL_MK_OPTION_CTRL((SA_ENC_AUX2_OFFSET), 16)
#define SA_CCM_CMDL_OPT2                    SALLD_CMDL_MK_OPTION_CTRL((SA_ENC_AUX3_OFFSET), 16)
#define SA_CCM_CMDL_OPT3                    SALLD_CMDL_MK_OPTION_CTRL((SA_ENC_AUX4_OFFSET), 16)
                                                    

/* AES-CTR Operation related definitions */
#define SA_AESCTR_CMDL_SIZE                 24    /* AES-CTR Command Label Including
                                                   * - Basic Parameters (8 byte)
                                                   * - AEs-CTR IV      (16 byte)
                                                   */
#define SA_AESCTR_ENC_SC_SIZE               SA_CTX_ENC_TYPE1_SIZE /* Including Key only */
#define SA_AESCTR_IV_SIZE                   16    /* AES-CTR IV */
#define SA_AESCTR_ENG_ID                    SALLD_CMDL_ENGINE_ID_ES1                                                     


/* GMAC Operation related definitions */
#define SA_GMAC_CMDL_SIZE                   48    /* GMAC Command Label Including
                                                   * - Basic Parameters (8 byte)
                                                   * - Length A         (8 byte)
                                                   * - AAD (Additional Authentication Data) (16 byte)
                                                   * - AES-CTR IV       (16 byte)
                                                   */
#define SA_GMAC_CMDL_SIZE_NOAAD             32    /* GMAC Command Label Including
                                                   * - Basic Parameters (8 byte)
                                                   * - Length A         (8 byte)
                                                   * - AES-CTR IV       (16 byte)
                                                   */
                                                   
#define SA_GMAC_ENC_SC_SIZE                 SA_CTX_ENC_TYPE2_SIZE /* Including Key and Aux1 */
#define SA_GMAC_IV_SIZE                     16    /* AES-CTR IV */
#define SA_GMAC_ENG_ID                      SALLD_CMDL_ENGINE_ID_ES1                                                     

#define SA_GMAC_ESP_AAD_LEN1                 8    /* GMAC IPSEC Length A with ESN Disabled */
                                                  /* AAD: SPI + SN */
#define SA_GMAC_ESP_AAD_LEN2                12    /* GMAC IPSEC Length A with ESN Enabled */ 
                                                  /* AAD: SPI + ESN */ 
                                                  
#define SA_GMAC_CMDL_OPT1                    SALLD_CMDL_MK_OPTION_CTRL((SA_ENC_AUX1_OFFSET + 16), 8)
#define SA_GMAC_CMDL_OPT2                    SALLD_CMDL_MK_OPTION_CTRL((SA_ENC_AUX2_OFFSET), 16)
#define SA_GMAC_CMDL_OPT3                    SALLD_CMDL_MK_OPTION_CTRL((SA_ENC_AUX3_OFFSET), 16)
                                                   
                                                  
/* CMAC Operation related definitions */
#define SA_CMAC_CMDL_SIZE                   24    /* CMAC Command Label Including
                                                   * - Basic Parameters (8 byte)
                                                   * - K1/K2            (16 byte)
                                                   */
#define SA_CMAC_ENC_SC_SIZE                 SA_CTX_ENC_TYPE1_SIZE /* Including Key */
#define SA_CMAC_IV_SIZE                     0     /* IV is not used */
#define SA_CMAC_ENG_ID                      SALLD_CMDL_ENGINE_ID_ES1                                                     

/* SRTP Operation related definitions */
#define SA_SRTP_IV_SIZE                     16

/******************************************************************************
 * Type:        SA_CTX_ENC_T  
 ******************************************************************************
 * Description: This structure defines the SA Security Context parameters
 *              of the Encryption Engine.
 *              
 *****************************************************************************/
typedef struct SA_CTX_ENC_tag
{
    uint16_t ctrlBitMap_1stCtrlWord;
    
    #define SA_ENC_MODE_SEL_MASK        0x80
    #define SA_ENC_MODE_SEL_ENC         0x00
    #define SA_ENC_MODE_SEC_NULL        0x80

#if defined (NSS_LITE2)
    /* Mask for setting USE_DKEK flag in the encryption context RAM */
    #define SA_ENC_MODE_USE_DKEK        0x40
#endif

    #define SA_ENC_DEFAULT_ENG_ID_MASK  0x1F
    
    /* 
     * The reserved 2 bits will be used to contain the limited encryption modes
     * supported by the simulator
     */
    #define SA_SIM_ES_MODE_SEL_MASK        0x60
    #define SA_SIM_ES_MODE_SEL_SHIFT       5
    
    /* Encyrtion Modes supported by Encryption Engine Pass 1 & 2 (Encryption) */
    /* Note: Both encryption and authentication modes need to share the common index */
    #define SA_SIM_ES_ENC_MODE_NOT_SUPPORTED   0
    #define SA_SIM_ES_ENC_MODE_AESCTR          1
    #define SA_SIM_ES_ENC_MODE_GCM             2
    #define SA_SIM_ES_ENC_MODE_RESERVED1       3 
    
    /* Authentications Modes supported by Encryption Engine Pass 1 & 2 (Authentication) */
    #define SA_SIM_ES_AUTH_MODE_NOT_SUPPORTED   0
    #define SA_SIM_ES_AUTH_MODE_RESERVED1       1   
    #define SA_SIM_ES_AUTH_MODE_RESERVED2       2
    #define SA_SIM_ES_AUTH_MODE_CMAC            3 

#if defined (NSS_LITE2)
    /* For SAUL and SA2UL there is no simulator support and hence it is not required */
    #define SA_SIM_ES_GET_FUNC_INDEX(x)
    #define SA_SIM_ES_SET_FUNC_INDEX(a,b)
#else
    #define SA_SIM_ES_GET_FUNC_INDEX(x)         (((x)&SA_SIM_ES_MODE_SEL_MASK) >> SA_SIM_ES_MODE_SEL_SHIFT)  
    #define SA_SIM_ES_SET_FUNC_INDEX(a,b)       UTL_SET_BITFIELD((a), b, 5, 2)
#endif
    
    uint16_t ctrlWord[(SA_MODE_CTRL_WORD_SIZE_IN_BYTE - 1)/2];
    
    uint32_t reserved1;
    
    /* 
     * Key used for cipher operation, this key can also be loaded in-band 
     * via option bytes
     */
    uint16_t encKey[SA_ENC_KEY_SIZE_IN_UINT32*2];

    /* 
     * This field is used to store auxiliary data. This field is required 
     * to support certain encryption modes like CCM to store second key and 
     * can be loaded in-band via option bytes. Mode control engine cannot 
     * alter the value of this field
     */
    uint16_t aux1[SA_ENC_AUX1_SIZE_IN_UINT32*2];
    
    /* 
     * This is second Aux data that can be used if the encryption mode require IV. 
     * This value can be altered by Mode control engine and can also be loaded 
     * in-band via option bytes. 
     */
    uint16_t aux2[SA_ENC_AUX2_SIZE_IN_UINT32*2];

    /* 
     * This is third  Aux data that can be used if the encryption mode require nonce. 
     * This value can be altered by Mode control engine and can also be loaded in-band 
     * via option bytes.
     * 
     */
    uint16_t aux3[SA_ENC_AUX3_SIZE_IN_UINT32*2];

    /* 
     * Aux data 4 is used to store intermediate mode control data to be used for 
     * next block. This space cannot be loaded from main host, but can be loaded 
     * in-band via via option bytes
     */
    uint16_t aux4[SA_ENC_AUX4_SIZE_IN_UINT32*2];
    
    /*
     * Reserved Used by Engine only
     */
    uint16_t reserved2[SA_ENC_RESERVED2_SIZE_IN_UINT32*2];
}   saCtxEnc_t;

#define SA_CTX_AUTH_SIZE_IN_BYTES            160
#define SA_AUTH_KEY_SIZE_IN_UINT32           8
#define SA_AUTH_AUX1_SIZE_IN_UINT32          8
#define SA_AUTH_AUX2_SIZE_IN_UINT32          8
#define SA_AUTH_RESERVED2_SIZE_IN_UINT32     8

#define SA_AUTH_KEY_OFFSET                   32
#define SA_AUTH_AUX1_OFFSET                  64
#define SA_AUTH_AUX2_OFFSET                  96

#define SA_AUTH_KEY_OFFSET_IN_UINT64         4
#define SA_AUTH_AUX1_OFFSET_IN_UINT64        8
#define SA_AUTH_AUX2_OFFSET_IN_UINT64        12

#define SA_CTX_AUTH_TYPE1_SIZE               64    /* Including Key only */
#define SA_CTX_AUTH_TYPE2_SIZE               96    /* Including Key and Aux1 */
#define SA_CTX_AUTH_TYPE3_SIZE               160   /* Including Key and Aux1
                                                      for sha 384 and 512 */

/******************************************************************************
 * Type:        SA Enc Mode Control Instructions (MCI)  
 ******************************************************************************
 * Description: The following structures provides the mode control instructions
 *              of the SA encryption engine related definitions  
 *****************************************************************************/

/******************************************************************************
 * Type:        SA_ENG_ALGO_E  
 ******************************************************************************
 * Description: This structure defines the algorithms
 *              of the Encryption Engine or the Air Ciphering Engine.
 *              
 *****************************************************************************/
typedef enum 
{
    SA_ENG_ALGO_ECB = 0,
    SA_ENG_ALGO_CBC,
    SA_ENG_ALGO_CFB,
    SA_ENG_ALGO_OFB,
    SA_ENG_ALGO_CTR,
    SA_ENG_ALGO_F8,
    SA_ENG_ALGO_F8F9,
    SA_ENG_ALGO_GCM,
    SA_ENG_ALGO_GMAC,
    SA_ENG_ALGO_CCM,
    SA_ENG_ALGO_CMAC,
    SA_ENG_ALGO_CBCMAC,
    SA_ENG_ALGO_CCM_NO_AAD,
    SA_NUM_ENG_ALGOS
} saEngAlgo_e;

/******************************************************************************
 * Type:        SA_ENG_OP_CORE_E  
 ******************************************************************************
 * Description: This structure defines the operation cores supported by the
 *              SA Engines.
 *              
 *****************************************************************************/
typedef enum 
{
    SA_ENG_OP_CORE_AES = 0,
    SA_ENG_OP_CORE_3DES,
    SA_ENG_OP_CORE_KASUMI,
    SA_ENG_OP_CORE_SNOW3G,
    SA_ENG_OP_CORE_ZUC,
    SA_NUM_ENG_OP_CORES
} saEngOpCore_e;

/******************************************************************************
 * Type:        SA_ENG_KEY_SIZE_E  
 ******************************************************************************
 * Description: This structure defines supported key sizes
 *              SA Engines.
 *              
 *****************************************************************************/
typedef enum 
{
    SA_ENG_KEY_SIZE_128 = 0,
    SA_ENG_KEY_SIZE_192,
    SA_ENG_KEY_SIZE_256,
    SA_ENG_NUM_KEY_SIZES
} saEngKeySize_e;

/* Convert the kwy size (16/24/32) to the key size index (0/1/2) */
#define SA_ENG_CONV_KEY_SIZE(size)          (((size) >> 3) - 2)

/*
 *  3DES only supports ECB, CBC, CFB and OFB.
 */
#define SA_3DES_FIRST_ALGO          SA_ENG_ALGO_ECB
#define SA_3DES_LAST_ALGO           SA_ENG_ALGO_OFB
#define SA_3DES_NUM_ALGOS           (SA_3DES_LAST_ALGO - SA_3DES_FIRST_ALGO + 1) 

/*
 *  3DES only supports CBC 
 *  Note: reserve room for other algorithm
 */
#define SA_DES_FIRST_ALGO          SA_ENG_ALGO_ECB
#define SA_DES_LAST_ALGO           SA_ENG_ALGO_OFB
#define SA_DES_NUM_ALGOS           (SA_DES_LAST_ALGO - SA_DES_FIRST_ALGO + 1) 

/* 3GPP Engine related definitions */

#define SA_KASUMI_FIRST_ALGO        SA_ENG_ALGO_F8
#define SA_KASUMI_NUM_ALGOS         1        
 
#define SA_SNOW3G_FIRST_ALGO        SA_ENG_ALGO_F8
#define SA_SNOW3G_NUM_ALGOS_GEN1    1
#define SA_SNOW3G_NUM_ALGOS         2
#define SA_SNOW3G_AUTH_NUM_ALGOS    1

#define SA_ZUC_FIRST_ALGO           SA_ENG_ALGO_F8
#define SA_ZUC_NUM_ALGOS            2  
#define SA_ZUC_AUTH_NUM_ALGOS       1
  

#define SA_AUTH_ALGO_F9             (NUM_SA_ENG_ALGOS + 10)

#define SA_KASUMI_AUTH_DIR0     0     /* uplink (From-Air) */
#define SA_KASUMI_AUTH_DIR1     1     /* downlink (To-Air) */
#define SA_KASUMI_AUTH_NUM_DIRS 2     

#define SA_ENG_MAX_MCI_SIZE     27

/* Engine core specific MCI tables */
extern const uint8_t sa_eng_aes_enc_mci_tbl[SA_NUM_ENG_ALGOS][SA_ENG_NUM_KEY_SIZES][SA_ENG_MAX_MCI_SIZE];
extern const uint8_t sa_eng_aes_dec_mci_tbl[SA_NUM_ENG_ALGOS][SA_ENG_NUM_KEY_SIZES][SA_ENG_MAX_MCI_SIZE];
extern const uint8_t sa_eng_des_enc_mci_tbl[SA_DES_NUM_ALGOS][SA_ENG_MAX_MCI_SIZE];
extern const uint8_t sa_eng_des_dec_mci_tbl[SA_DES_NUM_ALGOS][SA_ENG_MAX_MCI_SIZE];
extern const uint8_t sa_eng_3des_enc_mci_tbl[SA_3DES_NUM_ALGOS][SA_ENG_MAX_MCI_SIZE];
extern const uint8_t sa_eng_3des_dec_mci_tbl[SA_3DES_NUM_ALGOS][SA_ENG_MAX_MCI_SIZE];
extern const uint8_t sa_eng_ah_gmac_mci_tbl[SA_ENG_NUM_KEY_SIZES][SA_ENG_MAX_MCI_SIZE];
extern uint8_t sa_eng_kasumi_enc_mci_tbl[SA_KASUMI_NUM_ALGOS][SA_ENG_MAX_MCI_SIZE];
extern uint8_t sa_eng_snow3g_enc_mci_tbl[SA_SNOW3G_NUM_ALGOS_GEN1][SA_ENG_MAX_MCI_SIZE];
extern uint8_t sa_eng_kasumi_auth_mci_tbl[SA_KASUMI_AUTH_NUM_DIRS][SA_ENG_MAX_MCI_SIZE];
extern uint8_t sa_eng_kasumi_enc_mci_tbl2[SA_KASUMI_NUM_ALGOS][SA_ENG_MAX_MCI_SIZE];
extern uint8_t sa_eng_snow3g_enc_mci_tbl2[SA_SNOW3G_NUM_ALGOS][SA_ENG_MAX_MCI_SIZE];
extern uint8_t sa_eng_zuc_enc_mci_tbl2[SA_ZUC_NUM_ALGOS][SA_ENG_MAX_MCI_SIZE];
extern uint8_t sa_eng_kasumi_auth_mci_tbl2[SA_KASUMI_AUTH_NUM_DIRS][SA_ENG_MAX_MCI_SIZE];
extern uint8_t sa_eng_snow3g_auth_mci_tbl2[SA_SNOW3G_AUTH_NUM_ALGOS][SA_ENG_MAX_MCI_SIZE];
extern uint8_t sa_eng_zuc_auth_mci_tbl2[SA_ZUC_AUTH_NUM_ALGOS][SA_ENG_MAX_MCI_SIZE];

/******************************************************************************
 * Type:        SA_CTX_AUTH_T  
 ******************************************************************************
 * Description: This structure defines the SA Security Context parameters
 *              of the Authentication Engine.
 *              
 *****************************************************************************/
typedef struct SA_CTX_AUTH_tag
{
    uint16_t ctrlBitMap;
    
    #define SA_AUTH_MODE_SEL_MASK         0x8000
    #define SA_AUTH_MODE_SEL_AUTH         0x0000
    #define SA_AUTH_MODE_SEC_NULL         0x8000
    #define SA_AUTH_DEFAULT_ENG_ID_MASK   0x1F00
    #define SA_AUTH_DEFAULT_ENG_ID_SHIFT       8
    #define SA_AUTH_FLAG_UPLOAD_HASH      0x0040   /* Upload hash to TLR */
    #define SA_AUTH_FLAG_DIS_MSG_PADDING  0x0080   /* Reserved: Do not pad message */
    #define SA_AUTH_FLAG_USE_MASTER_KEY   0x0020   /* Reserved: Always hash */
    #define SA_AUTH_FLAG_DIS_HMAC         0x0010   /* Basic Hash */
    #define SA_AUTH_HASH_MODE_MASK        0x000f
    #define SA_AUTH_HASH_MODE_NULL             0 
    #define SA_AUTH_HASH_MODE_MD5              1 
    #define SA_AUTH_HASH_MODE_SHA1             2 
    #define SA_AUTH_HASH_MODE_SHA2_224         3 
    #define SA_AUTH_HASH_MODE_SHA2_256         4 
    #define SA_AUTH_HASH_MODE_SHA2_384         5
    #define SA_AUTH_HASH_MODE_SHA2_512         6

    
    #define SA_SIM_AS_GET_FUNC_INDEX(x)         ((x) & SA_AUTH_HASH_MODE_MASK)  
    
    uint16_t reserved1[15];
    
    /*
     * For Non SA2UL:
     *    Master Key or Pre computed inner digest for HMAC. The field expects either
     *    Master key or Pre-computed inner digest.
     *    Hash(key xor inner constant). The inner pad must be padded to 256 bits by adding
     *    padding bits towards LSB.
     * For SA2UL:
     *    Bit [255:0] of Master Key or Pre computed
     *    inner digest for HMAC. The field expects
     *    either Master key or Pre-computed inner
     *    digest.
     *    Hash(key xor inner constant). The inner pad
     *    must be padded to 512 bits by adding padding
     *    bits towards LSB.
     */
    uint16_t authKey[SA_AUTH_KEY_SIZE_IN_UINT32*2];

    /*
     * For Non SA2UL:
     *    Pre computed outer opad for HMAC. This field expects the hash carries over opad.
     *    i.e. Hash(key xor outer constant). The outer digest must be padded to 256 bits
     *    by adding padding bits towards LSB.
     * For SA2UL:
     *    Bit [255:0] of Pre computed outer opad for
     *    HMAC. This field expects the hash carries
     *    over opad. i.e.
     *    Hash(key xor outer constant). The outer
     *    digest must be padded to 512 bits by adding
     *    padding bits towards LSB.
     */
    uint16_t aux1[SA_AUTH_AUX1_SIZE_IN_UINT32*2];
    
    /*
     * For Non SA2UL:
     *    This field stores the partial hash if the current block does not contain the
     *    complete packet. This value if restored into authentication core when next
     *    block of same packet is active
     * For SA2UL:
     *    Bit[511:256] of Master Key or Pre computed
     *    inner digest for HMAC. The field expects
     *    either Master key or Pre-computed inner
     *    digest.
     *    Hash(key xor inner constant). Must be
     *    populated for SHA2-512. Leave this blank for
     *    all other modes.
     */
    uint16_t aux2[SA_AUTH_AUX2_SIZE_IN_UINT32*2];

    /*
     * For Non SA2UL:
     *    Reserved Used by Engine only
     * for sa2ul:
     *    Bit [511:256] of Pre computed outer opad for
     *    HMAC. This field expects the hash carries
     *    over opad. i.e.
     *    Hash(key xor outer constant). Must be
     *    populated for SHA2-512. Leave this blank for
     *    all other modes.
     */
    uint16_t aux3[SA_AUTH_RESERVED2_SIZE_IN_UINT32*2];
}   saCtxAuth_t;

#define SA_AC_KEY_SIZE_IN_UINT32           8
#define SA_AC_KEY1_SIZE_IN_UINT32          4
#define SA_AC_KEY2_SIZE_IN_UINT32          4
#define SA_AC_AUX1_SIZE_IN_UINT32          8
#define SA_AC_AUX2_SIZE_IN_UINT32          4
#define SA_AC_AUX3_SIZE_IN_UINT32          4
#define SA_AC_AUX4_SIZE_IN_UINT32          4
#define SA_AC_RESERVED2_SIZE_IN_UINT32     4

#define SA_AC_AUX2_OFFSET                  96


#define SA_AC_KEY_OFFSET_IN_BYTES          32
#define SA_AC_KEY_OFFSET_IN_UINT64         4
#define SA_AC_KEY1_OFFSET_IN_UINT64        4
#define SA_AC_KEY2_OFFSET_IN_UINT64        6
#define SA_AC_AUX1_OFFSET_IN_UINT64        8
#define SA_AC_AUX1_2_OFFSET_IN_UINT64      10
#define SA_AC_AUX2_OFFSET_IN_UINT64        12
#define SA_AC_AUX3_OFFSET_IN_UINT64        14 
#define SA_AC_AUX4_OFFSET_IN_UINT64        16

#define SA_CTX_AC_TYPE1_SIZE               64    /* Including Key only */
#define SA_CTX_AC_TYPE2_SIZE               96    /* Including Key and Aux1 */

/******************************************************************************
 * Type:        SA_CTX_AC_T  
 ******************************************************************************
 * Description: This structure defines the SA Security Context parameters
 *              of the Air Ciphering Engine.
 *              
 *****************************************************************************/
typedef struct SA_CTX_AC_tag
{
    uint16_t ctrlBitMap_1stCtrlWord;
    
    /* Note: the bit definitions of control word may change */
    #define SA_AC_MODE_SEL_MASK        0x80
    #define SA_AC_MODE_SEL_ENC         0x00
    #define SA_AC_MODE_SEC_NULL        0x80
    #define SA_AC_DEFAULT_ENG_ID_MASK  0x1F
    
    /* 
     * The reserved 2 bits will be used to contain the limited encryption modes
     * supported by the simulator
     */
    #define SA_SIM_AC_MODE_SEL_MASK        0x60
    #define SA_SIM_AC_MODE_SEL_SHIFT       5
    
    /* Encyrtion Modes supported by Air Ciphering Engine Pass 1 & 2(Encryption) */
    /* Note: Both encryption and authentication modes need to share the common index */
    #define SA_SIM_AC_ENC_MODE_NOT_SUPPORTED   0
    #define SA_SIM_AC_ENC_MODE_SNOW3G_F8       1
    #define SA_SIM_AC_ENC_MODE_KASUMI_F8       2
    #define SA_SIM_AC_ENC_MODE_RESERVED1       3 
    
    /* Authentication Modes supported by Air Ciphering Engine Pass 1 & 2 (Authentication) */
    #define SA_SIM_AC_AUTH_MODE_NOT_SUPPORTED  0
    #define SA_SIM_AC_AUTH_MODE_KASUMI_F9      3   
    
    #define SA_SIM_AC_GET_FUNC_INDEX(x)         (((x) & SA_SIM_AC_MODE_SEL_MASK) >> SA_SIM_AC_MODE_SEL_SHIFT)  
    #define SA_SIM_AC_SET_FUNC_INDEX(a,b)       UTL_SET_BITFIELD((a), b, 5, 2)
    
    uint16_t ctrlWord[(SA_MODE_CTRL_WORD_SIZE_IN_BYTE - 1)/2];
    
    uint32_t reserved1;
    
    /* 
     * Key used for cipher operation, this key can also be loaded in-band 
     * via option bytes
     */
    uint16_t key[SA_AC_KEY_SIZE_IN_UINT32*2];

    /* 
     * This field is used to store auxiliary data. This field is required 
     * to support certain encryption modes like CCM to store second key and 
     * can be loaded in-band via option bytes. Mode control engine cannot 
     * alter the value of this field
     */
    uint16_t aux1[SA_AC_AUX1_SIZE_IN_UINT32*2];
    
    /* 
     * This is second Aux data that can be used if the encryption mode require IV. 
     * This value can be altered by Mode control engine and can also be loaded 
     * in-band via option bytes. 
     */
    uint16_t aux2[SA_AC_AUX2_SIZE_IN_UINT32*2];

    /* 
     * This is third  Aux data that can be used if the encryption mode require nonce. 
     * This value can be altered by Mode control engine and can also be loaded in-band 
     * via option bytes.
     * 
     */
    uint16_t aux3[SA_AC_AUX3_SIZE_IN_UINT32*2];

    /* 
     * Aux data 4 is used to store intermediate mode control data to be used for 
     * next block. This space cannot be loaded from main host, but can be loaded 
     * in-band via via option bytes
     */
    uint16_t aux4[SA_AC_AUX4_SIZE_IN_UINT32*2];
    
    /*
     * Reserved Used by Engine only
     */
    uint16_t reserved2[SA_AC_RESERVED2_SIZE_IN_UINT32*2];
} saCtxAc_t;

/* Software Data Word 0 */
/* 31 Reserved
 * 30 Destination Info Present
 * 29:25 Engine ID: specify the destination processing engine
 * 24:20 Command Label Info Bit 4: Command Label present
 *                          Bits 3:0: Command label offset in 8-bytes
 * 19:16 DMA Control flags
 * 19 Reserved
 * 18 No Paylaod Flag
 * 17 Tear Flag
 * 16 Evict Flag
 * 15:00 SC ID: Security Context ID
 */
#define SA_SW_GET_SC_ID(a)                  UTL_GET_BITFIELD((a), 0, 16)
#define SA_SW_SET_SC_ID(a, b)               UTL_SET_BITFIELD((a), (b), 0, 16)
#define SA_SW_GET_CMDLB_OFFSET(a)           UTL_GET_BITFIELD((a), 20, 4) 
#define SA_SW_SET_CMDLB_OFFSET(a, b)        UTL_SET_BITFIELD((a), (b), 20, 4)
#define SA_SW_IS_CMDLB_PRESENT(a)           UTL_GET_BITFIELD((a), 24, 1)
#define SA_SW_SET_CMDLB_PRESENT(a, b)       UTL_SET_BITFIELD((a), (b), 24, 1)
#define SA_SW_GET_ENG_ID(a)                 UTL_GET_BITFIELD((a), 25, 5)
#define SA_SW_SET_ENG_ID(a, b)              UTL_SET_BITFIELD((a), (b), 25, 5)
#define SA_SW_IS_DEST_INFO_PRESENT(a)       UTL_GET_BITFIELD((a), 30, 1)
#define SA_SW_SET_DEST_INFO_PRESENT(a, b)   UTL_SET_BITFIELD((a), (b), 30, 1)
#define SA_SW_READ_FLAG_NOPAYLOAD(a)        UTL_GET_BITFIELD((a), 18, 1)
#define SA_SW_SET_FLAG_NOPAYLOAD(a, b)      UTL_SET_BITFIELD((a), (b), 18, 1)
#define SA_SW_READ_FLAG_TEAR(a)             UTL_GET_BITFIELD((a), 17, 1)
#define SA_SW_SET_FLAG_TEAR(a, b)           UTL_SET_BITFIELD((a), (b), 17, 1)
#define SA_SW_READ_FLAG_EVICT(a)            UTL_GET_BITFIELD((a), 16, 1)
#define SA_SW_SET_FLAG_EVICT(a, b)          UTL_SET_BITFIELD((a), (b), 16, 1)

#define SA_SC_FLAGS_EVICT                   0x01    /* Evict the security context */
#define SA_SC_FLAGS_TEAR                    0x02    /* Tear down the security context */
#define SA_SC_FLAGS_NOPAYLOAD               0x04    /* NO payload with the packet */  
 

/* Software Data Word 1 */
/* 31:0 SC PTR: Security Context Pointer */
#define SA_SW_GET_SC_PTR(a)            (a)

/* Software Data Word 2 */
/* 31:24 hash size */
/* 23:16 Flow Index */
/* 15:00 DEstination Queue */
#define SA_FORM_SW2(queueID, flowIndex, hashSize)       ((uint32_t)(queueID))                   |      \
                                                        ((uint32_t)((flowIndex) & 0xFF) << 16 ) |      \
                                                        ((uint32_t)((hashSize) & 0xFF) << 24)
                                                        

/* Software Data Word 2 for SA2_UL */
/* 31:24 hash size */
/* 15:00 security context pointer high 16-bits */
#define SA2_UL_FORM_SW2(scptrh, hashSize)               ((uint16_t)((scptrh) & 0xFFFF))                   |      \
                                                        ((uint32_t)((hashSize) & 0xFF) << 24)

/* Software Data Word 3 for SA2_UL, that should be put into first 32-bits of Protocol Specific word
 * 31:16 Egress CPPI Destination queue
 * 15:4  Reserved
 * 29:25 Engine ID: specify the destination processing engine
 * 24:20 Command Label Info Bit 4: Command Label present
 *                          Bits 3:0: Command label offset in 8-bytes
 * 19:16 DMA Control flags
 * 19 Reserved
 * 3  Non Secure Crypto
 * 2  Demote
 * 1  Promote
 * 0  Reserved
 */
#define SA2_UL_SW_GET_PROMOTE(a)                  UTL_GET_BITFIELD((a), 1, 1)
#define SA2_UL_SW_SET_PROMOTE(a, b)               UTL_SET_BITFIELD((a), (b), 1, 1)
#define SA2_UL_SW_GET_DEMOTE(a)                   UTL_GET_BITFIELD((a), 2, 1)
#define SA2_UL_SW_SET_DEMOTE(a, b)                UTL_SET_BITFIELD((a), (b), 2, 1)
#define SA2_UL_SW_GET_NONSEC_CRYPO(a)             UTL_GET_BITFIELD((a), 3, 1)
#define SA2_UL_SW_SET_NONSEC_CRYPO(a, b)          UTL_SET_BITFIELD((a), (b), 3, 1)
#define SA2_UL_SW_GET_EGRESS_DEST_QUEUE(a)        UTL_GET_BITFIELD((a), 16, 16)
#define SA2_UL_SW_SET_EGRESS_DEST_QUEUE(a, b)     UTL_SET_BITFIELD((a), (b), 16, 16)

/* Define SW Info Control Flags */
#define SA_SW_INFO_FLAG_CMLB_PRESENT        0x0100
#define SA_SW_INFO_FLAG_NOPAYLOAD           0x0004                                                        
#define SA_SW_INFO_FLAG_TEAR                0x0002 
#define SA_SW_INFO_FLAG_EVICT               0x0001  
#define SA_SW_INFO_FLAG_SHIFT               16
#define SA_SW_INFO_FLAG_MASK                ((SA_SW_INFO_FLAG_CMLB_PRESENT |   \
                                              SA_SW_INFO_FLAG_NOPAYLOAD    |   \
                                              SA_SW_INFO_FLAG_TEAR         |   \
                                              SA_SW_INFO_FLAG_EVICT) << SA_SW_INFO_FLAG_SHIFT)

#define SA_SW_INFO_SET_FLAGS(a, b)          (a) = ((a) & ~SA_SW_INFO_FLAG_MASK) | ((b) << SA_SW_INFO_FLAG_SHIFT)
                                                     

#endif /* _SACTX_H */

