/******************************************************************************
 * FILE PURPOSE: Secure RTCP Main File
 ******************************************************************************
 * FILE NAME: salldsrtcp.c
 *
 * DESCRIPTION: The main module for Secure RTCP Code
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
/* Standard include files */
#include <string.h>

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include "src/salldloc.h"
#include "src/salldport.h"
#include "salldsrtcp.h"
#include "salldsrtcploc.h"

#include "src/cipher/salldaes.h"

extern salldCoreCipher  salld_cipher_table[];
extern salldCoreMac     salld_mac_table[];

static srtcpFormIV srtcpFormIVTab[] = 
{
  salld_srtcp_form_ctr_iv,
  salld_srtcp_form_f8_iv
};

/****************************************************************************
 * FUNCTION PURPOSE: SRTCP Key Setup 
 ****************************************************************************
 * DESCRIPTION: Update the SRTCP channel key information based on the input
 *          parameters.
 *
 *  uint16_t salld_srtcp_setup_key(
 *            salldSrtcpInst_t*  inst            -> Point to SRTCP channel instance
 *            salldSrtcpKeyInfo_t*   pKeyInfo    -> Point to the instance key storage 
 *            salldSrtcpKeyParams_t* pKeyParams) -> Point to the key configuration
 *                                                 parameters.   
 *                       
 * Return values:  FALSE : invalid parameters
 *                 TRUE:   successful updtae         
 *
 ***************************************************************************/
static uint16_t salld_srtcp_setup_key(salldSrtcpInst_t *inst, 
                                  salldSrtcpKeyInfo_t* pKeyInfo, Sa_SrtpKeyParams_t* pKeyParams) 
{
  uint16_t ctrlBitMap = pKeyParams->ctrlBitfield;

  if (ctrlBitMap & sa_SRTP_KEY_CTRL_KEY_TYPE_FROM_TO)
  {
      salldSrtcpFromTo_t* pFromTo = &pKeyInfo->fromToBuffer;
    
      /* FromTo Key, record the parameters at the pending buffer */
      pKeyInfo->kdBitfield |= SALLD_SRTP_FROM_TO_MASK; /* set <from,to> */
    
      /* clean up the previous buffer */
      memset(pFromTo, 0, sizeof(salldSrtcpFromTo_t));

      pFromTo->kdBitfield = SALLD_SRTP_KD_RATE_MASK;
    
      if(ctrlBitMap & sa_SRTP_KEY_CTRL_MASTER_KEY)
      {
          /* Copy Master Key */
          misc_utlCopy((uint16_t *)pKeyParams->masterKey, pFromTo->masterKey, 
                       SALLD_BYTE_TO_TUINT(pKeyInfo->keySize.masterKeySize));
          pFromTo->kdBitfield |= SALLD_SRTP_NEW_MASTER_KEY_MASK;
      }
    
      if( ctrlBitMap & sa_SRTP_KEY_CTRL_MASTER_SALT)
      {
          /* Copy Master salt */
          misc_utlCopy((uint16_t *)pKeyParams->masterSalt, pFromTo->masterSalt, 
                       SALLD_BYTE_TO_TUINT(pKeyInfo->keySize.masterSaltSize));
          pFromTo->kdBitfield |= SALLD_SRTP_NEW_MASTER_SALT_MASK;
      }
    
      /* copy kdr */
      if(ctrlBitMap & sa_SRTP_KEY_CTRL_KEY_DERIVE_RATE)
      { 
          if(pKeyParams->derivRate <= 24)
          {
              pFromTo->kdBitfield &= ~SALLD_SRTP_KD_RATE_MASK; 
              pFromTo->kdBitfield |= pKeyParams->derivRate;
          }
      }
    
      /* copy from-to data */
      /* Note: only 32 bits are valid for RTCP operation */
      pFromTo->from  = (pKeyParams->fromEsnMsw << 16) | ((uint32_t)pKeyParams->fromEsnLsw);  
      pFromTo->to    = (pKeyParams->toEsnMsw << 16) | ((uint32_t)pKeyParams->toEsnLsw);

      /* all the keys are set then update the new key available field */
      if((pFromTo->kdBitfield & SALLD_SRTP_NEW_MASTER_KEY_MASK) 
          && (pFromTo->kdBitfield & SALLD_SRTP_NEW_MASTER_SALT_MASK))
      {
          pFromTo->kdBitfield ^= SALLD_SRTP_NEW_MASTER_KEY_MASK;  /* clear this */
          pFromTo->kdBitfield ^= SALLD_SRTP_NEW_MASTER_SALT_MASK; /* clear this */
          pFromTo->kdBitfield |= SALLD_SRTP_NEW_KEY_MASK;         /* set new key flag */
      }
  } /* From-To case*/
  else /* MKI or regular */
  {

      uint16_t tempBitfield = pKeyInfo->kdBitfield;
      pKeyInfo->kdBitfield= SALLD_SRTP_KD_RATE_MASK; /* Reset kd_bitfield */

      /*  MKI Active */
      if(ctrlBitMap & sa_SRTP_KEY_CTRL_MKI)
      {
          if((pKeyInfo->mkiLength) && (pKeyInfo->mkiLength != pKeyParams->mkiSize))
          {
  		      (salldLObj.callOutFuncs.DebugTrace) ((void *)inst->salldInst.ID, 
                                  sa_DBG_WARNING, sa_MSG_NO_MKI_LEN_CHANGE_ON_FLY, 0, 0);
              pKeyInfo->kdBitfield = tempBitfield; /* Restore old values */
              return FALSE;
          }
          else
          {
              pKeyInfo->kdBitfield |= SALLD_SRTP_MKI_MASK; /* set MKI */
              pKeyInfo->mkiLength = pKeyParams->mkiSize;
              pKeyInfo->mki = pKeyParams->mki;
          }
      }
    
      if(ctrlBitMap & sa_SRTP_KEY_CTRL_MASTER_KEY)
      {
          /* Copy Master Key */
          misc_utlCopy((uint16_t *)pKeyParams->masterKey, pKeyInfo->masterKey, 
                       SALLD_BYTE_TO_TUINT(pKeyInfo->keySize.masterKeySize));
          pKeyInfo->kdBitfield |= SALLD_SRTP_NEW_MASTER_KEY_MASK;
      }
    
      if( ctrlBitMap & sa_SRTP_KEY_CTRL_MASTER_SALT)
      {
          /* Copy Master salt */
          misc_utlCopy((uint16_t *)pKeyParams->masterSalt, pKeyInfo->masterSalt, 
                       SALLD_BYTE_TO_TUINT(pKeyInfo->keySize.masterSaltSize));
          pKeyInfo->kdBitfield |= SALLD_SRTP_NEW_MASTER_SALT_MASK;
      }
    
      /* copy kdr */
      if(ctrlBitMap & sa_SRTP_KEY_CTRL_KEY_DERIVE_RATE)
      { 
          if(pKeyParams->derivRate <= 24)
          {
              pKeyInfo->kdBitfield &= ~SALLD_SRTP_KD_RATE_MASK; 
              pKeyInfo->kdBitfield |= pKeyParams->derivRate;
          }
      }

      /* Max Key Lifetime */
      /* Only the lower 32-bit is used */
      if(ctrlBitMap & sa_SRTP_KEY_CTRL_KEY_LIFETIME)
      {
          pKeyInfo->keyLifetime = (pKeyParams->keyLifeTimeMsw << 16) | ((uint32_t)pKeyParams->keyLifeTimeLsw);
      }

  }

  if(ctrlBitMap & sa_SRTP_KEY_CTRL_ROC)
  {
      pKeyInfo->index = pKeyParams->roc;
  }

  if((pKeyInfo->kdBitfield & SALLD_SRTP_NEW_MASTER_KEY_MASK) 
      && (pKeyInfo->kdBitfield & SALLD_SRTP_NEW_MASTER_SALT_MASK))
  {
      pKeyInfo->kdBitfield &= ~SALLD_SRTP_KEY_EXPIRE_MASK;
      pKeyInfo->kdBitfield &= ~SALLD_SRTP_KEY_REQUEST_MASK;
      pKeyInfo->kdBitfield ^= SALLD_SRTP_NEW_MASTER_KEY_MASK;  /* clear this */
      pKeyInfo->kdBitfield ^= SALLD_SRTP_NEW_MASTER_SALT_MASK; /* clear this */
      pKeyInfo->kdBitfield |= SALLD_SRTP_NEW_KEY_MASK;         /* set new key flag */
  }
  
  return TRUE;
}            


