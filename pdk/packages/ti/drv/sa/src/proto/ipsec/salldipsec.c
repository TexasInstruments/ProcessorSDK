/******************************************************************************
 * FILE PURPOSE: IPSEC Main File
 ******************************************************************************
 * FILE NAME: salldipsec.c
 *
 * DESCRIPTION: The main module for IPSEC Code
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
/* RTSC header files */ 

/* Standard include files */
#include <string.h>

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include <ti/drv/sa/sa_osal.h>
#include "src/salldloc.h"
#include "src/salldport.h"
#include "salldipsec.h"
#include "salldipsecloc.h"

/* Defined for testing. Should be moved to makedefs.mk */
#define ipsecGetChID(mid_chnum)  ( (((uint16_t)(mid_chnum)) & 0x00FF )-1)

/****************************************************************************
 * FUNCTION PURPOSE: IPSEC Salt Conversion 
 ****************************************************************************
 * DESCRIPTION: Convert the IPSEC Salt into the common 4-byte format
 *              (CCM: 3-byte slat ==> flag || salt)  
 *
 *  void salld_ipsec_conv_salt(
 *            salldIpsecInst_t*  inst          -> Point to IPSEC channel instance
 *            uint16_t cipherMode                -> Encryption Mode
 *            salldIpsecComInfo_t*   pComInfo  ->pointer to the instance key storage 
 *            Sa_IpsecKeyParams_t* pKeyParams) ->pointer to the key configuration
 *                                               parameters.   
 *                       
 * Return values:  None
 *
 ***************************************************************************/
static void salld_ipsec_conv_salt(salldIpsecInst_t *inst, uint16_t cipherMode,
                                  salldIpsecComInfo_t* pComInfo, Sa_IpsecKeyParams_t* pKeyParams)
{
    uint8_t flags;
    
    if (cipherMode == sa_CipherMode_CCM)
    {
        /* Flags: 0||Adata||(t-2)/2||L-1 */
        flags = 0x40 |   /* AAD always exists in IPSEC */
                (((pComInfo->config.macSize - 2) >> 1) << 3) |
                (15 - pComInfo->config.sessionSaltSize - pComInfo->config.ivSize - 1);
        pktWrite8bits_m((tword *)pComInfo->sessionSalt, 0, flags);
        pktPackBytesIntoWords((tword *)pKeyParams->sessionSalt, (tword *)pComInfo->sessionSalt, pComInfo->config.sessionSaltSize, 1);    
    }
    else
    {
        /* Copy Master salt */
        misc_utlCopy((uint16_t *)pKeyParams->sessionSalt, pComInfo->sessionSalt, 
                    SALLD_BYTE_TO_TUINT(pComInfo->config.sessionSaltSize));
    }
}                                   

/****************************************************************************
 * FUNCTION PURPOSE: IPSEC Key Setup 
 ****************************************************************************
 * DESCRIPTION: Update the IPSEC channel key information based on the input
 *          parameters.
 *
 *  uint16_t salld_ipsec_setup_key(
 *            salldIpsecInst_t*  inst           -> Point to IPSEC channel instance
 *            uint16_t cipherMode                 -> Encryption Mode
 *            salldIpsecComInfo_t*   pComInfo   ->pointer to the instance key storage 
 *            Sa_IpsecKeyParams_t* pKeyParams) ->pointer to the key configuration
 *                                               parameters.   
 *                       
 * Return values:  FALSE : invalid parameters
 *                 TRUE:   successful updtae         
 *
 ***************************************************************************/
static uint16_t salld_ipsec_setup_key(salldIpsecInst_t *inst, uint16_t cipherMode,
                                  salldIpsecComInfo_t* pComInfo, Sa_IpsecKeyParams_t* pKeyParams) 
{
  uint16_t ctrlBitMap = pKeyParams->ctrlBitfield;
    
  if(ctrlBitMap & sa_IPSEC_KEY_CTRL_ENC_KEY)
  {
      /* Copy Master Key */
      misc_utlCopy((uint16_t *)pKeyParams->sessionEncKey, pComInfo->sessionEncKey, 
                   SALLD_BYTE_TO_TUINT(pComInfo->config.sessionEncKeySize));
  }
  
  if( ctrlBitMap & sa_IPSEC_KEY_CTRL_MAC_KEY)
  {
      /* Copy Master salt */
      misc_utlCopy((uint16_t *)pKeyParams->sessionAuthKey, pComInfo->sessionMacKey, 
                   SALLD_BYTE_TO_TUINT(pComInfo->config.sessionMacKeySize));
  }
  
  if( ctrlBitMap & sa_IPSEC_KEY_CTRL_SALT)
  {
      /* Convert Master salt */
      salld_ipsec_conv_salt(inst, cipherMode, pComInfo, pKeyParams);
  }

  return TRUE;
}  

