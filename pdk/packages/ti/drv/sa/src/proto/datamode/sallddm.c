/******************************************************************************
 * FILE PURPOSE: Data Mode Main File
 ******************************************************************************
 * FILE NAME: sallddm.c
 *
 * DESCRIPTION: The main module for Data Mode Code
 *
 * (C) Copyright 2009-2014, Texas Instruments Inc.
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
/* RTSC header files */ 

/* Standard include files */
#include <string.h>

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include <ti/drv/sa/sa_osal.h>
#include "src/salldloc.h"
#include "src/salldport.h"
#include "sallddm.h"
#include "sallddmloc.h"

/* Defined for testing. Should be moved to makedefs.mk */
#define dmGetChID(mid_chnum)  ( (((uint16_t)(mid_chnum)) & 0x00FF )-1)

/****************************************************************************
 * FUNCTION PURPOSE: Data Mode Key Setup 
 ****************************************************************************
 * DESCRIPTION: Update the Data Mode channel key information based on the input
 *              parameters.
 *
 *  uint16_t salld_data_mode_setup_key(
 *            salldDataModeInst_t*      inst        -> Point to Data Mode channel instance
 *            salldDataModeComInfo_t*   pComInfo    -> pointer to the instance key storage 
 *            Sa_DataModeKeyParams_t* pKeyParams) -> pointer to the key configuration
 *                                                     parameters.   
 *                       
 * Return values:  FALSE : invalid parameters
 *                 TRUE:   successful updtae         
 *
 ***************************************************************************/
uint16_t salld_data_mode_setup_key(salldDataModeInst_t *inst, 
                               salldDataModeComInfo_t* pComInfo, Sa_DataModeKeyParams_t* pKeyParams) 
{
  uint16_t ctrlBitMap = pKeyParams->ctrlBitfield;
    
  if(ctrlBitMap & sa_DATA_MODE_KEY_CTRL_ENC_KEY)
  {
      /* Copy Encryption Key */
      memcpy(pComInfo->sessionEncKey, pKeyParams->sessionEncKey, pComInfo->config.sessionEncKeySize);
  }
  
  if( ctrlBitMap & sa_DATA_MODE_KEY_CTRL_MAC_KEY)
  {
      /* Copy MAC Key */
      memcpy(pComInfo->sessionMacKey, pKeyParams->sessionAuthKey, pComInfo->config.sessionMacKeySize);
  }
  
  if( ctrlBitMap & sa_DATA_MODE_KEY_CTRL_SALT)
  {
      /* Copy session Salt */
      memcpy(pComInfo->sessionSalt, pKeyParams->sessionSalt, pComInfo->config.sessionSaltSize);
  }

  if(ctrlBitMap & sa_DATA_MODE_KEY_USE_DKEK)
  {
      /* Set USE_DKEK flag */
      pComInfo->config.ctrlBitMap |= sa_DM_CONFIG_USE_DKEK;
  }

  return TRUE;
} 

/****************************************************************************
 * FUNCTION PURPOSE: Verify Data Mode Configuration Parameters 
 ****************************************************************************
 * DESCRIPTION: Verify Data Mode general configuration parameters for consistency
 *              with the encryption and authentication mode
 *
 *  uint16_t salld_DM_verify_config_params(
 *            Sa_CipherMode_e cipherMode,                  -> Ciphering mode
 *            Sa_AuthMode_e   authMode,                    -> Aurthentication Mode
 *            Sa_DataModeConfigParams_t*  pDataModeConfig) ->pointer to the IPsec configuration
 *                                                    parameters.   
 *                       
 * Return values:  FALSE : invalid parameters
 *                 TRUE:   valid parameters          
 *
 ***************************************************************************/
static uint16_t salld_dm_verify_config_params(Sa_CipherMode_e cipherMode, Sa_AuthMode_e authMode,
                                              Sa_DataModeConfigParams_t*  pDataModeConfig)
{
    
    /* Common Check */
    if((pDataModeConfig->sessionEncKeySize > 32) ||
#if defined (NSS_LITE2)
       (pDataModeConfig->priv > 3)               ||
       (pDataModeConfig->sessionMacKeySize > 64))
#else
       (pDataModeConfig->sessionMacKeySize > 32))
