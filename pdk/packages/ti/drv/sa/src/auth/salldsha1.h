/* ========================================================================== */
/**
 *  @file   salldsha1.h
 *
 *  @brief  Header file of SHA1 module
 *
 *  ============================================================================
 *  
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
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
 *  ============================================================================
 */

/** @defgroup SHA1 */

#ifndef _SHA1_H
#define _SHA1_H

#include "src/salldport.h"
#include "src/salldloc.h"

/** @ingroup SHA1 */
/** @{ */

/**
 *  @def sha1_NUM_WORDS_IN_BLOCK
 *  @brief Number of 32 bit words within the SHA1 operating block size of 512 bits
 */
#define sha1_NUM_WORDS_IN_BLOCK	                  (512/TYP_TULONG_SIZE)

#ifdef ti_targets_C55_large
#define _TYPES_C55_LARGE 1
#else
#define _TYPES_C55_LARGE 0
#endif

#ifdef ti_targets_C64P
#define _TYPES_C64P 1
#else
#define _TYPES_C64P 0
#endif

#ifdef ti_targets_elf_C64P
#define _TYPES_ELF_C64P 1
#else
#define _TYPES_ELF_C64P 0
#endif


#ifdef ti_targets_C64P_big_endian
#define _TYPES_C64P_BIG_ENDIAN 1
#else
#define _TYPES_C64P_BIG_ENDIAN 0
#endif

#ifdef ti_targets_elf_C64P_big_endian
#define _TYPES_ELF_C64P_BIG_ENDIAN 1
#else
#define _TYPES_ELF_C64P_BIG_ENDIAN 0
#endif


#ifdef ti_targets_C66
#define _TYPES_C66 1
#else
#define _TYPES_C66 0
#endif

#ifdef ti_targets_elf_C66
#define _TYPES_ELF_C66 1
#else
#define _TYPES_ELF_C66 0
#endif


#ifdef ti_targets_C66_big_endian
#define _TYPES_C66_BIG_ENDIAN 1
#else
#define _TYPES_C66_BIG_ENDIAN 0
#endif

#ifdef ti_targets_elf_C66_big_endian
#define _TYPES_ELF_C66_BIG_ENDIAN 1
#else
#define _TYPES_ELF_C66_BIG_ENDIAN 0
#endif

#ifdef ti_targets_C674
#define _TYPES_C674 1
#else
#define _TYPES_C674 0
#endif

#ifdef ti_targets_elf_C674
#define _TYPES_ELF_C674 1
#else
#define _TYPES_ELF_C674 0
#endif


#ifdef ti_targets_C674_big_endian
#define _TYPES_C674_BIG_ENDIAN 1
#else
#define _TYPES_C674_BIG_ENDIAN 0
#endif

#ifdef ti_targets_elf_C674_big_endian
#define _TYPES_ELF_C674_BIG_ENDIAN 1
#else
#define _TYPES_ELF_C674_BIG_ENDIAN 0
#endif




#ifdef ti_targets_arm_Arm11
#define _TYPES_ARM11 1
#else
#define _TYPES_ARM11 0
#endif


#ifdef ti_targets_arm_Arm11_big_endian
#define _TYPES_ARM11_BIG_ENDIAN 1
#else
#define _TYPES_ARM11_BIG_ENDIAN 0
#endif

#define sha1_BLOCK_SIZE_IN_BYTE           64   /* Bytes */  

#define sha1_INIT_DATA_h0 0x67452301UL
#define sha1_INIT_DATA_h1 0xefcdab89UL
#define sha1_INIT_DATA_h2 0x98badcfeUL
#define sha1_INIT_DATA_h3 0x10325476UL
#define sha1_INIT_DATA_h4 0xc3d2e1f0UL

/**
 *  @brief  Structure used by SHA1 algorithm to compute the message digest
 *
 */
