/*
 *
 * Copyright (C) 2010-2019 Texas Instruments Incorporated - http://www.ti.com/
 *
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

/*
 *  File Name: pciev3_app.c
 *
 *  Processing/configuration functions for the PCIe Application Registers
 *
 */

#include <ti/drv/pcie/pcie.h>
#include <ti/drv/pcie/src/pcieloc.h>
#include <ti/drv/pcie/src/v3/pcieloc.h>


/*****************************************************************************
 **********  PCIe CONFIG REGISTERS COMMON TO TYPE0 AND TYPE1  *****************
 ****************************************************************************/


/*****************************************************************************
 * These APIs are using the endpoint (Type 0) structure and #defines, but they
 * should be used for both EP and RC (Type 0 and Type 1) PCIe modes.
 * Both types have the same swRegister layout, in the same location.
 ****************************************************************************/

/*****************************************************************************
 **********  PCIe APPLICATION REGISTERS  *****************
 ****************************************************************************/

/*****************************************************************************
 * Read and split up the Peripheral Version and ID swRegister
 ****************************************************************************/
pcieRet_e pciev3_read_pid_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  pciePidReg_t               *swReg
)
{
  CSL_user_cfgRegs *userCfg = (CSL_user_cfgRegs *)baseAddr;
  uint32_t val = swReg->raw = userCfg->REVID;

  pcie_getbits(val, CSL_USER_CFG_REVID_MODID,  swReg->modId);
  pcie_getbits(val, CSL_USER_CFG_REVID_REVRTL,    swReg->rtl);
  pcie_getbits(val, CSL_USER_CFG_REVID_REVMAJ,  swReg->revMaj);
  pcie_getbits(val, CSL_USER_CFG_REVID_CUSTOM, swReg->cust);
  pcie_getbits(val, CSL_USER_CFG_REVID_REVMIN,  swReg->revMin);

  /* Set unused fields to 0 (only used by rev 0/1 hw) */
  swReg->scheme = 0u;
  swReg->func   = 0u;

  return pcie_RET_OK;
} /* pciev3_read_pid_reg */

/*****************************************************************************
 * Read and split up the Command Status swRegister
 ****************************************************************************/
pcieRet_e pciev3_read_cmdStatus_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  pcieCmdStatusReg_t         *swReg
)
{
  CSL_user_cfgRegs *userCfg = (CSL_user_cfgRegs *)baseAddr;
  uint32_t val = swReg->raw = userCfg->CMD_STATUS;
  
  pcie_getbits(val, CSL_USER_CFG_CMD_STATUS_LINK_TRAINING_ENABLE,        swReg->ltssmEn);

  /* Set unused fields to 0 (only used by rev 0/1 hw) */
  swReg->postedWrEn = 0u;
  swReg->ibXltEn    = 0u;
  swReg->obXltEn    = 0u;

  return pcie_RET_OK;
} /* pciev3_read_cmdStatus_reg */

/*****************************************************************************
 * Combine and write the Command Status swRegister
 ****************************************************************************/
pcieRet_e pciev3_write_cmdStatus_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  pcieCmdStatusReg_t   *swReg
)
{
  CSL_user_cfgRegs *userCfg = (CSL_user_cfgRegs *)baseAddr;
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  pcie_setbits(new_val, CSL_USER_CFG_CMD_STATUS_LINK_TRAINING_ENABLE,        swReg->ltssmEn);

  userCfg->CMD_STATUS = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_cmdStatus_reg */

/*****************************************************************************
 * Read and split up the LINKSTATUS swRegister
 ****************************************************************************/
pcieRet_e pciev3_read_linkStatus_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  pcieTiConfDeviceCmdReg_t         *swReg
)
{
  CSL_user_cfgRegs *userCfg = (CSL_user_cfgRegs *)baseAddr;
  uint32_t val = swReg->raw = userCfg->LINKSTATUS;
  
  pcie_getbits(val, CSL_USER_CFG_LINKSTATUS_LTSSM_STATE,        swReg->ltssmState);

  return pcie_RET_OK;
} /* pciev3_read_cmdStatus_reg */

/*****************************************************************************
 * Combine and write the LINKSTATUS swRegister
 ****************************************************************************/
