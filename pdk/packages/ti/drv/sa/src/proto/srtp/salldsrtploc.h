#ifndef _SALLDSRTPLOC_H
#define _SALLDSRTPLOC_H
/******************************************************************************
 * FILE PURPOSE: Secure RTP Local Header File
 ******************************************************************************
 * FILE NAME:salldsrtploc.h
 *
 * DESCRIPTION: Secure RTP Local defines and header files
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
#include "salldsrtp.h"

/* Used in getting the right ROC/SEQ number */
/******************************************************************************
 * DATA DEFINITION:  SRTP ROC Transition States
 ******************************************************************************
 * DESCRIPTION: Define SRTP ROC Transition States
 *  
 *****************************************************************************/
#define SALLD_ROC_NOCHANGE    0
#define SALLD_ROC_MINUS_1     1
#define SALLD_ROC_PLUS_1      2

/* Defines when the key expiry notice will be sent by SALLD before the key expires */
/* NOTE: these values are just an example, may be changed later */
#define SALLD_RTP_KEY_LIFE_MARGIN       10
#define SALLD_RTCP_KEY_LIFE_MARGIN      1

/* Key Validation Results */
#define SALLD_KEY_VALIDATION_OK            0
#define SALLD_KEY_VALIDATION_NEW_KEY       1
#define SALLD_KEY_VALIDATION_NEW_KEY_NOW   2
#define SALLD_KEY_VALIDATION_ERROR         3

#define SALLD_SRTP_MASTER_KEY_SIZE                  (16)   /* 128 bits */
#define SALLD_SRTP_MASTER_SALT_SIZE                 (14)   /* 112 bits */
#define SALLD_SRTP_CIPHER_KEY_SIZE                  (16)   /* 128 bits */
#define SALLD_SRTP_SESSION_SALT_SIZE                (14)   /* 112 bits */
#define SALLD_SRTP_MAC_KEY_SIZE                     (20)   /* 160 bits */
#define SALLD_SRTP_MAX_MAC_TAG_SIZE                 (10)   /* 80bits */
#define SALLD_SRTP_MAX_SESSION_KEY_SIZE             (20)   /* = mac key size */
#define SALLD_SRTP_MAX_REPLAY_WINDOW_SIZE           (64)   /* 64 packets */
                      
#define SALLD_SRTP_MASTER_KEY_SIZE_IN_TUINT         (SALLD_SRTP_MASTER_KEY_SIZE/SALLD_SIZE_OF_TUINT_IN_BYTE)
#define SALLD_SRTP_MASTER_SALT_SIZE_IN_TUINT        (SALLD_SRTP_MASTER_SALT_SIZE/SALLD_SIZE_OF_TUINT_IN_BYTE)
#define SALLD_SRTP_CIPHER_KEY_SIZE_IN_TUINT         (SALLD_SRTP_CIPHER_KEY_SIZE/SALLD_SIZE_OF_TUINT_IN_BYTE)
#define SALLD_SRTP_SESSION_SALT_SIZE_IN_TUINT       (SALLD_SRTP_SESSION_SALT_SIZE/SALLD_SIZE_OF_TUINT_IN_BYTE)
#define SALLD_SRTP_MAC_KEY_SIZE_IN_TUINT            (SALLD_SRTP_MAC_KEY_SIZE/SALLD_SIZE_OF_TUINT_IN_BYTE)
#define SALLD_SRTP_MAX_MAC_TAG_SIZE_IN_TUINT        (SALLD_SRTP_MAX_MAC_TAG_SIZE/SALLD_SIZE_OF_TUINT_IN_BYTE)
#define SALLD_SRTP_MAX_SESSION_KEY_SIZE_IN_TUINT    (SALLD_SRTP_MAX_SESSION_KEY_SIZE/SALLD_SIZE_OF_TUINT_IN_BYTE)
#define SALLD_SRTP_MAX_SESSION_KEY_SIZE_IN_WORD     (SALLD_SRTP_MAX_SESSION_KEY_SIZE/SALLD_SIZE_OF_WORD_IN_BYTE)


#define SRTP_MASK                         0x0007
#define SRTP_DERIVE_CIPHER_KEY            0x0001
#define SRTP_DERIVE_MAC_KEY               0x0002
#define SRTP_DERIVE_SALT_KEY              0x0004

