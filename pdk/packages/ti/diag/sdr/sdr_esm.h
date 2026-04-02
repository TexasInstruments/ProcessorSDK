/*
 * SDR ESM
 *
 * Software Diagnostics Reference module for Error Signaling Module
 *
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
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
 * @file  sdr_esm.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDR Error Signaling Module interface.
 *
 *  The SDR ESM enumerations include:
 *      1. SDR ESM interrupt types
 *      2. SDR ESM interrupt sources
 *      3. SDR ESM Watchdog Timer IDs
 *
 *  The SDR ESM function macros include:
 *      1. Application provided callback function type for ECC/CCM/WDT specific ESM events
 *
 *  The SDR ESM data structures include:
 *      1. Structure of the ECC error sources which map to the ESM interrupt sources
 *      2. Structure of the ESM error configuration
 *      3. Structure of the initial ESM configuration
 *
 *  The SDR ESM APIs include:
 *      1. API to initialize the SDR ESM
 *      2. APIs to set/reset nERROR pin
 *      3. API to get nERROR pin status
 *      4. API to insert an ESM error
 *      5. API to execute self test of ESM module
 *      6. APIs to register handler for ECC/CCM with ESM
 *      7. APIs to register/de-register handler for WDT with ESM
 *      8. APIs to handle ESM high priority/low prioirty/config interrupts
 *      9. API to get ESM Interrupt Number corresponding to the input ESM interrupt type
 *      10.Application provided external callback function for ESM handling
 */

#ifndef INCLUDE_SDR_ESM_H_
#define INCLUDE_SDR_ESM_H_

#include <stdint.h>
#include <stdbool.h>

#include "sdr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDR_ESM_API SDR ESM API
 *
 * @section Overview
 * The SDR ESM module provides API to configure ESM events
 *
 * ============================================================================
 */
/**
@defgroup SDR_ESM_DATASTRUCT  SDR ESM Data Structures
@ingroup SDR_ESM_API
*/
/**
@defgroup SDR_ESM_FUNCTION  SDR ESM Functions
@ingroup SDR_ESM_API
*/
/**
@defgroup SDR_ESM_ENUM SDR ESM Enumerated Data Types
@ingroup SDR_ESM_API
*/

/**
@defgroup SDR_ESM_MACROS SDR ESM Macro defines
@ingroup SDR_ESM_API
*/

/** ===========================================================================
 *  @addtogroup SDR_ESM_ENUM
    @{
 * ============================================================================
 */
/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the values for ESM interrupt types
 * ----------------------------------------------------------------------------
 */
typedef enum {
   SDR_ESM_INT_TYPE_HI = 1,
    /**< Interrupt type Hi  */
   SDR_ESM_INT_TYPE_LO = 2,
    /**< Interrupt type Lo  */
   SDR_ESM_INT_TYPE_CFG = 3,
    /**< Interrupt type Config  */
} SDR_ESM_IntType;

/** ---------------------------------------------------------------------------
 * \brief Defines the different SDR ESM interrupt sources
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_ESM_CCM_OUTPUT_COMPARE_BLOCK_INT = 1,
    /**<  CCM Interrupt source  Output compare block*/
    SDR_ESM_CCM_VIM_ERR_INT = 2,
    /**<  CCM Interrupt source VIM compare */
    SDR_ESM_CCM_INACTIVITY_MONITOR_INT = 3,
   /**<  CCM Interrupt source inactivity monitor */
    SDR_ESM_CCM_SELF_TEST_ERR_INT = 4,
   /**<  CCM Interrupt source Self test error */
    SDR_ESM_CCM_STAT_ERR_INT = 5,
   /**<  CCM Interrupt source self test or split mode interrupt */
} SDR_ESM_CCM_IntSrc;


/** ---------------------------------------------------------------------------
 * \brief Defines the different Timer id events
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_ESM_TIMER_ID_0 = 1,
    /**<  Timer Id 0*/
    SDR_ESM_TIMER_ID_1 = 2,
    /**<  Timer Id 1 */
} SDR_ESM_WDT_IntSrc;

/** ---------------------------------------------------------------------------
 * \brief Defines the different ESM instance types
 * ----------------------------------------------------------------------------
 */
