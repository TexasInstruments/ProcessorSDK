/******************************************************************************
 * FILE PURPOSE:  SHA1 Message Authentication Algorithm c64 Port File
 ******************************************************************************
 * FILE NAME:   salldsha1.c  
 *
 * DESCRIPTION: Secure Hash Algorithm1 (SHA1) as per NIST FIPS PUB 180-1
 *
 *              Note: It runs on MIPS/ARM or DSP
 *              So maintain this support while you modify this code.
 *              
 * (C) Copyright 2008 Texas Instruments Inc - http://www.ti.com
 *
 * The following source was taken from cryptographic software written by Eric Young and has
 * been modified and adapted by TI
 *****************************************************************************
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
 *****************************************************************************/

/* Standard C headers */
#include <string.h>

#include "salldsha1.h"
/******************************************************************************
 * FUNCTION PURPOSE: SHA1 Instance Initialization
 ******************************************************************************
 * DESCRIPTION: Initializes the sha1 context
 *
 *    void salld_sha1Init(         
 *      void            *inst,      - Pointer to an unitialized SHA1 instance.
 *      void            *preInst) - Precomputed, algorithm instance pointer.
 *                                     This input parameter allows the SHA1 code to conform to an
 *                                     HMAC level optimization described in RFC2104 - Section 4.
 *                                     Passing a NULL pointer through this parameter will initialize
 *                                     the SHA1 algorithm instance normally.  Passing a precomputed 
 *                                     SHA1inst through this parameter will cause the precomputed 
 *                                     SHA1 data to be copied to the currently in use SHA1 algorithm
 *                                     instance.      
 *****************************************************************************/
void salld_sha1Init(void *inst, void *preInst)
{
  sha1Inst_t   *sha1Inst = (sha1Inst_t*)inst;
  sha1Inst_t   *k_pad_inst = (sha1Inst_t*)preInst;
  tuint i;

  /* If NULL is passed as the pointer to preInst set the sha1Inst to the initial
    * hashing parameters */
  if (!k_pad_inst)
  {
    sha1Inst->h0=sha1_INIT_DATA_h0;
    sha1Inst->h1=sha1_INIT_DATA_h1;
    sha1Inst->h2=sha1_INIT_DATA_h2;
    sha1Inst->h3=sha1_INIT_DATA_h3;
    sha1Inst->h4=sha1_INIT_DATA_h4;
    sha1Inst->Nl=0;
    sha1Inst->Nh=0;
    sha1Inst->num=0;
  } 
  /* If a pointer to a saved hash has been passed through the preInst point, set
    * the sha1Inst to the saved hash's parameters.  This is utilized in HMAC 
    * optimizations.  */
  else
  {
    sha1Inst->h0 = k_pad_inst->h0;
    sha1Inst->h1 = k_pad_inst->h1;
    sha1Inst->h2 = k_pad_inst->h2;
    sha1Inst->h3 = k_pad_inst->h3;
    sha1Inst->h4 = k_pad_inst->h4;
    sha1Inst->Nl = k_pad_inst->Nl;;
    sha1Inst->Nh = k_pad_inst->Nh;
    sha1Inst->num = k_pad_inst->num;

    for (i=0;i < sha1_NUM_WORDS_IN_BLOCK;i++)
    {
      sha1Inst->data[i] = k_pad_inst->data[i];
    }
  }   
}

/******************************************************************************
 * FUNCTION PURPOSE: SHA1 Message Digest Computation Function
 ******************************************************************************
 * DESCRIPTION: Computes the hash of the input message up to the last block of data
 *                 which is greater than or equal to 512 bits long.  If there is leftover message
 *                 data it is returned through the algorithm instance pointer, as well as the 
 *                 computed message digest up to that point.  If salld_sha1Update is called simultaneously
 *                 two or more times the leftover data from the prior call will be appended to the
 *                 data and processed.
 *
 *    void salld_sha1Update(         
 *      void          *inst,      - SHA1 instance pointer.
 *      tword        *data,     - message for hashing.
 *      tulong        len)        - length of message in bytes.
 *****************************************************************************/
