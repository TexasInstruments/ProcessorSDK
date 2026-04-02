/******************************************************************************
 * FILE PURPOSE: Secure RTP Main File
 ******************************************************************************
 * FILE NAME: salldsrtp.c
 *
 * DESCRIPTION: The main module for Secure RTP Code
 *
 * (C) Copyright 2009-2013, Texas Instruments Inc.
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
#include "src/cipher/salldaes.h"
#include "salldsrtp.h"
#include "salldsrtploc.h"

extern salldCoreCipher  salld_cipher_table[];
extern salldCoreMac     salld_mac_table[];

static srtpFormIV srtpFormIVTab[] = 
{
  salld_srtp_form_ctr_iv,
  salld_srtp_form_f8_iv
};

/* Defined for testing. Should be moved to makedefs.mk */

#define srtpGetChID(mid_chnum)  ( (((uint16_t)(mid_chnum)) & 0x00FF )-1)

/****************************************************************************
 * FUNCTION PURPOSE: SRTP Key Setup 
 ****************************************************************************
 * DESCRIPTION: Update the SRTP channel key information based on the input
 *          parameters.
 *
 *  uint16_t salld_srtp_setup_key(
 *            salldSrtpInst_t*  inst           -> Point to SRTP channel instance
 *            salldSrtpKeyInfo_t*   pKeyInfo   ->pointer to the instance key storage 
 *            Sa_SrtpKeyParams_t* pKeyParams) ->pointer to the key configuration
 *                                               parameters.   
 *                       
 * Return values:  FALSE : invalid parameters
 *                 TRUE:   successful updtae         
 *
 ***************************************************************************/
