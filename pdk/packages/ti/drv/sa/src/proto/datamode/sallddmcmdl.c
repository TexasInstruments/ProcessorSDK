/******************************************************************************
 * FILE PURPOSE: Data Mode Command Label File
 ******************************************************************************
 * FILE NAME: sallddm.c
 *
 * DESCRIPTION: Provide functions to construct and update the command labels
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
#include "src/salldloc.h"
#include "src/salldport.h"
#include "src/salldctx.h"
#include "sallddm.h"
#include "sallddmloc.h"

#include "src/auth/salldcmac.h"
#include "src/auth/salldxcbc.h"


/*******************************************************************************
 * Local Prototypes
 ******************************************************************************/
typedef uint16_t (*dmFromCmdlFunc)(
   salldDataModeComInfo_t *pComInfo,
   tword *pCmdl);
   
typedef void (*dmUpdateCmdlFunc)(
   salldDataModeComInfo_t *pComInfo,
   Sa_PayloadInfo_t *pPayloadInfo,
   Sa_PktDesc_t* pPktDesc,
   tword *pCmdl);

/****************************************************************************
 * FUNCTION PURPOSE: Format General Command Label 
 ****************************************************************************
 * DESCRIPTION: Construct command labels in General mode
 *
 *  uint16_t salld_data_mode_format_cmdl_gen(
 *            salldDataModeComInfo_t *pComInfo  -> Pointer to Common Control Info
 *            tword                  *pCmdl )   -> Pointer to the command label buffer
 *                       
 * Return values: Commad label size
 * 
 ***************************************************************************/
static uint16_t salld_data_mode_format_cmdl_gen(salldDataModeComInfo_t *pComInfo, tword *pCmdl)
{
    Sa_DataModeConfigParams_t  *pConfig = &pComInfo->config;
    Sa_CmdLbUpdateInfo_t    *pCmdlUpdate = &pComInfo->cmdlUpdateInfo;
    
    int offset = 0;
    
    pCmdlUpdate->subMode = sa_DM_GEN;
    
    if (pConfig->enc1st)
    {
        
        if (pComInfo->encEngId != SALLD_CMDL_ENGINE_NONE)
        {
            pCmdlUpdate->validBitfield |= sa_CMDL_UPDATE_VALID_ENC;
            
            pCmdlUpdate->encSizeInfo.index = 0;
            pCmdlUpdate->encOffsetInfo.index = 1;
            
            /* Encryption first */
            if((pComInfo->encEngId == SALLD_CMDL_ENGINE_ID_ES1) && (pComInfo->authEngId == SALLD_CMDL_ENGINE_ID_ES1))
                pComInfo->authEngId = SALLD_CMDL_ENGINE_ID_ES2;
            
            if((pComInfo->encEngId == SALLD_CMDL_ENGINE_ID_ACS1) && (pComInfo->authEngId == SALLD_CMDL_ENGINE_ID_ACS1))
                pComInfo->authEngId = SALLD_CMDL_ENGINE_ID_ACS2;
        
            /* Construct the Encryption Command Label */
            if(pComInfo->authEngId != SALLD_CMDL_ENGINE_NONE)
                pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_NESC, pComInfo->authEngId);
            else    
                pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_NESC, SALLD_CMDL_FINAL_ENGINE_ID);
                
                
         
            if (pConfig->ivSize)  
            {
                pCmdlUpdate->validBitfield |= sa_CMDL_UPDATE_VALID_ENC_IV;
                pCmdlUpdate->encIvInfo.index = SALLD_CMDL_HEADER_SIZE_BYTES >> 2;
                pCmdlUpdate->encIvInfo.size  = pConfig->ivSize;
            
                pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, 
                                SALLD_CMDL_HEADER_SIZE_BYTES + pConfig->ivSize);  
                    
                pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL1,
                                SALLD_CMDL_MK_OPTION_CTRL(SA_ENC_AUX2_OFFSET, pConfig->ivSize)); 
        
                offset = SALLD_CMDL_HEADER_SIZE_BYTES + pConfig->ivSize;
            }
            else
            {
                pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, 
                                SALLD_CMDL_HEADER_SIZE_BYTES);  
        
                offset = SALLD_CMDL_HEADER_SIZE_BYTES;
            }
        
        }
        
        /* Construct the Authentication Command Label */
        if (pComInfo->authEngId != SALLD_CMDL_ENGINE_NONE)
        {
            pCmdlUpdate->validBitfield |= sa_CMDL_UPDATE_VALID_AUTH;
            pCmdlUpdate->authSizeInfo.index = offset >> 2;
            pCmdlUpdate->authOffsetInfo.index = pCmdlUpdate->authSizeInfo.index + 1;
            
            pktWrite8bits_m(pCmdl, offset + SALLD_CMDL_BYTE_OFFSET_NESC, 
                            SALLD_CMDL_FINAL_ENGINE_ID);
                        
            if (SALLD_DM_TEST_CMAC(pComInfo))
            {
                pCmdlUpdate->validBitfield |= sa_CMDL_UPDATE_VALID_AUX_KEY;
                pCmdlUpdate->auxKeyInfo.index = (offset + SALLD_CMDL_HEADER_SIZE_BYTES) >> 2;
                
                pktWrite8bits_m(pCmdl, offset + SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, 
                                SALLD_CMDL_HEADER_SIZE_BYTES + 16);
                pktWrite8bits_m(pCmdl, offset + SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL1, 
                                SALLD_CMDL_MK_OPTION_CTRL(SA_ENC_AUX1_OFFSET, 16));
            
                offset += (SALLD_CMDL_HEADER_SIZE_BYTES + 16);
            } 
            else if ((pComInfo->authEngId == SALLD_CMDL_ENGINE_ID_ACS1) || (pComInfo->authEngId == SALLD_CMDL_ENGINE_ID_ACS2)) 
            {
                /* Kasumi-F9 */
                pCmdlUpdate->validBitfield |= sa_CMDL_UPDATE_VALID_AUTH_IV;
                pCmdlUpdate->authIvInfo.index = (offset + SALLD_CMDL_HEADER_SIZE_BYTES) >> 2;
                pCmdlUpdate->authIvInfo.size  = pConfig->ivSize;
                
                pktWrite8bits_m(pCmdl, offset + SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, 
                                SALLD_CMDL_HEADER_SIZE_BYTES + pConfig->ivSize);  
                    
                pktWrite8bits_m(pCmdl, offset + SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL1,
                                SALLD_CMDL_MK_OPTION_CTRL(SA_ENC_AUX2_OFFSET, pConfig->ivSize)); 
                                
                offset += (SALLD_CMDL_HEADER_SIZE_BYTES + pConfig->ivSize);
            
            }
            else
            {
                pktWrite8bits_m(pCmdl, offset + SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, 
                                SALLD_CMDL_HEADER_SIZE_BYTES);
        
                offset += SALLD_CMDL_HEADER_SIZE_BYTES;
            }              
        }
    }
    else
    {
        /* Authentication first */
        
        if (pComInfo->authEngId != SALLD_CMDL_ENGINE_NONE)
        {
            pCmdlUpdate->validBitfield |= sa_CMDL_UPDATE_VALID_AUTH;
            pCmdlUpdate->authSizeInfo.index = 0;
            pCmdlUpdate->authOffsetInfo.index = 1;
        
            if((pComInfo->authEngId == SALLD_CMDL_ENGINE_ID_ES1) && (pComInfo->encEngId == SALLD_CMDL_ENGINE_ID_ES1))
                pComInfo->encEngId = SALLD_CMDL_ENGINE_ID_ES2;
            
            if((pComInfo->authEngId == SALLD_CMDL_ENGINE_ID_ACS1) && (pComInfo->encEngId == SALLD_CMDL_ENGINE_ID_ACS1))
                pComInfo->encEngId = SALLD_CMDL_ENGINE_ID_ACS2;
        
            /* Construct the Authentication Command Label */
            if(pComInfo->encEngId != SALLD_CMDL_ENGINE_NONE)
                pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_NESC, pComInfo->encEngId);
            else    
                pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_NESC, SALLD_CMDL_FINAL_ENGINE_ID);
    
            if (SALLD_DM_TEST_CMAC(pComInfo))
            {
                pCmdlUpdate->validBitfield |= sa_CMDL_UPDATE_VALID_AUX_KEY;
                pCmdlUpdate->auxKeyInfo.index = (SALLD_CMDL_HEADER_SIZE_BYTES) >> 2;
            
                pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, 
                                SALLD_CMDL_HEADER_SIZE_BYTES + 16);
                pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL1, 
                                SALLD_CMDL_MK_OPTION_CTRL(SA_ENC_AUX1_OFFSET, 16));
            
                offset = (SALLD_CMDL_HEADER_SIZE_BYTES + 16);
            }
            else if ((pComInfo->authEngId == SALLD_CMDL_ENGINE_ID_ACS1) || (pComInfo->authEngId == SALLD_CMDL_ENGINE_ID_ACS2)) 
            {
                /* Kasumi-F9 */
                pCmdlUpdate->validBitfield |= sa_CMDL_UPDATE_VALID_AUTH_IV;
                pCmdlUpdate->authIvInfo.index = (SALLD_CMDL_HEADER_SIZE_BYTES) >> 2;
                pCmdlUpdate->authIvInfo.size  = pConfig->ivSize;
                pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, 
                                SALLD_CMDL_HEADER_SIZE_BYTES + pConfig->ivSize);  
                    
                pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL1,
                                SALLD_CMDL_MK_OPTION_CTRL(SA_ENC_AUX2_OFFSET, pConfig->ivSize)); 
                                
                offset = SALLD_CMDL_HEADER_SIZE_BYTES + pConfig->ivSize;
            
            }
            else
            {
                pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, 
                                SALLD_CMDL_HEADER_SIZE_BYTES);
        
                offset = SALLD_CMDL_HEADER_SIZE_BYTES;
            } 
        }
        
        /* Construct the Encryption Command Label */
        if (pComInfo->encEngId != SALLD_CMDL_ENGINE_NONE)
        {
            pCmdlUpdate->validBitfield |= sa_CMDL_UPDATE_VALID_ENC;
            pCmdlUpdate->encSizeInfo.index = offset >> 2;
            pCmdlUpdate->encOffsetInfo.index = pCmdlUpdate->encSizeInfo.index + 1;
            
            
            pktWrite8bits_m(pCmdl, offset + SALLD_CMDL_BYTE_OFFSET_NESC, 
                            SALLD_CMDL_FINAL_ENGINE_ID);
    
            if (pConfig->ivSize)  
            {
                pCmdlUpdate->validBitfield |= sa_CMDL_UPDATE_VALID_ENC_IV;
                pCmdlUpdate->encIvInfo.index = (offset + SALLD_CMDL_HEADER_SIZE_BYTES) >> 2;
                pCmdlUpdate->encIvInfo.size  = pConfig->ivSize;
            
                pktWrite8bits_m(pCmdl, offset + SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, 
                                SALLD_CMDL_HEADER_SIZE_BYTES + pConfig->ivSize);  
                    
                pktWrite8bits_m(pCmdl, offset + SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL1,
                                SALLD_CMDL_MK_OPTION_CTRL(SA_ENC_AUX2_OFFSET, pConfig->ivSize)); 
        
                offset += (SALLD_CMDL_HEADER_SIZE_BYTES + pConfig->ivSize);
            }
            else
            {
                pktWrite8bits_m(pCmdl, offset + SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, 
                                SALLD_CMDL_HEADER_SIZE_BYTES);  
        
                offset += SALLD_CMDL_HEADER_SIZE_BYTES;
            }
        }
    }
    
    return ((uint16_t) offset);
} 

