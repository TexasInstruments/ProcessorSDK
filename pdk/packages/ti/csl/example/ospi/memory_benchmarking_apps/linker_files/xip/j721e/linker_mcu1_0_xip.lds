/*----------------------------------------------------------------------------*/
/* File: linker_mcu1_0_xip.lds                                                */
/* Description:                                                               */
/*    Link command file for J721E MCU1_0 view                                 */
/*                                                                            */
/* (c) Texas Instruments 2021-2023, All rights reserved.                           */
/*----------------------------------------------------------------------------*/
/*  History:                                                                  */
/*    Aug 26th, 2016 Original version .......................... Loc Truong   */
/*    Aug 01th, 2017 new TCM mem map  .......................... Loc Truong   */
/*    Nov 07th, 2017 Changes for R5F Init Code.................. Vivek Dhande */
/*    May 14th, 2021 running on J7VCL and HSM..... Caleb Robey & Karan Saxena */
/*    Aug 15th, 2021 port to FreeRTOS and new XIP bootflow....... Caleb Robey */
/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
/* Standard linker options                                                    */

--retain="*(.bootCode)"
--retain="*(.startupCode)"
--retain="*(.startupData)"
--retain="*(.intvecs)"
--retain="*(.intc_text)"
--retain="*(.rstvectors)"
--fill_value=0
-e __VECS_ENTRY_POINT

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */

--stack_size=0x8000
--heap_size=0x4000
--entry_point=_freertosresetvectors

-stack  0x8000  /* SOFTWARE STACK SIZE */
-heap   0x4000  /* HEAP AREA SIZE      */

/*-------------------------------------------*/
/*       Stack Sizes for various modes       */
/*-------------------------------------------*/
__IRQ_STACK_SIZE   = 0x1000;
__FIQ_STACK_SIZE   = 0x0100;
__ABORT_STACK_SIZE = 0x0100;
__UND_STACK_SIZE   = 0x0100;
__SVC_STACK_SIZE   = 0x2000;

--define FILL_PATTERN=0xFEAA55EF
--define FILL_LENGTH=0x100

/* 1 MB of MCU Domain MSRAM is split as shown below */
/* Size used  F0000 Number of slices 4 */
/*                                  Rounding Offset */
/*SBL?      Start   41C00000    245760  0   */
/*          End     41C3C000                */
/*MCU 10    Start   41C3C100    245760  100 */
/*          End     41C78100                */
/*MCU 11    Start   41C78200    245760  100 */
/*          End     41CB4200                */

--define TASK_SIZE=0x1000
--define TEXT_SIZE=0xA0000
--define BUF_SIZE=0x2000

--define OCMCRAM_START=0x41C82000
--define OCMCRAM_END=0x41CFFFFF
--define MSMC_START=0x70000000
--define XIP_MCU1_0_START=0x501c0000
--define XIP_MCU1_0_END=0x52000000
--define DDR_START=0x80000000
--define SBL_RUNTIME_MEM=0x41C01000


