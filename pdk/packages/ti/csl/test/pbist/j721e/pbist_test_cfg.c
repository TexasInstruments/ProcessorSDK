/*
 *   Copyright (c) Texas Instruments Incorporated 2020
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
 *
 */

 /**
 *  \file     pbist_test_cfg.c
 *
 *  \brief    This file contains PBIST test configuration
 *
 *  \details  PBIST Test Configuration
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/csl/soc.h>
#include <ti/csl/csl_pbist.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/csl_cbass.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/csl/csl_rat.h>
#include <ti/csl/cslr_vpac.h>
#include <ti/csl/cslr_cp_ace.h>

#include "power_seq.h"
#include "pbist_test_cfg.h"

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/* This macro defined by default to indicate only a select memory section can
 * be run on the MCU memory PBIST instances to ensure test application running
 * on MCU is not corrupted. */
#define PBIST_MCU_SELECTED_MEMORY

/* Macro to determine if test is excluded because of failure.
 * Set to 0 to disable specific sections. */
#define PBIST_DISABLE_BECAUSE_OF_FAILURE 0

/* ========================================================================== */
/*                            Local function prototypes                       */
/* ========================================================================== */
/*
    InitRestore functions : Initialize or Restore based on init flag
    init : TRUE  --> Initialize
    init : FALSE --> Restore
*/
int32_t PBIST_A72AuxInitRestore(bool init);
int32_t PBIST_VPACAuxInitRestore(bool init);
int32_t PBIST_MainInfraAuxInitRestore(bool init);
int32_t PBIST_decoderAuxInitRestore(bool init);
int32_t PBIST_GPUAuxInitRestore(bool init);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
uint32_t PBIST_A72AuxDevList[A72_NUM_AUX_DEVICES] =
{
    TISCI_DEV_A72SS0,
};

uint32_t PBIST_MainInfraAuxDevList[MAIN_INFRA_NUM_AUX_DEVICES] =
{
    TISCI_DEV_DEBUGSS_WRAP0,
    TISCI_DEV_MCAN0,
    TISCI_DEV_MCAN1,
    TISCI_DEV_MCAN2,
    TISCI_DEV_MCAN3,
    TISCI_DEV_MCAN4,
    TISCI_DEV_MCAN5,
    TISCI_DEV_MCAN6,
    TISCI_DEV_MCAN7,
    TISCI_DEV_MCAN8,
    TISCI_DEV_MCAN9,
    TISCI_DEV_MCAN10,
    TISCI_DEV_MCAN11,
    TISCI_DEV_MCAN12,
    TISCI_DEV_MCAN13,
    TISCI_DEV_I3C0,
    TISCI_DEV_SA2_UL0,
    TISCI_DEV_CPSW0
};

uint32_t PBIST_MSMCAuxDevList[MSMC_NUM_AUX_DEVICES] =
{
    TISCI_DEV_COMPUTE_CLUSTER0_EN_MSMC_DOMAIN,
    TISCI_DEV_NAVSS0,
    TISCI_DEV_A72SS0,
};

uint32_t PBIST_GPUAuxDevList[GPU_NUM_AUX_DEVICES] =
{
    TISCI_DEV_GPU0,
    TISCI_DEV_GPU0_GPU_0,
};

uint32_t PBIST_DSSAuxDevList[DSS_NUM_AUX_DEVICES] =
{
    TISCI_DEV_DSS0,
    TISCI_DEV_DSS_EDP0,
    TISCI_DEV_CSI_RX_IF0,
    TISCI_DEV_CSI_RX_IF1,
    TISCI_DEV_CSI_TX_IF0,
    TISCI_DEV_CSI_PSILSS0,
    TISCI_DEV_DPHY_RX0,
    TISCI_DEV_DPHY_RX1,
    TISCI_DEV_DPHY_TX0,
    TISCI_DEV_SERDES_10G0,
   /* TODO: Need to check on SERDES8MTO*/
};

PBIST_TestHandle_t PBIST_TestHandleArray[PBIST_MAX_INSTANCE+1] =
{
    /* MCU PBIST - Only select memory guaranteed not to be utilized by this test application
     * can be run, since PBIST changes the memory upon which it is run. */
    {
        .testName            = "MCU PBIST",
        .numPostPbistToCheck = 1u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_MCU_PBIST0_BASE,
#ifdef PBIST_MCU_SELECTED_MEMORY
        .numPBISTRuns        = 1u,
        .PBISTConfigRun = {
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                /* NOTE: As the MCU R5f is running this code override bit cannot be set to enable all memory test */
                .algorithmsBitMap   = 0x00002000u,          /* Choose Algorithm 14 */
                .memoryGroupsBitMap = 0x0080000000000000u,  /* Choose ADC RAM */
                .scrambleValue      = 0xFEDCBA9876543210U,  /* Scramble Value */
            }
        },
#else // Golden Vectors not currently supported running from MCU core as this will be self destructive
        .numPBISTRuns        = CSL_MCU_PBIST0_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            /* Golden Vector Part 1 */
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                /* NOTE: As the MCU R5f is running this code override bit cannot be set to enable all memory test */
                .algorithmsBitMap   = CSL_MCU_PBIST0_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_MCU_PBIST0_MEM_BITMAP_0     /* Choose recommended mem bitmap 0  */
                .scrambleValue      = 0xFEDCBA9876543210U,  /* Scramble Value */
            },
            /* Golden Vector Part 2 */
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                /* NOTE: As the MCU R5f is running this code override bit cannot be set to enable all memory test */
                .algorithmsBitMap   = CSL_MCU_PBIST0_ALGO_BITMAP_1,   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_MCU_PBIST0_MEM_BITMAP_1,    /* Choose recommended mem bitmap 1  */
                .scrambleValue      = 0xFEDCBA9876543210U,  /* Scramble Value */
            }
        },
