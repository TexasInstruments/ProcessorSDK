/*----------------------------------------------------------------------------*/
/* File: mcuBootPerfLinker_small.lds                                          */
/* Description:                                                               */
/*    Link command file for AM65XX M4 MCU 0 view                              */
/*	  TI ARM Compiler version 15.12.3 LTS or later                            */
/*                                                                            */
/*    Platform: EVM                                                           */
/* (c) Texas Instruments 2020, All rights reserved.                           */
/*----------------------------------------------------------------------------*/
/*  History:                                                                  */
/*    Aug 26th, 2016 Original version .......................... Loc Truong   */
/*    Aug 01th, 2017 new TCM mem map  .......................... Loc Truong   */
/*    Nov 07th, 2017 Changes for R5F Init Code.................. Vivek Dhande */
/*    Mar 26th, 2019 Changes for R5F startup Code............... Vivek Dhande */
/*    May 11th, 2020 Changes for MCU-only resource usage.........J. Bergsagel */
/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
/* Standard linker options                                                    */
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

/* interface with SBL */
sblProfileLogAddr = 0x41c001f0;
sblProfileLogIndxAddr = 0x41c001f4;
sblProfileLogOvrFlwAddr = 0x41c001f8;
syfw_image = 0x41C3E000;

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY
{
        /* Used for SYSFW initlization */
	OCMRAM_SBL_SYSFW (RWIX) : origin=0x41C80000 length=0x2000
	OCMRAM_SBL_UNUSED (X)   : origin=0x41C60000 length=0x2000

        /* App code resides here  */
	RESET_VECTORS (X)       : origin=0x41C82000 length=0x100
	OCMRAM_BOOT_PERF (RWIX) : origin=0x41C82100 length=0x50000 - 0x100
	VECTORS (X)             : origin=0x41CD2100 length=0x1000


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
    .rstvectors : {} palign(8) 		> RESET_VECTORS
    .bootCode    	: {} palign(8) 		> OCMRAM_BOOT_PERF
    .startupCode 	: {} palign(128)    > OCMRAM_BOOT_PERF
    .startupData 	: {} palign(8) 		> OCMRAM_BOOT_PERF, type = NOINIT
    .sbl_mcu_1_0_resetvector : {} palign(8) > OCMRAM_BOOT_PERF
    .text    	: {} palign(8) 		> OCMRAM_BOOT_PERF
    .const   	: {} palign(8) 		> OCMRAM_BOOT_PERF
    .rodata   	: {} palign(8) 		> OCMRAM_BOOT_PERF
    .cinit   	: {} palign(8) 		> OCMRAM_BOOT_PERF
    .pinit   	: {} palign(8) 		> OCMRAM_BOOT_PERF
    .bss     	: {} align(4)  		> OCMRAM_BOOT_PERF
    .data    	: {} palign(128) 	> OCMRAM_BOOT_PERF
    .boardcfg_data   : {} palign(128)    > OCMRAM_BOOT_PERF
    .bss.devgroup    : {*(.bss.devgroup*)} align(4)       > OCMRAM_BOOT_PERF
    .const.devgroup  : {*(.const.devgroup*)} align(4)     > OCMRAM_BOOT_PERF

    .sysmem  	: {} 			> OCMRAM_BOOT_PERF
    .stack  	: {} align(8)		> OCMRAM_BOOT_PERF  (HIGH)
    .irqStack  	: {. = . + __IRQ_STACK_SIZE;} align(4)		> OCMRAM_BOOT_PERF  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)
    .fiqStack  	: {. = . + __FIQ_STACK_SIZE;} align(4)		> OCMRAM_BOOT_PERF  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)
    .abortStack  : {. = . + __ABORT_STACK_SIZE;} align(4)       > OCMRAM_BOOT_PERF  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)
    .undStack  	: {. = . + __UND_STACK_SIZE;} align(4)		> OCMRAM_BOOT_PERF  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)
    .svcStack  	: {. = . + __SVC_STACK_SIZE;} align(4)		> OCMRAM_BOOT_PERF  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)

/* Additional sections settings 	*/
    .sysfw_data_cfg_board      : {} palign(128) > OCMRAM_SBL_UNUSED (HIGH)
    .sysfw_data_cfg_board_rm   : {} palign(128) > OCMRAM_SBL_UNUSED (HIGH)
    .sysfw_data_cfg_board_sec  : {} palign(128) > OCMRAM_SBL_UNUSED (HIGH)

}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
