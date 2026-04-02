/*
 *  Copyright (c) Texas Instruments Incorporated 2020-2024
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
 */

/*!
 * \file  enet_board_j721e_evm.c
 *
 * \brief This file contains the implementation of J721E EVM support.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/osal/osal.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/include/phy/enetphy.h>
#include <ti/drv/enet/include/phy/dp83867.h>
#include <ti/drv/enet/examples/utils/include/enet_board.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>

#include <ti/board/board.h>
#include <ti/board/src/j721e_evm/include/board_pinmux.h>
#include <ti/board/src/j721e_evm/include/board_utils.h>
#include <ti/board/src/j721e_evm/include/board_control.h>
#include <ti/board/src/j721e_evm/include/board_cfg.h>
#include <ti/board/src/j721e_evm/include/board_ethernet_config.h>
#include <ti/board/src/j721e_evm/include/board_serdes_cfg.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* GESI PHYs connected to CPSW9G MDIO (MDIO_MDC_SEL0 = MDIO_MDC_SEL1 = High) */
#define ENETBOARD_J7XEVM_CPSW9G_MDIO_MUX             ENET_BIT(0U)

/* Initialize QSGMII board.
 *
 * Set GPIOs to release QSGMII PHY out of reset and release COMA_MODE pin. */
#define ENETBOARD_J7XEVM_QPENET_INIT                 ENET_BIT(1U)

/* Enable SERDES Sierra0 clocks (valid for J721E only) */
#define ENETBOARD_J7XEVM_SERDES_CFG                  ENET_BIT(2U)

/* Enable RMII route.
 *
 * I2C GPIO expander 2 (addr 0x22) on CPB board:
 *  - P11 = MCASP/TRACE_MUX_S0 = High
 *  - P12 = MCASP/TRACE_MUX_S1 = Low  */
#define ENETBOARD_J7XEVM_GESI_RMII_MUX               ENET_BIT(3U)

/* VSC8514 wait time between NRESET deassert and access of the SMI interface */
#define ENETBOARD_QSGMII_PHY_TWAIT_USECS             (105000U)

/* SerDes lane number used for ENET QSGMII / Bridge expansion boards in J721E EVM */
#define ENETBOARD_SERDES_LANE_NUM                    (1U)

/* SerDes lane function select: IP1 (0) Enet Switch Q/SGMII Lanes */
#define ENETBOARD_SERDES_LANE_FUNC_SEL_IP1           (0U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* SerDes 'modes' (PHY type + link rate) */
typedef enum EnetBoard_SerdesMode_e
{
    ENETBOARD_SERDES_MODE_NONE,
    ENETBOARD_SERDES_MODE_SGMII,
    ENETBOARD_SERDES_MODE_QSGMII,
    ENETBOARD_SERDES_MODE_XAUI,
} EnetBoard_SerdesMode;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static const EnetBoard_PortCfg *EnetBoard_findPortCfg(Enet_Type enetType,
                                                      uint32_t instId,
                                                      const EnetBoard_EthPort *ethPort,
                                                      const EnetBoard_PortCfg *ethPortCfgs,
                                                      uint32_t numEthPorts);

static EnetBoard_SerdesMode EnetBoard_getSerdesMode(uint32_t boardId);

static void EnetBoard_configSerdesClks(void);

static void EnetBoard_disableSerdesClks(void);

static Board_STATUS EnetBoard_serdesCfg(EnetBoard_SerdesMode serdesMode);

static uint32_t EnetBoard_getMacAddrListEeprom(uint8_t macAddr[][ENET_MAC_ADDR_LEN],
                                               uint32_t maxMacEntries);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

extern Board_initParams_t gBoardInitParams;

