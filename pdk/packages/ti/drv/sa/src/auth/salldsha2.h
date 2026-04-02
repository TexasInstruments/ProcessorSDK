#ifndef SALLDSHA2_H
#define SALLDSHA2_H
/******************************************************************************
 * FILE PURPOSE:  SHA2 Message Authentication Algorithm Defines
 ******************************************************************************
 * FILE NAME:   salldsha2.h  
 *
 * DESCRIPTION: Secure Hash Algorithm1-HMAC as per NIST FIPS PUB 180-1
 *
 * (C) Copyright 2009, Texas Instruments Inc. 
 *
 * The following source was taken from cryptographic software written by the below 
 * authors and has been modified and adapted by TI
 ******************************************************************************
 * Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 ******************************************************************************/
#define SALLD_SHA2_BLOCK_SIZE_IN_BYTE           64   /* Bytes */
#define SALLD_SHA2_LBLOCK	                    16   
#define SALLD_SHA2_BLOCK	                    16   /* Number of 32 bit words */
#define SALLD_SHA2_LAST_BLOCK_OFFSET_IN_BYTE    56   /* last two 32 bit words */
#define SALLD_SHA2_LENGTH_BLOCK                 8    /*????*/
#define SALLD_SHA2_DIGEST_LENGTH_IN_BYTE        32   /* */
#define SALLD_SHA224_DIGEST_LENGTH	            28
#define SALLD_SHA256_DIGEST_LENGTH	            32

/* Structure used by SHA2 algorithm */
typedef struct sha2Inst_s
{
	uint32_t h[8];                     /* H Buffers */
	uint32_t Nl,Nh;                  
	uint32_t data[SALLD_SHA2_LBLOCK];  /* 32 bit words in a BLOCK */
	uint16_t num;
    uint16_t mdLen;
} salldSha2Inst_t;

void salld_sha224_init(salldSha2Inst_t *inst);
void salld_sha224_update(salldSha2Inst_t *inst, tword *data, uint32_t len);
void salld_sha256_init(salldSha2Inst_t *inst);
void salld_sha256_update(salldSha2Inst_t *inst, tword *data, uint32_t len);

#endif
