;
;  Copyright (c) 2017-2023, Texas Instruments Incorporated
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
;  ======== Hwi_disp_always.s71 ========
;
;
        .if $isdefed("__TI_ASSEMBLER_VERSION__")
        .if __TI_ASSEMBLER_VERSION__ < 1003000
            .emsg "Compiler version must be 1.3.0 STS or higher"
        .endif
        .endif

        .cdecls C,NOLIST,"Hwi.h"

;
; The following are double word offsets (offset must be < 32 for A-reg access)
;
Hwi_SFA0      .set    1
Hwi_SFA1      .set    2
Hwi_SFA2      .set    3
Hwi_SFA3      .set    4
Hwi_SFA4      .set    5
Hwi_SFA5      .set    6
Hwi_SFA6      .set    7
Hwi_SFA7      .set    8
Hwi_SFAL0     .set    9
Hwi_SFAL1     .set    10
Hwi_SFAL2     .set    11
Hwi_SFAL3     .set    12
Hwi_SFAL4     .set    13
Hwi_SFAL5     .set    14
Hwi_SFAL6     .set    15
Hwi_SFAL7     .set    16
Hwi_SFAM0     .set    17
Hwi_SFAM1     .set    18
Hwi_SFAM2     .set    19
Hwi_SFAM3     .set    20
Hwi_SFAM4     .set    21
Hwi_SFAM5     .set    22
Hwi_SFAM6     .set    23
Hwi_SFAM7     .set    24
Hwi_SFD0      .set    25
Hwi_SFD1      .set    26
Hwi_SFD2      .set    27
Hwi_SFD3      .set    28
Hwi_SFD4      .set    29
Hwi_SFD5      .set    30
Hwi_SFD6      .set    31
Hwi_SFD7      .set    32
Hwi_SFD8      .set    33
Hwi_SFD9      .set    34
Hwi_SFD10     .set    35
Hwi_SFD11     .set    36
Hwi_SFD12     .set    37
Hwi_SFD13     .set    38
Hwi_SFD14     .set    39

Hwi_SFRP      .set    40
Hwi_SFFPCR    .set    41
Hwi_SFFSR     .set    42
Hwi_SFGFPGFR  .set    43
Hwi_SFGPLY    .set    44
Hwi_SFP0      .set    45
Hwi_SFP1      .set    46
Hwi_SFP2      .set    47
Hwi_SFP3      .set    48
Hwi_SFP4      .set    49
Hwi_SFP5      .set    50
Hwi_SFP6      .set    51
Hwi_SFP7      .set    52
Hwi_SFRXMR    .set    53