/****************************************************************************
 * FUNCTION PURPOSE: SRTCP Control 
 ****************************************************************************
 * DESCRIPTION: SRTP Control functions
 *
 * int16_t  salld_srtcp_control (
 *   void  *salldInst  - a pointer to SALLD channel instance
 *   void  *ctrl)      - a pointer to control structure
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *
 ***************************************************************************/
int16_t salld_srtcp_control (void *salldInst, void *salldCtrl)
{

  salldSrtcpInst_t *inst = (salldSrtcpInst_t *)salldInst;
  salldSrtcpTxInst_t *txInst = (salldSrtcpTxInst_t *) &inst->txInst;
  salldSrtcpRxInst_t *rxInst = (salldSrtcpRxInst_t *) &inst->rxInst;
  Sa_ChanCtrlInfo_t *ctrl  = (Sa_ChanCtrlInfo_t *)salldCtrl;
  uint16_t bitmap;

  switch(ctrl->ctrlType)
  {
    /* SALLD cipher mac and key size selection */
    case sa_CHAN_CTRL_GEN_CONFIG:
    {
        bitmap = ctrl->ctrlInfo.gen.validBitfield;
        if( bitmap & sa_CONTROLINFO_VALID_TX_CTRL)
        {
            Sa_GenConfigParams_t*  pGenConfig =  &ctrl->ctrlInfo.gen.txCtrl;
            Sa_SrtpConfigParams_t* pSrtpConfig = &ctrl->ctrlInfo.gen.txCtrl.params.srtp;
            
            txInst->cipherMode = pGenConfig->cipherMode;
            txInst->authMode   = pGenConfig->authMode;
            txInst->keyInfo.keySize = *pSrtpConfig;
        }
        if(bitmap & sa_CONTROLINFO_VALID_RX_CTRL)
        {
            Sa_GenConfigParams_t*  pGenConfig =  &ctrl->ctrlInfo.gen.rxCtrl;
            Sa_SrtpConfigParams_t* pSrtpConfig = &ctrl->ctrlInfo.gen.rxCtrl.params.srtp;
            
            rxInst->cipherMode = pGenConfig->cipherMode;
            rxInst->authMode   = pGenConfig->authMode;
            rxInst->keyInfo.keySize = *pSrtpConfig;
        }
        
        if (bitmap & sa_CONTROLINFO_VALID_REPLAY_WIN)
        {
             rxInst->windowCheck = ctrl->ctrlInfo.gen.replayWindowSize;   
             rxInst->replayWindow.winSize = ctrl->ctrlInfo.gen.replayWindowSize; 
        }
    }    
    break;
  
    /* Master key and Salt setup for srtp */
    case sa_CHAN_CTRL_KEY_CONFIG:
    {
        bitmap = ctrl->ctrlInfo.key.ctrlBitfield;
        
        if (bitmap & sa_KEY_CONTROL_TX_KEY_VALID)
        {
            Sa_SrtpKeyParams_t* pKeyParams = &ctrl->ctrlInfo.key.txKey.srtp;
            salldSrtcpKeyInfo_t*  pKeyInfo = &txInst->keyInfo;
            
            if(!salld_srtcp_setup_key(inst, pKeyInfo, pKeyParams))
                return(sa_ERR_PARAMS);
        }    

        if (bitmap & sa_KEY_CONTROL_RX_KEY_VALID)
        {
            Sa_SrtpKeyParams_t* pKeyParams = &ctrl->ctrlInfo.key.rxKey.srtp;
            salldSrtcpKeyInfo_t*  pKeyInfo = &rxInst->keyInfo;
            
            if(!salld_srtcp_setup_key(inst, pKeyInfo, pKeyParams))
                return(sa_ERR_PARAMS);
        }    
    }   
    break;
    
    default:
        return (sa_ERR_PARAMS);
  }
  
  return (sa_ERR_OK);
}
/******************************************************************************
 * FUNCTION PURPOSE: SRTCP Get Stats
 ******************************************************************************
 * DESCRIPTION: Extract SRTCP related statistics from the instance structure
 *
 * void salld_srtcp_get_stats (
 *    void   *salldInst,       - A pointer to SALLD instance
 *    uint16_t flags,            - various control flags
 *    void   *stats)           - The stat structure
 *
 * Return values:  sa_ERR_OK
 *
 *****************************************************************************/
