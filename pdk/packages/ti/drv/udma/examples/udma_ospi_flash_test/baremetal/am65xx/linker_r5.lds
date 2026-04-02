/* Linker Settings */
/* Standard linker options */
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

-stack  0x2000                  /* SOFTWARE STACK SIZE      */
-heap   0x2000                  /* HEAP AREA SIZE           */

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE    = 0x1000;
__FIQ_STACK_SIZE    = 0x1000;
__ABORT_STACK_SIZE  = 0x1000;
__UND_STACK_SIZE    = 0x1000;
__SVC_STACK_SIZE    = 0x1000;

/*----------------------------------------------------------------------------*/
/* Memory Sections                                                            */
MEMORY
{
    /* Refer the user guide for details on persistence of these sections */
    /* Also, when these memories can be used by apps */
    MCU0_ATCM_NOT_USED          (R)     : origin=0x0        length=0x7FFF
    /* To load .udma_critical_fxns / .udma_buffer_r5_tcm in TCM at run time */
    MCU0_BTCM_RUN_TIME_LOAD     (RWIX)  : origin=0x41010000 length=0x7FFF
    
    /* Used by SBL, can be used after APPs is started */
    MCU_MSRAM_RSVD_UNUSED       (R)     : origin=0x41C00000 length=0x200
    MCU_MSRAM_RSVD_SBL          (RWIX)  : origin=0x41C00200 length=0x3DE00

    VECTORS                     (RWIX)  : origin=0x41C3E000 length=0x100
    RESET_VECTORS               (RWIX)  : origin=0x41C3E100 length=0x1000
    OCMRAM                      (RWIX)  : origin=0x41C3F100 length=0x40F00

    /* COMPUTE_CLUSTER0_MSMC_SRAM */
    MSMC3                       (RWIX) : origin=0x70000000 length=0x1EFC00
    /* The origin and length is determined by board cfg, */
    /* refer user guide for details */
    MSMC3_RSVD_DMSC             (RWIX)  : origin=0x701F0000 length=0x1000
    DDR0                        (RWIX)  : origin=0x80000000 length=0x7FFFFFE4

}  /* end of MEMORY */

/* Section Configuration */
SECTIONS
{
    .intvecs 	    : {} palign(8) 		> VECTORS
    .intc_text 	    : {} palign(8) 		> VECTORS
    .rstvectors 	: {} palign(8) 		> RESET_VECTORS
    .bootCode       : {} palign(8)      > MSMC3
    .startupCode    : {} palign(8)      > MSMC3
    .startupData    : {} palign(8)      > MSMC3, type = NOINIT
    .text    	    : {} palign(8) 		> OCMRAM
    .const   	    : {} palign(8) 		> OCMRAM
    .rodata  	    : {} palign(8) 		> OCMRAM
    .cinit   	    : {} palign(8) 		> OCMRAM
    .pinit   	    : {} palign(8) 		> OCMRAM
    .bss     	    : {} align(4)  		> OCMRAM
    .far     	    : {} align(4)  		> OCMRAM
    .data    	    : {} palign(128) 	> OCMRAM
    .boardcfg_data  : {} palign(128)    > MSMC3
    .sysmem  	    : {} 				> OCMRAM

    /* USB ram disk dev-msc example */
    .bss:extMemCache:ramdisk : {} align (32)     > OCMRAM

    /* USB or any other LLD buffer for benchmarking */
    .benchmark_buffer (NOLOAD) {} ALIGN (8) > OCMRAM

    /* OSPI app sections */
    .udma_buffer_r5_tcm : {} palign(128) > MSMC3
    .udma_critical_fxns : {} palign(8)   > MSMC3

    /* To load .udma_critical_fxns / .udma_buffer_r5_tcm in TCM at run time */
    /* Defines copy table & location where it will be loaded by SBL/ICE */
    .r5tcm_run_time_load : {
                    *(.udma_critical_fxns),
                    *(.udma_buffer_r5_tcm)
                 } palign(8) load = MSMC3,
                    run = MCU0_BTCM_RUN_TIME_LOAD,
                    table(_r5tcm_run_time_load_section)

    /* To load .udma_critical_fxns / .udma_buffer_r5_tcm in TCM at run time */
    /* This is the region which will have all the copy tables */
    .ovly       : {} palign(128) load = MSMC3

    .stack  	: {} align(4)		> MSMC3  (HIGH)

    .irqStack  	: {. = . + __IRQ_STACK_SIZE;}   align(4)		> OCMRAM  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)
    .fiqStack  	: {. = . + __FIQ_STACK_SIZE;}   align(4)		> OCMRAM  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)
    .abortStack : {. = . + __ABORT_STACK_SIZE;} align(4)	    > OCMRAM  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)
    .undStack  	: {. = . + __UND_STACK_SIZE;}   align(4)		> OCMRAM  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)
    .svcStack  	: {. = . + __SVC_STACK_SIZE;}   align(4)		> OCMRAM  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
}  
