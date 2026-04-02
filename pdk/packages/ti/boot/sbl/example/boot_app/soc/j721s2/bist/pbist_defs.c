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
 *  \file pbist_defs.c
 *
 *  \brief PBIST SOC-specific structures and functions
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <sdl_types.h>
#include <soc.h>
#include <src/ip/sdl_ip_pbist.h>
#include <src/sdl/sdl_pbist.h>
#include <ti/csl/csl_clec.h>
#include <ti/csl/src/ip/vpac/V3/cslr_viss.h>
#include <ti/csl/src/ip/vpac/V3/cslr_vpac.h>
#include <ti/csl/csl_cbass.h>
#include <ti/csl/csl_rat.h>
#include <ti/csl/cslr_cp_ace.h>
#include <ti/csl/csl_dss.h>

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

/* SCI Client */
#include <ti/drv/sciclient/sciclient.h>

#include "pbist_utils.h"
#include "pbist_defs.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define A72_NUM_AUX_DEVICES               4

#define MAIN_INFRA0_NUM_AUX_DEVICES       4

#define MAIN_INFRA1_NUM_AUX_DEVICES       6

#define HC_NUM_AUX_DEVICES                6

#define CODEC_NUM_AUX_DEVICES             1

#define GPU_NUM_AUX_DEVICES               3

#define DSS_NUM_AUX_DEVICES               7

#define C7x_0_NUM_AUX_DEVICES              1

#define C7x_1_NUM_AUX_DEVICES              1              

/* This macro defined by default to indicate only a select memory section can
 * be run on the MCU memory PBIST instances to ensure test application running
 * on MCU is not corrupted. */
#define PBIST_MCU_SELECTED_MEMORY

/* HW POST run status definitions */
#define PBIST_POST_COMPLETED_SUCCESS_STR      "SDL_PBIST_POST_COMPLETED_SUCCESS"
#define PBIST_POST_COMPLETED_FAILURE_STR      "SDL_PBIST_POST_COMPLETED_FAILURE"
#define PBIST_POST_ATTEMPTED_TIMEOUT_STR      "SDL_PBIST_POST_TIMEOUT"
#define PBIST_POST_NOT_RUN_STR                "SDL_PBIST_POST_NOT_RUN"

#define PBIST_RAT_CFG_BASE SDL_MCU_R5FSS0_RAT_CFG_BASE

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Local function prototypes                       */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*
    InitRestore functions : Initialize or Restore based on init flag
    init : TRUE  --> Initialize
    init : FALSE --> Restore
*/
int32_t PBIST_A72AuxInitRestore(bool init);
int32_t PBIST_VPACAuxInitRestore(bool init);
int32_t PBIST_HCAuxInitRestore(bool init);
int32_t PBIST_MainInfra0AuxInitRestore(bool init);
int32_t PBIST_NAVSSAuxInitRestore(bool init);
int32_t PBIST_CodecAuxInitRestore(bool init);
int32_t PBIST_GPUAuxInitRestore(bool init);
int32_t PBIST_DSSAuxInitRestore(bool init);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint32_t PBIST_MainInfra0AuxDevList[MAIN_INFRA0_NUM_AUX_DEVICES] =
{
    TISCI_DEV_MMCSD0,
    TISCI_DEV_MMCSD1,
    TISCI_DEV_CPSW1,
    TISCI_DEV_DEBUGSS_WRAP0
};

uint32_t PBIST_MainInfra1AuxDevList[MAIN_INFRA1_NUM_AUX_DEVICES] =
{
    TISCI_DEV_MCAN0,
    TISCI_DEV_MCAN1,
    TISCI_DEV_MCAN2,
    TISCI_DEV_MCAN3,
    TISCI_DEV_MCAN4,
    TISCI_DEV_MCAN5
};

uint32_t PBIST_HCAuxDevList[HC_NUM_AUX_DEVICES] =
{
    TISCI_DEV_PCIE1,
    TISCI_DEV_USB0,
    TISCI_DEV_MMCSD0,
    TISCI_DEV_MMCSD1,
    TISCI_DEV_SA2_UL0,
    TISCI_DEV_VUSR_DUAL0
};

uint32_t PBIST_CodecAuxDevList[CODEC_NUM_AUX_DEVICES] =
{
    TISCI_DEV_CODEC0
};

