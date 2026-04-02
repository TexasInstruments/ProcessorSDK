/*
 *   Copyright (c) Texas Instruments Incorporated 2020
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

 /**
 *  \file     ccmr5_test_defs.h
 *
 *  \brief    This file contains CCM test definitions for R5 core.
 *
 *  \details  CCM tests defines needed for different SOCs
 **/

#ifndef CCMR5_TEST_DEFS_H
#define CCMR5_TEST_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
#ifdef SOC_AM65XX
#define CSL_TEST_CCM_BASE CSL_MCU_ARMSS0_COMPARE_CFG_BASE
#endif

#ifdef SOC_J721E
#define CSL_TEST_CCM_BASE CSL_MCU_R5FSS0_COMPARE_CFG_BASE
#endif

#ifdef SOC_J7200
#define CSL_TEST_CCM_BASE CSL_MCU_R5FSS0_COMPARE_CFG_BASE
#endif 

#ifdef SOC_J721S2
#define CSL_TEST_CCM_BASE CSL_MCU_R5FSS0_COMPARE_CFG_BASE
#endif 

#if defined (SOC_J784S4) || defined (SOC_J742S2) 
#define CSL_TEST_CCM_BASE CSL_MCU_R5FSS0_COMPARE_CFG_BASE
#endif 

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                 External Function Declarations                             */
/* ========================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* CCMR5_TEST_DEFS_H */
