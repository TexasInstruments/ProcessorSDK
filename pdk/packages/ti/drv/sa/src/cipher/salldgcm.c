/******************************************************************************
 * FILE PURPOSE:  GCM (Galois Counter Mode) Algorithms and Itilities
 ******************************************************************************
 * FILE NAME:   salldgcm.c  
 *
 * DESCRIPTION: GCM implementation 
 *
 * (C) Copyright 2009, Texas Instruments, Inc.
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

/* Local header files */
#include "salldgcm.h"

/******************************************************************************
 * FUNCTION PURPOSE: Derive GCM Hash
 ******************************************************************************
 * DESCRIPTION: Derive the GHASH to be used at the GCM algorithm
 *
 *    int16_t salld_aes_gcm_get_ghash(
 *              tword   *key,     Input key
 *              int16_t   keyLen,   Key size in bits
 *              tword   *ghash)   Galois Hash
 ******************************************************************************/
int16_t salld_aes_gcm_get_ghash (tword *key, int16_t keyLen, tword *ghash)
{

    tword temp[SALLD_AES_BLOCK_SIZE_IN_WORD];
    tulong roundkey[SALLD_MAX_ROUND_KEY_SIZE_IN_TULONG];
    int16_t nr;
  
    /* Derive the rondkey to be used in AES encryption */
    nr = aesKeyExpandEnc(roundkey, key, keyLen);
    
    /* Generate Galois Hash */
    memset(temp, 0, SALLD_AES_BLOCK_SIZE_IN_WORD);
    aesEncrypt(roundkey, temp, ghash, nr);
    
    return (0);
}

/* nothing past this point */