uint32_t PBIST_GPUAuxDevList[GPU_NUM_AUX_DEVICES] =
{
    TISCI_DEV_J7AEP_GPU_BXS464_WRAP0,
    TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPU_SS_0,
    TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPUCORE_0
};

uint32_t PBIST_DSSAuxDevList[DSS_NUM_AUX_DEVICES] =
{
    TISCI_DEV_DSS0,
    TISCI_DEV_DSS_EDP0,
    TISCI_DEV_DSS_DSI0,
    TISCI_DEV_CSI_TX_IF_V2_0,
    TISCI_DEV_CSI_TX_IF_V2_1,
    TISCI_DEV_CSI_RX_IF0,
    TISCI_DEV_CSI_RX_IF1
};

uint32_t PBIST_C7x_0AuxDevList[C7x_0_NUM_AUX_DEVICES] =
{
	TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_0,
};

uint32_t PBIST_C7x_1AuxDevList[C7x_1_NUM_AUX_DEVICES] =
{
	TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_0,
};

uint32_t PBIST_A72AuxDevList[A72_NUM_AUX_DEVICES] =
{ 
    TISCI_DEV_A72SS0_CORE0,
    TISCI_DEV_A72SS0_CORE1,
    TISCI_DEV_A72SS0,
    TISCI_DEV_A72SS0_CORE0_PBIST_WRAP,
};

