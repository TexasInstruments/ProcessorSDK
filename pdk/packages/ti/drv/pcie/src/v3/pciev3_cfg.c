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
 *  File Name: pciev3_cfg.c
 *
 *  Processing/configuration functions for the PCIe Configuration Registers
 *
 */

#include <ti/drv/pcie/pcie.h>
#include <ti/drv/pcie/src/pcieloc.h>
#include <ti/drv/pcie/src/v3/pcieloc.h>


/*****************************************************************************
 **********  PCIe CONFIG REGISTERS COMMON TO TYPE0 AND TYPE1  *****************
 ****************************************************************************/

/* pack/unpack for backwards compatibility */
#define PCIE_REV3_CLASSCODE_MASK ( \
           CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_PIB_MASK | \
           CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_SCC_MASK | \
           CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_CC_MASK)
#define PCIE_REV3_CLASSCODE_SHIFT (CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_PIB_SHIFT)

#define PCIE_REV3_TPH_CMPLT_SUPPORT_MASK ( \
           CSL_PCIE_EP_CORE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_TPH_CMPLT_SUPPORT_0_MASK | \
           CSL_PCIE_EP_CORE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_TPH_CMPLT_SUPPORT_1_MASK)
#define PCIE_REV3_TPH_CMPLT_SUPPORT_SHIFT (CSL_PCIE_EP_CORE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_TPH_CMPLT_SUPPORT_0_SHIFT)


/*****************************************************************************
 * These APIs are using the endpoint (Type 0) structure and #defines, but they
 * should be used for both EP and RC (Type 0 and Type 1) PCIe modes.
 * Both types have the same register layout, in the same location.
 ****************************************************************************/

 
/*****************************************************************************
 * Read and split up the Status and Command register
 ****************************************************************************/
pcieRet_e pciev3_read_statusCmd_reg
(
  volatile const uint32_t *hwReg_STATUS_COMMAND_REGISTER,
  pcieStatusCmdReg_t      *swReg
)
{
  uint32_t val = swReg->raw = *hwReg_STATUS_COMMAND_REGISTER;

  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_ISE,             swReg->ioSp);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_MSE,              swReg->memSp);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_BE,              swReg->busMs);
  
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_PERE,             swReg->resp);

  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_SE,               swReg->serrEn);

  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_IMD,              swReg->dis);

  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_IS,               swReg->stat);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_CL,               swReg->capList);

  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_MDPE,             swReg->parError);

  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_STA,              swReg->sigTgtAbort);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_RTA,              swReg->tgtAbort);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_RMA,              swReg->mstAbort);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_SSE,              swReg->sysError);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_DPE,              swReg->parity);

  return pcie_RET_OK;
} /* pciev3_read_statusCmd_reg */

/*****************************************************************************
 * Combine and write the Status and Command register
 ****************************************************************************/
pcieRet_e pciev3_write_statusCmd_reg
(
  volatile uint32_t  *hwReg_STATUS_COMMAND_REGISTER,
  pcieStatusCmdReg_t *swReg
)
{
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_ISE,             swReg->ioSp);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_MSE,              swReg->memSp);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_BE,              swReg->busMs);
  
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_PERE,             swReg->resp);

  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_SE,               swReg->serrEn);

  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_IMD,              swReg->dis);

  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_IS,               swReg->stat);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_CL,               swReg->capList);

  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_MDPE,             swReg->parError);

  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_STA,              swReg->sigTgtAbort);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_RTA,              swReg->tgtAbort);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_RMA,              swReg->mstAbort);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_SSE,              swReg->sysError);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_COMMAND_STATUS_DPE,              swReg->parity);

  *hwReg_STATUS_COMMAND_REGISTER = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_statusCmd_reg */

/*****************************************************************************
 * Read and split up the Base Address register
 ****************************************************************************/
pcieRet_e pciev3_read_baseAddr_reg
(
  volatile const uint32_t *hwReg_STATUS_COMMAND_REGISTER,
  pcieBaseAddrReg_t      *swReg
)
{
  uint32_t val = swReg->raw = *hwReg_STATUS_COMMAND_REGISTER;

  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_MSI0,                swReg->msio);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_R7,                  swReg->r7);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_S0,                  swReg->s0);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_P0,                  swReg->p0);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_R8,                  swReg->r8);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_BAMR0,               swReg->bamr0);

  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_BAMRW,               swReg->bamrw);

  return pcie_RET_OK;
} /* pciev3_read_baseAddr_reg */

/*****************************************************************************
 * Combine and write the Status and Command register
 ****************************************************************************/
