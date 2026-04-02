/******************************************************************************
 * FILE PURPOSE: Secure RTP Security Context File
 ******************************************************************************
 * FILE NAME: salldsrtpsc.c
 *
 * DESCRIPTION: The main module for Secure RTP Security Context related Code
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
/* RTSC header files */ 

/* Standard include files */
#include <string.h>

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include <ti/drv/sa/sa_osal.h>
#include "src/salldloc.h"
#include "src/salldport.h"
#include "src/salldctx.h"
#include "salldsrtp.h"
#include "salldsrtploc.h"

/****************************************************************************
 * FUNCTION PURPOSE: Construct SRTP Tx Security Context
 ****************************************************************************
 * DESCRIPTION: Construct Security Context from the configuration parameters 
 *          for Tx operations
 *
 *  int16_t salld_srtp_set_tx_sc(
 *            salldSrtpInst_t*  inst      -> Point to SRTP channel instance
 *            uint16_t              reKey)    -> Set SC for Rekey operation           
 * Return values:  
 *                          
 *
 ***************************************************************************/
int16_t salld_srtp_set_tx_sc(salldSrtpInst_t *inst, uint16_t rekey) 
{
  salldSrtpTxInst_t *txInst = (salldSrtpTxInst_t *) &inst->txInst;
  salldSrtpKeyInfo_t* pKeyInfo = &txInst->keyInfo;
  salldSrtpKeySizeParams_t*  pKeySize = &pKeyInfo->keySize;
  Sa_ScReqInfo_t* pScInfo = (rekey)?&txInst->scInfo[SRTP_PENDING_SC_INDEX]:
                                    &txInst->scInfo[SRTP_ACTIVE_SC_INDEX];
  int16_t encScSize = (txInst->cipherMode == sa_CipherMode_NULL)?0:SA_CTX_ENC_TYPE1_SIZE;
  int16_t authScSize = (txInst->authMode == sa_AuthMode_NULL)?0:SA_CTX_AUTH_TYPE2_SIZE;
  int16_t cmdlSize = 0;
  uint16_t ctrlBitfield = 0;
  uint8_t firstEngId = SALLD_CMDL_ENGINE_ID_AS1;
  
  saDMAReqInfo_t dmaReqInfo;
  
  /*
   * Calculate the security Context Size and allocate security Context
   *
   */
  pScInfo->scSize = SA_CTX_PHP_SRTP_TX_SIZE + encScSize + authScSize;
  salldLObj.callOutFuncs.ScAlloc((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo);
  
  if(pScInfo->scBuf == (uintptr_t)NULL)
    return(sa_ERR_NO_CTX_BUF);
  
  Sa_osalBeginScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  memset((void *)pScInfo->scBuf, 0, SALLD_BYTE_TO_WORD(pScInfo->scSize));
  
  /* Generate the session key */
  /* Clear Flag for non-FromTo Key */
  if(!(pKeyInfo->kdBitfield & SALLD_SRTP_FROM_TO_MASK))
  {
        /* If new Master Key/Salt is present reset the bit and flag
           the first key_derivation */
        if(pKeyInfo->kdBitfield & SALLD_SRTP_NEW_KEY_MASK)
        {
            pKeyInfo->kdBitfield ^= SALLD_SRTP_NEW_KEY_MASK;
            pKeyInfo->kdBitfield |= SALLD_SRTP_FIRST_KD_MASK;
        }
  }
  else
  {
  
        salldSrtpFromTo_t* pFromTo = &pKeyInfo->fromToBuffer; 
    
        /* From-to key */
        if(pFromTo->kdBitfield & SALLD_SRTP_NEW_KEY_MASK)
        {
            srtp_update_key(pKeyInfo);
            pKeyInfo->kdBitfield |= SALLD_SRTP_FIRST_KD_MASK;
            pKeyInfo->kdBitfield &= ~SALLD_SRTP_KEY_REQUEST_MASK;
        }
  }
  
  
  if ((!rekey) && !salld_srtp_derive_keys(pKeyInfo, txInst->lastSeqNum, pKeyInfo->roc))
  {
    /* Not able to derive the session keys */
    return(sa_ERR_PARAMS);
  }
  
  
  /* Prepare PHP Security Context */
  memset(&dmaReqInfo, 0, sizeof(saDMAReqInfo_t));
  dmaReqInfo.phpFetchSize = SA_CTX_DMA_SIZE_64;
  dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(encScSize);
  dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
  dmaReqInfo.phpEvictSize = SA_CTX_DMA_SIZE_64;

  salld_set_sc_phpCommom(&dmaReqInfo, &txInst->destInfo, SA_CTX_PKT_TYPE_SRTP,
                      pScInfo->scID, (tword *) pScInfo->scBuf);   
                      
  /* Prepare Security Context for the encryption Engine */
  if (txInst->cipherMode != sa_CipherMode_NULL)
  {
    salld_set_sc_enc(sa_PT_SRTP, txInst->cipherMode, pKeySize->sessionEncKeySize, 
                  pKeyInfo->sessionEncKey, 0, TRUE,
                  (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_SRTP_TX_SIZE));
    cmdlSize += (SALLD_CMDL_HEADER_SIZE_BYTES + SA_SRTP_IV_SIZE);   
    firstEngId = SALLD_CMDL_ENGINE_ID_ES1;
    SA_CTX_PROTO_SRTP_TX_SET_ENC_FLAG(ctrlBitfield, 1);           
  }
  
  /* Prepare Security Context for the authentication Engine */
  if (txInst->authMode != sa_AuthMode_NULL)
  {
    salld_set_sc_auth(txInst->authMode, pKeySize->sessionMacKeySize, 
                   pKeyInfo->sessionAuthKey, 
                   (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_SRTP_TX_SIZE + encScSize));
    cmdlSize += SALLD_CMDL_HEADER_SIZE_BYTES;              
                   
  }
  
  if (!cmdlSize)
  {
    firstEngId = SALLD_CMDL_ENGINE_SRTP_AC_HPS2;  
  }                     
                      
  /* Construct the SRTP Tx specific Security Context */    
  {
    tword* ctxIn = (tword *) (pScInfo->scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_COMMON_SIZE));
  
    SA_CTX_PROTO_SRTP_TX_SET_ENCRYPT_MODE(ctrlBitfield, txInst->cipherMode);
    if (pKeyInfo->mkiLength)
    {
        SA_CTX_PROTO_SRTP_TX_SET_MKI(ctrlBitfield, 1);    
        if (pKeyInfo->mkiLength == 2)
        {
            pktWrite16bits_m(ctxIn, SALLD_BYTE_TO_WORD(SALLD_FIELDOFFSET(saCtxProtoSrtpTx_t, mki)),
                            (uint16_t)pKeyInfo->mki); 
        }
        else
        {
            pktWrite32bits_m(ctxIn, SALLD_BYTE_TO_WORD(SALLD_FIELDOFFSET(saCtxProtoSrtpTx_t, mki)),
                             pKeyInfo->mki); 
        }
    }    
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpTx_t, numTxPktsLo), txInst->packetEncLsw);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpTx_t, numTxPktsHi), txInst->packetEncMsw);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpTx_t, ctrlBitfield), ctrlBitfield);
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpTx_t, roc), pKeyInfo->roc);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpTx_t, icvMkiSize), 
                     SALLD_MK_UINT16(pKeySize->macSize, pKeyInfo->mkiLength));
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpTx_t, firstEngIdCmdlLen), 
                     SALLD_MK_UINT16(firstEngId, cmdlSize));
    misc_utlCopy(pKeyInfo->sessionSalt,
                 (uint16_t *)(ctxIn + SALLD_BYTE_TO_WORD(SALLD_FIELDOFFSET(saCtxProtoSrtpTx_t, saltKey))),  
                 pKeySize->sessionSaltSize/2); 
    pktWrite8bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpTx_t, saltKeySize), 
                    pKeySize->sessionSaltSize);
  }
  
  
  /* Security Context swizzling */
  salld_swiz_128((uint8_t *) pScInfo->scBuf, (uint8_t *)pScInfo->scBuf, pScInfo->scSize);
  Sa_osalEndScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  
  /* Prepare the SW Info Words */
  salld_set_swInfo(SALLD_CMDL_ENGINE_SRTP_AC_HPS1, 0, NULL,
                   pScInfo, &txInst->swInfo, 0);

  /* Store the scBuf internally as offset to suppor multiprocess */
  pScInfo->scBuf = (uintptr_t) sa_CONV_ADDR_TO_OFFSET(salldLObj.scPoolBaseAddr, pScInfo->scBuf);
  
  return (sa_ERR_OK);

}            


