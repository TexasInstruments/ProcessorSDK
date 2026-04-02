******************************************************************************
;  @file  csl_arm_r5_readback_check.asm
;
;  @brief
;   Implementation file for the ARM R5 registers readback check.
;
;   \par
;   ============================================================================
;   @n   (C) Copyright 2026, Texas Instruments, Inc.
;
;   Redistribution and use in source and binary forms, with or without
;   modification, are permitted provided that the following conditions
;   are met:
;
;     Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
;
;     Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the
;     distribution.
;
;     Neither the name of Texas Instruments Incorporated nor the names of
;     its contributors may be used to endorse or promote products derived
;     from this software without specific prior written permission.
;
;   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
;   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;  LOSS OF USE,
;   DATA, OR PROFITS;  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
;   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;******************************************************************************

;==============================================================================
;  RB_CHK regVal, opc1, CRn, CRm, opc2
;==============================================================================
; Description: Read register value from memory, compare to regVal. If not equal
;              call RB_CHK_CB, if equal do nothing.
; Input Parameters: regVal, opc1, CRn, CRm, opc2
;   regVal : register holding expected value
;   opc1, CRn, CRm, opc2 : passed directly into MRC
; Output Parameters: none
;------------------------------------------------------------------------------
RB_CHK  .macro  opc1, regVal, CRn, CRm, opc2

    .if !$$defined(RB_CHK_CB)
        .ref CSL_armReadbackError
        .asg "CSL_armReadbackError", RB_CHK_CB
    .endif

    .if SM_READBACK_ENABLED                     ; Assembled only if symbol is nonzero
        DSB
        PUSH    {r7, lr}
        MRC     p15, opc1, r7, CRn, CRm, opc2   ; Reads specific r5f register
        CMP     r7, regVal                      ; Compare expected and actual value
        BEQ     RB_CHK_PASS?

        PUSH    {r0, r1}
        MOV     r0, regVal                      ; Expected value
        MOV     r1, r7                          ; Actual value
        BLX     RB_CHK_CB
        POP     {r0, r1}

RB_CHK_PASS?:
    POP     {r7, lr}

    .endif
    .endm