#endif
        .tisciPBISTDeviceId     = TISCI_DEV_MCU_PBIST0, /* PBIST device id  */
        .pollMode               = false,
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_MCU_PBIST0_DFT_PBIST_CPU_0,
        .procRstNeeded          = false,
        .secondaryCoreNeeded    = false,                /* Secondary core needed */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,                   /* No Aux devices */
        .auxInitRestoreFunction        = NULL,                 /* Auxilliary init function */ 
        .doneFlag               = false,                /* InitRestoreialize done flag */
    },
    /* Main R5F 0 */
    {
        .testName            = "Main R5F 0 PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_PBIST9_BASE, /* PBIST9: Main R5F 0 */
        .numPBISTRuns        = CSL_PBIST9_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                /* NOTE: override set to 1 is not functional currently */
                .algorithmsBitMap   = CSL_PBIST9_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_PBIST9_MEM_BITMAP_0,    /* Choose recommended mem bitmap 0  */
                .scrambleValue      = 0xFEDCBA9876543210U,        /* Scramble Value */
            }
        },
        .tisciPBISTDeviceId     = TISCI_DEV_PBIST9,    /* PBIST device id  */
        .pollMode               = false,
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_PBIST9_DFT_PBIST_CPU_0,
        .procRstNeeded          = true,
        .secondaryCoreNeeded    = true,                /* Secondary core needed */
        .coreName               = "Main R5F0 core0",   /* Primary core   */
        .secCoreName            = "Main R5F0 core1",   /* Secondary core */
        .tisciProcId            = SCICLIENT_PROC_ID_R5FSS0_CORE0, /* Main R5F core 0 Proc Id */
        .tisciSecProcId         = SCICLIENT_PROC_ID_R5FSS0_CORE1, /* Main R5F core 1 Proc Id */
        .tisciDeviceId          = TISCI_DEV_R5FSS0_CORE0,   /* Main R5F core 0 Device Id */
        .tisciSecDeviceId       = TISCI_DEV_R5FSS0_CORE1,   /* Main R5F core 1 Device Id */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,                   /* No Aux devices */
        .auxInitRestoreFunction = NULL,                 /* Auxilliary init function */ 
        .doneFlag               = false,                /* Initialize done flag  */
    },
    /* Main R5F 1 */
    {
        .testName            = "Main R5F 1 PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_PBIST10_BASE, /* PBIST10: Main MCU 1 */
        .numPBISTRuns        = CSL_PBIST10_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                /* NOTE: override set to 1 is not functional currently */
                .algorithmsBitMap   = CSL_PBIST10_ALGO_BITMAP_0,  /* Choose recommended Algo bitmap */
                .memoryGroupsBitMap = CSL_PBIST10_MEM_BITMAP_0,   /* Choose recommended mem bitmap  */
                .scrambleValue      = 0xFEDCBA9876543210U,        /* Scramble Value */
            }
        },
        .tisciPBISTDeviceId     = TISCI_DEV_PBIST10,   /* PBIST device id  */
        .pollMode               = false,
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_PBIST10_DFT_PBIST_CPU_0,
        .procRstNeeded          = true,
        .secondaryCoreNeeded    = true,                /* Secondary core needed */
        .coreName               = "Main R5F1 core0",   /* Primary core   */
        .secCoreName            = "Main R5F1 core1",   /* Secondary core */
        .tisciProcId            = SCICLIENT_PROC_ID_R5FSS1_CORE0, /* Main R5F core 0 Proc Id */
        .tisciSecProcId         = SCICLIENT_PROC_ID_R5FSS1_CORE1, /* Main R5F core 1 Proc Id */
        .tisciDeviceId          = TISCI_DEV_R5FSS1_CORE0,  /* Main R5F core 0 Device id */
        .tisciSecDeviceId       = TISCI_DEV_R5FSS1_CORE1,  /* Main R5F core 1 Device id */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,                      /* No Aux devices */
        .auxInitRestoreFunction = NULL,                /* Auxilliary init function */ 
        .doneFlag               = false,               /* Initialize done flag */
    },
    /* C7X */
    {
        .testName            = "C7X PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = CSL_COMPUTE_CLUSTER0_C71SS0_PBIST_BASE,
        .pPBISTRegs          = (CSL_pbistRegs *)PBIST_REGION_LOCAL_BASE,
        .numPBISTRuns        = CSL_C71SS0_PBIST_WRAP_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_C71SS0_PBIST_WRAP_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap */
                .memoryGroupsBitMap = CSL_C71SS0_PBIST_WRAP_MEM_BITMAP_0,    /* Choose recommended mem bitmap  */
                .scrambleValue      = 0xFEDCBA9876543210U,                  /* Scramble Value */
            }
        },
        .tisciPBISTDeviceId     = TISCI_DEV_C71X_0_PBIST_VD,   /* PBIST device id  */
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_12,
        .procRstNeeded          = true,
        .secondaryCoreNeeded    = false,               /* Secondary core needed */
        .coreName               = "C7x ",
        .tisciProcId            = SCICLIENT_PROC_ID_C71SS0,  /* C7x Proc Id */
        .tisciDeviceId          = TISCI_DEV_C71SS0,    /* C7x Device Id */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,                  /* No Aux devices */
        .auxInitRestoreFunction = NULL,                /* Auxilliary init function */ 
        .doneFlag               = false,               /* Initialize done flag */
    },
    /* A72 */
    {
        .testName            = "A72 PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = CSL_COMPUTE_CLUSTER0_A72SS0_PBIST0_BASE,
        .pPBISTRegs          = (CSL_pbistRegs *)PBIST_REGION_LOCAL_BASE,
        .numPBISTRuns        = CSL_A72SS0_CORE0_PBIST_WRAP_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_A72SS0_CORE0_PBIST_WRAP_ALGO_BITMAP_0,  /* Choose recommended Algo bitmap */
                .memoryGroupsBitMap = CSL_A72SS0_CORE0_PBIST_WRAP_MEM_BITMAP_0,   /* Choose recommended mem bitmap  */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            }
        },
        .tisciPBISTDeviceId     = TISCI_DEV_COMPUTE_CLUSTER0_PBIST_WRAP, /* Device Id for A72 PBIST */
        .pollMode               = false,
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_8,
        .procRstNeeded          = true,
        .secondaryCoreNeeded    = true,                /* Secondary core needed */
        .coreName               = "A72 core 0",        /* Primary core   */
        .secCoreName            = "A72 core 1",        /* Secondary core */
        .tisciProcId            = SCICLIENT_PROC_ID_A72SS0_CORE0,  /* A72 core 0 Proc Id */
        .tisciSecProcId         = SCICLIENT_PROC_ID_A72SS0_CORE1,  /* A72 core 1 Proc Id */
        .tisciDeviceId          = TISCI_DEV_A72SS0_CORE0,  /* A72 core 0 Device Id */
        .tisciSecDeviceId       = TISCI_DEV_A72SS0_CORE1,  /* A72 core 1 Device Id */
        .coreCustPwrSeqNeeded   = true,                    /* A72 needs custom powerdown sequence steps */
        .numAuxDevices          = A72_NUM_AUX_DEVICES,     /* Number of Aux devices   */
        .auxDeviceIdsP          = &PBIST_A72AuxDevList[0], /* Array of Aux device ids */
        .auxInitRestoreFunction = PBIST_A72AuxInitRestore, /* Auxilliary init function */ 
        .doneFlag               = false,                   /* Initialize done flag */
    },
    /* VPAC */
    {
        .testName            = "VPAC PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_PBIST3_BASE,
        .numPBISTRuns        = CSL_PBIST3_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            /* Golden Vector Part 1 */
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST3_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_PBIST3_MEM_BITMAP_0,    /* Choose recommended mem bitmap 0  */
                .scrambleValue      = 0xFEDCBA9876543210U,    /* Scramble Value */
            },
            /* Golden Vector Part 2 */
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST3_ALGO_BITMAP_1,   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_PBIST3_MEM_BITMAP_1,    /* Choose recommended mem bitmap 1  */
                .scrambleValue      = 0xFEDCBA9876543210U,       /* Scramble Value */
            }
        },
        .tisciPBISTDeviceId     = TISCI_DEV_PBIST3,   /* PBIST device id  */
        .pollMode               = false,
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_PBIST3_DFT_PBIST_CPU_0,
        .procRstNeeded          = true,
        .secondaryCoreNeeded    = false,            /* Secondary core needed */
        .coreName               = "VPAC core",      /* Primary core   */
        .tisciProcId            = 0x0u,             /* No ProcId for VPAC */
        .tisciDeviceId          = TISCI_DEV_VPAC0,  /* VPAC core Device Id */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,               /* No Aux devices */
        .auxInitRestoreFunction = PBIST_VPACAuxInitRestore,/* Auxilliary init function */ 
        .doneFlag               = false,            /* Initialize done flag */
    },
    /* DMPAC */
    {
        .testName            = "DMPAC PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_PBIST1_BASE,
        .numPBISTRuns        = CSL_PBIST1_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            /* Golden Vector Part 1 */
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   =   CSL_PBIST1_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_PBIST1_MEM_BITMAP_0,      /* Choose recommended mem bitmap 0 */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            },
            /* Golden Vector Part 2 */
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   =   CSL_PBIST1_ALGO_BITMAP_1,   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_PBIST1_MEM_BITMAP_1,      /* Choose recommended mem bitmap 1  */
                .scrambleValue      = 0xFEDCBA9876543210U,   /* Scramble Value */
            },
        },
        .tisciPBISTDeviceId     = TISCI_DEV_PBIST1,   /* PBIST device id  */
        .pollMode               = false,
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_PBIST1_DFT_PBIST_CPU_0,
        .procRstNeeded          = true,
        .secondaryCoreNeeded    = true,                       /* Secondary core needed */
        .coreName               = "DMPAC core",               /* Primary core   */
        .secCoreName            = "DMPAC SDE core",           /* Secondary core */
        .tisciProcId            = 0x0u,                       /* No ProcId for DMPAC */
        .tisciSecProcId         = 0x0u,                       /* No Sec ProcId for DMPAC */
        .tisciDeviceId          = TISCI_DEV_DMPAC0,           /* DMPAC Core Device Id */
        .tisciSecDeviceId       = TISCI_DEV_DMPAC0_SDE_0,     /* DMPAC_SDE Device Id */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,                  /* No Aux devices */
        .auxInitRestoreFunction = NULL,                /* Auxilliary init function */ 
        .doneFlag               = false,               /* Initialize done flag */
    },
    /* NAVSS */
    {
        .testName            = "NAVSS PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_PBIST7_BASE,
        .numPBISTRuns        = CSL_PBIST7_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            /* Golden Vector Part 1 */
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST7_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_PBIST7_MEM_BITMAP_0,    /* Choose recommended mem bitmap 0  */
                .scrambleValue      = 0xFEDCBA9876543210U,  /* Scramble Value */
            },
            /* Golden Vector Part 2 */
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST7_ALGO_BITMAP_1,   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_PBIST7_MEM_BITMAP_1,    /* Choose recommended mem bitmap 1  */
                .scrambleValue      = 0xFEDCBA9876543210U,  /* Scramble Value */
            }
        },
        .tisciPBISTDeviceId     = TISCI_DEV_PBIST7,   /* PBIST device id  */
        .pollMode               = false,
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_PBIST7_DFT_PBIST_CPU_0,
        .procRstNeeded          = true,
        .secondaryCoreNeeded    = false,                /* Secondary core needed */
        .coreName               = "NAVSS",              /* Primary core   */
        .tisciProcId            = 0x0u,                 /* No ProcId for NAVSS */
        .tisciDeviceId          = TISCI_DEV_NAVSS0,     /* NAVSS Device Id */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,                   /* No Aux devices */
        .auxInitRestoreFunction = NULL,                /* Auxilliary init function */ 
        .doneFlag               = false,                /* Initialize done flag */
    },
    /* HC */
    {
        .testName            = "HC PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_PBIST5_BASE,
        .numPBISTRuns        = 1u, // TODO - ADD SECOND GOLDEN VECTOR NOT YET FUNCTIONAL
        .PBISTConfigRun = {
            /* Golden Vector Part 1 */
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST5_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_PBIST5_MEM_BITMAP_0,    /* Choose recommended mem bitmap 0  */
                .scrambleValue      = 0xFEDCBA9876543210U,  /* Scramble Value */
            },
#if PBIST_DISABLE_BECAUSE_OF_FAILURE
            /* Golden Vector Part 2 */
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST5_ALGO_BITMAP_1   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_PBIST5_MEM_BITMAP_1,    /* Choose recommended mem bitmap 1 */
                .scrambleValue      = 0xFEDCBA9876543210U,  /* Scramble Value */
            }
