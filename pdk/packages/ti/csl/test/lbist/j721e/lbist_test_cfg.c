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
 *  \file     lbist_test_cfg.c
 *
 *  \brief    This file contains LBIST test configuration
 *
 *  \details  LBIST Test Configuration
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/csl_lbist.h>
#include <ti/csl/soc.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/sciclient/sciclient.h>

/* Osal API header files */
#include <ti/osal/HwiP.h>
#include <ti/osal/TimerP.h>

#include "lbist_test_cfg.h"

/* #define DEBUG */

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
/* Lbist Parameters */
#define LBIST_DC_DEF                   (0x3u)
#define LBIST_DIVIDE_RATIO             (0x02u)
#define LBIST_STATIC_PC_DEF            (0x3fffu) /* 16383 */
#define LBIST_RESET_PC_DEF             (0x0fu)
#define LBIST_SET_PC_DEF               (0x00u)
#define LBIST_SCAN_PC_DEF              (0x08u)
#define LBIST_PRPG_DEF                 (0x1fffffffffffffu)

/*
* LBIST setup parameters for each core
*/
#define LBIST_MAIN_R5_STATIC_PC_DEF    LBIST_STATIC_PC_DEF
#define LBIST_C7X_STATIC_PC_DEF        (2816)
#define LBIST_A72_STATIC_PC_DEF        (12288)
#define LBIST_DMPAC_STATIC_PC_DEF      (6272)
#define LBIST_VPAC_STATIC_PC_DEF       (5056)

/*
* LBIST expected MISR's (using parameters above)
*/
#define MAIN_R5_MISR_EXP_VAL           (0xad7f4501)
#define A72_MISR_EXP_VAL               (0xdd5cd3b3)
#define C7X_MISR_EXP_VAL               (0xd67bfff1)
#define VPAC_MISR_EXP_VAL              (0x18b373bf)
#define DMPAC_MISR_EXP_VAL             (0xf22e52b5)

/*
 * LBIST setup paramters for each core for ES1.1
 */
#define ES1_1_LBIST_SCAN_PC_DEF           (0x04u)
#define ES1_1_LBIST_MAIN_R5_STATIC_PC_DEF (0x3ac0u)
#define ES1_1_LBIST_C7X_STATIC_PC_DEF     (3008)
#define ES1_1_LBIST_A72_STATIC_PC_DEF     (15040)
#define ES1_1_LBIST_VPAC_STATIC_PC_DEF    (6016)

/*
 * LBIST expected MISR's for ES1.1 (using parameters above)
 */
#define ES1_1_MAIN_R5_MISR_EXP_VAL        (0x49379bdb)
#define ES1_1_A72_MISR_EXP_VAL            (0xe6a80965)
#define ES1_1_C7X_MISR_EXP_VAL            (0xcc08b144)
#define ES1_1_VPAC_MISR_EXP_VAL           (0xf99d3ab7)
#define ES1_1_DMPAC_MISR_EXP_VAL          (0xac4cc9c8)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint32_t LBIST_A72AuxDevList[A72_NUM_AUX_DEVICES] =
{
    TISCI_DEV_A72SS0,
};