int16_t salld_srtcp_get_stats (void *salldInst, uint16_t flags, void *stats)
{
  Sa_SrtcpStats_t  *pStats = (Sa_SrtcpStats_t *) stats;
  salldSrtcpInst_t   *inst   = (salldSrtcpInst_t *)salldInst; 
  salldSrtcpTxInst_t *txInst = &inst->txInst;
  salldSrtcpRxInst_t *rxInst = &inst->rxInst;

  pStats->replayOld = inst->salldInst.stats.replayOld;
  pStats->replayDup = inst->salldInst.stats.replayDup;
  pStats->authFail  = inst->salldInst.stats.authFail;
  pStats->pktEnc    = txInst->packetEnc; 
  pStats->pktDec    = rxInst->packetDec; 
  pStats->txRekey   = txInst->keyInfo.nRekey - 1;  /* the initial key is not counted as releying */
  pStats->rxRekey   = rxInst->keyInfo.nRekey - 1;  /* the initial key is not counted as releying */

  if(flags & sa_STATS_QUERY_FLAG_CLEAR)
  {
    inst->salldInst.stats.replayOld = 0;
    inst->salldInst.stats.replayDup = 0;
    inst->salldInst.stats.authFail  = 0;
  }
  return (sa_ERR_OK);
}

/******************************************************************************
 * FUNCTION PURPOSE: RTCP: Update the From-To key (Local)
 ******************************************************************************
 * DESCRIPTION: Copy the pending From-To key 
 *
 *****************************************************************************/
static void srtcp_update_key(salldSrtcpKeyInfo_t* pKeyInfo)
{
  salldSrtcpFromTo_t* pFromTo = &pKeyInfo->fromToBuffer;
  salldSrtcpKeySizeParams_t*  pKeySize = &pKeyInfo->keySize;

  /* master key */
  misc_utlCopy(pFromTo->masterKey, pKeyInfo->masterKey, 
               SALLD_BYTE_TO_TUINT(pKeySize->masterKeySize));
  
  /* master salt */
  misc_utlCopy(pFromTo->masterSalt, pKeyInfo->masterSalt, 
               SALLD_BYTE_TO_TUINT(pKeySize->masterSaltSize));
  
  /* key derivation rate */
  pKeyInfo->kdBitfield &= ~SALLD_SRTP_KD_RATE_MASK; 
  pKeyInfo->kdBitfield |= (pFromTo->kdBitfield & SALLD_SRTP_KD_RATE_MASK);
  /* from-to */
  pKeyInfo->from = pFromTo->from;
  pKeyInfo->to   = pFromTo->to;
  
  /* clear the new key mask */
  pFromTo->kdBitfield &= ~SALLD_SRTP_NEW_KEY_MASK;
}

/******************************************************************************
 * FUNCTION PURPOSE: SRTCP Derive Session Key (Local)
 ******************************************************************************
 * DESCRIPTION: Derives Session Keys for SRTCP. 
 *              NOTE: It assumes that key_derivation_rate is represented in 2^i
 *              form and its value is "i" instead of 2^i to save in DIV routine
 *              So, make sure that value provided is less that 32 (i.e. less 
 *              than 2^32 )
 *
 * uint16_t srtcp_derive_keys (
 *    void   *salldInst,    - A pointer to RTCP Key Information
 *    uint16_t index  )       - SRTCP indexin
 *****************************************************************************/
