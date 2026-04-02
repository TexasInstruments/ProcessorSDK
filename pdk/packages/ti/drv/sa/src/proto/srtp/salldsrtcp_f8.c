/******************************************************************************
 * FILE PURPOSE: Secure RTCP Form IV algorithm for AES_f8 mode
 ******************************************************************************
 * FILE NAME: salldsrtcp_f8.c
 *
 * DESCRIPTION: The main module for Secure RTCP Code in AES_f8 mode
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
/* SALLD header files */
#include "src/salldloc.h"
#include "src/salldport.h"
#include "src/cipher/salldaes.h"
#include "salldsrtcploc.h"

/******************************************************************************
 * FUNCTION PURPOSE: SRTCP IV Formating at AES F8 mode
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_srtcp_form_f8_iv (
 *    void*  keyInfo,    - A pointer to Key Info
 *    tword  *pkt,       - packet pointer
 *    tword  *iv,        - iv out pointer
 *    uint32_t index)      - SRTCP Sequence Number
 *
 *    IV' = 0x00000000 || ROC || V || P || RC || PT || length || SSRC
 *    IV  = AES_CTR(encryption_key XOR m, IV')
 *
 * Note: 4-byte alignment is required for IV
 *****************************************************************************/
int16_t salld_srtcp_form_f8_iv(void* keyInfo, tword *pkt, tword *iv, uint32_t index)
{
  int16_t   i;
  salldSrtcpKeyInfo_t* pKeyInfo = (salldSrtcpKeyInfo_t*) keyInfo;
  uint16_t  *pSalt = pKeyInfo->sessionSalt;
  uint16_t  maskedKey[SALLD_SRTP_CIPHER_KEY_SIZE_IN_TUINT];
  tulong  roundkey[SALLD_MAX_ROUND_KEY_SIZE_IN_TULONG];
  uint16_t  *pIV = (uint16_t *)iv;
  uint16_t  *pKey = pKeyInfo->sessionEncKey;
  uint16_t  nr;

  pktWrite32bits_m((tword *)pIV, 0, 0); /* 0x00..00 32 bit zeroes */

  /* put 32 bit E||SRTCP Index */
  pktWrite32bits_m((tword *)pIV, 4, (index | 0x80000000)); 

  /* copy next 8 bytes directly from pkt into IV */
  /* V || P || RC || PT || length || SSRC */
  misc_utlCopy((uint16_t*)pkt, &pIV[4], 4);
  
  /* get masked key */
  for(i = 0; i < SALLD_SRTP_SESSION_SALT_SIZE_IN_TUINT; i++)
  {
    maskedKey[i] = pSalt[i] ^ pKey[i];
  }
  
  for(i = SALLD_SRTP_SESSION_SALT_SIZE_IN_TUINT; 
            i < SALLD_SRTP_CIPHER_KEY_SIZE_IN_TUINT; i++)
  {
    maskedKey[i] = 0x5555 ^ pKey[i];
  }
  
  nr = aesKeyExpandEnc(roundkey, (tword *)maskedKey, SALLD_SRTP_CIPHER_KEY_SIZE<<3);
  
  /* Encrypt the IV with masked Key */
  aesEncrypt(roundkey, iv, iv, nr);

  return 0; 
} /* salld_srtcp_form_f8_iv */
/* Nothing past this point */
