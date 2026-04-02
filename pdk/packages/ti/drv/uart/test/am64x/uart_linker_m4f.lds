/* Linker Settings */
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

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x1000;
__FIQ_STACK_SIZE = 0x1000;
__ABORT_STACK_SIZE = 0x1000;
__UND_STACK_SIZE = 0x1000;
__SVC_STACK_SIZE = 0x1000;

/* Memory Map */
MEMORY
{
    VECS_M4F_MEM:     org = 0x00000000 len = 0x040
    IRAM_M4F_INTC_MEM:     org = 0x00000040 len = 0x400 - 0x040
    /* Memory assigned to move vector table for M4F core */
    IRAM_M4F_VTBL:   org = 0x00000400 len = 0x800
    /* M4F internal memory locations */
    IRAM_M4F_MEM   (RWIX)          : origin=0x0C00 length=0x30000 - 0xC00
    DRAM_M4F_MEM   (RWIX)          : origin=0x30000  length=0x10000
    DDR0           (RWIX)          : origin=0x80000000 length=0x80000000      /* 2GB */
}

/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY */

SECTIONS
{

    .intvecs : load > VECS_M4F_MEM
    .intc_text : load > IRAM_M4F_INTC_MEM
    .TI.noinit : load > IRAM_M4F_VTBL

    .bootCode      : {} palign(8)         > IRAM_M4F_MEM
    .startupCode   : {} palign(8)      > IRAM_M4F_MEM
    .startupData   : {} palign(8)      > DRAM_M4F_MEM, type = NOINIT
    .text          : {} palign(8)      > IRAM_M4F_MEM
    .const         : {} palign(8)      > DRAM_M4F_MEM
    .cinit         : {} palign(8)      > DDR0
    .pinit         : {} palign(8)      > IRAM_M4F_MEM
    .bss           : {} align(4)       > DRAM_M4F_MEM
    .far           : {} align(4)       > DRAM_M4F_MEM
    .data          : {} palign(128)    > DRAM_M4F_MEM
    .boardcfg_data : {} palign(128)    > DRAM_M4F_MEM
    .sysmem        : {}                > DRAM_M4F_MEM
    .data_buffer   : {} palign(128)    > DRAM_M4F_MEM

    /* USB or any other LLD buffer for benchmarking */
    .benchmark_buffer (NOLOAD) {} ALIGN (8) > DDR0

    .stack      : {} align(4)       > DRAM_M4F_MEM
    .irqStack   : {. = . + __IRQ_STACK_SIZE;} align(4)      > DRAM_M4F_MEM
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)
    .fiqStack   : {. = . + __FIQ_STACK_SIZE;} align(4)      > DRAM_M4F_MEM
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)
    .abortStack : {. = . + __ABORT_STACK_SIZE;} align(4)    > DRAM_M4F_MEM
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)
    .undStack   : {. = . + __UND_STACK_SIZE;} align(4)      > DRAM_M4F_MEM
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)
    .svcStack   : {. = . + __SVC_STACK_SIZE;} align(4)      > DRAM_M4F_MEM
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
}
