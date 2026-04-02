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
 *  \file csl_pcid.h
 *
 *  \brief CSL Header file for PCID
 *      This file defines the CSL APIs for PCID module
 *
 */

#ifndef CSL_PCID_H_
#define CSL_PCID_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <soc/vhwa_soc.h>

#include <include/pcid_cfg.h>

#ifdef __cplusplus
extern "C" {
#endif



/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Horizontal latency required for PCID - Bayer Path
 */
#define CSL_PCID_HORZ_LATENCY_BAYER_OUT                  (27u)

/** \brief Horizontal latency required for PCID - IR Path
 */
#define CSL_PCID_HORZ_LATENCY_IR_OUT                  (30u)

/** \brief Horizontal blanking required for PCID
 */
#define CSL_PCID_HORZ_BLANKING                  (27u)

/** \brief Vertical Blanking required for PCID
 */
#define CSL_PCID_VERT_BLANKING                 (4u)

/** \brief Default IR Output Data width - 12Bit. Valid ranges are 8-16
 */
#define CSL_PCID_IR_DWIDTH_12BIT                 (12u)

/** \brief 8bit IR Output Data width. Valid ranges are 8-16
 */
#define CSL_PCID_IR_DWIDTH_8BIT                 (8u)

/** \brief 16bit IR Output Data width. Valid ranges are 8-16
 */
#define CSL_PCID_IR_DWIDTH_16BIT                 (16u)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  struct CslPcid_FrameConfig
 *  \brief PCID Frame configurations
 *
 */

typedef struct
{
    uint32_t    width;
    /**< Frame width in pixels */
    uint32_t    height;
    /**< Frame heinght */
} CslPcid_FrameConfig;


/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Sets the frame configuration
 *
 *  \param pcidRegs         PCID Base Address, shall not be NULL
 *  \param cfg              Frame size configuration
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_pcidSetFrameConfig(CSL_pcidRegs *pcidRegs,
                                    const CslPcid_FrameConfig *cfg);

/**
 *  \brief Sets PCID configuration
 *
 *  \param pcidRegs         PCID Base Address, shall not be NULL
 *  \param pRemapLut        PCID IR remap LUT base address, shall not be NULL,
 *                              when IR remap is enabled
 *  \param cfg              PCID Configurations
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_pcidSetConfig(CSL_pcidRegs *pcidRegs,
                            CSL_irremap_lutRegs *pRemapLut,
                            const Pcid_Cfg *cfg);

#ifdef __cplusplus
}
#endif

#endif  /* CSL_PCID_H_ */