/****************************************************************************
 * FUNCTION PURPOSE: Verify IPSEC Configuration Parameters 
 ****************************************************************************
 * DESCRIPTION: Verify IPSEC general configuration parameters for consistency
 *              with the encryption and authentication mode
 *
 *  uint16_t salld_ipsec_verify_config_params(
 *            Sa_CipherMode_e cipherMode,           -> Ciphering mode
 *            Sa_AuthMode_e   authMode,             -> Aurthentication Mode
 *            Sa_IpsecConfigParams_t* pIpsecConfig) ->pointer to the IPsec configuration
 *                                                    parameters.   
 *                       
 * Return values:  FALSE : invalid parameters
 *                 TRUE:   valid parameters          
 *
 ***************************************************************************/
static uint16_t salld_ipsec_verify_config_params(Sa_CipherMode_e cipherMode, Sa_AuthMode_e authMode,
                                                 Sa_IpsecConfigParams_t* pIpsecConfig)
{
    
    /* Common Check */
    if((pIpsecConfig->sessionEncKeySize > 32) ||
       (pIpsecConfig->sessionMacKeySize > 32))
        return (FALSE);
           
    if((authMode == sa_AuthMode_NULL) && 
       ((cipherMode != sa_CipherMode_GCM) &&
        (cipherMode != sa_CipherMode_CCM)))
    {
        if(pIpsecConfig->macSize != 0)
            return(FALSE);
    }    
    
    /* Ciphering mode specific check */
    switch (cipherMode)
    {
        case sa_CipherMode_CCM:
            if((pIpsecConfig->encryptionBlockSize != 4)   ||
               (pIpsecConfig->ivSize != 8)          ||
               (pIpsecConfig->sessionSaltSize != 3) ||
               (authMode != sa_AuthMode_NULL))
                return (FALSE);    
            break;
            
        case sa_CipherMode_AES_CBC:
            if((pIpsecConfig->encryptionBlockSize != 16)  ||
               (pIpsecConfig->ivSize != 16)         ||
               (pIpsecConfig->sessionSaltSize != 0))  
                return (FALSE);    
            break;
        
        case sa_CipherMode_DES_CBC:
        case sa_CipherMode_3DES_CBC:
            if((pIpsecConfig->encryptionBlockSize != 8)   ||
               (pIpsecConfig->ivSize != 8)          ||
               (pIpsecConfig->sessionSaltSize != 0))  
                return (FALSE);    
            break;    
            
        case sa_CipherMode_AES_CTR:
        case sa_CipherMode_GCM:
            if((pIpsecConfig->encryptionBlockSize != 4)   ||
               (pIpsecConfig->ivSize != 8)          ||
               (pIpsecConfig->sessionSaltSize != 4)) 
               return (FALSE);
            
            if((cipherMode == sa_CipherMode_GCM) &&
               (authMode != sa_AuthMode_NULL))
               return (FALSE);  
            break;   
            
        case sa_CipherMode_NULL:
            if(((authMode != sa_AuthMode_GMAC) && (authMode != sa_AuthMode_GMAC_AH)) &&
               (pIpsecConfig->ivSize != 0))
               return (FALSE);
            break;     
        
        default:
            return (FALSE);
    }
    
    /* Encryption mode specific check */
    switch (authMode)
    {
        case sa_AuthMode_HMAC_MD5:
        case sa_AuthMode_CMAC:
        case sa_AuthMode_AES_XCBC:
            if(pIpsecConfig->sessionMacKeySize != 16)
                return (FALSE);
            break;
            
        case sa_AuthMode_HMAC_SHA1:
            if(pIpsecConfig->sessionMacKeySize != 20)
                return(FALSE);
            break;
            
        case sa_AuthMode_HMAC_SHA2_224:
        case sa_AuthMode_HMAC_SHA2_256:
            if(pIpsecConfig->sessionMacKeySize != 32)
                return(FALSE);
            break;

        case sa_AuthMode_GMAC:
            if((pIpsecConfig->ivSize != 8)           ||
               (pIpsecConfig->sessionSaltSize != 4)  ||
               (cipherMode != sa_CipherMode_NULL)) 
               return (FALSE);
        
        case sa_AuthMode_NULL:
            break;
            
        default:
            return(FALSE);
    }
    

  return TRUE;
}      

