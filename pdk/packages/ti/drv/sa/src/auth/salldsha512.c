/******************************************************************************
 * FILE PURPOSE:  SHA512 Message Authentication Algorithm
 ******************************************************************************
 * FILE NAME:   salldsha512.c  
 *
 * DESCRIPTION: Secure Hash Algorithm1-HMAC as per NIST FIPS PUB 180-1
 *
 * (C) Copyright 2017, Texas Instruments Inc. 
 *
 * The following source was taken from cryptographic software written by the below 
 * authors and has been modified and adapted by TI
 ******************************************************************************
 * Copyright (C) 1995-1997 Eric Young (eay@cryptsoft.com)
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
#include "salldsha512.h"
#include "salldsha512loc.h"

static const uint64_t K512[80] = {
    0x428a2f98d728ae22UL, 0x7137449123ef65cdUL, 0xb5c0fbcfec4d3b2fUL, 0xe9b5dba58189dbbcUL, 0x3956c25bf348b538UL, 
    0x59f111f1b605d019UL, 0x923f82a4af194f9bUL, 0xab1c5ed5da6d8118UL, 0xd807aa98a3030242UL, 0x12835b0145706fbeUL, 
    0x243185be4ee4b28cUL, 0x550c7dc3d5ffb4e2UL, 0x72be5d74f27b896fUL, 0x80deb1fe3b1696b1UL, 0x9bdc06a725c71235UL, 
    0xc19bf174cf692694UL, 0xe49b69c19ef14ad2UL, 0xefbe4786384f25e3UL, 0x0fc19dc68b8cd5b5UL, 0x240ca1cc77ac9c65UL, 
    0x2de92c6f592b0275UL, 0x4a7484aa6ea6e483UL, 0x5cb0a9dcbd41fbd4UL, 0x76f988da831153b5UL, 0x983e5152ee66dfabUL, 
    0xa831c66d2db43210UL, 0xb00327c898fb213fUL, 0xbf597fc7beef0ee4UL, 0xc6e00bf33da88fc2UL, 0xd5a79147930aa725UL, 
    0x06ca6351e003826fUL, 0x142929670a0e6e70UL, 0x27b70a8546d22ffcUL, 0x2e1b21385c26c926UL, 0x4d2c6dfc5ac42aedUL, 
    0x53380d139d95b3dfUL, 0x650a73548baf63deUL, 0x766a0abb3c77b2a8UL, 0x81c2c92e47edaee6UL, 0x92722c851482353bUL, 
    0xa2bfe8a14cf10364UL, 0xa81a664bbc423001UL, 0xc24b8b70d0f89791UL, 0xc76c51a30654be30UL, 0xd192e819d6ef5218UL, 
    0xd69906245565a910UL, 0xf40e35855771202aUL, 0x106aa07032bbd1b8UL, 0x19a4c116b8d2d0c8UL, 0x1e376c085141ab53UL, 
    0x2748774cdf8eeb99UL, 0x34b0bcb5e19b48a8UL, 0x391c0cb3c5c95a63UL, 0x4ed8aa4ae3418acbUL, 0x5b9cca4f7763e373UL, 
    0x682e6ff3d6b2b8a3UL, 0x748f82ee5defb2fcUL, 0x78a5636f43172f60UL, 0x84c87814a1f0ab72UL, 0x8cc702081a6439ecUL, 
    0x90befffa23631e28UL, 0xa4506cebde82bde9UL, 0xbef9a3f7b2c67915UL, 0xc67178f2e372532bUL, 0xca273eceea26619cUL, 
    0xd186b8c721c0c207UL, 0xeada7dd6cde0eb1eUL, 0xf57d4f7fee6ed178UL, 0x06f067aa72176fbaUL, 0x0a637dc5a2c898a6UL, 
    0x113f9804bef90daeUL, 0x1b710b35131c471bUL, 0x28db77f523047d84UL, 0x32caab7b40c72493UL, 0x3c9ebe0a15c9bebcUL, 
    0x431d67c49c100d4cUL, 0x4cc5d4becb3e42b6UL, 0x597f299cfc657e2aUL, 0x5fcb6fab3ad6faecUL, 0x6c44198c4a475817UL
};

/******************************************************************************
 * FUNCTION PURPOSE: SHA512 Support Function 
 ******************************************************************************
 * DESCRIPTION: SHA512 Block Processing Function 
 *
 *    void Sha512Block(         
 *      salldSha512Inst_t    *inst  - SHA512 instance
 *      uint32_t             *p)    - Array of Data
 *
 *****************************************************************************/