typedef enum {
   SDR_ESM_INSTANCE_MCU = 1,
    /**< MCU_ESM0 instance  */
   SDR_ESM_INSTANCE_WKUP = 2,
    /**< WKUP_ESM0 instance */
   SDR_ESM_INSTANCE_MAIN = 3,
    /**< ESM0 (Main domain) instance  */
   SDR_ESM_INSTANCE_MAX=0xFFFF
} SDR_ESM_InstanceType;

/* @} */

/**
 *  \addtogroup SDR_ESM_MACROS
 *  @{
 */


/** \brief Invalid interrupt number */
#define SDR_ESM_INTNUMBER_INVALID (0xffffffffu)

/** \brief Invalid Esm Instance */
#define SDR_ESM_INST_INVALID      (0xfffffffeu)

/** \brief Address field: Error Address invalid */
#define SDR_ESM_ERRORADDR_INVALID (0xffffffffu)

/** \brief Address field: Error RAM ID invalid */
#define SDR_ESM_ERRORRAMID_INVALID (0xffffffffu)

/** \brief Address field: Error Bit Error Offset invalid */
#define SDR_ESM_ERRORBITOFFSET_INVALID (0xffffffffffffffffu)

/** \brief Address field: Error Bit Error Group invalid */
#define SDR_ESM_ERRORBITGROUP_INVALID (0xffffffffu)

/** \brief Maximum number of EVENT words */
#define SDR_ESM_MAX_EVENT_MAP_NUM_WORDS (32u)

/**
 *  \anchor sdrEsmEccErrorSource_t
 *  \name ESM ECC Error source type
 *  @{
 */
/**
 * \brief  Source of the ECC error which maps to the ESM interrupt source
 */
typedef uint32_t sdrEsmEccErrorSource_t;

/** \brief MCU CPU0 detected 1-bit ECC error source */
#define SDR_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR (1u)
/** \brief MCU CPU0 detected 2-bit ECC error source */
#define SDR_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR (2u)
/** \brief MCU CPU1 detected 1-bit ECC error source */
#define SDR_ESM_ECC_PARAM_MCU_CPU1_SEC_ERROR (3u)
/** \brief MCU CPU1 detected 2-bit ECC error source */
#define SDR_ESM_ECC_PARAM_MCU_CPU1_DED_ERROR (4u)
/** \brief MCU CBASS detected 1-bit ECC error source */
#define SDR_ESM_ECC_PARAM_MCU_CBASS_SEC_ERROR (5u)
/** \brief MCU CBASS detected 2-bit ECC error source */
#define SDR_ESM_ECC_PARAM_MCU_CBASS_DED_ERROR (6u)


/** \brief Main MSMC ECC AGGR0 detected 1-bit ECC error source */
#define SDR_ESM_ECC_PARAM_MAIN_MSMC_AGGR0_SEC_ERROR (10001u)
/** \brief Main MSMC ECC AGGR0  detected 2-bit ECC error source */
#define SDR_ESM_ECC_PARAM_MAIN_MSMC_AGGR0_DED_ERROR (10002u)
/** \brief Main A72 ECC AGGR0 detected 1-bit ECC error source */
#define SDR_ESM_ECC_PARAM_MAIN_A72_AGGR0_SEC_ERROR (10003u)
/** \brief Main A72 ECC AGGR0 detected 2-bit ECC error source */
#define SDR_ESM_ECC_PARAM_MAIN_A72_AGGR0_DED_ERROR (10004u)

/* @} */

/* @} */

/**
 *  \addtogroup SDR_ESM_DATASTRUCT
 *  @{
 */

/** ============================================================================
 *
 * \brief   Call back function for ECC specific ESM events
 *
 * \param   intSource: Interrupt source
 * \param   errorAddr: Error Address where ECC error occurred 
 *
 * \return  None
 */
typedef void (* SDR_ESM_ECCCallback_t) (uint32_t intSource, uint32_t errorAddr);

/** ============================================================================
 *
 * \brief   Call back function for CCM specific ESM events
 *
 * \param   intSrc: Interrupt source
 *
 * \return  None
 */
typedef void (* SDR_ESM_CCMCallback_t) (SDR_ESM_CCM_IntSrc intSrc);

