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

--fill_value=0
--stack_size=0x4000
--heap_size=0x8000
--entry_point=_freertosresetvectors

-stack  0x4000  /* SOFTWARE STACK SIZE */
-heap   0x8000  /* HEAP AREA SIZE      */

/*-------------------------------------------*/
/*       Stack Sizes for various modes       */
/*-------------------------------------------*/
__IRQ_STACK_SIZE   = 0x1000;
__FIQ_STACK_SIZE   = 0x0100;
__ABORT_STACK_SIZE = 0x0100;
__UND_STACK_SIZE   = 0x0100;
__SVC_STACK_SIZE   = 0x0100;

/*--------------------------------------------------------------------------*/
/*                               Memory Map                                 */
/*--------------------------------------------------------------------------*/
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
    OCMC_RAM             (RWIX) : ORIGIN = 0x41C82000 LENGTH = 0x7DA00
    OCMC_RAM_X509_HEADER (RWIX) : ORIGIN = 0x41CFFA00 LENGTH = 0x00000500

     /* Memory regions to implement RAT mapping for ECC aggregators */
    ECC_CFG_SCRATCH  (RWIX)  : origin=0x60000000 length=0x400            /* 4 K */
    ECC_CFG_SCRATCH1 (RWIX)  : origin=0x60000400 length=0x400            /* 4 K */
    ECC_CFG_SCRATCH2 (RWIX)  : origin=0x60001000 length=0x1000            /* 4 K */
    ECC_CFG_SCRATCH3 (RWIX)  : origin=0x60002000 length=0x1000           /* 4 K */
    ECC_CFG_SCRATCH4 (RWIX)  : origin=0x60003000 length=0x1000           /* 4 K */
    ECC_CFG_SCRATCH5 (RWIX)  : origin=0x60004000 length=0x1000           /* 4 K */
    ECC_CFG_SCRATCH6 (RWIX)  : origin=0x60005000 length=0x400            /* 4 K */
    ECC_CFG_SCRATCH7 (RWIX)  : origin=0x60005400 length=0x400            /* 4 K */
    ECC_CFG_SCRATCH8 (RWIX)  : origin=0x60005800 length=0x400            /* 4 K */
    ECC_CFG_SCRATCH9 (RWIX)  : origin=0x60005c00 length=0x400            /* 4 K */

    OCMC_RAM_SBL_RUNTIME (RWIX) : origin=0x41C00000 length=0x80000

    /*======================== MSMC3 LOCATIONS ===================*/
    /*---------- Reserved Memory for ARM Trusted Firmware -------*/
    MSMC3_ARM_FW  (RWIX)        : ORIGIN = 0x70000000 LENGTH = 0x00020000   /* 128KB       */
    MSMC3         (RWIX)        : ORIGIN = 0x70020000 LENGTH = 0x007D0000   /* 8MB - 192KB */
    /*------------- Reserved Memory for DMSC Firmware -----------*/
    MSMC3_DMSC_FW (RWIX)        : ORIGIN = 0x707F0000 LENGTH = 0x00010000   /* 64KB        */

    /*======================= DDR LOCATION =======================*/
    DDR0 (RWIX)                 : ORIGIN = 0x80000000 LENGTH = 0x80000000   /* 2GB */
}

/*--------------------------------------------------------------*/
/*                     Section Configuration                    */
/*--------------------------------------------------------------*/
SECTIONS
{
    .freertosrstvectors      : {} palign(8)      > VECTORS
    .bootCode        : {} palign(8)      > OCMC_RAM
    .startupCode     : {} palign(8)      > OCMC_RAM
    .startupData     : {} palign(8)      > OCMC_RAM, type = NOINIT
    .text            : {} palign(8)      > OCMC_RAM
    GROUP {
        .text.hwi    : palign(8)
        .text.cache  : palign(8)
        .text.mpu    : palign(8)
        .text.boot   : palign(8)
    }                                    > OCMC_RAM
    .const           : {} palign(8)      > OCMC_RAM
    .rodata          : {} palign(8)      > MSMC3              /*Using the MSMC region to accomodate for memory constraints due to ECC */
    .cinit           : {} palign(8)      > OCMC_RAM
    .bss             : {} align(4)       > OCMC_RAM_SBL_RUNTIME
    .far             : {} align(4)       > OCMC_RAM
    .data            : {} palign(128)    > OCMC_RAM
    .sysmem          : {}                > OCMC_RAM
    .data_buffer     : {} palign(128)    > OCMC_RAM
    .bss.devgroup    : {*(.bss.devgroup*)} align(4)       > OCMC_RAM
    .const.devgroup  : {*(.const.devgroup*)} align(4)     > OCMC_RAM

    /* Memory regions to implement RAT mapping for ECC aggregators */
    .my_aggr_reg           : {*(.my_aggr_reg)}         > ECC_CFG_SCRATCH, type = NOINIT
    .my_aggr_reg1          : {*(.my_aggr_reg1)}        > ECC_CFG_SCRATCH1, type = NOINIT
    .my_aggr_reg2          : {*(.my_aggr_reg2)}        > ECC_CFG_SCRATCH2, type = NOINIT
    .my_aggr_reg3          : {*(.my_aggr_reg3)}        > ECC_CFG_SCRATCH3, type = NOINIT
    .my_aggr_reg4          : {*(.my_aggr_reg4)}        > ECC_CFG_SCRATCH4, type = NOINIT
    .my_aggr_reg5          : {*(.my_aggr_reg5)}        > ECC_CFG_SCRATCH5, type = NOINIT
    .my_aggr_reg6          : {*(.my_aggr_reg6)}        > ECC_CFG_SCRATCH6, type = NOINIT
    .my_aggr_reg7          : {*(.my_aggr_reg7)}        > ECC_CFG_SCRATCH7, type = NOINIT
    .my_aggr_reg8          : {*(.my_aggr_reg8)}        > ECC_CFG_SCRATCH8, type = NOINIT
    .my_aggr_reg9          : {*(.my_aggr_reg9)}        > ECC_CFG_SCRATCH9, type = NOINIT
    .boardcfg_data   : {} align(4)       > OCMC_RAM

    /* USB or any other LLD buffer for benchmarking */
    .benchmark_buffer (NOLOAD) {} ALIGN (8) > DDR0

    .stack      : {} align(4)                               > OCMC_RAM  (HIGH)

    .irqStack   : {. = . + __IRQ_STACK_SIZE;} align(4)      > OCMC_RAM  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)

    .fiqStack   : {. = . + __FIQ_STACK_SIZE;} align(4)      > OCMC_RAM  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)

    .abortStack : {. = . + __ABORT_STACK_SIZE;} align(4)    > OCMC_RAM  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)

    .undStack   : {. = . + __UND_STACK_SIZE;} align(4)      > OCMC_RAM  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)

    .svcStack   : {. = . + __SVC_STACK_SIZE;} align(4)      > OCMC_RAM  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
}