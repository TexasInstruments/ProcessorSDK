/******************************************************************************
 * Copyright (c) 2022 Texas Instruments Incorporated - http://www.ti.com
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
#include "board_utils.h"

extern Board_initParams_t gBoardInitParams;

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

static Board_STATUS Board_CfgSgmii(uint32_t boardID)
{
    CSL_SerdesResult result;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams serdesLaneEnableParams  = {0};
    uint32_t laneNum;
    uint32_t laneMask;

    if (BOARD_ID_ENET == boardID)
    {
        laneNum  = BOARD_SERDES_SGMII_ENET1_LANE_NUM;
        laneMask = BOARD_SERDES_SGMII_ENET1_LANE_MASK;
    }
    else
    {
        laneNum  = BOARD_SERDES_SGMII_ENET2_LANE_NUM;
        laneMask = BOARD_SERDES_SGMII_ENET2_LANE_MASK;
    }

    memset(&serdesLaneEnableParams, 0, sizeof(serdesLaneEnableParams));

    /* SGMII Config */
    serdesLaneEnableParams.serdesInstance    = (CSL_SerdesInstance)BOARD_SERDES_SGMII_INSTANCE;
    serdesLaneEnableParams.baseAddr          = CSL_WIZ16B8M4CT3_2_WIZ16B8M4CT3_BASE;
    serdesLaneEnableParams.refClock          = CSL_SERDES_REF_CLOCK_100M;
    serdesLaneEnableParams.refClkSrc         = CSL_SERDES_REF_CLOCK_INT0;
    serdesLaneEnableParams.linkRate          = CSL_SERDES_LINK_RATE_1p25G;
    serdesLaneEnableParams.numLanes          = 1U;
    serdesLaneEnableParams.laneMask          = laneMask;
    serdesLaneEnableParams.SSC_mode          = CSL_SERDES_NO_SSC;
    serdesLaneEnableParams.phyType           = CSL_SERDES_PHY_TYPE_SGMII;
    serdesLaneEnableParams.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    serdesLaneEnableParams.phyInstanceNum    = BOARD_SERDES_LANE_SELECT_CPSW;
    serdesLaneEnableParams.pcieGenType       = CSL_SERDES_PCIE_GEN3;

    serdesLaneEnableParams.laneCtrlRate[laneNum] = CSL_SERDES_LANE_FULL_RATE;
    serdesLaneEnableParams.loopbackMode[laneNum] = CSL_SERDES_LOOPBACK_DISABLED;

    CSL_serdesPorReset(serdesLaneEnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLaneEnableParams.phyType,
                       serdesLaneEnableParams.phyInstanceNum,
                       serdesLaneEnableParams.serdesInstance,
                       laneNum);


    result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                 serdesLaneEnableParams.baseAddr,
                                 serdesLaneEnableParams.refClock,
                                 serdesLaneEnableParams.refClkSrc,
                                 serdesLaneEnableParams.serdesInstance,
                                 serdesLaneEnableParams.phyType);

    if (CSL_SERDES_NO_ERR != result)
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
    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&serdesLaneEnableParams);
    if (0U != laneRetVal)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

