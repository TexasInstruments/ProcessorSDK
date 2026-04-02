/* linker options */
--fill_value=0
--stack_size=0x2000
--heap_size=0x1000

-e __VECS_ENTRY_POINT

MEMORY
{
    R5F_TCMA_SBL_RSVD(X): ORIGIN = 0x00000000 , LENGTH = 0x100
    R5F_TCMB0(RWIX)     : ORIGIN = 0x41010000 , LENGTH = 0x00008000

    /* Refer the user guide for details on persistence of these sections */
    OCMC_RAM_BOARD_CFG (RWIX)   : origin=0x41C80000 length=0x2000
    OCMC_RAM_SCISERVER (RWIX)   : origin=0x41C82000 length=0x60000
    RESET_VECTORS (X)           : origin=0x41CE2000 length=0x100
    OCMC_RAM (RWIX)             : origin=0x41CE2100 length=0x1DA00
    OCMC_RAM_X509_HEADER (RWIX) : origin=0x41CFFB00 length=0x500

    DDR0 (RWIX)         : ORIGIN = 0x80000000 , LENGTH = 0x2A000000
    DDR_NOCACHE (RWIX)  : ORIGIN = 0xAA000000 , LENGTH = 0x56000000
    MSMC_NOCACHE (RWIX) : ORIGIN = 0x707C0000 , LENGTH = 0x20000  /* 128 KB */
}

SECTIONS
{
    .vecs       : {
        __VECS_ENTRY_POINT = .;
    } palign(8) > RESET_VECTORS
    .text_boot {
        *boot.aer5f*<*boot.o*>(.text)
     }  palign(8)   > R5F_TCMB0
    .text:xdc_runtime_Startup_reset__I     : {} palign(8) > R5F_TCMB0
    .text:ti_sysbios_family_arm_v7r_Cache* : {} palign(8) > R5F_TCMB0
    .text:ti_sysbios_family_arm_MPU*       : {} palign(8) > R5F_TCMB0

    .text       : {} palign(8)   > DDR0
    .cinit      : {} palign(8)   > DDR0
    .bss        : {} align(8)    > DDR0
    .const      : {} palign(8)   > DDR0
    .data       : {} palign(128) > DDR0
    .sysmem     : {} align(8)    > DDR0
    .stack      : {} align(4)    > DDR0
    .data_buffer: {} palign(128) > DDR0

    .bss.devgroup*  : {} align(4) > DDR0
    .const.devgroup*: {} align(4) > DDR0
    .boardcfg_data  : {} align(4) > DDR0

    .bss:extMemCache:ramdisk  : {} palign(32) > DDR0

    /* USB or any other LLD buffer for benchmarking */
    .benchmark_buffer (NOLOAD): {}  palign(32) > DDR0

}
