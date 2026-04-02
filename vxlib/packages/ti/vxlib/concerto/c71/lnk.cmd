/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 2007-2012 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission to use, copy, modify, or distribute this software,            |**
**| whether in part or in whole, for any purpose is forbidden without        |**
**| a signed licensing agreement and NDA from Texas Instruments              |**
**| Incorporated (TI).                                                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

-stack 0x2000
-heap 0x800000

MEMORY {
   L1D_SRAM     : origin = 0x64E00000,  len = 0x004000
   L2_SRAM      : origin = 0x64800000,  len = 0x080000
   SL2_SRAM     : origin = 0x70000100,  len = 0x7fff00   // MSMCSRAM
   EXT_MEM      : origin = 0x80000000,  len = 0x0A00000
   EXTMEMPAGE (RWX): org = 0x80A00000, len = 0x200000

}

#if defined(LINK_L2SRAM)

SECTIONS
{

  GROUP (SRAM_SECTIONS) {

    .kernel {
      vxlib_C7*<*.o*> (.text:optimized)
    } SIZE(_kernel_size)

    .kernel_data {
      vxlib_C7*<*.o*> (.bss)
      vxlib_C7*<*.o*> (.const)
      vxlib_C7*<*.o*> (.neardata)
      vxlib_C7*<*.o*> (.rodata)
      vxlib_C7*<*.o*> (.far)
      vxlib_C7*<*.o*> (.fardata)
      vxlib_C7*<*.o*> (.switch)
    } SIZE(_data_size)

    .rts_mem {
      rts*<memcpy64.o*> (.text)
      rts*<memset.o*> (.text)
      rts*<strasg.o*> (.text)
      rts*<divi.o*> (.text)
      rts*<divu.o*> (.text)
      rts*<divd.o*> (.text)
      rts*<divf.o*> (.text)
      rts*<modf.o*> (.text)
      rts*<fixfu.o*> (.text)
      rts*<fixdu.o*> (.text)
      rts*<pow.o*> (.text)
      rts*<atan.o*> (.text)
      rts*<atan2.o*> (.text)
      rts*<sqrt.o*> (.text)
      rts*<round.o*> (.text)
      rts*<remu.o*> (.text)
      rts*<frcmpyd_div.o*> (.text)
      rts*<llshift.o*> (.text)
      rts*<trunc.o*> (.text)
      rts*<errno.o*> (.text)
      rts*<ldexp.o*> (.text)
      rts*<frexp.o*> (.text)
    } SIZE(_rts_size)

    .stack
    .sram_start

  } START(_sram_start) > L2_SRAM

    vectors   :> EXT_MEM
    .cinit    :> EXT_MEM
    .cio      :> EXT_MEM
    .data     :> EXT_MEM
    .text     :> EXT_MEM
    .bss      :> EXT_MEM
    .const    :> EXT_MEM
    .far      :> EXT_MEM
    .fardata  :> EXT_MEM
    .neardata :> EXT_MEM
    .rodata   :> EXT_MEM
    .sysmem   :> EXT_MEM
    .switch   :> EXT_MEM
}

#elif defined(AUTOTEST)

SECTIONS
{
  .sram_start START(_sram_start) > L2_SRAM NOINIT

  .kernel {
    vxlib_C7*<*.o*> (.text:optimized)
  } SIZE(_kernel_size) > SL2_SRAM

  .kernel_data {
    vxlib_C7*<*.o*> (.bss)
    vxlib_C7*<*.o*> (.const)
    vxlib_C7*<*.o*> (.neardata)
    vxlib_C7*<*.o*> (.rodata)
    vxlib_C7*<*.o*> (.far)
    vxlib_C7*<*.o*> (.fardata)
    vxlib_C7*<*.o*> (.switch)
  } SIZE(_data_size) > EXT_MEM

    vectors   :> EXT_MEM
    .cinit    :> EXT_MEM
    .cio      :> EXT_MEM
    .data     :> EXT_MEM
    .text     :> EXT_MEM
    .stack    :> EXT_MEM
    .bss      :> EXT_MEM
    .const    :> EXT_MEM
    .far      :> EXT_MEM
    .fardata  :> EXT_MEM
    .neardata :> EXT_MEM
    .rodata   :> EXT_MEM
    .sysmem   :> EXT_MEM
    .switch   :> EXT_MEM
}

