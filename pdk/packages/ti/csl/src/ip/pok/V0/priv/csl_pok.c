/**
 * @file  csl_pok.c
 *
 * @brief
 *  C implementation file for the POK module CSL-FL.
 *
 *  Contains the different control command and status query functions definitions
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2020, Texas Instruments, Inc.
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
#include <string.h>
#include <stdbool.h>
#include <ti/csl/csl_pok.h>
#include <ti/csl/src/ip/pok/V0/csl_pokId2Addr.h>

/*=============================================================================
 *  Internal definitions and functions
 *===========================================================================*/
static int32_t CSL_por_get_override(const volatile uint32_t     *pBaseAddr,
                                    CSL_por_override     *pUpdateAddr,
                                    uint32_t     set_shift,
                                    uint32_t     set_mask,
                                    uint32_t     en_shift,
                                    uint32_t     en_mask);

static int32_t CSL_por_set_override(volatile uint32_t     *pBaseAddr,
                                    const CSL_por_override *pUpdateAddr,
                                    uint32_t     set_shift,
                                    uint32_t     set_mask,
                                    uint32_t     en_shift,
                                    uint32_t     en_mask);


/*=============================================================================
 *  internal macros
 *===========================================================================*/
/* None */

/*=============================================================================
 *  static global variables
 *===========================================================================*/
/* None */

/*=============================================================================
 *  Internal functions
 *===========================================================================*/
/* None */

/*=============================================================================
 *  Interface functions
 *===========================================================================*/

static int32_t CSL_por_get_override(const volatile uint32_t     *pBaseAddr,
                                    CSL_por_override      *pOverRdAddr,
                                    uint32_t     set_shift,
                                    uint32_t     set_mask,
                                    uint32_t     en_shift,
                                    uint32_t     en_mask)
{
    uint32_t    set, en;
    int32_t     retVal = CSL_PASS;

    set    = CSL_REG32_FEXT_RAW(pBaseAddr, set_mask, set_shift);
    en     = CSL_REG32_FEXT_RAW(pBaseAddr, en_mask, en_shift);
    
    if ((set == (uint32_t)TRUE) &&
        (en  == (uint32_t) TRUE) )
    {
        *pOverRdAddr = CSL_POR_OVERRIDE_NOT_SET_DISABLE;
    }
    
    else if ((set == (uint32_t)FALSE) &&
             (en  == (uint32_t) FALSE) )
    {
        *pOverRdAddr = CSL_POR_OVERRIDE_SET_ENABLE;
    }
    else
    {
        *pOverRdAddr = CSL_OVERRIDE_SET_UNKNOWN;
        retVal = CSL_EFAIL;
    }

    return(retVal);
}


static int32_t CSL_por_set_override(volatile uint32_t     *pBaseAddr,
                                    const  CSL_por_override     *pOverRdAddr,
                                    uint32_t     set_shift,
                                    uint32_t     set_mask,
                                    uint32_t     en_shift,
                                    uint32_t     en_mask)
{
    int32_t     retVal = CSL_PASS;

    if (*pOverRdAddr == CSL_POR_OVERRIDE_SET_ENABLE)
    {
        CSL_REG32_FINS_RAW(pBaseAddr, en_mask, en_shift,0U);
        CSL_REG32_FINS_RAW(pBaseAddr, set_mask, set_shift,0U);
    }
    else if (*pOverRdAddr == CSL_POR_OVERRIDE_NOT_SET_DISABLE)
    {
        CSL_REG32_FINS_RAW(pBaseAddr, en_mask, en_shift,1U);
        CSL_REG32_FINS_RAW(pBaseAddr, set_mask, set_shift,1U);
    }
    else if (*pOverRdAddr == CSL_POR_SET_OVERRIDE_NO_CHANGE)
    {
        /* No change */
    }
    else
    {
        retVal = CSL_EBADARGS;
    }

    return (retVal);

}

/**
* Requirement: REQ_TAG(PDK-5888) REQ_TAG(PDK-5890)
* Design: did_csl_pok_cfg did_csl_pok_interfaces
*/

