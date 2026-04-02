/*
 *  Copyright (C) 2021-2024 Texas Instruments Incorporated
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

	.global _stub_start
	.global _stub_resume
	.global undef_handler
	.global swi_handler
	.global prefetch_abt_handler
	.global data_abt_handler
	.global reserved_handler
	.global irq_handler
	.global fiq_handler
	.global stub_entry
	.global _end_stack
    .global __mpu_init


       .text
       .global _lpm_vectors
       .arm
       .no_dead_strip

_lpm_vectors:

_reset:			ldr pc, _reset_handler
_undef:			ldr pc, _undef_handler
_swi:			ldr pc, _swi_handler
_prefetch_abt:		ldr pc, _prefetch_abt_handler
_data_abt:		ldr pc, _data_abt_handler
_reserved:		ldr pc, _reserved_handler
_irq:			ldr pc, _irq_handler
_fiq:			ldr pc, _fiq_handler

_reset_handler:
	.word _stub_start
_undef_handler:
	.word undef_handler
_swi_handler:
	.word swi_handler
_prefetch_abt_handler:
	.word prefetch_abt_handler
_data_abt_handler:
	.word data_abt_handler
_reserved_handler:
	.word reserved_handler
_irq_handler:
	.word irq_handler_asm
_fiq_handler:
	.word fiq_handler
_end_lpm_vectors:

_reset_vector_address:
	.word 0x00000000

_stub_start:
    // Check if it is IO DDR exit
    ldr     r0, =0x43018318
    ldr     r1, [r0]
    ldr     r0, =0x555555
    subs    r1, r1, r0
    beq     _stub_resume

    // save r4-r11 registers
	stmdb   sp!,{r4-r11}
	// Save current stack pointer
	adr r1, _sys_stack_pointer
	str sp, [r1]
	adr r1, _sys_lr_register
	str lr, [r1]

	// Copy vector table
	adr     r0, _reset
	ldr     r1, _reset_vector_address
	MOV     r2, #0x10
wordcopy:
	ldr     r3, [r0], #4
	str     r3, [r1], #4
	SUBS    r2, r2, #1
	BNE     wordcopy

	// switch to irq mode, set irq stack
	mrs	r0, cpsr
	mvn	r1, #0x1f
	and	r2, r1, r0
	orr	r2, r2, #0x12
	msr	cpsr_cf, r2
	adr r1, _sys_irq_stack_pointer // save the current irq stack pointer
	str sp, [r1]
	ldr	r13, _irq_stack_addr
	mov 	r14,#0

	//switch to fiq mode and save stack pointer
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x11
	msr	cpsr_cf, r0
	adr r1, _sys_fiq_stack_pointer
	str sp, [r1]

	//switch to abort mode and save stack pointer
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x17
	msr	cpsr_cf, r0
	adr r1, _sys_abort_stack_pointer
	str sp, [r1]

	//switch to undefined mode and save stack pointer
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x1b
	msr	cpsr_cf, r0
	adr r1, _sys_undefined_stack_pointer
	str sp, [r1]

	//switch to supervisor mode and save stack pointer
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x13
	msr	cpsr_cf, r0
	adr r1, _sys_supervisor_stack_pointer
	str sp, [r1]


	// switch to SYS mode
	mrs	r0, cpsr
	mvn	r1, #0x1f
	and	r2, r1, r0
	orr	r2, r2, #0x1f
	msr	cpsr_cf, r2

	// initialize the stack
	ldr sp, _stack_addr
	// Space for the jump
	nop
	nop

	// At this point we should be CPU0
	// jump to proper reset handler
	bl stub_entry

	//save the return value to r3
	mov r3, r0
	// switching back to DDR
	// disable interrupt
	cpsid i
	// switch to irq mode, set irq stack
	mrs	r0, cpsr
	mvn	r1, #0x1f
	and	r2, r1, r0
	orr	r2, r2, #0x12
	msr	cpsr_cf, r2
	ldr	r13, _sys_irq_stack_pointer
	mov 	r14,#0

	//switch to fiq mode and restore stack pointer
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x11
	msr	cpsr_cf, r0
	ldr	r13, _sys_fiq_stack_pointer

	//switch to abort mode and restore stack pointer
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x17
	msr	cpsr_cf, r0
	ldr	r13, _sys_abort_stack_pointer

	//switch to undefined ort mode and restore stack pointer
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x1b
	msr	cpsr_cf, r0
	ldr	r13, _sys_undefined_stack_pointer

	//switch to supervisor mode and restore stack pointer
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x13
	msr	cpsr_cf, r0
	ldr	r13, _sys_supervisor_stack_pointer


	// switch to SYS mode
	mrs	r0, cpsr
	mvn	r1, #0x1f
	and	r2, r1, r0
	orr	r2, r2, #0x1f
	msr	cpsr_cf, r2

	//restore the return value from r3
	mov r0, r3
	// reload the stack
	ldr sp, _sys_stack_pointer
	ldmia	sp!,{r4-r11}
	// Space for the jump
	nop
	nop
	ldr pc, _sys_lr_register

irq_handler_asm:
	stmdb	sp!,{r0-r3,r12,lr}
	bl irq_handler
	ldmia	sp!,{r0-r3,r12,lr}
	subs	pc, lr, #4

_stub_resume:
    // Disable FIQ and IRQ
    mrs r0, cpsr
    orr r0, r0, #0xC0
    msr cpsr_cf, r0

    #ifdef __ARM_FP
    // Setup priveleged and user mode access to CP10, CP11
    // required to enable NEON/VFP coprocessor access
    // ctrl reg BITS [23:22] - CP11, [21:20] - CP10 = b11
    mrc p15,#0x0,r0,c1,c0,#2
    mov r3,#0xf00000
    orr r0,r0,r3
    mcr p15,#0x0,r0,c1,c0,#2

    // Set the EN bit, FPEXC[30] to enable NEON AND VFP
    mov r0,#0x40000000
    fmxr fpexc,r0
    #endif

    // Set to SYSTEM mode
    mrs r0, cpsr
    bic r0, r0, #0x1F
    orr r0, r0, #0x1F
    msr cpsr_cf, r0

    // Initialize the stack
	ldr sp, _stack_addr
	// Space for the jump
	nop
	nop

    // Call the __mpu_init hook function.
    bl __mpu_init

	// At this point we should be CPU0
	// jump to proper reset handler
	bl stub_resume

	//save the return value to r3
	mov r3, r0
	// switching back to DDR
	// disable interrupt
	cpsid i
	// switch to irq mode, set irq stack
	mrs	r0, cpsr
	mvn	r1, #0x1f
	and	r2, r1, r0
	orr	r2, r2, #0x12
	msr	cpsr_cf, r2
	ldr	r13, _sys_irq_stack_pointer
	mov 	r14,#0

	//switch to fiq mode and restore stack pointer
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x11
	msr	cpsr_cf, r0
	ldr	r13, _sys_fiq_stack_pointer

	//switch to abort mode and restore stack pointer
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x17
	msr	cpsr_cf, r0
	ldr	r13, _sys_abort_stack_pointer

	//switch to undefined ort mode and restore stack pointer
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x1b
	msr	cpsr_cf, r0
	ldr	r13, _sys_undefined_stack_pointer

	//switch to supervisor mode and restore stack pointer
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x13
	msr	cpsr_cf, r0
	ldr	r13, _sys_supervisor_stack_pointer

	// switch to SYS mode
	mrs	r0, cpsr
	mvn	r1, #0x1f
	and	r2, r1, r0
	orr	r2, r2, #0x1f
	msr	cpsr_cf, r2

	//restore the return value from r3
	mov r0, r3
	// reload the stack
	ldr sp, _sys_stack_pointer
	ldmia	sp!,{r4-r11}
	// Space for the jump
	nop
	nop
	ldr pc, _sys_lr_register

_stack_addr:
	.word _end_stack
_irq_stack_addr:
	.word _end_stack - 0x200
_sys_stack_pointer:
	.word 0x00000000
_sys_lr_register:
	.word 0x00000000
_sys_irq_stack_pointer:
	.word 0x00000000
_sys_fiq_stack_pointer:
	.word 0x00000000
_sys_abort_stack_pointer:
	.word 0x00000000
_sys_undefined_stack_pointer:
	.word 0x00000000
_sys_supervisor_stack_pointer:
	.word 0x00000000