/****************************************************************************
 * FUNCTION PURPOSE: Format GCM Command Label 
 ****************************************************************************
 * DESCRIPTION: Construct command labels in GCM mode
 *
 *  uint16_t salld_data_mode_format_cmdl_gcm(
 *            salldDataModeComInfo_t *pComInfo  -> Pointer to Common Control Info
 *            tword                  *pCmdl )   -> Pointer to the command label buffer
 *                       
 * Return values: Commad label size
 * 
 ***************************************************************************/
static uint16_t salld_data_mode_format_cmdl_gcm(salldDataModeComInfo_t *pComInfo, tword *pCmdl)
{
    Sa_DataModeConfigParams_t  *pConfig = &pComInfo->config;
    Sa_CmdLbUpdateInfo_t    *pCmdlUpdate = &pComInfo->cmdlUpdateInfo;
    
    /* Look for ipsec kind of signature */
    if (((pConfig->aadSize == 8) || (pConfig->aadSize == 12) ) &&
         (pConfig->ivSize  == 8)                               &&
         (pConfig->sessionSaltSize == 4))          
    {
      pCmdlUpdate->subMode = sa_DM_GCM;
    }
    else
    {
      pCmdlUpdate->subMode = sa_DM_GCM_GEN;
    }

    pCmdlUpdate->validBitfield = sa_CMDL_UPDATE_VALID_ENC       |
                                 sa_CMDL_UPDATE_VALID_ENC_SIZE  |
                                 sa_CMDL_UPDATE_VALID_ENC_IV    |
                                 sa_CMDL_UPDATE_VALID_AAD;  
                                  
    pCmdlUpdate->encSizeInfo.index = 0;
    pCmdlUpdate->encOffsetInfo.index = 1;
    
    pCmdlUpdate->encSizeInfo2.index = (8 + 4) >> 2;
    pCmdlUpdate->aadInfo.index = (8 + 8) >> 2;
    pCmdlUpdate->aadInfo.size = pConfig->aadSize;
    
    pCmdlUpdate->encIvInfo.index = (32 + pConfig->sessionSaltSize) >> 2;
    pCmdlUpdate->encIvInfo.offset = pConfig->sessionSaltSize & 3;
    pCmdlUpdate->encIvInfo.size  = pConfig->ivSize;
    
    /* Construct the command label header (8 byte) */
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_NESC, SALLD_CMDL_FINAL_ENGINE_ID);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, SA_GCM_CMDL_SIZE);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL1, SA_GCM_CMDL_OPT1);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL2, SA_GCM_CMDL_OPT2);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL3, SA_GCM_CMDL_OPT3);
    
    /* Option 1: store the total encryption length (8 byte) */
    
    /* Option 2: store AAD with zero padding (16 byte) */
    
    /* Option 3: AES-CTR IV (salt| IV | 1) */
    misc_utlCopy(pComInfo->sessionSalt, 
                 (uint16_t *) (pCmdl + SALLD_BYTE_TO_WORD(32)),
                 SALLD_BYTE_TO_TUINT(pConfig->sessionSaltSize));
    pktWrite32bits_m(pCmdl, 32 + 12, 1);
    
    return(SA_GCM_CMDL_SIZE);
} 

