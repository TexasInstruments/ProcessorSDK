/*
 * SDTF RAT
 *
 * Software Diagnostics Reference Test for RAT module
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

#include <stdint.h>
#include <ti/csl/csl_rat.h>

#include "sdtf_common.h"
#include "sdtf_profile.h"
#include "sdtf_test.h"
#include "sdtf_rat.h"

/* Choosing unused address space */
#define SDTF_RAT_SELF_TEST_TRANSLATE_BASE  (0x10000000U)

__attribute((section(".sdtf_rat_testsection"))) __attribute__((aligned(32))) uint32_t SDTF_RATTestArray[8];

#define SDTF_RAT_REGION_INDEX 0

/*********************************************************************
 *
 * \brief Test RAT module
 *
 *
 * \return 0 if test pass, -1 in case of failure
 */
int32_t SDTF_runtestRAT(void)
{
    int32_t retValue=0;
    CSL_RatTranslationCfgInfo translationCfg;

    SDTF_printf("\n RAT Test: starting");

    translationCfg.translatedAddress = ((uint64_t)&SDTF_RATTestArray);
    translationCfg.sizeInBytes = sizeof(SDTF_RATTestArray);
    translationCfg.baseAddress = (uint32_t)SDTF_RAT_SELF_TEST_TRANSLATE_BASE;

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    retValue = SDTF_RATSelfTest(SDTF_RAT_REGION_INDEX, &translationCfg, &SDTF_RATTestArray[0]);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);
    if (retValue != 0 ) {
        SDTF_printf("\n RAT Test failed");
    } else {
        SDTF_printf("\n RAT Test  complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retValue;

}
/*********************************************************************
 *
 * \brief Self test for RAT module
 * Note: This function assumes both the mapped and translated addresses are accessible directly
 * for the self test
 * Note: The values in the region may be overwritten
 *
 * \param regionIndex: RAT region index to be used for the test
 * \param pTranslateCfg: Pointer to translation configuration for
 *                       the RAT mapping
 * \param pTestMemArea: Test memory area for doing the RAT region mapping
 *
 * \return 0 if test pass, -1 in case of failure
 */
/* Note this function assumes both the mapped and translated addresses are accessible directly
 * for the self test
 * Note: The values in the region may be overwritten
 */
int32_t SDTF_RATSelfTest(uint32_t regionIndex, CSL_RatTranslationCfgInfo *pTranslationCfg,
                         uint32_t *pTestMemArea)
{
    int32_t retValue=0;
    int32_t cslStatus;
    bool result;
    CSL_ratRegs *pRatRegs = (CSL_ratRegs *)CSL_MCU_ARMSS_RAT_CFG_BASE;
    uint32_t *pMappedBase;

    /* The assumption is that the tranlated Address is accessible directly as well
     * for Self test to work.
     */
    if ((pTranslationCfg->translatedAddress > (uint64_t)0xffffffffu)
            && (pTranslationCfg->translatedAddress != (uint64_t)pTestMemArea)){
        retValue = -1;
    }

    if (retValue == 0) {
        /* Write a value to RAT region */
        pTestMemArea[0] = 0xbabeface;

        /* Set up RAT translation */
        result = CSL_ratConfigRegionTranslation(pRatRegs, regionIndex, pTranslationCfg);
        if(result == false) {
            retValue = -1;
        }
    }

    if (retValue == 0) {
        /* Verify RAT configuration */
        cslStatus = CSL_ratVerifyConfigRegionTranslation(pRatRegs, regionIndex, pTranslationCfg);
        if (cslStatus != CSL_PASS) {
            retValue = -1;
        }
    }

    if (retValue == 0) {
        /* Now write another value to test area */
        pTestMemArea[1] = 0xaaaa5555;

        pMappedBase = ((uint32_t *)(SDTF_RAT_SELF_TEST_TRANSLATE_BASE));

        /* Check the first written value */
        if (pMappedBase[0] != pTestMemArea[0]) {
            retValue = -1;
        }
    }

    if (retValue == 0) {

        /* Check second value written after RAT configuration */
        if (pMappedBase[1] != pTestMemArea[1]) {
            retValue = -1;
        }
    }

    if (retValue == 0) {

        /* Write a value to mapped region */
        pMappedBase[2] = 0x5555aaaa;
        /* Check third write to mapped area */
        if (pMappedBase[1] != pTestMemArea[1]) {
            retValue = -1;
        }
    }

    /* Disable RAT region */
    CSL_ratDisableRegionTranslation(pRatRegs, regionIndex);

    return retValue;
}
