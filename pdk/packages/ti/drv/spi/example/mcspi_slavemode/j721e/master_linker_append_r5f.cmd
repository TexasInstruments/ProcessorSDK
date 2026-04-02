

/*--------------------------------------------------------------*/
/*                     Section Configuration                    */
/*--------------------------------------------------------------*/
SECTIONS
{
    .freertosrstvectors      : {} palign(8)      > VECTORS
    .bootCode        : {} palign(8)      > MCSPI_MASTER_OCMC_RAM
    .startupCode     : {} palign(8)      > MCSPI_MASTER_OCMC_RAM
    .startupData     : {} palign(8)      > MCSPI_MASTER_OCMC_RAM, type = NOINIT
    .text            : {} palign(8)      > MCSPI_MASTER_DDR0
    GROUP {
        .text.hwi    : palign(8)
        .text.cache  : palign(8)
        .text.mpu    : palign(8)
        .text.boot   : palign(8)
    }                                    > MCSPI_MASTER_DDR0
    .const           : {} palign(8)      > MCSPI_MASTER_DDR0
    .rodata          : {} palign(8)      > MCSPI_MASTER_DDR0
    .cinit           : {} palign(8)      > MCSPI_MASTER_DDR0
    .bss             : {} align(4)       > MCSPI_MASTER_DDR0
    .far             : {} align(4)       > MCSPI_MASTER_DDR0
    .data            : {} palign(128)    > MCSPI_MASTER_DDR0
    .sysmem          : {}                > MCSPI_MASTER_DDR0
    .data_buffer     : {} palign(128)    > MCSPI_MASTER_DDR0
    .bss.devgroup    : {*(.bss.devgroup*)} align(4)       > MCSPI_MASTER_DDR0
    .const.devgroup  : {*(.const.devgroup*)} align(4)     > MCSPI_MASTER_DDR0
    .boardcfg_data   : {} align(4)       > MCSPI_MASTER_DDR0

    /* USB or any other LLD buffer for benchmarking */
    .benchmark_buffer (NOLOAD) {} ALIGN (8) > MCSPI_MASTER_DDR0

    .stack      : {} align(4)                               > MCSPI_MASTER_DDR0  (HIGH)

    .irqStack   : {. = . + __IRQ_STACK_SIZE;} align(4)      > MCSPI_MASTER_DDR0  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)

    .fiqStack   : {. = . + __FIQ_STACK_SIZE;} align(4)      > MCSPI_MASTER_DDR0  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)

    .abortStack : {. = . + __ABORT_STACK_SIZE;} align(4)    > MCSPI_MASTER_DDR0  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)

    .undStack   : {. = . + __UND_STACK_SIZE;} align(4)      > MCSPI_MASTER_DDR0  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)

    .svcStack   : {. = . + __SVC_STACK_SIZE;} align(4)      > MCSPI_MASTER_DDR0  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
}