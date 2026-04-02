/******************************************************************************
 * FILE PURPOSE:  AES-CTR Mode Algorithms
******************************************************************************
 * FILE NAME:   salldAes_ctr.c  
 *
 * DESCRIPTION: AES Counter Mode code implementation 
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
 * FUNCTION PURPOSE: Derive NR from the key size
******************************************************************************
 * DESCRIPTION: Derive AES NR from the given key size in bits
 *
 *    int16_t salld_aes_get_nr(
 *              int16_t   keylen),   Key size in bits
******************************************************************************/
int16_t salld_aes_get_nr(int16_t keyLen)
{
    if(keyLen == 192)
        return(12);
    else if(keyLen == 256)
        return(14);
    else
        return(10);        
}

/******************************************************************************
 * FUNCTION PURPOSE: AES Counter Mode operation
******************************************************************************
 * DESCRIPTION: Does in place Encryption/Decryption of the Packets
 *
 *    int16_t salld_aes_ctr(
 *              uint32_t *key,     Expanded Round Key Array
 *              int16_t   keylen,   Key size in bits
 *              int16_t   nr,       NR: number of rounds
 *              tword   *iv,      Initialization Vector
 *              tword   *inpkt,   Input packet to do encryption/decryption
 *              int16_t   inputLen, Payload length in bytes
 *              tword   *outpkt)  Output encrypted/decrypted packet
******************************************************************************
 *    Algorithm: S(j) = E(key, (IV+j) mod 2^128);
 *               OutPkt(j) = S(j) XOR InPkt(j); for encryption/decryption
 *               
 *
 *                  IV          IV+1          IV+2              IV+L-1
 *                   |           |             |                  |
 *                   |           |             |                  |
 *                   v           v             v                  v
 *               +------+     +------+      +------+           +------+
 *               |      |     |      |      |      |           |      |
 *    key ------>|  AES |     |  AES |      |  AES |  . . .    |  AES |
 *               |      |     |      |      |      |           |      |
 *               +------+     +------+      +------+           +------+
 *                   |           |             |                  |
 *                   |           |             |                  |
 *                   v           v             v                  v
 *                  S(0)        S(1)          S(2)  . . .       S(L-1)
 *
******************************************************************************/
int16_t salld_aes_ctr_single_segment(uint32_t *key, int16_t keylen, int16_t nr, tword *iv, tword *inpkt,
        int16_t inputLen, tword *outpkt)
{
    int16_t i, j, numBlocks;
    int16_t resid_tword_len=0;
    uint16_t temp;
    /* Warning: The starting address should be even for asm code */
    tword block[SALLD_AES_BLOCK_SIZE_IN_WORD];


    if (inpkt == NULL || inputLen <= 0) 
    {
        return 0; /* nothing to do */
    }

    /* Calculate number of 16-byte blocks. */
    numBlocks = inputLen >> 4;

    for (i = numBlocks; i > 0; i--) 
    {
        aesEncrypt((tulong *)key, iv, block, nr);
        for (j=0; j<SALLD_AES_BLOCK_SIZE_IN_WORD; j++) 
        {
            /* XOR Plaintext and Cipher-Stream */
            outpkt[j] = inpkt[j] ^ block[j];
        }
        /* iv = iv + 1; check for (iv) mod 2^128 */
        /* We assume that packet payload length will never cross 2^23 bits */
        temp = (uint16_t)pktRead16bits_m(iv, SALLD_16LSB_OFFSET_IN_BYTE);

        temp++; /* iv = iv + 1 */

        /* TODO: generate a message if it crosses 0x0000FFFF value */

        /* store back the lower 16bits of iv */
        pktWrite16bits_m(iv, SALLD_16LSB_OFFSET_IN_BYTE, temp);

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

        aesEncrypt((tulong *)key, iv, block, nr);

        /* Only write onto output for the needed number of bytes
           so we don't overwrite any data  */
        for (j=0;j<resid_tword_len;j++)
        {
            outpkt[j] = block[j] ^ inpkt[j];
        }
    }
    return 0;
}

/* Provides multisegment processing for salld_aes_ctr */

