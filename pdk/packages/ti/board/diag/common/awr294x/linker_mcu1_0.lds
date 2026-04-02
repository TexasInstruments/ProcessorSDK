/*----------------------------------------------------------------------------*/
/* r5f_linker.cmd                                                             */
/*                                                                            */
/* (c) Texas Instruments 2020, All rights reserved.                           */
/*                                                                            */

/* USER CODE BEGIN (0) */
/* USER CODE END */
--retain="*(.intvecs)"
--retain="*(.intc_text)"
--retain="*(.rstvectors)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"
-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */


/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x1000;
__FIQ_STACK_SIZE = 0x1000;
__ABORT_STACK_SIZE = 0x1000;
__UND_STACK_SIZE = 0x1000;
__SVC_STACK_SIZE = 0x1000;

/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY{
PAGE 0:
    /*  Reset Vectors base address(RESET_VECTORS) should be 64 bytes aligned  */
    RESET_VECTORS (X) : origin=0x00000000 length=0x100
    /* RESET_VECTORS (X) : origin=0x00020000 length=0x100 */
    TCMA_RAM (RX) : origin=0x00000100 length=0x00007F00
    TCMB_RAM (RW) : origin=0x00080000 length=0x00008000
    SBL_RESERVED_L2_RAM (RW)   : origin=0x10200000 length=0x00060000
    CPPI_DESC (RW): origin=0x10260000 length=0x00001000
    L2_RAM (RW)   : origin=0x10261000 length=(0x00090000 - 0x00001000)
    L3_RAM (RW)   : origin=0x88000000 length=0x00300000
    L3_RAM_Ping (RW)   : origin=0x88300000 length=0x0040000
    L3_RAM_Pong (RW)   : origin=0x88340000 length=0x0040000
    HWA_RAM (RW)  : origin=0x82000000 length=0x00020000

PAGE 1:
    L3_RAM (RW)   : origin=0x88000000 length=0x00390000
}

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS{
    .rstvectors    : {} palign(8)      > RESET_VECTORS
    .bootCode      : {} palign(8)      > TCMA_RAM
    .startupCode   : {} palign(8)      > TCMA_RAM
    .startupData   : {} palign(8)      > TCMB_RAM, type = NOINIT

    /* The linker notation "X >> Y | Z" indicates section X is first allocated in Y
       and allowed to overflow into Z and can be split from Y to Z.
       The linker notation "X > Y | Z" indicates section X is first allocated in Y
       and allowed to overflow into Z and cannot be split from Y to Z. Some sections
       like bss are not allowed to be split so > notation is used for them
    */
    .l2ram   : {} > L2_RAM
    .l3ram   : {} > L3_RAM
	.hwaram  : {} > HWA_RAM
    .text    : {} >> TCMA_RAM | L2_RAM

    .const   : {} > L2_RAM
    .switch  : {} > L2_RAM
    .cio:    : {} > SBL_RESERVED_L2_RAM | L2_RAM
    .data:   : {} > L2_RAM

    .cinit   : {} > L2_RAM
    .pinit   : {} > L2_RAM
    .bss     : {} > SBL_RESERVED_L2_RAM | L2_RAM
    .stack   : {} > TCMB_RAM | SBL_RESERVED_L2_RAM | L2_RAM
    .sysmem  : {} > SBL_RESERVED_L2_RAM | L2_RAM
    uartbuffer : {} palign(8) > L3_RAM

    .bss:ENET_CPPI_DESC        (NOLOAD) {} ALIGN (128) > CPPI_DESC
    /* For Ethernet packet memory*/
    .bss:ENET_DMA_DESC_MEMPOOL (NOLOAD) {} ALIGN (128) > L3_RAM
    .bss:ENET_DMA_RING_MEMPOOL (NOLOAD) {} ALIGN (128) > L3_RAM
    .bss:ENET_DMA_PKT_MEMPOOL  (NOLOAD) {} ALIGN (128) > L3_RAM
    .bss:ENET_DMA_OBJ_MEM      (NOLOAD) {} ALIGN (128) > L3_RAM

    .irqStack  : {. = . + __IRQ_STACK_SIZE;} align(4)    > L2_RAM  (HIGH)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)
    .fiqStack  : {. = . + __FIQ_STACK_SIZE;} align(4)    > L2_RAM  (HIGH)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)
    .abortStack  : {. = . + __ABORT_STACK_SIZE;} align(4)> L2_RAM  (HIGH)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)
    .undStack  : {. = . + __UND_STACK_SIZE;} align(4)    > L2_RAM  (HIGH)
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)
    .svcStack  : {. = . + __SVC_STACK_SIZE;} align(4)    > L2_RAM  (HIGH)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
}
/*----------------------------------------------------------------------------*/

