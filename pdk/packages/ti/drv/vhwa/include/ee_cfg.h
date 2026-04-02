/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2018
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
 *  \ingroup  DRV_VISS_MODULE
 *  \defgroup DRV_EE_MODULE_CFG EE Configuration
 *            This is VISS EE Configuration file
 *
 *  @{
 */
/**
 *  \file   fcp_cfg.h
 *
 *  \brief  Inteface file for Flex Color processing module
 */

#ifndef EE_CFG_H_
#define EE_CFG_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_vpac_abstraction_layer.h>

#include <include/vhwa_common.h>


#ifdef __cplusplus
extern "C" {
#endif



/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


#define EE_LUT_SIZE         (4096U)



/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

typedef struct
{
    uint32_t enable;
    /**< Enable/Disable EE */
    uint32_t alignY12withChroma;
    uint32_t alignY8withChroma;
    uint32_t eeForY12OrY8;
    /**< Selects Y12 or Y8 for EE input
        0, Y12
        1, Y8 */
    uint32_t bypassY12;
    uint32_t bypassC12;
    uint32_t bypassY8;
    uint32_t bypassC8;

    uint32_t leftShift;
    uint32_t rightShift;

    uint32_t yeeShift;
    int32_t coeff[9];
    uint32_t yeeEThr;
    uint32_t yeeMergeSel;
    uint32_t haloReductionOn;
    uint32_t yesGGain;
    uint32_t yesEGain;
    uint32_t yesEThr1, yesEThr2;
    uint32_t yesGOfset;

    int32_t *lut;
} Ee_Config;

/* ========================================================================== */
/*                            Global Variables                                */
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

#endif  /* #ifndef EE_CFG_H_ */
 /** @} */
