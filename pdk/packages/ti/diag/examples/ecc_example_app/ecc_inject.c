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
 *
 */
/**
 *  \file ecc_inject.c
 *
 *  \brief This file contains functions that provide input ECC event triggers
 *         for the Error Correcting Code Module (ECC) application.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <diag_utils.h>

#include <ti/csl/csl_ecc_aggr.h>
#include <ti/csl/soc/cslr_soc_ecc_aggr.h>
#include <sdr_ecc.h>

#include "ecc_trigger_func.h"
#include "ecc_inject.h"


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* Use case definition structure */
typedef struct diag_ecc_use_case_s
{
    SDR_ECC_MemType         eccAggrId;     /* ECC Aggregator ID */
    SDR_ECC_MemSubType      eccRamId;      /* ECC RAM ID */
    SDR_ECC_InjectErrorType secInjErrType; /* SEC Inject Error Type */
    uint32_t                secFlipMask;   /* SEC Error injection bitmask */
    SDR_ECC_InjectErrorType dedInjErrType; /* DED Inject Error Type */
    uint32_t                dedFlipMask;   /* DED Error injection bitmask */
    SDR_ECC_RamIdType       ramIdType;     /* ECC RAM ID Type - Wrapper or Interconnect */
    uint32_t                chkGrp;        /* ECC Checker Group - Only for Interconnect Type */
    uint32_t                testAddressTrigLoc;    /* Memory location to read for event trigger.
                                                      For accessible Wrapper types only. */
    int32_t                 (*testTrigFunc)(void); /* Function to run for event trigger.
                                                      For inaccessible Wrapper types only. */
} diag_ecc_use_case_t;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t runSingleSECEvent(SDR_ECC_MemType eccAggrId,
                                 SDR_ECC_MemSubType eccRamId,
                                 SDR_ECC_InjectErrorType injErrType,
                                 uint32_t flipMask,
                                 uint32_t ramIdType,
                                 uint32_t chkGrp,
                                 uint32_t testAddressTrigLoc,
                                 int32_t (*testTrigFunc)(void) );

static int32_t runSingleDEDEvent(SDR_ECC_MemType eccAggrId,
                                 SDR_ECC_MemSubType eccRamId,
                                 SDR_ECC_InjectErrorType injErrType,
                                 uint32_t flipMask,
                                 uint32_t ramIdType,
                                 uint32_t chkGrp,
                                 uint32_t testAddressTrigLoc,
                                 int32_t (*testTrigFunc)(void) );

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */


