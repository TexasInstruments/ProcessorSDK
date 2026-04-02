/******************************************************************************
 * FILE PURPOSE: SALLD Automation commands.
 ******************************************************************************
 * FILE NAME:   salldcfg.c
 *
 * DESCRIPTION: Defines the SALLD module unit test configuration.
 *
 * (C) Copyright 2009-2014, Texas Instruments, Inc.
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
#include <ti/drv/sa/salld.h>
#include "../unittest.h"
#include "salldcfg.h"

static uint8_t salldsim_key1[32] = 
        #if 1
        {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
         0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
        #else
        {0x73, 0xf3, 0x97, 0xe4, 0x1d, 0xe9, 0x25, 0xe5, 0x9e, 0x06, 0xd6, 0x40, 0x03, 0x91, 0x2a, 0xe6, 
         0xa5, 0x24, 0xfb, 0x09, 0x12, 0x9c, 0x14, 0xde, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
        #endif 
static uint8_t salldsim_key2[32] =
        #if 1
        {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 
         0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
        #else
        {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
         0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11}; 
        #endif
static uint8_t salldsim_key3[32] = 
        {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x00, 
         0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x00};
static uint8_t salldsim_key4[32] = 
        {0x1F, 0x1E, 0x1D, 0x1C, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x00,
         0x1F, 0x1E, 0x1D, 0x1C, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x00};
        
        
static uint8_t salldsim_salt1[16] = 
        {0x0F, 0x0E, 0x0D, 0x0C, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x00, };
static uint8_t salldsim_salt2[4] = {0xde, 0xad, 0xbe, 0xef};
        
static uint8_t salldsim_encKey[salld_SIM_MAX_CHANNELS][32];
static uint8_t salldsim_macKey[salld_SIM_MAX_CHANNELS][32];

/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "GENCFG" handler function for IPSEC ESP.
 ******************************************************************************
 * DESCRIPTION: IPSEC configuration handler for "GENCFG"
 *
 *  void salldcfg_set_gencfg_esp (
 *     salldSimCommConfig_t*    pCommCfg,
 *     salldSimIpsecConfig_t*   pIpsecCfg, 
 *     Sa_IpsecConfigParams_t*  pIpsecParams)
 *
 *****************************************************************************/
static void salldcfg_set_gencfg_esp (
        salldSimCommConfig_t*    pCommCfg,
        salldSimIpsecConfig_t*   pIpsecCfg, 
        Sa_IpsecConfigParams_t*  pIpsecParams)
{
    /* Initialize all parameters to its default value */
    pIpsecParams->transportType = sa_IPSEC_TRANSPORT_TUNNEL; /* It is not used at LLD */
    pIpsecParams->spi = pIpsecCfg->spi;
    pIpsecParams->ctrlBitMap = (pIpsecCfg->esn)?sa_IPSEC_CONFIG_ESN:0;
    pIpsecParams->sessionEncKeySize     = pIpsecCfg->encKeySize;
    pIpsecParams->macSize               = pIpsecCfg->macSize; 
    pIpsecParams->sessionMacKeySize     = pIpsecCfg->macKeySize;    
    pIpsecParams->esnLo                 = 0x00000000;
    pIpsecParams->esnHi                 = 0x66666666;
    pIpsecParams->encryptionBlockSize   = 4;
    pIpsecParams->ivSize                = 8;
    pIpsecParams->sessionSaltSize       = 4; 
 
    /* Reset the cipher mode specific parameters */
    switch (pCommCfg->cipherMode)
    {
        case sa_CipherMode_CCM:
            pIpsecParams->sessionSaltSize = 3;
            break;
            
        case sa_CipherMode_AES_CBC:
            pIpsecParams->encryptionBlockSize   = 16;
            pIpsecParams->ivSize                = 16;
            pIpsecParams->sessionSaltSize       = 0;
            break;
        
        case sa_CipherMode_DES_CBC:
        case sa_CipherMode_3DES_CBC:
            pIpsecParams->encryptionBlockSize = 8;
            pIpsecParams->sessionSaltSize = 0;    
            break;  
            
        case sa_CipherMode_NULL:
            if (pCommCfg->authMode != sa_AuthMode_GMAC)
            {
                pIpsecParams->encryptionBlockSize = 1;
                pIpsecParams->sessionSaltSize = 0;    
                pIpsecParams->ivSize          = 0;
            }
            break;              
            
        case sa_CipherMode_AES_CTR:
        case sa_CipherMode_GCM:
        default:
            break;    
    }
    
    switch (pCommCfg->authMode)
    {
        case sa_AuthMode_HMAC_MD5:
        case sa_AuthMode_CMAC:
        case sa_AuthMode_AES_XCBC:
            pIpsecParams->sessionMacKeySize = 16;
            break;
            
        case sa_AuthMode_HMAC_SHA1:
            pIpsecParams->sessionMacKeySize = 20;
            break;
            
        case sa_AuthMode_HMAC_SHA2_224:
        case sa_AuthMode_HMAC_SHA2_256:
            pIpsecParams->sessionMacKeySize = 32;
            break;

        case sa_AuthMode_HMAC_SHA2_384:
        case sa_AuthMode_HMAC_SHA2_512:
            pIpsecParams->sessionMacKeySize = 64;
            break;

                        
        case sa_AuthMode_GMAC:
        default:
            break;        
    }
}      

