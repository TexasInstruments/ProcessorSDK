/*----------------------------------------------------------------------------*/
/* File: linker.cmd                                                           */
/* Description:								                                  */
/*    Link command file for AM64x Uniflash Flash programmer                   */
/*                                                                            */
/*    Platform: R5 Cores on AM64xx                                            */
/* (c) Texas Instruments 2020, All rights reserved.                           */
/*----------------------------------------------------------------------------*/
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
--entry_point=_ufpResetVectors

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x1000;
__FIQ_STACK_SIZE = 0x1000;
__ABORT_STACK_SIZE = 0x800;
__UND_STACK_SIZE = 0x800;
__SVC_STACK_SIZE = 0x2000;

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY
{
    /*  Reset Vectors base address(RESET_VECTORS) should be 64 bytes aligned  */
    RESET_VECTORS (X)  			: origin=0x70000000 length=0x100
    OCMRAM_UFP    (RWIX)   : origin=0x70000100 length=0x3E000-0x100
    OCMRAM_UFP_SYSFW (RWIX)   : origin=0x7003E000 length=0x40000

}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
/* 'intvecs' and 'intc_text' sections shall be placed within                  */
/* a range of +\- 16 MB                                                       */
    .rstvectors           : {} palign(8)                            > RESET_VECTORS
    .bootCode    	      : {} palign(8)                     		> OCMRAM_UFP
    .startupCode 	      : {} palign(8)                     		> OCMRAM_UFP
    .startupData 	      : {} palign(8)                     		> OCMRAM_UFP, type = NOINIT
    .text    	          : {} palign(8)                            > OCMRAM_UFP
    .const   	          : {} palign(8)                            > OCMRAM_UFP
    .cinit                : {} palign(8)                            > OCMRAM_UFP
    .pinit                : {} palign(8)                            > OCMRAM_UFP
    .boardcfg_data	  : {} palign(128)                          > OCMRAM_UFP

    .data                 : {} palign(128)                          > OCMRAM_UFP
    .bss     	          : {} align(4)                             > OCMRAM_UFP
    .sysmem  	          : {}                                      > OCMRAM_UFP

	.stack  	          : {} align(4)                             > OCMRAM_UFP  (HIGH)
	.irqStack  	          : {. = . + __IRQ_STACK_SIZE;} align(4)    > OCMRAM_UFP  (HIGH)
                            RUN_START(__IRQ_STACK_START)
                            RUN_END(__IRQ_STACK_END)
    .fiqStack  	          : {. = . + __FIQ_STACK_SIZE;} align(4)    > OCMRAM_UFP  (HIGH)
                            RUN_START(__FIQ_STACK_START)
                            RUN_END(__FIQ_STACK_END)
    .abortStack  	      : {. = . + __ABORT_STACK_SIZE;} align(4)  > OCMRAM_UFP  (HIGH)
                            RUN_START(__ABORT_STACK_START)
                            RUN_END(__ABORT_STACK_END)
    .undStack  	          : {. = . + __UND_STACK_SIZE;} align(4)    > OCMRAM_UFP  (HIGH)
                            RUN_START(__UND_STACK_START)
                            RUN_END(__UND_STACK_END)
    .svcStac              : {. = . + __SVC_STACK_SIZE;} align(4)    > OCMRAM_UFP  (HIGH)
                            RUN_START(__SVC_STACK_START)
                            RUN_END(__SVC_STACK_END)
    .firmware             : {} palign(8)                            > OCMRAM_UFP_SYSFW

}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
