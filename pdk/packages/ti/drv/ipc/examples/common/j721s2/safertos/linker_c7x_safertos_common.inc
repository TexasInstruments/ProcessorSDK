/*=========================*/
/*     Linker Settings     */
/*=========================*/

--ram_model
--args 0x1000
--diag_suppress=10068 /* "no matching section" */
--cinit_compression=off
-e _c_int00_secure

-heap  0x20000      /* HEAP AREA SIZE                */
-stack 0x20000      /* SOFTWARE STACK SIZE           */

#include "memory_map_defines.inc"

/*--------------------------------------------------------------------------*/
/*                               Memory Map                                 */
/*--------------------------------------------------------------------------*/
MEMORY
{
    L2SRAM (RWX):  org = 0x64800000,          len = 0x00070000

    /*===================== J721E DDR Locations =======================*/
    /* DDR Memory Map is included from memory_map_ddr.cmd -------------*/
}

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
/*----------------------------------------------------------------------------*/
SECTIONS
{
    boot:
    {
      boot.*<boot.oe71>(.text)
    } load > __CORE_TEXT ALIGN(0x200000)
    
    /* Code sections. */
    GROUP LOAD_START( lnkStartFlashAddress ),
          LOAD_END( lnkEndFlashAddress )
    {
        .interrupt_vectors      : {} ALIGN(0x400000)
        .vecs                   : {} ALIGN(0x400000)
        .secure_vecs            : {} ALIGN(0x200000)
        .text:_c_int00_secure   : {} ALIGN(0x200000)
        .text                   : {} ALIGN(0x200000)

        .cinit                  : {}  /* could be part of const */
        .const                  : {}
        .KERNEL_FUNCTION LOAD_START( lnkKernelFuncStartAddr ),
                         LOAD_END( lnkKernelFuncEndAddr )
                                : {} palign( 0x10000 )
    } > __CORE_DDR_SPACE

    /* Data sections. */
    GROUP  palign( 0x10000 ), LOAD_START( lnkRamStartAddr ), LOAD_END( lnkRamEndAddr )
    {
        .bss                    : {}  /* Zero-initialized data */
        .data                   : {}  /* Initialized data */

        .init_array             : {}  /* C++ initializations */
        .stack                  : {} ALIGN(0x10000)
        .args                   : {}
        .cio                    : {}
        .switch                 : {} /* For exception handling. */
        .sysmem      /* heap */
        .KERNEL_DATA LOAD_START( lnkKernelDataStartAddr ),
                     LOAD_END( lnkKernelDataEndAddr )
                                : {} palign( 0x800 )
    } > __CORE_DDR_SPACE

    GROUP:              >  __CORE_DDR_SPACE
    {
        .data.Mmu_tableArray          : type=NOINIT
        .data.Mmu_tableArraySlot      : type=NOINIT
        .data.Mmu_level1Table         : type=NOINIT
        .data.Mmu_tableArray_NS       : type=NOINIT
        .data.Mmu_tableArraySlot_NS   : type=NOINIT
        .data.Mmu_level1Table_NS      : type=NOINIT
    }

    ipc_data_buffer:       > __CORE_DDR_SPACE
    .resource_table: { __RESOURCE_TABLE = .;} > __CORE_EXT_D
}

/*-------------------------------- END ---------------------------------------*/
