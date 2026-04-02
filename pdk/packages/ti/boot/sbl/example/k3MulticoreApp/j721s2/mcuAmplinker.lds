/*----------------------------------------------------------------------------*/
/* File: linker.cmd                                                           */
/* Description:						                            		      */
/*    Linker command file for J721S2 Multicore Testcase	            	      */
/*                                                                            */
/*    Platform: R5 Cores on J721S2                                            */
/* (c) Texas Instruments 2021, All rights reserved.                           */
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
    MSMC3_DSP1_C7X	: origin=0x70020000 length=0x2000			/* 8KB */
    MSMC3_DSP2_C7X	: origin=0x70022000 length=0x2000			/* 8KB */
    MSMC3_MPU1_CPU0	: origin=0x70024000 length=0x2000			/* 8KB */
    MSMC3_MPU1_CPU1	: origin=0x70026000 length=0x2000			/* 8KB */
}

SECTIONS
{
    .sbl_c7x_0_resetvector      : {} palign(8) 		> MSMC3_DSP1_C7X
    .sbl_c7x_1_resetvector      : {} palign(8) 		> MSMC3_DSP2_C7X
    .sbl_mpu_1_0_resetvector 	: {} palign(8) 		> MSMC3_MPU1_CPU0
    .sbl_mpu_1_1_resetvector 	: {} palign(8) 		> MSMC3_MPU1_CPU1

}

