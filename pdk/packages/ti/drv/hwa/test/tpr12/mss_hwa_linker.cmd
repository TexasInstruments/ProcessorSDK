/*----------------------------------------------------------------------------*/
/* r4f_linker.cmd                                                                 */
/*                                                                            */
/* (c) Texas Instruments 2016, All rights reserved.                           */
/*                                                                            */

/* USER CODE BEGIN (0) */
/* USER CODE END */


/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
MEMORY{
PAGE 0:
    VECTORS  (X)  : origin=0x00000000 length=0x00000100
    TCMA_RAM (RX) : origin=0x00000100 length=0x00007F00
    TCMB_RAM (RW) : origin=0x00080000 length=0x00008000
    SBL_RESERVED_L2_RAM (RW)   : origin=0x10200000 length=0x00060000
    L2_RAM_BANK0 (RW) : origin=0x10260000 length=0x00020000
    L2_RAM_BANK1 (RW) : origin=0x10280000 length=0x00070000
    L3_RAM (RW)   : origin=0x88000000 length=0x00300000
    HWA_RAM (RW)  : origin=0x28000000 length=0x00020000

PAGE 1:
    L3_RAM (RW)   : origin=0x88000000 length=0x00300000
}

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS{
    .intvecs : {} > VECTORS
    
    /* Allocate data preferentially in one bank and code (.text) in another,
       this can improve performance due to simultaneous misses from L1P
       and L1D caches to L2 SRAM.
       The linker notation "X >> Y | Z" indicates section X is first allocated in Y
       and allowed to overflow into Z and can be split from Y to Z.
       The linker notation "X > Y | Z" indicates section X is first allocated in Y
       and allowed to overflow into Z and cannot be split from Y to Z. Some sections
       like bss are not allowed to be split so > notation is used for them 
    */
    .text    : {} >> TCMA_RAM | L2_RAM_BANK0 | L2_RAM_BANK1
    
    .const   : {} >> L2_RAM_BANK1 | L2_RAM_BANK0
    .switch  : {} >> L2_RAM_BANK1 | L2_RAM_BANK0
    .cio:    : {} >> SBL_RESERVED_L2_RAM | L2_RAM_BANK1 | L2_RAM_BANK0
    .data:   : {} >> L2_RAM_BANK1 | L2_RAM_BANK0
    
    .cinit   : {} > L2_RAM_BANK1 | L2_RAM_BANK0
    .pinit   : {} > L2_RAM_BANK1 | L2_RAM_BANK0
    .bss     : {} > SBL_RESERVED_L2_RAM | L2_RAM_BANK1 | L2_RAM_BANK0
    .stack   : {} > TCMB_RAM | SBL_RESERVED_L2_RAM | L2_RAM_BANK1 | L2_RAM_BANK0

    .boot:{
       *.*(*ti_sysbios_family_arm_MPU*)
       boot.aer5f*(*.text)
       *.*(*startup*)
       *.*(*Startup*)
       *.*(*Cache*)
     } > TCMA_RAM | TCMB_RAM
    .l3ram   : {} > L3_RAM
    systemHeap      : { } > L2_RAM_BANK1 | L2_RAM_BANK0
    .benchmarkL1    : { } > TCMA_RAM     | TCMB_RAM
	.benchmarkL2    : { } > L2_RAM_BANK1 | L2_RAM_BANK0
}
/*----------------------------------------------------------------------------*/


