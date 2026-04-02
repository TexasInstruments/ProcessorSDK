/*
 *  Copyright (C) 2020-2021 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* linker options */
--retain="*(.bootCode)"
--retain="*(.startupCode)"
--retain="*(.startupData)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"
--fill_value=0
--stack_size=0x2000
--heap_size=0x1000

-e __VECS_ENTRY_POINT
--retain="*(.utilsCopyVecsToAtcm)"

#define BTCM_START 0x41010000

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x100;
__FIQ_STACK_SIZE = 0x100;
__ABORT_STACK_SIZE = 0x100;
__UND_STACK_SIZE = 0x100;
__SVC_STACK_SIZE = 0x100;


MEMORY
{
    R5F_TCMA_SBL_RSVD(X)     : ORIGIN = 0x00000000 , LENGTH = 0x00000100
    RESET_VECTORS(RWIX)      : ORIGIN = 0x41010000 , LENGTH = 0x00000100
    R5F_TCMB0(RWIX)          : ORIGIN = 0x41010100 , LENGTH = 0x00007F00
    OCMC_RAM_BOARD_CFG (RWIX)   : ORIGIN = 0x41c80000 , LENGTH = 0x2000
    /* Sciserver App Space */
    OCMC_RAM_SCISERVER (RWIX)   : ORIGIN = 0x41C82000 , LENGTH = 0x60000
    OCMC_RAM_X509_HEADER1 (RWIX) : ORIGIN = 0x41cffb00 , LENGTH = 0x500 /* covers header for J7200/J721E */
    OCMC_RAM_X509_HEADER2 (RWIX) : ORIGIN = 0x41cfdb00 , LENGTH = 0x500 /* covers header for J721S2 */
}

SECTIONS
{
    .vecs : {
         *(.vecs)
    } palign(8) > BTCM_START
    .vecs       : {
        __VECS_ENTRY_POINT = .;
    } palign(8) > RESET_VECTORS
    xdc.meta (COPY): { *(xdc.meta) } > R5F_TCMB0
    .init_text  : {
                     boot.*(.text)
                     *(.text:ti_sysbios_family_arm_MPU_*)
                     *(.text:ti_sysbios_family_arm_v7r_Cache_*)
                  }  palign(8) > R5F_TCMB0
    .text:xdc_runtime_Startup_reset__I     : {} palign(8) > R5F_TCMB0
    .utilsCopyVecsToAtcm                   : {} palign(8) > R5F_TCMB0

    .text           : {} palign(8)   > OCMC_RAM_SCISERVER
    .cinit          : {} palign(8)   > OCMC_RAM_SCISERVER
    .bss            : {} align(8)    > OCMC_RAM_SCISERVER
    .far            : {} align(8)    > OCMC_RAM_SCISERVER
    .const          : {} palign(8)   > OCMC_RAM_SCISERVER
    .data           : {} palign(128) > OCMC_RAM_SCISERVER
    .sysmem         : {} align(8)    > OCMC_RAM_SCISERVER
    .stack          : {} align(4)    > R5F_TCMB0
    .data_buffer    : {} palign(128) > OCMC_RAM_SCISERVER
    .boardcfg_data  : {} palign(8)   > OCMC_RAM_SCISERVER
    .const*         : {} palign(4)   > R5F_TCMB0
    .bss*           : {} align(4)    > R5F_TCMB0

    .irqStack       : {. = . + __IRQ_STACK_SIZE;} align(4)    > OCMC_RAM_SCISERVER  (HIGH)
                        RUN_START(__IRQ_STACK_START)
                        RUN_END(__IRQ_STACK_END)
    .fiqStack       : {. = . + __FIQ_STACK_SIZE;} align(4)    > OCMC_RAM_SCISERVER  (HIGH)
                        RUN_START(__FIQ_STACK_START)
                        RUN_END(__FIQ_STACK_END)
    .abortStack     : {. = . + __ABORT_STACK_SIZE;} align(4)  > OCMC_RAM_SCISERVER  (HIGH)
                        RUN_START(__ABORT_STACK_START)
                        RUN_END(__ABORT_STACK_END)
    .undStack       : {. = . + __UND_STACK_SIZE;} align(4)    > OCMC_RAM_SCISERVER  (HIGH)
                        RUN_START(__UND_STACK_START)
                        RUN_END(__UND_STACK_END)
    .svcStac        : {. = . + __SVC_STACK_SIZE;} align(4)    > OCMC_RAM_SCISERVER  (HIGH)
                        RUN_START(__SVC_STACK_START)
                        RUN_END(__SVC_STACK_END)
    
}