/* List all needed values/ID's for all the use cases */
diag_ecc_use_case_t useCaseArray[6] =
{
    /* MCU ESM, MCU_R5FSS0_0 ECC Aggregator, Interconnect ECC Type */
    {SDR_ECC_MEMTYPE_MCU_R5F0_CORE,                   /* eccAggrId of type SDR_ECC_MemType */
     SDR_ECC_R5F_MEM_SUBTYPE_VBUSM2AXI_EDC_VECTOR_ID, /* eccRamId of type SDR_ECC_MemSubType - CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_RAM_ID */
     SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,          /* SEC Inject Error Type */
     0x10,                                            /* secFlipMask of type uint32_t */
     SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,          /* DED Inject Error Type */
     0x101,                                           /* dedFlipMask of type uint32_t */
     SDR_ECC_RAM_ID_TYPE_INTERCONNECT,                /* ramIdType of type SDR_ECC_RamIdType */
     CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_21_ID, /* EDC Group Type */
     0x0,                                             /* testAddressTrigLoc of type uint32_t,
                                                       * not used for Interconnect Type */
     NULL                                             /* testTrigFunc filled in initialization function */
    },
    /* MCU ESM, MCU_R5FSS0_0 ECC Aggregator, Wrapper ECC Type */
    {SDR_ECC_MEMTYPE_MCU_R5F0_CORE,                    /* eccAggrId of type SDR_ECC_MemType */
     SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,     /* eccRamId of type SDR_ECC_MemSubType - CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_RAM_ID */
     SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,           /* SEC Inject Error Type */
     0x10,                                             /* secFlipMask of type uint32_t */
     SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,           /* DED Inject Error Type */
     0x101,                                            /* dedFlipMask of type uint32_t */
     SDR_ECC_RAM_ID_TYPE_WRAPPER,                      /* ramIdType of type SDR_ECC_RamIdType */
     0,                                                /* Wrapper type, so no Group checker ID */
     0x40F82004u,                                      /* testAddressTrigLoc of type uint32_t */
     NULL                                              /* testTrigFunc filled in initialization function */
    },
    /* Main ESM, MSMC ECC Aggregator, Interconnect ECC Type */
    {SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,                  /* eccAggrId of type SDR_ECC_MemType */
     SDR_ECC_MAIN_MSMC_MEM_INTERCONN_SUBTYPE,          /* eccRamId of type SDR_ECC_MemSubType - CSL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_MSMC_MMR_BUSECC_RAM_ID  */
     SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,           /* SEC Inject Error Type */
     0x10,                                             /* secFlipMask of type uint32_t */
     SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,           /* DED Inject Error Type */
     0x101,                                            /* dedFlipMask of type uint32_t */
     SDR_ECC_RAM_ID_TYPE_INTERCONNECT,                 /* ramIdType of type SDR_ECC_RamIdType */
     CSL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_MSMC_MMR_BUSECC_GROUP_0_ID, /* EDC Group Type */
     0x0,                                              /* testAddressTrigLoc of type uint32_t,
                                                        * not used for Interconnect Type */
     NULL                                              /* testTrigFunc filled in initialization function */
    },
    /* Main ESM, MSMC ECC Aggregator, Wrapper ECC Type */
    {SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,                  /* eccAggrId of type SDR_ECC_MemType */
     SDR_ECC_MAIN_MSMC_MEM_WRAPPER_SUBTYPE,            /* eccRamId of type SDR_ECC_MemSubType */
     SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_ONCE,     /* SEC Inject Error Type */
     0x10,                                             /* secFlipMask of type uint32_t */
     SDR_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_ONCE,     /* DED Inject Error Type */
     0x101,                                            /* dedFlipMask of type uint32_t */
     SDR_ECC_RAM_ID_TYPE_WRAPPER,                      /* ramIdType of type SDR_ECC_RamIdType */
     0,                                                /* Wrapper type, so no Group checker ID */
     0x0,                                              /* testAddressTrigLoc of type uint32_t,
                                                        * not used for this inaccesible Wrapper type */
     NULL                                              /* testTrigFunc filled in initialization function */
    },
    /* Multiple iterations on Main ESM, MSMC ECC Aggregator, Interconnect ECC Type */
    {SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,                  /* eccAggrId of type SDR_ECC_MemType */
     SDR_ECC_MAIN_MSMC_MEM_INTERCONN_SUBTYPE,          /* eccRamId of type SDR_ECC_MemSubType */
     SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,           /* SEC Inject Error Type */
     0x10,                                             /* secFlipMask of type uint32_t */
     SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,           /* DED Inject Error Type */
     0x101,                                            /* dedFlipMask of type uint32_t */
     SDR_ECC_RAM_ID_TYPE_INTERCONNECT,                 /* ramIdType of type SDR_ECC_RamIdType */
     CSL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_MSMC_MMR_BUSECC_GROUP_0_ID, /* EDC Group Type */
     0x0,                                              /* testAddressTrigLoc of type uint32_t,
                                                        * not used for Interconnect Type */
     NULL                                              /* testTrigFunc filled in initialization function */
    },
    /* Main ESM, MSMC ECC Aggregator, Wrapper ECC Type */
    {SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,                  /* eccAggrId of type SDR_ECC_MemType */
     SDR_ECC_MAIN_MSMC_MEM_WRAPPER_SUBTYPE,            /* eccRamId of type SDR_ECC_MemSubType */
     SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_ONCE,     /* SEC Inject Error Type */
     0x10,                                             /* secFlipMask of type uint32_t */
     SDR_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_ONCE,     /* DED Inject Error Type */
     0x101,                                            /* dedFlipMask of type uint32_t */
     SDR_ECC_RAM_ID_TYPE_WRAPPER,                      /* ramIdType of type SDR_ECC_RamIdType */
     0,                                                /* Wrapper type, so no Group checker ID */
     0x0,                                              /* testAddressTrigLoc of type uint32_t,
                                                        * not used for this inaccessible Wrapper type */
     NULL                                              /* testTrigFunc filled in initialization function */
    },
};

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