/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "GENCFG" handler function for IPSEC AH.
 ******************************************************************************
 * DESCRIPTION: IPSEC configuration handler for "GENCFG"
 *
 *  void salldcfg_set_gencfg_ah (
 *     salldSimCommConfig_t*    pCommCfg,
 *     salldSimIpsecConfig_t*   pIpsecCfg, 
 *     Sa_IpsecConfigParams_t*  pIpsecParams)
 *
 *****************************************************************************/
static void salldcfg_set_gencfg_ah (
        salldSimCommConfig_t*    pCommCfg,
        salldSimIpsecConfig_t*   pIpsecCfg, 
        Sa_IpsecConfigParams_t*  pIpsecParams)
{
    /* Initialize all parameters to its default value */
    pIpsecParams->transportType = sa_IPSEC_TRANSPORT_TUNNEL; /* It is not used at LLD */
    pIpsecParams->spi = pIpsecCfg->spi;
    pIpsecParams->ctrlBitMap = (pIpsecCfg->esn)?sa_IPSEC_CONFIG_ESN:0;
    pIpsecParams->sessionEncKeySize     = pIpsecCfg->encKeySize;
    pIpsecParams->sessionMacKeySize     = pIpsecCfg->macKeySize;    
    pIpsecParams->macSize               = pIpsecCfg->macSize; 
    pIpsecParams->esnLo                 = 0;
    pIpsecParams->esnHi                 = 0x77777777;
    pIpsecParams->encryptionBlockSize   = 0;
    pIpsecParams->ivSize                = 0;
    pIpsecParams->sessionSaltSize       = 0; 
 
    switch (pCommCfg->authMode)
    {
        case sa_AuthMode_GMAC:
            pIpsecParams->encryptionBlockSize   = 4;
            pIpsecParams->ivSize                = 8;
            pIpsecParams->sessionSaltSize       = 4; 
            break;
    
        case sa_AuthMode_HMAC_MD5:
        case sa_AuthMode_CMAC:
        case sa_AuthMode_AES_XCBC:
            pIpsecParams->sessionMacKeySize = 16;
            break;
            
        case sa_AuthMode_HMAC_SHA1:
            pIpsecParams->sessionMacKeySize = 20;
            break;
            
        case sa_AuthMode_HMAC_SHA2_224:
        case sa_AuthMode_HMAC_SHA2_256:
            pIpsecParams->sessionMacKeySize = 32;
            break;

        case sa_AuthMode_HMAC_SHA2_384:
        case sa_AuthMode_HMAC_SHA2_512:
            pIpsecParams->sessionMacKeySize = 64;
            break;

                        
        default:
            break;        
    }
}  

/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "GENCFG" handler function for SRTP.
 ******************************************************************************
 * DESCRIPTION: SRTP configuration handler for "GENCFG"
 *
 *  void salldcfg_set_gencfg_srtp (
 *     salldSimCommConfig_t*    pCommCfg,
 *     salldSimSrtoConfig_t*    pSrtpCfg, 
 *     Sa_SrtpConfigParams_t*  pSrtpParams)
 *
 *****************************************************************************/
