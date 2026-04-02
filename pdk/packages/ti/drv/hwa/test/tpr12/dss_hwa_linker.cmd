/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define L1P_CACHE_SIZE (16*1024)
#define L1D_CACHE_SIZE (16*1024)

MEMORY
{
PAGE 0:

#if (L1P_CACHE_SIZE < 0x8000)
    L1PSRAM:        o = 0x00E00000, l = (0x00008000 - L1P_CACHE_SIZE)
#endif
#if (L1D_CACHE_SIZE < 0x8000)
    L1DSRAM:        o = 0x00F00000, l = (0x00008000 - L1D_CACHE_SIZE)
#endif
    L2SRAM:   o = 0x00800000, l = 0x00060000
    L3SRAM:   o = 0x88000000, l = 0x00300000
    HWA_RAM : o = 0x28000000, l = 0x00020000

    /* PAGEs 1 and onwards are for overlay purposes for memory optimization.
       Some examples:
       1. Overlay one-time only text with uninitialized data.
       2. Overlay L1PSRAM data path processing fast code and use copy tables
          to page in (before entering data path) and out of L1PSRAM (when entering
          sleep/low power).
    */
PAGE 1:
    L3SRAM:   o = 0x88000000, l = 0x00300000
}

/* Set L1D, L1P and L2 Cache Sizes */
ti_sysbios_family_c66_Cache_l1dSize = L1D_CACHE_SIZE;
ti_sysbios_family_c66_Cache_l1pSize = L1P_CACHE_SIZE;
ti_sysbios_family_c66_Cache_l2Size  = 0;

--retain="*(.intvecs)"

SECTIONS
{
    /* hard addresses forces vecs to be allocated there */
    .vecs:  {. = align(32); } > 0x00800000


    .fardata:  {} > L2SRAM
    .const:    {} > L2SRAM
    .switch:   {} > L2SRAM
    .cio:      {} > L2SRAM
    .data:     {} > L2SRAM
    .stack:    {} > L2SRAM
    .cinit:    {} > L2SRAM
    .far:      {} > L2SRAM

     GROUP
    {
       .rodata
       .bss
       .neardata
    } > L2SRAM

    .text: {} > L2SRAM

	 systemHeap :    { } > L2SRAM
    .benchmarkL2    : { } > L2SRAM
	.benchmarkL1     : { } > L1DSRAM
	.FFT4KtestVector : { } > L3SRAM

}

