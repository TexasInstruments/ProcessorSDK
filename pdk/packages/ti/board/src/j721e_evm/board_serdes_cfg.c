/******************************************************************************
 * Copyright (c) 2019-2022 Texas Instruments Incorporated - http://www.ti.com
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
 *****************************************************************************/
/**
 *  \file   board_cfg.c
 *
 *  \brief  EVM serdes configuration file
 *
 *  Configures the serdes module.
 *
 */

#include "board_serdes_cfg.h"

/**
 * \brief  Configures kick registers for Pinmux MMR access
 *
 * \param   lockCtrl [IN]   Register lock/unlock control
 *                          0 - Unlocks the MMR register write access
 *                          1 - Locks the MMR register write access
 *
 * \return  Board_STATUS
 */
static Board_STATUS Board_serdesKickCtrl(uint32_t lockCtrl)
{
    Board_STATUS status;

    if(lockCtrl)
    {
        status  = Board_lockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION1);
        status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION2);
    }
    else
    {
        status  = Board_unlockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION1);
        status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION2);
    }

    return (status);
}

static Board_STATUS Board_CfgSgmii(void)
{
    CSL_SerdesResult result;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams serdesLane0EnableParams  = {0};

    memset(&serdesLane0EnableParams, 0, sizeof(serdesLane0EnableParams));

    /* SGMII Config */
    serdesLane0EnableParams.serdesInstance    = (CSL_SerdesInstance)SGMII_SERDES_INSTANCE;
    serdesLane0EnableParams.baseAddr          = CSL_SERDES_16G0_BASE;
    serdesLane0EnableParams.refClock          = CSL_SERDES_REF_CLOCK_100M;
    serdesLane0EnableParams.refClkSrc         = CSL_SERDES_REF_CLOCK_INT;
    serdesLane0EnableParams.linkRate          = CSL_SERDES_LINK_RATE_1p25G;
    serdesLane0EnableParams.numLanes          = 0x2;
    serdesLane0EnableParams.laneMask          = 0x3;
    serdesLane0EnableParams.SSC_mode          = CSL_SERDES_NO_SSC;
    serdesLane0EnableParams.phyType           = CSL_SERDES_PHY_TYPE_SGMII;
    serdesLane0EnableParams.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    serdesLane0EnableParams.phyInstanceNum    = SERDES_LANE_SELECT_CPSW;
    serdesLane0EnableParams.pcieGenType        = CSL_SERDES_PCIE_GEN3;

    serdesLane0EnableParams.laneCtrlRate[0]   = CSL_SERDES_LANE_FULL_RATE;
    serdesLane0EnableParams.loopbackMode[0]   = CSL_SERDES_LOOPBACK_DISABLED;

    serdesLane0EnableParams.laneCtrlRate[1]   = CSL_SERDES_LANE_FULL_RATE;
    serdesLane0EnableParams.loopbackMode[1]   = CSL_SERDES_LOOPBACK_DISABLED;

    CSL_serdesPorReset(serdesLane0EnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLane0EnableParams.phyType,
                       serdesLane0EnableParams.phyInstanceNum,
                       serdesLane0EnableParams.serdesInstance,
                       SGMII_LANE_NUM);


    result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                 serdesLane0EnableParams.baseAddr,
                                 serdesLane0EnableParams.refClock,
                                 serdesLane0EnableParams.refClkSrc,
                                 serdesLane0EnableParams.serdesInstance,
                                 serdesLane0EnableParams.phyType);

    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }
    /* Assert PHY reset and disable all lanes */
    CSL_serdesDisablePllAndLanes(serdesLane0EnableParams.baseAddr, serdesLane0EnableParams.numLanes, serdesLane0EnableParams.laneMask);

    /* Load the Serdes Config File */
    result = CSL_serdesEthernetInit(&serdesLane0EnableParams);
    /* Return error if input params are invalid */
    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&serdesLane0EnableParams);
    if (CSL_SERDES_LANE_ENABLE_NO_ERR != laneRetVal)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