/****************************************************************************
 * FUNCTION PURPOSE: Format CCM Command Label 
 ****************************************************************************
 * DESCRIPTION: Construct command labels in CCM mode
 *
 *  uint16_t salld_data_mode_format_cmdl_ccm(
 *            salldDataModeComInfo_t *pComInfo  -> Pointer to Common Control Info
 *            tword                  *pCmdl )   -> Pointer to the command label buffer
 *                       
 * Return values: Commad label size
 * 
 ***************************************************************************/
static uint16_t salld_data_mode_format_cmdl_ccm(salldDataModeComInfo_t *pComInfo, tword *pCmdl)
{
    Sa_DataModeConfigParams_t  *pConfig = &pComInfo->config;
    uint8_t                 opcode;
    Sa_CmdLbUpdateInfo_t    *pCmdlUpdate = &pComInfo->cmdlUpdateInfo;
    uint16_t                size;                      

    /* Look for ipsec kind of signature */
    if (((pConfig->aadSize == 8) || (pConfig->aadSize == 12) ) &&
         (pConfig->ivSize  == 8)                               &&
         (pConfig->sessionSaltSize == 3)) 
    {
      pCmdlUpdate->subMode = sa_DM_CCM;
    }
    else
    {
      pCmdlUpdate->subMode = sa_DM_CCM_GEN;
    }

    pCmdlUpdate->validBitfield = sa_CMDL_UPDATE_VALID_ENC       |
                                 sa_CMDL_UPDATE_VALID_ENC_SIZE  |
                                 sa_CMDL_UPDATE_VALID_ENC_IV    |
                                 sa_CMDL_UPDATE_VALID_ENC_IV2   |
                                 sa_CMDL_UPDATE_VALID_AAD;  
                                  
    pCmdlUpdate->encSizeInfo.index = 0;
    pCmdlUpdate->encOffsetInfo.index = 1;
    
    pCmdlUpdate->aadInfo.index = (8) >> 2;
    pCmdlUpdate->aadInfo.offset = 2;
    pCmdlUpdate->aadInfo.size = pConfig->aadSize;

    size = pConfig->sessionSaltSize + 1; /* Includes the flag size */

    pCmdlUpdate->encSizeInfo2.index = (24 + size + pConfig->ivSize) >> 2;    
    
    pCmdlUpdate->encIvInfo.index  = (24 + size) >> 2;
    pCmdlUpdate->encIvInfo.offset = size & 3;
    pCmdlUpdate->encIvInfo.size   = pConfig->ivSize;
    
    pCmdlUpdate->encIvInfo2.index  = (40 + size) >> 2;
    pCmdlUpdate->encIvInfo2.offset = size & 3;
    pCmdlUpdate->encIvInfo2.size   = pConfig->ivSize;
    
    /* Construct the command label header (8 byte) */
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_NESC, SALLD_CMDL_FINAL_ENGINE_ID);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, SA_CCM_CMDL_SIZE);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL1, SA_CCM_CMDL_OPT1);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL2, SA_CCM_CMDL_OPT2);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL3, SA_CCM_CMDL_OPT3);
    pCmdl += SALLD_BYTE_TO_WORD(SALLD_CMDL_HEADER_SIZE_BYTES);
    
    /* Option 1: B1 (aadlen | AAD ) (16 byte) */
    pktWrite16bits_m(pCmdl, 0, pConfig->aadSize);
    pCmdl += SALLD_BYTE_TO_WORD(16);
    
    /* Option 2: B0 (nonce | IV | encypted data len) */
    opcode = pConfig->aadSize?0x40:0x00;
    opcode |= (((pConfig->macSize -2) >> 1) << 3);
    opcode |= (14 - pConfig->sessionSaltSize - pConfig->ivSize);
    pktWrite8bits_m(pCmdl, 0, opcode);
    
    /* Should be replaced with WordsIntoWords for C55x/C54x */
    pktPackBytesIntoWords((tword *)pComInfo->sessionSalt, pCmdl, pConfig->sessionSaltSize, 1);    
    pCmdl += SALLD_BYTE_TO_WORD(16);
    
    /* Option 3: AES-CTR and Tag Authentication IV (salt| IV | 0 ) */
    opcode &= 0x07;
    pktWrite8bits_m(pCmdl, 0, opcode);
    
    /* Should be replaced with WordsIntoWords for C55x/C54x */
    pktPackBytesIntoWords((tword *)pComInfo->sessionSalt, pCmdl, pConfig->sessionSaltSize, 1); 
    
    return(SA_CCM_CMDL_SIZE);   
} 

/****************************************************************************
 * FUNCTION PURPOSE: Format GMAC Command Label 
 ****************************************************************************
 * DESCRIPTION: Construct command labels in GMAC mode
 *
 *  uint16_t salld_data_mode_format_cmdl_gmac(
 *            salldDataModeComInfo_t *pComInfo  -> Pointer to Common Control Info
 *            tword                  *pCmdl )   -> Pointer to the command label buffer
 *                       
 * Return values: Commad label size
 * 
 ***************************************************************************/
