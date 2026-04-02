/* linker options */
--fill_value=0
--stack_size=0x8000
--heap_size=0x40000

-e __VECS_ENTRY_POINT
--retain="*(.utilsCopyVecsToAtcm)"

MEMORY
{
    R5F_TCMA_SBL_RSVD(X): ORIGIN = 0x00000000 , LENGTH = 0x100
    RESET_VECTORS(X)    : ORIGIN = 0x41c40000 , LENGTH = 0x100  /* Bottom 256 KB used by SBL */
    R5F_TCMB0(RWIX)     : ORIGIN = 0x41010000 , LENGTH = 0x00008000
    /* Reserved Memory for ARM Trusted Firmware */
    MSMC3_ARM_FW   (RWIX)   : origin=0x70000000 length=0x40000         /* 256KB */
    /* j721e MCMS3 locations */
    MSMC3   (RWIX)          : origin=0x70040000 length=0x7B0000        /* 8MB - 320KB */
    /* Reserved Memory for DMSC Firmware */
    MSMC3_DMSC_FW  (RWIX)   : origin=0x707F0000 length=0x10000         /* 64KB */
    DDR0 (RWIX)         : ORIGIN = 0x80000000 , LENGTH = 0x80000000
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
    .utilsCopyVecsToAtcm                   : {} palign(8) > R5F_TCMB0

    .text       : {} palign(8)   > DDR0
    .cinit      : {} palign(8)   > DDR0
    .bss        : {} align(8)    > DDR0
    .far        : {} align(8)    > DDR0
    .const      : {} palign(8)   > DDR0
    .rodata     : {} palign(8)   > DDR0
    .data       : {} palign(128) > DDR0
    .sysmem     : {} align(8)    > DDR0
    .stack      : {} align(4)    > DDR0
    .data_buffer: {} palign(128) > DDR0
    .boardcfg_data  : {} palign(8)   > DDR0
    .const*         : {} palign(4)   > DDR0
    .bss*           : {} align(4)    > DDR0
    .saSrcBuffers: {} palign(128)    > DDR0
    .saDstBuffers: {} palign(128)    > DDR0
    .scBufs      : {} palign(128)    > DDR0
}
