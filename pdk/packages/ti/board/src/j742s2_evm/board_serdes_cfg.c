/******************************************************************************
 * Copyright (c) 2024 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file   board_serdes_cfg.c
 *
 *  \brief  EVM serdes configuration file
 *
 *  Configures the serdes module.
 *
 */

#include "board_serdes_cfg.h"
#include "board_internal.h"

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
    CSL_SerdesLaneEnableParams serdesLaneEnableParams  = {0};

    memset(&serdesLaneEnableParams, 0, sizeof(serdesLaneEnableParams));

    /* SGMII Config */
    serdesLaneEnableParams.serdesInstance    = (CSL_SerdesInstance)BOARD_SERDES_SGMII_INSTANCE;
    serdesLaneEnableParams.baseAddr          = CSL_WIZ16B8M4CT3_1_WIZ16B8M4CT3_BASE;
    serdesLaneEnableParams.refClock          = CSL_SERDES_REF_CLOCK_100M;
    serdesLaneEnableParams.refClkSrc         = CSL_SERDES_REF_CLOCK_INT0;
    serdesLaneEnableParams.linkRate          = CSL_SERDES_LINK_RATE_1p25G;
    serdesLaneEnableParams.numLanes          = BOARD_SERDES_SGMII_ENET1_LANE_COUNT;
    serdesLaneEnableParams.laneMask          = BOARD_SERDES_SGMII_ENET1_LANE_MASK;
    serdesLaneEnableParams.SSC_mode          = CSL_SERDES_NO_SSC;
    serdesLaneEnableParams.phyType           = CSL_SERDES_PHY_TYPE_SGMII;
    serdesLaneEnableParams.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    serdesLaneEnableParams.phyInstanceNum    = BOARD_SERDES_LANE_SELECT_CPSW;
    serdesLaneEnableParams.pcieGenType       = CSL_SERDES_PCIE_GEN3;

    serdesLaneEnableParams.laneCtrlRate[BOARD_SERDES_SGMII_ENET1_LANE_NUM] = CSL_SERDES_LANE_FULL_RATE;
    serdesLaneEnableParams.loopbackMode[BOARD_SERDES_SGMII_ENET1_LANE_NUM] = CSL_SERDES_LOOPBACK_DISABLED;

    CSL_serdesPorReset(serdesLaneEnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLaneEnableParams.phyType,
                       serdesLaneEnableParams.phyInstanceNum,
                       serdesLaneEnableParams.serdesInstance,
                       BOARD_SERDES_SGMII_ENET1_LANE_NUM);


    result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                 serdesLaneEnableParams.baseAddr,
                                 serdesLaneEnableParams.refClock,
                                 serdesLaneEnableParams.refClkSrc,
                                 serdesLaneEnableParams.serdesInstance,
                                 serdesLaneEnableParams.phyType);

    if (result != CSL_SERDES_NO_ERR)
    {
        return BOARD_FAIL;
    }
    /* Assert PHY reset and disable all lanes */
    CSL_serdesDisablePllAndLanes(serdesLaneEnableParams.baseAddr,
                                 serdesLaneEnableParams.numLanes,
                                 serdesLaneEnableParams.laneMask);

    /* Load the Serdes Config File */
    result = CSL_serdesEthernetInit(&serdesLaneEnableParams);
    /* Return error if input params are invalid */
    if (result != CSL_SERDES_NO_ERR)
    {
        return BOARD_FAIL;
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&serdesLaneEnableParams);
    if (laneRetVal != 0)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

