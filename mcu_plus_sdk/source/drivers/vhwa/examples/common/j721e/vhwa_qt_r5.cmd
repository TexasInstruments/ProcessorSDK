/*----------------------------------------------------------------------------*/
/* File: k3m4_r5f_linker.cmd                                                  */
/* Description:                                                               */
/*    Link command file for Maxwell M4 MCU 0 view                             */
/*    TI ARM Compiler version 15.12.3 LTS or later                            */
/*                                                                            */
/*    Platform: QT                                                            */
/* (c) Texas Instruments 2019, All rights reserved.                           */
/*----------------------------------------------------------------------------*/
/*  History:                                                                  *'
/*    Aug 26th, 2016 Original version .......................... Loc Truong   */
/*    Aug 01th, 2017 new TCM mem map  .......................... Loc Truong   */
/*    Nov 07th, 2017 Changes for R5F Init Code.................. Vivek Dhande */
/*    Jan 18th, 2019 Moved Boot code to TCM ..................   Brijesh Jadav*/
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
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"
--fill_value=0
--stack_size=0x2000
--heap_size=0x1000
--entry_point=_resetvectors     /* Default C RTS boot.asm   */
-cr

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x1000;
__FIQ_STACK_SIZE = 0x1000;
__ABORT_STACK_SIZE = 0x1000;
__UND_STACK_SIZE = 0x1000;
__SVC_STACK_SIZE = 0x1000;

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY
{
    RESET_VECTORS (X)       : origin=0x70000000 length=0x100
    VECTORS (X)             : origin=0x70000100 length=0x1000
    BOOT_CODE (X)           : origin=0x70001100 length=0x5000

    MSMC3  (RWIX)             : origin=0x70006100 length=0x400000-0x6100

}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
    .intvecs    : {} palign(8)      > VECTORS
    .intc_text  : {} palign(8)      > VECTORS
    .rstvectors : {} palign(8)      > RESET_VECTORS
    .bootCode    	: {} palign(8) 		> BOOT_CODE
    .startupCode 	: {} palign(8) 		> BOOT_CODE
    .startupData 	: {} palign(8) 		> BOOT_CODE, type = NOINIT
    .text       : {} palign(8)      > MSMC3
    .const      : {} palign(8)      > MSMC3
    .rodata     : {} palign(8)      > MSMC3
    .cinit      : {} palign(8)      > VECTORS
    .pinit      : {} palign(8)      > VECTORS
    .bss        : {} align(4)       > MSMC3
    .data       : {} palign(128)    > MSMC3
    .sysmem     : {}                > MSMC3
    .stack      : {} align(4)       > MSMC3  (HIGH)

    /* Additional sections settings     */

    .irqStack   : {. = . + __IRQ_STACK_SIZE;} align(4)      > MSMC3  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)
    .fiqStack   : {. = . + __FIQ_STACK_SIZE;} align(4)      > MSMC3  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)
    .abortStack     : {. = . + __ABORT_STACK_SIZE;} align(4)        > MSMC3  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)
    .undStack   : {. = . + __UND_STACK_SIZE;} align(4)      > MSMC3  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)
    .svcStack   : {. = . + __SVC_STACK_SIZE;} align(4)      > MSMC3  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/