/****************************************************************************
 * FUNCTION PURPOSE: SALLD update IPSEC common information at the shadow 
 *                   instance  
 ****************************************************************************
 * DESCRIPTION: Pouplate and update the IPSEC common information at the
 *              shaddow instance which is used by another processor running in 
 *              opposite Endian mode.
 *
 *  void salld_ipsec_update_shadow_comInfo(
 *            salldIpsecComInfo_t*  srcInfo,         -> Pointer to the source
 *            salldIpsecComInfo_t*  destInfo         -> Pointer to the destination
 *                                 )
 * Return values:  None
 *
 ***************************************************************************/
static void salld_ipsec_update_shadow_comInfo(salldIpsecComInfo_t *srcInfo, salldIpsecComInfo_t *destInfo)
{
    Sa_IpsecConfigParams_t  *srcCfg  = &srcInfo->config; 
    Sa_IpsecConfigParams_t  *destCfg = &destInfo->config; 
    
    memcpy(destInfo, srcInfo, sizeof(salldIpsecComInfo_t));
    destInfo->esn  = SALLD_SWIZ(srcInfo->esn);
    
    destCfg->transportType = SALLD_SWIZ(srcCfg->transportType);
    destCfg->ctrlBitMap    = SALLD_SWIZ(srcCfg->ctrlBitMap);
    destCfg->encryptionBlockSize = SALLD_SWIZ(srcCfg->encryptionBlockSize);
    destCfg->sessionEncKeySize = SALLD_SWIZ(srcCfg->sessionEncKeySize);
    destCfg->sessionMacKeySize = SALLD_SWIZ(srcCfg->sessionMacKeySize);
    destCfg->sessionSaltSize = SALLD_SWIZ(srcCfg->sessionSaltSize);
    destCfg->ivSize = SALLD_SWIZ(srcCfg->ivSize);
    destCfg->macSize = SALLD_SWIZ(srcCfg->macSize);
    destCfg->nextHdr = SALLD_SWIZ(srcCfg->nextHdr);
    destCfg->spi = SALLD_SWIZ(srcCfg->spi);
    destCfg->esnLo = SALLD_SWIZ(srcCfg->esnLo);
    destCfg->esnHi = SALLD_SWIZ(srcCfg->esnHi);
}     

/****************************************************************************
 * FUNCTION PURPOSE: IPSEC update shadow instance 
 ****************************************************************************
 * DESCRIPTION: Pouplate and update the IPSEC-specific shadow instance will
 *              is used by another processor running in opposite Endian mode.
 *
 *  void salld_ipsec_update_shadowInst(
 *            salldIpsecInst_t*  inst          -> Point to IPSEC channel instance
 *                                    )
 * Return values:  None
 *
 ***************************************************************************/
