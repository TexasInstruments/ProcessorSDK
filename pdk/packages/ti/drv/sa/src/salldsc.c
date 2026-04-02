/******************************************************************************
 * FILE PURPOSE: Security Context Utility Functions
 ******************************************************************************
 * FILE NAME: salldsc.c
 *
 * DESCRIPTION: The main module for Security Context related Code
 *
 * (C) Copyright 2009-2012, Texas Instruments Inc.
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
#include "src/salldport.h"
#include "src/salldctx.h"
#include "src/salldloc.h"
#include "src/auth/salldhmac_md5.h"
#include "src/auth/salldhmac_sha1.h"
#include "src/auth/salldhmac_sha2.h"
#include "src/auth/salldhmac_sha512.h"
#include "src/cipher/salldgcm.h"
#include "src/cipher/salldaes.h"
#include "src/auth/salldxcbc.h"

/******************************************************************************
 * FUNCTION PURPOSE:  Perform 16-byte swizzling
 ******************************************************************************
 * DESCRIPTION: The SA sub-system requires all multi-byte fields in the security
 *              contexts in big endian format. Therfore, it is necessary to 
 *              perform 16-byte swizzling while CGEM is operating in Little Endian 
 *              mode. 
 *
 * void  salld_swiz_128 (
 *               uint8_t* in,     -  Pointer to the input data buffer 
 *               uint8_t* out,    -  Poniter to the output data buffer
 *               uint16_t size)   -  data size in bytes 
 *
 * Note: The output buffer can be the same as the input buffer
 *       But the input buffer will not be preserved in this case
 *       The size should be multiple of 16
 *       It will be necessary to add input check if this function may be used 
 *       externally.
 *
 *****************************************************************************/
void  salld_swiz_128 (uint8_t* in, uint8_t* out, uint16_t size)
{
    
    if (Sa_osalGetSysEndianMode() == sa_SYS_ENDIAN_MODE_BIG)
    {
        if (in != out) 
        {
            memcpy(out, in, size);
        }
    }    
    else
    {
        uint8_t data[16];
        int i, j;
    
        for (i = 0; i < size; i+=16)
        {
            memcpy(data, &in[i], 16);
            for (j = 0; j < 16; j++)
            {
                out[i + j] = data[15 - j];
            }    
        }
    }    
}


/******************************************************************************
 * FUNCTION PURPOSE:  Verify the Sc Buffer state
 ******************************************************************************
 * DESCRIPTION: This function verifies whether the security context buffer is 
 *              freed by SA
 *
 * uint16_t  Sa_isScBufFree (
 *              uint8_t* scBuf)   -  Poniter to the Security Context buffer
 *
 * Return:  TRUE    if buffer is free
 *          FALSE   otherwise
 *
 *****************************************************************************/
