/******************************************************************************
 * FILE PURPOSE: IPSEC AH Main File
 ******************************************************************************
 * FILE NAME: salldah.c
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
/* Standard include files */
#include <string.h>

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include <ti/drv/sa/sa_osal.h>
#include "src/salldloc.h"
#include "src/salldport.h"
#include "src/salldproto.h"
#include "salldipsec.h"
#include "salldipsecloc.h"

#include "src/auth/salldcmac.h"
#include "src/auth/salldxcbc.h"

/******************************************************************************
 * FUNCTION PURPOSE: IPSEC AH Send Data
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_ah_send_data (
 *    void *salldInst,      - A pointer to SALLD IPSEC instance
 *    void *pktInfo,        - packet pointer
 *    uint16_t clear) 
 *
 * here's the format of AH
 *
 *              |<----------------------------- AUTHENTICATION HEADER --------------------------------->|
 *  *-----------*---------------------------------------------------------------------------------------*-------------*
 *  * IP HEADER * NEXT HEADER | PAYLOAD LENGTH | RESERVED | SPI | SEQUENCE NUMBER | AUTHENTICATION DATA * IP DATAGRAM *
 *  *-----------*---------------------------------------------------------------------------------------*-------------*
 *  |<----------------------------------------- AUTHENTICATED PAYLOAD ----------------------------------------------->|
 *
 *       
 *  Perform the following actions:
 *	    - Reserve room for the Authentication Header (the next header, AH length, SPI, 
 *        sequence number and the authentication data) following the external IP header 
 *        for Tunnel mode or the original IP header for transport mode.
 *      - Generate and insert IV in front of ICV if required by the authentication algorithm
 *      - Perform ICV padding for IPV6 if necessary since the AH header should be multiple of 8-bytes
 *      - Extract the next header (protocol) from the IP header, and 
 *        replace it with AH Transport (51).
 *      - Fill in the next header and the AH length.
 *      - Adjust the payload length of the external IP header.
 *      - Update the packet size and protocol (IP) payload size in the header parsing information 
 *        to reserve room for the ESN and payload padding if necessary
 *      - Update statistics
 *
 * Note: Assume that enough space is reserved in front of the segment bugffer
 *       to insert the AH header
 *****************************************************************************/