#endif
        return (FALSE);

#if defined(NSS_LITE) || defined(NSS_LITE2)
    /* NSS LITE devices do not have Air Cipher Engines */
    if ((pDataModeConfig->ctrlBitMap & sa_DM_CONFIG_SELECT_AIR_CIPHER_ENG) == sa_DM_CONFIG_SELECT_AIR_CIPHER_ENG)
      return (FALSE);
#endif

    if((authMode == sa_AuthMode_NULL) && 
       ((cipherMode != sa_CipherMode_GCM) &&
        (cipherMode != sa_CipherMode_CCM)))
    {
        if(pDataModeConfig->macSize != 0)
            return(FALSE);
    }    
    
    /* Ciphering mode specific check */
    switch (cipherMode)
    {
        case sa_CipherMode_CCM:
            if(((pDataModeConfig->ivSize                + \
                 pDataModeConfig->sessionSaltSize) > 13) ||
                (pDataModeConfig->aadSize > 14)         ||
                (authMode != sa_AuthMode_NULL))
                return (FALSE);    
            break;
        
        case sa_CipherMode_DES_CBC:
        case sa_CipherMode_3DES_CBC:
            if((pDataModeConfig->ivSize != 8)          ||
               (pDataModeConfig->sessionSaltSize != 0))  
                return (FALSE);    
            break;    
            
        case sa_CipherMode_GCM:
            if(((pDataModeConfig->ivSize                 + \
                 pDataModeConfig->sessionSaltSize) != 12) ||
                (pDataModeConfig->aadSize > 16)           ||
                (authMode != sa_AuthMode_NULL))
                return (FALSE);    
            break;
            
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
        case sa_CipherMode_KASUMI_F8:
            if((pDataModeConfig->ivSize != 8))
                return (FALSE);    
            break;
            
        case sa_CipherMode_SNOW3G_F8:
            if((pDataModeConfig->ivSize != 16))
                return (FALSE);    
            break;
            
        case sa_CipherMode_AES_CTR:
        case sa_CipherMode_AES_F8:
        case sa_CipherMode_AES_CBC:
        case sa_CipherMode_GSM_A53:
        case sa_CipherMode_ECSD_A53:
        case sa_CipherMode_GEA3:
        case sa_CipherMode_NULL:
            break;     
        
#else
        case sa_CipherMode_AES_CTR:
        case sa_CipherMode_AES_F8:
        case sa_CipherMode_AES_CBC:
        case sa_CipherMode_NULL:
            break;     

#endif        
        
        default:
            return (FALSE);
    }
    
    /* Encryption mode specific check */
    switch (authMode)
    {
        case sa_AuthMode_GMAC:
          if(((pDataModeConfig->ivSize                 + \
               pDataModeConfig->sessionSaltSize) != 12) ||
              (pDataModeConfig->aadSize > 16)           ||
               (cipherMode != sa_CipherMode_NULL)) 
              return (FALSE);   
            break;
            
        case sa_AuthMode_GMAC_AH:
            if((pDataModeConfig->ivSize != 8)          ||
               (pDataModeConfig->sessionSaltSize != 4) ||
               (cipherMode != sa_CipherMode_NULL)) 
               return (FALSE);
            break;
            
        case sa_AuthMode_KASUMI_F9:
            if((pDataModeConfig->ivSize != 8))
               return (FALSE);
            break;
            
        case sa_AuthMode_MD5:
        case sa_AuthMode_SHA1:
        case sa_AuthMode_SHA2_224:
        case sa_AuthMode_SHA2_256:
        case sa_AuthMode_SHA2_384:
        case sa_AuthMode_SHA2_512:
        case sa_AuthMode_HMAC_MD5:
        case sa_AuthMode_HMAC_SHA1:
        case sa_AuthMode_HMAC_SHA2_224:
        case sa_AuthMode_HMAC_SHA2_256:
        case sa_AuthMode_CMAC:
        case sa_AuthMode_CBC_MAC:
        case sa_AuthMode_AES_XCBC:
        case sa_AuthMode_NULL:
        case sa_AuthMode_HMAC_SHA2_384:
        case sa_AuthMode_HMAC_SHA2_512:
            break;
            
        default:
            return(FALSE);
    }
    

  return TRUE;
}            
           

