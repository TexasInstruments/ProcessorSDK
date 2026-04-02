/******************************************************************************
 * FILE PURPOSE: AC Main File
 ******************************************************************************
 * FILE NAME: salldac.c
 *
 * DESCRIPTION: The main module for AC Code
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

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
/* RTSC header files */ 

/* Standard include files */
#include <string.h>

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include <ti/drv/sa/sa_osal.h>
#include <ti/csl/cslr_cp_ace.h>
#include "src/salldloc.h"
#include "src/salldport.h"
#include "salldac.h"
#include "salldacloc.h"

#include "src/auth/salldcmac.h"


/* Defined for testing. Should be moved to makedefs.mk */

#define acGetChID(mid_chnum)  ( (((uint16_t)(mid_chnum)) & 0x00FF )-1)

/******************************************************************************
 * DATA DEFINITION:  SALLD AC Header Configuration Structure
 ******************************************************************************
 * DESCRIPTION: Define the AC header configuration parameters 
 *  
 *****************************************************************************/
typedef struct
{
    uint16_t ctrlBitfield;        /* various Air Cipher control information */   
    uint8_t  hdrSize;             /* PDU header Size in bytes*/
    uint8_t  authHdrSize;         /* The size of PDU portion which should be authenticated */
    uint8_t  seqNumSize;          /* Sequemce number size in bits */
    uint8_t  seqNumShift;         /* Number of bits to the sequence number in the PDU header */    
} acHdrConfigParams_t;

/****************************************************************************
 * Table PURPOSE:   AC SC Header Configuration Tables 
 ****************************************************************************
 * DESCRIPTION:     The tables defines the SC PDU header related configuration 
 *                  parameter per PDU type 
 *
 ***************************************************************************/
 /* Tx (from Air) */
 static acHdrConfigParams_t  acHdrParamsTx[] =
 {
    /* sa_AcPduType_GSM */
    {
        SA_CTX_PROTO_AC_FLAG_IV_PRESENT,
        0,
        0,
        0,
        0        
    },
    
    /* sa_AcPduType_WCDMA_TMD */
    {
        SA_CTX_PROTO_AC_FLAG_COUNTC_PRESENT,
        0,
        0,
        0,
        0        
    },

    /* sa_AcPduType_WCDMA_UMD */
    {
        SA_CTX_PROTO_AC_FLAG_HDR_PRESENT,
        1,
        0,
        7,
        1        
    },

    /* sa_AcPduType_WCDMA_AMD */
    {
        SA_CTX_PROTO_AC_FLAG_HDR_PRESENT,
        2,
        0,
        12,
        3        
    },

    /* sa_AcPduType_LTE */
    {
        SA_CTX_PROTO_AC_FLAG_COUNTC_PRESENT,
        0,
        0,
        0,
        0        
    },
    
    /* sa_AcPduType_LTE_CP */
    {
        SA_CTX_PROTO_AC_FLAG_COUNTC_PRESENT,
        0,
        1,
        0,
        0        
    }
 };
 
 /* Rx (To Air) */
 static acHdrConfigParams_t  acHdrParamsRx[] =
 {
    /* sa_AcPduType_GSM */
    {
        SA_CTX_PROTO_AC_FLAG_IV_PRESENT,
        0,
        0,
        0,
        0        
    },
    
    /* sa_AcPduType_WCDMA_TMD */
    {
        SA_CTX_PROTO_AC_FLAG_COUNTC_INSERT,
        0,
        0,
        0,
        0        
    },

    /* sa_AcPduType_WCDMA_UMD */
    {
        SA_CTX_PROTO_AC_FLAG_HDR_PRESENT,
        1,
        0,
        7,
        1        
    },

    /* sa_AcPduType_WCDMA_AMD */
    {
        SA_CTX_PROTO_AC_FLAG_HDR_PRESENT,
        2,
        0,
        12,
        3        
    },

    /* sa_AcPduType_LTE */
    {
        SA_CTX_PROTO_AC_FLAG_COUNTC_INSERT,
        0,
        0,
        0,
        0        
    },
    
    /* sa_AcPduType_LTE_CP */
    {
        SA_CTX_PROTO_AC_FLAG_COUNTC_PRESENT,
        0,
        1,
        0,
        0        
    }
    
 };

/****************************************************************************
 * FUNCTION PURPOSE: AC Key Setup 
 ****************************************************************************
 * DESCRIPTION: Update the AC channel key information based on the input
 *          parameters.
 *
 *  uint16_t salld_ac_setup_key(
 *            salldAcInst_t*      inst        -> Point to AC channel instance
 *            salldAcComInfo_t*   pComInfo    -> pointer to the instance key storage 
 *            Sa_AcKeyParams_t* pKeyParams) -> pointer to the key configuration
 *                                               parameters.   
 *                       
 * Return values:  FALSE : invalid parameters
 *                 TRUE:   successful updtae         
 *
 ***************************************************************************/
static uint16_t salld_ac_setup_key(salldAcInst_t *inst, 
                                   salldAcComInfo_t* pComInfo, Sa_AcKeyParams_t* pKeyParams) 
{
  uint16_t ctrlBitMap = pKeyParams->ctrlBitfield;
    
  if(ctrlBitMap & sa_AC_KEY_CTRL_ENC_KEY)
  {
      if(pComInfo->config.sessionEncKeySize < SALLD_AC_MIN_INPUT_KEY_SIZE)
        return(FALSE);
  
      /* Copy and expand the encryption key */
      memcpy(pComInfo->sessionEncKey, pKeyParams->sessionEncKey, pComInfo->config.sessionEncKeySize);
                   
      if (pComInfo->config.sessionEncKeySize < SALLD_AC_MIN_ENC_KEY_SIZE)
      {
        memcpy((void *)(&pComInfo->sessionEncKey[0] + pComInfo->config.sessionEncKeySize), pKeyParams->sessionEncKey, 
               SALLD_AC_MIN_ENC_KEY_SIZE - pComInfo->config.sessionEncKeySize);
        pComInfo->config.sessionEncKeySize = SALLD_AC_MIN_ENC_KEY_SIZE;       
      
      }              
  }
  
  if( ctrlBitMap & sa_AC_KEY_CTRL_MAC_KEY)
  {
      /* Copy authentication key */
      memcpy(pComInfo->sessionMacKey, pKeyParams->sessionAuthKey, pComInfo->config.sessionMacKeySize);
  }
  return TRUE;
}            

#define SALLD_AC_KEY_ALLOCATION_ERROR 0xFFFF

/****************************************************************************
 * FUNCTION PURPOSE: AC Scratch Memory Key Storage Bitmap Allocate 
 ****************************************************************************
 * DESCRIPTION: Performs linear block search on the allocation bitmap for a free block.
 *				Will allocate block bitmap if free entry is found. 
 *
 *  uint16_t salld_ac_key_allocation_bitmap_allocate(
 *            int       numBlocks,               -> number of 32-bit words in bitmap
 *            uint32_t * scratchAllocBitMap)      -> Allocation bitmap in SALLD inst
 *                       
 * Return values:  	free block byte offset if a free block is found
 *					~0 when available free blocks are found    
 *
 ***************************************************************************/
static uint16_t salld_ac_key_allocation_bitmap_allocate(int numBlocks, uint32_t * scratchAllocBitMap)
{
  int      i = 0, j = 0;
  uint16_t freeBlockOffsetInBytes = 0;
  uint32_t allocBitMapTmp;
  
  /* For each bit field word */
  for(i = 0; i < numBlocks; i++)
  {
	allocBitMapTmp = *(scratchAllocBitMap+i);
	/* Check if block has any free entries */
	if(!~allocBitMapTmp)
		continue;
	/* For each bit in the word */
	for(j = 0; j < 32; j ++)
	{
		if(!(allocBitMapTmp>>j & 0x1))
		{
			freeBlockOffsetInBytes = (i*32 + j)*SALLD_AC_MIN_ENC_KEY_SIZE;
			uint32_t bitMask = 0x1 << j;
			*((uint32_t*)(scratchAllocBitMap+i)) |= bitMask;
			return freeBlockOffsetInBytes;
		}
	}
  }
  return SALLD_AC_KEY_ALLOCATION_ERROR;
}

/****************************************************************************
 * FUNCTION PURPOSE: AC Scratch Memory Key Storage Bitmap Free 
 ****************************************************************************
 * DESCRIPTION: Mark a block as freed in the allocation bitmap
 *
 *  uint16_t salld_ac_key_allocation_bitmap_free(
 *            uint32_t * scratchAllocBitMap      -> Allocation bitmap in SALLD inst
 *			  uint16_t 	 blockOffsetInBytes)	 -> Block to be freed, offset in byte
 *                       
 * Return values:  	TRUE when successful    
 *
 ***************************************************************************/
uint16_t salld_ac_key_allocation_bitmap_free(uint32_t * scratchAllocBitMap, uint16_t blockOffsetInBytes)
{
  uint32_t i = blockOffsetInBytes / SALLD_AC_MIN_ENC_KEY_SIZE / 32;
  uint32_t j = (blockOffsetInBytes / SALLD_AC_MIN_ENC_KEY_SIZE) % 32;
  uint32_t bitMask = ~(0x1 << j);
  *((uint32_t*)(scratchAllocBitMap+i)) &= bitMask;
  return TRUE;
}