static uint16_t salld_data_mode_format_cmdl_gmac(salldDataModeComInfo_t *pComInfo, tword *pCmdl)
{
    Sa_DataModeConfigParams_t  *pConfig = &pComInfo->config;
    int offset;
    
    Sa_CmdLbUpdateInfo_t    *pCmdlUpdate = &pComInfo->cmdlUpdateInfo;
    
    /* Look for ipsec kind of signature */
    if (((pConfig->aadSize == 8) || (pConfig->aadSize == 12) ) &&
         (pConfig->ivSize  == 8)                               &&
         (pConfig->sessionSaltSize == 4)) 
    {
      pCmdlUpdate->subMode = sa_DM_GMAC;
    }
    else
    {
      pCmdlUpdate->subMode = sa_DM_GMAC_GEN;
    }
    pCmdlUpdate->validBitfield = sa_CMDL_UPDATE_VALID_AUTH      |
                                 sa_CMDL_UPDATE_VALID_AUTH_SIZE |
                                 sa_CMDL_UPDATE_VALID_AUTH_IV   |
                                 sa_CMDL_UPDATE_VALID_AAD       |
                                 sa_CMDL_UPDATE_VALID_PAYLOAD;  
                                  
    pCmdlUpdate->authSizeInfo.index = 0;
    pCmdlUpdate->authOffsetInfo.index = 1;
    
    pCmdlUpdate->authSizeInfo2.index = (8 + 4) >> 2;
    
    pCmdlUpdate->aadInfo.index = (16) >> 2;
    pCmdlUpdate->aadInfo.offset = 0;
    pCmdlUpdate->aadInfo.size = pConfig->aadSize;
    
    pCmdlUpdate->payloadInfo.index = (16 + pConfig->aadSize) >> 2;
    pCmdlUpdate->payloadInfo.offset = pConfig->aadSize % 4;
    pCmdlUpdate->payloadInfo.size = 16 - pConfig->aadSize;
    
    pCmdlUpdate->authIvInfo.index  = (32 +  pConfig->sessionSaltSize) >> 2;
    pCmdlUpdate->authIvInfo.offset = pConfig->sessionSaltSize & 3;
    pCmdlUpdate->authIvInfo.size   = pConfig->ivSize;
    
    /* Construct the command label header (8 byte) */
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_NESC, SALLD_CMDL_FINAL_ENGINE_ID);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, SA_GMAC_CMDL_SIZE);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL1, SA_GMAC_CMDL_OPT1);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL2, SA_GMAC_CMDL_OPT2);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL3, SA_GMAC_CMDL_OPT3);
    /* Option 1: store the total authentication length (AAD + payload) (8 byte) */
    
    /* Option 2: store the AAD plus payload to 16 bytes */
    
    /* Option 3: AES-CTR IV (salt| IV | 1) */
    offset = SALLD_CMDL_HEADER_SIZE_BYTES + 8 + 16;
    misc_utlCopy(pComInfo->sessionSalt, 
                 (uint16_t *) (pCmdl + SALLD_BYTE_TO_WORD(offset)),
                 SALLD_BYTE_TO_TUINT(pConfig->sessionSaltSize));
    pktWrite32bits_m(pCmdl, offset + 12, 1);
    
    return (SA_GMAC_CMDL_SIZE); 
} 

/****************************************************************************
 * FUNCTION PURPOSE: Format GMAC (IPSEC AH) Command Label 
 ****************************************************************************
 * DESCRIPTION: Construct command labels in GMAC (IPSEC AH) mode
 *
 *  uint16_t salld_data_mode_format_cmdl_gmac_ah(
 *            salldDataModeComInfo_t *pComInfo  -> Pointer to Common Control Info
 *            tword                  *pCmdl )   -> Pointer to the command label buffer
 *                       
 * Return values: Commad label size
 * 
 ***************************************************************************/
static uint16_t salld_data_mode_format_cmdl_gmac_ah(salldDataModeComInfo_t *pComInfo, tword *pCmdl)
{
    Sa_DataModeConfigParams_t  *pConfig = &pComInfo->config;
    int offset;
    
    Sa_CmdLbUpdateInfo_t    *pCmdlUpdate = &pComInfo->cmdlUpdateInfo;
    
    pCmdlUpdate->subMode = sa_DM_GMAC_AH;
    pCmdlUpdate->validBitfield = sa_CMDL_UPDATE_VALID_AUTH      |
                                 sa_CMDL_UPDATE_VALID_AUTH_SIZE |
                                 sa_CMDL_UPDATE_VALID_AUTH_IV;   
                                  
    pCmdlUpdate->authSizeInfo.index = 0;
    pCmdlUpdate->authOffsetInfo.index = 1;
    
    pCmdlUpdate->authSizeInfo2.index = (8 + 4) >> 2;
    
    pCmdlUpdate->authIvInfo.index = (16 + 4) >> 2;
    pCmdlUpdate->authIvInfo.size  = 8;
    
    /* Construct the command label header (8 byte) */
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_NESC, SALLD_CMDL_FINAL_ENGINE_ID);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_LABEL_LEN, SA_GMAC_CMDL_SIZE_NOAAD);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL1, SA_GMAC_CMDL_OPT1);
    pktWrite8bits_m(pCmdl, SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL2, SA_GMAC_CMDL_OPT3);
    /* Option 1: store the total authentication length (payload) (8 byte) */
    
    /* Option 2: AES-CTR IV (salt| IV | 1) */
    offset = SALLD_CMDL_HEADER_SIZE_BYTES + 8;
    misc_utlCopy(pComInfo->sessionSalt, 
                 (uint16_t *) (pCmdl + SALLD_BYTE_TO_WORD(offset)),
                 SALLD_BYTE_TO_TUINT(pConfig->sessionSaltSize));
    pktWrite32bits_m(pCmdl, offset + 12, 1);
    
    return (SA_GMAC_CMDL_SIZE); 
} 


/******************************************************************************
 * Command Label Processing function Table  
 *****************************************************************************/
static dmFromCmdlFunc salldDataModeFormatCmdlFunc[] = 
{
  salld_data_mode_format_cmdl_gen,
  salld_data_mode_format_cmdl_gcm,
  salld_data_mode_format_cmdl_gmac,
  salld_data_mode_format_cmdl_gmac_ah,
  salld_data_mode_format_cmdl_ccm
};

/****************************************************************************
 * FUNCTION PURPOSE: Derive the Data Mode Command Label Operation Mode
 ****************************************************************************
 * DESCRIPTION: Derive the Data Mode Command Label Operation Mode from the Cipher 
 *              mode and the authentication mode
 *
 *  uint8_t salldDataModeGetCmdlOpMode(
 *             int16_t cipherMode,      -> Cipher Mode
 *             int16_t authMode)        -> Authentication Mode
 *   
 * Return values:  
 *        Data Mode Command Label Operation Mode                  
 *
 ***************************************************************************/
static uint8_t salldDataModeGetCmdlOpMode(int16_t cipherMode, int16_t authMode) 
{
    if (cipherMode == sa_CipherMode_GCM)
    {
        return (SALLD_DATA_MODE_CMDL_MODE_GCM);
    }
    else if (cipherMode == sa_CipherMode_CCM)
    {
        return (SALLD_DATA_MODE_CMDL_MODE_CCM);
    }
    else if (authMode == sa_AuthMode_GMAC) 
    {
        return (SALLD_DATA_MODE_CMDL_MODE_GMAC);
    }
    else if (authMode == sa_AuthMode_GMAC_AH) 
    {
        return (SALLD_DATA_MODE_CMDL_MODE_GMAC_AH);
    }
    else
    {
        return (SALLD_DATA_MODE_CMDL_MODE_GEN);
    }
}