static void salldcfg_set_gencfg_srtp (
        salldSimCommConfig_t*    pCommCfg,
        salldSimSrtpConfig_t*    pSrtpCfg, 
        Sa_SrtpConfigParams_t*   pSrtpParams)
{
    /* Initialize all parameters to its default value */
    pSrtpParams->macSize               = pSrtpCfg->macSize; 
    pSrtpParams->masterKeySize         = 16;
    pSrtpParams->masterSaltSize        = 14;
    pSrtpParams->sessionEncKeySize     = 16;    
    pSrtpParams->sessionMacKeySize     = (pCommCfg->authMode == sa_AuthMode_HMAC_SHA1)?20:16; 
    pSrtpParams->sessionSaltSize       = 14; 
}      

/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "GENCFG" handler function for AC.
 ******************************************************************************
 * DESCRIPTION: Air Ciphering configuration handler for "GENCFG"
 *
 *  void salldcfg_set_gencfg_ac (
 *     salldSimCommConfig_t* pCommCfg,
 *     salldSimAcConfig_t*   pAcCfg, 
 *     Sa_AcConfigParams_t*  pAcParams)
 *
 *****************************************************************************/
static void salldcfg_set_gencfg_ac (
        salldSimCommConfig_t* pCommCfg,
        salldSimAcConfig_t*   pAcCfg, 
        Sa_AcConfigParams_t*  pAcParams)
{
    /* Initialize all parameters to its default value */
    pAcParams->ctrlBitMap = 0x15 | (pAcCfg->upLink?0:sa_AC_CONFIG_DIR);
    pAcParams->pduType    = pAcCfg->pduType;
    pAcParams->countC     = 0x33330000;
    pAcParams->fresh      = 0xbabebeef;  
    pAcParams->ivLow26    = 0; 
    pAcParams->sessionEncKeySize = 0;
    pAcParams->macSize           = 0; 
    pAcParams->sessionMacKeySize = 0;    
    pAcParams->ivSize            = 0;
    
    if(pAcCfg->pduType == sa_AcPduType_LTE_CP)
        pAcParams->ctrlBitMap |= sa_AC_CONFIG_COPY_COUNTC;    
    
    if(pAcCfg->pduType == sa_AcPduType_LTE)
        pAcParams->ctrlBitMap |= (pAcCfg->countCPresent?sa_AC_CONFIG_COUNTC_BY_APP:0);    
 
    pAcParams->ctrlBitMap |= (pAcCfg->intKey?sa_AC_CONFIG_KEY_IN_SCRATCH:0);    
 
    /* Reset the cipher mode specific parameters */
    switch (pCommCfg->cipherMode)
    {
        case sa_CipherMode_AES_CTR:
        case sa_CipherMode_SNOW3G_F8:
        case sa_CipherMode_ZUC_F8:
            pAcParams->sessionEncKeySize = 16;
            pAcParams->ivSize            = 16;
            break;
            
        case sa_CipherMode_KASUMI_F8:
        case sa_CipherMode_GSM_A53:
        case sa_CipherMode_GEA3:
            pAcParams->sessionEncKeySize = 16;
            pAcParams->ivSize            = 8;
            break;
            
        case sa_CipherMode_ECSD_A53:
            pAcParams->sessionEncKeySize = 16;
            pAcParams->ivSize            = 8;
            break;
        
        default:
            break;    
    }
    
    switch (pCommCfg->authMode)
    {
    
        case sa_AuthMode_CMAC:
            pAcParams->sessionMacKeySize = 16;
            pAcParams->macSize = 4;
            break;
    
        case sa_AuthMode_KASUMI_F9:
            pAcParams->sessionMacKeySize = 16;
            pAcParams->macSize = 4;
            pAcParams->ivSize  = 8;
            break;
            
        case sa_AuthMode_SNOW3G_F9:
        case sa_AuthMode_ZUC_F9:
            pAcParams->sessionMacKeySize = 16;
            pAcParams->macSize = 4;
            pAcParams->ivSize  = 16;
            break;
            
        default:
            break;        
    }
}    

/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "GENCFG" handler function for Data Mode.
 ******************************************************************************
 * DESCRIPTION: Air Ciphering configuration handler for "GENCFG"
 *
 *  void salldcfg_set_gencfg_data (
 *     salldSimCommConfig_t* pCommCfg,
 *     salldSimDataModeConfig_t*   pDataModeCfg, 
 *     Sa_DataModeConfigParams_t*  pDataModeParams)
 *
 *****************************************************************************/
