/******************************************************************************
 * Copyright (c) 2020-2022 Texas Instruments Incorporated - http://www.ti.com
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

static Board_STATUS Board_serdesInitParams(CSL_SerdesLaneEnableParams *laneParams,
                                           uint32_t phyType)
{
    laneParams->serdesInstance    = (CSL_SerdesInstance)BOARD_SERDES_INSTANCE;
    laneParams->baseAddr          = CSL_SERDES_10G1_BASE;
    laneParams->refClock          = CSL_SERDES_REF_CLOCK_100M;
    laneParams->refClkSrc         = CSL_SERDES_REF_CLOCK_INT;
    laneParams->numLanes          = 0x4;
    laneParams->laneMask          = 0xf;
    laneParams->SSC_mode          = CSL_SERDES_NO_SSC;
    laneParams->phyType           = phyType;
    laneParams->operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    laneParams->phyInstanceNum    = SERDES_LANE_SELECT_CPSW;

    laneParams->laneCtrlRate[0]   = CSL_SERDES_LANE_FULL_RATE;
    laneParams->loopbackMode[0]   = CSL_SERDES_LOOPBACK_DISABLED;

    laneParams->laneCtrlRate[1]   = CSL_SERDES_LANE_FULL_RATE;
    laneParams->loopbackMode[1]   = CSL_SERDES_LOOPBACK_DISABLED;

    if(CSL_SERDES_PHY_TYPE_SGMII == phyType)
    {
        laneParams->pcieGenType       = CSL_SERDES_PCIE_GEN3;
        laneParams->linkRate          = CSL_SERDES_LINK_RATE_1p25G;
    }
    else if (CSL_SERDES_PHY_TYPE_QSGMII == phyType)
    {
        laneParams->pcieGenType       = CSL_SERDES_PCIE_GEN4;
        laneParams->linkRate          = CSL_SERDES_LINK_RATE_5G;
    }
    else
    {
        /* Unsupported phy type */
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

static Board_STATUS Board_serdesCfgEthernet(uint32_t phyType)
{
    CSL_SerdesStatus status;
    CSL_SerdesResult result;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams serdesLane0EnableParams  = {0};

    memset(&serdesLane0EnableParams, 0, sizeof(serdesLane0EnableParams));

    Board_serdesInitParams(&serdesLane0EnableParams, phyType);

    /* Bail out early if SERDES is already configured */
    status = CSL_serdesConfigStatus(serdesLane0EnableParams.baseAddr);
    if (UTRUE == status)
    {
        return BOARD_SOK;
    }

    CSL_serdesPorReset(serdesLane0EnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLane0EnableParams.phyType,
                       serdesLane0EnableParams.phyInstanceNum,
                       serdesLane0EnableParams.serdesInstance,
                       BOARD_SERDES_SGMII_LANE_NUM);

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

static Board_STATUS Board_serdesCfgEthernetUsxgmii(void)
{
    CSL_SerdesStatus status;
    CSL_SerdesResult result;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams laneParams_serdes0;

    memset(&laneParams_serdes0, 0, sizeof(laneParams_serdes0));

    /* Serdes-0: Lanes 2 and 3 (Ports 0 and 1 resp.) */
    laneParams_serdes0.serdesInstance    = (CSL_SerdesInstance)CSL_TORRENT_SERDES0;
    laneParams_serdes0.baseAddr          = CSL_SERDES_10G1_BASE;
    laneParams_serdes0.refClock          = CSL_SERDES_REF_CLOCK_100M;
    laneParams_serdes0.refClkSrc         = CSL_SERDES_REF_CLOCK_INT;
    laneParams_serdes0.numLanes          = 0x1;
    laneParams_serdes0.laneMask          = 0x4;
    laneParams_serdes0.SSC_mode          = CSL_SERDES_NO_SSC;
    laneParams_serdes0.phyType           = CSL_SERDES_PHY_TYPE_USXGMII;
    laneParams_serdes0.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    laneParams_serdes0.phyInstanceNum    = SERDES_LANE_SELECT_CPSW;

    laneParams_serdes0.laneCtrlRate[2]   = CSL_SERDES_LANE_FULL_RATE;
    laneParams_serdes0.loopbackMode[2]   = CSL_SERDES_LOOPBACK_DISABLED;

    laneParams_serdes0.laneCtrlRate[3]   = CSL_SERDES_LANE_FULL_RATE;
    laneParams_serdes0.loopbackMode[3]   = CSL_SERDES_LOOPBACK_DISABLED;

    laneParams_serdes0.pcieGenType       = CSL_SERDES_PCIE_GEN3;
    laneParams_serdes0.linkRate          = CSL_SERDES_LINK_RATE_5p15625G;
    /* End: Serdes-0: Lanes 2 and 3 (MAC Ports 0 and 1) */

    /* Bail out early if SERDES is already configured */
    status = CSL_serdesConfigStatus(laneParams_serdes0.baseAddr);
    if (CSL_SERDES_STATUS_PLL_NOT_LOCKED == status)
    {
        return BOARD_SOK;
    }

    CSL_serdesPorReset(laneParams_serdes0.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       laneParams_serdes0.phyType,
                       laneParams_serdes0.phyInstanceNum,
                       laneParams_serdes0.serdesInstance,
                       2);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       laneParams_serdes0.phyType,
                       laneParams_serdes0.phyInstanceNum,
                       laneParams_serdes0.serdesInstance,
                       3);

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

    /* Assert PHY reset and disable all lanes */
    CSL_serdesDisablePllAndLanes(laneParams_serdes0.baseAddr, laneParams_serdes0.numLanes, laneParams_serdes0.laneMask);

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
    Board_serdesKickCtrl(0U);

    /* SERDES0 Initializations */
    ret = Board_serdesCfgEthernet(CSL_SERDES_PHY_TYPE_SGMII);
    /* Lock MMR write access */
    Board_serdesKickCtrl(1U);
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
    Board_serdesKickCtrl(0U);

    /* SERDES0 Initializations */
    ret = Board_serdesCfgEthernet(CSL_SERDES_PHY_TYPE_QSGMII);
    /* Lock MMR write access */
    Board_serdesKickCtrl(1U);
    if(BOARD_SOK != ret)
    {
        return ret;
    }

    return BOARD_SOK;
}

/**
 *  \brief serdes configurations
 *
 *  The function configures the serdes module for USXGMII interface
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgUsxgmii(void)
{
    Board_STATUS ret;

    /* Unlock MMR write access */
    Board_serdesKickCtrl(0U);

    /* SERDES0 Initializations */
    ret = Board_serdesCfgEthernetUsxgmii();
    /* Lock MMR write access */
    Board_serdesKickCtrl(1U);
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

    serdesStatus = CSL_serdesConfigStatus(CSL_SERDES_10G1_BASE);
    if (CSL_SERDES_STATUS_PLL_LOCKED == serdesStatus)
    {
        ret = BTRUE;
    }

    return ret;
}