int16_t salld_ah_send_data (void *salldInst, void *pktInfo, uint16_t clear) 
{
  salldIpsecInst_t *inst   = (salldIpsecInst_t *)salldInst; 
  salldIpsecTxInst_t *txInst = (salldIpsecTxInst_t *) &inst->txInst;
  salldIpsecComInfo_t* pComInfo = &txInst->comInfo;
  Sa_IpsecConfigParams_t*  pConfig = &pComInfo->config;
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;
  Sa_PktDesc_t* pPktDesc = &pPktInfo->pktDesc;
  tword *pktIn, *pAH;
  tword *pIP, *pOrig;
  tword *pIPNew, *pNew;
  int16_t ipVerLen;
  int16_t ahHdrSize;  /* size of the AH header in bytes */
  int16_t ipLen, payloadLen;
  int16_t ahHdrLen;   /* Length field at the AH header */
  int16_t sI, ahSegmentOffset, payloadOffset, segOffsetTmp, segOffset;
  
  uint8_t nextHdr, nextHdrOffset;
  uint16_t  f_ipV6;
  uint16_t *segUsedSizes;
  int extHdrLen, maxIpLen;
  
  /* Encrypt and Authenticate Packet(s) */
  if(pPktDesc->nSegments < 1)
    return (sa_ERR_PARAMS);
  payloadOffset = pPktDesc->payloadOffset;
  sI = segOffsetTmp = segOffset = 0;
  segUsedSizes = pPktDesc->segUsedSizes;
  /* Calculate segment containing IP header as well as segment offset to IP Header*/
  segOffsetTmp = segUsedSizes[sI];
  while(segOffset + segOffsetTmp < payloadOffset)
  {
	segOffset += segOffsetTmp;
	segOffsetTmp = segUsedSizes[++sI];
  }
  ahSegmentOffset = payloadOffset - segOffset;
  
  pOrig = pIP = (tword *)pPktDesc->segments[sI];
  pIP += SALLD_BYTE_TO_WORD(ahSegmentOffset);
  pktIn = pIP;
  
  /*
   * calculate the size of authentictaion header and retrieve the pointer to the authentication header
   */
  ahHdrSize = IPSEC_AH_BASIC_SIZE_BYTES + pConfig->macSize + pConfig->ivSize;
  ipVerLen = pktRead8bits_m(pktIn, IPV4_BYTE_OFFSET_VER_HLEN);
  
  if ((f_ipV6 = ((ipVerLen & IPV4_VER_MASK) == IPV6_VER_VALUE)))
  {
    /* IPV6 requires that All header to be 8-byte aligned */
    ahHdrSize = SALLD_ROUND_UP(ahHdrSize, 8);
    payloadLen = pktRead16bits_m(pktIn, IPV6_BYTE_OFFSET_PLEN) + IPV6_HDR_SIZE_BYTES;
    ipLen = IPV6_HDR_SIZE_BYTES;
    nextHdr = pktRead8bits_m(pIP, IPV6_BYTE_OFFSET_PROTO);
    nextHdrOffset = IPV6_BYTE_OFFSET_PROTO;
    
    maxIpLen = pPktDesc->segUsedSizes[sI] - ahSegmentOffset;
    
    while ((nextHdr == IP_PROTO_IPV6_HOP_BY_HOP) ||
           (nextHdr == IP_PROTO_IPV6_ROUTE)      ||
           (nextHdr == IP_PROTO_IPV6_DEST_OPT))
    {
        nextHdrOffset = ipLen + IPV6_OPT_HEADER_OFFSET_PROTO;
        nextHdr = pktRead8bits_m(pIP, ipLen + IPV6_OPT_HEADER_OFFSET_PROTO);
        extHdrLen = pktRead8bits_m(pIP, ipLen + IPV6_OPT_HEADER_OFFSET_LEN);
        ipLen += (extHdrLen + 1) * IPV6_OPT_HEADER_LEN_UNIT_IN_BYTES;
        
        if(ipLen > maxIpLen)
            return (sa_ERR_PACKET);
    }         
    
  }
  else
  {
    ipLen = (ipVerLen & IPV4_HLEN_MASK) << 2;
    payloadLen = pktRead16bits_m(pIP, IPV4_BYTE_OFFSET_LEN);
    nextHdr = pktRead8bits_m(pIP, IPV4_BYTE_OFFSET_PROTO);
  }
  
  pIPNew = pIP - SALLD_BYTE_TO_WORD(ahHdrSize);
  pNew = pOrig - SALLD_BYTE_TO_WORD(ahHdrSize);
  
  misc_utlCopy((uint16_t *)pOrig, (uint16_t *)pNew,  SALLD_BYTE_TO_TUINT(ipLen + ahSegmentOffset));
  
  /* Extract the next header and update the IP header for AH */
  payloadLen += ahHdrSize;
  if (f_ipV6)
  {
    pktWrite8bits_m(pIPNew, nextHdrOffset, IP_PROTO_AUTH);
    pktWrite16bits_m(pIPNew, IPV6_BYTE_OFFSET_PLEN, payloadLen - IPV6_HDR_SIZE_BYTES);
  }
  else
  {
    pktWrite8bits_m(pIPNew, IPV4_BYTE_OFFSET_PROTO, IP_PROTO_AUTH);
    pktWrite16bits_m(pIPNew, IPV4_BYTE_OFFSET_LEN, payloadLen);
    
    /* Reclaculate the IPV4 haeder checksum */
    salld_set_ipv4_chksum(pIPNew);
  }
    
  /* Populate the AH header */
  pAH = pktIn = pIPNew + SALLD_BYTE_TO_WORD(ipLen);
  pktWrite8bits_m(pktIn, IPSEC_AH_OFFSET_NEXT_HEADER, nextHdr);
  ahHdrLen = ahHdrSize/4 - 2;
  pktWrite8bits_m(pktIn, IPSEC_AH_OFFSET_PAYLOAD_LEN, ahHdrLen);
  pktWrite16bits_m(pktIn, IPSEC_AH_OFFSET_RESERVED1, 0);
  pktWrite32bits_m(pktIn, IPSEC_AH_OFFSET_SPI, pConfig->spi);  
  
  pktIn += SALLD_BYTE_TO_WORD(IPSEC_AH_BASIC_SIZE_BYTES);
  
  /* Reserve room for IV */
  if (pConfig->ivSize)
  {
    pktIn += SALLD_BYTE_TO_WORD(pConfig->ivSize); 
  }
  
  /* zero out the hash and padding area */
  memset(pktIn, 0, SALLD_BYTE_TO_WORD(ahHdrSize) - (pktIn - pAH));
  
  /* Adjust the packet length */
  pPktDesc->payloadLen = payloadLen;
  pPktDesc->segUsedSizes[sI] += ahHdrSize;
  pPktDesc->size += ahHdrSize;
  pPktDesc->segments[sI] = pNew;
  
  /* Pass the software Info in the packet */
  pPktInfo->validBitMap |= sa_PKT_INFO_VALID_SW_INFO;
  pPktInfo->swInfo = txInst->swInfo;
  
  return(sa_ERR_OK);
      
} /* salld_ah_send_data */