/****************************************************************************
 * FUNCTION PURPOSE: Construct Data Mode Command Labels
 ****************************************************************************
 * DESCRIPTION: Construct SA command labels for Data Mode (to-SA) operations
 *
 *  uint16_t salld_data_mode_set_cmdl(
 *            salldIpsecInst_t*  inst)   -> Point to Data Mode channel instance
 *                       
 * Return values: sa_ERR_XXX
 * 
 ***************************************************************************/
int16_t salld_data_mode_set_cmdl(salldDataModeInst_t *inst) 
{
  salldDataModeTxInst_t *txInst = &inst->txInst;
  salldDataModeComInfo_t *pComInfo = &txInst->comInfo;
  Sa_DataModeConfigParams_t  *pConfig = &pComInfo->config;
  Sa_CmdLbUpdateInfo_t    *pCmdlUpdate = &pComInfo->cmdlUpdateInfo;
  tword                   *cmdLblBuf;
  uint8_t                 maxCmdLSize, cmdlMode;
  int                     i, j;
  
  /* Derive the command label mode */
  cmdlMode = salldDataModeGetCmdlOpMode(txInst->cipherMode, txInst->authMode);
  SALLD_DM_SET_CMDL_MODE(pComInfo, cmdlMode);
  
  /* Derive and store K1/k2 if CMAC mode */
  if (txInst->authMode == sa_AuthMode_CMAC)
  {
    SALLD_DM_SET_CMAC(pComInfo, 1);
      
    salld_aes_cmac_get_keys((tword *)pComInfo->sessionMacKey, pConfig->sessionMacKeySize << 3,
                         (tword *)pComInfo->aux, 
                         (tword *)&pComInfo->aux[SALLD_AES_BLOCK_SIZE_IN_TUINT]);
  }
  if (txInst->authMode == sa_AuthMode_AES_XCBC)
  {
    SALLD_DM_SET_CMAC(pComInfo, 1);
    salld_aes_xcbc_get_subkey((tword *)pComInfo->sessionMacKey, pConfig->sessionMacKeySize << 3,
                               0x2, 
                              (tword *)pComInfo->aux);
    salld_aes_xcbc_get_subkey((tword *)pComInfo->sessionMacKey, pConfig->sessionMacKeySize << 3,
                               0x3, 
                              (tword *)&pComInfo->aux[SALLD_AES_BLOCK_SIZE_IN_TUINT]);
      
  }
  
  
  /* Clear and control command labels */
  memset(txInst->cmdl.buf, 0, SALLD_BYTE_TO_WORD(sa_MAX_CMDLB_SIZE));
  
  /* Initialize the command update structure */
  memset(pCmdlUpdate, 0, sizeof(Sa_CmdLbUpdateInfo_t));
  pCmdlUpdate->encSizeInfo.offset   = 2;
  pCmdlUpdate->encSizeInfo.size     = 2;
  //pCmdlUpdate->encOffsetInfo.offset = 0;
  pCmdlUpdate->encOffsetInfo.size   = 1;
  //pCmdlUpdate->encSizeInfo2.offset = 0;
  pCmdlUpdate->encSizeInfo2.size    = 4;
  pCmdlUpdate->authSizeInfo.offset  = 2;
  pCmdlUpdate->authSizeInfo.size    = 2;
  //pCmdlUpdate->authOffsetInfo.offset= 0;
  pCmdlUpdate->authOffsetInfo.size  = 1;
  
  for (i = 0, j = 0; i < 32; i+= 4, j++)
  {
    uint8_t *pData = (uint8_t *)&pComInfo->aux[0];
    pCmdlUpdate->auxKey[j] = SALLD_MK_UINT32_FROM8S(pData[i], pData[i+1], pData[i+2], pData[i+3]);
  }
  
  /* Format the command label */
  #ifdef NSS_LITE2
    /* Write the first PS word as part of command label */
    SA2_UL_SW_SET_EGRESS_DEST_QUEUE(txInst->cmdl.buf[0], txInst->destInfo.queueID);

    /* Update promote request if present */
    if (pConfig->ctrlBitMap & sa_DM_CONFIG_PROMOTE_CHANNEL)
    {
        SA2_UL_SW_SET_PROMOTE(txInst->cmdl.buf[0],1);
    }

    /* Update demote request if present */
    if (pConfig->ctrlBitMap & sa_DM_CONFIG_DEMOTE_CHANNEL)
    {
        SA2_UL_SW_SET_DEMOTE(txInst->cmdl.buf[0],1);
    }

    /* Update nonSecure crypto request if present */
    if (pConfig->ctrlBitMap & sa_DM_CONFIG_USE_SECURE_CTX_FOR_NON_SECURE_CHANNEL)
    {
        SA2_UL_SW_SET_NONSEC_CRYPO(txInst->cmdl.buf[0],1);
    }

    cmdLblBuf = (tword *)&txInst->cmdl.buf[1];
    /* Note for NSS_LITE2 the first cmd lable word is used for PS info and hence
       it is effectively reduced by 4 bytes
     */
    maxCmdLSize = (sa_MAX_CMDLB_SIZE-4u);
  #else
    cmdLblBuf = (tword *)txInst->cmdl.buf;
    maxCmdLSize = sa_MAX_CMDLB_SIZE;
  #endif
  txInst->cmdl.size = (*salldDataModeFormatCmdlFunc[cmdlMode])(pComInfo, cmdLblBuf);

  /* Copy command label buffer as UINT32 words from the uint8_t big-endian array */
  if (txInst->cmdl.size <= maxCmdLSize)
  {
    int i;
    uint8_t   *pData = (uint8_t *)cmdLblBuf;
    uint32_t  *pCmdLb = (uint32_t *)cmdLblBuf;
    
    for ( i = 0; i < txInst->cmdl.size; i+=4, pCmdLb++)
    {
        *pCmdLb = SALLD_MK_UINT32_FROM8S(pData[i], pData[i+1], pData[i+2], pData[i+3]);
    }
  }
  

#ifdef NSS_LITE2
  /* 4 Bytes of additional informatoin in PS info */
  txInst->cmdl.size += 4;
#endif

  return (sa_ERR_OK);
} 

/****************************************************************************
 * FUNCTION PURPOSE: utility function for updating the command label
 ****************************************************************************/
static 
void salld_cmdLbl_util(uint32_t* cmdLb, uint8_t index, uint8_t offset, uint8_t* bPtr, uint8_t bSize)
{
  uint32_t temp   = 0;
  int      bIndex = 0;
  int8_t   iShift = 8 *( 3 - offset);
  int      w32Size, bytesInlast32bWord;

  /* number of bytes to be formed in last 32-bit word */
  bytesInlast32bWord = (bSize + offset ) & 3;

  /* number of bytes to be formed into full 32-bit words */
  w32Size = bSize - offset - bytesInlast32bWord;
  
  /* Handle the un-aligned first 32 word that needs update specially */
  if (offset != 0)
  {    
    while (iShift >= 0) {
      temp |= bPtr[bIndex ++] << iShift;
      iShift -= 8;
    }
    cmdLb[index ++] |= temp;
  }

  /* the loop runs for all full 32-bit words */
  while (bIndex < w32Size)
  {
    cmdLb[index ++] = bPtr[bIndex] << 24 | bPtr[bIndex+1] << 16 | \
                      bPtr[bIndex + 2] <<  8 | bPtr[bIndex + 3];
    bIndex += 4;
  }

  /* Handle any un-handled bytes towards the end */
  if (bytesInlast32bWord)
  {
    iShift = 24;
    temp = 0;
    while (bytesInlast32bWord) {
      temp |= bPtr[bIndex ++] << iShift;
      iShift -= 8;
      bytesInlast32bWord --;
    }
    cmdLb[index] = temp;
  }
}