#else

SECTIONS
{
  .sram_start START(_sram_start) > L2_SRAM NOINIT

  .kernel {
    *.obj (.text:optimized)
  } SIZE(_kernel_size) > EXT_MEM

  .kernel_data SIZE(_data_size)

    vectors   :> EXT_MEM
    .cinit    :> EXT_MEM
    .cio      :> EXT_MEM
    .data     :> EXT_MEM

  .data:pte:pte_lvl1_0000000000000000 > EXTMEMPAGE
  .data:pte:pte_lvl0 > EXTMEMPAGE
  .data:pte:pte_lvl2_40000000 > EXTMEMPAGE
  .data:pte:pte_lvl2_80000000 > EXTMEMPAGE
  .data:pte:pte_lvl3_64800000 > EXTMEMPAGE
  .data:pte:pte_lvl3_64E00000 > EXTMEMPAGE
  .data:pte:pte_lvl3_64D00000 > EXTMEMPAGE
  .data:pte:pte_lvl3_64D20000 > EXTMEMPAGE
  .data:pte:pte_lvl3_64D40000 > EXTMEMPAGE
  .data:pte:pte_lvl3_64D60000 > EXTMEMPAGE
  .data:pte:pte_lvl3_64D80000 > EXTMEMPAGE
  .data:pte:pte_lvl3_64DA0000 > EXTMEMPAGE
  .data:pte:pte_lvl3_64DC0000 > EXTMEMPAGE
  .data:pte:pte_lvl3_64DE0000 > EXTMEMPAGE
  .data:pte:pte_lvl3_70000000 > EXTMEMPAGE
  .data:pte:pte_lvl3_70200000 > EXTMEMPAGE
  .data:pte:pte_lvl3_70400000 > EXTMEMPAGE
  .data:pte:pte_lvl3_70600000 > EXTMEMPAGE
  .data:pte:pte_lvl3_80000000 > EXTMEMPAGE
  .data:pte:pte_lvl3_80200000 > EXTMEMPAGE
  .data:pte:pte_lvl3_80400000 > EXTMEMPAGE
  .data:pte:pte_lvl3_80600000 > EXTMEMPAGE
  .data:pte:pte_lvl3_80800000 > EXTMEMPAGE
  .data:pte:pte_lvl3_80A00000 > EXTMEMPAGE
  .data:pte:pte_lvl3_80C00000 > EXTMEMPAGE
  .data:pte:pte_lvl3_80E00000 > EXTMEMPAGE
  .data:pte:pte_lvl3_81000000 > EXTMEMPAGE
  .data:pte:pte_lvl3_81200000 > EXTMEMPAGE
  .data:pte:pte_lvl3_81400000 > EXTMEMPAGE
  .data:pte:pte_lvl3_81600000 > EXTMEMPAGE
  .data:pte:pte_lvl3_81800000 > EXTMEMPAGE
  .data:pte:pte_lvl3_81A00000 > EXTMEMPAGE
  .data:pte:pte_lvl3_81C00000 > EXTMEMPAGE
  .data:pte:pte_lvl3_81E00000 > EXTMEMPAGE

    .text     :> EXT_MEM
    .stack    :> EXT_MEM
    .bss      :> EXT_MEM
    .const    :> EXT_MEM
    .far      :> EXT_MEM
    .fardata  :> EXT_MEM
    .neardata :> EXT_MEM
    .rodata   :> EXT_MEM
    .sysmem   :> EXT_MEM
    .switch   :> EXT_MEM
}

#endif
