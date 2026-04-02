/*
 *  Copyright (C) 2026 Texas Instruments Incorporated
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
 *  \file c7x_illegal_instructions.h
 *
 *  \brief header for c7x_illegal_instructions.c
 */

#ifndef C7X_ILLEGAL_INSTRUCTIONS_H
#define C7X_ILLEGAL_INSTRUCTIONS_H

/*===========================================================================*/
/*                        Include Files                                      */
/*===========================================================================*/

#include <stdint.h>

/*===========================================================================*/
/*                        Private Function Declarations                      */
/*===========================================================================*/

/*********************************************************************
 * @fn      Illegal_instruction_injection
 *
 * @brief   Causes an exception by creating an illegal memory access
 *
 * @param   None
 *
 * @return  None
 */
void Illegal_instruction_injection(void);

/*********************************************************************
 * @fn      Floating_point_overflow_injection
 *
 * @brief   Causes an exception by causing a floating point multiplication
 *          to overflow
 *
 * @param   None
 *
 * @return  Returns 1U if test fails, returns 0U upon pass
 */
uint16_t Floating_point_overflow_injection(void);

/*********************************************************************
 * @fn      Floating_point_divide_by_zero
 *
 * @brief   Triggers an exception cause by division by zero.
 *
 * @param   None
 *
 * @return  Returns 1U if test fails, returns 0U upon pass
 */
uint16_t Floating_point_divide_by_zero(void);

/*********************************************************************
 * @fn      Privilege_violation_injection
 *
 * @brief   Triggers an exception by writing to a secure supervisor
 *          register in memory.
 *
 * @param   None
 *
 * @return  None
 */
void Privilege_violation_injection(void);

#endif /* C7X_ILLEGAL_INSTRUCTIONS_H */