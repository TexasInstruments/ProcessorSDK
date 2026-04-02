/*
 * SDR ECC
 *
 * Software Diagnostics Reference module for ECC
 *
 *  Copyright (c) Texas Instruments Incorporated 2018-2022
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

#include <string.h>
#include <stdint.h>
#include <sdr_common.h>
#include <sdr_ecc.h>
#include <sdr_esm.h>
#include <sdr_exception.h>

#include <ti/csl/csl_rat.h>
#include <ti/csl/csl_ecc_aggr.h>

#include "sdr_utils.h"
#include "sdr_ecc_utils.h"

#include "sdr_ecc_soc.h"
#include "sdr_ecc_core.h"

#include "sdr_ecc_priv.h"

/* Local defines */
#define SDR_ECC_INVALID_ERROR_SOURCE (0xffffffffu)

#define BITS_PER_WORD (32u)

#define ECC_AGGR_LINE_SIZE (4)

#define SDR_ECC_INVALID_SELF_TEST_RAM_ID (0xffffffffu)
#define SDR_ECC_INVALID_CHECKER_TYPE     (0xffffffffu)

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the values for ecc self test flag
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_ECC_ERROR_FLAG_NONE=0,
    /**< Flag set during ECC initialization or end of the ECC self test */
    SDR_ECC_ERROR_FLAG_STARTING=1,
    /**< Flag set at the start of the ECC self test */
    SDR_ECC_ERROR_FLAG_TRIGGERED=2,
    /**< Flag set when ECC error happens during the ECC self test */

} SDR_ECC_ErrorFlag;

/** ---------------------------------------------------------------------------
 * @brief This structure defines the elements of ECC software instance
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_ECC_Instance_s
{
    SDR_ECC_InitConfig_t eccInitConfig;
    /**< ecc Initial configuration */
    SDR_ECC_ErrorFlag eccErrorFlag;
    /**< Ecc error triggered flag */
    SDR_ECC_InjectErrorType eccSelfTestErrorType;
    /**< Ecc self type error type in progress */
    uint32_t eccSelfTestRamId;
    /**< Ram id used in self test in progress */
    uint32_t *eccSelfTestAddr;
    /**< Address used in self test in progress */
    uint32_t eccSelfTestcheckerType;
    /**< Group checker (for Interconnect RAM ID's only) */
}  SDR_ECC_Instance_t;

/* Global objects */
static SDR_ECC_Instance_t SDR_ECC_instance[SDR_ECC_AGGREGATOR_MAX_ENTRIES];

/* Local functions */
static SDR_Result SDR_ECC_getRamId(SDR_ECC_MemType eccMemType, SDR_ECC_MemSubType memSubType,
                           uint32_t *ramIdP, uint32_t *pRAMIdType);
static SDR_Result SDR_ECC_getAggregatorType(SDR_ECC_MemType eccMemType,
                           SDR_ECC_MemSubType memSubType, uint32_t *pIinjectOnly);
static SDR_Result SDR_ECC_getAggrBaseAddr(SDR_ECC_MemType eccMemType, CSL_ecc_aggrRegs **pEccAggr);
static SDR_Result SDR_ECC_memoryRefresh(uint32_t *memAddr, size_t size);
static void SDR_ECC_triggerAccessForEcc(const uint32_t *memoryAccessAddr);

static SDR_Result SDR_ECC_getMemConfig(SDR_ECC_MemType eccMemType, SDR_ECC_MemSubType memSubType,
                               SDR_MemConfig_t *memConfig);
static SDR_Result SDR_ECC_getEDCCheckerGroupConfig(SDR_ECC_MemType eccMemType,
                                                   SDR_ECC_MemSubType memSubType,
                                                   uint32_t chkGrp,
                                                   SDR_GrpChkConfig_t *grpChkConfig);
static uint32_t SDR_ECC_getDetectErrorSource (SDR_ECC_InjectErrorType injectErorType);
static SDR_Result SDR_ECC_getBitLocation(uint32_t bitMask,
                            uint32_t startBitLocation,
                            uint32_t *bitLocation);
static void SDR_ECC_handleEccAggrEvent (SDR_ECC_MemType eccMemType, uint32_t errorSrc,
                                       uint32_t errorAddr);
static void SDR_ECC_ESMCallBackFunction_MCU (uint32_t errorSrc, uint32_t errorAddr);
#ifdef SOC_J721E
static void SDR_ECC_ESMCallBackFunction_MAIN (uint32_t errorSrc, uint32_t errorAddr);
#endif
static SDR_Result SDR_ECC_checkMemoryType(SDR_ECC_MemType eccMemType, SDR_ECC_MemSubType memSubType);

static SDR_Result SDR_ECC_searchMemEntryTable(SDR_ECC_MemSubType memSubType,
                                              const SDR_MemConfig_t memEntryTable[],
                                              uint32_t tableSize,
                                              SDR_MemConfig_t *pMemConfig);

#ifdef SOC_J721E
/* Note that this example provide a single instance of mappedEccRegs (which is RAT-mapped
 * ECC aggregator configuration registers that lie in larger address space than the 32-bit
 * address space on the MCU.  If more ECC aggregator registers need to be mapped, additional
 * global variables are needed for each set of aggregator registers, and SDR_ECC_init() needs
 * to be modified to make SDR_ECC_mapEccAggrReg() calls for each one that needs to be mapped.
 * The expectation is that this mapping will be retained in perpetuity because in order to obtain
 * information about the ECC errors, the ECC Aggregator configuration registers require to be
 * visible from the MCU. */
__attribute((section(".my_aggr_reg"))) CSL_ecc_aggrRegs mappedEccRegs;
#endif

/** ============================================================================*
 *
 * \brief   Get the Error Source corresponding to the inject error
 *          type
 *
 *   @n  This function is called in ECC callback function to convert the injected
 *       error type to the source of the ECC error
 *
 * \param1  errorType: error Type
 *
 * \return  error Source or SDR_ECC_INVALID_ERROR_SOURCE in case of error
 */
static uint32_t SDR_ECC_getDetectErrorSource (SDR_ECC_InjectErrorType injectErorType)
{
    uint32_t errorSource;

    switch(injectErorType) {
        case SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE:
        case SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_ONCE:
        case SDR_INJECT_ECC_ERROR_FORCING_1BIT_REPEAT:
        case SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_REPEAT:
            errorSource = SDR_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR;
            break;


        case SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE:
        case SDR_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_ONCE:
        case SDR_INJECT_ECC_ERROR_FORCING_2BIT_REPEAT:
        case SDR_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_REPEAT:
            errorSource = SDR_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR;
            break;

        default:
            errorSource = ((uint32_t)SDR_ECC_INVALID_ERROR_SOURCE);
            break;
    }
    return errorSource;
}

#ifdef SOC_J721E

/* Returns index >= 0 if successful, otherwise returns -1 if failure */
static int32_t SDR_ECC_mapRatEccAggrBaseAddress(uint64_t highEccAggrAddr, int32_t desiredMapIdx, CSL_ecc_aggrRegs **ppEccAggr)
{
    bool result;
    int32_t mappedIdx;

    CSL_RatTranslationCfgInfo translationCfg;

    /* Add RAT configuration to access address > 32bit address range */
    translationCfg.translatedAddress = highEccAggrAddr;
    translationCfg.sizeInBytes = sizeof(CSL_ecc_aggrRegs);
    translationCfg.baseAddress = (uint32_t)(*ppEccAggr);

    /* Set up RAT translation */
    result = CSL_ratConfigRegionTranslation((CSL_ratRegs *)CSL_MCU_ARMSS_RAT_CFG_BASE,
                                                desiredMapIdx, &translationCfg);
    if (result == false) {
        mappedIdx = -1;
    } else {
        mappedIdx = desiredMapIdx;
    }

    return (mappedIdx);
}