uint32_t gEnetBoardUartBaseAddr[BOARD_SOC_DOMAIN_MAX][CSL_UART_MAIN_CNT] =
    {{CSL_UART0_BASE, CSL_UART1_BASE, CSL_UART2_BASE, CSL_UART3_BASE, CSL_UART4_BASE,
      CSL_UART5_BASE, CSL_UART6_BASE, CSL_UART7_BASE, CSL_UART8_BASE, CSL_UART9_BASE},
     {CSL_WKUP_UART0_BASE, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {CSL_MCU_UART0_BASE, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

/*!
 * \brief Common Processor Board (CPB) board's DP83867 PHY configuration.
 */
static const Dp83867_Cfg gEnetCpbBoard_dp83867PhyCfg =
{
    .txClkShiftEn         = BTRUE,
    .rxClkShiftEn         = BTRUE,
    .txDelayInPs          = 2000U,  /* 2.00 ns */
    .rxDelayInPs          = 2000U,  /* 2.00 ns */
    .txFifoDepth          = 4U,
    .idleCntThresh        = 4U,     /* Improves short cable performance */
    .impedanceInMilliOhms = 35000,  /* 35 ohms */
    .gpio0Mode            = DP83867_GPIO0_LED3,
    .gpio1Mode            = DP83867_GPIO1_COL, /* Unused */
    .ledMode              =
    {
        DP83867_LED_LINKED,         /* Unused */
        DP83867_LED_LINKED_100BTX,
        DP83867_LED_RXTXACT,
        DP83867_LED_LINKED_1000BT,
    },
};

/*
 * GESI board's DP83867 PHY configuration.
 */
static const Dp83867_Cfg gEnetGesiBoard_dp83867PhyCfg =
{
    .txClkShiftEn         = BTRUE,
    .rxClkShiftEn         = BTRUE,
    .txDelayInPs          = 2750U,  /* 2.75 ns */
    .rxDelayInPs          = 2500U,  /* 2.50 ns */
    .txFifoDepth          = 4U,
    .idleCntThresh        = 4U,     /* Improves short cable performance */
    .impedanceInMilliOhms = 35000,  /* 35 ohms */
    .gpio0Mode            = DP83867_GPIO0_LED3,
    .gpio1Mode            = DP83867_GPIO1_COL, /* Unused */
    .ledMode              =
    {
        DP83867_LED_LINKED,         /* Unused */
        DP83867_LED_LINKED_100BTX,
        DP83867_LED_RXTXACT,
        DP83867_LED_LINKED_1000BT,
    },
};

/*
 * J721E CPB board configuration.
 *
 * 1 x RGMII PHY connected to J721E CPSW_2G MAC port.
 */
static const EnetBoard_PortCfg gEnetCpbBoard_j7xEthPort[] =
{
    {    /* "MCU_ENET" */
        .enetType  = ENET_CPSW_2G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_1,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg    =
        {
            .phyAddr         = 0U,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = &gEnetCpbBoard_dp83867PhyCfg,
            .extendedCfgSize = sizeof(gEnetCpbBoard_dp83867PhyCfg),
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_INVALID,
        .linkCfg   = { ENET_SPEED_AUTO, ENET_DUPLEX_AUTO },
        .flags     = 0U,
    },
};

/*
 * J721E GESI board configuration.
 *
 * 4 x RGMII PHYs and 1 x RMII are connected to J721E CPSW_9G MAC ports.
 */
static const EnetBoard_PortCfg gEnetGesiBoard_j721eEthPort[] =
{
    {    /* "PRG1_RGMII1_B" */
        .enetType = ENET_CPSW_9G,
        .instId   = 0U,
        .macPort  = ENET_MAC_PORT_1,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg   =
        {
            .phyAddr         = 12U,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = &gEnetGesiBoard_dp83867PhyCfg,
            .extendedCfgSize = sizeof(gEnetGesiBoard_dp83867PhyCfg),
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_INVALID,
        .linkCfg   = { ENET_SPEED_AUTO, ENET_DUPLEX_AUTO },
        .flags    = ENETBOARD_J7XEVM_CPSW9G_MDIO_MUX,
    },
    {    /* "PRG1_RGMII2_T" */
        .enetType = ENET_CPSW_9G,
        .instId   = 0U,
        .macPort  = ENET_MAC_PORT_8,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg   =
        {
            .phyAddr         = 15U,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = &gEnetGesiBoard_dp83867PhyCfg,
            .extendedCfgSize = sizeof(gEnetGesiBoard_dp83867PhyCfg),
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_INVALID,
        .linkCfg   = { ENET_SPEED_AUTO, ENET_DUPLEX_AUTO },
        .flags    = ENETBOARD_J7XEVM_CPSW9G_MDIO_MUX,
    },
    {   /* "PRG0_RGMII1_B" */
        .enetType = ENET_CPSW_9G,
        .instId   = 0U,
        .macPort  = ENET_MAC_PORT_3,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg   =
        {
            .phyAddr         = 0U,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = &gEnetGesiBoard_dp83867PhyCfg,
            .extendedCfgSize = sizeof(gEnetGesiBoard_dp83867PhyCfg),
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_INVALID,
        .linkCfg   = { ENET_SPEED_AUTO, ENET_DUPLEX_AUTO },
        .flags    = ENETBOARD_J7XEVM_CPSW9G_MDIO_MUX,
    },
    {   /* "PRG0_RGMII02_T" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_4,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg    =
        {
            .phyAddr         = 3U,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = &gEnetGesiBoard_dp83867PhyCfg,
            .extendedCfgSize = sizeof(gEnetGesiBoard_dp83867PhyCfg),
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_INVALID,
        .linkCfg   = { ENET_SPEED_AUTO, ENET_DUPLEX_AUTO },
        .flags     = ENETBOARD_J7XEVM_CPSW9G_MDIO_MUX,
    },
    {    /* "RMII8" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_8,
        .mii       = { ENET_MAC_LAYER_MII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg    =
        {
            .phyAddr         = 23U,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = NULL,
            .extendedCfgSize = 0U,
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_INVALID,
        .linkCfg   = { ENET_SPEED_AUTO, ENET_DUPLEX_AUTO },
        .flags     = (ENETBOARD_J7XEVM_CPSW9G_MDIO_MUX |
                      ENETBOARD_J7XEVM_GESI_RMII_MUX),
    },
};

/*
 * J721E QSGMII board (QpENet) configuration.
 *
 * All 4 x QSGMII ports are connected to J721E CPSW_9G MAC ports.
 */
static const EnetBoard_PortCfg gEnetQpENetBoard_j721eEthPort[] =
{
    {    /* "P0" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_2,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_MAIN },
        .phyCfg    =
        {
            .phyAddr         = 16U,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = NULL,
            .extendedCfgSize = 0U,
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_WITH_PHY,
        .linkCfg   = { ENET_SPEED_AUTO, ENET_DUPLEX_AUTO },
        .flags     = (ENETBOARD_J7XEVM_QPENET_INIT |
                      ENETBOARD_J7XEVM_SERDES_CFG),
    },
    {    /* "P1" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_5,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
        .phyCfg    =
        {
            .phyAddr         = 17U,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = NULL,
            .extendedCfgSize = 0U,
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_WITH_PHY,
        .linkCfg   = { ENET_SPEED_AUTO, ENET_DUPLEX_AUTO },
        .flags     = (ENETBOARD_J7XEVM_QPENET_INIT |
                      ENETBOARD_J7XEVM_SERDES_CFG),
    },
    {    /* "P2" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_6,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
        .phyCfg    =
        {
            .phyAddr         = 18U,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = NULL,
            .extendedCfgSize = 0U,
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_WITH_PHY,
        .linkCfg   = { ENET_SPEED_AUTO, ENET_DUPLEX_AUTO },
        .flags     = (ENETBOARD_J7XEVM_QPENET_INIT |
                      ENETBOARD_J7XEVM_SERDES_CFG),
    },
    {    /* "P3" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_7,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
        .phyCfg    =
        {
            .phyAddr         = 19U,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = NULL,
            .extendedCfgSize = 0U,
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_WITH_PHY,
        .linkCfg   = { ENET_SPEED_AUTO, ENET_DUPLEX_AUTO },
        .flags     = (ENETBOARD_J7XEVM_QPENET_INIT |
                      ENETBOARD_J7XEVM_SERDES_CFG),
    },
};

/*
 * J721E SGMII board configuration.
 *
 * 1 x SGMII port connected to J721E CPSW_9G MAC port.
 */
static const EnetBoard_PortCfg gEnetSgmiiBoard_j721eEthPort[] =
{
    {
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_2,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_SERIAL },
        .phyCfg    =
        {
            .phyAddr         = 10U,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = NULL,
            .extendedCfgSize = 0U,
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_FORCEDLINK,
        .linkCfg   = { ENET_SPEED_1GBIT, ENET_DUPLEX_FULL },
        .flags     = (ENETBOARD_J7XEVM_QPENET_INIT |
                      ENETBOARD_J7XEVM_SERDES_CFG),
    },
};

/* 1 x SGMII port in MAC-to-MAC mode using (SGMII) ENET bridge expansion board */
static const EnetBoard_PortCfg gEnetSgmiiBridgeBoard_j721eEthPort[] =
{
    {
        .enetType   = ENET_CPSW_9G,
        .instId     = 0U,
        .macPort    = ENET_MAC_PORT_2,
        .mii        = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_SERIAL },
        .phyCfg     =
        {
            .phyAddr         = ENETPHY_INVALID_PHYADDR,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = NULL,
            .extendedCfgSize = 0U,
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_FORCEDLINK,
        .linkCfg   = { ENET_SPEED_1GBIT, ENET_DUPLEX_FULL },
        .flags     = ENETBOARD_J7XEVM_SERDES_CFG,
    },
};

/* 1 x XAUI port in MAC-to-MAC mode using (SGMII) ENET bridge expansion board */
static const EnetBoard_PortCfg gEnetXauiBridgeBoard_j721eEthPort[] =
{
    {
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_2,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_SERIAL },
        .phyCfg    =
        {
            .phyAddr         = ENETPHY_INVALID_PHYADDR,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = NULL,
            .extendedCfgSize = 0U,
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_FORCEDLINK,
        .linkCfg   = { ENET_SPEED_1GBIT, ENET_DUPLEX_FULL },
        .flags     = ENETBOARD_J7XEVM_SERDES_CFG,
    },
};

/*
 * J721E dummy board used for MAC loopback setup.
 */
static const EnetBoard_PortCfg gEnetLpbkBoard_j721eEthPort[] =
{
    {    /* CPSW_2G RGMII MAC loopback */
        .enetType  = ENET_CPSW_2G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_1,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg    = { .phyAddr = ENETPHY_INVALID_PHYADDR, },
        .sgmiiMode = ENET_MAC_SGMIIMODE_INVALID,
        .linkCfg   = { ENET_SPEED_1GBIT, ENET_DUPLEX_FULL },
        .flags     = 0U,
    },
    {    /* CPSW_2G RMII MAC loopback */
        .enetType  = ENET_CPSW_2G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_1,
        .mii       = { ENET_MAC_LAYER_MII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg    = { .phyAddr = ENETPHY_INVALID_PHYADDR, },
        .sgmiiMode = ENET_MAC_SGMIIMODE_INVALID,
        .linkCfg   = { ENET_SPEED_100MBIT, ENET_DUPLEX_FULL },
        .flags     = 0U,
    },
    {    /* CPSW_9G RGMII MAC loopback */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_1,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg    = { .phyAddr = ENETPHY_INVALID_PHYADDR, },
        .sgmiiMode = ENET_MAC_SGMIIMODE_INVALID,
        .linkCfg   = { ENET_SPEED_1GBIT, ENET_DUPLEX_FULL },
        .flags     = 0U,
    },
    {    /* CPSW_9G RGMII MAC loopback */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_1,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_SERIAL },
        .phyCfg    = { .phyAddr = ENETPHY_INVALID_PHYADDR, },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_FORCEDLINK,
        .linkCfg   = { ENET_SPEED_1GBIT, ENET_DUPLEX_FULL },
        .flags     = (ENETBOARD_J7XEVM_SERDES_CFG),
    },
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void EnetBoard_enetExpCfg(void)
{
    Board_STATUS boardStatus;

    /* Release PHY reset. Note this is needed for both SGMII and QSGMII boards */
    boardStatus = Board_cpswEnetExpPhyReset(0U);
    EnetAppUtils_assert(BOARD_SOK == boardStatus);

    if (Board_detectBoard(BOARD_ID_ENET))
    {
        /* Release the COMA_MODE pin */
        boardStatus = Board_cpswEnetExpComaModeCfg(0U);
        EnetAppUtils_assert(BOARD_SOK == boardStatus);
    }

    /* Wait till we can access QSGMII PHY registers after reset, irrespective
     * of the NRESET gpio set by ETHFW or bootloader */
    EnetAppUtils_delayInUsec(ENETBOARD_QSGMII_PHY_TWAIT_USECS);
}

const EnetBoard_PortCfg *EnetBoard_getPortCfg(Enet_Type enetType,
                                              uint32_t instId,
                                              const EnetBoard_EthPort *ethPort)
{
    const EnetBoard_PortCfg *portCfg = NULL;

    if (ethPort->boardId == ENETBOARD_CPB_ID)
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetCpbBoard_j7xEthPort,
                                        ENETPHY_ARRAYSIZE(gEnetCpbBoard_j7xEthPort));
    }

    if ((portCfg == NULL) &&
        (ethPort->boardId == ENETBOARD_GESI_ID) &&
        (ethPort->expPort == ENETBOARD_EXP_PORT_GESI))
    {
        if (enetType == ENET_CPSW_9G)
        {
            portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                            gEnetGesiBoard_j721eEthPort,
                                            ENETPHY_ARRAYSIZE(gEnetGesiBoard_j721eEthPort));
        }
    }

    if ((portCfg == NULL) &&
        (ethPort->boardId == ENETBOARD_QPENET_ID) &&
        (ethPort->expPort == ENETBOARD_EXP_PORT_ENET))
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetQpENetBoard_j721eEthPort,
                                        ENETPHY_ARRAYSIZE(gEnetQpENetBoard_j721eEthPort));
    }

    if ((portCfg == NULL) &&
        (ethPort->boardId == ENETBOARD_SGMII_ID) &&
        (ethPort->expPort == ENETBOARD_EXP_PORT_ENET))
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetSgmiiBoard_j721eEthPort,
                                        ENETPHY_ARRAYSIZE(gEnetSgmiiBoard_j721eEthPort));
    }

    if ((portCfg == NULL) &&
        (ethPort->boardId == ENETBOARD_BRIDGE_SGMII_ID) &&
        (ethPort->expPort == ENETBOARD_EXP_PORT_ENET))
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetSgmiiBridgeBoard_j721eEthPort,
                                        ENETPHY_ARRAYSIZE(gEnetSgmiiBridgeBoard_j721eEthPort));
    }

    if ((portCfg == NULL) &&
        (ethPort->boardId == ENETBOARD_BRIDGE_XAUI_ID) &&
        (ethPort->expPort == ENETBOARD_EXP_PORT_ENET))
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetXauiBridgeBoard_j721eEthPort,
                                        ENETPHY_ARRAYSIZE(gEnetXauiBridgeBoard_j721eEthPort));
    }

    if ((portCfg == NULL) &&
        ENET_NOT_ZERO(ethPort->boardId & ENETBOARD_LOOPBACK_ID))
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetLpbkBoard_j721eEthPort,
                                        ENETPHY_ARRAYSIZE(gEnetLpbkBoard_j721eEthPort));
    }

    return portCfg;
}

