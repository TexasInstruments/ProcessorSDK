/*
 *  Copyright (C) 2025 Texas Instruments Incorporated
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
 *  \file c7x_mma_error.h
 *
 *  \brief header for c7x_mma_error.c
 */
#ifndef C7X_MMA_ERROR_H
#define C7X_MMA_ERROR_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <c7x.h>

/* ========================================================================== */
/*                         Macros & Typedefs                                  */
/* ========================================================================== */

typedef enum
{
 MMA_OVERFLOW_EXCEPTION = 0,
 MMA_UNDERFLOW_EXCEPTION,
 MMA_C_MATRIX_WRITE_COLLISION,
 MMA_C_MATRIX_READ_COLLISION,
 MMA_AFSM_CONFIG_PARITY_ERROR,
 MMA_BFSM_CONFIG_PARITY_ERROR,
 MMA_CFSM_CONFIG_PARITY_ERROR,
 MMA_XFSM_CONFIG_PARITY_ERROR,
 MMA_OFFSET_PARITY_ERROR,
 MMA_TEST_DONE
}mma_error;

/*===========================================================================*/
/*                         Internal function declarations                    */
/*===========================================================================*/

/*********************************************************************
 * @fn      MMA_underflowError_injection
 *
 * @brief   Injects Underflow error
 *
 * @param   None
 *
 * @return  None
 */
void MMA_underflowError_injection(void);

/*********************************************************************
 * @fn      MMA_overflowError_injection
 *
 * @brief   Injects overflow error
 *
 * @param   None
 *
 * @return  None
 */

void MMA_overflowError_injection(void);

/*********************************************************************
 * @fn      MMA_cMatrixWrite_collision
 *
 * @brief   Injects C Matrix write collision error on MMA
 *
 * @param   None
 *
 * @return  None
 */
void MMA_cMatrixWrite_collision(void);

/*********************************************************************
 * @fn      MMA_cMatrixRead_collision
 *
 * @brief   Injects C Matrix read collision error on MMA
 *
 * @param   None
 *
 * @return  None
 */
void MMA_cMatrixRead_collision(void);

/*********************************************************************
 * @fn      MMA_AFSM_configParityError_injection
 *
 * @brief   Injects A FSM config parity error
 *
 * @param   None
 *
 * @return  None
 */
void MMA_AFSM_configParityError_injection(void);

/*********************************************************************
 * @fn      MMA_BFSM_configParityError_injection
 *
 * @brief   Injects B FSM config parity error
 *
 * @param   None
 *
 * @return  None
 */
void MMA_BFSM_configParityError_injection(void);

/*********************************************************************
 * @fn      MMA_CFSM_configParityError_injection
 *
 * @brief   Injects C FSM config parity error
 *
 * @param   None
 *
 * @return  None
 */
void MMA_CFSM_configParityError_injection(void);

/*********************************************************************
 * @fn      MMA_XFSM_configParityError_injection
 *
 * @brief   Injects X FSM config parity error
 *
 * @param   None
 *
 * @return  None
 */
void MMA_XFSM_configParityError_injection(void);

/*********************************************************************
 * @fn      MMA_offsetPartiyError_injection
 *
 * @brief   Injects offset parity error
 *
 * @param   None
 *
 * @return  None
 */
void MMA_offsetPartiyError_injection(void);

/*********************************************************************
 * @fn      ClearMMA
 *
 * @brief   Resets Clear MMA
 *
 * @param   None
 *
 * @return  None
 */
void ClearMMA(void);

/*********************************************************************
 * @fn      get_corrupted_offset
 *
 * @brief   Returns corrupted offset value
 *
 * @param   None
 *
 * @return  __HWA_OFFSET_REG
 */
__HWA_OFFSET_REG get_corrupted_offset(void);

/*********************************************************************
 * @fn      Get_CFSM_Corrupt_config
 *
 * @brief   Returns C FSM corrupt config
 *
 * @param   None
 *
 * @return  __HWA_CONFIG_REG_v1
 */
__HWA_CONFIG_REG_v1 Get_CFSM_Corrupt_config(void);

/*********************************************************************
 * @fn      Get_BFSM_Corrupt_config
 *
 * @brief   Returns B FSM corrupt config
 *
 * @param   None
 *
 * @return  __HWA_CONFIG_REG_v1
 */
__HWA_CONFIG_REG_v1 Get_BFSM_Corrupt_config(void);

/*********************************************************************
 * @fn      Get_AFSM_Corrupt_config
 *
 * @brief   Returns A FSM corrupt config
 *
 * @param   None
 *
 * @return  __HWA_CONFIG_REG_v1
 */
__HWA_CONFIG_REG_v1 Get_AFSM_Corrupt_config(void);

/*********************************************************************
 * @fn      Get_XFSM_Corrupt_config
 *
 * @brief   Returns X FSM corrupt config
 *
 * @param   None
 *
 * @return  __HWA_CONFIG_REG_v1
 */
__HWA_CONFIG_REG_v1 Get_XFSM_Corrupt_config(void);

/*********************************************************************
 * @fn      HWA_NOP
 *
 * @brief   No operation
 *
 * @param   None
 *
 * @return  None
 */
void HWA_NOP(uint8_t delay_count);

#endif /* C7X_MMA_ERROR_H */
