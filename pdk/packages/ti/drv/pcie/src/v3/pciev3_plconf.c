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
 *  File Name: pciev3_plconf.c
 *
 *  Processing/configuration functions for the PCIe PLCONF Registers
 *
 */

#include <ti/drv/pcie/pcie.h>
#include <ti/drv/pcie/src/pcieloc.h>
#include <ti/drv/pcie/src/v3/pcieloc.h>

/*****************************************************************************
 **********  PCIe LOCAL/REMOTE PORT LOGIC REGISTERS **********************
 ****************************************************************************/

  /* Define bitfield positions */
#define PCIEV2_FLTMASK1_CFG_DROP_MASK         (0x00008000U)
#define PCIEV2_FLTMASK1_CFG_DROP_SHIFT        (0x0000000FU)
#define PCIEV2_FLTMASK1_IO_DROP_MASK          (0x00004000U)
#define PCIEV2_FLTMASK1_IO_DROP_SHIFT         (0x0000000EU)
#define PCIEV2_FLTMASK1_MSG_DROP_MASK         (0x00002000U)
#define PCIEV2_FLTMASK1_MSG_DROP_SHIFT        (0x0000000DU)
#define PCIEV2_FLTMASK1_CPL_ECRC_DROP_MASK    (0x00001000U)
#define PCIEV2_FLTMASK1_CPL_ECRC_DROP_SHIFT   (0x0000000CU)

#define PCIEV2_FLTMASK1_ECRC_DROP_MASK        (0x00000800U)
#define PCIEV2_FLTMASK1_ECRC_DROP_SHIFT       (0x0000000BU)
#define PCIEV2_FLTMASK1_CPL_LEN_TEST_MASK     (0x00000400U)
#define PCIEV2_FLTMASK1_CPL_LEN_TEST_SHIFT    (0x0000000AU)
#define PCIEV2_FLTMASK1_CPL_ATTR_TEST_MASK    (0x00000200U)
#define PCIEV2_FLTMASK1_CPL_ATTR_TEST_SHIFT   (0x00000009U)
#define PCIEV2_FLTMASK1_CPL_TC_TEST_MASK      (0x00000100U)
#define PCIEV2_FLTMASK1_CPL_TC_TEST_SHIFT     (0x00000008U)

#define PCIEV2_FLTMASK1_CPL_FUNC_TEST_MASK    (0x00000080U)
#define PCIEV2_FLTMASK1_CPL_FUNC_TEST_SHIFT   (0x00000007U)
#define PCIEV2_FLTMASK1_CPL_REQID_TEST_MASK   (0x00000040U)
#define PCIEV2_FLTMASK1_CPL_REQID_TEST_SHIFT  (0x00000006U)
#define PCIEV2_FLTMASK1_CPL_TAGERR_TEST_MASK  (0x00000020U)
#define PCIEV2_FLTMASK1_CPL_TAGERR_TEST_SHIFT (0x00000005U)
#define PCIEV2_FLTMASK1_LOCKED_RD_AS_UR_MASK  (0x00000010U)
#define PCIEV2_FLTMASK1_LOCKED_RD_AS_UR_SHIFT (0x00000004U)

#define PCIEV2_FLTMASK1_CFG1_RE_AS_US_MASK    (0x00000008U)
#define PCIEV2_FLTMASK1_CFG1_RE_AS_US_SHIFT   (0x00000003U)
#define PCIEV2_FLTMASK1_UR_OUT_OF_BAR_MASK    (0x00000004U)
#define PCIEV2_FLTMASK1_UR_OUT_OF_BAR_SHIFT   (0x00000002U)
#define PCIEV2_FLTMASK1_UR_POISON_MASK        (0x00000002U)
#define PCIEV2_FLTMASK1_UR_POISON_SHIFT       (0x00000001U)
#define PCIEV2_FLTMASK1_UR_FUN_MISMATCH_MASK  (0x00000001U)
#define PCIEV2_FLTMASK1_UR_FUN_MISMATCH_SHIFT (0x00000000U)


