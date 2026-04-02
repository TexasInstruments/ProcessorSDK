;
;  Copyright (c) 2021 - 2023, Texas Instruments Incorporated
;  All rights reserved.
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions
;  are met:
;
;  *  Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
;
;  *  Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
;
;  *  Neither the name of Texas Instruments Incorporated nor the names of
;     its contributors may be used to endorse or promote products derived
;     from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
;  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
;  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
;  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
;  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
;  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
;  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
;  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
;  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
;  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;
; ======== Cache_asm.s71 ========
;
;

        .global Cache_getL2CFG
        .sect ".text:Cache_getL2CFG"
        .clink
Cache_getL2CFG:
;        .asmfunc

        mvc.s1   L2CFG, a4
||      ret.b1

;	.endasmfunc


        .global Cache_getL1DCFG
        .sect ".text:Cache_getL1DCFG"
        .clink
Cache_getL1DCFG:
;        .asmfunc

        mvc.s1   L1DCFG, a4
||      ret.b1

;	.endasmfunc


        .global Cache_setL2CFG
        .sect ".text:Cache_setL2CFG"
        .clink
Cache_setL2CFG:
;        .asmfunc

        mvc.s1   a4, L2CFG
||      ret.b1

;	.endasmfunc


        .global Cache_setL1DCFG
        .sect ".text:Cache_setL1DCFG"
        .clink
Cache_setL1DCFG:
;        .asmfunc

        mvc.s1   a4, L1DCFG
||      ret.b1

;	.endasmfunc


        .global Cache_setL2WB
        .sect ".text:Cache_setL2WB"
        .clink
Cache_setL2WB:
;        .asmfunc

        mvc.s1   a4, L2WB
||      ret.b1

;	.endasmfunc


        .global Cache_setL2WBINV
        .sect ".text:Cache_setL2WBINV"
        .clink
Cache_setL2WBINV:
;        .asmfunc

        mvc.s1   a4, L2WBINV
||      ret.b1

;	.endasmfunc


        .global Cache_setL1DWB
        .sect ".text:Cache_setL1DWB"
        .clink
Cache_setL1DWB:
;        .asmfunc

        mvc.s1   a4, L1DWB
||      ret.b1

;	.endasmfunc


        .global Cache_setL1DWBINV
        .sect ".text:Cache_setL1DWBINV"
        .clink
Cache_setL1DWBINV:
;        .asmfunc

        mvc.s1   a4, L1DWBINV
||      ret.b1

;	.endasmfunc