extern volatile uint32_t gEsmEventRcvdFlag;

extern volatile uint32_t eccEventInputTrig[6];

extern int32_t (*useCaseTriggerFuncArray[6])();

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*********************************************************************
 * @fn      runSingleSECEvent
 *
 * @brief   Execute ECC 1 bit inject test
 *
 * @param   eccAggrId - ID of the ECC Aggregator
 * @param   eccRamId - RAM ID into which the error will be injected
 * @param   injErrType - inject error type
 * @param   flipMask - bit mask that indicates what bit to inject the error
 * @param   ramIdType - type of RAM ID (either Wrapper or Interconnect Type)
 * @param   chkGrp - check group ID (only valid for Interconnect Type, otherwise set to 0)
 * @param   testAddressTrigLoc - memory address that needs to be read to trigger
 *                               SEC event.
 * @param   testTrigFunc - function that needs to be run to trigger SEC event.
 *                         It is expected that only testAddressTrigLoc or testTrigFunc
 *                         is needed for each use case (and cannot be both).
 *
 * @return  0 : Success; < 0 for failures
 */
static int32_t runSingleSECEvent(SDR_ECC_MemType eccAggrId,
                                 SDR_ECC_MemSubType eccRamId,
                                 SDR_ECC_InjectErrorType injErrType,
                                 uint32_t flipMask,
                                 uint32_t ramIdType,
                                 uint32_t chkGrp,
                                 uint32_t testAddressTrigLoc,
                                 int32_t (*testTrigFunc)(void) )
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;
#ifdef DEBUG_PRINT
    DIAG_printf("\n Single bit error inject: test starting for eccAggr %d, eccRamId %d, " \
                "flipMask 0x%x, ramIdType %d, testAddressTrigLoc = 0x%x\n",
                 eccAggrId, eccRamId, flipMask, ramIdType, testAddressTrigLoc);    
#endif
    /* Note the address is Actual error address */
    injectErrorConfig.pErrMem = (uint32_t *)(testAddressTrigLoc);

    /* Run one shot test for 1 bit error */
    injectErrorConfig.flipBitMask = flipMask;
    injectErrorConfig.chkGrp      = chkGrp;

    result = SDR_ECC_injectError(eccAggrId,
                                 eccRamId,
                                 injErrType,
                                 &injectErrorConfig);
    if (result != SDR_PASS ) {
        DIAG_printf("\n Single bit error inject at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        if (testAddressTrigLoc != 0) {
            /* Access the memory where injection is expected */
            testLocationValue = injectErrorConfig.pErrMem[0];
        } else if (testTrigFunc != NULL) {
            /* Call function to trigger error */
            retVal = (*testTrigFunc)();
            if (retVal != 0) {
                DIAG_printf("\n Single bit error inject: testTrigFunc call failed");
            }
        }

#ifdef DEBUG_PRINT
        /* Note this should result in actual error call back */
        DIAG_printf("\n Single bit error inject at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
#endif
    }

    return retVal;
} 

/*********************************************************************
 * @fn      runSingleDEDEvent
 *
 * @brief   Execute ECC 2 bit inject test
 *
 * @param   eccAggrId - ID of the ECC Aggregator
 * @param   eccRamId - RAM ID into which the error will be injected
 * @param   injErrType - inject error type
 * @param   flipMask - bit mask that indicates what bit to inject the error
 * @param   ramIdType - type of RAM ID (either Wrapper or Interconnect Type)
 * @param   chkGrp - check group ID (only valid for Interconnect Type, otherwise set to 0)
 * @param   testAddressTrigLoc - memory address that needs to be read to trigger
 *                               SEC event.
 * @param   testTrigFunc - function that needs to be run to trigger SEC event.
 *                         It is expected that only testAddressTrigLoc or testTrigFunc
 *                         is needed for each use case (and cannot be both).
 *
 * @return  0 : Success; < 0 for failures
 */