static uint16_t salld_srtp_setup_key(salldSrtpInst_t *inst, 
                                 salldSrtpKeyInfo_t* pKeyInfo, Sa_SrtpKeyParams_t* pKeyParams) 
{
  uint16_t ctrlBitMap = pKeyParams->ctrlBitfield;

  if (ctrlBitMap & sa_SRTP_KEY_CTRL_KEY_TYPE_FROM_TO)
  {
      salldSrtpFromTo_t* pFromTo = &pKeyInfo->fromToBuffer;
    
      /* FromTo Key, record the parameters at the pending buffer */
      pKeyInfo->kdBitfield |= SALLD_SRTP_FROM_TO_MASK; /* set <from,to> */
    
      /* clean up the previous buffer */
      memset(pFromTo, 0, sizeof(salldSrtpFromTo_t));

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
      pFromTo->fromMsb =  pKeyParams->fromEsnMsw;
      pFromTo->fromLsb =  pKeyParams->fromEsnLsw;
      pFromTo->toMsb   =  pKeyParams->toEsnMsw;
      pFromTo->toLsb   =  pKeyParams->toEsnLsw;

      /* all the keys are set then update the new key available field */
      if((pFromTo->kdBitfield & SALLD_SRTP_NEW_MASTER_KEY_MASK) 
          && (pFromTo->kdBitfield & SALLD_SRTP_NEW_MASTER_SALT_MASK))
      {
          pFromTo->kdBitfield ^= SALLD_SRTP_NEW_MASTER_KEY_MASK;  /* clear this */
          pFromTo->kdBitfield ^= SALLD_SRTP_NEW_MASTER_SALT_MASK; /* clear this */
          pFromTo->kdBitfield |= SALLD_SRTP_NEW_KEY_MASK;         /* set new key flag */
      }
  }
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
      if(ctrlBitMap & sa_SRTP_KEY_CTRL_KEY_LIFETIME)
      {
          pKeyInfo->keyLifetimeMsb = pKeyParams->keyLifeTimeMsw;
          pKeyInfo->keyLifetimeLsb = pKeyParams->keyLifeTimeLsw;
      }

  }

  if(ctrlBitMap & sa_SRTP_KEY_CTRL_ROC)
  {
      pKeyInfo->roc = pKeyParams->roc;
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
 * FUNCTION PURPOSE: Verify SRTP Configuration Parameters 
 ****************************************************************************
 * DESCRIPTION: Verify SRTP general configuration parameters for consistency
 *              with the encryption and authentication mode
 *
 *  uint16_t salld_ipsec_verify_config_params(
 *            Sa_CipherMode_e cipherMode,           -> Ciphering mode
 *            Sa_AuthMode_e   authMode,             -> Aurthentication Mode
 *            Sa_SrtpConfigParams_t* pSrtpConfig)   ->pointer to the SRTP configuration
 *                                                    parameters.   
 *                       
 * Return values:  FALSE : invalid parameters
 *                 TRUE:   valid parameters          
 *
 ***************************************************************************/
static uint16_t salld_srtp_verify_config_params(Sa_CipherMode_e cipherMode, Sa_AuthMode_e authMode,
                                                Sa_SrtpConfigParams_t* pSrtpConfig)
{
    /* Common Check */
    if((pSrtpConfig->masterKeySize != SALLD_SRTP_MASTER_KEY_SIZE)       ||
       (pSrtpConfig->masterSaltSize != SALLD_SRTP_MASTER_SALT_SIZE)     ||
       (pSrtpConfig->sessionEncKeySize != SALLD_SRTP_CIPHER_KEY_SIZE)   ||
       (pSrtpConfig->sessionSaltSize != SALLD_SRTP_SESSION_SALT_SIZE)) 
        return (FALSE);
        
    if((authMode != sa_AuthMode_NULL) &&
       ((pSrtpConfig->macSize != 4) &&
        (pSrtpConfig->macSize != 10)))
        return (FALSE);         
           
    /* Ciphering mode specific check */
    if((cipherMode != sa_CipherMode_AES_CTR) &&
       (cipherMode != sa_CipherMode_AES_F8)  &&
       (cipherMode != sa_CipherMode_NULL))
        return(FALSE);
           
    /* Encryption mode specific check */
    switch (authMode)
    {
        case sa_AuthMode_HMAC_MD5:
            if(pSrtpConfig->sessionMacKeySize != 16)
                return (FALSE);
            break;
            
        case sa_AuthMode_HMAC_SHA1:
            if(pSrtpConfig->sessionMacKeySize != 20)
                return(FALSE);
            break;
            
        case sa_AuthMode_NULL:
            break;
            
        default:
            return(FALSE);
    }
    

  return TRUE;
}            
     

/****************************************************************************
 * FUNCTION PURPOSE: SRTP Control 
 ****************************************************************************
 * DESCRIPTION: SRTP Control functions
 *
 * int16_t  salld_srtp_control (
 *   void  *salldInst  - a pointer to SALLD channel instance
 *   void  *ctrl)      - a pointer to control structure
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *
 ***************************************************************************/
int16_t salld_srtp_control (void *salldInst, void *salldCtrl)
{
  salldSrtpInst_t *inst = (salldSrtpInst_t *)salldInst;
  salldSrtpTxInst_t *txInst = (salldSrtpTxInst_t *) &inst->txInst;
  salldSrtpRxInst_t *rxInst = (salldSrtpRxInst_t *) &inst->rxInst;
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
            Sa_GenConfigParams_t*  pGenConfig =  &ctrl->ctrlInfo.gen.txCtrl;
            Sa_SrtpConfigParams_t* pSrtpConfig = &ctrl->ctrlInfo.gen.txCtrl.params.srtp;
            
            if(salld_srtp_verify_config_params(pGenConfig->cipherMode, pGenConfig->authMode, pSrtpConfig))
            {
            txInst->cipherMode = pGenConfig->cipherMode;
            txInst->authMode   = pGenConfig->authMode;
            txInst->destInfo   = pGenConfig->destInfo; 
            txInst->keyInfo.keySize = *pSrtpConfig;
        }
            else
            {
                return (sa_ERR_PARAMS);
            }
        }
        if(bitmap & sa_CONTROLINFO_VALID_RX_CTRL)
        {
            Sa_GenConfigParams_t*  pGenConfig =  &ctrl->ctrlInfo.gen.rxCtrl;
            Sa_SrtpConfigParams_t* pSrtpConfig = &ctrl->ctrlInfo.gen.rxCtrl.params.srtp;
            
            if(salld_srtp_verify_config_params(pGenConfig->cipherMode, pGenConfig->authMode, pSrtpConfig))
            {
            rxInst->cipherMode = pGenConfig->cipherMode;
            rxInst->authMode   = pGenConfig->authMode;
            rxInst->destInfo   = pGenConfig->destInfo; 
            rxInst->keyInfo.keySize = *pSrtpConfig;
        }
            else
            {
                return (sa_ERR_PARAMS);
            }
        }
        
        if (bitmap & sa_CONTROLINFO_VALID_REPLAY_WIN)
        {
             rxInst->windowCheck = ctrl->ctrlInfo.gen.replayWindowSize; 
             rxInst->replayWindow.winSize = ctrl->ctrlInfo.gen.replayWindowSize;   
        }
        
        /*
         * Is it time to form and register security context?
         */
        if(!SALLD_TEST_STATE_TX_SC_VALID(&inst->salldInst) && 
            SALLD_TEST_STATE_TX_ON(&inst->salldInst))
        {
            if ((ret = salld_srtp_set_tx_sc(inst, FALSE)) != sa_ERR_OK)
            {
                return(ret);
            }
            
            SALLD_SET_STATE_TX_SC_VALID(&inst->salldInst, 1);
        } 
        
        if(!SALLD_TEST_STATE_RX_SC_VALID(&inst->salldInst) && 
            SALLD_TEST_STATE_RX_ON(&inst->salldInst))
        {
            if ((ret = salld_srtp_set_rx_sc(inst, FALSE)) != sa_ERR_OK)
            {
                return(ret);
            }
            
            SALLD_SET_STATE_RX_SC_VALID(&inst->salldInst, 1);
            
            /* Register the receive security context */
            salldLObj.callOutFuncs.ChanRegister((Sa_ChanHandle) sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,salldInst), &inst->rxInst.swInfo);
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
            salldSrtpKeyInfo_t* pKeyInfo = &txInst->keyInfo;
            
            if(!salld_srtp_setup_key(inst, pKeyInfo, pKeyParams))
                return(sa_ERR_PARAMS);
        }    

        if (bitmap & sa_KEY_CONTROL_RX_KEY_VALID)
        {
            Sa_SrtpKeyParams_t* pKeyParams = &ctrl->ctrlInfo.key.rxKey.srtp;
            salldSrtpKeyInfo_t* pKeyInfo = &rxInst->keyInfo;
            
            if(!salld_srtp_setup_key(inst, pKeyInfo, pKeyParams))
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
 * FUNCTION PURPOSE: SRTP Get Stats
 ******************************************************************************
 * DESCRIPTION: Extract SRTP related statistics from the instance structure
 *
 * void salld_srtp_get_stats (
 *    void   *salldInst,       - A pointer to SALLD instance
 *    uint16_t flags,            - various control flags
 *    void   *stats)           - The stat structure
 *
 * Return values:  sa_ERR_OK
 *
 *****************************************************************************/
int16_t salld_srtp_get_stats (void *salldInst, uint16_t flags, void *stats)
{
  Sa_SrtpStats_t  *pStats = (Sa_SrtpStats_t *) stats;
  salldSrtpInst_t   *inst   = (salldSrtpInst_t *)salldInst; 
  salldSrtpTxInst_t *txInst = (salldSrtpTxInst_t *) &inst->txInst;
  salldSrtpRxInst_t *rxInst = (salldSrtpRxInst_t *) &inst->rxInst;

  pStats->replayOld = inst->salldInst.stats.replayOld;
  pStats->replayDup = inst->salldInst.stats.replayDup;
  pStats->authFail = inst->salldInst.stats.authFail;
  pStats->pktEncHi  = txInst->packetEncMsw;
  pStats->pktEncLo  = txInst->packetEncLsw;
  pStats->pktDecHi  = rxInst->packetDecMsw;
  pStats->pktDecLo  = rxInst->packetDecLsw;
  pStats->txROC     = txInst->keyInfo.roc;
  pStats->rxROC     = rxInst->keyInfo.roc;
  pStats->txRekey   = txInst->keyInfo.nRekey;  
  pStats->rxRekey   = rxInst->keyInfo.nRekey;

  if(flags & sa_STATS_QUERY_FLAG_CLEAR)
  {
    inst->salldInst.stats.replayOld = 0;
    inst->salldInst.stats.replayDup = 0;
    inst->salldInst.stats.authFail = 0;
  }
  return (sa_ERR_OK);
}

/******************************************************************************
 * FUNCTION PURPOSE: Update the From-To key (Local)
 ******************************************************************************
 * DESCRIPTION: Copy the pending From-To key 
 *
 *****************************************************************************/
void srtp_update_key(salldSrtpKeyInfo_t* pKeyInfo)
{
  salldSrtpFromTo_t* pFromTo = &pKeyInfo->fromToBuffer;
  salldSrtpKeySizeParams_t*  pKeySize = &pKeyInfo->keySize;

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
  pKeyInfo->fromMsb = pFromTo->fromMsb;
  pKeyInfo->fromLsb = pFromTo->fromLsb;
  pKeyInfo->toMsb = pFromTo->toMsb;
  pKeyInfo->toLsb = pFromTo->toLsb;
  
  /* clear the new key mask */
  pFromTo->kdBitfield &= ~SALLD_SRTP_NEW_KEY_MASK;
}

/******************************************************************************
 * FUNCTION PURPOSE: SRTP Derive Session Key (Local)
 ******************************************************************************
 * DESCRIPTION: Derives Session Keys for SRTP. 
 *              NOTE: It assumes that key_derivation_rate is represented in 2^i
 *              form and its value is "i" instead of 2^i to save in DIV routine
 *              So, make sure that value provided is less that 32 (i.e. less 
 *              than 2^32 )
 *
 * uint16_t salld_srtp_derive_keys (
 *    void  *salldInst,    - A pointer to SALLD instance
 *    uint16_t seq,          - SRTP Pkt Sequence Number
 *    uint16_t dir)          - SALLD_TX/SALLD_RX
 *****************************************************************************/
uint16_t salld_srtp_derive_keys(salldSrtpKeyInfo_t* pKeyInfo, uint16_t seq, uint32_t roc)
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
    uint16_t *r_msb = &pKeyInfo->rMsb;
    uint32_t *r_lsb = &pKeyInfo->rLsb;
    uint16_t *kdBitfield = &pKeyInfo->kdBitfield;
    salldSrtpKeySizeParams_t*  pKeySize = &pKeyInfo->keySize;
    int16_t nr;

    index[0] = roc;

    kdr = pKeyInfo->kdBitfield & SALLD_SRTP_KD_RATE_MASK; /* get the key_derivation_rate */
  
    if((kdr <= 24) && !(*kdBitfield & SALLD_SRTP_FIRST_KD_MASK))
    {
        /*
         * r = (roc || seq_num) >> key_deriv_rate;   
         */
        if (kdr >= 16)
        {
            index[1] = roc >> (kdr - 16);
            index[0] = 0;
        }
        else
        {
            index[1] = ((uint32_t)seq >> kdr) |
                        (roc << (16 -kdr));
            index[0] = roc >> (kdr + 16);           
        }
   
        /* Check whether we need to derive new key or not */
        if((index[1] == *r_lsb) && ((uint16_t)index[0] == *r_msb))
        {
            return FALSE; /* no key derivation required */
        }

        /* Else Derive new session key and update the index DIV key_derivation_rate */
        *r_lsb = index[1];
        *r_msb = (uint16_t) index[0];
    }
    else if(*kdBitfield & SALLD_SRTP_FIRST_KD_MASK)
    {   /* the very first key derivation for the master key */

        /* Derive the first time */
        /* To derive the key, seq num in the received packet should be used */
        /* We should use 'zero' seq num only when no key derivation rate is specified */
        if(kdr > 24)
        {
            index[0] = 0;
            index[1] = 0;
        }
        /*
        *   r = (roc || seq_num) >> key_deriv_rate;   
        */
        else if (kdr >= 16)
        {
            index[1] = roc >> (kdr - 16);
            index[0] = 0;
        }
        else
        {
            index[1] = ((uint32_t)seq >> kdr) |
                        (roc << (16 -kdr));
            index[0] = roc >> (kdr + 16);           
      
        }

        *r_lsb = index[1];
        *r_msb = (uint16_t) index[0];

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
    *pX++ = (*pSalt++); /* <label> = 0x0000 for session encr key */
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
    x[3] = master_salt[3] ^ SALLD_UINT16_BE(0x0001);
    salld_aes_ctr_single_segment(roundkey, pKeySize->masterKeySize<<3, nr, (tword *)x,  
                keyIn, (int16_t)pKeySize->sessionMacKeySize, (tword *)session_auth_key);

    /* derive session salt */
    x[SALLD_SRTP_MASTER_SALT_SIZE_IN_TUINT] = 0; /* Zero the 16 LSB in ctr */
    x[3] = master_salt[3] ^ SALLD_UINT16_BE(0x0002);
    salld_aes_ctr_single_segment(roundkey, pKeySize->masterKeySize<<3, nr, (tword *)x,  
                keyIn, (int16_t)SALLD_SRTP_SESSION_SALT_SIZE, (tword *)session_salt);

    return TRUE;  /* key_derivation has happend */
} /* salld_srtp_derive_keys */

/******************************************************************************
 * FUNCTION PURPOSE: Get ROC
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void srtp_get_roc (uint16_t seq, msuRxInst_t *rxInst)
 * 
 * Updates lastSeqNum also   
 *****************************************************************************/
static uint32_t srtp_get_roc(uint16_t seq, salldSrtpRxInst_t *rxInst, uint16_t *status)
{
  uint32_t v;
  if ((int32_t)(rxInst->lastSeqNum) < 0x0008000L)
  {
    if ( (((int32_t)seq - (int32_t)rxInst->lastSeqNum) > 0x0008000L) 
                            && !(rxInst->packetDecMsw == 0)
                            && !(rxInst->packetDecLsw == 0))
    {
      /*set v to (ROC-1) */
      v = rxInst->keyInfo.roc - 1;
      *status = SALLD_ROC_MINUS_1;
    }
    else
    {
      v = rxInst->keyInfo.roc;
      *status = SALLD_ROC_NOCHANGE;
    }
  }
  else
  {
    if (((int32_t)rxInst->lastSeqNum - (int32_t)(0x0008000L)) > (int32_t)seq)
    {
      /*    set v to (ROC+1) */
      v = rxInst->keyInfo.roc + 1;
      *status = SALLD_ROC_PLUS_1;
    }
    else
    {
      v = rxInst->keyInfo.roc;
      *status = SALLD_ROC_NOCHANGE;
    }
  }
  return v;
}

/******************************************************************************
 * FUNCTION PURPOSE: SRTP Key Validation
 ******************************************************************************
 * DESCRIPTION: Validate Key and derive new session key if required 
 *
 * int16_t srtp_key_validation (
 *****************************************************************************/
int16_t srtp_key_validation(void *salldInst, salldSrtpKeyInfo_t* pKeyInfo, uint16_t seq, uint32_t roc, 
                          Sa_KeyRequest_t* pKeyReq, uint32_t* numPktLsw, uint16_t* numPktMsw)
{

  uint64_t index, time1,time2;
  uint16_t kdr;
  uint16_t newKeyNow = FALSE;
   
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
      if((pKeyInfo->keyLifetimeMsb == 0)
        && (pKeyInfo->keyLifetimeLsb == 0))
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
      *numPktMsw = 0;
      *numPktLsw = 0;
      pKeyInfo->nRekey++;
      newKeyNow = TRUE;
    }
    else
    {
      /* Check key life time */
      if((*numPktMsw == pKeyInfo->keyLifetimeMsb)
        && (*numPktLsw == pKeyInfo->keyLifetimeLsb))
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
    salldSrtpFromTo_t* pFromTo = &pKeyInfo->fromToBuffer; 
    /* find master key based upon "index" */
    
    index = (roc << 16) ^ (uint32_t)seq;
    if(pFromTo->kdBitfield & SALLD_SRTP_NEW_KEY_MASK)
    {
      time1 = (pFromTo->fromMsb << 16) ^ (uint32_t)pFromTo->fromLsb;
      time2 = (pFromTo->toMsb << 16)  ^ (uint32_t)pFromTo->toLsb;
      if((index >= time1) && (index <= time2))
      {
        srtp_update_key(pKeyInfo);
        pKeyInfo->kdBitfield |= SALLD_SRTP_FIRST_KD_MASK;
        pKeyInfo->kdBitfield &= ~SALLD_SRTP_KEY_REQUEST_MASK;
        *numPktMsw = 0;
        *numPktLsw = 0;
        pKeyInfo->nRekey++;
        newKeyNow = TRUE;
        pKeyInfo->seqNumBase = time1;
      }
      else
      {
        time1 = (pKeyInfo->fromMsb << 16) ^ (uint32_t)pKeyInfo->fromLsb;
        time2 = (pKeyInfo->toMsb << 16) ^ (uint32_t)pKeyInfo->toLsb;
        if((index < time1) || (index > time2))
          return SALLD_KEY_VALIDATION_ERROR;
      }
    }
    else
    {
      /* check if the key present in context is valid else ask for it */
      time1 = (pKeyInfo->fromMsb << 16) ^ (uint32_t)pKeyInfo->fromLsb;
      time2 = (pKeyInfo->toMsb << 16) ^ (uint32_t)pKeyInfo->toLsb;
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
  if (salld_srtp_derive_keys(pKeyInfo, seq, roc))
  {
    if (!newKeyNow)
    {
        /* session key transition */
        kdr = pKeyInfo->kdBitfield & SALLD_SRTP_KD_RATE_MASK; /* get the key_derivation_rate */
        /* Derive new session key */
        pKeyInfo->seqNumBase = ((roc & 0xffff) << 16) + (uint32_t)seq;
        pKeyInfo->seqNumBase &= ~((1UL << kdr) - 1);
        
        return(SALLD_KEY_VALIDATION_NEW_KEY);    
    }
    else
    {
        return (SALLD_KEY_VALIDATION_NEW_KEY_NOW);
    }
  }  
  else
    return SALLD_KEY_VALIDATION_OK;   
}


/******************************************************************************
 * FUNCTION PURPOSE: SRTP Key Validation short version
 ******************************************************************************
 * DESCRIPTION: This function is only called at the receive_data_update() to
 *  generate the new key request in From-To key mode 
 *
 *****************************************************************************/
void srtp_key_validation_short(void *salldInst, salldSrtpKeyInfo_t* pKeyInfo, uint16_t seq, uint32_t roc, 
                                Sa_KeyRequest_t* pKeyReq)
{
  uint64_t index, time2;
    
  /* Determine Master Key/Salt */
  if((pKeyInfo->kdBitfield & SALLD_SRTP_FROM_TO_MASK))
  {
    /* <from,to> case */
    index = (roc << 16) ^ (uint32_t)seq;
    
    /* check if the key present in context is valid else ask for it */
    time2 = (pKeyInfo->toMsb << 16) ^ (uint32_t)pKeyInfo->toLsb;
    if(index >= (time2 - SALLD_RTP_KEY_LIFE_MARGIN))
    {
        /* Key Expiry Notice */
        if(!(pKeyInfo->kdBitfield & SALLD_SRTP_KEY_REQUEST_MASK))
        {
          pKeyInfo->kdBitfield |= SALLD_SRTP_KEY_REQUEST_MASK;
          (salldLObj.callOutFuncs.ChanKeyRequest) ((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,salldInst), pKeyReq);
        }
    }
  }
}


/******************************************************************************
 * FUNCTION PURPOSE: SRTP Send Data
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_srtp_send_data (
 *    void *salldInst,      - A pointer to SALLD SRTP instance
 *    void *pktInfo,        - packet pointer
 *    uint16_t clear)        
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
int16_t salld_srtp_send_data (void *salldInst, void *pktInfo, uint16_t clear) 
{
  int16_t retval=0;
  uint16_t seq;
  salldSrtpInst_t *inst   = (salldSrtpInst_t *)salldInst; 
  salldSrtpTxInst_t *txInst = (salldSrtpTxInst_t *) &inst->txInst;
  salldSrtpKeyInfo_t* pKeyInfo = &txInst->keyInfo;
  salldSrtpKeySizeParams_t*  pKeySize = &pKeyInfo->keySize;
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;
  Sa_PktDesc_t* pPktDesc = &pPktInfo->pktDesc;
  uint16_t cipher = txInst->cipherMode;
  uint16_t mac = txInst->authMode;
  uint32_t roundkey[SALLD_MAX_ROUND_KEY_SIZE_IN_TULONG];
  union {
    uint32_t tempEven;/* not needed here but precautionary to make iv even */
    tword iv[SALLD_AES_BLOCK_SIZE_IN_WORD];/* Warning: The starting address should be even for asm code */
  } ivu;
  tword *pktIn;
  tword *pRtp;
  int16_t pktLen;
  int16_t headerSize;
  int16_t mkiTagLen = 0;
  tword *pMki;
  uint16_t nr;
  Sa_KeyRequest_t keyReq;
    int16_t result, segOffsetTmp, segOffset;
    int16_t srtpSegmentIndex, mkiSegmentIndex, srtpSegmentOffset, mkiSegmentOffset, payloadOffset, i;
    uint16_t *segUsedSizes;
    salldAesDesc_t newPktDesc;

  /* Encrypt and Authenticate Packet(s) */
    /* Sanity Check */
    if(pPktDesc->nSegments < 1)
    return (sa_ERR_PARAMS);
  
    payloadOffset = pPktDesc->payloadOffset;
    srtpSegmentIndex = mkiSegmentIndex = mkiSegmentOffset = segOffsetTmp = i = segOffset = 0;
    segUsedSizes = pPktDesc->segUsedSizes;

    /* DESCRIPTION: 
    ** This loop calculates the index of the segment within pPktDesc->segments
    ** containing SRTP header as well as segment offset to SRTP Header.

    **
    **  int16_t segOffsetTmp        -   size of the current segment being processed. 
    **  int16_t segOffset           -   total size of previous segments before
    **                                  current segment
    **  int16_t srtpSegmentIndex    -   index of segment containing srtp header 
    **  int16_t srtpSegmentOffset   -   offset to the SRTP header from the
    **                                  beginning of the segment
    ** */
    segOffsetTmp = segUsedSizes[srtpSegmentIndex];
    while(segOffset + segOffsetTmp < payloadOffset)
    {
        segOffset += segOffsetTmp;
        segOffsetTmp = segUsedSizes[++srtpSegmentIndex];
    }
    srtpSegmentOffset = payloadOffset - segOffset;
/* Sanity check for max number of segments supported */
    if(pPktDesc->nSegments - srtpSegmentIndex > SALLD_MAX_SEGMENTS)

    {
        return (sa_ERR_PARAMS);
    }

	
    pRtp = (tword *)pPktDesc->segments[srtpSegmentIndex];
    pRtp += SALLD_BYTE_TO_WORD(srtpSegmentOffset);
    pktIn = pRtp;
	
    
  /* Determine current "index" (ROC||SEQ) */
  seq = pktRead16bits_m(pktIn, 2); /* get SEQ from the RTP packet */
  if(seq < txInst->lastSeqNum)
  {
    pKeyInfo->roc++;  /* we don't worry if roc rollovers? */
  }
  txInst->lastSeqNum = seq; /* assume tx will never have old packet */

  /* Prepare the key Request structure */
  keyReq.params.srtp.ctrlBitfield = sa_SRTP_KEY_REQUEST_TX_KEY;
  
  result = srtp_key_validation(salldInst, pKeyInfo, seq, pKeyInfo->roc, 
                               &keyReq, &txInst->packetEncLsw, 
                               &txInst->packetEncMsw);
                              
  if(result == SALLD_KEY_VALIDATION_ERROR)
    return(sa_ERR_KEY_EXPIRED);
    
  if((result == SALLD_KEY_VALIDATION_NEW_KEY) ||
     (result == SALLD_KEY_VALIDATION_NEW_KEY_NOW))
  {
    txInst->rekeyState = SRTP_REKEY_STATE_NEW_SC;
  }                              
  mkiSegmentIndex = srtpSegmentIndex;
  if(mac){
      mkiTagLen = pKeySize->macSize;
  }
  mkiTagLen += pKeyInfo->mkiLength;

  while(segOffset + segOffsetTmp < payloadOffset+pPktDesc->payloadLen)
  {
		  segOffset += segOffsetTmp;
		  segOffsetTmp = segUsedSizes[++mkiSegmentIndex];
  }
  
  if (inst->salldInst.stateBitfield & sa_CONTROLINFO_CTRL_SW_ONLY)
  {
     /* Encrypt and Authenticate the packet */
     headerSize = SALLD_RTP_HEADER_SIZE_IN_BYTE + 4*((pktRead16bits_m(pktIn, 0) & 0x0F00)>>8);
     pktIn += SALLD_BYTE_TO_WORD(headerSize);
     pktLen = pPktDesc->payloadLen - headerSize;
  
		/* Build segmented srtp packet descriptor for processing */
        newPktDesc.nSegments = 0;
        i = 0;

        /* DESCRTIPTION:
        ** This loop is in continuation of the previous loop. Using the same
        ** algorithm to find the beginning of the MKI, while constructing
        ** multi-segment newPktDesc for authentication and encrpytion.
        **  
        **  int16_t mkiSegmentIndex     -   index of segment containing mki
        **  int16_t mkiSegmentOffset    -   offset to MKI from the beginning of
        **                                  the segment
        **  tword **newPktDesc.segments -   array of pointers to SRTP payload
        **                                  segments
        **  int16_t *newPktDesc.inputLen-   length of each SRTP payload segment
        ** */
        mkiSegmentIndex = srtpSegmentIndex;
	
        while(segOffset + segOffsetTmp < payloadOffset+headerSize+pktLen - mkiTagLen)
		{
			newPktDesc.segments[i] = (tword *) pPktDesc->segments[mkiSegmentIndex];
			newPktDesc.inputLen[i++] = segOffsetTmp;
			segOffset += segOffsetTmp;
			segOffsetTmp = segUsedSizes[++mkiSegmentIndex];
		}
		
		if(!i)
		{
			newPktDesc.segments[0] = pktIn;
			newPktDesc.inputLen[0] = pktLen;
			newPktDesc.nSegments = 1;
		}
		else
		{
			newPktDesc.nSegments = i;
					/* Take care of first segment */
		segOffsetTmp = srtpSegmentOffset + headerSize;
		newPktDesc.segments[0] += SALLD_BYTE_TO_WORD(segOffsetTmp);
		newPktDesc.inputLen[0] -= segOffsetTmp;
		}
		
       
		mkiSegmentOffset = payloadOffset + headerSize + pktLen - segOffset;
		if(mkiSegmentOffset)
		{
			newPktDesc.segments[i] = (tword *) pPktDesc->segments[mkiSegmentIndex];
			newPktDesc.inputLen[i] = mkiSegmentOffset;
		}
		
		if(mkiSegmentIndex - srtpSegmentIndex > SALLD_MAX_SEGMENTS)
			return (sa_ERR_PARAMS);
		
     if(cipher) 
     {
         #if 0
         if(keyexp) /* if new session keys then expand it fresh */
         {
           /* Encryption Round Key expansion */
           nr = aesKeyExpandEnc((tulong *)roundkey, (tword *)pKeyInfo->sessionEncKey, pKeySize->sessionEncKeySize<<3);
           keyexp = FALSE;
         }
         else
         {                                  
             nr = salld_aes_get_nr(pKeySize->sessionEncKeySize<<3);  
         }
         #else
         /* Encryption Round Key expansion */
         nr = aesKeyExpandEnc((tulong *)roundkey, (tword *)pKeyInfo->sessionEncKey, pKeySize->sessionEncKeySize<<3);
         #endif

         /* form the IV - Initialization Vector for the confidentiality mode */
         (*srtpFormIVTab[cipher - sa_CipherMode_AES_CTR])((void *)pKeyInfo, pRtp, ivu.iv, pKeyInfo->roc);

			//bug
            //pktIn += SALLD_BYTE_TO_WORD(headerSize);
			newPktDesc.pktLen = pktLen;
         /* Encryption of the payload */
         retval |= 
                (*salld_cipher_table[cipher - sa_CipherMode_AES_CTR])(roundkey, 
				(int16_t)pKeySize->sessionEncKeySize, nr, ivu.iv, 
				&newPktDesc);
     }
     else
     {
         pktIn += SALLD_BYTE_TO_WORD(headerSize);
     }
  
        pMki = (tword*) pPktDesc->segments[mkiSegmentIndex] + SALLD_BYTE_TO_WORD(mkiSegmentOffset);

     if(mac) 
     {
         int16_t tag_len = (int16_t)pKeySize->macSize;

         /* append the roc at the end of the packet before calling MAC */
         pktWrite32bits_m(pMki, 0, pKeyInfo->roc);
    
         /* Get MAC code in tagOut */
            /*
         retval |= 
         (*salld_mac_table[mac - sa_AuthMode_HMAC_MD5])(pKeyInfo->sessionAuthKey, 
         (int16_t)pKeySize->sessionMacKeySize, 
         pMki + SALLD_BYTE_TO_WORD(pKeyInfo->mkiLength), 
         tag_len, pRtp,
         (int16_t)(pPktDesc->payloadLen + 4),
         SALLD_MAC_GENERATION, NULL);
			*/
			
			newPktDesc.segments[0] -= SALLD_BYTE_TO_WORD(headerSize);
			newPktDesc.inputLen[0] += SALLD_BYTE_TO_WORD(headerSize);
			newPktDesc.inputLen[newPktDesc.nSegments-1] += 4;
			newPktDesc.pktLen = (int16_t)(pPktDesc->payloadLen + 4);
			
			retval |= 
                (*salld_mac_table[mac - sa_AuthMode_HMAC_MD5])(pKeyInfo->sessionAuthKey, 
                        (int16_t)pKeySize->sessionMacKeySize, 
                        pMki + SALLD_BYTE_TO_WORD(pKeyInfo->mkiLength), 
                        tag_len, &newPktDesc,
                        SALLD_MAC_GENERATION, NULL);
         /* update the packet size after adding the MAC */
         mkiTagLen = tag_len;
     }
  
     /* If MKI is active */
     if(pKeyInfo->kdBitfield & SALLD_SRTP_MKI_MASK)
     {
         if (pKeyInfo->mkiLength == 2)
         {
            pktWrite16bits_m(pMki, 0, (uint16_t)pKeyInfo->mki);
         
         }
         else
         {
            pktWrite32bits_m(pMki, 0, pKeyInfo->mki);

         }
         mkiTagLen += pKeyInfo->mkiLength;
     }
  }
  else
  {
     /* SA shall perform the actual encryption and authentication */
     mkiTagLen = pKeyInfo->mkiLength + pKeySize->macSize;
     
     if (txInst->rekeyState != SRTP_REKEY_STATE_IDLE)
     {
        result = salld_srtp_tx_rekey_sm(inst);
        
        if(result != sa_ERR_OK)
            return (result);
     }
  }
  
  
  if(!retval)
  {
    /* Adjust the packet length */
    pPktDesc->payloadLen += mkiTagLen;
    pPktDesc->segUsedSizes[mkiSegmentIndex] += mkiTagLen;
    pPktDesc->size += mkiTagLen;
    
    /* Pass the software Info in the packet */
    pPktInfo->validBitMap |= sa_PKT_INFO_VALID_SW_INFO;
    pPktInfo->swInfo = txInst->swInfo;
  
    if(++txInst->packetEncLsw == 0)
      txInst->packetEncMsw++;
      
    return(sa_ERR_OK);
  }
  else
  {
    return(sa_ERR_GEN);
  }
} /* salldSrtpEncrypt */

