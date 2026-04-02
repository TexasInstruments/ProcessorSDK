#ifndef _SALLDXCBC_H
#define _SALLDXCBC_H
/******************************************************************************
 * FILE PURPOSE:  Macros and definitions for SALLD AES XCBC operation
 ******************************************************************************
 * FILE NAME:   salldxcbc.h  
 *
 * DESCRIPTION: SALLD AES XCBC related defines
 *
 *
 * (C) Copyright 2009, Texas Instrumnents, Inc.
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

/******************************************************************************
 * FUNCTION PURPOSE: Derive AES XCBC MAC Mode SubKey K1, K2, K3
 ******************************************************************************
 * DESCRIPTION: Derive sub-key K1, k2 and K3 used in the AES XCBC MAC mode
 *
 *    int16_t salld_aes_xcbc_get_subkey(
 *              tword   *key,     Input key
 *              int16_t keyLen,   Key size in bits
 *              tword   keyIndex  Key Index:1, 2 or 3 
 *              tword   *subKey)  Derived key array
 ******************************************************************************/
int16_t salld_aes_xcbc_get_subkey (tword *key, int16_t keyLen, tword keyIndex, tword *subKey);


#endif
/* nothing past this point */ 
