#ifndef _SALLDPORT_H
#define _SALLDPORT_H
/******************************************************************************
 * FILE PURPOSE:  Data Types and Macro for SALLD
 ******************************************************************************
 * FILE NAME:   salldport.h  
 *
 * DESCRIPTION: Data Types and Macro Defines for SALLD
 *
 *
 * (C) Copyright 2009-2012, Texas Instruments Inc.
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
/* System level header files */
#include <stdint.h> 
#include <string.h>
/* Legacy typedefs from TI MAS component */
typedef uint16_t    tuint;
typedef int16_t     tint;
typedef uint32_t    tulong;
typedef int32_t     tlong;
typedef uint8_t     tword;

#define TYP_TWORD_SIZE      8
#define TYP_TINT_SIZE       16
#define TYP_TULONG_SIZE     32

#if defined(BUILD_A15_0) || defined(BUILD_MCU) || defined (BUILD_MPU) || defined (BUILD_C7X)
#define _LITTLE_ENDIAN
#endif

#if ( !defined( _LITTLE_ENDIAN ) && !defined( _BIG_ENDIAN ) ) \
||  ( defined(_LITTLE_ENDIAN ) && defined( _BIG_ENDIAN ) )
#error either _LITTLE_ENDIAN or _BIG_ENDIAN must be defined
#endif

#ifndef NULL 
#define NULL 0
#endif

#define SALLD_SIZE_OF_TUINT_IN_BYTE ((TYP_TINT_SIZE)/(8))
#define SALLD_SIZE_OF_TUINT_IN_WORD ((TYP_TINT_SIZE)/(TYP_TWORD_SIZE))
#define SALLD_SIZE_OF_WORD_IN_BYTE ((TYP_TWORD_SIZE)/(8))
#define SALLD_BYTE_TO_TUINT(a)  ((a)/SALLD_SIZE_OF_TUINT_IN_BYTE)
#define SALLD_BYTE_TO_WORD(a)  ((a)/SALLD_SIZE_OF_WORD_IN_BYTE)

/* ggdsp abstraction for RTSC misc_utlCopy function */
static inline void misc_utlCopy(uint16_t * a, uint16_t * b, int16_t c) {
  memcpy((void *)(b), (void *)(a), (c) * sizeof(uint16_t));
}

#if defined(_BIG_ENDIAN)
    #define SALLD_UINT16_BE(x)      (x)
#else
    #define SALLD_UINT16_BE(x)      (((x) >> 8) ^ ((x) << 8))
#endif

#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))

#define ROTATE64(a,n)   (((a)<<(n))|((a)>>(64-(n))))

#define SALLD_DIV_ROUND_UP(val,rnd)   (((val)+(rnd)-1)/(rnd))
#define SALLD_ROUND_UP(val,rnd)       SALLD_DIV_ROUND_UP(val,rnd)*(rnd)


#define SALLD_MK_UINT16(high8,low8)                               \
    ((uint16_t)( ((uint16_t)(high8) << 8) | (uint16_t)(low8) ))

#define SALLD_UINT16_LOW8(a)	                                    \
    ((uint8_t)((a) & 0x00FF))

#define SALLD_UINT16_HIGH8(a)	                                    \
    ((uint8_t)(((a) >> 8) & 0x00FF))

#define SALLD_MK_UINT32(high16,low16)                             \
    ((uint32_t)( ((uint32_t)(high16) << 16) | (uint32_t)(low16) ))

#define SALLD_MK_UINT32_FROM8S(high8,med_high8,med_low8,low8)     \
    SALLD_MK_UINT32(SALLD_MK_UINT16(high8,med_high8), SALLD_MK_UINT16(med_low8, low8))

#define SALLD_MK_UINT32_FROM_8ARRAY(ptr) \
	((uint32_t)(ptr[0]<<24) | (ptr[1]<<16) | (ptr[2]<<8) | (ptr[3]))
	
#define SALLD_UINT32_LOW16(u32)                                   \
    ((uint16_t)((u32) & 0xFFFF))

#define SALLD_UINT32_HIGH16(u32)                                  \
    ((uint16_t)(((u32) >> 16) & 0xFFFF))

#define SALLD_UINT32_LOW8(u32)                                    \
    ((uint8_t)((u32) & 0x00FF))

#define SALLD_UINT32_MED_LOW8(u32)                                \
    ((uint8_t)(((u32) >> 8) & 0xFF))

#define SALLD_UINT32_MED_HIGH8(u32)                               \
    ((uint8_t)(((u32) >> 16) & 0xFF))

#define SALLD_UINT32_HIGH8(u32)                                   \
    ((uint8_t)(((u32) >> 24) & 0xFF))

#define SALLD_SWAP_UINT16(w)	    \
    (SALLD_MK_UINT16(SALLD_UINT16_LOW8(w), SALLD_UINT16_HIGH8(w)))