/******************************************************************************
 * FUNCTION PURPOSE: IPSEC AH Receive Data Post Processing
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_ah_receive_data_update (
 *    salldIpsecInst_t *inst,       - A pointer to IPSEC instance
 *    Sa_PktInfo_t   *pktInfo)      - packet pointer
 *
 *****************************************************************************/
static int16_t salld_ah_receive_data_update (salldIpsecInst_t *inst, Sa_PktInfo_t* pktInfo) 
{

  Sa_PktDesc_t* pPktDesc = &pktInfo->pktDesc;
  tword *pktIn;
  tword *pHdr, *pHdrNew;
  tword *pIP;
  tword *pIPNew;
  int16_t ahHdrSize, ipVerLen;
  int16_t ipLen, payloadLen;
  uint8_t nextHdr, nextHdrOffset;
  uint16_t  f_ipV6;
  uint16_t tempBuf[128];
  int16_t sI, ahSegmentOffset, payloadOffset, segOffsetTmp, segOffset;
  uint16_t *segUsedSizes; 
  int extHdrLen, maxIpLen;
  
  /* Encrypt and Authenticate Packet(s) */
  /* assume single segemnt at this moment */
  if(pPktDesc->nSegments < 1)
    return (sa_ERR_PARAMS);
  payloadOffset = pPktDesc->payloadOffset;
  sI = segOffsetTmp = segOffset = 0;
  segUsedSizes = pPktDesc->segUsedSizes;
  /* Calculate segment containing IP header as well as segment offset to IP Header */
  segOffsetTmp = segUsedSizes[sI];
  while(segOffset + segOffsetTmp < payloadOffset)
  {
	segOffset += segOffsetTmp;
	segOffsetTmp = segUsedSizes[++sI];
  }
  ahSegmentOffset = payloadOffset - segOffset;
  
  pHdr = (tword *)pPktDesc->segments[sI];
  pIP = pHdr + SALLD_BYTE_TO_WORD(ahSegmentOffset);
  pktIn = pIP;

  ipVerLen = pktRead8bits_m(pIP, IPV4_BYTE_OFFSET_VER_HLEN);
  
  if ((f_ipV6 = ((ipVerLen & IPV4_VER_MASK) == IPV6_VER_VALUE)))
  {
    ipLen = IPV6_HDR_SIZE_BYTES;
    payloadLen = pktRead16bits_m(pIP, IPV6_BYTE_OFFSET_PLEN) + IPV6_HDR_SIZE_BYTES;
    nextHdr = pktRead8bits_m(pIP, IPV6_BYTE_OFFSET_PROTO);
    nextHdrOffset = IPV6_BYTE_OFFSET_PROTO;
    
    maxIpLen = pPktDesc->segUsedSizes[sI] - ahSegmentOffset;
    
    while ((nextHdr == IP_PROTO_IPV6_HOP_BY_HOP) ||
           (nextHdr == IP_PROTO_IPV6_ROUTE)      ||
           (nextHdr == IP_PROTO_IPV6_DEST_OPT))
    {
        nextHdrOffset = ipLen + IPV6_OPT_HEADER_OFFSET_PROTO;
        nextHdr = pktRead8bits_m(pIP, ipLen + IPV6_OPT_HEADER_OFFSET_PROTO);
        extHdrLen = pktRead8bits_m(pIP, ipLen + IPV6_OPT_HEADER_OFFSET_LEN);
        ipLen += (extHdrLen + 1) * IPV6_OPT_HEADER_LEN_UNIT_IN_BYTES;
        
        if(ipLen > maxIpLen)
            return (sa_ERR_PACKET);
    }         
  }
  else
  {
    ipLen = (ipVerLen & IPV4_HLEN_MASK) << 2;
    payloadLen = pktRead16bits_m(pIP, IPV4_BYTE_OFFSET_LEN);
    nextHdr = pktRead8bits_m(pIP, IPV4_BYTE_OFFSET_PROTO);
  }
  
  if(nextHdr != IP_PROTO_AUTH)
    return (sa_ERR_PACKET);
  
  /*
   * May need to adjust the payload length of outer IP if (hardware) RA-based inner IP reassembly occurs.
   * The inner IP reassembly will remove the extra outer IP paylaod such as ESP padding, trailer and 
   * authentication tag and L2 CRC. Therefore, the total packet size should match the IP length within 
   * inner IP header. 
   *
   * Assumption: All the headers except L2 header reside at the same data segment.
   */  
  if (pktInfo->validBitMap & sa_PKT_INFO_VALID_RX_PAYLOAD_INFO)
  {
    Sa_RxPayloadInfo_t* pRxPayloadInfo = &pktInfo->rxPayloadInfo;
    int16_t ipVerLen2, payloadLen2, ahSegmentOffset2;
    tword *pIPin;
    
    if (pRxPayloadInfo->ipOffset2)
    {
        ahSegmentOffset2 = pRxPayloadInfo->ipOffset2 - segOffset;
        pIPin = pHdr + SALLD_BYTE_TO_WORD(ahSegmentOffset2);
        
        ipVerLen2 = pktRead8bits_m(pIPin, IPV4_BYTE_OFFSET_VER_HLEN);
        
        if ((ipVerLen2 & IPV4_VER_MASK) == IPV6_VER_VALUE )
        {
            payloadLen2 = pktRead16bits_m(pIPin, IPV6_BYTE_OFFSET_PLEN) + IPV6_HDR_SIZE_BYTES;
        }
        else if ((ipVerLen2 & IPV4_VER_MASK) == IPV4_VER_VALUE )
        {
            payloadLen2 = pktRead16bits_m(pIPin, IPV4_BYTE_OFFSET_LEN);
        }     
        else
        {
            return (sa_ERR_PACKET);
        }
        
        if ((pRxPayloadInfo->ipOffset2 + payloadLen2) == (pPktDesc->payloadLen + payloadOffset))
        {
            /* Adjust the payload length of the outer IP */
            payloadLen = pPktDesc->payloadLen;
        }
    }   
  } 
  
  pktIn += SALLD_BYTE_TO_WORD(ipLen);
  
  nextHdr = pktRead8bits_m(pktIn, IPSEC_AH_OFFSET_NEXT_HEADER);
  ahHdrSize = pktRead8bits_m(pktIn, IPSEC_AH_OFFSET_PAYLOAD_LEN);
  ahHdrSize = (ahHdrSize + 2) * 4;
  
  pHdrNew = pHdr + SALLD_BYTE_TO_WORD(ahHdrSize);
  pIPNew = pIP + SALLD_BYTE_TO_WORD(ahHdrSize);
  
  /* Overwrite to AH header */
  misc_utlCopy((uint16_t *)pHdr, tempBuf, 
                SALLD_BYTE_TO_TUINT(ahSegmentOffset + ipLen));
  misc_utlCopy(tempBuf, (uint16_t *)pHdrNew,
               SALLD_BYTE_TO_TUINT(ahSegmentOffset + ipLen));
  
  /* Replace the next header and update the IP header for AH */
  payloadLen -= ahHdrSize;
  if (f_ipV6)
  {
    pktWrite8bits_m(pIPNew, nextHdrOffset, nextHdr);
    pktWrite16bits_m(pIPNew, IPV6_BYTE_OFFSET_PLEN, payloadLen - ipLen);

  }
  else
  {
    pktWrite8bits_m(pIPNew, IPV4_BYTE_OFFSET_PROTO, nextHdr);
    pktWrite16bits_m(pIPNew, IPV4_BYTE_OFFSET_LEN, payloadLen);
    
    /* Reclaculate the IPV4 haeder checksum */
    salld_set_ipv4_chksum(pIPNew);
  }
  
  /* Adjust the packet length */
  pPktDesc->payloadLen = payloadLen;
  pPktDesc->segUsedSizes[sI] -= ahHdrSize;
  pPktDesc->size = payloadLen + payloadOffset;
  pPktDesc->segments[sI] = pHdrNew;
  
  return sa_ERR_OK; 
  
} /* salld_ah_receive_data_update */