static void salldcfg_set_gencfg_data (
        salldSimCommConfig_t* pCommCfg,
        salldSimDataModeConfig_t*   pDataModeCfg, 
        Sa_DataModeConfigParams_t*  pDataModeParams)
{
    /* Initialize all parameters to its default value */
    pDataModeParams->ctrlBitMap        = pDataModeCfg->ctrlBitMap;
    pDataModeParams->sessionEncKeySize = pDataModeCfg->encKeySize;
    pDataModeParams->sessionMacKeySize = pDataModeCfg->macKeySize;
    pDataModeParams->sessionSaltSize   = pDataModeCfg->saltSize;
    pDataModeParams->macSize           = pDataModeCfg->macSize;
    pDataModeParams->ivSize            = pDataModeCfg->ivSize;
    pDataModeParams->aadSize           = pDataModeCfg->aadSize;
    pDataModeParams->enc               = pDataModeCfg->enc;
    pDataModeParams->enc1st            = pDataModeCfg->enc1st;
#if defined(NSS_LITE2)
    pDataModeParams->priv              = pDataModeCfg->priv;
    pDataModeParams->privId            = pDataModeCfg->privId;
#endif
    return;
}      
  
/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "GENCFG" handler function.
 ******************************************************************************
 * DESCRIPTION: UT configuration handler for "GENCFG"
 *
 *  void salldcfg_set_gencfg (
 *     salldSimCommConfig_t* pCommCfg,
 *     void* pProtoCfg,  
 *     Sa_GenConfigParams_t *pGenCfg, 
 *     Bool tx)
 *
 *****************************************************************************/
void salldcfg_set_gencfg (salldSimCommConfig_t* pCommCfg, void* pProtoCfg, Sa_GenConfigParams_t *pGenCfg, Bool tx)
{

    switch (pCommCfg->protocolType)
    {
        case sa_PT_NULL:
            salldcfg_set_gencfg_data(pCommCfg, (salldSimDataModeConfig_t*)pProtoCfg, 
                                     &pGenCfg->params.data);
        break;
    
        case sa_PT_SRTP:
        case sa_PT_SRTCP:
            salldcfg_set_gencfg_srtp(pCommCfg, (salldSimSrtpConfig_t*)pProtoCfg, 
                                     &pGenCfg->params.srtp);
        break;
        
        case sa_PT_IPSEC_AH:
            salldcfg_set_gencfg_ah(pCommCfg, (salldSimIpsecConfig_t*)pProtoCfg, 
                                   &pGenCfg->params.ipsec);
        break;
        
        case sa_PT_IPSEC_ESP:
            salldcfg_set_gencfg_esp(pCommCfg, (salldSimIpsecConfig_t*)pProtoCfg, 
                                    &pGenCfg->params.ipsec);
        break;
        
        case sa_PT_3GPP_AC:
            salldcfg_set_gencfg_ac(pCommCfg, (salldSimAcConfig_t*)pProtoCfg, 
                                   &pGenCfg->params.ac);
        break;
        
        default:
        
        break;
    }  
}

/******************************************************************************
 * FUNCTION PURPOSE: Set default IPSEC Key Parameters
 ******************************************************************************
 * DESCRIPTION: Set default IPSEC Key parameters
 *****************************************************************************/
static void salldcfg_set_keycfg_default(Sa_IpsecKeyParams_t* pKeyParams, 
                                        int16_t chnum, uint8_t* encKey,
                                        uint8_t* authKey, uint8_t* saltKey)
{
                                          
    uint8_t* pEncKey = &salldsim_encKey[chnum - 1][0];
    uint8_t* pMacKey = &salldsim_macKey[chnum - 1][0];

    /* Initialize all parameters to its default value */
    pKeyParams->ctrlBitfield = 0;
    pKeyParams->sessionEncKey = pEncKey;
    pKeyParams->sessionAuthKey = pMacKey;
    pKeyParams->sessionSalt = saltKey;   
    
    memcpy(pEncKey, encKey, 32);
    memcpy(pMacKey, authKey, 32);
    
    /* Update key based on the channel number */
    pMacKey[0] = pEncKey[0] = chnum >> 8;
    pMacKey[1] = pEncKey[1] = chnum & 0xFF;
    
    pKeyParams->sessionEncKey = pEncKey;
    pKeyParams->sessionAuthKey = pMacKey;
    pKeyParams->sessionSalt = saltKey;
}

