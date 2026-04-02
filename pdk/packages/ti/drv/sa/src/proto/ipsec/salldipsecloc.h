#ifndef _SALLDIPSECLOC_H
#define _SALLDIPSECLOC_H
/******************************************************************************
 * FILE PURPOSE: IPSEC Local Header File
 ******************************************************************************
 * FILE NAME:salldahloc.h
 *
 * DESCRIPTION: IPSEC Local defines and header files
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
#include "salldipsec.h"

#define SALLD_IPSEC_MAX_ENC_KEY_SIZE                 (32)   /* 256 bits */
#define SALLD_IPSEC_MAX_MAC_KEY_SIZE                 (64)   /* 512 bits */
#define SALLD_IPSEC_MAX_SALT_SIZE                     (4)   /* 32 bits */

#define SALLD_IPSEC_MAX_IV_SIZE                      (16)   /* 128 bits */
#define SALLD_IPSEC_MAX_MAC_TAG_SIZE                 (16)   /* 256 bits */
#define SALLD_IPSEC_MAX_REPLAY_WINDOW_SIZE_SHORT_CTX (128)  /* Maxmium window size for small size of security context */
                                                            /* If the replay window size exceeds this limit, the PHP context size
                                                               will be increased to 256 unconditionally */
#define SALLD_IPSEC_MAX_REPLAY_WINDOW_SIZE           (1024) /* 1024 packets */

#define SALLD_IPSEC_MAX_ENC_KEY_SIZE_IN_TUINT        SALLD_BYTE_TO_TUINT(SALLD_IPSEC_MAX_ENC_KEY_SIZE) 
#define SALLD_IPSEC_MAX_MAC_KEY_SIZE_IN_TUINT        SALLD_BYTE_TO_TUINT(SALLD_IPSEC_MAX_MAC_KEY_SIZE) 
#define SALLD_IPSEC_MAX_SALT_SIZE_IN_TUINT           SALLD_BYTE_TO_TUINT(SALLD_IPSEC_MAX_SALT_SIZE)
#define SALLD_IPSEC_MAX_IV_SIZE_IN_TUINT             SALLD_BYTE_TO_TUINT(SALLD_IPSEC_MAX_IV_SIZE)
#define SALLD_IPSEC_MAX_MAC_TAG_SIZE_IN_TUINT        SALLD_BYTE_TO_TUINT(SALLD_IPSEC_MAX_MAC_TAG_SIZE) 

/******************************************************************************
 * DATA DEFINITION:  SALLD IPSEC Common Structure
 ******************************************************************************
 * DESCRIPTION: Define the IPSEC Common parameters for both Tx and Rx
 *  
 *****************************************************************************/
typedef struct salldIpsecComInfo_s{
  Sa_IpsecConfigParams_t  config;
  uint16_t   sessionSalt[SALLD_IPSEC_MAX_SALT_SIZE_IN_TUINT];      /* Salt */
  uint16_t   sessionEncKey[SALLD_IPSEC_MAX_ENC_KEY_SIZE_IN_TUINT]; /* Enc Key */
  uint16_t   sessionMacKey[SALLD_IPSEC_MAX_MAC_KEY_SIZE_IN_TUINT]; /* Mac Key */
  uint32_t   esn;                /* Present Extended Sequence Number */
  uint32_t   sn;                 /* Present Sequence Number */
} salldIpsecComInfo_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD IPSEC TX Instance Structure
 ******************************************************************************
 * DESCRIPTION: for Tx side
 *  
 *****************************************************************************/
typedef struct salldIpsecTxInst_s{
  uint16_t   cipherMode;
  uint16_t   authMode;
  uint32_t   lastSeqNum;          /* Last Sequence Number for Tx */
  uint32_t   packetEncLsw;        /* Number of Packets encrypted with this master key */
  uint32_t   packetEncMsw;        /* Number of Packets encrypted with this master key */
  uint32_t   packetRollover;      /* Number of Packets dropped due to sequence rollover error */
  uint32_t   byteCount;           /* Number of bytes processed by SA on TX */
  uint32_t   byteCountHi;         /* Number of bytes processed by SA on TX */
  Sa_ScReqInfo_t   scInfo;    /* Security Context Information */
  Sa_SWInfo_t      swInfo;    /* SA-specific SW Information */
  Sa_DestInfo_t    destInfo;  /* Destination related information */
  salldIpsecComInfo_t comInfo;  /* Common parameters */
} salldIpsecTxInst_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD IPSEC RX Instance Structure
 ******************************************************************************
 * DESCRIPTION: for Rx side
 *  
 *****************************************************************************/
typedef struct salldIpsecRxInst_s{
  uint16_t   cipherMode;
  uint16_t   authMode;
  uint32_t   lastSeqNum;          /* Last Sequence Number for Rx */
  uint32_t   packetDecLsw;        /* Number of Packets decrypted with this master key */
  uint32_t   packetDecMsw;        /* Number of Packets decrypted with this master key */
  uint32_t   paddingFail;         /* Number of packets with padding verification failure */
  uint16_t   windowCheck;         /* 0, 64 bit sliding window check */
  uint32_t   byteCount;           /* Number of bytes processed by SA on RX */
  uint32_t   byteCountHi;         /* Number of bytes processed by SA on RX */
  Sa_ScReqInfo_t   scInfo;    /* Security Context Information */
  Sa_SWInfo_t      swInfo;    /* SA-specific SW Information */
  Sa_DestInfo_t    destInfo;  /* Destination related information */
  salldIpsecComInfo_t comInfo;  /* Common parameters */
} salldIpsecRxInst_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD IPSEC Instance Structure
 ******************************************************************************
 * DESCRIPTION: The first element is always the msuInst_t derived data type
 *  
 *****************************************************************************/
typedef struct salldIpsecInst_s{
  salldInst_t    salldInst;  /* The SALLD Instance */
  salldIpsecTxInst_t  txInst; /* IPSEC TX Instance */
  salldIpsecRxInst_t  rxInst; /* IPSEC RX Instance */
} salldIpsecInst_t;

/* salldipsecinit.c */
int16_t salld_ipsec_init (void *salldInst, void *cfg);
int16_t salld_ipsec_close (void *salldInst);

/* salldipsec.c */
int16_t salld_ipsec_control (void *salldInst, void *ctrl);
int16_t salld_ipsec_get_stats (void *salldInst, uint16_t flags, void *stats);
void salld_ipsec_gen_iv(tword *iv, int16_t size);

/* salldesp.c */
int16_t salld_esp_send_data (void *salldInst, void *pktInfo, uint16_t clear);
int16_t salld_esp_receive_data (void *salldInst, void *pktInfo);
int16_t salld_esp_set_tx_sc(salldIpsecInst_t *inst); 
int16_t salld_esp_set_rx_sc(salldIpsecInst_t *inst); 

/* salldah.c */
int16_t salld_ah_send_data (void *salldInst, void *pktInfo, uint16_t clear);
int16_t salld_ah_receive_data (void *salldInst, void *pktInfo);
int16_t salld_ah_set_tx_sc(salldIpsecInst_t *inst); 
int16_t salld_ah_set_rx_sc(salldIpsecInst_t *inst); 

#endif /* _SALLDIPSECLOC_H */
