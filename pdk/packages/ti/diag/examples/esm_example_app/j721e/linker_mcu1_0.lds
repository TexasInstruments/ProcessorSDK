/*
 * ESM Example
 *
 * Error Signaling Module (ESM) Example Application
 *
 *  Copyright (c) Texas Instruments Incorporated 2020
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

/*----------------------------------------------------------------------------*/
/* File: linker_mcu1_0.lds                                                    */
/* Description:																  */
/*    Link command file for J721E R5F MCU 0 view							  */
/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
/* Standard linker options													  */
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
--heap_size=0x1000
--entry_point=_resetvectors		/* Default C RTS boot.asm	*/

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x1000;
__FIQ_STACK_SIZE = 0x1000;
__ABORT_STACK_SIZE = 0x1000;
__UND_STACK_SIZE = 0x1000;
__SVC_STACK_SIZE = 0x1000;

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY
{
    MCU1_0_RESET_VECTORS (X)    : origin=0x41C00000     length=0x100
    MCU1_1_RESET_VECTORS (X)    : origin=0x41C00100     length=0x200 - 0x100
    /* MCU0_R5F_0 local view 												  */
    MCU0_R5F_TCMA (X)		    : origin=0x0			length=0x7c00 fill=0xffffffff
    MCU0_R5F_TCMA_TEST (X)      : origin=0x7c00			length=0x400 fill=0xffffffff
    MCU0_R5F_TCMB0 (RWIX)	    : origin=0x41010000	    length=0x8000

    /* MCU0_R5F_1 SoC view													  */
    MCU0_R5F1_ATCM (RWIX)  	: origin=0x41400000 length=0x8000
    MCU0_R5F1_BTCM (RWIX) 	: origin=0x41410000 length=0x8000

    /* MCU0 memory used for SBL and SYSFW. Avaiable after boot for appi starts for dynamic use					  */
    SBL_RESERVED 	(RWIX) 		: origin=0x41C00100     length=0x60000 - 0x100  /* ~383KB */

    /* Refer the user guide for details on persistence of these sections */
    OCMC_RAM_BOARD_CFG (RWIX)   : origin=0x41C80000 length=0x2000
    OCMC_RAM_SCISERVER (RWIX)   : origin=0x41C82000 length=0x60000
    VECTORS_MCU1_0 (X)          : origin=0x41CE2000 length=0x1000
    VECTORS_MCU1_1 (X)          : origin=0x41CE1000 length=0x1000
    OCMC_RAM (RWIX)             : origin=0x41CE4100 length=0x1BA00
    OCMC_RAM_X509_HEADER (RWIX) : origin=0x41CFFB00 length=0x500

    /* MCU0 Location RESERVED for SDR RAT Self Test */
    SDR_OCMC_RAT_SELFTEST (R) : origin=0x41C7EFC0 length=32

    /* MCU0 Location RESERVED for SDR MPU Self Test */
    SDR_OCMC_MPU_SELFTEST (R) : origin=0x41C7EFE0 length=32

    /* J721E R5F locations													  */
    MSMC3	(RWIX)	 		: origin=0x70020000 length=0xD0000			/* 840 K */
    /* Reserved for DMSC */
    MSMC3_DMSC_FW (RWIX)	: origin=0x700F0000 length=0x10000			/* 64K */
    DDR0    (RWIX)  		: origin=0x80000000 length=0x80000000    	/* 2GB */

/* Additional memory settings	*/
/* N/A */

}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
/* 'intvecs' and 'intc_text' sections shall be placed within                  */
/* a range of +\- 16 MB                                                       */
    .intvecs    	: {} palign(8) 		> VECTORS_MCU1_0
    .intc_text     	: {} palign(8) 		> VECTORS_MCU1_0
    .rstvectors     : {} palign(8) 		> MCU0_R5F_TCMA
    .bootCode   	: {} palign(8) 		> MSMC3
    .startupCode    : {} palign(8)  	> MSMC3
    .text       	: {} palign(8) 		> MSMC3
    .const      	: {} palign(8) 		> MSMC3
    .cinit      	: {} palign(8) 		> MSMC3
    .pinit      	: {} palign(8) 		> MSMC3
    .bss        	: {} align(4)  		> MSMC3
    .data    	    : {} palign(128) 	> MSMC3
    .boardcfg_data  : {} palign(128)    > MSMC3
	.sysmem  	    : {} 				> MSMC3
    .bss:extMemCache:ramdisk : {} align (32)     > DDR0
	.stack  	    : {} align(4)		> MSMC3  (HIGH)
	.irqStack  	: {. = . + __IRQ_STACK_SIZE;} align(4)		 > MSMC3  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)
    .fiqStack  	    : {. = . + __FIQ_STACK_SIZE;} align(4)	 > MSMC3  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)
    .abortStack  	: {. = . + __ABORT_STACK_SIZE;} align(4) > MSMC3  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)
    .undStack  	    : {. = . + __UND_STACK_SIZE;} align(4)	 > MSMC3  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)
    .svcStack  	: {. = . + __SVC_STACK_SIZE;} align(4)		 > MSMC3  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)

/* Additional sections settings 	*/
/* N/A */

}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