/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "KEYCFG" handler function for SRTP
 ******************************************************************************
 * DESCRIPTION: SRTP configuration handler for "KEYCFG"
 *
 *  void salldcfg_set_keycfg_srtp (
 *     salldSimCommConfig_t* pCommCfg,
 *     salldSimSrtpConfig_t* pSrtpCfg,  
 *     Sa_SrtpKeyParams_t* pKeyParams, 
 *     int16_t  chnum)
 *
 *****************************************************************************/
void salldcfg_set_keycfg_srtp (salldSimCommConfig_t* pCommCfg, 
                              salldSimSrtpConfig_t* pSrtpCfg, 
                              Sa_SrtpKeyParams_t* pKeyParams, 
                              int16_t chnum)
{
    uint8_t* pMasterKey = &salldsim_encKey[chnum - 1][0];

    /* Initialize all parameters to its default value */
    pKeyParams->ctrlBitfield = sa_SRTP_KEY_CTRL_MASTER_KEY          |
                               sa_SRTP_KEY_CTRL_MASTER_SALT         | 
                               sa_SRTP_KEY_CTRL_KEY_DERIVE_RATE     |
                               sa_SRTP_KEY_CTRL_KEY_LIFETIME        |
                               sa_SRTP_KEY_CTRL_ROC;
                               
    pKeyParams->masterKey = pMasterKey;
    pKeyParams->masterSalt = salldsim_salt1;   
    
    memcpy(pMasterKey, salldsim_key1, 16);
    /* Update key based on the channel number */
    pMasterKey[0] = chnum >> 8;
    pMasterKey[1] = chnum & 0xFF;
    
    pKeyParams->derivRate = pSrtpCfg->derivRate;
    pKeyParams->keyLifeTimeMsw = 0;
    pKeyParams->keyLifeTimeLsw = pSrtpCfg->keyLifeTime;
    pKeyParams->fromEsnMsw = pSrtpCfg->fromEsn >> 16;
    pKeyParams->fromEsnLsw = pSrtpCfg->fromEsn & 0xFFFF;
    pKeyParams->toEsnMsw   = pSrtpCfg->toEsn >> 16;
    pKeyParams->toEsnLsw   = pSrtpCfg->toEsn & 0xFFFF;
    pKeyParams->mkiSize    = pSrtpCfg->mkiSize;
    pKeyParams->mki        = 1;
    pKeyParams->roc        = pSrtpCfg->index;
    
    if(pSrtpCfg->fromTo)
        pKeyParams->ctrlBitfield |= sa_SRTP_KEY_CTRL_KEY_TYPE_FROM_TO;
        
    if(pSrtpCfg->mkiSize)
        pKeyParams->ctrlBitfield |= sa_SRTP_KEY_CTRL_MKI;        
 
} 

/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "KEYCFG" handler function for IPSEC ESP
 ******************************************************************************
 * DESCRIPTION: IPSEC ESP configuration handler for "KEYCFG"
 *
 *  void salldcfg_set_keycfg_esp (
 *     salldSimCommConfig_t* pCommCfg,
 *     salldSimIpsecConfig_t* pIpsecCfg,  
 *     Sa_IpsecKeyParams_t* pKeyParams, 
 *     int16_t  chnum)
 *
 *****************************************************************************/