static Board_STATUS Board_CfgQsgmii(uint32_t boardID)
{
    CSL_SerdesResult result;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams serdesLaneEnableParams  = {0};
    uint32_t laneNum;
    uint32_t laneMask;

    if (BOARD_ID_ENET == boardID)
    {
        laneNum  = BOARD_SERDES_SGMII_ENET1_LANE_NUM;
        laneMask = BOARD_SERDES_SGMII_ENET1_LANE_MASK;
    }
    else
    {
        laneNum  = BOARD_SERDES_SGMII_ENET2_LANE_NUM;
        laneMask = BOARD_SERDES_SGMII_ENET2_LANE_MASK;
    }

    memset(&serdesLaneEnableParams, 0, sizeof(serdesLaneEnableParams));

    /* QSGMII Config */
    serdesLaneEnableParams.serdesInstance    = (CSL_SerdesInstance)BOARD_SERDES_SGMII_INSTANCE;
    serdesLaneEnableParams.baseAddr          = CSL_WIZ16B8M4CT3_2_WIZ16B8M4CT3_BASE;
    serdesLaneEnableParams.refClock          = CSL_SERDES_REF_CLOCK_100M;
    serdesLaneEnableParams.refClkSrc         = CSL_SERDES_REF_CLOCK_INT0;
    serdesLaneEnableParams.linkRate          = CSL_SERDES_LINK_RATE_5G;
    serdesLaneEnableParams.numLanes          = 1U;
    serdesLaneEnableParams.laneMask          = laneMask;
    serdesLaneEnableParams.SSC_mode          = CSL_SERDES_NO_SSC;
    serdesLaneEnableParams.phyType           = CSL_SERDES_PHY_TYPE_QSGMII;
    serdesLaneEnableParams.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    serdesLaneEnableParams.phyInstanceNum    = BOARD_SERDES_LANE_SELECT_CPSW;
    serdesLaneEnableParams.pcieGenType       = CSL_SERDES_PCIE_GEN4;

    serdesLaneEnableParams.laneCtrlRate[laneNum] = CSL_SERDES_LANE_FULL_RATE;
    serdesLaneEnableParams.loopbackMode[laneNum] = CSL_SERDES_LOOPBACK_DISABLED;

    CSL_serdesPorReset(serdesLaneEnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLaneEnableParams.phyType,
                       serdesLaneEnableParams.phyInstanceNum,
                       serdesLaneEnableParams.serdesInstance,
                       laneNum);

    result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                 serdesLaneEnableParams.baseAddr,
                                 serdesLaneEnableParams.refClock,
                                 serdesLaneEnableParams.refClkSrc,
                                 serdesLaneEnableParams.serdesInstance,
                                 serdesLaneEnableParams.phyType);

    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Load the Serdes Config File */
    result = CSL_serdesEthernetInit(&serdesLaneEnableParams);
    /* Return error if input params are invalid */
    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&serdesLaneEnableParams);
    if (0U != laneRetVal)
    {
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

static Board_STATUS Board_serdesCfgEthernetUsxgmii(uint32_t boardID)
{
    CSL_SerdesResult result;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams serdesLaneEnableParams;
    uint32_t laneNum;
    uint32_t laneMask;

    if (BOARD_ID_ENET == boardID)
    {
        laneNum  = BOARD_SERDES_SGMII_ENET1_LANE_NUM;
        laneMask = BOARD_SERDES_SGMII_ENET1_LANE_MASK;
    }
    else
    {
        laneNum  = BOARD_SERDES_SGMII_ENET2_LANE_NUM;
        laneMask = BOARD_SERDES_SGMII_ENET2_LANE_MASK;
    }

    memset(&serdesLaneEnableParams, 0, sizeof(serdesLaneEnableParams));

    /* Serdes-2: Lane 3 (MAC Port 8) */
    serdesLaneEnableParams.serdesInstance    = (CSL_SerdesInstance)BOARD_SERDES_SGMII_INSTANCE;
    serdesLaneEnableParams.baseAddr          = CSL_WIZ16B8M4CT3_2_WIZ16B8M4CT3_BASE;
    serdesLaneEnableParams.refClock          = CSL_SERDES_REF_CLOCK_156p25M;
    serdesLaneEnableParams.refClkSrc         = CSL_SERDES_REF_CLOCK_INT0;
    serdesLaneEnableParams.numLanes          = 1U;
    serdesLaneEnableParams.laneMask          = laneMask;
    serdesLaneEnableParams.SSC_mode          = CSL_SERDES_NO_SSC;
    serdesLaneEnableParams.phyType           = CSL_SERDES_PHY_TYPE_USXGMII;
    serdesLaneEnableParams.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    serdesLaneEnableParams.phyInstanceNum    = BOARD_SERDES_LANE_SELECT_CPSW;

    serdesLaneEnableParams.laneCtrlRate[laneNum] = CSL_SERDES_LANE_FULL_RATE;
    serdesLaneEnableParams.loopbackMode[laneNum] = CSL_SERDES_LOOPBACK_DISABLED;

    serdesLaneEnableParams.pcieGenType       = CSL_SERDES_PCIE_GEN4;
    serdesLaneEnableParams.linkRate          = CSL_SERDES_LINK_RATE_5p15625G;
    /* End: Serdes-2: Lane 3 (MAC Port 8) */

    CSL_serdesPorReset(serdesLaneEnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                       serdesLaneEnableParams.phyType,
                       serdesLaneEnableParams.phyInstanceNum,
                       serdesLaneEnableParams.serdesInstance,
                       laneNum);

    result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                 serdesLaneEnableParams.baseAddr,
                                 serdesLaneEnableParams.refClock,
                                 serdesLaneEnableParams.refClkSrc,
                                 serdesLaneEnableParams.serdesInstance,
                                 serdesLaneEnableParams.phyType);

    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Load the Serdes Config File */
    result = CSL_serdesEthernetInit(&serdesLaneEnableParams);
    /* Return error if input params are invalid */
    if (CSL_SERDES_NO_ERR != result)
    {
        return BOARD_FAIL;
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&serdesLaneEnableParams);
    if (0U != laneRetVal)
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
 * \param   boardID  [IN]  ID of the board to be configured for:
 * \n                      BOARD_ID_ENET(0x2) - ENET Board (ENET-EXP-1)
 * \n                      BOARD_ID_ENET2(0x4) - ENET board (ENET-EXP-2)
 *
 * \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgSgmii(void)
{
    Board_STATUS ret = BOARD_SOK;
    uint32_t boardID;

    boardID = gBoardInitParams.enetBoardID;

    if ((BOARD_ID_ENET  == boardID) ||
        (BOARD_ID_ENET2 == boardID))
    {
        /* Unlock MMR write access */
        Board_serdesKickCtrl(0);

        /* SGMII SERDES initializations */
        ret = Board_CfgSgmii(boardID);
        Board_serdesKickCtrl(1); /* Lock MMR write access */
    }
    else
    {
        ret = BOARD_INVALID_PARAM;
    }

    return ret;
}

/**
 *  \brief serdes configurations for Sierra 1 in QSGMII mode
 *
 *  The function configures the serdes module for QSGMII instances
 *
 * \param   boardID  [IN]  ID of the board to be configured for:
 * \n                      BOARD_ID_ENET(0x2) - ENET Board (ENET-EXP-1)
 * \n                      BOARD_ID_ENET2(0x4) - ENET board (ENET-EXP-2)
 *
 * \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgQsgmii(void)
{
    Board_STATUS ret = BOARD_SOK;
    uint32_t boardID;

    boardID = gBoardInitParams.enetBoardID;

    if ((BOARD_ID_ENET  == boardID) ||
        (BOARD_ID_ENET2 == boardID))
    {
        /* Unlock MMR write access */
        Board_serdesKickCtrl(0);

        /* QSGMII SERDES initializations */
        ret = Board_CfgQsgmii(boardID);
        Board_serdesKickCtrl(1); /* Lock MMR write access */
    }
    else
    {
        ret = BOARD_INVALID_PARAM;
    }

    return ret;
}