/****************************************************************************
 * FUNCTION PURPOSE: Verify Air Ciphering Configuration Parameters 
 ****************************************************************************
 * DESCRIPTION: Verify Air Ciphering general configuration parameters for consistency
 *              with the encryption and authentication mode
 *
 *  uint16_t salld_ipsec_verify_config_params(
 *            Sa_CipherMode_e cipherMode,           -> Ciphering mode
 *            Sa_AuthMode_e   authMode,             -> Aurthentication Mode
 *            Sa_AcConfigParams_t*  pAcConfig,      ->pointer to the Air Ciphering configuration
 *                                                    parameters.   
 *            int        fSassGen2) -> Flag indicates whether this is a 2nd generation SASS
 *                       
 * Return values:  FALSE : invalid parameters
 *                 TRUE:   valid parameters          
 *
 ***************************************************************************/
static uint16_t salld_ac_verify_config_params(Sa_CipherMode_e cipherMode, Sa_AuthMode_e authMode,
                                              Sa_AcConfigParams_t*  pAcConfig, int fSassGen2)
{
    
    /* Common Check */
    if (!fSassGen2)
    {
        if ((cipherMode != sa_CipherMode_NULL) && (authMode != sa_AuthMode_NULL))
        {
            /* SASS_GEN1: simulatneous ciphering and authentication is not supported */
            return FALSE;
        } 
    
        if((cipherMode == sa_CipherMode_ZUC_F8) ||
           (authMode == sa_AuthMode_SNOW3G_F9)  ||
           (authMode == sa_AuthMode_ZUC_F9))
        {
            /* Those ciphering mode and authentication modes are supported at SASS GEN2 device only */
            return FALSE;
        } 
    }  
    
    /* Ciphering mode specific check */
    switch (cipherMode)
    {
        case sa_CipherMode_AES_CTR:
        case sa_CipherMode_SNOW3G_F8:
        case sa_CipherMode_ZUC_F8:
            if((pAcConfig->sessionEncKeySize != 16)   ||
               (pAcConfig->ivSize != 16))          
               return (FALSE);
			if (cipherMode != sa_CipherMode_AES_CTR)
			{
  			  /* The first byte in the table would be non zero if 3gpp is enabled */
			  if (sa_eng_snow3g_enc_mci_tbl[0][0] == 0)
			    return (FALSE);
			}
            break;   
               
        case sa_CipherMode_KASUMI_F8:
        case sa_CipherMode_GSM_A53:
        case sa_CipherMode_GEA3:
        case sa_CipherMode_ECSD_A53:
            if((pAcConfig->sessionEncKeySize != 16)   ||
               (pAcConfig->ivSize != 8))          
               return (FALSE);
			/* The first byte in the table would be non zero if 3gpp is enabled */			
 		    if (sa_eng_kasumi_enc_mci_tbl[0][0] == 0)
               return (FALSE);
            break;   
            
        case sa_CipherMode_NULL:
            break;     
        
        default:
            return (FALSE);
    }
    
    /* Encryption mode specific check */
    switch (authMode)
    {
    
        case sa_AuthMode_CMAC:
            if((pAcConfig->sessionMacKeySize != 16)   ||
               (pAcConfig->macSize != 4))          
               return (FALSE);
            break;   
    
        case sa_AuthMode_KASUMI_F9:
            if((pAcConfig->sessionMacKeySize != 16)   ||
               (pAcConfig->ivSize != 8)               ||
               (pAcConfig->macSize != 4))          
               return (FALSE);
			/* The first byte in the table would be non zero if 3gpp is enabled */			
 		    if (sa_eng_kasumi_auth_mci_tbl[0][0] == 0)
			   	return (FALSE);				
            break;   
            
        case sa_AuthMode_SNOW3G_F9:
        case sa_AuthMode_ZUC_F9:
            if((pAcConfig->sessionMacKeySize != 16)   ||
               (pAcConfig->ivSize != 16)              ||
               (pAcConfig->macSize != 4))          
               return (FALSE);
  			/* The first byte in the table would be non zero if 3gpp is enabled */
			if (sa_eng_snow3g_auth_mci_tbl2[0][0] == 0)
			  return (FALSE);
               
            break;   
            
        case sa_AuthMode_NULL:
            break;
            
        default:
            return(FALSE);
    }
    

  return TRUE;
}            


/****************************************************************************
 * FUNCTION PURPOSE: AC Control 
 ****************************************************************************
 * DESCRIPTION: AC Control functions
 *
 * int16_t  salld_ac_control (
 *   void  *salldInst  - a pointer to SALLD channel instance
 *   void  *ctrl)      - a pointer to control structure
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *
 ***************************************************************************/
int16_t salld_ac_control (void *salldInst, void *salldCtrl)
{
  salldAcInst_t *inst = (salldAcInst_t *)salldInst;
  salldObj_t *sysInst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->salldInst.ownerInstOffset);
  salldAcTxInst_t *txInst = &inst->txInst;
  salldAcRxInst_t *rxInst = &inst->rxInst;
  Sa_ChanCtrlInfo_t *ctrl = (Sa_ChanCtrlInfo_t *)salldCtrl;
  uint16_t bitmap;
  int16_t ret;

  switch(ctrl->ctrlType)
  {
    /* SALLD cipher mac and key size selection */
    case sa_CHAN_CTRL_GEN_CONFIG:
    {
        bitmap = ctrl->ctrlInfo.gen.validBitfield;
        if( bitmap & sa_CONTROLINFO_VALID_TX_CTRL)
        {
            /* Input parameters check */
            Sa_GenConfigParams_t* pGenConfig = &ctrl->ctrlInfo.gen.txCtrl;
            Sa_AcConfigParams_t*  pAcConfig  = &ctrl->ctrlInfo.gen.txCtrl.params.ac;
            
            if (salld_ac_verify_config_params(pGenConfig->cipherMode, pGenConfig->authMode, pAcConfig, SALLD_TEST_SASS_GEN2(sysInst)))
            {
                txInst->cipherMode = pGenConfig->cipherMode;
                txInst->authMode   = pGenConfig->authMode;
                txInst->destInfo   = pGenConfig->destInfo; 
                txInst->comInfo.config = *pAcConfig;
            }
            else
            {
                return(sa_ERR_PARAMS);
            }
            
                
            /* 
             * Snow3G/ZUC signle-pass configuration 
             */
            if ((txInst->cipherMode == sa_CipherMode_SNOW3G_F8) && (txInst->authMode == sa_AuthMode_SNOW3G_F9))
            {
                txInst->cipherMode = sa_CipherMode_SNOW3G_F8F9;
                txInst->authMode = sa_AuthMode_NULL;    
            }
            else if ((txInst->cipherMode == sa_CipherMode_ZUC_F8) && (txInst->authMode == sa_AuthMode_ZUC_F9))
            {
                txInst->cipherMode = sa_CipherMode_ZUC_F8F9;
                txInst->authMode = sa_AuthMode_NULL;    
            }
                
        }
        if(bitmap & sa_CONTROLINFO_VALID_RX_CTRL)
        {
            /* Input parameters check */
            Sa_GenConfigParams_t*  pGenConfig = &ctrl->ctrlInfo.gen.rxCtrl;
            Sa_AcConfigParams_t*   pAcConfig  = &ctrl->ctrlInfo.gen.rxCtrl.params.ac;
            
            if (salld_ac_verify_config_params(pGenConfig->cipherMode, pGenConfig->authMode, pAcConfig, SALLD_TEST_SASS_GEN2(sysInst)))
            {
                rxInst->cipherMode = pGenConfig->cipherMode;
                rxInst->authMode   = pGenConfig->authMode;
                rxInst->destInfo   = pGenConfig->destInfo;
                rxInst->comInfo.config = *pAcConfig;
            }
            else
            {
                return(sa_ERR_PARAMS);
            }
            
                
            /* 
             * Snow3G/ZUC signle-pass configuration 
             */
            if ((rxInst->cipherMode == sa_CipherMode_SNOW3G_F8) && (rxInst->authMode == sa_AuthMode_SNOW3G_F9))
            {
                rxInst->cipherMode = sa_CipherMode_SNOW3G_F8F9;
                rxInst->authMode = sa_AuthMode_NULL;    
            }
            else if ((rxInst->cipherMode == sa_CipherMode_ZUC_F8) && (rxInst->authMode == sa_AuthMode_ZUC_F9))
            {
                rxInst->cipherMode = sa_CipherMode_ZUC_F8F9;
                rxInst->authMode = sa_AuthMode_NULL;    
            }
        }
        
        
        /*
         * Is it time to form and register security context?
         */
        if(!SALLD_TEST_STATE_TX_SC_VALID(&inst->salldInst) && 
            SALLD_TEST_STATE_TX_ON(&inst->salldInst))
        {
            if ((ret = salld_ac_set_tx_sc(inst)) != sa_ERR_OK)
            {
                return(ret);
            }
            
            SALLD_SET_STATE_TX_SC_VALID(&inst->salldInst, 1);
        } 
        
        if(!SALLD_TEST_STATE_RX_SC_VALID(&inst->salldInst) && 
            SALLD_TEST_STATE_RX_ON(&inst->salldInst))
        {
            if ((ret = salld_ac_set_rx_sc(inst)) != sa_ERR_OK)
            {
                return(ret);
            }
            
            SALLD_SET_STATE_RX_SC_VALID(&inst->salldInst, 1);
            
            /* Register the receive security context */
            salldLObj.callOutFuncs.ChanRegister((Sa_ChanHandle) sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr, salldInst), &inst->rxInst.swInfo);
        } 
    }    
    break;

    /* Master key and Salt setup for ac */
    case sa_CHAN_CTRL_KEY_CONFIG:
    {
        bitmap = ctrl->ctrlInfo.key.ctrlBitfield;

        if (bitmap & sa_KEY_CONTROL_TX_KEY_VALID)
        {
            Sa_AcKeyParams_t* pKeyParams = &ctrl->ctrlInfo.key.txKey.ac;
            salldAcComInfo_t*   pComInfo = &txInst->comInfo;
            if(!salld_ac_setup_key(inst, pComInfo, pKeyParams))
                    return(sa_ERR_PARAMS);
        }    

        if (bitmap & sa_KEY_CONTROL_RX_KEY_VALID)
        {
            Sa_AcKeyParams_t* pKeyParams = &ctrl->ctrlInfo.key.rxKey.ac;
            salldAcComInfo_t* pComInfo     = &rxInst->comInfo;
            if(!salld_ac_setup_key(inst, pComInfo, pKeyParams))
                    return(sa_ERR_PARAMS);
        }    
    }   
    break;
    
    default:
        return (sa_ERR_PARAMS);

  }
  return (sa_ERR_OK);
}