void salldcfg_set_keycfg_esp (salldSimCommConfig_t* pCommCfg, 
                              salldSimIpsecConfig_t* pIpsecCfg, 
                              Sa_IpsecKeyParams_t* pKeyParams, 
                              int16_t chnum)
{
    salldcfg_set_keycfg_default(pKeyParams, chnum, salldsim_key1, salldsim_key2, salldsim_salt2);
 
    /* Reset the cipher mode specific parameters */
    switch (pCommCfg->cipherMode)
    {
        case sa_CipherMode_GCM:
        case sa_CipherMode_CCM:
        case sa_CipherMode_AES_CTR:
            pKeyParams->ctrlBitfield |= (sa_IPSEC_KEY_CTRL_ENC_KEY|sa_IPSEC_KEY_CTRL_SALT);
            break;
            
        case sa_CipherMode_AES_CBC:
        case sa_CipherMode_DES_CBC:
        case sa_CipherMode_3DES_CBC:
            pKeyParams->ctrlBitfield |= sa_IPSEC_KEY_CTRL_ENC_KEY;
            break; 
               
        case sa_CipherMode_NULL:    
        default:
            break;    
    }
    
    switch (pCommCfg->authMode)
    {
        case sa_AuthMode_GMAC:
            pKeyParams->ctrlBitfield |= (sa_IPSEC_KEY_CTRL_MAC_KEY|sa_IPSEC_KEY_CTRL_SALT);
            break;
            
        case sa_AuthMode_HMAC_MD5:
        case sa_AuthMode_CMAC:
        case sa_AuthMode_AES_XCBC:
        case sa_AuthMode_HMAC_SHA1:
        case sa_AuthMode_HMAC_SHA2_224:
        case sa_AuthMode_HMAC_SHA2_256:
        case sa_AuthMode_HMAC_SHA2_384:
        case sa_AuthMode_HMAC_SHA2_512:
            pKeyParams->ctrlBitfield |= sa_IPSEC_KEY_CTRL_MAC_KEY;
            break;
            
        case sa_AuthMode_NULL:    
        default:
            break;        
    }
} 

/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "KEYCFG" handler function for IPSEC AH
 ******************************************************************************
 * DESCRIPTION: IPSEC AH configuration handler for "KEYCFG"
 *
 *  void salldcfg_set_keycfg_ah (
 *     salldSimCommConfig_t* pCommCfg,
 *     salldSimIpsecConfig_t* pIpsecCfg,  
 *     Sa_IpsecKeyParams_t* pKeyParams, 
 *     int16_t  chnum)
 *
 *****************************************************************************/
void salldcfg_set_keycfg_ah (salldSimCommConfig_t* pCommCfg, 
                             salldSimIpsecConfig_t* pIpsecCfg, 
                             Sa_IpsecKeyParams_t* pKeyParams, 
                             int16_t chnum)
{
    salldcfg_set_keycfg_default(pKeyParams, chnum, salldsim_key1, salldsim_key2, salldsim_salt2);
 
    switch (pCommCfg->authMode)
    {
        case sa_AuthMode_GMAC:
            pKeyParams->ctrlBitfield |= (sa_IPSEC_KEY_CTRL_MAC_KEY|sa_IPSEC_KEY_CTRL_SALT);
            break;
            
        case sa_AuthMode_HMAC_MD5:
        case sa_AuthMode_CMAC:
        case sa_AuthMode_AES_XCBC:
        case sa_AuthMode_HMAC_SHA1:
        case sa_AuthMode_HMAC_SHA2_224:
        case sa_AuthMode_HMAC_SHA2_256:
        case sa_AuthMode_HMAC_SHA2_384:
        case sa_AuthMode_HMAC_SHA2_512:
            pKeyParams->ctrlBitfield |= sa_IPSEC_KEY_CTRL_MAC_KEY;
            break;
            
        case sa_AuthMode_NULL:    
        default:
            break;        
    }
} 

/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "KEYCFG" handler function for Air Ciphering
 ******************************************************************************
 * DESCRIPTION: Air Ciphering configuration handler for "KEYCFG"
 *
 *  void salldcfg_set_keycfg_ac (
 *     salldSimCommConfig_t* pCommCfg,
 *     salldSimAcConfig_t* pAcCfg,  
 *     Sa_AcKeyParams_t* pKeyParams, 
 *     int16_t  chnum)
 *
 *****************************************************************************/
void salldcfg_set_keycfg_ac  (salldSimCommConfig_t* pCommCfg, 
                              salldSimAcConfig_t* pAcCfg, 
                              Sa_AcKeyParams_t* pKeyParams, 
                              int16_t chnum)
{

    uint8_t* pEncKey = &salldsim_encKey[chnum - 1][0];
    uint8_t* pMacKey = &salldsim_macKey[chnum - 1][0];

    /* Initialize all parameters to its default value */
    pKeyParams->ctrlBitfield = 0;
    pKeyParams->sessionEncKey = pEncKey;
    pKeyParams->sessionAuthKey = pMacKey;
    
    memcpy(pEncKey, salldsim_key3, 16);
    memcpy(pMacKey, salldsim_key4, 16);
    
    /* Update key based on the channel number */
    pMacKey[0] = pEncKey[0] = chnum >> 8;
    pMacKey[1] = pEncKey[1] = chnum & 0xFF;
    
    pKeyParams->sessionEncKey = pEncKey;
    pKeyParams->sessionAuthKey = pMacKey;
    
    pKeyParams->ctrlBitfield |= ((pCommCfg->cipherMode == sa_CipherMode_NULL)?0:sa_AC_KEY_CTRL_ENC_KEY);
    pKeyParams->ctrlBitfield |= ((pCommCfg->authMode == sa_AuthMode_NULL)?0:sa_AC_KEY_CTRL_MAC_KEY);
} 

