/*
 * ECC Example
 *
 * Error Correcting Code (ECC) Example Application
 *
 *  Copyright (c) 2020 Texas Instruments Incorporated
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
 *  \file esm.c
 *
 *  \brief ESM utility functions and handler ISR's for ESM and ECC
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
/* For Timer functions */
#include <ti/osal/osal.h>

/* For interrupt ID numbers */
#include <ti/csl/soc/cslr_soc_intr.h>
#include <ti/csl/csl_ecc_aggr.h>

/* Diagnostic example utility functions, e.g. print outputs */
#include <diag_utils.h>
#include <sdr_esm.h>
#include <sdr_ecc.h>

#include "esm.h"
#include "ecc_inject.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define SINGLE_SEC_DED_EVENT_USE_CASES 4

#define MAX_ESM_EVENTS_LOGGED (50)
#define MAX_ESM_INSTANCE      (SDR_ESM_INSTANCE_MAIN)
#define MAX_ESM_INT_TYPE      (SDR_ESM_INT_TYPE_CFG)

#define MAX_MEM_MSMC_SECTIONS (2u)
#define MAX_MEM_R5F_SECTIONS  (9u)

/* ESM event log entry */
typedef struct
{
    /* ECC Aggregator where the event originated from */
    SDR_ECC_MemType eccMemType;
    /* Type of ECC Error Received
     * Possible values are SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE = 1, or
     * SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE = 2 */
    uint32_t             errorSrc;
    /* Address where the ECC error occurred - Not currently populated/used */
    uint32_t             address;
    /* Ram ID for the memory where the error occurred */
    uint32_t             ramId;
    /* Offset into the Ram ID where the error occurred */
    uint64_t             bitErrorOffset;
    /* ECC Error Group where the error occurred - Interconnect RAM ID types only */
    uint32_t             bitErrorGroup;
    /* Use Case number */
    uint8_t              useCaseNum;
} ECC_Example_log_entry_t;

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t ECC_Example_EsmInitHandlerInit(SDR_ESM_InstanceType esmInstType);

static const char *printEccErrorType(SDR_ECC_InjectErrorType eccErrorType);
static int32_t checkUseCaseStructs(uint8_t actualLogSlot, uint8_t expectedLogSlot);
static int32_t checkUseCaseOutputs(uint8_t useCaseNum);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile uint32_t gEsmEventRcvdFlag = ESM_EVENT_NOT_RCVD;

static volatile ECC_Example_log_entry_t eccEventLog[MAX_ESM_EVENTS_LOGGED];
static volatile uint32_t totalEccEventsLogged              = 0;
static volatile uint32_t totalEccEventsPerUseCase[6]       = {0, 0, 0, 0, 0, 0};
static volatile uint32_t totalExpectedEccEvtsPerUseCase[6] = {2, 2, 2, 2, 20, 20};


