/*
 * Cache support for low power functionality
 *
 *
 * Copyright (C) 2024, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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

        .text

/* FUNCTION DEF: void cache_wb_data()
 *
 * Write back all of L1 data cache
 */
        .global cache_wb_data
        .type cache_wb_data,%function
        .section ".text","ax",%progbits
        .arm
        .align 2
cache_wb_data:
        stmfd   sp!, {r0-r7, r9-r11, lr}
        dmb                             // Ensure all previous memory accesses complete
        mov     r10, #0                 // start clean at cache level 0 (L1)
        mrs     r6, cpsr
        cpsid   i                       // disable interrupts
        mrc     p15, #1, r1, c0, c0, #0 // read the csidr
        msr     cpsr_c, r6              // restore interrupts
        and     r2, r1, #7              // extract the length of the cache lines
        add     r2, r2, #4              // add 4 (line length offset)
        mov     r4, #0x3ff
        ands    r4, r4, r1, lsr #3      // find maximum number on the way size
        clz     r5, r4                  // find bit position of way size inc.
        mov     r7, #0x7fff
        ands    r7, r7, r1, lsr #13     // extract max number of the index size
loop_way:
        mov     r9, r4                  // create working copy of max way size
loop_set:
        orr     r11, r10, r9, lsl r5    // factor way and cache number into r11
        orr     r11, r11, r7, lsl r2    // factor index number into r11
        mcr     p15, #0, r11, c7, c10, #2 // clean line by set/way
        subs    r9, r9, #1              // decrement the way
        bge     loop_set
        subs    r7, r7, #1              // decrement the index
        bge     loop_way
        dsb
        ldmfd   sp!, {r0-r7, r9-r11, lr}
        bx      lr

	.end

