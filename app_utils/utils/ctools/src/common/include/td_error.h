/**
* @file td_error.h
* @brief Driver error handling definitions.
*
* Copyright (C) 2025 Texas Instruments Incorporated - https://www.ti.com/
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*  Redistributions of source code must retain the above copyright 
*  notice, this list of conditions and the following disclaimer.
*
*  Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the 
*  documentation and/or other materials provided with the   
*  distribution.
*
*  Neither the name of Texas Instruments Incorporated nor the names of
*  its contributors may be used to endorse or promote products derived
*  from this software without specific prior written permission.
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

#ifndef __TD_ERROR_H
#define __TD_ERROR_H

#include <stdlib.h> //size_t common A15

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief IP driver error definitions.
*
* Any updates to the arrangement (appending to the enum exluded) of the
* _td_error_t enum will require a bump to the API major version.
*/
typedef enum
{
    /* Common errors */

    /**< No error */
    e_ERR_NONE = 0,
    /**< Hardware does not match intended software */
    e_ERR_BAD_HWTYPE = 1,
    /**< Bad parameter passed to IP driver */
    e_ERR_BAD_PARAM = 2,
    /**< Hardware could not be programmed */
    e_ERR_PROGRAM = 3,
    /**< Register/Memory read failure */
    e_ERR_READ = 4,
    /**< Register/Memory write failure */
    e_ERR_WRITE = 5,
    /**< IP driver not open */
    e_ERR_NOT_OPEN = 6,
    /**< Memory allocation error */
    e_ERR_ALLOC = 7,
    /**< Ownership error */
    e_ERR_OWNERSHIP = 8,
    /**< IP locked */
    e_ERR_LOCKED = 9,
    /* C66_TRC errors */
    e_ERR_AET_TRC_TCU = 10,
    /**< TCU_CNTL programming error */
    e_ERR_AET_TRC_ADTF = 11,
    /**< ADTF programming error */
    e_ERR_AET_TRC_BAD_FUNC_MODE = 12,
    /**< Invalid C66 function mode */
    e_ERR_AET_TRC_BAD_TEST_PATTERN = 13,
    /**< Port width exceeds 5-bit capacity */
    e_ERR_AET_TRC_BAD_PORT_WIDTH = 14,
    /**< Invalid programming for chip pin */
    e_ERR_AET_TRC_PIN_PGM = 15,
    /* CSETB errors */

    /**< Requested trigger count is greater than CSETB RAM depth. */
    e_ERR_CSETB_BAD_TRIG_CNT = 16,

    /**< AET errors */
    /**< NULL descriptor passed to the function */
    e_ERR_AET_NULL_DESCRIPTOR = 17,
	/**< Precise Breakpoint failed */
	e_ERR_AET_UNABLE_TO_DEFINE_PRECISE_HWBP = 18,
    /**< State event search failed */
    e_ERR_AET_UNABLE_TO_DEFINE_STATE_EVENT = 19,
    /**< Signal event search failed */
    e_ERR_AET_UNABLE_TO_DEFINE_SIGNAL_EVENT = 20,
    /**< Counter event search failed */
    e_ERR_AET_UNABLE_TO_DEFINE_CNT_EVENT = 21,
    /**< Bus event search failed */
    e_ERR_AET_UNABLE_TO_DEFINE_BUS_EVENT = 22,
    /**< Define expression failed */
    e_ERR_AET_UNABLE_TO_DEFINE_EXPR = 23,
    /**< Define action failed */
    e_ERR_AET_UNABLE_TO_DEFINE_ACTION = 24,
    /**< Missing function parameters. It might be the case of passing a 0 value of pointer */
    e_ERR_AET_MISSING_FUNC_PARAMETER = 25,
    /**< Invalid function parameter passed to the function. Could invalid descriptor ID. */
    e_ERR_AET_INVALID_FUNC_PARAMETER = 26,
    /**< Invalid job ID. */
    e_ERR_AET_INVALID_JOB_ID = 27,
    /**< Target memory or register access fail. */
    e_ERR_AET_TARGET_ACCESS_FAIL = 28,
    /**< Invlid handle passed to a export API. */
    e_ERR_INVALID_HANDLE = 29,
    /**< AET register write fail. */
    e_ERR_AET_REG_WRITE = 30,
    /**< AET ownership fail. */
    e_ERR_AET_OWN = 31,
	/**< AET resource in use */
	e_ERR_AET_RSC_IN_USE = 32, 

    /* ITM errors */
    /**< ITM port 0 is reserved for string messages and unavailable for 8/16/32 data exports */
    e_ERR_ITM_PORT_RESV_STRING = 33,
    /**< ITM synchonization (SYNC message support) is not implemented */
    e_ERR_ITM_SYNC_NOT_IMPLEMENTED = 34,
    /**< ITM functionality already configured for use */
    e_ERR_ITM_ALREADY_CONFIGURED = 35,

    /* STM errors */
    e_ERR_STM_INSUFFICIENT_RESOURCES = 36,
    e_ERR_STM_MASTER_NOT_SUPPORTED = 37,
    e_ERR_STM_FIFO_NOT_EMPTY = 38,
    e_ERR_STM_INVALID_TEST_PATTERN = 39,
    e_ERR_STM_INVALID_FUNC_MODE = 40,

    /* STM XPORT errors */
    e_ERR_STM_ERROR_STRING_FORMAT = 41,

    /* DRM errors */
    e_ERR_DRM_STM_NOT_SUPPORTED = 42,
    e_ERR_DRM_DSP_NOT_SUPPORTED = 43,
    e_ERR_DRM_CS_NOT_SUPPORTED = 44,
    e_ERR_DRM_CS_DSP_ONLY = 45,

    /* CMI error */
    e_ERR_CMI_IN_USE_MOD_ACTIVITY = 46,
    e_ERR_CMI_IN_USE_EVENT_CAPTURE = 47,

    /* PMU errors */
    e_ERR_PMU_CPU_NOT_SUPPORTED = 48,
    e_ERR_PMU_COUNTER_UNAVAILABLE = 49,

    /* TB errors */
    e_TB_FLUSH_NOT_COMPLETED = 50,

    /* ARM_DEBUG errors */
    e_ERR_ARM_DEBUG_RSCID_ERROR = 51,
    e_ERR_ARM_DEBUG_PMU_NOT_AVAIL = 52,
    e_ERR_ARM_DEBUG_PMU_OUT_OF_RESOURCES = 53,
    e_ERR_ARM_DEBUG_PMU_COUNTER_RESOURCE_INVALID = 54,
    e_ERR_ARM_DEBUG_DWT_NOT_THERE = 55,
    e_ERR_ARM_DEBUG_DWT_COUNTER_RESOURCE_NOT_THERE = 56,
    e_ERR_ARM_DEBUG_DWT_COUNTER_RESOURCE_INUSE = 57,
    e_ERR_ARM_DEBUG_DWT_TRACE_INCOMPATIBLE = 58,
    e_ERR_ARM_DEBUG_DWT_RSCTYPE_ERROR = 59,
    e_ERR_ARM_DEBUG_DWT_OUT_OF_RESOURCE = 60,
    e_ERR_ARM_DEBUG_FPB_NOT_AVAIL = 61,
    e_ERR_ARM_DEBUG_FPB_OUT_OF_RESOURCES = 62,
    e_ERR_ARM_DEBUG_CORE_NOT_AVAIL = 63,
    e_ERR_ARM_DEBUG_CORE_NOT_COMPATIBLE = 64,
    e_ERR_ARM_DEBUG_CORE_OUT_OF_RESOURCES = 65,
    e_ERR_ARM_DEBUG_CORE_RESOURCE_INVALID = 66,

    e_ERR_ARM_DEBUG_TRACE_NOT_AVAIL = 67,
    e_ERR_ARM_DEBUG_TRACE_NOT_SUPPORTED = 68,
    e_ERR_ARM_DEBUG_ETM_UNHANDLED = 69,
    e_ERR_ARM_DEBUG_ETM_RESOURCE_UNAVAILABLE = 70,
    e_ERR_ARM_DEBUG_ETM_RSCTYPE_INVALID = 71,
    e_ERR_ARM_DEBUG_ETM_RSCID_INUSE = 72,

    e_ERR_PLL_GOSTAT = 73,

   /* CTSET2 errors */
   e_ERR_CTSET2_CT_INSUFFICIENT_RESOURCES = 74,
   e_ERR_CTSET2_CT_RELEASE_RESOURCES = 75,
   e_ERR_CTSET2_CT_INDEX = 76,
   e_ERR_CTSET2_DB_COMPONENT = 77,
   e_ERR_CTSET2_DB_INDEX = 78,
   e_ERR_CTSET2_TA = 79,
   e_ERR_CTSET2_PARAM = 80,
   e_ERR_CTSET2_WRITE = 81,
   e_ERR_CTSET2_XTRIG = 82,


    /* CSCTI errors */
    e_ERR_CSCTI_INVALID_CHANNEL = 83,
    e_ERR_CSCTI_INAVLID_TRIGGER = 84,

    /* MTB errors */
    e_ERR_MTB_POSITION_ERROR = 85,
    e_ERR_MTB_BAD_TRIGGER = 86,
    e_ERR_MTB_BAD_BUFFER_SIZE = 87,
    e_ERR_MTB_BAD_WATERMARK = 88,
    e_ERR_MTB_BAD_READ_REQUEST = 89,
    /**< Trace position pointer exceeds watermark when trace enabled */
    /* Total error count */
    e_ERR_MAX
} td_error_t;


/**
* @brief Return error message for a specified error.
*
* @param err Specify error to return corresponding error message
* @param p_buf Pointer to character buffer to hold the error message
* @param buflen Buffer length for error message
* @return returns td_error_t
*/
td_error_t str_error(td_error_t err, char *p_buf, size_t buflen);

#ifdef __cplusplus
}
#endif

#endif //__TD_ERROR_H