/****************************************************************************
 * FUNCTION PURPOSE: Construct SRTP Rx Security Context
 ****************************************************************************
 * DESCRIPTION: Construct Security Context from the configuration parameters 
 *          for Rx operations
 *
 *  int16_t salld_srtp_set_rx_sc(
 *            salldSrtpInst_t*      inst        -> Point to SRTP channel instance
 *            uint16_t                  rekey)      -> Set SC for Rekey operation           
 *                       
 * Return values:  
 *                          
 *
 ***************************************************************************/
int16_t salld_srtp_set_rx_sc(salldSrtpInst_t *inst, uint16_t rekey) 
{
  salldSrtpRxInst_t *rxInst =  &inst->rxInst;
  salldWindow_t *pReplayCtrl = &rxInst->replayWindow;
  salldSrtpKeyInfo_t* pKeyInfo = &rxInst->keyInfo;
  salldSrtpKeySizeParams_t*  pKeySize = &pKeyInfo->keySize;
  Sa_ScReqInfo_t* pScInfo = (rekey)?&rxInst->scInfo[SRTP_PENDING_SC_INDEX]:
                                    &rxInst->scInfo[SRTP_ACTIVE_SC_INDEX]  ;
  int16_t encScSize = (rxInst->cipherMode == sa_CipherMode_NULL)?0:SA_CTX_ENC_TYPE1_SIZE;
  int16_t authScSize = (rxInst->authMode == sa_AuthMode_NULL)?0:SA_CTX_AUTH_TYPE2_SIZE;
  int16_t cmdlSize = 0;
  uint16_t ctrlBitfield = 0;
  uint8_t firstEngId = SALLD_CMDL_ENGINE_ID_AS1;
  saDMAReqInfo_t dmaReqInfo;
  int i;
  
  /*
   * Calculate the security Context Size and allocate security Context
   *
   */
  pScInfo->scSize = SA_CTX_PHP_SRTP_RX_SIZE + encScSize + authScSize;
  salldLObj.callOutFuncs.ScAlloc((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo);
  
  if(pScInfo->scBuf == (uintptr_t) NULL)
    return(sa_ERR_NO_CTX_BUF);
  
  Sa_osalBeginScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  memset((void *) pScInfo->scBuf, 0, SALLD_BYTE_TO_WORD(pScInfo->scSize));
  
  /* Generate the session key */
  /* Clear Flag for non-FromTo Key */
  if(!(pKeyInfo->kdBitfield & SALLD_SRTP_FROM_TO_MASK))
  {
        /* If new Master Key/Salt is present reset the bit and flag
        the first key_derivation */
        if(pKeyInfo->kdBitfield & SALLD_SRTP_NEW_KEY_MASK)
        {
            pKeyInfo->kdBitfield ^= SALLD_SRTP_NEW_KEY_MASK;
            pKeyInfo->kdBitfield |= SALLD_SRTP_FIRST_KD_MASK;
        }
  }
  else
  {
        salldSrtpFromTo_t* pFromTo = &pKeyInfo->fromToBuffer; 
    
        /* From-to key */
        if(pFromTo->kdBitfield & SALLD_SRTP_NEW_KEY_MASK)
        {
            srtp_update_key(pKeyInfo);
            pKeyInfo->kdBitfield |= SALLD_SRTP_FIRST_KD_MASK;
            pKeyInfo->kdBitfield &= ~SALLD_SRTP_KEY_REQUEST_MASK;
        }
  }
  
  
  if ((!rekey) && (!salld_srtp_derive_keys(pKeyInfo, rxInst->lastSeqNum, pKeyInfo->roc)))
  {
    /* Not able to derive the session keys */
    return(sa_ERR_PARAMS);
  }
  
  /* Prepare PHP Security Context */
  memset(&dmaReqInfo, 0, sizeof(saDMAReqInfo_t));
  dmaReqInfo.phpFetchSize = SA_CTX_DMA_SIZE_128;
  dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(encScSize);
  dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
  dmaReqInfo.phpEvictSize = SA_CTX_DMA_SIZE_128;

  salld_set_sc_phpCommom(&dmaReqInfo, &rxInst->destInfo, SA_CTX_PKT_TYPE_SRTP | SA_CTX_PKT_DIR_RX,
                        pScInfo->scID, (tword *) pScInfo->scBuf); 
                        
  /* Prepare Security Context for the encryption Engine */
  if (rxInst->cipherMode != sa_CipherMode_NULL)
  {
    salld_set_sc_enc(sa_PT_SRTP, rxInst->cipherMode, pKeySize->sessionEncKeySize, 
                  pKeyInfo->sessionEncKey, 0, FALSE, 
                  (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_SRTP_RX_SIZE));
    cmdlSize += (SALLD_CMDL_HEADER_SIZE_BYTES + SA_SRTP_IV_SIZE);   
    SA_CTX_PROTO_SRTP_RX_SET_ENC_FLAG(ctrlBitfield, 1);           
  }
  
  /* Prepare Security Context for the authentication Engine */
  if (rxInst->authMode != sa_AuthMode_NULL)
  {
    salld_set_sc_auth(rxInst->authMode, pKeySize->sessionMacKeySize, 
                  pKeyInfo->sessionAuthKey, 
                  (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_SRTP_RX_SIZE + encScSize));
    cmdlSize += SALLD_CMDL_HEADER_SIZE_BYTES;              
  }
  else
  {
    firstEngId = SALLD_CMDL_ENGINE_ID_ES1;
  }
  
  if (!cmdlSize)
  {
    firstEngId = SALLD_CMDL_ENGINE_SRTP_AC_HPS2;  
  }                     
                      
                      
  /* Construct the SRTP Rx specific Security Context */    
  {
    tword* ctxIn = (tword *) (pScInfo->scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_COMMON_SIZE));
  
    SA_CTX_PROTO_SRTP_RX_SET_ENCRYPT_MODE(ctrlBitfield, rxInst->cipherMode);
    /* The replay update operation should take care of the first packet natually */
    if (!rekey)
    {
        SA_CTX_PROTO_SRTP_RX_SET_FIRST_PKT(ctrlBitfield, 1);
    }
    
    if (pKeyInfo->mkiLength)
    {
        SA_CTX_PROTO_SRTP_RX_SET_MKI(ctrlBitfield, 1);    
        if (pKeyInfo->mkiLength == 2)
        {
            pktWrite16bits_m(ctxIn, SALLD_BYTE_TO_WORD(SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, mki)),
                            (uint16_t)pKeyInfo->mki); 
        }
        else
        {
            pktWrite32bits_m(ctxIn, SALLD_BYTE_TO_WORD(SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, mki)),
                             pKeyInfo->mki); 
        }
    }    
        
    if(pKeyInfo->kdBitfield & SALLD_SRTP_FROM_TO_MASK)
    {
        SA_CTX_PROTO_SRTP_RX_SET_FROMTO(ctrlBitfield, 1);
        /* FromTo related parameters */
        pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, fromIndexHi), 
                        pKeyInfo->fromMsb);
        pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, fromIndexLo), 
                        pKeyInfo->fromLsb);
        pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, toIndexHi), 
                        pKeyInfo->toMsb);
        pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, toIndexLo), 
                        pKeyInfo->toLsb);
    }    
    else
    {
        pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, keyLifetimeLo), 
                        pKeyInfo->keyLifetimeLsb);
        pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, keyLifetimeHi), 
                        pKeyInfo->keyLifetimeMsb);
    }
    
    /* Key derivation parameters */
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, keyDerivRemLo), 
                     pKeyInfo->rLsb);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, keyDerivRemHi), 
                     pKeyInfo->rMsb);
                     
    pktWrite8bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, keyDerivRate), 
                    (pKeyInfo->kdBitfield & SALLD_SRTP_KD_RATE_MASK));
    
    
    if (rxInst->windowCheck) 
    {
        SA_CTX_PROTO_SRTP_RX_SET_REPLAY(ctrlBitfield, 1); 
        
        /* Initialize the replay Control Block */
        pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winSize), rxInst->windowCheck);
        pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winBaseHi), 
                         (pKeyInfo->roc >> 16));
        if (rekey)
        {
            pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winBase), pReplayCtrl->winBase);
            pktWrite8bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winMaskIndexBitoff), pReplayCtrl->winMaskIndex*4);
            pktWrite8bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winMaskIndexBitoff) + 1, pReplayCtrl->winMaskBitoff);
            
            for ( i = 0; i < SA_CTX_WIN_MASK_SIZE; i++)
            {
                pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winMask) + 4*i, pReplayCtrl->winMask[i]);
            }
        }   
        else
        {
            pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winBase), 
                            (pKeyInfo->roc << 16));
        
        }              
                         
    }            
  
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, numRxPktsLo), rxInst->packetDecLsw);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, numRxPktsHi), rxInst->packetDecMsw);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, ctrlBitfield), ctrlBitfield);
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, roc), pKeyInfo->roc);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, icvMkiSize), 
                     SALLD_MK_UINT16(pKeySize->macSize, pKeyInfo->mkiLength));
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, firstEngIdCmdlLen), 
                     SALLD_MK_UINT16(firstEngId, cmdlSize));
    misc_utlCopy(pKeyInfo->sessionSalt,
                 (uint16_t *)(ctxIn + SALLD_BYTE_TO_WORD(SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, salt))),  
                 pKeySize->sessionSaltSize/2); 
    pktWrite8bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoSrtpRx_t, saltSize), 
                    pKeySize->sessionSaltSize);
  }
  
  
  /* Security Context swizzling */
  salld_swiz_128((uint8_t *) pScInfo->scBuf, (uint8_t *)pScInfo->scBuf, pScInfo->scSize);
  Sa_osalEndScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  
  /* It is going to replace the old one */
  salld_set_swInfo(SALLD_CMDL_ENGINE_SRTP_AC_HPS1, 0, NULL,
                   pScInfo, &rxInst->swInfo, 0);
  
  /* Store the scBuf internally as offset to suppor multiprocess */
  pScInfo->scBuf = (uintptr_t) sa_CONV_ADDR_TO_OFFSET(salldLObj.scPoolBaseAddr, pScInfo->scBuf);
  
  return (sa_ERR_OK);

}  