#endif
        },
        .tisciPBISTDeviceId     = TISCI_DEV_PBIST5, /* PBIST device id  */
        .pollMode               = false,
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_PBIST5_DFT_PBIST_CPU_0,
        .procRstNeeded          = false,
        .secondaryCoreNeeded    = false,            /* Secondary core needed */
        .coreName               = {0x0u},           /* No coreName   */
        .tisciProcId            = 0x0u,             /* No ProcId for HC */
        .tisciDeviceId          = 0x0u,             /* No Device Id */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,               /* No Aux devices */
        .auxInitRestoreFunction = NULL,             /* Auxilliary init function */ 
        .doneFlag               = false,            /* Initialize done flag */
    },
    /* C6X core 0 */
    {
        .testName            = "C6x core 0 PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = CSL_C66SS0_VBUSP_CFG_PBISTCFG_BASE,
        .pPBISTRegs          = (CSL_pbistRegs *)PBIST_REGION_LOCAL_BASE,
        .numPBISTRuns        = CSL_C66SS0_PBIST0_NUM_TEST_VECTORS,
        .PBISTConfigRun = {

            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_C66SS0_PBIST0_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_C66SS0_PBIST0_MEM_BITMAP_0,    /* Choose recommended mem bitmap 0  */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            },
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_C66SS0_PBIST0_ALGO_BITMAP_1,   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_C66SS0_PBIST0_MEM_BITMAP_1,    /* Choose recommended mem bitmap 1  */
                .scrambleValue      = 0xDCBA9876543210FEU, /* Scramble Value */
            }
        },
        .tisciPBISTDeviceId     = TISCI_DEV_C66SS0_PBIST0,   /* PBIST device id  */
        .pollMode               = false,
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_C66SS0_PBIST0_DFT_PBIST_CPU_0,
        .procRstNeeded          = true,
        .secondaryCoreNeeded    = false,                           /* Secondary core needed */
        .coreName               = "C6x 0",                         /* Primary core   */
        .tisciProcId            = SCICLIENT_PROC_ID_C66SS0_CORE0,  /* C6x core0 Proc Id */
        .tisciDeviceId          = TISCI_DEV_C66SS0_CORE0,          /* C6x Core 0 Device Id */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,                  /* No Aux devices */
        .auxInitRestoreFunction = NULL,             /* Auxilliary init function */ 
        .doneFlag               = false,               /* Initialize done flag */
    },
    /* C6X core 1 */
    {
        .testName            = "C6x core 1 PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = CSL_C66SS1_VBUSP_CFG_PBISTCFG_BASE,
        .pPBISTRegs          = (CSL_pbistRegs *)PBIST_REGION_LOCAL_BASE,
        .numPBISTRuns        = CSL_C66SS1_PBIST0_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_C66SS1_PBIST0_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_C66SS1_PBIST0_MEM_BITMAP_0,    /* Choose recommended mem bitmap  0 */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            },
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_C66SS1_PBIST0_ALGO_BITMAP_1,   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_C66SS1_PBIST0_MEM_BITMAP_1,    /* Choose recommended mem bitmap 1  */
                .scrambleValue      = 0xDCBA9876543210FEU, /* Scramble Value */
            }
        },
        .tisciPBISTDeviceId     = TISCI_DEV_C66SS1_PBIST0,   /* PBIST device id  */
        .pollMode               = false,
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_C66SS1_PBIST0_DFT_PBIST_CPU_0,
        .procRstNeeded          = true,              /* Initialize done flag */
        .secondaryCoreNeeded    = false,              /* Secondary core needed */
        .coreName               = "C6x 1",            /* Primary core   */
        .tisciProcId            = SCICLIENT_PROC_ID_C66SS1_CORE0,  /* C6x core0 Proc Id */
        .tisciDeviceId          = TISCI_DEV_C66SS1_CORE0,          /* C6x Core 0 Device Id */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,                  /* No Aux devices */
        .auxInitRestoreFunction = NULL,                /* Auxilliary init function */ 
        .doneFlag               = false,               /* Initialize done flag */
    },
    /* Main Infra */
    {
        .testName            = "Main Infra PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_PBIST6_BASE,
         // Golden Vectors not yet supported
        .numPBISTRuns        = CSL_PBIST6_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            /* Golden Vector Part 1 */
            {
                .override           = 0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST6_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_PBIST6_MEM_BITMAP_0,    /* Choose recommended mem bitmap 0  */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            },
            /* Golden Vector Part 2 */
            {
                .override           = 0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST6_ALGO_BITMAP_1,   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_PBIST6_MEM_BITMAP_1,    /* Choose recommended mem bitmap 1 */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            }
        },
        .tisciPBISTDeviceId     = TISCI_DEV_PBIST6,    /* PBIST device id  */
        .pollMode               = false,
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_PBIST6_DFT_PBIST_CPU_0,
        .procRstNeeded          = false,
        .secondaryCoreNeeded    = false,               /* Secondary core needed */
        .coreName               = {0x0u},              /* No coreName   */
        .tisciProcId            = 0x0u,                /* No Proc Id needed for Main Intrastructure */
        .tisciDeviceId          = 0x0u,                /* No Device Id needed for Main infrastructure */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = MAIN_INFRA_NUM_AUX_DEVICES,                  /* No Aux devices */
        .auxDeviceIdsP          = &PBIST_MainInfraAuxDevList[0], /* Array of Aux device ids */
        .auxInitRestoreFunction = PBIST_MainInfraAuxInitRestore, /* Auxilliary init function */ 
        .doneFlag               = false,               /* Initialize done flag */
    },
    /* MSMC */
    {
        .testName            = "MSMC PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = CSL_COMPUTE_CLUSTER0_MSMC_PBIST_BASE,
        .pPBISTRegs          = (CSL_pbistRegs *)PBIST_REGION_LOCAL_BASE,
        .numPBISTRuns        = CSL_COMPUTE_CLUSTER0_PBIST_WRAP_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            /* Golden Vector Part 1 */
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_COMPUTE_CLUSTER0_PBIST_WRAP_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_COMPUTE_CLUSTER0_PBIST_WRAP_MEM_BITMAP_0,    /* Choose recommended mem bitmap 0  */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            },
            /* Golden Vector Part 2 */
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_COMPUTE_CLUSTER0_PBIST_WRAP_ALGO_BITMAP_1,   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_COMPUTE_CLUSTER0_PBIST_WRAP_MEM_BITMAP_1,    /* Choose recommended mem bitmap 1  */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            }
        },
        .tisciPBISTDeviceId     = TISCI_DEV_COMPUTE_CLUSTER0_PBIST_WRAP,   /* PBIST device id  */
        .pollMode               = false,
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_PBIST_WRAP_DFT_PBIST_CPU_0,
        .procRstNeeded = true,
        .secondaryCoreNeeded    = true,                    /* Secondary core needed */
        .coreName               = "A72 core 0",            /* Primary core   */
        .secCoreName            = "A72 core 1",            /* Secondary core */
        .tisciProcId            = SCICLIENT_PROC_ID_A72SS0_CORE0, /* A72 core 0 Proc Id */
        .tisciSecProcId         = SCICLIENT_PROC_ID_A72SS0_CORE1, /* A72 core 1 Proc Id */
        .tisciDeviceId          = TISCI_DEV_A72SS0_CORE0,         /* A72 core 0 Device Id */
        .tisciSecDeviceId       = TISCI_DEV_A72SS0_CORE1,         /* A72 core 1 Device Id */
        .coreCustPwrSeqNeeded   = true,
        .numAuxDevices          = MSMC_NUM_AUX_DEVICES,           /* No Aux devices       */
        .auxDeviceIdsP          = &PBIST_MSMCAuxDevList[0],       /* Array of Aux device ids */
        .auxInitRestoreFunction = NULL,                           /* Auxilliary init function */
        .doneFlag               = false,                          /* Initialize done flag */
    },
   /* Encoder */
    {
        .testName            = "Encoder PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_PBIST2_BASE,
        .numPBISTRuns        = CSL_PBIST2_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST2_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_PBIST2_MEM_BITMAP_0,    /* Choose recommended mem bitmap 0  */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            },
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST2_ALGO_BITMAP_1,   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_PBIST2_MEM_BITMAP_1,    /* Choose recommended mem bitmap 1  */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            },

        },
        .tisciPBISTDeviceId     = TISCI_DEV_PBIST2,   /* PBIST device id  */
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_PBIST2_DFT_PBIST_CPU_0,
        .procRstNeeded          = false,
        .secondaryCoreNeeded    = false,                /* Secondary core needed */
        .coreName               = "Encoder",            /* Primary core          */
        .tisciProcId            = 0x0u,                 /* No ProcId for Encoder */
        .tisciDeviceId          = TISCI_DEV_ENCODER0,   /* Encoder Device Id     */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,                   /* No Aux devices        */
        .auxInitRestoreFunction = NULL,                 /* Auxilliary init function */ 
        .doneFlag               = false,                /* Initialize done flag  */
    },

    /* Decoder */
    {
        .testName            = "Decoder PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_PBIST0_BASE,
        .numPBISTRuns        = CSL_PBIST0_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST0_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_PBIST0_MEM_BITMAP_0,    /* Choose recommended mem bitmap 0  */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            },
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST0_ALGO_BITMAP_1,   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_PBIST0_MEM_BITMAP_1,    /* Choose recommended mem bitmap 1  */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value          */
            },

        },
        .tisciPBISTDeviceId     = TISCI_DEV_PBIST0,   /* PBIST device id  */
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_PBIST0_DFT_PBIST_CPU_0,
        .procRstNeeded          = false,
        .secondaryCoreNeeded    = false,                /* Secondary core needed */
        .coreName               = "Decoder",            /* Primary core          */
        .tisciProcId            = 0x0u,                 /* No ProcId for Decoder */
        .tisciDeviceId          = TISCI_DEV_DECODER0,   /* Device Id             */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,                   /* No Aux devices        */
        .auxInitRestoreFunction = PBIST_decoderAuxInitRestore, /* Auxilliary init function */ 
        .doneFlag               = false,                /* Initialize done flag  */
    },

    /* GPU */
    {
        .testName            = "GPU PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_GPU0_PBIST_CFG_BASE,
        .numPBISTRuns        = CSL_GPU0_DFT_PBIST_0_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_GPU0_DFT_PBIST_0_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_GPU0_DFT_PBIST_0_MEM_BITMAP_0,    /* Choose recommended mem bitmap 0  */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value        */
            },
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_GPU0_DFT_PBIST_0_ALGO_BITMAP_1,   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_GPU0_DFT_PBIST_0_MEM_BITMAP_1,    /* Choose recommended mem bitmap 1  */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value          */
            },

        },
        .tisciPBISTDeviceId     = TISCI_DEV_GPU0,   /* PBIST device id  */
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_GPU0_DFT_PBIST_0_DFT_PBIST_CPU_0,
        .procRstNeeded          = false,
        .secondaryCoreNeeded    = false,                /* Secondary core needed */
        .coreName               = "GPU",               /* Primary core   */
        .tisciProcId            = 0x0u,                 /* No ProcId for GPU */
        .tisciDeviceId          = TISCI_DEV_GPU0_GPUCORE_0, /* GPU Device Id */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = GPU_NUM_AUX_DEVICES,     /* Number of Aux devices   */
        .auxDeviceIdsP          = &PBIST_GPUAuxDevList[0], /* Array of Aux device ids */
        .auxInitRestoreFunction = PBIST_GPUAuxInitRestore,    /* Auxilliary init function */ 
        .doneFlag               = false,               /* Initialize done flag */
    },
    /* DSS EDP DSI */
    {
        .testName            = "DSS EDP PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = CSL_COMPUTE_CLUSTER0_MSMC_PBIST_BASE,
        .pPBISTRegs          = (CSL_pbistRegs *)PBIST_REGION_LOCAL_BASE,
        .numPBISTRuns        = CSL_PBIST4_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST4_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap 0 */
                .memoryGroupsBitMap = CSL_PBIST4_MEM_BITMAP_0,    /* Choose recommended mem bitmap  0 */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            },
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_PBIST4_ALGO_BITMAP_1,   /* Choose recommended Algo bitmap 1 */
                .memoryGroupsBitMap = CSL_PBIST4_MEM_BITMAP_1,    /* Choose recommended mem bitmap  1 */
                .scrambleValue      = 0xFEDCBA9876543210U, /* Scramble Value */
            },

        },
        .tisciPBISTDeviceId     = TISCI_DEV_PBIST4,   /* PBIST device id  */
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_PBIST4_DFT_PBIST_CPU_0,
        .procRstNeeded          = false,
        .secondaryCoreNeeded    = false,                /* Secondary core needed */
        .coreName               = "DSS",               /* Primary core   */
        .tisciProcId            = 0x0u,                 /* No ProcId for MSMC */
        .tisciDeviceId          = TISCI_DEV_DSS_DSI0, /* DSS Device Id */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = DSS_NUM_AUX_DEVICES,     /* Number of Aux devices   */
        .auxDeviceIdsP          = &PBIST_DSSAuxDevList[0], /* Array of Aux device ids */
        .auxInitRestoreFunction = NULL,                /* Auxilliary init function */ 
        .doneFlag               = false,               /* Initialize done flag */
    },
    /* MCU PULSAR PBIST */
    {
        .testName            = "MCU PULSAR PBIST",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_MCU_PBIST1_BASE,
        .numPBISTRuns        = CSL_MCU_PBIST1_NUM_TEST_VECTORS,
        .PBISTConfigRun = {
            {
                .override           = 0x0u,
                /* Override bit set to 0 to use memoryGroupsBitMap & algorithmsBitMap */
                .algorithmsBitMap   = CSL_MCU_PBIST1_ALGO_BITMAP_0,   /* Choose recommended Algo bitmap */
                /* NOTE: As the MCU R5f is running this code override bit cannot be set to enable all memory test */
                .memoryGroupsBitMap = CSL_MCU_PBIST1_MEM_BITMAP_0,    /* Choose recommended mem bitmap  */
                .scrambleValue      = 0xFEDCBA9876543210U,  /* Scramble Value       */
            },
        },
        .tisciPBISTDeviceId     = TISCI_DEV_MCU_PBIST1, /* PBIST device id  */
        .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_MCU_PBIST1_DFT_PBIST_CPU_0,
        .procRstNeeded          = false,
        .secondaryCoreNeeded    = false,                /* Secondary core needed */
        .coreCustPwrSeqNeeded   = false,
        .numAuxDevices          = 0u,                   /* No Aux devices        */
        .auxInitRestoreFunction = NULL,                 /* Auxilliary init function */ 
        .doneFlag               = false,                /* Initialize done flag */
    },

};