static void salld_sha512_block (salldSha512Inst_t *ctx, uint64_t *p, int16_t num)
{
    uint64_t a,b,c,d,e,f,g,h,s0,s1,T1,T2;
    uint64_t   X[16];
    int i;

    while (num--) {

        a = ctx->h[0];    b = ctx->h[1];    c = ctx->h[2];    d = ctx->h[3];
        e = ctx->h[4];    f = ctx->h[5];    g = ctx->h[6];    h = ctx->h[7];

        for (i=0;i<16;i++)
        {
            T1 = X[i] = p[i];
            T1 += h + Sigma1(e) + Ch(e,f,g) + K512[i];
            T2 = Sigma0(a) + Maj(a,b,c);
            h = g;    g = f;    f = e;    e = d + T1;
            d = c;    c = b;    b = a;    a = T1 + T2;
        }

        for (;i<80;i++)
        {
            s0 = X[(i+1)&0x0f];        s0 = sigma0(s0);
            s1 = X[(i+14)&0x0f];    s1 = sigma1(s1);

            T1 = X[i&0xf] += s0 + s1 + X[(i+9)&0xf];
            T1 += h + Sigma1(e) + Ch(e,f,g) + K512[i];
            T2 = Sigma0(a) + Maj(a,b,c);
            h = g;    g = f;    f = e;    e = d + T1;
            d = c;    c = b;    b = a;    a = T1 + T2;
        }

        ctx->h[0] += a;    ctx->h[1] += b;    ctx->h[2] += c;    ctx->h[3] += d;
        ctx->h[4] += e;    ctx->h[5] += f;    ctx->h[6] += g;    ctx->h[7] += h;
    }
}
/******************************************************************************
 * FUNCTION PURPOSE: SHA-384 Context Initialization
 ******************************************************************************
 * DESCRIPTION: Initializes the sha-384 context
 *
 *    void salld_sha384_init(         
 *      salldSha512Inst_t    *inst)  - SHA512 instance
 *
 *****************************************************************************/
void salld_sha384_init (salldSha512Inst_t *c)
{
    c->h[0]=0xcbbb9d5dc1059ed8UL;    c->h[1]=0x629a292a367cd507UL;
    c->h[2]=0x9159015a3070dd17UL;    c->h[3]=0x152fecd8f70e5939UL;
    c->h[4]=0x67332667ffc00b31UL;    c->h[5]=0x8eb44a8768581511UL;
    c->h[6]=0xdb0c2e0d64f98fa7UL;    c->h[7]=0x47b5481dbefa4fa4UL;
    c->Nl=0;    c->Nh=0;
    c->num=0;    c->md_len=SALLD_SHA384_DIGEST_LENGTH;
}

/******************************************************************************
 * FUNCTION PURPOSE: SHA-512 Context Initialization
 ******************************************************************************
 * DESCRIPTION: Initializes the sha-512 context
 *
 *    void salld_sha512_init(         
 *      salldSha512Inst_t    *inst)  - SHA512 instance
 *
 *****************************************************************************/