/****************************************************************************
 * FUNCTION PURPOSE: Update General Command Label 
 ****************************************************************************
 * DESCRIPTION: Update command labels in General mode
 *
 *  void  salld_data_mode_update_cmdl_gen(
 *            salldDataModeComInfo_t *pComInfo      -> Pointer to Common Control Info
 *            Sa_PayloadInfo_t       *pPayloadInfo  -> Pointer to the paylaod Information
 *            tword                  *pCmdl )       -> Pointer to the command label buffer
 *                       
 * Return values: None
 * 
 ***************************************************************************/
static void salld_data_mode_update_cmdl_gen(salldDataModeComInfo_t *pComInfo,
                                            Sa_PayloadInfo_t *pPayloadInfo,
                                            Sa_PktDesc_t* pPktDesc,
                                            tword *pCmdl)
{

    Sa_CmdLbUpdateInfo_t* updateInfo = &pComInfo->cmdlUpdateInfo;
    uint32_t *cmdLb = (uint32_t *)pCmdl;

    if (updateInfo->validBitfield & sa_CMDL_UPDATE_VALID_ENC)
    {
        cmdLb[updateInfo->encSizeInfo.index] |= pPayloadInfo->encSize;
        cmdLb[updateInfo->encOffsetInfo.index] |= ((uint32_t)pPayloadInfo->encOffset << 24);

        if (updateInfo->validBitfield & sa_CMDL_UPDATE_VALID_ENC_IV)
        {
            uint32_t *data = &cmdLb[updateInfo->encIvInfo.index];
            uint8_t *iv = pPayloadInfo->encIV;

            data[0] = SALLD_MK_UINT32_FROM8S(iv[0], iv[1], iv[2], iv[3]);
            data[1] = SALLD_MK_UINT32_FROM8S(iv[4], iv[5], iv[6], iv[7]);

            if (updateInfo->encIvInfo.size > 8)
            {
                data[2] = SALLD_MK_UINT32_FROM8S(iv[8], iv[9], iv[10], iv[11]);
                data[3] = SALLD_MK_UINT32_FROM8S(iv[12], iv[13], iv[14], iv[15]);
            }
        }
    }

    if (updateInfo->validBitfield & sa_CMDL_UPDATE_VALID_AUTH)
    {
        cmdLb[updateInfo->authSizeInfo.index] |= pPayloadInfo->authSize;
        cmdLb[updateInfo->authOffsetInfo.index] |= ((uint32_t)pPayloadInfo->authOffset << 24);

        if (updateInfo->validBitfield & sa_CMDL_UPDATE_VALID_AUTH_IV)
        {
            uint32_t *data = &cmdLb[updateInfo->authIvInfo.index];
            uint8_t *iv = pPayloadInfo->authIV;

            data[0] = SALLD_MK_UINT32_FROM8S(iv[0], iv[1], iv[2], iv[3]);
            data[1] = SALLD_MK_UINT32_FROM8S(iv[4], iv[5], iv[6], iv[7]);

            if (updateInfo->authIvInfo.size > 8)
            {
                data[2] = SALLD_MK_UINT32_FROM8S(iv[8], iv[9], iv[10], iv[11]);
                data[3] = SALLD_MK_UINT32_FROM8S(iv[12], iv[13], iv[14], iv[15]);
            }
        }

        if (updateInfo->validBitfield & sa_CMDL_UPDATE_VALID_AUX_KEY)
        {

            int offset = (pPayloadInfo->authSize & 0xF)?4:0;
            memcpy(&cmdLb[updateInfo->auxKeyInfo.index],  &updateInfo->auxKey[offset], 16);
        }
    }
            
} 

/****************************************************************************
 * FUNCTION PURPOSE: Update GCM Command Label 
 ****************************************************************************
 * DESCRIPTION: Update command labels in GCM mode
 *
 *  void  salld_data_mode_update_cmdl_gcm(
 *            salldDataModeComInfo_t *pComInfo      -> Pointer to Common Control Info
 *            Sa_PayloadInfo_t     *pPayloadInfo  -> Pointer to the paylaod Information
 *            tword                  *pCmdl )       -> Pointer to the command label buffer
 *                       
 * Return values: None
 * 
 ***************************************************************************/
static void salld_data_mode_update_cmdl_gcm(salldDataModeComInfo_t *pComInfo,
                                            Sa_PayloadInfo_t *pPayloadInfo,
                                             Sa_PktDesc_t* pPktDesc,
                                            tword *pCmdl)
{

    Sa_DataModeConfigParams_t  *pConfig = &pComInfo->config;
    uint32_t *cmdLb = (uint32_t *)pCmdl;
    uint8_t *iv  = pPayloadInfo->encIV;
    uint8_t *aad = pPayloadInfo->aad;
    Sa_CmdLbUpdateInfo_t  *cmdlUpdateInfo = &pComInfo->cmdlUpdateInfo;

    /* Update the command label header (8 byte) */
    cmdLb[0] |= pPayloadInfo->encSize;
    cmdLb[1] |= ((uint32_t)pPayloadInfo->encOffset << 24);

    /* Option 1: store the total encryption length (8 byte) */
    cmdLb[3] = pPayloadInfo->encSize << 3;    

    if (cmdlUpdateInfo->subMode == sa_DM_GCM)
    {    
      /* Option 2: store AAD with zero padding (16 byte) */
      cmdLb[4] = SALLD_MK_UINT32_FROM8S(aad[0], aad[1], aad[2], aad[3]);
      cmdLb[5] = SALLD_MK_UINT32_FROM8S(aad[4], aad[5], aad[6], aad[7]);
      if (pConfig->aadSize == 12)
      {
        cmdLb[6] = SALLD_MK_UINT32_FROM8S(aad[8], aad[9], aad[10], aad[11]);
      }
      
      /* Option 3: AES-CTR IV (salt| IV | 1) */
      /* Insert the 8-byte IV */
      cmdLb[9]  = SALLD_MK_UINT32_FROM8S(iv[0], iv[1], iv[2], iv[3]);
      cmdLb[10] = SALLD_MK_UINT32_FROM8S(iv[4], iv[5], iv[6], iv[7]);       
    }
    else /* Generic GCM case */
    {
       /* Option 1 is already stored */

       /* Option 2: store AAD with zero padding (16 byte) */
       if (pConfig->aadSize > 0 )
       {
          /* Insert AAD */
          salld_cmdLbl_util(cmdLb, cmdlUpdateInfo->aadInfo.index, \
                            cmdlUpdateInfo->aadInfo.offset, aad, cmdlUpdateInfo->aadInfo.size);
       } 

       /* Option 3: AES-CTR IV (salt| IV | 1) */
       /* Insert the IV */
       salld_cmdLbl_util(cmdLb, cmdlUpdateInfo->encIvInfo.index,  \
                         cmdlUpdateInfo->encIvInfo.offset, iv, cmdlUpdateInfo->encIvInfo.size);      
    }    
} 

