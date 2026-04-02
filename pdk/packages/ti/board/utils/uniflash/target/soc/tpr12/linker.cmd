/*----------------------------------------------------------------------------*/
/* File: linker.cmd                                                           */
/* Description:                                                               */
/*    Link command file for TPR12 Uniflash                                    */
/*                                                                            */
/*    Platform: R5 Cores on TPR12                                             */
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

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */

-u _resetvectors

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x1000;
__FIQ_STACK_SIZE = 0x1000;
__ABORT_STACK_SIZE = 0x800;
__UND_STACK_SIZE = 0x800;
__SVC_STACK_SIZE = 0x2000;

/***********************************************************************************
 * This is reserved for the initialization code which is a part of the TCMA. The
 * code should setup the MPU to allow L2 execution permissions
 ***********************************************************************************/
#define UFP_INIT_CODE_SIZE          640
/* Note: Set below variable to 1 for enabling CCS load if Uniflash flash programmer */
#define UFP_CCS_LOAD                0

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY
{
    /* IVT Table for the R5 Bootable image is always placed at the beginning of the TCM Memory */
    /* Initialization code which executes in the TCM memory and sets up the MPU. */
    INIT_CODE   : origin = 0x00000000, length = UFP_INIT_CODE_SIZE
    TCMA_RAM (RX) : origin=UFP_INIT_CODE_SIZE length=(0x00008000 - UFP_INIT_CODE_SIZE)
    TCMB_RAM (RW) : origin=0x00080000 length=0x00008000
    /* L2 MEMORY reserved for IVT+Initialization code: */
    L2_RESVD    : ORIGIN = 0x10200000, LENGTH = UFP_INIT_CODE_SIZE, fill = 0xBDBDBDBD
    L2_RAM (RW)   : origin = (0x10200000 + UFP_INIT_CODE_SIZE) length=(0x00060000 - UFP_INIT_CODE_SIZE)
    L3_RAM (RW)   : origin=0x88000000 length=0x00390000
    HWA_RAM (RW)  : origin=0x82000000 length=0x00020000
}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
    /* SBL Initialization Code: This needs to be relocated from the L2 to the TCMA
     * The code is executed initially and this will setup the MPU permissions. */
    .ufp_init_code: palign(8), fill=0xabcdabcd
    {
       *(.rstvectors) /* IVT is put at the beginning of the section */
       . = align(8);
       *(.bootCode)
       . = align(8);
#if  (UFP_CCS_LOAD == 1)
    } > INIT_CODE
#else
    } load=L2_RESVD, run=INIT_CODE
#endif
    .startupCode 	      : {} palign(8)                            > L2_RAM
    .startupData 	      : {} palign(8)                            > L2_RAM, type = NOINIT
    .text    	          : {} palign(8)                            > L2_RAM
    .const   	          : {} palign(8)                            > L2_RAM
    .cinit                : {} palign(8)                            > L2_RAM
    .pinit                : {} palign(8)                            > L2_RAM
    .boardcfg_data	  : {} palign(128)                          > L2_RAM

    .data                 : {} palign(128)                          > L2_RAM
    .bss     	          : {} align(4)                             > L2_RAM
    .sysmem  	          : {}                                      > L2_RAM

	.stack  	          : {} align(4)                             > L2_RAM  (HIGH)
	.irqStack  	          : {. = . + __IRQ_STACK_SIZE;} align(4)    > L2_RAM  (HIGH)
                            RUN_START(__IRQ_STACK_START)
                            RUN_END(__IRQ_STACK_END)
    .fiqStack  	          : {. = . + __FIQ_STACK_SIZE;} align(4)    > L2_RAM  (HIGH)
                            RUN_START(__FIQ_STACK_START)
                            RUN_END(__FIQ_STACK_END)
    .abortStack  	      : {. = . + __ABORT_STACK_SIZE;} align(4)  > L2_RAM  (HIGH)
                            RUN_START(__ABORT_STACK_START)
                            RUN_END(__ABORT_STACK_END)
    .undStack  	          : {. = . + __UND_STACK_SIZE;} align(4)    > L2_RAM  (HIGH)
                            RUN_START(__UND_STACK_START)
                            RUN_END(__UND_STACK_END)
    .svcStac              : {. = . + __SVC_STACK_SIZE;} align(4)    > L2_RAM  (HIGH)
                            RUN_START(__SVC_STACK_START)
                            RUN_END(__SVC_STACK_END)
}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