typedef struct sha1Inst_s
{
	tulong h0;                            /**< H buffer 0 - used to compute first
	                                                                      32 bits of message digest. */
	tulong h1;                            /**< H buffer 1 - used to compute second
	                                                                      32 bits of message digest. */
	tulong h2;                            /**< H buffer 2 - used to compute third
	                                                                      32 bits of message digest. */
	tulong h3;                            /**< H buffer 3 - used to compute fourth
	                                                                      32 bits of message digest. */
	tulong h4;                            /**< H buffer 4 - used to compute fifth
	                                                                      32 bits of message digest. */
	tulong Nl;                             /**< Lower 32 bits of the number of bits 
	                                                    in the message to be hashed.  Up to 2^64 bits can 
	                                                    be hashed so two variables are used. */
	tulong Nh;                            /**< Upper 32 bits of the number of bits 
	                                                    in the message to be hashed.  Up to 2^64 bits can 
	                                                    be hashed so two variables are used. */	                                                    
	tulong data[sha1_NUM_WORDS_IN_BLOCK];  /**< The sixteen 32 bit words
	                                                                            of the message block being
	                                                                            operated on. */
	tint   num;                            /**< Holds the number of unhashed bytes to be hashed 
	                                                    by the salld_sha1Final function, or a subsequent call to the
	                                                    salld_sha1Update function, if the message is not a 
	                                                    multiple of 512. */
} sha1Inst_t;

/**
 *  @def sha1_SAVEKPADHASH
 *  @brief  Macro for copying the SHA1 algorithm instance of a completed hash of a block to a
 *             SHA1 instance storage entity.  This macro can be utilized at the HMAC level to
 *             to implement an optimization described in RFC2104 - Section 4.
 *
 */
#define sha1_SAVEKPADHASH(sha1Inst,k_pad_hash) memcpy(k_pad_hash,sha1Inst,sizeof(sha1Inst_t)) 


/* ============ salld_sha1Init() ========== */
/**
 *  @brief      Initializes the sha1Inst algorithm structure.
 *
 *  @param[in, out]  inst     Pointer to an input unitialized algorithm instance.  Outputs a pointer
 *                                     to an initialized algorithm instance.
 *
 *  @param[in]  preInst      Precomputed, algorithm instance pointer.
 *                                     This input parameter allows the SHA1 code to conform to an
 *                                     HMAC level optimization described in RFC2104 - Section 4.
 *                                     Passing a NULL pointer through this parameter will initialize
 *                                     the SHA1 algorithm instance normally.  Passing a precomputed 
 *                                     sha1Inst_t through this parameter will cause the precomputed 
 *                                     SHA1 data to be copied to the currently in use SHA1 algorithm
 *                                     instance.                           
 *
 */
void salld_sha1Init(void *inst, void *preInst);

/* ============ salld_sha1Update() ========== */
/**
 *  @brief      Computes the hash of the input message up to the last block of data
 *                 which is greater than or equal to 512 bits long.  If there is leftover message
 *                 data it is returned through the algorithm instance pointer, as well as the 
 *                 computed message digest up to that point.
 *
 *  @param[in, out]  inst      Pointer to an input initialized algorithm instance.  Outputs a pointer
 *                                      to an algorithm instance containing remaining message data and
 *                                      the message digest computed up to that point.
 *
 *  @param[in]  data      Message data to be hashed.  Packed byte for 16 bit tword targets
 *
 *  @param[in]  len      Length of message to be hashed in bytes.
 *
 */
void salld_sha1Update(void *inst, tword *data, tulong len);

/* ============ salld_sha1Final() ========== */
/**
 *  @brief      Function where the final portion of the message which was less than 512 bits
 *                 is padded in accordance with the FIPS 180-1 standard, and hashed.
 *
 *  @param[in]  inst      Pointer to an input algorithm instance pointer containing remaining 
 *                                message and message digest.  Outputs a pointer to an algorithm
 *                                instance containing a completed message digest.
 *
 *  @param[out]  digest      Pointer to the 160-bit completed message digest.
 *
 */
void salld_sha1Final(void *inst, tword *digest);

/** @} */ /** ingroup */

