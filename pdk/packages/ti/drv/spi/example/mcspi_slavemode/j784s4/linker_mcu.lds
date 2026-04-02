/* Linker Settings */
--retain="*(.bootCode)"
--retain="*(.startupCode)"
--retain="*(.startupData)"
--retain="*(.intvecs)"
--retain="*(.intc_text)"
--retain="*(.rstvectors)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"
--fill_value=0
--stack_size=0x2000
--heap_size=0x1000
--entry_point=_resetvectors     /* Default C RTS boot.asm   */

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x2000                              /* HEAP AREA SIZE                */

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 0x1000;
__FIQ_STACK_SIZE = 0x1000;
__ABORT_STACK_SIZE = 0x1000;
__UND_STACK_SIZE = 0x1000;
__SVC_STACK_SIZE = 0x1000;

#define DDR0_ALLOCATED_START 0x80000000
#define DDR0_ALLOCATED_SIZE  0x10000000

#define OCMC_ALLOCATED_START 0x41CE3100
#define OCMC_ALLOCATED_SIZE  0x00001000

#define MCU2_0_DDR0_ALLOCATED_START   DDR0_ALLOCATED_START
#define MCU2_0_OCMC_ALLOCATED_START   OCMC_ALLOCATED_START


#define MCU2_1_DDR0_ALLOCATED_START   MCU2_0_DDR0_ALLOCATED_START + DDR0_ALLOCATED_SIZE
#define MCU2_1_OCMC_ALLOCATED_START   MCU2_0_OCMC_ALLOCATED_START + OCMC_ALLOCATED_SIZE

/* Memory Map */
MEMORY
{
    /* MCU0_R5F_1 local view */
    MCU0_R5F1_TCMA (X)       : origin=0x0        length=0x8000
    MCU0_R5F1_TCMB0 (RWIX)   : origin=0x41010000 length=0x8000

    /* MCU0_R5F_1 SoC view */
    MCU0_R5F1_ATCM (RWIX)   : origin=0x41400000 length=0x8000
    MCU0_R5F1_BTCM (RWIX)   : origin=0x41410000 length=0x8000

    /* Refer the user guide for details on persistence of these sections */
    OCMC_RAM_BOARD_CFG (RWIX)   : origin=0x41C80000 length=0x2000
    OCMC_RAM_SCISERVER (RWIX)   : origin=0x41C82000 length=0x60000
    VECTORS (X)                 : origin=0x41CE2000 length=0x1000
    RESET_VECTORS (X)           : origin=0x41CE3000 length=0x100
    MCSPI_MASTER_OCMC_RAM (RWIX) : ORIGIN = MCU2_0_OCMC_ALLOCATED_START LENGTH = OCMC_ALLOCATED_SIZE
    MCSPI_SLAVE_OCMC_RAM (RWIX) : ORIGIN = MCU2_1_OCMC_ALLOCATED_START LENGTH = OCMC_ALLOCATED_SIZE
    OCMC_RAM_X509_HEADER (RWIX) : origin=0x41CFDB00 length=0x500

    /* j784s4 MCMS3 locations */
    /* j784s4 Reserved Memory for ARM Trusted Firmware */
    MSMC3_ARM_FW   (RWIX)   : origin=0x70000000 length=0x20000         /* 128KB */
    MSMC3   (RWIX)          : origin=0x70020000 length=0x7D0000        /* 8MB - 192KB */
    /* j784s4 Reserved Memory for DMSC Firmware */
    MSMC3_DMSC_FW  (RWIX)   : origin=0x707F0000 length=0x10000         /* 64KB */

    MCSPI_MASTER_DDR0 (RWIX)    : ORIGIN = MCU2_0_DDR0_ALLOCATED_START LENGTH = DDR0_ALLOCATED_SIZE   /* 128 MB */
    MCSPI_SLAVE_DDR0 (RWIX)     : ORIGIN = MCU2_1_DDR0_ALLOCATED_START LENGTH = DDR0_ALLOCATED_SIZE   /* 128 MB */
}
