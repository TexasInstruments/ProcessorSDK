/******************************************************************************
 * FILE PURPOSE:  CMAC Message Authentication Algorithm
 ******************************************************************************
 * FILE NAME:   salldcmac.c  
 *
 * DESCRIPTION: CMAC as per RFC 
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
/* Standard header files */
#include <string.h> 

/* SALLD header files */
#include "src/salldloc.h"
#include "src/cipher/salldaes.h"
/* Local header files */
#include "salldcmac.h"
 

/******************************************************************************
 * FUNCTION PURPOSE: Shift 128 bit to the left
 ******************************************************************************
 * DESCRIPTION: Shift 128 bit to the left
 ******************************************************************************/
static inline void shl128(tword *in, tword *out)
{
    uint16_t index;
            
#if (SALLD_SIZE_OF_WORD_IN_BYTE == 2)
    tword temp;
    for (index = 0; index < 15; index++)
    {
        temp = (pktRead8bits_m(in, index) << 1) | (pktRead8bits_m(in, index+1) >> 7)
        pktWrite8bits_m(out, index, temp);    
    }
    temp = pktRead8bits_m(in, 15) << 1;
    pktWrite8bits_m(out, 15, temp);

#else
    for (index = 0; index < 15; index++)
    {
        out[index] = (in[index] << 1) | (in[index+1] >> 7);    
    }
    out[15] = in[15] << 1;
#endif

}

/******************************************************************************
 * FUNCTION PURPOSE: XOR 128 bit 
 ******************************************************************************
 * DESCRIPTION: XOR 128 bit 
 ******************************************************************************/
static inline void xor128(tword *a, tword *b, tword *out)
{
    int index;

    for (index=0; index<SALLD_AES_BLOCK_SIZE_IN_WORD; index++) 
    {
      /* XOR Plaintext and Cipher-Stream */
      out[index] = a[index] ^ b[index];
    }
}

/******************************************************************************
 * FUNCTION PURPOSE: AES CMAC Mode operation
 ******************************************************************************
 * DESCRIPTION: Authenticate the input packets
 *
 *    int16_t salld_aes_cmac(
 *              uint32_t *key,      Expanded Round Key Array
 *              int16_t   nr,       NR: number of rounds
 *              tword   *inpkt,   Input packet to do encryption/decryption
 *              int16_t   inputLen, Payload length in bytes
 *              tword   *tag)     Authentication Tag
 ******************************************************************************/
