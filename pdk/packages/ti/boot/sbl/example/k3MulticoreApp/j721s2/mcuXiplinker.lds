/*----------------------------------------------------------------------------*/
/* File: mcuXiplinker.cmd                                                     */
/* Description:								                                  */
/*    Link command file to execute from OSPI flash memory		              */
/*                                                                            */
/*    Platform: R5 Cores on J7                                                */
/* (c) Texas Instruments 2019-2023, All rights reserved.                      */
/*----------------------------------------------------------------------------*/

--entry_point=_sblTestResetVectors
--stack_size=0x1000

MEMORY
{
    OSPI_MCU1_CPU0_XIP_VEC (RIX)    : origin=0x501c0000 length=0x40			/* 64B */
    OSPI_MCU1_CPU0_XIP_1 (RIX)      : origin=0x501c0040 length=0x200
    OSPI_MCU1_CPU0_XIP_2 (RIX)      : origin=0x501c0240 length=0x200
    OCMRAM_SBL_SYSFW (RWX)	        : origin=0x41C83000 length=0x2000 - 0x800	/* 8KB */
}

SECTIONS
{
    .rstvectors			        : {} palign(8) 		> OSPI_MCU1_CPU0_XIP_VEC
    .text	                    : {} palign(8) 		> OSPI_MCU1_CPU0_XIP_1
    .rodata	                    : {} palign(8) 		> OSPI_MCU1_CPU0_XIP_1
    .sbl_mcu_1_0_resetvector	: {} palign(8) 		> OSPI_MCU1_CPU0_XIP_2
    .stack 			            : {} palign(8) 		> OCMRAM_SBL_SYSFW
}