static void salld_ipsec_update_shadowInst(salldIpsecInst_t *inst)
{
  salldIpsecInst_t   *sInst;
  salldIpsecTxInst_t *txInst = &inst->txInst;
  salldIpsecRxInst_t *rxInst = &inst->rxInst;
  salldIpsecTxInst_t *sTxInst;
  salldIpsecRxInst_t *sRxInst;
  salldInst_t        *shadowInst;

  /* shadow instance offset set to 0 indicates that there is no such instance */
  if (inst->salldInst.shadowInstOffset == 0)
    return;

  shadowInst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->salldInst.shadowInstOffset));
  sInst      =  (salldIpsecInst_t *) shadowInst;
  
  /* 
   * Most of parameters in salldInst were populated during channel creation and 
   * those parameters do not change. Update the active parameters only
   */ 
  sInst->salldInst.stateBitfield = SALLD_SWIZ(inst->salldInst.stateBitfield); 
  sTxInst = &sInst->txInst;
  sRxInst = &sInst->rxInst;
  
  /* Update Tx parameters */
  sTxInst->cipherMode   = SALLD_SWIZ(txInst->cipherMode);
  sTxInst->authMode     = SALLD_SWIZ(txInst->authMode);
  sTxInst->lastSeqNum   = SALLD_SWIZ(txInst->lastSeqNum);
  sTxInst->packetEncLsw = SALLD_SWIZ(txInst->packetEncLsw);
  sTxInst->packetEncMsw = SALLD_SWIZ(txInst->packetEncMsw);
  
  salld_update_shadow_scInfo(&txInst->scInfo, &sTxInst->scInfo);
  salld_update_shadow_swInfo(&txInst->swInfo, &sTxInst->swInfo);
  salld_update_shadow_destInfo(&txInst->destInfo, &sTxInst->destInfo);
  salld_ipsec_update_shadow_comInfo(&txInst->comInfo, &sTxInst->comInfo);
  
  /* Update Rx parameters */
  sRxInst->cipherMode   = SALLD_SWIZ(rxInst->cipherMode);
  sRxInst->authMode     = SALLD_SWIZ(rxInst->authMode);
  sRxInst->lastSeqNum   = SALLD_SWIZ(rxInst->lastSeqNum);
  sRxInst->packetDecLsw = SALLD_SWIZ(rxInst->packetDecLsw);
  sRxInst->packetDecMsw = SALLD_SWIZ(rxInst->packetDecMsw);
  sRxInst->paddingFail  = SALLD_SWIZ(rxInst->paddingFail);
  sRxInst->windowCheck  = SALLD_SWIZ(rxInst->windowCheck);
  
  salld_update_shadow_scInfo(&rxInst->scInfo, &sRxInst->scInfo);
  salld_update_shadow_swInfo(&rxInst->swInfo, &sRxInst->swInfo);
  salld_update_shadow_destInfo(&rxInst->destInfo, &sRxInst->destInfo);
  salld_ipsec_update_shadow_comInfo(&rxInst->comInfo, &sRxInst->comInfo);
  
}                                   
 
/****************************************************************************
 * FUNCTION PURPOSE: IPSEC Control 
 ****************************************************************************
 * DESCRIPTION: IPSEC Control functions
 *
 * int16_t  salld_ipsec_control (
 *   void  *salldInst  - a pointer to SALLD channel instance
 *   void  *ctrl)      - a pointer to control structure
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *
 ***************************************************************************/