/* Define bitfield positions */
#define PCIEV2_FLTMASK2_DROP_PRS_MASK        (0x00000080U)
#define PCIEV2_FLTMASK2_DROP_PRS_SHIFT       (0x00000007U)
#define PCIEV2_FLTMASK2_UNMASK_TD_MASK       (0x00000040U)
#define PCIEV2_FLTMASK2_UNMASK_TD_SHIFT      (0x00000006U)
#define PCIEV2_FLTMASK2_UNMASK_UR_POIS_MASK  (0x00000020U)
#define PCIEV2_FLTMASK2_UNMASK_UR_POIS_SHIFT (0x00000005U)
#define PCIEV2_FLTMASK2_DROP_LN_MASK         (0x00000010U)
#define PCIEV2_FLTMASK2_DROP_LN_SHIFT        (0x00000004U)
#define PCIEV2_FLTMASK2_FLUSH_REQ_MASK       (0x00000008U)
#define PCIEV2_FLTMASK2_FLUSH_REQ_SHIFT      (0x00000003U)
#define PCIEV2_FLTMASK2_DLLP_ABORT_MASK      (0x00000004U)
#define PCIEV2_FLTMASK2_DLLP_ABORT_SHIFT     (0x00000002U)
#define PCIEV2_FLTMASK2_VMSG1_DROP_MASK      (0x00000002U)
#define PCIEV2_FLTMASK2_VMSG1_DROP_SHIFT     (0x00000001U)
#define PCIEV2_FLTMASK2_VMSG0_DROP_MASK      (0x00000001U)
#define PCIEV2_FLTMASK2_VMSG0_DROP_SHIFT     (0x00000000U)

/* DEBUG0 */
#define PCIEV2_DEBUG0_TS_LINK_CTRL_MASK      (0xF0000000u)
#define PCIEV2_DEBUG0_TS_LINK_CTRL_SHIFT     (0x0000001Cu)

#define PCIEV2_DEBUG0_TS_LANE_K237_MASK      (0x08000000u)
#define PCIEV2_DEBUG0_TS_LANE_K237_SHIFT     (0x0000001Bu)

#define PCIEV2_DEBUG0_TS_LINK_K237_MASK      (0x04000000u)
#define PCIEV2_DEBUG0_TS_LINK_K237_SHIFT     (0x0000001Au)

#define PCIEV2_DEBUG0_RCVD_IDLE0_MASK        (0x02000000u)
#define PCIEV2_DEBUG0_RCVD_IDLE0_SHIFT       (0x00000019u)

#define PCIEV2_DEBUG0_RCVD_IDLE1_MASK        (0x01000000u)
#define PCIEV2_DEBUG0_RCVD_IDLE1_SHIFT       (0x00000018u)

#define PCIEV2_DEBUG0_PIPE_TXDATA_MASK       (0x00FFFF00u)
#define PCIEV2_DEBUG0_PIPE_TXDATA_SHIFT      (0x00000008u)

#define PCIEV2_DEBUG0_PIPE_TXDATAK_MASK      (0x000000C0u)
#define PCIEV2_DEBUG0_PIPE_TXDATAK_SHIFT     (0x00000006u)

#define PCIEV2_DEBUG0_TXB_SKIP_TX_MASK       (0x00000020u)
#define PCIEV2_DEBUG0_TXB_SKIP_TX_SHIFT      (0x00000005u)

#define PCIEV2_DEBUG0_LTSSM_STATE_MASK       (0x0000001Fu)
#define PCIEV2_DEBUG0_LTSSM_STATE_SHIFT      (0x00000000u)

/* DEBUG1 */

#define PCIEV2_DEBUG1_SCRAMBLER_DISABLE_MASK (0x80000000u)
#define PCIEV2_DEBUG1_SCRAMBLER_DISABLE_SHIFT (0x0000001Fu)

#define PCIEV2_DEBUG1_LINK_DISABLE_MASK      (0x40000000u)
#define PCIEV2_DEBUG1_LINK_DISABLE_SHIFT     (0x0000001Eu)