/** ============================================================================
 *
 * \brief   Call back function for WDT specific ESM events
 *
 * \param   intSrc:  Interrupt source for WDT events
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
typedef bool (* SDR_ESM_WDTCallback_t) (SDR_ESM_WDT_IntSrc intSrc);

/** ---------------------------------------------------------------------------
 * \brief ESM error configuration
 *
 * This structure defines the elements ESM error configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_ESM_Errorconfig_s
{
    uint32_t groupNumber;
    /**< Group number of error event  */
    uint32_t bitNumber;
    /**< Bit number within the group  */
} SDR_ESM_ErrorConfig_t;

/** ---------------------------------------------------------------------------
 * \brief ESM init configuration
 *
 * This structure defines ESM Init configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_ESM_InitConfig_s
{
    SDR_ESM_ErrorConfig_t esmErrorConfig;
    /**< Error event to be used for self test */
    uint32_t eventMap[SDR_ESM_MAX_EVENT_MAP_NUM_WORDS];
    /**< ESM Event bitmap */
    uint32_t eventPriorityMap[SDR_ESM_MAX_EVENT_MAP_NUM_WORDS];
    /**< ESM Event Priority bitmap */
    uint32_t errorOutputMap[SDR_ESM_MAX_EVENT_MAP_NUM_WORDS];
    /**< ESM bitmap for driving error pin: When selected error event occurs
     *  the error output pin will be asserted
     *  It is the application responsibility to reset the error
     *  if the system did not crash or lockup */
} SDR_ESM_InitConfig_t;

/* @} */

/**
 *  \addtogroup SDR_ESM_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief   Initializes ESM module for SDR
 *
 * \param   esmInstType: Instance of ESM
 * \param   esmInitConfig: Configuration for ESM
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_ESM_init (const SDR_ESM_InstanceType esmInstType,
                         const SDR_ESM_InitConfig_t *esmInitConfig);

/** ============================================================================ 
 *
 * \brief   Function sets the nERROR pin active.
 *
 * \param   esmInstType: Instance of ESM
 * \return  None
 */
void SDR_ESM_setNError(const SDR_ESM_InstanceType esmInstType);

/** ============================================================================
 *
 * \brief   Function sets the nERROR pin inactive.
 *
 * \param   esmInstType: Instance of ESM
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_ESM_resetNError(const SDR_ESM_InstanceType esmInstType);

/** ============================================================================
 *
 * \brief   Function returns the status of nERROR pin
 *
 * \param   esmInstType: Instance of ESM
 *
 * \return    true : active; false not active
 */
bool SDR_ESM_getNErrorStatus(const SDR_ESM_InstanceType esmInstType);

/** ============================================================================
 *
 * \brief   Insert ESM error
 *
 * \param   esmInstType: Instance of ESM
 * \param   esmErrorConfig: Configuration of ESM error
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_ESM_errorInsert(const SDR_ESM_InstanceType esmInstType,
                               const SDR_ESM_ErrorConfig_t *esmErrorConfig);

/** ============================================================================
 *
 * \brief   Execute self test of ESM module
 *
 * \param   esmInstType: Instance of ESM
 * \param   loopCount: Number of iterations to check status before timing out
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_ESM_selfTest (const SDR_ESM_InstanceType esmInstType,
                             uint32_t loopCount);

/** ============================================================================
 *
 * \brief   Register handler for ECC with ESM
 *
 * \param   esmInstType: Instance of ESM
 * \param   eccCallBackFunctionPtr: Call back function to register
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_ESM_registerECCHandler(const SDR_ESM_InstanceType esmInstType,
                                      SDR_ESM_ECCCallback_t eccCallBackFunctionPtr);


/** ============================================================================
 *
 * \brief   Register callback function for CCM errrors
 *
 * \param   esmInstType: Instance of ESM
 * \param   CCMCallBackFunctionPtr: Callback function pointer
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ESM_registerCCMHandler(const SDR_ESM_InstanceType esmInstType,
                                      SDR_ESM_CCMCallback_t CCMCallBackFunctionPtr);


/** ============================================================================
 *
 * \brief   Register callback function for WDT events
 *
 * \param   esmInstType: Instance of ESM
 * \param   WDTCallBackFunctionPtr: Callback function pointer
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ESM_registerWDTHandler(const SDR_ESM_InstanceType esmInstType,
                                      SDR_ESM_WDTCallback_t WDTCallBackFunctionPtr);

/** ============================================================================
 *
 * \brief   De-Register callback function for WDT events
 *
 * \param   esmInstType: Instance of ESM
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ESM_deRegisterWDTHandler(const SDR_ESM_InstanceType esmInstType);

/** ============================================================================
 *
 * \brief   Esm Hi Interrupt Handler for MCU Esm Instance
 *
 * \param  arg: argument for handler
 *
 * \return  None
 */