int16_t salld_ipsec_control (void *salldInst, void *salldCtrl)
{
  salldIpsecInst_t *inst = (salldIpsecInst_t *)salldInst;
  salldObj_t *sysInst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->salldInst.ownerInstOffset);
  salldIpsecTxInst_t *txInst = (salldIpsecTxInst_t *) &inst->txInst;
  salldIpsecRxInst_t *rxInst = (salldIpsecRxInst_t *) &inst->rxInst;
  Sa_ChanCtrlInfo_t *ctrl = (Sa_ChanCtrlInfo_t *)salldCtrl;
  uint16_t bitmap;
  int16_t ret;
  uint16_t  espMode = (salld_callTblPtr[inst->salldInst.protoTblIndex]->secProtocolType == sa_PT_IPSEC_ESP);
  int fUpdateShadow = FALSE;

  switch(ctrl->ctrlType)
  {
    /* SALLD cipher mac and key size selection */
    case sa_CHAN_CTRL_GEN_CONFIG:
    {
        bitmap = ctrl->ctrlInfo.gen.validBitfield;
        if( bitmap & sa_CONTROLINFO_VALID_TX_CTRL)
        {
            /* Input parameters check */
            Sa_GenConfigParams_t*   pGenConfig =  &ctrl->ctrlInfo.gen.txCtrl;
            Sa_IpsecConfigParams_t* pIpsecConfig = &ctrl->ctrlInfo.gen.txCtrl.params.ipsec;
            
            if (salld_ipsec_verify_config_params(pGenConfig->cipherMode, pGenConfig->authMode, pIpsecConfig))
            {
                txInst->cipherMode = pGenConfig->cipherMode;
                txInst->authMode   = pGenConfig->authMode;
                txInst->destInfo   = pGenConfig->destInfo; 
                txInst->comInfo.config = *pIpsecConfig;
                txInst->comInfo.esn = pIpsecConfig->esnHi;
            }
            else
            {
                return (sa_ERR_PARAMS);
            }
                
        }
        if(bitmap & sa_CONTROLINFO_VALID_RX_CTRL)
        {
            /* Input parameters check */
            Sa_GenConfigParams_t*  pGenConfig =  &ctrl->ctrlInfo.gen.rxCtrl;
            Sa_IpsecConfigParams_t* pIpsecConfig = &ctrl->ctrlInfo.gen.rxCtrl.params.ipsec;
            
            if (salld_ipsec_verify_config_params(pGenConfig->cipherMode, pGenConfig->authMode, pIpsecConfig))
            {
                rxInst->cipherMode = pGenConfig->cipherMode;
                rxInst->authMode   = pGenConfig->authMode;
                rxInst->destInfo   = pGenConfig->destInfo; 
                rxInst->comInfo.config = *pIpsecConfig;
                rxInst->comInfo.esn = pIpsecConfig->esnHi;
            }
            else
            {
                return (sa_ERR_PARAMS);
            }
        }
        
        if (bitmap & sa_CONTROLINFO_VALID_REPLAY_WIN)
        {
            if (SALLD_TEST_SASS_GEN2(sysInst))
            {
                if (ctrl->ctrlInfo.gen.replayWindowSize > SALLD_IPSEC_MAX_REPLAY_WINDOW_SIZE)
                    return (sa_ERR_PARAMS);    
            }  
            else if (ctrl->ctrlInfo.gen.replayWindowSize > SALLD_IPSEC_MAX_REPLAY_WINDOW_SIZE_SHORT_CTX)
            {   
                return (sa_ERR_PARAMS);
            }
            rxInst->windowCheck = ctrl->ctrlInfo.gen.replayWindowSize;
        }                      
        
        /*
         * Is it time to form and register security context?
         */
        if(!SALLD_TEST_STATE_TX_SC_VALID(&inst->salldInst) && 
            SALLD_TEST_STATE_TX_ON(&inst->salldInst))
        {
            if ((ret = (espMode)?salld_esp_set_tx_sc(inst):salld_ah_set_tx_sc(inst)) != sa_ERR_OK)
            {
                return(ret);
            }
            
            SALLD_SET_STATE_TX_SC_VALID(&inst->salldInst, 1);
            
            fUpdateShadow = TRUE; /* It is time to update the shadow instance */
            
            
        } 
        
        if(!SALLD_TEST_STATE_RX_SC_VALID(&inst->salldInst) && 
            SALLD_TEST_STATE_RX_ON(&inst->salldInst))
        {
            if ((ret = (espMode)?salld_esp_set_rx_sc(inst):salld_ah_set_rx_sc(inst)) != sa_ERR_OK)
            {
                return(ret);
            }
            
            SALLD_SET_STATE_RX_SC_VALID(&inst->salldInst, 1);
            
            /* Register the receive security context */
            salldLObj.callOutFuncs.ChanRegister((Sa_ChanHandle) sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,salldInst), &inst->rxInst.swInfo);
            
            fUpdateShadow = TRUE; /* It is time to update the shadow instance */
        } 
        
    }    
    break;

    /* Master key and Salt setup for ipsec */
    case sa_CHAN_CTRL_KEY_CONFIG:
    {
        bitmap = ctrl->ctrlInfo.key.ctrlBitfield;
        
        if (bitmap & sa_KEY_CONTROL_TX_KEY_VALID)
        {
            Sa_IpsecKeyParams_t* pKeyParams = &ctrl->ctrlInfo.key.txKey.ipsec;
            salldIpsecComInfo_t* pComInfo = &txInst->comInfo;
            
            if(!salld_ipsec_setup_key(inst, txInst->cipherMode, pComInfo, pKeyParams))
                return(sa_ERR_PARAMS);
        }    

        if (bitmap & sa_KEY_CONTROL_RX_KEY_VALID)
        {
            Sa_IpsecKeyParams_t* pKeyParams = &ctrl->ctrlInfo.key.rxKey.ipsec;
            salldIpsecComInfo_t* pComInfo = &rxInst->comInfo;
            
            if(!salld_ipsec_setup_key(inst, rxInst->cipherMode, pComInfo, pKeyParams))
                return(sa_ERR_PARAMS);
        }    
    }   
    break;
    
    default:
        return (sa_ERR_PARAMS);

  }
  
  if (fUpdateShadow && inst->salldInst.shadowInstOffset)
  {
    salld_ipsec_update_shadowInst(inst);  
  }  
  
  return (sa_ERR_OK);
}