/* HW POST-related functions */

int32_t PBIST_isPostPbistTimeout(uint32_t postStatMmrRegVal, Bool *pIsTimedOut)
{
    int32_t status = CSL_PASS;

    if (pIsTimedOut == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        *pIsTimedOut = ((postStatMmrRegVal >>
                         CSL_WKUP_CTRL_MMR_CFG0_WKUP_POST_STAT_POST_MCU_PBIST_TIMEOUT_SHIFT) &
                         0x1u) ? CSL_TRUE : CSL_FALSE;
    }
    return status;
}

int32_t PBIST_isPostPbistDone(uint32_t postStatMmrRegVal, Bool *pIsDone)
{
    int32_t status = CSL_PASS;

    if (pIsDone == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        *pIsDone = ((postStatMmrRegVal >>
                    CSL_WKUP_CTRL_MMR_CFG0_WKUP_POST_STAT_POST_MCU_PBIST_DONE_SHIFT) &
                    0x1u) ? CSL_TRUE : CSL_FALSE;
    }
    return status;
}

int32_t PBIST_postCheckResult(uint32_t postStatMmrRegVal, Bool *pResult)
{
    int32_t cslResult= CSL_PASS;

    if(pResult == NULL)
    {
        cslResult = CSL_EFAIL;
    }
    else
    {
        if ((postStatMmrRegVal &
            CSL_WKUP_CTRL_MMR_CFG0_WKUP_POST_STAT_POST_MCU_PBIST_FAIL_MASK) ==
            ((uint32_t)0x00000000u))
        {
            *pResult = CSL_TRUE;
        }
        else
        {
            *pResult = CSL_FALSE;
        }
    }

    return  cslResult;
}