static uint16_t srtcp_derive_keys(salldSrtcpKeyInfo_t* pKeyInfo, uint32_t indexin)
{
  int16_t i;
  uint32_t index[2];
  uint16_t *k_master = pKeyInfo->masterKey;
  uint16_t *pSalt = pKeyInfo->masterSalt;
  uint16_t *master_salt = pKeyInfo->masterSalt;
  uint32_t roundkey[SALLD_MAX_ROUND_KEY_SIZE_IN_TULONG];
  uint16_t x[SALLD_SRTP_MASTER_KEY_SIZE_IN_TUINT];
  tword keyIn[SALLD_SRTP_MAX_SESSION_KEY_SIZE_IN_WORD]; 
  uint16_t *session_key = pKeyInfo->sessionEncKey;
  uint16_t *session_salt = pKeyInfo->sessionSalt;
  uint16_t *pX;
  uint16_t *session_auth_key = pKeyInfo->sessionAuthKey;
  uint16_t kdr;
  uint32_t *r = &pKeyInfo->r;
  uint16_t *kdBitfield = &pKeyInfo->kdBitfield;
  salldSrtcpKeySizeParams_t*  pKeySize = &pKeyInfo->keySize;              
  int16_t nr;

  index[0] = (indexin >> 16);
  index[1] = (indexin << 16);

  kdr = pKeyInfo->kdBitfield & SALLD_SRTP_KD_RATE_MASK; /* get the key_derivation_rate */
  
  if((kdr <= 24) && !(*kdBitfield & SALLD_SRTP_FIRST_KD_MASK))
  {
  
    index[1] = indexin >> kdr;
    index[0] = 0;
    
    /* Check whether we need to derive new key or not */
    if(index[1] == *r)
    {
      return FALSE; /* no key derivation required */
    }
    
    *r = index[1];

  }
  else if(*kdBitfield & SALLD_SRTP_FIRST_KD_MASK)
  { 
  
    index[1] = indexin >> kdr;
    index[0] = 0;
  
    *r = index[1];
  
    /* Derive the first time */
    /* To derive the key, seq num in the received packet should be used */
    /* We should use 'zero' seq num only when no key derivation rate is specified */
    if(kdr > 24)
    {
      index[0] = 0;
      index[1] = 0;
    }
    (*kdBitfield) ^= SALLD_SRTP_FIRST_KD_MASK;
  }
  else 
    return FALSE; /* no new session key needed */
    
  /* New session  is required */  
  /* key_id = <label> || r */
  /* x = key_id XOR master_salt */
  pX = x;
  *pX++ = (*pSalt++);
  *pX++ = (*pSalt++);
  *pX++ = (*pSalt++);
  *pX++ = (*pSalt++) ^ SALLD_UINT16_BE(0x0003); /* <label> = 0x0003 for session encr key */
  *pX++ = (*pSalt++) ^ SALLD_UINT16_BE((uint16_t)(index[0] & 0xFFFF));
  *pX++ = (*pSalt++) ^ SALLD_UINT16_BE((uint16_t)(index[1] >>16));
  *pX++ = (*pSalt++) ^ SALLD_UINT16_BE((uint16_t)(index[1] & 0xFFFF));
  *pX++ = 0; /* 2 LSB octets are zeroed */
  
  nr = aesKeyExpandEnc((tulong *)roundkey, (tword *)k_master, pKeySize->masterKeySize<<3);

  for( i = 0; i < SALLD_SRTP_MAX_SESSION_KEY_SIZE_IN_WORD; i++)
    keyIn[i] = 0;
    
  /* derive session encryption key */
  salld_aes_ctr_single_segment(roundkey, pKeySize->masterKeySize<<3, nr, (tword *)x,  
               keyIn, (int16_t)pKeySize->sessionEncKeySize, (tword *)session_key);

  /* derive session mac key */
  x[SALLD_SRTP_MASTER_SALT_SIZE_IN_TUINT] = 0; /* Zero the 16 LSB in ctr */
  x[3] = master_salt[3] ^ SALLD_UINT16_BE(0x0004);
  salld_aes_ctr_single_segment(roundkey, pKeySize->masterKeySize<<3, nr, (tword *)x,  
               keyIn, (int16_t)pKeySize->sessionMacKeySize, (tword *)session_auth_key);

  /* derive session salt */
  x[SALLD_SRTP_MASTER_SALT_SIZE_IN_TUINT] = 0; /* Zero the 16 LSB in ctr */
  x[3] = master_salt[3] ^ SALLD_UINT16_BE(0x0005);
  salld_aes_ctr_single_segment(roundkey, pKeySize->masterKeySize<<3, nr, (tword *)x,  
               keyIn, (int16_t)SALLD_SRTP_SESSION_SALT_SIZE, (tword *)session_salt);

  return TRUE;  /* key_derivation has happend */
} /* srtcp_derive_keys */

/******************************************************************************
 * FUNCTION PURPOSE: SRTCP Key Validation
 ******************************************************************************
 * DESCRIPTION: Validate Key and derive new session key if required 
 *
 * int16_t srtcpKeysValidation (
 *****************************************************************************/
static int16_t srtcpKeysValidation(void *salldInst, salldSrtcpKeyInfo_t* pKeyInfo, uint32_t index, 
                                 Sa_KeyRequest_t* pKeyReq, uint32_t* numPkt)
{
  uint32_t time1,time2;
    
  /* Determine Master Key/Salt */
  if(!(pKeyInfo->kdBitfield & SALLD_SRTP_FROM_TO_MASK))
  {
    pKeyReq->params.srtp.ctrlBitfield |= sa_SRTP_KEY_REQUEST_KEY_TYPE_MKI;
  
    /* If new Master Key/Salt is present reset the bit and flag
       the first key_derivation */
    if(pKeyInfo->kdBitfield & SALLD_SRTP_NEW_KEY_MASK)
    {
      pKeyInfo->kdBitfield ^= SALLD_SRTP_NEW_KEY_MASK;
      
      /* Check key life time */
      if(pKeyInfo->keyLifetime == 0)
      {
        /* Key Expiry Notice */
        if(!(pKeyInfo->kdBitfield & SALLD_SRTP_KEY_EXPIRE_MASK))
        {
          pKeyInfo->kdBitfield |= SALLD_SRTP_KEY_EXPIRE_MASK;
          (salldLObj.callOutFuncs.ChanKeyRequest) ((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,salldInst), pKeyReq);
        }
        return SALLD_KEY_VALIDATION_ERROR;
      }
      pKeyInfo->kdBitfield |= SALLD_SRTP_FIRST_KD_MASK;
      *numPkt = 0;
      pKeyInfo->nRekey++;
    }
    else
    {
      /* Check key life time */
      if(*numPkt >= pKeyInfo->keyLifetime)
      {
        /* Key Expiry Notice */
        if(!(pKeyInfo->kdBitfield & SALLD_SRTP_KEY_EXPIRE_MASK))
        {
          pKeyInfo->kdBitfield |= SALLD_SRTP_KEY_EXPIRE_MASK;
          (salldLObj.callOutFuncs.ChanKeyRequest) ((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,salldInst), pKeyReq);
        }
        return SALLD_KEY_VALIDATION_ERROR;
      }
    }
  }
  else /* <from,to> case */
  {
    salldSrtcpFromTo_t* pFromTo = &pKeyInfo->fromToBuffer; 
    /* find master key based upon "index" */
    if(pFromTo->kdBitfield & SALLD_SRTP_NEW_KEY_MASK)
    {
      time1 = pFromTo->from;
      time2 = pFromTo->to;
      if((index >= time1) && (index <= time2))
      {
        srtcp_update_key(pKeyInfo);
        pKeyInfo->kdBitfield |= SALLD_SRTP_FIRST_KD_MASK;
        pKeyInfo->kdBitfield &= ~SALLD_SRTP_KEY_REQUEST_MASK;
        *numPkt = 0;
        pKeyInfo->nRekey++;
      }
      else
      {
        time1 = pKeyInfo->from;
        time2 = pKeyInfo->to;
        if((index < time1) || (index > time2))
          return SALLD_KEY_VALIDATION_ERROR;
      }
    }
    else
    {
      /* check if the key present in context is valid else ask for it */
      time1 = pKeyInfo->from;
      time2 = pKeyInfo->to;
      if(index >= (time2 - SALLD_RTP_KEY_LIFE_MARGIN))
      {
        /* Key Expiry Notice */
        if(!(pKeyInfo->kdBitfield & SALLD_SRTP_KEY_REQUEST_MASK))
        {
          pKeyInfo->kdBitfield |= SALLD_SRTP_KEY_REQUEST_MASK;
          (salldLObj.callOutFuncs.ChanKeyRequest) ((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,salldInst), pKeyReq);
        }
      }
      if((index > time2) || (index < time1))
        return SALLD_KEY_VALIDATION_ERROR;
    }
  }

  /* Derive Session Keys/Salt */
  if(srtcp_derive_keys(pKeyInfo, index))
    return SALLD_KEY_VALIDATION_NEW_KEY;
  else
    return SALLD_KEY_VALIDATION_OK;   
}
/******************************************************************************
 * FUNCTION PURPOSE: SRTCP Send Data
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_srtcp_send_data (
 *    void *salldInst,      - A pointer to SALLD SRTP instance
 *    void *pktInfo,        - packet pointer
 *    uint16_t clear)           - clear flag: no encryption
 *
 *  Description:
 *        1. Determine "index" (ROC||SEQ)
 *        2. Determine Master Key/Salt(from "index" and/or MKI).
 *        3. Derive Session Keys/Salt(from master key/salt, index
 *                                     and key_derivation_rate).
 *        4. Encrypt.
 *        5. If MKI is present append MKI to the packet.
 *        5. Authenticate.
 *
 *****************************************************************************/