/******************************************************************************
 * FUNCTION PURPOSE: SRTP Receive Data
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_srtp_receive_data_full (
 *    void *salldInst,    - A pointer to SRTP instance
 *    void *pktInfo)      - packet pointer
 * 
 *
 *****************************************************************************/
static int16_t salld_srtp_receive_data_full (void *salldInst, void *pktInfo) 
{
  int16_t j, retval=0;
  uint16_t seq;
  uint32_t mki; 
  salldSrtpInst_t *inst   = (salldSrtpInst_t *)salldInst; 
  salldSrtpRxInst_t *rxInst =  &inst->rxInst;
  salldSrtpKeyInfo_t* pKeyInfo = &rxInst->keyInfo;
  salldSrtpKeySizeParams_t*  pKeySize = &pKeyInfo->keySize;
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;
  Sa_PktDesc_t* pPktDesc = &pPktInfo->pktDesc;
  uint16_t cipher = rxInst->cipherMode;
  uint16_t mac = rxInst->authMode;
  uint32_t roundkey[SALLD_MAX_ROUND_KEY_SIZE_IN_TULONG];
  union {
    uint32_t temp_even;/* not needed here but precautionary to make iv even */
    tword iv[SALLD_AES_BLOCK_SIZE_IN_WORD];/* Warning: The starting address should be even for asm code */
  } ivu;
  tword *pktIn;
  tword *pRtp;
  uint32_t roc, temp;
  int16_t pktLen, tagSize = 0;
  int16_t mkiTagLen = 0;
  int16_t headerSize;
  tword *tagIn;
  uint16_t nr;
  Sa_KeyRequest_t keyReq;
  uint16_t roc_status = SALLD_ROC_NOCHANGE;
  
  int16_t result, segOffsetTmp, segOffset;
  int16_t srtpSegmentIndex, mkiSegmentIndex, srtpSegmentOffset, mkiSegmentOffset, payloadOffset, i;
  uint16_t segRem;
  uint16_t *segUsedSizes;
  salldAesDesc_t newPktDesc;
	tword pMki[14];

  /* Encrypt and Authenticate Packet(s) */
  /* Sanity check */
  if(pPktDesc->nSegments < 1)
      return (sa_ERR_PARAMS);
  
  payloadOffset = pPktDesc->payloadOffset;
  srtpSegmentIndex = mkiSegmentOffset = segOffsetTmp = i = segOffset = 0;
  segUsedSizes = pPktDesc->segUsedSizes;

  /* Calculate segment containing SRTP header as well as segment offset to
  ** SRTP Header 
  **
  **  int16_t segOffsetTmp        - size of the current segment being processed. 
  **  int16_t segOffset           - total size of previous segments before
  **                                current segment
  **  int16_t srtpSegmentIndex    - index of segment containing srtp header 
  **  int16_t srtpSegmentOffset   - offset to the SRTP header from the
  **                                beginning of the segment
  */
  segOffsetTmp = segUsedSizes[srtpSegmentIndex];
  while(segOffset + segOffsetTmp < payloadOffset)
  {
      segOffset += segOffsetTmp;
      segOffsetTmp = segUsedSizes[++srtpSegmentIndex];
  }
  srtpSegmentOffset = payloadOffset - segOffset;

  /* Authenticate and Decrypt Packet(s) */
  pRtp = (tword *)pPktDesc->segments[srtpSegmentIndex];
  pRtp += SALLD_BYTE_TO_WORD(srtpSegmentOffset);

  /* Determine current "index" (ROC||SEQ) */
  seq = pktRead16bits_m(pRtp, 2);
  roc = srtp_get_roc(seq, rxInst, &roc_status);

  headerSize = SALLD_RTP_HEADER_SIZE_IN_BYTE + 4*((pktRead16bits_m(pRtp, 0) & 0x0F00)>>8);
  if(mac) 
  {
      tagSize = pKeySize->macSize;
  }

	mkiTagLen = tagSize + pKeyInfo->mkiLength; 
  
  /* DESCRTIPTION:
  ** This loop is in continuation of the previous loop. Using the same
  ** algorithm to find the beginning of the MKI, while constructing
  ** multi-segment newPktDesc for authentication and encrpytion.
  **  
  **  int16_t mkiSegmentIndex     - index of segment containing mki
  **  int16_t mkiSegmentOffset    - offset to MKI from the beginning of
  **                                the segment
  **  tword **newPktDesc.segments - array of pointers to SRTP payload
  **                                segments
  **  int16_t *newPktDesc.inputLen- length of each SRTP payload segment
  ** */
  //pktLen = pPktDesc->payloadLen - headerSize;
  pktLen = payloadOffset + pPktDesc->payloadLen - mkiTagLen;  
  mkiSegmentIndex = srtpSegmentIndex;
  newPktDesc.nSegments = 0;
  i = 0;
  
  while(segOffset + segOffsetTmp < pktLen)
  {
        newPktDesc.segments[i] = (tword *) pPktDesc->segments[mkiSegmentIndex];
		newPktDesc.inputLen[i++] = segOffsetTmp;
		segOffset += segOffsetTmp;
		segOffsetTmp = segUsedSizes[++mkiSegmentIndex];
  }
  mkiSegmentOffset = pktLen - segOffset;
  
  newPktDesc.segments[0] = pRtp;
  
  if(!i)
  {
		newPktDesc.inputLen[0] = pPktDesc->payloadLen-mkiTagLen;
		i++;
  }
  else
  {
		/* Take care of first segment */
		newPktDesc.inputLen[0] -= srtpSegmentOffset;
	    
        if(mkiSegmentOffset)
	    {
		    newPktDesc.segments[i] = (tword *) pPktDesc->segments[mkiSegmentIndex];
		    newPktDesc.inputLen[i++] = mkiSegmentOffset;
	    }	
  }
	
  if((i + 1) > SALLD_MAX_SEGMENTS)
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
      misc_utlCopy((uint16_t *)pPktDesc->segments[mkiSegmentIndex+1], (uint16_t *)(pMki+ SALLD_BYTE_TO_TUINT(segRem)), SALLD_BYTE_TO_TUINT(mkiTagLen - segRem));
      pPktDesc->segUsedSizes[mkiSegmentIndex+1] = 0;
      pPktDesc->nSegments--;
  }
  
  // Add an extra segment for mki and tag */
  newPktDesc.segments[i] = (tword*)pMki;
  newPktDesc.inputLen[i++] = 4;
  newPktDesc.nSegments = i;  
  newPktDesc.pktLen = pPktDesc->payloadLen - mkiTagLen + 4;
    
  /* Prepare the key Request structure */
  keyReq.params.srtp.ctrlBitfield = sa_SRTP_KEY_REQUEST_RX_KEY;
  
  if(pKeyInfo->kdBitfield & SALLD_SRTP_MKI_MASK)
  {
		mki = (pKeyInfo->mkiLength == 2)?(uint32_t)pktRead16bits_m(pMki, 0): 
                                                   pktRead32bits_m(pMki, 0); 
    
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
          (salldLObj.callOutFuncs.ChanKeyRequest) ((Sa_ChanHandle) sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,salldInst), &keyReq);      
      }
      return sa_ERR_KEY_EXPIRED;
    }
  }
  
  result = srtp_key_validation(salldInst, pKeyInfo, seq, roc, 
                              &keyReq, &rxInst->packetDecLsw, 
                              &rxInst->packetDecMsw);

  if(result == SALLD_KEY_VALIDATION_ERROR)
    return(sa_ERR_KEY_EXPIRED);
    
  if((result == SALLD_KEY_VALIDATION_NEW_KEY) ||
     (result == SALLD_KEY_VALIDATION_NEW_KEY_NOW))
  {
    /* Need to prepare new security context and etc */
    rxInst->rekeyState = SRTP_REKEY_STATE_WAIT;
  }                              

  /* Do Sequence Number check/Replay check */
  if(rxInst->windowCheck == 64)
  {
  
    uint8_t ret_code;
    
    temp = ((roc & 0x0000FFFF) << 16) | (uint32_t)seq;
    
    ret_code = salld_replay_check(&(rxInst->replayWindow), temp);
    
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
    return sa_ERR_PARAMS;
  }

  /* Authenticate the packet */
  if(mac) 
  {
    //pktLen -= tagSize;
    tagIn = pMki + SALLD_BYTE_TO_WORD(pKeyInfo->mkiLength);
    
    /* copy MAC tag into iv Note: assumption is that the MAC tag is less than 16 bytes */
    /* append roc to make M = Authenticated Portion || ROC */
    for (j = 0; j < SALLD_BYTE_TO_WORD(tagSize); j ++) 
    {
      ivu.iv[j] = tagIn[j];
    }
    
    /* append roc to make M = Authenticated Portion || ROC */ 
		pktWrite32bits_m(pMki, 0, roc);
    /* Do Message Authentication */
		retval = 
            (*salld_mac_table[mac - sa_AuthMode_HMAC_MD5])(pKeyInfo->sessionAuthKey, 
                    (int16_t)pKeySize->sessionMacKeySize, ivu.iv, 
                    tagSize, &newPktDesc,
                    SALLD_MAC_AUTHENTICATION, NULL);

    if(retval)
    {
      inst->salldInst.stats.authFail++;
      return sa_ERR_AUTH_FAIL;
    }
  }

  if(cipher) 
  {
    /* Decryption of the payload */
    /* Encryption Round Key expansion */
    nr = aesKeyExpandEnc((tulong *)roundkey, (tword *)pKeyInfo->sessionEncKey, 
                         pKeySize->sessionEncKeySize<<3);

    /* form the IV - Initialization Vector for the confidentiality mode */
    (*srtpFormIVTab[cipher - sa_CipherMode_AES_CTR])((void *)pKeyInfo, pRtp, ivu.iv, roc);
    
        /* Decryption of the payload */
        newPktDesc.nSegments--;
        newPktDesc.segments[0] = pRtp + SALLD_BYTE_TO_WORD(headerSize);
	    newPktDesc.inputLen[0] -= headerSize;
        newPktDesc.pktLen -= (headerSize + 4); /* RTCP header: 8, ROC : 4 */
        
		retval = 
            (*salld_cipher_table[cipher - sa_CipherMode_AES_CTR])(roundkey, 
                    (int16_t)pKeySize->sessionEncKeySize, nr, ivu.iv, 
                    &newPktDesc);

  }
  
  /* Adjust the packet length */
  pPktDesc->payloadLen -= mkiTagLen;
  pPktDesc->segUsedSizes[mkiSegmentIndex] = mkiSegmentOffset;
  pPktDesc->size -= mkiTagLen;
  
  if(!retval)
  {
    if(++rxInst->packetDecLsw == 0)
      rxInst->packetDecMsw++; 

    switch(roc_status)
    {
      case SALLD_ROC_NOCHANGE:
        if(rxInst->lastSeqNum < seq)
          rxInst->lastSeqNum = seq;
        break;
      case SALLD_ROC_PLUS_1:
        rxInst->lastSeqNum = seq;
        pKeyInfo->roc = roc;
        break;
      case SALLD_ROC_MINUS_1:
        break;
      default:
        return sa_ERR_GEN;
    }
  
    temp = ((roc & 0x0000FFFF) << 16) | (uint32_t)seq;
  
    /* Update the Replay Window */
    if(rxInst->windowCheck == 64)
    {
      salld_replay_update(&(rxInst->replayWindow), temp);
    }
    
    if(rxInst->rekeyState != SRTP_REKEY_STATE_IDLE)
    {
        result = salld_srtp_rx_rekey_sm(inst, pKeyInfo->seqNumBase);
   
        if(result != sa_ERR_OK)
            return (result);
    }
    
    return (sa_ERR_OK); 
    
  }
  else
  {
    return(sa_ERR_GEN);
  }

  
} /* salld_srtp_receive_data_full */

