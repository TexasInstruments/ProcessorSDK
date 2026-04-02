/*
 * SDR TEST
 *
 * Software Diagnostics Reference Test
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

#include "sdtf_registercheck.h"

/******************************************************************************
 * @fn      SDTF_checkRegisterSet
 *
 * @brief   Check register set against static reference set
 *
 * @param   basePtr  Base address of the HW module
 * @param   staticReferenceRegisterSet Structure specifying reference register
 *          set     
 *
 * @return    0 : Success; < 0 for failures
 */
int32_t SDTF_checkRegisterSet(uint32_t *basePtr,
                          const SDTF_referenceRegisterSet_t *staticReferenceRegisterSet)
{
    int32_t i;

    for( i=0; i< staticReferenceRegisterSet->numWords; i++) {
        if( (staticReferenceRegisterSet->valuesPtr[i]
             & staticReferenceRegisterSet->maskPtr[i]) !=
            (basePtr[i] & staticReferenceRegisterSet->maskPtr[i])) {
            return -1;
        }
    }
    return 0;
}

/******************************************************************************
 * @fn      SDTF_copyToShadowConfig
 *
 * @brief   Copy register set to shadow config
 *
 * @param   basePtr  Base address of the HW module
 * @param   shadowRegisterSet Structure specifying reference register set    
 *
 * @return  None
 */           
void SDTF_copyToShadowConfig(uint32_t *basePtr,
                            SDTF_shadowRegisterSet_t *shadowRegisterSet )
{
    int32_t i;

    /* Copy register set to shadow copy */
    for( i=0; i< shadowRegisterSet->numWords; i++) {
        shadowRegisterSet->valuesPtr[i] = basePtr[i];
    }
    return;
}

/******************************************************************************
* @fn      SDTF_updateToShadowConfig
*
* @brief   update register set in shadow config
*
* @param   basePtr  Base address of the HW module
* @param   shadowRegisterSet Structure specifying shadow register set
* @param   offsetWords Offset in workds to update from the base address  
* @param   numWords Number of words to update  
*
* @return  None
*/     
void SDTF_updateToShadowConfig(uint32_t *basePtr,
                              SDTF_shadowRegisterSet_t *shadowRegisterSet,
                              uint32_t offsetWords, uint32_t numWords)
{
    int32_t i;

    /*Updates selective section of the register set */
    /* Copy register set to shadow copy */
    for( i=0; i< numWords; i++) {
        shadowRegisterSet->valuesPtr[i] = basePtr[offsetWords + i];
    }
    return;
}