int32_t CSL_pokSetControl   (CSL_wkupCtrlRegsBase_t           *pBaseAddress,
                             const CSL_pokCfg_t               *pPokCfg,
                             CSL_pok_id                        pokId)
{
    int32_t     retVal;

    if ((pBaseAddress          == NULL_PTR) ||
        (pPokCfg               == NULL_PTR) )
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        if ((pPokCfg->hystCtrl     == CSL_PWRSS_GET_HYSTERESIS_VALUE) ||
            (pPokCfg->voltDetMode  == CSL_PWRSS_GET_VOLTAGE_DET_MODE) ||
            (pPokCfg->trim         == CSL_PWRSS_GET_TRIM_VALUE) ||
            (pPokCfg->detectionCtrl == CSL_POK_GET_DETECTION_VALUE) ||
            (pPokCfg->pokEnSelSrcCtrl == CSL_POK_GET_ENSEL_VALUE))
        {
            retVal = CSL_EFAIL;
        }
        else if ((pPokCfg->hystCtrl        <= CSL_PWRSS_HYSTERESIS_NO_ACTION) ||
                 (pPokCfg->voltDetMode     <= CSL_PWRSS_VOLTAGE_DET_NO_ACTION) ||
                 (pPokCfg->trim            <= CSL_PWRSS_MAX_TRIM_VALUE) ||
                 (pPokCfg->detectionCtrl   <  CSL_POK_GET_DETECTION_VALUE) ||
                 (pPokCfg->pokEnSelSrcCtrl <  CSL_POK_GET_ENSEL_VALUE))
        {
            retVal = CSL_pokSetOperation(pBaseAddress,
                                         pPokCfg,
                                         pokId);
        }
        else
        {
            retVal = CSL_EBADARGS;
        }
    }
    return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5888) REQ_TAG(PDK-5890)
* Design: did_csl_pok_cfg did_csl_pok_interfaces
*/

int32_t CSL_pokGetControl   (CSL_wkupCtrlRegsBase_t           *pBaseAddress,
                             const CSL_pokCfg_t               *pPokCfg,
                             CSL_pokVal_t                     *pPokVal,
                             CSL_pok_id                        pokId)
{
    int32_t     retVal = CSL_PASS;

    if ( (pBaseAddress          == NULL_PTR) ||
         (pPokCfg               == NULL_PTR)  ||
         (pPokVal               == NULL_PTR) )
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        if ((pPokCfg->hystCtrl     == CSL_PWRSS_GET_HYSTERESIS_VALUE) ||
            (pPokCfg->voltDetMode  == CSL_PWRSS_GET_VOLTAGE_DET_MODE) ||
            (pPokCfg->trim         == CSL_PWRSS_GET_TRIM_VALUE)       ||
            (pPokCfg->detectionCtrl == CSL_POK_GET_DETECTION_VALUE)   ||
            (pPokCfg->pokEnSelSrcCtrl  == CSL_POK_GET_ENSEL_VALUE))
        {
            retVal = CSL_pokGetOperation(pBaseAddress,
                                         pPokCfg,
                                         pPokVal,
                                         pokId);
        }
        else if ((pPokCfg->hystCtrl        == CSL_PWRSS_HYSTERESIS_NO_ACTION) ||
                 (pPokCfg->voltDetMode     == CSL_PWRSS_VOLTAGE_DET_NO_ACTION) ||
                 (pPokCfg->trim            == CSL_PWRSS_TRIM_NO_ACTION) ||
                 (pPokCfg->detectionCtrl   == CSL_POK_DETECTION_NO_ACTION) ||
                 (pPokCfg->pokEnSelSrcCtrl == CSL_POK_ENSEL_NO_ACTION))
        {
            /* No Action */
        }
        else
        {
            retVal = CSL_EBADARGS;
        }
    }

    return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5888) REQ_TAG(PDK-5890)
* Design: did_csl_pok_cfg did_csl_pok_interfaces
*/

int32_t CSL_porGetStat   (const CSL_wkupCtrlRegsBase_t           *pBaseAddress,
                          CSL_pokPorStat_t                       *pPorStat)
{
    int32_t     retVal;
    const       CSL_wkup_ctrl_mmr_cfg0Regs      *pCtrlMMRCfgRegs = \
                                    (const CSL_wkup_ctrl_mmr_cfg0Regs *)pBaseAddress;

    if ( (pBaseAddress == NULL_PTR) ||
         (pPorStat     == NULL_PTR) )
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        pPorStat->porBGapOK = (Bool) CSL_REG32_FEXT(&pCtrlMMRCfgRegs->POR_STAT,
                                             WKUP_CTRL_MMR_CFG0_POR_STAT_BGOK);
        pPorStat->porModuleStatus = (CSL_por_module_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->POR_STAT,
                                             WKUP_CTRL_MMR_CFG0_POR_STAT_SOC_POR);
        retVal = CSL_PASS;
    }
    return (retVal);

}

