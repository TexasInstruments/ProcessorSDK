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

/*!
 *  \file
 *  \brief Common structures and accessors for device memory map.
 */

#ifndef _DEVINFO_H_
#define _DEVINFO_H_

#include <stdint.h>
#include <stdbool.h>
#include <ti/drv/lpm/include/io_retention/cgt_pragmas.h>

/*!
 *  \brief Structure used to desribe register attributes for most MMR testcases.
 */
struct reg_info {
  uintptr_t offset;       /* < register offset from base */
  uint32_t  reset_val;    /* < reset value of the register. */
  uint32_t  wr_mask;      /* < 1 means bit can be written */
  uint32_t  wr_eff_mask;  /* < 1 means writes to the bit change value in reg */
  uint32_t  wr_val;       /* < last written value */
  uint32_t  rd_val;       /* < last read value. */
};

/*!
 *  \brief Structure used to desribe register attributes for most MMR testcases for 64bit address.
 */
struct reg64_info {
  uint64_t  offset;       /* < register offset from base */
  uint32_t  reset_val;    /* < reset value of the register. */
  uint32_t  wr_mask;      /* < 1 means bit can be written */
  uint32_t  wr_eff_mask;  /* < 1 means writes to the bit change value in reg */
  uint32_t  wr_val;       /* < last written value */
  uint32_t  rd_val;       /* < last read value. */
};

/*!
 *  \brief Structure used to desribe register attributes for most MMR testcases.
 *  \sa struct reg_info
 */
struct greg_info {
    volatile uint32_t *addr;        /* < full address of the MMR */
    uint32_t          reset_val;    /* < reset value of the register. */
    uint32_t          wr_mask;      /* < 1 means bit can be written */
    uint32_t          wr_eff_mask;  /* < 1 means writes to the bit change value in reg */
    uint32_t          wr_val;       /* < last written value */
    uint32_t          rd_val;       /* < last read value. */
};


struct rsv_info {
    uint32_t start;
    uint32_t end;
};

struct rand_seq {
    uint32_t access_type;
    uint32_t offset;
    uint32_t rw_type;
    uint32_t access_stage;
};

