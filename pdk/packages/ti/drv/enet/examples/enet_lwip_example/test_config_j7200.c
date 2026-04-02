/*
 *  Copyright (c) Texas Instruments Incorporated 2022
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
 * \file  test_config_j7200.c
 *
 * \brief This file contains J7200-specific test configuration parameters.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <ti/board/board.h>
#include <ti/board/src/j7200_evm/include/board_ethernet_config.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_board.h>

#include "test_config.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*
 * Enable selection of test parameters:
 *   0 - dynamic (runtime via menu)
 *   1 - static (compile time)
 */
#define APP_ENABLE_STATIC_CFG                      (0U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
/* MCU or Main CPSW2G RGMII port in baseboard */
static EnetBoard_EthPort gEnetApp_RgmiiBaseBoard[] =
{
    {
        .macPort = ENET_MAC_PORT_1,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .boardId = ENETBOARD_CPB_ID,
        .expPort = ENETBOARD_EXP_PORT_NONE,
    },
};
#endif

#if defined(BUILD_MCU2_0)
/* QpENet board in ENET expander port */
static EnetBoard_EthPort gEnetApp_QpENetBoardEnet[] =
{
    {
        .macPort = ENET_MAC_PORT_1,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_MAIN },
        .boardId = ENETBOARD_QPENET_ID,
        .expPort = ENETBOARD_EXP_PORT_ENET,
    },
    {
        .macPort = ENET_MAC_PORT_2,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
        .boardId = ENETBOARD_QPENET_ID,
        .expPort = ENETBOARD_EXP_PORT_ENET,
    },
    {
        .macPort = ENET_MAC_PORT_3,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
        .boardId = ENETBOARD_QPENET_ID,
        .expPort = ENETBOARD_EXP_PORT_ENET,
    },
    {
        .macPort = ENET_MAC_PORT_4,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
        .boardId = ENETBOARD_QPENET_ID,
        .expPort = ENETBOARD_EXP_PORT_ENET,
    },
};

/* SGMII board (DP83869) in ENET expander port */
static EnetBoard_EthPort gEnetApp_SgmiiBoardEnet[] =
{
    {
        .macPort = ENET_MAC_PORT_1,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_SERIAL },
        .boardId = ENETBOARD_SGMII_ID,
        .expPort = ENETBOARD_EXP_PORT_ENET,
    },
};

/* RGMII port in GESI board */
static EnetBoard_EthPort gEnetApp_GesiBoard[] =
{
    {
        .macPort = ENET_MAC_PORT_2,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .boardId = ENETBOARD_GESI_ID,
        .expPort = ENETBOARD_EXP_PORT_GESI,
    },
};

/* SGMII bridge board in SGMII mode in ENET expander port + GESI board */
static EnetBoard_EthPort gEnetApp_SgmiiBridgeEnet[] =
{
    {
        .macPort = ENET_MAC_PORT_1,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_SERIAL },
        .boardId = ENETBOARD_BRIDGE_SGMII_ID,
        .expPort = ENETBOARD_EXP_PORT_ENET,
    },
    {
        .macPort = ENET_MAC_PORT_2,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .boardId = ENETBOARD_GESI_ID,
        .expPort = ENETBOARD_EXP_PORT_GESI,
    },
};

/* SGMII bridge board in XAUI mode in ENET expander port + GESI board */
static EnetBoard_EthPort gEnetApp_XauiBridgeEnet[] =
{
    {
        .macPort = ENET_MAC_PORT_1,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_SERIAL },
        .boardId = ENETBOARD_BRIDGE_XAUI_ID,
        .expPort = ENETBOARD_EXP_PORT_ENET,
    },
    {
        .macPort = ENET_MAC_PORT_2,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .boardId = ENETBOARD_GESI_ID,
        .expPort = ENETBOARD_EXP_PORT_GESI,
    },
};