static const EnetBoard_PortCfg *EnetBoard_findPortCfg(Enet_Type enetType,
                                                      uint32_t instId,
                                                      const EnetBoard_EthPort *ethPort,
                                                      const EnetBoard_PortCfg *ethPortCfgs,
                                                      uint32_t numEthPorts)
{
    const EnetBoard_PortCfg *ethPortCfg = NULL;
    bool found = BFALSE;
    uint32_t i;

    for (i = 0U; i < numEthPorts; i++)
    {
        ethPortCfg = &ethPortCfgs[i];

        if ((ethPortCfg->enetType == enetType) &&
            (ethPortCfg->instId == instId) &&
            (ethPortCfg->macPort == ethPort->macPort) &&
            (ethPortCfg->mii.layerType == ethPort->mii.layerType) &&
            (ethPortCfg->mii.sublayerType == ethPort->mii.sublayerType))
        {
            found = BTRUE;
            break;
        }
    }

    return found ? ethPortCfg : NULL;
}

/**
  *  \brief   Returns base address of given UART instance
  *
  *  \param   instNum [IN] UART instance
  *
  *  \param   domain  [IN] Domain of UART controller
  *                           BOARD_SOC_DOMAIN_MAIN - Main Domain
  *                           BOARD_SOC_DOMAIN_WKUP - Wakeup domain
  *                           BOARD_SOC_DOMAIN_MCU - MCU domain
  *
  *  \return  Valid base address in case of success or 0
  *
  */
