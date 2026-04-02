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

#ifndef _INCLUDE_SDTF_TEST_H_
#define _INCLUDE_SDTF_TEST_H_

#include <stdint.h>


#define SDTF_PROFILE_ONESHOT 0
#define SDTF_PROFILE_PERIODIC 1



extern volatile uint32_t SDTF_allTasksEnded;

typedef struct SDTF_periodic_config_s {
    uint32_t timerPeriodInms;
} SDTF_periodic_config_t;

/*********************************************************************
 * @fn      SDTF_oneShotTestAllModules
 *
  * @brief   Executes all one shot tests
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */

int32_t SDTF_oneShotTestAllModules(void);

/*********************************************************************
 * @fn      SDTF_configPeriodicTests
 *
 * @brief   Execute all the configured periodic tests
 *
 * @param   periodicConfig - configuration structure for periodic tests
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_configPeriodicTests(SDTF_periodic_config_t periodic_config);

/*********************************************************************
 * @fn      SDTF_triggerPeriodicTests
 *
 * @brief   Trigger the periodic tests
 *
 * @param   None
*
* @return  None
*/
void SDTF_triggerPeriodicTests(uintptr_t arg);

/*********************************************************************
 * @fn      SDTF_runPeriodicTests
 *
 * @brief   Execute all the configured periodic tests
 *
 * @param   None
*
* @return  0 : Success; < 0 for failures
*/
int32_t SDTF_runPeriodicTests(void);

/*********************************************************************
 * @fn      SDTF_runInteractiveTests
 *
 * @brief   Execute interactive tests
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runInteractiveTests(void);

/*********************************************************************
 * @fn      test_sdr_test
 *
 * @brief   Execute all sdr tests
 *
 * @param   None
 *
 * @return  None
 */
void test_sdr_test(void);

#endif /*  _INCLUDE_SDTF_TEST_H_ */