/****************************************************************************
 * FUNCTION PURPOSE: Data Mode Control 
 ****************************************************************************
 * DESCRIPTION: Data Mode Control functions
 *
 * int16_t  salld_data_mode_control (
 *   void  *salldInst  - a pointer to SALLD channel instance
 *   void  *ctrl)      - a pointer to control structure
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *
 ***************************************************************************/
int16_t salld_data_mode_control (void *salldInst, void *salldCtrl)
{
  salldDataModeInst_t *inst = (salldDataModeInst_t *)salldInst;
  salldDataModeTxInst_t *txInst = &inst->txInst;
  Sa_ChanCtrlInfo_t *ctrl = (Sa_ChanCtrlInfo_t *)salldCtrl;
  uint16_t bitmap;
  int16_t ret;

  switch(ctrl->ctrlType)
  {
    /* SALLD cipher, mac and key size selection */
    case sa_CHAN_CTRL_GEN_CONFIG:
    {
        bitmap = ctrl->ctrlInfo.gen.validBitfield;
        if( bitmap & sa_CONTROLINFO_VALID_TX_CTRL)
        {
            /* Input parameters check */
            Sa_GenConfigParams_t* pGenConfig = &ctrl->ctrlInfo.gen.txCtrl;
            Sa_DataModeConfigParams_t*  pDataModeConfig  = &ctrl->ctrlInfo.gen.txCtrl.params.data;
            
            if (salld_dm_verify_config_params(pGenConfig->cipherMode, pGenConfig->authMode, pDataModeConfig))
            {
                txInst->cipherMode = pGenConfig->cipherMode;
                txInst->authMode   = pGenConfig->authMode;
                txInst->destInfo   = pGenConfig->destInfo; 
                txInst->comInfo.config = *pDataModeConfig;
                if(txInst->cipherMode == sa_CipherMode_NULL)
                    txInst->comInfo.config.enc1st = FALSE;    
            }
            else
            {
                return(sa_ERR_PARAMS);
            }
#if defined(NSS_LITE2)
            txInst->comInfo.config.priv   = pDataModeConfig->priv;
            txInst->comInfo.config.privId = pDataModeConfig->privId;
#endif
        }
        
        /*
         * Is it time to form and register security context?
         */
        if(!SALLD_TEST_STATE_TX_SC_VALID(&inst->salldInst) && 
            SALLD_TEST_STATE_TX_ON(&inst->salldInst))
        {
            if ((ret = salld_data_mode_set_sc(inst)) != sa_ERR_OK)
            {
                return(ret);
            }
            
            if ((ret = salld_data_mode_set_cmdl(inst)) != sa_ERR_OK)
            {
                return(ret);
            }
            
            SALLD_SET_STATE_TX_SC_VALID(&inst->salldInst, 1);
        } 
    }    
    break;

    /* Master key and Salt setup for ac */
    case sa_CHAN_CTRL_KEY_CONFIG:
    {
        bitmap = ctrl->ctrlInfo.key.ctrlBitfield;
        
        if (bitmap & sa_KEY_CONTROL_TX_KEY_VALID)
        {
            Sa_DataModeKeyParams_t* pKeyParams = &ctrl->ctrlInfo.key.txKey.data;
            salldDataModeComInfo_t*   pComInfo = &txInst->comInfo;
            
            if(!salld_data_mode_setup_key(inst, pComInfo, pKeyParams))
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
 * FUNCTION PURPOSE: Data Mode Get Stats
 ******************************************************************************
 * DESCRIPTION: Extract Data Mode related statistics from the instance structure
 *
 * void salld_data_mode_get_stats (
 *    void   *salldInst,       - A pointer to SALLD instance
 *    uint16_t flags,            - various control flags
 *    void   *stats)           - The stat structure
 *
 * Return values:  sa_ERR_OK
 *
 *****************************************************************************/
int16_t salld_data_mode_get_stats (void *salldInst, uint16_t flags, void *stats)
{
  Sa_DataModeStats_t  *pStats = (Sa_DataModeStats_t *)  stats;
  salldDataModeInst_t   *inst   = (salldDataModeInst_t *)   salldInst; 
  salldDataModeTxInst_t *txInst = (salldDataModeTxInst_t *) &inst->txInst;

  pStats->pktHi  = txInst->packetMsw;
  pStats->pktLo  = txInst->packetLsw;

  return (sa_ERR_OK);
}

static const uint8_t saKasumiF9PaddingUp[8]   = {0x40, 0, 0, 0, 0, 0, 0, 0};
static const uint8_t saKasumiF9PaddingDown[8] = {0xC0, 0, 0, 0, 0, 0, 0, 0};

/******************************************************************************
 * FUNCTION PURPOSE: Data Mode Send Data
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_data_mode_send_data (
 *    void *salldInst,      - A pointer to SALLD Data Mode instance
 *    void *pktInfo,        - packet pointer
 *    uint16_t clear) 
 *
 *  Perform the following actions:
 *      - Update statistics
 *
 *****************************************************************************/
int16_t salld_data_mode_send_data (void *salldInst, void *pktInfo, uint16_t clear) 
{
  salldDataModeInst_t *inst   = (salldDataModeInst_t *)salldInst; 
  salldDataModeTxInst_t *txInst = &inst->txInst;
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;
  int16_t ret_code;
  
  /* Kasumi-F9 Padding:
   * The SASS is not able to generate Kasumi-F9 padding when the number of bytes to be authenticated is multiple of 8.
   * Therefore, we need to perform Kasumi-F9 padding for this special case 
   * Assumption:the padding bytes can be added to the end of data segments
   */
  if (txInst->authMode == sa_AuthMode_KASUMI_F9)
  {
    Sa_PayloadInfo_t *pPayloadInfo = &pPktInfo->payloadInfo;
    uint8_t *pPadding;
    
    if (!(pPayloadInfo->authSize & 0x7))
    {
        Sa_PktDesc_t *pPktDesc = &pPktInfo->pktDesc;
        int paddingSegIndex = pPktDesc->nSegments - 1;        
        
        /* The authentication data is 8-byte aligned, padding is required */
        if(pPktDesc->size != (pPayloadInfo->authSize + pPayloadInfo->authOffset))
            return(sa_ERR_GEN);
            
        if((pPktDesc->segAllocSizes[paddingSegIndex] - pPktDesc->segUsedSizes[paddingSegIndex]) < 8)
            return(sa_ERR_GEN);
            
        pPadding = (uint8_t *)pPktDesc->segments[paddingSegIndex];
        pPadding += pPktDesc->segUsedSizes[paddingSegIndex];    
            
        memcpy(pPadding,
               txInst->comInfo.config.enc?saKasumiF9PaddingDown:saKasumiF9PaddingUp,
               8);        
            
        pPktDesc->size += 8;
        pPktDesc->segUsedSizes[paddingSegIndex] += 8;
        pPayloadInfo->authSize += 8;
    }
  
  }

  /* Encrypt and Authenticate Packet(s) */
  ret_code = salld_data_mode_update_cmdl(inst, pPktInfo);
  
  if(ret_code != sa_ERR_OK)
    return (ret_code);  
    
  /* Pass the software Info in the packet */
  pPktInfo->validBitMap |= sa_PKT_INFO_VALID_SW_INFO;
  pPktInfo->swInfo = txInst->swInfo;

  if(txInst->packetLsw == 0xFFFFFFFFu)
  {
      txInst->packetLsw = 0u;
      txInst->packetMsw++;
  }
  else
  {
      txInst->packetLsw++;
  }
  return(sa_ERR_OK);
      
} /* salld_data_mode_send_data */

/******************************************************************************
 * FUNCTION PURPOSE: Data Mode Receive Data
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_data_mode_receive_data (
 *    void *salldInst,    - A pointer to SALLD instance
 *    void *pktInfo)      - packet pointer
 * 
 * Not Supported
 *****************************************************************************/
int16_t salld_data_mode_receive_data (void *salldInst, void *pktInfo) 
{
  /* Kasumi-F9 De-Padding */
  salldDataModeInst_t *inst   = (salldDataModeInst_t *)salldInst; 
  salldDataModeTxInst_t *txInst = &inst->txInst;
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;
  
  /* Kasumi-F9 Padding:
   * The SASS is not able to generate Kasumi-F9 padding when the number of bytes to be authenticated is multiple of 8.
   * Therefore, we need to perform Kasumi-F9 padding for this special case 
   */
  if (txInst->authMode == sa_AuthMode_KASUMI_F9)
  {
    Sa_PayloadInfo_t *pPayloadInfo = &pPktInfo->payloadInfo;
    
    if (!(pPayloadInfo->authSize & 0x7))
    {
        Sa_PktDesc_t *pPktDesc = &pPktInfo->pktDesc;
        int paddingSegIndex = pPktDesc->nSegments - 1;
        int numBytes = 8;  
        
        pPktDesc->size -= 8;
        
        while (numBytes)
        {
            if (pPktDesc->segUsedSizes[paddingSegIndex] >= numBytes)
            {
                pPktDesc->segUsedSizes[paddingSegIndex] -= numBytes;
                numBytes = 0;    
            }
            else
            {
                numBytes -= pPktDesc->segUsedSizes[paddingSegIndex];
                pPktDesc->segUsedSizes[paddingSegIndex] = 0;
                if (paddingSegIndex)
                {
                    paddingSegIndex--;    
                }
                else
                {
                    return(sa_ERR_GEN);
                }
                    
            }
        }
    }
  
  }

  return (sa_ERR_OK);
}

/****************************************************************************
 * FUNCTION PURPOSE: Construct Data Mode Security Context
 ****************************************************************************
 * DESCRIPTION: Construct Security Context from the configuration parameters 
 *          for Data Mode (to-SA) operations
 *
 *  uint16_t salld_data_mode_set_sc(
 *            salldIpsecInst_t*     inst)      -> Point to Data Mode channel instance
 *                       
 * Return values: sa_ERR_XXX
 * 
 * Assumption: The same algorithm will be used for both encryption and 
 *             authentication                          
 *
 ***************************************************************************/
int16_t salld_data_mode_set_sc(salldDataModeInst_t *inst) 
{
  salldObj_t *sysInst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->salldInst.ownerInstOffset);
  salldDataModeTxInst_t *txInst = &inst->txInst;
  salldDataModeComInfo_t *pComInfo = &txInst->comInfo;
  Sa_DataModeConfigParams_t  *pConfig = &pComInfo->config;
  Sa_ScReqInfo_t* pScInfo = &txInst->scInfo;
  int16_t encCmdlSize, encScSize, encScOffset;
  int16_t authCmdlSize, authScSize, authScOffset;
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  int16_t acAlgorithm = SA_AC_ALGORITHM_GSM_A53;
#endif
  saDMAReqInfo_t dmaReqInfo;
  uint16_t useEnc;
  uint16_t fRandomIV;
  uint8_t firstEngId;
  uint8_t tagSize;
#ifdef NSS_LITE2
  uint32_t ctxAttrCtrlBitMap;
#endif

  /*
   * Calculate the security Context Size and allocate security Context
   *
   */
  salld_sc_enc_get_info(txInst->cipherMode, pConfig->ivSize, &encCmdlSize, &encScSize, &pComInfo->encEngId, &fRandomIV, SALLD_TEST_SASS_GEN2(sysInst)); 
  salld_sc_auth_get_info(txInst->authMode, &useEnc, &authCmdlSize, &authScSize, &pComInfo->authEngId);
  pScInfo->scSize = SA_CTX_PHP_DATA_MODE_SIZE + encScSize + authScSize;
  salldLObj.callOutFuncs.ScAlloc((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo);

  /* Fix the encrpytion engine ID that is set to use from Encryption Engine
     when auth engine ID is Air Cipher OR if the channel is Air Cipher */
  if (pComInfo->encEngId  == SALLD_CMDL_ENGINE_ID_ES1)
  {
    if (((pConfig->ctrlBitMap & sa_DM_CONFIG_SELECT_AIR_CIPHER_ENG) == sa_DM_CONFIG_SELECT_AIR_CIPHER_ENG)  ||
         (pComInfo->authEngId == SALLD_CMDL_ENGINE_ID_ACS1))
    {
      pComInfo->encEngId = SALLD_CMDL_ENGINE_ID_ACS1;
    }
  }

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  if (pComInfo->authEngId == SALLD_CMDL_ENGINE_ID_ES1)
  {
    if ((pConfig->ctrlBitMap & sa_DM_CONFIG_SELECT_AIR_CIPHER_ENG) == sa_DM_CONFIG_SELECT_AIR_CIPHER_ENG)
    {
      pComInfo->authEngId = SALLD_CMDL_ENGINE_ID_ACS1;
    }
  }
#endif

  if(pScInfo->scBuf == (uintptr_t) NULL)
    return(sa_ERR_NO_CTX_BUF);
    
  Sa_osalBeginScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  
  memset((void *) pScInfo->scBuf, 0, SALLD_BYTE_TO_WORD(pScInfo->scSize));
  
  /* Prepare PHP Security Context */
  memset(&dmaReqInfo, 0, sizeof(saDMAReqInfo_t));
  dmaReqInfo.phpFetchSize = SA_CTX_DMA_SIZE_64;
  /* Non-Air Ciphering condition */
  if ((!useEnc) || pConfig->enc1st)
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(encScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
    encScOffset = SALLD_BYTE_TO_WORD(SA_CTX_PHP_DATA_MODE_SIZE);
    authScOffset = SALLD_BYTE_TO_WORD(SA_CTX_PHP_DATA_MODE_SIZE + encScSize);
  }
  else
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(encScSize);
    authScOffset = SALLD_BYTE_TO_WORD(SA_CTX_PHP_DATA_MODE_SIZE);
    encScOffset = SALLD_BYTE_TO_WORD(SA_CTX_PHP_DATA_MODE_SIZE + authScSize);
    
  }
  
  /* Air Ciphering condition */
  /* Assumption: Air Ciphering algorithm will be used as a pair */
  if (pConfig->enc1st && (pComInfo->encEngId == SALLD_CMDL_ENGINE_ID_ACS1))
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(encScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
    encScOffset = SALLD_BYTE_TO_WORD(SA_CTX_PHP_DATA_MODE_SIZE);
    authScOffset = SALLD_BYTE_TO_WORD(SA_CTX_PHP_DATA_MODE_SIZE + encScSize);
  }
  else if (pComInfo->authEngId == SALLD_CMDL_ENGINE_ID_ACS1)
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(encScSize);
    authScOffset = SALLD_BYTE_TO_WORD(SA_CTX_PHP_DATA_MODE_SIZE);
    encScOffset = SALLD_BYTE_TO_WORD(SA_CTX_PHP_DATA_MODE_SIZE + authScSize);
  }
  
  dmaReqInfo.phpEvictSize =  SA_CTX_DMA_SIZE_64;

#ifdef NSS_LITE2
  if (pConfig->ctrlBitMap & sa_DM_CONFIG_PROMOTE_CHANNEL)
  {
     ctxAttrCtrlBitMap  = SA_CTX_SA2UL_ALLOW_PROMOTE | SA_CTX_SA2UL_SECURE;
  }
  else if (pConfig->ctrlBitMap & sa_DM_CONFIG_DEMOTE_CHANNEL)
  {
     ctxAttrCtrlBitMap  = SA_CTX_SA2UL_ALLOW_DEMOTE | SA_CTX_SA2UL_SECURE;
  }
  else
  {
     /* Regular channel, non secure */
     ctxAttrCtrlBitMap = 0;
  }

  if (pConfig->ctrlBitMap & sa_DM_CONFIG_USE_SECURE_CTX_FOR_NON_SECURE_CHANNEL)
  {
     ctxAttrCtrlBitMap |= SA_CTX_SA2UL_ALLOW_NONSEC | SA_CTX_SA2UL_SECURE;
  }

  if (pConfig->priv)
  {
     /* Set for supervior mode */
     ctxAttrCtrlBitMap |= SA_CTX_SA2UL_SET_PRIV;
  }

  salld_set_sc_scctl(&dmaReqInfo, &txInst->destInfo, SA_CTX_PKT_TYPE_NONE,
                            pScInfo->scID, (tword *) pScInfo->scBuf, ctxAttrCtrlBitMap, pConfig->priv, pConfig->privId);
#else
  {
    salld_set_sc_phpCommom(&dmaReqInfo, &txInst->destInfo, SA_CTX_PKT_TYPE_NONE,
                           pScInfo->scID, (tword *) pScInfo->scBuf);
  }
#endif

  /* Prepare Security Context for the encryption Engine */
  if (encScSize)
  {
    #if !defined(NSS_LITE) && !defined(NSS_LITE2)
    if (pComInfo->encEngId == SALLD_CMDL_ENGINE_ID_ACS1)
    {
        salld_set_sc_acEnc(txInst->cipherMode, pConfig->sessionEncKeySize, 
                           pComInfo->sessionEncKey, NULL, pConfig->enc,
                           (tword *)pScInfo->scBuf + encScOffset,
                           &acAlgorithm, SALLD_TEST_SASS_GEN2(sysInst));
    }
    else
    #endif 
    {
    
        salld_set_sc_enc(sa_PT_NULL, txInst->cipherMode, pConfig->sessionEncKeySize, 
                         pComInfo->sessionEncKey, (uint8_t) pConfig->aadSize, pConfig->enc, 
                         (tword *)pScInfo->scBuf + encScOffset);
    }               

    if (pConfig->ctrlBitMap & sa_DM_CONFIG_USE_DKEK)
    {
        pktWrite8bits_m((tword *)pScInfo->scBuf, encScOffset, SA_ENC_MODE_USE_DKEK);
    }
  }
  
  /* Prepare Security Context for the authentication Engine */
  if (authScSize)
  {
    #if !defined(NSS_LITE) && !defined(NSS_LITE2)
    if (pComInfo->authEngId == SALLD_CMDL_ENGINE_ID_ACS1)
    {
        salld_set_sc_acAuth(txInst->authMode, pConfig->sessionMacKeySize, 
                            pComInfo->sessionMacKey, 
                            (tword *) pScInfo->scBuf + authScOffset,
                            &acAlgorithm,
                            pConfig->enc?SA_KASUMI_AUTH_DIR1:SA_KASUMI_AUTH_DIR0,
                            SALLD_TEST_SASS_GEN2(sysInst));
    }  
    else
    #endif
    {
        if (useEnc)
        {
            salld_set_sc_enc(sa_PT_NULL, txInst->authMode, pConfig->sessionMacKeySize, 
                          pComInfo->sessionMacKey, (uint8_t) pConfig->aadSize, FALSE,
                          (tword *) pScInfo->scBuf + authScOffset);
    
        }
        else
        {
            salld_set_sc_auth(txInst->authMode, pConfig->sessionMacKeySize, 
                          pComInfo->sessionMacKey, 
                          (tword *) pScInfo->scBuf + authScOffset);
        }               
    }                     
  }

  if ((pComInfo->encEngId == SALLD_CMDL_ENGINE_NONE) && (pComInfo->authEngId == SALLD_CMDL_ENGINE_NONE))
  {
    firstEngId = SALLD_CMDL_FINAL_ENGINE_ID;
  }
  else if (pComInfo->encEngId == SALLD_CMDL_ENGINE_NONE)
  {
    firstEngId = pComInfo->authEngId;
  }
  else if (pComInfo->authEngId == SALLD_CMDL_ENGINE_NONE)
  {
    firstEngId = pComInfo->encEngId;
  }
  else
  {
    firstEngId = pConfig->enc1st?pComInfo->encEngId:pComInfo->authEngId;
  }

  tagSize = SALLD_ROUND_UP(pConfig->macSize, 8);
  
  #ifdef SALLD_DATA_MODE_USE_PHP
  /* Construct the Data Mode specific Security Context */    
  {
    tword* ctxIn = (tword *)(pScInfo->scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_COMMON_SIZE));
  
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoDm_t, firstEngIdTagSize), 
                     SALLD_MK_UINT16(firstEngId, tagSize));
                                     
  }
  #endif
  
  /* Security Context swizzling */
  salld_swiz_128((uint8_t*) pScInfo->scBuf, (uint8_t*) pScInfo->scBuf, pScInfo->scSize);
  
  Sa_osalEndScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  
  /* Prepare the SW Info Words */
  #ifndef SALLD_DATA_MODE_USE_PHP
      #if defined(NSS_LITE2)
        salld_set_swInfo2(firstEngId, 0,
                         &txInst->destInfo,
                         pScInfo, &txInst->swInfo, tagSize);
      #else
        salld_set_swInfo(firstEngId, 0,
                       &txInst->destInfo,
                       pScInfo, &txInst->swInfo, tagSize);
      #endif
  #else
      /* Data packets enter PHP engine */
      salld_set_swInfo(SALLD_CMDL_ENGINE_SRTP_AC_HPS1, 0, 
                       &txInst->destInfo,
                       pScInfo, &txInst->swInfo, 0);
  #endif
  /* Store the scBuf internally as offset to suppor multiprocess */
  pScInfo->scBuf = (uintptr_t) sa_CONV_ADDR_TO_OFFSET(salldLObj.scPoolBaseAddr, pScInfo->scBuf);
  
  return (sa_ERR_OK);
} 