#define PCIEV2_DEBUG1_LINK_IN_TRAINING_MASK  (0x20000000u)
#define PCIEV2_DEBUG1_LINK_IN_TRAINING_SHIFT (0x0000001Du)

#define PCIEV2_DEBUG1_RCVR_REVRS_POL_EN_MASK (0x10000000u)
#define PCIEV2_DEBUG1_RCVR_REVRS_POL_EN_SHIFT (0x0000001Cu)

#define PCIEV2_DEBUG1_TRAINING_RST_N_MASK    (0x08000000u)
#define PCIEV2_DEBUG1_TRAINING_RST_N_SHIFT   (0x0000001Bu)

#define PCIEV2_DEBUG1_PIPE_TXDETECTRX_LB_MASK (0x00400000u)
#define PCIEV2_DEBUG1_PIPE_TXDETECTRX_LB_SHIFT (0x00000016u)

#define PCIEV2_DEBUG1_PIPE_TXELECIDLE_MASK   (0x00200000u)
#define PCIEV2_DEBUG1_PIPE_TXELECIDLE_SHIFT  (0x00000015u)

#define PCIEV2_DEBUG1_PIPE_TXCOMPLIANCE_MASK (0x00100000u)
#define PCIEV2_DEBUG1_PIPE_TXCOMPLIANCE_SHIFT (0x00000014u)

#define PCIEV2_DEBUG1_APP_INIT_RST_MASK      (0x00080000u)
#define PCIEV2_DEBUG1_APP_INIT_RST_SHIFT     (0x00000013u)

#define PCIEV2_DEBUG1_RMLH_TS_LINK_NUM_MASK  (0x0000FF00u)
#define PCIEV2_DEBUG1_RMLH_TS_LINK_NUM_SHIFT (0x00000008u)

#define PCIEV2_DEBUG1_XMLH_LINK_UP_MASK      (0x00000010u)
#define PCIEV2_DEBUG1_XMLH_LINK_UP_SHIFT     (0x00000004u)

#define PCIEV2_DEBUG1_RMLH_INSKIP_RCV_MASK   (0x00000008u)
#define PCIEV2_DEBUG1_RMLH_INSKIP_RCV_SHIFT  (0x00000003u)

#define PCIEV2_DEBUG1_RMLH_TS1_RCVD_MASK     (0x00000004u)
#define PCIEV2_DEBUG1_RMLH_TS1_RCVD_SHIFT    (0x00000002u)

#define PCIEV2_DEBUG1_RMLH_TS2_RCVD_MASK     (0x00000002u)
#define PCIEV2_DEBUG1_RMLH_TS2_RCVD_SHIFT    (0x00000001u)

#define PCIEV2_DEBUG1_RMLH_RCVD_LANE_REV_MASK (0x00000001u)
#define PCIEV2_DEBUG1_RMLH_RCVD_LANE_REV_SHIFT (0x00000000u)

/*****************************************************************************
 * These APIs work on both EP and RC.
 ****************************************************************************/

/*****************************************************************************
 * Read and split up the PL CONF Port Link Control (Sticky) register
 ****************************************************************************/
pcieRet_e pciev3_read_lnkCtrl_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  pcieLnkCtrlReg_t *swReg
)
{
  uint32_t val = *((uint32_t *)baseAddr);

  pcie_getbits(val, CSL_MAIN_CTRL_MMR_CFG0_PCIE0_CTRL_LANE_COUNT, swReg->lnkMode);

  return pcie_RET_OK;
} /* pciev3_read_lnkCtrl_reg */


/*****************************************************************************
 * Combine and write the PL CONF Port Link Control (Sticky) register
 ****************************************************************************/
pcieRet_e pciev3_write_lnkCtrl_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  pcieLnkCtrlReg_t *swReg
)
{
  uint32_t new_val = *((uint32_t *)baseAddr);

  pcie_range_check_begin;
  pcie_setbits(new_val, CSL_MAIN_CTRL_MMR_CFG0_PCIE0_CTRL_LANE_COUNT, swReg->lnkMode);

  *((uint32_t *)baseAddr) = new_val;
  return pcie_range_check_return;
} /* pciev3_write_lnkCtrl_reg */