static ECC_Example_log_entry_t expectedEccEventLog[MAX_ESM_EVENTS_LOGGED] =
{
    {SDR_ECC_MEMTYPE_MCU_R5F0_CORE,                   /* eccMemType */
     CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,                /* errorSrc */
     SDR_ESM_ERRORADDR_INVALID,                       /* address */
     SDR_ECC_R5F_MEM_SUBTYPE_VBUSM2AXI_EDC_VECTOR_ID, /* ramId */
     0x0000000000000004,                              /* bitErrorOffset */
     21,                                              /* bitErrorGroup */
     0                                                /* useCaseNum */
    },
    {SDR_ECC_MEMTYPE_MCU_R5F0_CORE,                   /* eccMemType */
     CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT,                /* errorSrc */
     SDR_ESM_ERRORADDR_INVALID,                       /* address */
     SDR_ECC_R5F_MEM_SUBTYPE_VBUSM2AXI_EDC_VECTOR_ID, /* ramId */
     0x0000000000000000,                              /* bitErrorOffset */
     21,                                              /* bitErrorGroup */
     0                                                /* useCaseNum */
    },
    {SDR_ECC_MEMTYPE_MCU_R5F0_CORE,                   /* eccMemType */
     CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,                /* errorSrc */
     SDR_ESM_ERRORADDR_INVALID,                       /* address */
     SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,    /* ramId */
     0x0000000000000024,                              /* bitErrorOffset */
     0,                                               /* bitErrorGroup */
     1                                                /* useCaseNum */
    },
    {SDR_ECC_MEMTYPE_MCU_R5F0_CORE,                   /* eccMemType */
     CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT,                /* errorSrc */
     SDR_ESM_ERRORADDR_INVALID,                       /* address */
     SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,    /* ramId */
     0x0000000000000020,                              /* bitErrorOffset */
     0,                                               /* bitErrorGroup */
     1                                                /* useCaseNum */
    },
    {SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,                 /* eccMemType */
     CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,                /* errorSrc */
     SDR_ESM_ERRORADDR_INVALID,                       /* address */
     SDR_ECC_MAIN_MSMC_MEM_INTERCONN_SUBTYPE,         /* ramId */
     0x0000000000000004,                              /* bitErrorOffset */
     0,                                               /* bitErrorGroup */
     2                                                /* useCaseNum */
    },
    {SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,                 /* eccMemType */
     CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT,                /* errorSrc */
     SDR_ESM_ERRORADDR_INVALID,                       /* address */
     SDR_ECC_MAIN_MSMC_MEM_INTERCONN_SUBTYPE,         /* ramId */
     0x0000000000000000,                              /* bitErrorOffset */
     0,                                               /* bitErrorGroup */
     2                                                /* useCaseNum */
    },
    {SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,                 /* eccMemType */
     CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,                /* errorSrc */
     SDR_ESM_ERRORADDR_INVALID,                       /* address */
     SDR_ECC_MAIN_MSMC_MEM_WRAPPER_SUBTYPE,           /* ramId */
     0x000000000000010c,                              /* bitErrorOffset - Note that
                                                       * this value can vary, so it is
                                                       * not checked */
     0,                                               /* bitErrorGroup */
     3                                                /* useCaseNum */
    },
    {SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,                 /* eccMemType */
     CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT,                /* errorSrc */
     SDR_ESM_ERRORADDR_INVALID,                       /* address */
     SDR_ECC_MAIN_MSMC_MEM_WRAPPER_SUBTYPE,           /* ramId */
     0x0000000000000108,                              /* bitErrorOffset - Note that
                                                       * this value can vary, so it is
                                                       * not checked */
     0,                                               /* bitErrorGroup */
     3                                                /* useCaseNum */
    },
    /* Multiple SEC events consecutively - the following entry should occur
     * NUM_MULTIPLE_SEC_EVENTS times */
    {SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,                 /* eccMemType */
     CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,                /* errorSrc */
     SDR_ESM_ERRORADDR_INVALID,                       /* address */
     SDR_ECC_MAIN_MSMC_MEM_INTERCONN_SUBTYPE,         /* ramId */
     0x0000000000000004,                              /* bitErrorOffset */
     0,                                               /* bitErrorGroup */
     4                                                /* useCaseNum */
    },
    /* Multiple DED events consecutively - the following entry should occur
     * NUM_MULTIPLE_DED_EVENTS times */
    {SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,                 /* eccMemType */
     CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT,                /* errorSrc */
     SDR_ESM_ERRORADDR_INVALID,                       /* address */
     SDR_ECC_MAIN_MSMC_MEM_INTERCONN_SUBTYPE,         /* ramId */
     0x0000000000000000,                              /* bitErrorOffset */
     0,                                               /* bitErrorGroup */
     4                                                /* useCaseNum */
    },
    /* Multiple SEC events consecutively - the following entry should occur
     * NUM_MULTIPLE_SEC_EVENTS times */
    {SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,                 /* eccMemType */
     CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,                /* errorSrc */
     SDR_ESM_ERRORADDR_INVALID,                       /* address */
     SDR_ECC_MAIN_MSMC_MEM_WRAPPER_SUBTYPE,           /* ramId */
     0x000000000000010c,                              /* bitErrorOffset - Note that
                                                       * this value can vary, so it is
                                                       * not checked */
     0,                                               /* bitErrorGroup */
     5                                                /* useCaseNum */
    },
    /* Multiple DED events consecutively - the following entry should occur
     * NUM_MULTIPLE_DED_EVENTS times */
    {SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,                 /* eccMemType */
     CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT,                /* errorSrc */
     SDR_ESM_ERRORADDR_INVALID,                       /* address */
     SDR_ECC_MAIN_MSMC_MEM_WRAPPER_SUBTYPE,           /* ramId */
     0x0000000000000108,                              /* bitErrorOffset - Note that
                                                       * this value can vary, so it is
                                                       * not checked */
     0,                                               /* bitErrorGroup */
     5                                                /* useCaseNum */
    },
};