#define sha1_F_00_19(b,c,d)	((((c) ^ (d)) & (b)) ^ (d)) 
#define sha1_F_20_39(b,c,d)	((b) ^ (c) ^ (d))
#define sha1_F_40_59(b,c,d)	(((b) & (c)) | (((b)|(c)) & (d))) 
#define sha1_F_60_79(b,c,d)	sha1_F_20_39(b,c,d)

#define sha1_K_00_19 0x5a827999UL
#define sha1_K_20_39 0x6ed9eba1UL
#define sha1_K_40_59 0x8f1bbcdcUL
#define sha1_K_60_79 0xca62c1d6UL

#define sha1_ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))

#define sha1_XUPDATE(a,ix,ia,ib,ic,id)	( (a)=(ia^ib^ic^id),	\
					  ix=(a)=sha1_ROTATE((a),1)	\
					)

static void sha1_block(sha1Inst_t *c, const void *p, tint num);

#ifndef sha1_SMALL_FOOTPRINT
/* Use large footprint macros if small footprint build time option is not defined */

#define sha1_BODY_00_15(i,a,b,c,d,e,f,xi) \
	(f)=xi+(e)+sha1_K_00_19+sha1_ROTATE((a),5)+sha1_F_00_19((b),(c),(d)); \
	(b)=sha1_ROTATE((b),30);

#define sha1_BODY_16_19(i,a,b,c,d,e,f,xi,xa,xb,xc,xd) \
	sha1_XUPDATE(f,xi,xa,xb,xc,xd); \
	(f)+=(e)+sha1_K_00_19+sha1_ROTATE((a),5)+sha1_F_00_19((b),(c),(d)); \
	(b)=sha1_ROTATE((b),30);

#define sha1_BODY_20_31(i,a,b,c,d,e,f,xi,xa,xb,xc,xd) \
	sha1_XUPDATE(f,xi,xa,xb,xc,xd); \
	(f)+=(e)+sha1_K_20_39+sha1_ROTATE((a),5)+sha1_F_20_39((b),(c),(d)); \
	(b)=sha1_ROTATE((b),30);

#define sha1_BODY_32_39(i,a,b,c,d,e,f,xa,xb,xc,xd) \
	sha1_XUPDATE(f,xa,xa,xb,xc,xd); \
	(f)+=(e)+sha1_K_20_39+sha1_ROTATE((a),5)+sha1_F_20_39((b),(c),(d)); \
	(b)=sha1_ROTATE((b),30);

#define sha1_BODY_40_59(i,a,b,c,d,e,f,xa,xb,xc,xd) \
	sha1_XUPDATE(f,xa,xa,xb,xc,xd); \
	(f)+=(e)+sha1_K_40_59+sha1_ROTATE((a),5)+sha1_F_40_59((b),(c),(d)); \
	(b)=sha1_ROTATE((b),30);

#define sha1_BODY_60_79(i,a,b,c,d,e,f,xa,xb,xc,xd) \
	sha1_XUPDATE(f,xa,xa,xb,xc,xd); \
	(f)=xa+(e)+sha1_K_60_79+sha1_ROTATE((a),5)+sha1_F_60_79((b),(c),(d)); \
	(b)=sha1_ROTATE((b),30);

#ifdef X
#undef X
#endif

  /*
   * Originally X was an array. As it's automatic it's natural
   * to expect RISC compiler to accomodate at least part of it in
   * the register bank, isn't it? Unfortunately not all compilers
   * "find" this expectation reasonable:-( On order to make such
   * compilers generate better code I replace X[] with a bunch of
   * X0, X1, etc. See the function body below...
   *					<appro@fy.chalmers.se>
   */
#define X(i)	XX##i