#define SALLD_SRTP_KD_RATE_MASK           0x001F
#define SALLD_SRTP_NEW_KEY_MASK           0x0020
#define SALLD_SRTP_FIRST_KD_MASK          0x0040
#define SALLD_SRTP_NEW_MASTER_KEY_MASK    0x0080
#define SALLD_SRTP_NEW_MASTER_SALT_MASK   0x0100
#define SALLD_SRTP_MKI_MASK               0x0200
#define SALLD_SRTP_FROM_TO_MASK           0x0400
#define SALLD_SRTP_KEY_EXPIRE_MASK        0x0800
#define SALLD_SRTP_KEY_REQUEST_MASK       0x1000

/******************************************************************************
 * DATA DEFINITION:  SRTP ReKey States
 ******************************************************************************
 * DESCRIPTION: Define SRTP Re-Key states
 *  
 *****************************************************************************/
#define SRTP_REKEY_STATE_IDLE           0    /* No Re-key operation pending */
#define SRTP_REKEY_STATE_WAIT           1    /* Wait for the re-key transition conditions
                                              * to be satisfied (From-Network only)
                                              * - sequence number exceeds re-play window size
                                              * - all packets with the previous R number
                                              *   is received
                                              */
#define SRTP_REKEY_STATE_NEW_SC         2    /* Generate and register the new SC */
#define SRTP_REKEY_STATE_FREE_SC        3    /* Free the pending SC when it is released
                                              * from SA
                                              */                                              



#define SRTP_ACTIVE_SC_INDEX        0
#define SRTP_PENDING_SC_INDEX       1

typedef Sa_SrtpConfigParams_t  salldSrtpKeySizeParams_t;

typedef struct salldSrtpFromTo_s {
  uint16_t   masterKey[SALLD_SRTP_MASTER_KEY_SIZE_IN_TUINT];    /* */
  uint16_t   masterSalt[SALLD_SRTP_MASTER_SALT_SIZE_IN_TUINT];  /* */
  uint16_t   kdBitfield;
  uint16_t   fromLsb;            /* lower 16 bits of "from" index == SEQ */
  uint16_t   toLsb;              /* lower 16 bits of "to" index <from, to> == SEQ */
  uint32_t   fromMsb;            /* upper 32 bits of "from" index == roc */
  uint32_t   toMsb;              /* upper 32 bits of "to" index <from, to> == roc */
} salldSrtpFromTo_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD SRTP Key Structure
 ******************************************************************************
 * DESCRIPTION: Define the SRTP Key related parameters
 *  
 *****************************************************************************/
typedef struct salldSrtpKeyInfo_s{
  salldSrtpKeySizeParams_t  keySize;
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
  uint16_t   keyLifetimeMsb;     /* Number of Packets encrypted with this master key */
  uint16_t   fromLsb;            /* lower 16 bits of "from" index == SEQ */
  uint16_t   toLsb;              /* lower 16 bits of "to" index <from, to> == SEQ */
  uint16_t   rMsb;
  int16_t    mkiLength;          /* MKI length in the Packet */
  uint32_t   mki;                 /* Master Key Identifier in the context */
  uint32_t   roc;                /* Present Tx ROC */
  uint32_t   keyLifetimeLsb;     /* Number of Packets encrypted with this master key */
  uint32_t   rLsb;
  uint32_t   fromMsb;            /* upper 32 bits of "from" index == roc */
  uint32_t   toMsb;              /* upper 32 bits of "to" index <from, to> == roc */
  uint32_t   seqNumBase;         /* starting sequence number for the new master or
                                  session keys */ 
  salldSrtpFromTo_t fromToBuffer;
} salldSrtpKeyInfo_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD SRTP TX Instance Structure
 ******************************************************************************
 * DESCRIPTION: for Tx side
 *  
 *****************************************************************************/
typedef struct salldSrtpTxInst_s{
  uint16_t   cipherMode;
  uint16_t   authMode;
  uint16_t   lastSeqNum;          /* Last SEQ for Tx */
  uint16_t   packetEncMsw;        /* Number of Packets encrypted with this master key */
  uint32_t   packetEncLsw;        /* Number of Packets encrypted with this master key */
  uint16_t   rekeyState;          /* as defined above */
  Sa_ScReqInfo_t   scInfo[2]; /* Security Context Information */
  Sa_SWInfo_t      swInfo;    /* SA-specific SW Information */
  Sa_DestInfo_t    destInfo;  /* Destination related information */
  salldSrtpKeyInfo_t keyInfo;   /* Key related parameters */
} salldSrtpTxInst_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD SRTP RX Instance Structure
 ******************************************************************************
 * DESCRIPTION: for Rx side
 *  
 *****************************************************************************/
