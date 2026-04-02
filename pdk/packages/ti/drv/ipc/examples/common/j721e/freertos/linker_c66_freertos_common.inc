/*=========================*/
/*     Linker Settings     */
/*=========================*/

-stack  0x2000      /* SOFTWARE STACK SIZE */
-heap   0x1000      /* HEAP AREA SIZE      */
--symbol_map _Hwi_intcVectorTable=Hwi_intcVectorTable

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
    .hwi_vect: {. = align(32); } > __BOOT ALIGN(0x400)
    .text:csl_entry:{}           > __BOOT
    .text:_c_int00          load > __BOOT ALIGN(0x400)
    .text:                       > __CORE_DDR_SPACE
    .stack:                      > __CORE_DDR_SPACE
    GROUP:                       > __CORE_DDR_SPACE
    {
        .bss:
        .neardata:
        .rodata:
    }
    .cio:                        > __CORE_DDR_SPACE
    .const:                      > __CORE_DDR_SPACE
    .data:                       > __CORE_DDR_SPACE
    .switch:                     > __CORE_DDR_SPACE
    .sysmem:                     > __CORE_DDR_SPACE
    .far:                        > __CORE_DDR_SPACE
    .args:                       > __CORE_DDR_SPACE
    .ppinfo:                     > __CORE_DDR_SPACE
    .ppdata:                     > __CORE_DDR_SPACE
    .ti.decompress:              > __CORE_DDR_SPACE
    .ti.handler_table:           > __CORE_DDR_SPACE

    /* COFF sections */
    .pinit:                      > __CORE_DDR_SPACE
    .cinit:                      > __CORE_DDR_SPACE

    /* EABI sections */
    .binit:                      > __CORE_DDR_SPACE
    .init_array:                 > __CORE_DDR_SPACE
    .fardata:                    > __CORE_DDR_SPACE
    .c6xabi.exidx:               > __CORE_DDR_SPACE
    .c6xabi.extab:               > __CORE_DDR_SPACE

    .csl_vect:                   > __CORE_DDR_SPACE
  
    ipc_data_buffer:             > __CORE_DDR_SPACE type=NOLOAD
    .resource_table: 
    { 
        __RESOURCE_TABLE = .;
    }                            > __CORE_EXT_DATA_BASE

    .tracebuf : {} align(1024)   > __CORE_EXT_DATA
}
