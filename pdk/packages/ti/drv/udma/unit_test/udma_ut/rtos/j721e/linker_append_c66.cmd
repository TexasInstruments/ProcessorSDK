SECTIONS
{
    .udma_buffer_msmc       : {} palign(128)    > MSMC3
    .udma_buffer_ddr        : {} palign(128)    > DDR0
    .udma_buffer_internal   : {} palign(128)    > L2SRAM
    .udma_buffer_ospi       : {} palign(128)    > DDR0
}