/**
 *  \brief serdes configurations for USXGMII mode
 *
 *  The function configures the serdes module for USXGMII interface
 *
 * \param   boardID  [IN]  ID of the board to be configured for:
 * \n                      BOARD_ID_ENET(0x2) - ENET Board (ENET-EXP-1)
 * \n                      BOARD_ID_ENET2(0x4) - ENET board (ENET-EXP-2)
 *
 * \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgUsxgmii(void)
{
    Board_STATUS ret = BOARD_SOK;
    uint32_t boardID;

    boardID = gBoardInitParams.enetBoardID;

    if ((BOARD_ID_ENET  == boardID) ||
        (BOARD_ID_ENET2 == boardID))
    {
        /* Unlock MMR write access */
        Board_serdesKickCtrl(0);
        /* USXGMII SERDES initializations */
        ret = Board_serdesCfgEthernetUsxgmii(boardID);
        Board_serdesKickCtrl(1); /* Lock MMR write access */
    }
    else
    {
        ret = BOARD_INVALID_PARAM;
    }

    return ret;
}

/**
 *  \brief Query SerDes configuration status
 *
 *  The function gets the configuration status of Torrent SerDes module.
 *
 *  \retval TRUE    SerDes1 is configured
 *  \retval FALSE   SerDes1 is not configured
 */
bool Board_serdesCfgStatus(void)
{
    CSL_SerdesStatus serdesStatus;
    bool ret = BFALSE;

    serdesStatus = CSL_serdesConfigStatus(CSL_WIZ16B8M4CT3_2_WIZ16B8M4CT3_BASE);
    if (CSL_SERDES_STATUS_PLL_LOCKED == serdesStatus)
    {
        ret = BTRUE;
    }

    return ret;
}
