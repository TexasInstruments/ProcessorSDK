/******************************************************************************
 * FILE PURPOSE: Secure RTCP Form IV algorithm for AES_CTR mode
 ******************************************************************************
 * FILE NAME: salldsrtcp_ctr.c
 *
 * DESCRIPTION: The main module for Secure RTCP Code in AES_CTR mode
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

/* SALLD header files */
#include "src/salldloc.h"
#include "src/salldport.h"
#include "salldsrtcploc.h"

/******************************************************************************
 * FUNCTION PURPOSE: SRTCP Form IV for ctr mode
 ******************************************************************************
 * DESCRIPTION: 
 *
 * void salld_srtcp_form_ctr_iv (
 *    void*  keyInfo,    - A pointer to Key Info
 *    tword  *pkt,        - packet pointer
 *    tword  *iv)         - iv out pointer
 *    uint32_t index)       - RTCP Sequence Number
 *
 *    IV = (session_salt<<16) XOR (SSRC<<64) XOR (RTCP index << 16)
 *
 * Note: 2-byte alignment is required for IV
 *
 *****************************************************************************/
int16_t salld_srtcp_form_ctr_iv(void* keyInfo, tword *pkt, tword *iv, uint32_t index)
{
  salldSrtcpKeyInfo_t* pKeyInfo = (salldSrtcpKeyInfo_t*)keyInfo;
  uint16_t *pSalt = pKeyInfo->sessionSalt;
  uint16_t *pIV = (uint16_t *)iv;

  pIV[0] = pSalt[0];
  pIV[1] = pSalt[1];
  pIV[2] = pSalt[2] ^ SALLD_UINT16_BE(pktRead16bits_m(pkt, 4));
  pIV[3] = pSalt[3] ^ SALLD_UINT16_BE(pktRead16bits_m(pkt, 6));
  pIV[4] = pSalt[4];
  pIV[5] = pSalt[5] ^ SALLD_UINT16_BE(index >> 16);
  pIV[6] = pSalt[6] ^ SALLD_UINT16_BE(index & 0xFFFF);
  pIV[7] = 0;

  return 0; 
} /* salld_srtcp_form_ctr_iv */
/* Nothing past this point */