static uint32_t EnetBoard_getUartBaseAddr(uint8_t instNum,
                                          uint8_t domain)
{
    uint32_t baseAddr = 0;

    if((instNum < CSL_UART_MAIN_CNT) &&
       (domain <= BOARD_SOC_DOMAIN_MCU))
    {
        baseAddr = gEnetBoardUartBaseAddr[domain][instNum];
    }

    return (baseAddr);
}


/**
 *  \brief   This function initializes the default UART instance for use for
 *           console operations.
 *
 *  \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS EnetBoard_uartStdioInit(void)
{
    Board_STATUS status;
    Board_IoExpCfg_t ioExpCfg;
    UART_HwAttrs uart_cfg;
    uint32_t uartInst;
    uint32_t uartBaseAddr;
    uint32_t socDomainUART;
    uint32_t socDomainCore;

    uartInst      = gBoardInitParams.uartInst;
    socDomainUART = gBoardInitParams.uartSocDomain;

    socDomainCore = Board_getSocDomain();

    /* Disable the UART interrupt */
    UART_socGetInitCfg(uartInst, &uart_cfg);

    if(socDomainUART != socDomainCore)
    {
        uartBaseAddr = EnetBoard_getUartBaseAddr(uartInst, socDomainUART);
        if(uartBaseAddr != 0)
        {
            uart_cfg.baseAddr = uartBaseAddr;
        }
        else
        {
            return BOARD_INVALID_PARAM;
        }

        if(socDomainUART == BOARD_SOC_DOMAIN_MAIN)
        {
            uart_cfg.frequency = BOARD_UART_CLK_MAIN;
        }
        else
        {
            uart_cfg.frequency = BOARD_UART_CLK_WKUP;
        }
    }

    uart_cfg.enableInterrupt = true;
    UART_socSetInitCfg(uartInst, &uart_cfg);

    /* MAIN UART4 instance is connected through mux which is controlled by
           IO expander. Need to configure the mux to use UART4 instance */
    if(uartInst == 4)
    {
        ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR;
        ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
        ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
        ioExpCfg.enableIntr  = false;
        ioExpCfg.ioExpType   = THREE_PORT_IOEXP;
        ioExpCfg.portNum     = PORTNUM_1;
        ioExpCfg.pinNum      = PIN_NUM_1;
        ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;

        status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, (void *)&ioExpCfg);
        if(status != BOARD_SOK)
        {
            return status;
        }

        ioExpCfg.pinNum      = PIN_NUM_2;
        ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

        status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, (void *)&ioExpCfg);
        if(status != BOARD_SOK)
        {
            return status;
        }
    }

    UART_stdioInit(uartInst);

    return BOARD_SOK;
}

