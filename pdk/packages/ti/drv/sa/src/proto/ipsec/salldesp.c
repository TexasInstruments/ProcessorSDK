/******************************************************************************
 * FILE PURPOSE: IPSEC ESP Main File
 ******************************************************************************
 * FILE NAME: salldesp.c
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
#include "src/salldctx.h"
#include "src/salldproto.h"
#include "salldipsec.h"
#include "salldipsecloc.h"

#include "src/auth/salldcmac.h"
#include "src/auth/salldxcbc.h"

/******************************************************************************
 * FUNCTION PURPOSE: IPSEC ESP Send Data
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_esp_send_data (
 *    void *salldInst,      - A pointer to SALLD IPSEC instance
 *    void *pktInfo,        - packet pointer
 *    uint16_t clear)        
 *
 * here's the format of ESP
 *
 *              |<-------- ESP HEADER ------>|<--------------- ENCRYPTED PAYLOAD ------------------>|
 *  *-----------*----------------------------*-------------*---------*------------------------------*-----*
 *  * IP HEADER * SPI | SEQUENCE NUMBER | IV * IP DATAGRAM * PADDING * PADDING LENGTH | NEXT HEADER * ICV *
 *  *-----------*----------------------------*-------------*---------*------------------------------*-----*
 *                                                                   |<---------- ESP TAIL -------->|
 *              |<----------------------------- AUTHENTICATED BY ICV ------------------------------>|
 *
 *
 *  Perform the following actions:
 *   - Reserve room for the ESP Header (SPI and sequence number) following 
 *     the external IP header for Tunnel mode or the original IP header 
 *     for transport mode.
 *   - Generate and insert the Initialization Vector (IV) in front of the ESP payload  
 *   - Extract the protocol (next header) from the original IP header, and 
 *     replace it with ESP Transport (50).
 *   - Calculate the ESP padding size, insert ESP padding and the ESP Trail 
 *     (padding size + next header)
 *   - Adjust the payload length of the external IP header
 *   - Update the packet size and protocol (IP) payload size in the header 
 *     parsing information to reserve room for the ESN and authentication data 
 *     if necessary
 *   - Prepare the CPPI Software information as defined at section 5.1.12.
 *   - Update statistics
 *
 * Note: Assume that enough space is reserved in front of the segment bugffer
 *       to insert the ESP header. And enough room is reserved for the ESP 
 *       padding and hash location.
 *****************************************************************************/
int16_t salld_esp_send_data (void *salldInst, void *pktInfo, uint16_t clear) 
{
  salldIpsecInst_t *inst   = (salldIpsecInst_t *)salldInst; 
  salldIpsecTxInst_t *txInst = (salldIpsecTxInst_t *) &inst->txInst;
  salldIpsecComInfo_t* pComInfo = &txInst->comInfo;
  Sa_IpsecConfigParams_t*  pConfig = &pComInfo->config;
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;
  Sa_PktDesc_t* pPktDesc = &pPktInfo->pktDesc;
  salldIpsecEspTxRxInfo_t txInfo;
  int16_t      retval = sa_ERR_OK;

  memset(&txInfo, 0, sizeof (salldIpsecEspTxRxInfo_t));

  txInfo.encryptionBlockSize = pConfig->encryptionBlockSize;
  txInfo.ivSize              = pConfig->ivSize;
  txInfo.macSize             = pConfig->macSize;
  
  if (pPktInfo->validBitMap & sa_PKT_INFO_VALID_IPSEC_NAT_T_INFO) {
  	/* Indicate NAT_T_INFO is present in the pkt and set the bit filed */
    txInfo.validBitMap        |= sa_ESP_TXRX_VALID_IPSEC_NAT_T_INFO;
    memcpy (&txInfo.natTInfo, &pPktInfo->natTInfo, sizeof (Sa_ipsecNatTInfo_t));
  }

  retval = salld_esp_pre_proc_util(&txInfo, pPktDesc);

  if (retval != sa_ERR_OK)
  	 return (retval);
  
  /* Pass the software Info in the packet */
  pPktInfo->validBitMap |= sa_PKT_INFO_VALID_SW_INFO;
  pPktInfo->swInfo = txInst->swInfo;
  
  return(retval);
      
} /* salld_esp_send_data */