/*****************************************************************************
 * Combine and write the PL CONF Link Width and Speed Change Control (Sticky)
 * register
 ****************************************************************************/
pcieRet_e pciev3_write_gen2_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  pcieGen2Reg_t *swReg
)
{
  uint32_t *mmrCfg = (uint32_t *)baseAddr;
  uint32_t new_val = swReg->raw;
  pcie_range_check_begin;

  pcie_setbits(new_val, CSL_MAIN_CTRL_MMR_CFG0_PCIE0_CTRL_LANE_COUNT,        swReg->lnEn);

  *mmrCfg = swReg->raw = new_val;

  return pcie_range_check_return;
} /* pciev3_write_plconfWidthSpeedCtl_reg */


/*****************************************************************************
 * Read and split up the PL CONF iATU Region Control 1 register
 ****************************************************************************/
pcieRet_e pciev3_read_plconfIatuRegCtrl1_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  const pciePlconfIatuIndexReg_t *simIatuWindow,
  pciePlconfIatuRegCtrl1Reg_t *swReg
)
{
  pcieRet_e retVal = pcie_RET_OK;
  uint32_t val;
  /* Don't need to check NULL this is internal API */
  uint8_t regionIndex = simIatuWindow->regionIndex;

  if (regionIndex < 16)
  {
    if (simIatuWindow->regionDirection == 0U)
    {
       /* 0U == OUTBOUND */
       val = swReg->raw = baseAddr->ATU_WRAPPER_OB[regionIndex].DESC0;

       /* TLP type bit[3:0] - 0xA - Config, 0x2 - MEmory WR/RD, 0xC - message, 0xD - Vendor message, 0x6 */
       pcie_getbits(val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC0_TT,                 swReg->type);
       pcie_getbits(val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC0_PTC,                   swReg->tc);
       pcie_getbits(val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC0_ATTR,                 swReg->attr);
       pcie_getbits(val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC0_FNUM,      swReg->functionNumber);
    }
  }
  else
  {
    retVal = pcie_RET_RANGECHK;
  }

  /* Set unused fields to 0 (only used by rev 0/1 hw) */
  swReg->at = 0u;

  return retVal;
} /* pciev3_read_plconfIatuRegCtrl1_reg */


/*****************************************************************************
 * Combine and write the PL CONF iATU Region Control 1 register
 ****************************************************************************/
pcieRet_e pciev3_write_plconfIatuRegCtrl1_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  const pciePlconfIatuIndexReg_t *simIatuWindow,
  pciePlconfIatuRegCtrl1Reg_t *swReg
)
{
  uint32_t new_val = swReg->raw;
  pcieRet_e retVal = pcie_RET_OK;
  /* Don't need to check NULL this is internal API */
  uint8_t regionIndex = simIatuWindow->regionIndex;

  pcie_range_check_begin;

  if (regionIndex < 16)
  {
    if (simIatuWindow->regionDirection == 0U)
    {
       /* 0U == OUTBOUND */
       /* TLP type bit[3:0] - 0xA - Config, 0x2 - MEmory WR/RD, 0xC - message, 0xD - Vendor message, 0x6 */
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC0_TT,                 swReg->type);
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC0_PTC,                   swReg->tc);
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC0_PTC,                 swReg->attr);
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC0_FNUM,      swReg->functionNumber);
       baseAddr->ATU_WRAPPER_OB[regionIndex].DESC0 = new_val;
    }
  }
  else
  {
    retVal = pcie_RET_RANGECHK;
  }

  return retVal;
} /* pciev3_write_plconfIatuRegCtrl1_reg */


/*****************************************************************************
 * Read and split up the PL CONF iATU Region Lower Base Address register
 ****************************************************************************/
