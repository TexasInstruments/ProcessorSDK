#ifndef _SALLDCFG_H
#define _SALLDCFG_H
/******************************************************************************
 * FILE PURPOSE: Internal definitions and prototypes for SALLD configuration.
 ******************************************************************************
 * FILE NAME:   salldcfg.h
 *
 * DESCRIPTION: Contains internal definitions and function prototypes for 
 *              SALLD test configuration
 *
 * (C) Copyright 2009-2014 Texas Instruments, Inc.
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
#include "salldsim.h"

#define SALLD_UT_CFG_INFO_INDEX_TX     0
#define SALLD_UT_CFG_INFO_INDEX_RX     1

/* Salld Common configuration */
typedef struct {
    Sa_SecProto_e       protocolType;
    Sa_CipherMode_e     cipherMode;    /* Specify the cipher mode as defined at salldCipherModes_e */
    Sa_AuthMode_e       authMode;      /* Specify the authentication mode as defined at salldAuthModes_e */
    uint16_t            replayWinSize;
    Sa_DestInfo_t       destInfo[2];   /* Specify the post-SA destination information */
} salldSimCommConfig_t;

typedef struct {
    uint32_t              spi;
    uint16_t              encKeySize;  /* (16/24/32) with some restrictions per RFCs */
    uint16_t              macKeySize;  /* (16/20/32) with some restrictions per RFCs */
    uint16_t              macSize;     /* (8/12/16) with some restrictions per RFCs */
    Bool                  esn;       /* TRUE: ESN on
                                      * FALSE: ESN off
                                      * Should be set to TRUE for 50% test cases
                                      */
} salldSimIpsecConfig_t;

typedef struct {
    uint16_t              macSize;     /* (8/12/16) with some restrictions per RFCs */
    uint16_t              derivRate;   /**< Specify the key derivation rate in n of 2^n format */
    uint16_t              mkiSize;     /* (0/2/4) */
    Bool                  fromTo;    /* TRUE: From-To Key
                                      * FALSE: MKI Key
                                      * Should be set to TRUE for 50% test cases
                                      */
    uint32_t              keyLifeTime; /**< Specify the maximum number of allowed for the master */
    uint32_t              fromEsn;     /**< Specify the starting 48-bit extended sequence number */
    uint32_t              toEsn;       /**< Specify the lastg 48-bit extended sequence number
                                          of the specified From-To keys */
    uint32_t              index;       /**< SRTP: roc; SRTCP: initial sequence number */                                      
    
} salldSimSrtpConfig_t;

typedef struct {
    Sa_AcPduType_e      pduType;
    /*
     * The Direction is actually fixed since the SA LLD is running at RNC
     * To Air Traffic (From Network): Dir: 1 (RNC to UE)
     * From Air Traffic (To Network): Dir: 0 (UE to RNC)
     *
     * Note: The configuration parameter upLink defined below is introduced so that
     *       we can verify the To-Air/From-Air traffic as a pair
     */
    Bool                upLink;      /* Dir 0:UE to RNC(uplink)
                                            1:RNC to UE(downlink) */
    /* Key to be kept in internal memory*/
    Bool                intKey;      /* 1: Key in internal memory 
                                        0: Key in external memory (DDR) */
                                        
    Bool                countCPresent;  /* CountC is maintained by application in stead of SASS */
                                        /* note: It is applicable to LTE to-air traffic only since
                                                 it is the only scenario that count-C is maintained
                                                 at SASS */                                        
} salldSimAcConfig_t;


typedef struct {
    uint16_t              ctrlBitMap;  /* Data mode control bit map */
    uint16_t              encKeySize;  /* (16/24/32) */
    uint16_t              macKeySize;  /* (16/20/24/32) */
    uint16_t              macSize;     /* (8/12/16) */
    uint16_t              aadSize;     /* (8/12) */  
    Bool                  enc;       /* TRUE:encryption; FALSE: decryption */
    Bool                  enc1st;    /* TRUE:encryption first; FALSE: authentication first */
    uint16_t              ivSize;      /* iv size in bytes */
    uint16_t              saltSize;    /* salt size in bytes */
#if defined(NSS_LITE2)
    uint8_t               secure;     /* Secure bit for the contexdt */
    uint8_t               priv;        /* Priv value for the Ctx */
    uint8_t               privId;     /* PrivID for the ctx */
#endif
} salldSimDataModeConfig_t;

salldSimChannel_t* salldcfg_chan_init (salldSimCommConfig_t* pCommCfg, void* pProtoCfg);

#endif  /* _SALLDCFG_H */
/* nothing past this point */
