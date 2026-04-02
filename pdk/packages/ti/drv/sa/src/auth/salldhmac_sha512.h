#ifndef SALLDHMAC_SHA512_H
#define SALLDHMAC_SHA512_H
/******************************************************************************
 * FILE PURPOSE:  HMAC_SHA512 Message Authentication Algorithm Defines
 ******************************************************************************
 * FILE NAME:   salldhmac_sha512.h
 *
 * DESCRIPTION: Secure Hash Algorithm1-HMAC as per NIST FIPS PUB 180-4
 *
 *              
 * (C) Copyright 2017, Texas Instruments, Inc.
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

#define SALLD_MSU_HMAC_IPAD_PATTERN 0x3636
#define SALLD_MSU_HMAC_OPAD_PATTERN 0x5c5c

#define SALLD_MSU_HMAC_PAD_LEN_IN_BYTE         (64)
#define SALLD_MSU_HMAC_PAD_LEN_IN_WORD         SALLD_BYTE_TO_WORD(SALLD_MSU_HMAC_PAD_LEN_IN_BYTE)
#define SALLD_MSU_HMAC_PAD_LEN_IN_TUINT        (32)

#define SALLD_HMAC_SHA512_DIGEST_LEN_IN_BYTE  (64)
#define SALLD_HMAC_SHA512_DIGEST_LEN_IN_WORD SALLD_BYTE_TO_WORD(SALLD_HMAC_SHA1_DIGEST_LEN_IN_BYTE)
#define SALLD_HMAC_SHA512_DIGEST_LEN_IN_TUINT (32)

#define SALLD_MSU_HMAC1024_PAD_LEN_IN_BYTE     (128)
#define SALLD_HMAC_SHA1024_DIGEST_LEN_IN_WORD SALLD_BYTE_TO_WORD(SALLD_MSU_HMAC1024_PAD_LEN_IN_BYTE)
#define SALLD_MSU_HMAC1024_PAD_LEN_IN_TUINT    (64)

#define SALLD_MSU_MAC_AUTHENTICATION            1
#define SALLD_MSU_MAC_GENERATION                2


int16_t salld_hmac_sha384_get_pad(uint16_t *key, uint16_t keyLen, uint16_t *ipad, uint16_t *opad);
int16_t salld_hmac_sha512_get_pad(uint16_t *key, uint16_t keyLen, uint16_t *ipad, uint16_t *opad);

#endif