/**
* Requirement: REQ_TAG(PDK-5888) REQ_TAG(PDK-5890)
* Design: did_csl_pok_cfg did_csl_pok_interfaces
*/

int32_t CSL_porSetControl (CSL_wkupCtrlRegsBase_t           *pBaseAddress,
                           const CSL_pokPorCfg_t            *pPorCfg)
{
    int32_t     retVal = CSL_PASS;

    CSL_wkup_ctrl_mmr_cfg0Regs      *pCtrlMMRCfgRegs = \
                                    (CSL_wkup_ctrl_mmr_cfg0Regs *)pBaseAddress;

    if ( (pBaseAddress == NULL_PTR) ||
         (pPorCfg      == NULL_PTR) ||
         (pPorCfg->trim_select > CSL_POR_TRIM_SELECTION_GET_VALUE) )
    {
        retVal = CSL_EBADARGS;
    }

    if (retVal == CSL_PASS)
    {
        /* Check for valid trim select */
        if ((pPorCfg->maskHHVOutputEnable != TRUE) &&
            (pPorCfg->maskHHVOutputEnable != FALSE))
        {
            retVal = CSL_EBADARGS;
        }
    }

    if (retVal == CSL_PASS)
    {
        retVal = CSL_por_set_override(&pCtrlMMRCfgRegs->POR_CTRL,
                                      &pPorCfg->override[CSL_PORHV_OVERRIDE_INDEX],
                                      CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET0_SHIFT,
                                      CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET0_MASK,
                                      CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD0_SHIFT,
                                      CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD0_MASK);

        if (retVal == CSL_PASS)
        {

            retVal = CSL_por_set_override(&pCtrlMMRCfgRegs->POR_CTRL,
                              &pPorCfg->override[CSL_BGAP_OVERRIDE_INDEX],
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET1_SHIFT,
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET1_MASK,
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD1_SHIFT,
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD1_MASK);
        }

        if (retVal == CSL_PASS)
        {

            retVal = CSL_por_set_override(&pCtrlMMRCfgRegs->POR_CTRL,
                              &pPorCfg->override[CSL_POKHV_OVERRIDE_INDEX],
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET2_SHIFT,
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET2_MASK,
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD2_SHIFT,
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD2_MASK);
        }

        if (retVal == CSL_PASS)
        {

            retVal = CSL_por_set_override(&pCtrlMMRCfgRegs->POR_CTRL,
                              &pPorCfg->override[CSL_POKLVA_OVERRIDE_INDEX],
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET3_SHIFT,
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET3_MASK,
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD3_SHIFT,
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD3_MASK);
        }

        if (retVal == CSL_PASS)
        {

            retVal = CSL_por_set_override(&pCtrlMMRCfgRegs->POR_CTRL,
                              &pPorCfg->override[CSL_POKLVB_OVERRIDE_INDEX],
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET4_SHIFT,
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET4_MASK,
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD4_SHIFT,
                              CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD4_MASK);
        }

        if (retVal == CSL_PASS)
        {
            /* Mask HHV output when applying new TRIM values */
            if (pPorCfg->maskHHVOutputEnable == FALSE)
            {
                CSL_REG32_FINS(&pCtrlMMRCfgRegs->POR_CTRL, WKUP_CTRL_MMR_CFG0_POR_CTRL_MASK_HHV, 0U);
            }
            else
            {
                CSL_REG32_FINS(&pCtrlMMRCfgRegs->POR_CTRL, WKUP_CTRL_MMR_CFG0_POR_CTRL_MASK_HHV, 1U);
            }

            if (pPorCfg->trim_select == CSL_POR_TRIM_SELECTION_FROM_HHV_DEFAULT)
            {
                CSL_REG32_FINS(&pCtrlMMRCfgRegs->POR_CTRL, WKUP_CTRL_MMR_CFG0_POR_CTRL_TRIM_SEL, 0U);
            }
            else if (pPorCfg->trim_select == CSL_POR_TRIM_SELECTION_FROM_CTRL_REGS)
            {
                CSL_REG32_FINS(&pCtrlMMRCfgRegs->POR_CTRL, WKUP_CTRL_MMR_CFG0_POR_CTRL_TRIM_SEL, 1U);
            }
            else
            {
                /* No Action */
            }
        }
    }

    return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5888) REQ_TAG(PDK-5890)
