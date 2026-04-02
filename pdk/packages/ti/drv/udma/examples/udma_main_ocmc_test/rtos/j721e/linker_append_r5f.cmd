--define MAIN_OCMC_RAM_START=0x03610000

MEMORY
{
    MAIN_OCMC_RAM (RWIX)	 : origin=MAIN_OCMC_RAM_START	length=0x80000
}
SECTIONS
{
    .udma_buffer_main_ocmc  : {} palign(128)    > MAIN_OCMC_RAM
}