static int32_t runSingleDEDEvent(SDR_ECC_MemType eccAggrId,
                                 SDR_ECC_MemSubType eccRamId,
                                 SDR_ECC_InjectErrorType injErrType,
                                 uint32_t flipMask, 
                                 uint32_t ramIdType,
                                 uint32_t chkGrp,
                                 uint32_t testAddressTrigLoc,
                                 int32_t (*testTrigFunc)(void) )
{
    SDR_Result result;
    int32_t retVal=0;

    SDR_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;
#ifdef DEBUG_PRINT
    DIAG_printf("\n Double bit error inject: test starting for eccAggr %d, eccRamId %d, " \
                "flipMask 0x%x, ramIdType %d, testAddressTrigLoc = 0x%x\n",
                 eccAggrId, eccRamId, flipMask, ramIdType, testAddressTrigLoc);    
#endif
    /* Note the address is Actual error address */
    injectErrorConfig.pErrMem = (uint32_t *)(testAddressTrigLoc);

    /* Run one shot test for 2 bit error */
    injectErrorConfig.flipBitMask = flipMask;
    injectErrorConfig.chkGrp      = chkGrp;

    result = SDR_ECC_injectError(eccAggrId,
                                 eccRamId,
                                 injErrType,
                                 &injectErrorConfig);

    if (result != SDR_PASS ) {
        DIAG_printf("\n Double bit error inject at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        if (testAddressTrigLoc != 0) {
            /* Access the memory where injection is expected */
            testLocationValue = injectErrorConfig.pErrMem[0];
        } else if (testTrigFunc != NULL) {
            retVal = (*testTrigFunc)();
            if (retVal != 0) {
                DIAG_printf("\n Double bit error inject: testTrigFunc call failed");
            }
        }
#ifdef DEBUG_PRINT
        /* Note this should result in actual error call back */
        DIAG_printf("\n Double bit error inject at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
#endif
    }

    return retVal;
} 


/*********************************************************************
 * @fn      diag_ecc_initUseCases
 *
 * @brief   Perform any remaining initialization of the use cases
 *          based on SoC-specific definitions
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t diag_ecc_initUseCases(void)
{
    int8_t i;
    int32_t retValue;

    /* Populate the Use Cases with any SoC-specific testTrigFunc */
    for (i = 0; i < 6; i++) {
        useCaseArray[i].testTrigFunc = useCaseTriggerFuncArray[i];
    }

    retValue = eventTriggerSetup();

    return (retValue);
}