LBIST_TestHandle_t LBIST_TestHandleArray_ES1_0[LBIST_MAX_CORE_INDEX+1] =
{
 /* HW POST - DMSC - Checks MISR results only */
 {
  .coreName               = "HWPOST - DMSC",
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_DMSC_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_DMSC_LBIST_SIG),
  .doneFlag               = false,                    /* Initialize done flag */
  .numAuxDevices          = 0u,                       /* No Aux devices */
  .hwPostCoreCheck        = true,
  .hwPostCoreNum          = LBIST_POST_CORE_DMSC,
  .handler                = NULL             ,       /* LBIST event handler */
 },
 /* HW POST - MCU - Checks MISR results only */
 {
  .coreName               = "HWPOST - MCU",
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_MCU_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_MCU_LBIST_SIG),
  .doneFlag               = false,                    /* Initialize done flag */
  .numAuxDevices          = 0u,                       /* No Aux devices */
  .hwPostCoreCheck        = true,
  .hwPostCoreNum          = LBIST_POST_CORE_MCU,
  .handler                = NULL             ,       /* LBIST event handler */
 },
 /* Main R5F 0 */
 {
  .coreName               = "Main R5F0-0",
  .secondaryCoreNeeded    = true,             /* Secondary core needed */
  .wfiCheckNeeded         = false,            /* wfi check needed */
  .secCoreName            = "Main R5F1-0",    /* Secondary core */
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_MAIN_R5F0_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_MAIN_R5F0_LBIST_SIG),
  .expectedMISR           = MAIN_R5_MISR_EXP_VAL ,    /* Expected signature for main R5 0*/
  .cpuStatusFlagMask      = TISCI_MSG_VAL_PROC_BOOT_STATUS_FLAG_R5_WFI, /* Expected boot status value for wfi */
  .handler                = LBIST_eventHandler,       /* LBIST event handler */
  .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_GLUELOGIC_MAIN_PULSAR0_LBIST_GLUE_DFT_LBIST_BIST_DONE_0, /* BIST DONE interrupt number */
  .tisciProcId            = SCICLIENT_PROC_ID_R5FSS0_CORE0, /* Main R5F core 0 Proc Id */
  .tisciSecProcId         = SCICLIENT_PROC_ID_R5FSS0_CORE1, /* Main R5F core 1 Proc Id */
  .tisciDeviceId          = TISCI_DEV_R5FSS0_CORE0,   /* Main R5F core 0 Device Id */
  .tisciSecDeviceId       = TISCI_DEV_R5FSS0_CORE1,   /* Main R5F core 1 Device Id */
  .doneFlag               = false,                    /* Initialize done flag */
  .numAuxDevices          = 0u,                       /* No Aux devices */
  .LBISTConfig = {
      .dc_def        = LBIST_DC_DEF,
      .divide_ratio  = LBIST_DIVIDE_RATIO,
      .static_pc_def = LBIST_MAIN_R5_STATIC_PC_DEF,
      .set_pc_def    = LBIST_SET_PC_DEF,
      .reset_pc_def  = LBIST_RESET_PC_DEF,
      .scan_pc_def   = LBIST_SCAN_PC_DEF,
      .prpg_def      = LBIST_PRPG_DEF,
  },
 },
 /* Main R5F 1 */
 {
  .coreName               = "Main R5F1-0",
  .secondaryCoreNeeded    = true,            /* Secondary core needed */
  .wfiCheckNeeded         = false,           /* wfi check needed */
  .secCoreName            = "Main R5F1-1",   /* Secondary core */
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_MAIN_R5F1_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_MAIN_R5F1_LBIST_SIG),
  .expectedMISR           = MAIN_R5_MISR_EXP_VAL,    /* Expected signature Main R5 1*/
  .cpuStatusFlagMask      = TISCI_MSG_VAL_PROC_BOOT_STATUS_FLAG_R5_WFI, /* Expected boot status value for wfi */
  .handler                = LBIST_eventHandler,      /* LBIST event handler */
  .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_GLUELOGIC_MAIN_PULSAR1_LBIST_GLUE_DFT_LBIST_BIST_DONE_0,/* BIST DONE interrupt number */
  .tisciProcId            = SCICLIENT_PROC_ID_R5FSS1_CORE0, /* Main R5F core 0 Proc Id */
  .tisciSecProcId         = SCICLIENT_PROC_ID_R5FSS1_CORE1, /* Main R5F core 1 Proc Id */
  .tisciDeviceId          = TISCI_DEV_R5FSS1_CORE0,  /* Main R5F core 0 Device id */
  .tisciSecDeviceId       = TISCI_DEV_R5FSS1_CORE1,  /* Main R5F core 1 Device id */
  .doneFlag               = false,                   /* Initialize done flag */
  .numAuxDevices          = 0u,                      /* No Aux devices */
  .LBISTConfig = {
      .dc_def        = LBIST_DC_DEF,
      .divide_ratio  = LBIST_DIVIDE_RATIO,
      .static_pc_def = LBIST_MAIN_R5_STATIC_PC_DEF,
      .set_pc_def    = LBIST_SET_PC_DEF,
      .reset_pc_def  = LBIST_RESET_PC_DEF,
      .scan_pc_def   = LBIST_SCAN_PC_DEF,
      .prpg_def      = LBIST_PRPG_DEF,
  },
 },
 /* C7x */
 {
  .coreName               = "C7x ",
  .secondaryCoreNeeded    = false,  /* Secondary core needed */
  .wfiCheckNeeded         = false,  /* wfi check needed */
  .secCoreName            = "None",   /* Secondary core */
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_C7X_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_C7X_LBIST_SIG),
  .expectedMISR           = C7X_MISR_EXP_VAL,        /* Expected signature for C7x*/
  .cpuStatusFlagMask      = TISCI_MSG_VAL_PROC_BOOT_STATUS_FLAG_R5_WFI, /* Expected boot status value for wfi */
  .handler                = LBIST_eventHandler,      /* LBIST event handler */
  .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_C7X_4_DFT_LBIST_DFT_LBIST_BIST_DONE_0,/* BIST DONE interrupt number */
  .tisciProcId            = SCICLIENT_PROC_ID_C71SS0,  /* C7x Proc Id */
  .tisciSecProcId         = 0,
  .tisciDeviceId          = TISCI_DEV_C71SS0,          /* C7x Device Id */
  .tisciSecDeviceId       = 0,
  .doneFlag               = false,                     /* Initialize done flag */
  .numAuxDevices          = 0u,                        /* No Aux devices */
  .LBISTConfig = {
      .dc_def        = LBIST_DC_DEF,
      .divide_ratio  = LBIST_DIVIDE_RATIO,
      .static_pc_def = LBIST_C7X_STATIC_PC_DEF,
      .set_pc_def    = LBIST_SET_PC_DEF,
      .reset_pc_def  = LBIST_RESET_PC_DEF,
      .scan_pc_def   = LBIST_SCAN_PC_DEF,
      .prpg_def      = LBIST_PRPG_DEF,
  },
 },

 /* VPAC */
 {
  .coreName               = "VPAC",
  .secondaryCoreNeeded    = false,           /* Secondary core needed */
  .wfiCheckNeeded         = false,           /* wfi check needed */
  .secCoreName            = "None",          /* Secondary core */
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_VPAC_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_VPAC_LBIST_SIG),
  .expectedMISR           = VPAC_MISR_EXP_VAL,                          /* Expected signature for C6x*/
  .cpuStatusFlagMask      = TISCI_MSG_VAL_PROC_BOOT_STATUS_FLAG_R5_WFI, /* Expected boot status value for wfi */
  .handler                = LBIST_eventHandler,                         /* LBIST event handler */
  .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_GLUELOGIC_VPAC_LBIST_GLUE_DFT_LBIST_BIST_DONE_0,/* BIST DONE interrupt number */
  .tisciProcId            = 0,  /* No proc id */
  .tisciSecProcId         = 0,  /* No Proc Id */
  .tisciDeviceId          = TISCI_DEV_VPAC0,                       /* VPAC Device Id */
  .tisciSecDeviceId       = 0,
  .doneFlag               = false,                                 /* Initialize done flag */
  .numAuxDevices          = 0u,                                    /* No Aux devices */
  .LBISTConfig = {
      .dc_def        = LBIST_DC_DEF,
      .divide_ratio  = LBIST_DIVIDE_RATIO,
      .static_pc_def = LBIST_VPAC_STATIC_PC_DEF,
      .set_pc_def    = LBIST_SET_PC_DEF,
      .reset_pc_def  = LBIST_RESET_PC_DEF,
      .scan_pc_def   = LBIST_SCAN_PC_DEF,
      .prpg_def      = LBIST_PRPG_DEF,
  },
 },

 /* DMPAC */
 {
  .coreName               = "DMPAC",
  .secondaryCoreNeeded    = false,           /* Secondary core needed */
  .wfiCheckNeeded         = false,           /* wfi check needed */
  .secCoreName            = "None",          /* Secondary core */
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_DMPAC_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_DMPAC_LBIST_SIG),
  .expectedMISR           = DMPAC_MISR_EXP_VAL,                         /* Expected signature for C6x*/
  .cpuStatusFlagMask      = TISCI_MSG_VAL_PROC_BOOT_STATUS_FLAG_R5_WFI, /* Expected boot status value for wfi */
  .handler                = LBIST_eventHandler,                         /* LBIST event handler */
  .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_GLUELOGIC_DMPAC_LBIST_GLUE_DFT_LBIST_BIST_DONE_0,/* BIST DONE interrupt number */
  .tisciProcId            = 0,  /* No proc id */
  .tisciSecProcId         = 0,  /* No Proc Id */
  .tisciDeviceId          = TISCI_DEV_DMPAC0,                       /* DMPAC Device Id */
  .tisciSecDeviceId       = 0,
  .doneFlag               = false,                                  /* Initialize done flag */
  .numAuxDevices          = 0u,                                     /* No Aux devices */
  .LBISTConfig = {
      .dc_def        = LBIST_DC_DEF,
      .divide_ratio  = LBIST_DIVIDE_RATIO,
      .static_pc_def = LBIST_DMPAC_STATIC_PC_DEF,
      .set_pc_def    = LBIST_SET_PC_DEF,
      .reset_pc_def  = LBIST_RESET_PC_DEF,
      .scan_pc_def   = LBIST_SCAN_PC_DEF,
      .prpg_def      = LBIST_PRPG_DEF,
  },
 },
 
 /* A72 */
 {
  .coreName               = "A72 core 0",
  .secondaryCoreNeeded    = true,           /* Secondary core needed */
  .wfiCheckNeeded         = false,          /* wfi check needed */
  .secCoreName            = "A72 core 1",   /* Secondary core */
  .pLBISTRegs                = (CSL_lbistRegs *)(CSL_A72_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_A72_LBIST_SIG),
  .expectedMISR           = A72_MISR_EXP_VAL,                           /* Expected signature for A72 */
  .cpuStatusFlagMask      = TISCI_MSG_VAL_PROC_BOOT_STATUS_FLAG_R5_WFI, /* Expected boot status value for wfi */
  .handler                = LBIST_eventHandler,                         /* LBIST event handler */
  .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_ARM0_DFT_LBIST_DFT_LBIST_BIST_DONE_0,/* BIST DONE interrupt number */
  .tisciProcId            = SCICLIENT_PROC_ID_A72SS0_CORE0, /* A72 core 0 Proc Id */
  .tisciSecProcId         = SCICLIENT_PROC_ID_A72SS0_CORE1, /* A72 core 1 Proc Id */
  .tisciDeviceId          = TISCI_DEV_A72SS0_CORE0,  /* A72 core 0 Device Id */
  .tisciSecDeviceId       = TISCI_DEV_A72SS0_CORE1,  /* A72 core 1 Device Id */
  .doneFlag               = false,                   /* Initialize done flag */
  .numAuxDevices          = A72_NUM_AUX_DEVICES,     /* Number of Aux devices */
  .auxDeviceIdsP          = &LBIST_A72AuxDevList[0], /* Array of Aux device ids */
  .LBISTConfig = {
      .dc_def        = LBIST_DC_DEF,
      .divide_ratio  = LBIST_DIVIDE_RATIO,
      .static_pc_def = LBIST_A72_STATIC_PC_DEF,
      .set_pc_def    = LBIST_SET_PC_DEF,
      .reset_pc_def  = LBIST_RESET_PC_DEF,
      .scan_pc_def   = LBIST_SCAN_PC_DEF,
      .prpg_def      = LBIST_PRPG_DEF,
  },
 },

};