; 512 bit (64 byte) offsets
Hwi_SFVB0     .set    54
Hwi_SFVB1     .set    Hwi_SFVB0 + (8*1)
Hwi_SFVB2     .set    Hwi_SFVB0 + (8*2)
Hwi_SFVB3     .set    Hwi_SFVB0 + (8*3)
Hwi_SFVB4     .set    Hwi_SFVB0 + (8*4)
Hwi_SFVB5     .set    Hwi_SFVB0 + (8*5)
Hwi_SFVB6     .set    Hwi_SFVB0 + (8*6)
Hwi_SFVB7     .set    Hwi_SFVB0 + (8*7)
Hwi_SFVB8     .set    Hwi_SFVB0 + (8*8)
Hwi_SFVB9     .set    Hwi_SFVB0 + (8*9)
Hwi_SFVB10    .set    Hwi_SFVB0 + (8*10)
Hwi_SFVB11    .set    Hwi_SFVB0 + (8*11)
Hwi_SFVB12    .set    Hwi_SFVB0 + (8*12)
Hwi_SFVB13    .set    Hwi_SFVB0 + (8*13)
Hwi_SFVB14    .set    Hwi_SFVB0 + (8*14)
Hwi_SFVB15    .set    Hwi_SFVB0 + (8*15)
Hwi_SFVBL0    .set    Hwi_SFVB0 + (8*16)
Hwi_SFVBL1    .set    Hwi_SFVB0 + (8*17)
Hwi_SFVBL2    .set    Hwi_SFVB0 + (8*18)
Hwi_SFVBL3    .set    Hwi_SFVB0 + (8*19)
Hwi_SFVBL4    .set    Hwi_SFVB0 + (8*20)
Hwi_SFVBL5    .set    Hwi_SFVB0 + (8*21)
Hwi_SFVBL6    .set    Hwi_SFVB0 + (8*22)
Hwi_SFVBL7    .set    Hwi_SFVB0 + (8*23)
Hwi_SFVBM0    .set    Hwi_SFVB0 + (8*24)
Hwi_SFVBM1    .set    Hwi_SFVB0 + (8*25)
Hwi_SFVBM2    .set    Hwi_SFVB0 + (8*26)
Hwi_SFVBM3    .set    Hwi_SFVB0 + (8*27)
Hwi_SFVBM4    .set    Hwi_SFVB0 + (8*28)
Hwi_SFVBM5    .set    Hwi_SFVB0 + (8*29)
Hwi_SFVBM6    .set    Hwi_SFVB0 + (8*30)
Hwi_SFVBM7    .set    Hwi_SFVB0 + (8*31)
Hwi_SFCUCR0   .set    Hwi_SFVB0 + (8*32)
Hwi_SFCUCR1   .set    Hwi_SFVB0 + (8*33)
Hwi_SFCUCR2   .set    Hwi_SFVB0 + (8*34)
Hwi_SFCUCR3   .set    Hwi_SFVB0 + (8*35)
Hwi_SFSTRACNTR0 .set  Hwi_SFVB0 + (8*36)
Hwi_SFSTRACNTR1 .set  Hwi_SFVB0 + (8*37)
Hwi_SFSTRACNTR2 .set  Hwi_SFVB0 + (8*38)
Hwi_SFSTRACNTR3 .set  Hwi_SFVB0 + (8*39)
Hwi_SFSTRACR0 .set    Hwi_SFVB0 + (8*40)
Hwi_SFSTRACR1 .set    Hwi_SFVB0 + (8*41)
Hwi_SFSTRACR2 .set    Hwi_SFVB0 + (8*42)
Hwi_SFSTRACR3 .set    Hwi_SFVB0 + (8*43)
Hwi_SFSE0_0   .set    Hwi_SFVB0 + (8*44)
Hwi_SFSE0_1   .set    Hwi_SFVB0 + (8*45)
Hwi_SFSE0_2   .set    Hwi_SFVB0 + (8*46)
Hwi_SFSE0_3   .set    Hwi_SFVB0 + (8*47)
Hwi_SFSE1_0   .set    Hwi_SFVB0 + (8*48)
Hwi_SFSE1_1   .set    Hwi_SFVB0 + (8*49)
Hwi_SFSE1_2   .set    Hwi_SFVB0 + (8*50)
Hwi_SFSE1_3   .set    Hwi_SFVB0 + (8*51)

Hwi_STACKFRAMESIZE      .set    Hwi_SFSE1_3 + 8      ; DW (8 byte) size


SP              .set    d15

    .if $isdefed("__TI_ELFABI__")
    .if __TI_ELFABI__
        .asg Hwi_dispatchC, _Hwi_dispatchC
        .asg Hwi_dispatchAlways, _Hwi_dispatchAlways
        .asg Hwi_Module_state, _Hwi_Module_state
    .endif
    .endif

        .global Hwi_dispatchC
        .global Hwi_dispatchAlways
        .global SysCall_syscallHandler
    .if $isdefed("Exception_vectors__D")
        .global Exception_dispatch
    .endif

Hwi_Module_state .tag Hwi_Module_State

        .sect .vecs

        .global soft_reset

soft_reset:
        b.b1      $PCR_OFFSET(soft_reset)

        .align 0x200
int_exc:
    .if $isdefed("Exception_vectors__D")
        std.d2x   a0, *SP++[-1]
||      mv.d1     SP, a0
        std.d1    a1, *SP++[-1]
        mvk64.l1  0x0, a1
        b.b1      $PCR_OFFSET(Exception_dispatch)
    .else
        b.b1      $PCR_OFFSET(int_exc)
    .endif

        .align 0x200
page_fault:
    .if $isdefed("Exception_vectors__D")
        std.d2x   a0, *SP++[-1]
||      mv.d1     SP, a0
        std.d1    a1, *SP++[-1]
        mvk64.l1  0x1, a1
        b.b1      $PCR_OFFSET(Exception_dispatch)
    .else
        b.b1      $PCR_OFFSET(page_fault)
    .endif

        .align 0x200