pcieRet_e pciev3_read_plconfIatuRegLowerBase_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  const pciePlconfIatuIndexReg_t *simIatuWindow,
  pciePlconfIatuRegLowerBaseReg_t *swReg
)
{
  pcieRet_e retVal = pcie_RET_OK;
  uint32_t val;
  /* Don't need to check NULL this is internal API */
  uint8_t regionIndex = simIatuWindow->regionIndex;

  if (regionIndex < 16)
  {
    if (simIatuWindow->regionDirection == 0U)
    {
       /* 0U == OUTBOUND */
       val = swReg->raw = baseAddr->ATU_WRAPPER_OB[regionIndex].ADDR0;

       pcie_getbits(val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_DATA, swReg->iatuRegLowerBase);
       pcie_getbits(val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_NUM_BITS, swReg->zero);
    }
    else
    {
       /* INBOUND */
       val = swReg->raw = baseAddr->ATU_FUNC_WRAPPER_IB_EP[0][regionIndex].ADDR0;

       pcie_getbits(val, CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_0_ADDR0_DATA, swReg->iatuRegLowerBase);
    }
  }
  else
  {
    retVal = pcie_RET_RANGECHK;
  }

  return retVal;
} /* pciev3_read_plconfIatuRegLowerBase_reg */


/*****************************************************************************
 * Combine and write the PL CONF iATU Region Lower Base Address register
 ****************************************************************************/
pcieRet_e pciev3_write_plconfIatuRegLowerBase_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  const pciePlconfIatuIndexReg_t *simIatuWindow,
  pciePlconfIatuRegLowerBaseReg_t *swReg
)
{
  uint32_t new_val = swReg->raw;
  pcieRet_e retVal = pcie_RET_OK;
  /* Don't need to check NULL this is internal API */
  uint8_t regionIndex = simIatuWindow->regionIndex;

  pcie_range_check_begin;

  if (regionIndex < 16)
  {
    if (simIatuWindow->regionDirection == 0U)
    {
       /* 0U == OUTBOUND */
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_DATA, swReg->iatuRegLowerBase);
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_NUM_BITS, swReg->zero);

       swReg->raw = baseAddr->ATU_WRAPPER_OB[regionIndex].ADDR0 = new_val;
    }
    else
    {
       /* INBOUND */
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_0_ADDR0_DATA, swReg->iatuRegLowerBase);

       swReg->raw = baseAddr->ATU_FUNC_WRAPPER_IB_EP[0][regionIndex].ADDR0 = new_val<<8;

       retVal = pcie_range_check_return;
    }
  }
  else
  {
    retVal = pcie_RET_RANGECHK;
  }

  return retVal;
} /* pciev3_write_plconfIatuRegLowerBase_reg */


/*****************************************************************************
 * Combine and write the PL CONF iATU Region Lower Base Address register
 ****************************************************************************/
pcieRet_e pciev3_write_plconfIatuRegLowerBaseRc_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  const pciePlconfIatuIndexReg_t *simIatuWindow,
  pciePlconfIatuRegLowerBaseReg_t *swReg
)
{
  uint32_t new_val = swReg->raw;
  pcieRet_e retVal = pcie_RET_OK;
  /* Don't need to check NULL this is internal API */
  uint8_t regionIndex = simIatuWindow->regionIndex;

  pcie_range_check_begin;

  if (regionIndex < 16)
  {
    if (simIatuWindow->regionDirection == 0U)
    {
       /* 0U == OUTBOUND */
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_DATA, swReg->iatuRegLowerBase);
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_NUM_BITS, swReg->zero);

       swReg->raw = baseAddr->ATU_WRAPPER_OB[regionIndex].ADDR0 = new_val;
    }
    else
    {
       /* INBOUND */
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_DATA, swReg->iatuRegLowerBase);
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_NUM_BITS, swReg->zero);
       swReg->raw = baseAddr->ATU_WRAPPER_IB[regionIndex].ADDR0 = new_val;

       retVal = pcie_range_check_return;
    }
  }
  else
  {
    retVal = pcie_RET_RANGECHK;
  }

  return retVal;
} /* pciev3_write_plconfIatuRegLowerBase_reg */