/*********************************************************************
 * @fn      diag_ecc_runUseCaseTrigger
 *
 * @brief   Initiate the input trigger event(s) for each use case
 *
 * @param   useCaseId - use case instance for which trigger event(s)
 *          should be initiated
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t diag_ecc_runUseCaseTrigger(uint8_t useCaseId)
{
    int32_t retVal1, retVal2, numMultiSecEvents, numMultiDedEvents, i;
    int32_t status = 0;

    DIAG_printf("\nStarting Use Case %d \n", useCaseId);
    switch(useCaseId)
    {
        case 0:
            /* UC-1: Single SEC/DED events on MCU_ESM0 with Interconnect ECC type */
        case 1:
            /* UC-2: Single SEC/DED events on MCU_ESM0 with Wrapper ECC type */
        case 2:
            /* UC-3: Single SEC/DED events on Main Domain ESM0 with Interconnect ECC type */
        case 3:
            /* UC-4: Single SEC/DED events on Main Domain ESM0 with Wrapper ECC type. */
            gEsmEventRcvdFlag = ESM_EVENT_NOT_RCVD;

            /* Trigger a single SEC event */
            retVal1 = runSingleSECEvent(useCaseArray[useCaseId].eccAggrId,
                                        useCaseArray[useCaseId].eccRamId,
                                        useCaseArray[useCaseId].secInjErrType,
                                        useCaseArray[useCaseId].secFlipMask,
                                        useCaseArray[useCaseId].ramIdType,
                                        useCaseArray[useCaseId].chkGrp,
                                        useCaseArray[useCaseId].testAddressTrigLoc,
                                        useCaseArray[useCaseId].testTrigFunc);

            /* Wait for the ESM event to complete */
            while(gEsmEventRcvdFlag == ESM_EVENT_NOT_RCVD) {}

            /* Trigger a single DED event */
            retVal2 = runSingleDEDEvent(useCaseArray[useCaseId].eccAggrId,
                                        useCaseArray[useCaseId].eccRamId,
                                        useCaseArray[useCaseId].dedInjErrType,
                                        useCaseArray[useCaseId].dedFlipMask,
                                        useCaseArray[useCaseId].ramIdType,
                                        useCaseArray[useCaseId].chkGrp,
                                        useCaseArray[useCaseId].testAddressTrigLoc,
                                        useCaseArray[useCaseId].testTrigFunc);

            if ((retVal1 == 0) && (retVal2 == 0)) {
                eccEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_SUCCESS;
            } else {
                eccEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_FAILURE;
            }
            break;

        case 4:
            /* UC-5: Multiple SEC and DED events on Main Domain ESM0 with Interconnect ECC type */
        case 5:
            /* UC-6: Multiple SEC and DED events on Main Domain ESM0 with Wrapper ECC type */
            numMultiSecEvents = 0;
            numMultiDedEvents = 0;
            for (i = 0; i < NUM_MULTIPLE_SEC_EVENTS; i++)
            {
                gEsmEventRcvdFlag = ESM_EVENT_NOT_RCVD;
                /* Trigger a single SEC event */
                retVal1 = runSingleSECEvent(useCaseArray[useCaseId].eccAggrId,
                                            useCaseArray[useCaseId].eccRamId,
                                            useCaseArray[useCaseId].secInjErrType,
                                            useCaseArray[useCaseId].secFlipMask,
                                            useCaseArray[useCaseId].ramIdType,
                                            useCaseArray[useCaseId].chkGrp,
                                            useCaseArray[useCaseId].testAddressTrigLoc,
                                            useCaseArray[useCaseId].testTrigFunc);

                /* Wait for the ESM event to complete */
                while(gEsmEventRcvdFlag == ESM_EVENT_NOT_RCVD) {}

                if (retVal1 != 0) {
                    break;
                } else {
                    numMultiSecEvents++;
                }
            }

            for (i = 0; i < NUM_MULTIPLE_DED_EVENTS; i++)
            {
                gEsmEventRcvdFlag = ESM_EVENT_NOT_RCVD;
                /* Trigger a single DED event */
                retVal2 = runSingleDEDEvent(useCaseArray[useCaseId].eccAggrId,
                                            useCaseArray[useCaseId].eccRamId,
                                            useCaseArray[useCaseId].dedInjErrType,
                                            useCaseArray[useCaseId].dedFlipMask,
                                            useCaseArray[useCaseId].ramIdType,
                                            useCaseArray[useCaseId].chkGrp,
                                            useCaseArray[useCaseId].testAddressTrigLoc,
                                            useCaseArray[useCaseId].testTrigFunc);

                /* Wait for the ESM event to complete */
                while(gEsmEventRcvdFlag == ESM_EVENT_NOT_RCVD) {}

                if (retVal2 != 0) {
                    break;
                } else {
                    numMultiDedEvents++;
                }
            }

            if ((retVal1 == 0) && (numMultiSecEvents == NUM_MULTIPLE_SEC_EVENTS) &&
                (retVal2 == 0) && (numMultiDedEvents == NUM_MULTIPLE_DED_EVENTS)) {
                eccEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_SUCCESS;
            } else {
                eccEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_FAILURE;
            }
            break;

        default:
            DIAG_printf("ERR: Invalid Use Case ID %d \n", useCaseId);
            status = -1;
            break;
    }

    return (status);
}

