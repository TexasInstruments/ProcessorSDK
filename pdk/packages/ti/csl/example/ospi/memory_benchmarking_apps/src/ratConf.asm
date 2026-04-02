;
;  Copyright (c) 2021, Texas Instruments Incorporated
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
; ======== ratConf.asm ========
; Sets the RAT for the main domain to map 40-bit ocmram address to 32-bit space
;
    .text
    .sect   ".ratConf"
;==============================================================================
;   void ratConf( void )
;==============================================================================
    .global ti_sysbios_family_arm_v7r_keystone3_Hwi_vectors

    .global ratConf

ratConf:
        
        push.w  {r0, r1, r5, r7}
        movw    r0, #0x0040
        movt    r0, #0x0FF9
        movw    r1, #0x0013
        movt    r1, #0x8000
        str     r1, [r0]

        movw    r1, #0x0000
        movt    r1, #0xD000
        str     r1, [r0, #0x4]

        movw    r1, #0x0000
        movt    r1, #0x0200
        str     r1, [r0, #0x8]

        movw    r1, #0x004F
        movt    r1, #0x0000
        str     r1, [r0, #0xc]
        pop.w   {r0, r1, r5, r7}

        beq     exit
exit:
        bx      lr
        

        .end

