/******************************************************************************
 * FILE PURPOSE:  HMAC-SHA1 Message Authentication Algorithm
 ******************************************************************************
 * FILE NAME:   salldhmac_sha1.c  
 *
 * DESCRIPTION: Secure Hash Algorithm-HMAC as per RFC2104
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
#include <ti/drv/sa/salld.h>
#include "src/salldport.h"
#include "src/salldloc.h"
#include "salldhmac_sha1.h"

uint32_t MacLastWordMask[3] = {0xff000000, 0xffff0000, 0xffffff00};

uint32_t MacPaddingWord[4] = {
    0x80000000,
    0x00800000,
    0x00008000,
    0x00000080
};

/******************************************************************************
 * FUNCTION PURPOSE: Output Intermediate Hash Value
 ******************************************************************************
 * DESCRIPTION: Output SHA1 Intermendiate Hash Value into byte array
 *
 *    void salld_sha1_output(
 *      salldSha1Inst_t*  pInst     - Pointer to Sha1 instance
 *      tword*            hash)     - hash output
 *
 *****************************************************************************/
static inline void salld_sha1_output(sha1Inst_t* pInst, tword *hash)
{
    pktWrite32bits_m(hash, 0,  pInst->h0);
    pktWrite32bits_m(hash, 4,  pInst->h1);
    pktWrite32bits_m(hash, 8,  pInst->h2);
    pktWrite32bits_m(hash, 12, pInst->h3);
    pktWrite32bits_m(hash, 16, pInst->h4);
}


/******************************************************************************
 * FUNCTION PURPOSE: HMAC-SHA1 Algorithm Implementation
 ******************************************************************************
 * DESCRIPTION: Generates HMAC-SHA1 Authentication Tag and compares it with
 *
 *    int16_t salld_hmac_sha1(        - 0: success; 1: error.
 *      uint16_t           *key      - MAC key pointer
 *      int16_t            keyLen    - in bytes
 *      tword            *tag,     - in/out authentication tag pointer
 *      int16_t            tagLen,   - in/out tag length in bytes
 *      salldAesDesc_t 		*desc, 	- Descriptor containing source packet, possibly in segments
 *		tword            *pkt,     - source packet twords
 *      int16_t            pktLen,   - packet size in bytes
 *      uint16_t           options)  - SALLD_MAC_GENERATION/SALLD_MAC_AUTHENTICATION
 *
 * Note: assumption is that keyLen will be even number always
 *****************************************************************************/