/****************************************************************************
 * FUNCTION PURPOSE: Update CCM Command Label 
 ****************************************************************************
 * DESCRIPTION: Update command labels in CCM mode
 *
 *  void  salld_data_mode_update_cmdl_ccm(
 *            salldDataModeComInfo_t *pComInfo      -> Pointer to Common Control Info
 *            Sa_PayloadInfo_t     *pPayloadInfo  -> Pointer to the paylaod Information
 *            tword                  *pCmdl )       -> Pointer to the command label buffer
 *                       
 * Return values: None
 * 
 ***************************************************************************/
static void salld_data_mode_update_cmdl_ccm(salldDataModeComInfo_t *pComInfo,
                                            Sa_PayloadInfo_t *pPayloadInfo,
                                             Sa_PktDesc_t* pPktDesc,
                                            tword *pCmdl)
{

    Sa_DataModeConfigParams_t  *pConfig = &pComInfo->config;
    uint32_t *cmdLb = (uint32_t *)pCmdl;
    uint8_t *iv  = pPayloadInfo->encIV;
    uint8_t *aad = pPayloadInfo->aad;
    Sa_CmdLbUpdateInfo_t  *pCmdlUpdate = &pComInfo->cmdlUpdateInfo;
    
    /* Update the command label header (8 byte) */
    cmdLb[0] |= pPayloadInfo->encSize;
    cmdLb[1] |= ((uint32_t)pPayloadInfo->encOffset << 24);
    if (pCmdlUpdate->subMode == sa_DM_CCM)
    {
      /* Option 1: B1 (aadlen | AAD ) (16 byte) */
      /* Insert AAD */
      cmdLb[2] |= ((aad[0] << 8) + aad[1]);
      cmdLb[3] = SALLD_MK_UINT32_FROM8S(aad[2], aad[3], aad[4], aad[5]);
      if (pConfig->aadSize == 8)
      {
          cmdLb[4] = (aad[6] << 24) + (aad[7] << 16);
      }
      else
      {
          cmdLb[4] = SALLD_MK_UINT32_FROM8S(aad[6], aad[7], aad[8], aad[9]);
          cmdLb[5] = (aad[10] << 24) + (aad[11] << 16);
      }
      
      /* Option 2: B0 (flag | salt | IV | encypted data len) (16 byte) */
      /* Option 3: AES-CTR IV (salt| IV | 1)  (16 byte) */
      /* Insert IV and encrypted data len */
      /* IV1 & 2*/
      cmdLb[9] = pPayloadInfo->encSize;
      cmdLb[11] = cmdLb[7] = SALLD_MK_UINT32_FROM8S(iv[0], iv[1], iv[2], iv[3]);
      cmdLb[12] = cmdLb[8] = SALLD_MK_UINT32_FROM8S(iv[4], iv[5], iv[6], iv[7]);
    }
    else /* Generic CCM case to be handled */
    {
       /* Handle Option 1, (Option 2 & Option 3) in two different groups */
       /* Option 1 Update: if AAD size is 0, no updates to Option 1 */
       if (pConfig->aadSize > 0)            
       {
          /* Option 1: B1 (aadlen | AAD ) (16 byte) */
          /* update AAD */
          salld_cmdLbl_util(&cmdLb[0], pCmdlUpdate->aadInfo.index, \
                             pCmdlUpdate->aadInfo.offset, &aad[0], pConfig->aadSize);        
       }

       /* Option 2 update IV: B0 (flag | salt | IV | encypted data len) (16 byte) */
       salld_cmdLbl_util(&cmdLb[0], pCmdlUpdate->encIvInfo.index, \
                           pCmdlUpdate->encIvInfo.offset, iv, pCmdlUpdate->encIvInfo.size);
       cmdLb[9] |= pPayloadInfo->encSize;

       /* Option 3: AES-CTR IV (salt| IV | 0)  (16 byte) */
       /* Insert IV and encrypted data len */
       /* IV1 & 2*/
       cmdLb[10] = cmdLb[6] & 0x07FFFFFFUL;
       cmdLb[11] = cmdLb[7];    
       cmdLb[12] = cmdLb[8];
       cmdLb[13] = cmdLb[9] & 0xFFFF0000UL;
    }   
} 

/****************************************************************************
 * FUNCTION PURPOSE: Update GMAC Command Label 
 ****************************************************************************
 * DESCRIPTION: Update command labels in GMAC mode
 *
 *  void  salld_data_mode_update_cmdl_gmac(
 *            salldDataModeComInfo_t *pComInfo      -> Pointer to Common Control Info
 *            Sa_PayloadInfo_t     *pPayloadInfo  -> Pointer to the paylaod Information
 *            tword                  *pCmdl )       -> Pointer to the command label buffer
 *                       
 * Return values: None
 * 
 ***************************************************************************/