typedef struct salldSrtpRxInst_s{
  uint16_t   cipherMode;
  uint16_t   authMode;
  uint16_t   lastSeqNum;          /* SEQ for Rx */
  uint16_t   packetDecMsw;        /* Number of Packets decrypted with this master key */
  uint32_t   packetDecLsw;        /* Number of Packets decrypted with this master key */
  uint16_t   windowCheck;         /* 0, 64 bit sliding window check */
  uint16_t   rekeyState;          /* as defined above */
  salldWindow_t      replayWindow;    /* Sequence Number sliding window */
  Sa_ScReqInfo_t   scInfo[2]; /* Security Context Information */
  Sa_SWInfo_t      swInfo;    /* SA-specific SW Information */
  Sa_DestInfo_t    destInfo;  /* Destination related information */
  salldSrtpKeyInfo_t keyInfo;   /* Key related parameters */
} salldSrtpRxInst_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD SRTP Instance Structure
 ******************************************************************************
 * DESCRIPTION: The first element is always the msuInst_t derived data type
 *  
 *****************************************************************************/
typedef struct salldSrtpInst_s{
  salldInst_t    salldInst;  /* The SALLD Instance */
  salldSrtpTxInst_t  txInst; /* SRTP TX Instance */
  salldSrtpRxInst_t  rxInst; /* SRTP RX Instance */
} salldSrtpInst_t;

/*******************************************************************************
 * DATA DEFINITION:  SALLD Software IV Formating functions
 *******************************************************************************
 * DESCRIPTION:  Defines the function prototypes of the SRTP/SRTCP IV formating
 *               routines .
 ******************************************************************************/
typedef int16_t (*srtpFormIV)(void *pKeyInfo, tword *pkt, tword *iv, uint32_t roc);

typedef int16_t (*srtcpFormIV)(void *pKeyInfo, tword *pkt, tword *iv, uint32_t index);

/******************************************************************************
 * SRTP Module global functions 
 *****************************************************************************/
/* salldsrtpinit.c */ 
int16_t salld_srtp_init (void *salldInst, void *cfg);
int16_t salld_srtp_close (void *salldInst);

/* salldsrtp.c */
int16_t salld_srtp_control (void *salldInst, void *ctrl);
int16_t salld_srtp_get_stats (void *salldInst, uint16_t flags, void *stats);
int16_t salld_srtp_send_data (void *salldInst, void *pktInfo, uint16_t clear);
int16_t salld_srtp_receive_data (void *salldInst, void *pktInfo);
uint16_t salld_srtp_derive_keys (salldSrtpKeyInfo_t* pKeyInfo, uint16_t seq, uint32_t roc);
int16_t srtp_key_validation(void *salldInst, salldSrtpKeyInfo_t* pKeyInfo, uint16_t seq, uint32_t roc, 
                          Sa_KeyRequest_t* pKeyReq, uint32_t* numPktLsw, uint16_t* numPktMsw);
void srtp_update_key(salldSrtpKeyInfo_t* pKeyInfo);

/* salldstrpsc.c */
int16_t salld_srtp_set_tx_sc(salldSrtpInst_t *inst, uint16_t rekey);
int16_t salld_srtp_set_rx_sc(salldSrtpInst_t *inst, uint16_t rekey);
int16_t salld_srtp_tx_rekey_sm(salldSrtpInst_t*  inst);
int16_t salld_srtp_rx_rekey_sm(salldSrtpInst_t* inst, uint32_t seqNum);

/* salldsrtp_ctr.c */
int16_t salld_srtp_form_ctr_iv(void *keyInfo, tword *pkt, tword *iv, uint32_t roc);

/* salldsrtp_f8.c */
int16_t salld_srtp_form_f8_iv(void *keyInfo, tword *pkt, tword *iv, uint32_t roc);


#endif /* _SALLDSRTPLOC_H */