/* Initialization structure for MCU ESM instance */
static SDR_ESM_InitConfig_t ECC_Example_esmInitConfig_MCU =
{
    .esmErrorConfig = {0u, 3u}, /* Self test error config */
    .eventMap = {0xffffffffu, 0x033fffffu, 0x7fffffffu, 0xffffffe7u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                },
     /**< All events enable: except timer and self test  events, */
    /*    and Main ESM output.  Disabling vim compare error as well.
     *    Exclude MCU Timer 0, which is left on by SBL in continuous mode.
     *    Ensure inclusion of the following ESM events that will be used in the
     *    example application:
     *    MCU_R5FSS0_CORE0_ECC_CORRECTED_LEVEL_0 interrupt to MCU_ESM0_LVL_IN_32
     *    MCU_R5FSS0_CORE0_ECC_UNCORRECTED_LEVEL_0 interrupt to MCU_ESM0_LVL_IN_33
     */
    .eventPriorityMap = {0xffffffffu, 0x033ffffeu, 0x7fffffffu, 0xffffffe7u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                        },
    /**< All events high priority: except timer, selftest error events,
     *   and Main ESM output. Exclude MCU Timer 0, which is left on by
     *   SBL in continuous mode.
     *   MCU_R5FSS0_CORE0_ECC_CORRECTED_LEVEL_0 interrupt to MCU_ESM0_LVL_IN_32 set
     *   to Low Priority.
     *   MCU_R5FSS0_CORE0_ECC_UNCORRECTED_LEVEL_0 interrupt to MCU_ESM0_LVL_IN_33 set
     *   to High Priority.
     */
    .errorOutputMap = {0xffffffffu, 0x033ffffeu, 0x7fffffffu, 0xffffffe7u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                      },
    /**< All events high priority: except timer, selftest error events,
     *   and Main ESM output. Exclude MCU Timer 0, which is left on by
     *   SBL in continuous mode, and MCU Timer 1, which is used for controlling
     *   the Output pin clearing.
     *   MCU_R5FSS0_CORE0_ECC_CORRECTED_LEVEL_0 interrupt to MCU_ESM0_LVL_IN_32 set
     *   to Low Priority with no Output pin routing.
     *   MCU_R5FSS0_CORE0_ECC_UNCORRECTED_LEVEL_0 interrupt to MCU_ESM0_LVL_IN_33 set
     *   to High Priority with Output pin routing.
     */
};

/* Initialization structure for Main ESM instance */
static SDR_ESM_InitConfig_t ECC_Example_esmInitConfig_MAIN =
{
    .esmErrorConfig = {1u, 8u}, /* Self test error config */
    .eventMap = {0x00000000u, 0xfffffffbu, 0x7fffffffu, 0xffffffffu,
                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0xffffffffu, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                },
     /**< All events enable: except clkstop events for unused clocks
      *   and PCIE events.
      *   This example enables the following Main ESM input events that are
      *   enabled here:
      *   COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_0 (MSMC Uncorrected DED ECC error)
      *   interrupt to ESM0_LVL_IN_48.
      *   COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_1 (MSMC Corrected SEC ECC error)
      *   interrupt to ESM0_LVL_IN_49.
      *   COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_7 (A72 Uncorrected DED ECC error)
      *   interrupt to ESM0_LVL_IN_55.
      *   COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_8 (A72 Corrected SEC ECC error)
      *   interrupt to ESM0_LVL_IN_56.
      */
    .eventPriorityMap = {0x00000000u, 0xffedfffbu, 0x7fffffffu, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0xffffffffu, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                        },
    /**< All events high priority: except clkstop events for unused clocks
     *   and PCIE events.
     *   This example sets the following Main ESM input events as high
     *   priority events:
     *   COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_0 (MSMC Uncorrected DED ECC error)
     *   interrupt to ESM0_LVL_IN_48.
     *   COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_7 (A72 Uncorrected DED ECC error)
     *   interrupt to ESM0_LVL_IN_55.
     *   This example sets the following Main ESM input events as low
     *   priority events:
     *   COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_1 (MSMC Corrected SEC ECC error)
     *   interrupt to ESM0_LVL_IN_49.
     *   COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_8 (A72 Corrected SEC ECC error)
     *   interrupt to ESM0_LVL_IN_56.
     */
    .errorOutputMap = {0x00000000u, 0xffedfffbu, 0x7fffffffu, 0xffffffffu,
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0xffffffffu, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                      },
    /**< All events high priority: except clkstop for unused clocks
     *   and PCIE events
     *   This example sets the following Main ESM input events as high
     *   priority events with Output Pin routing:
     *   COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_0 (MSMC Uncorrected DED ECC error)
     *   interrupt to ESM0_LVL_IN_48.
     *   COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_7 (A72 Uncorrected DED ECC error)
     *   interrupt to ESM0_LVL_IN_55.
     *   This example sets the following Main ESM input events as low
     *   priority events with no Output Pin routing:
     *   COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_1 (MSMC Corrected SEC ECC error)
     *   interrupt to ESM0_LVL_IN_49.
     *   COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_8 (A72 Corrected SEC ECC error)
     *   interrupt to ESM0_LVL_IN_56.
     *   */
};

static SDR_ECC_MemSubType ECC_Example_R5FCoresubMemTypeList[MAX_MEM_R5F_SECTIONS] =
{
    SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK1_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_VBUSM2AXI_EDC_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_AXI2VBUSM_MEM_MST_WRITE_EDC_VECTOR_ID
};

