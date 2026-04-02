/*
 * SDTF WDT
 *
 * Software Diagnostics Reference Test for Watchdog module
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

#ifndef _INCLUDE_SDTF_WDT_H_
#define _INCLUDE_SDTF_WDT_H_

#include <stdint.h>

#define SDTF_RTI_DWWD_TIMEOUT_VALUE     (10000u)
/**< DWWD Time Out Value in ms */

#define SDTF_RTI_CLOCK_SOURCE_32KHZ  (32u)
/**< Clock source */

#define SDTF_RTI_DWWDSIZE (SDR_WDT_WINDOWSIZE_50_PERCENT)

/*********************************************************************
 *
  * \brief  Initialize watchdog timer
 *
 *
 * @return  none
 */

int32_t SDTF_WDT_init(void);

/*********************************************************************
 *
  * \brief  Feed watchdog timer
 *
 *
 * @return  none
 */
void SDTF_WDT_feedWatchdogTimer(void);

/*********************************************************************
 *
 * \brief   Execute Watchdog self test for end violation
 *
 * \param   None
 *
 * \return  0 : Success; < 0 for failures
 */
int32_t SDTF_runWDTSelfTestEndViolation(void);

/*********************************************************************
 *
 * \brief   Execute Watchdog self test for window Violation
 *
 * \param   None
 *
 * \return  0 : Success; < 0 for failures
 */
int32_t SDTF_runWDTSelfTestWindowViolation(void);

/*********************************************************************
 *
 * \brief   Execute Watchdog negative tests
 *
 * \param   None
 *
 * \return  0 : Success; < 0 for failures
 */
int32_t SDTF_runWDTNegativeTests(void);

#endif /*  _INCLUDE_SDTF_WDT_H_ */