int16_t salld_srtcp_send_data (void *salldInst, void *pktInfo, uint16_t clear) 
{
  int16_t retval = 0;
  salldSrtcpInst_t *inst   = (salldSrtcpInst_t *)salldInst; 
  salldSrtcpTxInst_t *txInst = &inst->txInst;
  salldSrtcpKeyInfo_t* pKeyInfo = &txInst->keyInfo;
  salldSrtcpKeySizeParams_t*  pKeySize = &pKeyInfo->keySize;
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;
  Sa_PktDesc_t* pPktDesc = &pPktInfo->pktDesc;
  uint16_t cipher = txInst->cipherMode;
  uint16_t mac = txInst->authMode;
  union {
    uint32_t temp_even;/* not needed here but precautionary to make iv even */
    tword iv[SALLD_AES_BLOCK_SIZE_IN_WORD];/* Warning: The starting address should be even for asm code */
  } ivu;
  uint32_t roundkey[SALLD_MAX_ROUND_KEY_SIZE_IN_TULONG];
  tword *pRtcp, *pMki;
  int16_t pktLen;
  int16_t mkiTagLen = 4;  //Sequence Number
  tword *tagOut;
  uint32_t e = 0; /* E bit */
  Sa_KeyRequest_t keyReq;
  int16_t result, segOffsetTmp, segOffset;
  int16_t nr;
  int16_t srtcpSegmentIndex, srtcpSegmentOffset, mkiSegmentIndex, mkiSegmentOffset, payloadOffset, i;
  uint16_t *segUsedSizes;
  salldAesDesc_t newPktDesc;
  
  /* Encrypt and Authenticate Packet(s) */
  /* assume single segemnt at this moment */
  if(pPktDesc->nSegments < 1)
    return (sa_ERR_PARAMS);
    
  payloadOffset = pPktDesc->payloadOffset;
  srtcpSegmentIndex = mkiSegmentOffset = segOffsetTmp = segOffset = i = 0;
  segUsedSizes = pPktDesc->segUsedSizes;
  
  /* DESCRIPTION: 
  ** This loop calculates the index of the segment within pPktDesc->segments
  ** containing SRTP header as well as segment offset to SRTP Header.
  **
  **  int16_t segOffsetTmp         - size of the current segment being processed. 
  **  int16_t segOffset            - total size of previous segments before
  **                                 current segment
  **  int16_t srtcpSegmentIndex    - index of segment containing srtcp header 
  **  int16_t srtcpSegmentOffset   - offset to the SRTCP header from the
  **                                 beginning of the segment
  ** */
  segOffsetTmp = segUsedSizes[srtcpSegmentIndex];
  while(segOffset + segOffsetTmp < payloadOffset)
  {
      segOffset += segOffsetTmp;
      segOffsetTmp = segUsedSizes[++srtcpSegmentIndex];
  }
  srtcpSegmentOffset = payloadOffset - segOffset;
  
  /* Sanity check for max number of segments supported */
  if(pPktDesc->nSegments - srtcpSegmentIndex > SALLD_MAX_SEGMENTS)
  {
      return (sa_ERR_PARAMS);
  }
  
  pRtcp = (tword *)pPktDesc->segments[srtcpSegmentIndex];
  pRtcp += SALLD_BYTE_TO_WORD(srtcpSegmentOffset);
  
  /* Prepare the key Request structure */
  keyReq.params.srtp.ctrlBitfield = sa_SRTP_KEY_REQUEST_TX_KEY;
  
  result = srtcpKeysValidation(salldInst, pKeyInfo, pKeyInfo->index, 
                              &keyReq, &txInst->packetEnc); 
                              
  if(result == SALLD_KEY_VALIDATION_ERROR)
    return(sa_ERR_KEY_EXPIRED);
    
  /* Encrypt and Authenticate the packet */

  /* Build segmented srtp packet descriptor for processing */
  /* DESCRTIPTION:
  ** This loop is in continuation of the previous loop. Using the same
  ** algorithm to find the beginning of the MKI, while constructing
  ** multi-segment newPktDesc for authentication and encrpytion.
  **  
  **  int16_t mkiSegmentIndex     - index of segment containing mki
  **  int16_t mkiSegmentOffset    - offset to MKI from the beginning of
  **                                the segment
  **  tword **newPktDesc.segments - array of pointers to SRTCP payload
  **                                segments
  **  int16_t *newPktDesc.inputLen- length of each SRTCP payload segment
  ** */
  pktLen = payloadOffset + pPktDesc->payloadLen;
  mkiSegmentIndex = srtcpSegmentIndex;

  while((segOffset + segOffsetTmp) < pktLen)
  {
     newPktDesc.segments[i] = (tword *) pPktDesc->segments[mkiSegmentIndex];
     newPktDesc.inputLen[i++] = segOffsetTmp;
     segOffset += segOffsetTmp;
     segOffsetTmp = segUsedSizes[++mkiSegmentIndex];
  }
  mkiSegmentOffset = pktLen - segOffset;
  
  /* Prepare and fix newPktDesc */
  newPktDesc.segments[0] = pRtcp + SALLD_BYTE_TO_WORD(8);
  if(!i)
  {
    newPktDesc.inputLen[0] = pPktDesc->payloadLen - 8;
    i++;
  }
  else
  {
    /* Take care of first segment */
    newPktDesc.inputLen[0] -= (srtcpSegmentOffset + 8);
    
    if(mkiSegmentOffset)
    {
        newPktDesc.segments[i] = (tword *) pPktDesc->segments[mkiSegmentIndex];
        newPktDesc.inputLen[i++] = mkiSegmentOffset;
    }
    
  }
  newPktDesc.nSegments = i;
  newPktDesc.pktLen = pPktDesc->payloadLen - 8;
  
  if((mkiSegmentIndex - srtcpSegmentIndex + 1) > SALLD_MAX_SEGMENTS)
    return (sa_ERR_PARAMS);
    
  /* Reserved at least 18 bytes for index, mki and authentication tag */ 
  if((pPktDesc->segAllocSizes[mkiSegmentIndex] - segUsedSizes[mkiSegmentIndex]) < 18)
    return (sa_ERR_PARAMS);

  if(cipher && (!clear)) 
  {
    /* Encryption Round Key expansion */
    nr = aesKeyExpandEnc((tulong *)roundkey, (tword *)pKeyInfo->sessionEncKey, pKeySize->sessionEncKeySize<<3);
    
    /* form the IV - Initialization Vector for the confidentiality mode */
    (*srtcpFormIVTab[cipher - sa_CipherMode_AES_CTR])((void *)pKeyInfo, pRtcp, ivu.iv, pKeyInfo->index);

	retval = 
      (*salld_cipher_table[cipher - sa_CipherMode_AES_CTR])(roundkey, (int16_t)pKeySize->sessionEncKeySize, nr,
                                                    ivu.iv, &newPktDesc);
    if(!retval)
      e = 0x80000000;
  }
  
  pMki = (tword*) pPktDesc->segments[mkiSegmentIndex] + SALLD_BYTE_TO_WORD(mkiSegmentOffset);
  pktWrite32bits_m(pMki, 0, (pKeyInfo->index | e));
  pKeyInfo->index++;
  
  tagOut = pMki + SALLD_BYTE_TO_WORD(4);

  /* If MKI is active */
  if(pKeyInfo->kdBitfield & SALLD_SRTP_MKI_MASK)
  {
      /* Support 2-byte and 4-byte MKI only */
      if (pKeyInfo->mkiLength == 2)
      {
        pktWrite16bits_m(tagOut, 0, (uint16_t)pKeyInfo->mki);
      }
      else
      {
        pktWrite32bits_m(tagOut, 0, pKeyInfo->mki);
      }
      tagOut += SALLD_BYTE_TO_WORD(pKeyInfo->mkiLength);
      mkiTagLen += pKeyInfo->mkiLength;
  }

  if(mac) 
  {
    int16_t tag_len = (int16_t)pKeySize->macSize;
    /* Add MAC code */
    newPktDesc.segments[0] -= SALLD_BYTE_TO_WORD(8);
    newPktDesc.inputLen[0] += SALLD_BYTE_TO_WORD(8);
    newPktDesc.inputLen[newPktDesc.nSegments-1] += 4;
    newPktDesc.pktLen = (int16_t)(pPktDesc->payloadLen + 4);
	
    retval |= 
    (*salld_mac_table[mac - sa_AuthMode_HMAC_MD5])(pKeyInfo->sessionAuthKey, 
    (int16_t)pKeySize->sessionMacKeySize, 
    tagOut, 
    tag_len, &newPktDesc,
    SALLD_MAC_GENERATION, NULL);
    
    /* update the packet size after adding the MAC */
    mkiTagLen += pKeySize->macSize;
  }

  if (!retval)
  {
    /* Adjust the packet length */
    pPktDesc->payloadLen += mkiTagLen;
    pPktDesc->segUsedSizes[mkiSegmentIndex] += mkiTagLen;
    pPktDesc->size += mkiTagLen;
    txInst->packetEnc++;
    return(sa_ERR_OK);
  }
  else
    return (sa_ERR_GEN);
     
} /* salld_srtcp_send_data */