PBIST_TestHandle_t PBIST_TestHandleArray[PBIST_MAX_INSTANCE+1] =
{
/* HW POST - Used to check the status of HW POST MCU PBIST */
{
    .testName              = "PBIST HWPOST MCU",
    .pbistInst             = SDL_PBIST_INST_MCUR5F0,
    .numPostPbistToCheck   = 1u,
},
/* VPAC */
{
    .testName               = "VPAC PBIST",
    .pbistInst              = SDL_PBIST_INST_VPAC_0,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_PBIST8,   /* PBIST device id  */
    .procRstNeeded          = true,
    .secondaryCoreNeeded    = false,            /* Secondary core needed */
    .coreName               = "VPAC core",      /* Primary core   */
    .tisciProcId            = 0x0u,             /* No ProcId for VPAC */
    .tisciDeviceId          = TISCI_DEV_VPAC0,  /* VPAC core Device Id */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = 0u,               /* No Aux devices */
    .auxInitRestoreFunction = PBIST_VPACAuxInitRestore,/* Auxilliary init function */
},
/* Main Infra 1 */
{
    .testName               = "Main Infra1 PBIST",
    .pbistInst              = SDL_PBIST_INST_MAININFRA1,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_PBIST1,    /* PBIST device id  */
    .procRstNeeded          = false,
    .secondaryCoreNeeded    = false,               /* Secondary core needed */
    .coreName               = "",                  /* No coreName   */
    .tisciProcId            = 0x0u,                /* No Proc Id needed for Main Intrastructure */
    .tisciDeviceId          = 0x0u,                /* No Device Id needed for Main infrastructure */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = MAIN_INFRA1_NUM_AUX_DEVICES,    /* No Aux devices */
    .auxDeviceIdsP          = &PBIST_MainInfra1AuxDevList[0], /* Array of Aux device ids */
    .auxInitRestoreFunction = NULL, /* Auxilliary init function */
},
/* DMPAC */
{
    .testName               = "DMPAC PBIST",
    .pbistInst              = SDL_PBIST_INST_DMPAC,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_PBIST7,   /* PBIST device id  */
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
},
/* Main R5F 0 */
{
    .testName               = "Main R5F 0 PBIST",
    .pbistInst              = SDL_PBIST_INST_MAINR5F0,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_PBIST2,    /* PBIST device id  */
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
},
/* Main R5F 1 */
{
    .testName               = "Main R5F 1 PBIST",
    .pbistInst              = SDL_PBIST_INST_MAINR5F1,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_PBIST10,   /* PBIST device id  */
    .procRstNeeded          = true,
    .secondaryCoreNeeded    = true,                /* Secondary core needed */
    .coreName               = "Main R5F1 core0",   /* Primary core   */
    .secCoreName            = "Main R5F1 core1",   /* Secondary core */
    .tisciProcId            = SCICLIENT_PROC_ID_R5FSS1_CORE0, /* Main R5F core 0 Proc Id */
    .tisciSecProcId         = SCICLIENT_PROC_ID_R5FSS1_CORE1, /* Main R5F core 1 Proc Id */
    .tisciDeviceId          = TISCI_DEV_R5FSS1_CORE0,  /* Main R5F core 0 Device id */
    .tisciSecDeviceId       = TISCI_DEV_R5FSS1_CORE1,  /* Main R5F core 1 Device id */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = 0u,                   /* No Aux devices */
    .auxInitRestoreFunction = NULL,                 /* Auxilliary init function */
},
/* Main Infra */
{
    .testName               = "Main Infra PBIST",
    .pbistInst              = SDL_PBIST_INST_MAININFRA0,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_PBIST0,    /* PBIST device id  */
    .procRstNeeded          = false,
    .secondaryCoreNeeded    = false,               /* Secondary core needed */
    .coreName               = "",                  /* No coreName   */
    .tisciProcId            = 0x0u,                /* No Proc Id needed for Main Intrastructure */
    .tisciDeviceId          = 0x0u,                /* No Device Id needed for Main infrastructure */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = MAIN_INFRA0_NUM_AUX_DEVICES,    /* No Aux devices */
    .auxDeviceIdsP          = &PBIST_MainInfra0AuxDevList[0], /* Array of Aux device ids */
    .auxInitRestoreFunction = PBIST_MainInfra0AuxInitRestore, /* Auxilliary init function */
},
/* HC */
{
    .testName               = "HC PBIST",
    .pbistInst              = SDL_PBIST_INST_HC,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_PBIST3, /* PBIST device id  */
    .procRstNeeded          = false,
    .secondaryCoreNeeded    = false,            /* Secondary core needed */
    .coreName               = "",               /* No coreName   */
    .tisciProcId            = 0x0u,             /* No ProcId for HC */
    .tisciDeviceId          = 0x0u,             /* No Device Id */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = HC_NUM_AUX_DEVICES, /* No Aux devices */
    .auxDeviceIdsP          = &PBIST_HCAuxDevList[0], /* Array of Aux device ids */
    .auxInitRestoreFunction = PBIST_HCAuxInitRestore,             /* Auxilliary init function */
},
/* NAVSS */
{
    .testName               = "NAVSS PBIST",
    .pbistInst              = SDL_PBIST_INST_NAVSS,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_PBIST4,   /* PBIST device id  */
    .procRstNeeded          = true,
    .secondaryCoreNeeded    = false,                /* Secondary core needed */
    .coreName               = "NAVSS",              /* Primary core   */
    .tisciProcId            = 0x0u,                 /* No ProcId for NAVSS */
    .tisciDeviceId          = TISCI_DEV_NAVSS0,     /* NAVSS Device Id */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = 0u,                   /* No Aux devices */
    .auxInitRestoreFunction = PBIST_NAVSSAuxInitRestore, /* Auxilliary init function */
},
/* Codec_1 */
{
    .testName               = "Codec PBIST",
    .pbistInst              = SDL_PBIST_INST_CODEC_1,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_PBIST11,     /* PBIST device id  */
    .procRstNeeded          = false,
    .secondaryCoreNeeded    = false,                /* Secondary core needed */
    .coreName               = "Encoder",            /* Primary core          */
    .tisciProcId            = 0x0u,                 /* No ProcId for Encoder */
    .tisciDeviceId          = TISCI_DEV_CODEC0,   /* Encoder Device Id     */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = CODEC_NUM_AUX_DEVICES, /* No Aux devices        */
    .auxDeviceIdsP          = &PBIST_CodecAuxDevList[0], /* Array of Aux device ids */
    .auxInitRestoreFunction = PBIST_CodecAuxInitRestore, /* Auxilliary init function */
},
/* GPU */
{
    .testName               = "GPU PBIST",
    .pbistInst              = SDL_PBIST_INST_GPU,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_DFT_EMBED_PBIST_0, /* PBIST device id */
    .procRstNeeded          = false,
    .secondaryCoreNeeded    = false,                      /* Secondary core needed */
    .coreName               = "GPU",                      /* Primary core   */
    .tisciProcId            = 0x0u,                       /* No ProcId for GPU */
    .tisciDeviceId          = TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPUCORE_0,   /* GPU Device Id */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = GPU_NUM_AUX_DEVICES,        /* Number of Aux devices   */
    .auxDeviceIdsP          = &PBIST_GPUAuxDevList[0],    /* Array of Aux device ids */
    .auxInitRestoreFunction = PBIST_GPUAuxInitRestore,    /* Auxilliary init function */
},
/* C7X_0 */
{
    .testName               = "C7X_0 PBIST",
    .pbistInst              = SDL_PBIST_INST_C7X_0,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_C71X_0_PBIST_VD,   /* PBIST device id  */
    .procRstNeeded          = true,
    .secondaryCoreNeeded    = false,               /* Secondary core needed */
    .coreName               = "C7x 0",
    .tisciProcId            = SCICLIENT_PROC_ID_COMPUTE_CLUSTER0_C71SS0_0,  /* C7x Proc Id */
    .tisciDeviceId          = TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_0,    /* C7x Device Id */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = C7x_0_NUM_AUX_DEVICES,                  /* Number of Aux devices */
    .auxDeviceIdsP          = &PBIST_C7x_0AuxDevList[0],
    .auxInitRestoreFunction = NULL,                /* Auxilliary init function */
},
/* C7X_1 */
{
    .testName               = "C7X_1 PBIST",
    .pbistInst              = SDL_PBIST_INST_C7X_1,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_C71X_1_PBIST_VD,   /* PBIST device id  */
    .procRstNeeded          = true,
    .secondaryCoreNeeded    = false,               /* Secondary core needed */
    .coreName               = "C7x 1",
    .tisciProcId            = SCICLIENT_PROC_ID_COMPUTE_CLUSTER0_C71SS1_0,  /* C7x Proc Id */
    .tisciDeviceId          = TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_0,    /* C7x Device Id */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = C7x_1_NUM_AUX_DEVICES,                  /* Number of Aux devices */
    .auxDeviceIdsP          = &PBIST_C7x_1AuxDevList[0],
    .auxInitRestoreFunction = NULL,                /* Auxilliary init function */
},
/* A72 */
{
    .testName               = "A72 PBIST",
    .pbistInst              = SDL_PBIST_INST_A72,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_COMPUTE_CLUSTER0_PBIST_WRAP_0, /* Device Id for A72 PBIST */
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
},
/* MSMC */
{
    .testName               = "MSMC PBIST",
    .pbistInst              = SDL_PBIST_INST_MSMC,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_COMPUTE_CLUSTER0_PBIST_WRAP_0,   /* PBIST device id  */
    .procRstNeeded          = true,
    .secondaryCoreNeeded    = true,                    /* Secondary core needed */
    .coreName               = "A72 core 0",            /* Primary core   */
    .secCoreName            = "A72 core 1",            /* Secondary core */
    .tisciProcId            = SCICLIENT_PROC_ID_A72SS0_CORE0, /* A72 core 0 Proc Id */
    .tisciSecProcId         = SCICLIENT_PROC_ID_A72SS0_CORE1, /* A72 core 1 Proc Id */
    .tisciDeviceId          = TISCI_DEV_A72SS0_CORE0,         /* A72 core 0 Device Id */
    .tisciSecDeviceId       = TISCI_DEV_A72SS0_CORE1,         /* A72 core 1 Device Id */
    .coreCustPwrSeqNeeded   = true,
    .numAuxDevices          = 0u,           /* No Aux devices       */
    .auxInitRestoreFunction = NULL,                           /* Auxilliary init function */
},
/* MCU PULSAR PBIST */
{
    .testName               = "MCU PULSAR PBIST",
    .pbistInst              = SDL_PBIST_INST_MCUR5F0,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_MCU_PBIST2, /* PBIST device id  */
    .procRstNeeded          = false,
    .secondaryCoreNeeded    = false,                /* Secondary core needed */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = 0u,                   /* No Aux devices        */
    .auxInitRestoreFunction = NULL,                 /* Auxilliary init function */
},
/* MCU PBIST - Only select memory guaranteed not to be utilized by this test application
 * can be run, since PBIST changes the memory upon which it is run. */
{
    .testName               = "MCU PBIST",
    .pbistInst              = SDL_PBIST_INST_MCU_0,
    .numPostPbistToCheck    = 1u,
    .tisciPBISTDeviceId     = TISCI_DEV_MCU_PBIST0, /* PBIST device id  */
    .procRstNeeded          = false,
    .secondaryCoreNeeded    = false,                /* Secondary core needed */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = 0u,                   /* No Aux devices */
    .auxInitRestoreFunction = NULL,                 /* Auxilliary init function */
},
/* MCU 1 PBIST - Only select memory guaranteed not to be utilized by this test application
 * can be run, since PBIST changes the memory upon which it is run. */
{
    .testName               = "MCU 1 PBIST",
    .pbistInst              = SDL_PBIST_INST_MCU_1,
    .numPostPbistToCheck    = 1u,
    .tisciPBISTDeviceId     = TISCI_DEV_MCU_PBIST1, /* PBIST device id  */
    .procRstNeeded          = false,
    .secondaryCoreNeeded    = false,                /* Secondary core needed */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = 0u,                   /* No Aux devices */
    .auxInitRestoreFunction = NULL,                 /* Auxilliary init function */
},
/* DSS EDP DSI */
{
    .testName               = "DSS EDP PBIST",
    .pbistInst              = SDL_PBIST_INST_DSS,
    .numPostPbistToCheck    = 0u,
    .tisciPBISTDeviceId     = TISCI_DEV_PBIST5,        /* PBIST device id  */
    .procRstNeeded          = false,
    .secondaryCoreNeeded    = false,                   /* Secondary core needed */
    .coreName               = "DSS",                   /* Primary core   */
    .tisciProcId            = 0x0u,                    /* No ProcId for MSMC */
    .tisciDeviceId          = TISCI_DEV_DSS_DSI0,      /* DSS Device Id */
    .coreCustPwrSeqNeeded   = false,
    .numAuxDevices          = DSS_NUM_AUX_DEVICES,     /* Number of Aux devices   */
    .auxDeviceIdsP          = &PBIST_DSSAuxDevList[0], /* Array of Aux device ids */
    .auxInitRestoreFunction = PBIST_DSSAuxInitRestore, /* Auxilliary init function */
},	
};

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

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
    translationCfg.translatedAddress = CSL_COMPUTE_CLUSTER0_CC_BASE;
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