uint16_t  Sa_isScBufFree (uint8_t* scBuf)
{
    uint16_t flag;
    uint8_t data[16];
    
    Sa_osalBeginScAccess((void *)scBuf, 16);
    
    salld_swiz_128(scBuf, data, 16);
    
    Sa_osalEndScAccess((void *)scBuf, 0);
  
    flag = pktRead16bits_m((tword*)data, 0);
    
    if (!(flag & SA_CTX_SCCTL_FLAG_OWNER))
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/******************************************************************************
 * FUNCTION PURPOSE:  Verify the Sc Buffer Update state
 ******************************************************************************
 * DESCRIPTION: This function verifies whether the security context buffer has 
 *              been evicted with the latest statistic by SA
 *
 * uint16_t  salld_is_sc_updated (
 *              uint8_t* scBuf)   -  Poniter to the Security Context buffer
 *
 * Return:  TRUE    if buffer is updated
 *          FALSE   otherwise
 *
 * Note: This function should be invoked with multi-core critical section
 *       protection
 *       The cache protection is doine at the caller
 *
 *****************************************************************************/
uint16_t  salld_is_sc_updated (uint8_t* scBuf)
{
    uint16_t flag;
    uint8_t data[16];
    
    salld_swiz_128(scBuf, data, 16);
    
    flag = pktRead16bits_m((tword*)data, 0);
    
    if (!(flag & SA_CTX_SCCTL_FLAG_WAIT_UPDATE))
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/******************************************************************************
 * FUNCTION PURPOSE:  Set WAIT_UPDATE flag in the SC Buffer
 ******************************************************************************
 * DESCRIPTION: This function sets the WAIT_UPDATE flag to indicate that 
 *              the SC Buffer is waiting for an update from SA
 *
 * void  salld_sc_set_wait_update (
 *              uint8_t* scBuf)   -  Poniter to the Security Context buffer
 *
 * Note: This function should be invoked with multi-core critical section
 *       protection
 *       The cache protection is doine at the caller
 *
 *****************************************************************************/
void  salld_sc_set_wait_update (uint8_t* scBuf)
{
    uint16_t flag;
    uint8_t data[16];
    
    salld_swiz_128(scBuf, data, 16);
  
    flag = pktRead16bits_m((tword*)data, 0);
    
    flag |= SA_CTX_SCCTL_FLAG_WAIT_UPDATE;
    
    pktWrite16bits_m((tword*)data, 0, flag);
    
    salld_swiz_128(data, scBuf, 16);
}

/****************************************************************************
 * FUNCTION PURPOSE: Construct Software Information Words
 ****************************************************************************
 * DESCRIPTION: Construct Software Information Words
 *
 *  void salld_set_swInfo(
 *       uint16_t          engID            -> SA processing Engine ID
 *       uint16_t          ctrlFlags        -> Control Flags
 *       Sa_DestInfo_t*  pDestInfo          -> Pointer to the Destination Info
 *       Sa_ScReqInfo_t* pScInfo            -> Pointer to the SC Info
 *       Sa_SWInfo_t*    pSwInfo            -> Pointer to Sw Info
 *       uint8_t         statusWordSize)    -> Specify the output psInfo size 
 *                                             for authentication tag, but it should be 
 *                                             8-byte aligned
 *                       
 ***************************************************************************/
void salld_set_swInfo(uint16_t engID, uint16_t ctrlFlags, Sa_DestInfo_t* pDestInfo,
                      Sa_ScReqInfo_t* pScInfo, Sa_SWInfo_t*  pSwInfo, uint8_t statusWordSize)
{
    uintptr_t  scBuf;
    pSwInfo->size = 2;
    scBuf = (uintptr_t) Sa_osalGetSCPhyAddr((void *)pScInfo->scBuf);
    pSwInfo->swInfo[1] = (uint32_t)scBuf;
    pSwInfo->swInfo[0] = 0;
    SA_SW_SET_SC_ID(pSwInfo->swInfo[0],  pScInfo->scID);
    SA_SW_SET_ENG_ID(pSwInfo->swInfo[0], engID);
    SA_SW_INFO_SET_FLAGS(pSwInfo->swInfo[0], ctrlFlags);
    
    if (pDestInfo)
    {
        pSwInfo->size = 3;
        SA_SW_SET_DEST_INFO_PRESENT(pSwInfo->swInfo[0], 1);
        SA_SW_SET_CMDLB_PRESENT(pSwInfo->swInfo[0], 1);
        pSwInfo->swInfo[2] = SA_FORM_SW2(pDestInfo->queueID,
                                         pDestInfo->flowID,
                                         statusWordSize);
    }
}    

#if defined(NSS_LITE2)
/****************************************************************************
 * FUNCTION PURPOSE: Construct Software Information Words for SA2_UL
 ****************************************************************************
 * DESCRIPTION: Construct Software Information Words for SA2_UL
 *
 *  void salld_set_swInfo2(
 *       uint16_t          engID            -> SA processing Engine ID
 *       uint16_t          ctrlFlags        -> Control Flags
 *       Sa_DestInfo_t*  pDestInfo          -> Pointer to the Destination Info
 *       Sa_ScReqInfo_t* pScInfo            -> Pointer to the SC Info
 *       Sa_SWInfo_t*    pSwInfo            -> Pointer to Sw Info
 *       uint8_t         statusWordSize)    -> Specify the output psInfo size
 *                                             for authentication tag, but it should be
 *                                             8-byte aligned
 *
 ***************************************************************************/
void salld_set_swInfo2(uint16_t engID, uint16_t ctrlFlags, Sa_DestInfo_t* pDestInfo,
                      Sa_ScReqInfo_t* pScInfo, Sa_SWInfo_t*  pSwInfo, uint8_t statusWordSize)
{
    uint64_t scptr;
    uint32_t scptr_l;
    uint16_t scptr_h;

    pSwInfo->size = 3;
    scptr         = (uint64_t) Sa_osalGetSCPhyAddr((void *) pScInfo->scBuf);
    scptr_l       = (uint32_t) (((uint64_t)scptr >>  0) & 0xFFFFFFFFUL);
    scptr_h       = (uint16_t) (((uint64_t)scptr >> 32) & 0x0000FFFFUL);

    /*
     * Since software is always required to provide the full 48-bit SCPTR
     * in SW Word 1 and 2, this bit should be always set to ‘1’
     */
    pSwInfo->swInfo[0] = 0;
    SA_SW_SET_DEST_INFO_PRESENT(pSwInfo->swInfo[0], 1);
    SA_SW_SET_SC_ID(pSwInfo->swInfo[0],  pScInfo->scID);
    SA_SW_SET_ENG_ID(pSwInfo->swInfo[0], engID);
    SA_SW_INFO_SET_FLAGS(pSwInfo->swInfo[0], ctrlFlags);
    SA_SW_SET_CMDLB_PRESENT(pSwInfo->swInfo[0], 1);

    /* CPPI Software Data Word 1 (Extended Packet Info Word 2) */
    pSwInfo->swInfo[1] = scptr_l;

    /* CPPI Software Data Word 2 (Extended Packet Info Word 3) */
    pSwInfo->swInfo[2] = SA2_UL_FORM_SW2(scptr_h, statusWordSize);
}
#endif

/****************************************************************************
 * FUNCTION PURPOSE: Construct Security Context of the PHP Common
 ****************************************************************************
 * DESCRIPTION: Construct Security Context of the PHP Engine Common Portion
 *
 *  void salld_set_sc_phpCommom(
 *       saDMAReqInfo_t*   pDmaReqInfo      -> Pointer to the DMA Req Info
 *       Sa_DestInfo_t*  pDestInfo          -> Pointer to the Destination Info
 *       uint16_t   pktInfoByte               -> Packet Info
 *       uint16_t   ctxId                     -> Context ID
 *       tword*   ctxBuf)                   -> Pointer to the context Buffer               
 *                       
 ***************************************************************************/
void salld_set_sc_phpCommom(saDMAReqInfo_t*   pDmaReqInfo,  Sa_DestInfo_t*  pDestInfo,  
                            uint16_t pktInfoByte, uint16_t ctxId,  tword*  ctxBuf)
{
    uint16_t ctrlDmaInfo;
    uint8_t  flowId = (uint8_t)(pDestInfo->flowID & (uint8_t) 0xFFU);
    uintptr_t  scBufPtr = (uintptr_t)Sa_osalGetSCPhyAddr(ctxBuf);
    
    ctrlDmaInfo = SA_CTX_SCCTL_MK_DMA_INFO(pDmaReqInfo->phpFetchSize,
                                           pDmaReqInfo->engFetchSize[0],
                                           pDmaReqInfo->engFetchSize[1],
                                           pDmaReqInfo->phpEvictSize);
    if(pDestInfo->ctrlBitfield & sa_DEST_INFO_CTRL_USE_LOC_DMA)pktInfoByte |= SA_CTX_CTRL_USE_LOC_DMA;                                       
    pktWrite16bits_m(ctxBuf, 0, ctrlDmaInfo|SA_CTX_SCCTL_FLAG_OWNER);  
    pktWrite16bits_m(ctxBuf, 2, ctxId);  /* filled by hardware, reference only */
    pktWrite32bits_m(ctxBuf, 4, (uint32_t)scBufPtr); /* filled by hardware, reference only */
    
    pktWrite8bits_m(ctxBuf, 8, pktInfoByte);  
    pktWrite8bits_m(ctxBuf, 9, flowId);
    pktWrite16bits_m(ctxBuf, 10, pDestInfo->queueID);
    pktWrite32bits_m(ctxBuf, 12, pDestInfo->swInfo0);
    pktWrite32bits_m(ctxBuf, 16, pDestInfo->swInfo1);
}    

/****************************************************************************
 * FUNCTION PURPOSE: Construct SCCTL Security Context for SA2_UL
 ****************************************************************************
 * DESCRIPTION: Construct SCCTL of the Security Context Portion
 *
 *  void salld_set_sc_scctl(
 *       saDMAReqInfo_t*   pDmaReqInfo      -> Pointer to the DMA Req Info
 *       Sa_DestInfo_t*  pDestInfo          -> Pointer to the Destination Info
 *       uint16_t   pktInfoByte             -> Packet Info
 *       uint16_t   ctxId                   -> Context ID
 *       tword*   ctxBuf                    -> Pointer to the context Buffer
 *       uint8_t  priv                      -> priv to be kept for Security Context
 *       uint8_t  privId)                    > privId to be kept for Security Context
 *
 ***************************************************************************/
void salld_set_sc_scctl(saDMAReqInfo_t*   pDmaReqInfo,  Sa_DestInfo_t*  pDestInfo,
                            uint16_t pktInfoByte, uint16_t ctxId, tword*  ctxBuf, uint32_t ctrlBitMap, uint8_t priv, uint8_t privId)
{
    uintptr_t scptr;
    uint32_t scptr_l;
    uint16_t scptr_h;
    uint16_t ctrlDmaInfo;
    uint32_t ctrlSecWord =0;
    uint32_t byteOffset = 0;

    ctrlDmaInfo = SA_CTX_SCCTL_MK_DMA_INFO(pDmaReqInfo->phpFetchSize,
                                           pDmaReqInfo->engFetchSize[0],
                                           pDmaReqInfo->engFetchSize[1],
                                           pDmaReqInfo->phpEvictSize);

    scptr         = (uintptr_t) Sa_osalGetSCPhyAddr(ctxBuf);
    scptr_l       = (uint32_t) ((uint64_t)scptr >> 0   & 0xFFFFFFFFU);
    scptr_h       = (uint16_t) ((uint64_t)scptr >> 32  & 0x0000FFFFU);

    if(pDestInfo->ctrlBitfield & sa_DEST_INFO_CTRL_USE_LOC_DMA)pktInfoByte |= SA_CTX_CTRL_USE_LOC_DMA;
    pktWrite16bits_m(ctxBuf, byteOffset, ctrlDmaInfo|SA_CTX_SCCTL_FLAG_OWNER);
    byteOffset += 2;
    pktWrite16bits_m(ctxBuf, byteOffset, ctxId);   /* filled by hardware, reference only */
    byteOffset += 2;
    /* Derive the next word as below */
    /* Bits 31     OverWrite Flow Id, Host usually sets this bit in case of promotion or demotion
     * Bits 24:30: Reserved
     * Bits 16:23  PrivID
     * Bits 10:15  Reserved
     * Bits 8:9    Priv
     * Bits 7:     Allow Promote
     * Bits 6:     Allow Demote
     * Bits 5:     Allow NonSecure Data Pipe
     * Bits 1:4    Reserved
     * Bits 0:     Secure
     */
    ctrlSecWord = 0;
    if (ctrlBitMap == SA_CTX_SA2UL_SECURE) {
      ctrlSecWord |=  1;
    }

    if (ctrlBitMap == SA_CTX_SA2UL_ALLOW_NONSEC) {
      ctrlSecWord |=  (1 << 5);
    }

    if (ctrlBitMap == SA_CTX_SA2UL_ALLOW_DEMOTE) {
      ctrlSecWord |=  (1 << 6);
    }

    if (ctrlBitMap == SA_CTX_SA2UL_ALLOW_PROMOTE) {
      ctrlSecWord |=  (1 << 7);
    }

    if(ctrlBitMap == SA_CTX_SA2UL_SET_PRIV)
    {
        /* Set the Priv values */
        priv &= 3;
        ctrlSecWord |=  (priv << 8);
    }

     /* Set the privId for creating the context
      */
     ctrlSecWord |=  (privId << 16);

    pktWrite32bits_m(ctxBuf, byteOffset, ctrlSecWord);
    byteOffset += 4;
    pktWrite16bits_m(ctxBuf, byteOffset, pDestInfo->flowID);
    byteOffset += 2;
    pktWrite16bits_m(ctxBuf, byteOffset, scptr_h);/* filled by hardware, reference only */
    byteOffset += 2;
    pktWrite32bits_m(ctxBuf, byteOffset, scptr_l); /* filled by hardware, reference only */
    byteOffset += 4;

}

/****************************************************************************
 * FUNCTION PURPOSE: Find the Encryption related information
 ****************************************************************************
 * DESCRIPTION: Derive the parameters related to the encryption operation
 *              based on the encryption mode
 *
 *  void salld_sc_enc_get_info(
 *            int16_t    mode,      -> Encryption mode
 *            uint16_t   ivSize,    -> Size of the initialization vector
 *            int16_t*   pCmdlSize, -> Command Label size
 *            int16_t*   pScSize,   -> Security Context size  
 *            uint8_t    pEngId,    -> Processing Engine ID
 *            uint16_t*  pfRandonIV,-> flag indicates whether random IV is required 
 *            int        fSassGen2) -> Flag indicates whether this is a 2nd generation SASS
 *
 ***************************************************************************/
void salld_sc_enc_get_info(int16_t mode, uint16_t ivSize, int16_t*  pCmdlSize, 
                           int16_t* pScSize, uint8_t* pEngId, uint16_t* pfRandowIV, int fSassGen2) 
{
    uint8_t engId;
    uint16_t fRandowIV = FALSE;

    switch (mode)
    {
        case sa_CipherMode_NULL:
            *pScSize = 0;
            *pCmdlSize = 0;
            engId = SALLD_CMDL_ENGINE_NONE;
            break;
            
        case sa_CipherMode_AES_CTR:
            *pScSize = SA_AESCTR_ENC_SC_SIZE;
            *pCmdlSize = SA_AESCTR_CMDL_SIZE;
            engId = SA_AESCTR_ENG_ID;
            break;
    
        case sa_CipherMode_GCM:
            *pScSize = SA_GCM_ENC_SC_SIZE;
            *pCmdlSize = SA_GCM_CMDL_SIZE;
            engId = SA_GCM_ENG_ID;
            break;
            
        case sa_CipherMode_CCM:
            *pScSize = SA_CCM_ENC_SC_SIZE;
            *pCmdlSize = SA_CCM_CMDL_SIZE;
            engId = SA_CCM_ENG_ID;
            break;
            
        case sa_CipherMode_GSM_A53:
        case sa_CipherMode_ECSD_A53:
        case sa_CipherMode_GEA3:
        case sa_CipherMode_KASUMI_F8:
            *pScSize = SA_CTX_ENC_TYPE2_SIZE;
            *pCmdlSize = SALLD_CMDL_HEADER_SIZE_BYTES + ivSize;
            engId = SALLD_CMDL_ENGINE_ID_ACS1;
            break;  
            
        case sa_CipherMode_SNOW3G_F8:
            if (!fSassGen2)
            {
                /* SASS_GEN1 */
                *pScSize = SA_CTX_ENC_TYPE2_SIZE;
                *pCmdlSize = SALLD_CMDL_HEADER_SIZE_BYTES + ivSize;
                engId = SALLD_CMDL_ENGINE_ID_ACS1;
                break;
            } 
             
            /* SASS_GEN2: pass through */
        
        case sa_CipherMode_ZUC_F8:
            *pScSize = SA_CTX_ENC_TYPE1_SIZE;
            *pCmdlSize = SALLD_CMDL_HEADER_SIZE_BYTES + 32;
            engId = SALLD_CMDL_ENGINE_ID_ACS1;
            break;    
            
        case sa_CipherMode_SNOW3G_F8F9:
        case sa_CipherMode_ZUC_F8F9:
            *pScSize = SA_CTX_ENC_TYPE1_SIZE;
            *pCmdlSize = SALLD_CMDL_HEADER_SIZE_BYTES + 48;
            engId = SALLD_CMDL_ENGINE_ID_ACS1;
            break;    
            
        case sa_CipherMode_AES_CBC: 
        case sa_CipherMode_DES_CBC: 
        case sa_CipherMode_3DES_CBC: 
            *pScSize = SA_CTX_ENC_TYPE1_SIZE;
            *pCmdlSize = SALLD_CMDL_HEADER_SIZE_BYTES + ivSize;
            engId = SALLD_CMDL_ENGINE_ID_ES1;
            fRandowIV = TRUE;
            break;            
            
        default:
            *pScSize = SA_CTX_ENC_TYPE1_SIZE;
            *pCmdlSize = SALLD_CMDL_HEADER_SIZE_BYTES + ivSize;
            engId = SALLD_CMDL_ENGINE_ID_ES1;
            break;            
    }
    
    if(pEngId)*pEngId = engId;
    if(pfRandowIV)*pfRandowIV = fRandowIV;
    
}

/****************************************************************************
 * FUNCTION PURPOSE: Find the Authentication related information
 ****************************************************************************
 * DESCRIPTION: Derive the parameters related to the authentication operation
 *              based on the authentication mode
 *
 *  void salld_sc_auth_get_info(
 *            int16_t    mode,      -> Authentication mode
 *            uint16_t*    useEnc,    -> 1: Use Encryption Engine for Authentication  
 *            int16_t*   pCmdlSize, -> Command Label size
 *            int16_t*   pScSize,   -> Security Context size  
 *            uint8_t    pEngId)    -> Processing Engine ID 
 *
 ***************************************************************************/
void salld_sc_auth_get_info(int16_t mode, uint16_t* useEnc, int16_t*  pCmdlSize, 
                            int16_t* pScSize, uint8_t* pEngId) 
{

    uint8_t engId = SALLD_CMDL_ENGINE_ID_AS1;
    uint16_t fUseEnc = FALSE;
    
    *pCmdlSize = SALLD_CMDL_HEADER_SIZE_BYTES;
    
    switch (mode)
    {
        case sa_AuthMode_NULL:
            *pScSize = 0;
            *pCmdlSize = 0;
            engId = SALLD_CMDL_ENGINE_NONE;
            break;
    
        case sa_AuthMode_HMAC_MD5:
        case sa_AuthMode_HMAC_SHA1:
        case sa_AuthMode_HMAC_SHA2_224:
        case sa_AuthMode_HMAC_SHA2_256:
            *pScSize = SA_CTX_AUTH_TYPE2_SIZE;
            break;

        case sa_AuthMode_HMAC_SHA2_384:
        case sa_AuthMode_HMAC_SHA2_512:
            *pScSize = SA_CTX_AUTH_TYPE3_SIZE;
            break;

        case sa_AuthMode_GMAC:
            *pScSize = SA_GMAC_ENC_SC_SIZE;
            *pCmdlSize = SA_GMAC_CMDL_SIZE;
            engId = SA_GMAC_ENG_ID;
            fUseEnc = TRUE;
            break;
            
        case sa_AuthMode_GMAC_AH:
            *pScSize = SA_GMAC_ENC_SC_SIZE;
            *pCmdlSize = SA_GMAC_CMDL_SIZE_NOAAD;
            engId = SA_GMAC_ENG_ID;
            fUseEnc = TRUE;
            break;
            
        case sa_AuthMode_CMAC:
        case sa_AuthMode_AES_XCBC:
            *pScSize = SA_CMAC_ENC_SC_SIZE;
            *pCmdlSize = SA_CMAC_CMDL_SIZE;
            engId = SA_CMAC_ENG_ID;
            fUseEnc = TRUE;
            break;
            
        case sa_AuthMode_CBC_MAC:
            engId = SALLD_CMDL_ENGINE_ID_ES1;
            *pScSize = SA_CTX_ENC_TYPE1_SIZE;
            fUseEnc = TRUE;
            break;
            
        case sa_AuthMode_KASUMI_F9:
            *pScSize = SA_CTX_AUTH_TYPE2_SIZE;
            *pCmdlSize = SALLD_CMDL_HEADER_SIZE_BYTES + 8;
            engId = SALLD_CMDL_ENGINE_ID_ACS1;
            break;
            
        case sa_AuthMode_SNOW3G_F9:
        case sa_AuthMode_ZUC_F9:
            *pScSize = SA_CTX_AUTH_TYPE1_SIZE;
            *pCmdlSize = SALLD_CMDL_HEADER_SIZE_BYTES + 32;
            engId = SALLD_CMDL_ENGINE_ID_ACS1;
            break;
            
        default:
            *pScSize = SA_CTX_AUTH_TYPE1_SIZE;
            break;            
    }
    
    if(pEngId)*pEngId = engId;
    if(useEnc)*useEnc = fUseEnc;
}

/****************************************************************************
 * FUNCTION PURPOSE: Construct Security Context of the Encryption Engine
 ****************************************************************************
 * DESCRIPTION: Construct Security Context of the Encryption Engine
 *
 *  void salld_set_sc_enc(
 *            Sa_SecProto_e protoType    ->Security Protocol Type
 *            uint16_t   mode         -> Encryption mode
 *            uint16_t   keySize      -> Encryption Key Size
 *            uint16_t*  key1         -> Pointer to Encryption/Authentication key
 *            uint8_t    aadLen       -> length of the Additional Authenticated Data 
 *                                     in bytes
 *            uint16_t     enc          -> Encryption/Decryption (0/1)
 *            tword*   ctxBuf)      -> Pointer to the context Buffer               
 *                       
 ***************************************************************************/
void  salld_set_sc_enc(Sa_SecProto_e protoType, uint16_t mode,  uint16_t keySize,   
                       uint16_t* key, uint8_t aadLen, uint16_t enc, tword*  ctxBuf)
{
    uint8_t modeCtrl = SA_ENC_MODE_SEL_ENC;
    uint8_t simOpMode = SA_SIM_ES_ENC_MODE_NOT_SUPPORTED;
    const uint8_t *pMci = NULL;
    uint16_t invkey = FALSE;
    uint16_t validKeySize = FALSE;

    saCtxEnc_t* pCtxEnc = (saCtxEnc_t*)ctxBuf;
    uint16_t ghash[SALLD_AES_BLOCK_SIZE_IN_TUINT];

    /* Copy the Control words based on the encryption mode and the operation 
     * (encrypt/decrypt) */
    if ((keySize == 16u) ||
        (keySize == 24u) ||
        (keySize == 32u) )
    {
        validKeySize = TRUE;
    }

    /* keySize = 8 is also allowed for below modes */
    if ((mode == sa_CipherMode_DES_CBC) ||
        (mode == sa_CipherMode_3DES_CBC))
    {
        if (keySize == 8u)
        {
            validKeySize = TRUE;
        }
    }

    /* Do below operations only for valid KeySizes */
    if (validKeySize == TRUE)
    {
        switch (mode)
        {
            case sa_CipherMode_AES_CTR:
                simOpMode = SA_SIM_ES_ENC_MODE_AESCTR;
                pMci = &sa_eng_aes_enc_mci_tbl[SA_ENG_ALGO_CTR][SA_ENG_CONV_KEY_SIZE(keySize)][0];
                break;
                
            case sa_CipherMode_AES_F8:
                pMci = &sa_eng_aes_enc_mci_tbl[SA_ENG_ALGO_F8][SA_ENG_CONV_KEY_SIZE(keySize)][0];
                break;
                
            case sa_CipherMode_AES_CBC:
                pMci = (enc)?&sa_eng_aes_enc_mci_tbl[SA_ENG_ALGO_CBC][SA_ENG_CONV_KEY_SIZE(keySize)][0]:
                             &sa_eng_aes_dec_mci_tbl[SA_ENG_ALGO_CBC][SA_ENG_CONV_KEY_SIZE(keySize)][0];
                invkey = !enc;             
                break;
                
            case sa_CipherMode_DES_CBC:
                pMci = (enc)?&sa_eng_des_enc_mci_tbl[SA_ENG_ALGO_CBC][0]:
                             &sa_eng_des_dec_mci_tbl[SA_ENG_ALGO_CBC][0];
                break;
                
            case sa_CipherMode_3DES_CBC:
                pMci = (enc)?&sa_eng_3des_enc_mci_tbl[SA_ENG_ALGO_CBC][0]:
                             &sa_eng_3des_dec_mci_tbl[SA_ENG_ALGO_CBC][0];
                break;
                
                
            case sa_CipherMode_CCM:
                if (aadLen == 0) {
                  pMci = (enc)?&sa_eng_aes_enc_mci_tbl[SA_ENG_ALGO_CCM_NO_AAD][SA_ENG_CONV_KEY_SIZE(keySize)][0]:
                             &sa_eng_aes_dec_mci_tbl[SA_ENG_ALGO_CCM_NO_AAD][SA_ENG_CONV_KEY_SIZE(keySize)][0];
                }
                else {
                  pMci = (enc)?&sa_eng_aes_enc_mci_tbl[SA_ENG_ALGO_CCM][SA_ENG_CONV_KEY_SIZE(keySize)][0]:
                             &sa_eng_aes_dec_mci_tbl[SA_ENG_ALGO_CCM][SA_ENG_CONV_KEY_SIZE(keySize)][0];
                }
                break;            
                
            case sa_CipherMode_GCM:
                /* Store LENA in the security context */
                simOpMode = SA_SIM_ES_ENC_MODE_GCM;
                pMci = (enc)?&sa_eng_aes_enc_mci_tbl[SA_ENG_ALGO_GCM][SA_ENG_CONV_KEY_SIZE(keySize)][0]:
                             &sa_eng_aes_dec_mci_tbl[SA_ENG_ALGO_GCM][SA_ENG_CONV_KEY_SIZE(keySize)][0];
                pktWrite8bits_m((tword *)pCtxEnc->aux1, SA_GCM_ENC_AUX1_LENA_OFFSET, (aadLen << 3));
                /* pass through */
            
            case sa_AuthMode_GMAC:
                /* Derive and copy the Galios Hash */
                if (simOpMode == SA_SIM_ES_ENC_MODE_NOT_SUPPORTED)
                {
                    pMci = (protoType == sa_PT_IPSEC_AH)?
                        &sa_eng_ah_gmac_mci_tbl[SA_ENG_CONV_KEY_SIZE(keySize)][0]:
                        &sa_eng_aes_enc_mci_tbl[SA_ENG_ALGO_GMAC][SA_ENG_CONV_KEY_SIZE(keySize)][0];
                    
                }    
                salld_aes_gcm_get_ghash((tword *)key, keySize << 3, (tword *)ghash);
                misc_utlCopy(ghash, (uint16_t *)pCtxEnc->aux1,
                             SALLD_AES_BLOCK_SIZE_IN_TUINT);
                break;   
                
            case sa_AuthMode_GMAC_AH:
                /* Derive and copy the Galios Hash */
                if (simOpMode == SA_SIM_ES_ENC_MODE_NOT_SUPPORTED)
                {
                    pMci = &sa_eng_ah_gmac_mci_tbl[SA_ENG_CONV_KEY_SIZE(keySize)][0];
                    
                }    
                salld_aes_gcm_get_ghash((tword *)key, keySize << 3, (tword *)ghash);
                misc_utlCopy(ghash, (uint16_t *)pCtxEnc->aux1,
                             SALLD_AES_BLOCK_SIZE_IN_TUINT);
                break;   
                
            case sa_AuthMode_CMAC:
            case sa_AuthMode_AES_XCBC:
                simOpMode = SA_SIM_ES_AUTH_MODE_CMAC;
                pMci = &sa_eng_aes_enc_mci_tbl[SA_ENG_ALGO_CMAC][SA_ENG_CONV_KEY_SIZE(keySize)][0];
                break;
                
            case sa_AuthMode_CBC_MAC:
                pMci = &sa_eng_aes_enc_mci_tbl[SA_ENG_ALGO_CBCMAC][SA_ENG_CONV_KEY_SIZE(keySize)][0];
                break;
                 
            default:
                break;     
        }
    }

    SA_SIM_ES_SET_FUNC_INDEX(modeCtrl, simOpMode);
    pktWrite8bits_m(ctxBuf, 0, modeCtrl);
    
    if (pMci)
    {
        /* Copy the mode control instructions */
        pktPackBytesIntoWords((tword *)pMci, ctxBuf, SA_ENG_MAX_MCI_SIZE, 1);
    }
    
    /* Copy the keys */
    if (invkey)
    {
        aesInvKey((tword *)pCtxEnc->encKey, (tword *)key, (tint)(keySize << 3));
    }  
    else if (mode == sa_AuthMode_AES_XCBC)
    {
        salld_aes_xcbc_get_subkey((tword *)key, (tint)(keySize << 3),
                                   0x1, 
                                  (tword *)pCtxEnc->encKey);
    
    }
    else
    {
        misc_utlCopy(key, (uint16_t *)pCtxEnc->encKey,
                    SALLD_BYTE_TO_TUINT(keySize)); 
    }           
                 
}


/****************************************************************************
 * FUNCTION PURPOSE: Construct Security Context of the Authentication Engine
 ****************************************************************************
 * DESCRIPTION: Construct Security Context of the Authentication Engine
 *
 *  void salld_set_sc_auth(
 *            uint16_t   mode     -> Authentication mode
 *            uint16_t   keySize  -> Authentication Key Size
 *            uint16_t*  key      -> Pointer to the authentication key if any
 *            tword*   ctxBuf)  -> Pointer to the context Buffer               
 *                       
 ***************************************************************************/
void salld_set_sc_auth(uint16_t mode,  uint16_t keySize,  uint16_t* key, tword*  ctxBuf)
{

    uint16_t modeCtrl = SA_AUTH_MODE_SEL_AUTH | SA_AUTH_FLAG_UPLOAD_HASH;
    uint16_t iPad[SALLD_HMAC_MAX_DIGEST_LEN_IN_TUINT] = { 0 };
    uint16_t oPad[SALLD_HMAC_MAX_DIGEST_LEN_IN_TUINT] = { 0 };
    saCtxAuth_t* pCtxAuth = (saCtxAuth_t*)ctxBuf;
    int16_t  hashSize = 0;
    uint16_t useKey = TRUE;
    
    switch (mode)
    {
        case sa_AuthMode_MD5:
            modeCtrl |= (SA_AUTH_FLAG_DIS_HMAC|SA_AUTH_HASH_MODE_MD5);
            useKey = FALSE;
            break; 
        case sa_AuthMode_SHA1:
            modeCtrl |= (SA_AUTH_FLAG_DIS_HMAC|SA_AUTH_HASH_MODE_SHA1);
            useKey = FALSE;
            break; 
        case sa_AuthMode_SHA2_224:
            modeCtrl |= (SA_AUTH_FLAG_DIS_HMAC|SA_AUTH_HASH_MODE_SHA2_224);
            useKey = FALSE;
            break; 
        case sa_AuthMode_SHA2_256:
            modeCtrl |= (SA_AUTH_FLAG_DIS_HMAC|SA_AUTH_HASH_MODE_SHA2_256);
            useKey = FALSE;
            break; 

        case sa_AuthMode_SHA2_384:
            modeCtrl |= (SA_AUTH_FLAG_DIS_HMAC|SA_AUTH_HASH_MODE_SHA2_384);
            useKey = FALSE;
            break;

        case sa_AuthMode_SHA2_512:
            modeCtrl |= (SA_AUTH_FLAG_DIS_HMAC|SA_AUTH_HASH_MODE_SHA2_512);
            useKey = FALSE;
            break;

        case sa_AuthMode_HMAC_MD5:
            modeCtrl |= SA_AUTH_HASH_MODE_MD5;
            hashSize = SALLD_HMAC_MD5_DIGEST_LEN_IN_TUINT;
            salld_hmac_md5_get_pad(key, keySize, iPad, oPad);
            break;
                
        case sa_AuthMode_HMAC_SHA1:
            modeCtrl |= SA_AUTH_HASH_MODE_SHA1;
            hashSize = SALLD_HMAC_SHA1_DIGEST_LEN_IN_TUINT;
            salld_hmac_sha1_get_pad(key, keySize, iPad, oPad);
            break;
        
        case sa_AuthMode_HMAC_SHA2_224:
            modeCtrl |= SA_AUTH_HASH_MODE_SHA2_224;
            hashSize = SALLD_HMAC_SHA2_DIGEST_LEN_IN_TUINT;
            salld_hmac_sha224_get_pad(key, keySize, iPad, oPad);
            break;
            
        case sa_AuthMode_HMAC_SHA2_256:
            modeCtrl |= SA_AUTH_HASH_MODE_SHA2_256;
            hashSize = SALLD_HMAC_SHA2_DIGEST_LEN_IN_TUINT;
            salld_hmac_sha256_get_pad(key, keySize, iPad, oPad);
            break;
#if defined(NSS_LITE2)
        case sa_AuthMode_HMAC_SHA2_384:
            modeCtrl |= SA_AUTH_HASH_MODE_SHA2_384;
            hashSize = SALLD_HMAC_SHA512_DIGEST_LEN_IN_TUINT;
            salld_hmac_sha384_get_pad(key, keySize, iPad, oPad);
            break;

        case sa_AuthMode_HMAC_SHA2_512:
            modeCtrl |= SA_AUTH_HASH_MODE_SHA2_512;
            hashSize = SALLD_HMAC_SHA512_DIGEST_LEN_IN_TUINT;
            salld_hmac_sha512_get_pad(key, keySize, iPad, oPad);
            break;
#endif
        default:
            /* Error Handling: */
            return;
    }
    
    pktWrite16bits_m(ctxBuf, 0, modeCtrl);
    
    /* Copy the Keys or iPad/oPad */
    /* Update for the next block */
    if ( (mode == sa_AuthMode_HMAC_SHA2_384) ||
         (mode == sa_AuthMode_HMAC_SHA2_512))
    {
        /* Copy the Keys or iPad/oPad */
        if (useKey)
        {

            /* Copy in two chunks */
            hashSize = hashSize/2;
            misc_utlCopy(iPad, (uint16_t *)pCtxAuth->aux2,
                         hashSize);
            misc_utlCopy(oPad, (uint16_t *)pCtxAuth->aux3,
                         hashSize);
            misc_utlCopy(&iPad[hashSize], (uint16_t *)pCtxAuth->authKey,
                         hashSize);
            misc_utlCopy(&oPad[hashSize], (uint16_t *)pCtxAuth->aux1,
                         hashSize);
        }
    }
    else
    {
        if ( (useKey) && (hashSize  < (SA_AUTH_KEY_SIZE_IN_UINT32 << 2u)) )
        {
            misc_utlCopy(iPad, (uint16_t *)pCtxAuth->authKey,
                         hashSize); 
            misc_utlCopy(oPad, (uint16_t *)pCtxAuth->aux1,
                         hashSize); 
        }
    }
} 

#if !defined(NSS_LITE) && !defined(NSS_LITE2)

/****************************************************************************
 * FUNCTION PURPOSE: Construct Security Context of the Air Ciphering Engine
 ****************************************************************************
 * DESCRIPTION: Construct Security Context of the Air Ciphering Engine
 *
 *  void salld_set_sc_acEnc(
 *            uint16_t   mode     -> Encryption mode
 *            uint16_t   keySize  -> Encryption Key Size
 *            uint16_t*  key1     -> Pointer to the Encryption Key
 *            uint16_t*  key2     -> Pointer to the Authentication Key (For F8F9 operation only)
 *            uint16_t   enc      -> Encryption/Decryption (0/1)
 *            tword*     ctxBuf   -> Pointer to the context Buffer  
 *            int16_t*   algorithm, -> Air Ciphering algorithm             
 *            int        fSassGen2) -> Flag indicates whether this is a 2nd generation SASS
 *
 *                       
 ***************************************************************************/
void  salld_set_sc_acEnc(uint16_t mode,  uint16_t keySize,  uint16_t*  key, uint16_t*  key2,
                         uint16_t enc, tword* ctxBuf, int16_t* algorithm, int fSassGen2)
{

    uint8_t modeCtrl = SA_ENC_MODE_SEL_ENC;
    uint8_t simOpMode = SA_SIM_AC_ENC_MODE_NOT_SUPPORTED;
    const uint8_t *pMci = NULL;
    uint16_t modKey[8];
    int i;
    
    /* Copy the Control words based on the encryption mode and the operation 
     * (encrypt/decrypt) */
    switch (mode)
    {
        case sa_CipherMode_AES_CTR:
            *algorithm = SA_AC_ALGORITHM_AES;
            pMci = &sa_eng_aes_enc_mci_tbl[SA_ENG_ALGO_CTR][SA_ENG_CONV_KEY_SIZE(keySize)][0];
            break;
            
        case sa_CipherMode_GSM_A53:
        case sa_CipherMode_ECSD_A53:
        case sa_CipherMode_GEA3:
            *algorithm = SA_AC_ALGORITHM_GSM_A53;
            simOpMode = SA_SIM_AC_ENC_MODE_KASUMI_F8;    
            pMci = (fSassGen2)?&sa_eng_kasumi_enc_mci_tbl2[SA_ENG_ALGO_F8 - SA_KASUMI_FIRST_ALGO][0]:
                               &sa_eng_kasumi_enc_mci_tbl[SA_ENG_ALGO_F8 - SA_KASUMI_FIRST_ALGO][0];
            break;
            
        case sa_CipherMode_KASUMI_F8:
            *algorithm = SA_AC_ALGORITHM_KASUMI;
            simOpMode = SA_SIM_AC_ENC_MODE_KASUMI_F8;    
            pMci = (fSassGen2)?&sa_eng_kasumi_enc_mci_tbl2[SA_ENG_ALGO_F8 - SA_KASUMI_FIRST_ALGO][0]:
                               &sa_eng_kasumi_enc_mci_tbl[SA_ENG_ALGO_F8 - SA_KASUMI_FIRST_ALGO][0];
                   
            break;
            
        case sa_CipherMode_SNOW3G_F8:
            *algorithm = SA_AC_ALGORITHM_SNOW3G;
            simOpMode = SA_SIM_AC_ENC_MODE_SNOW3G_F8;           
            pMci = (fSassGen2)?&sa_eng_snow3g_enc_mci_tbl2[SA_ENG_ALGO_F8 - SA_SNOW3G_FIRST_ALGO][0]:
                               &sa_eng_snow3g_enc_mci_tbl[SA_ENG_ALGO_F8 - SA_SNOW3G_FIRST_ALGO][0];
            break;                 
            
        case sa_CipherMode_ZUC_F8:
            *algorithm = SA_AC_ALGORITHM_ZUC;
            pMci = &sa_eng_zuc_enc_mci_tbl2[SA_ENG_ALGO_F8 - SA_ZUC_FIRST_ALGO][0];
            break;
            
        case sa_CipherMode_SNOW3G_F8F9:
            *algorithm = SA_AC_ALGORITHM_SNOW3G;
            pMci = &sa_eng_snow3g_enc_mci_tbl2[SA_ENG_ALGO_F8F9 - SA_SNOW3G_FIRST_ALGO][0];
            break;
            
        case sa_CipherMode_ZUC_F8F9:
            *algorithm = SA_AC_ALGORITHM_ZUC;
            pMci = &sa_eng_zuc_enc_mci_tbl2[SA_ENG_ALGO_F8F9 - SA_ZUC_FIRST_ALGO][0];
            break;
            
        default:
            break;     
    } 
    
    SA_SIM_AC_SET_FUNC_INDEX(modeCtrl, simOpMode);
    pktWrite8bits_m(ctxBuf, 0, modeCtrl);
    
    if (pMci)
    {
        /* Copy the mode control instructions */
        pktPackBytesIntoWords((tword *)pMci, ctxBuf, SA_ENG_MAX_MCI_SIZE, 1);
    }
     
    /* Copy the keys */
    if (mode == sa_CipherMode_SNOW3G_F8)
    {
        /* reverse the order of the CK as K0||K1||K2||K3 */
        misc_utlCopy(&key[6], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key)),
                     2); 
        misc_utlCopy(&key[4], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 4),
                     2); 
        misc_utlCopy(&key[2], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 8),
                     2); 
        misc_utlCopy(&key[0], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 12),
                     2); 
    }
    else if ((mode == sa_CipherMode_AES_CTR)  || (mode == sa_CipherMode_ZUC_F8))
    {
        misc_utlCopy(key, (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key)),
                    SALLD_BYTE_TO_TUINT(keySize)); 
    } 
    else if (mode == sa_CipherMode_SNOW3G_F8F9)
    {
    
        /* reverse the order of the CK as K0||K1||K2||K3 */
        misc_utlCopy(&key[6], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key)),
                     2); 
        misc_utlCopy(&key[4], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 4),
                     2); 
        misc_utlCopy(&key[2], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 8),
                     2); 
        misc_utlCopy(&key[0], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 12),
                     2); 
    
        /* reverse the order of the CK as K0||K1||K2||K3 */
        misc_utlCopy(&key2[6], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 16),
                     2); 
        misc_utlCopy(&key2[4], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 20),
                     2); 
        misc_utlCopy(&key2[2], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 24),
                     2); 
        misc_utlCopy(&key2[0], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 28),
                     2); 
    
    }
    else if (mode == sa_CipherMode_ZUC_F8F9)
    {
        misc_utlCopy(key, (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key)),
                    SALLD_BYTE_TO_TUINT(keySize)); 
                    
        misc_utlCopy(key2, (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 16),
                     SALLD_BYTE_TO_TUINT(keySize)); 
    }
    else
    {
        misc_utlCopy(key, (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key)),
                    SALLD_BYTE_TO_TUINT(keySize)); 
                    
        /* Construct and store the Kasumi Modify Key */
        for (i = 0; i < 8; i++)
        {
            modKey[i] = key[i] ^ 0x5555;
        }            
                    
        /* Copy 16-byte KM into Aux1 */ 
        misc_utlCopy(modKey, (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, aux1)), 8);
    }             
}