/** ============================================================================*
 *
 * \brief   Map an ECC Aggregator using the RAT
 *
 *   @n  This function is called prior to SDR_ECC_init() for an ECC aggregator
 *   where the base address of the ECC Aggegator is greater than 32-bits, so
 *   needs a mapping by the RAT.
 *
 * \param1  errorType: error Type
 *
 * \return  error Source or SDR_ECC_INVALID_ERROR_SOURCE in case of error
 */
/* Returns index >= 0 if successful, otherwise returns -1 if failure */
SDR_Result SDR_ECC_mapEccAggrReg(SDR_ECC_MemType eccMemType, CSL_ecc_aggrRegs **ppEccAggr)
{
    SDR_Result retVal = SDR_PASS;
    CSL_ecc_aggrRegs *eccAggrRegs;
    int32_t mapIdx;

    if ((NULL == ppEccAggr) || (NULL == *ppEccAggr))
    {
        retVal = SDR_BADARGS;
    } else {
        eccAggrRegs = *ppEccAggr;
        mapIdx = SDR_ECC_mapRatEccAggrBaseAddress(SDR_ECC_aggrHighBaseAddressTable[eccMemType - \
                                      SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0],
                                      eccMemType - SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0,
                                      &eccAggrRegs);
        if (mapIdx < 0) {
            retVal = SDR_FAIL;
        } else {
            /* Mapping was successful, so fill the array with the local address */
            SDR_ECC_aggrHighBaseAddressTableTrans[mapIdx] = eccAggrRegs;  
        }
    }

    return retVal;
}

#endif

/** ============================================================================*
 *
 * \brief   Ecc call back function registered with exception handler
 *
 * \param1  eccMemType: ECC Memory Type
 * \param2  errorSrc: Source of ECC Error
 * \param3  errorAddr: ECC error address
 *
 * \return  None
 */
static void SDR_ECC_handleEccAggrEvent (SDR_ECC_MemType eccMemType, uint32_t errorSrc,
                                       uint32_t errorAddr)
{
    CSL_ecc_aggrRegs *eccAggrRegs;
    CSL_Ecc_AggrEccRamErrorStatusInfo eccErrorStatusWrap = {0};
    CSL_Ecc_AggrEDCInterconnectErrorStatusInfo eccErrorStatusInterconn = {0};
    uint8_t bitErrCnt = 0;
    uint32_t ramId=0;
    uint32_t ramIdType=0;
    SDR_Result retVal;
    SDR_ECC_MemSubType memSubType;
    SDR_MemConfig_t memConfig;
    uint32_t i;
    uint64_t bitErrorOffset = 0;
    uint64_t bitErrorGroup = 0;
    bool eventFound = ((bool)false);
    bool eventFound1 = ((bool)false);
    int32_t cslResult = -1;
    uint32_t selfTestErrorSrc;

    SDR_ECC_getAggrBaseAddr(eccMemType, &eccAggrRegs);

    /* Check which Ram Id triggered the error */
    for (i = ((uint32_t)0U);
         i < SDR_ECC_instance[eccMemType].eccInitConfig.numRams;
         i++) {
        /* Get corresponding ram Id */
        memSubType = SDR_ECC_instance[eccMemType].eccInitConfig.pMemSubTypeList[i];
        retVal = SDR_ECC_getRamId(eccMemType, memSubType, &ramId, &ramIdType);
        if (retVal != SDR_PASS) {
            /* Unexpected FAILURE: call Assert */
            SDR_assertExternalFunction(SDR_ECC_RAM_ID_NOT_FOUND);
            continue;
        }

        /* Check if this event is triggered, by reading the ECC aggregator status
         * register */
        if (ramIdType == SDR_ECC_RAM_ID_TYPE_WRAPPER) {
            cslResult = CSL_ecc_aggrIsEccRamIntrPending(eccAggrRegs,
                                                        ramId,
                                                        errorSrc,
                                                        &eventFound1);
        } else if (ramIdType == SDR_ECC_RAM_ID_TYPE_INTERCONNECT) {
            cslResult = CSL_ecc_aggrIsEDCInterconnectIntrPending(eccAggrRegs,
                                                                 ramId,
                                                                 errorSrc,
                                                                 &eventFound1);
        }

        if((cslResult == CSL_PASS)  && (eventFound1)) {
            eventFound = ((bool)true);

            /* Read the locations of the bit errors */
            if (ramIdType == SDR_ECC_RAM_ID_TYPE_WRAPPER) {
                /* Get the error status information for Wrapper type */
                (void)CSL_ecc_aggrGetEccRamErrorStatus(eccAggrRegs,
                                                       ramId,
                                                       &eccErrorStatusWrap);
                /* Get the total number of interrupts pending */
                if (errorSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) {
                    bitErrCnt = eccErrorStatusWrap.singleBitErrorCount;
                } else if (errorSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT) {
                    bitErrCnt = eccErrorStatusWrap.doubleBitErrorCount;
                }
            } else if (ramIdType == SDR_ECC_RAM_ID_TYPE_INTERCONNECT) {
                (void)CSL_ecc_aggrGetEDCInterconnectErrorStatus(eccAggrRegs,
                                                                ramId,
                                                                &eccErrorStatusInterconn);

                /* Get the total number of Inject interrupts pending. Note that
                 * "Inject" interrupts are separate from "Normal" interrupts for
                 * Interconnect ECC type */
                if (errorSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) {
                    bitErrCnt = eccErrorStatusInterconn.injectSingleBitErrorCount;
                } else if (errorSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT) {
                    bitErrCnt = eccErrorStatusInterconn.injectDoubleBitErrorCount;
                }
            }

            /* Clear the event(s) */
            if (ramIdType == SDR_ECC_RAM_ID_TYPE_WRAPPER) {
                (void)CSL_ecc_aggrClrEccRamNIntrPending(eccAggrRegs,
                                                        ramId,
                                                        errorSrc,
                                                        bitErrCnt);
            } else if (ramIdType == SDR_ECC_RAM_ID_TYPE_INTERCONNECT) {
                (void)CSL_ecc_aggrClrEDCInterconnectNIntrPending(eccAggrRegs,
                                                                 ramId,
                                                                 errorSrc,
                                                                 CSL_ECC_AGGR_ERROR_SUBTYPE_INJECT,
                                                                 bitErrCnt);
            }

            /* Check If it matches self test set flag.
             * NOTE: For EDC interconnect type single bit error injection with parity checker type
             *       Single bit error injection can result in Uncorrectable (double bit) error event
             */
            selfTestErrorSrc = SDR_ECC_getDetectErrorSource(SDR_ECC_instance[eccMemType].eccSelfTestErrorType);
            if (((errorSrc == selfTestErrorSrc)
                 || ((ramIdType == SDR_ECC_RAM_ID_TYPE_INTERCONNECT)
                     && (SDR_ECC_instance[eccMemType].eccSelfTestcheckerType != CSL_ECC_AGGR_CHECKER_TYPE_EDC) 
                     && (selfTestErrorSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT)
                     && (errorSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT)
                    )
                )
                && (ramId == SDR_ECC_instance[eccMemType].eccSelfTestRamId)) {

                 SDR_ECC_instance[eccMemType].eccErrorFlag = SDR_ECC_ERROR_FLAG_TRIGGERED;
            } else {
                if (ramIdType == SDR_ECC_RAM_ID_TYPE_WRAPPER) {
                    retVal = SDR_ECC_getMemConfig(eccMemType, ramId, &memConfig);
                    if (retVal == SDR_PASS) {
                        if (errorSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) {
                            bitErrorOffset = eccErrorStatusWrap.eccRow * memConfig.rowSize +
                                             eccErrorStatusWrap.eccBit1;
                        } else if (errorSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT) {
                            /* In case of DED error, eccBit1 is not valid, so calculate
                             * the bit offset of the start of the row with the DED error */
                            bitErrorOffset = eccErrorStatusWrap.eccRow * memConfig.rowSize;
                        }
                    }
                    /* Bit error group not used for Wrapper type */
                    bitErrorGroup  = 0;
                } else if (ramIdType == SDR_ECC_RAM_ID_TYPE_INTERCONNECT) {
                    bitErrorGroup  = eccErrorStatusInterconn.eccGroup;
                    if (errorSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) {
                        bitErrorOffset = eccErrorStatusInterconn.eccBit1;
                    } else if (errorSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT) {
                        /* In case of DED error, eccBit1 is not valid */
                        bitErrorOffset = 0;
                    }
                }

                /* Call application callback function to indicate error */
                SDR_ECC_applicationCallbackFunction(eccMemType,
                                                    errorSrc,
                                                    errorAddr,
                                                    ramId,
                                                    bitErrorOffset,
                                                    bitErrorGroup);
            }
        } /* if((cslResult == CSL_PASS)  && (eventFound1)) { */
    } /* RAM ID for loop */

    /* Ack Ecc aggregator error */
    cslResult = CSL_ecc_aggrAckIntr(eccAggrRegs, errorSrc);
    if ((cslResult != CSL_PASS) || (!eventFound)) {
        /* Unexpected event; Call assert */
        SDR_assertExternalFunction(SDR_ECC_INTERRUPT_WITH_NOEVENT);
    }
    return;
}

