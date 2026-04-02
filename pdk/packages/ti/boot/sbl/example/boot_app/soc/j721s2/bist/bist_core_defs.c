/*
 *  Copyright (c) Texas Instruments Incorporated 2025
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  \file     bist_core_defs.c
 *
 *  \brief    This file defines available main domain bist modules and order of bist per stage
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include "bist_core_defs.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* NOTE: The PBIST and LBIST tests that are run in each stage on this SOC are
 * defined in this file.  To make modifications, the user simply can add/remove/move
 * ID's in the following arrays.
 * Run before any booting of cores:
 * pbist_pre_boot_stage
 * lbist_pre_boot_stage
 * Run before the first boot stage:
 * pbist_first_boot_stage
 * lbist_first_boot_stage
 * Run before the second boot stage:
 * pbist_second_boot_stage
 * lbist_second_boot_stage
 * Run before the third boot stage:
 * pbist_third_boot_stage
 * lbist_third_boot_stage
 * Care should be taken to ensure that BIST sections for particular cores match
 * the boot cores selected for each boot stage in soc/<SOC Device>/boot_core_defs.c.
 * */

/* Defines PBIST's needed for all cores before the first stage of the Main Domain
 * boot sequence for J721S2 SOC */
int pbist_pre_boot_stage[] =
{
    PBIST_HWPOST_MCU_INDEX,    /* Read results of HW POST MCU PBIST */          
	
};

int pbist_pre_boot_stage_status[] =
{
    0,  /* HW Post status */
    0,  /* Main Infra_0 */   

};

int pbist_pre_boot_stage_neg_status[] =
{
    0,  /* HW Post status */
    0,  /* Main Infra_0 */
};

int pbist_pre_boot_stage_rom_test_status[] =
{
    0,  /* HW Post status */
};

/* Defines LBIST's needed for all cores before the first stage of the Main Domain
 * boot sequence for J721S2 SOC */
int lbist_pre_boot_stage[] =
{
   LBIST_HWPOST_INST_SMS_INDEX,    /* Read results of HW POST SMS LBIST */
   LBIST_HWPOST_INST_MCU_INDEX     /* Read results of HW POST MCU LBIST */
};

int lbist_pre_boot_stage_status[] =
{
    0,   /* HW POST SMS LBIST */
    0,   /* HW POST MCU LBIST */
};

/* Defines PBIST's for the first stage of the Main Domain
 * boot sequence for J721S2 SOC */
int pbist_first_boot_stage[] =
{
   PBIST_INSTANCE_VPAC_0,     
   PBIST_INSTANCE_MAININFRA_1,
   PBIST_INSTANCE_DMPAC,      
   PBIST_INSTANCE_MAINR5F0,   
   PBIST_INSTANCE_MAINR5F1,   
   PBIST_INSTANCE_MAININFRA_0,
   PBIST_INSTANCE_HC,         
   PBIST_INSTANCE_NAVSS,      
   PBIST_INSTANCE_CODEC_1,    
   PBIST_INSTANCE_GPU,        
   PBIST_INSTANCE_C7x_0,      
   PBIST_INSTANCE_C7x_1,      
   PBIST_INSTANCE_A72_0,      
   PBIST_INSTANCE_MSMC,       
   PBIST_INSTANCE_MCUR5F1,    
   PBIST_INSTANCE_MCU_0,      
   PBIST_INSTANCE_MCU_1,   
   PBIST_INSTANCE_DSS
};


/* Defines LBIST's for the first stage of the Main Domain
 * boot sequence for J721S2 SOC */
int lbist_first_boot_stage[] =
{
    LBIST_INST_MAINR5F0_INDEX               
};

/* Defines LBIST's for the second stage of the Main Domain
 * boot sequence for J721S2 SOC */
int lbist_second_boot_stage[] =
{
    LBIST_INST_MAINR5F1_INDEX,   /* Main R5F 1 */
    LBIST_INST_C7X0_INDEX,       /* C7X_0 */
    LBIST_INST_C7X1_INDEX,       /* C7X_1 */
    LBIST_INST_VPAC0_INDEX,      /* VPAC_0 */
    LBIST_INST_DMPAC_INDEX       /* DMPAC */
};

/* Defines LBIST's for the third stage of the Main Domain
 * boot sequence for J721S2 SOC */
int lbist_third_boot_stage[] =
{
    LBIST_INST_A72_0_INDEX

};

/* Points to pbist arrays for each of the boot stages */
int *pbist_array_stage[NUM_BOOT_STAGES] =
{
    pbist_first_boot_stage,
};

/* Points to lbist arrays for each of the boot stages */
int *lbist_array_stage[NUM_BOOT_STAGES] =
{
    lbist_first_boot_stage,
    lbist_second_boot_stage,
    lbist_third_boot_stage
};