/*! \brief base2core macro for m3 core.
 *  \sa base2core
 *
 *  \param base The global address of the peripheral you're trying to test.
 *  \returns The M3's view of the global address.
 *
 *  \warning <b>DO NOT</b> call this macro directly. Please call \c base2core to ensure portable code.
 *  
 *  \b Note:
 *  - Based on the config's default RAT setup.
 *  - DMSC (M3) total available RAT space is 2GB.
 *  - With the below table implemented, the space left over will be 384MB which can be remapped as needed.
 *
 *  <b> Mapping Table (Default) </b>
 *  Shown in ascending Global Addresses.
 *  | DMSC RAT Region | Global Address View | M3 Address View (e.g. pre-RAT) | Size (MB) |
 *  | --------------- | ------------------- | ------------------------------ | ---- |
 *  | 0 | \c 0x00000000   - \c 0x3fffffff   | \c 0x80000000 - \c 0xbfffffff | 1024 |
 *  | 1 | \c 0x40000000   - \c 0x43ffffff   | \c 0x70000000 - \c 0x73ffffff | 64 |
 *  | 1 | \c 0x46000000   - \c 0x47ffffff   | \c 0x76000000 - \c 0x77ffffff | 32 |
 *  | 2 | \c 0x48000000   - \c 0x4fffffff   | \c 0xC0000000 - \c 0xC7ffffff | 128 |
 *  | 3 | \c 0x50000000   - \c 0x51ffffff   | \c 0x7C000000 - \c 0x7Dffffff | 32 |
 *  | 4 | \c 0x58000000   - \c 0x59ffffff   | \c 0x7E000000 - \c 0x7Fffffff | 32 |
 *  | 5 | \c 0x70000000   - \c 0x73ffffff   | \c 0x78000000 - \c 0x7Bffffff | 64 |
 *  | 6 | \c 0x80000000   - \c 0x87ffffff   | \c 0x60000000 - \c 0x67ffffff | 128 |
 *  | 7 | \c 0x8_00000000 - \c 0x8_07ffffff | \c 0x68000000 - \c 0x6fffffff | 128 |
 *  | 8 | \c 0x6c000000   - \c 0x6fffffff   | \c 0xD0000000 - \c 0xD3ffffff | 64 |
 *
 *  Same table as above show in ascending M3 Addresses.
 *  | M3 Address View (e.g. pre-RAT) | Global Address View (e.g. post-RAT) | Size (MB) | Comments |
 *  | ------------------------------ | ----------------------------------- | --------  | -------- |
 *  | \c 0x60000000 - \c 0x67ffffff | \c   0x80000000 - \c   0x87ffffff  | 128  | DDR_LO SRAM |
 *  | \c 0x68000000 - \c 0x6fffffff | \c  0x800000000 - \c  0x807ffffff  | 128  | DDR_HI SRAM |
 *  | \c 0x70000000 - \c 0x73ffffff | \c   0x40000000 - \c   0x43ffffff  | 64   | OCMSRAM |
 *  | \c 0x74000000 - \c 0x75ffffff | \c   0x44000000 - \c   0x45ffffff  | 32   | Will return address error. Cannot access DMSC slave port from DMSC master port. |
 *  | \c 0x76000000 - \c 0x77ffffff | \c   0x46000000 - \c   0x47ffffff  | 32   | |
 *  | \c 0x78000000 - \c 0x79ffffff | \c   0x70000000 - \c   0x73ffffff  | 32   | MSMC SRAM |
 *  | \c 0x7A000000 - \c 0x7Bffffff | \c 0x4F02000000 - \c 0x4F03ffffff  | 32   | MSMC SRAM |
 *  | \c 0x7C000000 - \c 0x7Dffffff | \c   0x50000000 - \c   0x51ffffff  | 32   | FSS1 |
 *  | \c 0x7E000000 - \c 0x7fffffff | \c   0x58000000 - \c   0x59ffffff  | 32   | FSS2 |
 *  | \c 0x80000000 - \c 0xbfffffff | \c   0x00000000 - \c   0x3fffffff  | 1024 | PSRAM1, PSRAM2 |
 *  | \c 0xc0000000 - \c 0xc7ffffff | \c   0x48000000 - \c   0x4fffffff  | 128  | |
 *  | \c 0xD0000000 - \c 0xD3ffffff | \c   0x6c000000 - \c   0x6fffffff  | 64   | |
 *  | \c 0xcc000000 - \c 0xdfffffff | Unmapped | 384 | This region is unmapped by default and is free to use by test-writers. |
 *
 */
#define m3_base2core(base) \
  ( (((base) &    0xFE000000u) == 0x44000000u)              ? (((base) < 0x44100000u) ? ((base) & 0xfffffu) : (base)) : /* internal DMSC, IRAM DRAM and peripherals */ \
     ((base) <    0x40000000u)                              ?  ((base) + 0x80000000u) : /* for psram1, psram2 */ \
     ((base) <    0x48000000u)                              ?  ((base) | 0x30000000u) : /* for ocmsram */ \
    (((base) >=   0x50000000u) && ((base) <   0x52000000u)) ?  ((base) + 0x2C000000u) : /* for fss1 */ \
    (((base) >=   0x58000000u) && ((base) <   0x5A000000u)) ?  ((base) + 0x26000000u) : /* for fss2 */ \
    (((base) >=   0x70000000u) && ((base) <   0x74000000u)) ?  ((base) | 0x08000000u) : /* for msmc sram */ \
     ((base) <    0x50000000u)                              ?  ((base) + 0x78000000u) : \
    (((base) &    0xFC000000u) == 0x6C000000u)              ?  (((base) - 0x6C000000u) + 0xD0000000u) : \
    (((base) >=   0x80000000u) && ((base) <   0x88000000u)) ?  (((base) | 0x60000000u) & ~0x80000000u) : /* ddr_lo */ \
    (((base) >=  0x880000000u) && ((base) <  0x888000000u)) ?  (((base) | 0x68000000u) & ~0x800000000u) : /* ddr_hi */ \
    (((base) >= 0x4F02000000u) && ((base) < 0x4F04000000u)) ?  ((base & 0x01FFFFFFu ) + 0x7A000000u) : /* main_ocmsram */ \
      (base) \
  )

