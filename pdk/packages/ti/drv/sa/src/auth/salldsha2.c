/******************************************************************************
 * FILE PURPOSE:  SHA2 Message Authentication Algorithm
 ******************************************************************************
 * FILE NAME:   salldsha2.c  
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
 
/* RTSC header files */ 

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include "src/salldport.h"
#include "salldsha2.h"
#include "salldsha2loc.h"

static const uint32_t K256[64] = {
	0x428a2f98UL,0x71374491UL,0xb5c0fbcfUL,0xe9b5dba5UL,
	0x3956c25bUL,0x59f111f1UL,0x923f82a4UL,0xab1c5ed5UL,
	0xd807aa98UL,0x12835b01UL,0x243185beUL,0x550c7dc3UL,
	0x72be5d74UL,0x80deb1feUL,0x9bdc06a7UL,0xc19bf174UL,
	0xe49b69c1UL,0xefbe4786UL,0x0fc19dc6UL,0x240ca1ccUL,
	0x2de92c6fUL,0x4a7484aaUL,0x5cb0a9dcUL,0x76f988daUL,
	0x983e5152UL,0xa831c66dUL,0xb00327c8UL,0xbf597fc7UL,
	0xc6e00bf3UL,0xd5a79147UL,0x06ca6351UL,0x14292967UL,
	0x27b70a85UL,0x2e1b2138UL,0x4d2c6dfcUL,0x53380d13UL,
	0x650a7354UL,0x766a0abbUL,0x81c2c92eUL,0x92722c85UL,
	0xa2bfe8a1UL,0xa81a664bUL,0xc24b8b70UL,0xc76c51a3UL,
	0xd192e819UL,0xd6990624UL,0xf40e3585UL,0x106aa070UL,
	0x19a4c116UL,0x1e376c08UL,0x2748774cUL,0x34b0bcb5UL,
	0x391c0cb3UL,0x4ed8aa4aUL,0x5b9cca4fUL,0x682e6ff3UL,
	0x748f82eeUL,0x78a5636fUL,0x84c87814UL,0x8cc70208UL,
	0x90befffaUL,0xa4506cebUL,0xbef9a3f7UL,0xc67178f2UL 
};

/******************************************************************************
 * FUNCTION PURPOSE: SHA2 Support Function 
 ******************************************************************************
 * DESCRIPTION: SHA1 Block Processing Function 
 *
 *    void Sha256Block(         
 *      salldSha2Inst_t    *inst  - SHA2 instance
 *      uint32_t             *p)    - Array of Data
 *
 *****************************************************************************/
static void sha256Block (salldSha2Inst_t *inst, uint32_t *p)
{
	uint32_t a,b,c,d,e,f,g,h,s0,s1,T1,T2;
	uint32_t	X[16];
	int i;

	a = inst->h[0];	b = inst->h[1];	c = inst->h[2];	d = inst->h[3];
	e = inst->h[4];	f = inst->h[5];	g = inst->h[6];	h = inst->h[7];

	for (i=0;i<16;i++)
	{
        T1 = X[i] = p[i];
	    T1 += h + Sigma1(e) + Ch(e,f,g) + K256[i];
	    T2 = Sigma0(a) + Maj(a,b,c);
	    h = g;	g = f;	f = e;	e = d + T1;
	    d = c;	c = b;	b = a;	a = T1 + T2;
	}

	for (;i<64;i++)
	{
	    s0 = X[(i+1)&0x0f];	    s0 = sigma0(s0);
	    s1 = X[(i+14)&0x0f];	s1 = sigma1(s1);

	    T1 = X[i&0xf] += s0 + s1 + X[(i+9)&0xf];
	    T1 += h + Sigma1(e) + Ch(e,f,g) + K256[i];
	    T2 = Sigma0(a) + Maj(a,b,c);
	    h = g;	g = f;	f = e;	e = d + T1;
	    d = c;	c = b;	b = a;	a = T1 + T2;
	}

	inst->h[0] += a;	inst->h[1] += b;	inst->h[2] += c;	inst->h[3] += d;
	inst->h[4] += e;	inst->h[5] += f;	inst->h[6] += g;	inst->h[7] += h;
}

/******************************************************************************
 * FUNCTION PURPOSE: SHA2-224 Context Initialization
 ******************************************************************************
 * DESCRIPTION: Initializes the sha2-224 context
 *
 *    void salld_sha224_init(         
 *      salldSha2Inst_t    *inst)  - SHA1 instance
 *
 *****************************************************************************/