/** ============================================================================*
 *
 * \brief   Ecc call back function registered with exception handler
 *          Certain ECC events on CPU memory are reported as Exception events
 *          And this function provides a way to intercept the event to do self test
 *
 * \param1  errorSrc: source of the ECC error reported.
 * \param2  errorAddr: Error address at which the actual error happened
 * \param3  ramId: ramId at which the actual error happened
 * \param4  bitErrorOffset: offset within the ramId that the bit error occurred
 *          For SEC errors, this contains the exact bit location for the single
 *          corrected error.  For DED Wrapper errors, this contains the bit location
 *          of the start of the row where the double error occurred.  For DED
 *          Interconnect errors, this field is invalid and set to 0.
 * \param5  bitErrorGroup: group checker that reported the error
 *          (Interconnect ECC type only).
 *
 * \return  None
 */
static void SDR_ECC_callBackFunction (uint32_t errorSrc, uint32_t errorAddr,
                                      uint32_t ramId, uint64_t bitErrorOffset,
                                      uint32_t bitErrorGroup)
{
    SDR_ECC_MemType eccMemType;


    /* Check Error Source and call acknowledge appropriate interrupt */
    switch (errorSrc) {
        case SDR_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR:
        case SDR_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR:
        case SDR_ESM_ECC_PARAM_MCU_CPU1_SEC_ERROR:
        case SDR_ESM_ECC_PARAM_MCU_CPU1_DED_ERROR:
            /* Get corresponding Memory type */
            switch(errorSrc)
            {
                default:
                case SDR_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR:
                case SDR_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR:
                    eccMemType = SDR_ECC_MEMTYPE_MCU_R5F0_CORE;
                    break;

                case SDR_ESM_ECC_PARAM_MCU_CPU1_SEC_ERROR:
                case SDR_ESM_ECC_PARAM_MCU_CPU1_DED_ERROR:
                    eccMemType = SDR_ECC_MEMTYPE_MCU_R5F1_CORE;
                    break;

            }

            /* If self test in progress, indicate triggered flag */
            if ((errorSrc
                 == SDR_ECC_getDetectErrorSource(SDR_ECC_instance[eccMemType].eccSelfTestErrorType))
                && ((errorAddr == ((uint32_t)SDR_ESM_ERRORADDR_INVALID))
                || (((uintptr_t)errorAddr)
                    == ((uintptr_t)SDR_ECC_instance[eccMemType].eccSelfTestAddr)))
                && (SDR_ECC_instance[eccMemType].eccErrorFlag == SDR_ECC_ERROR_FLAG_STARTING)) {
                SDR_ECC_instance[eccMemType].eccErrorFlag = SDR_ECC_ERROR_FLAG_TRIGGERED;
            } else {
                /* Execute call back */
                SDR_ECC_applicationCallbackFunction(eccMemType,
                                                    errorSrc,
                                                    errorAddr,
                                                    ramId,
                                                    bitErrorOffset,
                                                    bitErrorGroup);
            }

            break;

        default:
            break;
    }


}


/** ============================================================================*
 *
 * \brief   Ecc call back function registered with MCU ESM handler
 *
 * \param1  errorSrc: Error source
 *
 * \return  None
 */
static void SDR_ECC_ESMCallBackFunction_MCU (uint32_t errorSrc, uint32_t errorAddr)
{
    SDR_ECC_MemType eccMemType;
    CSL_Ecc_AggrIntrSrc intrSrcType;

    /* Check Error Source and call acknowledge appropriate interrupt */
    switch (errorSrc) {
        case SDR_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR:
        case SDR_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR:
        case SDR_ESM_ECC_PARAM_MCU_CPU1_SEC_ERROR:
        case SDR_ESM_ECC_PARAM_MCU_CPU1_DED_ERROR:
        case SDR_ESM_ECC_PARAM_MCU_CBASS_SEC_ERROR:
        case SDR_ESM_ECC_PARAM_MCU_CBASS_DED_ERROR:
            /* Get corresponding Memory type */
            switch(errorSrc)
            {
                default:
                case SDR_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR:
                    intrSrcType = CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT;
                    eccMemType  = SDR_ECC_MEMTYPE_MCU_R5F0_CORE;
                    break;

                case SDR_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR:
                    intrSrcType = CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT;
                    eccMemType  = SDR_ECC_MEMTYPE_MCU_R5F0_CORE;
                    break;

                case SDR_ESM_ECC_PARAM_MCU_CPU1_SEC_ERROR:
                    intrSrcType = CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT;
                    eccMemType  = SDR_ECC_MEMTYPE_MCU_R5F1_CORE;
                    break;

                case SDR_ESM_ECC_PARAM_MCU_CPU1_DED_ERROR:
                    intrSrcType = CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT;
                    eccMemType  = SDR_ECC_MEMTYPE_MCU_R5F1_CORE;
                    break;

#ifdef SOC_J721E
                case SDR_ESM_ECC_PARAM_MCU_CBASS_SEC_ERROR:
                    intrSrcType = CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT;
                    eccMemType  = SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0;
                    break;

                case SDR_ESM_ECC_PARAM_MCU_CBASS_DED_ERROR:
                    intrSrcType = CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT;
                    eccMemType  = SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0;
                    break;
#endif
            }

            /* Handle ECC Aggregator event */
            SDR_ECC_handleEccAggrEvent(eccMemType, intrSrcType, errorAddr);

            break;

        default:
            break;
    }


}

#ifdef SOC_J721E
/** ============================================================================*
 *
 * \brief   Ecc call back function registered with Main Domain ESM handler
 *
 * \param1  errorSrc: Error source
 *
 * \return  None
 */
static void SDR_ECC_ESMCallBackFunction_MAIN (uint32_t errorSrc, uint32_t errorAddr)
{
    SDR_ECC_MemType eccMemType;
    uint32_t eccEventType;

    /* Check Error Source and call acknowledge appropriate interrupt */
    switch (errorSrc) {
        case SDR_ESM_ECC_PARAM_MAIN_MSMC_AGGR0_SEC_ERROR:
        case SDR_ESM_ECC_PARAM_MAIN_MSMC_AGGR0_DED_ERROR:
        case SDR_ESM_ECC_PARAM_MAIN_A72_AGGR0_SEC_ERROR:
        case SDR_ESM_ECC_PARAM_MAIN_A72_AGGR0_DED_ERROR:
            /* Get corresponding Memory type */
            switch(errorSrc)
            {
                default:
                case SDR_ESM_ECC_PARAM_MAIN_MSMC_AGGR0_SEC_ERROR:
                    eccMemType   = SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0;
                    eccEventType = CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT;
                    break;
                case SDR_ESM_ECC_PARAM_MAIN_MSMC_AGGR0_DED_ERROR:
                    eccMemType   = SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0;
                    eccEventType = CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT;
                    break;
                case SDR_ESM_ECC_PARAM_MAIN_A72_AGGR0_SEC_ERROR:
                    eccMemType   = SDR_ECC_MEMTYPE_MAIN_A72_AGGR0;
                    eccEventType = CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT;
                    break;
                case SDR_ESM_ECC_PARAM_MAIN_A72_AGGR0_DED_ERROR:
                    eccMemType   = SDR_ECC_MEMTYPE_MAIN_A72_AGGR0;
                    eccEventType = CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT;
                    break;
            }
            /* Handle ECC Aggregator event */
            SDR_ECC_handleEccAggrEvent(eccMemType, eccEventType, errorAddr);

            break;

        default:
            break;
    }


}
#endif