/*!
 *  \brief core2base macro for m3 core.
 *  \sa core2base m3_base2core
 *
 *  \param core The M3's local address view.
 *  \returns The global address.
 *
 *  \warning <b>DO NOT</b> call this macro directly. Please call \c core2base to ensure portable code.
 *
 *  \b Note:
 *  - This macro provides the opposite function of base2core.
 */
#define m3_core2base(core) \
   ( ((core) < 0x44000000u)                             ? ((core) + 0x44000000u) : /* address is in the iram or dram space of dmsc */ \
    (((core) < 0x60000000u) || ((core) >= 0xe0000000u)) ?  (core)                : /* address is already global in these cases */ \
     ((core) < 0x68000000u)                             ? ((core) + 0x20000000u) : \
     ((core) < 0x70000000u)                             ? ((core) - 0x68000000u + 0x800000000u) : \
     ((core) < 0x78000000u)                             ? ((core) - 0x30000000u) : \
     ((core) < 0x7a000000u)                             ? ((core) - 0x08000000u) : \
     ((core) < 0x7c000000u)                             ? ((core & 0x01ffffff) + 0x4F02000000u) : \
     ((core) < 0x7e000000u)                             ? ((core) - 0x2c000000u) : \
     ((core) < 0x80000000u)                             ? ((core) - 0x7c000000u + 0x58000000u) : \
     ((core) < 0xc0000000u)                             ? ((core) - 0x80000000u) : \
     ((core) < 0xc8000000u)                             ? ((core) - 0xc0000000u + 0x48000000u) : \
    (((core) >= 0xd0000000u) && ((core) < 0xd4000000u)) ? ((core) - 0xd0000000u + 0x6c000000u) : \
      (core))

/*!
 * \brief base2core macro for R5F core to account for MAIN_OCMSRAM re-located
 * to DDR
 *
 *  <b> Mapping Table (Default) </b>
 *  | R5F Region | Global Address View                               | R5F Address View              | Size (KB) |
 *  | ---------- | ------------------------------------------------- | ----------------------------- | --------- |
 *  | 0          | \c 0x0000000000     - \c 0x00000007ff             | \c 0xfff00000 - \c 0xfff007ff |        2  |
 *  | 1          | \c 0x0000200000     - \c 0x00002003ff             | \c 0xfff00800 - \c 0xfff00bff |        1  |
 *  | 2          | \c 0x0040280000     - \c 0x00402801ff             | \c 0xfff00c00 - \c 0xfff00dff |      512b |
 *  | 3          | \c 0x4F02000000     - \c 0x4F02080000             | \c 0xfc000000 - \c 0xfc07ffff |      512  |
 *  | 4          | \c TCM_EXTEND_BASE0 - \c TCM_EXTEND_BASE0+0x10000 | \c 0x00010000 - \c 0x0001ffff |       64  |
 *  | 5          | \c TCM_EXTEND_BASE1 - \c TCM_EXTEND_BASE1+0x10000 | \c 0x00020000 - \c 0x0002ffff |       64  |
 *  | 6          | \c 0x0880000000     - \c 0x0888000000             | \c 0xfc000000 - \c 0xfc07ffff |      128M |
 */

#define TCM_EXTEND_BASE0         (0x00000000)

