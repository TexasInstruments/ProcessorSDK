/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
    DDR_C66X_0:         o = 0x90000000 l = 0x900000
    DDR_C66X_1:         o = 0x90900000 l = 0x900000
    DDR_DSP1_C7X:       o = 0x91200000 l = 0x900000
}

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x1000                              /* HEAP AREA SIZE                */
--symbol_map _Hwi_intcVectorTable=Hwi_intcVectorTable


SECTIONS
{    
    .hwi_vect: {. = align(32); } > DDR_C66X_1
    .text:csl_entry:{}           > DDR_C66X_1
    .text:_c_int00          load > DDR_C66X_1 ALIGN(0x10000)
    .text:                  load > DDR_C66X_1
    .stack:                 load > DDR_C66X_1
    .sbl_c66x_1_resetvector load > DDR_C66X_1
    GROUP:                  load > DDR_C66X_1
    {
        .bss:
        .neardata:
        .rodata:
    }
    .cio:                   load >  DDR_C66X_1
    .const:                 load >  DDR_C66X_1
    .data:                  load >  DDR_C66X_1
    .switch:                load >  DDR_C66X_1
    .sysmem:                load >  DDR_C66X_1
    .far:                   load >  DDR_C66X_1
    .args:                  load >  DDR_C66X_1

    /* COFF sections */
    .pinit:                 load >  DDR_C66X_1
    .cinit:                 load >  DDR_C66X_1

    /* EABI sections */
    .binit:                 load >  DDR_C66X_1
    .init_array:            load >  DDR_C66X_1
    .fardata:               load >  DDR_C66X_1
    .c6xabi.exidx:          load >  DDR_C66X_1
    .c6xabi.extab:          load >  DDR_C66X_1

    .csl_vect:              load >  DDR_C66X_1
}
