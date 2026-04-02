#ifndef _SALLDACLOC_H
#define _SALLDACLOC_H
/******************************************************************************
 * FILE PURPOSE: AC Local Header File
 ******************************************************************************
 * FILE NAME:salldacloc.h
 *
 * DESCRIPTION: AC Local defines and header files
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
#include "salldac.h"

#define SALLD_AC_MIN_INPUT_KEY_SIZE               (8)    /*  64 bits */
#define SALLD_AC_MIN_ENC_KEY_SIZE                 (16)   /* 128 bits */
#define SALLD_AC_MAX_ENC_KEY_SIZE                 (32)   /* 256 bits */
#define SALLD_AC_MAX_ENC_KEY_SIZE                 (32)   /* 256 bits */
#define SALLD_AC_MAX_MAC_KEY_SIZE                 (64)   /* 512 bits */

#define SALLD_AC_MAX_IV_SIZE                      (16)   /* 128 bits */
#define SALLD_AC_MAX_MAC_TAG_SIZE                 ( 4)   /*  64 bits */

#define SALLD_AC_MAX_ENC_KEY_SIZE_IN_TUINT        SALLD_BYTE_TO_TUINT(SALLD_AC_MAX_ENC_KEY_SIZE) 
#define SALLD_AC_MAX_MAC_KEY_SIZE_IN_TUINT        SALLD_BYTE_TO_TUINT(SALLD_AC_MAX_MAC_KEY_SIZE) 
#define SALLD_AC_MAX_IV_SIZE_IN_TUINT             SALLD_BYTE_TO_TUINT(SALLD_AC_MAX_IV_SIZE)
#define SALLD_AC_MAX_MAC_TAG_SIZE_IN_TUINT        SALLD_BYTE_TO_TUINT(SALLD_AC_MAX_MAC_TAG_SIZE) 

#define SALLD_AC_MAX_KEYS_IN_SCRATCH 			  1024
#define SALLD_AC_MAX_32BIT_BLOCKS_IN_BITMAP		  (SALLD_AC_MAX_KEYS_IN_SCRATCH/32)
#define SALLD_AC_MAX_KEYS_IN_SCRATCH2 			  2048
#define SALLD_AC_MAX_32BIT_BLOCKS_IN_BITMAP2	  (SALLD_AC_MAX_KEYS_IN_SCRATCH2/32)

#define SALLD_AC_BYTE_TO_UINT32_OFFSET(x) 		  ((x) >> 2)
#define SALLD_AC_SCRATCH_MEMORY_BLOCK_SIZE 	      0x4000
#define SALLD_AC_SCRATCH_MEMORY_BLOCK_MASK 	      0x3FFF
#define SALLD_AC_SCRATCH_MEMORY_BLOCK_NUM(x)      ((x) >> 14) 	      


#define SALLD_AC_DEFAULT_KEY_SIZE_IN_WORD 		  4

/******************************************************************************
 * DATA DEFINITION:  SALLD AC Common Structure
 ******************************************************************************
 * DESCRIPTION: Define the AC Common parameters for both Tx and Rx
 *  
 *****************************************************************************/
typedef struct salldAcComInfo_s{
  Sa_AcConfigParams_t  config;
  uint16_t	 sessionEncKeyScratchOffset;
  uint16_t	 sessionMacKeyScratchOffset;
  uint16_t   sessionEncKey[SALLD_AC_MAX_ENC_KEY_SIZE_IN_TUINT]; /* Enc Key */
  uint16_t   sessionMacKey[SALLD_AC_MAX_MAC_KEY_SIZE_IN_TUINT]; /* Mac Key */
} salldAcComInfo_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD AC TX Instance Structure
 ******************************************************************************
 * DESCRIPTION: for Tx side
 *  
 *****************************************************************************/
typedef struct salldAcTxInst_s{
  uint16_t   cipherMode;
  uint16_t   authMode;
  uint32_t   packetDecLsw;      /* Number of Packets decrypted with this master key */
  uint32_t   packetDecMsw;      /* Number of Packets decrypted with this master key */
  uint32_t   countC;           /* 32-bit CountC */
  Sa_ScReqInfo_t scInfo;    /* Security Context Information */
  Sa_SWInfo_t    swInfo;    /* SA-specific SW Information */
  Sa_DestInfo_t  destInfo;  /* Destination related information */
  salldAcComInfo_t comInfo;   /* Common parameters */
} salldAcTxInst_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD AC RX Instance Structure
 ******************************************************************************
 * DESCRIPTION: for Rx side
 *  
 *****************************************************************************/
typedef struct salldAcRxInst_s{
  uint16_t   cipherMode;
  uint16_t   authMode;
  uint32_t   packetEncLsw;     /* Number of Packets encrypted with this master key */
  uint32_t   packetEncMsw;     /* Number of Packets encrypted with this master key */
  uint32_t   countC;           /* 32-bit CountC */
  Sa_ScReqInfo_t scInfo;   /* Security Context Information */
  Sa_SWInfo_t    swInfo;   /* SA-specific SW Information */
  Sa_DestInfo_t  destInfo; /* Destination related information */
  salldAcComInfo_t comInfo;  /* Common parameters */
} salldAcRxInst_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD AC Instance Structure
 ******************************************************************************
 * DESCRIPTION: The first element is always the msuInst_t derived data type
 *  
 *****************************************************************************/
typedef struct salldAcInst_s{
  salldInst_t      salldInst;  /* The SALLD Instance */
  salldAcTxInst_t  txInst;     /* AC TX Instance */
  salldAcRxInst_t  rxInst;     /* AC RX Instance */
} salldAcInst_t;

/******************************************************************************
 * AC Internal global functions 
 *****************************************************************************/
 
/* salldacinit.c */ 
int16_t salld_ac_init (void *salldInst, void *cfg);
int16_t salld_ac_close (void *salldInst); 

/* salldac.c */
int16_t salld_ac_control (void *salldInst, void *ctrl);
int16_t salld_ac_get_stats (void *salldInst, uint16_t flags, void *stats);
int16_t salld_ac_send_data (void *salldInst, void *pktInfo, uint16_t clear);
int16_t salld_ac_receive_data (void *salldInst, void *pktInfo);
int16_t salld_ac_set_tx_sc(salldAcInst_t *inst); 
int16_t salld_ac_set_rx_sc(salldAcInst_t *inst); 

#endif /* _SALLDACLOC_H */
