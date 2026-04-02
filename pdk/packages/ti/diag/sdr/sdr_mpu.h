/*
 * SDR MPU
 *
 * Software Diagnostics Reference module for MPU module
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
 * @file  sdr_mpu.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDR MPU interface.
 *  ============================================================================
 */

#ifndef INCLUDE_SDR_MPU_H_
#define INCLUDE_SDR_MPU_H_

#include "sdr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDR_MPU_API SDR MPU API
 *
 * @section Overview
 * The SDR MPU module provides API for MPU Diagnostics
 *
 * ============================================================================
 */
/**
@defgroup SDR_MPU_DATASTRUCT  SDR MPU Data Structures
@ingroup SDR_MPU_API
*/
/**
@defgroup SDR_MPU_FUNCTION  SDR MPU Functions
@ingroup SDR_MPU_API
*/
/**
@defgroup SDR_MPU_ENUM SDR MPU Enumerated Data Types
@ingroup SDR_MPU_API
*/

/**
@defgroup SDR_MPU_MACROS SDR MPU Macro defines
@ingroup SDR_MPU_API
*/

/** ===========================================================================
 *  @addtogroup SDR_MPU_ENUM
    @{
 * ============================================================================
 */

/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the type of Memory access to be used
 *        to trigger MPU exception
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_MPU_DATA_READ_ACCESS = 1,
    /**<  Use DATA Read access for MPU violation */
    SDR_MPU_DATA_WRITE_ACCESS = 2,
    /**<  Use DATA Write access for MPU violation */
} SDR_MPU_memAccessType;

/* @} */

/**
 *  \addtogroup SDR_MPU_MACROS
 *  @{
 */


/* @} */

/**
 *  \addtogroup SDR_MPU_DATASTRUCT
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief MPU Data configuration
 *
 * This structure defines the elements of the MPU Data configuration
 * Please refer to ARM R5 Technical Reference Manual for details on MPU
 * The self test code assumes a free memory region ID to be provided which
 * configures the test memory location sub region to trigger the intended
 * MPU permission fault.
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_MPU_memConfig_s
{
    uint32_t *pMemLocation;
    /**< Pointer to 16 byte aligned 32 byte memory block for self test 
         example location as 0x41C7EFE0 */
    SDR_MPU_memAccessType memAccessType;
    /**< Memory access type to be used to generate exception */
    uint32_t  mpuRegionId;
    /**< MPU Region ID to be used for MPU Self Test */
} SDR_MPU_memConfig_t;

/* @} */

/**
 *  \addtogroup SDR_MPU_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief   Self Test API for MPU module
 *         NOTE: Need to call SDR_MPU_init function before calling this API
 *
 * \param   pMemConfig: Pointer to memory configuration to be used for MPU
 *                        exception

 * \param   loopCount: Based on this count the diagnostics will wait in a loop
 *                     value of 0 indicates WAIT_FOREVER
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_MPU_selfTest (const SDR_MPU_memConfig_t *pMemConfig,
                             uint32_t loopCount);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDR_MPU_H_ */
