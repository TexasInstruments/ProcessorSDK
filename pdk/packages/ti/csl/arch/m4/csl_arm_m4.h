    /**
 * @file  csl_arm_m4.h
 *
 * @brief
 *  Header file containing various enumerations, structure definitions and function
 *  declarations for the ARM M4 IP.
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2020, Texas Instruments, Inc.
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
#ifndef CSL_ARM_M4_H_
#define CSL_ARM_M4_H_

#include <stdint.h>
#include <ti/csl/hw_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
#define CSL_M4_CPUID_VARIENT_MASK                                   (0x00F00000U)
#define CSL_M4_CPUID_VARIENT_SHIFT                                  (20U)
#define CSL_M4_CPUID_PARTNO_MASK                                    (0x000000F0U)
#define CSL_M4_CPUID_PARTNO_SHIFT                                   (4U)
#define CSL_M4_CPUID_REVISION_MASK                                  (0x0000000FU)
#define CSL_M4_CPUID_REVISION_SHIFT                                 (0U)
/** ===========================================================================
 *
 * @defgroup CSL_ARM_M4_API M4 API
 * @ingroup CSL_ARM_M4_API
 *
 * ============================================================================
 */
/**
@defgroup CSL_ARM_M4_DATASTRUCT  M4 Data Structures
@ingroup CSL_ARM_M4_API
*/
/**
@defgroup CSL_ARM_M4_FUNCTION  M4 Functions
@ingroup CSL_ARM_M4_API
*/
/**
@defgroup CSL_ARM_M4_ENUM M4 Enumerated Data Types
@ingroup CSL_ARM_M4_API
*/

/** ===========================================================================
 *  @addtogroup CSL_ARM_M4_ENUM
    @{
 * ============================================================================
 */

/**
 * \brief  Structure containing the Exception Handlers.
 *         If application does not want register an exception handler then
 *         below handlers can be assigned to 'NULL' value.
 *         In such case, default handler will be used which have
 *         the infinite loop.
 */
typedef struct
{
    uint32_t varient;
    /**< Implementation defined varient number
     */
    uint32_t partno;
    /**< Implementation defined part number
     */
    uint32_t revision;
    /**< Implementation defined revision number 
     */
}CSL_ArmM4CPUInfo;

/* @} */

/**
 *  \addtogroup CSL_ARM_M4_DATASTRUCT
 *  @{
 */

/* @} */

/**
 *  \addtogroup CSL_ARM_M4_FUNCTION
 *  @{
 */

/**
 *  \brief Executes DSB instruction
 */
void CSL_armM4Dsb(void);

/**
 *  \brief Get the cpu info for M4 Core
 *
 *  \param cpuInfo          Pointer to CPU info structure
 *                          Refer struct #CSL_ArmM4CPUInfo
 *
 *  \return None
 * Please NOTE that this function has to be called in privileged mode only
 */
void CSL_armM4GetCpuID( CSL_ArmM4CPUInfo *cpuInfo );


/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif  /* end of CSL_ARM_M4_H_ definition */