pcieRet_e pciev3_write_baseAddr_reg
(
  volatile uint32_t  *hwReg_STATUS_COMMAND_REGISTER,
  pcieBaseAddrReg_t *swReg
)
{
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_MSI0,             swReg->msio);
  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_R7,               swReg->r7);
  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_S0,              swReg->s0);
  
  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_P0,             swReg->p0);

  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_R8,               swReg->r8);

  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_BAMR0,              swReg->bamr0);

  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_BAMRW,               swReg->bamrw);

  *hwReg_STATUS_COMMAND_REGISTER = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_baseAddr_reg */

/*****************************************************************************
 * Read and split up the Class Code and Revision ID register
 ****************************************************************************/
pcieRet_e pciev3_read_revId_reg
(
  volatile const uint32_t *hwReg_CLASSCODE_REVISIONID,
  pcieRevIdReg_t          *swReg
)
{
  uint32_t val = swReg->raw = *hwReg_CLASSCODE_REVISIONID;

  pcie_getbits(val, PCIE_REV3_CLASSCODE,                                 swReg->classCode);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_RID, swReg->revId);

  return pcie_RET_OK;
} /* pciev3_read_revId_reg */

/*****************************************************************************
 * Combine and write the Class Code and Revision ID register
 ****************************************************************************/
pcieRet_e pciev3_write_revId_reg
(
  volatile uint32_t *hwReg_CLASSCODE_REVISIONID,
  pcieRevIdReg_t    *swReg
)
{
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  pcie_setbits(new_val, PCIE_REV3_CLASSCODE,                                 swReg->classCode);
  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_RID, swReg->revId);

  *hwReg_CLASSCODE_REVISIONID = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_revId_reg */


/* MSI capabilities*/
/*****************************************************************************
 * Read and split up the Message Signaled Interrupt Capability register
 ****************************************************************************/
pcieRet_e pciev3_read_msiCap_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  pcieMsiCapReg_t            *swReg
)
{
  uint32_t *regVal = (uint32_t *)baseAddr;
  uint32_t val = swReg->raw = *regVal;

  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_CID1,           swReg->capId);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_CP1,  swReg->nextCap);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_ME,           swReg->msiEn);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MMC, swReg->multMsgCap);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MME,  swReg->multMsgEn);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_BAC64,  swReg->en64bit);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MC,     swReg->extDataCap);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_R0,      swReg->extDataEn);

  return pcie_RET_OK;
} /* pciev3_read_msiCap_reg */

/*****************************************************************************
 * Combine and write the Message Signaled Interrupt Capability register
 ****************************************************************************/
pcieRet_e pciev3_write_msiCap_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  pcieMsiCapReg_t      *swReg
)
{
  uint32_t new_val = swReg->raw;
  uint32_t *regVal = (uint32_t *)baseAddr;
  pcie_range_check_begin;

  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_CID1,           swReg->capId);
  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_CP1,  swReg->nextCap);
  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_ME,           swReg->msiEn);
  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MMC, swReg->multMsgCap);
  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MME,  swReg->multMsgEn);
  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_BAC64,  swReg->en64bit);
  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MC,     swReg->extDataCap);
  pcie_setbits(new_val, CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_R0,      swReg->extDataEn);

  *regVal = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_msiCap_reg */

/*****************************************************************************
 * Read and split up the Address of MSI write TLP req lower 32 bits register
 ****************************************************************************/
pcieRet_e pciev3_read_msiLo32_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  pcieMsiLo32Reg_t           *swReg
)
{
  CSL_pcie_rp_coreRegs *baseAddrRc = (CSL_pcie_rp_coreRegs *)baseAddr;
  uint32_t val = swReg->raw = baseAddrRc->RC_I_RC_PCIE_BASE.I_MSI_MSG_LOW_ADDR;

  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_MSI_MSG_LOW_ADDR_MAL, swReg->addr);

  return pcie_RET_OK;
} /* pciev3_read_epcfgdbicsMsiAddrL32_reg */

/*****************************************************************************
 * Combine and write the Address of MSI write TLP req lower 32 bits register
 ****************************************************************************/
pcieRet_e pciev3_write_msiLo32_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  pcieMsiLo32Reg_t     *swReg
)
{
  CSL_pcie_rp_coreRegs *baseAddrRc = (CSL_pcie_rp_coreRegs *)baseAddr;
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_MSI_MSG_LOW_ADDR_MAL, swReg->addr);

  baseAddrRc->RC_I_RC_PCIE_BASE.I_MSI_MSG_LOW_ADDR = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_epcfgdbicsMsiAddrL32_reg */

