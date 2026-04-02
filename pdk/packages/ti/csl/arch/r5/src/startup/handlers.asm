;******************************************************************************
;* Copyright (c) 2023 Texas Instruments Incorporated                          *
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
;* handlers.asm

    .text
    .arm
;***************************************************************
;* FUNCTION DEF: masterIsr
;***************************************************************
    .global masterIsr
    .sect ".text.hwi"
    ; Whenever this exception occurs, link register(lr) is loaded
    ; with (IA)+4 in both arm and thumb mode. Here IA is the address of the
    ; instruction which was not executed because of IRQ/FIQ took priority.
    ;
    ; So to execute the instruction which was not executed due to
    ; interrupt, subtract 4 from lr
masterIsr:

    ; Return to the interrupted instruction.
    SUB     lr, lr, #4

    ; Push the return address and SPSR.
    PUSH    {lr}
    MRS     lr, SPSR
    PUSH    {lr}

    ; Push used registers.
    PUSH    {r0-r4, r12}

    ;  Save the floating point context
    FMRX  R0, FPSCR
    VPUSH {D0-D15}
    PUSH  {R0}

    ; Align stack pointer to 8 byte boundary
    MOV     r2, sp
    AND     r2, r2, #4
    SUB     sp, sp, r2

    ; Call the interrupt handler.
    PUSH    {r0-r4, lr}
    LDR     r1, masterIsr_c_addr
    BLX     r1
    POP     {r0-r4, lr}
    ADD     sp, sp, r2

    ; disable IRQ
    CPSID   i
    DSB
    ISB

    ;  Restore the floating point context
    POP   {R0}
    VPOP  {D0-D15}
    VMSR  FPSCR, R0

    ; Restore used registers, LR and SPSR before returning.
    POP     {r0-r4, r12}
    POP     {LR}
    MSR     SPSR_cxsf, LR
    POP     {LR}
    MOVS    PC, LR

;***************************************************************
;* FUNCTION DEF: dataAbortExptnHandler
;***************************************************************
    .global dataAbortExptnHandler
    .sect ".text.hwi"
    ; Whenever this exception occurs, link register(lr) is loaded with IA+8
    ; in both arm and thumb mode. Here IA is the address of the load or store
    ; instruction that generated the Data Abort
    ; So in arm mode to execute the next instruction, subtract 4 from lr
    ; and for thumb mode to execute the next instruction, subtract 6 from lr.
    ; This is needed as the size of instruction which caused the data abort is of 2 bytes in
    ; thumb mode and 4 bytes in arm mode
    ;
    ; branches to label ARM_STATE if the thumb state bit is not set
    ; lr = lr - 6 in thumb mode and lr = lr -4 in arm mode
dataAbortExptnHandler:

    ; Push used registers.
    PUSH    {r0-r4, r12}

    ; SPSR has the snapshot of CPSR before data abort. Compare thumb state bit in SPSR
    MRS r0, SPSR
    AND r1, r0, #0x20
    CMP R1, #0

    ; branches to label ARM_STATE if the thumb state bit is not set
    BEQ ARM_STATE
    SUB lr, lr, #2
ARM_STATE:
    SUB lr, lr, #4

    ; Push the return address and SPSR.
    PUSH    {lr}
    MRS	lr, SPSR
    PUSH    {lr}

    ; Call the data abort handler.
    LDR r1, dataAbortExptnHandler_c_addr
    BLX r1

    ; Restore used registers, LR and SPSR before returning.
    POP {LR}
    MSR SPSR_cxsf, LR
    POP {LR}
    POP {r0-r4, r12}
    MOVS    PC, LR

;***************************************************************
;* FUNCTION DEF: swIntrExptnHandler
;***************************************************************
    .global swIntrExptnHandler
    .sect ".text.hwi"
    ; Whenever this exception occurs, link register(lr) is loaded with IA+4 in
    ; arm mode and IA+2 in thumb mode. Here IA is the address of instruction
    ; which causes the software interrupt. So in-order to execute the next instruction
    ; no need to change the link register value.
swIntrExptnHandler:

    ; Push used registers.
    PUSH    {r0-r4, r12}

    ; Push the return address and SPSR.
    PUSH    {lr}
    MRS	lr, SPSR
    PUSH    {lr}

    ; Call the software interrupt handler.
    LDR r1, swIntrExptnHandler_c_addr
    BLX r1

    ; Restore used registers, LR and SPSR before  returning.
    POP {LR}
    MSR SPSR_cxsf, LR
    POP {LR}
    POP {r0-r4, r12}
    MOVS    PC, LR

