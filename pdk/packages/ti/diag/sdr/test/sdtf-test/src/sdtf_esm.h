/*
 * SDTF ESM
 *
 * Software Diagnostics Reference Test for ESM Module
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

#ifndef _INCLUDE_SDTF_ESM_H_
#define _INCLUDE_SDTF_ESM_H_

#include <stdint.h>

/*********************************************************************
 * @fn      SDTF_runESMSelfTest_MCU
 *
 * @brief   Execute ESM Self test for MCU
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMSelfTest_MCU(void);

/*********************************************************************
 * @fn      SDTF_runESMSelfTest_WKUP
 *
 * @brief   Execute ESM Self test for WKUP
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMSelfTest_WKUP(void);

/*********************************************************************
 * @fn      SDTF_runESMSelfTest_MAIN
 *
 * @brief   Execute ESM Self test for MAIN
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMSelfTest_MAIN(void);

/*********************************************************************
 * @fn      SDTF_runESMInject_MCU
 *
 * @brief   Execute ESM Inject for MCU
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMInject_MCU(void);

/*********************************************************************
 * @fn      SDTF_runESMInject_WKUP
 *
 * @brief   Execute ESM Inject for WKUP
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMInject_WKUP(void);

/*********************************************************************
 * @fn      SDTF_runESMInject_MAIN
 *
 * @brief   Execute ESM Inject for MAIN
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMInject_MAIN(void);

/*********************************************************************
 * @fn      SDTF_runESMAPITest
 *
 * @brief   Execute ESM API test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMAPITest(void);

/*********************************************************************
 * @fn      SDTF_runESMNegativeTest
 *
 * @brief   Execute ESM Negative tests
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMNegativeTest(void);

#endif /*  _INCLUDE_SDTF_ESM_H_ */
