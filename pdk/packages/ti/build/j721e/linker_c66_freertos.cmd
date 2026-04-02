/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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

MEMORY
{
    L2SRAM:       o = 0x00800000 l = 0x00048000   /* 288KB LOCAL L2/SRAM */
    L1PSRAM:      o = 0x00E00000 l = 0x00008000   /* 32KB LOCAL L1P/SRAM */
    L1DSRAM:      o = 0x00F00000 l = 0x00008000   /* 32KB LOCAL L1D/SRAM */
    DDR0:         o = 0x80000000 l = 0x20000000   /* 512MB DDR0 NAVSS */
    /* j721e MSMC3 Memory													  */
    /* j721e Reserved Memory for ARM Trusted Firmware                         */
    MSMC3_ARM_FW   (RWIX)   : o = 0x70000000, l = 0x20000       /* 128KB */
    MSMC3 (RWIX)            : o = 0x70020000  l = 0x7D0000      /* 8MB - 192KB */
    /* j721e Reserved Memory for DMSC Firmware                                */
    MSMC3_DMSC_FW  (RWIX)   : o = 0x707F0000, l = 0x10000       /* 64KB */
}

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x1000                              /* HEAP AREA SIZE                */
--symbol_map _Hwi_intcVectorTable=Hwi_intcVectorTable


SECTIONS
{    
    .hwi_vect: {. = align(32); } > 0x80000000
    .text:csl_entry:{} > DDR0
    .text:_c_int00      load >  DDR0 ALIGN(0x10000)
    .text:              load >  DDR0
    .stack:             load >  DDR0
    GROUP:              load >  DDR0
    {
        .bss:
        .neardata:
        .rodata:
    }
    .cio:               load >  DDR0
    .const:             load >  DDR0
    .data:              load >  DDR0
    .switch:            load >  DDR0
    .sysmem:            load >  DDR0
    .far:               load >  DDR0
    .args:              load >  DDR0

    /* COFF sections */
    .pinit:             load >  DDR0
    .cinit:             load >  DDR0

    /* EABI sections */
    .binit:             load >  DDR0
    .init_array:        load >  DDR0
    .fardata:           load >  DDR0
    .c6xabi.exidx:      load >  DDR0
    .c6xabi.extab:      load >  DDR0

    .csl_vect:          load >  L2SRAM
}
