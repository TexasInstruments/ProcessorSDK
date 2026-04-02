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
 *  File Name: pciev3_ep.c
 *
 *  Processing/configuration functions for the PCIe driver.
 *
 */

#include <ti/drv/pcie/pcie.h>
#include <ti/drv/pcie/src/pcieloc.h>
#include <ti/drv/pcie/src/v3/pcieloc.h>

/*****************************************************************************
 **********  External APIs **********************
 ****************************************************************************/


#define PCIE_EP_EXPROM_BAR_BASE_FULL_MASK (CSL_EPCFGDBICS_EXPANSION_ROM_BAR_EXROM_ADDRESS_RO_MASK | CSL_EPCFGDBICS_EXPANSION_ROM_BAR_EXROM_ADDRESS_MASK)
#define PCIE_EP_EXPROM_BAR_BASE_FULL_SHIFT (CSL_EPCFGDBICS_EXPANSION_ROM_BAR_EXROM_ADDRESS_RO_SHIFT)


/*****************************************************************************
 * Read and split up the BAR register
 ****************************************************************************/
pcieRet_e pciev3_read_type0Bar_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  pcieBarReg_t *swReg,
  int32_t barNum
)
{

  uint32_t val = swReg->raw = baseAddr->LM_I_REGF_LM_PCIE_BASE.I_PF_BAR_CONFIG_REG[0].I_PF_BAR_CONFIG_REG[barNum/4];

  /* get the BARxA and BARxC accroding to the barNum */
  switch (barNum%4)
  {
  case 0:
    pcie_getbits(val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR0C, swReg->barxc);
    pcie_getbits(val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR0A, swReg->barxa);
    break;
  case 1:
      pcie_getbits(val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR1C, swReg->barxc);
      pcie_getbits(val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR1A, swReg->barxa);
      break;
  case 2:
      pcie_getbits(val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR2C, swReg->barxc);
      pcie_getbits(val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR2A, swReg->barxa);
      break;
  case 3:
      pcie_getbits(val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR3C, swReg->barxc);
      pcie_getbits(val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR3A, swReg->barxa);
      break;
  default:
        swReg->prefetch = 0;
      swReg->base = 0;
        break;
  }

  return pcie_RET_OK;
} /* pciev3_read_type0Bar_reg */

/*****************************************************************************
 * Combine and write the BAR register
 ****************************************************************************/
pcieRet_e pciev3_write_type0Bar_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  pcieBarReg_t *swReg,
  int32_t barNum
)
{
  uint32_t new_val = swReg->raw;

  pcie_range_check_begin;

  /* set the BARxA and BARxC accroding to the barNum */
  switch (barNum%4)
  {
  case 0:
    pcie_setbits(new_val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR0C, swReg->barxc);
    pcie_setbits(new_val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR0A, swReg->barxa);
    break;
  case 1:
      pcie_setbits(new_val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR1C, swReg->barxc);
      pcie_setbits(new_val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR1A, swReg->barxa);
      break;
  case 2:
      pcie_setbits(new_val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR2C, swReg->barxc);
      pcie_setbits(new_val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR2A, swReg->barxa);
      break;
  case 3:
      pcie_setbits(new_val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR3C, swReg->barxc);
      pcie_setbits(new_val, CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR3A, swReg->barxa);
      break;
  default:
        break;
  }

  baseAddr->LM_I_REGF_LM_PCIE_BASE.I_PF_BAR_CONFIG_REG[0].I_PF_BAR_CONFIG_REG[barNum/4] = swReg->raw = new_val;
  return pcie_range_check_return;
} /* pciev3_write_type0Bar_reg */


/*****************************************************************************
 * Read and split up the BAR 32bits register
 ****************************************************************************/
pcieRet_e pciev3_read_type0Bar32bit_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  pcieBar32bitReg_t *swReg,
  int32_t barNum,
  uint8_t pfNum
)
{
  swReg->reg32 = swReg->raw = baseAddr->EP_PF_I_PCIE[pfNum].EP_PF_I_PCIE_BASE.I_BASE_ADDR[barNum];
  return pcie_RET_OK;
} /* pciev3_read_type0Bar32bit_reg */

/*****************************************************************************
 * Combine and write the BAR 32bits register
 ****************************************************************************/
pcieRet_e pciev3_write_type0Bar32bit_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  pcieBar32bitReg_t *swReg,
  int32_t barNum,
  uint8_t pfNum
)
{
  baseAddr->EP_PF_I_PCIE[pfNum].EP_PF_I_PCIE_BASE.I_BASE_ADDR[barNum] = swReg->raw = swReg->reg32;
  return pcie_RET_OK;
} /* pciev3_write_type0Bar32bit_reg */

/* Nothing past this point */

