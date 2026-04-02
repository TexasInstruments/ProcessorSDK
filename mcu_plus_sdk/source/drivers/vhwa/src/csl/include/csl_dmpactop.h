/**
 *   Copyright (c) Texas Instruments Incorporated 2018-2022
 *   All rights reserved.
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
 *  \file csl_dmpactop.h
 *
 *  \brief DOF/SDE common functionality interface file
 *
 */

#ifndef CSL_DMPAC_TOP_H_
#define CSL_DMPAC_TOP_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_dmpac_abstraction_layer.h>


#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/**
 *  \anchor DMPAC_ModuleEnable
 *  \name   DMPAC module ID's
 *  \brief  MACRO's used for enabling individual accelerator in DMPAC
 *          Used in clock gating accelerator when disabled.
 *
 *  @{
 */
#define DMPAC_MODULE_DOF    (1U)
#define DMPAC_MODULE_SDE    (2U)
 /** @} */

/**
 *  \brief Enum for available FOCO channel.
 */
/**
 *  \anchor DMPAC_FOCOChnl
 *  \name   DMPAC FOCO Channel
 *  \brief  Available FOCO channel in DMPAC
 *
 *  @{
 */
#define DMPAC_FOCO_CHANNEL_0    (0U)
#define DMPAC_FOCO_CHANNEL_1    (1U)
#define DMPAC_FOCO_CHANNEL_2    (2U)
#define DMPAC_FOCO_CHANNEL_3    (3U)
#define DMPAC_MAX_FOCO_CHANNELS (4U)   
/** @} */

/**
 *  struct Dmpac_FocoConfig
 *  \brief FOCO configuratio paramenters.
 */
typedef struct
{
    uint32_t channelEnable;
    /**<Set to 0x1 to enable
     * FOCO engine.
     */

    uint32_t shiftEnable;

    uint32_t shiftM1;

    uint32_t dir;

    uint32_t round;

    uint32_t mask;

    uint32_t preload;

    uint32_t postload;

    uint32_t trig;
} Dmpac_FocoConfig;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Function to enable/disable HWA in DMPAC
 *
 *  \param dmpacTopRegs     Pointer to structure containing DMPAC Base Address
 *  \param module           Id of the module to be enabled/disabled
 *  \param enable           flag UTRUE/UFALSE
 *  \return                 Returns 0 on success else returns error value
 */
void CSL_dmpacEnableModule(CSL_dmpacRegs *dmpacTopRegs,
                           uint32_t module,
                           uint32_t enable);

/**
 *  \brief Function to FOCO module in DMPAC
 *
 *  \param focoRegs         VPAC Base Address
 *  \param module           Id of the module to be enabled/disabled
 *  \param enable           flag UTRUE/UFALSE
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_dmpacFocoSetConfig(CSL_dmpac_foco_coreRegs *focoRegs,
                               uint32_t channel,
                               const Dmpac_FocoConfig *cfg);

#ifdef __cplusplus
}
#endif

#endif  /* CSL_DMPAC_TOP_H_ */