static Board_STATUS Board_CfgQsgmii(void)
{
    CSL_SerdesResult result;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams serdesLaneEnableParams  = {0};

    memset(&serdesLaneEnableParams, 0, sizeof(serdesLaneEnableParams));

    /* QSGMII Config */
    serdesLaneEnableParams.serdesInstance    = (CSL_SerdesInstance)BOARD_SERDES_SGMII_INSTANCE;
    serdesLaneEnableParams.baseAddr          = CSL_WIZ16B8M4CT3_1_WIZ16B8M4CT3_BASE;
    serdesLaneEnableParams.refClock          = CSL_SERDES_REF_CLOCK_100M;
    serdesLaneEnableParams.refClkSrc         = CSL_SERDES_REF_CLOCK_INT0;
    serdesLaneEnableParams.linkRate          = CSL_SERDES_LINK_RATE_5G;
    serdesLaneEnableParams.numLanes          = BOARD_SERDES_SGMII_ENET1_LANE_COUNT;
    serdesLaneEnableParams.laneMask          = BOARD_SERDES_SGMII_ENET1_LANE_MASK;
    serdesLaneEnableParams.SSC_mode          = CSL_SERDES_NO_SSC;
    serdesLaneEnableParams.phyType           = CSL_SERDES_PHY_TYPE_QSGMII;
    serdesLaneEnableParams.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    serdesLaneEnableParams.phyInstanceNum    = BOARD_SERDES_LANE_SELECT_CPSW;
    serdesLaneEnableParams.pcieGenType       = CSL_SERDES_PCIE_GEN4;

    serdesLaneEnableParams.laneCtrlRate[BOARD_SERDES_SGMII_ENET1_LANE_NUM] = CSL_SERDES_LANE_FULL_RATE;
    serdesLaneEnableParams.loopbackMode[BOARD_SERDES_SGMII_ENET1_LANE_NUM] = CSL_SERDES_LOOPBACK_DISABLED;

    CSL_serdesPorReset(serdesLaneEnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLaneEnableParams.phyType,
                       serdesLaneEnableParams.phyInstanceNum,
                       serdesLaneEnableParams.serdesInstance,
                       BOARD_SERDES_SGMII_ENET1_LANE_NUM);


    result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                 serdesLaneEnableParams.baseAddr,
                                 serdesLaneEnableParams.refClock,
                                 serdesLaneEnableParams.refClkSrc,
                                 serdesLaneEnableParams.serdesInstance,
                                 serdesLaneEnableParams.phyType);

    if (result != CSL_SERDES_NO_ERR)
    {
        return BOARD_FAIL;
    }

    /* Load the Serdes Config File */
    result = CSL_serdesEthernetInit(&serdesLaneEnableParams);
    /* Return error if input params are invalid */
    if (result != CSL_SERDES_NO_ERR)
    {
        return BOARD_FAIL;
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&serdesLaneEnableParams);
    if (laneRetVal != 0)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

static Board_STATUS Board_CfgQsgmii1(void)
{
    CSL_SerdesResult result;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams serdesLaneEnableParams  = {0};

    memset(&serdesLaneEnableParams, 0, sizeof(serdesLaneEnableParams));

    /* QSGMII Config */
    serdesLaneEnableParams.serdesInstance    = (CSL_SerdesInstance)BOARD_SERDES_SGMII_INSTANCE;
    serdesLaneEnableParams.baseAddr          = CSL_WIZ16B8M4CT3_1_WIZ16B8M4CT3_BASE;
    serdesLaneEnableParams.refClock          = CSL_SERDES_REF_CLOCK_100M;
    serdesLaneEnableParams.refClkSrc         = CSL_SERDES_REF_CLOCK_INT0;
    serdesLaneEnableParams.linkRate          = CSL_SERDES_LINK_RATE_5G;
    serdesLaneEnableParams.numLanes          = BOARD_SERDES_SGMII_ENET1_LANE_COUNT;
    serdesLaneEnableParams.laneMask          = BOARD_SERDES_SGMII_ENET1_LANE_MASK;
    serdesLaneEnableParams.SSC_mode          = CSL_SERDES_NO_SSC;
    serdesLaneEnableParams.phyType           = CSL_SERDES_PHY_TYPE_QSGMII;
    serdesLaneEnableParams.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    serdesLaneEnableParams.phyInstanceNum    = BOARD_SERDES_LANE_SELECT_CPSW;
    serdesLaneEnableParams.pcieGenType       = CSL_SERDES_PCIE_GEN4;

    serdesLaneEnableParams.laneCtrlRate[BOARD_SERDES_SGMII_ENET1_LANE_NUM] = CSL_SERDES_LANE_FULL_RATE;
    serdesLaneEnableParams.loopbackMode[BOARD_SERDES_SGMII_ENET1_LANE_NUM] = CSL_SERDES_LOOPBACK_DISABLED;

    CSL_serdesPorReset(serdesLaneEnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLaneEnableParams.phyType,
                       serdesLaneEnableParams.phyInstanceNum,
                       serdesLaneEnableParams.serdesInstance,
                       BOARD_SERDES_SGMII_ENET1_LANE_NUM);


    result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                 serdesLaneEnableParams.baseAddr,
                                 serdesLaneEnableParams.refClock,
                                 serdesLaneEnableParams.refClkSrc,
                                 serdesLaneEnableParams.serdesInstance,
                                 serdesLaneEnableParams.phyType);

    if (result != CSL_SERDES_NO_ERR)
    {
        return BOARD_FAIL;
    }

    /* Load the Serdes Config File */
    result = CSL_serdesEthernetInit(&serdesLaneEnableParams);
    /* Return error if input params are invalid */
    if (result != CSL_SERDES_NO_ERR)
    {
        return BOARD_FAIL;
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&serdesLaneEnableParams);
    if (laneRetVal != 0)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

