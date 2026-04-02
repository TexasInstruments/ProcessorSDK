#ifndef _SALLDSRTCPLOC_H
#define _SALLDSRTCPLOC_H
/******************************************************************************
 * FILE PURPOSE: Secure RTCP Local Header File
 ******************************************************************************
 * FILE NAME: salldsrtcploc.h
 *
 * DESCRIPTION: Secure RTCP Local defines and header files
 *
 *              
 * 
 *
 * (C) Copyright 2009, Texas Instruments Inc.
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
/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include "src/salldloc.h"
#include "salldsrtcp.h"
#include "salldsrtploc.h"

typedef Sa_SrtpConfigParams_t  salldSrtcpKeySizeParams_t;

typedef struct salldSrtcpFromTo_s {
  uint16_t   masterKey[SALLD_SRTP_MASTER_KEY_SIZE_IN_TUINT];    /* */
  uint16_t   masterSalt[SALLD_SRTP_MASTER_SALT_SIZE_IN_TUINT];  /* */
  uint16_t   kdBitfield;
  uint32_t   from;               /* 32-bits "from" index  */
  uint32_t   to;                 /* 32-bits "to" index  */
} salldSrtcpFromTo_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD SRTCP Key Structure
 ******************************************************************************
 * DESCRIPTION: Define the SRTCP Key related parameters
 *  
 *****************************************************************************/
typedef struct salldSrtcpKeyInfo_s{
  salldSrtcpKeySizeParams_t  keySize;
  uint16_t   masterKey[SALLD_SRTP_MASTER_KEY_SIZE_IN_TUINT];    /* */
  uint16_t   masterSalt[SALLD_SRTP_MASTER_SALT_SIZE_IN_TUINT];  /* */
  uint16_t   nRekey;
  uint16_t   kdBitfield;         /* bitfields for key derivation and rekeying purposes */
          /* |54321|0|9|8|7|6|5|43210| */
          /* |xxxxx|-|-|-|-|-|-|-----| */
          /* 0-4 bits are key_derivation_rate kdr of from 2^kdr */
          /* 0x1F means only once keys need to be derived */
          /* otherwise it belongs to {1,2,4,..,24} */
          /* bit 5: new_key_available */
          /* bit 6: first key derivation */
          /* bit 7: new master key */
          /* bit 8: new master salt */
          /* bit 9: MKI active 1 = on/0 = off */
          /* bit 10: <from,to> active 1/inactive 0 */
  uint16_t   sessionEncKey[SALLD_SRTP_CIPHER_KEY_SIZE_IN_TUINT]; /* Tx Session Key */
  uint16_t   sessionSalt[SALLD_SRTP_SESSION_SALT_SIZE_IN_TUINT];  /* Tx Session Salt */
  uint16_t   sessionAuthKey[SALLD_SRTP_MAC_KEY_SIZE_IN_TUINT];   /* Tx Authentication Key */
  int16_t    mkiLength;          /* MKI length in the Packet */
  uint32_t   mki;                /* Master Key Identifier in the context */
  uint32_t   index;              /* RTCP Index 31 bit value */
  uint32_t   keyLifetime;        /* Number of Packets encrypted with this master key */
  uint32_t   r;
  uint32_t   from;               /* 32-bits "from" index  */
  uint32_t   to;                 /* 32-bits "to" index  */
  salldSrtcpFromTo_t fromToBuffer;
} salldSrtcpKeyInfo_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD SRTCP TX Instance Structure
 ******************************************************************************
 * DESCRIPTION: for Tx side
 *  
 *****************************************************************************/
typedef struct salldSrtcpTxInst_s{
  uint16_t   cipherMode;
  uint16_t   authMode;
  uint32_t   packetEnc;          /* Number of Packets encrypted with this master key */
  salldSrtcpKeyInfo_t keyInfo; /* Key related parameters */
} salldSrtcpTxInst_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD SRTCP RX Instance Structure
 ******************************************************************************
 * DESCRIPTION: for Rx side
 *  
 *****************************************************************************/
typedef struct salldSrtcpRxInst_s{
  uint16_t   cipherMode;
  uint16_t   authMode;
  uint32_t   packetDec;          /* Number of Packets decrypted with this master key */
  uint16_t   windowCheck;        /* 0, 64 bit sliding window check */
  salldWindow_t       replayWindow;    /* Sequence Number sliding window */
  salldSrtcpKeyInfo_t keyInfo; /* Key related parameters */
} salldSrtcpRxInst_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD SRTCP Instance Structure
 ******************************************************************************
 * DESCRIPTION: The first element is always the salldInst_t derived data type
 *  
 *****************************************************************************/
typedef struct salldSrtcpInst_s{
  salldInst_t    salldInst;   /* The SALLD Instance */
  salldSrtcpTxInst_t  txInst; /* SRTCP TX Instance */
  salldSrtcpRxInst_t  rxInst; /* SRTCP RX Instance */
} salldSrtcpInst_t;

/******************************************************************************
 * SRTCP module global functions 
 *****************************************************************************/
/* salldsrtcpinit.c */
int16_t salld_srtcp_init (void *salldInst, void *cfg);
int16_t salld_srtcp_close (void *salldInst);

/* salldsrtcp.c */
int16_t salld_srtcp_control (void *salldInst, void *ctrl);
int16_t salld_srtcp_get_stats (void *salldInst, uint16_t flags, void *stats);
int16_t salld_srtcp_send_data (void *salldInst, void *pktInfo, uint16_t clear);
int16_t salld_srtcp_receive_data (void *salldInst, void *pktInfo);

/* salldsrtcp_ctr.c */
int16_t salld_srtcp_form_ctr_iv(void* keyInfo, tword *pkt, tword *iv, uint32_t index);

/* salldsrtcp_f8.c */
int16_t salld_srtcp_form_f8_iv(void* keyInfo, tword *pkt, tword *iv, uint32_t index);


#endif /* _SALLDSRTCPLOC_H */
