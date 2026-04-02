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
 *  \file vhwa_soc.h
 *
 *  \brief VHWA Driver J721E SOC file containing private APIs used by VHWA
 *         controller driver.
 */

#ifndef VHWA_SOC_H_
#define VHWA_SOC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <drivers/hw_include/cslr_vpac.h>
#if defined(SOC_J722S)
    #include <drivers/hw_include/cslr_dmpac.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/** Number of spare socket for VISS */
#define VHWA_VISS_SPARE_SOCKET          (1u)
/** Number of spare socket for MSC */
#define VHWA_MSC_SPARE_SOCKET           (1u)
/**
 *  \anchor VHWA_irqNum
 *  \name   Interrupt numbers for VHWA
 *  \brief  This is default IRQ numbers for the VHWA IPs for the Main R5F core
 *          This can be changed by user, after initializing init parameters.
 *
 *  @{
 */
#if defined (SOC_AM62A) || defined (SOC_AM62AX)
    #define VHWA_M2M_VISS_IRQ_NUM                               CSLR_WKUP_R5FSS0_CORE0_INTR_VPAC0_VPAC_LEVEL_0 //(178u)
    #define VHWA_M2M_MSC0_IRQ_NUM                               CSLR_WKUP_R5FSS0_CORE0_INTR_VPAC0_VPAC_LEVEL_1 //(179u)
    #define VHWA_M2M_MSC1_IRQ_NUM                               CSLR_WKUP_R5FSS0_CORE0_INTR_VPAC0_VPAC_LEVEL_2 //(180u)
    #define VHWA_M2M_LDC_IRQ_NUM                                CSLR_WKUP_R5FSS0_CORE0_INTR_VPAC0_VPAC_LEVEL_3 //(182u)
#else
    #define VHWA_M2M_VISS_IRQ_NUM                               CSLR_R5FSS0_CORE0_INTR_VPAC0_VPAC_LEVEL_0 //(178u)
    #define VHWA_M2M_MSC0_IRQ_NUM                               CSLR_R5FSS0_CORE0_INTR_VPAC0_VPAC_LEVEL_1 //(179u)
    #define VHWA_M2M_MSC1_IRQ_NUM                               CSLR_R5FSS0_CORE0_INTR_VPAC0_VPAC_LEVEL_2 //(180u)
    #define VHWA_M2M_LDC_IRQ_NUM                                CSLR_R5FSS0_CORE0_INTR_VPAC0_VPAC_LEVEL_3 //(182u)
#endif

#define VHWA_VPAC_INTD_VISS_INTR_REG                        (0u)
#define VHWA_VPAC_INTD_LDC_INTR_REG                         (1u)
#define VHWA_VPAC_INTD_MSC_INTR_REG                         (2u)
#define VHWA_VPAC_INTD_NF_INTR_REG                          (2u)
#define VHWA_VPAC_INTD_WDTIMER_INTR_REG                     (3u)
#define VHWA_VPAC_INTD_PIPE_INTR_REG                        (3u)

#if defined(SOC_J722S)
#define VHWA_DMPAC_INTD_DOF_INTR_REG                        (0u)
#define VHWA_DMPAC_INTD_SDE_INTR_REG                        (0u)
#define VHWA_DMPAC_INTD_PIPE_INTR_REG                       (1u)
#define VHWA_DMPAC_INTD_WDTIMER_INTR_REG                    (2u)
#endif

/** \brief VPAC SL2 start Address */
#define VPAC_SL2_START_ADDRESS      (0x100000000ULL)
/** \brief DMPAC/VPAC SL2 Size */
#define VPAC_SL2_SIZE             (320U * 1024U)

#if defined(SOC_J722S) 
    #define VHWA_M2M_SDE_IRQ_NUM                                CSLR_R5FSS0_CORE0_INTR_DMPAC0_DMPAC_LEVEL_0 // CSLR_R5FSS0_CORE0_INTR_SAM67_DMPAC_WRAP0_DMPAC_LEVEL_0 //(5u)
    #define VHWA_M2M_DOF_IRQ_NUM                                CSLR_R5FSS0_CORE0_INTR_DMPAC0_DMPAC_LEVEL_1 //CSLR_R5FSS0_CORE0_INTR_SAM67_DMPAC_WRAP0_DMPAC_LEVEL_1 //(6u)

    /** \brief DMPAC/VPAC SL2 Size */
    #define DMPAC_SL2_START_ADDRESS     (0x100000000ULL) //UTC View //(CSL_SAM67_DMPAC_WRAP0_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV_BASE) //SOC view, would have performance degradation
    #define DMPAC_SL2_SIZE  (0x80000UL)//(CSL_DMPAC0_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV_SIZE) //(CSL_SAM67_DMPAC_WRAP0_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV_SIZE)
#endif





/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    uint32_t           irqNum;
    /**< Core IRQ Number to be used for LDC,
     *   Used internally for isr registration */
    uint32_t           vhwaIrqNum;
    /**< VHWA IRQ Number,
     *   Refer to #VHWA_IrqNum for valid number.
     *   Used to enable the irq in INTD number */
} Vhwa_IrqInfo;

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief API to get Soc Specific information, like base address,
 *         irq number ete., for MSC Driver.
 *
 *  \param  None
 *
 *  \return Pointer to Vhwa_IrqInfo object
 */
void Msc_getIrqInfo(Vhwa_IrqInfo irqInfo[]);

/**
 *  \brief API to get Soc Specific information, like base address,
 *         irq number ete., for LDC Driver.
 *
 *  \param  None
 *
 *  \return Pointer to Vhwa_IrqInfo object
 */
void Ldc_getIrqInfo(Vhwa_IrqInfo *irqInfo);

/**
 *  \brief API to get interrupt number for DOF,
 *
 *  \param  None
 *
 *  \return Pointer to Vhwa_IrqInfo object
 */
void Dof_getIrqInfo(Vhwa_IrqInfo *irqInfo);

/**
 *  \brief API to get Soc Specific information, like base address,
 *         irq number ete., for VISS Driver.
 *
 *  \param  None
 *
 *  \return Pointer to Vhwa_IrqInfo object
 */
void Viss_getIrqInfo(Vhwa_IrqInfo *irqInfo);

/**
 *  \brief API to get Soc Specific information, like base address,
 *         irq number ete., for NF Driver.
 *
 *  \param  None
 *
 *  \return Pointer to Vhwa_IrqInfo object
 */
void Nf_getIrqInfo(Vhwa_IrqInfo *irqInfo);

/**
 *  \brief API to get interrupt number for SDE
 *
 *  \param  None
 *
 *  \return Pointer to Vhwa_IrqInfo object
 */
void Sde_getIrqInfo(Vhwa_IrqInfo *irqInfo);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef VHWA_SOC_H_ */