;***************************************************************
;* FUNCTION DEF: prefetchAbortExptnHandler
;***************************************************************
    .global prefetchAbortExptnHandler
    .sect ".text.hwi"
    ; Whenever this exception occurs, link register(lr) is loaded
    ; with (IA)+4 in case of both arm and thumb mode. Here IA is the address
    ; of the instruction which had the prefetch abort. So in-order to execute the
    ; same instruction again if R5 comes back from the prefect abort handler, 
    ; subtract 4 from lr so that you execute IA again
prefetchAbortExptnHandler:

    ; Push used registers.
    PUSH    {r0-r4, r12}

    SUB lr, lr, #4

    ; Push the return address and SPSR.
    PUSH    {lr}
    MRS	lr, SPSR
    PUSH    {lr}

    ; Call the prefetch abort handler.
    LDR r1, prefetchAbortExptnHandler_c_addr
    BLX r1

    ; Restore used registers, LR and SPSR before  returning.
    POP {LR}
    MSR SPSR_cxsf, LR
    POP {LR}
    POP {r0-r4, r12}
    MOVS    PC, LR

;***************************************************************
;* FUNCTION DEF: irqExptnHandler
;***************************************************************
    .global irqExptnHandler
    .sect ".text.hwi"
    ; Whenever this exception occurs, link register(lr) is loaded
    ; with (IA)+4 in both arm and thumb mode. Here IA is the address of the
    ; instruction which was not executed because of IRQ/FIQ took priority.
    ; So to execute the instruction which was not executed due to
    ; interrupt, subtract 4 from lr
irqExptnHandler:

    ; Push used registers.
    PUSH    {r0-r4, r12}

    SUB lr, lr, #4

    ; Push the return address and SPSR.
    PUSH    {lr}
    MRS	lr, SPSR
    PUSH    {lr}

    ; Call the IRQ interrupt handler.
    LDR	r1, irqExptnHandler_c_addr
    BLX	r1

    ; Restore used registers, LR and SPSR before  returning.
    POP {LR}
    MSR SPSR_cxsf, LR
    POP {LR}
    POP {r0-r4, r12}
    MOVS    PC, LR

;***************************************************************
;* FUNCTION DEF: fiqExptnHandler
;***************************************************************
    .global fiqExptnHandler
    .sect ".text.hwi"
    ; Whenever this exception occurs, link register(lr) is loaded
    ; with (IA)+4 in both arm and thumb mode. Here IA is the address of the
    ; instruction which was not executed because of IRQ/FIQ took priority.
    ; So to execute the instruction which was not executed due to
    ; interrupt, subtract 4 from lr
fiqExptnHandler:

    ; Push used registers
    PUSH    {r0-r4, r12}

    SUB lr, lr, #4

    ; Push the return address and SPSR
    PUSH    {lr}
    MRS lr, SPSR
    PUSH    {lr}

    ; Call the FIQ interrupt handler
    LDR r1, fiqExptnHandler_c_addr
    BLX r1

    ; Restore used registers, LR and SPSR before returning
    POP {LR}
    MSR SPSR_cxsf, LR
    POP {LR}
    POP {r0-r4, r12}
    MOVS    PC, LR

;***************************************************************
;* FUNCTION DEF: undefInstructionExptnHandler
;***************************************************************
    .global undefInstructionExptnHandler
    .sect ".text.hwi"
    ; Whenever this exception occurs link Register(lr) is
    ; loaded with (IA)+2 in case of thumb and
    ; IA+4 in case of arm. Here IA is the address of the undefined instruction
    ; So no need to update the value of lr since it is already
    ; loaded with address pointing to the next instruction
undefInstructionExptnHandler:

    ; Push used registers
    PUSH    {r0-r4, r12}

    ; Push the return address and SPSR
    PUSH    {lr}
    MRS lr, SPSR
    PUSH    {lr}

    ; Call the undefined instruction exception handler
    LDR r1, undefInstructionExptnHandler_c_addr
    BLX r1

    ; Restore used registers, LR and SPSR before  returning
    POP {LR}
    MSR SPSR_cxsf, LR
    POP {LR}
    POP {r0-r4, r12}
    MOVS    PC, LR

;***************************************************************
;* UNDEFINED REFERENCES
;***************************************************************
    .global masterIsr_c
    .global dataAbortExptnHandler_c
    .global swIntrExptnHandler_c
    .global prefetchAbortExptnHandler_c
    .global irqExptnHandler_c
    .global fiqExptnHandler_c
    .global undefInstructionExptnHandler_c

masterIsr_c_addr:                    .long masterIsr_c
dataAbortExptnHandler_c_addr:        .long dataAbortExptnHandler_c
swIntrExptnHandler_c_addr:           .long swIntrExptnHandler_c
prefetchAbortExptnHandler_c_addr:    .long prefetchAbortExptnHandler_c
irqExptnHandler_c_addr:              .long irqExptnHandler_c
undefInstructionExptnHandler_c_addr: .long undefInstructionExptnHandler_c
fiqExptnHandler_c_addr:              .long fiqExptnHandler_c

