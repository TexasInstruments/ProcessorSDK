/*----------------------------------------------------------------------------*/
/* File: linker.lds                                                           */
/* Description:                                                               */
/*    Link command file for J7200 Multicore Testcase                          */
/*                                                                            */
/*    Platform: A72 cores on J7200                                            */
/* (c) Texas Instruments 2020, All rights reserved.                           */
/*----------------------------------------------------------------------------*/

ENTRY(_sblTestResetVectors);

MEMORY
{
    MSMC3_MCU1_CPU0	: ORIGIN = 0x000070010000, LENGTH = 0x00002000			/* 8KB */
    MSMC3_MCU1_CPU1	: ORIGIN = 0x000070012000, LENGTH = 0x00002000			/* 8KB */
    MSMC3_MCU2_CPU0	: ORIGIN = 0x000070014000, LENGTH = 0x00002000			/* 8KB */
    MSMC3_MCU2_CPU1	: ORIGIN = 0x000070016000, LENGTH = 0x00002000			/* 8KB */
    MSMC3_MPU1_CPU0 : ORIGIN = 0x000070024000, LENGTH = 0x00002000			/* 8KB */
    MSMC3_MPU1_CPU1 : ORIGIN = 0x000070026000, LENGTH = 0x00002000			/* 8KB */
}

SECTIONS
{
    .sbl_mcu_1_0_resetvector : {} > MSMC3_MCU1_CPU0
    .sbl_mcu_1_1_resetvector : {} > MSMC3_MCU1_CPU1
    .sbl_mcu_2_0_resetvector : {} > MSMC3_MCU2_CPU0
    .sbl_mcu_2_1_resetvector : {} > MSMC3_MCU2_CPU1
    .sbl_mpu_1_0_resetvector : {} > MSMC3_MPU1_CPU0
    .sbl_mpu_1_1_resetvector : {} > MSMC3_MPU1_CPU1
}