/****************************************************************************
 * FUNCTION PURPOSE: Extracts Statistics Info from the Security Contexts
 ****************************************************************************
 * DESCRIPTION: Extracts Statistics Info from the Security Contexts 
 *
 *  void salld_ac_sc_extract_stats(
 *            salldAcTxInst_t *txInst,       
 *            salldAcRxInst_t *rxInst,
 *            salldComStats_t *pErrStats,
 *            uint8_t         *txScBuf,
 *            uint8_t         *rxScBuf) 
 *                                               
 * Return values:  
 *                          
 *
 ***************************************************************************/
static void salld_ac_sc_extract_stats( 
                        salldAcTxInst_t *txInst,    
                        salldAcRxInst_t *rxInst,
                        salldComStats_t *pErrStats,
                        uint8_t         *txScBuf,
                        uint8_t         *rxScBuf) 
{
    uint8_t data[64];
    tword* ctxIn = data + SALLD_BYTE_TO_WORD(SA_CTX_PHP_COMMON_SIZE);
    
    if (txScBuf)
    {
        /* Security Context swizzling */
        salld_swiz_128(txScBuf, data, 64);
    
        txInst->packetDecMsw = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, numPktsHi)); 
        txInst->packetDecLsw = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, numPkts)); 
        txInst->countC       = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, countC)); 

        pErrStats->authFail  = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, numHashFails));
    }

    if (rxScBuf)
    {
        /* Security Context swizzling */
        salld_swiz_128(rxScBuf, data, 64);

        rxInst->packetEncMsw = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, numPktsHi)); 
        rxInst->packetEncLsw = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, numPkts)); 
        rxInst->countC       = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, countC)); 
    }
}                        

/******************************************************************************
 * FUNCTION PURPOSE: AC Get Stats
 ******************************************************************************
 * DESCRIPTION: Extract AC related statistics from the instance structure
 *
 * void salld_ac_get_stats (
 *    void   *salldInst,       - A pointer to SALLD instance
 *    uint16_t flags,            - various control flags
 *    void   *stats)           - The stat structure
 *
 * Return values:  sa_ERR_OK
 *
 *****************************************************************************/
int16_t salld_ac_get_stats (void *salldInst, uint16_t flags, void *stats)
{
    Sa_AcStats_t  *pStats = (Sa_AcStats_t *)  stats;
    salldAcInst_t   *inst   = (salldAcInst_t *)   salldInst; 
    salldAcTxInst_t *txInst = (salldAcTxInst_t *) &inst->txInst;
    salldAcRxInst_t *rxInst = (salldAcRxInst_t *) &inst->rxInst;
	/* Need to convert back to address since, the scBuf is stored internally as offset */
    uint8_t *txScBuf = (uint8_t *) sa_CONV_OFFSET_TO_ADDR( salldLObj.scPoolBaseAddr, txInst->scInfo.scBuf);
    uint8_t *rxScBuf = (uint8_t *) sa_CONV_OFFSET_TO_ADDR( salldLObj.scPoolBaseAddr, rxInst->scInfo.scBuf);	
    int16_t retCode = sa_ERR_OK;
    
    uint32_t key;
	uint32_t tx_valid, rx_valid;

	tx_valid = SALLD_TEST_STATE_TX_SC_VALID(&inst->salldInst);
	rx_valid = SALLD_TEST_STATE_RX_SC_VALID(&inst->salldInst);		
    
    /* Initialize a statistics update */
    Sa_osalMtCsEnter(&key);
    
    if(tx_valid)Sa_osalBeginScAccess((void *)txScBuf, SA_CTX_PHP_AC_TYPE1_SIZE);
    if(rx_valid)Sa_osalBeginScAccess((void *)rxScBuf, SA_CTX_PHP_AC_TYPE1_SIZE);
    
  
    if (flags & sa_STATS_QUERY_FLAG_TRIG)
    {
        if (tx_valid && salld_is_sc_updated(txScBuf))
        {
            /* ask for a new update */
            salld_sc_set_wait_update(txScBuf);
            Sa_osalEndScAccess((void *)txScBuf, SA_CTX_PHP_AC_TYPE1_SIZE);
            salld_send_null_pkt(salldInst, NULL, &txInst->swInfo, SA_SC_FLAGS_EVICT); 
            retCode = sa_ERR_STATS_UNAVAIL;
        } 
        
        if (rx_valid && salld_is_sc_updated(rxScBuf))
        {
            /* ask for a new update */
            salld_sc_set_wait_update(rxScBuf);
            Sa_osalEndScAccess((void *)rxScBuf, SA_CTX_PHP_AC_TYPE1_SIZE);
            
            salld_send_null_pkt(salldInst, NULL, &rxInst->swInfo, SA_SC_FLAGS_EVICT);
            retCode = sa_ERR_STATS_UNAVAIL;
        } 
        
        
        if (retCode == sa_ERR_OK)
        {
            salld_ac_sc_extract_stats(txInst, rxInst, &inst->salldInst.stats, txScBuf, rxScBuf);
        }
        
    }
    else
    {
        if ((flags & sa_STATS_QUERY_FLAG_NOW) ||
           ((!tx_valid || salld_is_sc_updated(txScBuf)) && (!rx_valid || salld_is_sc_updated(rxScBuf))))
        {
			if (!rx_valid)
				rxScBuf = NULL;
			if (!tx_valid)
				txScBuf = NULL;
            salld_ac_sc_extract_stats(txInst, rxInst, &inst->salldInst.stats, txScBuf, rxScBuf);
        }
        else
        {
            retCode = sa_ERR_STATS_UNAVAIL;
        }
    }
    
    Sa_osalMtCsExit(key);
    
    pStats->authFail    = inst->salldInst.stats.authFail;
    pStats->pktToAirHi  = rxInst->packetEncMsw;
    pStats->pktToAirLo  = rxInst->packetEncLsw;
    pStats->toAirCountC = rxInst->countC;
    pStats->pktFromAirHi= txInst->packetDecMsw;
    pStats->pktFromAirLo= txInst->packetDecLsw;
    pStats->fromAirCountC = txInst->countC;

    return (retCode);
}



/******************************************************************************
 * FUNCTION PURPOSE: Air Ciphering Send Data To Network
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_ac_send_data (
 *    void *salldInst,      - A pointer to SALLD AC instance
 *    void *pktInfo,        - packet pointer
 *    uint16_t clear) 
 *
 *  Perform the following actions:
 *      - Pass the software Info
 *
 *****************************************************************************/
int16_t salld_ac_send_data (void *salldInst, void *pktInfo, uint16_t clear) 
{
  salldAcInst_t *inst   = (salldAcInst_t *)salldInst; 
  salldAcTxInst_t *txInst = &inst->txInst;
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;

  /* Pass the software Info in the packet */
  pPktInfo->validBitMap |= sa_PKT_INFO_VALID_SW_INFO;
  pPktInfo->swInfo = txInst->swInfo;
  
  return(sa_ERR_OK);
      
} /* salld_ac_send_data */

/******************************************************************************
 * FUNCTION PURPOSE: Air Ciphering Receive Data From Network 
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_ac_receive_data (
 *    void *salldInst,    - A pointer to SALLD instance
 *    void *pktInfo)      - packet pointer
 * 
 * Perform the following actions:
 *      - Pass the software Info
 *
 *****************************************************************************/
int16_t salld_ac_receive_data (void *salldInst, void *pktInfo) 
{
  salldAcInst_t *inst   = (salldAcInst_t *)salldInst; 
  salldAcRxInst_t *rxInst = &inst->rxInst;
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;

  /* Pass the software Info in the packet */
  pPktInfo->validBitMap |= sa_PKT_INFO_VALID_SW_INFO;
  pPktInfo->swInfo = rxInst->swInfo;
  
  return (sa_ERR_OK);
}

/****************************************************************************
 * FUNCTION PURPOSE: Construct Air Ciphering Tx Security Context
 ****************************************************************************
 * DESCRIPTION: Construct Security Context from the configuration parameters 
 *          for Air Ciphering Tx (From-Air) operations
 *
 *  uint16_t salld_ac_set_tx_sc(
 *            salldIpsecInst_t*     inst)      -> Point to AC channel instance
 *                       
 * Return values: sa_ERR_XXX
 * 
 * Assumption: The same algorithm will be used for both encryption and 
 *             authentication                          
 *
 ***************************************************************************/
