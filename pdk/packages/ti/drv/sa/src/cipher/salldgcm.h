#ifndef _SALLDGCM_H
#define _SALLDGCM_H
/******************************************************************************
 * FILE PURPOSE:  Macros and definitions for GCM (Galois Counter Mode) Algorithms
 ******************************************************************************
 * FILE NAME:   salldgcm.h  
 *
 * DESCRIPTION: SALLD GCM related defines
 *
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

#include "salldaes.h"

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
int16_t salld_aes_gcm_get_ghash (tword *key, int16_t keyLen, tword *ghash);

#endif
/* nothing past this point */ 
