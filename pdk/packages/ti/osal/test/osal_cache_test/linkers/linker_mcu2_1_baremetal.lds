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
--entry_point=_resetvectors     /* Default C RTS boot.asm   */

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
    VECTORS (X)                        : origin=0x00000100 length=0x1000
    RESET_VECTORS (X)                  : origin=0x00000000 length=0x100
    MSMC3_MCU2_0   (RWIX)              : origin=0x70080000 length=0x6000
    DDR0_MCU2_0    (RWIX)              : origin=0x80000000 length=0x10000000
    MSMC3_MCU2_1   (RWIX)              : origin=0x70086000 length=0x6000
    DDR0_MCU2_1    (RWIX)              : origin=0x90000000 length=0x10000000
}

/* Section Configuration */
SECTIONS
{
    /* 'intvecs' and 'intc_text' sections shall be placed within */
    /* a range of +\- 16 MB */
    .intvecs       : {} palign(8)      > VECTORS
    .intc_text     : {} palign(8)      > VECTORS
    .rstvectors    : {} palign(8)      > RESET_VECTORS
    .bootCode      : {} palign(8)      > MSMC3_MCU2_1
    .startupCode   : {} palign(8)      > MSMC3_MCU2_1
    .startupData   : {} palign(8)      > MSMC3_MCU2_1, type = NOINIT
    .text          : {} palign(8)      > DDR0_MCU2_1
    .const         : {} palign(8)      > DDR0_MCU2_1
    .rodata        : {} palign(8)      > DDR0_MCU2_1
    .cinit         : {} palign(8)      > DDR0_MCU2_1
    .pinit         : {} palign(8)      > DDR0_MCU2_1
    .bss           : {} align(4)       > DDR0_MCU2_1
    .far           : {} align(4)       > DDR0_MCU2_1
    .data          : {} palign(128)    > DDR0_MCU2_1
    .sysmem        : {}                > DDR0_MCU2_1
    .data_buffer   : {} palign(128)    > DDR0_MCU2_1
    .bss.devgroup    : {*(.bss.devgroup*)} align(4)        > DDR0_MCU2_1
    .const.devgroup  : {*(.const.devgroup*)} align(4)      > DDR0_MCU2_1
    .boardcfg_data : {} align(4)       > DDR0_MCU2_1

    .stack      : {} align(4)       > DDR0_MCU2_1  (HIGH)
    .irqStack   : {. = . + __IRQ_STACK_SIZE;} align(4)      > DDR0_MCU2_1  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)
    .fiqStack   : {. = . + __FIQ_STACK_SIZE;} align(4)      > DDR0_MCU2_1  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)
    .abortStack : {. = . + __ABORT_STACK_SIZE;} align(4)    > DDR0_MCU2_1  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)
    .undStack   : {. = . + __UND_STACK_SIZE;} align(4)      > DDR0_MCU2_1  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)
    .svcStack   : {. = . + __SVC_STACK_SIZE;} align(4)      > DDR0_MCU2_1  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
}