/* define the unlock and lock values */
#define KICK0_UNLOCK_VAL 0x68EF3490
#define KICK1_UNLOCK_VAL 0xD172BC5A
#define KICK_LOCK_VAL    0x00000000

int32_t PBIST_VPACAuxInitRestore(bool init)
{
    int32_t testResult = 0;
    CSL_viss_topRegs *vissTopRegsP;

    *((uint32_t *)(SDL_WKUP_CTRL_MMR0_CFG0_BASE + SDL_WKUP_CTRL_MMR_CFG0_LOCK6_KICK0)) = KICK0_UNLOCK_VAL;
    *((uint32_t *)(SDL_WKUP_CTRL_MMR0_CFG0_BASE + SDL_WKUP_CTRL_MMR_CFG0_LOCK6_KICK1)) = KICK1_UNLOCK_VAL;
    *((uint32_t *)(SDL_WKUP_CTRL_MMR0_CFG0_BASE + SDL_WKUP_CTRL_MMR_CFG0_MAIN_CLKGATE_CTRL1)) = 0xFFFFFFFF;

    vissTopRegsP = (CSL_viss_topRegs *)CSL_VPAC0_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_BASE;
    if (init)
    {
        *((uint32_t *)(CSL_VPAC0_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_BASE + CSL_VISS_TOP_TEST_CNTL)) = 0x1;
    *((uint32_t *)(CSL_VPAC0_VPAC_REGS_VPAC_REGS_CFG_IP_MMRS_BASE + CSL_VPAC_CNTL_VPAC_TEST_CTRL)) = 0x00000003;
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

int32_t PBIST_HCAuxInitRestore(bool init)
{
    int32_t testResult = 0;

    CSL_Cp_aceRegs *SA2ULRegsP;

    SA2ULRegsP = (CSL_Cp_aceRegs *)CSL_SA2_UL0_BASE;

    *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK0)) = KICK0_UNLOCK_VAL;
    *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK1)) = KICK1_UNLOCK_VAL;
    *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_PBIST_EN)) = 0xFFFFFFFF;

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

