/* linker options */
--fill_value=0
--stack_size=0x2000
--heap_size=0x1000

-e __VECS_ENTRY_POINT

MEMORY
{
    /*R5F_TCMA_SBL_RSVD(X): ORIGIN = 0x00000000 , LENGTH = 0x100*/
    RESET_VECTORS(X)    : ORIGIN = 0x00000000 , LENGTH = 0x00008000
    R5F_TCMB0(RWIX)     : ORIGIN = 0x41010000 , LENGTH = 0x00008000
    /* am64x MCMS3 locations */
    MSMC3 (RWIX)        : ORIGIN = 0x70000000 , LENGTH = 0x170000 /* ~1.5MB */
    /* Reserved for SYSFW Secure Proxy */
    MSMC3_H (RWIX)      : ORIGIN = 0x70170000 , LENGTH = 0x90000 /* ~0.5MB */
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

    .text       : {} palign(8)   > MSMC3
    .cinit      : {} palign(8)   > MSMC3
    .bss        : {} align(8)    > MSMC3
    .far        : {} align(8)    > MSMC3
    .const      : {} palign(8)   > MSMC3
    .data       : {} palign(128) > MSMC3
    .sysmem     : {} align(8)    > MSMC3
    .stack      : {} align(4)    > MSMC3
    .data_buffer: {} palign(128) > MSMC3

    .benchmark_buffer: (NOLOAD) {} align (8) > MSMC3
    
    /* SA sections */
    .scBufs     	: {} align(4)  		> MSMC3
    .saSrcBuffers     	: {} align(4)  		> MSMC3
    .saDstBuffers     	: {} align(4)  		> MSMC3
    
}
