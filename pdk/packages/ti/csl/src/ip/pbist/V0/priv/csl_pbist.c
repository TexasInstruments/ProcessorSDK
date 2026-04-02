/**
 * @file  csl_pbist.c
 *
 * @brief
 *  CSL-FL implementation file for the pbist module.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2019-2020, Texas Instruments, Inc.
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

#include <ti/csl/cslr_pbist.h>
#include <ti/csl/csl_pbist.h>
#include <ti/csl/csl_types.h>

/* PBIST test mode */
#define CSL_PBIST_TEST_MODE (CSL_PBIST_MARGIN_MODE_PBIST_DFT_WRITE_MASK \
                             | (1u << CSL_PBIST_MARGIN_MODE_PBIST_DFT_READ_SHIFT))

/* PBIST Functional mode  */
#define CSL_PBIST_FUNCTIONAL_MODE       (0x0U)

/**
 * Requirement: REQ_TAG(PDK-5937)
 * Design: did_csl_pbist_interfaces did_csl_pbist_config_control
 */
int32_t CSL_PBIST_softReset(CSL_pbistRegs *pPBISTRegs)
{
    int32_t cslResult= CSL_PASS;
    
    if (pPBISTRegs == NULL_PTR)
    {
        cslResult = CSL_EFAIL;
    }
    else
    {
        /* Turns on PBIST clock */
        pPBISTRegs->PACT = CSL_PBIST_PACT_PACT_MASK;

        /* Zero out Loop counter 0 */
        pPBISTRegs->L0 = ((uint32_t)0x0u);

        /* Zero out Pbist Id register */
        pPBISTRegs->PID = ((uint32_t)0x0u);

        /* Set override register to all 1 */
        pPBISTRegs->OVER = (CSL_PBIST_OVER_RINFO_MASK
                           | CSL_PBIST_OVER_READ_MASK
                           | CSL_PBIST_OVER_MM_MASK
                           | CSL_PBIST_OVER_ALGO_MASK);

        /* Zero out Data logger 0 */
        pPBISTRegs->DLR = ((uint32_t)0x0u);

        /* Zero out Clock Mux Select register */
        pPBISTRegs->CMS = ((uint32_t)0x0u);
    }
    return  cslResult;
}

/**
 * Requirement: REQ_TAG(PDK-5934) REQ_TAG(PDK-5937)
 * Design: did_csl_pbist_interfaces did_csl_pbist_config_control
 */
int32_t CSL_PBIST_start(CSL_pbistRegs *pPBISTRegs,
                        const CSL_PBIST_config_t * const pConfig)
{
    int32_t cslResult= CSL_PASS;

    if ((pPBISTRegs == NULL_PTR) || (pConfig == NULL_PTR))
    {
        cslResult = CSL_EFAIL;
    }
    else
    {
        /* Turns on PBIST clock in PBIST ACTivate register */
        pPBISTRegs->PACT = CSL_PBIST_PACT_PACT_MASK;

        /* Set Margin mode register for Test mode */
        pPBISTRegs->MARGIN_MODE = CSL_PBIST_TEST_MODE;

        /* Zero out Loop counter 0 */
        pPBISTRegs->L0 = ((uint32_t)0x0u);

        /* Set algorithm bitmap */
        pPBISTRegs->ALGO = pConfig->algorithmsBitMap;

        /* Set Memory group bitmap */
        pPBISTRegs->RINFO = pConfig->memoryGroupsBitMap;

        /* Zero out override register */
        pPBISTRegs->OVER = pConfig->override;

        /* Set Scramble value */
        pPBISTRegs->SCR = pConfig->scrambleValue;

        /* Set DLR register for ROM based testing and Config Access */
        pPBISTRegs->DLR = (CSL_PBIST_DLR_DLR0_ROM_MASK
                           | CSL_PBIST_DLR_DLR0_CAM_MASK);
    }
    
    return  cslResult;
}

/**
 * Requirement: REQ_TAG(PDK-5942)
 * Design: did_csl_pbist_interfaces did_csl_pbist_check_result
 */
int32_t CSL_PBIST_checkResult (const CSL_pbistRegs *pPBISTRegs, Bool *pResult)
{
    int32_t cslResult= CSL_PASS;

    if ((pPBISTRegs == NULL_PTR) || (pResult == NULL_PTR))
    {
        cslResult = CSL_EFAIL;
    }
    else
    {
        /* Check Fail Status Fail Register: expected to be all zeros.
           If any bit set indicates error */
        if (pPBISTRegs->FSRF == ((uint64_t)0x00000000u))
        {
            *pResult = CSL_TRUE;
        }
        else
        {
            *pResult = CSL_FALSE;
        }
    }
    
    return  cslResult;   
}



/**
 * Requirement: REQ_TAG(PDK-5937)
 * Design: did_csl_pbist_interfaces did_csl_pbist_config_control
 */
int32_t CSL_PBIST_releaseTestMode(CSL_pbistRegs *pPBISTRegs)
{
    int32_t cslResult= CSL_PASS;
    if (pPBISTRegs == NULL_PTR)
    {
        cslResult = CSL_EFAIL;
    }
    else
    {
        /* Set Margin mode register to relese test mode and switch to Functional mode */
        pPBISTRegs->MARGIN_MODE = CSL_PBIST_FUNCTIONAL_MODE;

        /* Put RAM Group Select (RGS) = 0 */
        pPBISTRegs->RAMT = ((uint32_t)0x0u);

        /* Bit 0: Disable PBIST access */
        pPBISTRegs->PACT = ((uint32_t)0x0u);
    }      
     
    return  cslResult; 

}

/* Nothing past this point */