/******************************************************************************
 * FUNCTION PURPOSE: SHA1 block hashing function (coded for speed)
 ******************************************************************************
 * DESCRIPTION: Hashes input data according to the SHA-1 algorithm.  Data is passed to this
 *                        function in multiples of 512 bits.  The data will be hashed in blocks of 512 bits.
 *                        The function will loop until all 512 bit blocks have been hashed.
 *
 *                        Note: This function has been written for speed by unrolling all loops and 
 *                                 eliminating as many variable assignments as possible.
 *
 *    void sha1_block(         
 *      sha1Inst_t   *c,      - SHA1 instance pointer.
 *      const void    *p,     - Input data pointer.
 *      tint              num)   - Number of 512 bit blocks the input data consists of
 *                                           
 *****************************************************************************/
static void sha1_block(sha1Inst_t *c, const void *p, tint num)
{
  const tword *data=(const tword *)p;
  tulong A,B,C,D,E,T,l;
  tulong XX0,XX1,XX2,XX3,XX4,XX5,XX6,XX7,XX8, XX9,XX10,XX11,XX12,XX13,XX14,XX15;

  A=c->h0;
  B=c->h1;
  C=c->h2;
  D=c->h3;
  E=c->h4;
  
  for (;;)
  {
    salld_util_GETU32_I(data,l); X( 0)=l;
    salld_util_GETU32_I(data,l); X( 1)=l;
    sha1_BODY_00_15( 0,A,B,C,D,E,T,X( 0)); salld_util_GETU32_I(data,l); X( 2)=l;
    sha1_BODY_00_15( 1,T,A,B,C,D,E,X( 1)); salld_util_GETU32_I(data,l); X( 3)=l;
    sha1_BODY_00_15( 2,E,T,A,B,C,D,X( 2)); salld_util_GETU32_I(data,l); X( 4)=l;
    sha1_BODY_00_15( 3,D,E,T,A,B,C,X( 3)); salld_util_GETU32_I(data,l); X( 5)=l;
    sha1_BODY_00_15( 4,C,D,E,T,A,B,X( 4)); salld_util_GETU32_I(data,l); X( 6)=l;
    sha1_BODY_00_15( 5,B,C,D,E,T,A,X( 5)); salld_util_GETU32_I(data,l); X( 7)=l;
    sha1_BODY_00_15( 6,A,B,C,D,E,T,X( 6)); salld_util_GETU32_I(data,l); X( 8)=l;
    sha1_BODY_00_15( 7,T,A,B,C,D,E,X( 7)); salld_util_GETU32_I(data,l); X( 9)=l;
    sha1_BODY_00_15( 8,E,T,A,B,C,D,X( 8)); salld_util_GETU32_I(data,l); X(10)=l;
    sha1_BODY_00_15( 9,D,E,T,A,B,C,X( 9)); salld_util_GETU32_I(data,l); X(11)=l;
    sha1_BODY_00_15(10,C,D,E,T,A,B,X(10)); salld_util_GETU32_I(data,l); X(12)=l;
    sha1_BODY_00_15(11,B,C,D,E,T,A,X(11)); salld_util_GETU32_I(data,l); X(13)=l;
    sha1_BODY_00_15(12,A,B,C,D,E,T,X(12)); salld_util_GETU32_I(data,l); X(14)=l;
    sha1_BODY_00_15(13,T,A,B,C,D,E,X(13)); salld_util_GETU32_I(data,l); X(15)=l;
    sha1_BODY_00_15(14,E,T,A,B,C,D,X(14)); 
    sha1_BODY_00_15(15,D,E,T,A,B,C,X(15)); 
   
    sha1_BODY_16_19(16,C,D,E,T,A,B,X( 0),X( 0),X( 2),X( 8),X(13));
    sha1_BODY_16_19(17,B,C,D,E,T,A,X( 1),X( 1),X( 3),X( 9),X(14));
    sha1_BODY_16_19(18,A,B,C,D,E,T,X( 2),X( 2),X( 4),X(10),X(15));
    sha1_BODY_16_19(19,T,A,B,C,D,E,X( 3),X( 3),X( 5),X(11),X( 0));

    sha1_BODY_20_31(20,E,T,A,B,C,D,X( 4),X( 4),X( 6),X(12),X( 1));
    sha1_BODY_20_31(21,D,E,T,A,B,C,X( 5),X( 5),X( 7),X(13),X( 2));
    sha1_BODY_20_31(22,C,D,E,T,A,B,X( 6),X( 6),X( 8),X(14),X( 3));
    sha1_BODY_20_31(23,B,C,D,E,T,A,X( 7),X( 7),X( 9),X(15),X( 4));
    sha1_BODY_20_31(24,A,B,C,D,E,T,X( 8),X( 8),X(10),X( 0),X( 5));
    sha1_BODY_20_31(25,T,A,B,C,D,E,X( 9),X( 9),X(11),X( 1),X( 6));
    sha1_BODY_20_31(26,E,T,A,B,C,D,X(10),X(10),X(12),X( 2),X( 7));
    sha1_BODY_20_31(27,D,E,T,A,B,C,X(11),X(11),X(13),X( 3),X( 8));
    sha1_BODY_20_31(28,C,D,E,T,A,B,X(12),X(12),X(14),X( 4),X( 9));
    sha1_BODY_20_31(29,B,C,D,E,T,A,X(13),X(13),X(15),X( 5),X(10));
    sha1_BODY_20_31(30,A,B,C,D,E,T,X(14),X(14),X( 0),X( 6),X(11));
    sha1_BODY_20_31(31,T,A,B,C,D,E,X(15),X(15),X( 1),X( 7),X(12));

    sha1_BODY_32_39(32,E,T,A,B,C,D,X( 0),X( 2),X( 8),X(13));
    sha1_BODY_32_39(33,D,E,T,A,B,C,X( 1),X( 3),X( 9),X(14));
    sha1_BODY_32_39(34,C,D,E,T,A,B,X( 2),X( 4),X(10),X(15));
    sha1_BODY_32_39(35,B,C,D,E,T,A,X( 3),X( 5),X(11),X( 0));
    sha1_BODY_32_39(36,A,B,C,D,E,T,X( 4),X( 6),X(12),X( 1));
    sha1_BODY_32_39(37,T,A,B,C,D,E,X( 5),X( 7),X(13),X( 2));
    sha1_BODY_32_39(38,E,T,A,B,C,D,X( 6),X( 8),X(14),X( 3));
    sha1_BODY_32_39(39,D,E,T,A,B,C,X( 7),X( 9),X(15),X( 4));

    sha1_BODY_40_59(40,C,D,E,T,A,B,X( 8),X(10),X( 0),X( 5));
    sha1_BODY_40_59(41,B,C,D,E,T,A,X( 9),X(11),X( 1),X( 6));
    sha1_BODY_40_59(42,A,B,C,D,E,T,X(10),X(12),X( 2),X( 7));
    sha1_BODY_40_59(43,T,A,B,C,D,E,X(11),X(13),X( 3),X( 8));
    sha1_BODY_40_59(44,E,T,A,B,C,D,X(12),X(14),X( 4),X( 9));
    sha1_BODY_40_59(45,D,E,T,A,B,C,X(13),X(15),X( 5),X(10));
    sha1_BODY_40_59(46,C,D,E,T,A,B,X(14),X( 0),X( 6),X(11));
    sha1_BODY_40_59(47,B,C,D,E,T,A,X(15),X( 1),X( 7),X(12));
    sha1_BODY_40_59(48,A,B,C,D,E,T,X( 0),X( 2),X( 8),X(13));
    sha1_BODY_40_59(49,T,A,B,C,D,E,X( 1),X( 3),X( 9),X(14));
    sha1_BODY_40_59(50,E,T,A,B,C,D,X( 2),X( 4),X(10),X(15));
    sha1_BODY_40_59(51,D,E,T,A,B,C,X( 3),X( 5),X(11),X( 0));
    sha1_BODY_40_59(52,C,D,E,T,A,B,X( 4),X( 6),X(12),X( 1));
    sha1_BODY_40_59(53,B,C,D,E,T,A,X( 5),X( 7),X(13),X( 2));
    sha1_BODY_40_59(54,A,B,C,D,E,T,X( 6),X( 8),X(14),X( 3));
    sha1_BODY_40_59(55,T,A,B,C,D,E,X( 7),X( 9),X(15),X( 4));
    sha1_BODY_40_59(56,E,T,A,B,C,D,X( 8),X(10),X( 0),X( 5));
    sha1_BODY_40_59(57,D,E,T,A,B,C,X( 9),X(11),X( 1),X( 6));
    sha1_BODY_40_59(58,C,D,E,T,A,B,X(10),X(12),X( 2),X( 7));
    sha1_BODY_40_59(59,B,C,D,E,T,A,X(11),X(13),X( 3),X( 8));

    sha1_BODY_60_79(60,A,B,C,D,E,T,X(12),X(14),X( 4),X( 9));
    sha1_BODY_60_79(61,T,A,B,C,D,E,X(13),X(15),X( 5),X(10));
    sha1_BODY_60_79(62,E,T,A,B,C,D,X(14),X( 0),X( 6),X(11));
    sha1_BODY_60_79(63,D,E,T,A,B,C,X(15),X( 1),X( 7),X(12));
    sha1_BODY_60_79(64,C,D,E,T,A,B,X( 0),X( 2),X( 8),X(13));
    sha1_BODY_60_79(65,B,C,D,E,T,A,X( 1),X( 3),X( 9),X(14));
    sha1_BODY_60_79(66,A,B,C,D,E,T,X( 2),X( 4),X(10),X(15));
    sha1_BODY_60_79(67,T,A,B,C,D,E,X( 3),X( 5),X(11),X( 0));
    sha1_BODY_60_79(68,E,T,A,B,C,D,X( 4),X( 6),X(12),X( 1));
    sha1_BODY_60_79(69,D,E,T,A,B,C,X( 5),X( 7),X(13),X( 2));
    sha1_BODY_60_79(70,C,D,E,T,A,B,X( 6),X( 8),X(14),X( 3));
    sha1_BODY_60_79(71,B,C,D,E,T,A,X( 7),X( 9),X(15),X( 4));
    sha1_BODY_60_79(72,A,B,C,D,E,T,X( 8),X(10),X( 0),X( 5));
    sha1_BODY_60_79(73,T,A,B,C,D,E,X( 9),X(11),X( 1),X( 6));
    sha1_BODY_60_79(74,E,T,A,B,C,D,X(10),X(12),X( 2),X( 7));
    sha1_BODY_60_79(75,D,E,T,A,B,C,X(11),X(13),X( 3),X( 8));
    sha1_BODY_60_79(76,C,D,E,T,A,B,X(12),X(14),X( 4),X( 9));
    sha1_BODY_60_79(77,B,C,D,E,T,A,X(13),X(15),X( 5),X(10));
    sha1_BODY_60_79(78,A,B,C,D,E,T,X(14),X( 0),X( 6),X(11));
    sha1_BODY_60_79(79,T,A,B,C,D,E,X(15),X( 1),X( 7),X(12));
	
    c->h0=(c->h0+E)&0xffffffffL; 
    c->h1=(c->h1+T)&0xffffffffL;
    c->h2=(c->h2+A)&0xffffffffL;
    c->h3=(c->h3+B)&0xffffffffL;
    c->h4=(c->h4+C)&0xffffffffL;

    if (--num == 0)
    {
      break;
    }

    A=c->h0;
    B=c->h1;
    C=c->h2;
    D=c->h3;
    E=c->h4;

  }
}