static Board_STATUS Board_CfgQsgmii2(void)
{
    CSL_SerdesResult result;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams serdesLaneEnableParams  = {0};

    memset(&serdesLaneEnableParams, 0, sizeof(serdesLaneEnableParams));

    /* QSGMII Config */
    serdesLaneEnableParams.serdesInstance    = (CSL_SerdesInstance)BOARD_SERDES_SGMII_INSTANCE;
    serdesLaneEnableParams.baseAddr          = CSL_WIZ16B8M4CT3_1_WIZ16B8M4CT3_BASE;
    serdesLaneEnableParams.refClock          = CSL_SERDES_REF_CLOCK_100M;
    serdesLaneEnableParams.refClkSrc         = CSL_SERDES_REF_CLOCK_INT0;
    serdesLaneEnableParams.linkRate          = CSL_SERDES_LINK_RATE_5G;
    serdesLaneEnableParams.numLanes          = BOARD_SERDES_SGMII_ENET2_LANE_COUNT;
    serdesLaneEnableParams.laneMask          = BOARD_SERDES_SGMII_ENET2_LANE_MASK;
    serdesLaneEnableParams.SSC_mode          = CSL_SERDES_NO_SSC;
    serdesLaneEnableParams.phyType           = CSL_SERDES_PHY_TYPE_QSGMII;
    serdesLaneEnableParams.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    serdesLaneEnableParams.phyInstanceNum    = BOARD_SERDES_LANE_SELECT_CPSW;
    serdesLaneEnableParams.pcieGenType       = CSL_SERDES_PCIE_GEN4;

    serdesLaneEnableParams.laneCtrlRate[BOARD_SERDES_SGMII_ENET2_LANE_NUM] = CSL_SERDES_LANE_FULL_RATE;
    serdesLaneEnableParams.loopbackMode[BOARD_SERDES_SGMII_ENET2_LANE_NUM] = CSL_SERDES_LOOPBACK_DISABLED;

    CSL_serdesPorReset(serdesLaneEnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLaneEnableParams.phyType,
                       serdesLaneEnableParams.phyInstanceNum,
                       serdesLaneEnableParams.serdesInstance,
                       BOARD_SERDES_SGMII_ENET2_LANE_NUM);


    result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                 serdesLaneEnableParams.baseAddr,
                                 serdesLaneEnableParams.refClock,
                                 serdesLaneEnableParams.refClkSrc,
                                 serdesLaneEnableParams.serdesInstance,
                                 serdesLaneEnableParams.phyType);

    if (result != CSL_SERDES_NO_ERR)
    {
        return BOARD_FAIL;
    }

    /* Load the Serdes Config File */
    result = CSL_serdesEthernetInit(&serdesLaneEnableParams);
    /* Return error if input params are invalid */
    if (result != CSL_SERDES_NO_ERR)
    {
        return BOARD_FAIL;
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&serdesLaneEnableParams);
    if (laneRetVal != 0)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