#ifndef CONFIG_SRC
  #if (defined(R5FCORE))
    #undef  TCM_EXTEND_BASE0
    #if ((DDNUM/3) == 0)
      #define TCM_EXTEND_BASE    (0x41C00000)
      #if ((DDNUM%3) == 1)
        #define TCM_EXTEND_BASE0 (TCM_EXTEND_BASE + 0x00020000)
      #else
        #define TCM_EXTEND_BASE0 (TCM_EXTEND_BASE)
      #endif
    #else
      #define TCM_EXTEND_BASE    (0x70040000)
      #if ((DDNUM%3) == 1)
        #define TCM_EXTEND_BASE0 (TCM_EXTEND_BASE + (((DDNUM/3)-1) * 0x00040000) + 0x00020000)
      #else
        #define TCM_EXTEND_BASE0 (TCM_EXTEND_BASE + (((DDNUM/3)-1) * 0x00040000))
      #endif
    #endif
  #endif
#endif

#define TCM_EXTEND_BASE1         (TCM_EXTEND_BASE0+0x00010000)

#ifndef CONFIG_SRC 
  #ifdef DLOCKSTEP
    #define r5f_base2core_m(base) \
        ((((base) < 0x0000000800u))              ? ((base & 0x000007ffu) + 0xfff00000u) : \
         (((base) >= 0x0000200000u)    && ((base) < 0x0000200400u))              ? ((base & 0x000003ffu) + 0xfff00800u) : \
         (((base) >= 0x0040280000u)    && ((base) < 0x0040280200u))              ? ((base & 0x000001ffu) + 0xfff00c00u) : \
         (((base) >= 0x4F02000000u)    && ((base) < 0x4F02080000u))              ? ((base & 0x0007ffffu) + 0xfc000000u) : \
         (((base) >= 0x0880000000u)    && ((base) < 0x0888000000u))              ? ((base & 0x07ffffffu) + 0xf0000000u) : \
         (((base) >= TCM_EXTEND_BASE0) && ((base) < (TCM_EXTEND_BASE0+0x20000))) ? ((base & 0x0001ffffu) + 0x00020000u) : \
         (base))
  #else
    #define r5f_base2core_m(base) \
        ((((base) < 0x0000000800u))              ? ((base & 0x000007ffu) + 0xfff00000u) : \
         (((base) >= 0x0000200000u)    && ((base) < 0x0000200400u))              ? ((base & 0x000003ffu) + 0xfff00800u) : \
         (((base) >= 0x0040280000u)    && ((base) < 0x0040280200u))              ? ((base & 0x000001ffu) + 0xfff00c00u) : \
         (((base) >= 0x4F02000000u)    && ((base) < 0x4F02080000u))              ? ((base & 0x0007ffffu) + 0xfc000000u) : \
         (((base) >= 0x0880000000u)    && ((base) < 0x0888000000u))              ? ((base & 0x07ffffffu) + 0xf0000000u) : \
         (((base) >= TCM_EXTEND_BASE0) && ((base) < (TCM_EXTEND_BASE0+0x10000))) ? ((base & 0x0000ffffu) + 0x00010000u) : \
         (((base) >= TCM_EXTEND_BASE1) && ((base) < (TCM_EXTEND_BASE1+0x10000))) ? ((base & 0x0000ffffu) + 0x00020000u) : \
         (base))
  #endif
#else
    #define r5f_base2core_m(base) \
        ((((base) >= 0x0000000000u)    && ((base) < 0x0000000800u))              ? ((base & 0x000007ffu) + 0xfff00000u) : \
         (((base) >= 0x0000200000u)    && ((base) < 0x0000200400u))              ? ((base & 0x000003ffu) + 0xfff00800u) : \
         (((base) >= 0x0040280000u)    && ((base) < 0x0040280200u))              ? ((base & 0x000001ffu) + 0xfff00c00u) : \
         (((base) >= 0x4F02000000u)    && ((base) < 0x4F02080000u))              ? ((base & 0x0007ffffu) + 0xfc000000u) : \
         (((base) >= 0x0880000000u)    && ((base) < 0x0888000000u))              ? ((base & 0x07ffffffu) + 0xf0000000u) : \
         (((base) >= TCM_EXTEND_BASE0) && ((base) < (TCM_EXTEND_BASE0+0x10000))) ? ((base & 0x0000ffffu) + 0x00010000u) : \
         (((base) >= TCM_EXTEND_BASE1) && ((base) < (TCM_EXTEND_BASE1+0x10000))) ? ((base & 0x0000ffffu) + 0x00020000u) : \
         (base))