/*****************************************************************************
 * Read and split up the PL CONF iATU Region Limit Address register
 ****************************************************************************/
pcieRet_e pciev3_read_plconfIatuRegUpperBase_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  const pciePlconfIatuIndexReg_t *simIatuWindow,
  pciePlconfIatuRegUpperBaseReg_t *swReg
)
{

  pcieRet_e retVal = pcie_RET_OK;
  uint32_t val;
  /* Don't need to check NULL this is internal API */
  uint8_t regionIndex = simIatuWindow->regionIndex;

  if (regionIndex < 16)
  {
    if (simIatuWindow->regionDirection == 0U)
    {
       /* 0U == OUTBOUND */
       val = swReg->raw = baseAddr->ATU_WRAPPER_OB[regionIndex].ADDR1;

       pcie_getbits(val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR1_DATA, swReg->iatuRegUpperBase);
    }
    else
    {
       /* INBOUND */
       val = swReg->raw = baseAddr->ATU_FUNC_WRAPPER_IB_EP[0][regionIndex].ADDR1;

       pcie_getbits(val, CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_0_ADDR1_DATA, swReg->iatuRegUpperBase);
    }
  }
  else
  {
    retVal = pcie_RET_RANGECHK;
  }

  return retVal;
} /* pciev3_read_plconfIatuRegUpperBase_reg */


/*****************************************************************************
 * Combine and write the PL CONF iATU Region Limit Address register
 ****************************************************************************/
pcieRet_e pciev3_write_plconfIatuRegUpperBase_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  const pciePlconfIatuIndexReg_t *simIatuWindow,
  pciePlconfIatuRegUpperBaseReg_t *swReg
)
{
  uint32_t new_val = swReg->raw;
  pcieRet_e retVal = pcie_RET_OK;
  /* Don't need to check NULL this is internal API */
  uint8_t regionIndex = simIatuWindow->regionIndex;

  pcie_range_check_begin;

  if (regionIndex < 16)
  {
    if (simIatuWindow->regionDirection == 0U)
    {
       /* 0U == OUTBOUND */
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR1_DATA, swReg->iatuRegUpperBase);

       swReg->raw = baseAddr->ATU_WRAPPER_OB[regionIndex].ADDR1 = new_val;
    }
    else
    {
       /* INBOUND */
       pcie_setbits(new_val, CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_0_ADDR1_DATA, swReg->iatuRegUpperBase);

       swReg->raw = baseAddr->ATU_FUNC_WRAPPER_IB_EP[0][regionIndex].ADDR1 = new_val;

       retVal = pcie_range_check_return;
    }
  }
  else
  {
    retVal = pcie_RET_RANGECHK;
  }

  return retVal;
} /* pciev3_write_plconfIatuRegUpperBase_reg */


/*****************************************************************************
 * Read and split up the PL CONF iATU Region Lower Target Address register
 ****************************************************************************/
pcieRet_e pciev3_read_plconfIatuRegLowerTarget_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  const pciePlconfIatuIndexReg_t *simIatuWindow,
  pciePlconfIatuRegLowerTargetReg_t *swReg
)
{
  pcieRet_e retVal = pcie_RET_OK;
  uint32_t val;
  /* Don't need to check NULL this is internal API */
  uint8_t regionIndex = simIatuWindow->regionIndex;

  if (regionIndex < 16)
  {
    if (simIatuWindow->regionDirection == 0U)
    {
       /* 0U == OUTBOUND */
       val = swReg->raw = baseAddr->ATU_WRAPPER_OB[regionIndex].AXI_ADDR0;

       pcie_getbits(val, CSL_PCIE_RP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_DATA, swReg->iatuRegLowerTarget);
       pcie_getbits(val, CSL_PCIE_RP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_REGION_SIZE, swReg->zero);
    }
    else
    {
       /* INBOUND */
    }
  }
  else
  {
    retVal = pcie_RET_RANGECHK;
  }

  return retVal;
} /* pciev3_read_plconfIatuRegLowerTarget_reg */