int16_t salld_ac_set_tx_sc(salldAcInst_t *inst) 
{
  salldObj_t *sysInst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->salldInst.ownerInstOffset);
  salldAcTxInst_t *txInst = &inst->txInst;
  salldAcComInfo_t *pComInfo = &txInst->comInfo;
  Sa_AcConfigParams_t  *pConfig = &pComInfo->config;
  Sa_ScReqInfo_t* pScInfo = &txInst->scInfo;
  int16_t encScSize;
  int16_t authScSize;
  int16_t encCmdlSize;
  int16_t authCmdlSize;
  int16_t cmdlSize;
  int16_t phpScSize = (txInst->authMode == sa_AuthMode_CMAC)?SA_CTX_PHP_AC_TYPE2_SIZE:SA_CTX_PHP_AC_TYPE1_SIZE;
  int16_t acAlgorithm = SA_AC_ALGORITHM_GSM_A53;
  saDMAReqInfo_t dmaReqInfo;
  int16_t dir = (pConfig->ctrlBitMap & sa_AC_CONFIG_DIR)?1:0;
  uint8_t *salld_ac_tmp_scBuf;
  uint8_t f8f9opt = 0;
  int i, j;
  
  /*
   * Calculate the security Context Size and allocate security Context
   *
   */
  salld_sc_enc_get_info(txInst->cipherMode, pConfig->ivSize, &encCmdlSize, &encScSize, NULL, NULL, SALLD_TEST_SASS_GEN2(sysInst)); 
  salld_sc_auth_get_info(txInst->authMode, NULL, &authCmdlSize, &authScSize, NULL);
  
  pScInfo->scSize = phpScSize + encScSize + authScSize;
  salldLObj.callOutFuncs.ScAlloc((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo);
  
  if(pScInfo->scBuf == (uintptr_t) NULL)
    return(sa_ERR_NO_CTX_BUF);
    
  Sa_osalBeginScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  
    
  /* Check if need to use special buffer for security context */
  if (pConfig->ctrlBitMap & sa_AC_CONFIG_KEY_IN_SCRATCH)
  {
    salld_ac_tmp_scBuf = (uint8_t*)salldLObj.intBuf;
	if (salld_ac_tmp_scBuf == NULL)
		return(sa_ERR_INV_INT_MEM);
  }
  else
  {
    salld_ac_tmp_scBuf = (uint8_t *) pScInfo->scBuf;
  }
  
  if(salld_ac_tmp_scBuf == NULL)
    return(sa_ERR_NO_CTX_BUF);
  
  
  memset(salld_ac_tmp_scBuf, 0, SALLD_BYTE_TO_WORD(pScInfo->scSize));
  
  /* Prepare PHP Security Context */
  memset(&dmaReqInfo, 0, sizeof(saDMAReqInfo_t));
  dmaReqInfo.phpFetchSize = SA_CTX_SIZE_TO_DMA_SIZE(phpScSize);
  if (encScSize == 0)
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_DMA_SIZE_0;
  }
  else
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(encScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
  }
  
  dmaReqInfo.phpEvictSize = SA_CTX_DMA_SIZE_64;
  
  salld_set_sc_phpCommom(&dmaReqInfo, &txInst->destInfo, SA_CTX_PKT_TYPE_3GPP_AIR,
                         pScInfo->scID, salld_ac_tmp_scBuf);  
                      
  /* Prepare Security Context for the encryption Engine */
  if (encScSize)
  {
        salld_set_sc_acEnc(txInst->cipherMode, pConfig->sessionEncKeySize, 
                       pComInfo->sessionEncKey,  pComInfo->sessionMacKey, FALSE,
                       salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize),
                       &acAlgorithm, SALLD_TEST_SASS_GEN2(sysInst));
  }
  
  /* Prepare Security Context for the authentication Engine */
  if (authScSize)
  {
        salld_set_sc_acAuth(txInst->authMode, pConfig->sessionMacKeySize, 
                       pComInfo->sessionMacKey, 
                       salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize + encScSize),
                       &acAlgorithm,
                       dir, SALLD_TEST_SASS_GEN2(sysInst));
  }
                      
  /* Construct the Air Ciphering Tx specific Security Context */    
  {
    tword* ctxIn = salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_COMMON_SIZE);
    tword* pAux1 = ctxIn + SALLD_FIELDOFFSET(saCtxProtoAc_t, aux1);
    acHdrConfigParams_t *pHdrConfig = &acHdrParamsTx[pConfig->pduType];
    uint16_t ctrlBitfield = pHdrConfig->ctrlBitfield, ctrlBitfield2 = 0;
  
    SA_CTX_PROTO_AC_SET_ALGORITHM(ctrlBitfield, acAlgorithm);
    if(dir)
        SA_CTX_PROTO_AC_SET_DIR(ctrlBitfield, 1);
    if (encScSize)
        SA_CTX_PROTO_AC_SET_FLAG_ENC(ctrlBitfield, 1);
    if (authScSize)
        SA_CTX_PROTO_AC_SET_FLAG_AUTH(ctrlBitfield, 1);
    if( txInst->authMode == sa_AuthMode_CMAC)
        SA_CTX_PROTO_AC_SET_FLAG_CMAC(ctrlBitfield, 1);
    else if ( txInst->authMode == sa_AuthMode_KASUMI_F9)     
        SA_CTX_PROTO_AC_SET_FLAG_KASUMI_F9(ctrlBitfield, 1);
    else if ((acAlgorithm == SA_AC_ALGORITHM_SNOW3G) || (acAlgorithm == SA_AC_ALGORITHM_ZUC))
        SA_CTX_PROTO_AC_SET_FLAG_SNOW3G_ZUC(ctrlBitfield, 1);
        
    if ((txInst->cipherMode == sa_CipherMode_SNOW3G_F8F9) || (txInst->cipherMode == sa_CipherMode_ZUC_F8F9))
    {
        SA_CTX_PROTO_AC_SET_FLAG_F8F9(ctrlBitfield, 1);
        SA_CTX_PROTO_AC_SET_FLAG_AUTH(ctrlBitfield, 1);
        f8f9opt = (pHdrConfig->authHdrSize << SA_F8F9_OPT_M_SHIFT) | SA_F8F9_OPT_RX;
    }    
    
    if(pConfig->ctrlBitMap & sa_AC_CONFIG_COPY_COUNTC)
        SA_CTX_PROTO_AC_SET_FLAG_COPY_COUNT_C(ctrlBitfield2, 1); 
        
    /* Check if keys need to be stored in scratch memory */
    if (pConfig->ctrlBitMap & sa_AC_CONFIG_KEY_IN_SCRATCH)
    {
        uint16_t encKeyOffsetInByte = 0;
        CSL_Cp_aceRegs * scratchRegs = (CSL_Cp_aceRegs*) salldLObj.baseAddr;
		salldObj_t * owner = (salldObj_t *) sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr,inst->salldInst.ownerInstOffset);  
        uint32_t * scratchAllocBitMap = owner->scratchAllocBitmap;
        uint32_t * scratchKeyArrayBaseAddr;
        int numKeyMapBlocks = SALLD_TEST_SASS_GEN2(sysInst)?SALLD_AC_MAX_32BIT_BLOCKS_IN_BITMAP2:SALLD_AC_MAX_32BIT_BLOCKS_IN_BITMAP;
        
        if(SA_CTX_PROTO_AC_GET_FLAG_F8F9(ctrlBitfield))
        {
            /* Note: authScSize == 0 in this case */
            uint16_t macKeyOffsetInByte = 0, calculatedOffset = 0;
            uint32_t * scratchKeyBuf;
        
            /* Initialize pointer to enc key in ctx */
            uint8_t* ctxEncKey = (uint8_t*)(salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize) + SALLD_FIELDOFFSET(saCtxAc_t, key));
            uint8_t* ctxMacKey = ctxEncKey + 16;
            
            /* Allocate key storage places */
            if ((encKeyOffsetInByte = salld_ac_key_allocation_bitmap_allocate(numKeyMapBlocks, scratchAllocBitMap)) == SALLD_AC_KEY_ALLOCATION_ERROR)
                return(sa_ERR_SCRATCH_MEMORY_FULL);
                
            if ((macKeyOffsetInByte = salld_ac_key_allocation_bitmap_allocate(numKeyMapBlocks, scratchAllocBitMap)) == SALLD_AC_KEY_ALLOCATION_ERROR)
            {
                salld_ac_key_allocation_bitmap_free(scratchAllocBitMap, encKeyOffsetInByte);
                return(sa_ERR_SCRATCH_MEMORY_FULL);
            }    
            
            /* Calculate free block offset into scratch memory */
            calculatedOffset = SALLD_AC_BYTE_TO_UINT32_OFFSET((encKeyOffsetInByte & SALLD_AC_SCRATCH_MEMORY_BLOCK_MASK));
            
            /*
             * Note: The SRAM2 location is the same as the one on older keystone2 devices and SRAM1 is only available 
             *       on advanced keystone2 devices. Therefore, we need to use SRAM2 for the first 1K 16-byte keys
             *
             */
            scratchKeyArrayBaseAddr = (SALLD_AC_SCRATCH_MEMORY_BLOCK_NUM(encKeyOffsetInByte))?
                                      (uint32_t*)(scratchRegs->SRAM1):(uint32_t*)(scratchRegs->SRAM2);
                                      
            /* Move pointer to beginning of free allocation block */
            scratchKeyBuf = scratchKeyArrayBaseAddr + calculatedOffset;
            
            /* Copy key from ctx into scratch memory */
            for(i = 0, j = 0; i< SALLD_AC_DEFAULT_KEY_SIZE_IN_WORD; i++, j+=4)
            {
                /* tmp variable to make compiler happy */
                uint8_t *tmp = ctxEncKey+j;
                scratchKeyBuf[i] = SALLD_MK_UINT32_FROM_8ARRAY(tmp);
            }
            
            /* Erase key from ctx */
            memset(ctxEncKey, 0, 16);
            
            /* Record key offset in pComInfo in order to free on channel close */
            pComInfo->sessionEncKeyScratchOffset = encKeyOffsetInByte;
            
            /* Record key offset in scratch memory */
            pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, encKeyOffset), encKeyOffsetInByte);
            
            /* Calculate free block offset into scratch memory */
            calculatedOffset = SALLD_AC_BYTE_TO_UINT32_OFFSET((macKeyOffsetInByte & SALLD_AC_SCRATCH_MEMORY_BLOCK_MASK));
            
            /*
             * Note: The SRAM2 location is the same as the one on older keystone2 devices and SRAM1 is only available 
             *       on advanced keystone2 devices. Therefore, we need to use SRAM2 for the first 1K 16-byte keys
             *
             */
            scratchKeyArrayBaseAddr = (SALLD_AC_SCRATCH_MEMORY_BLOCK_NUM(macKeyOffsetInByte))?
                                      (uint32_t*)(scratchRegs->SRAM1):(uint32_t*)(scratchRegs->SRAM2);
            
            /* Move pointer to beginning of free allocation block */
            scratchKeyBuf = scratchKeyArrayBaseAddr + calculatedOffset;
            
            /* Copy key from ctx into scratch memory */
            for(i = 0, j = 0; i< SALLD_AC_DEFAULT_KEY_SIZE_IN_WORD; i++, j+=4)
            {
                /* tmp variable to make compiler happy */
                uint8_t *tmp = ctxMacKey+j;
                scratchKeyBuf[i] = SALLD_MK_UINT32_FROM_8ARRAY(tmp);
            }
            /* Erase key from ctx */
            memset(ctxMacKey, 0, 16);
            
            /* Record key offset in pComInfo in order to free on channel close */
            pComInfo->sessionMacKeyScratchOffset = macKeyOffsetInByte;
            
            /* Record offset key offset in scratch memory */
            pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, macKeyOffset), macKeyOffsetInByte);
            
            /* Set flag for firmware to fetch key from scratch memory */
            SA_CTX_PROTO_AC_SET_FLAG_KEY_IN_SCRATCH(ctrlBitfield,1);
            
            /* Increment the command label size by 32 bytes */
            encCmdlSize += 32;
        
        } 
    
        else if (encScSize)
        {
            uint16_t calculatedOffset = 0;
            uint32_t * scratchEncKeyBuf;
            
            /* Initialize pointer to enc key in ctx */
            uint8_t* ctxEncKey = (uint8_t*)(salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize) + SALLD_FIELDOFFSET(saCtxAc_t, key));
            
            /* Allocate key storage places */
            if ((encKeyOffsetInByte = salld_ac_key_allocation_bitmap_allocate(numKeyMapBlocks, scratchAllocBitMap)) == SALLD_AC_KEY_ALLOCATION_ERROR)
                return(sa_ERR_SCRATCH_MEMORY_FULL);
            
            /* Calculate free block offset into scratch memory */
            calculatedOffset = SALLD_AC_BYTE_TO_UINT32_OFFSET((encKeyOffsetInByte & SALLD_AC_SCRATCH_MEMORY_BLOCK_MASK));
            
            /*
             * Note: The SRAM2 location is the same as the one on older keystone2 devices and SRAM1 is only available 
             *       on advanced keystone2 devices. Therefore, we need to use SRAM2 for the first 1K 16-byte keys
             *
             */
            scratchKeyArrayBaseAddr = (SALLD_AC_SCRATCH_MEMORY_BLOCK_NUM(encKeyOffsetInByte))?
                                      (uint32_t*)(scratchRegs->SRAM1):(uint32_t*)(scratchRegs->SRAM2);
            
            /* Move pointer to beginning of free allocation block */
            scratchEncKeyBuf = scratchKeyArrayBaseAddr + calculatedOffset;
            
            /* Copy key from ctx into scratch memory */
            for(i = 0, j = 0; i< SALLD_AC_DEFAULT_KEY_SIZE_IN_WORD; i++, j+=4)
            {
                /* tmp variable to make compiler happy */
                uint8_t *tmp = ctxEncKey+j;
                scratchEncKeyBuf[i] = SALLD_MK_UINT32_FROM_8ARRAY(tmp);
            }
            /* Erase key from ctx */
            memset(ctxEncKey, 0, 16);
            
            /* Record key offset in pComInfo in order to free on channel close */
            pComInfo->sessionEncKeyScratchOffset = encKeyOffsetInByte;
            
            /* Record offset key offset in scratch memory */
            pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, encKeyOffset), encKeyOffsetInByte);
            
            /* Set flag for firmware to fetch key from scratch memory */
            SA_CTX_PROTO_AC_SET_FLAG_KEY_IN_SCRATCH(ctrlBitfield,1);
            
            /* Increment the command label size by 16 bytes */
            encCmdlSize += 16;
            
            /* Check to see if we need to create modkey for kasumi */
            if((acAlgorithm == SA_AC_ALGORITHM_GSM_A53) || (acAlgorithm == SA_AC_ALGORITHM_KASUMI))
            {
                uint8_t* ctxAux1 = (uint8_t*)(salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize) + SALLD_FIELDOFFSET(saCtxAc_t, aux1));
                
                /* Erase kasumi modkey from ctx */
                memset(ctxAux1, 0, 16);
                
                /* Set flag for kasumi modkey recreation by firmware */
                SA_CTX_PROTO_AC_SET_FLAG_KASUMI_F8(ctrlBitfield, 1);
                
                /* Make room for cmdl to store kasumi modkey as an option*/
                encCmdlSize += 16;
            }
        }
        
        if (authScSize)
        {
            uint16_t macKeyOffsetInByte = 0, calculatedOffset = 0;
            uint32_t * scratchMacKeyBuf;
            
            /* Initialize pointer to mac key in ctx */
            uint8_t* ctxMacKey = (uint8_t*)(salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize + encScSize) + SALLD_FIELDOFFSET(saCtxAc_t, key));
            
            if ((acAlgorithm == SA_AC_ALGORITHM_SNOW3G) || (acAlgorithm == SA_AC_ALGORITHM_ZUC))
            {
                /* Auth Key is stored at the second half of the key area  */
                ctxMacKey += 16; 
            }
            
            /* Allocate key storage places */
            if ((macKeyOffsetInByte = salld_ac_key_allocation_bitmap_allocate(numKeyMapBlocks, scratchAllocBitMap)) == SALLD_AC_KEY_ALLOCATION_ERROR)
            {
                if(encScSize)
                    salld_ac_key_allocation_bitmap_free(scratchAllocBitMap, encKeyOffsetInByte);
                    
                return(sa_ERR_SCRATCH_MEMORY_FULL);
            }    
            
            /* Calculate free block offset into scratch memory */
            calculatedOffset = SALLD_AC_BYTE_TO_UINT32_OFFSET((macKeyOffsetInByte & SALLD_AC_SCRATCH_MEMORY_BLOCK_MASK));
            
            /*
             * Note: The SRAM2 location is the same as the one on older keystone2 devices and SRAM1 is only available 
             *       on advanced keystone2 devices. Therefore, we need to use SRAM2 for the first 1K 16-byte keys
             *
             */
            scratchKeyArrayBaseAddr = (SALLD_AC_SCRATCH_MEMORY_BLOCK_NUM(macKeyOffsetInByte))?
                                      (uint32_t*)(scratchRegs->SRAM1):(uint32_t*)(scratchRegs->SRAM2);
                
            /* Move pointer to beginning of free allocation block */
            scratchMacKeyBuf = scratchKeyArrayBaseAddr + calculatedOffset;
            
            /* Copy key from ctx into scratch memory */
            for(i = 0, j = 0; i< SALLD_AC_DEFAULT_KEY_SIZE_IN_WORD; i++, j+=4)
            {
                /* tmp variable to make compiler happy */
                uint8_t *tmp = ctxMacKey+j;
                scratchMacKeyBuf[i] = SALLD_MK_UINT32_FROM_8ARRAY(tmp);
            }
            /* Erase key from ctx */
            memset(ctxMacKey, 0, 16);
            
            /* Record key offset in pComInfo in order to free on channel close */
            pComInfo->sessionMacKeyScratchOffset = macKeyOffsetInByte;
            
            /* Record offset key offset in scratch memory */
            pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, macKeyOffset), macKeyOffsetInByte);
            
            /* Set flag for firmware to fetch key from scratch memory */
            SA_CTX_PROTO_AC_SET_FLAG_KEY_IN_SCRATCH(ctrlBitfield,1);
            
            /* Increment the command label size by 16 bytes */
            authCmdlSize += 16;
            
            if(txInst->authMode == sa_AuthMode_KASUMI_F9)
            {
                uint8_t* ctxAux1 = (uint8_t*)(salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize + encScSize) + SALLD_FIELDOFFSET(saCtxAc_t, aux1));
                
                /* Erase kasumi modkey from ctx */
                memset(ctxAux1, 0, 16);
                
                /* Make room for cmdl to store kasumi modkey as an option*/
                authCmdlSize += 16;
            }
        }
    }
    cmdlSize = encCmdlSize + authCmdlSize;
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, ctrlBitfield), ctrlBitfield);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, ctrlBitfield2), ctrlBitfield2);

    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, countC), pConfig->countC);
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, fresh), pConfig->fresh);
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, ivLow26), pConfig->ivLow26);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, firstEngIdCmdlLen), 
                     SALLD_MK_UINT16((cmdlSize?SALLD_CMDL_ENGINE_ID_ACS1:SALLD_CMDL_ENGINE_SRTP_AC_HPS2), cmdlSize));
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, hdrSizeAuthHdrSize), 
                     SALLD_MK_UINT16(pHdrConfig->hdrSize, pHdrConfig->authHdrSize));
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, seqNumSizeShift), 
                     SALLD_MK_UINT16(pHdrConfig->seqNumSize, pHdrConfig->seqNumShift));
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, bearerIvSize), 
                     SALLD_MK_UINT16((pConfig->ctrlBitMap & sa_AC_CONFIG_BEARER_MASK), pConfig->ivSize));
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, ivOptIcvSize), 
                     SALLD_MK_UINT16(SA_CTX_PROTO_AC_GET_FLAG_F8F9(ctrlBitfield)?f8f9opt:
                                     SALLD_CMDL_MK_OPTION_CTRL(SA_AC_AUX2_OFFSET, pConfig->ivSize),
                                     pConfig->macSize));
                                     
                                     
    /* Derive and Contruct K1/K2 if CMAC mode */                               
    if (txInst->authMode == sa_AuthMode_CMAC)
    {
        salld_aes_cmac_get_keys((tword *)pComInfo->sessionMacKey, pConfig->sessionMacKeySize << 3,
                                (tword *)pAux1, 
                                (tword *)(pAux1 + SALLD_BYTE_TO_WORD(SALLD_AES_BLOCK_SIZE_IN_BYTE)));
    }
                                     
  }
   
  /* Security Context swizzling */
  salld_swiz_128(salld_ac_tmp_scBuf, salld_ac_tmp_scBuf, pScInfo->scSize);
  
  if((uint8_t *) pScInfo->scBuf != salld_ac_tmp_scBuf)
    memcpy((void *) pScInfo->scBuf, salld_ac_tmp_scBuf, pScInfo->scSize);
    
  Sa_osalEndScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  /* Prepare the SW Info Words */
  salld_set_swInfo(SALLD_CMDL_ENGINE_SRTP_AC_HPS1, 0, NULL,
                   pScInfo, &txInst->swInfo, 0);
  /* Store the scBuf internally as offset to suppor multiprocess */
  pScInfo->scBuf = (uintptr_t) sa_CONV_ADDR_TO_OFFSET(salldLObj.scPoolBaseAddr, pScInfo->scBuf);
  
  return (sa_ERR_OK);
}            