/** ============================================================================*
 *
 * \brief   Initializes an ESM module for usage with ECC module
 *
 * \param   esmInstType: Instance of ESM
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ECC_initEsm (const SDR_ESM_InstanceType esmInstType)
{
#ifdef SOC_J721E
    if (esmInstType == SDR_ESM_INSTANCE_MAIN)
    {
        (void)SDR_ESM_registerECCHandler(esmInstType,
                                         &SDR_ECC_ESMCallBackFunction_MAIN);

    } else { /* MCU domain */
#endif
        (void)SDR_ESM_registerECCHandler(esmInstType,
                                         &SDR_ECC_ESMCallBackFunction_MCU);
        /* currently as this is not reported by ESM, register with
           exception handler */
        SDR_EXCEPTION_registerECCHandler(&SDR_ECC_callBackFunction);
#ifdef SOC_J721E
    }
#endif

    return SDR_PASS;
}

/** ============================================================================*
 *
 * \brief   Initializes ECC module for ECC detection
 *
 * \param1 eccAggrInstNumber Instance number of ECC aggregator
 * \param2 pECCInitConfig     Pointer to Ecc init configuration
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 *          NOTE: On failure the ECC peripheral registers may be initialized
 *          partially.
 */
SDR_Result SDR_ECC_init (SDR_ECC_MemType eccMemType,
                         const SDR_ECC_InitConfig_t *pECCInitConfig)
{
    CSL_ecc_aggrRegs *eccAggrRegs;
    uint32_t         numMemRegions;
    SDR_Result retVal = SDR_PASS;
    uint32_t ramId, ramIdType;
    uint32_t i;
    int32_t cslResult;
    SDR_ECC_MemSubType memSubType;
    uint32_t injectOnlyFlag;

    if (pECCInitConfig == NULL) {
        retVal = SDR_BADARGS;
    }

    if (retVal == SDR_PASS) {
#ifdef SOC_J721E
        if (eccMemType >= SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0)
        {
            /* Currently, only MAIN MSMC AGGR0 configuration registers can be
             * mapped by this code.  To expand to other aggregators, additional
             * instances of "mappedEccRegs" are needed and should be mapped to
             * those additional aggregators. */
            eccAggrRegs = &mappedEccRegs;
            retVal = SDR_ECC_mapEccAggrReg(eccMemType, &eccAggrRegs);
        } else {
            eccAggrRegs = (SDR_ECC_aggrBaseAddressTable[eccMemType]);
        }
#else
        eccAggrRegs = (SDR_ECC_aggrBaseAddressTable[eccMemType]);
#endif

        /* Disable all interrupts to start clean */
        (void)CSL_ecc_aggrDisableAllIntrs(eccAggrRegs);

        /* Get the number of RAMs */
        cslResult = CSL_ecc_aggrGetNumRams(eccAggrRegs, &numMemRegions);
        if ((cslResult != CSL_PASS) || (numMemRegions == 0U)) {
            retVal = SDR_FAIL;
        }
    }

    if (retVal == SDR_PASS) {
        /* Record the Init configuration */
        SDR_ECC_instance[eccMemType].eccInitConfig = *pECCInitConfig;

        /* Enable the single bit ECC interrupts */
        /* Note: The following statement enables interrupts for all RAMs */
        cslResult = CSL_ecc_aggrEnableIntrs(eccAggrRegs,
                                CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT);
        if (cslResult != CSL_PASS) {
            retVal = SDR_FAIL;
        }
    }

    if (retVal == SDR_PASS) {
            /* Enable the Double bit ECC Interrupts */
            cslResult = CSL_ecc_aggrEnableIntrs(eccAggrRegs,
                                CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT);
            if (cslResult != CSL_PASS) {
                retVal = SDR_FAIL;
            }
    }
    if (retVal == SDR_PASS) {
        /* Enable ECC */
        for ( i = ((uint32_t)0U); i < pECCInitConfig->numRams; i++) {

            /* Get memory Sub type to be configured */
             memSubType = pECCInitConfig->pMemSubTypeList[i];

             /* Get the corresponding ram Id */
             retVal = SDR_ECC_getRamId(eccMemType, memSubType, &ramId, &ramIdType);
             if (retVal == SDR_PASS) {

                 /* Get the corresponding ram Id */
                 retVal = SDR_ECC_getAggregatorType(eccMemType, memSubType, &injectOnlyFlag);
             }
             if (retVal == SDR_PASS) {
                 if (injectOnlyFlag) {
                     /* Call CSL APIs to enable ECC, specific to the module */
                     retVal = SDR_ECC_configECCRam(ramId);
                     if (retVal != SDR_PASS) {
                         retVal = SDR_FAIL;
                     }
                 } else {
                     if (ramIdType == SDR_ECC_RAM_ID_TYPE_INTERCONNECT)
                     {
                         /* Enables ECC, ecc checkmreg, rmw parity errors */
                         cslResult = CSL_ecc_aggrConfigEDCInterconnect(eccAggrRegs,
                                                  ramId, (bool)true);
                         if (cslResult != CSL_PASS) {
                             retVal = SDR_FAIL;
                         }                        
                     }
                     else
                     {
                         /* Enables ECC, ecc checkmreg, rmw parity errors */
                         cslResult = CSL_ecc_aggrConfigEccRam(eccAggrRegs,
                                                  ramId, (bool)true, (bool)true, (bool)true);
                         if (cslResult != CSL_PASS) {
                             retVal = SDR_FAIL;
                         }
                     }
                 }
             }
             if (retVal != SDR_PASS) {
                 break;
             }
        }

        /* Initialize object for self test */
        SDR_ECC_instance[eccMemType].eccErrorFlag = SDR_ECC_ERROR_FLAG_NONE;
        SDR_ECC_instance[eccMemType].eccSelfTestErrorType = SDR_INJECT_ECC_NO_ERROR;
        SDR_ECC_instance[eccMemType].eccSelfTestRamId = (SDR_ECC_INVALID_SELF_TEST_RAM_ID);
        SDR_ECC_instance[eccMemType].eccSelfTestAddr = NULL;
        SDR_ECC_instance[eccMemType].eccSelfTestcheckerType = SDR_ECC_INVALID_CHECKER_TYPE;
    }

    return retVal;
}

/** ============================================================================*
 *
 * \brief   Refresh memory to make sure ECC is generated
 *
 *
 * \param1  memAddr: Address to refresh ( Need to aligned to 32 bit)
 * \param2  size: Size of memory region to refresh
 *                Should be multiple of 4
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures

 */
static SDR_Result SDR_ECC_memoryRefresh(uint32_t *memAddr, size_t size)
{
    uint32_t i;
    SDR_Result result = SDR_PASS;
    volatile uint32_t tmpValue;

    /* Simply read and copy back data */
    for (i = ((uint32_t)0u); i < (size>>2u); i++) {
        tmpValue = memAddr[i];
        memAddr[i] = tmpValue;
    }
    return result;
}

