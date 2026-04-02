/*----------------------------------------------------------------------------*/
/* File: linker.cmd                                                           */
/* Description:                                                               */
/*    Link command file for J7200 Multicore Testcase                          */
/*                                                                            */
/*    Platform: R5 Cores on J7200                                             */
/* (c) Texas Instruments 2020, All rights reserved.                           */
/*----------------------------------------------------------------------------*/

--entry_point=_sblTestResetVectors

MEMORY
{
    MSMC3_MCU1_CPU0	: origin=0x70010000 length=0x2000			/* 8KB */
    MSMC3_MCU1_CPU1	: origin=0x70012000 length=0x2000			/* 8KB */
    MSMC3_MCU2_CPU0	: origin=0x70014000 length=0x2000			/* 8KB */
    MSMC3_MCU2_CPU1	: origin=0x70016000 length=0x2000			/* 8KB */
    MSMC3_MPU1_CPU0	: origin=0x70024000 length=0x2000			/* 8KB */
    MSMC3_MPU1_CPU1	: origin=0x70026000 length=0x2000			/* 8KB */
}

SECTIONS
{
    .sbl_mpu_1_0_resetvector 	: {} palign(8) 		> MSMC3_MPU1_CPU0
    .sbl_mpu_1_1_resetvector 	: {} palign(8) 		> MSMC3_MPU1_CPU1
}