int32_t PBIST_MainInfra0AuxInitRestore(bool init)
{
    int32_t testResult = 0;

    *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK0)) = KICK0_UNLOCK_VAL;
    *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK1)) = KICK1_UNLOCK_VAL;
    *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_PBIST_EN)) = 0xFFFFFFFF;

    return testResult;
}

int32_t PBIST_NAVSSAuxInitRestore(bool init)
{
    *((uint32_t *)(SDL_WKUP_CTRL_MMR0_CFG0_BASE + SDL_WKUP_CTRL_MMR_CFG0_LOCK6_KICK0)) = KICK0_UNLOCK_VAL;
    *((uint32_t *)(SDL_WKUP_CTRL_MMR0_CFG0_BASE + SDL_WKUP_CTRL_MMR_CFG0_LOCK6_KICK1)) = KICK1_UNLOCK_VAL;
    *((uint32_t *)(SDL_WKUP_CTRL_MMR0_CFG0_BASE + SDL_WKUP_CTRL_MMR_CFG0_MAIN_CLKGATE_CTRL1)) = 0xFFFFFFFF;

    return 0;
}

int32_t PBIST_CodecAuxInitRestore(bool init)
{
    int32_t testResult = 0;
    *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK0)) = KICK0_UNLOCK_VAL;
    *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK1)) = KICK1_UNLOCK_VAL;
    *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_PBIST_EN)) = 0xFFFFFFFF;
    return testResult;
}


