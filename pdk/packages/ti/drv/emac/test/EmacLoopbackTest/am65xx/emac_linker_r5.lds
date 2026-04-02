
/* This is standard linker options used by examples applications and tests    */
/* Please refer user guide that came with this release for more details       */
/* on which sections could be used at load time and runtime                   */

--fill_value=0
/* Retain entry point */
-e __VECS_ENTRY_POINT

/*----------------------------------------------------------------------------*/
/* Memory Sections                                                            */
MEMORY
{
    /* Refer the user guide for details on persistence of these sections */
    /* Also, when these memories can be used by apps */
    MCU0_ATCM_NOT_USED          (R)     : origin=0x0        length=0x7FFF
    MCU0_BTCM_NOT_USED          (R)     : origin=0x41010000 length=0x7FFF
    /* Used by SBL, can be used after APPs is started */
    MCU_MSRAM_RSVD_UNUSED       (R)     : origin=0x41C00000 length=0x200
    MCU_MSRAM_RSVD_SBL          (RWIX)  : origin=0x41C00200 length=0x3DE00

    VECTORS                     (RWIX)  : origin=0x41C3E000 length=0x100
    RESET_VECTORS               (RWIX)  : origin=0x41C3E100 length=0x1000
    OCMRAM                      (RWIX)  : origin=0x41C3F100 length=0x40F00

    /* COMPUTE_CLUSTER0_MSMC_SRAM */
    MSMC3                       (RWIX) : origin=0x70000000 length=0x1EFC00
    /* The origin and length is determined by board cfg, */
    /* refer user guide for details */
    MSMC3_RSVD_DMSC             (RWIX)  : origin=0x701F0000 length=0x1000
    DDR0                        (RWIX)  : origin=0x80000000 length=0x7FFFFFE4

}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
    /* Place sysbios entry point - starts */
    .vecs : { *(.vecs) } palign(8) > VECTORS
    .vecs : { __VECS_ENTRY_POINT = .; } > VECTORS

    xdc.meta (COPY): { *(xdc.meta) } > OCMRAM
    .init_text  : {
                     boot.*(.text)
                     *(.text:ti_sysbios_family_arm_MPU_*)
                     *(.text:ti_sysbios_family_arm_v7r_Cache_*)
                  }  palign(8) > OCMRAM

    .text:xdc_runtime_Startup_reset__I: {} palign(8) > OCMRAM

    .bootCode       : {} palign(8) > OCMRAM
    .startupCode    : {} palign(8) > OCMRAM
    .startupData    : {} palign(8) > OCMRAM, type = NOINIT
    .utilsCopyVecsToAtcm : {} palign(8) > OCMRAM

    /* Place sysbios entry point - ends */

    .text    : {} palign(8) > MSMC3
    .const   : {} palign(8) > MSMC3
    .cinit   : {} palign(8) > MSMC3
    .pinit   : {} palign(8) > MSMC3

    /* For NDK packet memory, we need to map this sections before .bss*/
    .bss:NDK_MMBUFFER  (NOLOAD) {} ALIGN (128) > DDR0
    .bss:NDK_PACKETMEM (NOLOAD) {} ALIGN (128) > DDR0

    .bss    : {} align(8)   > DDR0
    .far    : {} align(8)   > DDR0
    .data   : {} palign(128)> MSMC3
    .boardcfg_data : {} palign(128) > MSMC3
    .sysmem : {} align(8)   > MSMC3
    .stack  : {} align(8)   > MSMC3  (HIGH)

    /* SA sections */
    .scBufs         : {} align(8)   > DDR0
    .saSrcBuffers   : {} align(8)   > DDR0
    .saDstBuffers   : {} align(8)   > DDR0

    /* LLD buffer for benchmarking */
    .benchmark_buffer (NOLOAD) {} ALIGN (8) > DDR0

    /* Additional sections settings */

}  /* end of SECTIONS */

/*----------------------------------------------------------------------------*/
/* Misc linker settings                                                       */


/*-------------------------------- END ---------------------------------------*/