#endif

/*!
 * \brief core2base macro for R5F core.
 */
#ifndef CONFIG_SRC 
  #ifdef DLOCKSTEP
    #define r5f_core2base_m(core) \
       (((core >= 0xF0000000u) && (core < 0xF8000000u)) ? ((core & 0x07FFFFFFu) | 0x0880000000) : \
        ((core >= 0xFC000000u) && (core < 0xFC080000u)) ? ((core & 0x0007FFFFu) | 0x4F02000000) : \
        ((core >= 0xFFF00000u) && (core < 0xFFF00800u)) ? ((core & 0x000007FFu) | 0x0000000000) : \
        ((core >= 0xFFF00800u) && (core < 0xFFF00C00u)) ? ((core & 0x000003FFu) | 0x0000200000) : \
        ((core >= 0xFFF00C00u) && (core < 0xFFF00E00u)) ? ((core & 0x000001FFu) | 0x0040280000) : \
        ((core >= 0x00020000u) && (core < 0x00040000u)) ? ((core & 0x0001FFFFu) | TCM_EXTEND_BASE0) : \
        (core))
  #else
    #define r5f_core2base_m(core) \
       (((core >= 0xF0000000u) && (core < 0xF8000000u)) ? ((core & 0x07FFFFFFu) | 0x0880000000) : \
        ((core >= 0xFC000000u) && (core < 0xFC080000u)) ? ((core & 0x0007FFFFu) | 0x4F02000000) : \
        ((core >= 0xFFF00000u) && (core < 0xFFF00800u)) ? ((core & 0x000007FFu) | 0x0000000000) : \
        ((core >= 0xFFF00800u) && (core < 0xFFF00C00u)) ? ((core & 0x000003FFu) | 0x0000200000) : \
        ((core >= 0xFFF00C00u) && (core < 0xFFF00E00u)) ? ((core & 0x000001FFu) | 0x0040280000) : \
        ((core >= 0x00010000u) && (core < 0x00020000u)) ? ((core & 0x0000FFFFu) | TCM_EXTEND_BASE0) : \
        ((core >= 0x00020000u) && (core < 0x00030000u)) ? ((core & 0x0000FFFFu) | TCM_EXTEND_BASE1) : \
        (core))
  #endif
#else
    #define r5f_core2base_m(core) \
       (((core >= 0xF0000000u) && (core < 0xF8000000u)) ? ((core & 0x07FFFFFFu) | 0x0880000000) : \
        ((core >= 0xFC000000u) && (core < 0xFC080000u)) ? ((core & 0x0007FFFFu) | 0x4F02000000) : \
        ((core >= 0xFFF00000u) && (core < 0xFFF00800u)) ? ((core & 0x000007FFu) | 0x0000000000) : \
        ((core >= 0xFFF00800u) && (core < 0xFFF00C00u)) ? ((core & 0x000003FFu) | 0x0000200000) : \
        ((core >= 0xFFF00C00u) && (core < 0xFFF00E00u)) ? ((core & 0x000001FFu) | 0x0040280000) : \
        ((core >= 0x00010000u) && (core < 0x00020000u)) ? ((core & 0x0000FFFFu) | TCM_EXTEND_BASE0) : \
        ((core >= 0x00020000u) && (core < 0x00030000u)) ? ((core & 0x0000FFFFu) | TCM_EXTEND_BASE1) : \
        (core))
#endif


