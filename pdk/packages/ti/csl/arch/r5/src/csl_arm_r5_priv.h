/*
 *  Copyright (C) 2022 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file  csl_arm_r5_priv.h
 *
 *  \brief This file contains the internal defines for ARM R5 IP
 *         
 *  This includes:-
 *  - Cache Line operations by MVA
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifndef CSL_ARM_R5_PRIV_H_
#define CSL_ARM_R5_PRIV_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Invalidate a data cache line by MVA
 *
 *  This function is used to invalidate a data cache Line by MVA.
 *  For performance optimizations, this function will not invoke memory 
 *  barrier instructions. Instead end user is supposed to handle the same 
 *  after updating all the required cache lines
 *
 *  \param address  [IN]    Modified virtual address
 *
 *  \return None
 */
void CSL_armR5CacheInvalidateDcacheMva( uint32_t address );

/**
 *  \brief Clean a data cache line by MVA
 *
 *  This function is used to clean a data cache Line by MVA.
 *  For performance optimizations, this function will not invoke memory 
 *  barrier instructions. Instead end user is supposed to handle the same 
 *  after updating all the required cache lines
 *
 *  \param address  [IN]    Modified virtual address
 *
 *  \return None
 */
void CSL_armR5CacheCleanDcacheMva( uint32_t address );

/* ========================================================================== */
/*                          Global Declarations                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* CSL_ARM_R5_PRIV_H_ */
/********************************* End of file ******************************/