/** ============================================================================
 *
 * \brief   Initializes Memory to be ready for ECC error detection.
 *          Assumes ECC is already enabled.
 *
 * \param  eccMemType ECC memory type
 * \param  memSubType: Memory subtype
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ECC_initMemory (SDR_ECC_MemType eccMemType,
                               SDR_ECC_MemSubType memSubType)
{
    SDR_Result result=SDR_PASS;
    SDR_MemConfig_t memConfig;
    uint32_t ramId, ramIdType;

    /* Get Ram Id to check if memSubType is of Wrapper RAM ID type */
    result = SDR_ECC_getRamId(eccMemType, memSubType, &ramId, &ramIdType);

    if ((result == SDR_PASS) && (ramIdType == SDR_ECC_RAM_ID_TYPE_WRAPPER))
    {
        /* Get memory configuration for memSubType of Wrapper RAM ID type */
        result = SDR_ECC_getMemConfig(eccMemType, memSubType, &memConfig);
        if (result == SDR_PASS) {

            /* Initialize only if readable */
            if (memConfig.readable) {
                /* Initialised the whole memory so that ECC is updated */
                result = SDR_ECC_memoryRefresh((uint32_t *)memConfig.memStartAddr,
                                           (size_t)memConfig.size);
            }
        }
    }
    /* If RAM ID was found, but ramIdType is SDR_ECC_RAM_ID_TYPE_INTERCONNECT,
     * then no initialization is required and success is returned */

    return result;
}

/** ============================================================================
 *
 * \brief   Does access to trigger ECC
 *
 * \param  pMemoryAccessAddr: Memory Access Address
 *
 * @return  None

 */
static void SDR_ECC_triggerAccessForEcc(const uint32_t *pMemoryAccessAddr)
{
    volatile uint32_t testLocationValue;


   /* Read value to trigger ECC error injection */
   testLocationValue = *(pMemoryAccessAddr);

}

/** ============================================================================
 *
 * \brief   Runs self test by injecting and error and monitor response
 *          Assumes ECC is already enabled.
 *
 * \param1  eccMemType ECC memory type
 * \param2  memSubType: Memory subtype
 * \param3  errorType: ECC Self test type
 * \param4  pECCErrorConfig: Memory configuration self test area
 * \param4  selfTestTimeOut: Number of retries before time out
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ECC_selfTest(SDR_ECC_MemType eccMemType,
                                  SDR_ECC_MemSubType memSubType,
                                  SDR_ECC_InjectErrorType errorType,
                                  const SDR_ECC_InjectErrorConfig_t *pECCErrorConfig,
                                  uint32_t selfTestTimeOut)
{
    SDR_Result retVal = SDR_PASS;
    uint32_t timeCount = 0;
    uint32_t testLocationPreserve;
    uint32_t ramId, ramIdType;
    uint32_t retVal2;
    uint32_t *testLocationAddress;
    SDR_MemConfig_t memConfig = {0};
    SDR_GrpChkConfig_t grpChkConfig = {0};

    if (pECCErrorConfig == NULL) {
        retVal = SDR_BADARGS;
    }

    if (retVal == SDR_PASS) {
        /* Configure error configuration based on Test type */
        /* Get Ram Id */
        retVal = SDR_ECC_getRamId(eccMemType, memSubType,
                                 &ramId, &ramIdType);
    }

    if (retVal == SDR_PASS) {

        if (ramIdType == SDR_ECC_RAM_ID_TYPE_WRAPPER) {
            /* Get memory configuration only for Wrapper RAM ID's */
            retVal = SDR_ECC_getMemConfig(eccMemType, memSubType, &memConfig);
        } else {
            /* Get EDC Checker group config */
            retVal = SDR_ECC_getEDCCheckerGroupConfig(eccMemType, memSubType,
                                                      pECCErrorConfig->chkGrp,
                                                      &grpChkConfig);
        }
    }

    if (retVal == SDR_PASS) {

        /* Get actual location address for the memory */
        testLocationAddress = pECCErrorConfig->pErrMem;

        if (memConfig.readable) {
            /* Store test location value */
            testLocationPreserve = *(testLocationAddress);
        }

        /* Set self test error flag */
        SDR_ECC_instance[eccMemType].eccErrorFlag = SDR_ECC_ERROR_FLAG_STARTING;
        SDR_ECC_instance[eccMemType].eccSelfTestErrorType = errorType;
        SDR_ECC_instance[eccMemType].eccSelfTestRamId = ramId;
        SDR_ECC_instance[eccMemType].eccSelfTestAddr = testLocationAddress;
        SDR_ECC_instance[eccMemType].eccSelfTestcheckerType = grpChkConfig.grpChkType;

        /* Enable PMU to monitor event about to be triggerred */
        SDR_ECC_enableECCEventCheck(eccMemType, memSubType, errorType);

        /* Inject error */
        retVal = SDR_ECC_injectError(eccMemType,
                                    memSubType, errorType,
                                    pECCErrorConfig);
    }

    if (retVal == SDR_PASS) {

        if (memConfig.readable) {
            /* Trigger access for ECC error injection to complete */
            SDR_ECC_triggerAccessForEcc(testLocationAddress);
        }

        /* Wait for error to take effect */
        while((SDR_ECC_instance[eccMemType].eccErrorFlag != SDR_ECC_ERROR_FLAG_TRIGGERED)
              && (timeCount < selfTestTimeOut)) {

            /* In cases there are no interrupts for the ECC event poll directly */
            retVal2 = SDR_ECC_pollErrorEvent(eccMemType, memSubType, errorType);
            if (retVal2 == SDR_ECC_EVENT_FOUND) {
                SDR_ECC_instance[eccMemType].eccErrorFlag = SDR_ECC_ERROR_FLAG_TRIGGERED;
            }

            /* Increment timeout counter */
            timeCount++;
        }

        /* Check expected error occurred or timeout */
        if ((SDR_ECC_instance[eccMemType].eccErrorFlag != SDR_ECC_ERROR_FLAG_TRIGGERED)) {
            retVal = SDR_FAIL;
        } else {

            if (memConfig.readable) {
                /* correct error injected if not autocorrect by hardware */
                *(testLocationAddress) = testLocationPreserve;
            }
        }

    }
    /* Disable any ECC event check */
    SDR_ECC_disableECCEventCheck(eccMemType, errorType);

    /* Reset self test error flag */
    SDR_ECC_instance[eccMemType].eccErrorFlag = SDR_ECC_ERROR_FLAG_NONE;
    SDR_ECC_instance[eccMemType].eccSelfTestErrorType = SDR_INJECT_ECC_NO_ERROR;
    SDR_ECC_instance[eccMemType].eccSelfTestRamId = (SDR_ECC_INVALID_SELF_TEST_RAM_ID);
    SDR_ECC_instance[eccMemType].eccSelfTestAddr = NULL;
    SDR_ECC_instance[eccMemType].eccSelfTestcheckerType = SDR_ECC_INVALID_CHECKER_TYPE;

    return retVal;
}

/** ============================================================================
 *
 * \brief   Get bit location in word.
 *
 * \param1  bitMask: Input bitmask
 * \param2  startBitLocation: Bit location to start from
 * \param3  pPbitLocation: Pointer to Next location of 1b found
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
static SDR_Result SDR_ECC_getBitLocation(uint32_t bitMask,
                                         uint32_t startBitLocation,
                                         uint32_t *pPbitLocation)
{
    SDR_Result result = SDR_PASS;
    uint32_t bitCount;

    if (startBitLocation >= BITS_PER_WORD) {
        result = SDR_FAIL;
    } else {
        /* Find first bit error for single bit */
         for (bitCount=startBitLocation; bitCount < BITS_PER_WORD; bitCount++) {
             if ((bitMask
                 & (((uint32_t)1u) << bitCount)) != 0u  ) {
                 *pPbitLocation = bitCount;
                 break;
             }
         }
         if ( bitCount >= BITS_PER_WORD) {
             result = SDR_FAIL;
         }
    }
    return result;
}

