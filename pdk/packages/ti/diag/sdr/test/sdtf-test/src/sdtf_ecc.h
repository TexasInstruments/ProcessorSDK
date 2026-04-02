/*
 * SDTF ECC
 *
 * Software Diagnostics Reference module for ECC Module
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

#ifndef _INCLUDE_SDTF_ECC_H_
#define _INCLUDE_SDTF_ECC_H_

#include <stdint.h>

/*********************************************************************
 * @fn      SDTF_runECC1BitInjectTest
 *
 * @brief   Execute ECC 1 bit inject test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitInjectTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitInjectTest
 *
 * @brief   Execute ECC 2 bit Inject test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitInjectTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitVIMRAMInjectTest
 *
 * @brief   Execute ECC 1 bit inject test on VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitVIMRAMInjectTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitVIMRAMInjectTest
 *
 * @brief   Execute ECC 2 bit Inject test with VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitVIMRAMInjectTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitVIMRAMDEDvector
 *
 * @brief   Execute ECC 2 bit Inject test with VIM RAM,but expect to
 *          trigger VIM DED vector
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitVIMRAMDEDvector(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitVIMRAMSelfTest
 *
 * @brief   Execute ECC 1 bit self test on VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitVIMRAMSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitVIMRAMSelfTest
 *
 * @brief   Execute ECC 2 bit Self test with VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitVIMRAMSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitSelfTest
 *
 * @brief   Execute ECC 1 bit Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitSelfTest
 *
 * @brief   Execute ECC 2 bit Self Test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC single bit error test on BTCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB0TCM0Bank0SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on BTCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB0TCM0Bank0SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Single bit error test on BTCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB0TCM0Bank1SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on BTCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB0TCM0Bank1SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitB1TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on B1TCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB1TCM0Bank0SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitB1TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on B1TCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB1TCM0Bank0SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitB1TCM0Bank1SelfTest
 *
 * @brief   Execute ECC Single bit error test on B1TCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB1TCM0Bank1SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitB1TCM0Bank1SelfTest
 *
 * @brief   Execute ECC Single bit error test on B1TCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB1TCM0Bank1SelfTest(void);

/*********************************************************************
 *
 * @brief   Indicate DED trigger
 *
 *
 * @return  0 : Success; < 0 for failures
 */
void SDTF_ECC_indicateDEDTrigger(void);


/*********************************************************************
 *
 * @brief   Run Negative tests on ECC module
 *
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_ECC_runNegativeTests(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitCodeInjectTest
 *
 * @brief   Execute ECC 2 bit Inject error or program section
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitCodeInjectTest(void);


/*********************************************************************
 * @fn      SDTF_runECC2BitCBASSSelfTest
 *
 * @brief   Execute ECC Double bit error self test on CBASS ECC aggregator
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitCBASSSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitCBASSInjectTest
 *
 * @brief   Execute ECC Doule bit error inject test on CBASS ECC aggregator
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitCBASSInjectTest(void);


/*********************************************************************
 * @fn      SDTF_runECC1BitCBASSSelfTest
 *
 * @brief   Execute ECC Single bit error self test on CBASS ECC aggregator
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitCBASSSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitCBASSInjectTest
 *
 * @brief   Execute ECC Single bit error inject test on CBASS ECC aggregator
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitCBASSInjectTest(void);


#endif /*  _INCLUDE_SDTF_ECC_H_ */
