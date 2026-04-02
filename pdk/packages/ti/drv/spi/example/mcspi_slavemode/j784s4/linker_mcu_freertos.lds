/*=========================*/
/*     Linker Settings     */
/*=========================*/

--retain="*(.bootCode)"
--retain="*(.startupCode)"
--retain="*(.startupData)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"

--fill_value=0
--stack_size=0x4000
--heap_size=0x8000
--entry_point=_freertosresetvectors

-stack  0x4000  /* SOFTWARE STACK SIZE */
-heap   0x8000  /* HEAP AREA SIZE      */

/*-------------------------------------------*/
/*       Stack Sizes for various modes       */
/*-------------------------------------------*/
__IRQ_STACK_SIZE   = 0x1000;
__FIQ_STACK_SIZE   = 0x0100;
__ABORT_STACK_SIZE = 0x0100;
__UND_STACK_SIZE   = 0x0100;
__SVC_STACK_SIZE   = 0x0100;

#define DDR0_ALLOCATED_START 0x80000000
#define DDR0_ALLOCATED_SIZE  0x10000000

#define OCMC_ALLOCATED_START 0x41CE3100
#define OCMC_ALLOCATED_SIZE  0x00001000

#define MCU2_0_DDR0_ALLOCATED_START   DDR0_ALLOCATED_START
#define MCU2_0_OCMC_ALLOCATED_START   OCMC_ALLOCATED_START


#define MCU2_1_DDR0_ALLOCATED_START   MCU2_0_DDR0_ALLOCATED_START + DDR0_ALLOCATED_SIZE
#define MCU2_1_OCMC_ALLOCATED_START   MCU2_0_OCMC_ALLOCATED_START + OCMC_ALLOCATED_SIZE

/*--------------------------------------------------------------------------*/
/*                               Memory Map                                 */
/*--------------------------------------------------------------------------*/
MEMORY
{
    VECTORS (X)                 : ORIGIN = 0x00000000 LENGTH = 0x00000040

    /*=================== MCU0_R5F_0 Local View ========================*/
    MCU0_R5F_TCMA  (X)          : ORIGIN = 0x00000040 LENGTH = 0x00007FC0
    MCU0_R5F_TCMB0 (RWIX)       : ORIGIN = 0x41010000 LENGTH = 0x00008000

    /*==================== MCU0_R5F_1 SoC View =========================*/
    MCU0_R5F1_ATCM (RWIX)       : ORIGIN = 0x41400000 LENGTH = 0x00008000
    MCU0_R5F1_BTCM (RWIX)       : ORIGIN = 0x41410000 LENGTH = 0x00008000

    /*- Refer user guide for details on persistence of these sections -*/
    OCMC_RAM_BOARD_CFG   (RWIX) : ORIGIN = 0x41C80000 LENGTH = 0x00002000
    OCMC_RAM_SCISERVER   (RWIX) : ORIGIN = 0x41C82000 LENGTH = 0x00060000
    MCSPI_MASTER_OCMC_RAM (RWIX) : ORIGIN = MCU2_0_OCMC_ALLOCATED_START LENGTH = OCMC_ALLOCATED_SIZE
    MCSPI_SLAVE_OCMC_RAM (RWIX) : ORIGIN = MCU2_1_OCMC_ALLOCATED_START LENGTH = OCMC_ALLOCATED_SIZE
    OCMC_RAM_X509_HEADER (RWIX) : ORIGIN = 0x41CFDB00 LENGTH = 0x00000500

    /*========================J784S4 MCMS3 LOCATIONS ===================*/
    /*---------- J784S4 Reserved Memory for ARM Trusted Firmware -------*/
    MSMC3_ARM_FW  (RWIX)        : ORIGIN = 0x70000000 LENGTH = 0x00020000   /* 128KB       */
    MSMC3   (RWIX)              : ORIGIN=0x70020000 LENGTH=0x7D0000        /* 8MB - 192KB */
    /* j784s4 Reserved Memory for DMSC Firmware */
    MSMC3_DMSC_FW  (RWIX)       : ORIGIN=0x707F0000 LENGTH=0x10000         /* 64KB */

    /*======================= J784S4 DDR LOCATION =======================*/
    MCSPI_MASTER_DDR0 (RWIX)    : ORIGIN = MCU2_0_DDR0_ALLOCATED_START LENGTH = DDR0_ALLOCATED_SIZE   /* 128 MB */
    MCSPI_SLAVE_DDR0 (RWIX)     : ORIGIN = MCU2_1_DDR0_ALLOCATED_START LENGTH = DDR0_ALLOCATED_SIZE   /* 128 MB */
}
