/*
*
* Copyright (c) 2021 Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef __CGTPRAGMAS_H
#define __CGTPRAGMAS_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Symbol alignment */

#ifdef TITOOLS
    #define EMIT_PRAGMA_DATA_ALIGN                      #pragma DATA_ALIGN
    #define DO_PRAGMA_ALIGN(_symbol, _alignment)        EMIT_PRAGMA_DATA_ALIGN( _symbol, _alignment )
#elif defined(RVCT)
    #define DO_PRAGMA_ALIGN(_symbol, _alignment)        __align(_alignment)
#elif defined(GCC)
    #define DO_PRAGMA_ALIGN(_symbol, _alignment)        __attribute__ ((aligned (_alignment)))
#else
    #define DO_PRAGMA_ALIGN(_symbol, _alignment)        __attribute__ ((aligned (_alignment)))
#endif


/* Code Section */
#ifdef TITOOLS
    #define EMIT_PRAGMA_CODE_SECTION(x)                 _Pragma(#x)
    #define DO_PRAGMA_CODE_SECTION(_symbol,_section)    EMIT_PRAGMA_CODE_SECTION(CODE_SECTION( _symbol, _section ))
#elif defined(RVCT)
    #define DO_PRAGMA_CODE_SECTION(_symbol,_section)    __attribute__ ((section (_section)))
#elif defined(GCC)
    #define DO_PRAGMA_CODE_SECTION(_symbol,_section)    __attribute__ ((section (_section)))
#else
    #define DO_PRAGMA_CODE_SECTION(_symbol,_section)    __attribute__ ((section (_section)))
#endif


/* Data Section */
#ifdef TITOOLS
    #define EMIT_PRAGMA_DATA_SECTION(x)                 _Pragma(#x)
    #define DO_PRAGMA_DATA_SECTION(_symbol,_section)    EMIT_PRAGMA_DATA_SECTION(DATA_SECTION( _symbol, _section ))
#elif defined(RVCT)
    #define DO_PRAGMA_DATA_SECTION(_symbol,_section)    __attribute__ ((section (_section)))
#elif defined(GCC)
    #define DO_PRAGMA_DATA_SECTION(_symbol,_section)    __attribute__ ((section (_section)))
#else
    #define DO_PRAGMA_DATA_SECTION(_symbol,_section)    __attribute__ ((section (_section)))
#endif


/* Inline */
#ifdef RVCT
    #define INLINE  __inline
#elif defined(TITOOLS)
    #define INLINE  inline
#elif defined(GCC)
    #define INLINE  inline
#else
    #define INLINE inline
#endif

#ifdef RVCT
    #define ALWAYS_INLINE  
#elif defined(TITOOLS)
    #define ALWAYS_INLINE  __attribute__ ((always_inline))
#elif defined(GCC)
    #define ALWAYS_INLINE  __attribute__ ((always_inline))
#else
    #define ALWAYS_INLINE  __attribute__ ((always_inline))
#endif  /* RVCT */


/* Optimize */

#ifdef RVCT
    #define OPTIMIZE(LEVEL) 
#elif defined(TITOOLS)
    #define OPTIMIZE(LEVEL)
#elif defined(GCC)
    #define OPTIMIZE(LEVEL) __attribute__ ((optimize(LEVEL)))
#else
    #define OPTIMIZE(LEVEL)
#endif



/* Embedded Assembly */

#ifdef RVCT
    #define ASSEMBLY    asm
#elif defined(GCC)
    #define ASSEMBLY    __asm
#elif defined(TITOOLS)
    #define ASSEMBLY    asm
#else
    #define ASSEMBLY    asm
#endif

#define  CONCAT2(a, b) XCAT2(a, b)
#define  XCAT2(a, b)   a ## b
#define  CONCAT3(a, b, c) XCAT3(a, b, c)
#define  XCAT3(a, b, c)   a ## b ## c

#ifdef __cplusplus
}
#endif
#endif	/* __CGTPRAGMAS_H */

/* EOF */

