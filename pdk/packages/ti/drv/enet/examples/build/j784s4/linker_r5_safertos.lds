/*=========================*/
/*     Linker Settings     */
/*=========================*/

--retain="*(.bootCode)"
--retain="*(.startupCode)"
--retain="*(.startupData)"
--retain="*(.intvecs)"
--retain="*(.intc_text)"
--retain="*(.rstvectors)"
--retain="*(.safeRTOSrstvectors)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"

--fill_value=0
--diag_suppress=10063                   /* entry point not _c_int00 */
--stack_size=0x4000
--heap_size=0x8000
--entry_point=_axSafeRTOSresetVectors     /* C RTS boot.asm with added SVC handler	*/

-stack  0x4000  /* SOFTWARE STACK SIZE */
-heap   0x8000  /* HEAP AREA SIZE      */

/*-------------------------------------------*/
/*       Stack Sizes for various modes       */
/*-------------------------------------------*/
__IRQ_STACK_SIZE   = 0x1000;
__FIQ_STACK_SIZE   = 0x1000;
__ABORT_STACK_SIZE = 0x1000;
__UND_STACK_SIZE   = 0x1000;
__SVC_STACK_SIZE   = 0x1000;

/*--------------------------------------------------------------------------*/
/*                               Memory Map                                 */
/*--------------------------------------------------------------------------*/
MEMORY
{
    VECTORS (X)                 : ORIGIN = 0x00000000 LENGTH = 0x00000100

    /*=================== MCU0_R5F_0 Local View ========================*/
    MCU0_R5F_TCMA  (X)          : ORIGIN = 0x00000100 LENGTH = 0x00007F00
    MCU0_R5F_TCMB0 (RWIX)       : ORIGIN = 0x41010000 LENGTH = 0x00008000

    /*==================== MCU0_R5F_1 SoC View =========================*/
    MCU0_R5F1_ATCM (RWIX)       : ORIGIN = 0x41400000 LENGTH = 0x00008000
    MCU0_R5F1_BTCM (RWIX)       : ORIGIN = 0x41410000 LENGTH = 0x00008000

    /*- Refer user guide for details on persistence of these sections -*/
    OCMC_RAM_BOARD_CFG   (RWIX) : ORIGIN = 0x41C80000 LENGTH = 0x00002000
    OCMC_RAM_SCISERVER   (RWIX) : ORIGIN = 0x41C82000 LENGTH = 0x00060000
    OCMC_RAM             (RWIX) : ORIGIN = 0x41CE3100 LENGTH = 0x0001AA00
    OCMC_RAM_X509_HEADER (RWIX) : ORIGIN = 0x41CFDB00 LENGTH = 0x00000500

    /*========================J784S4 MCMS3 LOCATIONS ===================*/
    /*---------- J784S4 Reserved Memory for ARM Trusted Firmware -------*/
    MSMC3_ARM_FW  (RWIX)        : ORIGIN = 0x70000000 LENGTH = 0x00020000   /* 128KB       */
    MSMC3         (RWIX)        : ORIGIN = 0x70020000 LENGTH = 0x007D0000   /* 8MB - 192KB */
    /*------------- J784S4 Reserved Memory for DMSC Firmware -----------*/
    MSMC3_DMSC_FW (RWIX)        : ORIGIN = 0x707F0000 LENGTH = 0x00010000   /* 64KB        */

    /*======================= J784S4 DDR LOCATION =======================*/
    DDR0 (RWIX)                 : ORIGIN = 0x80000000 LENGTH = 0x80000000   /* 2GB */
}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
/*----------------------------------------------------------------------------*/
SECTIONS
{
/* Vector sections. */
    GROUP
    {
        .safeRTOSrstvectors                                 : {} palign(8)
        .rstvectors                                         : {} palign(8)
    } > VECTORS

/* Startup code sections. */
    GROUP
    {
        .bootCode                                               : {} palign( 8 )
        .startupCode                                            : {} palign( 8 )
        .cinit                                                  : {} align( 32 )
        .pinit                                                  : {} align( 32 )
        .MPU_INIT_FUNCTION                                      : {} palign( 8 )
        .startupData                                            : {} palign( 8 ), type = NOINIT
    } > MCU0_R5F_TCMA

/* Code sections. */
    GROUP LOAD_START( lnkFlashStartAddr ), LOAD_END( lnkFlashEndAddr )
    {
        .KERNEL_FUNCTION LOAD_START( lnkKernelFuncStartAddr ),
                         LOAD_END( lnkKernelFuncEndAddr )       : {} palign( 0x10000 )
    } > DDR0

    .unpriv_flash palign( 0x10000 ) :
    {
        *(.text)
        *(.rodata)
    } > DDR0

/* Data sections. */
    GROUP  palign( 0x10000 ), LOAD_START( lnkRamStartAddr ), LOAD_END( lnkRamEndAddr )
    {
        .bss                                                    : {} align( 4 )
        .far                                                    : {} align( 4 )
        .data                                                   : {} palign( 128 )
        .boardcfg_data                                          : {} palign( 128 )
        .sysmem                                                 : {}
        .bss.devgroup                                           : {*(.bss.devgroup*)} align( 4 )
        .const.devgroup                                         : {*(.const.devgroup*)} align( 4 )
        .KERNEL_DATA LOAD_START( lnkKernelDataStartAddr ),
                     LOAD_END( lnkKernelDataEndAddr )           : {} palign( 0x800 )


    /* Additional sections settings. */

        .data_buffer                                            : {} palign(128)
        /* USB or any other LLD buffer for benchmarking */
        .benchmark_buffer (NOLOAD)                              : {} ALIGN (8)

    /* Stack sections. */
        .stack  RUN_START( lnkStacksStartAddr ) : {}                            align(4)
        .irqStack                               : {. = . + __IRQ_STACK_SIZE;}   align(4)
        RUN_START(__IRQ_STACK_START)
        RUN_END(__IRQ_STACK_END)
        .fiqStack                               : {. = . + __FIQ_STACK_SIZE;}   align(4)
        RUN_START(__FIQ_STACK_START)
        RUN_END(__FIQ_STACK_END)
        .abortStack                             : {. = . + __ABORT_STACK_SIZE;} align(4)
        RUN_START(__ABORT_STACK_START)
        RUN_END(__ABORT_STACK_END)
        .undStack                               : {. = . + __UND_STACK_SIZE;}   align(4)
        RUN_START(__UND_STACK_START)
        RUN_END(__UND_STACK_END)
        .svcStack    END( lnkStacksEndAddr )    : {. = . + __SVC_STACK_SIZE;}   align(4)
        RUN_START(__SVC_STACK_START)
        RUN_END(__SVC_STACK_END)
    } > DDR0
}

/*-------------------------------- END ---------------------------------------*/
