;
;  Copyright (c) 2020, Texas Instruments Incorporated
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
; ======== utilsCopyVecs2ATcm.asm ========
; Copies sysbios defined vector to ATM area
;
    .text
    .sect   ".utilsCopyVecsToAtcm"
;==============================================================================
;   void utilsCopyVecs2ATcm( void )
;==============================================================================
    .global ti_sysbios_family_arm_v7r_keystone3_Hwi_vectors

    .global utilsCopyVecs2ATcm
utilsCopyVecs2ATcm:
        
        mrc     p15, #0, r0, c9, c1, #1 ; read ATCM Region Register
        tst     r0, #1
        beq     exit                    ; do nothing if ATCM disabled

        movw    r0, ti_sysbios_family_arm_v7r_keystone3_Hwi_vectors
        movt    r0, ti_sysbios_family_arm_v7r_keystone3_Hwi_vectors
        mov     r1, #0                  ; ATCM address
        mov	r2, #64			; 256 bytes
loop:
        ldr	r3, [r0], #4
        str     r3, [r1], #4
        subs    r2, r2, #4
        bgt     loop
exit:
        bx      lr
        

        .end
