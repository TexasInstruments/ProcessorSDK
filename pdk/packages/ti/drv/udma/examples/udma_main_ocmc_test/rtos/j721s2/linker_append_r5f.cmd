--define MAIN_OCMC_RAM=0xD0000000

SECTIONS
{
    .udma_buffer_main_ocmc  : {} palign(128)    > MAIN_OCMC_RAM
}