static Board_STATUS Board_CfgQsgmii(void)
{
    CSL_SerdesResult result;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams serdesLane0EnableParams  = {0};

    memset(&serdesLane0EnableParams, 0, sizeof(serdesLane0EnableParams));

    /* QSGMII Config */
    serdesLane0EnableParams.serdesInstance    = (CSL_SerdesInstance)SGMII_SERDES_INSTANCE;
    serdesLane0EnableParams.baseAddr          = CSL_SERDES_16G0_BASE;
    serdesLane0EnableParams.refClock          = CSL_SERDES_REF_CLOCK_100M;
    serdesLane0EnableParams.refClkSrc         = CSL_SERDES_REF_CLOCK_INT;
    serdesLane0EnableParams.linkRate          = CSL_SERDES_LINK_RATE_5G;
    serdesLane0EnableParams.numLanes          = 1;
    serdesLane0EnableParams.laneMask          = 0x2;
    serdesLane0EnableParams.SSC_mode          = CSL_SERDES_NO_SSC;
    serdesLane0EnableParams.phyType           = CSL_SERDES_PHY_TYPE_QSGMII;
    serdesLane0EnableParams.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    serdesLane0EnableParams.phyInstanceNum    = SERDES_LANE_SELECT_CPSW;
    serdesLane0EnableParams.pcieGenType       = CSL_SERDES_PCIE_GEN4;
    serdesLane0EnableParams.laneCtrlRate[1]   = CSL_SERDES_LANE_FULL_RATE;
    serdesLane0EnableParams.loopbackMode[1]   = CSL_SERDES_LOOPBACK_DISABLED;

    CSL_serdesPorReset(serdesLane0EnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLane0EnableParams.phyType,
                       serdesLane0EnableParams.phyInstanceNum,
                       serdesLane0EnableParams.serdesInstance,
                       SGMII_LANE_NUM);


    result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                 serdesLane0EnableParams.baseAddr,
                                 serdesLane0EnableParams.refClock,
                                 serdesLane0EnableParams.refClkSrc,
                                 serdesLane0EnableParams.serdesInstance,
                                 serdesLane0EnableParams.phyType);

    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Load the Serdes Config File */
    result = CSL_serdesEthernetInit(&serdesLane0EnableParams);
    /* Return error if input params are invalid */
    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&serdesLane0EnableParams);
    if (CSL_SERDES_LANE_ENABLE_NO_ERR != laneRetVal)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