nme:
        b.b1      $PCR_OFFSET(nme)

        .align 0x200
evt:
        b.b1      $PCR_OFFSET(Hwi_dispatchAlways)

    .if $isdefed("SysCall_vector__D")

        .align 0x1000
scall:
        .align 0x40
        mvk64.l1  0, a0    ; used for enterPrivMode
        b.b1      $PCR_OFFSET(SysCall_syscallHandler)

        .align 0x40
        mvk64.l1  1, a0    ; used for enterUnprivMode
        b.b1      $PCR_OFFSET(SysCall_syscallHandler)

        .align 0x40
        mvk64.l1  2, a0    ; used for restorePrivMode
        b.b1      $PCR_OFFSET(SysCall_syscallHandler)

    .endif

        .sect .secure_vecs

        .global secure_soft_reset

secure_soft_reset:
        b.b1      $PCR_OFFSET(secure_soft_reset)

        .align 0x200
secure_int_exc:
    .if $isdefed("Exception_vectors__D")
        std.d2x   a0, *SP++[-1]
||      mv.d1     SP, a0
        std.d1    a1, *SP++[-1]
        mvk64.l1  0x0, a1
        b.b1      $PCR_OFFSET(Exception_dispatch)
    .else
        b.b1      $PCR_OFFSET(secure_int_exc)
    .endif

        .align 0x200
secure_page_fault:
    .if $isdefed("Exception_vectors__D")
        std.d2x   a0, *SP++[-1]
||      mv.d1     SP, a0
        std.d1    a1, *SP++[-1]
        mvk64.l1  0x1, a1
        b.b1      $PCR_OFFSET(Exception_dispatch)
    .else
        b.b1      $PCR_OFFSET(secure_page_fault)
    .endif

        .align 0x200
secure_nme:
        b.b1      $PCR_OFFSET(secure_nme)

        .align 0x200
secure_evt:
        b.b1      $PCR_OFFSET(Hwi_dispatchAlways)

        .sect ".text:Hwi_dispatchAlways"

        .clink

Hwi_dispatchAlways:
;        .asmfunc

        std.d1    a0, *ECSP[0x300]
||      std.d2x   a1, *ECSP[0x301]

        addd.d1   SP, -Hwi_STACKFRAMESIZE*8, SP     ; alloc stack frame

;;        std.d1    a0, *SP[Hwi_SFA0]    ; already saved
;;||      std.d2x   a1, *SP[Hwi_SFA1]    ; already saved
        std.d1    a2, *SP[Hwi_SFA2]
||      std.d2x   a3, *SP[Hwi_SFA3]
        std.d1    a4, *SP[Hwi_SFA4]
||      std.d2x   a5, *SP[Hwi_SFA5]
        std.d1    a6, *SP[Hwi_SFA6]
||      std.d2x   a7, *SP[Hwi_SFA7]
||      mv.l1     al0, a0
||      mv.s1     al1, a1
        std.d1    a0, *SP[Hwi_SFAL0]
||      std.d2x   a1, *SP[Hwi_SFAL1]
||      mv.l1     al2, a0
||      mv.s1     al3, a1
        std.d1    a0, *SP[Hwi_SFAL2]
||      std.d2x   a1, *SP[Hwi_SFAL3]
||      mv.l1     al4, a0
||      mv.s1     al5, a1
        std.d1    a0, *SP[Hwi_SFAL4]
||      std.d2x   a1, *SP[Hwi_SFAL5]
||      mv.l1     al6, a0
||      mv.s1     al7, a1
        std.d1    a0, *SP[Hwi_SFAL6]
||      std.d2x   a1, *SP[Hwi_SFAL7]
||      mv.m1     am0, a0
        std.d1    a0, *SP[Hwi_SFAM0]
||      mv.m1     am1, a0
        std.d1    a0, *SP[Hwi_SFAM1]
||      mv.m1     am2, a0
        std.d1    a0, *SP[Hwi_SFAM2]
||      mv.m1     am3, a0
        std.d1    a0, *SP[Hwi_SFAM3]
||      mv.m1     am4, a0
        std.d1    a0, *SP[Hwi_SFAM4]
||      mv.m1     am5, a0
        std.d1    a0, *SP[Hwi_SFAM5]