/****************************************************************************
 * FUNCTION PURPOSE: Construct Security Context of the Air Ciphering Engine
 ****************************************************************************
 * DESCRIPTION: Construct Security Context of the Air Ciphering Engine
 *
 *  void salldSetScAc(
 *            uint16_t   mode         -> Authentication mode
 *            uint16_t   keySize      -> Authentication Key Size
 *            uint16_t*  key          -> Pointer to the Authentication Key
 *            tword*     ctxBuf,      -> Pointer to the context Buffer               
 *            int16_t*   algorithm,   -> Air Ciphering algorithm
 *            int16_t    dir,         -> (0:uplink 1:downlink)  
 *            int        fSassGen2)   -> Flag indicates whether this is a 2nd generation SASS
 *                       
 ***************************************************************************/
void salld_set_sc_acAuth(uint16_t mode,  uint16_t keySize,  uint16_t*  key, 
                         tword*  ctxBuf, int16_t* algorithm, int16_t dir, int fSassGen2)
{

    uint8_t modeCtrl = SA_ENC_MODE_SEL_ENC;
    uint8_t simOpMode = SA_SIM_AC_AUTH_MODE_NOT_SUPPORTED;
    const uint8_t *pMci = NULL;
    uint16_t modKey[8];
    int i;
    
    /* Copy the Control words based on the encryption mode and the operation 
     * (encrypt/decrypt) */
    switch (mode)
    {
        case sa_AuthMode_KASUMI_F9:
            *algorithm = SA_AC_ALGORITHM_KASUMI;
            simOpMode = SA_SIM_AC_AUTH_MODE_KASUMI_F9;
            pMci = (fSassGen2)?&sa_eng_kasumi_auth_mci_tbl2[dir][0]:
                               &sa_eng_kasumi_auth_mci_tbl[dir][0];  
            break;
            
        case sa_AuthMode_SNOW3G_F9:
            *algorithm = SA_AC_ALGORITHM_SNOW3G;
            pMci = &sa_eng_snow3g_auth_mci_tbl2[0][0];
            break;

        case sa_AuthMode_ZUC_F9:
            *algorithm = SA_AC_ALGORITHM_ZUC;
            pMci = &sa_eng_zuc_auth_mci_tbl2[0][0];
            break;
            
        case sa_AuthMode_CMAC:
            *algorithm = SA_AC_ALGORITHM_AES;
            pMci = &sa_eng_aes_enc_mci_tbl[SA_ENG_ALGO_CMAC][SA_ENG_CONV_KEY_SIZE(keySize)][0];
            break;
            
        default:
            break;     
    } 
    
    SA_SIM_AC_SET_FUNC_INDEX(modeCtrl, simOpMode);
    pktWrite8bits_m(ctxBuf, 0, modeCtrl);

    if (pMci)
    {
        /* Copy the mode control instructions */
        pktPackBytesIntoWords((tword *)pMci, ctxBuf, SA_ENG_MAX_MCI_SIZE, 1);
    }
    
    /* Copy the keys */
    if (mode == sa_AuthMode_SNOW3G_F9)
    {
        /* reverse the order of the CK as K0||K1||K2||K3 */
        misc_utlCopy(&key[6], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 16),
                     2); 
        misc_utlCopy(&key[4], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 20),
                     2); 
        misc_utlCopy(&key[2], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 24),
                     2); 
        misc_utlCopy(&key[0], (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 28),
                     2); 
    }
    else if (mode == sa_AuthMode_ZUC_F9)
    {
        /* Copy the keys */
        misc_utlCopy(key, (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key) + 16),
                     SALLD_BYTE_TO_TUINT(keySize)); 
    }
    else
    {
        /* Copy the keys */
        misc_utlCopy(key, (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, key)),
                    SALLD_BYTE_TO_TUINT(keySize)); 
    }             
                 
   /* Construct and store the Kasumi Modify Key */
   if (mode == sa_AuthMode_KASUMI_F9)
   {
   
        for (i = 0; i < 8; i++)
        {
            modKey[i] = key[i] ^ 0xAAAA;
        }            
               
        /* Copy 16-byte KM into Aux1 */ 
        misc_utlCopy(modKey, (uint16_t *)(ctxBuf + SALLD_FIELDOFFSET(saCtxAc_t, aux1)), 8);
   }
}

#endif
   
/* Nothing past this point */