/* Defines number of LBIST's run in each stage */
uint8_t num_lbists_per_boot_stage[NUM_BOOT_STAGES] =
{
    sizeof(lbist_first_boot_stage)  / sizeof(int),
    sizeof(lbist_second_boot_stage) / sizeof(int),
    sizeof(lbist_third_boot_stage)  / sizeof(int)
};

/* Defines number of PBIST's run in each stage */
uint8_t num_pbists_per_boot_stage[NUM_BOOT_STAGES] =
{
    sizeof(pbist_first_boot_stage)  / sizeof(int),
};

uint8_t num_lbists_pre_boot = sizeof(lbist_pre_boot_stage) / sizeof(int);
uint8_t num_pbists_pre_boot = sizeof(pbist_pre_boot_stage) / sizeof(int);

const char *BootApp_pbistName(uint32_t pbistID)
{
    char *name;

    switch(pbistID)
    {
       case (PBIST_HWPOST_MCU_INDEX):
           name="PBIST_HWPOST_MCU_INDEX";
           break;
       case (PBIST_INSTANCE_VPAC_0):
           name="PBIST_INSTANCE_VPAC_0";
           break;
       case (PBIST_INSTANCE_MAININFRA_1):
           name="PBIST_INSTANCE_MAININFRA_1";
           break;
       case (PBIST_INSTANCE_DMPAC):
           name="PBIST_INSTANCE_DMPAC";
           break;
       case (PBIST_INSTANCE_MAINR5F0):
           name="PBIST_INSTANCE_MAINR5F0";
           break;
       case (PBIST_INSTANCE_MAINR5F1):
           name="PBIST_INSTANCE_MAINR5F1";
           break;
       case (PBIST_INSTANCE_MAININFRA_0):
           name="PBIST_INSTANCE_MAININFRA_0";
           break;
       case (PBIST_INSTANCE_HC):
           name="PBIST_INSTANCE_HC";
           break;
       case (PBIST_INSTANCE_NAVSS):
           name="PBIST_INSTANCE_NAVSS";
           break;
       case (PBIST_INSTANCE_CODEC_1):
           name="PBIST_INSTANCE_CODEC_1";
           break;		
       case (PBIST_INSTANCE_GPU):	
           name="PBIST_INSTANCE_GPU";
           break;			
       case (PBIST_INSTANCE_C7x_0):
           name="PBIST_INSTANCE_C7x_0";
           break;	
       case (PBIST_INSTANCE_C7x_1):
           name="PBIST_INSTANCE_C7x_1";
           break;	
       case (PBIST_INSTANCE_A72_0):
           name="PBIST_INSTANCE_A72_0";
           break;	
       case (PBIST_INSTANCE_MSMC):
           name="PBIST_INSTANCE_MSMC";
           break;
       case (PBIST_INSTANCE_MCUR5F1):
           name="PBIST_INSTANCE_MCUR5F1";
           break;	
       case (PBIST_INSTANCE_MCU_0):
           name="PBIST_INSTANCE_MCU_0";
           break;	
       case (PBIST_INSTANCE_MCU_1):
           name="PBIST_INSTANCE_MCU_1";
           break;			
       case (PBIST_INSTANCE_DSS):
           name="PBIST_INSTANCE_DSS";
           break;				
       default:
           name="INVALID ID";
           break;
    }

    return name;
}

const char *BootApp_lbistName(uint32_t lbistID)
{
    char *name;

    switch(lbistID)
    {
        case (LBIST_HWPOST_INST_SMS_INDEX):
            name="LBIST_HWPOST_INST_SMS_INDEX";
            break;
        case (LBIST_HWPOST_INST_MCU_INDEX):
            name="LBIST_HWPOST_INST_MCU_INDEX";
            break;
        case (LBIST_INST_MAINR5F0_INDEX):
            name="LBIST_INST_MAINR5F0_INDEX";
            break;
        case (LBIST_INST_MAINR5F1_INDEX):
            name="LBIST_INST_MAINR5F1_INDEX";
            break;
        case (LBIST_INST_C7X0_INDEX):
            name="LBIST_INST_C7X0_INDEX";
            break;
        case (LBIST_INST_C7X1_INDEX):
            name="LBIST_INST_C7X1_INDEX";
            break;
        case (LBIST_INST_VPAC0_INDEX):
            name="LBIST_INST_VPAC0_INDEX";
            break;
        case (LBIST_INST_DMPAC_INDEX):
            name="LBIST_INST_DMPAC_INDEX";
            break;
        case (LBIST_INST_A72_0_INDEX):
            name="LBIST_INST_A72_0_INDEX";
            break;
        default:
            name="INVALID ID";
            break;
    }

    return name;
}