static SDR_ECC_InitConfig_t ECC_Example_R5FCoreECCInitConfig =
{
    .numRams = MAX_MEM_R5F_SECTIONS,
    /**< Number of Rams ECC is enabled  */
    .pMemSubTypeList = &(ECC_Example_R5FCoresubMemTypeList[0]),
    /**< Sub type list  */
};


static SDR_ECC_MemSubType ECC_Example_MSMCsubMemTypeList[MAX_MEM_MSMC_SECTIONS] =
{
    SDR_ECC_MAIN_MSMC_MEM_INTERCONN_SUBTYPE,
    SDR_ECC_MAIN_MSMC_MEM_WRAPPER_SUBTYPE
};

static SDR_ECC_InitConfig_t ECC_Example_MSMCMemECCInitConfig =
{
    .numRams = MAX_MEM_MSMC_SECTIONS,
    /**< Number of Rams ECC is enabled  */
    .pMemSubTypeList = &(ECC_Example_MSMCsubMemTypeList[0]),
    /**< Sub type list  */
};

static HwiP_Handle ECC_Example_EsmHiHwiPHandle;
static HwiP_Handle ECC_Example_EsmLoHwiPHandle;
static HwiP_Handle ECC_Example_EsmCfgHwiPHandle;

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* Completion of Use Case from ESM Handler perspective updates these flags */
extern volatile uint32_t eccOutputResult[6];
/* Current use case being run */
extern volatile uint8_t  currUseCase;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void SDR_ECC_applicationCallbackFunction(SDR_ECC_MemType eccMemType,
                                         uint32_t errorSrc,
                                         uint32_t address,
                                         uint32_t ramId,
                                         uint64_t bitErrorOffset,
                                         uint32_t bitErrorGroup){

    int32_t useCaseCheck;

#ifdef PRINT_EVENTS
    DIAG_printf("\n  ECC Error Call back function called : eccMemType %d, errorSrc 0x%x, " \
                "address 0x%x, ramId %d, bitErrorOffset 0x%08x%08x, bitErrorGroup %d\n",
                eccMemType, errorSrc, address, ramId, (uint32_t)(bitErrorOffset >> 32),
                (uint32_t)(bitErrorOffset & 0x00000000FFFFFFFF), bitErrorGroup);
    DIAG_printf("  Take action \n");
#endif

    /* Log the event */
    eccEventLog[totalEccEventsLogged].useCaseNum     = currUseCase;
    eccEventLog[totalEccEventsLogged].eccMemType     = eccMemType;
    eccEventLog[totalEccEventsLogged].errorSrc       = errorSrc;
    eccEventLog[totalEccEventsLogged].address        = address;
    eccEventLog[totalEccEventsLogged].ramId          = ramId;
    eccEventLog[totalEccEventsLogged].bitErrorOffset = bitErrorOffset;
    eccEventLog[totalEccEventsLogged].bitErrorGroup  = bitErrorGroup;

    gEsmEventRcvdFlag = ESM_EVENT_RCVD;

    totalEccEventsLogged++;
    totalEccEventsPerUseCase[currUseCase]++;

    if (totalEccEventsPerUseCase[currUseCase] ==
        totalExpectedEccEvtsPerUseCase[currUseCase])
    {
        /* Check all the events from this use case*/
        useCaseCheck = checkUseCaseOutputs(currUseCase);
        if (useCaseCheck == 0) {
            eccOutputResult[currUseCase] = USE_CASE_STATUS_COMPLETED_SUCCESS;
        } else {
            eccOutputResult[currUseCase] = USE_CASE_STATUS_COMPLETED_FAILURE;
        }
    }
    /* Any additional customer specific actions can be added here */

}

/*********************************************************************
* @fn      ECC_Example_esmInit
*
* @brief   Initializes ESM instances and ECC instances
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t ECC_Example_esmInit (void)
{
    int32_t retValue=0;
    SDR_Result result;

#ifdef UART_PRINTF
    /* Initialize UART instance */
    DIAG_uartStdioInit();
