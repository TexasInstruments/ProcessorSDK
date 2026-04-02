/*=========================*/
/*     Linker Settings     */
/*=========================*/

--ram_model
-heap  0x20000
-stack 0x20000
--args 0x1000
--diag_suppress=10068 /* "no matching section" */
--cinit_compression=off
-e _c_int00_secure


/*---------------------------------------------------------*/
/*                         Memory Map                      */
/*---------------------------------------------------------*/
MEMORY
{
    /*=================== C7x L2 SRAM ====================*/
    L2SRAM  (RWX) : ORIGIN = 0x64800000 LENGTH = 0x00070000
    /*================ J721S2 DDR Locations ==============*/
    /* DDR Memory Map is included from memory_map_ddr.cmd */
}

/*--------------------------------------------------------------*/
/*                     Section Configuration                    */
/*--------------------------------------------------------------*/
SECTIONS
{
    boot:
    {
      boot.*<boot.oe71>(.text)
    } load                  > __CORE_TEXT       ALIGN(0x200000)
    .vecs                   > __CORE_DDR_SPACE  ALIGN(0x400000)
    .secure_vecs            > __CORE_DDR_SPACE  ALIGN(0x200000)
    .text:_c_int00_secure   > __CORE_DDR_SPACE  ALIGN(0x200000)
    .text                   > __CORE_DDR_SPACE  ALIGN(0x200000)

    .bss        >       __CORE_DDR_SPACE  /* Zero-initialized data */
    .data       >       __CORE_DDR_SPACE  /* Initialized data */

    .cinit      >       __CORE_DDR_SPACE  /* could be part of const */
    .init_array >       __CORE_DDR_SPACE  /* C++ initializations */
    .stack      >       __CORE_DDR_SPACE  ALIGN(0x20000) /* Needed for Nested ISR handling */
    .args       >       __CORE_DDR_SPACE
    .cio        >       __CORE_DDR_SPACE
    .const      >       __CORE_DDR_SPACE
    .switch     >       __CORE_DDR_SPACE /* For exception handling. */
    .sysmem     >       __CORE_DDR_SPACE /* heap */

    GROUP:      >  __CORE_DDR_SPACE
    {
        .data.Mmu_tableArray          : type=NOINIT
        .data.Mmu_tableArraySlot      : type=NOINIT
        .data.Mmu_level1Table         : type=NOINIT
        .data.Mmu_tableArray_NS       : type=NOINIT
        .data.Mmu_tableArraySlot_NS   : type=NOINIT
        .data.Mmu_level1Table_NS      : type=NOINIT
    }

    ipc_data_buffer:        > __CORE_DDR_SPACE
    .resource_table:
    {
        __RESOURCE_TABLE = .;
    }                       > __CORE_EXT_D
}