/******************************************************************************
 * FUNCTION PURPOSE: SALLD Receive Data
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_srtp_receive_data (
 *    void *salldInst,             - A pointer to SALLD instance
 *    void *pktInfo)      - packet pointer
 * 
 *
 *****************************************************************************/
static int16_t salld_srtp_receive_data_update (void *salldInst, void *pktInfo) 
{
  uint16_t seq, result;
  salldSrtpInst_t *inst   = (salldSrtpInst_t *)salldInst; 
  salldSrtpRxInst_t *rxInst =  &inst->rxInst;
  salldSrtpKeyInfo_t* pKeyInfo = &rxInst->keyInfo;
  salldSrtpKeySizeParams_t*  pKeySize = &pKeyInfo->keySize;
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;
  Sa_PktDesc_t* pPktDesc = &pPktInfo->pktDesc;
  uint16_t mac = rxInst->authMode;
  Sa_KeyRequest_t keyReq;
  tword *pRtp;
  uint32_t roc, temp;
  int16_t mkiTagLen = 0;
  uint16_t roc_status = SALLD_ROC_NOCHANGE;
  int16_t srtpSegmentIndex, srtpSegmentOffset, payloadOffset, segOffsetTmp, segOffset;
  uint16_t *segUsedSizes;

  /* Encrypt and Authenticate Packet(s) */
  /* assume single segemnt at this moment */
  if(pPktDesc->nSegments < 1)
  return (sa_ERR_PARAMS);
  
  payloadOffset = pPktDesc->payloadOffset;
  srtpSegmentIndex = segOffsetTmp = segOffset = 0;
  segUsedSizes = pPktDesc->segUsedSizes; 
  
  /* Calculate segment containing SRTP header as well as segment offset to
  ** SRTP Header */
  segOffsetTmp = segUsedSizes[srtpSegmentIndex];
  while(segOffset + segOffsetTmp < payloadOffset)
  {
      segOffset += segOffsetTmp;
      segOffsetTmp = segUsedSizes[++srtpSegmentIndex];
  }
  srtpSegmentOffset = payloadOffset - segOffset; 
  
  /* Authenticate and Decrypt Packet(s) */
  pRtp = (tword *)pPktDesc->segments[srtpSegmentIndex];
  pRtp += SALLD_BYTE_TO_WORD(srtpSegmentOffset);
  
  /* Determine current "index" (ROC||SEQ) */
  seq = pktRead16bits_m(pRtp, 2);
  roc = srtp_get_roc(seq, rxInst, &roc_status);
  
  if(pKeyInfo->kdBitfield & SALLD_SRTP_MKI_MASK)
  {
    mkiTagLen = pKeyInfo->mkiLength;
  }

  /* Authenticate the packet */
  if(mac) 
  {
    mkiTagLen += pKeySize->macSize;
  }
  
  /* Check whether it is time to request new From-To key */
  /* Prepare the key Request structure */
  keyReq.params.srtp.ctrlBitfield = sa_SRTP_KEY_REQUEST_RX_KEY;
  srtp_key_validation_short(salldInst, pKeyInfo, seq, roc, &keyReq);

  /* Adjust the packet length */
  pPktDesc->payloadLen -= mkiTagLen;
  pPktDesc->segUsedSizes[0] -= mkiTagLen;
  pPktDesc->size -= mkiTagLen;
  
  if(++rxInst->packetDecLsw == 0)
    rxInst->packetDecMsw++; 

  switch(roc_status)
  {
    case SALLD_ROC_NOCHANGE:
      if(rxInst->lastSeqNum < seq)
        rxInst->lastSeqNum = seq;
      break;
    case SALLD_ROC_PLUS_1:
      rxInst->lastSeqNum = seq;
      pKeyInfo->roc = roc;
      break;
    case SALLD_ROC_MINUS_1:
      break;
    default:
      return sa_ERR_GEN;
  }
  
  temp = ((roc & 0x0000FFFF) << 16) | (uint32_t)seq;
  
  /* Update the Replay Window */
  if(rxInst->windowCheck == 64)
  {
    salld_replay_update(&(rxInst->replayWindow), temp);
  }
  
  if(rxInst->rekeyState != SRTP_REKEY_STATE_IDLE)
  {
      result = salld_srtp_rx_rekey_sm(inst, pKeyInfo->seqNumBase);
   
      if(result != sa_ERR_OK)
          return (result);
  }

  return sa_ERR_OK; 
  
} /* salld_srtp_receive_data_update */