/** ============================================================================
 *
 * \brief   Injects ECC error at specified location
 *          Assumes ECC is already enabled.
 *
 * \param1  memType: Memory type for self test
 * \param2  memSubType: Memory subtype
 * \param3  errorType: ECC error type
 * \param4  pECCErrorConfig: Pointer to Error configuration
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ECC_injectError(SDR_ECC_MemType eccMemType,
                              SDR_ECC_MemSubType memSubType,
                              SDR_ECC_InjectErrorType errorType,
                              const SDR_ECC_InjectErrorConfig_t *pECCErrorConfig)
{
    uint32_t regValue;
    volatile uint32_t regValue2;
    uint32_t firstBitLocation, secondBitLocation;
    uint32_t errAddrOffset;
    CSL_Ecc_AggrEDCInterconnectErrorInfo eccErrInjInfo;
    CSL_ecc_aggrRegs *eccAggrRegs;
    uint32_t ramId, ramIdType = 0;
    SDR_Result retVal = SDR_PASS;
    int32_t cslRetval = CSL_EFAIL;
    SDR_MemConfig_t memConfig;
    SDR_GrpChkConfig_t grpChkConfig;

    if (pECCErrorConfig == NULL) {
        retVal = SDR_BADARGS;
    }

    if (retVal == SDR_PASS) {
        /* Based on ECC MemType (i.e. which aggregator), find the appropriate base address
         * for that ECC Aggegator. */
        retVal = SDR_ECC_getAggrBaseAddr(eccMemType, &eccAggrRegs);
    }

    if (retVal == SDR_PASS) {
        /* Get Ram Id */
        retVal = SDR_ECC_getRamId(eccMemType, memSubType,
                                 &ramId, &ramIdType);
    }

    if (ramIdType == SDR_ECC_RAM_ID_TYPE_WRAPPER) {
        if (retVal == SDR_PASS) {
            /* Get memory configuration */
            retVal = SDR_ECC_getMemConfig(eccMemType, memSubType, &memConfig);
        }

        if ((retVal == SDR_PASS) && (memConfig.readable == true))
        {
            if (retVal == SDR_PASS) {
                if ( ((uintptr_t)pECCErrorConfig->pErrMem) < memConfig.memStartAddr) {
                    retVal = SDR_FAIL;
                } else {
                    /* Calculate error offset */
                    errAddrOffset =  ((uintptr_t)pECCErrorConfig->pErrMem - memConfig.memStartAddr)
                                    / (memConfig.stride);
                }
            }

            if (retVal == SDR_PASS) {
                /* Set error Address in ECC Wrapper RAM ID */
                cslRetval = CSL_ecc_aggrWriteEccRamErrCtrlReg(eccAggrRegs,
                                                              ramId, 0u,
                                                              errAddrOffset);
                if (cslRetval != CSL_PASS) {
                    retVal = SDR_FAIL;
                }
            }
        }

        if (retVal == SDR_PASS) {
            /* Read ECC Ram Control Register */
            cslRetval = CSL_ecc_aggrReadEccRamCtrlReg(eccAggrRegs,
                                                     ramId, &regValue);
            if (cslRetval != CSL_PASS) {
                retVal = SDR_FAIL;
            }
        }

        if (retVal == SDR_PASS) {

            switch (errorType) {
                 case SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE:
                 case SDR_INJECT_ECC_ERROR_FORCING_1BIT_REPEAT:
                 case SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_ONCE:
                 case SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_REPEAT:
                     /* Get bit location */
                     retVal = SDR_ECC_getBitLocation(pECCErrorConfig->flipBitMask,
                                                     0u, &firstBitLocation);
                     if (retVal != SDR_PASS) {
                         break;
                     }

                     /* Write bit error configuration for single bit */
                     cslRetval = CSL_ecc_aggrWriteEccRamErrCtrlReg(eccAggrRegs,
                                                       ramId, 1, firstBitLocation);
                     if (cslRetval != CSL_PASS) {
                         retVal = SDR_FAIL;
                         break;
                     }

                     if ((errorType == SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE) ||
                         (errorType == SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_ONCE)){
                         /* Configure settings for inject error once  */
                         regValue |= CSL_ECC_RAM_CTRL_ERROR_ONCE_MASK;
                     } else {
                         /* Configure settings for inject error repeat */
                         regValue = (regValue & (~CSL_ECC_RAM_CTRL_ERROR_ONCE_MASK));
                     }
                     if ((errorType == SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE) ||
                         (errorType == SDR_INJECT_ECC_ERROR_FORCING_1BIT_REPEAT)){
                         /* Configure settings for single bit error, specific row */
                         regValue = (regValue
                                     & (~(CSL_ECC_RAM_CTRL_FORCE_N_ROW_MASK | CSL_ECC_RAM_CTRL_FORCE_DED_MASK)))
                                    | CSL_ECC_RAM_CTRL_FORCE_SEC_MASK;
                     } else {
                         /* Configure settings for Single bit error N Row */
                         regValue = (regValue
                                     & (~CSL_ECC_RAM_CTRL_FORCE_DED_MASK))
                                    | CSL_ECC_RAM_CTRL_FORCE_SEC_MASK
                                    | CSL_ECC_RAM_CTRL_FORCE_N_ROW_MASK;
                     }
                     break;

                 case SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE:
                 case SDR_INJECT_ECC_ERROR_FORCING_2BIT_REPEAT:
                 case SDR_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_ONCE:
                 case SDR_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_REPEAT:
                     /* Get bit location */
                     retVal = SDR_ECC_getBitLocation(pECCErrorConfig->flipBitMask,
                                                 0, &firstBitLocation);
                     if (retVal != SDR_PASS) {
                         break;
                     }

                     /* Get Second bit location */
                     retVal = SDR_ECC_getBitLocation(pECCErrorConfig->flipBitMask,
                                                 firstBitLocation+1u, &secondBitLocation);
                     if (retVal != SDR_PASS) {
                         break;
                     }

                     /* Record double bit error position */
                     regValue2 = firstBitLocation | (secondBitLocation << 16);

                     /* Set bit error configuration settings to register */
                     cslRetval = CSL_ecc_aggrWriteEccRamErrCtrlReg(eccAggrRegs,
                                               ramId, 1, regValue2);
                     if (cslRetval != CSL_PASS) {
                         retVal = SDR_FAIL;
                         break;
                     }

                     if ((errorType == SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE) ||
                             (errorType == SDR_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_ONCE)){
                         /* Configure settings for Double bit error */
                         regValue |= CSL_ECC_RAM_CTRL_ERROR_ONCE_MASK;
                     } else {
                         /* Configure settings for single bit error */
                         regValue = (regValue & (~CSL_ECC_RAM_CTRL_ERROR_ONCE_MASK));
                     }

                     if ((errorType == SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE) ||
                         (errorType == SDR_INJECT_ECC_ERROR_FORCING_2BIT_REPEAT)) {
                         /* Configure settings for double bit error, specific row */
                         regValue = (regValue
                                     & (~(CSL_ECC_RAM_CTRL_FORCE_N_ROW_MASK+CSL_ECC_RAM_CTRL_FORCE_SEC_MASK)))
                                    | CSL_ECC_RAM_CTRL_FORCE_DED_MASK;
                     } else {
                         /* Configure settings for Double bit error N Row*/
                         regValue = (regValue
                                     & (~CSL_ECC_RAM_CTRL_FORCE_SEC_MASK))
                                    | CSL_ECC_RAM_CTRL_FORCE_DED_MASK
                                    | CSL_ECC_RAM_CTRL_FORCE_N_ROW_MASK;
                     }
                     break;

                 default:
                     break;
             }
        }

        if (retVal == SDR_PASS) {
            /* Write bit error configuration to register */
            cslRetval = CSL_ecc_aggrWriteEccRamCtrlReg(eccAggrRegs,
                                                       ramId, regValue);
            if (cslRetval != CSL_PASS) {
                retVal = SDR_FAIL;
            }
        }

        if (retVal == SDR_PASS) {
            /* Just read back ctrl register to confirm write */
            /* NOTE: The read value may not be same as what is written as some fields
             * in the register are not writable or can self clear
             */
            cslRetval = CSL_ecc_aggrReadEccRamCtrlReg(eccAggrRegs,
                                                ramId,
                                                (uint32_t *)(&regValue2));
            if (cslRetval != CSL_PASS) {
                retVal = SDR_FAIL;
            }
        }
    } /* if (ramIdType == SDR_ECC_RAM_ID_TYPE_WRAPPER) {*/
    else if (ramIdType == SDR_ECC_RAM_ID_TYPE_INTERCONNECT) {

        if (retVal == SDR_PASS) {
            /* Get memory configuration */
            retVal = SDR_ECC_getEDCCheckerGroupConfig(eccMemType, memSubType,
                                                      pECCErrorConfig->chkGrp,
                                                      &grpChkConfig);
        }
        if (retVal == SDR_PASS) {
        
            eccErrInjInfo.eccGroup = pECCErrorConfig->chkGrp;

            eccErrInjInfo.bNextBit = FALSE;
            eccErrInjInfo.eccPattern = 2U;

            switch (errorType) {
                case SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE:
                    /* Get bit location */
                    retVal = SDR_ECC_getBitLocation(pECCErrorConfig->flipBitMask,
                                                    0u, &firstBitLocation);
                    if ((retVal != SDR_PASS)
                        || (firstBitLocation > grpChkConfig.dataWidth)){
                        retVal = SDR_FAIL;
                        break;
                    }

                    eccErrInjInfo.eccBit1  = firstBitLocation;
                    eccErrInjInfo.eccBit2  = 0u;
                    eccErrInjInfo.intrSrc  = CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT;
                    break;

                case SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE:
                    /* Get bit location */
                    retVal = SDR_ECC_getBitLocation(pECCErrorConfig->flipBitMask,
                                                     0u, &firstBitLocation);
                    if ((retVal != SDR_PASS)
                        || (firstBitLocation > grpChkConfig.dataWidth)){
                        retVal = SDR_FAIL;
                        break;
                    }
                    eccErrInjInfo.eccBit1  = firstBitLocation;

                    /* Get Second bit location */
                    retVal = SDR_ECC_getBitLocation(pECCErrorConfig->flipBitMask,
                                                    firstBitLocation+1u, &secondBitLocation);
                    if ((retVal != SDR_PASS)
                        || (secondBitLocation > grpChkConfig.dataWidth)){
                        retVal = SDR_FAIL;
                        break;
                    }
                    eccErrInjInfo.eccBit2  = secondBitLocation;
                    eccErrInjInfo.intrSrc  = CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT;
                    break;

                default:
                     retVal = SDR_FAIL;
                     break;
            }
        }

        if (retVal == SDR_PASS) {
 
            cslRetval = CSL_ecc_aggrForceEDCInterconnectError(eccAggrRegs,
                                                              ramId,
                                                              &eccErrInjInfo);
            if (cslRetval != CSL_PASS) {
                retVal = SDR_FAIL;
            }
        }
    } /* else if (ramIdType == SDR_ECC_RAM_ID_TYPE_INTERCONNECT) { */

    return retVal;
}

