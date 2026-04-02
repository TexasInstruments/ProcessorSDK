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
 *
 */

/**
 *  \file vhwa_dmpac_soc.c
 *
 *  \brief File containing the SOC related configuration functions for VHWA DMPAC.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <soc/vhwa_soc.h>
#include <include/vhwa_common.h>
#include <soc/vhwa_dmpac_priv.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Global containing SoC information. */
static Dof_SocInfo gDof_SocInfo = {
    (CSL_dmpacRegs *)CSL_DMPAC0_DMPAC_REGS_DMPAC_REGS_CFG_IP_MMRS_BASE,
    (CSL_dmpac_intd_cfgRegs *)CSL_DMPAC0_CP_INTD_CFG_INTD_CFG_BASE,
    (CSL_dmpac_foco_coreRegs *)CSL_DMPAC0_DMPAC_FOCO_0_CFG_SLV_DMPAC_FOCO_CORE_FOCO_REGS_CFG_IP_MMRS_BASE,
    (CSL_dmpac_dofRegs *)CSL_DMPAC0_PAR_DOF_CFG_VP_MMR_VBUSP_DOFCORE_BASE,
    (CSL_lseRegs *)CSL_DMPAC0_DMPAC_FOCO_0_CFG_SLV_VPAC_FOCO_LSE_CFG_VP_BASE,
    (CSL_htsRegs *)CSL_DMPAC0_HTS_S_VBUSP_BASE
};

static Sde_SocInfo gSde_SocInfo = {
    (CSL_dmpacRegs *)CSL_DMPAC0_DMPAC_REGS_DMPAC_REGS_CFG_IP_MMRS_BASE,
    (CSL_dmpac_intd_cfgRegs *)CSL_DMPAC0_CP_INTD_CFG_INTD_CFG_BASE,
    (CSL_dmpac_foco_coreRegs *)CSL_DMPAC0_DMPAC_FOCO_1_CFG_SLV_DMPAC_FOCO_CORE_FOCO_REGS_CFG_IP_MMRS_BASE,
    (CSL_dmpac_sdeRegs *)CSL_DMPAC0_PAR_PAR_SDE_S_VBUSP_MMR_VBUSP_MMR_BASE,
    (CSL_lseRegs *)CSL_DMPAC0_DMPAC_FOCO_1_CFG_SLV_VPAC_FOCO_LSE_CFG_VP_BASE,
    (CSL_htsRegs *)CSL_DMPAC0_HTS_S_VBUSP_BASE
};


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void Dof_getSocInfo(Dof_SocInfo *socInfo)
{
/* LDRA_JUSTIFY_START
<metric start> branch <metric end>
<justification start> 
Rationale: The negative test framework and test apps cannot reach the false case of this condition. 
           'socInfo' is passed using Fvid2Control from Imaging kernel layer along with pointer to initPrms which holds the pointer to socInfo, initPrms is defined as an object in appVhwaDmpacInit().
Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
However, due to the stated rationale, this is not tested.
<justification end> */
    if (NULL != socInfo)
/* LDRA_JUSTIFY_END */
    {
        socInfo->dmpacCntlRegs = gDof_SocInfo.dmpacCntlRegs;
        socInfo->dmpacIntdRegs = gDof_SocInfo.dmpacIntdRegs;
        socInfo->dmpacFocoRegs = gDof_SocInfo.dmpacFocoRegs;
        socInfo->dofRegs      = gDof_SocInfo.dofRegs;
        socInfo->lseRegs      = gDof_SocInfo.lseRegs;
        socInfo->htsRegs      = gDof_SocInfo.htsRegs;
    }
}

void Dof_getIrqInfo(Vhwa_IrqInfo *irqInfo)
{
/* LDRA_JUSTIFY_START
<metric start> branch <metric end>
<justification start> 
Rationale: The negative test framework and test apps cannot reach the false case of this condition. 
           'irqInfo' is passed using Fvid2Control from Imaging kernel layer along with pointer to initPrms which holds the pointer to irqInfo, initPrms is defined as an object in appVhwaDmpacInit().
Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
However, due to the stated rationale, this is not tested.
<justification end> */
    if (NULL != irqInfo)
/* LDRA_JUSTIFY_END */
    {
        irqInfo->irqNum     = VHWA_M2M_DOF_IRQ_NUM;
        irqInfo->vhwaIrqNum = VHWA_M2M_DMPAC_IRQ_NUM_1;
    }
}

void Sde_getSocInfo(Sde_SocInfo *socInfo)
{
/* LDRA_JUSTIFY_START
<metric start> branch <metric end>
<justification start> 
Rationale: The negative test framework and test apps cannot reach the false case of this condition. 
           'socInfo' is passed using Fvid2Control from Imaging kernel layer along with pointer to initPrms which holds the pointer to socInfo, initPrms is defined as an object in appVhwaDmpacInit().
Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
However, due to the stated rationale, this is not tested.
<justification end> */
    if (NULL != socInfo)
/* LDRA_JUSTIFY_END */
    {
        socInfo->dmpacCntlRegs = gSde_SocInfo.dmpacCntlRegs;
        socInfo->dmpacIntdRegs = gSde_SocInfo.dmpacIntdRegs;
        socInfo->dmpacFocoRegs = gSde_SocInfo.dmpacFocoRegs;
        socInfo->sdeRegs      = gSde_SocInfo.sdeRegs;
        socInfo->lseRegs      = gSde_SocInfo.lseRegs;
        socInfo->htsRegs      = gSde_SocInfo.htsRegs;
    }
}

void Sde_getIrqInfo(Vhwa_IrqInfo *irqInfo)
{
/* LDRA_JUSTIFY_START
<metric start> branch <metric end>
<justification start> 
Rationale: The negative test framework and test apps cannot reach the false case of this condition. 
           'irqInfo' is passed using Fvid2Control from Imaging kernel layer along with pointer to initPrms which holds the pointer to irqInfo, initPrms is defined as an object in appVhwaDmpacInit().
Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
However, due to the stated rationale, this is not tested.
<justification end> */
    if (NULL != irqInfo)
/* LDRA_JUSTIFY_END */
    {
        irqInfo->irqNum     = VHWA_M2M_SDE_IRQ_NUM;
        irqInfo->vhwaIrqNum = VHWA_M2M_DMPAC_IRQ_NUM_0;
    }
}