static Board_STATUS Board_CfgSierra0Clks(void)
{
    uint64_t clkRateHz = 100000000U, clkRateRead = 0U;
    uint32_t moduleId  = TISCI_DEV_SERDES_16G0;
    int32_t i, result;
    uint32_t clkID[] = { TISCI_DEV_SERDES_16G0_CORE_REF1_CLK,
                         TISCI_DEV_SERDES_16G0_CORE_REF_CLK };
    uint32_t parentID[] = { TISCI_DEV_SERDES_16G0_CORE_REF1_CLK_PARENT_HSDIV4_16FFT_MAIN_2_HSDIVOUT4_CLK,
                            TISCI_DEV_SERDES_16G0_CORE_REF_CLK_PARENT_HSDIV4_16FFT_MAIN_2_HSDIVOUT4_CLK };

    for(i = 0; i < (sizeof(clkID)/sizeof(clkID[0])); i++)
    {
        /* Disable the clock */
        result = Sciclient_pmModuleClkRequest(moduleId,
                                              clkID[i],
                                              TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ,
                                              0U,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
        if (CSL_SERDES_NO_ERR != result)
        {
            return BOARD_FAIL;
        }

        /* Set the parent */
        result = Sciclient_pmSetModuleClkParent(moduleId,
                                                clkID[i],
                                                parentID[i],
                                                SCICLIENT_SERVICE_WAIT_FOREVER);

        if (CSL_SERDES_NO_ERR != result)
        {
            return BOARD_FAIL;
        }

        /* Set the clock at the desirable frequency */
        result = Sciclient_pmSetModuleClkFreq(moduleId,
                                              clkID[i],
                                              clkRateHz,
                                              TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);

        if (CSL_SERDES_NO_ERR != result)
        {
            return BOARD_FAIL;
        }

        /* Confirm if clock is set */
        clkRateRead = 0U;
        result = Sciclient_pmGetModuleClkFreq(moduleId,
                                              clkID[i],
                                              &clkRateRead,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);

        if ((CSL_SERDES_NO_ERR != result) || (clkRateRead != clkRateHz))
        {
            return BOARD_FAIL;
        }
    }

    /* Set module to ON state */
    result = Sciclient_pmSetModuleState(moduleId,
                                        TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                        (TISCI_MSG_FLAG_AOP | TISCI_MSG_FLAG_DEVICE_RESET_ISO),
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Reset if changed state to enabled */
    result = Sciclient_pmSetModuleRst(moduleId,
                                      0x0U /*resetBit*/,
                                      SCICLIENT_SERVICE_WAIT_FOREVER);

    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }


    return BOARD_SOK;
}

static Board_STATUS Board_CfgMultilinkPcieQsgmii(void)
{
    CSL_SerdesResult result;
    CSL_SerdesMultilink multiLinkId = CSL_SERDES_PCIe_QSGMII_MULTILINK;
    CSL_SerdesInstance serdesInstanceId = CSL_SIERRA_SERDES0;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams serdesLane0EnableParams;
    CSL_SerdesLaneEnableParams serdesLane1EnableParams;

    memset(&serdesLane0EnableParams, 0, sizeof(serdesLane0EnableParams));
    memset(&serdesLane1EnableParams, 0, sizeof(serdesLane1EnableParams));

    /* PCIe Config */
    serdesLane0EnableParams.serdesInstance    = CSL_SIERRA_SERDES0;
    serdesLane0EnableParams.baseAddr          = CSL_SERDES_16G0_BASE;
    serdesLane0EnableParams.refClock          = CSL_SERDES_REF_CLOCK_100M;
    serdesLane0EnableParams.refClkSrc         = CSL_SERDES_REF_CLOCK_INT;
    serdesLane0EnableParams.linkRate          = CSL_SERDES_LINK_RATE_5G;
    serdesLane0EnableParams.numLanes          = 1U;
    serdesLane0EnableParams.laneMask          = 0x1U;
    serdesLane0EnableParams.SSC_mode          = CSL_SERDES_NO_SSC;
    serdesLane0EnableParams.phyType           = CSL_SERDES_PHY_TYPE_PCIe;
    serdesLane0EnableParams.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    serdesLane0EnableParams.phyInstanceNum    = SERDES_PCIE_PHY_INST_NUM;
    serdesLane0EnableParams.pcieGenType       = CSL_SERDES_PCIE_GEN4;
    serdesLane0EnableParams.laneCtrlRate[0]   = CSL_SERDES_LANE_FULL_RATE;
    serdesLane0EnableParams.loopbackMode[0]   = CSL_SERDES_LOOPBACK_DISABLED;

    /* QSGMII Config */
    serdesLane1EnableParams.serdesInstance    = CSL_SIERRA_SERDES0;
    serdesLane1EnableParams.baseAddr          = CSL_SERDES_16G0_BASE;
    serdesLane1EnableParams.refClock          = CSL_SERDES_REF_CLOCK_100M;
    serdesLane1EnableParams.refClkSrc         = CSL_SERDES_REF_CLOCK_INT;
    serdesLane1EnableParams.linkRate          = CSL_SERDES_LINK_RATE_5G;
    serdesLane1EnableParams.numLanes          = 1U;
    serdesLane1EnableParams.laneMask          = 0x2U;
    serdesLane1EnableParams.SSC_mode          = CSL_SERDES_NO_SSC;
    serdesLane1EnableParams.phyType           = CSL_SERDES_PHY_TYPE_QSGMII;
    serdesLane1EnableParams.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    serdesLane1EnableParams.phyInstanceNum    = SERDES_LANE_SELECT_CPSW;
    serdesLane1EnableParams.pcieGenType       = CSL_SERDES_PCIE_GEN4;
    serdesLane1EnableParams.laneCtrlRate[1]   = CSL_SERDES_LANE_FULL_RATE;
    serdesLane1EnableParams.loopbackMode[1]   = CSL_SERDES_LOOPBACK_DISABLED;

    CSL_serdesPorReset(CSL_SERDES_16G0_BASE);

    /* Select the IP type, IP instance num, Serdes Lane Number for PCIe */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLane0EnableParams.phyType,
                       serdesLane0EnableParams.phyInstanceNum,
                       serdesLane0EnableParams.serdesInstance,
                       PCIE0_LANE_NUM);

    /* Select the IP type, IP instance num, Serdes Lane Number for QSGMII */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLane1EnableParams.phyType,
                       serdesLane1EnableParams.phyInstanceNum,
                       serdesLane1EnableParams.serdesInstance,
                       SGMII_LANE_NUM);

    result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                 CSL_SERDES_16G0_BASE,
                                 CSL_SERDES_REF_CLOCK_100M,
                                 CSL_SERDES_REF_CLOCK_INT,
                                 CSL_SIERRA_SERDES0,
                                 0U /* unused */);

    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Load the Serdes Config File */
    result = CSL_serdesMultiLinkInit(multiLinkId,
                                     serdesInstanceId,
                                     &serdesLane0EnableParams,
                                     &serdesLane1EnableParams);
    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Enable PCIe lane */
    laneRetVal = CSL_serdesLaneEnable(&serdesLane0EnableParams);
    if (CSL_SERDES_LANE_ENABLE_NO_ERR != laneRetVal)
    {
        return BOARD_FAIL;
    }

    /* Enable QSGMII lane */
    laneRetVal = CSL_serdesLaneEnable(&serdesLane1EnableParams);
    if (CSL_SERDES_LANE_ENABLE_NO_ERR != laneRetVal)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

