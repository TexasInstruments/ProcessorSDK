/*=========================*/
/*     Linker Settings     */
/*=========================*/
--retain="*(.bootCode)"
--retain="*(.startupCode)"
--retain="*(.startupData)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"
--retain="*(.utilsCopyVecsToAtcm)"

--fill_value=0
--stack_size=0x4000
--heap_size=0x8000
--entry_point=_freertosresetvectors

/*-------------------------------------------*/
/*       Stack Sizes for various modes       */
/*-------------------------------------------*/
__IRQ_STACK_SIZE   = 0x1000;
__FIQ_STACK_SIZE   = 0x0100;
__ABORT_STACK_SIZE = 0x0100;
__UND_STACK_SIZE   = 0x0100;
__SVC_STACK_SIZE   = 0x0100;

MEMORY
{
    VECTORS (X)                 : ORIGIN = 0x00000000 LENGTH = 0x00000040

    /*=================== MCU0_R5F_0 Local View ========================*/
    MCU0_R5F_TCMA  (X)          : ORIGIN = 0x00000040 LENGTH = 0x00007FC0
    MCU0_R5F_TCMB0 (RWIX)       : ORIGIN = 0x41010000 LENGTH = 0x00008000

    /*==================== MCU0_R5F_1 SoC View =========================*/
    MCU0_R5F1_ATCM (RWIX)       : ORIGIN = 0x41400000 LENGTH = 0x00008000
    MCU0_R5F1_BTCM (RWIX)       : ORIGIN = 0x41410000 LENGTH = 0x00008000

    /*- Refer user guide for details on persistence of these sections -*/
    OCMC_RAM_BOARD_CFG   (RWIX) : ORIGIN = 0x41C80000 LENGTH = 0x00002000
    OCMC_RAM_SCISERVER   (RWIX) : ORIGIN = 0x41C82000 LENGTH = 0x00060000
    OCMC_RAM             (RWIX) : ORIGIN = 0x41CE3100 LENGTH = 0x0001CA00
    OCMC_RAM_X509_HEADER (RWIX) : ORIGIN = 0x41CFFB00 LENGTH = 0x00000500

    /*======================== MCMS3 LOCATIONS ===================*/
    /*---------- Reserved Memory for ARM Trusted Firmware -------*/
    MSMC3_ARM_FW  (RWIX)        : ORIGIN = 0x70000000 LENGTH = 0x00040000   /* 256KB       */
    MSMC3         (RWIX)        : ORIGIN = 0x70040000 LENGTH = 0x003B0000   /* 4MB - 320KB */
    /*------------- Reserved Memory for DMSC Firmware -----------*/
    MSMC3_DMSC_FW (RWIX)        : ORIGIN = 0x703F0000 LENGTH = 0x00010000   /* 64KB        */

    /*======================= DDR LOCATION =======================*/
    DDR0 (RWIX)                 : ORIGIN = 0x80000000 LENGTH = 0x1000000   /* 16MB */
    DATA_BUFFER (RWIX)          : ORIGIN = 0x90000000, LENGTH = 0x70000000
}

SECTIONS
{
    .freertosrstvectors : {} palign(8)      > VECTORS
    .bootCode           : {} palign(8)      > OCMC_RAM
    .startupCode        : {} palign(8)      > OCMC_RAM
    .text.hwi           : {} palign(8)      > OCMC_RAM
    .startupData        : {} palign(8)      > OCMC_RAM, type = NOINIT
    .text               : {} palign(8)      > DDR0
    GROUP {             
        .text.hwi       : palign(8)
        .text.cache     : palign(8)
        .text.mpu       : palign(8)
        .text.boot      : palign(8)
    }                                       > DDR0
    .const              : {} palign(8)      > DDR0
    .rodata             : {} palign(8)      > DDR0
    .cinit              : {} palign(8)      > DDR0
    .bss                : {} align(4)       > DDR0
    .far                : {} align(4)       > DDR0
    .data               : {} palign(128)    > DDR0
    .sysmem             : {}                > DDR0
    .data_buffer        : {} palign(128)    > DDR0
    .bss.devgroup       : {*(.bss.devgroup*)} align(4)       > DDR0
    .const.devgroup     : {*(.const.devgroup*)} align(4)     > DDR0
    .boardcfg_data      : {} align(4)       > DDR0

    /* USB or any other LLD buffer for benchmarking */
    .benchmark_buffer (NOLOAD) {} ALIGN (8) > DDR0

    .stack      : {} align(4)                               > DDR0  (HIGH)

    .irqStack   : {. = . + __IRQ_STACK_SIZE;} align(4)      > DDR0  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)

    .fiqStack   : {. = . + __FIQ_STACK_SIZE;} align(4)      > DDR0  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)

    .abortStack : {. = . + __ABORT_STACK_SIZE;} align(4)    > DDR0  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)

    .undStack   : {. = . + __UND_STACK_SIZE;} align(4)      > DDR0  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)

    .svcStack   : {. = . + __SVC_STACK_SIZE;} align(4)      > DDR0  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
}