MEMORY
{
    MCU0_R5F_TCMB0_VECS (RWIX)	: origin=0x41010000	length=0x100
    MCU0_R5F_TCMB0 (RWIX)	: origin=0x41010100	length=0x8000-0x100
    BUF0                     : origin=SBL_RUNTIME_MEM + (BUF_SIZE*0)             length=BUF_SIZE
    BUF1                     : origin=SBL_RUNTIME_MEM + (BUF_SIZE*1)             length=BUF_SIZE
    BUF2                     : origin=SBL_RUNTIME_MEM + (BUF_SIZE*2)             length=BUF_SIZE
    BUF3                     : origin=SBL_RUNTIME_MEM + (BUF_SIZE*3)             length=BUF_SIZE
    BUF4                     : origin=SBL_RUNTIME_MEM + (BUF_SIZE*4)             length=BUF_SIZE
    BUF5                     : origin=SBL_RUNTIME_MEM + (BUF_SIZE*5)             length=BUF_SIZE
    BUF6                     : origin=SBL_RUNTIME_MEM + (BUF_SIZE*6)             length=BUF_SIZE
    BUF7                     : origin=SBL_RUNTIME_MEM + (BUF_SIZE*7)             length=BUF_SIZE
    BUF8                     : origin=SBL_RUNTIME_MEM + (BUF_SIZE*8)             length=BUF_SIZE
    BUF9                     : origin=SBL_RUNTIME_MEM + (BUF_SIZE*9)             length=BUF_SIZE
    BUF10                    : origin=SBL_RUNTIME_MEM + (BUF_SIZE*10)            length=BUF_SIZE
    BUF11                    : origin=SBL_RUNTIME_MEM + (BUF_SIZE*11)            length=BUF_SIZE
    BUF12                    : origin=SBL_RUNTIME_MEM + (BUF_SIZE*12)            length=BUF_SIZE
    BUF13                    : origin=SBL_RUNTIME_MEM + (BUF_SIZE*13)            length=BUF_SIZE
    BUF14                    : origin=SBL_RUNTIME_MEM + (BUF_SIZE*14)            length=BUF_SIZE
    BUF15                    : origin=SBL_RUNTIME_MEM + (BUF_SIZE*15)            length=BUF_SIZE
    BUF_CPY                  : origin=SBL_RUNTIME_MEM + (BUF_SIZE*16)            length=BUF_SIZE
    OCMC_RAM (RWIX)          : origin=OCMCRAM_START                              length=(OCMCRAM_END - OCMCRAM_START)
    TSK0                     : origin=XIP_MCU1_0_START + (TASK_SIZE*0)               length=TASK_SIZE
    TSK1                     : origin=XIP_MCU1_0_START + (TASK_SIZE*1)               length=TASK_SIZE
    TSK2                     : origin=XIP_MCU1_0_START + (TASK_SIZE*2)               length=TASK_SIZE
    TSK3                     : origin=XIP_MCU1_0_START + (TASK_SIZE*3)               length=TASK_SIZE
    TSK4                     : origin=XIP_MCU1_0_START + (TASK_SIZE*4)               length=TASK_SIZE
    TSK5                     : origin=XIP_MCU1_0_START + (TASK_SIZE*5)               length=TASK_SIZE
    TSK6                     : origin=XIP_MCU1_0_START + (TASK_SIZE*6)               length=TASK_SIZE
    TSK7                     : origin=XIP_MCU1_0_START + (TASK_SIZE*7)               length=TASK_SIZE
    TSK8                     : origin=XIP_MCU1_0_START + (TASK_SIZE*8)               length=TASK_SIZE
    TSK9                     : origin=XIP_MCU1_0_START + (TASK_SIZE*9)               length=TASK_SIZE
    TSK10                    : origin=XIP_MCU1_0_START + (TASK_SIZE*10)              length=TASK_SIZE
    TSK11                    : origin=XIP_MCU1_0_START + (TASK_SIZE*11)              length=TASK_SIZE
    TSK12                    : origin=XIP_MCU1_0_START + (TASK_SIZE*12)              length=TASK_SIZE
    TSK13                    : origin=XIP_MCU1_0_START + (TASK_SIZE*13)              length=TASK_SIZE
    TSK14                    : origin=XIP_MCU1_0_START + (TASK_SIZE*14)              length=TASK_SIZE
    TSK15                    : origin=XIP_MCU1_0_START + (TASK_SIZE*15)              length=TASK_SIZE
    TEXT_ARR                 : origin=XIP_MCU1_0_START + (TASK_SIZE*16)              length=TEXT_SIZE
    XIP_FLASH                : origin=XIP_MCU1_0_START + (TASK_SIZE*16) + TEXT_SIZE  length=(XIP_MCU1_0_END - (XIP_MCU1_0_START + (TASK_SIZE*16) + TEXT_SIZE))
    MSMC   (RWIX)            : origin=MSMC_START                                     length=0x80000

}  /* end of MEMORY */


