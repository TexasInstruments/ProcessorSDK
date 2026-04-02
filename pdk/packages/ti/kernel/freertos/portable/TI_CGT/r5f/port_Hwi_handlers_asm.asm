;
;  Copyright (c) 2018-2021, Texas Instruments Incorporated
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
; ======== handlers_nonos.asm ========
;

    .armfunc _freertosresetvectors

;******************************************************
;* UNDEFINED REFERENCES                               *
;******************************************************

    .global	_c_int00
    .global	HwiP_undefined_handler
    .global	HwiP_svc_handler
    .global	HwiP_prefetch_abort_handler
    .global	HwiP_data_abort_handler
    .global	HwiP_irq_handler
    .global	HwiP_fiq_handler
    .global	HwiP_reserved_handler

;****************************************************************************
; Setup Reset Vectors always in ARM mode
;****************************************************************************

    .arm
	.global	_freertosresetvectors
    .sect   ".freertosrstvectors"
_freertosresetvectors:
    
        LDR pc, c_int00_addr        ; Reset
        LDR pc, undefInst_addr      ; Undefined Instruction
        LDR pc, swi_addr            ; Software interrupt
        LDR pc, preAbort_addr       ; Abort (prefetch)
        LDR pc, dataAbort_addr      ; Abort (data)
        LDR pc, rsvd_addr           ; rsvd
        LDR pc, irq_addr            ; IRQ
        LDR pc, fiq_addr            ; FIQ
    


c_int00_addr .long _c_int00
undefInst_addr .long HwiP_undefined_handler
swi_addr .long HwiP_svc_handler
preAbort_addr .long HwiP_prefetch_abort_handler
dataAbort_addr .long HwiP_data_abort_handler
rsvd_addr .long HwiP_reserved_handler
irq_addr .long HwiP_irq_handler
fiq_addr .long HwiP_fiq_handler
	.end
