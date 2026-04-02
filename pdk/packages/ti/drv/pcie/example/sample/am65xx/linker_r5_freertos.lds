/* This is standard linker options used by examples applications and tests    */
/* Please refer user guide that came with this release for more details       */
/* on which sections could be used at load time and runtime                   */

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
--retain="*(.dstBufSec)"
--retain="*(.statBuf)"

--fill_value=0
--stack_size=0x15000
--heap_size=0x8000
--entry_point=_freertosresetvectors

-stack  0x10000  /* SOFTWARE STACK SIZE */
-heap   0x8000  /* HEAP AREA SIZE      */

/*-------------------------------------------*/
/*       Stack Sizes for various modes       */
/*-------------------------------------------*/
__IRQ_STACK_SIZE   = 0x4000;
__FIQ_STACK_SIZE   = 0x0100;
__ABORT_STACK_SIZE = 0x0100;
__UND_STACK_SIZE   = 0x0100;
__SVC_STACK_SIZE   = 0x4000;

/*--------------------------------------------------------------------------*/
/*                               Memory Map                                 */
/*--------------------------------------------------------------------------*/
MEMORY
{
    /*--- Refer the user guide for details on persistence of these sections ---*/
    /*------------ Also, when these memories can be used by apps --------------*/
    MCU0_ATCM_NOT_USED          (R)     : ORIGIN = 0x00000000 LENGTH = 0x00007FFF
    MCU0_BTCM_NOT_USED          (R)     : ORIGIN = 0x41010000 LENGTH = 0x00007FFF
    /*----------- Used by SBL, can be used after APPs is started --------------*/
    MCU_MSRAM_RSVD_UNUSED       (R)     : ORIGIN = 0x41C00000 LENGTH = 0x00000200
    MCU_MSRAM_RSVD_SBL          (RWIX)  : ORIGIN = 0x41C00200 LENGTH = 0x0003DE00

    VECTORS                     (RWIX)  : ORIGIN = 0x41C3E000 LENGTH = 0x00000100
    RESET_VECTORS               (RWIX)  : ORIGIN = 0x41C3E100 LENGTH = 0x00001000
    OCMC_RAM                    (RWIX)  : ORIGIN = 0x41C3F100 LENGTH = 0x00040F00

    /*====================== COMPUTE_CLUSTER0_MSMC_SRAM =======================*/
    MSMC3                       (RWIX)  : ORIGIN = 0x70000000 LENGTH = 0x001EFC00
    /*---------- The ORIGIN  and LENGTH  is determined by board cfg, ----------*/
    /*------------------ refer user guide for details -------------------------*/
    MSMC3_RSVD_DMSC             (RWIX)  : ORIGIN = 0x701F0000 LENGTH = 0x00001000

    /*========================== AM65xx DDR LOCATION ==========================*/
    DDR0                        (RWIX)  : ORIGIN = 0x80000000 LENGTH = 0x7FFFFFE4

}

/*--------------------------------------------------------------*/
/*                     Section Configuration                    */
/*--------------------------------------------------------------*/
SECTIONS
{
    .intc_text               : {} palign(8) > VECTORS
    .freertosrstvectors      : {} palign(8) > RESET_VECTORS
    .bootCode                : {} palign(8) > OCMC_RAM
    .startupCode             : {} palign(8) > OCMC_RAM
    .startupData             : {} palign(8) > OCMC_RAM, type = NOINIT

    .text                    : {} palign(8) > DDR0
    GROUP {
        .text.hwi            : palign(8)
        .text.cache          : palign(8)
        .text.mpu            : palign(8)
        .text.boot           : palign(8)
    }                                       > DDR0
    .const                   : {} palign(8) > DDR0
    .cinit                   : {} palign(8) > DDR0
    .pinit                   : {} palign(8) > DDR0

    .bss                     : {} align(8)   > DDR0
    .far                     : {} align(8)   > DDR0
    .rodata                  : {} align(8)   > DDR0
    .data                    : {} palign(128)> DDR0
    .boardcfg_data           : {} palign(128)> DDR0
    .sysmem                  : {} align(8)   > DDR0

    /*------- USB ram disk dev-msc example ------*/
    .bss:extMemCache:ramdisk : {} align (32) > DDR0

    /*--------------- SA sections ---------------*/
    .scBufs                  : {} align(8)   > DDR0
    .saSrcBuffers            : {} align(8)   > DDR0
    .saDstBuffers            : {} align(8)   > DDR0

    .dstBufSec               : {} align(8)   > MSMC3
    .statBuf                 : {} align(8)   > OCMC_RAM

    /*------- LLD buffer for benchmarking -------*/
    .benchmark_buffer (NOLOAD) {} ALIGN (8) > DDR0

    .stack      : {}                            align(8)    > DDR0  (HIGH)

    .irqStack   : {. = . + __IRQ_STACK_SIZE;}   align(8)    > DDR0  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)

    .fiqStack  	: {. = . + __FIQ_STACK_SIZE;}   align(8)    > DDR0  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)

    .abortStack : {. = . + __ABORT_STACK_SIZE;} align(8)    > DDR0  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)

    .undStack  	: {. = . + __UND_STACK_SIZE;}   align(8)    > DDR0  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)

    .svcStack  	: {. = . + __SVC_STACK_SIZE;}   align(8)    > DDR0  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
}
