;******************************************************************************
;*                                                                            *
;* Copyright (c) 2021 Texas Instruments Incorporated                          *
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
;****************************************************************************
; Setup Reset Vectors
;****************************************************************************
	.arm

	.sect   ".rstvectors"
	.global _keywriterResetVectors

_keywriterResetVectors:
	LDR pc, keywriterEntry ; Reset
	B KeywrErrLoop ; Undefined Instruction
	B KeywrErrLoop ; SVC call
	B KeywrErrLoop ; Prefetch abort
	B KeywrErrLoop ; Data abort
	B KeywrErrLoop ; Hypervisor
	B KeywrErrLoop ; IRQ
	B KeywrErrLoop ; FIQ

keywriterEntry	.long _keywriterEntry

;****************************************************************************
; Keywriter Entry
;****************************************************************************
	.def	_keywriterEntry
	.global KeywrErrLoop
	.ref	_c_int00

_c_int00_addr		.long _c_int00

_keywriterEntry:

	MRC	p15, #0, r1, c0, c0, #5
	BFC	r1, #8, #24
	CMP	r1, #0
	BNE	KeywrErrLoop

	LDR	r1, _c_int00_addr
	BLX	r1

KeywrErrLoop:
	WFI
	B	KeywrErrLoop

; Certificate will be appended to keywriter binary, 
; keywr_end provides a way to get the address of the certificate
	.sect ".keywr_bin_end"
	.global keywr_end
	.def keywr_end
keywr_end	.long 0xC0FFEEEE
