/*
 * ESM Example
 *
 * Error Signaling Module (ESM) Example Application
 *
 *  Copyright (c) 2020 Texas Instruments Incorporated
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
#ifndef INCLUDE_EVENT_TRIG_H_
#define INCLUDE_EVENT_TRIG_H_

/*===========================================================================*/
/*                         Internal function declarations                    */
/*===========================================================================*/

int32_t diag_esm_timerInit(void);
int32_t diag_esm_timersDelete(void);
int32_t diag_esm_runUseCaseTrigger(uint8_t useCaseId);
int32_t diag_esm_setNormalThresholds(void);
int32_t diag_esm_setThresholdsForCriticalTrigger(void);
int32_t diag_esm_printCurrentThresholds(void);

void    diag_esm_vtmLtThr0CrossedUpdateInt(void);
void    diag_esm_vtmGtThr1CrossedUpdateInt(void);
void    diag_esm_vtmGtThr2CrossedUpdateInt(void);
int32_t diag_esm_timerUpdateInt(uint32_t intEsmSrc);
void    diag_esm_startPinClearTimer(bool forcePinClear);

#define USE_CASE_STATUS_NOT_RUN           (0u)
#define USE_CASE_STATUS_COMPLETED_SUCCESS (1u)
#define USE_CASE_STATUS_COMPLETED_FAILURE (2u)


#endif /*  INCLUDE_EVENT_TRIG_H_ */
