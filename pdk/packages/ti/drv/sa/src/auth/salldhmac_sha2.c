/******************************************************************************
 * FILE PURPOSE:  HMAC-SHA2 Message Authentication Algorithm
 ******************************************************************************
 * FILE NAME:   salldhmac_sha2.c  
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
/* RTSC header files */ 

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include "src/salldport.h"
#include "src/salldloc.h"
#include "salldhmac_sha2.h"
#include "salldsha2.h"
#include "salldsha2loc.h"


/******************************************************************************
 * FUNCTION PURPOSE: HMAC-SHA2 Algorithm Implementation
 ******************************************************************************
 * DESCRIPTION: Generates HMAC-SHA2 Authentication Tag and compares it with
 *
 *    int16_t salld_hmac_sha2(        - 0: success; 1: error.
 *      uint16_t           *key      - MAC key pointer
 *      int16_t            keyLen    - in bytes
 *      tword            *tag,     - in/out authentication tag pointer
 *      int16_t            tagLen,   - in/out tag length in bytes
 *      tword            *pkt,     - source packet twords
 *      int16_t            pktLen,   - packet size in bytes
 *      uint16_t           options)  - SALLD_MAC_GENERATION/SALLD_MAC_AUTHENTICATION
 *
 * Note: assumption is that keyLen will be even number always
 *****************************************************************************/
int16_t salld_hmac_sha2(uint16_t *key, int16_t keyLen, tword *tag, int16_t tagLen, tword *pkt, 
                     int16_t pktLen, uint16_t options, tword *pad)
{
  return(0);
}/* salld_hmac_sha2 */

/******************************************************************************
 * FUNCTION PURPOSE: HMAC-SHA224 Intermediate Hash Generation
 ******************************************************************************
 * DESCRIPTION: Generates HMAC-SHA224 Intermediate Hashs
 *
 *    nt16 salld_hmac_sha224_get_pad(  - 0: success; 1: error.
 *      uint16_t           *key       - MAC key pointer
 *      int16_t            keyLen     - in bytes
 *      uint16_t           *ipad      - ipad itermediate hash,
 *      uint16_t           *opad      - opad itermediate hash) 
 *****************************************************************************/
int16_t salld_hmac_sha224_get_pad(uint16_t *key, uint16_t keyLen, uint16_t *ipad,  uint16_t *opad)
{					
  int16_t  i;
  salldSha2Inst_t sha2Inst;
  uint8_t k_ipad[SALLD_HMAC_PAD_LEN_IN_BYTE];
  uint8_t k_opad[SALLD_HMAC_PAD_LEN_IN_BYTE];
  uint8_t *key1 = (uint8_t *)key;

  /* Do HMAC-SHA2 computation */

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
  salld_sha224_init(&sha2Inst);
  /* start with inner pad k_ipad */
  salld_sha224_update(&sha2Inst, (tword *)k_ipad, SALLD_HMAC_PAD_LEN_IN_BYTE);
  /* Output the intermediate hash */
  salld_sha2_output(&sha2Inst, (tword *)ipad);

  /* now do sha1 on K_opad  */
  /*Init the SHA1 state for 2nd pass */
  salld_sha224_init(&sha2Inst);
  /* start with outer pad k_opad */
  salld_sha224_update(&sha2Inst, (tword *)k_opad, SALLD_HMAC_PAD_LEN_IN_BYTE);
  /* Output the intermediate hash */
  salld_sha2_output(&sha2Inst, (tword *)opad);

  return(0);
}

/******************************************************************************
 * FUNCTION PURPOSE: HMAC-SHA256 Intermediate Hash Generation
 ******************************************************************************
 * DESCRIPTION: Generates HMAC-SHA256 Intermediate Hashs
 *
 *    nt16 salld_hmac_sha256_get_pad(  - 0: success; 1: error.
 *      uint16_t           *key       - MAC key pointer
 *      int16_t            keyLen     - in bytes
 *      uint16_t           *ipad      - ipad itermediate hash,
 *      uint16_t           *opad      - opad itermediate hash) 
 *****************************************************************************/
int16_t salld_hmac_sha256_get_pad(uint16_t *key, uint16_t keyLen, uint16_t *ipad,  uint16_t *opad)
{					
  int16_t  i;
  salldSha2Inst_t sha2Inst;
  uint8_t k_ipad[SALLD_HMAC_PAD_LEN_IN_BYTE];
  uint8_t k_opad[SALLD_HMAC_PAD_LEN_IN_BYTE];
  uint8_t *key1 = (uint8_t *)key;

  /* Do HMAC-SHA2 computation */

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
  salld_sha256_init(&sha2Inst);
  /* start with inner pad k_ipad */
  salld_sha256_update(&sha2Inst, (tword *)k_ipad, SALLD_HMAC_PAD_LEN_IN_BYTE);
  /* Output the intermediate hash */
  salld_sha2_output(&sha2Inst, (tword *)ipad);

  /* now do sha1 on K_opad  */
  /*Init the SHA1 state for 2nd pass */
  salld_sha256_init(&sha2Inst);
  /* start with outer pad k_opad */
  salld_sha256_update(&sha2Inst, (tword *)k_opad, SALLD_HMAC_PAD_LEN_IN_BYTE);
  /* Output the intermediate hash */
  salld_sha2_output(&sha2Inst, (tword *)opad);

  return(0);
}


/* nothing past this point */
