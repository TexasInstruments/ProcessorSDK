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
; ======== utilsEnableTcmCopyVecs2ATcm.asm ========
; Enables ATCM & BTCM if not enabled and initializes same
;   Copies the vector to ATCM
;
    .text
    .sect   ".utilsCopyVecsToAtcm" ; No special sections required, reusing
;==============================================================================
;   void utilsCopyVecs2ATcm( void )
;==============================================================================
    ; Global variables/functions used
    .global ti_sysbios_family_arm_v7r_keystone3_Hwi_vectors
    .ref memset

    .global utilsCopyVecs2ATcm
utilsCopyVecs2ATcm:
        
        push    {lr}                    ; Remmember the caller
        mrc     p15, #0, r0, c9, c1, #1 ; read ATCM Region Register
        tst     r0, #1
        bne     reset_atcm              ; reset ATCM if enabled

        ; enable ATCM
        mrc     p15, #0, r0, c9, c1, #1
        bfc     r0, #12, #20
        orr     r0, r0, #0x1
        mcr     p15, #0, r0, c9, c1, #1

reset_atcm:
        ; reset ATCM
        mov     r0, #0      ; ATCM Base
        mov     r1, #170    ; decimal value for 0xAA
        mov     r2, #100    ; size of ATCM, increase this if required
        bl      memset


        mrc     p15, #0, r0, c9, c1, #1 ; read BTCM Region Register
        tst     r0, #1
        bne     reset_btcm              ; reset BTCM if enabled

        ; enable BTCM
        ldr     r1, sbl_btcm_base
        mrc     p15, #0, r0, c9, c1, #0
        bfc     r0, #12, #20
        orr     r0, r0, r1
        orr     r0, r0, #0x1
        mcr     p15, #0, r0, c9, c1, #0

reset_btcm:
        ; reset BTCM
        ldr     r0, sbl_btcm_base   ; BTCM Base
        mov     r1, #85             ; decimal value for 0x55
        mov     r2, #100            ; size of BTCM, increase this if required
        bl      memset

        ; Copy sysbios vecs to ATCM
        mrc     p15, #0, r0, c9, c1, #1 ; read ATCM Region Register
        tst     r0, #1
        beq     exit                    ; do nothing if ATCM disabled

        movw    r0, ti_sysbios_family_arm_v7r_keystone3_Hwi_vectors
        movt    r0, ti_sysbios_family_arm_v7r_keystone3_Hwi_vectors
        mov     r1, #0                  ; ATCM address
        mov     r2, #64                 ; 256 bytes
loop:
        ldr     r3, [r0], #4
        str     r3, [r1], #4
        subs    r2, r2, #4
        bgt     loop

exit:
        pop     {lr}
        bx      lr

sbl_btcm_base .word 0x41010000

        

        .end