#ifndef CONFIG_SRC
/*!
 * \brief base2core macro for c66 core.
 *
 * Only 1 rat entry is programmed. We are leaving a hold to test c66 cfg port 
 *  <b> Mapping Table (Default) </b>
 *  | C66 Region | Global Address View        | c66 Address View              | Size (MB) |
 *  | ---------- | -------------------------- | ------------------------------| ---- |
    | x   | \c 0x4D81000000 - \c 0x4D81ffffff | \c 0x00000000 - \c 0x00ffffff |   64 | mapping for c66 local address to global view of its L2 SRAM
 *  | 0   | \c   0x00000000 - \c   0x00ffffff | \c 0xf0000000 - \c 0xf0ffffff |   16 |
 *  | 0   | \c   0x0c000000 - \c   0x0fffffff | \c 0xfc000000 - \c 0xffffffff |   64 |
 */
#if (DDNUM == 1)
#define c66_base2core(base) \
  (  ((base & 0xFFFF000000u) == 0x4D81000000u) ? (base & 0x00FFFFFFu) : \
     ((base & 0xFFFC000000u) == 0x000C000000u) ? (base                 | 0xF0000000u) : \
     ((base & 0xFFFF000000u) == 0x0000000000u) ? (base                 | 0xF0000000u) : (base))
#else
#define c66_base2core(base) \
  (  ((base & 0xFFFF000000u) == 0x4D80000000u) ? (base & 0x00FFFFFFu) : \
     ((base & 0xFFFC000000u) == 0x000C000000u) ? (base                 | 0xF0000000u) : \
     ((base & 0xFFFF000000u) == 0x0000000000u) ? (base                 | 0xF0000000u) : (base))
#endif


/*!
 * \brief core2base macro for c66 core.
 */
#if (DDNUM == 1)
#define c66_core2base(core) \
  ( ((core & 0xFF000000u) == 0x00000000u) ? (core | 0x4D81000000u) : \
    ((core & 0xFC000000u) == 0xFC000000u) ? (core &   0x0FFFFFFFu) : \
    ((core & 0xFF000000u) == 0xF0000000u) ? (core &   0x00FFFFFFu) : (core))
#else
#define c66_core2base(core) \
  ( ((core & 0xFF000000u) == 0x00000000u) ? (core | 0x4D80000000u) : \
    ((core & 0xFC000000u) == 0xFC000000u) ? (core &   0x0FFFFFFFu) : \
    ((core & 0xFF000000u) == 0xF0000000u) ? (core &   0x00FFFFFFu) : (core))
#endif
#endif


#if (defined M3CORE && !defined CUSTOM_RAT_SETUP)
#define base2core_m(base) ((uintptr_t)m3_base2core((uint64_t)(base)))
#define core2base_m(core) ((uint64_t)m3_core2base((uintptr_t)(core)))
#elif (defined C66XCORE)
#define base2core_m(base) ((uintptr_t)c66_base2core((uint64_t)(base)))
#define core2base_m(core) ((uint64_t)c66_core2base((uintptr_t)(core)))
#elif (defined R5FCORE)
#define base2core_m(base) ((uintptr_t)r5f_base2core_m((uint64_t)(base)))
#define core2base_m(core) ((uint64_t)r5f_core2base_m((uintptr_t)(core)))
#else
/*! \brief Default definition for base2core which is simply a pass-through.
 *  \sa m3_base2core
 *
 *  \b Notes:
 *  - DMSC (M3) has a default RAT setup different than its reset values.
 *  - Currently all other cores don't automatically configure RAT so therefore base2core by default is pass-through.
 */
#define base2core_m(base) ((uintptr_t)((uint64_t)(base)))

/*!
 *  \brief Default definition of core2base is simply a pass-through
 *  \sa m3_core2base
 */
#define core2base_m(core) ((uint64_t)((uintptr_t)(core)))
#endif

