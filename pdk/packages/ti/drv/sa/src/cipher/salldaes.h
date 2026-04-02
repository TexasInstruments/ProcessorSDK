#ifndef _SALLDAES_H
#define _SALLDAES_H
/******************************************************************************
 * FILE PURPOSE:  Macros and definitions private to AES encryption/Decryption
 ******************************************************************************
 * FILE NAME:   salldaes.h  
 *
 * DESCRIPTION: AES Encryption/Decryption related defines
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

#include "src/salldport.h"
#include "src/salldloc.h"
#define SALLD_NO_KEY_ERROR     0
#define SALLD_BAD_KEY_MAT      1
#define SALLD_BAD_CIPHER_STATE 2

#define SALLD_ENCRYPT_BLOCK_SIZE_INBYTES  16
#define SALLD_DECRYPT_BLOCK_SIZE_INBYTES  16
#define SALLD_ENCRYPT_BLOCK_SIZE_INWORDS  8
#define SALLD_DECRYPT_BLOCK_SIZE_INWORDS  8

#define SALLD_MAX_AES_KEY_SIZE            60

/******************************************************************************
 * FUNCTION PURPOSE: Derive NR from the key size
 ******************************************************************************
 * DESCRIPTION: Derive AES NR from the given key size in bits
 *
 *    int16_t salld_aes_get_nr(
 *              int16_t   keylen),   Key size in bits
 ******************************************************************************/
int16_t salld_aes_get_nr(int16_t keyLen);


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
/*int16_t salld_aes_ctr(uint32_t *key, int16_t keylen, int16_t nr, tword *iv, tword *inpkt,
                   int16_t inputLen, tword *outpkt);*/
				   
int16_t salld_aes_ctr(uint32_t *key, int16_t keylen, int16_t nr, tword *iv,
					salldAesDesc_t *desc);
					
int16_t salld_aes_ctr_update(salldAesInst_t *inst, salldAesDesc_t *desc);

int16_t salld_aes_ctr_single_segment(uint32_t *key, int16_t keylen, int16_t nr, tword *iv, tword *inpkt,
        int16_t inputLen, tword *outpkt);

int16_t salld_aes_ctr_multi_segment(uint32_t *key, int16_t keylen, int16_t nr, tword *iv, salldAesInst_t * inst, salldAesDesc_t *desc);
/******************************************************************************
 * FUNCTION PURPOSE: AES f8 Mode operation
 ******************************************************************************
 * DESCRIPTION: Does inplace encryption/decryption in f8 mode
 *
 *    int8_t salld_aes_f8(
 *              uint32_t *key,     Expanded Round Key Array
 *              int16_t   keylen,   Key size in bits
 *              int16_t   nr,       NR: number of rounds
 *              tword   *iv,      Initialization Vector
 *              tword   *inpkt,   Input packet to do encryption/decryption
 *              int16_t   inputLen, Payload length in bytes
 *              tword   *outpkt)  Output encrypted/decrypted packet
 ******************************************************************************
 *    Algorithm: S(j) = E(key, IV XOR j XOR S(j-1));
 *               OutPkt(j) = S(j) XOR InPkt(j); for encryption/decryption
 *
 *                  IV
 *                   |
 *                   +-----------+-------------+--  ...     ------+
 *                   |           |             |                  |
 *                   |   j=1 -> (*)    j=2 -> (*)   ...  j=L-1 ->(*)
 *                   |           |             |                  |
 *                   |      +-> (*)       +-> (*)   ...      +-> (*)
 *                   |      |    |        |    |             |    |
 *                   v      |    v        |    v             |    v
 *               +------+   | +------+    | +------+         | +------+
 *               |      |   | |      |    | |      |         | |      |
 *    key ------>|  AES |   | |  AES |    | |  AES |         | |  AES |
 *               |      |   | |      |    | |      |         | |      |
 *               +------+   | +------+    | +------+         | +------+
 *                   |      |    |        |    |             |    |
 *                   +------+    +--------+    +--  ...  ----+    |
 *                   |           |             |                  |
 *                   v           v             v                  v
 *                  S(0)        S(1)          S(2)  . . .       S(L-1)
 *
 ******************************************************************************/
int16_t salld_aes_f8(uint32_t *key, int16_t keylen, int16_t nr, tword *iv, 
						salldAesDesc_t *desc);


#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup AES */
/** @{ */

/**
 *  @def aes_128_BIT_ROUND_KEY_SIZE_IN_TULONG
 *  @brief Number of round keys generated from a 128 bit encryption or decryption key
 */
#define aes_128_BIT_ROUND_KEY_SIZE_IN_TULONG (44)
/**
 *  @def aes_192_BIT_ROUND_KEY_SIZE_IN_TULONG
 *  @brief Number of round keys generated from a 192 bit encryption or decryption key
 */
#define aes_192_BIT_ROUND_KEY_SIZE_IN_TULONG (52)
/**
 *  @def aes_256_BIT_ROUND_KEY_SIZE_IN_TULONG
 *  @brief Number of round keys generated from a 256 bit encryption or decryption key
 */
#define aes_256_BIT_ROUND_KEY_SIZE_IN_TULONG (60)