/******************************************************************************
 * FUNCTION PURPOSE: IPSEC ESP Receive Data Post Processing
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_esp_receive_data_update (
 *    salldIpsecInst_t *inst,       - A pointer to IPSEC instance
 *    Sa_PktInfo_t   *pktInfo)      - packet pointer
 *
 *****************************************************************************/
int16_t salld_esp_receive_data_update (salldIpsecInst_t *inst, Sa_PktInfo_t* pktInfo) 
{

  salldIpsecRxInst_t *rxInst = &inst->rxInst;
  salldIpsecComInfo_t* pComInfo = &rxInst->comInfo;
  Sa_IpsecConfigParams_t*  pConfig = &pComInfo->config;
  Sa_PktDesc_t* pPktDesc = &pktInfo->pktDesc;
  salldIpsecEspTxRxInfo_t rxInfo;
  int16_t      ret;

  memset(&rxInfo, 0, sizeof (salldIpsecEspTxRxInfo_t));

  rxInfo.ivSize              = pConfig->ivSize;
  rxInfo.macSize             = pConfig->macSize;  
  
  if (pktInfo->validBitMap & sa_PKT_INFO_VALID_RX_PAYLOAD_INFO)
  {
    rxInfo.validBitMap |= sa_ESP_TXRX_VALID_RX_PAYLOAD_INFO;
    rxInfo.rxPayloadInfo = pktInfo->rxPayloadInfo;
  }

  ret = salld_esp_post_proc_util (&rxInfo, pPktDesc);
  
  if (ret != sa_ERR_OK)
  {
	uint32_t mtCsKey;
	
	Sa_osalMtCsEnter(&mtCsKey);
	
	/* Invalidate the protocol-specific channel instance */
    Sa_osalBeginMemAccess(inst, sizeof(salldIpsecInst_t));  
  
	rxInst->paddingFail++;
	
    Sa_osalEndMemAccess(inst, sizeof(salldIpsecInst_t));
	
	/* Critical Section End */
	Sa_osalMtCsExit(mtCsKey);
	
  }  
  
  return (ret);

} /* salld_esp_receive_data_update */

/******************************************************************************
 * FUNCTION PURPOSE: IPSEC ESP Receive Data
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_esp_receive_data (
 *    void *salldInst,    - A pointer to SALLD instance
 *    void *pktInfo)      - packet pointer
 * 
 * Perform the following actions:
 *  - Update the corresponding statistics and return Error if the SASS packet 
 *    error occurs.
 *  - Verify the ESP padding bytes
 *  - Extract the next header from the ESP Trailer and replace the one in the 
 *    IP header with it.
 *  - Update the packet size and protocol (IP) payload size in the header 
 *    parsing information by removing the size of the authentication data, 
 *    the ESP header, padding and the ESP trailer.
 *  - Remove the ESP header, Trailer and the Authentication data
 *  - Update statistics
 *
 *****************************************************************************/
int16_t salld_esp_receive_data (void *salldInst, void *pktInfo) 
{
    
  salldIpsecInst_t* inst   = (salldIpsecInst_t *)salldInst; 
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;
  int16_t ret;
  
  switch (pPktInfo->pktErrCode)
  {
    case sa_PKT_ERR_OK:
      ret = salld_esp_receive_data_update(inst, pPktInfo);
      break;
        
    case sa_PKT_ERR_REPLAY_OLD:
       ret = sa_ERR_REPLAY_OLD;
       break;
           
    case sa_PKT_ERR_REPLAY_DUP: 
       ret = sa_ERR_REPLAY_DUP;
       break;
    
    case sa_PKT_ERR_AUTH_FAIL:
       ret = sa_ERR_AUTH_FAIL;
       break;
    
    default:
      ret = sa_ERR_GEN;
      break;
  }
  
  return (ret);
}

/****************************************************************************
 * FUNCTION PURPOSE: Derive the ESP Command Label Operation Mode
 ****************************************************************************
 * DESCRIPTION: Derive the ESP Command Label Operation Mode from the Cipher 
 *              mode and the authentication mode
 *
 *  uint8_t salld_esp_get_cmdl_opmode(
 *             int16_t cipherMode,      -> Cipher Mode
 *             int16_t authMode)        -> Authentication Mode
 *   
 * Return values:  
 *        ESP Command Label Operation Mode                  
 *
 ***************************************************************************/