/****************************************************************************
 * FUNCTION PURPOSE: SRTP Tx Re-Key Transation State Machine 
 ****************************************************************************
 * DESCRIPTION: SRTP Re-Key State machine to handle the SA Security Context
 *              Transation for Tx operations
 *
 *  int16_t salld_srtp_tx_rekey_sm(
 *            salldSrtpInst_t*      inst)       -> Point to SRTP channel instance
 *                       
 * Return values:  
 *                          
 *
 ***************************************************************************/
int16_t salld_srtp_tx_rekey_sm(salldSrtpInst_t*  inst)
{ 
  salldSrtpTxInst_t *txInst = (salldSrtpTxInst_t *) &inst->txInst;
  int16_t  ret = sa_ERR_OK;
  uint8_t* scBuf;
  
  switch (txInst->rekeyState)
  {
    case SRTP_REKEY_STATE_IDLE:
        return (sa_ERR_OK);
  
    case SRTP_REKEY_STATE_NEW_SC:
        /* 
         * It is time to format and use the new security context
         * Perform the following actions
         * - Send Null packet to free the current Security Context
         * - Format the new security context
         * - (?) register the new security context
         * - change state to SRTP_REKEY_STATE_FREE_SC
         */
        {
            /* Prepare and send Null packet */
            salld_send_null_pkt ((Sa_ChanHandle) inst, &txInst->destInfo, 
                                  &txInst->swInfo, SA_SC_FLAGS_TEAR);
            /* Generate the new security context */
            ret = salld_srtp_set_tx_sc(inst, TRUE);
            if(ret != sa_ERR_OK)
                return (ret);
                
            txInst->rekeyState = SRTP_REKEY_STATE_FREE_SC;     
        }
        break; 
         
    case SRTP_REKEY_STATE_FREE_SC:
        /* Verify whether it is time to free the current active SC? */
        {
            Sa_ScReqInfo_t* pScInfo = &txInst->scInfo[SRTP_ACTIVE_SC_INDEX];

            /* Do the conversion since we store offset for internal use to support multiprocess */
			scBuf = (uint8_t *) sa_CONV_OFFSET_TO_ADDR(salldLObj.scPoolBaseAddr,pScInfo->scBuf);
            if (Sa_isScBufFree(scBuf))
            {
                /* free the security context */
                salldLObj.callOutFuncs.ScFree((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo->scID);
            
                txInst->scInfo[SRTP_ACTIVE_SC_INDEX] = txInst->scInfo[SRTP_PENDING_SC_INDEX];
                memset(&txInst->scInfo[SRTP_PENDING_SC_INDEX], 0, sizeof(Sa_ScReqInfo_t));
                txInst->rekeyState = SRTP_REKEY_STATE_IDLE;
            }
        }
        break;
        
     default:
        return (sa_ERR_GEN);    
  }
  
  return(ret);
         
}


/****************************************************************************
 * FUNCTION PURPOSE: SRTP Rx Re-Key Transation State Machine 
 ****************************************************************************
 * DESCRIPTION: SRTP Re-Key State machine to handle the SA Security Context
 *              Transation for Tx operations
 *
 *  int16_t salld_srtp_rx_rekey_sm(
 *            salldSrtpInst_t*      inst        -> Point to SRTP channel instance
 *            uint16_t                seqNum)     -> Receive Sequence Number
 *           
 * Return values:  
 *                          
 *
 ***************************************************************************/
int16_t salld_srtp_rx_rekey_sm(salldSrtpInst_t* inst, uint32_t seqNum)
{ 
  salldSrtpRxInst_t *rxInst = (salldSrtpRxInst_t *) &inst->rxInst;
  int16_t  ret = sa_ERR_OK;
  Sa_SWInfo_t  swInfo;    /* SA-specific SW Information */
  uint8_t*   scBuf;
  
  
  switch (rxInst->rekeyState)
  {
    case SRTP_REKEY_STATE_IDLE:
        return (sa_ERR_OK);
        
        
    case SRTP_REKEY_STATE_WAIT:
        /* 
         * We enter this state only if the session key expires in SA and we may still
         * receive packets with old session key 
         */
        {
            uint16_t passThrough = FALSE;
            
            if (rxInst->windowCheck)
            {
                /* 
                 * Verify wether we have received all packets up to the specified 
                 * sequence number 
                 */ 
                passThrough = salld_replay_is_all_received(&rxInst->replayWindow, seqNum);
            }
            else
            {
                passThrough = TRUE;
            }
            
            if(!passThrough) 
                break;
            
        } 
         
            
        /* It is time to update Security Context */    
        
  
    case SRTP_REKEY_STATE_NEW_SC:
        /* 
         * It is time to format and use the new security context
         * Perform the following actions
         * - unregister the old security context
         * - Send Null packet to free the current Security Context
         * - Format the new security context
         * - register the new security context
         * - change state to SRTP_REKEY_STATE_FREE_SC
         */
        {
        
            /* Generate the new security context */
            ret = salld_srtp_set_rx_sc(inst, TRUE);
            if(ret != sa_ERR_OK)
                return (ret);
        
            /* Register the new Context Info */
            /* It is going to replace the old one */
            salld_set_swInfo(SALLD_CMDL_ENGINE_SRTP_AC_HPS1, 0, NULL,
                             &rxInst->scInfo[SRTP_ACTIVE_SC_INDEX], &swInfo, 0);
            
            salldLObj.callOutFuncs.ChanRegister((Sa_ChanHandle) sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), &rxInst->swInfo);
        
            /* Prepare and send Null packet */
            salld_send_null_pkt ((Sa_ChanHandle) inst, &rxInst->destInfo, 
                                  &swInfo, SA_SC_FLAGS_TEAR);
        
            /* Unregister the current Context Info */
            salldLObj.callOutFuncs.ChanUnRegister((Sa_ChanHandle) sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), &swInfo);
                
            rxInst->rekeyState = SRTP_REKEY_STATE_FREE_SC;     
        }
        break; 
         
    case SRTP_REKEY_STATE_FREE_SC:
        /* Verify whether it is time to free the current active SC? */
        {
            Sa_ScReqInfo_t* pScInfo = &rxInst->scInfo[SRTP_ACTIVE_SC_INDEX];
            
			/* Do the conversion since we store offset for internal use to support multiprocess */
			scBuf = (uint8_t *) sa_CONV_OFFSET_TO_ADDR(salldLObj.scPoolBaseAddr,pScInfo->scBuf);

            if (Sa_isScBufFree(scBuf))
            {
                /* free the security context */
                salldLObj.callOutFuncs.ScFree((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo->scID);
            
                rxInst->scInfo[SRTP_ACTIVE_SC_INDEX] = rxInst->scInfo[SRTP_PENDING_SC_INDEX];
                memset(&rxInst->scInfo[SRTP_PENDING_SC_INDEX], 0, sizeof(Sa_ScReqInfo_t));
                rxInst->rekeyState = SRTP_REKEY_STATE_IDLE;
            }
            
        }
        break;
        
     default:
        return (sa_ERR_GEN);    
  }
  
  return(ret);
         
}
            
/* Nothing past this point */

