/**
 *   Copyright (c) Texas Instruments Incorporated 2019
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
 */

/**
 *  \file csl_h3q.h
 *
 *  \brief CSL interface file for H3A module
 *
 */

#ifndef CSL_H3A_H_
#define CSL_H3A_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <soc/vhwa_soc.h>
#include "include/h3a_cfg.h"

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

/* None */


/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Function to H3A configuration.
 *         Based on the module selected in the cfg, it configures either
 *         AF or AEWB modules. It also sets the common registers which are
 *         applicable for both the AF/AEWB modules.
 *
 *  \param h3aRegs          H3A Register overlay
 *  \param cfg              Pointer to H3a_Config structure
 *                          This parameter should be non-NULL.
 *  \return                 Returns FVID2_SOK on success or FVID2 error code
 */
int32_t CSL_h3aSetConfig(CSL_rawfe_h3a_cfgRegs *h3aRegs,
    const H3a_Config *cfg);

/**
 *  \brief Function to start AF/AEWB module
 *
 *  \param h3aRegs          H3A Register overlay
 *  \param module           Id of the module, to be started,
 *                          must be H3A_MODULE_AF or H3A_MODULE_AEWB
 */
void CSL_h3aStart(CSL_rawfe_h3a_cfgRegs *h3aRegs, uint32_t module);

/**
 *  \brief Function to stop AF/AEWB module
 *
 *  \param h3aRegs          H3A Register overlay
 *  \param module           Id of the module, to be stopped,
 *                          must be H3A_MODULE_AF or H3A_MODULE_AEWB
 *  \return                 Returns FVID2_SOK on success or FVID2 error code
 */
void CSL_h3aStop(CSL_rawfe_h3a_cfgRegs *h3aRegs, uint32_t module);

/**
 *  \brief Function to check if AF/AEWB busy
 *
 *  \param h3aRegs          H3A Register overlay
 *  \param module           Id of the module,
 *                          must be H3A_MODULE_AF or H3A_MODULE_AEWB
 *  \return                 Returns UTRUE if it is busy, or UFALSE
 */
uint32_t CSL_h3aIsBusy(const CSL_rawfe_h3a_cfgRegs *h3aRegs, uint32_t module);

/**
 *  \brief Function to calculate size of the output based on
 *         given AEWB configuration.
 *
 *  \param                  AEWB configuration, it does not check
 *                          if module is set to AEWB. It directly
 *                          accesses AEWB config from H3a_Config
 *  \return                 Size of the output buffer required
 *                          for this configuration
 */
uint32_t CSL_h3aCalcAewbSize(const H3a_Config *cfg);

/**
 *  \brief Function to calculate size of the output based on
 *         given AF configuration.
 *
 *  \param                  AF configuration, it does not check
 *                          if module is set to AF. It directly
 *                          accesses AF config from H3a_Config
 *  \return                 Size of the output buffer required
 *                          for this configuration
 */
uint32_t CSL_h3aCalcAfSize(const H3a_Config *cfg);

/* ========================================================================== */
/*                           Function Definition                              */
/* ========================================================================== */


#ifdef __cplusplus
}
#endif

#endif  /* CSL_H3A_H_ */