static uint8_t salld_esp_get_cmdl_opmode(int16_t cipherMode, int16_t authMode) 
{
    if (cipherMode == sa_CipherMode_GCM)
    {
        return (SA_IPSEC_ESP_CMDL_MODE_GCM*2);
    }
    else if (cipherMode == sa_CipherMode_CCM)
    {
        return (SA_IPSEC_ESP_CMDL_MODE_CCM*2);
    }
    else if (authMode == sa_AuthMode_GMAC)
    {
        return (SA_IPSEC_ESP_CMDL_MODE_GMAC*2);
    }
    else
    {
        return (SA_IPSEC_ESP_CMDL_MODE_GEN*2);
    }
}

/****************************************************************************
 * FUNCTION PURPOSE: Construct IPSEC ESP Tx Security Context
 ****************************************************************************
 * DESCRIPTION: Construct Security Context from the configuration parameters 
 *          for IPSEC ESP Tx operations
 *
 *  uint16_t salld_esp_set_tx_sc(
 *            salldIpsecInst_t*     inst)      -> Point to IPSEC channel instance
 *                       
 * Return values:  
 *                          
 *
 ***************************************************************************/
int16_t salld_esp_set_tx_sc(salldIpsecInst_t *inst) 
{
  salldInst_t *salldInst = &inst->salldInst;
  salldObj_t *sysInst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, salldInst->ownerInstOffset);
  salldIpsecTxInst_t *txInst = &inst->txInst;
  salldIpsecComInfo_t *pComInfo = &txInst->comInfo;
  Sa_IpsecConfigParams_t  *pConfig = &pComInfo->config;
  Sa_ScReqInfo_t* pScInfo = &txInst->scInfo;
  int16_t encCmdlSize, encScSize, phpScSize;
  int16_t authCmdlSize, authScSize;
  uint8_t encEngId, authEngId, firstEngId;
  saDMAReqInfo_t dmaReqInfo;
  uint16_t useEnc;
  uint16_t esnEn = ((pConfig->ctrlBitMap & sa_IPSEC_CONFIG_ESN) == sa_IPSEC_CONFIG_ESN);
  uint16_t randomIV;
  
  /*
   * Calculate the security Context Size and allocate security Context
   *
   */
  salld_sc_enc_get_info(txInst->cipherMode, pConfig->ivSize, &encCmdlSize, &encScSize, &encEngId, &randomIV, SALLD_TEST_SASS_GEN2(sysInst)); 
  salld_sc_auth_get_info(txInst->authMode, &useEnc, &authCmdlSize, &authScSize, &authEngId);
                              
  firstEngId = encScSize?encEngId:authEngId;
  /*
   * Check to see if enc/auth engine pair is being used, if so, use the calculated engine pair numbers 
   */
  if (salldInst->engSelect == 1)
  { 
    if(firstEngId == SALLD_CMDL_ENGINE_ID_ES1)
    {
        firstEngId = SALLD_CMDL_ENGINE_ID_2ES1;
    }
    else if(firstEngId == SALLD_CMDL_ENGINE_ID_AS1)
    {
        firstEngId = SALLD_CMDL_ENGINE_ID_2AS1;
    }
  }
  
  phpScSize = ((txInst->authMode == sa_AuthMode_CMAC) || (txInst->authMode == sa_AuthMode_AES_XCBC))?
               SA_CTX_PHP_IPSEC_TX_TYPE2_SIZE:SA_CTX_PHP_IPSEC_TX_TYPE1_SIZE;  
  
  pScInfo->scSize = phpScSize + encScSize + authScSize;
  salldLObj.callOutFuncs.ScAlloc((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo);
  
  if(pScInfo->scBuf == (uintptr_t )NULL)
    return(sa_ERR_NO_CTX_BUF);
    
  pScInfo->scID |= ((pConfig->ctrlBitMap & sa_IPSEC_CONFIG_PERMANENT_SC)?0x8000:0);  
  
  Sa_osalBeginScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  
  memset((void *) pScInfo->scBuf, 0, SALLD_BYTE_TO_WORD(pScInfo->scSize));
  
  /* Prepare PHP Security Context */
  dmaReqInfo.phpFetchSize = SA_CTX_SIZE_TO_DMA_SIZE(phpScSize);
  if ((encScSize == 0) && useEnc)
  {
    /* Special case: Use Encryption Engine for Authentication and there is no encryption */
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_DMA_SIZE_0;
  }
  else
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(encScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
  }
  dmaReqInfo.phpEvictSize = SA_CTX_DMA_SIZE_64;

  salld_set_sc_phpCommom(&dmaReqInfo, &txInst->destInfo, SA_CTX_PKT_TYPE_IPSEC_ESP,
                         pScInfo->scID, (tword *) pScInfo->scBuf);    
    
  /* Construct the IPSEC ESP Tx specific Security Context */    
  {
    tword* ctxIn = (tword *) (pScInfo->scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_COMMON_SIZE));
    tword* pSalt = ctxIn + SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, salt1);
    tword* pAux1 = ctxIn + SALLD_BYTE_TO_WORD(SA_CTX_ESP_AUX_OFFSET_TX);
    
    uint16_t ctrlBitfield = (uint16_t)salld_esp_get_cmdl_opmode(txInst->cipherMode,
                                                            txInst->authMode);
    /* Construct the ctrlBitfield */                                 
    //SA_CTX_PROTO_IPSEC_ESP_TX_SET_TRANSPORT_TYPE(ctrlBitfield, pConfig->transportType);
    if(pConfig->ctrlBitMap & sa_IPSEC_CONFIG_ESN)
        SA_CTX_PROTO_IPSEC_ESP_TX_SET_ESN(ctrlBitfield, 1);
    if(useEnc)    
        SA_CTX_PROTO_IPSEC_ESP_TX_SET_USEENC(ctrlBitfield, 1); 
    if((txInst->authMode == sa_AuthMode_CMAC) || (txInst->authMode == sa_AuthMode_AES_XCBC))
        SA_CTX_PROTO_IPSEC_ESP_TX_SET_CMAC(ctrlBitfield, 1);
    if(txInst->cipherMode == sa_CipherMode_AES_CTR)
        SA_CTX_PROTO_IPSEC_ESP_TX_SET_AESCTR(ctrlBitfield, 1);
    if(!encScSize)
        SA_CTX_PROTO_IPSEC_ESP_TX_SET_NULLENC(ctrlBitfield, 1);
    if(randomIV)
        SA_CTX_PROTO_IPSEC_ESP_TX_SET_RANDOM_IV(ctrlBitfield, 1);   
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, ctrlBitfield), ctrlBitfield);
    
    /* Set ESN */
    pktWrite32bits_m(ctxIn, 
                     SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, esn) + SALLD_FIELDOFFSET(saIpsecEsn_t, lo), 
                     pConfig->esnLo);
    pktWrite32bits_m(ctxIn, 
                     SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, esn) + SALLD_FIELDOFFSET(saIpsecEsn_t, hi), 
                     pConfig->esnHi);
    /* Set SPI, icvSize, ivSize, commandl label length and the first engine ID */                 
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, spi), pConfig->spi);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, icvIvSize), 
                     SALLD_MK_UINT16(pConfig->macSize, pConfig->ivSize));
    pktWrite8bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, firstEngIdCmdlLen), 
                    firstEngId);
    pktWrite8bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspTx_t, firstEngIdCmdlLen) + 1, 
                    encCmdlSize + authCmdlSize);
                    
    /* Copy Master Salt if necessary */
    if (pConfig->sessionSaltSize)
    {
        misc_utlCopy(pComInfo->sessionSalt, (uint16_t *)pSalt,
                     SA_IPSEC_MAX_SALT_SIZE_IN_TUINT); 
    }    
    
    /* Derive and Contruct K1/K2 if CMAC mode */                               
    if (txInst->authMode == sa_AuthMode_CMAC)
    {
        salld_aes_cmac_get_keys((tword *)pComInfo->sessionMacKey, pConfig->sessionMacKeySize << 3,
                                (tword *)pAux1, 
                                (tword *)(pAux1 + SALLD_BYTE_TO_WORD(SALLD_AES_BLOCK_SIZE_IN_BYTE)));
    }
    else if (txInst->authMode == sa_AuthMode_AES_XCBC)
    {
        salld_aes_xcbc_get_subkey((tword *)pComInfo->sessionMacKey, pConfig->sessionMacKeySize << 3,
                                   0x2, 
                                  (tword *)pAux1);
                                  
        salld_aes_xcbc_get_subkey((tword *)pComInfo->sessionMacKey, pConfig->sessionMacKeySize << 3,
                                   0x3, 
                                  (tword *)(pAux1 + SALLD_BYTE_TO_WORD(SALLD_AES_BLOCK_SIZE_IN_BYTE)));
    
    }
    
    
  }
  
  /* Prepare Security Context for the encryption Engine */
  if (txInst->cipherMode != sa_CipherMode_NULL)
  {
    salld_set_sc_enc(sa_PT_IPSEC_ESP, txInst->cipherMode, pConfig->sessionEncKeySize, 
                     pComInfo->sessionEncKey,  
                     esnEn?SA_GCM_ESP_AAD_LEN2:SA_GCM_ESP_AAD_LEN1,
                     TRUE,
                     (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(phpScSize));
  }
  
  /* Prepare Security Context for the authentication Engine */
  if (txInst->authMode != sa_AuthMode_NULL)
  {
     if (useEnc)
     {
         salld_set_sc_enc(sa_PT_IPSEC_ESP, txInst->authMode, pConfig->sessionMacKeySize, 
                          pComInfo->sessionMacKey,  
                          esnEn?SA_GCM_ESP_AAD_LEN2:SA_GCM_ESP_AAD_LEN1,
                          FALSE,
                          (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(phpScSize + encScSize));
    
     }
     else
     {
         salld_set_sc_auth(txInst->authMode, pConfig->sessionMacKeySize, 
                           pComInfo->sessionMacKey, 
                           (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(phpScSize + encScSize));
     }               
  }
  
  /* Security Context swizzling */
  salld_swiz_128((uint8_t *) pScInfo->scBuf, (uint8_t *)pScInfo->scBuf, pScInfo->scSize);
  Sa_osalEndScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  
  /* Prepare the SW Info Words */
  salld_set_swInfo((salldInst->engSelect == 1)?SALLD_CMDL_ENGINE_IPSEC_2HPS1: SALLD_CMDL_ENGINE_IPSEC_HPS1, 
                   0, NULL, pScInfo, &txInst->swInfo, 0);

  /* Store the scBuf internally as offset to suppor multiprocess */
  pScInfo->scBuf = (uintptr_t) sa_CONV_ADDR_TO_OFFSET(salldLObj.scPoolBaseAddr, pScInfo->scBuf);  
  
  return (sa_ERR_OK);
}            

/****************************************************************************
 * FUNCTION PURPOSE: Construct IPSEC ESP Rx Security Context
 ****************************************************************************
 * DESCRIPTION: Construct Security Context from the configuration parameters 
 *          for Rx operations
 *
 *  uint16_t salld_esp_set_rx_sc(
 *            salldIpsecInst_t*      inst)       -> Point to SRTP channel instance
 *                       
 * Return values:  
 *                          
 *
 ***************************************************************************/
int16_t salld_esp_set_rx_sc(salldIpsecInst_t *inst) 
{
  salldInst_t *salldInst = &inst->salldInst;
  salldObj_t *sysInst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, salldInst->ownerInstOffset);
  salldIpsecRxInst_t *rxInst =  &inst->rxInst;
  salldIpsecComInfo_t* pComInfo = &rxInst->comInfo;
  Sa_IpsecConfigParams_t*  pConfig = &pComInfo->config;
  Sa_ScReqInfo_t* pScInfo = &rxInst->scInfo;
  int16_t encCmdlSize, encScSize, phpScSize;
  int16_t authCmdlSize, authScSize;
  uint8_t encEngId, authEngId, firstEngId;
  saDMAReqInfo_t dmaReqInfo;
  uint16_t useEnc;
  uint16_t esnEn = ((pConfig->ctrlBitMap & sa_IPSEC_CONFIG_ESN) == sa_IPSEC_CONFIG_ESN);
  uint16_t randomIV;
  
  /*
   * Calculate the security Context Size and allocate security Context
   *
   */
  salld_sc_enc_get_info(rxInst->cipherMode, pConfig->ivSize, &encCmdlSize, &encScSize, &encEngId, &randomIV, SALLD_TEST_SASS_GEN2(sysInst)); 
  salld_sc_auth_get_info(rxInst->authMode, &useEnc, &authCmdlSize, &authScSize, &authEngId);
  
  firstEngId = (rxInst->authMode == sa_AuthMode_NULL)?encEngId:authEngId;
  /*
   * Check to see if enc/auth engine pair is being used, if so, use the calculated engine pair numbers 
   */
  if (salldInst->engSelect == 1)
  { 
    if(firstEngId == SALLD_CMDL_ENGINE_ID_ES1)
    {
        firstEngId = SALLD_CMDL_ENGINE_ID_2ES1;
    }
    else if(firstEngId == SALLD_CMDL_ENGINE_ID_AS1)
    {
        firstEngId = SALLD_CMDL_ENGINE_ID_2AS1;
    }
  }
  
  phpScSize = ((rxInst->authMode == sa_AuthMode_CMAC) || (rxInst->authMode == sa_AuthMode_AES_XCBC))?
              SA_CTX_PHP_IPSEC_RX_TYPE2_SIZE:SA_CTX_PHP_IPSEC_RX_TYPE1_SIZE; 
              
  /*
   * Adjust phpScSize if large replay window is required
   */
  if (rxInst->windowCheck > SALLD_IPSEC_MAX_REPLAY_WINDOW_SIZE_SHORT_CTX)
  {
    phpScSize = 256;
  }              
  
  pScInfo->scSize = phpScSize + encScSize + authScSize;
  salldLObj.callOutFuncs.ScAlloc((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo);
  
  if(pScInfo->scBuf == (uintptr_t) NULL)
    return(sa_ERR_NO_CTX_BUF);
    
  pScInfo->scID |= ((pConfig->ctrlBitMap & sa_IPSEC_CONFIG_PERMANENT_SC)?0x8000:0);  
  
  Sa_osalBeginScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  
  memset((void *) pScInfo->scBuf, 0, SALLD_BYTE_TO_WORD(pScInfo->scSize));
  
  /* Prepare PHP Security Context */
  /* Note: phpFetchSize = 00b indicates the PHP context size = 256 */
  dmaReqInfo.phpFetchSize = (phpScSize == 256)?0:SA_CTX_SIZE_TO_DMA_SIZE(phpScSize);
  if (useEnc)
  {
    /* Special case: Use Encryption as authentication engine */
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(encScSize);
  }
  else
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(encScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
  }
  
  /* Note: phpEVictSize = 00b indicates the PHP context size = 256 */
  dmaReqInfo.phpEvictSize = (phpScSize == 256)?0:SA_CTX_DMA_SIZE_96;
  
  salld_set_sc_phpCommom(&dmaReqInfo, &rxInst->destInfo, SA_CTX_PKT_TYPE_IPSEC_ESP | SA_CTX_PKT_DIR_RX,
                         pScInfo->scID, (tword *) pScInfo->scBuf);    
   
  /* Construct the IPSEC ESP Rx specific Security Context */    
  {
    tword* ctxIn = (tword *) (pScInfo->scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_COMMON_SIZE));
    tword* pSalt = ctxIn + SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, salt1);
    tword* pAux1 = ctxIn + SALLD_BYTE_TO_WORD(SA_CTX_ESP_AUX_OFFSET_RX);
    uint16_t ctrlBitfield = (uint16_t)salld_esp_get_cmdl_opmode(rxInst->cipherMode,
                                                            rxInst->authMode);
                                                        
    /* Construct the ctrlBitfield */                                 
    //SA_CTX_PROTO_IPSEC_ESP_RX_SET_TRANSPORT_TYPE(ctrlBitfield, pConfig->transportType);
    if(pConfig->ctrlBitMap & sa_IPSEC_CONFIG_ESN)
        SA_CTX_PROTO_IPSEC_ESP_RX_SET_ESN(ctrlBitfield, 1); 
    if (rxInst->windowCheck) 
    {
        SA_CTX_PROTO_IPSEC_ESP_RX_SET_REPLAY(ctrlBitfield, 1); 
    } 
    
    /* Initialize the replay Control Block */
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winSize), rxInst->windowCheck);
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winBaseHi), pConfig->esnHi);
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winBase), pConfig->esnLo);
    
    if(useEnc)    
        SA_CTX_PROTO_IPSEC_ESP_RX_SET_USEENC(ctrlBitfield, 1); 
    if((rxInst->authMode == sa_AuthMode_CMAC) || (rxInst->authMode == sa_AuthMode_AES_XCBC))
    {
        SA_CTX_PROTO_IPSEC_ESP_RX_SET_CMAC(ctrlBitfield, 1);
    }
    if(rxInst->cipherMode == sa_CipherMode_AES_CTR)
        SA_CTX_PROTO_IPSEC_ESP_RX_SET_AESCTR(ctrlBitfield, 1);
    if(!encScSize)
        SA_CTX_PROTO_IPSEC_ESP_RX_SET_NULLENC(ctrlBitfield, 1);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, ctrlBitfield), ctrlBitfield);
    
    /* Set SPI, icvSize, ivSize, commandl label length and the first engine ID */                 
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, spi), pConfig->spi);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, icvIvSize), 
                     SALLD_MK_UINT16(pConfig->macSize, pConfig->ivSize));
    pktWrite8bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, firstEngIdCmdlLen), 
                    firstEngId);
    pktWrite8bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecEspRx_t, firstEngIdCmdlLen) + 1, 
                    encCmdlSize + authCmdlSize); 
    /* Copy Master Salt if necessary */
    if (pConfig->sessionSaltSize)
    {
        misc_utlCopy(pComInfo->sessionSalt, (uint16_t *)pSalt,
                     SA_IPSEC_MAX_SALT_SIZE_IN_TUINT); 
    }    
    
    /* Derive and Contruct K1/K2 if CMAC mode */                               
    if (rxInst->authMode == sa_AuthMode_CMAC)
    {
        salld_aes_cmac_get_keys((tword *)pComInfo->sessionMacKey, pConfig->sessionMacKeySize << 3,
                                (tword *)pAux1, 
                                (tword *)(pAux1 + SALLD_BYTE_TO_WORD(SALLD_AES_BLOCK_SIZE_IN_BYTE)));
    }
    else if (rxInst->authMode == sa_AuthMode_AES_XCBC)
    {
        salld_aes_xcbc_get_subkey((tword *)pComInfo->sessionMacKey, pConfig->sessionMacKeySize << 3,
                                   0x2, 
                                  (tword *)pAux1);
                                  
        salld_aes_xcbc_get_subkey((tword *)pComInfo->sessionMacKey, pConfig->sessionMacKeySize << 3,
                                   0x3, 
                                  (tword *)(pAux1 + SALLD_BYTE_TO_WORD(SALLD_AES_BLOCK_SIZE_IN_BYTE)));
    
    }
    
                     
  }
                      
  /* Prepare Security Context for the encryption Engine */
  if (rxInst->cipherMode != sa_CipherMode_NULL)
  {
    tword* ctxIn = (tword *) (pScInfo->scBuf + (useEnc?SALLD_BYTE_TO_WORD(phpScSize+authScSize):
                                     SALLD_BYTE_TO_WORD(phpScSize)));
    salld_set_sc_enc(sa_PT_IPSEC_ESP, rxInst->cipherMode, pConfig->sessionEncKeySize, 
                     pComInfo->sessionEncKey,  
                     esnEn?SA_GCM_ESP_AAD_LEN2:SA_GCM_ESP_AAD_LEN1,
                     FALSE,
                     ctxIn);
  }
  
  /* Prepare Security Context for the authentication Engine */
  if (rxInst->authMode != sa_AuthMode_NULL)
  {
     if (useEnc)
     {
         salld_set_sc_enc(sa_PT_IPSEC_ESP, rxInst->authMode, pConfig->sessionMacKeySize, 
                          pComInfo->sessionMacKey,  
                          esnEn?SA_GCM_ESP_AAD_LEN2:SA_GCM_ESP_AAD_LEN1,
                          FALSE,
                          (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(phpScSize));
    
     }
     else
     {

         salld_set_sc_auth(rxInst->authMode, pConfig->sessionMacKeySize, 
                           pComInfo->sessionMacKey, 
                           (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(phpScSize + encScSize));
     }             
  }
  
  /* Security Context swizzling */
  salld_swiz_128((uint8_t *) pScInfo->scBuf, (uint8_t *) pScInfo->scBuf, pScInfo->scSize);
  Sa_osalEndScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  
  /* Prepare the SW Info Words */
  salld_set_swInfo((salldInst->engSelect == 1)?SALLD_CMDL_ENGINE_IPSEC_2HPS1: SALLD_CMDL_ENGINE_IPSEC_HPS1, 
                   0, NULL, pScInfo, &rxInst->swInfo, 0);

  /* Store the scBuf internally as offset to suppor multiprocess */
  pScInfo->scBuf = (uintptr_t) sa_CONV_ADDR_TO_OFFSET(salldLObj.scPoolBaseAddr, pScInfo->scBuf);
  
  return (sa_ERR_OK);

}            

/* Nothing past this point */

