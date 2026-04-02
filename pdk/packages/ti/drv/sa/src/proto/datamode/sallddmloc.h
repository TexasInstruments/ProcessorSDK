#ifndef _SALLDDMLOC_H
#define _SALLDDMLOC_H
/******************************************************************************
 * FILE PURPOSE: Data Mode Local Header File
 ******************************************************************************
 * FILE NAME:salldacloc.h
 *
 * DESCRIPTION: Data Mode Local defines and header files
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
#include "sallddm.h"

#if defined(NSS_LITE) || defined (NSS_LITE2)
#undef SALLD_DATA_MODE_USE_PHP
#else
#define SALLD_DATA_MODE_USE_PHP
#endif


#define SALLD_DM_MAX_ENC_KEY_SIZE                 (32)   /* 256 bits */
#define SALLD_DM_MAX_MAC_KEY_SIZE                 (64)   /* 512 bits */

#define SALLD_DM_MAX_IV_SIZE                      (16)   /* 128 bits */
#define SALLD_DM_MAX_SALT_SIZE                    ( 4)   /*  32 bits */

#define SALLD_DM_MAX_AAD_SIZE                     (12)   
#define SALLD_DM_MAX_AUX_SIZE                     (32)   

#define SALLD_DM_MAX_ENC_KEY_SIZE_IN_TUINT        SALLD_BYTE_TO_TUINT(SALLD_DM_MAX_ENC_KEY_SIZE) 
#define SALLD_DM_MAX_MAC_KEY_SIZE_IN_TUINT        SALLD_BYTE_TO_TUINT(SALLD_DM_MAX_MAC_KEY_SIZE) 
#define SALLD_DM_MAX_IV_SIZE_IN_TUINT             SALLD_BYTE_TO_TUINT(SALLD_DM_MAX_IV_SIZE)
#define SALLD_DM_MAX_SALT_SIZE_IN_TUINT           SALLD_BYTE_TO_TUINT(SALLD_DM_MAX_SALT_SIZE) 
#define SALLD_DM_MAX_AAD_SIZE_IN_TUINT            SALLD_BYTE_TO_TUINT(SALLD_DM_MAX_AAD_SIZE)
#define SALLD_DM_MAX_AUX_SIZE_IN_TUINT            SALLD_BYTE_TO_TUINT(SALLD_DM_MAX_AUX_SIZE)

#ifdef SALLD_DATA_MODE_USE_PHP
    #define SALLD_CMDL_FINAL_ENGINE_ID  SALLD_CMDL_ENGINE_SRTP_AC_HPS2
#else
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    #define SALLD_CMDL_FINAL_ENGINE_ID  SALLD_CMDL_ENGINE_ID_OUTPORT2
#else
    #ifdef NSS_LITE2
    #define SALLD_CMDL_FINAL_ENGINE_ID  SALLD_CMDL_ENGINE_ID_OUTPORT2
    #else
    #define SALLD_CMDL_FINAL_ENGINE_ID  SALLD_CMDL_ENGINE_ID_OUTPORT1
    #endif
#endif    
#endif    

/******************************************************************************
 * DATA DEFINITION:  SALLD Data Mode Common Structure
 ******************************************************************************
 * DESCRIPTION: Define the Data Mode Common parameters for both Tx and Rx
 *
 *****************************************************************************/
typedef struct salldDataModeComInfo_s{
  uint16_t    ctrlBitfield;        
  /* Bit 0-7: Command label mode as defined at salldcmdl.h
   * Bit 8: CMAC enabled
   * Bit 9-15: Reserved
   */   
  #define SALLD_DM_SET_CMDL_MODE(a,b)       UTL_SET_BITFIELD((a)->ctrlBitfield, (b), 0, 8)
  #define SALLD_DM_GET_CMDL_MODE(a)         UTL_GET_BITFIELD((a)->ctrlBitfield, 0, 8)
  #define SALLD_DM_SET_CMAC(a, b)           UTL_SET_BITFIELD((a)->ctrlBitfield, (b), 8, 1)
  #define SALLD_DM_TEST_CMAC(a)             UTL_GET_BITFIELD((a)->ctrlBitfield, 8, 1)
  uint8_t     encEngId;       /* Record the encryption Engine ID */          
  uint8_t     authEngId;      /* Record the authentication Engine ID */
  Sa_DataModeConfigParams_t  config;
  uint16_t    sessionEncKey[SALLD_DM_MAX_ENC_KEY_SIZE_IN_TUINT]; /* Enc Key */
  uint16_t    sessionMacKey[SALLD_DM_MAX_MAC_KEY_SIZE_IN_TUINT]; /* Mac Key */
  uint16_t    sessionSalt[SALLD_DM_MAX_SALT_SIZE_IN_TUINT];      /* Salt */
  uint16_t    aux[SALLD_DM_MAX_AUX_SIZE_IN_TUINT];               /* store Auxiliary data such as
                                                                    k1/k2 in CMAC mode */
  Sa_CmdLbUpdateInfo_t  cmdlUpdateInfo;
                                                                    
} salldDataModeComInfo_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD Data Mode Command Label Structure
 ******************************************************************************
 * DESCRIPTION: Define the Data Mode Command label parameters
 *
 *****************************************************************************/
typedef struct salldDataModeCmdlInfo_s{
  uint16_t  size;   /**< Specify the command label size in bytes */
  uint32_t  buf[sa_MAX_CMDLB_SIZE/4];  /**< Store the command label */
} salldDataModeCmdlInfo_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD Data Mode TX Instance Structure
 ******************************************************************************
 * DESCRIPTION: for Tx side
 *  
 *****************************************************************************/
typedef struct salldDataModeTxInst_s{
  uint16_t   cipherMode;
  uint16_t   authMode;
  uint32_t   packetLsw;         /* Number of processed Packets */
  uint32_t   packetMsw;         /* Number of processed Packets */
  Sa_ScReqInfo_t scInfo;    /* Security Context Information */
  Sa_SWInfo_t    swInfo;    /* SA-specific SW Information */
  Sa_DestInfo_t  destInfo;  /* Destination related information */
  salldDataModeComInfo_t comInfo;   /* Common parameters */
  salldDataModeCmdlInfo_t cmdl;     /* Store constructed Command labels */
} salldDataModeTxInst_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD Data Mode Instance Structure
 ******************************************************************************
 * DESCRIPTION: The first element is always the msuInst_t derived data type
 *  
 *****************************************************************************/
typedef struct salldDataModeInst_s{
  salldInst_t            salldInst;     /* The SALLD Instance */
  salldDataModeTxInst_t  txInst;        /* Data Mode TX Instance */
} salldDataModeInst_t;

/* sallddminit.c */
int16_t salld_data_mode_init (void *salldInst, void *cfg);
int16_t salld_data_mode_close (void *salldInst);

/* sallddm.c */
int16_t salld_data_mode_control (void *salldInst, void *ctrl);
int16_t salld_data_mode_get_stats (void *salldInst, uint16_t flags, void *stats);
int16_t salld_data_mode_send_data (void *salldInst, void *pktInfo, uint16_t clear);
int16_t salld_data_mode_receive_data (void *salldInst, void *pktInfo);

/* sallddmcmdl.c */
int16_t salld_data_mode_set_sc(salldDataModeInst_t *inst); 
int16_t salld_data_mode_set_cmdl(salldDataModeInst_t *inst);
int16_t salld_data_mode_update_cmdl(salldDataModeInst_t *inst, Sa_PktInfo_t* pktInfo); 

#endif /* _SALLDACLOC_H */
