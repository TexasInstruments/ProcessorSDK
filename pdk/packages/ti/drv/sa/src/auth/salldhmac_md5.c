/******************************************************************************
 * FILE PURPOSE:  HMAC-MD5 Message Authentication Algorithm
 ******************************************************************************
 * FILE NAME:   salldhmac_md5.c  
 *
 * DESCRIPTION: Secure Hash Algorithm-HMAC as per RFC2104
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
/* RTSC header files */ 

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include "src/salldport.h"
#include "src/salldloc.h"
#include "salldhmac_md5.h"
#include "salldmd5.h"
#include "salldmd5loc.h"

/******************************************************************************
 * FUNCTION PURPOSE: HMAC-MD5 Algorithm Implementation
 ******************************************************************************
 * DESCRIPTION: Generates HMAC-MD5 Authentication Tag and compares it with
 *
 *    int16_t salld_hmac_md5(          - 0: success; 1: error.
 *      uint16_t           *key       - MAC key pointer
 *      int16_t            keyLen     - in bytes
 *      tword            *tag,      - in/out authentication tag pointer
 *      int16_t            tagLen,    - in/out tag length in bytes
 *      salldAesDesc_t 		*desc, 	- Descriptor containing source packet, possibly in segments
 *		tword            *pkt,      - source packet twords
 *      int16_t            pktLen,    - packet size in bytes
 *      uint16_t           options)   - SALLD_MAC_GENERATION/SALLD_MAC_AUTHENTICATION
 *
 * Note: assumption is that keyLen will be even number always
 *****************************************************************************/
int16_t salld_hmac_md5(uint16_t *key, int16_t keyLen, tword *tag, int16_t tagLen, 
						salldAesDesc_t *desc, uint16_t options, tword *pad)
{
  int16_t  i;
  uint16_t retval=0;
  salldMd5Inst_t md5Inst;
  tword  mac[SALLD_HMAC_MD5_DIGEST_LEN_IN_WORD] = { 0 };
  uint8_t k_ipad[SALLD_HMAC_PAD_LEN_IN_BYTE];
  uint8_t k_opad[SALLD_HMAC_PAD_LEN_IN_BYTE];
  tword  temp_res[SALLD_HMAC_MD5_DIGEST_LEN_IN_WORD];
  uint8_t *key1 = (uint8_t *)key;

  if(tagLen <= 0)  /* length in bytes */
  {
      return(0); /* don't do anything */
  }
  
  /* Do HMAC-MD5 computation */

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

  /* do md5 on  K_ipad + text */

  /*Init the MD5 state for 1st pass */
  salld_md5_init(&md5Inst);
  /* start with inner pad k_ipad */
  salld_md5_update(&md5Inst, (tword*)k_ipad, SALLD_HMAC_PAD_LEN_IN_BYTE);
  /* then text of datagram */
  for(i = 0 ; i < desc->nSegments; i++)
  {
	salld_md5_update(&md5Inst, desc->segments[i], (uint32_t)desc->inputLen[i]);
  }
  /* finish up 1st pass */
  salld_md5_final(&md5Inst, temp_res);

  /* do md5 on K_opad +  (K^ipad+text) */

  /*Init the MD5 state for 2nd pass */
  salld_md5_init(&md5Inst);
  /* start with outer pad k_opad */
  salld_md5_update(&md5Inst, (tword*) k_opad, SALLD_HMAC_PAD_LEN_IN_BYTE );
  /* then result of 1st hash */
  salld_md5_update(&md5Inst, temp_res, SALLD_HMAC_MD5_DIGEST_LEN_IN_BYTE );
  /* Finish up 2nd pass */
  salld_md5_final(&md5Inst, mac);

  /* compare the generated MAC with previous tag in Rx case */
  if (options == SALLD_MAC_AUTHENTICATION)
  {
    for (i = 0; i< (tagLen)/SALLD_SIZE_OF_WORD_IN_BYTE; i++)
    {
      if (tag[i] != mac[i])
      {
        retval = 1;
      }
    }
  }
  else if (options == SALLD_MAC_GENERATION)
  {
    for (i = 0; i< (tagLen)/SALLD_SIZE_OF_WORD_IN_BYTE; i++)
    {
      tag[i] = mac[i];
    }
  }

  return(retval);
}/* salld_hmac_md5 */

/******************************************************************************
 * FUNCTION PURPOSE: HMAC-MD5 Intermediate Hash Generation
 ******************************************************************************
 * DESCRIPTION: Generates HMAC-MD5 Intermediate Hashs
 *
 *    nt16 salld_hmac_md5_get_pad(     - 0: success; 1: error.
 *      uint16_t           *key       - MAC key pointer
 *      int16_t            keyLen     - in bytes
 *      uint16_t           *ipad      - ipad itermediate hash,
 *      uint16_t           *opad      - opad itermediate hash) 
 *****************************************************************************/
int16_t salld_hmac_md5_get_pad(uint16_t *key, uint16_t keyLen, uint16_t *ipad,  uint16_t *opad)
{					
  int16_t  i;
  salldMd5Inst_t md5Inst;
  uint8_t k_ipad[SALLD_HMAC_PAD_LEN_IN_BYTE];
  uint8_t k_opad[SALLD_HMAC_PAD_LEN_IN_BYTE];
  uint8_t *key1 = (uint8_t *)key;

  /* Do HMAC-MD5 computation */

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

  /* do md5 on  K_ipad */

  /*Init the MD5 state for 1st pass */
  salld_md5_init(&md5Inst);
  /* start with inner pad k_ipad */
  salld_md5_update(&md5Inst, (tword *)k_ipad, SALLD_HMAC_PAD_LEN_IN_BYTE);
  /* Output the intermediate hash */
  salld_md5_output(&md5Inst, (tword *)ipad);

  /* now do md5 on K_opad  */
  /*Init the MD5 state for 2nd pass */
  salld_md5_init(&md5Inst);
  /* start with outer pad k_opad */
  salld_md5_update(&md5Inst, (tword *)k_opad, SALLD_HMAC_PAD_LEN_IN_BYTE );
  /* Output the intermediate hash */
  salld_md5_output(&md5Inst, (tword *)opad);

  return(0);
}


/* nothing past this point */