int16_t salld_aes_cmac (uint32_t *key, int16_t nr, tword *inpkt, int16_t inputLen, tword *tag)
{

    tword l[SALLD_AES_BLOCK_SIZE_IN_WORD];
    tword k1[SALLD_AES_BLOCK_SIZE_IN_WORD];
    tword k2[SALLD_AES_BLOCK_SIZE_IN_WORD];
    tword c[SALLD_AES_BLOCK_SIZE_IN_WORD];
    tword temp[SALLD_AES_BLOCK_SIZE_IN_WORD];
    tword mn[SALLD_AES_BLOCK_SIZE_IN_WORD];

    int16_t numBlocks;
    int16_t resid_tword_len=0;
    tword temp1;
    int i;

	// Compute subkeys K1 and K2 
    memset(temp, 0, SALLD_AES_BLOCK_SIZE_IN_WORD);
    aesEncrypt((tulong *)key, temp, l, nr);
    temp1 = pktRead8bits_m(l, 0);

	if ((temp1 & 0x80) == 0x80) { // if MSB of L is set 
		/* shl128(l,lshl); */
		/* xor128(lshl,r,k1); */
        shl128(l,k1);
        temp1 = pktRead8bits_m(k1, 15) ^ 0x87;
        pktWrite8bits_m(k1, 15, temp1);
	}
	else	{
		shl128(l,k1);
	}

    temp1 = pktRead8bits_m(k1, 0);	
	if ((temp1 & 0x80) == 0x80) { // if MSB of K1 is set 
		shl128(k1,k2);
        temp1 = pktRead8bits_m(k2, 15) ^ 0x87;
        pktWrite8bits_m(k2, 15, temp1);
	}
	else	{
		shl128(k1,k2);
	}
    
	// Compute the MAC 

    if (inputLen == 0)
    {
        numBlocks = 1;
		/* clear temp */
        memset(temp, 0, SALLD_AES_BLOCK_SIZE_IN_WORD);
        pktWrite8bits_m(temp, 0, 0x80);
		xor128(temp, k2, temp);
        aesEncrypt((tulong *)key, temp, tag, nr);
        return 0;
    }
    else
    {
	    numBlocks = inputLen >> 4;
        resid_tword_len = 
                (inputLen - SALLD_AES_BLOCK_SIZE_IN_BYTE * numBlocks  
                + (SALLD_SIZE_OF_WORD_IN_BYTE - 1)) / SALLD_SIZE_OF_WORD_IN_BYTE;
        
	    if (resid_tword_len != 0) { numBlocks++; }
        memset(c, 0, SALLD_AES_BLOCK_SIZE_IN_WORD);  // Set c0 to all zeroes  
    }
    
    /* Process the first n-1 block */
	for (i = 0; i< numBlocks-1; i++)	{
		xor128(c,inpkt,temp);
        aesEncrypt((tulong *)key, temp, c, nr);
        inpkt += SALLD_AES_BLOCK_SIZE_IN_WORD;
	}

	/* Final block */
	if (resid_tword_len == 0)	{
		xor128(inpkt, k1, mn);
		xor128(c, mn, temp);
        aesEncrypt((tulong *)key, temp, tag, nr);
	}
	else	
    {
		/* clear temp */
        memset(temp, 0, SALLD_AES_BLOCK_SIZE_IN_WORD);
        
		/* copy last fragment into temp */
		for (i=0; i<resid_tword_len; i++) {
			temp[i] = inpkt[i];	
		}
		/* set the leftmost bit of the padding */
        pktWrite8bits_m(temp, resid_tword_len, 0x80);

		xor128(temp, k2, mn);
		xor128(mn, c, temp);
        aesEncrypt((tulong *)key, temp, tag, nr);
	}
    
    return 0;
    
}

/******************************************************************************
 * FUNCTION PURPOSE: Derive AES CMAC Mode K1 and K2
 ******************************************************************************
 * DESCRIPTION: Derive k1 and K2 used in the AES CMAC mode
 *
 *    int16_t salld_aes_cmac_get_keys(
 *              tword   *key,     Input key
 *              int16_t   keyLen,   Key size in bits
 *              tword   *k1,      k1 array
 *              tword   *k2)      k2 array
 ******************************************************************************/
int16_t salld_aes_cmac_get_keys (tword *key, int16_t keyLen, tword *k1, tword *k2)
{

    tword l[SALLD_AES_BLOCK_SIZE_IN_WORD];
    tword temp[SALLD_AES_BLOCK_SIZE_IN_WORD];
    tulong roundkey[SALLD_MAX_ROUND_KEY_SIZE_IN_TULONG];

    int16_t nr;
    
    tword temp1;

	// Compute subkeys K1 and K2 
    nr = aesKeyExpandEnc(roundkey, key, keyLen);
    memset(temp, 0, SALLD_AES_BLOCK_SIZE_IN_WORD);
    aesEncrypt(roundkey, temp, l, nr);
    temp1 = pktRead8bits_m(l, 0);

	if ((temp1 & 0x80) == 0x80) { // if MSB of L is set 
        shl128(l,k1);
        temp1 = pktRead8bits_m(k1, 15) ^ 0x87;
        pktWrite8bits_m(k1, 15, temp1);
	}
	else	{
		shl128(l,k1);
	}

    temp1 = pktRead8bits_m(k1, 0);	
	if ((temp1 & 0x80) == 0x80) { // if MSB of K1 is set 
		shl128(k1,k2);
        temp1 = pktRead8bits_m(k2, 15) ^ 0x87;
        pktWrite8bits_m(k2, 15, temp1);
	}
	else	{
		shl128(k1,k2);
	}
    
    return (0);
}