SECTIONS
{
    .freertosrstvectors : {} palign(8) > MCU0_R5F_TCMB0_VECS

    .bootCode           : {} palign(8) > MCU0_R5F_TCMB0
    .startupCode        : {} palign(8) > MCU0_R5F_TCMB0
    .startupData        : {} palign(8) > MCU0_R5F_TCMB0, type = NOINIT

    .text_boot {
        r5_mpu_freertos.oer5f (.const:gCslR5MpuCfg)
        *ti.csl.aer5f*<*>(*Intr*)
    }                                  > MCU0_R5F_TCMB0

    GROUP {
            .text.hwi       : palign(8)
            .text.cache     : palign(8)
            .text.mpu       : palign(8)
            .text.boot      : palign(8)
            .text.abort     : palign(8)
        }                              > MCU0_R5F_TCMB0

    .sbl_mcu_1_0_resetvector               : {} palign(8)      > MCU0_R5F_TCMB0_VECS

    .text                                  : {} palign(8)      > TEXT_ARR
    .task_0                                : {} palign(8)      > TSK0
    .task_1                                : {} palign(8)      > TSK1
    .task_2                                : {} palign(8)      > TSK2
    .task_3                                : {} palign(8)      > TSK3
    .task_4                                : {} palign(8)      > TSK4
    .task_5                                : {} palign(8)      > TSK5
    .task_6                                : {} palign(8)      > TSK6
    .task_7                                : {} palign(8)      > TSK7
    .task_8                                : {} palign(8)      > TSK8
    .task_9                                : {} palign(8)      > TSK9
    .task_10                               : {} palign(8)      > TSK10
    .task_11                               : {} palign(8)      > TSK11
    .task_12                               : {} palign(8)      > TSK12
    .task_13                               : {} palign(8)      > TSK13
    .task_14                               : {} palign(8)      > TSK14
    .task_15                               : {} palign(8)      > TSK15
    .const.devgroup                        : {*(.const.devgroup*)} align(4)     > MSMC
    .const                                 : {} palign(8)      > MSMC
    .rodata                                : {} palign(8)      > MSMC
    .cinit                                 : {} palign(8)      > MCU0_R5F_TCMB0
    .pinit                                 : {} palign(8)      > MCU0_R5F_TCMB0
    .boardcfg_data                         : {} align(4)       > MCU0_R5F_TCMB0
    .bss                                   : {} align(4)       > MSMC
    .buf_cpy                               : {} align(4)       > BUF_CPY
    .data                                  : {} palign(128)    > OCMC_RAM
    .sysmem                                : {}                > MSMC
    .bss.devgroup                          : {*(.bss.devgroup*)} align(4)       > MSMC
    .buf_0                                 : {} align(4)       > BUF0
    .buf_1                                 : {} align(4)       > BUF1
    .buf_2                                 : {} align(4)       > BUF2
    .buf_3                                 : {} align(4)       > BUF3
    .buf_4                                 : {} align(4)       > BUF4
    .buf_5                                 : {} align(4)       > BUF5
    .buf_6                                 : {} align(4)       > BUF6
    .buf_7                                 : {} align(4)       > BUF7
    .buf_8                                 : {} align(4)       > BUF8
    .buf_9                                 : {} align(4)       > BUF9
    .buf_10                                : {} align(4)       > BUF10
    .buf_11                                : {} align(4)       > BUF11
    .buf_12                                : {} align(4)       > BUF12
    .buf_13                                : {} align(4)       > BUF13
    .buf_14                                : {} align(4)       > BUF14
    .buf_15                                : {} align(4)       > BUF15

    .stack                                 : {} align(4)       > OCMC_RAM (HIGH)

    .irqStack   : {. = . + __IRQ_STACK_SIZE;} align(4)      > OCMC_RAM (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)

    .fiqStack   : {. = . + __FIQ_STACK_SIZE;} align(4)      > OCMC_RAM (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)

    .abortStack : {. = . + __ABORT_STACK_SIZE;} align(4)    > OCMC_RAM (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)

    .undStack   : {. = . + __UND_STACK_SIZE;} align(4)      > OCMC_RAM (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)

    .svcStack   : {. = . + __SVC_STACK_SIZE;} align(4)      > OCMC_RAM (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
}
