/*
 *  Copyright (c) Texas Instruments Incorporated 2022-2024
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
 * \file  enet_board_j742s2_evm.c
 *
 * \brief This file contains the implementation of J742S2 EVM support.
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
#include <ti/board/src/j742s2_evm/include/board_pinmux.h>
#include <ti/board/src/j742s2_evm/include/board_utils.h>
#include <ti/board/src/j742s2_evm/include/board_control.h>
#include <ti/board/src/j742s2_evm/include/board_cfg.h>
#include <ti/board/src/j742s2_evm/include/board_ethernet_config.h>
#include <ti/board/src/j742s2_evm/include/board_serdes_cfg.h>

#include <ti/csl/arch/csl_arch.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Initialize QSGMII board.
 *
 * Set GPIOs to release QSGMII PHY out of reset and release COMA_MODE pin. */
#define ENETBOARD_J7XEVM_QPENET_INIT                 ENET_BIT(0U)

/* Enable SERDES2 lane 2 for ENET Board usage */
#define ENETBOARD_J7XEVM_SERDES_ENET1_CFG            ENET_BIT(1U)

/* Enable SERDES2 lane 3 for ENET2 Board usage */
#define ENETBOARD_J7XEVM_SERDES_ENET2_CFG            ENET_BIT(2U)

/* VSC8514 wait time between NRESET deassert and access of the SMI interface */
#define ENETBOARD_QSGMII_PHY_TWAIT_USECS             (105000U)

/* SerDes 2 lane used for ENET1 expansion port */
#define ENETBOARD_SERDES_ENET1_LANE_NUM              (2U)

/* SerDes 2 lane used for ENET2 expansion port */
#define ENETBOARD_SERDES_ENET2_LANE_NUM              (3U)

/* SerDes lane function select: IP1 (0) alternate Enet Switch Q/SGMII Lanes */
#define ENETBOARD_SERDES_LANE_FUNC_SEL_IP1           (0U)

/* SerDes2 core_ref_clk rate needed for SGMII (100MHz) */
#define ENETBOARD_SERDES2_CORE_REF_CLK_RATE_HZ       (100000000LLU)

/* SerDes2 core_ref1_clk rate needed for XAUI/USXGMII (156.25MHz) */
#define ENETBOARD_SERDES2_CORE_REF1_CLK_RATE_HZ      (156250000LLU)

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
    ENETBOARD_SERDES_MODE_USXGMII_5G,
    ENETBOARD_SERDES_MODE_USXGMII_10G,
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

static void EnetBoard_configSerdesClks(EnetBoard_SerdesMode serdesMode);

static void EnetBoard_disableSerdesClks(void);

static Board_STATUS EnetBoard_serdesCfg(uint32_t laneNum,
                                        EnetBoard_SerdesMode serdesMode);

static uint32_t EnetBoard_getMacAddrListEeprom(uint8_t macAddr[][ENET_MAC_ADDR_LEN],
                                               uint32_t maxMacEntries);

static uint32_t EnetBoard_getMacAddrPoolStatic(uint8_t macAddr[][ENET_MAC_ADDR_LEN],
                                               uint32_t poolSize);

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
 * J742S2 CPB board configuration.
 *
 * 1 x RGMII PHY connected to J742S2 MCU CPSW_2G MAC port.
 * 1 x RGMII PHY connected to J742S2 Main CPSW_2G MAC port.
 */
