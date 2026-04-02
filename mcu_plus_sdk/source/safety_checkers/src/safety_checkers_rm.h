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
 *
 *   RM safety checker library APIs are integrated into the safety application to
 *   verify the RM modules configurations and validate the current RM module configurations
 *   against golden reference.
 */

/**
 *  \file    safety_checkers_rm.h
 *
 *  \brief   This file contains RM safety checkers library interfaces and related data structures.
 *
 */
/** @} */

#ifndef SAFETY_CHECKERS_RM_H_
#define SAFETY_CHECKERS_RM_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup RM_SAFETY_CHECKERS
 *
 * @defgroup SAFETY_CHECKERS_RM_MACROS  SAFETY_CHECKERS_RM macro definition
 *  @{
 */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Maximum number of registers in a register group */
#define SAFETY_CHECKERS_RM_MAX_REG_PER_GROUP							(20U)

/** @} */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/**
 * @ingroup RM_SAFETY_CHECKERS
 *
 * @defgroup SAFETY_CHECKERS_RM_STRUCTURE  SAFETY_CHECKERS_RM structure definition
 *  @{
 */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/** 
 *
 * \brief   Structure to hold the base address and the register 
 *          details of RM control module registers
 *	 
 */
typedef struct 
{
    /* Base address of register region */
    uint32_t baseAddr;
    /* Number of different registers in a memory region */
    uint32_t regNum;
    /* Total number of registers in a register group */
    uint32_t regArrayLen;
    /* Start  address of register group */
    uint32_t regStartOffset;
    /* Block Offset Value for register group */
    uint32_t regOffsetArr [SAFETY_CHECKERS_RM_MAX_REG_PER_GROUP];
}  SafetyCheckers_RmRegData;

/** @} */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* None */

/**
 * @ingroup RM_SAFETY_CHECKERS
 *
 * @defgroup SAFETY_CHECKERS_RM_MODULE_API  SAFETY_CHECKERS RM checkers APIs
 *  @{
 */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief    API to get the register value for all the RM control modules
 *           E.g. When the RM register configuration is requested, SafetyCheckers_rmGetRegCfg
 *           collects the register configuration values for all the RM control module by
 *           getting the base address and the length from the structure. The collected
 *           dump shall be considered as a golden sample and shall be returned to
 *           the application.
 *           Register values from the RM control modules 
 *			 IR, IA, RA and DMA are used for taking the register dump.
 *
 * \param    rmRegCfg          [IN/OUT]       Pointer to store the Register configuration
 * \param    size              [IN]           Size of reg cfg array
 *
 * \return   SAFETY_CHECKERS_SOK in case of success,else failure.
 *
 */
int32_t SafetyCheckers_rmGetRegCfg(uintptr_t *rmRegCfg, uint32_t size);

/**
 * \brief    API to read back and compare the RM control module registers data            
 *           E.g. When the RM verify register dump is requested,
 *           SafetyCheckers_rmVerifyRegCfg API collects the value for RM registers
 *           and compares with the received register values.
 *			 Register values from the RM control modules 
 *			 IR, IA, RA and DMA are used for verifying the register dump.
 *            
 * \param    rmRegCfg                  [IN]       Pointer of the golden sample
 * \param    size                      [IN]       Size of reg dump pointer
 *
 * \return   SAFETY_CHECKERS_SOK in case of success else failure
 *
 */
int32_t SafetyCheckers_rmVerifyRegCfg(const uintptr_t *rmRegCfg, uint32_t size);

/** @} */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef SAFETY_CHECKERS_RM_H_ */