#endif

    DIAG_printf("\nECC_Example_esmInit: UART ready to print, proceeding with ESM init \n");

    if (retValue == 0) {
        /* Initialize MCU ESM module */
        result = SDR_ESM_init(SDR_ESM_INSTANCE_MCU, &ECC_Example_esmInitConfig_MCU);
        if (result != SDR_PASS) {
            /* print error and quit */
            DIAG_printf("ECC_Example_esmInit: Error initializing MCU ESM: result = %d\n",
                        result);

            retValue = -1;
        } else {
            DIAG_printf("\nECC_Example_esmInit: Init MCU ESM complete \n");
        }
    }

    if (retValue == 0) {
        /* Initialize MAIN ESM module */
        result = SDR_ESM_init(SDR_ESM_INSTANCE_MAIN, &ECC_Example_esmInitConfig_MAIN);
        if (result != SDR_PASS) {
            /* print error and quit */
            DIAG_printf("ECC_Example_esmInit: Error initializing MAIN ESM: result = %d\n",
                        result);

            retValue = -1;
        } else {
            DIAG_printf("\nECC_Example_esmInit: Init MAIN ESM complete \n");
        }
    }

    if (retValue == 0) {
        /* Initialize ECC callbacks within the MCU ESM */
        result = SDR_ECC_initEsm(SDR_ESM_INSTANCE_MCU);
        if (result != SDR_PASS) {
            /* print error and quit */
             DIAG_printf("ECC_Example_esmInit: Error initializing ECC callback for " \
                         "MCU ESM: result = %d\n",
                         result);

            retValue = -1;
        } else {
            DIAG_printf("\nECC_Example_esmInit: ECC Callback Init complete for MCU ESM \n");
        }
    }

    /* Initialize all ECC aggregators that desire monitoring that report events to the
     * MCU ESM instance.
     * This example only shows MCU R5F ECC aggregator instance.  If additional
     * ECC aggregators are needed, they should also be initialized here. */
    if (retValue == 0) {
        /* Initialize ECC */
        result = SDR_ECC_init(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                              &ECC_Example_R5FCoreECCInitConfig);
        if (result != SDR_PASS) {
            /* print error and quit */
             DIAG_printf("ECC_Example_esmInit: Error initializing R5F core " \
                         "ECC: result = %d\n",
                         result);

            retValue = -1;
        } else {
            DIAG_printf("\nECC_Example_esmInit: R5F Core Init ECC complete \n");
        }
    }



    if (retValue == 0) {
        /* Initialize ECC callbacks within the Main ESM */
        result = SDR_ECC_initEsm(SDR_ESM_INSTANCE_MAIN);
        if (result != SDR_PASS) {
            /* print error and quit */
             DIAG_printf("ECC_Example_esmInit: Error initializing ECC callback for " \
                         "Main ESM: result = %d\n",
                         result);

            retValue = -1;
        } else {
            DIAG_printf("\nECC_Example_esmInit: ECC Callback Init complete for Main ESM \n");
        }
    }

    if (retValue == 0) {
        /* Initialize VIM ECC memory ; This is specifically for VIM in lockstep*/
        /* All other memories are auto initialized by hardware */
        result = SDR_ECC_initMemory(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                    SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID);
        if (result != SDR_PASS) {
             /* print error and quit */
             DIAG_printf("ECC_Example_esmInit: Error initializing ECC memory: result = %d\n",
                         result);
             retValue = -1;;
         } else {
             DIAG_printf("\nECC_Example_esmInit: ECC Memory init complete \n");
         }
    }

    /* Initialize all ECC aggregators that desire monitoring that report events to the
     * Main ESM instance.
     * This example only shows MSMC_AGGR0 instance.  If additional ECC aggregators are needed,
     * they should also be initialized here. */
    if (retValue == 0) {
        /* Initialize ECC */
        result = SDR_ECC_init(SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,
                              &ECC_Example_MSMCMemECCInitConfig);
        if (result != SDR_PASS) {
            /* print error and quit */
             DIAG_printf("ECC_Example_esmInit: Error initializing MSMC ECC: result = %d\n",
                         result);

            retValue = -1;
        } else {
            DIAG_printf("\nECC_Example_esmInit: MSMC Init ECC complete \n");
        }
    }

    if (retValue == 0) {
        /* Register MCU Esm handlers */
        retValue = ECC_Example_EsmInitHandlerInit(SDR_ESM_INSTANCE_MCU);
        if (retValue != 0) {
            DIAG_printf("\nECC_Example_esmInit: Init MCU ESM handlers failed \n");
        } else {
            DIAG_printf("\nECC_Example_esmInit: Init MCU ESM handlers complete \n");
        }

        /* Register Main Esm handlers */
        retValue = ECC_Example_EsmInitHandlerInit(SDR_ESM_INSTANCE_MAIN);
        if (retValue != 0) {
            DIAG_printf("\nECC_Example_esmInit: Init MAIN ESM handlers failed \n");
        } else {
            DIAG_printf("\nECC_Example_esmInit: Init MAIN ESM handlers complete \n");
        }
    }

    return retValue;
}