/** ============================================================================
 *
 * \brief   Get Ram Id for given memory and memory subtype
 *
 * \param1  eccMemType: Memory type for self test
 * \param2  memSubType: Memory subtype for self test
 * \param3  pRAMId: pointer to return Ram Id
 * \param4  pRAMIdType: pointer to return Ram Id Type
 *
 * @return  SDR_PASS : Success; SDR_FAIL for failures
 */
static SDR_Result SDR_ECC_getRamId(SDR_ECC_MemType eccMemType, SDR_ECC_MemSubType memSubType,
    uint32_t *pRAMId, uint32_t *pRAMIdType)
{
    SDR_Result retVal = SDR_PASS;

    retVal = SDR_ECC_checkMemoryType(eccMemType, memSubType);
    if (retVal == SDR_PASS) {
        /* Get ram Id from table for the particular aggregator */
        switch (eccMemType) {
            case SDR_ECC_MEMTYPE_MCU_R5F0_CORE:
                *pRAMId     = SDR_ECC_mcuArmssRamIdTable[memSubType].RAMId;
                *pRAMIdType = SDR_ECC_mcuArmssRamIdTable[memSubType].ramIdType;
                break;

#ifdef SOC_J721E
            case SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0:
                *pRAMId     = SDR_ECC_mainMsmcAggr0RamIdTable[memSubType].RAMId;
                *pRAMIdType = SDR_ECC_mainMsmcAggr0RamIdTable[memSubType].ramIdType;
                break;

            case SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0:
                *pRAMId     = SDR_ECC_mcuEccAggr0RamIdTable[memSubType].RAMId;
                *pRAMIdType = SDR_ECC_mcuEccAggr0RamIdTable[memSubType].ramIdType;
                break;
#endif

            default:
                retVal = SDR_FAIL;
                break;
        }
    }
    return retVal;
}

/** ============================================================================
 *
 * \brief   Check ECC check valid memory and memory subtype
 *
 * \param1  eccMemType: Memory type for self test
 * \param2  memSubType: Memory subtype for self test
 *
 * @return  SDR_PASS : Success; SDR_FAIL for failures
 */
static SDR_Result SDR_ECC_checkMemoryType(SDR_ECC_MemType eccMemType, SDR_ECC_MemSubType memSubType)
{
    SDR_Result retVal = SDR_PASS;

    switch (eccMemType) {
        case SDR_ECC_MEMTYPE_MCU_R5F0_CORE:
            /* Do bound check */
            if (((uint32_t)memSubType)
                    >= ((uint32_t)(sizeof(SDR_ECC_mcuArmssRamIdTable) /
                                   sizeof(SDR_ECC_mcuArmssRamIdTable[0])))) {
                retVal = SDR_FAIL;
            }
            break;

#ifdef SOC_J721E
        case SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0:
            /* Do bound check */
            if (((uint32_t)memSubType)
                    >= ((uint32_t)(sizeof(SDR_ECC_mainMsmcAggr0RamIdTable) /
                                   sizeof(SDR_ECC_mainMsmcAggr0RamIdTable[0])))) {
                retVal = SDR_FAIL;
            }
            break;

        case SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0:
            /* Do bound check */
            if (((uint32_t)memSubType)
                    >= ((uint32_t)(sizeof(SDR_ECC_mcuEccAggr0RamIdTable) /
                                   sizeof(SDR_ECC_mcuEccAggr0RamIdTable[0])))) {
                retVal = SDR_FAIL;
            }
            break;
#endif

        default:
            retVal = SDR_FAIL;
            break;
    }

    return retVal;
}

/** ============================================================================
 *
 * \brief   Get ECC aggregator type for given memory and memory subtype
 *
 * \param1  eccMemType: Memory type for self test
 * \param2  memSubType: Memory subtype for self test
 * \param3  pInjectOnly: pointer to variable indicating ECC aggregator
 *                       inject only
 *
 * @return  SDR_PASS : Success; SDR_FAIL for failures
 */
static SDR_Result SDR_ECC_getAggregatorType(SDR_ECC_MemType eccMemType,
                                            SDR_ECC_MemSubType memSubType,
                                            uint32_t *pIinjectOnly)
{
    SDR_Result retVal = SDR_PASS;

    retVal = SDR_ECC_checkMemoryType(eccMemType, memSubType);
    if (retVal == SDR_PASS) {
        switch (eccMemType) {
            case SDR_ECC_MEMTYPE_MCU_R5F0_CORE:
                *pIinjectOnly = SDR_ECC_mcuArmssRamIdTable[memSubType].aggregatorTypeInjectOnly;
                break;

#ifdef SOC_J721E
            case SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0:
                *pIinjectOnly = SDR_ECC_mainMsmcAggr0RamIdTable[memSubType].aggregatorTypeInjectOnly;
                break;

            case SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0:
                *pIinjectOnly = SDR_ECC_mcuEccAggr0RamIdTable[memSubType].aggregatorTypeInjectOnly;
                break;
#endif

            default:
                retVal = SDR_FAIL;
                break;
        }
    }

    return retVal;
}

