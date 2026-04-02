/*
 * SDR CRC
 *
 * Software Diagnostics Reference module for CRC module
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
 * @file  sdr_crc.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDR CRC interface.
 *  ============================================================================
 */

#ifndef INCLUDE_SDR_CRC_H_
#define INCLUDE_SDR_CRC_H_

#include "sdr_common.h"
#include <ti/csl/csl_crc.h>

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDR_CRC_API SDR CRC API
 *
 * @section Overview
 * The SDR CRC module provides API for WatchDog Timer Diagnostics
 *
 * ============================================================================
 */
/**
@defgroup SDR_CRC_DATASTRUCT  SDR CRC Data Structures
@ingroup SDR_CRC_API
*/
/**
@defgroup SDR_CRC_FUNCTION  SDR CRC Functions
@ingroup SDR_CRC_API
*/
/**
@defgroup SDR_CRC_ENUM SDR CRC Enumerated Data Types
@ingroup SDR_CRC_API
*/

/**
@defgroup SDR_CRC_MACROS SDR CRC Macro defines
@ingroup SDR_CRC_API
*/

/** ===========================================================================
 *  @addtogroup SDR_CRC_ENUM
    @{
 * ============================================================================
 */
/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the Data size for input CRC DATA
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_CRC_DATA_8_BIT = 1,
    /**< 8 Bit data packed */
    SDR_CRC_DATA_16_BIT = 2,
    /**< 16 Bit data packed */
    SDR_CRC_DATA_32_BIT = 3,
    /**< 32 Bit data packed */
} SDR_CRC_dataBitSize;
/* @} */

/**
 *  \addtogroup SDR_CRC_MACROS
 *  @{
 */


/* @} */

/**
 *  \addtogroup SDR_CRC_DATASTRUCT
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief CRC Data configuration
 *
 * This structure defines the elements of the CRC Data configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_CRC_dataConfig_s
{
    uint32_t *pCRCData;
    /**< Pointer to Data used for CRC  */
    uint32_t size;
    /**< Size of Data in Bytes  */
    SDR_CRC_dataBitSize dataBitSize;
    /**< Data Bit size  */
    crcOperationMode_t crcOpMode;
    /**< CRC operation mode, only full CPU mode is supported  */
    uint32_t refCRCValueLSW;
    /**< Reference CRC value Lower 32 bits */
    uint32_t refCRCValueMSW;
    /**< Reference CRC value Upper 32 bits */

} SDR_CRC_dataConfig_t;

/* @} */

/**
 *  \addtogroup SDR_CRC_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief   Initializes CRC module for SDR
 *
 * \return  None
 */
void SDR_CRC_init (void);

/** ============================================================================
 *
 * \brief   Self Test API for CRC module
 *
 * \param   crcChannel: CRC channel number to be used
 * \param   pDataConfig: Pointer to data configuration
 *
 * \return  SDR_PASS: Success; SDR_FAIL: Failure; SDR_BADARGS: Bad arguments error
 */
SDR_Result SDR_CRC_selftest (uint32_t crcChannel, const SDR_CRC_dataConfig_t *pDataConfig);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDR_CRC_H_ */