/*********************************************************************
* @fn      ECC_Example_EsmInitHandlerInit
*
* @brief   Register all callbacks for an ESM instance
*
* @param   esmInstType: Instance type of ESM 
*
* @return    0 : Success; < 0 for failures
*/
static int32_t ECC_Example_EsmInitHandlerInit(SDR_ESM_InstanceType esmInstType)
{
    int32_t result = 0;
    uint32_t intNumHi, intNumLo, intNumCfg;
    HwiP_Params       hwiParams;
    void (*pHiInterruptHandler)(uintptr_t);
    void (*pLoInterruptHandler)(uintptr_t);
    void (*pConfigInterruptHandler)(uintptr_t);

    switch (esmInstType) {
        case (SDR_ESM_INSTANCE_WKUP):
            pHiInterruptHandler = &SDR_ESM_hiInterruptHandler_WKUP;
            pLoInterruptHandler = &SDR_ESM_loInterruptHandler_WKUP;
            pConfigInterruptHandler = &SDR_ESM_configInterruptHandler_WKUP;
            break;

        case (SDR_ESM_INSTANCE_MAIN):
            pHiInterruptHandler = &SDR_ESM_hiInterruptHandler_MAIN;
            pLoInterruptHandler = &SDR_ESM_loInterruptHandler_MAIN;
            pConfigInterruptHandler = &SDR_ESM_configInterruptHandler_MAIN;
            break;

        case (SDR_ESM_INSTANCE_MCU):
        default:
            pHiInterruptHandler = &SDR_ESM_hiInterruptHandler_MCU;
            pLoInterruptHandler = &SDR_ESM_loInterruptHandler_MCU;
            pConfigInterruptHandler = &SDR_ESM_configInterruptHandler_MCU;
            break;
    }

    /* Get the interrupt number corresponding to ESM HI interrupt */
    intNumHi = SDR_ESM_getIntNumber(esmInstType,
                                    SDR_ESM_INT_TYPE_HI);
    if ( intNumHi == SDR_ESM_INTNUMBER_INVALID ) {
        result = -1;
    } else {
        /* Get the interrupt number corresponding to ESM LO interrupt */
        intNumLo = SDR_ESM_getIntNumber(esmInstType,
                                        SDR_ESM_INT_TYPE_LO);
        if ( intNumLo == SDR_ESM_INTNUMBER_INVALID ) {
            result = -1;
        } else {
            /* Get the interrupt number corresponding to ESM Config interrupt */
            intNumCfg = SDR_ESM_getIntNumber(esmInstType,
                                             SDR_ESM_INT_TYPE_CFG);
            if ( intNumCfg == SDR_ESM_INTNUMBER_INVALID ) {
                result = -1;
            } else {
                DIAG_printf("\n ECC_Example_EsmInitHandlerInit: " \
                            "Obtain interrupt number complete \n");
                /* Clear all interrupts first */
                HwiP_disableInterrupt(intNumHi);
                HwiP_disableInterrupt(intNumLo);
                HwiP_disableInterrupt(intNumCfg);

                DIAG_printf("\n ECC_Example_EsmInitHandlerInit: " \
                            "Disable interrupts complete \n");

                /* Default parameter initialization */
                HwiP_Params_init(&hwiParams);

                hwiParams.arg = intNumHi;
                DIAG_printf("\n ECC_Example_EsmInitHandlerInit: " \
                            "HwiP_Params_init complete \n");

                /* Register call back function for ESM Hi Interrupt */
                ECC_Example_EsmHiHwiPHandle = HwiP_create(intNumHi,
                                                          (HwiP_Fxn)(*pHiInterruptHandler),
                                                          (void *)&hwiParams);
                DIAG_printf("\n ECC_Example_EsmInitHandlerInit: " \
                            "intNumHi registration complete \n");
                if (ECC_Example_EsmHiHwiPHandle == (HwiP_Handle) NULL) {
                    result = -1;
                } else {
                    hwiParams.arg = intNumLo;
                    /* Register call back function for ESM Lo Interrupt */
                    ECC_Example_EsmLoHwiPHandle = HwiP_create(intNumLo,
                                                              (HwiP_Fxn)(*pLoInterruptHandler),
                                                              (void *)&hwiParams);
                    DIAG_printf("\n ECC_Example_EsmInitHandlerInit: " \
                                "intNumLo registration complete \n");
                    if (ECC_Example_EsmLoHwiPHandle == (HwiP_Handle) NULL) {
                        result = -1;
                    } else {
                        hwiParams.arg = intNumCfg;
                        /* Register call back function for ESM Config Interrupt */
                        ECC_Example_EsmCfgHwiPHandle = HwiP_create(intNumCfg,
                                                   (HwiP_Fxn)(*pConfigInterruptHandler),
                                                   (void *)&hwiParams);
                        DIAG_printf("\n ECC_Example_EsmInitHandlerInit: " \
                                    "intNumCfg registration complete \n");
                        if (ECC_Example_EsmCfgHwiPHandle == (HwiP_Handle) NULL) {
                            result = -1;
                        } else {
                            /* Enable all ESM Interrupts */
                            HwiP_enableInterrupt(intNumHi);
                            HwiP_enableInterrupt(intNumLo);
                            HwiP_enableInterrupt(intNumCfg);
                       }
                    }
                }
            }
        }
    }

    return result;
}