int16_t salld_aes_ctr(uint32_t *key, int16_t keylen, int16_t nr, tword *iv, salldAesDesc_t *desc)
{
    int16_t i, j, numBlocks, totLen;
    uint16_t ivSeqn;
    /* Warning: The starting address should be even for asm code */
    tword block[SALLD_AES_BLOCK_SIZE_IN_WORD];
    int segIdx = 0;
    int keyRem;
    int totRem = desc->pktLen;
    tword *segPtr = desc->segments[segIdx];
    tword *blockPtr;
    int segRem = desc->inputLen[segIdx++];
    int thisLen;


    if (segPtr == NULL || segRem <= 0) 
    {
        return 0; /* nothing to do */
    }

    totLen = desc->pktLen;

    /* Calculate number of 16-byte blocks. */
    numBlocks = (totLen + 15) >> 4;

    /* We assume that packet payload length will never cross 2^23 bits */
    ivSeqn = (uint16_t)pktRead16bits_m(iv, SALLD_16LSB_OFFSET_IN_BYTE);

    for (i = numBlocks; i > 0; i--) 
    {
        /* Make one block of key */
        aesEncrypt((tulong *)key, iv, block, nr);
        blockPtr = block;

        /* iv = iv + 1; check for (iv) mod 2^128 */
        ivSeqn++; /* iv = iv + 1 */

        /* TODO: generate a message if it crosses 0x0000FFFF value */
        /* store back the lower 16bits of iv */
        if (totRem >= SALLD_AES_BLOCK_SIZE_IN_WORD) {
            /* Only write back the IV if the entire key block (16 bytes) is
             * consumed */
            pktWrite16bits_m(iv, SALLD_16LSB_OFFSET_IN_BYTE, ivSeqn);
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

int16_t salld_aes_ctr_multisegment(uint32_t *key, int16_t keylen, int16_t nr, tword *iv,salldAesInst_t * inst, salldAesDesc_t *desc)
{
    memcpy(inst->key, key, keylen);
    memcpy(inst->iv, iv, SALLD_AES_BLOCK_SIZE_IN_BYTE);
    inst->keylen = keylen;
    inst->num = 0;
    inst->nr = nr;
    inst->index = 0;
	int16_t i;
	for(i = 0; i < desc->nSegments; i++)
	{
		salld_aes_ctr_update(inst, desc);
	}
	return 0;
}

int16_t salld_aes_ctr_update(salldAesInst_t *inst, salldAesDesc_t *desc)
{
    int16_t i, j, numBlocks;
    int16_t resid_word_len = 0;
    int16_t index = inst->index;
    tword *inpkt = desc->segments[index];
	tword *outpkt = inpkt;
    int16_t inputLen = desc->inputLen[index];
    uint16_t tmp;

    if (inpkt == NULL ||  inputLen == 0)
    {
        return 0;
    }

    /* Handle the initial partial block */
    if(inst->num)
    {
        for(j = inst->num; (j < SALLD_AES_BLOCK_SIZE_IN_WORD) && inputLen; j++, inputLen--)
        {
            *outpkt++ = *inpkt++ ^ inst->block[j];
        }
        inst->num = j % SALLD_AES_BLOCK_SIZE_IN_WORD;

        if(inputLen)
        {
            tmp = (uint16_t)pktRead16bits_m(inst->iv,SALLD_16LSB_OFFSET_IN_BYTE);
            tmp++;
            pktWrite16bits_m(inst->iv, SALLD_16LSB_OFFSET_IN_BYTE, tmp);
        }
        else
        {
            if(!inst->num)
            {
                tmp = (uint16_t)pktRead16bits_m(inst->iv,SALLD_16LSB_OFFSET_IN_BYTE);
                tmp++;
                pktWrite16bits_m(inst->iv, SALLD_16LSB_OFFSET_IN_BYTE, tmp);
            }
        }
    }

    numBlocks = inputLen >> 4;
    for(i = numBlocks; i > 0; i--)
    {
        aesEncrypt((tulong *)(inst->key), inst->iv, inst->block, inst->nr);
        for(j = 0; j < SALLD_AES_BLOCK_SIZE_IN_WORD; j++)
        {
            outpkt[j] = inpkt[j] ^ inst->block[j];
        }
        tmp = (uint16_t)pktRead16bits_m(inst->iv,SALLD_16LSB_OFFSET_IN_BYTE);
        tmp++;
        pktWrite16bits_m(inst->iv, SALLD_16LSB_OFFSET_IN_BYTE, tmp);
        inpkt += SALLD_AES_BLOCK_SIZE_IN_WORD;
        outpkt += SALLD_AES_BLOCK_SIZE_IN_WORD;
    }

    resid_word_len = inputLen - SALLD_AES_BLOCK_SIZE_IN_BYTE * numBlocks;
    if(resid_word_len > 0)
    {
        aesEncrypt((tulong *)inst->key, inst->iv, inst->block, inst->nr);
        for(j = 0; j < resid_word_len; j++)
        {
            outpkt[j] = inpkt[j] ^ inst->block[j];
        }
        inst->num = resid_word_len;
    }
    inst->index++;
	return 0;
}


/* nothing past this point */
