/*----------------------------------------------------------------------------*/
/* File: emac_linker_r5_no_ddr.cmd                                                  */
/* Description:																  */
/*    Link command file for AM65XX M4 MCU 0 view							  */
/*	  TI ARM Compiler version 15.12.3 LTS or later							  */
/*                                                                            */
/*    Platform: QT                                                            */
/* (c) Texas Instruments 2019, All rights reserved.                           */
/*----------------------------------------------------------------------------*/
/*  History:															      *'
/*    Aug 26th, 2016 Original version .......................... Loc Truong   */
/*    Aug 01th, 2017 new TCM mem map  .......................... Loc Truong   */
/*    Nov 07th, 2017 Changes for R5F Init Code.................. Vivek Dhande */
/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
/* Standard linker options													  */
--retain="*(.intvecs)"
--retain="*(.intc_text)"
--retain="*(.rstvectors)"
--fill_value=0
--entry_point=ti_sysbios_family_arm_v7r_keystone3_Hwi_vectors /* Default BIOS */
--retain="*(.utilsCopyVecsToAtcm)"

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY
{
    VECTORS (X)  		: origin=0x41C7F000 length=0x1000
    /*  Reset Vectors base address(RESET_VECTORS) should be 64 bytes aligned  */
    RESET_VECTORS (X) 		: origin=0x41C00000 length=0x100
    /* MCU0_R5F_0 local view */
    MCU0_R5F_TCMA_SBL_RSVD (X)	: origin=0x0		length=0x100
    MCU0_R5F_TCMA (X)		: origin=0x100		length=0x8000 - 0x100
    MCU0_R5F_TCMB0 (RWIX)	: origin=0x41010000	length=0x8000

    /* MCU0_R5F_1 SoC view */
    MCU0_R5F1_ATCM (RWIX)  	: origin=0x41400000 length=0x8000
    MCU0_R5F1_BTCM (RWIX) 	: origin=0x41410000 length=0x8000

    /* Fully avaialble for apps. Used by SBL to load SYSFW */
    OCMRAM_LOW 	(RWIX) 		: origin=0x41C00100 length=0x40600 - 0x100	     /* ~257KB */

    /* MCU0 memory used for SBL. Avaiable after boot for app starts for dynamic use */
    SBL_RESERVED 	(RWIX) 	: origin=0x41C40600 length=0x60000 - 0x40600	     /* ~126KB */

    /* MCU0 share locations */
    OCMRAM 	(RWIX) 		: origin=0x41C60000 length=0x20000 - 0x1000	     /* ~124KB */

    /* AM65XX M4 locations */
    MSMC3       (RWIX)          : origin=0x70000000 length=0xF0000             /* 1MB - 64K */
    MSMC3_H (RWIX)              : origin=0x70100000 length=0xE2000             /* 1MB -56K */

    /* Reserved for DMSC */
    MSMC3_DMSC (RWIX)           : origin=0x701F0000 length=0x10000             /* 64K */

    DDR0    (RWIX)  		: origin=0x80000000 length=0x80000000    	/* 2GB */

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
    .rstvectors 	: {} palign(8) 		> RESET_VECTORS
    .utilsCopyVecsToAtcm : {} palign(8) > MCU0_R5F_TCMB0
    .text    	: {} palign(8) 		> OCMRAM_LOW
    .const   	: {} palign(8) 		> OCMRAM_LOW
    .cinit   	: {} palign(8) 		> OCMRAM_LOW
    .pinit   	: {} palign(8) 		> OCMRAM_LOW
    .bss     	: {} align(4)  		> MSMC3
    .data    	: {} palign(128) 	> MSMC3_H
    .boardcfg_data        : {} palign(128)           > OCMRAM
    .sysmem  	: {} 				> OCMRAM
    .startup_text: { -l *sysbios.aer5f*(.text)
                     -l *boot.aer5f*(.text)
                     -l *auto_init.aer5f*(.text)
                     -l *emacUnitTest_r5_per5ft.oer5f*(.text)
                    } > OCMRAM
    .stack  	: {} align(4)		> MSMC3

/* Additional sections settings 	*/

}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
