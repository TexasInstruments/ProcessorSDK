/*
 *  Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \ingroup CSL_SOC
 *  \defgroup CSL_SOC_UTIL CSL SOC Utility API
 *            CSL SOC-specific utility functions
 *
 *  @{
 */
/**
 *  \file     csl_soc_util.h
 *
 *  \brief    This file contains the prototypes of SOC utility API
 *            This also contains some related macros.
 *
 */

#ifndef CSL_SOC_UTIL_H
#define CSL_SOC_UTIL_H

#include <ti/csl/cslr.h>
#include <ti/csl/tistdtypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * \anchor CSL_ESMGroupNum
 * \name CSL ESM Group Numbers
 * \brief CSL ESM Group numbers
 *  @{
**/
/*! ESM Group 1 */
#define CSL_ESM_GROUP_1     (1U)

/*! ESM Group 2 */
#define CSL_ESM_GROUP_2     (2U)

/*! ESM Group 3 */
#define CSL_ESM_GROUP_3     (3U)

/*! Minimum ESM Group */
#define CSL_ESM_GROUP_MIN   CSL_ESM_GROUP_1

/*! Maximum ESM Group */
#if (__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R') /* R5F */
#define CSL_ESM_GROUP_MAX   CSL_ESM_GROUP_3
#elif defined (_TMS320C6X)
#define CSL_ESM_GROUP_MAX   CSL_ESM_GROUP_2
#else
#error "awr294x ESM: unsupported core"
#endif
/* @} */

/* \brief the number of Group 2/3 events */
#define CSL_ESM_GROUP2_3_NUM_EVENTS     (32U)

/**
 * @brief   Error Code: SOC_translateAddress couldn't translate the provided address
 *          This value is chosen since all other values can map to a valid address in
 *          some domain.
 */
#define CSL_TRANSLATEADDR_INVALID    (0xFFFFFFFFU)

/**
 * @brief
 *  Enumeration for address translation from one domain to the other
 *
 * @details
 *  The enumeration describes the direction of address translation.
 */
typedef enum CSL_TranslateAddr_Dir_e
{
    /**
     * @brief
     *  Cross CPU address translation (MSS to DSS or DSS to MSS)
     *    From the running core to the other core
     */
    CSL_TranslateAddr_Dir_TO_OTHER_CPU,

    /**
     * @brief
     *  Cross CPU address translation (MSS to DSS or DSS to MSS)
     *    From the other core to the running core
     */
    CSL_TranslateAddr_Dir_FROM_OTHER_CPU
}CSL_TranslateAddr_Dir;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 *  \brief CSL Local to Global address translation function.
 *
 *  This function is used by the driver and application to convert local address
 *  to global address.
 *
 *  \param locAddr  [IN] Local address
 *
 *  \return Corresponding Global address
 */
extern uint64_t CSL_locToGlobAddr(uintptr_t locAddr);

/**
 *  \brief CSL Global to Local address translation function.
 *
 *  This function is used by the driver and application to convert global address
 *  to local address.
 *
 *  \param globAddr  [IN] Global address
 *
 *  \return Corresponding Local address
 */
extern uintptr_t CSL_globToLocAddr(uint64_t globAddr);

/**
 *  \brief CSL ESM event gating function
 *
 *  This function is used by the driver and application to gate/ungate an ESM
 *  Group 2/3 event
 *
 *  \param  groupNumber  [IN] Group number for the ESM event
 *  \param  eventNumber  [IN] ESM event number
 *  \param  gating       [IN] Gating of the ESM event:
 *                            - ungating: 0
 *                            - gating: 1
 *
 *  \return  status           Success of the operation
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 *
 *  \note: This function is applicable to ESM Group 2 and 3 only
 */
extern int32_t CSL_esmEventGating(uint8_t groupNumber, uint8_t eventNumber, uint8_t gating);

/**
 *  @b Description
 *  @n
 *      This function translates the inAddr from one domain to the other
 *
 *  @param[in] inAddr
 *      input address that needs to be translated
 *  @param[in] direction
 *      hint on which domain to translate from and to
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - valid translated address and errCode is set to CSL_PASS
 *  @retval
 *      Error       - CSL_TRANSLATEADDR_INVALID and errCode is set to CSL_EFAIL
 */
extern uint32_t CSL_TranslateAddress(uint32_t inAddr, CSL_TranslateAddr_Dir direction, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      This function returns the cpu frefuency
 *
 *  @retval
 *      CPU frequency in Hz
 */
extern uint32_t CSL_SocGetCpuFreq(void);

/**
 *  @b Description
 *  @n
 *      This function returns the sys clock frefuency
 *
 *  @retval
 *      Sys clock frequency in Hz
 */
extern uint32_t CSL_SocGetSysclkFreq(void);

#ifdef __cplusplus
}
#endif

#endif /* CSL_SOC_UTIL_H */
/** @} */

/**
 *  \defgroup CSL_SOC CSL SOC Module
 */