/****************************************************************************
 * FUNCTION PURPOSE: Construct Air Ciphering Rx Security Context
 ****************************************************************************
 * DESCRIPTION: Construct Security Context from the configuration parameters 
 *          for Air Ciphering Rx (To-Air) operations
 *
 *  uint16_t salld_ac_set_rx_sc(
 *            salldIpsecInst_t*     inst)      -> Point to AC channel instance
 *                       
 * Return values: sa_ERR_XXX
 * 
 * Assumption: The same algorithm will be used for both encryption and 
 *             authentication                          
 *
 ***************************************************************************/
int16_t salld_ac_set_rx_sc(salldAcInst_t *inst) 
{
  salldObj_t *sysInst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->salldInst.ownerInstOffset);
  salldAcRxInst_t *rxInst =  &inst->rxInst;
  salldAcComInfo_t *pComInfo = &rxInst->comInfo;
  Sa_AcConfigParams_t  *pConfig = &pComInfo->config;
  Sa_ScReqInfo_t* pScInfo = &rxInst->scInfo;
  int16_t encScSize;
  int16_t authScSize;
  int16_t phpScSize = (rxInst->authMode == sa_AuthMode_CMAC)?SA_CTX_PHP_AC_TYPE2_SIZE:SA_CTX_PHP_AC_TYPE1_SIZE;
  int16_t encCmdlSize;
  int16_t authCmdlSize;
  int16_t cmdlSize;
  int16_t acAlgorithm = SA_AC_ALGORITHM_GSM_A53;
  saDMAReqInfo_t dmaReqInfo;
  int16_t dir = (pConfig->ctrlBitMap & sa_AC_CONFIG_DIR)?1:0;
  int i,j;
  uint8_t *salld_ac_tmp_scBuf;
  uint8_t f8f9opt = 0;
  
  /*
   * Calculate the security Context Size and allocate security Context
   *
   */
  salld_sc_enc_get_info(rxInst->cipherMode, pConfig->ivSize, &encCmdlSize, &encScSize, NULL, NULL, SALLD_TEST_SASS_GEN2(sysInst)); 
  salld_sc_auth_get_info(rxInst->authMode, NULL, &authCmdlSize, &authScSize, NULL);
  
  pScInfo->scSize = phpScSize + encScSize + authScSize;
  salldLObj.callOutFuncs.ScAlloc((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo);
  
  if(pScInfo->scBuf == (uintptr_t) NULL)
    return(sa_ERR_NO_CTX_BUF);
    
  Sa_osalBeginScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  
  /* Check if need to use special buffer for security context */
  if (pConfig->ctrlBitMap & sa_AC_CONFIG_KEY_IN_SCRATCH)
  {
    salld_ac_tmp_scBuf = (uint8_t*)salldLObj.intBuf;
	if (salld_ac_tmp_scBuf == NULL)
		return(sa_ERR_INV_INT_MEM);	
  }
  else
  {
    salld_ac_tmp_scBuf = (uint8_t *)pScInfo->scBuf;
  }
  
  if(salld_ac_tmp_scBuf == NULL)
    return(sa_ERR_NO_CTX_BUF);
  
  memset(salld_ac_tmp_scBuf, 0, SALLD_BYTE_TO_WORD(pScInfo->scSize));
  
  /* Prepare PHP Security Context */
  memset(&dmaReqInfo, 0, sizeof(saDMAReqInfo_t));
  dmaReqInfo.phpFetchSize = SA_CTX_SIZE_TO_DMA_SIZE(phpScSize);
  if (authScSize)
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(encScSize);
  }
  else
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(encScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_DMA_SIZE_0;
  }
  dmaReqInfo.phpEvictSize = SA_CTX_DMA_SIZE_64;

  salld_set_sc_phpCommom(&dmaReqInfo, &rxInst->destInfo, SA_CTX_PKT_TYPE_3GPP_AIR|SA_CTX_PKT_DIR_RX,
                         pScInfo->scID, salld_ac_tmp_scBuf);  
                      
  /* Prepare Security Context for the encryption Engine */
  if (encScSize)
  {
        salld_set_sc_acEnc(rxInst->cipherMode, pConfig->sessionEncKeySize, 
                    pComInfo->sessionEncKey, pComInfo->sessionMacKey, TRUE,
                    salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize + authScSize),
                    &acAlgorithm, SALLD_TEST_SASS_GEN2(sysInst));
  }
                      
                      
  /* Prepare Security Context for the authentication Engine */
  if (authScSize)
  {
        salld_set_sc_acAuth(rxInst->authMode, pConfig->sessionMacKeySize, 
                     pComInfo->sessionMacKey, 
                     salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize),
                     &acAlgorithm,
                     dir, SALLD_TEST_SASS_GEN2(sysInst));
  }
                      
  /* Construct the Air Ciphering Rx specific Security Context */    
  {
    tword* ctxIn = salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_COMMON_SIZE);
    tword* pAux1 = ctxIn + SALLD_FIELDOFFSET(saCtxProtoAc_t, aux1);
    acHdrConfigParams_t *pHdrConfig = &acHdrParamsRx[pConfig->pduType];
    uint16_t ctrlBitfield = pHdrConfig->ctrlBitfield, ctrlBitfield2=0;
    
  
    SA_CTX_PROTO_AC_SET_ALGORITHM(ctrlBitfield, acAlgorithm);
    if(dir)
        SA_CTX_PROTO_AC_SET_DIR(ctrlBitfield, 1);
    if (encScSize)
        SA_CTX_PROTO_AC_SET_FLAG_ENC(ctrlBitfield, 1);
    if (authScSize)
        SA_CTX_PROTO_AC_SET_FLAG_AUTH(ctrlBitfield, 1);
    if (rxInst->authMode == sa_AuthMode_CMAC)
        SA_CTX_PROTO_AC_SET_FLAG_CMAC(ctrlBitfield, 1);
    else if ( rxInst->authMode == sa_AuthMode_KASUMI_F9)     
        SA_CTX_PROTO_AC_SET_FLAG_KASUMI_F9(ctrlBitfield, 1);
    else if ((acAlgorithm == SA_AC_ALGORITHM_SNOW3G) || (acAlgorithm == SA_AC_ALGORITHM_ZUC))
        SA_CTX_PROTO_AC_SET_FLAG_SNOW3G_ZUC(ctrlBitfield, 1);
        
    if ((rxInst->cipherMode == sa_CipherMode_SNOW3G_F8F9) || (rxInst->cipherMode == sa_CipherMode_ZUC_F8F9))
    {
        SA_CTX_PROTO_AC_SET_FLAG_F8F9(ctrlBitfield, 1);
        SA_CTX_PROTO_AC_SET_FLAG_AUTH(ctrlBitfield, 1);
        f8f9opt = (pHdrConfig->authHdrSize << SA_F8F9_OPT_M_SHIFT) | SA_F8F9_OPT_TX;
    }    
        
    if(pConfig->ctrlBitMap & sa_AC_CONFIG_COPY_COUNTC)
        SA_CTX_PROTO_AC_SET_FLAG_COPY_COUNT_C(ctrlBitfield2, 1); 
    if((pConfig->ctrlBitMap & sa_AC_CONFIG_COUNTC_BY_APP) && (pConfig->pduType == sa_AcPduType_LTE))
        ctrlBitfield |= SA_CTX_PROTO_AC_FLAG_COUNTC_PRESENT; 

    /* Check if keys need to be stored in scratch memory */
    if (pConfig->ctrlBitMap & sa_AC_CONFIG_KEY_IN_SCRATCH)
    {
        uint16_t encKeyOffsetInByte = 0;
        CSL_Cp_aceRegs * scratchRegs = (CSL_Cp_aceRegs*) salldLObj.baseAddr;
	    salldObj_t * owner = (salldObj_t *) sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr,inst->salldInst.ownerInstOffset);  
        uint32_t * scratchAllocBitMap = owner->scratchAllocBitmap;
        uint32_t * scratchKeyArrayBaseAddr;
        int numKeyMapBlocks = SALLD_TEST_SASS_GEN2(sysInst)?SALLD_AC_MAX_32BIT_BLOCKS_IN_BITMAP2:SALLD_AC_MAX_32BIT_BLOCKS_IN_BITMAP;
        
        if(SA_CTX_PROTO_AC_GET_FLAG_F8F9(ctrlBitfield))
        {
            /* Note: authScSize == 0 in this case */
            uint16_t macKeyOffsetInByte = 0, calculatedOffset = 0;
            uint32_t * scratchKeyBuf;

            /* Initialize pointer to enc key in ctx */
            uint8_t* ctxEncKey = (uint8_t*)(salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize) + SALLD_FIELDOFFSET(saCtxAc_t, key));
            uint8_t* ctxMacKey = ctxEncKey + 16;
            
            /* Allocate key storage places */
            if ((encKeyOffsetInByte = salld_ac_key_allocation_bitmap_allocate(numKeyMapBlocks, scratchAllocBitMap)) == SALLD_AC_KEY_ALLOCATION_ERROR)
                return(sa_ERR_SCRATCH_MEMORY_FULL);
                
            if ((macKeyOffsetInByte = salld_ac_key_allocation_bitmap_allocate(numKeyMapBlocks, scratchAllocBitMap)) == SALLD_AC_KEY_ALLOCATION_ERROR)
            {
                salld_ac_key_allocation_bitmap_free(scratchAllocBitMap, encKeyOffsetInByte);
                return(sa_ERR_SCRATCH_MEMORY_FULL);
            }    
            
            /* Calculate free block offset into scratch memory */
            calculatedOffset = SALLD_AC_BYTE_TO_UINT32_OFFSET((encKeyOffsetInByte & SALLD_AC_SCRATCH_MEMORY_BLOCK_MASK));
            
            /*
             * Note: The SRAM2 location is the same as the one on older keystone2 devices and SRAM1 is only available 
             *       on advanced keystone2 devices. Therefore, we need to use SRAM2 for the first 1K 16-byte keys
             *
             */
            scratchKeyArrayBaseAddr = (SALLD_AC_SCRATCH_MEMORY_BLOCK_NUM(encKeyOffsetInByte))?
                                      (uint32_t*)(scratchRegs->SRAM1):(uint32_t*)(scratchRegs->SRAM2);
            
            /* Move pointer to beginning of free allocation block */
            scratchKeyBuf = scratchKeyArrayBaseAddr + calculatedOffset;
            
            /* Copy key from ctx into scratch memory */
            for(i = 0, j = 0; i< SALLD_AC_DEFAULT_KEY_SIZE_IN_WORD; i++, j+=4)
            {
                /* tmp variable to make compiler happy */
                uint8_t *tmp = ctxEncKey+j;
                scratchKeyBuf[i] = SALLD_MK_UINT32_FROM_8ARRAY(tmp);
            }
            
            /* Erase key from ctx */
            memset(ctxEncKey, 0, 16);
            
            /* Record key offset in pComInfo in order to free on channel close */
            pComInfo->sessionEncKeyScratchOffset = encKeyOffsetInByte;
            
            /* Record key offset in scratch memory */
            pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, encKeyOffset), encKeyOffsetInByte);
            
            /* Calculate free block offset into scratch memory */
            calculatedOffset = SALLD_AC_BYTE_TO_UINT32_OFFSET((macKeyOffsetInByte & SALLD_AC_SCRATCH_MEMORY_BLOCK_MASK));
            
            /*
             * Note: The SRAM2 location is the same as the one on older keystone2 devices and SRAM1 is only available 
             *       on advanced keystone2 devices. Therefore, we need to use SRAM2 for the first 1K 16-byte keys
             *
             */
            scratchKeyArrayBaseAddr = (SALLD_AC_SCRATCH_MEMORY_BLOCK_NUM(macKeyOffsetInByte))?
                                      (uint32_t*)(scratchRegs->SRAM1):(uint32_t*)(scratchRegs->SRAM2);
            
            /* Move pointer to beginning of free allocation block */
            scratchKeyBuf = scratchKeyArrayBaseAddr + calculatedOffset;
            
            /* Copy key from ctx into scratch memory */
            for(i = 0, j = 0; i< SALLD_AC_DEFAULT_KEY_SIZE_IN_WORD; i++, j+=4)
            {
                /* tmp variable to make compiler happy */
                uint8_t *tmp = ctxMacKey+j;
                scratchKeyBuf[i] = SALLD_MK_UINT32_FROM_8ARRAY(tmp);
            }
            /* Erase key from ctx */
            memset(ctxMacKey, 0, 16);
            
            /* Record key offset in pComInfo in order to free on channel close */
            pComInfo->sessionMacKeyScratchOffset = macKeyOffsetInByte;
            
            /* Record offset key offset in scratch memory */
            pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, macKeyOffset), macKeyOffsetInByte);
            
            /* Set flag for firmware to fetch key from scratch memory */
            SA_CTX_PROTO_AC_SET_FLAG_KEY_IN_SCRATCH(ctrlBitfield,1);
            
            /* Increment the command label size by 32 bytes */
            encCmdlSize += 32;
        
        } 
        else if (encScSize)
        {
            uint16_t calculatedOffset = 0;
            uint32_t * scratchEncKeyBuf;

            /* Initialize pointer to enc key in ctx */
            uint8_t* ctxEncKey = (uint8_t*)(salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize + authScSize) + SALLD_FIELDOFFSET(saCtxAc_t, key));
        
            /* Allocate key storage places */
            if ((encKeyOffsetInByte = salld_ac_key_allocation_bitmap_allocate(numKeyMapBlocks, scratchAllocBitMap)) == SALLD_AC_KEY_ALLOCATION_ERROR)
                return(sa_ERR_SCRATCH_MEMORY_FULL);
            
            /* Calculate free block offset into scratch memory */
            calculatedOffset = SALLD_AC_BYTE_TO_UINT32_OFFSET((encKeyOffsetInByte & SALLD_AC_SCRATCH_MEMORY_BLOCK_MASK));
            
            /*
             * Note: The SRAM2 location is the same as the one on older keystone2 devices and SRAM1 is only available 
             *       on advanced keystone2 devices. Therefore, we need to use SRAM2 for the first 1K 16-byte keys
             *
             */
            scratchKeyArrayBaseAddr = (SALLD_AC_SCRATCH_MEMORY_BLOCK_NUM(encKeyOffsetInByte))?
                                      (uint32_t*)(scratchRegs->SRAM1):(uint32_t*)(scratchRegs->SRAM2);
            
            /* Move pointer to beginning of free allocation block */
            scratchEncKeyBuf = scratchKeyArrayBaseAddr + calculatedOffset;
            
            /* Copy key from ctx into scratch memory */
            for(i = 0, j = 0; i< SALLD_AC_DEFAULT_KEY_SIZE_IN_WORD; i++, j+=4)
            {
                /* tmp variable to make compiler happy */
                uint8_t *tmp = ctxEncKey+j;
                scratchEncKeyBuf[i] = SALLD_MK_UINT32_FROM_8ARRAY(tmp);
            }
            /* Erase key from ctx */
            memset(ctxEncKey, 0, 16);
            
            /* Record key offset in pComInfo in order to free on channel close */
            pComInfo->sessionEncKeyScratchOffset = encKeyOffsetInByte;
            
            /* Record key offset in scratch memory */
            pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, encKeyOffset), encKeyOffsetInByte);
            
            /* Set flag for firmware to fetch key from scratch memory */
            SA_CTX_PROTO_AC_SET_FLAG_KEY_IN_SCRATCH(ctrlBitfield,1);
            
            /* Increment the command label size by 16 bytes */
            encCmdlSize += 16;
            
            if((acAlgorithm == SA_AC_ALGORITHM_GSM_A53) || (acAlgorithm == SA_AC_ALGORITHM_KASUMI))
            {
                uint8_t* ctxAux1 = (uint8_t*)(salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize + authScSize) + SALLD_FIELDOFFSET(saCtxAc_t, aux1));
                
                /* Erase kasumi modkey from ctx */
                memset(ctxAux1, 0, 16);
                
                /* Set flag for kasumi modkey recreation by firmware */
                SA_CTX_PROTO_AC_SET_FLAG_KASUMI_F8(ctrlBitfield, 1);
                
                /* Make room for cmdl to store kasumi modkey as an option*/
                encCmdlSize += 16;
            }
        }
        
        if (authScSize)
        {
            uint16_t macKeyOffsetInByte = 0, calculatedOffset = 0;
            uint32_t * scratchMacKeyBuf;
            
            /* Initialize pointer to mac key in ctx */
            uint8_t* ctxMacKey = (uint8_t*)(salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize) + SALLD_FIELDOFFSET(saCtxAc_t, key));
            
            if ((acAlgorithm == SA_AC_ALGORITHM_SNOW3G) || (acAlgorithm == SA_AC_ALGORITHM_ZUC))
            {
                /* Auth Key is stored at the second half of the key area  */
                ctxMacKey += 16; 
            }       
            
            /* Allocate key storage places */
            if ((macKeyOffsetInByte = salld_ac_key_allocation_bitmap_allocate(numKeyMapBlocks, scratchAllocBitMap)) == SALLD_AC_KEY_ALLOCATION_ERROR)
            {
                if(encScSize)
                    salld_ac_key_allocation_bitmap_free(scratchAllocBitMap, encKeyOffsetInByte);
                    
                return(sa_ERR_SCRATCH_MEMORY_FULL);
            }    
            
            /* Calculate free block offset into scratch memory */
            calculatedOffset = SALLD_AC_BYTE_TO_UINT32_OFFSET((macKeyOffsetInByte & SALLD_AC_SCRATCH_MEMORY_BLOCK_MASK));
            
            /*
             * Note: The SRAM2 location is the same as the one on older keystone2 devices and SRAM1 is only available 
             *       on advanced keystone2 devices. Therefore, we need to use SRAM2 for the first 1K 16-byte keys
             *
             */
            scratchKeyArrayBaseAddr = (SALLD_AC_SCRATCH_MEMORY_BLOCK_NUM(macKeyOffsetInByte))?
                                      (uint32_t*)(scratchRegs->SRAM1):(uint32_t*)(scratchRegs->SRAM2);
            
            /* Move pointer to beginning of free allocation block */
            scratchMacKeyBuf = scratchKeyArrayBaseAddr + calculatedOffset;
            
            /* Copy key from ctx into scratch memory */
            for(i = 0, j = 0; i< SALLD_AC_DEFAULT_KEY_SIZE_IN_WORD; i++, j+=4)
            {
                /* tmp variable to make compiler happy */
                uint8_t *tmp = ctxMacKey+j;
                scratchMacKeyBuf[i] = SALLD_MK_UINT32_FROM_8ARRAY(tmp);
            }
            /* Erase key from ctx */
            memset(ctxMacKey, 0, 16);
            
            /* Record key offset in pComInfo in order to free on channel close */
            pComInfo->sessionMacKeyScratchOffset = macKeyOffsetInByte;
            
            /* Record offset key offset in scratch memory */
            pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, macKeyOffset), macKeyOffsetInByte);
            
            /* Set flag for firmware to fetch key from scratch memory */
            SA_CTX_PROTO_AC_SET_FLAG_KEY_IN_SCRATCH(ctrlBitfield,1);
            
            /* Increment the command label size by 16 bytes */
            authCmdlSize += 16;
            if(rxInst->authMode == sa_AuthMode_KASUMI_F9)
            {
                uint8_t* ctxAux1 = (uint8_t*)(salld_ac_tmp_scBuf + SALLD_BYTE_TO_WORD(phpScSize) + SALLD_FIELDOFFSET(saCtxAc_t, aux1));
                
                /* Erase kasumi modkey from ctx */
                memset(ctxAux1, 0, 16);
                
                /* Make room for cmdl to store kasumi modkey as an option*/
                authCmdlSize += 16;
            }
            
        }
    }
    
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, ctrlBitfield), ctrlBitfield);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, ctrlBitfield2), ctrlBitfield2);

    if (ctrlBitfield & SA_CTX_PROTO_AC_FLAG_COUNTC_INSERT)
    {
        /*
         * The countC in the security context records the 32-bit countC used by the last to-air 
         * packet. The PHP firmware needs to increment the count-C by one and then
         * use it for the incoming packet. Therefore, we need to adjust the inital count-C value
         * in the security context.
         */
        pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, countC), pConfig->countC - 1);
    }
    else
    {
        pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, countC), pConfig->countC);
    
    }    
    cmdlSize = encCmdlSize + authCmdlSize;
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, fresh), pConfig->fresh);
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, ivLow26), pConfig->ivLow26);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, firstEngIdCmdlLen), 
                     SALLD_MK_UINT16((cmdlSize?SALLD_CMDL_ENGINE_ID_ACS1:SALLD_CMDL_ENGINE_SRTP_AC_HPS2), cmdlSize));
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, hdrSizeAuthHdrSize), 
                     SALLD_MK_UINT16(pHdrConfig->hdrSize, pHdrConfig->authHdrSize));
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, seqNumSizeShift), 
                     SALLD_MK_UINT16(pHdrConfig->seqNumSize, pHdrConfig->seqNumShift));
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, bearerIvSize), 
                     SALLD_MK_UINT16((pConfig->ctrlBitMap & sa_AC_CONFIG_BEARER_MASK), pConfig->ivSize));
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoAc_t, ivOptIcvSize), 
                     SALLD_MK_UINT16(SA_CTX_PROTO_AC_GET_FLAG_F8F9(ctrlBitfield)?f8f9opt:
                                     SALLD_CMDL_MK_OPTION_CTRL(SA_AC_AUX2_OFFSET, pConfig->ivSize),
                                     pConfig->macSize));
                                     
    /* Derive and Contruct K1/K2 if CMAC mode */                               
    if (rxInst->authMode == sa_AuthMode_CMAC)
    {
        salld_aes_cmac_get_keys((tword *)pComInfo->sessionMacKey, pConfig->sessionMacKeySize << 3,
                                (tword *)pAux1, 
                                (tword *)(pAux1 + SALLD_BYTE_TO_WORD(SALLD_AES_BLOCK_SIZE_IN_BYTE)));
    }
                                     
  }
  
  /* Security Context swizzling */
  salld_swiz_128(salld_ac_tmp_scBuf, salld_ac_tmp_scBuf, pScInfo->scSize);
  
  if((uint8_t *)pScInfo->scBuf != salld_ac_tmp_scBuf)
    memcpy((void *) pScInfo->scBuf, salld_ac_tmp_scBuf, pScInfo->scSize);
  Sa_osalEndScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  
  /* Prepare the SW Info Words */
  salld_set_swInfo(SALLD_CMDL_ENGINE_SRTP_AC_HPS1, 0, NULL,
                   pScInfo, &rxInst->swInfo, 0);

  /* Store the scBuf internally as offset to suppor multiprocess */
  pScInfo->scBuf = (uintptr_t) sa_CONV_ADDR_TO_OFFSET(salldLObj.scPoolBaseAddr, pScInfo->scBuf);
  
  return (sa_ERR_OK);
}            

