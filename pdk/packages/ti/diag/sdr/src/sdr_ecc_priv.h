/*
 * SDR ECC
 *
 * Software Diagnostics Reference module for ECC
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

#ifndef INCLUDE_SDR_ECC_PRIV_H_
#define INCLUDE_SDR_ECC_PRIV_H_

#include <stdbool.h>

#define SDR_ECC_EVENT_FOUND (1u)

#define SDR_ECC_RAMID_INVALID      (0xffffffffu)
#define SDR_ECC_INJECTTYPE_INVALID (0xffffffffu)
#define SDR_ECC_ECC_TYPE_INVALD    (0xffffffffu)

typedef struct SDR_RAMIdEntry_s
{
    uint32_t RAMId;
    /**< Unique Ram Identifier */
    uint32_t aggregatorTypeInjectOnly;
    /**< Variable to track if the ECC aggregator configuration
     * is only inject only or a full fledged ecc aggregator
     * including tracking of error status
     * A value of 1 represents "Inject Only", and a value of
     * 0 represents "Inject with Error Capture" */
    uint32_t ramIdType;
    /**< Variable to track if the RAM ID is interconnect type (1)
     * or wrapper type (0) */
} SDR_RAMIdEntry_t;

/** ---------------------------------------------------------------------------
 * \brief This structure defines the elements of memory configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_MemConfig_s
{
    SDR_ECC_MemSubType memSubType;
    /**< Memory subtype - used to index/search for these memory configs */
    uintptr_t memStartAddr;
    /**< Memory start address */
    uint32_t size;
    /**< Size of memory in bytes */
    uint32_t stride;
    /**< Stride of memory in bytes */
    uint32_t rowSize;
    /**< Size of each row in the memory in number of bits */
    bool readable;
    /**< Memory section whether it is directly readable */
}  SDR_MemConfig_t;

/** ---------------------------------------------------------------------------
 * \brief This structure defines the elements of ECC Group checker for Interconnect
 * SDR_ECC_RamIdType
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_GrpChkConfig_s
{
    uint32_t grpChkType;
    /**< Group Checker type */
    uint32_t dataWidth;
    /**< Length of memory bus covered in bits */
}  SDR_GrpChkConfig_t;

#endif /* INCLUDE_SDR_ECC_PRIV_H_ */