/****************************************************************************
 * FUNCTION PURPOSE: Data Mode Get SwInfo 
 ****************************************************************************
 * DESCRIPTION: Data Mode Get SwInfo
 *
 * int16_t  salld_data_mode_get_swInfo (
 *   Sa_ChanHandle        handle      - SALLD channel identifier
 *   uint16_t             dir         - packet directions
 *   Sa_SWInfo_t         *pSwInfo)    - a pointer to swInfo
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *                 sa_ERR_UNSUPPORTED
 *
 ***************************************************************************/
int16_t salld_data_mode_get_swInfo (void *salldInst, uint16_t dir, Sa_SWInfo_t* pChanSwInfo)
{
  salldDataModeInst_t *inst = (salldDataModeInst_t *)salldInst;
  int16_t ret = sa_ERR_OK;
  
  if (dir == sa_PKT_DIR_TO_NETWORK)
  {
    salldDataModeTxInst_t *txInst = (salldDataModeTxInst_t *) &inst->txInst;
    memcpy(pChanSwInfo, &txInst->swInfo, sizeof(Sa_SWInfo_t));
  }
  else
  {
    ret = sa_ERR_PARAMS;
  }
  
  return(ret);
}

/****************************************************************************
 * Table PURPOSE:   Data Mode function call table 
 ****************************************************************************
 * DESCRIPTION:     The tables are used to link Data Mode functions to the 
 *                  SALLD library if required 
 *
 ***************************************************************************/
Sa_ProtocolCallTbl_t Sa_callTblDataMode = 
{
  sa_PT_NULL,
  salld_data_mode_init,
  salld_data_mode_control,
  salld_data_mode_get_stats,
  salld_data_mode_send_data,
  salld_data_mode_receive_data,
  salld_data_mode_close,
  salld_data_mode_get_swInfo
};
           
   
/* Nothing past this point */