||      mv.m1     am6, a0
        std.d1    a0, *SP[Hwi_SFAM6]
||      mv.m1     am7, a0
        std.d1    a0, *SP[Hwi_SFAM7]
        std.d1    d0, *SP[Hwi_SFD0]
        std.d1    d1, *SP[Hwi_SFD1]
        std.d1    d2, *SP[Hwi_SFD2]
        std.d1    d3, *SP[Hwi_SFD3]
        std.d1    d4, *SP[Hwi_SFD4]
        std.d1    d5, *SP[Hwi_SFD5]
        std.d1    d6, *SP[Hwi_SFD6]
        std.d1    d7, *SP[Hwi_SFD7]
        std.d1    d8, *SP[Hwi_SFD8]
        std.d1    d9, *SP[Hwi_SFD9]
        std.d1    d10, *SP[Hwi_SFD10]
        std.d1    d11, *SP[Hwi_SFD11]
        std.d1    d12, *SP[Hwi_SFD12]
        std.d1    d13, *SP[Hwi_SFD13]
        std.d1    d14, *SP[Hwi_SFD14]

        vst8d.d2  vb0, *SP[Hwi_SFVB0]
        vst8d.d2  vb1, *SP[Hwi_SFVB1]
        vst8d.d2  vb2, *SP[Hwi_SFVB2]
        vst8d.d2  vb3, *SP[Hwi_SFVB3]
        vst8d.d2  vb4, *SP[Hwi_SFVB4]
        vst8d.d2  vb5, *SP[Hwi_SFVB5]
        vst8d.d2  vb6, *SP[Hwi_SFVB6]
        vst8d.d2  vb7, *SP[Hwi_SFVB7]
        vst8d.d2  vb8, *SP[Hwi_SFVB8]
        vst8d.d2  vb9, *SP[Hwi_SFVB9]
        vst8d.d2  vb10, *SP[Hwi_SFVB10]
        vst8d.d2  vb11, *SP[Hwi_SFVB11]
        vst8d.d2  vb12, *SP[Hwi_SFVB12]
        vst8d.d2  vb13, *SP[Hwi_SFVB13]
        vst8d.d2  vb14, *SP[Hwi_SFVB14]
        vst8d.d2  vb15, *SP[Hwi_SFVB15]
||      vmv.s2    vbl0, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBL0]
||      vmv.s2    vbl1, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBL1]
||      vmv.s2    vbl2, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBL2]
||      vmv.s2    vbl3, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBL3]
||      vmv.s2    vbl4, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBL4]
||      vmv.s2    vbl5, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBL5]
||      vmv.s2    vbl6, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBL6]
||      vmv.s2    vbl7, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBL7]
||      vmv.m2    vbm0, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBM0]
||      vmv.m2    vbm1, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBM1]
||      vmv.m2    vbm2, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBM2]
||      vmv.m2    vbm3, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBM3]
||      vmv.m2    vbm4, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBM4]
||      vmv.m2    vbm5, vb0
        vst8d.d2  vb0, *SP[Hwi_SFVBM5]
||      vmv.m2    vbm6, vb0
||      mvc.c2    cucr0, vb1                  ; 2 cycle latency for CUCR regs
        vst8d.d2  vb0, *SP[Hwi_SFVBM6]
||      vmv.m2    vbm7, vb0
||      mvc.c2    cucr1, vb2
        vst8d.d2  vb0, *SP[Hwi_SFVBM7]
||      mvc.s1    rp, a0
||      mvc.c2    cucr2, vb3
        std.d1    a0, *SP[Hwi_SFRP]
||      vst8d.d2  vb1, *SP[Hwi_SFCUCR0]
||      mvc.s1    fpcr, a0
||      mvc.c2    cucr3, vb4
        std.d1    a0, *SP[Hwi_SFFPCR]
||      vst8d.d2  vb2, *SP[Hwi_SFCUCR1]
||      mvc.s1    fsr, a0
        std.d1    a0, *SP[Hwi_SFFSR]
||      vst8d.d2  vb3, *SP[Hwi_SFCUCR2]
||      mvc.s1    gfpgfr, a0
        std.d1    a0, *SP[Hwi_SFGFPGFR]
