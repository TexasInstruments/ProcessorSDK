/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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
 *  \defgroup SAFETY_CHECKERS Safety checkers
 */

 /**
 *  \ingroup  SAFETY_CHECKERS
 *  \defgroup RM_SAFETY_CHECKERS RM Safety Checkers Library
 *
 *  @{
 */

/**
 *  \file     safety_checkers_rm_soc.h
 *
 *  \brief    This file contains data structures for RM safety checker module
 *
 */

#ifndef SAFETY_CHECKERS_RM_SOC_H_
#define SAFETY_CHECKERS_RM_SOC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "safety_checkers_rm.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup RM_SAFETY_CHECKERS
 *
 * @defgroup SAFETY_CHECKERS_RM_SOC_MACROS  SAFETY_CHECKERS_RM library macro definition
 *  @{
 */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Offsets for RM register blobs */
#define SAFETY_CHECKERS_RM_REG_HEX0			(0X00U)
#define SAFETY_CHECKERS_RM_REG_HEX4			(0x04U)
#define SAFETY_CHECKERS_RM_REG_HEX8			(0X08U)
#define SAFETY_CHECKERS_RM_REG_HEX40		(0X40U)
#define SAFETY_CHECKERS_RM_REG_HEX100		(0X100U)
#define SAFETY_CHECKERS_RM_REG_HEX1000		(0X1000U)

/** @} */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef SAFETY_CHECKERS_RM_SOC_H_ */

/** @} */
