#ifndef _SALLDCMAC_H
#define _SALLDCMAC_H
/******************************************************************************
 * FILE PURPOSE:  Macros and definitions for SALLD CMAC encryption
 ******************************************************************************
 * FILE NAME:   salldcmac.h  
 *
 * DESCRIPTION: SALLD CMAC related defines
 *
 *
 * (C) Copyright 2009, Texas Instrumnents, Inc.
 *****************************************************************************/

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
int16_t salld_aes_cmac (uint32_t *key, int16_t nr, tword *inpkt, int16_t inputLen, tword *tag);
                     
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
int16_t salld_aes_cmac_get_keys (tword *key, int16_t keyLen, tword *k1, tword *k2);
                     


#endif
/* nothing past this point */ 