/****************************************************************************
 * FUNCTION PURPOSE: Air Ciphering Get SwInfo 
 ****************************************************************************
 * DESCRIPTION: Air Ciphering Get SwInfo
 *
 * int16_t  salld_ac_get_swInfo (
 *   Sa_ChanHandle        handle      - SALLD channel identifier
 *   uint16_t             dir         - packet directions
 *   Sa_SWInfo_t         *pSwInfo)    - a pointer to swInfo
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *
 ***************************************************************************/
int16_t salld_ac_get_swInfo (void *salldInst, uint16_t dir, Sa_SWInfo_t* pChanSwInfo)
{
  salldAcInst_t *inst = (salldAcInst_t *)salldInst;
  int16_t ret = sa_ERR_OK;
  
  if (dir == sa_PKT_DIR_FROM_NETWORK)
  {
    salldAcRxInst_t *rxInst = (salldAcRxInst_t *) &inst->rxInst;
    memcpy(pChanSwInfo, &rxInst->swInfo, sizeof(Sa_SWInfo_t));
    
  }
  else if (dir == sa_PKT_DIR_TO_NETWORK)
  {
    salldAcTxInst_t *txInst = (salldAcTxInst_t *) &inst->txInst;
    memcpy(pChanSwInfo, &txInst->swInfo, sizeof(Sa_SWInfo_t));
  }
  else
  {
    ret = sa_ERR_PARAMS;
  }
  
  return(ret);
}


/****************************************************************************
 * Table PURPOSE:   AC function call table 
 ****************************************************************************
 * DESCRIPTION:     The tables are used to link AC functions to the 
 *                  SALLD library if required 
 *
 ***************************************************************************/
Sa_ProtocolCallTbl_t Sa_callTblAc = 
{
  sa_PT_3GPP_AC,
  salld_ac_init,
  salld_ac_control,
  salld_ac_get_stats,
  salld_ac_send_data,
  salld_ac_receive_data,
  salld_ac_close,
  salld_ac_get_swInfo
};

#endif
   
/* Nothing past this point */