#define SALLD_SWAP_UINT32(u32)                \
    (SALLD_MK_UINT32_FROM8S(                  \
        SALLD_UINT32_LOW8(u32),               \
        SALLD_UINT32_MED_LOW8(u32),           \
        SALLD_UINT32_MED_HIGH8(u32),          \
        SALLD_UINT32_HIGH8(u32)))             
#define SALLD_SWIZ(x)  (sizeof((x)) == 1 ? (x) : (sizeof((x)) == 2 ? salld_swiz16((x)) : (sizeof((x)) == 4 ? salld_swiz32((x)) : (sizeof((x)) == 8 ? salld_swiz64((x)):0))))

/*********************************************************************
 * FUNCTION PURPOSE: Swizzling
 *********************************************************************
 * DESCRIPTION: The PA sub-system requires all multi-byte fields in
 *              big endian format.
 *********************************************************************/
static inline uint16_t salld_swiz16(uint16_t x)
{
  return ((x >> 8) | (x << 8));
}

static inline uint32_t salld_swiz32 (uint32_t x)
{
  return (((x) >> 24) | (((x) >> 8) & 0xff00L) | (((x) << 8) & 0xff0000L) | ((x) << 24));
}

static inline uint64_t salld_swiz64 (uint64_t x)
{
  return (((x) >> 56) | (((x) >> 40) & 0xff00LL) | (((x) >> 16) & 0xff0000LL) | (((x) >> 8) & 0xff000000LL) | \
          (((x) << 8) & 0xff00000000LL) | (((x) << 24) & 0xff0000000000LL) | (((x) << 48) & 0xff000000000000LL) | ((x) << 56));
}

/* Main macros for accessing configuration bit fields       *
 * Input parameter a is tuint containing bit field.         *
 * b is bit offset withing bit field. c is number of bits   *
 * used by that parameter. x is new value of parameter that *
 * is packed in this bit field.                             */
#define UTL_GET_BITFIELD(a,b,c)    (((a)>>(b)) & ((1U<<(c))-1))
//#define UTL_SET_BITFIELD(a,x,b,c)  a &=~((((tuint)1<<(c))-(tuint)1)<<(b)), a |=((tuint)(x)<<(b))

/* following one enforces strict setting to prevent overflow into other bits, would
   cost program space for additional protection */
#define UTL_SET_BITFIELD(a,x,b,c)  (a) &= ~(((1U<<(c))-1)<<(b)), \
                                   (a) |= (((x) & ((1U<<(c))-1))<<(b))

#ifndef SALLD_FIELDOFFSET
	#define SALLD_FIELDOFFSET(type,member) ((size_t) &(((type*) 0)->member))*SALLD_SIZE_OF_WORD_IN_BYTE
#endif

#ifndef max
    #define max(x, y)   (x > y)?x:y
#endif

#ifndef min
    #define min(x, y)   (x < y)?x:y
#endif

/* swap before writing and swap after reading */
#define salld_util_GETU16(pt) ( (((tuint)(((pt)[0])       )) << 8) ^ \
                     (((tuint)(((pt)[1])&0x00FF))     ) )
#define salld_util_PUTU16(ct, st) { (ct)[0] = (tword)((st) >> 8); \
                         (ct)[1] = (tword)(st); }
#define salld_util_GETU16_I(pt, gt) { gt = salld_util_GETU16(pt); (pt) += 2; }
#define salld_util_PUTU16_I(ct, st) { salld_util_PUTU16(ct, st); (ct) += 2; }
#define salld_util_GETU16_O(pt, offset) ( (((tuint)((pt)[offset]) ) <<8) ^ (tuint)(((pt)[offset+1])&0x00FF) )
/* offset is in bytes */
/* Write uint16 from src to at pt + offset */
#define salld_util_PUTU16_O(pt, offset, st) { salld_util_PUTU16(((pt)+(offset)), st) }

#define salld_util_GETU32(pt) ( ( ((tulong)((pt)[0]) ) << 24) ^ \
                     ( (((tulong)((pt)[1]) )&0x000000FF) << 16) ^ \
                     ( (((tulong)((pt)[2]) )&0x000000FF) <<  8) ^ \
                     ( (((tulong)((pt)[3]) )&0x000000FF)      ) )
#define salld_util_PUTU32(ct, st) {(ct)[0] = (tword)((st)>>24); (ct)[1] = (tword)((st)>>16); \
                             (ct)[2] = (tword)((st) >>  8); (ct)[3] = (tword)(st);}
#define salld_util_GETU32_I(pt, gt) { gt = salld_util_GETU32(pt); (pt) += 4; }
#define salld_util_PUTU32_I(ct, st) { salld_util_PUTU32(ct, st); (ct) += 4; }
#define salld_util_ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))

/*
 * Global array used by the MAC routines
 
 * Note: They are defined at salldsha1.c
 */
extern uint32_t MacLastWordMask[];
extern uint32_t MacPaddingWord[];



#endif
/* nothing past this point */ 