pcieRet_e pciev3_write_linkStatus_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  pcieTiConfDeviceCmdReg_t   *swReg
)
{
  CSL_user_cfgRegs *userCfg = (CSL_user_cfgRegs *)baseAddr;
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  pcie_setbits(new_val, CSL_USER_CFG_LINKSTATUS_LTSSM_STATE,     swReg->ltssmState);

  userCfg->LINKSTATUS = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_cmdStatus_reg */

/*****************************************************************************
 * Read and split up the Endpoint Interrupt Request Set swRegister
 ****************************************************************************/
pcieRet_e pciev3_read_epIrqSet_reg
(
  const CSL_user_cfgRegs *baseAddr,
  pcieEpIrqSetReg_t          *swReg,
  int_fast32_t                swRegNum
)
{
    uint32_t val = swReg->raw = baseAddr->LEGACY_INTR_SET;
    
    switch (swRegNum)
    {
    case 0:
       pcie_getbits(val, CSL_USER_CFG_LEGACY_INTR_SET_INTA_IN, swReg->epIrqSet);
       break;
    case 1:
       pcie_getbits(val, CSL_USER_CFG_LEGACY_INTR_SET_INTB_IN, swReg->epIrqSet);
       break;
    case 2:
       pcie_getbits(val, CSL_USER_CFG_LEGACY_INTR_SET_INTC_IN, swReg->epIrqSet);
       break;
    case 3:
       pcie_getbits(val, CSL_USER_CFG_LEGACY_INTR_SET_INTD_IN, swReg->epIrqSet);
       break;
    }
  return pcie_RET_OK;
} /* pciev3_read_epIrqSet_reg */


/*****************************************************************************
 * Combine and write the Endpoint Interrupt Request Set swRegister
 ****************************************************************************/
pcieRet_e pciev3_write_epIrqSet_reg
(
  CSL_user_cfgRegs *baseAddr,
  pcieEpIrqSetReg_t    *swReg,
  int_fast32_t                swRegNum
)
{
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  switch (swRegNum)
  {
  case 0:
     pcie_setbits(new_val, CSL_USER_CFG_LEGACY_INTR_SET_INTA_IN, swReg->epIrqSet);
     break;
  case 1:
     pcie_setbits(new_val, CSL_USER_CFG_LEGACY_INTR_SET_INTB_IN, swReg->epIrqSet);
     break;
  case 2:
     pcie_setbits(new_val, CSL_USER_CFG_LEGACY_INTR_SET_INTC_IN, swReg->epIrqSet);
     break;
  case 3:
     pcie_setbits(new_val, CSL_USER_CFG_LEGACY_INTR_SET_INTD_IN, swReg->epIrqSet);
     break;
  }

  baseAddr->LEGACY_INTR_SET = swReg->raw = new_val;
  return pcie_range_check_return;
} /* pciev3_write_epIrqSet_reg */

/*****************************************************************************
 * Read and split up the Endpoint Interrupt Request Clear swRegister
 ****************************************************************************/
pcieRet_e pciev3_read_epIrqClr_reg
(
  const CSL_user_cfgRegs *baseAddr,
  pcieEpIrqClrReg_t          *swReg,
  int_fast32_t                swRegNum
)
{ 
    uint32_t val = swReg->raw = baseAddr->LEGACY_INTR_SET;
    
    switch (swRegNum)
    {
    case 0:
       pcie_getbits(val, CSL_USER_CFG_LEGACY_INTR_SET_INTA_IN, swReg->epIrqClr);
       break;
    case 1:
       pcie_getbits(val, CSL_USER_CFG_LEGACY_INTR_SET_INTB_IN, swReg->epIrqClr);
       break;
    case 2:
       pcie_getbits(val, CSL_USER_CFG_LEGACY_INTR_SET_INTC_IN, swReg->epIrqClr);
       break;
    case 3:
       pcie_getbits(val, CSL_USER_CFG_LEGACY_INTR_SET_INTD_IN, swReg->epIrqClr);
       break;
    }
  return pcie_RET_OK;
} /* pciev3_read_epIrqClr_reg */

/*****************************************************************************
 * Combine and write the Endpoint Interrupt Request Clear swRegister
 ****************************************************************************/
pcieRet_e pciev3_write_epIrqClr_reg
(
  CSL_user_cfgRegs *baseAddr,
  pcieEpIrqClrReg_t    *swReg,
  int_fast32_t                swRegNum
)
{
    uint32_t new_val = swReg->raw;
    pcie_range_check_begin;
    
    switch (swRegNum)
    {
    case 0:
       pcie_setbits(new_val, CSL_USER_CFG_LEGACY_INTR_SET_INTA_IN, swReg->epIrqClr);
       break;
    case 1:
       pcie_setbits(new_val, CSL_USER_CFG_LEGACY_INTR_SET_INTB_IN, swReg->epIrqClr);
       break;
    case 2:
       pcie_setbits(new_val, CSL_USER_CFG_LEGACY_INTR_SET_INTC_IN, swReg->epIrqClr);
       break;
    case 3:
       pcie_setbits(new_val, CSL_USER_CFG_LEGACY_INTR_SET_INTD_IN, swReg->epIrqClr);
       break;
    }

  baseAddr->LEGACY_INTR_SET = swReg->raw = new_val;
  return pcie_range_check_return;
} /* pciev3_write_epIrqClr_reg */

/*****************************************************************************
 * Read and split up a Legacy Interrupt Status swRegister
 ****************************************************************************/
pcieRet_e pciev3_read_legacyIrqStatus_reg
(
  const CSL_intd_cfgRegs *baseAddr,
  pcieLegacyIrqStatusReg_t   *swReg,
  int_fast32_t                swRegNum
)
{
  /* swRegNum generated internally no need for bounds check */
  uint32_t val = swReg->raw = baseAddr->STATUS_REG_SYS_0;

  switch (swRegNum)
  {
  case 0:
     pcie_getbits(val, CSL_INTD_CFG_STATUS_REG_SYS_0_STATUS_SYS_PCIE_LEGACY_0, swReg->legacyIrqStatus);
     break;
  case 1:
     pcie_getbits(val, CSL_INTD_CFG_STATUS_REG_SYS_0_STATUS_SYS_PCIE_LEGACY_1, swReg->legacyIrqStatus);
     break;
  case 2:
     pcie_getbits(val, CSL_INTD_CFG_STATUS_REG_SYS_0_STATUS_SYS_PCIE_LEGACY_2, swReg->legacyIrqStatus);
     break;
  case 3:
     pcie_getbits(val, CSL_INTD_CFG_STATUS_REG_SYS_0_STATUS_SYS_PCIE_LEGACY_3, swReg->legacyIrqStatus);
     break;
  }

  return pcie_RET_OK;
} /* pciev3_read_legacyIrqStatus_reg */


/*****************************************************************************
 * Combine and write a Legacy Interrupt Status swRegister
 ****************************************************************************/
pcieRet_e pciev3_write_legacyIrqStatus_reg
(
  CSL_intd_cfgRegs     *baseAddr,
  pcieLegacyIrqStatusReg_t *swReg,
  int_fast32_t              swRegNum
)
{
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  switch (swRegNum)
  {
  case 0:
     pcie_setbits(new_val, CSL_INTD_CFG_STATUS_REG_SYS_0_STATUS_SYS_PCIE_LEGACY_0, swReg->legacyIrqStatus);
     break;
  case 1:
     pcie_setbits(new_val, CSL_INTD_CFG_STATUS_REG_SYS_0_STATUS_SYS_PCIE_LEGACY_1, swReg->legacyIrqStatus);
     break;
  case 2:
     pcie_setbits(new_val, CSL_INTD_CFG_STATUS_REG_SYS_0_STATUS_SYS_PCIE_LEGACY_2, swReg->legacyIrqStatus);
     break;
  case 3:
     pcie_setbits(new_val, CSL_INTD_CFG_STATUS_REG_SYS_0_STATUS_SYS_PCIE_LEGACY_3, swReg->legacyIrqStatus);
     break;
  }

  /* swRegNum generated internally no need for bounds check */
  baseAddr->STATUS_REG_SYS_0 = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_legacyIrqStatus_reg */

/*****************************************************************************
 * Read and split up a Legacy Interrupt Enable Set swRegister
 ****************************************************************************/
pcieRet_e pciev3_read_legacyIrqEnableSet_reg
(
  const CSL_intd_cfgRegs  *baseAddr,
  pcieLegacyIrqEnableSetReg_t *swReg,
  int_fast32_t                 swRegNum
)
{
  /* swRegNum generated internally no need for bounds check */
  uint32_t val = swReg->raw = baseAddr->ENABLE_REG_SYS_0;

  switch (swRegNum)
  {
  case 0:
     pcie_getbits(val, CSL_INTD_CFG_ENABLE_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_0, swReg->legacyIrqEnSet);
     break;
  case 1:
     pcie_getbits(val, CSL_INTD_CFG_ENABLE_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_1, swReg->legacyIrqEnSet);
     break;
  case 2:
     pcie_getbits(val, CSL_INTD_CFG_ENABLE_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_2, swReg->legacyIrqEnSet);
     break;
  case 3:
     pcie_getbits(val, CSL_INTD_CFG_ENABLE_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_3, swReg->legacyIrqEnSet);
     break;
  }
  return pcie_RET_OK;
} /* pciev3_read_legacyIrqEnableSet_reg */

/*****************************************************************************
 * Combine and write a Legacy Interrupt Enable Set swRegister
 ****************************************************************************/
pcieRet_e pciev3_write_legacyIrqEnableSet_reg
(
  CSL_intd_cfgRegs        *baseAddr,
  pcieLegacyIrqEnableSetReg_t *swReg,
  int_fast32_t                 swRegNum
)
{
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  switch (swRegNum)
  {
  case 0:
     pcie_setbits(new_val, CSL_INTD_CFG_ENABLE_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_0, swReg->legacyIrqEnSet);
     break;
  case 1:
     pcie_setbits(new_val, CSL_INTD_CFG_ENABLE_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_1, swReg->legacyIrqEnSet);
     break;
  case 2:
     pcie_setbits(new_val, CSL_INTD_CFG_ENABLE_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_2, swReg->legacyIrqEnSet);
     break;
  case 3:
     pcie_setbits(new_val, CSL_INTD_CFG_ENABLE_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_3, swReg->legacyIrqEnSet);
     break;
  }

  /* swRegNum generated internally no need for bounds check */
  baseAddr->ENABLE_REG_SYS_0 = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_legacyIrqEnableSet_reg */

/*****************************************************************************
 * Read and split up the Legacy Interrupt Enable Clear swRegister
 ****************************************************************************/
pcieRet_e pciev3_read_legacyIrqEnableClr_reg
(
  const CSL_intd_cfgRegs  *baseAddr,
  pcieLegacyIrqEnableClrReg_t *swReg,
  int_fast32_t                 swRegNum
)
{ 
  /* swRegNum generated internally no need for bounds check */
  uint32_t val = swReg->raw = baseAddr->ENABLE_CLR_REG_SYS_0;

  switch (swRegNum)
  {
  case 0:
     pcie_getbits(val, CSL_INTD_CFG_ENABLE_CLR_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_0_CLR, swReg->legacyIrqEnClr);
     break;
  case 1:
     pcie_getbits(val, CSL_INTD_CFG_ENABLE_CLR_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_1_CLR, swReg->legacyIrqEnClr);
     break;
  case 2:
     pcie_getbits(val, CSL_INTD_CFG_ENABLE_CLR_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_2_CLR, swReg->legacyIrqEnClr);
     break;
  case 3:
     pcie_getbits(val, CSL_INTD_CFG_ENABLE_CLR_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_3_CLR, swReg->legacyIrqEnClr);
     break;
  }

  return pcie_RET_OK;
} /* pciev3_read_legacyIrqEnableClr_reg */


/*****************************************************************************
 * Combine and write the Legacy Interrupt Enable Clear swRegister
 ****************************************************************************/
pcieRet_e pciev3_write_legacyIrqEnableClr_reg
(
  CSL_intd_cfgRegs        *baseAddr,
  pcieLegacyIrqEnableClrReg_t *swReg,
  int_fast32_t                 swRegNum
)
{
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  switch (swRegNum)
  {
  case 0:
     pcie_setbits(new_val, CSL_INTD_CFG_ENABLE_CLR_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_0_CLR, swReg->legacyIrqEnClr);
     break;
  case 1:
     pcie_setbits(new_val, CSL_INTD_CFG_ENABLE_CLR_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_1_CLR, swReg->legacyIrqEnClr);
     break;
  case 2:
     pcie_setbits(new_val, CSL_INTD_CFG_ENABLE_CLR_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_2_CLR, swReg->legacyIrqEnClr);
     break;
  case 3:
     pcie_setbits(new_val, CSL_INTD_CFG_ENABLE_CLR_REG_SYS_0_ENABLE_SYS_EN_PCIE_LEGACY_3_CLR, swReg->legacyIrqEnClr);
     break;
  }
  baseAddr->ENABLE_CLR_REG_SYS_0 = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_legacyIrqEnableClr_reg */

/* Nothing past this point */