int32_t EnetBoard_init(void)
{
    Board_initCfg boardCfg;
    Board_STATUS boardStatus;

    EnetAppUtils_setupSciServer();

    /* Board configuration parameters. Further configuration done explicitly (i.e. ENETCTRL) */
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK;
    boardStatus = Board_init(boardCfg);

    if (BOARD_SOK == boardStatus)
    {
        boardStatus = EnetBoard_uartStdioInit();
    }

    EnetAppUtils_assert(boardStatus == BOARD_SOK);

    return (boardStatus == BOARD_SOK) ? ENET_SOK : ENET_EFAIL;
}

void EnetBoard_deinit(void)
{
    Board_initCfg boardCfg;
    Board_STATUS boardStatus;
    int32_t enetCard = Board_detectEnetCard();

    if ( (enetCard != BOARD_ENET_QSGMII) &&
         (enetCard != BOARD_ENET_SGMII))
    {
        EnetBoard_disableSerdesClks();
    }

    //TODO should Enet control be reseted as part of deinit
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_UART_STDIO;
    boardStatus = Board_deinit(boardCfg);
    EnetAppUtils_assert(boardStatus == BOARD_SOK);
}

int32_t EnetBoard_setupPorts(Enet_Type enetType,
                             uint32_t instId,
                             EnetBoard_EthPort *ethPorts,
                             uint32_t numEthPorts)
{
    EnetBoard_SerdesMode serdesMode;
    Board_STATUS boardStatus;
    Board_PinmuxConfig_t pinmuxCfg;
    bool gesiDetected = BFALSE;
    int32_t enetCard;
    uint32_t boardEnet = 0U;
    uint32_t hits = 0U;
    uint32_t flags = 0U;
    uint32_t i;
    int32_t status = ENET_SOK;

    for (i = 0U; i < numEthPorts; i++)
    {
        EnetBoard_EthPort *ethPort = &ethPorts[i];
        const EnetBoard_PortCfg *portCfg;

        portCfg = EnetBoard_getPortCfg(enetType, instId, ethPort);
        if (portCfg != NULL)
        {
            EnetBoard_setEnetControl(enetType, instId, ethPort->macPort, &ethPort->mii);
            flags |= portCfg->flags;

            if (ethPort->expPort == ENETBOARD_EXP_PORT_ENET)
            {
                boardEnet = ethPort->boardId;
            }

            hits++;
        }
    }

    EnetAppUtils_print("EnetBoard_setupPorts: %u of %u ports configurations found\n", hits, numEthPorts);

    /* Set pinmux for CPSW9G */
    Board_pinmuxGetCfg(&pinmuxCfg);
    pinmuxCfg.gesiExp = BOARD_PINMUX_GESI_CPSW;
    Board_pinmuxSetCfg(&pinmuxCfg);

    boardStatus = Board_init(BOARD_INIT_PINMUX_CONFIG);
    EnetAppUtils_assert(boardStatus == BOARD_SOK);

    /* Check if GESI board is detected if any GESI related configuration is required */
    if (ENET_NOT_ZERO(flags & ENETBOARD_J7XEVM_GESI_RMII_MUX) ||
        ENET_NOT_ZERO(flags & ENETBOARD_J7XEVM_CPSW9G_MDIO_MUX))
    {
        gesiDetected = Board_detectBoard(BOARD_ID_GESI);
        if (!gesiDetected)
        {
            EnetAppUtils_print("EnetBoard_setupPorts: GESI board not detected\n");
            status = ENET_EFAIL;
        }
    }

    /* RMII route in GESI board */
    if ((status == ENET_SOK) &&
        ENET_NOT_ZERO(flags & ENETBOARD_J7XEVM_GESI_RMII_MUX))
    {
        boardStatus = Board_control(BOARD_CTRL_CMD_SET_RMII_DATA_MUX, NULL);
        EnetAppUtils_assert(boardStatus == BOARD_SOK);
    }

    /* GESI PHYs connected to CPSW9G MDIO */
    if ((status == ENET_SOK) &&
        ENET_NOT_ZERO(flags & ENETBOARD_J7XEVM_CPSW9G_MDIO_MUX))
    {
        boardStatus = Board_control(BOARD_CTRL_CMD_SET_GESI_CPSW_MDIO_MUX, NULL);
        EnetAppUtils_assert(boardStatus == BOARD_SOK);
    }

    /* Check if QpENet board is detected if any QSMII related configuration is required */ 
    if ((status == ENET_SOK) &&
        (ENET_NOT_ZERO(flags & ENETBOARD_J7XEVM_QPENET_INIT)))
    {
        enetCard = Board_detectEnetCard();
        if ((enetCard == BOARD_ENET_QSGMII) ||
            (enetCard == BOARD_ENET_SGMII))
        {
            /* QpENet QSGMII PHY init */
            EnetBoard_enetExpCfg();
        }
        else
        {
            EnetAppUtils_print("EnetBoard_setupPorts: QpENet (QSGMII) or SGMII board not detected\n");
            status = ENET_EFAIL;
        }
    }

    /* SERDES configuration and clocks setup */
    if ((status == ENET_SOK) &&
        ENET_NOT_ZERO(flags & ENETBOARD_J7XEVM_SERDES_CFG))
    {
        serdesMode = EnetBoard_getSerdesMode(boardEnet);

        EnetBoard_configSerdesClks();

        boardStatus = EnetBoard_serdesCfg(serdesMode);
        EnetAppUtils_assert(boardStatus == BOARD_SOK);
    }

    return status;
}

