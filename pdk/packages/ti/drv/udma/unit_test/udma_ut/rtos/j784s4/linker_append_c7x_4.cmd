#define DDR0_ALLOCATED_START  0xA0000000
#define C7X_ALLOCATED_START DDR0_ALLOCATED_START + 0x08000000

#define C7X_EXT_DATA_BASE   (C7X_ALLOCATED_START + 0x00100000)
#define C7X_MEM_TEXT_BASE   (C7X_ALLOCATED_START + 0x00200000)
#define C7X_MEM_DATA_BASE   (C7X_ALLOCATED_START + 0x00300000)
#define C7X_DDR_SPACE_BASE  (C7X_ALLOCATED_START + 0x00400000)

MEMORY
{
    C7X_DDR_SPACE_2: org = (C7X_DDR_SPACE_BASE + 0x00C00000),  len = (0x08000000 - 0x00C00000)
    MSMC3 (RWIX):  org = 0x70400000,          len = 0x00080000   /* 512 KB per core */
    L2SRAM (RWX):  org = 0x67800000,          len = 0x00070000
}

SECTIONS
{
    .udma_buffer_msmc       >       MSMC3
    .udma_buffer_ddr        >       C7X_DDR_SPACE_2
    .udma_buffer_internal   >       L2SRAM
    .udma_buffer_ospi       >       C7X_DDR_SPACE_2
}