/******************************************************************************
 * FUNCTION PURPOSE: SALLD Receive Data
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_srtp_receive_data (
 *    void *salldInst,             - A pointer to SALLD instance
 *    void *pktInfo)      - packet pointer
 * 
 *
 *****************************************************************************/
int16_t salld_srtp_receive_data (void *salldInst, void *pktInfo) 
{
    
  salldSrtpInst_t* inst   = (salldSrtpInst_t *)salldInst; 
  salldComStats_t* pStats = &inst->salldInst.stats;
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;
  int16_t ret;
  
  if (!(pPktInfo->validBitMap & sa_PKT_INFO_VALID_PKT_ERR_CODE) ||
     (inst->salldInst.stateBitfield & sa_CONTROLINFO_CTRL_SW_ONLY))
  {
    return (salld_srtp_receive_data_full(salldInst, pktInfo));
  }
  
  switch (pPktInfo->pktErrCode)
  {
    case sa_PKT_ERR_OK:
      ret = salld_srtp_receive_data_update(salldInst, pktInfo);
      break;
        
    case sa_PKT_ERR_REPLAY_OLD:
       pStats->replayOld++;
       ret = sa_ERR_REPLAY_OLD;
       break;
           
    case sa_PKT_ERR_REPLAY_DUP: 
       pStats->replayDup++;
       ret = sa_ERR_REPLAY_DUP;
       break;
    
    case sa_PKT_ERR_AUTH_FAIL:
       pStats->authFail++;
       ret = sa_ERR_AUTH_FAIL;
       break;
    
    case sa_PKT_ERR_INVALID_KEY:
    case sa_PKT_ERR_INVALID_MKI:
    default:
      ret = salld_srtp_receive_data_full(salldInst, pktInfo);
      break;
  }
  
  return (ret);
}