static void salld_data_mode_update_cmdl_gmac(salldDataModeComInfo_t *pComInfo,
                                             Sa_PayloadInfo_t *pPayloadInfo,
                                             Sa_PktDesc_t* pPktDesc,
                                             tword *pCmdl)
{

    Sa_DataModeConfigParams_t  *pConfig = &pComInfo->config;
    uint32_t *cmdLb = (uint32_t *)pCmdl;
    uint8_t aadSize = pConfig->aadSize;
    uint8_t *iv  = pPayloadInfo->authIV;
    uint8_t *aad = pPayloadInfo->aad;
    uint8_t *payload = (uint8_t *) pPktDesc->segments[0] + pPayloadInfo->authOffset;
    uint8_t dataAppended = 16 - aadSize; /* payload appended to AAD in bytes */
    Sa_CmdLbUpdateInfo_t  *cmdlUpdateInfo = &pComInfo->cmdlUpdateInfo;
   
    /* Update the command label header (8 byte) */
    cmdLb[0] |=  pPayloadInfo->authSize  - dataAppended;
    cmdLb[1] |=  ((pPayloadInfo->authOffset + dataAppended) << 24);
    
    /* Option 1: store the total authentication length (8 byte) */
    cmdLb[3] = (pPayloadInfo->authSize + aadSize) << 3;

    if (cmdlUpdateInfo->subMode == sa_DM_GMAC)
    {
  		/* Option 2: store the AAD plus payload to 16 bytes */
  		/* Assume all the AAD pending bytes (up to 16 bytes) is within the first data segment */
  		cmdLb[4] = SALLD_MK_UINT32_FROM8S(aad[0], aad[1], aad[2], aad[3]);
  		cmdLb[5] = SALLD_MK_UINT32_FROM8S(aad[4], aad[5], aad[6], aad[7]);
  		if (aadSize == 8)
  		{
  			cmdLb[6] = SALLD_MK_UINT32_FROM8S(payload[0], payload[1], payload[2], payload[3]);
  			cmdLb[7] = SALLD_MK_UINT32_FROM8S(payload[4], payload[5], payload[6], payload[7]);
  		}
  		else
  		{
  			cmdLb[6] = SALLD_MK_UINT32_FROM8S(aad[8], aad[9], aad[10], aad[11]);
  			cmdLb[7] = SALLD_MK_UINT32_FROM8S(payload[0], payload[1], payload[2], payload[3]);
  		}

  		/* Option 3: AES-CTR IV (salt| IV | 1) */
  		cmdLb[9]  = SALLD_MK_UINT32_FROM8S(iv[0], iv[1], iv[2], iv[3]);
  		cmdLb[10] = SALLD_MK_UINT32_FROM8S(iv[4], iv[5], iv[6], iv[7]);
    }
    else  /* GMAC generic case */     
    {
       /* Option 2: store the AAD plus payload to 16 bytes 
        * Assumption here is payload is at least 16 bytes 
        * as we are not checking payload size
        */
       if (aadSize) 
       {
         /* Insert AAD */
         salld_cmdLbl_util(cmdLb, cmdlUpdateInfo->aadInfo.index, \
                           cmdlUpdateInfo->aadInfo.offset, aad, cmdlUpdateInfo->aadInfo.size);
      
       }

       /* append payload if needed */
       salld_cmdLbl_util(cmdLb, cmdlUpdateInfo->payloadInfo.index, \
                         cmdlUpdateInfo->payloadInfo.offset, payload, dataAppended);

       /* Option 3: AES-CTR IV (salt| IV | 1) */
       /* Insert the IV */
       salld_cmdLbl_util(cmdLb, cmdlUpdateInfo->authIvInfo.index,  \
                      cmdlUpdateInfo->authIvInfo.offset, iv, cmdlUpdateInfo->authIvInfo.size);             
    }
} 

/****************************************************************************
 * FUNCTION PURPOSE: Update GMAC Command Label 
 ****************************************************************************
 * DESCRIPTION: Update command labels in GMAC mode
 *
 *  void  salld_data_mode_update_cmdl_gmac(
 *            salldDataModeComInfo_t *pComInfo    -> Pointer to Common Control Info
 *            Sa_PayloadInfo_t     *pPayloadInfo  -> Pointer to the paylaod Information
 *            tword                  *pCmdl )     -> Pointer to the command label buffer
 *                       
 * Return values: None
 * 
 ***************************************************************************/
static void salld_data_mode_update_cmdl_gmac_ah(salldDataModeComInfo_t *pComInfo,
                                                Sa_PayloadInfo_t *pPayloadInfo,
                                                Sa_PktDesc_t* pPktDesc,
                                                tword *pCmdl)
{

    uint32_t *cmdLb = (uint32_t *)pCmdl;
    uint8_t *iv  = pPayloadInfo->authIV;
   
    /* Update the command label header (8 byte) */
    cmdLb[0] |=  pPayloadInfo->authSize;
    cmdLb[1] |=  (pPayloadInfo->authOffset << 24);
    
    /* Option 1: store the total authentication length (8 byte) */
    cmdLb[3] = (pPayloadInfo->authSize) << 3;
   
    /* Option 2: AES-CTR IV (salt| IV | 1) */
    cmdLb[5]  = SALLD_MK_UINT32_FROM8S(iv[0], iv[1], iv[2], iv[3]);
    cmdLb[6] = SALLD_MK_UINT32_FROM8S(iv[4], iv[5], iv[6], iv[7]);
    
} 


/******************************************************************************
 * Command Label Processing function Table  
 *****************************************************************************/
static dmUpdateCmdlFunc salldDataModeUpdateCmdlFunc[] = 
{
  salld_data_mode_update_cmdl_gen,
  salld_data_mode_update_cmdl_gcm,
  salld_data_mode_update_cmdl_gmac,
  salld_data_mode_update_cmdl_gmac_ah,
  salld_data_mode_update_cmdl_ccm
};
           

/****************************************************************************
 * FUNCTION PURPOSE: Update Data Mode Command Labels
 ****************************************************************************
 * DESCRIPTION: Update SA command labels for Data Mode (to-SA) operations
 *
 *  uint16_t salld_data_mode_update_cmdl(
 *            salldIpsecInst_t*  inst,   -> Pointer to Data Mode channel instance
 *            Sa_PktInfo_t*    pktInfo)-> Pointer to the packet Info            
 * Return values: sa_ERR_XXX
 * 
 ***************************************************************************/
int16_t salld_data_mode_update_cmdl(salldDataModeInst_t *inst, Sa_PktInfo_t* pktInfo) 
{
  salldDataModeTxInst_t *txInst = &inst->txInst;
  salldDataModeComInfo_t *pComInfo = &txInst->comInfo;
  tword                  *cmdLblBuf;
  
  uint8_t cmdlMode;                
  
  if(!(pktInfo->validBitMap & sa_PKT_INFO_VALID_PAYLOAD_INFO))
    return (sa_ERR_PARAMS);
    
  if((!(pktInfo->validBitMap & sa_PKT_INFO_VALID_CMDLB_INFO)) ||
     (pktInfo->cmdlb.cmdLbBuf == NULL))
    return (sa_ERR_PARAMS);
  
  /* Derive the command label mode */
  cmdlMode = SALLD_DM_GET_CMDL_MODE(pComInfo);
  
  /* Copy and update command labels */
  memcpy(pktInfo->cmdlb.cmdLbBuf, txInst->cmdl.buf, txInst->cmdl.size);
  pktInfo->cmdlb.cmdLbSize = txInst->cmdl.size;
  
  /* Update the command label */
  #ifdef NSS_LITE2
  cmdLblBuf = (tword *)(pktInfo->cmdlb.cmdLbBuf + 4);
  #else
  cmdLblBuf = (tword *)pktInfo->cmdlb.cmdLbBuf;
  #endif
  (*salldDataModeUpdateCmdlFunc[cmdlMode])(pComInfo, &pktInfo->payloadInfo, &pktInfo->pktDesc, cmdLblBuf);
  
  if (pktInfo->cmdlb.cmdLbUpdateInfo)
  {
    memcpy(pktInfo->cmdlb.cmdLbUpdateInfo, &pComInfo->cmdlUpdateInfo, sizeof(Sa_CmdLbUpdateInfo_t));
  }
  
  return (sa_ERR_OK);
}            

   
/* Nothing past this point */

