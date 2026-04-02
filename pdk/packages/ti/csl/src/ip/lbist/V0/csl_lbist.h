/********************************************************************
 * Copyright (C) 2019-2020 Texas Instruments Incorporated.
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
 *
 *   @file  csl_lbist.h
 *
 *   @path  $(CSLPATH)/src/ip/lbist/V0
 *
 *   @desc  This file contains the CSL-FL API's for LBIST
 *
 *   @defgroup CSL_LBIST_API LBIST API
 *
 *   This is the CSL-FL API documentation for the Logical Built-In Self Test
 *   (LBIST) module.
 *
 *
 *
 */

#ifndef CSL_LBIST_H_
#define CSL_LBIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/cslr_lbist.h>

/**
@defgroup CSL_LBIST_DATASTRUCT  LBIST Data Structures
@ingroup CSL_LBIST_API
*/
/**
@defgroup CSL_LBIST_FUNCTION  LBIST Functions
@ingroup CSL_LBIST_API
*/
/**
@defgroup CSL_LBIST_ENUM LBIST Enumerated Data Types
@ingroup CSL_LBIST_API
*/

/**
 *  @addtogroup CSL_LBIST_ENUM
    @{
 *
 */

/** @} */

/**
 *  @addtogroup CSL_LBIST_DATASTRUCT
    @{
 *
 */

/** ---------------------------------------------------------------------------
 * @brief   This structure contains the different register elements of the LBIST
 *          Module
 *
 * ----------------------------------------------------------------------------
 */

/** ---------------------------------------------------------------------------
 * @brief   This structure contains the different configuration used for LBIST
 *          Note that the LBIST configuration is SOC and instance specific
 *          Check SOC documentation for configuration details
 *
 * ----------------------------------------------------------------------------
 */

typedef struct CSL_LBIST_config
{
    /** Clock delay after scan_enable switching */
    uint32_t dc_def;
    /** LBIST clock divide ratio */
    uint32_t divide_ratio;
    /** Bitmap of stuck-at patterns to run */
    uint32_t static_pc_def;
    /** Bitmap of set patterns to run */
    uint32_t set_pc_def;
    /** Bitmap of reset patterns to run */
    uint32_t reset_pc_def;
    /** Bitmap of chain test patterns to run */
    uint32_t scan_pc_def;
    /** Initial seed for Pseudo Random Pattern generator (PRPG) */
    uint64_t prpg_def;
} CSL_LBIST_config_t;

/** @} */

/**
 *  @addtogroup CSL_LBIST_FUNCTION
    @{
 *
 */

/**
 *  \brief Enable LBIST Isolation
 *
 *  This function enables LBIST isolation. Isolation needs to be enabled before.
 *  starting LBIST.
 *
 *
 *  \param pLBISTRegs       [IN]  Pointer to LBIST register map
 *
 *  \return The CSL error code for the API.
 *                                 If pLBISTRegs is NULL: CSL_EBADARGS
 *                                 Success: CSL_PASS
 */
int32_t CSL_LBIST_enableIsolation(CSL_lbistRegs *pLBISTRegs);

/**
 *  \brief Disable LBIST Isolation
 *
 *  This function disables LBIST isolation. This is done after LBIST is complete.
 *  NOTE: This should not be called when the LBIST test is running. If called
 *  while LBIST test is running, it may lead to undefined behaviour.
 *
 *
 *  \param pLBISTRegs       [IN]  Pointer to LBIST register map
 *
 *  \return The CSL error code for the API.
 *                                 If pLBISTRegs is NULL: CSL_EBADARGS
 *                                 Success: CSL_PASS
 */
int32_t CSL_LBIST_disableIsolation(CSL_lbistRegs *pLBISTRegs);

/**
 *  \brief Reset LBIST
 *
 *  This function resets LBIST module. This is done preparing for a new LBIST
 *  execution, as well as exiting an LBIST execution.
 *
 *
 *  \param pLBISTRegs       [IN]  Pointer to LBIST register map
 *
 *  \return The CSL error code for the API.
 *                                 If pLBISTRegs is NULL: CSL_EBADARGS
 *                                 Success: CSL_PASS
 */
int32_t CSL_LBIST_reset(CSL_lbistRegs *pLBISTRegs);

/**
 *  \brief Enable run BIST mode
 *
 *  This function enables Run BIST mode. This is needed to be set before starting
 *  LBIST.
 *
 *
 *  \param pLBISTRegs       [IN]  Pointer to LBIST register map
 *
 *  \return The CSL error code for the API.
 *                                 If pLBISTRegs is NULL: CSL_EBADARGS
 *                                 Success: CSL_PASS
 */
int32_t CSL_LBIST_enableRunBISTMode(CSL_lbistRegs *pLBISTRegs);

