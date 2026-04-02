/*----------------------------------------------------------------------------*/
/* File: linker.cmd                                                           */
/* Description:						                            		      */
/*    Linker command file for J784S4 Multicore Testcase	            	      */
/*                                                                            */
/*    Platform: R5 Cores on J784S4                                            */
/* (c) Texas Instruments 2022, All rights reserved.                           */
/*----------------------------------------------------------------------------*/

--entry_point=_sblTestResetVectors

MEMORY
{
    MSMC3_MCU1_CPU0	: origin=0x70010000 length=0x2000			/* 8KB */
    MSMC3_MCU1_CPU1	: origin=0x70012000 length=0x2000			/* 8KB */
    MSMC3_MCU2_CPU0	: origin=0x70014000 length=0x2000			/* 8KB */
    MSMC3_MCU2_CPU1	: origin=0x70016000 length=0x2000			/* 8KB */
    MSMC3_MCU3_CPU0	: origin=0x70018000 length=0x2000			/* 8KB */
    MSMC3_MCU3_CPU1	: origin=0x7001A000 length=0x2000			/* 8KB */
    MSMC3_MCU4_CPU0	: origin=0x7001C000 length=0x2000			/* 8KB */
    MSMC3_MCU4_CPU1	: origin=0x7001E000 length=0x2000			/* 8KB */
    MSMC3_DSP1_C7X	: origin=0x70020000 length=0x2000			/* 8KB */
    MSMC3_DSP2_C7X	: origin=0x70022000 length=0x2000			/* 8KB */
    MSMC3_DSP3_C7X	: origin=0x70024000 length=0x2000			/* 8KB */
    MSMC3_DSP4_C7X	: origin=0x70026000 length=0x2000			/* 8KB */
    MSMC3_MPU1_CPU0	: origin=0x70028000 length=0x2000			/* 8KB */
    MSMC3_MPU1_CPU1	: origin=0x7002A000 length=0x2000			/* 8KB */
    MSMC3_MPU1_CPU2	: origin=0x7002C000 length=0x2000			/* 8KB */
    MSMC3_MPU1_CPU3	: origin=0x7002E000 length=0x2000			/* 8KB */
    MSMC3_MPU2_CPU0	: origin=0x70030000 length=0x2000			/* 8KB */
    MSMC3_MPU2_CPU1	: origin=0x70032000 length=0x2000			/* 8KB */
    MSMC3_MPU2_CPU2	: origin=0x70034000 length=0x2000			/* 8KB */
    MSMC3_MPU2_CPU3	: origin=0x70036000 length=0x2000			/* 8KB */
}

SECTIONS
{
    .sbl_c7x_0_resetvector      : {} palign(8) 		> MSMC3_DSP1_C7X
    .sbl_c7x_1_resetvector      : {} palign(8) 		> MSMC3_DSP2_C7X
    .sbl_c7x_2_resetvector      : {} palign(8) 		> MSMC3_DSP3_C7X
    .sbl_c7x_3_resetvector      : {} palign(8) 		> MSMC3_DSP4_C7X
    .sbl_mpu_1_0_resetvector 	: {} palign(8) 		> MSMC3_MPU1_CPU0
    .sbl_mpu_1_1_resetvector 	: {} palign(8) 		> MSMC3_MPU1_CPU1
    .sbl_mpu_1_2_resetvector 	: {} palign(8) 		> MSMC3_MPU1_CPU2
    .sbl_mpu_1_3_resetvector 	: {} palign(8) 		> MSMC3_MPU1_CPU3
    .sbl_mpu_2_0_resetvector 	: {} palign(8) 		> MSMC3_MPU2_CPU0
    .sbl_mpu_2_1_resetvector 	: {} palign(8) 		> MSMC3_MPU2_CPU1
    .sbl_mpu_2_2_resetvector 	: {} palign(8) 		> MSMC3_MPU2_CPU2
    .sbl_mpu_2_3_resetvector 	: {} palign(8) 		> MSMC3_MPU2_CPU3

}