int16_t salld_hmac_sha1(uint16_t *key, int16_t keyLen, tword *tag, int16_t tagLen, 
						salldAesDesc_t *desc, uint16_t options, tword *pad)
{
  int16_t  i;
  uint16_t retval=0;
  sha1Inst_t sha1Inst;
  tword  mac[SALLD_HMAC_SHA1_DIGEST_LEN_IN_WORD];
  uint8_t k_ipad[SALLD_HMAC_PAD_LEN_IN_BYTE];
  uint8_t k_opad[SALLD_HMAC_PAD_LEN_IN_BYTE];
  tword  temp_res[SALLD_HMAC_SHA1_DIGEST_LEN_IN_WORD];
  uint8_t *key1 = (uint8_t *)key;

  if(tagLen <= 0)  /* length in bytes */
  {
    return(0); /* don't do anything */
  }
  
  /* Do HMAC-SHA1 computation */

  /* assumption is that keyLen will be even number always */
  /* set up key xor ipad, opad */
  for(i = 0; i < keyLen; i++) 
  { 
    k_ipad[i] = key1[i] ^ SALLD_HMAC_IPAD_PATTERN;
    k_opad[i] = key1[i] ^ SALLD_HMAC_OPAD_PATTERN;
  }
  /* Instead of XOR with zero */
  for( ; i < SALLD_HMAC_PAD_LEN_IN_BYTE; i++) 
  {
	k_ipad[i] = SALLD_HMAC_IPAD_PATTERN;      
	k_opad[i] = SALLD_HMAC_OPAD_PATTERN;
  }

  /* do sha1 on  K_ipad + text */

  /*Init the SHA1 state for 1st pass */
  salld_sha1Init(&sha1Inst, NULL);
  /* start with inner pad k_ipad */
  salld_sha1Update(&sha1Inst, (tword *)k_ipad, SALLD_HMAC_PAD_LEN_IN_BYTE);
  /* then text of datagram */
  for(i = 0 ; i < desc->nSegments; i++)
  {
	salld_sha1Update(&sha1Inst, desc->segments[i], (uint32_t)desc->inputLen[i]);
  }
  /* finish up 1st pass */
  salld_sha1Final(&sha1Inst, temp_res);

  /* do sha1 on K_opad +  (K^ipad+text) */

  /*Init the SHA1 state for 2nd pass */
  salld_sha1Init(&sha1Inst, NULL);
  /* start with outer pad k_opad */
  salld_sha1Update(&sha1Inst, (tword *)k_opad, SALLD_HMAC_PAD_LEN_IN_BYTE );
  /* then result of 1st hash */
  salld_sha1Update(&sha1Inst, temp_res, SALLD_HMAC_SHA1_DIGEST_LEN_IN_BYTE );
  /* Finish up 2nd pass */
  salld_sha1Final(&sha1Inst, mac);

  /* compare the generated MAC with previous tag in Rx case */
  if (options == SALLD_MAC_AUTHENTICATION)
  {
#if (SALLD_SIZE_OF_WORD_IN_BYTE == 2)
    for (i = oddFlag, j = 0; j < tagLen; i ++, j ++) {    
      ByteT = (i & 0x001 ? tag[i/SALLD_SIZE_OF_WORD_IN_BYTE] & 0x00ff : tag[i/SALLD_SIZE_OF_WORD_IN_BYTE] >> 8);
      ByteM = (j & 0x001 ? mac[j/SALLD_SIZE_OF_WORD_IN_BYTE] & 0x00ff : mac[j/SALLD_SIZE_OF_WORD_IN_BYTE] >> 8);
      if(ByteT != ByteM)
#else
    for (i = 0; i< (tagLen)/SALLD_SIZE_OF_WORD_IN_BYTE; i++)
    {
      if (tag[i] != mac[i])
#endif
      {
        retval = 1;
      }
    }
  }
  else if (options == SALLD_MAC_GENERATION)
  {
#if (SALLD_SIZE_OF_WORD_IN_BYTE == 2)
    for (i = oddFlag, j = 0; j < tagLen; i ++, j ++) {
      ByteM = (j & 0x001 ? mac[j/SALLD_SIZE_OF_WORD_IN_BYTE] & 0x00ff : mac[j/SALLD_SIZE_OF_WORD_IN_BYTE] >> 8);
      if(i & 0x001) {
        tag[i/SALLD_SIZE_OF_WORD_IN_BYTE] &= 0xff00;
        tag[i/SALLD_SIZE_OF_WORD_IN_BYTE] |= ByteM;
      } else {
        tag[i/SALLD_SIZE_OF_WORD_IN_BYTE] &= 0x00ff;
        tag[i/SALLD_SIZE_OF_WORD_IN_BYTE] |= (ByteM << 8);
      }
    }
#else
    for (i = 0; i< (tagLen)/SALLD_SIZE_OF_WORD_IN_BYTE; i++)
    {
      tag[i] = mac[i];
    }
#endif
  }
  return(retval);
}/* salld_hmac_sha1 */

/******************************************************************************
 * FUNCTION PURPOSE: HMAC-SHA1 Intermediate Hash Generation
 ******************************************************************************
 * DESCRIPTION: Generates HMAC-SHA1 Intermediate Hashs
 *
 *    nt16 salld_hmac_sha1_get_pad(         - 0: success; 1: error.
 *      uint16_t           *key       - MAC key pointer
 *      int16_t            keyLen    - in bytes
 *      uint16_t           *ipad      - ipad itermediate hash,
 *      uint16_t           *opad      - opad itermediate hash) 
 *****************************************************************************/
int16_t salld_hmac_sha1_get_pad(uint16_t *key, uint16_t keyLen, uint16_t *ipad,  uint16_t *opad)
{					
  int16_t  i;
  sha1Inst_t sha1Inst;
  uint8_t k_ipad[SALLD_HMAC_PAD_LEN_IN_BYTE];
  uint8_t k_opad[SALLD_HMAC_PAD_LEN_IN_BYTE];
  uint8_t *key1 = (uint8_t *)key;

  /* Do HMAC-SHA1 computation */

  /* assumption is that keyLen will be even number always */
  /* set up key xor ipad, opad */
  for(i = 0; i < keyLen; i++) 
  { 
    k_ipad[i] = key1[i] ^ SALLD_HMAC_IPAD_PATTERN;
    k_opad[i] = key1[i] ^ SALLD_HMAC_OPAD_PATTERN;
  }
  /* Instead of XOR with zero */
  for( ; i < SALLD_HMAC_PAD_LEN_IN_BYTE; i++) 
  {
	k_ipad[i] = SALLD_HMAC_IPAD_PATTERN;      
	k_opad[i] = SALLD_HMAC_OPAD_PATTERN;
  }

  /* do sha1 on  K_ipad */

  /*Init the SHA1 state for 1st pass */
  salld_sha1Init(&sha1Inst, NULL);
  /* start with inner pad k_ipad */
  salld_sha1Update(&sha1Inst, (tword *)k_ipad, SALLD_HMAC_PAD_LEN_IN_BYTE);
  /* Output the intermediate hash */
  salld_sha1_output(&sha1Inst, (tword *)ipad);

  /* now do sha1 on K_opad  */
  /*Init the SHA1 state for 2nd pass */
  salld_sha1Init(&sha1Inst, NULL);
  /* start with outer pad k_opad */
  salld_sha1Update(&sha1Inst, (tword *)k_opad, SALLD_HMAC_PAD_LEN_IN_BYTE );
  /* Output the intermediate hash */
  salld_sha1_output(&sha1Inst, (tword *)opad);

  return(0);
}

/* nothing past this point */