/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "KEYCFG" handler function for Data Mode
 ******************************************************************************
 * DESCRIPTION: Data Mode configuration handler for "KEYCFG"
 *
 *  void salldcfg_set_keycfg_data (
 *     salldSimCommConfig_t*     pCommCfg,
 *     salldSimDataModeConfig_t* pDataModeCfg,  
 *     Sa_DataModeKeyParams_t*   pKeyParams, 
 *     int16_t  chnum)
 *
 *****************************************************************************/
void salldcfg_set_keycfg_data (salldSimCommConfig_t* pCommCfg, 
                               salldSimDataModeConfig_t* pDataModeCfg, 
                               Sa_DataModeKeyParams_t* pKeyParams, 
                               int16_t chnum)
{

    uint8_t* pEncKey = &salldsim_encKey[chnum - 1][0];
    uint8_t* pMacKey = &salldsim_macKey[chnum - 1][0];

    /* Initialize all parameters to its default value */
    pKeyParams->ctrlBitfield = 0;
    pKeyParams->sessionEncKey = pEncKey;
    pKeyParams->sessionAuthKey = pMacKey;
    pKeyParams->sessionSalt = salldsim_salt2;   
    
    memcpy(pEncKey, salldsim_key3, 32);
    memcpy(pMacKey, salldsim_key4, 32);
    
    /* Update key based on the channel number */
    /*
     * A paor of channel should use the same keys
     */
    chnum -= 1; 
    chnum &= 0xfffe; 
    pMacKey[0] = pEncKey[0] = chnum >> 8;
    pMacKey[1] = pEncKey[1] = chnum & 0xFF;
    
    pKeyParams->ctrlBitfield |= (pDataModeCfg->encKeySize?sa_DATA_MODE_KEY_CTRL_ENC_KEY:0);
    pKeyParams->ctrlBitfield |= (pDataModeCfg->macKeySize?sa_DATA_MODE_KEY_CTRL_MAC_KEY:0);
    
    /* Reset the cipher mode specific parameters */
    switch (pCommCfg->cipherMode)
    {
        case sa_CipherMode_GCM:
        case sa_CipherMode_CCM:
            pKeyParams->ctrlBitfield |= (sa_DATA_MODE_KEY_CTRL_SALT);
            break;
            
        case sa_CipherMode_NULL:    
        default:
            break;    
    }
    
    switch (pCommCfg->authMode)
    {
        case sa_AuthMode_GMAC:
        case sa_AuthMode_GMAC_AH:
            pKeyParams->ctrlBitfield |= (sa_DATA_MODE_KEY_CTRL_SALT);
            break;
            
        case sa_AuthMode_NULL:    
        default:
            break;        
    }
} 


/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "KEYCFG" handler function.
 ******************************************************************************
 * DESCRIPTION: UT configuration handler for "KEYCFG"
 *
 *  void salldcfg_set_keycfg (
 *     salldSimCommConfig_t* pCommCfg,
 *     void* pProtoCfg,  
 *     Sa_ProtocolKeyParams_t *pKeyCfg, 
 *     int16_t  chnum,
 *     Bool tx)
 *
 *****************************************************************************/