static const EnetBoard_PortCfg gEnetCpbBoard_j7xEthPort[] =
{
    {   /* "MCU_RGMII1" */
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
    {   /* "RGMII1" */
        .enetType  = ENET_CPSW_2G,
        .instId    = 1U,
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
 * J742S2 QSGMII board (QpENet) configuration (ENET Board 1).
 *
 * All 4 x QSGMII ports are connected to J742S2 CPSW_9G MAC ports.
 */
static const EnetBoard_PortCfg gEnetQpENetBoard1_j742s2EthPort[] =
{
    {   /* "P0" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_1,
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
                      ENETBOARD_J7XEVM_SERDES_ENET1_CFG),
    },
    {   /* "P1" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_3,
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
                      ENETBOARD_J7XEVM_SERDES_ENET1_CFG),
    },
    {   /* "P2" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_4,
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
                      ENETBOARD_J7XEVM_SERDES_ENET1_CFG),
    },
    {   /* "P3" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_5,
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
                      ENETBOARD_J7XEVM_SERDES_ENET1_CFG),
    },
};

/*
 * J742S2 QSGMII board (QpENet) configuration (ENET Board 2).
 *
 * All 4 x QSGMII ports are connected to J742S2 CPSW_9G MAC ports.
 */
static const EnetBoard_PortCfg gEnetQpENetBoard2_j742s2EthPort[] =
{
    {   /* "P0" */
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
                      ENETBOARD_J7XEVM_SERDES_ENET2_CFG),
    },
    {   /* "P1" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_6,
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
                      ENETBOARD_J7XEVM_SERDES_ENET2_CFG),
    },
    {   /* "P2" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_7,
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
                      ENETBOARD_J7XEVM_SERDES_ENET2_CFG),
    },
    {   /* "P3" */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_8,
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
                      ENETBOARD_J7XEVM_SERDES_ENET2_CFG),
    },
};

/*
 * J742S2 SGMII board configuration.
 *
 * 1 x SGMII port connected to J742S2 CPSW_9G MAC port.
 */
static const EnetBoard_PortCfg gEnetSgmiiBoard_j742s2EthPort[] =
{
    {
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_1,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_SERIAL },
        .phyCfg    =
        {
            .phyAddr         = 10U,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = NULL,
            .extendedCfgSize = 0U,
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_WITH_PHY,
        .linkCfg   = { ENET_SPEED_AUTO, ENET_DUPLEX_AUTO },
        .flags     = (ENETBOARD_J7XEVM_QPENET_INIT |
                      ENETBOARD_J7XEVM_SERDES_ENET1_CFG),
    },
};

/* 1 x XAUI port in MAC-to-MAC mode using (SGMII) ENET bridge expansion board */
static const EnetBoard_PortCfg gEnetSgmiiBridgeBoard2_j742s2EthPort[] =
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
        .flags     = ENETBOARD_J7XEVM_SERDES_ENET2_CFG,
    },
};

/* 1 x XAUI port in MAC-to-MAC mode using (SGMII) ENET bridge expansion board */
static const EnetBoard_PortCfg gEnetXauiBridgeBoard2_j742s2EthPort[] =
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
        .flags     = ENETBOARD_J7XEVM_SERDES_ENET2_CFG,
    },
};

/* 1 x USXGMII port in MAC-to-MAC mode using (SGMII) ENET bridge expansion board */
static EnetBoard_PortCfg gEnetUsxgmiiBridgeBoard2_j742s2EthPort[] =
{
    {
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_2,
        .mii       = { ENET_MAC_LAYER_XGMII, ENET_MAC_SUBLAYER_SERIAL },
        .phyCfg    =
        {
            .phyAddr         = ENETPHY_INVALID_PHYADDR,
            .isStrapped      = BFALSE,
            .skipExtendedCfg = BFALSE,
            .extendedCfg     = NULL,
            .extendedCfgSize = 0U,
        },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_FORCEDLINK,
        .linkCfg   = { ENET_SPEED_10GBIT, ENET_DUPLEX_FULL },
        .flags     = ENETBOARD_J7XEVM_SERDES_ENET2_CFG,
    }
};

/*
 * J742S2 dummy board used for MAC loopback setup.
 */