/** ============================================================================
 *
 * \brief   Get ECC memory configuration for given memory subtype (only valid for
 *          Wrapper RAM ID's).
 *
 * \param1  memSubType: Memory subtype for which we are searching for memory
 *                      configuration
 * \param2  memEntryTable: Memory configuration table for all Wrapper RAM ID's
 *                         for a particular memType (i.e. ECC aggregator).
 * \param3  tableSize: Size of the memory configuration table to search

 * \param4  pMemConfig: pointer to memory configuration structure that will be
 *                      filled upon successful retrieval.
 *
 * @return  SDR_PASS : Success; SDR_FAIL for failures
 */
static SDR_Result SDR_ECC_searchMemEntryTable(SDR_ECC_MemSubType memSubType,
                                              const SDR_MemConfig_t memEntryTable[],
                                              uint32_t tableSize,
                                              SDR_MemConfig_t *pMemConfig)
{
    SDR_Result retVal = SDR_FAIL;
    uint32_t length = tableSize;
    uint32_t first, last, middle;

    /* Binary search, as the assumption is that memSubTypes in table are
     * in order */
    first = 0;
    last = length - 1;
    middle = (first + last) / 2;

    while (first <= last) {
        if (memEntryTable[middle].memSubType < memSubType) {
            first = middle + 1;
        } else if (memEntryTable[middle].memSubType == memSubType) {
            /* Fill the memory configuration structure */
            *pMemConfig = memEntryTable[middle];
            retVal = SDR_PASS;
            break;
        } else {
            last = middle - 1;
        }

        middle = (first + last) / 2;
    }
    if (first > last) {
        retVal = SDR_FAIL;
    }

    return retVal;
}

/** ============================================================================
 *
 * \brief   Get Memory configuration for given memory type  and memory subtype
 *
 * \param1  eccMemType: Memory type for self test
 * \param2  memSubType: Memory subtype for self test
 * \param3  pMemConfig: pointer to return memory configuration
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
static SDR_Result SDR_ECC_getMemConfig(SDR_ECC_MemType eccMemType, SDR_ECC_MemSubType memSubType,
                              SDR_MemConfig_t *pMemConfig)
{
    SDR_Result retVal = SDR_PASS;
    uint32_t tableSize;

    retVal = SDR_ECC_checkMemoryType(eccMemType, memSubType);

    if (retVal == SDR_PASS) {
        /* Get memory configuration from table */
        if (eccMemType == SDR_ECC_MEMTYPE_MCU_R5F0_CORE) {
            tableSize = sizeof(SDR_ECC_mcuArmssMemEntries) /
                        sizeof(SDR_ECC_mcuArmssMemEntries[0]);
            retVal = SDR_ECC_searchMemEntryTable(memSubType,
                                                 SDR_ECC_mcuArmssMemEntries,
                                                 tableSize,
                                                 pMemConfig);
#ifdef SOC_J721E
        } else if (eccMemType == SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0) {
            tableSize = sizeof(SDR_ECC_mainMsmcAggr0MemEntries) /
                        sizeof(SDR_ECC_mainMsmcAggr0MemEntries[0]);
            retVal = SDR_ECC_searchMemEntryTable(memSubType,
                                                 SDR_ECC_mainMsmcAggr0MemEntries,
                                                 tableSize,
                                                 pMemConfig);

        } else if (eccMemType == SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0) {
            tableSize = sizeof(SDR_ECC_MCUCBASSMemEntries) /
                        sizeof(SDR_ECC_MCUCBASSMemEntries[0]);
            retVal = SDR_ECC_searchMemEntryTable(memSubType,
                                                 SDR_ECC_MCUCBASSMemEntries,
                                                 tableSize,
                                                 pMemConfig);
#endif
        } else {
            retVal = SDR_FAIL;
        }
    }

   return retVal;
}

/** ============================================================================
 *
 * \brief   Get EDC Checker configuration given memory type, memory subtype
 *          and memory subsubtype
 *
 * \param1  eccMemType: Memory type for self test
 * \param2  memSubType: Memory subtype for self test
 * \param2  chkGrp:     Checker group for self test
 * \param3  grpChkConfig: pointer to Group checker configuration
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
static SDR_Result SDR_ECC_getEDCCheckerGroupConfig(SDR_ECC_MemType eccMemType,
                                                   SDR_ECC_MemSubType memSubType,
                                                   uint32_t chkGrp,
                                                   SDR_GrpChkConfig_t *grpChkConfig)
{
    SDR_Result retVal = SDR_PASS;

    retVal = SDR_ECC_checkMemoryType(eccMemType, memSubType);

    if (retVal == SDR_PASS) {
        /* Get EDC checker configuration from table */
        if (eccMemType == SDR_ECC_MEMTYPE_MCU_R5F0_CORE) {
            if((memSubType == SDR_ECC_R5F_MEM_SUBTYPE_VBUSM2AXI_EDC_VECTOR_ID)
               && (chkGrp < SDR_PULSAR_CPU_RAM_ID_VBUSM2_AXI_EDC_VECTOR_GRP_MAX_ENTRIES)) {
                *grpChkConfig = SDR_ECC_ramIdVbusM2AxiEdcVectorGrpEntries[chkGrp];
            } else
            {
                retVal = SDR_FAIL;
            }

#ifdef SOC_J721E
        } else if (eccMemType == SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0) {
            if((memSubType == SDR_ECC_MAIN_MSMC_MEM_INTERCONN_SUBTYPE)
               && (chkGrp < SDR_MSMC_AGGR0_RAM_ID_MSMC_MMR_BUSECC_GRP_MAX_ENTRIES)) {
                *grpChkConfig = SDR_ECC_ramIdMsmcMMRBuseccGrpEntries[chkGrp];
            } else
            {
                retVal = SDR_FAIL;
            }
       } else if (eccMemType == SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0) {
            if((memSubType == SDR_ECC_MCU_CBASS_MEM_SUBTYPE_EDC_CTRL_ID)
               && (chkGrp < CSL_MCU_CBASS_ECC_AGGR_IMCU_COR_FW_GRP_MAX_ENTRIES )) {
                *grpChkConfig = SDR_ECC_MCU_CBASS_ramId2GrpEntries[chkGrp];
            } else
            {
                retVal = SDR_FAIL;
            }
#endif
        } else {
            retVal = SDR_FAIL;
        }
    }

   return retVal;
}

/** ============================================================================
 *
 * \brief   Get Ecc Aggregator Base Address for given memory type
 *
 * \param1  eccMemType: Memory type for self test
 * \param2  pEccAggr: pointer to Ecc Aggregator address
 *
 * @return  SDR_PASS : Success; SDR_FAIL for failures
 */
static SDR_Result SDR_ECC_getAggrBaseAddr(SDR_ECC_MemType eccMemType, CSL_ecc_aggrRegs **pEccAggr)
{
    SDR_Result retVal = SDR_PASS;

#ifdef SOC_AM65XX
    if ((eccMemType == SDR_ECC_MEMTYPE_MCU_R5F0_CORE) 
        || (eccMemType == SDR_ECC_MEMTYPE_MCU_PSRAM0)){
        /* Note in the SDR_ECC_aggrBaseAddressTable only the above are
         * supported currently
         */
        *pEccAggr = SDR_ECC_aggrBaseAddressTable[eccMemType];
    }
#endif

#ifdef SOC_J721E
    if ((eccMemType == SDR_ECC_MEMTYPE_MCU_R5F0_CORE) 
        || (eccMemType == SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0)){
        /* Note in the SDR_ECC_aggrBaseAddressTable only the above are
         * supported currently
         */
        *pEccAggr = SDR_ECC_aggrBaseAddressTable[eccMemType];

    } else if ((eccMemType >= SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0) && 
               (eccMemType <= SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR2)) {
        *pEccAggr = SDR_ECC_aggrHighBaseAddressTableTrans[eccMemType - \
                                       SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0];
    }
#endif
    else {
        retVal = SDR_FAIL;
    }

    return retVal;
}