/*****************************************************************************
 * Read and split up the Address of MSI write TLP req upper 32 bits register
 ****************************************************************************/
pcieRet_e pciev3_read_msiUp32_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  pcieMsiUp32Reg_t           *swReg
)
{
  CSL_pcie_rp_coreRegs *baseAddrRc = (CSL_pcie_rp_coreRegs *)baseAddr;
  uint32_t val = swReg->raw = baseAddrRc->RC_I_RC_PCIE_BASE.I_MSI_MSG_HI_ADDR;

  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_MSI_MSG_HI_ADDR_MAH, swReg->addr);

  return pcie_RET_OK;
} /* pciev3_read_msiUp32_reg */

/*****************************************************************************
 * Combine and write the Address of MSI write TLP req upper 32 bits register
 ****************************************************************************/
pcieRet_e pciev3_write_msiUp32_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  pcieMsiUp32Reg_t     *swReg
)
{
  CSL_pcie_rp_coreRegs *baseAddrRc = (CSL_pcie_rp_coreRegs *)baseAddr;
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_MSI_MSG_HI_ADDR_MAH, swReg->addr);

  baseAddrRc->RC_I_RC_PCIE_BASE.I_MSI_MSG_HI_ADDR = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_epcfgdbicsMsiAddrU32_reg */

/*****************************************************************************
 * Read and split up the Data of MSI write TLP req register
 ****************************************************************************/
pcieRet_e pciev3_read_msiData_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  pcieMsiDataReg_t           *swReg
)
{
  CSL_pcie_rp_coreRegs *baseAddrRc = (CSL_pcie_rp_coreRegs *)baseAddr;
  uint32_t val = swReg->raw = baseAddrRc->RC_I_RC_PCIE_BASE.I_MSI_MSG_DATA;

  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_MSI_MSG_DATA_MD, swReg->data);

  return pcie_RET_OK;
} /* pciev3_read_epcfgdbicsMsiData_reg */

/*****************************************************************************
 * Combine and write the Data of MSI write TLP req register
 ****************************************************************************/
pcieRet_e pciev3_write_msiData_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  pcieMsiDataReg_t     *swReg
)
{
  CSL_pcie_rp_coreRegs *baseAddrRc = (CSL_pcie_rp_coreRegs *)baseAddr;
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_MSI_MSG_DATA_MD, swReg->data);

  baseAddrRc->RC_I_RC_PCIE_BASE.I_MSI_MSG_DATA = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_epcfgdbicsMsiData_reg */

/*****************************************************************************
 * Read and split up the Device Status and Control register
 ****************************************************************************/
pcieRet_e pciev3_read_devStatCtrl_reg
(
  volatile const uint32_t *hwReg_DEV_CAS,
  pcieDevStatCtrlReg_t    *swReg
)
{
  uint32_t val = swReg->raw = *hwReg_DEV_CAS;

  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ECER,       swReg->corErRp);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ECER,  swReg->nFatalErRp);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ECER,      swReg->fatalErRp);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ECER,     swReg->reqRp);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ERO,             swReg->relaxed);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_MP,      swReg->maxPayld);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ETE,               swReg->xtagEn);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ETE,          swReg->phantomEn);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ETE,          swReg->auxPwrEn);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ENS,              swReg->noSnoop);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_MRR,        swReg->maxSz);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_CED,        swReg->corrEr);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_NFED,   swReg->nFatalEr);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_FED,       swReg->fatalEr);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_URD, swReg->rqDet);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_APD,       swReg->auxPwr);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_TP,            swReg->tpend);

  return pcie_RET_OK;
} /* pciev3_read_devStatCtrl_reg */


/*****************************************************************************
 * Combine and write the Device Status and Control register
 ****************************************************************************/
pcieRet_e pciev3_write_devStatCtrl_reg
(
  volatile uint32_t    *hwReg_DEV_CAS,
  pcieDevStatCtrlReg_t *swReg
)
{
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ECER,       swReg->corErRp);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ECER,  swReg->nFatalErRp);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ECER,      swReg->fatalErRp);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ECER,     swReg->reqRp);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ERO,             swReg->relaxed);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_MP,      swReg->maxPayld);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ETE,               swReg->xtagEn);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ETE,          swReg->phantomEn);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ETE,          swReg->auxPwrEn);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_ENS,              swReg->noSnoop);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_MRR,        swReg->maxSz);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_CED,        swReg->corrEr);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_NFED,   swReg->nFatalEr);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_FED,       swReg->fatalEr);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_URD, swReg->rqDet);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_APD,       swReg->auxPwr);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_PCIE_DEV_CTRL_STATUS_TP,            swReg->tpend);

  *hwReg_DEV_CAS = swReg->raw = new_val;
  return pcie_range_check_return;
} /* pciev3_write_devStatCtrl_reg */