/* PBIST_setFirewall: Sets firewall settings to be able to access CLEC registers */
static int32_t PBIST_setFirewall(void)
{
    int32_t retVal = CSL_PASS;
    uint32_t reqFlag = TISCI_MSG_FLAG_AOP | TISCI_MSG_FLAG_DEVICE_EXCLUSIVE;
    uint32_t timeout =  SCICLIENT_SERVICE_WAIT_FOREVER;
    struct  tisci_msg_fwl_set_firewall_region_req request;
    Sciclient_ReqPrm_t reqParam;
    Sciclient_RespPrm_t respParam;

    request.fwl_id       = (uint32_t)CSL_STD_FW_NAVSS0_NAV_SRAM0_ID;
    request.region = (uint32_t) 1U; /* Pick up any unused region : 1
                                       NOTE: region 0 is used by default by
                                             System firmware for MSMC Memory currently*/
    request.n_permission_regs = CSL_FW_NUM_CBASS_FW_EP_REGION_PERMISSION;
    request.control = (FW_REGION_ENABLE & CSL_CBASS_ISC_EP_REGION_CONTROL_ENABLE_MASK);
    request.permissions[0] = (FW_MCU_R5F0_PRIVID << CSL_CBASS_FW_EP_REGION_PERMISSION_PRIV_ID_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_DEBUG_SHIFT);
    request.permissions[1] = (FW_MCU_R5F0_PRIVID << CSL_CBASS_FW_EP_REGION_PERMISSION_PRIV_ID_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_DEBUG_SHIFT);
    request.permissions[2] = (FW_MCU_R5F0_PRIVID << CSL_CBASS_FW_EP_REGION_PERMISSION_PRIV_ID_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_DEBUG_SHIFT);
    request.start_address = CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE;
    request.end_address = CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE + CSL_COMPUTE_CLUSTER0_CLEC_REGS_SIZE;

    reqParam.messageType    = (uint16_t) TISCI_MSG_SET_FWL_REGION;
    reqParam.flags          = (uint32_t) reqFlag;
    reqParam.pReqPayload    = (const uint8_t *) &request;
    reqParam.reqPayloadSize = (uint32_t) sizeof (request);
    reqParam.timeout        = (uint32_t) timeout;

    respParam.flags           = (uint32_t) 0;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) 0;
    respParam.respPayloadSize = (uint32_t) 0;


    if (((reqFlag & TISCI_MSG_FLAG_AOP) != TISCI_MSG_FLAG_AOP)&&
        (reqFlag != 0U))
    {
        retVal = CSL_EFAIL;
    }
    if (retVal == CSL_PASS)
    {
        retVal = Sciclient_service(&reqParam, &respParam);
    }
    if ((retVal != CSL_PASS) ||
        ((reqFlag != 0U) &&
        ((respParam.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)))
    {
        retVal = CSL_EFAIL;
    }
    return retVal;
}

