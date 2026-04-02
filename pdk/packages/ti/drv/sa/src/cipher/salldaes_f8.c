/******************************************************************************
 * FILE PURPOSE:  AES-f8 Mode Algorithms
 ******************************************************************************
 * FILE NAME:   salldAes_f8.c  
 *
 * DESCRIPTION: AES f8 (3GPP) Mode code implementation 
 *
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
#include "salldaes.h"
/******************************************************************************
 * FUNCTION PURPOSE: AES f8 Mode operation
 ******************************************************************************
 * DESCRIPTION: Does inplace encryption/decryption in f8 mode
 *
 *    int8_t salld_aes_f8(
 *              uint32_t *key,     Expanded Round Key Array
 *              int16_t   keylen,   Key size in bits
 *              int16_t   nr,       NR: number of rounds
 *              tword   *iv,      Initialization Vector
 *              tword   *inpkt,   Input packet to do encryption/decryption
 *              int16_t   inputLen, Payload length in bytes
 *              tword   *outpkt)  Output encrypted/decrypted packet
 ******************************************************************************
 *    Algorithm: S(j) = E(key, IV XOR j XOR S(j-1));
 *               OutPkt(j) = S(j) XOR InPkt(j); for encryption/decryption
 *
 *                  IV
 *                   |
 *                   +-----------+-------------+--  ...     ------+
 *                   |           |             |                  |
 *                   |   j=1 -> (*)    j=2 -> (*)   ...  j=L-1 ->(*)
 *                   |           |             |                  |
 *                   |      +-> (*)       +-> (*)   ...      +-> (*)
 *                   |      |    |        |    |             |    |
 *                   v      |    v        |    v             |    v
 *               +------+   | +------+    | +------+         | +------+
 *               |      |   | |      |    | |      |         | |      |
 *    key ------>|  AES |   | |  AES |    | |  AES |         | |  AES |
 *               |      |   | |      |    | |      |         | |      |
 *               +------+   | +------+    | +------+         | +------+
 *                   |      |    |        |    |             |    |
 *                   +------+    +--------+    +--  ...  ----+    |
 *                   |           |             |                  |
 *                   v           v             v                  v
 *                  S(0)        S(1)          S(2)  . . .       S(L-1)
 *
 ******************************************************************************/

#if 0
int16_t salld_aes_f8(uint32_t *key, int16_t keylen, int16_t nr, tword *iv, 
						salldAesInst_t *inst, salldAesDesc_t *desc)
{
  int16_t i, j, numBlocks;
  int16_t resid_tword_len=0;
  tword block[SALLD_AES_BLOCK_SIZE_IN_WORD];
  uint16_t temp;
  tword iv_local[SALLD_AES_BLOCK_SIZE_IN_WORD];
  tword *tempIV = iv;
  uint16_t j_16 = 1;
  tword *inpkt, *outpkt;
  int16_t inputLen;
  
  inpkt = desc->segments[0];
  outpkt = desc->segments[0];
  inputLen = desc->pktLen;

  if (inpkt == NULL || inputLen <= 0) 
  {
    return 0; /* nothing to do */
  }

  /* Calculate number of 16-bit blocks. */
  numBlocks = inputLen >> 4;

  /* clean 112 bits in the local iv array */
  for (j=0; j<SALLD_16LSB_OFFSET_IN_WORD;j++) 
  {
    iv_local[j] = 0;
  }
  
  for (i = numBlocks; i > 0; i--) 
  {

    aesEncrypt((tulong *)key, tempIV, block, nr);

    for (j=0; j<SALLD_AES_BLOCK_SIZE_IN_WORD;j++) 
    {
      /* XOR Plaintext and Cipher-Stream */
      outpkt[j] = inpkt[j] ^ block[j];
    }

    temp = (uint16_t)pktRead16bits_m(iv, SALLD_16LSB_OFFSET_IN_BYTE);
    /* IV XOR j */
    temp ^= SALLD_UINT16_BE(j_16);
    j_16++; 
    /* IV XOR j XOR S(j-1) */ 
    temp ^= (uint16_t)pktRead16bits_m(block, SALLD_16LSB_OFFSET_IN_BYTE);

    /* store back the lower 16bits of this into iv_local*/
    pktWrite16bits_m(iv_local, SALLD_16LSB_OFFSET_IN_BYTE, temp);
    
    for (j=0; j<SALLD_16LSB_OFFSET_IN_WORD;j++) 
    {
      /* IV XOR j XOR S(j-1) */
      iv_local[j] = iv[j] ^ block[j];
    }

    tempIV = iv_local;
    inpkt += SALLD_AES_BLOCK_SIZE_IN_WORD;
    outpkt += SALLD_AES_BLOCK_SIZE_IN_WORD;
  }

  /* last incomplete block */
  resid_tword_len = 
    (inputLen - SALLD_AES_BLOCK_SIZE_IN_BYTE * numBlocks  
    + (SALLD_SIZE_OF_WORD_IN_BYTE - 1)) / SALLD_SIZE_OF_WORD_IN_BYTE;
       
  if (resid_tword_len > 0) 
  {
    /* Encrypt the iv/ctr for last incomplete block */
    aesEncrypt((tulong *)key, tempIV, block, nr);

    /* Only write onto output for the needed number of bytes
       so we don't overwrite any data  */
    for (j=0;j<resid_tword_len;j++)
    {
      outpkt[j] = block[j] ^ inpkt[j];
    }
  }

 return 0;
}
#endif


