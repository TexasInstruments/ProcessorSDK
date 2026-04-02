/********************************************************************
 * Copyright (C) 2020 Texas Instruments Incorporated.
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
 *  Name        : cslr_compute_cluster_cc.h
*/
#ifndef CSLR_COMPUTE_CLUSTER_CC_H_
#define CSLR_COMPUTE_CLUSTER_CC_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <ti/csl/cslr.h>
#include <stdint.h>

/**************************************************************************
* Module Base Offset Values
**************************************************************************/

#define CSL_COMPUTE_CLUSTER_CC_REGS_BASE                          (0x00000000U)


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t ARM_0_REV;
    volatile uint8_t  Resv_256[252];
    volatile uint32_t ARM_0_PBIST_BISOR_CNTRL;
} CSL_compute_cluster_ccRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV                          (0x00000000U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_PBIST_BISOR_CNTRL            (0x00000100U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* ARM_0_REV */

#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_SCHEME_MASK              (0xC0000000U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_SCHEME_SHIFT             (0x0000001EU)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_SCHEME_MAX               (0x00000003U)

#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_BU_MASK                  (0x30000000U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_BU_SHIFT                 (0x0000001CU)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_BU_MAX                   (0x00000003U)

#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_MODULE_ID_MASK           (0x0FFF0000U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_MODULE_ID_SHIFT          (0x00000010U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_MODULE_ID_MAX            (0x00000FFFU)

#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_REVRTL_MASK              (0x0000F800U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_REVRTL_SHIFT             (0x0000000BU)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_REVRTL_MAX               (0x0000001FU)

#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_REVMAX_MASK              (0x00000700U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_REVMAX_SHIFT             (0x00000008U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_REVMAX_MAX               (0x00000007U)

#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_CUSTOM_MASK              (0x000000C0U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_CUSTOM_SHIFT             (0x00000006U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_CUSTOM_MAX               (0x00000003U)

#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_REVMIN_MASK              (0x0000003FU)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_REVMIN_SHIFT             (0x00000000U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_REV_REVMIN_MAX               (0x0000003FU)


/* ARM_0_PBIST_BISOR_CNTRL */

#define CSL_COMPUTE_CLUSTER_CC_ARM_0_PBIST_BISOR_CNTRL_RESERVED_MASK (0xFFFFFFFEU)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_PBIST_BISOR_CNTRL_RESERVED_SHIFT (0x00000001U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_PBIST_BISOR_CNTRL_RESERVED_MAX (0x7FFFFFFFU)

#define CSL_COMPUTE_CLUSTER_CC_ARM_0_PBIST_BISOR_CNTRL_DFT_SELF_REPAIR_MODE_OVERRIDE_MASK (0x00000001U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_PBIST_BISOR_CNTRL_DFT_SELF_REPAIR_MODE_OVERRIDE_SHIFT (0x00000000U)
#define CSL_COMPUTE_CLUSTER_CC_ARM_0_PBIST_BISOR_CNTRL_DFT_SELF_REPAIR_MODE_OVERRIDE_MAX (0x00000001U)


#ifdef __cplusplus
}
#endif
#endif