||      vst8d.d2  vb4, *SP[Hwi_SFCUCR3]
||      mvc.s1    gply, a0
        std.d1    a0, *SP[Hwi_SFGPLY]
||      mvpb.l2   p0, b0
||      mvpb.s2   p1, b1
        std.d1x   b0, *SP[Hwi_SFP0]
||      std.d2    b1, *SP[Hwi_SFP1]
||      mvpb.l2   p2, b2
||      mvpb.s2   p3, b3
        std.d1x   b2, *SP[Hwi_SFP2]
||      std.d2    b3, *SP[Hwi_SFP3]
||      mvpb.l2   p4, b4
||      mvpb.s2   p5, b5
        std.d1x   b4, *SP[Hwi_SFP4]
||      std.d2    b5, *SP[Hwi_SFP5]
||      mvpb.l2   p6, b6
||      mvpb.s2   p7, b7
        std.d1x   b6, *SP[Hwi_SFP6]
||      std.d2    b7, *SP[Hwi_SFP7]
||      mvc.s1    rxmr_s, a0
        std.d1    a0, *SP[Hwi_SFRXMR]
||      mvc.c2    sa0cntr0, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSTRACNTR0]
||      mvc.c2    sa1cntr0, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSTRACNTR1]
||      mvc.c2    sa2cntr0, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSTRACNTR2]
||      mvc.c2    sa3cntr0, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSTRACNTR3]
||      mvc.c2    sa0cr, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSTRACR0]
||      mvc.c2    sa1cr, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSTRACR1]
||      mvc.c2    sa2cr, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSTRACR2]
||      mvc.c2    sa3cr, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSTRACR3]
||      sesave.l2 3, 1, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSE1_3]
||      sesave.l2 2, 1, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSE1_2]
||      sesave.l2 1, 1, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSE1_1]
||      sesave.l2 0, 1, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSE1_0]
||      sesave.l2 3, 0, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSE0_3]
||      sesave.l2 2, 0, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSE0_2]
||      sesave.l2 1, 0, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSE0_1]
||      sesave.l2 0, 0, vb15
        vst8d.d2 vb15, *SP[Hwi_SFSE0_0]

        mvc.s1    AHPEE, a4
        addkpc.d1 $PCR_OFFSET(Hwi_dispatchC), a3
        calla.s1     a3

        seclose.d2 0
        seclose.d2 1

        vld8d.d1 *SP[Hwi_SFSE0_3], vb15
        serstr.d2 3, 0, vb15
||      vld8d.d1 *SP[Hwi_SFSE0_2], vb15
        serstr.d2 2, 0, vb15
||      vld8d.d1 *SP[Hwi_SFSE0_1], vb15
        serstr.d2 1, 0, vb15
||      vld8d.d1 *SP[Hwi_SFSE0_0], vb15
        serstr.d2 0, 0, vb15
||      vld8d.d1 *SP[Hwi_SFSE1_3], vb15
        serstr.d2 3, 1, vb15
||      vld8d.d1 *SP[Hwi_SFSE1_2], vb15
        serstr.d2 2, 1, vb15
||      vld8d.d1 *SP[Hwi_SFSE1_1], vb15
        serstr.d2 1, 1, vb15
||      vld8d.d1 *SP[Hwi_SFSE1_0], vb15
        serstr.d2 0, 1, vb15

        saclose.c2 0
        saclose.c2 1
        saclose.c2 2
        saclose.c2 3

        vld8d.d1 *SP[Hwi_SFSTRACNTR0], vb15
        mvc.c2    vb15, sa0cntr0
||      vld8d.d1 *SP[Hwi_SFSTRACNTR1], vb15
        mvc.c2    vb15, sa1cntr0
||      vld8d.d1 *SP[Hwi_SFSTRACNTR2], vb15
        mvc.c2    vb15, sa2cntr0
||      vld8d.d1 *SP[Hwi_SFSTRACNTR3], vb15
        mvc.c2    vb15, sa3cntr0
||      vld8d.d1 *SP[Hwi_SFSTRACR0], vb15
        mvc.c2    vb15, sa0cr
||      vld8d.d1 *SP[Hwi_SFSTRACR1], vb15
        mvc.c2    vb15, sa1cr
||      vld8d.d1 *SP[Hwi_SFSTRACR2], vb15
        mvc.c2    vb15, sa2cr