/*****************************************************************************
 * Combine and write the PL CONF iATU Region Lower Target Address register
 ****************************************************************************/
pcieRet_e pciev3_write_plconfIatuRegLowerTarget_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  const pciePlconfIatuIndexReg_t *simIatuWindow,
  pciePlconfIatuRegLowerTargetReg_t *swReg
)
{
  uint32_t new_val = swReg->raw;
  pcieRet_e retVal = pcie_RET_OK;
  /* Don't need to check NULL this is internal API */
  uint8_t regionIndex = simIatuWindow->regionIndex;

  pcie_range_check_begin;

  if (regionIndex < 16)
  {
    if (simIatuWindow->regionDirection == 0U)
    {
       /* 0U == OUTBOUND */
       pcie_setbits(new_val, CSL_PCIE_RP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_DATA, swReg->iatuRegLowerTarget);
       pcie_setbits(new_val, CSL_PCIE_RP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_REGION_SIZE, swReg->zero);
       swReg->raw = baseAddr->ATU_WRAPPER_OB[regionIndex].AXI_ADDR0 = new_val;
    }
    else
    {
       /* INBOUND */
    }
  }
  else
  {
    retVal = pcie_RET_RANGECHK;
  }

  return retVal;
} /* pciev3_write_plconfIatuRegLowerTarget_reg */


/*****************************************************************************
 * Read and split up the PL CONF iATU Region Upper Target Address register
 ****************************************************************************/
pcieRet_e pciev3_read_plconfIatuRegUpperTarget_reg
(
  const CSL_pcie_ep_coreRegs *baseAddr,
  const pciePlconfIatuIndexReg_t *simIatuWindow,
  pciePlconfIatuRegUpperTargetReg_t *swReg
)
{
  pcieRet_e retVal = pcie_RET_OK;
  uint32_t val;
  /* Don't need to check NULL this is internal API */
  uint8_t regionIndex = simIatuWindow->regionIndex;

  if (regionIndex < 16)
  {
    if (simIatuWindow->regionDirection == 0U)
    {
       /* 0U == OUTBOUND */
       val = swReg->raw = baseAddr->ATU_WRAPPER_OB[regionIndex].AXI_ADDR1;

       pcie_getbits(val, CSL_PCIE_RP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR1_DATA, swReg->iatuRegUpperTarget);
    }
    else
    {
       /* INBOUND */
    }
  }
  else
  {
    retVal = pcie_RET_RANGECHK;
  }

  return retVal;
} /* pciev3_read_plconfIatuRegUpperTarget_reg */


/*****************************************************************************
 * Combine and write the PL CONF iATU Region Upper Target Address register
 ****************************************************************************/
pcieRet_e pciev3_write_plconfIatuRegUpperTarget_reg
(
  CSL_pcie_ep_coreRegs *baseAddr,
  const pciePlconfIatuIndexReg_t *simIatuWindow,
  pciePlconfIatuRegUpperTargetReg_t *swReg
)
{
  uint32_t new_val = swReg->raw;
  pcieRet_e retVal = pcie_RET_OK;
  /* Don't need to check NULL this is internal API */
  uint8_t regionIndex = simIatuWindow->regionIndex;

  pcie_range_check_begin;

  if (regionIndex < 16)
  {
    if (simIatuWindow->regionDirection == 0U)
    {
       /* 0U == OUTBOUND */
       pcie_setbits(new_val, CSL_PCIE_RP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR1_DATA, swReg->iatuRegUpperTarget);
       swReg->raw = baseAddr->ATU_WRAPPER_OB[regionIndex].AXI_ADDR1 = new_val;
    }
    else
    {
       /* INBOUND */
    }
  }
  else
  {
    retVal = pcie_RET_RANGECHK;
  }

  return retVal;
} /* pciev3_write_plconfIatuRegUpperTarget_reg */


/* Nothing past this point */

