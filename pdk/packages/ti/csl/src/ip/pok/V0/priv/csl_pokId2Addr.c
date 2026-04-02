/**
 * @file  csl_pokId2Addr.c
 *
 * @brief
 *  C implementation file for the POK module CSL-FL.
 *
 *  Translates POK ID to POK Address. This is a SOC specific source file
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
typedef struct CSL_pokShiftsAndMasks
{
    volatile    uint32_t    *pokAddr;
    volatile    uint32_t    *pokDetAddr;
    volatile    uint32_t    *pokEnSelAddr;
    uint32_t    hystMask;
    uint32_t    hystShift;
    uint32_t    vdDetMask;
    uint32_t    vdDetShift;
    uint32_t    detEnMask;
    uint32_t    detEnShift;
    uint32_t    pokEnSelMask;
    uint32_t    pokEnSelShift;
    uint32_t    trimMask;
    uint32_t    trimShift;
} CSL_pokShiftsAndMasks_t;

static int32_t CSL_pokGetShiftsAndMasks(CSL_wkupCtrlRegsBase_t     *pBaseAddress, 
                                     CSL_pok_id pokId,
                                     CSL_pokShiftsAndMasks_t *pShMasks);

static int32_t CSL_pokGetShiftsAndMasks(CSL_wkupCtrlRegsBase_t     *pBaseAddress,
                                     CSL_pok_id pokId,
                                     CSL_pokShiftsAndMasks_t *pShMasks)
{
    CSL_wkup_ctrl_mmr_cfg0Regs      *pCtrlMMRCfgRegs = \
                                    (CSL_wkup_ctrl_mmr_cfg0Regs *)pBaseAddress;
    int32_t                         retVal = CSL_PASS;

    switch (pokId)
    {
         /* PMIC POK ID */
         case    CSL_POK_VDDA_PMIC_IN_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VDDA_PMIC_IN_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDA_PMIC_IN_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDA_PMIC_IN_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDA_PMIC_IN_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDA_PMIC_IN_CTRL_OVER_VOLT_DET_SHIFT;
 
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->WKUP_PRG0_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_VDDA_PMIC_IN_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_VDDA_PMIC_IN_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->WKUP_PRG0_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_EN_SEL_SHIFT;
             break;
 
             /* CORE POK Under Voltage ID */
         case     CSL_POK_VDD_CORE_UV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VDD_CORE_UV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CORE_UV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CORE_UV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CORE_UV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CORE_UV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CORE_UV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CORE_UV_CTRL_POK_TRIM_SHIFT;
 
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VDD_CORE_UV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VDD_CORE_UV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* CPU under voltage POK ID */
         case     CSL_POK_VDD_CPU_UV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VDD_CPU_UV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CPU_UV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CPU_UV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CPU_UV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CPU_UV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CPU_UV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CPU_UV_CTRL_POK_TRIM_SHIFT;
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VDD_CPU_UV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VDD_CPU_UV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_SHIFT;
             break;
 
             /* Wakeup General POK Under Voltage ID */
         case     CSL_POK_VDDSHV_WKUP_GEN_UV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VDDSHV_WKUP_GEN_UV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDSHV_WKUP_GEN_UV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDSHV_WKUP_GEN_UV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDSHV_WKUP_GEN_UV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDSHV_WKUP_GEN_UV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDSHV_WKUP_GEN_UV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDSHV_WKUP_GEN_UV_CTRL_POK_TRIM_SHIFT;
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->WKUP_PRG0_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_VDDSHV_WKUP_GEN_UV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_VDDSHV_WKUP_GEN_UV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->WKUP_PRG0_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* MCU under voltage VDD POK ID */
         case     CSL_POK_VDDR_MCU_UV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VDDR_MCU_UV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_MCU_UV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_MCU_UV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_MCU_UV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_MCU_UV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_MCU_UV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_MCU_UV_CTRL_POK_TRIM_SHIFT;
 
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->WKUP_PRG0_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_VDDR_MCU_UV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_VDDR_MCU_UV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->WKUP_PRG0_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* VMON under voltage POK ID */
         case     CSL_POK_VMON_EXT_UV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VMON_EXT_UV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VMON_EXT_UV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VMON_EXT_UV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VMON_EXT_UV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VMON_EXT_UV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VMON_EXT_UV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VMON_EXT_UV_CTRL_POK_TRIM_SHIFT;
 
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VMON_EXT_UV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VMON_EXT_UV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* MCU overvoltage POK ID */
         case     CSL_POK_VDD_MCU_OV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VDD_MCU_OV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_MCU_OV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_MCU_OV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_MCU_OV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_MCU_OV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_MCU_OV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_MCU_OV_CTRL_POK_TRIM_SHIFT;
 
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->WKUP_PRG0_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_VDD_MCU_OV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_VDD_MCU_OV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->WKUP_PRG0_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* VDD CORE POK ID */
         case     CSL_POK_VDDR_CORE_UV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VDDR_CORE_UV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_CORE_UV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_CORE_UV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_CORE_UV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_CORE_UV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_CORE_UV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_CORE_UV_CTRL_POK_TRIM_SHIFT;
 
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VDDR_CORE_UV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VDDR_CORE_UV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* Wakeup General POK Over voltage ID */
         case     CSL_POK_VDDSHV_WKUP_GEN_OV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VDDSHV_WKUP_GEN_OV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDSHV_WKUP_GEN_OV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDSHV_WKUP_GEN_OV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDSHV_WKUP_GEN_OV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDSHV_WKUP_GEN_OV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDSHV_WKUP_GEN_OV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDSHV_WKUP_GEN_OV_CTRL_POK_TRIM_SHIFT;
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->WKUP_PRG0_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_VDDSHV_WKUP_GEN_OV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_VDDSHV_WKUP_GEN_OV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->WKUP_PRG0_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* CORE VDD Over Voltage POK ID */
         case     CSL_POK_VDD_CORE_OV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VDD_CORE_OV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CORE_OV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CORE_OV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CORE_OV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CORE_OV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CORE_OV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CORE_OV_CTRL_POK_TRIM_SHIFT;
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VDD_CORE_OV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VDD_CORE_OV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_SHIFT;
             break;
 
             /* MCU  over Voltage POK ID */
         case     CSL_POK_VDDR_MCU_OV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VDDR_MCU_OV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_MCU_OV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_MCU_OV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_MCU_OV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_MCU_OV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_MCU_OV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_MCU_OV_CTRL_POK_TRIM_SHIFT;
 
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->WKUP_PRG0_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_VDDR_MCU_OV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_VDDR_MCU_OV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->WKUP_PRG0_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG0_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* CPU  over Voltage POK ID */
         case     CSL_POK_VDD_CPU_OV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VDD_CPU_OV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CPU_OV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CPU_OV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CPU_OV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CPU_OV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CPU_OV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDD_CPU_OV_CTRL_POK_TRIM_SHIFT;
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VDD_CPU_OV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VDD_CPU_OV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* CORE VDDR over Voltage POK ID */
         case     CSL_POK_VDDR_CORE_OV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VDDR_CORE_OV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_CORE_OV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_CORE_OV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_CORE_OV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_CORE_OV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_CORE_OV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VDDR_CORE_OV_CTRL_POK_TRIM_SHIFT;
 
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VDDR_CORE_OV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VDDR_CORE_OV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* VMON POK Over Voltage ID */
         case     CSL_POK_VMON_EXT_OV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POK_VMON_EXT_OV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VMON_EXT_OV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VMON_EXT_OV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POK_VMON_EXT_OV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POK_VMON_EXT_OV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POK_VMON_EXT_OV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POK_VMON_EXT_OV_CTRL_POK_TRIM_SHIFT;
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VMON_EXT_OV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_VMON_EXT_OV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->MAIN_PRG_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_MAIN_PRG_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* POKHV Under Voltage POK ID */
         case     CSL_POR_POKHV_UV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POR_POKHV_UV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POR_POKHV_UV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POR_POKHV_UV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POR_POKHV_UV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POR_POKHV_UV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POR_POKHV_UV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POR_POKHV_UV_CTRL_POK_TRIM_SHIFT;
 
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->WKUP_PRG1_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG1_CTRL_POKHV_UV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG1_CTRL_POKHV_UV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->WKUP_PRG1_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG1_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG1_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* POKLV Under Voltage POK ID */
         case     CSL_POR_POKLV_UV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POR_POKLV_UV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POR_POKLV_UV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POR_POKLV_UV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POR_POKLV_UV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POR_POKLV_UV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POR_POKLV_UV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POR_POKLV_UV_CTRL_POK_TRIM_SHIFT;

             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->WKUP_PRG1_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG1_CTRL_POKLV_UV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG1_CTRL_POKLV_UV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->WKUP_PRG1_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG1_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG1_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
             /* POKHV Over Voltage POK ID */
         case     CSL_POR_POKHV_OV_ID:
             pShMasks->pokAddr     = &pCtrlMMRCfgRegs->POR_POKHV_OV_CTRL;
             pShMasks->hystMask    = CSL_WKUP_CTRL_MMR_CFG0_POR_POKHV_OV_CTRL_HYST_EN_MASK;
             pShMasks->hystShift   = CSL_WKUP_CTRL_MMR_CFG0_POR_POKHV_OV_CTRL_HYST_EN_SHIFT;
             pShMasks->vdDetMask   = CSL_WKUP_CTRL_MMR_CFG0_POR_POKHV_OV_CTRL_OVER_VOLT_DET_MASK;
             pShMasks->vdDetShift  = CSL_WKUP_CTRL_MMR_CFG0_POR_POKHV_OV_CTRL_OVER_VOLT_DET_SHIFT;
             pShMasks->trimMask    = CSL_WKUP_CTRL_MMR_CFG0_POR_POKHV_OV_CTRL_POK_TRIM_MASK;
             pShMasks->trimShift   = CSL_WKUP_CTRL_MMR_CFG0_POR_POKHV_OV_CTRL_POK_TRIM_SHIFT;
 
             /* POK Detection enable/disable control */
             pShMasks->pokDetAddr  = &pCtrlMMRCfgRegs->WKUP_PRG1_CTRL;
             pShMasks->detEnMask   = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG1_CTRL_POKHV_OV_EN_MASK;
             pShMasks->detEnShift  = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG1_CTRL_POKHV_OV_EN_SHIFT;
 
             /* POK Enable Selection SRC */
             pShMasks->pokEnSelAddr = &pCtrlMMRCfgRegs->WKUP_PRG1_CTRL;
             pShMasks->pokEnSelMask = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG1_CTRL_POK_EN_SEL_MASK;
             pShMasks->pokEnSelShift = CSL_WKUP_CTRL_MMR_CFG0_WKUP_PRG1_CTRL_POK_EN_SEL_SHIFT;
 
             break;
 
         default:
             pShMasks->pokAddr      = (volatile uint32_t *) NULL_PTR;
             pShMasks->pokDetAddr   = (volatile uint32_t *) NULL_PTR;
             pShMasks->pokEnSelAddr = (volatile uint32_t *) NULL_PTR;
             retVal  = CSL_EBADARGS;
             break;
     }

     return(retVal);

}

