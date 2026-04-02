/**
 *   Copyright (c) Texas Instruments Incorporated 2021-2022
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
 *  \file csl_nsf4.h
 *
 *  \brief CSL interface file for NSF4 module
 *
 */

#ifndef CSL_NSF4_H_
#define CSL_NSF4_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <include/nsf4_cfg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Horizontal latency required for NSF4V
 */
#if defined (VHWA_VPAC_IP_REV_VPAC3L)
#define CSL_NSF4V_HORZ_LATENCY                  (44u)
#else
#define CSL_NSF4V_HORZ_LATENCY                  (44u + 7u)
#endif

/** \brief Horizontal Blanking required for NSF4V
 */
#if defined (VHWA_VPAC_IP_REV_VPAC3L)
#define CSL_NSF4V_HORZ_BLANKING                 (28u)
#else
#define CSL_NSF4V_HORZ_BLANKING                 (30u)
#endif

/** \brief Vertical Blanking required for NSF4V
 */
#define CSL_NSF4V_VERT_BLANKING                 (15u)

/** \brief Bit Position of the TN Mode config in the Nsf4v_Config.mode variable
 */
#define CSL_NSF4V_TN_MODE_BIT                   (0x10U)

#if defined (VHWA_VPAC_IP_REV_VPAC3L)
/** \brief Horizontal latency required for NSF4V.DWB
 */
#define CSL_NSF4V_HORZ_LATENCY_DWB                 (7u)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  struct CslNsf4v_FrameConfig
 *  \brief VPAC NSF4v CSL configuration,
 *         Currently this structure is used to configure NSF4v input frame size.
 *
 *         The other modules of the NSF4v are configured using different APIs
 */
typedef struct
{
    uint32_t                        width;
    /**< Input frame width */
    uint32_t                        height;
    /**< Input frame height */
} CslNsf4v_FrameConfig;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Function to NSF4 configuration
 *
 *  \param nsf4Regs         NSF4 Register overlay
 *  \param cfg              Pointer to NSF4_Config structure
 *                          This parameter should be non-NULL.
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_nsf4vSetConfig(CSL_nsf4vRegs *nsf4Regs, const Nsf4v_Config *cfg);

int32_t CSL_nsf4vSetFrameConfig(CSL_nsf4vRegs *nsf4Regs,
    const CslNsf4v_FrameConfig *cfg);

int32_t CSL_nsf4SetHistLut(CSL_nsf4vRegs *nsf4Regs,
                           CSL_rawhist_lutRegs *nsf4LutRegs,
                           const Nsf4_HistLutConfig *lutCfg);

int32_t CSL_nsf4ReadHistogram(const CSL_rawhist_dataRegs *histRegs,
                        uint32_t histData[]);

int32_t CSL_nsf4ChangeBank(CSL_nsf4vRegs *nsf4Regs);

/* ========================================================================== */
/*                           Function Definition                              */
/* ========================================================================== */


#ifdef __cplusplus
}
#endif

#endif  /* CSL_NSF4_H_ */