#else
/* Use small footprint macros if small footprint build time option is defined */

#define sha1_BODY_00_15(xi)		 do {	\
	T=E+sha1_K_00_19+sha1_F_00_19(B,C,D);	\
	E=D, D=C, C=sha1_ROTATE(B,30), B=A;	\
	A=sha1_ROTATE(A,5)+T+xi;	    } while(0)

#define sha1_BODY_16_19(xa,xb,xc,xd)	 do {	\
	sha1_XUPDATE(T,xa,xa,xb,xc,xd);	\
	T+=E+sha1_K_00_19+sha1_F_00_19(B,C,D);	\
	E=D, D=C, C=sha1_ROTATE(B,30), B=A;	\
	A=sha1_ROTATE(A,5)+T;	    } while(0)

#define sha1_BODY_20_39(xa,xb,xc,xd)	 do {	\
	sha1_XUPDATE(T,xa,xa,xb,xc,xd);	\
	T+=E+sha1_K_20_39+sha1_F_20_39(B,C,D);	\
	E=D, D=C, C=sha1_ROTATE(B,30), B=A;	\
	A=sha1_ROTATE(A,5)+T;	    } while(0)

#define sha1_BODY_40_59(xa,xb,xc,xd)	 do {	\
	sha1_XUPDATE(T,xa,xa,xb,xc,xd);	\
	T+=E+sha1_K_40_59+sha1_F_40_59(B,C,D);	\
	E=D, D=C, C=sha1_ROTATE(B,30), B=A;	\
	A=sha1_ROTATE(A,5)+T;	    } while(0)