LBIST_TestHandle_t LBIST_TestHandleArray_ES1_1[LBIST_MAX_CORE_INDEX+1] =
{
 /* HW POST - DMSC - Checks MISR results only */
 {
  .coreName               = "HWPOST - DMSC",
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_DMSC_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_DMSC_LBIST_SIG),
  .doneFlag               = false,                    /* Initialize done flag */
  .numAuxDevices          = 0u,                       /* No Aux devices */
  .hwPostCoreCheck        = true,
  .hwPostCoreNum          = LBIST_POST_CORE_DMSC,
  .handler                = NULL             ,       /* LBIST event handler */
 },
 /* HW POST - MCU - Checks MISR results only */
 {
  .coreName               = "HWPOST - MCU",
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_MCU_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_MCU_LBIST_SIG),
  .doneFlag               = false,                    /* Initialize done flag */
  .numAuxDevices          = 0u,                       /* No Aux devices */
  .hwPostCoreCheck        = true,
  .hwPostCoreNum          = LBIST_POST_CORE_MCU,
  .handler                = NULL             ,       /* LBIST event handler */
 },
 /* Main R5F 0 */
 {
  .coreName               = "Main R5F0-0",
  .secondaryCoreNeeded    = true,             /* Secondary core needed */
  .wfiCheckNeeded         = false,            /* wfi check needed */
  .secCoreName            = "Main R5F1-0",    /* Secondary core */
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_MAIN_R5F0_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_MAIN_R5F0_LBIST_SIG),
  .expectedMISR           = ES1_1_MAIN_R5_MISR_EXP_VAL,    /* Expected signature for main R5 0*/
  .cpuStatusFlagMask      = TISCI_MSG_VAL_PROC_BOOT_STATUS_FLAG_R5_WFI, /* Expected boot status value for wfi */
  .handler                = LBIST_eventHandler,       /* LBIST event handler */
  .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_GLUELOGIC_MAIN_PULSAR0_LBIST_GLUE_DFT_LBIST_BIST_DONE_0, /* BIST DONE interrupt number */
  .tisciProcId            = SCICLIENT_PROC_ID_R5FSS0_CORE0, /* Main R5F core 0 Proc Id */
  .tisciSecProcId         = SCICLIENT_PROC_ID_R5FSS0_CORE1, /* Main R5F core 1 Proc Id */
  .tisciDeviceId          = TISCI_DEV_R5FSS0_CORE0,   /* Main R5F core 0 Device Id */
  .tisciSecDeviceId       = TISCI_DEV_R5FSS0_CORE1,   /* Main R5F core 1 Device Id */
  .doneFlag               = false,                    /* Initialize done flag */
  .numAuxDevices          = 0u,                       /* No Aux devices */
  .LBISTConfig = {
      .dc_def        = LBIST_DC_DEF,
      .divide_ratio  = LBIST_DIVIDE_RATIO,
      .static_pc_def = ES1_1_LBIST_MAIN_R5_STATIC_PC_DEF,
      .set_pc_def    = LBIST_SET_PC_DEF,
      .reset_pc_def  = LBIST_RESET_PC_DEF,
      .scan_pc_def   = ES1_1_LBIST_SCAN_PC_DEF,
      .prpg_def      = LBIST_PRPG_DEF,
  },
 },
 /* Main R5F 1 */
 {
  .coreName               = "Main R5F1-0",
  .secondaryCoreNeeded    = true,            /* Secondary core needed */
  .wfiCheckNeeded         = false,           /* wfi check needed */
  .secCoreName            = "Main R5F1-1",   /* Secondary core */
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_MAIN_R5F1_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_MAIN_R5F1_LBIST_SIG),
  .expectedMISR           = ES1_1_MAIN_R5_MISR_EXP_VAL,    /* Expected signature Main R5 1*/
  .cpuStatusFlagMask      = TISCI_MSG_VAL_PROC_BOOT_STATUS_FLAG_R5_WFI, /* Expected boot status value for wfi */
  .handler                = LBIST_eventHandler,      /* LBIST event handler */
  .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_GLUELOGIC_MAIN_PULSAR1_LBIST_GLUE_DFT_LBIST_BIST_DONE_0,/* BIST DONE interrupt number */
  .tisciProcId            = SCICLIENT_PROC_ID_R5FSS1_CORE0, /* Main R5F core 0 Proc Id */
  .tisciSecProcId         = SCICLIENT_PROC_ID_R5FSS1_CORE1, /* Main R5F core 1 Proc Id */
  .tisciDeviceId          = TISCI_DEV_R5FSS1_CORE0,  /* Main R5F core 0 Device id */
  .tisciSecDeviceId       = TISCI_DEV_R5FSS1_CORE1,  /* Main R5F core 1 Device id */
  .doneFlag               = false,                   /* Initialize done flag */
  .numAuxDevices          = 0u,                      /* No Aux devices */
  .LBISTConfig = {
      .dc_def        = LBIST_DC_DEF,
      .divide_ratio  = LBIST_DIVIDE_RATIO,
      .static_pc_def = ES1_1_LBIST_MAIN_R5_STATIC_PC_DEF,
      .set_pc_def    = LBIST_SET_PC_DEF,
      .reset_pc_def  = LBIST_RESET_PC_DEF,
      .scan_pc_def   = ES1_1_LBIST_SCAN_PC_DEF,
      .prpg_def      = LBIST_PRPG_DEF,
  },
 },
 /* C7x */
 {
  .coreName               = "C7x ",
  .secondaryCoreNeeded    = false,  /* Secondary core needed */
  .wfiCheckNeeded         = false,  /* wfi check needed */
  .secCoreName            = "None",   /* Secondary core */
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_C7X_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_C7X_LBIST_SIG),
  .expectedMISR           = ES1_1_C7X_MISR_EXP_VAL,        /* Expected signature for C7x*/
  .cpuStatusFlagMask      = TISCI_MSG_VAL_PROC_BOOT_STATUS_FLAG_R5_WFI, /* Expected boot status value for wfi */
  .handler                = LBIST_eventHandler,      /* LBIST event handler */
  .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_C7X_4_DFT_LBIST_DFT_LBIST_BIST_DONE_0,/* BIST DONE interrupt number */
  .tisciProcId            = SCICLIENT_PROC_ID_C71SS0,  /* C7x Proc Id */
  .tisciSecProcId         = 0,
  .tisciDeviceId          = TISCI_DEV_C71SS0,          /* C7x Device Id */
  .tisciSecDeviceId       = 0,
  .doneFlag               = false,                     /* Initialize done flag */
  .numAuxDevices          = 0u,                        /* No Aux devices */
  .LBISTConfig = {
      .dc_def        = LBIST_DC_DEF,
      .divide_ratio  = LBIST_DIVIDE_RATIO,
      .static_pc_def = ES1_1_LBIST_C7X_STATIC_PC_DEF,
      .set_pc_def    = LBIST_SET_PC_DEF,
      .reset_pc_def  = LBIST_RESET_PC_DEF,
      .scan_pc_def   = ES1_1_LBIST_SCAN_PC_DEF,
      .prpg_def      = LBIST_PRPG_DEF,
  },
 },

 /* VPAC */
 {
  .coreName               = "VPAC",
  .secondaryCoreNeeded    = false,           /* Secondary core needed */
  .wfiCheckNeeded         = false,           /* wfi check needed */
  .secCoreName            = "None",          /* Secondary core */
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_VPAC_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_VPAC_LBIST_SIG),
  .expectedMISR           = ES1_1_VPAC_MISR_EXP_VAL,                          /* Expected signature for C6x*/
  .cpuStatusFlagMask      = TISCI_MSG_VAL_PROC_BOOT_STATUS_FLAG_R5_WFI, /* Expected boot status value for wfi */
  .handler                = LBIST_eventHandler,                         /* LBIST event handler */
  .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_GLUELOGIC_VPAC_LBIST_GLUE_DFT_LBIST_BIST_DONE_0,/* BIST DONE interrupt number */
  .tisciProcId            = 0,  /* No proc id */
  .tisciSecProcId         = 0,  /* No Proc Id */
  .tisciDeviceId          = TISCI_DEV_VPAC0,                       /* VPAC Device Id */
  .tisciSecDeviceId       = 0,
  .doneFlag               = false,                                 /* Initialize done flag */
  .numAuxDevices          = 0u,                                    /* No Aux devices */
  .LBISTConfig = {
      .dc_def        = LBIST_DC_DEF,
      .divide_ratio  = LBIST_DIVIDE_RATIO,
      .static_pc_def = ES1_1_LBIST_VPAC_STATIC_PC_DEF,
      .set_pc_def    = LBIST_SET_PC_DEF,
      .reset_pc_def  = LBIST_RESET_PC_DEF,
      .scan_pc_def   = ES1_1_LBIST_SCAN_PC_DEF,
      .prpg_def      = LBIST_PRPG_DEF,
  },
 },

 /* DMPAC */
 {
  .coreName               = "DMPAC",
  .secondaryCoreNeeded    = false,           /* Secondary core needed */
  .wfiCheckNeeded         = false,           /* wfi check needed */
  .secCoreName            = "None",          /* Secondary core */
  .pLBISTRegs             = (CSL_lbistRegs *)(CSL_DMPAC_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_DMPAC_LBIST_SIG),
  .expectedMISR           = ES1_1_DMPAC_MISR_EXP_VAL,                         /* Expected signature for C6x*/
  .cpuStatusFlagMask      = TISCI_MSG_VAL_PROC_BOOT_STATUS_FLAG_R5_WFI, /* Expected boot status value for wfi */
  .handler                = LBIST_eventHandler,                         /* LBIST event handler */
  .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_GLUELOGIC_DMPAC_LBIST_GLUE_DFT_LBIST_BIST_DONE_0,/* BIST DONE interrupt number */
  .tisciProcId            = 0,  /* No proc id */
  .tisciSecProcId         = 0,  /* No Proc Id */
  .tisciDeviceId          = TISCI_DEV_DMPAC0,                       /* DMPAC Device Id */
  .tisciSecDeviceId       = 0,
  .doneFlag               = false,                                  /* Initialize done flag */
  .numAuxDevices          = 0u,                                     /* No Aux devices */
  .LBISTConfig = {
      .dc_def        = LBIST_DC_DEF,
      .divide_ratio  = LBIST_DIVIDE_RATIO,
      .static_pc_def = LBIST_DMPAC_STATIC_PC_DEF,
      .set_pc_def    = LBIST_SET_PC_DEF,
      .reset_pc_def  = LBIST_RESET_PC_DEF,
      .scan_pc_def   = ES1_1_LBIST_SCAN_PC_DEF,
      .prpg_def      = LBIST_PRPG_DEF,
  },
 },
 
 /* A72 */
 {
  .coreName               = "A72 core 0",
  .secondaryCoreNeeded    = true,           /* Secondary core needed */
  .wfiCheckNeeded         = false,          /* wfi check needed */
  .secCoreName            = "A72 core 1",   /* Secondary core */
  .pLBISTRegs                = (CSL_lbistRegs *)(CSL_A72_LBIST_BASE),
  .pLBISTSig              = (uint32_t *)(CSL_A72_LBIST_SIG),
  .expectedMISR           = ES1_1_A72_MISR_EXP_VAL,                           /* Expected signature for A72 */
  .cpuStatusFlagMask      = TISCI_MSG_VAL_PROC_BOOT_STATUS_FLAG_R5_WFI, /* Expected boot status value for wfi */
  .handler                = LBIST_eventHandler,                         /* LBIST event handler */
  .interruptNumber        = CSLR_MCU_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_ARM0_DFT_LBIST_DFT_LBIST_BIST_DONE_0,/* BIST DONE interrupt number */
  .tisciProcId            = SCICLIENT_PROC_ID_A72SS0_CORE0, /* A72 core 0 Proc Id */
  .tisciSecProcId         = SCICLIENT_PROC_ID_A72SS0_CORE1, /* A72 core 1 Proc Id */
  .tisciDeviceId          = TISCI_DEV_A72SS0_CORE0,  /* A72 core 0 Device Id */
  .tisciSecDeviceId       = TISCI_DEV_A72SS0_CORE1,  /* A72 core 1 Device Id */
  .doneFlag               = false,                   /* Initialize done flag */
  .numAuxDevices          = A72_NUM_AUX_DEVICES,     /* Number of Aux devices */
  .auxDeviceIdsP          = &LBIST_A72AuxDevList[0], /* Array of Aux device ids */
  .LBISTConfig = {
      .dc_def        = LBIST_DC_DEF,
      .divide_ratio  = LBIST_DIVIDE_RATIO,
      .static_pc_def = ES1_1_LBIST_A72_STATIC_PC_DEF,
      .set_pc_def    = LBIST_SET_PC_DEF,
      .reset_pc_def  = LBIST_RESET_PC_DEF,
      .scan_pc_def   = ES1_1_LBIST_SCAN_PC_DEF,
      .prpg_def      = LBIST_PRPG_DEF,
  },
 },

};