void SDR_ESM_hiInterruptHandler_MCU (uintptr_t arg);

/** ============================================================================
 *
 * \brief   Esm Hi Interrupt Handler for WKUP Esm Instance
 *
 * \param  arg: argument for handler
 *
 * \return  None
 */
void SDR_ESM_hiInterruptHandler_WKUP (uintptr_t arg);

/** ============================================================================
 *
 * \brief   Esm Hi Interrupt Handler for MAIN Esm Instance
 *
 * \param  arg: argument for handler
 *
 * \return  None
 */
void SDR_ESM_hiInterruptHandler_MAIN (uintptr_t arg);

/** ============================================================================
 *
 * \brief   Esm Lo Interrupt Handler for MCU Esm Instance
 *
 * \param  arg: argument for handler
 *
 * \return  None
 */
void SDR_ESM_loInterruptHandler_MCU (uintptr_t arg);

/** ============================================================================
 *
 * \brief   Esm Lo Interrupt Handler for WKUP Esm Instance
 *
 * \param  arg: argument for handler
 *
 * \return  None
 */
void SDR_ESM_loInterruptHandler_WKUP (uintptr_t arg);

/** ============================================================================
 *
 * \brief   Esm Lo Interrupt Handler for MAIN Esm Instance
 *
 * \param  arg: argument for handler
 *
 * \return  None
 */
void SDR_ESM_loInterruptHandler_MAIN (uintptr_t arg);

/** ============================================================================
 *
 * \brief   Esm Config Interrupt Handler for MCU Instance
 *
 * \param1  arg: argument for handler
 *
 * \return  None
 */
void SDR_ESM_configInterruptHandler_MCU(uintptr_t arg);

/** ============================================================================
 *
 * \brief   Esm Config Interrupt Handler for WKUP Instance
 *
 * \param1  arg: argument for handler
 *
 * \return  None
 */
void SDR_ESM_configInterruptHandler_WKUP(uintptr_t arg);

/** ============================================================================
 *
 * \brief   Esm Config Interrupt Handler for MAIN Instance
 *
 * \param1  arg: argument for handler
 *
 * \return  None
 */
void SDR_ESM_configInterruptHandler_MAIN(uintptr_t arg);

/** ============================================================================
 *
 * \brief   Esm get Interrupt Number corresponding to the
 *          input interrupt type
 *
 * \param   esmInstType: Instance of ESM
 * \param   esmIntType: ESM Interrupt type
 *
 * \return  Interrupt Number or SDR_ESM_INTNUMBER_INVALID error
 */
uint32_t SDR_ESM_getIntNumber(const SDR_ESM_InstanceType esmInstType,
                              SDR_ESM_IntType esmIntType);

/** ============================================================================
 *
 * \brief   Application provided external callback function for ESM handling
 *          Called inside the reference functions when ESM error events occur.
 *          NOTE: This is application supplied and not part of the SDR
 *          If not supplied by application this will result in an linker error
 *
 * \param  esmInstType: Instance of ESM
 * \param  esmIntType: ESM Interrupt type
 * \param  grpChannel: Group channel number of the event source
 * \param  index: index within the group of the event source
 * \param  instSrc: Interrupt source number which is the esm event number
 *
 *
 * \return  None
 */
void SDR_ESM_applicationCallbackFunction(SDR_ESM_InstanceType esmInstType,
                                         SDR_ESM_IntType esmIntType,
                                         uint32_t grpChannel,
                                         uint32_t index,
                                         uint32_t intSrc);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDR_ESM_H_ */