static Board_STATUS Board_serdesCfgEthernetXaui(void)
{
    CSL_SerdesResult result;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams laneParams_serdes0;

    memset(&laneParams_serdes0, 0, sizeof(laneParams_serdes0));

    /* Serdes-0: Lane 1 (MAC Port 1) */
    laneParams_serdes0.serdesInstance    = (CSL_SerdesInstance)SGMII_SERDES_INSTANCE;
    laneParams_serdes0.baseAddr          = CSL_SERDES_16G0_BASE;
    laneParams_serdes0.refClock          = CSL_SERDES_REF_CLOCK_100M;
    laneParams_serdes0.refClkSrc         = CSL_SERDES_REF_CLOCK_INT;
    laneParams_serdes0.numLanes          = 0x1;
    laneParams_serdes0.laneMask          = 0x2;
    laneParams_serdes0.SSC_mode          = CSL_SERDES_NO_SSC;
    laneParams_serdes0.phyType           = CSL_SERDES_PHY_TYPE_XAUI;
    laneParams_serdes0.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    laneParams_serdes0.phyInstanceNum    = SERDES_LANE_SELECT_CPSW;

    laneParams_serdes0.laneCtrlRate[1]   = CSL_SERDES_LANE_FULL_RATE;
    laneParams_serdes0.loopbackMode[1]   = CSL_SERDES_LOOPBACK_DISABLED;

    laneParams_serdes0.pcieGenType       = CSL_SERDES_PCIE_GEN4;
    laneParams_serdes0.linkRate          = CSL_SERDES_LINK_RATE_3p125G;
    /* End: Serdes-0: Lane 1 (MAC Port 1) */

    CSL_serdesPorReset(laneParams_serdes0.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       laneParams_serdes0.phyType,
                       laneParams_serdes0.phyInstanceNum,
                       laneParams_serdes0.serdesInstance,
                       SGMII_LANE_NUM);

    result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                 laneParams_serdes0.baseAddr,
                                 laneParams_serdes0.refClock,
                                 laneParams_serdes0.refClkSrc,
                                 laneParams_serdes0.serdesInstance,
                                 laneParams_serdes0.phyType);

    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Load the Serdes Config File */
    result = CSL_serdesEthernetInit(&laneParams_serdes0);
    /* Return error if input params are invalid */
    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&laneParams_serdes0);
    if (CSL_SERDES_LANE_ENABLE_NO_ERR != laneRetVal)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

/**
 *  \brief serdes configurations
 *
 *  The function configures the serdes1 module for one lane pcie interface
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgSgmii(void)
{
    Board_STATUS ret;

    /* Unlock MMR write access */
    Board_serdesKickCtrl(0);

    /* SERDES0 Initializations */
    ret = Board_CfgSgmii();
    /* Lock MMR write access */
    Board_serdesKickCtrl(1);
    if(BOARD_SOK != ret)
    {
        return ret;
    }

    return BOARD_SOK;
}

/**
 *  \brief serdes configurations
 *
 *  The function configures the serdes1 module for one lane pcie interface
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgQsgmii(void)
{
    Board_STATUS ret;

    /* Unlock MMR write access */
    Board_serdesKickCtrl(0);

    /* SERDES0 Initializations */
    ret = Board_CfgQsgmii();
    /* Lock MMR write access */
    Board_serdesKickCtrl(1);
    if(BOARD_SOK != ret)
    {
        return ret;
    }

    return BOARD_SOK;
}

/**
 *  \brief serdes configurations
 *
 *  The function configures the serdes module for XAUI interface
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgXaui(void)
{
    Board_STATUS ret;

    /* Unlock MMR write access */
    Board_serdesKickCtrl(0);

    /* SERDES0 Initializations */
    ret = Board_serdesCfgEthernetXaui();
    /* Lock MMR write access */
    Board_serdesKickCtrl(1);
    if(BOARD_SOK != ret)
    {
        return ret;
    }

    return BOARD_SOK;
}

bool Board_serdesCfgStatus(void)
{
    CSL_SerdesStatus serdesStatus;
    bool ret = BFALSE;

    serdesStatus = CSL_serdesConfigStatus(CSL_SERDES_16G0_BASE);
    if (CSL_SERDES_STATUS_PLL_LOCKED == serdesStatus)
    {
        ret = BTRUE;
    }

    return ret;
}

/**
 *  \brief serdes configurations
 *
 *  The function configures the serdes for multi-link on Sierra0: PCIe and QSGMII
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfg(void)
{
    Board_STATUS ret;

    /* Unlock MMR write access */
    Board_serdesKickCtrl(0);

    /* Configure SERDES clocks */
    ret = Board_CfgSierra0Clks();
    if(BOARD_SOK == ret)
    {
        /* SERDES0 Initializations */
        ret = Board_CfgMultilinkPcieQsgmii();
    }

    /* Lock MMR write access */
    Board_serdesKickCtrl(1);

    return BOARD_SOK;
}
