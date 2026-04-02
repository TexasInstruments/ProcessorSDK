/*----------------------------------------------------------------------------*/
/* r5f_linker.cmd                                                             */
/*                                                                            */
/* (c) Texas Instruments 2020, All rights reserved.                           */
/*                                                                            */

/* USER CODE BEGIN (0) */
/* USER CODE END */
--retain="*(.intc_text)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"
-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x1000                              /* HEAP AREA SIZE                */

-u _freertosresetvectors

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x4000;
__FIQ_STACK_SIZE = 0x4000;
__ABORT_STACK_SIZE = 0x4000;
__UND_STACK_SIZE = 0x4000;
__SVC_STACK_SIZE = 0x4000;

/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY{
PAGE 0:
    /*  Reset Vectors base address(RESET_VECTORS) should be 64 bytes aligned  */
    RESET_VECTORS (X) : origin=0x00000000 length=0x100
    /* RESET_VECTORS (X) : origin=0x00020000 length=0x100 */
    TCMA_RAM (RX) : origin=0x00000100 length=0x00003F00
    TCMB_RAM (RW) : origin=0x00080000 length=0x00008000
    SBL_RESERVED_L2_RAM_0 (RW)   : origin=0x10200000 length=0x00030000
    SBL_RESERVED_L2_RAM_1 (RW)   : origin=0x10230000 length=0x00030000
    L2_RAM_0 (RW) : origin=0x10260000 length=0x00090000
    L3_RAM_0 (RW) : origin=0x88000000 length=0x001C8000
    L3_RAM_1 (RW) : origin=0x881C8000 length=0x001C8000
    HWA_RAM_0 (RW) : origin=0x82000000 length=0x00010000
    HWA_RAM_1 (RW) : origin=0x82010000 length=0x00010000

PAGE 1:
    L3_RAM_0 (RW) : origin=0x88000000 length=0x001C8000
    L3_RAM_1 (RW) : origin=0x881C8000 length=0x001C8000
}

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS{
    .freertosrstvectors    : {} palign(8)      > RESET_VECTORS
    .bootCode      : {} palign(8)      > TCMA_RAM
    .startupCode   : {} palign(8)      > TCMA_RAM
    .text.hwi   : {} palign(8)      > TCMA_RAM
    .startupData   : {} palign(8)      > TCMB_RAM, type = NOINIT

    /* The linker notation "X >> Y | Z" indicates section X is first allocated in Y
       and allowed to overflow into Z and can be split from Y to Z.
       The linker notation "X > Y | Z" indicates section X is first allocated in Y
       and allowed to overflow into Z and cannot be split from Y to Z. Some sections
       like bss are not allowed to be split so > notation is used for them
    */
    .text    : {} >> TCMA_RAM | L2_RAM_0

    .const   : {} > L2_RAM_0
    .switch  : {} > L2_RAM_0
    .cio:    : {} > SBL_RESERVED_L2_RAM_0 | L2_RAM_0
    .data:   : {} > L2_RAM_0

    .cinit   : {} > L2_RAM_0
    .pinit   : {} > L2_RAM_0
    .bss     : {} > SBL_RESERVED_L2_RAM_0 | L2_RAM_0
    .stack   : {} > TCMB_RAM | SBL_RESERVED_L2_RAM_0 | L2_RAM_0
    .sysmem  : {} > SBL_RESERVED_L2_RAM_0 | L2_RAM_0
    .benchmarkL1    : { } > TCMA_RAM     | TCMB_RAM
	.benchmarkL2    : { } > L2_RAM_0
    .irqStack  : {. = . + __IRQ_STACK_SIZE;} align(4)    > L2_RAM_0  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)
    .fiqStack  : {. = . + __FIQ_STACK_SIZE;} align(4)    > L2_RAM_0  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)
    .abortStack  : {. = . + __ABORT_STACK_SIZE;} align(4)> L2_RAM_0  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)
    .undStack  : {. = . + __UND_STACK_SIZE;} align(4)    > L2_RAM_0  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)
    .svcStack  : {. = . + __SVC_STACK_SIZE;} align(4)    > L2_RAM_0  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
}
/*----------------------------------------------------------------------------*/