static EnetBoard_SerdesMode EnetBoard_getSerdesMode(uint32_t boardId)
{
    EnetBoard_SerdesMode serdesMode;

    switch (boardId)
    {
        case ENETBOARD_SGMII_ID:
        case ENETBOARD_BRIDGE_SGMII_ID:
            serdesMode = ENETBOARD_SERDES_MODE_SGMII;
            break;
        case ENETBOARD_QPENET_ID:
            serdesMode = ENETBOARD_SERDES_MODE_QSGMII;
            break;
        case ENETBOARD_BRIDGE_XAUI_ID:
            serdesMode = ENETBOARD_SERDES_MODE_XAUI;
            break;
        case ENETBOARD_LOOPBACK_ID:
            serdesMode = ENETBOARD_SERDES_MODE_SGMII;
            break;
        default:
            serdesMode = ENETBOARD_SERDES_MODE_NONE;
            break;
    }

    return serdesMode;
}

static void EnetBoard_configSerdesClks(void)
{
    uint32_t moduleId, clkId, clkRateHz;

    /* Configure the required PLLs for SERDES0 */
    moduleId  = TISCI_DEV_SERDES_16G0;
    clkId     = TISCI_DEV_SERDES_16G0_CORE_REF1_CLK;
    clkRateHz = 100000000U;
    EnetAppUtils_clkRateSet(moduleId, clkId, clkRateHz);

    clkId     = TISCI_DEV_SERDES_16G0_CORE_REF_CLK;
    clkRateHz = 100000000U;
    EnetAppUtils_clkRateSet(moduleId, clkId, clkRateHz);
    EnetAppUtils_setDeviceState(moduleId, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON, 0U);
}

static void EnetBoard_disableSerdesClks(void)
{
    uint32_t moduleId;

    /* Set module set to HW AUTO */
    moduleId = TISCI_DEV_SERDES_16G0;
    EnetAppUtils_setDeviceState(moduleId, TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF, 0U);
}