/* Captures common Initialization: currently initializes CLEC interrupt routing
   for C7x & A72 */
int32_t PBIST_commonInit(void)
{
    CSL_ErrType_t status;
    int32_t retValue = 0;
    CSL_ClecEventConfig evtCfg;

    /* Add firewall entry to gain access to CLEC registers */
    status = PBIST_setFirewall();
    if (status != CSL_PASS)
    {
        UART_printf(" PBIST_setFirewall failed \n");
        retValue = -1;
    }

    if (retValue == 0)
    {
        evtCfg.secureClaimEnable = 1U;
        evtCfg.evtSendEnable = 1U;
        evtCfg.extEvtNum = CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12;
        evtCfg.rtMap = 2U;
        evtCfg.c7xEvtNum = 0U;

        /* Configure interrupt router to take care of routing C7x PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent C7x failed \n");
            retValue = -1;
        }
    }

    if (retValue == 0)
    {
        evtCfg.extEvtNum = CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_8;
        /* Configure interrupt router to take care of routing A72 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_8,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent A72 failed \n");
            retValue = -1;
        }
    }

    return status;
}

void PBIST_eventHandler( uint32_t instanceId)
{
    CSL_ErrType_t status;

    if (instanceId == PBIST_INSTANCE_C7X)
    {
        /* Clear C7x PBIST interrupt event in CLEC */
        status = CSL_clecClearEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                       CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12);
        if (status != CSL_PASS) {
            /* This is really not expected in normal operation:
               Add exception if needed
            */
        }
    }
    if (instanceId == PBIST_INSTANCE_A72)
    {
        /* Clear A72 PBIST interrupt event in CLEC*/
        status = CSL_clecClearEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                       CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_8);
        if (status != CSL_PASS) {
            /* This is really not expected in normal operation:
               Add exception if needed
            */
        }
    }

    PBIST_TestHandleArray[instanceId].doneFlag = true;

    return;
}