/****************************************************************************
 * FUNCTION PURPOSE: Extracts Statistics Info from the Security Contexts
 ****************************************************************************
 * DESCRIPTION: Extracts Statistics Info from the Security Contexts 
 *
 *  void salld_ipsec_sc_extract_stats(
 *            salldIpsecTxInst_t *txInst,       
 *            salldIpsecRxInst_t *rxInst,
 *            salldComStats_t    *pErrStats,
 *            uint8_t            *txScBuf,
 *            uint8_t            *rxScBuf,
 *            Sa_Replay_Cxt_t    *replayCxt) 
 *                                               
 * Return values:  
 *                          
 *
 ***************************************************************************/
static void salld_ipsec_sc_extract_stats( 
                        salldIpsecTxInst_t *txInst,    
                        salldIpsecRxInst_t *rxInst,
                        salldComStats_t    *pErrStats,
                        uint8_t            *txScBuf,
                        uint8_t            *rxScBuf,
                        Sa_Replay_Cxt_t    *replayCxt
                        ) 
{
    uint8_t data[96];
    
    tword* ctxIn = data + SALLD_BYTE_TO_WORD(SA_CTX_PHP_COMMON_SIZE);
    
    if (txScBuf)
    {
        /* Security Context swizzling */
        salld_swiz_128(txScBuf, data, 64);
    
        txInst->packetEncMsw = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, numTxPktsHi)); 
        txInst->packetEncLsw = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, numTxPkts)); 
        txInst->packetRollover = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, rollOverCounter));
        txInst->comInfo.esn  = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, esn) + sizeof(uint32_t));
        txInst->comInfo.sn   = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, esn));
        txInst->byteCount    = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, byteCount)); 
        txInst->byteCountHi  = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, byteCountHi));
    }

    if (rxScBuf)
    {
        int index, bitOff, i,j;
        int winSize32 = SALLD_DIV_ROUND_UP(rxInst->windowCheck, 32);
        int winMaskSize = (rxInst->windowCheck > SA_CTX_MAX_REPLAY_WINDOW_SIZE)?SA_CTX_WIN_MASK_SIZE2:SA_CTX_WIN_MASK_SIZE; 
        
        uint32_t bitMask[SA_CTX_WIN_MASK_SIZE2];
        /* Security Context swizzling */
        salld_swiz_128(rxScBuf, data, 96);

        rxInst->packetDecMsw = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, numRxPktsHi)); 
        rxInst->packetDecLsw = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, numRxPkts)); 
        replayCxt->winBaseHi = rxInst->comInfo.esn  = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winBaseHi));
        replayCxt->winBase   = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winBase));
        rxInst->byteCount    = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, byteCount)); 
        rxInst->byteCountHi  = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, byteCountHi));
        pErrStats->replayOld = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, numOldPkts));
        pErrStats->replayDup = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, numDupPkts));
        pErrStats->authFail  = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, numHashFails));
        
        index  = pktRead8bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winMaskIndexBitoff)) >> 2; /* index in bytes */
        bitOff = pktRead8bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winMaskIndexBitoff) + 1); 
        
        for(i = 0; i < SA_CTX_WIN_MASK_SIZE; i++)
            bitMask[i] = pktRead32bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winMask) + i*4);
            
        if (rxInst->windowCheck > SA_CTX_MAX_REPLAY_WINDOW_SIZE)
        {
            uint8_t data2[128-16];
        
            /* Security Context swizzling */
            salld_swiz_128(rxScBuf+128, data2, 128 - 16);
            
            /* read the second group of bitMask */
            for(j = 0; i < SA_CTX_WIN_MASK_SIZE2; i++, j+=4)
                bitMask[i] = pktRead32bits_m(data2, j);
        }     
        
        for (i = 0; i < winSize32; i++)
        {
            replayCxt->winMask[i] = (bitMask[index]  >> (bitOff));
            
            if (index == (winMaskSize - 1))
            {
                replayCxt->winMask[i] |= (bitMask[0] << (32-bitOff));
                index = 0;   
            }
            else
            {
                replayCxt->winMask[i] |= (bitMask[++index] << (32-bitOff));
            }
            
        }
    }
}                        