void salld_sha512_init (salldSha512Inst_t *c)
{
    c->h[0]=0x6a09e667f3bcc908UL;    c->h[1]=0xbb67ae8584caa73bUL;
    c->h[2]=0x3c6ef372fe94f82bUL;    c->h[3]=0xa54ff53a5f1d36f1UL;
    c->h[4]=0x510e527fade682d1UL;    c->h[5]=0x9b05688c2b3e6c1fUL;
    c->h[6]=0x1f83d9abfb41bd6bUL;    c->h[7]=0x5be0cd19137e2179UL;
    c->Nl=0;    c->Nh=0;
    c->num=0;    c->md_len=SALLD_SHA512_DIGEST_LENGTH;
}

/******************************************************************************
 * FUNCTION PURPOSE: SHA512 Update Function
 ******************************************************************************
 * DESCRIPTION: SHA512 function
 *
 *    void salld_sha512_update(         
 *      sha512Inst_t     *sha512Inst,- SHA512 instance
 *      uint8_t               *data,      - message for digesting
 *      int16_t              len)        - length of message in BYTE
 *
 *
 * @Note: This function is simplied yo handle only one update call per Init.
 *        It can easily enhanced to handle multiple calls
 *****************************************************************************/
void salld_sha512_update(salldSha512Inst_t *sha512Inst, uint8_t *data, int16_t len)
{
  uint64_t *p;
  int16_t  i, ew,ec,sw;
  uint32_t l;
  uint64_t w;

  if (len == 0) return;

  /*  */
  l = (sha512Inst->Nl + (len<<3)) & 0xffffffffffffffffUL;
  if (l < sha512Inst->Nl) /* overflow */
  {
    sha512Inst->Nh++;
  }
  //sha512Inst->Nh += (len>>61);  len is too small here
  sha512Inst->Nl = l;

  /* we now can process the input data in blocks of SHA_CBLOCK
   * chars and save the leftovers to sha512Inst->data. */
  p = sha512Inst->data;
  while (len >= SALLD_SHA512_BLOCK_SIZE_IN_BYTE)
  {
    for (sw = (SALLD_SHA512_BLOCK); sw; sw--)
    {
      *p++ = ((uint64_t)data[0] << 56) |
             ((uint64_t)data[1] << 48) |
             ((uint64_t)data[2] << 40) |
             ((uint64_t)data[3] << 32) |
             ((uint64_t)data[4] << 24) |
             ((uint64_t)data[5] << 16) |
             ((uint64_t)data[6] <<  8) |
             ((uint64_t)data[7] <<  0);
      data += 8;       
    }
    p = sha512Inst->data;
    salld_sha512_block(sha512Inst, p, 1);
    len -= SALLD_SHA512_BLOCK_SIZE_IN_BYTE;
  }
  ec = (int16_t)len;
  sha512Inst->num = ec;
  ew = (ec >> 3);
  ec &= 0x07;

  for (sw = 0; sw < ew; sw++)
  {
      p[sw] = ((uint64_t)data[0] << 56) |
              ((uint64_t)data[1] << 48) |
              ((uint64_t)data[2] << 40) |
              ((uint64_t)data[3] << 32) |
              ((uint64_t)data[4] << 24) |
              ((uint64_t)data[5] << 16) |
              ((uint64_t)data[6] <<  8) |
              ((uint64_t)data[7] <<  0);
      data += 8;        
  }
  
  if(ec)
  {
    for ( i = 0, w = 0; i < ec; i++)
    {
        w <<= 8;
        w |= *data;
        data++;
    }
    p[sw] = w << ((8 - ec)*8);
  }
}
/******************************************************************************
 * FUNCTION PURPOSE: SHA384 Update Function
 ******************************************************************************
 * DESCRIPTION: SHA384 Update Function
 *
 *    void salld_sha384_update(         
 *      salldSha512Inst_t   *sha512Inst   - SHA512 instance
 *      tword              *data,      - message for digesting
 *      uint32_t             len)        - length of message in BYTE
 *
 *****************************************************************************/
void salld_sha384_update(salldSha512Inst_t *sha512Inst, uint8_t *data, int16_t len)
{
    salld_sha512_update(sha512Inst, data, len);
}

/* nothing past this point */

