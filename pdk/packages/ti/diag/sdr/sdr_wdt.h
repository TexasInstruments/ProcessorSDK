/*
 * SDR WDT
 *
 * Software Diagnostics Reference module for Watch Dog Timer module
 *
 *  Copyright (c) Texas Instruments Incorporated 2019-2020
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
 * @file  sdr_wdt.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDR WDT interface.
 *  ============================================================================
 */

#ifndef INCLUDE_SDR_WDT_H_
#define INCLUDE_SDR_WDT_H_

#include "sdr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDR_WDT_API SDR WDT API
 *
 * @section Overview
 * The SDR WDT module provides API for WatchDog Timer Diagnostics
 *
 * ============================================================================
 */
/**
@defgroup SDR_WDT_DATASTRUCT  SDR WDT Data Structures
@ingroup SDR_WDT_API
*/
/**
@defgroup SDR_WDT_FUNCTION  SDR WDT Functions
@ingroup SDR_WDT_API
*/
/**
@defgroup SDR_WDT_ENUM SDR WDT Enumerated Data Types
@ingroup SDR_WDT_API
*/

/**
@defgroup SDR_WDT_MACROS SDR WDT Macro defines
@ingroup SDR_WDT_API
*/

/** ===========================================================================
 *  @addtogroup SDR_WDT_ENUM
    @{
 * ============================================================================
 */

/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the different Watchdog Timer Ids
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_WDT_TIMER_0 = 1,
    /**<  Timer id 0 */
    SDR_WDT_TIMER_1 = 2,
    /**<  Timer id 1 */
} SDR_WDT_TimerId;

/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the different Watchdog window sizes
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_WDT_WINDOWSIZE_100_PERCENT = 1,
    /**<  Watchdog window size 100 % */
    SDR_WDT_WINDOWSIZE_50_PERCENT = 2,
    /**<  Watchdog window size 50 % */
    SDR_WDT_WINDOWSIZE_25_PERCENT = 3,
    /**<  Watchdog window size 25 % */
    SDR_WDT_WINDOWSIZE_12_5_PERCENT = 4,
    /**<  Watchdog window size 12.5 % */
    SDR_WDT_WINDOWSIZE_6_25_PERCENT = 5,
    /**<  Watchdog window size 6.25 % */
    SDR_WDT_WINDOWSIZE_3_125_PERCENT = 6,
    /**<  Watchdog window size 3.125 % */
} SDR_WDT_WindowSize;

/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the type of test
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_WDT_TEST_TYPE_END_VIOLATION = 1,
    /**<  Test type End Violation */
    SDR_WDT_TEST_TYPE_WINDOW_VIOLATION = 2,
    /**<  Test type window Violation */
} SDR_WDT_TestType;

/* @} */

/**
 *  \addtogroup SDR_WDT_MACROS
 *  @{
 */


/* @} */

/**
 *  \addtogroup SDR_WDT_DATASTRUCT
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief WDT Timer configuration
 *
 * This structure defines the elements of the Timer configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_WDT_TimerConfig_s
{
    uint64_t timerPeriod;
    /**< Watchdog timer period in timestamp units */
    SDR_WDT_TimerId timerId;
    /**< Unique Timer identifier  */
    SDR_WDT_WindowSize windowSize;
    /**< Window size  */
} SDR_WDT_TimerConfig_t;

/** ---------------------------------------------------------------------------
 * \brief Watchdog timer test configuation
 *
 * This structure defines the elements of the test configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_WDT_TestConfig_s
{
    uint64_t timeoutPeriod;
    /**< Watchdog time out period in timestamp units  */
    SDR_WDT_TestType testType;
    /**< Test type  */
} SDR_WDT_TestConfig_t;

/* @} */

/**
 *  \addtogroup SDR_WDT_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief  Runs diagnostics on configured Watchdog Timer
 *
 * \param  pTimerConfig:  Pointer to timer configuration
 * \param  pTestConfig:   Pointer to test configuration
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_WDT_selftest (const SDR_WDT_TimerConfig_t *pTimerConfig,
                             const SDR_WDT_TestConfig_t *pTestConfig);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDR_WDT_H_ */