/* SGMII bridge board in USXGMII 5Gbps mode in ENET expander port + GESI board */
static EnetBoard_EthPort gEnetApp_Usxgmii5gBridgeEnet[] =
{
    {
        .macPort = ENET_MAC_PORT_1,
        .mii     = { ENET_MAC_LAYER_XGMII, ENET_MAC_SUBLAYER_SERIAL },
        .boardId = ENETBOARD_BRIDGE_USXGMII_5G_ID,
        .expPort = ENETBOARD_EXP_PORT_ENET,
    },
    {
        .macPort = ENET_MAC_PORT_2,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .boardId = ENETBOARD_GESI_ID,
        .expPort = ENETBOARD_EXP_PORT_GESI,
    },
};

/* SGMII bridge board in USXGMII 10Gbps mode in ENET expander port + GESI board */
static EnetBoard_EthPort gEnetApp_Usxgmii10gBridgeEnet[] =
{
    {
        .macPort = ENET_MAC_PORT_1,
        .mii     = { ENET_MAC_LAYER_XGMII, ENET_MAC_SUBLAYER_SERIAL },
        .boardId = ENETBOARD_BRIDGE_USXGMII_10G_ID,
        .expPort = ENETBOARD_EXP_PORT_ENET,
    },
    {
        .macPort = ENET_MAC_PORT_2,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .boardId = ENETBOARD_GESI_ID,
        .expPort = ENETBOARD_EXP_PORT_GESI,
    },
};
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#if (0U == APP_ENABLE_STATIC_CFG)
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
static void EnetApp_getTestMacPort(EnetBoard_EthPort *ethPorts,
                                   uint32_t *numEthPorts)
{
    uint32_t choice = 0U;
    bool retry;

    do
    {
        EnetAppUtils_print("Select MAC port\n");
        EnetAppUtils_print("0: ENET_MAC_PORT_1 - RGMII\n");
        retry = BFALSE;
        choice = EnetAppUtils_getNum();
        switch (choice)
        {
            case 0:
                memcpy(ethPorts, gEnetApp_RgmiiBaseBoard, sizeof(gEnetApp_RgmiiBaseBoard));
                *numEthPorts = ENET_ARRAYSIZE(gEnetApp_RgmiiBaseBoard);
                break;

            default:
                EnetAppUtils_print("Wrong MAC port, enter again\n");
                retry = BTRUE;
                break;
        }
    }
    while (retry);
}
#elif defined(BUILD_MCU2_0)
static void EnetApp_getTestMacPort(EnetBoard_EthPort *ethPorts,
                                   uint32_t *numEthPorts)
{
    uint32_t choice = 0U;
    int32_t enetCard;
    bool retry;

    do
    {
        EnetAppUtils_print("Select MAC port\n");
        EnetAppUtils_print("0: ENET_MAC_PORT_1 - Q/SGMII (based on which board is attached)\n");
        EnetAppUtils_print("1: ENET_MAC_PORT_2 - RGMII (GESI)\n");
        EnetAppUtils_print("2: ENET_MAC_PORT_1 & 2 - Bridge SGMII + RGMII\n");
        EnetAppUtils_print("3: ENET_MAC_PORT_1 & 2 - Bridge XAUI + RGMII\n");
        EnetAppUtils_print("4: ENET_MAC_PORT_1 & 2 - Bridge USXGMII 5Gbps + RGMII\n");
        EnetAppUtils_print("5: ENET_MAC_PORT_1 & 2 - Bridge USXGMII 10Gbps + RGMII\n");

        retry = BFALSE;
        choice = EnetAppUtils_getNum();
        switch (choice)
        {
            case 0:
                enetCard = Board_detectEnetCard();
                if (enetCard == BOARD_ENET_QSGMII)
                {
                    memcpy(ethPorts, gEnetApp_QpENetBoardEnet, sizeof(gEnetApp_QpENetBoardEnet));
                    *numEthPorts = ENET_ARRAYSIZE(gEnetApp_QpENetBoardEnet);
                }
                else if (enetCard == BOARD_ENET_SGMII)
                {
                    memcpy(ethPorts, gEnetApp_SgmiiBoardEnet, sizeof(gEnetApp_SgmiiBoardEnet));
                    *numEthPorts = ENET_ARRAYSIZE(gEnetApp_SgmiiBoardEnet);
                }
                else
                {
                    EnetAppUtils_print("SGMII/QSGMII DB not detected, enter again\n");
                    retry = BTRUE;
                }
                break;

            case 1:
                memcpy(ethPorts, gEnetApp_GesiBoard, sizeof(gEnetApp_GesiBoard));
                *numEthPorts = ENET_ARRAYSIZE(gEnetApp_GesiBoard);
                break;

            case 2:
                memcpy(ethPorts, gEnetApp_SgmiiBridgeEnet, sizeof(gEnetApp_SgmiiBridgeEnet));
                *numEthPorts = ENET_ARRAYSIZE(gEnetApp_SgmiiBridgeEnet);
                break;

            case 3:
                memcpy(ethPorts, gEnetApp_XauiBridgeEnet, sizeof(gEnetApp_XauiBridgeEnet));
                *numEthPorts = ENET_ARRAYSIZE(gEnetApp_XauiBridgeEnet);
                break;

            case 4:
                memcpy(ethPorts, gEnetApp_Usxgmii5gBridgeEnet, sizeof(gEnetApp_Usxgmii5gBridgeEnet));
                *numEthPorts = ENET_ARRAYSIZE(gEnetApp_Usxgmii5gBridgeEnet);
                break;

            case 5:
                memcpy(ethPorts, gEnetApp_Usxgmii10gBridgeEnet, sizeof(gEnetApp_Usxgmii10gBridgeEnet));
                *numEthPorts = ENET_ARRAYSIZE(gEnetApp_Usxgmii10gBridgeEnet);
                break;

            default:
                EnetAppUtils_print("Wrong MAC port, enter again\n");
                retry = BTRUE;
                break;
        }
    }
    while (retry);
}
#endif