#define mkptr64(base,offset)      ((volatile uint64_t *)(base2core((base)+(offset))))
#define mkptr32(base,offset)      ((volatile uint32_t *)(base2core((base)+(offset))))
#define mkptr16(base,offset)      ((volatile uint16_t *)(base2core((base)+(offset))))
#define mkptr8(base,offset)       ((volatile uint8_t *)(base2core((base)+(offset))))
#define mkptr(base,offset)        mkptr32(base,offset)

#define mkptr64_m(base,offset)    ((volatile uint64_t *)(base2core_m((base)+(offset))))
#define mkptr32_m(base,offset)    ((volatile uint32_t *)(base2core_m((base)+(offset))))
#define mkptr16_m(base,offset)    ((volatile uint16_t *)(base2core_m((base)+(offset))))
#define mkptr8_m(base,offset)     ((volatile uint8_t *)(base2core_m((base)+(offset))))
#define mkptr_m(base,offset)      mkptr32_m(base,offset)

#if defined (TBARM9CORE)
#undef  mkptr64
#define mkptr64(base,offset)      mkptr64_m(base,offset)
#undef  mkptr32
#define mkptr32(base,offset)      mkptr32_m(base,offset)
#undef  mkptr16
#define mkptr16(base,offset)      mkptr16_m(base,offset)
#undef  mkptr8
#define mkptr8(base,offset)       mkptr8_m(base,offset)
#undef  mkptr
#define mkptr(base,offset)        mkptr_m(base,offset)
#endif

#ifdef A72CORE
/* A72 with cache enable run much faster so we need to increase the loop count to give enough wait time */
#define sleep(ticks)                      {volatile int i; for (i=0; i < (ticks * 2); i++);}
#define waitfor(cond,timeout)             {volatile int i; for (i=0; ((i < (timeout * 5)) && !(cond)); i++);}
#else
#define sleep(ticks)                      {volatile int i; for (i=0; i < ticks; i++);}
#define waitfor(cond,timeout)             {volatile int i; for (i=0; ((i < timeout) && !(cond)); i++);}
#endif

/*!
 *  \brief Alias for core2base macro
 *  \sa core2base
 */
#define local2global(address)             (core2base(address))
#define local2global_m(address)           (core2base_m(address))
#if defined (TBARM9CORE)
#undef  local2global
#define local2global(address)             local2global_m(address)
#endif

/*!
 *  \brief Alias for base2core macro
 *  \sa base2core
 */
#define global2local(address)             (base2core(address))
#define global2local_m(address)           (base2core_m(address))
#if defined (TBARM9CORE)
#undef  global2local
#define global2local(address)             global2local_m(address)
#endif

/*!
 *  \brief Alias for local2global macro
 *  \sa local2global
 */
#define l2g(a)                            (local2global(a))
#define l2g_m(a)                          (local2global_m(a))
#if defined (TBARM9CORE)
#undef  l2g
#define l2g(a)                            l2g_m(a)
#endif

/*!
 *  \brief Alias for global2local macro
 *  \sa global2local
 */
#define g2l(a)                            (global2local(a))
#define g2l_m(a)                          (global2local_m(a))
#if defined (TBARM9CORE)
#undef  g2l
#define g2l(a)                            g2l_m(a)
#endif

/*! \brief end the test and report
 *  \param status Possible values are PASS/FAIL
 */
void end_test(uint32_t);

void test_main(void);

/* uintptr_t base2core (uint64_t base);  */
/* uint64_t  core2base (uintptr_t core); */
#if defined(R5FCORE)
__attribute__ ((section ("boot")))
#endif
static INLINE ALWAYS_INLINE OPTIMIZE(2) uintptr_t base2core (uint64_t base) {
  return base2core_m (base);
}

#if defined(R5FCORE)
__attribute__ ((section ("boot")))
#endif
static INLINE ALWAYS_INLINE OPTIMIZE(2) uint64_t core2base (uintptr_t core) {
  return core2base_m (core);
}

#endif
