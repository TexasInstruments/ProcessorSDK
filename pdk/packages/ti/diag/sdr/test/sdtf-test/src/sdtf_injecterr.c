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

#include <sdr_common.h>
#include <sdr_ecc.h>
#include "sdtf_errortype.h"
#include "sdtf_common.h"
#include "sdtf_injecterr.h"

void *SDTF_ramTestaddr=(void *)0x41c50000;

/*******************************************************************************
 * @fn      SDTF_injectError
 *
 * @brief   Routine to inject error
 *
 * @param   errorType  Type of error to introduce
 *
 * @return    0 : Success; < 0 for failures
 */
int32_t SDTF_injectError(SDTF_errorType_t errorType)
{
    SDR_Result result;

    switch(errorType) {
        case SDTF_ERROR_TYPE_ECC_RAM_SINGLE_BIT:
            {
                SDR_ECC_InjectErrorConfig_t injectErrorConfig;

                injectErrorConfig.pErrMem =  SDTF_ramTestaddr;
                injectErrorConfig.flipBitMask =  SDTF_RAM_TEST_1BIT;
                /* Call SDR API to inject ecc single bit error */
                result = SDR_ECC_injectError(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                            SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                            SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                            &injectErrorConfig);
                if(result != SDR_PASS) {
                    SDTF_printf("\n SDTF_ERROR_TYPE_ECC_RAM_SINGLE_BIT test failed");
                    return -1;
                }
            }
            break;

        case SDTF_ERROR_TYPE_ECC_RAM_DOUBLE_BIT:
	    /* Call SDR API to inject ecc double bit error */
            {
                SDR_ECC_InjectErrorConfig_t injectErrorConfig;

                injectErrorConfig.pErrMem =  SDTF_ramTestaddr;
                injectErrorConfig.flipBitMask =  SDTF_RAM_TEST_2BIT;
                /* Call SDR API to inject ecc double bit error */
                result = SDR_ECC_injectError(SDR_ECC_MEMTYPE_MCU_R5F0_CORE,
                                            SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                            SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                                            &injectErrorConfig);
                if(result != SDR_PASS) {
                    SDTF_printf("\n SDTF_ERROR_TYPE_ECC_RAM_DOUBLE_BIT test failed");
                    return -1;
                }

            }
            break;

        case SDTF_ERROR_TYPE_NONE:
        case SDTF_ERROR_TYPE_MAX_ERROR:
        default:
            SDTF_printf(" ERROR: invalid Error type %d \n", errorType);
            break;
    }

    return 0;
}