/****************************************************************************
 * FUNCTION PURPOSE: SRTP Get SwInfo 
 ****************************************************************************
 * DESCRIPTION: SRTP Get SwInfo
 *
 * int16_t  salld_srtp_get_swInfo (
 *   Sa_ChanHandle        handle      - SALLD channel identifier
 *   uint16_t             dir         - packet directions
 *   Sa_SWInfo_t         *pSwInfo)    - a pointer to swInfo
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *
 ***************************************************************************/
int16_t salld_srtp_get_swInfo (void *salldInst, uint16_t dir, Sa_SWInfo_t* pChanSwInfo)
{
  salldSrtpInst_t *inst = (salldSrtpInst_t *)salldInst;
  int16_t ret = sa_ERR_OK;
  
  if (dir == sa_PKT_DIR_FROM_NETWORK)
  {
    salldSrtpRxInst_t *rxInst = (salldSrtpRxInst_t *) &inst->rxInst;
    memcpy(pChanSwInfo, &rxInst->swInfo, sizeof(Sa_SWInfo_t));
    
  }
  else if (dir == sa_PKT_DIR_TO_NETWORK)
  {
    salldSrtpTxInst_t *txInst = (salldSrtpTxInst_t *) &inst->txInst;
    memcpy(pChanSwInfo, &txInst->swInfo, sizeof(Sa_SWInfo_t));
  }
  else
  {
    ret = sa_ERR_PARAMS;
  }
  
  return(ret);
}


/****************************************************************************
 * FUNCTION PURPOSE:   SRTP function call table 
 ****************************************************************************
 * DESCRIPTION:     The table is used to link SRTP functions to the 
 *                  SALLD library if required 
 *
 ***************************************************************************/
Sa_ProtocolCallTbl_t Sa_callTblStrp = 
{
  sa_PT_SRTP,
  salld_srtp_init,
  salld_srtp_control,
  salld_srtp_get_stats,
  salld_srtp_send_data,
  salld_srtp_receive_data,
  salld_srtp_close,
  salld_srtp_get_swInfo
};

   
/* Nothing past this point */



















