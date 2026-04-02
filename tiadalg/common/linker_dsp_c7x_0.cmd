/****************************************************************************/
/*  lnk.cmd   v#.##                                                         */
/*  Copyright (c) 2014-%%%%  Texas Instruments Incorporated                 */
/*                                                                          */
/*  Usage: cl7x  <src files...> -z -o <out file> -m <map file> lnk.cmd      */
/*                                                                          */
/*    Description: THIS FILE IS A SAMPLE linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on a C7100 simulator.    */
/*                 Use it as a guideline.  You will want to change the      */
/*                 memory layout to match your specific C7xxx target        */
/*                 system.  You may want to change the allocation scheme    */
/*                 according to the size of your program.                   */
/*                                                                          */
/*    Notes: (1)   You must specify a directory in which your library is    */
/*                 located.  either add a -i"<directory>" line to this      */
/*                 file or use the system environment variable C_DIR to     */
/*                 specify a search path for the libraries.                 */
/*                                                                          */
/*           (2)   You may need to specify your library name with           */
/*                 -llibrary.lib if the correct library is not found        */
/*                 automatically.                                           */
/*                                                                          */
/****************************************************************************/
-c
-heap 0x400000
-stack 0x10000
--args 0x1000
--diag_suppress=10068 // "no matching section"
--cinit_compression=off

MEMORY
{
  /*448KB of L2 SRAM */
  L2SRAM_C7x_0           (RWX)   : org = 0x64800000, len = 0x070000
  L1DSRAM_C7x_0                  : org = 0x64E00000, len = 0x4000

  /*448KB of L2 SRAM */
  L2SRAM_C7x_1           (RWX)   : org = 0x65800000, len = 0x070000
  L1DSRAM_C7x_1                  : org = 0x65E00000, len = 0x4000

  /* 0.5MB of MSMC*/
  MSMC_P0          (RWX)         : org = 0x70080000, len = 0x80000

  /* 3MB of MSMC*/
  MSMC_P1          (RWX)         : org = 0x70100000, len = 0x300000

  /*79 MB*/
  DDR0_C7x_0_ANALYTICS (RWX)     : org = 0x88800000, len = 0x4FF0000

  /*79 MB*/
  DDR0_C7x_1_ANALYTICS (RWX)     : org = 0xD5A00000, len = 0x4FF0000

}

SECTIONS
{
    .ss_vectors                         > DDR0_C7x_0_ANALYTICS
    .text:_c_int00:                     > DDR0_C7x_0_ANALYTICS
    .text                               > DDR0_C7x_0_ANALYTICS
    .bss:app_log_mem      (NOLOAD) : {} > DDR0_C7x_0_ANALYTICS
    .bss                                > DDR0_C7x_0_ANALYTICS  /* Zero-initialized data */
    .data                               > DDR0_C7x_0_ANALYTICS  /* Initialized data */
    .cinit                              > DDR0_C7x_0_ANALYTICS  /* could be part of const */
    .init_array                         > DDR0_C7x_0_ANALYTICS  /* C++ initializations */
    .stack                align = 0x100 > DDR0_C7x_0_ANALYTICS  /* Temporarily in DDR instead of MSMC*/
    .args                               > DDR0_C7x_0_ANALYTICS
    .cio                                > DDR0_C7x_0_ANALYTICS
    .const                              > DDR0_C7x_0_ANALYTICS
    .switch                             > DDR0_C7x_0_ANALYTICS /* For exception handling. */
    .sysmem                             > DDR0_C7x_0_ANALYTICS /* heap */
    .ioBufs                             > DDR0_C7x_0_ANALYTICS
    .l1ScratchBuf align = 0x100         > L1DSRAM_C7x_0
    .l2ScratchBuf align = 0x8000        > L2SRAM_C7x_0
    .l3ScratchBuf align = 0x100         > MSMC_P1
    .l4ScratchBuf align = 0x100         > DDR0_C7x_0_ANALYTICS
    
    .dmem1                              > L2SRAM_C7x_0
    .ddrcacheSect                       > DDR0_C7x_0_ANALYTICS
}

/* Move frequently accessed sections to MSMC (ENABLED) */
SECTIONS
{

    GROUP (frequenty_accessed_functions)
    {
        .text:tidl_printf
        .text:DmaUtilsAutoInc3d_prepareTr
        .text:DmaUtilsAutoInc3d_configure
        .text:DmaUtilsAutoInc3d_trigger
        .text:DmaUtilsAutoInc3d_wait
        .text:DmaUtilsAutoInc3d_getTrMemReq
        .text:DmaUtilsAutoInc3d_setupTr
        .text:DmaUtilsAutoInc3d_setupTr
        .text:DmaUtilsAutoInc3d_getTotalBlockCount
        .text:Udma_chDruSubmitTr
        .text:Udma_chEnable
    } align = 0x80 > DDR0_C7x_0_ANALYTICS
}