/*****************************************************************************
 * Read and split up the Link Capabilities register
 ****************************************************************************/
pcieRet_e pciev3_read_linkCap_reg
(
  volatile const uint32_t *hwReg_LNK_CAP,
  pcieLinkCapReg_t        *swReg
)
{
  uint32_t val = swReg->raw = *hwReg_LNK_CAP;

  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_LINK_CAP_MLS,               swReg->maxLinkSpeed);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_LINK_CAP_MLW,               swReg->maxLinkWidth);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_LINK_CAP_ASPM,             swReg->asLinkPm);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_LINK_CAP_L0EL,             swReg->losExitLat);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_LINK_CAP_L1EL,              swReg->l1ExitLat);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_LINK_CAP_CPM,              swReg->clkPwrMgmt);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_LINK_CAP_SERC,                swReg->downErrRepCap);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_LINK_CAP_DARC,           swReg->dllRepCap);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_LINK_CAP_LBNC,              swReg->bwNotifyCap);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_LINK_CAP_ASPMOC,          swReg->aspmOptComp);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_LINK_CAP_ASPMOC,            swReg->portNum);

  return pcie_RET_OK;
} /* pciev3_read_linkCap_reg */


/*****************************************************************************
 * Combine and write the Link Capabilities register
 ****************************************************************************/
pcieRet_e pciev3_write_linkCap_reg
(
  volatile uint32_t *hwReg_LNK_CAP,
  pcieLinkCapReg_t  *swReg
)
{
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  pcie_setbits(new_val, CSL_MAIN_CTRL_MMR_CFG0_PCIE0_CTRL_GENERATION_SEL,            swReg->maxLinkSpeed);

  *hwReg_LNK_CAP = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_linkCap_reg */


/*****************************************************************************
 * Read and split up the Link Status and Control register
 ****************************************************************************/
pcieRet_e pciev3_read_linkStatCtrl_reg
(
  volatile const uint32_t *hwReg_LNK_CAS,
  pcieLinkStatCtrlReg_t   *swReg
)
{
  uint32_t val = swReg->raw = *hwReg_LNK_CAS;

  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_NLS, swReg->linkSpeed);
  pcie_getbits(val, CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_NLW, swReg->negotiatedLinkWd);

  return pcie_RET_OK;
} /* pciev3_read_linkStatCtrl_reg */


/*****************************************************************************
 * Read and split up the Advanced Capabilities and Control register
 ****************************************************************************/
pcieRet_e pciev3_read_accr_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  pcieAccrReg_t              *swReg
)
{
  CSL_pcie_rp_coreRegs *baseAddrRp = (CSL_pcie_rp_coreRegs *)baseAddr;
  uint32_t val = swReg->raw = baseAddrRp->RC_I_RC_PCIE_BASE.I_ADV_ERR_CAP_CTL;

  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_MHRE,  swReg->multHdrEn);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_MHRC, swReg->multHdrCap);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_EEC,       swReg->chkEn);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_ECC,      swReg->chkCap);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_EEG,         swReg->genEn);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_EGC,        swReg->genCap);
  pcie_getbits(val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_FEP,   swReg->erPtr);

  return pcie_RET_OK;
} /* pciev3_read_accr_reg */


/*****************************************************************************
 * Combine and write the Advanced Capabilities and Control register
 ****************************************************************************/
pcieRet_e pciev3_write_accr_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  pcieAccrReg_t        *swReg
)
{
  CSL_pcie_rp_coreRegs *baseAddrRp = (CSL_pcie_rp_coreRegs *)baseAddr;
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_MHRE,  swReg->multHdrEn);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_MHRC, swReg->multHdrCap);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_EEC,       swReg->chkEn);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_ECC,      swReg->chkCap);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_EEG,         swReg->genEn);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_EGC,        swReg->genCap);
  pcie_setbits(new_val, CSL_PCIE_RP_CORE_RC_I_RC_PCIE_BASE_I_ADV_ERR_CAP_CTL_FEP,   swReg->erPtr);

  baseAddrRp->RC_I_RC_PCIE_BASE.I_ADV_ERR_CAP_CTL = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_accr_reg */

/* Nothing past this point */