||      vld8d.d1 *SP[Hwi_SFSTRACR3], vb15
        mvc.c2    vb15, sa3cr

        ldd.d1    *SP[Hwi_SFRXMR], a0
        ldd.d1    *SP[Hwi_SFP7], b7
||      mvc.s1    a0, rxmr_s
        ldd.d1    *SP[Hwi_SFP6], b6
||      mv.l2     b7, p7
        ldd.d1    *SP[Hwi_SFP5], b5
||      mv.l2     b6, p6
        ldd.d1    *SP[Hwi_SFP4], b4
||      mv.l2     b5, p5
        ldd.d1    *SP[Hwi_SFP3], b3
||      mv.l2     b4, p4
        ldd.d1    *SP[Hwi_SFP2], b2
||      mv.l2     b3, p3
        ldd.d1    *SP[Hwi_SFP1], b1
||      mv.l2     b2, p2
        ldd.d1    *SP[Hwi_SFP0], b0
||      mv.l2     b1, p1
        vld8d.d1  *SP[Hwi_SFCUCR3], vb0
||      mv.l2     b0, p0
        vld8d.d1  *SP[Hwi_SFCUCR2], vb0
||      mvc.c2    vb0, cucr3
        vld8d.d1  *SP[Hwi_SFCUCR1], vb0
||      mvc.c2    vb0, cucr2
        vld8d.d1  *SP[Hwi_SFCUCR0], vb0
||      mvc.c2    vb0, cucr1
        ldd.d1    *SP[Hwi_SFGPLY], a0
||      mvc.c2    vb0, cucr0
        ldd.d1    *SP[Hwi_SFGFPGFR], a0
||      mvc.s1    a0, gply
        ldd.d1    *SP[Hwi_SFRP], a0
||      mvc.s1    a0, gfpgfr
        ldd.d1    *SP[Hwi_SFFPCR], a0
||      mvc.s1    a0, rp
        ldd.d1    *SP[Hwi_SFFSR], a0
||      mvc.s1    a0, fpcr
        mvc.s1    a0, fsr

        vld8d.d1  *SP[Hwi_SFVBM7], vb0
        vmv.m2    vb0, vbm7
||      vld8d.d1  *SP[Hwi_SFVBM6], vb0
        vmv.m2    vb0, vbm6
||      vld8d.d1  *SP[Hwi_SFVBM5], vb0
        vmv.m2    vb0, vbm5
||      vld8d.d1  *SP[Hwi_SFVBM4], vb0
        vmv.m2    vb0, vbm4
||      vld8d.d1  *SP[Hwi_SFVBM3], vb0
        vmv.m2    vb0, vbm3
||      vld8d.d1  *SP[Hwi_SFVBM2], vb0
        vmv.m2    vb0, vbm2
||      vld8d.d1  *SP[Hwi_SFVBM1], vb0
        vmv.m2    vb0, vbm1
||      vld8d.d1  *SP[Hwi_SFVBM0], vb0
        vmv.m2    vb0, vbm0
||      vld8d.d1  *SP[Hwi_SFVBL7], vb0
        vmv.m2    vb0, vbl7
||      vld8d.d1  *SP[Hwi_SFVBL6], vb0
        vmv.m2    vb0, vbl6
||      vld8d.d1  *SP[Hwi_SFVBL5], vb0
        vmv.m2    vb0, vbl5
||      vld8d.d1  *SP[Hwi_SFVBL4], vb0
        vmv.m2    vb0, vbl4
||      vld8d.d1  *SP[Hwi_SFVBL3], vb0
        vmv.m2    vb0, vbl3
||      vld8d.d1  *SP[Hwi_SFVBL2], vb0
        vmv.m2    vb0, vbl2
||      vld8d.d1  *SP[Hwi_SFVBL1], vb0
        vmv.m2    vb0, vbl1
||      vld8d.d1  *SP[Hwi_SFVBL0], vb0
        vmv.m2    vb0, vbl0