void EnetApp_getTestConfig(Enet_Type *enetType,
                           uint32_t *instId,
                           EnetBoard_EthPort *ethPorts,
                           uint32_t *numEthPorts)
{
    uint32_t choice = 0U;
    bool retry;
    static const char enetTypeSetting[] =
    {
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
        "0: CPSW_2G\n"
#elif defined(BUILD_MCU2_0)
        "1: CPSW_5G\n"
#endif
    };

    do
    {
        EnetAppUtils_print("Select peripheral type\n");
        EnetAppUtils_print("%s\n", enetTypeSetting);
        retry = BFALSE;

        choice = EnetAppUtils_getNum();
        switch (choice)
        {
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
            case 0:
                *enetType = ENET_CPSW_2G;
                *instId   = 0U;
                EnetApp_getTestMacPort(ethPorts, numEthPorts);
                break;
#elif defined(BUILD_MCU2_0)
            case 1:
                *enetType = ENET_CPSW_5G;
                *instId   = 0U;
                EnetApp_getTestMacPort(ethPorts, numEthPorts);
                break;
#endif
           default:
                EnetAppUtils_print("Wrong peripheral type, enter again\n");
                retry = BTRUE;
                break;
        }
    }
    while (retry);
}
#else /* (1U == APP_ENABLE_STATIC_CFG) */
void EnetApp_getTestConfig(Enet_Type *enetType,
                           uint32_t *instId,
                           EnetBoard_EthPort *ethPorts,
                           uint32_t *numEthPorts)
{
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
    *enetType = ENET_CPSW_2G;
    *instId   = 0U;
    memcpy(ethPorts, gEnetApp_RgmiiBaseBoard, sizeof(gEnetApp_RgmiiBaseBoard));
    *numEthPorts = ENET_ARRAYSIZE(gEnetApp_RgmiiBaseBoard);
#elif defined(BUILD_MCU2_0)
    *enetType = ENET_CPSW_5G;
    *instId   = 0U;
    ethPorts[0U].macPort          = ENET_MAC_PORT_1;
    ethPorts[0U].boardId          = ENETBOARD_QPENET_ID;
    ethPorts[0U].expPort          = ENETBOARD_EXP_PORT_ENET;
    ethPorts[0U].mii.layerType    = ENET_MAC_LAYER_GMII;
    ethPorts[0U].mii.sublayerType = ENET_MAC_SUBLAYER_QUAD_SERIAL_MAIN;
    *numEthPorts = 1U;
#else
#error "lwIP example is not supported on this core"
#endif
}
#endif /* (0U == APP_ENABLE_STATIC_CFG) */
