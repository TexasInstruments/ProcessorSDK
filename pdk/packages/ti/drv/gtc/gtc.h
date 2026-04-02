/*
 *  Copyright (c) Texas Instruments Incorporated 2024
 *  All rights reserved.
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
 */

/**
 *  \file  gtc.h
 *
 *  \brief GTC configuration APIs */

#ifndef GTC_H_
#define GTC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define GTC_CLK_RATE_200_MHZ                (200000000U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Set the FID register for GTC
 *
 *  \param  None
 *
 *  \return  Returns CSL_PASS if FID is configured correctly
 */
int32_t GTC_setFID(void);

/**
 *  \brief Set the GTC Parent clock and Clock Frequency
 *
 *  \param  None
 *
 *  \return  Returns CSL_PASS if GTC Clock Parent and Frequency is configured correctly
 */
int32_t GTC_configClock(void);

/**
 *  \brief Enable the Global Time Counter
 *
 *  \param  None
 *
 *  \return None
*/
void GTC_enable(void);

/**
 *  \brief Disable the Global Time Counter
 *
 *  \param  None
 *
 *  \return None
*/
void GTC_disable(void);

/**
 *  \brief Initialize GTC
 *
 *  \param  None
 *
 *  \return Returns CSL_PASS on success
*/
int32_t GTC_init(void);

/**
 *  \brief Read GTC counter value
 *
 *  \param  None
 *
 *  \return Returns GTC Counter value
*/
uint64_t GTC_readCounter64(void);

/**
 *  \brief Set GTC counter value
 *
 *  \param  Value of the GTC counter to be set
 *
 *  \return None
*/
void GTC_setCounter64(uint64_t value);

/**
 *  \brief Set GTC Push Event Select
 *
 *  \param  Value of the bit to be selected to generate push Event
 *
 *  \return None
*/
void GTC_selectPushEvent(uint32_t value);

/* ========================================================================== */
/*                      Static Function Declarations                          */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef GTC_H_ */

/* @} */