static int32_t LBIST_isPostLbistTimeout(uint32_t postStatMmrRegVal,
                                        uint8_t section,
                                        Bool *pIsTimedOut);

static int32_t LBIST_isPostLbistDone(uint32_t postStatMmrRegVal,
                                     uint8_t section,
                                     Bool *pIsDone);

static int32_t LBIST_isPostLbistTimeout(uint32_t postStatMmrRegVal,
                                        uint8_t section,
                                        Bool *pIsTimedOut)
{
    int32_t status = CSL_PASS;
    uint32_t shift = 0U;

    if ((pIsTimedOut == NULL) || (section > LBIST_POST_CORE_MAX))
    {
        status = CSL_EBADARGS;
    }
    else
    {
        if (section == LBIST_POST_CORE_DMSC)
        {
            shift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_POST_STAT_POST_DMSC_LBIST_TIMEOUT_SHIFT;
        }
        else if (section == LBIST_POST_CORE_MCU)
        {
            shift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_POST_STAT_POST_MCU_LBIST_TIMEOUT_SHIFT;
        }

        *pIsTimedOut = ((postStatMmrRegVal >> shift) & 0x1u) ? CSL_TRUE : CSL_FALSE;
    }

    return status;
}

static int32_t LBIST_isPostLbistDone(uint32_t postStatMmrRegVal,
                                     uint8_t section,
                                     Bool *pIsDone)
{
    int32_t status = CSL_PASS;
    uint32_t shift = 0U;

    if ((pIsDone == NULL) || (section > LBIST_POST_CORE_MAX))
    {
        status = CSL_EBADARGS;
    }
    else
    {
        if (section == LBIST_POST_CORE_DMSC)
        {
            shift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_POST_STAT_POST_DMSC_LBIST_DONE_SHIFT;
        }
        else if (section == LBIST_POST_CORE_MCU)
        {
            shift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_POST_STAT_POST_MCU_LBIST_DONE_SHIFT;
        }

        *pIsDone = ((postStatMmrRegVal >> shift) & 0x1u) ? CSL_TRUE : CSL_FALSE;
    }
    return status;
}

