/**
 *  \file   linker.lds
 *
 *  \brief  Link command file for OTP Keywriter 
 * 			Platform: R5 cores on j721e
 *
 */

/*
 * Copyright (C) 2021 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

--retain="*(.bootCode)"
--retain="*(.startupCode)"
--retain="*(.startupData)"
--retain="*(.intvecs)"
--retain="*(.intc_text)"
--retain="*(.rstvectors)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"
--fill_value=0
--stack_size=0x2000
--heap_size=0x2000
/* Keywriter entry in init.asm */
--entry_point=_keywriterResetVectors

-stack  0x2000
-heap   0x2000

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE=0x1000;
__FIQ_STACK_SIZE=0x1000;
__ABORT_STACK_SIZE=0x800;
__UND_STACK_SIZE=0x800;
__SVC_STACK_SIZE=0x2000;

/* 
 * Reset Vectors base address(RESET_VECTORS) should be 64 bytes aligned
 * MCU0 memory used for Keywriter. Available to app for dynamic use ~160KB
 * RBL uses 0x41CC0000 and beyond. Keywriter, at load cannot cross this 
 */
MEMORY
{
    RESET_VECTORS      (X) 	   : origin=0x41C00100         length=0x100
    OCMRAM_Keywriter   (RWIX)  : origin=0x41C00200         length=0x70000-0x200
    OCMRAM_Certificate (RWIX)  : origin=0x41C70000         length=4
    OCMRAM_Stack       (RWIX)  : origin=0x41C70000+0x2000  length=0xA000
}

SECTIONS
{
    .rstvectors                   : {} palign(8)                                > RESET_VECTORS
    .bootCode                     : {} palign(8)                                > OCMRAM_Keywriter
    .startupCode                  : {} palign(8)                                > OCMRAM_Keywriter
    .startupData                  : {} palign(8)                                > OCMRAM_Keywriter, type = NOINIT
    .keywriter_profile_info       : {} palign(8)                                > RESET_VECTORS  (HIGH)
    .text                         : {} palign(8)                                > OCMRAM_Keywriter
    .const                        : {} palign(8)                                > OCMRAM_Keywriter
    .rodata                       : {} palign(8)                                > OCMRAM_Keywriter
    .const.devgroup.MCU_WAKEUP    : {} align(4)                                 > OCMRAM_Keywriter
    .const.devgroup.MAIN          : {} align(4)                                 > OCMRAM_Keywriter
    .const.devgroup.DMSC_INTERNAL : {} align(4)                                 > OCMRAM_Keywriter
    .cinit                        : {} palign(8)                                > OCMRAM_Keywriter
    .pinit                        : {} palign(8)                                > OCMRAM_Keywriter
    .boardcfg_data                : {} palign(128)                              > OCMRAM_Keywriter
    .data                         : {} palign(128)                              > OCMRAM_Keywriter, type = NOINIT
    .bss                          : {} align(4)                                 > OCMRAM_Keywriter
    .bss.devgroup.MAIN            : {} align(4)                                 > OCMRAM_Keywriter
    .bss.devgroup.MCU_WAKEUP      : {} align(4)                                 > OCMRAM_Keywriter
    .bss.devgroup.DMSC_INTERNAL   : {} align(4)                                 > OCMRAM_Keywriter
    .sysmem                       : {}                                          > OCMRAM_Keywriter
    .stack                        : {} align(4)                                 > OCMRAM_Stack  (HIGH)
    .irqStack                     : {. = . + __IRQ_STACK_SIZE;}   align(4)      > OCMRAM_Stack  (HIGH), RUN_START(__IRQ_STACK_START), RUN_END(__IRQ_STACK_END)
    .fiqStack                     : {. = . + __FIQ_STACK_SIZE;}   align(4)      > OCMRAM_Stack  (HIGH), RUN_START(__FIQ_STACK_START), RUN_END(__FIQ_STACK_END)
    .abortStack                   : {. = . + __ABORT_STACK_SIZE;} align(4)      > OCMRAM_Stack  (HIGH), RUN_START(__ABORT_STACK_START), RUN_END(__ABORT_STACK_END)
    .undStack                     : {. = . + __UND_STACK_SIZE;}   align(4)      > OCMRAM_Stack  (HIGH), RUN_START(__UND_STACK_START), RUN_END(__UND_STACK_END)
    .svcStac                      : {. = . + __SVC_STACK_SIZE;}   align(4)      > OCMRAM_Stack  (HIGH), RUN_START(__SVC_STACK_START), RUN_END(__SVC_STACK_END)
	.keywr_bin_end                : {}                                          > OCMRAM_Certificate

}
