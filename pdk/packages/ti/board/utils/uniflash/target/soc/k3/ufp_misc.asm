;******************************************************************************
;*                                                                            *
;* Copyright (c) 2020 Texas Instruments Incorporated                          *
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

;****************************************************************************
; UFP Read ATCM Region Register
;****************************************************************************
	.sect   ".text"
	.global ufpTcmEn

ufpTcmEn:
	
    ;Enable ATCM @0x0
    MRC     p15, #0, r0, c9, c1, #1
    BFC     r0, #12, #20
    ORR     r0, r0, #0x1
    MCR     p15, #0, r0, c9, c1, #1

    ;Enable BTCM @0x41010000
    LDR     r1, ufp_btcm_base
    MRC     p15, #0, r0, c9, c1, #0
    BFC     r0, #12, #20
    ORR     r0, r0, r1
    ORR     r0, r0, #0x1
    MCR     p15, #0, r0, c9, c1, #0

    BX      lr

ufp_btcm_base .word 0x41010000

	


;****************************************************************************
; UFP Read ATCM Size Register
;****************************************************************************
	.sect   ".text"
	.global ufpAtcmSize

ufpAtcmSize:
	
    mrc     p15, #0, r0, c9, c1, #1     ;; Read ATCM region Register
    and     r0, r0, #0xFF               ;; Extract  ATCM region Register
    lsr     r0, r0, #0x2                ;; Extract  ATCM region Register
    mov     r1, #0x200                  ;; Calculate size of ATCM in bytes
    lsl     r0, r1, r0                  ;; Calculate size of ATCM in bytes

    BX      lr

	

;****************************************************************************
; UFP Read BTCM Size Register
;****************************************************************************
	.sect   ".text"
	.global ufpBtcmSize

ufpBtcmSize:
	
    mrc     p15, #0, r0, c9, c1, #0      ;; Read BTCM region Register
    and     r0, r0, #0xFF                ;; Extract  BTCM region Register
    lsr     r0, r0, #0x2                 ;; Extract  BTCM region Register
    mov     r1, #0x200                   ;; Calculate size of BTCM in bytes
    lsl     r0, r1, r0                   ;; Calculate size of BTCM in bytes

    BX      lr

    .sect   ".utilsCopyVecsToAtcm"
    .global _resetvectors

    .global utilsCopyVecs2ATcm
utilsCopyVecs2ATcm:
        mrc     p15, #0, r0, c9, c1, #1 ; read ATCM Region Register
        tst     r0, #1
        beq     exit                    ; do nothing if ATCM disabled

        movw    r0, _resetvectors
        movt    r0, _resetvectors
        mov     r1, #0                  ; ATCM address
        mov	r2, #64			; 256 bytes
loop:
        ldr	r3, [r0], #4
        str     r3, [r1], #4
        subs    r2, r2, #4
        bgt     loop
exit:
        bx      lr


;****************************************************************************
; Supress Klockworks Ptr Errors
;****************************************************************************
	.sect   ".text"
	.global uint32_to_void_ptr
	.global uint32_to_const_void_ptr
	.global uint32_to_uint32_ptr
	.global uint32_ptr_to_void_ptr
	.global const_uint8_ptr_to_void_ptr
	.global uint32_to_int32
	.global uint64_to_uint32
	.global uint64_to_int32

uint32_to_int32:
uint32_to_void_ptr:
uint32_to_const_void_ptr:
uint32_to_uint32_ptr:
uint32_ptr_to_void_ptr:
const_uint8_ptr_to_void_ptr:
uint64_to_uint32:
uint64_to_int32:

	

	BX	lr	
	