static Board_STATUS EnetBoard_serdesCfg(EnetBoard_SerdesMode serdesMode)
{
    CSL_SerdesLaneEnableStatus laneStatus = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams lanePrms;
    CSL_SerdesResult result;
    CSL_SerdesStatus status = BOARD_SOK;

    memset(&lanePrms, 0, sizeof(lanePrms));

    lanePrms.serdesInstance    = (CSL_SerdesInstance)CSL_SIERRA_SERDES0;
    lanePrms.baseAddr          = CSL_SERDES_16G0_BASE;
    lanePrms.refClock          = CSL_SERDES_REF_CLOCK_100M;
    lanePrms.refClkSrc         = CSL_SERDES_REF_CLOCK_INT;
    lanePrms.numLanes          = 1U;
    lanePrms.laneMask          = ENET_BIT(ENETBOARD_SERDES_LANE_NUM);
    lanePrms.SSC_mode          = CSL_SERDES_NO_SSC;
    lanePrms.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    lanePrms.phyInstanceNum    = ENETBOARD_SERDES_LANE_FUNC_SEL_IP1;

    lanePrms.laneCtrlRate[ENETBOARD_SERDES_LANE_NUM] = CSL_SERDES_LANE_FULL_RATE;
    lanePrms.loopbackMode[ENETBOARD_SERDES_LANE_NUM] = CSL_SERDES_LOOPBACK_DISABLED;

    switch (serdesMode)
    {
        case ENETBOARD_SERDES_MODE_SGMII:
            EnetAppUtils_print("Configuring SerDes lane %u for SGMII mode\n", ENETBOARD_SERDES_LANE_NUM);
            lanePrms.phyType       = CSL_SERDES_PHY_TYPE_SGMII;
            lanePrms.linkRate      = CSL_SERDES_LINK_RATE_1p25G;
            break;

        case ENETBOARD_SERDES_MODE_QSGMII:
            EnetAppUtils_print("Configuring SerDes lane %u for QSGMII\n", ENETBOARD_SERDES_LANE_NUM);
            lanePrms.phyType       = CSL_SERDES_PHY_TYPE_QSGMII;
            lanePrms.linkRate      = CSL_SERDES_LINK_RATE_5G;
            break;

        case ENETBOARD_SERDES_MODE_XAUI:
            EnetAppUtils_print("Configuring SerDes lane %u for XAUI mode\n", ENETBOARD_SERDES_LANE_NUM);
            lanePrms.phyType       = CSL_SERDES_PHY_TYPE_XAUI;
            lanePrms.linkRate      = CSL_SERDES_LINK_RATE_3p125G;
            break;

        default:
            EnetAppUtils_print("Invalid SerDes mode %d\n", serdesMode);
            status = BOARD_FAIL;
            break;
    }

    if (status == BOARD_SOK)
    {
        CSL_serdesPorReset(lanePrms.baseAddr);
    }

    /* Select the IP type, IP instance num, Serdes lane number */
    if (status == BOARD_SOK)
    {
        CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                           lanePrms.phyType,
                           lanePrms.phyInstanceNum,
                           lanePrms.serdesInstance,
                           ENETBOARD_SERDES_LANE_NUM);
    }

    /* Configure ref clock */
    if (status == BOARD_SOK)
    {
        result = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                     lanePrms.baseAddr,
                                     lanePrms.refClock,
                                     lanePrms.refClkSrc,
                                     lanePrms.serdesInstance,
                                     lanePrms.phyType);
        if (result != CSL_SERDES_NO_ERR)
        {
            EnetAppUtils_print("Failed to set SerDes ref_clk sel: %d\n", result);
            status = BOARD_FAIL;
        }
    }

    /* Assert PHY reset and disable all lanes */
    if (status == BOARD_SOK)
    {
        CSL_serdesDisablePllAndLanes(lanePrms.baseAddr,
                                     lanePrms.numLanes,
                                     lanePrms.laneMask);
    }

    /* Load the Serdes Config File */
    if (status == BOARD_SOK)
    {
        result = CSL_serdesEthernetInit(&lanePrms);
        if (result != CSL_SERDES_NO_ERR)
        {
            EnetAppUtils_print("Failed to initialize SerDes Ethernet: %d\n", result);
            status = BOARD_FAIL;
        }
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    if (status == BOARD_SOK)
    {
        laneStatus = CSL_serdesLaneEnable(&lanePrms);
        if (laneStatus != 0)
        {
            EnetAppUtils_print("Failed to enable SerDes lane: %d\n", laneStatus);
            status = BOARD_FAIL;
        }
    }

    return status;
}

