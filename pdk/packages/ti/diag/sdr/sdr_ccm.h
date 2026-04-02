/*
 * SDR CCM
 *
 * Software Diagnostics Reference module for CPU Comparator Module
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
 * @file  sdr_ccm.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDR CPU Comparator Module interface.
 *
 *  The SDR CCM enumerations include:
 *      1. SDR CCM monitor type
 *      2. SDR CCM self test type (normal or force error)
 *
 *  The SDR CCM data structures include:
 *      1. SDR CCM error status structure
 *
 *  The SDR CCM APIs include:
 *      1. API to initialize the SDR CCM
 *      2. API to inject a forced error on SDR CCM
 *      3. API to execute self test on SDR CCM
 *      4. API to execute Self test on SDR CCM by inverting the polarity
 *      5. Application provided callback function for CCM error handling
 */

#ifndef INCLUDE_SDR_CCM_H_
#define INCLUDE_SDR_CCM_H_

#include <stdint.h>
#include <stdbool.h>

#include "sdr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDR_CCM_API SDR CCM API
 *
 * ============================================================================
 */
/**
@defgroup SDR_CCM_DATASTRUCT  SDR CCM Data Structures
@ingroup SDR_CCM_API
*/
/**
@defgroup SDR_CCM_FUNCTION  SDR CCM Functions
@ingroup SDR_CCM_API
*/
/**
@defgroup SDR_CCM_ENUM SDR CCM Enumerated Data Types
@ingroup SDR_CCM_API
*/

/** ===========================================================================
 *  @addtogroup SDR_CCM_ENUM
    @{
 * ============================================================================
 */

/** ---------------------------------------------------------------------------
 * \brief Defines the different CCM monitor types
 *
 * ---------------------------------------------------------------------------
 */
typedef enum {
   SDR_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK = 1,
    /**<  CCM monitor type Output compare block*/
   SDR_CCM_MONITOR_TYPE_VIM = 2,
    /**<  CCM monitor type VIM */
  SDR_CCM_MONITOR_TYPE_INACTIVITY_MONITOR = 3,
   /**<  CCM monitor type inactivity monitor */
} SDR_CCM_MonitorType;

/** ---------------------------------------------------------------------------
 * \brief Types of self test for CCM module
 *
 * ---------------------------------------------------------------------------
 */
typedef enum {
   SDR_CCM_SELFTEST_TYPE_NORMAL = 1,
    /**<  Self Test type normal */
   SDR_CCM_SELFTEST_TYPE_ERR_FORCING = 2,
    /**<  Self Test type Error forcing */
} SDR_CCM_SelfTestType;

/* @} */

/**
 *  \addtogroup SDR_CCM_DATASTRUCT
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief Structure for CCM error status
 *
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_CCM_ErrorStatus_s
{
    bool compareErrorFlag;
    /**< CCM compare error flag: true indicates error */
    bool selfTestErrorFlag;
    /**< CCM self test error flag: true indicates error */
    bool selfTestErrorTypeFlag;
    /**< CCM self test error type flag: true indicates error */

} SDR_CCM_ErrorStatus_t;

/* @} */

/**
 *  \addtogroup SDR_CCM_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief   Initializes CCM module
 *
 * \param  pInitConfig Init configuration structure pointer
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 *          NOTE: In case of SDR_FAIL the peripheral may be configured partially
 */
SDR_Result SDR_CCM_init (void);

/** ============================================================================
 *
 * \brief   Injects forced error on CCM
 *
 * \param   monitorType: Type of monitor to inject error
 *
 * \pre     SDR_CCM_init should be called before calling this function
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_CCM_injectError (SDR_CCM_MonitorType monitorType);

/** ============================================================================
 *
 * \brief   Execute Self test on CCM
 *          NOTE: SDR_CCM_init should be called before calling this function
 *
 * \param   monitorType: Monitor Type
 * \param   selfTestType: Self test type
 * \param   maxCheckCount: max count to check for self test
 *                         completion ( 0 means indefinite)
 * \param   pErrorStatus: Pointer to Error status structure to report
 *                        error status
 *
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_CCM_selfTest (SDR_CCM_MonitorType monitorType,
                             SDR_CCM_SelfTestType selfTestType,
                             uint32_t maxCheckCount,
                             SDR_CCM_ErrorStatus_t *pErrorStatus);

/** ============================================================================
 *
 * \brief   Execute Self test on CCM by inverting the polarity
 *
 * \param   polarityInversionMask: Mask used to invert polarity of
 *            selected signals. ( Meaning of signals refer to hardware
 *            manuals)
 * \param   maxCheckCount: Maximum count to recheck status of error trigger
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_CCM_selfTestPolarityInvert (uint32_t polarityInversionMask,
                                           uint32_t maxCheckCount);

/** ============================================================================
 *
 * \brief   Application provided external callback function for CCM error handling
 *          Called inside the reference functions when CCM errors occur.
 *          NOTE: This is application supplied and not part of the SDR
 *          If not supplied by application this will result in an linker error
 *
 * \param  monitorType: Error source for the ECC error event.
 * \param  address: Address at which the ECC error occurred.
 *
 *
 * \return  None
 */
void SDR_CCM_applicationCallbackFunction(SDR_CCM_MonitorType monitorType,
                                         SDR_CCM_ErrorStatus_t *pErrorStatus);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDR_CCM_H_ */