#ifdef PRINT_EVENTS
static const char *printEsmIntType(SDR_ESM_IntType esmIntType)
{
    char *pStr;

    switch(esmIntType)
    {
        case SDR_ESM_INT_TYPE_HI:
            pStr = "High Priority ESM event";
            break;
        case SDR_ESM_INT_TYPE_LO:
            pStr = "Low Priority ESM event";
            break;
        case SDR_ESM_INT_TYPE_CFG:
            pStr = "Config ESM event";
            break;
        default:
            pStr = NULL;
            break;
    }

    return pStr;
}
#endif

static const char *printEccErrorType(SDR_ECC_InjectErrorType eccErrorType)
{
    char *pStr;

    switch(eccErrorType)
    {
        case SDR_INJECT_ECC_NO_ERROR:
            pStr = "No ECC Error";
            break;
        case SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE:
            pStr = "1-Bit ECC Error";
            break;
        case SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE:
            pStr = "2-Bit ECC Error";
            break;
        default:
            pStr = NULL;
            break;
    }

    return pStr;

}

static const char *printEccAggregator(SDR_ECC_MemType eccMemType)
{
    char *pStr;

    switch(eccMemType)
    {
        case SDR_ECC_MEMTYPE_MCU_R5F0_CORE:
            pStr = "R5FSS0_CORE0_ECC_AGGR ECC Aggregator";
            break;
        case SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0:
            pStr = "COMPUTE_CLUSTER0_MSMC_ECC_AGGR0 ECC Aggregator";
            break;
        default:
            pStr = NULL;
            break;
    }

    return pStr;

}

/*********************************************************************
* @fn      ECC_Example_printSummary
*
* @brief   Print summary of all the example use cases run
*
* @param   None
*
* @return  None
*/
void ECC_Example_printSummary(void)
{
    int32_t i;

    DIAG_printf("\n\n");
    DIAG_printf("ECC Example Application summary\n");
    DIAG_printf("-------------------------------\n");
    DIAG_printf("Completed %d Use Cases\n", currUseCase);

    DIAG_printf("Received Total of %d ECC Events\n", totalEccEventsLogged);

    DIAG_printf("\nUse Case Event Log\n");
    DIAG_printf("------------------\n");
    for (i = 0; i < totalEccEventsLogged; i++) {
        DIAG_printf("\nUse Case %d: ECC Call back function called : \n"
                    "    ECC Aggregator = %s, \n" \
                    "    ECC Error Type = %s, address 0x%x, \n" \
                    "    ramId %d, bitErrorOffset 0x%08x%08x, bitErrorGroup %d\n",
                    eccEventLog[i].useCaseNum,
                    printEccAggregator(eccEventLog[i].eccMemType),
                    printEccErrorType((SDR_ECC_InjectErrorType)eccEventLog[i].errorSrc),
                    eccEventLog[i].address,
                    eccEventLog[i].ramId,
                    (uint32_t)(eccEventLog[i].bitErrorOffset >> 32),
                    (uint32_t)(eccEventLog[i].bitErrorOffset & 0x00000000FFFFFFFF),
                    eccEventLog[i].bitErrorGroup);
    }
    for (i = 0; i < 6; i++) {
        DIAG_printf("\nUse Case %d received %d ECC events\n", i, totalEccEventsPerUseCase[i]);
    }
}

