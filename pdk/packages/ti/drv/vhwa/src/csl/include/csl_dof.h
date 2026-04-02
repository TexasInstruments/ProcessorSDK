/**
 *   Copyright (c) Texas Instruments Incorporated 2018
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
 *  \file csl_dof.h
 *
 *  \brief DOF interface file
 *
 */

#ifndef CSL_DMPAC_DOF_H_
#define CSL_DMPAC_DOF_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "include/dof_cfg.h"

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
 *  struct CSL_DofBufParams
 *  \brief Buffer Connfiguration parameters of DMPAC DOF.
 */
typedef struct
{
  uint32_t currImageAddress;
  /**<Current SL2 base byte address. Should be aligned to 64 bytes
   */
  uint32_t currBufWidth;
  /**<Width of Current Frame Growing Window in bytes.
   * Should be multiple of 64 bytes
   */
  uint32_t currBufHeight;
  /**<Height of Current Frame Growing Window.  Max 31 rows.
   */
  uint32_t refImageAddress;
  /**<Reference SL2 base byte address. Should be aligned to 64 bytes
   */
  uint32_t refBufWidth;
  /**<Width of reference Frame Growing Window in bytes.
   * Should be multiple of 64 bytes
   */
  uint32_t refBufHeight;
  /**<Height of reference Frame Growing Window.  Max 255 rows.
   */
  uint32_t spatialPredAddress;
  /**<Spatial Predictor SL2 base byte address.
   * Should be aligned to 64 bytes
   */
  uint32_t spatialPredDepth;
  /**<Depth of SL2 buffer
   */
  uint32_t temporalPredAddress;
  /**<Temporal Predictor SL2 base byte address.
   * Should be aligned to 64 bytes
   */
  uint32_t temporalPredDepth;
  /**<Depth of SL2 buffer
   */
  uint32_t binaryMapAddress;
  /**<Binary Map for sparse flow vector generation
   * Buffer Base Address Should be aligned to 64 bytes
   */
  uint32_t binaryMapDepth;
  /**<Depth of SL2 buffer
   */
  uint32_t outFlowAddress;
  /**<Output Flow Vector Buffer Base Address
   * Should be aligned to 64 bytes
   */
  uint32_t outFlowpDepth;
  /**<Depth of SL2 buffer
   */
} CSL_DofBufParams;

/**
 * \brief Holds DOF status register(s).
 */
typedef struct {
    uint32_t STAT;
} CSL_DofStatusRead;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Sets the entire DOF configuration to the DOF registers.
 *
 *  \param dofRegs          Pointer to structure containing DOF Base Address
 *  \param config           Pointer to #Dof_Config structure
 *                          containing the register configurations.
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_dofSetConfig(CSL_dmpac_dofRegs           *dofRegs,
                         const Dof_Config            *cfg);

/**
 *  \brief API for setting DOF Confidance score params
 *
 *  \param dofRegs          Pointer to structure containing DOF Base Address
 *  \param csParam          Pointer to the structure containing
 *                          Confidance score params
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_dofSetConfScoreParam(CSL_dmpac_dofRegs               *dofRegs,
                                 const Dof_ConfScoreParam        *csParam);


/**
 *  \brief API for getting the CS histogram of the output flow vector.
 *
 *  \param dofRegs          Pointer to structure containing DOF Base Address
 *  \param csHistogram      Pointer to the memory where Confidence Score
 *                          Histogram will be stored
 *  \param arg              Returns 0 on success else returns error value
 */
int32_t CSL_dofGetCsHistogram(const CSL_dmpac_dofRegs  *dofRegs,
                              uint32_t           *csHistogram);


/**
 *  \brief API for setting DOF SL2 Buffer and image params
 *
 *  \param dofRegs          Pointer to structure containing DOF Base Address
 *  \param bufParam         Pointer to the structure containing
 *                          SL2 buffer and Image Parameters
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_dofSetBufParam(CSL_dmpac_dofRegs           *dofRegs,
                           const CSL_DofBufParams      *bufParam);

/**
 *  \brief API for enabling the PSA signature generation for flow vector
 *
 *  \param dofRegs          Pointer to structure containing DOF Base Address
 *  \param enable           Flag to enable/disable the PSA
 *
 *  \return                 None
 */
void CSL_dofEnablePsa(CSL_dmpac_dofRegs       *dofRegs,
                      uint32_t                 enable);

/**
 *  \brief API for getting the PSA signature of the output flow vector
 *
 *  \param dofRegs          Pointer to structure containing DOF Base Address
 *  \param psaSig           Pointer to the memory where crc for the flow
 *                          vector will be stored
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_dofGetPsa(const CSL_dmpac_dofRegs           *dofRegs,
                      uint32_t                    *psaSig);

#ifdef __cplusplus
}
#endif

#endif  /* CSL_DMPAC_DOF_H_ */
