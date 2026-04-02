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
 *
 */

/**
 *  \file csl_sde.h
 *
 *  \brief  SDE CSL interface file
 */

#ifndef CSL_DMPAC_SDE_H_
#define CSL_DMPAC_SDE_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "include/sde_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

typedef struct {
	uint32_t STATUS;
} CSL_SdeStatusRead;

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/**
 *  struct CSL_SdeBufParams
 *  \brief Buffer Connfiguration parameters of DMPAC SDE.
 */
typedef struct
{
  uint32_t baseImageAddress;
  /**< Base Image SL2 Base Address
   * Should be aligned to 64 bytes
   */
  uint32_t baseImageWidth;
  /**< Base Image buffer width
   * Shall satisfy  >= 1.5 * image width and is aligned to 64 byte
   */
  uint32_t refImageAddress;
  /**< Reference Image SL2 Base Address
   * Should be aligned to 64 bytes
   */
  uint32_t refImageWidth;
  /**< Reference Image buffer width
   * Shall satisfy  >= 1.5 * image width and is aligned to 64 byte
   */
  uint32_t disparityAddress;
  /**< Disparity output buffer SL2 Base Address
   * Should be aligned to 64 bytes
   */

  uint32_t numOutputBufs;
  /**< Number of output buffers in SL2 (Shall be greater than one)
   */

  uint32_t rowCostBufferAddress;
  /**< Rowcost buffer SL2 Base Address
   * Should be aligned to 64 bytes
   */
} CSL_SdeBufParams;


/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Sets the entire SDE configuration to the SDE registers.
 *
 *  \param sdeRegs          Pointer to structure containing SDE Base Address
 *  \param config           Pointer to Sde_Config structure
 *                          containing the register configurations.
 *                          This parameter should be non-NULL.
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_sdeSetConfig(CSL_dmpac_sdeRegs            *sdeRegs,
                         const Sde_Config             *cfg);

/**
 *  \brief API for setting SDE SL2 Buffer parameters
 *
 *  \param sdeRegs          Pointer to structure containing SDE Base Address
 *  \param bufParam         Pointer to the structure containing
 *                          SL2 buffer and Image Parameters
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_sdeSetBufParam(CSL_dmpac_sdeRegs           *sdeRegs,
                           const CSL_SdeBufParams      *bufParam);

/**
 *  \brief API to get the confidence score histogram for SDE.
 *
 *  \param sdeRegs          Pointer to structure containing SDE Base Address
 *  \param csHistogram      Pointer to the memory where Confidence Score
 *                          Histogram will be stored
 */
int32_t CSL_sdeGetCsHistogram(const CSL_dmpac_sdeRegs  *sdeRegs,
                              uint32_t           *csHistogram);

/**
 *  \brief API for getting the CRC of the disparity output
 *
 *  \param sdeRegs          Pointer to structure containing SDE Base Address
 *  \param crc              Pointer to the memory where crc for the disparity
 *                          output is stored
 *
 *  \return                 none
 */
int32_t CSL_sdeGetPsa(const CSL_dmpac_sdeRegs           *sdeRegs,
                      uint32_t                    *crc);

/**
 *  \brief API for getting the PSA signature of the output flow vector
 *
 *  \param sdeRegs          Pointer to structure containing SDE Base Address
 *  \param psaSig           Pointer to the memory where crc for the flow
 *                          vector will be stored
 *
 *  \return                 Returns 0 on success else returns error value
 */
void CSL_sdeEnablePsa(CSL_dmpac_sdeRegs           *sdeRegs,
                      uint32_t                    enable);

#ifdef __cplusplus
}
#endif

#endif  /* CSL_DMPAC_SDE_H_ */