int16_t salld_aes_f8(uint32_t *key, int16_t keylen, int16_t nr, tword *iv, 
						salldAesDesc_t *desc)
{
  int16_t i, j, numBlocks, totLen;
  tword block[SALLD_AES_BLOCK_SIZE_IN_WORD];
  uint16_t ivSeqn;
  tword iv_local[SALLD_AES_BLOCK_SIZE_IN_WORD];
  tword *tempIV = iv;
  uint16_t j_16 = 1;
  int segIdx = 0;
  int keyRem;
  int totRem = totLen = desc->pktLen;
  tword *segPtr = desc->segments[segIdx];
  tword *blockPtr;
  int segRem = desc->inputLen[segIdx++];
  int thisLen;

  if (segPtr == NULL || segRem <= 0) 
  {
    return 0; /* nothing to do */
  }

  /* Calculate number of 16-bit blocks. */
  numBlocks = (totLen + 15) >> 4;

  /* clean 112 bits in the local iv array */
  for (j=0; j<SALLD_16LSB_OFFSET_IN_WORD;j++) 
  {
    iv_local[j] = 0;
  }
  
  /* We assume that packet payload length will never cross 2^23 bits */
  ivSeqn = (uint16_t)pktRead16bits_m(iv, SALLD_16LSB_OFFSET_IN_BYTE);
  for (i = numBlocks; i > 0; i--) 
  {

    /* Make one block of key */
    aesEncrypt((tulong *)key, tempIV, block, nr);
    blockPtr = block;
    
    /* IV XOR j */
    ivSeqn ^= SALLD_UINT16_BE(j_16);
    j_16++; 
    /* IV XOR j XOR S(j-1) */ 
    ivSeqn ^= (uint16_t)pktRead16bits_m(block, SALLD_16LSB_OFFSET_IN_BYTE);
    
    /* Only write back the IV if the entire key block (16 bytes) is consumed */
    if(totRem >= SALLD_AES_BLOCK_SIZE_IN_WORD)
    {
        /* store back the lower 16bits of this into iv_local*/
        pktWrite16bits_m(iv_local, SALLD_16LSB_OFFSET_IN_BYTE, ivSeqn);
    
        for (j=0; j<SALLD_16LSB_OFFSET_IN_WORD;j++) 
        {
            /* IV XOR j XOR S(j-1) */
            iv_local[j] = iv[j] ^ block[j];
        }

        tempIV = iv_local;
    }
    
    /* Process as many sub segments needed to consume the key block */
        for (keyRem = SALLD_AES_BLOCK_SIZE_IN_WORD;
             (keyRem > 0) && (segRem > 0) && (totRem > 0); ) {
            thisLen = keyRem;
            if (thisLen > segRem) {
                thisLen = segRem;
            }
            keyRem -= thisLen;
            segRem -= thisLen;
            totRem -= thisLen;
#pragma PROB_ITERATE(16,16)
            for (j=0; j < thisLen; j++) 
            {
              /* XOR Plaintext and Cipher-Stream */
              *segPtr = *segPtr ^ *blockPtr;
              segPtr++;
              blockPtr++;
            }
            if (segRem == 0) {
                segRem = desc->inputLen[segIdx];
                segPtr = desc->segments[segIdx++];
            }
        }
  }
    return 0;
}

/* nothing past this point */
