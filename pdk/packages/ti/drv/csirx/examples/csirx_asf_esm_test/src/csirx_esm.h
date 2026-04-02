/*
 *  Copyright (c) Texas Instruments Incorporated 2025
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
 *  \file csirx_esm.h
 *
 *  \brief header for utility functions for csirx esm
 *
 */
#ifndef CSIRX_ESM_H_
#define CSIRX_ESM_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/csl/tistdtypes.h>
#if defined (SOC_J742S2)
#include <src/ip/esm/V0/V0_0/sdlr_esm.h>
#include <include/soc/j784s4/sdlr_intr_esm0.h>
#include <src/ip/esm/V0/esm.h>
#endif
#include <sdl_esm.h>
#include <sdl_esm_soc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define BITS_PER_WORD (32u)
#define SDL_ESM_EN_KEY_ENBALE_VAL (0xFU)
#define ARRAY_SIZE (32u)
#define INIT_VAL (0u)
#define MASK_BIT (1u)
#define STATUS_NUM (1u)
#define ESM_INTR_GRP_NUM (32U)
#define GROUP_NUMBER_BIT_SHIFT  (5u)
#define NO_EVENT_VALUE (0xffffu)
#define INVALID_BIT (0u)
#define FLAG_NO (0u)
#define FLAG_YES (1u)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
typedef struct {
     /**< Store application argument */
     void *arg;
     /**< SDL ESM configuration */
     SDL_ESM_config esmInitConfig;
     /**< Store applicataion callback function */
     SDL_ESM_applicationCallback callback;
} SDL_CSIRX_ESM_Instance_t;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 * \brief   Initializes ESM module
 *
 * \param   esmInstType    Instance of ESM
 * \param   esmInitConfig  Configuration for ESM
 *
 * \return  SDL_RETURN_PASS for Success; SDL_RETURN_FAIL for Failures
 */
SDL_Result ESM_init (const SDL_ESM_Inst esmInstType,
                         const SDL_ESM_config *esmInitConfig,
                         SDL_ESM_applicationCallback callback,
                         void *arg);

/**
 * \brief        Check that ESM instance type or ESM base address is valid for this
 *               device, and fill the SDL_ESM instance
 *
 * \param [in]   esmInstType:     ESM instance type
 * \param [out]  pEsmInstancePtr: Pointer to location of ESM instance structure
 *
 * \return       true: if valid instance type; false if not valid instance type
 */
bool CSIRX_ESM_selectEsmInst(const SDL_ESM_Inst esmInstType,
                           SDL_CSIRX_ESM_Instance_t **pEsmInstancePtr);

/**
 * \brief        Check that ESM instance type is valid for this device, and fill the
 *               ESM base address
 *
 * \param [in]   esmInstType: ESM instance type
 * \param [out]  esmBaseAddr: Base address for ESM instance's registers
 *
 * \return       true: if valid instance type; false if not valid instance type
 */
bool CSIRX_ESM_getBaseAddr(const SDL_ESM_Inst esmInstType, uint32_t *esmBaseAddr);

/**
 * \brief        Get the max number of ESM events supported for a given ESM instance
 *
 * \param [in]   esmInstType: ESM instance type
 * \param [out]  esmMaxNumEvents: Maximum number of ESM events supported
 *
 * \return       true: if valid instance type; false if not valid instance type
 */
bool CSIRX_ESM_getMaxNumEvents(const SDL_ESM_Inst esmInstType, uint32_t *esmMaxNumEvents);

/**
 * \brief        Check that ESM instance type or ESM base address is valid for this
 *               device, and fill the SDL_ESM instance
 *
 * \param [in]   esmInstBaseAddr: Base address for ESM instance's registers.
 *                                Function fills pointer to instance for this
 *                                set of registers
 * \param [out]  pEsmInstType:    Pointer to ESM instance type
 * \param [out]  pEsmInstancePtr: Pointer to location of ESM instance structure
 *
 * \return       true: if valid base address; false if not valid base address
 */
void CSIRX_ESM_selectEsmInstFromAddr(uint32_t esmInstBaseAddr,
                                   SDL_ESM_Inst *pEsmInstType,
                                   SDL_CSIRX_ESM_Instance_t **pEsmInstancePtr);
/**
 * \brief   Interrupt handler for Hi ESM interrupt
 *
 * \param   esmIntrPriorityLvlType Interrupt priority level
 * \param   arg     Argument to callback function
 *
 * \return  None
 */
void CSIRX_ESM_appCallbackIntrHandler (esmIntrPriorityLvl_t esmIntrPriorityLvlType,
                              uintptr_t arg);

/**
 * \brief   Process Interrupt source and call callback function
 *
 * \param   esmIntType Interrupt type
 * \param   intSrc  Interrupt source
 *
 * \return  None
 */
void CSIRX_ESM_processInterruptSource(uint32_t esmInstBaseAddr,
                                        SDL_ESM_IntType esmIntType,
                                        uint32_t intSrc);

/**
 * \brief   Get the Group number and Index for the given interrrupt
 *          source
 *
 * \param  intSrc   Interrupt source
 * \param  groupNumber  Group number
 * \param  intIndex  Index
 *
 * \return  None
 */
void CSIRX_ESM_getGroupNumberIndex(uint32_t intSrc, uint32_t *groupNumber,
                                    uint32_t *intIndex);


/**
 * \brief   This API is used to write EOI.
 *
 * \param  intrType   Interrupt type
 *
 * \return  retVal   0 - Success; else - Failure
 */
int32_t CSIRX_MainESM_writeEOI(esmIntrType_t intrType);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

#endif /* End of CSIRX_ESM_H_ */
