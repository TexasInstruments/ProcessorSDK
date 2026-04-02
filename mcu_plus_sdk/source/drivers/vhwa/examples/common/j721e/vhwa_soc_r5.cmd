/*----------------------------------------------------------------------------*/
/* File: k3m4_r5f_linker.cmd                                                  */
/* Description:                                                               */
/*    Link command file for j7 M4 MCU 0 view                              */
/*    TI ARM Compiler version 15.12.3 LTS or later                            */
/*                                                                            */
/*    Platform: QT                                                            */
/* (c) Texas Instruments 2017, All rights reserved.                           */
/*----------------------------------------------------------------------------*/
/*  History:                                                                  *'
/*    Aug 26th, 2016 Original version .......................... Loc Truong   */
/*    Aug 01th, 2017 new TCM mem map  .......................... Loc Truong   */
/*    Nov 07th, 2017 Changes for R5F Init Code.................. Vivek Dhande */
/*    Mar 26th, 2019 Changes for R5F startup Code............... Vivek Dhande */
/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
/* Standard linker options                                                    */
--retain="*(.bootCode)"
--retain="*(.startupCode)"
--retain="*(.startupData)"
--retain="*(.intvecs)"
--retain="*(.intc_text)"
--retain="*(.rstvectors)"
--fill_value=0
--stack_size=0x2000
--heap_size=0x1000

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY
{
    VECTORS (X)             : origin=0x41C7F000 length=0x1000
    /*  Reset Vectors base address(RESET_VECTORS) should be 64 bytes aligned  */
    RESET_VECTORS (X)           : origin=0x41C00000 length=0x100
    /* MCU0_R5F_0 local view                                                  */
    MCU0_R5F_TCMA (X)       : origin=0x0            length=0x8000
    MCU0_R5F_TCMB0 (RWIX)   : origin=0x41010000 length=0x8000

    /* MCU0_R5F_1 SoC view                                                    */
    MCU0_R5F1_ATCM (RWIX)   : origin=0x41400000 length=0x8000
    MCU0_R5F1_BTCM (RWIX)   : origin=0x41410000 length=0x8000

    /* MCU0 share locations                                                   */
    OCMRAM  (RWIX)          : origin=0x41C00100 length=0x80000 - 0x1100      /* ~510KB */

    /* j7 M4 locations                                                    */
    MSMC3   (RWIX)          : origin=0x70000000 length=0x200000         /* 2MB */
    DDR0    (RWIX)          : origin=0x80000000 length=0x80000000       /* 2GB */

/* Additional memory settings   */

}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
/* 'intvecs' and 'intc_text' sections shall be placed within                  */
/* a range of +\- 16 MB                                                       */
    .intvecs    : {} palign(8)      > VECTORS
    .intc_text  : {} palign(8)      > VECTORS
    .rstvectors     : {} palign(8)      > RESET_VECTORS
    .bootCode    	: {} palign(8) 		> MSMC3
    .startupCode 	: {} palign(8) 		> MSMC3
    .startupData 	: {} palign(8) 		> MSMC3, type = NOINIT
    .text       : {} palign(8)      > MSMC3
    .const      : {} palign(8)      > OCMRAM
    .rodata     : {} palign(8)      > OCMRAM
    .cinit      : {} palign(8)      > OCMRAM
    .pinit      : {} palign(8)      > OCMRAM
    .bss        : {} align(4)       > MSMC3
    .data       : {} palign(128)    > OCMRAM
    .data_buffer: {} palign(128)    > DDR0
    .sysmem     : {}                > OCMRAM
    .stack      : {} align(4)       > OCMRAM  (HIGH)

/* Additional sections settings     */

}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/