static const EnetBoard_PortCfg gEnetLpbkBoard_j742s2EthPort[] =
{
    {   /* MCU CPSW_2G RGMII MAC loopback */
        .enetType  = ENET_CPSW_2G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_1,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg    = { .phyAddr = ENETPHY_INVALID_PHYADDR, },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_FORCEDLINK,
        .linkCfg   = { ENET_SPEED_1GBIT, ENET_DUPLEX_FULL },
        .flags     = 0U,
    },
    {   /* MCU CPSW_2G RMII MAC loopback */
        .enetType  = ENET_CPSW_2G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_1,
        .mii       = { ENET_MAC_LAYER_MII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg    = { .phyAddr = ENETPHY_INVALID_PHYADDR, },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_FORCEDLINK,
        .linkCfg   = { ENET_SPEED_100MBIT, ENET_DUPLEX_FULL },
        .flags     = 0U,
    },
    {   /* Main CPSW_2G RGMII MAC loopback */
        .enetType  = ENET_CPSW_2G,
        .instId    = 1U,
        .macPort   = ENET_MAC_PORT_1,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg    = { .phyAddr = ENETPHY_INVALID_PHYADDR, },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_FORCEDLINK,
        .linkCfg   = { ENET_SPEED_1GBIT, ENET_DUPLEX_FULL },
        .flags     = 0U,
    },
    {   /* Main CPSW_2G RMII MAC loopback */
        .enetType  = ENET_CPSW_2G,
        .instId    = 1U,
        .macPort   = ENET_MAC_PORT_1,
        .mii       = { ENET_MAC_LAYER_MII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg    = { .phyAddr = ENETPHY_INVALID_PHYADDR, },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_FORCEDLINK,
        .linkCfg   = { ENET_SPEED_100MBIT, ENET_DUPLEX_FULL },
        .flags     = 0U,
    },
    {   /* CPSW_9G SGMII MAC loopback */
        .enetType  = ENET_CPSW_9G,
        .instId    = 0U,
        .macPort   = ENET_MAC_PORT_1,
        .mii       = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_SERIAL },
        .phyCfg    = { .phyAddr = ENETPHY_INVALID_PHYADDR, },
        .sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_FORCEDLINK,
        .linkCfg   = { ENET_SPEED_1GBIT, ENET_DUPLEX_FULL },
        .flags     = ENETBOARD_J7XEVM_SERDES_ENET1_CFG,
    },
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static bool EnetBoard_detectEnetCard(uint32_t *boardId,
                                     int32_t *enetCard)
{
    uint32_t board;
    int32_t card;
    bool detected = BFALSE;

    /* Try in ENET Board first */
    board = BOARD_ID_ENET;
    card = Board_detectEnetCard(board);
    detected = ((card != BOARD_ENET_NONE) && (card != BOARD_ENET_UNKOWN));

    /* If not detected, then try in ENET2 Board next */
    if (!detected)
    {
        board = BOARD_ID_ENET2;
        card = Board_detectEnetCard(board);
        detected = ((card != BOARD_ENET_NONE) && (card != BOARD_ENET_UNKOWN));
    }

    if (detected)
    {
        *boardId = board;
        *enetCard = card;
    }
    else
    {
        *enetCard = BOARD_ENET_NONE;
    }

    return detected;
}

static void EnetBoard_enetExpCfg(uint32_t boardId,
                                 int32_t enetCard)
{
    Board_initParams_t prms;
    Board_STATUS boardStatus;

    Board_getInitParams(&prms);
    prms.enetBoardID = boardId;
    Board_setInitParams(&prms);

    /* Release PHY reset. Note this is needed for both SGMII and QSGMII boards */
    boardStatus = Board_cpswEnetExpPhyReset(0U);
    EnetAppUtils_assert(BOARD_SOK == boardStatus);

    if (enetCard == BOARD_ENET_QSGMII)
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
        (ethPort->boardId == ENETBOARD_QPENET_ID) &&
        (ethPort->expPort == ENETBOARD_EXP_PORT_ENET1))
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetQpENetBoard1_j742s2EthPort,
                                        ENETPHY_ARRAYSIZE(gEnetQpENetBoard1_j742s2EthPort));
    }

    if ((portCfg == NULL) &&
        (ethPort->boardId == ENETBOARD_QPENET_ID) &&
        (ethPort->expPort == ENETBOARD_EXP_PORT_ENET2))
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetQpENetBoard2_j742s2EthPort,
                                        ENETPHY_ARRAYSIZE(gEnetQpENetBoard2_j742s2EthPort));
    }

    if ((portCfg == NULL) &&
        (ethPort->boardId == ENETBOARD_SGMII_ID) &&
        (ethPort->expPort == ENETBOARD_EXP_PORT_ENET1))
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetSgmiiBoard_j742s2EthPort,
                                        ENETPHY_ARRAYSIZE(gEnetSgmiiBoard_j742s2EthPort));
    }

    if ((portCfg == NULL) &&
        (ethPort->boardId == ENETBOARD_BRIDGE_SGMII_ID) &&
        (ethPort->expPort == ENETBOARD_EXP_PORT_ENET2))
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetSgmiiBridgeBoard2_j742s2EthPort,
                                        ENETPHY_ARRAYSIZE(gEnetSgmiiBridgeBoard2_j742s2EthPort));
    }

    if ((portCfg == NULL) &&
        (ethPort->boardId == ENETBOARD_BRIDGE_XAUI_ID) &&
        (ethPort->expPort == ENETBOARD_EXP_PORT_ENET2))
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetXauiBridgeBoard2_j742s2EthPort,
                                        ENETPHY_ARRAYSIZE(gEnetXauiBridgeBoard2_j742s2EthPort));
    }

    if ((portCfg == NULL) &&
        ((ethPort->boardId == ENETBOARD_BRIDGE_USXGMII_5G_ID) ||
         (ethPort->boardId == ENETBOARD_BRIDGE_USXGMII_10G_ID)) &&
        (ethPort->expPort == ENETBOARD_EXP_PORT_ENET2))
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetUsxgmiiBridgeBoard2_j742s2EthPort,
                                        ENETPHY_ARRAYSIZE(gEnetUsxgmiiBridgeBoard2_j742s2EthPort));
    }

    if ((portCfg == NULL) &&
        (ethPort->boardId == ENETBOARD_LOOPBACK_ID))
    {
        portCfg = EnetBoard_findPortCfg(enetType, instId, ethPort,
                                        gEnetLpbkBoard_j742s2EthPort,
                                        ENETPHY_ARRAYSIZE(gEnetLpbkBoard_j742s2EthPort));
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
    UART_HwAttrs uart_cfg;
    uint32_t uartInst;
    uint32_t uartBaseAddr;
    uint32_t socDomainUART;
    uint32_t socDomainCore;

#ifdef BUILD_MCU
    CSL_ArmR5CPUInfo info = {0U, 0U, 0U};

    CSL_armR5GetCpuID(&info);
    if (info.grpId != (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        /* Main R5 cores will use main domain UART instances */
        socDomainUART = BOARD_SOC_DOMAIN_MAIN;
        uartInst      = gBoardInitParams.uartInst;
    }
    else
    {
        socDomainUART = gBoardInitParams.uartSocDomain;
        if(socDomainUART == BOARD_SOC_DOMAIN_MCU)
        {
            uartInst = BOARD_MCU_UART_INSTANCE;
        }
        else
        {
            uartInst = gBoardInitParams.uartInst;
        }
    }
#else
    socDomainUART = gBoardInitParams.uartSocDomain;
    uartInst      = gBoardInitParams.uartInst;
#endif

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
    uint32_t boardId = 0U;
    int32_t enetCard = BOARD_ENET_NONE;

    EnetBoard_detectEnetCard(&boardId, &enetCard);
    if ((enetCard == BOARD_ENET_QSGMII) ||
        (enetCard == BOARD_ENET_SGMII))
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
    uint32_t hits = 0U;
    uint32_t flags = 0U;
    uint32_t i;
    uint32_t boardId = BOARD_ID_ENET;
    int32_t enetCard = BOARD_ENET_NONE;
    uint32_t boardEnet1 = 0U;
    uint32_t boardEnet2 = 0U;
    uint32_t laneNum;
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

            if (ethPort->expPort == ENETBOARD_EXP_PORT_ENET1)
            {
                boardEnet1 = ethPort->boardId;
            }

            if (ethPort->expPort == ENETBOARD_EXP_PORT_ENET2)
            {
                boardEnet2 = ethPort->boardId;
            }

            hits++;
        }
    }

    EnetAppUtils_print("EnetBoard_setupPorts: %u of %u ports configurations found\n", hits, numEthPorts);

    /* Perform PDK Board level initialization */
    if (status == ENET_SOK)
    {
        boardStatus = Board_init(BOARD_INIT_PINMUX_CONFIG);
        EnetAppUtils_assert(boardStatus == BOARD_SOK);
        status = boardStatus;
    }

    /* Check if QpENet board is detected if any QSMII related configuration is required */
    if ((status == ENET_SOK) &&
        (ENET_NOT_ZERO(flags & ENETBOARD_J7XEVM_QPENET_INIT)))
    {
        EnetBoard_detectEnetCard(&boardId, &enetCard);
        if ((enetCard == BOARD_ENET_QSGMII) ||
            (enetCard == BOARD_ENET_SGMII))
        {
            boardStatus = Board_control(BOARD_CTRL_CMD_SET_IO_MUX_PORTB2, NULL);
            EnetAppUtils_assert(boardStatus == BOARD_SOK);

            EnetBoard_enetExpCfg(boardId, enetCard);
        }
        else
        {
            EnetAppUtils_print("EnetBoard_setupPorts: QpENet (QSGMII) board not detected\n");
            status = ENET_EFAIL;
        }
    }

    /* SERDES Torrent configuration and clocks setup */
    if (ENET_NOT_ZERO(flags & ENETBOARD_J7XEVM_SERDES_ENET1_CFG) ||
        ENET_NOT_ZERO(flags & ENETBOARD_J7XEVM_SERDES_ENET2_CFG))
    {
        if (ENET_NOT_ZERO(flags & ENETBOARD_J7XEVM_SERDES_ENET1_CFG))
        {
            laneNum = ENETBOARD_SERDES_ENET1_LANE_NUM;
            serdesMode = EnetBoard_getSerdesMode(boardEnet1);
        }
        else
        {
            laneNum = ENETBOARD_SERDES_ENET2_LANE_NUM;
            serdesMode = EnetBoard_getSerdesMode(boardEnet2);
        }

        Board_unlockMMR();
        EnetBoard_configSerdesClks(serdesMode);

        boardStatus = EnetBoard_serdesCfg(laneNum, serdesMode);
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
        case ENETBOARD_BRIDGE_USXGMII_5G_ID:
            serdesMode = ENETBOARD_SERDES_MODE_USXGMII_5G;
            break;
        case ENETBOARD_BRIDGE_USXGMII_10G_ID:
            serdesMode = ENETBOARD_SERDES_MODE_USXGMII_10G;
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

static void EnetBoard_configSerdesClks(EnetBoard_SerdesMode serdesMode)
{
    uint32_t moduleId = TISCI_DEV_SERDES_10G1;
    uint32_t coreRefClkId = TISCI_DEV_SERDES_10G1_CORE_REF_CLK;
    uint32_t coreRefClkPar = 0U;
    uint64_t coreRefClkHz = 0ULL;
    int32_t status;

    switch (serdesMode)
    {
        case ENETBOARD_SERDES_MODE_SGMII:
        case ENETBOARD_SERDES_MODE_QSGMII:
            coreRefClkHz  = 100000000;
            coreRefClkPar = TISCI_DEV_SERDES_10G1_CORE_REF_CLK_PARENT_HSDIV4_16FFT_MAIN_2_HSDIVOUT4_CLK;
            break;

        case ENETBOARD_SERDES_MODE_XAUI:
        case ENETBOARD_SERDES_MODE_USXGMII_5G:
        case ENETBOARD_SERDES_MODE_USXGMII_10G:
            coreRefClkHz  = 156250000;
            coreRefClkPar = TISCI_DEV_SERDES_10G1_CORE_REF_CLK_PARENT_HSDIV4_16FFT_MAIN_3_HSDIVOUT4_CLK;
            break;

        default:
            EnetAppUtils_print("Invalid SerDes mode %d\n", serdesMode);
            EnetAppUtils_assert(BFALSE);
            break;
    }

    /* Ensure the parent clock is at the desired frequency */
    EnetAppUtils_clkRateSet(moduleId, coreRefClkPar, coreRefClkHz);

    /* Reparent to MAIN_PLL3_HSDIV4 or MAIN_PLL2_HSDIV4 depending on the req ref clock frequency */
    status = Sciclient_pmSetModuleClkParent(moduleId, coreRefClkId, coreRefClkPar, SCICLIENT_SERVICE_WAIT_FOREVER);
    if (status != CSL_PASS)
    {
        EnetAppUtils_print("Failed to reparent core_ref_clk: %d\n", status);
        EnetAppUtils_assert(BFALSE);
    }

    /* Enable SerDes2 module */
    EnetAppUtils_setDeviceState(moduleId, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON, 0U);
}

static void EnetBoard_disableSerdesClks(void)
{
    uint32_t moduleId = TISCI_DEV_SERDES_10G1;

    /* Set module set to HW AUTO */
    EnetAppUtils_setDeviceState(moduleId, TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF, 0U);
}

static Board_STATUS EnetBoard_serdesCfg(uint32_t laneNum,
                                        EnetBoard_SerdesMode serdesMode)
{
    CSL_SerdesLaneEnableStatus laneStatus = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SerdesLaneEnableParams lanePrms;
    CSL_SerdesResult result;
    CSL_SerdesStatus status = BOARD_SOK;

    memset(&lanePrms, 0, sizeof(lanePrms));

    lanePrms.serdesInstance    = (CSL_SerdesInstance)CSL_TORRENT_SERDES1;
    lanePrms.baseAddr          = CSL_WIZ16B8M4CT3_1_WIZ16B8M4CT3_BASE;
    lanePrms.numLanes          = 1U;
    lanePrms.laneMask          = ENET_BIT(laneNum);
    lanePrms.SSC_mode          = CSL_SERDES_NO_SSC;
    lanePrms.operatingMode     = CSL_SERDES_FUNCTIONAL_MODE;
    lanePrms.phyInstanceNum    = ENETBOARD_SERDES_LANE_FUNC_SEL_IP1;

    lanePrms.laneCtrlRate[laneNum] = CSL_SERDES_LANE_FULL_RATE;
    lanePrms.loopbackMode[laneNum] = CSL_SERDES_LOOPBACK_DISABLED;

    switch (serdesMode)
    {
        case ENETBOARD_SERDES_MODE_SGMII:
            EnetAppUtils_print("Configuring SerDes lane %u for SGMII\n", laneNum);
            lanePrms.phyType       = CSL_SERDES_PHY_TYPE_SGMII;
            lanePrms.linkRate      = CSL_SERDES_LINK_RATE_1p25G;
            lanePrms.refClock      = CSL_SERDES_REF_CLOCK_100M;
            lanePrms.refClkSrc     = CSL_SERDES_REF_CLOCK_INT0;
            break;

        case ENETBOARD_SERDES_MODE_QSGMII:
            EnetAppUtils_print("Configuring SerDes lane %u for QSGMII\n", laneNum);
            lanePrms.phyType       = CSL_SERDES_PHY_TYPE_QSGMII;
            lanePrms.linkRate      = CSL_SERDES_LINK_RATE_5G;
            lanePrms.refClock      = CSL_SERDES_REF_CLOCK_100M;
            lanePrms.refClkSrc     = CSL_SERDES_REF_CLOCK_INT0;
            break;

        case ENETBOARD_SERDES_MODE_XAUI:
            EnetAppUtils_print("Configuring SerDes lane %u for XAUI\n", laneNum);
            lanePrms.phyType       = CSL_SERDES_PHY_TYPE_XAUI;
            lanePrms.linkRate      = CSL_SERDES_LINK_RATE_3p125G;
            lanePrms.refClock      = CSL_SERDES_REF_CLOCK_156p25M;
            lanePrms.refClkSrc     = CSL_SERDES_REF_CLOCK_INT0;
            break;

        case ENETBOARD_SERDES_MODE_USXGMII_5G:
            EnetAppUtils_print("Configuring SerDes lane %u for USXGMII (5Gbps)\n", laneNum);
            lanePrms.phyType       = CSL_SERDES_PHY_TYPE_USXGMII;
            lanePrms.linkRate      = CSL_SERDES_LINK_RATE_5p15625G;
            lanePrms.refClock      = CSL_SERDES_REF_CLOCK_156p25M;
            lanePrms.refClkSrc     = CSL_SERDES_REF_CLOCK_INT0;
            break;

        case ENETBOARD_SERDES_MODE_USXGMII_10G:
            EnetAppUtils_print("Configuring SerDes lane %u for USXGMII (10Gbps)\n", laneNum);
            lanePrms.phyType       = CSL_SERDES_PHY_TYPE_USXGMII;
            lanePrms.linkRate      = CSL_SERDES_LINK_RATE_10p3125G;
            lanePrms.refClock      = CSL_SERDES_REF_CLOCK_156p25M;
            lanePrms.refClkSrc     = CSL_SERDES_REF_CLOCK_INT0;
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
                           laneNum);
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
    else if (EnetMacPort_isXfi(mii))
    {
        modeSel = XFI;
    }
    else
    {
        EnetAppUtils_print("Invalid MII type: layer %u suyblayer %u\n", mii->layerType, mii->sublayerType);
        EnetAppUtils_assert(BFALSE);
    }

    switch (enetType)
    {
        case ENET_CPSW_2G:
            if (instId == 0U)
            {
                Board_unlockMMR();
                status = Board_cpsw2gMacModeConfig(modeSel);
            }
            else
            {
                status = Board_cpsw2gMainMacModeConfig(modeSel);
            }
            break;
        case ENET_CPSW_9G:
            status = Board_cpsw9gMacModeConfig(portNum, modeSel);
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
    Board_STATUS boardStatus;

    if (maxMacEntries < 1U)
    {
        EnetAppUtils_print("Invalid number of requested MAC addresses\n");
        EnetAppUtils_assert(BFALSE);
    }

    switch (enetType)
    {
        case ENET_CPSW_2G:
            if (instId == 0U)
            {
                EnetSoc_getEFusedMacAddrs(macAddr, &numMacAddrs);
            }
            else if (instId == 1U)
            {
                boardStatus = Board_readMacAddr(BOARD_ID_EVM, &macAddr[0][0], ENET_MAC_ADDR_LEN, &numMacAddrs);
                if (boardStatus != BOARD_SOK)
                {
                    EnetAppUtils_print("Failed to read Main CPSW2G MAC IDs from EEPROM: %d\n", boardStatus);
                    numMacAddrs = 0U;
                }
            }
            break;

        case ENET_CPSW_9G:
            numMacAddrs = EnetBoard_getMacAddrListEeprom(macAddr, maxMacEntries);
            break;

        default:
            EnetAppUtils_print("Invalid peripheral type or inst\n");
            EnetAppUtils_assert(BFALSE);
            break;
    }

    /* Fallback to static address if MAC ID is not programmed yet */
    if (EnetUtils_isMcastAddr(&macAddr[0][0]))
    {
        numMacAddrs = 0U;
    }

    if (numMacAddrs < maxMacEntries)
    {
        EnetAppUtils_print("No valid MAC address found in EEPROM, falling back to static address\n");
        numMacAddrs += EnetBoard_getMacAddrPoolStatic(&macAddr[numMacAddrs], maxMacEntries - numMacAddrs);
    }

    return numMacAddrs;
}

static uint32_t EnetBoard_getMacAddrListEeprom(uint8_t macAddr[][ENET_MAC_ADDR_LEN],
                                               uint32_t maxMacEntries)
{
    uint8_t macAddrBuf[ENET_RM_NUM_MACADDR_MAX * BOARD_MAC_ADDR_BYTES];
    Board_STATUS boardStatus;
    uint32_t boardId = 0U;
    int32_t enetCard;
    uint32_t macAddrCnt, tempCnt;
    uint32_t allocMacEntries = 0;
    uint32_t i;
    bool detected;

    detected = EnetBoard_detectEnetCard(&boardId, &enetCard);
    if (detected)
    {
        /* Read number of MAC addresses in QUAD Eth board */
        boardStatus = Board_readMacAddrCount(boardId, &macAddrCnt);
        EnetAppUtils_assert(boardStatus == BOARD_SOK);
        EnetAppUtils_assert(macAddrCnt <= ENET_RM_NUM_MACADDR_MAX);

        /* Read MAC addresses */
        boardStatus = Board_readMacAddr(boardId,
                                        macAddrBuf,
                                        sizeof(macAddrBuf),
                                        &tempCnt);
        EnetAppUtils_assert(boardStatus == BOARD_SOK);
        EnetAppUtils_assert(tempCnt == macAddrCnt);

        /* Save only those required to meet the max number of MAC entries */
        allocMacEntries = EnetUtils_min(macAddrCnt, maxMacEntries);
        for (i = 0U; i < allocMacEntries; i++)
        {
            ENET_UTILS_COMPILETIME_ASSERT(ENET_MAC_ADDR_LEN == BOARD_MAC_ADDR_BYTES);
            memcpy(macAddr[i], &macAddrBuf[i * BOARD_MAC_ADDR_BYTES], ENET_MAC_ADDR_LEN);
        }

        if (allocMacEntries == 0U)
        {
            EnetAppUtils_print("No MAC addresses read from QENET board\n");
            EnetAppUtils_assert(BFALSE);
        }
    }

    return allocMacEntries;
}

static uint32_t EnetBoard_getMacAddrPoolStatic(uint8_t macAddr[][ENET_MAC_ADDR_LEN],
                                               uint32_t poolSize)
{
    uint32_t allocCnt = 0U;
    uint8_t macAddrBuf[][ENET_MAC_ADDR_LEN] =
    {
        { 0x70U, 0xFFU, 0x76U, 0x1DU, 0x92U, 0xC1U },
        { 0x70U, 0xFFU, 0x76U, 0x1DU, 0x92U, 0xC2U },
        { 0x70U, 0xFFU, 0x76U, 0x1DU, 0x92U, 0xC3U },
        { 0x70U, 0xFFU, 0x76U, 0x1DU, 0x92U, 0xC4U },
        { 0x70U, 0xFFU, 0x76U, 0x1DU, 0x92U, 0xC5U },
        { 0x70U, 0xFFU, 0x76U, 0x1DU, 0X8BU, 0xC4U },
        { 0x70U, 0xFFU, 0x76U, 0x1DU, 0X8BU, 0xC5U },
        { 0x70U, 0xFFU, 0x76U, 0x1DU, 0X8BU, 0xC6U },
        { 0x70U, 0xFFU, 0x76U, 0x1DU, 0X8BU, 0xC7U },
    };
    uint32_t macAddrCnt = ENET_ARRAYSIZE(macAddrBuf);

    /* Save only those required to meet the max number of MAC entries */
    allocCnt = EnetUtils_min(macAddrCnt, poolSize);
    memcpy(&macAddr[0U][0U], &macAddrBuf[0U][0U], allocCnt * ENET_MAC_ADDR_LEN);

    return allocCnt;
}