/* ============ aesKeyExpandEnc() ========== */
/**
 *  @brief      Expands the cipher key into the encryption key schedule.  Some
 *                 implementations only support a key length of 128 bits.  Also, for c64x+ 
 *                 applications, it is up to the caller to assure 64-bit alignment for all buffers.<BR><BR>
 *                 WARNING: Proper function operation and return value cannot be assured if 
 *                 'keyBits' is not passed the value 128, 192, or 256.
 *
 *  @param[out]  rk    Pointer to the expanded key: tulong array.
 *
 *  @param[in]  cipherKey      Input AES encryption key: byte array, packed byte for 16 bit
 *                                         tword targets.  Endianness of target must be accounted for when
 *                                         considering the input to this parameter.     
 *
 *  @param[in]  keyBits      The length of the encryption key, 128, 192, or 256 bits. 
 *
 *  @return     Number of encryption rounds, 10, 12, or 14.
 *
 */
tint aesKeyExpandEnc(tulong *rk, tword cipherKey[], tint keyBits);

/* ============ aesEncrypt() ========== */
/**
 *  @brief      AES encryption algorithm.  The encryption takes place in 128 bit blocks.  The
 *                 number of rounds should be derived from the key size:
 *                 (128, 192, 256) ==> (10, 12, 14).  Also, for c64x+ applications, it is up to the
 *                 caller to assure 64-bit alignment for all buffers. <BR><BR>
 *                 WARNING: Proper function operation and cipher text output cannot be assured if 
 *                 'nr' is not passed the value 10, 12, or 14.
 *
 *
 *  @param[in]  rk    Expanded round key: tulong array.
 *
 *  @param[in]  pt      Input plaintext block to be encrypted: byte array, packed byte for 16 bit
 *                             tword targets.  Endianness of target must be accounted for when
 *                             considering the input to this parameter.
 *
 *  @param[out]  ct      Output ciphertext block: byte array, packed byte for 16 bit
 *                                tword targets.
 *
 *  @param[in]  nr      Number of rounds, 10, 12, or 14.
 *
 */
void aesEncrypt(tulong rk[], tword pt[], tword ct[], tint nr);

/* ============ aesKeyExpandDec() ========== */
/**
 *  @brief      Expands the cipher key into the decryption key schedule.  Some
 *                 implementations only support a key length of 128 bits.  Also, for c64x+ 
 *                 applications, it is up to the caller to assure 64-bit alignment for all buffers.<BR><BR>
 *                 WARNING: Proper function operation and return value cannot be assured if 
 *                 'keyBits' is not passed the value 128, 192, or 256.
 *
 *  @param[out]  rk    Pointer to the expanded key: tulong array.
 *
 *  @param[in]  cipherKey      Input AES decryption key: byte array, packed byte for 16 bit
 *                                         tword targets.  Endianness of target must be accounted for when
 *                                         considering the input to this parameter.
 *
 *  @param[in]  keyBits      The length of the decryption key, 128, 192, or 256 bits. 
 *
 *  @return     Number of decryption rounds, 10, 12, or 14.
 *
 */
tint aesKeyExpandDec(tulong *rk, tword cipherKey[], tint keyBits);

/* ============ aesDecrypt() ========== */
/**
 *  @brief      AES decryption algorithm.  The decryption takes place in 128 bit blocks.  The
 *                 number of rounds should be derived from the key size:
 *                 (128, 192, 256) ==> (10, 12, 14).  Also, for c64x+ applications, it is up to the
 *                 caller to assure 64-bit alignment for all buffers. <BR><BR>
 *                 WARNING: Proper function operation and plain text output cannot be assured if 
 *                 'nr' is not passed the value 10, 12, or 14.
 *
 *  @param[in]  rk    Expanded round key: tulong array.
 *
 *  @param[in]  ct      Input ciphertext block to be decrypted: byte array, packed byte for 16 bit
 *                             tword targets.  Endianness of target must be accounted for when
 *                             considering the input to this parameter.
 *
 *  @param[out]  pt      Output plaintext block: byte array, packed byte for 16 bit
 *                                tword targets.
 *
 *  @param[in]  nr      Number of rounds, 10, 12, or 14.
 *
 */
void aesDecrypt(tulong *rk, tword ct[], tword pt[], tint nr);

/* ============ aesInvKey() ========== */
/**
 *  @brief      Derive the inverse key which is used at AES-CBC decryption operation.  
 *              Some implementations only support a key length of 128 bits.  
 *              WARNING: Proper function operation cannot be assured if 
 *              'keyBits' is not passed the value 128, 192, or 256.
 *
 *  @param[out] invKey      Output AES inverse key: byte array, packed byte for 16 bit tword targets.
 *
 *  @param[in]  cipherKey   Input AES encryption key: byte array, packed byte for 16 bit
 *                                         tword targets.  Endianness of target must be accounted for when
 *                                         considering the input to this parameter.     
 *
 *  @param[in]  keyBits     The length of the encryption key, 128, 192, or 256 bits. 
 *
 *  @return     None
 *
 */
void aesInvKey(tword invKey[], tword cipherKey[], tint keyBits);
/** @} */ /** ingroup */


#ifdef __cplusplus
}
#endif /* extern "C" */
#endif
/* nothing past this point */ 