static int32_t checkUseCaseStructs(uint8_t actualLogSlot, uint8_t expectedLogSlot)
{
    int32_t retValue = 0;
    char *pStrErrType;
    uint8_t useCaseNum = eccEventLog[actualLogSlot].useCaseNum;

    if (expectedEccEventLog[expectedLogSlot].errorSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT)
    {
        pStrErrType = "SEC";
    } else if (expectedEccEventLog[expectedLogSlot].errorSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT)
    {
        pStrErrType = "DED";
    }

    /* Check event for proper information */
    if (expectedEccEventLog[expectedLogSlot].eccMemType !=
        eccEventLog[actualLogSlot].eccMemType) {
        DIAG_printf("Error - Incorrect eccMemType for %s event in " \
                    "Use Case %d\n", pStrErrType, useCaseNum);
        retValue = -1;
    }
    if (expectedEccEventLog[expectedLogSlot].errorSrc !=
        eccEventLog[actualLogSlot].errorSrc) {
        DIAG_printf("Error - Incorrect errorSrc for %s event in " \
                    "Use Case %d\n", pStrErrType, useCaseNum);
        retValue = -1;
    }
    if (expectedEccEventLog[expectedLogSlot].address !=
        eccEventLog[actualLogSlot].address) {
        DIAG_printf("Error - Incorrect address for %s event in " \
                    "Use Case %d\n", pStrErrType, useCaseNum);
        retValue = -1;
    }
    if (expectedEccEventLog[expectedLogSlot].ramId !=
        eccEventLog[actualLogSlot].ramId) {
        DIAG_printf("Error - Incorrect ramId for %s event in " \
                    "Use Case %d\n", pStrErrType, useCaseNum);
        retValue = -1;
    }
    /* bitErrorOffset is not checked in case of MSMC Wrapper Use Cases,
     * since exact location of the error depends on triggering of CLEC
     * events (and not a direct access to a memory location */
    if ((useCaseNum != 3) && (useCaseNum != 5))
    {
        if (expectedEccEventLog[expectedLogSlot].bitErrorOffset !=
            eccEventLog[actualLogSlot].bitErrorOffset) {
            DIAG_printf("Error - Incorrect bitErrorOffset for %s event in " \
                        "Use Case %d\n", pStrErrType, useCaseNum);
            retValue = -1;
        }
    }
    if (expectedEccEventLog[expectedLogSlot].bitErrorGroup !=
        eccEventLog[actualLogSlot].bitErrorGroup) {
        DIAG_printf("Error - Incorrect bitErrorGroup for %s event in " \
                    "Use Case %d\n", pStrErrType, useCaseNum);
        retValue = -1;
    }

    return retValue;

}


static int32_t checkUseCaseOutputs(uint8_t useCaseNum)
{
    uint8_t i;
    int32_t status = 0;
    int32_t retValue;
    uint8_t useCaseLogStartSlot;

#ifdef DEBUG_PRINT
    DIAG_printf("Checking values for Use Case %d\n", useCaseNum);
#endif
    if (useCaseNum == 5) {
        useCaseLogStartSlot = SINGLE_SEC_DED_EVENT_USE_CASES * 2 +
                              NUM_MULTIPLE_SEC_EVENTS + NUM_MULTIPLE_DED_EVENTS;
    } else if (useCaseNum == 4) {
        useCaseLogStartSlot = SINGLE_SEC_DED_EVENT_USE_CASES * 2;
    } else {
        useCaseLogStartSlot = useCaseNum * 2;
    }

    if ((useCaseNum == 4) || (useCaseNum == 5)) {
        /* Check all NUM_MULTIPLE_SEC_EVENTS for proper information */
        for (i = 0; i < NUM_MULTIPLE_SEC_EVENTS; i++) {
            retValue = checkUseCaseStructs(useCaseLogStartSlot + i, 2 * useCaseNum);
            if (retValue != 0) {
                status = -1;
            }
        }
        /* Check all NUM_MULTIPLE_DED_EVENTS for proper information */
        for (i = 0; i < NUM_MULTIPLE_DED_EVENTS; i++) {
            retValue = checkUseCaseStructs(useCaseLogStartSlot + i + NUM_MULTIPLE_SEC_EVENTS,
                                           2 * useCaseNum + 1);
            if (retValue != 0) {
                status = -1;
            }
        }
    } else {
        /* Use Case only has single SEC event and single DED event */
        /* Check SEC event */
        retValue = checkUseCaseStructs(2 * useCaseNum, 2 * useCaseNum);
        if (retValue != 0) {
            status = -1;
        }
        /* Check DED event */
        retValue = checkUseCaseStructs(2 * useCaseNum + 1, 2 * useCaseNum + 1);
        if (retValue != 0) {
            status = -1;
        }
    }

    return status;
}


void SDR_ESM_applicationCallbackFunction(SDR_ESM_InstanceType esmInstType,
                                         SDR_ESM_IntType esmIntType,
                                         uint32_t grpChannel,
                                         uint32_t index,
                                         uint32_t intSrc)
{
    /* All ECC events should be received via the ECC application callback */
    /* This function just needs to be defined for proper linking with the SDR
     * binary which expects the application to define this function */
    /* Any additional customer-specific actions to address other ESM events
     * separate from ECC events can be added here */
#ifdef PRINT_EVENTS
    /* Print information to screen */
    DIAG_printf("\n  ESM Call back function called : instType 0x%x, intType 0x%x, " \
                "grpChannel 0x%x, index 0x%x, intSrc 0x%x \n",
                esmInstType, esmIntType, grpChannel, index, intSrc);
    DIAG_printf("  Take action \n");

    DIAG_printf("  ESM instance #%d, ESM interrupt type = %s\n",
                esmInstType, printEsmIntType(esmIntType));
#endif

    return;
}

void SDR_assertExternalFunction(SDR_assertErrorNumber errorNumber)
{

    DIAG_printf("\n  Assert called with error Number %d \n",
                errorNumber);
    /* Asserts are fatal: Currently just wait for ever */
    while(1);
}
