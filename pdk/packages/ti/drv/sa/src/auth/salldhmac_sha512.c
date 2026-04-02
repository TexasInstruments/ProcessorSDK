/******************************************************************************
 * FILE PURPOSE:  HMAC-SHA512 Message Authentication Algorithm
 ******************************************************************************
 * FILE NAME:   hmac_sha512.c  
 *
 * DESCRIPTION: Secure Hash Algorithm-HMAC as per RFC2104
 *
 * (C) Copyright 2017, Texas Instruments Inc. 
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
#include "salldhmac_sha512.h"
#include "salldsha512.h"
#include "salldsha512loc.h"


/******************************************************************************
 * FUNCTION PURPOSE: HMAC-SHA512 Algorithm Implementation
 ******************************************************************************
 * DESCRIPTION: Generates HMAC-SHA512 Authentication Tag and compares it with
 *
 *    int16_t salld_hmac_sha512(    - 0: success; 1: error.
 *      uint8_t             *key       - MAC key pointer
 *      int16_t            key_len    - in bytes
 *      uint8_t             *tag,      - in/out authentication tag pointer
 *      int16_t            tag_len,   - in/out tag length in bytes
 *      uint8_t             *pkt,      - source packet uint8_ts
 *      int16_t            pkt_len,   - packet size in bytes
 *      uint16_t            options)   - MSU_MAC_GENERATION/MSU_MAC_AUTHENTICATION
 *
 * Note: assumption is that keyLen will be even number always
 *****************************************************************************/
int16_t salld_hmac_sha512(uint16_t *key, int16_t keyLen, tword *tag, int16_t tagLen, tword *pkt, 
                     int16_t pktLen, uint16_t options, tword *pad)
{
  return (0);
} /* salld_hmac_sha512 */

/******************************************************************************
 * FUNCTION PURPOSE: HMAC-SHA384 Intermediate Hash Generation
 ******************************************************************************
 * DESCRIPTION: Generates HMAC-SHA384 Intermediate Hash
 *
 *    int16_t salld_hmac_sha384_get_pad(  - 0: success; 1: error.
 *      uint8_t    *key       - MAC key pointer
 *      uint16_t    key_len    - in bytes
 *      uint16_t   *ipad    - ipad itermediate hash,
 *      uint16_t   *opad    - opad itermediate hash) 
 *****************************************************************************/
int16_t salld_hmac_sha384_get_pad(uint16_t *key, uint16_t key_len, uint16_t *ipad,  uint16_t *opad)
{
  int16_t    i;
  salldSha512Inst_t sha512Inst;
  uint16_t*  pKey = (uint16_t *)key;
  uint16_t   k_ipad[SALLD_MSU_HMAC1024_PAD_LEN_IN_TUINT];
  uint16_t   k_opad[SALLD_MSU_HMAC1024_PAD_LEN_IN_TUINT];

  /* Do HMAC-MD5 computation */

  /* assumption is that key_len will be even number always */
  /* set up key xor ipad, opad */
  for(i = 0; i < key_len/2; i++) 
  { 
      k_ipad[i] = pKey[i] ^ SALLD_MSU_HMAC_IPAD_PATTERN;
      k_opad[i] = pKey[i] ^ SALLD_MSU_HMAC_OPAD_PATTERN;
  }
  /* Instead of XOR with zero */
  for( ; i < SALLD_MSU_HMAC1024_PAD_LEN_IN_TUINT; i++) 
  {
      k_ipad[i] = SALLD_MSU_HMAC_IPAD_PATTERN;      
      k_opad[i] = SALLD_MSU_HMAC_OPAD_PATTERN;
  }

  /* do sha512 on  K_ipad */
  /*Init the SHA512 state for 1st pass */
  salld_sha384_init(&sha512Inst);
  /* start with inner pad k_ipad */
  salld_sha384_update(&sha512Inst, (uint8_t *)k_ipad, SALLD_MSU_HMAC1024_PAD_LEN_IN_BYTE);
  /* Output the intermediate hash */
  salld_sha512_output(&sha512Inst, (tword *)ipad);

  /* now do sha512 on K_opad  */
  /*Init the SHA512 state for 2nd pass */
  salld_sha384_init(&sha512Inst);
  /* start with outer pad k_opad */
  salld_sha384_update(&sha512Inst, (uint8_t *)k_opad, SALLD_MSU_HMAC1024_PAD_LEN_IN_BYTE);
  /* Output the intermediate hash */
  salld_sha512_output(&sha512Inst, (tword *)opad);

  return(0);
}

/******************************************************************************
 * FUNCTION PURPOSE: HMAC-SHA512 Intermediate Hash Generation
 ******************************************************************************
 * DESCRIPTION: Generates HMAC-SHA512 Intermediate Hash
 *
 *    int16_t salld_hmac_sha512_get_pad(  - 0: success; 1: error.
 *      uint8_t             *key       - MAC key pointer
 *      uint16_t             key_len   - in bytes
 *      sha512Inst_t   *ipad      - ipad itermediate hash,
 *      sha512Inst_t   *opad      - opad itermediate hash) 
 *****************************************************************************/
int16_t salld_hmac_sha512_get_pad(uint16_t *key, uint16_t key_len, uint16_t *ipad,  uint16_t *opad)
{
  int16_t  i;
  salldSha512Inst_t sha512Inst;
  uint16_t*  pKey = (uint16_t *)key;
  uint16_t   k_ipad[SALLD_MSU_HMAC1024_PAD_LEN_IN_TUINT];
  uint16_t   k_opad[SALLD_MSU_HMAC1024_PAD_LEN_IN_TUINT];

  /* Do HMAC-MD5 computation */

  /* assumption is that key_len will be even number always */
  /* set up key xor ipad, opad */
  for(i = 0; i < key_len/2; i++) 
  { 
      k_ipad[i] = pKey[i] ^ SALLD_MSU_HMAC_IPAD_PATTERN;
      k_opad[i] = pKey[i] ^ SALLD_MSU_HMAC_OPAD_PATTERN;
  }
  /* Instead of XOR with zero */
  for( ; i < SALLD_MSU_HMAC1024_PAD_LEN_IN_TUINT; i++) 
  {
      k_ipad[i] = SALLD_MSU_HMAC_IPAD_PATTERN;      
      k_opad[i] = SALLD_MSU_HMAC_OPAD_PATTERN;
  }
  
  /* do sha512 on  K_ipad */

  /*Init the SHA512 state for 1st pass */
  salld_sha512_init(&sha512Inst);
  /* start with inner pad k_ipad */
  salld_sha512_update(&sha512Inst, (uint8_t *)k_ipad, SALLD_MSU_HMAC1024_PAD_LEN_IN_BYTE);
  /* Output the intermediate hash */
  salld_sha512_output(&sha512Inst, (tword *)ipad);

  /* now do sha512 on K_opad  */
  /*Init the SHA512 state for 2nd pass */
  salld_sha512_init(&sha512Inst);
  /* start with outer pad k_opad */
  salld_sha512_update(&sha512Inst, (uint8_t *)k_opad, SALLD_MSU_HMAC1024_PAD_LEN_IN_BYTE);
  /* Output the intermediate hash */
  salld_sha512_output(&sha512Inst, (tword *)opad);

  return(0);
}


/* nothing past this point */