/*
    InitRestore functions : Initialize or Restore based on init flag
    init : TRUE  --> Initialize
    init : FALSE --> Restore
*/
int32_t PBIST_A72AuxInitRestore(bool init)
{
    int32_t testResult = 0;
    bool result;

    CSL_RatTranslationCfgInfo translationCfg;
    uint32_t *localP = (uint32_t *)PBIST_REGION2_LOCAL_BASE;

    /* Add RAT configuration to access address > 32bit address range */
    translationCfg.translatedAddress = CSL_COMPUTE_CLUSTER0_CC_REGS_BASE;
    translationCfg.sizeInBytes = PBIST_REG_REGION_SIZE;
    translationCfg.baseAddress = (uint32_t)PBIST_REGION2_LOCAL_BASE;

    /* Set up RAT translation */
    result = CSL_ratConfigRegionTranslation((CSL_ratRegs *)PBIST_RAT_CFG_BASE,
                                            PBIST_RAT_REGION2_INDEX, &translationCfg);
    if (result == false) {
        UART_printf("   CSL_ratConfigRegionTranslation...FAILED \n");
        testResult = -1;
    }

    if (testResult == 0)
    {
        if (init)
        {
            *((uint32_t *)(((uint32_t)localP) + 0x100)) = 0x1;
        }
        else
        {
            *((uint32_t *)(((uint32_t)localP) + 0x100)) = 0x0;
        }
    }
    if (testResult == 0)
    {
        /* Disable RAT translation */
        result = CSL_ratDisableRegionTranslation((CSL_ratRegs *)PBIST_RAT_CFG_BASE,
                                                 PBIST_RAT_REGION2_INDEX);
        if (result == false) {
            UART_printf("   CSL_ratDisableRegionTranslation...FAILED \n");
            testResult = -1;
        }
    }

    return testResult;
}

