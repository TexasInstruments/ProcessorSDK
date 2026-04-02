
#define DEFAULT_SECTION_ALIGNMENT   ( 1K )

/*=========================*/
/*     Linker Settings     */
/*=========================*/

-stack  0x2000      /* SOFTWARE STACK SIZE */
-heap   0x1000      /* HEAP AREA SIZE      */
-u InterruptVectorTable

/*--------------------------------------------------------------------------*/
/*                               Memory Map                                 */
/*--------------------------------------------------------------------------*/
MEMORY
{
    /*===================== C66 SRAM Locations ========================*/
    L2SRAM                      : ORIGIN = 0x00800000 LENGTH = 0x00048000   /* 288KB LOCAL L2/SRAM */
    L1PSRAM                     : ORIGIN = 0x00E00000 LENGTH = 0x00008000   /* 32KB LOCAL L1P/SRAM */
    L1DSRAM                     : ORIGIN = 0x00F00000 LENGTH = 0x00008000   /* 32KB LOCAL L1D/SRAM */

    /*=================== COMPUTE_CLUSTER0_MSMC_SRAM ==================*/
    /*---------- J721E Reserved Memory for ARM Trusted Firmware -------*/
    MSMC3_ARM_FW         (RWIX) : ORIGIN = 0x70000000 LENGTH = 0x00020000   /* 128KB       */
    /*-----------------------------------------------------------------*/
    MSMC3                (RWIX) : ORIGIN = 0x70020000 LENGTH = 0x007D0000   /* 8MB - 192KB */
    /*------------- J721E Reserved Memory for DMSC Firmware -----------*/
    MSMC3_DMSC_FW        (RWIX) : ORIGIN = 0x707F0000 LENGTH = 0x00010000   /* 64KB        */

    /*===================== J721E DDR Locations =======================*/
    /* DDR Memory Map is included from memory_map_ddr.cmd -------------*/
}

/*--------------------------------------------------------------*/
/*                     Section Configuration                    */
/*--------------------------------------------------------------*/
SECTIONS
{    
   .interrupt_vectors : {. = align(1K); }  > __BOOT 
   .priv_code           palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_START( lnkKernelFuncStartAddr ),
                        LOAD_END( lnkKernelFuncEndAddr ) :
    {
        *(.KERNEL_FUNCTION)
    } > __CORE_DDR_SPACE

    .text:_c_int00      load >  __CORE_DDR_SPACE ALIGN(0x400)
    .text:              load >  __CORE_DDR_SPACE palign( DEFAULT_SECTION_ALIGNMENT ), 
                                     LOAD_START( lnkStartFlashAddress )

    .unpriv_rodata      palign( DEFAULT_SECTION_ALIGNMENT ) :
    {
        *(.const)
        *(.switch)
    } > __CORE_DDR_SPACE

    .cinit              LOAD_END( lnkEndFlashAddress )          : {} > __CORE_DDR_SPACE

    .KERNEL_DATA        palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_START( lnkKernelDataStartAddr ),
                        LOAD_END( lnkKernelDataEndAddr )        : {} > __CORE_DDR_SPACE

    GROUP               palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_START( lnkUnprivilegedDataStartAddr ),
                        LOAD_END( lnkUnprivilegedDataEndAddr )
    {
        .rodata:
        .bss:
        .neardata:
        .cio:
        .data:
        .sysmem:
        .fardata:
        .far:
    } > __CORE_DDR_SPACE

    .stack              palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_START( lnkStacksStartAddr )        : {} > __CORE_DDR_SPACE 
    ipc_data_buffer:             > __CORE_DDR_SPACE type=NOLOAD
    .resource_table: 
    { 
        __RESOURCE_TABLE = .;
    }                            > __CORE_DDR_SPACE

    .tracebuf : {} align(1024)   > __CORE_DDR_SPACE
}