void EnetBoard_setEnetControl(Enet_Type enetType,
                              uint32_t instId,
                              Enet_MacPort macPort,
                              EnetMacPort_Interface *mii)
{
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t modeSel = 0U;
    int32_t status = BOARD_FAIL;

    if (EnetMacPort_isRmii(mii))
    {
        modeSel = RMII;
    }
    else if (EnetMacPort_isRgmii(mii))
    {
        modeSel = RGMII;
    }
    else if (EnetMacPort_isSgmii(mii))
    {
        modeSel = SGMII;
    }
    else if (EnetMacPort_isQsgmii(mii))
    {
        modeSel = (mii->sublayerType == ENET_MAC_SUBLAYER_QUAD_SERIAL_MAIN) ? QSGMII : QSGMII_SUB;
    }
    else
    {
        EnetAppUtils_print("Invalid MII type: layer %u suyblayer %u\n", mii->layerType, mii->sublayerType);
        EnetAppUtils_assert(BFALSE);
    }

    switch (enetType)
    {
        case ENET_CPSW_2G:
            Board_unlockMMR();
            status = Board_cpsw2gMacModeConfig(modeSel);
            break;
        case ENET_CPSW_9G:
            status = Board_cpsw9gEthConfig(portNum, modeSel);
            break;
        default:
            break;
    }

    EnetAppUtils_assert(status == BOARD_SOK);
}

uint32_t EnetBoard_getMacAddrList(Enet_Type enetType,
                                  uint32_t instId,
                                  uint8_t macAddr[][ENET_MAC_ADDR_LEN],
                                  uint32_t maxMacEntries)
{
    uint32_t numMacAddrs = maxMacEntries;

    if (maxMacEntries < 1U)
    {
        EnetAppUtils_print("Invalid number of requested MAC addresses\n");
        EnetAppUtils_assert(BFALSE);
    }

    switch (enetType)
    {
        case ENET_CPSW_2G:
            EnetSoc_getEFusedMacAddrs(macAddr, &numMacAddrs);
            break;

        case ENET_CPSW_9G:
            numMacAddrs = EnetBoard_getMacAddrListEeprom(macAddr, maxMacEntries);
            break;

        default:
            EnetAppUtils_print("Invalid peripheral type or inst\n");
            EnetAppUtils_assert(BFALSE);
            break;
    }

    return numMacAddrs;
}

static uint32_t EnetBoard_getMacAddrListEeprom(uint8_t macAddr[][ENET_MAC_ADDR_LEN],
                                               uint32_t maxMacEntries)
{
    uint8_t macAddrBuf[ENET_RM_NUM_MACADDR_MAX * BOARD_MAC_ADDR_BYTES];
    Board_STATUS boardStatus;
    uint32_t macAddrCnt, tempCnt;
    uint32_t allocMacEntries = 0;
    uint32_t i, j;

    if (Board_detectBoard(BOARD_ID_GESI))
    {
        /* Read number of MAC addresses in GESI board */
        boardStatus = Board_readMacAddrCount(BOARD_ID_GESI, &macAddrCnt);
        EnetAppUtils_assert(boardStatus == BOARD_SOK);
        EnetAppUtils_assert(macAddrCnt <= ENET_RM_NUM_MACADDR_MAX);

        /* Read MAC addresses */
        boardStatus = Board_readMacAddr(BOARD_ID_GESI,
                                        macAddrBuf,
                                        sizeof(macAddrBuf),
                                        &tempCnt);
        EnetAppUtils_assert(boardStatus == BOARD_SOK);
        EnetAppUtils_assert(tempCnt == macAddrCnt);

        /* Save only those required to meet the max number of MAC entries */
        macAddrCnt = EnetUtils_min(macAddrCnt, maxMacEntries);
        for (i = 0U; i < macAddrCnt; i++)
        {
            ENET_UTILS_COMPILETIME_ASSERT(ENET_MAC_ADDR_LEN == BOARD_MAC_ADDR_BYTES);
            memcpy(macAddr[i], &macAddrBuf[i * BOARD_MAC_ADDR_BYTES], ENET_MAC_ADDR_LEN);
        }

        allocMacEntries = macAddrCnt;
    }

    if (Board_detectBoard(BOARD_ID_ENET))
    {
        /* Read number of MAC addresses in QUAD Eth board */
        boardStatus = Board_readMacAddrCount(BOARD_ID_ENET, &macAddrCnt);
        EnetAppUtils_assert(boardStatus == BOARD_SOK);
        EnetAppUtils_assert(macAddrCnt <= ENET_RM_NUM_MACADDR_MAX);

        /* Read MAC addresses */
        boardStatus = Board_readMacAddr(BOARD_ID_ENET,
                                        macAddrBuf,
                                        sizeof(macAddrBuf),
                                        &tempCnt);
        EnetAppUtils_assert(boardStatus == BOARD_SOK);
        EnetAppUtils_assert(tempCnt == macAddrCnt);

        /* Save only those required to meet the max number of MAC entries */
        macAddrCnt = EnetUtils_min(macAddrCnt, maxMacEntries - allocMacEntries);
        for (i = 0U, j = allocMacEntries; i < macAddrCnt; i++, j++)
        {
            ENET_UTILS_COMPILETIME_ASSERT(ENET_MAC_ADDR_LEN == BOARD_MAC_ADDR_BYTES);
            memcpy(macAddr[j], &macAddrBuf[i * BOARD_MAC_ADDR_BYTES], ENET_MAC_ADDR_LEN);
        }

        allocMacEntries += macAddrCnt;
    }

    if (allocMacEntries == 0U)
    {
        EnetAppUtils_print("EnetBoard_getMacAddrList Failed - GESI/ENET board not present\n");
        EnetAppUtils_assert(BFALSE);
    }

    return allocMacEntries;
}