/******************************************************************************
 * FUNCTION PURPOSE: SRTCP Receive Data
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_srtcp_receive_data (
 *    void *salldInst,    - A pointer to SRTP instance
 *    void *pktInfo)      - packet pointer
 * 
 *
 *****************************************************************************/
int16_t salld_srtcp_receive_data (void *salldInst, void *pktInfo) 
{
  int16_t  retval=0;
  uint16_t tagSize=0;
  salldSrtcpInst_t *inst   = (salldSrtcpInst_t *)salldInst; 
  salldSrtcpRxInst_t *rxInst =  &inst->rxInst;
  salldSrtcpKeyInfo_t* pKeyInfo = &rxInst->keyInfo;
  salldSrtcpKeySizeParams_t*  pKeySize = &pKeyInfo->keySize;
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;
  Sa_PktDesc_t* pPktDesc = &pPktInfo->pktDesc;
  uint16_t cipher = rxInst->cipherMode;
  uint16_t mac = rxInst->authMode;
  union {
    uint32_t tempEven;/* added to make iv even */
    tword iv[SALLD_AES_BLOCK_SIZE_IN_WORD];
  } ivu;
  uint32_t roundkey[SALLD_MAX_ROUND_KEY_SIZE_IN_TULONG];
  tword *pktIn;
  tword *pRtcp;
  uint32_t index;
  tword *tagIn;
  uint32_t mki;
  uint32_t e; /* E bit */
  Sa_KeyRequest_t keyReq;
  uint16_t mkiTagLen;
  int16_t result, segOffsetTmp, segOffset;
  int16_t nr, pktLen, segRem;
  int16_t srtcpSegmentIndex, mkiSegmentIndex, srtcpSegmentOffset, mkiSegmentOffset, payloadOffset, i;
  uint16_t *segUsedSizes;
  salldAesDesc_t newPktDesc;
  tword pMki[20];

  /* Encrypt and Authenticate Packet(s) */
  /* assume single segemnt at this moment */
  if(pPktDesc->nSegments < 1)
    return (sa_ERR_PARAMS);
    
  payloadOffset = pPktDesc->payloadOffset;
  srtcpSegmentIndex = mkiSegmentOffset = segOffsetTmp = i = segOffset = 0;
  segUsedSizes = pPktDesc->segUsedSizes;
  pktLen = pPktDesc->payloadLen;
  
  /* DESCRIPTION: 
  ** This loop calculates the index of the segment within pPktDesc->segments
  ** containing SRTCP header as well as segment offset to SRTCP Header.
  **
  **  int16_t segOffsetTmp        - size of the current segment being processed. 
  **  int16_t segOffset           - total size of previous segments before
  **                                current segment
  **  int16_t srtcpSegmentIndex   - index of segment containing srtp header 
  **  int16_t srtcpSegmentOffset  - offset to the SRTP header from the
  **                                beginning of the segment
  ** */
  segOffsetTmp = segUsedSizes[srtcpSegmentIndex];
  while(segOffset + segOffsetTmp < payloadOffset)
  {
      segOffset += segOffsetTmp;
      segOffsetTmp = segUsedSizes[++srtcpSegmentIndex];
  }
  srtcpSegmentOffset = payloadOffset - segOffset;

  /* Authenticate and Decrypt Packet(s) */
  pRtcp = (tword *)pPktDesc->segments[srtcpSegmentIndex];
  pRtcp += SALLD_BYTE_TO_WORD(srtcpSegmentOffset);

  if(mac) 
  {
    tagSize = pKeySize->macSize;
  }

  mkiTagLen = tagSize + pKeyInfo->mkiLength + 4; 

  /* DESCRTIPTION:
  ** This loop is in continuation of the previous loop. Using the same
  ** algorithm to find the beginning of the MKI, while constructing
  ** multi-segment newPktDesc for authentication and encrpytion.
  **  
  **  int16_t mkiSegmentIndex     - index of segment containing mki
  **  int16_t mkiSegmentOffset    - offset to MKI from the beginning of
  **                                the segment
  **  tword **newPktDesc.segments - array of pointers to SRTCP payload
  **                                segments
  **  int16_t *newPktDesc.inputLen- length of each SRTCP payload segment
  ** */
  pktLen = payloadOffset + pPktDesc->payloadLen - mkiTagLen;
  mkiSegmentIndex = srtcpSegmentIndex;
  while((segOffset + segOffsetTmp) < pktLen)
  {
      newPktDesc.segments[i] = (tword *) pPktDesc->segments[mkiSegmentIndex];
      newPktDesc.inputLen[i++] = segOffsetTmp;
      segOffset += segOffsetTmp;
      segOffsetTmp = segUsedSizes[++mkiSegmentIndex];
  }
  
  mkiSegmentOffset = pktLen - segOffset;

  /* prepare and fix packet descriptor */ 
  newPktDesc.segments[0] = pRtcp;
  if(!i)
  {
	  newPktDesc.inputLen[0] = pPktDesc->payloadLen-mkiTagLen;
	  i++;
  }
  else
  {
	  /* Take care of first segment */
      newPktDesc.inputLen[0] -= srtcpSegmentOffset;
    
      if(mkiSegmentOffset)
      {
	      newPktDesc.segments[i]   = (tword *) pPktDesc->segments[mkiSegmentIndex];
	      newPktDesc.inputLen[i++] = mkiSegmentOffset;
      }	
  }

  if( (i + 1) > SALLD_MAX_SEGMENTS)
    return (sa_ERR_PARAMS);
        
  /* Authenticate and Decrypt Packet(s) */
  pktIn = (tword*) pPktDesc->segments[mkiSegmentIndex] + SALLD_BYTE_TO_WORD(mkiSegmentOffset);
  segRem = segUsedSizes[mkiSegmentIndex] - mkiSegmentOffset;
  
  if(segRem >= mkiTagLen)
  {
      misc_utlCopy((uint16_t *)pktIn, (uint16_t *)pMki, SALLD_BYTE_TO_TUINT(mkiTagLen));
  }
  else
  {
      misc_utlCopy((uint16_t *)pktIn, (uint16_t *)pMki, SALLD_BYTE_TO_TUINT(segRem));
      misc_utlCopy((uint16_t *)pPktDesc->segments[mkiSegmentIndex+1], (uint16_t *)(pMki + SALLD_BYTE_TO_WORD(segRem)), SALLD_BYTE_TO_TUINT(mkiTagLen - segRem));
      pPktDesc->segUsedSizes[mkiSegmentIndex+1] = 0;
      pPktDesc->nSegments--;
  }
  
  // Add an extra segment for SRTCP index */
  newPktDesc.segments[i] = (tword*)pMki;
  newPktDesc.inputLen[i++] = 4;
  newPktDesc.nSegments = i;  
  newPktDesc.pktLen = pPktDesc->payloadLen - mkiTagLen + 4;
  
  /* Prepare the key Request structure */
  keyReq.params.srtp.ctrlBitfield = sa_SRTP_KEY_REQUEST_RX_KEY;

  /* MKI verification */
  if(pKeyInfo->kdBitfield & SALLD_SRTP_MKI_MASK)
  {
    /* Note:  only support 2-byte and 4-byte MKI */
    mki = (pKeyInfo->mkiLength == 2)?(uint32_t)pktRead16bits_m(pMki, 4): 
                                               pktRead32bits_m(pMki, 4); 
    
    /* Check if the mki has changed then call the key manager to give new key 
    and return */
    if(mki != pKeyInfo->mki)
    {
      if(!(pKeyInfo->kdBitfield & SALLD_SRTP_KEY_REQUEST_MASK))
      {
          pKeyInfo->kdBitfield |= SALLD_SRTP_KEY_REQUEST_MASK;
          keyReq.params.srtp.ctrlBitfield |= (sa_SRTP_KEY_REQUEST_KEY_TYPE_MKI |
                                              sa_SRTP_KEY_REQUEST_MKI_VALID);
          keyReq.params.srtp.mki = mki;                                    
          (salldLObj.callOutFuncs.ChanKeyRequest) ((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,salldInst), &keyReq);      
      }
      return sa_ERR_KEY_EXPIRED;
    }
  }

  /* Determine index from the packet */
  index = pktRead32bits_m(pMki, 0);
  e = index & 0x80000000;
  index &= 0x7FFFFFFF;
  
  result = srtcpKeysValidation(salldInst, pKeyInfo, index, 
                               &keyReq, &rxInst->packetDec); 

  if(result == SALLD_KEY_VALIDATION_ERROR)
    return(sa_ERR_KEY_EXPIRED);
                          
  /* Do Sequence Number check/Replay check */
  if(rxInst->windowCheck == 64)
  {
  
    uint8_t ret_code;
    
    ret_code = salld_replay_check(&(rxInst->replayWindow), index);
    
    switch (ret_code)
    {
        case SALLD_REPLAY_RC_DUP:
            inst->salldInst.stats.replayDup++;
            return sa_ERR_REPLAY_DUP;
            
        case SALLD_REPLAY_RC_OLD:
            inst->salldInst.stats.replayOld++;
            return sa_ERR_REPLAY_OLD;
            
        default:
            break;    
    }
  
  }
  else if(rxInst->windowCheck != 0)
  {
    return (sa_ERR_PARAMS);
  }

  if(mac) 
  {
  
    /* Do Message Authentication */
    tagIn = pMki + SALLD_BYTE_TO_WORD(pKeyInfo->mkiLength+4);
	
    retval = 
        (*salld_mac_table[mac - sa_AuthMode_HMAC_MD5])(pKeyInfo->sessionAuthKey, 
        (int16_t)pKeySize->sessionMacKeySize, tagIn, 
        tagSize, &newPktDesc,
        SALLD_MAC_AUTHENTICATION, NULL);
	
    if(retval)
    {
        inst->salldInst.stats.authFail++;
        return sa_ERR_AUTH_FAIL;
    }
  }

  /* Record the latest RTCP index */
  pKeyInfo->index = index;

  if(cipher && e) 
  {
    /* Decryption of the payload */
    /* Encryption Round Key expansion */
    nr = aesKeyExpandEnc((tulong *)roundkey, (tword *)pKeyInfo->sessionEncKey, 
                         pKeySize->sessionEncKeySize<<3);

    /* form the IV - Initialization Vector for the confidentiality mode */
    (*srtcpFormIVTab[cipher - sa_CipherMode_AES_CTR])((void *)pKeyInfo, pRtcp, ivu.iv, pKeyInfo->index);

    /* Decryption of the payload */
    newPktDesc.nSegments--;
    newPktDesc.segments[0] = pRtcp + SALLD_BYTE_TO_WORD(8);
	newPktDesc.inputLen[0] -= 8;
    newPktDesc.pktLen -= 12; /* RTCP header: 8, RTCP index : 4 */
	retval |= 
    (*salld_cipher_table[cipher - sa_CipherMode_AES_CTR])(roundkey, 
                            (int16_t)pKeySize->sessionEncKeySize, nr, ivu.iv, 
                            &newPktDesc);   /*TODO #define unencrypted 8 byte header */
  }
  
  if(!retval)
  {
    rxInst->packetDec++;
    
    /* Adjust the packet length */
    pPktDesc->payloadLen -= mkiTagLen;
    pPktDesc->segUsedSizes[mkiSegmentIndex] = mkiSegmentOffset;
    pPktDesc->size -= mkiTagLen;

    /* Update the Replay Window */
    if(rxInst->windowCheck == 64)
    {
      salld_replay_update(&(rxInst->replayWindow), index);
    }
    
    return(sa_ERR_OK);
    
  }
  else
  {
    return(sa_ERR_GEN);
  }
  
} /* salld_srtcp_receive_data */

