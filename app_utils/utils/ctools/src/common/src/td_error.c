/**
* td_error.c
*
* Error Handling Implementations
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

#include "td_error.h"
#include "string.h"

const char *const p_error_msgs[e_ERR_MAX] = {
    /* Common errors */
    "no error",                                     /* e_NONE */
    "Hardware does not match intended software",    /* e_IP_BAD_HWTYPE */
    "Bad parameter passed to IP driver",            /* e_IP_BAD_PARAM */
    "Hardware could not be programmed",             /* e_IP_PROGRAM */
    "Register/memory read failure",                 /* e_IP_READ */
    "Register/memory write failure",                /* e_IP_WRITE */
    "IP driver not open",                           /* e_IP_NOT_OPEN */
    "Memory allocation error",                      /* e_IP_ALLOC */
    "Ownership error",                              /* e_IP_OWNERSHIP */
    "IP locked",                                    /* e_IP_LOCKED */
    /* C66X errors */
    "TCU_CNTL programming error",           /* e_C66X_TCU */
    "ADTF programming error",               /* e_C66X_ADTF */
    "Invalid C66X function mode",           /* e_C66X_BAD_FUNC_MODE */
    "Invalid C66X test pattern",            /* e_C66X_BAD_TEST_PATTERN */
    "Port width exceeds 5-bit capacity",    /* e_C66X_BAD_PORT_WIDTH */
    "Invalid programming for chip pin",     /* e_C66X_PIN_PGM_ERR */
    /* CSETB errors */
    "Requested trigger count is greater than CSETB RAM depth.",	/* e_CSETB_BAD_TRIG_CNT */
    /* C66 AET Errors*/
    "NULL descriptor pointer passed to an API",  /* e_ERR_AET_NULL_DESCRIPTOR               */
    "Unable to define precise HWBP",             /* e_ERR_AET_UNABLE_TO_DEFINE_PRECISE_HWBP */
    "Unable to define state event",              /* e_ERR_AET_UNABLE_TO_DEFINE_STATE_EVENT  */
    "unable to define signal event",             /* e_ERR_AET_UNABLE_TO_DEFINE_SIGNAL_EVENT */
    "unable to define counter event",            /* e_ERR_AET_UNABLE_TO_DEFINE_CNT_EVENT    */
    "unable to define bus event",                /* e_ERR_AET_UNABLE_TO_DEFINE_BUS_EVENT    */
    "unable to define expression",               /* e_ERR_AET_UNABLE_TO_DEFINE_EXPR         */
    "unable to define action",                   /* e_ERR_AET_UNABLE_TO_DEFINE_ACTION       */
    "Missing function parameters, or 0 pointer", /* e_ERR_AET_MISSING_FUNC_PARAMETER */
    "Invalid function parameter/descriptor ID",  /* e_ERR_AET_INVALID_FUNC_PARAMETER */
    "Invalid job ID",                            /* e_ERR_AET_INVALID_JOB_ID         */
    "Target memory or register access fail",     /* e_ERR_AET_TARGET_ACCESS_FAIL     */
    "Invalid handle passed to API",              /* e_ERR_INVALID_HANDLE             */
    "AET register write fail",                   /* e_ERR_AET_REG_WRITE              */
    "Unable to claim AET ownership",             /* e_ERR_AET_OWN                    */
    "Unable to allocate AET resource",           /* e_ERR_AET_RSC_IN_USE             */
    /* ITM errors */
    "ITM port 0 is reserved for string messages, but not data access",  /* e_ERR_ITM_PORT_RESV_STRING     */
    "ITM synchonization (SYNC message support) is not implemented",     /* e_ERR_ITM_SYNC_NOT_IMPLEMENTED */
    "ITM functionality already configured for use",                     /* e_ERR_ITM_ALREADY_CONFIGURED */
    /* STM errors */
    "Number of requested master enables exceeds limit.",      /* e_STM_INSUFFICIENT_RESOURCES */
    "Requested STM master in not supported by the hardware.", /* e_STM_MASTER_NOT_SUPPORTED */
    "STM FIFO did not empty on Flush().",                     /* e_STM_FIFO_NOT_EMPTY */
    "Invalid STM test pattern.",                              /* e_STM_INVALID_TEST_PATTERN */
    "Invlide STM function mode.",                             /* e_STM_INVALID_FUNC_MODE */
    /* STM XPORT errors*/
    "STM XPORT message is in the wrong format.", /* e_STM_ERROR_STRING_FORMAT */
    /* DRM errors */
    "DRM does not support STM programming.",                   /* e_ERR_DRM_STM_NOT_SUPPORTED */
    "DRM does not support DSP programming.",                   /* e_ERR_DRM_DSP_NOT_SUPPORTED */
    "DRM does not support CS programming.",                    /* e_ERR_DRM_CS_NOT_SUPPORTED */
    "DRM does not support conccurent use of CS and DSP pins.", /* e_ERR_DRM_CS_DSP_ONLY */
    /* CMI errors */
    "CMI already enabled for module activity.", /* e_ERR_CMI_IN_USE_MOD_ACTIVITY */
    "CMI already enabled for event capture.",   /* e_ERR_CMI_IN_USE_EVENT_CAPTURE */
    /* PMU errors */
    "PMU is not supported.",                    /* e_ERR_PMU_CPU_NOT_SUPPORTED */
    "PMU counter resource is not available.",   /* e_ERR_PMU_COUNTER_UNAVAILABLE */
    /* Trace Buffer errors */
    "Flush not complete.", /* e_TB_FLUSH_NOT_COMPLETED */
    /* ARM_DEBUG errors */
    "Invalid Resource ID", /* e_ERR_ARM_DEBUG_RSCID_ERROR = 51*/
    "PMU resource is not available.",  /* e_ERR_ARM_DEBUG_PMU_NOT_AVAIL = 52, */
    "All resources are in use.", /* e_ERR_ARM_DEBUG_PMU_OUT_OF_RESOURCES = 53, */
    "Internal error: PMU counter resource is invalid.", /* e_ERR_ARM_DEBUG_PMU_COUNTER_RESOURCE_INVALID = 54, */
    "DWT resource is not available.",   /* e_ERR_ARM_DEBUG_DWT_NOT_THERE = 55 */
    "Requested DWT counter resource unavailable: not implemented", /* e_ERR_ARM_DEBUG_DWT_COUNTER_RESOURCE_NOT_THERE = 56 */
    "Requested DWT counter resource unavailable: in use", /* e_ERR_ARM_DEBUG_DWT_COUNTER_RESOURCE_INUSE = 57 */
    "DWT trace request incompatible with pre-existing request", /* e_ERR_ARM_DEBUG_DWT_TRACE_INCOMPATIBLE = 58 */
    "Internal error : DWT RSCTYPE error detected", /* e_ERR_ARM_DEBUG_DWT_RSCTYPE_ERROR = 59 */
    "DWT : Unable to allocate resources for request.", /*e_ERR_ARM_DEBUG_DWT_OUT_OF_RESOURCE = 60 */
    "FPB resource is not available.", /*e_ERR_ARM_DEBUG_FPB_NOT_AVAIL = 61 */
    "FPB : All resources are in use.", /*e_ERR_ARM_DEBUG_CORE_OUT_OF_RESOURCES = 62 */


    "Core debug resource is not available.", /*  e_ERR_ARM_DEBUG_CORE_NOT_AVAIL = 63, */
    "Incompatible Core debug detected.", /* e_ERR_ARM_DEBUG_CORE_NOT_COMPATIBLE = 64, */
    "All resources are in use.", /* e_ERR_ARM_DEBUG_CORE_OUT_OF_RESOURCES = 65, */
    "Internal error: Core debug resource is invalid.", /* e_ERR_ARM_DEBUG_CORE_RESOURCE_INVALID = 66, */
    "Trace resource is not available.", /* e_ERR_ARM_DEBUG_TRACE_NOT_AVAIL = 67, */
    "Trace is not supported.", /* e_ERR_ARM_DEBUG_TRACE_NOT_SUPPORTED = 68, */
    "Internal error: Unhandled ETM error.", /* e_ERR_ARM_DEBUG_ETM_UNHANDLED = 69, */
    "Trace resource is not available.", /* e_ERR_ARM_DEBUG_ETM_RESOURCE_UNAVAILABLE = 70, */
    "Internal error: RSCTYPE is invalid.", /* e_ERR_ARM_DEBUG_ETM_RSCTYPE_INVALID = 71 */
    "Internal error: RSCID is in use.", /* e_ERR_ARM_DEBUG_ETM_RSCID_INUSE = 72, */
    "PLL programming issue", /* e_ERR_PLL_GOSTAT = 73 */

   "No more CTSET2 counter resource available.", /* e_ERR_CTSET2_CT_INSUFFICIENT_RESOURCES = 74, */
   "Trouble releasing CTSET2 resource.", /* e_ERR_CTSET2_CT_RELEASE_RESOURCES = 75, */
   "Invalid CTSET2 counter index.", /* e_ERR_CTSET2_CT_INDEX = 76, */
   "Internal Error : Could not find the corresponding CTSET2 component in the database.", /*e_ERR_CTSET2_DB_COMPONENT = 77 */
   "Internal Error : Index for the CTSET2 component is not valid.", /* e_ERR_CTSET2_DB_INDEX = 78 */
   "Internal Error:  Could not get target adapter for CTSET2.", /* e_ERR_CTSET2_TA = 79 */
   "Internal Error:  Invalid parameter for CTSET2 function call.", /* e_ERR_CTSET2_PARAM = 80 */
   "Internal Error:  Writes to CTSET2 registers failed.", /* e_ERR_CTSET2_WRITE = 81 */
   "Trouble configuring the CTIs related to CTSET2 timer event crossing triggering job.", /* e_ERR_CTSET2_XTRIG = 82 */

   "Invalid CSCTI channel.",    /* e_ERR_CSCTI_INVALID_CHANNEL = 83 */
   "Invalid CSCTI trigger.",     /* e_ERR_CSCTI_INAVLID_TRIGGER = 84 */
        /* MTB errors */

    "Invalid MTB position value", /* e_ERR_MTB_POSITION_ERROR = 85 */
    "Invalid MTB trigger setting", /* e_ERR_MTB_BAD_TRIGGER = 86 */
    "Invalid MTB buffer size setting", /* e_ERR_MTB_BAD_BUFFER_SIZE = 87*/
    "Invalid MTB watermark", /* e_ERR_MTB_BAD_WATERMARK = 88 */
    "Invalid MTB read request", /* e_ERR_MTB_BAD_READ_REQUEST = 89 */
};

td_error_t str_error(td_error_t err, char *p_buf, size_t buflen)
{
    if ((0 == p_buf) || (0 == buflen))
    {
        return e_ERR_BAD_PARAM;
    }
    else
    {
        const char * str = 0;
        if ((err > 0) && (err < e_ERR_MAX)) {
            str = p_error_msgs[err];
        }
        else {
            str = "Undefined Error";
        }
        if (buflen > strlen(str)) {
            strcpy(p_buf, str);
        }
        else {
            strncpy(p_buf, str, buflen - 1);
            p_buf[buflen - 1] = 0;
        }

        return e_ERR_NONE;
    }
}
