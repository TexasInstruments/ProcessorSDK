/*----------------------------------------------------------------------------*/
/* File: linker_mcu1_0.lds                                                   */
/* Description:                                                              */
/*    Link command file for J721E R5F MCU 0 view                             */
/*                                                                            */
/* (c) Texas Instruments 2020, All rights reserved.                           */
/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
/* Standard linker options													  */
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

    /* MCU0_R5F_0 local view */
    MCU0_R5F_TCMA (X)		: origin=0x0		length=0x8000
    MCU0_R5F_TCMB0 (RWIX)	: origin=0x41010000	length=0x8000

    /* MCU0_R5F_1 SoC view */
    MCU0_R5F1_ATCM (RWIX)  	: origin=0x41400000 length=0x8000
    MCU0_R5F1_BTCM (RWIX) 	: origin=0x41410000 length=0x8000

    /* Refer the user guide for details on persistence of these sections */
    OCMC_RAM_BOARD_CFG (RWIX)   : origin=0x41C80000 length=0x2000
    OCMC_RAM_SCISERVER (RWIX)   : origin=0x41C82000 length=0x60000
    VECTORS (X)                 : origin=0x41CE2000 length=0x1000
    RESET_VECTORS (X)           : origin=0x41CE3000 length=0x100
    OCMC_RAM (RWIX)             : origin=0x41CE3100 length=0x1AA00
    OCMC_RAM_X509_HEADER (RWIX) : origin=0x41CFDB00 length=0x500

    /* MCU0 memory used for SBL. Avaiable after boot for app starts for dynamic use */
    SBL_RESERVED 	(RWIX) 	: origin=0x41C40600 length=0x60000 - 0x40600	     /* ~126KB */

    /* MCU0 share locations */
    OCMRAM 	(RWIX) 		: origin=0x41C60000 length=0x20000 - 0x1000	     /* ~124KB */

    /* j721s2 MCMS3 locations */
    /* j721s2 Reserved Memory for ARM Trusted Firmware */
    MSMC3_ARM_FW   (RWIX)   : origin=0x70000000 length=0x40000         /* 256KB */
    MSMC3   (RWIX)          : origin=0x70040000 length=0x3B0000        /* 4MB - 320KB */
    /* j721s2 Reserved Memory for DMSC Firmware */
    MSMC3_DMSC_FW  (RWIX)   : origin=0x703F0000 length=0x10000         /* 64KB */
    DDR0    (RWIX)  		: origin=0xA0100000 length=0xF00000    	             /* Reserve Memory for R5F */

/* Additional memory settings	*/

}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
/* 'intvecs' and 'intc_text' sections shall be placed within                  */
/* a range of +\- 16 MB                                                       */
    .intvecs 	: {} palign(8) 		> VECTORS
    .intc_text 	: {} palign(8) 		> VECTORS
    .rstvectors 	: {} palign(8) 	> MCU0_R5F_TCMA
    .bootCode      : {} palign(8)      > OCMC_RAM
    .startupCode   : {} palign(8)      > OCMC_RAM
    .startupData   : {} palign(8)      > OCMC_RAM, type = NOINIT
    .text    	: {} palign(8) 		> OCMC_RAM
    .const   	: {} palign(8) 		> OCMC_RAM
    .resource_table : {} palign(8) 	> OCMC_RAM
    .cinit   	: {} palign(8) 		> OCMC_RAM
    .pinit   	: {} palign(8) 		> OCMC_RAM
    .bss     	: {} align(4)  		> DDR0
    .far     	: {} align(4)  		> DDR0
    .data    	: {} palign(128) 	> DDR0
    .boardcfg_data        : {} palign(128)           > DDR0
    .sysmem  	: {} 				> DDR0

    .stack  	: {} align(4)		> DDR0  
    .irqStack  	: {. = . + __IRQ_STACK_SIZE;} align(4)		> DDR0 (HIGH) 
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)
    .fiqStack  	: {. = . + __FIQ_STACK_SIZE;} align(4)		> DDR0 (HIGH) 
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)
    .abortStack  	: {. = . + __ABORT_STACK_SIZE;} align(4)    > DDR0 (HIGH) 
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)
    .undStack  	: {. = . + __UND_STACK_SIZE;} align(4)		> DDR0 (HIGH) 
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)
    .svcStack  	: {. = . + __SVC_STACK_SIZE;} align(4)		> DDR0 (HIGH) 
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)

/* Additional sections settings 	*/

}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