void salldcfg_set_keycfg (salldSimCommConfig_t* pCommCfg, void* pProtoCfg, 
                          Sa_ProtocolKeyParams_t *pKeyCfg, 
                          int16_t  chnum, Bool tx)
{
    switch (pCommCfg->protocolType)
    {
        case sa_PT_NULL:
            salldcfg_set_keycfg_data(pCommCfg, (salldSimDataModeConfig_t*)pProtoCfg, 
                                     &pKeyCfg->data, chnum);
        break;
    
        case sa_PT_SRTP:
        case sa_PT_SRTCP:
            salldcfg_set_keycfg_srtp(pCommCfg, (salldSimSrtpConfig_t*)pProtoCfg, 
                                     &pKeyCfg->srtp, chnum);
        break;
        
        case sa_PT_IPSEC_AH:
            salldcfg_set_keycfg_ah(pCommCfg, (salldSimIpsecConfig_t*)pProtoCfg, 
                                   &pKeyCfg->ipsec, chnum);
        break;
        
        case sa_PT_IPSEC_ESP:
            salldcfg_set_keycfg_esp(pCommCfg, (salldSimIpsecConfig_t*)pProtoCfg, 
                                    &pKeyCfg->ipsec, chnum);
        break;
        
        case sa_PT_3GPP_AC:
            salldcfg_set_keycfg_ac(pCommCfg, (salldSimAcConfig_t*)pProtoCfg, 
                                   &pKeyCfg->ac, chnum);
        break;
        
        default:
        
        break;
    }  
}

/******************************************************************************
 * FUNCTION PURPOSE: SALLD Test configuration function.
 ******************************************************************************
 * DESCRIPTION: Prepare the SALLD channel and associated resources for the
 *  specified unit test configuration
 *
 *  salldSimChannel_t salldcfg_test_init (
 *    salldSimCommConfig_t* pCommCfg, - pointer to common channel configuration
 *    void* pProtoCfg)                - pointer to protocol-specific channel configuration
 *
 *****************************************************************************/
salldSimChannel_t* salldcfg_chan_init (salldSimCommConfig_t* pCommCfg, void* pProtoCfg)
{
#if defined(SAU_PRMOTE_DEMOTE_TEST)
    salldSimChannel_t *pSimChan = SALLD_ALLOC_CHAN(&tFrameworkSecure);
#else
    salldSimChannel_t *pSimChan = SALLD_ALLOC_CHAN(&tFramework);
#endif
    Sa_GenConfigParams_t *pTxGenCfg;
    Sa_GenConfigParams_t *pRxGenCfg;
    Sa_ProtocolKeyParams_t *pTxKeyCfg;
    Sa_ProtocolKeyParams_t *pRxKeyCfg;
    
    if (pSimChan == NULL)
    {
        salld_sim_print("salldcfg_chan_init: Salld Channel Handle is not available!\n");
        return (NULL);
    }   
    else
    {
        pTxGenCfg = &pSimChan->txInfo.genCfg;
        pRxGenCfg = &pSimChan->rxInfo.genCfg;
        pTxKeyCfg = &pSimChan->txInfo.keyCfg;
        pRxKeyCfg = &pSimChan->rxInfo.keyCfg;
    } 
    
    /* Initialize the channel */
    memset(pSimChan, 0, sizeof(salldSimChannel_t));
    pSimChan->ID = (salld_SIM_ID << 8) | (uint8_t)(numSalldSimChans);
    
    /* Non-protocol specific channel configuration */
    pSimChan->protocolType = pCommCfg->protocolType;
    pSimChan->relayWinSize = pCommCfg->replayWinSize;
    
    pTxGenCfg->destInfo = pCommCfg->destInfo[SALLD_UT_CFG_INFO_INDEX_TX];
    pTxGenCfg->cipherMode = pCommCfg->cipherMode;
    pTxGenCfg->authMode = pCommCfg->authMode;
    
    pRxGenCfg->destInfo = pCommCfg->destInfo[SALLD_UT_CFG_INFO_INDEX_RX];
    pRxGenCfg->cipherMode = pCommCfg->cipherMode;
    pRxGenCfg->authMode = pCommCfg->authMode;
    
    /* Protocol specific channel configuration */
    salldcfg_set_gencfg(pCommCfg, pProtoCfg, pTxGenCfg, TRUE);
    salldcfg_set_gencfg(pCommCfg, pProtoCfg, pRxGenCfg, FALSE);
    salldcfg_set_keycfg(pCommCfg, pProtoCfg, pTxKeyCfg, numSalldSimChans, TRUE);
    salldcfg_set_keycfg(pCommCfg, pProtoCfg, pRxKeyCfg, numSalldSimChans, FALSE);
    
    return (pSimChan);
}

/* nothing past this point */