#define sha1_BODY_60_79(xa,xb,xc,xd)	 do {	\
	sha1_XUPDATE(T,xa,xa,xb,xc,xd);	\
	T=E+sha1_K_60_79+sha1_F_60_79(B,C,D);	\
	E=D, D=C, C=sha1_ROTATE(B,30), B=A;	\
	A=sha1_ROTATE(A,5)+T+xa;	    } while(0)

/******************************************************************************
 * FUNCTION PURPOSE: SHA1 block hashing function (coded for size)
 ******************************************************************************
 * DESCRIPTION: Hashes input data according to the SHA-1 algorithm.  Data is passed to this
 *                        function in multiples of 512 bits.  The data will be hashed in blocks of 512 bits.
 *                        The function will loop until all 512 bit blocks have been hashed.
 *
 *                        Note: This function has been written for a small memory footprint
 *
 *    void sha1_block(         
 *      sha1Inst_t   *c,      - SHA1 instance pointer.
 *      const void    *p,     - Input data pointer.
 *      tint              num)   - Number of 512 bit blocks the input data consists of
 *                                           
 *****************************************************************************/
static void sha1_block(sha1Inst_t *c, const void *p, tint num)
{
  const tword *data=p;
  tulong A,B,C,D,E,T,l;
  tint i;
  tulong X[16];

  A=c->h0;
  B=c->h1;
  C=c->h2;
  D=c->h3;
  E=c->h4;

  for (;;)
  {
    for (i=0;i<16;i++)
    {
      salld_util_GETU32_I(data,l); 
      X[i]=l; 
      sha1_BODY_00_15(X[i]); 
    }
    for (i=0;i<4;i++)
    {
      sha1_BODY_16_19(X[i], X[i+2],X[i+8], X[(i+13)&15]); 
    }
    for (;i<24;i++)
    {
      sha1_BODY_20_39(X[i&15], X[(i+2)&15], X[(i+8)&15], X[(i+13)&15]); 
    }
    for (i=0;i<20;i++)
    {
      sha1_BODY_40_59(X[(i+8)&15], X[(i+10)&15], X[i&15], X[(i+5)&15]);  
    }
    for (i=4;i<24;i++)
    {
      sha1_BODY_60_79(X[(i+8)&15], X[(i+10)&15], X[i&15], X[(i+5)&15]);  
    }

    c->h0=(c->h0+A)&0xffffffffL; 
    c->h1=(c->h1+B)&0xffffffffL;
    c->h2=(c->h2+C)&0xffffffffL;
    c->h3=(c->h3+D)&0xffffffffL;
    c->h4=(c->h4+E)&0xffffffffL;

    if (--num == 0) 
    {
      break;
    }

    A=c->h0;
    B=c->h1;
    C=c->h2;
    D=c->h3;
    E=c->h4;

  }
}

#endif /* #ifndef sha1_SMALL_FOOTPRINT */

#endif /* _SHA1_H */