/*=============================================================================
 *  internal macros
 *===========================================================================*/
/* None */

/*=============================================================================
 *  static global variables
 *===========================================================================*/
/* None */

/*=============================================================================
 *  Non Interface function - internal use only
 *===========================================================================*/
int32_t  CSL_pokSetOperation(CSL_wkupCtrlRegsBase_t           *pBaseAddress,
                             const CSL_pokCfg_t               *pPokCfg,
                             CSL_pok_id                        pokId)
{
    int32_t                          retVal;
    CSL_pokShiftsAndMasks_t          shiftsNMasks;

    if ((pBaseAddress          == NULL_PTR) ||
        (pPokCfg               == NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        retVal =  CSL_pokGetShiftsAndMasks(pBaseAddress, pokId, &shiftsNMasks);
    }

    if (retVal == CSL_PASS)
    {
        if (pPokCfg->hystCtrl != CSL_PWRSS_HYSTERESIS_NO_ACTION)
        {
            CSL_REG32_FINS_RAW(shiftsNMasks.pokAddr, shiftsNMasks.hystMask, shiftsNMasks.hystShift, pPokCfg->hystCtrl);
        }

        if (pPokCfg->voltDetMode != CSL_PWRSS_VOLTAGE_DET_NO_ACTION)
        {
            CSL_REG32_FINS_RAW(shiftsNMasks.pokAddr, shiftsNMasks.vdDetMask, shiftsNMasks.vdDetShift, pPokCfg->voltDetMode);
        }

        if (pPokCfg->trim <= CSL_PWRSS_MAX_TRIM_VALUE)
        {
            CSL_REG32_FINS_RAW(shiftsNMasks.pokAddr, shiftsNMasks.trimMask, shiftsNMasks.trimShift, pPokCfg->trim);
        }

        if (pPokCfg->detectionCtrl == CSL_POK_DETECTION_ENABLE)
        {
            CSL_REG32_FINS_RAW(shiftsNMasks.pokDetAddr, shiftsNMasks.detEnMask, shiftsNMasks.detEnShift, CSL_POK_DETECTION_ENABLE);
        }

        if (pPokCfg->detectionCtrl == CSL_POK_DETECTION_DISABLE)
        {
            CSL_REG32_FINS_RAW(shiftsNMasks.pokDetAddr, shiftsNMasks.detEnMask, shiftsNMasks.detEnShift, CSL_POK_DETECTION_DISABLE);
        }

        if (pPokCfg->pokEnSelSrcCtrl == CSL_POK_ENSEL_HWTIEOFFS)
        {
            CSL_REG32_FINS_RAW(shiftsNMasks.pokEnSelAddr, shiftsNMasks.pokEnSelMask, shiftsNMasks.pokEnSelShift, CSL_POK_ENSEL_HWTIEOFFS);
        }

        if (pPokCfg->pokEnSelSrcCtrl == CSL_POK_ENSEL_PRG_CTRL)
        {
            CSL_REG32_FINS_RAW(shiftsNMasks.pokEnSelAddr, shiftsNMasks.pokEnSelMask, shiftsNMasks.pokEnSelShift, CSL_POK_ENSEL_PRG_CTRL);
        }

    }
    return (retVal);
}



int32_t  CSL_pokGetOperation(CSL_wkupCtrlRegsBase_t           *pBaseAddress,
                             const CSL_pokCfg_t               *pPokCfg,
                             CSL_pokVal_t                     *pPokVal,
                             CSL_pok_id                        pokId)

{
    CSL_wkup_ctrl_mmr_cfg0Regs      *pCtrlMMRCfgRegs = \
                                (CSL_wkup_ctrl_mmr_cfg0Regs *)pBaseAddress;
    int32_t                          retVal;
    CSL_pokShiftsAndMasks_t          shiftsNMasks;


    if ((pBaseAddress          == NULL_PTR) ||
        (pPokCfg               == NULL_PTR) ||
        (pPokVal               == NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        retVal = CSL_pokGetShiftsAndMasks(pBaseAddress, pokId, &shiftsNMasks);
        switch (pokId)
        {
            /* PMIC POK ID */
            case    CSL_POK_VDDA_PMIC_IN_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->WKUP_PRG0_STAT,
                                                           WKUP_CTRL_MMR_CFG0_WKUP_PRG0_STAT_POK_VDDA_PMIC_IN);
                break;

                /* CORE POK Under Voltage ID */
            case     CSL_POK_VDD_CORE_UV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->MAIN_PRG_STAT,
                                                           WKUP_CTRL_MMR_CFG0_MAIN_PRG_STAT_POK_VDD_CORE_UV);

                break;

                /* CPU under voltage POK ID */
            case     CSL_POK_VDD_CPU_UV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->MAIN_PRG_STAT,
                                                           WKUP_CTRL_MMR_CFG0_MAIN_PRG_STAT_POK_VDD_CPU_UV);

                break;

                /* Wakeup General POK Under Voltage ID */
            case     CSL_POK_VDDSHV_WKUP_GEN_UV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->WKUP_PRG0_STAT,
                                                           WKUP_CTRL_MMR_CFG0_WKUP_PRG0_STAT_POK_VDDSHV_WKUP_GEN_UV);

                break;

                /* MCU under voltage VDD POK ID */
            case     CSL_POK_VDDR_MCU_UV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->WKUP_PRG0_STAT,
                                                           WKUP_CTRL_MMR_CFG0_WKUP_PRG0_STAT_POK_VDDR_MCU_UV);
                break;

                /* VMON under voltage POK ID */
            case     CSL_POK_VMON_EXT_UV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->MAIN_PRG_STAT,
                                                           WKUP_CTRL_MMR_CFG0_MAIN_PRG_STAT_POK_VMON_EXT_UV);

                break;

                /* MCU overvoltage POK ID */
            case     CSL_POK_VDD_MCU_OV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->WKUP_PRG0_STAT,
                                                           WKUP_CTRL_MMR_CFG0_WKUP_PRG0_STAT_POK_VDD_MCU_OV);

                break;

                /* VDD CORE POK ID */
            case     CSL_POK_VDDR_CORE_UV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->MAIN_PRG_STAT,
                                                           WKUP_CTRL_MMR_CFG0_MAIN_PRG_STAT_POK_VDDR_CORE_UV);

                break;

                /* Wakeup General POK Over voltage ID */
            case     CSL_POK_VDDSHV_WKUP_GEN_OV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->WKUP_PRG0_STAT,
                                                           WKUP_CTRL_MMR_CFG0_WKUP_PRG0_STAT_POK_VDDSHV_WKUP_GEN_OV);

                break;

                /* CORE VDD Over Voltage POK ID */
            case     CSL_POK_VDD_CORE_OV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->MAIN_PRG_STAT,
                                                           WKUP_CTRL_MMR_CFG0_MAIN_PRG_STAT_POK_VDD_CORE_OV);

                break;

                /* MCU  over Voltage POK ID */
            case     CSL_POK_VDDR_MCU_OV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->WKUP_PRG0_STAT,
                                                           WKUP_CTRL_MMR_CFG0_WKUP_PRG0_STAT_POK_VDDR_MCU_OV);

                break;

                /* CPU  over Voltage POK ID */
            case     CSL_POK_VDD_CPU_OV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->MAIN_PRG_STAT,
                                                           WKUP_CTRL_MMR_CFG0_MAIN_PRG_STAT_POK_VDD_CPU_OV);

                break;

                /* CORE VDDR over Voltage POK ID */
            case     CSL_POK_VDDR_CORE_OV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->MAIN_PRG_STAT,
                                                           WKUP_CTRL_MMR_CFG0_MAIN_PRG_STAT_POK_VDDR_CORE_OV);

                break;

                /* VMON POK Over Voltage ID */
            case     CSL_POK_VMON_EXT_OV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->MAIN_PRG_STAT,
                                                           WKUP_CTRL_MMR_CFG0_MAIN_PRG_STAT_POK_VMON_EXT_UV);

                break;

                /* POKHV Under Voltage POK ID */
            case     CSL_POR_POKHV_UV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->WKUP_PRG1_STAT,
                                                           WKUP_CTRL_MMR_CFG0_WKUP_PRG1_STAT_POKHV_UV);

                break;

                /* POKLV Under Voltage POK ID */
            case     CSL_POR_POKLV_UV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->WKUP_PRG1_STAT,
                                                           WKUP_CTRL_MMR_CFG0_WKUP_PRG1_STAT_POKLV_UV);

                break;

                /* POKHV Over Voltage POK ID */
            case     CSL_POR_POKHV_OV_ID:
                /* Get the Voltage Threshold Status for this POK ID */
                pPokVal->voltageThrStatus = (CSL_voltage_thr_status)CSL_REG32_FEXT(&pCtrlMMRCfgRegs->WKUP_PRG1_STAT,
                                                           WKUP_CTRL_MMR_CFG0_WKUP_PRG1_STAT_POKHV_OV);

                break;

            default:
                retVal  = CSL_EBADARGS;
                break;
        }
    }

    if (retVal == CSL_PASS)
    {
        if (pPokCfg->hystCtrl == CSL_PWRSS_GET_HYSTERESIS_VALUE)
        {
            pPokVal->hystCtrl = (CSL_pwrss_hysteresis)CSL_REG32_FEXT_RAW(shiftsNMasks.pokAddr, shiftsNMasks.hystMask, shiftsNMasks.hystShift);
        }

        if (pPokCfg->voltDetMode == CSL_PWRSS_GET_VOLTAGE_DET_MODE)
        {
            pPokVal->voltDetMode = (CSL_pwrss_vd_mode)CSL_REG32_FEXT_RAW(shiftsNMasks.pokAddr, shiftsNMasks.vdDetMask, shiftsNMasks.vdDetShift);
        }

        if ((pPokCfg->trim <= CSL_PWRSS_MAX_TRIM_VALUE) &&
            (pokId != CSL_POK_VDDA_PMIC_IN_ID))
        {
            pPokVal->trim = (CSL_pwrss_trim)CSL_REG32_FEXT_RAW(shiftsNMasks.pokAddr, shiftsNMasks.trimMask, shiftsNMasks.trimShift);
        }

        if (pPokCfg->detectionCtrl == CSL_POK_GET_DETECTION_VALUE)
        {
            pPokVal->detectionStatus = (CSL_POK_detection_status)CSL_REG32_FEXT_RAW(shiftsNMasks.pokDetAddr, shiftsNMasks.detEnMask, shiftsNMasks.detEnShift);
        }

        if (pPokCfg->pokEnSelSrcCtrl == CSL_POK_GET_ENSEL_VALUE)
        {
            pPokVal->pokEnSelSrcCtrl = (CSL_POK_enSelSrc)CSL_REG32_FEXT_RAW(shiftsNMasks.pokEnSelAddr, shiftsNMasks.pokEnSelMask, shiftsNMasks.pokEnSelShift);
        }

    }

    return (retVal);

}


/* Nothing past this point */
 
