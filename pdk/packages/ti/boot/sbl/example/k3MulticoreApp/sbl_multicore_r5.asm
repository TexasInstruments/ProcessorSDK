;******************************************************************************
;*                                                                            *
;* Copyright (c) 2018-2019 Texas Instruments Incorporated                     *
;* http://www.ti.com/                                                         *
;*                                                                            *
;*  Redistribution and  use in source  and binary forms, with  or without     *
;*  modification,  are permitted provided  that the  following conditions     *
;*  are met:                                                                  *
;*                                                                            *
;*     Redistributions  of source  code must  retain the  above copyright     *
;*     notice, this list of conditions and the following disclaimer.          *
;*                                                                            *
;*     Redistributions in binary form  must reproduce the above copyright     *
;*     notice, this  list of conditions  and the following  disclaimer in     *
;*     the  documentation  and/or   other  materials  provided  with  the     *
;*     distribution.                                                          *
;*                                                                            *
;*     Neither the  name of Texas Instruments Incorporated  nor the names     *
;*     of its  contributors may  be used to  endorse or  promote products     *
;*     derived  from   this  software  without   specific  prior  written     *
;*     permission.                                                            *
;*                                                                            *
;*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS     *
;*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT     *
;*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR     *
;*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT     *
;*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     *
;*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT     *
;*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     *
;*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
;*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT     *
;*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     *
;*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      *
;*                                                                            *
;******************************************************************************

	.arm

	.include sbl_multicore_r5_sections.inc

	.global SBLApp_main

	.global _c_int00
	.global _sblTestResetVectors
	
    .align(128)
_c_int00:
_sblTestResetVectors: LDR pc, sblTestEntry ; Entry
_sblUndefVectors:     LDR pc, sblUndefLoop
_sblSvcVectors:       LDR pc, sblSvcLoop
_sblPabortVectors:    LDR pc, sblPabortLoop
_sblDabortVectors:    LDR pc, sblDabortLoop
_sblHyVectors:        LDR pc, sblHypLoop
_sblIrqVectors:       LDR pc, sblIrqLoop
_sblFiqVectors:       LDR pc, sblFiqLoop

sblTestEntry	.long _sblTestEntry
sblUndefLoop	.long _sblUndefLoop
sblSvcLoop	.long _sblSvcLoop
sblPabortLoop	.long _sblPabortLoop
sblDabortLoop	.long _sblDabortLoop
sblHypLoop	.long _sblHypLoop
sblIrqLoop	.long _sblIrqLoop
sblFiqLoop	.long _sblFiqLoop
	

	.global _sblTestEntry

_sblTestEntry:

	; Setup stack
	ADR	r1, _sblTestStackBase
	BIC	r1, r1, #0xf
	MOV	sp, r1

	LDR	r1, SBLApp_mainAddr
	BLX	r1

	; Test complete.
	WFI
    ADD r0, r0, #0x1
_sbTestDone:	B _sbTestDone

    .global SBLApp_isLockStepModeEnabled
; Checks if cluster is booted in lockstep or not
SBLApp_isLockStepModeEnabled:
    DMB
    MRC       p15, #0, r0, c15, c2, #1  ; Read Build Options 2 Register
    LSL       r0, r0, #(1)
    LSR       r0, r0, #(31)             ; Extract 30th Bit
    BX        lr

SBLApp_mainAddr	.long SBLApp_main

_sblUndefLoop:  B _sblUndefLoop
_sblSvcLoop:    B _sblSvcLoop
_sblPabortLoop: B _sblPabortLoop
_sblDabortLoop: B _sblDabortLoop
_sblHypLoop:    B _sblHypLoop
_sblIrqLoop:    B _sblIrqLoop
_sblFiqLoop:    B _sblFiqLoop	

_sblTestStackTop:
	.space 512
_sblTestStackBase:

	