void salld_sha1Update(void *inst, tword *data, tulong len)
{
  tword *p;
  tulong l;
  tint n;
  sha1Inst_t *c = (sha1Inst_t *)inst;

  if (len==0)
  {
    return;
  }

  l=(c->Nl+((len)<<3))&0xffffffffUL;

  /* 95-05-24 eay Fixed a bug with the overflow handling, thanks to
    * Wei Dai <weidai@eskimo.com> for pointing it out. */
  if (l < c->Nl) /* overflow */
  {
    c->Nh++;
  }
  c->Nh+=(len>>29);	/* might cause compiler warning on 16-bit */
  c->Nl=l;

  /* Number of bytes leftover from previous call to salld_sha1Update */
  n = c->num;
  
  if (n > 0)
  {
    p=(tword *)c->data;

    if ((n+len) >= sha1_BLOCK_SIZE_IN_BYTE)
    {
      /* If leftover bytes plus new bytes are greater than the standard SHA-1 block (64 bytes)
        * append the new data to the leftover data and hash the block */
      memcpy (p+n, data, (sha1_BLOCK_SIZE_IN_BYTE-n));  
      sha1_block(c,p,1);
      n = (sha1_BLOCK_SIZE_IN_BYTE-n);
      data += n;
      len -= n;
      c->num = 0;
      memset (p,0,sha1_BLOCK_SIZE_IN_BYTE);	/* keep it zeroed */
    }
    else
    {
      /* If leftover bytes plus new bytes aren't greater than 64 bytes append new data and
        * update the number of leftover bytes in the SHA-1 context */
      memcpy (p+n,data,len);
      c->num += (tuint)len;
      return;
    }
  }

  n = len/sha1_BLOCK_SIZE_IN_BYTE;
  if (n > 0)
  {
    /* Hash as many 512 bit blocks of the input data as possible */
    sha1_block(c,data,n);
    n *= sha1_BLOCK_SIZE_IN_BYTE;
    data += n;
    len  -= n;
  }

  /* Store any leftover bytes in the data field of the SHA-1 context */
  if (len != 0)
  {
    p = (tword *)c->data;
    c->num = len;
    memcpy (p,data,len);
  }
  
  return;
}

/******************************************************************************
 * FUNCTION PURPOSE: SHA1 Final Stage of Message Digest Computation
 ******************************************************************************
 * DESCRIPTION: Function where the final portion of the message which was less than 512 bits
 *                 is padded in accordance with the FIPS 180-1 standard, and hashed.
 *
 *    void salld_sha1Final(         
 *      void            *inst      - SHA1 instance pointer.
 *      tword          *md)     - Pointer to completed message digest.
 *****************************************************************************/
void salld_sha1Final(void *inst, tword *md)
{
  sha1Inst_t *c = (sha1Inst_t *)inst;
  tword *p = (tword *)c->data;
  tint n = c->num;

  /* Append a 1 signifying the end of the input data */
  p[n] = 0x80; /* there is always room for one */
  n++;

  if (n > (sha1_BLOCK_SIZE_IN_BYTE-8))
  {
    /* Pad the rest of the block and hash it if the current block doesn't have room to append 
      * the number of bits hashed */
    memset (p+n,0,sha1_BLOCK_SIZE_IN_BYTE-n);

    n=0;
    sha1_block(c,p,1);
  }

  /* Pad the current block up to the last 8 bytes.  The last 8 bytes are reserved for the number
    * of bits that were hashed */
  memset (p+n,0,sha1_BLOCK_SIZE_IN_BYTE-8-n);

  p += (sha1_BLOCK_SIZE_IN_BYTE-8);

  salld_util_PUTU32_I(p, c->Nh);
  salld_util_PUTU32_I(p, c->Nl);

  p -= (sha1_BLOCK_SIZE_IN_BYTE);
  sha1_block(c,p,1);
  c->num=0;
  memset (p,0,sha1_BLOCK_SIZE_IN_BYTE);

  salld_util_PUTU32_I(md, c->h0);
  salld_util_PUTU32_I(md, c->h1);
  salld_util_PUTU32_I(md, c->h2);
  salld_util_PUTU32_I(md, c->h3);
  salld_util_PUTU32_I(md, c->h4);

  return;
}

/* nothing past this point */