/******************************************************************************
 * FUNCTION PURPOSE: IPSEC Get Stats
 ******************************************************************************
 * DESCRIPTION: Extract IPSEC related statistics from the instance structure
 *
 * void salld_ipsec_get_stats (
 *    void   *salldInst,       - A pointer to SALLD instance
 *    uint16_t flags,            - various control flags
 *    void   *stats)           - The stat structure
 *
 * Return values:  sa_ERR_OK
 *
 *****************************************************************************/
int16_t salld_ipsec_get_stats (void *salldInst, uint16_t flags, void *stats)
{
    Sa_IpsecStats_t  *pStats = (Sa_IpsecStats_t *) stats;
    salldIpsecInst_t   *inst   = (salldIpsecInst_t *) salldInst; 
    salldIpsecTxInst_t *txInst = (salldIpsecTxInst_t *) &inst->txInst;
    salldIpsecRxInst_t *rxInst = (salldIpsecRxInst_t *) &inst->rxInst;
    uint8_t *txScBuf = (uint8_t*) sa_CONV_OFFSET_TO_ADDR(salldLObj.scPoolBaseAddr, txInst->scInfo.scBuf);
    uint8_t *rxScBuf = (uint8_t*) sa_CONV_OFFSET_TO_ADDR(salldLObj.scPoolBaseAddr, rxInst->scInfo.scBuf);
    Sa_Replay_Cxt_t   replayCxt;
    int16_t retCode = sa_ERR_OK;    
    uint32_t key;
	uint32_t tx_valid, rx_valid;
    
	tx_valid = SALLD_TEST_STATE_TX_SC_VALID(&inst->salldInst);
	rx_valid = SALLD_TEST_STATE_RX_SC_VALID(&inst->salldInst);	
    
    /* Initialize a statistics update */
    Sa_osalMtCsEnter(&key);
    
    if(tx_valid)Sa_osalBeginScAccess((void *)txScBuf, SA_CTX_PHP_IPSEC_TX_TYPE1_SIZE);
    if(rx_valid)Sa_osalBeginScAccess((void *)rxScBuf, SA_CTX_PHP_IPSEC_RX_TYPE1_SIZE);
  
    if (flags & sa_STATS_QUERY_FLAG_TRIG)
    {
        if (tx_valid && salld_is_sc_updated(txScBuf))
        {
            /* ask for a new update */
            salld_sc_set_wait_update(txScBuf);
            Sa_osalEndScAccess((void *)txScBuf, SA_CTX_PHP_IPSEC_TX_TYPE1_SIZE);
            salld_send_null_pkt(salldInst, NULL, &txInst->swInfo, SA_SC_FLAGS_EVICT);
             
        } 
        
        if (rx_valid && salld_is_sc_updated(rxScBuf))
        {
            /* ask for a new update */
            salld_sc_set_wait_update(rxScBuf);
            Sa_osalEndScAccess((void *)rxScBuf, SA_CTX_PHP_IPSEC_RX_TYPE1_SIZE);
            
            salld_send_null_pkt(salldInst, NULL, &rxInst->swInfo, SA_SC_FLAGS_EVICT);
        }
        
        retCode = sa_ERR_STATS_UNAVAIL;
    }
    else
    {
        if ((flags & sa_STATS_QUERY_FLAG_NOW) ||
           ((!tx_valid || salld_is_sc_updated(txScBuf)) && (!rx_valid || salld_is_sc_updated(rxScBuf))))
        {
            memset(&replayCxt, 0, sizeof(Sa_Replay_Cxt_t));
			if (!rx_valid)
				rxScBuf = NULL;
			if (!tx_valid)
				txScBuf = NULL;

			salld_ipsec_sc_extract_stats(txInst, rxInst, &inst->salldInst.stats, txScBuf, rxScBuf, &replayCxt);
        }
        else
        {
            retCode = sa_ERR_STATS_UNAVAIL;
        }
    }
    
    Sa_osalMtCsExit(key);

    if (retCode == sa_ERR_OK)
    {
        pStats->replayOld = inst->salldInst.stats.replayOld;
        pStats->replayDup = inst->salldInst.stats.replayDup;
        pStats->authFail  = inst->salldInst.stats.authFail;
        pStats->txRollover = txInst->packetRollover;
        pStats->pktEncHi  = txInst->packetEncMsw;
        pStats->pktEncLo  = txInst->packetEncLsw;
        pStats->pktDecHi  = rxInst->packetDecMsw;
        pStats->pktDecLo  = rxInst->packetDecLsw;
        pStats->txESN     = txInst->comInfo.esn;
        pStats->txSN      = txInst->comInfo.sn;
        pStats->rxESN     = rxInst->comInfo.esn;
        pStats->txByteCountHi = txInst->byteCountHi;
        pStats->txByteCountLo = txInst->byteCount;
        pStats->rxByteCountHi = rxInst->byteCountHi;
        pStats->rxByteCountLo = rxInst->byteCount;

        pStats->replayCxt = replayCxt;
    }
    
    return (retCode);
}