static Board_STATUS Board_serdesCfgEthernetUsxgmii(void)
{
    CSL_SerdesResult result;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams serdesLaneEnableParams;

    memset(&serdesLaneEnableParams, 0, sizeof(serdesLaneEnableParams));

    /* Serdes-2: Lane 3 (MAC Port 8) */
    serdesLaneEnableParams.serdesInstance    = (CSL_SerdesInstance)BOARD_SERDES_SGMII_INSTANCE;
    serdesLaneEnableParams.baseAddr          = CSL_WIZ16B8M4CT3_1_WIZ16B8M4CT3_BASE;
    serdesLaneEnableParams.refClock          = CSL_SERDES_REF_CLOCK_156p25M;
    serdesLaneEnableParams.refClkSrc         = CSL_SERDES_REF_CLOCK_INT0;
    serdesLaneEnableParams.numLanes          = BOARD_SERDES_SGMII_ENET2_LANE_COUNT;
    serdesLaneEnableParams.laneMask          = BOARD_SERDES_SGMII_ENET2_LANE_MASK;
    serdesLaneEnableParams.SSC_mode          = CSL_SERDES_NO_SSC;
    serdesLaneEnableParams.phyType           = CSL_SERDES_PHY_TYPE_USXGMII;
    serdesLaneEnableParams.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    serdesLaneEnableParams.phyInstanceNum    = BOARD_SERDES_LANE_SELECT_CPSW;

    serdesLaneEnableParams.laneCtrlRate[BOARD_SERDES_SGMII_ENET2_LANE_NUM] = CSL_SERDES_LANE_FULL_RATE;
    serdesLaneEnableParams.loopbackMode[BOARD_SERDES_SGMII_ENET2_LANE_NUM] = CSL_SERDES_LOOPBACK_DISABLED;

    serdesLaneEnableParams.pcieGenType       = CSL_SERDES_PCIE_GEN4;
    serdesLaneEnableParams.linkRate          = CSL_SERDES_LINK_RATE_5p15625G;
    /* End: Serdes-1*/

    CSL_serdesPorReset(serdesLaneEnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLaneEnableParams.phyType,
                       serdesLaneEnableParams.phyInstanceNum,
                       serdesLaneEnableParams.serdesInstance,
                       BOARD_SERDES_SGMII_ENET2_LANE_NUM);

    result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                 serdesLaneEnableParams.baseAddr,
                                 serdesLaneEnableParams.refClock,
                                 serdesLaneEnableParams.refClkSrc,
                                 serdesLaneEnableParams.serdesInstance,
                                 serdesLaneEnableParams.phyType);

    if (result != CSL_SERDES_NO_ERR)
    {
        return BOARD_FAIL;
    }

    /* Load the Serdes Config File */
    result = CSL_serdesEthernetInit(&serdesLaneEnableParams);
    /* Return error if input params are invalid */
    if (result != CSL_SERDES_NO_ERR)
    {
        return BOARD_FAIL;
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&serdesLaneEnableParams);
    if (laneRetVal != 0)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

/**
 *  \brief serdes configurations for Sierra 1 in SGMII mode
 *
 *  The function configures the serdes module for SGMII instance
 *
 * \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgSgmii(void)
{
    Board_STATUS ret;

    /* Unlock MMR write access */
    Board_serdesKickCtrl(0);

    /* SGMII SERDES initializations */
    ret = Board_CfgSgmii();
    Board_serdesKickCtrl(1); /* Lock MMR write access */
    if(ret != BOARD_SOK)
    {
        return ret;
    }

    return BOARD_SOK;
}

/**
 *  \brief serdes configurations for Sierra 1 in QSGMII mode
 *
 *  The function configures the serdes module for QSGMII instances
 *
 * \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgQsgmii(uint8_t serdesType)
{
    Board_STATUS ret;

    /* Unlock MMR write access */
    Board_serdesKickCtrl(0);

	if (serdesType == BOARD_SERDES_ENET1_CARD)
	{
		/* QSGMII SERDES1 initializations */
        ret = Board_CfgQsgmii1();
	}
	else if (serdesType == BOARD_SERDES_ENET2_CARD)
	{
		/* QSGMII SERDES2 initializations */
        ret = Board_CfgQsgmii2();
	}
    else
    {
        /* QSGMII SERDES initializations */
        ret = Board_CfgQsgmii();
    }

    Board_serdesKickCtrl(1); /* Lock MMR write access */
    if(ret != BOARD_SOK)
    {
        return ret;
    }

    return BOARD_SOK;
}

/**
 *  \brief serdes configurations for USXGMII mode
 *
 *  The function configures the serdes module for USXGMII interface
 *
 * \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgUsxgmii(void)
{
    Board_STATUS ret;

    /* Unlock MMR write access */
    Board_serdesKickCtrl(0);
    /* USXGMII SERDES initializations */
    ret = Board_serdesCfgEthernetUsxgmii();
    Board_serdesKickCtrl(1); /* Lock MMR write access */
    if(ret != BOARD_SOK)
    {
        return ret;
    }

    return BOARD_SOK;
}

/**
 *  \brief Query SerDes configuration status
 *
 *  The function gets the configuration status of Torrent SerDes module.
 *
 *  \retval TRUE    SerDes1 is configured
 *  \retval FALSE   SerDes1 is not configured
 */
int32_t Board_serdesCfgStatus(void)
{
    CSL_SerdesStatus serdesStatus;
    int32_t ret = FALSE;

    serdesStatus = CSL_serdesConfigStatus(CSL_WIZ16B8M4CT3_1_WIZ16B8M4CT3_BASE);
    if (serdesStatus == CSL_SERDES_STATUS_PLL_LOCKED)
    {
        ret = TRUE;
    }

    return ret;
}
