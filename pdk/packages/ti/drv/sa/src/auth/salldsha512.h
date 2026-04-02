#ifndef SALLDSHA512_H
#define SALLDSHA512_H
/******************************************************************************
 * FILE PURPOSE:  SHA512 Message Authentication Algorithm Defines
 ******************************************************************************
 * FILE NAME:   sha512.h
 *
 * DESCRIPTION: Secure Hash Algorithm1-HMAC as per NIST FIPS PUB 180-1
 *
 *              
 *              Note: It runs on MIPS/ARM or DSP
 *              So maintain this support while you modify this code.
 *
 * (C) Copyright 2017, Texas Instruments, Inc.
 *****************************************************************************/
#define SALLD_SHA512_BLOCK_SIZE_IN_BYTE          128   /* Bytes */
#define SALLD_SHA512_LBLOCK                       16   /* 16*64 */
#define SALLD_SHA512_BLOCK                        16   /* Number of 64 bit words */
#define SALLD_SHA512_LAST_BLOCK_OFFSET_IN_BYTE   112   /* last two 64 bit words */
#define SALLD_SHA512_CBLOCK    (SHA512_LBLOCK*4)
#define SALLD_SHA384_DIGEST_LENGTH    48
#define SALLD_SHA512_DIGEST_LENGTH    64


/* Structure used by SHA512 algorithm */
typedef struct salldSha512Inst_s
{
    uint64_t h[8];                   /* H Buffers */
    uint64_t Nl,Nh;                  
    uint64_t data[SALLD_SHA512_LBLOCK];    /* 64 bit words in a BLOCK */
    uint16_t num;
    uint16_t md_len;
} salldSha512Inst_t;

void     salld_sha384_init(salldSha512Inst_t *c);
void     salld_sha384_update(salldSha512Inst_t *c, uint8_t *data, int16_t len);
void     salld_sha512_init(salldSha512Inst_t *c);
void     salld_sha512_update(salldSha512Inst_t *c, uint8_t *data, int16_t len);

#endif
