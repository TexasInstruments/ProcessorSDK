#ifndef _SALLDHMAC_MD5_H
#define _SALLDHMAC_MD5_H
/******************************************************************************
 * FILE PURPOSE:  HMAC_MD5 Message Authentication Algorithm Defines
 ******************************************************************************
 * FILE NAME:   salldHmac_md5.h  
 *
 * DESCRIPTION: MD5-HMAC
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
#include "src/salldport.h"

#define SALLD_HMAC_IPAD_PATTERN 0x36
#define SALLD_HMAC_OPAD_PATTERN 0x5c

#define SALLD_HMAC_PAD_LEN_IN_BYTE         (64)
#define SALLD_HMAC_PAD_LEN_IN_TUINT        (32)
#define SALLD_HMAC_PAD_LEN_IN_WORD         SALLD_BYTE_TO_WORD(SALLD_HMAC_PAD_LEN_IN_BYTE)

#define SALLD_HMAC_MD5_DIGEST_LEN_IN_BYTE  (16)
#define SALLD_HMAC_MD5_DIGEST_LEN_IN_WORD  SALLD_BYTE_TO_WORD(SALLD_HMAC_MD5_DIGEST_LEN_IN_BYTE)
#define SALLD_HMAC_MD5_DIGEST_LEN_IN_TUINT (8)

int16_t salld_hmac_md5(uint16_t *key, int16_t keyLen, tword *tag, int16_t tagLen, 
						salldAesDesc_t *desc, uint16_t options, tword *pad);
int16_t salld_hmac_md5_get_pad(uint16_t *key, uint16_t keyLen, uint16_t *ipad,  uint16_t *opad);
#endif