int32_t PBIST_VPACAuxInitRestore(bool init)
{
    int32_t testResult = 0;
    CSL_viss_topRegs *vissTopRegsP;

    vissTopRegsP = (CSL_viss_topRegs *)CSL_VPAC0_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_BASE;
    if (init)
    {
        vissTopRegsP->VISS_CNTL = CSL_VISS_TOP_VISS_CNTL_NSF4V_EN_MASK
                                  | CSL_VISS_TOP_VISS_CNTL_GLBCE_EN_MASK;
    }
    else
    {
        vissTopRegsP->VISS_CNTL &= (~(CSL_VISS_TOP_VISS_CNTL_NSF4V_EN_MASK
                                  | CSL_VISS_TOP_VISS_CNTL_GLBCE_EN_MASK));
    }

    return testResult;
}

int32_t PBIST_MainInfraAuxInitRestore(bool init)
{
    int32_t testResult = 0;
    CSL_Cp_aceRegs *SA2ULRegsP;

    SA2ULRegsP = (CSL_Cp_aceRegs *)CSL_SA2_UL0_BASE;

    if (init)
    {
        SA2ULRegsP->UPDATES.ENGINE_ENABLE |= CSL_CP_ACE_UPDATES_ENGINE_ENABLE_PKA_EN_MASK;
    }
    else
    {
        SA2ULRegsP->UPDATES.ENGINE_ENABLE &= (~CSL_CP_ACE_UPDATES_ENGINE_ENABLE_PKA_EN_MASK);
    }

    return testResult;
}

int32_t PBIST_decoderAuxInitRestore(bool init)
{
    int32_t testResult = 0;
    uint32_t *localP = (uint32_t *)CSL_DECODER0_MTX_CORE_BASE; 
 
    if (init)
    {
        *((uint32_t *)(((uint32_t)localP) + 0x440)) = 0xffff0113;
        *((uint32_t *)(((uint32_t)localP) + 0x444)) = 0x00000000;
        *((uint32_t *)(((uint32_t)localP) + 0x24020)) = 0x00070840;
    }
    else
    {
        /* TODO: Need to see how to restore */
    }
    return testResult;
}

int32_t PBIST_GPUAuxInitRestore(bool init)
{
    int32_t testResult = 0;
    bool result;

    CSL_RatTranslationCfgInfo translationCfg;

    uint64_t *localP = (uint64_t *)PBIST_REGION2_LOCAL_BASE; 

    /* Add RAT configuration to access address > 32bit address range */
    translationCfg.translatedAddress = CSL_GPU0_CORE_MMRS_BASE;
    translationCfg.sizeInBytes = PBIST_REG_REGION2_SIZE;
    translationCfg.baseAddress = (uint32_t)PBIST_REGION2_LOCAL_BASE;

    /* Set up RAT translation */
    result = CSL_ratConfigRegionTranslation((CSL_ratRegs *)PBIST_RAT_CFG_BASE,
                                            PBIST_RAT_REGION2_INDEX, &translationCfg);
    if (result == false) {
        UART_printf("   CSL_ratConfigRegionTranslation...FAILED \n");
        testResult = -1;
    }

    if (testResult == 0)
    {
        if (init)
        {
            *((uint64_t *)(((uint32_t)localP) + 0xA100)) = 0xffff0113;

            while(*((uint64_t *)(((uint32_t)localP) + 0xA100)) !=  (uint64_t)0x0);
            *((uint64_t *)(((uint32_t)localP) + 0x00)) = 0x0555550015155555;
            *((uint64_t *)(((uint32_t)localP) + 0x80)) = 0x0000000400000000;
        }
        else
        {
          /* TODO: Need to see how to revert */
        }
    }
    if (testResult == 0)
    {
        /* Disable RAT translation */
        result = CSL_ratDisableRegionTranslation((CSL_ratRegs *)PBIST_RAT_CFG_BASE,
                                                 PBIST_RAT_REGION2_INDEX);
        if (result == false) {
            UART_printf("   CSL_ratDisableRegionTranslation...FAILED \n");
            testResult = -1;
        }
    }

    return testResult;
}

/* Nothing past this point */