* Design: did_csl_pok_cfg did_csl_pok_interfaces
*/
int32_t CSL_porGetControl (CSL_wkupCtrlRegsBase_t           *pBaseAddress,
                           const CSL_pokPorCfg_t            *pPorCfg,
                           CSL_pokPorVal_t                  *pVal)
{
    int32_t     retVal;
    CSL_wkup_ctrl_mmr_cfg0Regs      *pCtrlMMRCfgRegs = \
                                    (CSL_wkup_ctrl_mmr_cfg0Regs *)pBaseAddress;

    if ( (pBaseAddress == NULL_PTR) ||
         (pPorCfg      == NULL_PTR)  ||
         (pVal         == NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        retVal = CSL_PASS;

        if (pPorCfg->trim_select == CSL_POR_TRIM_SELECTION_GET_VALUE)
        {
            /* Read the trim sel */
            pVal->trim_select = (CSL_por_trim_sel)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->POR_CTRL, WKUP_CTRL_MMR_CFG0_POR_CTRL_TRIM_SEL);
        }
        else
        {
            pVal->trim_select = CSL_PWRSS_INVALID_TRIM_VALUE;
            retVal = CSL_EFAIL;
        }

        /* Read the HHV mask output control */
        pVal->maskHHVOutputEnable = (Bool) CSL_REG32_FEXT(&pCtrlMMRCfgRegs->POR_CTRL, WKUP_CTRL_MMR_CFG0_POR_CTRL_MASK_HHV);

        if ((pPorCfg->override[CSL_PORHV_OVERRIDE_INDEX]    == CSL_POR_GET_OVERRIDE_VALUE) &&
            (retVal                                        == CSL_PASS))
        {

            retVal = CSL_por_get_override(&pCtrlMMRCfgRegs->POR_CTRL,
                                          &pVal->override[CSL_PORHV_OVERRIDE_INDEX],
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET0_SHIFT,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET0_MASK,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD0_SHIFT,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD0_MASK
                                         );
        }
        else
        {
            /* Set the states to unknown */
            retVal = CSL_EFAIL;
        }

        if ((pPorCfg->override[CSL_BGAP_OVERRIDE_INDEX]    == CSL_POR_GET_OVERRIDE_VALUE) &&
            (retVal                                       == CSL_PASS))
        {
            retVal = CSL_por_get_override(&pCtrlMMRCfgRegs->POR_CTRL,
                                          &pVal->override[CSL_BGAP_OVERRIDE_INDEX],
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET1_SHIFT,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET1_MASK,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD1_SHIFT,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD1_MASK
                                         );
        }
        else
        {
            /* Set the states to unknown */
            retVal = CSL_EFAIL;
        }

        if ((pPorCfg->override[CSL_POKHV_OVERRIDE_INDEX]    == CSL_POR_GET_OVERRIDE_VALUE) &&
            (retVal                                       == CSL_PASS))
        {
            retVal = CSL_por_get_override(&pCtrlMMRCfgRegs->POR_CTRL,
                                          &pVal->override[CSL_POKHV_OVERRIDE_INDEX],
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET2_SHIFT,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET2_MASK,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD2_SHIFT,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD2_MASK
                                         );
        }
        else
        {
            /* Set the states to unknown */
            retVal = CSL_EFAIL;
        }

        if ((pPorCfg->override[CSL_POKLVA_OVERRIDE_INDEX]    == CSL_POR_GET_OVERRIDE_VALUE) &&
            (retVal                                       == CSL_PASS))
        {
            retVal = CSL_por_get_override(&pCtrlMMRCfgRegs->POR_CTRL,
                                          &pVal->override[CSL_POKLVA_OVERRIDE_INDEX],
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET3_SHIFT,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET3_MASK,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD3_SHIFT,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD3_MASK
                                         );
        }
        else
        {
            /* Set the states to unknown */
            retVal = CSL_EFAIL;
        }

        if ((pPorCfg->override[CSL_POKLVB_OVERRIDE_INDEX]    == CSL_POR_GET_OVERRIDE_VALUE) &&
            (retVal                                         == CSL_PASS))
        {
            retVal = CSL_por_get_override(&pCtrlMMRCfgRegs->POR_CTRL,
                                          &pVal->override[CSL_POKLVB_OVERRIDE_INDEX],
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET4_SHIFT,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD_SET4_MASK,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD4_SHIFT,
                                          CSL_WKUP_CTRL_MMR_CFG0_POR_CTRL_OVRD4_MASK
                                         );
        }
        else
        {
            /* Set the states to unknown */
            retVal = CSL_EFAIL;
        }
    }

    return (retVal);
}

/* Nothing past this point */
 