/****************************************************************************
 * FUNCTION PURPOSE: IPSEC Get SwInfo 
 ****************************************************************************
 * DESCRIPTION: IPSEC Get SwInfo
 *
 * int16_t  salld_ipsec_get_swInfo (
 *   Sa_ChanHandle        handle      - SALLD channel identifier
 *   uint16_t             dir         - packet directions
 *   Sa_SWInfo_t         *pSwInfo)    - a pointer to swInfo
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *
 ***************************************************************************/
int16_t salld_ipsec_get_swInfo (void *salldInst, uint16_t dir, Sa_SWInfo_t* pChanSwInfo)
{
  salldIpsecInst_t *inst = (salldIpsecInst_t *)salldInst;
  int16_t ret = sa_ERR_OK;
  
  if (dir == sa_PKT_DIR_FROM_NETWORK)
  {
    salldIpsecRxInst_t *rxInst = (salldIpsecRxInst_t *) &inst->rxInst;
    memcpy(pChanSwInfo, &rxInst->swInfo, sizeof(Sa_SWInfo_t));
    
  }
  else if (dir == sa_PKT_DIR_TO_NETWORK)
  {
    salldIpsecTxInst_t *txInst = (salldIpsecTxInst_t *) &inst->txInst;
    memcpy(pChanSwInfo, &txInst->swInfo, sizeof(Sa_SWInfo_t));
  }
  else
  {
    ret = sa_ERR_PARAMS;
  }
  
  return(ret);
}


/****************************************************************************
 * FUNCTION PURPOSE:   IPSEC function call table 
 ****************************************************************************
 * DESCRIPTION:     The tables are used to link IPSEC functions to the 
 *                  SALLD library if required 
 *
 ***************************************************************************/
Sa_ProtocolCallTbl_t Sa_callTblIpsecAh = 
{
  sa_PT_IPSEC_AH,
  salld_ipsec_init,
  salld_ipsec_control,
  salld_ipsec_get_stats,
  salld_ah_send_data,
  salld_ah_receive_data,
  salld_ipsec_close,
  salld_ipsec_get_swInfo
};

Sa_ProtocolCallTbl_t Sa_callTblIpsecEsp = 
{
  sa_PT_IPSEC_ESP,
  salld_ipsec_init,
  salld_ipsec_control,
  salld_ipsec_get_stats,
  salld_esp_send_data,
  salld_esp_receive_data,
  salld_ipsec_close,
  salld_ipsec_get_swInfo
};

   
/* Nothing past this point */