/****************************************************************************
 * FUNCTION PURPOSE: SRTCP Get SwInfo 
 ****************************************************************************
 * DESCRIPTION: SRTCP Get SwInfo
 *
 * int16_t  salld_srtcp_get_swInfo (
 *   Sa_ChanHandle        handle      - SALLD channel identifier
 *   uint16_t             dir         - packet directions
 *   Sa_SWInfo_t         *pSwInfo)    - a pointer to swInfo
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_UNSUPPORTED
 *
 ***************************************************************************/
int16_t salld_srtcp_get_swInfo (void *salldInst, uint16_t dir, Sa_SWInfo_t* pChanSwInfo)
{
  return(sa_ERR_UNSUPPORTED);
}


/****************************************************************************
 * FUNCTION PURPOSE:   SRTP function call table 
 ****************************************************************************
 * DESCRIPTION:     The table is used to link SRTP functions to the 
 *                  SALLD library if required 
 *
 ***************************************************************************/
Sa_ProtocolCallTbl_t Sa_callTblSrtcp = 
{
  sa_PT_SRTCP,
  salld_srtcp_init,
  salld_srtcp_control,
  salld_srtcp_get_stats,
  salld_srtcp_send_data,
  salld_srtcp_receive_data,
  salld_srtcp_close,
  salld_srtcp_get_swInfo
};

   
/* Nothing past this point */
