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
 *  \file     vtm.h
 *
 *  \brief    This header provides VTM task function.
 */


#ifndef __VTM__
#define __VTM__
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarartions                            */
/* ========================================================================== */

/* Function to run VTM tests */
void BootApp_vtmFxn(void);

/* SDL OSAL Wrapper */
int32_t BootApp_osalWrapper(void);

/* Function to initiate the input trigger event for each use case */
int32_t BootApp_vtmRunTestCaseTrigger(uint8_t useCaseId);

/* Function to reset the VTM thresholds back to some typical default values */
int32_t BootApp_vtmSetNormalThresholds(void);

/* Function to simulate thresholds as if temperature continues to increase toward gt_Thr2 */
int32_t BootApp_vtmSetThresholdsForCriticalTrigger(void);

/* Function to clear VTM THR0 interrupt (Less Than Temp event) for VTM input events to the ESM. */
void    BootApp_vtmIntrruptLtThr0(void);

/* Function to clear VTM THR1 interrupts for VTM input event to the ESM. */
void    BootApp_vtmIntrruptGtThr1(void);

/* Funcion to clear VTM THR2 interrupts for VTM input event to the ESM. */
void    BootApp_vtmIntrruptGtThr2(void);

#define USE_CASE_STATUS_NOT_RUN           (0u)
#define USE_CASE_STATUS_COMPLETED_SUCCESS (1u)
#define USE_CASE_STATUS_COMPLETED_FAILURE (2u)

#endif /* __VTM__ */