int32_t LBIST_runPostLbistCheck(uint32_t hwPostCoreNum,
                                       CSL_lbistRegs *pLBISTRegs,
                                       uint32_t *pLBISTSig)
{
    int32_t  status = 0;
    uint32_t calculatedMISR = 0U;
    uint32_t expectedMISR = 0U;
    int32_t  postStatus = LBIST_POST_COMPLETED_SUCCESS;
    uint32_t postRegVal;
    Bool     LBISTResult;

    /* Read HW POST status register */
    postRegVal = CSL_REG32_RD(CSL_WKUP_CTRL_MMR0_CFG0_BASE +
                              CSL_WKUP_CTRL_MMR_CFG0_WKUP_POST_STAT);

    /* Check if HW POST LBIST was performed */
    status = LBIST_isPostLbistDone(postRegVal, hwPostCoreNum, &LBISTResult);
    if (status != CSL_PASS)
    {
        UART_printf("   HW POST: LBIST_isPostLbistDone failed\n");
    }
    else
    {
        if (LBISTResult != true)
        {
            /* HW POST: LBIST not completed, check if it timed out */
            status = LBIST_isPostLbistTimeout(postRegVal,
                                              hwPostCoreNum,
                                              &LBISTResult);
            if (LBISTResult != true)
            {
                /* HW POST: LBIST was not performed at all on this device
                 * for this core */
                postStatus = LBIST_POST_NOT_RUN;
                UART_printf("\n   HW POST: LBIST not run on this device\n");
            }
            else
            {
                /* HW POST: LBIST was attempted but timed out for this section */
                postStatus = LBIST_POST_ATTEMPTED_TIMEOUT;
                UART_printf("\n   HW POST: LBIST failed with HW POST timeout\n");
            }
        }
        else
        {
            /* Get the output MISR and the expected MISR */
            if (status == 0)
            {
                status = CSL_LBIST_getMISR(pLBISTRegs, &calculatedMISR);
                if (status != CSL_PASS)
                {
                    UART_printf("\n HW POST: Get MISR failed \n");
                }
            }

            if (status == 0)
            {
                status = CSL_LBIST_getExpectedMISR(pLBISTSig, &expectedMISR);
                if ( status != CSL_PASS)
                {
                    UART_printf("\n HW POST: Get Expected MISR failed \n");
                }
            }

            /* Compare the results, and set failure if they do not match */
#ifdef DEBUG
            UART_printf(" For HW POST LBIST core %d, Expected MISR= 0x%x, Calculated MISR = 0x%x\n",
                        hwPostCoreNum,
                        expectedMISR,
                        calculatedMISR);
#endif
            if (calculatedMISR != expectedMISR)
            {
                /* HW POST: LBIST was completed, but the test failed for this
                 * core */
                postStatus = LBIST_POST_COMPLETED_FAILURE;
                UART_printf("\n   HW POST: LBIST failed with MISR mismatch: Expected 0x%x got 0x%x\n",
                            expectedMISR, calculatedMISR);
            }
            else if ((calculatedMISR == 0) && (expectedMISR == 0))
            {
                postStatus = LBIST_POST_NOT_RUN;
                UART_printf("\n   HW POST: Device does not contain proper seed for MISR in eFuse - "\
                            "HW POST LBIST not supported on this device \n");
            }
            else
            {
                UART_printf("\n   HW POST: LBIST MISR matched \n");
            }
        } /* if (LBISTResult != true) */
    } /* if (status != CSL_PASS) */

    if (status == 0)
    {
        /* All function calls returned successfully */
        return(postStatus);
    }
    else
    {
        /* Error in function calls */
        return(status);
    }
}

LBIST_TestHandle_t* LBIST_getTestHandleArray(void)
{
    LBIST_TestHandle_t *handle;
    uint32_t siliconRev = 0U;

    siliconRev = CSL_REG32_FEXT((CSL_WKUP_CTRL_MMR0_CFG0_BASE +
                                CSL_WKUP_CTRL_MMR_CFG0_JTAGID),
                                WKUP_CTRL_MMR_CFG0_JTAGID_VARIANT);
    if (0U != siliconRev)
    {
        handle = LBIST_TestHandleArray_ES1_1;
    }
    else
    {
        handle = LBIST_TestHandleArray_ES1_0;
    }
    return handle;
}

/* Nothing past this point */