/**
 *  \brief Clear run BIST mode
 *
 *  This function clears Run BIST mode. This is done after LBIST execution is
 *  complete. If called in the middle of the test, the test may terminate
 *  prematurely.
 *
 *
 *  \param pLBISTRegs       [IN]  Pointer to LBIST register map
 *
 *  \return The CSL error code for the API.
 *                                 If pLBISTRegs is NULL: CSL_EBADARGS
 *                                 Success: CSL_PASS
 */
int32_t CSL_LBIST_clearRunBISTMode(CSL_lbistRegs *pLBISTRegs);

/**
 *  \brief LBIST start
 *
 *  This function starts LBIST execution.
 *
 *
 *  \param pLBISTRegs       [IN]  Pointer to LBIST register map
 *
 *  \return The CSL error code for the API.
 *                                 If pLBISTRegs is NULL: CSL_EBADARGS
 *                                 Success: CSL_PASS
 */
int32_t CSL_LBIST_start(CSL_lbistRegs *pLBISTRegs);

/**
 *  \brief LBIST stop
 *
 *  This function stops LBIST execution. This is normally called after checking
 *  the signature and clearing the run-BIST mode.
 *  But this can also be called asynchronously to stop LBIST in the middle of
 *  a test.
 *
 *
 *  \param pLBISTRegs       [IN]  Pointer to LBIST register map
 *
 *  \return The CSL error code for the API.
 *                                 If pLBISTRegs is NULL: CSL_EBADARGS
 *                                 Success: CSL_PASS
 */
int32_t CSL_LBIST_stop(CSL_lbistRegs *pLBISTRegs);

/**
 *  \brief LBIST check running
 *
 *  This function checks if LBIST is running. Note that this is specifically
 *  checking if the LBIST is currently running, and this is different from
 *  done status.
 *
 *
 *  \param pLBISTRegs       [IN]  Pointer to LBIST register map
 *  \param pIsRunning    [OUT] Pointer to variable to indicate running status
 *                             CSL_TRUE : LBIST is running
 *                             CSL_FALSE: LBIST is not running

 *  \return The CSL error code for the API.
 *                                 If pLBISTRegs or pIsRunning is NULL: CSL_EBADARGS
 *                                 Success: CSL_PASS
 */
int32_t CSL_LBIST_isRunning (const CSL_lbistRegs *pLBISTRegs, Bool *pIsRunning);

/**
 *  \brief LBIST check done
 *
 *  This function checks if LBIST is complete. Note that this is different from
 *  running status, which shows if the LBIST is curently running.
 *
 *
 *  \param pLBISTRegs       [IN]  Pointer to LBIST register map
 *  \param pIsDone       [OUT] Pointer to variable to indicate done status
 *                             CSL_TRUE : LBIST is done
 *                             CSL_FALSE: LBIST is not complete
 *
 *  \return The CSL error code for the API.
 *                                 If pLBISTRegs or pIsDone is NULL: CSL_EBADARGS
 *                                 Success: CSL_PASS
 */
int32_t CSL_LBIST_isDone (const CSL_lbistRegs *pLBISTRegs, Bool *pIsDone);

/**
 *  \brief LBIST Program config
 *
 *  This function configures the parameters for LBIST execution.
 *
 *
 *  \param pLBISTRegs       [IN]  Pointer to LBIST register map
 *  \param pConfig          [IN]  Pointer to LBIST program configuration
 *
 *  \return The CSL error code for the API.
 *                                 If pLBISTRegs or pConfig is NULL: CSL_EBADARGS
 *                                 Success: CSL_PASS
 */
int32_t CSL_LBIST_programConfig(CSL_lbistRegs *pLBISTRegs,
                                const CSL_LBIST_config_t *pConfig);

/**
 *  \brief LBIST get MISR
 *
 *  This function returns the MISR register. This register holds the Multiple
 *  Input Signature generated by LBIST test. It should be called after the
 *  LBIST execution has completed.
 *
 *
 *  \param pLBISTRegs       [IN]  Pointer to LBIST register map
 *  \param pMISRValue    [OUT] Pointer to MISR value return
 *
 *  \return The CSL error code for the API.
 *                                 If pLBISTRegs or pMISRValue is NULL: CSL_EBADARGS
 *                                 Success: CSL_PASS
 */
int32_t CSL_LBIST_getMISR(CSL_lbistRegs *pLBISTRegs, uint32_t *pMISRValue);

/**
 *  \brief LBIST get expected MISR
 *
 *  This function returns the expected MISR register value. This register holds
 *  the expected Multiple Input Signature generated by LBIST test.
 *
 *
 *  \param pLBISTSig          [IN]  Pointer to LBIST Sig Register
 *  \param pExpectedMISRValue [OUT] Pointer to MISR value return
 *
 *  \return The CSL error code for the API.
 *                                 If pLBISTRegs or pExpectedMISRValue is NULL: CSL_EBADARGS
 *                                 Success: CSL_PASS
 */
int32_t CSL_LBIST_getExpectedMISR(const uint32_t *pLBISTSig,
                                  uint32_t *pExpectedMISRValue);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* CSL_LBIST_H_ */