int32_t PBIST_GPUAuxInitRestore(bool init)
{
    int32_t testResult = 0;
    bool result;

    CSL_RatTranslationCfgInfo translationCfg;
    
    uint64_t *localP = (uint64_t *)PBIST_REGION2_LOCAL_BASE;
    
    /* Add RAT configuration to access address > 32bit address range */
    translationCfg.translatedAddress = SDL_AEP_GPU_BXS464_WRAP0_CORE_MMRS_BASE;
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
            *((uint64_t *)(((uint32_t)localP) + 0xA100)) = 0x0;
    
            while(*((uint64_t *)(((uint32_t)localP) + 0xA100)) !=  (uint64_t)0x0);
    
            /* Force all GPU clocks on */
            *((uint64_t *)(((uint32_t)localP) + 0x00)) = 0x0015550015115555;
#if 0
            /* Force the FW Processor clocks to ON to enable testing of mars memories */
            *((uint32_t *)(((uint32_t)localP) + 0x3090)) = 0x00000001;
            i = 1000;
            while(i--);
            *((uint32_t *)(((uint32_t)localP) + 0x3480)) = 0x00000001;
            while((*((uint32_t *)(((uint32_t)localP) + 0x3488)) & 0x400) != (uint32_t)0x400);
            *((uint32_t *)(((uint32_t)localP) + 0x3480)) = 0x80000001;
            i = 1000;
            while(i--);
            while((*((uint32_t *)(((uint32_t)localP) + 0x3488)) & 0x200) != (uint32_t)0x200);
            *((uint32_t *)(((uint32_t)localP) + 0x3420)) = 0x000001FF;
            *((uint32_t *)(((uint32_t)localP) + 0x34B8)) = 0x002307F8;
            while((*((uint32_t *)(((uint32_t)localP) + 0x34B8)) & 0x1700) != (uint32_t)0x0000);
            i = 1000;
            while(i--);
            *((uint32_t *)(((uint32_t)localP) + 0x3480)) = 0x00;
            i = 1000;
            while(i--);
#endif
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

int32_t PBIST_DSSAuxInitRestore(bool init)
{
    int32_t testResult = 0;
    uint32_t value;
    uint32_t *common_m = (uint32_t *)(CSL_DSS0_DISPC_0_COMMON_M_BASE);

    uint32_t *vp2 = (uint32_t *)(CSL_DSS0_VP2_BASE);

    uint32_t *ovr2 = (uint32_t *)(SDL_DSS0_OVR2_BASE);

    uint32_t *vid1 = (uint32_t *)(SDL_DSS0_VID1_BASE);
    uint32_t *vid2 = (uint32_t *)(SDL_DSS0_VIDL1_BASE);
    uint32_t *vid3 = (uint32_t *)(SDL_DSS0_VID2_BASE);
    uint32_t *vid4 = (uint32_t *)(SDL_DSS0_VIDL2_BASE);


    if (init==TRUE)
    {
        *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK0)) = KICK0_UNLOCK_VAL;
        *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK1)) = KICK1_UNLOCK_VAL;
        *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_PBIST_EN)) = 0xFFFFFFFF;

        *((uint32_t *)(SDL_WKUP_CTRL_MMR0_CFG0_BASE + SDL_WKUP_CTRL_MMR_CFG0_LOCK6_KICK0)) = KICK0_UNLOCK_VAL;
        *((uint32_t *)(SDL_WKUP_CTRL_MMR0_CFG0_BASE + SDL_WKUP_CTRL_MMR_CFG0_LOCK6_KICK1)) = KICK1_UNLOCK_VAL;
        *((uint32_t *)(SDL_WKUP_CTRL_MMR0_CFG0_BASE + SDL_WKUP_CTRL_MMR_CFG0_MAIN_CLKGATE_CTRL2)) = 0xFFFFFFFF;

        CSL_REG32_WR(&((CSL_dss_vid1Regs*)(vid1))->FBDC_ATTRIBUTES, 1);
        CSL_REG32_WR(&((CSL_dss_vidl1Regs*)(vid2))->FBDC_ATTRIBUTES, 1);
        CSL_REG32_WR(&((CSL_dss_vid1Regs*)(vid3))->FBDC_ATTRIBUTES, 1);
        CSL_REG32_WR(&((CSL_dss_vidl1Regs*)(vid4))->FBDC_ATTRIBUTES, 1);

        CSL_REG32_FINS(&((CSL_dss_vidl1Regs*)(vid2))->ATTRIBUTES, DSS_VIDL1_ATTRIBUTES_ENABLE, 1);

        value = CSL_FMK(DSS_OVR1_ATTRIBUTES_ENABLE, 1) | CSL_FMK(DSS_OVR1_ATTRIBUTES_CHANNELIN, 1);
        CSL_REG32_WR(&((CSL_dss_ovr1Regs *)(ovr2))->ATTRIBUTES, value);

        {
            uint32_t division = 2400 / 100;

            uint32_t divfactor = 0x8000 | (division-1);

            *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_LOCK2_KICK0)) = KICK0_UNLOCK_VAL;
            *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + SDL_MAIN_CTRL_MMR_CFG0_LOCK2_KICK1)) = KICK1_UNLOCK_VAL;

            *((uint32_t *)(SDL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_DSS_DISPC0_CLKSEL1)) = 0x1;
            *((uint32_t *)(CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL19_PID + CSL_MAIN_PLL_MMR_CFG_PLL0_HSDIV_CTRL0)) = divfactor;
        }

        CSL_REG32_FINS(&((CSL_dss_common_mRegs*)(common_m))->DISPC_CONNECTIONS, DSS_COMMON_M_DISPC_CONNECTIONS_DPI_0_CONN, 4);

        CSL_REG32_FINS(&((CSL_dss_vp1Regs*)(vp2))->CONTROL, DSS_VP1_CONTROL_ENABLE, 1);
    }
    else
    {
        CSL_REG32_FINS(&((CSL_dss_vidl1Regs*)(vid2))->ATTRIBUTES, DSS_VIDL1_ATTRIBUTES_ENABLE, 0);
        CSL_REG32_FINS(&((CSL_dss_vp1Regs*)(vp2))->CONTROL, DSS_VP1_CONTROL_ENABLE, 0);
    }

    return testResult;
}

char * PBIST_getPostStatusString(SDL_PBIST_postStatus postStatus)
{
    if (postStatus == SDL_PBIST_POST_NOT_RUN)
    {
        return PBIST_POST_NOT_RUN_STR;
    }
    else if (postStatus == SDL_PBIST_POST_TIMEOUT)
    {
        return PBIST_POST_ATTEMPTED_TIMEOUT_STR;
    }
    else if (postStatus == SDL_PBIST_POST_COMPLETED_FAILURE)
    {
        return PBIST_POST_COMPLETED_FAILURE_STR;
    }
    else if (postStatus == SDL_PBIST_POST_COMPLETED_SUCCESS)
    {
        return PBIST_POST_COMPLETED_SUCCESS_STR;
    }
    return NULL;
}

void PBIST_printPostStatus(SDL_PBIST_postResult *result)
{
    UART_printf("    HW POST MCU Status : %s\n", (PBIST_getPostStatusString(result->mcuPostStatus)) ? : "Invalid");

    return;
}
