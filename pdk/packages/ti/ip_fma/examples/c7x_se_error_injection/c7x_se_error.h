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
 *  \file c7x_se_error.h
 *
 *  \brief header for c7x_se_error.c
 */
#ifndef C7X_SE_ERROR_H
#define C7X_SE_ERROR_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <c7x.h>

/*===========================================================================*/
/*                         Internal function declarations                    */
/*===========================================================================*/

/*********************************************************************
 * @fn      SE_OpeningError_Injection
 *
 * @brief   Injects an error while opening a stream in a 
 *          streaming engine
 *
 * @param   None
 *
 * @return  None
 */
void SE_OpeningError_Injection(void);

/*********************************************************************
 * @fn      SE_SavingError_Injection
 *
 * @brief   Injects an error while saving a stream in a 
 *          streaming engine
 *
 * @param   None
 *
 * @return  None
 */
void SE_SavingError_Injection(void);

/*********************************************************************
 * @fn      SE_RestoringError_Injection
 *
 * @brief   Injects an error while restoring a stream in a 
 *          streaming engine
 *
 * @param   None
 *
 * @return  None
 */
void SE_RestoringError_Injection(void);

/*********************************************************************
 * @fn      SE_ResourceAccessError_Injection
 *
 * @brief   Injects an error by accessing a closed stream
 *
 * @param   None
 *
 * @return  None
 */
void SE_ResourceAccessError_Injection(void);

#endif /* C7X_SE_ERROR_H */