/******************************************************************************
 * FUNCTION PURPOSE: IPSEC AH Receive Data
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_ah_receive_data (
 *    void *salldInst,    - A pointer to SALLD instance
 *    void *pktInfo)      - packet pointer
 * 
 * Perform the following actions:
 *  - Update the corresponding statistics and return Error if the SASS packet 
 *    error occurs.    '
 *  - Extract the next header from the AH header and replace the one in 
 *    the IP header with it.
 *  - Update the packet size and protocol (IP) payload size in the header parsing 
 *    information by removing the size of the AH header
 *  - Remove the AH Header
 *  - Update statistics
 *
 *****************************************************************************/
int16_t salld_ah_receive_data (void *salldInst, void *pktInfo) 
{
    
  salldIpsecInst_t* inst   = (salldIpsecInst_t *)salldInst; 
  Sa_PktInfo_t* pPktInfo = (Sa_PktInfo_t*)pktInfo;
  int16_t ret;
  
  switch (pPktInfo->pktErrCode)
  {
    case sa_PKT_ERR_OK:
      ret = salld_ah_receive_data_update(inst, pPktInfo);
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
 * FUNCTION PURPOSE: Derive the AH Command Label Operation Mode
 ****************************************************************************
 * DESCRIPTION: Derive the AH Command Label Operation Mode from 
 *              the authentication mode
 *
 *  uint8_t salld_ah_get_cmdl_opmode(
 *             int16_t authMode)        -> Authentication Mode
 *   
 * Return values:  
 *        AH Command Label Operation Mode                  
 *
 ***************************************************************************/
static uint8_t salld_ah_get_cmdl_opmode(int16_t authMode) 
{
    if ((authMode == sa_AuthMode_GMAC) || (authMode == sa_AuthMode_GMAC_AH))
    {
        return (SA_IPSEC_AH_CMDL_MODE_GMAC*2);
    }
    else
    {
        return (SA_IPSEC_AH_CMDL_MODE_GEN*2);
    }
}

/****************************************************************************
 * FUNCTION PURPOSE: Construct IPSEC AH Tx Security Context
 ****************************************************************************
 * DESCRIPTION: Construct Security Context from the configuration parameters 
 *          for IPSEC AH Tx operations
 *
 *  uint16_t salld_ah_set_tx_sc(
 *            salldIpsecInst_t*     inst)      -> Point to IPSEC channel instance
 *                       
 * Return values:  
 *                          
 *
 ***************************************************************************/
int16_t salld_ah_set_tx_sc(salldIpsecInst_t *inst) 
{
  salldInst_t *salldInst = &inst->salldInst;
  salldIpsecTxInst_t *txInst = &inst->txInst;
  salldIpsecComInfo_t *pComInfo = &txInst->comInfo;
  Sa_IpsecConfigParams_t  *pConfig = &pComInfo->config;
  Sa_ScReqInfo_t* pScInfo = &txInst->scInfo;
  int16_t phpScSize;
  int16_t authCmdlSize, authScSize;
  uint8_t authEngId;
  saDMAReqInfo_t dmaReqInfo;
  uint16_t useEnc;
  
  /*
   * Calculate the security Context Size and allocate security Context
   *
   */
  salld_sc_auth_get_info(txInst->authMode, &useEnc, &authCmdlSize, &authScSize, &authEngId);
  if (txInst->authMode == sa_AuthMode_GMAC) 
  {
    authCmdlSize = SA_GMAC_CMDL_SIZE_NOAAD; /* IPSEC AH GMAC patch */
  }  
  
  phpScSize = ((txInst->authMode == sa_AuthMode_CMAC) || (txInst->authMode == sa_AuthMode_AES_XCBC))?
              SA_CTX_PHP_IPSEC_TX_TYPE2_SIZE:SA_CTX_PHP_IPSEC_TX_TYPE1_SIZE;  
  
  pScInfo->scSize = phpScSize + authScSize;
  
  /*
   * Check to see if enc/auth engine pair is being used, if so, use the calculated engine pair numbers 
   */
  if (salldInst->engSelect == 1)
  { 
    if(authEngId == SALLD_CMDL_ENGINE_ID_ES1)
    {
        authEngId = SALLD_CMDL_ENGINE_ID_2ES1;
    }
    else if(authEngId == SALLD_CMDL_ENGINE_ID_AS1)
    {
        authEngId = SALLD_CMDL_ENGINE_ID_2AS1;
    }
  }
  
  salldLObj.callOutFuncs.ScAlloc((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo);
  
  if(pScInfo->scBuf == (uintptr_t) NULL)
    return(sa_ERR_NO_CTX_BUF);
  
  Sa_osalBeginScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  memset((void *) pScInfo->scBuf, 0, SALLD_BYTE_TO_WORD(pScInfo->scSize));
  
  /* Prepare PHP Security Context */
  dmaReqInfo.phpFetchSize = SA_CTX_SIZE_TO_DMA_SIZE(phpScSize);
  if (useEnc)
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_DMA_SIZE_0;
  }
  else
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_DMA_SIZE_0;
    dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
  }
  dmaReqInfo.phpEvictSize = SA_CTX_DMA_SIZE_64;

  salld_set_sc_phpCommom(&dmaReqInfo, &txInst->destInfo, SA_CTX_PKT_TYPE_IPSEC_AH,
                      pScInfo->scID, (tword *) pScInfo->scBuf);    
                      
  /* Construct the IPSEC AH Tx specific Security Context */    
  {
    tword* ctxIn = (tword *) (pScInfo->scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_COMMON_SIZE));
    tword* pSalt = ctxIn + SALLD_FIELDOFFSET(saCtxProtoIpsecAhTx_t, salt1);
    tword* pAux1 = ctxIn + SALLD_BYTE_TO_WORD(SA_CTX_AH_AUX_OFFSET_TX);
    uint16_t ctrlBitfield = (uint16_t)salld_ah_get_cmdl_opmode(txInst->authMode);
  
    /* Construct the ctrlBitfield */                                 
    SA_CTX_PROTO_IPSEC_AH_TX_SET_TRANSPORT_TYPE(ctrlBitfield, pConfig->transportType);
    if(pConfig->ctrlBitMap & sa_IPSEC_CONFIG_ESN)
        SA_CTX_PROTO_IPSEC_AH_TX_SET_ESN(ctrlBitfield, 1); 
    if(useEnc)    
        SA_CTX_PROTO_IPSEC_AH_TX_SET_USEENC(ctrlBitfield, 1); 
    if((txInst->authMode == sa_AuthMode_CMAC) || (txInst->authMode == sa_AuthMode_AES_XCBC))
        SA_CTX_PROTO_IPSEC_AH_TX_SET_CMAC(ctrlBitfield, 1);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecAhTx_t, ctrlBitfield), ctrlBitfield);
    
    /* Set ESN */
    pktWrite32bits_m(ctxIn, 
                     SALLD_FIELDOFFSET(saCtxProtoIpsecAhTx_t, esn) + SALLD_FIELDOFFSET(saIpsecEsn_t, lo), 
                     pConfig->esnLo);
    pktWrite32bits_m(ctxIn, 
                     SALLD_FIELDOFFSET(saCtxProtoIpsecAhTx_t, esn) + SALLD_FIELDOFFSET(saIpsecEsn_t, hi), 
                     pConfig->esnHi);
                     
    /* Set SPI, icvSize, ivSize, commandl label length and the first engine ID */                 
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecAhTx_t, spi), pConfig->spi);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecAhTx_t, icvIvSize), 
                     SALLD_MK_UINT16(pConfig->macSize, pConfig->ivSize));
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecAhTx_t, firstEngIdCmdlLen), 
                    SALLD_MK_UINT16(authEngId, authCmdlSize));
                    
    /* Copy Master Salt if necessary */
    if (pConfig->sessionSaltSize)
    {
        misc_utlCopy(pComInfo->sessionSalt, (uint16_t *)pSalt,
                     SALLD_BYTE_TO_TUINT(pConfig->sessionSaltSize)); 
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
  
  /* Prepare Security Context for the encryption/authentication Engine */
  if (useEnc)
  {
      salld_set_sc_enc(sa_PT_IPSEC_AH, txInst->authMode, pConfig->sessionMacKeySize, 
                    pComInfo->sessionMacKey, 0, FALSE, 
                    (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(phpScSize));
  
  }
  else
  {
      salld_set_sc_auth(txInst->authMode, pConfig->sessionMacKeySize, 
                     pComInfo->sessionMacKey, 
                     (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(phpScSize));
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
 * FUNCTION PURPOSE: Construct IPSEC AH Rx Security Context
 ****************************************************************************
 * DESCRIPTION: Construct Security Context from the configuration parameters 
 *          for Rx operations
 *
 *  uint16_t salld_ah_set_rx_sc(
 *            salldIpsecInst_t*      inst)       -> Point to SRTP channel instance
 *                       
 * Return values:  
 *
 ***************************************************************************/
int16_t salld_ah_set_rx_sc(salldIpsecInst_t *inst) 
{
  salldInst_t *salldInst = &inst->salldInst;
  salldIpsecRxInst_t *rxInst =  &inst->rxInst;
  salldIpsecComInfo_t* pComInfo = &rxInst->comInfo;
  Sa_IpsecConfigParams_t*  pConfig = &pComInfo->config;
  Sa_ScReqInfo_t* pScInfo = &rxInst->scInfo;
  int16_t phpScSize;                                               
  int16_t authCmdlSize, authScSize;
  uint8_t authEngId;
  saDMAReqInfo_t dmaReqInfo;
  uint16_t useEnc;
  
  /*
   * Calculate the security Context Size and allocate security Context
   *
   */
  salld_sc_auth_get_info(rxInst->authMode, &useEnc, &authCmdlSize, &authScSize, &authEngId);
  if (rxInst->authMode == sa_AuthMode_GMAC)
  {
    authCmdlSize = SA_GMAC_CMDL_SIZE_NOAAD; /* IPSEC AH GMAC patch */
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
               
  pScInfo->scSize = phpScSize + authScSize;
  
  /*
   * Check to see if enc/auth engine pair is being used, if so, use the calculated engine pair numbers 
   */
  if (salldInst->engSelect == 1)
  { 
    if(authEngId == SALLD_CMDL_ENGINE_ID_ES1)
    {
        authEngId = SALLD_CMDL_ENGINE_ID_2ES1;
    }
    else if(authEngId == SALLD_CMDL_ENGINE_ID_AS1)
    {
        authEngId = SALLD_CMDL_ENGINE_ID_2AS1;
    }
  }
  
  salldLObj.callOutFuncs.ScAlloc((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo);
  
  if(pScInfo->scBuf == (uintptr_t) NULL)
    return(sa_ERR_NO_CTX_BUF);
    
  Sa_osalBeginScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  memset((void *) pScInfo->scBuf, 0, SALLD_BYTE_TO_WORD(pScInfo->scSize));
  
  /* Prepare PHP Security Context */
  /* Note: phpFetchSize = 00b indicates the PHP context size = 256 */
  dmaReqInfo.phpFetchSize = (phpScSize == 256)?0:SA_CTX_SIZE_TO_DMA_SIZE(phpScSize);
  if (useEnc)
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
    dmaReqInfo.engFetchSize[1] = SA_CTX_DMA_SIZE_0;
  }
  else
  {
    dmaReqInfo.engFetchSize[0] = SA_CTX_DMA_SIZE_0;
    dmaReqInfo.engFetchSize[1] = SA_CTX_SIZE_TO_DMA_SIZE(authScSize);
  }
  /* Note: phpEvictSize = 00b indicates the PHP context size = 256 */
  dmaReqInfo.phpEvictSize = (phpScSize == 256)?0:SA_CTX_DMA_SIZE_96;
  salld_set_sc_phpCommom(&dmaReqInfo, &rxInst->destInfo, SA_CTX_PKT_TYPE_IPSEC_AH | SA_CTX_PKT_DIR_RX,
                         pScInfo->scID, (tword *)pScInfo->scBuf);    
                      
               
  /* Construct the IPSEC AH Rx specific Security Context */    
  {
    tword* ctxIn = (tword *) (pScInfo->scBuf + SALLD_BYTE_TO_WORD(SA_CTX_PHP_COMMON_SIZE));
    tword* pSalt = ctxIn + SALLD_FIELDOFFSET(saCtxProtoIpsecAhRx_t, salt1);
    tword* pAux1 = ctxIn + SALLD_BYTE_TO_WORD(SA_CTX_AH_AUX_OFFSET_RX);
    uint16_t ctrlBitfield = (uint16_t)salld_ah_get_cmdl_opmode(rxInst->authMode);
    
    SA_CTX_PROTO_IPSEC_AH_RX_SET_TRANSPORT_TYPE(ctrlBitfield, pConfig->transportType);
    if(pConfig->ctrlBitMap & sa_IPSEC_CONFIG_ESN)
        SA_CTX_PROTO_IPSEC_AH_RX_SET_ESN(ctrlBitfield, 1); 
        
    if (rxInst->windowCheck) 
    {
        SA_CTX_PROTO_IPSEC_AH_RX_SET_REPLAY(ctrlBitfield, 1); 
    }    
    
    /* Initialize the replay Control Block */
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winSize), rxInst->windowCheck);
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winBaseHi), pConfig->esnHi);
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saReplayCtl_t, winBase), pConfig->esnLo);
    
    if(useEnc)    
        SA_CTX_PROTO_IPSEC_AH_RX_SET_USEENC(ctrlBitfield, 1); 
    if((rxInst->authMode == sa_AuthMode_CMAC) || (rxInst->authMode == sa_AuthMode_AES_XCBC))
        SA_CTX_PROTO_IPSEC_AH_RX_SET_CMAC(ctrlBitfield, 1);
            
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecAhRx_t, ctrlBitfield), ctrlBitfield);
    
    /* Set SPI, icvSize, ivSize, commandl label length and the first engine ID */                 
    pktWrite32bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecAhRx_t, spi), pConfig->spi);
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecAhRx_t, icvIvSize), 
                     SALLD_MK_UINT16(pConfig->macSize, pConfig->ivSize));
    pktWrite16bits_m(ctxIn, SALLD_FIELDOFFSET(saCtxProtoIpsecAhRx_t, firstEngIdCmdlLen), 
                    SALLD_MK_UINT16(authEngId, authCmdlSize));
                    
    /* Copy Master Salt if necessary */
    if (pConfig->sessionSaltSize)
    {
        misc_utlCopy(pComInfo->sessionSalt, (uint16_t *)pSalt,
                     SALLD_BYTE_TO_TUINT(pConfig->sessionSaltSize)); 
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
  
  if (useEnc)
  {
      salld_set_sc_enc(sa_PT_IPSEC_AH, rxInst->authMode, pConfig->sessionMacKeySize, 
                    pComInfo->sessionMacKey, 0, FALSE,
                    (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(phpScSize));
  
  }
  else
  {

      salld_set_sc_auth(rxInst->authMode, pConfig->sessionMacKeySize, 
                     pComInfo->sessionMacKey, 
                     (tword *) pScInfo->scBuf + SALLD_BYTE_TO_WORD(phpScSize));
  }          
  
  /* Security Context swizzling */
  salld_swiz_128((uint8_t *) pScInfo->scBuf, (uint8_t* )pScInfo->scBuf, pScInfo->scSize);
  Sa_osalEndScAccess((void *)pScInfo->scBuf, pScInfo->scSize);
  
  /* Prepare the SW Info Words */
  salld_set_swInfo((salldInst->engSelect == 1)?SALLD_CMDL_ENGINE_IPSEC_2HPS1: SALLD_CMDL_ENGINE_IPSEC_HPS1,
                   0, NULL, pScInfo, &rxInst->swInfo, 0);

  /* Store the scBuf internally as offset to suppor multiprocess */
  pScInfo->scBuf = (uintptr_t) sa_CONV_ADDR_TO_OFFSET(salldLObj.scPoolBaseAddr, pScInfo->scBuf);
  
  return (sa_ERR_OK);

}            

/* Nothing past this point */