void salld_sha224_init (salldSha2Inst_t *inst)
{
	inst->h[0]=0xc1059ed8UL;	inst->h[1]=0x367cd507UL;
	inst->h[2]=0x3070dd17UL;	inst->h[3]=0xf70e5939UL;
	inst->h[4]=0xffc00b31UL;	inst->h[5]=0x68581511UL;
	inst->h[6]=0x64f98fa7UL;	inst->h[7]=0xbefa4fa4UL;
	inst->Nl=0;	inst->Nh=0;
	inst->num=0;	inst->mdLen=SALLD_SHA224_DIGEST_LENGTH;
}

/******************************************************************************
 * FUNCTION PURPOSE: SHA2-256 Context Initialization
 ******************************************************************************
 * DESCRIPTION: Initializes the sha2-256 context
 *
 *    void salld_sha256_init(         
 *      salldSha2Inst_t    *inst)  - SHA1 instance
 *
 *****************************************************************************/
void salld_sha256_init (salldSha2Inst_t *inst)
{
	inst->h[0]=0x6a09e667UL;	inst->h[1]=0xbb67ae85UL;
	inst->h[2]=0x3c6ef372UL;	inst->h[3]=0xa54ff53aUL;
	inst->h[4]=0x510e527fUL;	inst->h[5]=0x9b05688cUL;
	inst->h[6]=0x1f83d9abUL;	inst->h[7]=0x5be0cd19UL;
	inst->Nl=0;	inst->Nh=0;
	inst->num=0;	inst->mdLen=SALLD_SHA256_DIGEST_LENGTH;
}

/******************************************************************************
 * FUNCTION PURPOSE: SHA256 Update Function
 ******************************************************************************
 * DESCRIPTION: SHA256 Update Function
 *
 *    void salld_sha256_update(         
 *      salldSha21Inst_t   *sha2Inst   - SHA2 instance
 *      tword              *data,      - message for digesting
 *      uint32_t             len)        - length of message in BYTE
 *
 *****************************************************************************/
void salld_sha256_update(salldSha2Inst_t *sha2Inst, tword *data, uint32_t len)
{
  uint32_t *p;
  int16_t  ew,ec,sw;
  uint32_t l;
  uint32_t offset=0;

  if (len == 0) return;

  /*  */
  l = (sha2Inst->Nl + (len<<3)) & 0xffffffff;
  if (l < sha2Inst->Nl) /* overflow */
  {
    sha2Inst->Nh++;
  }
  sha2Inst->Nh += (len>>29);
  sha2Inst->Nl = l;

  /* we now can process the input data in blocks of SHA_CBLOCK
   * chars and save the leftovers to sha2Inst->data. */
  p = sha2Inst->data;
  while (len >= SALLD_SHA2_BLOCK_SIZE_IN_BYTE)
  {
    for (sw = (SALLD_SHA2_BLOCK/4); sw; sw--, offset += 16)
    {
        *p++ = pktRead32bits_m(data, offset);
        *p++ = pktRead32bits_m(data, offset+4);
        *p++ = pktRead32bits_m(data, offset+8);
        *p++ = pktRead32bits_m(data, offset+12);
    }
    p = sha2Inst->data;
    sha256Block(sha2Inst, p);
    len -= SALLD_SHA2_BLOCK_SIZE_IN_BYTE;
  }
  ec = (int16_t)len;
  sha2Inst->num = ec;
  ew = (ec >> 2);
  ec &= 0x03;

  for (sw = 0; sw < ew; sw++)
  {
    p[sw] = pktRead32bits_m(data, offset);
  }
  
  if(ec)
  {
    p[sw] = pktRead32bits_m(data, offset) & MacLastWordMask[ec-1];
  }
}

/******************************************************************************
 * FUNCTION PURPOSE: SHA224 Update Function
 ******************************************************************************
 * DESCRIPTION: SHA224 Update Function
 *
 *    void salld_sha224_update(         
 *      salldSha21Inst_t   *sha2Inst   - SHA2 instance
 *      tword              *data,      - message for digesting
 *      uint32_t             len)        - length of message in BYTE
 *
 *****************************************************************************/
void salld_sha224_update(salldSha2Inst_t *sha2Inst, tword *data, uint32_t len)
{
    salld_sha256_update(sha2Inst, data, len);
}

/* nothing past this point */