||      vld8d.d1  *SP[Hwi_SFVB15], vb15
        vld8d.d1  *SP[Hwi_SFVB14], vb14
        vld8d.d1  *SP[Hwi_SFVB13], vb13
        vld8d.d1  *SP[Hwi_SFVB12], vb12
        vld8d.d1  *SP[Hwi_SFVB11], vb11
        vld8d.d1  *SP[Hwi_SFVB10], vb10
        vld8d.d1  *SP[Hwi_SFVB9], vb9
        vld8d.d1  *SP[Hwi_SFVB8], vb8
        vld8d.d1  *SP[Hwi_SFVB7], vb7
        vld8d.d1  *SP[Hwi_SFVB6], vb6
        vld8d.d1  *SP[Hwi_SFVB5], vb5
        vld8d.d1  *SP[Hwi_SFVB4], vb4
        vld8d.d1  *SP[Hwi_SFVB3], vb3
        vld8d.d1  *SP[Hwi_SFVB2], vb2
        vld8d.d1  *SP[Hwi_SFVB1], vb1
        vld8d.d1  *SP[Hwi_SFVB0], vb0

        ldd.d1    *SP[Hwi_SFD14], d14
        ldd.d1    *SP[Hwi_SFD13], d13
        ldd.d1    *SP[Hwi_SFD12], d12
        ldd.d1    *SP[Hwi_SFD11], d11
        ldd.d1    *SP[Hwi_SFD10], d10
        ldd.d1    *SP[Hwi_SFD9], d9
        ldd.d1    *SP[Hwi_SFD8], d8
        ldd.d1    *SP[Hwi_SFD7], d7
        ldd.d1    *SP[Hwi_SFD6], d6
        ldd.d1    *SP[Hwi_SFD5], d5
        ldd.d1    *SP[Hwi_SFD4], d4
        ldd.d1    *SP[Hwi_SFD3], d3
        ldd.d1    *SP[Hwi_SFD2], d2
        ldd.d1    *SP[Hwi_SFD1], d1
        ldd.d1    *SP[Hwi_SFD0], d0
        ldd.d1    *SP[Hwi_SFAM7], a0
        mv.m1     a0, am7
||      ldd.d1    *SP[Hwi_SFAM6], a0
        mv.m1     a0, am6
||      ldd.d1    *SP[Hwi_SFAM5], a0
        mv.m1     a0, am5
||      ldd.d1    *SP[Hwi_SFAM4], a0
        mv.m1     a0, am4
||      ldd.d1    *SP[Hwi_SFAM3], a0
        mv.m1     a0, am3
||      ldd.d1    *SP[Hwi_SFAM2], a0
        mv.m1     a0, am2
||      ldd.d1    *SP[Hwi_SFAM1], a0
        mv.m1     a0, am1
||      ldd.d1    *SP[Hwi_SFAM0], a0
        mv.m1     a0, am0
||      ldd.d1    *SP[Hwi_SFAL7], a0
        mv.m1     a0, al7
||      ldd.d1    *SP[Hwi_SFAL6], a0
        mv.m1     a0, al6
||      ldd.d1    *SP[Hwi_SFAL5], a0
        mv.m1     a0, al5
||      ldd.d1    *SP[Hwi_SFAL4], a0
        mv.m1     a0, al4
||      ldd.d1    *SP[Hwi_SFAL3], a0
        mv.m1     a0, al3
||      ldd.d1    *SP[Hwi_SFAL2], a0
        mv.m1     a0, al2
||      ldd.d1    *SP[Hwi_SFAL1], a0
        mv.m1     a0, al1
||      ldd.d1    *SP[Hwi_SFAL0], a0
        mv.m1     a0, al0
        ldd.d1    *SP[Hwi_SFA7], a7
        ldd.d1    *SP[Hwi_SFA6], a6
        ldd.d1    *SP[Hwi_SFA5], a5
        ldd.d1    *SP[Hwi_SFA4], a4
        ldd.d1    *SP[Hwi_SFA3], a3
        ldd.d1    *SP[Hwi_SFA2], a2
;;        ldd.d1    *SP[Hwi_SFA1], a1    ; restored below
;;        ldd.d1    *SP[Hwi_SFA0], a0    ; restored below

        addd.d1   SP, Hwi_STACKFRAMESIZE*8, SP     ; free stack frame

        ldd.d1    *ECSP[0], a0
||      ldd.d2    *ECSP[1], a1
        rete.s1   a0, a1
||      ldd.d1    *ECSP[0x300], a0
||      ldd.d2    *ECSP[0x301], a1

;        .endasmfunc
