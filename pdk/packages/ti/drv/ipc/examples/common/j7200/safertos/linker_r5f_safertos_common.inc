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
--entry_point=_axSafeRTOSresetVectors     /* C RTS boot.asm with added SVC handler    */

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
    /*=================== MCU R5F TCM Local View ======================*/
    MCU_R5F_TCMA_VECS       (X) : ORIGIN = 0x00000000 LENGTH = 0x00000100
    MCU_R5F_TCMA            (X) : ORIGIN = 0x00000100 LENGTH = 0x00007F00
    MCU_R5F_TCMB_VECS       (X) : ORIGIN = 0x41010000 LENGTH = 0x00000100
    MCU_R5F_TCMB            (X) : ORIGIN = 0x41010100 LENGTH = 0x00007F00

    /*==================== MCU R5F TCM SOC View =======================*/
    /*---------------------- MCU R5FSS0 CORE0 -------------------------*/
    MCU1_R5F0_ATCM_RSVD  (RWIX) : ORIGIN = 0x41000000 LENGTH = 0x00000040
    MCU1_R5F0_ATCM       (RWIX) : ORIGIN = 0x41000040 LENGTH = 0x00007FC0
 // MCU1_R5F0_BTCM_RSVD  (RWIX) : ORIGIN = 0x41010000 LENGTH = 0x00000040 (Documented only, to avoid conflict above with MCU_R5F_TCMB_VECS) 
 // MCU1_R5F0_BTCM       (RWIX) : ORIGIN = 0x41010040 LENGTH = 0x00007FC0 (Documented only, to avoid conflict above with MCU_R5F_TCMB)
    /*---------------------- MCU R5FSS0 CORE1 -------------------------*/
    MCU1_R5F1_ATCM_RSVD  (RWIX) : ORIGIN = 0x41400000 LENGTH = 0x00000040
    MCU1_R5F1_ATCM       (RWIX) : ORIGIN = 0x41400040 LENGTH = 0x00007FC0
    MCU1_R5F1_BTCM_RSVD  (RWIX) : ORIGIN = 0x41410000 LENGTH = 0x00000040
    MCU1_R5F1_BTCM       (RWIX) : ORIGIN = 0x41410040 LENGTH = 0x00007FC0
    /*---------------------- MAIN R5FSS0 CORE0 ------------------------*/
    MCU2_R5F0_ATCM_RSVD  (RWIX) : ORIGIN = 0x05C00000 LENGTH = 0x00000040
    MCU2_R5F0_ATCM       (RWIX) : ORIGIN = 0x05C00040 LENGTH = 0x00007FC0
    MCU2_R5F0_BTCM_RSVD  (RWIX) : ORIGIN = 0x05C10000 LENGTH = 0x00000040
    MCU2_R5F0_BTCM       (RWIX) : ORIGIN = 0x05C10040 LENGTH = 0x00007FC0
    /*---------------------- MAIN R5FSS0 CORE1 ------------------------*/
    MCU2_R5F1_ATCM_RSVD  (RWIX) : ORIGIN = 0x05D00000 LENGTH = 0x00000040
    MCU2_R5F1_ATCM       (RWIX) : ORIGIN = 0x05D00040 LENGTH = 0x00007FC0
    MCU2_R5F1_BTCM_RSVD  (RWIX) : ORIGIN = 0x05D10000 LENGTH = 0x00000040
    MCU2_R5F1_BTCM       (RWIX) : ORIGIN = 0x05D10040 LENGTH = 0x00007FC0
    
    /*===================== MCU MSRAM Locations =======================*/
    OCMC_RAM_BOARD_CFG   (RWIX) : ORIGIN = 0x41C80000 LENGTH = 0x00002000
    OCMC_RAM             (RWIX) : ORIGIN = 0x41C82000 LENGTH = 0x0007DB00
    OCMC_RAM_X509_HEADER (RWIX) : ORIGIN = 0x41CFFB00 LENGTH = 0x00000500

    /*=================== COMPUTE_CLUSTER0_MSMC_SRAM ==================*/
    /*---------- J7200 Reserved Memory for ARM Trusted Firmware -------*/
    MSMC3_ARM_FW         (RWIX) : ORIGIN = 0x70000000 LENGTH = 0x00020000   /* 128KB       */
    /*-----------------------------------------------------------------*/
    MSMC3                (RWIX) : ORIGIN = 0x70020000 LENGTH = 0x000D0000   /* 1MB - 192KB */
    /*------------- J7200 Reserved Memory for DMSC Firmware -----------*/
    MSMC3_DMSC_FW        (RWIX) : ORIGIN = 0x700F0000 LENGTH = 0x00010000   /* 64KB        */

    /*===================== J7200 DDR Locations =======================*/
    /* DDR Memory Map is included from memory_map_ddr.cmd -------------*/
}

/*--------------------------------------------------------------*/
/*                     Section Configuration                    */
/*--------------------------------------------------------------*/
SECTIONS
{
    GROUP
    {
        .safeRTOSrstvectors                                 : {} palign(8)
        .rstvectors                                         : {} palign(8)
    } > __VECS
    GROUP
    {
        .bootCode                                               : {} palign( 8 )
        .startupCode                                            : {} palign( 8 )
        .cinit                                                  : {} align( 32 )
        .pinit                                                  : {} align( 32 )
        .MPU_INIT_FUNCTION                                      : {} palign( 8 )
        .startupData                                            : {} palign( 8 ), type = NOINIT
    } > __BOOT
    

    .data_buffer     : {} palign(128)    > __CORE_DDR_SPACE

    /* USB or any other LLD buffer for benchmarking */
    .benchmark_buffer (NOLOAD) {} ALIGN (8) > __CORE_DDR_SPACE
    

    ipc_data_buffer (NOINIT) : {} palign(128)   > __CORE_DDR_SPACE
    .resource_table          :
    {
        __RESOURCE_TABLE = .;
    }                                           > __CORE_EXT_DATA_BASE

    .tracebuf                : {} align(1024)   > __CORE_EXT_DATA
    
    GROUP LOAD_START( lnkFlashStartAddr ), LOAD_END( lnkFlashEndAddr )
    {
        .KERNEL_FUNCTION LOAD_START( lnkKernelFuncStartAddr ),
                         LOAD_END( lnkKernelFuncEndAddr )       : {} palign( 0x10000 )
    } > __CORE_DDR_SPACE

    .unpriv_flash palign( 0x10000 ) :
    {
        *(.text)
        *(.rodata)
    } > __CORE_DDR_SPACE

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
    } > __CORE_DDR_SPACE   (HIGH)
}
